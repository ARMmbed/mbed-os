/* ESP8266Interface Example
 * Copyright (c) 2015 ARM Limited
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

#ifndef ESP8266_H
#define ESP8266_H

#if DEVICE_SERIAL && DEVICE_INTERRUPTIN && defined(MBED_CONF_EVENTS_PRESENT) && defined(MBED_CONF_NSAPI_PRESENT) && defined(MBED_CONF_RTOS_API_PRESENT)
#include <stdint.h>
#include <ctime>

#include "drivers/BufferedSerial.h"
#include "netsocket/nsapi_types.h"
#include "netsocket/WiFiAccessPoint.h"
#include "PinNames.h"
#include "platform/ATCmdParser.h"
#include "platform/Callback.h"
#include "platform/mbed_chrono.h"
#include "platform/mbed_error.h"
#include "rtos/Mutex.h"
#include "rtos/ThisThread.h"
#include "netsocket/SocketAddress.h"

// Various timeouts for different ESP8266 operations
// (some of these can't use literal form as they're needed for defaults in this header, where
// we shouldn't add a using directive for them. Defines only used in the C++ file can use literals).
#ifndef ESP8266_CONNECT_TIMEOUT
#define ESP8266_CONNECT_TIMEOUT 15s
#endif
#ifndef ESP8266_SEND_TIMEOUT
#define ESP8266_SEND_TIMEOUT    2s
#endif
#ifndef ESP8266_RECV_TIMEOUT
#define ESP8266_RECV_TIMEOUT    std::chrono::seconds(2)
#endif
#ifndef ESP8266_MISC_TIMEOUT
#define ESP8266_MISC_TIMEOUT    std::chrono::seconds(2)
#endif
#ifndef ESP8266_DNS_TIMEOUT
#define ESP8266_DNS_TIMEOUT     15s
#endif

#define ESP8266_SCAN_TIME_MIN 0ms
#define ESP8266_SCAN_TIME_MAX 1500ms
#define ESP8266_SCAN_TIME_MIN_DEFAULT 120ms
#define ESP8266_SCAN_TIME_MAX_DEFAULT 360ms

// Firmware version
#define ESP8266_SDK_VERSION 2000000
#define ESP8266_SDK_VERSION_MAJOR ESP8266_SDK_VERSION/1000000

#define ESP8266_AT_VERSION 1000000
#define ESP8266_AT_VERSION_MAJOR ESP8266_AT_VERSION/1000000
#define ESP8266_AT_VERSION_TCP_PASSIVE_MODE 1070000
#define ESP8266_AT_VERSION_WIFI_SCAN_CHANGE 1060000

#define FW_AT_LEAST_VERSION(MAJOR,MINOR,PATCH,NUSED/*Not used*/,REF) \
    (((MAJOR)*1000000+(MINOR)*10000+(PATCH)*100) >= REF ? true : false)

struct esp8266_socket {
    int id;
    nsapi_protocol_t proto;
    bool connected;
    bool bound;
    SocketAddress addr;
    int keepalive; // TCP
};

/** ESP8266Interface class.
    This is an interface to a ESP8266 radio.
 */
class ESP8266 {
public:
    ESP8266(PinName tx, PinName rx, bool debug = false, PinName rts = NC, PinName cts = NC);

    /**
    * ESP8266 firmware SDK version
    *
    * @param major Major version number
    * @param minor Minor version number
    * @param patch Patch version number
    */
    struct fw_sdk_version {
        int major;
        int minor;
        int patch;
        fw_sdk_version(int major, int minor, int patch) : major(major), minor(minor), patch(patch) {}
    };

    /**
    * ESP8266 firmware AT version
    *
    * @param major Major version number
    * @param minor Minor version number
    * @param patch Patch version number
    */
    struct fw_at_version {
        int major;
        int minor;
        int patch;
        fw_at_version(int major, int minor, int patch) : major(major), minor(minor), patch(patch) {}
    };

    /**
    * Check AT command interface of ESP8266
    *
    * @return true if ready to respond on AT commands
    */
    bool at_available(void);

    /**
     * Disable echo - required for OOB processing to work
     *
     * @return true if echo was successfully disabled
     */
    bool echo_off(void);

    /**
    * Check sdk version from which firmware is created
    *
    * @return fw_sdk_version which tells major, minor and patch version
    */
    struct fw_sdk_version sdk_version(void);

    /**
    * Check AT instruction set version from which firmware is created
    *
    * @return fw_at_version which tells major, minor and patch version
    */
    struct fw_at_version at_version(void);

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
    * @return NSAPI_ERROR_OK in success, negative error code in failure
    */
    nsapi_error_t connect(const char *ap, const char *passPhrase);

    /**
    * Disconnect ESP8266 from AP
    *
    * @return true only if ESP8266 is disconnected successfully
    */
    bool disconnect(void);

    /**
    * Enable or disable Remote IP and Port printing with +IPD
    *
    * @param enable, 1 on, 0 off
    * @return true only if ESP8266 is disconnected successfully
    */
    bool ip_info_print(int enable);

    /**
    * Get the IP address of ESP8266
    *
    * @return null-teriminated IP address or null if no IP address is assigned
    */
    const char *ip_addr(void);

    /**
     * Set static IP address, gateway and netmask
     *
     * @param ip IP address to set
     * @param gateway (optional) gateway to set
     * @param netmask (optional) netmask to set
     *
     * @return true if operation was successful and flase otherwise
     */
    bool set_ip_addr(const char *ip, const char *gateway, const char *netmask);

    /**
    * Get the MAC address of ESP8266
    *
    * @return null-terminated MAC address or null if no MAC address is assigned
    */
    const char *mac_addr(void);

    /** Get the local gateway
    *
    *  @return         Null-terminated representation of the local gateway
    *                  or null if no network mask has been recieved
    */
    const char *gateway();

    /** Get the local network mask
     *
     *  @return         Null-terminated representation of the local network mask
     *                  or null if no network mask has been recieved
     */
    const char *netmask();

    /* Return RSSI for active connection
     *
     * @return      Measured RSSI
     */
    int8_t rssi();

    /** Scan mode
     */
    enum scan_mode {
        SCANMODE_ACTIVE = 0, /*!< active mode */
        SCANMODE_PASSIVE = 1 /*!< passive mode */
    };

    /** Scan for available networks
     *
     * @param  ap    Pointer to allocated array to store discovered AP
     * @param  limit Size of allocated @a res array, or 0 to only count available AP
     * @param  t_max Maximum scan time per channel
     * @param  t_min Minimum scan time per channel in active mode, can be omitted in passive mode
     * @return       Number of entries in @a res, or if @a count was 0 number of available networks, negative on error
     *               see @a nsapi_error
     */
    int scan(WiFiAccessPoint *res, unsigned limit, scan_mode mode,
             std::chrono::duration<unsigned, std::milli> t_max,
             std::chrono::duration<unsigned, std::milli> t_min);

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
    * @param udp_mode UDP socket's mode, zero means can't change remote, 1 can change once, 2 can change multiple times
    * @return NSAPI_ERROR_OK in success, negative error code in failure
    */
    nsapi_error_t open_udp(int id, const char *addr, int port, int local_port = 0, int udp_mode = 0);

    /**
    * Open a socketed connection
    *
    * @param type the type of socket to open "UDP" or "TCP"
    * @param id id to give the new socket, valid 0-4
    * @param port port to open connection with
    * @param addr the IP address of the destination
    * @param port the port on the destination
    * @param tcp_keepalive TCP connection's keep alive time, zero means disabled
    * @return NSAPI_ERROR_OK in success, negative error code in failure
    */
    nsapi_error_t open_tcp(int id, const char *addr, int port, int keepalive = 0);

    /**
    * Sends data to an open socket
    *
    * @param id id of socket to send to
    * @param data data to be sent
    * @param amount amount of data to be sent - max 2048
    * @return number of bytes on success, negative error code in failure
    */
    nsapi_size_or_error_t send(int id, const void *data, uint32_t amount);

    /**
    * Receives datagram from an open UDP socket
    *
    * @param id id to receive from
    * @param data placeholder for returned information
    * @param amount number of bytes to be received
    * @return the number of bytes received
    */
    int32_t recv_udp(struct esp8266_socket *socket, void *data, uint32_t amount, mbed::chrono::milliseconds_u32 timeout = ESP8266_RECV_TIMEOUT);

    /**
    * Receives stream data from an open TCP socket
    *
    * @param id id to receive from
    * @param data placeholder for returned information
    * @param amount number of bytes to be received
    * @return the number of bytes received
    */
    int32_t recv_tcp(int id, void *data, uint32_t amount, mbed::chrono::milliseconds_u32 timeout = ESP8266_RECV_TIMEOUT);

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
    void set_timeout(mbed::chrono::milliseconds_u32 timeout = ESP8266_MISC_TIMEOUT);

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
    void sigio(mbed::Callback<void()> func);

    /**
    * Attach a function to call whenever sigio happens in the serial
    *
    * @param obj pointer to the object to call the member function on
    * @param method pointer to the member function to call
    */
    template <typename T, typename M>
    void sigio(T *obj, M method)
    {
        sigio(mbed::Callback<void()>(obj, method));
    }

    /**
    * Attach a function to call whenever network state has changed.
    *
    * @param func A pointer to a void function, or 0 to set as none
    */
    void attach(mbed::Callback<void()> status_cb);

    /**
     * Configure SNTP (Simple Network Time Protocol)
     *
     * @param enable   true to enable SNTP or false to disable it
     * @param timezone timezone offset [-11,13] (0 by default)
     * @param server0  optional parameter indicating the first SNTP server ("cn.ntp.org.cn" by default)
     * @param server1  optional parameter indicating the second SNTP server ("ntp.sjtu.edu.cn" by default)
     * @param server2  optional parameter indicating the third SNTP server ("us.pool.ntp.org" by default)
     *
     * @retval true if successful, false otherwise
     */
    bool set_sntp_config(bool enable, int timezone = 0, const char *server0 = nullptr,
                         const char *server1 = nullptr, const char *server2 = nullptr);

    /**
     * Read out the configuration of SNTP (Simple Network Time Protocol)
     *
     * @param enable   true if SNTP is enabled
     * @param timezone timezone offset [-11,13]
     * @param server0  name of the first SNTP server
     * @param server1  name of the second SNTP server (optional, nullptr if not set)
     * @param server2  name of the third SNTP server (optional, nullptr if not set)
     *
     * @retval true if successful, false otherwise
     */
    bool get_sntp_config(bool *enable, int *timezone, char *server0,
                         char *server1, char *server2);

    /**
     * Read out SNTP time from ESP8266.
     *
     * @param t std::tm structure to be filled in
     * @retval true on success, false otherwise
     *
     * @note ESP8266 must be connected and needs a couple of seconds
     * before returning correct time. It may return 1 Jan 1970 if it is not ready.
     *
     * @note esp8266.sntp-enable must be set to true in mbed_app.json file.
     */
    bool get_sntp_time(std::tm *t);

    template <typename T, typename M>
    void attach(T *obj, M method)
    {
        attach(mbed::Callback<void()>(obj, method));
    }

    /**
     * Read default Wifi mode from flash
     *
     * return Station, SoftAP or SoftAP+Station - 0 on failure
     */
    int8_t default_wifi_mode();

    /**
     * Default Wifi mode written to flash only if changes
     */
    bool set_default_wifi_mode(const int8_t mode);

    /**
     * @param track_ap      if TRUE, sets the county code to be the same as the AP's that ESP is connected to,
     *                      if FALSE the code will not change
     * @param country_code  ISO 3166-1 Alpha-2 coded country code
     * @param channel_start the channel number to start at
     * @param channels      number of channels
     */
    bool set_country_code_policy(bool track_ap, const char *country_code, int channel_start, int channels);

    /** Get the connection status
     *
     *  @return         The connection status according to ConnectionStatusType
     */
    nsapi_connection_status_t connection_status() const;

    /**
     * Start board's and ESP8266's UART flow control
     *
     * @return true if started
     */
    bool start_uart_hw_flow_ctrl();

    /**
     * Stop board's and ESP8266's UART flow control
     *
     * @param board_only    true to apply to board only, false to apply both
     * @return true if started
     */
    bool stop_uart_hw_flow_ctrl(bool board_only = false);

    /*
     * From AT firmware v1.7.0.0 onwards enables TCP passive mode
     */
    bool cond_enable_tcp_passive_mode();

    /**
     * For executing OOB processing on background
     *
     * @param timeout AT parser receive timeout
     * @param if TRUE, process all OOBs instead of only one
     */
    void bg_process_oob(std::chrono::duration<uint32_t, std::milli> timeout, bool all);

    /**
     * Flush the serial port input buffers.
     *
     * If you do HW reset for ESP module, you should
     * flush the input buffers from existing responses
     * from the device.
     */
    void flush();

    static const int8_t WIFIMODE_STATION = 1;
    static const int8_t WIFIMODE_SOFTAP = 2;
    static const int8_t WIFIMODE_STATION_SOFTAP = 3;
    static const int8_t SOCKET_COUNT = 5;

    /**
     * Enables or disables uart input and deep sleep
     *
     * @param lock if TRUE, uart input is enabled and  deep sleep is locked
     * if FALSE, uart input is disabled and  deep sleep is unlocked
     */
    int uart_enable_input(bool lock);

private:
    // FW version
    struct fw_sdk_version _sdk_v;
    struct fw_at_version _at_v;

    // FW version specific settings and functionalities
    bool _tcp_passive;
    int32_t _recv_tcp_passive(int id, void *data, uint32_t amount, std::chrono::duration<uint32_t, std::milli> timeout);
    mbed::Callback<void()> _callback;

    // UART settings
    mbed::BufferedSerial _serial;
    PinName _serial_rts;
    PinName _serial_cts;
    rtos::Mutex _smutex; // Protect serial port access

    // AT Command Parser
    mbed::ATCmdParser _parser;

    // Wifi scan result handling
    bool _recv_ap(nsapi_wifi_ap_t *ap);

    // Socket data buffer
    struct packet {
        struct packet *next;
        int id;
        char remote_ip[16];
        int remote_port;
        uint32_t len; // Remaining length
        uint32_t alloc_len; // Original length
        // data follows
    } *_packets, * *_packets_end;
    void _clear_socket_packets(int id);
    void _clear_socket_sending(int id);
    int _sock_active_id;

    // Memory statistics
    size_t _heap_usage; // (Socket data buffer usage)

    // OOB processing
    void _process_oob(std::chrono::duration<uint32_t, std::milli> timeout, bool all);

    // OOB message handlers
    void _oob_packet_hdlr();
    void _oob_connect_err();
    void _oob_conn_already();
    void _oob_err();
    void _oob_socket0_closed();
    void _oob_socket1_closed();
    void _oob_socket2_closed();
    void _oob_socket3_closed();
    void _oob_socket4_closed();
    void _oob_connection_status();
    void _oob_socket_close_err();
    void _oob_watchdog_reset();
    void _oob_busy();
    void _oob_tcp_data_hdlr();
    void _oob_ready();
    void _oob_scan_results();
    void _oob_send_ok_received();
    void _oob_send_fail_received();

    // OOB state variables
    int _connect_error;
    bool _disconnect;
    bool _fail;
    bool _sock_already;
    bool _closed;
    bool _error;
    bool _busy;
    bool _reset_done;
    int _sock_sending_id;

    // Modem's address info
    char _ip_buffer[16];
    char _gateway_buffer[16];
    char _netmask_buffer[16];
    char _mac_buffer[18];

    // Modem's socket info
    struct _sock_info {
        bool open;
        nsapi_protocol_t proto;
        char *tcp_data;
        int32_t tcp_data_avbl; // Data waiting on modem
        int32_t tcp_data_rcvd;
        bool send_fail;     // Received 'SEND FAIL'. Expect user will close the socket.
    };
    struct _sock_info _sock_i[SOCKET_COUNT];

    // Scan results
    struct _scan_results {
        WiFiAccessPoint *res;
        unsigned limit;
        unsigned cnt;
    };
    struct _scan_results _scan_r;

    // Connection state reporting
    nsapi_connection_status_t _conn_status;
    mbed::Callback<void()> _conn_stat_cb; // ESP8266Interface registered
};
#endif
#endif
