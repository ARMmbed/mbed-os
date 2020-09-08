//*****************************************************************************
//
//! @file am_util_stdio.c
//!
//! @brief A few printf-style functions for use with Ambiq products
//!
//! Functions for performing printf-style operations without dynamic memory
//! allocation.
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

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include "am_util_stdio.h"

//*****************************************************************************
//
// Global Variables
//
//*****************************************************************************

// function pointer for printf
am_util_stdio_print_char_t g_pfnCharPrint;

// buffer for printf
static char g_prfbuf[AM_PRINTF_BUFSIZE];

// Flag to do conversion of '\n' to '\n\r' in sprintf()
static bool g_bTxtXlate = false;

//*****************************************************************************
//
//! @brief Sets the interface for printf calls.
//!
//! @param pfnCharPrint - Function pointer to be used to print to interface
//!
//! This function initializes the global print function which is used for
//! printf. This allows users to define their own printf interface and pass it
//! in as a am_util_stdio_print_char_t type.
//!
//! @return None.
//
//*****************************************************************************
void
am_util_stdio_printf_init(am_util_stdio_print_char_t pfnCharPrint)
{
    g_pfnCharPrint = pfnCharPrint;
}

//*****************************************************************************
//
//! @brief Converts strings to 32-bit unsigned integers.
//!
//! @param *str - Pointer to the string to convert
//! @param **endptr - strtoul will set this to the char after the converted num
//! @param base - Base of the number as written in the input string.
//!
//! This function was implemented in a way similar to the strtoul function
//! found in the C standard library. It will attempt to extract a numerical
//! value from the input string, and return it to the caller. Invalid input
//! strings will return a value of zero.
//!
//! @return uint32_t representing the numerical value from the string.
//
//*****************************************************************************
uint32_t
am_util_stdio_strtoul(const char *str, char **endptr, int base)
{
    char *pos;
    uint32_t ui32BaseVal;
    uint32_t ui32RetVal;

    //
    // Prepare a pointer to start advancing through the string.
    //
    pos = (char *)str;

    //
    // Determine what base we are using. Default to '16', but change to other
    // values as specified by the user. If the user didn't specify anything,
    // try to guess the base value from looking at the first few characters of
    // the input
    //
    ui32BaseVal = 16;

    //
    // Switch to octal for a leading zero
    //
    if (*pos == '0')
    {
        ui32BaseVal = 8;
        pos++;

        //
        // Switch back to hex for a leading '0x'
        //
        if (*pos == 'x')
        {
            ui32BaseVal = 16;
            pos++;
        }
    }

    //
    // No matter what, if the user specified a base value, use that as the real
    // base value.
    //
    if (base)
    {
        ui32BaseVal = base;
    }

    //
    // Start accumulating the converted integer value
    //
    ui32RetVal = 0;

    //
    // Loop through the digits, one character at a time. End the loop if the
    // number is out of range
    //
    while ((*pos >= 'a' && *pos <= 'f' && ui32BaseVal == 16) ||
           (*pos >= 'A' && *pos <= 'F' && ui32BaseVal == 16) ||
           (*pos >= '0' && *pos <= '9'))
    {
        //
        // Make sure to stop if we hit a NULL byte.
        //
        if (*pos == 0)
        {
            break;
        }

        //
        // Multiply by the base value to move up one 'digit'
        //
        ui32RetVal *= ui32BaseVal;

        //
        // Add the value of the next character.
        //
        if (*pos >= '0' && *pos <= '9')
        {
            ui32RetVal += *pos - '0';
        }
        else if (*pos >= 'A' && *pos <= 'F')
        {
            ui32RetVal += (*pos - 'A') + 10;
        }
        else
        {
            ui32RetVal += (*pos - 'a') + 10;
        }

        //
        // Grab the next character.
        //
        pos++;
    }

    //
    // If we get here, hopefully it means that we have parsed a number
    // correctly. The 'pos' pointer should already be pointing at the character
    // right after the last valid number, so set the enptr appropriately, and
    // return the calculated numerical value of the string.
    //
    if (endptr)
    {
        *endptr = pos;
    }

    return ui32RetVal;
}

//*****************************************************************************
//
//  Divide an unsigned 32-bit value by 10.
//
//  Note: Adapted from Ch10 of Hackers Delight (hackersdelight.org).
//
//*****************************************************************************
static uint64_t
divu64_10(uint64_t ui64Val)
{
    uint64_t q64, r64;
    uint32_t q32, r32, ui32Val;

    //
    // If a 32-bit value, use the more optimal 32-bit routine.
    //
    if ( ui64Val >> 32 )
    {
        q64 = (ui64Val>>1) + (ui64Val>>2);
        q64 += (q64 >> 4);
        q64 += (q64 >> 8);
        q64 += (q64 >> 16);
        q64 += (q64 >> 32);
        q64 >>= 3;
        r64 = ui64Val - q64*10;
        return q64 + ((r64 + 6) >> 4);
    }
    else
    {
        ui32Val = (uint32_t)(ui64Val & 0xffffffff);
        q32 = (ui32Val>>1) + (ui32Val>>2);
        q32 += (q32 >> 4);
        q32 += (q32 >> 8);
        q32 += (q32 >> 16);
        q32 >>= 3;
        r32 = ui32Val - q32*10;
        return (uint64_t)(q32 + ((r32 + 6) >> 4));
    }
}

//*****************************************************************************
//
// Return the number of decimal digits in an uint64_t.
//
// example: 10000 return 5, 123 returns 3.
//
//*****************************************************************************
static int
ndigits_in_u64(uint64_t ui64Val)
{
    int iNDigits = ui64Val ? 0 : 1;

    while ( ui64Val )
    {
        //
        // ui32Val /= 10;
        //
        ui64Val = divu64_10(ui64Val);
        ++iNDigits;
    }

    return iNDigits;
}

//*****************************************************************************
//
// Return the number of decimal digits in a 64-bit integer.
//
// Note: Does not include the '-' sign.
//
// example: -3 returns 1, 3 returns 1, 15 returns 2, -15 returns 2, ...
//
//*****************************************************************************
static int
ndigits_in_i64(int64_t i64Val)
{
    if ( i64Val < 0 )
    {
        //
        // Get absolute value
        //
        i64Val = -i64Val;
    }

    return ndigits_in_u64((uint64_t) i64Val);
}

//*****************************************************************************
//
// Return the number of hex digits in an uint64_t.
//
//*****************************************************************************
static int
ndigits_in_hex(uint64_t ui64Val)
{
    int iDigits = ui64Val ? 0 : 1;

    while ( ui64Val )
    {
        ui64Val >>= 4;
        ++iDigits;
    }

    return iDigits;
}

//*****************************************************************************
//
// Converts a string representing a decimal value to an int32_t.
//
// Returns the int32_t integer value.
//
// Note: If a count of the number of chars is desired, then provide
// pui32CharCnt.
//
//*****************************************************************************
static uint32_t
decstr_to_int(const char *pcStr, uint32_t *pui32CharCnt)
{
    bool bNeg = false;
    uint32_t ui32Val = 0, uCnt = 0;

    if ( *pcStr == '-')
    {
        bNeg = true;
        pcStr++;
        uCnt++;
    }

    while ( *pcStr >= '0'  &&  *pcStr <= '9' )
    {
        ++uCnt;

        //
        // Multiply accumulated value by 10.
        //
        ui32Val *= 10;

        //
        // Add in the new low digit.
        //
        ui32Val += (*pcStr - '0');
        pcStr++;
    }

    if ( pui32CharCnt )
    {
        *pui32CharCnt = uCnt;
    }

    return bNeg ? -ui32Val : ui32Val;
}

//*****************************************************************************
//
// Converts ui64Val to a string.
// Note: pcBuf[] must be sized for a minimum of 21 characters.
//
// Returns the number of decimal digits in the string.
//
// NOTE: If pcBuf is NULL, will compute a return ui64Val only (no chars
// written).
//
//*****************************************************************************
static int
uint64_to_str(uint64_t ui64Val, char *pcBuf)
{
    char tbuf[25];
    int ix = 0, iNumDig = 0;
    unsigned uMod;
    uint64_t u64Tmp;

    do
    {
        //
        // Divide by 10
        //
        u64Tmp = divu64_10(ui64Val);

        //
        // Get modulus
        //
        uMod = ui64Val - (u64Tmp * 10);

        tbuf[ix++] = uMod + '0';
        ui64Val = u64Tmp;
    } while ( ui64Val );

    //
    // Save the total number of digits
    //
    iNumDig = ix;

    //
    // Now, reverse the buffer when saving to the caller's buffer.
    //
    if ( pcBuf )
    {
        while ( ix-- )
        {
            *pcBuf++ = tbuf[ix];
        }

        //
        // Terminate the caller's buffer
        //
        *pcBuf = 0x00;
    }

    return iNumDig;
}

//*****************************************************************************
//
// Converts ui64Val to a hex string.  Alpha chars are lower case.
// Input:
//  ui64Val = Value to be converted.
//  pcBuf[] must be sized for a minimum of 17 characters.
//
// Returns the number of hex digits required for ui64Val (does not
//  include the terminating NULL char in the string).
//
// NOTE: If pcBuf is NULL, will compute a return value only (no chars
// written).
//
//*****************************************************************************
static int
uint64_to_hexstr(uint64_t ui64Val, char *pcBuf, bool bLower)
{
    int iNumDig, ix = 0;
    char cCh, tbuf[20];

    if ( ui64Val == 0 )
    {
        tbuf[ix++] = '0';   // Print a '0'
    }

    while ( ui64Val )
    {
        cCh = ui64Val & 0xf;

        //
        // Alpha character
        //
        if ( cCh > 9 )
        {
            cCh += bLower ? 0x27 : 0x7;
        }

        tbuf[ix++] = cCh + '0';
        ui64Val >>= 4;
    }

    //
    // Save the total number of digits
    //
    iNumDig = ix;

    //
    // Now, reverse the buffer when saving to the callers buffer.
    //
    if (pcBuf)
    {
        while (ix--)
        {
            *pcBuf++ = tbuf[ix];
        }

        //
        // Terminate the caller's buffer
        //
        *pcBuf = 0;
    }

    return iNumDig;
}

//*****************************************************************************
//
// Return length of the given string.
//
//*****************************************************************************
static uint32_t
simple_strlen(char *pcBuf)
{
    uint32_t ui32RetVal = 0;
    if ( !pcBuf )
    {
        return ui32RetVal;
    }

    while ( *pcBuf++ )
    {
        ui32RetVal++;
    }
    return ui32RetVal;
}

//*****************************************************************************
//
// Pad a string buffer with pad characters.
//
//*****************************************************************************
static int32_t
padbuffer(char *pcBuf, uint8_t cPadChar, int32_t i32NumChars)
{
    int32_t i32Cnt = 0;

    if ( i32NumChars <= 0 )
    {
        return i32Cnt;
    }

    while ( i32NumChars-- )
    {
        if ( pcBuf )
        {
            *pcBuf++ = cPadChar;
        }
        i32Cnt++;
    }

    return i32Cnt;
}

//*****************************************************************************
//
//! @brief Text mode translates linefeed (\n) characters to carriage return/
//! linefeed (CR/LF) combinations in printf() and sprintf() functions.
//!
//! @param bSetTextTranslationMode - true: Do LF to CR/LF translation.
//! false: Don't do the text mode translation.
//!
//! This function causes the printf() and sprintf() functions to translate
//! newline characters (\\n) into CR/LF (\\r\\n) combinations.
//!
//! @return Previous mode.
//
//*****************************************************************************
bool
am_util_stdio_textmode_set(bool bSetTextTranslationMode)
{
    bool bRet = g_bTxtXlate;

    //
    // true=cvt LF chars to CR/LF
    //
    g_bTxtXlate = bSetTextTranslationMode;

    //
    // return previous mode.
    //
    return bRet;
}

//*****************************************************************************
//
//  Float to ASCII text. A basic implementation for providing support for
//  single-precision %f.
//
//  param
//      fValue     = Float value to be converted.
//      pcBuf      = Buffer to place string AND input of buffer size.
//      iPrecision = Desired number of decimal places.
//      IMPORTANT: On entry, the first 32-bit word of pcBuf must
//                 contain the size (in bytes) of the buffer!
//                 The recommended size is at least 16 bytes.
//
//  This function performs a basic translation of a floating point single
//  precision value to a string.
//
//  return Number of chars printed to the buffer.
//
//*****************************************************************************
#define AM_FTOA_ERR_VAL_TOO_SMALL   -1
#define AM_FTOA_ERR_VAL_TOO_LARGE   -2
#define AM_FTOA_ERR_BUFSIZE         -3

typedef union
{
    int32_t I32;
    float F;
} i32fl_t;

static int ftoa(float fValue, char *pcBuf, int iPrecision)
{
    i32fl_t unFloatValue;
    int iExp2, iBufSize;
    int32_t i32Significand, i32IntPart, i32FracPart;
    char *pcBufInitial, *pcBuftmp;

    iBufSize = *(uint32_t*)pcBuf;
    if (iBufSize < 4)
    {
        return AM_FTOA_ERR_BUFSIZE;
    }

    if (fValue == 0.0f)
    {
        // "0.0"
        *(uint32_t*)pcBuf = 0x00 << 24 | ('0' << 16) | ('.' << 8) | ('0' << 0);
        return 3;
    }

    pcBufInitial = pcBuf;

    unFloatValue.F = fValue;

    iExp2 = ((unFloatValue.I32 >> 23) & 0x000000FF) - 127;
    i32Significand = (unFloatValue.I32 & 0x00FFFFFF) | 0x00800000;
    i32FracPart = 0;
    i32IntPart = 0;

    if (iExp2 >= 31)
    {
        return AM_FTOA_ERR_VAL_TOO_LARGE;
    }
    else if (iExp2 < -23)
    {
        return AM_FTOA_ERR_VAL_TOO_SMALL;
    }
    else if (iExp2 >= 23)
    {
        i32IntPart = i32Significand << (iExp2 - 23);
    }
    else if (iExp2 >= 0)
    {
        i32IntPart = i32Significand >> (23 - iExp2);
        i32FracPart = (i32Significand << (iExp2 + 1)) & 0x00FFFFFF;
    }
    else // if (iExp2 < 0)
    {
        i32FracPart = (i32Significand & 0x00FFFFFF) >> -(iExp2 + 1);
    }

    if (unFloatValue.I32 < 0)
    {
        *pcBuf++ = '-';
    }

    if (i32IntPart == 0)
    {
        *pcBuf++ = '0';
    }
    else
    {
        if (i32IntPart > 0)
        {
            uint64_to_str(i32IntPart, pcBuf);
        }
        else
        {
            *pcBuf++ = '-';
            uint64_to_str(-i32IntPart, pcBuf);
        }
        while (*pcBuf)    // Get to end of new string
        {
            pcBuf++;
        }
    }

    //
    // Now, begin the fractional part
    //
    *pcBuf++ = '.';

    if (i32FracPart == 0)
    {
        *pcBuf++ = '0';
    }
    else
    {
        int jx, iMax;

        iMax = iBufSize - (pcBuf - pcBufInitial) - 1;
        iMax = (iMax > iPrecision) ? iPrecision : iMax;

        for (jx = 0; jx < iMax; jx++)
        {
            i32FracPart *= 10;
            *pcBuf++ = (i32FracPart >> 24) + '0';
            i32FracPart &= 0x00FFFFFF;
        }

        //
        // Per the printf spec, the number of digits printed to the right of the
        // decimal point (i.e. iPrecision) should be rounded.
        // Some examples:
        // Value        iPrecision          Formatted value
        // 1.36399      Unspecified (6)     1.363990
        // 1.36399      3                   1.364
        // 1.36399      4                   1.3640
        // 1.36399      5                   1.36399
        // 1.363994     Unspecified (6)     1.363994
        // 1.363994     3                   1.364
        // 1.363994     4                   1.3640
        // 1.363994     5                   1.36399
        // 1.363995     Unspecified (6)     1.363995
        // 1.363995     3                   1.364
        // 1.363995     4                   1.3640
        // 1.363995     5                   1.36400
        // 1.996        Unspecified (6)     1.996000
        // 1.996        2                   2.00
        // 1.996        3                   1.996
        // 1.996        4                   1.9960
        //
        // To determine whether to round up, we'll look at what the next
        // decimal value would have been.
        //
        if ( ((i32FracPart * 10) >> 24) >= 5 )
        {
            //
            // Yes, we need to round up.
            // Go back through the string and make adjustments as necessary.
            //
            pcBuftmp = pcBuf - 1;
            while ( pcBuftmp >= pcBufInitial )
            {
                if ( *pcBuftmp == '.' )
                {
                }
                else if ( *pcBuftmp == '9' )
                {
                    *pcBuftmp = '0';
                }
                else
                {
                    *pcBuftmp += 1;
                    break;
                }
                pcBuftmp--;
            }
        }
    }

    //
    // Terminate the string and we're done
    //
    *pcBuf = 0x00;

    return (pcBuf - pcBufInitial);
} // ftoa()

//******************************************************************************
//
//! @brief Format data into string. (va_list implementation)
//!
//! @param *pcBuf - Pointer to the buffer to store the string
//! @param *pcFmt - Pointer to formatter string
//!
//! A lite version of vsprintf().
//!      Currently handles the following specifiers:
//!      %c
//!      %s
//!      %[0][width]d (also %i)
//!      %[0][width]u
//!      %[0][width]x
//!      %[.precision]f
//!
//!     Note than any unrecognized or unhandled format specifier character is
//!     simply printed.  For example, "%%" will print a '%' character.
//!
//! @return uint32_t representing the number of characters printed.
//
//******************************************************************************
uint32_t
am_util_stdio_vsprintf(char *pcBuf, const char *pcFmt, va_list pArgs)
{
    char *pcStr;
    uint64_t ui64Val;
    int64_t i64Val;
    uint32_t ui32NumChars, ui32CharCnt = 0;
    int iWidth, iVal, iPrecision;
    uint8_t ui8CharSpecifier, ui8PadChar;
    bool bLower, bLongLong, bNeg;
    uint32_t ui32strlen = 0;

    while ( *pcFmt != 0x0 )
    {
        iPrecision = 6;             // printf() default precision for %f is 6

        if ( *pcFmt != '%' )
        {
            //
            // Accumulate the string portion of the format specification.
            //
            if ( pcBuf )
            {
                // If '\n', convert to '\r\n'
                if ( *pcFmt == '\n'  &&  g_bTxtXlate )
                {
                    *pcBuf++ = '\r';
                    ++ui32CharCnt;
                }
                *pcBuf++ = *pcFmt;
            }

            ++pcFmt;
            ++ui32CharCnt;
            continue;
        }

        //
        // Handle the specifier.
        //
        ++pcFmt;
        bLower = bLongLong = false;

        //
        // Default to space as ui8PadChar
        //
        ui8PadChar = ' ';

        if ( *pcFmt == '0' )
        {
            ui8PadChar = '0';
            ++pcFmt;
        }

        //
        // Width specifier
        //
        iWidth = decstr_to_int(pcFmt, &ui32NumChars);
        pcFmt += ui32NumChars;

        //
        // For now, only support a negative width specifier for %s
        //
        if ( ( *pcFmt != 's' )  &&  ( iWidth < 0 ) )
        {
            iWidth = -iWidth;
        }

        //
        // Check for precision specifier
        //
        if (*pcFmt == '.')
        {
            ++pcFmt;
            iPrecision = decstr_to_int(pcFmt, &ui32NumChars);
            pcFmt += ui32NumChars;
        }

        //
        // Check for the long or long long length field sub-specifiers, 'l' or
        // 'll', which must be a modifier for either 'd', 'i', 'u', 'x', or 'X'
        // (or even 'o', which is not currently supported). Other sub-specifiers
        // like 'hh','h', etc. are not currently handled.
        // Note - 'l' is used in Coremark, a primary reason it's supported here.
        //
        if ( *pcFmt == 'l' )
        {
            pcFmt++;
            if ( *pcFmt == 'l' )    // "ll" (long long)
            {
                pcFmt++;
                bLongLong = true;
            }
        }

        switch ( *pcFmt )
        {
            case 'c':
                ui8CharSpecifier = va_arg(pArgs, uint32_t);

                if ( pcBuf )
                {
                    *pcBuf++ = ui8CharSpecifier;
                }

                ++ui32CharCnt;
                break;

            case 's':
                pcStr = va_arg(pArgs, char *);

                //
                // For %s, we support the width specifier. If iWidth is negative
                // the string is left-aligned (padding on the right).  Otherwise
                // the string is padded at the beginning with spaces.
                //
                ui32strlen = simple_strlen(pcStr);
                if ( iWidth > 0 )
                {
                    // Pad the beginning of the string (right-aligned).
                    if ( ui32strlen < iWidth )
                    {
                        // String needs some padding.
                        iWidth -= ui32strlen;
                        iWidth = padbuffer(pcBuf, ui8PadChar, iWidth);
                        pcBuf += pcBuf ? iWidth : 0;
                        ui32CharCnt += iWidth;
                        iWidth = 0;
                    }
                }

                while (*pcStr != 0x0)
                {
                    if ( pcBuf )
                    {
                        *pcBuf++ = *pcStr;
                    }

                    ++pcStr;
                    ++ui32CharCnt;
                }

                if ( iWidth )
                {
                    iWidth = -iWidth;

                    // Pad the end of the string (left-aligned).
                    if ( ui32strlen < iWidth )
                    {
                        // String needs some padding.
                        iWidth -= ui32strlen;
                        iWidth = padbuffer(pcBuf, ui8PadChar, iWidth);
                        pcBuf += pcBuf ? iWidth : 0;
                        ui32CharCnt += iWidth;
                        iWidth = 0;
                    }
                }
                break;

            case 'x':
                bLower = true;
            case 'X':
                ui64Val = bLongLong ? va_arg(pArgs, uint64_t) :
                                      va_arg(pArgs, uint32_t);

                if ( iWidth )
                {
                    //
                    // Compute # of leading chars
                    //
                    iWidth -= ndigits_in_hex(ui64Val);

                    iWidth = padbuffer(pcBuf, ui8PadChar, iWidth);
                    pcBuf += pcBuf ? iWidth : 0;
                    ui32CharCnt += iWidth;
                    iWidth = 0;
                }

                iVal = uint64_to_hexstr(ui64Val, pcBuf, bLower);

                if ( pcBuf )
                {
                    pcBuf += iVal;
                }

                ui32CharCnt += iVal;
                break;

            case 'u':
                ui64Val = bLongLong ? va_arg(pArgs, uint64_t) :
                                      va_arg(pArgs, uint32_t);

                if ( iWidth )
                {
                    //
                    // We need to pad the beginning of the value.
                    // Compute # of leading chars
                    //
                    iWidth -= ndigits_in_u64(ui64Val);

                    iWidth = padbuffer(pcBuf, ui8PadChar, iWidth);
                    pcBuf += pcBuf ? iWidth : 0;
                    ui32CharCnt += iWidth;
                    iWidth = 0;
                }

                iVal = uint64_to_str(ui64Val, pcBuf);

                if ( pcBuf )
                {
                    pcBuf += iVal;
                }

                ui32CharCnt += iVal;
                break;

            case 'd':
            case 'i':
                //
                // Output for a negative number, for example, -5:
                //   %d:-5
                //  %5d:   -5
                // %05d:-0005
                //
                i64Val = bLongLong ? va_arg(pArgs, int64_t) :
                                     va_arg(pArgs, int32_t);

                //
                // Get absolute value
                //
                if ( i64Val < 0 )
                {
                    ui64Val = -i64Val;          // Get absolute value
                    bNeg = true;
                }
                else
                {
                    ui64Val = i64Val;
                    bNeg = false;
                }

                if ( iWidth )
                {
                    //
                    // We need to pad the beginning of the value.
                    // Compute # of leading chars
                    //
                    iWidth -= ndigits_in_i64(ui64Val);

                    if ( bNeg )
                    {
                        --iWidth;

                        //
                        // Allow for the negative sign
                        //
                        if ( ui8PadChar == '0' )
                        {
                            //
                            // Print the neg sign BEFORE the leading zeros
                            //
                            if ( pcBuf )
                            {
                                *pcBuf++ = '-';
                            }

                            ++ui32CharCnt;
                        }
                    }

                    iWidth = padbuffer(pcBuf, ui8PadChar, iWidth);
                    pcBuf += pcBuf ? iWidth : 0;
                    ui32CharCnt += iWidth;
                    iWidth = 0;

                    if ( bNeg  &&  (ui8PadChar == ' ') )
                    {
                        //
                        // Print the neg sign AFTER the leading blanks
                        //
                        if ( pcBuf )
                        {
                            *pcBuf++ = '-';
                        }

                        ++ui32CharCnt;
                    }
                }
                else
                {
                    if ( bNeg )
                    {
                        if ( pcBuf )
                        {
                            *pcBuf++ = '-';
                        }
                        ++ui32CharCnt;
                    }
                }

                iVal = uint64_to_str(ui64Val, pcBuf);

                if ( pcBuf )
                {
                    pcBuf += iVal;
                }

                ui32CharCnt += iVal;
                break;


            case 'f':
            case 'F':
                if ( pcBuf )
                {
                    float fValue = va_arg(pArgs, double);

                    //
                    // pcBuf is an input (size of buffer) and also an output of ftoa()
                    //
                    *(uint32_t*)pcBuf = 20;

                    iVal = ftoa(fValue, pcBuf, iPrecision);
                    if ( iVal < 0 )
                    {
                        uint32_t u32PrntErrVal;
                        if ( iVal == AM_FTOA_ERR_VAL_TOO_SMALL )
                        {
                            u32PrntErrVal = (0x00 << 24) | ('0' << 16) |
                                            ('.' << 8)   | ('0' << 0);  // "0.0"
                        }
                        else if ( iVal == AM_FTOA_ERR_VAL_TOO_LARGE )
                        {
                            u32PrntErrVal = (0x00 << 24) | ('#' << 16) |
                                            ('.' << 8)   | ('#' << 0);  // "#.#"
                        }
                        else
                        {
                            u32PrntErrVal = (0x00 << 24) | ('?' << 16) |
                                            ('.' << 8)   | ('?' << 0);  // "?.?"
                        }
                        *(uint32_t*)pcBuf = u32PrntErrVal;
                        iVal = 3;
                    }
                    ui32CharCnt += iVal;
                    pcBuf += iVal;
                }
                break;

            //
            // Invalid specifier character
            // For non-handled specifiers, we'll just print the character.
            // e.g. this will allow the normal printing of a '%' using
            // "%%".
            //
            default:
                if ( pcBuf )
                {
                    *pcBuf++ = *pcFmt;
                }

                ++ui32CharCnt;
                break;

        } // switch()

        //
        // Bump the format specification to the next character
        //
        ++pcFmt;

    } // while ()

    //
    // Terminate the string
    //
    if ( pcBuf )
    {
        *pcBuf = 0x0;
    }

    return (ui32CharCnt);
}

//******************************************************************************
//
//! @brief Format data into string.
//!
//! @param *pcBuf - Pointer to the buffer to store the string
//! @param *pcFmt - Pointer to formater string
//!
//! A lite version of vsprintf().
//!      Currently handles the following specifiers:
//!      %c
//!      %s
//!      %[0][width]d (also %i)
//!      %[0][width]u
//!      %[0][width]x
//!
//!     Note than any unrecognized or unhandled format specifier character is
//!     simply printed.  For example, "%%" will print a '%' character.
//!
//! @return uint32_t representing the number of characters printed.
//
//******************************************************************************
uint32_t
am_util_stdio_sprintf(char *pcBuf, const char *pcFmt, ...)
{
    uint32_t ui32CharCnt;

    va_list pArgs;
    va_start(pArgs, pcFmt);
    ui32CharCnt = am_util_stdio_vsprintf(pcBuf, pcFmt, pArgs);
    va_end(pArgs);

    return ui32CharCnt;
}

//*****************************************************************************
//
//! @brief A lite version of printf()
//!
//! @param *pcFmt - Pointer to formatter string
//!
//!  See am_util_stdio_sprintf() for more details.
//!
//! @return uint32_t representing the number of characters printed.
//
// *****************************************************************************
uint32_t
am_util_stdio_printf(const char *pcFmt, ...)
{
    uint32_t ui32NumChars;

    if (!g_pfnCharPrint)
    {
        return 0;
    }

    //
    // Convert to the desired string.
    //
    va_list pArgs;
    va_start(pArgs, pcFmt);
    ui32NumChars = am_util_stdio_vsprintf(g_prfbuf, pcFmt, pArgs);
    va_end(pArgs);

    //
    // This is where we print the buffer to the configured interface.
    //
    g_pfnCharPrint(g_prfbuf);

    //
    // return the number of characters printed.
    //
    return ui32NumChars;
}

//*****************************************************************************
//
//! @brief Clear the terminal screen
//!
//! This function clears a standard terminal screen.
//!
//! @return None.
//
//*****************************************************************************
void
am_util_stdio_terminal_clear(void)
{
    //
    // Escape codes to clear a terminal screen and put the cursor in the top
    // left corner.
    // We'll first print a number of spaces, which helps get the ITM in sync
    // with AM Flash, especially after a reset event or a system clock
    // frequency change.
    //
    am_util_stdio_printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}
