/*
¸	 * SS_sd.c
 *
 *  Created on: 08.04.2018
 *      Author: Bartuœ
 */

#include "SS_sd.h"
//#include "rtc.h"
/*
	FATFS fatfs;
	FIL measurement_file;
	FRESULT result = FR_DISK_ERR;
	uint32_t wbytes;
	uint32_t sd_stack[100];
void SS_sd_init(void)
{
	char file_name[21], dir[9];
	//sprintf(dir, "/%02d-%02d-%02d",  rtc_date.Date, rtc_date.Month, rtc_date.Year);
	//sprintf(file_name, "%02d-%02d-%02d.txt\x03", rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds);

	result = f_mount(&fatfs, "", 0);
	result = f_mkdir(&dir[1]);
	result = f_chdir(dir);
	result = f_open(&measurement_file, file_name, FA_CREATE_ALWAYS | FA_WRITE);
	result = f_write(&measurement_file, "test\r\n", 6, (void *)&wbytes);
	result = f_sync(&measurement_file);
}
void SS_sd_put_data(uint32_t data)
{	uint16_t data_counter;
	sd_stack[data_counter] = data;
	data_counter++;
	if(data_counter > 99)
	{
		SS_staszek_sd_bin_write(sd_stack, data_counter);
		data_counter = 0;
	}
}
void SS_sd_bin_write(uint32_t *sd_stack, uint16_t data_counter)
{
	result = f_write(&measurement_file, (uint8_t*)sd_stack, data_counter*4, (void *)&wbytes);
	result = f_sync(&measurement_file);
}
*/
