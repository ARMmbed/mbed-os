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

#include "CellularContext.h"
#ifdef CELLULAR_DEVICE
using namespace mbed;
MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use CellularBase::get_default_instance() instead.")
class OnboardCellularInterface : public CellularBase {
public:
    OnboardCellularInterface(bool debug = false)
    {
        context = CellularContext::get_default_instance();
        MBED_ASSERT(context != NULL);
        CellularDevice *dev = CellularDevice::get_default_instance();
        MBED_ASSERT(dev != NULL);
        dev->modem_debug_on(debug);
    }
public: // from NetworkInterface
    virtual nsapi_error_t set_blocking(bool blocking)
    {
        return context->set_blocking(blocking);
    }
    virtual NetworkStack *get_stack()
    {
        return context->get_stack();
    }
    virtual const char *get_ip_address()
    {
        return context->get_ip_address();
    }
    virtual void attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
    {
        context->attach(status_cb);
    }
    virtual nsapi_error_t connect()
    {
        return context->connect();
    }
    virtual nsapi_error_t disconnect()
    {
        return context->disconnect();
    }

    // from CellularBase
    virtual void set_plmn(const char *plmn)
    {
        context->set_plmn(plmn);
    }
    virtual void set_sim_pin(const char *sim_pin)
    {
        context->set_sim_pin(sim_pin);
    }
    virtual nsapi_error_t connect(const char *sim_pin, const char *apn = 0, const char *uname = 0,
                                  const char *pwd = 0)
    {
        return context->connect(sim_pin, apn, uname, pwd);
    }
    virtual void set_credentials(const char *apn, const char *uname = 0, const char *pwd = 0)
    {
        context->set_credentials(apn, uname, pwd);
    }
    virtual const char *get_netmask()
    {
        return context->get_netmask();
    }
    virtual const char *get_gateway()
    {
        return context->get_gateway();
    }
    virtual bool is_connected()
    {
        return context->is_connected();
    }

private:
    CellularContext *context;
};

#define ONBOARD_CELLULAR_INTERFACE_AVAILABLE
#elif MODEM_ON_BOARD && MODEM_ON_BOARD_UART && NSAPI_PPP_AVAILABLE

#include "UARTCellularInterface.h"

/** OnboardCellularInterface class
 *
 *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/framework/API/CellularPower.h instead.
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

    /** Constructor
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/framework/API/CellularPower.h instead.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/framework/API/CellularPower.h instead.")
    OnboardCellularInterface(bool debug = false);

    /** Destructor
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/framework/API/CellularPower.h instead.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/framework/API/CellularPower.h instead.")
    virtual ~OnboardCellularInterface();

protected:
    /** Sets the modem up for powering on
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/framework/API/CellularPower.h instead.
     *
     *  modem_init() is equivalent to plugging in the device, for example, attaching power and serial port.
     *  Uses onboard_modem_api.h where the target provides the implementation of onboard_modem_api.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/framework/API/CellularPower.h instead.")
    virtual void modem_init();

    /** Sets the modem in unplugged state
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/framework/API/CellularPower.h instead.
     *
     *  modem_deinit() will be equivalent to pulling the plug off of the device, in other words, detaching power
     *  and serial port. This puts the modem in lowest power state.
     *  Uses onboard_modem_api.h where the target provides the implementation of onboard_modem_api.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/framework/API/CellularPower.h instead.")
    virtual void modem_deinit();

    /** Powers up the modem
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/framework/API/CellularPower.h instead.
     *
     *  modem_power_up() is equivalent to pressing the soft power button.
     *  The driver may repeat this if the modem is not responsive to AT commands.
     *  Uses onboard_modem_api.h where the target provides the implementation of onboard_modem_api.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/framework/API/CellularPower.h instead.")
    virtual void modem_power_up();

    /** Powers down the modem
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/framework/API/CellularPower.h instead.
     *
     *  modem_power_down() is equivalent to turning off the modem by button press.
     *  Uses onboard_modem_api.h where the target provides the implementation of onboard_modem_api.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/framework/API/CellularPower.h instead.")
    virtual void modem_power_down();
};

#define ONBOARD_CELLULAR_INTERFACE_AVAILABLE

#endif //MODEM_ON_BOARD && MODEM_ON_BOARD_UART && NSAPI_PPP_AVAILABLE
#endif //ONBOARD_CELLULAR_INTERFACE_
