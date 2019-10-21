/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifndef __CC_H__
#define __CC_H__

#include <stdint.h>
#include <stddef.h> /* for size_t */
#include "mbed_toolchain.h"
#include "lwipopts.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ARM/LPC17xx is little endian only */
#if !defined(BYTE_ORDER) || (BYTE_ORDER != LITTLE_ENDIAN && BYTE_ORDER != BIG_ENDIAN)
#ifdef BYTE_ORDER
#undef BYTE_ORDER
#endif
#define BYTE_ORDER LITTLE_ENDIAN
#endif

#ifndef LWIP_PROVIDE_ERRNO
/* Use LWIP error codes */
#define LWIP_PROVIDE_ERRNO
#endif

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

/* Provide Thumb-2 routines for GCC to improve performance */
#if defined(TOOLCHAIN_GCC) && defined(__thumb2__)
    #define MEMCPY(dst,src,len)     thumb2_memcpy(dst,src,len)
    #define LWIP_CHKSUM             thumb2_checksum
    /* Set algorithm to 0 so that unused lwip_standard_chksum function
       doesn't generate compiler warning */
    #define LWIP_CHKSUM_ALGORITHM   0

    void* thumb2_memcpy(void* pDest, const void* pSource, size_t length);
    uint16_t thumb2_checksum(const void* pData, int length);
#else
    /* Used with IP headers only */
    #define LWIP_CHKSUM_ALGORITHM   1
#endif


#ifdef LWIP_DEBUG

#if MBED_CONF_LWIP_USE_MBED_TRACE
void lwip_mbed_tracef_debug(const char *fmt, ...);
void lwip_mbed_tracef_error(const char *fmt, ...);
void lwip_mbed_tracef_warn(const char *fmt, ...);
MBED_NORETURN void lwip_mbed_assert_fail(const char *msg, const char *func, const char *file, unsigned int line);

#define LWIP_PLATFORM_DIAG(vars)         lwip_mbed_tracef_debug vars
#define LWIP_PLATFORM_DIAG_SEVERE(vars)  lwip_mbed_tracef_error vars
#define LWIP_PLATFORM_DIAG_SERIOUS(vars) lwip_mbed_tracef_error vars
#define LWIP_PLATFORM_DIAG_WARNING(vars) lwip_mbed_tracef_warn vars

#define LWIP_PLATFORM_ASSERT(message) lwip_mbed_assert_fail(message, __func__, __FILE__, __LINE__)

#else // MBED_CONF_LWIP_USE_MBED_TRACE
#include <stdio.h>

MBED_NORETURN void assert_printf(const char *msg, int line, const char *file);

/* Plaform specific diagnostic output */
#define LWIP_PLATFORM_DIAG(vars) printf vars
#define LWIP_PLATFORM_ASSERT(flag) { assert_printf((flag), __LINE__, __FILE__); }
#endif // MBED_CONF_LWIP_USE_MBED_TRACE
#endif

#if TRACE_TO_ASCII_HEX_DUMP
#define TRACE_TO_ASCII_HEX_DUMPF(prefix, len, data) trace_to_ascii_hex_dump(prefix, len, data)
void trace_to_ascii_hex_dump(char* prefix, int len, char *data);
#else
#define TRACE_TO_ASCII_HEX_DUMPF(prefix, len, data) ((void)0)
#endif

#include "cmsis.h"
#define LWIP_PLATFORM_HTONS(x)      __REV16(x)
#define LWIP_PLATFORM_HTONL(x)      __REV(x)

/* Define the memory area for the lwip's memory pools */
#ifndef MEMP_SECTION
#if defined(TARGET_LPC4088) || defined(TARGET_LPC4088_DM)
#  if defined (__ICCARM__)
#     define MEMP_SECTION
#  elif defined(TOOLCHAIN_GCC_CR)
#     define MEMP_SECTION __attribute__((section(".data.$RamPeriph32")))
#  else
#     define MEMP_SECTION __attribute__((section("AHBSRAM0"),aligned))
#  endif
#elif defined(TARGET_LPC1768)
#  if defined (__ICCARM__)
#     define MEMP_SECTION
#  elif defined(TOOLCHAIN_GCC_CR)
#     define MEMP_SECTION __attribute__((section(".data.$RamPeriph32")))
#  else
#     define MEMP_SECTION __attribute__((section("AHBSRAM1"),aligned))
#  endif
#endif
#endif

#ifdef MEMP_SECTION
#define SET_MEMP_SECTION(name) extern uint8_t MEMP_SECTION name[]

#if defined (__ICCARM__)
#pragma default_variable_attributes = @ ".ethusbram"
#endif
SET_MEMP_SECTION(memp_memory_REASSDATA_base);
SET_MEMP_SECTION(memp_memory_TCP_PCB_LISTEN_base);
SET_MEMP_SECTION(memp_memory_PBUF_POOL_base);
SET_MEMP_SECTION(memp_memory_NETCONN_base);
SET_MEMP_SECTION(memp_memory_IGMP_GROUP_base);
SET_MEMP_SECTION(memp_memory_UDP_PCB_base);
SET_MEMP_SECTION(memp_memory_TCP_PCB_base);
SET_MEMP_SECTION(memp_memory_FRAG_PBUF_base);
SET_MEMP_SECTION(memp_memory_PBUF_base);
SET_MEMP_SECTION(memp_memory_MLD6_GROUP_base);
SET_MEMP_SECTION(memp_memory_IP6_REASSDATA_base);
SET_MEMP_SECTION(memp_memory_NETBUF_base);
SET_MEMP_SECTION(memp_memory_TCPIP_MSG_INPKT_base);
SET_MEMP_SECTION(memp_memory_SYS_TIMEOUT_base);
SET_MEMP_SECTION(memp_memory_TCP_SEG_base);
SET_MEMP_SECTION(memp_memory_TCPIP_MSG_API_base);
#if defined (__ICCARM__)
#pragma default_variable_attributes =
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* __CC_H__ */
