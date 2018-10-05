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

#define US_TICKER_TIMER1      CMSDK_DUALTIMER1
#define US_TICKER_TIMER2      CMSDK_DUALTIMER2
#define US_TICKER_TIMER_IRQn  DUALTIMER_IRQn

int us_ticker_inited = 0;

void us_ticker_init(void) {
    if (us_ticker_inited) return;
    us_ticker_inited = 1;
    
    US_TICKER_TIMER1->TimerControl = 0x0; // disable timer
    US_TICKER_TIMER2->TimerControl = 0x00; // disable timer
    US_TICKER_TIMER1->TimerLoad = 0xFFFFFFFF;
    US_TICKER_TIMER2->TimerLoad = 0xFFFFFFFF;
    
    US_TICKER_TIMER1->TimerControl = 0x62; // enable interrupt and set to 32 bit counter and set to periodic mode
    US_TICKER_TIMER2->TimerControl = 0x42; // enable interrupt and set to 32 bit counter
    
    US_TICKER_TIMER1->TimerControl |= 0x80; // enable counter
    US_TICKER_TIMER2->TimerControl |= 0x80; // enable counter
    
    NVIC_SetVector(US_TICKER_TIMER_IRQn, (uint32_t)us_ticker_irq_handler);
    NVIC_EnableIRQ(US_TICKER_TIMER_IRQn);
}

uint32_t us_ticker_read() {
    uint32_t return_value = 0;

    if (!us_ticker_inited)
        us_ticker_init();
    return_value = ((~US_TICKER_TIMER2->TimerValue)/24);
    return return_value;
}

void us_ticker_set_interrupt(timestamp_t timestamp) {
    int delta = 0;

    if (!us_ticker_inited)
        us_ticker_init();
    delta = (int)(timestamp - us_ticker_read());
    if (delta <= 0) {
        // This event was in the past:
        us_ticker_irq_handler();
        return;
    }
    // enable interrupt
    US_TICKER_TIMER1->TimerControl = 0x0; // disable timer
    US_TICKER_TIMER1->TimerControl = 0x62; // enable interrupt and set to 32 bit counter and set to periodic mode
    US_TICKER_TIMER1->TimerLoad = (delta)*24; //initialise the timer value
    US_TICKER_TIMER1->TimerControl |= 0x80; //enable timer
}

void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(US_TICKER_TIMER_IRQn);
}

void us_ticker_disable_interrupt(void) {
    
    US_TICKER_TIMER1->TimerControl &= 0xDF;
    US_TICKER_TIMER2->TimerControl &= 0xDF;

}

void us_ticker_clear_interrupt(void) {

    US_TICKER_TIMER1->TimerIntClr = 0x1;
    US_TICKER_TIMER2->TimerIntClr = 0x1;

}

void us_ticker_free(void)
{

}
