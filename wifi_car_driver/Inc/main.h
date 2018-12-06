/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define FP_Reset_Pin GPIO_PIN_13
#define FP_Reset_GPIO_Port GPIOC
#define ECHO_L_Pin GPIO_PIN_14
#define ECHO_L_GPIO_Port GPIOC
#define TRIG_B_Pin GPIO_PIN_15
#define TRIG_B_GPIO_Port GPIOC
#define VBAT_SAMP_Pin GPIO_PIN_0
#define VBAT_SAMP_GPIO_Port GPIOC
#define CHRG_Pin GPIO_PIN_1
#define CHRG_GPIO_Port GPIOC
#define DCMI_SCL_Pin GPIO_PIN_2
#define DCMI_SCL_GPIO_Port GPIOC
#define TRIG_F_Pin GPIO_PIN_3
#define TRIG_F_GPIO_Port GPIOC
#define INF_2_Pin GPIO_PIN_0
#define INF_2_GPIO_Port GPIOA
#define INR_2_Pin GPIO_PIN_1
#define INR_2_GPIO_Port GPIOA
#define DCMI_HSYNC_Pin GPIO_PIN_4
#define DCMI_HSYNC_GPIO_Port GPIOA
#define IMU_INT_Pin GPIO_PIN_5
#define IMU_INT_GPIO_Port GPIOA
#define DCMI_PCLK_Pin GPIO_PIN_6
#define DCMI_PCLK_GPIO_Port GPIOA
#define ECHO_F_Pin GPIO_PIN_4
#define ECHO_F_GPIO_Port GPIOC
#define TRIG_L_Pin GPIO_PIN_5
#define TRIG_L_GPIO_Port GPIOC
#define INF_1_Pin GPIO_PIN_0
#define INF_1_GPIO_Port GPIOB
#define INR_1_Pin GPIO_PIN_1
#define INR_1_GPIO_Port GPIOB
#define ECHO_B_Pin GPIO_PIN_12
#define ECHO_B_GPIO_Port GPIOB
#define NETLINK_Pin GPIO_PIN_13
#define NETLINK_GPIO_Port GPIOB
#define CL_1_Pin GPIO_PIN_14
#define CL_1_GPIO_Port GPIOB
#define CL_2_Pin GPIO_PIN_15
#define CL_2_GPIO_Port GPIOB
#define DCMI_D0_Pin GPIO_PIN_6
#define DCMI_D0_GPIO_Port GPIOC
#define DCMI_D1_Pin GPIO_PIN_7
#define DCMI_D1_GPIO_Port GPIOC
#define DCMI_D2_Pin GPIO_PIN_8
#define DCMI_D2_GPIO_Port GPIOC
#define DCMI_D3_Pin GPIO_PIN_9
#define DCMI_D3_GPIO_Port GPIOC
#define TRIG_R_Pin GPIO_PIN_11
#define TRIG_R_GPIO_Port GPIOA
#define ECHO_R_Pin GPIO_PIN_12
#define ECHO_R_GPIO_Port GPIOA
#define LED0_Pin GPIO_PIN_15
#define LED0_GPIO_Port GPIOA
#define DCMI_D8_Pin GPIO_PIN_10
#define DCMI_D8_GPIO_Port GPIOC
#define DCMI_D4_Pin GPIO_PIN_11
#define DCMI_D4_GPIO_Port GPIOC
#define DCMI_D9_Pin GPIO_PIN_12
#define DCMI_D9_GPIO_Port GPIOC
#define DCMI_SDA_Pin GPIO_PIN_4
#define DCMI_SDA_GPIO_Port GPIOB
#define DCMI_D5_Pin GPIO_PIN_6
#define DCMI_D5_GPIO_Port GPIOB
#define DCMI_VSYNC_Pin GPIO_PIN_7
#define DCMI_VSYNC_GPIO_Port GPIOB
#define DCMI_D6_Pin GPIO_PIN_8
#define DCMI_D6_GPIO_Port GPIOB
#define DCMI_D7_Pin GPIO_PIN_9
#define DCMI_D7_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
