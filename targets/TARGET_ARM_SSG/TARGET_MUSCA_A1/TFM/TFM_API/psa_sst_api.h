/*
 * Copyright (c) 2017-2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/**
 * \file  psa_sst_api.h
 *
 * \brief Platform security architecture (PSA) API for secure storage partition
 */

#ifndef __PSA_SST_API__
#define __PSA_SST_API__

#include "psa_sst_asset_defs.h"
#include "psa_sst_asset_macros.h"

#include "tfm_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * PSA SST API version
 */
#define PSA_SST_API_VERSION_MAJOR 0
#define PSA_SST_API_VERSION_MINOR 2

/* The return value is shared with the TF-M partition status value.
 * The SST return codes shouldn't overlap with predefined TFM status values.
 */
#define PSA_SST_ERR_OFFSET (TFM_PARTITION_SPECIFIC_ERROR_MIN)

/*!
 * \enum psa_sst_err_t
 *
 * \brief Secure storage service error types
 *
 */
enum psa_sst_err_t {
    PSA_SST_ERR_SUCCESS = 0,
    PSA_SST_ERR_ASSET_NOT_PREPARED = PSA_SST_ERR_OFFSET,
    PSA_SST_ERR_ASSET_NOT_FOUND,
    PSA_SST_ERR_PARAM_ERROR,
    PSA_SST_ERR_STORAGE_SYSTEM_FULL,
    PSA_SST_ERR_SYSTEM_ERROR,
    /* Following entry is only to ensure the error code of int size */
    PSA_SST_ERR_FORCE_INT_SIZE = INT_MAX
};

/**
 * \brief Allocates space for the asset, referenced by asset UUID,
 *        without setting any data in the asset.
 *
 * \param[in] asset_uuid  Asset UUID
 * \param[in] token       Must be set to NULL, reserved for future use.
 *                        Pointer to the asset token to be used to generate
 *                        the asset key to encrypt and decrypt the asset
 *                        data. This is an optional parameter that has to
 *                        be NULL in case the token is not provied.
 * \param[in] token_size  Must be set to 0, reserved for future use.
 *                        Token size. In case the token is not provided
 *                        the token size has to be 0.
 *
 * \return Returns an PSA_SST_ERR_SUCCESS if asset is created correctly.
 *         Otherwise, error code as specified in \ref psa_sst_err_t
 */
enum psa_sst_err_t  psa_sst_create(uint32_t asset_uuid,
                                   const uint8_t *token,
                                   uint32_t token_size);

/**
 * \brief Gets asset's information referenced by asset UUID.
 *
 * \param[in]  asset_uuid  Asset UUID
 * \param[in]  token       Must be set to NULL, reserved for future use.
 *                         Pointer to the asset token to be used to generate
 *                         the asset key to encrypt and decrypt the asset
 *                         data. This is an optional parameter that has to
 *                         be NULL in case the token is not provied.
 * \param[in]  token_size  Must be set to 0, reserved for future use.
 *                         Token size. In case the token is not provided
 *                         the token size has to be 0.
 * \param[out] info        Pointer to store the asset's information
 *                         \ref psa_sst_asset_info_t
 *
 * \return Returns error code as specified in \ref psa_sst_err_t
 */
enum psa_sst_err_t psa_sst_get_info(uint32_t asset_uuid,
                                    const uint8_t *token,
                                    uint32_t token_size,
                                    struct psa_sst_asset_info_t *info);

/**
 * \brief Gets asset's attributes referenced by asset UUID.
 *
 * \param[in]  asset_uuid  Asset UUID
 * \param[in]  token       Must be set to NULL, reserved for future use.
 *                         Pointer to the asset token to be used to generate
 *                         the asset key to encrypt and decrypt the asset
 *                         data. This is an optional parameter that has to
 *                         be NULL in case the token is not provied.
 * \param[in]  token_size  Must be set to 0, reserved for future use.
 *                         Token size. In case the token is not provided
 *                         the token size has to be 0.
 * \param[out] attrs       Pointer to store the asset's attributes
 *                         \ref psa_sst_asset_attrs_t
 *
 * \return Returns error code as specified in \ref psa_sst_err_t
 */
enum psa_sst_err_t psa_sst_get_attributes(uint32_t asset_uuid,
                                          const uint8_t *token,
                                          uint32_t token_size,
                                          struct psa_sst_asset_attrs_t *attrs);

/**
 * \brief Sets asset's attributes referenced by asset UUID.
 *
 * \param[in] asset_uuid  Asset UUID
 * \param[in] token       Must be set to NULL, reserved for future use.
 *                        Pointer to the asset token to be used to generate
 *                        the asset key to encrypt and decrypt the asset
 *                        data. This is an optional parameter that has to
 *                        be NULL in case the token is not provied.
 * \param[in] token_size  Must be set to 0, reserved for future use.
 *                        Token size. In case the token is not provided
 *                        the token size has to be 0.
 * \param[in] attrs       Pointer to new the asset's attributes
 *                        \ref psa_sst_asset_attrs_t
 *
 * \return Returns error code as specified in \ref psa_sst_err_t
 */
enum psa_sst_err_t psa_sst_set_attributes(
                                     uint32_t asset_uuid,
                                     const uint8_t *token,
                                     uint32_t token_size,
                                     const struct psa_sst_asset_attrs_t *attrs);

/**
 * \brief Reads asset's data from asset referenced by asset UUID.
 *
 * \param[in]  asset_uuid  Asset UUID
 * \param[in]  token       Must be set to NULL, reserved for future use.
 *                         Pointer to the asset token to be used to generate
 *                         the asset key to encrypt and decrypt the asset
 *                         data. This is an optional parameter that has to
 *                         be NULL in case the token is not provied.
 * \param[in]  token_size  Must be set to 0, reserved for future use.
 *                         Token size. In case the token is not provided
 *                         the token size has to be 0.
 * \param[in]  size        Size of the data to read
 * \param[in]  offset      Offset within asset to start to read
 * \param[out] data        Pointer to data vector to store data
 *
 * \return Returns error code as specified in \ref psa_sst_err_t
 */
enum psa_sst_err_t psa_sst_read(uint32_t asset_uuid,
                                const uint8_t *token,
                                uint32_t token_size,
                                uint32_t size,
                                uint32_t offset,
                                uint8_t *data);

/**
 * \brief Reads asset's data on behalf of the given client ID if the
 *        client has reference permissions.
 *        This function is only accessible for secure partition with specific
 *        permissions. Hence, It must not be accessible from the Non-secure
 *        Processing Environment (NSPE).
 *
 * \param[in]  client_id   Client ID which is referenced in the read
 * \param[in]  asset_uuid  Asset UUID
 * \param[in]  token       Must be set to NULL, reserved for future use.
 *                         Pointer to the asset token to be used to generate
 *                         the asset key to encrypt and decrypt the asset
 *                         data. This is an optional parameter that has to
 *                         be NULL in case the token is not provied.
 * \param[in]  token_size  Must be set to 0, reserved for future use.
 *                         Token size. In case the token is not provided
 *                         the token size has to be 0.
 * \param[in]  size        Size of the data to read
 * \param[in]  offset      Offset within asset to start to read
 * \param[out] data        Pointer to data vector to store data
 *
 * \return Returns error code as specified in \ref psa_sst_err_t
 */
enum psa_sst_err_t psa_sst_reference_read(int32_t  client_id,
                                          uint32_t asset_uuid,
                                          const uint8_t *token,
                                          uint32_t token_size,
                                          uint32_t size,
                                          uint32_t offset,
                                          uint8_t *data);

/**
 * \brief Writes data into an asset referenced by asset UUID.
 *
 * \param[in] asset_uuid  Asset UUID
 * \param[in] token       Must be set to NULL, reserved for future use.
 *                        Pointer to the asset token to be used to generate
 *                        the asset key to encrypt and decrypt the asset
 *                        data. This is an optional parameter that has to
 *                        be NULL in case the token is not provied.
 * \param[in] token_size  Must be set to 0, reserved for future use.
 *                        Token size. In case the token is not provided
 *                        the token size has to be 0.
 * \param[in] size        Size of the data to start to write
 * \param[in] offset      Offset within asset to write the data
 * \param[in] data        Pointer to data vector which contains the data to
 *                        write
 *
 * \return Returns error code as specified in \ref psa_sst_err_t
 */
enum psa_sst_err_t psa_sst_write(uint32_t asset_uuid,
                                 const uint8_t *token,
                                 uint32_t token_size,
                                 uint32_t size,
                                 uint32_t offset,
                                 const uint8_t *data);

/**
 * \brief Deletes the asset referenced by the asset UUID.
 *
 * \param[in] asset_uuid  Asset UUID
 * \param[in] token       Must be set to NULL, reserved for future use.
 *                        Pointer to the asset token to be used to generate
 *                        the asset key to encrypt and decrypt the asset
 *                        data. This is an optional parameter that has to
 *                        be NULL in case the token is not provied.
 * \param[in] token_size  Must be set to 0, reserved for future use.
 *                        Token size. In case the token is not provided
 *                        the token size has to be 0.
 *
 * \return Returns error code as specified in \ref psa_sst_err_t
 */
enum psa_sst_err_t psa_sst_delete(uint32_t asset_uuid,
                                  const uint8_t *token,
                                  uint32_t token_size);

#ifdef __cplusplus
}
#endif

#endif /* __PSA_SST_API__ */
