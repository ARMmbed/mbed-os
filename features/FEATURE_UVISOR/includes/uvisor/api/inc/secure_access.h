/*
 * Copyright (c) 2015, ARM Limited, All Rights Reserved
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
#ifndef __UVISOR_API_SECURE_ACCESS_H__
#define __UVISOR_API_SECURE_ACCESS_H__

#include "api/inc/uvisor_exports.h"
#include "api/inc/vmpu_exports.h"
#include <stddef.h>
#include <stdint.h>

static UVISOR_FORCEINLINE void uvisor_write32(uint32_t volatile * volatile addr, uint32_t val)
{
    UVISOR_ASM_MEMORY_ACCESS(str, uint32_t, addr, val);
}

static UVISOR_FORCEINLINE void uvisor_write16(uint16_t volatile * volatile addr, uint16_t val)
{
    UVISOR_ASM_MEMORY_ACCESS(strh, uint16_t, addr, val);
}

static UVISOR_FORCEINLINE void uvisor_write8(uint8_t volatile * volatile addr, uint8_t val)
{
    UVISOR_ASM_MEMORY_ACCESS(strb, uint8_t, addr, val);
}

static UVISOR_FORCEINLINE uint32_t uvisor_read32(uint32_t volatile * volatile addr)
{
    return UVISOR_ASM_MEMORY_ACCESS(ldr, uint32_t, addr);
}

static UVISOR_FORCEINLINE uint16_t uvisor_read16(uint16_t volatile * volatile addr)
{
    return UVISOR_ASM_MEMORY_ACCESS(ldrh, uint16_t, addr);
}

static UVISOR_FORCEINLINE uint8_t uvisor_read8(uint8_t volatile * volatile addr)
{
    return UVISOR_ASM_MEMORY_ACCESS(ldrb, uint8_t, addr);
}

/* The switch statement will be optimised away since the compiler already knows
 * the sizeof_type. */
static UVISOR_FORCEINLINE void __address_write(size_t sizeof_type, volatile uint32_t *addr, uint32_t val)
{
    switch(sizeof_type) {
        case 4:
            uvisor_write32((volatile uint32_t * volatile) addr, (uint32_t) val);
            break;
        case 2:
            uvisor_write16((volatile uint16_t * volatile) addr, (uint16_t) val);
            break;
        case 1:
            uvisor_write8((volatile uint8_t * volatile) addr, (uint8_t) val);
            break;
        default:
            uvisor_error(USER_NOT_ALLOWED);
            break;
    }
}

#define ADDRESS_WRITE(type, addr, val) __address_write(sizeof(type), (volatile uint32_t *) addr, (uint32_t) val)

/* the conditional statement will be optimised away since the compiler already
 * knows the sizeof(type) */
#define ADDRESS_READ(type, addr) \
    (sizeof(type) == 4 ? uvisor_read32((uint32_t volatile * volatile) (addr)) : \
     sizeof(type) == 2 ? uvisor_read16((uint16_t volatile * volatile) (addr)) : \
     sizeof(type) == 1 ? uvisor_read8((uint8_t volatile * volatile) (addr)) : 0)

#define UNION_READ(type, addr, fieldU, fieldB) \
    ({ \
        type res; \
        res.fieldU = ADDRESS_READ(type, addr); \
        res.fieldB; \
    })

#endif /* __UVISOR_API_SECURE_ACCESS_H__ */
