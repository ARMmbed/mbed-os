/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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
#ifndef RIL_CELLULARDEVICE_H_
#define RIL_CELLULARDEVICE_H_

#include "CellularDevice.h"
#include "RIL_CellularBase.h"
#include "Semaphore.h"

namespace mbed {

class RILAdaptation;
class RIL_CellularContext;
class RIL_CellularNetwork;
class RIL_CellularInformation;

class RIL_CellularDevice: public CellularDevice, public RIL_CellularBase {
public:
    RIL_CellularDevice();
    virtual ~RIL_CellularDevice();

    // Cellular properties. Defines what features/technologies are supported/used by the module
    enum CellularProperty {
        PROPERTY_C_EREG,            // 0 = not supported, 1 = supported. Does modem support this registration mode?
        PROPERTY_C_GREG,            // 0 = not supported, 1 = supported. Does modem support this registration mode?
        PROPERTY_C_REG,             // 0 = not supported, 1 = supported. Does modem support this registration mode?
        PROPERTY_IPV4_PDP_TYPE,     // 0 = not supported, 1 = supported. Does modem support IPV4?
        PROPERTY_IPV6_PDP_TYPE,     // 0 = not supported, 1 = supported. Does modem support IPV6?
        PROPERTY_IPV4V6_PDP_TYPE,   // 0 = not supported, 1 = supported. Does modem support dual stack IPV4V6?
        PROPERTY_NON_IP_PDP_TYPE,   // 0 = not supported, 1 = supported. Does modem support Non-IP?
        PROPERTY_MAX
    };

    intptr_t get_property(CellularProperty key);

    // from CellularDevice
    virtual void register_sleep_cb(int type, void (*cb)(void));

    virtual nsapi_error_t hard_power_on();

    virtual nsapi_error_t hard_power_off();

    virtual nsapi_error_t soft_power_on();

    virtual nsapi_error_t soft_power_off();

    virtual nsapi_error_t set_pin(const char *sim_pin);

    virtual nsapi_error_t get_sim_state(SimState &state);

    virtual CellularContext *create_context(const char *apn = NULL, bool cp_req = false,
                                            bool nonip_req = false);

    virtual void delete_context(CellularContext *context);

    virtual CellularNetwork *open_network();

    virtual CellularSMS *open_sms();

    virtual CellularInformation *open_information();

    virtual void close_network();

    virtual void close_sms();

    virtual void close_information();

    virtual void set_timeout(int timeout);

    virtual void modem_debug_on(bool on);

    virtual nsapi_error_t init();

    virtual nsapi_error_t is_ready();

    virtual void set_ready_cb(Callback<void()> callback);

    virtual nsapi_error_t set_power_save_mode(int periodic_time, int active_time = 0);

    virtual ATHandler *get_at_handler();

    virtual nsapi_error_t release_at_handler(ATHandler *at_handler);

    virtual CellularContext *get_context_list() const;

    virtual nsapi_error_t send_at_command(char *data, size_t data_len);
    virtual nsapi_error_t set_baud_rate(int baud_rate);
    virtual nsapi_error_t clear();
protected:
    virtual RIL_CellularContext *create_context_impl(const char *apn, bool cp_req = false, bool nonip_req = false);
    virtual RIL_CellularNetwork *open_network_impl();
    virtual RIL_CellularInformation *open_information_impl();
    virtual void unsolicited_response(int response_id, const void *data, size_t data_len);

    const intptr_t *_property_array;
    int _data;
    rtos::Mutex _api_mutex;

private:
    friend class RILAdaptation;
    void request_ack(ril_token_t *token);
    void request_timed_callback(RIL_TimedCallback callback, void *param, const struct timeval *relative_time);
    // helper methods
    void get_radiostatus(SimState *state = NULL, bool unsolicited_resp = false);
    void sim_status_response(ril_token_t *token, RIL_Errno err, void *response, size_t response_len);
    void common_response(ril_token_t *token, RIL_Errno err, void *response, size_t response_len);
    void check_registration_state();

    RILAdaptation *_ril;
    RIL_CellularNetwork *_network;
    RIL_CellularInformation *_information;
    RIL_CellularContext *_context_list;
    int _registation_status;

    bool _device_ready;
    Callback<void()> _device_ready_cb;
};

} /* namespace mbed */

#endif /* RIL_CELLULARDEVICE_H_ */
