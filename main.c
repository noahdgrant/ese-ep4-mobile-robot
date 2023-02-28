/******************************************************************************
* Name: main.c (client)
* Author(s): Noah Grant, Wyatt Richard
* Date: February 17, 2023
* Description: Keypad control for mobile robot.
******************************************************************************/

#include "SysClock.h"
#include "Utility.h"
#include "UART.h"
#include "Stepper.h"
#include "RCServo.h"
#include "LED.h"
#include "PushButton.h"
#include "KeyPad.h"

int main(void){	
	// INITIALIZE
	uint8_t pressedKey = '\0';		// Key pressed by user
	int8_t RCServoAngle = 0;			// Servo angle
	uint8_t StepperMode = 0;			// Stepper mode (continuous or single output)
	uint8_t StepperLastStep = 0;	// The last step the servo took
	
	System_Clock_Init();					// Scale clock speed to 72MHz
	SystemCoreClockUpdate();
	
	UART2_Init();
	Stepper_Init();
	RCServo_Init();
	LED_Init();
	KeyPad_Init();
	
	// Print menu
	UART_printf("ESS Lab 4 - Keypad\n");
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
		}
		
		// Stepper continuous output mode
		if(StepperMode){
			Stepper_Step(StepperLastStep);
		}
	}
}
