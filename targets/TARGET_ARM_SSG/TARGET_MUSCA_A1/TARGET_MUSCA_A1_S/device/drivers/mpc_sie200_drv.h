/*
 * Copyright (c) 2016-2019 Arm Limited
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
 * \file mpc_sie200_drv.h
 * \brief Generic driver for ARM SIE 200 Memory Protection
 *        Controllers (MPC).
 */

#ifndef __MPC_SIE_200_DRV_H__
#define __MPC_SIE_200_DRV_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Error code returned by the driver functions */
enum mpc_sie200_error_t {
    MPC_SIE200_ERR_NONE,          /*!< No error */
    MPC_SIE200_INVALID_ARG,       /*!< MPC invalid input arguments */
    MPC_SIE200_NOT_INIT,          /*!< MPC not initialized */
    MPC_SIE200_ERR_NOT_IN_RANGE,  /*!< Address does not belong to a range
                                   *   controlled by the MPC */
    MPC_SIE200_ERR_NOT_ALIGNED,   /*!< Address is not aligned on the block size
                                   *   of this MPC */
    MPC_SIE200_ERR_INVALID_RANGE, /*!< The given address range to configure
                                   *   is invalid. This could be because:
                                   *   - The base and limit swapped
                                   *   - The base and limit addresses
                                   *     are in different ranges */
    MPC_SIE200_ERR_RANGE_SEC_ATTR_NON_COMPATIBLE, /*!< The given range cannot be
                                                   *   accessed with the wanted
                                                   *   security attributes */
};

/* Security attribute used in various place of the API */
enum mpc_sie200_sec_attr_t {
    MPC_SIE200_SEC_ATTR_SECURE,     /*!< Secure attribute */
    MPC_SIE200_SEC_ATTR_NONSECURE,  /*!< Non-secure attribute */
    /*!< Used when getting the configuration of a memory range and some blocks
     *   are secure whereas some other are non secure */
    MPC_SIE200_SEC_ATTR_MIXED,
};

/* What can happen when trying to do an illegal memory access */
enum mpc_sie200_sec_resp_t {
    MPC_SIE200_RESP_RAZ_WI,    /*!< Read As Zero, Write Ignored */
    MPC_SIE200_RESP_BUS_ERROR  /*!< Bus error */
};

/* Description of a memory range controlled by the MPC */
struct mpc_sie200_memory_range_t {
    const uint32_t base;   /*!< Base address (included in the range) */
    const uint32_t limit;  /*!< Limit address (included in the range) */
    const uint32_t range_offset; /*!< Offset of current range area to the 0
                                      point of the whole area (the sum of the
                                      sizes of the previous memory ranges
                                      covered by the same MPC) */
    const enum mpc_sie200_sec_attr_t attr; /*!< Optional security attribute
                                                needed to be matched when
                                                accessing this range.
                                                For example, the non-secure
                                                alias of a memory region can not
                                                be accessed using secure access,
                                                and configuring the MPC to
                                                secure using that range will not
                                                be permitted by the driver. */
};

/* ARM MPC SIE 200 device configuration structure */
struct mpc_sie200_dev_cfg_t {
    const uint32_t base;  /*!< MPC base address */
};

/* ARM MPC SIE 200 device data structure */
struct mpc_sie200_dev_data_t {
    const struct mpc_sie200_memory_range_t** range_list;  /*!< Array of pointers
                                                               to memory ranges
                                                               controlled by
                                                               the MPC */
    uint8_t nbr_of_ranges;  /*!< Number of memory ranges in the list */
    uint8_t state;          /*!< Indicates if the MPC driver
                                 is initialized and enabled */
    uint16_t reserved;      /*!< 32 bits alignment */
};

/* ARM MPC SIE 200 device structure */
struct mpc_sie200_dev_t {
    const struct mpc_sie200_dev_cfg_t* const cfg;  /*!< MPC configuration */
    struct mpc_sie200_dev_data_t* const data;      /*!< MPC data */
};

/**
 * \brief Initializes a MPC device.
 *
 * \param[in] dev            MPC device \ref mpc_sie200_dev_t
 * \param[in] range_list     List of memory ranges controller by the MPC
 *                           (\ref mpc_sie200_memory_range_t). This list can not
 *                           freed after the initializations.
 * \param[in] nbr_of_ranges  Number of memory ranges
 *
 * \return Returns error code as specified in \ref mpc_sie200_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum mpc_sie200_error_t mpc_sie200_init(struct mpc_sie200_dev_t* dev,
                            const struct mpc_sie200_memory_range_t** range_list,
                            uint8_t nbr_of_ranges);

/**
 * \brief Gets MPC block size. All regions must be aligned on this block
 *        size (base address and limit+1 address).
 *
 * \param[in]  dev       MPC device \ref mpc_sie200_dev_t
 * \param[out] blk_size  MPC block size
 *
 * \return Returns error code as specified in \ref mpc_sie200_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum mpc_sie200_error_t mpc_sie200_get_block_size(struct mpc_sie200_dev_t* dev,
                                                  uint32_t* blk_size);

/**
 * \brief Configures a memory region (base and limit included).
 *
 * \param[in] dev    MPC device \ref mpc_sie200_dev_t
 * \param[in] base   Base address of the region to poll. This bound is
 *                   included. It does not need to be aligned in any way.
 *
 * \param[in] limit  Limit address of the region to poll. This bound is
 *                   included. (limit+1) does not need to be aligned
 *                   in any way.
 * \param[in] attr   Security attribute of the region. If the region has mixed
 *                   secure/non-secure, a special value is returned
 *                   (\ref mpc_sie200_sec_attr_t).
 *
 *            In case base and limit+1 addresses are not aligned on
 *            the block size, the enclosing region with base and
 *            limit+1 aligned on block size will be queried.
 *            In case of early termination of the function (error), the
 *            security attribute will be set to MPC_SIE200_ATTR_MIXED.
 *
 * \return Returns error code as specified in \ref mpc_sie200_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum mpc_sie200_error_t mpc_sie200_config_region(struct mpc_sie200_dev_t* dev,
                                               const uint32_t base,
                                               const uint32_t limit,
                                               enum mpc_sie200_sec_attr_t attr);

/**
 * \brief Gets a memory region configuration(base and limit included).
 *
 * \param[in]  dev    MPC device \ref mpc_sie200_dev_t
 * \param[in]  base   Base address of the region to get the configuration.
 * \param[in]  limit  Limit address of the region to get the configuration.
 * \param[out] attr   Security attribute of the region
 *                    \ref mpc_sie200_sec_attr_t
 *
 * \return Returns error code as specified in \ref mpc_sie200_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum mpc_sie200_error_t mpc_sie200_get_region_config(
                                              struct mpc_sie200_dev_t* dev,
                                              uint32_t base,
                                              uint32_t limit,
                                              enum mpc_sie200_sec_attr_t* attr);

/**
 * \brief Gets the MPC control value.
 *
 * \param[in]  dev       MPC device \ref mpc_sie200_dev_t
 * \param[out] ctrl_val  Current MPC control value.
 *
 * \return Returns error code as specified in \ref mpc_sie200_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum mpc_sie200_error_t mpc_sie200_get_ctrl(struct mpc_sie200_dev_t* dev,
                                            uint32_t* ctrl_val);

/**
 * \brief Sets the MPC control value.
 *
 * \param[in] dev       MPC device \ref mpc_sie200_dev_t
 * \param[in] mpc_ctrl  New MPC control value
 *
 * \return Returns error code as specified in \ref mpc_sie200_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum mpc_sie200_error_t mpc_sie200_set_ctrl(struct mpc_sie200_dev_t* dev,
                                            uint32_t mpc_ctrl);

/**
 * \brief Gets the configured secure response.
 *
 * \param[in]  dev      MPC device \ref mpc_sie200_dev_t
 * \param[out] sec_rep  Configured secure response (\ref mpc_sie200_sec_resp_t).
 *
 * \return Returns error code as specified in \ref mpc_sie200_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum mpc_sie200_error_t mpc_sie200_get_sec_resp(struct mpc_sie200_dev_t* dev,
                                           enum mpc_sie200_sec_resp_t* sec_rep);

/**
 * \brief Enables MPC interrupt.
 *
 * \param[in] dev  MPC device \ref mpc_sie200_dev_t
 *
 * \return Returns error code as specified in \ref mpc_sie200_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum mpc_sie200_error_t mpc_sie200_irq_enable(struct mpc_sie200_dev_t* dev);

/**
 * \brief Disables MPC interrupt
 *
 * \param[in] dev  MPC device \ref mpc_sie200_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void mpc_sie200_irq_disable(struct mpc_sie200_dev_t* dev);

/**
 * \brief Clears MPC interrupt.
 *
 * \param[in] dev  MPC device \ref mpc_sie200_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void mpc_sie200_clear_irq(struct mpc_sie200_dev_t* dev);

/**
 * \brief Returns the MPC interrupt state.
 *
 * \param[in] dev  MPC device \ref mpc_sie200_dev_t
 *
 * \return Returns 1 if the interrupt is active, 0 otherwise.
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t mpc_sie200_irq_state(struct mpc_sie200_dev_t* dev);

/**
 * \brief Locks down the MPC configuration.
 *
 * \param[in] dev  MPC device \ref mpc_sie200_dev_t
 *
 * \return Returns error code as specified in \ref mpc_sie200_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum mpc_sie200_error_t mpc_sie200_lock_down(struct mpc_sie200_dev_t* dev);

#ifdef __cplusplus
}
#endif
#endif /* __MPC_SIE_200_DRV_H__ */
