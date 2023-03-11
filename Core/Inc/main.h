/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define AD_D0_Pin GPIO_PIN_0
#define AD_D0_GPIO_Port GPIOC
#define AD_D1_Pin GPIO_PIN_1
#define AD_D1_GPIO_Port GPIOC
#define AD_D2_Pin GPIO_PIN_2
#define AD_D2_GPIO_Port GPIOC
#define AD_D3_Pin GPIO_PIN_3
#define AD_D3_GPIO_Port GPIOC
#define tlc_OE_Pin GPIO_PIN_0
#define tlc_OE_GPIO_Port GPIOA
#define AD_D4_Pin GPIO_PIN_4
#define AD_D4_GPIO_Port GPIOC
#define AD_D5_Pin GPIO_PIN_5
#define AD_D5_GPIO_Port GPIOC
#define tlc_SDI_Pin GPIO_PIN_9
#define tlc_SDI_GPIO_Port GPIOD
#define tlc_CLK_Pin GPIO_PIN_10
#define tlc_CLK_GPIO_Port GPIOD
#define tlc_LE_Pin GPIO_PIN_11
#define tlc_LE_GPIO_Port GPIOD
#define LD5_Pin GPIO_PIN_14
#define LD5_GPIO_Port GPIOD
#define AD_D6_Pin GPIO_PIN_6
#define AD_D6_GPIO_Port GPIOC
#define AD_D7_Pin GPIO_PIN_7
#define AD_D7_GPIO_Port GPIOC
#define tlc_SDO_Pin GPIO_PIN_9
#define tlc_SDO_GPIO_Port GPIOC
#define LED_indicator_Pin GPIO_PIN_15
#define LED_indicator_GPIO_Port GPIOA
#define AD_SCLK_Pin GPIO_PIN_5
#define AD_SCLK_GPIO_Port GPIOD
#define AD_SDATA_Pin GPIO_PIN_6
#define AD_SDATA_GPIO_Port GPIOD
#define AD_SLOAD_Pin GPIO_PIN_7
#define AD_SLOAD_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
