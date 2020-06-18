/* Copyright (c) 2017-2020 ARM Limited
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

/* This implementation of the PSA Client API is provided for application
 * compatibility on single v7-M targets. The client implementation is stubbed
 * and non-functional, but present and returns sane errors. */

#include "psa/client.h"

uint32_t psa_framework_version(void)
{
    return PSA_FRAMEWORK_VERSION;
}

uint32_t psa_version(uint32_t sid)
{
    return PSA_VERSION_NONE;
}

psa_handle_t psa_connect(uint32_t sid, uint32_t version)
{
    return PSA_ERROR_PROGRAMMER_ERROR;
}

psa_status_t psa_call(psa_handle_t handle, int32_t type,
                      const psa_invec *in_vec,
                      size_t in_len,
                      psa_outvec *out_vec,
                      size_t out_len)
{
    return PSA_ERROR_PROGRAMMER_ERROR;
}

void psa_close(psa_handle_t handle)
{
    return;
}
