/* NetworkStack
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

#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include "netsocket/nsapi_types.h"
#include "netsocket/SocketAddress.h"
#include "Callback.h"
#include "DNS.h"


// Predeclared classes
class NetworkStack;
class EthInterface;
class WiFiInterface;
class MeshInterface;
class CellularBase;
class EMACInterface;

/** NetworkInterface class
 *
 *  Common interface that is shared between network devices
 *  @addtogroup netsocket
 */
class NetworkInterface: public DNS {
public:

    virtual ~NetworkInterface() {};

    /** Return the default network interface
     *
     * Returns the default network interface, as determined by JSON option
     * target.network-default-interface-type or other overrides.
     *
     * The type of the interface returned can be tested via the ethInterface()
     * etc downcasts.
     *
     * The default behaviour is to return the default interface for the
     * interface type specified by target.network-default-interface-type. Targets
     * should set this in their targets.json to guide default selection,
     * and applications may override.
     *
     * The interface returned should be already configured for use such that its
     * connect() method works with no parameters. For connection types needing
     * configuration, settings should normally be obtained from JSON - the
     * settings for the core types are under the "nsapi" JSON config tree.
     *
     * The list of possible settings for default interface type is open-ended,
     * as is the number of possible providers. Core providers are:
     *
     * * ETHERNET: EthernetInterface, using default EMAC and OnboardNetworkStack
     * * MESH: ThreadInterface or LoWPANNDInterface, using default NanostackRfPhy
     * * CELLULAR: OnboardModemInterface
     * * WIFI: None - always provided by a specific class
     *
     * Specific drivers may be activated by other settings of the
     * default-network-interface-type configuration.  This will depend on the
     * target and the driver. For example a board may have its default setting
     * as "AUTO" which causes it to autodetect an Ethernet cable. This should
     * be described in the target's documentation.
     *
     * An application can override all target settings by implementing
     * NetworkInterface::get_default_instance() themselves - the default
     * definition is weak, and calls get_target_default_instance().
     */
    static NetworkInterface *get_default_instance();

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
     *                  or null if no IP address has been received
     */
    virtual const char *get_ip_address();

    /** Get the local network mask
     *
     *  @return         Null-terminated representation of the local network mask 
     *                  or null if no network mask has been received
     */
    virtual const char *get_netmask();

    /** Get the local gateway
     *
     *  @return         Null-terminated representation of the local gateway
     *                  or null if no network mask has been received
     */
    virtual const char *get_gateway();

    /** Set a static IP address
     *
     *  Configures this network interface to use a static IP address.
     *  Implicitly disables DHCP, which can be enabled in set_dhcp.
     *  Requires that the network is disconnected.
     *
     *  @param ip_address Null-terminated representation of the local IP address
     *  @param netmask    Null-terminated representation of the local network mask
     *  @param gateway    Null-terminated representation of the local gateway
     *  @return           0 on success, negative error code on failure
     */
    virtual nsapi_error_t set_network(
            const char *ip_address, const char *netmask, const char *gateway);

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

    /** Start the interface
     *
     *  @return     0 on success, negative error code on failure
     */
    virtual nsapi_error_t connect() = 0;

    /** Stop the interface
     *
     *  @return     0 on success, negative error code on failure
     */
    virtual nsapi_error_t disconnect() = 0;

    /** Translates a hostname to an IP address with specific version
     *
     *  The hostname may be either a domain name or an IP address. If the
     *  hostname is an IP address, no network transactions will be performed.
     *
     *  If no stack-specific DNS resolution is provided, the hostname
     *  will be resolve using a UDP socket on the stack.
     *
     *  @param host     Hostname to resolve
     *  @param address  Destination for the host SocketAddress
     *  @param version  IP version of address to resolve, NSAPI_UNSPEC indicates
     *                  version is chosen by the stack (defaults to NSAPI_UNSPEC)
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t gethostbyname(const char *host,
            SocketAddress *address, nsapi_version_t version = NSAPI_UNSPEC);

    /** Hostname translation callback (asynchronous)
     *
     *  Callback will be called after DNS resolution completes or a failure occurs.
     *
     *  Callback should not take more than 10ms to execute, otherwise it might
     *  prevent underlying thread processing. A portable user of the callback
     *  should not make calls to network operations due to stack size limitations.
     *  The callback should not perform expensive operations such as socket recv/send
     *  calls or blocking operations.
     *
     *  @param status  0 on success, negative error code on failure
     *  @param address On success, destination for the host SocketAddress
     */
    typedef mbed::Callback<void (nsapi_error_t result, SocketAddress *address)> hostbyname_cb_t;

    /** Translates a hostname to an IP address (asynchronous)
     *
     *  The hostname may be either a domain name or an IP address. If the
     *  hostname is an IP address, no network transactions will be performed.
     *
     *  If no stack-specific DNS resolution is provided, the hostname
     *  will be resolve using a UDP socket on the stack.
     *
     *  Call is non-blocking. Result of the DNS operation is returned by the callback.
     *  If this function returns failure, callback will not be called. In case result
     *  is success (IP address was found from DNS cache), callback will be called
     *  before function returns.
     *
     *  @param host     Hostname to resolve
     *  @param callback Callback that is called for result
     *  @param version  IP version of address to resolve, NSAPI_UNSPEC indicates
     *                  version is chosen by the stack (defaults to NSAPI_UNSPEC)
     *  @return         0 on immediate success,
     *                  negative error code on immediate failure or
     *                  a positive unique id that represents the hostname translation operation
     *                  and can be passed to cancel
     */
    virtual nsapi_value_or_error_t gethostbyname_async(const char *host, hostbyname_cb_t callback,
            nsapi_version_t version = NSAPI_UNSPEC);

    /** Cancels asynchronous hostname translation
     *
     *  When translation is cancelled, callback will not be called.
     *
     *  @param id       Unique id of the hostname translation operation
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t gethostbyname_async_cancel(int id);

    /** Add a domain name server to list of servers to query
     *
     *  @param address  Destination for the host address
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t add_dns_server(const SocketAddress &address);

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

    /** Set blocking status of connect() which by default should be blocking
     *
     *  @param blocking true if connect is blocking
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t set_blocking(bool blocking);

    /** Dynamic downcast to an EthInterface */
    virtual EthInterface *ethInterface() {
        return 0;
    }

    /** Dynamic downcast to a WiFiInterface */
    virtual WiFiInterface *wifiInterface() {
        return 0;
    }

    /** Dynamic downcast to a MeshInterface */
    virtual MeshInterface *meshInterface() {
        return 0;
    }

    /** Dynamic downcast to a CellularBase */
    virtual CellularBase *cellularBase() {
        return 0;
    }

    /** Dynamic downcast to an EMACInterface */
    virtual EMACInterface *emacInterface() {
        return 0;
    }

protected:
    friend class Socket;
    friend class UDPSocket;
    friend class TCPSocket;
    friend class TCPServer;
    friend class SocketAddress;
    template <typename IF>
    friend NetworkStack *nsapi_create_stack(IF *iface);

    /** Provide access to the NetworkStack object
     *
     *  @return The underlying NetworkStack object
     */
    virtual NetworkStack *get_stack() = 0;

    /** Get the target's default network instance.
     *
     * This method can be overridden by the target. Default implementations
     * are provided weakly by various subsystems as described in
     * NetworkInterface::get_default_instance(), so targets should not
     * need to override in simple cases.
     *
     * If a target has more elaborate interface selection, it can completely
     * override this behaviour by implementing
     * NetworkInterface::get_target_default_instance() themselves, either
     * unconditionally, or for a specific network-default-interface-type setting
     *
     * For example, a device with both Ethernet and Wi-fi could be set up its
     * target so that:
     *    * DEVICE_EMAC is set, and it provides EMAC::get_default_instance(),
     *      which means EthernetInterface provides EthInterface::get_target_instance()
     *      based on that EMAC.
     *    * It provides WifiInterface::get_target_default_instance().
     *    * The core will route NetworkInterface::get_default_instance() to
     *      either of those if network-default-interface-type is set to
     *      ETHERNET or WIFI.
     *    * The board overrides NetworkInterface::get_target_default_instance()
     *      if network-default-interface-type is set to AUTO. This returns
     *      either EthInterface::get_default_instance() or WiFIInterface::get_default_instance()
     *      depending on a cable detection.
     *
     *
     * performs the search described by get_default_instance.
     */
    static NetworkInterface *get_target_default_instance();
};


#endif
