//*****************************************************************************
//
//! @file am_util_stdio.h
//!
//! @brief A few printf-style functions for use with Ambiq products
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
#ifndef AM_UTIL_STDIO_H
#define AM_UTIL_STDIO_H

/* get va_list from compiler. */
#include <stdarg.h>

#ifdef __cplusplus
extern "C"
{
#endif
//*****************************************************************************
//
// Macro definitions
//
//*****************************************************************************

// buffer size for printf
#ifndef AM_PRINTF_BUFSIZE
#define AM_PRINTF_BUFSIZE              256
#endif

typedef void (*am_util_stdio_print_char_t)(char *pcStr);

//*****************************************************************************
//
// External function definitions
//
//*****************************************************************************
extern void am_util_stdio_printf_init(am_util_stdio_print_char_t pfnCharPrint);
extern uint32_t am_util_stdio_strtoul(const char *str, char **endptr, int base);
extern bool am_util_stdio_textmode_set(bool bSetTextTranslationMode);
extern uint32_t am_util_stdio_vsprintf(char *pcBuf, const char *pcFmt, va_list pArgs);
extern uint32_t am_util_stdio_sprintf(char *pui8Buf, const char *pui8Fmt, ...);
extern uint32_t am_util_stdio_printf(const char *pui8Fmt, ...);
extern void am_util_stdio_terminal_clear(void);

#ifdef __cplusplus
}
#endif

#endif // AM_UTIL_STDIO_H

