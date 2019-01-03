#ifndef __MOTOR_BSP_H__
#define __MOTOR_BSP_H__


#include "mcu_lib.h"
#include "pid_regulators.h"


#define MAX_PWM			WHEEL_PWM_PERIOD
#define MIN_PWM			0
#define MAX_MOTOR_NUM	2

#define LEFT_WHEEL_MOTOR        0
#define RIGHT_WHEEL_MOTOR       1

typedef struct MotorsParamsType
{
	__IO u32 *PWM_CHA;
	__IO u32 *PWM_CHB;
	u8 motor_num;							/* 当前电机排序. */
    u8 encoder_dir;             			/* 当前编码器方向. */
    u8 motor_dir;							/* 当前电机的转向，通过IMU的加速度来判断. */
	u8 motor_dir_real;						/* 电机真实的控制方向. */
    s16 velocity_factor;					/* 里程计校准因子. */
	
//	s32 present_speed;						/* 当前车子速度. */
    s32 acccumulated_distance;				/* 当前最新一次的里程计距离. */
    s32 accumulated_distance_remainder;		/* 之前累计的里程计距离. */
	
	s32 velocity;							/* 当前车子速度. */
	s32 last_motor_output;					/* 上一次的电机PWM开环输出. */

	// Speed loop
    s32 targetVelocity;						/* 闭环控制的目标线速度. */
    s32 cur_pwm_out;						/* 闭环控制的当前PWM输出. */
	s32 last_pid_pwm_output;				/* 上一次的电机PWM闭环输出. */
    PIDObjTyp velocityPID;					/* 闭环控制的电机PID参数. */
} MotorParamsTypedef;

extern MotorParamsTypedef MotorParams[2];

typedef struct
{
    int32_t acc;
    int32_t rem;
} AccCtrlType;

void Motor_Init(void);
void WheelCalSpeed_IRQHandler(uint16_t GPIO_Pin);
void Motor_Output(MotorParamsTypedef * pm, s32 out);
void BSP_WHEEL_MOTOR_MotorFreeStop(void);
void BSP_WHEEL_MOTOR_MotorBreakStop(void);
int8_t BSP_WHEEL_MOTOR_PWM_Out(uint8_t idx, int32_t out, AccCtrlType *pacc);
int32_t AccCalc(int32_t cur, int32_t aim, AccCtrlType *acc);


#endif
