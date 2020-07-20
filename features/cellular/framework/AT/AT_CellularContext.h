/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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
#ifndef AT_CELLULARCONTEXT_H_
#define AT_CELLULARCONTEXT_H_

#include "CellularContext.h"
#include "ATHandler.h"
#include "rtos/Semaphore.h"
#include "AT_CellularDevice.h"

const int MAX_APN_LENGTH = 63 + 1;

namespace mbed {

class AT_CellularContext : public CellularContext {
public:
    AT_CellularContext(ATHandler &at, CellularDevice *device, const char *apn = 0, bool cp_req = false, bool nonip_req = false);
    virtual ~AT_CellularContext();

// from CellularInterface/NetworkInterface
    virtual nsapi_error_t set_blocking(bool blocking);
    virtual NetworkStack *get_stack();
    virtual nsapi_error_t get_ip_address(SocketAddress *address);
    virtual char *get_interface_name(char *interface_name);
    virtual void attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb);
    virtual nsapi_error_t connect();
    virtual nsapi_error_t disconnect();
    virtual nsapi_connection_status_t get_connection_status() const;
    virtual bool is_connected();
    // from CellularInterface
    virtual void set_plmn(const char *plmn);
    virtual void set_sim_pin(const char *sim_pin);
    virtual nsapi_error_t connect(const char *sim_pin, const char *apn = 0, const char *uname = 0,
                                  const char *pwd = 0);
    virtual void set_credentials(const char *apn, const char *uname = 0, const char *pwd = 0);

// from CellularContext
    virtual nsapi_error_t get_pdpcontext_params(pdpContextList_t &params_list);
    virtual nsapi_error_t get_rate_control(CellularContext::RateControlExceptionReports &reports,
                                           CellularContext::RateControlUplinkTimeUnit &time_unit, int &uplink_rate);
    virtual nsapi_error_t get_apn_backoff_timer(int &backoff_timer);
    virtual nsapi_error_t set_device_ready();
    virtual nsapi_error_t set_sim_ready();
    virtual nsapi_error_t register_to_network();
    virtual nsapi_error_t attach_to_network();
#if (DEVICE_SERIAL && DEVICE_INTERRUPTIN) || defined(DOXYGEN_ONLY)
    virtual nsapi_error_t configure_hup(PinName dcd_pin = NC, bool active_high = false);
#endif // #if DEVICE_SERIAL

    virtual ControlPlane_netif *get_cp_netif();

    AT_CellularDevice *get_device() const;

protected:
    virtual void enable_hup(bool enable);

    virtual void cellular_callback(nsapi_event_t ev, intptr_t ptr);

    /** Does the authentication for the PDP Context if user name and password are provided.
     *  Can be overridden by the modem target if 3GPP default implementation if not an option
     *
     *  @return NSAPI_ERROR_OK if no credentials provided or authentication was successful
     *          NSAPI_ERROR_AUTH_FAILURE if authentication failed
     *          NSAPI_ERROR_DEVICE_ERROR if communication with the modemm failed
     */
    virtual nsapi_error_t do_user_authentication();

    /** Activates PDP context or in PPP mode opens data channel.
     *  Can be overridden by the modem target if 3GPP default implementation if not an option
     */
    virtual void do_connect();

    virtual void do_disconnect();

    /** Get the operation specific timeout. Used in synchronous mode when setting the maximum
     *   waiting time. Modem specific implementation can override this to provide different timeouts.
     *
     *  @param  op  current operation
     *  @return     timeout in milliseconds
     */
    virtual uint32_t get_timeout_for_operation(ContextOperation op) const;

    virtual nsapi_error_t activate_non_ip_context();
    virtual nsapi_error_t setup_control_plane_opt();
    virtual void deactivate_non_ip_context();
    virtual void deactivate_ip_context();
    virtual void set_disconnect();
    virtual void deactivate_context();
    virtual bool get_context();
    AT_CellularDevice::CellularProperty pdp_type_t_to_cellular_property(pdp_type_t pdp_type);
    bool set_new_context(int cid);
    /** Get string name for NIDD context type.
     *  @return     NIDD context text, e.g. Non-IP or NONIP
     */
    virtual const char *get_nonip_context_type_str();

private:
#if NSAPI_PPP_AVAILABLE
    nsapi_error_t open_data_channel();
    void ppp_status_cb(nsapi_event_t ev, intptr_t ptr);
    void ppp_disconnected();
#endif // #if NSAPI_PPP_AVAILABLE
    nsapi_error_t do_activate_context();
    virtual void activate_context();
    nsapi_error_t find_and_activate_context();
    nsapi_error_t activate_ip_context();
    void check_and_deactivate_context();
    void delete_current_context();
    nsapi_error_t check_operation(nsapi_error_t err, ContextOperation op);
    void ciot_opt_cb(mbed::CellularNetwork::CIoT_Supported_Opt ciot_opt);
    virtual void do_connect_with_retry();
    void set_cid(int cid);

private:
    ContextOperation _current_op;
    rtos::Semaphore _semaphore;
    rtos::Semaphore _cp_opt_semaphore;

    PinName _dcd_pin;
    bool _active_high;

protected:
    char _found_apn[MAX_APN_LENGTH];
    // flag indicating if CP was requested to be setup
    bool _cp_req;
    bool _is_connected;
    ATHandler &_at;
};

} // namespace mbed

#endif // AT_CELLULARCONTEXT_H_
