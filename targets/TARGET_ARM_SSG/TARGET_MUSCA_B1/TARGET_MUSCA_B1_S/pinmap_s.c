/* mbed Microcontroller Library
 * Copyright (c) 2017-2020 Arm Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * This file implements APIS defined in hal/pinmap.h
 *
 * Pin functions are not available in interrupt context, because the
 * NS side is only allowed to call TF-M secure functions (veneers) from
 * the NS Thread mode.
 *
 */

#include "mbed_assert.h"
#include "mbed_error.h"
#include "device_definition.h"
#include "objects.h"
#include "pinmap.h"
#include "musca_b1_scc_drv.h"

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
 * \brief Configures the GPIO pin and sets the alternate function
 *
 * \param[in] pin       GPIO pin number \ref PinName
 * \param[in] function  Alternate function to set \ref PinFunction
 */
void pin_function(PinName pin, int function)
{
    enum gpio_altfunc_t flags;
    uint32_t result = 0;

    MBED_ASSERT(pin != NC);

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

#ifdef MUSCA_B1_SCC_DEV
    result = musca_b1_scc_set_alt_func(&MUSCA_B1_SCC_DEV, flags, (1u<<pin));
    if (result != SCC_ERR_NONE) {
        error("Can not set pin alt func %d", pin);
    }
#else
    error("SCC device associated with expansion pin %d, is disabled", pin);
#endif
}

/**
 * \brief Sets pin mode for the given GPIO pin
 *
 * \param[in] pin     GPIO pin number \ref PinName
 * \param[in] mode    Pin mode to set \ref PinMode
 */
void pin_mode(PinName pin, PinMode mode)
{
    uint32_t result = 0;

    MBED_ASSERT(pin != NC);

#ifdef MUSCA_B1_SCC_DEV
    result = musca_b1_scc_set_pinmode(&MUSCA_B1_SCC_DEV, (1u<<pin),
                                      translate_pinmode(mode));
    if (result != SCC_ERR_NONE) {
        error("Can not set pin mode %d", pin);
    }
#else
    error("SCC device associated with expansion pin %d, is disabled", pin);
#endif
}
