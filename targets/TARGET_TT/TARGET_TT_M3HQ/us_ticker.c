/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
 * SPDX-License-Identifier: Apache-2.0
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
#include "gpio_include.h"


#define T32A_INT_MASK ((uint32_t)0x0000000F)
#define TXZ_ENABLE    1
static bool us_ticker_inited = false;      // Is ticker initialized yet?

const ticker_info_t* us_ticker_get_info()
{
    static const ticker_info_t info = {
        1248125, // (39.94 MHz / 32 )
        32       // 32 bit counter
    };
    return &info;
}

void us_ticker_init(void)
{
    if (us_ticker_inited) {
        us_ticker_disable_interrupt();
        return;
    }
    us_ticker_inited = true;

    TSB_CG_FSYSENA_IPENA24 = TXZ_ENABLE;
    TSB_T32A0->MOD         = T32A_MODE_32;
    TSB_T32A0->RUNC        = (T32A_RUN_ENABLE | T32A_COUNT_STOP);
    TSB_T32A0->CRC         = T32A_PRSCLx_32;
    TSB_T32A0->IMC         = (T32A_IMUFx_MASK_REQ | T32A_IMOFx_MASK_REQ);
    TSB_T32A0->RUNC        = (T32A_RUN_ENABLE | T32A_COUNT_START);

    NVIC_SetVector(INTT32A00C_IRQn, (uint32_t)us_ticker_irq_handler);
    NVIC_EnableIRQ(INTT32A00C_IRQn);
}

uint32_t us_ticker_read(void)
{
    if (!us_ticker_inited) {
        us_ticker_init();
    }

    return (TSB_T32A0->TMRC);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    NVIC_DisableIRQ(INTT32A00C_IRQn);
    TSB_T32A0->RUNC  = (T32A_RUN_ENABLE | T32A_COUNT_STOP);
    TSB_T32A0->RGC1  = timestamp;
    NVIC_EnableIRQ(INTT32A00C_IRQn);
    TSB_T32A0->RUNC = (T32A_RUN_ENABLE | T32A_COUNT_START);
}

void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(INTT32A00C_IRQn);
    NVIC_EnableIRQ(INTT32A00C_IRQn);
}

void us_ticker_disable_interrupt(void)
{
    // Disable interrupts by NVIC
    NVIC_DisableIRQ(INTT32A00C_IRQn);
    NVIC_ClearPendingIRQ(INTT32A00C_IRQn);
    TSB_T32A0->STC = T32A_INT_MASK; 
}

void us_ticker_clear_interrupt(void)
{
    TSB_T32A0->STC = T32A_INT_MASK;
    NVIC_ClearPendingIRQ(INTT32A00C_IRQn);
}

void us_ticker_free(void)
{
    TSB_T32A0->RUNC  = (T32A_RUN_DISABLE | T32A_COUNT_STOP);
    us_ticker_inited = false;
    TSB_T32A0->STC   = 0x0F;
    // Clear Pending interrupt in NVIC
    NVIC_ClearPendingIRQ(INTT32A00C_IRQn);
    // Disable interrupt in NVIC
    NVIC_DisableIRQ(INTT32A00C_IRQn);
    // Disable Clock.
    TSB_CG_FSYSENA_IPENA24 = 0;
}
