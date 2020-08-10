/*
Copyright (c) 2019 SparkFun Electronics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "mbed_assert.h"
#include "pinmap.h"
#include "mbed_error.h"

#include "extensions.h"
#include "am_mcu_apollo.h"

void pin_config(PinName pin, am_hal_gpio_pincfg_t pincfg){
    if(pin == (PinName)NC){ return; }
    MBED_ASSERT(AM_HAL_STATUS_SUCCESS == am_hal_gpio_pinconfig(pin, pincfg));
}

void pinmap_config(PinName pin, const PinMap *map) {
    // fully configure a pin by a pin map entry
    if (pin == NC) {
        return;
    }

    am_hal_gpio_pincfg_t pincfg;
    while (map->pin != NC) {
        if (map->pin == pin) {
            pincfg = *((am_hal_gpio_pincfg_t*)(map->function));
            pin_config(pin, pincfg);
            return;
        }
        map++;
    }
    MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_PINMAP_INVALID), "could not pinmap_config", pin);
}

void pin_function(PinName pin, int function)
{
    // am_hal_gpio_pincfg_t cfg = {0};
    // cfg.uFuncSel = function;
    // am_hal_gpio_pinconfig((uint32_t)(pin), cfg); // apply configuration

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

    AM_CRITICAL_BEGIN

    GPIO->PADKEY = GPIO_PADKEY_PADKEY_Key;

    AM_REGVAL(ui32PadregAddr) = (AM_REGVAL(ui32PadregAddr) & ui32PadClearMask) | ui32Padreg;

    GPIO->PADKEY = 0;

    AM_CRITICAL_END
}

void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(0);
    // gpio_t obj = {
    //     .pad = (ap3_gpio_pad_t)pin,
    //     .cfg = {0},
    // };
    // gpio_mode(gpio_t * obj, PinMode mode)
}
