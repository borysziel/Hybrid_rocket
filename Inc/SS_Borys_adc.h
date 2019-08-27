/*
 * SS_Borys_adc.h
 *
 *  Created on: 23.05.2018
 *      Author: Bartuœ
 */

#ifndef SS_BORYS_ADC_H_
#define SS_BORYS_ADC_H_

#include "stm32f4xx_hal.h"
#include "adc.h"
#include "SS_S25FL.h"
#include "SS_Borys_adc.h"
void SS_borys_adc_init(void);
void SS_borys_adc_compute_tenso(void);
uint16_t *SS_borys_adc_get_data_pointer(void);
void SS_borys_adc_compute_voltage(void);
uint16_t SS_borys_adc_get_voltage(void);
uint16_t SS_borys_adc_get_tenso(void);
void SS_MS56_calculate_tenso_flash(void);
#endif /* SS_BORYS_ADC_H_ */
