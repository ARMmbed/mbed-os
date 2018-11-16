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
#ifndef UBLOX_AT_CELLULARCONTEXT_H_
#define UBLOX_AT_CELLULARCONTEXT_H_

#include "AT_CellularContext.h"

namespace mbed {

class UBLOX_AT_CellularContext: public AT_CellularContext {
public:
    UBLOX_AT_CellularContext(ATHandler &at, CellularDevice *device, const char *apn, bool cp_req = false, bool nonip_req = false);
    virtual ~UBLOX_AT_CellularContext();

    virtual void do_connect();
    virtual const char *get_gateway();

protected:
    virtual NetworkStack *get_stack();

private:

    /** Length of IMSI buffer.
     */
    static const int MAX_IMSI_LENGTH = 15;

    /** The type of authentication to use.
     */
    nsapi_security_t _auth;

    /** Connect the on board IP stack of the modem.
     *
     * @return      True if successful, otherwise false.
     */
    nsapi_error_t open_data_channel();

    /** Activate one of the on-board modem's connection profiles.
     *
     * @param apn      The APN to use.
     * @param username The user name to use.
     * @param password The password to use.
     * @param auth     The authentication method to use
     *                 (NSAPI_SECURITY_NONE, NSAPI_SECURITY_PAP,
     *                 NSAPI_SECURITY_CHAP or NSAPI_SECURITY_UNKNOWN).
     * @return         True if successful, otherwise false.
     */
    bool activate_profile(const char *apn, const char *username, const char *password);

    /** Convert nsapi_security_t to the modem security numbers.
     *
     * @param nsapi_security      Security protocol.
     * @return                    Modem security numbers.
     */
    int nsapi_security_to_modem_security(nsapi_security_t nsapi_security);

    /** Disconnect the on board IP stack of the modem.
     *
     * @return      True if successful, otherwise false.
     */
    bool disconnect_modem_stack();

    /** Read IMSI of modem.
     */
    nsapi_error_t get_imsi(char *imsi);

    /** Get the next set of credentials from the database.
     */
    void get_next_credentials(char **config);
};

} /* namespace mbed */

#endif // UBLOX_AT_CELLULARCONTEXT_H_
