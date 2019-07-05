/* mbed Microcontroller Library
 * Copyright (c) 2019 Arm Limited
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

#include "mbed_assert.h"
#include "mbed_error.h"
#include "pinmap.h"
#include "objects.h"
#include "musca_a1_scc_drv.h"

#if TARGET_MUSCA_A1_NS

const PinMap PinMap_UART_TX[] = {
    {UART0_TX, UART_0, ALTERNATE_FUNC_1},
    {UART1_TX, UART_1, PRIMARY_FUNC},
    {NC,  NC, 0}
};

const PinMap PinMap_UART_RX[] = {
    {UART0_RX, UART_0, ALTERNATE_FUNC_1},
    {UART1_RX, UART_1, PRIMARY_FUNC},
    {NC,  NC, 0}
};

/**
 * \brief Configures the GPIO pin and sets the alternate function
 *
 * \param[in] pin       GPIO pin number \ref PinName
 * \param[in] function  Alternate function to set \ref PinFunction
 */
void pin_function(PinName pin, int function)
{
    enum gpio_altfunc_t flags;

    MBED_ASSERT(pin != NC);

    /* The pin has to be a GPIO pin */
    if (pin >= PA0 && pin <= PA15) {
        switch (function) {
            case PRIMARY_FUNC:
                flags = GPIO_MAIN_FUNC;
                break;
            case ALTERNATE_FUNC_1:
                flags = GPIO_ALTFUNC_1;
                break;
            case ALTERNATE_FUNC_2:
                flags = GPIO_ALTFUNC_2;
                break;
            case ALTERNATE_FUNC_3:
                flags = GPIO_ALTFUNC_3;
                break;
            default:
                return;
        }

#ifdef MUSCA_A1_SCC_DEV
    musca_a1_scc_set_alt_func(&MUSCA_A1_SCC_DEV, flags, (1u<<pin));
#endif /* MUSCA_A1_SCC_DEV */
    }
}

/**
 * \brief Translates between different pin mode enums
 *
 * \param[in] mode    Pin mode to translate \ref PinMode
 *
 * \return   Translated pin mode \ref pinmode_select_t
 */
static enum pinmode_select_t translate_pinmode(PinMode mode)
{
    switch (mode) {
        case PullNone:
            return PINMODE_NONE;
        case PullDown:
            return PINMODE_PULL_DOWN;
        case PullUp:
            return PINMODE_PULL_UP;
        default:
            return PINMODE_NONE;
    }
}

/**
 * \brief Sets pin mode for the given GPIO pin
 *
 * \param[in] pin     GPIO pin number \ref PinName
 * \param[in] mode    Pin mode to set \ref PinMode
 */
void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != NC);

#ifdef MUSCA_A1_SCC_DEV
    musca_a1_scc_set_pinmode(&MUSCA_A1_SCC_DEV, (1u<<pin), translate_pinmode(mode));
#endif /* MUSCA_A1_SCC_DEV */

}

#else // TARGET_MUSCA_A1_NS

void pin_function(PinName pin, int function)
{
}
void pin_mode(PinName pin, PinMode mode)
{
}

#endif
