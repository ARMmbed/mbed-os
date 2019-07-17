
/* CC3200_SIMPLELINK Class
 * Copyright (c) 2018 ARM Limited
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

#ifndef CC3200_SIMPLELINK_H
#define CC3200_SIMPLELINK_H
#include "mbed.h"
#include <assert.h>
#include "nsapi_types.h"
#include "nsapi.h"
#include "rtos.h"
#include "netsocket/WiFiAccessPoint.h"
#include "ti/drivers/net/wifi/netcfg.h"
#include "ti/drivers/ti_SPI.h"
#include "ti/drivers/dma/UDMACC32XX.h"
#include "ti/drivers/net/wifi/sl_socket.h"
#include "ti/drivers/net/wifi/netapp.h"
#include "ti/drivers/net/wifi/wlan.h"

#define MAX_SCAN_TIMEOUT        (15000)
#define MAX_SCAN_ENTRIES        (15)
#define MAX_SCAN_ATTEMPTS       (10)

#ifndef CC3200_SIMPLELINK_MISC_TIMEOUT
#define CC3200_SIMPLELINK_MISC_TIMEOUT    2000
#endif

/**
 * This is the interface class to CC3200 Simplelink
 */
class CC3200_SIMPLELINK
{
public:
    CC3200_SIMPLELINK();

	/**
    * Initialize CC3200_SIMPLELINK to default state (STATION mode)
    */
    nsapi_error_t initialize();

	/**
    * Startup the CC3200_SIMPLELINK
    *
    * @param role role of WIFI 0-Station, 2-AP, 3-P2P
    * @return true only if it was setup correctly
    */
    bool startup(int role);

    /**
    * Reset CC3200_SIMPLELINK
    *
    * @return true only if CC3200_SIMPLELINK resets successfully
    */
    //bool reset(void);

    /**
    * Enable/Disable DHCP
    *
    * @param enabled DHCP enabled when true
    * @param mode mode of DHCP 0-softAP, 1-station, 2-both
    * @return true only if CC3200_SIMPLELINK enables/disables DHCP successfully
    */
    bool dhcp(bool enabled, int mode);

    /**
    * Connect CC3200 SimpleLink to AP
    *
    * @param ap the name of the AP
    * @param passPhrase the password of AP
    * @param security security type
    * @return NSAPI_ERROR_OK only if CC3200 SimpleLink is connected successfully
    */
    nsapi_error_t connect(const char *ap, const char *passPhrase, nsapi_security_t security);

    /**
    * Disconnect CC3200 SimpleLink from AP
    *
    * @return true only if CC3200_SIMPLELINK is disconnected successfully
    */
    bool disconnect(void);

    /**
    * Get the IP address of CC3200 SimpleLink
    *
    * @return null-teriminated IP address or null if no IP address is assigned
    */
    const char *getIPAddress(void);

    /**
    * Get the MAC address of CC3200 SimpleLink
    *
    * @return null-terminated MAC address or null if no MAC address is assigned
    */
    const char *getMACAddress(void);

     /** Get the local gateway
     *
     *  @return         Null-terminated representation of the local gateway
     *                  or null if no network mask has been received
     */
    const char *getGateway();

    /** Get the local network mask
     *
     *  @return         Null-terminated representation of the local network mask
     *                  or null if no network mask has been recieved
     */
    const char *getNetmask();

    /** Get DNS server address
     *
     *  @param  ipaddr    Pointer to allocated array to store DNS server raw address
     *  @param  len       Size of the storage area
     *  @return           NSAPI_ERROR_OK on success or other error codes on failure
     */
    nsapi_error_t getDNS(unsigned char* ip4addr, uint32_t len);

    /* Return RSSI for active connection
     *
     * @return      Measured RSSI
     */
    int8_t getRSSI();

    /** Scan for available networks
     *
     * @param  ap    Pointer to allocated array to store discovered AP
     * @param  limit Size of allocated @a res array, or 0 to only count available AP
     * @return       Number of entries in @a res, or if @a count was 0 number of available networks, negative on error
     *               see @a nsapi_error
     */
    int scan(WiFiAccessPoint *res, unsigned count);

    /** Set channel
     *
     * @param  channel    channel to set
     * @return            NSAPI error code
     */
    nsapi_error_t set_channel(int channel);

    /**Perform a dns query
    *
    * @param name Hostname to resolve
    * @param ip   Buffer to store IP address
    * @return     NSAPI error code
    */
    nsapi_error_t dns_lookup(const char *name, char *ip, uint32_t ip_size, nsapi_version_t version);

    /**
    * Open a socket
    *
    * @return handle of the opened socket only if socket opened successfully, 0 or error otherwise
    */
    nsapi_error_t open_socket(nsapi_protocol_t proto);

    /**
    * Close a socket
    *
    * @return true if socket was closed successfully, false otherwise
    */
    bool close_socket(uint32_t sock);

    /**
    * Set socket options
    *
    * @param sd socket descritor
    * @param level protocol level for this option
    * @param optname option name to interrogate
    * @param optval points to the value for the option
    * @param optlen length of the option value
    * @return nsapi status
    */
    nsapi_error_t setsockopt(uint32_t sd, int level,
            int optname, const void *optval, unsigned optlen);

    /**
    * Get socket options
    *
    * @param sd socket descriptor
    * @param level protocol level for this option
    * @param optname option name to interrogate
    * @param optval points to the value for the option
    * @param optlen length of the option value
    * @return nsapi status
    */
    nsapi_error_t getsockopt(uint32_t sd, int level,
            int optname, void *optval, unsigned* optlen);
    /**
    * Initiate a connection on a socket (blocking)
    *
    * @param sd          socket handle
    * @param sock_addr   SocketAddress structure
    * @return nsapi status
    */
    nsapi_error_t connect_socket(uint32_t sd, const SocketAddress &sock_addr);

    /**
    * Assigns a socket an local address
    *
    * @param sd    socket handle
    * @param sock_addr   SocketAddress structure
    *
    * @return nsapi status
    */
    nsapi_error_t bind_socket(uint32_t sd, const SocketAddress &sock_addr);

    /**
    * Write data to socket Initiate a connection on a socket (nonblocking)
    *
    * @param sd       socket handle
    * @param buf      pointer to a buffer containing the data to be sent
    * @param bufLen   data length in bytes
    * @param sock_addr   SocketAddress structure
    * @return a socket handle
    */
    int sendto_socket(uint32_t sd, const void * buf, uint32_t bufLen, const SocketAddress &sock_addr);

    /**
    * Sends data to an open socket (nonblocking)
    *
    * @param sd sd of socket to send to
    * @param data data to be sent
    * @param size number of bytes to be sent - max 1024
    * @return number of sent bytes on success, negative error code on failure
    */
    int32_t send(int id, const void *data, uint32_t size);

    /**
    * Receives stream data from an open TCP socket (nonblocking)
    *
    * @param sd sd to receive from
    * @param data placeholder for returned information
    * @param size number of bytes to be received
    * @return the number of bytes received
    */
    int32_t recv(int sd, void *data, uint32_t size);

    /**
    * Receives data from socket
    *
    * @param sd sd to receive from
    * @param buf placeholder for returned information
    * @param size number of bytes to be received
    * @param sock_addr   SocketAddress structure
    * @return the number of bytes received
    */
    int32_t recvfrom(uint32_t sd, void * buf, uint32_t size, SocketAddress &sock_addr);

    /**
    * Allows timeout to be changed between commands
    *
    * @param timeout_ms timeout of the connection
    */
    void setTimeout(uint32_t timeout_ms=CC3200_SIMPLELINK_MISC_TIMEOUT);

    /**
    * Attach a function to call whenever network state has changed
    *
    * @param func A pointer to a void function, or 0 to set as none
    */
    void attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb);

    /**
     * Read default Wifi role
     *
     * return Station, AP or P2P
     */
    int8_t get_current_wifi_mode();

    /**
     * Write default Wifi
     */
    //bool set_default_wifi_mode(const int8_t mode);

    /** Get the connection status
     *
     *  @return         The connection status according to ConnectionStatusType
     */
    nsapi_connection_status_t get_connection_status() const;

    /** Set the connection status
     *
     *  @param status   The connection status to set
     */
    void set_connection_status(nsapi_connection_status_t status);

    static const int8_t WIFI_ROLE_STATION = 1;
    static const int8_t WIFI_ROLE_AP = 2;
    static const int8_t WIFI_ROLE_P2P = 3;

private:

    typedef union{
        SlSockAddrIn6_t     in6;   /* Socket info for Ipv6 */
        SlSockAddrIn_t      in4;   /* Socket info for Ipv4 */
    }sockAddr_t;

    char _ip_buffer[NSAPI_IPv4_SIZE];
    char _gateway_buffer[NSAPI_IPv4_SIZE];
    char _netmask_buffer[NSAPI_IPv4_SIZE];
    char _mac_buffer[NSAPI_MAC_SIZE];
    char _dns_server_buffer[NSAPI_IPv4_SIZE];

    int _connect_error;
    bool _initialized;
    int  _current_role;
    bool _fail;
    bool _closed;
    nsapi_connection_status_t _connection_status;
    mbed::Callback<void(nsapi_event_t, intptr_t)> _connection_status_cb;
    char _connected_ssid[SL_WLAN_SSID_MAX_LENGTH+1]; /* 32 is what 802.11 defines as longest possible name; +1 for the \0 */
    int _connected_channel;
    SlWlanNetworkEntry_t netEntries[MAX_SCAN_ENTRIES];
    int _timeout;
    void _fill_sl_address(sockAddr_t &sAddr, SlSockAddr_t* &sa, int32_t &addrSize, const SocketAddress &sock_addr);
};
#endif /* CC3200_SIMPLELINK_H_ */

