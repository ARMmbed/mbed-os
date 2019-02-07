/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <arm_cmse.h>
#include "tfm_secure_api.h"
#include "tfm_nspm.h"
#include "secure_utilities.h"
#include "secure_fw/spm/spm_api.h"
#include "region_defs.h"
#include "tfm_api.h"

#define EXC_RETURN_SECURE_FUNCTION 0xFFFFFFFD

#ifndef TFM_LVL
#error TFM_LVL is not defined!
#endif

/* Macros to pick linker symbols and allow references to sections */
#define REGION(a, b, c) a##b##c
#define REGION_NAME(a, b, c) REGION(a, b, c)
#define REGION_DECLARE(a, b, c) extern uint32_t REGION_NAME(a, b, c)

#ifndef TFM_PSA_API /* Only use scratch if using veneer functions, not IPC */
REGION_DECLARE(Image$$, TFM_UNPRIV_SCRATCH, $$ZI$$Base);
REGION_DECLARE(Image$$, TFM_UNPRIV_SCRATCH, $$ZI$$Limit);
#endif /* !defined(TFM_PSA_API) */

/* This is the "Big Lock" on the secure side, to guarantee single entry
 * to SPE
 */
int32_t tfm_secure_lock;

/**
 * \brief Check whether a memory range is inside a memory region.
 *
 * \param[in] p             The start address of the range to check
 * \param[in] s             The size of the range to check
 * \param[in] region_start  The start address of the region, which should
 *                          contain the range
 * \param[in] region_len    The size of the region, which should contain the
 *                          range
 *
 * \return TFM_SUCCESS if the region contains the range,
 *         TFM_ERROR_GENERIC otherwise.
 */
static int32_t check_address_range(const void *p, size_t s,
                                   uintptr_t region_start, uint32_t region_len)
{
    int32_t range_in_region;

    /* Check for overflow in the range parameters */
    if ((uintptr_t)p > UINTPTR_MAX-s) {
        return TFM_ERROR_GENERIC;
    }

    /* We trust the region parameters, and don't check for overflow */

    /* Calculate the result */
    range_in_region = ((uintptr_t)p >= region_start) &&
                      ((uintptr_t)p+s <= region_start+region_len);
    if (range_in_region) {
        return TFM_SUCCESS;
    } else {
        return TFM_ERROR_GENERIC;
    }
}

/**
 * \brief Check whether the current partition has access to a memory range
 *
 * This function assumes, that the current MPU configuration is set for the
 * partition to be checked. The flags should contain information of the
 * execution mode of the partition code (priv/unpriv), and access type
 * (read/write) as specified in "ARMv8-M Security Extensions: Requirements on
 * Development Tools" chapter "Address range check intrinsic"
 *
 * \param[in] p      The start address of the range to check
 * \param[in] s      The size of the range to check
 * \param[in] flags  The flags to pass to the cmse_check_address_range func
 *
 * \return TFM_SUCCESS if the partition has access to the memory range,
 *         TFM_ERROR_GENERIC otherwise.
 */
static int32_t has_access_to_region(const void *p, size_t s, int flags)
{
    int32_t range_access_allowed_by_mpu;

#ifndef TFM_PSA_API /* Only use scratch if using veneer functions, not IPC */
    uint32_t scratch_base =
        (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_SCRATCH, $$ZI$$Base);
    uint32_t scratch_limit =
        (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_SCRATCH, $$ZI$$Limit);
#endif /* !defined(TFM_PSA_API) */

    /* Use the TT instruction to check access to the partition's regions*/
    range_access_allowed_by_mpu =
                          cmse_check_address_range((void *)p, s, flags) != NULL;

    if (range_access_allowed_by_mpu) {
        return TFM_SUCCESS;
    }

#ifndef TFM_PSA_API /* Only use scratch if using veneer functions, not IPC */
    /* If the check for the current MPU settings fails, check for the share
     * region, only if the partition is secure
     */
    if ((flags & CMSE_NONSECURE) == 0) {
        if (check_address_range(p, s, scratch_base,
                                scratch_limit+1-scratch_base) == TFM_SUCCESS) {
            return TFM_SUCCESS;
        }
    }
#endif /* !defined(TFM_PSA_API) */

    /* If all else fails, check whether the region is in the non-secure
     * memory
     */
    if (check_address_range(p, s, NS_CODE_START,
                            NS_CODE_LIMIT+1-NS_CODE_START) == TFM_SUCCESS ||
        check_address_range(p, s, NS_DATA_START,
                            NS_DATA_LIMIT+1-NS_DATA_START) == TFM_SUCCESS) {
        return TFM_SUCCESS;
    } else {
        return TFM_ERROR_GENERIC;
    }
}

int32_t tfm_core_has_read_access_to_region(const void *p, size_t s,
                                           uint32_t ns_caller,
                                           uint32_t privileged)
{
    int flags = CMSE_MPU_READ;

    if (privileged == TFM_PARTITION_UNPRIVILEGED_MODE) {
        flags |= CMSE_MPU_UNPRIV;
    }

    if (ns_caller) {
        flags |= CMSE_NONSECURE;
    }

    return has_access_to_region(p, s, flags);
}

int32_t tfm_core_has_write_access_to_region(void *p, size_t s,
                                            uint32_t ns_caller,
                                            uint32_t privileged)
{
    int flags = CMSE_MPU_READWRITE;

    if (privileged == TFM_PARTITION_UNPRIVILEGED_MODE) {
        flags |= CMSE_MPU_UNPRIV;
    }

    if (ns_caller) {
        flags |= CMSE_NONSECURE;
    }

    return has_access_to_region(p, s, flags);
}

void tfm_secure_api_error_handler(void)
{
    ERROR_MSG("Security violation when calling secure API");
    while (1) {
        ;
    }
}

