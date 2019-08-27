/*
 * SS_ignition.c
 *
 *  Created on: 05.05.2018
 *      Author: Bartuœ
 */

#include "stm32f4xx_hal.h"

static uint8_t ign_status = 0, arm_status = 0;


uint16_t IGNITION_DELAY = 5000;
uint8_t START_IGNITION = 0;
uint8_t TEST_STASZEK_SENSEI_MES = 0;
uint8_t START_IGNITER_DELAY = 0;
uint16_t IGNITER_DELAY = 10000;
uint16_t  IGNITER_DELAY_CLOSE = 300;

uint8_t SS_borys_get_ign_test_pointer(void)
{
	ign_status = 1 - HAL_GPIO_ReadPin(IGN_TEST_GPIO_Port, IGN_TEST_Pin);
	return ign_status;
}
uint8_t SS_borys_get_arm_test_pointer(void)
{
	arm_status = 1 - HAL_GPIO_ReadPin(KEY_TEST_GPIO_Port, KEY_TEST_Pin);
	return arm_status;
}

void SS_igniter_on(void)
{
	HAL_GPIO_WritePin(IGNITION_GPIO_Port,IGNITION_Pin,SET);
}
void SS_igniter_off(void)
{
	HAL_GPIO_WritePin(IGNITION_GPIO_Port,IGNITION_Pin,RESET);
}
