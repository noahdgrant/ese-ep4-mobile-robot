/********************************************************************************
* Name: LCD.c (implementation)
* Author(s): Noah Grant, Wyatt Richard
* Date: March 31, 2023
* Description: LCD functions for mobile robot.
********************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include "stm32f303xe.h"
#include "LCD.h"
#include "Utility.h"


/******************************************************************
*												PRIVATE FUNCTIONS													*
******************************************************************/
/*************************************************
* LCD_GPIO_Init() - Initialize GPIO PA6-11 pins for LCD.
* No inputs.
* No return value.
*************************************************/
static void LCD_GPIO_Init(void){
	ENABLE_GPIO_CLOCK(A);
	
	// Initialize GPIO PA6-11 pins one at a time
	for(int i=6; i<=11; i++){
		GPIO_MODER_SET(A, i, GPIO_MODE_OUT);
		GPIO_OTYPER_SET(A, i, GPIO_OTYPE_PP);
		GPIO_PUPDR_SET(A, i, GPIO_PUPD_NO);
		GPIO_OSPEEDR_SET(A, i, GPIO_OSPEED_LOW);		// ?? - found online
	}
}


/******************************************************************
*												PUBLIC FUNCTIONS													*
******************************************************************/
/*************************************************
* LCD_Init() - Initialize LCD.
* No inputs.
* No return value.
*************************************************/
void LCD_Init(void){	
	LCD_GPIO_Init();
	
	Delay_ms(20);														// Wait 10ms
	
	// Get ready for LCD communication
	CLEAR_BITS(LCD_PORT, LCD_PORT_BITS);		// Clear all bits on the LCD port
	LCD_E_LO;																// Set E LOW
	LCD_RS_IR;															// Set RS to instruction
	Delay_ms(15);														// Wait 10ms
	
	// Syncing sequence 1
	// Send 0x03 on the data bus, wait for 5ms
	LCD_E_HI;
	LCD_BUS(0x03UL);
	LCD_E_LO;
	Delay_ms(5);
	
	// Syncing sequence 2
	// Send 0x03 on the data bus, wait for 1ms
	LCD_E_HI;
	LCD_BUS(0x03UL);
	LCD_E_LO;
	Delay_ms(1);
	
	// Syncing sequence 3
	// Send 0x03 on the data bus, no wait
	LCD_E_HI;
	LCD_BUS(0x03UL);
	LCD_E_LO;
	
	// Syncing sequence 4
	// Send 0x02 on the data bus, no wait
	LCD_E_HI;
	LCD_BUS(0x02UL);
	LCD_E_LO;
	
	// Send function command to the LCD for 4-bit mode, 2 display lines, and 5x8 font
	LCD_cmd(LCD_CMD_FUNCTION | LCD_FUNCTION_5X8FONT | LCD_FUNCTION_2LINES | LCD_FUNCTION_4BITBUS);
	
	// Send display command to LCD to turn OFF LCD for the remaining part of the initialization
	LCD_cmd(LCD_CMD_DISPLAY | LCD_DISPLAY_OFF);
	
	// Send clear command to the LCD
	LCD_cmd(LCD_CMD_CLEAR);
	
	// Send entry command to the LCD for move cursor mode and increment mode (i.e. left-to-right entry)
	LCD_cmd(LCD_CMD_ENTRY | LCD_ENTRY_MOVE_CURSOR);
	
	// Send display command again to LCD to turn ON LCD with no cursor display and no cursor blinking
	LCD_cmd(LCD_CMD_DISPLAY | LCD_DISPLAY_NOBLINK | LCD_DISPLAY_NOCURSOR);
}

/*************************************************
* LCD_Clear() - Clear LCD screen.
* No inputs.
* No return value.
*************************************************/
void LCD_Clear(void){
	Delay_ms(LCD_STD_CMD_DELAY);
	LCD_cmd(LCD_CMD_CLEAR);
	Delay_ms(LCD_LONG_CMD_DELAY);
	//LCD_cmd(0x0);			// ?? 
}

/******************************************************************
* LCD_HomeCursor() - Home the LCD cursor to the top left position.
* No inputs.
* No return value.
******************************************************************/
void LCD_HomeCursor(void){
	LCD_cmd(LCD_CMD_HOME);
	Delay_ms(LCD_LONG_CMD_DELAY);
}

/*************************************************
* LCD_cmd() - Send a command to the LCD.
* No inputs.
* No return value.
*************************************************/
void LCD_cmd(uint8_t cmd){
	//Delay_ms(LCD_STD_CMD_DELAY);		// this shifts the LCD output during startup ??
	
	LCD_E_LO;
	LCD_RS_IR;
	
	Delay_ms(TEST_DELAY);
	LCD_E_HI;
	LCD_BUS(HI_NYBBLE(cmd));
	Delay_ms(TEST_DELAY);
	LCD_E_LO;
	
	Delay_ms(TEST_DELAY);
	LCD_E_HI;
	LCD_BUS(LO_NYBBLE(cmd));
	Delay_ms(TEST_DELAY);
	LCD_E_LO;
}

/*************************************************
* LCD_data() - Send data to the LCD.
* No inputs.
* No return value.
*************************************************/
void LCD_data(uint8_t data){
	//Delay_ms(LCD_STD_CMD_DELAY);
	
	LCD_E_LO;
	LCD_RS_DR;
	
	Delay_ms(TEST_DELAY);
	LCD_E_HI;
	LCD_BUS(HI_NYBBLE(data));
	Delay_ms(TEST_DELAY);
	LCD_E_LO;
	
	Delay_ms(TEST_DELAY);
	LCD_E_HI;
	LCD_BUS(LO_NYBBLE(data));
	Delay_ms(TEST_DELAY);
	LCD_E_LO;
}

/*************************************************
* LCD_putc() - Send a character to LCD.
* No inputs.
* No return value.
*************************************************/
void LCD_putc(unsigned char ch){
	if(ch == '\n'){
		LCD_cmd(LCD_CMD_SETDDADDR | LCD_DDRAM_ADDR_LINE2); 		// maybe toggle
	}
	else if(ch == '\r'){
		LCD_HomeCursor();
	}
	else{
		LCD_data(ch);
	}
}

/*************************************************
* LCD_puts() - Send a string to LCD.
* No inputs.
* No return value.
*************************************************/
void LCD_puts(char* str){
	while(*str != '\0'){
		LCD_putc(*str++);
	}
}

/*************************************************
* LCD_printf() - Output a string on LCD.
* fmt		- String to transmit.
* No return value.
*************************************************/
void LCD_printf(char* str, ... ){
	va_list args;
	char buffer[MAX_LCD_BUFSIZE];
	
	va_start(args, str);
	(void)vsnprintf(buffer, MAX_LCD_BUFSIZE, str, args);
	va_end(args);
	LCD_puts(buffer);
}

/***********************************************************
* LCD_customc() - Send a customer character to the LCD.
* character		- Customer character hex values.
* address			- The address to store the customer character.
* No return value.
************************************************************/
void LCD_customc(uint8_t character[8], uint8_t address){
	//up to 8 custom characters can be added can be accessed in cgram character code 0x00 to 0x07
	LCD_cmd(LCD_CMD_CGRAMADDR + address);
	Delay_ms(4); 								// Wait > 39us
	for(int i=0; i<8; i++){
		LCD_data(character[i]);
	}

	// Select display RAM & set address to 0
	LCD_cmd(0x80); 				// First character
	LCD_data(address); 		// Display the font
}

/*
example custom font: smile face
// Select CGRAM and set address to 0x00
LCD_SendCmd(0x40 + 0x00);
Delay(4); // Wait > 39us
// Define smile face
LCD_SendData(0x00); // 1st row byte
LCD_SendData(0x0A); // 2nd row byte
LCD_SendData(0x0A); // 3rd row byte
LCD_SendData(0x0A); // 4th row byte
LCD_SendData(0x00); // 5th row byte
LCD_SendData(0x11); // 6th row byte
LCD_SendData(0x0E); // 7th row byte
LCD_SendData(0x00); // 8th row byte
// Select display RAM & set address to 0
LCD_SendCmd(0x80); // First character
LCD_SendData(0x00); // Display the font
*/
