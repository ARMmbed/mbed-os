/* WHD implementation of NetworkInterfaceAPI
 * Copyright (c) 2017-2019 ARM Limited
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

#ifndef WHD_STA_INTERFACE_H
#define WHD_STA_INTERFACE_H

#include "netsocket/WiFiInterface.h"
#include "netsocket/EMACInterface.h"
#include "netsocket/OnboardNetworkStack.h"
#include "WhdAccessPoint.h"
#include "whd_emac.h"
#include "whd_interface.h"
#include "whd_types_int.h"

struct ol_desc;

/** WhdSTAInterface class
 *  Implementation of the NetworkStack for the WHD
 */
class WhdSTAInterface : public WiFiInterface, public EMACInterface {
public:
    class OlmInterface {
    public:
        /** Get the default OLM interface. */
        static OlmInterface &get_default_instance();

        OlmInterface(struct ol_desc *list = NULL) {}

        virtual int init_ols(void *whd, void *ip)
        {
            return 0;
        }
        virtual int sleep()
        {
            return 0;
        }
        virtual int wake()
        {
            return 0;
        }

        virtual void deinit_ols(void) {}
    };

    WhdSTAInterface(
        WHD_EMAC &emac = WHD_EMAC::get_instance(),
        OnboardNetworkStack &stack = OnboardNetworkStack::get_default_instance(),
        OlmInterface &olm = OlmInterface::get_default_instance(),
        whd_interface_shared_info_t &shared = whd_iface_shared);

    static WhdSTAInterface *get_default_instance();

    /* Turn on the wifi device*/
    void wifi_on();

    /** Start the interface
     *
     *  Attempts to connect to a WiFi network. Requires ssid and passphrase to be set.
     *  If passphrase is invalid, NSAPI_ERROR_AUTH_ERROR is returned.
     *
     *  @return         0 on success, negative error code on failure
     */
    nsapi_error_t connect();

    /** Start the interface
     *
     *  Attempts to connect to a WiFi network.
     *
     *  @param ssid      Name of the network to connect to
     *  @param pass      Security passphrase to connect to the network
     *  @param security  Type of encryption for connection (Default: NSAPI_SECURITY_NONE)
     *  @param channel   This parameter is not supported, setting it to anything else than 0 will result in NSAPI_ERROR_UNSUPPORTED
     *  @return          0 on success, or error code on failure
     */
    nsapi_error_t connect(const char *ssid, const char *pass, nsapi_security_t security = NSAPI_SECURITY_NONE, uint8_t channel = 0);

    /** Stop the interface
     *  @return             0 on success, negative on failure
     */
    nsapi_error_t disconnect();

    /** Set the WiFi network credentials
     *
     *  @param ssid      Name of the network to connect to
     *  @param pass      Security passphrase to connect to the network
     *  @param security  Type of encryption for connection
     *                   (defaults to NSAPI_SECURITY_NONE)
     *  @return          0 on success, or error code on failure
     */
    nsapi_error_t set_credentials(const char *ssid, const char *pass, nsapi_security_t security = NSAPI_SECURITY_NONE);

    /** Set the WiFi network channel - NOT SUPPORTED
     *
     * This function is not supported and will return NSAPI_ERROR_UNSUPPORTED
     *
     *  @param channel   Channel on which the connection is to be made, or 0 for any (Default: 0)
     *  @return          Not supported, returns NSAPI_ERROR_UNSUPPORTED
     */
    nsapi_error_t set_channel(uint8_t channel)
    {
        if (channel != 0) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        return 0;
    }

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

    /** Gets the current radio signal strength for active connection
     *
     * @return          Connection strength in dBm (negative value)
     */
    int8_t get_rssi();

    /** Scan for available networks in WiFiAccessPoint format
     *
     * This function will block.
     *
     * @param  ap       Pointer to allocated array of WiFiAccessPoint format for discovered AP
     * @param  count    Size of allocated @a res array, or 0 to only count available AP
     * @param  timeout  Timeout in milliseconds; 0 for no timeout (Default: 0)
     * @return          Number of entries in @a, or if @a count was 0 number of available networks, negative on error
     *                  see @a nsapi_error
     */
    int scan(WiFiAccessPoint *res, unsigned count);

    /** Scan for available networks in WhdAccessPoint format
     *
     * This function will block.
     *
     * @param  ap       Pointer to allocated array of WhdAccessPoint format for discovered AP
     * @param  count    Size of allocated @a res array, or 0 to only count available AP
     * @param  timeout  Timeout in milliseconds; 0 for no timeout (Default: 0)
     * @return          Number of entries in @a, or if @a count was 0 number of available networks, negative on error
     *                  see @a nsapi_error
     */
    int scan_whd(WhdAccessPoint *res, unsigned count);

    /* is interface connected, if yes return WICED_SUCCESS else WICED_NOT_CONNECTED */
    int is_interface_connected();

    /* get bssid of the AP  if success return WICED_SUCCESS else WICED_ERROR */
    int get_bssid(uint8_t *bssid);

    /* print WHD log (this routine will malloc/free a buffer
     * You need to enable printing with WHD_LOGGING_BUFFER_ENABLE
     */
    int whd_log_print(void);

    /* read WHD log */
    int whd_log_read(char *buffer, int buffer_size);

    /* Get EDCF AC params */
    nsapi_error_t wifi_get_ac_params_sta(void *ac_param);

    /* get iovar value */
    int wifi_get_iovar_value(const char *iovar, uint32_t *value);

    /* set iovar value */
    int wifi_set_iovar_value(const char *iovar, uint32_t value);

    /* set ioctl value */
    int wifi_set_ioctl_value(uint32_t ioctl, uint32_t value) ;

    /* get  ioctl value */
    int wifi_get_ioctl_value(uint32_t ioctl, uint32_t *value);

    /* get ioctl buffer */
    int wifi_get_ioctl_buffer(uint32_t ioctl, uint8_t *buffer, uint16_t len);

    /* set ioctl buffer */
    int wifi_set_ioctl_buffer(uint32_t ioctl, uint8_t *buffer, uint16_t len);

    /* get WHD ifp value */
    int wifi_get_ifp(whd_interface_t *ifp);

    /* set wifi interface up */
    int wifi_set_up(void);

    /* set wifi interface down */
    int wifi_set_down(void);

    /* set wifi coex configuration */
    int wifi_set_coex_config(whd_coex_config_t *coex_config);

    /** Set Offload Manager Information
     *  NOTE: Only allowed while disconnected
     *
     * @param  olm      Offload Manager info structure
     * @return          true if completed successfully
     *                  false if Interface is connected
     */
    int set_olm(OlmInterface *olm)
    {
        if (get_connection_status() == NSAPI_STATUS_DISCONNECTED) {
            _olm = olm;
            return true;
        }
        return false;
    }

    /** Network stack is suspended
     *
     * @return          0 if successful
     */
    int net_suspended()
    {
        int ret = _olm->sleep();
        return ret;
    }

    /** Network stack is resuming
     *
     * @return          0 if successful
     */
    int net_resuming()
    {
        int ret = _olm->wake();
        return ret;
    }

protected:
    int internal_scan(WiFiAccessPoint *aps, unsigned count, scan_result_type sres_type);

private:
    char _ssid[33]; /* The longest possible name (defined in 802.11) +1 for the \0 */
    char _pass[64]; /* The longest allowed passphrase + 1 */
    nsapi_security_t _security;
    WHD_EMAC &_whd_emac;
    OlmInterface *_olm;
    whd_interface_shared_info_t &_iface_shared;
};

#endif
