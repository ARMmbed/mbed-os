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

static void PreEnterStopModes(void)
{
    flash_prefetch_speculation_status_t speculationStatus =
    {
        kFLASH_prefetchSpeculationOptionDisable, /* Disable instruction speculation.*/
        kFLASH_prefetchSpeculationOptionDisable, /* Disable data speculation.*/
    };

    __ISB();

    /*
     * Before enter stop modes, the flash cache prefetch should be disabled.
     * Otherwise the prefetch might be interrupted by stop, then the data and
     * and instruction from flash are wrong.
     */
    FLASH_PflashSetPrefetchSpeculation(&speculationStatus);
}

static void PostExitStopModes(void)
{
    flash_prefetch_speculation_status_t speculationStatus =
    {
        kFLASH_prefetchSpeculationOptionEnable, /* Enable instruction speculation.*/
        kFLASH_prefetchSpeculationOptionEnable, /* Enable data speculation.*/
    };

    FLASH_PflashSetPrefetchSpeculation(&speculationStatus);

    __ISB();
}

void hal_deepsleep(void)
{
#if (defined(FSL_FEATURE_SOC_MCG_COUNT) && FSL_FEATURE_SOC_MCG_COUNT)
#if (defined(FSL_FEATURE_MCG_HAS_PLL) && FSL_FEATURE_MCG_HAS_PLL)
    mcg_mode_t mode = CLOCK_GetMode();
#endif // FSL_FEATURE_MCG_HAS_PLL
#endif // FSL_FEATURE_SOC_MCG_COUNT

    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);

    PreEnterStopModes();
    SMC_SetPowerModeVlps(SMC);
    PostExitStopModes();

#if (defined(FSL_FEATURE_SOC_MCG_COUNT) && FSL_FEATURE_SOC_MCG_COUNT)
#if (defined(FSL_FEATURE_MCG_HAS_PLL) && FSL_FEATURE_MCG_HAS_PLL)
    /*
     * If enter stop modes when MCG in PEE mode, then after wakeup, the MCG is in PBE mode,
     * need to enter PEE mode manually.
     */
    if (mode == kMCG_ModePEE) {
        BOARD_BootClockRUN();
    }
#endif // FSL_FEATURE_MCG_HAS_PLL
#endif // FSL_FEATURE_SOC_MCG_COUNT
}
