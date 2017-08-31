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

// number of warm-up cycle = warm-up time to set / input frequency cycle (s)
// number of 3*10^-6 (s) / (1/12 (MHz)) = 60000 = 0xea60
#define CG_WUODR_INT     ((uint16_t)0xea60)

void external_losc_enable(void);

void hal_sleep(void)
{
    // Set low power consumption mode IDLE
    CG_SetSTBYMode(CG_STBY_MODE_IDLE);
    // Enter idle mode
    __WFI();
}

void hal_deepsleep(void)
{
    // Set low power consumption mode STOP1
    CG_SetSTBYMode(CG_STBY_MODE_STOP1);
    // Setup warm up time
    CG_SetWarmUpTime(CG_WARM_UP_SRC_OSC_EXT_HIGH, CG_WUODR_INT);
    // Enter stop1 mode
    __WFI();
    // Switch over from IHOSC to EHOSC
    external_losc_enable();
}

void external_losc_enable(void)
{
    // Enable high-speed oscillator
    CG_SetFoscSrc(CG_FOSC_OSC_EXT);
    // Select internal(fIHOSC) as warm-up clock
    CG_SetWarmUpTime(CG_WARM_UP_SRC_OSC_INT_HIGH, CG_WUODR_INT);
    // Start warm-up
    CG_StartWarmUp();
    // Wait until EHOSC become stable
    while (CG_GetWarmUpState() != DONE) {
        // Do nothing
    }

    // Set fosc source
    CG_SetFoscSrc(CG_FOSC_OSC_EXT);
    // Wait for <OSCSEL> to become "1"
    while (CG_GetFoscSrc() != CG_FOSC_OSC_EXT) {
        // Do nothing
    }

    // Stop IHOSC
    CG_SetFoscSrc(CG_FOSC_OSC_INT);
}
