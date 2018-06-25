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
#include "us_ticker_api.h"

#define rTIMER_INTSTATE     (RDA_TIMINTST->INTST)
#define RDA_TIMER_IRQn      (TIMER_IRQn)
#define TIMER0_CONTROL_ENABLE       (0x01)

extern uint32_t us_ticker_soft_int_flag;
extern void us_ticker_irq_callback();

static void rda_timer_isr(void)
{
    uint32_t int_status = rTIMER_INTSTATE & 0x000FUL;

    if ((int_status == 0x05) ||((int_status == 0) && (us_ticker_soft_int_flag == 1))){
        us_ticker_irq_callback();
        us_ticker_irq_handler();
    }
}

void rda_timer_irq_set(void)
{
    NVIC_SetVector(RDA_TIMER_IRQn, (uint32_t)rda_timer_isr);
    NVIC_SetPriority(RDA_TIMER_IRQn, 0x1FUL);
    NVIC_EnableIRQ(RDA_TIMER_IRQn);
}
