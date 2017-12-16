/*
 * tps62740_psu.c
 *
 *  Created on: 2017. gada 13. dec.
 *      Author: Aleksandrs
 */


#include "tps62740_psu.h"
#include "stdint.h"
#include "hal-config.h"

#include "em_gpio.h"
#include "em_cmu.h"
#include "em_adc.h"

#include "xprintf.h"

//ADC conversation value:
static volatile uint32_t TPS62740_vout_value;

//Initialize default configuration:
void TPS62740_init()
{
	//configure output voltage configuration pins, lowest voltage 1.9V after power up:
	GPIO_PinModeSet(VOUT_CFG_0_PORT, VOUT_CFG_0_PIN, gpioModePushPull, 1);
	GPIO_PinModeSet(VOUT_CFG_1_PORT, VOUT_CFG_1_PIN, gpioModePushPull, 0);
	GPIO_PinModeSet(VOUT_CFG_2_PORT, VOUT_CFG_2_PIN, gpioModePushPull, 0);
	GPIO_PinModeSet(VOUT_CFG_3_PORT, VOUT_CFG_3_PIN, gpioModePushPull, 0);

	//capacitor charge boost configuration, default disabled:
	GPIO_PinModeSet(BOOST_CNTRL_PORT, BOOST_CNTRL_PIN, gpioModePushPull, 0);

	//configure power good input:
	GPIO_PinModeSet(PWR_GOOD_PORT, PWR_GOOD_PIN, gpioModeInputPull, 1);

	//ADC clock enable:
	CMU_ClockEnable(cmuClock_ADC0, true);

	//Initialize output voltage sample ADC:
	ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
	ADC_InitSingle_TypeDef singleInit = ADC_INITSINGLE_DEFAULT;

	/* Initialize for single conversion */
	singleInit.input = adcSingleInpCh4;
	ADC_InitSingle(ADC0, &singleInit);

	/* Enable ADC Interrupt*/
	ADC_IntEnable(ADC0, ADC_IEN_SINGLE);

	//Enable ADC single conversation interrupt:
	NVIC_ClearPendingIRQ(ADC0_IRQn);
	NVIC_EnableIRQ(ADC0_IRQn);
}

//Set output voltage value:
void TPS62740_setOutputVoltage(TPS62740_voltage_config_t vout_value)
{
	switch (vout_value) {
	case VOUT_1_8:
		GPIO_PinOutClear(VOUT_CFG_0_PORT, VOUT_CFG_0_PIN);
		GPIO_PinOutClear(VOUT_CFG_1_PORT, VOUT_CFG_1_PIN);
		GPIO_PinOutClear(VOUT_CFG_2_PORT, VOUT_CFG_2_PIN);
		GPIO_PinOutClear(VOUT_CFG_3_PORT, VOUT_CFG_3_PIN);
		break;
	case VOUT_1_9:
		GPIO_PinOutSet(VOUT_CFG_0_PORT, VOUT_CFG_0_PIN);
		GPIO_PinOutClear(VOUT_CFG_1_PORT, VOUT_CFG_1_PIN);
		GPIO_PinOutClear(VOUT_CFG_2_PORT, VOUT_CFG_2_PIN);
		GPIO_PinOutClear(VOUT_CFG_3_PORT, VOUT_CFG_3_PIN);
		break;
	case VOUT_2_0:
		GPIO_PinOutClear(VOUT_CFG_0_PORT, VOUT_CFG_0_PIN);
		GPIO_PinOutSet(VOUT_CFG_1_PORT, VOUT_CFG_1_PIN);
		GPIO_PinOutClear(VOUT_CFG_2_PORT, VOUT_CFG_2_PIN);
		GPIO_PinOutClear(VOUT_CFG_3_PORT, VOUT_CFG_3_PIN);
		break;
	case VOUT_2_1:
		GPIO_PinOutSet(VOUT_CFG_0_PORT, VOUT_CFG_0_PIN);
		GPIO_PinOutSet(VOUT_CFG_1_PORT, VOUT_CFG_1_PIN);
		GPIO_PinOutClear(VOUT_CFG_2_PORT, VOUT_CFG_2_PIN);
		GPIO_PinOutClear(VOUT_CFG_3_PORT, VOUT_CFG_3_PIN);
		break;
	case VOUT_2_2:
		GPIO_PinOutClear(VOUT_CFG_0_PORT, VOUT_CFG_0_PIN);
		GPIO_PinOutClear(VOUT_CFG_1_PORT, VOUT_CFG_1_PIN);
		GPIO_PinOutSet(VOUT_CFG_2_PORT, VOUT_CFG_2_PIN);
		GPIO_PinOutClear(VOUT_CFG_3_PORT, VOUT_CFG_3_PIN);
		break;
	case VOUT_2_3:
		GPIO_PinOutSet(VOUT_CFG_0_PORT, VOUT_CFG_0_PIN);
		GPIO_PinOutClear(VOUT_CFG_1_PORT, VOUT_CFG_1_PIN);
		GPIO_PinOutSet(VOUT_CFG_2_PORT, VOUT_CFG_2_PIN);
		GPIO_PinOutClear(VOUT_CFG_3_PORT, VOUT_CFG_3_PIN);
		break;
	case VOUT_2_4:
		GPIO_PinOutClear(VOUT_CFG_0_PORT, VOUT_CFG_0_PIN);
		GPIO_PinOutSet(VOUT_CFG_1_PORT, VOUT_CFG_1_PIN);
		GPIO_PinOutSet(VOUT_CFG_2_PORT, VOUT_CFG_2_PIN);
		GPIO_PinOutClear(VOUT_CFG_3_PORT, VOUT_CFG_3_PIN);
		break;
	case VOUT_2_5:
		GPIO_PinOutSet(VOUT_CFG_0_PORT, VOUT_CFG_0_PIN);
		GPIO_PinOutSet(VOUT_CFG_1_PORT, VOUT_CFG_1_PIN);
		GPIO_PinOutSet(VOUT_CFG_2_PORT, VOUT_CFG_2_PIN);
		GPIO_PinOutClear(VOUT_CFG_3_PORT, VOUT_CFG_3_PIN);
		break;
	case VOUT_2_6:
		GPIO_PinOutClear(VOUT_CFG_0_PORT, VOUT_CFG_0_PIN);
		GPIO_PinOutClear(VOUT_CFG_1_PORT, VOUT_CFG_1_PIN);
		GPIO_PinOutClear(VOUT_CFG_2_PORT, VOUT_CFG_2_PIN);
		GPIO_PinOutSet(VOUT_CFG_3_PORT, VOUT_CFG_3_PIN);
		break;
	case VOUT_2_7:
		GPIO_PinOutSet(VOUT_CFG_0_PORT, VOUT_CFG_0_PIN);
		GPIO_PinOutClear(VOUT_CFG_1_PORT, VOUT_CFG_1_PIN);
		GPIO_PinOutClear(VOUT_CFG_2_PORT, VOUT_CFG_2_PIN);
		GPIO_PinOutSet(VOUT_CFG_3_PORT, VOUT_CFG_3_PIN);
		break;
	case VOUT_2_8:
		GPIO_PinOutClear(VOUT_CFG_0_PORT, VOUT_CFG_0_PIN);
		GPIO_PinOutSet(VOUT_CFG_1_PORT, VOUT_CFG_1_PIN);
		GPIO_PinOutClear(VOUT_CFG_2_PORT, VOUT_CFG_2_PIN);
		GPIO_PinOutSet(VOUT_CFG_3_PORT, VOUT_CFG_3_PIN);
		break;
	case VOUT_2_9:
		GPIO_PinOutSet(VOUT_CFG_0_PORT, VOUT_CFG_0_PIN);
		GPIO_PinOutSet(VOUT_CFG_1_PORT, VOUT_CFG_1_PIN);
		GPIO_PinOutClear(VOUT_CFG_2_PORT, VOUT_CFG_2_PIN);
		GPIO_PinOutSet(VOUT_CFG_3_PORT, VOUT_CFG_3_PIN);
		break;
	case VOUT_3_0:
		GPIO_PinOutClear(VOUT_CFG_0_PORT, VOUT_CFG_0_PIN);
		GPIO_PinOutClear(VOUT_CFG_1_PORT, VOUT_CFG_1_PIN);
		GPIO_PinOutSet(VOUT_CFG_2_PORT, VOUT_CFG_2_PIN);
		GPIO_PinOutSet(VOUT_CFG_3_PORT, VOUT_CFG_3_PIN);
		break;
	case VOUT_3_1:
		GPIO_PinOutSet(VOUT_CFG_0_PORT, VOUT_CFG_0_PIN);
		GPIO_PinOutClear(VOUT_CFG_1_PORT, VOUT_CFG_1_PIN);
		GPIO_PinOutSet(VOUT_CFG_2_PORT, VOUT_CFG_2_PIN);
		GPIO_PinOutSet(VOUT_CFG_3_PORT, VOUT_CFG_3_PIN);
		break;
	case VOUT_3_2:
		GPIO_PinOutClear(VOUT_CFG_0_PORT, VOUT_CFG_0_PIN);
		GPIO_PinOutSet(VOUT_CFG_1_PORT, VOUT_CFG_1_PIN);
		GPIO_PinOutSet(VOUT_CFG_2_PORT, VOUT_CFG_2_PIN);
		GPIO_PinOutSet(VOUT_CFG_3_PORT, VOUT_CFG_3_PIN);
		break;
	case VOUT_3_3:
		GPIO_PinOutSet(VOUT_CFG_0_PORT, VOUT_CFG_0_PIN);
		GPIO_PinOutSet(VOUT_CFG_1_PORT, VOUT_CFG_1_PIN);
		GPIO_PinOutSet(VOUT_CFG_2_PORT, VOUT_CFG_2_PIN);
		GPIO_PinOutSet(VOUT_CFG_3_PORT, VOUT_CFG_3_PIN);
		break;
	default:
		xprintf("[ERROR] Unsupported output voltage configuration!");
		break;
	}
}

//Set boost resistor state:
void TPS62740_setBoostResistorState(TPS62740_boost_resistor_state_t state)
{
	if (state == BOOST_ON) {
		GPIO_PinOutSet(BOOST_CNTRL_PORT, BOOST_CNTRL_PIN);
	}
	else {
		GPIO_PinOutClear(BOOST_CNTRL_PORT, BOOST_CNTRL_PIN);
	}
}

//Get boost resistor state:
TPS62740_boost_resistor_state_t TPS62740_getBoostResistorState()
{
	if (GPIO_PinOutGet(BOOST_CNTRL_PORT, BOOST_CNTRL_PIN)) {
		return BOOST_ON;
	}
	else {
		return BOOST_OFF;
	}
}

//Get power good state:
TPS62740_power_good_state_t TPS62740_getVoltageOutputState()
{
	if (GPIO_PinInGet(PWR_GOOD_PORT, PWR_GOOD_PIN)) {
		return POWER_GOOD;
	}
	else {
		return POWER_NOT_GOOD;
	}
}

//ADC start single conversation:
void TPS62740_readOutputVoltage()
{
	ADC_Start(ADC0, adcStartSingle);
}

//ADC interrupt handler:
void ADC0_IRQHandler(void)
{
	if (ADC_IntGet(ADC0) & ADC_IF_SINGLE) {
		TPS62740_vout_value = ADC_DataSingleGet(ADC0);
		//xprintf("[INFO] ADC value: %d", TPS62740_vout_value);

		ADC_IntClear(ADC0, ADC_IF_SINGLE);
	}
}
