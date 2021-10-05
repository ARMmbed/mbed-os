/* EMW3080B implementation of NetworkInterfaceAPI
 * Copyright (c) STMicroelectronics 2021
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

#ifndef EMW3080B_INTERFACE_H
#define EMW3080B_INTERFACE_H

#if defined(MBED_CONF_NSAPI_PRESENT)

#include <inttypes.h>

#include "mbed.h"
#include "mbed_debug.h"
#include "mx_wifi.h"
#include "netsocket/WiFiInterface.h"
#include "netsocket/EMACInterface.h"
#include "netsocket/OnboardNetworkStack.h"
#include "EMW3080B_EMAC.h"


/** EMW3080BInterface class
 *  Implementation of the NetworkStack for the EMW3080B
 */
class EMW3080BInterface : public WiFiInterface, public EMACInterface  {
public:
    EMW3080BInterface(bool debug = MBED_CONF_EMW3080B_WIFI_DEBUG,
                      PinName mosi   = MBED_CONF_EMW3080B_WIFI_MOSI,
                      PinName miso   = MBED_CONF_EMW3080B_WIFI_MISO,
                      PinName sclk   = MBED_CONF_EMW3080B_WIFI_SCLK,
                      PinName nss    = MBED_CONF_EMW3080B_WIFI_NSS,
                      PinName notify = MBED_CONF_EMW3080B_WIFI_NOTIFY,
                      PinName flow   = MBED_CONF_EMW3080B_WIFI_FLOW,
                      PinName reset  = MBED_CONF_EMW3080B_WIFI_RESET,
                      PinName tx     = MBED_CONF_EMW3080B_WIFI_TX,
                      PinName rx     = MBED_CONF_EMW3080B_WIFI_RX,
                      EMAC &emac      = EMW3080B_EMAC::get_instance(),
                      OnboardNetworkStack &stack = OnboardNetworkStack::get_default_instance()
                     );

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

    /** Gets the current radio signal strength for active connection
     *
     * @return          Connection strength in dBm (negative value)
     */
    int8_t get_rssi();

    /** Scan for available networks
     *
     * This function will block.
     *
     * @param  ap       Pointer to allocated array to store discovered AP
     * @param  count    Size of allocated @a res array, or 0 to only count available AP
     * @param  timeout  Timeout in milliseconds; 0 for no timeout (Default: 0)
     * @return          Number of entries in @a, or if @a count was 0 number of available networks, negative on error
     *                  see @a nsapi_error
     */
    int scan(WiFiAccessPoint *res, unsigned count);

    MX_WIFIObject_t MxWifiObj;

private:

    nsapi_security_t emw_sec2nsapi_sec(mwifi_security_t sec)
    {
        nsapi_security_t sec_out;

        switch (sec) {
            case MX_WIFI_SEC_NONE:
                sec_out = NSAPI_SECURITY_NONE;
                break;
            case MX_WIFI_SEC_WEP:
                sec_out = NSAPI_SECURITY_WEP;
                break;
            case MX_WIFI_SEC_WPA_AES:
            case MX_WIFI_SEC_WPA_TKIP:
                sec_out = NSAPI_SECURITY_WPA;
                break;
            case MX_WIFI_SEC_WPA2_AES:
            case MX_WIFI_SEC_WPA2_TKIP:
            case MX_WIFI_SEC_WPA2_MIXED:
                sec_out = NSAPI_SECURITY_WPA2;
                break;
            case MX_WIFI_SEC_AUTO:
                sec_out = NSAPI_SECURITY_WPA_WPA2;
                break;
            default:
                sec_out = NSAPI_SECURITY_WPA_WPA2;
                break;

        }
        return sec_out;
    }

    MX_WIFI_SecurityType_t nsapi_sec2emw_sec(nsapi_security_t sec)
    {
        MX_WIFI_SecurityType_t mx_sec;

        switch (sec) {
            case NSAPI_SECURITY_NONE:
                mx_sec = MX_WIFI_SEC_NONE;
                break;
            case NSAPI_SECURITY_WEP:
                mx_sec =  MX_WIFI_SEC_WEP;
                break;
            case NSAPI_SECURITY_WPA:
                mx_sec =  MX_WIFI_SEC_WPA_AES;
                break;
            case NSAPI_SECURITY_WPA2:
                mx_sec =  MX_WIFI_SEC_WPA2_AES;
                break;
            default:
                mx_sec =  MX_WIFI_SEC_AUTO;
                break;
        }

        return mx_sec;
    }

    /* MXCHIP array size for SCAN is 2KB , so limits number of AP to 20 */
#define      MAX_AP_COUNT    20


    int8_t   IO_Init(uint16_t mode);
    int8_t   IO_DeInit(void);
    void     IO_Delay(uint32_t delayms);
    uint16_t IO_Send(uint8_t *data, uint16_t len);
    uint16_t IO_Receive(uint8_t *buffer, uint16_t buff_size);

    void probe(void);
    void release(void);
    char                     _ssid[33]; /* The longest possible name (defined in 802.11) +1 for the \0 */
    char                     _pass[64]; /* The longest allowed passphrase + 1 */
    mwifi_ap_info_t          _ap_info[MAX_AP_COUNT];
    MX_WIFI_SecurityType_t   _sec;
    volatile  bool          _isConnected;
    Mutex                   _mutex;

    uint8_t _debug_level;
};

#endif /* MBED_CONF_NSAPI_PRESENT */

#endif /* EMW3080B_INTERFACE_H */
