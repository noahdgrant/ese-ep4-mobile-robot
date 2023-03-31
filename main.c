/******************************************************************************
* Name: main.c (client)
* Author(s): Noah Grant, Wyatt Richard
* Date: March 24, 2023
* Description: Encoder control for mobile robot.
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
#include "LCD.h"
#include "Encoder.h"

int main(void){	
	// INITIALIZE	
	char cmd = '\0';
	
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
	LCD_Init();
	Encoder_Init();
	
	UART_printf("ESS Lab 8 - Encoders\n");
	UART_printf("S - Motors STOP\n");
	UART_printf("G - Motors ON\n");
	UART_printf("F - Check encoder counts\n\n");

	// PROGRAM LOOP
	while(1){
		cmd = UART_getcNB();
		
		switch(cmd){
			case 'f':{
				Encoder_CalculateSpeed();
				UART_printf("Left encoder: %d\n", Global_LeftEncoderPeriod);
				UART_printf("RIght encoder: %d\n\n", Global_RightEncoderPeriod);
				cmd = '\0';
				break;
			}
			case 's':{
				DCMotor_SetDir(DCMOTOR_LEFT, DCMOTOR_STOP);
				DCMotor_SetDir(DCMOTOR_RIGHT, DCMOTOR_STOP);
				DCMotor_SetSpeed(DCMOTOR_LEFT, 0);
				DCMotor_SetSpeed(DCMOTOR_RIGHT, 0);
				cmd = '\0';
				break;
			}
			case 'g':{
				DCMotor_SetDir(DCMOTOR_LEFT, DCMOTOR_FWD);
				DCMotor_SetDir(DCMOTOR_RIGHT, DCMOTOR_FWD);
				DCMotor_SetSpeed(DCMOTOR_LEFT, 80);
				DCMotor_SetSpeed(DCMOTOR_RIGHT, 80);	
				cmd = '\0';
				break;
			}
			default:{
			}
		}
	}
}
