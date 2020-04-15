/*
 * Copyright (c) 2015-2016, Nuvoton Technology Corporation
 *
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

#include "analogin_api.h"

void WDT_IRQHandler(void)
{
    /* Check WDT interrupt flag */
    if (WDT_GET_TIMEOUT_INT_FLAG()) {
        WDT_CLEAR_TIMEOUT_INT_FLAG();
        WDT_RESET_COUNTER();
    }

    /* Check WDT wake-up flag */
    if (WDT_GET_TIMEOUT_WAKEUP_FLAG()) {
        WDT_CLEAR_TIMEOUT_WAKEUP_FLAG();
    }
}

void mbed_sdk_init(void)
{
    // NOTE: Support singleton semantics to be called from other init functions
    static int inited = 0;
    if (inited) {
        return;
    }
    inited = 1;
    
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable HIRC clock (Internal RC 22.1184MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
    /* Enable HXT clock (external XTAL 12MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);
    /* Enable LIRC for lp_ticker */
    CLK_EnableXtalRC(CLK_PWRCTL_LIRCEN_Msk);
    /* Enable LXT for RTC */
    CLK_EnableXtalRC(CLK_PWRCTL_LXTEN_Msk);

    /* Wait for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
    /* Wait for HXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);
    /* Wait for LIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_LIRCSTB_Msk);
    /* Wait for LXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_LXTSTB_Msk);

    /* Select HCLK clock source as HIRC and HCLK clock divider as 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));
    
    /* Set core clock as 192000000 from PLL */
    CLK_SetCoreClock(192000000);
    
    /* Set PCLK0/PCLK1 to HCLK/2 */
    CLK->PCLKDIV = (CLK_PCLKDIV_PCLK0DIV2 | CLK_PCLKDIV_PCLK1DIV2); // PCLK divider set 2
    
#if DEVICE_ANALOGIN
    /* Vref connect to internal */
    SYS->VREFCTL = (SYS->VREFCTL & ~SYS_VREFCTL_VREFCTL_Msk) | SYS_VREFCTL_VREF_3_0V;
#endif
    
    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();

    /* Lock protected registers */
    SYS_LockReg();

    /* Get around h/w issue with reset from power-down mode
     *
     * When UART interrupt enabled and WDT reset from power-down mode, in the next
     * cycle, UART interrupt keeps breaking in and cannot block unless via NVIC. To
     * get around it, we make up a signal of WDT wake-up from power-down mode in the
     * start of boot process on detecting WDT reset.
     */
    if (SYS_IS_WDT_RST()) {
        /* Enable IP module clock */
        CLK_EnableModuleClock(WDT_MODULE);

        /* Select IP clock source */
        CLK_SetModuleClock(WDT_MODULE, CLK_CLKSEL1_WDTSEL_LIRC, 0);

        /* The name of symbol WDT_IRQHandler() is mangled in C++ and cannot
         * override that in startup file in C. Note the NVIC_SetVector call
         * cannot be left out when WDT_IRQHandler() is redefined in C++ file.
         *
         * NVIC_SetVector(WDT_IRQn, (uint32_t) WDT_IRQHandler);
         */
        NVIC_EnableIRQ(WDT_IRQn);

        SYS_UnlockReg();

        /* Configure/Enable WDT */
        WDT->CTL = WDT_TIMEOUT_2POW4 |      // Timeout interval of 2^4 LIRC clocks
                WDT_CTL_WDTEN_Msk |         // Enable watchdog timer
                WDT_CTL_INTEN_Msk |         // Enable interrupt
                WDT_CTL_WKF_Msk |           // Clear wake-up flag
                WDT_CTL_WKEN_Msk |          // Enable wake-up on timeout
                WDT_CTL_IF_Msk |            // Clear interrupt flag
                WDT_CTL_RSTF_Msk |          // Clear reset flag
                !WDT_CTL_RSTEN_Msk |        // Disable reset
                WDT_CTL_RSTCNT_Msk;         // Reset up counter

        CLK_PowerDown();

        /* Clear all flags & Disable WDT/INT/WK/RST */
        WDT->CTL = (WDT_CTL_WKF_Msk | WDT_CTL_IF_Msk | WDT_CTL_RSTF_Msk | WDT_CTL_RSTCNT_Msk);

        NVIC_DisableIRQ(WDT_IRQn);

        SYS_LockReg();
    }
}
