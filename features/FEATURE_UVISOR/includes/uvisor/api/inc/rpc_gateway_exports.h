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
#ifndef __UVISOR_API_RPC_GATEWAY_EXPORTS_H__
#define __UVISOR_API_RPC_GATEWAY_EXPORTS_H__

#include "api/inc/uvisor_exports.h"
#include "api/inc/magic_exports.h"
#include <stdint.h>

/* ldr pc, [pc, #<label - instr + 4>]
 * LDR (immediate) - ARMv7M ARM section A7.7.42
 * 1111;1 00 0; 0 10 1; <Rn - 1111>; <Rt - 1111>; <imm12> (Encoding T3) */
#define LDR_PC_PC_IMM_OPCODE(instr, label) \
    ((uint32_t) (0xF000F8DFUL | ((((uint32_t) (label) - ((uint32_t) (instr) + 4)) & 0xFFFUL) << 16)))

/** RPC gateway structure
 *
 * This struct is packed because we must ensure that the `ldr_pc` field has no
 * padding before itself and will be located at a valid instruction location,
 * and that the `caller` and `target` field are at a pre-determined offset from
 * the `ldr_pc` field.
 */
typedef struct RPCGateway {
    uint32_t ldr_pc;
    uint32_t magic;
    uint32_t box_ptr;
    uint32_t target;
    uint32_t caller; /* This is not for use by anything other than the ldr_pc. It's like a pretend literal pool. */
} UVISOR_PACKED UVISOR_ALIGN(4) TRPCGateway;

#endif /* __UVISOR_API_RPC_GATEWAY_EXPORTS_H__ */
