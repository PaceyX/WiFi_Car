/**
******************************************************************************
* @file    navipack_api.c
* @author  Jalon
* @date    2016.07.01
* @brief   通讯协议接口，该文件在移植过程中需要根据实际情况更改，
           部分函数需要用户实现
* @attention Copyright (C) 2016 Inmotion Corporation
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
		case 0:
			
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
bool Navipack_TxCallback(u8* pbuf, u16 len)
{
    // 实际数据发送
//    return Comm_SendData(pbuf, len);
	return true;
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
