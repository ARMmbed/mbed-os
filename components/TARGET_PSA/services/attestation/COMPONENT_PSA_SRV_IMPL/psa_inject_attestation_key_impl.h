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

#ifndef __PSA_INITIAL_ATTESTATION_IMPL_H__
#define __PSA_INITIAL_ATTESTATION_IMPL_H__

#include "psa/crypto.h"
#include <stdint.h>
#include <string.h>

#ifdef   __cplusplus
extern "C"
{
#endif

psa_status_t
psa_attestation_inject_key_impl(const uint8_t *key_data,
                                size_t key_data_length,
                                psa_key_type_t type,
                                uint8_t *public_key_data,
                                size_t public_key_data_size,
                                size_t *public_key_data_length);

#ifdef   __cplusplus
}
#endif

#endif /* __PSA_INITIAL_ATTESTATION_IMPL_H__ */
