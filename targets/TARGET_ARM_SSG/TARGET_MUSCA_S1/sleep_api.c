/* mbed Microcontroller Library
 * Copyright (c) 2019-2020 Arm Limited
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

#include "device_cfg.h"
#include "sleep_api.h"
#include "timer_cmsdk_drv.h"

#if DEVICE_SLEEP

void hal_sleep(void)
{
    __WFI();
}

/* Since there is no power management implemented in Musca, Deep Sleep could
 * be supported only by additional software components, registering and managing
 * the currently configured IPs. This would also mean a huge implementation
 * overhead, that is not intended to be added. Therefore, Deep Sleep is almost
 * identical to Sleep, representing a "Waiting For Interrupt" state, and
 * disabling the Microsec ticker in addition.
 */
void hal_deepsleep(void)
{
#ifdef USEC_TIMER_DEV
    timer_cmsdk_disable(&USEC_TIMER_DEV);
#endif
    __WFI();
#ifdef USEC_TIMER_DEV
    timer_cmsdk_enable(&USEC_TIMER_DEV);
#endif
}

#endif
