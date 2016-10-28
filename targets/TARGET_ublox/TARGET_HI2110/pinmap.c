/* mbed Microcontroller Library
 * Copyright (c) 2016 u-blox
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

/* As well as claiming and setting pins, the functions here also need
 * to take into account the way the pins are powered.  On the Boudica
 * chip they are arranged in three banks, PIO 0:5, PIO 6:10 and
 * PIO 11:19.
 *
 * The arrangement for which PIO bank is powered is determined by the module
 * in which the HI2110 chip is mounted, hence the use of conditional
 * compilation below.
 */

#include "stdbool.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "pinmap.h"

/* ----------------------------------------------------------------
 * MACROS
 * ----------------------------------------------------------------*/

#define HAL_PIO_MASK_FUNC (0xFF)
#define HAL_PIO_MODULO_4_MASK (0x3)

/* ----------------------------------------------------------------
 * GLOBAL VARIABLES
 * ----------------------------------------------------------------*/

/* ----------------------------------------------------------------
 * FUNCTION PROTOTYPES
 * ----------------------------------------------------------------*/

static inline uint32_t clr_mask (PinName pin);
static inline uint32_t set_mask (PinName pin, int function);
static inline volatile uint32_t * func_reg (PinName pin);

/* ----------------------------------------------------------------
 * NON-API FUNCTIONS
 * ----------------------------------------------------------------*/

// Return the clear mask for a pin
static inline uint32_t clr_mask (PinName pin)
{
    return HAL_PIO_MASK_FUNC << ((pin & HAL_PIO_MODULO_4_MASK) << 3);
}

// Return the set mask for a pin and a given function
static inline uint32_t set_mask (PinName pin, int function)
{
    return function << ((pin & HAL_PIO_MODULO_4_MASK) << 3);
}

// Return the function register for a pin
static inline volatile uint32_t * func_reg (PinName pin)
{
    return &PIO_FUNC0 + (pin >> 2);
}

// Return the owner of a pin
// 0: None
// 1: security core
// 2: protocol core
// 3: apps core
static inline uint8_t get_owner(PinName pin)
{
    uint8_t pio_owner_shift = (pin & 0x0F) << 1;
    volatile uint32_t * pio_owner_reg = (&PIO_OWNER0 + (pin >> 4));

    return 0x03 & (*pio_owner_reg >> pio_owner_shift);
}

/* ----------------------------------------------------------------
 * MBED "INTERNAL" API CALLS
 * ----------------------------------------------------------------*/

void pin_function(PinName pin, int function)
{
    volatile uint32_t *pio_func_reg;

    /* Set the function for the given pin */
    pio_func_reg = func_reg (pin);
    *pio_func_reg = (*pio_func_reg & ~(clr_mask(pin))) | set_mask(pin, function);

    /* Power the pin */
#ifdef TARGET_SARA_NBIOT
    /* On Sara NBIoT, GPIO pin 19 has to be high to power GPIO pins 0 to 10 */
    if ((pin >= p0) && (pin <= p10)) {
        /* Grab pin 19 as a GPIO if we don't have it already */
        if (get_owner(p19) != 0x03) {
            pio_func_reg = func_reg (p19);
            *pio_func_reg = (*pio_func_reg & ~(clr_mask(p19))) | set_mask(p19, 1); /* 1 == PIN_FUNCTION_GPIO */

            MBED_ASSERT (get_owner(p19) == 0x03);
        }

        /* Set pin 19 to be an output and to be high */
        GPIO_DIR |= (1ul << p19);
        GPIO_OUT_BITSET = (1ul << p19);

        /* Note: the level on pins 6 to 10 is controlled by the protocol
         * processor to be the VCC level required by the SIM.  The
         * application has no control over this. */
    }
    /* The power to GPIOs 11 to 19 is fed directly from pin 51 of the module */
#endif
}

void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != (PinName)NC);

    switch (mode) {
        case PullUp:
        {
            MBED_ASSERT(false);  /* Not currently supported on HI2100 */
        }
        break;
        case PullDown:
        {
            GPIO_PULLEN_BITSET = 1U << pin;
        }
        break;
        case PullNone:
        {
            GPIO_PULLEN_BITCLR = 1U << pin;
        }
        break;
        default:
        break;
    }
}
