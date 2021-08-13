/*
 * Copyright (c) 2017-2018, Arm Limited. All rights reserved.
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
    /* Following entry is only to ensure the error code of int size */
    TFM_PLAT_ERR_FORCE_INT_SIZE = INT_MAX
};

/*!
 * \def TFM_LINK_SET_OBJECT_IN_PARTITION_SECTION(TFM_PARTITION_NAME)
 *
 * \brief This macro provides a mechanism to place a function code in a specific
 *        secure partition at linker time in TF-M Level 3.
 *
 * \param[in] TFM_PARTITION_NAME  TF-M partition name assigned in the manifest
 *                                file "tfm_partition_name" field.
 */
#define TFM_LINK_SET_OBJECT_IN_PARTITION_SECTION(TFM_PARTITION_NAME) \
                __attribute__((section(TFM_PARTITION_NAME"_ATTR_FN")))

#endif /* __TFM_PLAT_DEFS_H__ */
