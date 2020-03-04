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
#include "ATHandler.h"
#include "AT_CellularDevice.h"

namespace mbed {

#define AT_NETWORK_TRIALS 5
#define TIMER_UNIT_LENGTH 3
#define TWO_BYTES_HEX 4
#define FOUR_BYTES_HEX 8
#define ONE_BYTE_BINARY 8

/**
 *  Class AT_CellularNetwork
 *
 *  Class for attaching to a network and getting information from it.
 */
class AT_CellularNetwork : public CellularNetwork {
public:

    AT_CellularNetwork(ATHandler &atHandler, AT_CellularDevice &device);
    virtual ~AT_CellularNetwork();
    // declare friend so it can access stack
    friend class AT_CellularDevice;

    enum RegistrationMode {
        RegistrationModeDisable = 0,
        RegistrationModeEnable, // <stat>
        RegistrationModeLAC, // <stat>[,<[lac>,]<[ci>],[<AcT>],[<rac>]]
    };

public: // CellularNetwork

    virtual nsapi_error_t set_registration(const char *plmn = 0);

    virtual nsapi_error_t get_network_registering_mode(NWRegisteringMode &mode);

    virtual nsapi_error_t set_attach();

    virtual nsapi_error_t get_attach(AttachStatus &status);

    virtual nsapi_error_t detach();

    virtual void attach(Callback<void(nsapi_event_t, intptr_t)> status_cb);

    virtual nsapi_error_t set_access_technology(RadioAccessTechnology rat);

    virtual nsapi_error_t scan_plmn(operList_t &operators, int &ops_count);

    virtual nsapi_error_t set_ciot_optimization_config(CIoT_Supported_Opt supported_opt,
                                                       CIoT_Preferred_UE_Opt preferred_opt,
                                                       Callback<void(CIoT_Supported_Opt)> network_support_cb);

    virtual nsapi_error_t get_ciot_ue_optimization_config(CIoT_Supported_Opt &supported_opt,
                                                          CIoT_Preferred_UE_Opt &preferred_opt);

    virtual nsapi_error_t get_ciot_network_optimization_config(CIoT_Supported_Opt &supported_network_opt);

    virtual nsapi_error_t get_signal_quality(int &rssi, int *ber = NULL);

    virtual int get_3gpp_error();

    virtual nsapi_error_t get_operator_params(int &format, operator_t &operator_params);

    virtual nsapi_error_t set_registration_urc(RegistrationType type, bool on);

    virtual nsapi_error_t get_operator_names(operator_names_list &op_names);

    virtual bool is_active_context(int *number_of_active_contexts = NULL, int cid = -1);

    virtual nsapi_error_t get_registration_params(registration_params_t &reg_params);

    virtual nsapi_error_t get_registration_params(RegistrationType type, registration_params_t &reg_params);

    virtual nsapi_error_t set_receive_period(int mode, EDRXAccessTechnology act_type, uint8_t edrx_value);

    virtual nsapi_error_t set_packet_domain_event_reporting(bool on);

protected:
    /** Sets access technology to be scanned. Modem specific implementation.
     *
     *  @param op_rat Access technology
     *
     *  @return       zero on success
     */
    virtual nsapi_error_t set_access_technology_impl(RadioAccessTechnology op_rat);

    /** Sends a command to query the active state of the PDP contexts.
     *  Can be overridden by the target class.
     */
    virtual void get_context_state_command();

    /** Clear the network and contexts to a known default state
     *
     *  @return         NSAPI_ERROR_OK on success
     */
    virtual nsapi_error_t clear();

private:
    void urc_creg();
    void urc_cereg();
    void urc_cgreg();
    void urc_cgev();
    void urc_cciotopti();

    void read_reg_params_and_compare(RegistrationType type);
    void read_reg_params(RegistrationType type, registration_params_t &reg_params);

    // Returns active time(Table 10.5.163/3GPP TS 24.008: GPRS Timer 2 information element) in seconds
    int calculate_active_time(const char *active_time_string, int active_time_length);
    // Returns periodic tau(Table 10.5.163a/3GPP TS 24.008: GPRS Timer 3 information element) in seconds
    int calculate_periodic_tau(const char *periodic_tau_string, int periodic_tau_length);

    // calls network callback only if status was changed, updates local connection status
    void call_network_cb(nsapi_connection_status_t status);

protected:
    Callback<void(nsapi_event_t, intptr_t)> _connection_status_cb;
    Callback<void(CIoT_Supported_Opt)> _ciotopt_network_support_cb;
    RadioAccessTechnology _op_act;
    nsapi_connection_status_t _connect_status;
    CIoT_Supported_Opt _supported_network_opt;

    registration_params_t _reg_params;
    mbed::Callback<void()> _urc_funcs[C_MAX];

    ATHandler &_at;
    AT_CellularDevice &_device;
};

} // namespace mbed

#endif // AT_CELLULAR_NETWORK_H_
