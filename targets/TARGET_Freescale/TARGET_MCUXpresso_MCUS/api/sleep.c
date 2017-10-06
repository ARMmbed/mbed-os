/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "sleep_api.h"
#include "cmsis.h"
#include "fsl_smc.h"
#include "fsl_clock_config.h"

void hal_sleep(void)
{
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);

    SMC_SetPowerModeWait(SMC);
}

void hal_deepsleep(void)
{
#if (defined(FSL_FEATURE_SOC_MCG_COUNT) && FSL_FEATURE_SOC_MCG_COUNT)
#if defined(kMCG_ModePEE)
    mcg_mode_t mode = CLOCK_GetMode();
#endif
#endif

    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);

    SMC_SetPowerModeVlps(SMC);

#if (defined(FSL_FEATURE_SOC_MCG_COUNT) && FSL_FEATURE_SOC_MCG_COUNT)
    /*
     * If enter stop modes when MCG in PEE mode, then after wakeup, the MCG is in PBE mode,
     * need to enter PEE mode manually.
     */
#if defined(kMCG_ModePEE)
    if (mode == kMCG_ModePEE) {
        BOARD_BootClockRUN();
    }
#endif
#endif
}
