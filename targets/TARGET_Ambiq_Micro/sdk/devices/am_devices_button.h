//*****************************************************************************
//
//! @file am_devices_button.h
//!
//! @brief Functions for controlling an array of buttons.
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
#ifndef AM_DEVICES_BUTTON_H
#define AM_DEVICES_BUTTON_H

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// Number of "ticks" to delay before registering a button press or release.
//
//*****************************************************************************
#define AM_DEVICES_BUTTON_DEBOUNCE_DELAY    0x4

//*****************************************************************************
//
// Button polarity macros
//
//*****************************************************************************
#define AM_DEVICES_BUTTON_NORMAL_HIGH       0x1
#define AM_DEVICES_BUTTON_NORMAL_LOW        0x0

//*****************************************************************************
//
// Structure for keeping track of buttons.
//
//*****************************************************************************
typedef struct
{
    uint32_t ui32GPIONumber;
    uint32_t ui32Polarity;
    uint32_t ui32Count;
    bool bPressed;
    bool bChanged;
}
am_devices_button_t;

//*****************************************************************************
//
// Macro for declaring a button structure.
//
//*****************************************************************************
#define AM_DEVICES_BUTTON(ui32GPIONumber, ui32Polarity)                       \
    {ui32GPIONumber, ui32Polarity, 0, 0, 0}

//*****************************************************************************
//
// Macros for checking button state.
//
//*****************************************************************************
#define am_devices_button_is_up(button)                                       \
    ((button).bPressed == false)

#define am_devices_button_is_down(button)                                     \
    ((button).bPressed == true)

#define am_devices_button_pressed(button)                                     \
    (((button).bPressed == true) && ((button).bChanged == true))

#define am_devices_button_released(button)                                    \
    (((button).bPressed == false) && ((button).bChanged == true))


//*****************************************************************************
//
// External function definitions
//
//*****************************************************************************
extern void am_devices_button_init(am_devices_button_t *psButton);

extern void am_devices_button_array_init(am_devices_button_t *psButtons,
                                         uint32_t ui32NumButtons);
extern void am_devices_button_tick(am_devices_button_t *psButton);

extern void am_devices_button_array_tick(am_devices_button_t *psButtons,
                                         uint32_t ui32NumButtons);
#ifdef __cplusplus
}
#endif

#endif // AM_DEVICES_BUTTON_H

