/**************************************************************************
*
* Copyright 2011-2016 by Andrey Butok. FNET Community.
* Copyright 2008-2010 by Andrey Butok. Freescale Semiconductor, Inc.
*
***************************************************************************
*
*  Licensed under the Apache License, Version 2.0 (the "License"); you may
*  not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
*  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
**********************************************************************/
/*!
* @brief IP protocol API.
*
***************************************************************************/

#ifndef _FNET_IP_H_

#define _FNET_IP_H_

/*! @addtogroup fnet_socket */
/*! @{ */

/**************************************************************************/ /*!
 * @brief 32-bit IPv4 address type.
 ******************************************************************************/
typedef fnet_uint32_t fnet_ip4_addr_t;

/**************************************************************************/ /*!
 * @def FNET_IP4_ADDR_INIT
 * @param a First octet of an IP address.
 * @param b Second octet of an IP address.
 * @param c Third  octet of an IP address.
 * @param d Fourth  octet of an IP address.
 * @hideinitializer
 * @brief Converts the standard dotted-decimal notation @c a.b.c.d
 *        to an integer value, suitable for use as an Internet address (in network byte order).
 ******************************************************************************/
#define FNET_IP4_ADDR_INIT(a, b, c, d)   ((fnet_ip4_addr_t)(FNET_NTOHL((((fnet_uint32_t)(a)&0xFFUL)<< 24U) + (((fnet_uint32_t)(b)&0xFFUL)<< 16U) + (((fnet_uint32_t)(c)&0xFFUL)<< 8U ) + ((fnet_uint32_t)(d)&0xFFUL))))

/**************************************************************************/ /*!
 * @def     FNET_IP4_ADDR_STR_SIZE
 * @brief   Size of the string buffer that will contain
 *          null-terminated ASCII string of an IPv4 address
 *          in standard "." notation.
 * @see fnet_inet_ntoa, fnet_inet_ntop
 * @showinitializer
 ******************************************************************************/
#define FNET_IP4_ADDR_STR_SIZE          sizeof("255.255.255.255")

/* 169.254/16 prefix that is valid for Link-Local communication. RFC3927*/
#define FNET_IP4_ADDR_LINK_LOCAL_PREFIX      FNET_IP4_ADDR_INIT(169,254,0,0)

/* IPv4 Link-Local broadcast. RFC3927*/
#define FNET_IP4_ADDR_LINK_LOCAL_BROADCAST   FNET_IP4_ADDR_INIT(169,254,255,255)

/************************************************************************
*    Definitions for options.
*************************************************************************/
/* The type field is divided into three internal fields:*/
#define IPOPT_COPIED(t)   ((t)&0x80U)    /* 1-bit copied flag */
#define IPOPT_CLASS (t)   ((t)&0x60U)    /* 2-bit class field */
#define IPOPT_NUMBER(t)   ((t)&0x1fU)    /* 5-bit number field */
/* Class field: */
#define IPOPT_CONTROL     (0x00U)        /* control */
#define IPOPT_RESERVED1   (0x20U)        /* reserved */
#define IPOPT_DEBMEAS     (0x40U)        /* debugging and measurement */
#define IPOPT_RESERVED2   (0x60U)        /* reserved */
/* Currently defined IP options */
#define IPOPT_EOL         (0U)           /* end of option list */
#define IPOPT_NOP         (1U)           /* no operation */

#define IPOPT_RR          (7U)           /* record  route */
#define IPOPT_TS          (68U)          /* timestamp */
#define IPOPT_SECURITY    (130U)         /* basic security */
#define IPOPT_LSRR        (131U)         /* loose source and record route */
#define IPOPT_SATID       (136U)         /* stream id */
#define IPOPT_SSRR        (137U)         /* strict source and record route */

#define IPOPT_TYPE        (0U)
#define IPOPT_LENGTH      (1U)
#define IPOPT_OFFSET      (2U)
#define IPOPT_OFFSET_MIN  (4U)           /* minimum value of 'offset'*/

/************************************************************************
*    Definitions for IP type of service.
*************************************************************************/
#define IP_TOS_NORMAL      (0x0U)
#define IP_TOS_LOWDELAY    (0x10U)
#define IP_TOS_THROUGHPUT  (0x08U)
#define IP_TOS_RELIABILITY (0x04U)

/************************************************************************
*    Timestamp option
*************************************************************************/
#define IPOPT_TS_FLAG_TSONLY     (0U)    /* Record timestamps.*/
#define IPOPT_TS_FLAG_TSANDADDR  (1U)    /* Record addresses and timestamps.*/
#define IPOPT_TS_FLAG_TSPRESPEC  (3U)    /* Record timestamps only at the prespecified systems.*/

/************************************************************************
*    Definitions of five different classes.
*************************************************************************/
#define FNET_IP4_CLASS_A(i)     (((fnet_ip4_addr_t)(i) & FNET_HTONL(0x80000000U)) == 0U)
#define FNET_IP4_CLASS_A_NET    FNET_HTONL(0xff000000U)

#define FNET_IP4_CLASS_B(i)     (((fnet_ip4_addr_t)(i) & FNET_HTONL(0xc0000000U)) == FNET_HTONL(0x80000000U))
#define FNET_IP4_CLASS_B_NET    FNET_HTONL(0xffff0000U)

#define FNET_IP4_CLASS_C(i)     (((fnet_ip4_addr_t)(i) & FNET_HTONL(0xe0000000U)) == FNET_HTONL(0xc0000000U))
#define FNET_IP4_CLASS_C_NET    FNET_HTONL(0xffffff00U)

#define FNET_IP4_CLASS_D(i)     (((fnet_ip4_addr_t)(i) & FNET_HTONL(0xf0000000U)) == FNET_HTONL(0xe0000000U))
#define FNET_IP4_CLASS_D_NET    FNET_HTONL(0xf0000000U)
/* Host groups are identified by class D IP addresses.*/
#define FNET_IP4_ADDR_IS_MULTICAST(addr)   (FNET_IP4_CLASS_D(addr)?FNET_TRUE:FNET_FALSE)

#define FNET_IP4_ADDR_IS_UNSPECIFIED(addr) (((addr) == INADDR_ANY)?FNET_TRUE:FNET_FALSE)

#define FNET_IP4_CLASS_E(i)     (((fnet_ip4_addr_t)(i) & FNET_HTONL(0xf0000000U)) == FNET_HTONL(0xf0000000U))
#define FNET_IP4_EXPERIMENTAL(i) FNET_IP4_CLASS_E(i)
#define FNET_IP4_BADCLASS(i)     FNET_IP4_CLASS_E(i)

#define FNET_IP4_ADDR1(ipaddr)   ((fnet_uint8_t)(fnet_ntohl(ipaddr) >> 24U) & 0xffU)
#define FNET_IP4_ADDR2(ipaddr)   ((fnet_uint8_t)(fnet_ntohl(ipaddr) >> 16U) & 0xffU)
#define FNET_IP4_ADDR3(ipaddr)   ((fnet_uint8_t)(fnet_ntohl(ipaddr) >> 8U) & 0xffU)
#define FNET_IP4_ADDR4(ipaddr)   ((fnet_uint8_t)(fnet_ntohl(ipaddr)) & 0xffU)

/*! @} */

#endif
