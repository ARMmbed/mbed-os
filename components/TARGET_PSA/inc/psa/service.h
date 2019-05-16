/* Copyright (c) 2017-2018 ARM Limited
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

#if defined(TARGET_TFM)

#include "interface/include/psa_service.h"
#include "secure_fw/core/ipc/include/tfm_utils.h"
#define SPM_PANIC(format, ...) tfm_panic()

#elif defined(TARGET_MBED_SPM)

#include "TARGET_MBED_SPM/psa_defs.h"
#include "TARGET_MBED_SPM/COMPONENT_SPE/spm_server.h"
#include "TARGET_MBED_SPM/COMPONENT_SPE/spm_panic.h"

#else

#error "Compiling psa service header on non-secure target is not allowed"

#endif
