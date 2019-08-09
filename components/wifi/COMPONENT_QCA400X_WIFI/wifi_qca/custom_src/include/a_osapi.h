//------------------------------------------------------------------------------
// Copyright (c) Qualcomm Atheros, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to
// the limitations in the disclaimer below) provided that the following conditions are met:
//
// · Redistributions of source code must retain the above copyright notice, this list of conditions and the
//   following disclaimer.
// · Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//   following disclaimer in the documentation and/or other materials provided with the distribution.
// · Neither the name of nor the names of its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE. THIS SOFTWARE IS
// PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================
#ifndef _A_OSAPI_H_
#define _A_OSAPI_H_

/* PORT_NOTE: Include any System header files here to resolve any types used
 *  in A_CUSTOM_DRIVER_CONTEXT */
#include <string.h>

#include "a_config.h"
#include "a_types.h"
#include <athdefs.h>
#include <cust_netbuf.h>
#include "platform/mbed_wait_api.h"
#include "mbed_assert.h"

#define MEM_TYPE_ATHEROS_PERSIST_RX_PCB 0x2001

/* there exist within the common code a few places that make use of the
 * inline option.  This macro can be used to properly declare inline for the compiler
 * or it can be used to disable inlining be leaving the macro blank.
 */
#ifndef INLINE
#define INLINE inline
#endif
/* PREPACK and/or POSTPACK are used with compilers that require each data structure
 * that requires single byte alignment be declared individually. these macros typically
 * would be used in place of athstartpack.h and athendpack.h which would be used with
 * compilers that allow this feature to be invoked through a one-time pre-processor command.
 */

#if defined(__ICCARM__)
/* IAR compiler */
#   define PREPACK __packed
#   define POSTPACK
#   define FIELD_PACKED
#   define PREWEAK_CODE __weak
#   define POSTWEAK_CODE
#elif defined(__GNUC__)
/* GCC */
#   define PREPACK
#   define POSTPACK __attribute__ ((__packed__))
#   define FIELD_PACKED
#   define PREWEAK_CODE
#   define POSTWEAK_CODE __attribute__((weak))
#elif defined(__CC_ARM)
/* KEIL compiler v5 */
#   define PREPACK __packed
#   define POSTPACK
#   define FIELD_PACKED
#   define PREWEAK_CODE __weak
#   define POSTWEAK_CODE
#elif __ARMCC_VERSION
/* KEIL compiler v6 */
#   define PREPACK
#   define POSTPACK __attribute__ ((__packed__))
#   define FIELD_PACKED
#   define PREWEAK_CODE
#   define POSTWEAK_CODE __attribute__((weak))
#else
#   error "Unsupported toolchain"
#endif

#ifndef min
#define min(a, b) ((a) < (b)) ? (a) : (b)
#endif

#ifndef max
#define max(a, b) ((a) > (b)) ? (a) : (b)
#endif

/* unaligned little endian access */
#define A_LE_READ_2(p) ((uint16_t)((((uint8_t *)(p))[0]) | (((uint8_t *)(p))[1] << 8)))

#define A_LE_READ_4(p)                                                                             \
    ((uint32_t)((((uint8_t *)(p))[0]) | (((uint8_t *)(p))[1] << 8) | (((uint8_t *)(p))[2] << 16) | \
                (((uint8_t *)(p))[3] << 24)))

/*
 * Endianes macros - Used to achieve proper translation to/from wifi chipset endianess.
 * these macros should be directed to OS macros or functions that serve this purpose. the
 * naming convention is the direction of translation (BE2CPU == BigEndian to native CPU)
 * appended with the bit-size (BE2CPU16 == 16 bit operation BE2CPU32 == 32 bit operation
 */

#if !defined(A_LITTLE_ENDIAN) && !defined(A_BIG_ENDIAN)
#   error "Either A_BIG_ENDIAN or A_LITTLE_ENDIAN must be #defined"
#elif defined(A_LITTLE_ENDIAN) && defined(A_BIG_ENDIAN)
#   error "A_BIG_ENDIAN and A_LITTLE_ENDIAN cannot be #defined at the same time"
#elif defined(A_LITTLE_ENDIAN)
#   define A_BE2CPU16(x) ((uint16_t)(((x) & 0x00ff) << 8) | (uint16_t)(((x) >> 8) & 0x00ff))
#   define A_BE2CPU32(x) \
    ( (((x)& 0x000000ff) << 24) | (((x) & 0x0000ff00) << 8) | (((x) & 0x00ff0000) >> 8) | (((x) >> 24) & 0x000000ff) )
#   define A_LE2CPU16(x) (x)
#   define A_LE2CPU32(x) (x)
#   define A_CPU2BE16(x) ((uint16_t)(((x) & 0x00ff) << 8) | (uint16_t)(((x) >> 8) & 0x00ff))
#   define A_CPU2BE32(x) \
    ( (((x) & 0x000000ff) << 24) | (((x) & 0x0000ff00) << 8) | (((x) & 0x00ff0000) >> 8) | (((x) >> 24) & 0x000000ff) )
#   define A_CPU2LE32(x) (x)
#   define A_CPU2LE16(x) (x)
#elif defined(A_BIG_ENDIAN)
#   define A_BE2CPU16(x) (x)
#   define A_BE2CPU32(x) (x)
#   define A_LE2CPU16(x) ((uint16_t)(((x) & 0x00ff) << 8) | (uint16_t)(((x) >> 8) & 0x00ff))
#   define A_LE2CPU32(x) \
    ((((x) & 0x000000ff) << 24) | (((x)&0x0000ff00) << 8) | (((x)&0x00ff0000) >> 8) | (((x) >> 24) & 0x000000ff))
#   define A_CPU2BE16(x) (x)
#   define A_CPU2BE32(x) (x)
#   define A_CPU2LE32(x) \
    ((((x) & 0x000000ff) << 24) | (((x)&0x0000ff00) << 8) | (((x)&0x00ff0000) >> 8) | (((x) >> 24) & 0x000000ff))
#   define A_CPU2LE16(x) ((uint16_t)(((x) & 0x00ff) << 8) | (uint16_t)(((x) >> 8) & 0x00ff))
#endif

/* A_MEM -- macros that should be mapped to OS/STDLIB equivalent functions */
#define A_MEMCPY(dst, src, len) memcpy((dst), (src), (len))
#define A_MEMZERO(addr, len) memset((addr), 0, (len))
#define A_MEMCMP(addr1, addr2, len) memcmp((addr1), (addr2), (len))

extern uint32_t g_totAlloc;
extern uint32_t g_poolid;

#define DRIVER_STATE_INVALID 0
#define DRIVER_STATE_INIT 1
#define DRIVER_STATE_RUN 2
#define DRIVER_STATE_SHUTDOWN 3
extern uint8_t g_driverState;

inline uint32_t _get_size(void *addr)
{
    uint32_t *ptr = (uint32_t *)addr;
    ptr -= 3;

    if (g_poolid == 0xffffffff)
    {
        g_poolid = ptr[1];
    }

    if (ptr[1] == g_poolid)
    {
        return ptr[0];
    }
    else
    {
        QCADRV_PRINTF("STUCK\n");
        while (1)
        {
        };
    }
}

void *a_malloc(int32_t size, uint8_t id);
void a_free(void *addr, uint8_t id);

/* Definitions used for ID param in calls to A_MALLOC and A_FREE */
//		NAME					VALUE 	DESCRIPTION
#define MALLOC_ID_CONTEXT 1     // persistent allocation for the life of Driver
#define MALLOC_ID_NETBUFF 2     // buffer used to perform TX/RX SPI transaction
#define MALLOC_ID_NETBUFF_OBJ 3 // NETBUF object
#define MALLOC_ID_TEMPORARY 4   // other temporary allocation
/* Macros used for heap memory allocation and free */
#define A_MALLOCINIT()
#define A_MALLOC(size, id) a_malloc(size, id)
#define A_FREE(addr, id) a_free(addr, id)

#define A_PRINTF(args...) QCADRV_PRINTF(args)

/*
 * A_MDELAY - used to delay specified number of milliseconds.
 */
#define A_MDELAY(msecs) wait_us(msecs * 1000);

#if DRIVER_CONFIG_DISABLE_ASSERT
#define A_ASSERT(expr)
#else /* DRIVER_CONFIG_DISABLE_ASSERT */
extern void *p_Global_Cxt;
extern void qcadrv_assert_func(const char *, int);
//#define A_ASSERT(expr) if(!(expr)) do { qcadrv_assert_func(__FUNCTION__, __LINE__); } while(0)
#define A_ASSERT(expr) MBED_ASSERT(expr)
#endif /* DRIVER_CONFIG_DISABLE_ASSERT */

/* UNUSED_ARGUMENT - used to solve compiler warnings where arguments to functions are not used
 * within the function.
 */

#endif /* _OSAPI_MQX_H_ */
