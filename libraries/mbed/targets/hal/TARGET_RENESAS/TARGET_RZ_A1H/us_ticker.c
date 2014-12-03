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
#include "PeripheralNames.h"
#include "mtu2_iodefine.h"

#define US_TICKER_TIMER      (OSTM0.OSTMnCMP)
#define US_TICKER_TIMER_IRQn TIMER3_IRQn

int us_ticker_inited = 0;

void us_ticker_interrupt(void) {
    us_ticker_irq_handler();
    GIC_EndInterrupt(TGI2A_IRQn);
}

void us_ticker_init(void) {
    if (us_ticker_inited) return;
    us_ticker_inited = 1;
    
    /* Power Control for Peripherals      */
    CPGSTBCR3 &= ~ 0x8;   // turn on MTU2

    // timer settings
    MTU2.TSYR = 0x6;      // cascading T_1-T_2

    MTU2.TCR_2 = 0x03;    // divider 1/64
    MTU2.TCR_1 = 0x07;    // count-up from T_2 pulse(cascade)

    MTU2.TCNT_1 = 0x00;   // counter value set to 0
    MTU2.TCNT_2 = 0x00;   // 

    MTU2.TSTR |= 0x06;     //
    MTU2.TSR_2 = 0xc0;    // timer start

    // INTC settings
    InterruptHandlerRegister(TGI2A_IRQn, (void (*)(uint32_t))us_ticker_interrupt);
    GIC_SetPriority(TGI2A_IRQn, 5);
    GIC_EnableIRQ(TGI2A_IRQn);
    __enable_irq();
}

//static const float PCLK     =33.33,          // dummy
                   //PRESCALE =64.0;           // dummy
static const float FACTOR_C2U = 1.9201920192019204, //(PRESCALE/PCLK)
                   FACTOR_U2C = 0.52078125;         //(PCLK/PRESCALE)

#define F_CLK2us(val)  ((uint32_t)((val)*FACTOR_C2U))
#define F_us2CLK(val)  ((uint32_t)((val)*FACTOR_U2C))


uint32_t us_ticker_read() {
    static uint32_t max_val = 0x8551eb85; //*F_us2CLK(0xffffffff)+1;
    uint32_t val;
    if (!us_ticker_inited)
        us_ticker_init();
    
    val = MTU2.TCNT_1<<16 | MTU2.TCNT_2;  // concat cascaded Counters
    if (val > max_val) {  // if overflow (in us-timer)
        val -= max_val;   // correct value
        MTU2.TCNT_1 = 0;  // reset counter
        MTU2.TCNT_2 = val;
    }
    val = F_CLK2us(val);
    return val;
}

void us_ticker_set_interrupt(timestamp_t timestamp) {
    // set match value
    timestamp = F_us2CLK(timestamp);
    MTU2.TGRA_2 = timestamp & 0xffff;
    // enable match interrupt
    MTU2.TIER_2 = 0x01;
}

void us_ticker_disable_interrupt(void) {
    MTU2.TIER_2 &= ~(0xc0);
}

void us_ticker_clear_interrupt(void) {
    MTU2.TSR_2 &= 0xc0;
}
