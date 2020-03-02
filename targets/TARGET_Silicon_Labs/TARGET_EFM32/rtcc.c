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
#if DEVICE_RTC || DEVICE_LPTICKER

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
    RTCC->RET[0].REG = 0;
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
    return RTCC_CounterGet() + RTCC->RET[0].REG;
}

void rtc_write(time_t t)
{
    core_util_critical_section_enter();
    RTCC->RET[0].REG = t - RTCC_CounterGet();
    core_util_critical_section_exit();
}

/************************* LP_TICKER **************************/
const ticker_info_t* lp_ticker_get_info(void)
{
    static const ticker_info_t rtc_info = {
        LOW_ENERGY_CLOCK_FREQUENCY,
        32
    };
    return &rtc_info;
}

void RTCC_IRQHandler(void)
{
    uint32_t flags;
    flags = RTCC_IntGet();
    if (flags & RTCC_IF_CC0) {
        lp_ticker_irq_handler();
    }
}

void lp_ticker_init()
{
    if (!lptick_inited) {
        rtc_init();
        core_util_critical_section_enter();
        lptick_offset = RTCC_CounterGet();
        core_util_critical_section_exit();
        RTCC_CCChConf_TypeDef lp_chan_init = RTCC_CH_INIT_COMPARE_DEFAULT;
        lp_chan_init.compBase = rtccCompBasePreCnt;
        lp_chan_init.compMask = 0;
        RTCC_ChannelInit(0, &lp_chan_init);
        lptick_inited = true;

        /* Enable Interrupt from RTCC in NVIC, but don't start generating them */
        RTCC_IntDisable(RTCC_IF_CC0);
        RTCC_IntClear(RTCC_IF_CC0);
        NVIC_EnableIRQ(RTCC_IRQn);
    } else {
        RTCC_IntDisable(RTCC_IF_CC0);
        RTCC_IntClear(RTCC_IF_CC0);
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
    RTCC_ChannelCCVSet(0, timestamp + (lptick_offset << 15));
    RTCC_IntEnable(RTCC_IF_CC0);
}

void lp_ticker_fire_interrupt(void)
{
    RTCC_IntEnable(RTCC_IF_CC0);
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
    core_util_critical_section_enter();
    uint32_t ticks = RTCC_CombinedCounterGet() - (lptick_offset << 15);
    core_util_critical_section_exit();

    return (timestamp_t) (ticks);
}
#endif /* RTCC_PRESENT */
#endif /* DEVICE_RTC */