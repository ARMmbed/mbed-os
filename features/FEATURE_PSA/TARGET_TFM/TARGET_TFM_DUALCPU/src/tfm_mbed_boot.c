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
#include "tfm_multi_core_api.h"
#include "tfm_ns_mailbox.h"
#include "platform_multicore.h"
#include "tfm_ns_interface.h"

static struct ns_mailbox_queue_t ns_mailbox_queue;

void mbed_tfm_init(void)
{
    /*
     * In case the of dual CPU, we need to initialize IPC, mailbox
     * and NS interface after the RTOS has started to enable
     * communication from Secure and Non-Secure cores.
     */
    int32_t ret;

    ret = tfm_ns_wait_for_s_cpu_ready();
    /*
     * Normally would expect "TFM_SUCCESS" returned here by TF-M, as this
     * isn't a mailbox function. There may be some platforms other than PSoC6,
     * which doesn't require tfm_ns_wait_for_s_cpu_ready() implementation.
     * "PLATFORM_MAILBOX_SUCCESS" is a low-level error code and should be
     * replaced by "TFM_SUCCESS".
     * As the function definition has been imported from the TF-M, therefore
     * a issue has been raised - https://developer.trustedfirmware.org/T660
     */
    if (ret != PLATFORM_MAILBOX_SUCCESS) {
        /* Avoid undefined behavior after multi-core sync-up failed */
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM,
                                   MBED_ERROR_CODE_INITIALIZATION_FAILED),
                   "Failed to sync-up multi-core");
    }

    ret = tfm_ns_mailbox_init(&ns_mailbox_queue);
    if (ret != MAILBOX_SUCCESS) {
        /* Avoid undefined behavior after NS mailbox initialization failed */
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM,
                                   MBED_ERROR_CODE_INITIALIZATION_FAILED),
                   "Failed to initialize NS mailbox");
    }

    ret = tfm_ns_interface_init();
    if (ret != TFM_SUCCESS) {
        /* Avoid undefined behavior after NS interface initialization failed */
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM,
                                   MBED_ERROR_CODE_INITIALIZATION_FAILED),
                   "Failed to initialize NS interface");
    }
}
