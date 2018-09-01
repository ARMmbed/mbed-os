/* ESP8266Interface Example
 * Copyright (c) 2015 ARM Limited
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

#ifndef ESP8266_H
#define ESP8266_H

#include "ATCmdParser.h"
#include "nsapi_types.h"
#include "rtos.h"

// Various timeouts for different ESP8266 operations
#ifndef ESP8266_CONNECT_TIMEOUT
#define ESP8266_CONNECT_TIMEOUT 15000
#endif
#ifndef ESP8266_SEND_TIMEOUT
#define ESP8266_SEND_TIMEOUT    2000
#endif
#ifndef ESP8266_RECV_TIMEOUT
#define ESP8266_RECV_TIMEOUT    2000
#endif
#ifndef ESP8266_MISC_TIMEOUT
#define ESP8266_MISC_TIMEOUT    2000
#endif

/** ESP8266Interface class.
    This is an interface to a ESP8266 radio.
 */
class ESP8266
{
public:
    ESP8266(PinName tx, PinName rx, bool debug=false);

    /**
    * Check firmware version of ESP8266
    *
    * @return integer firmware version or -1 if firmware query command gives outdated response
    */
    int get_firmware_version(void);
    
    /**
    * Startup the ESP8266
    *
    * @param mode mode of WIFI 1-client, 2-host, 3-both
    * @return true only if ESP8266 was setup correctly
    */
    bool startup(int mode);

    /**
    * Reset ESP8266
    *
    * @return true only if ESP8266 resets successfully
    */
    bool reset(void);

    /**
    * Enable/Disable DHCP
    *
    * @param enabled DHCP enabled when true
    * @param mode mode of DHCP 0-softAP, 1-station, 2-both
    * @return true only if ESP8266 enables/disables DHCP successfully
    */
    bool dhcp(bool enabled, int mode);

    /**
    * Connect ESP8266 to AP
    *
    * @param ap the name of the AP
    * @param passPhrase the password of AP
    * @return NSAPI_ERROR_OK only if ESP8266 is connected successfully
    */
    nsapi_error_t connect(const char *ap, const char *passPhrase);

    /**
    * Disconnect ESP8266 from AP
    *
    * @return true only if ESP8266 is disconnected successfully
    */
    bool disconnect(void);

    /**
    * Get the IP address of ESP8266
    *
    * @return null-teriminated IP address or null if no IP address is assigned
    */
    const char *getIPAddress(void);

    /**
    * Get the MAC address of ESP8266
    *
    * @return null-terminated MAC address or null if no MAC address is assigned
    */
    const char *getMACAddress(void);

     /** Get the local gateway
     *
     *  @return         Null-terminated representation of the local gateway
     *                  or null if no network mask has been recieved
     */
    const char *getGateway();

    /** Get the local network mask
     *
     *  @return         Null-terminated representation of the local network mask 
     *                  or null if no network mask has been recieved
     */
    const char *getNetmask();

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
    int scan(WiFiAccessPoint *res, unsigned limit);
    
    /**Perform a dns query
    *
    * @param name Hostname to resolve
    * @param ip   Buffer to store IP address
    * @return 0 true on success, false on failure
    */
    bool dns_lookup(const char *name, char *ip);

    /**
    * Open a socketed connection
    *
    * @param type the type of socket to open "UDP" or "TCP"
    * @param id id to give the new socket, valid 0-4
    * @param port port to open connection with
    * @param addr the IP address of the destination
    * @param port the port on the destination
    * @param local_port UDP socket's local port, zero means any
    * @return true only if socket opened successfully
    */
    nsapi_error_t open_udp(int id, const char* addr, int port, int local_port = 0);

    /**
    * Open a socketed connection
    *
    * @param type the type of socket to open "UDP" or "TCP"
    * @param id id to give the new socket, valid 0-4
    * @param port port to open connection with
    * @param addr the IP address of the destination
    * @param port the port on the destination
    * @param tcp_keepalive TCP connection's keep alive time, zero means disabled
    * @return true only if socket opened successfully
    */
    bool open_tcp(int id, const char* addr, int port, int keepalive = 0);

    /**
    * Sends data to an open socket
    *
    * @param id id of socket to send to
    * @param data data to be sent
    * @param amount amount of data to be sent - max 1024
    * @return NSAPI_ERROR_OK in success, negative error code in failure
    */
    nsapi_error_t send(int id, const void *data, uint32_t amount);

    /**
    * Receives datagram from an open UDP socket
    *
    * @param id id to receive from
    * @param data placeholder for returned information
    * @param amount number of bytes to be received
    * @return the number of bytes received
    */
    int32_t recv_udp(int id, void *data, uint32_t amount, uint32_t timeout=ESP8266_RECV_TIMEOUT);

    /**
    * Receives stream data from an open TCP socket
    *
    * @param id id to receive from
    * @param data placeholder for returned information
    * @param amount number of bytes to be received
    * @return the number of bytes received
    */
    int32_t recv_tcp(int id, void *data, uint32_t amount, uint32_t timeout=ESP8266_RECV_TIMEOUT);

    /**
    * Closes a socket
    *
    * @param id id of socket to close, valid only 0-4
    * @return true only if socket is closed successfully
    */
    bool close(int id);

    /**
    * Allows timeout to be changed between commands
    *
    * @param timeout_ms timeout of the connection
    */
    void setTimeout(uint32_t timeout_ms=ESP8266_MISC_TIMEOUT);

    /**
    * Checks if data is available
    */
    bool readable();

    /**
    * Checks if data can be written
    */
    bool writeable();

    /**
    * Attach a function to call whenever sigio happens in the serial
    *
    * @param func A pointer to a void function, or 0 to set as none
    */
    void sigio(Callback<void()> func);

    /**
    * Attach a function to call whenever sigio happens in the serial
    *
    * @param obj pointer to the object to call the member function on
    * @param method pointer to the member function to call
    */
    template <typename T, typename M>
    void sigio(T *obj, M method) {
        sigio(Callback<void()>(obj, method));
    }

    /**
    * Attach a function to call whenever network state has changed
    *
    * @param func A pointer to a void function, or 0 to set as none
    */
    void attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb);

    /**
     * Read default Wifi mode from flash
     *
     * return Station, SoftAP or SoftAP+Station - 0 on failure
     */
    int8_t get_default_wifi_mode();

    /**
     * Write default Wifi mode to flash
     */
    bool set_default_wifi_mode(const int8_t mode);

    /** Get the connection status
     *
     *  @return         The connection status according to ConnectionStatusType
     */
    nsapi_connection_status_t get_connection_status() const;

    static const int8_t WIFIMODE_STATION = 1;
    static const int8_t WIFIMODE_SOFTAP = 2;
    static const int8_t WIFIMODE_STATION_SOFTAP = 3;
    static const int8_t SOCKET_COUNT = 5;

private:
    UARTSerial _serial;
    ATCmdParser _parser;
    Mutex _smutex; // Protect serial port access

    struct packet {
        struct packet *next;
        int id;
        uint32_t len;
        // data follows
    } *_packets, **_packets_end;
    void _packet_handler();
    void _connect_error_handler();
    bool recv_ap(nsapi_wifi_ap_t *ap);
    void _oob_socket0_closed_handler();
    void _oob_socket1_closed_handler();
    void _oob_socket2_closed_handler();
    void _oob_socket3_closed_handler();
    void _oob_socket4_closed_handler();
    void _connection_status_handler();
    void _oob_socket_close_error();
    void _clear_socket_packets(int id);

    char _ip_buffer[16];
    char _gateway_buffer[16];
    char _netmask_buffer[16];
    char _mac_buffer[18];

    int _connect_error;
    bool _fail;
    bool _closed;
    int _socket_open[SOCKET_COUNT];
    nsapi_connection_status_t _connection_status;
    Callback<void(nsapi_event_t, intptr_t)> _connection_status_cb;
};

#endif
