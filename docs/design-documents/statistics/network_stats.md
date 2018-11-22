# Network statistics

This document discusses data collection from API point of view and impact it causes on existing drivers. Aim is not to cause any changes into existing application APIs or driver APIs.

Another aim is that all statistics data we collect is generic enough that the collector application does not need to know the network protocols that are in use. However, these are IP specific, so we don't consider Non-IP protocols here.

## Network APIs where we are going to monitor the data

![](https://s3-us-west-2.amazonaws.com/mbed-os-docs-images/networkstacks.png)

APIs that consist Mbed OS networking are:

* Network drivers. May driver APIs exist, see [IP networking](https://os.mbed.com/docs/v5.10/reference/ip-networking.html)
* [Network interface](https://os.mbed.com/docs/v5.10/apis/network-interfaces.html)
* [Socket API](https://os.mbed.com/docs/v5.10/apis/network-socket.html)
* [NetworkStack](https://os.mbed.com/docs/v5.10/porting/networkstack.html)

### Network drivers

**May provide data:** packet counters and network status. In case of external IP stack could also provide information about sockets.

Some network drivers exist outside of the Mbed OS tree, therefore we will not consider adding any data collection into driver APIs.
Causing external drivers to be refactored is a time consuming work and requires too much coordination with partners.

Also, some drivers run external IP stacks through AT command interface making it impossible to collect some of the data, for example packet counters.

**Conclusion:** No changes to drivers and no data collection from driver side.

### NetworkInterface

**May provide data:** Network status and events when status changes. IP addresses,  etc.

`NetworkInterface` is a porting interface for networked drivers that require control API, therefore any API change would impact all network drivers that provide this API. For example all external IP stacks provide this API.

Any change on network state is reflected by the driver that calls the event handler that application can register. Refer to [Network Status](https://os.mbed.com/docs/v5.10/apis/network-status.html) from Mbed OS handbook. Problem is that this registering happens on driver side, and the high layer `NetworkInterface` API only provides a dummy implementation. If event based information is required API change is forced to either application side, or driver side.

**Conclusion:** Network status is already provided by `NetworkInterface::get_connection_status()`. Event would cause API change, so will not be done for 5.11. But there are requests to change the API, so we can do it in 5.12. IP addresses are already provided by `NetworkInteface` API, so will not be touched by stats API.

### Socket

**May provide data:** Number of sockets open, connection endpoints, amount of data sent per socket, total amount of data sent, protocol types.

In Mbed OS the `Socket` is a abstract interface that follows the POSIX Socket API. Any type of sockets can be created by inheriting the base class. But Mbed OS internally knows only two type of sockets, TCP and UDP, and those share the same base class `InternetSocket`. Basically all data that device is able to send to network flows through these three classes `InternetSocket`, `TCPSocket` and `UDPSocket`.

Sockets can be constructed and destructed on the fly, so the data collected should be stored elsewhere.

**Conclusion:** We can either collect one cumulative number of all data send through sockets, or numbers per opened socket, endpoint or port number. Or we can provide list of sockets currently active in the system.

### NetworkStack

**May provide data:** Same as Socket

`NetworkStack` is a porting layer for a driver that provides a socket API. Each stack provides `struct nsapi_stack_api` which contains all functions that Mbed OS Socket API calls.

**Conclusion:** Same data can be collected from `InternetSocket` class so we will not touch the porting layer.

## Data collection API

This is proposal of the new API that provides network statistics. Some parts can be done in Mbed OS 5.11 timeframe without API changes. I'll also propose API changes for 5.12 to collect more data.

### Socket status list

We could show some kind of list of sockets that are currently open, or have been open. Instead of abstract ID, we provide pointers to the `Socket` objects, we those pointers are not thread safe, because the original object might already been desctructed when data is parsed. Therefore the last associated destination address is copied into this structure. Or if we want to save memory, we could record just the destination port number.

```
typedef enum socket_state {
    CLOSED,                         /**< Socket is closed and does not exist anymore in the system */
    OPEN,                           /**< Socket is open, but not associated to any peer address */
    CONNECTED,                      /**< Socket is associated to peer address, either by connect() or sendto()/recvfrom() calls */
    LISTEN,                         /**< Socket is listening for incoming connections */
};

typedef struct {
    Socket *socket;                 /**< Pointer to original Socket objet. Not safe to access, used for identifying */
    SocketAddress peername          /**< Last associated peername of this socket (Destination address) */
    socket_state state;             /**< State of this socket */
    uint32_t tick_last_change;      /**< osKernelGetTick() when state last changed */
    size_t sent;                    /**< Data sent through this socket */
    size_t received;                /**< Data received through this socket */
} mbed_stats_socket_t

/** Write socket statistics into structure pointed by stats.
  * @count How many structures can be written
  * @return How many structures was written
  */
size_t mbed_stats_socket_get_each(mbed_stats_thread_t *stats, size_t count);

```

RAM requirement `sizeof(SocketAddress) + sizeof(Socket*) + sizeof(enum) + sizeof(uint32_t)*3=26+4+4+4*3 = 46` per socket stats. How many we keep in memory? 10?


### Network status events (For Mbed OS 5.12)

Because in the future, the event handler needs API changes anyway, as requested in [ONME-3917](https://jira.arm.com/browse/ONME-3917) we can then consider adding statistics collection into status events. This would be done so that `NetworkInterface` would be a wrapper between user callbacks and callbacks coming from the driver.

Status events should capture system timestamp associated with the events. API is similar to some in `mbed_stats.h` that requests array of events. Internally the stats should have some cap of how many events can be kept in memory. Perhaps configurable?

```
typedef struct {
    uint32_t tick;                   /**< osKernelGetTick() value of when event happened, or equivalent timestamp */
    NetworkInteface *interface;      /**< What interface caused this event */
    nsapi_connection_status_t status /**< What was the event */
} mbed_stats_connection_t;

/** Write network connection statistics into structure pointed by stats.
  * @count How many structures can be written
  * @return How many structures was written
  */
size_t mbed_stats_connection_get_each(mbed_stats_thread_t *stats, size_t count);
```

RAM requirements `4 * 3 = 12` per how many statuses we keep in memory.


## Examples of what can we collect after these changes

This is not a full list.

|Data|Where to get it?|
|----|----------------|
|State of each network interface|`NetworkInterface::get_connection_status()` and `mbed_stats_connection_t.status`|
|How many network interfaces have been created|return value of `mbed_stats_connection_get_each()`|
|What is the IP address of each interface|`NetworkInterface::get_ip_address()`|
|MAC address of each interface|`NetworkInterface::get_mac_address()`|
|How many sockets have been used|`mbed_stats_network_t.udp_sockets + mbed_stats_network_t.tcp_sockets`|
|How many sockets are used now|Count each `mbed_stats_socket_t.state == OPEN||CONNECTED`|
|Where did we send data?|Check each `mbed_stats_socket_t.peername`|
|How much did we send data to that target|Check each `mbed_stats_socket_t.sent` where `peername == target`|
|What happened to sockets after network lost|Check `mbed_stats_connection_t.tick` and compare to `mbed_stats_socket_t.tick_last_change`|