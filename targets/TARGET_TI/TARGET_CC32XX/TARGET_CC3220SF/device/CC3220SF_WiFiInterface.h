/* CC3220 implementation of NetworkInterfaceAPI
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

#ifndef CC3220SF_INTERFACE_H
#define CC3220SF_INTERFACE_H

#include "mbed.h"
#include "cc3200_simplelink.h"

#define CC3220SF_SOCKET_COUNT  (MAX_CONCURRENT_ACTIONS)
#define MAX_RECV_PACKET_SIZE   (1544)

/** TI (CC3220SF) interface class
 *  Implementation of the NetworkStack for TI CC3200 Simplelink stack
 */
class CC3220SFInterface: public NetworkStack, public WiFiInterface
{
public:
    /**
     * @brief CC3220SFInterface default constructor
     */

    CC3220SFInterface();

    /** Start the interface
     *  @return          0 on success, negative on failure
     */
    virtual nsapi_error_t connect();

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
     virtual nsapi_error_t connect(const char *ssid, const char *pass,
                         nsapi_security_t security = NSAPI_SECURITY_NONE,
                         uint8_t channel = 0);

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
       *
       *  @param channel   Channel on which the connection is to be made, or 0 for any (Default: 0)
       *  @return          0 on success, or error code on failure
       */
     virtual nsapi_error_t set_channel(uint8_t channel);

     /** Stop the interface
       *  @return             0 on success, negative on failure
       */
     virtual int disconnect();

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
    //virtual nsapi_error_t set_network(const char *ip_address, const char *netmask, const char *gateway);

    /** Enable or disable DHCP on the network
     *
     *  Requires that the network is disconnected
     *
     *  @param dhcp     False to disable dhcp (defaults to enabled)
     *  @return         0 on success, negative error code on failure
     */
    //virtual nsapi_error_t set_dhcp(bool dhcp);

    /** Get the internally stored IP address
         *  @return             IP address of the interface or null if not yet connected
         */
    virtual const char *get_ip_address();

    /** Get the internally stored MAC address
     *  @return             MAC address of the interface
     */
    virtual const char *get_mac_address();

     /** Get the local gateway
     *
     *  @return         Null-terminated representation of the local gateway
     *                  or null if no network mask has been recieved
     */
    virtual const char *get_gateway();

    /** Get the local network mask
     *
     *  @return         Null-terminated representation of the local network mask
     *                  or null if no network mask has been recieved
     */
    virtual const char *get_netmask();

    /** Gets the current radio signal strength for active connection
     *
     * @return          Connection strength in dBm (negative value)
     */
    virtual int8_t get_rssi();

    /** Scan for available networks
     *
     *  The scan will
     *  If the network interface is set to non-blocking mode, scan will attempt to scan
     *  for WiFi networks asynchronously and return NSAPI_ERROR_WOULD_BLOCK. If a callback
     *  is attached, the callback will be called when the operation has completed.
     *  @param  res      Pointer to allocated array to store discovered AP
     *  @param  count    Size of allocated @a res array, or 0 to only count available AP
     *  @return          Number of entries in @a, or if @a count was 0 number of available networks, negative on error
     *  see @a nsapi_error
     */
     virtual nsapi_size_or_error_t scan(WiFiAccessPoint *res, unsigned count);


    /** Translates a hostname to an IP address with specific version
     *
     *  The hostname may be either a domain name or an IP address. If the
     *  hostname is an IP address, no network transactions will be performed.
     *
     *  If no stack-specific DNS resolution is provided, the hostname
     *  will be resolve using a UDP socket on the stack.
     *
     *  @param address  Destination for the host SocketAddress
     *  @param host     Hostname to resolve
     *  @param version  IP version of address to resolve, NSAPI_UNSPEC indicates
     *                  version is chosen by the stack (defaults to NSAPI_UNSPEC)
     *  @return         0 on success, negative error code on failure
     */
    using NetworkInterface::gethostbyname;
    //virtual nsapi_error_t gethostbyname(const char *name, SocketAddress *address, nsapi_version_t version);

    /** Add a domain name server to list of servers to query
     *
     *  @param addr     Destination for the host address
     *  @return         0 on success, negative error code on failure
     */
    using NetworkInterface::add_dns_server;

    virtual nsapi_error_t get_dns_server(int index, SocketAddress *address, const char* interface_name = NULL);

    /** Set socket options
     *
     *  The setsockopt allow an application to pass stack-specific hints
     *  to the underlying stack. For unsupported options,
     *  NSAPI_ERROR_UNSUPPORTED is returned and the socket is unmodified.
     *
     *  @param handle   Socket handle
     *  @param level    Stack-specific protocol level
     *  @param optname  Stack-specific option identifier
     *  @param optval   Option value
     *  @param optlen   Length of the option value
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t setsockopt(nsapi_socket_t handle, int level,
            int optname, const void *optval, unsigned optlen);

    /** Get socket options
     *
     *  getsockopt allows an application to retrieve stack-specific options
     *  from the underlying stack using stack-specific level and option names,
     *  or to request generic options using levels from nsapi_socket_level_t.
     *
     *  For unsupported options, NSAPI_ERROR_UNSUPPORTED is returned
     *  and the socket is unmodified.
     *
     *  @param level    Stack-specific protocol level or nsapi_socket_level_t
     *  @param optname  Level-specific option name
     *  @param optval   Destination for option value
     *  @param optlen   Length of the option value
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t getsockopt(nsapi_socket_t handle, int level, int optname,
            void *optval, unsigned *optlen);

    /** Register callback for status reporting
     *
     *  The specified status callback function will be called on status changes
     *  on the network. The parameters on the callback are the event type and
     *  event-type dependent reason parameter.
     *
     *  @param status_cb The callback for status changes
     */
    virtual void attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb);

    /** Get the connection status
     *
     *  @return         The connection status according to ConnectionStatusType
     */
    virtual nsapi_connection_status_t get_connection_status() const;

protected:
    /** Open a socket
     *  @param handle       Handle in which to store new socket
     *  @param proto        Type of socket to open, NSAPI_TCP or NSAPI_UDP
     *  @return             0 on success, negative on failure
     */
    virtual int socket_open(void **handle, nsapi_protocol_t proto);

    /** Close the socket
     *  @param handle       Socket handle
     *  @return             0 on success, negative on failure
     *  @note On failure, any memory associated with the socket must still
     *        be cleaned up
     */
    virtual int socket_close(void *handle);

    /** Bind a server socket to a specific port
     *  @param handle       Socket handle
     *  @param address      Local address to listen for incoming connections on
     *  @return             0 on success, negative on failure.
     */
    virtual int socket_bind(void *handle, const SocketAddress &address);

    /** Start listening for incoming connections
     *  @param handle       Socket handle
     *  @param backlog      Number of pending connections that can be queued up at any
     *                      one time [Default: 1]
     *  @return             0 on success, negative on failure
     */
    virtual int socket_listen(void *handle, int backlog);

    /** Connects this TCP socket to the server
     *  @param handle       Socket handle
     *  @param address      SocketAddress to connect to
     *  @return             0 on success, negative on failure
     */
    virtual int socket_connect(void *handle, const SocketAddress &address);

    /** Accept a new connection.
     *  @param handle       Handle in which to store new socket
     *  @param server       Socket handle to server to accept from
     *  @return             0 on success, negative on failure
     *  @note This call is not-blocking, if this call would block, must
     *        immediately return NSAPI_ERROR_WOULD_WAIT
     */
    virtual int socket_accept(void *handle, void **socket, SocketAddress *address);

    /** Send data to the remote host
     *  @param handle       Socket handle
     *  @param data         The buffer to send to the host
     *  @param size         The length of the buffer to send
     *  @return             Number of written bytes on success, negative on failure
     *  @note This call is not-blocking, if this call would block, must
     *        immediately return NSAPI_ERROR_WOULD_WAIT
     */
    virtual int socket_send(void *handle, const void *data, unsigned size);

    /** Receive data from the remote host
     *  @param handle       Socket handle
     *  @param data         The buffer in which to store the data received from the host
     *  @param size         The maximum length of the buffer
     *  @return             Number of received bytes on success, negative on failure
     *  @note This call is not-blocking, if this call would block, must
     *        immediately return NSAPI_ERROR_WOULD_WAIT
     */
    virtual int socket_recv(void *handle, void *data, unsigned size);

    /** Send a packet to a remote endpoint
     *  @param handle       Socket handle
     *  @param address      The remote SocketAddress
     *  @param data         The packet to be sent
     *  @param size         The length of the packet to be sent
     *  @return             The number of written bytes on success, negative on failure
     *  @note This call is not-blocking, if this call would block, must
     *        immediately return NSAPI_ERROR_WOULD_WAIT
     */
    virtual int socket_sendto(void *handle, const SocketAddress &address, const void *data, unsigned size);

    /** Receive a packet from a remote endpoint
     *  @param handle       Socket handle
     *  @param address      Destination for the remote SocketAddress or null
     *  @param buffer       The buffer for storing the incoming packet data
     *                      If a packet is too long to fit in the supplied buffer,
     *                      excess bytes are discarded
     *  @param size         The length of the buffer
     *  @return             The number of received bytes on success, negative on failure
     *  @note This call is not-blocking, if this call would block, must
     *        immediately return NSAPI_ERROR_WOULD_WAIT
     */
    virtual int socket_recvfrom(void *handle, SocketAddress *address, void *buffer, unsigned size);

    /** Register a callback on state change of the socket
     *  @param handle       Socket handle
     *  @param callback     Function to call on state change
     *  @param data         Argument to pass to callback
     *  @note Callback may be called in an interrupt context.
     */
    virtual void socket_attach(void *handle, void (*callback)(void *), void *data);

    /** Provide access to the NetworkStack object
     *
     *  @return The underlying NetworkStack object
     */
    virtual NetworkStack *get_stack()
    {
        return this;
    }

private:
    static const int CC3220SF_SSID_MAX_LENGTH = 32; /* 32 is what 802.11 defines as longest possible name */
    static const int CC3220SF_PASSPHRASE_MAX_LENGTH = 63; /* The longest allowed passphrase */
    static const int CC3220SF_PASSPHRASE_MIN_LENGTH = 8; /* The shortest allowed passphrase */

    struct {
        void (*callback)(void *);
        void *data;
    } _cbs[CC3220SF_SOCKET_COUNT];

    struct cc3200_socket {
        int id;
        int sd;
        nsapi_protocol_t proto;
        volatile bool connected;
        char read_data_internal_buffer[MAX_RECV_PACKET_SIZE];
        volatile uint32_t read_data_available;
        uint32_t data_to_read;
        bool callback_pending;
        SocketAddress addr;
    };
    bool _id_in_use[CC3220SF_SOCKET_COUNT];
    void * _socket_obj[CC3220SF_SOCKET_COUNT]; // store addresses of socket handles
    Mutex _mutex;
    Thread _thread_read_socket;
    CC3200_SIMPLELINK _cc3200_simplelink;
    int _initialized;
    int _started;
    char _ssid[CC3220SF_SSID_MAX_LENGTH + 1]; /* 32 is what 802.11 defines as longest possible name; +1 for the \0 */
    nsapi_security_t _security;
    uint8_t _channel;
    char _pass[CC3220SF_PASSPHRASE_MAX_LENGTH + 1];

    void _socket_background_thread();
    int _socket_receive_data(void *obj, void *data);
    nsapi_error_t _init(void);
    nsapi_error_t _startup(const int8_t wifi_mode);
    Semaphore _connect_sem;
    Semaphore _ip_set_sem;
    static CC3220SFInterface * cc3200sf_wifi_instance;
    friend void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent);
    friend void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent);
};
#endif
