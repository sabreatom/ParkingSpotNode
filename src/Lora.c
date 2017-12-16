/*
 * Lora.c
 *
 *  Created on: 2017. gada 28. nov.
 *      Author: Aleksandrs
 */

#include "Lora.h"
#include "RN2483_driver.h"

//----------------------------------------
//Init Lora communication:
//----------------------------------------

void Lora_init()
{
	RN2483_Init();

	//configure module:
	//RN2483_send_cmd("sys factoryRESET");
	//RN2483_send_cmd("sys reset");
	RN2483_send_cmd("radio set mod lora");
	RN2483_send_cmd("radio set freq 868100000");
	RN2483_send_cmd("radio set pwr 14");
	RN2483_send_cmd("radio set sf sf7");
	RN2483_send_cmd("radio set crc on");
	RN2483_send_cmd("radio set cr 4/5");
	RN2483_send_cmd("radio set wdt 0");
	RN2483_send_cmd("radio set sync 12");
	RN2483_send_cmd("radio set bw 500");
	RN2483_send_cmd("mac pause");
}

//----------------------------------------
//Send parking data:
//----------------------------------------

void Lora_send_parking_data(parking_data_struct data)
{
	RN2483_send_data((uint8_t*)&data, sizeof(parking_data_struct));
}

//----------------------------------------
