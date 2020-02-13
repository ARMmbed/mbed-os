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
#include "ATHandler.h"

namespace mbed {

class AT_CellularInformation;
class AT_CellularNetwork;
class AT_CellularSMS;
class AT_CellularContext;
class FileHandle;

/**
 *  Class AT_CellularDevice
 *
 *  A class defines opening and closing of cellular interfaces.
 *  Deleting/Closing of opened interfaces can be done only through this class.
 */
class AT_CellularDevice : public CellularDevice {
public:
    /* Supported features by the modem
     *
     * NOTE! These are used as index to feature table, so the only allowed modification to this is appending
     *       to the end (just before PROPERTY_MAX). Do not modify any of the existing fields.
     */
    enum CellularProperty {
        PROPERTY_C_EREG,                // AT_CellularNetwork::RegistrationMode. What support modem has for this registration type.
        PROPERTY_C_GREG,                // AT_CellularNetwork::RegistrationMode. What support modem has for this registration type.
        PROPERTY_C_REG,                 // AT_CellularNetwork::RegistrationMode. What support modem has for this registration type.
        PROPERTY_AT_CGSN_WITH_TYPE,     // 0 = not supported, 1 = supported. AT+CGSN without type is likely always supported similar to AT+GSN.
        PROPERTY_AT_CGDATA,             // 0 = not supported, 1 = supported. Alternative is to support only ATD*99***<cid>#
        PROPERTY_AT_CGAUTH,             // 0 = not supported, 1 = supported. APN authentication AT commands supported
        PROPERTY_AT_CNMI,               // 0 = not supported, 1 = supported. New message (SMS) indication AT command
        PROPERTY_AT_CSMP,               // 0 = not supported, 1 = supported. Set text mode AT command
        PROPERTY_AT_CMGF,               // 0 = not supported, 1 = supported. Set preferred message format AT command
        PROPERTY_AT_CSDH,               // 0 = not supported, 1 = supported. Show text mode AT command
        PROPERTY_IPV4_PDP_TYPE,         // 0 = not supported, 1 = supported. Does modem support IPV4?
        PROPERTY_IPV6_PDP_TYPE,         // 0 = not supported, 1 = supported. Does modem support IPV6?
        PROPERTY_IPV4V6_PDP_TYPE,       // 0 = not supported, 1 = supported. Does modem support IPV4 and IPV6 simultaneously?
        PROPERTY_NON_IP_PDP_TYPE,       // 0 = not supported, 1 = supported. Does modem support Non-IP?
        PROPERTY_AT_CGEREP,             // 0 = not supported, 1 = supported. Does modem support AT command AT+CGEREP.
        PROPERTY_AT_COPS_FALLBACK_AUTO, // 0 = not supported, 1 = supported. Does modem support mode 4 of AT+COPS= ?
        PROPERTY_SOCKET_COUNT,          // The number of sockets of modem IP stack
        PROPERTY_IP_TCP,                // 0 = not supported, 1 = supported. Modem IP stack has support for TCP
        PROPERTY_IP_UDP,                // 0 = not supported, 1 = supported. Modem IP stack has support for TCP
        PROPERTY_AT_SEND_DELAY,         // Sending delay between AT commands in ms
        PROPERTY_MAX
    };

public:
    AT_CellularDevice(FileHandle *fh);
    virtual ~AT_CellularDevice();

    virtual nsapi_error_t clear();

    virtual nsapi_error_t hard_power_on();

    virtual nsapi_error_t hard_power_off();

    virtual nsapi_error_t soft_power_on();

    virtual nsapi_error_t soft_power_off();

    virtual nsapi_error_t set_pin(const char *sim_pin);

    virtual nsapi_error_t get_sim_state(SimState &state);

    virtual CellularContext *create_context(const char *apn = NULL, bool cp_req = false, bool nonip_req = false);

    virtual void delete_context(CellularContext *context);

    virtual CellularNetwork *open_network();

    virtual CellularInformation *open_information();

    virtual void close_network();

    virtual void close_information();

    virtual void set_timeout(int timeout);

    virtual void modem_debug_on(bool on);

    virtual nsapi_error_t init();

    virtual nsapi_error_t shutdown();

    virtual nsapi_error_t is_ready();

    virtual void set_ready_cb(Callback<void()> callback);

    virtual nsapi_error_t set_power_save_mode(int periodic_time, int active_time = 0);

    virtual ATHandler *get_at_handler();

    virtual CellularContext *get_context_list() const;

    virtual nsapi_error_t set_baud_rate(int baud_rate);

#if MBED_CONF_CELLULAR_USE_SMS
    virtual CellularSMS *open_sms();

    virtual void close_sms();
#endif

    /** Get value for the given key.
     *
     *  @param key  key for value to be fetched
     *  @return     property value for the given key. Value type is defined in enum CellularProperty
     */
    intptr_t get_property(CellularProperty key);

    /** Cellular module need to define an array of cellular properties which defines module supported property values.
     *
     *  @param property_array array of module properties
     */
    void set_cellular_properties(const intptr_t *property_array);

protected:
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

    /** Create new instance of AT_CellularInformation or if overridden, modem specific implementation.
     *
     *  @param at   ATHandler reference for communication with the modem.
     *  @return new instance of class AT_CellularInformation
     */
    virtual AT_CellularInformation *open_information_impl(ATHandler &at);

#if MBED_CONF_CELLULAR_USE_SMS
    /** Create new instance of AT_CellularSMS or if overridden, modem specific implementation.
     *
     *  @param at   ATHandler reference for communication with the modem.
     *  @return new instance of class AT_CellularSMS
     */
    virtual AT_CellularSMS *open_sms_impl(ATHandler &at);
#endif // MBED_CONF_CELLULAR_USE_SMS

    virtual void cellular_callback(nsapi_event_t ev, intptr_t ptr, CellularContext *ctx = NULL);
    void send_disconnect_to_context(int cid);
    // Sets commonly used URCs
    void set_at_urcs();
    // To be used for setting target specific URCs
    virtual void set_at_urcs_impl();
    // Sets up parameters for AT handler, for now only the send delay and URCs.
    // This kind of routine is needed for initialisation routines that are virtual and therefore cannot be called from constructor.
    void setup_at_handler();
    virtual nsapi_error_t set_baud_rate_impl(int baud_rate);

private:
    void urc_nw_deact();
    void urc_pdn_deact();

protected:
    ATHandler _at;

private:
#if MBED_CONF_CELLULAR_USE_SMS
    AT_CellularSMS *_sms;
#endif // MBED_CONF_CELLULAR_USE_SMS

    AT_CellularNetwork *_network;
    AT_CellularInformation *_information;
    AT_CellularContext *_context_list;

    std::chrono::duration<int, std::milli> _default_timeout;
    bool _modem_debug_on;
    const intptr_t *_property_array;
};

} // namespace mbed
#endif // AT_CELLULAR_DEVICE_H_
