/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f7xx_hal.h"

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
#define LCD_Touch_CLK_Pin GPIO_PIN_2
#define LCD_Touch_CLK_GPIO_Port GPIOE
#define LCD_Touch_IRQ_Pin GPIO_PIN_3
#define LCD_Touch_IRQ_GPIO_Port GPIOE
#define LCD_Touch_CS_Pin GPIO_PIN_4
#define LCD_Touch_CS_GPIO_Port GPIOE
#define LCD_Touch_DO_Pin GPIO_PIN_5
#define LCD_Touch_DO_GPIO_Port GPIOE
#define LCD_Touch_DIN_Pin GPIO_PIN_6
#define LCD_Touch_DIN_GPIO_Port GPIOE
#define User_B1_Button_Pin GPIO_PIN_13
#define User_B1_Button_GPIO_Port GPIOC
#define User_B1_Button_EXTI_IRQn EXTI15_10_IRQn
#define ADC3_09_R4_Pin GPIO_PIN_3
#define ADC3_09_R4_GPIO_Port GPIOF
#define ADC3_15_R5_Pin GPIO_PIN_5
#define ADC3_15_R5_GPIO_Port GPIOF
#define LCD_SCK_Pin GPIO_PIN_7
#define LCD_SCK_GPIO_Port GPIOF
#define LCD_MISO_Pin GPIO_PIN_8
#define LCD_MISO_GPIO_Port GPIOF
#define LCD_MOSI_Pin GPIO_PIN_9
#define LCD_MOSI_GPIO_Port GPIOF
#define ADC3_08_R6_Pin GPIO_PIN_10
#define ADC3_08_R6_GPIO_Port GPIOF
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define ADC3_10_R2_Pin GPIO_PIN_0
#define ADC3_10_R2_GPIO_Port GPIOC
#define RMII_MDC_Pin GPIO_PIN_1
#define RMII_MDC_GPIO_Port GPIOC
#define ADC3_13_R3_Pin GPIO_PIN_3
#define ADC3_13_R3_GPIO_Port GPIOC
#define RMII_REF_CLK_Pin GPIO_PIN_1
#define RMII_REF_CLK_GPIO_Port GPIOA
#define RMII_MDIO_Pin GPIO_PIN_2
#define RMII_MDIO_GPIO_Port GPIOA
#define ADC3_03_R1_Pin GPIO_PIN_3
#define ADC3_03_R1_GPIO_Port GPIOA
#define TIM3_CH1__Pin GPIO_PIN_6
#define TIM3_CH1__GPIO_Port GPIOA
#define RMII_CRS_DV_Pin GPIO_PIN_7
#define RMII_CRS_DV_GPIO_Port GPIOA
#define RMII_RXD0_Pin GPIO_PIN_4
#define RMII_RXD0_GPIO_Port GPIOC
#define RMII_RXD1_Pin GPIO_PIN_5
#define RMII_RXD1_GPIO_Port GPIOC
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define LED_8_Pin GPIO_PIN_13
#define LED_8_GPIO_Port GPIOF
#define LED_5_Pin GPIO_PIN_14
#define LED_5_GPIO_Port GPIOF
#define LED_3_Pin GPIO_PIN_15
#define LED_3_GPIO_Port GPIOF
#define LCD_DC_Pin GPIO_PIN_1
#define LCD_DC_GPIO_Port GPIOG
#define LED_7_Pin GPIO_PIN_9
#define LED_7_GPIO_Port GPIOE
#define LED_6_Pin GPIO_PIN_11
#define LED_6_GPIO_Port GPIOE
#define LED_4_Pin GPIO_PIN_13
#define LED_4_GPIO_Port GPIOE
#define RMII_TXD1_Pin GPIO_PIN_13
#define RMII_TXD1_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define STLK_RX_Pin GPIO_PIN_8
#define STLK_RX_GPIO_Port GPIOD
#define STLK_TX_Pin GPIO_PIN_9
#define STLK_TX_GPIO_Port GPIOD
#define USB_PowerSwitchOn_Pin GPIO_PIN_6
#define USB_PowerSwitchOn_GPIO_Port GPIOG
#define USB_OverCurrent_Pin GPIO_PIN_7
#define USB_OverCurrent_GPIO_Port GPIOG
#define TIM3_CH2__Pin GPIO_PIN_7
#define TIM3_CH2__GPIO_Port GPIOC
#define TIM3_CH3__Pin GPIO_PIN_8
#define TIM3_CH3__GPIO_Port GPIOC
#define EXTI09__Pin GPIO_PIN_9
#define EXTI09__GPIO_Port GPIOC
#define USB_SOF_Pin GPIO_PIN_8
#define USB_SOF_GPIO_Port GPIOA
#define USB_VBUS_Pin GPIO_PIN_9
#define USB_VBUS_GPIO_Port GPIOA
#define USB_ID_Pin GPIO_PIN_10
#define USB_ID_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define EXTI10__Pin GPIO_PIN_10
#define EXTI10__GPIO_Port GPIOC
#define EXTI10__EXTI_IRQn EXTI15_10_IRQn
#define EXTI11__Pin GPIO_PIN_11
#define EXTI11__GPIO_Port GPIOC
#define EXTI11__EXTI_IRQn EXTI15_10_IRQn
#define EXTI12__Pin GPIO_PIN_12
#define EXTI12__GPIO_Port GPIOC
#define EXTI12__EXTI_IRQn EXTI15_10_IRQn
#define LCD_RST_Pin GPIO_PIN_3
#define LCD_RST_GPIO_Port GPIOD
#define LCD_CS_Pin GPIO_PIN_4
#define LCD_CS_GPIO_Port GPIOD
#define LED_1_Pin GPIO_PIN_9
#define LED_1_GPIO_Port GPIOG
#define RMII_TX_EN_Pin GPIO_PIN_11
#define RMII_TX_EN_GPIO_Port GPIOG
#define RMII_TXD0_Pin GPIO_PIN_13
#define RMII_TXD0_GPIO_Port GPIOG
#define LED_2_Pin GPIO_PIN_14
#define LED_2_GPIO_Port GPIOG
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
