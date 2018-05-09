/* CellularInterface
 * Copyright (c) 2015 ARM Limited
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

#ifndef CELLULAR_INTERFACE_H
#define CELLULAR_INTERFACE_H

#include "netsocket/NetworkInterface.h"

/** CellularInterface class
 *
 *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/framework/API/CellularNetwork.h instead.
 *
 *  Common interface that is shared between ethernet hardware
 *  @addtogroup netsocket
 */
class CellularInterface : public NetworkInterface
{
public:
    /** CellularInterface lifetime
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/framework/API/CellularNetwork.h instead.")
    virtual ~CellularInterface() {};

    /** Set the cellular network APN and credentials
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/framework/API/CellularNetwork.h instead.
     *
     *  @param apn      Optional name of the network to connect to
     *  @param username Optional username for the APN
     *  @param password Optional password fot the APN
     *  @return         0 on success, negative error code on failure
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/framework/API/CellularNetwork.h instead.")
    virtual nsapi_error_t set_credentials(const char *apn,
            const char *username = 0, const char *password = 0) = 0;

    /** Start the interface
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/framework/API/CellularNetwork.h instead.
     *
     *  @param apn      Optional name of the network to connect to
     *  @param username Optional username for your APN
     *  @param password Optional password for your APN
     *  @return         0 on success, negative error code on failure
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/framework/API/CellularNetwork.h instead.")
    virtual nsapi_error_t connect(const char *apn,
            const char *username = 0, const char *password = 0) = 0;

    /** Start the interface
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/framework/API/CellularNetwork.h instead.
     *
     *  Attempts to connect to a cellular network based on supplied credentials
     *
     *  @return         0 on success, negative error code on failure
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/framework/API/CellularNetwork.h instead.")
    virtual nsapi_error_t connect() = 0;

    /** Stop the interface
     *
     *  @deprecated This API will be deprecated in mbed-os-5.9. Use mbed-os/features/cellular/framework/API/CellularNetwork.h instead.
     *
     *  @return         0 on success, negative error code on failure
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, use mbed-os/features/cellular/framework/API/CellularNetwork.h instead.")
    virtual nsapi_error_t disconnect() = 0;
};


#endif
