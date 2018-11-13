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
#include "spm_server.h"
#include "spm_panic.h"
#include "psa_neg_client_part1_partition.h"

#define msg_buff_SIZE 128
char msg_buff[msg_buff_SIZE];

void server_main1(void *ptr)
{
    uint32_t signals = 0;
    memset(msg_buff, 0, sizeof(msg_buff));
    while (true) {
        signals = psa_wait_any(PSA_BLOCK);
        if (signals & NEG_CLIENT_PART1_ROT_SRV1_MSK) {
            psa_msg_t msg = {0};
            psa_get(NEG_CLIENT_PART1_ROT_SRV1_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT:
                case PSA_IPC_DISCONNECT:
                    break;
                case PSA_IPC_CALL: {
                    memset(msg_buff, 0, msg_buff_SIZE);
                    uint32_t bytes_read = 0;
                    for (size_t i = 0; i < PSA_MAX_IOVEC; i++) {
                        bytes_read = psa_read(msg.handle, i, msg_buff + bytes_read, msg.in_size[i]);
                    }

                    if (msg.out_size[0] > 0) {
                        psa_write(msg.handle, 0, msg_buff, bytes_read);
                    }
                    break;
                }
                default:
                    SPM_PANIC("Unknown message type");
                    break;
            }

            psa_reply(msg.handle, PSA_SUCCESS);
        }
    }
}
