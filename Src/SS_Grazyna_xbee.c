/*
l * SS_Grazyna_xbee.c

 *
 *  Created on: 10.02.2018
 *      Author: Tomasz
 */

/* to make it work:
 * call function SS_grazyna_check_timout every 1ms
 * call function SS_grazyna_handle_rx_half_cplt in usart half cplt callback
 * call function SS_grazyna_handle_rx_cplt in usart cplt callback
 * proper defines in SS_Grazyna.h header */

#include "SS_Grazyna_xbee.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "crc.h"
#include "gpio.h"
#include "SS_support.h"
#include "SS_can.h"
#include "SS_servos.h"
#include "SS_ignition.h"
#include "SS_Borys_adc.h"
#include "SS_S25FL.h"
#include "SS_MS5607.h"


// 0x05 0x06 0x00 0x00 0x00 0x00 0x5a 0x0d 0x70 0x47 zapytanie o napi�cie
uint8_t NACK_MESSAGE[10] = {0xD5,0x01,0x00,0x00,0x00,0x00,0xC7,0xF0,0x00,0x30};
enum GRAZYNA_HEADER_TYPE grazyna_header_type;
union FRAME rx_frame;
uint8_t rcv_buffer[10], tx_buffer[10];
static uint8_t rcv_length = 10, header_ID, header_type;
uint8_t RCV_VALID_FLAG;
uint32_t crc_comp, crc_rcv;
uint8_t buf;
enum CAN_HEADER_TYPE CAN;
uint16_t VALID_CAPTURE = 0;
void SS_grazyna_init(void)
{
	HAL_UART_Receive_IT(&XBEE_UART, rx_frame.data_u8.data, 10);
}
void SS_grazyna_handle_rx_cplt(void)		//called in interrupt, keep as short as possible
{
	if(rcv_length == 1)						//if last CRC was wrong, look for valid ID
	{
		if(rx_frame.frame.type == 0x05 || rx_frame.frame.type == 0x15 || rx_frame.frame.type == 0xD5)		//valid ID found
		{
			HAL_UART_Receive_IT(&XBEE_UART, &rx_frame.data_u8.data[1], 9);
			rcv_length = 10;
		}
		else
			HAL_UART_Receive_IT(&XBEE_UART, rx_frame.data_u8.data, 1);		//no valid ID, look further
	}
	else
	{
		crc_rcv = rx_frame.frame.crc;
		rx_frame.frame.crc = 0x00000000;
		crc_comp = HAL_CRC_Calculate(&hcrc, rx_frame.data_u32_u16.data_u32, 2);
#ifdef SKIP_CRC
		header_type = rx_frame.frame.type;
		header_ID = rx_frame.frame.id;

		HAL_UART_Receive_IT(&XBEE_UART, rx_frame.data_u8.data, 10);

		RCV_VALID_FLAG = 1;

#else
		if(crc_rcv == crc_comp)
		{
			header_type = rx_frame.frame.type;
			header_ID = rx_frame.frame.id;
			HAL_UART_Receive_IT(&XBEE_UART, rx_frame.data_u8.data, 10);
			RCV_VALID_FLAG = 1;
			VALID_CAPTURE = 1;
		}
		else
		{
			HAL_UART_Receive_IT(&XBEE_UART, rx_frame.data_u8.data, 1);
			HAL_UART_Transmit(&XBEE_UART, (uint8_t*)&crc_comp, 4,200);

			//HAL_GPIO_TogglePin(LED8_GPIO_Port, LED8_Pin);
			SS_grazyna_send_nack();
			rcv_length = 1;
		}
#endif
	}
}

void SS_grazyna_handle_received(void)
{

		if(header_type == GRAZYNA_SET)
		{
			switch (header_ID)
			{
				/*case GRAZYNA_BORYS_OX_SERVO_POS:
					SS_grazyna_frame_send(0xD5, GRAZYNA_BORYS_OX_SERVO_POS, 2);
					break;*/
				case GRAZYNA_BORYS_OX_SERVO_CLOSED_POS:
					SS_servo_oxidizer_set_closed_position(rx_frame.frame.data);
					SS_grazyna_frame_send(0xD5, GRAZYNA_BORYS_OX_SERVO_CLOSED_POS, 2);
					break;
				case GRAZYNA_BORYS_OX_SERVO_OPENED_POS:
					SS_servo_oxidizer_set_opened_position(rx_frame.frame.data);
					SS_grazyna_frame_send(0xD5, GRAZYNA_BORYS_OX_SERVO_OPENED_POS, 2);
					break;
				case GRAZYNA_BORYS_VENTING_SERVO_CLOSED_POS:
					SS_servo_venting_set_closed_position(rx_frame.frame.data);
					SS_grazyna_frame_send(0xD5, GRAZYNA_BORYS_VENTING_SERVO_CLOSED_POS, 2);
					break;
				case GRAZYNA_BORYS_VENTING_SERVO_OPENED_POS:
					SS_servo_venting_set_opened_position(rx_frame.frame.data);
					SS_grazyna_frame_send(0xD5,GRAZYNA_BORYS_VENTING_SERVO_OPENED_POS, 2);
					break;
				case GRAZYNA_BORYS_SET_ALTITUDE:
					SET_ALTITUDE = 1;
					SS_set_close_altitude(&ms5607,rx_frame.frame.data);
					close_servo_altitude = SS_get_close_altitude(&ms5607);
					SS_grazyna_frame_send(0xD5, GRAZYNA_BORYS_SET_ALTITUDE, 2);
					break;
				case GRAZYNA_BORYS_OPEN_OX_SERVO:
					SS_servo_oxidizer_open();
					SS_grazyna_frame_send(0xD5, GRAZYNA_BORYS_OPEN_OX_SERVO, 2);
					TEST_OPEN_SERVO  = 1;
					SERVO_OPENED  = 1;
					break;
				case GRAZYNA_BORYS_CLOSE_OX_SERVO:
					SS_servo_oxidizer_close();
					SS_grazyna_frame_send(0xD5, GRAZYNA_BORYS_CLOSE_OX_SERVO, 2);
					TEST_OPEN_SERVO = 0;
					break;
				case GRAZYNA_BORYS_OPEN_VENTING_SERVO:
					SS_servo_venting_open();
					SS_grazyna_frame_send(0xD5, GRAZYNA_BORYS_OPEN_VENTING_SERVO, 2);
					break;
				case GRAZYNA_BORYS_CLOSE_VENTING_SERVO:
					SS_servo_venting_close();
					SS_grazyna_frame_send(0xD5, GRAZYNA_BORYS_CLOSE_VENTING_SERVO, 2);
					break;
				case GRAZYNA_IGN_SEQ:
					HAL_TIM_Base_Start_IT(&htim6);
					if(SS_borys_get_arm_test_pointer() && SS_borys_get_ign_test_pointer())
					{
						START_IGNITER_DELAY = 1;
					}

					break;
				case GRAZYNA_BORYS_OX_VENT:
					SS_servo_venting_open();
					SS_grazyna_frame_send(0xD5, GRAZYNA_BORYS_OX_VENT, 2);
					break;
				case GRAZYNA_BORYS_CZAPLA_ON:
					SS_can_tx_no_data(CAN_CZAPLA_ON);
					SS_grazyna_frame_send(0xD5,GRAZYNA_BORYS_CZAPLA_ON, 2);
					break;
				case GRAZYNA_BORYS_CZAPLA_OFF:
					SS_can_tx_no_data(CAN_CZAPLA_OFF);
					SS_grazyna_frame_send(0xD5,GRAZYNA_BORYS_CZAPLA_OFF, 2);
					break;
				case GRAZYNA_BORYS_STASZEK_ON:
					SS_can_tx_no_data(CAN_STASZEK_ON);
					break;
				case GRAZYNA_BORYS_STASZEK_OFF:
					SS_can_tx_no_data(CAN_STASZEK_OFF);
					break;
				case GRAZYNA_LAUNCH_SEQUENCE: /// Zapytac sue cy dac lepie do maina
					HAL_TIM_Base_Start_IT(&htim6);
					TEST_STASZEK_SENSEI_MES = 1;
					HAL_GPIO_WritePin(LED5_GPIO_Port,LED5_Pin,SET);
					if(START_FLIGHT)
					{
						HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,SET);

					}
					break;
				case GRAZYNA_ABORT:
					HAL_GPIO_WritePin(IGNITION_GPIO_Port, IGNITION_Pin, RESET);//sprawdzic
					SS_servo_oxidizer_close();
					SS_grazyna_frame_send(0xD5, GRAZYNA_ABORT, 2);
					SERVO_OPENED = 0;
					TEST_OPEN_SERVO = 0;
					break;
				case GRAZYNA_IGN:
					START_IGNITION = 1;
					break;
				case GRAZYNA_BORYS_START_MEAS:
					HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,SET);
					WRITE_FLASH = 1;
					tick_start = HAL_GetTick();
					break;
				case GRAZYNA_BORYS__LAUNCH_WRITE_ADC_FLASH:
					HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,SET);
					WRITE_FLASH = 1;
					tick_start = HAL_GetTick();
					SS_grazyna_frame_send(0xD5,GRAZYNA_BORYS__LAUNCH_WRITE_ADC_FLASH, 2);
					break;
				case GRAZYNA_BORYS__LAUNCH_STOP_WRITING_TO_FLASH:
					HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,RESET);
					HAL_GPIO_WritePin(LED6_GPIO_Port,LED6_Pin,RESET);
					WRITE_FLASH = 0;
					SS_grazyna_frame_send(0xD5,GRAZYNA_BORYS__LAUNCH_STOP_WRITING_TO_FLASH, 2);
					break;
				case GRAZYNA_BORYS_STOP_WRITE_MEAS:
					HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,RESET);
					HAL_GPIO_WritePin(LED6_GPIO_Port,LED6_Pin,RESET);
					WRITE_FLASH = 0;
					break;
				case GRAZYNA_BORYS_COPY_FLASH_TO_PC:
					HAL_GPIO_WritePin(LED6_GPIO_Port,LED6_Pin,SET);
					SS_s25fl_read_data_logs_to_uart(MS56_BARO);
					SS_s25fl_read_data_logs_to_uart(MS56_ALTITUDE);
					SS_s25fl_read_data_logs_to_uart(MS56_TEMP);
					//SS_s25fl_read_data_logs_to_uart(TENSO_THRUST);
					HAL_GPIO_WritePin(LED6_GPIO_Port,LED6_Pin,RESET);
					break;
				case GRAZYNA_BORYS_ERASE_ALL_FLASH_DATA:
					ERASE_FLASH = 1;
					break;
				case GRAZYNA_BORYS_OX_LOAD:
					SS_can_tx_no_data(CAN_ROMEK_FUEL_LOAD);
					break;
				case GRAZYNA_BORYS_OX_UNLOAD:
					SS_can_tx_no_data(CAN_ROMEK_FUEL_UNLOAD);
					break;

				/*case GRAZYNA_BORYS_ERASE_FLASH:
					HAL_GPIO_WritePin(LED6_GPIO_Port,LED6_Pin,SET);
					//ERASE_FLASH = 1;
					break;*/
				default:
					//SS_grazyna_frame_send(header_type, header_ID, 0x00);
					break;
			}
		}
		else if(header_type == GRAZYNA_GET)
		{
			switch (header_ID)
			{
				/*case GRAZYNA_MANIEK_VOLTAGE:
					SS_grazyna_frame_send(0x05, GRAZYNA_MANIEK_VOLTAGE, SS_maniek_adc_get_voltage());
					break;*/
				case GRAZYNA_BORYS_IGN_GET_STATUS:
					SS_borys_adc_compute_tenso();
					SS_grazyna_frame_send(0x05,GRAZYNA_BORYS_IGN_GET_STATUS, SS_borys_get_ign_test_pointer());
					break;
				case GRAZYNA_BORYS_ARM_GET_STATUS:
					SS_grazyna_frame_send(0x05,GRAZYNA_BORYS_ARM_GET_STATUS, SS_borys_get_arm_test_pointer());
					break;
				case GRAZYNA_BORYS_VOLTAGE:
					SS_borys_adc_compute_voltage();
					SS_grazyna_frame_send(0x05, GRAZYNA_BORYS_VOLTAGE, SS_borys_adc_get_voltage());
					break;
				case GRAZYNA_STASZEK_TENSO_PROP_RAW:
					SS_borys_adc_compute_tenso();
					SS_grazyna_frame_send(0x05, GRAZYNA_STASZEK_TENSO_PROP_RAW, SS_borys_adc_get_tenso());
					break;
				case GRAZYNA_BORYS_GET_REFERENCE_PRESSURE:
					SS_grazyna_frame_send(0x05,GRAZYNA_BORYS_GET_REFERENCE_PRESSURE, ref_pres);
					 break;
				case GRAZYNA_BORYS_GET_ALTITUDE:
					SS_grazyna_frame_send(0x05, GRAZYNA_BORYS_GET_ALTITUDE,SS_MS56_get_average_altitude(&ms5607));
					 break;
				case GRAZYNA_BORYS_GET_PRESSURE:
					SS_grazyna_frame_send(0x05, GRAZYNA_BORYS_GET_PRESSURE, SS_MS56_get_average_press(&ms5607));
					 break;

					/*case BORYS_GET_SD_STATUS:

					if(result == FR_OK)
						 buf = 1;
					else buf = 0;
					SS_grazyna_frame_send(0x05,BORYS_GET_SD_STATUS, &buf, 1);
					break;*/
				default:
					//SS_grazyna_frame_send(0x05, header_ID, 0x00);
					break;
			}
		}
	}

void SS_grazyna_send_nack(void)
{
	HAL_UART_Transmit_IT(&XBEE_UART, NACK_MESSAGE, 10);
}
uint8_t SS_grazyna_get_valid_rcv_flag(void)
{
	if(RCV_VALID_FLAG)
	{
		RCV_VALID_FLAG = 0;
		return 1;
	}
	else return 0;
}
void SS_grazyna_frame_send(uint8_t header_type, uint8_t header_ID, uint32_t data)
{
	union FRAME tx_frame;
	tx_frame.frame.type = header_type;
	tx_frame.frame.id = header_ID;
	tx_frame.frame.data = data;
	tx_frame.frame.crc = 0x00;
	tx_frame.frame.crc = HAL_CRC_Calculate(&hcrc, tx_frame.data_u32_u16.data_u32, 2);
	HAL_UART_Transmit(&XBEE_UART, tx_frame.data_u8.data,10,200);
}
void SS_grazyna_handle_can(CAN_RxHeaderTypeDef rx_header, uint8_t *data)
{

}





