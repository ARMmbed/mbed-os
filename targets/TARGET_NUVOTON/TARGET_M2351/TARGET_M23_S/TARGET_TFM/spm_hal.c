/*
 * Copyright (c) 2018-2019, Nuvoton Technology Corporation
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

#include <stdio.h>
#include "platform/include/tfm_spm_hal.h"
#include "spm_api.h"
#include "spm_db.h"
#include "target_cfg.h"
#include "secure_utilities.h"
#include "mbed_error.h"

void tfm_spm_hal_init_isolation_hw(void)
{
    TZ_SAU_Setup();
    SCU_Setup();
    FMC_NSBA_Setup();
}

void tfm_spm_hal_configure_default_isolation(
        const struct tfm_spm_partition_platform_data_t *platform_data)
{
    if (platform_data) {
        /* TODO */
    }
}

#if TFM_LVL > 1

/* TODO */
#error("TFM level 2/3 are not supported yet")

#endif

void tfm_spm_hal_setup_isolation_hw(void)
{
#if TFM_LVL > 1

/* TODO */
#error("TFM level 2/3 are not supported yet")

#endif
}

void SCU_IRQHandler(void)
{
    uint32_t scu_sv_ints = SCU_GET_INT_FLAG(NU_SCU_SV_MSAK);

    /* TODO: More detailed debug message */
    LOG_MSG("Oops... MPC fault!!!");

    /* Clear SCU interrupt flag and pending SCU IRQ */
    SCU_CLR_INT_FLAG(scu_sv_ints);
    NVIC_ClearPendingIRQ(SCU_IRQn);

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
    return *((uint32_t *)(memory_regions.non_secure_code_start+ 4));
}
