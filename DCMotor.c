/********************************************************************************
* Name: DCMotor.h (implementation)
* Author(s): Noah Grant, Wyatt Richard
* Date: March 24, 2023
* Description: DC motor functions.
********************************************************************************/

#include "DCMotor.h"
#include "Utility.h"
#include "stm32f303xe.h"

// Drive Motor Configuration Parameters
// - Motor Speed Control Pins:
//    Left Motor		PC10
//    Right Motor   PC11
// - Motor Direction Control Pins:
//    Left Motor Forward 	(A)		PC12
//    Left Motor Reverse 	(B)   PC13
//    Right Motor Forward (A)   PC8
//    Right Motor Reverse (B)   PC9
//
// - Direction Control Truth Table
//     STOP   FWD	RWD   UNDEFINED
// (A)  0      1     0      1     
// (B)  0      0     1      1

void DCMotor_Init(void){
	// Enable GPIO Clock
	ENABLE_GPIO_CLOCK(C);
	
	// Configure Motor Control Pins 
	//  For Direction Control, just ordinary GPIO configurations
	//    MODE = Output
	//    Output Type = Push Pull
	//    Pull-up / Pull-down = No Pull
	//    Initial Output Value should be set to 0 (STOP by default)
	GPIO_MODER_SET(C, 8, GPIO_MODE_OUT);
	GPIO_MODER_SET(C, 9, GPIO_MODE_OUT);
	GPIO_MODER_SET(C, 12, GPIO_MODE_OUT);
	GPIO_MODER_SET(C, 13, GPIO_MODE_OUT);
	
	GPIO_OTYPER_SET(C, 8, GPIO_OTYPE_PP);
	GPIO_OTYPER_SET(C, 9, GPIO_OTYPE_PP);
	GPIO_OTYPER_SET(C, 12, GPIO_OTYPE_PP);
	GPIO_OTYPER_SET(C, 13, GPIO_OTYPE_PP);
	
	GPIO_PUPDR_SET(C, 8, GPIO_PUPD_NO);
	GPIO_PUPDR_SET(C, 9, GPIO_PUPD_NO);
	GPIO_PUPDR_SET(C, 12, GPIO_PUPD_NO);
	GPIO_PUPDR_SET(C, 13, GPIO_PUPD_NO);
	
	CLEAR_BITS(GPIOC->ODR, GPIO_ODR_8);
	CLEAR_BITS(GPIOC->ODR, GPIO_ODR_9);
	CLEAR_BITS(GPIOC->ODR, GPIO_ODR_12);
	CLEAR_BITS(GPIOC->ODR, GPIO_ODR_13);
	
	//  For Speed Control, connect TIM8 channels to PC10 and PC11 with the following configurations
	//	  MODE = Alternative Function 4
	//    Output Type = Push Pull
	//    Pull-up / Pull-down = No Pull
	
	//  PC10 will be connected to TIM8 channel 1N (complemented output)
	//  PC11 will be connected to TIM8 channel 2N (complemented output)
	
	GPIO_MODER_SET(C, 10, GPIO_MODE_AF);
	GPIO_MODER_SET(C, 11, GPIO_MODE_AF);
	
	GPIO_AFR_SET(C, 10, 4UL);
	GPIO_AFR_SET(C, 11, 4UL);
	
	GPIO_OTYPER_SET(C, 10, GPIO_OTYPE_PP);
	GPIO_OTYPER_SET(C, 11, GPIO_OTYPE_PP);
	
	GPIO_PUPDR_SET(C, 10, GPIO_PUPD_NO);
	GPIO_PUPDR_SET(C, 11, GPIO_PUPD_NO);
	
	// Next, Configure Timer 8 for CH1N and CH2N - Mostly like Servo with 2 exceptions 
	// 1. Turn on Timer 8
	
	SET_BITS(RCC->APB2ENR, RCC_APB2ENR_TIM8EN);
	
	// 2. Program Timer 8 prescaler to count in microseconds
		//You need -1 to allow the timer start sequence (mandatory)
		
	SET_BITS(TIM8->PSC, 71UL);										// Set PSC so it counts in 1us (1-10us)
		// Timer Period = (Prescaler + 1) / SystemClockFreq
		// 1us = (Prescaler + 1) / 72MHz
		// (Prescaler + 1) = 72
		// Prescaler = 71
		
	// 3. Set TIM8 to upcounting mode (MANDATORY!! because TIM8 default mode can be something else)
		// [DIFFERENT!] This is different from TIM15 in Servo Lab because TIM15 only supports upcounting
	CLEAR_BITS(TIM8->CR1, TIM_CR1_DIR);						// Set TIM8 counting direction to upcounting
	
	// 4. Set the TIM8 reload value to 1000us period (Remember -1 because it's repeating)
	FORCE_BITS(TIM8->ARR, 0xFFFFUL, 999UL);		// Set ARR to 1000us
		// ARR = Repeating Counter Period - 1
	
	// 5. Enable TIM8 ARR preload
	SET_BITS(TIM8->CR1, TIM_CR1_ARPE);						// Enable ARR preload (ARPE) in CR1
		
	// 6. Set TIM8 main output enabled
	SET_BITS(TIM8->BDTR, TIM_BDTR_MOE);					// Set main output enabled (MOE) in BDTR
	
	
		
	
	// Configure CH1N (Complementary) of TIM8 for Left Wheel PWM output compare mode
	
	// 1. Select PWM mode 1 for TIM8 channel 1N
		// Set OC1M bits to 0110 (i.e. set bit 1 and 2) in CCMR1
		// Read manual again to confirm
	FORCE_BITS(TIM8->CCMR1, TIM_CCMR1_OC1M_Msk, 0x6UL << TIM_CCMR1_OC1M_Pos);				// Set TIM8 to PWM mode
	
	// 2. Enable output compare preload on channel 1 (i.e. update counter register only on timer enabled (UE) )
	    // Set OC1PE bit in CCMR1
	SET_BITS(TIM8->CCMR1, TIM_CCMR1_OC1PE); 
	
	// 3. Enable the COMPLEMENTARY output channel (CH1N)
		// Set CC1NE bit in CCER
		// [DIFFERENT!] This is defferent because PC10 and PC11 only has AF options of TIM8 CH1N/CH2N
		// Read Manual to confirm
	SET_BITS(TIM8->CCER, TIM_CCER_CC1NE);
	
	// 4. Make CH1N active HI (standard polarity)
		// Clear CC1NP bit in CCER
		// [DIFFERENT!] Also because we are working with completentary output
	CLEAR_BITS(TIM8->CCER, TIM_CCER_CC1NP);
	
	// 5. Set the CH1N initial PWM ON-time to 0 by clearing CCR1, so that PWM will not be outputted until the first 1ms elapsed
	CLEAR_BITS(TIM8->CCR1, TIM_CCR1_CCR1);
	
	
	// Configure CH2N (Complementary) of TIM8 for Right Wheel PWM output compare mode
	
	// 1. Select PWM mode 1 for TIM8 channel 2N
		// Set OC2M to 0110 in CCMR1 - same as CH1N configurations
		// Read Manual to confirm
	FORCE_BITS(TIM8->CCMR1, TIM_CCMR1_OC2M_Msk, 0x6UL << TIM_CCMR1_OC2M_Pos);				// Set TIM8 to PWM mode
	
	// 2. Enable output compare preload on channel 2 (i.e. update counter register only on timer enabled (UE) )
		// Set OC2PE to 1 in CCMR1 - same as CH1N configurations
	SET_BITS(TIM8->CCMR1, TIM_CCMR1_OC2PE); 
	
	// 3. Enable the COMPLEMENTARY output channel (CH2N)
		// Set CC2NE in CCER - same as CH1N
	SET_BITS(TIM8->CCER, TIM_CCER_CC2NE);
	
	// 4. Make CH2N active HI (standard polarity)
		// Clear CC2NP in CCER - same as CH1N
	CLEAR_BITS(TIM8->CCER, TIM_CCER_CC2NP);
	
	// 5. Set the CH2N initial PWM ON-time to 0 by clearing CCR1, so that PWM will not be outputted until the first 1ms elapsed
	CLEAR_BITS(TIM8->CCR1, TIM_CCR1_CCR1);
	
	
	// Finally, Start TIM8 CH1N and CH2N Outputs
	// 1. Force an update event to preload all the registers
		// Set UG to 1 in TIM8 EGR
	SET_BITS(TIM8->EGR, TIM_EGR_UG);
	
	// 2. Enable TIM8 to start counting
		// Set CEN in TIM8 CR1
	SET_BITS(TIM8->CR1, TIM_CR1_CEN);
}


void DCMotor_SetSpeed(uint8_t motor, uint16_t dutyCycle){
	// This part is the same as Servo
	//		Write different values into CCR1/CCR2 to control the motor speed using dutyCycle
	//      Make sure you cap the DutyCycle to 100% / 0% so to not cause odd system behaviours
	
	if(dutyCycle > 100){
		dutyCycle = 100;
	}
	
	// Convert to ms ON-time
	dutyCycle *= 10;	// dutyCycle = (dutyCycle * 1000) / 100
	
	if(motor == DCMOTOR_LEFT){
		FORCE_BITS(TIM15->CCR1, 0xFFFFUL, dutyCycle);
	}
	else if(motor == DCMOTOR_RIGHT){
		FORCE_BITS(TIM15->CCR2, 0xFFFFUL, dutyCycle);
	}
}	

void DCMotor_SetDir(uint8_t motor, uint8_t dir){
	// - Motor Direction Control Pins:
	//	Left Motor Forward (A)	PC12
	//	Left Motor Reverse (B)	PC13
	//	Right Motor Forward (A)	PC8
	//	Right Motor Reverse (B)	PC9
	// - Direction Control Truth Table
	//     STOP   FWD	RWD   UNDEFINED
	// (A)  0      1     0      1     
	// (B)  0      0     1      1

	// POSSIBLE INPUTS
	// motor:		0 - left
	//					1 - right
	//
	// dir:			0 - stop
	//					1 - forward
	//					2 - backwards

	// Deploy a switch structure to set the corresponding GPIO pins according to the direction input
	if(motor == DCMOTOR_LEFT){
		if(dir == DCMOTOR_STOP){
			//left motor stop
			GPIOC->ODR &= ~(1UL << (1*12));
			GPIOC->ODR &= ~(1UL << (1*13));
		}
		else if(dir == DCMOTOR_FWD){
			//left motor fwd
			GPIOC->ODR |= (1UL << (1*12));
			GPIOC->ODR &= ~(1UL << (1*13));
		}
		else if(dir == DCMOTOR_BWD){
			//left motor rwd
			GPIOC->ODR &= ~(1UL << (1*12));
			GPIOC->ODR |= (1UL << (1*13));
		}
	}
	else if (motor == DCMOTOR_RIGHT){
		if(dir == DCMOTOR_STOP){
			//right motor stop
			GPIOC->ODR &= ~(1UL << (1*8));
			GPIOC->ODR &= ~(1UL << (1*9));

		}
		else if(dir == DCMOTOR_FWD){
			//right motor fwd
			GPIOC->ODR |= (1UL << (1*8));
			GPIOC->ODR &= ~(1UL << (1*9));
		}
		else if(dir == DCMOTOR_BWD){
			//right motor rwd
			GPIOC->ODR &= ~(1UL << (1*8));
			GPIOC->ODR |= (1UL << (1*9));
		}
	}
}




