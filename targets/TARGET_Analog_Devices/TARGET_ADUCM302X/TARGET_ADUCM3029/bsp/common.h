/*!
 *****************************************************************************
 * @file:    common.h
 * @brief:   Common include file for all example 
 *-----------------------------------------------------------------------------
 *
Copyright (c) 2010-2016 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/


#ifndef COMMON_H
#define COMMON_H

#ifdef __ICCARM__
/*
* Pm106 (rule 20.9): the input/output library <stdio.h> shall not be used in
                     production code
*   The purpose of this header is to provide I/O facilities based on stdio.
*/
#pragma diag_suppress=Pm106
#endif /* __ICCARM__ */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>


#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm011 (rule 6.3): The basic types of char, int, short, long, float shall not be used.
* Pm064 (rule 16.1): functions with variable number of arguments shall not be used.
*/
#pragma diag_suppress=Pm011,Pm064
#endif /* __ICCARM__ */


#ifdef __cplusplus
extern "C" {
#endif

/* Enable REDIRECT_OUTPUT_TO_UART to send the output to UART terminal. */
/* #define REDIRECT_OUTPUT_TO_UART */

extern char aDebugString[150]; 

#ifdef __ICCARM__
/*
* Pm154 (rule 19.10): in the definition of a function-like macro, each instance
*                     of a parameter shall be enclosed in parentheses
*   The __VA_ARGS__ macro cannot be enclosed in parentheses.
*/
#pragma diag_suppress=Pm154
#endif /* __ICCARM__ */

#define DEBUG_MESSAGE(...) \
  do { \
    sprintf(aDebugString,__VA_ARGS__); \
    common_Perf(aDebugString); \
  } while(0)

#ifdef __ICCARM__
#pragma diag_default=Pm154
#endif /* __ICCARM__ */

#define DEBUG_RESULT(s,result,expected_value) \
  do { \
    if ((result) != (expected_value)) { \
      sprintf(aDebugString,"%s  %d", __FILE__,__LINE__); \
      common_Fail(aDebugString); \
      sprintf(aDebugString,"%s Error Code: 0x%08X\n\rFailed\n\r",(s),(result)); \
      common_Perf(aDebugString); \
      exit(0); \
    } \
  } while (0)

/********************************************************************************
* API function prototypes
*********************************************************************************/
void common_Init(void);
void common_Pass(void);
void common_Fail(char *FailureReason);
void common_Perf(char *InfoString);

#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */
