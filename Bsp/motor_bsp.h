#ifndef __MOTOR_BSP_H__
#define __MOTOR_BSP_H__


#include "mcu_lib.h"
#include "pid_regulators.h"




void Motor_Init(void);
void WheelCalSpeed_IRQHandler(uint16_t GPIO_Pin);


#endif
