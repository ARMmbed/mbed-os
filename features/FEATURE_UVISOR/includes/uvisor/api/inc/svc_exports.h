/*
 * Copyright (c) 2013-2016, ARM Limited, All Rights Reserved
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
#ifndef __UVISOR_API_SVC_EXPORTS_H__
#define __UVISOR_API_SVC_EXPORTS_H__

#include "api/inc/uvisor_exports.h"
#include <stdint.h>

/* An SVCall takes a 8bit immediate, which is used as follows:
 *
 * For fast APIs:
 *
 *   7   6   5   4   3   2   1   0
 * .---.---.---.---.---.---.---.---.
 * | 1 | N | N | N | h | h | h | h |
 * '---'---'---'---'---'---'---'---'
 *   |   |_______|   |___________|
 *   |       |         |
 *   |       |         |
 *   |       |         |
 *   |       |        (h) Handler index in the hardcoded table (max 16)
 *   |       |
 *   |      (N) Number of arguments to pass to the handler (max 4)
 *   |
 *   SVCall mode: Fast
 *
 * For slow APIs:
 *
 *   7   6   5   4   3   2   1   0
 * .---.---.---.---.---.---.---.---.
 * | 0 | c | c | c | c | c | c | c |
 * '---'---'---'---'---'---'---'---'
 *   |   |_______________________|
 *   |               |
 *   |               |
 *   |              (c) Handler index in the custom table (max 128)
 *   |
 *   SVCall mode: Slow
 */

/* 1 bit of the SVCall imm8 field is used to determine the mode, fast/slow. */
#define UVISOR_SVC_MODE_FAST  1
#define UVISOR_SVC_MODE_SLOW  0
#define UVISOR_SVC_MODE_BIT   7
#define UVISOR_SVC_MODE_MASK  ((uint8_t) (1 << UVISOR_SVC_MODE_BIT))
#define UVISOR_SVC_MODE(mode) ((uint8_t) (((mode) << UVISOR_SVC_MODE_BIT) & UVISOR_SVC_MODE_MASK))

/* 7 or 4 bits of the SVCall imm8 field are used to determine the table index,
 * depending on the mode, fast/slow. */
#define UVISOR_SVC_FAST_INDEX_MAX  (1 << 4)
#define UVISOR_SVC_SLOW_INDEX_MAX  (1 << 7)
#define UVISOR_SVC_FAST_INDEX_BIT  0
#define UVISOR_SVC_FAST_INDEX_MASK ((uint8_t) (0xF << UVISOR_SVC_FAST_INDEX_BIT))
#define UVISOR_SVC_SLOW_INDEX_BIT  0
#define UVISOR_SVC_SLOW_INDEX_MASK ((uint8_t) (0x7F << UVISOR_SVC_SLOW_INDEX_BIT))
#define UVISOR_SVC_FAST_INDEX(index) ((uint8_t) (((index) << UVISOR_SVC_FAST_INDEX_BIT) & UVISOR_SVC_FAST_INDEX_MASK))
#define UVISOR_SVC_SLOW_INDEX(index) ((uint8_t) (((index) << UVISOR_SVC_SLOW_INDEX_BIT) & UVISOR_SVC_SLOW_INDEX_MASK))

/* When the SVC mode is "fast", the imm8 field also contains a specification of
 * the call interface (number of arguments).
 * This is needed for context switches, since the stack manipulation routines
 * need to know how many arguments to copy from source to destination. */
#define UVISOR_SVC_FAST_NARGS_BIT         4
#define UVISOR_SVC_FAST_NARGS_MASK        ((uint8_t) (0x7 << UVISOR_SVC_FAST_NARGS_BIT))
#define UVISOR_SVC_FAST_NARGS_SET(nargs)  ((uint8_t) (((nargs) << UVISOR_SVC_FAST_NARGS_BIT) & UVISOR_SVC_FAST_NARGS_MASK))
#define UVISOR_SVC_FAST_NARGS_GET(svc_id) (((uint8_t) (svc_id) & UVISOR_SVC_FAST_NARGS_MASK) >> UVISOR_SVC_FAST_NARGS_BIT)

/* Macros to build the SVCall imm8 field.
 * For slow APIs only the SVC handler index is needed.
 * For fast APIs the SVC handler index and the number of arguments are needed. */
#define UVISOR_SVC_CUSTOM_TABLE(index) ((uint8_t) (UVISOR_SVC_MODE(UVISOR_SVC_MODE_SLOW) | \
                                                   UVISOR_SVC_SLOW_INDEX(index)))
#define UVISOR_SVC_FIXED_TABLE(index, nargs) ((uint8_t) (UVISOR_SVC_MODE(UVISOR_SVC_MODE_FAST) | \
                                                         UVISOR_SVC_FAST_INDEX(index) | \
                                                         UVISOR_SVC_FAST_NARGS_SET(nargs)))

/* SVC immediate values for custom table */
#define UVISOR_SVC_ID_ISR_SET               UVISOR_SVC_CUSTOM_TABLE(1)
#define UVISOR_SVC_ID_ISR_GET               UVISOR_SVC_CUSTOM_TABLE(2)
#define UVISOR_SVC_ID_IRQ_ENABLE            UVISOR_SVC_CUSTOM_TABLE(3)
#define UVISOR_SVC_ID_IRQ_DISABLE           UVISOR_SVC_CUSTOM_TABLE(4)
#define UVISOR_SVC_ID_IRQ_PEND_CLR          UVISOR_SVC_CUSTOM_TABLE(5)
#define UVISOR_SVC_ID_IRQ_PEND_SET          UVISOR_SVC_CUSTOM_TABLE(6)
#define UVISOR_SVC_ID_IRQ_PEND_GET          UVISOR_SVC_CUSTOM_TABLE(7)
#define UVISOR_SVC_ID_IRQ_PRIO_SET          UVISOR_SVC_CUSTOM_TABLE(8)
#define UVISOR_SVC_ID_IRQ_PRIO_GET          UVISOR_SVC_CUSTOM_TABLE(9)
#define UVISOR_SVC_ID_BENCHMARK_CFG         UVISOR_SVC_CUSTOM_TABLE(10)
#define UVISOR_SVC_ID_BENCHMARK_RST         UVISOR_SVC_CUSTOM_TABLE(11)
#define UVISOR_SVC_ID_BENCHMARK_STOP        UVISOR_SVC_CUSTOM_TABLE(12)
#define UVISOR_SVC_ID_HALT_USER_ERR         UVISOR_SVC_CUSTOM_TABLE(13)
#define UVISOR_SVC_ID_IRQ_LEVEL_GET         UVISOR_SVC_CUSTOM_TABLE(14)
#define UVISOR_SVC_ID_BOX_ID_SELF           UVISOR_SVC_CUSTOM_TABLE(15)
#define UVISOR_SVC_ID_BOX_ID_CALLER         UVISOR_SVC_CUSTOM_TABLE(16)
#define UVISOR_SVC_ID_BOX_NAMESPACE_FROM_ID UVISOR_SVC_CUSTOM_TABLE(17)
#define UVISOR_SVC_ID_DEBUG_REBOOT          UVISOR_SVC_CUSTOM_TABLE(18)
#define UVISOR_SVC_ID_DEBUG_REGISTER_BOX    UVISOR_SVC_CUSTOM_TABLE(19)
#define UVISOR_SVC_ID_IRQ_DISABLE_ALL       UVISOR_SVC_CUSTOM_TABLE(20)
#define UVISOR_SVC_ID_IRQ_ENABLE_ALL        UVISOR_SVC_CUSTOM_TABLE(21)
#define UVISOR_SVC_ID_PAGE_MALLOC           UVISOR_SVC_CUSTOM_TABLE(22)
#define UVISOR_SVC_ID_PAGE_FREE             UVISOR_SVC_CUSTOM_TABLE(23)

/* SVC immediate values for hardcoded table (call from unprivileged) */
#define UVISOR_SVC_ID_UNVIC_OUT           UVISOR_SVC_FIXED_TABLE(0, 0)
/* Deprecated: UVISOR_SVC_ID_CX_IN(nargs) UVISOR_SVC_FIXED_TABLE(1, nargs) */
/* Deprecated: UVISOR_SVC_ID_CX_OUT       UVISOR_SVC_FIXED_TABLE(2, 0) */
#define UVISOR_SVC_ID_REGISTER_GATEWAY    UVISOR_SVC_FIXED_TABLE(3, 0)
#define UVISOR_SVC_ID_BOX_INIT_FIRST      UVISOR_SVC_FIXED_TABLE(4, 0)
#define UVISOR_SVC_ID_BOX_INIT_NEXT       UVISOR_SVC_FIXED_TABLE(5, 0)

/* SVC immediate values for hardcoded table (call from privileged) */
#define UVISOR_SVC_ID_UNVIC_IN         UVISOR_SVC_FIXED_TABLE(0, 0)

/** Generate the SVCall opcode from the SVC ID. */
#define UVISOR_SVC_OPCODE(id) ((uint16_t) 0xDF00 | (uint8_t) ((id) & 0xFF))

/* macro to execute an SVCall; additional metadata can be provided, which will
 * be appended right after the svc instruction */
/* note: the macro is implicitly overloaded to allow 0 to 4 32bits arguments */
#if defined(__CC_ARM)

#elif defined(__GNUC__)

#define UVISOR_SVC(id, metadata, ...) \
    ({ \
        UVISOR_MACRO_REGS_ARGS(uint32_t, ##__VA_ARGS__); \
        UVISOR_MACRO_REGS_RETVAL(uint32_t, res); \
        asm volatile( \
            "svc %[svc_id]\n" \
            metadata \
            : UVISOR_MACRO_GCC_ASM_OUTPUT(res) \
            : UVISOR_MACRO_GCC_ASM_INPUT(__VA_ARGS__), \
              [svc_id] "I" ((id) & 0xFF) \
        ); \
        res; \
    })

#define UVISOR_FUNCTION_CALL(dst_fn, ...) \
    ({ \
        UVISOR_MACRO_REGS_ARGS(uint32_t, ##__VA_ARGS__); \
        UVISOR_MACRO_REGS_RETVAL(uint32_t, res); \
        asm volatile( \
            "bl " UVISOR_TO_STRING(dst_fn) "\n" \
            : UVISOR_MACRO_GCC_ASM_OUTPUT(res) \
            : UVISOR_MACRO_GCC_ASM_INPUT(__VA_ARGS__) \
        ); \
        res; \
    })

#endif /* defined(__CC_ARM) || defined(__GNUC__) */

#endif /* __UVISOR_API_SVC_EXPORTS_H__ */
