/*
 * Copyright (c) 2019 ARM Limited
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

#ifndef PPP_SERVICE_IF_H
#define PPP_SERVICE_IF_H

#include "nsapi_types.h"

#include <stdint.h>

typedef uint8_t   u8_t;
typedef int8_t    s8_t;
typedef uint16_t  u16_t;
typedef int16_t   s16_t;
typedef uint32_t  u32_t;
typedef int32_t   s32_t;

typedef s8_t err_t;

#include <inttypes.h>
#ifndef X8_F
#define X8_F  "02" PRIx8
#endif
#ifndef U16_F
#define U16_F PRIu16
#endif
#ifndef S16_F
#define S16_F PRId16
#endif
#ifndef X16_F
#define X16_F PRIx16
#endif
#ifndef U32_F
#define U32_F PRIu32
#endif
#ifndef S32_F
#define S32_F PRId32
#endif
#ifndef X32_F
#define X32_F PRIx32
#endif
#ifndef SZT_F
#define SZT_F PRIuPTR
#endif

struct netif;

typedef enum {
    /** No error, everything OK. */
    ERR_OK         = 0,
    /** Out of memory error.     */
    ERR_MEM        = -1,
    /** Buffer error.            */
    ERR_BUF        = -2,
    /** Timeout.                 */
    ERR_TIMEOUT    = -3,
    /** Routing problem.         */
    ERR_RTE        = -4,
    /** Operation in progress    */
    ERR_INPROGRESS = -5,
    /** Illegal value.           */
    ERR_VAL        = -6,
    /** Operation would block.   */
    ERR_WOULDBLOCK = -7,
    /** Address in use.          */
    ERR_USE        = -8,
    /** Already connecting.      */
    ERR_ALREADY    = -9,
    /** Conn already established.*/
    ERR_ISCONN     = -10,
    /** Not connected.           */
    ERR_CONN       = -11,
    /** Low-level netif error    */
    ERR_IF         = -12,

    /** Connection aborted.      */
    ERR_ABRT       = -13,
    /** Connection reset.        */
    ERR_RST        = -14,
    /** Connection closed.       */
    ERR_CLSD       = -15,
    /** Illegal argument.        */
    ERR_ARG        = -16
} err_enum_t;

/** Eliminates compiler warning about unused arguments */
#ifndef PPP_UNUSED_ARG
#define PPP_UNUSED_ARG(x) (void)x
#endif /* PPP_UNUSED_ARG */

#define PPP_MAX(x , y)  (((x) > (y)) ? (x) : (y))
#define PPP_MIN(x , y)  (((x) < (y)) ? (x) : (y))

typedef nsapi_addr_t ip4_addr_t;
typedef nsapi_addr_t ip6_addr_t;
typedef nsapi_addr_t ip_addr_t;

#define IPADDR_STRLEN_MAX   46

#define IP_CLASSA(a)        ((((u32_t)(a)) & 0x80000000UL) == 0)
#define IP_CLASSA_NET       0xff000000
#define IP_CLASSA_NSHIFT    24
#define IP_CLASSA_HOST      (0xffffffff & ~IP_CLASSA_NET)
#define IP_CLASSA_MAX       128

#define IP_CLASSD(a)        (((u32_t)(a) & 0xf0000000UL) == 0xe0000000UL)
#define IP_CLASSD_NET       0xf0000000          /* These ones aren't really */
#define IP_CLASSD_NSHIFT    28                  /*   net and host fields, but */
#define IP_CLASSD_HOST      0x0fffffff          /*   routing needn't know. */
#define IP_MULTICAST(a)     IP_CLASSD(a)

#define IP_BADCLASS(a)      (((u32_t)(a) & 0xf0000000UL) == 0xf0000000UL)

#define IP_LOOPBACKNET      127                 /* official! */

#define ip4_addr_set_u32(dest_ipaddr, src_u32) \
    ppp_ip4_addr_set((nsapi_addr_t *)dest_ipaddr, &src_u32)

#define ip_addr_set_ip4_u32_val(ipaddr, val) \
    ppp_ip4_addr_set((nsapi_addr_t *)&ipaddr, &val)

struct pbuf {
    struct pbuf *next;     // Next buffer on the chain
    void *memory_manager;  // Memory manager used to allocate buffer
    void *buffer;          // Buffer allocated by memory manager
    void *payload;         // Pointer to payload of the first buffer on the chain (payload_start + headroom)
    void *payload_start;   // Pointer to payload start of the first buffer on the chain
    uint16_t len;          // Length of the first buffer on the chain (equals to total length on alloc)
    uint16_t tot_len;      // Total length of the first buffer on the chain
};

typedef err_t (*netif_input_fn)(struct pbuf *p, struct netif *inp);
#if PPP_IPV4_SUPPORT
typedef err_t (*netif_output_fn)(struct netif *netif, struct pbuf *p,
                                 const ip4_addr_t *ipaddr);
#endif
#if PPP_IPV6_SUPPORT
typedef err_t (*netif_output_ip6_fn)(struct netif *netif, struct pbuf *p,
                                     const ip6_addr_t *ipaddr);
#endif
typedef err_t (*netif_init_fn)(struct netif *netif);

struct netif {
    /** Pointer to PPP Service */
    void *service_ptr;
    /** Pointer to memory manager */
    void *memory_manager;
    /** Pointer to memory stream */
    void *stream;
    /** This function is called by the PPP service
     *  to pass a packet up the TCP/IP stack. */
    netif_input_fn input;
#if PPP_IPV4_SUPPORT
    /** This function is called by the IP module when it wants
     *  to send a packet on the interface. */
    netif_output_fn output;
    nsapi_addr_t ipv4_addr;
    nsapi_addr_t ipv4_netmask;
    nsapi_addr_t ipv4_gateway;
    nsapi_addr_t ipv4_dns_server[2];
    u8_t ipv4_up;
#endif /* PPP_IPV4_SUPPORT */
#if PPP_IPV6_SUPPORT
    /** This function is called by the IPv6 module when it wants
     *  to send a packet on the interface. */
    netif_output_ip6_fn output_ip6;
    nsapi_addr_t ipv6_addr;
    u8_t ipv6_up;
#endif /* PPP_IPV6_SUPPORT */
    /** This field can be set by the PPP protocol and could point
     *  to state information for the protocol. */
    void *state;
    /** maximum transfer unit (in bytes) */
    u16_t mtu;
#if PPP_DEBUG
    u8_t num;
#endif
};

#define netif_set_link_up(netif) ppp_netif_set_link_up(netif)
#define netif_set_link_down(netif) ppp_netif_set_link_down(netif)

#define PPP_MEMPOOL_PROTOTYPE(name)
#define PPP_MEMPOOL_INIT(name)
#define PPP_MEMPOOL_ALLOC(name) 1
#define PPP_MEMPOOL_FREE(name, x)

#define PPP_MEMPOOL_DECLARE(name,num,size,desc) \
    uint32_t name = size;

#define PBUF_RAW  1
#define PBUF_POOL 2
#define PBUF_RAM  2

typedef enum {
    PPP_BUF_HEAP = 0,
    PPP_BUF_POOL
} ppp_buf_type_e;

#define MEMPOOL_ALLOC(size) \
        malloc(size)

#define MEMPOOL_FREE(x, ptr) \
        free(ptr)

#define pbuf_remove_header(buf, size) \
    ppp_memory_buffer_remove_header(buf, size)

#define pbuf_add_header(buf, size) \
    ppp_memory_buffer_add_header(buf, size)

#define LINK_STATS_INC(x)
#define MIB2_STATS_NETIF_INC(n, x)
#define MIB2_INIT_NETIF(netif, type, speed)
#define MIB2_STATS_NETIF_ADD(n, x, val)

#define PPP_ASSERT(message, assertion)

typedef int sys_prot_t;

#define SYS_ARCH_DECL_PROTECT(lev) sys_prot_t lev
#define SYS_ARCH_PROTECT(lev) lev = ppp_sys_arch_protect(ppp->netif->service_ptr)
#define SYS_ARCH_UNPROTECT(lev) ppp_sys_arch_unprotect(ppp->netif->service_ptr, lev)

#define PPPOS_DECL_PROTECT(lev) SYS_ARCH_DECL_PROTECT(lev)
#define PPPOS_PROTECT(lev)      SYS_ARCH_PROTECT(lev)
#define PPPOS_UNPROTECT(lev)    SYS_ARCH_UNPROTECT(lev)

#include <ctype.h>
#define ppp_isdigit(c)           isdigit((unsigned char)(c))

#define sys_now ppp_sys_now
#define sys_jiffies() ppp_sys_jiffies();

#define TIMEOUT(f, a, t)        do { ppp_sys_untimeout((f), (a)); ppp_sys_timeout(pcb->netif->service_ptr, (t)*1000, (f), (a)); } while(0)
#define TIMEOUTMS(f, a, t)      do { ppp_sys_untimeout((f), (a)); ppp_sys_timeout(pcb->netif->service_ptr, (t), (f), (a)); } while(0)
#define UNTIMEOUT(f, a)         do { ppp_sys_untimeout((f), (a)); } while(0)

#define sys_timeout(msecs, handler, arg) ppp_sys_timeout(pcb->netif->service_ptr, msecs, (ppp_sys_timeout_handler) handler, (void *) arg)
#define sys_untimeout(handler, arg) ppp_sys_untimeout(handler, (void *) arg)

#define OUTPUT_BUFFER           0
#define INPUT_BUFFER            1

#if defined(__arm__) && defined(__ARMCC_VERSION) && (__ARMCC_VERSION < 6010050)
/* Keil uVision4 tools */
#define PACK_STRUCT_BEGIN __packed
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(fld) fld
#define ALIGNED(n)  __align(n)
#elif defined (__IAR_SYSTEMS_ICC__)
/* IAR Embedded Workbench tools */
#define PACK_STRUCT_BEGIN __packed
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(fld) fld
#define IAR_STR(a) #a
#define ALIGNED(n) _Pragma(IAR_STR(data_alignment= ## n ##))
#else
/* GCC tools (CodeSourcery) */
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT __attribute__ ((__packed__))
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(fld) fld
#define ALIGNED(n)  __attribute__((aligned (n)))
#endif

/**
 * MEMCPY: override this if you have a faster implementation at hand than the
 * one included in your C library
 */
#if !defined MEMCPY
#define MEMCPY(dst,src,len)             memcpy(dst,src,len)
#endif

#define UNUSED_ARG(x) (void)x

#ifdef __cplusplus
extern "C" {
#endif

struct pbuf *ppp_memory_buffer_allocate(void *memory_manager, uint16_t size, ppp_buf_type_e type);
void ppp_memory_buffer_free(struct pbuf *buffer);
uint16_t ppp_memory_buffer_pool_alloc_unit_get(void *memory_manager);
void ppp_memory_buffer_cat(void *memory_manager, struct pbuf *to_buf, struct pbuf *cat_buf);
void ppp_memory_buffer_set_len(void *memory_manager, struct pbuf *buf, uint16_t len);
uint8_t ppp_memory_buffer_remove_header(struct pbuf *buffer, uint16_t header_len);
uint8_t ppp_memory_buffer_add_header(struct pbuf *buffer, uint16_t header_len);

struct pbuf *ppp_memory_buffer_convert_to(void *memory_manager, void *mem_buf);
void *ppp_memory_buffer_convert_from(struct pbuf *pbuf);

struct netif *ppp_netif_add(struct netif *netif, void *state, netif_init_fn init);
#define ppp_netif_remove(param)
err_t ppp_ip_input(struct pbuf *p, struct netif *inp);
void ppp_ip4_addr_set(nsapi_addr_t *addr, uint32_t *src);

void ppp_set_link_up(struct netif *netif);
void ppp_set_link_down(struct netif *netif);

typedef void ppp_service_cb(void *arg);
err_t ppp_call_callback(void *service_ptr, ppp_service_cb callback, void *arg);

sys_prot_t ppp_sys_arch_protect(void *service_ptr);
void ppp_sys_arch_unprotect(void *service_ptr, sys_prot_t p);

typedef void (*ppp_sys_timeout_handler)(void *arg);
void ppp_sys_timeout(void *service_ptr, u32_t msecs, ppp_sys_timeout_handler handler, void *arg);
void ppp_sys_untimeout(ppp_sys_timeout_handler handler, void *arg);

uint32_t ppp_sys_now(void);
uint32_t ppp_sys_jiffies(void);

void ppp_trace_to_ascii_hex_dump(int output, int len, char *data);
void ppp_trace_to_ascii_hex_dump_print(int output);

#ifdef __cplusplus
}
#endif

#endif /* PPP_SERVICE_IF_H */
