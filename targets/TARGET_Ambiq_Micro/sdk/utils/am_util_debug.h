//*****************************************************************************
//
//! @file am_util_debug.h
//!
//! @brief Useful functions for debugging.
//!
//! These functions and macros were created to assist with debugging. They are
//! intended to be as unintrusive as possible and designed to be removed from
//! the compilation of a project when they are no longer needed.
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
// SPDX-License-Identifier: BSD-3-Clause
#ifndef AM_UTIL_DEBUG_H
#define AM_UTIL_DEBUG_H

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// Debug printf macros.
//
//*****************************************************************************
#ifdef AM_DEBUG_PRINTF

#define am_util_debug_printf_init(x)                                          \
    am_util_stdio_printf_init(x);

#define am_util_debug_printf(...)                                             \
    am_util_stdio_printf(__VA_ARGS__);

#else

#define am_util_debug_printf_init(...)
#define am_util_debug_printf(...)

#endif // AM_DEBUG_PRINTF

//*****************************************************************************
//
// Debug trace macros.
//
//*****************************************************************************
#ifdef AM_DEBUG_TRACE

#define am_util_debug_trace_init(PinNumber)                                   \
    do                                                                        \
    {                                                                         \
        am_hal_gpio_out_bit_clear(PinNumber);                                 \
        am_hal_gpio_pin_config(PinNumber, AM_HAL_GPIO_OUTPUT);                \
    }                                                                         \
    while(0)


#define am_util_debug_trace_start(PinNumber)                                  \
    am_hal_gpio_out_bit_set(PinNumber)

#define am_util_debug_trace_end(PinNumber)                                    \
    am_hal_gpio_out_bit_clear(PinNumber)

#else

#define am_util_debug_trace_init(PinNumber)
#define am_util_debug_trace_start(PinNumber)
#define am_util_debug_trace_end(PinNumber)

#endif // AM_DEBUG_TRACE


#ifdef __cplusplus
}
#endif

#endif // AM_UTIL_DEBUG_H

