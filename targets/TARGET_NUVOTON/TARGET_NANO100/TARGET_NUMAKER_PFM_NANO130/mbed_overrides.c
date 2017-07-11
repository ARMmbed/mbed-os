/* mbed Microcontroller Library
 * Copyright (c) 2015-2017 Nuvoton
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

#include "cmsis.h"

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

    /* Enable HIRC clock (internal OSC 12MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRC_EN_Msk);
    /* Enable HXT clock (external XTAL 12MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXT_EN_Msk);
    /* Enable LIRC clock (OSC 10KHz) for lp_ticker */
    CLK_EnableXtalRC(CLK_PWRCTL_LIRC_EN_Msk);
    /* Enable LXT clock (XTAL 32KHz) for RTC */
    CLK_EnableXtalRC(CLK_PWRCTL_LXT_EN_Msk);

    /* Wait for HIRC clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_HIRC_STB_Msk);
    /* Wait for HXT clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_HXT_STB_Msk);
    /* Wait for LIRC clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_LIRC_STB_Msk);
    /* Wait for LXT clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_LXT_STB_Msk);

    /* Set HCLK source form HXT and HCLK source divide 1  */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HXT, CLK_HCLK_CLK_DIVIDER(1));

    /*  Set HCLK frequency 42MHz */
    CLK_SetCoreClock(42000000);

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();

    /* Lock protected registers */
    SYS_LockReg();
}
