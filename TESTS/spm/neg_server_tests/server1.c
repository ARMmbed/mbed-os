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

#include <stdbool.h>
#include <string.h>
#include "unity.h"
#include "spm_panic.h"
#include "spm_server.h"
#include "psa_neg_server_part1_partition.h"


void server_main1(void *ptr)
{
    psa_msg_t msg = {0};
    uint32_t signals = 0;

    while (true) {
        signals = psa_wait_any(PSA_BLOCK);
        if (signals & PART1_CLEAR_NO_DOORBELL_MSK) {
            psa_get(PART1_CLEAR_NO_DOORBELL_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    psa_clear();
                    TEST_FAIL_MESSAGE("server_clear_no_doorbell negative test failed");
                }
                default: {
                    TEST_FAIL_MESSAGE("server_clear_no_doorbell msg type failure");
                }
            }
        }
        else if (signals & PART1_END_INVALID_RETVAL_CONNECT_MSK) {
            psa_get(PART1_END_INVALID_RETVAL_CONNECT_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    psa_end(msg.handle, PSA_CONNECTION_ACCEPTED + 1);
                    TEST_FAIL_MESSAGE("server_end_invalid_retval_connect negative test failed");
                }
                default: {
                    TEST_FAIL_MESSAGE("server_end_invalid_retval_connect msg type failure");
                }
            }
        }
        else if (signals & PART1_END_INVALID_RETVAL_CALL_MSK) {
            psa_get(PART1_END_INVALID_RETVAL_CALL_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    psa_end(msg.handle, PSA_CONNECTION_ACCEPTED);
                    break;
                }
                case PSA_IPC_CALL: {
                    psa_end(msg.handle, PSA_DROP_CONNECTION + 1);
                    TEST_FAIL_MESSAGE("server_end_invalid_retval_call negative test failed");
                }
                default: {
                    TEST_FAIL_MESSAGE("server_end_invalid_retval_call msg type failure");
                }
            }
        }
        else {
            SPM_ASSERT(false);
        }
    }
}

