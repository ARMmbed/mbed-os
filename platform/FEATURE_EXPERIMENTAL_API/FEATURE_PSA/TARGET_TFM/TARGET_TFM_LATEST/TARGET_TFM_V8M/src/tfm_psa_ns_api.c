/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "psa/client.h"
#include "tfm_ns_interface.h"
#include "tfm_api.h"
#include "tfm_psa_call_param.h"

/**** API functions ****/

uint32_t psa_framework_version(void)
{
    return tfm_ns_interface_dispatch(
                                (veneer_fn)tfm_psa_framework_version_veneer,
                                0,
                                0,
                                0,
                                0);
}

uint32_t psa_version(uint32_t sid)
{
    return tfm_ns_interface_dispatch(
                                (veneer_fn)tfm_psa_version_veneer,
                                sid,
                                0,
                                0,
                                0);
}

psa_handle_t psa_connect(uint32_t sid, uint32_t version)
{
    return tfm_ns_interface_dispatch(
                                (veneer_fn)tfm_psa_connect_veneer,
                                sid,
                                version,
                                0,
                                0);
}

psa_status_t psa_call(psa_handle_t handle, int32_t type,
                      const psa_invec *in_vec,
                      size_t in_len,
                      psa_outvec *out_vec,
                      size_t out_len)
{
    if ((type > INT16_MAX) ||
        (type < INT16_MIN) ||
        (in_len > UINT8_MAX) ||
        (out_len > UINT8_MAX)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    return tfm_ns_interface_dispatch(
                                (veneer_fn)tfm_psa_call_veneer,
                                (uint32_t)handle,
                                PARAM_PACK(type, in_len, out_len),
                                (uint32_t)in_vec,
                                (uint32_t)out_vec);
}

void psa_close(psa_handle_t handle)
{
    (void)tfm_ns_interface_dispatch(
                         (veneer_fn)tfm_psa_close_veneer,
                         (uint32_t)handle,
                         0,
                         0,
                         0);
}
