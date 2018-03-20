/* ================================================================================

     Project      :   ADuCM4050
     File         :   ADuCM4050_device.h
     Description  :   C Register Definitions

     Date         :   Feb 7, 2017

     Copyright (c) 2014-2017 Analog Devices, Inc.  All Rights Reserved.
     This software is proprietary and confidential to Analog Devices, Inc. and
     its licensors.

     This file was auto-generated. Do not make local changes to this file.

   ================================================================================ */

#ifndef _ADUCM4050_DEVICE_H
#define _ADUCM4050_DEVICE_H

/* pickup integer types */
#if defined(_LANGUAGE_C) || (defined(__GNUC__) && !defined(__ASSEMBLER__))
#include <stdint.h>
#endif /* _LANGUAGE_C */

/* pickup register bitfield and bit masks */
#include "adi_ADuCM4050_typedefs.h"

#ifndef __IO
#ifdef __cplusplus
#define     __I     volatile      /* read-only */
#define     __C
#else
#define     __I     volatile      /* read-only */
#define     __C     const
#endif
#define     __O     volatile      /* write-only */
#define     __IO    volatile      /* read-write */
#endif

#if defined (_MISRA_RULES)
/*
  anonymous unions violate ISO 9899:1990 and therefore MISRA Rule 1.1.
  Use of unions violates MISRA Rule 18.4.
  Anonymous unions are required for this implementation.
  Re-use of identifiers violates MISRA Rule 5.7.
  Field names are repeated for the ADuCM4050 register map.
*/
#pragma diag(push)
#pragma diag(suppress:misra_rule_1_1:"Allow anonymous unions")
#pragma diag(suppress:misra_rule_5_1:"Allow names over 32 character limit")
#pragma diag(suppress:misra_rule_5_3:"Header will re-use typedef identifiers")
#pragma diag(suppress:misra_rule_5_6:"Header will re-use identifiers in the same scope")
#pragma diag(suppress:misra_rule_5_7:"Header will re-use identifiers")
#pragma diag(suppress:misra_rule_18_4:"Allow the use of a union")
#endif /* _MISRA_RULES */

/** @defgroup TMR General Purpose Timer (TMR) Module
 *  General Purpose Timer
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_TMR_TypeDef
 *  \brief  General Purpose Timer
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_TypeDef__
typedef struct _ADI_TMR_TypeDef
{
    __IO     uint16_t   LOAD;                          /*!< 16-bit Load Value */
    __I __C  uint8_t  RESERVED0[2];
    __I __C  uint16_t   CURCNT;                        /*!< 16-bit Timer Value */
    __I __C  uint8_t  RESERVED1[2];
    __IO     uint16_t   CTL;                           /*!< Control */
    __I __C  uint8_t  RESERVED2[2];
    __O      uint16_t   CLRINT;                        /*!< Clear Interrupt */
    __I __C  uint8_t  RESERVED3[2];
    __I __C  uint16_t   CAPTURE;                       /*!< Capture */
    __I __C  uint8_t  RESERVED4[2];
    __IO     uint16_t   ALOAD;                         /*!< 16-bit Load Value, Asynchronous */
    __I __C  uint8_t  RESERVED5[2];
    __I __C  uint16_t   ACURCNT;                       /*!< 16-bit Timer Value, Asynchronous */
    __I __C  uint8_t  RESERVED6[2];
    __I __C  uint16_t   STAT;                          /*!< Status */
    __I __C  uint8_t  RESERVED7[2];
    __IO     uint16_t   PWMCTL;                        /*!< PWM Control Register */
    __I __C  uint8_t  RESERVED8[2];
    __IO     uint16_t   PWMMATCH;                      /*!< PWM Match Value */
    __I __C  uint8_t  RESERVED9[2];
    __IO     uint16_t   EVENTSELECT;                   /*!< Timer Event Selection Register */
} ADI_TMR_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_TypeDef__ */

/*!@}*/

/** @defgroup TMR_RGB Timer_RGB with 3 PWM outputs (TMR_RGB) Module
 *  Timer_RGB with 3 PWM outputs
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_TMR_RGB_TypeDef
 *  \brief  Timer_RGB with 3 PWM outputs
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_TMR_RGB_TypeDef__
typedef struct _ADI_TMR_RGB_TypeDef
{
    __IO     uint16_t   LOAD;                          /*!< 16-bit load value */
    __I __C  uint8_t  RESERVED0[2];
    __I __C  uint16_t   CURCNT;                        /*!< 16-bit timer value */
    __I __C  uint8_t  RESERVED1[2];
    __IO     uint16_t   CTL;                           /*!< Control */
    __I __C  uint8_t  RESERVED2[2];
    __O      uint16_t   CLRINT;                        /*!< Clear interrupt */
    __I __C  uint8_t  RESERVED3[2];
    __I __C  uint16_t   CAPTURE;                       /*!< Capture */
    __I __C  uint8_t  RESERVED4[2];
    __IO     uint16_t   ALOAD;                         /*!< 16-bit load value, asynchronous */
    __I __C  uint8_t  RESERVED5[2];
    __I __C  uint16_t   ACURCNT;                       /*!< 16-bit timer value, asynchronous */
    __I __C  uint8_t  RESERVED6[2];
    __I __C  uint16_t   STAT;                          /*!< Status */
    __I __C  uint8_t  RESERVED7[2];
    __IO     uint16_t   PWM0CTL;                       /*!< PWM0 Control Register */
    __I __C  uint8_t  RESERVED8[2];
    __IO     uint16_t   PWM0MATCH;                     /*!< PWM0 Match Value */
    __I __C  uint8_t  RESERVED9[2];
    __IO     uint16_t   EVENTSELECT;                   /*!< Timer Event selection Register */
    __I __C  uint8_t  RESERVED10[2];
    __IO     uint16_t   PWM1CTL;                       /*!< PWM1 Control Register */
    __I __C  uint8_t  RESERVED11[2];
    __IO     uint16_t   PWM1MATCH;                     /*!< PWM1 Match Value */
    __I __C  uint8_t  RESERVED12[2];
    __IO     uint16_t   PWM2CTL;                       /*!< PWM2 Control Register */
    __I __C  uint8_t  RESERVED13[2];
    __IO     uint16_t   PWM2MATCH;                     /*!< PWM2 Match Value */
} ADI_TMR_RGB_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_TMR_RGB_TypeDef__ */

/*!@}*/

/** @defgroup RTC Real-Time Clock (RTC) Module
 *  Real-Time Clock
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_RTC_TypeDef
 *  \brief  Real-Time Clock
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RTC_TypeDef__
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
    __IO     uint16_t   CR3SS;                         /*!< RTC Control 3 for Configuring SensorStrobe Channel */
    __I __C  uint8_t  RESERVED19[2];
    __IO     uint16_t   CR4SS;                         /*!< RTC Control 4 for Configuring SensorStrobe Channel */
    __I __C  uint8_t  RESERVED20[2];
    __IO     uint16_t   SSMSK;                         /*!< RTC Mask for SensorStrobe Channel */
    __I __C  uint8_t  RESERVED21[10];
    __I __C  uint16_t   IC2;                           /*!< RTC Input Capture Channel 2 */
    __I __C  uint8_t  RESERVED22[2];
    __I __C  uint16_t   IC3;                           /*!< RTC Input Capture Channel 3 */
    __I __C  uint8_t  RESERVED23[2];
    __I __C  uint16_t   IC4;                           /*!< RTC Input Capture Channel 4 */
    __I __C  uint8_t  RESERVED24[2];
    __IO     uint16_t   SS1;                           /*!< RTC SensorStrobe Channel 1 */
    __I __C  uint8_t  RESERVED25[2];
    __IO     uint16_t   SS2;                           /*!< RTC SensorStrobe Channel 2 */
    __I __C  uint8_t  RESERVED26[2];
    __IO     uint16_t   SS3;                           /*!< RTC SensorStrobe Channel 3 */
    __I __C  uint8_t  RESERVED27[2];
    __IO     uint16_t   SS4;                           /*!< RTC SensorStrobe Channel 4 */
    __I __C  uint8_t  RESERVED28[2];
    __I __C  uint16_t   SR4;                           /*!< RTC Status 4 */
    __I __C  uint8_t  RESERVED29[2];
    __I __C  uint16_t   SR5;                           /*!< RTC Status 5 */
    __I __C  uint8_t  RESERVED30[2];
    __I __C  uint16_t   SR6;                           /*!< RTC Status 6 */
    __I __C  uint8_t  RESERVED31[2];
    __I __C  uint16_t   SS1TGT;                        /*!< RTC SensorStrobe Channel 1 Target */
    __I __C  uint8_t  RESERVED32[2];
    __I __C  uint16_t   FRZCNT;                        /*!< RTC Freeze Count */
    __I __C  uint8_t  RESERVED33[2];
    __I __C  uint16_t   SS2TGT;                        /*!< RTC SensorStrobe Channel 2 Target */
    __I __C  uint8_t  RESERVED34[2];
    __I __C  uint16_t   SS3TGT;                        /*!< RTC SensorStrobe Channel 3 Target */
    __I __C  uint8_t  RESERVED35[6];
    __IO     uint16_t   SS1LOWDUR;                     /*!< RTC Auto-Reload Low Duration for SensorStrobe Channel 1 */
    __I __C  uint8_t  RESERVED36[2];
    __IO     uint16_t   SS2LOWDUR;                     /*!< RTC Auto-Reload Low Duration for SensorStrobe Channel 2 */
    __I __C  uint8_t  RESERVED37[2];
    __IO     uint16_t   SS3LOWDUR;                     /*!< RTC Auto-Reload Low Duration for SensorStrobe Channel 3 */
    __I __C  uint8_t  RESERVED38[6];
    __IO     uint16_t   SS1HIGHDUR;                    /*!< RTC Auto-Reload High Duration for SensorStrobe Channel 1 */
    __I __C  uint8_t  RESERVED39[2];
    __IO     uint16_t   SS2HIGHDUR;                    /*!< RTC Auto-Reload High Duration for SensorStrobe Channel 2 */
    __I __C  uint8_t  RESERVED40[2];
    __IO     uint16_t   SS3HIGHDUR;                    /*!< RTC Auto-Reload High Duration for SensorStrobe Channel 3 */
    __I __C  uint8_t  RESERVED41[6];
    __IO     uint16_t   SSMSKOT;                       /*!< RTC Masks for SensorStrobe Channels on Time Control */
    __I __C  uint8_t  RESERVED42[2];
    __IO     uint16_t   CR5SSS;                        /*!< RTC Control 5 for Configuring SensorStrobe Channel GPIO Sampling */
    __I __C  uint8_t  RESERVED43[2];
    __IO     uint16_t   CR6SSS;                        /*!< RTC Control 6 for Configuring SensorStrobe Channel GPIO Sampling Edge */
    __I __C  uint8_t  RESERVED44[2];
    __IO     uint16_t   CR7SSS;                        /*!< RTC Control 7 for Configuring SensorStrobe Channel GPIO Sampling Activity */
    __I __C  uint8_t  RESERVED45[2];
    __IO     uint16_t   SR7;                           /*!< RTC Status 7 */
    __I __C  uint8_t  RESERVED46[2];
    __I __C  uint16_t   SR8;                           /*!< RTC Status 8 */
    __I __C  uint8_t  RESERVED47[2];
    __I __C  uint16_t   SR9;                           /*!< RTC Status 9 */
    __I __C  uint8_t  RESERVED48[6];
    __IO     uint16_t   GPMUX0;                        /*!< RTC GPIO Pin Mux Control Register 0 */
    __I __C  uint8_t  RESERVED49[2];
    __IO     uint16_t   GPMUX1;                        /*!< RTC GPIO Pin Mux Control Register 1 */
} ADI_RTC_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RTC_TypeDef__ */

/*!@}*/

/** @defgroup SYS System Identification and Debug Enable (SYS) Module
 *  System Identification and Debug Enable
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_SYS_TypeDef
 *  \brief  System Identification and Debug Enable
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SYS_TypeDef__
typedef struct _ADI_SYS_TypeDef
{
    __I __C  uint8_t  RESERVED0[32];
    __I __C  uint16_t   ADIID;                         /*!< ADI Identification */
    __I __C  uint8_t  RESERVED1[2];
    __I __C  uint16_t   CHIPID;                        /*!< Chip Identifier */
    __I __C  uint8_t  RESERVED2[26];
    __O      uint16_t   SWDEN;                         /*!< Serial Wire Debug Enable */
} ADI_SYS_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SYS_TypeDef__ */

/*!@}*/

/** @defgroup WDT Watchdog Timer (WDT) Module
 *  Watchdog Timer
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_WDT_TypeDef
 *  \brief  Watchdog Timer
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_WDT_TypeDef__
typedef struct _ADI_WDT_TypeDef
{
    __IO     uint16_t   LOAD;                          /*!< Load Value */
    __I __C  uint8_t  RESERVED0[2];
    __I __C  uint16_t   CCNT;                          /*!< Current Count Value */
    __I __C  uint8_t  RESERVED1[2];
    __IO     uint16_t   CTL;                           /*!< Control */
    __I __C  uint8_t  RESERVED2[2];
    __O      uint16_t   RESTART;                       /*!< Clear Interrupt */
    __I __C  uint8_t  RESERVED3[10];
    __I __C  uint16_t   STAT;                          /*!< Status */
} ADI_WDT_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_WDT_TypeDef__ */

/*!@}*/

/** @defgroup I2C I2C Master/Slave (I2C) Module
 *  I2C Master/Slave
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_I2C_TypeDef
 *  \brief  I2C Master/Slave
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_I2C_TypeDef__
typedef struct _ADI_I2C_TypeDef
{
    __IO     uint16_t   MCTL;                          /*!< Master Control */
    __I __C  uint8_t  RESERVED0[2];
    __IO     uint16_t   MSTAT;                         /*!< Master Status */
    __I __C  uint8_t  RESERVED1[2];
    __I __C  uint16_t   MRX;                           /*!< Master Receive Data */
    __I __C  uint8_t  RESERVED2[2];
    __IO     uint16_t   MTX;                           /*!< Master Transmit Data */
    __I __C  uint8_t  RESERVED3[2];
    __IO     uint16_t   MRXCNT;                        /*!< Master Receive Data Count */
    __I __C  uint8_t  RESERVED4[2];
    __I __C  uint16_t   MCRXCNT;                       /*!< Master Current Receive Data Count */
    __I __C  uint8_t  RESERVED5[2];
    __IO     uint16_t   ADDR1;                         /*!< Master Address Byte 1 */
    __I __C  uint8_t  RESERVED6[2];
    __IO     uint16_t   ADDR2;                         /*!< Master Address Byte 2 */
    __I __C  uint8_t  RESERVED7[2];
    __IO     uint16_t   BYT;                           /*!< Start Byte */
    __I __C  uint8_t  RESERVED8[2];
    __IO     uint16_t   DIV;                           /*!< Serial Clock Period Divisor */
    __I __C  uint8_t  RESERVED9[2];
    __IO     uint16_t   SCTL;                          /*!< Slave Control */
    __I __C  uint8_t  RESERVED10[2];
    __IO     uint16_t   SSTAT;                         /*!< Slave I2C Status/Error/IRQ */
    __I __C  uint8_t  RESERVED11[2];
    __I __C  uint16_t   SRX;                           /*!< Slave Receive */
    __I __C  uint8_t  RESERVED12[2];
    __IO     uint16_t   STX;                           /*!< Slave Transmit */
    __I __C  uint8_t  RESERVED13[2];
    __IO     uint16_t   ALT;                           /*!< Hardware General Call ID */
    __I __C  uint8_t  RESERVED14[2];
    __IO     uint16_t   ID0;                           /*!< First Slave Address Device ID */
    __I __C  uint8_t  RESERVED15[2];
    __IO     uint16_t   ID1;                           /*!< Second Slave Address Device ID */
    __I __C  uint8_t  RESERVED16[2];
    __IO     uint16_t   ID2;                           /*!< Third Slave Address Device ID */
    __I __C  uint8_t  RESERVED17[2];
    __IO     uint16_t   ID3;                           /*!< Fourth Slave Address Device ID */
    __I __C  uint8_t  RESERVED18[2];
    __IO     uint16_t   STAT;                          /*!< Master and Slave FIFO Status */
    __I __C  uint8_t  RESERVED19[2];
    __O      uint16_t   SHCTL;                         /*!< Shared Control */
    __I __C  uint8_t  RESERVED20[2];
    __IO     uint16_t   TCTL;                          /*!< Timing Control Register */
    __I __C  uint8_t  RESERVED21[2];
    __IO     uint16_t   ASTRETCH_SCL;                  /*!< Automatic Stretch SCL */
} ADI_I2C_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_I2C_TypeDef__ */

/*!@}*/

/** @defgroup SPI Serial Peripheral Interface (SPI) Module
 *  Serial Peripheral Interface
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_SPI_TypeDef
 *  \brief  Serial Peripheral Interface
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPI_TypeDef__
typedef struct _ADI_SPI_TypeDef
{
    __IO     uint16_t   STAT;                          /*!< Status */
    __I __C  uint8_t  RESERVED0[2];
    __I __C  uint16_t   RX;                            /*!< Receive */
    __I __C  uint8_t  RESERVED1[2];
    __O      uint16_t   TX;                            /*!< Transmit */
    __I __C  uint8_t  RESERVED2[2];
    __IO     uint16_t   DIV;                           /*!< SPI Baud Rate Selection */
    __I __C  uint8_t  RESERVED3[2];
    __IO     uint16_t   CTL;                           /*!< SPI Configuration */
    __I __C  uint8_t  RESERVED4[2];
    __IO     uint16_t   IEN;                           /*!< SPI Interrupts Enable */
    __I __C  uint8_t  RESERVED5[2];
    __IO     uint16_t   CNT;                           /*!< Transfer Byte Count */
    __I __C  uint8_t  RESERVED6[2];
    __IO     uint16_t   DMA;                           /*!< SPI DMA Enable */
    __I __C  uint8_t  RESERVED7[2];
    __I __C  uint16_t   FIFO_STAT;                     /*!< FIFO Status */
    __I __C  uint8_t  RESERVED8[2];
    __IO     uint16_t   RD_CTL;                        /*!< Read Control */
    __I __C  uint8_t  RESERVED9[2];
    __IO     uint16_t   FLOW_CTL;                      /*!< Flow Control */
    __I __C  uint8_t  RESERVED10[2];
    __IO     uint16_t   WAIT_TMR;                      /*!< Wait Timer for Flow Control */
    __I __C  uint8_t  RESERVED11[2];
    __IO     uint16_t   CS_CTL;                        /*!< Chip Select Control for Multi-slave Connections */
    __I __C  uint8_t  RESERVED12[2];
    __IO     uint16_t   CS_OVERRIDE;                   /*!< Chip Select Override */
    __I __C  uint8_t  RESERVED13[4];
} ADI_SPI_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPI_TypeDef__ */

/*!@}*/

/** @defgroup UART  (UART) Module
 *  
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_UART_TypeDef
 *  \brief  
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_UART_TypeDef__
typedef struct _ADI_UART_TypeDef
{
    union {
    __I __C  uint16_t   RX;                            /*!< Receive Buffer Register */
    __O      uint16_t   TX;                            /*!< Transmit Holding Register */
    };
    __I __C  uint8_t  RESERVED0[2];
    __IO     uint16_t   IEN;                           /*!< Interrupt Enable */
    __I __C  uint8_t  RESERVED1[2];
    __I __C  uint16_t   IIR;                           /*!< Interrupt ID */
    __I __C  uint8_t  RESERVED2[2];
    __IO     uint16_t   LCR;                           /*!< Line Control */
    __I __C  uint8_t  RESERVED3[2];
    __IO     uint16_t   MCR;                           /*!< Modem Control */
    __I __C  uint8_t  RESERVED4[2];
    __I __C  uint16_t   LSR;                           /*!< Line Status */
    __I __C  uint8_t  RESERVED5[2];
    __I __C  uint16_t   MSR;                           /*!< Modem Status */
    __I __C  uint8_t  RESERVED6[2];
    __IO     uint16_t   SCR;                           /*!< Scratch Buffer */
    __I __C  uint8_t  RESERVED7[2];
    __IO     uint16_t   FCR;                           /*!< FIFO Control */
    __I __C  uint8_t  RESERVED8[2];
    __IO     uint16_t   FBR;                           /*!< Fractional Baud Rate */
    __I __C  uint8_t  RESERVED9[2];
    __IO     uint16_t   DIV;                           /*!< Baud Rate Divider */
    __I __C  uint8_t  RESERVED10[2];
    __IO     uint16_t   LCR2;                          /*!< Second Line Control */
    __I __C  uint8_t  RESERVED11[2];
    __IO     uint16_t   CTL;                           /*!< UART Control Register */
    __I __C  uint8_t  RESERVED12[2];
    __I __C  uint16_t   RFC;                           /*!< RX FIFO Byte Count */
    __I __C  uint8_t  RESERVED13[2];
    __I __C  uint16_t   TFC;                           /*!< TX FIFO Byte Count */
    __I __C  uint8_t  RESERVED14[2];
    __IO     uint16_t   RSC;                           /*!< RS485 Half-duplex Control */
    __I __C  uint8_t  RESERVED15[2];
    __IO     uint16_t   ACR;                           /*!< Auto Baud Control */
    __I __C  uint8_t  RESERVED16[2];
    __I __C  uint16_t   ASRL;                          /*!< Auto Baud Status (Low) */
    __I __C  uint8_t  RESERVED17[2];
    __I __C  uint16_t   ASRH;                          /*!< Auto Baud Status (High) */
} ADI_UART_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_UART_TypeDef__ */

/*!@}*/

/** @defgroup BEEP Beeper Driver (BEEP) Module
 *  Beeper Driver
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_BEEP_TypeDef
 *  \brief  Beeper Driver
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_BEEP_TypeDef__
typedef struct _ADI_BEEP_TypeDef
{
    __IO     uint16_t   CFG;                           /*!< Beeper Configuration */
    __I __C  uint8_t  RESERVED0[2];
    __IO     uint16_t   STAT;                          /*!< Beeper Status */
    __I __C  uint8_t  RESERVED1[2];
    __IO     uint16_t   TONEA;                         /*!< Tone A Data */
    __I __C  uint8_t  RESERVED2[2];
    __IO     uint16_t   TONEB;                         /*!< Tone B Data */
} ADI_BEEP_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_BEEP_TypeDef__ */

/*!@}*/

/** @defgroup ADC  (ADC) Module
 *  
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_ADC_TypeDef
 *  \brief  
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_ADC_TypeDef__
typedef struct _ADI_ADC_TypeDef
{
    __IO     uint16_t   CFG;                           /*!< ADC Configuration */
    __I __C  uint8_t  RESERVED0[2];
    __IO     uint16_t   PWRUP;                         /*!< ADC Power-up Time */
    __I __C  uint8_t  RESERVED1[2];
    __IO     uint16_t   CAL_WORD;                      /*!< Calibration Word */
    __I __C  uint8_t  RESERVED2[2];
    __IO     uint16_t   CNV_CFG;                       /*!< ADC Conversion Configuration */
    __I __C  uint8_t  RESERVED3[2];
    __IO     uint16_t   CNV_TIME;                      /*!< ADC Conversion Time */
    __I __C  uint8_t  RESERVED4[2];
    __IO     uint16_t   AVG_CFG;                       /*!< Averaging Configuration */
    __I __C  uint8_t  RESERVED5[10];
    __IO     uint16_t   IRQ_EN;                        /*!< Interrupt Enable */
    __I __C  uint8_t  RESERVED6[2];
    __IO     uint16_t   STAT;                          /*!< ADC Status */
    __I __C  uint8_t  RESERVED7[2];
    __IO     uint16_t   OVF;                           /*!< Overflow of Output Registers */
    __I __C  uint8_t  RESERVED8[2];
    __IO     uint16_t   ALERT;                         /*!< Alert Indication */
    __I __C  uint8_t  RESERVED9[2];
    __I __C  uint16_t   CH0_OUT;                       /*!< Conversion Result Channel 0 */
    __I __C  uint8_t  RESERVED10[2];
    __I __C  uint16_t   CH1_OUT;                       /*!< Conversion Result Channel 1 */
    __I __C  uint8_t  RESERVED11[2];
    __I __C  uint16_t   CH2_OUT;                       /*!< Conversion Result Channel 2 */
    __I __C  uint8_t  RESERVED12[2];
    __I __C  uint16_t   CH3_OUT;                       /*!< Conversion Result Channel 3 */
    __I __C  uint8_t  RESERVED13[2];
    __I __C  uint16_t   CH4_OUT;                       /*!< Conversion Result Channel 4 */
    __I __C  uint8_t  RESERVED14[2];
    __I __C  uint16_t   CH5_OUT;                       /*!< Conversion Result Channel 5 */
    __I __C  uint8_t  RESERVED15[2];
    __I __C  uint16_t   CH6_OUT;                       /*!< Conversion Result Channel 6 */
    __I __C  uint8_t  RESERVED16[2];
    __I __C  uint16_t   CH7_OUT;                       /*!< Conversion Result Channel 7 */
    __I __C  uint8_t  RESERVED17[2];
    __I __C  uint16_t   BAT_OUT;                       /*!< Battery Monitoring Result */
    __I __C  uint8_t  RESERVED18[2];
    __I __C  uint16_t   TMP_OUT;                       /*!< Temperature Result */
    __I __C  uint8_t  RESERVED19[2];
    __I __C  uint16_t   TMP2_OUT;                      /*!< Temperature Result 2 */
    __I __C  uint8_t  RESERVED20[2];
    __I __C  uint16_t   DMA_OUT;                       /*!< DMA Output Register */
    __I __C  uint8_t  RESERVED21[2];
    __IO     uint16_t   LIM0_LO;                       /*!< Channel 0 Low Limit */
    __I __C  uint8_t  RESERVED22[2];
    __IO     uint16_t   LIM0_HI;                       /*!< Channel 0 High Limit */
    __I __C  uint8_t  RESERVED23[2];
    __IO     uint16_t   HYS0;                          /*!< Channel 0 Hysteresis */
    __I __C  uint8_t  RESERVED24[6];
    __IO     uint16_t   LIM1_LO;                       /*!< Channel 1 Low Limit */
    __I __C  uint8_t  RESERVED25[2];
    __IO     uint16_t   LIM1_HI;                       /*!< Channel 1 High Limit */
    __I __C  uint8_t  RESERVED26[2];
    __IO     uint16_t   HYS1;                          /*!< Channel 1 Hysteresis */
    __I __C  uint8_t  RESERVED27[6];
    __IO     uint16_t   LIM2_LO;                       /*!< Channel 2 Low Limit */
    __I __C  uint8_t  RESERVED28[2];
    __IO     uint16_t   LIM2_HI;                       /*!< Channel 2 High Limit */
    __I __C  uint8_t  RESERVED29[2];
    __IO     uint16_t   HYS2;                          /*!< Channel 2 Hysteresis */
    __I __C  uint8_t  RESERVED30[6];
    __IO     uint16_t   LIM3_LO;                       /*!< Channel 3 Low Limit */
    __I __C  uint8_t  RESERVED31[2];
    __IO     uint16_t   LIM3_HI;                       /*!< Channel 3 High Limit */
    __I __C  uint8_t  RESERVED32[2];
    __IO     uint16_t   HYS3;                          /*!< Channel 3 Hysteresis */
    __I __C  uint8_t  RESERVED33[38];
    __IO     uint16_t   CFG1;                          /*!< Reference Buffer Low Power Mode */
    __I __C  uint8_t  RESERVED34[576];
} ADI_ADC_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_ADC_TypeDef__ */

/*!@}*/

/** @defgroup DMA DMA (DMA) Module
 *  DMA
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_DMA_TypeDef
 *  \brief  DMA
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_DMA_TypeDef__
typedef struct _ADI_DMA_TypeDef
{
    __I __C  uint32_t   STAT;                          /*!< DMA Status */
    __O      uint32_t   CFG;                           /*!< DMA Configuration */
    __IO     uint32_t   PDBPTR;                        /*!< DMA Channel Primary Control Database Pointer */
    __I __C  uint32_t   ADBPTR;                        /*!< DMA Channel Alternate Control Database Pointer */
    __I __C  uint8_t  RESERVED0[4];
    __O      uint32_t   SWREQ;                         /*!< DMA Channel Software Request */
    __I __C  uint8_t  RESERVED1[8];
    __IO     uint32_t   RMSK_SET;                      /*!< DMA Channel Request Mask Set */
    __O      uint32_t   RMSK_CLR;                      /*!< DMA Channel Request Mask Clear */
    __IO     uint32_t   EN_SET;                        /*!< DMA Channel Enable Set */
    __O      uint32_t   EN_CLR;                        /*!< DMA Channel Enable Clear */
    __IO     uint32_t   ALT_SET;                       /*!< DMA Channel Primary Alternate Set */
    __O      uint32_t   ALT_CLR;                       /*!< DMA Channel Primary Alternate Clear */
    __O      uint32_t   PRI_SET;                       /*!< DMA Channel Priority Set */
    __O      uint32_t   PRI_CLR;                       /*!< DMA Channel Priority Clear */
    __I __C  uint8_t  RESERVED2[8];
    __IO     uint32_t   ERRCHNL_CLR;                   /*!< DMA per Channel Error Clear */
    __IO     uint32_t   ERR_CLR;                       /*!< DMA Bus Error Clear */
    __IO     uint32_t   INVALIDDESC_CLR;               /*!< DMA per Channel Invalid Descriptor Clear */
    __I __C  uint8_t  RESERVED3[1964];
    __IO     uint32_t   BS_SET;                        /*!< DMA Channel Bytes Swap Enable Set */
    __O      uint32_t   BS_CLR;                        /*!< DMA Channel Bytes Swap Enable Clear */
    __I __C  uint8_t  RESERVED4[8];
    __IO     uint32_t   SRCADDR_SET;                   /*!< DMA Channel Source Address Decrement Enable Set */
    __O      uint32_t   SRCADDR_CLR;                   /*!< DMA Channel Source Address Decrement Enable Clear */
    __IO     uint32_t   DSTADDR_SET;                   /*!< DMA Channel Destination Address Decrement Enable Set */
    __O      uint32_t   DSTADDR_CLR;                   /*!< DMA Channel Destination Address Decrement Enable Clear */
    __I __C  uint8_t  RESERVED5[1984];
    __I __C  uint32_t   REVID;                         /*!< DMA Controller Revision ID */
} ADI_DMA_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_DMA_TypeDef__ */

/*!@}*/

/** @defgroup FLCC Flash Controller (FLCC) Module
 *  Flash Controller
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_FLCC_TypeDef
 *  \brief  Flash Controller
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_TypeDef__
typedef struct _ADI_FLCC_TypeDef
{
    __IO     uint32_t   STAT;                          /*!< Status */
    __IO     uint32_t   IEN;                           /*!< Interrupt Enable */
    __IO     uint32_t   CMD;                           /*!< Command */
    __IO     uint32_t   KH_ADDR;                       /*!< Write Address */
    __IO     uint32_t   KH_DATA0;                      /*!< Write Lower Data */
    __IO     uint32_t   KH_DATA1;                      /*!< Write Upper Data */
    __IO     uint32_t   PAGE_ADDR0;                    /*!< Lower Page Address */
    __IO     uint32_t   PAGE_ADDR1;                    /*!< Upper Page Address */
    __O      uint32_t   KEY;                           /*!< Key */
    __I __C  uint32_t   WR_ABORT_ADDR;                 /*!< Write Abort Address */
    __IO     uint32_t   WRPROT;                        /*!< Write Protection */
    __I __C  uint32_t   SIGNATURE;                     /*!< Signature */
    __IO     uint32_t   UCFG;                          /*!< User Configuration */
    __IO     uint32_t   TIME_PARAM0;                   /*!< Time Parameter 0 */
    __IO     uint32_t   TIME_PARAM1;                   /*!< Time Parameter 1 */
    __IO     uint32_t   ABORT_EN_LO;                   /*!< IRQ Abort Enable (Lower Bits) */
    __IO     uint32_t   ABORT_EN_HI;                   /*!< IRQ Abort Enable (Upper Bits) */
    __IO     uint32_t   ECC_CFG;                       /*!< ECC Configuration */
    __I __C  uint32_t   ECC_ADDR;                      /*!< ECC Status (Address) */
    __I __C  uint8_t  RESERVED0[4];
    __IO     uint32_t   POR_SEC;                       /*!< Flash Security */
    __IO     uint32_t   VOL_CFG;                       /*!< Volatile Flash Configuration */
} ADI_FLCC_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_TypeDef__ */

/*!@}*/

/** @defgroup FLCC_CACHE Cache Controller (FLCC_CACHE) Module
 *  Cache Controller
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_FLCC_CACHE_TypeDef
 *  \brief  Cache Controller
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_FLCC_CACHE_TypeDef__
typedef struct _ADI_FLCC_CACHE_TypeDef
{
    __I __C  uint32_t   STAT;                          /*!< Cache Status Register */
    __IO     uint32_t   SETUP;                         /*!< Cache Setup Register */
    __O      uint32_t   KEY;                           /*!< Cache Key Register */
    __I __C  uint8_t  RESERVED0[40];
} ADI_FLCC_CACHE_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_FLCC_CACHE_TypeDef__ */

/*!@}*/

/** @defgroup GPIO  (GPIO) Module
 *  
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_GPIO_TypeDef
 *  \brief  
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_GPIO_TypeDef__
typedef struct _ADI_GPIO_TypeDef
{
    __IO     uint32_t   CFG;                           /*!< Port  Configuration */
    __IO     uint16_t   OEN;                           /*!< Port Output Enable */
    __I __C  uint8_t  RESERVED0[2];
    __IO     uint16_t   PE;                            /*!< Port Output Pull-up/Pull-down Enable */
    __I __C  uint8_t  RESERVED1[2];
    __IO     uint16_t   IEN;                           /*!< Port  Input Path Enable */
    __I __C  uint8_t  RESERVED2[2];
    __I __C  uint16_t   IN;                            /*!< Port  Registered Data Input */
    __I __C  uint8_t  RESERVED3[2];
    __IO     uint16_t   OUT;                           /*!< Port Data Output */
    __I __C  uint8_t  RESERVED4[2];
    __O      uint16_t   SET;                           /*!< Port Data Out Set */
    __I __C  uint8_t  RESERVED5[2];
    __O      uint16_t   CLR;                           /*!< Port Data Out Clear */
    __I __C  uint8_t  RESERVED6[2];
    __O      uint16_t   TGL;                           /*!< Port Pin Toggle */
    __I __C  uint8_t  RESERVED7[2];
    __IO     uint16_t   POL;                           /*!< Port Interrupt Polarity */
    __I __C  uint8_t  RESERVED8[2];
    __IO     uint16_t   IENA;                          /*!< Port Interrupt A Enable */
    __I __C  uint8_t  RESERVED9[2];
    __IO     uint16_t   IENB;                          /*!< Port Interrupt B Enable */
    __I __C  uint8_t  RESERVED10[2];
    __IO     uint16_t   INT;                           /*!< Port Interrupt Status */
    __I __C  uint8_t  RESERVED11[2];
    __IO     uint16_t   DS;                            /*!< Port Drive Strength Select */
} ADI_GPIO_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_GPIO_TypeDef__ */

/*!@}*/

/** @defgroup SPORT Serial Port (SPORT) Module
 *  Serial Port
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_SPORT_TypeDef
 *  \brief  Serial Port
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_SPORT_TypeDef__
typedef struct _ADI_SPORT_TypeDef
{
    __IO     uint32_t   CTL_A;                         /*!< Half SPORT 'A' Control Register */
    __IO     uint32_t   DIV_A;                         /*!< Half SPORT 'A' Divisor Register */
    __IO     uint32_t   IEN_A;                         /*!< Half SPORT A's Interrupt Enable register */
    __IO     uint32_t   STAT_A;                        /*!< Half SPORT 'A' Status register */
    __IO     uint32_t   NUMTRAN_A;                     /*!< Half SPORT A Number of transfers register */
    __IO     uint32_t   CNVT_A;                        /*!< Half SPORT 'A' CNV width */
    __I __C  uint8_t  RESERVED0[8];
    __O      uint32_t   TX_A;                          /*!< Half SPORT 'A' Tx Buffer Register */
    __I __C  uint8_t  RESERVED1[4];
    __I __C  uint32_t   RX_A;                          /*!< Half SPORT 'A' Rx Buffer Register */
    __I __C  uint8_t  RESERVED2[20];
    __IO     uint32_t   CTL_B;                         /*!< Half SPORT 'B' Control Register */
    __IO     uint32_t   DIV_B;                         /*!< Half SPORT 'B' Divisor Register */
    __IO     uint32_t   IEN_B;                         /*!< Half SPORT B's Interrupt Enable register */
    __IO     uint32_t   STAT_B;                        /*!< Half SPORT 'B' Status register */
    __IO     uint32_t   NUMTRAN_B;                     /*!< Half SPORT B Number of transfers register */
    __IO     uint32_t   CNVT_B;                        /*!< Half SPORT 'B' CNV width register */
    __I __C  uint8_t  RESERVED3[8];
    __O      uint32_t   TX_B;                          /*!< Half SPORT 'B' Tx Buffer Register */
    __I __C  uint8_t  RESERVED4[4];
    __I __C  uint32_t   RX_B;                          /*!< Half SPORT 'B' Rx Buffer Register */
    __I __C  uint8_t  RESERVED5[16];
} ADI_SPORT_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_SPORT_TypeDef__ */

/*!@}*/

/** @defgroup CRC CRC Accelerator (CRC) Module
 *  CRC Accelerator
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_CRC_TypeDef
 *  \brief  CRC Accelerator
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRC_TypeDef__
typedef struct _ADI_CRC_TypeDef
{
    __IO     uint32_t   CTL;                           /*!< CRC Control */
    __O      uint32_t   IPDATA;                        /*!< Input Data Word */
    __IO     uint32_t   RESULT;                        /*!< CRC Result */
    __IO     uint32_t   POLY;                          /*!< Programmable CRC Polynomial */
    union {
    __O      uint8_t    IPBITS[8];                     /*!< Input Data Bits */
    __O      uint8_t    IPBYTE;                        /*!< Input Data Byte */
    };
} ADI_CRC_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRC_TypeDef__ */

/*!@}*/

/** @defgroup RNG Random Number Generator (RNG) Module
 *  Random Number Generator
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_RNG_TypeDef
 *  \brief  Random Number Generator
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_RNG_TypeDef__
typedef struct _ADI_RNG_TypeDef
{
    __IO     uint16_t   CTL;                           /*!< RNG Control Register */
    __I __C  uint8_t  RESERVED0[2];
    __IO     uint16_t   LEN;                           /*!< RNG Sample Length Register */
    __I __C  uint8_t  RESERVED1[2];
    __IO     uint16_t   STAT;                          /*!< RNG Status Register */
    __I __C  uint8_t  RESERVED2[2];
    __I __C  uint32_t   DATA;                          /*!< RNG Data Register */
    __I __C  uint32_t   OSCCNT;                        /*!< Oscillator Count */
    __I __C  int8_t     OSCDIFF[4];                    /*!< Oscillator Difference */
} ADI_RNG_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_RNG_TypeDef__ */

/*!@}*/

/** @defgroup CRYPT Register Map for the Crypto Block (CRYPT) Module
 *  Register Map for the Crypto Block
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_CRYPT_TypeDef
 *  \brief  Register Map for the Crypto Block
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CRYPT_TypeDef__
typedef struct _ADI_CRYPT_TypeDef
{
    __IO     uint32_t   CFG;                           /*!< Configuration Register */
    __IO     uint32_t   DATALEN;                       /*!< Payload Data Length */
    __IO     uint32_t   PREFIXLEN;                     /*!< Authentication Data Length */
    __IO     uint32_t   INTEN;                         /*!< Interrupt Enable Register */
    __IO     uint32_t   STAT;                          /*!< Status Register */
    __O      uint32_t   INBUF;                         /*!< Input Buffer */
    __I __C  uint32_t   OUTBUF;                        /*!< Output Buffer */
    __IO     uint32_t   NONCE0;                        /*!< Nonce Bits [31:0] */
    __IO     uint32_t   NONCE1;                        /*!< Nonce Bits [63:32] */
    __IO     uint32_t   NONCE2;                        /*!< Nonce Bits [95:64] */
    __IO     uint32_t   NONCE3;                        /*!< Nonce Bits [127:96] */
    __O      uint32_t   AESKEY0;                       /*!< AES Key Bits [31:0] */
    __O      uint32_t   AESKEY1;                       /*!< AES Key Bits [63:32] */
    __O      uint32_t   AESKEY2;                       /*!< AES Key Bits [95:64] */
    __O      uint32_t   AESKEY3;                       /*!< AES Key Bits [127:96] */
    __O      uint32_t   AESKEY4;                       /*!< AES Key Bits [159:128] */
    __O      uint32_t   AESKEY5;                       /*!< AES Key Bits [191:160] */
    __O      uint32_t   AESKEY6;                       /*!< AES Key Bits [223:192] */
    __O      uint32_t   AESKEY7;                       /*!< AES Key Bits [255:224] */
    __IO     uint32_t   CNTRINIT;                      /*!< Counter Initialization Vector */
    __IO     uint32_t   SHAH0;                         /*!< SHA Bits [31:0] */
    __IO     uint32_t   SHAH1;                         /*!< SHA Bits [63:32] */
    __IO     uint32_t   SHAH2;                         /*!< SHA Bits [95:64] */
    __IO     uint32_t   SHAH3;                         /*!< SHA Bits [127:96] */
    __IO     uint32_t   SHAH4;                         /*!< SHA Bits [159:128] */
    __IO     uint32_t   SHAH5;                         /*!< SHA Bits [191:160] */
    __IO     uint32_t   SHAH6;                         /*!< SHA Bits [223:192] */
    __IO     uint32_t   SHAH7;                         /*!< SHA Bits [255:224] */
    __IO     uint32_t   SHA_LAST_WORD;                 /*!< SHA Last Word and Valid Bits Information */
    __IO     uint32_t   CCM_NUM_VALID_BYTES;           /*!< NUM_VALID_BYTES */
    __IO     uint32_t   PRKSTORCFG;                    /*!< PRKSTOR Configuration */
    __I __C  uint8_t  RESERVED0[4];
    __O      uint32_t   KUW0;                          /*!< Key Wrap Unwrap Register 0 */
    __O      uint32_t   KUW1;                          /*!< Key Wrap Unwrap Register 1 */
    __O      uint32_t   KUW2;                          /*!< Key Wrap Unwrap Register 2 */
    __O      uint32_t   KUW3;                          /*!< Key Wrap Unwrap Register 3 */
    __O      uint32_t   KUW4;                          /*!< Key Wrap Unwrap Register 4 */
    __O      uint32_t   KUW5;                          /*!< Key Wrap Unwrap Register 5 */
    __O      uint32_t   KUW6;                          /*!< Key Wrap Unwrap Register 6 */
    __O      uint32_t   KUW7;                          /*!< Key Wrap Unwrap Register 7 */
    __O      uint32_t   KUW8;                          /*!< Key Wrap Unwrap Register 8 */
    __O      uint32_t   KUW9;                          /*!< Key Wrap Unwrap Register 9 */
    __O      uint32_t   KUW10;                         /*!< Key Wrap Unwrap Register 10 */
    __O      uint32_t   KUW11;                         /*!< Key Wrap Unwrap Register 11 */
    __O      uint32_t   KUW12;                         /*!< Key Wrap Unwrap Register 12 */
    __O      uint32_t   KUW13;                         /*!< Key Wrap Unwrap Register 13 */
    __O      uint32_t   KUW14;                         /*!< Key Wrap Unwrap Register 14 */
    __O      uint32_t   KUW15;                         /*!< Key Wrap Unwrap Register 15 */
    __O      uint32_t   KUWVALSTR1;                    /*!< Key Wrap Unwrap Validation String [63:32] */
    __O      uint32_t   KUWVALSTR2;                    /*!< Key Wrap Unwrap Validation String [31:0] */
} ADI_CRYPT_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CRYPT_TypeDef__ */

/*!@}*/

/** @defgroup PMG Power Management  (PMG) Module
 *  Power Management 
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_PMG_TypeDef
 *  \brief  Power Management 
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_TypeDef__
typedef struct _ADI_PMG_TypeDef
{
    __IO     uint32_t   IEN;                           /*!< Power Supply Monitor Interrupt Enable */
    __IO     uint32_t   PSM_STAT;                      /*!< Power Supply Monitor Status */
    __IO     uint32_t   PWRMOD;                        /*!< Power Mode Register */
    __O      uint32_t   PWRKEY;                        /*!< Key Protection for PWRMOD and  SRAMRET */
    __I __C  uint32_t   SHDN_STAT;                     /*!< Shutdown Status Register */
    __IO     uint32_t   SRAMRET;                       /*!< Control for Retention SRAM in Hibernate Mode */
    __I __C  uint8_t  RESERVED0[32];
    __IO     uint32_t   TRIM;                          /*!< Trimming Bits */
    __I __C  uint8_t  RESERVED1[4];
    __IO     uint32_t   RST_STAT;                      /*!< Reset Status */
    __IO     uint32_t   CTL1;                          /*!< HPBUCK Control */
    __I __C  uint8_t  RESERVED2[20];
} ADI_PMG_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_TypeDef__ */

/*!@}*/

/** @defgroup XINT External interrupt configuration (XINT) Module
 *  External interrupt configuration
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_XINT_TypeDef
 *  \brief  External interrupt configuration
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_XINT_TypeDef__
typedef struct _ADI_XINT_TypeDef
{
    __IO     uint32_t   CFG0;                          /*!< External Interrupt configuration */
    __I __C  uint32_t   EXT_STAT;                      /*!< External Wakeup Interrupt Status register */
    __I __C  uint8_t  RESERVED0[8];
    __IO     uint32_t   CLR;                           /*!< External Interrupt clear */
    __IO     uint32_t   NMICLR;                        /*!< Non-maskable interrupt clear */
} ADI_XINT_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_XINT_TypeDef__ */

/*!@}*/

/** @defgroup CLKG_OSC Clocking  (CLKG_OSC) Module
 *  Clocking 
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_CLKG_OSC_TypeDef
 *  \brief  Clocking 
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CLKG_OSC_TypeDef__
typedef struct _ADI_CLKG_OSC_TypeDef
{
    __I __C  uint8_t  RESERVED0[12];
    __O      uint32_t   KEY;                           /*!< Key Protection for OSCCTRL */
    __IO     uint32_t   CTL;                           /*!< Oscillator Control */
    __I __C  uint8_t  RESERVED1[8];
} ADI_CLKG_OSC_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CLKG_OSC_TypeDef__ */

/*!@}*/

/** @defgroup PMG_TST Power Management  (PMG_TST) Module
 *  Power Management 
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_PMG_TST_TypeDef
 *  \brief  Power Management 
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PMG_TST_TypeDef__
typedef struct _ADI_PMG_TST_TypeDef
{
    __I __C  uint8_t  RESERVED0[96];
    __IO     uint32_t   SRAM_CTL;                      /*!< Control for SRAM Parity and Instruction SRAM */
    __I __C  uint32_t   SRAM_INITSTAT;                 /*!< Initialization Status Register */
    __O      uint16_t   CLR_LATCH_GPIOS;               /*!< Clear GPIO After Shutdown Mode */
    __I __C  uint8_t  RESERVED1[2];
    __IO     uint32_t   SCRPAD_IMG;                    /*!< Scratch Pad Image */
    __I __C  uint32_t   SCRPAD_3V_RD;                  /*!< Scratch Pad Saved in Battery Domain */
    __IO     uint32_t   FAST_SHT_WAKEUP;               /*!< Fast Shutdown Wake-up Enable */
} ADI_PMG_TST_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PMG_TST_TypeDef__ */

/*!@}*/

/** @defgroup CLKG_CLK Clocking  (CLKG_CLK) Module
 *  Clocking 
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_CLKG_CLK_TypeDef
 *  \brief  Clocking 
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_CLKG_CLK_TypeDef__
typedef struct _ADI_CLKG_CLK_TypeDef
{
    __IO     uint32_t   CTL0;                          /*!< Misc Clock Settings */
    __IO     uint32_t   CTL1;                          /*!< Clock Dividers */
    __IO     uint32_t   CTL2;                          /*!< HF Oscillator Divided Clock Select */
    __IO     uint32_t   CTL3;                          /*!< System PLL */
    __I __C  uint8_t  RESERVED0[4];
    __IO     uint32_t   CTL5;                          /*!< User Clock Gating Control */
    __IO     uint32_t   STAT0;                         /*!< Clocking Status */
    __I __C  uint8_t  RESERVED1[20];
} ADI_CLKG_CLK_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_CLKG_CLK_TypeDef__ */

/*!@}*/

/** @defgroup BUSM Bus matrix (BUSM) Module
 *  Bus matrix
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_BUSM_TypeDef
 *  \brief  Bus matrix
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_BUSM_TypeDef__
typedef struct _ADI_BUSM_TypeDef
{
    __IO     uint32_t   ARBIT0;                        /*!< Arbitration Priority Configuration for FLASH and SRAM0 */
    __IO     uint32_t   ARBIT1;                        /*!< Arbitration Priority Configuration for SRAM1 and SIP */
    __IO     uint32_t   ARBIT2;                        /*!< Arbitration Priority Configuration for APB32 and APB16 */
    __IO     uint32_t   ARBIT3;                        /*!< Arbitration Priority Configuration for APB16 priority for core and for DMA1 */
    __I __C  uint8_t  RESERVED0[4];
    __IO     uint32_t   ARBIT4;                        /*!< Arbitration Priority Configuration for SRAM1 and SIP */
} ADI_BUSM_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_BUSM_TypeDef__ */

/*!@}*/

/** @defgroup PTI Parallel Test Interface (PTI) Module
 *  Parallel Test Interface
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_PTI_TypeDef
 *  \brief  Parallel Test Interface
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_PTI_TypeDef__
typedef struct _ADI_PTI_TypeDef
{
    __IO     uint32_t   RST_ISR_STARTADDR;             /*!< Reset ISR Start Address */
    __IO     uint32_t   RST_STACK_PTR;                 /*!< Reset Stack Pointer */
    __IO     uint32_t   CTL;                           /*!< Parallel Test Interface Control Register */
} ADI_PTI_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_PTI_TypeDef__ */

/*!@}*/

/** @defgroup NVIC Cortex-M3 Interrupt Controller (NVIC) Module
 *  Cortex-M3 Interrupt Controller
 *  @{
 */

/*! ==========================================================================
 *  \struct ADI_NVIC_TypeDef
 *  \brief  Cortex-M3 Interrupt Controller
 *  ========================================================================== */
#ifndef __ADI_NO_DECL_STRUCT_ADI_NVIC_TypeDef__
typedef struct _ADI_NVIC_TypeDef
{
    __I __C  uint8_t  RESERVED0[4];
    __IO     uint32_t   INTNUM;                        /*!< Interrupt Control Type */
    __I __C  uint8_t  RESERVED1[8];
    __IO     uint32_t   STKSTA;                        /*!< Systick Control and Status */
    __IO     uint32_t   STKLD;                         /*!< Systick Reload Value */
    __IO     uint32_t   STKVAL;                        /*!< Systick Current Value */
    __IO     uint32_t   STKCAL;                        /*!< Systick Calibration Value */
    __I __C  uint8_t  RESERVED2[224];
    __IO     uint32_t   INTSETE0;                      /*!< IRQ0..31 Set_Enable */
    __IO     uint32_t   INTSETE1;                      /*!< IRQ32..63 Set_Enable */
    __I __C  uint8_t  RESERVED3[120];
    __IO     uint32_t   INTCLRE0;                      /*!< IRQ0..31 Clear_Enable */
    __IO     uint32_t   INTCLRE1;                      /*!< IRQ32..63 Clear_Enable */
    __I __C  uint8_t  RESERVED4[120];
    __IO     uint32_t   INTSETP0;                      /*!< IRQ0..31 Set_Pending */
    __IO     uint32_t   INTSETP1;                      /*!< IRQ32..63 Set_Pending */
    __I __C  uint8_t  RESERVED5[120];
    __IO     uint32_t   INTCLRP0;                      /*!< IRQ0..31 Clear_Pending */
    __IO     uint32_t   INTCLRP1;                      /*!< IRQ32..63 Clear_Pending */
    __I __C  uint8_t  RESERVED6[120];
    __IO     uint32_t   INTACT0;                       /*!< IRQ0..31 Active Bit */
    __IO     uint32_t   INTACT1;                       /*!< IRQ32..63 Active Bit */
    __I __C  uint8_t  RESERVED7[248];
    __IO     uint32_t   INTPRI0;                       /*!< IRQ0..3 Priority */
    __IO     uint32_t   INTPRI1;                       /*!< IRQ4..7 Priority */
    __IO     uint32_t   INTPRI2;                       /*!< IRQ8..11 Priority */
    __IO     uint32_t   INTPRI3;                       /*!< IRQ12..15 Priority */
    __IO     uint32_t   INTPRI4;                       /*!< IRQ16..19 Priority */
    __IO     uint32_t   INTPRI5;                       /*!< IRQ20..23 Priority */
    __IO     uint32_t   INTPRI6;                       /*!< IRQ24..27 Priority */
    __IO     uint32_t   INTPRI7;                       /*!< IRQ28..31 Priority */
    __IO     uint32_t   INTPRI8;                       /*!< IRQ32..35 Priority */
    __IO     uint32_t   INTPRI9;                       /*!< IRQ36..39 Priority */
    __IO     uint32_t   INTPRI10;                      /*!< IRQ40..43 Priority */
    __I __C  uint8_t  RESERVED8[2260];
    __IO     uint32_t   INTCPID;                       /*!< CPUID Base */
    __IO     uint32_t   INTSTA;                        /*!< Interrupt Control State */
    __IO     uint32_t   INTVEC;                        /*!< Vector Table Offset */
    __IO     uint32_t   INTAIRC;                       /*!< Application Interrupt/Reset Control */
    __IO     uint16_t   INTCON0;                       /*!< System Control */
    __I __C  uint8_t  RESERVED9[2];
    __IO     uint32_t   INTCON1;                       /*!< Configuration Control */
    __IO     uint32_t   INTSHPRIO0;                    /*!< System Handlers 4-7 Priority */
    __IO     uint32_t   INTSHPRIO1;                    /*!< System Handlers 8-11 Priority */
    __IO     uint32_t   INTSHPRIO3;                    /*!< System Handlers 12-15 Priority */
    __IO     uint32_t   INTSHCSR;                      /*!< System Handler Control and State */
    __IO     uint32_t   INTCFSR;                       /*!< Configurable Fault Status */
    __IO     uint32_t   INTHFSR;                       /*!< Hard Fault Status */
    __IO     uint32_t   INTDFSR;                       /*!< Debug Fault Status */
    __IO     uint32_t   INTMMAR;                       /*!< Mem Manage Address */
    __IO     uint32_t   INTBFAR;                       /*!< Bus Fault Address */
    __IO     uint32_t   INTAFSR;                       /*!< Auxiliary Fault Status */
    __IO     uint32_t   INTPFR0;                       /*!< Processor Feature Register 0 */
    __IO     uint32_t   INTPFR1;                       /*!< Processor Feature Register 1 */
    __IO     uint32_t   INTDFR0;                       /*!< Debug Feature Register 0 */
    __IO     uint32_t   INTAFR0;                       /*!< Auxiliary Feature Register 0 */
    __IO     uint32_t   INTMMFR0;                      /*!< Memory Model Feature Register 0 */
    __IO     uint32_t   INTMMFR1;                      /*!< Memory Model Feature Register 1 */
    __IO     uint32_t   INTMMFR2;                      /*!< Memory Model Feature Register 2 */
    __IO     uint32_t   INTMMFR3;                      /*!< Memory Model Feature Register 3 */
    __IO     uint32_t   INTISAR0;                      /*!< ISA Feature Register 0 */
    __IO     uint32_t   INTISAR1;                      /*!< ISA Feature Register 1 */
    __IO     uint32_t   INTISAR2;                      /*!< ISA Feature Register 2 */
    __IO     uint32_t   INTISAR3;                      /*!< ISA Feature Register 3 */
    __IO     uint32_t   INTISAR4;                      /*!< ISA Feature Register 4 */
    __I __C  uint8_t  RESERVED10[396];
    __IO     uint32_t   INTTRGI;                       /*!< Software Trigger Interrupt Register */
    __I __C  uint8_t  RESERVED11[204];
    __IO     uint32_t   INTPID4;                       /*!< Peripheral Identification Register 4 */
    __IO     uint32_t   INTPID5;                       /*!< Peripheral Identification Register 5 */
    __IO     uint32_t   INTPID6;                       /*!< Peripheral Identification Register 6 */
    __IO     uint32_t   INTPID7;                       /*!< Peripheral Identification Register 7 */
    __IO     uint32_t   INTPID0;                       /*!< Peripheral Identification Bits7:0 */
    __IO     uint32_t   INTPID1;                       /*!< Peripheral Identification Bits15:8 */
    __IO     uint32_t   INTPID2;                       /*!< Peripheral Identification Bits16:23 */
    __IO     uint32_t   INTPID3;                       /*!< Peripheral Identification Bits24:31 */
    __IO     uint32_t   INTCID0;                       /*!< Component Identification Bits7:0 */
    __IO     uint32_t   INTCID1;                       /*!< Component Identification Bits15:8 */
    __IO     uint32_t   INTCID2;                       /*!< Component Identification Bits16:23 */
    __IO     uint32_t   INTCID3;                       /*!< Component Identification Bits24:31 */
} ADI_NVIC_TypeDef;
#endif /* !__ADI_NO_DECL_STRUCT_ADI_NVIC_TypeDef__ */

/*!@}*/

/* ******************************************************************************
 *    Peripheral Memory Map Declarations
 * *****************************************************************************/
/*!    @defgroup PMEMMAPDEC Peripheral Memory Map Declarations
 *     \addtogroup PMEMMAPDEC
 *     @{ */
#define ADI_TMR0_BASE                  0x40000000    /*!<  Base address of TMR0 */
#define ADI_TMR1_BASE                  0x40000400    /*!<  Base address of TMR1 */
#define ADI_TMR2_BASE                  0x40000800    /*!<  Base address of TMR2 */
#define ADI_TMR_RGB_BASE               0x40000c00    /*!<  Base address of TMR_RGB */
#define ADI_RTC0_BASE                  0x40001000    /*!<  Base address of RTC0 */
#define ADI_RTC1_BASE                  0x40001400    /*!<  Base address of RTC1 */
#define ADI_SYS_BASE                   0x40002000    /*!<  Base address of SYS */
#define ADI_WDT0_BASE                  0x40002c00    /*!<  Base address of WDT0 */
#define ADI_I2C0_BASE                  0x40003000    /*!<  Base address of I2C0 */
#define ADI_SPI0_BASE                  0x40004000    /*!<  Base address of SPI0 */
#define ADI_SPI1_BASE                  0x40004400    /*!<  Base address of SPI1 */
#define ADI_SPI2_BASE                  0x40024000    /*!<  Base address of SPI2 */
#define ADI_UART0_BASE                 0x40005000    /*!<  Base address of UART0 */
#define ADI_UART1_BASE                 0x40005400    /*!<  Base address of UART1 */
#define ADI_BEEP0_BASE                 0x40005c00    /*!<  Base address of BEEP0 */
#define ADI_ADC0_BASE                  0x40007000    /*!<  Base address of ADC0 */
#define ADI_DMA0_BASE                  0x40010000    /*!<  Base address of DMA0 */
#define ADI_FLCC0_BASE                 0x40018000    /*!<  Base address of FLCC0 */
#define ADI_FLCC0_CACHE_BASE           0x40018058    /*!<  Base address of FLCC0_CACHE */
#define ADI_GPIO0_BASE                 0x40020000    /*!<  Base address of GPIO0 */
#define ADI_GPIO1_BASE                 0x40020040    /*!<  Base address of GPIO1 */
#define ADI_GPIO2_BASE                 0x40020080    /*!<  Base address of GPIO2 */
#define ADI_GPIO3_BASE                 0x400200c0    /*!<  Base address of GPIO3 */
#define ADI_SPORT0_BASE                0x40038000    /*!<  Base address of SPORT0 */
#define ADI_CRC0_BASE                  0x40040000    /*!<  Base address of CRC0 */
#define ADI_RNG0_BASE                  0x40040400    /*!<  Base address of RNG0 */
#define ADI_CRYPT0_BASE                0x40044000    /*!<  Base address of CRYPT0 */
#define ADI_PMG0_BASE                  0x4004c000    /*!<  Base address of PMG0 */
#define ADI_XINT0_BASE                 0x4004c080    /*!<  Base address of XINT0 */
#define ADI_CLKG0_OSC_BASE             0x4004c100    /*!<  Base address of CLKG0_OSC */
#define ADI_PMG0_TST_BASE              0x4004c200    /*!<  Base address of PMG0_TST */
#define ADI_CLKG0_CLK_BASE             0x4004c300    /*!<  Base address of CLKG0_CLK */
#define ADI_BUSM0_BASE                 0x4004c800    /*!<  Base address of BUSM0 */
#define ADI_PTI0_BASE                  0x4004cd00    /*!<  Base address of PTI0 */
#define ADI_NVIC0_BASE                 0xe000e000    /*!<  Base address of NVIC0 */

/*!    @} */

/* ******************************************************************************
 *    Peripheral Pointer Declarations
 * *****************************************************************************/
/*!    @Defgroup Pptrdec Peripheral Pointer Declarations
 *     \Addtogroup Pptrdec
 *     @{ */
#define pADI_TMR0                      ((ADI_TMR_TypeDef      *) ADI_TMR0_BASE       ) /*!<  Pointer to General Purpose Timer (TMR0) */
#define pADI_TMR1                      ((ADI_TMR_TypeDef      *) ADI_TMR1_BASE       ) /*!<  Pointer to General Purpose Timer (TMR1) */
#define pADI_TMR2                      ((ADI_TMR_TypeDef      *) ADI_TMR2_BASE       ) /*!<  Pointer to General Purpose Timer (TMR2) */
#define pADI_TMR_RGB                   ((ADI_TMR_RGB_TypeDef  *) ADI_TMR_RGB_BASE    ) /*!<  Pointer to Timer_RGB with 3 PWM outputs (TMR_RGB) */
#define pADI_RTC0                      ((ADI_RTC_TypeDef      *) ADI_RTC0_BASE       ) /*!<  Pointer to Real-Time Clock (RTC0) */
#define pADI_RTC1                      ((ADI_RTC_TypeDef      *) ADI_RTC1_BASE       ) /*!<  Pointer to Real-Time Clock (RTC1) */
#define pADI_SYS                       ((ADI_SYS_TypeDef      *) ADI_SYS_BASE        ) /*!<  Pointer to System Identification and Debug Enable (SYS) */
#define pADI_WDT0                      ((ADI_WDT_TypeDef      *) ADI_WDT0_BASE       ) /*!<  Pointer to Watchdog Timer (WDT0) */
#define pADI_I2C0                      ((ADI_I2C_TypeDef      *) ADI_I2C0_BASE       ) /*!<  Pointer to I2C Master/Slave (I2C0) */
#define pADI_SPI0                      ((ADI_SPI_TypeDef      *) ADI_SPI0_BASE       ) /*!<  Pointer to Serial Peripheral Interface (SPI0) */
#define pADI_SPI1                      ((ADI_SPI_TypeDef      *) ADI_SPI1_BASE       ) /*!<  Pointer to Serial Peripheral Interface (SPI1) */
#define pADI_SPI2                      ((ADI_SPI_TypeDef      *) ADI_SPI2_BASE       ) /*!<  Pointer to Serial Peripheral Interface (SPI2) */
#define pADI_UART0                     ((ADI_UART_TypeDef     *) ADI_UART0_BASE      ) /*!<  Pointer to  (UART0) */
#define pADI_UART1                     ((ADI_UART_TypeDef     *) ADI_UART1_BASE      ) /*!<  Pointer to  (UART1) */
#define pADI_BEEP0                     ((ADI_BEEP_TypeDef     *) ADI_BEEP0_BASE      ) /*!<  Pointer to Beeper Driver (BEEP0) */
#define pADI_ADC0                      ((ADI_ADC_TypeDef      *) ADI_ADC0_BASE       ) /*!<  Pointer to  (ADC0) */
#define pADI_DMA0                      ((ADI_DMA_TypeDef      *) ADI_DMA0_BASE       ) /*!<  Pointer to DMA (DMA0) */
#define pADI_FLCC0                     ((ADI_FLCC_TypeDef     *) ADI_FLCC0_BASE      ) /*!<  Pointer to Flash Controller (FLCC0) */
#define pADI_FLCC0_CACHE               ((ADI_FLCC_CACHE_TypeDef *) ADI_FLCC0_CACHE_BASE) /*!<  Pointer to Cache Controller (FLCC0_CACHE) */
#define pADI_GPIO0                     ((ADI_GPIO_TypeDef     *) ADI_GPIO0_BASE      ) /*!<  Pointer to  (GPIO0) */
#define pADI_GPIO1                     ((ADI_GPIO_TypeDef     *) ADI_GPIO1_BASE      ) /*!<  Pointer to  (GPIO1) */
#define pADI_GPIO2                     ((ADI_GPIO_TypeDef     *) ADI_GPIO2_BASE      ) /*!<  Pointer to  (GPIO2) */
#define pADI_GPIO3                     ((ADI_GPIO_TypeDef     *) ADI_GPIO3_BASE      ) /*!<  Pointer to  (GPIO3) */
#define pADI_SPORT0                    ((ADI_SPORT_TypeDef    *) ADI_SPORT0_BASE     ) /*!<  Pointer to Serial Port (SPORT0) */
#define pADI_CRC0                      ((ADI_CRC_TypeDef      *) ADI_CRC0_BASE       ) /*!<  Pointer to CRC Accelerator (CRC0) */
#define pADI_RNG0                      ((ADI_RNG_TypeDef      *) ADI_RNG0_BASE       ) /*!<  Pointer to Random Number Generator (RNG0) */
#define pADI_CRYPT0                    ((ADI_CRYPT_TypeDef    *) ADI_CRYPT0_BASE     ) /*!<  Pointer to Register Map for the Crypto Block (CRYPT0) */
#define pADI_PMG0                      ((ADI_PMG_TypeDef      *) ADI_PMG0_BASE       ) /*!<  Pointer to Power Management  (PMG0) */
#define pADI_XINT0                     ((ADI_XINT_TypeDef     *) ADI_XINT0_BASE      ) /*!<  Pointer to External interrupt configuration (XINT0) */
#define pADI_CLKG0_OSC                 ((ADI_CLKG_OSC_TypeDef *) ADI_CLKG0_OSC_BASE  ) /*!<  Pointer to Clocking  (CLKG0_OSC) */
#define pADI_PMG0_TST                  ((ADI_PMG_TST_TypeDef  *) ADI_PMG0_TST_BASE   ) /*!<  Pointer to Power Management  (PMG0_TST) */
#define pADI_CLKG0_CLK                 ((ADI_CLKG_CLK_TypeDef *) ADI_CLKG0_CLK_BASE  ) /*!<  Pointer to Clocking  (CLKG0_CLK) */
#define pADI_BUSM0                     ((ADI_BUSM_TypeDef     *) ADI_BUSM0_BASE      ) /*!<  Pointer to Bus matrix (BUSM0) */
#define pADI_PTI0                      ((ADI_PTI_TypeDef      *) ADI_PTI0_BASE       ) /*!<  Pointer to Parallel Test Interface (PTI0) */
#define pADI_NVIC0                     ((ADI_NVIC_TypeDef     *) ADI_NVIC0_BASE      ) /*!<  Pointer to Cortex-M3 Interrupt Controller (NVIC0) */

/*!    @} */


/*  =========================================================================
 *! \enum   IRQn_Type
 *! \brief  Interrupt Number Assignments
 *  ========================================================================= */
#ifndef __ADI_NO_DECL_ENUM_IRQn_Type__

typedef enum
{
  RESET_IRQn                     =   -15,    /*!< Cortex-M4 Reset */
  NonMaskableInt_IRQn            =   -14,    /*!< Cortex-M4 Non-maskable Interrupt */
  HardFault_IRQn                 =   -13,    /*!< Cortex-M4 Hardware Fault */
  MemoryManagement_IRQn          =   -12,    /*!< Cortex-M4 Memory Management Interrupt */
  BusFault_IRQn                  =   -11,    /*!< Cortex-M4 Bus Fault */
  UsageFault_IRQn                =   -10,    /*!< Cortex-M4 Usage Fault */
  SVCall_IRQn                    =    -5,    /*!< Cortex-M4 SVCall Interrupt */
  DebugMonitor_IRQn              =    -4,    /*!< Cortex-M4 Debug Monitor */
  PendSV_IRQn                    =    -2,    /*!< Cortex-M4 PendSV Interrupt */
  SysTick_IRQn                   =    -1,    /*!< Cortex-M4 SysTick Interrupt */
  RTC1_EVT_IRQn                  =     0,    /*!< Event */
  XINT_EVT0_IRQn                 =     1,    /*!< External Wakeup Interrupt n */
  XINT_EVT1_IRQn                 =     2,    /*!< External Wakeup Interrupt n */
  XINT_EVT2_IRQn                 =     3,    /*!< External Wakeup Interrupt n */
  XINT_EVT3_IRQn                 =     4,    /*!< External Wakeup Interrupt n */
  WDT_EXP_IRQn                   =     5,    /*!< Expiration */
  PMG0_VREG_OVR_IRQn             =     6,    /*!< Voltage Regulator (VREG) Overvoltage */
  PMG0_BATT_RANGE_IRQn           =     7,    /*!< Battery Voltage (VBAT) Out of Range */
  RTC0_EVT_IRQn                  =     8,    /*!< Event */
  SYS_GPIO_INTA_IRQn             =     9,    /*!< GPIO Interrupt A */
  SYS_GPIO_INTB_IRQn             =    10,    /*!< GPIO Interrupt B */
  TMR0_EVT_IRQn                  =    11,    /*!< Event */
  TMR1_EVT_IRQn                  =    12,    /*!< Event */
  FLCC_EVT_IRQn                  =    13,    /*!< Event */
  UART0_EVT_IRQn                 =    14,    /*!< UART0 Event */
  SPI0_EVT_IRQn                  =    15,    /*!< Event */
  SPI2_EVT_IRQn                  =    16,    /*!< Event */
  I2C_SLV_EVT_IRQn               =    17,    /*!< Slave Event */
  I2C_MST_EVT_IRQn               =    18,    /*!< Master Event */
  DMA_CHAN_ERR_IRQn              =    19,    /*!< Channel Error */
  DMA0_CH0_DONE_IRQn             =    20,    /*!< Channel 0 Done */
  DMA0_CH1_DONE_IRQn             =    21,    /*!< Channel 1 Done */
  DMA0_CH2_DONE_IRQn             =    22,    /*!< Channel 2 Done */
  DMA0_CH3_DONE_IRQn             =    23,    /*!< Channel 3 Done */
  DMA0_CH4_DONE_IRQn             =    24,    /*!< Channel 4 Done */
  DMA0_CH5_DONE_IRQn             =    25,    /*!< Channel 5 Done */
  DMA0_CH6_DONE_IRQn             =    26,    /*!< Channel 6 Done */
  DMA0_CH7_DONE_IRQn             =    27,    /*!< Channel 7 Done */
  DMA0_CH8_DONE_IRQn             =    28,    /*!< Channel 8 Done */
  DMA0_CH9_DONE_IRQn             =    29,    /*!< Channel 9 Done */
  DMA0_CH10_DONE_IRQn            =    30,    /*!< Channel 10 Done */
  DMA0_CH11_DONE_IRQn            =    31,    /*!< Channel 11 Done */
  DMA0_CH12_DONE_IRQn            =    32,    /*!< Channel 12 Done */
  DMA0_CH13_DONE_IRQn            =    33,    /*!< Channel 13 Done */
  DMA0_CH14_DONE_IRQn            =    34,    /*!< Channel 14 Done */
  DMA0_CH15_DONE_IRQn            =    35,    /*!< Channel 15 Done */
  SPORT_A_EVT_IRQn               =    36,    /*!< Channel A Event */
  SPORT_B_EVT_IRQn               =    37,    /*!< Channel B Event */
  CRYPT_EVT_IRQn                 =    38,    /*!< Event */
  DMA0_CH24_DONE_IRQn            =    39,    /*!< Channel 24 Done */
  TMR2_EVT_IRQn                  =    40,    /*!< Event */
  CLKG_XTAL_OSC_EVT_IRQn         =    41,    /*!< Crystal Oscillator Event */
  SPI1_EVT_IRQn                  =    42,    /*!< Event */
  CLKG_PLL_EVT_IRQn              =    43,    /*!< PLL Event */
  RNG0_EVT_IRQn                  =    44,    /*!< Event */
  BEEP_EVT_IRQn                  =    45,    /*!< Event */
  ADC0_EVT_IRQn                  =    46,    /*!< Event */
  DMA0_CH16_DONE_IRQn            =    56,    /*!< Channel 16 Done */
  DMA0_CH17_DONE_IRQn            =    57,    /*!< Channel 17 Done */
  DMA0_CH18_DONE_IRQn            =    58,    /*!< Channel 18 Done */
  DMA0_CH19_DONE_IRQn            =    59,    /*!< Channel 19 Done */
  DMA0_CH20_DONE_IRQn            =    60,    /*!< Channel 20 Done */
  DMA0_CH21_DONE_IRQn            =    61,    /*!< Channel 21 Done */
  DMA0_CH22_DONE_IRQn            =    62,    /*!< Channel 22 Done */
  DMA0_CH23_DONE_IRQn            =    63,    /*!< Channel 23 Done */
  UART1_EVT_IRQn                 =    66,    /*!< Event */
  DMA0_CH25_DONE_IRQn            =    67,    /*!< Channel 25 Done */
  DMA0_CH26_DONE_IRQn            =    68,    /*!< Channel 26 Done */
  TMR_RGB_EVT_IRQn               =    69,    /*!< Event */
  CLKG_ROOTCLK_ERR_IRQn          =    71,    /*!< Root Clock Error */
} IRQn_Type;  /* typedef name for fixed interrupt numbers */
#endif /* !__ADI_NO_DECL_ENUM_IRQn_Type__ */



#if defined (_MISRA_RULES)
#pragma diag(pop)
#endif /* _MISRA_RULES */

#endif
