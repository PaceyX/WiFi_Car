#include "iic_soft_bsp.h"




/*
*	@brief: Initialise simulate iic pin.
*	@param: none.
*	@return: none.
*/
void IIC_GPIO_Init(void)
{
      
	GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();   //使能GPIOH时钟
    
    //PH4,5初始化设置
    GPIO_Initure.Pin=GPIO_PIN_3|GPIO_PIN_10;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //快速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

    IIC_SDA=1;
    IIC_SCL=1;  
}

/**
*	@brief	for circular delay function.
*	@note	STM32F446 180M Frequency adjust.
*/
static void Delay_us(uint32_t time)
{
	uint8_t i;
	
	while(time--)
	{
		i = 42;
		while(i--);	
	}
}

void Delay_ms(uint32_t time) 
{
	Delay_us(time * 1000);
}

/*
*	@brief: make a start signal.
*	@param: none.
*	@return: none.
*/
void IIC_Start(void)
{
	SDA_OUT;
    IIC_SDA = 1;  //SDA = 1;
	IIC_SCL = 1;
    Delay_us(4);
    IIC_SDA = 0;
    Delay_us(4);
    IIC_SCL = 0;  //SCL = 0;

}

/*
*	@brief: make a stop signal.
*	@param: none.
*	@return: none.
*/
void IIC_Stop(void)
{
	SDA_OUT;
	IIC_SCL = 0;   //SCL = 0
    IIC_SDA = 0;   //SDA = 0;
    Delay_us(4);
    IIC_SCL = 1;   //SCL = 1;
    IIC_SDA = 1;   //SDA = 1;
    Delay_us(4);
}

/*
*	@brief: receive slave ack signal.
*	@param: none.
*	@return: 1: noack  0: ack.
*/
unsigned char IIC_ReceiveACK(void)
{
//	u8 ErrorTime = 0;
//	while(SDA == 1)
//	{
//		ErrorTime++;
//		if(ErrorTime>250)
//		{
//			IIC_Stop();
//			printf("iic time out.\n");
//		}
//	}
	unsigned char ACK;

	SDA_IN;
    IIC_SDA = 1;     
    IIC_SCL = 1;     
    Delay_us(2);

    if (READ_SDA==1)  
    {
        ACK = 1;   
    }
    else ACK = 0;  
    IIC_SCL = 0;    
    Delay_us(2);
	return ACK;
}

/*
*	@brief:	send ack signal, send 0.
*	@param: none.
*	@return: none.
*/
void IIC_SendACK(void)
{
	IIC_SCL = 0;
	SDA_OUT;
	IIC_SDA = 0;	//SDA = 0   ACK
	Delay_us(2);
	IIC_SCL = 1;
	Delay_us(2);
	IIC_SCL = 0;
}

/*
*	@brief:	send nack signal, send 1.
*	@param: none.
*	@return: none.
*/
void IIC_SendNACK(void)
{
    IIC_SCL = 0;   //SCL = 0;
	SDA_OUT;
	IIC_SDA = 1;   //SDA = 1    NACK
	Delay_us(2);
	IIC_SCL = 1;
    Delay_us(2);
    IIC_SCL = 0;   //SCL = 0;
}

/*
*	@brief:	iic send one byte.
*	@param: one byte.
*	@return: none.
*/
void IIC_SendByte(unsigned char dat)
{
    unsigned char i;

	SDA_OUT;
    IIC_SCL = 0;     //SCL = 0;

    for( i=0;i<8;i++ ) //
    {
        if( (dat<<i)&0x80 ) IIC_SDA = 1;   //SDA = 1;
        else 				IIC_SDA = 0;   //SDA = 0;
        
		Delay_us(2);
        IIC_SCL = 1;  //SCL = 1;
        Delay_us(2);
        IIC_SCL = 0;  //SCL = 0;
        Delay_us(2);

    }
}

/*
*	@brief:	iic receive one byte.
*	@param: none.
*	@return: one byte.
*/
u8 IIC_ReadByte(unsigned char ack)
{
    unsigned char dat;
    unsigned char i;

	SDA_IN;
    for( i=0;i<8;i++ )
    {    
		IIC_SCL = 0;    
		Delay_us(2);
		IIC_SCL = 1; 
        dat <<= 1;
        dat = dat | (READ_SDA);
        Delay_us(2);

    }
	if(!ack)
		IIC_SendNACK();
	else
		IIC_SendACK();
	
    return dat;
}

/*
*	@brief:	read one byte from special register.
*	@param: 1, slaver address  2, read register address.
*	@return: ack signal.
*/
u8 IIC_ReadOneByte(u8 SlaveAddr ,u8 ReadAddr)
{
	u8 temp = 0;
	IIC_Start();
	IIC_SendByte(SlaveAddr);
	IIC_ReceiveACK();
	IIC_SendByte(ReadAddr);
	IIC_ReceiveACK();
	
	IIC_Start();
	IIC_SendByte(SlaveAddr+1);
	IIC_ReceiveACK();
	temp = IIC_ReadByte(0);			//not ack, stop iic after read one byte.
	IIC_Stop();
	return temp;
}

/*
*	@brief: write data to slaver.
*	@param: 1, slaver address   2, write address  3, the data.
*	@return: none.
*/
void IIC_WriteOneByte(u8 SlaveAddr, u8 WriteAddr,u8 data)
{
	IIC_Start();
	IIC_SendByte(SlaveAddr);
	IIC_ReceiveACK();
	IIC_SendByte(WriteAddr);
	IIC_ReceiveACK();
	IIC_SendByte(data);
	IIC_ReceiveACK();
	IIC_Stop();
	Delay_ms(10);
}

/*
*	@brief: 
*	@param: 
*	@return: 
*/
void IIC_ReadmultiyBytes(u8 SlaveAddr,u8 RegAddr,u8 Len,u8 *Buf_Addr)
{
	u8 i;
	IIC_Start();
	IIC_SendByte(SlaveAddr);
	IIC_ReceiveACK();
	IIC_SendByte(RegAddr);
	IIC_ReceiveACK();
	IIC_Start();
	IIC_SendByte(SlaveAddr+1);
	IIC_ReceiveACK();
	for(i=0;i<Len;i++)
	{
		if(i!=Len-1) 
		{
			Buf_Addr[i]=IIC_ReadByte(1);	//??	
		}
		else
			Buf_Addr[i]=IIC_ReadByte(0);	//????
    }
	
	IIC_Stop();
}

