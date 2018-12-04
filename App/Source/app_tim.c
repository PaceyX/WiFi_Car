#include "stm32f4xx_hal.h"
#include "app_tim.h"

static uint32_t RunTick;
SYSFlagType RunFlag;

#define SYS_RUN_FLAG(time)	do{\
	RunFlag.ms##time = 0;	\
	RunFlag.ms##time##Cnt = tick;	\
}while(0)

#define TIME_FLAG(time)		TimeCalc(time, &(RunFlag.ms##time),&(RunFlag.ms##time##Cnt))
void TimeCalc(uint32_t time_factor, uint8_t * p_flag, uint32_t * p_cnt)
{ 
	if(((RunTick - *p_cnt) >= time_factor))
	{
		*p_flag = 1;
		*p_cnt += time_factor;
	}
	else
	{
		*p_flag = 0;
	}
}

void RunFlagInit(void)
{
	uint32_t tick = HAL_GetTick();			//Get current time. HAL_GetTick();
	
	SYS_RUN_FLAG(1);			//init value.
	SYS_RUN_FLAG(3);
	SYS_RUN_FLAG(4);
	SYS_RUN_FLAG(20);
	SYS_RUN_FLAG(80);
	SYS_RUN_FLAG(100);
	SYS_RUN_FLAG(250);
	SYS_RUN_FLAG(500);
	SYS_RUN_FLAG(1000);
}

void RunFlagHandleTask(void)
{
	RunTick = HAL_GetTick();
	
	TIME_FLAG(1);
	TIME_FLAG(3);
	TIME_FLAG(4);
	TIME_FLAG(20);
	TIME_FLAG(80);
	TIME_FLAG(100);
	TIME_FLAG(250);
	TIME_FLAG(500);
	TIME_FLAG(1000);
	
	if(RunFlag.ms1000)	HAL_GPIO_TogglePin(GPIOA, LED0_Pin);
}



