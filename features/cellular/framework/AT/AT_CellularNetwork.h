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

#ifndef AT_CELLULAR_NETWORK_H_
#define AT_CELLULAR_NETWORK_H_

#include "CellularNetwork.h"
#include "AT_CellularBase.h"
#include "NetworkStack.h"

namespace mbed {

#define AT_NETWORK_TRIALS 5

/**
 *  Class AT_CellularNetwork
 *
 *  Class for connecting to a network and getting information from it.
 */
class AT_CellularNetwork : public CellularNetwork, public AT_CellularBase
{

public:

    AT_CellularNetwork(ATHandler &atHandler);
    virtual ~AT_CellularNetwork();
    // declare friend so it can access stack
    friend class AT_CellularDevice;

public: // NetworkInterface

    virtual nsapi_error_t set_credentials(const char *apn,
                                          const char *username = 0, const char *password = 0);

    virtual nsapi_error_t set_credentials(const char *apn, AuthenticationType type,
                const char *username = 0, const char *password = 0);

    virtual nsapi_error_t connect(const char *apn,
                                  const char *username = 0, const char *password = 0);

    virtual nsapi_error_t connect();

    virtual nsapi_error_t disconnect();

protected:
    virtual NetworkStack *get_stack();

public: // CellularNetwork
    virtual nsapi_error_t set_registration(const char *plmn = 0);

    virtual nsapi_error_t get_registration_status(RegistrationType type, RegistrationStatus &status);

    virtual nsapi_error_t set_attach(int timeout = 10*1000);

    virtual nsapi_error_t get_attach(AttachStatus &status);

    virtual nsapi_error_t get_rate_control(CellularNetwork::RateControlExceptionReports &reports,
        CellularNetwork::RateControlUplinkTimeUnit &time_unit, int &uplink_rate);

    virtual nsapi_error_t get_apn_backoff_timer(int &backoff_timer);

    virtual void attach(Callback<void(nsapi_event_t, intptr_t)> status_cb);

    virtual nsapi_connection_status_t get_connection_status() const;

    virtual nsapi_error_t set_blocking(bool blocking);

    virtual const char *get_ip_address();

    virtual nsapi_error_t set_access_technology(operator_t::RadioAccessTechnology op_rat);

    virtual nsapi_error_t scan_plmn(operList_t &operators, int &ops_count);

    virtual nsapi_error_t set_ciot_optimization_config(Supported_UE_Opt supported_opt,
                                                       Preferred_UE_Opt preferred_opt);

    virtual nsapi_error_t get_ciot_optimization_config(Supported_UE_Opt& supported_opt,
                                                       Preferred_UE_Opt& preferred_opt);

    virtual nsapi_error_t set_stack_type(nsapi_ip_stack_t stack_type);

    virtual nsapi_ip_stack_t get_stack_type();

    virtual nsapi_error_t get_pdpcontext_params(pdpContextList_t& params_list);

    virtual nsapi_error_t get_extended_signal_quality(int &rxlev, int &ber, int &rscp, int &ecno, int &rsrq, int &rsrp);

    virtual nsapi_error_t get_signal_quality(int &rssi, int &ber);

    virtual nsapi_error_t get_cell_id(int &cell_id);

    virtual int get_3gpp_error();

    virtual nsapi_error_t get_operator_params(int &format, operator_t &operator_params);

protected:

    /** Check if modem supports the given stack type.
     *
     *  @return true if supported
     */
    virtual bool get_modem_stack_type(nsapi_ip_stack_t requested_stack);

    /** Check if modem supports given registration type.
     *
     *  @param reg_type enum RegistrationType
     *  @return         true if given registration type is supported by modem
     */
    virtual bool has_registration(RegistrationType reg_type);

    /** Sets access technology to be scanned. Modem specific implementation.
     *
     *  @param op_rat Access technology
     *
     *  @return       zero on success
     */
    virtual nsapi_error_t set_access_technology_impl(operator_t::RadioAccessTechnology op_rat);

private:
    //  "NO CARRIER" urc
    void urc_no_carrier();
    nsapi_error_t set_context_to_be_activated();
    nsapi_ip_stack_t string_to_stack_type(const char* pdp_type);

    void free_credentials();

    nsapi_error_t open_data_channel();
    bool get_context();
    bool set_new_context(int cid);
    nsapi_error_t set_registration_urc(bool on);
    nsapi_error_t delete_current_context();

#if NSAPI_PPP_AVAILABLE
    void ppp_status_cb(nsapi_event_t, intptr_t);
#endif

protected:
    NetworkStack *_stack;
    char *_apn;
    char *_uname;
    char *_pwd;
    nsapi_ip_stack_t _ip_stack_type_requested;
    nsapi_ip_stack_t _ip_stack_type;
    int _cid;
    Callback<void(nsapi_event_t, intptr_t)> _connection_status_cb;
    operator_t::RadioAccessTechnology _op_act;
    AuthenticationType _authentication_type;
    int _lac;
    int _cell_id;
    RegistrationType _last_reg_type;
    nsapi_connection_status_t _connect_status;
    bool _new_context_set;
};

} // namespace mbed

#endif // AT_CELLULAR_NETWORK_H_
