#ifndef __ULTRASONIC_BSP_H__
#define __ULTRASONIC_BSP_H__


#include "mcu_lib.h"


typedef enum {
	ULTRA_FRONT = 0,
	ULTRA_BACK = 1,
	ULTRA_LEFT = 2,
	ULTRA_RIGHT = 3,
	ULTRA_AMOUNT = 4
} UltraTypeDef;

void BSP_Ultrasonic_Init(void);
void UltraCalDistance_IRQHandler(void);

#endif
