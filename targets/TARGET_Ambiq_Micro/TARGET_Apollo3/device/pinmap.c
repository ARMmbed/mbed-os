/*
 * Copyright (c) 2020 SparkFun Electronics
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "mbed_assert.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "mbed_critical.h"
#include "extensions.h"
#include "am_mcu_apollo.h"

void pin_config(PinName pin, am_hal_gpio_pincfg_t pincfg)
{
    if (pin == (PinName)NC) {
        return;
    }
    MBED_ASSERT(AM_HAL_STATUS_SUCCESS == am_hal_gpio_pinconfig(pin, pincfg));
}

void pinmap_config(PinName pin, const PinMap *map)
{
    // fully configure a pin by a pin map entry
    if (pin == NC) {
        return;
    }

    am_hal_gpio_pincfg_t pincfg;
    while (map->pin != NC) {
        if (map->pin == pin) {
            pincfg = *((am_hal_gpio_pincfg_t *)(map->function));
            pin_config(pin, pincfg);
            return;
        }
        map++;
    }
    MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_PINMAP_INVALID), "could not pinmap_config", pin);
}

void pin_function(PinName pin, int function)
{
#define PADREG_FLD_FNSEL_S 3

    uint32_t ui32Padreg;
    uint32_t ui32Funcsel;

    MBED_ASSERT(pin < AM_HAL_GPIO_MAX_PADS);

    //
    // Initialize the PADREG accumulator variables.
    //
    ui32Padreg = 0;

    // Get the requested function
    ui32Funcsel = (uint32_t)function;

    ui32Padreg |= ui32Funcsel << PADREG_FLD_FNSEL_S;

    //
    // At this point, the configuration variable ui32Padreg,
    //  values is set (at bit position 0) and ready to write
    //  to its respective register bitfields.
    //
    uint32_t ui32PadregAddr;
    uint32_t ui32PadClearMask;
    uint32_t ui32PadShft;

    ui32PadregAddr = AM_REGADDR(GPIO, PADREGA) + (pin & ~0x3);

    ui32PadShft = ((pin & 0x3) << 3);
    ui32PadClearMask = ~((uint32_t)0x38 << ui32PadShft);

    // Get the new values into their rightful bit positions.
    ui32Padreg <<= ui32PadShft;

    core_util_critical_section_enter();

    GPIO->PADKEY = GPIO_PADKEY_PADKEY_Key;

    AM_REGVAL(ui32PadregAddr) = (AM_REGVAL(ui32PadregAddr) & ui32PadClearMask) | ui32Padreg;

    GPIO->PADKEY = 0;

    core_util_critical_section_exit();
}


//pin_mode should not be utilized by apollo3 hal, but is provided to complete the pinmap API
//Use pinmap_config for apollo3 target insted of pinmap_pinout, this will have better hal verification of settings
//if this function does not provide sufficient functionality, try pinmap_config or gpio_mode
void pin_mode(PinName pin, PinMode mode)
{
    am_hal_gpio_pincfg_t bfGpioCfg = {0};
    am_hal_gpio_pincfg_allow_t sAllowableChanges = {0};

    if (mode & (PinMode)PullNone) {
        bfGpioCfg.ePullup = AM_HAL_GPIO_PIN_PULLUP_NONE;
        sAllowableChanges.ePullup = true;
    }
    if (mode & (PinMode)PullUp) {
        bfGpioCfg.ePullup = AM_HAL_GPIO_PIN_PULLUP_WEAK;
        sAllowableChanges.ePullup = true;
    }
    if (mode & (PinMode)PullDown) {
        bfGpioCfg.ePullup = AM_HAL_GPIO_PIN_PULLDOWN;
        sAllowableChanges.ePullup = true;
    }

    ap3_hal_gpio_pinconfig_partial(pin, bfGpioCfg, sAllowableChanges);
}
