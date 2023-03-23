/********************************************************************************
* Name: DCMotor.h (interface)
* Author(s): Noah Grant, Wyatt Richard
* Date: March 24, 2023
* Description: DC motor functions.
********************************************************************************/

#ifndef DCMOTOR_H
#define	 DCMOTOR_H

#include "stm32f303xe.h"

#define DCMOTOR_LEFT 0UL
#define DCMOTOR_RIGHT	1UL
#define DCMOTOR_STOP 0UL
#define DCMOTOR_FWD	1UL
#define DCMOTOR_BWD	2UL

void DCMotor_Init(void);
void DCMotor_SetSpeed(uint8_t motor, uint16_t dutyCycle);
void DCMotor_SetDir(uint8_t motor, uint8_t dir);

#endif
