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
#include "unity.h"
#include "spm_panic.h"
#include "spm_server.h"
#include "psa_part2_partition.h"

#define MSG_BUF_SIZE            128
#define CLIENT_RSP_BUF_SIZE     128
#define INVALID_INT_MASK        0xFFFFFFFF
#define MULTIPLE_SIGNUM         0x00000003
#define NUM_OF_FLAGS            24
#define INVALID_PARTION_ID      10
#define FALSE_RHANDLE           10


void server_main2(void *ptr)
{
    psa_msg_t msg = {0};
    uint32_t signals = 0;
    char msg_buf[MSG_BUF_SIZE] =  {0};
    char res_buff[MSG_BUF_SIZE] = {0};

    while (true) {
        signals = psa_wait_any(PSA_WAIT_BLOCK);
        if (signals & PART2_INT_MASK_MSK) {
            psa_wait_interrupt(INVALID_INT_MASK, PSA_WAIT_BLOCK);
            TEST_FAIL_MESSAGE("server_interrupt_mask_invalid negative test failed");
        }
        else if (signals & PART2_GET_MSG_NULL_MSK) {
            psa_get(PART2_GET_MSG_NULL_MSK, NULL);
            TEST_FAIL_MESSAGE("server_get_msg_null negative test failed");
        }
        else if (signals & PART2_GET_SIGNUM_MULTIPLE_BIT_MSK) {
            psa_get(MULTIPLE_SIGNUM, &msg);
            TEST_FAIL_MESSAGE("server_get_multiple_bit_signum negative test failed");
        }
        else if (signals & PART2_GET_SIGNUM_NOT_SUBSET_MSK) {
            psa_get(PART2_GET_SIGNUM_NOT_SUBSET_MSK << NUM_OF_FLAGS, &msg);
            TEST_FAIL_MESSAGE("server_get_signum_not_subset negative test failed");
        }
        else if (signals & PART2_GET_SIGNUM_NOT_ACTIVE_MSK) {
            psa_get(PART2_GET_MSG_NULL_MSK, &msg);      //send wrong flag
            TEST_FAIL_MESSAGE("server_get_signum_not_active negative test failed");
        }
        else if (signals & PART2_READ_INVALID_HANDLE_MSK) {
            psa_get(PART2_READ_INVALID_HANDLE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_MSG_TYPE_CONNECT: {
                    break;
                }
                case PSA_IPC_MSG_TYPE_CALL: {
                    psa_handle_t invalid_handle = msg.handle  + 10;
                    psa_read(invalid_handle, 0, msg_buf, msg.size[0]);
                    TEST_FAIL_MESSAGE("server_read_invalid_handle negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_read_invalid_handle msg type failure");
                }
            }
            psa_end(msg.handle, PSA_SUCCESS);
        }
        else if (signals & PART2_READ_NULL_HANDLE_MSK) {
            psa_get(PART2_READ_NULL_HANDLE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_MSG_TYPE_CONNECT: {
                    break;
                }
                case PSA_IPC_MSG_TYPE_CALL: {
                    psa_read(PSA_NULL_HANDLE, 0, msg_buf, msg.size[0]);
                    TEST_FAIL_MESSAGE("server_read_null_handle negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_read_null_handle msg type failure");
                }
            }
            psa_end(msg.handle, PSA_SUCCESS);
        }
        else if (signals & PART2_WRITE_BUFFER_NULL_MSK) {
            psa_get(PART2_WRITE_BUFFER_NULL_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_MSG_TYPE_CONNECT: {
                    break;
                }
                case PSA_IPC_MSG_TYPE_CALL: {
                    size_t read_bytes = 0;
                    for (size_t i = 0; i< PSA_MAX_INVEC_LEN; i++) {
                        read_bytes += psa_read(msg.handle, i, msg_buf + read_bytes, msg.size[i]);
                    }

                    psa_write(msg.handle, 0, NULL, read_bytes);
                    TEST_FAIL_MESSAGE("server_write_null_buffer negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_write_null_buffer msg type failure");
                }
            }
            psa_end(msg.handle, PSA_SUCCESS);
        }
        else if (signals & PART2_WRITE_OFFSET_BIGGER_MAX_MSK) {
            psa_get(PART2_WRITE_OFFSET_BIGGER_MAX_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_MSG_TYPE_CONNECT: {
                    break;
                }
                case PSA_IPC_MSG_TYPE_CALL: {
                    size_t read_bytes = 0;
                    for (size_t i = 0; i< PSA_MAX_INVEC_LEN; i++) {
                        read_bytes += psa_read(msg.handle, i, msg_buf + read_bytes, msg.size[i]);
                    }

                    psa_write(msg.handle, UINT32_MAX, res_buff, read_bytes);
                    TEST_FAIL_MESSAGE("server_write_offset_bigger_than_max_value negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_write_offset_bigger_than_max_value msg type failure");
                }
            }
            psa_end(msg.handle, PSA_SUCCESS);
        }
        else if (signals & PART2_WRITE_OFFSET_BIGGER_RX_SIZE_MSK) {
            psa_get(PART2_WRITE_OFFSET_BIGGER_RX_SIZE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_MSG_TYPE_CONNECT: {
                    break;
                }
                case PSA_IPC_MSG_TYPE_CALL: {
                    size_t read_bytes = 0;
                    for (size_t i = 0; i< PSA_MAX_INVEC_LEN; i++) {
                        read_bytes += psa_read(msg.handle, i, msg_buf + read_bytes, msg.size[i]);
                    }

                    psa_write(msg.handle, CLIENT_RSP_BUF_SIZE, res_buff, 1);
                    TEST_FAIL_MESSAGE("server_write_offset_bigger_than_rx_size negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_write_offset_bigger_than_rx_size msg type failure");
                }
            }
            psa_end(msg.handle, PSA_SUCCESS);
        }
        else if (signals & PART2_WRITE_RX_BUFF_NULL_MSK) {
            psa_get(PART2_WRITE_RX_BUFF_NULL_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_MSG_TYPE_CONNECT: {
                    break;
                }
                case PSA_IPC_MSG_TYPE_CALL: {
                    size_t read_bytes = 0;
                    for (size_t i = 0; i< PSA_MAX_INVEC_LEN; i++) {
                        read_bytes += psa_read(msg.handle, i, msg_buf + read_bytes, msg.size[i]);
                    }

                    psa_write(msg.handle, 0, res_buff, read_bytes);
                    TEST_FAIL_MESSAGE("server_write_rx_buff_null negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_write_rx_buff_null msg type failure");
                }
            }
            psa_end(msg.handle, PSA_SUCCESS);
        }
        else if (signals & PART2_WRITE_INVALID_HANDLE_MSK) {
            psa_get(PART2_WRITE_INVALID_HANDLE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_MSG_TYPE_CONNECT: {
                    break;
                }
                case PSA_IPC_MSG_TYPE_CALL: {
                    size_t read_bytes = 0;
                    for (size_t i = 0; i< PSA_MAX_INVEC_LEN; i++) {
                        read_bytes += psa_read(msg.handle, i, msg_buf + read_bytes, msg.size[i]);
                    }

                    psa_handle_t invalid_handle = msg.handle  + 10;
                    psa_write(invalid_handle, 0, res_buff, read_bytes);
                    TEST_FAIL_MESSAGE("server_write_invalid_handle negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_write_invalid_handle msg type failure");
                }
            }
            psa_end(msg.handle, PSA_SUCCESS);
        }
        else if (signals & PART2_WRITE_NULL_HANDLE_MSK) {
            psa_get(PART2_WRITE_NULL_HANDLE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_MSG_TYPE_CONNECT: {
                    break;
                }
                case PSA_IPC_MSG_TYPE_CALL: {
                    size_t read_bytes = 0;
                    for (size_t i = 0; i< PSA_MAX_INVEC_LEN; i++) {
                        read_bytes += psa_read(msg.handle, i, msg_buf + read_bytes, msg.size[i]);
                    }

                    psa_write(PSA_NULL_HANDLE, 0, res_buff, read_bytes);
                    TEST_FAIL_MESSAGE("server_write_null_handle negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_write_null_handle msg type failure");
                }
            }
            psa_end(msg.handle, PSA_SUCCESS);
        }
        else if (signals & PART2_END_INVALID_HANDLE_MSK) {
            psa_get(PART2_END_INVALID_HANDLE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_MSG_TYPE_CONNECT: {
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_end_invalid_handle msg type failure");
                }
            }
            psa_handle_t invalid_handle = msg.handle  + 10;
            psa_end(invalid_handle, PSA_SUCCESS);
            TEST_FAIL_MESSAGE("server_end_invalid_handle negative test failed");
        }
        else if (signals & PART2_END_NULL_HANDLE_MSK) {
            psa_get(PART2_END_NULL_HANDLE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_MSG_TYPE_CONNECT: {
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_end_null_handle msg type failure");
                }
            }
            psa_end(PSA_NULL_HANDLE, PSA_SUCCESS);
            TEST_FAIL_MESSAGE("server_end_null_handle negative test failed");
        }
        else if (signals & PART2_SET_RHANDLE_INVALID_MSK) {
            bool is_disconnect = false;
            psa_get(PART2_SET_RHANDLE_INVALID_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_MSG_TYPE_CONNECT: {
                    break;
                }
                case PSA_IPC_MSG_TYPE_DISCONNECT: {
                    uint32_t false_rhandle = FALSE_RHANDLE;
                    psa_set_rhandle(msg.handle, &false_rhandle);
                    is_disconnect = true;
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_end_rhandle_invalid msg type failure");
                }
            }
            psa_end(msg.handle, PSA_SUCCESS);
            if (is_disconnect) {
                TEST_FAIL_MESSAGE("server_end_rhandle_invalid negative test failed");
            }
        }
        else if (signals & PART2_NOTIFY_PART_ID_INVALID_MSK) {
            psa_get(PART2_NOTIFY_PART_ID_INVALID_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_MSG_TYPE_CONNECT: {
                    psa_notify(INVALID_PARTION_ID);
                    TEST_FAIL_MESSAGE("server_notify_part_id_invalid negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_notify_part_id_invalid msg type failure");
                }
            }
            psa_end(msg.handle, PSA_SUCCESS);
        }
        else if (signals & PART2_IDENTITY_INVALID_HANDLE_MSK) {
            psa_get(PART2_IDENTITY_INVALID_HANDLE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_MSG_TYPE_CONNECT: {
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
            psa_end(msg.handle, PSA_SUCCESS);
        }
        else if (signals & PART2_IDENTITY_NULL_HANDLE_MSK) {
            psa_get(PART2_IDENTITY_NULL_HANDLE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_MSG_TYPE_CONNECT: {
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
            psa_end(msg.handle, PSA_SUCCESS);
        }
        else if (signals & PART2_SET_RHANDLE_INVALID_HANDLE_MSK) {
            psa_get(PART2_SET_RHANDLE_INVALID_HANDLE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_MSG_TYPE_CONNECT: {
                    psa_handle_t invalid_handle = msg.handle  + 10;
                    psa_set_rhandle(invalid_handle, NULL);
                    TEST_FAIL_MESSAGE("server_set_rhandle_invalid_handle negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_set_rhandle_invalid_handle msg type failure");
                }
            }
            psa_end(msg.handle, PSA_SUCCESS);
        }
        else if (signals & PART2_SET_RHANDLE_NULL_HANDLE_MSK) {
            psa_get(PART2_SET_RHANDLE_NULL_HANDLE_MSK, &msg);
            switch (msg.type) {
                case PSA_IPC_MSG_TYPE_CONNECT: {
                    psa_set_rhandle(PSA_NULL_HANDLE, NULL);
                    TEST_FAIL_MESSAGE("server_set_rhandle_part_id_invalid negative test failed");
                    break;
                }
                default: {
                    TEST_FAIL_MESSAGE("server_set_rhandle_part_id_invalid msg type failure");
                }
            }
            psa_end(msg.handle, PSA_SUCCESS);
        }
        else {
            SPM_ASSERT(false);
        }
    }

    return;
}
