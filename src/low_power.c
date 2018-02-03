/*
 * low_power.c
 *
 *  Created on: Feb 4, 2018
 *      Author: aleksandrs
 */

#include <stdint.h>
#include <stdbool.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_emu.h"
#include "em_cmu.h"
#include "em_rtc.h"

#include "low_power.h"

#define RTC_FREQ    32768

//----------------------------------------------------------
//RTC compare counter interrupt handler:
//----------------------------------------------------------

void RTC_IRQHandler(void)
{
  /* Clear interrupt source */
  RTC_IntClear(RTC_IFC_COMP0);
}

//----------------------------------------------------------
//Initialize RTC for entering low power mode and waking-up:
//----------------------------------------------------------

void LP_init()
{
	RTC_Init_TypeDef rtcInit = RTC_INIT_DEFAULT;

	/* Enable LE domain registers */
	//CMU_ClockEnable(cmuClock_CORELE, true);

	/* Enable LFXO as LFACLK in CMU. This will also start LFXO */
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

	/* Set a clock divisor of 32 to reduce power consumption. */
	CMU_ClockDivSet(cmuClock_RTC, cmuClkDiv_32);

	/* Enable RTC clock */
	CMU_ClockEnable(cmuClock_RTC, true);

	/* Initialize RTC */
	rtcInit.enable   = false;  /* Do not start RTC after initialization is complete. */
	rtcInit.debugRun = false;  /* Halt RTC when debugging. */
	rtcInit.comp0Top = true;   /* Wrap around on COMP0 match. */
	RTC_Init(&rtcInit);

	/* Interrupt every minute */
	RTC_CompareSet(0, ((RTC_FREQ / 32) * 60 ) - 1 );

	/* Enable interrupt */
	NVIC_EnableIRQ(RTC_IRQn);
	RTC_IntEnable(RTC_IEN_COMP0);

	/* Start Counter */
	RTC_Enable(true);
}

//----------------------------------------------------------
//Enter sleep mode:
//----------------------------------------------------------

void LP_sleep()
{
	EMU_EnterEM2(true);
}

//----------------------------------------------------------
