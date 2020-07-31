/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
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
#include "mbed_drv_cfg.h"

#define SHIFT_NUM      5  /* P0/32 */

static int us_ticker_inited = 0;

void us_ticker_init(void)
{
    GIC_DisableIRQ(OSTMI1_IRQn);
    GIC_ClearPendingIRQ(OSTMI1_IRQn);

    /* Power Control for Peripherals      */
    volatile uint8_t dummy_buf;
    CPG.STBCR3.BYTE  &= ~(0x20u);
    dummy_buf = CPG.STBCR3.BYTE;  /* (Dummy read)                                         */
    (void)dummy_buf;

    if (us_ticker_inited) {
        return;
    }
    us_ticker_inited = 1;

    // timer settings
    OSTM1.OSTMnTT.BYTE   = 0x01;    /* Stop the counter and clears the OSTM1TE bit.     */
    OSTM1.OSTMnCTL.BYTE  = 0x02;    /* Free running timer mode. Interrupt disabled when star counter  */

    OSTM1.OSTMnTS.BYTE   = 0x1;     /* Start the counter and sets the OSTM0TE bit.     */

    // INTC settings
    InterruptHandlerRegister(OSTMI1_IRQn, (void (*)(uint32_t))us_ticker_irq_handler);
    GIC_SetPriority(OSTMI1_IRQn, 5);
    GIC_SetConfiguration(OSTMI1_IRQn, 3);
}

void us_ticker_free(void)
{
    GIC_DisableIRQ(OSTMI1_IRQn);
    GIC_ClearPendingIRQ(OSTMI1_IRQn);

    /* Power Control for Peripherals      */
    volatile uint8_t dummy_buf;
    CPG.STBCR3.BYTE  |= 0x20u;
    dummy_buf = CPG.STBCR3.BYTE;  /* (Dummy read)                                         */
    (void)dummy_buf;
}

uint32_t us_ticker_read()
{
    return (uint32_t)(OSTM1.OSTMnCNT.LONG >> SHIFT_NUM);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    OSTM1.OSTMnCMP.LONG = (uint32_t)(timestamp << SHIFT_NUM);
    GIC_EnableIRQ(OSTMI1_IRQn);
}

void us_ticker_fire_interrupt(void)
{
    GIC_SetPendingIRQ(OSTMI1_IRQn);
    GIC_EnableIRQ(OSTMI1_IRQn);
}

void us_ticker_disable_interrupt(void)
{
    GIC_DisableIRQ(OSTMI1_IRQn);
}

void us_ticker_clear_interrupt(void)
{
    GIC_ClearPendingIRQ(OSTMI1_IRQn);
}

const ticker_info_t *us_ticker_get_info()
{
    static const ticker_info_t info = {
        (uint32_t)((float)RENESAS_RZ_A2_P0_CLK * 2 / (float)(1 << SHIFT_NUM) + 0.5f),
        (32 - SHIFT_NUM)
    };
    return &info;
}

