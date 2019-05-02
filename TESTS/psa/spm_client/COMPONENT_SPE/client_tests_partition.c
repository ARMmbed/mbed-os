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
#include "psa/client.h"
#include "psa/service.h"
#include "mbed_spm_partitions.h"

#define MSG_BUF_SIZE 128
uint8_t data[MSG_BUF_SIZE] = {0};

void client_part_main(void *ptr)
{
    psa_signal_t signals = 0;
    psa_msg_t msg = {0};
    while (1) {
        signals = psa_wait(CLIENT_TESTS_PART1_WAIT_ANY_SID_MSK, PSA_BLOCK);
        if (signals & PART1_ROT_SRV1_MSK) {
            if (PSA_SUCCESS != psa_get(PART1_ROT_SRV1_MSK, &msg)) {
                continue;
            }

            switch (msg.type) {
                case PSA_IPC_CONNECT:
                case PSA_IPC_DISCONNECT:
                    break;
                case PSA_IPC_CALL: {
                    memset(data, 0, sizeof(data));
                    if (msg.in_size[0] + msg.in_size[1] + msg.in_size[2] > 1) {
                        size_t offset = psa_read(msg.handle, 0, (void *)data, msg.in_size[0]);
                        offset += psa_read(msg.handle, 1, (void *)(data + offset), msg.in_size[1]);
                        psa_read(msg.handle, 2, (void *)(data + offset), msg.in_size[2]);
                    }

                    if (msg.out_size[0] > 0) {
                        uint8_t resp_size = data[0];
                        uint8_t resp_offset = data[1];
                        psa_write(msg.handle, 0, (const void *)(data + resp_offset), resp_size);
                    }
                    break;
                }

                default: {
                    SPM_PANIC("Invalid msg type");
                }
            }

            psa_reply(msg.handle, PSA_SUCCESS);
        } else if (signals & DROP_CONN_MSK) {
            if (PSA_SUCCESS != psa_get(DROP_CONN_MSK, &msg)) {
                continue;
            }

            switch (msg.type) {
                case PSA_IPC_CONNECT:
                case PSA_IPC_DISCONNECT:
                    psa_reply(msg.handle, PSA_SUCCESS);
                    break;
                case PSA_IPC_CALL:
                    psa_reply(msg.handle, PSA_DROP_CONNECTION);
                    break;
                default:
                    SPM_PANIC("Invalid msg type");
            }
        } else if (signals & SECURE_CLIENTS_ONLY_MSK) {
            if (PSA_SUCCESS != psa_get(SECURE_CLIENTS_ONLY_MSK, &msg)) {
                continue;
            }

            switch (msg.type) {
                case PSA_IPC_CONNECT:
                case PSA_IPC_DISCONNECT:
                case PSA_IPC_CALL:
                    psa_reply(msg.handle, PSA_SUCCESS);
                    break;
                default:
                    SPM_PANIC("Invalid msg type");
            }
        } else {
            SPM_PANIC("Received invalid signal %lu", signals);
        }
    }
}

