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
 * \file cache_drv.h
 * \brief Driver for L1 instruction cache based on SSE-200 version r1p0
 */

#ifndef __ARM_CACHE_DRV_H__
#define __ARM_CACHE_DRV_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief L1 cache configuration structure
 */
struct arm_cache_dev_cfg_t {
    const uint32_t base;     /*!< L1 cache base address */
};

/**
 * \brief L1 cache device structure
 */
struct arm_cache_dev_t {
    const struct arm_cache_dev_cfg_t* const cfg;  /*!< L1 cache configuration */
};

#define ARM_CACHEICHWPARAMS_CSIZE_OFF       0x0u
                                /*!< Instruction cache size bit field offset */
#define ARM_CACHEICHWPARAMS_STATS_OFF       0x4u
                                /*!< Statistic functionality bit field offset */
#define ARM_CACHEICHWPARAMS_DMA_OFF         0x5u
                                /*!< DMA engine bit field offset */
#define ARM_CACHEICHWPARAMS_INVMAT_OFF      0x6u
                                /*!< Bit field offset to indicates whether
                                 * invalidate cache line on write match is
                                 * enabled */
#define ARM_CACHEICHWPARAMS_COFFSIZE_OFF    0xCu
                                /*!< Cacheable Block Size bit field offset */
#define ARM_CACHEICHWPARAMS_COFFSET_OFF     0x10u
                                /*!< Cacheable Offset Addr bit field offset */

#define ARM_CACHEICCTRL_CACHEEN_OFF         0x0u
                                /*!< Cache enable bit field offset */
#define ARM_CACHEICCTRL_FINV_OFF            0x2u
                                /*!< Full invalidate bit field offset */
#define ARM_CACHEICCTRL_STATEN_OFF          0x3u
                                /*!< Enable Statistic bit field offset */
#define ARM_CACHEICCTRL_STATC_OFF           0x4u
                                /*!< Clear Statistic bit field offset */
#define ARM_CACHEICCTRL_HALLOC_OFF          0x5u
                                /*!< Enable handler alloc bit field offset */

#define ARM_CACHE_INTR_IC_OFF   0x0u /*!< Invalidate Complete IRQ offset */
#define ARM_CACHE_INTR_CDC_OFF  0x1u /*!< Cache Disable Complete IRQ offset */
#define ARM_CACHE_INTR_CEC_OFF  0x2u /*!< Cache Enable Complete IRQ offset */
#define ARM_CACHE_INTR_CFE_OFF  0x3u /*!< Cache Fill Error IRQ offset */
#define ARM_CACHE_INTR_SV_OFF   0x4u /*!< Security violation IRQ offset */
#define ARM_CACHE_INTR_SS_OFF   0x5u /*!< Statistics Saturated IRQ offset */

/**
 * \brief L1 Cache Interrupt data structure
 */
enum arm_cache_intr_t {
  arm_cache_ic_intr_mask  = (0x1u<<ARM_CACHE_INTR_IC_OFF),
                                  /*!< Invalidate Complete IRQ Mask */
  arm_cache_cdc_intr_mask = (0x1u<<ARM_CACHE_INTR_CDC_OFF),
                                  /*!< Cache Disable Complete IRQ Mask */
  arm_cache_cec_intr_mask = (0x1u<<ARM_CACHE_INTR_CEC_OFF),
                                  /*!< Cache Enable Complete IRQ Mask */
  arm_cache_cfe_intr_mask = (0x1u<<ARM_CACHE_INTR_CFE_OFF),
                                  /*!< Cache Fill Error IRQ Mask */
  arm_cache_sv_intr_mask  = (0x1u<<ARM_CACHE_INTR_SV_OFF),
                                  /*!< Security violation IRQ Mask */
  arm_cache_ss_intr_mask  = (0x1u<<ARM_CACHE_INTR_SS_OFF),
                                  /*!< Statistics Saturated IRQ Mask */
};

/**
 * \brief L1 Cache size data structure
 */
enum arm_cache_size_t {
  arm_cache_size_err = 0,     /*!< Invalid Cache size*/
  arm_cache_size_512B = 9,    /*!< Cache size 512 byte*/
  arm_cache_size_1KB = 10,    /*!< Cache size 1KB*/
  arm_cache_size_2KB = 11,    /*!< Cache size 2KB*/
  arm_cache_size_4KB = 12,    /*!< Cache size 4KB*/
  arm_cache_size_8KB = 13,    /*!< Cache size 8KB*/
  arm_cache_size_16KB = 14,   /*!< Cache size 16KB*/
};

/**
 * \brief Returns cache size
 *
 * \param[in] dev         L1 cache device struct \ref arm_cache_dev_t
 *
 * \return Returns cache size struct \ref arm_cache_size_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum arm_cache_size_t arm_cache_get_size(struct arm_cache_dev_t* dev);

/**
 * \brief Check if statistic functionality is available in Icache
 *
 * \param[in] dev         L1 cache device struct \ref arm_cache_dev_t
 *
 * \return Returns bool, true if statistic functionality is available,
 * false otherwise
 *
 * \note This function doesn't check if dev is NULL.
 */
bool arm_cache_is_stat_func_available(struct arm_cache_dev_t* dev);

/**
 * \brief Check if invalidate cache line on write match is enabled
 *
 * \param[in] dev         L1 cache device struct \ref arm_cache_dev_t
 *
 * \return Returns bool, true if invalidate cache line is enabled,
 * false otherwise
 *
 * \note This function doesn't check if dev is NULL.
 */
bool arm_cache_is_invalidate_cache_line_enabled(struct arm_cache_dev_t* dev);

/**
 * \brief Enables cache in ARM SSE-200.
 *
 * \param[in] dev         L1 cache device struct \ref arm_cache_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_cache_enable(struct arm_cache_dev_t* dev);

/**
 * \brief Enables cache, returns when enable action is completed.
 *
 * \param[in] dev         L1 cache device struct \ref arm_cache_dev_t
 *
 * \note Ensure that Cache Enable Interrupt is cleared before calling this API,
 *       \ref arm_cache_clear_intr()
 * \note This is a blocking API, returns when the cache is enabled
 * \note This function doesn't check if dev is NULL.
 */
void arm_cache_enable_blocking(struct arm_cache_dev_t* dev);

/**
 * \brief Disables cache in ARM SSE-200.
 *
 * \param[in] dev         L1 cache device struct \ref arm_cache_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_cache_disable(struct arm_cache_dev_t* dev);

/**
 * \brief Disables cache, returns when disable action is completed.
 *
 * \param[in] dev         L1 cache device struct \ref arm_cache_dev_t
 *
 * \note Ensure that Cache Disable Interrupt is cleared before calling this API,
 *       \ref arm_cache_clear_intr()
 * \note This is a blocking API, returns when the cache is disabled
 * \note This function doesn't check if dev is NULL.
 */
void arm_cache_disable_blocking(struct arm_cache_dev_t* dev);

/**
 * \brief Check if cache is enabled.
 *
 * \param[in] dev         L1 cache device struct \ref arm_cache_dev_t
 *
 * \return Returns bool, true if cache is enabled, false otherwise
 *
 * \note This function doesn't check if dev is NULL.
 */
bool arm_cache_is_enabled(struct arm_cache_dev_t* dev);

/**
 * \brief Invalidates full cache in ARM SSE-200.
 *
 * \param[in] dev         L1 cache device struct \ref arm_cache_dev_t
 *
 * \note Calling this API will trigger the Instruction cache to invalidate
 *       all cache lines.
 * \note This function doesn't check if dev is NULL.
 */
void arm_cache_full_invalidate(struct arm_cache_dev_t* dev);

/**
 * \brief Invalidates full cache, returns when invalidated action is completed.
 *
 * \param[in] dev         L1 cache device struct \ref arm_cache_dev_t
 *
 * \note Ensure that Invalidate Complete Intr is cleared before calling this API
 *       \ref arm_cache_clear_intr()
 * \note This is a blocking API, returns when the cache is invalidated
 * \note Calling this API will trigger the Instruction cache to invalidate
 *       all cache lines.
 * \note This function doesn't check if dev is NULL.
 */
void arm_cache_full_invalidate_blocking(struct arm_cache_dev_t* dev);

/**
 * \brief Enables statistic function for cache in ARM SSE-200.
 *
 * \param[in] dev         L1 cache device struct \ref arm_cache_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_cache_statistic_enable(struct arm_cache_dev_t* dev);

/**
 * \brief Disables statistic function for cache in ARM SSE-200.
 *
 * \param[in] dev         L1 cache device struct \ref arm_cache_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_cache_statistic_disable(struct arm_cache_dev_t* dev);

/**
 * \brief Clear statistic values for cache in ARM SSE-200.
 *
 * \param[in] dev         L1 cache device struct \ref arm_cache_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_cache_clear_statistic_value(struct arm_cache_dev_t* dev);

/**
 * \brief Enables handler allocation function for cache in ARM SSE-200.
 *
 * \param[in] dev         L1 cache device struct \ref arm_cache_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_cache_handler_alloc_enable(struct arm_cache_dev_t* dev);

/**
 * \brief Disables handler allocation function for cache in ARM SSE-200.
 *
 * \param[in] dev         L1 cache device struct \ref arm_cache_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_cache_handler_alloc_disable(struct arm_cache_dev_t* dev);

/**
 * \brief Enables cache interrupts
 *
 * \param[in] dev    Cache device struct \ref arm_cache_dev_t
 * \param[in] mask   Bit mask for enabling interrupts \ref arm_cache_intr_t
 *
 * \note User is responsible to set mask accordingly. Mask set to 0 will not
 *       have any effect.
 * \note User is responsible to configure the interrupt vector and
 *       the interrupt controller.
 * \note This function doesn't check if dev is NULL.
 */
void arm_cache_enable_intr(struct arm_cache_dev_t* dev,
                    enum arm_cache_intr_t mask);

/**
 * \brief Disables cache interrupts
 *
 * \param[in] dev    Cache device struct \ref arm_cache_dev_t
 * \param[in] mask   Bit mask for disabling interrupts \ref arm_cache_intr_t
 *
 * \note User is responsible to set mask accordingly. Mask set to 0 will not
 *       have any effect.
 * \note This function doesn't check if dev is NULL.
 */
void arm_cache_disable_intr(struct arm_cache_dev_t* dev,
                    enum arm_cache_intr_t mask);

/**
 * \brief Clears cache Interrupt
 *
 * \param[in] dev  Cache device struct \ref arm_cache_dev_t
 * \param[in] mask Bit mask for clearing interrupts \ref arm_cache_intr_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_cache_clear_intr(struct arm_cache_dev_t* dev,
                    enum arm_cache_intr_t mask);

/**
 * \brief Returns the cache Masked interrupt status
 *
 * \param[in] dev  Cache device struct \ref arm_cache_dev_t
 *
 * \return Masked interrupt status \ref arm_cache_intr_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum arm_cache_intr_t arm_cache_get_masked_intr_status(
                    struct arm_cache_dev_t* dev);

/**
 * \brief Returns the cache Raw interrupt status
 *
 * \param[in] dev  Cache device struct \ref arm_cache_dev_t
 *
 * \return Raw interrupt status \ref arm_cache_intr_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum arm_cache_intr_t arm_cache_get_raw_intr_status(
                    struct arm_cache_dev_t* dev);

/**
 * \brief Returns the debug fill error address
 *
 * \param[in] dev  Cache device struct \ref arm_cache_dev_t
 *
 * \return Address that is involved in a recent fill error
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t arm_cache_get_debug_fill_address(struct  arm_cache_dev_t* dev);

/**
 * \brief Returns the Icache hit counter value
 *
 * \param[in] dev  Cache device struct \ref arm_cache_dev_t
 *
 * \return Icache hit counter value since the last counter clear operation
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t arm_cache_get_hit_count(struct  arm_cache_dev_t* dev);

/**
 * \brief Returns the Icache miss counter value
 *
 * \param[in] dev  Cache device struct \ref arm_cache_dev_t
 *
 * \return Icache miss counter value since the last counter clear operation
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t arm_cache_get_miss_count(struct  arm_cache_dev_t* dev);

/**
 * \brief Returns the Icache uncached access counter value
 *
 * \param[in] dev  Cache device struct \ref arm_cache_dev_t
 *
 * \return Icache uncached counter value since the last counter clear operation
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t arm_cache_get_uncached_count(struct  arm_cache_dev_t* dev);

#ifdef __cplusplus
}
#endif
#endif /* __ARM_CACHE_DRV_H__ */

