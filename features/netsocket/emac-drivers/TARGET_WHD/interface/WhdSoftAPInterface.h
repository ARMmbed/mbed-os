/*
 * Copyright (c) 2018-2019 ARM Limited
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


#ifndef WHD_SOFTAP_INTERFACE_H
#define WHD_SOFTAP_INTERFACE_H

#include "netsocket/EMACInterface.h"
#include "netsocket/OnboardNetworkStack.h"
#include "whd_emac.h"
#include "CyDhcpServer.h"
#include "whd_interface.h"
#include <memory>

/**
 * Vendor IE details
 */
typedef struct {
    uint8_t         oui[WIFI_IE_OUI_LENGTH];     /**< Unique identifier for the IE */
    uint8_t         subtype;                     /**< Sub-type of the IE */
    void           *data;                        /**< Pointer to IE data */
    uint16_t        length;                      /**< IE data length */
    uint16_t        which_packets;               /**< Mask of the packet in which this IE details to be included */
} whd_custom_ie_info_t;



/** WhdSoftAPInterface class
 *  Implementation of the SoftAPInterface for the Whd
 */
class WhdSoftAPInterface : public EMACInterface {
public:
    /** Construct SoftAP interface
     *  @return  pointer to default WhdSoftAPInterface instance
     */
    WhdSoftAPInterface(WHD_EMAC &emac = WHD_EMAC::get_instance(WHD_AP_ROLE),
                       OnboardNetworkStack &stack = OnboardNetworkStack::get_default_instance(),
                       whd_interface_shared_info_t &shared = whd_iface_shared);

    /** Get the default WhdSoftAPInterface instance.
     *  @return  pointer to default WhdSoftAPInterface instance
     */
    static WhdSoftAPInterface *get_default_instance();

    /** Set static IP address for SoftAP
     *
     *  Configures the static IP address of SoftAP
     *  Requires that the network is stopped.
     *
     *  @param ip_address  Null-terminated representation of the local IP address
     *  @param netmask     Null-terminated representation of the local network mask
     *  @param gateway     Null-terminated representation of the local gateway
     *  @return            0 on success, negative error code on failure
     * int set_network(const char *ip_address, const char *netmask, const char *gateway);
     */

    /** Start a SoftAP
     *
     *  @param ssid                 Name of the SoftAP to create
     *  @param pass                 Security passphrase for connection to SoftAP
     *  @param security             Type of encryption for connection
     *  @param channel              Channel for SoftAP
     *  @param start_dhcp_server    Start dhcp server for connection
     *  @param whd_custom_ie        Optional Custom IE
     *  @param ap_sta_concur        Enable STA+AP concurrency mode
     *
     *  @return          0 on success, or error code on failure
     *                  see @a nsapi_error
     */
    int start(const char *ssid, const char *pass, nsapi_security_t security, uint8_t channel,
              bool start_dhcp_server = true, const whd_custom_ie_info_t *ie_info = NULL, bool ap_sta_concur = false);

    /**
     * Remove Wi-Fi custom IE
     *
     * @param[in] ie_info   : Pointer to the structure which contains custom IE information
     *
     *  @return          0 on success, or error code on failure
     *                  see @a nsapi_error
     */
    int remove_custom_ie(const whd_custom_ie_info_t *ie_info);

    /** Stop the Software Access Point
     *
     *  @return          0 on success, or error code on failure
     *                  see @a nsapi_error
     */
    int stop(void);

    /**
     * Gets information about associated clients.
     *
     * @note Only applicable if softAP interface is up
     *
     * @param[out] client_list_buffer : pointer to a buffer that will be populated with a variable length structure defined by @ref whd_maclist_t
     * @param[in]  buffer_length      : length of the buffer
     *
     *  @return          0 on success, or error code on failure
     *                  see @a nsapi_error
     */
    int get_associated_client_list(void *client_list_buffer, uint16_t buffer_length);


    /**
     * Register soft AP event handler
     *
     * @param[in] softap_event_handler  : A function pointer to the event handler
     *
     *  @return          0 on success, or error code on failure
     *                  see @a nsapi_error
     */
    int register_event_handler(whd_event_handler_t softap_event_handler);

    /**
     * Unregister soft AP event handler
     *
     *  @return          0 on success, or error code on failure
     *                  see @a nsapi_error
     */
    int unregister_event_handler(void);

    /** Set blocking status of interface. 
     *  Nonblocking mode unsupported.
     *
     *  @param blocking true if connect is blocking
     *  @return         0 on success, negative error code on failure
     */
    nsapi_error_t set_blocking(bool blocking)
    {
        if (blocking) {
            _blocking = blocking;
            return NSAPI_ERROR_OK;
        } else {
            return NSAPI_ERROR_UNSUPPORTED;
        }
    }

protected:
    WHD_EMAC &_whd_emac;
    std::unique_ptr<CyDhcpServer> _dhcp_server;
    whd_interface_shared_info_t &_iface_shared;
};

#endif
