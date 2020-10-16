/* LWIP implementation of NetworkInterfaceAPI
 * Copyright (c) 2015 ARM Limited
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

#ifndef ETHERNET_INTERFACE_H
#define ETHERNET_INTERFACE_H

#include "nsapi.h"
#include "EMACInterface.h"


/** Implementation of the NetworkStack for an EMAC-based Ethernet driver.
 */
class EthernetInterface : public EMACInterface, public EthInterface {
public:
    /** Create an EMAC-based Ethernet interface.
     *
     * The default arguments obtain the default EMAC, which will be target-
     * dependent (and the target may have some JSON option to choose which
     * is the default, if there are multiple). The default stack is configured
     * by JSON option nsapi.default-stack.
     *
     * Due to inability to return errors from the constructor, no real
     * work is done until the first call to connect().
     *
     * @param emac  Reference to EMAC to use.
     * @param stack Reference to onboard-network stack to use.
     */
    EthernetInterface(EMAC &emac = EMAC::get_default_instance(),
                      OnboardNetworkStack &stack = OnboardNetworkStack::get_default_instance()) : EMACInterface(emac, stack) { }
};

#endif
