/*
* Copyright (c) 2018-2019 ARM Limited. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the License); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an AS IS BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <stdint.h>
#include "attestation.h"

enum psa_attest_err_t
attest_check_memory_access(void *addr,
                           uint32_t size,
                           enum attest_memory_access_t access) {
    if (size == 0)
    {
        return PSA_ATTEST_ERR_INVALID_INPUT;
    }
    return PSA_ATTEST_ERR_SUCCESS;
}
