//*****************************************************************************
//
//! @file am_util_string.c
//!
//! @brief A subset of the functions provided in the C standard string library.
//!
//! The functions here are reimplementation of some of the standard "string"
//! functions.
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
#include "am_util_string.h"


//*****************************************************************************
//
//! @brief Table for quick lookup of character attributes.
//
//*****************************************************************************
#if MINIMIZE_CATTR_TABLE
#define CATTR_TBL_SIZE  128
#else
#define CATTR_TBL_SIZE  256
#endif

const uint8_t am_cattr[CATTR_TBL_SIZE] =
{
    AM_CATTR_NONE,                                                          /* 0x00 */
    AM_CATTR_NONE,                                                          /* 0x01 */
    AM_CATTR_NONE,                                                          /* 0x02 */
    AM_CATTR_NONE,                                                          /* 0x03 */
    AM_CATTR_NONE,                                                          /* 0x04 */
    AM_CATTR_NONE,                                                          /* 0x05 */
    AM_CATTR_NONE,                                                          /* 0x06 */
    AM_CATTR_NONE,                                                          /* 0x07 */
    AM_CATTR_NONE,                                                          /* 0x08 */
    AM_CATTR_WHSPACE,                                                       /* 0x09 */
    AM_CATTR_WHSPACE,                                                       /* 0x0A */
    AM_CATTR_WHSPACE,                                                       /* 0x0B */
    AM_CATTR_WHSPACE,                                                       /* 0x0C */
    AM_CATTR_WHSPACE,                                                       /* 0x0D */
    AM_CATTR_NONE,                                                          /* 0x0E */
    AM_CATTR_NONE,                                                          /* 0x0F */
    AM_CATTR_NONE,                                                          /* 0x00 */
    AM_CATTR_NONE,                                                          /* 0x11 */
    AM_CATTR_NONE,                                                          /* 0x12 */
    AM_CATTR_NONE,                                                          /* 0x13 */
    AM_CATTR_NONE,                                                          /* 0x14 */
    AM_CATTR_NONE,                                                          /* 0x15 */
    AM_CATTR_NONE,                                                          /* 0x16 */
    AM_CATTR_NONE,                                                          /* 0x17 */
    AM_CATTR_NONE,                                                          /* 0x18 */
    AM_CATTR_NONE,                                                          /* 0x19 */
    AM_CATTR_NONE,                                                          /* 0x1A */
    AM_CATTR_NONE,                                                          /* 0x1B */
    AM_CATTR_NONE,                                                          /* 0x1C */
    AM_CATTR_NONE,                                                          /* 0x1D */
    AM_CATTR_NONE,                                                          /* 0x1E */
    AM_CATTR_NONE,                                                          /* 0x1F */
    AM_CATTR_WHSPACE,                                                       /* 0x20, space */
    AM_CATTR_FILENM83,                                                      /* 0x21, ! */
    AM_CATTR_NONE,                                                          /* 0x22, " */
    AM_CATTR_FILENM83,                                                      /* 0x23, # */
    AM_CATTR_FILENM83,                                                      /* 0x24, $ */
    AM_CATTR_FILENM83,                                                      /* 0x25, % */
    AM_CATTR_FILENM83,                                                      /* 0x26, & */
    AM_CATTR_FILENM83,                                                      /* 0x27, ' */
    AM_CATTR_FILENM83,                                                      /* 0x28, ( */
    AM_CATTR_FILENM83,                                                      /* 0x29, ) */
    AM_CATTR_NONE,                                                          /* 0x2A, * */
    AM_CATTR_NONE,                                                          /* 0x2B, + */
    AM_CATTR_NONE,                                                          /* 0x2C, , */
    AM_CATTR_FILENM83,                                                      /* 0x2D, - */
    AM_CATTR_FILENM83,                                                      /* 0x2E, . */
    AM_CATTR_NONE,                                                          /* 0x2F, / */
    AM_CATTR_DIGIT | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,                   /* 0x30, 0 */
    AM_CATTR_DIGIT | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,                   /* 0x31, 1 */
    AM_CATTR_DIGIT | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,                   /* 0x32, 2 */
    AM_CATTR_DIGIT | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,                   /* 0x33, 3 */
    AM_CATTR_DIGIT | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,                   /* 0x34, 4 */
    AM_CATTR_DIGIT | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,                   /* 0x35, 5 */
    AM_CATTR_DIGIT | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,                   /* 0x36, 6 */
    AM_CATTR_DIGIT | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,                   /* 0x37, 7 */
    AM_CATTR_DIGIT | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,                   /* 0x38, 8 */
    AM_CATTR_DIGIT | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,                   /* 0x39, 9 */
    AM_CATTR_NONE,                                                          /* 0x3A, : */
    AM_CATTR_NONE,                                                          /* 0x3B, ; */
    AM_CATTR_NONE,                                                          /* 0x3C, < */
    AM_CATTR_NONE,                                                          /* 0x3D, = */
    AM_CATTR_NONE,                                                          /* 0x3E, > */
    AM_CATTR_NONE,                                                          /* 0x3F, ? */
    AM_CATTR_FILENM83,                                                      /* 0x40, @ */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,  /* 0x41, A */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,  /* 0x42, B */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,  /* 0x43, C */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,  /* 0x44, D */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,  /* 0x45, E */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,  /* 0x46, F */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_FILENM83,                    /* 0x47, G */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_FILENM83,                    /* 0x48, H */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_FILENM83,                    /* 0x49, I */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_FILENM83,                    /* 0x4A, J */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_FILENM83,                    /* 0x4B, K */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_FILENM83,                    /* 0x4C, L */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_FILENM83,                    /* 0x4D, M */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_FILENM83,                    /* 0x4E, N */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_FILENM83,                    /* 0x4F, O */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_FILENM83,                    /* 0x50, P */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_FILENM83,                    /* 0x51, Q */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_FILENM83,                    /* 0x52, R */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_FILENM83,                    /* 0x53, S */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_FILENM83,                    /* 0x54, T */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_FILENM83,                    /* 0x55, U */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_FILENM83,                    /* 0x56, V */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_FILENM83,                    /* 0x57, W */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_FILENM83,                    /* 0x58, X */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_FILENM83,                    /* 0x59, Y */
    AM_CATTR_ALPHA | AM_CATTR_UPPER | AM_CATTR_FILENM83,                    /* 0x5A, Z */
    AM_CATTR_NONE,                                                          /* 0x5B, [ */
    AM_CATTR_NONE,                                                          /* 0x5C, \ */
    AM_CATTR_NONE,                                                          /* 0x5D, ] */
    AM_CATTR_FILENM83,                                                      /* 0x5E, ^ */
    AM_CATTR_FILENM83,                                                      /* 0x5F, _ */
    AM_CATTR_FILENM83,                                                      /* 0x60, ` */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,  /* 0x61, a */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,  /* 0x62, b */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,  /* 0x63, c */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,  /* 0x64, d */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,  /* 0x65, e */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_XDIGIT | AM_CATTR_FILENM83,  /* 0x66, f */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_FILENM83,                    /* 0x67, g */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_FILENM83,                    /* 0x68, h */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_FILENM83,                    /* 0x69, i */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_FILENM83,                    /* 0x6A, j */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_FILENM83,                    /* 0x6B, k */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_FILENM83,                    /* 0x6C, l */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_FILENM83,                    /* 0x6D, m */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_FILENM83,                    /* 0x6E, n */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_FILENM83,                    /* 0x6F, o */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_FILENM83,                    /* 0x70, p */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_FILENM83,                    /* 0x71, q */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_FILENM83,                    /* 0x72, r */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_FILENM83,                    /* 0x73, s */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_FILENM83,                    /* 0x74, t */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_FILENM83,                    /* 0x75, u */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_FILENM83,                    /* 0x76, v */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_FILENM83,                    /* 0x77, w */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_FILENM83,                    /* 0x78, x */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_FILENM83,                    /* 0x79, y */
    AM_CATTR_ALPHA | AM_CATTR_LOWER | AM_CATTR_FILENM83,                    /* 0x7A, z */
    AM_CATTR_FILENM83,                                                      /* 0x7B, { */
    AM_CATTR_NONE,                                                          /* 0x7C, | */
    AM_CATTR_FILENM83,                                                      /* 0x7D, } */
    AM_CATTR_FILENM83,                                                      /* 0x7E, ~ */
    AM_CATTR_NONE                                                           /* 0x7F, delete */

    //
    // All bit7 chars are AM_CATTR_NONE.
    //
};

//*****************************************************************************
//
//! @brief Character "is" functions
//!
//! This family of functions tests a given integer value in order to determine
//!  whether the integer satisfies the test condition.
//! These functions are generally based on the C99 standard functions.
//!
//! By default all of the "is" functions are implemented as macros. To implement
//!  as functions rather than macros, use a global compiler command line (-D)
//!  option to define AM_UTIL_STRING_CTYPE_DISABLE_MACROS.
//!
//! Standard functions currently implemented include:
//!  isalnum(), isalpha(), islower(), isupper(), isdigit(), isxdigit(),
//!  isspace().
//!
//! Standard functions not currently implemented include:
//!  iscntrl(), isgraph(), isprint(), ispunct(), isblank() (new for C99).
//!
//! Non-standard functions currently implemented include:
//!  isfilenm83().
//!
//! @return Each function returns a nonzero value if the integer satisfies
//!  the test condition and 0 if it does not.
//
//*****************************************************************************

#ifdef AM_UTIL_STRING_CTYPE_DISABLE_MACROS
int
am_util_string_isalnum(int c)
{
#if MINIMIZE_CATTR_TABLE
    return (c & 0xffffff80) ? 0 : (am_cattr[c] & (AM_CATTR_ALPHA | AM_CATTR_DIGIT)) ? 1 : 0;
#else
    return (am_cattr[c & 0xff] & (AM_CATTR_ALPHA | AM_CATTR_DIGIT)) ? 1 : 0;
#endif
}

int
am_util_string_isalpha(int c)
{
#if MINIMIZE_CATTR_TABLE
    return (c & 0xffffff80) ? 0 : (am_cattr[c] & AM_CATTR_ALPHA) ? 1 : 0;
#else
    return (am_cattr[c & 0xff] & AM_CATTR_ALPHA) ? 1 : 0;
#endif
}

int
am_util_string_isdigit(int c)
{
#if MINIMIZE_CATTR_TABLE
    return (c & 0xffffff80) ? 0 : (am_cattr[c] & AM_CATTR_DIGIT) ? 1 : 0;
#else
    return (am_cattr[c & 0xff] & AM_CATTR_DIGIT) ? 1 : 0;
#endif
}

int am_util_string_islower(int c)
{
#if MINIMIZE_CATTR_TABLE
    return (c & 0xffffff80) ? 0 : (am_cattr[c] & AM_CATTR_LOWER) ? 1 : 0;
#else
    return (am_cattr[c & 0xff] & AM_CATTR_LOWER) ? 1 : 0;
#endif
}

int
am_util_string_isspace(int c)
{
#if MINIMIZE_CATTR_TABLE
    return (c & 0xffffff80) ? 0 : (am_cattr[c] & AM_CATTR_WHSPACE) ? 1 : 0;
#else
    return (am_cattr[c & 0xff] & AM_CATTR_WHSPACE) ? 1 : 0;
#endif
}

int
am_util_string_isupper(int c)
{
#if MINIMIZE_CATTR_TABLE
    return (c & 0xffffff80) ? 0 : (am_cattr[c] & AM_CATTR_UPPER) ? 1 : 0;
#else
    return (am_cattr[c & 0xff] & AM_CATTR_UPPER) ? 1 : 0;
#endif
}

int
am_util_string_isxdigit(int c)
{
#if MINIMIZE_CATTR_TABLE
    return (c & 0xffffff80) ? 0 : (am_cattr[c] & AM_CATTR_XDIGIT) ? 1 : 0;
#else
    return (am_cattr[c & 0xff] & AM_CATTR_XDIGIT) ? 1 : 0;
#endif
}

int am_util_string_tolower(int c)
{
#if MINIMIZE_CATTR_TABLE
    return (am_cattr[c & 0x7f] & AM_CATTR_UPPER) ? c | 0x20 : c;
#else
    return (am_cattr[c & 0xff] & AM_CATTR_UPPER) ? c | 0x20 : c;
#endif
}

int am_util_string_toupper(int c)
{
#if MINIMIZE_CATTR_TABLE
    return (am_cattr[c & 0x7f] & AM_CATTR_LOWER) ? c & ~0x20 : c;
#else
    return (am_cattr[c & 0xff] & AM_CATTR_LOWER) ? c & ~0x20 : c;
#endif
}


//
// Non-standard "is" Functions
//
int
am_util_string_isfilenm83(int c)
{
#if MINIMIZE_CATTR_TABLE
    return (c & 0xffffff80) ? 0 : (am_cattr[c] & AM_CATTR_FILENM83) ? 1 : 0;
#else
    return (am_cattr[c & 0xff] & AM_CATTR_FILENM83) ? 1 : 0;
#endif
}
#endif // AM_UTIL_STRING_CTYPE_DISABLE_MACROS


//*****************************************************************************
//
//! @brief Compare two strings.
//!
//! @param str1 is the first string to compare.
//! @param str2 is the second string to compare
//!
//! This function steps through a pair of strings, character by character, to
//! determine if the strings contain the same characters. If the strings match,
//! this function will return a zero. If str1 is alphabetically earlier than
//! str2, the return value will be negative. Otherwise, the return value will
//! be positive.
//!
//! @return 0 for a perfect match, negative value if str1<str2, positive value
//!  if str1>str2.
//
//*****************************************************************************
int32_t
am_util_string_strcmp(const char *str1, const char *str2)
{
    return am_util_string_strncmp(str1, str2, 0xffffffff);
}

//*****************************************************************************
//
//! @brief Compare two strings with a specified count.
//!
//! @param str1 is the first string to compare.
//! @param str2 is the second string to compare
//! @param num is the maximum number of characters to compare.
//!
//! This function steps through a pair of strings, character by character, to
//! determine if the strings contain the same characters. If the strings match,
//! this function will return a zero. If str1 is alphabetically earlier than
//! str2, the return value will be negative. Otherwise, the return value will
//! be positive.
//!
//! @return 0 for a perfect match, negative value if str1<str2, positive value
//!  if str1>str2.
//
//*****************************************************************************
int32_t
am_util_string_strncmp(const char *str1, const char *str2, uint32_t num)
{
    while ( num-- )
    {
        // Check for inequality OR end of string
        if ( *str1 != *str2 || *str1 == '\0' )
        {
            return *str1 - *str2;
        }

        str1++;
        str2++;
    }

    //
    // Since we made it here, the strings must be equal to n characters.
    //
    return 0;
}

//*****************************************************************************
//
//! @brief Compare two strings with a specified count and without regard to
//! letter case in the strings.
//!
//! @param str1 is the first string to compare.
//! @param str2 is the second string to compare
//! @param num is the maximum number of characters to compare.
//!
//! This function steps through a pair of strings, character by character, to
//! determine if the strings contain the same characters. If the strings match,
//! this function will return a zero. If str1 is alphabetically earlier than
//! str2, the return value will be negative. Otherwise, the return value will
//! be positive.
//!
//! @return 0 for a perfect match, negative value if str1<str2, positive value
//!  if str1>str2.
//
//*****************************************************************************
int32_t
am_util_string_strnicmp(const char *str1, const char *str2, int num)
{
    uint8_t cChar1, cChar2;

    while ( *str1 && *str2 && num )
    {
        cChar1 = *str1;
        cChar2 = *str2;

        cChar1 |= ( am_cattr[cChar1] & AM_CATTR_UPPER ) ? 0x20 : 0x00;
        cChar2 |= ( am_cattr[cChar2] & AM_CATTR_UPPER ) ? 0x20 : 0x00;

        if ( cChar1 != cChar2 )
        {
            return cChar1 - cChar2;
        }

        str1++;
        str2++;
        num--;
    }

    //
    // Since we made it here, the strings must be equal to n characters.
    //
    return 0;
}

//*****************************************************************************
//
//! @brief Compare two strings with case-insensitivity.
//!
//! @param str1 is the first string to compare.
//! @param str2 is the second string to compare
//!
//! This function compares each character in the 2 strings, converting all
//! alpha characters to lower-case to make the comparison.
//!
//! To illustrate a possible unexpected outcome due to comparing the strings
//! as lower case, consider the example strings "AMBIQ_MICRO" and "AMBIQMICRO".
//! For these strings, stricmp() will return a negative value (indicating the
//! first as before the second), whereas strcmp() will return a positive value.
//!
//! @return 0 for a case-insensitive match, negative value if str1<str2,
//!  positive value if str1>str2.
//
//*****************************************************************************
int32_t
am_util_string_stricmp(const char *str1, const char *str2)
{
    uint8_t cChar1, cChar2;

    while ( *str1 && *str2 )
    {
        cChar1 = *str1++;
        cChar2 = *str2++;

        cChar1 |= ( am_cattr[cChar1] & AM_CATTR_UPPER ) ? 0x20 : 0x00;
        cChar2 |= ( am_cattr[cChar2] & AM_CATTR_UPPER ) ? 0x20 : 0x00;

        if ( cChar1 != cChar2 )
        {
            return cChar1 - cChar2;
        }
    }

    return *str1 - *str2;
}

//*****************************************************************************
//
//! @brief Return the length of a string.
//!
//! @param pcStr pointer to the string.
//!
//! This function returns the length of the string at pcStr.
//!
//! @return length of the string pcStr.
//
//*****************************************************************************
uint32_t
am_util_string_strlen(const char *pcStr)
{
    const char *pcS;

    //
    // Loop through the string.
    //
    for (pcS = pcStr; *pcS; ++pcS);

    //
    // Return the length.
    //
    return(pcS - pcStr);
}

//*****************************************************************************
//
//! @brief Copies a string.
//!
//! @param pcDst pointer to the destination string.
//! @param pcSrc pointer to the source string to be copied to pcDst.
//!
//! This function copies pcSrc to the location specified by pcDst.
//!
//! @return pcDst (the location of the destination string).
//
//*****************************************************************************
char *
am_util_string_strcpy(char *pcDst, const char *pcSrc)
{
    char *pcRet = pcDst;

    //
    // Blindly copy the string until we hit a terminating NULL char.
    //
    do
    {
        *pcDst++ = *pcSrc;
    } while ( *pcSrc++ );

    return pcRet;
}

//*****************************************************************************
//
//! @brief Copies a specified number of characters of a string.
//!
//! @param pcDst pointer to the destination string.
//! @param pcSrc pointer to the source string to be copied to pcDst.
//!
//! This function copies uNum characters of pcSrc to the location specified
//!  by pcDst.
//! If uNum is less than the length of pcSrc, a NULL terminating character
//!  is not appended to the copied string. Thus the resultant string will be
//!  exactly uNum chars in length and not terminated.
//! If uNum is greater than the length of pcSrc, then pcDst is padded with
//!  NULL characters up to the uNum length.
//! Behavior is undefined if the addresses ranges overlap.
//!
//! @return pcDst (the location of the destination string).
//
//*****************************************************************************
char *
am_util_string_strncpy(char *pcDst, const char *pcSrc, uint32_t uNum)
{
    char *pcRet = pcDst;

    while (uNum > 0)
    {
        if ( *pcSrc )
        {
            *pcDst++ = *pcSrc++;
        }
        else
        {
            *pcDst++ = 0x00;
        }
        uNum--;
    }

    return pcRet;
}

//*****************************************************************************
//
//! @brief Concatenate a string.
//!
//! @param pcDst pointer to the destination string.
//! @param pcSrc pointer to the source string to be copied to pcDst.
//!
//! This function concatenates the string at pcSrc to the existing string
//! at pcDst.
//!
//! Both strings, pcDst and pcSrc, must be NULL-terminated.
//! No overflow checking is performed.
//! pcDst and pcSrc shall not overlap.
//!
//! @return pcDst (the location of the destination string).
//
//*****************************************************************************
char *
am_util_string_strcat(char *pcDst, const char *pcSrc)
{
    char *pcRet = pcDst;

    //
    // Find the end of the existing string.
    //
    while ( *pcDst++ );
    pcDst--;

    //
    // Now, copy the new string.
    //
    am_util_string_strcpy(pcDst, pcSrc);

    return pcRet;
}
