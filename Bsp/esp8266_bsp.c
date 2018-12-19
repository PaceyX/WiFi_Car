#include "esp8266_bsp.h"
#include "app_comm1.h"


char * ack_debug;
uint8_t EspAckInfo[100];

/**
*	@brief	Send cmd to esp8266.
*	@param	cmd : the cmd.
*			replay1 : the return ack info.
*			cmd_ytpe : the return type of this cmd.
*			waittime : the time wait 8266 return ack.
*	@retval	true : send and receive ack, or send success.
*			false : send fail. or not receive ack.
*/
bool Esp8266_Send_Cmd(char * cmd, char * reply1, AckType cmd_type, uint32_t waittime)
{
	uint32_t time, time2;
	u8 *data;
    u32 len, i;
	u32 length;
	u8 * ret = &EspAckInfo[0];
	
	memset(ret, 0, sizeof((char *)ret));	/* clear all ack buf. */
	
	/* send */
	if(cmd_type == NoAck)
	{
		Comm2_SendData((uint8_t *)(cmd), sizeof(cmd));
		return true;
	}
	
	Comm2_SendData((uint8_t *)(cmd), sizeof(cmd));
	
	/* wait received result or until timeout. */
	time = HAL_GetTick();
	time2 =  0;
	length = 0;
	
	while( (HAL_GetTick() - time) < waittime)
	{
		if(CommUsart_RecvData(&CommUsart2, &data, &len))
		{
			for(i=0; i<len; i++)
			{
				ret[length] = data[i];
				length++;
				time2 = HAL_GetTick();
			}
		}
		
		if( time2 != 0 && ((HAL_GetTick() - time2) > 50 ) )		/* receive and done*/
		{
			if(strstr((char *)ret, reply1) != NULL)
				return true;
			else
				return false;
		}	
	}
	return false;
}

bool ESP8266_Set_Mode(ModeTypeDef mode)
{
	switch ( mode )
	{
		case STA:
			return Esp8266_Send_Cmd( "AT+CWMODE=1", "OK", ReceiveAck, 2500 ); 
		case AP:
		  return Esp8266_Send_Cmd( "AT+CWMODE=2", "OK", ReceiveAck, 2500 ); 
		case STA_AP:
		  return Esp8266_Send_Cmd( "AT+CWMODE=3", "OK", ReceiveAck, 2500 ); 
		default:
		  return false;
	}
}

void Esp8266_Init(void)
{
	if(Esp8266_Send_Cmd("AT", "AT", ReceiveAck, 300))
		Comm1_printf("Esp8266 AT Cmd Ack. \n");
//	HAL_Delay(10);
	if(ESP8266_Set_Mode(STA))
	{
		Comm1_printf("STA Mode Set Success. \n");
	}
}


