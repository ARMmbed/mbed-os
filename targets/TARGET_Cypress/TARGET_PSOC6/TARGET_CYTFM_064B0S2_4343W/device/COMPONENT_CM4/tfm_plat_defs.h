/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PLAT_DEFS_H__
#define __TFM_PLAT_DEFS_H__
/**
 * \note The interfaces defined in this file must be implemented for each
 *       target.
 */

#include <stdint.h>
#include <limits.h>

enum tfm_plat_err_t {
    TFM_PLAT_ERR_SUCCESS = 0,
    TFM_PLAT_ERR_SYSTEM_ERR,
    TFM_PLAT_ERR_MAX_VALUE,
    TFM_PLAT_ERR_INVALID_INPUT,
    TFM_PLAT_ERR_UNSUPPORTED,
    /* Following entry is only to ensure the error code of int size */
    TFM_PLAT_ERR_FORCE_INT_SIZE = INT_MAX
};

#if defined(TFM_PSA_API) && defined(TFM_LVL) && (TFM_LVL != 1)

/*!
 * \def TFM_LINK_SET_RO_IN_PARTITION_SECTION(TFM_PARTITION_NAME)
 *
 * \brief This macro provides a mechanism to place a function code or a data
 *        variable in the code section (e.g. RO) of a specific secure partition
 *        at linker time.
 *
 * \param[in] TFM_PARTITION_NAME  TF-M partition name assigned in the manifest
 *                                file "name" field.
 */
#define TFM_LINK_SET_RO_IN_PARTITION_SECTION(TFM_PARTITION_NAME) \
                __attribute__((section(TFM_PARTITION_NAME"_ATTR_FN")))

/*!
 * \def TFM_LINK_SET_RW_IN_PARTITION_SECTION(TFM_PARTITION_NAME)
 *
 * \brief This macro provides a mechanism to place data variables in the RW data
 *        section of a specific secure partition at linker time.
 *
 * \param[in] TFM_PARTITION_NAME  TF-M partition name assigned in the manifest
 *                                file "name" field.
 */
#define TFM_LINK_SET_RW_IN_PARTITION_SECTION(TFM_PARTITION_NAME) \
                __attribute__((section(TFM_PARTITION_NAME"_ATTR_RW")))

/*!
 * \def TFM_LINK_SET_ZI_IN_PARTITION_SECTION(TFM_PARTITION_NAME)
 *
 * \brief This macro provides a mechanism to place data variables in the ZI data
 *        section of a specific secure partition at linker time.
 *
 * \param[in] TFM_PARTITION_NAME  TF-M partition name assigned in the manifest
 *                                file "name" field.
 */
#define TFM_LINK_SET_ZI_IN_PARTITION_SECTION(TFM_PARTITION_NAME) \
                __attribute__((section(TFM_PARTITION_NAME"_ATTR_ZI")))
#else
#define TFM_LINK_SET_RO_IN_PARTITION_SECTION(TFM_PARTITION_NAME)
#define TFM_LINK_SET_RW_IN_PARTITION_SECTION(TFM_PARTITION_NAME)
#define TFM_LINK_SET_ZI_IN_PARTITION_SECTION(TFM_PARTITION_NAME)
#endif
#endif /* __TFM_PLAT_DEFS_H__ */
