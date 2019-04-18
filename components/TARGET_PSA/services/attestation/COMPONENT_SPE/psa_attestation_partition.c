/*
* Copyright (c) 2018-2019 ARM Limited. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the License); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an AS IS BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

// ---------------------------------- Includes ---------------------------------
#include "psa/client.h"
#include "psa/service.h"

#include "mbed_spm_partitions.h"
#include "psa_initial_attestation_api.h"
#include "psa_attest_inject_key.h"
#include "psa_inject_attestation_key_impl.h"
#include "attestation.h"
#include <stdlib.h>
#include <string.h>
#include "psa/crypto.h"

int32_t g_caller_id = 0;

static void set_caller_id(psa_msg_t *msg)
{
    g_caller_id = msg->client_id;
}

// ------------------------- Partition's Main Thread ---------------------------

static void psa_attest_get_token(void)
{
    psa_msg_t msg = { 0 };
    enum psa_attest_err_t status = PSA_ATTEST_ERR_SUCCESS;

    if (PSA_SUCCESS != psa_get(PSA_ATTEST_GET_TOKEN, &msg)) {
        return;
    }
    switch (msg.type) {
        case PSA_IPC_CONNECT:
        case PSA_IPC_DISCONNECT: {
            break;
        }
        case PSA_IPC_CALL: {
            uint8_t *challenge_buff = NULL;
            uint8_t *token_buff = NULL;
            uint32_t bytes_read = 0;

            challenge_buff = calloc(1, msg.in_size[0]);
            if (challenge_buff == NULL) {
                status = PSA_ATTEST_ERR_GENERAL;
                break;
            }
            bytes_read = psa_read(msg.handle, 0,
                                  challenge_buff, msg.in_size[0]);
            if (bytes_read != msg.in_size[0]) {
                free(challenge_buff);
                SPM_PANIC("SPM read length mismatch");
            }

            token_buff = calloc(1, msg.out_size[0]);
            if (token_buff == NULL) {
                status = PSA_ATTEST_ERR_GENERAL;
                free(challenge_buff);
                break;
            }

            psa_invec in_vec[1] = { { challenge_buff, msg.in_size[0] } };
            psa_outvec out_vec[1] = { { token_buff, msg.out_size[0] } };

            status = attest_init();
            if (status != PSA_ATTEST_ERR_SUCCESS) {
                free(challenge_buff);
                free(token_buff);
                break;
            }

            set_caller_id(&msg);
            status = initial_attest_get_token(in_vec, 1, out_vec, 1);
            if (status == PSA_ATTEST_ERR_SUCCESS) {
                psa_write(msg.handle, 0, out_vec[0].base, out_vec[0].len);
            }

            free(challenge_buff);
            free(token_buff);
            break;
        }

        default: {
            SPM_PANIC("Unexpected message type %d!", (int)(msg.type));
            break;
        }
    }

    psa_reply(msg.handle, status);
}

static void psa_attest_get_token_size(void)
{
    psa_msg_t msg = { 0 };
    enum psa_attest_err_t status = PSA_ATTEST_ERR_SUCCESS;

    if (PSA_SUCCESS != psa_get(PSA_ATTEST_GET_TOKEN_SIZE, &msg)) {
        return;
    }
    switch (msg.type) {
        case PSA_IPC_CONNECT:
        case PSA_IPC_DISCONNECT: {
            break;
        }
        case PSA_IPC_CALL: {
            uint32_t challenge_size;
            uint32_t token_size;
            uint32_t bytes_read = 0;

            bytes_read = psa_read(msg.handle, 0,
                                  &challenge_size, msg.in_size[0]);
            if (bytes_read != msg.in_size[0]) {
                SPM_PANIC("SPM read length mismatch");
            }

            psa_invec in_vec[1] = { { &challenge_size, msg.in_size[0] } };
            psa_outvec out_vec[1] = { { &token_size, msg.out_size[0] } };

            status = attest_init();
            if (status != PSA_ATTEST_ERR_SUCCESS) {
                break;
            }

            set_caller_id(&msg);
            status = initial_attest_get_token_size(in_vec, 1, out_vec, 1);
            if (status == PSA_ATTEST_ERR_SUCCESS) {
                psa_write(msg.handle, 0, out_vec[0].base, out_vec[0].len);
            }

            break;
        }

        default: {
            SPM_PANIC("Unexpected message type %d!", (int)(msg.type));
            break;
        }
    }

    psa_reply(msg.handle, status);
}

static void psa_attest_inject_key(void)
{
    psa_msg_t msg = { 0 };
    psa_status_t status = PSA_SUCCESS;

    if (PSA_SUCCESS != psa_get(PSA_ATTEST_INJECT_KEY, &msg)) {
        return;
    }
    switch (msg.type) {
        case PSA_IPC_CONNECT:
        case PSA_IPC_DISCONNECT: {
            break;
        }
        case PSA_IPC_CALL: {
            uint8_t *public_key_data = NULL;
            size_t public_key_data_length = 0;
            uint8_t *key_data = NULL;
            psa_key_type_t type;
            uint32_t bytes_read = 0;

            if (msg.in_size[0] != sizeof(psa_key_type_t)) {
                status = PSA_ERROR_INVALID_ARGUMENT;
                break;
            }

            bytes_read = psa_read(msg.handle, 0, &type, msg.in_size[0]);
            if (bytes_read != msg.in_size[0]) {
                SPM_PANIC("SPM read length mismatch");
            }

            public_key_data = calloc(1, msg.out_size[0]);
            if (public_key_data == NULL) {
                status = PSA_ERROR_INSUFFICIENT_MEMORY;
                break;
            }
            if (msg.in_size[1] != 0) {
                key_data = calloc(1, msg.in_size[1]);
                if (key_data == NULL) {
                    status = PSA_ERROR_INSUFFICIENT_MEMORY;
                    free(public_key_data);
                    break;
                }

                bytes_read = psa_read(msg.handle, 1,
                                      key_data, msg.in_size[1]);
                if (bytes_read != msg.in_size[1]) {
                    free(public_key_data);
                    free(key_data);
                    SPM_PANIC("SPM read length mismatch");
                }
            }
            status = psa_attestation_inject_key_impl(key_data,
                                                     msg.in_size[1],
                                                     type,
                                                     public_key_data,
                                                     msg.out_size[0],
                                                     &public_key_data_length);

            if (status == PSA_SUCCESS) {
                psa_write(msg.handle, 0, public_key_data, public_key_data_length);
            }

            psa_write(msg.handle, 1,
                      &public_key_data_length, sizeof(public_key_data_length));
            free(public_key_data);
            if (key_data != NULL) {
                free(key_data);
            }
            break;

        }

        default: {
            SPM_PANIC("Unexpected message type %d!", (int)(msg.type));
            break;
        }
    }

    psa_reply(msg.handle, status);
}

void attest_main(void *ptr)
{
    while (1) {
        uint32_t signals = psa_wait(ATTEST_SRV_WAIT_ANY_SID_MSK, PSA_BLOCK);
        if (signals & PSA_ATTEST_GET_TOKEN) {
            psa_attest_get_token();
        }
        if (signals & PSA_ATTEST_GET_TOKEN_SIZE) {
            psa_attest_get_token_size();
        }
        if (signals & PSA_ATTEST_INJECT_KEY) {
            psa_attest_inject_key();
        }
    }
}
