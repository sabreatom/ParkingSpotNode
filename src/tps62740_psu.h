/*
 * tps62740_psu.h
 *
 *  Created on: 2017. gada 13. dec.
 *      Author: Aleksandrs
 */

#ifndef SRC_TPS62740_PSU_H_
#define SRC_TPS62740_PSU_H_

//Power supply output voltage values:
typedef enum {
	VOUT_1_8 = 0,
	VOUT_1_9,
	VOUT_2_0,
	VOUT_2_1,
	VOUT_2_2,
	VOUT_2_3,
	VOUT_2_4,
	VOUT_2_5,
	VOUT_2_6,
	VOUT_2_7,
	VOUT_2_8,
	VOUT_2_9,
	VOUT_3_0,
	VOUT_3_1,
	VOUT_3_2,
	VOUT_3_3
} TPS62740_voltage_config_t;

//Capacitor charge boost resistor state:
typedef enum {
	BOOST_OFF = 0,
	BOOST_ON
} TPS62740_boost_resistor_state_t;

//Power supply voltage output state:
typedef enum {
	POWER_NOT_GOOD = 0,
	POWER_GOOD
} TPS62740_power_good_state_t;

void TPS62740_init();

void TPS62740_setOutputVoltage(TPS62740_voltage_config_t vout_value);

void TPS62740_setBoostResistorState(TPS62740_boost_resistor_state_t state);
TPS62740_boost_resistor_state_t TPS62740_getBoostResistorState();

TPS62740_power_good_state_t TPS62740_getVoltageOutputState();

void TPS62740_readOutputVoltage();

#endif /* SRC_TPS62740_PSU_H_ */
