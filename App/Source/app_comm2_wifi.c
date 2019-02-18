#include "app_comm2_wifi.h"
#include "queue.h"
#include "app_tim.h"
#include "app_comm1.h"
#include "navipack_api.h"


#define UART2_BUFF_SIZE  128


NavipackComm_Type NavipackComm2;

CommUsartType CommUsart2;

static QueueType TxQueue;
static NaviPack_HeadType TxQueuePool[10];

static u8 CommTxBuffer[100*2];
static u8 CommRxBuffer[100];
static u8 UartBuffer2[UART2_BUFF_SIZE];

/**
* @brief  通讯传输层初始化
* @param  huart: 串口句柄
* @retval 是否成功
*/
bool Comm2_Init(UART_HandleTypeDef *huart)
{
    NavipackComm_Type *comm = &NavipackComm2;

    comm->rxBuffer = CommRxBuffer;
    comm->txBuffer = CommTxBuffer;
    comm->rxSize = sizeof(CommRxBuffer);
    comm->txSize = sizeof(CommTxBuffer);
    comm->rxDataLen = 0;
    comm->txDataLen = 0;

	comm->commPort  = 2;//use usart1
	
    Queue_Init(&TxQueue, TxQueuePool, sizeof(TxQueuePool), sizeof(TxQueuePool[0]));
    
    NaviPack_Init();
    
    CommUsart2.buffer_size = UART2_BUFF_SIZE;
    CommUsart2.dma_rx_buffer = UartBuffer2;
    
    CommUsart_Init(&CommUsart2, huart);

    return true;
}

/**
* @brief  Wifi 模块通讯接收及发送处理 Task
* @param  None
* @retval None
*/
void Comm2_Task(void)
{
    u8 *data;
    u32 len, i;
    NaviPack_HeadType head;
    
    // TX
    if(CommUsart_CanSendData(&CommUsart2) && Queue_Query(&TxQueue, &head))
    {
        if(NaviPack_TxProcessor(&NavipackComm2, &head))
        {
            Queue_Get(&TxQueue, NULL);
        }
    }
	
    // RX
    if(CommUsart_RecvData(&CommUsart2, &data, &len))
    {
        for(i=0; i<len; i++)
        {
            NaviPack_RxProcessor(&NavipackComm2, data[i]);
        }
    }
}

/**
* @brief  wifi发送接收
* @param  data : 数据
* @retval None
*/
u8 Comm2_SendData(u8 *data, u16 len)
{
    return CommUsart_SendData(&CommUsart2, data, len);
}

/**
* @brief  推送一个发送事件
* @param  handle  : 对象编号
* @param  head    : 数据头指针
* @retval 是否成功
*/
bool Comm2_PostTxEvent(NaviPack_HeadType *head)
{
    return Queue_Put(&TxQueue, head);
};

/**
* @brief  通讯定时发送 Task
* @param  p_arg: 参数
* @retval None
*/
void Comm2_SendToWifiTask(void)
{
    static NaviPack_HeadType Status_head =
    {
        NAVIPACK_SLAVE_ID,
        MCU_STATUS_REG,
        0,
        sizeof(ChassisStatusRegister),
    };
    
    static NaviPack_HeadType Sensor_head =
    {
        NAVIPACK_SLAVE_ID,
        MCU_SENSOR_REG,
        16,
        sizeof(ChassisSensorRegister),
    };
	
    static int32_t startup_boost_dly = 0;
    static int32_t count_num = 0;
	
    if (RunFlag.ms1)
    {
		if(count_num == 6 && (startup_boost_dly >= 3000))
		{
			Comm2_PostTxEvent(&Sensor_head);
		}
		else if (count_num == 12)
		{
			Comm2_PostTxEvent(&Status_head);
			count_num = 0;
		}
		else
		{
			count_num++;
		}
		
		startup_boost_dly++;
    }
}

