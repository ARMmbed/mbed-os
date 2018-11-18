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

#include <string.h>
#include "cmsis_os2.h"
#include "spm_server.h"
#include "spm_panic.h"
#include "psa_server_test_part2_partition.h"

static psa_msg_t msg = {0};

void part2_main(void *ptr)
{
    uint32_t signals = 0;
    size_t len = 0;
    char *str = NULL;

    while (1) {
        signals = psa_wait_any(PSA_BLOCK);
        if (0 == (signals & (ROT_SRV_REVERSE_MSK | ROT_SRV_DB_TST_MSK))) {
            SPM_PANIC("returned from psa_wait_any without ROT_SRV_REVERSE_MSK or ROT_SRV_DB_TST_MSK bit on\n");
        }
        if (signals & ROT_SRV_REVERSE_MSK) {
            psa_get(ROT_SRV_REVERSE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CALL: {
                    if ((msg.in_size[0] + msg.in_size[1] + msg.in_size[2]) == 0) {
                        SPM_PANIC("got a zero message size to REVERSE ROT_SRV\n");
                    }

                    len = msg.in_size[0];
                    str = (char *)malloc(sizeof(char) * len);
                    if (NULL == str) {
                        SPM_PANIC("memory allocation failure\n");
                    }
                    psa_read(msg.handle, 0, str, len);
                    for (size_t i = 0; i < len / 2; i ++) {
                        char a = str[i];
                        str[i] = str[len - i - 1];
                        str[len - i - 1] = a;
                    }

                    psa_write(msg.handle, 0, str, len);
                    free(str);
                    str = NULL;
                    break;
                }
                case PSA_IPC_CONNECT:
                case PSA_IPC_DISCONNECT:
                    break;
                default:
                    SPM_PANIC("Unexpected message type %d!", (int)(msg.type));
                    break;
            }

            psa_reply(msg.handle, PSA_SUCCESS);
        } else { // -- Doorbell test

            psa_get(ROT_SRV_DB_TST_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CALL: {
                    int32_t caller_part_id = psa_identity(msg.handle);
                    // Doorbell contract is valid only between secure partitions
                    if (PSA_NSPE_IDENTIFIER == caller_part_id) {
                        SPM_PANIC("Caller partition is non secure\n");
                    }
                    // In doorbell scenario the server first calls psa_reply()
                    psa_reply(msg.handle, PSA_SUCCESS);
                    // Then the servers waits to some driver making long calculations - imitate using osDelay()
                    osDelay(20);
                    // After work is done, ring the doorbell
                    psa_notify(caller_part_id);
                    break;
                }
                case PSA_IPC_CONNECT:
                case PSA_IPC_DISCONNECT:
                    psa_reply(msg.handle, PSA_SUCCESS);
                    break;
                default:
                    SPM_PANIC("Unexpected message type %d!", (int)(msg.type));
                    break;
            }
        }
    }
}
