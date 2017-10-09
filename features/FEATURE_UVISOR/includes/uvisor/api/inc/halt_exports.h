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
#ifndef __UVISOR_API_HALT_EXPORTS_H__
#define __UVISOR_API_HALT_EXPORTS_H__

#include "uvisor_exports.h"

#define UVISOR_ERROR_INVALID_BOX_ID             (-2)
#define UVISOR_ERROR_BUFFER_TOO_SMALL           (-3)
#define UVISOR_ERROR_BOX_NAMESPACE_ANONYMOUS    (-4)
#define UVISOR_ERROR_BAD_MAGIC                  (-5)
#define UVISOR_ERROR_BAD_VERSION                (-6)
#define UVISOR_ERROR_OUT_OF_STRUCTURES          (-7)
#define UVISOR_ERROR_INVALID_PARAMETERS         (-8)
#define UVISOR_ERROR_NOT_IMPLEMENTED            (-9)
#define UVISOR_ERROR_TIMEOUT                    (-10)


#define UVISOR_ERROR_CLASS_MASK     (0xFFFF0000UL)
#define UVISOR_ERROR_MASK           (0x0000FFFFUL)

#define UVISOR_ERROR_CLASS_PAGE     (1UL << 16)

typedef enum {
    USER_NOT_ALLOWED = 1,
    DEBUG_BOX_HALT,
} THaltUserError;

typedef enum {
    HALT_NO_ERROR = 0,
    PERMISSION_DENIED = 1,
    SANITY_CHECK_FAILED,
    NOT_IMPLEMENTED,
    NOT_ALLOWED,
    FAULT_MEMMANAGE,
    FAULT_BUS,
    FAULT_USAGE,
    FAULT_HARD,
    FAULT_DEBUG,
    FAULT_SECURE,
    __THALTERROR_MAX /* always keep as the last element of the enum */
} THaltError;

/** A basic exception frame
 *
 * This struct contains the registers always saved during an exception in the
 * order they are placed in the memory.
 * If FPU state is also saved it's placed after this register block.
 * On ARMv8-M in certain cases an additional state context may be placed on
 * the stack before this block.
 */
typedef struct {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t retaddr;
    uint32_t retpsr;
} UVISOR_PACKED exception_frame_t;

/* A pointer to this structure will be given to halt_error() handler
 * of the debug box driver. */
typedef struct {
    /* A basic exception stack frame that is always present with a valid stack. */
    exception_frame_t stack_frame;

    /* A few registers that may be useful for debug. */
    uint32_t lr;
    uint32_t control;
    uint32_t ipsr;

    /* Fault status registers. */
    uint32_t mmfar;
    uint32_t bfar;
    uint32_t cfsr;
    uint32_t hfsr;
    uint32_t dfsr;
    uint32_t afsr;

    /* Bitmask telling which of the above regions are valid. */
    uint32_t valid_data;
} UVISOR_PACKED THaltInfo;

/* Bitmask to specify which HaltInfo regions are valid. */
typedef enum {
    HALT_INFO_STACK_FRAME = 0x1,
    HALT_INFO_REGISTERS   = 0x2
} THaltInfoValidMask;

#endif /* __UVISOR_API_HALT_EXPORTS_H__ */
