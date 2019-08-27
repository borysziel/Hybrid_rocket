/*
 * SS_Borys_adc.c
 *
 *  Created on: 23.05.2018
 *      Author: Bartuœ
 */
#include "SS_Borys_adc.h"

#define BORYS_ADC_FREQ 50

uint16_t borys_voltage = 0;
uint32_t borys_tenso = 0;
uint16_t borys_scaled_tenso_av = 0;
uint16_t adc_data;
uint32_t adc_data_tens;
uint8_t n;
uint32_t tmp;
uint8_t data_flash[8];
void SS_borys_adc_init(void)
{
	uint32_t freq = HAL_RCC_GetPCLK2Freq();
	TIM_HandleTypeDef htim;
	htim.Instance = TIM8;
	htim.Init.Prescaler = (freq/1000)/BORYS_ADC_FREQ;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.Period = 999;
	htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim);
	HAL_TIM_Base_Start(&htim);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc_data, 1);

	uint32_t freq2 = HAL_RCC_GetPCLK1Freq();
	TIM_HandleTypeDef htim3;
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = (freq2/1000)/BORYS_ADC_FREQ;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 999;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim3);
	HAL_TIM_Base_Start(&htim3);
	HAL_ADC_Start_DMA(&hadc3, (uint32_t*)&adc_data_tens, 1);
}
uint16_t *SS_borys_adc_get_data_pointer(void)
{
	return adc_data;
}
void SS_MS56_calculate_tenso_flash(void)
{

		SS_s25fl_prepare_time_ms(data_flash, HAL_GetTick());
		data_flash[4] = (uint8_t)(adc_data_tens>>24);
		data_flash[5] = (uint8_t)(adc_data_tens>>16);
		data_flash[6] = (uint8_t)(adc_data_tens>>8);
		data_flash[7] = (uint8_t)(adc_data_tens);
		SS_s25fl_write_data8(data_flash, TENSO_THRUST);

}
void SS_borys_adc_compute_tenso(void)
{
	  borys_tenso=(float)(100*35.0*((3.3*adc_data_tens/4.0950)/247.42)/10.0);
	  if(n<10)
	  {
		  n++;
		  tmp += borys_tenso;
	  }
	  else
	  {

		  borys_scaled_tenso_av = tmp/1000.0;
		  n=0;
		  tmp = 0;
	  }
}

void SS_borys_adc_compute_voltage(void)
{

	 borys_voltage = (float)(adc_data * ((3.3/4095.0)*4.4))*100.0;
}
uint16_t SS_borys_adc_get_voltage(void)
{
	return borys_voltage;
}
uint16_t SS_borys_adc_get_tenso(void)
{
	return borys_tenso;
}
