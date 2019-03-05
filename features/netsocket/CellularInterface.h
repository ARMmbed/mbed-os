/* Copyright (c) 2019 ARM Limited
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

#ifndef CELLULAR_INTERFACE_H_
#define CELLULAR_INTERFACE_H_

#include "netsocket/NetworkInterface.h"

/**
 * @addtogroup cellular
 * @{
 */

/** Common interface that is shared between cellular interfaces.
 */
class CellularInterface: public NetworkInterface {

public:
    /** Get the default cellular interface.
     *
     * This is provided as a weak method so applications can override.
     * Default behavior is to get the target's default interface, if
     * any.
     *
     * @return pointer to interface, if any.
     */
    static CellularInterface *get_default_instance();

    /** Set the cellular network credentials.
     *
     *  Please check documentation of connect() for default behavior of APN settings.
     *
     *  @param apn      Access point name.
     *  @param uname    Username (optional).
     *  @param pwd      Password (optional).
     */
    virtual void set_credentials(const char *apn, const char *uname = 0,
                                 const char *pwd = 0) = 0;

    /** Set the plmn. PLMN controls to what network device registers.
     *
     *  @param plmn     user to force what network to register.
     */
    virtual void set_plmn(const char *plmn) = 0;

    /** Set the PIN code for SIM card.
     *
     *  @param sim_pin      PIN for the SIM card.
     */
    virtual void set_sim_pin(const char *sim_pin) = 0;

    /** Attempt to connect to a cellular network with a PIN and credentials.
     *
     *  @param sim_pin     PIN for the SIM card.
     *  @param apn         Access point name (optional).
     *  @param uname       Username (optional).
     *  @param pwd         Password (optional).
     *  @return            NSAPI_ERROR_OK on success, or negative error code on failure.
     */
    virtual nsapi_error_t connect(const char *sim_pin, const char *apn = 0,
                                  const char *uname = 0,
                                  const char *pwd = 0) = 0;

    /** Attempt to connect to a cellular network.
     *
     *  If the SIM requires a PIN, and it is invalid or not set, NSAPI_ERROR_AUTH_ERROR is returned.
     *
     *  @return         NSAPI_ERROR_OK on success, or negative error code on failure.
     */
    virtual nsapi_error_t connect() = 0;

    /** Stop the interface.
     *
     *  @return         NSAPI_ERROR_OK on success, or error code on failure.
     */
    virtual nsapi_error_t disconnect() = 0;

    /** Check if the connection is currently established.
     *
     * @return `true` if the cellular module have successfully acquired a carrier and is
     *         connected to an external packet data network using PPP, `false` otherwise.
     */
    virtual bool is_connected() = 0;

    /** Get the local IP address.
     *
     *  @return         Null-terminated representation of the local IP address,
     *                  or null if no IP address has been received.
     */
    virtual const char *get_ip_address() = 0;

    /** Get the local network mask.
     *
     *  @return         Null-terminated representation of the local network mask,
     *                  or null if no network mask has been received.
     */
    virtual const char *get_netmask() = 0;

    /** Get the local gateways.
     *
     *  @return         Null-terminated representation of the local gateway,
     *                  or null if no network mask has been received.
     */
    virtual const char *get_gateway() = 0;

    /** @copydoc NetworkInterface::cellularBase
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.12", "Migrated to CellularInterface")
    virtual CellularInterface *cellularBase()
    {
        return this;
    }

    /** @copydoc NetworkInterface::cellularInterface
     */
    virtual CellularInterface *cellularInterface()
    {
        return this;
    }

#if !defined(DOXYGEN_ONLY)

protected:
    /** Get the target's default cellular interface.
     *
     * This is provided as a weak method so targets can override. The
     * default implementation configures and returns the OnBoardModemInterface,
     * if available.
     *
     * @return Pointer to interface, if any.
     */
    static CellularInterface *get_target_default_instance();

#endif //!defined(DOXYGEN_ONLY)

public:
    /** Set default parameters on a cellular interface.
     *
     * A cellular interface instantiated directly or using
     * CellularInterface::get_default_instance() is initially unconfigured.
     * This call can be used to set the default parameters that would
     * have been set if the interface had been requested using
     * NetworkInterface::get_default_instance() (see nsapi JSON
     * configuration).
     */
    virtual void set_default_parameters();
};

#endif // CELLULAR_INTERFACE_H_
