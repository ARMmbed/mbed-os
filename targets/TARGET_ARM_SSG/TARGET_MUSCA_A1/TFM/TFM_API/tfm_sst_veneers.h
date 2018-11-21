/*
 * Copyright (c) 2017-2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_SST_VENEERS_H__
#define __TFM_SST_VENEERS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tfm_sst_defs.h"

/**
 * \brief Allocates space for the asset, referenced by asset UUID,
 *        without setting any data in the asset.
 *
 * \param[in] s_token     Pointer to the asset's token \ref tfm_sst_token_t
 * \param[in] asset_uuid  Asset UUID \ref tfm_sst_asset_id_t
 *
 * \return Returns PSA_SST_ERR_SUCCESS if the asset has been created correctly.
 *         If SST area is not prepared, it returns
 *         PSA_SST_ERR_ASSET_NOT_PREPARED. If SST area is full, it returns
 *         PSA_SST_ERR_STORAGE_SYSTEM_FULL. If application id doesn't have the
 *         write rights, it returns PSA_SST_ERR_PERMS_NOT_ALLOWED.
 */
enum psa_sst_err_t tfm_sst_veneer_create(uint32_t asset_uuid,
                                         const struct tfm_sst_token_t *s_token);

/**
 * \brief Gets asset's information referenced by asset UUID.
 *
 * \param[in]  asset_uuid  Asset UUID
 * \param[in]  s_token     Pointer to the asset's token \ref tfm_sst_token_t
 * \param[out] info        Pointer to store the asset's information
 *                         \ref psa_sst_asset_info_t
 *
 * \return Returns error code as specified in \ref psa_sst_err_t
 */
enum psa_sst_err_t tfm_sst_veneer_get_info(uint32_t asset_uuid,
                                          const struct tfm_sst_token_t *s_token,
                                          struct psa_sst_asset_info_t *info);

/**
 * \brief Gets asset's attributes referenced by asset UUID.
 *
 * \param[in]  asset_uuid  Asset UUID
 * \param[in]  s_token     Pointer to the asset's token \ref tfm_sst_token_t
 * \param[out] attrs       Pointer to store the asset's attributes
 *                         \ref tfm_sst_asset_attrs_t
 *
 * \return Returns error code as specified in \ref psa_sst_err_t
 */
enum psa_sst_err_t tfm_sst_veneer_get_attributes(uint32_t asset_uuid,
                                          const struct tfm_sst_token_t *s_token,
                                          struct psa_sst_asset_attrs_t *attrs);

/**
 * \brief Sets asset's attributes referenced by asset UUID.
 *
 * \param[in] asset_uuid  Asset UUID
 * \param[in] s_token     Pointer to the asset's token \ref tfm_sst_token_t
 * \param[in] attrs       Pointer to new the asset's attributes
 *                        \ref psa_sst_asset_attrs_t
 *
 * \return Returns error code as specified in \ref psa_sst_err_t
 */
enum psa_sst_err_t tfm_sst_veneer_set_attributes(uint32_t asset_uuid,
                                     const struct tfm_sst_token_t *s_token,
                                     const struct psa_sst_asset_attrs_t *attrs);

/**
 * \brief Reads asset's data from asset referenced by asset UUID.
 *

 * \param[in]     client_id   Client ID which calls the service.
 *                            In case, the caller is a secure partition, this
 *                            parameter can be a non-secure or secure client ID
 *                            if the read is in behalf of that client.
 * \param[in]     asset_uuid  Asset UUID
 * \param[in]     s_token     Pointer to the asset's token
 *                            \ref tfm_sst_token_t
 * \param[in/out] data        Pointer to data vector \ref tfm_sst_buf_t to
 *                            store data, size and offset
 *
 * \return Returns the number of bytes written or a castable \ref psa_sst_err_t
 *         value
 */
enum psa_sst_err_t tfm_sst_veneer_read(int32_t client_id,
                                       uint32_t asset_uuid,
                                       const struct tfm_sst_token_t *s_token,
                                       struct tfm_sst_buf_t *data);

/**
 * \brief Writes data into an asset referenced by asset UUID.
 *
 * \param[in] asset_uuid  Asset UUID
 * \param[in] s_token     Pointer to the asset's token \ref tfm_sst_token_t
 * \param[in] data        Pointer to data vector \ref tfm_sst_buf_t which
 *                        contains the data to write
 *
 * \return Returns the number of bytes written or a castable \ref psa_sst_err_t
 *         value
 */
enum psa_sst_err_t tfm_sst_veneer_write(uint32_t asset_uuid,
                                        const struct tfm_sst_token_t *s_token,
                                        struct tfm_sst_buf_t *data);

/**
 * \brief Deletes the asset referenced by the asset UUID.
 *
 * \param[in] asset_uuid  Asset UUID
 * \param[in] s_token     Pointer to the asset's token \ref tfm_sst_token_t
 *
 * \return Returns PSA_SST_ERR_PERMS_NOT_ALLOWED if the asset can't be deleted
 *         to by this app ID. Returns PSA_SST_ERR_ASSET_REF_INVALID, if asset
 *         no longer exists. Otherwise, PSA_SST_ERR_SUCCESS.
 */
enum psa_sst_err_t tfm_sst_veneer_delete(uint32_t asset_uuid,
                                         const struct tfm_sst_token_t *s_token);
#ifdef __cplusplus
}
#endif

#endif /* __TFM_SST_VENEERS_H__ */
