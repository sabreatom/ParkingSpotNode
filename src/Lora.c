/*
 * Lora.c
 *
 *  Created on: 2017. gada 28. nov.
 *      Author: Aleksandrs
 */

#include "Lora.h"
#include "RN2483_driver.h"

//----------------------------------------
//LoRa message related values:
//----------------------------------------

#define LORA_NODE_ID_MSB		0 //TODO: needs to be unique for every device and obtained from UUID chip
#define LORA_NODE_ID_LSB		1
#define LORA_MSG_VER			1
#define LORA_PARKING_MSG_ID		1 //message ID, every type of message has to have its own ID

#define LORA_PARKING_MSG_SIZE	19

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
	uint8_t msg[19] = {0};

	//construct parking message:
	msg[0] = LORA_PARKING_MSG_SIZE; //total message size
	msg[1] = LORA_MSG_VER; //message version
	msg[2] = LORA_NODE_ID_MSB; //node ID msb
	msg[3] = LORA_NODE_ID_LSB; //node ID lsb
	msg[4] = LORA_PARKING_MSG_ID;
	msg[5] = 0; //parking data format version and flags
	msg[6] =  (data.mag_x_val >> 24) & 0xFF; //x value
	msg[7] =  (data.mag_x_val >> 16) & 0xFF;
	msg[8] =  (data.mag_x_val >> 8) & 0xFF;
	msg[9] =  data.mag_x_val & 0xFF;
	msg[10] =  (data.mag_y_val >> 24) & 0xFF; //y value
	msg[11] =  (data.mag_y_val >> 16) & 0xFF;
	msg[12] =  (data.mag_y_val >> 8) & 0xFF;
	msg[13] =  data.mag_y_val & 0xFF;
	msg[14] =  (data.mag_z_val >> 24) & 0xFF; //z value
	msg[15] =  (data.mag_z_val >> 16) & 0xFF;
	msg[16] =  (data.mag_z_val >> 8) & 0xFF;
	msg[17] =  data.mag_z_val & 0xFF;
	msg[18] = data.parked;

	RN2483_send_data(msg, LORA_PARKING_MSG_SIZE);
}

//----------------------------------------
