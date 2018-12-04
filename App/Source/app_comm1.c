#include "app_comm1.h"
#include "navipack_api.h"
#include "usart_bsp.h"
#include "queue.h"


#define UART1_BUFF_SIZE  128


NavipackComm_Type NavipackComm1;

static CommUsartType CommUsart;

static QueueType TxQueue;
static NaviPack_HeadType TxQueuePool[10];

static u8 CommTxBuffer[100*2];
static u8 CommRxBuffer[100];
static u8 UartBuffer[UART1_BUFF_SIZE];


/**
* @brief  通讯传输层初始化
* @param  huart: 串口句柄
* @retval 是否成功
*/
bool Comm1_Init(UART_HandleTypeDef *huart)
{
    NavipackComm_Type *comm = &NavipackComm1;

    comm->rxBuffer = CommRxBuffer;
    comm->txBuffer = CommTxBuffer;
    comm->rxSize = sizeof(CommRxBuffer);
    comm->txSize = sizeof(CommTxBuffer);
    comm->rxDataLen = 0;
    comm->txDataLen = 0;

    Queue_Init(&TxQueue, TxQueuePool, sizeof(TxQueuePool), sizeof(TxQueuePool[0]));
    
    NaviPack_Init();
    
    CommUsart.buffer_size = UART1_BUFF_SIZE;
    CommUsart.dma_rx_buffer = UartBuffer;
    
    CommUsart_Init(&CommUsart, huart);

    return true;
}


/**
* @brief  通讯接收及发送处理 Task
* @param  None
* @retval None
*/
void Comm1_Task(void)
{
    u8 *data;
    u32 len, i;
    NaviPack_HeadType head;
    
    // TX
    if(CommUsart_CanSendData(&CommUsart) && Queue_Query(&TxQueue, &head))
    {
        if(NaviPack_TxProcessor(&NavipackComm1, &head))
        {
            Queue_Get(&TxQueue, NULL);
        }
    }
	
    // RX
    if(CommUsart_RecvData(&CommUsart, &data, &len))
    {
        for(i=0; i<len; i++)
        {
//            Comm_RecvPackage(data[i]);
        }
    }
}

/**
* @brief  发送接收
* @param  data : 数据
* @retval None
*/
u8 Comm1_SendData(u8 *data, u16 len)
{
    return CommUsart_SendData(&CommUsart, data, len);
}
