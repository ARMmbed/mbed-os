/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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

/** mbed OS HAL API defined us_ticker as an increment ticker
 *  MPS2 platform provided in SSE-200 are decrement tickers
 *  with interrupt fired counter reaches 0.
 *
 *  So 2 Timers are used to construct mbed OS HAL ticker.
 *  
 *  TIMER1 is for counting, and returns inverted binary when read from it
 *  TIMER1 will be kept in free-running mode (default, and not generate interrupts)
 *  
 *  TIMER2 is for generating interrupts
 *  So TIMER2 is set to periodic mode, which start decrement counting form LOADVALUE generates interrupts at 0
 *  and TIMER2 also set into one-shot mode, which counter halts when is reaches 0 
 */
 
static int us_ticker_inited = 0;

void us_ticker_init(void)
{
    if (us_ticker_inited) {
        us_ticker_disable_interrupt();
        return;
    }

    US_TICKER_TIMER1->TimerControl  = 0x0ul; // disable TIMER1 and reset all control
    US_TICKER_TIMER2->TimerControl  = 0x0ul; // disable TIMER2 and reset all control

    US_TICKER_TIMER1->TimerLoad     = 0xFFFFFFFFul;
    US_TICKER_TIMER2->TimerLoad     = 0xFFFFFFFFul;

    US_TICKER_TIMER1->TimerControl |= CMSDK_DUALTIMER1_CTRL_SIZE_Msk; // set TIMER1 to 32 bit counter
    US_TICKER_TIMER2->TimerControl |= CMSDK_DUALTIMER2_CTRL_SIZE_Msk; // set TIMER2 to 32 bit counter

    US_TICKER_TIMER1->TimerControl |= 0x1 << CMSDK_DUALTIMER1_CTRL_PRESCALE_Pos; // set TIMER1 with 4 stages prescale
    US_TICKER_TIMER2->TimerControl |= 0x1 << CMSDK_DUALTIMER2_CTRL_PRESCALE_Pos; // set TIMER2 with 4 stages prescale

    US_TICKER_TIMER2->TimerControl |= CMSDK_DUALTIMER2_CTRL_MODE_Msk;     // set TIMER2 periodic mode
    US_TICKER_TIMER2->TimerControl |= CMSDK_DUALTIMER2_CTRL_ONESHOOT_Msk; // set TIMER2 one-shot mode

    US_TICKER_TIMER1->TimerControl |= CMSDK_DUALTIMER1_CTRL_EN_Msk; // enable TIMER1 counter

    NVIC_SetVector(US_TICKER_TIMER_IRQn, (uint32_t)us_ticker_irq_handler);
    us_ticker_inited = 1;
}

void us_ticker_free(void)
{
    US_TICKER_TIMER1->TimerControl  &= ~CMSDK_DUALTIMER1_CTRL_EN_Msk; // disable TIMER1
    US_TICKER_TIMER2->TimerControl  &= ~CMSDK_DUALTIMER2_CTRL_EN_Msk; // disable TIMER2
    us_ticker_disable_interrupt();
    us_ticker_inited = 0;
}

uint32_t us_ticker_read()
{
    return ~US_TICKER_TIMER1->TimerValue;
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t delta =  timestamp - us_ticker_read();
    US_TICKER_TIMER2->TimerControl &= ~CMSDK_DUALTIMER2_CTRL_EN_Msk;   // disable TIMER2
    US_TICKER_TIMER2->TimerLoad = delta;                               // Set TIMER2 load value
    US_TICKER_TIMER2->TimerControl |= CMSDK_DUALTIMER2_CTRL_INTEN_Msk; // enable TIMER2 interrupt
    US_TICKER_TIMER2->TimerControl |= CMSDK_DUALTIMER2_CTRL_EN_Msk;    // enable TIMER2 counter
    NVIC_EnableIRQ(US_TICKER_TIMER_IRQn);
}

void us_ticker_fire_interrupt(void)
{
    NVIC_EnableIRQ(US_TICKER_TIMER_IRQn);
    NVIC_SetPendingIRQ(US_TICKER_TIMER_IRQn);
}


void us_ticker_disable_interrupt(void)
{
    US_TICKER_TIMER2->TimerControl &= ~CMSDK_DUALTIMER2_CTRL_INTEN_Msk;
    US_TICKER_TIMER2->TimerControl &= ~CMSDK_DUALTIMER2_CTRL_EN_Msk;  // disable TIMER2
    NVIC_DisableIRQ(US_TICKER_TIMER_IRQn);
}

void us_ticker_clear_interrupt(void)
{
    US_TICKER_TIMER2->TimerIntClr = CMSDK_DUALTIMER2_INTCLR_Msk;
}

const ticker_info_t *us_ticker_get_info(void)
{
    static const ticker_info_t info = {
        1562500,    // 4 stages prescaled from 25MHz (dived by 16)
        32     // 32 bit counter
    };
    return &info;
}
