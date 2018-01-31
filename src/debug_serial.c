/*
 * debug_serial.c
 *
 *  Created on: 2017. gada 25. nov.
 *      Author: Aleksandrs
 */


#include "stdint.h"
#include "hal-config.h"
#include "debug_serial.h"
#include "em_cmu.h"
#include "xprintf.h"
#include "fifo.h"
#include "em_leuart.h"
#include "em_gpio.h"

//RX FIFO:
#define DEBUG_RX_FIFO_SIZE		16
static FIFO_t rx_fifo;
static unsigned int rx_buf[DEBUG_RX_FIFO_SIZE];

//TX FIFO:
#define DEBUG_TX_FIFO_SIZE		200
static FIFO_t tx_fifo;
static unsigned int tx_buf[DEBUG_TX_FIFO_SIZE];

//LEUART initialization configuration:
const LEUART_Init_TypeDef LEUART_init_config =
{
  .enable   = leuartEnable,       /* Activate data reception on LEUn_TX pin. */
  .refFreq  = 0,              /* Inherit the clock frequency from the LEUART clock source */
  .baudrate = 9600,                 /* Baudrate = 9600 bps */
  .databits = leuartDatabits8,      /* Each LEUART frame containes 8 databits */
  .parity   = leuartNoParity,       /* No parity bits in use */
  .stopbits = leuartStopbits1,      /* Setting the number of stop bits in a frame to 2 bitperiods */
};

//----------------------------------------------------------------
//Output function:
//----------------------------------------------------------------

static void Debug_chr_out(unsigned char data)
{
	FIFO_wr_single_element(&tx_fifo, data);

	LEUART_IntEnable(LEUART0, LEUART_IF_TXBL);
}

//----------------------------------------------------------------
//Input function:
//----------------------------------------------------------------

static unsigned char Debug_chr_int()
{
	unsigned int tmp = 0;

	FIFO_rd_single_element(&rx_fifo, &tmp);

	return 0xFF & tmp;
}

//----------------------------------------------------------------
//Initialize debug console:
//----------------------------------------------------------------

void Debug_Init()
{
	CMU_OscillatorEnable(cmuOsc_LFXO, true, true);

	CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);

	CMU_ClockEnable(cmuClock_LEUART0, true);          // enable LEUART peripheral clock
	CMU_ClockEnable(cmuClock_CORELE, true);           // enable the Low Energy Peripheral Interface clock
	CMU_ClockEnable(cmuClock_GPIO, true);             // enable GPIO peripheral clock

	//Initialize GPIO:
	GPIO_PinModeSet(PORTIO_LEUART0_RX_PORT, PORTIO_LEUART0_RX_PIN, gpioModeInputPull, 0);
	GPIO_PinModeSet(PORTIO_LEUART0_TX_PORT, PORTIO_LEUART0_TX_PIN, gpioModePushPull, 1);

	LEUART0->ROUTE = LEUART_ROUTE_LOCATION_LOC0 | LEUART_ROUTE_RXPEN | LEUART_ROUTE_TXPEN;

	//Initialize FIFOs:
	FIFO_init(&rx_fifo, rx_buf, DEBUG_RX_FIFO_SIZE);
	FIFO_init(&tx_fifo, tx_buf, DEBUG_TX_FIFO_SIZE);

	//LEUART_Enable(LEUART0, leuartEnable);

	//xprintf initialization:
	xfunc_out = Debug_chr_out;
	xfunc_in = Debug_chr_int;

	LEUART_IntEnable(LEUART0, LEUART_IF_RXDATAV);
	NVIC_ClearPendingIRQ(LEUART0_IRQn);
	NVIC_EnableIRQ(LEUART0_IRQn);

	//Initializing:
	LEUART_Init(LEUART0, &LEUART_init_config);
}

//----------------------------------------------------------------
//LEUART interrupt handler:
//----------------------------------------------------------------

void LEUART0_IRQHandler()
{
	uint32_t leuartif = LEUART_IntGet(LEUART0);
	unsigned int tmp = 0;

	//data received:
	if (leuartif & LEUART_IF_RXDATAV) {
		FIFO_wr_single_element(&rx_fifo, LEUART_Rx(LEUART0));

		LEUART_IntClear(LEUART0, LEUART_IF_RXDATAV);
	}

	//TX buffer free to send data:
	if (LEUART0->STATUS & LEUART_IF_TXBL) {
		if (!FIFO_isEmpty(&tx_fifo)){
			FIFO_rd_single_element(&tx_fifo, &tmp);
			LEUART_Tx(LEUART0, tmp);
		}

		if (FIFO_isEmpty(&tx_fifo)) { //not bytes to send, so disable transmit interrupt
			LEUART_IntDisable(LEUART0, LEUART_IF_TXBL);
			LEUART_IntClear(LEUART0, LEUART_IF_TXBL);
		}
	}
}

