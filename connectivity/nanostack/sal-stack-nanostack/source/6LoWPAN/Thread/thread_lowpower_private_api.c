/*
 * Copyright (c) 2017-2019, Arm Limited and affiliates.
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


#include "nsconfig.h"
#include <ns_types.h>
#include "thread_lowpower_api.h"

/*
 * Dummy implementation for thread_lowpower_api.
 */

#ifndef HAVE_THREAD_V2
int thread_lowpower_test_probe_send(int8_t interface_id, uint8_t *address, uint8_t *metrics_ptr, uint8_t metrics_len, thread_lowpower_resp_cb response_cb)
{
    (void)interface_id;
    (void)address;
    (void)metrics_ptr;
    (void)metrics_len;
    (void)response_cb;
    return -1;
}
int thread_lowpower_metrics_management_request_send(int8_t interface_id, uint8_t *address, uint8_t series_id, uint8_t series_flags, uint16_t timeout,  uint8_t *metrics_ptr, uint8_t metrics_len, thread_lowpower_resp_cb response_cb)
{

    (void)interface_id;
    (void)address;
    (void)series_id;
    (void)series_flags;
    (void)timeout;
    (void)metrics_ptr;
    (void)metrics_len;
    (void)response_cb;
    return -1;
}
#endif
