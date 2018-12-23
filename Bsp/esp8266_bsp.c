#include "esp8266_bsp.h"
#include "app_comm1.h"
#include "c_algorithm.h"


#define ACK_BUF_SIZE	100

uint8_t EspAckInfo[ACK_BUF_SIZE];
char LocalIP[100] = {0};

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
	
	memset(ret, 0, ACK_BUF_SIZE);	/* clear all ack buf. */
	
	/* send */
	if(cmd_type == NoAck)
	{
		Comm2_SendData((uint8_t *)(cmd), CalculateStringlength((uint8_t *)cmd));
		return true;
	}
	
	Comm2_SendData((uint8_t *)(cmd), CalculateStringlength((uint8_t *)cmd));
	
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
		
		if( time2 != 0 && ((HAL_GetTick() - time2) > 100 ) )		/* receive and done*/
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
			return Esp8266_Send_Cmd( "AT+CWMODE=1\r\n", "OK", ReceiveAck, 2500 ); 
		case AP:
		  return Esp8266_Send_Cmd( "AT+CWMODE=2\r\n", "OK", ReceiveAck, 2500 ); 
		case STA_AP:
		  return Esp8266_Send_Cmd( "AT+CWMODE=3\r\n", "OK", ReceiveAck, 2500 ); 
		default:
		  return false;
	}
}

bool ESP8266_Set_AP(char * ip)
{
	char ip_cmd[30];
	
	sprintf(ip_cmd, "AT+CIPAP=\"%s\"\r\n", ip);
	
	return Esp8266_Send_Cmd(ip_cmd, "OK", ReceiveAck, 5000);
}

/**
*	@brief	Build AP.
*/
bool ESP8266_Build_AP(char * ssid, char * password, ApEncryptTypeDef encrypt_mode)
{
	char cmd[120];
	
	sprintf (cmd, "AT+CWSAP=\"%s\",\"%s\",1,%d\r\n", ssid, password, encrypt_mode);
	
	return Esp8266_Send_Cmd(cmd, "OK", ReceiveAck, 1000);
}

bool ESP8266_Enable_MultiLink(FunctionalState sw)
{
	char cmd[20];
	
	sprintf(cmd, "AT+CIPMUX=%d\r\n", ( sw ? 1 : 0 ));
	
	return Esp8266_Send_Cmd(cmd, "OK", ReceiveAck, 500 );
}

bool ESP8266_Enable_Server(char * port_num, char * out_time)
{
	char cmd1[120], cmd2[120];
	
	sprintf ( cmd1, "AT+CIPSERVER=%d,%s\r\n", 1, port_num );
	sprintf ( cmd2, "AT+CIPSTO=%s\r\n", out_time );
	
	return ( Esp8266_Send_Cmd(cmd1, "OK", ReceiveAck, 500) && Esp8266_Send_Cmd(cmd2, "OK", ReceiveAck, 500) );
	
}

bool ESP8266_Shutdown_Server(char * port_num)
{
	char cmd[120];
	
	sprintf (cmd, "AT+CIPSERVER=0,%s\r\n", port_num);
	
	return Esp8266_Send_Cmd(cmd, "OK", ReceiveAck, 500);
}

bool ESP8266_Inquiry_ApIp(char * ap_ip, uint8_t ip_length)
{
	char * pCh;
	char uc;
	
	if(Esp8266_Send_Cmd("AT+CIFSR\r\n", "OK", ReceiveAck, 500))
	{
		pCh = strstr((char *)EspAckInfo, "APIP,\"");
		if(pCh)		pCh += 6;
		else		return false;
		
		for(uc = 0; uc < ip_length; uc++)
		{
			ap_ip[uc] = *(pCh + uc);
			if(ap_ip[uc] == '\"')
			{
				ap_ip[uc] = '\0';
				break;
			}
		}
	}
	return true;
}

/**
*	@brief	透传发送字符串
*	@param	str : the send string.
*			length : the string length.
*/
bool ESP8266_SendString(char * str,  u16 len)
{
	char cStr [20];
	
	/*
	此处设置要发送的连接设备，作为AP模式最大能连接5个设备，可在此全部扩展。默认只发送第一个连接的设备. 
	*/
	sprintf ( cStr, "AT+CIPSEND=%d,%d\r\n", 0, CalculateStringlength((uint8_t *)str) );
	
	return Esp8266_Send_Cmd(cStr, "> ", ReceiveAck, 500) && Esp8266_Send_Cmd(str, "SEND OK", ReceiveAck, 500);
}




void Esp8266_Init(void)
{
	char temp[100];
	
	/* 复位后返回的信息太多，实际是执行成功的，但是发送函数接收返回ACK的bug不够. */
#if 0
	if(Esp8266_Send_Cmd("AT+RST\r\n", "OK", ReceiveAck, 300))
		Comm1_printf("Esp8266 Reset. \n");	
#endif
	
	/* AT Test. */
	if(Esp8266_Send_Cmd("AT\r\n", "OK", ReceiveAck, 300))
		Comm1_printf("Esp8266 AT Cmd Test Ok. \n");

	/* Set Mode. */
	if(ESP8266_Set_Mode(AP))
		Comm1_printf("AP Mode Set Success. \n");

	/* Set AP ip. */
	if(ESP8266_Set_AP(SERVER_AP))
		Comm1_printf("AP Set Success : 192.168.78.1. \n");
	
	/* Set AP ssid, password, encrypt_mode. */
	if(ESP8266_Build_AP(SERVER_AP_SSID, SERVER_AP_PASSWORD, SERVER_AP_ENCRYPT_TYPE))
		Comm1_printf("AP Key Set Success. \n");
	
	/* Enable Multi Link. */
	if(ESP8266_Enable_MultiLink(ENABLE))
		Comm1_printf("Enable Multi Link. \n");
	
	/* Enable Server Port. */
	if(ESP8266_Enable_Server(SERVER_PORT, SERVER_OUTTIME))
		Comm1_printf("Enable Server Port : 8080. \n");
	
	if(ESP8266_Inquiry_ApIp(LocalIP, 20))
	{
		Comm1_printf("ESP8266 AP Mode Init Success. \n");
		HAL_Delay(10);
		
		memset(temp, 0, 100);
		sprintf(temp, "The Moudle WiFi SSID is : %s ,  Password : %s \n", SERVER_AP_SSID, SERVER_AP_PASSWORD);
		Comm1_printf((char *)temp);
		HAL_Delay(10);
		
		memset(temp, 0, 100);
		sprintf(temp, "SSID : %s , IP : %s, Port : %s \n", SERVER_AP_SSID,  LocalIP, SERVER_PORT);
		Comm1_printf((char *)temp);
		HAL_Delay(10);
	}
}


