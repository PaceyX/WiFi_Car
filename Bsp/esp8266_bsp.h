#ifndef __ESP8266_BSP_H__
#define __ESP8266_BSP_H__

#include <stdbool.h>
#include "mcu_lib.h"
#include "app_comm2_wifi.h"
#include "string.h"
#include "usart_bsp.h"


/* The user function used by esp8266 driver. */
#define esp8266_cmd_send()




typedef enum {
	NoAck = 0,
	ReceiveAck = 1
} AckType;



bool Esp8266_Send_Cmd(char * cmd, char * reply1, AckType cmd_type, uint32_t waittime);


#endif


