/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/**
 * \file  psa_sst_asset_macros.h
 *
 * \brief Platform security architecture (PSA) asset macros definitions for
 *        secure storage partition
 */
#ifndef __PSA_SST_ASSET_MACROS_H__
#define __PSA_SST_ASSET_MACROS_H__

#include "psa_sst_asset_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \def PSA_SST_ASSET_IS_ASSET_CAT(psa_sst_def_cat_type, type)
 *
 * \brief Macro to check if the asset category type (type) is equal to the
 *        PSA SST asset defined category types (psa_sst_def_cat_type).
 *
 * \param[in] psa_sst_def_cat_type  PSA SST asset defined category type
 * \param[in] type                  Asset type
 *
 * \return Returns 1 if the asset type is equal to the PSA SST asset defined
 *         type. Otherwise, it returns 0.
 */
#define PSA_SST_ASSET_IS_ASSET_CAT(psa_sst_def_cat_type, type) \
        (((type & PSA_SST_ASSET_CAT_VENDOR_DEFINED) != 0) && \
          ((type & PSA_SST_ASSET_CAT_TYPE_MASK) == psa_sst_def_cat_type))

/*!
 * \def PSA_SST_ASSET_IS_ASSET_TYPE(psa_sst_def_type, type)
 *
 * \brief Macro to check if the asset type (type) is equal to the
 *        PSA SST asset defined type (psa_sst_def_type).
 *
 * \param[in] psa_sst_def_type  PSA SST asset defined type
 * \param[in] type              Asset type
 *
 * \return Returns 1 if the asset type is equal to the PSA SST asset defined
 *         type. Otherwise, it returns 0.
 */
#define PSA_SST_ASSET_IS_ASSET_TYPE(psa_sst_def_type, type) \
        (((type & PSA_SST_ASSET_CAT_VENDOR_DEFINED) != 0) && \
          (type == psa_sst_def_type))

/*!
 * \def PSA_SST_ASSET_IS_VENDOR_DEFINED(type)
 *
 * \brief Macro to check if the asset type is vendor defined.
 *
 * \param[in] type  Asset type
 *
 * \return Returns 1 if the asset type is equal to PSA_SST_ASSET_VENDOR_DEFINED.
 *         Otherwise, it returns 0.
 */
#define PSA_SST_ASSET_IS_VENDOR_DEFINED(type) \
        ((type & PSA_SST_ASSET_VENDOR_DEFINED) != 0)

/*!
 * \def PSA_SST_ASSET_IS_KEY_TYPE(type)
 *
 * \brief Macro to check if the asset type is a key.
 *
 * \param[in] type  Asset type
 *
 * \return Returns 1 if it is a key. Otherwise, it returns 0.
 */
#define PSA_SST_ASSET_IS_KEY_TYPE(type) \
        ((type & PSA_SST_ASSET_KEY_MASK) != 0)

/*!
 * \def PSA_SST_ASSET_IS_ASYMMETRIC_KEY_TYPE(type)
 *
 * \brief Macro to check if the asset type is a symmetric key.
 *
 * \param[in] type  Asset type
 *
 * \return Returns 1 if it is an asymmetric key. Otherwise, it returns 0.
 */
#define PSA_SST_ASSET_IS_ASYMMETRIC_KEY_TYPE(type) \
        ((type & PSA_SST_ASSET_KEY_ASYMMETRIC_MASK) != 0)

/*!
 * \def PSA_SST_ASSET_HAS_ATTR(psa_sst_def_asset_attr, attrs)
 *
 * \brief Macro to check if the asset attributes (attrs) have a specific PSA
 *        SST asset defined bit attribute (psa_sst_def_asset_attr).
 *
 * \param[in] psa_sst_def_type  PSA SST asset defined bit attribute
 * \param[in] attrs             Asset attributes
 *
 * \return Returns 1 if the asset has the PSA SST asset defined
 *         attribute. Otherwise, it returns 0.
 */
#define PSA_SST_ASSET_HAS_ATTR(psa_sst_def_asset_attr, attrs) \
        (((attrs & psa_sst_def_asset_attr) != 0) && \
         ((attrs & PSA_SST_ASSET_ATTR_VENDOR_DEFINED) == 0))

/*!
 * \def PSA_SST_ASSET_HAS_ATTR_VENDOR_DEFINED(attrs)
 *
 * \brief Macro to check if the asset attributes are vendor defined.
 *
 * \param[in] attrs  Asset attributes
 *
 * \return Returns 1 if the asset type is equal to PSA_SST_ASSET_VENDOR_DEFINED.
 *         Otherwise, it returns 0.
 */
#define PSA_SST_ASSET_HAS_ATTR_VENDOR_DEFINED(attrs) \
        ((attrs & PSA_SST_ASSET_ATTR_VENDOR_DEFINED) != 0)

#ifdef __cplusplus
}
#endif

#endif /* __PSA_SST_ASSET_MACROS_H__ */
