//*****************************************************************************
//
//! @file am_devices_led.h
//!
//! @brief Functions for controlling an array of LEDs
//!
//! @addtogroup devices External Device Control Library
//! @addtogroup LED SPI Device Control for programmable LEDs.
//! @ingroup devices
//! @{
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
#ifndef AM_DEVICES_LED_H
#define AM_DEVICES_LED_H

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// LED polarity macros
//
//*****************************************************************************
#define AM_DEVICES_LED_POL_POLARITY_M       0x1
#define AM_DEVICES_LED_ON_HIGH              0x1
#define AM_DEVICES_LED_ON_LOW               0x0

//*****************************************************************************
//
// LED direct drive indicator macro
// Or this in with the polarity value to use the GPIO DATA register instead of
// the GPIO DATA ENABLE register to directly drive an LED buffer.
//
//*****************************************************************************
#define AM_DEVICES_LED_POL_DIRECT_DRIVE_M   0x2

//*****************************************************************************
//
// Structure for keeping track of LEDs
//
//*****************************************************************************
typedef struct
{
    uint32_t ui32GPIONumber;
    uint32_t ui32Polarity;
}
am_devices_led_t;

//*****************************************************************************
//
// External function definitions
//
//*****************************************************************************
extern void am_devices_led_init(am_devices_led_t *psLED);
extern void am_devices_led_array_init(am_devices_led_t *psLEDs, uint32_t ui32NumLEDs);
extern void am_devices_led_array_disable(am_devices_led_t *psLEDs, uint32_t ui32NumLEDs);
extern void am_devices_led_on(am_devices_led_t *psLEDs, uint32_t ui32LEDNum);
extern void am_devices_led_off(am_devices_led_t *psLEDs, uint32_t ui32LEDNum);
extern void am_devices_led_toggle(am_devices_led_t *psLEDs, uint32_t ui32LEDNum);
extern bool am_devices_led_get(am_devices_led_t *psLEDs, uint32_t ui32LEDNum);
extern void am_devices_led_array_out(am_devices_led_t *psLEDs, uint32_t ui32NumLEDs,
                                     uint32_t ui32Value);
#ifdef __cplusplus
}
#endif

#endif // AM_DEVICES_LED_H

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
