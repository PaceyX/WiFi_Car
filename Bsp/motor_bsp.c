#include "motor_bsp.h"
#include "tim.h"

MotorParamsTypedef MotorParams[2];



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

void WheelCalSpeed_IRQHandler(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == CL_1_LEFT_Pin)
	{
		
	}
	else if(GPIO_Pin == CL_2_RIGHT_Pin)
	{
		
	}
}


