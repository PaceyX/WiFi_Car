#include "app_motion.h"
#include "motor_bsp.h"



MotorControlType MotoCtrl;
MotionStatus_Type CurrentMotionStatus = MS_IDLE, NextMotionStatus = MS_IDLE;


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


void MotionCtrlTask(void)
{
	NextMotionStatus = MS_SPEED_OPEN;
	
	CurrentMotionStatus = NextMotionStatus;
	
	switch(CurrentMotionStatus)
	{
		case MS_IDLE:
			MotoCtrl.lpwm = 0;
			MotoCtrl.rpwm = 0;
			MotoCtrl.ctrl_mode = MC_FREE;
			break;
		case MS_STEP:
			
			break;
		case MS_SPEED_LOOP:
			
			break;
		case MS_SPEED_OPEN:
			MotoCtrl.lpwm = 0;
			MotoCtrl.rpwm = 0;
			MotoCtrl.ctrl_mode = MC_PWM;
			break;
		case MS_TEST:
			
			break;
	}
	
	MotionProcess();
}
