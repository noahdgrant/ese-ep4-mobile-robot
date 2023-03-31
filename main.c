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
	uint8_t customChar[8] = {'<', '>', '|', '}', '{', ']', '[', '^'};
	uint8_t customChar0[8] = {0x00, 0x0A, 0x0A, 0x0A, 0x00, 0x0E, 0x11, 0x00};		// Frowning face
	
	// Frames for animation
	// Frame 0						
  uint8_t F0image01[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07};
  uint8_t F0image02[8] = {0x00, 0x08, 0x10, 0x10, 0x10, 0x1F, 0x1F, 0x18};
  uint8_t F0image03[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x04};
	uint8_t F0image10[8] = {0x06, 0x0D, 0x1B, 0x13, 0x07, 0x0F, 0x0F, 0x1F};
  uint8_t F0image11[8] = {0x0F, 0x1E, 0x1C, 0x18, 0x18, 0x10, 0x10, 0x00};
	// Frame 1
  uint8_t F1image00[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03};
  uint8_t F1image01[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x17};
  uint8_t F1image02[8] = {0x00, 0x08, 0x10, 0x10, 0x10, 0x1F, 0x1F, 0x18};
  uint8_t F1image03[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x04};
  uint8_t F1image10[8] = {0x06, 0x05, 0x03, 0x03, 0x07, 0x0F, 0x0F, 0x1F};
  uint8_t F1image11[8] = {0x0F, 0x1E, 0x1C, 0x18, 0x18, 0x10, 0x10, 0x00};
	// Frame 2
  uint8_t F2image01[8] = {0x00, 0x00, 0x00, 0x01, 0x07, 0x04, 0x19, 0x17};
  uint8_t F2image02[8] = {0x00, 0x08, 0x10, 0x10, 0x10, 0x1F, 0x1F, 0x18};
  uint8_t F2image03[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x04};
	uint8_t F2image10[8] = {0x00, 0x01, 0x03, 0x03, 0x07, 0x0F, 0x0F, 0x1F};
  uint8_t F2image11[8] = {0x0F, 0x1E, 0x1C, 0x18, 0x18, 0x10, 0x10, 0x00};
	// Frame 3
  uint8_t F3image01[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07};
  uint8_t F3image02[8] = {0x00, 0x08, 0x10, 0x10, 0x10, 0x1F, 0x1F, 0x1A};
  uint8_t F3image03[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x04};
	uint8_t F3image10[8] = {0x00, 0x01, 0x03, 0x03, 0x07, 0x0F, 0x0F, 0x1F};
  uint8_t F3image11[8] = {0x0F, 0x1E, 0x1C, 0x18, 0x18, 0x10, 0x10, 0x00};
  uint8_t F3image12[8] = {0x02, 0x07, 0x07, 0x07, 0x07, 0x07, 0x02, 0x00};
	// Frame 4
	uint8_t F4image01[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07};
  uint8_t F4image02[8] = {0x00, 0x08, 0x10, 0x10, 0x10, 0x1F, 0x1F, 0x1A};
  uint8_t F4image03[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x04};
  uint8_t F4image10[8] = {0x00, 0x01, 0x03, 0x03, 0x07, 0x0F, 0x0F, 0x1F};
  uint8_t F4image11[8] = {0x0F, 0x1E, 0x1C, 0x18, 0x18, 0x11, 0x10, 0x00};
  uint8_t F4image12[8] = {0x02, 0x04, 0x0B, 0x15, 0x1A, 0x15, 0x1A, 0x0E};
  uint8_t F4image13[8] = {0x00, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x00};
	// Frame 5
  uint8_t F5image01[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03};
  uint8_t F5image02[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10};
  uint8_t F5image12[8] = {0x0A, 0x14, 0x0B, 0x15, 0x1A, 0x15, 0x1A, 0x0E};
  uint8_t F5image13[8] = {0x00, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x00};
  uint8_t F5image11[8] = {0x0D, 0x0A, 0x0D, 0x07, 0x00, 0x00, 0x00, 0x00};
	// Frame 6
  uint8_t F6image02[8] = {0x00, 0x04, 0x0A, 0x0A, 0x11, 0x03, 0x06, 0x0C};
  uint8_t F6image01[8] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x02, 0x01};
  uint8_t F6image03[8] = {0x00, 0x00, 0x10, 0x18, 0x00, 0x00, 0x18, 0x04};
  uint8_t F6image12[8] = {0x04, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t F6image13[8] = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	// Frame 7
	uint8_t F7image02[8] = {0x00, 0x04, 0x0A, 0x0A, 0x11, 0x03, 0x06, 0x0C};
  uint8_t F7image01[8] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x02, 0x01};
  uint8_t F7image03[8] = {0x00, 0x00, 0x10, 0x18, 0x00, 0x00, 0x00, 0x00};
  uint8_t F7image12[8] = {0x15, 0x0E, 0x0E, 0x04, 0x15, 0x0E, 0x04, 0x1F};
	
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
	LCD_SetCustomCharIdentifier(customChar);
	
	// PROGRAM LOOP
	while(1){
		// frame 00
		LCD_Clear();
		LCD_HomeCursor();
		LCD_CustomChar(F0image01, 0);
		LCD_CustomChar(F0image02, 1);
		LCD_CustomChar(F0image03, 2);
		LCD_CustomChar(F0image10, 3);
		LCD_CustomChar(F0image11, 4);
		LCD_Clear();
		LCD_HomeCursor();
		LCD_printf(" %c%c%c Hello!\n%c%c", customChar[0],customChar[1],customChar[2],customChar[3],customChar[4]);
		Delay_ms(250);
		// frame 01
		LCD_Clear();
		LCD_HomeCursor();
		LCD_CustomChar(F1image00, 0);
		LCD_CustomChar(F1image01, 1);
		LCD_CustomChar(F1image02, 2);
		LCD_CustomChar(F1image03, 3);
		LCD_CustomChar(F1image10, 4);
		LCD_CustomChar(F1image11, 5);
		LCD_Clear();
		LCD_HomeCursor();
		LCD_printf("%c%c%c%c Hello!\n%c%c", customChar[0],customChar[1],customChar[2],customChar[3],customChar[4], customChar[5]);
    Delay_ms(250);
		// frame 02
		LCD_Clear();
		LCD_HomeCursor();
		LCD_CustomChar(F2image01, 0);
		LCD_CustomChar(F2image02, 1);
		LCD_CustomChar(F2image03, 2);
		LCD_CustomChar(F2image10, 3);
		LCD_CustomChar(F2image11, 4);
		LCD_Clear();
		LCD_HomeCursor();
		LCD_printf(" %c%c%c Hello!\n%c%c", customChar[0],customChar[1],customChar[2],customChar[3],customChar[4]);
    Delay_ms(250);
		// frame 03
		LCD_Clear();
		LCD_HomeCursor();
		LCD_CustomChar(F3image01, 0);
		LCD_CustomChar(F3image02, 1);
		LCD_CustomChar(F3image03, 2);
		LCD_CustomChar(F3image10, 3);
		LCD_CustomChar(F3image11, 4);
		LCD_CustomChar(F3image12, 5);
		LCD_Clear();
		LCD_HomeCursor();
		LCD_printf(" %c%c%c Hello!\n%c%c%c", customChar[0],customChar[1],customChar[2],customChar[3],customChar[4]);
    Delay_ms(700);
		// frame 04
		LCD_Clear();
		LCD_HomeCursor();
		LCD_CustomChar(F4image01, 0);
		LCD_CustomChar(F4image02, 1);
		LCD_CustomChar(F4image03, 2);
		LCD_CustomChar(F4image10, 3);
		LCD_CustomChar(F4image11, 4);
		LCD_CustomChar(F4image12, 5);
		LCD_CustomChar(F4image13, 6);
		LCD_Clear();
		LCD_HomeCursor();
		LCD_printf(" %c%c%c Hello!\n%c%c%c%c", customChar[0],customChar[1],customChar[2],customChar[3],customChar[4],customChar[5],customChar[6]);
    Delay_ms(250);
		// frame 05
		LCD_Clear();
		LCD_HomeCursor();
		LCD_CustomChar(F5image01, 0);
		LCD_CustomChar(F5image02, 1);
		LCD_CustomChar(F5image11, 2);
		LCD_CustomChar(F5image12, 3);
		LCD_CustomChar(F5image13, 4);
		LCD_Clear();
		LCD_HomeCursor();
		LCD_printf(" %c%c  Hello!\n %c%c%c", customChar[0],customChar[1],customChar[2],customChar[3],customChar[4],customChar[5],customChar[6]);
    Delay_ms(250);
		// frame 06
		LCD_Clear();
		LCD_HomeCursor();
		LCD_CustomChar(F6image01, 0);
		LCD_CustomChar(F6image02, 1);
		LCD_CustomChar(F6image03, 2);
		LCD_CustomChar(F6image12, 3);
		LCD_CustomChar(F6image13, 4);
		LCD_Clear();
		LCD_HomeCursor();
		LCD_printf(" %c%c%c Hello!\n  %c%c", customChar[0],customChar[1],customChar[2],customChar[3],customChar[4],customChar[5],customChar[6]);
    Delay_ms(700);
		// frame 07
		LCD_Clear();
		LCD_HomeCursor();
		LCD_CustomChar(F7image01, 0);
		LCD_CustomChar(F7image02, 1);
		LCD_CustomChar(F7image03, 2);
		LCD_CustomChar(F7image12, 3);
		LCD_Clear();
		LCD_HomeCursor();
		LCD_printf(" %c%c%c Hello!\n  %c", customChar[0],customChar[1],customChar[2],customChar[3],customChar[4],customChar[5],customChar[6]);

    Delay_ms(1250);
	}
}
