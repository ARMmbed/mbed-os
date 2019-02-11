/* Copyright (c) 2017,2018 Arm Limited and affiliates.
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

#include "CellularList.h"
#include "Callback.h"
#include "nsapi_types.h"

namespace mbed {

/* Maximum length of IPV6 address in ipv4-like dotted format. More info in 3gpp 27007.*/
const int MAX_IPV6_ADDR_IN_IPV4LIKE_DOTTED_FORMAT = 63;
/* Maximum length of access point name */
const int MAX_ACCESSPOINT_NAME_LENGTH = 100;
const int MAX_OPERATOR_NAME_LONG = 16;
const int MAX_OPERATOR_NAME_SHORT = 8;

/**
 * @addtogroup cellular
 * @{
 */

/// An abstract interface for connecting to a network and getting information from it.
class CellularNetwork {
protected:
    // friend of CellularDevice so that it's the only way to close/delete this class.
    friend class CellularDevice;

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
        StatusNotAvailable = -1,
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
        RegisteredCSFBNotPreferredRoaming,
        AlreadyRegistered = 11, // our definition when modem says that we are not registered but we have active PDP Context
        RegistrationStatusMax
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
        RAT_UNKNOWN,
        RAT_MAX = 11 // to reserve string array
    };

    /// 3GPP TS 27.007 - 7.3 PLMN selection +COPS
    struct operator_t {
        enum Status {
            Unknown,
            Available,
            Current,
            Forbiden
        };

        Status op_status;
        char op_long[MAX_OPERATOR_NAME_LONG + 1];
        char op_short[MAX_OPERATOR_NAME_SHORT + 1];
        char op_num[MAX_OPERATOR_NAME_SHORT + 1];
        RadioAccessTechnology op_rat;
        operator_t *next;

        operator_t()
        {
            op_status = Unknown;
            op_rat = RAT_UNKNOWN;
            next = NULL;
            op_long[0] = '\0';
            op_short[0] = '\0';
            op_num[0] = '\0';
        }
    };

    typedef CellularList<operator_t> operList_t;

    /// Cellular operator names in numeric and alpha format
    struct operator_names_t {
        char numeric[MAX_OPERATOR_NAME_SHORT + 1];
        char alpha[MAX_OPERATOR_NAME_LONG + 1];
        operator_names_t *next;
        operator_names_t()
        {
            numeric[0] = '\0';
            alpha[0] = '\0';
            next = NULL;
        }
    };
    typedef CellularList<operator_names_t> operator_names_list;

    /// Network registering mode
    enum NWRegisteringMode {
        NWModeAutomatic = 0,    // automatic registering
        NWModeManual,           // manual registering with plmn
        NWModeDeRegister,       // deregister from network
        NWModeSetOnly,          // set only <format> (for read command +COPS?), do not attempt registration/deregistration
        NWModeManualAutomatic   // if manual fails, fallback to automatic
    };

    /// Network registration information
    struct registration_params_t {
        RegistrationType _type;
        RegistrationStatus _status;
        RadioAccessTechnology _act;
        int _cell_id;
        int _lac;
        int _active_time;
        int _periodic_tau;

        registration_params_t()
        {
            _type = C_MAX;
            _status = StatusNotAvailable;
            _act = RAT_UNKNOWN;
            _cell_id = -1;
            _lac = -1;
            _active_time = -1;
            _periodic_tau = -1;
        }
    };

    /** Request registering to network.
     *
     *  @param plmn     format is in numeric format or 0 for automatic network registration
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t set_registration(const char *plmn = 0) = 0;

    /** Get the current network registering mode
     *
     *  @param mode     on successful return contains the current network registering mode
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t get_network_registering_mode(NWRegisteringMode &mode) = 0;

    /** Activate/deactivate listening of network events for the given RegistrationType.
     *  This should be called after network class is created and ready to receive AT commands.
     *  After successful call network class starts to get information about network changes like
     *  registration statue, access technology, cell id...
     *
     *  @param type     RegistrationType to set urc on/off
     *  @param on       Controls are urc active or not
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_UNSUPPORTED if the modem does not support RegistrationType
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t set_registration_urc(RegistrationType type, bool on) = 0;


    /** Request attach to network.
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t set_attach() = 0;

    /** Request attach status from network.
     *
     *  @param status   see AttachStatus values
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t get_attach(AttachStatus &status) = 0;

    /** Request detach and deregister from a network.
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t detach() = 0;

    /** Sets radio access technology.
     *
     *  @param rat           Radio access technology
     *  @return              NSAPI_ERROR_OK on success
     *                       NSAPI_ERROR_UNSUPPORTED if the given rat is RAT_UNKNOWN or inheriting target class
     *                                               has not implemented method set_access_technology_impl(...)
     *                       OR return value of the inheriting target class set_access_technology_impl(...)
     */
    virtual nsapi_error_t set_access_technology(RadioAccessTechnology rat) = 0;

    /** Scans for operators module can reach.
     *
     *  @param operators     Container of reachable operators and their access technologies
     *  @param ops_count     Number of found operators
     *  @return              NSAPI_ERROR_OK on success
     *                       NSAPI_ERROR_NO_MEMORY on memory failure
     *                       NSAPI_ERROR_DEVICE_ERROR on other failures
     */
    virtual nsapi_error_t scan_plmn(operList_t &operators, int &ops_count) = 0;

    /** Set CIoT optimizations.
     *
     *  @param supported_opt Supported CIoT EPS optimizations.
     *  @param preferred_opt Preferred CIoT EPS optimizations.
     *  @return              NSAPI_ERROR_OK on success
     *                       NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t set_ciot_optimization_config(Supported_UE_Opt supported_opt,
                                                       Preferred_UE_Opt preferred_opt) = 0;

    /** Get CIoT optimizations.
     *
     *  @param supported_opt Supported CIoT EPS optimizations.
     *  @param preferred_opt Preferred CIoT EPS optimizations.
     *  @return              NSAPI_ERROR_OK on success
     *                       NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t get_ciot_optimization_config(Supported_UE_Opt &supported_opt,
                                                       Preferred_UE_Opt &preferred_opt) = 0;

    /** Get extended signal quality parameters.
     *
     *  @param rxlev         signal strength level
     *  @param ber           bit error rate
     *  @param rscp          signal code power
     *  @param ecno          ratio of the received energy per PN chip to the total received power spectral density
     *  @param rsrq          signal received quality
     *  @param rsrp          signal received power
     *  @return              NSAPI_ERROR_OK on success
     *                       NSAPI_ERROR_DEVICE_ERROR on other failures
     */
    virtual nsapi_error_t get_extended_signal_quality(int &rxlev, int &ber, int &rscp, int &ecno, int &rsrq, int &rsrp) = 0;

    /** Get signal quality parameters.
     *
     *  @param rssi          signal strength level
     *  @param ber           bit error rate
     *  @return              NSAPI_ERROR_OK on success
     *                       NSAPI_ERROR_DEVICE_ERROR on other failures
     */
    virtual nsapi_error_t get_signal_quality(int &rssi, int &ber) = 0;

    /** Get the last 3GPP error code
     *  @return see 3GPP TS 27.007 error codes
     */
    virtual int get_3gpp_error() = 0;

    /** Get the operator parameters.
     *
     *  @param format           format of the operator field
     *  @param operator_params  applicable operator param fields filled
     *  @return                 NSAPI_ERROR_OK on success
     *                          NSAPI_ERROR_DEVICE_ERROR on case of other failures
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

    /** Read operator names
     *
     *  @param op_names      on successful return contains linked list of operator names.
     *  @return              NSAPI_ERROR_OK on success
     *                       NSAPI_ERROR_NO_MEMORY on memory failure
     *                       NSAPI_ERROR_DEVICE_ERROR on other failures
     */
    virtual nsapi_error_t get_operator_names(operator_names_list &op_names) = 0;

    /** Check if there is any PDP context active
     *
     *  @return true is any context is active, false otherwise or in case of error
     */
    virtual bool is_active_context() = 0;

    /** Gets the latest received registration parameters from the network:
     *  type, status, access technology, cell_id, lac, active_time, periodic_tau.
     *
     *  @param reg_params   see registration_params_t
     *  @return             NSAPI_ERROR_OK on success
     *                      NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t get_registration_params(registration_params_t &reg_params) = 0;

    /** Gets the current network registration parameters from the network with type:
    *   status, access technology, cell_id, lac, active_time, periodic_tau.
    *
    *  @param type         see RegistrationType values
    *  @param reg_params   see registration_params_t
    *  @return             NSAPI_ERROR_OK on success
    *                      NSAPI_ERROR_UNSUPPORTED if the modem does not support RegistrationType
    *                      NSAPI_ERROR_DEVICE_ERROR on failure
    */
    virtual nsapi_error_t get_registration_params(RegistrationType type, registration_params_t &reg_params) = 0;
};

/**
 * @}
 */

} // namespace mbed

#endif // CELLULAR_NETWORK_H_
