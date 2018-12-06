/* HAL */
#include "stm32f4xx_hal.h"
#include "usart.h"

/* USER*/
#include "app_tim.h"
#include "app_comm1.h"
#include "app_comm2_wifi.h"
#include "esp8266_bsp.h"


void AppMainLoop(void)
{
	while(1)
	{
		RunFlagHandleTask();
		Comm2_SendToWifiTask();
		Comm1_Task();
		Comm2_Task();
	}
}

char rece[20];

void UserInit(void)
{
	Comm1_Init(&huart1);
	Comm2_Init(&huart2);
	
	RunFlagInit();
	
//	Esp8266_Send_Cmd("AT", rece, ReceiveAck, 10);
}
