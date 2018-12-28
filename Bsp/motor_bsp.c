#include "motor_bsp.h"
#include "tim.h"


typedef struct MotorsParamsType
{
	__IO u32 *PWM_CHA;
	__IO u32 *PWM_CHB;
    u8 encoder_dir;             			/* 当前编码器方向. */
    u8 motor_dir;							/* 当前电机的转向，通过IMU的加速度来判断. */
	u8 motor_num;							/* 当前电机数量. */
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

MotorParamsTypedef MotorParams[2];


typedef struct MotorSpeedTypedef
{
	
} MotorSpeedTypedef;


void Motor_Init(void)
{
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
	
	TIM2->CCR1 = 0;
	TIM2->CCR2 = 0;
	TIM3->CCR3 = 0;
	TIM3->CCR4 = 0;
}

void WheelCalSpeed_IRQHandler(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == CL_1_LEFT_Pin)
	{
		
	}
	else if(GPIO_Pin == CL_2_RIGHT_Pin)
	{
		
	}
}


