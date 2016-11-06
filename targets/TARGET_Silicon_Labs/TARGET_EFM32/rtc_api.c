/***************************************************************************//**
 * @file rtc_api.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "device.h"
#if DEVICE_RTC

#include "rtc_api.h"
#include "rtc_api_HAL.h"
#include "em_cmu.h"
#include "sleep_api.h"
#include "sleepmodes.h"

#if (defined RTC_COUNT) && (RTC_COUNT > 0)
#include "em_rtc.h"
#endif

#if (defined RTCC_COUNT) && (RTCC_COUNT > 0)
#include "em_rtcc.h"
#endif

static bool         rtc_inited  = false;
static time_t       time_base   = 0;
static uint32_t     useflags    = 0;
static uint32_t     time_extend = 0;

static void (*comp0_handler)(void) = NULL;

#ifndef RTCC_COUNT

/* Using RTC API */

#define RTC_LEAST_ACTIVE_SLEEPMODE  EM2
#define RTC_NUM_BITS                (24)

void RTC_IRQHandler(void)
{
    uint32_t flags;
    flags = RTC_IntGet();
    if (flags & RTC_IF_OF) {
        RTC_IntClear(RTC_IF_OF);
        /* RTC has overflowed (24 bits). Use time_extend as software counter for 32 more bits. */
        time_extend += 1;
    }
    if (flags & RTC_IF_COMP0) {
        RTC_IntClear(RTC_IF_COMP0);
        if (comp0_handler != NULL) {
            comp0_handler();
        }
    }
}

uint32_t rtc_get_32bit(void)
{
    uint32_t pending = (RTC_IntGet() & RTC_IF_OF) ? 1 : 0;
    return (RTC_CounterGet() + ((time_extend + pending) << RTC_NUM_BITS));
}

uint64_t rtc_get_full(void)
{
    uint64_t ticks = 0;
    ticks += time_extend;
    ticks = ticks << RTC_NUM_BITS;
    ticks += RTC_CounterGet();
    return ticks;
}

void rtc_init_real(uint32_t flags)
{
    useflags |= flags;

    if (!rtc_inited) {
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
        NVIC_SetVector(RTC_IRQn, (uint32_t)RTC_IRQHandler);
        NVIC_EnableIRQ(RTC_IRQn);

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
    useflags &= ~flags;

    /* Disable the RTC if it was inited and is no longer in use by anyone. */
    if (rtc_inited && (useflags == 0)) {
        NVIC_DisableIRQ(RTC_IRQn);
        RTC_Reset();
        CMU_ClockEnable(cmuClock_RTC, false);
        unblockSleepMode(RTC_LEAST_ACTIVE_SLEEPMODE);
        rtc_inited = false;
    }
}

#else

/* Using RTCC API */

#define RTCC_LEAST_ACTIVE_SLEEPMODE  EM2
#define RTCC_NUM_BITS                (32)

void RTCC_IRQHandler(void)
{
    uint32_t flags;
    flags = RTCC_IntGet();

    if (flags & RTCC_IF_OF) {
        RTCC_IntClear(RTCC_IF_OF);
        /* RTC has overflowed (32 bits). Use time_extend as software counter for 32 more bits. */
        time_extend += 1;
    }

    if (flags & RTCC_IF_CC0) {
        RTCC_IntClear(RTCC_IF_CC0);
        if (comp0_handler != NULL) {
            comp0_handler();
        }
    }
}

uint32_t rtc_get_32bit(void)
{
    return RTCC_CounterGet();
}

uint64_t rtc_get_full(void)
{
    uint64_t ticks = 0;
    ticks += time_extend;
    ticks = ticks << RTCC_NUM_BITS;
    ticks += RTCC_CounterGet();
    return ticks;
}

void rtc_init_real(uint32_t flags)
{
    useflags |= flags;

    if (!rtc_inited) {
        CMU_ClockEnable(cmuClock_RTCC, true);

        /* Enable clock to the interface of the low energy modules */
        CMU_ClockEnable(cmuClock_CORELE, true);

        /* Initialize RTC */
        RTCC_Init_TypeDef init = RTCC_INIT_DEFAULT;
        init.enable = 1;
        init.precntWrapOnCCV0 = false;
        init.cntWrapOnCCV1 = false;
#if RTC_CLOCKDIV_INT == 8
        init.presc = rtccCntPresc_8;
#else
#error invalid prescaler value RTC_CLOCKDIV_INT
#endif

        /* Enable Interrupt from RTC */
        RTCC_IntEnable(RTCC_IEN_OF);
        NVIC_SetVector(RTCC_IRQn, (uint32_t)RTCC_IRQHandler);
        NVIC_EnableIRQ(RTCC_IRQn);

        /* Initialize */
        RTCC_Init(&init);

        blockSleepMode(RTCC_LEAST_ACTIVE_SLEEPMODE);
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
    useflags &= ~flags;

    /* Disable the RTC if it was inited and is no longer in use by anyone. */
    if (rtc_inited && (useflags == 0)) {
        NVIC_DisableIRQ(RTCC_IRQn);
        RTCC_Reset();
        CMU_ClockEnable(cmuClock_RTCC, false);
        unblockSleepMode(RTCC_LEAST_ACTIVE_SLEEPMODE);
        rtc_inited = false;
    }
}

#endif /* RTCC_COUNT */

void rtc_set_comp0_handler(uint32_t handler)
{
    comp0_handler = (void (*)(void)) handler;
}

void rtc_init(void)
{
    /* Register that the RTC is used for timekeeping. */
    rtc_init_real(RTC_INIT_RTC);
}

int rtc_isenabled(void)
{
    return rtc_inited;
}

time_t rtc_read(void)
{
    return (time_t) (rtc_get_full() >> RTC_FREQ_SHIFT) + time_base;
}

time_t rtc_read_uncompensated(void)
{
    return (time_t) (rtc_get_full() >> RTC_FREQ_SHIFT);
}

void rtc_write(time_t t)
{
    /* We have to check that the RTC did not tick while doing this. */
    /* If the RTC ticks we just redo this. */
    uint32_t time;
    do {
        time = rtc_read_uncompensated();
        time_base = t - time;
    } while (time != (uint32_t)rtc_read_uncompensated());
}

#endif
