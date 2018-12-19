#ifndef __APP_COMM1_H__
#define __APP_COMM1_H__



#include "mcu_lib.h"


bool Comm1_Init(UART_HandleTypeDef *huart);
void Comm1_Task(void);
u8 Comm1_SendData(u8 *data, u16 len);
void Comm1_printf(char * data);


#endif
