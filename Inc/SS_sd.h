/*
 * SS_sd.h
 *
 *  Created on: 08.04.2018
 *      Author: Bartuœ
 */

#ifndef SS_SD_H_
#define SS_SD_H_

#include "stm32f4xx_hal.h"
#include "SS_support.h"
//#include "fatfs.h"
extern uint32_t wbytes;
//extern 	FATFS fatfs;
//extern	FIL measurement_file;
//extern 	FRESULT result;
extern uint32_t sd_stack[100];
void SS_sd_put_data(uint32_t data);
void SS_sd_init(void);
void SS_sd_bin_write(uint32_t *sd_stack, uint16_t data_counter);

#endif /* SS_SD_ 	H_ */
