#include "motor_bsp.h"
#include "tim.h"



void Motor_Init(void)
{
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
	
	TIM2->CCR1 = 0;
	TIM2->CCR2 = 200;
	TIM3->CCR3 = 0;
	TIM3->CCR4 = 200;
}
