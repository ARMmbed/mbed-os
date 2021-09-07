/*
 * Copyright (c) 2020, Nuvoton Technology Corporation
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

#include "sleep_api.h"

#if DEVICE_SLEEP

#include "cmsis.h"
#include "device.h"
#include "objects.h"
#include "PeripheralPins.h"
#include "platform/mbed_toolchain.h"
#include <stdbool.h>

/* Merge SYS_UnlockReg_S()/CLK_Idle_S() or CLK_PowerDown_S()/SYS_LockReg_S()
 * into nu_idle_s() or nu_powerdown_s()
 *
 * These APIs are secure calls. For performance, merge them into one when
 * nu_idle_s() and nu_powerdown_s() are available.
 *
 * NOTE: MBED_WEAK gets unnecessary because nu_idle_s/nu_powerdown_s are always available
 *       since Mbed OS 6.
 * NOTE: With ARMCLANG, MBED_WEAK doesn't cause nu_idle_s/nu_powerdown_s to link in even though
 *       they are strong symbols. Add MBED_USED to ensure they are linked in.
 */
void nu_idle_s(void);
void nu_powerdown_s(void);

#if DEVICE_SERIAL
bool serial_can_deep_sleep(void);
#endif

/**
 * Enter idle mode, in which just CPU is halted.
 */
void hal_sleep(void)
{
    nu_idle_s();
}

/**
 * Enter power-down mode, in which HXT/HIRC are halted.
 */
void hal_deepsleep(void)
{
#if DEVICE_SERIAL
    if (!serial_can_deep_sleep()) {
        return;
    }
#endif

    nu_powerdown_s();
}

#endif
