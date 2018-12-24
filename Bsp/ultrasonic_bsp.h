#ifndef __ULTRASONIC_BSP_H__
#define __ULTRASONIC_BSP_H__


#include "mcu_lib.h"




typedef enum {
	ULTRA_FRONT,
	ULTRA_BACK,
	ULTRA_LEFT,
	ULTRA_RIGHT,
	ULTRA_AMOUNT
} UltraTypeDef;

void UltraCalDistance_IRQHandler(uint16_t GPIO_Pin);





#endif
