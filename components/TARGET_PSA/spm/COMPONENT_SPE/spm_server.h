/* Copyright (c) 2017-2018 ARM Limited
 *
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

#ifndef __MBED_SPM_SERVER_H__
#define __MBED_SPM_SERVER_H__

/** @addtogroup SPM
 * The SPM (Secure Partition Manager) is responsible for isolating software in partitions,@n
 * managing the execution of software within partitions, and providing IPC between partitions.
 * @{
 */

/* -------------------------------------- Includes ----------------------------------- */

#include "psa_defs.h"

/* --------------------------------- extern "C" wrapper ------------------------------ */

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup RoT-Service-API
 *  The C interface for a Root of Trust Service in a Partition.
 * @{
 */

/**
 * Return the signals that have been asserted.@n
 *
 * @param[in] timeout timeout value:@n
 *            @a PSA_BLOCK block the caller until any signal is asserted.@n
 *            @a PSA_POLL Returns immediately.
 * @return 32-bit value of asserted signals.
 */
uint32_t psa_wait_any(uint32_t timeout);

/**
 * Return interrupt/doorbell signals that have been asserted based on the bitmask provided.@n
 * The mask contains a set of signals the caller is interested in handling and must be a subset
 * of combined interrupt and doorbell mask for the calling partition.
 *
 * @param[in] interrupt_mask mask of signals.
 * @param[in] timeout timeout value:@n
 *            @a PSA_BLOCK block the caller until one of the requested signals is asserted.@n
 *            @a PSA_POLL Returns immediately.
 * @return 32-bit value of asserted signals.
 */
uint32_t psa_wait_interrupt(uint32_t interrupt_mask, uint32_t timeout);

/**
 * Return the Partition ID of the caller.
 *
 * @note Bit[31] is set if the caller is from the NSPE.
 *
 * @param[in] msg_handle Handle for the caller's message.
 * @return Caller's partition ID
 */
int32_t psa_identity(psa_handle_t msg_handle);

/**
 * Get the message which corresponds to a given signal.
 *
 * @param[in]  signum an asserted signal returned from psa_wait().
 * @param[out] msg    pointer to a psa_msg structure.
 */
void psa_get(psa_signal_t signum, psa_msg_t *msg);

/**
 * Associate the caller-provided private data with a specified handle.
 *
 * @param[in] msg_handle Handle for the caller's message.
 * @param[in] rhandle    Reverse handle allocated by the Root of Trust Service.
 */
void psa_set_rhandle(psa_handle_t msg_handle, void *rhandle);

/**
 * Copy up to @a len bytes from position @a offset within the client message
 * payload into the Secure Partition buffer @a buffer.@n
 *
 * @note Callers should know how much data is available to read based on the@n
 *       @a size attribute of the psa_msg structure returned from psa_get().@n
 *       The copy is truncated if the requested range extends beyond the end of the payload.@n
 *       In such a case, the remaining space in @a buffer is not modified.
 *
 * @param[in]  msg_handle Handle for the client's message.
 * @param[in]  invec_idx  ::psa_invec index to be read.
 * @param[out] buf        Buffer to copy the requested data to.
 * @param[in]  num_bytes  Number of bytes to read from the client's message payload.
 * @return Number of bytes copied or 0 if offset is greater than the size attribute of psa_msg.
 */
size_t psa_read(psa_handle_t msg_handle, uint32_t invec_idx, void *buf, size_t num_bytes);

/**
 * Advance the current read offset by skipping @a num_bytes bytes for input vector
 * indexed by @а invec_idx.@n
 * If @a num_bytes is greater than the remaining number of bytes in the vector then
 * all the remaining bytes are skipped.
 *
 * @param[in]  msg_handle Handle for the client's message.
 * @param[in]  invec_idx  ::psa_invec index to be skipped.
 * @param[in]  num_bytes  Number of bytes to skip.
 * @return Number of bytes skipped or 0 if offset is greater than the size attribute of psa_msg.
 */
size_t psa_skip(psa_handle_t msg_handle, uint32_t invec_idx, size_t num_bytes);

/**
 * Write a response payload of @a bytes bytes starting at position @a offset in the client's response buffer.
 *
 * @note If the caller writes data beyond the client's response buffer size
 *       (@a response_size attribute of the psa_msg structure returned from psa_get()) a fatal error occurs.
 *
 * @param[in] msg_handle  Handle for the client's message.
 * @param[in] outvec_idx  ::psa_outvec index to be written to.
 * @param[in] buffer      Buffer with the data to write.
 * @param[in] num_bytes   Number of bytes to write.
 */
void psa_write(psa_handle_t msg_handle, uint32_t outvec_idx, const void *buffer, size_t num_bytes);

/**
 * Complete handling of specific message and unblocks the client.
 *
 * A return code must be specified, which will be sent to the client.@n
 * Negative return code represent errors, Positive integers are application-specific.
 *
 * @param[in] msg_handle Handle for the client's message.
 * @param[in] status Message result value to be reported to the client.
 */
void psa_reply(psa_handle_t msg_handle, psa_error_t status);

/**
 * Send a doorbell signal to a specific partition that is listening for that signal type.
 *
 * @param[in] partition_id partition ID of the target partition.
 */
void psa_notify(int32_t partition_id);

/**
 * Clear the doorbell signal.
 */
void psa_clear(void);

/**
 * Inform the SPM that an interrupt has been handled (end of interrupt).
 *
 * @param[in] irq_signal The interrupt signal that has been processed.
 */
void psa_eoi(uint32_t irq_signal);

/** @}*/ // end of RoT-Service-API group

#ifdef __cplusplus
}
#endif

/** @}*/ // end of SPM group

#endif  /* __MBED_SPM_SERVER_H__ */
