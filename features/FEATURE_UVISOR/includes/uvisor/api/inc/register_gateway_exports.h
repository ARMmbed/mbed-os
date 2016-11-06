/*
 * Copyright (c) 2015-2016, ARM Limited, All Rights Reserved
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
#ifndef __UVISOR_API_REGISTER_GATEWAY_EXPORTS_H__
#define __UVISOR_API_REGISTER_GATEWAY_EXPORTS_H__

/** Register gateway magic
 *
 * The following magic is used to verify a register gateway structure. It has
 * been generated starting from the ARM Thumb/Thumb-2 invalid opcode.
 * ARM Thumb (with the Thumb-2 extension): 0xF7FxAxxx (32 bits)
 * The 'x's can be freely chosen (they have been chosen randomly here). This
 * requires the Thumb-2 extensions because otherwise only 16 bits opcodes are
 * accepted.
 */
#if defined(__thumb__) && defined(__thumb2__)
#define UVISOR_REGISTER_GATEWAY_MAGIC 0xF7F3A89E
#else
#error "Unsupported instruction set. The ARM Thumb-2 instruction set must be supported."
#endif /* __thumb__ && __thumb2__ */

/** Register gateway structure
 *
 * This struct is packed because we must ensure that the `svc_opcode` field has
 * no padding before itself, and that the `bxlr` field is at a pre-determined
 * offset from the `branch` field.
 */
typedef struct {
    uint16_t svc_opcode;
    uint16_t branch;
    uint32_t magic;
    uint32_t box_ptr;
    uint32_t address;
    uint32_t mask;
    uint16_t operation;
    uint16_t bxlr;
} UVISOR_PACKED UVISOR_ALIGN(4) TRegisterGateway;

/** Register gateway operation - Masks
 * @internal
 * These are used to extract the operation fields.
 */
#define __UVISOR_RGW_OP_TYPE_MASK   ((uint16_t) 0x00FF)
#define __UVISOR_RGW_OP_TYPE_POS    0
#define __UVISOR_RGW_OP_WIDTH_MASK  ((uint16_t) 0x3F00)
#define __UVISOR_RGW_OP_WIDTH_POS   8
#define __UVISOR_RGW_OP_SHARED_MASK ((uint16_t) 0x8000)
#define __UVISOR_RGW_OP_SHARED_POS  15

/** Register gateway operations
 * The user can specify the following properties:
 *   - Access type: read/write, and-, or-, xor-, replac-ed.
 *   - Access width: 8, 16, 32 bits.
 *   - Access shared: Whether the gateway can be shared with other boxes.
 * These parameters are stored in a 16-bit value as follows:
 *
 *   15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
 *   |  |__________________|  |____________________|
 *   |           |                       |
 *   shared      width                   type
 *
 * @note The operation value must be hardcoded.
 */
#define UVISOR_RGW_OP(type, width, shared) \
    ((uint16_t) ((((uint16_t) (type) << __UVISOR_RGW_OP_TYPE_POS) & __UVISOR_RGW_OP_TYPE_MASK) | \
                 (((uint16_t) (width) << __UVISOR_RGW_OP_WIDTH_POS) & __UVISOR_RGW_OP_WIDTH_MASK) | \
                 (((uint16_t) (shared) << __UVISOR_RGW_OP_SHARED_POS) & __UVISOR_RGW_OP_SHARED_MASK)))

/** Register gateway operation - Shared */
#define UVISOR_RGW_SHARED    1
#define UVISOR_RGW_EXCLUSIVE 0

/** Register gateway operation - Type */
#define UVISOR_RGW_OP_READ          0 /**< value = *address */
#define UVISOR_RGW_OP_READ_AND      1 /**< value = *address & mask */
#define UVISOR_RGW_OP_WRITE         2 /**< *address = value */
#define UVISOR_RGW_OP_WRITE_AND     3 /**< *address &= value | mask */
#define UVISOR_RGW_OP_WRITE_OR      4 /**< *address |= value & ~mask */
#define UVISOR_RGW_OP_WRITE_XOR     5 /**< *address ^= value & mask */
#define UVISOR_RGW_OP_WRITE_REPLACE 6 /**< *address = (*address & ~mask) | (value & mask) */

#endif /* __UVISOR_API_REGISTER_GATEWAY_EXPORTS_H__ */
