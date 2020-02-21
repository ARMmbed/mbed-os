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

#if !defined(MBED_CONF_RTOS_PRESENT)
#error [NOT_SUPPORTED] PSA compliance attestation test cases require RTOS to run.
#else
void test_entry_a001(val_api_t *val_api, psa_api_t *psa_api); 

int main(void)
{
    test_start(test_entry_a001, COMPLIANCE_TEST_ATTESTATION);
}
#endif
