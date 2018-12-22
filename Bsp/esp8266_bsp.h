#ifndef __ESP8266_BSP_H__
#define __ESP8266_BSP_H__

#include <stdbool.h>
#include "mcu_lib.h"
#include "app_comm2_wifi.h"
#include "string.h"
#include "usart_bsp.h"


/* The user function used by esp8266 driver. */
#define esp8266_cmd_send()

#define CMD_WAITTIME	50

#define SERVER_AP						"192.168.78.1"
#define SERVER_AP_SSID         			"WiFi_Car"   
#define SERVER_AP_PASSWORD           	"123456789" 
#define SERVER_AP_ENCRYPT_TYPE         	OPEN   
#define SERVER_PORT						"8080"
#define SERVER_OUTTIME					"1800"	/* S. */

typedef enum {
	NoAck = 0,
	ReceiveAck = 1
} AckType;

typedef enum{
	STA,
	AP,
	STA_AP  
} ModeTypeDef;

typedef enum{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ApEncryptTypeDef;

void Esp8266_Init(void);
bool Esp8266_Send_Cmd(char * cmd, char * reply1, AckType cmd_type, uint32_t waittime);
bool ESP8266_SendString(char * str);

#endif


