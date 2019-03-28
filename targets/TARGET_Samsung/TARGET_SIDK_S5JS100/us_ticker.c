/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <stddef.h>
#include "cmsis.h"
#include "us_ticker_api.h"
#include "PeripheralNames.h"

#define TIMER_TICKS_US      (26)

static uint32_t us_ticker_initialized = 0;
static int us_user_interrupt = 0;
static unsigned int us_user_intset;

const ticker_info_t *us_ticker_get_info()
{
	static const ticker_info_t info = {
		26000000,
		32
	};
	return &info;
}

void __us_ticker_irq_handler(void) {
	putreg32(1, S5JS100_TIMER0_BASE + 0x10);
	NVIC_ClearPendingIRQ((IRQn_Type)S5JS100_IRQ_TINT0);

//	if (us_user_interrupt == 1) {
		putreg32(us_user_intset + getreg32(S5JS100_TIMER0_BASE + 0x34), S5JS100_TIMER0_BASE + 0x8), 
		us_ticker_irq_handler();
		us_user_interrupt = 0;
//  	} else if (us_user_interrupt == 2) {
//		us_ticker_irq_handler();
//		us_user_interrupt = 0;
//	}
}

void us_ticker_init(void) {
	if (us_ticker_initialized)
		return;
	us_ticker_initialized = 1;

	putreg32(0, S5JS100_TIMER0_BASE + 0x4);
	//putreg32(0xFFFFFFFF, S5JS100_TIMER0_BASE + 0x0);
	putreg32(1, S5JS100_TIMER0_BASE + 0x14);
	putreg32(3, S5JS100_TIMER0_BASE + 0x4);

	/* Timer 0 get IRQn */
	NVIC_SetVector((IRQn_Type)S5JS100_IRQ_TINT0, (uint32_t)__us_ticker_irq_handler);
	NVIC_EnableIRQ((IRQn_Type)S5JS100_IRQ_TINT0);
}

uint32_t us_ticker_read()
{
	int ret = ~(getreg32(S5JS100_TIMER0_BASE + 0x34));
	return ret;// / TIMER_TICKS_US;
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
	us_user_intset = ~(timestamp);
	putreg32(us_user_intset, S5JS100_TIMER0_BASE + 0x1C);
	us_user_interrupt = 1;
}

void us_ticker_fire_interrupt(void)
{
	us_user_interrupt = 2;
    NVIC_SetPendingIRQ((IRQn_Type)S5JS100_IRQ_TINT0);
}

void us_ticker_disable_interrupt(void)
{
    putreg32(0, S5JS100_TIMER0_BASE + 0x14);
}

void us_ticker_clear_interrupt(void)
{
    putreg32(1, S5JS100_TIMER0_BASE + 0x10);
}
void us_ticker_free(void)
{
	//printf("%s.....\n", __func__);
}
