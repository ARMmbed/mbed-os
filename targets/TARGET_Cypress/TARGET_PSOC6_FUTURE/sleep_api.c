/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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
#include "device.h"
#include "cy_syspm.h"

#if DEVICE_SLEEP

void hal_sleep(void)
{
    Cy_SysPm_Sleep(CY_SYSPM_WAIT_FOR_INTERRUPT);
}

void hal_deepsleep(void)
{
#if DEVICE_LPTICKER
    if(CY_SYSPM_SUCCESS == Cy_SysPm_DeepSleep(CY_SYSPM_WAIT_FOR_INTERRUPT)) {
        // Have to make sure PLL clock is restored before continuing.
        // FLL clock is not used in basic configuration.
        while(!Cy_SysClk_PllLocked(1)) {
            // Just wait here.
        }
    }
#endif
}

#endif // DEVICE_SLEEP
