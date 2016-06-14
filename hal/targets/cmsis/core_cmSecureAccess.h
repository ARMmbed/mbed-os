/**************************************************************************//**
 * @file     core_cmSecureAccess.h
 * @brief    CMSIS Cortex-M Core Secure Access Header File
 * @version  XXX
 * @date     10. June 2016
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2016 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/


#ifndef __CORE_CM_SECURE_ACCESS_H
#define __CORE_CM_SECURE_ACCESS_H


/* ###########################  Core Secure Access  ########################### */

#ifdef FEATURE_UVISOR
#include "uvisor-lib.h"

/* Secure uVisor implementation. */

/** Set the value at the target address.
 *
 * Equivalent to: `*address = value`.
 * @param address[in]  Target address
 * @param value[in]    Value to write at the address location.
 */
#define SECURE_WRITE(address, value) \
    uvisor_write(main, UVISOR_RGW_SHARED, address, value, UVISOR_RGW_OP_WRITE, 0xFFFFFFFFUL)

/** Get the value at the target address.
 *
 * @param address[in]  Target address
 * @returns The value `*address`.
 */
#define SECURE_READ(address) \
    uvisor_read(main, UVISOR_RGW_SHARED, address, UVISOR_RGW_OP_READ, 0xFFFFFFFFUL)

/** Get the selected bits at the target address.
 *
 * @param address[in]  Target address
 * @param mask[in]     Bits to select out of the target address
 * @returns The value `*address & mask`.
 */
#define SECURE_BITS_GET(address, mask) \
    UVISOR_BITS_GET(main, UVISOR_RGW_SHARED, address, mask)

/** Check the selected bits at the target address.
 *
 * @param address[in]  Address at which to check the bits
 * @param mask[in]     Bits to select out of the target address
 * @returns The value `((*address & mask) == mask)`.
 */
#define SECURE_BITS_CHECK(address, mask) \
    UVISOR_BITS_CHECK(main, UVISOR_RGW_SHARED, address, mask)

/** Set the selected bits to 1 at the target address.
 *
 * Equivalent to: `*address |= mask`.
 * @param address[in]  Target address
 * @param mask[in]     Bits to select out of the target address
 */
#define SECURE_BITS_SET(address, mask) \
    UVISOR_BITS_SET(main, UVISOR_RGW_SHARED, address, mask)

/** Clear the selected bits at the target address.
 *
 * Equivalent to: `*address &= ~mask`.
 * @param address[in]  Target address
 * @param mask[in]     Bits to select out of the target address
 */
#define SECURE_BITS_CLEAR(address, mask) \
    UVISOR_BITS_CLEAR(main, UVISOR_RGW_SHARED, address, mask)

/** Set the selected bits at the target address to the given value.
 *
 * Equivalent to: `*address = (*address & ~mask) | (value & mask)`.
 * @param address[in]  Target address
 * @param mask[in]     Bits to select out of the target address
 * @param value[in]    Value to write at the address location. Note: The value
 *                     must be already shifted to the correct bit position
 */
#define SECURE_BITS_SET_VALUE(address, mask, value) \
    UVISOR_BITS_SET_VALUE(main, UVISOR_RGW_SHARED, address, mask, value)

/** Toggle the selected bits at the target address.
 *
 * Equivalent to: `*address ^= mask`.
 * @param address[in]  Target address
 * @param mask[in]     Bits to select out of the target address
 */
#define SECURE_BITS_TOGGLE(address, mask) \
    UVISOR_BITS_TOGGLE(main, UVISOR_RGW_SHARED, address, mask)

#else

/* Insecure fallback implementation. */

/** Set the value at the target address.
 *
 * Equivalent to: `*address = value`.
 * @param address[in]  Target address
 * @param value[in]    Value to write at the address location.
 */
#define SECURE_WRITE(address, value) \
    *(address) = (value)

/** Get the value at the target address.
 *
 * @param address[in]  Target address
 * @returns The value `*address`.
 */
#define SECURE_READ(address) \
    (*(address))

/** Get the selected bits at the target address.
 *
 * @param address[in]  Target address
 * @param mask[in]     Bits to select out of the target address
 * @returns The value `*address & mask`.
 */
#define SECURE_BITS_GET(address, mask) \
    (*(address) & (mask))

/** Check the selected bits at the target address.
 *
 * @param address[in]  Address at which to check the bits
 * @param mask[in]     Bits to select out of the target address
 * @returns The value `((*address & mask) == mask)`.
 */
#define SECURE_BITS_CHECK(address, mask) \
    ((*(address) & (mask)) == (mask))

/** Set the selected bits to 1 at the target address.
 *
 * Equivalent to: `*address |= mask`.
 * @param address[in]  Target address
 * @param mask[in]     Bits to select out of the target address
 */
#define SECURE_BITS_SET(address, mask) \
    *(address) |= (mask)

/** Clear the selected bits at the target address.
 *
 * Equivalent to: `*address &= ~mask`.
 * @param address[in]  Target address
 * @param mask[in]     Bits to select out of the target address
 */
#define SECURE_BITS_CLEAR(address, mask) \
    *(address) &= ~(mask)

/** Set the selected bits at the target address to the given value.
 *
 * Equivalent to: `*address = (*address & ~mask) | (value & mask)`.
 * @param address[in]  Target address
 * @param mask[in]     Bits to select out of the target address
 * @param value[in]    Value to write at the address location. Note: The value
 *                     must be already shifted to the correct bit position
 */
#define SECURE_BITS_SET_VALUE(address, mask, value) \
    *(address) = (*(address) & ~(mask)) | ((value) & (mask))

/** Toggle the selected bits at the target address.
 *
 * Equivalent to: `*address ^= mask`.
 * @param address[in]  Target address
 * @param mask[in]     Bits to select out of the target address
 */
#define SECURE_BITS_TOGGLE(address, mask) \
    *(address) ^= (mask)

#endif

#endif /* __CORE_CM_SECURE_ACCESS_H */
