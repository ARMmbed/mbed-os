/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include "PeripheralNames.h"
#include "clk_freqs.h"

static int us_ticker_inited = 0;

void us_ticker_init(void) {
    if (us_ticker_inited)
        return;
    us_ticker_inited = 1;
    
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;   // Clock PIT
    PIT->MCR = 0;                       // Enable PIT
    
    //Timer on PIT0+1, ticker on PIT 2+3
    //Init timer
    PIT->CHANNEL[1].LDVAL = 0xFFFFFFFF;
    PIT->CHANNEL[1].TCTRL = PIT_TCTRL_CHN_MASK | PIT_TCTRL_TEN_MASK;   // Start timer 1, chained to timer 0
    
    // Use channel 0 as a prescaler for channel 1
    uint32_t ldval = (bus_frequency() + 500000) / 1000000 - 1;
    PIT->CHANNEL[0].LDVAL = ldval;
    PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TEN_MASK;    // Start timer
    
    //Init ticker
    PIT->CHANNEL[2].LDVAL = ldval;
    PIT->CHANNEL[2].TCTRL = PIT_TCTRL_TEN_MASK;    // Start timer 2 as prescaler
    
    NVIC_SetVector(PIT3_IRQn, (uint32_t)us_ticker_irq_handler);
    NVIC_EnableIRQ(PIT3_IRQn);
}

/******************************************************************************
 * Timer for us timing.
 ******************************************************************************/

uint32_t us_ticker_read() {
    if (!us_ticker_inited)
        us_ticker_init();
   
   return ~(PIT->CHANNEL[1].CVAL);
}

/******************************************************************************
 * Ticker Event
 ******************************************************************************/

void us_ticker_disable_interrupt(void) {
    PIT->CHANNEL[3].TCTRL &= ~PIT_TCTRL_TIE_MASK;
}

void us_ticker_clear_interrupt(void) {
    PIT->CHANNEL[3].TFLG = 1;
}

void us_ticker_set_interrupt(timestamp_t timestamp) {
    uint32_t delta = timestamp - us_ticker_read();
    PIT->CHANNEL[3].TCTRL = 0;
    PIT->CHANNEL[3].LDVAL = delta;
    PIT->CHANNEL[3].TCTRL = PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK | PIT_TCTRL_CHN_MASK;
    
}

void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(PIT3_IRQn);
}

void us_ticker_free(void)
{

}
