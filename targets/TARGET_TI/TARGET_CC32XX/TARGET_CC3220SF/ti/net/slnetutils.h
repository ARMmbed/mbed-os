/*
 * Copyright (c) 2017-2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#ifndef __SL_NET_UTILS_H__
#define __SL_NET_UTILS_H__

#include "slnetsock.h"

#ifdef    __cplusplus
extern "C" {
#endif

/*!
    \defgroup SlNetUtils SlNetUtils group

    \short Sockets related commands and configuration

*/
/*!

    \addtogroup SlNetUtils
    @{

*/

/*****************************************************************************/
/* Structure/Enum declarations                                               */
/*****************************************************************************/

/* Creating one address parameter from 4 separate address parameters */
#define SLNETUTIL_IPV4_VAL(add_3,add_2,add_1,add_0)                         ((((uint32_t)add_3 << 24) & 0xFF000000) | (((uint32_t)add_2 << 16) & 0xFF0000) | (((uint32_t)add_1 << 8) & 0xFF00) | ((uint32_t)add_0 & 0xFF) )

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/

/*!

    \brief Initialize the SlNetUtil module

    \param[in] flags            Reserved

    \return                     Zero on success, or negative error code on failure
*/
int32_t SlNetUtil_init(int32_t flags);

/*!
    \brief Get host IP by name\n
    Obtain the IP Address of machine on network, by machine name.

    \param[in]     ifBitmap     Specifies the interfaces which the the host ip
                                needs to be retrieved from according tothe
                                priority until one of them will return an answer.\n
                                Value 0 is used in order to choose automatic
                                interfaces selection according to the priority
                                interface list.
                                Value can be combination of interfaces by OR'ing
                                multiple interfaces bit identifiers (SLNETIFC_IDENT_
                                defined in slnetif.h)
                                Note: interface identifier bit must be configured
                                prior to this socket creation using SlNetIf_add().
    \param[in]     name         Host name
    \param[in]     nameLen      Name length
    \param[out]    ipAddr       This parameter is filled in with an array of
                                IP addresses. In case that host name is not
                                resolved, ipAddrLen is zero.
    \param[in,out] ipAddrLen    Holds the size of the ipAddr array, when function
                                successful, the ipAddrLen parameter will be updated with
                                the number of the IP addresses found.
    \param[in]     family       Protocol family

    \return                     The interface ID of the interface which run
                                successfully the function, or negative on failure.\n
                                #SLNETERR_POOL_IS_EMPTY may be return in case
                                there are no resources in the system\n
                                Possible DNS error codes:
                                - #SLNETERR_NET_APP_DNS_QUERY_NO_RESPONSE
                                - #SLNETERR_NET_APP_DNS_NO_SERVER
                                - #SLNETERR_NET_APP_DNS_QUERY_FAILED
                                - #SLNETERR_NET_APP_DNS_MALFORMED_PACKET
                                - #SLNETERR_NET_APP_DNS_MISMATCHED_RESPONSE

    \slnetutil_init_precondition

    \warning
            In case an IP address in a string format is set as input, without
            any prefix (e.g. "1.2.3.4") the device will not try to access the
            DNS and it will return the input address in the \c ipAddr field
    \par  Example
    - Getting IPv4 using get host by name:
    \code
    uint16_t DestIPListSize = 1;
    uint32_t DestIP[1];
    uint32_t ifID;
    int16_t  SockId;
    SlNetSock_AddrIn_t LocalAddr; //address of the server to connect to
    int32_t LocalAddrSize;

    ifID = SlNetUtil_getHostByName(0, "www.google.com", strlen("www.google.com"), DestIP, &DestIPListSize, SLNETSOCK_PF_INET);

    LocalAddr.sin_family = SLNETSOCK_AF_INET;
    LocalAddr.sin_addr.s_addr = SlNetUtil_htonl(DestIP[0]);
    LocalAddr.sin_port = SlNetUtil_htons(80);
    LocalAddrSize = sizeof(SlNetSock_AddrIn_t);

    SockId = SlNetSock_create(SLNETSOCK_AF_INET, SLNETSOCK_SOCK_STREAM, ifID, 0);

    if (SockId >= 0)
    {
        status = SlNetSock_connect(SockId, (SlNetSock_Addr_t *)&LocalAddr, LocalAddrSize);
    }
    \endcode
*/
int32_t SlNetUtil_getHostByName(uint32_t ifBitmap, char *name, const uint16_t nameLen, uint32_t *ipAddr, uint16_t *ipAddrLen, const uint8_t family);


/*!
    \brief Reorder the bytes of a 32-bit unsigned value

    This function is used to reorder the bytes of a 32-bit unsigned value
    from host order to network order.

    \param[in] val              Variable in host order

    \return                     Return the variable in network order

    \slnetutil_init_precondition

    \sa         SlNetSock_bind()
    \sa         SlNetSock_connect()
    \sa         SlNetSock_recvFrom()
    \sa         SlNetSock_accept()
*/
uint32_t SlNetUtil_htonl(uint32_t val);


/*!
    \brief Reorder the bytes of a 32-bit unsigned value

    This function is used to reorder the bytes of a 32-bit unsigned
    value from network order to host order.

    \param[in] val              Variable in network order

    \return                     Return the variable in host order

    \slnetutil_init_precondition

    \sa         SlNetSock_bind()
    \sa         SlNetSock_connect()
    \sa         SlNetSock_recvFrom()
    \sa         SlNetSock_accept()
*/
uint32_t SlNetUtil_ntohl(uint32_t val);


/*!
    \brief Reorder the bytes of a 16-bit unsigned value

    This functions is used to reorder the bytes of a 16-bit unsigned
    value from host order to network order.

    \param[in] val              Variable in host order

    \return                     Return the variable in network order

    \slnetutil_init_precondition

    \sa         SlNetSock_bind()
    \sa         SlNetSock_connect()
    \sa         SlNetSock_recvFrom()
    \sa         SlNetSock_accept()
*/
uint16_t SlNetUtil_htons(uint16_t val);


/*!
    \brief Reorder the bytes of a 16-bit unsigned value

    This functions is used to reorder the bytes of a 16-bit unsigned value
    from network order to host order.

    \param[in] val              Variable in network order

    \return                     Return the variable in host order

    \slnetutil_init_precondition

    \sa         SlNetSock_bind()
    \sa         SlNetSock_connect()
    \sa         SlNetSock_recvFrom()
    \sa         SlNetSock_accept()
*/
uint16_t SlNetUtil_ntohs(uint16_t val);


/*!
    \brief Converts IP address in binary representation to string representation

    This functions is used to converts IP address in binary representation
    to IP address in string representation.

    \param[in]  addrFamily     Specifies the address family of the created
                               socket
                               For example:
                                 - #SLNETSOCK_AF_INET for network address IPv4
                                 - #SLNETSOCK_AF_INET6 for network address IPv6
    \param[in]  binaryAddr     Pointer to an IP address structure indicating the
                               address in binary representation
    \param[out] strAddr        Pointer to the address string representation
                               for IPv4 or IPv6 according to the address
                               family
    \param[in]  strAddrLen     Specifies the length of the StrAddress_dst,
                               the maximum length of the address in string
                               representation for IPv4 or IPv6 according to
                               the address family

    \return                    strAddr on success, or NULL on failure

    \slnetutil_init_precondition

    \par    Example
    - IPv4 demo of inet_ntop()
    \code
        SlNetSock_AddrIn_t sa;
        char str[SLNETSOCK_INET_ADDRSTRLEN];

        // store this IP address in sa:
        SlNetUtil_inetPton(SLNETSOCK_AF_INET, "192.0.2.33", &(sa.sin_addr));
        // now get it back and print it
        SlNetUtil_inetNtop(SLNETSOCK_AF_INET, &(sa.sin_addr), str, SLNETSOCK_INET_ADDRSTRLEN);
    \endcode
*/
const char *SlNetUtil_inetNtop(int16_t addrFamily, const void *binaryAddr, char *strAddr, SlNetSocklen_t strAddrLen);


/*!
    \brief Converts IP address in string representation to binary representation

    This functions is used to converts IP address in string representation
    to IP address in binary representation.

    \param[in]  addrFamily     Specifies the address family of the created
                               socket
                               For example:
                                 - #SLNETSOCK_AF_INET for network address IPv4
                                 - #SLNETSOCK_AF_INET6 for network address IPv6
    \param[out] strAddr        Specifies the IP address in string representation
                               for IPv4 or IPv6 according to the address
                               family
    \param[in]  binaryAddr     Pointer to an address structure that will be
                               filled by the IP address in Binary representation

    \return                    1 on success, -1 on failure, or 0 if the input
                               isn't a valid IP address

    \slnetutil_init_precondition

    \par    Example
    - IPv6 demo of inet_pton()
    \code
        SlNetSock_AddrIn6_t sa;

        // store this IP address in sa:
        SlNetUtil_inetPton(SLNETSOCK_AF_INET6, "0:0:0:0:0:0:0:0", &(sa.sin6_addr));
    \endcode
*/
int32_t SlNetUtil_inetPton(int16_t addrFamily, const char *strAddr, void *binaryAddr);

/*!

 Close the Doxygen group.
 @}

*/


#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SL_NET_UTILS_H__ */
