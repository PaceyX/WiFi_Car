/**
******************************************************************************
* @file    navipack_protocol.h
* @author  *
* @date    *
* @brief   通讯协议会话层相关数据结构定义
* @attention Copyright (C) 
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
	u32 switchControl;
	u8 cec_mode;
	u32 led;
}ChassisControlRegister;

// Master Control mode
#define IDLE_MODE       0
#define SPEED_MODE      1
#define CHARGE_MODE     2
#define STEP_MODE	    3

typedef struct
{
	s32 angularPos;
	s32 leftEncoderPos;      ///<当前左边里程计的积分位置
	s32 rightEncoderPos;     ///<当前右边里程计的积分位置
	s32 lineVelocity;
	s32 angularVelocity;
	u8 chargeStatus;
	u8 batteryStatus;
	u16 errorState;
	u32 systick_ms;
	s16 skid_error;
	u8 motion_status;
	u8 motor_control_status;
	s32 l_speed;
	s32 r_speed;
	s32 lpwm;
	s32 rpwm;
	
}ChassisStatusRegister;

typedef struct 
{
	u16 ultraFront;
	u16 ultraBack;
	u16 ultraLeft;
	u16 ultraRight;
	u16 ultrasonicRese[4];
	s16 temperature;
	s16 gyro_pitch;
	s16 gyro_roll;
	s16 gyro_yaw;
	s16 accel_pitch;
	s16 accel_roll;
	s16 accel_vert;
}ChassisSensorRegister;

typedef struct 
{
	u8 cameraData[5 * 1024];
}ChassisCameraRegister;

typedef struct
{
	s8 coor_x[240];
	s8 coor_y[240];
	s16 angular_yaw;
	s8 position_x;
	s8 position_y;
}ChassisMapRgister;

typedef struct
{
	u16 wheel_p;
	u16 wheel_i;
	u16 wheel_d;
}ChassisParamterRegister;


#pragma pack(pop)



typedef NacipackProtocolHeader NaviPack_HeadType;
typedef ChassisControlRegister NaviPack_CtrlType;
typedef ChassisStatusRegister  NaviPack_StatusType;
typedef ChassisSensorRegister  NaviPack_SensorType;
typedef ChassisCameraRegister  NaviPack_CameraType;
typedef ChassisMapRgister	   NaviPack_MapDataType;
typedef ChassisParamterRegister NaviPack_Paramter;


#define NAVIPACK_SLAVE_ID 	0x12

/* Function Code - ID. */
#define MCU_CONTROL_READ_REG    0x01
#define MCU_CONTROL_WRITE_REG	0x02
#define MCU_STATUS_REG	 		0x03
#define MCU_SENSOR_REG			0x04
#define MCU_CAMERA_REG			0x05
#define MCU_MAP_RAG				0x06
#define MCU_PARAM_READ_REG		0x07
#define MCU_PARAM_WRITE_REG		0x08



#endif

