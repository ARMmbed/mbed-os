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

#include "device.h"
#if DEVICE_RTC

#include "rtc_api.h"
#include "rtc_api_HAL.h"
#include "em_cmu.h"
#include "em_rtc.h"
#include "sleep_api.h"
#include "sleepmodes.h"

static bool rtc_inited = false;
static time_t time_base = 0;
static uint32_t useflags = 0;

static void (*comp0_handler)(void) = NULL; 

#define RTC_LEAST_ACTIVE_SLEEPMODE EM2


void RTC_IRQHandler(void)
{
    uint32_t flags;
    flags = RTC_IntGet();
    if (flags & RTC_IF_OF)
    {
        RTC_IntClear(RTC_IF_OF);
        /* RTC has overflowed (24 bits). Use time_base as software counter for upper 8 bits. */
        time_base += 1 << 24;
    }
    if (flags & RTC_IF_COMP0)
    {
        RTC_IntClear(RTC_IF_COMP0);
        if (comp0_handler != NULL)
        {
            comp0_handler();
        }
    }
}

void rtc_set_comp0_handler(uint32_t handler)
{
    comp0_handler = (void (*)(void)) handler;
}

void rtc_init(void) 
{
    /* Register that the RTC is used for timekeeping. */
    rtc_init_real(RTC_INIT_RTC);
}


void rtc_init_real(uint32_t flags)
{
    useflags |= flags;

    if (!rtc_inited)
    {
        /* Start LFXO and wait until it is stable */
        CMU_OscillatorEnable(cmuOsc_LFXO, true, true);

        /* Route the LFXO clock to the RTC */
        CMU_ClockSelectSet(cmuClock_LFA, LOW_ENERGY_CLOCK_SOURCE);
        CMU_ClockEnable(cmuClock_RTC, true);

        /* Enable clock to the interface of the low energy modules */
        CMU_ClockEnable(cmuClock_CORELE, true);

        /* Scale clock to save power */
        CMU_ClockDivSet(cmuClock_RTC, RTC_CLOCKDIV);

        /* Initialize RTC */
        RTC_Init_TypeDef init = RTC_INIT_DEFAULT;
        init.enable = 1;
        /* Don't use compare register 0 as top value */
        init.comp0Top = 0;

        /* Enable Interrupt from RTC */
        RTC_IntEnable(RTC_IEN_OF);
        NVIC_EnableIRQ(RTC_IRQn);
        NVIC_SetVector(RTC_IRQn, (uint32_t)RTC_IRQHandler);

        /* Initialize */
        RTC_Init(&init);

        blockSleepMode(RTC_LEAST_ACTIVE_SLEEPMODE);
        rtc_inited = true;
    }
}

void rtc_free(void) 
{
    rtc_free_real(RTC_INIT_RTC);
}

void rtc_free_real(uint32_t flags)
{
    /* Clear use flag */
    flags &= ~flags; 

    /* Disable the RTC if it was inited and is no longer in use by anyone. */
    if (rtc_inited && (flags == 0))
    {
        NVIC_DisableIRQ(RTC_IRQn);
        RTC_Reset();
        CMU_ClockEnable(cmuClock_RTC, false);
        unblockSleepMode(RTC_LEAST_ACTIVE_SLEEPMODE);
        rtc_inited = false;
    }
}

int rtc_isenabled(void)
{
    return rtc_inited;
}

time_t rtc_read(void)
{
    return (time_t) ((RTC_CounterGet() + time_base) >> RTC_FREQ_SHIFT);
}

void rtc_write(time_t t)
{
    /* We have to check that the RTC did not tick while doing this. */
    /* If the RTC ticks we just redo this. */
    uint32_t rtc_count;
    do {
        rtc_count = RTC_CounterGet();
        time_base = (t << RTC_FREQ_SHIFT) - rtc_count;
    } while (rtc_count != RTC_CounterGet());
}

#endif
