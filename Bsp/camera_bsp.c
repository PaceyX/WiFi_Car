#include "camera_bsp.h"





void Bsp_Camera_Init(uint32_t resolution)
{
	;
}


/**
  * @brief  Manages error callback by re-initializing I2C.
  * @param  Addr: I2C Address
  */
static void I2Cx_Error(uint8_t Addr)
{

}

/**
  * @brief  Camera writes single data.
  * @param  Addr: I2C address
  * @param  Reg: Register address 
  * @param  Value: Data to be written
  */
void Camera_I2C_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{

}

/**
  * @brief  Camera reads single data.
  * @param  Addr: I2C address
  * @param  Reg: Register address 
  * @retval Read data
  */
uint8_t Camera_I2C_Read(uint8_t Addr, uint8_t Reg)
{
	uint8_t Value = 0;
	
	
	return Value;
}

