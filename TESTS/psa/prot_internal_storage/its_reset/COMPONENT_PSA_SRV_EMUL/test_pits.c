/* Copyright (c) 2018 ARM Limited
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

#ifndef TARGET_PSA
#error [NOT_SUPPORTED] ITS tests can run only on PSA-enabled targets.
#endif // TARGET_PSA

#include "test_pits.h"
#include "test_pits_impl.h"

psa_its_status_t test_psa_its_reset(void)
{
    return test_psa_its_reset_impl();
}
