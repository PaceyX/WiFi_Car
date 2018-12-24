#ifndef __MOTOR_BSP_H__
#define __MOTOR_BSP_H__


#include "mcu_lib.h"
#include "pid_regulators.h"


typedef struct MotorsParamsType
{
	__IO u32 *PWM_CHA;
	__IO u32 *PWM_CHB;
    u8 encoder_dir;             			/* 当前电机编码器的方向. */
    u8 motor_dir;							/* 电机的目标方向. */
	u8 motor_num;							/* 电机编号定义. */
	u8 motor_dir_real;						/* 电机的真实方向. */
    s16 velocity_factor;					/* 速度校准因子. */
	
	s32 present_speed;						/* 当前脉冲速度. */
    s32 acccumulated_distance;				/* 新的一次里程计数据. */
    s32 accumulated_distance_remainder;		/* 累计的里程计数据. */
	
	s32 last_motor_output;					/* 记录开环上一次PWM输出. */
	s32 last_pid_pwm_output;				/* 记录闭环上一次PWM输出. */
	
	// Speed loop
    s32 velocity;							/* 当前电机线速度. */
    s32 targetVelocity;						/* 当前电机目标线速度. */
    s32 cur_pwm_out;						/* 当前电机PWM输出. */
	
    PIDObjTyp velocityPID;					/* 电机的PID数据赋值. */
} MotorParamsTypedef;

void Motor_Init(void);
void WheelCalSpeed_IRQHandler(uint16_t GPIO_Pin);


#endif
