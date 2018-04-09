/* Copyright (c) 2017 ARM Limited
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

#ifndef __UVISOR_MBED_PSA_DEFS_H__
#define __UVISOR_MBED_PSA_DEFS_H__

/** @addtogroup SPM
 * @{
 */

/* -------------------------------------- Includes ----------------------------------- */

#include <stdint.h>
#include <stdlib.h>

/* --------------------------------- extern "C" wrapper ------------------------------ */

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------ Definitions ---------------------------------- */

#if !defined(UINT32_MAX)
#define UINT32_MAX ((uint32_t)-1)
#endif

#define PSA_NSPE_IDENTIFIER (-1L)  /**< "Partition" identifier of the NSPE.*/

#define PSA_NULL_HANDLE ((psa_handle_t)0)   /**< Denotes an invalid handle.*/

#define PSA_MAX_INVEC_LEN (3UL) /**< Maximum number of psa_invec_t structures allowed for psa_call().*/
#define PSA_MAX_OUTVEC_LEN (3UL) /**< Maximum number of psa_outvec_t structures allowed for psa_write().*/

#define PSA_WAIT_POLL (0UL) /**< Returns immediately even if none of the requested signals is asserted.*/
#define PSA_WAIT_BLOCK UINT32_MAX /**< Block the caller until one of the requested signals is asserted.*/

#define PSA_MINOR_VERSION_POLICY_RELAXED (0UL) /**< Don't perform minor version check during psa_connect().*/
#define PSA_MINOR_VERSION_POLICY_STRICT (1UL) /**< Force minor version check during psa_connect().*/

#define PSA_SEC_FUNC_SEM_MAX_COUNT (1UL) /**< Maximum number of available tokens for a Secure Function semaphore.*/
#define PSA_SEC_FUNC_SEM_INITIAL_COUNT (0UL) /**< Initial number of available tokens for a Secure Function semaphore.*/

#define PSA_MMIO_PERM_READ_ONLY                     0x000000001
#define PSA_MMIO_PERM_READ_WRITE                    0x000000003

#define PSA_SUCCESS                    (0L)

/* Error codes */
#define PSA_INVALID_PARAMETERS        (-1L)
#define PSA_INVALID_MEM               (-2L)
#define PSA_CONNECTION_REFUSED_PERM   (-3L)
#define PSA_CONNECTION_REFUSED_BUSY   (-4L)
#define PSA_GENERIC_ERROR             (-5L)
#define PSA_MEM_ALLOC_FAILED          (-6L)
#define PSA_MSG_UNDELIVERED           (-7L)

#define PSA_UNUSED(var) ((void)(var))

/* -------------------------------------- Enums -------------------------------------- */

/**
 * Enumeration for the available PSA IPC message types
 */
typedef enum spm_msg_type {
    PSA_IPC_MSG_TYPE_INVALID = 0,                       /**< Invalid message type */
    PSA_IPC_MSG_TYPE_CONNECT = 1,                       /**< Sent by psa_connect() */
    PSA_IPC_MSG_TYPE_CALL = 2,                          /**< Sent by psa_call() */
    PSA_IPC_MSG_TYPE_DISCONNECT = 3,                    /**< Sent by psa_close() */
    PSA_IPC_MSG_TYPE_MAX = PSA_IPC_MSG_TYPE_DISCONNECT
} SpmMsgType;

/* -------------------------------------- Typedefs ----------------------------------- */

typedef uint32_t psa_signal_t;
typedef int32_t psa_error_t;
typedef int32_t psa_handle_t;
typedef psa_error_t error_t;

/* -------------------------------------- Structs ------------------------------------ */

/**
 * Structure containing the PSA IPC message sent from a client partition to a secure function.
 */
typedef struct psa_msg {
    uint32_t type;                       /**< The message type, one of ::spm_msg_type.*/
    psa_handle_t handle;                 /**< Handle for the internal message structure.*/
    void *rhandle;                       /**< Reverse handle.*/
    size_t in_size[PSA_MAX_INVEC_LEN];   /**< Array of sizes in bytes of the message payloads.*/
    size_t out_size[PSA_MAX_OUTVEC_LEN]; /**< Array of sizes in bytes of the response buffers.*/
} psa_msg_t;

/**
 * Structure which describes a scatter-gather input buffer.
 */
typedef struct psa_invec {
    const void *base;   /**< Starting address of the buffer.*/
    size_t len;         /**< Length in bytes of the buffer.*/
} psa_invec_t;

/**
 * Structure which describes a scatter-gather output buffer.
 */
typedef struct psa_outvec {
    void *base;      /**< Starting address of the buffer.*/
    size_t len;      /**< Length in bytes of the buffer.*/
} psa_outvec_t;

#ifdef __cplusplus
}
#endif


/** @}*/ // end of SPM group

#endif  /* __UVISOR_MBED_PSA_DEFS_H__ */
