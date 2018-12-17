#ifndef __CAMERA_BSP_H__
#define __CAMERA_BSP_H__

#include "mcu_lib.h"




void Camera_I2C_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);
uint8_t Camera_I2C_Read(uint8_t Addr, uint8_t Reg);




#endif
