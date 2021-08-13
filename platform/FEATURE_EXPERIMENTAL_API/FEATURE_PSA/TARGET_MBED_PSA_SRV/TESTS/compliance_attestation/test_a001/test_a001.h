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
#ifndef _TEST_A001_CLIENT_TESTS_H_
#define _TEST_A001_CLIENT_TESTS_H_

#include "val_attestation.h"
#define test_entry CONCAT(test_entry_, a001)
#define val CONCAT(val,test_entry)
#define psa CONCAT(psa,test_entry)

#define TOKEN_SIZE          512

extern val_api_t *val;
extern psa_api_t *psa;
extern client_test_t test_a001_attestation_list[];

int32_t psa_initial_attestation_get_token_test(caller_security_t caller);
int32_t psa_initial_attestation_get_token_size_test(caller_security_t caller);
#endif /* _TEST_A001_CLIENT_TESTS_H_ */
