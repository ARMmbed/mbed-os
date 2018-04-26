/*
 * Copyright (c) 2013-2018, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include <string.h>
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include "mbedtls/ecdsa.h"
#include "ecdsa_test.h"

#if defined(MBEDTLS_ECDSA_C)
/* Tests whether the mbedtls_ecdsa_* functions are included in the build configuration
 * This test does not generate asserts. Instead, a failure will cause a compile error
 * Testing the functionality of the ecdsa functions is beyond the scope
 */
int mbedtls_ecdsa_test_mbedos(int verbose) {
    mbedtls_ecp_group group;
    group.N.p = NULL; // will cause the ecdsa function to exit early
    mbedtls_ecdsa_sign(&group, NULL, NULL, NULL, NULL, 0, NULL, NULL);

    return 0;
}
#endif // MBEDTLS_ECDSA_C
