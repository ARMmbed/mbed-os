/* Copyright (c) 2019 ARM Limited
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

#include "psa/lifecycle.h"
#include "platform_srv_impl.h"

uint32_t psa_security_lifecycle_state(void)
{
    uint32_t lc_state = 0;
    psa_status_t status = PSA_SUCCESS;
    status = psa_platfrom_lifecycle_get_impl(&lc_state);
    if (status != PSA_SUCCESS) {
        lc_state = PSA_LIFECYCLE_UNKNOWN;
    }
    return lc_state;
}

psa_status_t mbed_psa_reboot_and_request_new_security_state(uint32_t new_state)
{
    return psa_platfrom_lifecycle_change_request_impl(new_state);
}

void mbed_psa_system_reset(void)
{
    mbed_psa_system_reset_impl();
}
