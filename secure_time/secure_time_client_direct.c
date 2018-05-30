/* Copyright (c) 2018 ARM Limited
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

// This is the direct API call implementation of the Secure Time NSPE client
// interface.
// It can be used to built standalone Secure Time module without requiring SPM
// implementation.

#if defined(DEVICE_TRNG)

#include "secure_time_client.h"
#include "secure_time_impl.h"

int32_t secure_time_set_trusted_init(uint64_t *nonce)
{
    return secure_time_set_trusted_init_impl(nonce);
}

int32_t secure_time_set_trusted_commit(const void *blob, size_t blob_size)
{
    return secure_time_set_trusted_commit_impl(blob, blob_size);
}

int32_t secure_time_set(uint64_t new_time)
{
    return secure_time_set_impl(new_time);
}

uint64_t secure_time_get(void)
{
    return secure_time_get_impl();
}

#endif // defined(DEVICE_TRNG)
