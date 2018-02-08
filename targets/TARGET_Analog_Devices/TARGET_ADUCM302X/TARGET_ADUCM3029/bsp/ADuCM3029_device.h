/*!
*****************************************************************************
 * @file:    ADuCM3029_device.h
 * @brief:   ADuCM3029 C Register Definitions
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

#ifndef _WRAP_ADUCM3029_DEVICE_H
#define _WRAP_ADUCM3029_DEVICE_H

#include <ADuCM3029_typedefs.h>
#include <sys/adi_cio_macros.h>

#ifdef __ICCARM__
/* IAR MISRA C 2004 error suppressions:
 *
 * Pm093 (rule 18.4): use of union - overlapping storage shall not be used.
 *    Unions are required by sys/ADUCM302x_device.h.
 */
#pragma diag_suppress=Pm093
#endif /* __ICCARM__ */

/* UART and RTC structs need to be redefined with backward compatibility names. */
#define __ADI_NO_DECL_STRUCT_ADI_UART_TypeDef__
#define __ADI_NO_DECL_STRUCT_ADI_RTC_TypeDef__

/* The generated header. */
#include <sys/ADuCM302x_device.h>

typedef struct _ADI_UART_TypeDef
{
    union {
    __I __C  uint16_t RX;                            /*!< Receive Buffer Register */
    __I __C  uint16_t COMRX;
    __O      uint16_t TX;                            /*!< Transmit Holding Register */
    __O      uint16_t COMTX;
    };
    __I __C  uint8_t  RESERVED0[2];
    union {
    __IO     uint16_t IEN;                           /*!< Interrupt Enable */
    __IO     uint16_t COMIEN;
    };
    __I __C  uint8_t  RESERVED1[2];
    union {
    __I __C  uint16_t IIR;                           /*!< Interrupt ID */
    __I __C  uint16_t COMIIR;
    };
    __I __C  uint8_t  RESERVED2[2];
    union {
    __IO     uint16_t LCR;                           /*!< Line Control */
    __IO     uint16_t COMLCR;
    };
    __I __C  uint8_t  RESERVED3[2];
    union {
    __IO     uint16_t MCR;                           /*!< Modem Control */
    __IO     uint16_t COMMCR;
    };
    __I __C  uint8_t  RESERVED4[2];
    union {
    __I __C  uint16_t LSR;                           /*!< Line Status */
    __I __C  uint16_t COMLSR;
    };
    __I __C  uint8_t  RESERVED5[2];
    union {
    __I __C  uint16_t MSR;                           /*!< Modem Status */
    __I __C  uint16_t COMMSR;
    };
    __I __C  uint8_t  RESERVED6[2];
    union {
    __IO     uint16_t SCR;                           /*!< Scratch buffer */
    __IO     uint16_t COMSCR;
    };
    __I __C  uint8_t  RESERVED7[2];
    union {
    __IO     uint16_t FCR;                           /*!< FIFO Control */
    __IO     uint16_t COMFCR;
    };
    __I __C  uint8_t  RESERVED8[2];
    union {
    __IO     uint16_t FBR;                           /*!< Fractional Baud Rate */
    __IO     uint16_t COMFBR;
    };
    __I __C  uint8_t  RESERVED9[2];
    union {
    __IO     uint16_t DIV;                           /*!< Baudrate divider */
    __IO     uint16_t COMDIV;
    };
    __I __C  uint8_t  RESERVED10[2];
    union {
    __IO     uint16_t LCR2;                          /*!< second Line Control */
    __IO     uint16_t COMLCR2;
    };
    __I __C  uint8_t  RESERVED11[2];
    union {
    __IO     uint16_t CTL;                           /*!< UART control register */
    __IO     uint16_t COMCTL;
    };
    __I __C  uint8_t  RESERVED12[2];
    union {
    __I __C  uint16_t RFC;                           /*!< RX FIFO byte count */
    __I __C  uint16_t COMRFC;
    };
    __I __C  uint8_t  RESERVED13[2];
    union {
    __I __C  uint16_t TFC;                           /*!< TX FIFO byte count */
    __I __C  uint16_t COMTFC;
    };
    __I __C  uint8_t  RESERVED14[2];
    union {
    __IO     uint16_t RSC;                           /*!< RS485 half-duplex Control */
    __IO     uint16_t COMRSC;
    };
    __I __C  uint8_t  RESERVED15[2];
    union {
    __IO     uint16_t ACR;                           /*!< Auto Baud Control */
    __IO     uint16_t COMACR;
    };
    __I __C  uint8_t  RESERVED16[2];
    union {
    __I __C  uint16_t ASRL;                          /*!< Auto Baud Status (Low) */
    __I __C  uint16_t COMASRL;
    };
    __I __C  uint8_t  RESERVED17[2];
    union {
    __I __C  uint16_t ASRH;                          /*!< Auto Baud Status (High) */
    __I __C  uint16_t COMASRH;
    };
} ADI_UART_TypeDef;


typedef struct _ADI_RTC_TypeDef
{
    __IO     uint16_t   CR0;                           /*!< RTC Control 0 */
    __I __C  uint8_t  RESERVED0[2];
    __IO     uint16_t   SR0;                           /*!< RTC Status 0 */
    __I __C  uint8_t  RESERVED1[2];
    __I __C  uint16_t   SR1;                           /*!< RTC Status 1 */
    __I __C  uint8_t  RESERVED2[2];
    __IO     uint16_t   CNT0;                          /*!< RTC Count 0 */
    __I __C  uint8_t  RESERVED3[2];
    __IO     uint16_t   CNT1;                          /*!< RTC Count 1 */
    __I __C  uint8_t  RESERVED4[2];
    __IO     uint16_t   ALM0;                          /*!< RTC Alarm 0 */
    __I __C  uint8_t  RESERVED5[2];
    __IO     uint16_t   ALM1;                          /*!< RTC Alarm 1 */
    __I __C  uint8_t  RESERVED6[2];
    __IO     uint16_t   TRM;                           /*!< RTC Trim */
    __I __C  uint8_t  RESERVED7[2];
    __O      uint16_t   GWY;                           /*!< RTC Gateway */
    __I __C  uint8_t  RESERVED8[6];
    __IO     uint16_t   CR1;                           /*!< RTC Control 1 */
    __I __C  uint8_t  RESERVED9[2];
    __IO     uint16_t   SR2;                           /*!< RTC Status 2 */
    __I __C  uint8_t  RESERVED10[2];
    __I __C  uint16_t   SNAP0;                         /*!< RTC Snapshot 0 */
    __I __C  uint8_t  RESERVED11[2];
    __I __C  uint16_t   SNAP1;                         /*!< RTC Snapshot 1 */
    __I __C  uint8_t  RESERVED12[2];
    __I __C  uint16_t   SNAP2;                         /*!< RTC Snapshot 2 */
    __I __C  uint8_t  RESERVED13[2];
    __I __C  uint16_t   MOD;                           /*!< RTC Modulo */
    __I __C  uint8_t  RESERVED14[2];
    __I __C  uint16_t   CNT2;                          /*!< RTC Count 2 */
    __I __C  uint8_t  RESERVED15[2];
    __IO     uint16_t   ALM2;                          /*!< RTC Alarm 2 */
    __I __C  uint8_t  RESERVED16[2];
    __IO     uint16_t   SR3;                           /*!< RTC Status 3 */
    __I __C  uint8_t  RESERVED17[2];
    __IO     uint16_t   CR2IC;                         /*!< RTC Control 2 for Configuring Input Capture Channels */
    __I __C  uint8_t  RESERVED18[2];
    union {
    __IO     uint16_t   CR3SS;                         /*!< RTC Control 3 for Configuring SensorStrobe Channel */
    __IO     uint16_t   CR3OC;
    };
    __I __C  uint8_t  RESERVED19[2];
    union {
    __IO     uint16_t   CR4SS;                         /*!< RTC Control 4 for Configuring SensorStrobe Channel */
    __IO     uint16_t   CR4OC;
    };
    __I __C  uint8_t  RESERVED20[2];
    union {
    __IO     uint16_t   SSMSK;                         /*!< RTC Mask for SensorStrobe Channel */
    __IO     uint16_t   OCMSK;
    };
    __I __C  uint8_t  RESERVED21[2];
    union {
    __IO     uint16_t   SS1ARL;                        /*!< RTC Auto-Reload for SensorStrobe Channel 1 */
    __IO     uint16_t   OC1ARL;
    };
    __I __C  uint8_t  RESERVED22[6];
    __I __C  uint16_t   IC2;                           /*!< RTC Input Capture Channel 2 */
    __I __C  uint8_t  RESERVED23[2];
    __I __C  uint16_t   IC3;                           /*!< RTC Input Capture Channel 3 */
    __I __C  uint8_t  RESERVED24[2];
    __I __C  uint16_t   IC4;                           /*!< RTC Input Capture Channel 4 */
    __I __C  uint8_t  RESERVED25[2];
    union {
    __IO     uint16_t   SS1;                           /*!< RTC SensorStrobe Channel 1 */
    __IO     uint16_t   OC1;
    };
    __I __C  uint8_t  RESERVED26[14];
    __I __C  uint16_t   SR4;                           /*!< RTC Status 4 */
    __I __C  uint8_t  RESERVED27[2];
    __I __C  uint16_t   SR5;                           /*!< RTC Status 5 */
    __I __C  uint8_t  RESERVED28[2];
    __I __C  uint16_t   SR6;                           /*!< RTC Status 6 */
    __I __C  uint8_t  RESERVED29[2];
    union {
    __I __C  uint16_t   SS1TGT;                        /*!< RTC SensorStrobe Channel 1 Target */
    __I __C  uint16_t   OC1TGT;
    };
    __I __C  uint8_t  RESERVED30[2];
    __I __C  uint16_t   FRZCNT;                        /*!< RTC Freeze Count */
} ADI_RTC_TypeDef;


#ifdef __ICCARM__
#pragma diag_default=Pm093
#endif /* __ICCARM__ */

#endif /* _WRAP_ADUCM3029_DEVICE_H__ */
