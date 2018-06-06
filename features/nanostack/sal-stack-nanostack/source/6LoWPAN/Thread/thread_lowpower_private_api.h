/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \brief Nanostack lowpower instance and corresponding methods.
 */

#ifndef THREAD_THREAD_LOWPOWER_PRIVATE_API_H_
#define THREAD_THREAD_LOWPOWER_PRIVATE_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#define thread_lowpower_process_request(mle_msg)
#define thread_lowpower_process_response(src_address, instance_id, data_ptr, data_len)
#define thread_lowpower_timer(cur, ticks)
#define thread_lowpower_link_metrics_write(cur, destination_address, ptr) (ptr)
#define thread_lowpower_link_metrics_length(cur, destination_address) (0)
#define thread_lowpower_metrics_management_query_request_process(cur, mle_msg, security_headers, linkMargin)

#ifdef __cplusplus
}
#endif

#endif /* THREAD_THREAD_LOWPOWER_PRIVATE_API_H_ */
