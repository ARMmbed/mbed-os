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

#ifndef __MBED_PSA_DEFS_H__
#define __MBED_PSA_DEFS_H__

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

#if !defined(INT32_MIN)
#define INT32_MIN   (-0x7fffffff - 1)
#endif

#define PSA_FRAMEWORK_VERSION    (0x0009) /**< Version of the PSA Framework API. */
#define PSA_VERSION_NONE         (0L)     /**< Identifier for an unimplemented RoT Service. */

#define PSA_NSPE_IDENTIFIER (-1L)  /**< "Partition" identifier of the NSPE.*/

#define PSA_NULL_HANDLE ((psa_handle_t)0)   /**< Denotes an invalid handle.*/

#define PSA_MAX_IOVEC (4UL) /**< Maximum number of psa_invec_t and psa_outvec_t structures allowed for psa_call().*/

#define PSA_POLL  (0x00000000UL) /**< Returns immediately even if none of the requested signals is asserted.*/
#define PSA_BLOCK (0x80000000UL) /**< Block the caller until one of the requested signals is asserted.*/

#define PSA_MINOR_VERSION_POLICY_RELAXED (0UL) /**< Don't perform minor version check during psa_connect().*/
#define PSA_MINOR_VERSION_POLICY_STRICT (1UL)  /**< Force minor version check during psa_connect().*/

#define PSA_DOORBELL (0x00000008UL) /**< Mask for PSA_DOORBELL signal.*/

#define PSA_SUCCESS              (0L) /**< A general result code for calls to psa_call()  indicating success.*/
#define PSA_CONNECTION_ACCEPTED  (0L) /**< The result code for calls to psa_connect() indicating the acceptance of a new connection request.*/
#define PSA_IPC_CONNECT          (1)  /**< The IPC message type that indicates a new connection.*/
#define PSA_IPC_CALL             (2)  /**< The IPC message type that indicates a client request.*/
#define PSA_IPC_DISCONNECT       (3)  /**< The IPC message type that indicates the end of a connection.*/
#define PSA_IPC_VERSION          (4)  /**< The IPC message type that indicates a client query for a specific sid.*/

/* Error codes */
#define PSA_DROP_CONNECTION     (INT32_MIN)       /**< The result code in a call to psa_reply() to indicate a non-recoverable error in the client.*/
#define PSA_CONNECTION_REFUSED  (INT32_MIN + 1)    /**< The return value from psa_connect() if the RoT Service or SPM was unable to establish a connection.*/

#define PSA_UNUSED(var) ((void)(var))

/* -------------------------------------- Typedefs ----------------------------------- */

typedef uint32_t psa_signal_t;
typedef int32_t psa_error_t;
typedef int32_t psa_handle_t;
typedef psa_error_t error_t;

/* -------------------------------------- Structs ------------------------------------ */

/**
 * Structure containing the PSA IPC message sent from a client partition to a Root of Trust Service.
 */
typedef struct psa_msg {
    uint32_t type;                   /**< The message type.*/
    psa_handle_t handle;             /**< Handle for the internal message structure.*/
    void *rhandle;                   /**< Reverse handle.*/
    size_t in_size[PSA_MAX_IOVEC];   /**< Array of sizes in bytes of the message payloads.*/
    size_t out_size[PSA_MAX_IOVEC];  /**< Array of sizes in bytes of the response buffers.*/
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

#endif  /* __MBED_PSA_DEFS_H__ */
