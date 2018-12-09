#include "camera_bsp.h"





//void Bsp_Camera_Init(uint32_t resolution)
//{
//	;
//}


///**
//  * @brief  Manages error callback by re-initializing I2C.
//  * @param  Addr: I2C Address
//  */
//static void I2Cx_Error(uint8_t Addr)
//{
//  /* De-initialize the I2C communication bus */
//  HAL_I2C_DeInit(&hi2c2);
//  
//  /* Re-Initialize the I2C communication bus */
//  MX_I2C2_Init();
//}

///**
//  * @brief  Camera writes single data.
//  * @param  Addr: I2C address
//  * @param  Reg: Register address 
//  * @param  Value: Data to be written
//  */
//void Camera_I2C_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
//{
//	hIicMemoryWrite(&hi2c2, Addr, Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, 100);
//}

///**
//  * @brief  Camera reads single data.
//  * @param  Addr: I2C address
//  * @param  Reg: Register address 
//  * @retval Read data
//  */
//uint8_t Camera_I2C_Read(uint8_t Addr, uint8_t Reg)
//{
//	HAL_StatusTypeDef status = HAL_OK;
//	uint8_t Value = 0;
//	
//	status = hIicMemoryRead(&hi2c2, Addr, Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, 10);
//	
//	if(status != HAL_OK)
//	{
//		I2Cx_Error(Addr);
//	}
//	
//	return Value;
//}

