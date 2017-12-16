/*
 * uptime.c
 *
 *  Created on: 2017. gada 26. nov.
 *      Author: Aleksandrs
 */

#include "uptime.h"
#include "hal-config.h"
#include "em_cmu.h"

//Uptime counter:
static volatile uint64_t uptime_count;

//-------------------------------------------
//Initialize system timer for uptime counting:
//-------------------------------------------

void Uptime_init()
{
	uptime_count = 0;
	SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000);
}

//-------------------------------------------
//Get uptime value:
//-------------------------------------------

uint64_t Uptime_getValue()
{
	return uptime_count;
}

//-------------------------------------------
//Interrupt handler for system timer:
//-------------------------------------------

void SysTick_Handler(void)
{
	uptime_count++;
}

//-------------------------------------------
