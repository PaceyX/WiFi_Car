#ifndef __IIC_BSP_H__
#define __IIC_BSP_H__

#include "mcu_lib.h"



HAL_StatusTypeDef hIicMemoryRead(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, 
        uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef hIicMemoryWrite(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, 
        uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef hIicMemoryWriteByDma(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef hIicMemoryReadByDma(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size);














#endif
