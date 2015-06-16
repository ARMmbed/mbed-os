/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stddef.h>
#include "us_ticker_api.h"
#include "cmsis.h"
#include "mbed_assert.h"
#include "ins_gclk.h"
#include "compiler.h"
#include "tc.h"
#include "tc_interrupt.h"

#define TICKER_COUNTER_uS		TC4
#define TICKER_COUNTER_IRQn		TC4_IRQn
#define TICKER_COUNTER_Handlr	TC4_Handler

static int us_ticker_inited = 0;

struct tc_module us_ticker_module;

void us_ticker_irq_handler_internal(struct tc_module* us_tc_module)
{
	us_ticker_irq_handler();
	
	/* Disable the interrupt */
	us_ticker_disable_interrupt();
}

void us_ticker_init(void)
{
	uint32_t			cycles_per_us;
	uint32_t			prescaler = 0;
	struct tc_config	config_tc;
	enum status_code	ret_status;
	
	if (us_ticker_inited) return;
	us_ticker_inited = 1;


	tc_get_config_defaults(&config_tc);
	
	cycles_per_us = system_gclk_gen_get_hz(config_tc.clock_source) / 1000000;
	MBED_ASSERT(cycles_per_us > 0);
	while((cycles_per_us & 1) == 0 && prescaler <= 10) {
		cycles_per_us = cycles_per_us >> 1;
		prescaler++;
	}
	config_tc.clock_prescaler = TC_CTRLA_PRESCALER(prescaler);
	config_tc.counter_size = TC_COUNTER_SIZE_32BIT;
	config_tc.run_in_standby = true;
	config_tc.counter_32_bit.value = 0;
	config_tc.counter_32_bit.compare_capture_channel[0] = 0xFFFFFFFF;
	
	//config_tc.oneshot = true;

	/* Initialize the timer */
	ret_status = tc_init(&us_ticker_module, TICKER_COUNTER_uS, &config_tc);
	MBED_ASSERT(ret_status == STATUS_OK);
	
	/* Register callback function */
	tc_register_callback(&us_ticker_module, (tc_callback_t)us_ticker_irq_handler_internal, TC_CALLBACK_CC_CHANNEL0);
	
	/* Enable the timer module */
	tc_enable(&us_ticker_module);
}

uint32_t us_ticker_read()
{
	if (!us_ticker_inited)
        us_ticker_init();
	
	return tc_get_count_value(&us_ticker_module);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
	uint32_t cur_time;
	int32_t delta;
	
	cur_time = us_ticker_read();
	delta = (int32_t)((uint32_t)timestamp - cur_time);
	if (delta < 0)
	{
		/* Event already occurred in past */
		us_ticker_irq_handler();
		return;
	}

	tc_set_compare_value(&us_ticker_module, TC_CALLBACK_CC_CHANNEL0, timestamp);
	
	NVIC_SetVector(TICKER_COUNTER_IRQn, (uint32_t)TICKER_COUNTER_Handlr);
	NVIC_EnableIRQ(TICKER_COUNTER_IRQn);
	
	/* Enable the callback */
	tc_enable_callback(&us_ticker_module, TC_CALLBACK_CC_CHANNEL0);
	
	/* Enable the timer module */
	//tc_enable(&us_ticker_module);
}

void us_ticker_disable_interrupt(void) {
	/* Disable the callback */
	tc_disable_callback(&us_ticker_module, TC_CALLBACK_CC_CHANNEL0);
	NVIC_DisableIRQ(TICKER_COUNTER_IRQn);
}

void us_ticker_clear_interrupt(void) {
	/* Disable the interrupt, this is clear the interrupt also */
	us_ticker_disable_interrupt();
	
	NVIC_DisableIRQ(TICKER_COUNTER_IRQn);
	NVIC_SetVector(TICKER_COUNTER_IRQn, (uint32_t)NULL);
}