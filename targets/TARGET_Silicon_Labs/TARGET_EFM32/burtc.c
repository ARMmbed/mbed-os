/***************************************************************************//**
 * @file rtc_burtc.c
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
#if DEVICE_RTC

/* Use BURTC on devices that have it, and don't use the RTCC abstraction */
#if defined(BURTC_PRESENT) && !defined(RTCC_PRESENT)

#include "clocking.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_burtc.h"
#include "rtc_api.h"

void rtc_init(void)
{
    /* Only reset & configure the RTC if it has never run before */
    if(BUS_RegBitRead(&BURTC->CTRL, _BURTC_CTRL_RSTEN_SHIFT)) {
        EMU_EM4Init_TypeDef em4_init = EMU_EM4INIT_DEFAULT;
#if (LOW_ENERGY_CLOCK_SOURCE == LFXO)
        em4_init.osc = emuEM4Osc_LFXO;
#elif (LOW_ENERGY_CLOCK_SOURCE == LFRCO)
        em4_init.osc = emuEM4Osc_LFRCO;
#else
#error "Can't use BURTC on mbed with ULFRCO"
#endif
        EMU_EM4Init(&em4_init);

        BURTC_Init_TypeDef burtc_init = BURTC_INIT_DEFAULT;
        burtc_init.mode = burtcModeEM4;
#if (LOW_ENERGY_CLOCK_SOURCE == LFXO)
        burtc_init.clkSel = burtcClkSelLFXO;
#elif (LOW_ENERGY_CLOCK_SOURCE == LFRCO)
        burtc_init.clkSel = burtcClkSelLFRCO;
#else
#error "Can't use BURTC on mbed with ULFRCO"
#endif
        burtc_init.clkDiv = burtcClkDiv_128;
        burtc_init.lowPowerMode = burtcLPEnable;

        BURTC_Reset();
        BUS_RegBitWrite(&RMU->CTRL, _RMU_CTRL_BURSTEN_SHIFT, 0);
        BURTC_Init(&burtc_init);
        BURTC_RetRegSet(0, 0);
        BURTC_RetRegSet(1, 0);
    }
}

void rtc_free(void)
{
    /* Nothing to release here */
}

int rtc_isenabled(void)
{
    return (BUS_RegBitRead(&BURTC->CTRL, _BURTC_CTRL_RSTEN_SHIFT) == 0);
}

time_t rtc_read(void)
{
    uint32_t ts = 0;
    uint32_t ts2 = 1;
    do {
        if (BURTC->IF & BURTC_IF_OF) {
            BURTC_RetRegSet(1, BURTC_RetRegGet(1)+1);
            BURTC->IFC = BURTC_IFC_OF;
        }

        ts = ts2;
        ts2 = (BURTC_CounterGet() >> 8) + BURTC_RetRegGet(0) + (BURTC_RetRegGet(1) << 16);
    } while (ts != ts2);
    return ts2;
}

void rtc_write(time_t t)
{
    BURTC_RetRegSet(0, t - (BURTC_CounterGet() >> 8));
    BURTC_RetRegSet(1, 0);
    if (BURTC->IF & BURTC_IF_OF) {
        BURTC->IFC = BURTC_IFC_OF;
    }
}

#endif /* BURTC_PRESENT && !RTCC_PRESENT */
#endif /* DEVICE_RTC */