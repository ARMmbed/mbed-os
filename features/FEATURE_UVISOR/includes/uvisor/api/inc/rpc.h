/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
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
#ifndef __UVISOR_API_RPC_H__
#define __UVISOR_API_RPC_H__

#include "api/inc/rpc_exports.h"
#include "api/inc/uvisor_exports.h"
#include <stdint.h>
#include <stddef.h>


/** Wait for incoming RPC.
 *
 * @param fn_ptr_array       an array of RPC function targets that this call to
 *                           `rpc_fncall_waitfor` should handle RPC to
 * @param fn_count           the number of function targets in this array
 * @param box_id_caller[out] a memory location to store the box ID of the
 *                           calling box (the source box of the RPC). This is
 *                           set before the RPC is dispatched, so that the RPC
 *                           target function can read from this location to
 *                           determine the calling box ID. Optional.
 * @param timeout_ms         specifies how long to wait (in ms) for an incoming
 *                           RPC message before returning
 */
UVISOR_EXTERN int rpc_fncall_waitfor(const TFN_Ptr fn_ptr_array[], size_t fn_count, int * box_id_caller, uint32_t timeout_ms);

/** Wait for an outgoing RPC to finish.
 *
 * Wait for the result of a previously started asynchronous RPC. After this
 * call, ret will contain the return value of the RPC. The return value of this
 * function may indicate that there was an error or a timeout with non-zero.
 *
 * @param result[in]     The token to wait on for the result of an asynchronous RPC
 * @param timeout_ms[in] How long to wait (in ms) for the asynchronous RPC
 *                       message to finish before returning
 * @param ret[out]       The return value resulting from the finished RPC to
 *                       the target function
 * @returns              Non-zero on error or timeout, zero on successful wait
 */
UVISOR_EXTERN int rpc_fncall_wait(uvisor_rpc_result_t result, uint32_t timeout_ms, uint32_t * ret);

#endif /* __UVISOR_API_RPC_H__ */
