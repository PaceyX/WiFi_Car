/**
******************************************************************************
* @file    comm_usart.h
* @author  Inmotion NaviPanel team
* @date    2016/09/14
* @brief   通讯用串口驱动
* @attention Copyright (C) 2016 Inmotion Corporation
******************************************************************************
*/

#include "usart_bsp.h"


/**
* @brief  通讯用串口初始化
* @param  hcomm : 句柄
* @param  huart : 串口句柄
* @retval None
*/
void CommUsart_Init(CommUsartType *hcomm, UART_HandleTypeDef *huart)
{
    hcomm->huart = huart;
    hcomm->tx_tc_flag = __HAL_DMA_GET_TC_FLAG_INDEX(huart->hdmatx);			/* Transfer complete flag: TC = 1 (TX Line). */
    hcomm->offset = 0;
    
    __HAL_UART_CLEAR_OREFLAG(huart);										/* Clear SR and DR Register. */
    HAL_DMA_Start(huart->hdmarx, \
				(uint32_t)&huart->Instance->DR, \
				(uint32_t)hcomm->dma_rx_buffer, \
				hcomm->buffer_size);										/* Config DMA. */
    huart->Instance->CR3 |= USART_CR3_DMAR;									/* Enable Reception by setting the DMAR bit in CR3 Register. */
}

/**
* @brief  通讯串口发送数据
* @param  hcomm : 句柄
* @param  data  : 数据指针
* @param  len   : 数据长度
* @retval 1 发送成功
*/
uint8_t CommUsart_SendData(CommUsartType *hcomm, const uint8_t *data, uint16_t len)
{
    return USER_UART_Transmit_DMA(hcomm->huart, data, len, hcomm->tx_tc_flag) == HAL_OK;
}

/**
* @brief  通讯串口是否允许发送
* @param  hcomm : 句柄
* @retval true 可以发送
*/
bool CommUsart_CanSendData(CommUsartType *hcomm)
{
    if(hcomm->huart->hdmatx->State != HAL_DMA_STATE_BUSY 
        || __HAL_DMA_GET_FLAG(hcomm->huart->hdmatx, hcomm->tx_tc_flag))
    {
        return true;
    }
    return false;
}

/**
* @brief  通讯串口接收数据
* @param  hcomm : 句柄
* @param  pbuf  : 返回接收到的数据指针
* @param  plen  : 返回接收到的数据长度
* @retval 是否有数据
*/
bool CommUsart_RecvData(CommUsartType *hcomm, uint8_t **pbuf, uint32_t* plen)
{
    uint16_t data_cnt = hcomm->buffer_size - __HAL_DMA_GET_COUNTER(hcomm->huart->hdmarx);	/* Get The Data has Received. */
    *pbuf = &hcomm->dma_rx_buffer[hcomm->offset];										/* Make sure address the data will be stored. */
    if(data_cnt < hcomm->offset)
    {
        *plen = hcomm->buffer_size - hcomm->offset;
        hcomm->offset = 0;
    }
    else
    {
        *plen = data_cnt - hcomm->offset;
        hcomm->offset = data_cnt;
    }
    
    if(*plen > 0) return true;
    
    return false;
}

/**
* @brief  通讯串口接收指定长度数据
* @param  hcomm : 句柄
* @param  buf   : 保存接收数据的 buffer
* @param  len   : 需要接收的数据长度
* @retval 是否有数据
*/
bool CommUsart_RecvDataByLength(CommUsartType *hcomm, uint8_t *buf, uint32_t len, uint32_t timeout)
{
    uint32_t tick = HAL_GetTick();
    uint32_t i;
    
    for(i=0; i < len;)
    {
        uint16_t data_cnt = hcomm->buffer_size - __HAL_DMA_GET_COUNTER(hcomm->huart->hdmarx);
        
        if(data_cnt != hcomm->offset)
        {
            buf[i++] = hcomm->dma_rx_buffer[hcomm->offset++];
            if(hcomm->offset >= hcomm->buffer_size)
            {
                hcomm->offset = 0;
            }
        }
        
        if(timeout != HAL_MAX_DELAY && HAL_GetTick() - tick > timeout)
        {
            return false;
        }
    }
    
    return true;
}

/**
* @brief  使能串口接收中断
* @param  hcomm : 句柄
* @param  en    : true 使能
* @retval None
*/
void CommUsart_EnableIT(CommUsartType *hcomm, bool en)
{
    if(en)
    {
        __HAL_UART_ENABLE_IT(hcomm->huart, UART_IT_RXNE);
    }
    else
    {
        __HAL_UART_DISABLE_IT(hcomm->huart, UART_IT_RXNE);
    }
}

/**
* @brief  串口 DMA 发送接口
* @param  huart      : 串口句柄
* @param  pData      : 数据指针
* @param  Size       : 数据长度
* @param  tx_tc_flag : 发送完成 flag 偏移
* @retval None
*/
HAL_StatusTypeDef USER_UART_Transmit_DMA(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t tx_tc_flag)
{
    if(__HAL_DMA_GET_FLAG(huart->hdmatx, tx_tc_flag))
    {
		if(huart->gState == HAL_UART_STATE_BUSY_TX_RX)
		{
			huart->gState = HAL_UART_STATE_BUSY_TX;
		}
		else
		{
			huart->gState = HAL_UART_STATE_READY;
		}
		
		__HAL_UNLOCK(huart->hdmatx);
		__HAL_DMA_CLEAR_FLAG(huart->hdmatx, tx_tc_flag);
    }
    return HAL_UART_Transmit_DMA(huart, (uint8_t*)pData, Size);
}



