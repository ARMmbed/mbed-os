/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
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
#include "us_ticker_api.h"
#include "mbed_critical.h"

#define TMR16A_100US                0x960 // fsys = fc = 24MHz, Ttmra = 1/24us, 100us*24us = 2400 = 0x960
#define TMR16A_SYSCK                ((uint32_t)0x00000001)
#define TMR16A_RUN                  ((uint32_t)0x00000001)
#define TMR16A_STOP                 ((uint32_t)0x00000000)
#define OVERFLOW_32BIT              (0xFFFFFFFF / 0x64)

static uint8_t us_ticker_inited = 0;                // Is ticker initialized yet?
static volatile uint32_t ticker_int_counter = 0;    // Amount of overflows until user interrupt
static volatile uint32_t us_ticker = 0;             // timer counter

void INT16A0_IRQHandler(void)
{
    us_ticker++;

    if (us_ticker > OVERFLOW_32BIT) {
        us_ticker = 0;
    }
}

void INT16A1_IRQHandler(void)
{
    us_ticker_irq_handler();
}

// initialize us_ticker
void us_ticker_init(void)
{
    // Enable clock supply to TA0
    CG_SetFcPeriphA(CG_FC_PERIPH_TMR16A, ENABLE);
    if (us_ticker_inited) {
        return;
    }
    us_ticker_inited = 1;

    // Stops and clear count operation
    TSB_T16A0->RUN = TMR16A_STOP;
    TSB_T16A0->CR = TMR16A_SYSCK;
    // Permits INTTA0 interrupt
    NVIC_EnableIRQ(INT16A0_IRQn);
    // Match counter set to max value
    TSB_T16A0->RG = TMR16A_100US;
    TSB_T16A0->RUN = TMR16A_RUN;
}

uint32_t us_ticker_read(void)
{
    uint32_t ret_val = 0;

    if (!us_ticker_inited) {
        us_ticker_init();
    }

    uint32_t tickerbefore = 0;
    do {
        tickerbefore = us_ticker;
        ret_val = (us_ticker * 100);
    } while (tickerbefore != us_ticker);

    return ret_val;
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t delta = 0;

    // Stops and clear count operation
    TSB_T16A1->RUN = TMR16A_STOP;
    TSB_T16A1->CR = TMR16A_SYSCK;
    // Set the compare register
    delta = (timestamp - us_ticker_read());
    TSB_T16A1->RG = delta;
    // Set Interrupt
    NVIC_EnableIRQ(INT16A1_IRQn);
    // Start TMR_TA1 timer
    TSB_T16A1->RUN = TMR16A_RUN;
}

void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(INT16A1_IRQn);
}

void us_ticker_disable_interrupt(void)
{
    NVIC_DisableIRQ(INT16A1_IRQn);
}

void us_ticker_clear_interrupt(void)
{
    //no flags to clear
}
