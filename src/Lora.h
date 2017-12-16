/*
 * Lora.h
 *
 *  Created on: 2017. gada 28. nov.
 *      Author: Aleksandrs
 */

#ifndef SRC_LORA_H_
#define SRC_LORA_H_

#include "stdint.h"
#include "hal-config.h"

//Parking data struct:
typedef struct {
	uint32_t mag_x_val;
	uint32_t mag_y_val;
	uint32_t mag_z_val;
	uint8_t parked;
} __attribute__((packed)) parking_data_struct;

void Lora_init();
void Lora_send_parking_data(parking_data_struct data);

#endif /* SRC_LORA_H_ */
