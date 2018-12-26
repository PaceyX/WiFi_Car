/* HAL */
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "tim.h"


/* USER*/
#include "app_tim.h"
#include "app_comm1.h"
#include "app_comm2_wifi.h"
#include "esp8266_bsp.h"
#include "ov9650_bsp.h"
#include "iic_soft_bsp.h"
#include "motor_bsp.h"
#include "app_task.h"
#include "ultrasonic_bsp.h"



void AppMainLoop(void)
{
	while(1)
	{
		RunFlagHandleTask();
//		Comm2_SendToWifiTask();
		Comm1_Task();
		Comm2_Task();
		Sample_Task();
	}
}


void UserInit(void)
{
	RunFlagInit();
	IIC_GPIO_Init();
	Comm1_Init(&huart1);
	Comm2_Init(&huart2);
	HAL_TIM_Base_Start_IT(&htim6);
	Esp8266_Init();
	Motor_Init();
	
	HAL_GPIO_WritePin(GPIOB, NETLINK_Pin, GPIO_PIN_SET);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	WheelCalSpeed_IRQHandler(GPIO_Pin);
	
}

void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim)
{
//	if(htim == &htim6)	UltraCalDistance_IRQHandler();
		
}
