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
#include "pinmap.h"
#include "fsl_port.h"
#include "PeripheralPins.h"

extern void serial_wait_tx_complete(uint32_t uart_index);

void hal_sleep(void)
{
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);

    SMC_SetPowerModeWait(SMC);
}

void hal_deepsleep(void)
{
    uint32_t pin_func;
#if (defined(FSL_FEATURE_SOC_MCG_COUNT) && FSL_FEATURE_SOC_MCG_COUNT)
#if (defined(FSL_FEATURE_MCG_HAS_PLL) && FSL_FEATURE_MCG_HAS_PLL)
    smc_power_state_t original_power_state;

    original_power_state = SMC_GetPowerModeState(SMC);
#endif // FSL_FEATURE_MCG_HAS_PLL
#endif // FSL_FEATURE_SOC_MCG_COUNT

    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);

    /* Wait till debug UART is done transmitting */
    serial_wait_tx_complete(STDIO_UART);

    /*
     * Set pin for current leakage.
     * Debug console RX pin: Set to pinmux to disable.
     * Debug console TX pin: Don't need to change.
     */
    pin_function(STDIO_UART_RX, (int)kPORT_PinDisabledOrAnalog);

    SMC_PreEnterStopModes();
    SMC_SetPowerModeVlps(SMC);
    SMC_PostExitStopModes();

    pin_func = pinmap_find_function(STDIO_UART_RX, PinMap_UART_RX);
    pin_function(STDIO_UART_RX, pin_func);

#if (defined(FSL_FEATURE_SOC_MCG_COUNT) && FSL_FEATURE_SOC_MCG_COUNT)
#if (defined(FSL_FEATURE_MCG_HAS_PLL) && FSL_FEATURE_MCG_HAS_PLL)
    /*
     * If enter stop modes when MCG in PEE mode, then after wakeup, the MCG is in PBE mode,
     * need to enter PEE mode manually.
     */
    if (original_power_state == kSMC_PowerStateRun) {
        /* Wait for PLL lock. */
        while (!(kMCG_Pll0LockFlag & CLOCK_GetStatusFlags()))
        {
        }
        CLOCK_SetPeeMode();
    }
#endif // FSL_FEATURE_MCG_HAS_PLL
#endif // FSL_FEATURE_SOC_MCG_COUNT
}
