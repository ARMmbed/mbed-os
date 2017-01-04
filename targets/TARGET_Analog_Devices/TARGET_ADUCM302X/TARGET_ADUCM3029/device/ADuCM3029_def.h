/*!
*****************************************************************************
 * @file:    ADuCM3029_def.h
 * @brief:   ADuCM3029 MMR addresses and fields
 * @version: $Revision: 35096 $
 * @date:    $Date: 2016-07-14 12:41:19 +0100 (Thu, 14 Jul 2016) $
 *-----------------------------------------------------------------------------
 *
Copyright (c) 2015-2016 Analog Devices, Inc.

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

#ifndef _WRAP_ADUCM3029_DEF_H
#define _WRAP_ADUCM3029_DEF_H

#ifdef __ICCARM__
/* IAR MISRA C 2004 error suppressions:
 *
 * Pm008 (rule 2.4): sections of code should not be 'commented out'.
 *   Some comments are wrongly identified as code.
 *
 * Pm009 (rule 5.1): identifiers shall not rely on significance of more than 31 characters.
 *   The sys/ADuCM302x.h header relies on more. The IAR compiler supports that.
 *
 * Pm081 (rule 19.6): #undef should not be used
 *   Needed to work around incorrect definitions in sys/ADuCM302x.h.
 */
#pragma diag_suppress=Pm008,Pm009,Pm081
#endif /* __ICCARM__ */

#ifdef __IASMARM__
/* Define masks to plain numeric literal for IAR assembler. */
#define _ADI_MSK_3( mask, smask, type ) (mask)
#endif /* __IASMARM__ */

#include <sys/ADuCM302x.h>

/* Backward compatibility shims for renamed UART registers. */

#define REG_UART0_COMRX   (REG_UART0_RX)
#define REG_UART0_COMTX   (REG_UART0_TX)
#define REG_UART0_COMIEN  (REG_UART0_IEN)
#define REG_UART0_COMIIR  (REG_UART0_IIR)
#define REG_UART0_COMLCR  (REG_UART0_LCR)
#define REG_UART0_COMMCR  (REG_UART0_MCR)
#define REG_UART0_COMLSR  (REG_UART0_LSR)
#define REG_UART0_COMMSR  (REG_UART0_MSR)
#define REG_UART0_COMSCR  (REG_UART0_SCR)
#define REG_UART0_COMFCR  (REG_UART0_FCR)
#define REG_UART0_COMFBR  (REG_UART0_FBR)
#define REG_UART0_COMDIV  (REG_UART0_DIV)
#define REG_UART0_COMLCR2 (REG_UART0_LCR2)
#define REG_UART0_COMCTL  (REG_UART0_CTL)
#define REG_UART0_COMRFC  (REG_UART0_RFC)
#define REG_UART0_COMTFC  (REG_UART0_TFC)
#define REG_UART0_COMRSC  (REG_UART0_RSC)
#define REG_UART0_COMACR  (REG_UART0_ACR)
#define REG_UART0_COMASRL (REG_UART0_ASRL)
#define REG_UART0_COMASRH (REG_UART0_ASRH)

#define BITP_UART_COMRX_RBR         (BITP_UART_RX_RBR)
#define BITP_UART_COMTX_THR         (BITP_UART_TX_THR)
#define BITP_UART_COMIEN_EDMAR      (BITP_UART_IEN_EDMAR)
#define BITP_UART_COMIEN_EDMAT      (BITP_UART_IEN_EDMAT)
#define BITP_UART_COMIEN_EDSSI      (BITP_UART_IEN_EDSSI)
#define BITP_UART_COMIEN_ELSI       (BITP_UART_IEN_ELSI)
#define BITP_UART_COMIEN_ETBEI      (BITP_UART_IEN_ETBEI)
#define BITP_UART_COMIEN_ERBFI      (BITP_UART_IEN_ERBFI)
#define BITP_UART_COMIIR_FEND       (BITP_UART_IIR_FEND)
#define BITP_UART_COMIIR_STA        (BITP_UART_IIR_STAT)
#define BITP_UART_COMIIR_NIRQ       (BITP_UART_IIR_NIRQ)
#define BITP_UART_COMLCR_BRK        (BITP_UART_LCR_BRK)
#define BITP_UART_COMLCR_SP         (BITP_UART_LCR_SP)
#define BITP_UART_COMLCR_EPS        (BITP_UART_LCR_EPS)
#define BITP_UART_COMLCR_PEN        (BITP_UART_LCR_PEN)
#define BITP_UART_COMLCR_STOP       (BITP_UART_LCR_STOP)
#define BITP_UART_COMLCR_WLS        (BITP_UART_LCR_WLS)
#define BITP_UART_COMMCR_LOOPBACK   (BITP_UART_MCR_LOOPBACK)
#define BITP_UART_COMMCR_OUT2       (BITP_UART_MCR_OUT2)
#define BITP_UART_COMMCR_OUT1       (BITP_UART_MCR_OUT1)
#define BITP_UART_COMMCR_RTS        (BITP_UART_MCR_RTS)
#define BITP_UART_COMMCR_DTR        (BITP_UART_MCR_DTR)
#define BITP_UART_COMLSR_FIFOERR    (BITP_UART_LSR_FIFOERR)
#define BITP_UART_COMLSR_TEMT       (BITP_UART_LSR_TEMT)
#define BITP_UART_COMLSR_THRE       (BITP_UART_LSR_THRE)
#define BITP_UART_COMLSR_BI         (BITP_UART_LSR_BI)
#define BITP_UART_COMLSR_FE         (BITP_UART_LSR_FE)
#define BITP_UART_COMLSR_PE         (BITP_UART_LSR_PE)
#define BITP_UART_COMLSR_OE         (BITP_UART_LSR_OE)
#define BITP_UART_COMLSR_DR         (BITP_UART_LSR_DR)
#define BITP_UART_COMMSR_DCD        (BITP_UART_MSR_DCD)
#define BITP_UART_COMMSR_RI         (BITP_UART_MSR_RI)
#define BITP_UART_COMMSR_DSR        (BITP_UART_MSR_DSR)
#define BITP_UART_COMMSR_CTS        (BITP_UART_MSR_CTS)
#define BITP_UART_COMMSR_DDCD       (BITP_UART_MSR_DDCD)
#define BITP_UART_COMMSR_TERI       (BITP_UART_MSR_TERI)
#define BITP_UART_COMMSR_DDSR       (BITP_UART_MSR_DDSR)
#define BITP_UART_COMMSR_DCTS       (BITP_UART_MSR_DCTS)
#define BITP_UART_COMSCR_SCR        (BITP_UART_SCR_SCR)
#define BITP_UART_COMFCR_RFTRIG     (BITP_UART_FCR_RFTRIG)
#define BITP_UART_COMFCR_FDMAMD     (BITP_UART_FCR_FDMAMD)
#define BITP_UART_COMFCR_TFCLR      (BITP_UART_FCR_TFCLR)
#define BITP_UART_COMFCR_RFCLR      (BITP_UART_FCR_RFCLR)
#define BITP_UART_COMFCR_FIFOEN     (BITP_UART_FCR_FIFOEN)
#define BITP_UART_COMFBR_FBEN       (BITP_UART_FBR_FBEN)
#define BITP_UART_COMFBR_DIVM       (BITP_UART_FBR_DIVM)
#define BITP_UART_COMFBR_DIVN       (BITP_UART_FBR_DIVN)
#define BITP_UART_COMDIV_DIV        (BITP_UART_DIV_DIV)
#define BITP_UART_COMLCR2_OSR       (BITP_UART_LCR2_OSR)
#define BITP_UART_COMCTL_REV        (BITP_UART_CTL_REV)
#define BITP_UART_COMCTL_RXINV      (BITP_UART_CTL_RXINV)
#define BITP_UART_COMCTL_FORCECLKON (BITP_UART_CTL_FORCECLK)
#define BITP_UART_COMRFC_RFC        (BITP_UART_RFC_RFC)
#define BITP_UART_COMTFC_TFC        (BITP_UART_TFC_TFC)
#define BITP_UART_COMRSC_DISTX      (BITP_UART_RSC_DISTX)
#define BITP_UART_COMRSC_DISRX      (BITP_UART_RSC_DISRX)
#define BITP_UART_COMRSC_OENSP      (BITP_UART_RSC_OENSP)
#define BITP_UART_COMRSC_OENP       (BITP_UART_RSC_OENP)
#define BITP_UART_COMACR_EEC        (BITP_UART_ACR_EEC)
#define BITP_UART_COMACR_SEC        (BITP_UART_ACR_SEC)
#define BITP_UART_COMACR_TOIEN      (BITP_UART_ACR_TOIEN)
#define BITP_UART_COMACR_DNIEN      (BITP_UART_ACR_DNIEN)
#define BITP_UART_COMACR_ABE        (BITP_UART_ACR_ABE)
#define BITP_UART_COMASRL_CNT       (BITP_UART_ASRL_CNT)
#define BITP_UART_COMASRL_NEETO     (BITP_UART_ASRL_NEETO)
#define BITP_UART_COMASRL_NSETO     (BITP_UART_ASRL_NSETO)
#define BITP_UART_COMASRL_BRKTO     (BITP_UART_ASRL_BRKTO)
#define BITP_UART_COMASRL_DONE      (BITP_UART_ASRL_DONE)
#define BITP_UART_COMASRH_CNT       (BITP_UART_ASRH_CNT)

#define BITM_UART_COMRX_RBR         (BITM_UART_RX_RBR)
#define BITM_UART_COMTX_THR         (BITM_UART_TX_THR)
#define BITM_UART_COMIEN_EDMAR      (BITM_UART_IEN_EDMAR)
#define BITM_UART_COMIEN_EDMAT      (BITM_UART_IEN_EDMAT)
#define BITM_UART_COMIEN_EDSSI      (BITM_UART_IEN_EDSSI)
#define BITM_UART_COMIEN_ELSI       (BITM_UART_IEN_ELSI)
#define BITM_UART_COMIEN_ETBEI      (BITM_UART_IEN_ETBEI)
#define BITM_UART_COMIEN_ERBFI      (BITM_UART_IEN_ERBFI)
#define BITM_UART_COMIIR_FEND       (BITM_UART_IIR_FEND)
#define BITM_UART_COMIIR_STA        (BITM_UART_IIR_STAT)
#define BITM_UART_COMIIR_NIRQ       (BITM_UART_IIR_NIRQ)
#define BITM_UART_COMLCR_BRK        (BITM_UART_LCR_BRK)
#define BITM_UART_COMLCR_SP         (BITM_UART_LCR_SP)
#define BITM_UART_COMLCR_EPS        (BITM_UART_LCR_EPS)
#define BITM_UART_COMLCR_PEN        (BITM_UART_LCR_PEN)
#define BITM_UART_COMLCR_STOP       (BITM_UART_LCR_STOP)
#define BITM_UART_COMLCR_WLS        (BITM_UART_LCR_WLS)
#define BITM_UART_COMMCR_LOOPBACK   (BITM_UART_MCR_LOOPBACK)
#define BITM_UART_COMMCR_OUT2       (BITM_UART_MCR_OUT2)
#define BITM_UART_COMMCR_OUT1       (BITM_UART_MCR_OUT1)
#define BITM_UART_COMMCR_RTS        (BITM_UART_MCR_RTS)
#define BITM_UART_COMMCR_DTR        (BITM_UART_MCR_DTR)
#define BITM_UART_COMLSR_FIFOERR    (BITM_UART_LSR_FIFOERR)
#define BITM_UART_COMLSR_TEMT       (BITM_UART_LSR_TEMT)
#define BITM_UART_COMLSR_THRE       (BITM_UART_LSR_THRE)
#define BITM_UART_COMLSR_BI         (BITM_UART_LSR_BI)
#define BITM_UART_COMLSR_FE         (BITM_UART_LSR_FE)
#define BITM_UART_COMLSR_PE         (BITM_UART_LSR_PE)
#define BITM_UART_COMLSR_OE         (BITM_UART_LSR_OE)
#define BITM_UART_COMLSR_DR         (BITM_UART_LSR_DR)
#define BITM_UART_COMMSR_DCD        (BITM_UART_MSR_DCD)
#define BITM_UART_COMMSR_RI         (BITM_UART_MSR_RI)
#define BITM_UART_COMMSR_DSR        (BITM_UART_MSR_DSR)
#define BITM_UART_COMMSR_CTS        (BITM_UART_MSR_CTS)
#define BITM_UART_COMMSR_DDCD       (BITM_UART_MSR_DDCD)
#define BITM_UART_COMMSR_TERI       (BITM_UART_MSR_TERI)
#define BITM_UART_COMMSR_DDSR       (BITM_UART_MSR_DDSR)
#define BITM_UART_COMMSR_DCTS       (BITM_UART_MSR_DCTS)
#define BITM_UART_COMSCR_SCR        (BITM_UART_SCR_SCR)
#define BITM_UART_COMFCR_RFTRIG     (BITM_UART_FCR_RFTRIG)
#define BITM_UART_COMFCR_FDMAMD     (BITM_UART_FCR_FDMAMD)
#define BITM_UART_COMFCR_TFCLR      (BITM_UART_FCR_TFCLR)
#define BITM_UART_COMFCR_RFCLR      (BITM_UART_FCR_RFCLR)
#define BITM_UART_COMFCR_FIFOEN     (BITM_UART_FCR_FIFOEN)
#define BITM_UART_COMFBR_FBEN       (BITM_UART_FBR_FBEN)
#define BITM_UART_COMFBR_DIVM       (BITM_UART_FBR_DIVM)
#define BITM_UART_COMFBR_DIVN       (BITM_UART_FBR_DIVN)
#define BITM_UART_COMDIV_DIV        (BITM_UART_DIV_DIV)
#define BITM_UART_COMLCR2_OSR       (BITM_UART_LCR2_OSR)
#define BITM_UART_COMCTL_REV        (BITM_UART_CTL_REV)
#define BITM_UART_COMCTL_RXINV      (BITM_UART_CTL_RXINV)
#define BITM_UART_COMCTL_FORCECLKON (BITM_UART_CTL_FORCECLK)
#define BITM_UART_COMRFC_RFC        (BITM_UART_RFC_RFC)
#define BITM_UART_COMTFC_TFC        (BITM_UART_TFC_TFC)
#define BITM_UART_COMRSC_DISTX      (BITM_UART_RSC_DISTX)
#define BITM_UART_COMRSC_DISRX      (BITM_UART_RSC_DISRX)
#define BITM_UART_COMRSC_OENSP      (BITM_UART_RSC_OENSP)
#define BITM_UART_COMRSC_OENP       (BITM_UART_RSC_OENP)
#define BITM_UART_COMACR_EEC        (BITM_UART_ACR_EEC)
#define BITM_UART_COMACR_SEC        (BITM_UART_ACR_SEC)
#define BITM_UART_COMACR_TOIEN      (BITM_UART_ACR_TOIEN)
#define BITM_UART_COMACR_DNIEN      (BITM_UART_ACR_DNIEN)
#define BITM_UART_COMACR_ABE        (BITM_UART_ACR_ABE)
#define BITM_UART_COMASRL_CNT       (BITM_UART_ASRL_CNT)
#define BITM_UART_COMASRL_NEETO     (BITM_UART_ASRL_NEETO)
#define BITM_UART_COMASRL_NSETO     (BITM_UART_ASRL_NSETO)
#define BITM_UART_COMASRL_BRKTO     (BITM_UART_ASRL_BRKTO)
#define BITM_UART_COMASRL_DONE      (BITM_UART_ASRL_DONE)
#define BITM_UART_COMASRH_CNT       (BITM_UART_ASRH_CNT)


/* Backward compatibility shim for corrected RTC_SR5.WPENDSR3 bit name. */

#define BITP_RTC_SR5_WPNDSR0        (BITP_RTC_SR5_WPENDSR3)
#define BITM_RTC_SR5_WPNDSR0        (BITM_RTC_SR5_WPENDSR3)


/* Backward compatibility shim for renamed crypto registers. */

#define BITP_CRYPT_CFG_KEYLEN       (BITP_CRYPT_CFG_AESKEYLEN)
#define BITP_CRYPT_CFG_ENDIAN       (BITP_CRYPT_CFG_AES_BYTESWAP)

#define ENUM_CRYPT_CFG_LITTLE_ENDIAN         (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))
#define ENUM_CRYPT_CFG_BIG_ENDIAN            (_ADI_MSK_3(0x00000040,0x00000040UL, uint32_t  ))


/* Workarounds for registers that are wrongly defined as signed in generated headers. */

#undef BITM_FLCC_KH_DATA0_VALUE
#define BITM_FLCC_KH_DATA0_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t ))    /*  Lower half of 64-bit dual word data to be written on a WRITE command */

#undef BITM_FLCC_KH_DATA1_VALUE
#define BITM_FLCC_KH_DATA1_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t ))    /*  Upper half of 64-bit dual word data to be written on a WRITE command */

#undef BITM_FLCC_KEY_VALUE
#undef ENUM_FLCC_KEY_USERKEY
#define BITM_FLCC_KEY_VALUE                  (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t ))    /*  Key register */
#define ENUM_FLCC_KEY_USERKEY                (_ADI_MSK_3(0x676C7565,0x676C7565UL, uint32_t ))    /*  VALUE: USERKEY */

#undef BITM_FLCC_WR_ABORT_ADDR_VALUE
#define BITM_FLCC_WR_ABORT_ADDR_VALUE        (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t ))    /*  Holds the address targeted by an ongoing write command and retains its value after an ABORT event */

#undef BITM_FLCC_WRPROT_WORD
#define BITM_FLCC_WRPROT_WORD                (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t ))    /*  Clear bits to write protect related groups of user space pages. Once cleared these bits can only be set again by resetting the part */

#undef BITM_FLCC_SIGNATURE_VALUE
#define BITM_FLCC_SIGNATURE_VALUE            (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t ))    /*  Provides read access to the most recently generated signature */

#undef BITM_FLCC_CACHE_KEY_VALUE
#define BITM_FLCC_CACHE_KEY_VALUE            (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t ))    /*  Cache Key register */


/* Redefine SPI_FLOW_CTL.RDBURSTSZ with correct size and position. */

#undef BITP_SPI_FLOW_CTL_RDBURSTSZ
#undef BITM_SPI_FLOW_CTL_RDBURSTSZ
#define BITP_SPI_FLOW_CTL_RDBURSTSZ           8
#define BITM_SPI_FLOW_CTL_RDBURSTSZ          (_ADI_MSK_3(0x00000F00,0x00000F00U, uint16_t  ))    /*  Read data burst size minus 1 */


#ifdef __ICCARM__
#pragma diag_default=Pm008,Pm009,Pm081
#endif /* __ICCARM__ */

#endif /* _WRAP_ADUCM3029_DEF_H */
