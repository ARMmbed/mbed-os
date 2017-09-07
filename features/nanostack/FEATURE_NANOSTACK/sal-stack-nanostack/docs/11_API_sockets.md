Socket API
==============================
This chapter describes the socket use and the available APIs.

## API Headers

Socket API can be fully harnessed by including the following header files:

```
#include net_interface.h
#include socket_api.h
```

## Socket types available

Sockets are a common abstraction model for network communication and are used in most Operating Systems (OS). The 6LoWPAN stack API follows the _Berkeley Software Distribution_ (BSD) socket API conventions closely with some extensions necessitated by the event-based scheduling model. The stack supports the socket types shown in _Table 3-18_.

**Table 3-18 Socket types**

Socket name|Socket description
-----------|------------------
`SOCKET_UDP`|UDP socket type.
`SOCKET_TCP`|TCP socket type.
`SOCKET_ICMP`|ICMP raw socket type; see section _ICMP socket instruction_.
`SOCKET_RAW`|raw IPv6 socket type.

### ICMP socket instruction

When using _Internet Control Message Protocol_ (ICMP) sockets, the minimum packet length is eight bytes where the first four bytes comprise the ICMP header, as described in _Table 3-19_. The stack will calculate the checksum automatically before transmitting the packet.

**Table 3-19 General ICMP packet structure**

Type|Code|Checksum|Payload|Notes
----|----|--------|-------|-----
1|1|2|n (min. 4)|Length in bytes
`0xXX`|`0xXX`|`0x00 0x00`|n bytes|Transmit
`0xXX`|`0xXX`|`0xXX 0xXX`|n bytes|Receive


ICMP echo request with four bytes of payload (ping6), as shown in _Table 3-20_.

**Table 3-20 ICMP echo request**

Type|Code|Checksum|Payload
----|----|--------|-------
`0x80`|`0x00`|`0x00 0x00`|`0x00 0x01 0x02 0x03`

ICMP echo response for the message, as shown in _Table 6-4_.

**Table 3-21 ICMP echo response**

Type|Code|Checksum|Payload
----|----|--------|-------
`0x81`|`0x00`|`0xXX 0xXX`|`0x00 0x01 0x02 0x03`

## Receive callback structure

When there is data to read from the socket, a receive callback function is called from the stack with the socket event parameter. A socket event can be, for example, a change in the state of the _Transmission Control Protocol_ (TCP) socket and the socket _Transmit_ (TX) process is ready or the process failed `(SOCKET_TX_FAIL` or `SOCKET_TX_DONE)`. All supported socket event types are listed in _Table 3-22_. The receive callback function must be defined when a socket is opened using the `socket_open( )` API.

The socket call back structure, `socket_callback_t` is defined as below:

```
typedef struct socket_callback_t {
    uint8_t 	event_type;
    int8_t 		socket_id;
    int8_t 		interface_id;
    uint16_t  	d_len;
    uint8_t 	LINK_LQI;
} socket_callback_t;
```

Member|Description
------|-----------
`event_type`|Socket event type as provided in Table 3-22.
`socket_id`|The ID of the socket that caused the event.
`interface_id`|The network interface ID. Same as received from `arm_nwk_interface_init`.
`d_len`|The length of data available or sent.
`LINK_LQI`|The Link Quality Indicator value if the interface can provide any.

**Table 3-22 Supported socket event types**

Event type|Description
----------|-----------
`SOCKET_DATA`|Data received.
`SOCKET_CONNECT_DONE`|TCP connection ready.
`SOCKET_CONNECT_FAIL`|TCP connection failed.
`SOCKET_CONNECT_AUTH_FAIL`|TCP connection authentication failed.
`SOCKET_INCOMING_CONNECTION`|TCP connection state change from listen to establishment.
`SOCKET_TX_FAIL`|Socket data send failed.
`SOCKET_CONNECT_CLOSED`|TCP connection closed.
`SOCKET_CONNECTION_RESET`|TCP connection reset.
`SOCKET_NO_ROUTE`|No route available to destination.
`SOCKET_TX_DONE`|TX process done (one per datagram, or if stream will be called each time some data acknowledged)
`SOCKET_NO_RAM `|If no RAM is present.
`SOCKET_CONNECTION_PROBLEM`|If TCP is suffering a connection problem (a soft event, it continues to retry).

An example parsing socket event:

```
#define APP_SOCK_RX_SIZE	250

/*Application socket payload buffer used for RX and TX*/
static uint8_t rx_buffer[APP_SOCK_RX_SIZE];

void main_receive
(SOCKET_NO_ROUTE
	void *cb
)
{
	socket_callback_t *cb_res = cb;
    int16_t length;

    if( cb_res->event_type == SOCKET_DATA )
    {
        sn_nsdl_addr_s sn_addr_s;

        //Read data from the RX buffer
        length = socket_read( cb_res->socket_id,
				&app_src,
				rx_buffer,
				APP_SOCK_RX_SIZE );

        if( length )
        {
            if( cb_res->socket_id == app_udp_socket )
            {
                // Handles data received in UDP socket

                sn_nsdl_process_coap(rx_buffer, length, &sn_addr_s);
            }

        }
    }
}
```

## Using TCP sockets

When a TCP socket is opened, it is in an unusable state and must be set to either a _listen_ or _connect_ state before it can be used to receive or transmit data.

You can set the socket to a _listen_ state with the `socket_listen( )` function. After the call, the socket can accept an incoming connection from a remote host. The TCP implementation of the 6LoWPAN stack supports only one connection from a remote host. 

To connect the TCP socket to a remote host, call `socket_connect( )` with the correct arguments. After the function call, an application (non-blocking) must await the socket event to confirm the successful state change of the socket.

After receiving a successful state event, data can be sent using the `socket_send( )` call. 

The connection can be shut down by calling function `socket_shutdown( )`. The 6LoWPAN stack shuts down the connection automatically after a server timeout or when the remote end closes the connection. When the socket is no longer needed it must be released by calling the function `socket_close( )`.

## Using UDP and ICMP sockets

A _User Datagram Protocol_ (UDP) socket is ready to receive and send data immediately after a successful call to `socket_open( )`. Data can then be transmitted using the `socket_sendto( )` function call. The same function call can also be used for an ICMP socket.

## Detailed Socket API usage

This section describes the socket layer functions in more detail. Each function is presented with example parameters and possible return values.

### How to open a socket

To initialize a socket ready for communication:

```
int8_t socket_open
(
	uint8_t		protocol,
	uint16_t	identifier,
	void		(*passed_fptr)(void *)
)
```

Parameter|Description
---------|-----------
`protocol`|The protocol to be used over this socket and valid values for the argument are:<br>`SOCKET_UDP` UDP: standard communication.<br>`SOCKET_TCP` TCP: standard communication.<br>`SOCKET_ICMP` ICMPv6: used for ping functionality.<br>`SOCKET_RAW` raw IPv6: used for other specialised protocols.
`identifier`|The port identifier for UDP and TCP. 0 indicates that the port is not specified and it will be selected automatically when using the socket. The port can also be bound later with the function `socket_bind()`.
`passed_fptr`|A function pointer to the desired socket receive callback function.

<dl>
<dt>Return value</dt>
<dd>0 Socket ID, used as a reference to the specific socket in subsequent calls.</dd>
<dd>-1 No free sockets or invalid protocol.</dd>
</dl>

### How to release a socket

To release a socket:

`int8_t socket_close( int8_t socket )`

Parameter|Description
---------|-----------
`socket`|The socket ID of the socket to be released.

<dl>
<dt>Return value</dt>
<dd>0 Socket release successful.</dd>
<dd>-1 Socket release failed. Socket ID invalid or already released.</dd>
</dl>

### How to bind a socket

To bind socket to a port and address:

```
int8_t socket_bind
(
	int8_t 				socket,
	const ns_address_t 	*address
)
```

Parameter|Description
---------|-----------
`socket`|The socket ID returned by `socket_open`.
`address`|Structure that contains port and/or address to be bound to the socket.

<dl>
<dt>Return value</dt>
<dd>0 on success.</dd>
<dd>-1 if given address is NULL.</dd>
<dd>-2 if port is already bound to another socket.</dd>
<dd>-4 if socket is already bound.</dd>
<dd>-5 bind is not supported on this type of socket.</dd>
</dl>

The port and the address can be bound only once. The port can also be bound to the socket with the function `socket_open( )`.

To bind a local address to a socket based on a destination address and address selection preferences:

```
int8_t socket_bind2addrsel
(
	int8_t 				socket,
	const ns_address_t 	*dst_address
)
```

Parameter|Description
---------|-----------
`socket`|The socket ID returned by `socket_open`.
`dst_address`|Destination address to which the socket wants to communicate.

<dl>
<dt>Return value</dt>
<dd>0 on success.</dd>
<dd>-1 if given address is NULL or socket ID is invalid.</dd>
<dd>-2 if memory allocation failed.</dd>
<dd>-3 if socket is already bound to address.</dd>
<dd>-4 if interface cannot be found.</dd>
<dd>-5 if source address selection fails.</dd>
<dd>-6 bind2addrsel is not supported on this type of socket.</dd>
</dl>

### How to connect a socket

To connect a socket to a remote host:

```
int8_t socket_connect
(
	int8_t			socket,
	ns_address_t	*address,
	uint8_t			randomly_take_src_numbers
)
```

Parameter|Description
---------|-----------
`socket`|The socket ID, which is used to connect to the remote host.
`address`|A pointer to an <code>address_t</code> structure that contains the address of the remote host.
`randomly_take_src_numbers`|Value 1 indicates that a randomly selected source port number is used.

<dl>
<dt>Return value</dt>
<dd>0 Valid request.</dd>
<dd>-1 Fail.</dd>
</dl>

### How to read data from a socket

To read received data from a socket:

```
int16_t socket_read
(
	int8_t			socket,
	ns_address_t	*address,
	uint8_t			*buffer,
	uint16_t		length
)
```

Parameter|Description
---------|-----------
`socket`|The socket ID of the socket to be read.
`address`|A pointer to an address structure containing the source address of the packet (populated by the stack). Can be NULL.
`buffer`|A pointer to a byte array containing the payload of the packet.
`length`|The length of the payload data to be stored in the buffer.

<dl>
<dt>Return value</dt>
<dd>> 0 The length of the data copied to the buffer.</dd>
<dd>0 No data is available to read.</dd>
<dd>-1 Invalid input parameters.</dd>
</dl>

The application receives an event type `SOCKET_DATA` to its receive socket callback when the data is available. The application
must read the data from the callback because the stack frees the data after it has called the receive socket callback.
All data must be read by one call. If data is too long to fit in the supplied buffer the excess bytes are discarded.

### How to send data to a socket

To transmit data using a socket, the 6LoWPAN stack offers two different functions depending on the transport layer protocol that is used, as shown in _Table 3-23_.

After successfully calling the function, the application must await the TX process to complete.

**Table 3-23 The two transmit function calls**

Function|Socket types
--------|------------
`socket_sendto( )`|UDP, ICMP and RAW
`socket_send( )`|TCP, or UDP if connected


_Table 3-24_ describes the possible response events when the outcome of the function call is successful.

**Table 3-24 The possible response events following a successful function call**

Response Event|Socket Type|Description
--------------|-----------|-----------
`SOCKET_TX_DONE`|TCP/UDP|UDP: link layer TX ready (d_len = length of datagram). TCP: some data acknowledged (d_len = unacknowledged data remaining in send queue).
`SOCKET_TX_FAIL`|TCP/UDP|UDP: link layer TX fails. TCP: transmit timeout (no ACKs) and connection closed.
`SOCKET_CONNECTION_RESET`|TCP|Either the peer reset the connection or there was a protocol error. Connection closed.

To transmit data on an unconnected socket:

```
int8_t socket_sendto
(
	int8_t			socket,
	ns_address_t	address,
	uint8_t			*buffer,
	uint16_t		length
)
```

Parameter|Description
---------|-----------
`socket`|The socket ID to use for transmission.
`address`|A pointer to an address structure containing the destination address of the packet (populated by the application).
`buffer`|A pointer to a byte array containing the payload of the packet.
`length`|The length of the payload data in the buffer.


<dl>
<dt>Return value</dt>
<dd>>0 Data packet accepted by the stack.</dd>
<dd>-1 Fail.</dd>
</dl>

To send data via a connected socket:

<span class="notes">**Note**: A socket connection must be ready before using this function. The stack will automatically use the address of the remote connected host address as the destination for the packet.</span>

```
int8_t socket_send
(
	int8_t		socket,
	uint8_t		*buffer,
	uint16_t	length
)
```

Parameter|Description
---------|-----------
`socket`|The socket ID to use for transmission.
`buffer`|A pointer to a byte array containing the payload of the packet.
`length`|The length of the payload data in the buffer.

<dl>
<dt>Return value</dt>
<dd>>0 Data packet accepted by the stack.</dd>
<dd>-1 Fail.</dd>
</dl>

### TCP socket configuration

The TCP socket configuration API offers three function calls, as shown in _Table 3-25_ and are further described.

**Table 3-25 The TCP socket configuration functions**

Function|Description
--------|-----------
`socket_listen()`|Set socket to the listen state.
`socket_accept()`|Accepts an incoming TCP connection.
`socket_shutdown()`|Shut down socket connection.

To set a TCP socket into the listen state:

```
int8_t socket_listen
(
	int8_t	socket,
	uint8_t	backlog
)
```

Parameter|Description
---------|-----------
`socket`|The socket ID that is to be set to the listen state.
`backlog`|The pending connections queue size.

<dl>
<dt>Return value</dt>
<dd>0 Valid request.</dd>
<dd>-1 Fail.</dd>
</dl>

For connecting a socket, please refer to the section  above [How to connect a socket](#how-to-connect-a-socket). 

There are three possible responses from the stack for `socket_connect( )`:

- `SOCKET_CONNECT_DONE`
	- TCP handshake ready.

- `SOCKET_CONNECT_FAIL`
	- TCP handshake fail - connection actively refused or protocol error.

- `SOCKET_TX_FAIL`
	- TCP handshake fail - timed out.

For accepting an incoming TCP connection, use `socket_accept()` function.

```
int8_t socket_accept()
(
	int8_t	listen_socket_id,
	ns_address_t	*addr,
    void (*passed_fptr)  (void *)
)
```

Parameter|Description
---------|-----------
`listen_socket_id`|The socket ID of the listening socket.
`addr`|Pointer to the address structure where you wish to save the address
`passed_fptr`|A function pointer to a function that is called whenever a data frame is received to the new socket

<dl>
<dt>Return value</dt>
<dd>0 or greter than zero, i.e.,  id for the new socket.</dd>
<dd>-1 Fail.</dd>
</dl>

To shut down a TCP connection:

```
int8_t socket_shutdown
(
	int8_t			socket,
	uint8_t			how
)
```

Parameter|Description
---------|-----------
`socket`|The socket ID of the socket to be shut down.
`how`|How socket is to be shut down, one of `SOCKET_SHUT_RD`, `SOCKET_SHUT_WR` or `SOCKET_SHUT_RDWR`.

<dl>
<dt>Return value</dt>
<dd>0 Valid request.</dd>
<dd><b>Note:</b>This does not imply that the state of the socket has been successfully changed.</dd>
<dd>-1 Fail.</dd>
</dl>

### Modifying socket options

To specify miscellaneous options for a socket:

```
int8_t socket_setsockopt
(
	int8_t		socket,
	uint8_t		level,
	uint8_t		opt_name,
	const void	*opt_value,
	uint16_t	opt_len
)
```

Parameter|Description
---------|-----------
`socket`|The socket ID.
`level`|The option level.
`opt_name`|The option name.
`opt_value`|A pointer to the value of the specified option.
`opt_len`|The size of the data pointed to by a value.

<dl>
<dt>Return value</dt>
<dd>0 Done.</dd>
<dd>-1 Invalid socket ID.</dd>
<dd>-2 Invalid or unsupported option.</dd>
<dd>-3 Invalid option value.</dd>
</dl>

Each socket has unique control of the following items:

- IPv6 traffic class, option name: `SOCKET_IPV6_TCLASS`
- Socket source address mode, option name: `SOCKET_IPV6_ADDRESS_SELECT`

A socket uses a configured setup until the user changes it with a new function call.

<span class="notes">**Note**: `SOCKET_IPV6_ADDRESS_SELECT` is only supported when the interface bootstrap address mode is `NET_6LOWPAN_MULTI_GP_ADDRESS`.</span>

#### How to set address mode for a socket

A socket can be configured to use a primary or secondary address as the source address when transmitting packets using `socket_sendto( )`. The primary (default) address is an IPv6 address that is created using a short address type, whereas the secondary address, also IPv6 based, is created using a MAC address type. The source address is currently the only configurable parameter using `socket_configuration` and when multimode is not used, the source address configuration has no effect. The primary address is the default address when no configuration is needed.

```
int8_t set_coap_socket_src_address_mode(int16_t address_mode)
{

 return socket_setsockopt(app_udp_socket,SOCKET_IPPROTO_IPV6,
SOCKET_IPV6_ADDRESS_SELECT, &address_mode),sizeof(address_mode)));
}
```

### How to set traffic class for a socket

You can use `socket_setsockopt()` to set the socket traffic class. When this option is set, it will stay until modified. Therefore, if you want to set the class for one specific packet, you must call `socket_setsockopt()` again with a default traffic class after the packet has been sent.

Parameters for Traffic class:

Parameter|Description
---------|-----------
`socket`|The socket identified.
`level`|`SOCKET_IPPROTO_IPV6`
`opt_name`|`SOCKET_IPV6_TCLASS`
`opt_value`|A pointer to `int16_t` value. Valid values are from 0 to 255. -1 is for system default.
`opt_len`|The size of `int16_t`, 2 bytes.

[RFC 4594](https://tools.ietf.org/html/rfc4594) specifies the appropriate traffic class values. The 6LoWPAN stack does not interpret the specified traffic class. It is just passed through.

### How to set flow label for a socket

You can use `socket_setsockopt()` to set the socket flow label.

Parameters for flow label:

Parameter|Description
---------|-----------
`socket`|The socket identified.
`level`|`SOCKET_IPPROTO_IPV6`
`opt_name`|`SOCKET_IPV6_FLOW_LABEL`
`opt_value`|A pointer to `int32_t` value. Valid values are from `0` to `0xfffff`. -1 is for system default (set with `arm_nwk_ipv6_auto_flow_label()`). -2 will always autogenerate a flow label, regardless of system default.
`opt_len`|The size of `int32_t`, 4 bytes.

The stack auto-generates flow labels on outgoing packets following the guidelines in [RFC 6437](https://tools.ietf.org/html/rfc6437). The stack does not interpret the flow label on received packets, and nor does the socket API report flow label to the application.

