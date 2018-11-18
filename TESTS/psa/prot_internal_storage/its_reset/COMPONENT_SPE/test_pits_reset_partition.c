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

// -------------------------------------- Includes -----------------------------------

#include <string.h>
#include "cmsis_os2.h"
#include "spm_server.h"
#include "spm_panic.h"
#include "psa_test_its_reset_partition.h"
#include "psa_prot_internal_storage.h"
#include "test_pits_impl.h"

#ifdef   __cplusplus
extern "C"
{
#endif

void test_pits_entry(void *ptr)
{
    uint32_t signals = 0;
    psa_msg_t msg = {0};
    psa_error_t status = PSA_SUCCESS;

    while (1) {
        signals = psa_wait_any(PSA_BLOCK);
        if ((signals & TEST_PSA_ITS_RESET_MSK) != 0) {
            psa_get(TEST_PSA_ITS_RESET_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: //fallthrough
                case PSA_IPC_DISCONNECT: {
                    break;
                }
                case PSA_IPC_CALL: {
                    status = test_psa_its_reset_impl();
                    break;
                }
                default: {
                    SPM_PANIC("Unexpected message type %d!", (int)(msg.type));
                    break;
                }
            }

            psa_reply(msg.handle, status);
        }
    }
}

#ifdef   __cplusplus
}
#endif
