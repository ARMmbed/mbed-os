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
#include "objects.h"
#include "pinmap.h"
#include "musca_b1_scc_drv.h"
#include "tfm_ioctl_api.h"
#include "rtx_lib.h"


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
    enum tfm_platform_err_t ret = TFM_PLATFORM_ERR_SUCCESS;

    MBED_ASSERT(pin != NC);

    /* Secure service can't be called in interrupt context. */
    if (IsIrqMode()) {
        MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_HAL,
                MBED_ERROR_INVALID_OPERATION),
                "Pin secure service can't be called in interrupt context\n");
         return;
    }

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

    ret =  tfm_platform_set_pin_alt_func(flags, (1u<<pin), &result);
    if (result != SCC_ERR_NONE) {
        error("Can not set pin alt func %d", pin);
    }
    if (ret != TFM_PLATFORM_ERR_SUCCESS) {
        error("tfm_platform_set_pin_alt_func failed for pin %d", pin);
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
    enum tfm_platform_err_t ret = TFM_PLATFORM_ERR_SUCCESS;
    uint32_t result = 0;
    MBED_ASSERT(pin != NC);

    /* Secure service can't be called in interrupt context. */
    if (IsIrqMode()) {
        MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_HAL,
                MBED_ERROR_INVALID_OPERATION),
                "Pin secure service can't be called in interrupt context\n");
         return;
    }

    ret = tfm_platform_set_pin_mode((1u<<pin), translate_pinmode(mode),
                                    &result);
    if (result != SCC_ERR_NONE) {
        error("Can not set pin mode %d", pin);
    }
    if (ret != TFM_PLATFORM_ERR_SUCCESS) {
        error("tfm_platform_set_pin_mode failed for pin %d", pin);
    }
}
