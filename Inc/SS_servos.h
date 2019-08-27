	/*
 * SS_servos.h
 *
 *  Created on: 25.02.2018
 *      Author: Tomasz
 */

#ifndef SS_SERVOS_H_
#define SS_SERVOS_H_

#include "stm32f4xx_hal.h"
#include "adc.h"
#include "tim.h"

extern uint16_t SERVO_DELAY;
extern uint8_t START_SERVO_DELAY;
extern uint16_t SERVO_DELAY_CLOSE;
extern uint8_t START_SERVO_DELAY_CLOSE;

extern uint8_t TEST_FLIGHT_SERVO_CLOSE;
extern uint16_t TEST_FLIGHT_DELAY_SERVO_CLOSE;
uint8_t TEST_OPEN_SERVO;
uint8_t SERVO_OPENED;
struct SERVO servo_oxidizer;
struct SERVO servo_venting;
struct SERVO{
	uint16_t freq;			//Hz
	//uint16_t position;
	uint16_t position_percentage;
	uint16_t closed_position;
	uint16_t opened_position;
	//uint16_t current;
	uint32_t *pointer;
	TIM_TypeDef *timer; 	//TIMx
	uint32_t channel;		//1,2,3 or 4
};

struct SERVO *SS_servo_get_oxidizer_pointer(void);
struct SERVO *SS_servo_get_venting_pointer(void);
void SS_servo_init(struct SERVO *servo);
void SS_servo_compute_current(void);
uint16_t SS_servo_get_current(void);
void SS_servo_init_all(void);
void SS_servo_init(struct SERVO *servo);
void SS_servo_set_position(uint16_t value);

void SS_servo_oxidizer_close(void);
void SS_servo_oxidizer_open(void);
void SS_servo_oxidizer_set_closed_position(uint16_t position);
void SS_servo_oxidizer_set_opened_position(uint16_t position);
void SS_servo_venting_set_closed_position(uint16_t position);
void SS_servo_venting_set_opened_position(uint16_t position);
void SS_servo_set_ignition_delay(uint16_t delay);
void SS_servo_venting_close(void);
void SS_servo_venting_open(void);
void SS_servo_venting_set_closed_position(uint16_t position);
void SS_servo_venting_set_opened_position(uint16_t position);

#endif /* SS_SERVOS_H_ */
