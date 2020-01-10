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

#include "cmsis.h"
#include "target_cfg.h"
#include "tfm_secure_api.h"

/* Why rename this file to .cpp from .c
 *
 * NU_TZ_NSC_REGION_START/NU_TZ_NSC_REGION_SIZE would consist of linker-generated symbols.
 * To avoid compile error 'initializer element is not a compile-time constant' with
 * 'memory_regions' by emitted by C compiler, we rename this file to .cpp.
 *
 * With renaming to .cpp, to avoid name mangling for TFM HAL functions by C++ compiler, we
 * declare these functions with modifier 'extern "C"'.
 */

/* Check relevant macro has been defined */
#if (! defined(TZ_START_NS))
#error("TZ_START_NS not defined")
#endif
#if (! defined(NU_ROM_START_NS))
#error("NU_ROM_START_NS not defined")
#endif
#if (! defined(NU_ROM_SIZE_NS))
#error("NU_ROM_SIZE_NS not defined")
#endif
#if (! defined(NU_TZ_NSC_REGION_START))
#error("NU_TZ_NSC_REGION_START not defined")
#endif
#if (! defined(NU_TZ_NSC_REGION_SIZE))
#error("NU_TZ_NSC_REGION_SIZE not defined")
#endif
#if (! defined(SCB_AIRCR_SYSRESETREQS_VAL))
#error("SCB_AIRCR_SYSRESETREQS_VAL not defined")
#endif

const struct memory_region_limits memory_regions = {
    .non_secure_code_start = TZ_START_NS,

    .non_secure_partition_base = NU_ROM_START_NS,
    .non_secure_partition_limit = NU_ROM_START_NS + NU_ROM_SIZE_NS - 1,

    .veneer_base = NU_TZ_NSC_REGION_START,
    .veneer_limit = NU_TZ_NSC_REGION_START + NU_TZ_NSC_REGION_SIZE - 1
};

extern "C" void enable_fault_handlers(void)
{
    /* M2351 doesn't implement Main Extension, so BUS, MEM, USG and Secure faults (SCB->SHCSR) are not supported. */
}

extern "C" void system_reset_cfg(void)
{
    SCB_Setup();
}

extern "C" void tfm_spm_hal_init_debug(void)
{
    /* Configure debug authentication
     *
     * Support macros: DAUTH_NONE/DAUTH_NS_ONLY/DAUTH_FULL/DAUTH_CHIP_DEFAULT
     *
     * On Nuvoton's M2351, there's no need to configure debug authentication because
     * it depends on input signals.
     */
    #warning("Ignore debug authentication option because it depends on input signals") 
}

/* Configures all interrupts of non-secure peripherals to target NS state */
extern "C" void nvic_interrupt_target_state_cfg()
{
    TZ_NVIC_Setup();
}

/* Enables the interrupts associated to the secure peripherals (plus the isolation
 * boundary violation interrupts). */
extern "C" void nvic_interrupt_enable()
{
    /* Enable SCU security violation (isolation boundary violation) interrupts */
    SCU_ENABLE_INT(NU_SCU_SV_MSAK);
    
    /* FIXME: Enable interrupts of secure peripherals */
}
