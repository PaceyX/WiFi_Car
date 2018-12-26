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
	uint8_t 			reading;
	uint8_t				read_ok;
	uint16_t			distance;
} UltraDataTypeDef;

UltraDataTypeDef UltraSensor[ULTRA_AMOUNT] = {
	[ULTRA_FRONT] = {TRIG_F_GPIO_Port, TRIG_F_Pin, ECHO_F_GPIO_Port, ECHO_F_Pin, 0},
	[ULTRA_BACK]  = {TRIG_B_GPIO_Port, TRIG_B_Pin, ECHO_B_GPIO_Port, ECHO_B_Pin, 0},
	[ULTRA_LEFT]  = {TRIG_L_GPIO_Port, TRIG_L_Pin, ECHO_L_GPIO_Port, ECHO_L_Pin, 0},
	[ULTRA_RIGHT] = {TRIG_R_GPIO_Port, TRIG_R_Pin, ECHO_R_GPIO_Port, ECHO_R_Pin, 0}
};

//static __inline UltraTypeDef GetUltraPin(uint16_t GPIO_Pin)
//{
//	for(uint8_t i = 0; i < ULTRA_AMOUNT; i++)
//	{
//		if(UltraSensor[i].Echo_GPIO_Pin == GPIO_Pin)
//		{
//			return (UltraTypeDef)i;
//		}
//	}
//	return (UltraTypeDef)0;
//}

/**
*	@brief	for circular delay function.
*	@note	STM32F446 180M Frequency adjust.
*/
static void delay_us(uint32_t time)
{
	uint8_t i;
	
	while(time--)
	{
		i = 42;
		while(i--);	
	}
}

void UltraDistanceSample(void)
{
	UltraDataTypeDef * ultr;
	
	for(int i = 0; i < ULTRA_AMOUNT; i++)
	{
		ultr = &UltraSensor[ (UltraTypeDef)i ];
		
		HAL_GPIO_WritePin(ultr->Trig_GPIO, ultr->Trig_GPIO_Pin, GPIO_PIN_SET );
	}
	
	delay_us(20);
	
	for(int i = 0; i < ULTRA_AMOUNT; i++)
	{
		ultr = &UltraSensor[ (UltraTypeDef)i ];
		
		HAL_GPIO_WritePin(ultr->Trig_GPIO, ultr->Trig_GPIO_Pin, GPIO_PIN_RESET );
	}
	
	for(int i = 0; i < ULTRA_AMOUNT; i++)
	{
		ultr = &UltraSensor[ (UltraTypeDef)i ];
		
		ultr->trig_send = 1;
		ultr->read_ok = 0;
		ultr->interval = 0;
		ultr->reading = 0;
	}
}

void UltraCalDistance_IRQHandler(void)
{
//	UltraTypeDef ultr_exit_line = GetUltraPin(GPIO_Pin);
	UltraDataTypeDef * ultr;

	
	for(u8 i = 0; i < ULTRA_AMOUNT; i++)
	{
		ultr = &UltraSensor[ (UltraTypeDef)i ];
		
		if(ultr->trig_send == 1)	/* make sure current info sended. */
		{
			if(HAL_GPIO_ReadPin(ultr->Echo_GPIO, ultr->Echo_GPIO_Pin) == 1)
			{
				ultr->interval += 10;		/* per irq is 10us. */
				ultr->reading = 1;
			}
			
			if(ultr->reading == 1 && \
				HAL_GPIO_ReadPin(ultr->Echo_GPIO, ultr->Echo_GPIO_Pin) == 0)
			{
				ultr->distance = ultr->interval * 340 / 2000 / 10;	//cm
				ultr->trig_send = 0;
				ultr->read_ok = 1;
				ultr->reading = 0;
			}
		}
	}
}

uint16_t BSP_GetUltraDistance(UltraTypeDef ultr)
{
	return UltraSensor[ultr].distance;
}
