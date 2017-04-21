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
#ifndef __UVISOR_API_DISABLED_H__
#define __UVISOR_API_DISABLED_H__

#include "api/inc/uvisor_exports.h"
#include <stdint.h>

UVISOR_EXTERN void uvisor_disabled_switch_in(const uint32_t *dst_box_cfgtbl_ptr);
UVISOR_EXTERN void uvisor_disabled_switch_out(void);

/* The host OS can override the implementations of these functions in case a
 * different handling of IRQs is required when uVisor is disabled. */
UVISOR_EXTERN void uvisor_disabled_set_vector(uint32_t irqn, uint32_t vector);
UVISOR_EXTERN uint32_t uvisor_disabled_get_vector(uint32_t irqn);

#endif /* __UVISOR_API_DISABLED_H__ */
