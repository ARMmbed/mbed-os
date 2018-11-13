/* Copyright (c) 2017 ARM Limited
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

#include <string.h>
#include "cmsis_os2.h"
#include "unity.h"
#include "spm_panic.h"
#include "spm_server.h"
#include "spm_client.h"
#include "spm_panic.h"
#include "psa_neg_dual_part2_partition.h"
#include "psa_neg_dual_part2_ifs.h"

#define MINOR_VER 5

void server_main2(void *ptr)
{
    uint32_t  signals   = 0;
    psa_msg_t msg       = {0};

    while (1) {
        signals = psa_wait_any(PSA_BLOCK);
        if (signals & DUMMY_MSK) {
            psa_get(DUMMY_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CALL:
                case PSA_IPC_CONNECT:
                case PSA_IPC_DISCONNECT:
                    psa_reply(msg.handle, PSA_SUCCESS);
                    break;
                default:
                    SPM_PANIC("Invalid msg type");
            }
        }
        else if (signals & PART2_CALL_INSIDE_PARTITION_MSK) {
            psa_get(PART2_CALL_INSIDE_PARTITION_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    psa_reply(msg.handle, PSA_SUCCESS);
                    break;
                }
                case PSA_IPC_CALL: {
                    psa_connect(DUMMY, MINOR_VER);
                    TEST_FAIL_MESSAGE("server_call_sid_in_same_partition negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_call_sid_in_same_partition msg type failure");
                }
            }
        } else {
            SPM_PANIC("Unknown signal (0x%08x)", signals);
        }
    }
}
