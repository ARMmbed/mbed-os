/*
 * Copyright (c) 2018-2019, Nuvoton Technology Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TARGET_CFG_H__
#define __TARGET_CFG_H__

#include "tfm_peripherals_def.h"

/* Mask for SCU security violation interrupts */
#define NU_SCU_SV_MSAK      (((1 << 18) - 1) & ~(3 << 2))

extern const struct memory_region_limits memory_regions;

/**
 * \brief Store the addresses of memory regions
 */
struct memory_region_limits {
    uint32_t non_secure_code_start;
    uint32_t non_secure_partition_base;
    uint32_t non_secure_partition_limit;
    uint32_t veneer_base;
    uint32_t veneer_limit;
};

/**
 * \brief Holds the data necessary to do isolation for a specific peripheral.
 */
struct tfm_spm_partition_platform_data_t
{
    uint32_t periph_start;
    uint32_t periph_limit;
};

#endif /* __TARGET_CFG_H__ */
