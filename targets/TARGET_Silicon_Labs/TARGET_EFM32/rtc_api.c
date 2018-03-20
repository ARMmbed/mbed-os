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

#if (defined RTC_COUNT) && (RTC_COUNT > 0)
#include "em_rtc.h"
#endif

#if (defined RTCC_COUNT) && (RTCC_COUNT > 0)
#include "em_rtcc.h"
#endif

static bool         rtc_inited      = false;
static bool         rtc_cancelled   = false;
static time_t       time_base       = 0;
static uint32_t     useflags        = 0;
static uint32_t     time_extend     = 0;
static uint32_t     extended_comp0  = 0;

static void (*comp0_handler)(void)  = NULL;

#ifndef RTCC_PRESENT
/* Using RTC API */

#if RTC_CLOCKDIV_INT > 16
#error invalid prescaler value RTC_CLOCKDIV_INT, since LP ticker resolution will exceed 1ms.
#endif

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
        if (comp0_handler != NULL && ((time_extend == extended_comp0) || (rtc_cancelled))) {
            rtc_cancelled = false;
            comp0_handler();
        }
    }
}

uint64_t rtc_get_full(void)
{
    uint64_t ticks = 0;

    do
    {
        ticks = RTC_CounterGet();

        if (RTC_IntGet() & RTC_IF_OF) {
            RTC_IntClear(RTC_IF_OF);
            /* RTC has overflowed in a critical section, so handle the overflow here */
            time_extend += 1;
        }

        ticks += (uint64_t)time_extend << RTC_BITS;
    }
    while ( (ticks & RTC_MAX_VALUE) != RTC_CounterGet() );

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
        rtc_inited = false;
    }
}

void rtc_enable_comp0(bool enable)
{
    RTC_FreezeEnable(true);
    if (!enable) {
        RTC_IntDisable(RTC_IF_COMP0);
    } else {
        RTC_IntEnable(RTC_IF_COMP0);
    }
    RTC_FreezeEnable(false);
}

void rtc_set_comp0_value(uint64_t value, bool enable)
{
    rtc_enable_comp0(false);

    /* Set callback */
    RTC_FreezeEnable(true);
    extended_comp0 = (uint32_t) (value >> RTC_BITS);
    RTC_CompareSet(0, (uint32_t) (value & RTC_MAX_VALUE));
    RTC_FreezeEnable(false);

    rtc_enable_comp0(enable);
}

void rtc_force_comp0(void)
{
    rtc_cancelled = true;
    RTC_IntSet(RTC_IFS_COMP0);
}

#else
/* Using RTCC API */

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
        if (comp0_handler != NULL && ((time_extend == extended_comp0) || (rtc_cancelled))) {
            comp0_handler();
        }
    }
}

uint64_t rtc_get_full(void)
{
    uint64_t ticks = 0;

    do
    {
        ticks = RTCC_CounterGet();

        if (RTCC_IntGet() & RTCC_IF_OF) {
            RTCC_IntClear(RTCC_IF_OF);
            /* RTCC has overflowed in critical section, so handle the rollover here */
            time_extend += 1;
        }

        ticks += (uint64_t)time_extend << RTC_BITS;
    }
    while ( (ticks & RTC_MAX_VALUE) != RTCC_CounterGet() );

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
#if RTC_CLOCKDIV_INT == 1
        init.presc = rtccCntPresc_1;
#elif RTC_CLOCKDIV_INT == 2
        init.presc = rtccCntPresc_2;
#elif RTC_CLOCKDIV_INT == 4
        init.presc = rtccCntPresc_4;
#elif RTC_CLOCKDIV_INT == 8
        init.presc = rtccCntPresc_8;
#elif RTC_CLOCKDIV_INT == 16
        init.presc = rtccCntPresc_16;
#else
#error invalid prescaler value RTC_CLOCKDIV_INT, since LP ticker resolution will exceed 1ms.
#endif

        /* Enable Interrupt from RTC */
        RTCC_IntEnable(RTCC_IEN_OF);
        NVIC_SetVector(RTCC_IRQn, (uint32_t)RTCC_IRQHandler);
        NVIC_EnableIRQ(RTCC_IRQn);

        /* Initialize */
        RTCC_Init(&init);
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
        rtc_inited = false;
    }
}

void rtc_enable_comp0(bool enable)
{
    if(!enable) {
        RTCC_IntDisable(RTCC_IF_CC0);
    } else {
        RTCC_IntEnable(RTCC_IF_CC0);
    }
}

void rtc_set_comp0_value(uint64_t value, bool enable)
{
    rtc_enable_comp0(false);

    /* init channel */
    RTCC_CCChConf_TypeDef ccchConf = RTCC_CH_INIT_COMPARE_DEFAULT;
    RTCC_ChannelInit(0,&ccchConf);
    /* Set callback */
    extended_comp0 = (uint32_t) (value >> RTC_BITS);
    RTCC_ChannelCCVSet(0, (uint32_t) (value & RTC_MAX_VALUE));

    rtc_enable_comp0(enable);
}

void rtc_force_comp0(void)
{
    rtc_cancelled = true;
    RTCC_IntSet(RTCC_IFS_CC0);
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
