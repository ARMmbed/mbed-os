/*
 * Copyright (c) 2015-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>
#include "app_util_platform.h"

#if defined(SOFTDEVICE_PRESENT)
static volatile bool state_saved = false;

static void nordic_nvic_critical_region_enter(void);
static void nordic_nvic_critical_region_exit(void);
#endif

void hal_critical_section_enter()
{
#ifdef NRF52
    ASSERT(APP_LEVEL_PRIVILEGED == privilege_level_get())
#endif

#if defined(SOFTDEVICE_PRESENT)
    /* return value can be safely ignored */
    nordic_nvic_critical_region_enter();
#else
    app_util_disable_irq();
#endif
}

void hal_critical_section_exit()
{
#ifdef NRF52
    ASSERT(APP_LEVEL_PRIVILEGED == privilege_level_get())
#endif

#if defined(SOFTDEVICE_PRESENT)
    /* return value can be safely ignored */
    nordic_nvic_critical_region_exit();
#else
    app_util_enable_irq();
#endif
}


bool hal_in_critical_section(void)
{
    return (state_saved != 0);
}


#if defined(SOFTDEVICE_PRESENT)
/**@brief Enters critical region.
 *
 * @post Application interrupts will be disabled.
 * @sa nordic_nvic_critical_region_exit
 */
static inline void nordic_nvic_critical_region_enter(void)
{
    int was_masked = __sd_nvic_irq_disable();

    if (state_saved == false) {
            nrf_nvic_state.__irq_masks[0] = ( NVIC->ICER[0] & __NRF_NVIC_APP_IRQS_0 );
            NVIC->ICER[0] = __NRF_NVIC_APP_IRQS_0;
#ifdef NRF52
            nrf_nvic_state.__irq_masks[1] = ( NVIC->ICER[1] & __NRF_NVIC_APP_IRQS_1 );
            NVIC->ICER[1] = __NRF_NVIC_APP_IRQS_1;
#endif
    }

    state_saved = true;

    if (!was_masked) {
        __sd_nvic_irq_enable();
    }
}

/**@brief Exit critical region.
 *
 * @pre Application has entered a critical region using ::nordic_nvic_critical_region_enter.
 * @post If not in a nested critical region, the application interrupts will restored to the state before ::nordic_nvic_critical_region_enter was called.
 */
static inline void nordic_nvic_critical_region_exit(void)
{
    state_saved = false;

    int was_masked = __sd_nvic_irq_disable();
    NVIC->ISER[0] = nrf_nvic_state.__irq_masks[0];
#ifdef NRF52
    NVIC->ISER[1] = nrf_nvic_state.__irq_masks[1];
#endif
    if (!was_masked) {
    __sd_nvic_irq_enable();
    }
}
#endif
