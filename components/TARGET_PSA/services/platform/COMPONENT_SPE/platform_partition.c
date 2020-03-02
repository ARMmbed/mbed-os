/* Copyright (c) 2019-2020 Arm Limited
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
#include "psa/lifecycle.h"
#include "psa/internal_trusted_storage.h"
#include "psa/service.h"

#define INPUT_BUFFER_SIZE  64
#define OUTPUT_BUFFER_SIZE 64

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

static enum tfm_platform_err_t
platform_sp_ioctl_ipc(const psa_msg_t *msg) {
    void *input = NULL;
    void *output = NULL;
    psa_invec invec = {0};
    psa_outvec outvec = {0};
    uint8_t input_buffer[INPUT_BUFFER_SIZE] = {0};
    uint8_t output_buffer[OUTPUT_BUFFER_SIZE] = {0};
    tfm_platform_ioctl_req_t request = 0;
    enum tfm_platform_err_t ret = TFM_PLATFORM_ERR_SYSTEM_ERROR;
    size_t num = 0;
    uint32_t in_len = PSA_MAX_IOVEC;
    uint32_t out_len = PSA_MAX_IOVEC;

    while ((in_len > 0) && (msg->in_size[in_len - 1] == 0))
    {
        in_len--;
    }

    while ((out_len > 0) && (msg->out_size[out_len - 1] == 0))
    {
        out_len--;
    }

    if ((in_len < 1) || (in_len > 2) ||
            (out_len > 1))
    {
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }

    num = psa_read(msg->handle, 0, &request, sizeof(request));
    if (num != sizeof(request))
    {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    if (in_len > 1)
    {
        if (msg->in_size[1] > INPUT_BUFFER_SIZE) {
            return PSA_ERROR_PROGRAMMER_ERROR;
        }
        num = psa_read(msg->handle, 1, &input_buffer, msg->in_size[1]);
        if (num != msg->in_size[1]) {
            return PSA_ERROR_PROGRAMMER_ERROR;
        }
        invec.base = input_buffer;
        invec.len = msg->in_size[1];
        input = &invec;
    }

    if (out_len > 0)
    {
        if (msg->out_size[0] > OUTPUT_BUFFER_SIZE) {
            return PSA_ERROR_PROGRAMMER_ERROR;
        }
        outvec.base = output_buffer;
        outvec.len = msg->out_size[0];
        output = &outvec;
    }

    ret = tfm_platform_hal_ioctl(request, input, output);

    if (output != NULL)
    {
        psa_write(msg->handle, 0, outvec.base, outvec.len);
    }

    return ret;
}

static psa_status_t platform_ioctl(psa_msg_t *msg)
{
    /*  platform_sp_ioctl_ipc returns either psa_status_t or one of the
     *  following errorcodes:
     *  enum tfm_platform_err_t {
     *      TFM_PLATFORM_ERR_SUCCESS = 0,
     *      TFM_PLATFORM_ERR_SYSTEM_ERROR,
     *      TFM_PLATFORM_ERR_INVALID_PARAM,
     *      TFM_PLATFORM_ERR_NOT_SUPPORTED,
     *
     *      TFM_PLATFORM_ERR_FORCE_INT_SIZE = INT_MAX
     *  };
     */
    return platform_sp_ioctl_ipc(msg);
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
        if ((signals & PSA_PLATFORM_IOCTL_MSK)  != 0) {
            if (PSA_SUCCESS != psa_get(PSA_PLATFORM_IOCTL_MSK, &msg)) {
                continue;
            }
            message_handler(&msg, platform_ioctl);
        }
    }
}
