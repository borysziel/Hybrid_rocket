
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "can.h"
#include "crc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "SS_MS5607.h"
#include "SS_Grazyna_xbee.h"
#include "SS_it.h"
#include "SS_support.h"
#include "SS_servos.h"
#include "math.h"
#include "SS_ignition.h"
#include "SS_can.h"
#include "SS_Borys_adc.h"
#include "SS_S25FL.h"
#include "SS_Borys_adc.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t Received;
uint32_t raw_tenso;
uint32_t scaled_tenso;
uint32_t tmp;
uint32_t scaled_tenso_av;
//uint32_t scaled_tenso_av_prev = 0;
uint32_t wbytes;
//FATFS fatfs;
//FIL measurement_file;
//FRESULT result;
uint8_t differenceTenso = 0;
int16_t close_servo_altitude = 2500;
int32_t average_altitude = 0;
struct MS5607 ms5607;
uint8_t SET_ALTITUDE = 0;
int32_t can_data;
int32_t ref_pres;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	 uint8_t fly_stage;


	 int32_t altitude_rocket = 0;
	 int32_t pressure_rocket = 0;
	 int16_t average_pressure = 0;
	 int32_t prev_avarage_altitude = 80000000;
	 uint8_t arrays[2];
	 uint8_t data[8];
	 uint8_t sprint_length;
	 uint8_t i = 0;
	 uint32_t counter_can = 0;
	 uint8_t counter = 0;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_ADC1_Init();
  MX_CAN1_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  MX_TIM6_Init();
  MX_TIM11_Init();
  MX_ADC3_Init();
  MX_TIM8_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */

  	 HAL_TIM_Base_Start_IT(&htim6);

  	 HAL_ADC_Start_IT(&hadc3);
  	 //HAL_ADC_Start(&hadc1);
	 SS_MS56_init(&ms5607);
	 SS_grazyna_init();
	 SS_can_start();
	 SS_s25fl_reset();
	 SS_borys_adc_init();
	 HAL_GPIO_WritePin(IGNITION_GPIO_Port,IGNITION_Pin,RESET);
	 SS_servo_init_all();
	 //close_servo_altitude = 2600;

	 /*  result = f_mount(&fatfs, "", 0);
  	result = f_open(&measurement_file, "Staszek.txt", FA_CREATE_ALWAYS | FA_WRITE);
  	result = f_write(&measurement_file, "chuuuuuj\n", 6, (void *)&wbytes);
  	result = f_close(&measurement_file);
  	SS_borys_adc_init();
*/
	  	while(i<=10)
		{
			SS_MS56_read_convert_non_polling(&ms5607);
			SS_MS56_calculate_average_press(&ms5607, 10);
			if(ms56_stage == 0)
				 i++;
		}
  		SS_MS56_set_ref_press(&ms5607);
  		ref_pres = SS_MS56_get_ref_press(&ms5607);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
/*
 *
	 		  	  HAL_ADC_PollForConversion(&hadc1,500);
	 		  	  raw_tenso=HAL_ADC_GetValue(&hadc1);
	 		  	  scaled_tenso=100.0*(200.0*((3.3*raw_tenso/4.0950)/247.42)/10.0);
	 		  	  HAL_ADC_Start(&hadc1);
	 		 	  if(n<20)
	 		 	  {
	 		 		  n++;
	 		 		  tmp += scaled_tenso;
	 			  }
	 		 	  else
	 		 	  {

	 		 		  scaled_tenso_av = tmp/2000.0;
	 		 		  n=0;
	 		 		  tmp = 0;
	 		 	  }

		  HAL_ADC_PollForConversion(&hadc1,500);
		  raw_tenso=HAL_ADC_GetValue(&hadc1);
		  scaled_tenso=100.0*(10.0*((3.3*raw_tenso/4.0950)/247.42)/9.0);
		  size = sprintf(data, "%d\n\r", scaled_tenso);
		  HAL_UART_Transmit(&huart2,data,size,500);
		  HAL_ADC_Start(&hadc1);
*/



	  	  //HAL_GPIO_TogglePin(LED4_GPIO_Port,LED4_Pin);
	  	  //SS_can_tx_no_data(CAN_CZAPLA_ON);
	  	  //HAL_Delay(1000);
	  	  //SS_can_tx_no_data(CAN_CZAPLA_OFF);
	  	  //HAL_Delay(1000);
		//SS_can_tx_no_data(CAN_ROMEK_TEST_CAN);
	  	  //SS_MS56_read_convert(&ms5607);

	  	  //SS_can_tx_no_data(CAN_SENSEI_INCLINATION);
	  	  //HAL_Delay(400);
	 // HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,SET);
	  	  if(SS_grazyna_get_valid_rcv_flag())
		  {
			  SS_grazyna_handle_received();
		  }

		  SS_MS56_read_convert_non_polling(&ms5607);

		  //ALTITUDE
		  ///////zmienic pod grazyne zeby dzialala, zwmienic pomiar ms5607 cisnienie podczas startu
		  ////////////
		  ////////////
		  //////////
		  ///////////
		  //////////



		  SS_servo_oxidizer_open();
		  HAL_Delay(2000);
		  SS_servo_oxidizer_close();
		  //PRESSURE
		  pressure_rocket = SS_MS56_get_read_convert(&ms5607);
		  SS_MS56_calculate_average_press(&ms5607,3);
		  average_pressure = SS_MS56_get_average_press(&ms5607);

		  altitude_rocket = SS_MS56_get_altitude(&ms5607);
	  	  SS_MS56_calculate_average_altitiude(&ms5607,3);
		  average_altitude = SS_MS56_get_average_altitude(&ms5607);
		  //HAL_UART_Transmit(&huart2,0x0F,20100);
  		  SS_MS56_calculate_altitude_flash(&ms5607);

		 /* if(TEST_OPEN_SERVO)
		  {
			  SS_can_tx_no_data(CAN_SENSEI_LECIM);
			  TEST_OPEN_SERVO=0;
		  }*/
		  if(ERASE_FLASH)
		  {
				HAL_GPIO_WritePin(LED5_GPIO_Port,LED5_Pin,SET);
				SS_s25fl_erase_full_chip();
				HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,SET);
				HAL_GPIO_WritePin(LED5_GPIO_Port,LED5_Pin,RESET);
		  }
	 	  if(WRITE_FLASH)
	  	  {
	 		  HAL_GPIO_WritePin(LED6_GPIO_Port,LED6_Pin,SET);
	  		  SS_MS56_calculate_average_press_flash(&ms5607,5);
	  		  SS_MS56_calculate_altitude_flash(&ms5607);
	  		  SS_MS56_calculate_average_temp_flash(&ms5607,5);
	 		  /*if(zmienna==1)
	 		  {
	  		  SS_MS56_calculate_tenso_flash();
	  		  zmienna = 0;
	 		  }*/

	  	  }
	 	  if(TEST_STASZEK_SENSEI_MES)
	 	  {

	 		  if(counter_can >= 5000)
	 		  {
		 		  SS_can_tx_no_data(CAN_STASZEK_SENSEI_MES);
		 		  TEST_STASZEK_SENSEI_MES = 0;
		 		  counter_can = 0;
		 		  counter++;
		 		  if(counter == 5)
		 		  {
		 			  TEST_STASZEK_SENSEI_MES = 0;
		 		  }
	 		  }
	 		  counter_can++;

	 	  }
	 	 /* if(SERVO_OPENED)
	  	  {

	  		  int8_t cnt1 = 1;
	  		  if(prev_avarage_altitude > average_altitude)
	  		  {
	  			  //HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,SET);
	  			  cnt1++;
	  			  prev_avarage_altitude = average_altitude;
	  		  }
	  		  else if(cnt1>0) cnt1--;

	  		  if(cnt1 > 10)
	  		  {
	  			SS_servo_oxidizer_close();
	  			SS_can_tx_no_data(CAN_SENSEI_ENGINE_OFF);
	  			//HAL_GPIO_WritePin(LED6_GPIO_Port,LED6_Pin,SET);
	  		  }
	  	  }*/
	 	  /*if(SERVO_OPENED)
	 	  {

	 	 	 if(average_altitude > close_servo_altitude)
				{
				 SS_servo_oxidizer_close();
				 //SS_can_tx_no_data(CAN_SENSEI_ENGINE_OFF);
				}
	 	  }*/
//	 	  if(STASZEK_ZAMYKAMY)
//	 	  {
//	 		 SS_servo_oxidizer_close();
//	 	  }


  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Activate the Over-Drive mode 
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
