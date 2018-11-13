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

#include "string.h"
#include "spm_server.h"
#include "spm_panic.h"
#include "psa_server_test_part1_partition.h"
#include "server_tests.h"

extern psa_test_server_side_func test_list[];
static size_t num_of_tests = 0;
static psa_msg_t msg = {0};
static void init_num_of_tests()
{
    size_t i = 0;
    while(test_list[i] != NULL) {
        i++;
    }

    num_of_tests = i;
}

void part1_main(void *ptr)
{
    uint32_t signals = 0;
    psa_error_t test_status = PSA_SUCCESS; // status of the api calls during the test
    psa_error_t test_result = PSA_SUCCESS; // result of the critical section of the test
    test_action_t action;
    uint32_t test_idx = 0;

    init_num_of_tests();
    while (1) {
        signals = psa_wait_any(PSA_BLOCK);
        if (0 == (signals & CONTROL_MSK)) {
            SPM_PANIC("returned from psa_wait_any without CONTROL_ROT_SRV bit on signals=(0x%08x)\n", signals);
        }

        psa_get(CONTROL_MSK, &msg);
        switch (msg.type) {
        case PSA_IPC_CALL:
            if (msg.in_size[0] == 0) {
                SPM_PANIC("got a zero message size to CONTROL ROT_SRV\n");
            }

            if (psa_read(msg.handle, 0, &action, sizeof(action)) != sizeof(action)) {
                SPM_PANIC("could not read the entire test payload structure\n");
            }

            switch (action) {
                case START_TEST:
                    if ((test_idx >= num_of_tests) || (test_list[test_idx] == NULL)) {
                        SPM_PANIC("Invalid test ID was sent!\n");
                    }

                    psa_reply(msg.handle, PSA_SUCCESS);
                    test_status = test_list[test_idx](&test_result);
                    break;
                case GET_TEST_RESULT:
                    test_idx++;
                    psa_write(msg.handle, 0, &test_result, sizeof(test_result));
                    psa_reply(msg.handle, test_status);
                    break;
                default:
                    SPM_PANIC("Got illegal Value in test action");
            }

            break;
        case PSA_IPC_CONNECT:
        case PSA_IPC_DISCONNECT:
            psa_reply(msg.handle, PSA_SUCCESS);
            break;
        default:
            SPM_PANIC("Unexpected message type %d!", (int)(msg.type));
        }
    }
}
