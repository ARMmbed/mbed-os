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
#include "fsl_pit_hal.h"
#include "fsl_sim_hal.h"
#include "fsl_clock_manager.h"
#include "fsl_clock_configs.h"

static void pit_init(void);
static void lptmr_init(void);

static int us_ticker_inited = 0;

void us_ticker_init(void) {
    if (us_ticker_inited) {
        return;
    }
    us_ticker_inited = 1;

    pit_init();
    lptmr_init();
}


uint32_t us_ticker_read() {
    if (!us_ticker_inited) {
        us_ticker_init();
    }

    return ~(pit_hal_read_timer_count(1));
}
/******************************************************************************
 * Timer for us timing.
 ******************************************************************************/
static void pit_init(void) {
    uint32_t busClock;

    clock_hal_set_gate(kSimClockModulePIT, 0, true);
    pit_hal_enable();
    clock_manager_get_frequency(kBusClock, &busClock);
    pit_hal_set_timer_period_count(0, busClock / 1000000 - 1);
    pit_hal_set_timer_period_count(1, 0xFFFFFFFF);
    pit_hal_configure_timer_chain(1, true);

    pit_hal_timer_start(0);
    pit_hal_timer_start(1);
}

/******************************************************************************
 * Timer Event
 *
 * It schedules interrupts at given (32bit)us interval of time.
 * It is implemented used the 16bit Low Power Timer that remains powered in all
 * power modes.
 ******************************************************************************/
static void lptmr_isr(void);

static void lptmr_init(void) {
    clock_hal_set_gate(kSimClockModuleLPTIMER, 0, true);

    /* Set interrupt handler */
    NVIC_SetVector(LPTimer_IRQn, (uint32_t)lptmr_isr);
    NVIC_EnableIRQ(LPTimer_IRQn);

    /* TODO: check clock manager, due to nonstandard 50 MHz */
    //No suitable external oscillator clock -> Use fast internal oscillator (4MHz / divider)
    MCG->C1 |= MCG_C1_IRCLKEN_MASK;
    MCG->C2 |= MCG_C2_IRCS_MASK;
    LPTMR0->PSR =  LPTMR_PSR_PCS(0);
    switch (MCG->SC & MCG_SC_FCRDIV_MASK) {
        case MCG_SC_FCRDIV(0):                  //4MHz
            LPTMR0->PSR |= LPTMR_PSR_PRESCALE(1);
            break;
        case MCG_SC_FCRDIV(1):                  //2MHz
            LPTMR0->PSR |= LPTMR_PSR_PRESCALE(0);
            break;
        default:                                //1MHz or anything else, in which case we put it on 1MHz
            MCG->SC &= ~MCG_SC_FCRDIV_MASK;
            MCG->SC |= MCG_SC_FCRDIV(2);
            LPTMR0->PSR |= LPTMR_PSR_PBYP_MASK;
    }
}

void us_ticker_disable_interrupt(void) {
    BW_LPTMR_CSR_TIE(0);
}

void us_ticker_clear_interrupt(void) {
    // we already clear interrupt in lptmr_isr
}

static uint32_t us_ticker_int_counter = 0;
static uint16_t us_ticker_int_remainder = 0;

static void lptmr_set(unsigned short count) {
    HW_LPTMR_CSR_WR(0);
    BW_LPTMR_CMR_COMPARE(count);
    BW_LPTMR_CSR_TIE(1);
    BW_LPTMR_CSR_TEN(1);
}

static void lptmr_isr(void) {
    // write 1 to TCF to clear the LPT timer compare flag
    BW_LPTMR_CSR_TCF(1);

    if (us_ticker_int_counter > 0) {
        lptmr_set(0xFFFF);
        us_ticker_int_counter--;
    } else {
        if (us_ticker_int_remainder > 0) {
            lptmr_set(us_ticker_int_remainder);
            us_ticker_int_remainder = 0;
        } else {
            // This function is going to disable the interrupts if there are
            // no other events in the queue
            us_ticker_irq_handler();
        }
    }
}

void us_ticker_set_interrupt(unsigned int timestamp) {
    int delta = (int)(timestamp - us_ticker_read());
    if (delta <= 0) {
        // This event was in the past:
        us_ticker_irq_handler();
        return;
    }

    us_ticker_int_counter   = (uint32_t)(delta >> 16);
    us_ticker_int_remainder = (uint16_t)(0xFFFF & delta);
    if (us_ticker_int_counter > 0) {
        lptmr_set(0xFFFF);
        us_ticker_int_counter--;
    } else {
        lptmr_set(us_ticker_int_remainder);
        us_ticker_int_remainder = 0;
    }
}
