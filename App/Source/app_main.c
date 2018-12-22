/* HAL */
#include "stm32f4xx_hal.h"
#include "usart.h"

/* USER*/
#include "app_tim.h"
#include "app_comm1.h"
#include "app_comm2_wifi.h"
#include "esp8266_bsp.h"
#include "ov9650_bsp.h"
#include "iic_soft_bsp.h"

void AppMainLoop(void)
{
	while(1)
	{
		RunFlagHandleTask();
//		Comm2_SendToWifiTask();
		Comm1_Task();
		Comm2_Task();
		HAL_Delay(500);
		ESP8266_SendString("sddfdfagfdgadfgadfgdadfgsdfgsdfgsdfhsdfhsdfhsdfhsdfhsdfhsdhf\n");
	}
}


void UserInit(void)
{
	RunFlagInit();
	HAL_GPIO_WritePin(GPIOB, NETLINK_Pin, GPIO_PIN_SET);
	IIC_GPIO_Init();
	Comm1_Init(&huart1);
	Comm2_Init(&huart2);
	
	HAL_Delay(100);
	Esp8266_Init();

}
