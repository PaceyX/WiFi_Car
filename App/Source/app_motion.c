#include "app_motion.h"
#include "motor_bsp.h"
#include "app_tim.h"
#include "app_comm2_wifi.h"




MotorControlType MotoCtrl;
MotionStatus_Type LastMotionStatus = MS_IDLE, CurrentMotionStatus = MS_IDLE, NextMotionStatus = MS_IDLE;
StepStatueTypedef StepStatus = STEP_INIT;

/**
*	@brief	应用层最底层运动处理.
*/
void MotionProcess(void)
{
	static AccCtrlType pwm_acc[2] = { { 5000, 0 }, { 5000, 0 } };
	
	switch (MotoCtrl.ctrl_mode)
    {
		case MC_FREE:		/* slow stop. */
			BSP_WHEEL_MOTOR_MotorFreeStop();
			break;
		case MC_BREAK:		/* stop. */
			BSP_WHEEL_MOTOR_MotorBreakStop();
			break;
		case MC_PWM:		/* open control. */
			BSP_WHEEL_MOTOR_PWM_Out(LEFT_WHEEL_MOTOR, MotoCtrl.lpwm, &pwm_acc[0]);
			BSP_WHEEL_MOTOR_PWM_Out(RIGHT_WHEEL_MOTOR, MotoCtrl.rpwm, &pwm_acc[1]);
			break;
		case MC_SPEED:		/* loop control. */
			break;
	}
}

bool StepProcess(bool new_flag)
{
	static bool finished = false;
	
	if(new_flag)
	{
		StepStatus = STEP_INIT;
	}
	
	switch(StepStatus)
	{
		case STEP_INIT:
			
			break;
		case STEP_RUN:
			
			break;
		case STEP_FINISHED:
			
			break;
	}
}


void MotionCtrlTask(void)
{
	if(!RunFlag.ms1)	return;
		
	NextMotionStatus = NavipackComm2.control.mode;		/* Get control mode from master. */
		
	CurrentMotionStatus = NextMotionStatus;
	switch(CurrentMotionStatus)
	{
		case MS_IDLE:
			MotoCtrl.lpwm = 0;
			MotoCtrl.rpwm = 0;
			MotoCtrl.ctrl_mode = MC_FREE;
			break;
		case MS_SPEED_LOOP:
			
			break;
		case MS_SPEED_OPEN:
			MotoCtrl.lpwm = 0;
			MotoCtrl.rpwm = 0;
			MotoCtrl.ctrl_mode = MC_PWM;
			break;
		case MS_STEP:
			
			break;
		case MS_TEST:
			
			break;
	}
	
	LastMotionStatus = CurrentMotionStatus;
	
	MotionProcess();
}
