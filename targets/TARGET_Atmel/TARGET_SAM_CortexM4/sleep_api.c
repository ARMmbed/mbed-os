/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include "mbed_assert.h"
#include "sleep_api.h"
#include "sleepmgr.h"

/** Send the device to sleep
 *
 * The processor is setup ready for sleep, and sent to sleep using __WFI(). In this mode, the
 * system clock to the core is stopped until a reset or an interrupt occurs.
 * @param[void] void
 * @return      void
 */
void hal_sleep(void)
{
    enum sleepmgr_mode sleep_mode;

    sleep_mode = SLEEPMGR_SLEEP_WFI;
    sleepmgr_sleep(sleep_mode);

}
/** Send the device to deep sleep
 *
 * This processor is setup ready for deep sleep, and sent to sleep using __WFI(). This mode
 * has the same sleep features as sleep plus it powers down peripherals and clocks. All state
 * is still maintained.
 * @param[void] void
 * @return      void
 */
void hal_deepsleep(void)
{
    enum sleepmgr_mode sleep_mode;

    sleep_mode = SLEEPMGR_SLEEP_WFE;
    sleepmgr_sleep(sleep_mode);
}
