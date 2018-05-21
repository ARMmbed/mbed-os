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
#include "us_ticker_api.h"
#include "lp_ticker_api.h"

#define rTIMER_INTSTATE     (RDA_TIMINTST->INTST)
#define rTIMER1_INTCLEAR    (RDA_TIM1->INTCLR)
#define rTIMER1_CONTROL     (RDA_TIM1->TCTRL)
#define rPOWER_CONTROL      (RDA_SCU->PWRCTRL)

#define RDA_TIMER_IRQn      (TIMER_IRQn)

static uint8_t is_timer_irq_set = 0;

extern int us_ticker_soft_int_flag;
#if DEVICE_LOWPOWERTIMER
extern int lp_ticker_soft_int_flag;
#endif /* DEVICE_LOWPOWERTIMER */

#if DEVICE_RTC
extern void rtc_base_update(void);
#endif /* DEVICE_RTC */

static void rda_timer_isr(void)
{
    uint32_t int_status = rTIMER_INTSTATE & 0x000FUL;
    if ((int_status & (0x01UL << 2)) || us_ticker_soft_int_flag) {
        /* Check the flag firstly, because following hanlder can change it */
        if (us_ticker_soft_int_flag) {
            us_ticker_soft_int_flag = 0;
        }
        us_ticker_irq_handler();
    }

#if DEVICE_LOWPOWERTIMER
    if ((int_status & (0x01UL << 3)) || lp_ticker_soft_int_flag) {
        /* Check the flag firstly, because following hanlder can change it */
        if (lp_ticker_soft_int_flag) {
            lp_ticker_soft_int_flag = 0;
        }
        lp_ticker_irq_handler();
    }
#endif /* DEVICE_LOWPOWERTIMER */

    if (int_status & (0x01UL << 1)) {
        rPOWER_CONTROL |= ((0x01UL << 28) | (0x01UL << 27)); // clear int & ts
        __DSB();
        while (rPOWER_CONTROL & (0x01UL << 28));
#if DEVICE_RTC
        rtc_base_update();
#endif /* DEVICE_RTC */
    }
}

void rda_timer_irq_set(void)
{
    if (0 == is_timer_irq_set) {
        is_timer_irq_set = 1;
        NVIC_SetVector(RDA_TIMER_IRQn, (uint32_t)rda_timer_isr);
        NVIC_SetPriority(RDA_TIMER_IRQn, 0x1FUL);
        NVIC_EnableIRQ(RDA_TIMER_IRQn);
    }
}
