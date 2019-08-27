#include "SS_it.h"
#include "SS_can.h"
uint8_t zmienna = 0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &XBEE_UART)		//XBEE
	{
		SS_grazyna_handle_rx_cplt();
		//HAL_GPIO_TogglePin(LED5_GPIO_Port, LED5_Pin);
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

}

// na testy dlugosc pracy silnika
void HAL_SYSTICK_Callback(void)
{
	static uint16_t grazyna_cnt = 0;
		if(VALID_CAPTURE)
		{
			VALID_CAPTURE = 0;
			grazyna_cnt = 0;
		}
		else
		{
			if(grazyna_cnt < 1000)
				grazyna_cnt++;
			else
				SS_grazyna_init();
		}
	zmienna = 1;
	SS_MS56_decrement_wait_ready();
	//SS_MS56_calculate_tenso_flash();
	if(START_IGNITION)
	{
		if(IGNITION_DELAY <= 0)
		{
			SS_igniter_on();

			if(IGNITER_DELAY_CLOSE <= 0)
			{
				SS_igniter_off();
				START_IGNITION = 0;
			}
			else
				IGNITER_DELAY_CLOSE--;
		}
	}
	if(START_IGNITER_DELAY)
	{

		if(IGNITER_DELAY <= 0)
		{
			SS_igniter_on();

			if(IGNITER_DELAY_CLOSE <= 0)
				{
				SS_igniter_off();
				START_IGNITER_DELAY = 0;
				START_SERVO_DELAY = 1;
				}

			else
				IGNITER_DELAY_CLOSE--;


		}
		else
			IGNITER_DELAY--;
	}

	if(START_SERVO_DELAY)
	{

		if(SERVO_DELAY <= 0)
		{
			SS_servo_oxidizer_open();
			SERVO_OPENED = 1;
			TEST_OPEN_SERVO = 1;
			START_SERVO_DELAY = 0;
			START_SERVO_DELAY_CLOSE = 1;
			TEST_FLIGHT_SERVO_CLOSE = 1;
			START_FLIGHT = 0;
		}
		else
			SERVO_DELAY--;
	}
	////TESTY SILNIKA

	/*if(START_SERVO_DELAY_CLOSE)
	{

		if(SERVO_DELAY_CLOSE <= 0)
		{
			SS_servo_oxidizer_close();
			START_SERVO_DELAY_CLOSE = 0;
		}
		else
			SERVO_DELAY_CLOSE--;
	}*/

	//////////PIERWSZY LOT
	/*if(TEST_FLIGHT_SERVO_CLOSE)
	{
		if(TEST_FLIGHT_DELAY_SERVO_CLOSE <=0)
		{
			SS_servo_oxidizer_close();
			TEST_FLIGHT_SERVO_CLOSE = 0;

		}
		else
			TEST_FLIGHT_DELAY_SERVO_CLOSE--;
	}*/

}

/*


void HAL_SYSTICK_Callback(void)
{
	static uint16_t comm_led_reset = 0, can_check = 0, hz_10 = 0;
	can_check++;
	SS_led_systick_callback();
	if(1000 <= can_check)
	{
		can_check = 0;
		SS_can_tx_no_data(CAN_CHECK_WHO_IS_ONLINE);
	}
	hz_10++;
	if(hz_10 >= 100)
	{
		hz_10 = 0;
		//SS_staszek_check_ign_status();
	}
}
*/

