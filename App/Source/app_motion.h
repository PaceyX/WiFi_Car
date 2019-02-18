#ifndef __APP_MOTION_H__
#define __APP_MOTION_H__


#include "mcu_lib.h"




typedef enum{
	MC_FREE,		/* slow stop. */
	MC_BREAK,		/* stop */
	MC_PWM,			/* open */
	MC_SPEED,		/* loop */
}MotorCtrlModeType;

typedef struct
{
	MotorCtrlModeType ctrl_mode;
	int32_t target_v;
	int32_t target_w;
	int32_t cur_v;
	int32_t cur_w;
	int32_t lpwm;
	int32_t rpwm;
}MotorControlType;

/* Sync with master. */
typedef enum{
	MS_IDLE,
	MS_STEP,
	MS_SPEED_LOOP,
	MS_SPEED_OPEN,
	MS_TEST,
}MotionStatus_Type;

typedef enum{
	STEP_INIT,
	STEP_RUN,
	STEP_FINISHED,
} StepStatueTypedef;


void MotionCtrlTask(void);

#endif
