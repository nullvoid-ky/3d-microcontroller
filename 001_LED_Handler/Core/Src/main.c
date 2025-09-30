/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "eth.h"
#include "i2c.h"
#include "rng.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stddef.h>  /* for size_t */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

  // Your sequence: each row = LED_1..LED_8 as 0/1
  static const uint8_t SEQ[16][8] = {
		  {1,1,1,1,0,0,0,0},
		  {1,1,1,0,0,0,0,0},
		  {1,1,0,1,0,0,0,0},
		  {1,1,0,0,0,0,0,0},
		  {1,0,1,1,0,0,0,0},
		  {1,0,1,0,0,0,0,0},
		  {1,0,0,1,0,0,0,0},
		  {1,0,0,0,0,0,0,0},
		  {0,1,1,1,0,0,0,0},
		  {0,1,1,0,0,0,0,0},
		  {0,1,0,1,0,0,0,0},
		  {0,1,0,0,0,0,0,0},
		  {0,0,1,1,0,0,0,0},
		  {0,0,1,0,0,0,0,0},
		  {0,0,0,1,0,0,0,0},
		  {0,0,0,0,0,0,0,0},
  };

#define BTN1_PIN   EXTI09__Pin
#define BTN1_PORT  EXTI09__GPIO_Port

#define BTN2_PIN   EXTI10__Pin
#define BTN2_PORT  EXTI10__GPIO_Port

#define BTN3_PIN   EXTI11__Pin
#define BTN3_PORT  EXTI11__GPIO_Port

#define BTN4_PIN   EXTI12__Pin
#define BTN4_PORT  EXTI12__GPIO_Port


/* ===== Canonical GPIO name aliases for LEDs =====
   Map CubeMX-generated symbols (LED_1_Pin, etc.) to uniform PORT_/PIN_ names.
   If a label doesn't exist in your project it simply won't be used. */

#ifdef LED_1_Pin
  #define PORT_LED1   LED_1_GPIO_Port
  #define PIN_LED1    LED_1_Pin
#endif
#ifdef LED_2_Pin
  #define PORT_LED2   LED_2_GPIO_Port
  #define PIN_LED2    LED_2_Pin
#endif
#ifdef LED_3_Pin
  #define PORT_LED3   LED_3_GPIO_Port
  #define PIN_LED3    LED_3_Pin
#endif
#ifdef LED_4_Pin
  #define PORT_LED4   LED_4_GPIO_Port
  #define PIN_LED4    LED_4_Pin
#endif
#ifdef LED_5_Pin
  #define PORT_LED5   LED_5_GPIO_Port
  #define PIN_LED5    LED_5_Pin
#endif
#ifdef LED_6_Pin
  #define PORT_LED6   LED_6_GPIO_Port
  #define PIN_LED6    LED_6_Pin
#endif
#ifdef LED_7_Pin
  #define PORT_LED7   LED_7_GPIO_Port
  #define PIN_LED7    LED_7_Pin
#endif
#ifdef LED_8_Pin
  #define PORT_LED8   LED_8_GPIO_Port
  #define PIN_LED8    LED_8_Pin
#endif

#ifdef LD1_Pin
  #define PORT_LD1    LD1_GPIO_Port
  #define PIN_LD1     LD1_Pin
#endif
#ifdef LD2_Pin
  #define PORT_LD2    LD2_GPIO_Port
  #define PIN_LD2     LD2_Pin
#endif
#ifdef LD3_Pin
  #define PORT_LD3    LD3_GPIO_Port
  #define PIN_LD3     LD3_Pin
#endif

/* Small holder for GPIO pairs */
typedef struct { GPIO_TypeDef *port; uint16_t pin; } GpioDef;

/* A single array that contains ALL LEDs (LED_1..LED_8 and LD1..LD3) */
static const GpioDef all_leds[] = {
#ifdef PORT_LED1
  { PORT_LED1, PIN_LED1 },
#endif
#ifdef PORT_LED2
  { PORT_LED2, PIN_LED2 },
#endif
#ifdef PORT_LED3
  { PORT_LED3, PIN_LED3 },
#endif
#ifdef PORT_LED4
  { PORT_LED4, PIN_LED4 },
#endif
#ifdef PORT_LED5
  { PORT_LED5, PIN_LED5 },
#endif
#ifdef PORT_LED6
  { PORT_LED6, PIN_LED6 },
#endif
#ifdef PORT_LED7
  { PORT_LED7, PIN_LED7 },
#endif
#ifdef PORT_LED8
  { PORT_LED8, PIN_LED8 },
#endif
#ifdef PORT_LD1
  { PORT_LD1, PIN_LD1 },
#endif
#ifdef PORT_LD2
  { PORT_LD2, PIN_LD2 },
#endif
#ifdef PORT_LD3
  { PORT_LD3, PIN_LD3 },
#endif
};

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
UART_HandleTypeDef huart3;
size_t idx = 0;
uint16_t rounds = 16;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* Write same state to an array of GPIOs */
static void set_array(const GpioDef *arr, size_t count, GPIO_PinState s)
{
  for (size_t i = 0; i < count; ++i) {
    HAL_GPIO_WritePin(arr[i].port, arr[i].pin, s);
  }
}


/* Write same state to an array of GPIOs */
static void set_led(const GpioDef led, GPIO_PinState s)
{
	HAL_GPIO_WritePin(led.port, led.pin, s);
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_ADC3_Init();
  MX_RNG_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
  /* Optional: ensure all LEDs start OFF */
  set_array(all_leds, sizeof(all_leds)/sizeof(all_leds[0]), GPIO_PIN_RESET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */



//
//	  char msg[96];
//	  int n = snprintf(msg, sizeof(msg),
//			  "LED%d is %d\r\n",
//			  j+1,
//			  state
//			  );
//	  size_t len = (n < 0) ? 0 : (n >= (int)sizeof(msg) ? sizeof(msg) - 1 : (size_t)n);
//	  HAL_UART_Transmit(&huart3, (uint8_t*)msg, (uint16_t)len, 1000);
//
//
  for(int i = 0 ; i < 16; i ++){
	  idx = i;
	for (size_t j = 0; j < 8; ++j) {
	GPIO_PinState state = SEQ[idx][j] ? GPIO_PIN_SET : GPIO_PIN_RESET;
	  set_led(all_leds[j] ,state);
	}
	HAL_Delay(400);
  }
  while(1){}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}
/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  // simple debounce shared by all buttons
  static uint32_t last_tick = 0;
  uint32_t now = HAL_GetTick();
  if (now - last_tick < 200) return;   // ~200 ms
  last_tick = now;


  // persist current frame index across button presses
  // how many LEDs we actually have in all_leds (cap at 8)
  if (GPIO_Pin == BTN2_PIN) {
    // clear all & reset sequence to the stadrt
    for (size_t j = 0; j < 8; ++j) {
      HAL_GPIO_WritePin(all_leds[j].port, all_leds[j].pin, GPIO_PIN_RESET);
    }
  }

  // BTN3/BTN4 free for other actions if you want later
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  __disable_irq();
  while (1)
  {
    /* Optionally flash a board LED fast here to indicate a fault */
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
