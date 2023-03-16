/********************************************************************************
* Name: Ultrasonic.h (interface)
* Author(s): Noah Grant, Wyatt Richard
* Date: March 17, 2023
* Description: Ultrasonic sensor functions.
********************************************************************************/

#ifndef __Ultrasonic_H
#define __Ultrasonic_H

#include "stm32f303xe.h"

void Ultra_InitTrigger(void);
void Ultra_StartTrigger(void);
void Ultra_InitEcho(void);
void Ultra_EchoIRQ(void);
uint32_t Ultra_ReadSensor(void);

#endif
