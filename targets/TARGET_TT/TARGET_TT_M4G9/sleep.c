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
#include "sleep_api.h"

// Number of warm-up cycle = (warming up time (s) / clock period (s)) - 16
#define CG_WUODR_INT_5MS                ((uint16_t)0x0C34)
#define CG_STBY_MODE_IDLE               0x0
#define CG_STBY_MODE_STOP1              0x1
#define EXTERNEL_OSC_MASK               0xFFFFFFF1
#define SIWDT_DISABLE                   0xB1
#define WUPT_LOWER_MASK                 0x000F
#define WUPT_UPPER_MASK                 0xFFF0

static void external_losc_enable(void);

void hal_sleep(void)
{
    // Set low power consumption mode IDLE
    TSB_CG->STBYCR = CG_STBY_MODE_IDLE;

    // Enter idle mode
    __DSB();
    __WFI();
}

void hal_deepsleep(void)
{
    uint32_t wupt_lower = 0U;
    uint32_t wupt_upper = 0U;
    uint32_t tmp = 0U;

    TSB_CG_FSYSMENB_IPMENB31 = TXZ_ENABLE;

    TSB_SIWD0->EN = TXZ_DISABLE;
    TSB_SIWD0->CR = SIWDT_DISABLE;


    while ((TSB_FC->SR0 & TXZ_DONE) != TXZ_DONE) {
        // Flash wait
    }

    while (TSB_CG_WUPHCR_WUEF) {
        // Wait for end of Warming-up for IHOSC1
    }

    TSB_CG_WUPHCR_WUCLK = TXZ_DISABLE;
    wupt_lower = ((CG_WUODR_INT_5MS & WUPT_LOWER_MASK) << 16U);
    wupt_upper = ((CG_WUODR_INT_5MS & WUPT_UPPER_MASK) << 16U);
    TSB_CG->WUPHCR |= (wupt_lower | wupt_upper);
    TSB_CG->STBYCR = CG_STBY_MODE_STOP1;
    TSB_CG_PLL0SEL_PLL0SEL = TXZ_DISABLE;


    while (TSB_CG_PLL0SEL_PLL0ST) {
        // Wait for PLL status of fsys until off state(fosc=0)
    }

    // Stop PLL of fsys
    TSB_CG_PLL0SEL_PLL0ON = TXZ_DISABLE;
    TSB_CG_OSCCR_IHOSC1EN = TXZ_ENABLE;
    TSB_CG_OSCCR_OSCSEL = TXZ_DISABLE;

    while (TSB_CG_OSCCR_OSCF) {
        // Wait for fosc status until IHOSC1 = 0
    }

    tmp = TSB_CG->OSCCR;
    tmp &= EXTERNEL_OSC_MASK;
    TSB_CG->OSCCR = tmp;


    // Enter stop1 mode
    __DSB();
    __WFI();

    // Switch over from IHOSC to EHOSC
    // After coming out off sleep mode, Restore the clock setting to EHOSC.
    external_losc_enable();
}

static void external_losc_enable(void)
{
    uint32_t wupt_lower = 0U;
    uint32_t wupt_upper = 0U;

    // Enable high-speed oscillator
    TSB_CG->OSCCR  |= (TXZ_ENABLE << 1);

    // Select internal(fIHOSC) as warm-up clock
    wupt_lower = ((CG_WUODR_INT_5MS & WUPT_LOWER_MASK) << 16U);
    wupt_upper = ((CG_WUODR_INT_5MS & WUPT_UPPER_MASK) << 16U);
    TSB_CG->WUPHCR |= (wupt_lower | wupt_upper);

    // Start warm-up
    TSB_CG->WUPHCR |= TXZ_ENABLE;

    // Wait until EHOSC become stable
    while ((TSB_CG->WUPHCR & 0x0002)) {
        // Do nothing
    }

    // Set fosc source
    TSB_CG->OSCCR |= (1 << 8);

    // Wait for <OSCSEL> to become "1"
    while (!((TSB_CG->OSCCR & 0x200)>> 9)) {
        // Do nothing...
    }

    // Stop IHOSC
    TSB_CG->OSCCR &= ~TXZ_ENABLE;
}
