/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <arm_cmse.h>

#include "tfm_svc.h"
#include "tfm_secure_api.h"
#include "tfm_internal.h"
#include "secure_fw/include/tfm_spm_services_api.h"
#include "spm_api.h"

uint8_t *tfm_scratch_area;
uint32_t tfm_scratch_area_size;
nsfptr_t ns_entry;

void jump_to_ns_code(void)
{
#if TFM_LVL == 3 || ((!defined(TFM_PSA_API)) && (TFM_LVL != 1))
    /* Initialization is done, set thread mode to unprivileged. */
    tfm_spm_partition_change_privilege(TFM_PARTITION_UNPRIVILEGED_MODE);
#endif
    /* All changes made to memory will be effective after this point */
    __DSB();
    __ISB();

    /* Calls the non-secure Reset_Handler to jump to the non-secure binary */
    ns_entry();
}

#ifndef TFM_PSA_API
#if defined(__ARM_ARCH_8M_MAIN__)
__attribute__((naked)) int32_t tfm_core_sfn_request(
                                           const struct tfm_sfn_req_s *desc_ptr)
{
    __ASM volatile(
          "PUSH   {r4-r12, lr}\n"
          "SVC    %[SVC_REQ]\n"
          "MOV    r4, #0\n"
          "MOV    r5, #0\n"
          "MOV    r6, #0\n"
          "MOV    r7, #0\n"
          "MOV    r8, #0\n"
          "MOV    r9, #0\n"
          "MOV    r10, #0\n"
          "MOV    r11, #0\n"
          "BLX    lr\n"
          "SVC    %[SVC_RET]\n"
          "POP    {r4-r12, pc}\n"
          : : [SVC_REQ] "I" (TFM_SVC_SFN_REQUEST)
            , [SVC_RET] "I" (TFM_SVC_SFN_RETURN)
          : "r0");
}
#elif defined(__ARM_ARCH_8M_BASE__)
__attribute__((naked)) int32_t tfm_core_sfn_request(
                                           const struct tfm_sfn_req_s *desc_ptr)
{
    __ASM volatile(
          ".syntax unified\n"
          "PUSH   {lr}\n"
          "PUSH   {r4-r7}\n"
          "MOV    r4, r8\n"
          "MOV    r5, r9\n"
          "MOV    r6, r10\n"
          "MOV    r7, r11\n"
          "PUSH   {r4-r7}\n"
          "MOV    r4, r12\n"
          "PUSH   {r4}\n"
          "SVC    %[SVC_REQ]\n"
          "MOVS   r4, #0\n"
          "MOV    r5, r4\n"
          "MOV    r6, r4\n"
          "MOV    r7, r4\n"
          "MOV    r8, r4\n"
          "MOV    r9, r4\n"
          "MOV    r10, r4\n"
          "MOV    r11, r4\n"
          "BLX    lr\n"
          "SVC    %[SVC_RET]\n"
          "POP    {r4}\n"
          "MOV    r12, r4\n"
          "POP    {r4-r7}\n"
          "MOV    r8, r4\n"
          "MOV    r9, r5\n"
          "MOV    r10, r6\n"
          "MOV    r11, r7\n"
          "POP    {r4-r7}\n"
          "POP    {pc}\n"
          : : [SVC_REQ] "I" (TFM_SVC_SFN_REQUEST)
            , [SVC_RET] "I" (TFM_SVC_SFN_RETURN)
          : "r0");
}
#else
#error "Unsupported ARM Architecture."
#endif

__attribute__((naked))
int32_t tfm_core_memory_permission_check(const void *ptr,
                                         uint32_t len,
                                         int32_t access)
{
  __ASM volatile(
        "SVC    %0\n"
        "BX     lr\n"
        : : "I" (TFM_SVC_MEMORY_CHECK));
}

__attribute__((naked))
int32_t tfm_core_get_caller_client_id(int32_t *caller_client_id)
{
    __ASM volatile(
        "SVC %0\n"
        "BX LR\n"
        : : "I" (TFM_SVC_GET_CALLER_CLIENT_ID));
}

__attribute__((naked))
int32_t tfm_spm_request_reset_vote(void)
{
    __ASM volatile(
        "MOVS   R0, %0\n"
        "B      tfm_spm_request\n"
        : : "I" (TFM_SPM_REQUEST_RESET_VOTE));
}

__attribute__((naked))
int32_t tfm_spm_request(void)
{
    __ASM volatile(
        "SVC    %0\n"
        "BX     lr\n"
        : : "I" (TFM_SVC_SPM_REQUEST));
}

__attribute__((naked))
int32_t tfm_core_validate_secure_caller(void)
{
    __ASM volatile(
        "SVC    %0\n"
        "BX     lr\n"
        : : "I" (TFM_SVC_VALIDATE_SECURE_CALLER));
}

__attribute__((naked))
int32_t tfm_core_set_buffer_area(enum tfm_buffer_share_region_e share)
{
    __ASM volatile(
        "SVC    %0\n"
        "BX     lr\n"
        : : "I" (TFM_SVC_SET_SHARE_AREA));
}
#endif

__attribute__((naked))
int32_t tfm_core_get_boot_data(uint8_t major_type,
                               struct tfm_boot_data *boot_status,
                               uint32_t len)
{
    __ASM volatile(
        "SVC    %0\n"
        "BX     lr\n"
        : : "I" (TFM_SVC_GET_BOOT_DATA));
}
