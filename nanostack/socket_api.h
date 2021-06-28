/*
 * Copyright (c) 2010-2021, Pelion and affiliates.
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
 *  - socket_close(), A function to close a socket.
 *  - socket_connect(), A function to connect to a remote peer.
 *  - socket_bind(), A function to bind a local address or port or both.
 *  - socket_getpeername(), A function to get remote address and port of a connected socket.
 *  - socket_getsockname(), A function to get local address and port of a bound socket.
 *
 * \section socket-read Socket read API at callback
 *  - socket_read(), A function to read received data buffer from a socket.
 *  - socket_recvmsg(), A function to read received data buffer from a socket to Posix defined message structure
 *
 * \section socket-tx Socket TX API
 * - socket_send(), A function to write data buffer to a socket.
 * - socket_sendto(), A function to write data to a specific destination in the socket.
 * - socket_sendmsg(), A function which support socket_send and socket_sendto functionality which supports ancillary data
 *
 * \section sock-connect TCP socket connection handle
 *  - socket_listen(), A function to set the socket to listening mode.
 *  - socket_accept(), A function to accept an incoming connection.
 *  - socket_shutdown(), A function to shut down a connection.
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
 * The application must read the received data with socket_read() or socket_recvmsg() during the callback because the stack will release the data
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
 * | SOCKET_CONNECT_DONE        | 0x10  | TCP connection ready.                                               |
 * | SOCKET_CONNECT_FAIL        | 0x20  | TCP connection failed.                                              |
 * | SOCKET_INCOMING_CONNECTION | 0x40  | TCP incoming connection on listening socket.                        |
 * | SOCKET_TX_FAIL             | 0x50  | Socket data send failed.                                            |
 * | SOCKET_CONNECT_CLOSED      | 0x60  | TCP connection closed.                                              |
 * | SOCKET_CONNECTION_RESET    | 0x70  | TCP connection reset.                                               |
 * | SOCKET_NO_ROUTE            | 0x80  | No route available to destination.                                  |
 * | SOCKET_TX_DONE             | 0x90  | UDP: link layer TX ready (d_len = length of datagram).              |
 * |                            |       | TCP: some data acknowledged (d_len = data remaining in send queue)  |
 * | SOCKET_NO_RAM              | 0xA0  | No RAM available.                                                   |
 * | SOCKET_CONNECTION_PROBLEM  | 0xB0  | TCP connection is retrying.                                         |
 *
 *
 * \section socket-tcp How to use TCP sockets:
 *
 * | API                           | Socket Type   | Description                                                      |
 * | :---------------------------: | :-----------: | :------------------------------------------------------------:   |
 * | socket_open()                 | Server/Client | Open socket to specific or dynamic port number.                  |
 * | socket_shutdown()             | Server/Client | Shut down opened TCP connection.                                 |
 * | socket_listen()               | Server        | Set server port to listen state.                                 |
 * | socket_accept()               | Server        | Accept a connection to a listening socket as a new socket.       |
 * | socket_connect()              | Client        | Connect client socket to specific destination.                   |
 * | socket_close()                | Server/Client | Closes the TCP Socket.                                           |
 * | socket_send()                 | Server/Client | Send data to peer.                                               |
 * | socket_recv()                 | Server/Client | Receive data from peer.                                          |
 *
 * When the TCP socket is opened it is in closed state. It must be set either to listen or to connect state before it can be used to receive or transmit data.
 *
 * A socket can be set to listen mode with the socket_listen() function. After the call, the socket can accept an incoming connection from a remote host.
 *
 * A TCP socket can be connected to a remote host with socket_connect() with correct arguments. After the function call, a (non-blocking) application must wait for the socket event to confirm the successful state change of the socket.
 * After the successful state change, data can be sent using socket_send().
 * The connection can be shut down in either direction with socket_shutdown() function - shutting down write signals end-of-data to the peer.
 *
 *
 * \section socket-udpicmp How to use UDP and RAW socket:
 *
 * A UDP socket is ready to receive and send data immediately after a successful call of socket_open() and a NET_READY event is received.
 * Data can be transmitted with the socket_sendto() function. An ICMP socket works with same function call.
 *
 * \section socket-trafficpriority How to set socket message priority to improve Quality of Service (QoS):
 *
 * IPv6 header has a field traffic class that contains a 6-bit Differentiated Services Code Point (DSCP) field that is used for packet
 * classification. By default the packet class level is set to 0 NS_DSCP_DEFAULT.
 *
 * Recommend QoS levels:
 *
 * |     Level        |Description                                                                                          |
 * | :--------------: | :-------------------------------------------------------------------------------------------------: |
 * | NS_DSCP_DEFAULT  | Default level for normal data usage                                                                 |
 * | NS_DSCP_AF11     | Higher Application data service for prioritize packet forwarding.                                   |
 * | NS_DSCP_EF       | Expedited Forwarding (EF) for short messages. Allows low loss, low delay, and low jitter services.  |
 * |                  | This is meant for very important messages like alerts. EF packet length should be kept in           |
 * |                  | minimum. This should not be used for any other purpose as it will block other network traffic       |
 * | NS_DSCP_CS6      | Network protocol message Priority. Application should not use this.                                 |
 *
 * High priority messages can be set to use higher than default class by using socket_setsockopt() and
 * socket_option_traffic_class_dsc_set() helper.
 *
 * Example to send a message using Expedited Forwarding class:
 *
 * //Set EF class to high priority messages
 * int16_t traffic_class = socket_option_traffic_class_dsc_set(NS_DSCP_EF);
 * socket_setsockopt(socket_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_TCLASS, &traffic_class, sizeof traffic_class);
 *
 * //Set traffic class back to default
 * traffic_class = socket_option_traffic_class_dsc_set(NS_DSCP_DEFAULT);
 * socket_setsockopt(socket_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_TCLASS, &traffic_class, sizeof traffic_class);
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
    uint8_t event_type;     /**< Socket Callback Event check list below */
    int8_t socket_id;       /**< Socket id queue which socket cause call back */
    int8_t interface_id;    /**< Network Interface ID where Packet came */
    uint16_t  d_len;        /**< Data length if event type is SOCKET_DATA */
    uint8_t LINK_LQI;       /**< LINK LQI info if interface cuold give */
} socket_callback_t;

/*!
 * \struct ns_msghdr
 * \brief Normal IP socket message structure for socket_recvmsg() and socket_sendmsg().
 */

typedef struct ns_msghdr {
    void *msg_name;                 /**< Optional address send use for destination and receive it will be source. MUST be ns_address_t */
    uint_fast16_t msg_namelen;      /**< Length of optional address use sizeof(ns_address_t) when msg_name is defined */
    ns_iovec_t *msg_iov;            /**< Message data vector list */
    uint_fast16_t  msg_iovlen;      /**< Data vector count in msg_iov */
    void *msg_control;              /**< Ancillary data list of ns_cmsghdr_t pointer */
    uint_fast16_t  msg_controllen;  /**< Ancillary data length */
    int msg_flags;                  /**< Flags for received messages */
} ns_msghdr_t;

/*!
 * \struct ns_cmsghdr
 * \brief Control messages.
 */
typedef struct ns_cmsghdr {
    uint16_t cmsg_len;      /**< Ancillary data control messages length including cmsghdr */
    uint8_t cmsg_level;     /**< Originating protocol level should be SOCKET_IPPROTO_IPV6 */
    uint8_t cmsg_type;      /**< Protocol Specific types for example SOCKET_IPV6_PKTINFO,  */
} ns_cmsghdr_t;

/** \name  Error values
 * \anchor ERROR_CODES
 */
///@{
/** No data currently available to read, or insufficient queue space for write */
#define NS_EWOULDBLOCK (-100)
///@}

/** \name socket_recvfrom() or socket_recvmsg() flags.
 * \anchor MSG_HEADER_FLAGS
 */
///@{
/** In msg_flags, indicates that given data buffer was smaller than received datagram. Can also be passed as an flag parameter to recvmsg. */
#define NS_MSG_TRUNC    1
/** Indicates that given ancillary data buffer was smaller than enabled at socket msg->msg_controllen define proper writed data lengths. */
#define NS_MSG_CTRUNC   2
/** Can be passed as an input flag to socket_recvfrom() to not consume data */
#define NS_MSG_PEEK     4
/** \deprecated Can be passed as an input flag to get legacy returns of zero - used by socket_read() and socket_sendto() */
#define NS_MSG_LEGACY0  0x4000
///@}
/*!
 * \struct ns_in6_pktinfo
 * \brief IPv6 packet info which is used for socket_recvmsg() socket_sendmsg().
 */
typedef struct ns_in6_pktinfo {
    uint8_t ipi6_addr[16];    /**< src/dst IPv6 address */
    int8_t  ipi6_ifindex;    /**< send/recv interface index */
} ns_in6_pktinfo_t;

/** \name Socket DSCP (Differentiated Services Code Point) QoS level examples.
 * \anchor MSG_QOS_LEVELS
 */
///@{
/** Standard priority and it is socket default */
#define NS_DSCP_DEFAULT 0
/** Application high priority service: Stack priorities these messages over the default priority messages */
#define NS_DSCP_AF11 10
/** Expedited Forwarding (EF) QoS level enable high priority state: low loss, low delay, and low jitter services */
#define NS_DSCP_EF 46
/** Network protocol traffic allocated QoS level stack may internally use that */
#define NS_DSCP_CS6 48
///@}

/** Helper Traffic class Differentiated Services Code for QoS  0-63. 0 is default which define Lowest Priority
 *
 * */
#define socket_option_traffic_class_dsc_set(x)  (uint8_t)((x & 63) << 2)

/** \name Alignment macros for control message headers
* \anchor CMSG_ALIGN_FLAGS
*/
///@{
/** Base header alignment size */
#define CMSG_HEADER_ALIGN sizeof(long)
/** Base data alignment size */
#define CMSG_DATA_ALIGN CMSG_HEADER_ALIGN
/** Returns control message alignment size for data or header based upon alignment base */
#ifndef NS_ALIGN_SIZE
#define NS_ALIGN_SIZE(length, aligment_base) \
    ((length + (aligment_base -1 )) & ~(aligment_base -1))
#endif
///@}

/**
 * \brief Parse first control message header from message ancillary data.
 *
 * \param msgh Pointer for socket message.
 *
 * \return Pointer to first control message header , Could be NULL when control_message is undefined.
 */
#define NS_CMSG_FIRSTHDR(msgh) \
    ((msgh)->msg_controllen >= sizeof(struct ns_cmsghdr) ? \
            (struct ns_cmsghdr *)(msgh)->msg_control : \
            (struct ns_cmsghdr *)NULL )
/**
 * \brief Parse next control message from message by current control message header.
 *
 * \param msgh Pointer for socket message.
 * \param cmsg Pointer for last parsed control message
 *
 * \return Pointer to Next control message header , Could be NULL when no more control messages data.
 */

ns_cmsghdr_t *NS_CMSG_NXTHDR(const ns_msghdr_t *msgh, const ns_cmsghdr_t *cmsg);

/**
 * \brief Get Data pointer from control message header.
 *
 * \param cmsg Pointer (ns_cmsghdr_t) for last parsed control message
 *
 * \return Data pointer.
 */
#define NS_CMSG_DATA(cmsg) \
    ((uint8_t *)(cmsg) + NS_ALIGN_SIZE(sizeof(ns_cmsghdr_t), CMSG_DATA_ALIGN))

/**
 * \brief Returns the total space required for a cmsg header plus the specified data, allowing for all padding
 *
 * \param length  size of the ancillary data
 *
 * For example, the space required for a SOCKET_IPV6_PKTINFO is NS_CMSG_SPACE(sizeof(ns_in6_pktinfo))
 *
 * \return Total size of CMSG header, data and trailing padding.
 */
#define NS_CMSG_SPACE(length) \
    (NS_ALIGN_SIZE(sizeof(ns_cmsghdr_t), CMSG_DATA_ALIGN) + NS_ALIGN_SIZE(length, CMSG_HEADER_ALIGN))

/**
 * \brief Calculate length to store in cmsg_len with taking into account any necessary alignment.
 *
 * \param length  size of the ancillary data
 *
 * For example, cmsg_len for a SOCKET_IPV6_PKTINFO is NS_CMSG_LEN(sizeof(ns_in6_pktinfo))
 *
 * \return Size of CMSG header plus data, without trailing padding.
 */
#define NS_CMSG_LEN(length) \
    (NS_ALIGN_SIZE(sizeof(ns_cmsghdr_t), CMSG_DATA_ALIGN) + length)

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
int8_t socket_open(uint8_t protocol, uint16_t identifier, void (*passed_fptr)(void *));

/**
 * \brief A function to close a socket.
 *
 * \param socket ID of the socket to be closed.
 *
 * \return 0 socket closed.
 * \return -1 socket not closed.
 */
int8_t socket_close(int8_t socket);

/**
 * \brief A function to set a socket to listening mode.
 *
 * \param socket The socket ID.
 * \param backlog The pending connections queue size.
 * \return 0 on success.
 * \return -1 on failure.
 */
int8_t socket_listen(int8_t socket, uint8_t backlog);

/**
 * \brief A function to accept a new connection on an socket.
 *
 * \param socket_id The socket ID of the listening socket.
 * \param addr Either NULL pointer or pointer to structure where the remote address of the connecting host is copied.
 * \param passed_fptr A function pointer to a function that is called whenever a data frame is received to the new socket.
 * \return 0 or greater on success; return value is the new socket ID.
 * \return -1 on failure.
 * \return NS_EWOULDBLOCK if no pending connections.
 */
int8_t socket_accept(int8_t socket_id, ns_address_t *addr, void (*passed_fptr)(void *));

/**
 * \brief A function to connect to remote peer (TCP).
 *
 * \param socket The socket ID.
 * \param address The address of a remote peer.
 * \deprecated \param randomly_take_src_number Ignored - random local port is always chosen if not yet bound
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
int8_t socket_connect(int8_t socket, ns_address_t *address, uint8_t randomly_take_src_number);

/**
 * \brief Bind socket to address.
 *
 * Used by the application to bind a socket to a port and/or an address. Binding of each
 * of address and port can only be done once.
 *
 * If address is ns_in6addr_any, the address binding is not changed. If port is 0,
 * the port binding is not changed.
 *
 * \param socket Socket ID of the socket to bind.
 * \param address Address structure containing the port and/or address to bind.
 *
 * \return 0 on success.
 * \return -1 if the given address is NULL.
 * \return -2 if the port is already bound to another socket.
 * \return -3 if address is not us.
 * \return -4 if the socket is already bound.
 * \return -5 bind is not supported on this type of socket.
 *
 */
int8_t socket_bind(int8_t socket, const ns_address_t *address);

/**
 * \brief Bind a local address to a socket based on the destination address and
 *  the address selection preferences.
 *
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
int8_t socket_bind2addrsel(int8_t socket, const ns_address_t *dst_address);

/**
 * Options for the socket_shutdown() parameter 'how'.
 */
#define SOCKET_SHUT_RD      0   ///< Disables further receive operations.
#define SOCKET_SHUT_WR      1   ///< Disables further send operations.
#define SOCKET_SHUT_RDWR    2   ///< Disables further send and receive operations.

/**
 * \brief A function to shut down a connection.
 *
 * \param socket The ID of the socket to be shut down.
 * \param how How socket is to be shut down, one of SOCKET_SHUT_XX.
 *
 * \return 0 on success.
 * \return -1 if the given socket ID is not found, if the socket type is wrong or TCP layer returns a failure.
 * \return -2 if socket is not connected.
 */
int8_t socket_shutdown(int8_t socket, uint8_t how);

/**
 * \brief Send data via a connected socket by client.
 *
 * Note: The socket connection must be ready before using this function.
 * The stack uses automatically the address of the remote connected host as the destination address for the packet.
 *
 * This call is equivalent to socket_sendto() with address set to NULL - see
 * that call for more details.
 *
 * \param socket The socket ID.
 * \param buffer A pointer to data.
 * \param length Data length.
 */
int16_t socket_send(int8_t socket, const void *buffer, uint16_t length);

/**
 * \brief A function to read received data buffer from a socket.
 * \deprecated
 *
 * Used by the application to get data from a socket. See socket_recvfrom()
 * for more details.
 *
 * This is equivalent to socket_recvfrom, except that it passes the
 * flag NS_MSG_LEGACY0, which modifies the return behaviour for zero data.
 *
 * \param socket The socket ID.
 * \param src_addr A pointer to a structure where the sender's address is stored.
 *                 May be NULL if not required.
 * \param buffer A pointer to an array where the read data is written to.
 * \param length The maximum length of the allocated buffer.
 *
 * \return >0 indicates the length of the data copied to buffer.
 * \return 0 if no data was read (includes zero-length datagram,
 *           end of stream and no data currently available)
 * \return -1 invalid input parameters.
 */
int16_t socket_read(int8_t socket, ns_address_t *src_addr, uint8_t *buffer, uint16_t length);

/**
 * \brief A function to read received data buffer from a socket,
 *
 * Equivalent to socket_recvfrom with src_address set to NULL.
 *
 * \param socket The socket ID.
 * \param buffer A pointer to an array where the read data is written to.
 * \param length The maximum length of the allocated buffer.
 * \param flags Flags for read call
 *
 * \return as for socket_recvfrom
 */
int16_t socket_recv(int8_t socket, void *buffer, uint16_t length, int flags);

/**
 * \brief A function to read received data buffer from a socket
 *
 * Used by the application to get data from a socket.
 *
 * This has two modes of operation.
 *
 * 1) For non-stream sockets, if the receive queue is disabled (set to 0 via
 *    SOCKET_SO_RCVBUF), which is the non-stream default and original Nanostack
 *    behaviour, then applications receive exactly one SOCKET_DATA callback per
 *    datagram, indicating that datagram's length. They must make 1 read call
 *    in that callback, and they will be given the data. If not read, the
 *    datagram is discarded on return from the callback.
 *
 * 2) Otherwise - stream sockets or SOCKET_SO_RCVBUF non-zero - behaviour is
 *    akin to traditional BSD. SOCKET_DATA callbacks occur when new data arrives,
 *    and read calls can be made any time. Data will be queued to an extent
 *    determined by the receive buffer size. The length in the data callback
 *    is the total amount of data in the receive queue - possibly multiple
 *    datagrams.
 *
 * \param socket The socket ID.
 * \param buffer A pointer to an array where the read data is written to.
 * \param length The maximum length of the allocated buffer.
 * \param flags Flags for read call
 * \param src_addr A pointer to a structure where the sender's address is stored.
 *                 May be NULL if not required.
 *
 * The returned length is normally the length of data actually written to the buffer; if
 * NS_MSG_TRUNC is set in flags, then for non-stream sockets, the actual datagram length is
 * returned instead, which may be larger than the buffer size.
 *
 * Return values assume flag NS_MSG_LEGACY0 is not set - if it is set, they are
 * as per socket_read().
 *
 * \return >0 indicates the length of the data copied to buffer (or original datagram size)
 * \return 0 if end of stream or zero-length datagram
 * \return -1 invalid input parameters.
 * \return NS_EWOULDBLOCK if no data is currently available
 */
int16_t socket_recvfrom(int8_t socket, void *buffer, uint16_t length, int flags, ns_address_t *src_addr);

/**
 * \brief A function to read received message with ancillary data from a socket.
 *
 * Used by the application to get data from a socket. See socket_recvfrom for
 * details of the two delivery mechanisms.
 *
 * Ancillary data must request by socket_setsockopt().
 *
 * msg->msg_controllen is updated to indicate actual length of ancillary data output
 *
 * \param socket The socket ID.
 * \param msg A pointer to a structure where messages is stored with or without ancillary data
 * \param flags A flags for message read.
 *
 * \return as for socket_recvfrom
 */
int16_t socket_recvmsg(int8_t socket, ns_msghdr_t *msg, int flags);

/**
 * \brief A function to send UDP, TCP or raw ICMP data via the socket.
 *
 * Used by the application to send data.
 *
 * The return of 0 on success is unconventional, and obtained by passing
 * NS_MSG_LEGACY0 to socket_sendmsg internally - to get conventional
 * return values, you can use socket_sendmsg() instead.
 *
 * \param socket The socket ID.
 * \param address A pointer to the destination address information.
 * \param buffer A pointer to data to be sent.
 * \param length Length of the data to be sent.
 *
 * \return 0 On success (whole packet queued)
 * \return NS_EWOULDBLOCK if nothing written due to lack of queue space.
 *
 * Error returns:
 *
 * \return -1 Invalid socket ID.
 * \return -2 Socket memory allocation fail.
 * \return -3 TCP state not established or address scope not defined .
 * \return -4 Unknown interface.
 * \return -5 Socket not connected
 * \return -6 Packet too short (ICMP raw socket error).
 */
int16_t socket_sendto(int8_t socket, const ns_address_t *address, const void *buffer, uint16_t length);

/**
 * \brief A function to send UDP, TCP or raw ICMP data via the socket with or without ancillary data or destination address.
 *
 * Used by the application to send data message header support also vector list socket_send() and socket_sendto() use this functionality internally.
 *
 * \param socket The socket ID.
 * \param msg A pointer to the Message header which include address, payload and ancillary data.
 * \param flags A flags for message send (eg NS_MSG_LEGACY0)
 *
 * Messages destination address is defined by msg->msg_name which must be ns_address_t. If msg->msg_nme is NULL socket select connected address
 *
 * Messages payload and length is defined msg->msg_iov and msg->msg_iovlen. API support to send multiple data vector.
 *
 * Supported ancillary data for send defined by msg->msg_control and msg->msg_controllen.
 *
 * msg->msg_flags is unused, and need not be initialised.
 *
 * The following main return values assume flag NS_MSG_LEGACY0 is not set -
 * if it is set, they are as per socket_sendto().
 *
 * \return length if entire amount written (which could be 0)
 * \return value >0 and <length if partial amount written (stream only)
 * \return NS_EWOULDBLOCK if nothing written due to lack of queue space.

 * Error returns:
 *
 * \return -1 Invalid socket ID.
 * \return -2 Socket memory allocation fail.
 * \return -3 TCP state not established or address scope not defined .
 * \return -4 Unknown interface.
 * \return -5 Socket not connected
 * \return -6 Packet too short (ICMP raw socket error).
 */
int16_t socket_sendmsg(int8_t socket, const ns_msghdr_t *msg, int flags);

/**
 * \brief A function to read local address and port for a bound socket.
 *
 * This call writes ns_in6addr_any if address is not bound and 0 if the port is not bound.
 *
 * \param socket The socket ID.
 * \param address A pointer to the address structure where the local address information is written to.
 *
 * \return 0 on success.
 * \return -1 if no socket is found.
 */
int8_t socket_getsockname(int8_t socket, ns_address_t *address);

/**
 * \brief A function to read remote address and port for a connected socket.
 *
 * \param socket The socket ID.
 * \param address A pointer to the address structure where the remote address information is written to.
 *
 * \return 0 on success.
 * \return -1 if no socket is found.
 * \return -2 if no socket is not connected.
 */
int8_t socket_getpeername(int8_t socket, ns_address_t *address);

/* Backwards compatibility */
static inline int8_t socket_read_session_address(int8_t socket, ns_address_t *address)
{
    return socket_getpeername(socket, address);
}

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
#define SOCKET_SOL_SOCKET           0   /**< Socket level */
#define SOCKET_IPPROTO_IPV6         41  /**< IPv6. */
///@}

/** \name Option names for protocol level SOCKET_SOL_SOCKET.
 * \anchor OPTNAMES_SOCKET
 */
///@{
/** Specify receive buffer size in payload bytes, as int32_t. 0 means traditional Nanostack behaviour - unread data dropped unless read in data callback */
#define SOCKET_SO_RCVBUF                    1
/** Specify send buffer size in payload bytes, as int32_t. Only currently used for stream sockets. */
#define SOCKET_SO_SNDBUF                    2
/** Specify receive low water mark in payload bytes, as int32_t. Not yet implemented. */
#define SOCKET_SO_RCVLOWAT                  3
/** Specify send low water mark in payload bytes, as int32_t. Queued sends will only be accepted if this many bytes of send queue space are available, else NS_EWOULDBLOCK is returned.  */
#define SOCKET_SO_SNDLOWAT                  4
///@}

/** \name IPv6 socket options
 * \anchor OPTNAMES_IPV6
 *
 * IPv6 socket options summary
 *
 * | opt_name / cmsg_type         | Data type         | set/getsockopt  | sendmsg | recvmsg                           |
 * | :--------------------------: | :---------- ----: | :-------------: | :-----: | :-------------------------------: |
 * | SOCKET_IPV6_TCLASS           | int16_t           |     Yes         |   Yes   | If enabled with RECVTCLASS        |
 * | SOCKET_IPV6_UNICAST_HOPS     | int16_t           |     Yes         |   No    | No                                |
 * | SOCKET_IPV6_MULTICAST_HOPS   | int16_t           |     Yes         |   No    | No                                |
 * | SOCKET_IPV6_ADDR_PREFERENCES | int               |     Yes         |   No    | No                                |
 * | SOCKET_IPV6_USE_MIN_MTU      | int8_t            |     Yes         |   Yes   | No                                |
 * | SOCKET_IPV6_DONTFRAG         | int8_t            |     Yes         |   Yes   | No                                |
 * | SOCKET_IPV6_FLOW_LABEL       | int32_t           |     Yes         |   No    | No                                |
 * | SOCKET_IPV6_HOPLIMIT         | int16_t           |     No          |   Yes   | If enabled with RECVHOPLIMIT      |
 * | SOCKET_IPV6_PKTINFO          | ns_in6_pktinfo_t  |     No          |   Yes   | If enabled with RECVPKTINFO       |
 * | SOCKET_IPV6_RECVPKTINFO      | bool              |     Yes         |   No    | No                                |
 * | SOCKET_IPV6_RECVHOPLIMIT     | bool              |     Yes         |   No    | No                                |
 * | SOCKET_IPV6_RECVTCLASS       | bool              |     Yes         |   No    | No                                |
 * | SOCKET_IPV6_MULTICAST_IF     | int8_t            |     Yes         |   No    | No                                |
 * | SOCKET_IPV6_MULTICAST_LOOP   | bool              |     Yes         |   Yes   | No                                |
 * | SOCKET_IPV6_JOIN_GROUP       | ns_ipv6_mreq_t    |     Set only    |   No    | No                                |
 * | SOCKET_IPV6_LEAVE_GROUP      | ns_ipv6_mreq_t    |     Set only    |   No    | No                                |
 * | SOCKET_LATENCY               | ns_ipv6_latency_t |     Get only    |   No    | No                                |
 * | SOCKET_STAGGER               | ns_ipv6_stagger_t |     Get only    |   No    | No                                |
 * | SOCKET_EDFE_MODE             | bool              |     Set only    |   No    | No                                |
 * | SOCKET_BROADCAST_PAN         | int8_t            |     Yes         |   No    | No                                |
 * | SOCKET_LINK_LAYER_SECURITY   | int8_t            |     Yes         |   No    | No                                |
 * | SOCKET_INTERFACE_SELECT      | int8_t            |     Yes         |   No    | No                                |
 *
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
/** Hop limit control for socket_sendmsg() and indicate for hop limit socket_recmsg(), as int16_t; valid values 0-255, -1 for default for outgoing packet */
#define SOCKET_IPV6_HOPLIMIT                8
/** Specify control messages packet info for send and receive as ns_in6_pktinfo_t socket_sendmsg() it define source address and outgoing interface. socket_recvmsg() it define messages destination address and arrival interface. Reference at RFC3542*/
#define SOCKET_IPV6_PKTINFO                 9

/** Specify socket_recvmsg() ancillary data request state for Packet info (destination address and interface id), as bool; valid values true write enabled, false write disabled */
#define SOCKET_IPV6_RECVPKTINFO             10
/** Specify socket_recvmsg() ancillary data request state for receive messages hop-limit, as bool; valid values true  write enabled, false information write disabled */
#define SOCKET_IPV6_RECVHOPLIMIT            11
/** Specify socket_recvmsg() ancillary data request state for receive messages traffic class, as bool; valid values true  write enabled, false information write disabled */
#define SOCKET_IPV6_RECVTCLASS              12

/** Set the interface to use for outgoing multicast packets, as int8_t (RFC3493 S5.2 says unsigned int); 0 means unspecified (use routing table) */
#define SOCKET_IPV6_MULTICAST_IF            13
/** Specify whether outgoing multicast packets are looped back, as bool (RFC3493 S5.2 says unsigned int) */
#define SOCKET_IPV6_MULTICAST_LOOP          14
/** Join a multicast group, using ns_ipv6_mreq_t */
#define SOCKET_IPV6_JOIN_GROUP              15
/** Leave a multicast group, using ns_ipv6_mreq_t */
#define SOCKET_IPV6_LEAVE_GROUP             16

#define SOCKET_LATENCY                      0xf9 /**< Not standard, read estimated latency to reach destination */
#define SOCKET_STAGGER                      0xfa /**< Not standard, read estimated stagger value that can be used as initial delay after bootstrap or firmware update. */
#define SOCKET_EDFE_MODE                    0xfb /**< Not standard, Extended Directed Frame Exchange mode enabled/disabled in MAC layer */
#define SOCKET_BROADCAST_PAN                0xfc /**< Internal use - transmit with IEEE 802.15.4 broadcast PAN ID */
#define SOCKET_LINK_LAYER_SECURITY          0xfd /**< Not standard enable or disable socket security at link layer (For 802.15.4). */
#define SOCKET_INTERFACE_SELECT             0xfe /**< Not standard socket interface ID. */
#define SOCKET_IPV6_ADDRESS_SELECT          0xff /**< Deprecated - use SOCKET_IPV6_ADDR_PREFERENCES instead. */

/** Multicast group request used for setsockopt() */
typedef struct ns_ipv6_mreq {
    uint8_t ipv6mr_multiaddr[16];       /**< IPv6 multicast address */
    int8_t ipv6mr_interface;            /**< interface id */
} ns_ipv6_mreq_t;

/** Latency request used for getsockopt() */
typedef struct {
    uint8_t dest_addr[16];      /**< [IN] IPv6 destination address */
    uint32_t latency;           /**< [OUT] estimated latency value in milliseconds */
} ns_ipv6_latency_t;

/** Stagger request used for getsockopt() */
typedef struct {
    uint8_t dest_addr[16];      /**< [IN] IPv6 destination address */
    uint16_t data_amount;       /**< [IN] Amount of data in kilobytes */
    uint16_t stagger_min;       /**< [OUT] Minimum stagger value in seconds */
    uint16_t stagger_max;       /**< [OUT] Maximum stagger value in seconds */
    uint16_t stagger_rand;      /**< [OUT] Randomized stagger value in seconds */
} ns_ipv6_stagger_t;
/**
 * \brief Set an option for a socket
 *
 * Used to specify miscellaneous options for a socket. Supported levels and
 * names defined above.
 *
 * \param socket The socket ID.
 * \param level The protocol level.
 * \param opt_name The option name (interpretation depends on level).
 *                 See \ref OPTNAMES_SOCKET and \ref OPTNAMES_IPV6.
 * \param opt_value A pointer to value for the specified option.
 * \param opt_len Size of the data pointed to by the value.
 *
 * \return 0 on success.
 * \return -1 invalid socket ID.
 * \return -2 invalid/unsupported option.
 * \return -3 invalid option value.
 */
int8_t socket_setsockopt(int8_t socket, uint8_t level, uint8_t opt_name, const void *opt_value, uint16_t opt_len);

///@}

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
 * \return -3 data can't be retrieved.
 */
int8_t socket_getsockopt(int8_t socket, uint8_t level, uint8_t opt_name, void *opt_value, uint16_t *opt_len);

#ifdef __cplusplus
}
#endif
#endif /*_NS_SOCKET_H*/
