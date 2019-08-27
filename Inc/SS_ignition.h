/*
 * SS_ignition.h
 *
 *  Created on: 05.05.2018
 *      Author: Bartuœ
 */

#ifndef SS_IGNITION_H_
#define SS_IGNITION_H_

static uint8_t ign_status,arm_status;

extern uint8_t IGNITION_DELAY_CLOSE;
extern uint16_t IGNITION_DELAY;
extern uint8_t START_IGNITION;
extern uint8_t START_IGNITER_DELAY;
extern uint16_t IGNITER_DELAY;
extern uint16_t  IGNITER_DELAY_CLOSE;
extern uint8_t TEST_STASZEK_SENSEI_MES;
uint8_t SS_borys_get_ign_test_pointer(void);
uint8_t SS_borys_get_arm_test_pointer(void);
void SS_igniter_on(void);
void SS_igniter_off(void);

#endif /* SS_IGNITION_H_ */
