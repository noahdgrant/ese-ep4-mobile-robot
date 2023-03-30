/******************************************************************************
* Name: main.c (client)
* Author(s): Noah Grant, Wyatt Richard
* Date: March 24, 2023
* Description: LCD control for mobile robot.
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

int main(void){	
	// INITIALIZE	
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
	
	LCD_Clear();
	LCD_HomeCursor();
	LCD_printf("1");

	// PROGRAM LOOP
	while(1){
		LCD_printf("1");
	}
}
