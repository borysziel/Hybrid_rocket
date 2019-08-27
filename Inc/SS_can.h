/*
 * SS_can.h
 *
 *  Created on: 07.05.2018
 *      Author: Bartuœ
 */

#ifndef SS_CAN_H_
#define SS_CAN_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_can.h"
#include "can.h"
#include "SS_Grazyna_xbee.h"
#include "SS_servos.h"
#include "SS_support.h"
#include "SS_ignition.h"

CAN_FilterTypeDef filter_config, can_filter;
CAN_RxHeaderTypeDef rx_header;
CAN_TxHeaderTypeDef tx_header;

uint8_t INCLINATION;
uint8_t STASZEK_ON;
uint8_t STASZEK_OFF;
uint8_t STASZEK_ZAMYKAMY;
extern uint8_t START_FLIGHT;
#define CAN_OFFSET 200
extern int32_t can_data;
typedef enum  CAN_HEADER_TYPE{

	CAN_ROMEK_FUEL_LOAD = 235,
	CAN_ROMEK_FUEL_UNLOAD,
	CAN_ROMEK_OX_LOAD,
	CAN_ROMEK_OX_UNLOAD,
	CAN_ROMEK_OX_VENT,
	CAN_ROMEK_OX_FEED,

	CAN_SENSEI_LECIM = 0x44,
	CAN_SENSEI_INCLINATION = 0x60,
	CAN_SENSEI_ENGINE_OFF = 0x22,
	CAN_SENSEI_INCLINATION2 = 309,
	CAN_ROMEK_TEST = 287,
	CAN_ROMEK_TEST_CAN,
	CAN_STASZEK_LECIM = 400,
	CAN_STASZEK_ZAMYKAMY,
	CAN_CZAPLA_ON = 331,
	CAN_CZAPLA_OFF,
	CAN_STASZEK_ON,
	CAN_STASZEK_OFF,
	CAN_STASZEK_SENSEI_MES = 208,
	TEST_STASZEK_SENSEI_MES_OK,

};

uint8_t RX_FIFO0_READY;
uint8_t rx_data0[8], rx_data1[8];
uint8_t romek_rx_data0[8], romek_rx_data1[8];

typedef __attribute__((packed)) union CAN_RX_DATA{
	uint8_t data_u8[4];
	uint32_t data_u32;
};
union CAN_RX_DATA rx_data_u;

void SS_can_interrupts_enable(void);
void SS_can_start(void);
void SS_can_filters_init(void);
uint32_t SS_can_tx_no_data(enum CAN_HEADER_TYPE can_header_type);
uint32_t SS_can_tx_data(enum CAN_HEADER_TYPE can_header_type, uint8_t data[8], uint8_t data_length);
void SS_can_handle_received_fifo0(void);
void SS_can_set_rx_fifo0_data_ready(void);
void SS_can_clear_rx_fifo0_data_ready(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

#endif /* SS_CAN_H_ */
