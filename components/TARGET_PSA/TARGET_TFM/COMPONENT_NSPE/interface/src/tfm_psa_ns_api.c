/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "interface/include/psa_client.h"
#include "tfm_ns_lock.h"
#include "tfm_api.h"

/**** API functions ****/

uint32_t psa_framework_version(void)
{
    return tfm_ns_lock_dispatch((veneer_fn)tfm_psa_framework_version_veneer,
                                0,
                                0,
                                0,
                                0);
}

uint32_t psa_version(uint32_t sid)
{
    return tfm_ns_lock_dispatch((veneer_fn)tfm_psa_version_veneer,
                                sid,
                                0,
                                0,
                                0);
}

psa_handle_t psa_connect(uint32_t sid, uint32_t minor_version)
{
    return tfm_ns_lock_dispatch((veneer_fn)tfm_psa_connect_veneer,
                                sid,
                                minor_version,
                                0,
                                0);
}

psa_status_t psa_call(psa_handle_t handle,
                      const psa_invec *in_vec,
                      size_t in_len,
                      psa_outvec *out_vec,
                      size_t out_len)
{
    /* FixMe: sanity check can be added to offload some NS thread checks from
     * TFM secure API
     */

    /* Due to v8M restrictions, TF-M NS API needs to add another layer of
     * serialization in order for NS to pass arguments to S
     */
    psa_invec in_vecs, out_vecs;

    in_vecs.base = in_vec;
    in_vecs.len = in_len;
    out_vecs.base = out_vec;
    out_vecs.len = out_len;
    return tfm_ns_lock_dispatch((veneer_fn)tfm_psa_call_veneer,
                                (uint32_t)handle,
                                (uint32_t)&in_vecs,
                                (uint32_t)&out_vecs,
                                0);
}

void psa_close(psa_handle_t handle)
{
    tfm_ns_lock_dispatch((veneer_fn)tfm_psa_close_veneer,
                         (uint32_t)handle,
                         0,
                         0,
                         0);
}

