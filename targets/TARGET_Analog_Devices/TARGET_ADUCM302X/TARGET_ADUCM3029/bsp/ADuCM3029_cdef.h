/*!
*****************************************************************************
 * @file:    ADuCM3029_cdef.h
 * @brief:   ADuCM3029 C MMR Pointer Definitions
 * @version: $Revision: 36179 $
 * @date:    $Date: 2017-02-10 09:56:54 -0500 (Fri, 10 Feb 2017) $
 *-----------------------------------------------------------------------------
 *
Copyright (c) 2015-2017 Analog Devices, Inc.

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

#ifndef _WRAP_ADUCM3029_CDEF_H
#define _WRAP_ADUCM3029_CDEF_H

#include <ADuCM3029_def.h>

#include <stdint.h>

#ifdef __ICCARM__
/* IAR MISRA C 2004 error suppressions:
 *
 * Pm081 (rule 19.6): #undef should not be used
 *   Needed to work around incorrect definitions in sys/ADuCM302x_cdef.h.
 */
#pragma diag_suppress=Pm081
#endif /* __ICCARM__ */

#include <sys/adi_cio_macros.h>
#include <sys/ADuCM302x_cdef.h>


/* Backward compatibility shims for renamed UART registers. */

#define pREG_UART0_COMRX   (pREG_UART0_RX)
#define pREG_UART0_COMTX   (pREG_UART0_TX)
#define pREG_UART0_COMIEN  (pREG_UART0_IEN)
#define pREG_UART0_COMIIR  (pREG_UART0_IIR)
#define pREG_UART0_COMLCR  (pREG_UART0_LCR)
#define pREG_UART0_COMMCR  (pREG_UART0_MCR)
#define pREG_UART0_COMLSR  (pREG_UART0_LSR)
#define pREG_UART0_COMMSR  (pREG_UART0_MSR)
#define pREG_UART0_COMSCR  (pREG_UART0_SCR)
#define pREG_UART0_COMFCR  (pREG_UART0_FCR)
#define pREG_UART0_COMFBR  (pREG_UART0_FBR)
#define pREG_UART0_COMDIV  (pREG_UART0_DIV)
#define pREG_UART0_COMLCR2 (pREG_UART0_LCR2)
#define pREG_UART0_COMCTL  (pREG_UART0_CTL)
#define pREG_UART0_COMRFC  (pREG_UART0_RFC)
#define pREG_UART0_COMTFC  (pREG_UART0_TFC)
#define pREG_UART0_COMRSC  (pREG_UART0_RSC)
#define pREG_UART0_COMACR  (pREG_UART0_ACR)
#define pREG_UART0_COMASRL (pREG_UART0_ASRL)
#define pREG_UART0_COMASRH (pREG_UART0_ASRH)


/* Backward compatibility shim for renamed RTC registers and fields. */

#define pREG_RTC0_CR3OC  (pREG_RTC0_CR3SS)
#define pREG_RTC0_CR4OC  (pREG_RTC0_CR4SS)
#define pREG_RTC0_OCMSK  (pREG_RTC0_SSMSK)
#define pREG_RTC0_OC1ARL (pREG_RTC0_SS1ARL)
#define pREG_RTC0_OC1    (pREG_RTC0_SS1)
#define pREG_RTC0_OC1TGT (pREG_RTC0_SS1TGT)
#define pREG_RTC1_CR3OC  (pREG_RTC1_CR3SS)
#define pREG_RTC1_CR4OC  (pREG_RTC1_CR4SS)
#define pREG_RTC1_OCMSK  (pREG_RTC1_SSMSK)
#define pREG_RTC1_OC1ARL (pREG_RTC1_SS1ARL)
#define pREG_RTC1_OC1    (pREG_RTC1_SS1)
#define pREG_RTC1_OC1TGT (pREG_RTC1_SS1TGT)


#ifdef __ICCARM__
#pragma diag_default=Pm081
#endif /* __ICCARM__ */

#endif /* _WRAP_ADUCM3029_CDEF_H */
