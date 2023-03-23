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
#include "Ultrasonic.h"

int main(void){	
	// INITIALIZE
	uint32_t distance = 0;
	char cmd = '\0';
	
	System_Clock_Init();								// Scale clock speed to 72MHz
	SystemCoreClockUpdate();
	
	UART2_Init();
	Ultra_InitTrigger();
	Ultra_InitEcho();
	
	UART_printf("ESS Lab 7 - Ultrasonic Sensor\n\n");

	// PROGRAM LOOP
	while(1){
		cmd = UART_getcNB();
		
			if(cmd == 'f'){
				Ultra_StartTrigger();
				
				while(!Ultra_EchoRx()){
					// Do something
					UART_printf("waiting\n");
				}
				distance = Ultra_ReadSensor();
				UART_printf("Distance: %dcm\n", distance);
				
				cmd = '\0';
			}
	}
}
