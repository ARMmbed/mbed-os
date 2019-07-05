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
#include "psa_service.h"

__attribute__((naked, section("SFN")))
psa_signal_t psa_wait(psa_signal_t signal_mask, uint32_t timeout)

{
    __ASM volatile("SVC %0           \n"
                   "BX LR            \n"
                   : : "I" (TFM_SVC_PSA_WAIT));
}

__attribute__((naked, section("SFN")))
psa_status_t psa_get(psa_signal_t signal, psa_msg_t *msg)
{
    __ASM volatile("SVC %0           \n"
                   "BX LR            \n"
                   : : "I" (TFM_SVC_PSA_GET));
}

__attribute__((naked, section("SFN")))
void psa_set_rhandle(psa_handle_t msg_handle, void *rhandle)
{
    __ASM volatile("SVC %0           \n"
                   "BX LR            \n"
                   : : "I" (TFM_SVC_PSA_SET_RHANDLE));
}

__attribute__((naked, section("SFN")))
size_t psa_read(psa_handle_t msg_handle, uint32_t invec_idx,
                void *buffer, size_t num_bytes)

{
    __ASM volatile("SVC %0           \n"
                   "BX LR            \n"
                   : : "I" (TFM_SVC_PSA_READ));
}

__attribute__((naked, section("SFN")))
size_t psa_skip(psa_handle_t msg_handle, uint32_t invec_idx, size_t num_bytes)
{
    __ASM volatile("SVC %0           \n"
                   "BX LR            \n"
                   : : "I" (TFM_SVC_PSA_SKIP));
}

__attribute__((naked, section("SFN")))
void psa_write(psa_handle_t msg_handle, uint32_t outvec_idx,
               const void *buffer, size_t num_bytes)
{
    __ASM volatile("SVC %0           \n"
                   "BX LR            \n"
                   : : "I" (TFM_SVC_PSA_WRITE));
}

__attribute__((naked, section("SFN")))
void psa_reply(psa_handle_t msg_handle, psa_status_t retval)
{
    __ASM volatile("SVC %0           \n"
                   "BX LR            \n"
                   : : "I" (TFM_SVC_PSA_REPLY));
}

__attribute__((naked, section("SFN")))
void psa_notify(int32_t partition_id)
{
    __ASM volatile("SVC %0           \n"
                   "BX LR            \n"
                   : : "I" (TFM_SVC_PSA_NOTIFY));
}

__attribute__((naked, section("SFN")))
void psa_clear(void)
{
    __ASM volatile("SVC %0           \n"
                   "BX LR            \n"
                   : : "I" (TFM_SVC_PSA_CLEAR));
}

__attribute__((naked, section("SFN")))
void psa_eoi(psa_signal_t irq_signal)
{
    __ASM volatile("SVC %0           \n"
                   "BX LR            \n"
                   : : "I" (TFM_SVC_PSA_EOI));
}
