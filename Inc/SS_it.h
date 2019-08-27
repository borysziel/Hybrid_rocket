/*
 * SS_it.h
 *
 *  Created on: 05.04.2018
 *      Author: Bartuœ
 */

#ifndef SS_IT_H_
#define SS_IT_H_



#include "stm32f4xx_hal.h"
#include "tim.h"
#include "SS_servos.h"
#include "SS_ignition.h"
#include "usart.h"
#include "SS_Grazyna_xbee.h"
#include "SS_support.h"
#include "SS_Borys_adc.h"

extern uint8_t zmienna;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_SYSTICK_Callback(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_SYSTICK_Callback(void);



#endif /* SS_IT_H_ */
