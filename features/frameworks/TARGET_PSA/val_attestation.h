/** @file
 * Copyright (c) 2019, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#ifndef _VAL_INITIAL_ATTESTATION_H_
#define _VAL_INITIAL_ATTESTATION_H_

#include "val.h"
#include "psa_initial_attestation_api.h"

#define MAX_CHALLENGE_SIZE      PSA_INITIAL_ATTEST_CHALLENGE_SIZE_64

enum attestation_function_code {
    VAL_INITIAL_ATTEST_GET_TOKEN        = 0x1,
    VAL_INITIAL_ATTEST_GET_TOKEN_SIZE   = 0x2,
    VAL_INITIAL_ATTEST_VERIFY_TOKEN     = 0x3,
};

int32_t val_attestation_function(int type, ...);
#endif /* _VAL_INITIAL_ATTESTATION_H_ */
