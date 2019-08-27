/*
 * SS_MS5607.h
 *
 *  Created on: 22.12.2017
 *      Author: Tomasz
 */

#ifndef SS_MS5607_H_
#define SS_MS5607_H_

#include "stm32f4xx_hal.h"
#include "SS_S25FL.h"

#define MS56_PRESS_256 0x40
#define MS56_PRESS_512 0x42
#define MS56_PRESS_1024 0x44
#define MS56_PRESS_2048 0x46
#define MS56_PRESS_4096 0x48

#define MS56_TEMP_256 0x50
#define MS56_TEMP_512 0x52
#define MS56_TEMP_1024 0x54
#define MS56_TEMP_2048 0x56
#define MS56_TEMP_4096 0x58

#define MS56_RESET 0x1E
#define MS56_ADC_READ 0x00
#define MS56_PROM_READ_BASE 0xA0

//   USER DEFINES   //
#define HSPI_MS56 hspi1
extern uint8_t ms56_stage;
extern uint8_t tick_start;
extern int32_t average_altitude;
enum RESULT
{
	MS56_OK = 0,
	MS56_NO_COMMUNICATION = 1
};
struct MS5607
{
	uint16_t PROM[8];
	uint8_t tempOSR;
	uint8_t pressOSR;
	uint8_t tempSens;
	uint8_t pressSens;
	int32_t press;
	int32_t temp;
	uint32_t uncomp_press;
	int32_t uncomp_temp;
	int32_t refPress;
	int32_t altitude;
	int32_t average_press;
	int32_t average_altitude;
	int32_t average_temp;
	int16_t set_altitude;
    enum RESULT result;


};

void SS_MS56_CS_DISABLE(void);
void SS_MS56_CS_ENABLE(void);
enum RESULT SS_MS56_reset(void);
enum RESULT SS_MS56_prom_read(struct MS5607 *ms5607);
enum RESULT SS_MS56_adc_read(uint32_t *data);
enum RESULT SS_MS56_convertion_press_start(struct MS5607 *ms5607);
enum RESULT SS_MS56_convertion_temp_start(struct MS5607 *ms5607);
void SS_MS56_wait(uint8_t type);
void SS_MS56_read_convert(struct MS5607 *ms5607);
void SS_MS56_init(struct MS5607 *ms5607);
int32_t SS_MS56_set_altitude(struct MS5607 *ms5607);
void SS_MS56_decrement_wait_ready(void);
void SS_MS56_read_convert_non_polling(struct MS5607 *ms5607);
void SS_MS56_calculate_average_press(struct MS5607 * ms5607, uint8_t cnt);
void SS_MS56_calculate_average_press_flash(struct MS5607 *ms5607, uint8_t average_cnt);
void SS_MS56_calculate_average_temp(struct MS5607 * ms5607, uint8_t average_cnt);
void SS_MS56_calculate_average_temp_flash(struct MS5607 * ms5607, uint8_t average_cnt);
void SS_MS56_calculate_altitude_flash(struct MS5607 * ms5607);
void SS_MS56_calculate_average_altitiude(struct MS5607 * ms5607, uint8_t average_cnt);
void SS_MS56_calculate_average_altitiude2(struct MS5607 * ms5607, uint8_t average_cnt);
int32_t SS_MS56_get_altitude(struct MS5607 *ms5607);
uint32_t SS_MS56_get_average_altitude(struct MS5607 * ms5607);
void SS_set_close_altitude(struct MS5607 *ms5607,uint16_t value);
int32_t SS_get_close_altitude(struct MS5607 *ms5607);
#endif /* SS_MS5607_H_ */
