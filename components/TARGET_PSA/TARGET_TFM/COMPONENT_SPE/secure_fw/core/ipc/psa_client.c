/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <inttypes.h>
#include <stdio.h>
#include "tfm_svc.h"
#include "psa_client.h"

__attribute__((naked, section("SFN")))
uint32_t psa_framework_version(void)
{
    __ASM volatile("SVC %0           \n"
                   "BX LR            \n"
                   : : "I" (TFM_SVC_PSA_FRAMEWORK_VERSION));
}

__attribute__((naked, section("SFN")))
uint32_t psa_version(uint32_t sid)
{
    __ASM volatile("SVC %0           \n"
                   "BX LR            \n"
                   : : "I" (TFM_SVC_PSA_VERSION));
}

__attribute__((naked, section("SFN")))
psa_handle_t psa_connect(uint32_t sid, uint32_t minor_version)
{
    __ASM volatile("SVC %0           \n"
                   "BX LR            \n"
                   : : "I" (TFM_SVC_PSA_CONNECT));
}

__attribute__((naked, section("SFN")))
psa_status_t psa_call(psa_handle_t handle,
                      const psa_invec *in_vec,
                      size_t in_len,
                      psa_outvec *out_vec,
                      size_t out_len)
{
    __ASM volatile("SVC %0           \n"
                   "BX LR            \n"
                   : : "I" (TFM_SVC_PSA_CALL));
}

__attribute__((naked, section("SFN")))
void psa_close(psa_handle_t handle)
{
    __ASM volatile("SVC %0           \n"
                   "BX LR            \n"
                   : : "I" (TFM_SVC_PSA_CLOSE));
}
