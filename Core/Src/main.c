/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#include "Driver.h"
#include "ADC_serial.h"
#include "usbd_cdc_if.h"
#include <string.h>
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
const int Log2[129] = {8, 0, 1, 0, 2, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 4,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7};
int intensity_percentage[9] = {25, 25, 25, 25, 25, 25, 25, 25, 25}; // intensity value Rx from computer and set by user, from 10 tp 99
int Integration_Time = 1800;
uint8_t read_ccd_buffer[9][2088];
uint8_t read_ccd_send[9][129];
int usb_Rx_flag = 0;
int overwrite_flag = 0;
int num_frame = 2;
uint8_t LEDs = 0x01;
uint8_t LED_tmp = 0x01;
uint8_t Rx_Buffer[16];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
	if (htim == &htim1){
		TIM2->CNT = 66;
	}
}

void DMA_Callback(DMA_HandleTypeDef * hmda){
	if (hmda == &hdma_tim8_ch3){
		for(int i=32;i<2080;i++){
			if (overwrite_flag)
				read_ccd_send[Log2[LED_tmp]][(i-32)/16] = (read_ccd_buffer[Log2[LED_tmp]][i] >> 4) / num_frame;
			else
				read_ccd_send[Log2[LED_tmp]][(i-32)/16] = read_ccd_send[Log2[LED_tmp]][(i-32)/16] + (read_ccd_buffer[Log2[LED_tmp]][i] >> 4) / num_frame;
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim == &htim6){
		if (LEDs == 0x00)
			LEDs = 0x01;
		else
			LEDs <<= 1U;
		LED_set(LEDs);
		Switch2Special();
		Conf_set(intensity_percentage[Log2[LEDs]]);
		Switch2Normal();
		output_en();
	}
	if (htim == &htim5){
		TIM1->CNT = 0;
		if (LEDs != LED_tmp){
			overwrite_flag = 1;
			read_ccd_send[Log2[LED_tmp]][128] = Log2[LED_tmp];
			CDC_Transmit_FS(read_ccd_send[Log2[LED_tmp]], 129);
			LED_tmp = LEDs;
		}else
			overwrite_flag = 0;
		HAL_DMA_Start_IT(&hdma_tim8_ch3, (uint32_t) &GPIOC->IDR, (uint32_t) read_ccd_buffer[Log2[LED_tmp]], 2088);
	}
}

void USB_Rx_Message_interpreter(char *data){
	switch(data[0]){
		case 'S':// start command
			CDC_Transmit_FS((uint8_t *)"Start Command Recieved!\n", strlen("Start Command Recieved!\n"));
			break;
		case 'I':// new Integration Time value
			CDC_Transmit_FS((uint8_t *)"Integration Time Command Recieved!\n", strlen("Integration Time Command Recieved!\n"));
			Integration_Time = atoi(data+1);
			TIM5->ARR = Integration_Time;
//			TIM1->ARR = Integration_Time;
			TIM6->ARR = (Integration_Time - 1800) * 0.8242 + 25000; // implement with LUT
			num_frame = TIM6->ARR / TIM5->ARR;
			break;
		case 'G':// new LED Intensity 
			CDC_Transmit_FS((uint8_t *)"Gain Command Recieved!\n", strlen("Gain Command Recieved!\n"));
			intensity_percentage[data[1]-'1'] = atoi(data+3);
			if (intensity_percentage[data[1]-'1'] > 99)
				intensity_percentage[data[1]-'1'] = 99;
			else if (intensity_percentage[data[1]-'1'] < 25)
				intensity_percentage[data[1]-'1'] = 25;
			break;
		case 'E':// End command
			CDC_Transmit_FS((uint8_t *)"End Command Recieved!\n", strlen("End Command Recieved!\n"));
			break;
		case 'D': // Data requenst
			CDC_Transmit_FS(read_ccd_send[data[1]-'1'], 129);
			break;
		default: 
			CDC_Transmit_FS((uint8_t *)"Unknown Command!\n", strlen("Unknown Command!\n"));
			break;
	}
}

void HAL_SYSTICK_Callback(void){
	if (usb_Rx_flag == 1){
		usb_Rx_flag = 0;
		USB_Rx_Message_interpreter((char *)Rx_Buffer);
	}
	if ((HAL_GetTick() % 500) == 0)
		LD5_GPIO_Port->BSRR = ((LD5_GPIO_Port->ODR & LD5_Pin) << 16U) | (~LD5_GPIO_Port->ODR & LD5_Pin); // Toggle LD5
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
  MX_USB_DEVICE_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM6_Init();
  MX_TIM5_Init();
  MX_TIM1_Init();
  MX_TIM8_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	ADC_init();
	
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2); // CCD ROG
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4);
	TIM5->DIER |= 1U; // set UIE bit
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // CCD CLK
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // CDSCLK1
	HAL_TIM_OC_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1); // CDSCLK2
	HAL_TIM_OC_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3); // ADCCLK
	TIM8->DIER |= (1 << 11U); // set CC3DE bit
	HAL_TIM_Base_Start_IT(&htim6); // for LEDs
	
	
	hdma_tim8_ch3.XferCpltCallback = &DMA_Callback;	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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

#ifdef  USE_FULL_ASSERT
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
