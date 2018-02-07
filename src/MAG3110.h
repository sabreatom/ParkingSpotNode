/*
 * MAG3110.h
 *
 *  Created on: Jan 15, 2018
 *      Author: aleksandrs
 */

#ifndef SRC_MAG3110_H_
#define SRC_MAG3110_H_

#include <stdbool.h>

typedef struct {
	uint32_t mag_x_val;
	uint32_t mag_y_val;
	uint32_t mag_z_val;
} MAG3110_mag_value_t;

void MAG3110_Init();
MAG3110_mag_value_t MAG3110_read();
bool MAG3110_checkNewMeasurement();
bool MAG3110_isCarPresent(MAG3110_mag_value_t value);

#endif /* SRC_MAG3110_H_ */
