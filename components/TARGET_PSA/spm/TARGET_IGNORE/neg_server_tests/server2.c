/* Copyright (c) 2017-2018 ARM Limited
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
#include "psa_neg_server_part2_partition.h"

#define MSG_BUF_SIZE            128
#define CLIENT_RSP_BUF_SIZE     128
#define INVALID_INT_MASK        0xFFFFFFFF
#define MULTIPLE_SIGNUM         0x00000003
#define NUM_OF_FLAGS            24
#define INVALID_PARTION_ID      10


char msg_buf[MSG_BUF_SIZE];
char res_buff[MSG_BUF_SIZE];

void server_main2(void *ptr)
{
    psa_msg_t msg = {0};
    uint32_t signals = 0;
    memset(msg_buf, 0, sizeof(msg_buf));
    memset(res_buff, 0, sizeof(res_buff));

    while (true) {
        signals = psa_wait_any(PSA_BLOCK);
        if (signals & PART2_INT_MASK_MSK) {
            psa_wait_interrupt(INVALID_INT_MASK, PSA_BLOCK);
            TEST_FAIL_MESSAGE("server_interrupt_mask_invalid negative test failed");
        } else if (signals & PART2_GET_MSG_NULL_MSK) {
            psa_get(PART2_GET_MSG_NULL_MSK, NULL);
            TEST_FAIL_MESSAGE("server_get_msg_null negative test failed");
        } else if (signals & PART2_GET_SIGNUM_MULTIPLE_BIT_MSK) {
            psa_get(MULTIPLE_SIGNUM, &msg);
            TEST_FAIL_MESSAGE("server_get_multiple_bit_signum negative test failed");
        } else if (signals & PART2_GET_SIGNUM_NOT_SUBSET_MSK) {
            psa_get(PART2_GET_SIGNUM_NOT_SUBSET_MSK << NUM_OF_FLAGS, &msg);
            TEST_FAIL_MESSAGE("server_get_signum_not_subset negative test failed");
        } else if (signals & PART2_GET_SIGNUM_NOT_ACTIVE_MSK) {
            psa_get(PART2_GET_MSG_NULL_MSK, &msg);      //send wrong flag
            TEST_FAIL_MESSAGE("server_get_signum_not_active negative test failed");
        } else if (signals & PART2_GET_SIGNUM_TWICE_MSK) {
            psa_get(PART2_GET_SIGNUM_TWICE_MSK, &msg);
            psa_get(PART2_GET_SIGNUM_TWICE_MSK, &msg);
            TEST_FAIL_MESSAGE("server_get_signum_twice negative test failed");
        } else if (signals & PART2_READ_INVALID_HANDLE_MSK) {
            psa_get(PART2_READ_INVALID_HANDLE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    break;
                }
                case PSA_IPC_CALL: {
                    psa_handle_t invalid_handle = msg.handle  + 10;
                    psa_read(invalid_handle, 0, msg_buf, msg.in_size[0]);
                    TEST_FAIL_MESSAGE("server_read_invalid_handle negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_read_invalid_handle msg type failure");
                }
            }
            psa_reply(msg.handle, PSA_SUCCESS);
        } else if (signals & PART2_READ_NULL_HANDLE_MSK) {
            psa_get(PART2_READ_NULL_HANDLE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    break;
                }
                case PSA_IPC_CALL: {
                    psa_read(PSA_NULL_HANDLE, 0, msg_buf, msg.in_size[0]);
                    TEST_FAIL_MESSAGE("server_read_null_handle negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_read_null_handle msg type failure");
                }
            }
            psa_reply(msg.handle, PSA_SUCCESS);
        } else if (signals & PART2_READ_NULL_BUFFER_MSK) {
            psa_get(PART2_READ_NULL_BUFFER_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    break;
                }
                case PSA_IPC_CALL: {
                    psa_read(msg.handle, 0, NULL, msg.in_size[0]);
                    TEST_FAIL_MESSAGE("server_read_null_buffer negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_read_null_buffer msg type failure");
                }
            }
            psa_reply(msg.handle, PSA_SUCCESS);
        } else if (signals & PART2_WRITE_BUFFER_NULL_MSK) {
            psa_get(PART2_WRITE_BUFFER_NULL_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    break;
                }
                case PSA_IPC_CALL: {
                    size_t read_bytes = 0;
                    for (size_t i = 0; i < PSA_MAX_IOVEC - 1; i++) {
                        read_bytes += psa_read(msg.handle, i, msg_buf + read_bytes, msg.in_size[i]);
                    }

                    psa_write(msg.handle, 0, NULL, read_bytes);
                    TEST_FAIL_MESSAGE("server_write_null_buffer negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_write_null_buffer msg type failure");
                }
            }
            psa_reply(msg.handle, PSA_SUCCESS);
        } else if (signals & PART2_WRITE_RX_BUFF_NULL_MSK) {
            psa_get(PART2_WRITE_RX_BUFF_NULL_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    break;
                }
                case PSA_IPC_CALL: {
                    size_t read_bytes = 0;
                    for (size_t i = 0; i < PSA_MAX_IOVEC; i++) {
                        read_bytes += psa_read(msg.handle, i, msg_buf + read_bytes, msg.in_size[i]);
                    }

                    psa_write(msg.handle, 0, res_buff, read_bytes);
                    TEST_FAIL_MESSAGE("server_write_rx_buff_null negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_write_rx_buff_null msg type failure");
                }
            }
            psa_reply(msg.handle, PSA_SUCCESS);
        } else if (signals & PART2_WRITE_INVALID_HANDLE_MSK) {
            psa_get(PART2_WRITE_INVALID_HANDLE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    break;
                }
                case PSA_IPC_CALL: {
                    size_t read_bytes = 0;
                    for (size_t i = 0; i < PSA_MAX_IOVEC - 1; i++) {
                        read_bytes += psa_read(msg.handle, i, msg_buf + read_bytes, msg.in_size[i]);
                    }

                    psa_handle_t invalid_handle = msg.handle + 10;
                    psa_write(invalid_handle, 0, res_buff, read_bytes);
                    TEST_FAIL_MESSAGE("server_write_invalid_handle negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_write_invalid_handle msg type failure");
                }
            }
            psa_reply(msg.handle, PSA_SUCCESS);
        } else if (signals & PART2_WRITE_NULL_HANDLE_MSK) {
            psa_get(PART2_WRITE_NULL_HANDLE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    break;
                }
                case PSA_IPC_CALL: {
                    size_t read_bytes = 0;
                    for (size_t i = 0; i < PSA_MAX_IOVEC - 1; i++) {
                        read_bytes += psa_read(msg.handle, i, msg_buf + read_bytes, msg.in_size[i]);
                    }

                    psa_write(PSA_NULL_HANDLE, 0, res_buff, read_bytes);
                    TEST_FAIL_MESSAGE("server_write_null_handle negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_write_null_handle msg type failure");
                }
            }
            psa_reply(msg.handle, PSA_SUCCESS);
        } else if (signals & PART2_REPLY_INVALID_HANDLE_MSK) {
            psa_get(PART2_REPLY_INVALID_HANDLE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_reply_invalid_handle msg type failure");
                }
            }
            psa_handle_t invalid_handle = msg.handle  + 10;
            psa_reply(invalid_handle, PSA_SUCCESS);
            TEST_FAIL_MESSAGE("server_reply_invalid_handle negative test failed");
        } else if (signals & PART2_REPLY_NULL_HANDLE_MSK) {
            psa_get(PART2_REPLY_NULL_HANDLE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    psa_reply(PSA_NULL_HANDLE, PSA_CONNECTION_ACCEPTED);
                    TEST_FAIL_MESSAGE("server_reply_null_handle negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_reply_null_handle msg type failure");
                }
            }
        } else if (signals & PART2_NOTIFY_PART_ID_INVALID_MSK) {
            psa_get(PART2_NOTIFY_PART_ID_INVALID_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    psa_notify(INVALID_PARTION_ID);
                    TEST_FAIL_MESSAGE("server_notify_part_id_invalid negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_notify_part_id_invalid msg type failure");
                }
            }
            psa_reply(msg.handle, PSA_SUCCESS);
        } else if (signals & PART2_IDENTITY_INVALID_HANDLE_MSK) {
            psa_get(PART2_IDENTITY_INVALID_HANDLE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    int32_t ret = 0;
                    psa_handle_t invalid_handle = msg.handle  + 10;
                    ret = psa_identity(invalid_handle);
                    PSA_UNUSED(ret);
                    TEST_FAIL_MESSAGE("server_psa_identity_invalid_handle negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_psa_identity_invalid_handle msg type failure");
                }
            }
            psa_reply(msg.handle, PSA_SUCCESS);
        } else if (signals & PART2_IDENTITY_NULL_HANDLE_MSK) {
            psa_get(PART2_IDENTITY_NULL_HANDLE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    int32_t ret = 0;
                    ret = psa_identity(PSA_NULL_HANDLE);
                    PSA_UNUSED(ret);
                    TEST_FAIL_MESSAGE("server_psa_identity_null_handle negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_psa_identity_null_handle msg type failure");
                }
            }
            psa_reply(msg.handle, PSA_SUCCESS);
        } else if (signals & PART2_SET_RHANDLE_INVALID_HANDLE_MSK) {
            psa_get(PART2_SET_RHANDLE_INVALID_HANDLE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    psa_handle_t invalid_handle = msg.handle  + 10;
                    psa_set_rhandle(invalid_handle, NULL);
                    TEST_FAIL_MESSAGE("server_set_rhandle_invalid_handle negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_set_rhandle_invalid_handle msg type failure");
                }
            }
            psa_reply(msg.handle, PSA_SUCCESS);
        } else if (signals & PART2_SET_RHANDLE_NULL_HANDLE_MSK) {
            psa_get(PART2_SET_RHANDLE_NULL_HANDLE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    psa_set_rhandle(PSA_NULL_HANDLE, NULL);
                    TEST_FAIL_MESSAGE("server_set_rhandle_null_handle negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_set_rhandle_null_handle msg type failure");
                }
            }
            psa_reply(msg.handle, PSA_SUCCESS);
        } else if (signals & PART2_READ_WRAPAROUND_MSK) {
            psa_get(PART2_READ_WRAPAROUND_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    break;
                }
                case PSA_IPC_CALL: {
                    psa_read(msg.handle, 0, (void *)0x80000000, UINT32_MAX);
                    TEST_FAIL_MESSAGE("server_read_on_wraparound_msg_ptr negative test failed");
                    break;
                }

                default: {
                    TEST_FAIL_MESSAGE("server_read_on_wraparound_msg_ptr msg type failure");
                }
            }
            psa_reply(msg.handle, PSA_SUCCESS);
        } else if (signals & PART2_READ_EXCESS_INVEC_MSK) {
            psa_get(PART2_READ_EXCESS_INVEC_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    break;
                }
                case PSA_IPC_CALL: {
                    uint32_t val = 0;
                    psa_read(msg.handle, PSA_MAX_IOVEC, &val, sizeof(val));
                    TEST_FAIL_MESSAGE("server_read_on_wraparound_msg_ptr negative test failed");
                    break;
                }

                default: {
                    TEST_FAIL_MESSAGE("server_read_on_wraparound_msg_ptr msg type failure");
                }
            }
            psa_reply(msg.handle, PSA_SUCCESS);
        } else if (signals & PART2_WRITE_WRAPAROUND_MSK) {
            psa_get(PART2_WRITE_WRAPAROUND_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    break;
                }
                case PSA_IPC_CALL: {
                    psa_write(msg.handle, 0, (void *)0x80000000, UINT32_MAX);
                    TEST_FAIL_MESSAGE("server_write_on_wraparound_msg_ptr negative test failed");
                    break;
                }

                default: {
                    TEST_FAIL_MESSAGE("server_write_on_wraparound_msg_ptr msg type failure");
                }
            }
            psa_reply(msg.handle, PSA_SUCCESS);
        } else if (signals & PART2_WRITE_SIZE_OVERFLOW_MSK) {
            psa_get(PART2_WRITE_SIZE_OVERFLOW_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    break;
                }
                case PSA_IPC_CALL: {
                    uint8_t write_buf[CLIENT_RSP_BUF_SIZE + 1] = {0};
                    psa_write(msg.handle, 0, write_buf, sizeof(write_buf));
                    TEST_FAIL_MESSAGE("server_write_with_size_overflow negative test failed");
                    break;
                }

                default: {
                    TEST_FAIL_MESSAGE("server_write_with_size_overflow msg type failure");
                }
            }
            psa_reply(msg.handle, PSA_SUCCESS);
        } else if (signals & PART2_WRITE_EXCESS_OUTVEC_MSK) {
            psa_get(PART2_WRITE_EXCESS_OUTVEC_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_CONNECT: {
                    break;
                }
                case PSA_IPC_CALL: {
                    uint32_t val = 0;
                    psa_write(msg.handle, PSA_MAX_IOVEC, &val, sizeof(val));
                    TEST_FAIL_MESSAGE("server_write_from_excess_outvec negative test failed");
                    break;
                }

                default: {
                    TEST_FAIL_MESSAGE("server_write_from_excess_outvec msg type failure");
                }
            }
            psa_reply(msg.handle, PSA_SUCCESS);
        } else {
            SPM_PANIC("Unknown signal (0x%08x)", signals);
        }
    }
}

