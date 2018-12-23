/**
******************************************************************************
* @file    navipack_api.c
* @author  *
* @date    *
* @brief   通讯协议接口，该文件在移植过程中需要根据实际情况更改，
           部分函数需要用户实现
* @attention Copyright (C)
******************************************************************************
*/

/** @addtogroup Navipack_MCU_SDK
* @{
*/

/** @defgroup MCU_SDK_API SDK API
* @brief Navipack MCU SDK API
* @{
*/

#include "navipack_api.h"

// 引入需要的头文件
#include "string.h"
#include "app_comm2_wifi.h"
#include "app_comm1.h"
#include "esp8266_bsp.h"


/**
* @brief  初始化
* @retval 是否成功
*/
bool NaviPack_Init(void)
{
    // 用户可添加初始化处理
    return true;
}

/**
* @brief  通讯接收数据处理函数
* @param  comm : 通讯对象
* @param  data : 接收数据，1 byte
* @retval 是否成功处理了数据包
*/
bool NaviPack_RxProcessor(NavipackComm_Type *comm, u8 data)
{
    return NaviPack_SessionRxProcessor(comm, data);
}

/**
* @brief  接收到合法的寄存器请求时回调
* @param  comm : 通讯结构指针
* @param  head : 数据指针
* @retval 是否处理了请求
*/
bool Navipack_RxCallback(NavipackComm_Type *comm, NaviPack_HeadType *head)
{
    switch(head->functionCode)
    {
		case MCU_CONTROL_READ_REG:
			Comm2_PostTxEvent(head);
			break;
		case MCU_CONTROL_WRITE_REG:
			if(!RegisterWrite(comm , head, (u8*)&comm->control, sizeof(comm->control), 0))		return false;
			break;
		case MCU_PARAM_READ_REG:
			Comm2_PostTxEvent(head);
			break;
		case MCU_PARAM_WRITE_REG:
			if(!RegisterWrite(comm , head, (u8*)&comm->paramter, sizeof(comm->paramter), 0))		return false;
			break;
	}
    return true;
}

/**
* @brief  通讯发送数据处理函数
* @param  comm : 通讯对象
* @param  head : 接收数据，单 byte
* @retval 是否成功处理了数据包
*/
bool NaviPack_TxProcessor(NavipackComm_Type *comm, NaviPack_HeadType *head)
{
    if(head->deviceAddr == 0x15)
    {
        switch(head->functionCode)
        {
			case MCU_CONTROL_READ_REG:
				return RegisterRead(comm, head, 0, (u8*)&comm->control, sizeof(comm->control), 0);
			case MCU_STATUS_REG:
				return RegisterRead(comm, head, 0, (u8*)&comm->status, sizeof(comm->status), 0);
			case MCU_SENSOR_REG:
				return RegisterRead(comm, head, 0, (u8*)&comm->sensor, sizeof(comm->sensor), 0);
			case MCU_CAMERA_REG:
				return RegisterRead(comm, head, 0, (u8*)&comm->camera, sizeof(comm->camera), 0);
			case MCU_MAP_RAG:
				return RegisterRead(comm, head, 0, (u8*)&comm->map, sizeof(comm->map), 0);
			case MCU_PARAM_READ_REG:
				return RegisterRead(comm, head, 0, (u8*)&comm->paramter, sizeof(comm->paramter), 0);
		}
	}
    return false;
}

/**
* @brief  实际发送数据的函数
* @param  pbuf : 数据指针
* @param  len  : 数据长度
* @retval 是否成功发送
*/
bool Navipack_TxCallback(u8* pbuf, u16 len, u8 commport)
{
	if(commport == 1)	//usart1
		return Comm1_SendData(pbuf, len);
	else if(commport == 2)		//usart2
		return ESP8266_SendString( (char *)pbuf, len);
	else
		return false;
}

/**
* @brief  实际发送数据的函数
* @param  head : 数据头指针
* @param  len  : 数据内容长度
* @retval 是否成功发送
*/
bool Navipack_CheckLength(NaviPack_HeadType *head, u16 len)
{
    return ((head->len == len - sizeof(NaviPack_HeadType) - 2)//CleanPack will attach Cmd_id to Most instructions
						|| (head->len == len - sizeof(NaviPack_HeadType)));
}

/**
* @}
*/

/**
* @}
*/
