/*
 * SS_MS6507.c
 *
 *  Created on: 22.12.2017
 *      Author: Tomasz
 */
#include "SS_MS5607.h"
#include "spi.h"
#include "stm32f4xx_hal.h"

uint8_t MS56_WAIT_READY = 0;
uint8_t ms56_stage = 0;
uint8_t data_flash[8];
uint8_t tick_start = 0;

void SS_MS56_CS_ENABLE(void)
{
	HAL_GPIO_WritePin(MS56_CS_GPIO_Port, MS56_CS_Pin, RESET);
}
void SS_MS56_CS_DISABLE(void)
{
	HAL_GPIO_WritePin(MS56_CS_GPIO_Port, MS56_CS_Pin, SET);
}


void SS_set_close_altitude(struct MS5607 *ms5607,uint16_t value)
{
	ms5607->set_altitude = value;
}
int32_t SS_get_close_altitude(struct MS5607 *ms5607)
{
	return ms5607->set_altitude;
}
enum RESULT SS_MS56_reset(void)
{
	uint8_t buff = MS56_RESET;
	SS_MS56_CS_ENABLE();
	HAL_SPI_TransmitReceive(&HSPI_MS56, &buff, &buff, 1, 200);
	while(HAL_SPI_GetState(&HSPI_MS56) == HAL_SPI_STATE_BUSY);
	SS_MS56_CS_DISABLE();
	HAL_Delay(3);
	if(254 == buff)
		return 0;
	else
		return 1;
}
enum RESULT SS_MS56_prom_read(struct MS5607 *ms5607)
{
	uint8_t buff = MS56_PROM_READ_BASE;
	uint8_t buffRX[2];
	for(uint8_t i=0; i<8; i++)
	{
		SS_MS56_CS_ENABLE();
		HAL_SPI_Transmit(&HSPI_MS56, &buff, 1, 200);
		while(HAL_SPI_GetState(&HSPI_MS56) == HAL_SPI_STATE_BUSY_TX);

		HAL_SPI_Receive(&HSPI_MS56, buffRX, 2, 200);
		while(HAL_SPI_GetState(&HSPI_MS56) == HAL_SPI_STATE_BUSY_RX);
		if(HAL_SPI_GetState(&HSPI_MS56) == HAL_SPI_STATE_ERROR)
			return 1;
		SS_MS56_CS_DISABLE();
		buff += 2;
		ms5607->PROM[i] = buffRX[1] + (uint16_t)(buffRX[0]<<8);
	}
	return 0;
}
enum RESULT SS_MS56_adc_read(uint32_t *data)
{
	uint8_t buffTX = MS56_ADC_READ;
	uint8_t buffRX[3];
	SS_MS56_CS_ENABLE();
	HAL_SPI_Transmit(&HSPI_MS56, &buffTX, 1, 200);
	while(HAL_SPI_GetState(&HSPI_MS56) == HAL_SPI_STATE_BUSY_TX);

	HAL_SPI_Receive(&HSPI_MS56, buffRX, 3, 200);
	while(HAL_SPI_GetState(&HSPI_MS56) == HAL_SPI_STATE_BUSY_RX);

	SS_MS56_CS_DISABLE();
	*data = buffRX[2] + ((uint16_t)buffRX[1]<<8) + ((uint32_t)buffRX[0]<<16);
	if(HAL_SPI_GetState(&HSPI_MS56) == HAL_SPI_STATE_ERROR)
		return 1;
	else
		return 0;
}
enum RESULT SS_MS56_convertion_press_start(struct MS5607 *ms5607)
{
	SS_MS56_CS_ENABLE();
	HAL_SPI_Transmit(&HSPI_MS56, &ms5607->pressOSR, 1, 200);
	while(HAL_SPI_GetState(&HSPI_MS56) == HAL_SPI_STATE_BUSY_TX);

	SS_MS56_CS_DISABLE();
	if(HAL_SPI_GetState(&HSPI_MS56) == HAL_SPI_STATE_ERROR)
		return 1;
	else
		return 0;
}
enum RESULT SS_MS56_convertion_temp_start(struct MS5607 *ms5607)
{
	SS_MS56_CS_ENABLE();
	HAL_SPI_Transmit(&HSPI_MS56, &ms5607->tempOSR, 1, 200);
	while(HAL_SPI_GetState(&HSPI_MS56) == HAL_SPI_STATE_BUSY_TX);

	SS_MS56_CS_DISABLE();
	if(HAL_SPI_GetState(&HSPI_MS56) == HAL_SPI_STATE_ERROR)
		return 1;
	else
		return 0;
}
void SS_MS56_set_wait_it(uint8_t type)
{
	switch (type) {
		case MS56_PRESS_256:
			MS56_WAIT_READY = 1+1;
			break;
		case MS56_PRESS_512:
			MS56_WAIT_READY = 2+1;
			break;
		case MS56_PRESS_1024:
			MS56_WAIT_READY = 3+1;
			break;
		case MS56_PRESS_2048:
			MS56_WAIT_READY = 5+1;
			break;
		case MS56_PRESS_4096:
			MS56_WAIT_READY = 9+1;
			break;
		case MS56_TEMP_256:
			MS56_WAIT_READY = 1+1;
			break;
		case MS56_TEMP_512:
			MS56_WAIT_READY = 2+1;
			break;
		case MS56_TEMP_1024:
			MS56_WAIT_READY = 3+1;
			break;
		case MS56_TEMP_2048:
			MS56_WAIT_READY = 5+1;
			break;
		case MS56_TEMP_4096:
			MS56_WAIT_READY = 9+1;
			break;
	}
}
void SS_MS56_wait(uint8_t type)
{
	switch (type) {
		case MS56_PRESS_256:
			HAL_Delay(1);
			break;
		case MS56_PRESS_512:
			HAL_Delay(2);
			break;
		case MS56_PRESS_1024:
			HAL_Delay(3);
			break;
		case MS56_PRESS_2048:
			HAL_Delay(5);
			break;
		case MS56_PRESS_4096:
			HAL_Delay(9);
			break;
		case MS56_TEMP_256:
			HAL_Delay(1);
			break;
		case MS56_TEMP_512:
			HAL_Delay(2);
			break;
		case MS56_TEMP_1024:
			HAL_Delay(3);
			break;
		case MS56_TEMP_2048:
			HAL_Delay(5);
			break;
		case MS56_TEMP_4096:
			HAL_Delay(9);
			break;
	}
}
void SS_MS56_DMA_read_convert(struct MS5607 *ms5607)
{
	int64_t dT;
	int64_t OFF, SENS;
	SS_MS56_convertion_press_start(ms5607);
	SS_MS56_wait(ms5607->pressOSR);


}

void SS_MS56_read_convert(struct MS5607 *ms5607)
{
	int64_t dT;
	int64_t OFF, SENS;

	SS_MS56_convertion_press_start(ms5607);
	SS_MS56_wait(ms5607->pressOSR);
	SS_MS56_adc_read(&ms5607->uncomp_press);

	SS_MS56_convertion_temp_start(ms5607);
	SS_MS56_wait(ms5607->tempOSR);
	SS_MS56_adc_read(&ms5607->uncomp_temp);

	dT = ms5607->uncomp_temp - ((int32_t)ms5607->PROM[5]<<8);
	ms5607->temp = 2000 + (((int64_t)ms5607->PROM[6]*dT)>>23);
	OFF = ((int64_t)ms5607->PROM[2]<<17) + (((int64_t)ms5607->PROM[4]*dT)>>6);
	SENS = ((int64_t)ms5607->PROM[1]<<16) + ((int64_t)(ms5607->PROM[3]*dT)>>7);
	ms5607->press = ((((int64_t)ms5607->uncomp_press * SENS)>>21) - OFF)>>15;
}
int32_t SS_MS56_get_read_convert(struct MS5607 *ms5607)
{
	return ms5607->press;
}

void SS_MS56_read_convert_non_polling(struct MS5607 *ms5607)
{
	static int64_t dT;
	static int64_t OFF, SENS;
	switch (ms56_stage) {
		case 0:
			SS_MS56_convertion_press_start(ms5607);
			SS_MS56_set_wait_it(ms5607->pressOSR);
			ms56_stage++;
			break;
		case 2:
			SS_MS56_adc_read(&ms5607->uncomp_press);
			SS_MS56_convertion_temp_start(ms5607);
			SS_MS56_set_wait_it(ms5607->tempOSR);
			ms56_stage++;
			break;
		case 4:
			SS_MS56_adc_read(&ms5607->uncomp_temp);
			dT = ms5607->uncomp_temp - ((int32_t)ms5607->PROM[5]<<8);
			ms5607->temp = 2000 + (((int64_t)ms5607->PROM[6]*dT)>>23);
			OFF = ((int64_t)ms5607->PROM[2]<<17) + (((int64_t)ms5607->PROM[4]*dT)>>6);
			SENS = ((int64_t)ms5607->PROM[1]<<16) + ((int64_t)(ms5607->PROM[3]*dT)>>7);
			ms5607->press = ((((int64_t)ms5607->uncomp_press * SENS)>>21) - OFF)>>15;
			ms56_stage = 0;
			break;
		default:
			break;
	}
}
void SS_MS56_init(struct MS5607 *ms5607)
{
	SS_MS56_reset();
	ms5607->pressOSR = MS56_PRESS_4096;
	ms5607->tempOSR = MS56_TEMP_4096;
	ms5607->refPress = 101000;
	SS_MS56_prom_read(ms5607);
}
void SS_MS56_set_ref_press(struct MS5607 *ms5607)
{
	ms5607->refPress= ms5607->average_press;
}
int32_t SS_MS56_get_ref_press(struct MS5607 *ms5607)
{
	return ms5607->refPress;
}
int32_t SS_MS56_get_altitude(struct MS5607 *ms5607)
{
	ms5607->altitude=44330*(1-pow(((double)ms5607->press/(double)ms5607->refPress),(1/5.255)));
	return ms5607->altitude;
}
void SS_MS56_decrement_wait_ready(void)
{
	if(MS56_WAIT_READY == 1)
	{
		ms56_stage++;
		MS56_WAIT_READY = 0;
	}
	if(MS56_WAIT_READY>1)
		MS56_WAIT_READY--;
}
void SS_MS56_calculate_average_press(struct MS5607 * ms5607, uint8_t average_cnt)
{
	static int32_t sum = 0;
	static uint8_t counter = 0;
	if(!ms56_stage)    //stage = 0 means new record accured
	{
		sum += ms5607->press;
		counter++;
	}
	if(counter == average_cnt)
	{
		counter = 0;
		ms5607->average_press = sum / average_cnt;
		sum = 0;

	}
}
int32_t SS_MS56_get_average_press(struct MS5607 * ms5607)
{
	return ms5607->average_press;
}
void SS_MS56_calculate_average_altitiude(struct MS5607 * ms5607, uint8_t average_cnt)
{
	static int32_t sum = 0;
	static uint8_t counter = 0;
	if(!ms56_stage)
	{
		sum += ms5607->altitude;
		counter++;
	}
	if(counter == average_cnt)
	{
		counter = 0;
		ms5607->average_altitude = sum / average_cnt;
		sum = 0;

	}
}
void SS_MS56_calculate_average_altitiude2(struct MS5607 * ms5607, uint8_t average_cnt)
{
	static int32_t sum = 0;
	static uint8_t counter = 0;
	if(!ms56_stage)    //stage = 0 means new record accured
	{
		sum += ms5607->press;
		counter++;
	}
	if(counter == average_cnt)
	{
		counter = 0;
		ms5607->average_press = sum / average_cnt;
		sum = 0;
		ms5607->average_altitude=44330*(1-pow(((double)ms5607->average_press/(double)ms5607->refPress),(1/5.255)));

	}
}
uint32_t SS_MS56_get_average_altitude(struct MS5607 * ms5607)
{

	return ms5607->average_altitude;
}
void SS_MS56_calculate_average_temp(struct MS5607 * ms5607, uint8_t average_cnt)
{
	static int32_t sum = 0;
	static uint8_t counter = 0;
	if(!ms56_stage)    //stage = 0 means new record accured
	{
		sum += ms5607->temp;
		counter++;
	}
	if(counter == average_cnt)
	{
		counter = 0;
		ms5607->average_temp = sum / average_cnt;
		sum = 0;
	}
}
void SS_MS56_calculate_average_temp_flash(struct MS5607 *ms5607, uint8_t average_cnt)
{
	static int32_t sum = 0;
	static uint8_t counter = 0;
	if(!ms56_stage)    //stage = 0 means new record accured
	{
		sum += ms5607->temp;
		counter++;
	}
	if(counter == average_cnt)
	{
		counter = 0;
		ms5607->average_temp = sum / average_cnt;
		SS_s25fl_prepare_time_ms(data_flash, HAL_GetTick()-tick_start);
		data_flash[4] = (uint8_t)(ms5607->average_temp>>24);
		data_flash[5] = (uint8_t)(ms5607->average_temp>>16);
		data_flash[6] = (uint8_t)(ms5607->average_temp>>8);
		data_flash[7] = (uint8_t)(ms5607->average_temp);
		SS_s25fl_write_data8(data_flash, MS56_TEMP);
		sum = 0;
	}
}
void SS_MS56_calculate_altitude_flash(struct MS5607 *ms5607)
{
	if(!ms56_stage)    //stage = 0 means new record accured
	{
		SS_s25fl_prepare_time_ms(data_flash, HAL_GetTick()-tick_start);
		data_flash[4] = (uint8_t)(ms5607->altitude>>24);
		data_flash[5] = (uint8_t)(ms5607->altitude>>16);
		data_flash[6] = (uint8_t)(ms5607->altitude>>8);
		data_flash[7] = (uint8_t)(ms5607->altitude);
		SS_s25fl_write_data8(data_flash, MS56_ALTITUDE);
	}

}
void SS_MS56_calculate_average_press_flash(struct MS5607 *ms5607, uint8_t average_cnt)
{
	static int32_t sum = 0;
	static uint8_t counter = 0;
	if(!ms56_stage)    //stage = 0 means new record accured
	{
		sum += ms5607->press;
		counter++;
	}
	if(counter == average_cnt)
	{
		counter = 0;
		ms5607->average_press = sum / average_cnt;
		SS_s25fl_prepare_time_ms(data_flash, HAL_GetTick()-tick_start);
		data_flash[4] = (uint8_t)(ms5607->average_press>>24);
		data_flash[5] = (uint8_t)(ms5607->average_press>>16);
		data_flash[6] = (uint8_t)(ms5607->average_press>>8);
		data_flash[7] = (uint8_t)(ms5607->average_press);
		SS_s25fl_write_data8(data_flash, MS56_BARO);
		sum = 0;
	}
}
