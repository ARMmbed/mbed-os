/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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

#include "mbed_assert.h"
#include "pinmap.h"
#include "PortNames.h"
#include "mbed_error.h"

/**
 * Configure pin multi-function
 */
void pin_function(PinName pin, int data)
{
    MBED_ASSERT(pin != (PinName)NC);
    uint32_t pin_index = NU_PINNAME_TO_PIN(pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(pin);
    __IO uint32_t *GPx_MFPx = ((__IO uint32_t *) &SYS->GPA_MFPL) + port_index * 2 + (pin_index / 8);
    uint32_t MFP_Msk = NU_MFP_MSK(pin_index);

    // E.g.: SYS->GPA_MFPL  = (SYS->GPA_MFPL & (~SYS_GPA_MFPL_PA0MFP_Msk) ) | SYS_GPA_MFPL_PA0MFP_SC0_CD  ;
    *GPx_MFPx  = (*GPx_MFPx & (~MFP_Msk)) | data;
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
    case PullUp:
        mode_intern = GPIO_MODE_INPUT;
        break;

    case PullDown:
    case PullNone:
        // NOTE: Not support
        return;

    case PushPull:
        mode_intern = GPIO_MODE_OUTPUT;
        break;

    case OpenDrain:
        mode_intern = GPIO_MODE_OPEN_DRAIN;
        break;

    case Quasi:
        mode_intern = GPIO_MODE_QUASI;
        break;
    }

    GPIO_SetMode(gpio_base, 1 << pin_index, mode_intern);
}
