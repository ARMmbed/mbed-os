/***************************************************************************//**
 * @file pinmap.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/

#include "pinmap.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "mbed_assert.h"

static int gpio_clock_inited = 0;

void pin_function(PinName pin, int function)
{
    //Intentionally left empty. We have repurposed the function field.
}

void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT((0xFFFFFF00 |pin) != NC);

    /* Enable GPIO clock if not already done */
    if (!gpio_clock_inited) {
        CMU_ClockEnable(cmuClock_GPIO, true);
        gpio_clock_inited = 1;
    }

    /* Pin and port index encoded in one uint32.
     * First four bits represent the pin number
     * The remaining bits represent the port number */
    uint32_t pin_number = (uint32_t) pin;
    int pin_index = (pin_number & 0xF);
    int port_index = pin_number >> 4;

    GPIO_PinModeSet(port_index, pin_index, mode & 0xF, GPIO_PinOutGet(port_index, pin_index & 0xF));
}

// TODO_LP get pin_mode to be able to store previous settings
