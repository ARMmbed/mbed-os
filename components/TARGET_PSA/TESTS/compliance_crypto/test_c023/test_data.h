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
    psa_key_usage_t         usage;
    psa_algorithm_t         key_alg;
} test_data;

static test_data check1[] = {
#ifdef ARCH_TEST_HKDF
#ifdef ARCH_TEST_SHA256
{"Test psa_key_policy_get_usage with usage as encrypt\n",
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_HKDF(PSA_ALG_SHA_256),
},

{"Test psa_key_policy_get_usage with usage as decrypt\n",
 PSA_KEY_USAGE_DECRYPT, PSA_ALG_HKDF(PSA_ALG_SHA_256),
},

{"Test psa_key_policy_get_usage with usage as derive\n",
 PSA_KEY_USAGE_DERIVE, PSA_ALG_HKDF(PSA_ALG_SHA_256),
},

{"Test psa_key_policy_get_usage with usage as export\n",
 PSA_KEY_USAGE_EXPORT, PSA_ALG_HKDF(PSA_ALG_SHA_256),
},

{"Test psa_key_policy_get_usage with usage as sign\n",
 PSA_KEY_USAGE_SIGN, PSA_ALG_HKDF(PSA_ALG_SHA_256),
},

{"Test psa_key_policy_get_usage with usage as verify\n",
 PSA_KEY_USAGE_VERIFY, PSA_ALG_HKDF(PSA_ALG_SHA_256),
},
#endif
#endif

};
