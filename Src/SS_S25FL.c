/*
 * SS_S25FL.c
 *
 *  Created on: 27.05.2018
 *      Author: Bartuœ
 */


#include "SS_S25FL.h"


uint8_t WAIT_FOR_TX_END = 0; WAIT_FOR_RX_END = 0;
//uint8_t WRITE_FLASH = 0;
//uint8_t STOP_WRITE_FLASH = 0;
char tx_char[30];

uint8_t flash_bufor1[256], flash_bufor2[256];
uint8_t *flash_bufor_pointer = flash_bufor1;
uint16_t flash_bufor_inc = 0;
uint32_t flash_page_inc = 16;
void SS_s25fl_select(void)
{
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, RESET);
}
void SS_s25fl_deselect(void)
{
	HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, SET);
}
void SS_s25fl_reset(void)
{
	HAL_GPIO_WritePin(FLASH_RESET_GPIO_Port, FLASH_RESET_Pin, 0);
	HAL_Delay(200);
	HAL_GPIO_WritePin(FLASH_RESET_GPIO_Port, FLASH_RESET_Pin, 1);
}
uint8_t SS_s25fl_read_status(void)
{
	uint8_t reg = 0x05;
	SS_s25fl_select();
	HAL_SPI_Transmit(&hspi2, &reg, 1, 200);
	HAL_SPI_Receive(&hspi2, &reg, 1, 200);
	SS_s25fl_deselect();
	return reg;
}
uint8_t SS_s25fl_get_id(void)
{
	uint8_t dummy [3], ID = 0x9F;
	SS_s25fl_select();
	HAL_SPI_Transmit(&hspi2, &ID, 1, 200);
	HAL_SPI_Receive(&hspi2, dummy, 2, 200);
	HAL_SPI_Receive(&hspi2, &ID, 1, 200);
	SS_s25fl_deselect();
	return ID;
}
void SS_s25fl_write_enable(void)
{
	uint8_t reg = 0x06;
	SS_s25fl_select();
	HAL_SPI_Transmit(&hspi2, &reg, 1, 200);
	SS_s25fl_deselect();
	while(SS_s25fl_check_write_progress());
}
uint8_t SS_s25fl_check_write_progress(void)
{
	return (SS_s25fl_read_status() & 0x01);
}
void SS_s25fl_erase_4k(uint32_t adress)
{
	uint8_t reg = 0x21;
	while(SS_s25fl_check_write_progress());
	SS_s25fl_write_enable();
	SS_s25fl_select();
	HAL_SPI_Transmit(&hspi2, &reg, 1, 200);
	reg = adress>>24;
	HAL_SPI_Transmit(&hspi2, &reg, 1, 200);
	reg = adress>>16;
	HAL_SPI_Transmit(&hspi2, &reg, 1, 200);
	reg = adress>>8;
	HAL_SPI_Transmit(&hspi2, &reg, 1, 200);
	reg = adress & 0xff;
	HAL_SPI_Transmit(&hspi2, &reg, 1, 200);
	SS_s25fl_deselect();
	while(SS_s25fl_check_write_progress());
}
void SS_s25fl_erase_64k(uint32_t adress)
{
	uint8_t reg = 0xDC;
	while(SS_s25fl_check_write_progress());
	SS_s25fl_write_enable();
	SS_s25fl_select();
	HAL_SPI_Transmit(&hspi2, &reg, 1, 200);
	reg = adress>>24;
	HAL_SPI_Transmit(&hspi2, &reg, 1, 200);
	reg = adress>>16;
	HAL_SPI_Transmit(&hspi2, &reg, 1, 200);
	reg = adress>>8;
	HAL_SPI_Transmit(&hspi2, &reg, 1, 200);
	reg = adress & 0xff;
	HAL_SPI_Transmit(&hspi2, &reg, 1, 200);
	SS_s25fl_deselect();
	while(SS_s25fl_check_write_progress());
}
void SS_s25fl_write_page(uint32_t page, uint8_t *array, uint16_t length)
{

	uint8_t buf = 0x12;
	uint32_t adress = page << 9;
	while(SS_s25fl_check_write_progress());
	SS_s25fl_write_enable();
	SS_s25fl_select();
	HAL_SPI_Transmit(&hspi2, &buf, 1, 200);
	buf = (adress>>24) & 0xff;
	HAL_SPI_Transmit(&hspi2, &buf, 1, 200);
	buf = (adress>>16) & 0xff;
	HAL_SPI_Transmit(&hspi2, &buf, 1, 200);
	buf = (adress>>8) & 0xff;
	HAL_SPI_Transmit(&hspi2, &buf, 1, 200);
	buf = (adress) & 0xff;
	HAL_SPI_Transmit(&hspi2, &buf, 1, 200);
	HAL_SPI_Transmit(&hspi2, array, length, 500);
	SS_s25fl_deselect();
}
void SS_s25fl_write_page_dma(uint32_t page, uint8_t *array, uint16_t length)
{

	uint8_t buf = 0x12;
	uint32_t adress = page << 9;
	while(SS_s25fl_check_write_progress());
	SS_s25fl_write_enable();
	SS_s25fl_select();
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX);
	HAL_SPI_Transmit_DMA(&hspi2, &buf, 1);
	buf = (adress>>24) & 0xff;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX);
	HAL_SPI_Transmit_DMA(&hspi2, &buf, 1);
	buf = (adress>>16) & 0xff;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX);
	HAL_SPI_Transmit_DMA(&hspi2, &buf, 1);
	buf = (adress>>8) & 0xff;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX);
	HAL_SPI_Transmit_DMA(&hspi2, &buf, 1);
	buf = (adress) & 0xff;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX);
	HAL_SPI_Transmit_DMA(&hspi2, &buf, 1);
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX);
	HAL_SPI_Transmit_DMA(&hspi2, array, length);
	WAIT_FOR_TX_END = 1;
}
void SS_s25fl_read_page(uint32_t page, uint8_t *array, uint16_t length)
{

	uint8_t buf = 0x13;
	uint32_t adress = page << 9;
	while(SS_s25fl_check_write_progress());
	SS_s25fl_write_enable();
	SS_s25fl_select();
	HAL_SPI_Transmit(&hspi2, &buf, 1, 200);
	buf = (adress>>24) & 0xff;
	HAL_SPI_Transmit(&hspi2, &buf, 1, 200);
	buf = (adress>>16) & 0xff;
	HAL_SPI_Transmit(&hspi2, &buf, 1, 200);
	buf = (adress>>8) & 0xff;
	HAL_SPI_Transmit(&hspi2, &buf, 1, 200);
	buf = (adress) & 0xff;
	HAL_SPI_Transmit(&hspi2, &buf, 1, 200);
	HAL_SPI_Receive(&hspi2, array, length, 500);
	SS_s25fl_deselect();
}
void SS_s25fl_read_page_dma(uint32_t page, uint8_t *array, uint16_t length)
{

	uint8_t buf = 0x13;
	uint32_t adress = page << 9;
	while(SS_s25fl_check_write_progress());
	SS_s25fl_write_enable();
	SS_s25fl_select();
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX);
	HAL_SPI_Transmit_DMA(&hspi2, &buf, 1);
	buf = (adress>>24) & 0xff;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX);
	HAL_SPI_Transmit_DMA(&hspi2, &buf, 1);
	buf = (adress>>16) & 0xff;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX);
	HAL_SPI_Transmit_DMA(&hspi2, &buf, 1);
	buf = (adress>>8) & 0xff;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX);
	HAL_SPI_Transmit_DMA(&hspi2, &buf, 1);
	buf = (adress) & 0xff;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX);
	HAL_SPI_Transmit_DMA(&hspi2, &buf, 1);
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_RX);
	HAL_SPI_Receive_DMA(&hspi2, array, length);
	WAIT_FOR_RX_END = 1;
}
void SS_s25fl_tx_clpt_callback(void)
{
	if(WAIT_FOR_TX_END)
	{
		WAIT_FOR_TX_END = 0;
		//SS_s25fl_deselect();
	}
}
void SS_s25fl_rx_clpt_callback(void)
{
	if(WAIT_FOR_RX_END)
	{
		WAIT_FOR_RX_END = 0;
		//SS_s25fl_deselect();
	}
}
uint8_t SS_s25fl_get_rx_end(void)
{
	return 1-WAIT_FOR_RX_END;
}
uint8_t SS_s25fl_get_tx_end(void)
{
	return 1-WAIT_FOR_TX_END;
}
void SS_s25fl_prepare_time_us(uint8_t *data, uint32_t time)
{
	data[1] = (uint8_t)(time>>24);
	data[2] = (uint8_t)(time>>16);
	data[3] = (uint8_t)(time>>8);
	data[4] = (uint8_t)(time);
}
void SS_s25fl_prepare_time_ms(uint8_t *data, uint32_t time)
{
	data[1] = (uint8_t)(time>>16);
	data[2] = (uint8_t)(time>>8);
	data[3] = (uint8_t)(time);
}
void SS_s25fl_write_data8(uint8_t* data, enum FLASH_ID id)
{
	if(flash_bufor_inc <= 248)
	{
		flash_bufor_pointer[flash_bufor_inc++] = id;
		flash_bufor_pointer[flash_bufor_inc++] = data[1];
		flash_bufor_pointer[flash_bufor_inc++] = data[2];
		flash_bufor_pointer[flash_bufor_inc++] = data[3];
		flash_bufor_pointer[flash_bufor_inc++] = data[4];
		flash_bufor_pointer[flash_bufor_inc++] = data[5];
		flash_bufor_pointer[flash_bufor_inc++] = data[6];
		flash_bufor_pointer[flash_bufor_inc++] = data[7];
	}
	else
	{
		if(flash_bufor_pointer == flash_bufor1)
		{
			SS_s25fl_write_page(flash_page_inc++, flash_bufor1, 256);
			flash_bufor_pointer = flash_bufor2;

		}
		else
		{
			SS_s25fl_write_page(flash_page_inc++, flash_bufor2, 256);
			flash_bufor_pointer = flash_bufor1;

		}
		flash_bufor_inc = 0;
	}
}
void SS_s25fl_erase_full_chip(void)
{
	uint8_t reg = 0x60;
	while(SS_s25fl_check_write_progress());
	SS_s25fl_write_enable();
	SS_s25fl_select();
	HAL_SPI_Transmit(&hspi2, &reg, 1, 200);
	SS_s25fl_deselect();
	while(SS_s25fl_check_write_progress());
}
void SS_s25fl_read_data_logs_to_uart(uint8_t id)
{
	uint16_t inc = 0;
	uint32_t check_page = 16;
	uint8_t check_array[256];
	uint8_t length;
	do
		{
			SS_s25fl_read_page(check_page, (uint8_t*)check_array, 256);
			do
			{
				if (check_array[inc] == id)
				{

					length = sprintf(tx_char, "%d %d\r\n", (uint32_t)(check_array[inc+1] << 16) + (uint32_t)(check_array[inc+2] << 8) +
							(uint32_t)(check_array[inc+3]), (uint32_t)(check_array[inc+4]) + (uint32_t)(check_array[inc+5] << 16) +
							(uint32_t)(check_array[inc+6] << 8) + (uint32_t)(check_array[inc+7]));
					HAL_UART_Transmit(&huart2, tx_char, length, 500);
					inc += 8;
				}
				else
					inc += 8;
			}while(inc < 256);
			inc = 0;
			check_page++;
		}while((check_array[0] != 0xff) && (check_array[7] != 0xff) && (check_array[8] != 0xff) && (check_array[63] != 0xff));
	HAL_UART_Transmit(&huart2, "\r\nfinito\r\n", 12, 500);

}
void SS_s25fl_read_data_adc_to_uart(uint8_t id)
{
	uint16_t inc = 0;
	uint32_t check_page = 16;
	uint8_t check_array[256];
	uint8_t length;
	do
		{
			SS_s25fl_read_page(check_page, (uint8_t*)check_array, 256);
			do
			{
				if (check_array[inc] == id)
				{

					length = sprintf(tx_char, "%d %d\r\n", (uint32_t)(check_array[inc+1] << 24) + (uint32_t)(check_array[inc+2] << 16) +
							(uint32_t)(check_array[inc+3] << 8) + (uint32_t)(check_array[inc+4]), (uint32_t)(check_array[inc+5] << 16) +
							(uint32_t)(check_array[inc+6] << 8) + (uint32_t)(check_array[inc+7]));
					HAL_UART_Transmit(&huart2, tx_char, length, 500);
					inc += 8;
				}
				else
					inc += 8;
			}while(inc < 256);
			inc = 0;
			check_page++;
		}while((check_array[6] != 0xff) && (check_array[201] != 0xff) && (check_array[8] != 0xff) && (check_array[141] != 0xff));
	HAL_UART_Transmit(&huart2, "\r\nfinito\r\n", 12, 500);

}
