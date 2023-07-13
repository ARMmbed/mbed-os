/*
 * Copyright (c) 2022-2023 Macronix International Co. LTD. All rights reserved.
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
#ifndef _JEDEC_SECURITY_HAL_H_
#define _JEDEC_SECURITY_HAL_H_

#include <stdint.h>
#include <stddef.h>
#include "vendor_security_impl.h"
#include "crypto_wrapper.h"

#ifdef __cplusplus
    extern "C" {
#endif

typedef struct {
    bool is_initialized;                      /*!< Secure Flash state:Initialized or uninitialized */
    void *vendor_ctx;                         /*!< Vendor context */
    vendor_security_op_t *vendor_security_op; /*!< Vendor specific security operations */
    crypto_wrapper_t *crypto_wrapper;         /*!< Crypto wrapper functions */
    uint8_t packet[PACKET_MAX_LEN];           /*!< Buffer holding command packet */
    uint32_t packet_len;                      /*!< Command packet length */
    uint8_t verify;                           /*!< Verification indication of responses from secure Flash */
    jqueue_t q;                               /*!< Response queue */
} jedec_ctx_t;

/**
 * \brief Bind JEDEC context with vendor specific implementation.
 *
 * \param[in] vendor      Vendor specific implementation
 * \param[in] vendor_ctx  Vendor specific context
 *
 */
jedec_error_t jedec_set_vendor(vendor_security_op_t *vendor,
                               crypto_wrapper_t *crypto_wrapper,
                               void *vendor_ctx);

/**
 * \brief Set verify flag of packets returned in the queue.
 *
 * \param[in] verify      Verify indication
 *
 */
jedec_error_t jedec_set_verify(uint8_t verify);

/**
 * \brief Move the secure Flash's status to initialized status.
 *
 * \param[in] auth_key_id  Input authenticatoin key id for secure initialization
 *
 * \return JEDEC_ERROR_NONE - success
 *         JEDEC_ERROR_INV_ARGS - invalid input arguments
 *         JEDEC_ERROR_AUTH_FAIL - authentication failure
 *         JEDEC_ERROR_COMM_FAIL - communication error
 */
jedec_error_t jedec_secure_init(uint32_t auth_key_id);
/**
 * \brief Security uninitialization of secure Flash.
 *
 * \param[in] auth_key_id  Input authentication key id for secure uninitialization
 *
 * \return JEDEC_ERROR_NONE - success
 *         JEDEC_ERROR_SESSION - no session has been established
 *         JEDEC_ERROR_INV_ARGS - invalid input arguments
 *         JEDEC_ERROR_AUTH_FAIL - authentication failure
 *         JEDEC_ERROR_COMM_FAIL - communication error
 */
jedec_error_t jedec_secure_uninit(uint32_t auth_key_id);
/**
 * \brief Establish a cryptographic session with secure Flash.
 *
 * \param[in] root_key_id      Preshared root key ID
 * \param[in] verify           Indicate whether subsequent flash responses
 *                             within session should be verified
 * \param[out] session_key_id  Generated session key ID
 *
 * \return JEDEC_ERROR_NONE - success
 *         JEDEC_ERROR_INV_ARGS - invalid input arguments
 *         JEDEC_ERROR_AUTH_FAIL - authentication failure
 *         JEDEC_ERROR_COMM_FAIL - communication error
 *         JEDEC_ERROR_MAX_SESSIONS_REACHED - no more sessions may be created
 */
jedec_error_t jedec_create_session(uint32_t root_key_id, bool verify, uint32_t *session_key_id);
/**
 * \brief Terminate a cryptographic session with secure Flash.
 *
 * \param[in] session_key_id  Corresponding session key ID
 *
 * \return JEDEC_ERROR_NONE - success
 *         JEDEC_ERROR_INV_ARGS - invalid input arguments
 *         JEDEC_ERROR_COMM_FAIL - communication error
 */
jedec_error_t jedec_terminate_session(uint32_t session_key_id);
/**
 * \brief Program data to secure Flash.
 *
 * \param[in]  secureflash       Secure Flash to access
 * \param[in]  addr              Target address in secure Flash
 * \param[in]  data              Data to be programmed to secure Flash
 * \param[in]  len               Number of bytes requested to be programmed
 * \param[in]  session_key_id    Session key ID
 * \param[out] bytes_programmed  Number of bytes that have been programmed
 *
 * \return JEDEC_ERROR_NONE - success
 *         JEDEC_ERROR_SESSION - no session has been established
 *         JEDEC_ERROR_INV_ARGS - invalid input arguments
 *         JEDEC_ERROR_AUTH_FAIL - authentication failure
 *         JEDEC_ERROR_COMM_FAIL - communication error
 *         JEDEC_ERROR_INVALID_ADDR -address is outside addressable flash address space
 */
jedec_error_t jedec_secure_program(uint32_t addr, const uint8_t *data, uint32_t len,
                                   uint32_t session_key_id, uint32_t *bytes_programmed);
/**
 * \brief Erase a sector/block specified by a base address of secure Flash.
 *
 * \param[in] secureflash     Secure Flash to access
 * \param[in] addr            Address from which to start erase
 * \param[in] len             Number of bytes requested to be programmed
 * \param[in] session_key_id  Session key ID
 *
 * \return JEDEC_ERROR_NONE - success
 *         JEDEC_ERROR_SESSION - no session has been established
 *         JEDEC_ERROR_INV_ARGS - invalid input arguments
 *         JEDEC_ERROR_AUTH_FAIL - authentication failure
 *         JEDEC_ERROR_COMM_FAIL - communication error
 */
jedec_error_t jedec_secure_erase(uint32_t addr, uint32_t len, uint32_t session_key_id);
/**
 * \brief Read protected data from secure Flash.
 *
 * \param[in]  secureflash     Secure Flash to access
 * \param[in]  addr            Target (starting) address in secure Flash
 * \param[in]  data            Data read from secure Flash
 * \param[in]  len             Number of bytes requested to be read
 * \param[in]  session_key_id  Session key ID
 * \param[out] bytes_read      Number of bytes that have been read
 *
 * \return JEDEC_ERROR_NONE - success
 *         JEDEC_ERROR_SESSION - no session has been established
 *         JEDEC_ERROR_INV_ARGS - invalid input arguments
 *         JEDEC_ERROR_AUTH_FAIL - authentication failure
 *         JEDEC_ERROR_COMM_FAIL - communication error
 *         JEDEC_ERROR_INVALID_ADDR -address is outside addressable flash address space
 *         JEDEC_ERROR_CMD_NOT_SUPPORTED_ON_DEVICE -command not supported by vendor's flash
 */
jedec_error_t jedec_secure_read(uint32_t addr, uint8_t *data, uint32_t len,
                                uint32_t session_key_id, uint32_t *bytes_read);
/**
 * \brief Copy data from secure Flash one location to another.
 *
 * \param[in] secureflash     Secure Flash to access
 * \param[in] src_addr        Source address in secure Flash
 * \param[in] dst_addr        Destination address in secure Flash
 * \param[in] len             Number of bytes to copy
 * \param[in] session_key_id  Session key ID
 *
 * \return JEDEC_ERROR_NONE - success
 *         JEDEC_ERROR_SESSION - no session has been established
 *         JEDEC_ERROR_INV_ARGS - invalid input arguments
 *         JEDEC_ERROR_AUTH_FAIL - authentication failure
 *         JEDEC_ERROR_COMM_FAIL - communication error
 *         JEDEC_ERROR_INVALID_ADDR -address is outside addressable flash address space
 *         JEDEC_ERROR_CMD_NOT_SUPPORTED_ON_DEVICE -command not supported by vendor's flash
 */
jedec_error_t jedec_secure_copy(uint32_t src_addr, uint32_t dst_addr, uint32_t len,
                                uint32_t session_key_id);
/**
 * \brief Obtain current secure configuration parameters of secure Flash.
 *
 * \param[in]     secureflash     Secure Flash to access
 * \param[in/out] region_descr_p  Pointer to secure region configuration linked list.
 * \param[in]     session_key_id  Session key ID
 * \param[in]     region_idx      Region index
 *
 * \return JEDEC_ERROR_NONE - success
 *         JEDEC_ERROR_SESSION - no session has been established
 *         JEDEC_ERROR_INV_ARGS - invalid input arguments
 *         JEDEC_ERROR_AUTH_FAIL - authentication failure
 *         JEDEC_ERROR_COMM_FAIL - communication error
 */
jedec_error_t jedec_secure_get_regions_info(region_ll_node_t *region_descr_p,
                                            uint32_t session_key_id, int8_t region_idx);
/**
 * \brief Configure secure Flash regions.
 *
 * \param[in] secureflash  Secure Flash to access
 * \param[in] head         Pointer to the header of secure regions'
 *                         configuration parameters link list
 *
 * \return JEDEC_ERROR_NONE - success
 *         JEDEC_ERROR_SESSION - no session has been established
 *         JEDEC_ERROR_INV_ARGS - invalid input arguments
 *         JEDEC_ERROR_AUTH_FAIL - authentication failure
 *         JEDEC_ERROR_COMM_FAIL - communication error
 *         JEDEC_ERROR_CMD_NOT_SUPPORTED_ON_DEVICE - command not supported by vendor's flash
 */
jedec_error_t jedec_secure_manage_region(region_ll_node_t *head);

#ifdef __cplusplus
}
#endif

#endif /* _JEDEC_SECURITY_HAL_H_ */
