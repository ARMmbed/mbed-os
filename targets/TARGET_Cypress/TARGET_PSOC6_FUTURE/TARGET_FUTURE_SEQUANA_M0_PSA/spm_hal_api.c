/* Copyright (c) 2017-2018 ARM Limited
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


/* -------------------------------------- Includes ----------------------------------- */

#include "cy_sysint.h"
#include "spm_internal.h"

#ifdef PU_ENABLE
#include "cyprotection_config.h"
#endif // PU_ENABLE


/* -------------------------------------- HAL API ------------------------------------ */

// These implementations are meant to be used only for SPM running on PSoC6 M0+ core.

void spm_hal_start_nspe(void)
{
    Cy_SysEnableCM4(PSA_NON_SECURE_ROM_START);
}

void spm_hal_memory_protection_init(void)
{
#ifdef PU_ENABLE
    cy_en_prot_status_t status;

    /* smpu */
    status = smpu_protect((cy_smpu_region_config_t *)flash_spm_smpu_config, sizeof(flash_spm_smpu_config) / sizeof(flash_spm_smpu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    status = smpu_protect((cy_smpu_region_config_t *)sram_spm_smpu_config, sizeof(sram_spm_smpu_config) / sizeof(sram_spm_smpu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead

    /* fixed region ppu */
    status = ppu_fixed_rg_protect((cy_ppu_fixed_rg_cfg_t *)fixed_rg_pc0_ppu_config, sizeof(fixed_rg_pc0_ppu_config) / sizeof(fixed_rg_pc0_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    status = ppu_fixed_rg_protect((cy_ppu_fixed_rg_cfg_t *)fixed_rg_spm_ppu_config, sizeof(fixed_rg_spm_ppu_config) / sizeof(fixed_rg_spm_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    status = ppu_fixed_rg_protect((cy_ppu_fixed_rg_cfg_t *)fixed_rg_any_ppu_config, sizeof(fixed_rg_any_ppu_config) / sizeof(fixed_rg_any_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    /* fixed slave ppu */
    status = ppu_fixed_sl_protect((cy_ppu_fixed_sl_cfg_t *)fixed_sl_pc0_ppu_config, sizeof(fixed_sl_pc0_ppu_config) / sizeof(fixed_sl_pc0_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    status = ppu_fixed_sl_protect((cy_ppu_fixed_sl_cfg_t *)fixed_sl_spm_ppu_config, sizeof(fixed_sl_spm_ppu_config) / sizeof(fixed_sl_spm_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    status = ppu_fixed_sl_protect((cy_ppu_fixed_sl_cfg_t *)fixed_sl_any_ppu_config, sizeof(fixed_sl_any_ppu_config) / sizeof(fixed_sl_any_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    /* programmable ppu */
    status = ppu_prog_protect((cy_ppu_prog_cfg_t *)prog_pc0_ppu_config, sizeof(prog_pc0_ppu_config) / sizeof(prog_pc0_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    status = ppu_prog_protect((cy_ppu_prog_cfg_t *)prog_spm_ppu_config, sizeof(prog_spm_ppu_config) / sizeof(prog_spm_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    /* fixed group ppu */
    status = ppu_fixed_gr_protect((cy_ppu_fixed_gr_cfg_t *)fixed_gr_pc0_ppu_config, sizeof(fixed_gr_pc0_ppu_config) / sizeof(fixed_gr_pc0_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
    status = ppu_fixed_gr_protect((cy_ppu_fixed_gr_cfg_t *)fixed_gr_spm_ppu_config, sizeof(fixed_gr_spm_ppu_config) / sizeof(fixed_gr_spm_ppu_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead

    /* bus masters */
    status = bus_masters_protect((cy_bus_master_config_t *)bus_masters_config, sizeof(bus_masters_config) / sizeof(bus_masters_config[0]));
    CY_ASSERT(status == CY_PROT_SUCCESS);  // TODO: Panic instead
#endif // PU_ENABLE
}
