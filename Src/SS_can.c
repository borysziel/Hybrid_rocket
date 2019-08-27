/*
 * SS_can.c
 *
 *  Created on: 07.05.2018
 *      Author: Bartuœ
 */

#include "SS_can.h"
#include "can.h"
uint8_t RX_FIFO0_READY = 0;
union CAN_RX_DATA rx_data_u;
//uint8_t INCLINATION = 0;
uint8_t START_FLIGHT = 0;
void SS_can_interrupts_enable(void)
{
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO1_MSG_PENDING | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO0_FULL);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_BUSOFF);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_ERROR);
}
void SS_can_start(void)
{
	SS_can_filters_init();
	HAL_CAN_Start(&hcan1);
	SS_can_interrupts_enable();
	rx_data_u.data_u32 = 0;
}
void SS_can_filters_init(void)
{
//	can_filter.FilterBank = 7;
//	can_filter.FilterMode = CAN_FILTERMODE_IDMASK;
//	can_filter.FilterScale = CAN_FILTERSCALE_32BIT;
//	can_filter.FilterIdHigh = 0x0000;
//	can_filter.FilterIdLow = 0x0000;
//	can_filter.FilterMaskIdHigh = 0xffff;
//	can_filter.FilterMaskIdLow = 0xffff;
//	can_filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
//	can_filter.FilterActivation = ENABLE;
//
//	if(HAL_CAN_ConfigFilter(&hcan1, &can_filter) != HAL_OK)
//	{
//		/* Filter configuration Error */
//		Error_Handler();
//	}
	can_filter.FilterBank = 0;
	can_filter.FilterMode = CAN_FILTERMODE_IDMASK;
	can_filter.FilterScale = CAN_FILTERSCALE_32BIT;
	can_filter.FilterIdHigh = 0x0000;
	can_filter.FilterIdLow = 0x0000;
	can_filter.FilterMaskIdHigh = 0x0000;
	can_filter.FilterMaskIdLow = 0x0000;
	can_filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	can_filter.FilterActivation = ENABLE;

	if(HAL_CAN_ConfigFilter(&hcan1, &can_filter) != HAL_OK)
	{
		/* Filter configuration Error */
		Error_Handler();
	}
}

uint32_t SS_can_tx_no_data(enum CAN_HEADER_TYPE can_header_type)
{
	uint32_t mailbox;
	uint8_t data;
	tx_header.StdId = 0x00;
	tx_header.ExtId = can_header_type;//can_header_type;
	tx_header.RTR = CAN_RTR_DATA;
	tx_header.IDE = CAN_ID_EXT;
	tx_header.DLC = 0;
	tx_header.TransmitGlobalTime = DISABLE;

	if(HAL_CAN_AddTxMessage(&hcan1, &tx_header, &data, &mailbox) != HAL_OK)
	{	//HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,SET);
		mailbox = 0;

	}
	return mailbox;
}

uint32_t SS_can_tx_data(enum CAN_HEADER_TYPE can_header_type, uint8_t data[8], uint8_t data_length)
{
	uint32_t mailbox;
	tx_header.StdId = 0x00;
	tx_header.ExtId = can_header_type;
	tx_header.RTR = CAN_RTR_DATA;
	tx_header.IDE = CAN_ID_EXT;
	tx_header.DLC = data_length;
	tx_header.TransmitGlobalTime = DISABLE;

	if(HAL_CAN_AddTxMessage(&hcan1, &tx_header, data, &mailbox) != HAL_OK)
		mailbox = 0;
	return mailbox;
}

void SS_can_handle_received_fifo0(void)
{
	HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, rx_data0);

}
void SS_can_set_rx_fifo0_data_ready(void)
{
	RX_FIFO0_READY = 1;
}
void SS_can_clear_rx_fifo0_data_ready(void)
{
	if(HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0))
		RX_FIFO0_READY = 1;
	else
		RX_FIFO0_READY = 0;
}
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	HAL_GPIO_TogglePin(LED5_GPIO_Port, LED5_Pin);
	volatile uint32_t error = HAL_CAN_GetError(&hcan1);
}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	//HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin,SET);
	if(hcan == &hcan1)
	{
		//HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin,SET);
		HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, rx_data0);
		/*if(rx_header.ExtId == CAN_SENSEI_INCLINATION2)
		{
		  can_data  = (int32_t)(rx_data0[4]<<24);
		  can_data += (int32_t)(rx_data0[5]<<16);
		  can_data += (int32_t)(rx_data0[6]<<8);
		  can_data += (int32_t)(rx_data0[7]);
		  HAL_GPIO_TogglePin(LED6_GPIO_Port,LED6_Pin);
		}*/

//		if(rx_header0.ExtId == CAN_SENSEI_INCLINATION)
//		{
//			SS_servo_oxidizer_close();
//		}
		/*if(rx_header.ExtId == CAN_SENSEI_INCLINATION)
		{
			HAL_GPIO_TogglePin(LED5_GPIO_Port,LED5_Pin);
			INCLINATION = 1;
		}*/

		if(rx_header.ExtId == CAN_SENSEI_INCLINATION)
		{
			HAL_GPIO_TogglePin(LED4_GPIO_Port,LED4_Pin);
			INCLINATION = 1;
		}
		else if(rx_header.ExtId == CAN_CZAPLA_ON)
		{
			HAL_GPIO_TogglePin(LED5_GPIO_Port,LED5_Pin);
		}
		else if(rx_header.ExtId == CAN_CZAPLA_OFF)
		{
			HAL_GPIO_TogglePin(LED6_GPIO_Port,LED6_Pin);
		}
		else if(rx_header.ExtId == CAN_STASZEK_ON)
		{
			SS_grazyna_frame_send(0xD5,GRAZYNA_BORYS_STASZEK_ON, 2);
		}
		else if(rx_header.ExtId == CAN_STASZEK_OFF)
		{
			SS_grazyna_frame_send(0xD5,GRAZYNA_BORYS_STASZEK_OFF, 2);
		}
		else if(rx_header.ExtId == CAN_STASZEK_ZAMYKAMY)
		{
			STASZEK_ZAMYKAMY = 1;
		}
		else if(rx_header.ExtId == TEST_STASZEK_SENSEI_MES_OK)
		{
			START_FLIGHT = 1;
			if(SS_borys_get_arm_test_pointer() && SS_borys_get_ign_test_pointer())
			{
				START_IGNITER_DELAY = 1;
			}
			TEST_STASZEK_SENSEI_MES = 0;
		}

	}

}
