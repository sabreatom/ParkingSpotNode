/*
 * RN2483_driver.c
 *
 *  Created on: 2017. gada 18. nov.
 *      Author: Aleksandrs
 */

#include "stdint.h"
#include "stdio.h"
#include "hal-config.h"
#include "RN2483_driver.h"
#include "em_cmu.h"
#include "fifo.h"
#include "em_usart.h"
#include "em_gpio.h"
#include "uptime.h"

//RX FIFO:
#define RN2483_RX_FIFO_SIZE		100
static FIFO_t rx_fifo;
static unsigned int rx_buf[RN2483_RX_FIFO_SIZE];

//TX FIFO:
#define RN2483_TX_FIFO_SIZE		100
static FIFO_t tx_fifo;
static unsigned int tx_buf[RN2483_TX_FIFO_SIZE];

//LEUART initialization configuration:
USART_InitAsync_TypeDef uartInit =
  {
    .enable = usartDisable,     // wait to enable transmitter and receiver
    .refFreq = 0,               // setting refFreq to 0 will invoke the CMU_ClockFreqGet() function and measure the HFPER clock
    .baudrate = 57600,         // desired baud rate
    .oversampling = usartOVS16, // set oversampling to x16
    .databits = usartDatabits8, // 8 data bits
    .parity = usartNoParity,    // no parity bits
    .stopbits = usartStopbits1, // 1 stop bit
    .mvdis = false,             // use majority voting
    .prsRxEnable = false,       // not using PRS input
    .prsRxCh = usartPrsRxCh0,   // doesn't matter what channel we select
  };

//----------------------------------------------------------------
//Output function:
//----------------------------------------------------------------

static void RN2483_chr_out(unsigned char data)
{
	FIFO_wr_single_element(&tx_fifo, data);

	USART_IntEnable(USART1, USART_IF_TXBL);
}

//----------------------------------------------------------------
//Input function:
//----------------------------------------------------------------

unsigned char RN2483_chr_in()
{
	unsigned int tmp = 0;

	FIFO_rd_single_element(&rx_fifo, &tmp);

	return 0xFF & tmp;
}

//----------------------------------------------------------------
//Initialize communication with RN2483:
//----------------------------------------------------------------

void RN2483_Init()
{
	CMU_OscillatorEnable(cmuOsc_HFXO, true, true);          // enable HF XTAL osc and wait for it to stabilize
	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);        // select HF XTAL osc as system clock source (24MHz)

	CMU_ClockEnable(cmuClock_GPIO, true);             // enable GPIO peripheral clock
	CMU_ClockEnable(cmuClock_USART1, true);                 // enable USART1 peripheral clock

	//Initialize GPIO:
	GPIO_PinModeSet(PORTIO_USART1_RX_PORT, PORTIO_USART1_RX_PIN, gpioModeInputPull, 0);
	GPIO_PinModeSet(PORTIO_USART1_TX_PORT, PORTIO_USART1_TX_PIN, gpioModePushPull, 1);

	//Initialize UART:
	USART_InitAsync(USART1, &uartInit);      // apply configuration to USART1
	USART1->ROUTE = USART_ROUTE_LOCATION_LOC3 | USART_ROUTE_RXPEN | USART_ROUTE_TXPEN;

	//Initialize FIFOs:
	FIFO_init(&rx_fifo, rx_buf, RN2483_RX_FIFO_SIZE);
	FIFO_init(&tx_fifo, tx_buf, RN2483_TX_FIFO_SIZE);

	USART_IntEnable(USART1, USART_IF_RXDATAV);

	USART_IntClear(USART1, _USART_IF_MASK);  // clear all USART interrupt flags

	//Clear interrupts:
	NVIC_ClearPendingIRQ(USART1_RX_IRQn);    // clear pending RX interrupt flag in NVIC
	NVIC_ClearPendingIRQ(USART1_TX_IRQn);    // clear pending TX interrupt flag in NVIC

	NVIC_EnableIRQ(USART1_RX_IRQn);
	NVIC_EnableIRQ(USART1_TX_IRQn);

	//Enable UART:
	USART_Enable(USART1, usartEnable);       // enable transmitter and receiver
}

//----------------------------------------------------------------
//Send command to RN2483:
//----------------------------------------------------------------

RN2483_cmd_response_t RN2483_send_cmd(const char* cmd)
{
	RN2483_flush_rx_fifo();

	while (*cmd != '\0') {
		RN2483_chr_out(*cmd);
		cmd++;
	}

	RN2483_chr_out('\r');
	RN2483_chr_out('\n');

	uint64_t cur_uptime =  Uptime_getValue();

	while ((Uptime_getValue() - cur_uptime) < 800) ; //delay

	return RN2483_CMD_OK;
}

//-----------------------------------------------------------------
//Send data:
//----------------------------------------

void RN2483_send_data(uint8_t* data, uint8_t num_bytes)
{
	const char tx_cmd[] = "radio tx ";
	char tmp_str[10] = {0};
	uint8_t i = 0;

	RN2483_flush_rx_fifo();

	//send command:
	while (tx_cmd[i] != '\0'){
		RN2483_chr_out(tx_cmd[i]);
		i++;
	}

	//send data:
	for (uint8_t j = 0; j < num_bytes; j++){
		//RN2483_chr_out(*(data + j));

		sprintf(tmp_str, "%02x", *(data + j));
		RN2483_chr_out(tmp_str[0]);
		RN2483_chr_out(tmp_str[1]);
	}

	//terminate:
	RN2483_chr_out('\r');
	RN2483_chr_out('\n');
}

//----------------------------------------------------------------
//Flush data from RX FIFO buffer:
//----------------------------------------------------------------

void RN2483_flush_rx_fifo()
{
	FIFO_flush(&rx_fifo);
}

//---------------------------------------------------------------
//Check if data received:
//---------------------------------------------------------------

uint8_t RN2483_check_rx_fifo()
{
	if (FIFO_isEmpty(&rx_fifo)) {
		return 0;
	}
	else {
		return 1;
	}
}

//----------------------------------------------------------------
//LEUART interrupt handlers:
//----------------------------------------------------------------

void USART1_RX_IRQHandler(void)
{
	uint32_t uartif = USART_IntGet(USART1);

	//data received:
	if (uartif & USART_IF_RXDATAV) {
		FIFO_wr_single_element(&rx_fifo, USART_Rx(USART1));

		USART_IntClear(USART1, USART_IF_RXDATAV);
	}
}

void USART1_TX_IRQHandler(void)
{
	unsigned int tmp = 'j';

	//TX buffer free to send data:
	if (USART1->STATUS & USART_IF_TXBL) {
		if (!FIFO_isEmpty(&tx_fifo)) {
			FIFO_rd_single_element(&tx_fifo, &tmp);
			USART_Tx(USART1, tmp);
		}

		if (FIFO_isEmpty(&tx_fifo)) {
			USART_IntDisable(USART1, USART_IF_TXBL);
			USART_IntClear(USART1, USART_IF_TXBL);
		}
	}
}
