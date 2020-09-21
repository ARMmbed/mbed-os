/*
 * Copyright (c) 2017-2020 Arm Limited
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

/**
 * \file cache_drv.c
 * \brief Driver for L1 instruction cache based on SSE-200 version r1p0
 */

#include "cache_drv.h"

/**
 * \brief L1 cache register map structure
 */
struct _arm_cache_reg_map_t {
    volatile uint32_t cacheichwparams;
             /*!< Offset: 0x000 (R/ ) HW Parameters Register */
    volatile uint32_t cacheicctrl;
             /*!< Offset: 0x004 (R/W) Control Register */
    volatile uint32_t reserved_0[62];
             /*!< Offset: 0x00C-0x0FC Reserved */
    volatile uint32_t cacheicirqstat;
             /*!< Offset: 0x100 (R/ ) Interrupt Request Status Register */
    volatile uint32_t cacheicirqsclr;
             /*!< Offset: 0x104 ( /W) Interrupt Status Clear Register */
    volatile uint32_t cacheicirqen;
             /*!< Offset: 0x108 (R/W) Interrupt Enable Register */
    volatile uint32_t cacheicdbgfillerr;
             /*!< Offset: 0x10C (R/ ) Fill Error Address Register */
    volatile uint32_t reserved_1[124];
             /*!< Offset: 0x110-0x2FC Reserved */
    volatile uint32_t cacheicsh;
             /*!< Offset: 0x300 (R/ ) Cache Statistic Hit Register */
    volatile uint32_t cacheicsm;
             /*!< Offset: 0x304 (R/ ) Cache Statistic Miss Register */
    volatile uint32_t cacheicsuc;
             /*!< Offset: 0x308 (R/ ) Cache Statistic Uncached Register */
    volatile uint32_t reserved_2[331];
             /*!< Offset: 0x30C-0xFCC Reserved */
    volatile uint32_t cachepidr4;
             /*!< Offset: 0xFD0 (R/ ) Product ID Register 4 */
    volatile uint32_t cachepidr5;
             /*!< Offset: 0xFD4 (R/ ) Product ID Register 5 */
    volatile uint32_t cachepidr6;
             /*!< Offset: 0xFD8 (R/ ) Product ID Register 6 */
    volatile uint32_t cachepidr7;
             /*!< Offset: 0xFDC (R/ ) Product ID Register 7 */
    volatile uint32_t cachepidr0;
             /*!< Offset: 0xFE0 (R/ ) Product ID Register 0 */
    volatile uint32_t cachepidr1;
             /*!< Offset: 0xFE4 (R/ ) Product ID Register 1 */
    volatile uint32_t cachepidr2;
             /*!< Offset: 0xFE8 (R/ ) Product ID Register 2 */
    volatile uint32_t cachepidr3;
             /*!< Offset: 0xFEC (R/ ) Product ID Register 3 */
    volatile uint32_t cachecidr0;
             /*!< Offset: 0xFF0 (R/ ) Component ID Register 0 */
    volatile uint32_t cachecidr1;
             /*!< Offset: 0xFF4 (R/ ) Component ID Register 1 */
    volatile uint32_t cachecidr2;
             /*!< Offset: 0xFF8 (R/ ) Component ID Register 2 */
    volatile uint32_t cachecidr3;
             /*!< Offset: 0xFFC (R/ ) Component ID Register 3 */
};

#define ARM_CACHEICHWPARAMS_CSIZE_MASK (0xFu<<ARM_CACHEICHWPARAMS_CSIZE_OFF)
#define ARM_CACHEICHWPARAMS_STATS_MASK (0x1u<<ARM_CACHEICHWPARAMS_STATS_OFF)
#define ARM_CACHEICHWPARAMS_INVMAT_MASK (0x1u<<ARM_CACHEICHWPARAMS_INVMAT_OFF)

#define ARM_CACHEICCTRL_CACHEEN_MASK (0x1u<<ARM_CACHEICCTRL_CACHEEN_OFF)
#define ARM_CACHEICCTRL_PINV_MASK    (0x1u<<ARM_CACHEICCTRL_PINV_OFF)
#define ARM_CACHEICCTRL_FINV_MASK    (0x1u<<ARM_CACHEICCTRL_FINV_OFF)
#define ARM_CACHEICCTRL_STATEN_MASK  (0x1u<<ARM_CACHEICCTRL_STATEN_OFF)
#define ARM_CACHEICCTRL_STATC_MASK   (0x1u<<ARM_CACHEICCTRL_STATC_OFF)
#define ARM_CACHEICCTRL_HALLOC_MASK  (0x1u<<ARM_CACHEICCTRL_HALLOC_OFF)

enum arm_cache_size_t arm_cache_get_size(struct arm_cache_dev_t* dev)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    enum arm_cache_size_t val = (enum arm_cache_size_t)
    (p_cache->cacheichwparams & ARM_CACHEICHWPARAMS_CSIZE_MASK);
    /**
     *  9: 512 byte
     *  10: 1 KB
     *  11: 2 KB
     *  12: 4 KB
     *  13: 8 KB
     *  14: 16 KB
     *  Other values are reserved, returning error
     */
    if(val < arm_cache_size_512B || val > arm_cache_size_16KB) {
        return arm_cache_size_err;
    } else {
        return val;
    }
}

bool arm_cache_is_stat_func_available(struct arm_cache_dev_t* dev)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    return (bool)(p_cache->cacheichwparams & ARM_CACHEICHWPARAMS_STATS_MASK);
}

bool arm_cache_is_invalidate_cache_line_enabled(struct arm_cache_dev_t* dev)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    return (bool)(p_cache->cacheichwparams & ARM_CACHEICHWPARAMS_INVMAT_MASK);
}

void arm_cache_enable(struct arm_cache_dev_t* dev)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    p_cache->cacheicctrl |= ARM_CACHEICCTRL_CACHEEN_MASK;

    return;
}

void arm_cache_enable_blocking(struct arm_cache_dev_t* dev)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    p_cache->cacheicctrl |= ARM_CACHEICCTRL_CACHEEN_MASK;

    while(!(arm_cache_get_raw_intr_status(dev) & arm_cache_cec_intr_mask));

    arm_cache_clear_intr(dev, arm_cache_cec_intr_mask);

    return;
}

void arm_cache_disable(struct arm_cache_dev_t* dev)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    p_cache->cacheicctrl &= ~ARM_CACHEICCTRL_CACHEEN_MASK;

    return;
}

void arm_cache_disable_blocking(struct arm_cache_dev_t* dev)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    p_cache->cacheicctrl &= ~ARM_CACHEICCTRL_CACHEEN_MASK;

    while(!(arm_cache_get_raw_intr_status(dev) & arm_cache_cdc_intr_mask));

    arm_cache_clear_intr(dev, arm_cache_cdc_intr_mask);

    return;
}

bool arm_cache_is_enabled(struct arm_cache_dev_t* dev)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;
    return (bool)(p_cache->cacheicctrl & ARM_CACHEICCTRL_CACHEEN_MASK);
}

void arm_cache_full_invalidate(struct arm_cache_dev_t* dev)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    p_cache->cacheicctrl |= ARM_CACHEICCTRL_FINV_MASK;

    return;
}

void arm_cache_full_invalidate_blocking(struct arm_cache_dev_t* dev)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    p_cache->cacheicctrl |= ARM_CACHEICCTRL_FINV_MASK;

    while(!(arm_cache_get_raw_intr_status(dev) & arm_cache_ic_intr_mask));

    arm_cache_clear_intr(dev, arm_cache_ic_intr_mask);

    return;
}

void arm_cache_statistic_enable(struct arm_cache_dev_t* dev)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    p_cache->cacheicctrl |= ARM_CACHEICCTRL_STATEN_MASK;

    return;
}

void arm_cache_statistic_disable(struct arm_cache_dev_t* dev)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    p_cache->cacheicctrl &= ~ARM_CACHEICCTRL_STATEN_MASK;

    return;
}

void arm_cache_clear_statistic_value(struct arm_cache_dev_t* dev)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    p_cache->cacheicctrl |= ARM_CACHEICCTRL_STATC_MASK;

    return;
}

void arm_cache_handler_alloc_enable(struct arm_cache_dev_t* dev)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    p_cache->cacheicctrl |= ARM_CACHEICCTRL_HALLOC_MASK;

    return;
}

void arm_cache_handler_alloc_disable(struct arm_cache_dev_t* dev)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    p_cache->cacheicctrl &= ~ARM_CACHEICCTRL_HALLOC_MASK;

    return;
}

void arm_cache_enable_intr(struct arm_cache_dev_t* dev,
                    enum arm_cache_intr_t mask)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    p_cache->cacheicirqen |= (uint32_t)(mask);

    return;
}

void arm_cache_disable_intr(struct arm_cache_dev_t* dev,
                    enum arm_cache_intr_t mask)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    p_cache->cacheicirqen &= ~(uint32_t)(mask);

    return;
}
void arm_cache_clear_intr(struct arm_cache_dev_t* dev,
                    enum arm_cache_intr_t mask)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    p_cache->cacheicirqsclr = (uint32_t)mask;

    return;
}

enum arm_cache_intr_t arm_cache_get_masked_intr_status(
                    struct arm_cache_dev_t* dev)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    return (enum arm_cache_intr_t)
       (p_cache->cacheicirqstat & p_cache->cacheicirqen);
}

enum arm_cache_intr_t arm_cache_get_raw_intr_status(
                    struct arm_cache_dev_t* dev)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    return (enum arm_cache_intr_t)(p_cache->cacheicirqstat);
}

uint32_t arm_cache_get_debug_fill_address(struct  arm_cache_dev_t* dev)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    return p_cache->cacheicdbgfillerr;
}

uint32_t arm_cache_get_hit_count(struct  arm_cache_dev_t* dev)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    return p_cache->cacheicsh;
}

uint32_t arm_cache_get_miss_count(struct  arm_cache_dev_t* dev)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    return p_cache->cacheicsm;
}

uint32_t arm_cache_get_uncached_count(struct  arm_cache_dev_t* dev)
{
    struct _arm_cache_reg_map_t* p_cache =
        (struct _arm_cache_reg_map_t*)dev->cfg->base;

    return p_cache->cacheicsuc;
}
