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

#ifndef CELLULAR_NETWORK_H_
#define CELLULAR_NETWORK_H_

#include "NetworkInterface.h"
#include "CellularList.h"

namespace mbed {

/* Maximum length of IPV6 address in ipv4-like dotted format. More info in 3gpp 27007.*/
const int MAX_IPV6_ADDR_IN_IPV4LIKE_DOTTED_FORMAT = 63;
/* Maximum length of access point name */
const int MAX_ACCESSPOINT_NAME_LENGTH = 100;
const int MAX_OPERATOR_NAME_LONG = 16;
const int MAX_OPERATOR_NAME_SHORT = 8;

/**
 *  Class CellularNetwork
 *
 *  An abstract interface for connecting to a network and getting information from it.
 */
class CellularNetwork : public NetworkInterface
{
protected:
    // friend of CellularDevice so that it's the only way to close/delete this class.
    friend class CellularDevice;

    /**
     * virtual Destructor
     */
    virtual ~CellularNetwork() {}

public:
    /* Definition for Supported CIoT EPS optimizations type. */
    enum Supported_UE_Opt {
        SUPPORTED_UE_OPT_NO_SUPPORT = 0, /* No support. */
        SUPPORTED_UE_OPT_CONTROL_PLANE,  /* Support for control plane CIoT EPS optimization. */
        SUPPORTED_UE_OPT_USER_PLANE,     /* Support for user plane CIoT EPS optimization. */
        SUPPORTED_UE_OPT_BOTH,           /* Support for both control plane CIoT EPS optimization and user plane CIoT EPS
                                            optimization. */
        SUPPORTED_UE_OPT_MAX
    };

    /* Definition for Preferred CIoT EPS optimizations type. */
    enum Preferred_UE_Opt {
        PREFERRED_UE_OPT_NO_PREFERENCE = 0, /* No preference. */
        PREFERRED_UE_OPT_CONTROL_PLANE,     /* Preference for control plane CIoT EPS optimization. */
        PREFERRED_UE_OPT_USER_PLANE,        /* Preference for user plane CIoT EPS optimization. */
        PREFERRED_UE_OPT_MAX
    };

    /* Network registration status */
    enum RegistrationStatus {
        NotRegistered = 0,
        RegisteredHomeNetwork,
        SearchingNetwork,
        RegistrationDenied,
        Unknown,
        RegisteredRoaming,
        RegisteredSMSOnlyHome,
        RegisteredSMSOnlyRoaming,
        AttachedEmergencyOnly,
        RegisteredCSFBNotPreferredHome,
        RegisteredCSFBNotPreferredRoaming = 10
    };

    /* Network registration type */
    enum RegistrationType {
        C_EREG = 0,
        C_GREG,
        C_REG,
        C_MAX
    };

    /* device attach status to network */
    enum AttachStatus {
        Detached = 0,
        Attached,
    };

    /*  whether the additional exception reports are allowed to be sent when the maximum uplink rate is reached */
    enum RateControlExceptionReports {
        NotAllowedToBeSent = 0,
        AllowedToBeSent
    };

    /* specifies the time unit to be used for the maximum uplink rate */
    enum RateControlUplinkTimeUnit {
        Unrestricted = 0,
        Minute,
        Hour,
        Day,
        Week
    };

    /* authentication type when activating or modifying the pdp context */
    enum AuthenticationType {
        NOAUTH = 0,
        PAP,
        CHAP
    };

    // 3GPP TS 27.007 - 7.3 PLMN selection +COPS
    struct operator_t {
        enum Status {
            Unknown,
            Available,
            Current,
            Forbiden
        };

        enum RadioAccessTechnology {
            RAT_GSM,
            RAT_GSM_COMPACT,
            RAT_UTRAN,
            RAT_EGPRS,
            RAT_HSDPA,
            RAT_HSUPA,
            RAT_HSDPA_HSUPA,
            RAT_E_UTRAN,
            RAT_CATM1,
            RAT_NB1,
            RAT_UNKNOWN
        };


        Status op_status;
        char op_long[MAX_OPERATOR_NAME_LONG+1];
        char op_short[MAX_OPERATOR_NAME_SHORT+1];
        char op_num[MAX_OPERATOR_NAME_SHORT+1];
        RadioAccessTechnology op_rat;
        operator_t *next;

        operator_t() {
            op_status = Unknown;
            op_rat = RAT_UNKNOWN;
            next = NULL;
        }
    };

    typedef CellularList<operator_t> operList_t;

    /* PDP Context information */
    struct pdpcontext_params_t {
        char apn[MAX_ACCESSPOINT_NAME_LENGTH+1];
        char local_addr[MAX_IPV6_ADDR_IN_IPV4LIKE_DOTTED_FORMAT+1];
        char local_subnet_mask[MAX_IPV6_ADDR_IN_IPV4LIKE_DOTTED_FORMAT+1];
        char gateway_addr[MAX_IPV6_ADDR_IN_IPV4LIKE_DOTTED_FORMAT+1];
        char dns_primary_addr[MAX_IPV6_ADDR_IN_IPV4LIKE_DOTTED_FORMAT+1];
        char dns_secondary_addr[MAX_IPV6_ADDR_IN_IPV4LIKE_DOTTED_FORMAT+1];
        char p_cscf_prim_addr[MAX_IPV6_ADDR_IN_IPV4LIKE_DOTTED_FORMAT+1];
        char p_cscf_sec_addr[MAX_IPV6_ADDR_IN_IPV4LIKE_DOTTED_FORMAT+1];
        int cid;
        int bearer_id;
        int im_signalling_flag;
        int lipa_indication;
        int ipv4_mtu;
        int wlan_offload;
        int local_addr_ind;
        int non_ip_mtu;
        int serving_plmn_rate_control_value;
        pdpcontext_params_t* next;

        pdpcontext_params_t() {
            apn[0] = '\0';
            local_addr[0] = '\0';
            local_subnet_mask[0] = '\0';
            gateway_addr[0] = '\0';
            dns_primary_addr[0] = '\0';
            dns_secondary_addr[0] = '\0';
            p_cscf_prim_addr[0] = '\0';
            p_cscf_sec_addr[0] = '\0';
            cid = -1;
            bearer_id = -1;
            im_signalling_flag = -1;
            lipa_indication = -1;
            ipv4_mtu = -1;
            wlan_offload = -1;
            local_addr_ind = -1;
            non_ip_mtu = -1;
            serving_plmn_rate_control_value = -1;
            next = NULL;
        }
    };
    typedef CellularList<pdpcontext_params_t> pdpContextList_t;

    /** Request registering to network.
     *
     *  @param plmn     format is in numeric format or 0 for automatic network registration
     *  @return         zero on success
     */
    virtual nsapi_error_t set_registration(const char *plmn = 0) = 0;

    /** Gets the network registration status.
     *
     * @param type      see RegistrationType values
     * @param status    see RegistrationStatus values
     * @return zero on success
     */
    virtual nsapi_error_t get_registration_status(RegistrationType type, RegistrationStatus &status) = 0;

    /** Set the cellular network APN and credentials
     *
     *  @param apn      Optional name of the network to connect to
     *  @param username Optional username for the APN
     *  @param password Optional password fot the APN
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t set_credentials(const char *apn,
                                          const char *username = 0, const char *password = 0) = 0;

    /** Set the cellular network APN and credentials
     *
     *  @param apn      Name of the network to connect to
     *  @param type     Authentication type to use
     *  @param username Optional username for the APN
     *  @param password Optional password fot the APN
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t set_credentials(const char *apn, AuthenticationType type,
                const char *username = 0, const char *password = 0) = 0;

    /** Request attach to network.
     *
     *  @param timeout milliseconds to wait for attach response
     *  @return        zero on success
     */
    virtual nsapi_error_t set_attach(int timeout = 10*1000) = 0;

    /** Request attach status from network.
     *
     *  @param status see AttachStatus values
     *  @return       zero on success
     */
    virtual nsapi_error_t get_attach(AttachStatus &status) = 0;

    /** Get APN rate control.
     *
     *  @remark optional params are not updated if not received from network, so use good defaults
     *  @param reports       Additional exception reports at maximum rate reached are allowed to be sent [optional]
     *  @param time_unit     Uplink time unit with values 0=unrestricted, 1=minute, 2=hour, 3=day, 4=week [optional]
     *  @param uplink_rate   Maximum number of messages per timeUnit [optional]
     *  @return              zero on success
     */
    virtual nsapi_error_t get_rate_control(CellularNetwork::RateControlExceptionReports &reports,
        CellularNetwork::RateControlUplinkTimeUnit &time_unit, int &uplink_rate) = 0;

    /** Get backoff timer value
     *
     *  @param backoff_timer Backoff timer value associated with PDP APN in seconds
     *  @return              zero on success
     */
    virtual nsapi_error_t get_apn_backoff_timer(int &backoff_timer) = 0;

    /** Sets radio access technology.
     *
     *  @param op_rat Radio access technology
     *  @return       zero on success
     */
    virtual nsapi_error_t set_access_technology(operator_t::RadioAccessTechnology op_rat) = 0;

    /** Scans for operators module can reach.
     *
     *  @param operators Container of reachable operators and their access technologies
     *  @param ops_count Number of found operators
     *  @return          zero on success
     */
    virtual nsapi_error_t scan_plmn(operList_t &operators, int &ops_count) = 0;

    /** Set CIoT optimizations.
     *
     *  @param supported_opt Supported CIoT EPS optimizations.
     *  @param preferred_opt Preferred CIoT EPS optimizations.
     *  @return zero on success
     */
    virtual nsapi_error_t set_ciot_optimization_config(Supported_UE_Opt supported_opt,
                                                       Preferred_UE_Opt preferred_opt) = 0;

    /** Get CIoT optimizations.
     *
     *  @param supported_opt Supported CIoT EPS optimizations.
     *  @param preferred_opt Preferred CIoT EPS optimizations.
     *  @return zero on success
     */
    virtual nsapi_error_t get_ciot_optimization_config(Supported_UE_Opt& supported_opt,
                                                       Preferred_UE_Opt& preferred_opt) = 0;

    /** Start the interface. Attempts to connect to a cellular network.
     *
     *  @return 0 on success, negative error code on failure
     */
    virtual nsapi_error_t connect() = 0;

    /** Start the interface. Attempts to connect to a cellular network.
     *
     *  @param apn      Optional name of the network to connect to
     *  @param username Optional username for your APN
     *  @param password Optional password for your APN
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t connect(const char *apn,
                                  const char *username = 0, const char *password = 0) = 0;

    /**
     * Set the pdn type to be used
     *
     * @param stack_type the stack type to be used.
     *
     * @return NSAPI_ERROR_OK on success
     */
    virtual nsapi_error_t set_stack_type(nsapi_ip_stack_t stack_type) = 0;

    /**
     * Get the pdn type in use
     *
     * @return stack type
     */
    virtual nsapi_ip_stack_t get_stack_type() = 0;

    /** Get the relevant information for an active non secondary PDP context.
     *
     *  @remark optional params are not updated if not received from network.
     *  @param params_list  reference to linked list which is filled on successful call
     *  @return             0 on success, negative error code on failure
     */
    virtual nsapi_error_t get_pdpcontext_params(pdpContextList_t& params_list) = 0;

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
    virtual nsapi_error_t get_extended_signal_quality(int &rxlev, int &ber, int &rscp, int &ecno, int &rsrq, int &rsrp) = 0;

    /** Get signal quality parameters.
     *
     *  @param rssi signal strength level
     *  @param ber bit error rate
     *  @return NSAPI_ERROR_OK on success, negative error code on failure
     */
    virtual nsapi_error_t get_signal_quality(int &rssi, int &ber) = 0;

    /** Get cell id.
     *
     *  @param cell_id cell id
     *  @return NSAPI_ERROR_OK on success, negative error code on failure
     */
    virtual nsapi_error_t get_cell_id(int &cell_id) = 0;

    /** Get the last 3GPP error code
     *  @return see 3GPP TS 27.007 error codes
     */
    virtual int get_3gpp_error() = 0;

    /** Get the operator parameters.
     *
     *  @param format format of the operator field
     *  @param operator_params applicable operator param fields filled
     *  @return NSAPI_ERROR_OK on success, negative error code on failure
     */
    virtual nsapi_error_t get_operator_params(int &format, operator_t &operator_params) = 0;

    /** Register callback for status reporting
     *
     *  The specified status callback function will be called on status changes
     *  on the network. The parameters on the callback are the event type and
     *  event-type dependent reason parameter.
     *
     *  @param status_cb The callback for status changes
     */
    virtual void attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb) = 0;

    /** Get the connection status
     *
     *  @return         The connection status according to ConnectionStatusType
     */
    virtual nsapi_connection_status_t get_connection_status() const = 0;

    /** Set blocking status of connect() which by default should be blocking
     *
     *  @param blocking true if connect is blocking
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t set_blocking(bool blocking) = 0;

};

} // namespace mbed

#endif // CELLULAR_NETWORK_H_
