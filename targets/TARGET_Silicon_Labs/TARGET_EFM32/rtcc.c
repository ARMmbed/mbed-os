/***************************************************************************//**
 * @file rtc_rtcc.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2018 Silicon Labs, http://www.silabs.com</b>
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
#if DEVICE_RTC || DEVICE_LPTIMER

/* Use RTCC on devices that have it */
#if defined(RTCC_PRESENT)

#include "em_cmu.h"
#include "em_rmu.h"
#include "em_rtcc.h"
#include "rtc_api.h"
#include "lp_ticker_api.h"
#include "clocking.h"

static bool lptick_inited = false;
static uint32_t lptick_offset = 0;
static uint32_t extended_comp0 = 0;

void rtc_init(void)
{
    if (BUS_RegBitRead((&RTCC->CTRL), _RTCC_CTRL_ENABLE_SHIFT) != 0) {
        /* RTCC already up & running */
        return;
    }

    /* Make sure RTCC doesn't get reset (mbed specification) */
    RMU_ResetControl(rmuResetWdog, rmuResetModeLimited);
    RMU_ResetControl(rmuResetCoreLockup, rmuResetModeLimited);
    RMU_ResetControl(rmuResetSys, rmuResetModeLimited);
    RMU_ResetControl(rmuResetPin, rmuResetModeFull);

    /* Set up the RTCC and let it run, Forrest, run */
    RTCC_Reset();
    RTCC_Init_TypeDef rtcc_init = RTCC_INIT_DEFAULT;
    rtcc_init.presc = rtccCntPresc_32768;
    RTCC_Init(&rtcc_init);
    RTCC_Enable(true);
}

void rtc_free(void)
{
    /* Can't turn off any clock gates here since other parts of the
     * system might be using the CORELE clock. */
}

int rtc_isenabled(void)
{
    return BUS_RegBitRead((&RTCC->CTRL), _RTCC_CTRL_ENABLE_SHIFT);
}

time_t rtc_read(void)
{
    return RTCC_CounterGet();
}

void rtc_write(time_t t)
{
    core_util_critical_section_enter();
    uint32_t diff = t - RTCC_CounterGet();
    if (extended_comp0 != 0xFFFFFFFFUL) {
        extended_comp0 += diff;
    }
    lptick_offset += diff;

    RTCC_CounterSet(t);
    core_util_critical_section_exit();
}

/************************* LP_TICKER **************************/
void RTCC_IRQHandler(void)
{
    uint32_t flags;
    flags = RTCC_IntGet();
    if (flags & RTCC_IF_CC0) {
        RTCC_IntClear(RTCC_IF_CC0);
        if ((RTCC_CounterGet() - lptick_offset) == extended_comp0) {
            RTCC_IntDisable(RTCC_IF_CC0);
            lp_ticker_irq_handler();
        }
        if (0xFFFFFFFFUL == extended_comp0) {
            RTCC_IntDisable(RTCC_IF_CC0);
            lp_ticker_irq_handler();
        }
    }
}

void lp_ticker_init()
{
    if (!lptick_inited) {
        rtc_init();
        lptick_offset = RTCC_CounterGet();
        RTCC_CCChConf_TypeDef lp_chan_init = RTCC_CH_INIT_COMPARE_DEFAULT;
        lp_chan_init.compBase = rtccCompBasePreCnt;
        lp_chan_init.compMask = 17;
        RTCC_ChannelInit(0, &lp_chan_init);
        lptick_inited = true;

        /* Enable Interrupt from RTCC */
        NVIC_EnableIRQ(RTCC_IRQn);
    }
}

void lp_ticker_free()
{
    if (lptick_inited) {
        lp_ticker_disable_interrupt();
        lp_ticker_clear_interrupt();
        lptick_inited = false;
    }
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    uint64_t rtc_compare_value;
    uint64_t current_ticks = 0;
    do
    {
        current_ticks = (uint64_t)((uint64_t)RTCC_CounterGet() - lptick_offset) << 15;
        current_ticks += RTCC_PreCounterGet();
    }
    while ( (current_ticks & 0x7FFF) != RTCC_PreCounterGet() );

    uint64_t ticks_temp = (current_ticks * 1000000) / LOW_ENERGY_CLOCK_FREQUENCY;
    timestamp_t current_time = ticks_temp & 0xFFFFFFFF;

    /* calculate offset value */
    timestamp_t offset = timestamp - current_time;

    /* If the requested timestamp is too far in the future, we might not be able
     * to set the interrupt accurately due to potentially having ticked between
     * calculating the timestamp to set and us calculating the offset. */
    if(offset > 0xFFFF0000) offset = 100;

    /* map offset to RTC value */
    // ticks = offset * RTC frequency div 1000000
    rtc_compare_value = ((uint64_t)offset * LOW_ENERGY_CLOCK_FREQUENCY) / 1000000;

    /* If RTC offset is less then 2 RTC ticks, the interrupt won't fire */
    if(rtc_compare_value < 2) {
        rtc_compare_value = 2;
    }

    rtc_compare_value += current_ticks;

    extended_comp0 = rtc_compare_value >> 15;
    RTCC_ChannelCCVSet(0, rtc_compare_value & 0xFFFFFFFFUL);
    RTCC_IntEnable(RTCC_IF_CC0);
}

void lp_ticker_fire_interrupt(void)
{
    extended_comp0 = 0xFFFFFFFFUL;
    RTCC_IntSet(RTCC_IF_CC0);
}

void lp_ticker_disable_interrupt()
{
    RTCC_IntDisable(RTCC_IF_CC0);
}

void lp_ticker_clear_interrupt()
{
    RTCC_IntClear(RTCC_IF_CC0);
}

timestamp_t lp_ticker_read()
{
    lp_ticker_init();

    uint64_t ticks_temp;
    uint64_t ticks = 0;

    do
    {
        ticks = (uint64_t)((uint64_t)RTCC_CounterGet() - lptick_offset) << 15;
        ticks += RTCC_PreCounterGet();
    }
    while ( (ticks & 0x7FFF) != RTCC_PreCounterGet() );

    /* ticks = counter tick value
     * timestamp = value in microseconds
     * timestamp = ticks * 1.000.000 / RTC frequency
     */

    ticks_temp = (ticks * 1000000) / LOW_ENERGY_CLOCK_FREQUENCY;
    return (timestamp_t) (ticks_temp & 0xFFFFFFFF);
}
#endif /* RTCC_PRESENT */
#endif /* DEVICE_RTC */