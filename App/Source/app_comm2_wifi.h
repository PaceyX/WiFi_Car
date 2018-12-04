#ifndef __APP_COMM2_WIFI_H__
#define __APP_COMM2_WIFI_H__

#include <stdbool.h>
#include "usart.h"
#include "mcu_lib.h"
#include "navipack_api.h"
#include "usart_bsp.h"


extern CommUsartType CommUsart2;

bool Comm2_Init(UART_HandleTypeDef *huart);
void Comm2_SendToWifiTask(void);
void Comm2_Task(void);

u8 Comm2_SendData(u8 *data, u16 len);
bool Comm2_PostTxEvent(NaviPack_HeadType *head);

#endif
