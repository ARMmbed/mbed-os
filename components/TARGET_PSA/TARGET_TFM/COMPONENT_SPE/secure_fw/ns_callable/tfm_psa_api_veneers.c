/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdbool.h>
#include <stdio.h>
#include "psa_client.h"
#include "psa_service.h"
#include "secure_utilities.h"
#include "tfm_secure_api.h"
#include "tfm_api.h"
#include "tfm_svcalls.h"

/* FixMe: check if this is really needed */
extern int32_t tfm_secure_lock;

__attribute__ ((always_inline)) __STATIC_INLINE
int32_t tfm_psa_veneer_sanity_check(struct tfm_sfn_req_s *desc_ptr)
{
    if (desc_ptr->ns_caller) {
        if (tfm_secure_lock != 0) {
            /* Secure domain is already locked!
             * FixMe: Decide if this is a fault or permitted in case of PSA
             * API usage
             */
            return TFM_ERROR_SECURE_DOMAIN_LOCKED;
        }
    } else {
        /* Secure partition should not call a different secure partition
         * using TFM PSA veneers
         */
        return TFM_ERROR_INVALID_EXC_MODE;
    }
    return TFM_SUCCESS;
}

/* Veneer implementation */

#define TFM_CORE_NS_IPC_REQUEST_VENEER(fn, a, b, c, d) \
            return tfm_core_ns_ipc_request(fn, (int32_t)a, (int32_t)b, \
                (int32_t)c, (int32_t)d)

__attribute__ ((naked, section("SFN")))
static int32_t tfm_core_ipc_request(const struct tfm_sfn_req_s *desc_ptr)
{
    __ASM volatile("SVC %0           \n"
                   "BX LR            \n"
                   : : "I" (TFM_SVC_IPC_REQUEST));
}

__attribute__ ((always_inline)) __STATIC_INLINE
int32_t tfm_core_ns_ipc_request(void *fn, int32_t arg1, int32_t arg2,
                                int32_t arg3, int32_t arg4)
{
    int32_t args[4] = {arg1, arg2, arg3, arg4};
    struct tfm_sfn_req_s desc = {0};

    desc.sfn = fn;
    desc.args = args;
    desc.ns_caller = cmse_nonsecure_caller();

    if (__get_active_exc_num() != EXC_NUM_THREAD_MODE)
    {
        /* FIXME: Proper error handling to be implemented */
        return TFM_ERROR_INVALID_EXC_MODE;
    } else {
        return tfm_core_ipc_request(&desc);
    }
}

/* FixMe: these functions need to have different attributes compared to those
 * legacy veneers which may be called by secure partitions.
 * They won't call legacy SFN but instead will be handlers for TF-M
 */

__tfm_secure_gateway_attributes__
uint32_t tfm_psa_framework_version_veneer(void)
{
    TFM_CORE_NS_IPC_REQUEST_VENEER(tfm_svcall_psa_framework_version, 0, 0,
                                   0, 0);
}

__tfm_secure_gateway_attributes__
uint32_t tfm_psa_version_veneer(uint32_t sid)
{
    TFM_CORE_NS_IPC_REQUEST_VENEER(tfm_svcall_psa_version, sid, 0, 0, 0);
}

__tfm_secure_gateway_attributes__
psa_handle_t tfm_psa_connect_veneer(uint32_t sid, uint32_t minor_version)
{
    TFM_CORE_NS_IPC_REQUEST_VENEER(tfm_svcall_psa_connect, sid,
                                   minor_version, 0, 0);
}

__tfm_secure_gateway_attributes__
psa_status_t tfm_psa_call_veneer(psa_handle_t handle,
                                 const psa_invec *in_vecs,
                                 psa_outvec *out_vecs)
{
    TFM_CORE_NS_IPC_REQUEST_VENEER(tfm_svcall_psa_call, handle, in_vecs,
                                   out_vecs, 0);
}

__tfm_secure_gateway_attributes__
psa_status_t tfm_psa_close_veneer(psa_handle_t handle)
{
    TFM_CORE_NS_IPC_REQUEST_VENEER(tfm_svcall_psa_close, handle, 0, 0, 0);
}

void tfm_psa_ipc_request_handler(uint32_t svc_ctx[])
{
    uint32_t *r0_ptr = svc_ctx;

    /* The only argument to the SVC call is stored in the stacked r0 */
    struct tfm_sfn_req_s *desc_ptr = (struct tfm_sfn_req_s *) *r0_ptr;

    if(tfm_psa_veneer_sanity_check(desc_ptr) != TFM_SUCCESS) {
        /* FixMe: consider error handling - this may be critical error */
        *r0_ptr = TFM_ERROR_INVALID_PARAMETER;
        return;
    }

    /* Store SVC return value in stacked r0 */
    *r0_ptr = desc_ptr->sfn((int32_t)desc_ptr->args,
                            desc_ptr->ns_caller,
                            0,
                            0);

    return;
}
