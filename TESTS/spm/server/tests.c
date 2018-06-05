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
#include "spm_client.h"
#include "spm_server.h"
#include "spm_panic.h"
#include "psa_server_test_part1_partition.h"
#include "psa_server_test_part2_ifs.h"
#include "server_tests.h"

/**
 * Proccess a generic connect message to TEST SF.
 * @return PSA_SUCCESS or negative error code if failed.
 */
static psa_error_t proccess_connect_request(void)
{
    psa_error_t res = PSA_SUCCESS;
    psa_msg_t msg = {0};
    uint32_t signals = psa_wait_any(PSA_WAIT_BLOCK);
    if ((signals & TEST_MSK) == 0) {
        res = PSA_GENERIC_ERROR;
    }

    psa_get(TEST_MSK, &msg);
    if (msg.type != PSA_IPC_MSG_TYPE_CONNECT) {
        res = ((res != PSA_SUCCESS) ? res : PSA_GENERIC_ERROR);
    }

    psa_end(msg.handle, res);

    return res;
}

/**
 * Proccess a generic disconnect message to TEST SF.
 * @return PSA_SUCCESS or negative error code if failed.
 */
static psa_error_t proccess_disconnect_request(void)
{
    psa_error_t res = PSA_SUCCESS;
    psa_msg_t msg = {0};
    uint32_t signals = psa_wait_any(PSA_WAIT_BLOCK);
    if ((signals & TEST_MSK) == 0) {
        res = PSA_GENERIC_ERROR;
    }

    psa_get(TEST_MSK, &msg);
    if (msg.type != PSA_IPC_MSG_TYPE_DISCONNECT) {
        res = ((res != PSA_SUCCESS) ? res : PSA_GENERIC_ERROR);
    }

    return res;
}

PSA_TEST_SERVER(wait_timeout)
{
    psa_error_t test_status = PSA_SUCCESS;
    uint32_t signals = psa_wait_any(7);
    *status_ptr = ((signals & TEST_MSK) == 0) ? PSA_SUCCESS : PSA_GENERIC_ERROR;;

    test_status = proccess_connect_request();
    if (test_status != PSA_SUCCESS) {
        return test_status;
    }
    test_status = proccess_disconnect_request();
    if (test_status != PSA_SUCCESS) {
        return test_status;
    }

    return ((signals & TEST_MSK) == 0) ? PSA_SUCCESS : PSA_GENERIC_ERROR;
}

PSA_TEST_SERVER(identity_during_connect)
{
    psa_error_t test_status = PSA_SUCCESS;
    psa_error_t disconnect_status = PSA_SUCCESS;
    psa_msg_t msg = {0};
    int32_t identity = 0;

    uint32_t signals = psa_wait_any(PSA_WAIT_BLOCK);
    if ((signals & TEST_MSK) == 0) {
        test_status = PSA_GENERIC_ERROR;
    }

    psa_get(TEST_MSK, &msg);
    if (msg.type != PSA_IPC_MSG_TYPE_CONNECT) {
        test_status = ((test_status != PSA_SUCCESS) ? test_status : PSA_GENERIC_ERROR);
    }

    identity = psa_identity(msg.handle);
    *status_ptr = (identity == -1) ? PSA_SUCCESS : PSA_GENERIC_ERROR;

    psa_end(msg.handle, PSA_SUCCESS);

    disconnect_status = proccess_disconnect_request();
    test_status  = (test_status != PSA_SUCCESS) ? test_status : disconnect_status;

    return test_status;
}

PSA_TEST_SERVER(identity_during_call)
{
    psa_error_t test_status = PSA_SUCCESS;
    psa_error_t disconnect_status = PSA_SUCCESS;
    psa_msg_t msg = {0};
    int32_t identity = 0;
    uint32_t signals = 0;

    test_status = proccess_connect_request();
    if (test_status != PSA_SUCCESS) {
        return test_status;
    }

    signals = psa_wait_any(PSA_WAIT_BLOCK);
    if ((signals & TEST_MSK) == 0) {
        test_status = PSA_GENERIC_ERROR;
    }

    psa_get(TEST_MSK, &msg);
    if (msg.type != PSA_IPC_MSG_TYPE_CALL) {
        test_status = ((test_status != PSA_SUCCESS) ? test_status : PSA_GENERIC_ERROR);
    }

    identity = psa_identity(msg.handle);
    *status_ptr = (identity == -1) ? PSA_SUCCESS : PSA_GENERIC_ERROR;

    psa_end(msg.handle, PSA_SUCCESS);

    disconnect_status = proccess_disconnect_request();
    test_status  = (test_status != PSA_SUCCESS) ? test_status : disconnect_status;

    return test_status;
}

PSA_TEST_SERVER(msg_size_assertion)
{
    psa_error_t test_status = PSA_SUCCESS;
    psa_error_t disconnect_status = PSA_SUCCESS;
    psa_msg_t msg = {0};
    uint32_t signals = 0;
    size_t read_size = 0;

    char *buff = malloc(sizeof(char) * 11);
    if (NULL == buff) {
        SPM_PANIC("memory allocation failure\n");
    }
    memset(buff, 0, 11);

    test_status = proccess_connect_request();
    if (test_status != PSA_SUCCESS) {
        free(buff);
        return test_status;
    }

    signals = psa_wait_any(PSA_WAIT_BLOCK);
    if ((signals & TEST_MSK) == 0) {
        test_status = PSA_GENERIC_ERROR;
    }

    psa_get(TEST_MSK, &msg);
    if (msg.type != PSA_IPC_MSG_TYPE_CALL) {
        test_status = ((test_status != PSA_SUCCESS) ? test_status : PSA_GENERIC_ERROR);
    }
    for (size_t i = 0; i < PSA_MAX_INVEC_LEN; i++) {
        read_size += psa_read(msg.handle, i, buff + read_size, msg.in_size[i]);
    }

    if (((msg.in_size[0] + msg.in_size[1] + msg.in_size[2]) != 11)  ||
        (read_size != 11) ||
        (strncmp(buff, "abcdfghijkn", 11) != 0)) {
        *status_ptr = PSA_GENERIC_ERROR;
    } else {
        *status_ptr = PSA_SUCCESS;
    }

    psa_end(msg.handle, test_status);
    free(buff);
    disconnect_status = proccess_disconnect_request();
    test_status  = (test_status != PSA_SUCCESS) ? test_status : disconnect_status;
    return test_status;
}

PSA_TEST_SERVER(reject_connection)
{
    psa_error_t res = PSA_SUCCESS;
    psa_msg_t msg = {0};
    uint32_t signals = psa_wait_any(PSA_WAIT_BLOCK);
    if ((signals & TEST_MSK) == 0) {
        res = PSA_GENERIC_ERROR;
    }

    psa_get(TEST_MSK, &msg);
    if (msg.type != PSA_IPC_MSG_TYPE_CONNECT) {
        res = ((res != PSA_SUCCESS) ? res : PSA_GENERIC_ERROR);
    }

    psa_end(msg.handle, PSA_CONNECTION_REFUSED_PERM);
    *status_ptr = res;
    return res;
}

PSA_TEST_SERVER(read_at_outofboud_offset)
{
    uint32_t signals = 0;
    psa_msg_t msg = {0};
    psa_error_t test_status = PSA_SUCCESS;
    psa_error_t disconnect_status = PSA_SUCCESS;
    uint32_t buff = 52;

    test_status = proccess_connect_request();
    if (test_status != PSA_SUCCESS) {
        return test_status;
    }

    signals = psa_wait_any(PSA_WAIT_BLOCK);
    if ((signals & TEST_MSK) == 0) {
        test_status = PSA_GENERIC_ERROR;
    }

    psa_get(TEST_MSK, &msg);
    if (msg.type != PSA_IPC_MSG_TYPE_CALL) {
        test_status = ((test_status != PSA_SUCCESS) ? test_status : PSA_GENERIC_ERROR);
    }

    size_t read_size = psa_read(msg.handle, 1, &buff, sizeof(buff));
    if ((0 != read_size) || (52 != buff)){
        *status_ptr = PSA_GENERIC_ERROR;
    } else {
        *status_ptr = PSA_SUCCESS;
    }

    psa_end(msg.handle, test_status);
    disconnect_status = proccess_disconnect_request();
    test_status  = (test_status != PSA_SUCCESS) ? test_status : disconnect_status;
    return test_status;
}


PSA_TEST_SERVER(msg_read_truncation)
{
    psa_error_t test_status = PSA_SUCCESS;
    psa_error_t disconnect_status = PSA_SUCCESS;
    psa_msg_t msg = {0};
    uint32_t signals = 0;
    size_t read_size = 0;
    char *buff = malloc(sizeof(char) * 11);
    if (NULL == buff) {
        SPM_PANIC("memory allocation failure\n");
    }
    memset(buff, 0, 11);

    test_status = proccess_connect_request();
    if (test_status != PSA_SUCCESS) {
        free(buff);
        return test_status;
    }

    signals = psa_wait_any(PSA_WAIT_BLOCK);
    if ((signals & TEST_MSK) == 0) {
        test_status = PSA_GENERIC_ERROR;
    }

    psa_get(TEST_MSK, &msg);
    if (msg.type != PSA_IPC_MSG_TYPE_CALL) {
        test_status = ((test_status != PSA_SUCCESS) ? test_status : PSA_GENERIC_ERROR);
    }

    read_size = psa_read(msg.handle, 1, buff, 11);
    if ((msg.in_size[1] != read_size) ||
        ((msg.in_size[0] + msg.in_size[1] + msg.in_size[2]) != 11) ||
        (buff[6] != 0)   ||
        (strncmp(buff, "fghijk", 6) != 0)) {
        *status_ptr = PSA_GENERIC_ERROR;
    } else {
        *status_ptr = PSA_SUCCESS;
    }

    psa_end(msg.handle, test_status);
    disconnect_status = proccess_disconnect_request();
    free(buff);
    test_status  = (test_status != PSA_SUCCESS) ? test_status : disconnect_status;
    return test_status;
}

PSA_TEST_SERVER(skip_zero)
{
    psa_error_t test_status = PSA_SUCCESS;
    psa_error_t disconnect_status = PSA_SUCCESS;
    psa_msg_t msg = {0};
    uint32_t signals = 0;
    size_t read_size = 0;
    size_t skip_size = 0;
    char *buff = malloc(sizeof(char) * 11);
    if (NULL == buff) {
        SPM_PANIC("memory allocation failure\n");
    }

    test_status = proccess_connect_request();
    if (test_status != PSA_SUCCESS) {
        free(buff);
        return test_status;
    }

    signals = psa_wait_any(PSA_WAIT_BLOCK);
    if ((signals & TEST_MSK) == 0) {
        test_status = PSA_GENERIC_ERROR;
    }

    psa_get(TEST_MSK, &msg);
    if (msg.type != PSA_IPC_MSG_TYPE_CALL) {
        test_status = ((test_status != PSA_SUCCESS) ? test_status : PSA_GENERIC_ERROR);
    }

    skip_size = psa_skip(msg.handle, 0, 0);
    read_size = psa_read(msg.handle, 0, buff, 11);
    if ((skip_size != 0) ||
        (read_size != 11) ||
        (strncmp(buff, "abcdefghijk", 11)) != 0) {
        *status_ptr = PSA_GENERIC_ERROR;
    } else {
        *status_ptr = PSA_SUCCESS;
    }

    psa_end(msg.handle, test_status);
    disconnect_status = proccess_disconnect_request();
    free(buff);
    test_status  = (test_status != PSA_SUCCESS) ? test_status : disconnect_status;
    return test_status;
}

PSA_TEST_SERVER(skip_some)
{
    psa_error_t test_status = PSA_SUCCESS;
    psa_error_t disconnect_status = PSA_SUCCESS;
    psa_msg_t msg = {0};
    uint32_t signals = 0;
    size_t read_size1 = 0;
    size_t read_size2 = 0;
    size_t skip_size = 0;
    char *buff = malloc(sizeof(char) * 11);
    if (NULL == buff) {
        SPM_PANIC("memory allocation failure\n");
    }

    test_status = proccess_connect_request();
    if (test_status != PSA_SUCCESS) {
        free(buff);
        return test_status;
    }

    signals = psa_wait_any(PSA_WAIT_BLOCK);
    if ((signals & TEST_MSK) == 0) {
        test_status = PSA_GENERIC_ERROR;
    }

    psa_get(TEST_MSK, &msg);
    if (msg.type != PSA_IPC_MSG_TYPE_CALL) {
        test_status = ((test_status != PSA_SUCCESS) ? test_status : PSA_GENERIC_ERROR);
    }

    read_size1 = psa_read(msg.handle, 0, buff, 3);
    skip_size = psa_skip(msg.handle, 0, 5);
    read_size2 = psa_read(msg.handle, 0, buff + 3, 8);
    if ((read_size1 != 3) ||
        (skip_size != 5) ||
        (read_size2 != 8) ||
        (strncmp(buff, "abcijklmnop", 11) != 0)) {
        *status_ptr = PSA_GENERIC_ERROR;
    } else {
        *status_ptr = PSA_SUCCESS;
    }

    psa_end(msg.handle, test_status);
    disconnect_status = proccess_disconnect_request();
    free(buff);
    test_status  = (test_status != PSA_SUCCESS) ? test_status : disconnect_status;
    return test_status;
}

PSA_TEST_SERVER(skip_more_than_left)
{
    psa_error_t test_status = PSA_SUCCESS;
    psa_error_t disconnect_status = PSA_SUCCESS;
    psa_msg_t msg = {0};
    uint32_t signals = 0;
    size_t read_size1 = 0;
    size_t read_size2 = 0;
    size_t skip_size = 0;
    char *buff = malloc(sizeof(char) * 8);
    if (NULL == buff) {
        SPM_PANIC("memory allocation failure\n");
    }

    test_status = proccess_connect_request();
    if (test_status != PSA_SUCCESS) {
        free(buff);
        return test_status;
    }

    signals = psa_wait_any(PSA_WAIT_BLOCK);
    if ((signals & TEST_MSK) == 0) {
        test_status = PSA_GENERIC_ERROR;
    }

    psa_get(TEST_MSK, &msg);
    if (msg.type != PSA_IPC_MSG_TYPE_CALL) {
        test_status = ((test_status != PSA_SUCCESS) ? test_status : PSA_GENERIC_ERROR);
    }

    read_size1 = psa_read(msg.handle, 0, buff, 5);
    skip_size = psa_skip(msg.handle, 0, 4);
    read_size2 = psa_read(msg.handle, 0, buff + 5, 2);
    if ((read_size1 != 5) ||
        (skip_size != 3) ||
        (read_size2 != 0) ||
        (strncmp(buff, "abcde", 5) != 0)) {
        *status_ptr = PSA_GENERIC_ERROR;
    } else {
        *status_ptr = PSA_SUCCESS;
    }

    psa_end(msg.handle, test_status);
    disconnect_status = proccess_disconnect_request();
    free(buff);
    test_status  = (test_status != PSA_SUCCESS) ? test_status : disconnect_status;
    return test_status;
}

PSA_TEST_SERVER(rhandle_factorial)
{
    uint32_t signals = 0;
    psa_msg_t msg = {0};
    factorial_data_t *num = NULL;
    factorial_data_t *asserted_ptr = NULL;
    uint32_t connect_count = 0;
    uint32_t call_count = 0;
    uint32_t disconnect_count = 0;

    while (1) {
        signals = psa_wait_any(PSA_WAIT_BLOCK);
        if (0 == (signals & TEST_MSK)) {
            SPM_PANIC("returned from psa_wait_any without SF_FACTORIAL bit on\n");
        }

        psa_get(TEST_MSK, &msg);
        switch (msg.type) {
        case PSA_IPC_MSG_TYPE_CONNECT:
            if (NULL != msg.rhandle){
                SPM_PANIC("got rhandle on connect message\n");
            }

            num = (factorial_data_t *)malloc(sizeof(factorial_data_t));
            if (NULL == num) {
                SPM_PANIC("memory allocation failure\n");
            }
            num->count = 0;
            num->val = 1;
            psa_set_rhandle(msg.handle, num);
            asserted_ptr = num;
            connect_count++;
            break;
        case PSA_IPC_MSG_TYPE_CALL:
            if (msg.in_size[0] + msg.in_size[1] + msg.in_size[2] > 0) {
                SPM_PANIC("SF_FACTORIAL SF should not get any params\n");
            }

            if (NULL == msg.rhandle){
                SPM_PANIC("got NULL rhandle on call message\n");
            }

            if (asserted_ptr != msg.rhandle){
                SPM_PANIC("rhandle value changed between calls\n");
            }

            num = (factorial_data_t *)msg.rhandle;
            num->count++;
            num->val *= num->count;
            psa_write(msg.handle, 0, &(num->val), sizeof(num->val));
            call_count++;
            break;
        case PSA_IPC_MSG_TYPE_DISCONNECT:
            if (NULL == msg.rhandle){
                SPM_PANIC("got NULL rhandle on disconnect message\n");
            }

            if (asserted_ptr != msg.rhandle){
                SPM_PANIC("rhandle value changed between calls\n");
            }

            free(msg.rhandle);
            disconnect_count++;
            break;
        default:
            SPM_PANIC("Unexpected message type %d!", (int)(msg.type));
        }

        num = NULL;
        psa_end(msg.handle, PSA_SUCCESS);
        if (disconnect_count > 0) {
            break;
        }
    }

    if ((connect_count != 1) ||
        (call_count != 5)    ||
        (disconnect_count != 1)) {
        *status_ptr = PSA_GENERIC_ERROR;
    } else {
        *status_ptr = PSA_SUCCESS;
    }

    return *status_ptr;
}

PSA_TEST_SERVER(cross_partition_call)
{
    uint32_t signals = 0;
    psa_msg_t msg = {0};
    psa_error_t test_status = PSA_SUCCESS;
    psa_error_t disconnect_status = PSA_SUCCESS;
    psa_error_t partition_call_status = PSA_SUCCESS;
    uint32_t data_read = 0;
    uint32_t str_len = 0;
    char *buff = malloc(sizeof(char) * 60);

    if (NULL == buff) {
        SPM_PANIC("memory allocation failure\n");
    }

    memset(buff, 0, 60);

    test_status = proccess_connect_request();
    if (test_status != PSA_SUCCESS) {
        free(buff);
        return test_status;
    }

    signals = psa_wait_any(PSA_WAIT_BLOCK);
    if ((signals & TEST_MSK) == 0) {
        test_status = PSA_GENERIC_ERROR;
    }

    psa_get(TEST_MSK, &msg);
    if ((msg.in_size[0] + msg.in_size[1] + msg.in_size[2]) == 0) {
        test_status = ((test_status != PSA_SUCCESS) ? test_status : PSA_GENERIC_ERROR);
    }

    str_len = msg.in_size[0];
    data_read = psa_read(msg.handle, 0, buff, str_len);
    if (data_read != 21) {
        test_status = ((test_status != PSA_SUCCESS) ? test_status : PSA_GENERIC_ERROR);
    }

    memcpy(buff + str_len, buff, str_len);
    data_read *= 2;

    psa_invec_t data = { buff, data_read };

    psa_outvec_t resp = { buff, data_read };
    psa_handle_t conn_handle = psa_connect(SF_REVERSE, 5);
    if (conn_handle <= 0) {
        partition_call_status = PSA_GENERIC_ERROR;
    }

    if (partition_call_status == PSA_SUCCESS) {
        partition_call_status = psa_call(conn_handle, &data, 1, &resp, 1);
    }

    *status_ptr = partition_call_status;
    if (partition_call_status == PSA_SUCCESS) {
        psa_close(conn_handle);
    }

    if (PSA_SUCCESS == partition_call_status) {
        psa_write(msg.handle, 0, buff, data_read);
    }

    psa_end(msg.handle, partition_call_status);
    free(buff);
    disconnect_status = proccess_disconnect_request();
    test_status  = (test_status != PSA_SUCCESS) ? test_status : disconnect_status;
    return test_status;
}

// Test a common DOORBELL scenario
PSA_TEST_SERVER(doorbell_test)
{
    uint32_t signals = 0;
    psa_msg_t msg = {0};
    psa_error_t test_status = PSA_SUCCESS;
    psa_error_t disconnect_status = PSA_SUCCESS;
    psa_error_t partition_call_status = PSA_SUCCESS;


    test_status = proccess_connect_request();
    if (test_status != PSA_SUCCESS) {
        return test_status;
    }

    signals = psa_wait_any(PSA_WAIT_BLOCK);
    if ((signals & TEST_MSK) == 0) {
        test_status = PSA_GENERIC_ERROR;
    }

    psa_get(TEST_MSK, &msg);
    if (((msg.in_size[0] + msg.in_size[1] + msg.in_size[2]) != 0) || (msg.out_size[0] != 0)) {
        test_status = ((test_status != PSA_SUCCESS) ? test_status : PSA_GENERIC_ERROR);
    }

    // -- Connection with partition2 - START
    psa_handle_t conn_handle = psa_connect(SF_DB_TST, 5);
    if (conn_handle <= 0) {
        partition_call_status = PSA_GENERIC_ERROR;
    }

    if (partition_call_status == PSA_SUCCESS) {
        partition_call_status = psa_call(conn_handle, NULL, 0, NULL, 0);
    }

    if (partition_call_status == PSA_SUCCESS) {
        // Wait for doorball notification - Only after that call psa_end() for the client called you
        signals = psa_wait_interrupt(SERVER_TEST_PART1_DOORBELL_MSK, PSA_WAIT_BLOCK);
        if ((signals & SERVER_TEST_PART1_DOORBELL_MSK) == 0) {
            partition_call_status = PSA_GENERIC_ERROR;
        }
    }

    if (partition_call_status == PSA_SUCCESS) {
        psa_clear();
        psa_close(conn_handle);
    }
    // -- Connection with partition2 - END

    *status_ptr = partition_call_status;

    psa_end(msg.handle, partition_call_status);
    disconnect_status = proccess_disconnect_request();

    test_status  = (test_status != PSA_SUCCESS) ? test_status : disconnect_status;

    return test_status;
}

PSA_TEST_SERVER(psa_end_on_NULL_HANDLE)
{
    psa_error_t test_status = PSA_SUCCESS;
    psa_error_t disconnect_status = PSA_SUCCESS;
    psa_msg_t msg = {0};

    uint32_t signals = psa_wait_any(PSA_WAIT_BLOCK);
    if ((signals & TEST_MSK) == 0) {
        test_status = PSA_GENERIC_ERROR;
    }

    psa_get(TEST_MSK, &msg);
    if (msg.type != PSA_IPC_MSG_TYPE_CONNECT) {
        test_status = ((test_status != PSA_SUCCESS) ? test_status : PSA_GENERIC_ERROR);
    }

    *status_ptr = (msg.handle != PSA_NULL_HANDLE) ? PSA_SUCCESS : PSA_GENERIC_ERROR;

    psa_end(PSA_NULL_HANDLE, PSA_SUCCESS);
    psa_end(msg.handle, PSA_SUCCESS);

    disconnect_status = proccess_disconnect_request();
    test_status  = (test_status != PSA_SUCCESS) ? test_status : disconnect_status;

    return test_status;
}


psa_test_server_side_func test_list[] = {
    PSA_TEST_SERVER_NAME(wait_timeout),
    PSA_TEST_SERVER_NAME(identity_during_connect),
    PSA_TEST_SERVER_NAME(identity_during_call),
    PSA_TEST_SERVER_NAME(msg_size_assertion),
    PSA_TEST_SERVER_NAME(reject_connection),
    PSA_TEST_SERVER_NAME(read_at_outofboud_offset),
    PSA_TEST_SERVER_NAME(msg_read_truncation),
    PSA_TEST_SERVER_NAME(skip_zero),
    PSA_TEST_SERVER_NAME(skip_some),
    PSA_TEST_SERVER_NAME(skip_more_than_left),
    PSA_TEST_SERVER_NAME(rhandle_factorial),
    PSA_TEST_SERVER_NAME(cross_partition_call),
    PSA_TEST_SERVER_NAME(doorbell_test),
    PSA_TEST_SERVER_NAME(psa_end_on_NULL_HANDLE),
    NULL
};
