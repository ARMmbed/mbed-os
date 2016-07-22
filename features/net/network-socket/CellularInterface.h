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
 
#include "network-socket/NetworkInterface.h"
 

/** CellularInterface class
 *
 *  Common interface that is shared between ethernet hardware
 */
class CellularInterface : public NetworkInterface
{
public:
    /** Start the interface
     *
     *  @param apn      Optional name of the network to connect to
     *  @param username Optional username for your APN
     *  @param password Optional password for your APN 
     *  @return         0 on success, negative error code on failure
     */
    virtual int connect(const char *apn = 0, const char *username = 0, const char *password = 0) = 0;
 
    /** Stop the interface
     *
     *  @return         0 on success, negative error code on failure
     */
    virtual int disconnect() = 0;

    /** Get the local MAC address
     *
     *  @return         Null-terminated representation of the local MAC address
     */
    virtual const char *get_mac_address() = 0;
};

 
#endif
