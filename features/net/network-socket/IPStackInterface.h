/* IPStackInterface
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

#ifndef IP_STACK_INTERFACE_H
#define IP_STACK_INTERFACE_H

#include "platform.h"

#if DEVICE_EMAC

#include "emac_api.h"

/**
 * IPStackInterface
 *
 * Abstraction class on top of IP stack, enables WiFiInterface implementation to setup IP stack
 */
class IPStackInterface
{
public:
    /**
     * Brings up the IP stack using provided @a emac interface
     *
     * @param emac Emack backend to use
     */
    virtual void bringup(emac_interface_t *emac);

    /**
     * Brings down the IP stack
     */
    virtual void bringdown();

    /**
     * Sends the dhcp request
     *
     * @param timeout Request timeout in ms
     * @return NSAPI_ERROR_OK in case of success, error code otherwise
     */
    virtual int start_dhcp(uint timeout = 15000);

    /**
     * Starts the interface with static IP
     *
     * @param  ip      Static IP to use (in XYZ.XYZ.XYZ.XYZ format)
     * @param  netmask Network mask to use (in XYZ.XYZ.XYZ.XYZ format)
     * @param  gw      Gateway IP address (in XYZ.XYZ.XYZ.XYZ format)
     * @return         NSAPI_ERROR_OK in case of success, error code otherwise
     */
    virtual int start_static_ip(const char *ip, const char *netmask, const char *gw);

    /**
     * Returns  MAC address
     *
     * @return MAC address in "00:11:22:33:44:55" form
     */
    virtual const char *get_mac_address();

    /**
     * Returns interfaces IP address
     *
     * @return IP address in "10.11.12.13" form
     */
    virtual const char *get_ip_address();
private:
    char _ip_address[16]; //IPv4
};

#endif /* DEVICE_EMAC */

#endif /* IP_STACK_INTERFACE_H */
