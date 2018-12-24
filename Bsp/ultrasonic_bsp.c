#include "ultrasonic_bsp.h"
#include "app_tim.h"


typedef enum {
    LEVEL_LOW = 0,
    LEVEL_HIGH = !LEVEL_LOW,
} LevelTypeDef;

typedef struct {
	GPIO_TypeDef * 		Trig_GPIO;
	uint16_t 			Trig_GPIO_Pin;
	GPIO_TypeDef * 		Echo_GPIO;
	uint16_t 			Echo_GPIO_Pin;
	uint8_t				trig_send;
	IntervalTimeType	echo_start_time;
	IntervalTimeType	echo_end_time;
	uint32_t			interval;
	LevelTypeDef		level;
	uint8_t				read_ok;
	uint16_t			distance;
} UltraDataTypeDef;

static UltraDataTypeDef UltraSensor[ULTRA_AMOUNT] = {
	[ULTRA_FRONT] = {TRIG_F_GPIO_Port, TRIG_F_Pin, ECHO_F_GPIO_Port, ECHO_F_Pin, 0},
	[ULTRA_BACK]  = {TRIG_B_GPIO_Port, TRIG_B_Pin, ECHO_B_GPIO_Port, ECHO_B_Pin, 0},
	[ULTRA_LEFT]  = {TRIG_L_GPIO_Port, TRIG_L_Pin, ECHO_L_GPIO_Port, ECHO_L_Pin, 0},
	[ULTRA_RIGHT] = {TRIG_R_GPIO_Port, TRIG_R_Pin, ECHO_R_GPIO_Port, ECHO_R_Pin, 0}
};

static __inline UltraTypeDef GetUltraPin(uint16_t GPIO_Pin)
{
	for(uint8_t i = 0; i < ULTRA_AMOUNT; i++)
	{
		if(UltraSensor[i].Echo_GPIO_Pin == GPIO_Pin)
		{
			return (UltraTypeDef)i;
		}
	}
	return (UltraTypeDef)0;
}

void UltraCalDistance(UltraTypeDef ultr)
{
	
}

void UltraCalDistance_IRQHandler(uint16_t GPIO_Pin)
{
	UltraTypeDef ultr = GetUltraPin(GPIO_Pin);	
	
	if(ultr == (UltraTypeDef)0)	return;
	else	UltraCalDistance(ultr);
}

