/**
******************************************************************************
* @file    PID_regulators.h
* @author  Inmotion NaviPanel team
* @date    2016/09/14
* @brief   PID 计算
* @attention Copyright (C) 2016 Inmotion Corporation
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
 
#ifndef __PID_REGULATORS__H
#define __PID_REGULATORS__H

/* Includes ------------------------------------------------------------------*/

#include "mcu_lib.h"
typedef struct PIDObjTypStruct
{
	s32 kp;     
	s32 ki;
	s32 kd;
	s64 ki_plus;
	s32 outabslimit;
	s32 remainder;
	s32 lasterr[2];
	s64 max_ki_plus;
}PIDObjTyp;

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void PIDInit(PIDObjTyp *ppid, u32 p, u32 i, u32 d);
void PIDSetParams(PIDObjTyp *ppid, u32 p, u32 i, u32 d);
s32 PIDRegulatorS32(s32 aim, s32 cur, s16 out, PIDObjTyp *ppid);
s32 PIDRegulatorS32_Pos(s32 aim, s32 cur, PIDObjTyp *ppid);
void PIDReset(PIDObjTyp* ppid);

/* Exported variables ------------------------------------------------------- */

#endif 

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
