/* Copyright (c) 2019 ARM Limited
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

#include "mbed_spm_partitions.h"
#include "platform_srv_impl.h"
#include "psa/internal_trusted_storage.h"
#include "psa/service.h"

typedef psa_status_t (*SignalHandler)(psa_msg_t *);

static psa_status_t lifecycle_get(psa_msg_t *msg)
{
    uint32_t lc_state;

    if (msg->out_size[0] != sizeof(lc_state)) {
        return PSA_DROP_CONNECTION;
    }

    psa_status_t status = psa_platfrom_lifecycle_get_impl(&lc_state);
    if (status == PSA_SUCCESS) {
        psa_write(msg->handle, 0, &lc_state, sizeof(lc_state));
    }

    return status;
}

static psa_status_t lifecycle_change_request(psa_msg_t *msg)
{
    uint32_t lc_state;

    if (msg->in_size[0] != sizeof(lc_state)) {
        return PSA_DROP_CONNECTION;
    }
    if (psa_read(msg->handle, 0, &lc_state, sizeof(lc_state)) != sizeof(lc_state)) {
        return PSA_DROP_CONNECTION;
    }
    return psa_platfrom_lifecycle_change_request_impl(lc_state);

}

static MBED_NORETURN psa_status_t system_reset_request(psa_msg_t *msg)
{
    (void)msg;
    mbed_psa_system_reset_impl();
}

static void message_handler(psa_msg_t *msg, SignalHandler handler)
{
    psa_status_t status = PSA_SUCCESS;
    switch (msg->type) {
        case PSA_IPC_CONNECT: //fallthrough
        case PSA_IPC_DISCONNECT: {
            break;
        }
        case PSA_IPC_CALL: {
            status = handler(msg);
            break;
        }
        default: {
            SPM_PANIC("Unexpected message type %lu!", msg->type);
            break;
        }
    }
    psa_reply(msg->handle, status);
}

void platform_partition_entry(void *ptr)
{
    psa_signal_t signals = 0;
    psa_msg_t msg = {0};
    while (1) {
        signals = psa_wait(PLATFORM_WAIT_ANY_SID_MSK, PSA_BLOCK);
        if ((signals & PSA_PLATFORM_LC_GET_MSK) != 0) {
            if (PSA_SUCCESS != psa_get(PSA_PLATFORM_LC_GET_MSK, &msg)) {
                continue;
            }
            message_handler(&msg, lifecycle_get);
        }
        if ((signals & PSA_PLATFORM_LC_SET_MSK) != 0) {
            if (PSA_SUCCESS != psa_get(PSA_PLATFORM_LC_SET_MSK, &msg)) {
                continue;
            }
            message_handler(&msg, lifecycle_change_request);
        }
        if ((signals & PSA_PLATFORM_SYSTEM_RESET_MSK) != 0) {
            if (PSA_SUCCESS != psa_get(PSA_PLATFORM_SYSTEM_RESET_MSK, &msg)) {
                continue;
            }
            message_handler(&msg, system_reset_request);
        }
    }
}
