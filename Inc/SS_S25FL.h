/*
 * SS_S25FL.h
 *
 *  Created on: 27.05.2018
 *      Author: Bartuœ
 */
#include "stm32f4xx_hal.h"
#include "spi.h"
#include "usart.h"

#ifndef SS_S25FL_H_
#define SS_S25FL_H_


uint8_t WRITE_FLASH;
uint8_t STOP_WRITE_FLASH;
uint8_t ERASE_FLASH;
enum FLASH_ID{
	MS56_BARO,
	MS56_ALTITUDE,
	MS56_TEMP,
	TENSO_THRUST,
};

void SS_s25fl_reset(void);
void SS_s25fl_deselect(void);
void SS_s25fl_select(void);
uint8_t SS_s25fl_read_status(void);
uint8_t SS_s25fl_get_id(void);
uint8_t SS_s25fl_check_write_progress(void);
void SS_s25fl_erase_4k(uint32_t adress);
void SS_s25fl_read_page(uint32_t page, uint8_t *array, uint16_t length);
void SS_s25fl_write_page(uint32_t page, uint8_t *array, uint16_t length);
void SS_s25fl_erase_4k(uint32_t adress);
void SS_s25fl_erase_full_chip(void);
void SS_s25fl_read_page_dma(uint32_t page, uint8_t *array, uint16_t length);
void SS_s25fl_write_page_dma(uint32_t page, uint8_t *array, uint16_t length);
uint8_t SS_s25fl_get_tx_end(void);
uint8_t SS_s25fl_get_rx_end(void);
void SS_s25fl_rx_clpt_callback(void);
void SS_s25fl_tx_clpt_callback(void);
void SS_s25fl_erase_full_chip(void);
void SS_s25fl_read_data_adc_to_uart(uint8_t id);
void SS_s25fl_read_data_logs_to_uart(uint8_t id);
void SS_s25fl_prepare_time_us(uint8_t *data, uint32_t time);
void SS_s25fl_write_data8(uint8_t* data, enum FLASH_ID id);
void SS_s25fl_prepare_time_ms(uint8_t *data, uint32_t time);


#endif /* SS_S25FL_H_ */
