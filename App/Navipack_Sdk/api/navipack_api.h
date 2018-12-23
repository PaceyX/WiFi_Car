/**
******************************************************************************
* @file    navipack_api.h
* @author  *
* @date    *
* @brief   通讯协议相关声明
* @attention Copyright (C)
******************************************************************************
*/
#ifndef  __NAVIPACK_API_H__
#define  __NAVIPACK_API_H__
/** @addtogroup MCU_SDK_API
* @{
*/

#include "navipack_session_layer.h"

bool NaviPack_Init(void);
bool NaviPack_TxProcessor(NavipackComm_Type *comm, NaviPack_HeadType *head);
bool NaviPack_RxProcessor(NavipackComm_Type *comm, u8 data);
bool Navipack_CheckLength(NaviPack_HeadType *head, u16 len);

/**
* @}
*/
#endif
