/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32f2xx_hal.h"

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
#define out_5_Pin GPIO_PIN_2
#define out_5_GPIO_Port GPIOE
#define out_4_Pin GPIO_PIN_3
#define out_4_GPIO_Port GPIOE
#define out_3_Pin GPIO_PIN_4
#define out_3_GPIO_Port GPIOE
#define out_2_Pin GPIO_PIN_5
#define out_2_GPIO_Port GPIOE
#define out_1_Pin GPIO_PIN_6
#define out_1_GPIO_Port GPIOE
#define d1_Pin GPIO_PIN_13
#define d1_GPIO_Port GPIOC
#define d2_Pin GPIO_PIN_14
#define d2_GPIO_Port GPIOC
#define d3_Pin GPIO_PIN_15
#define d3_GPIO_Port GPIOC
#define in_1_Pin GPIO_PIN_0
#define in_1_GPIO_Port GPIOC
#define in_2_Pin GPIO_PIN_2
#define in_2_GPIO_Port GPIOC
#define in_3_Pin GPIO_PIN_3
#define in_3_GPIO_Port GPIOC
#define in_4_Pin GPIO_PIN_0
#define in_4_GPIO_Port GPIOA
#define in_5_Pin GPIO_PIN_3
#define in_5_GPIO_Port GPIOA
#define spi3_cs_Pin GPIO_PIN_4
#define spi3_cs_GPIO_Port GPIOA
#define in_6_Pin GPIO_PIN_5
#define in_6_GPIO_Port GPIOA
#define in_7_Pin GPIO_PIN_6
#define in_7_GPIO_Port GPIOA
#define spi3_wp_Pin GPIO_PIN_0
#define spi3_wp_GPIO_Port GPIOB
#define in_b_cc1_Pin GPIO_PIN_7
#define in_b_cc1_GPIO_Port GPIOE
#define in_a_cc1_Pin GPIO_PIN_8
#define in_a_cc1_GPIO_Port GPIOE
#define relay_2_Pin GPIO_PIN_9
#define relay_2_GPIO_Port GPIOE
#define relay_3_Pin GPIO_PIN_10
#define relay_3_GPIO_Port GPIOE
#define relay_4_Pin GPIO_PIN_11
#define relay_4_GPIO_Port GPIOE
#define relay_5_Pin GPIO_PIN_12
#define relay_5_GPIO_Port GPIOE
#define relay_6_Pin GPIO_PIN_13
#define relay_6_GPIO_Port GPIOE
#define relay_7_Pin GPIO_PIN_14
#define relay_7_GPIO_Port GPIOE
#define relay_8_Pin GPIO_PIN_15
#define relay_8_GPIO_Port GPIOE
#define out_16_Pin GPIO_PIN_14
#define out_16_GPIO_Port GPIOB
#define out_15_Pin GPIO_PIN_15
#define out_15_GPIO_Port GPIOB
#define led_485_1_Pin GPIO_PIN_10
#define led_485_1_GPIO_Port GPIOD
#define led_485_3_Pin GPIO_PIN_11
#define led_485_3_GPIO_Port GPIOD
#define led_bms_Pin GPIO_PIN_12
#define led_bms_GPIO_Port GPIOD
#define led_lan_Pin GPIO_PIN_13
#define led_lan_GPIO_Port GPIOD
#define led_ccs_Pin GPIO_PIN_15
#define led_ccs_GPIO_Port GPIOD
#define led_232_2_Pin GPIO_PIN_8
#define led_232_2_GPIO_Port GPIOC
#define led_232_6_Pin GPIO_PIN_9
#define led_232_6_GPIO_Port GPIOC
#define out_14_Pin GPIO_PIN_2
#define out_14_GPIO_Port GPIOD
#define out_13_Pin GPIO_PIN_3
#define out_13_GPIO_Port GPIOD
#define out_12_Pin GPIO_PIN_4
#define out_12_GPIO_Port GPIOD
#define out_11_Pin GPIO_PIN_7
#define out_11_GPIO_Port GPIOD
#define out_10_Pin GPIO_PIN_7
#define out_10_GPIO_Port GPIOB
#define out_9_Pin GPIO_PIN_8
#define out_9_GPIO_Port GPIOB
#define out_8_Pin GPIO_PIN_9
#define out_8_GPIO_Port GPIOB
#define out_7_Pin GPIO_PIN_0
#define out_7_GPIO_Port GPIOE
#define out_6_Pin GPIO_PIN_1
#define out_6_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
