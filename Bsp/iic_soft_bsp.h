#ifndef __IIC_SOFT_BSP_H__
#define __IIC_SOFT_BSP_H__


#include "stm32f4xx_hal.h"
#include "mcu_lib.h"



//#define IIC1								//must be only one be defined.
#define IIC2

/**********************************************************************************/


//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //OUTPUT
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //IN

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //OUTPUT
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //IN

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //OUTPUT
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //IN

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //OUTPUT 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //IN

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //OUTPUT 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //IN

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //OUTPUT 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //IN

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //OUTPUT 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //IN

/**********************************************************************************/


#ifdef  IIC1

#define RCC_PERIPH		RCC_APB2Periph_GPIOB				//RCC Config
#define IIC_SCL_Pin		GPIO_Pin_6							//SCL Pin
#define IIC_SDA_Pin		GPIO_Pin_7							//SDA Pin
#define IIC_GPIO_Port	GPIOB		

#define SDA_IN			{GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0X80000000;}
#define SDA_OUT 		{GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0X30000000;}
	
#define SCL_H()  		(PBout(6) = 1)						//SCL Push high
#define SCL_L()  		(PBout(6) = 0)						//SCL Pull low
#define SDA_H()  		(PBout(7) = 1)						//SDA Push high
#define SDA_L()  		(PBout(7) = 0)						//SDA Pull low
#define SDA  			(PBin(7))							//Read SDA

#endif
#ifdef	IIC2

#define RCC_PERIPH		RCC_APB2Periph_GPIOB				//RCC Config
#define IIC_SCL_Pin		GPIO_Pin_10							//SCL Pin
#define IIC_SDA_Pin		GPIO_Pin_3							//SDA Pin
#define IIC_GPIO_Port	GPIOB								//IIC Port

#define SDA_OUT			{IIC_GPIO_Port->MODER &= 0xFFFFFF3F; IIC_GPIO_Port->MODER |= 1<<6; } //SDA push_pull output.
#define SDA_IN			{IIC_GPIO_Port->MODER &= 0xFFFFFF3F; IIC_GPIO_Port->MODER |= 0<<6; } //SDA floating input.

#define SCL_H()  		(PBout(10) = 1)						//SCL Push high
#define SCL_L()  		(PBout(10) = 0)						//SCL Pull low
#define SDA_H()  		(PBout(11) = 1)						//SDA Push high
#define SDA_L()  		(PBout(11) = 0)						//SDA Pull low
#define SDA  			(PBin(11))							//Read SDA

#endif


void IIC_GPIO_Init(void);

void IIC_Start(void);
void IIC_Stop(void);
unsigned char IIC_ReceiveACK(void);
void IIC_SendACK(void);
void IIC_SendNACK(void);
void IIC_SendByte(unsigned char dat);
u8 	 IIC_ReadByte(unsigned char ack);
void IIC_WriteOneByte(u8 SlaveAddr, u8 WriteAddr,u8 data);
void IIC_ReadmultiyBytes(u8 SlaveAddr,u8 RegAddr,u8 Len,u8 *Buf_Addr);

#endif


