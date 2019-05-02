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
#include <stdlib.h>
#include "psa/client.h"
#include "psa/service.h"
#include "mbed_spm_partitions.h"

// ------------------------------------ Definitions ----------------------------------

#define SERVER_READ_MSG_BUF_SIZE        30
#define SERVER_RSP_BUF_SIZE             20
#define ACTUAL_MSG_SIZE                 22

// ---------------------------------- Global Variables -------------------------------

const char SERVER_EXPECTED_READ_MSG[] = "Hello and welcome SPM";
const char WRITE_MSG_BUF[] = "Response1";

// ------------------------------ Partition's Main Thread ----------------------------

void smoke_part_main(void *ptr)
{
    uint32_t  signals   = 0;
    int32_t   client_id = 0;
    psa_msg_t msg       = {0};

    while (1) {

        signals = psa_wait(ROT_SRV1_MSK, PSA_BLOCK);
        if ((signals & ROT_SRV1_MSK) != ROT_SRV1_MSK) {
            SPM_PANIC("Received unknown signal (0x%08lx)\n", signals);
        }

        if (PSA_SUCCESS != psa_get(ROT_SRV1_MSK, &msg)) {
            continue;
        }

        if (msg.handle != PSA_NULL_HANDLE) {
            client_id = msg.client_id;
            if (client_id != -1) {
                SPM_PANIC("Received message from unexpected source (0x%08lx)\n", client_id);
            }
        }

        switch (msg.type) {
            case PSA_IPC_CALL: {
                if (
                    ((msg.in_size[0] + msg.in_size[1] + msg.in_size[2] + msg.in_size[3]) != ACTUAL_MSG_SIZE) ||
                    (msg.out_size[0] != SERVER_RSP_BUF_SIZE)
                ) {
                    SPM_PANIC("Received message does not comply with message convention");
                }


                char *read_msg_buf = malloc(sizeof(char) * SERVER_READ_MSG_BUF_SIZE);
                if (NULL == read_msg_buf) {
                    SPM_PANIC("Failed to allocate Memory");
                }
                memset(read_msg_buf, 0, SERVER_READ_MSG_BUF_SIZE);
                char *read_ptr = read_msg_buf;

                for (size_t i = 0; i < PSA_MAX_IOVEC - 1; i++) {
                    uint32_t bytes_read = psa_read(msg.handle, i, read_ptr, msg.in_size[i]);
                    if (bytes_read != msg.in_size[i]) {
                        SPM_PANIC("Expected to read %zu, got %lu", msg.in_size[i], bytes_read);
                    }

                    read_ptr += bytes_read;
                }

                int cmp_res = strcmp(SERVER_EXPECTED_READ_MSG, read_msg_buf);
                if (cmp_res != 0) {
                    SPM_PANIC("psa_read() - Bad reading!!");
                }

                psa_write(msg.handle, 0, WRITE_MSG_BUF, strlen(WRITE_MSG_BUF) + 1);
                free(read_msg_buf);
                read_msg_buf = NULL;
                read_ptr = NULL;
                break;
            }
            case PSA_IPC_DISCONNECT:
            // Fallthrough
            case PSA_IPC_CONNECT: {
                if (
                    (msg.out_size[0] != 0) || (msg.out_size[1] != 0) ||
                    (msg.out_size[2] != 0) || (msg.out_size[3] != 0) ||
                    (msg.in_size[0] != 0) || (msg.in_size[1] != 0) ||
                    (msg.in_size[2] != 0) || (msg.in_size[3] != 0)
                ) {
                    SPM_PANIC("Should not receive iovecs in PSA_IPC_CONNECT or PSA_IPC_DISCONNECT");
                }

                break;
            }
            default:
                SPM_PANIC("Unexpected message type %lu!", msg.type);
                break;
        }

        psa_reply(msg.handle, PSA_SUCCESS);
    }
}

