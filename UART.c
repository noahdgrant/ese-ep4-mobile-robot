/******************************************************************************
* Name: UART.c (implementation)
* Author(s): Noah Grant, Wyatt Richard
* Date: January 25, 2023
* Description: UART functions to initialize, configure, and Tx/Rx.
******************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include "UART.h"
#include "stm32f303xe.h"


/******************************************************************
*									LOCAL CONSTANTS AND VARIABLES									  *
******************************************************************/

#define BAUD_RATE 9600
#define UART_MAX_BUFF_SIZE 100


/******************************************************************
*												PRIVATE FUNCTIONS													*
******************************************************************/

/****************************************************
* UART2_config() - Configure UART2 message settings.
* No inputs.
* No return value.
****************************************************/
static void UART2_Config(void){
	// 1. Disable UART2 (set UE on CR1 to 0)
		// USART2 -> CR1, clear UE bit
	USART2->CR1 &= ~USART_CR1_UE;
	
	// 2. Set the baud rate register (ie. clock division regsiter) (BRR) to hit 9600 baud
		// USART2 -> BRR = System Clock Rate / Baud Rate
	USART2->BRR = SystemCoreClock / BAUD_RATE;
	
	// 3. Configure data size (8bit), start bit (1), stop bit (1/2/1.5), parity bit (no parity, even / odd parity)
		// USART2 -> CR1, use M mask OR focus on bit 12 and 28.  They are M0 amd M1
	  //   Set to 00 to make data frame size 8-bit
	USART2->CR1 &= ~USART_CR1_M;
	
		// OVER8 setup (stick with 16x)
		// USART2 -> CR1, bit OVER8 set to 0
	USART2->CR1 &= ~USART_CR1_OVER8;
	
		// USART2 -> CR2, STOP set to 00 (1 bit), 01 (0.5 bit), 10 (2 bits), 11 (1.5 bit)
	USART2->CR2 &= ~USART_CR2_STOP;
	
	// 4. Enable transmit and receive block (TE and RE)
		// USART2 -> CR1, set TE and RE
	USART2->CR1 |= USART_CR1_TE;	// Enable transmitter
	USART2->CR1 |= USART_CR1_RE;	// Enable receiver
	
	// 5. Enable UART2 (set UE and CR1 to 1)
		// USART2 -> CR1, set CR1
	USART2->CR1 |= USART_CR1_UE;
		
	// 6. Wait for the UART2 clock to boot up and get ready
	while((USART2->ISR & USART_ISR_TEACK) == 0);	// Wait till Transmitter is ready to go
	while((USART2->ISR & USART_ISR_REACK) == 0);	// Wait till Receiver is ready to go
}


/******************************************************************
*												PUBLIC FUNCTIONS													*
******************************************************************/

/******************************************
* UART2_Init() - Initialize UART2 setting.
* No inputs.
* No return value.
******************************************/
void UART2_Init(void){
	// Enable PA2 and PA3 on AF7 for UART2 Comm
	
	// 1. Enable APB1, so it is now driven by the scaled clock
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;	// RCC->APB1ENR |= (0x1UL << (1 * 17));
	
	// 1.1 Need to choose UART clock source (so to drive the UART subsystem)
	// Select USART2 clock source (Want to use SYSCLK)
		// 00: PCLK (peripheral crystal clock (not available)
		// 01: System clock (SYSCLK)
		// 10: HSI16 clock source (research into what this is)
		// 11; LSE clock source (research into what this is)
	RCC->CFGR3 &= ~RCC_CFGR3_USART2SW_1;	// clear the MSB: (RCC->CFGR3 &= ~(0x3UL << 16))
	RCC->CFGR3 |= RCC_CFGR3_USART2SW_0;		// Set the LSB to 1: (RCC->CFGR3 |= (0x1UL << 16))
	
	// 2. Set MODER to AF for PA2 and PA3
	// 2.1 Turn on AHB so GPIOs are ON (AHB: Advanced High-performance Bridge)
	// Enable GPIO clock. (Port A is most commonly used)
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	
	// 2.2 Configure the desired pins - we will use PA2 and PA3
	//    PA.2 => USART2 TX (on AF7)
	//    PA.3 => USART2 RX (on AF7)
	// Set AF: 10
	GPIOA->MODER &= ~(3UL << (2*2));		// Clear GPIOA Pin 2 Mode bits
	GPIOA->MODER &= ~(3UL << (2*3));		// Clear GPIOA Pin 3 Mode bits
	GPIOA->MODER |= (2UL << (2*2));		// Set GPIOA Pin 2 Mode bits
	GPIOA->MODER |= (2UL << (2*3));		// Set GPIOA Pin 3 Mode bits
	
	// 3. Alternate Function register for PA2 and PA3. Set them to 07 (AF7)
		// Zero in on the AFRL (AFR[0]), set AF7 on AFR[0] to be 0x7 (AF7)
	GPIOA->AFR[0] |= (7UL << (4*2));	// Set PA2 to AF7	(each AFR is 4 bits)
	GPIOA->AFR[0] |= (7UL << (4*3));	// Set PA3 to AF7 (each AFR is 4 bits)
	
	// 4. Output speed register (OSPEEDR) set it to LOW speed (we are on APB, ie. slower south bridge)
		// 00: Low
		// 01: Medium
		// 10: Fast
		// 11: High
	// Set to 00 by clearing the bits
	GPIOA->OSPEEDR &= ~(3UL << (2*2));	// Low speed for PA2
	GPIOA->OSPEEDR &= ~(3UL << (2*3));	// Low speed for PA3
	
	// 5. Pull-up / pull-down register (PUPDR) set it to No Pull
	// PUPDR has 2 bits per pin
		// No Pull = 00
	// Set to 00 by clearing the bits
	GPIOA->PUPDR &= ~(3UL << (2*2));	// Set PA2 to 'no pull'
	GPIOA->PUPDR &= ~(3UL << (2*3));	// Set PA3 to 'no pull'
	
	// 6. Output type regsiter (OTYPER) set it to push-pull
		// Push Pull = 0
	GPIOA->OTYPER &= ~(1UL << (1*2)); // Set PA2 to 'push-pull'
	GPIOA->OTYPER &= ~(1UL << (1*3));	// Set PA3 to 'push-pull'
	
	// Configure UART2
	UART2_Config();
}

/**************************************************************
* UART_putc() - Write a char to transmit data register.
* c	- Char to transmit.
* No return value.
**************************************************************/
void UART_putc(char c){
	// Wait TXE (TX empty) is set
	while((USART2->ISR & USART_ISR_TXE) == 0);
	
	// Write char to the trasmit data register
	// Writing USART_TDR automatically clears the TXE flag
	USART2->TDR = (uint8_t)c;
}

/********************************************************
* UART_puts() - Write string to transmit data register.
* str		- String to transmit.
* No return value.
********************************************************/
void UART_puts(char *str){
	// Don't send trailing NULL char
	while(*str){
		UART_putc(*str++);
	}
}

/*******************************************************
* UART_getc() - Get char from user.
* No inputs.
* Returns a char.
*******************************************************/
char UART_getc(void){
	// Wait until RXNE flag is set
	while((USART2->ISR & USART_ISR_RXNE) == 0);
	
	// Read char from the receive data regsiter
	// Reading USART_RDR automatically clears the RXNE flag
	return((char)USART2->RDR);
}

/*******************************************************
* UART_getcNB() - Get char from user (non-blocking).
* No inputs.
* Returns a char.
*******************************************************/
char UART_getcNB(void){
	// If RXNE flag is set
	if(USART2->ISR & USART_ISR_RXNE){
		// Read char from the receive data regsiter
		// Reading USART_RDR automatically clears the RXNE flag
		return((char)USART2->RDR);
	}
	else{
		return('\0');
	}
}

/*******************************************************
* UART_printf() - Formats and transmits string.
* fmt		- String to transmit.
* No return value.
*******************************************************/
void UART_printf(char* fmt, ...){
	char buff[UART_MAX_BUFF_SIZE];
	
	// Instructions for function with variable argument list in W2 slides
	va_list args;
	
	// 1. Call va_start with local variable and the name of the last fixed parameter 
	va_start(args, fmt);
	
	// 2.
	vsnprintf(buff, UART_MAX_BUFF_SIZE, fmt, args);
	
	// 3. Call va_end() with your local variable when finished to clean up
	va_end(args);
	
	// Transmit UART buff
	UART_puts(buff);
}
