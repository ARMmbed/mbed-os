/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#ifndef AT_CELLULAR_DEVICE_H_
#define AT_CELLULAR_DEVICE_H_

#include "CellularDevice.h"

namespace mbed {

class ATHandler;
class AT_CellularInformation;
class AT_CellularNetwork;
class AT_CellularSMS;
class AT_CellularContext;

/**
 *  Class AT_CellularDevice
 *
 *  A class defines opening and closing of cellular interfaces.
 *  Deleting/Closing of opened interfaces can be done only through this class.
 */
class AT_CellularDevice : public CellularDevice {
public:
    AT_CellularDevice(FileHandle *fh);
    virtual ~AT_CellularDevice();

    virtual nsapi_error_t hard_power_on();

    virtual nsapi_error_t hard_power_off();

    virtual nsapi_error_t soft_power_on();

    virtual nsapi_error_t soft_power_off();

    virtual nsapi_error_t set_pin(const char *sim_pin);

    virtual nsapi_error_t get_sim_state(SimState &state);

    virtual CellularContext *create_context(FileHandle *fh = NULL, const char *apn = NULL, bool cp_req = false, bool nonip_req = false);

#if (DEVICE_SERIAL && DEVICE_INTERRUPTIN) || defined(DOXYGEN_ONLY)
    virtual CellularContext *create_context(UARTSerial *serial, const char *const apn, PinName dcd_pin = NC, bool active_high = false, bool cp_req = false, bool nonip_req = false);
#endif // #if DEVICE_SERIAL

    virtual void delete_context(CellularContext *context);

    virtual CellularNetwork *open_network(FileHandle *fh = NULL);

    virtual CellularSMS *open_sms(FileHandle *fh = NULL);

    virtual CellularInformation *open_information(FileHandle *fh = NULL);

    virtual void close_network();

    virtual void close_sms();

    virtual void close_information();

    virtual void set_timeout(int timeout);

    virtual uint16_t get_send_delay() const;

    virtual void modem_debug_on(bool on);

    virtual nsapi_error_t init();

    virtual nsapi_error_t shutdown();

    virtual nsapi_error_t is_ready();

    virtual void set_ready_cb(Callback<void()> callback);

    virtual nsapi_error_t set_power_save_mode(int periodic_time, int active_time = 0);


    virtual ATHandler *get_at_handler(FileHandle *fh);

    virtual ATHandler *get_at_handler();

    /** Releases the given at_handler. If last reference to at_hander then it's deleted.
     *
     *  @param at_handler
     *  @return NSAPI_ERROR_OK on success, NSAPI_ERROR_PARAMETER on failure
     */
    virtual nsapi_error_t release_at_handler(ATHandler *at_handler);

    /** Creates new instance of AT_CellularContext or if overridden, modem specific implementation.
     *
     *  @param at           ATHandler reference for communication with the modem.
     *  @param apn          access point to use with context
     *  @param cp_req       flag indicating if control plane EPS optimization needs to be setup
     *  @param nonip_req    flag indicating if PDP context needs to be Non-IP
     *  @return             new instance of class AT_CellularContext
     *
     */
    virtual AT_CellularContext *create_context_impl(ATHandler &at, const char *apn, bool cp_req = false, bool nonip_req = false);

    /** Create new instance of AT_CellularNetwork or if overridden, modem specific implementation.
     *
     *  @param at   ATHandler reference for communication with the modem.
     *  @return new instance of class AT_CellularNetwork
     */
    virtual AT_CellularNetwork *open_network_impl(ATHandler &at);

    /** Create new instance of AT_CellularSMS or if overridden, modem specific implementation.
     *
     *  @param at   ATHandler reference for communication with the modem.
     *  @return new instance of class AT_CellularSMS
     */
    virtual AT_CellularSMS *open_sms_impl(ATHandler &at);

    /** Create new instance of AT_CellularInformation or if overridden, modem specific implementation.
     *
     *  @param at   ATHandler reference for communication with the modem.
     *  @return new instance of class AT_CellularInformation
     */
    virtual AT_CellularInformation *open_information_impl(ATHandler &at);

    virtual CellularContext *get_context_list() const;

    AT_CellularNetwork *_network;
    AT_CellularSMS *_sms;
    AT_CellularInformation *_information;
    AT_CellularContext *_context_list;
    int _default_timeout;
    bool _modem_debug_on;
    ATHandler *_at;

protected:
    virtual void cellular_callback(nsapi_event_t ev, intptr_t ptr, CellularContext *ctx = NULL);
    void send_disconnect_to_context(int cid);
    // Sets commonly used URCs
    void set_at_urcs();
    // To be used for setting target specific URCs
    virtual void set_at_urcs_impl();
    // Sets up parameters for AT handler, for now only the send delay and URCs.
    // This kind of routine is needed for initialisation routines that are virtual and therefore cannot be called from constructor.
    void setup_at_handler();

private:
    void urc_nw_deact();
    void urc_pdn_deact();
};

} // namespace mbed
#endif // AT_CELLULAR_DEVICE_H_
