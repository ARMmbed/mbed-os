/*
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __UVISOR_API_IPC_H__
#define __UVISOR_API_IPC_H__

#include "api/inc/ipc_exports.h"
#include "api/inc/uvisor_exports.h"
#include <stdint.h>
#include <stddef.h>


/** Wait for any of the specified IPC operations to complete.
 *
 * @note  This function currently spins, burning through power.
 *
 * @param[in]  wait_tokens  a bitfield of tokens to wait on
 * @param[out] done_tokens  a bitfield which tokens completed
 * @param[in]  timeout_ms   how long to wait (in ms) for an IPC operation
 *                          before returning. 0 means don't wait at all. Any
 *                          other value means wait forever.
 * @return     0 on success, non-zero error code otherwise
 */
UVISOR_EXTERN int ipc_waitforany(uint32_t wait_tokens, uint32_t * done_tokens, uint32_t timeout_ms);

/** Wait for all of the specified IPC operations to complete.
 *
 * @note  This function currently spins, burning through power.
 *
 * @param[in]  wait_tokens  a bitfield of tokens to wait on
 * @param[out] done_tokens  a bitfield which tokens completed
 * @param[in]  timeout_ms   how long to wait (in ms) for an IPC operation
 *                          before returning. 0 means don't wait at all.
 *                          Any other value means wait forever.
 * @return     0 on success, non-zero error code otherwise
 */
UVISOR_EXTERN int ipc_waitforall(uint32_t wait_tokens, uint32_t * done_tokens, uint32_t timeout_ms);

/** Asynchronously send an IPC message
 *
 * @note The memory used for sending the message (pointed to by msg) must be
 * valid until after the send is complete.
 *
 * @param[in]  desc   an IPC descriptor for the message
 * @param[in]  msg    the message to send
 *
 * @return     0 on success, non-zero error code otherwise
 * */
UVISOR_EXTERN int ipc_send(uvisor_ipc_desc_t * desc, const void * msg);

/** Asynchronously receive an IPC message
 *
 * @note The memory used for receiving the message (pointed to by msg) must be
 * valid until after the receive is complete.
 *
 * @param[inout] desc   an IPC descriptor for the message
 * @param[out]   msg    the memory to copy the message to
 *
 * @return     0 on success, non-zero error code otherwise
 */
UVISOR_EXTERN int ipc_recv(uvisor_ipc_desc_t * desc, void * msg);

#endif /* __UVISOR_API_IPC_H__ */
