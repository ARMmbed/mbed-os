/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
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
#ifndef __UVISOR_API_MAGIC_EXPORTS_H__
#define __UVISOR_API_MAGIC_EXPORTS_H__

#include <stdint.h>

/* udf imm16
 * UDF - ARMv7M ARM section A7.7.191
 * 111 1;0 111;1111; <imm4>; 1 01 0; <imm12> (Encoding T2)
 */
#define UDF_OPCODE(imm16) \
    ((uint32_t) (0xA000F7F0UL | (((uint32_t) (imm16) & 0xFFFU) << 16U) | (((uint32_t) (imm16) & 0xF000UL) >> 12)))

/** Magics
 *
 * The following magics are used to verify various things within uVisor.The
 * magics are chosen to be one of the explicitly undefined Thumb-2
 * instructions.
 */
#if defined(__thumb__) && defined(__thumb2__)
#define UVISOR_RPC_GATEWAY_MAGIC_ASYNC UDF_OPCODE(0x07C2)
#define UVISOR_RPC_GATEWAY_MAGIC_SYNC  UDF_OPCODE(0x07C3)
#define UVISOR_POOL_MAGIC              UDF_OPCODE(0x07C4)
#define UVISOR_POOL_QUEUE_MAGIC        UDF_OPCODE(0x07C5)
#define UVISOR_DEBUG_BOX_MAGIC         UDF_OPCODE(0x07C6)
#else
#error "Unsupported instruction set. The ARM Thumb-2 instruction set must be supported."
#endif /* __thumb__ && __thumb2__ */


#endif /* __UVISOR_API_MAGIC_EXPORTS_H__ */
