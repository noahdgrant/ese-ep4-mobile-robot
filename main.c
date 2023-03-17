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
	//uint32_t Global_UltraEcho = 0;			// Global
	uint32_t distance = 0;
	
	System_Clock_Init();								// Scale clock speed to 72MHz
	SystemCoreClockUpdate();
	
	UART2_Init();
	Ultra_InitTrigger();
	Ultra_InitEcho();

	// PROGRAM LOOP
	while(1){
		distance = Ultra_ReadSensor();
		UART_printf("Distance: %d\n", distance);
		Delay_ms(500);
	}
}
