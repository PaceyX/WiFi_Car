#include "camera_bsp.h"
#include "iic_bsp.h"
#include "i2c.h"



void Bsp_Camera_Init(uint32_t resolution)
{
	;
}

/**
  * @brief  Camera writes single data.
  * @param  Addr: I2C address
  * @param  Reg: Register address 
  * @param  Value: Data to be written
  */
void Camera_Io_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
	hIicMemoryWrite(&hi2c2, Addr, Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, 100);
}
