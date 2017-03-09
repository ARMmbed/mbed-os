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
#ifndef __UVISOR_API_UNSUPPORTED_H__
#define __UVISOR_API_UNSUPPORTED_H__

#include "uvisor/api/inc/uvisor_exports.h"
#include <stddef.h>
#include <stdint.h>

/* uVisor hook for unsupported platforms */
UVISOR_EXTERN void uvisor_init(void);

/*******************************************************************************
 * Re-definitions from:
 ******************************************************************************/

/* uvisor-lib/box-config.h */

UVISOR_EXTERN const uint32_t __uvisor_mode;

#define UVISOR_DISABLED 0

#define UVISOR_SET_MODE(mode) \
    UVISOR_SET_MODE_ACL_COUNT(mode, NULL, 0)

#define UVISOR_SET_MODE_ACL(mode, acl_list) \
    UVISOR_SET_MODE_ACL_COUNT(mode, acl_list, UVISOR_ARRAY_COUNT(acl_list))

#define UVISOR_SET_MODE_ACL_COUNT(mode, acl_list, acl_list_count) \
    UVISOR_EXTERN const uint32_t __uvisor_mode = UVISOR_DISABLED; \
    static const void *public_box_acl = acl_list; \
    extern const __attribute__((section(".keep.uvisor.cfgtbl_ptr_first"), aligned(4))) void * const public_box_cfg_ptr = &public_box_acl;

#define __UVISOR_BOX_CONFIG_NOCONTEXT(box_name, acl_list, stack_size) \
    static const void *box_acl_ ## box_name = acl_list; \
    extern const __attribute__((section(".keep.uvisor.cfgtbl_ptr"), aligned(4))) void * const box_name ## _cfg_ptr = &box_acl_ ## box_name;

#define __UVISOR_BOX_CONFIG_CONTEXT(box_name, acl_list, stack_size, context_type) \
    context_type box_ctx_ ## box_name; \
    context_type * const uvisor_ctx = &box_ctx_ ## box_name; \
    static const void *box_acl_ ## box_name = acl_list; \
    const __attribute__((section(".keep.uvisor.cfgtbl_ptr"), aligned(4))) volatile void *box_name ## _cfg_ptr = \
        &box_acl_ ## box_name;

#define __UVISOR_BOX_MACRO(_1, _2, _3, _4, NAME, ...) NAME
#define UVISOR_BOX_CONFIG(...) \
    __UVISOR_BOX_MACRO(__VA_ARGS__, __UVISOR_BOX_CONFIG_CONTEXT, \
                                    __UVISOR_BOX_CONFIG_NOCONTEXT)(__VA_ARGS__)

#define UVISOR_BOX_CONFIG_ACL(...) UVISOR_BOX_CONFIG(__VA_ARGS__)

#define UVISOR_BOX_CONFIG_CTX(...) UVISOR_BOX_CONFIG(__VA_ARGS__)

#define UVISOR_BOX_NAMESPACE(...)

#define UVISOR_BOX_HEAPSIZE(...)

/* uvisor-lib/interrupts.h */

#define vIRQ_SetVector(irqn, vector)        NVIC_SetVector((IRQn_Type) (irqn), (uint32_t) (vector))
#define vIRQ_GetVector(irqn)                NVIC_GetVector((IRQn_Type) (irqn))
#define vIRQ_EnableIRQ(irqn)                NVIC_EnableIRQ((IRQn_Type) (irqn))
#define vIRQ_DisableIRQ(irqn)               NVIC_DisableIRQ((IRQn_Type) (irqn))
#define vIRQ_DisableAll                     __disable_irq
#define vIRQ_EnableAll                      __enable_irq
#define vIRQ_ClearPendingIRQ(irqn)          NVIC_ClearPendingIRQ((IRQn_Type) (irqn))
#define vIRQ_SetPendingIRQ(irqn)            NVIC_SetPendingIRQ((IRQn_Type) (irqn))
#define vIRQ_GetPendingIRQ(irqn)            NVIC_GetPendingIRQ((IRQn_Type) (irqn))
#define vIRQ_SetPriority(irqn, priority)    NVIC_SetPriority((IRQn_Type) (irqn), (uint32_t) (priority))
#define vIRQ_GetPriority(irqn)              NVIC_GetPriority((IRQn_Type) (irqn))

/* uvisor-lib/register_gateway.h */

#define UVISOR_OP_READ(op)  (op)
#define UVISOR_OP_WRITE(op) ((1 << 4) | (op))
#define UVISOR_OP_NOP       0x0
#define UVISOR_OP_AND       0x1
#define UVISOR_OP_OR        0x2
#define UVISOR_OP_XOR       0x3

/* Default mask for whole register operatins */
#define __UVISOR_OP_DEFAULT_MASK 0x0

static UVISOR_FORCEINLINE uint32_t uvisor_read(uint32_t addr, uint32_t op, uint32_t mask)
{
    switch(op)
    {
        case UVISOR_OP_READ(UVISOR_OP_NOP):
            return *((uint32_t *) addr);
        case UVISOR_OP_READ(UVISOR_OP_AND):
            return *((uint32_t *) addr) & mask;
        case UVISOR_OP_READ(UVISOR_OP_OR):
            return *((uint32_t *) addr) | mask;
        case UVISOR_OP_READ(UVISOR_OP_XOR):
            return *((uint32_t *) addr) ^ mask;
        default:
            /* FIXME */
            return 0;
    }
}

#define uvisor_read(addr) uvisor_read((uint32_t) (addr), UVISOR_OP_READ(UVISOR_OP_NOP), __UVISOR_OP_DEFAULT_MASK)

static UVISOR_FORCEINLINE void uvisor_write(uint32_t addr, uint32_t val, uint32_t op, uint32_t mask)
{
    switch(op)
    {
        case UVISOR_OP_WRITE(UVISOR_OP_NOP):
            *((uint32_t *) addr) = val;
        case UVISOR_OP_WRITE(UVISOR_OP_AND):
            *((uint32_t *) addr) &= val | ~mask;
        case UVISOR_OP_WRITE(UVISOR_OP_OR):
            *((uint32_t *) addr) |= val & mask;
        case UVISOR_OP_WRITE(UVISOR_OP_XOR):
            *((uint32_t *) addr) ^= val & mask;
        default:
            /* FIXME */
            return;
    }
}

#define uvisor_write(addr, val) uvisor_write((uint32_t) (addr), (uint32_t) (val), \
                                             UVISOR_OP_WRITE(UVISOR_OP_NOP), __UVISOR_OP_DEFAULT_MASK)

/* uvisor-lib/secure_access.h */

static UVISOR_FORCEINLINE void uvisor_write32(uint32_t volatile *addr, uint32_t val)
{
    *(addr) = val;
}

static UVISOR_FORCEINLINE void uvisor_write16(uint16_t volatile *addr, uint16_t val)
{
    *(addr) = val;
}

static UVISOR_FORCEINLINE void uvisor_write8(uint8_t volatile *addr, uint8_t val)
{
    *(addr) = val;
}

static UVISOR_FORCEINLINE uint32_t uvisor_read32(uint32_t volatile *addr)
{
    return *(addr);
}

static UVISOR_FORCEINLINE uint16_t uvisor_read16(uint16_t volatile *addr)
{
    return *(addr);
}

static UVISOR_FORCEINLINE uint8_t uvisor_read8(uint8_t volatile *addr)
{
    return *(addr);
}

/* The conditional statement will be optimised away since the compiler already
 * knows the sizeof(type). */
#define ADDRESS_READ(type, addr) \
    (sizeof(type) == 4 ? uvisor_read32((volatile uint32_t *) (addr)) : \
     sizeof(type) == 2 ? uvisor_read16((volatile uint16_t *) (addr)) : \
     sizeof(type) == 1 ? uvisor_read8((volatile uint8_t *) (addr)) : 0)

/* The switch statement will be optimised away since the compiler already knows
 * the sizeof_type. */
static UVISOR_FORCEINLINE void __address_write(size_t sizeof_type, volatile uint32_t *addr, uint32_t val)
{
    switch(sizeof_type) {
        case 4:
            uvisor_write32((volatile uint32_t *) addr, (uint32_t) val);
            break;
        case 2:
            uvisor_write16((volatile uint16_t *) addr, (uint16_t) val);
            break;
        case 1:
            uvisor_write8((volatile uint8_t *) addr, (uint8_t) val);
            break;
    }
}

#define ADDRESS_WRITE(type, addr, val) __address_write(sizeof(type), (volatile uint32_t *) addr, (uint32_t) val)

#define UNION_READ(type, addr, fieldU, fieldB) ((*((volatile type *) (addr))).fieldB)

/* uvisor-lib/secure_gateway.h */

#define secure_gateway(dst_box, dst_fn, ...) dst_fn(__VA_ARGS__)

#endif /* __UVISOR_API_UNSUPPORTED_H__ */
