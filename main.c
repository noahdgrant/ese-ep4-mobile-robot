/******************************************************************************
* Name: main.c (client)
* Author(s): Noah Grant, Wyatt Richard
* Date: March 17, 2023
* Description: Ultrasonic control for mobile robot.
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
	 uint32_t Global_UltraEcho = 0;			// Global
	
	System_Clock_Init();					// Scale clock speed to 72MHz
	SystemCoreClockUpdate();
	
	UART2_Init();


	// PROGRAM LOOP
	while(1){
		
	}
}
