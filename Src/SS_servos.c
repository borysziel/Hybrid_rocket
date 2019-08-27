/*
 * SS_servos.c
 *
 *  Created on: 25.02.2018
 *      Author: Tomasz
 */
#include "SS_servos.h"

struct SERVO servo_oxidizer;
struct SERVO servo_venting;

uint16_t SERVO_DELAY = 3000;
uint8_t START_SERVO_DELAY = 0;
uint16_t SERVO_DELAY_CLOSE = 8000;
uint8_t START_SERVO_DELAY_CLOSE = 0;
//uint8_t SERVO_OPENED = 0;
uint8_t TEST_FLIGHT_SERVO_CLOSE = 0;
uint16_t TEST_FLIGHT_DELAY_SERVO_CLOSE = 15000;

struct SERVO *SS_servo_get_oxidizer_pointer(void)
{
	return &servo_oxidizer;
}
struct SERVO *SS_servo_get_venting_pointer(void)
{
	return &servo_venting;
}
/*uint16_t SS_servo_get_current(void)
{
//	servo_oxidizer.current = SS_maniek_adc_compute_servo(3);
//	return servo_oxidizer.current;
}*/
uint16_t SS_servo_get_ox_position(void)
{
	return 0;
}
void SS_servo_init(struct SERVO *servo)
{
	TIM_HandleTypeDef htim;
	RCC_ClkInitTypeDef  *RCC_ClkInitStruct;
	uint32_t *lat;
	uint32_t freq;
	HAL_RCC_GetClockConfig(RCC_ClkInitStruct, lat);
	if(servo->timer == TIM8 || servo->timer == TIM1 || servo->timer == TIM11 || servo->timer == TIM10 || servo->timer == TIM9)
	{
		freq = HAL_RCC_GetPCLK2Freq();
		if(RCC_ClkInitStruct->APB2CLKDivider != 1)
			freq *= 2;
	}
	else
	{
		freq = HAL_RCC_GetPCLK1Freq();
		if(RCC_ClkInitStruct->APB2CLKDivider != 1)
			freq *= 2;
	}

	htim.Instance = servo->timer;
	htim.Init.Prescaler = (freq/1000)/servo->freq;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.Period = 999;
	htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	if(HAL_TIM_Base_GetState(&htim) != HAL_TIM_STATE_READY)
	{
		HAL_TIM_Base_Init(&htim);
		HAL_TIM_Base_Start(&htim);
	}
	switch (servo->channel)
	{
		case 1:
			HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_1);
			servo->pointer = &servo->timer->CCR1;
			break;
		case 2:
			HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_2);
			servo->pointer = &servo->timer->CCR2;
			break;
		case 3:
			HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_3);
			servo->pointer = &servo->timer->CCR3;
			break;
		case 4:
			HAL_TIM_PWM_Start(&htim, TIM_CHANNEL_4);
			servo->pointer = &servo->timer->CCR4;
			break;
		default:
		    _Error_Handler(__FILE__, __LINE__);
			break;
	}
	*servo->pointer = servo->closed_position;
	//servo->pointer = &TIM1->CCR1;
}

void SS_servo_init_all(void)
{
	servo_venting.timer = TIM3;
	servo_venting.channel = 1;
	servo_venting.freq = 333;
	servo_venting.opened_position = 250;
	servo_venting.closed_position = 450;
	SS_servo_init(&servo_venting);

	servo_oxidizer.timer = TIM2;
	servo_oxidizer.channel = 1;
	servo_oxidizer.freq = 330;
	servo_oxidizer.opened_position = 300;
	servo_oxidizer.closed_position = 600;
	SS_servo_init(&servo_oxidizer);

}

void SS_servo_set_position(uint16_t value)
{
	*servo_oxidizer.pointer =  value;
	*servo_venting.pointer =  value;
}
void SS_servo_oxidizer_close(void)
{
	*servo_oxidizer.pointer = servo_oxidizer.closed_position;
}
void SS_servo_oxidizer_open(void)
{
	*servo_oxidizer.pointer = servo_oxidizer.opened_position;
}

void SS_servo_oxidizer_set_closed_position(uint16_t position)
{
	servo_oxidizer.closed_position = position;
}
void SS_servo_oxidizer_set_opened_position(uint16_t position)
{
	servo_oxidizer.opened_position = position;
}

void SS_servo_venting_set_closed_position(uint16_t position)
{
	servo_venting.closed_position = position;
}
void SS_servo_venting_set_opened_position(uint16_t position)
{
	servo_venting.opened_position = position;
}

void SS_servo_set_ignition_delay(uint16_t delay)
{
	SERVO_DELAY = delay;
}

void SS_servo_venting_close(void)
{
	*servo_venting.pointer = servo_venting.closed_position;
}
void SS_servo_venting_open(void)
{
	*servo_venting.pointer = servo_venting.opened_position;
}
