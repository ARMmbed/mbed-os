/*
 * Copyright (c) 2015-2015, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __UVISOR_API_REGISTER_GATEWAY_H__
#define __UVISOR_API_REGISTER_GATEWAY_H__

#include "api/inc/register_gateway_exports.h"
#include "api/inc/uvisor_exports.h"
#include "api/inc/svc_exports.h"
#include <stdint.h>

/** Get the offset of a struct member.
 * @internal
 */
#define __UVISOR_OFFSETOF(type, member) ((uint32_t) (&(((type *)(0))->member)))

/** Generate the SVCall opcode from the SVC ID. */
#define UVISOR_SVC_OPCODE(id) ((uint16_t) (0xDF00 | ((id) & 0xFF)))

/** Generate the opcode of the 16-bit Thumb-2 16-bit T2 encoding of the branch
 *  instruction.
 * @internal
 * @note The branch instruction is encoded according to the Thumb-2 immediate
 * encoding rules:
 *     <instr_addr>: B.N <label>
 *     imm = (<label>_addr - PC) / 2
 * Where:
 *     PC = <instr>_addr + 4
 * The +4 is to account for the pipelined PC at the time of the branch
 * instruction. See ARM DDI 0403E.b page A4-102 for more details.
 * @param instr[in] Address of the branch instruction
 * @param label[in] Address of the label
 * @returns the 16-bit encoding of the B.N <label> instruction.
 */
#define BRANCH_OPCODE(instr, label) \
    (uint16_t) (0xE000 | (uint8_t) ((((uint32_t) (label) - ((uint32_t) (instr) + 4)) / 2) & 0xFF))

/** `BX LR` encoding
 * @internal
 */
#define BXLR                           ((uint16_t) 0x4770)

/** Register Gateway - Read operation
 *
 * This macro provides an API to perform 32-bit read operations on restricted
 * registers. Such accesses are assembled into a read-only flash structure that
 * is read and validated by uVisor before performing the operation.
 *
 * @warning This API currently only supports link-time known value for the
 * address, operation and mask.
 *
 * @param box_name[in]  The name of the source box as decalred in
 *                      `UVISOR_BOX_CONFIG`.
 * @param shared[in]    Whether the gateway can be shared with other boxes or
 *                      not. Two values are available: UVISOR_RGW_SHARED,
 *                      UVISOR_RGW_EXCLUSIVE.
 * @param addr[in]      The address for the data access.
 * @param operation[in] The operation to perform at the address for the read. It
 *                      is chosen among the `UVISOR_RGW_OP_*` macros.
 * @param mask[in]      The mask to apply for the read operation.
 * @returns The value read from address using the operation and mask provided
 * (or their respective defaults if they have not been provided).
 */
#define uvisor_read(box_name, shared, addr, op, msk) \
    ({ \
        /* Instanstiate the gateway. This gets resolved at link-time. */ \
        UVISOR_ALIGN(4) static TRegisterGateway const register_gateway = { \
            .svc_opcode = UVISOR_SVC_OPCODE(UVISOR_SVC_ID_REGISTER_GATEWAY), \
            .branch     = BRANCH_OPCODE(__UVISOR_OFFSETOF(TRegisterGateway, branch), \
                                        __UVISOR_OFFSETOF(TRegisterGateway, bxlr)), \
            .magic      = UVISOR_REGISTER_GATEWAY_MAGIC, \
            .box_ptr    = (uint32_t) & box_name ## _cfg_ptr, \
            .address    = (uint32_t) addr, \
            .mask       = msk, \
            .operation  = UVISOR_RGW_OP(op, sizeof(*addr), shared), \
            .bxlr       = BXLR  \
        }; \
        \
        /* Pointer to the register gateway we just created. The pointer is
         * located in a discoverable linker section. */ \
        __attribute__((section(".keep.uvisor.register_gateway_ptr"))) \
        static uint32_t const register_gateway_ptr = (uint32_t) &register_gateway; \
        (void) register_gateway_ptr; \
        \
        /* Call the actual gateway. */ \
        uint32_t result = ((uint32_t (*)(void)) ((uint32_t) &register_gateway | 1))(); \
        (typeof(*addr)) result; \
    })

/** Register Gateway - Write operation
 *
 * This macro provides an API to perform 32-bit write operations on restricted
 * registers. Such accesses are assembled into a read-only flash structure that
 * is read and validated by uVisor before performing the operation.
 *
 * @warning This API currently only supports link-time known value for the
 * address, operation and mask.
 *
 * @param box_name[in]  The name of the source box as decalred in
 *                      `UVISOR_BOX_CONFIG`.
 * @param shared[in]    Whether the gateway can be shared with other boxes or
 *                      not. Two values are available: UVISOR_RGW_SHARED,
 *                      UVISOR_RGW_EXCLUSIVE.
 * @param addr[in]      The address for the data access.
 * @param val[in]       The value to write at address.
 * @param operation[in] The operation to perform at the address for the read. It
 *                      is chosen among the `UVISOR_RGW_OP_*` macros.
 * @param mask[in]      The mask to apply for the write operation.
 */
#define uvisor_write(box_name, shared, addr, val, op, msk) \
    { \
        /* Instanstiate the gateway. This gets resolved at link-time. */ \
        UVISOR_ALIGN(4) static TRegisterGateway const register_gateway = { \
            .svc_opcode = UVISOR_SVC_OPCODE(UVISOR_SVC_ID_REGISTER_GATEWAY), \
            .branch     = BRANCH_OPCODE(__UVISOR_OFFSETOF(TRegisterGateway, branch), \
                                        __UVISOR_OFFSETOF(TRegisterGateway, bxlr)), \
            .magic      = UVISOR_REGISTER_GATEWAY_MAGIC, \
            .box_ptr    = (uint32_t) & box_name ## _cfg_ptr, \
            .address    = (uint32_t) addr, \
            .mask       = msk, \
            .operation  = UVISOR_RGW_OP(op, sizeof(*addr), shared), \
            .bxlr       = BXLR  \
        }; \
        \
        /* Pointer to the register gateway we just created. The pointer is
         * located in a discoverable linker section. */ \
        __attribute__((section(".keep.uvisor.register_gateway_ptr"))) \
        static uint32_t const register_gateway_ptr = (uint32_t) &register_gateway; \
        (void) register_gateway_ptr; \
        \
        /* Call the actual gateway.
         * The value is passed as the first argument. */ \
        ((void (*)(uint32_t)) ((uint32_t) ((uint32_t) &register_gateway | 1)))((uint32_t) (val)); \
    }

/** Get the selected bits at the target address.
 * @param box_name[in] Box name as defined by the uVisor box configuration
 *                     macro `UVISOR_BOX_CONFIG`
 * @param shared[in]   Whether the gateway can be shared with other boxes or
 *                     not. Two values are available: UVISOR_RGW_SHARED,
 *                     UVISOR_RGW_EXCLUSIVE.
 * @param address[in]  Target address
 * @param mask[in]     Bits to select out of the target address
 * @returns The value `*address & mask`.
 */
#define UVISOR_BITS_GET(box_name, shared, address, mask) \
    /* Register gateway implementation:
     * *address & mask */ \
    uvisor_read(box_name, shared, address, UVISOR_RGW_OP_READ_AND, mask)

/** Check the selected bits at the target address.
 * @param box_name[in] Box name as defined by the uVisor box configuration
 *                     macro `UVISOR_BOX_CONFIG`
 * @param shared[in]   Whether the gateway can be shared with other boxes or
 *                     not. Two values are available: UVISOR_RGW_SHARED,
 *                     UVISOR_RGW_EXCLUSIVE.
 * @param address[in]  Address at which to check the bits
 * @param mask[in]     Bits to select out of the target address
 * @returns The value `((*address & mask) == mask)`.
 */
#define UVISOR_BITS_CHECK(box_name, shared, address, mask) \
    ((UVISOR_BITS_GET(box_name, shared, address, mask)) == (mask))

/** Set the selected bits to 1 at the target address.
 *
 * Equivalent to: `*address |= mask`.
 * @param box_name[in] Box name as defined by the uVisor box configuration
 *                     macro `UVISOR_BOX_CONFIG`
 * @param shared[in]   Whether the gateway can be shared with other boxes or
 *                     not. Two values are available: UVISOR_RGW_SHARED,
 *                     UVISOR_RGW_EXCLUSIVE.
 * @param address[in]  Target address
 * @param mask[in]     Bits to select out of the target address
 */
#define UVISOR_BITS_SET(box_name, shared, address, mask) \
    /* Register gateway implementation:
     * *address |= (mask & mask) */ \
    uvisor_write(box_name, shared, address, mask, UVISOR_RGW_OP_WRITE_OR, mask)

/** Clear the selected bits at the target address.
 *
 * Equivalent to: `*address &= ~mask`.
 * @param box_name[in] Box name as defined by the uVisor box configuration
 *                     macro `UVISOR_BOX_CONFIG`
 * @param shared[in]   Whether the gateway can be shared with other boxes or
 *                     not. Two values are available: UVISOR_RGW_SHARED,
 *                     UVISOR_RGW_EXCLUSIVE.
 * @param address[in]  Target address
 * @param mask[in]     Bits to select out of the target address
 */
#define UVISOR_BITS_CLEAR(box_name, shared, address, mask) \
    /* Register gateway implementation:
     * *address &= (0x00000000 | ~mask) */ \
    uvisor_write(box_name, shared, address, 0x00000000, UVISOR_RGW_OP_WRITE_AND, mask)

/** Set the selected bits at the target address to the given value.
 *
 * Equivalent to: `*address = (*address & ~mask) | (value & mask)`.
 * @param box_name[in] Box name as defined by the uVisor box configuration
 *                     macro `UVISOR_BOX_CONFIG`
 * @param shared[in]   Whether the gateway can be shared with other boxes or
 *                     not. Two values are available: UVISOR_RGW_SHARED,
 *                     UVISOR_RGW_EXCLUSIVE.
 * @param address[in]  Target address
 * @param mask[in]     Bits to select out of the target address
 * @param value[in]    Value to write at the address location. Note: The value
 *                     must be already shifted to the correct bit position
 */
#define UVISOR_BITS_SET_VALUE(box_name, shared, address, mask, value) \
    /* Register gateway implementation:
     * *address = (*address & ~mask) | (value & mask) */ \
    uvisor_write(box_name, shared, address, value, UVISOR_RGW_OP_WRITE_REPLACE, mask)

/** Toggle the selected bits at the target address.
 *
 * Equivalent to: `*address ^= mask`.
 * @param box_name[in] Box name as defined by the uVisor box configuration
 *                     macro `UVISOR_BOX_CONFIG`
 * @param shared[in]   Whether the gateway can be shared with other boxes or
 *                     not. Two values are available: UVISOR_RGW_SHARED,
 *                     UVISOR_RGW_EXCLUSIVE.
 * @param address[in]  Target address
 * @param mask[in]     Bits to select out of the target address
 */
#define UVISOR_BITS_TOGGLE(box_name, shared, address, mask) \
    /* Register gateway implementation:
     * *address ^= (0xFFFFFFFF & mask) */ \
    uvisor_write(box_name, shared, address, 0xFFFFFFFF, UVISOR_RGW_OP_WRITE_XOR, mask)

#endif /* __UVISOR_API_REGISTER_GATEWAY_H__ */
