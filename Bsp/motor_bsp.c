#include "motor_bsp.h"
#include "tim.h"




MotorParamsTypedef MotorParams[2] = 
{
	{&(TIM2->CCR1), &(TIM2->CCR2), 0},		/* Left_Wheel. */
	{&(TIM3->CCR3), &(TIM3->CCR4), 1}		/* Right_Wheel. */
};


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

void Motor_Output(MotorParamsTypedef * pm, s32 out)
{
	s16 hTimePhA = 0;
	
	if(out > 0)		/* 正转 */
	{
		hTimePhA = out;
	}
	else if(out < 0)	/* 反转 */
	{
		hTimePhA = -out;
	}
	else			/* stop */
	{
		hTimePhA = 0;
	}
	
	if(hTimePhA > MAX_PWM)
	{
		hTimePhA = MAX_PWM;
	}
	
	if(hTimePhA < MIN_PWM)
	{
		hTimePhA = MIN_PWM;
	}
	
	pm->last_motor_output = out > 0 ? hTimePhA : -hTimePhA;
	
	if(pm->motor_num == 0)		/* left. */
	{
		if(out > 0)
		{
			*pm->PWM_CHA = 1000;
			*pm->PWM_CHB = 0;
		}
		else if(out < 0)
		{
			*pm->PWM_CHA = 0;
			*pm->PWM_CHB = 1000;
		}
		else
		{
			*pm->PWM_CHA = 0;
			*pm->PWM_CHB = 0;
		}
	}
	
	if(pm->motor_num == 1)		/* right. */
	{
		if(out > 0)
		{
			*pm->PWM_CHA = 0;
			*pm->PWM_CHB = 1000;
		}
		else if(out < 0)
		{
			*pm->PWM_CHA = 1000;
			*pm->PWM_CHB = 0;
		}
		else
		{
			*pm->PWM_CHA = 0;
			*pm->PWM_CHB = 0;
		}
	}
}

int32_t AccCalc(int32_t cur, int32_t aim, AccCtrlType *acc)
{
    int32_t step, dp;

    dp = (cur - aim);
    step = acc->acc + acc->rem;
    cur = 1000 * cur + step * (dp > 0 ? -1 : 1);
    acc->rem = abs(cur % 1000);
    cur = abs(dp) * 1000 > step ? (cur / 1000) : aim;
    return cur;
}

int8_t BSP_WHEEL_MOTOR_PWM_Out(uint8_t idx, int32_t out, AccCtrlType *pacc)
{
    if(idx >= MAX_MOTOR_NUM)
    {
        return -1;
    }

    if(pacc == NULL)
    {
        Motor_Output(&MotorParams[idx], out);
    }
    else
    {
        Motor_Output(&MotorParams[idx], AccCalc(MotorParams[idx].last_motor_output, out, pacc));
    }

    return 0;
}

void BSP_WHEEL_MOTOR_MotorFreeStop(void)
{
	static AccCtrlType stop_acc[2] = { { 1000000, 0 }, { 1000000, 0 } };
    Motor_Output(&MotorParams[0], AccCalc(MotorParams[0].last_motor_output, 0, &stop_acc[0]));
    Motor_Output(&MotorParams[1], AccCalc(MotorParams[1].last_motor_output, 0, &stop_acc[1]));
}

void BSP_WHEEL_MOTOR_MotorBreakStop(void)
{
    Motor_Output(&MotorParams[0], 0);
    Motor_Output(&MotorParams[1], 0);
}
