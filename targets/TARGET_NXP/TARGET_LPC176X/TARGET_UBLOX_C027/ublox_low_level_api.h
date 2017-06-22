/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#ifndef UBLOX_LOW_LEVEL_H
#define UBLOX_LOW_LEVEL_H

#ifdef __cplusplus
extern "C" {
#endif

void ublox_mdm_init(void);

void ublox_mdm_power_on(int usb);
void ublox_mdm_powerOn(int usb);

void ublox_mdm_power_off(void);
void ublox_mdm_powerOff(void);

void ublox_gps_powerOn(void);

void ublox_gps_powerOff(void);

#ifdef __cplusplus
}
#endif

#endif // UBLOX_LOW_LEVEL_H
