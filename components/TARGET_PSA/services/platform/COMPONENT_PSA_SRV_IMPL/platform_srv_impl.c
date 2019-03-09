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
#include "psa/internal_trusted_storage.h"
#include "platform_srv_impl.h"
#include "cmsis.h"

#ifndef MBED_CONF_LIFECYCLE_STATE
#define MBED_CONF_LIFECYCLE_STATE PSA_LIFECYCLE_ASSEMBLY_AND_TEST
#endif

psa_status_t psa_platfrom_lifecycle_get_impl(uint32_t *lc_state)
{
    *lc_state = MBED_CONF_LIFECYCLE_STATE;
    return PSA_SUCCESS;
}

psa_status_t psa_its_reset();

psa_status_t psa_platfrom_lifecycle_change_request_impl(uint32_t state)
{
    if (PSA_LIFECYCLE_ASSEMBLY_AND_TEST == state) {
        return psa_its_reset();
    }
    return PSA_ERROR_NOT_SUPPORTED;
}

MBED_WEAK void mbed_psa_system_reset_impl(void)
{
    /* Reset the system */
    NVIC_SystemReset();
}
