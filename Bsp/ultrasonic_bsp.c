#include "ultrasonic_bsp.h"
#include "app_tim.h"

UltraTypeDef CurrentChioce;

typedef enum {
    LEVEL_LOW = 0,
    LEVEL_HIGH = !LEVEL_LOW,
} LevelTypeDef;

typedef struct {
	GPIO_TypeDef * 		Trig_GPIO;
	uint16_t 			Trig_GPIO_Pin;
	GPIO_TypeDef * 		Echo_GPIO;
	uint16_t 			Echo_GPIO_Pin;
	uint8_t				set_start_flag;
	uint8_t				set_end_flag;
	IntervalTimeType	echo_start_time;
	IntervalTimeType	echo_end_time;
	uint16_t			interval;
	uint8_t recvive_signal_flag;
	uint8_t 			reading;
	
	uint16_t			distance;
} UltraDataTypeDef;

UltraDataTypeDef UltraSensor[ULTRA_AMOUNT] = {
	[ULTRA_FRONT] = {TRIG_F_GPIO_Port, TRIG_F_Pin, ECHO_F_GPIO_Port, ECHO_F_Pin, 0, 1},
	[ULTRA_BACK]  = {TRIG_B_GPIO_Port, TRIG_B_Pin, ECHO_B_GPIO_Port, ECHO_B_Pin, 0, 1},
	[ULTRA_LEFT]  = {TRIG_L_GPIO_Port, TRIG_L_Pin, ECHO_L_GPIO_Port, ECHO_L_Pin, 0, 1},
	[ULTRA_RIGHT] = {TRIG_R_GPIO_Port, TRIG_R_Pin, ECHO_R_GPIO_Port, ECHO_R_Pin, 0, 1}
};

__inline UltraTypeDef GetUltraPin(uint16_t GPIO_Pin)
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

void BSP_Ultrasonic_Init(void)
{
	UltraDataTypeDef * ultr;
	
	for(int i = 0; i < ULTRA_AMOUNT; i++)
	{
		ultr = &UltraSensor[ (UltraTypeDef)i ];
		HAL_GPIO_WritePin(ultr->Trig_GPIO, ultr->Trig_GPIO_Pin, GPIO_PIN_RESET );
		ultr->set_start_flag = 0;
	}
	
	HAL_Delay(10);
	
	ultr = &UltraSensor[ULTRA_FRONT];
	ultr->set_start_flag = 1;	/* start cycle. */
	CurrentChioce = ULTRA_FRONT;
}

static void Ultr_SendStartSignal(void)
{
	static uint8_t cnt = 0;
	static uint8_t cou = 0;
	
	UltraDataTypeDef * ultr = &UltraSensor[ (UltraTypeDef)cnt ];
	
	/* 1 : first get frong ultrsonic distance. */
	if(ultr->set_start_flag == 1 && cnt == 0)	/* first : FRONT. */
	{
		HAL_GPIO_WritePin(ultr->Trig_GPIO, ultr->Trig_GPIO_Pin, GPIO_PIN_SET );
		
		cou++;
		
		if(cou == 2)	/* send 20us high level. */
		{
			cou = 0;
			HAL_GPIO_WritePin(ultr->Trig_GPIO, ultr->Trig_GPIO_Pin, GPIO_PIN_RESET );
			ultr->set_start_flag = 2;
		}
	}
	else if(ultr->set_start_flag == 3 && cnt == 0)	/* wait close. */
	{
		if(ultr->set_end_flag == 1)
		{
			cnt = 1;
			ultr->set_start_flag = 0;
			UltraSensor[(UltraTypeDef)cnt].set_start_flag = 1;
		}
	}
	
	/* 2 : second get back ultrsonic distance. */
	if(ultr->set_start_flag == 1 && cnt == 1)
	{
		HAL_GPIO_WritePin(ultr->Trig_GPIO, ultr->Trig_GPIO_Pin, GPIO_PIN_SET );
		
		cou++;
		
		if(cou == 2)	/* send 20us high level. */
		{
			cou = 0;
			HAL_GPIO_WritePin(ultr->Trig_GPIO, ultr->Trig_GPIO_Pin, GPIO_PIN_RESET );
			ultr->set_start_flag = 2;
		}
	}
	else if(ultr->set_start_flag == 3 && cnt == 1)	/* wait close. */
	{
		if(ultr->set_end_flag == 1)
		{
			cnt = 2;
			ultr->set_start_flag = 0;
			UltraSensor[(UltraTypeDef)cnt].set_start_flag = 1;
		}
	}
	
	/* 3 : third get left ultrsonic distance. */
	if(ultr->set_start_flag == 1 && cnt == 2)
	{
		HAL_GPIO_WritePin(ultr->Trig_GPIO, ultr->Trig_GPIO_Pin, GPIO_PIN_SET );
		
		cou++;
		
		if(cou == 2)	/* send 20us high level. */
		{
			cou = 0;
			HAL_GPIO_WritePin(ultr->Trig_GPIO, ultr->Trig_GPIO_Pin, GPIO_PIN_RESET );
			ultr->set_start_flag = 2;
		}
	}
	else if(ultr->set_start_flag == 3 && cnt == 2)	/* wait close. */
	{
		if(ultr->set_end_flag == 1)
		{
			cnt = 3;
			ultr->set_start_flag = 0;
			UltraSensor[(UltraTypeDef)cnt].set_start_flag = 1;
		}
	}
	
	/* 4 : fourth get right ultrsonic distance. */
	if(ultr->set_start_flag == 1 && cnt == 3)
	{
		HAL_GPIO_WritePin(ultr->Trig_GPIO, ultr->Trig_GPIO_Pin, GPIO_PIN_SET );
		
		cou++;
		
		if(cou == 2)	/* send 20us high level. */
		{
			cou = 0;
			HAL_GPIO_WritePin(ultr->Trig_GPIO, ultr->Trig_GPIO_Pin, GPIO_PIN_RESET );
			ultr->set_start_flag = 2;
		}
	}
	else if(ultr->set_start_flag == 3 && cnt == 3)	/* wait close. */
	{
		if(ultr->set_end_flag == 1)
		{
			cnt = 0;
			ultr->set_start_flag = 0;
			UltraSensor[(UltraTypeDef)cnt].set_start_flag = 1;
		}
	}
	
	CurrentChioce = (UltraTypeDef)cnt;
}

static void Ultr_GetEchoSignal(void)
{
	static uint16_t time = 0;
	UltraDataTypeDef * ultr;
	
	ultr = &UltraSensor[ CurrentChioce ];
	
	if( (HAL_GPIO_ReadPin(ultr->Echo_GPIO, ultr->Echo_GPIO_Pin) == 1) && (ultr->set_start_flag == 2) )
	{
		ultr->recvive_signal_flag = 1;
	}
	
	if( (ultr->recvive_signal_flag == 1) && (ultr->set_start_flag == 2) )
	{
		time++;
		
		if( HAL_GPIO_ReadPin(ultr->Echo_GPIO, ultr->Echo_GPIO_Pin) == 0 )
		{
			ultr->recvive_signal_flag = 0;
			ultr->set_start_flag = 3;
			ultr->interval = time * 10;
			ultr->set_end_flag = 1;
			ultr->distance = ultr->interval * 340 / 2000 / 10;	//cm
			time = 0;
		}
	}

}

void UltraCalDistance_IRQHandler(void)
{
//	UltraTypeDef ultr_exit_line = GetUltraPin(GPIO_Pin);

	Ultr_SendStartSignal();
	Ultr_GetEchoSignal();
}

uint16_t BSP_GetUltraDistance(UltraTypeDef ultr)
{
	return UltraSensor[ultr].distance;
}
