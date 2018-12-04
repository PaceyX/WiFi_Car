/**
******************************************************************************
* @file    navipack_protocol.h
* @author  Jalon
* @date    2016.06.16
* @brief   通讯协议会话层相关数据结构定义
* @attention Copyright (C) 2016 Inmotion Corporation
******************************************************************************
*/
#ifndef __NAVIPACK_PROTOCOL_H__
#define __NAVIPACK_PROTOCOL_H__

#include "mcu_lib.h"

#pragma pack(push, 1)

typedef struct
{
    u8 deviceAddr;
    u8 functionCode;
    u16 startAddr;
    u32 len;
}NacipackProtocolHeader;

/** 
  * @brief 控制寄存器
  */ 
typedef struct
{
    s32 lineVelocity;
    s32 angularVelocity;
    u8 mode;
    s32 Vacc;
    s32 Wacc;
    s32 stepS;
    s16 stepPhi;
}ChassisControlRegister;

typedef struct
{
	u32 systick_ms;
	s32 angularPos;
	s32 leftEncoderPos;      ///<当前左边里程计的积分位置
	s32 rightEncoderPos;     ///<当前右边里程计的积分位置
	s32 lineVelocity;
	s32 angularVelocity;
	s16 skid_error;
	s16 gyro_pitch;
	s16 gyro_roll;
	s16 gyro_yaw;
	s16 accel_pitch;
	s16 accel_roll;
	s16 accel_vert;
}FastChassisStatusRegister;

typedef struct     //
{
	u16 ultrasonic[4];
	u32 systick_ms;
	u8 collisionSensor;
	u8 dropSensor;
	u16 irSensor;
	u16 battery_voltage;
	u16 errorState;
	s16 temperature;
	
	u8 chargeStatus;
	u8 batteryStatus;
	u8 real_battery_level;
	u8 motor_control_status;    // mcu 内部状态机值
	u8 motion_status;           // mcu 内部状态机值
	
	s32 l_speed;
	s32 r_speed;
	s32 lpwm;
	s32 rpwm;
	u32 feedbackStatus;

	u32 sensorSwitchStatus;
	
	s32 debug_info1;
	s32 debug_info2;
}SlowChassisStatusRegister;

typedef NacipackProtocolHeader NaviPack_HeadType;
typedef ChassisControlRegister NaviPack_CtrlType;
typedef FastChassisStatusRegister NaviPack_FastStatusType;
typedef SlowChassisStatusRegister NaviPack_SlowStatusType;

#define NAVIPACK_SLAVE_ID 	0x12

/* Function Code - ID. */
#define MCU_CONTROL_REG         0x01
#define MCU_FAST_STATUS_REG	 	0x02
#define MCU_SLOW_STATUS_REG		0x03
//..
#define MCU_SEND_DEBUG_DATA		0x05

#endif

