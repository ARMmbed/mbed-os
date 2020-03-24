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

#include "cmsis.h"
#include "mbed_error.h"

void mbed_sdk_init(void)
{
    // NOTE: Support singleton semantics to be called from other init functions
    static int inited = 0;
    if (inited) {
        return;
    }
    inited = 1;

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)    
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable HIRC clock (Internal RC 12MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
    /* Enable HXT clock (external XTAL 12MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);
    /* Enable LIRC for lp_ticker */
    CLK_EnableXtalRC(CLK_PWRCTL_LIRCEN_Msk);
    /* Enable LXT for RTC */
    CLK_EnableXtalRC(CLK_PWRCTL_LXTEN_Msk);
    /* Enable HIRC48 clock (Internal RC 48MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRC48EN_Msk);

    /* Wait for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
    /* Wait for HXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);
    /* Wait for LIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_LIRCSTB_Msk);
    /* Wait for LXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_LXTSTB_Msk);
    /* Wait for HIRC48 clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRC48STB_Msk);

    
#if defined(NU_CHIP_MAJOR) && (NU_CHIP_MAJOR == 1UL)
    /* NOTE: There is a reset halt issue with PLL in A version. Work around it
     *       by using HIRC48 instead of PLL as HCLK clock source. */

    /* Trim HIRC48 to 48M against LXT */

    /* Reset TISTS48M status flags */
    SYS->TISTS48M |= (SYS_TISTS48M_FREQLOCK_Msk | SYS_TISTS48M_TFAILIF_Msk | SYS_TISTS48M_CLKERRIF_Msk);
    
    /* With reference clock from LXT, start trimming HIRC48 to 48M */
    SYS->TCTL48M = (SYS->TCTL48M & ~(SYS_TCTL48M_FREQSEL_Msk | SYS_TCTL48M_REFCKSEL_Msk)) | (0x01 << SYS_TCTL48M_FREQSEL_Pos) | (0x00 << SYS_TCTL48M_REFCKSEL_Pos);
    
    /* Wait for HIRC48 clock locked */
    while (1)
    {
        if (SYS->TISTS48M & SYS_TISTS48M_FREQLOCK_Msk)
        {
            break;
        }
        else if (SYS->TISTS48M & SYS_TISTS48M_TFAILIF_Msk)
        {
            error("HIRC48 auto-trim error: SYS_TISTS48M_TFAILIF_Msk");
        }
        else if (SYS->TISTS48M & SYS_TISTS48M_CLKERRIF_Msk)
        {
            error("HIRC48 auto-trim error: SYS_TISTS48M_CLKERRIF_Msk");
        }
    }
    
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC48, CLK_CLKDIV0_HCLK(1UL));
#else
    /* Set core clock as 64M from PLL */
    CLK_SetCoreClock(FREQ_64MHZ);
#endif
    
    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();

    /* Lock protected registers */
    SYS_LockReg();
#else
    SystemCoreClockUpdate();
#endif		
}
