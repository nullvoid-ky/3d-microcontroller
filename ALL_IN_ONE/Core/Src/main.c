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
#include "eth.h"
#include "rng.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "ILI9341_Touchscreen.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"

#include "snow_tiger.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FSM_SIZE 17
typedef struct { GPIO_TypeDef *port; uint16_t pin; } GpioDef;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/*START LED HANDLER*/
static const GpioDef all_leds[] = {
  { LED1_GPIO_Port, LED1_Pin },
  { LED2_GPIO_Port, LED2_Pin },
  { LED3_GPIO_Port, LED3_Pin },
  { LED4_GPIO_Port, LED4_Pin },
  { LED5_GPIO_Port, LED5_Pin },
  { LED6_GPIO_Port, LED6_Pin },
  { LED7_GPIO_Port, LED7_Pin },
  { LED8_GPIO_Port, LED8_Pin }
};
static const GpioDef all_lds[] = {
  { LD1_GPIO_Port, LD1_Pin },
  { LD2_GPIO_Port, LD2_Pin },
  { LD3_GPIO_Port, LD3_Pin },
};
// Edit by your self
// Edit FSM_SIZE IF CHANGED SIZE
//static const uint8_t SEQ1[FSM_SIZE][8] = {
//		{1,0,0,0,0,0,0,1},
//		{0,1,0,0,0,0,1,0},
//		{0,0,1,0,0,1,0,0},
//		{0,0,0,1,1,0,0,0},
//};
// 0 - 15
// Edit by your self
// Edit FSM_SIZE IF CHANGED SIZE
static const uint8_t SEQ2[FSM_SIZE][8] = {
{0,0,0,0,0,0,0,1},
{0,0,0,0,0,0,1,0},
{0,0,0,0,0,0,1,1},
{0,0,0,0,0,1,0,0},
{0,0,0,0,0,1,0,1},
{0,0,0,0,0,1,1,0},
{0,0,0,0,0,1,1,1},
{0,0,0,0,1,0,0,0},
{0,0,0,0,1,0,0,1},
{0,0,0,0,1,0,1,0},
{0,0,0,0,1,0,1,1},
{0,0,0,0,1,1,0,0},
{0,0,0,0,1,1,0,1},
{0,0,0,0,1,1,1,0},
{0,0,0,0,1,1,1,1}
};
size_t idx = 0;
/*END LED HANDLER*/




/* --- App state --- */
static volatile uint32_t ms_tick = 0;     // นับ ms จาก TIM5
static volatile uint8_t paused = 0;       // 0 = นับอยู่, 1 = หยุดชั่วคราว
static volatile uint32_t pause_ms = 0;    // เวลาตั้งแต่เริ่ม pause (ms)
static volatile int current_val = 3030;   // เริ่มนับจาก 1002 (คู่)
static volatile int end_val = 3002;   // เริ่มนับจาก 1002 (คู่)
static volatile uint8_t done = 0;         // ครบงานแล้ว

/* PWM helper: TIM3 period จาก Cube = 65535 */
#define TIM3_ARR   (htim3.Init.Period)
static inline void RGB_Set(uint16_t r, uint16_t g, uint16_t b) {
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, r); // RED  PA6
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, g); // GREEN PC7
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, b); // BLUE  PC8
}
static inline void RGB_SetPercent(uint8_t r, uint8_t g, uint8_t b) {
    uint32_t rr = (uint32_t)r * TIM3_ARR / 100;
    uint32_t gg = (uint32_t)g * TIM3_ARR / 100;
    uint32_t bb = (uint32_t)b * TIM3_ARR / 100;
    RGB_Set((uint16_t)rr, (uint16_t)gg, (uint16_t)bb);
}




/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

static void set_led(const GpioDef led, GPIO_PinState s)
{
	HAL_GPIO_WritePin(led.port, led.pin, s);
}
static void LED_FSM(size_t *idx, const GpioDef all_leds[8], const uint8_t SEQ[FSM_SIZE][8]);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_ADC3_Init();
  MX_RNG_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM5_Init();
  MX_SPI5_Init();
  /* USER CODE BEGIN 2 */


  uint16_t ADC_MIN = 80;
  uint16_t ADC_MAX = 4020;
  uint16_t ADC_PERIOD = 4;
  uint16_t ADC_SCALE = (ADC_MAX - ADC_MIN) / ADC_PERIOD;


  uint16_t pos_x = 120;
  uint16_t pos_y = 160;
  uint16_t r = 20;
  uint16_t c = YELLOW;
  uint16_t now_state = 0;
  uint16_t bg = GREEN;

  ILI9341_Init();
  // EXTI 9..5 (เช่น PC9, PC10, PC11)
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  // EXTI 15..10 (เช่น PC13 USER, PC12)
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);


  /* Start TIM5 as 1ms tick */
  HAL_TIM_Base_Start_IT(&htim5);

  /* Start TIM3 PWM channels for RGB */
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // RED
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2); // GREEN
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3); // BLUE
  RGB_Set(0,0,0); // ปิดไฟแรกเริ่ม


//char temp_buff[16] ;
//char humid_buff[16] ;
//uint16_t t = 30;
//uint16_t h = 40;
ILI9341_Fill_Screen(WHITE);
//ILI9341_Draw_Filled_Circle(120, 180, 24, RED);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//	     if (HAL_ADC_Start(&hadc3) != HAL_OK) {
//	         Error_Handler();
//	     }
//
//	     /* Wait for conversion to complete (timeout 100 ms) */
//	     if (HAL_ADC_PollForConversion(&hadc3, 100) == HAL_OK) {
//	         uint32_t adc_val = HAL_ADC_GetValue(&hadc3);  // 0..4095
//	         if (adc_val <= ADC_MIN + ADC_SCALE * 1){
//	        	 r = 0;
//	        	 if (now_state != 1){
//	        		 ILI9341_Fill_Screen(bg);
//	            	 ILI9341_Draw_Filled_Circle(0, 0, 1, GREEN);
//	        		 ILI9341_Draw_Filled_Rectangle_Coord(pos_x, pos_y, pos_x+r, pos_y+r, c);
//	        	 }
//	        	 now_state = 1;
//	         }
//	         else if (adc_val <= ADC_MIN + ADC_SCALE * 2){
//	        	 r = 50;
//	        	 if (now_state != 2){
//	        		 ILI9341_Fill_Screen(bg);
//	            	 ILI9341_Draw_Filled_Circle(0, 0, 1, GREEN);
//	        		 ILI9341_Draw_Filled_Rectangle_Coord(pos_x, pos_y, pos_x+r, pos_y+r, c);
////	            	 ILI9341_Draw_Filled_Circle(pos_x, pos_y, r, c);
//	        	 }
//	        	 now_state = 2;
//	         }
//	         else if (adc_val <= ADC_MIN + ADC_SCALE * 3){
//	        	 r = 75;
//	        	 if (now_state != 3){
//	        		 ILI9341_Fill_Screen(bg);
//	            	 ILI9341_Draw_Filled_Circle(0, 0, 1, GREEN);
//	        		 ILI9341_Draw_Filled_Rectangle_Coord(pos_x, pos_y, pos_x+r, pos_y+r, c);
////	            	 ILI9341_Draw_Filled_Circle(pos_x, pos_y, r, c);
//	        	 }
//	        	 now_state = 3;
//	         }
//	         else {
//	        	 r = 100;
//	        	 if (now_state != 4){
//	        		 ILI9341_Fill_Screen(bg);
//	            	 ILI9341_Draw_Filled_Circle(0, 0, 1, GREEN);
//	        		 ILI9341_Draw_Filled_Rectangle_Coord(pos_x, pos_y, pos_x+r, pos_y+r, c);
////	            	 ILI9341_Draw_Filled_Circle(pos_x, pos_y, r, c);
//	        	 }
//	        	 now_state = 4;
//	         }
//	         char buf[32];
//	         int len = snprintf(buf, sizeof(buf), "ADC3_IN3=%lu\tSTATE=%lu\r\n", adc_val, now_state);
//	         if (len > 0) {
//	             HAL_UART_Transmit(&huart3, (uint8_t*)buf, (uint16_t)len, 100);
//	         }
//	     }
//	     HAL_Delay(250);

//	  snprintf(temp_buff, sizeof(temp_buff), "%5.1lu C", t);
//	  snprintf(humid_buff, sizeof(humid_buff), "%5.1lu %%RH", h);
//	  ILI9341_Draw_Text(temp_buff, 10, 20, BLACK, 2, WHITE);
//	  ILI9341_Draw_Text(humid_buff, 150, 20, BLACK, 2, WHITE);
	  /*START LED CODE HANDLER*/
//	    LED_FSM(&idx, all_leds, SEQ2);
	  /*END LED CODE HANDLER*/

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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

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

    if (done) return;

    if (GPIO_Pin == EXTI_09_BTN1_Pin) {
        if (!paused) {
            paused = 1;
            pause_ms = 0;
//            RGB_V2
//            RGB_SetPercent(100, 0, 0);
        }
    }
}

static void LED_FSM(size_t *idx, const GpioDef all_leds[8], const uint8_t SEQ[FSM_SIZE][8])
{
    for (size_t j = 0; j < 8; ++j) {
        GPIO_PinState state = SEQ[*idx][j] ? GPIO_PIN_SET : GPIO_PIN_RESET;
        set_led(all_leds[j], state);
    }
    HAL_Delay(400);
    for (size_t j = 0; j < 8; ++j) {
        set_led(all_leds[j], GPIO_PIN_RESET);
    }
    HAL_Delay(400);
    *idx = *idx + 1;
    if (*idx == FSM_SIZE){
    	*idx = 0;
    }
}


extern UART_HandleTypeDef huart3;

static void uart_print_num(int v) {
    char buf[32];
    int n = snprintf(buf, sizeof(buf), "%d\r\n", v);
    HAL_UART_Transmit(&huart3, (uint8_t*)buf, (uint16_t)n, HAL_MAX_DELAY);
}

static void uart_print_stop(void) {
    const char *s = "STOP\r\n";
    HAL_UART_Transmit(&huart3, (uint8_t*)s, (uint16_t)strlen(s), HAL_MAX_DELAY);
}

/* Cube HAL calls this on timer update event */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (done) return;

    if (htim->Instance == TIM5) {
        ms_tick++;

        if (paused) {
            pause_ms++;

            if (pause_ms == 1)   {
//            	0-1
//            	RGB_V2
            	RGB_SetPercent(0, 100, 0);
            }
            if (pause_ms == 500) {
//            	1-2
//            	set_led(all_lds[2], GPIO_PIN_SET);
            	//            	RGB_V2
				RGB_SetPercent(0, 75, 0);
			}
            if (pause_ms == 2000) {
//            	2-3
//            	set_led(all_lds[1], GPIO_PIN_SET);
            	//            	RGB_V2
            	RGB_SetPercent(0, 50, 0);
            }
            if (pause_ms == 3000) {
//            	3-4
//            	set_led(all_lds[0], GPIO_PIN_SET);
            	//            	RGB_V2
            	RGB_SetPercent(0, 25, 0);
            }
            if (pause_ms == 4000) {

//            	set_led(all_lds[2], GPIO_PIN_RESET);
//            	set_led(all_lds[1], GPIO_PIN_RESET);
//            	set_led(all_lds[0], GPIO_PIN_RESET);
            	//            	RGB_V2
                RGB_SetPercent(0, 0, 0);
                paused = 0;
                pause_ms = 0;
                ms_tick = 0; // รีเซ็ต 1s accumulator ให้เที่ยงตรงก่อนนับต่อ
            }
            return;
        }

        /* กรณีปกติ: พิมพ์ทุก 1000ms */
        if (ms_tick >= 1000) {
            ms_tick = 0;

            /* พิมพ์ค่าปัจจุบัน */
            uart_print_num(current_val);

            /* เดินไปเลขคู่ถัดไป */
            current_val -= 2;

            /* จบที่ 1030 แล้ว STOP */
            if (current_val < end_val) {
                uart_print_stop();
                /* ปิด timer interrupt และหยุดโปรแกรม */
                HAL_TIM_Base_Stop_IT(&htim5);
                done = 1;
            }
        }
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
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
