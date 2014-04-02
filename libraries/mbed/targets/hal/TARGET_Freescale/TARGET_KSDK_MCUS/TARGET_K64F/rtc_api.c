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
#include "rtc_api.h"
#include "pinmap.h"
#include "fsl_rtc_hal.h"
#include "fsl_clock_manager.h"

const PinMap PinMap_RTC[] = {
    {NC, OSC32KCLK, 0},
};

void rtc_init(void) {
    rtc_hal_init_config_t hal_config = {0};

    hal_config.disableClockOutToPeripheral = true;
    if (PinMap_RTC[0].pin == NC) {
        hal_config.enable32kOscillator = true;
    }
    clock_manager_set_gate(kClockModuleRTC, 0U, true);
    hal_config.startSecondsCounterAt = 1; /* TSR = 1 */
    rtc_hal_init(&hal_config);

    // select RTC clock source
    SIM->SOPT1 &= ~SIM_SOPT1_OSC32KSEL_MASK;
    SIM->SOPT1 |= SIM_SOPT1_OSC32KSEL(PinMap_RTC[0].peripheral);

    rtc_hal_config_oscillator(true);
    rtc_hal_counter_enable(true);
}

void rtc_free(void) {
    // [TODO]
}

/*
 * Little check routine to see if the RTC has been enabled
 * 0 = Disabled, 1 = Enabled
 */
int rtc_isenabled(void) {
    clock_manager_set_gate(kClockModuleRTC, 0U, true);
    return (int)rtc_hal_is_counter_enabled();
}

time_t rtc_read(void) {
    return BR_RTC_TSR_TSR;
}

void rtc_write(time_t t) {
    if (t == 0) {
        t = 1;
    }
    rtc_hal_counter_enable(false);
    BW_RTC_TSR_TSR(t);
    rtc_hal_counter_enable(true);
}
