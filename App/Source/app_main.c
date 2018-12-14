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

//u8 send[10] = {"abcdefg\n"};
//u32 flag;
void AppMainLoop(void)
{
	while(1)
	{
		RunFlagHandleTask();
		Comm2_SendToWifiTask();
		Comm1_Task();
		Comm2_Task();
//		HAL_Delay(100);
//		Comm1_SendData(send, sizeof(send));
//		Comm2_SendData(send, sizeof(send));
	}
}

char rece[20];
 
void UserInit(void)
{
	RunFlagInit();
	IIC_GPIO_Init();
	Comm1_Init(&huart1);
	Comm2_Init(&huart2);
}
