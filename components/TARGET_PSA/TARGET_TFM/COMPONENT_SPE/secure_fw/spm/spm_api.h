/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __SPM_API_H__
#define __SPM_API_H__

/* This file contains the apis exported by the SPM to tfm core */
#include "tfm_api.h"
#include "spm_partition_defs.h"
#include "secure_fw/core/tfm_secure_api.h"

#define SPM_INVALID_PARTITION_IDX     (~0U)

/* Privileged definitions for partition thread mode */
#define TFM_PARTITION_PRIVILEGED_MODE   1
#define TFM_PARTITION_UNPRIVILEGED_MODE 0

enum spm_err_t {
    SPM_ERR_OK = 0,
    SPM_ERR_PARTITION_DB_NOT_INIT,
    SPM_ERR_PARTITION_ALREADY_ACTIVE,
    SPM_ERR_PARTITION_NOT_AVAILABLE,
    SPM_ERR_INVALID_PARAMETER,
    SPM_ERR_INVALID_CONFIG,
};

enum spm_part_state_t {
    SPM_PARTITION_STATE_UNINIT = 0,
    SPM_PARTITION_STATE_IDLE,
    SPM_PARTITION_STATE_RUNNING,
    SPM_PARTITION_STATE_SUSPENDED,
    SPM_PARTITION_STATE_BLOCKED,
    SPM_PARTITION_STATE_CLOSED
};

enum spm_part_flag_mask_t {
    SPM_PART_FLAG_APP_ROT = 0x01,
    SPM_PART_FLAG_PSA_ROT = 0x02,
    SPM_PART_FLAG_IPC     = 0x04
};

/**
 * \brief Holds the iovec parameters that are passed to a service
 *
 * \note The size of the structure is (and have to be) multiple of 8 bytes
 */
struct iovec_args_t {
    psa_invec in_vec[PSA_MAX_IOVEC];   /*!< Array of psa_invec objects */
    size_t in_len;                     /*!< Number psa_invec objects in in_vec
                                        */
    psa_outvec out_vec[PSA_MAX_IOVEC]; /*!< Array of psa_outvec objects */
    size_t out_len;                    /*!< Number psa_outvec objects in out_vec
                                        */
};

/**
 * \brief Runtime context information of a partition
 */
struct spm_partition_runtime_data_t {
    uint32_t partition_state;
    uint32_t caller_partition_idx;
    int32_t caller_client_id;
    uint32_t share;
    uint32_t stack_ptr;
    uint32_t lr;
    int32_t iovec_api;               /*!< Whether the function in the partition
                                      * had been called using the iovec API.
                                      * FIXME: Remove the field once this is the
                                      * only option
                                      */
    struct iovec_args_t iovec_args;
    psa_outvec *orig_outvec;
};


/**
 * \brief Returns the index of the partition with the given partition ID.
 *
 * \param[in] partition_id     Partition id
 *
 * \return the partition idx if partition_id is valid,
 *         \ref SPM_INVALID_PARTITION_IDX othervise
 */
uint32_t get_partition_idx(uint32_t partition_id);

#if (TFM_LVL != 1) || defined(TFM_PSA_API)
/**
 * \brief Get bottom of stack region for a partition
 *
 * \param[in] partition_idx     Partition index
 *
 * \return Stack region bottom value
 *
 * \note This function doesn't check if partition_idx is valid.
 */
uint32_t tfm_spm_partition_get_stack_bottom(uint32_t partition_idx);

/**
 * \brief Get top of stack region for a partition
 *
 * \param[in] partition_idx     Partition index
 *
 * \return Stack region top value
 *
 * \note This function doesn't check if partition_idx is valid.
 */
uint32_t tfm_spm_partition_get_stack_top(uint32_t partition_idx);
#endif

#if (TFM_LVL != 1) && !defined(TFM_PSA_API)
/**
 * \brief Configure isolated sandbox for a partition
 *
 * \param[in] partition_idx     Partition index
 *
 * \return Error code \ref spm_err_t
 *
 * \note This function doesn't check if partition_idx is valid.
 */
enum spm_err_t tfm_spm_partition_sandbox_config(uint32_t partition_idx);

/**
 * \brief Deconfigure sandbox for a partition
 *
 * \param[in] partition_idx     Partition index
 *
 * \return Error code \ref spm_err_t
 *
 * \note This function doesn't check if partition_idx is valid.
 */
enum spm_err_t tfm_spm_partition_sandbox_deconfig(uint32_t partition_idx);

/**
 * \brief Get the start of the zero-initialised region for a partition
 *
 * \param[in] partition_idx     Partition idx
 *
 * \return Start of the zero-initialised region
 *
 * \note This function doesn't check if partition_idx is valid.
 */
uint32_t tfm_spm_partition_get_zi_start(uint32_t partition_idx);

/**
 * \brief Get the limit of the zero-initialised region for a partition
 *
 * \param[in] partition_idx     Partition idx
 *
 * \return Limit of the zero-initialised region
 *
 * \note This function doesn't check if partition_idx is valid.
 * \note The address returned is not part of the region.
 */
uint32_t tfm_spm_partition_get_zi_limit(uint32_t partition_idx);

/**
 * \brief Get the start of the read-write region for a partition
 *
 * \param[in] partition_idx     Partition idx
 *
 * \return Start of the read-write region
 *
 * \note This function doesn't check if partition_idx is valid.
 */
uint32_t tfm_spm_partition_get_rw_start(uint32_t partition_idx);

/**
 * \brief Get the limit of the read-write region for a partition
 *
 * \param[in] partition_idx     Partition idx
 *
 * \return Limit of the read-write region
 *
 * \note This function doesn't check if partition_idx is valid.
 * \note The address returned is not part of the region.
 */
uint32_t tfm_spm_partition_get_rw_limit(uint32_t partition_idx);

/**
 * \brief Save stack pointer for partition in database
 *
 * \param[in] partition_idx  Partition index
 * \param[in] stack_ptr      Stack pointer to be stored
 *
 * \note This function doesn't check if partition_idx is valid.
 */
void tfm_spm_partition_set_stack(uint32_t partition_idx, uint32_t stack_ptr);
#endif

/**
 * \brief Get the id of the partition for its index from the db
 *
 * \param[in] partition_idx     Partition index
 *
 * \return Partition ID for that partition
 *
 * \note This function doesn't check if partition_idx is valid.
 */
uint32_t tfm_spm_partition_get_partition_id(uint32_t partition_idx);

/**
 * \brief Get the flags associated with a partition
 *
 * \param[in] partition_idx     Partition index
 *
 * \return Flags associated with the partition
 *
 * \note This function doesn't check if partition_idx is valid.
 */
uint32_t tfm_spm_partition_get_flags(uint32_t partition_idx);

#ifndef TFM_PSA_API
/**
 * \brief Get the current runtime data of a partition
 *
 * \param[in] partition_idx     Partition index
 *
 * \return The runtime data of the specified partition
 *
 * \note This function doesn't check if partition_idx is valid.
 */
const struct spm_partition_runtime_data_t *
             tfm_spm_partition_get_runtime_data(uint32_t partition_idx);

/**
 * \brief Returns the index of the partition that has running state
 *
 * \return The index of the partition with the running state, if there is any
 *         set. 0 otherwise.
 */
uint32_t tfm_spm_partition_get_running_partition_idx(void);

/**
 * \brief Save stack pointer and link register for partition in database
 *
 * \param[in] partition_idx  Partition index
 * \param[in] stack_ptr      Stack pointer to be stored
 * \param[in] lr             Link register to be stored
 *
 * \note This function doesn't check if partition_idx is valid.
 */
void tfm_spm_partition_store_context(uint32_t partition_idx,
        uint32_t stack_ptr, uint32_t lr);

/**
 * \brief Set the current state of a partition
 *
 * \param[in] partition_idx  Partition index
 * \param[in] state          The state to be set
 *
 * \note This function doesn't check if partition_idx is valid.
 * \note The state has to have the value set of \ref spm_part_state_t.
 */
void tfm_spm_partition_set_state(uint32_t partition_idx, uint32_t state);

/**
 * \brief Set the caller partition index for a given partition
 *
 * \param[in] partition_idx        Partition index
 * \param[in] caller_partition_idx The index of the caller partition
 *
 * \note This function doesn't check if any of the partition_idxs are valid.
 */
void tfm_spm_partition_set_caller_partition_idx(uint32_t partition_idx,
                                                uint32_t caller_partition_idx);

/**
* \brief Set the caller client ID for a given partition
*
* \param[in] partition_idx        Partition index
* \param[in] caller_client_id     The ID of the calling client
*
* \note This function doesn't check if any of the partition_idxs are valid.
*/
void tfm_spm_partition_set_caller_client_id(uint32_t partition_idx,
                                            int32_t caller_client_id);

/**
 * \brief Set the buffer share region of the partition
 *
 * \param[in] partition_idx  Partition index
 * \param[in] share          The buffer share region to be set
 *
 * \return Error code \ref spm_err_t
 *
 * \note This function doesn't check if partition_idx is valid.
 * \note share has to have the value set of \ref tfm_buffer_share_region_e
 */
enum spm_err_t tfm_spm_partition_set_share(uint32_t partition_idx,
                                           uint32_t share);

/**
 * \brief Set the iovec parameters for the partition
 *
 * \param[in] partition_idx  Partition index
 * \param[in] args           The arguments of the secure function
 *
 * args is expected to be of type int32_t[4] where:
 *   args[0] is in_vec
 *   args[1] is in_len
 *   args[2] is out_vec
 *   args[3] is out_len
 *
 * \return Error code \ref spm_err_t
 *
 * \note This function doesn't check if partition_idx is valid.
 * \note This function assumes that the iovecs that are passed in args are
 *       valid, and does no sanity check on them at all.
 */
enum spm_err_t tfm_spm_partition_set_iovec(uint32_t partition_idx,
                                           const int32_t *args);

/**
 * \brief Execute partition init function
 *
 * \return Error code \ref spm_err_t
 */
enum spm_err_t tfm_spm_partition_init(void);

/**
 * \brief Clears the context info from the database for a partition.
 *
 * \param[in] partition_idx     Partition index
 *
 * \note This function doesn't check if partition_idx is valid.
 */
void tfm_spm_partition_cleanup_context(uint32_t partition_idx);
#endif /* !defined(TFM_PSA_API) */

/**
 * \brief Initialize partition database
 *
 * \return Error code \ref spm_err_t
 */
enum spm_err_t tfm_spm_db_init(void);

/**
 * \brief Change the privilege mode for partition thread mode.
 *
 * \param[in] privileged        Privileged mode,
 *                                \ref TFM_PARTITION_PRIVILEGED_MODE
 *                                and \ref TFM_PARTITION_UNPRIVILEGED_MODE
 *
 * \note Barrier instructions are not called by this function, and if
 *       it is called in thread mode, it might be necessary to call
 *       them after this function returns (just like it is done in
 *       jump_to_ns_code()).
 */
void tfm_spm_partition_change_privilege(uint32_t privileged);

#endif /*__SPM_API_H__ */
