/* LWIP implementation of NetworkInterfaceAPI
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

#ifndef ETHERNET_INTERFACE_H
#define ETHERNET_INTERFACE_H

#include "nsapi.h"
#include "rtos.h"
#include "lwip/netif.h"

// Forward declaration
class NetworkStack;


/** EthernetInterface class
 *  Implementation of the NetworkStack for LWIP
 */
class EthernetInterface : public EthInterface
{
public:
    /** Start the interface
     *  @return             0 on success, negative on failure
     */
    virtual int connect();

    /** Stop the interface
     *  @return             0 on success, negative on failure
     */
    virtual int disconnect();

    /** Get the internally stored IP address
     *  @return             IP address of the interface or null if not yet connected
     */
    virtual const char *get_ip_address();

    /** Get the internally stored MAC address
     *  @return             MAC address of the interface
     */
    virtual const char *get_mac_address();

protected:
    /** Provide access to the underlying stack
     *
     *  @return The underlying network stack 
     */
    virtual NetworkStack *get_stack();
};


#endif
