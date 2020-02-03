#include "mbed_assert.h"
#include "pinmap.h"
#include "mbed_error.h"

#include "am_mcu_apollo.h"

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
