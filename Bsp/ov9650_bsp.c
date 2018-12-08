#include "ov9650_bsp.h"
#include "iic_soft_bsp.h"



uint8_t i2c_cmd[2];


/**
  * @brief  Read the OV9650 Camera identity.
  * @param  DeviceAddr: Device address on communication Bus.
  * @retval the OV9650 ID
  */
uint16_t OV9650_ReadID(uint16_t DeviceAddr)
{
	IIC_WriteOneByte(OV9650_I2C_ADDR_W, 0xFF, 0x01);		/* 原理待确认. */
	
	IIC_ReadmultiyBytes(OV9650_I2C_ADDR_R, OV9650_PID_ADDR, 2, i2c_cmd);
	
	return i2c_cmd[0];
}





