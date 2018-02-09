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
#define MAX_APN_LENGTH 63

class AT_CellularNetwork : public CellularNetwork, public AT_CellularBase
{

public:

    AT_CellularNetwork(ATHandler &atHandler);
    virtual ~AT_CellularNetwork();

public: // NetworkInterface

    /** Set the cellular network APN and credentials
     *
     *  @param apn      Optional name of the network to connect to
     *  @param username Optional username for the APN
     *  @param password Optional password fot the APN
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t set_credentials(const char *apn,
                                          const char *username = 0, const char *password = 0);

    /** Set the cellular network APN and credentials
     *
     *  @param apn      Name of the network to connect to
     *  @param type     Authentication type to use
     *  @param username Optional username for the APN
     *  @param password Optional password fot the APN
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t set_credentials(const char *apn, AuthenticationType type,
                const char *username = 0, const char *password = 0);

    /** Start the interface. Attempts to connect to a cellular network.
     *
     *  @param apn      Optional name of the network to connect to
     *  @param username Optional username for your APN
     *  @param password Optional password for your APN
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t connect(const char *apn,
                                  const char *username = 0, const char *password = 0);

    /** Start the interface. Attempts to connect to a cellular network.
     *
     *  @return 0 on success, negative error code on failure
     */
    virtual nsapi_error_t connect();

    /** Stop the interface
     *
     *  @return 0 on success, negative error code on failure
     */
    virtual nsapi_error_t disconnect();

    /** Provide access to the NetworkStack object
     *
     *  @return The underlying NetworkStack object
     */
    virtual NetworkStack *get_stack();

public: // CellularNetwork
    /** Request registering to network.
     *
     *  @param plmn     format is in numeric format or 0 for automatic network registration
     *  @return         zero on success
     */
    virtual nsapi_error_t set_registration(char *plmn = 0);

    /**
     * Gets the network registration status.
     * @param type      see RegistrationType values
     * @param status    see RegistrationStatus values
     * @return zero on success
     */
    virtual nsapi_error_t get_registration_status(RegistrationType type, RegistrationStatus &status);

    /** Request attach to network.
     *
     *  @param timeout milliseconds to wait for attach response
     *  @return        zero on success
     */
    virtual nsapi_error_t set_attach(int timeout = 10*1000);

    /** Request attach status from network.
     *
     *  @param status see AttachStatus values
     *  @return       zero on success
     */
    virtual nsapi_error_t get_attach(AttachStatus &status);

    /** Get APN rate control.
     *
     *  @remark optional params are not updated if not received from network, so use good defaults
     *
     *  @param reports       Additional exception reports at maximum rate reached are allowed to be sent [optional]
     *  @param time_unit     Uplink time unit with values 0=unrestricted, 1=minute, 2=hour, 3=day, 4=week [optional]
     *  @param uplink_rate   Maximum number of messages per timeUnit [optional]
     *  @return              zero on success
     */
    virtual nsapi_error_t get_rate_control(CellularNetwork::RateControlExceptionReports &reports,
        CellularNetwork::RateControlUplinkTimeUnit &time_unit, int &uplink_rate);

    /** Get backoff timer value
     *
     *  @param backoff_time Backoff timer value associated with PDP APN in seconds
     *  @return             zero on success
     */
    virtual nsapi_error_t get_backoff_time(int &backoff_time);

    /** Get notified if the connection gets lost
     *
     *  @param cb         user defined callback
     */
    void connection_status_cb(Callback<void(nsapi_error_t)> cb);

    /** Get the local IP address
     *
     *  @return         Null-terminated representation of the local IP address
     *                  or null if no IP address has been received
     */
    virtual const char *get_ip_address();

    /** Sets radio access technology.
     *
     *  @param op_rat Radio access technology
     *  @return       zero on success
     */
    virtual nsapi_error_t set_access_technology(operator_t::RadioAccessTechnology op_rat);

    /** Scans for operators module can reach.
     *
     *  @param operators Container of reachable operators and their access technologies
     *  @param ops_count Number of found operators
     *  @return          zero on success
     */
    virtual nsapi_error_t scan_plmn(operList_t &operators, int &ops_count);

    /** Set CIoT optimizations.
     *
     *  @param supported_opt Supported CIoT EPS optimizations.
     *  @param preferred_opt Preferred CIoT EPS optimizations.
     *  @return zero on success
     */
    virtual nsapi_error_t set_ciot_optimization_config(Supported_UE_Opt supported_opt,
                                                       Preferred_UE_Opt preferred_opt);

    /** Get CIoT optimizations.
     *
     *  @param supported_opt Supported CIoT EPS optimizations.
     *  @param preferred_opt Preferred CIoT EPS optimizations.
     *  @return zero on success
     */
    virtual nsapi_error_t get_ciot_optimization_config(Supported_UE_Opt& supported_opt,
                                                       Preferred_UE_Opt& preferred_opt);


    /** Set the pdn type to be used
     *
     *  @param stack_type the stack type to be used.
     *  @return NSAPI_ERROR_OK on success
     */
     virtual nsapi_error_t set_stack_type(nsapi_ip_stack_t stack_type);

    /** Get the pdn type in use
     *
     *  @return stack type
     */
     virtual nsapi_ip_stack_t get_stack_type();

    /** Get the relevant information for an active non secondary PDP context.
     *
     *  @remark optional params are not updated if not received from network.
     *
     *  @param params_list  reference to linked list which is filled on successful call
     *  @return             0 on success, negative error code on failure
     */
     virtual nsapi_error_t get_pdpcontext_params(pdpContextList_t& params_list);

    /** Get extended signal quality parameters.
     *
     *  @param rxlev signal strength level
     *  @param ber bit error rate
     *  @param rscp signal code power
     *  @param ecno ratio of the received energy per PN chip to the total received power spectral density
     *  @param rsrq signal received quality
     *  @param rsrp signal received power
     *  @return NSAPI_ERROR_OK on success, negative error code on failure
     */
    virtual nsapi_error_t get_extended_signal_quality(int &rxlev, int &ber, int &rscp, int &ecno, int &rsrq, int &rsrp);

    /** Get signal quality parameters.
     *
     *  @param rssi signal strength level
     *  @param ber bit error rate
     *  @return NSAPI_ERROR_OK on success, negative error code on failure
     */
    virtual nsapi_error_t get_signal_quality(int &rssi, int &ber);

    /** Get cell id.
     *
     *  @param cell_id cell id
     *  @return NSAPI_ERROR_OK on success, negative error code on failure
     */
    virtual nsapi_error_t get_cell_id(int &cell_id);

    /** Get the last 3GPP error code
     *  @return see 3GPP TS 27.007 error codes
     */
    virtual uint8_t get_3gpp_error();

    /** Get the operator params
     *
     *  @param format format of the operator field
     *  @param operator_params applicable operator param fields filled
     *  @return NSAPI_ERROR_OK on success, negative error code on failure
     */
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

    /** Sets access technology to be scanned.
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

    nsapi_error_t open_data_channel();
    bool get_context(nsapi_ip_stack_t supported_stack);
    bool set_new_context(nsapi_ip_stack_t stack, int cid);
    nsapi_error_t set_registration_urc(bool on);

protected:
    NetworkStack *_stack;
    char _apn[MAX_APN_LENGTH];
    const char *_uname;
    const char *_pwd;
    nsapi_ip_stack_t _ip_stack_type_requested;
    nsapi_ip_stack_t _ip_stack_type;
    int _cid;
    Callback<void(nsapi_error_t)> _connection_status_cb;
    operator_t::RadioAccessTechnology _op_act;
    AuthenticationType _authentication_type;
    int _lac;
    int _cell_id;
    operator_t::RadioAccessTechnology _AcT;
    RegistrationType _last_reg_type;
};

} // namespace mbed

#endif // AT_CELLULAR_NETWORK_H_
