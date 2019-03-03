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

#include "val_crypto.h"

typedef struct {
    char                    test_desc[75];
    size_t                  size;
    psa_status_t            expected_status;
} test_data;

static test_data check1[] = {
{"Test psa_generate_random to get 0 Byte data\n", 0, PSA_SUCCESS
},

{"Test psa_generate_random to get 16 Byte data\n", 16, PSA_SUCCESS
},

{"Test psa_generate_random to get 24 Byte data\n", 24, PSA_SUCCESS
},

{"Test psa_generate_random to get 32 Byte data\n", 32, PSA_SUCCESS
},

{"Test psa_generate_random to get 64 Byte data\n", 64, PSA_SUCCESS
},

{"Test psa_generate_random to get 128 Byte data\n", 128, PSA_SUCCESS
},

{"Test psa_generate_random to get 256 Byte data\n", 256, PSA_SUCCESS
},

{"Test psa_generate_random to get 512 Byte data\n", 512, PSA_SUCCESS
},

{"Test psa_generate_random to get 1000 Byte data\n", 100, PSA_SUCCESS
},

{"Test psa_generate_random to get 1024 Byte data\n", 1024, PSA_SUCCESS
},
};
