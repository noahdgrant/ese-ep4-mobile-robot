/******************************************************************************
* Name: main.c (client)
* Author(s): Noah Grant, Wyatt Richard
* Date: March 24, 2023
* Description: DC motor control for mobile robot.
******************************************************************************/

#include "SysClock.h"
#include "Utility.h"
#include "UART.h"
#include "Stepper.h"
#include "RCServo.h"
#include "LED.h"
#include "PushButton.h"
#include "KeyPad.h"
#include "Ultrasonic.h"
#include "DCMotor.h"

int main(void){	
	// INITIALIZE
	uint8_t pressedKey = '\0';		// Key pressed by user
	int8_t RCServoAngle = 0;			// Servo angle
	uint8_t StepperMode = 0;			// Stepper mode (continuous or single output)
	uint8_t StepperLastStep = 0;	// The last step the servo took
	uint32_t distance = 0;				// Distance recorded by the ultrasonic sensor

	
	System_Clock_Init();					// Scale clock speed to 72MHz
	SystemCoreClockUpdate();
	
	UART2_Init();
	Stepper_Init();
	RCServo_Init();
	LED_Init();
	KeyPad_Init();
	DCMotor_Init();
	Ultra_InitTrigger();
	Ultra_InitEcho();
	
	// Print menu
	UART_printf("Mobile Robot Control Testing\n");
	UART_printf("Press a key on the keypad\n");

	// PROGRAM LOOP
	while(1){
		pressedKey = KeyPad_GetKey();
		switch(pressedKey){
			// STEPPER
			// OFF
			case '0':{
				StepperLastStep = 0;
				Stepper_Step(0);
				break;
			}
			// Full step CW
			case '1':{
				StepperLastStep = 1;
				Stepper_Step(1);
				break;
			}
			// Full step CCW
			case '2':{
				StepperLastStep = 2;
				Stepper_Step(2);
				break;
			}
			// Half step CW
			case '3':{
				StepperLastStep = 3;
				Stepper_Step(3);
				break;
			}
			// Half step CCW
			case '4':{
				StepperLastStep = 4;
				Stepper_Step(4);
				break;
			}
			// Toggle between continuous output mode or single output mode
			case '#':{
				if(StepperMode == 1){
					StepperMode = 0;
				}
				else{
					StepperMode = 1;
				}
				break;
			}
			
			// LED
			case '*':{
				LED_Toggle();
				break;
			}
			
			// SERVO
			// Decrease servo angle
			case '7':{
				RCServoAngle -= 5;
				RCServo_SetAngle(RCServoAngle);
				break;
			}
			// Centre servo
			case '8':{
				RCServoAngle = 0;
				RCServo_SetAngle(RCServoAngle);
				break;
			}
			// Increase servo angle
			case '9':{
				RCServoAngle += 5;
				RCServo_SetAngle(RCServoAngle);
				break;
			}
 			// DC Motor
			// Both Forward 100%
			case 'A':{
				UART_printf("FWD 100%% both motors\n");
				DCMotor_SetSpeed(DCMOTOR_LEFT, 100);
				DCMotor_SetSpeed(DCMOTOR_RIGHT, 100);
				DCMotor_SetDir(DCMOTOR_LEFT, DCMOTOR_FWD);
				DCMotor_SetDir(DCMOTOR_RIGHT, DCMOTOR_FWD);
				break;
			}
			// Both reverse 100%
			case 'B':{
				UART_printf("BWD 100%% both motors\n");
				DCMotor_SetSpeed(DCMOTOR_LEFT, 100);
				DCMotor_SetSpeed(DCMOTOR_RIGHT, 100);
				DCMotor_SetDir(DCMOTOR_LEFT, DCMOTOR_BWD);
				DCMotor_SetDir(DCMOTOR_RIGHT, DCMOTOR_BWD);
				break;
			}
			// Both Stop
			case 'C':{
				UART_printf("STOP both motors\n");
				DCMotor_SetSpeed(DCMOTOR_LEFT, 0);
				DCMotor_SetSpeed(DCMOTOR_RIGHT, 0);
				DCMotor_SetDir(DCMOTOR_LEFT, DCMOTOR_STOP);
				DCMotor_SetDir(DCMOTOR_RIGHT, DCMOTOR_STOP);
				break;
			}
			// Ultrasonic
			case 'D':{
				Ultra_StartTrigger();
				
				while(!Ultra_EchoRx()){
					// Do something
					UART_printf("waiting\n");
				}
				distance = Ultra_ReadSensor();
				UART_printf("Distance: %dcm\n", distance);
				break;
			}
		}
		
		// Stepper continuous output mode
		if(StepperMode){
			Stepper_Step(StepperLastStep);
		}
	}
}

//int main(void){	
//	// INITIALIZE
//	char cmd = '\0';
//	
//	System_Clock_Init();								// Scale clock speed to 72MHz
//	SystemCoreClockUpdate();
//	
//	UART2_Init();
//	DCMotor_Init();
//	
//	// Print menu
//	UART_printf("ESS Lab 7 - DC Motor\n\n");
//	UART_printf("Q. Left motor STOP\n");
//	UART_printf("W. Left motor FWD\n");
//	UART_printf("E. Left motor BWD\n");
//	UART_printf("A. Right motor STOP\n");
//	UART_printf("S. Right motor FWD\n");
//	UART_printf("D. Right motor BWD\n");
//	UART_printf("1. LEFT 0 duty cycle\n");
//	UART_printf("2. LEFT 50 duty cycle\n");
//	UART_printf("3. LEFT 100 duty cycle\n");
//	UART_printf("4. RIGHT 0 duty cycle\n");
//	UART_printf("5. RIGHT 50 duty cycle\n");
//	UART_printf("6. RIGHT 100 duty cycle\n");
//	UART_printf("-----------------------\n");
//	UART_printf("Y. STOP both motors\n");
//	UART_printf("U. FWD 50%% both motors\n");
//	UART_printf("I. FWD 100%% both motors\n");
//	UART_printf("O. BWD 50%% both motors \n");
//	UART_printf("P. BWD 100%% both motors\n");
//	UART_printf("H. 0-point left turn\n");
//	UART_printf("J. 0-point right turn\n");
//	UART_printf("V. left motor 50%% FWD\n");
//	UART_printf("B. left motor 50%% BWD\n");
//	UART_printf("N. right motor 50%% FWD\n");
//	UART_printf("M. right motor 50%% BWD\n");
//	UART_printf("\nOUTPUT:\n\n");

//	// PROGRAM LOOP
//	while(1){
//		cmd = UART_getcNB();
//		
//		switch(cmd){
//			case('q'):{
//				UART_printf("LEFT STOP\n");
//				DCMotor_SetDir(DCMOTOR_LEFT, DCMOTOR_STOP);
//				break;
//			}
//			case('w'):{
//				UART_printf("LEFT FWD\n");
//				DCMotor_SetDir(DCMOTOR_LEFT, DCMOTOR_FWD);
//				break;
//			}
//			case('e'):{
//				UART_printf("LEFT BWD\n");
//				DCMotor_SetDir(DCMOTOR_LEFT, DCMOTOR_BWD);
//				break;
//			}
//			case('a'):{
//				UART_printf("RIGHT STOP\n");
//				DCMotor_SetDir(DCMOTOR_RIGHT, DCMOTOR_STOP);
//				break;
//			}
//			case('s'):{
//				UART_printf("RIGHT FWD\n");
//				DCMotor_SetDir(DCMOTOR_RIGHT, DCMOTOR_FWD);
//				break;
//			}
//			case('d'):{
//				UART_printf("RIGHT BWD\n");
//				DCMotor_SetDir(DCMOTOR_RIGHT, DCMOTOR_BWD);
//				break;
//			}
//			case('1'):{
//				UART_printf("LEFT 0\n");
//				DCMotor_SetSpeed(DCMOTOR_LEFT, 0);
//				break;
//			}
//			case('2'):{
//				UART_printf("LEFT 50\n");
//				DCMotor_SetSpeed(DCMOTOR_LEFT, 50);
//				break;
//			}
//			case('3'):{
//				UART_printf("LEFT 100\n");
//				DCMotor_SetSpeed(DCMOTOR_LEFT, 100);
//				break;
//			}
//			case('4'):{
//				UART_printf("RIGHT 0\n");
//				DCMotor_SetSpeed(DCMOTOR_RIGHT, 0);
//				break;
//			}
//			case('5'):{
//				UART_printf("RIGHT 50\n");
//				DCMotor_SetSpeed(DCMOTOR_RIGHT, 50);
//				break;
//			}
//			case('6'):{
//				UART_printf("RIGHT 100\n");
//				DCMotor_SetSpeed(DCMOTOR_RIGHT, 100);
//				break;
//			}
//			// Combined control
//			// STOP both motors
//			case('y'):{
//				UART_printf("STOP both motors\n");
//				DCMotor_SetSpeed(DCMOTOR_LEFT, 0);
//				DCMotor_SetSpeed(DCMOTOR_RIGHT, 0);
//				DCMotor_SetDir(DCMOTOR_LEFT, DCMOTOR_STOP);
//				DCMotor_SetDir(DCMOTOR_RIGHT, DCMOTOR_STOP);
//				break;
//			}
//			// FWD 50% both motors
//			case('u'):{
//				UART_printf("FWD 50%% both motors\n");
//				DCMotor_SetSpeed(DCMOTOR_LEFT, 50);
//				DCMotor_SetSpeed(DCMOTOR_RIGHT, 50);
//				DCMotor_SetDir(DCMOTOR_LEFT, DCMOTOR_FWD);
//				DCMotor_SetDir(DCMOTOR_RIGHT, DCMOTOR_FWD);
//				break;
//			}
//			// FWD 100% both motors
//			case('i'):{
//				UART_printf("FWD 100%% both motors\n");
//				DCMotor_SetSpeed(DCMOTOR_LEFT, 100);
//				DCMotor_SetSpeed(DCMOTOR_RIGHT, 100);
//				DCMotor_SetDir(DCMOTOR_LEFT, DCMOTOR_FWD);
//				DCMotor_SetDir(DCMOTOR_RIGHT, DCMOTOR_FWD);
//				break;
//			}
//			// BWD 50% both motors
//			case('o'):{
//				UART_printf("BWD 50%% both motors\n");
//				DCMotor_SetSpeed(DCMOTOR_LEFT, 50);
//				DCMotor_SetSpeed(DCMOTOR_RIGHT, 50);
//				DCMotor_SetDir(DCMOTOR_LEFT, DCMOTOR_BWD);
//				DCMotor_SetDir(DCMOTOR_RIGHT, DCMOTOR_BWD);
//				break;
//			}
//			// BWD 100% both motors
//			case('p'):{
//				UART_printf("BWD 100%% both motors\n");
//				DCMotor_SetSpeed(DCMOTOR_LEFT, 100);
//				DCMotor_SetSpeed(DCMOTOR_RIGHT, 100);
//				DCMotor_SetDir(DCMOTOR_LEFT, DCMOTOR_BWD);
//				DCMotor_SetDir(DCMOTOR_RIGHT, DCMOTOR_BWD);
//				break;
//			}
//			// 0-point left turn
//			case('h'):{
//				UART_printf("0-point left turn\n");
//				DCMotor_SetSpeed(DCMOTOR_LEFT, 50);
//				DCMotor_SetSpeed(DCMOTOR_RIGHT, 50);
//				DCMotor_SetDir(DCMOTOR_LEFT, DCMOTOR_BWD);
//				DCMotor_SetDir(DCMOTOR_RIGHT, DCMOTOR_FWD);
//				break;
//			}
//			// 0-point right turn
//			case('j'):{
//				UART_printf("0-point right turn\n");
//				DCMotor_SetSpeed(DCMOTOR_LEFT, 50);
//				DCMotor_SetSpeed(DCMOTOR_RIGHT, 50);
//				DCMotor_SetDir(DCMOTOR_LEFT, DCMOTOR_FWD);
//				DCMotor_SetDir(DCMOTOR_RIGHT, DCMOTOR_BWD);
//				break;
//			}
//			// left motor 50% FWD
//			case('v'):{
//				UART_printf("Left motor 50%% FWD\n");
//				DCMotor_SetSpeed(DCMOTOR_LEFT, 50);
//				DCMotor_SetDir(DCMOTOR_LEFT, DCMOTOR_FWD);
//				break;
//			}
//			// left motor 50% BWD
//			case('b'):{
//				UART_printf("Left motor 50%% BWD\n");
//				DCMotor_SetSpeed(DCMOTOR_LEFT, 50);
//				DCMotor_SetDir(DCMOTOR_LEFT, DCMOTOR_BWD);
//				break;
//			}
//			// right motor 50% FWD
//			case('n'):{
//				UART_printf("Right motor 50%% FWD\n");
//				DCMotor_SetSpeed(DCMOTOR_RIGHT, 50);
//				DCMotor_SetDir(DCMOTOR_RIGHT, DCMOTOR_FWD);
//				break;
//			}
//			// left motor 50% BWD
//			case('m'):{
//				UART_printf("Right motor 50%% BWD\n");
//				DCMotor_SetSpeed(DCMOTOR_RIGHT, 50);
//				DCMotor_SetDir(DCMOTOR_RIGHT, DCMOTOR_BWD);
//				break;
//			}
//			default:{
//			}
//		}
//		
//		cmd = '\0';
//	}
//}
