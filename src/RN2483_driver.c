/*
 * RN2483_driver.c
 *
 *  Created on: 2017. gada 18. nov.
 *      Author: Aleksandrs
 */

#include "RN2483_driver.h"
#include "em_cmu.h"

//LEUART initialization config:
static LEUART_Init_TypeDef LEUART_init_config =
{
  .enable   = leuartEnable,       /* Activate data reception on LEUn_TX pin. */
  .refFreq  = 0,                    /* Inherit the clock frequenzy from the LEUART clock source */
  .baudrate = 115200,                 /* Baudrate = 9600 bps */
  .databits = leuartDatabits8,      /* Each LEUART frame containes 8 databits */
  .parity   = leuartNoParity,       /* No parity bits in use */
  .stopbits = leuartStopbits1,      /* Setting the number of stop bits in a frame to 2 bitperiods */
};

//----------------------------------------------------------------
//Initialize communication with RN2483:
//----------------------------------------------------------------

void RN2483_Init()
{
	//Enable clock sources:
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_LEUART0, true);

	//Reseting and initializing:
	LEUART_Reset(LEUART0);
	LEUART_Init(LEUART0, &LEUART_init_config);

	//Initialize GPIO:
	GPIO_PinModeSet(PORTIO_LEUART0_RX_PORT, PORTIO_LEUART0_RX_PIN, gpioModeInputPull, 1);
	GPIO_PinModeSet(PORTIO_LEUART0_TX_PORT, PORTIO_LEUART0_TX_PIN, gpioModePushPull, 1);

}

//----------------------------------------------------------------
