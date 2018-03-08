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

#ifndef EASY_CELLULAR_CONNECTION_H

#define EASY_CELLULAR_CONNECTION_H

#include "CellularConnectionFSM.h"
#if defined(CELLULAR_DEVICE) || defined(DOXYGEN_ONLY)

#include "netsocket/CellularBase.h"

#define USE_APN_LOOKUP (MBED_CONF_CELLULAR_USE_APN_LOOKUP || (NSAPI_PPP_AVAILABLE && MBED_CONF_PPP_CELL_IFACE_APN_LOOKUP))

namespace mbed
{

/** EasyCellularConnection class
 *
 *  Simplified adapter for cellular connection
 */
class EasyCellularConnection: public CellularBase
{

public:
    EasyCellularConnection(bool debug = false);
    virtual ~EasyCellularConnection();

public:
    /** Set the Cellular network credentials
     *
     *  Please check documentation of connect() for default behaviour of APN settings.
     *
     *  @param apn      Access point name
     *  @param uname    optionally, Username
     *  @param pwd      optionally, password
     */
    virtual void set_credentials(const char *apn, const char *uname = 0,
                                 const char *pwd = 0);

    /** Set the pin code for SIM card
     *
     *  @param sim_pin      PIN for the SIM card
     */
    virtual void set_sim_pin(const char *sim_pin);

    /** Start the interface
     *
     *  Attempts to connect to a Cellular network.
     *
     *  @param sim_pin     PIN for the SIM card
     *  @param apn         optionally, access point name
     *  @param uname       optionally, Username
     *  @param pwd         optionally, password
     *  @return            NSAPI_ERROR_OK on success, or negative error code on failure
     */
    virtual nsapi_error_t connect(const char *sim_pin, const char *apn = 0,
                                  const char *uname = 0,
                                  const char *pwd = 0);

    /** Start the interface
     *
     *  Attempts to connect to a Cellular network.
     *  If the SIM requires a PIN, and it is not set/invalid, NSAPI_ERROR_AUTH_ERROR is returned.
     *
     *  @return            NSAPI_ERROR_OK on success, or negative error code on failure
     */
    virtual nsapi_error_t connect();

    /** Stop the interface
     *
     *  @return         0 on success, or error code on failure
     */
    virtual nsapi_error_t disconnect();

    /** Check if the connection is currently established or not
     *
     * @return true/false   If the cellular module have successfully acquired a carrier and is
     *                      connected to an external packet data network using PPP, isConnected()
     *                      API returns true and false otherwise.
     */
    virtual bool is_connected();

    /** Get the local IP address
     *
     *  @return         Null-terminated representation of the local IP address
     *                  or null if no IP address has been received
     */
    virtual const char *get_ip_address();

    /** Get the local network mask
     *
     *  @return         Null-terminated representation of the local network mask
     *                  or null if no network mask has been received
     */
    virtual const char *get_netmask();

    /** Get the local gateways
     *
     *  @return         Null-terminated representation of the local gateway
     *                  or null if no network mask has been received
     */
    virtual const char *get_gateway();

    /** Register callback for status reporting
     *
     *  @param status_cb The callback for status changes
     */
    virtual void attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb);

    /** Turn modem debug traces on
     *
     *  @param on         set true to enable debug traces
     */
    void modem_debug_on(bool on);

protected:

    /** Provide access to the NetworkStack object
     *
     *  @return The underlying NetworkStack object
     */
    virtual NetworkStack *get_stack();

private:
    /** Callback for cellular status changes
     *
     *  @return true to continue state machine
     */
    bool cellular_status(int state, int next_state);
    nsapi_error_t init();
    nsapi_error_t check_connect();

    bool _is_connected;
    bool _is_initialized;
#if USE_APN_LOOKUP
    bool _credentials_set;
#endif // #if USE_APN_LOOKUP
    CellularConnectionFSM::CellularState _target_state;

    UARTSerial _cellularSerial;
    rtos::Semaphore _cellularSemaphore;
    CellularConnectionFSM _cellularConnectionFSM;
    nsapi_error_t _credentials_err;
};

} // namespace

#endif // CELLULAR_DEVICE || DOXYGEN

#endif // EASY_CELLULAR_CONNECTION_H

/** @}*/
