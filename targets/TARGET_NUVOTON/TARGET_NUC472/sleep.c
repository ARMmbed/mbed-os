/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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
#include <stdbool.h>

#if DEVICE_SERIAL
bool serial_can_deep_sleep(void);
#endif

/**
 * Enter idle mode, in which just CPU is halted.
 */
void hal_sleep(void)
{
    SYS_UnlockReg();
    CLK_Idle();
    SYS_LockReg();
}

/**
 * Enter power-down mode, in which HXT/HIRC are halted.
 *
 * \note On NUC472, on wake-up from power-down mode, we may meet hard fault or
 *       some other unknown error. Before its cause is found, we enter idle mode
 *       instead for a workaround. To simulate power-down mode with idle mode, we
 *       also disable us_ticker during power-down period.
 */
#include "nu_modutil.h"
const struct nu_modinit_s *nu_us_ticker_modinit(void);
void hal_deepsleep(void)
{
#if DEVICE_SERIAL
    if (!serial_can_deep_sleep()) {
        return;
    }
#endif

    CLK_DisableModuleClock(nu_us_ticker_modinit()->clkidx);
    SYS_UnlockReg();
    CLK_Idle();
    SYS_LockReg();
    CLK_EnableModuleClock(nu_us_ticker_modinit()->clkidx);
}

#endif
