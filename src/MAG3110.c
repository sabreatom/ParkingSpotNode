/*
 * MAG3110.c
 *
 *  Created on: Jan 15, 2018
 *      Author: aleksandrs
 */

#include "stdint.h"
#include "stdio.h"
#include "hal-config.h"
#include "MAG3110.h"
#include "em_cmu.h"
#include "em_i2c.h"
#include "em_gpio.h"
#include "uptime.h"

//MAG3110 I2C address:
#define MAG3110_I2C_ADDR	0x0E

//I2C TX buffer:
#define TX_BUFFER_SIZE_BYTES	10
#define RX_BUFFER_SIZE_BYTES	10
static uint8_t _tx_buffer[TX_BUFFER_SIZE_BYTES];
static uint8_t _rx_buffer[RX_BUFFER_SIZE_BYTES];

//----------------------------------------------------
//Initialize I2C communication:
//----------------------------------------------------

void MAG3110_Init()
{
	I2C_Init_TypeDef i2c_init = I2C_INIT_DEFAULT;

	//enable I2C clock:
	CMU_ClockEnable(cmuClock_I2C0, true);

	GPIO_PinModeSet(PORTIO_I2C0_SDA_PORT, PORTIO_I2C0_SDA_PIN, gpioModeWiredAnd, 0);      // configure SDA pin as open drain output
	GPIO_PinModeSet(PORTIO_I2C0_SCL_PORT, PORTIO_I2C0_SCL_PIN, gpioModeWiredAnd, 0);      // configure SCL pin as open drain output

	I2C_Init(I2C0, &i2c_init);                     // apply configuration to I2C0
	I2C0->CTRL |= I2C_CTRL_AUTOACK;                        // enable AUTO-ACK feature
	I2C0->ROUTE = I2C_ROUTE_LOCATION_LOC4 | I2C_ROUTE_SDAPEN | I2C_ROUTE_SCLPEN; //set route for pins

	//enable auto reset for MAG3110:
	_tx_buffer[0] = 0x11; //control register 2
	_tx_buffer[1] = 0x80; //enable auto reset

	I2C_TransferReturn_TypeDef ret;
	I2C_TransferSeq_TypeDef sensor_message =
	{
		.addr = (MAG3110_I2C_ADDR << 1),            // set sensor slave address
		.flags = I2C_FLAG_WRITE,                     // indicate basic write
		.buf[0].data = _tx_buffer,                    // point to tx_buffer
		.buf[0].len = 2,                             // specify number of bytes
	};
	ret = I2C_TransferInit(I2C0, &sensor_message); // start I2C write transaction with MAG3110

	while(ret == i2cTransferInProgress) {          // continue until all data has been sent
		ret = I2C_Transfer(I2C0);
	}

	//put MAG3110 in active mode with sample rate 0.63:
	_tx_buffer[0] = 0x10; //control register 1
	_tx_buffer[1] = 0xC9; //set active mode with sample rate 0.63Hz

	sensor_message.addr = (MAG3110_I2C_ADDR << 1);            // set sensor slave address
	sensor_message.flags = I2C_FLAG_WRITE;                     // indicate basic write
	sensor_message.buf[0].data = _tx_buffer;                    // point to tx_buffer
	sensor_message.buf[0].len = 2;                             // specify number of bytes
	ret = I2C_TransferInit(I2C0, &sensor_message); // start I2C write transaction with MAG3110

	while(ret == i2cTransferInProgress) {          // continue until all data has been sent
		ret = I2C_Transfer(I2C0);
	}
}

//----------------------------------------------------
//Read magnetic sensor value:
//----------------------------------------------------

MAG3110_mag_value_t MAG3110_read()
{
	I2C_TransferReturn_TypeDef ret;

	_tx_buffer[0] = 0x01; //x axe measurement MSB

	I2C_TransferSeq_TypeDef sensor_message =
	{
		.addr = (MAG3110_I2C_ADDR << 1),            // set sensor slave address
		.flags = I2C_FLAG_WRITE_READ,                     // indicate basic write
		.buf[0].data = _tx_buffer,                    // point to tx_buffer
		.buf[0].len = 1,                             // specify number of bytes
		.buf[1].len = 6,                 // specify # bytes to be read
		.buf[1].data = _rx_buffer,        // point to rx_buffer
	};
	ret = I2C_TransferInit(I2C0, &sensor_message); // start I2C write/read transaction with sensor

	while(ret == i2cTransferInProgress) {          // continue until all data has been received
		ret = I2C_Transfer(I2C0);
	}

	//return value:
	MAG3110_mag_value_t data = {0};
	data.mag_x_val = (_rx_buffer[0] << 8) + _rx_buffer[1];
	data.mag_y_val = (_rx_buffer[2] << 8) + _rx_buffer[3];
	data.mag_z_val = (_rx_buffer[4] << 8) + _rx_buffer[5];

	return data;
}

//----------------------------------------------------
//Check if new MAG3110 measurement data is ready:
//----------------------------------------------------

bool MAG3110_checkNewMeasurement()
{
	I2C_TransferReturn_TypeDef ret;

	_tx_buffer[0] = 0x00; //x axe measurement MSB

	I2C_TransferSeq_TypeDef sensor_message =
	{
		.addr = (MAG3110_I2C_ADDR << 1),            // set sensor slave address
		.flags = I2C_FLAG_WRITE_READ,                     // indicate basic write
		.buf[0].data = _tx_buffer,                    // point to tx_buffer
		.buf[0].len = 1,                             // specify number of bytes
		.buf[1].len = 1,                 // specify # bytes to be read
		.buf[1].data = _rx_buffer,        // point to rx_buffer
	};
	ret = I2C_TransferInit(I2C0, &sensor_message); // start I2C write/read transaction with sensor

	while(ret == i2cTransferInProgress) {          // continue until all data has been received
		ret = I2C_Transfer(I2C0);
	}

	if ((_rx_buffer[0] & 0x07) == 0x07) {
		return true;
	}
	else {
		return false;
	}
}

//----------------------------------------------------
