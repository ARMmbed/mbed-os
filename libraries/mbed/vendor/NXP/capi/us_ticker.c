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

#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368) || defined (TARGET_LPC1788)
#define US_TICKER_TIMER      ((LPC_TIM_TypeDef *)LPC_TIM3_BASE)
#define US_TICKER_TIMER_IRQn TIMER3_IRQn

#elif defined(TARGET_LPC11U24)
#define US_TICKER_TIMER          ((LPC_CTxxBx_Type *)LPC_CT32B1_BASE)
#define US_TICKER_TIMER_IRQn     TIMER_32_1_IRQn

#elif defined(TARGET_LPC812)
#define US_TICKER_TIMER_IRQn     SCT_IRQn

#endif

int us_ticker_inited = 0;

void us_ticker_init(void) {
    if (us_ticker_inited) return;
    us_ticker_inited = 1;
    
#ifdef TARGET_LPC812
    // Enable the SCT clock
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 8);
    
    // Clear peripheral reset the SCT:
    LPC_SYSCON->PRESETCTRL |= (1 << 8);
    
    // Unified counter (32 bits)
    LPC_SCT->CONFIG |= 1;
    
    // halt and clear the counter
    LPC_SCT->CTRL_L |= (1 << 2) | (1 << 3);
    
    // System Clock (12)MHz -> us_ticker (1)MHz
    LPC_SCT->CTRL_L |= ((SystemCoreClock/1000000 - 1) << 5);
    
    // unhalt the counter:
    //    - clearing bit 2 of the CTRL register
    LPC_SCT->CTRL_L &= ~(1 << 2);
    
#else
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368) || defined (TARGET_LPC1788)
    LPC_SC->PCONP |= 1 << 23; // Clock TIMER_3
    
    US_TICKER_TIMER->CTCR = 0x0; // timer mode
    uint32_t PCLK = SystemCoreClock / 4;
    
#elif defined(TARGET_LPC11U24)
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<10); // Clock TIMER_1
    uint32_t PCLK = SystemCoreClock;

#endif
    US_TICKER_TIMER->TCR = 0x2;  // reset
    
    uint32_t prescale = PCLK / 1000000; // default to 1MHz (1 us ticks)
    US_TICKER_TIMER->PR = prescale - 1;
    US_TICKER_TIMER->TCR = 1; // enable = 1, reset = 0
#endif

    NVIC_SetVector(US_TICKER_TIMER_IRQn, (uint32_t)us_ticker_irq_handler);
    NVIC_EnableIRQ(US_TICKER_TIMER_IRQn);
}

uint32_t us_ticker_read() {
    if (!us_ticker_inited)
        us_ticker_init();
    
#ifdef TARGET_LPC812
    return LPC_SCT->COUNT_U;
#else
    return US_TICKER_TIMER->TC;
#endif
}

void us_ticker_set_interrupt(unsigned int timestamp) {
#ifdef TARGET_LPC812
    // halt the counter: 
    //    - setting bit 2 of the CTRL register
    LPC_SCT->CTRL_L |= (1 << 2);
    
    // set timestamp in compare register
    LPC_SCT->MATCH[0].U = timestamp;
    
    // unhalt the counter:
    //    - clearing bit 2 of the CTRL register
    LPC_SCT->CTRL_L &= ~(1 << 2);
    
    // if events are not enabled, enable them
    if (!(LPC_SCT->EVEN & 0x01)) {
        
        // comb mode = match only
        LPC_SCT->EVENT[0].CTRL = (1 << 12);
        
        // ref manual:
        //   In simple applications that do not 
        //   use states, write 0x01 to this 
        //   register to enable an event
        LPC_SCT->EVENT[0].STATE |= 0x1;
        
        // enable events
        LPC_SCT->EVEN |= 0x1;
    }
    
#else
    // set match value
    US_TICKER_TIMER->MR0 = timestamp;
    // enable match interrupt
    US_TICKER_TIMER->MCR |= 1;
#endif
}

void us_ticker_disable_interrupt(void) {
#ifdef TARGET_LPC812
    LPC_SCT->EVEN &= ~1;
#else
    US_TICKER_TIMER->MCR &= ~1;
#endif
}

void us_ticker_clear_interrupt(void) {
#ifdef TARGET_LPC812
    LPC_SCT->EVFLAG = 1;
#else
    US_TICKER_TIMER->IR = 1;
#endif
}
