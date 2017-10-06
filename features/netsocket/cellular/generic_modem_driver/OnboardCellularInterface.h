/* Copyright (c) 2017 ARM Limited
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

#ifndef ONBOARD_CELLULAR_INTERFACE_
#define ONBOARD_CELLULAR_INTERFACE_

#if MODEM_ON_BOARD && MODEM_ON_BOARD_UART && NSAPI_PPP_AVAILABLE

#include "UARTCellularInterface.h"

/** OnboardCellularInterface class
 *
 *  This interface serves as the controller/driver for an
 *  onboard modem implementing onboard_modem_api.h.
 *
 *  Depending on the type of on-board modem, OnboardCellularInterface
 *  could be derived from different implementation classes.
 *  Portable applications should only rely on it being a CellularBase.
 */
class OnboardCellularInterface : public UARTCellularInterface {

public:

    OnboardCellularInterface(bool debug = false);

    virtual ~OnboardCellularInterface();

protected:
    /** Sets the modem up for powering on
     *
     *  modem_init() is equivalent to plugging in the device, e.g., attaching power and serial port.
     *  Uses onboard_modem_api.h where the implementation of onboard_modem_api is provided by the target.
     */
    virtual void modem_init();

    /** Sets the modem in unplugged state
     *
     *  modem_deinit() will be equivalent to pulling the plug off of the device, i.e., detaching power
     *  and serial port. This puts the modem in lowest power state.
     *  Uses onboard_modem_api.h where the implementation of onboard_modem_api is provided by the target.
     */
    virtual void modem_deinit();

    /** Powers up the modem
     *
     *  modem_power_up() is equivalent to pressing the soft power button.
     *  The driver may repeat this if the modem is not responsive to AT commands.
     *  Uses onboard_modem_api.h where the implementation of onboard_modem_api is provided by the target.
     */
    virtual void modem_power_up();

    /** Powers down the modem
     *
     *  modem_power_down() is equivalent to turning off the modem by button press.
     *  Uses onboard_modem_api.h where the implementation of onboard_modem_api is provided by the target.
     */
    virtual void modem_power_down();
};

#endif //MODEM_ON_BOARD && MODEM_ON_BOARD_UART && NSAPI_PPP_AVAILABLE
#endif //ONBOARD_CELLULAR_INTERFACE_
