/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
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

void hal_sleep(void)
{
    // Set low power consumption mode IDLE
    CG_SetSTBYMode(CG_STBY_MODE_IDLE);
    __DSB();
    // Enter idle mode
    __WFI();
}

void hal_deepsleep(void)
{
    // deepsleep = sleep because, TMPM46BF10FG does not support the low power
    // consumption mode configured with the SLEEPDEEP bit in the Cortex-M4 core.
    // Setting the bit of the system control register is prohibited.
    hal_sleep();
}
