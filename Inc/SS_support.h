/*
 * SS_support.h
 *
 *  Created on: 06.04.2018
 *      Author: Bartuœ
 */

#ifndef SS_SUPPORT_H_
#define SS_SUPPORT_H_


#include "stm32f4xx_hal.h"
#include "tim.h"
#include "SS_it.h"
#include "adc.h"

#define rocket_mass

uint8_t sequence_ID;


enum sequence_ID{
	SEQUENCE_1 = 0x8C,
	SEQUENCE_2 = 0x8D,
	SEQUENCE_3 = 0x8E,
	SEQUENCE_4 = 0x8F,
	SEQUENCE_5 = 0x90,
	SEQUENCE_6 = 0x91,
	SEQUENCE_7 = 0x92,
	SEQUENCE_8 = 0x93,
	SEQUENCE_9 = 0x94,
	SEQUENCE_10,
	SEQUENCE_11
};


void SS_battery_compute_voltage(void);
uint16_t SS_battery_get_voltage(void);




#endif /* SS_SUPPORT_H_ */
