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
#ifndef RIL_CELLULARCONTEXT_H_
#define RIL_CELLULARCONTEXT_H_

#include "CellularContext.h"
#include "OnboardNetworkStack.h"
#include "RIL_CellularDevice.h"
#include "RIL_CellularBase.h"
class L3IP;

namespace mbed {

#define MAX_IPV6_SIZE 63+1
class RIL_CellularDevice;

class RIL_CellularContext: public CellularContext, public RIL_CellularBase {
public:
    RIL_CellularContext(RIL_CellularDevice &device, const char *apn = 0, bool cp_req = false, bool nonip_req = false);
    virtual ~RIL_CellularContext();

    // from CellularBase/NetworkInterface
    virtual nsapi_error_t set_blocking(bool blocking);
    virtual NetworkStack *get_stack();
    virtual nsapi_error_t get_ip_address(SocketAddress *address);
    virtual char *get_interface_name(char *interface_name);
    virtual void attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb);
    virtual nsapi_error_t connect();
    virtual nsapi_error_t disconnect();
    virtual bool is_connected();
    // from CellularBase
    virtual void set_plmn(const char *plmn);
    virtual void set_sim_pin(const char *sim_pin);
    virtual nsapi_error_t connect(const char *sim_pin, const char *apn = 0, const char *uname = 0,
                                  const char *pwd = 0);
    virtual void set_credentials(const char *apn, const char *uname = 0, const char *pwd = 0);
    virtual const char *get_netmask();
    virtual const char *get_gateway();
    virtual nsapi_connection_status_t get_connection_status() const;

    // from CellularContext
    virtual nsapi_error_t get_pdpcontext_params(pdpContextList_t &params_list);
    virtual nsapi_error_t get_rate_control(CellularContext::RateControlExceptionReports &reports,
                                           CellularContext::RateControlUplinkTimeUnit &time_unit, int &uplink_rate);
    virtual nsapi_error_t get_apn_backoff_timer(int &backoff_timer);
    virtual nsapi_error_t set_device_ready();
    virtual nsapi_error_t set_sim_ready();
    virtual nsapi_error_t register_to_network();
    virtual nsapi_error_t attach_to_network();
    virtual void set_file_handle(FileHandle *fh);
    virtual ControlPlane_netif *get_cp_netif();
    virtual CellularDevice *get_device() const;
    virtual nsapi_error_t configure_hup(PinName dcd_pin, bool active_high);

    virtual const char *get_nonip_context_type_str();

    static const char *pdp_type_to_string(pdp_type_t pdp_type);
    static RIL_CellularDevice::CellularProperty pdp_type_t_to_cellular_property(pdp_type_t pdp_type);
    static nsapi_ip_stack_t pdp_type_to_ip_stack(pdp_type_t pdp_type);

protected:
    // from CellularContext
    virtual void cellular_callback(nsapi_event_t ev, intptr_t ptr);
    virtual void enable_hup(bool enable);
    virtual void create_interface_and_connect();
    virtual void do_connect();
    virtual L3IP *get_L3IP_driver();
    virtual void delete_L3IP_driver();

    /** Get the operation specific timeout. Used in synchronous mode when setting the maximum
     *   waiting time. Modem specific implementation can override this to provide different timeouts.
     *
     *  @param  op  current operation
     *  @return     timeout in milliseconds
     */
    virtual uint32_t get_timeout_for_operation(ContextOperation op) const;

    friend class RIL_CellularDevice;
    void unsolicited_response(int response_id, const void *data, size_t data_len);

    nsapi_error_t disconnect(bool request_deactivate_data_call);

    L3IP *_l3ip_driver;
    char *_ifname;
    int _mtu;

private:

    struct data_call_addr_data_t {
        char _local_addr_ipv4[NSAPI_IPv4_SIZE];
        char _local_addr_ipv6[NSAPI_IPv6_SIZE];
        char _gateway_addr_ipv4[NSAPI_IPv4_SIZE];
        char _gateway_addr_ipv6[NSAPI_IPv6_SIZE];
        char _primary_dns_addr_ipv4[NSAPI_IPv4_SIZE];
        char _secondary_dns_addr_ipv4[NSAPI_IPv4_SIZE];
        char _primary_dns_addr_ipv6[NSAPI_IPv6_SIZE];
        char _secondary_dns_addr_ipv6[NSAPI_IPv6_SIZE];

        data_call_addr_data_t()
        {
            memset(_local_addr_ipv4, 0, sizeof(_local_addr_ipv4));
            memset(_local_addr_ipv6, 0, sizeof(_local_addr_ipv6));
            memset(_gateway_addr_ipv4, 0, sizeof(_gateway_addr_ipv4));
            memset(_gateway_addr_ipv6, 0, sizeof(_gateway_addr_ipv6));
            memset(_primary_dns_addr_ipv4, 0, sizeof(_primary_dns_addr_ipv4));
            memset(_secondary_dns_addr_ipv4, 0, sizeof(_secondary_dns_addr_ipv4));
            memset(_primary_dns_addr_ipv6, 0, sizeof(_primary_dns_addr_ipv6));
            memset(_secondary_dns_addr_ipv6, 0, sizeof(_secondary_dns_addr_ipv6));
        }
    };

    struct attach_req_t {
        const char *data[6];
        const char *apn;
        const char *pdp_type;
        char auth[2];
        const char *uname;
        const char *pwd;
        const char *padding3;
    };

    struct connect_req_t {
        const char *data[8];
        char rat[3];
        char data_profile[11];
        const char *apn;
        const char *uname;
        const char *pwd;
        char auth[2];
        const char *pdp_type;
        char *opt;
    };

    struct disconnect_ret_t {
        const char *data[2];
        char cid[5];
        char reason[2];
    };

    // internal state for connect. Needed to specify what states are done successfully
    // when running retry logic.
    enum Connect_state {
        CS_INVALID = -1,
        CS_INITIAL_ATTACH,
        CS_ATTACH,
        CS_SETUP_DATA_CALL,
        CS_INTERFACE_ADDED
    };

    /** Helper method to call callback function if it is provided
     *
     *  @param status connection status which is parameter in callback function
     */
    void call_network_cb(nsapi_connection_status_t status);
    void stack_status_cb(nsapi_event_t ev, intptr_t ptr);
    nsapi_error_t check_operation(nsapi_error_t err, ContextOperation current_op);
    void request_setup_data_call_response(ril_token_t *token, RIL_Errno err, void *response, size_t response_len);
    void request_deactivate_data_call_response(ril_token_t *token, RIL_Errno err, void *response, size_t response_len);
    void set_state(cell_callback_data_t *data, cellular_connection_status_t st);
    void do_connect_with_retry();
    void do_initial_attach();
    void request_set_initial_attach_apn_response(ril_token_t *token, RIL_Errno err, void *response, size_t response_len);
    nsapi_error_t close_stack_interface();
    nsapi_error_t add_dns_server_to_stack(const char *dns_server_address);
    nsapi_error_t handle_data_call_setup_addresses(const RIL_Data_Call_Response_v11 *response);
    void get_signal_quality();

private:
    bool _is_connected;
    bool _is_running;
    ContextOperation  _current_op;
    ContextOperation  _final_op;
    ContextOperation  _current_state;
    rtos::Semaphore _semaphore;
    OnboardNetworkStack::Interface *_stack_interface;
    rtos::Mutex _api_mutex;
    // used in connect after device is attached for retry logic
    Connect_state _connect_state;
    bool _set_registration_complete;
    data_call_addr_data_t *_data_call_addresses;
    cell_signal_quality_t _signal_quality;
};

} /* namespace mbed */

#endif /* RIL_CELLULARCONTEXT_H_ */
