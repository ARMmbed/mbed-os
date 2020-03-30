/* mbed Microcontroller Library
 * Copyright (c) 2020 ARM Limited
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

#ifndef UBLOX_ONBOARD_MODEM_API_H_
#define UBLOX_ONBOARD_MODEM_API_H_

#ifdef __cplusplus
extern "C" {
#endif

/** Sets the modem up for powering on
 *  modem_init() will be equivalent to plugging in the device, i.e.,
 *  attaching power and serial port.
 */
void onboard_modem_init(void);

/** Sets the modem in unplugged state
 *  modem_deinit() will be equivalent to pulling the plug off of the device, i.e.,
 *  detaching power and serial port.
 *  This puts the modem in lowest power state.
 */
void onboard_modem_deinit(void);

/** Powers up the modem
 *  modem_power_up() will be equivalent to pressing the soft power button.
 *  The driver may repeat this if the modem is not responsive to AT commands.

 */
void onboard_modem_power_up(void);

/** Powers down the modem
 *  modem_power_down() will be equivalent to turning off the modem by button press.
 */
void onboard_modem_power_down(void);

#ifdef __cplusplus
}
#endif

#endif /* UBLOX_ONBOARD_MODEM_API_H_ */
