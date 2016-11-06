/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and limitations under the License.
 *
 */
#ifndef _NS_SOCKET_API_H
#define _NS_SOCKET_API_H

#ifdef __cplusplus
extern "C" {
#endif
/**
 * \file socket_api.h
 * \brief 6LoWPAN Library Socket API
 *
 * \section socket-com Common socket API
 *  - socket_open(), A function to open a socket.
 *  - socket_free(), A function to free a socket.
 *
 * \section socket-read Socket read API at callback
 *  - socket_read(), A function to read received data buffer from a socket.
 *  - socket_read_session_address(), A function to read session info for a TCP event.
 *
 * \section socket-tx Socket TX API
 * - socket_send(), A function to write data buffer to a socket.
 * - socket_sendto(), A function to write data to a specific destination in the socket.
 *
 * \section sock-connect TCP socket connection handle
 *  - socket_listen(), A function to set the socket to listening mode.
 *  - socket_connect(), A function to connect to a remote peer.
 *  - socket_close(), A function to close a connection.
 *
 * Sockets are a common abstraction model for network communication and are used in most operating systems.
 * 6LoWPAN Library API follows BSD socket API conventions closely with some extensions necessitated
 * by the event-based scheduling model.
 *
 * Library supports the following socket types:
 * | Socket name    |   Socket description           |
 * | :------------: | :----------------------------: |
 * | SOCKET_UDP     |   UDP socket type              |
 * | SOCKET_TCP     |   TCP socket type              |
 * | SOCKET_ICMP    |   ICMP raw socket type         |
 *
 * \section socket-raw ICMP RAW socket instruction
 * An ICMP raw socket can be created with the function socket_open() and the
 * identifier 0xffff. When using ICMP sockets, the minimum packet length is 4
 * bytes which is the 4 bytes of ICMP header. The first byte is for type, second
 * for code and last two are for the checksum that must always be set to zero.
 * The stack will calculate the checksum automatically before transmitting the packet.
 * THe header is followed by the payload if there is any.
 * NOTE: While it is perfectly legal to send an ICMP packet without payload,
 * some packet sniffing softwares might regard such packets as malformed.
 * In such a case, a dummy payload can be attached by providing a socket_sendto()
 * function with a pointer to your desired data buffer.
 * Moreover, the current implementation of the stack allows only certain ICMP types, for example
 * ICMP echo, reply, destination unreachable, router advertisement, router solicitation.
 * It will drop any other unimplemented types. For example, Private experimentation type (200) will
 * be dropped by default.

 * | ICMP Type |    ICMP Code | Checksum  | Payload    | Notes           |
 * | :-------: | :----------: | :-------: | :--------: | :--------------:|
 * | 1         | 1            | 2         | n bytes    | Length in bytes |
 * | 0xXX      | 0xXX         | 0x00 0x00 | n bytes    | Transmit        |
 * | 0xXX      | 0xXX         | 0xXX 0xXX | n bytes    | Receive         |
 *
 * ICMP echo request with 4 bytes of payload (ping6).
 *
 * | ICMP Type |    ICMP Code | Checksum  | Payload             |
 * | :-------: | :----------: | :-------: | :-----------------: |
 * | 0x80      | 0x00         | 0x00 0x00 | 0x00 0x01 0x02 0x03 |
 *
 * ICMP echo response for the message above.
 *
 * | ICMP Type |    ICMP Code | Checksum  | Payload             |
 * | :-------: | :----------: | :-------: | :-----------------: |
 * | 0x81      | 0x00         | 0xXX 0xXX | 0x00 0x01 0x02 0x03 |
 *
 * \section socket-receive Socket receiving
 * When there is data to read from the socket, a receive callback function is called with the event type parameter set to SOCKET_DATA.
 * The application must read the received data with socket_read() during the callback because the stack will release the data
 * when returning from the receive callback.
 *
 * Socket event has event_type and socket_id fields. The receive callback function must be defined when socket is opened using socket_open() API.
 *
 * All supported socket event types are listed here:
 *
 * | Event Type                 | Value | Description                                                         |
 * | :------------------------: | :---: | :-----------------------------------------------------------------: |
 * | SOCKET_EVENT_MASK          | 0xF0  | NC Socket event mask.                                               |
 * | SOCKET_DATA                | 0x00  | Data received, read data length available in d_len field.           |
 * | SOCKET_BIND_DONE           | 0x10  | TCP connection ready.                                               |
 * | SOCKET_TX_FAIL             | 0x50  | Socket data send failed.                                            |
 * | SOCKET_CONNECT_CLOSED      | 0x60  | TCP connection closed.                                              |
 * | SOCKET_CONNECT_FAIL_CLOSED | 0x70  | TCP connection closed no ACK received.                              |
 * | SOCKET_NO_ROUTER           | 0x80  | No route available to destination.                                  |
 * | SOCKET_TX_DONE             | 0x90  | Last socket TX process done, in TCP, whole TCP process is ready.    |
 * | SOCKET_NO_RAM              | 0xA0  | No RAM available.                                                   |
 *
 *
 * \section socket-tcp How to use TCP sockets:
 *
 * | API                           | Socket Type   | Description                                                      |
 * | :---------------------------: | :-----------: | :------------------------------------------------------------:   |
 * | socket_open()                 | Server/Client | Open socket to specific or dynamic port number.                  |
 * | socket_close()                | Server/Client | Close opened TCP connection.                                     |
 * | socket_listen()               | Server        | Set server port to listen state.                                 |
 * | socket_connect()              | Client        | Connect client socket to specific destination.                   |
 * | socket_send()                 | Client        | Send data to session based destination.                          |
 * | socket_sendto()               | Server/Client | Send data to specific destination.                               |
 * | socket_read_session_address() | Server/Client | Read socket TCP session address and port information.            |
 *
 * When the TCP socket is opened it is in closed state. It must be set either to listen or to connect state before it can be used to receive or transmit data.
 *
 * A socket can be set to listen mode with the socket_listen() function. After the call, the socket can accept an incoming connection from a remote host.
 * The listen mode closes the connection automatically after server timeout or when the client or application closes the connection manually by socket_close() function.
 *
 * A TCP socket can be connected to a remote host with socket_connect() with correct arguments. After the function call, a (non-blocking) application must wait for the socket event to confirm the successful state change of the socket.
 * After the successful state change, data can be sent using socket_send() by client and socket_send() by server.
 * The connection can be closed with socket_close() function or by server timeout.
 *
 * \section socket-udpicmp How to use UDP and RAW socket:
 *
 * A UDP socket is ready to receive and send data immediately after a successful call of socket_open() and a NET_READY event is received.
 * Data can be transmitted with the socket_sendto() function. An ICMP socket works with same function call.
 *
 */

#include "ns_address.h"

/** \name Protocol IDs used with sockets. */
///@{
/** UDP socket type. */
#define SOCKET_UDP      17
/** Normal TCP socket type. */
#define SOCKET_TCP      6
/** ICMPv6 raw socket type */
#define SOCKET_ICMP     32
/** Raw IPv6 socket type (socket_open identifier is IPv6 protocol number) */
#define SOCKET_RAW      2
/** REMOVED Feature in this release socket open return error Local Sockets for Tun interface functionality to APP-APP trough any BUS */
#define SOCKET_LOCAL    1
///@}

/** ICMP socket instruction
 *
 * ICMP header is comprised of 4 bytes. The first byte is for type, second for code and
 * the last two are for checksum that must always be zero.
 * The stack will calculate the checksum automatically when sending the packet.
 * The payload comes after the header and it can be of any length. It can also be set to 0.
 *
 * This applies for reading and sending.
 *
 *      ICMP TYPE   ICMP Code   ICMP Checksum (2 bytes)     Payload n bytes
 *      --------    --------    -------- --------           -------- -------- ....... -------
 *      0xXX        0xXX        0x00    0x00                0xXX    0xXX    ......  0xXX
 *
 * Example data flow for ping:
 *
 * ICMP echo request with 4-bytes payload (Ping6).
 *      0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03
 *
 * ICMP echo response for the message above.
 *      0x81, 0x00, 0xXX, 0xXX, 0x00, 0x01, 0x02, 0x03
 */

/*!
 * \struct socket_callback_t
 * \brief Socket Callback function structure type.
 */

typedef struct socket_callback_t {
    uint8_t event_type;     /**< Socket Callback Event check list below*/
    int8_t socket_id;       /**< Socket id queue which socket cause call back */
    int8_t interface_id;    /**< Network Interface ID where Packet came */
    uint16_t  d_len;        /**< Data length if event type is SOCKET_DATA */
    uint8_t LINK_LQI;       /**< LINK LQI info if interface cuold give */
} socket_callback_t;

/** IPv6 wildcard address IN_ANY */
extern const uint8_t ns_in6addr_any[16];

/**
 * \brief Create and initialize a socket for communication.
 *
 * \param protocol Defines the protocol to use.
 * \param identifier The socket port. 0 indicates that port is not specified and it will be selected automatically when using the socket.
 * \param passed_fptr A function pointer to a function that is called whenever a data frame is received to this socket.
 *
 * \return 0 or greater on success; Return value is the socket ID.
 * \return -1 on failure.
 * \return -2 on port reserved.
 */
extern int8_t socket_open(uint8_t protocol, uint16_t identifier, void (*passed_fptr)(void *));

/**
 * \brief A function to free a socket.
 *
 * \param socket ID of the socket to be released.
 *
 * \return 0 socket released.
 * \return -1 socket not released.
 */
extern int8_t socket_free(int8_t socket);

/**
 * \brief A function to set the socket to listening mode.
 *
 * \param socket Socket ID to bind.
 * \return 0 on success.
 * \return -1 on failure.
 */
extern int8_t socket_listen(int8_t socket);

/**
 * \brief A function to connect to remote peer (TCP).
 *
 * \param socket The socket ID.
 * \param address The address of a remote peer.
 * \param randomly_take_src_number 1 enables find next free random port number for the current one.
 *
 * \return 0 on success.
 * \return -1 in case of an invalid socket ID or parameter.
 * \return -2 if memory allocation fails.
 * \return -3 if the socket is in listening state.
 * \return -4 if the socket is already connected.
 * \return -5 connect is not supported on this type of socket.
 * \return -6 if the TCP session state is wrong.
 * \return -7 if the source address selection fails.
 */
extern int8_t socket_connect(int8_t socket, ns_address_t *address, uint8_t randomly_take_src_number);

/**
 * \brief Bind socket to address.
 *
 * Used by the application to bind a socket to a port and/or an address. Binding can
 * be done only once. The port or address cannot be changed after binding.
 *
 * \param socket Socket ID of the socket to bind.
 * \param address Address structure containing the port and address to bind.
 *
 * \return 0 on success.
 * \return -1 if the given address is NULL.
 * \return -2 if the port is already bound to another socket.
 * \return -4 if the socket is already bound.
 * \return -5 bind is not supported on this type of socket
 */
extern int8_t socket_bind(int8_t socket, const ns_address_t *address);

/**
 * \brief Bind a local address to a socket based on the destination address and
 *  the address selection preferences.
 *  Binding happens to the same address that socket_connect() would bind to.
 *  Reference: RFC5014 IPv6 Socket API for Source Address Selection.
 *
 * \param socket The socket ID.
 * \param dst_address The destination address to which the socket wants to communicate.
 *
 * \return 0 on success.
 * \return -1 if the given address is NULL or socket ID is invalid.
 * \return -2 if the memory allocation failed.
 * \return -3 if the socket is already bound to address.
 * \return -4 if the interface cannot be found.
 * \return -5 if the source address selection fails.
 * \return -6 bind2addrsel is not supported on this type of socket.
 */
extern int8_t socket_bind2addrsel(int8_t socket, const ns_address_t *dst_address);

/**
 * \brief A function to close a connection.
 *
 * \param socket The ID of the socket to be closed.
 * \param address The address of the destination client. When using as a client, a null pointer shall be passed.
 *
 * \return 0 on success.
 * \return -1 if the given socket ID is not found, if the socket type is wrong or tcp_close() returns a failure.
 * \return -2 if no active TCP session was found.
 * \return -3 if the referred socket ID port is declared as a zero.
 *
 * Note: It is highly recommended to always use randomly_take_src_number. The stack generates a new source port between 49152-65534.
 */
extern int8_t socket_close(int8_t socket, ns_address_t *address);

/**
 * \brief Send data via a connected TCP socket by client.
 *
 * Note: The socket connection must be ready before using this function.
 * The stack uses automatically the address of the remote connected host as the destination address for the packet.
 *
 * \param socket The socket ID.
 * \param buffer A pointer to data.
 * \param length Data length.
 *
 * \return 0 done
 * \return -1 Invalid socket ID.
 * \return -2 Socket memory allocation fail.
 * \return -3 TCP state not established.
 * \return -4 Socket TX process busy.
 * \return -5 Socket is not connected.
 * \return -6 Packet too short.
 */
extern int8_t socket_send(int8_t socket, uint8_t *buffer, uint16_t length);

/**
 * \brief A function to read received data buffer from a socket.
 *
 * Used by the application to get data from a socket. This method must be called once
 * from a socket callback when handling event SOCKET_DATA. If the received data does not fit
 * in the buffer provided the excess data bytes are discarded.
 *
 * \param socket The socket ID.
 * \param src_addr A pointer to a structure where the sender's address is stored.
 * \param buffer A pointer to an array where the read data is written to.
 * \param length The maximum length of the allocated buffer.
 *
 * \return greater than 0 indicates the length of the data copied to buffer.
 * \return 0 if no data is available to read.
 * \return -1 invalid input parameters.
 */
extern int16_t socket_read(int8_t socket, ns_address_t *src_addr, uint8_t *buffer, uint16_t length);

/**
 * \brief A function to send UDP, TCP or raw ICMP data via the socket.
 *
 * Used by the application to send data.
 *
 * \param socket The socket ID.
 * \param address A pointer to the destination address information.
 * \param buffer A pointer to data to be sent.
 * \param length Length of the data to be sent.
 * \return 0 on success.
 * \return -1 Invalid socket ID.
 * \return -2 Socket memory allocation fail.
 * \return -3 TCP state not established.
 * \return -4 Socket TX process busy.
 * \return -6 Packet too short.
 */
extern int8_t socket_sendto(int8_t socket, ns_address_t *address, uint8_t *buffer, uint16_t length);

/**
 * \brief A function to read session info for TCP event.
 *
 *
 * \param socket The socket ID.
 * \param address A pointer to the address structure where the session address information is read to.
 *
 * \return 0 on success.
 * \return -1 if no socket is found or TCP is not compiled into this project.
 * \return -2 if no session information is found.
 *
 * Note: This function should be called only at socket callback when the socket event is SOCKET_BIND_DONE or SOCKET_TX_DONE.
 * The following sections introduce those functions.
 */
extern int8_t socket_read_session_address(int8_t socket, ns_address_t *address);


/** \name Flags for SOCKET_IPV6_ADDR_PREFERENCES - opposites 16 bits apart. */
///@{
#define SOCKET_IPV6_PREFER_SRC_TMP              0x00000001 /**< Prefer temporary address (RFC 4941); default. */
#define SOCKET_IPV6_PREFER_SRC_PUBLIC           0x00010000 /**< Prefer public address (RFC 4941). */
#define SOCKET_IPV6_PREFER_SRC_6LOWPAN_SHORT    0x00000100 /**< Prefer 6LoWPAN short address. */
#define SOCKET_IPV6_PREFER_SRC_6LOWPAN_LONG     0x01000000 /**< Prefer 6LoWPAN long address. */
///@}

/** \name Options for SOCKET_IPV6_ADDRESS_SELECT. */
///@{
#define SOCKET_SRC_ADDRESS_MODE_PRIMARY     0 /**< Default value always. */
#define SOCKET_SRC_ADDRESS_MODE_SECONDARY   1 /**< This mode typically selects the secondary address. */
///@}

/** \name Protocol levels used for socket_setsockopt. */
///@{
#define SOCKET_IPPROTO_IPV6         41	/**< IPv6. */
///@}

/** \name Option names for protocol level SOCKET_IPPROTO_IPV6.
 * \anchor OPTNAMES_IPV6
 */
///@{
/** Specify traffic class for outgoing packets, as int16_t (RFC 3542 S6.5 says int); valid values 0-255, or -1 for system default. */
#define SOCKET_IPV6_TCLASS                  1
/** Specify hop limit for outgoing unicast packets, as int16_t (RFC 3493 S5.1 says int); valid values 0-255, or -1 for system default. */
#define SOCKET_IPV6_UNICAST_HOPS            2
/** Specify hop limit for outgoing multicast packets, as int16_t (RFC 3493 S5.2 says int); valid values 0-255, or -1 for system default. */
#define SOCKET_IPV6_MULTICAST_HOPS          3
/** Specify source address preferences, as uint32_t - see RFC 5014; valid values combinations of SOCKET_IPV6_PREFER_xxx flags. */
#define SOCKET_IPV6_ADDR_PREFERENCES        4
/** Specify PMTU preference, as int8_t (RFC 3542 S11.1 says int); valid values -1 (PMTUD for unicast, default), 0 (PMTUD always), 1 (PMTUD off). */
#define SOCKET_IPV6_USE_MIN_MTU             5
/** Specify not to fragment datagrams, as int8_t (RFC 3542 S11.2 says int); valid values 0 (fragment to path MTU, default), 1 (no fragmentation, TX fails if bigger than outgoing interface MTU). */
#define SOCKET_IPV6_DONTFRAG                6
/** Specify flow label for outgoing packets, as int32_t; valid values 0-0xfffff, or -1 for system default, or -2 to always autogenerate */
#define SOCKET_IPV6_FLOW_LABEL              7

#define SOCKET_BROADCAST_PAN                0xfc /**< Internal use - transmit with IEEE 802.15.4 broadcast PAN ID */
#define SOCKET_LINK_LAYER_SECURITY          0xfd /**< Not standard enable or disable socket security at link layer (For 802.15.4). */
#define SOCKET_INTERFACE_SELECT             0xfe /**< Not standard socket interface ID. */
#define SOCKET_IPV6_ADDRESS_SELECT          0xff /**< Deprecated - use SOCKET_IPV6_ADDR_PREFERENCES instead. */
///@}

/**
 * \brief Set an option for a socket
 *
 * Used to specify miscellaneous options for a socket. Supported levels and
 * names defined above.
 *
 * \param socket The socket ID.
 * \param level The protocol level.
 * \param opt_name The option name (interpretation depends on level). See \ref OPTNAMES_IPV6.
 * \param opt_value A pointer to value for the specified option.
 * \param opt_len Size of the data pointed to by the value.
 *
 * \return 0 on success.
 * \return -1 invalid socket ID.
 * \return -2 invalid/unsupported option.
 * \return -3 invalid option value.
 */
extern int8_t socket_setsockopt(int8_t socket, uint8_t level, uint8_t opt_name, const void *opt_value, uint16_t opt_len);

/**
 * \brief Get an option for a socket.
 *
 * Used to read miscellaneous options for a socket. Supported levels and
 * names defined above. If the buffer is smaller than the option, the output
 * is silently truncated; otherwise opt_len is modified to indicate the actual
 * length.
 *
 * \param socket The socket ID.
 * \param level The protocol level.
 * \param opt_name The option name (interpretation depends on level). See \ref OPTNAMES_IPV6.
 * \param opt_value A pointer to output buffer.
 * \param opt_len A pointer to length of output buffer; updated on exit.
 *
 * \return 0 on success.
 * \return -1 invalid socket ID.
 * \return -2 invalid/unsupported option.
 */
extern int8_t socket_getsockopt(int8_t socket, uint8_t level, uint8_t opt_name, void *opt_value, uint16_t *opt_len);

#ifdef __cplusplus
}
#endif
#endif /*_NS_SOCKET_H*/
