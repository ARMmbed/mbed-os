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

#include "psa_manifest/sid.h"
#include "psa/lifecycle.h"
#include "psa/client.h"
#include "mbed_toolchain.h"
#include "mbed_error.h"
#include "tfm_platform_api.h"

uint32_t psa_security_lifecycle_state(void)
{
    psa_handle_t conn = psa_connect(PSA_PLATFORM_LC_GET, 1);
    if (conn <= PSA_NULL_HANDLE) {
        return PSA_LIFECYCLE_UNKNOWN;
    }

    uint32_t lc_state = 0;
    psa_outvec resp[1] = { {&lc_state, sizeof(lc_state)} };

    psa_status_t status = psa_call(conn, NULL, 0, resp, 1);
    if (status == PSA_DROP_CONNECTION) {
        lc_state = PSA_LIFECYCLE_UNKNOWN;
    }

    psa_close(conn);

    return lc_state;
}

psa_status_t mbed_psa_reboot_and_request_new_security_state(uint32_t new_state)
{
    psa_handle_t conn = psa_connect(PSA_PLATFORM_LC_SET, 1);
    if (conn <= PSA_NULL_HANDLE) {
        return (psa_status_t) conn;
    }

    psa_invec msg[1] = {
        { &new_state, sizeof(new_state) }
    };

    psa_status_t status = psa_call(conn, msg, 1, NULL, 0);

    psa_close(conn);
    return status;
}

void mbed_psa_system_reset(void)
{
    psa_handle_t conn = psa_connect(PSA_PLATFORM_SYSTEM_RESET, 1);
    if (conn > PSA_NULL_HANDLE) {
        psa_call(conn, NULL, 0, NULL, 0);
    }
    error("reset failed - cannot connect to service handle=%ld", conn);
}

enum tfm_platform_err_t
tfm_platform_ioctl(tfm_platform_ioctl_req_t request,
                   psa_invec *input, psa_outvec *output) {
    tfm_platform_ioctl_req_t req = request;
    struct psa_invec in_vec[2] = { {0} };
    size_t inlen, outlen;
    psa_status_t status = PSA_ERROR_CONNECTION_REFUSED;
    psa_handle_t handle = PSA_NULL_HANDLE;

    in_vec[0].base = &req;
    in_vec[0].len = sizeof(req);
    if (input != NULL)
    {
        in_vec[1].base = input->base;
        in_vec[1].len = input->len;
        inlen = 2;
    } else
    {
        inlen = 1;
    }

    if (output != NULL)
    {
        outlen = 1;
    } else
    {
        outlen = 0;
    }

    handle = psa_connect(PSA_PLATFORM_IOCTL, 1);
    if (handle <= 0)
    {
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }

    status = psa_call(handle,
                      in_vec, inlen,
                      output, outlen);
    psa_close(handle);

    if (status < PSA_SUCCESS)
    {
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    } else
    {
        return (enum tfm_platform_err_t) status;
    }
}
