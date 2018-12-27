/**
******************************************************************************
* @file    ultrasonic.c
* @author  Pacey
* @date    2018/03/13
* @brief   US015 ultrasonic distance module.
* @attention Copyright (C) X-FLY team.
******************************************************************************
*/


#include "ultrasonic.h"


#define TRIG_SET_LOW		HAL_GPIO_WritePin(TRIG_F_GPIO_Port, TRIG_F_Pin, GPIO_PIN_RESET);
#define TRIG_SET_HIGH		HAL_GPIO_WritePin(TRIG_F_GPIO_Port, TRIG_F_Pin, GPIO_PIN_SET);
#define READ_ECHO_PIN		HAL_GPIO_ReadPin(ECHO_F_GPIO_Port, ECHO_F_Pin)	

#define SONIC_VELOCITY		(0.034)			/* cm/us */	

typedef struct
{
	uint8_t set_start_flag;
	uint8_t set_end_flag;
	uint8_t recvive_signal_flag;
	uint16_t go_back_time;
	float distance;
}UltraTypedef;
UltraTypedef ultra;


/**
*   @brief  Gpio init of ultrasonic trig pin.
*	@param	none.
*	@retval	none.
*/
void BSP_GPIO_SonicInit(void)
{
    TRIG_SET_LOW;				/* Trig pin set low. */
	ultra.set_end_flag = 1;		/* init. */
}

/**
*	@brief	ultrasonic sampling.
*	@param	none.
*	@bote	Sampling frequency : at least 20ms.
*	@retval	none.
*/
void ultraDistanceSampling(void)
{	
	ultra.set_start_flag = 1;
	//printf("%f\n", ultra.distance);
}

/**
*	@brief	calculate distance. 340m/s * (time / 2)
*	@param	none.
*	@retval none.
*/
static void CalculateDistance(void)
{
	ultra.distance = SONIC_VELOCITY * (ultra.go_back_time / 2);
}

/**
*  @brief  Send start info.
*  @param  none.
*  @retval none.
*  @note   send 10us high level.
*/
static void SendStartSignal(void)
{
	static uint8_t cou = 0;
	
	if(ultra.set_start_flag == 1)
	{
//		if(ultra.set_end_flag == 0)		return;
		
		TRIG_SET_HIGH;
		cou++;
		if(cou == 2)	/* send 20us high level. */
		{
			TRIG_SET_LOW;
			ultra.set_start_flag = 2;
//			ultra.set_end_flag = 0;
			cou = 0;
		}
	}
}

/**
*	@brief	Get echo pin signal.
*	@param	none.
*	@retval none.
*/
static void GetEchoSignal(void)
{
	static uint16_t time = 0;
	
	if(READ_ECHO_PIN == GPIO_PIN_SET && ultra.set_start_flag == 2)
	{
		ultra.recvive_signal_flag = 1;
	}
	
	if(ultra.recvive_signal_flag == 1 && ultra.set_start_flag == 2)
	{
		time++;		/* once 10us. */
		if(READ_ECHO_PIN == GPIO_PIN_RESET)
		{
			ultra.recvive_signal_flag = 0;
			ultra.set_start_flag = 0;
			ultra.go_back_time = time*10;
			ultra.set_end_flag = 1;
			CalculateDistance();				/* calculate distance. */
			time = 0;
		}
	}
	/* No ack handle situation in here. */
	
}


/**
*  @brief  Call back function.
*  @param  none.
*  @retval none.
*  @note   Be called in timer interrupt that breaks every 10us.
*/
void ultrasonicCallBack(void)
{
	SendStartSignal();
	GetEchoSignal();
}
