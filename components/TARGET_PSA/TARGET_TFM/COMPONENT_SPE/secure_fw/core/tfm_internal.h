/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "secure_utilities.h"

#ifndef __TFM_INTERNAL_H__
#define __TFM_INTERNAL_H__

/*
 * This function pointer is meant to only hold non secure function pointers.
 * It will be turned into a non-secure one (LSB cleared) before being called
 * whatever happens anyway (unless cast to another function pointer type).
 * Registers will be cleared before branching so that no information leaks
 * from secure to non-secure world.
 */
typedef void __attribute__((cmse_nonsecure_call)) (*nsfptr_t) (void);

extern nsfptr_t ns_entry;

/**
 * \brief Signal that secure partition initialisation is finished
 */
void tfm_secure_api_init_done(void);

/**
 * \brief Jumps to non-secure code.
 */
void jump_to_ns_code(void);

/**
 * \brief Called if veneer is running in thread mode
 */
uint32_t tfm_core_partition_request_svc_handler(
        const uint32_t *svc_args, uint32_t lr);

/**
 * \brief Called when secure service returns
 */
uint32_t tfm_core_partition_return_handler(uint32_t lr);

/**
 * \brief Called by secure service to check if client is secure
 */
void tfm_core_validate_secure_caller_handler(const uint32_t svc_args[]);

/**
 * \brief Stores caller's client id in state context
 */
void tfm_core_get_caller_client_id_handler(const uint32_t svc_args[]);

/**
 * \brief Checks if a secure service's access to a memory location is permitted
 */
void tfm_core_memory_permission_check_handler(const uint32_t svc_args[]);

/**
 * \brief Handle an SPM request by a secure service
 */
void tfm_core_spm_request_handler(const struct tfm_exc_stack_t *svc_ctx);

/**
 * \brief Check whether a buffer is ok for writing to by the privileged API
 *        function.
 *
 * This function checks whether the caller partition owns the buffer, can write
 * to it, and the buffer has proper alignment.
 *
 * \param[in] partition_idx     Partition index
 * \param[in] start_addr        The start address of the buffer
 * \param[in] len               The length of the buffer
 * \param[in] alignment         The expected alignment (in bits)
 *
 * \return 1 if the check passes, 0 otherwise.
 *
 * \note For a 0 long buffer the check fails.
 */
int32_t tfm_core_check_buffer_access(uint32_t  partition_idx,
                                     void     *start_addr,
                                     size_t    len,
                                     uint32_t  alignment);

/**
 * \brief Retrieve secure partition related data from shared memory area, which
 *        stores shared data between bootloader and runtime firmware.
 *
 * \param[in] args  Pointer to stack frame, which carries input parameters.
 */
void tfm_core_get_boot_data_handler(uint32_t args[]);

/**
 * \brief Validate the content of shared memory area, which stores the shared
 *        data between bootloader and runtime firmware.
 */
void tfm_core_validate_boot_data(void);

#endif /* __TFM_INTERNAL_H__ */
