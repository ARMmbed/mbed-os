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
#ifdef DEVICE_WIFI_AP
#include "WiFiSoftAPInterface.h"
#endif

#include "mbed.h"
#include "netsocket/WiFiAccessPoint.h"
#include "netsocket/EMACInterface.h"
#include "nsapi_types.h"
#include "lwip/netif.h"
#include "rtos.h"
#include "cb_wlan.h"

#define ODIN_WIFI_MAX_MAC_ADDR_STR  (18)
#define ODIN_WIFI_SCAN_CACHE_SIZE   (5)

struct odin_wifi_msg_s;
struct user_connect_s;
struct user_scan_s;
struct user_ap_start_s;
struct wlan_status_started_s;
struct wlan_status_connected_s;
struct wlan_status_connection_failure_s;
struct wlan_scan_indication_s;

/** OdinWiFiInterface class
 *  Implementation of the WiFiInterface for the ODIN-W2 module
 */
#ifdef DEVICE_WIFI_AP
class OdinWiFiInterface : public WiFiInterface, public WiFiSoftAPInterface, public EMACInterface
#else
class OdinWiFiInterface : public WiFiInterface, public EMACInterface
#endif
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
    virtual nsapi_error_t connect(
        const char *ssid,
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
    virtual nsapi_error_t set_timeout(int ms);

#ifdef DEVICE_WIFI_AP

    /** Set IP config for access point
         *
         *  This function has to be called before the access point is started.
         *
         *  @param gateway     Null-terminated representation of the local gateway
         *  @param netmask     Null-terminated representation of the network mask
         *  @return            0 on success, negative error code on failure
         */
    //TODO: In previous WiFiInterface.h but not in new WiFiSoftAPInterface
    virtual nsapi_error_t set_ap_network(const char *ip_address, const char *netmask, const char *gateway);

    /** Set the WiFi network credentials
     *
     *  @param ssid      Name of the network to connect to
     *  @param pass      Security passphrase to connect to the network
     *  @param security  Type of encryption for connection
     *                   (defaults to NSAPI_SECURITY_NONE)
     *  @return          0 on success, or error code on failure
     */
    virtual nsapi_error_t set_ap_credentials(const char *ssid, const char *pass = 0,
            nsapi_security_t security = NSAPI_SECURITY_NONE);

    /** Set the WiFi network channel
     *
     *  @param channel   Channel on which the connection is to be made, or 0 for any (Default: 0)
     *  @return          0 on success, or error code on failure
     */
    virtual nsapi_error_t set_ap_channel(uint8_t channel);

    /** Gets the current number of active connections
     *
     *  @return         number of active connections
     */
    virtual int get_ap_connection_count();

    /** Gets the max supported number of active connections
     *
     *  @return         maximum number of active connections
     */
    virtual int get_ap_max_connection_count();

    /** Enable or disable DHCP on the network access point
     *
     *  Enables DHCP in SoftAP mode. Defaults to enabled unless
     *  a static IP address has been assigned. Requires that the network is
     *  service stopped.
     *
     *  @param dhcp     True to enable DHCP
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t set_ap_dhcp(bool dhcp);

    /** Set the beacon interval.
     *
     *  Note that the value needs to be set before ap_start in order to take effect.
     *
     *  @param interval  Beason interval in time units (Default: 100 time units = 102.4 ms)
     *  @return          0 on success, or error code on failure
     */
    virtual nsapi_error_t set_ap_beacon_interval(uint16_t interval);

    /** Start the interface
     *
     *  Attempts to serve a WiFi network.
     *
     *  @param ssid      Name of the network to connect to
     *  @param pass      Security passphrase to connect to the network
     *  @param security  Type of encryption for connection (Default: NSAPI_SECURITY_NONE)
     *  @param channel   Channel on which the connection is to be made, or 0 for any (Default: 0)
     *  @return          0 on success, or error code on failure
     */
    virtual nsapi_error_t ap_start(const char *ssid, const char *pass = 0,
            nsapi_security_t security = NSAPI_SECURITY_NONE, uint8_t channel = 0);

    /** Start the interface
     *
     *  Attempts to serve a WiFi network. Requires ssid to be set.
     *  passphrase is optional.
     *  If passphrase is invalid, NSAPI_ERROR_AUTH_ERROR is returned.
     *
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t ap_start();

    /** Stop the interface
     *
     *  @return         0 on success, or error code on failure
     */
    virtual nsapi_error_t ap_stop();   

#endif

private:

    enum OdinWifiState {
        S_NOT_INITIALISED = 1,
        S_WAIT_START,
        S_STARTED,
        S_WAIT_STOP,

        S_STA_IDLE,
        S_STA_WAIT_CONNECT,
        S_STA_CONNECTED,
        S_STA_DISCONNECTED_WAIT_CONNECT,
        S_STA_CONNECTION_FAIL_WAIT_DISCONNECT,
        //S_STA_LINK_LOSS_WAIT_DISCONNECT,
        S_STA_WAIT_DISCONNECT,

        S_AP_IDLE,
        S_AP_WAIT_START,
        S_AP_STARTED,
        S_AP_WAIT_STOP,
        S_AP_FAIL_WAIT_STOP,
        S_AP_WAIT_DRV_STOP,
        S_AP_WAIT_DRV_START,

        S_INVALID
    };

    struct sta_s {
        const char          *ssid;
        const char          *passwd;
        nsapi_security_t    security;
        uint8_t             channel;
        int                 timeout_ms;
    };

    struct ap_s {
        const char          *ssid;
        const char          *passwd;
        nsapi_security_t    security;
        uint8_t             channel;
        bool                use_dhcp;

        char                ip_address[IPADDR_STRLEN_MAX];
        char                netmask[IPADDR_STRLEN_MAX];
        char                gateway[IPADDR_STRLEN_MAX];

        int                 cnt_connected;

        nsapi_error_t       error_code;
        uint16_t            beacon_interval;
    };

    struct scan_cache_s {
        int                 count;
        uint8_t             last_channel;
        cbWLAN_MACAddress   bssid[ODIN_WIFI_SCAN_CACHE_SIZE];
    };

    OdinWifiState entry_connect_fail_wait_disconnect();
    OdinWifiState entry_wait_connect();
    OdinWifiState entry_wait_disconnect();
    //OdinWifiState entry_link_loss_wait_disconnect(void);
    OdinWifiState entry_ap_wait_start();
    OdinWifiState entry_ap_started();
    OdinWifiState entry_ap_wait_stop();
    OdinWifiState entry_ap_fail_wait_stop();
    OdinWifiState entry_ap_wait_drv_stop();
    OdinWifiState entry_ap_wait_drv_start();

    void handle_in_msg();
    void handle_cached_msg();

    void handle_user_connect(user_connect_s *user_connect);
    void handle_user_disconnect();
    void handle_user_scan(user_scan_s *user_scan);
    void handle_user_connect_timeout();
    void handle_user_stop();

    void handle_user_ap_start(user_ap_start_s *user_ap_start);
    void handle_user_ap_stop();

    void handle_wlan_status_started(wlan_status_started_s *start);
    void handle_wlan_status_stopped(void);
    void handle_wlan_status_error(void);
    void handle_wlan_status_connecting(void);
    void handle_wlan_status_connected(wlan_status_connected_s *wlan_connect);
    void handle_wlan_status_connection_failure(wlan_status_connection_failure_s *connect_failure);
    void handle_wlan_status_disconnected(void);
    void handle_wlan_scan_indication();

    void handle_wlan_status_ap_up();
    void handle_wlan_status_ap_down();

    void init(bool debug);
    nsapi_error_t wlan_set_channel(uint8_t channel);
    nsapi_error_t wlan_connect(
            const char          *ssid,
            const char          *passwd,
            nsapi_security_t    security);
    nsapi_error_t wlan_ap_start(
            const char          *ssid,
            const char          *pass,
            nsapi_security_t    security,
            uint8_t             channel,
            uint16_t            beacon_interval);

    void timeout_user_connect();
    void update_scan_list(cbWLAN_ScanIndicationInfo *scan_info);
    void send_user_response_msg(unsigned int type, nsapi_error_t error_code);
    void wlan_status_indication(cbWLAN_StatusIndicationInfo status, void *data);
    void wlan_scan_indication(cbWLAN_ScanIndicationInfo *scan_info, cb_boolean is_last_result);

    static bool                 _wlan_initialized; // Controls that cbWLAN is initiated only once
    static int32_t              _target_id;

    OdinWifiState       _state;
    OdinWifiState       _state_sta;
    OdinWifiState       _state_ap;

    struct sta_s        _sta;
    struct ap_s         _ap;
    char                _mac_addr_str[ODIN_WIFI_MAX_MAC_ADDR_STR];

    cbWLAN_StatusConnectedInfo      _wlan_status_connected_info;
    cbWLAN_StatusDisconnectedInfo   _wlan_status_disconnected_info;

    bool                            _scan_active;
    WiFiAccessPoint                 *_scan_list;
    nsapi_size_t                    _scan_list_size;
    nsapi_size_t                    _scan_list_cnt;
    struct scan_cache_s             _scan_cache;

    friend struct wlan_callb_s;

    Mutex                               _mutex;
    Queue<odin_wifi_msg_s, 6>           _in_queue;
    Queue<odin_wifi_msg_s, 1>           _out_queue;
    Queue<odin_wifi_msg_s, 1>           _cache_queue;
    MemoryPool<odin_wifi_msg_s, 7>      *_msg_pool;
    Thread                              _thread;
    //Timeout                             _timeout; //Randomly lost interrupts/callbacks; replaced by Timer
    Timer                               _timer;

    bool    _debug;
    int     _dbg_timeout;
};

#endif
