/*
 * Copyright (c) 2019, Nuvoton Technology Corporation
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

#if defined(NU_HXT_ENABLE) && (NU_HXT_ENABLE == 1UL)
    /* HXT Enable: Set XT1_OUT(PF.2) and XT1_IN(PF.3) to input mode */
    PF->MODE &= ~(GPIO_MODE_MODE2_Msk | GPIO_MODE_MODE3_Msk);
#endif

    /* LXT Enable: Set X32_OUT(PF.4) and X32_IN(PF.5) to input mode */
    PF->MODE &= ~(GPIO_MODE_MODE4_Msk | GPIO_MODE_MODE5_Msk);

    /* Enable HIRC clock (Internal RC 48MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
#if defined(NU_HXT_ENABLE) && (NU_HXT_ENABLE == 1UL)
    /* Enable HXT clock (external XTAL 12MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);
#endif
    /* Enable LIRC for lp_ticker */
    CLK_EnableXtalRC(CLK_PWRCTL_LIRCEN_Msk);
    /* Enable LXT for RTC */
    CLK_EnableXtalRC(CLK_PWRCTL_LXTEN_Msk);

    /* Wait for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
#if defined(NU_HXT_ENABLE) && (NU_HXT_ENABLE == 1UL)
    /* Wait for HXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);
#endif
    /* Wait for LIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_LIRCSTB_Msk);
    /* Wait for LXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_LXTSTB_Msk);

#if defined(NU_HXT_ENABLE) && (NU_HXT_ENABLE == 1UL)
    /* HXT Enable: Disable digital input path of analog pin XT1_OUT to prevent leakage */
    GPIO_DISABLE_DIGITAL_PATH(PF, (1ul << 2));
    /* HXT Enable: Disable digital input path of analog pin XT1_IN to prevent leakage */
    GPIO_DISABLE_DIGITAL_PATH(PF, (1ul << 3));
#endif

    /* LXT Enable: Disable digital input path of analog pin X32_OUT to prevent leakage */
    GPIO_DISABLE_DIGITAL_PATH(PF, (1ul << 4));
    /* LXT Enable: Disable digital input path of analog pin XT32_IN to prevent leakage */
    GPIO_DISABLE_DIGITAL_PATH(PF, (1ul << 5));

    /* Select HCLK clock source as HIRC and HCLK clock divider as 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Set both PCLK0 and PCLK1 as HCLK/2 */
    CLK->PCLKDIV = CLK_PCLKDIV_APB0DIV_DIV2 | CLK_PCLKDIV_APB1DIV_DIV2;
    
    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();

    /* Lock protected registers */
    SYS_LockReg();
}
