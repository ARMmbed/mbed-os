/*
 * Copyright (c) 2017-2018, Nuvoton Technology Corporation
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

#include <arm_cmse.h>
#include "mbed_assert.h"
#include "pinmap.h"
#include "PortNames.h"
#include "PeripheralNames.h"
#include "mbed_error.h"
#include "partition_M2351.h"
#include "hal_secure.h"
#if defined(DOMAIN_NS) && (DOMAIN_NS == 1L) && (TFM_LVL > 0)
#include "tfm_ns_lock.h"
#endif

/**
 * Configure pin multi-function
 */
void pin_function(PinName pin, int data)
{
    pin_function_s(pin, data);
}

/**
 * Configure pin pull-up/pull-down
 */
void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != (PinName)NC);
    uint32_t pin_index = NU_PINNAME_TO_PIN(pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(pin);
    GPIO_T *gpio_base = NU_PORT_BASE(port_index);

    uint32_t mode_intern = GPIO_MODE_INPUT;

    switch (mode) {
        case InputOnly:
            mode_intern = GPIO_MODE_INPUT;
            break;

        case PushPullOutput:
            mode_intern = GPIO_MODE_OUTPUT;
            break;

        case OpenDrain:
            mode_intern = GPIO_MODE_OPEN_DRAIN;
            break;

        case QuasiBidirectional:
            mode_intern = GPIO_MODE_QUASI;
            break;

        default:
            /* H/W doesn't support separate configuration for input pull mode/direction.
             * We expect upper layer would have translated input pull mode/direction
             * to I/O mode */
            return;
    }

    GPIO_SetMode(gpio_base, 1 << pin_index, mode_intern);

    /* Invalid combinations of PinMode/PinDirection
     *
     * We assume developer would avoid the following combinations of PinMode/PinDirection
     * which are invalid:
     * 1. InputOnly/PIN_OUTPUT
     * 2. PushPullOutput/PIN_INPUT
     */
}

#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)

static void pin_function_impl(int32_t pin, int32_t data, bool nonsecure_caller)
{
    MBED_ASSERT(pin != (PinName)NC);
    uint32_t pin_index = NU_PINNAME_TO_PIN(pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(pin);

    /* Guard access to secure GPIO from non-secure domain */
    if (nonsecure_caller &&
        (! (SCU_INIT_IONSSET_VAL & (1 << (port_index + 0))))) {
        error("Non-secure domain tries to control secure or undefined GPIO.");
    }

    __IO uint32_t *GPx_MFPx = ((__IO uint32_t *) &SYS->GPA_MFPL) + port_index * 2 + (pin_index / 8);
    uint32_t MFP_Msk = NU_MFP_MSK(pin_index);

    // E.g.: SYS->GPA_MFPL  = (SYS->GPA_MFPL & (~SYS_GPA_MFPL_PA0MFP_Msk) ) | SYS_GPA_MFPL_PA0MFP_SC0_CD  ;
    *GPx_MFPx  = (*GPx_MFPx & (~MFP_Msk)) | data;
}

#if (TFM_LVL > 0)
__NONSECURE_ENTRY
int32_t pin_function_veneer(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3)
{
    int32_t pin = (int32_t) arg0;
    int32_t data = (int32_t) arg1;
    pin_function_impl(pin, data, cmse_nonsecure_caller());
    return 0;
}
#endif

#endif

#if defined(DOMAIN_NS) && (DOMAIN_NS == 1) && (TFM_LVL > 0)

void pin_function_s(int32_t pin, int32_t data)
{
    tfm_ns_lock_dispatch(pin_function_veneer, pin, data, 0, 0);
}

#elif defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)

#if (TFM_LVL == 0)
__NONSECURE_ENTRY
#endif
void pin_function_s(int32_t pin, int32_t data)
{
    pin_function_impl(pin, data, cmse_nonsecure_caller());
}

#endif
