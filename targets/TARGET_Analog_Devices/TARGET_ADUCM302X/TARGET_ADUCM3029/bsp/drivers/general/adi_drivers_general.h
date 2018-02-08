/*!
 *****************************************************************************
 * @file:    adi_drivers_general.h
 * @brief:   Macros and types used in multiple drivers
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
 *
 *****************************************************************************/
#ifndef ADI_DRIVERS_GENERAL__H
#define ADI_DRIVERS_GENERAL__H


/* Macros related to alignment in the different toolchains supported */

/* 
 * These macros are designed to be used as follows:
 * ADI_ALIGNED_PRAGMA(<required_alignment>)
 * <variable_declaration> ADI_ALIGNED_ATTRIBUTE(<required_alignment>)
 */

#if defined ( __ICCARM__ )
/*
* IAR MISRA C 2004 error suppressions.
*
*
* Pm120 (rule 19.10): In the definition of a function-like macro each parameter
*                     shall be enclosed in parenthesis. 
*                     This is not possible in attributes and pragmas
* Pm154 (rule 19.13): The # and ## preprocessor operators shall not be used.
*                     We need to do this to abstract the macros for the 
*                     different toolchains
*/
#pragma diag_suppress=Pm120,Pm154
#endif

#define PRAGMA(x) _Pragma(#x)
#define ATTRIBUTE(x) __attribute__((x))

#if defined (__GNUC__)
  /* Gcc uses attributes */
  #define ADI_ALIGNED_PRAGMA(num)
  #define ADI_ALIGNED_ATTRIBUTE(num) ATTRIBUTE(aligned(num))
  #define ADI_UNUSED_ATTRIBUTE ATTRIBUTE(unused)
#elif defined ( __ICCARM__ )
  /* IAR uses a pragma */
  #define ADI_ALIGNED_ATTRIBUTE(num)
  #define ADI_ALIGNED_PRAGMA(num) PRAGMA(data_alignment=num) 
  #define ADI_UNUSED_ATTRIBUTE
#elif defined (__CC_ARM)
  /* Keil uses a decorator which is placed in the same position as pragmas */
  #define ADI_ALIGNED_ATTRIBUTE(num)
  #define ADI_ALIGNED_PRAGMA(num) __align(##num)
  #define ADI_UNUSED_ATTRIBUTE ATTRIBUTE(unused)
#else
#error "Toolchain not supported"
#endif


#if defined ( __ICCARM__ )
#pragma diag_default=Pm120,Pm154
#endif
#endif /* ADI_DRIVERS_GENERAL__H */
