/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include "platform/include/tfm_spm_hal.h"
#include "spm_api.h"
#include "spm_db.h"
#include "tfm_platform_core_api.h"
#include "target_cfg.h"
#include "region_defs.h"
#include "secure_utilities.h"

/* Get address of memory regions to configure MPU */
extern const struct memory_region_limits memory_regions;

void tfm_spm_hal_init_isolation_hw(void)
{
    /* Configures non-secure memory spaces in the target */
    sau_and_idau_cfg();
    mpc_init_cfg();
    ppc_init_cfg();
}

void tfm_spm_hal_configure_default_isolation(
        const struct tfm_spm_partition_platform_data_t *platform_data)
{
    /*if (platform_data) {
        ppc_configure_to_secure(platform_data->periph_ppc_bank,
                                platform_data->periph_ppc_loc);
    }*/
}

void tfm_spm_hal_setup_isolation_hw(void)
{
#if TFM_LVL != 1
    if (tfm_spm_mpu_init() != SPM_ERR_OK) {
        ERROR_MSG("Failed to set up initial MPU configuration! Halting.");
        while (1) {
            ;
        }
    }
#endif
}

void SEC_VIO_IRQHandler(void)
{
    /* Clear interrupt flag and pending IRQ */
    NVIC_ClearPendingIRQ(SEC_VIO_IRQn);

    /* Print fault message and block execution */
    LOG_MSG("Oops... MPC/PPC fault!!!");

    /* Inform TF-M core that isolation boundary has been violated */
    tfm_access_violation_handler();
}

uint32_t tfm_spm_hal_get_ns_VTOR(void)
{
    return memory_regions.non_secure_code_start;
}

uint32_t tfm_spm_hal_get_ns_MSP(void)
{
    return *((uint32_t *)memory_regions.non_secure_code_start);
}

uint32_t tfm_spm_hal_get_ns_entry_point(void)
{
    return *((uint32_t *)(memory_regions.non_secure_code_start + 4));
}
