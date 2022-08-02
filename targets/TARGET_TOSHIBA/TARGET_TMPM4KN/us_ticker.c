/* mbed Microcontroller Library
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2021
 * SPDX-License-Identifier: Apache-2.0
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
#include "txzp_t32a.h"
#include "TMPM4KNA.h"


static uint8_t us_ticker_inited = 0;    // Is ticker initialized yet?


const ticker_info_t *us_ticker_get_info()
{
    static const ticker_info_t info = {
        2500000,
        32
    };
    return &info;
}
// Initialize us_ticker
void us_ticker_init(void)
{
    if (us_ticker_inited) {
        us_ticker_disable_interrupt();
        return;
    }
    us_ticker_inited = 1;
    TSB_CG_FSYSMENA_IPMENA28 = TXZ_ENABLE;

    // Configure Timer T32A0
    TSB_T32A0->MOD  =  T32A_MODE_32;
    TSB_T32A0->RUNC = (T32A_RUN_ENABLE | T32A_COUNT_STOP);
    TSB_T32A0->CRC  =  T32A_PRSCLx_32;
    TSB_T32A0->IMC  = (T32A_IMUFx_MASK_REQ | T32A_IMOFx_MASK_REQ);
    TSB_T32A0->RUNC = (T32A_RUN_ENABLE | T32A_COUNT_START);

    NVIC_SetVector(INTT32A00AC_IRQn, (uint32_t)us_ticker_irq_handler);
    NVIC_EnableIRQ(INTT32A00AC_IRQn);
}

uint32_t us_ticker_read(void)
{
    uint32_t ret_val = 0;

    if (!us_ticker_inited) {
        us_ticker_init();
    }

    ret_val = (TSB_T32A0->TMRC);
    return ret_val;
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    NVIC_DisableIRQ(INTT32A00AC_IRQn);
    TSB_T32A0->RUNC = (T32A_RUN_ENABLE | T32A_COUNT_STOP);
    TSB_T32A0->RGC1 =  timestamp ;
    NVIC_EnableIRQ(INTT32A00AC_IRQn);
    TSB_T32A0->RUNC = (T32A_RUN_ENABLE | T32A_COUNT_START);
}

void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(INTT32A00AC_IRQn);
    NVIC_EnableIRQ(INTT32A00AC_IRQn);
}

void us_ticker_disable_interrupt(void)
{
    NVIC_ClearPendingIRQ(INTT32A00AC_IRQn);
    NVIC_DisableIRQ(INTT32A00AC_IRQn);
}

void us_ticker_clear_interrupt(void)
{
    NVIC_ClearPendingIRQ(INTT32A00AC_IRQn);
}

void us_ticker_free(void)
{
    TSB_T32A0->RUNC = T32A_RUN_DISABLE;
    NVIC_ClearPendingIRQ(INTT32A00AC_IRQn);
    NVIC_DisableIRQ(INTT32A00AC_IRQn);
    TSB_CG_FSYSMENA_IPMENA28 = TXZ_DISABLE;
}
