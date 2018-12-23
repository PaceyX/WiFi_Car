/**
******************************************************************************
* @file    navipack_session_layer.h
* @author  *
* @date    *
* @brief   通讯协议会话层解析相关声明
* @attention Copyright (C)
******************************************************************************
*/
#ifndef __NAVIPACK_SESSION_LAYER_H__
#define __NAVIPACK_SESSION_LAYER_H__
/** @addtogroup Session_Layer
* @{
*/

#include "navipack_type.h"
#include "navipack_transport_layer.h"

#define CHASSIS_PLATFORM
#include "navipack_protocol.h"

#define REG_ID_STATUS   1
#define REG_ID_COTROL   2
#define REG_ID_CONFIG   3
#define REG_ID_USER     4
#define REG_ID_UID      5

typedef struct
{
    // 需要初始化
    u8 *rxBuffer;   ///< 接收 Buffer 指针
    u8 *txBuffer;   ///< 发送 Buffer 指针
    u16 rxSize;     ///< 接收 Buffer 尺寸
    u16 txSize;     ///< 发送 Buffer 尺寸
    
    u16 rxDataLen;
    u16 txDataLen;
    TransportFrame_Type rxFrame;
    TransportFrame_Type txFrame;
	
	u8  commPort;   //choice which usart
	
	NaviPack_CtrlType control;
	NaviPack_StatusType status;
	NaviPack_SensorType sensor;
	NaviPack_CameraType camera;
	NaviPack_MapDataType map;
	NaviPack_Paramter paramter;
	
	u8  Cmd_id[2];  //CleanPack will attach Cmd_id to Most instructions, you need to send it back to CleanPack
}NavipackComm_Type;

bool NaviPack_SessionRxProcessor(NavipackComm_Type *comm, u8 data);
bool RegisterWrite(NavipackComm_Type *comm , NaviPack_HeadType *head, u8 *reg, u32 size, u8 reg_id);
bool RegisterRead(NavipackComm_Type *comm, NaviPack_HeadType *head, u8 err_id, u8 *reg, u32 reg_size, u8 reg_id);
bool Navipack_TransportUnpacking(NavipackComm_Type *comm, u8 data);
bool Navipack_TransportPacking(NavipackComm_Type *comm, u8 *in_buf, u16 len, u8 pack_flag);

bool Navipack_TxCallback(u8* pbuf, u16 len, u8 commport);
bool Navipack_RxCallback(NavipackComm_Type *comm, NaviPack_HeadType *head);

/**
* @}
*/
#endif
