/* ODIN-W2 implementation of WiFiInterface
 * Copyright (c) 2016 u-blox Malmö AB
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

#ifndef ODIN_WIFI_INTERFACE_H
#define ODIN_WIFI_INTERFACE_H

#include "WiFiInterface.h"
#include "Callback.h"
#include "mbed_events.h"

#include "rtos.h"
#include "cmsis_os.h"
#include "emac_api.h"
#include "nsapi_types.h"
#include "lwip/netif.h"

typedef Queue<void*, 1> MsgQueue;

class OdinWiFiInterface;

struct PrivContext;

/** OdinWiFiInterface class
 *  Implementation of the WiFiInterface for the ODIN-W2 module
 */
class OdinWiFiInterface : public WiFiInterface
{
public:
    /** OdinWiFiInterface lifetime
     */
    OdinWiFiInterface();
    
    OdinWiFiInterface(bool debug);

    ~OdinWiFiInterface();
    
    /** Set the WiFi network credentials
     *
     *  @param ssid      Name of the network to connect to
     *  @param pass      Security passphrase to connect to the network
     *  @param security  Type of encryption for connection
     *                   (defaults to NSAPI_SECURITY_NONE)
     *  @return          0 on success, or error code on failure
     */
    virtual nsapi_error_t set_credentials(const char *ssid, const char *pass, nsapi_security_t security = NSAPI_SECURITY_NONE);
    
    /** Set the WiFi network channel
     *
     *  @param channel   Channel on which the connection is to be made, or 0 for any (Default: 0)
     *  @return          0 on success, or error code on failure
     */
    virtual nsapi_error_t set_channel(uint8_t channel);
    
    /** Start the interface
     *
     *  Attempts to connect to a WiFi network.
     *
     *  @param ssid      Name of the network to connect to
     *  @param pass      Security passphrase to connect to the network
     *  @param security  Type of encryption for connection (Default: NSAPI_SECURITY_NONE)
     *  @param channel   Channel on which the connection is to be made, or 0 for any (Default: 0)
     *  @return          0 on success, or error code on failure
     */
    virtual nsapi_error_t connect(const char *ssid,
        const char *pass,
        nsapi_security_t security = NSAPI_SECURITY_NONE,
        uint8_t channel = 0);

    /** Start the interface
     *
     *  Attempts to connect to a WiFi network. Requires ssid and passphrase to be set.
     *  If passphrase is invalid, NSAPI_ERROR_AUTH_ERROR is returned.
     *
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t connect();

    /** Stop the interface
     *
     *  @return         0 on success, or error code on failure
     */
    virtual nsapi_error_t disconnect();

    /** Get the local MAC address
     *
     *  Provided MAC address is intended for info or debug purposes and
     *  may not be provided if the underlying network interface does not
     *  provide a MAC address
     *  
     *  @return         Null-terminated representation of the local MAC address
     *                  or null if no MAC address is available
     */
    virtual const char *get_mac_address();

    /** Get the local IP address
     *
     *  @return         Null-terminated representation of the local IP address
     *                  or null if no IP address has been recieved
     */
    virtual const char *get_ip_address();

    /** Get the local network mask
     *
     *  @return         Null-terminated representation of the local network mask 
     *                  or null if no network mask has been recieved
     */
    virtual const char *get_netmask();

    /** Get the local gateway
     *
     *  @return         Null-terminated representation of the local gateway
     *                  or null if no network mask has been recieved
     */
    virtual const char *get_gateway();

    /** Set a static IP address
     *
     *  Configures this network interface to use a static IP address.
     *  Implicitly disables DHCP, which can be enabled in set_dhcp.
     *  Requires that the network is disconnected.
     *
     *  @param address  Null-terminated representation of the local IP address
     *  @param netmask  Null-terminated representation of the local network mask
     *  @param gateway  Null-terminated representation of the local gateway
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t set_network(const char *ip_address, const char *netmask, const char *gateway);

    /** Enable or disable DHCP on the network
     *
     *  Enables DHCP on connecting the network. Defaults to enabled unless
     *  a static IP address has been assigned. Requires that the network is
     *  disconnected.
     *
     *  @param dhcp     True to enable DHCP
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t set_dhcp(bool dhcp);
    
    /** Gets the current radio signal strength for active connection
     *
     *  @return         Connection strength in dBm (negative value),
     *                  or 0 if measurement impossible
     */
    virtual int8_t get_rssi();

    /** Scan for available networks
     *
     *  If the network interface is set to non-blocking mode, scan will attempt to scan
     *  for WiFi networks asynchronously and return NSAPI_ERROR_WOULD_BLOCK. If a callback
     *  is attached, the callback will be called when the operation has completed.
     *
     * @param  ap       Pointer to allocated array to store discovered AP
     * @param  count    Size of allocated @a res array, or 0 to only count available AP
     * @param  timeout  Timeout in milliseconds; 0 for no timeout (Default: 0)
     * @return          Number of entries in @a, or if @a count was 0 number of available networks, negative on error
     *                  see @a nsapi_error
     */
    virtual nsapi_size_or_error_t scan(WiFiAccessPoint *res, nsapi_size_t count);

    /** Sets timeout for connection setup. Note that the time for DHCP retrieval is not included.
     *
     *  @param timeout  Timeout in ms. Use 0 for waiting forever. The timeout might take up to X sec longer than
     *                  specified since the Wi-Fi driver might need some time to finish and cleanup.
     *  @return         0 on success, negative error code on failure
     */
    virtual void set_timeout(int timeout);

    virtual NetworkStack *get_stack();

protected:

private:
    
    nsapi_error_t connect_async(const char *ssid,
        const char *pass,
        nsapi_security_t security = NSAPI_SECURITY_NONE,
        uint8_t channel = 0,
        void *data = NULL,
        unsigned timeout = 0);
    
    bool start(bool debug);
    bool stop();
    
    char _mac_addr_str[18];
    // Private context to share between C and C++ calls
    PrivContext* _priv_context;
    const char *_ssid;
    const char *_pass;
    char _ip_address[IPADDR_STRLEN_MAX];
    char _netmask[IPADDR_STRLEN_MAX];
    char _gateway[IPADDR_STRLEN_MAX];
    nsapi_security_t _security;
    uint8_t _channel;
    bool _use_dhcp;
    int _timeout;
    // Event queue when the driver context need to be used
    EventQueue* _odin_event_queue;
    int32_t target_id;
    // Event queue for sending start up and connection events from driver to this class
    MsgQueue _event_queue;
    // Message queue for sending scan events from driver to this class
    osMessageQId _scan_msg_queue_id;
    osMessageQDef_t _queue_def;
};

#endif
