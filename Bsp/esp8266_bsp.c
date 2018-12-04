#include "esp8266_bsp.h"


/**
*	@brief	对ESP8266发送AT指令
*	@param	cmd : 待发送的命令
*			replay1 : 
*			cmd_ytpe : 
*/
bool Esp8266_Send_Cmd(char * cmd, char * reply1, AckType cmd_type, uint32_t waittime)
{
	uint32_t time, time2;
	u8 *data;
    u32 len, i;
	u32 length;
	
	/* send */
	if(cmd_type == NoAck)
	{
		Comm2_SendData((uint8_t *)(cmd), strlen(cmd));
		return true;
	}
	
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
				reply1[length] = data[i];
				length++;
				time2 = HAL_GetTick();
			}
		}
		
		if( time2 != 0 && ((HAL_GetTick() - time2) > 3) )		/* receive and done*/
		{
			return true;
		}	
	}
	return false;
}
