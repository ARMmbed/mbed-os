/* mbed Microcontroller Library
 * Copyright (c) 2020 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed_error.h"
#include "tfm_ns_interface.h"

void mbed_tfm_init(void)
{
    /*
     * In case of V8-M, we need to initialize NS interface
     * after the RTOS has started to enable
     * communication from Secure and Non-Secure cores.
     */
    int32_t ret;

    ret = tfm_ns_interface_init();
    if (ret != TFM_SUCCESS) {
        /* Avoid undefined behavior after NS interface initialization failed */
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM,
                                   MBED_ERROR_CODE_INITIALIZATION_FAILED),
                   "Failed to initialize NS interface");
    }
}
