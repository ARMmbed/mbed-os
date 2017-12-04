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

#ifndef ONBOARD_MODEM_API_H_
#define ONBOARD_MODEM_API_H_

/** onboard_modem_api is a standardizing API for Modem type devices under mbed-os.
 * It provides a simple hardware abstraction layer on top of the modem drivers
 * written for mbed-os.
 *
 * It is required from the engineers porting any modem type device (e.g., Cellular)
 * to provide an implementation of this API in their respective target folder as well as
 * usage of standard PinNames (in PinNames.h) is required. For example,
 *
 *   MDMTXD = P0_15,    // Transmit Data
 *   MDMRXD = P0_16,    // Receive Data
 *   MDMCTS = P0_17,    // Clear to Send
 *   MDMDCD = P0_18,    // Data Carrier Detect
 *   MDMDSR = P0_19,    // Data Set Ready
 *   MDMDTR = P0_20,    // Data Terminal Ready (set high or use handshake)
 *   MDMRI  = P0_21,    // Ring Indicator
 *   MDMRTS = P0_22,    // Request to Send (set high or use handshake)
 *
 *   MDM_PIN_POLARITY must also be defined as 0 (active low) or 1 (active high).
 *
 *   NOTE: This API should only be used when the modem exists on-board, i.e., the modem is
 *         NOT a plugged-in component.
 */

#if MODEM_ON_BOARD

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

#endif /* MODEM_ON_BOARD*/
#endif /* ONBOARD_MODEM_API_H_ */
