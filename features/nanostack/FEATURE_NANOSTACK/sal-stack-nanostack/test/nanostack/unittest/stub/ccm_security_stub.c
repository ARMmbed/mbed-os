/*
 * Copyright (c) 2014-2016, Arm Limited and affiliates.
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

#include <stdint.h>
#include <string.h>
#include "ccmLIB.h"
#include "platform/arm_hal_aes.h"
#include "ccm_security_stub.h"

ccm_security_stub_def_t ccm_security_stub_def;

ccm_globals_t *ccm_sec_init(uint8_t sec_level, const uint8_t *ccm_key, uint8_t mode, uint8_t ccm_l)
{
    return ccm_security_stub_def.ccm_struct_ptr;
}

int8_t ccm_process_run(ccm_globals_t *ccm_params)
{
    return ccm_security_stub_def.value_int8;
}

