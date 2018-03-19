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
#include "spm_server.h"
#include "spm_panic.h"
#include "psa_part1_partition.h"

#define MSG_BUF_SIZE 128
uint8_t data[MSG_BUF_SIZE] = {0};

void server_main(void *ptr)
{
    uint32_t signals = 0;
    while (1) {
        signals = psa_wait_any(PSA_WAIT_BLOCK);
        if (signals & PART1_SF1_MSK) {
            psa_msg_t msg = {0};
            psa_get(PART1_SF1_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_MSG_TYPE_CONNECT: {
                    break;
                }
                case PSA_IPC_MSG_TYPE_CALL: {
                    memset(data, 0, sizeof(data));
                    if (msg.size[0] + msg.size[1] + msg.size[2] > 1) {
                        size_t offset = 0;
                        offset += psa_read(msg.handle, 0, (void*)data, msg.size[0]);
                        offset += psa_read(msg.handle, 1, (void*)(data + offset), msg.size[1]);
                        offset += psa_read(msg.handle, 2, (void*)(data + offset), msg.size[2]);
                    }
                    if (msg.response_size > 0) {
                        uint8_t resp_size = data[0];
                        uint8_t resp_offset = data[1];
                        psa_write(msg.handle, 0, (const void*)(data + resp_offset), resp_size);
                    }
                    break;
                }
                case PSA_IPC_MSG_TYPE_DISCONNECT: {
                    break;
                }
                default: {
                    SPM_PANIC("Invalid msg type");
                }
            }
            psa_end(msg.handle, PSA_SUCCESS);
        }
    }

    return;
}
