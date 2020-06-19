/*
 * Copyright (c) 2020, Arm Limited and affiliates.
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
#include "val_interfaces.h"
#include "pal_mbed_os_intf.h"
#include "lifecycle.h"

#ifndef PS_ALLOW_ENTIRE_STORAGE_FILL
#error [NOT_SUPPORTED] Test is too long for CI, thus always fails on timeout.
#endif

#ifdef ITS_TEST
void test_entry_s003(val_api_t *val_api, psa_api_t *psa_api);
#elif PS_TEST
void test_entry_p003(val_api_t *val_api, psa_api_t *psa_api);
#endif

int main(void)
{
#ifdef ITS_TEST
    test_start(test_entry_s003, COMPLIANCE_TEST_STORAGE);
#elif PS_TEST
    test_start(test_entry_p003, COMPLIANCE_TEST_STORAGE);
#endif

}
