//*****************************************************************************
//
//! @file am_devices_button.c
//!
//! @brief Functions for controlling an array of LEDs
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2020, Ambiq Micro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision 2.4.2 of the AmbiqSuite Development Package.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "am_mcu_apollo.h"
#include "am_devices_button.h"

//*****************************************************************************
//
//! @brief Configures the necessary pins for an array of buttons.
//!
//! @param psButton is a pointer to a button structure.
//!
//! This function configures a GPIO to read a button in a low-power way.
//!
//! @return None.
//
//*****************************************************************************
void
am_devices_button_init(am_devices_button_t *psButton)
{
    //
    // Disable the pin to save power.
    //
#if AM_APOLLO3_GPIO
    am_hal_gpio_pinconfig(psButton->ui32GPIONumber, g_AM_HAL_GPIO_DISABLE);
#else // AM_APOLLO3_GPIO
    am_hal_gpio_pin_config(psButton->ui32GPIONumber, AM_HAL_PIN_DISABLE);
#endif // AM_APOLLO3_GPIO


    //
    // Initialize the state variables.
    //
    psButton->ui32Count = 0;
    psButton->bPressed = false;
    psButton->bChanged = false;
}

//*****************************************************************************
//
//! @brief Configures the necessary pins for an array of buttons.
//!
//! @param psButtons is an array of button structures.
//! @param ui32NumButtons is the total number of buttons in the array.
//!
//! This function configures the GPIOs for an array of buttons.
//!
//! @return None.
//
//*****************************************************************************
void
am_devices_button_array_init(am_devices_button_t *psButtons,
                             uint32_t ui32NumButtons)
{
    uint32_t i;

    //
    // Loop through the list of buttons, configuring each one individually.
    //
    for ( i = 0; i < ui32NumButtons; i++ )
    {
        am_devices_button_init(psButtons + i);
    }
}

//*****************************************************************************
//
//! @brief Configures the necessary pins for an array of buttons.
//!
//! @param psButtons is an array of button structures.
//! @param ui32NumButtons is the total number of buttons in the array.
//!
//! This function configures the GPIOs for an array of buttons.
//!
//! @return None.
//
//*****************************************************************************
void
am_devices_button_tick(am_devices_button_t *psButton)
{
    uint32_t ui32PinState;
    bool bRawButtonPressed;

    //
    // Enable the button pin.
    //
#if AM_APOLLO3_GPIO
    am_hal_gpio_pinconfig(psButton->ui32GPIONumber, g_AM_HAL_GPIO_INPUT);
#else // AM_APOLLO3_GPIO
    am_hal_gpio_pin_config(psButton->ui32GPIONumber, AM_HAL_PIN_INPUT);
#endif // AM_APOLLO3_GPIO

    //
    // Read the pin state. If the pin is in its normal (unpressed) state, set
    // its "state" counter to zero.
    //
#if AM_APOLLO3_GPIO
    am_hal_gpio_state_read(psButton->ui32GPIONumber, AM_HAL_GPIO_INPUT_READ, &ui32PinState);
#else // AM_APOLLO3_GPIO
    ui32PinState = am_hal_gpio_input_bit_read(psButton->ui32GPIONumber);
#endif // AM_APOLLO3_GPIO

    //
    // Check to see if the button is "pressed" according to our GPIO reading.
    //
    bRawButtonPressed = (ui32PinState != psButton->ui32Polarity);

    //
    // Is this button state different from the last saved state?
    //
    if ( bRawButtonPressed != psButton->bPressed )
    {
        //
        // If so, increase the debounce count.
        //
        psButton->ui32Count++;
    }
    else
    {
        //
        // Otherwise, set the count back to zero.
        //
        psButton->ui32Count = 0;
    }

    //
    // If we hit the button debounce delay, record a button press to the
    // structure, and reset the count.
    //
    if ( psButton->ui32Count >= AM_DEVICES_BUTTON_DEBOUNCE_DELAY )
    {
        psButton->bPressed = bRawButtonPressed;
        psButton->bChanged = true;
        psButton->ui32Count = 0;
    }
    else
    {
        //
        // If we didn't just record a press/release event, update the structure
        // to say that the current state isn't new.
        //
        psButton->bChanged = false;
    }

    //
    // Disable the button pin to save power.
    //
#if AM_APOLLO3_GPIO
    am_hal_gpio_pinconfig(psButton->ui32GPIONumber, g_AM_HAL_GPIO_DISABLE);
#else // AM_APOLLO3_GPIO
    am_hal_gpio_pin_config(psButton->ui32GPIONumber, AM_HAL_PIN_DISABLE);
#endif // AM_APOLLO3_GPIO
}

//*****************************************************************************
//
//! @brief Configures the necessary pins for an array of buttons.
//!
//! @param psButtons is an array of button structures.
//! @param ui32NumButtons is the total number of buttons in the array.
//!
//! This function configures the GPIOs for an array of buttons.
//!
//! @return None.
//
//*****************************************************************************
void
am_devices_button_array_tick(am_devices_button_t *psButtons,
                             uint32_t ui32NumButtons)
{
    uint32_t i;

    //
    // Run the "tick" function for each button in the list.
    //
    for ( i = 0; i < ui32NumButtons; i++ )
    {
        am_devices_button_tick(psButtons + i);
    }
}
