/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NANOSTACKETHERNETINTERFACE_H
#define NANOSTACKETHERNETINTERFACE_H

#include "EthInterface.h"
#include "MeshInterfaceNanostack.h"
#include "NanostackEthernetPhy.h"

class Nanostack::EthernetInterface final : public Nanostack::Interface {
public:
    nsapi_error_t bringup(bool dhcp, const char *ip,
                          const char *netmask, const char *gw,
                          nsapi_ip_stack_t stack = DEFAULT_STACK,
                          bool blocking = true) override;
    nsapi_error_t bringdown() override;
    void get_mac_address(uint8_t *buf);
    char *get_mac_address(char *buf, nsapi_size_t buflen) override;

    char *get_interface_name(char *buf) override;
private:
    friend class Nanostack;
    friend class NanostackEthernetInterface;
    EthernetInterface(NanostackEthernetPhy &phy) : Interface(phy) {}
    nsapi_error_t initialize();
protected:
    NanostackEthernetPhy &get_phy() const
    {
        return static_cast<NanostackEthernetPhy &>(Interface::get_phy());
    }
};

/** Ethernet interface for Nanostack.
 *
 * Configure Nanostack to use Ethernet connectivity.
 */
class NanostackEthernetInterface : public InterfaceNanostack, public EthInterface, private mbed::NonCopyable<NanostackEthernetInterface> {
public:
    NanostackEthernetInterface() = default;

    nsapi_error_t initialize(NanostackEthernetPhy *phy);

protected:
    Nanostack::EthernetInterface *get_interface() const
    {
        return static_cast<Nanostack::EthernetInterface *>(_interface);
    }
    nsapi_error_t do_initialize() override;

};

#endif // NANOSTACKETHERNETINTERFACE_H
