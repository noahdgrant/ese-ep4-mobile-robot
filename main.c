/******************************************************************************
* Name: main.c (client)
* Author(s): Noah Grant, Wyatt Richard
* Date: April 14, 2023
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
	Ultra_Init();
	LCD_Init();
	Encoder_Init();
	
	UART_printf("ESS Lab 8 - Encoders\n");
	UART_printf("F - Check encoder counts\n");
	UART_printf("S - Motors STOP\n");
	UART_printf("G - Motors 100 duty cycle\n");
	UART_printf("H - Motors 60 duty cycle\n\n");

	// PROGRAM LOOP
	while(1){
		cmd = UART_getcNB();
		
		switch(cmd){
			case 'f':{
				Encoder_CalculateSpeed();
				UART_printf("Left encoder: %d us/vane\n", Global_LeftEncoderPeriod);
				UART_printf("Right encoder: %d us/vane\n\n", Global_RightEncoderPeriod);
				cmd = '\0';
				break;
			}
			case 's':{
				DCMotor_Stop();
				cmd = '\0';
				break;
			}
			case 'g':{
				DCMotor_Forward(100);
				cmd = '\0';
				break;
			}
			case 'h':{
				DCMotor_Forward(60);
				cmd = '\0';
				break;
			}
			default:{
			}
		}
	}
}
