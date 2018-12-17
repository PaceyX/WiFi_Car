#ifndef __USART_BSP_H__
#define __USART_BSP_H__

#include "usart.h"
#include <stdbool.h>


#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))



typedef struct{
    UART_HandleTypeDef *huart;			/* Specify handle. */
    uint32_t tx_tc_flag;						/* Transmittion Complete flag. */
    uint8_t* dma_rx_buffer;					/* DMA Receive Buffer. */
    uint16_t buffer_size;					/* DMA Receive Buffer Size. */
    uint32_t offset;							/* Offset specify. */
}CommUsartType;



void CommUsart_Init(CommUsartType *hcomm, UART_HandleTypeDef *huart);
uint8_t CommUsart_SendData(CommUsartType *hcomm, const uint8_t *data, uint16_t len);
bool CommUsart_CanSendData(CommUsartType *hcomm);
bool CommUsart_RecvData(CommUsartType *hcomm, uint8_t **pbuf, uint32_t* plen);
bool CommUsart_RecvDataByLength(CommUsartType *hcomm, uint8_t *buf, uint32_t len, uint32_t timeout);
void CommUsart_EnableIT(CommUsartType *hcomm, bool en);
HAL_StatusTypeDef USER_UART_Transmit_DMA(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size);






#endif
