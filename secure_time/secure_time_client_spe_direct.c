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

// This is the direct API call implementation of the Secure Time SPE client 
// interface.
// This file extends NSPE interface implementation by implementing APIs
// available from SPE only.
// This file can be used to built standalone Secure Time module without
// requiring full SPM implementation.

#include "secure_time_client_spe.h"
#include "secure_time_impl.h"

#if SECURE_TIME_ENABLED

int32_t secure_time_set_stored_public_key(const void* pubkey, size_t key_size)
{
    return secure_time_set_stored_public_key_impl(pubkey, key_size);
}

int32_t secure_time_get_stored_public_key(
    void *pubkey,
    size_t size,
    size_t *actual_size
)
{
    return secure_time_get_stored_public_key_impl(pubkey, size, actual_size);
}

int32_t secure_time_get_stored_public_key_size(size_t *actual_size)
{
    return secure_time_get_stored_public_key_size_impl(actual_size);
}

#endif // SECURE_TIME_ENABLED
