/* LWIPIPStackInterface
 * Copyright (c) 2015 - 2016 ARM Limited
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

#include "platform.h"

#if DEVICE_EMAC

#include "IPStackInterface.h"
#include "lwip_stack.h"

/** LWIP specific implementation of IPStackInterface */

void IPStackInterface::bringup(emac_interface_t *emac)
{
    // no dhcp at this stage, neither other details
    lwip_bringup(emac, false, 0, 0, 0);
}

void IPStackInterface::bringdown()
{
    lwip_bringdown();
}

int IPStackInterface::start_dhcp(uint timeout)
{
    return lwip_start_dhcp(timeout);
}

int IPStackInterface::start_static_ip(const char *ip, const char *netmask, const char *gw)
{
    return lwip_start_static_ip(ip, netmask, gw);
}

const char * IPStackInterface::get_mac_address()
{
    return lwip_get_mac_address();
}

const char * IPStackInterface::get_ip_address()
{
    return lwip_get_ip_address(_ip_address, sizeof(_ip_address));
}

#endif /* DEVICE_EMAC */
