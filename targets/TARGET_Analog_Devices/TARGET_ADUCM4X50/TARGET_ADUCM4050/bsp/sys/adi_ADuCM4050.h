/* ================================================================================

     Project      :   ADuCM4050
     File         :   ADuCM4050.h
     Description  :   Register Definitions

     Date         :   Feb 7, 2017

     Copyright (c) 2014-2017 Analog Devices, Inc.  All Rights Reserved.
     This software is proprietary and confidential to Analog Devices, Inc. and
     its licensors.

     This file was auto-generated. Do not make local changes to this file.

   ================================================================================ */

#ifndef _DEF_ADUCM4050_H
#define _DEF_ADUCM4050_H

#if defined(_LANGUAGE_C) || (defined(__GNUC__) && !defined(__ASSEMBLER__))
#include <stdint.h>
#endif /* _LANGUAGE_C */


#if defined (_MISRA_RULES)
#pragma diag(push)
#pragma diag(suppress:misra_rule_5_1:"Allow names over 32 character limit")
#pragma diag(suppress:misra_rule_19_7:"ADI header allows function-like macros")
#pragma diag(suppress:misra_rule_19_13:"ADI headers can use the # and ## preprocessor operators")
#endif /* _MISRA_RULES */

/* _ADI_MSK_3 might be defined in wrapper includes - otherwise provide a default */
#if !defined(_ADI_MSK_3)
/* do not add casts to literal constants in assembly code */
#if defined(_LANGUAGE_ASM) || defined(__ASSEMBLER__)
/* Use unsuffixed literals for BITM macros */
#define _ADI_MSK_3( mask, smask, type ) (mask) 
#else
/* Use casted suffixed literals for BITM macros */
#define _ADI_MSK_3( mask, smask, type ) ((type)(smask))
#endif
#endif

#ifndef __ADI_GENERATED_DEF_HEADERS__
#define __ADI_GENERATED_DEF_HEADERS__    1
#endif

#define __ADI_HAS_ADC__            1
#define __ADI_HAS_BEEP__           1
#define __ADI_HAS_BUSM__           1
#define __ADI_HAS_CLKG_OSC__       1
#define __ADI_HAS_CLKG__           1
#define __ADI_HAS_CLKG_CLK__       1
#define __ADI_HAS_CRC__            1
#define __ADI_HAS_CRYPT__          1
#define __ADI_HAS_DMA__            1
#define __ADI_HAS_XINT__           1
#define __ADI_HAS_FLCC__           1
#define __ADI_HAS_FLCC_CACHE__     1
#define __ADI_HAS_FLCC_DFT__       1
#define __ADI_HAS_FLCC_TEST__      1
#define __ADI_HAS_GPIO__           1
#define __ADI_HAS_TMR__            1
#define __ADI_HAS_I2C__            1
#define __ADI_HAS_NVIC__           1
#define __ADI_HAS_PMG__            1
#define __ADI_HAS_PMG_TST__        1
#define __ADI_HAS_PTI__            1
#define __ADI_HAS_RNG__            1
#define __ADI_HAS_RTC__            1
#define __ADI_HAS_SPI__            1
#define __ADI_HAS_SPORT__          1
#define __ADI_HAS_SYS__            1
#define __ADI_HAS_TMR_RGB__        1
#define __ADI_HAS_UART__           1
#define __ADI_HAS_WDT__            1

/* ============================================================================================================================
        General Purpose Timer
   ============================================================================================================================ */

/* ============================================================================================================================
        TMR0
   ============================================================================================================================ */
#define REG_TMR0_LOAD                        0x40000000            /*  TMR0 16-bit Load Value */
#define REG_TMR0_CURCNT                      0x40000004            /*  TMR0 16-bit Timer Value */
#define REG_TMR0_CTL                         0x40000008            /*  TMR0 Control */
#define REG_TMR0_CLRINT                      0x4000000C            /*  TMR0 Clear Interrupt */
#define REG_TMR0_CAPTURE                     0x40000010            /*  TMR0 Capture */
#define REG_TMR0_ALOAD                       0x40000014            /*  TMR0 16-bit Load Value, Asynchronous */
#define REG_TMR0_ACURCNT                     0x40000018            /*  TMR0 16-bit Timer Value, Asynchronous */
#define REG_TMR0_STAT                        0x4000001C            /*  TMR0 Status */
#define REG_TMR0_PWMCTL                      0x40000020            /*  TMR0 PWM Control Register */
#define REG_TMR0_PWMMATCH                    0x40000024            /*  TMR0 PWM Match Value */
#define REG_TMR0_EVENTSELECT                 0x40000028            /*  TMR0 Timer Event Selection Register */

/* ============================================================================================================================
        TMR1
   ============================================================================================================================ */
#define REG_TMR1_LOAD                        0x40000400            /*  TMR1 16-bit Load Value */
#define REG_TMR1_CURCNT                      0x40000404            /*  TMR1 16-bit Timer Value */
#define REG_TMR1_CTL                         0x40000408            /*  TMR1 Control */
#define REG_TMR1_CLRINT                      0x4000040C            /*  TMR1 Clear Interrupt */
#define REG_TMR1_CAPTURE                     0x40000410            /*  TMR1 Capture */
#define REG_TMR1_ALOAD                       0x40000414            /*  TMR1 16-bit Load Value, Asynchronous */
#define REG_TMR1_ACURCNT                     0x40000418            /*  TMR1 16-bit Timer Value, Asynchronous */
#define REG_TMR1_STAT                        0x4000041C            /*  TMR1 Status */
#define REG_TMR1_PWMCTL                      0x40000420            /*  TMR1 PWM Control Register */
#define REG_TMR1_PWMMATCH                    0x40000424            /*  TMR1 PWM Match Value */
#define REG_TMR1_EVENTSELECT                 0x40000428            /*  TMR1 Timer Event Selection Register */

/* ============================================================================================================================
        TMR2
   ============================================================================================================================ */
#define REG_TMR2_LOAD                        0x40000800            /*  TMR2 16-bit Load Value */
#define REG_TMR2_CURCNT                      0x40000804            /*  TMR2 16-bit Timer Value */
#define REG_TMR2_CTL                         0x40000808            /*  TMR2 Control */
#define REG_TMR2_CLRINT                      0x4000080C            /*  TMR2 Clear Interrupt */
#define REG_TMR2_CAPTURE                     0x40000810            /*  TMR2 Capture */
#define REG_TMR2_ALOAD                       0x40000814            /*  TMR2 16-bit Load Value, Asynchronous */
#define REG_TMR2_ACURCNT                     0x40000818            /*  TMR2 16-bit Timer Value, Asynchronous */
#define REG_TMR2_STAT                        0x4000081C            /*  TMR2 Status */
#define REG_TMR2_PWMCTL                      0x40000820            /*  TMR2 PWM Control Register */
#define REG_TMR2_PWMMATCH                    0x40000824            /*  TMR2 PWM Match Value */
#define REG_TMR2_EVENTSELECT                 0x40000828            /*  TMR2 Timer Event Selection Register */

/* ============================================================================================================================
        TMR Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          TMR_LOAD                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_LOAD_VALUE                   0            /*  Load Value */
#define BITM_TMR_LOAD_VALUE                  (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Load Value */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_CURCNT                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_CURCNT_VALUE                 0            /*  Current Count */
#define BITM_TMR_CURCNT_VALUE                (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Current Count */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_CTL                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_CTL_SYNCBYP                 15            /*  Synchronization Bypass */
#define BITP_TMR_CTL_RSTEN                   14            /*  Counter and Prescale Reset Enable */
#define BITP_TMR_CTL_EVTEN                   13            /*  Event Select */
#define BITP_TMR_CTL_RLD                      7            /*  Reload Control */
#define BITP_TMR_CTL_CLK                      5            /*  Clock Select */
#define BITP_TMR_CTL_EN                       4            /*  Timer Enable */
#define BITP_TMR_CTL_MODE                     3            /*  Timer Mode */
#define BITP_TMR_CTL_UP                       2            /*  Count up */
#define BITP_TMR_CTL_PRE                      0            /*  Prescaler */
#define BITM_TMR_CTL_SYNCBYP                 (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  Synchronization Bypass */
#define BITM_TMR_CTL_RSTEN                   (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  Counter and Prescale Reset Enable */
#define BITM_TMR_CTL_EVTEN                   (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  Event Select */
#define BITM_TMR_CTL_RLD                     (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  Reload Control */
#define BITM_TMR_CTL_CLK                     (_ADI_MSK_3(0x00000060,0x00000060U, uint16_t  ))    /*  Clock Select */
#define BITM_TMR_CTL_EN                      (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Timer Enable */
#define BITM_TMR_CTL_MODE                    (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Timer Mode */
#define BITM_TMR_CTL_UP                      (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Count up */
#define BITM_TMR_CTL_PRE                     (_ADI_MSK_3(0x00000003,0x00000003U, uint16_t  ))    /*  Prescaler */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_CLRINT                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_CLRINT_EVTCAPT               1            /*  Clear Captured Event Interrupt */
#define BITP_TMR_CLRINT_TIMEOUT               0            /*  Clear Timeout Interrupt */
#define BITM_TMR_CLRINT_EVTCAPT              (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Clear Captured Event Interrupt */
#define BITM_TMR_CLRINT_TIMEOUT              (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Clear Timeout Interrupt */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_CAPTURE                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_CAPTURE_VALUE                0            /*  16-bit Captured Value */
#define BITM_TMR_CAPTURE_VALUE               (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  16-bit Captured Value */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_ALOAD                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_ALOAD_VALUE                  0            /*  Load Value, Asynchronous */
#define BITM_TMR_ALOAD_VALUE                 (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Load Value, Asynchronous */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_ACURCNT                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_ACURCNT_VALUE                0            /*  Counter Value */
#define BITM_TMR_ACURCNT_VALUE               (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Counter Value */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_STAT                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_STAT_CNTRST                  8            /*  Counter Reset Occurring */
#define BITP_TMR_STAT_PDOK                    7            /*  Clear Interrupt Register Synchronization */
#define BITP_TMR_STAT_BUSY                    6            /*  Timer Busy */
#define BITP_TMR_STAT_CAPTURE                 1            /*  Capture Event Pending */
#define BITP_TMR_STAT_TIMEOUT                 0            /*  Timeout Event Occurred */
#define BITM_TMR_STAT_CNTRST                 (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Counter Reset Occurring */
#define BITM_TMR_STAT_PDOK                   (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  Clear Interrupt Register Synchronization */
#define BITM_TMR_STAT_BUSY                   (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  Timer Busy */
#define BITM_TMR_STAT_CAPTURE                (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Capture Event Pending */
#define BITM_TMR_STAT_TIMEOUT                (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Timeout Event Occurred */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_PWMCTL                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_PWMCTL_IDLESTATE             1            /*  PWM Idle State */
#define BITP_TMR_PWMCTL_MATCH                 0            /*  PWM Match Enabled */
#define BITM_TMR_PWMCTL_IDLESTATE            (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  PWM Idle State */
#define BITM_TMR_PWMCTL_MATCH                (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  PWM Match Enabled */
#define ENUM_TMR_PWMCTL_IDLE_LOW             (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  IDLESTATE: PWM idles low */
#define ENUM_TMR_PWMCTL_IDLE_HIGH            (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  IDLESTATE: PWM idles high */
#define ENUM_TMR_PWMCTL_PWM_TOGGLE           (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  MATCH: PWM in toggle mode */
#define ENUM_TMR_PWMCTL_PWM_MATCH            (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  MATCH: PWM in match mode */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_PWMMATCH                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_PWMMATCH_VALUE               0            /*  PWM Match Value */
#define BITM_TMR_PWMMATCH_VALUE              (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  PWM Match Value */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_EVENTSELECT                      Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_EVENTSELECT_EVTRANGE         0            /*  Event Select Range */
#define BITM_TMR_EVENTSELECT_EVTRANGE        (_ADI_MSK_3(0x0000003F,0x0000003FU, uint16_t  ))    /*  Event Select Range */


/* ============================================================================================================================
        Timer_RGB with 3 PWM outputs
   ============================================================================================================================ */

/* ============================================================================================================================
        TMR_RGB
   ============================================================================================================================ */
#define REG_TMR_RGB_LOAD                     0x40000C00            /*  TMR_RGB 16-bit load value */
#define REG_TMR_RGB_CURCNT                   0x40000C04            /*  TMR_RGB 16-bit timer value */
#define REG_TMR_RGB_CTL                      0x40000C08            /*  TMR_RGB Control */
#define REG_TMR_RGB_CLRINT                   0x40000C0C            /*  TMR_RGB Clear interrupt */
#define REG_TMR_RGB_CAPTURE                  0x40000C10            /*  TMR_RGB Capture */
#define REG_TMR_RGB_ALOAD                    0x40000C14            /*  TMR_RGB 16-bit load value, asynchronous */
#define REG_TMR_RGB_ACURCNT                  0x40000C18            /*  TMR_RGB 16-bit timer value, asynchronous */
#define REG_TMR_RGB_STAT                     0x40000C1C            /*  TMR_RGB Status */
#define REG_TMR_RGB_PWM0CTL                  0x40000C20            /*  TMR_RGB PWM0 Control Register */
#define REG_TMR_RGB_PWM0MATCH                0x40000C24            /*  TMR_RGB PWM0 Match Value */
#define REG_TMR_RGB_EVENTSELECT              0x40000C28            /*  TMR_RGB Timer Event selection Register */
#define REG_TMR_RGB_PWM1CTL                  0x40000C2C            /*  TMR_RGB PWM1 Control Register */
#define REG_TMR_RGB_PWM1MATCH                0x40000C30            /*  TMR_RGB PWM1 Match Value */
#define REG_TMR_RGB_PWM2CTL                  0x40000C34            /*  TMR_RGB PWM2 Control Register */
#define REG_TMR_RGB_PWM2MATCH                0x40000C38            /*  TMR_RGB PWM2 Match Value */

/* ============================================================================================================================
        TMR_RGB Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          TMR_RGB_LOAD                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_RGB_LOAD_VALUE               0            /*  Load value */
#define BITM_TMR_RGB_LOAD_VALUE              (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Load value */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_RGB_CURCNT                       Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_RGB_CURCNT_VALUE             0            /*  Current count */
#define BITM_TMR_RGB_CURCNT_VALUE            (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Current count */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_RGB_CTL                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_RGB_CTL_SYNCBYP             15            /*  Synchronization bypass */
#define BITP_TMR_RGB_CTL_RSTEN               14            /*  Counter and prescale reset enable */
#define BITP_TMR_RGB_CTL_EVTEN               13            /*  Event select */
#define BITP_TMR_RGB_CTL_RLD                  7            /*  Reload control */
#define BITP_TMR_RGB_CTL_CLK                  5            /*  Clock select */
#define BITP_TMR_RGB_CTL_EN                   4            /*  Timer enable */
#define BITP_TMR_RGB_CTL_MODE                 3            /*  Timer mode */
#define BITP_TMR_RGB_CTL_UP                   2            /*  Count up */
#define BITP_TMR_RGB_CTL_PRE                  0            /*  Prescaler */
#define BITM_TMR_RGB_CTL_SYNCBYP             (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  Synchronization bypass */
#define BITM_TMR_RGB_CTL_RSTEN               (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  Counter and prescale reset enable */
#define BITM_TMR_RGB_CTL_EVTEN               (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  Event select */
#define BITM_TMR_RGB_CTL_RLD                 (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  Reload control */
#define BITM_TMR_RGB_CTL_CLK                 (_ADI_MSK_3(0x00000060,0x00000060U, uint16_t  ))    /*  Clock select */
#define BITM_TMR_RGB_CTL_EN                  (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Timer enable */
#define BITM_TMR_RGB_CTL_MODE                (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Timer mode */
#define BITM_TMR_RGB_CTL_UP                  (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Count up */
#define BITM_TMR_RGB_CTL_PRE                 (_ADI_MSK_3(0x00000003,0x00000003U, uint16_t  ))    /*  Prescaler */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_RGB_CLRINT                       Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_RGB_CLRINT_EVTCAPT           1            /*  Clear captured event interrupt */
#define BITP_TMR_RGB_CLRINT_TIMEOUT           0            /*  Clear timeout interrupt */
#define BITM_TMR_RGB_CLRINT_EVTCAPT          (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Clear captured event interrupt */
#define BITM_TMR_RGB_CLRINT_TIMEOUT          (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Clear timeout interrupt */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_RGB_CAPTURE                      Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_RGB_CAPTURE_VALUE            0            /*  16-bit captured value */
#define BITM_TMR_RGB_CAPTURE_VALUE           (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  16-bit captured value */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_RGB_ALOAD                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_RGB_ALOAD_VALUE              0            /*  Load value, asynchronous */
#define BITM_TMR_RGB_ALOAD_VALUE             (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Load value, asynchronous */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_RGB_ACURCNT                      Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_RGB_ACURCNT_VALUE            0            /*  Counter value */
#define BITM_TMR_RGB_ACURCNT_VALUE           (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Counter value */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_RGB_STAT                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_RGB_STAT_CNTRST              8            /*  Counter reset occurring */
#define BITP_TMR_RGB_STAT_PDOK                7            /*  Clear Interrupt Register synchronization */
#define BITP_TMR_RGB_STAT_BUSY                6            /*  Timer Busy */
#define BITP_TMR_RGB_STAT_CAPTURE             1            /*  Capture event pending */
#define BITP_TMR_RGB_STAT_TIMEOUT             0            /*  Timeout event occurred */
#define BITM_TMR_RGB_STAT_CNTRST             (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Counter reset occurring */
#define BITM_TMR_RGB_STAT_PDOK               (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  Clear Interrupt Register synchronization */
#define BITM_TMR_RGB_STAT_BUSY               (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  Timer Busy */
#define BITM_TMR_RGB_STAT_CAPTURE            (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Capture event pending */
#define BITM_TMR_RGB_STAT_TIMEOUT            (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Timeout event occurred */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_RGB_PWM0CTL                      Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_RGB_PWM0CTL_IDLESTATE        1            /*  PWM Idle State */
#define BITP_TMR_RGB_PWM0CTL_MATCH            0            /*  PWM Match enabled */
#define BITM_TMR_RGB_PWM0CTL_IDLESTATE       (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  PWM Idle State */
#define BITM_TMR_RGB_PWM0CTL_MATCH           (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  PWM Match enabled */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_RGB_PWM0MATCH                    Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_RGB_PWM0MATCH_VALUE          0            /*  PWM Match Value */
#define BITM_TMR_RGB_PWM0MATCH_VALUE         (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  PWM Match Value */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_RGB_EVENTSELECT                  Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_RGB_EVENTSELECT_EVTRANGE     0            /*  Event select range */
#define BITM_TMR_RGB_EVENTSELECT_EVTRANGE    (_ADI_MSK_3(0x0000003F,0x0000003FU, uint16_t  ))    /*  Event select range */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_RGB_PWM1CTL                      Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_RGB_PWM1CTL_IDLESTATE        1            /*  PWM Idle State */
#define BITP_TMR_RGB_PWM1CTL_MATCH            0            /*  PWM Match enabled */
#define BITM_TMR_RGB_PWM1CTL_IDLESTATE       (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  PWM Idle State */
#define BITM_TMR_RGB_PWM1CTL_MATCH           (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  PWM Match enabled */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_RGB_PWM1MATCH                    Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_RGB_PWM1MATCH_VALUE          0            /*  PWM Match Value */
#define BITM_TMR_RGB_PWM1MATCH_VALUE         (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  PWM Match Value */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_RGB_PWM2CTL                      Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_RGB_PWM2CTL_IDLESTATE        1            /*  PWM Idle State */
#define BITP_TMR_RGB_PWM2CTL_MATCH            0            /*  PWM Match enabled */
#define BITM_TMR_RGB_PWM2CTL_IDLESTATE       (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  PWM Idle State */
#define BITM_TMR_RGB_PWM2CTL_MATCH           (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  PWM Match enabled */

/* -------------------------------------------------------------------------------------------------------------------------
          TMR_RGB_PWM2MATCH                    Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_TMR_RGB_PWM2MATCH_VALUE          0            /*  PWM Match Value */
#define BITM_TMR_RGB_PWM2MATCH_VALUE         (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  PWM Match Value */


/* ============================================================================================================================
        Real-Time Clock
   ============================================================================================================================ */

/* ============================================================================================================================
        RTC0
   ============================================================================================================================ */
#define REG_RTC0_CR0                         0x40001000            /*  RTC0 RTC Control 0 */
#define REG_RTC0_SR0                         0x40001004            /*  RTC0 RTC Status 0 */
#define REG_RTC0_SR1                         0x40001008            /*  RTC0 RTC Status 1 */
#define REG_RTC0_CNT0                        0x4000100C            /*  RTC0 RTC Count 0 */
#define REG_RTC0_CNT1                        0x40001010            /*  RTC0 RTC Count 1 */
#define REG_RTC0_ALM0                        0x40001014            /*  RTC0 RTC Alarm 0 */
#define REG_RTC0_ALM1                        0x40001018            /*  RTC0 RTC Alarm 1 */
#define REG_RTC0_TRM                         0x4000101C            /*  RTC0 RTC Trim */
#define REG_RTC0_GWY                         0x40001020            /*  RTC0 RTC Gateway */
#define REG_RTC0_CR1                         0x40001028            /*  RTC0 RTC Control 1 */
#define REG_RTC0_SR2                         0x4000102C            /*  RTC0 RTC Status 2 */
#define REG_RTC0_SNAP0                       0x40001030            /*  RTC0 RTC Snapshot 0 */
#define REG_RTC0_SNAP1                       0x40001034            /*  RTC0 RTC Snapshot 1 */
#define REG_RTC0_SNAP2                       0x40001038            /*  RTC0 RTC Snapshot 2 */
#define REG_RTC0_MOD                         0x4000103C            /*  RTC0 RTC Modulo */
#define REG_RTC0_CNT2                        0x40001040            /*  RTC0 RTC Count 2 */
#define REG_RTC0_ALM2                        0x40001044            /*  RTC0 RTC Alarm 2 */
#define REG_RTC0_SR3                         0x40001048            /*  RTC0 RTC Status 3 */
#define REG_RTC0_CR2IC                       0x4000104C            /*  RTC0 RTC Control 2 for Configuring Input Capture Channels */
#define REG_RTC0_CR3SS                       0x40001050            /*  RTC0 RTC Control 3 for Configuring SensorStrobe Channel */
#define REG_RTC0_CR4SS                       0x40001054            /*  RTC0 RTC Control 4 for Configuring SensorStrobe Channel */
#define REG_RTC0_SSMSK                       0x40001058            /*  RTC0 RTC Mask for SensorStrobe Channel */
#define REG_RTC0_IC2                         0x40001064            /*  RTC0 RTC Input Capture Channel 2 */
#define REG_RTC0_IC3                         0x40001068            /*  RTC0 RTC Input Capture Channel 3 */
#define REG_RTC0_IC4                         0x4000106C            /*  RTC0 RTC Input Capture Channel 4 */
#define REG_RTC0_SS1                         0x40001070            /*  RTC0 RTC SensorStrobe Channel 1 */
#define REG_RTC0_SS2                         0x40001074            /*  RTC0 RTC SensorStrobe Channel 2 */
#define REG_RTC0_SS3                         0x40001078            /*  RTC0 RTC SensorStrobe Channel 3 */
#define REG_RTC0_SS4                         0x4000107C            /*  RTC0 RTC SensorStrobe Channel 4 */
#define REG_RTC0_SR4                         0x40001080            /*  RTC0 RTC Status 4 */
#define REG_RTC0_SR5                         0x40001084            /*  RTC0 RTC Status 5 */
#define REG_RTC0_SR6                         0x40001088            /*  RTC0 RTC Status 6 */
#define REG_RTC0_SS1TGT                      0x4000108C            /*  RTC0 RTC SensorStrobe Channel 1 Target */
#define REG_RTC0_FRZCNT                      0x40001090            /*  RTC0 RTC Freeze Count */
#define REG_RTC0_SS2TGT                      0x40001094            /*  RTC0 RTC SensorStrobe Channel 2 Target */
#define REG_RTC0_SS3TGT                      0x40001098            /*  RTC0 RTC SensorStrobe Channel 3 Target */
#define REG_RTC0_SS1LOWDUR                   0x400010A0            /*  RTC0 RTC Auto-Reload Low Duration for SensorStrobe Channel 1 */
#define REG_RTC0_SS2LOWDUR                   0x400010A4            /*  RTC0 RTC Auto-Reload Low Duration for SensorStrobe Channel 2 */
#define REG_RTC0_SS3LOWDUR                   0x400010A8            /*  RTC0 RTC Auto-Reload Low Duration for SensorStrobe Channel 3 */
#define REG_RTC0_SS1HIGHDUR                  0x400010B0            /*  RTC0 RTC Auto-Reload High Duration for SensorStrobe Channel 1 */
#define REG_RTC0_SS2HIGHDUR                  0x400010B4            /*  RTC0 RTC Auto-Reload High Duration for SensorStrobe Channel 2 */
#define REG_RTC0_SS3HIGHDUR                  0x400010B8            /*  RTC0 RTC Auto-Reload High Duration for SensorStrobe Channel 3 */
#define REG_RTC0_SSMSKOT                     0x400010C0            /*  RTC0 RTC Masks for SensorStrobe Channels on Time Control */
#define REG_RTC0_CR5SSS                      0x400010C4            /*  RTC0 RTC Control 5 for Configuring SensorStrobe Channel GPIO Sampling */
#define REG_RTC0_CR6SSS                      0x400010C8            /*  RTC0 RTC Control 6 for Configuring SensorStrobe Channel GPIO Sampling Edge */
#define REG_RTC0_CR7SSS                      0x400010CC            /*  RTC0 RTC Control 7 for Configuring SensorStrobe Channel GPIO Sampling Activity */
#define REG_RTC0_SR7                         0x400010D0            /*  RTC0 RTC Status 7 */
#define REG_RTC0_SR8                         0x400010D4            /*  RTC0 RTC Status 8 */
#define REG_RTC0_SR9                         0x400010D8            /*  RTC0 RTC Status 9 */
#define REG_RTC0_GPMUX0                      0x400010E0            /*  RTC0 RTC GPIO Pin Mux Control Register 0 */
#define REG_RTC0_GPMUX1                      0x400010E4            /*  RTC0 RTC GPIO Pin Mux Control Register 1 */

/* ============================================================================================================================
        RTC1
   ============================================================================================================================ */
#define REG_RTC1_CR0                         0x40001400            /*  RTC1 RTC Control 0 */
#define REG_RTC1_SR0                         0x40001404            /*  RTC1 RTC Status 0 */
#define REG_RTC1_SR1                         0x40001408            /*  RTC1 RTC Status 1 */
#define REG_RTC1_CNT0                        0x4000140C            /*  RTC1 RTC Count 0 */
#define REG_RTC1_CNT1                        0x40001410            /*  RTC1 RTC Count 1 */
#define REG_RTC1_ALM0                        0x40001414            /*  RTC1 RTC Alarm 0 */
#define REG_RTC1_ALM1                        0x40001418            /*  RTC1 RTC Alarm 1 */
#define REG_RTC1_TRM                         0x4000141C            /*  RTC1 RTC Trim */
#define REG_RTC1_GWY                         0x40001420            /*  RTC1 RTC Gateway */
#define REG_RTC1_CR1                         0x40001428            /*  RTC1 RTC Control 1 */
#define REG_RTC1_SR2                         0x4000142C            /*  RTC1 RTC Status 2 */
#define REG_RTC1_SNAP0                       0x40001430            /*  RTC1 RTC Snapshot 0 */
#define REG_RTC1_SNAP1                       0x40001434            /*  RTC1 RTC Snapshot 1 */
#define REG_RTC1_SNAP2                       0x40001438            /*  RTC1 RTC Snapshot 2 */
#define REG_RTC1_MOD                         0x4000143C            /*  RTC1 RTC Modulo */
#define REG_RTC1_CNT2                        0x40001440            /*  RTC1 RTC Count 2 */
#define REG_RTC1_ALM2                        0x40001444            /*  RTC1 RTC Alarm 2 */
#define REG_RTC1_SR3                         0x40001448            /*  RTC1 RTC Status 3 */
#define REG_RTC1_CR2IC                       0x4000144C            /*  RTC1 RTC Control 2 for Configuring Input Capture Channels */
#define REG_RTC1_CR3SS                       0x40001450            /*  RTC1 RTC Control 3 for Configuring SensorStrobe Channel */
#define REG_RTC1_CR4SS                       0x40001454            /*  RTC1 RTC Control 4 for Configuring SensorStrobe Channel */
#define REG_RTC1_SSMSK                       0x40001458            /*  RTC1 RTC Mask for SensorStrobe Channel */
#define REG_RTC1_IC2                         0x40001464            /*  RTC1 RTC Input Capture Channel 2 */
#define REG_RTC1_IC3                         0x40001468            /*  RTC1 RTC Input Capture Channel 3 */
#define REG_RTC1_IC4                         0x4000146C            /*  RTC1 RTC Input Capture Channel 4 */
#define REG_RTC1_SS1                         0x40001470            /*  RTC1 RTC SensorStrobe Channel 1 */
#define REG_RTC1_SS2                         0x40001474            /*  RTC1 RTC SensorStrobe Channel 2 */
#define REG_RTC1_SS3                         0x40001478            /*  RTC1 RTC SensorStrobe Channel 3 */
#define REG_RTC1_SS4                         0x4000147C            /*  RTC1 RTC SensorStrobe Channel 4 */
#define REG_RTC1_SR4                         0x40001480            /*  RTC1 RTC Status 4 */
#define REG_RTC1_SR5                         0x40001484            /*  RTC1 RTC Status 5 */
#define REG_RTC1_SR6                         0x40001488            /*  RTC1 RTC Status 6 */
#define REG_RTC1_SS1TGT                      0x4000148C            /*  RTC1 RTC SensorStrobe Channel 1 Target */
#define REG_RTC1_FRZCNT                      0x40001490            /*  RTC1 RTC Freeze Count */
#define REG_RTC1_SS2TGT                      0x40001494            /*  RTC1 RTC SensorStrobe Channel 2 Target */
#define REG_RTC1_SS3TGT                      0x40001498            /*  RTC1 RTC SensorStrobe Channel 3 Target */
#define REG_RTC1_SS1LOWDUR                   0x400014A0            /*  RTC1 RTC Auto-Reload Low Duration for SensorStrobe Channel 1 */
#define REG_RTC1_SS2LOWDUR                   0x400014A4            /*  RTC1 RTC Auto-Reload Low Duration for SensorStrobe Channel 2 */
#define REG_RTC1_SS3LOWDUR                   0x400014A8            /*  RTC1 RTC Auto-Reload Low Duration for SensorStrobe Channel 3 */
#define REG_RTC1_SS1HIGHDUR                  0x400014B0            /*  RTC1 RTC Auto-Reload High Duration for SensorStrobe Channel 1 */
#define REG_RTC1_SS2HIGHDUR                  0x400014B4            /*  RTC1 RTC Auto-Reload High Duration for SensorStrobe Channel 2 */
#define REG_RTC1_SS3HIGHDUR                  0x400014B8            /*  RTC1 RTC Auto-Reload High Duration for SensorStrobe Channel 3 */
#define REG_RTC1_SSMSKOT                     0x400014C0            /*  RTC1 RTC Masks for SensorStrobe Channels on Time Control */
#define REG_RTC1_CR5SSS                      0x400014C4            /*  RTC1 RTC Control 5 for Configuring SensorStrobe Channel GPIO Sampling */
#define REG_RTC1_CR6SSS                      0x400014C8            /*  RTC1 RTC Control 6 for Configuring SensorStrobe Channel GPIO Sampling Edge */
#define REG_RTC1_CR7SSS                      0x400014CC            /*  RTC1 RTC Control 7 for Configuring SensorStrobe Channel GPIO Sampling Activity */
#define REG_RTC1_SR7                         0x400014D0            /*  RTC1 RTC Status 7 */
#define REG_RTC1_SR8                         0x400014D4            /*  RTC1 RTC Status 8 */
#define REG_RTC1_SR9                         0x400014D8            /*  RTC1 RTC Status 9 */
#define REG_RTC1_GPMUX0                      0x400014E0            /*  RTC1 RTC GPIO Pin Mux Control Register 0 */
#define REG_RTC1_GPMUX1                      0x400014E4            /*  RTC1 RTC GPIO Pin Mux Control Register 1 */

/* ============================================================================================================================
        RTC Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          RTC_CR0                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_CR0_WPNDINTEN               15            /*  Enable Write Pending Sourced Interrupts to the CPU */
#define BITP_RTC_CR0_WSYNCINTEN              14            /*  Enable Write Synchronization Sourced Interrupts to the CPU */
#define BITP_RTC_CR0_WPNDERRINTEN            13            /*  Enable Write Pending Error Sourced Interrupts to the CPU When an RTC Register-write Pending Error Occurs */
#define BITP_RTC_CR0_ISOINTEN                12            /*  Enable ISOINT Sourced Interrupts to the CPU When Isolation of the RTC Power Domain is Activated and Subsequently De-activated */
#define BITP_RTC_CR0_MOD60ALMINTEN           11            /*  Enable Periodic Modulo-60 RTC Alarm  Sourced Interrupts to the CPU */
#define BITP_RTC_CR0_MOD60ALM                 5            /*  Periodic, Modulo-60 Alarm Time in Prescaled RTC Time Units Beyond a Modulo-60 Boundary */
#define BITP_RTC_CR0_MOD60ALMEN               4            /*  Enable RTC Modulo-60 Counting of Time Past a Modulo-60 Boundary */
#define BITP_RTC_CR0_TRMEN                    3            /*  Enable RTC Digital Trimming */
#define BITP_RTC_CR0_ALMINTEN                 2            /*  Enable ALMINT Sourced Alarm Interrupts to the CPU */
#define BITP_RTC_CR0_ALMEN                    1            /*  Enable the RTC Alarm (Absolute) Operation */
#define BITP_RTC_CR0_CNTEN                    0            /*  Global Enable for the RTC */
#define BITM_RTC_CR0_WPNDINTEN               (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  Enable Write Pending Sourced Interrupts to the CPU */
#define BITM_RTC_CR0_WSYNCINTEN              (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  Enable Write Synchronization Sourced Interrupts to the CPU */
#define BITM_RTC_CR0_WPNDERRINTEN            (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  Enable Write Pending Error Sourced Interrupts to the CPU When an RTC Register-write Pending Error Occurs */
#define BITM_RTC_CR0_ISOINTEN                (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  Enable ISOINT Sourced Interrupts to the CPU When Isolation of the RTC Power Domain is Activated and Subsequently De-activated */
#define BITM_RTC_CR0_MOD60ALMINTEN           (_ADI_MSK_3(0x00000800,0x00000800U, uint16_t  ))    /*  Enable Periodic Modulo-60 RTC Alarm  Sourced Interrupts to the CPU */
#define BITM_RTC_CR0_MOD60ALM                (_ADI_MSK_3(0x000007E0,0x000007E0U, uint16_t  ))    /*  Periodic, Modulo-60 Alarm Time in Prescaled RTC Time Units Beyond a Modulo-60 Boundary */
#define BITM_RTC_CR0_MOD60ALMEN              (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Enable RTC Modulo-60 Counting of Time Past a Modulo-60 Boundary */
#define BITM_RTC_CR0_TRMEN                   (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Enable RTC Digital Trimming */
#define BITM_RTC_CR0_ALMINTEN                (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Enable ALMINT Sourced Alarm Interrupts to the CPU */
#define BITM_RTC_CR0_ALMEN                   (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Enable the RTC Alarm (Absolute) Operation */
#define BITM_RTC_CR0_CNTEN                   (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Global Enable for the RTC */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SR0                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SR0_ISOENB                  14            /*  Visibility of 32kHz Sourced Registers */
#define BITP_RTC_SR0_WSYNCTRM                13            /*  Synchronisation Status of Posted Writes to TRM */
#define BITP_RTC_SR0_WSYNCALM1               12            /*  Synchronisation Status of Posted Writes to ALM1 */
#define BITP_RTC_SR0_WSYNCALM0               11            /*  Synchronisation Status of Posted Writes to ALM0 */
#define BITP_RTC_SR0_WSYNCCNT1               10            /*  Synchronisation Status of Posted Writes to CNT1 */
#define BITP_RTC_SR0_WSYNCCNT0                9            /*  Synchronisation Status of Posted Writes to CNT0 */
#define BITP_RTC_SR0_WSYNCSR0                 8            /*  Synchronisation Status of Posted Writes to SR0 */
#define BITP_RTC_SR0_WSYNCCR0                 7            /*  Synchronisation Status of Posted Writes to CR0 */
#define BITP_RTC_SR0_WPNDINT                  6            /*  Write Pending Interrupt */
#define BITP_RTC_SR0_WSYNCINT                 5            /*  Write Synchronisation Interrupt */
#define BITP_RTC_SR0_WPNDERRINT               4            /*  Write Pending Error Interrupt Source */
#define BITP_RTC_SR0_ISOINT                   3            /*  RTC Power-Domain Isolation Interrupt Source */
#define BITP_RTC_SR0_MOD60ALMINT              2            /*  Modulo-60 RTC Alarm Interrupt Source */
#define BITP_RTC_SR0_ALMINT                   1            /*  Alarm Interrupt Source */
#define BITM_RTC_SR0_ISOENB                  (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  Visibility of 32kHz Sourced Registers */
#define BITM_RTC_SR0_WSYNCTRM                (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  Synchronisation Status of Posted Writes to TRM */
#define BITM_RTC_SR0_WSYNCALM1               (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  Synchronisation Status of Posted Writes to ALM1 */
#define BITM_RTC_SR0_WSYNCALM0               (_ADI_MSK_3(0x00000800,0x00000800U, uint16_t  ))    /*  Synchronisation Status of Posted Writes to ALM0 */
#define BITM_RTC_SR0_WSYNCCNT1               (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Synchronisation Status of Posted Writes to CNT1 */
#define BITM_RTC_SR0_WSYNCCNT0               (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  Synchronisation Status of Posted Writes to CNT0 */
#define BITM_RTC_SR0_WSYNCSR0                (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Synchronisation Status of Posted Writes to SR0 */
#define BITM_RTC_SR0_WSYNCCR0                (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  Synchronisation Status of Posted Writes to CR0 */
#define BITM_RTC_SR0_WPNDINT                 (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  Write Pending Interrupt */
#define BITM_RTC_SR0_WSYNCINT                (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  Write Synchronisation Interrupt */
#define BITM_RTC_SR0_WPNDERRINT              (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Write Pending Error Interrupt Source */
#define BITM_RTC_SR0_ISOINT                  (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  RTC Power-Domain Isolation Interrupt Source */
#define BITM_RTC_SR0_MOD60ALMINT             (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Modulo-60 RTC Alarm Interrupt Source */
#define BITM_RTC_SR0_ALMINT                  (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Alarm Interrupt Source */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SR1                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SR1_WPNDTRM                 13            /*  Pending Status of Posted Writes to TRM */
#define BITP_RTC_SR1_WPNDALM1                12            /*  Pending Status of Posted Writes to ALM1 */
#define BITP_RTC_SR1_WPNDALM0                11            /*  Pending Status of Posted Writes to ALM0 */
#define BITP_RTC_SR1_WPNDCNT1                10            /*  Pending Status of Posted Writes to CNT1 */
#define BITP_RTC_SR1_WPNDCNT0                 9            /*  Pending Status of Posted Writes to CNT0 */
#define BITP_RTC_SR1_WPNDSR0                  8            /*  Pending Status of Posted Clearances of Interrupt Sources in SR0 */
#define BITP_RTC_SR1_WPNDCR0                  7            /*  Pending Status of Posted Writes to CR0 */
#define BITM_RTC_SR1_WPNDTRM                 (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  Pending Status of Posted Writes to TRM */
#define BITM_RTC_SR1_WPNDALM1                (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  Pending Status of Posted Writes to ALM1 */
#define BITM_RTC_SR1_WPNDALM0                (_ADI_MSK_3(0x00000800,0x00000800U, uint16_t  ))    /*  Pending Status of Posted Writes to ALM0 */
#define BITM_RTC_SR1_WPNDCNT1                (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Pending Status of Posted Writes to CNT1 */
#define BITM_RTC_SR1_WPNDCNT0                (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  Pending Status of Posted Writes to CNT0 */
#define BITM_RTC_SR1_WPNDSR0                 (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Pending Status of Posted Clearances of Interrupt Sources in SR0 */
#define BITM_RTC_SR1_WPNDCR0                 (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  Pending Status of Posted Writes to CR0 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_CNT0                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_CNT0_VALUE                   0            /*  Lower 16 Prescaled (Non-Fractional) Bits of the RTC Real-Time Count */
#define BITM_RTC_CNT0_VALUE                  (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Lower 16 Prescaled (Non-Fractional) Bits of the RTC Real-Time Count */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_CNT1                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_CNT1_VALUE                   0            /*  Upper 16 Prescaled (Non-Fractional) Bits of the RTC Real-Time Count */
#define BITM_RTC_CNT1_VALUE                  (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Upper 16 Prescaled (Non-Fractional) Bits of the RTC Real-Time Count */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_ALM0                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_ALM0_VALUE                   0            /*  Lower 16 Prescaled (i.e. Non-Fractional) Bits of the RTC Alarm Target Time */
#define BITM_RTC_ALM0_VALUE                  (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Lower 16 Prescaled (i.e. Non-Fractional) Bits of the RTC Alarm Target Time */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_ALM1                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_ALM1_VALUE                   0            /*  Upper 16 Prescaled (Non-Fractional) Bits of the RTC Alarm Target Time */
#define BITM_RTC_ALM1_VALUE                  (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Upper 16 Prescaled (Non-Fractional) Bits of the RTC Alarm Target Time */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_TRM                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_TRM_IVL2EXPMIN               6            /*  Minimum Power-of-two Interval of Prescaled RTC Time Units Which TRM:TRMIVL TRMIVL Can Select */
#define BITP_RTC_TRM_IVL                      4            /*  Trim Interval in Prescaled RTC Time Units */
#define BITP_RTC_TRM_ADD                      3            /*  Trim Polarity */
#define BITP_RTC_TRM_VALUE                    0            /*  Trim Value in Prescaled RTC Time Units to Be Added or Subtracted from the RTC Count at the End of a Periodic Interval Selected by TRM:TRMIVL */
#define BITM_RTC_TRM_IVL2EXPMIN              (_ADI_MSK_3(0x000003C0,0x000003C0U, uint16_t  ))    /*  Minimum Power-of-two Interval of Prescaled RTC Time Units Which TRM:TRMIVL TRMIVL Can Select */
#define BITM_RTC_TRM_IVL                     (_ADI_MSK_3(0x00000030,0x00000030U, uint16_t  ))    /*  Trim Interval in Prescaled RTC Time Units */
#define BITM_RTC_TRM_ADD                     (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Trim Polarity */
#define BITM_RTC_TRM_VALUE                   (_ADI_MSK_3(0x00000007,0x00000007U, uint16_t  ))    /*  Trim Value in Prescaled RTC Time Units to Be Added or Subtracted from the RTC Count at the End of a Periodic Interval Selected by TRM:TRMIVL */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_GWY                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_GWY_SWKEY                    0            /*  Software-keyed Command Issued by the CPU */
#define BITM_RTC_GWY_SWKEY                   (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Software-keyed Command Issued by the CPU */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_CR1                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_CR1_PRESCALE2EXP             5            /*  Prescale Power of 2 Division Factor for the RTC Base Clock */
#define BITP_RTC_CR1_CNTMOD60ROLLINTEN        4            /*  Enable for the RTC Modulo-60 Count Roll-Over Interrupt Source, in SR2:RTCCNTMOD60ROLLINT    */
#define BITP_RTC_CR1_CNTROLLINTEN             3            /*  Enable for the RTC Count Roll-Over Interrupt Source, in SR2:RTCCNTROLLINT */
#define BITP_RTC_CR1_TRMINTEN                 2            /*  Enable for the RTC Trim Interrupt Source, in SR2:RTCTRMINT */
#define BITP_RTC_CR1_PSINTEN                  1            /*  Enable for the Prescaled, Modulo-1 Interrupt Source, in SR2:RTCPSINT */
#define BITP_RTC_CR1_CNTINTEN                 0            /*  Enable for the RTC Count Interrupt Source */
#define BITM_RTC_CR1_PRESCALE2EXP            (_ADI_MSK_3(0x000001E0,0x000001E0U, uint16_t  ))    /*  Prescale Power of 2 Division Factor for the RTC Base Clock */
#define BITM_RTC_CR1_CNTMOD60ROLLINTEN       (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Enable for the RTC Modulo-60 Count Roll-Over Interrupt Source, in SR2:RTCCNTMOD60ROLLINT    */
#define BITM_RTC_CR1_CNTROLLINTEN            (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Enable for the RTC Count Roll-Over Interrupt Source, in SR2:RTCCNTROLLINT */
#define BITM_RTC_CR1_TRMINTEN                (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Enable for the RTC Trim Interrupt Source, in SR2:RTCTRMINT */
#define BITM_RTC_CR1_PSINTEN                 (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Enable for the Prescaled, Modulo-1 Interrupt Source, in SR2:RTCPSINT */
#define BITM_RTC_CR1_CNTINTEN                (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Enable for the RTC Count Interrupt Source */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SR2                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SR2_WSYNCALM2MIR            15            /*  Synchronization Status of Posted Writes to ALM2 */
#define BITP_RTC_SR2_WSYNCCR1MIR             14            /*  Synchronization Status of Posted Writes to CR1 */
#define BITP_RTC_SR2_WPNDALM2MIR             13            /*  Pending Status of Posted Writes to ALM2 */
#define BITP_RTC_SR2_WPNDCR1MIR              12            /*  Pending Status of Posted Writes to CR1 */
#define BITP_RTC_SR2_TRMBDYMIR                7            /*  Mirror of MOD:RTCTRMBDY */
#define BITP_RTC_SR2_CNTMOD60ROLL             6            /*  RTC Count Modulo-60 Roll-Over */
#define BITP_RTC_SR2_CNTROLL                  5            /*  RTC Count Roll-Over */
#define BITP_RTC_SR2_CNTMOD60ROLLINT          4            /*  RTC Modulo-60 Count Roll-Over Interrupt Source */
#define BITP_RTC_SR2_CNTROLLINT               3            /*  RTC Count Roll-Over Interrupt Source */
#define BITP_RTC_SR2_TRMINT                   2            /*  RTC Trim Interrupt Source */
#define BITP_RTC_SR2_PSINT                    1            /*  RTC Prescaled, Modulo-1 Boundary Interrupt Source */
#define BITP_RTC_SR2_CNTINT                   0            /*  RTC Count Interrupt Source */
#define BITM_RTC_SR2_WSYNCALM2MIR            (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  Synchronization Status of Posted Writes to ALM2 */
#define BITM_RTC_SR2_WSYNCCR1MIR             (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  Synchronization Status of Posted Writes to CR1 */
#define BITM_RTC_SR2_WPNDALM2MIR             (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  Pending Status of Posted Writes to ALM2 */
#define BITM_RTC_SR2_WPNDCR1MIR              (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  Pending Status of Posted Writes to CR1 */
#define BITM_RTC_SR2_TRMBDYMIR               (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  Mirror of MOD:RTCTRMBDY */
#define BITM_RTC_SR2_CNTMOD60ROLL            (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  RTC Count Modulo-60 Roll-Over */
#define BITM_RTC_SR2_CNTROLL                 (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  RTC Count Roll-Over */
#define BITM_RTC_SR2_CNTMOD60ROLLINT         (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  RTC Modulo-60 Count Roll-Over Interrupt Source */
#define BITM_RTC_SR2_CNTROLLINT              (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  RTC Count Roll-Over Interrupt Source */
#define BITM_RTC_SR2_TRMINT                  (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  RTC Trim Interrupt Source */
#define BITM_RTC_SR2_PSINT                   (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  RTC Prescaled, Modulo-1 Boundary Interrupt Source */
#define BITM_RTC_SR2_CNTINT                  (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  RTC Count Interrupt Source */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SNAP0                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SNAP0_VALUE                  0            /*  Constituent Part of the 47-bit Input Capture Channel 0, Containing a Sticky Snapshot of  CNT0 */
#define BITM_RTC_SNAP0_VALUE                 (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Constituent Part of the 47-bit Input Capture Channel 0, Containing a Sticky Snapshot of  CNT0 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SNAP1                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SNAP1_VALUE                  0            /*  Part of the 47-bit Input Capture Channel 0 Containing a Sticky Snapshot of  CNT1 */
#define BITM_RTC_SNAP1_VALUE                 (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Part of the 47-bit Input Capture Channel 0 Containing a Sticky Snapshot of  CNT1 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SNAP2                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SNAP2_VALUE                  0            /*  Part of the 47-bit Input Capture Channel 0 Containing a Sticky Snapshot of CNT2 */
#define BITM_RTC_SNAP2_VALUE                 (_ADI_MSK_3(0x00007FFF,0x00007FFFU, uint16_t  ))    /*  Part of the 47-bit Input Capture Channel 0 Containing a Sticky Snapshot of CNT2 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_MOD                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_MOD_CNT0_4TOZERO            11            /*  Mirror of CNT0[4:0] */
#define BITP_RTC_MOD_TRMBDY                  10            /*  Trim Boundary Indicator */
#define BITP_RTC_MOD_INCR                     6            /*  Most Recent Increment Value Added to the RTC Count in CNT1 and CNT0 */
#define BITP_RTC_MOD_CNTMOD60                 0            /*  Modulo-60 Value of the RTC Count: CNT1 and CNT0 */
#define BITM_RTC_MOD_CNT0_4TOZERO            (_ADI_MSK_3(0x0000F800,0x0000F800U, uint16_t  ))    /*  Mirror of CNT0[4:0] */
#define BITM_RTC_MOD_TRMBDY                  (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Trim Boundary Indicator */
#define BITM_RTC_MOD_INCR                    (_ADI_MSK_3(0x000003C0,0x000003C0U, uint16_t  ))    /*  Most Recent Increment Value Added to the RTC Count in CNT1 and CNT0 */
#define BITM_RTC_MOD_CNTMOD60                (_ADI_MSK_3(0x0000003F,0x0000003FU, uint16_t  ))    /*  Modulo-60 Value of the RTC Count: CNT1 and CNT0 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_CNT2                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_CNT2_VALUE                   0            /*  Fractional Bits of the RTC Real-Time Count */
#define BITM_RTC_CNT2_VALUE                  (_ADI_MSK_3(0x00007FFF,0x00007FFFU, uint16_t  ))    /*  Fractional Bits of the RTC Real-Time Count */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_ALM2                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_ALM2_VALUE                   0            /*  Fractional Bits of the Alarm Target Time */
#define BITM_RTC_ALM2_VALUE                  (_ADI_MSK_3(0x00007FFF,0x00007FFFU, uint16_t  ))    /*  Fractional Bits of the Alarm Target Time */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SR3                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SR3_ALMINTMIR               13            /*  Read-only Mirror of the SR0:ALMINT Interrupt Source */
#define BITP_RTC_SR3_SS4IRQ                  12            /*  Sticky Interrupt Source for the SensorStrobe Channel 4 */
#define BITP_RTC_SR3_SS3IRQ                  11            /*  Sticky Interrupt Source for the SensorStrobe Channel 3 */
#define BITP_RTC_SR3_SS2IRQ                  10            /*  Sticky Interrupt Source for the SensorStrobe Channel 2 */
#define BITP_RTC_SR3_SS1IRQ                   9            /*  Sticky Interrupt Source for SensorStrobe Channel 1 */
#define BITP_RTC_SR3_SS4FEIRQ                 8            /*  Sticky Interrupt Source for the SensorStrobe Channel 4 Falling Edge */
#define BITP_RTC_SR3_SS3FEIRQ                 7            /*  Sticky Interrupt Source for the SensorStrobe Channel 3 Falling Edge */
#define BITP_RTC_SR3_SS2FEIRQ                 6            /*  Sticky Interrupt Source for the SensorStrobe Channel 2 Falling Edge */
#define BITP_RTC_SR3_SS1FEIRQ                 5            /*  Sticky Interrupt Source for the SensorStrobe Channel 1 Falling Edge */
#define BITP_RTC_SR3_IC4IRQ                   4            /*  Sticky Interrupt Source for the RTC Input Capture Channel 4 */
#define BITP_RTC_SR3_IC3IRQ                   3            /*  Sticky Interrupt Source for the RTC Input Capture Channel 3 */
#define BITP_RTC_SR3_IC2IRQ                   2            /*  Sticky Interrupt Source for the RTC Input Capture Channel 2 */
#define BITP_RTC_SR3_IC0IRQ                   0            /*  Sticky Interrupt Source for the RTC Input Capture Channel 0 */
#define BITM_RTC_SR3_ALMINTMIR               (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  Read-only Mirror of the SR0:ALMINT Interrupt Source */
#define BITM_RTC_SR3_SS4IRQ                  (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  Sticky Interrupt Source for the SensorStrobe Channel 4 */
#define BITM_RTC_SR3_SS3IRQ                  (_ADI_MSK_3(0x00000800,0x00000800U, uint16_t  ))    /*  Sticky Interrupt Source for the SensorStrobe Channel 3 */
#define BITM_RTC_SR3_SS2IRQ                  (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Sticky Interrupt Source for the SensorStrobe Channel 2 */
#define BITM_RTC_SR3_SS1IRQ                  (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  Sticky Interrupt Source for SensorStrobe Channel 1 */
#define BITM_RTC_SR3_SS4FEIRQ                (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Sticky Interrupt Source for the SensorStrobe Channel 4 Falling Edge */
#define BITM_RTC_SR3_SS3FEIRQ                (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  Sticky Interrupt Source for the SensorStrobe Channel 3 Falling Edge */
#define BITM_RTC_SR3_SS2FEIRQ                (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  Sticky Interrupt Source for the SensorStrobe Channel 2 Falling Edge */
#define BITM_RTC_SR3_SS1FEIRQ                (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  Sticky Interrupt Source for the SensorStrobe Channel 1 Falling Edge */
#define BITM_RTC_SR3_IC4IRQ                  (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Sticky Interrupt Source for the RTC Input Capture Channel 4 */
#define BITM_RTC_SR3_IC3IRQ                  (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Sticky Interrupt Source for the RTC Input Capture Channel 3 */
#define BITM_RTC_SR3_IC2IRQ                  (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Sticky Interrupt Source for the RTC Input Capture Channel 2 */
#define BITM_RTC_SR3_IC0IRQ                  (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Sticky Interrupt Source for the RTC Input Capture Channel 0 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_CR2IC                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_CR2IC_ICOWUSEN              15            /*  Enable Overwrite of Unread Snapshots for All Input Capture Channels */
#define BITP_RTC_CR2IC_IC4IRQEN              14            /*  Interrupt Enable for the RTC Input Capture Channel 4 */
#define BITP_RTC_CR2IC_IC3IRQEN              13            /*  Interrupt Enable for the RTC Input Capture Channel 3 */
#define BITP_RTC_CR2IC_IC2IRQEN              12            /*  Interrupt Enable for the RTC Input Capture Channel 2 */
#define BITP_RTC_CR2IC_IC0IRQEN              10            /*  Interrupt Enable for the RTC Input Capture Channel 0 */
#define BITP_RTC_CR2IC_IC4LH                  9            /*  Polarity of the Active-going Capture Edge for the Input Capture Channel 4 */
#define BITP_RTC_CR2IC_IC3LH                  8            /*  Polarity of the Active-going Capture Edge for the Input Capture Channel 3 */
#define BITP_RTC_CR2IC_IC2LH                  7            /*  Polarity of the Active-going Capture Edge for the Input Capture Channel 2 */
#define BITP_RTC_CR2IC_IC0LH                  5            /*  Polarity of the Active-Going Capture Edge for the RTC Input Capture Channel 0 */
#define BITP_RTC_CR2IC_IC4EN                  4            /*  Enable for the RTC Input Capture Channel 4 */
#define BITP_RTC_CR2IC_IC3EN                  3            /*  Enable for the RTC Input Capture Channel 3 */
#define BITP_RTC_CR2IC_IC2EN                  2            /*  Enable for the RTC Input Capture Channel 2 */
#define BITP_RTC_CR2IC_IC0EN                  0            /*  Enable for the RTC Input Capture Channel 0 */
#define BITM_RTC_CR2IC_ICOWUSEN              (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  Enable Overwrite of Unread Snapshots for All Input Capture Channels */
#define BITM_RTC_CR2IC_IC4IRQEN              (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  Interrupt Enable for the RTC Input Capture Channel 4 */
#define BITM_RTC_CR2IC_IC3IRQEN              (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  Interrupt Enable for the RTC Input Capture Channel 3 */
#define BITM_RTC_CR2IC_IC2IRQEN              (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  Interrupt Enable for the RTC Input Capture Channel 2 */
#define BITM_RTC_CR2IC_IC0IRQEN              (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Interrupt Enable for the RTC Input Capture Channel 0 */
#define BITM_RTC_CR2IC_IC4LH                 (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  Polarity of the Active-going Capture Edge for the Input Capture Channel 4 */
#define BITM_RTC_CR2IC_IC3LH                 (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Polarity of the Active-going Capture Edge for the Input Capture Channel 3 */
#define BITM_RTC_CR2IC_IC2LH                 (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  Polarity of the Active-going Capture Edge for the Input Capture Channel 2 */
#define BITM_RTC_CR2IC_IC0LH                 (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  Polarity of the Active-Going Capture Edge for the RTC Input Capture Channel 0 */
#define BITM_RTC_CR2IC_IC4EN                 (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Enable for the RTC Input Capture Channel 4 */
#define BITM_RTC_CR2IC_IC3EN                 (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Enable for the RTC Input Capture Channel 3 */
#define BITM_RTC_CR2IC_IC2EN                 (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Enable for the RTC Input Capture Channel 2 */
#define BITM_RTC_CR2IC_IC0EN                 (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Enable for the RTC Input Capture Channel 0 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_CR3SS                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_CR3SS_SS4IRQEN              12            /*  Posedge EdgeInterrupt Enable for the SensorStrobe Channel 4 */
#define BITP_RTC_CR3SS_SS3IRQEN              11            /*  Posedge EdgeInterrupt Enable for the SensorStrobe Channel 3 */
#define BITP_RTC_CR3SS_SS2IRQEN              10            /*  Posedge EdgeInterrupt Enable for the SensorStrobe Channel 2 */
#define BITP_RTC_CR3SS_SS1IRQEN               9            /*  Interrupt Enable for SensorStrobe Channel 1 */
#define BITP_RTC_CR3SS_SS4FEIRQEN             8            /*  Falling Edge Interrupt Enable for the SensorStrobe Channel 4 */
#define BITP_RTC_CR3SS_SS3FEIRQEN             7            /*  Falling Edge Interrupt Enable for the SensorStrobe Channel 3 */
#define BITP_RTC_CR3SS_SS2FEIRQEN             6            /*  Falling Edge Interrupt Enable for the SensorStrobe Channel 2 */
#define BITP_RTC_CR3SS_SS1FEIRQEN             5            /*  Falling Edge Interrupt Enable for the SensorStrobe Channel 1 */
#define BITP_RTC_CR3SS_SS4EN                  4            /*  Enable for the SensorStrobe Channel 4 */
#define BITP_RTC_CR3SS_SS3EN                  3            /*  Enable for the SensorStrobe Channel 3 */
#define BITP_RTC_CR3SS_SS2EN                  2            /*  Enable for the SensorStrobe Channel 2 */
#define BITP_RTC_CR3SS_SS1EN                  1            /*  Enable for SensorStrobe Channel 1 */
#define BITM_RTC_CR3SS_SS4IRQEN              (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  Posedge EdgeInterrupt Enable for the SensorStrobe Channel 4 */
#define BITM_RTC_CR3SS_SS3IRQEN              (_ADI_MSK_3(0x00000800,0x00000800U, uint16_t  ))    /*  Posedge EdgeInterrupt Enable for the SensorStrobe Channel 3 */
#define BITM_RTC_CR3SS_SS2IRQEN              (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Posedge EdgeInterrupt Enable for the SensorStrobe Channel 2 */
#define BITM_RTC_CR3SS_SS1IRQEN              (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  Interrupt Enable for SensorStrobe Channel 1 */
#define BITM_RTC_CR3SS_SS4FEIRQEN            (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Falling Edge Interrupt Enable for the SensorStrobe Channel 4 */
#define BITM_RTC_CR3SS_SS3FEIRQEN            (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  Falling Edge Interrupt Enable for the SensorStrobe Channel 3 */
#define BITM_RTC_CR3SS_SS2FEIRQEN            (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  Falling Edge Interrupt Enable for the SensorStrobe Channel 2 */
#define BITM_RTC_CR3SS_SS1FEIRQEN            (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  Falling Edge Interrupt Enable for the SensorStrobe Channel 1 */
#define BITM_RTC_CR3SS_SS4EN                 (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Enable for the SensorStrobe Channel 4 */
#define BITM_RTC_CR3SS_SS3EN                 (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Enable for the SensorStrobe Channel 3 */
#define BITM_RTC_CR3SS_SS2EN                 (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Enable for the SensorStrobe Channel 2 */
#define BITM_RTC_CR3SS_SS1EN                 (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Enable for SensorStrobe Channel 1 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_CR4SS                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_CR4SS_SS3ARLEN              11            /*  Enable for Fine Control on SensorStrobe Channel 3 Period and Duty Cycle */
#define BITP_RTC_CR4SS_SS2ARLEN              10            /*  Enable for Fine Control on SensorStrobe Channel 2 Period and Duty Cycle */
#define BITP_RTC_CR4SS_SS1ARLEN               9            /*  Enable for Fine Control on SensorStrobe Channel 1 Period and Duty Cycle */
#define BITP_RTC_CR4SS_SS4POL                 8            /*  SensorStrobe Channel 4 Polarity Control */
#define BITP_RTC_CR4SS_SS3POL                 7            /*  SensorStrobe Channel 3 Polarity Control */
#define BITP_RTC_CR4SS_SS2POL                 6            /*  SensorStrobe Channel 2 Polarity Control */
#define BITP_RTC_CR4SS_SS1POL                 5            /*  SensorSTrobe Channel 1 Polarity Control */
#define BITP_RTC_CR4SS_SS4MSKEN               4            /*  Enable for Thermometer-Code Masking of the SensorStrobe Channel 4 */
#define BITP_RTC_CR4SS_SS3MSKEN               3            /*  Enable for Thermometer-Code Masking of the SensorStrobe Channel 3 */
#define BITP_RTC_CR4SS_SS2MSKEN               2            /*  Enable for Thermometer-Code Masking of the SensorStrobe Channel 2 */
#define BITP_RTC_CR4SS_SS1MSKEN               1            /*  Enable for Thermometer-Code Masking of the SensorStrobe Channel 1 */
#define BITM_RTC_CR4SS_SS3ARLEN              (_ADI_MSK_3(0x00000800,0x00000800U, uint16_t  ))    /*  Enable for Fine Control on SensorStrobe Channel 3 Period and Duty Cycle */
#define BITM_RTC_CR4SS_SS2ARLEN              (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Enable for Fine Control on SensorStrobe Channel 2 Period and Duty Cycle */
#define BITM_RTC_CR4SS_SS1ARLEN              (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  Enable for Fine Control on SensorStrobe Channel 1 Period and Duty Cycle */
#define BITM_RTC_CR4SS_SS4POL                (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  SensorStrobe Channel 4 Polarity Control */
#define BITM_RTC_CR4SS_SS3POL                (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  SensorStrobe Channel 3 Polarity Control */
#define BITM_RTC_CR4SS_SS2POL                (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  SensorStrobe Channel 2 Polarity Control */
#define BITM_RTC_CR4SS_SS1POL                (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  SensorSTrobe Channel 1 Polarity Control */
#define BITM_RTC_CR4SS_SS4MSKEN              (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Enable for Thermometer-Code Masking of the SensorStrobe Channel 4 */
#define BITM_RTC_CR4SS_SS3MSKEN              (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Enable for Thermometer-Code Masking of the SensorStrobe Channel 3 */
#define BITM_RTC_CR4SS_SS2MSKEN              (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Enable for Thermometer-Code Masking of the SensorStrobe Channel 2 */
#define BITM_RTC_CR4SS_SS1MSKEN              (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Enable for Thermometer-Code Masking of the SensorStrobe Channel 1 */
#define ENUM_RTC_CR4SS_NO_MSK                (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  SS1MSKEN: Do not apply a mask to SensorStrobe Channel 1 Register */
#define ENUM_RTC_CR4SS_THERM_MSK             (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  SS1MSKEN: Apply thermometer decoded mask */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SSMSK                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SSMSK_SS4MSK                12            /*  SensorStrobe Channel 4 Period Control */
#define BITP_RTC_SSMSK_SS3MSK                 8            /*  SensorStrobe Channel 3 Period Control */
#define BITP_RTC_SSMSK_SS2MSK                 4            /*  SensorStrobe Channel 2 Period Control */
#define BITP_RTC_SSMSK_SS1MSK                 0            /*  Concatenation of Thermometer-Encoded Masks for the 16-bit SensorStrobe Channels */
#define BITM_RTC_SSMSK_SS4MSK                (_ADI_MSK_3(0x0000F000,0x0000F000U, uint16_t  ))    /*  SensorStrobe Channel 4 Period Control */
#define BITM_RTC_SSMSK_SS3MSK                (_ADI_MSK_3(0x00000F00,0x00000F00U, uint16_t  ))    /*  SensorStrobe Channel 3 Period Control */
#define BITM_RTC_SSMSK_SS2MSK                (_ADI_MSK_3(0x000000F0,0x000000F0U, uint16_t  ))    /*  SensorStrobe Channel 2 Period Control */
#define BITM_RTC_SSMSK_SS1MSK                (_ADI_MSK_3(0x0000000F,0x0000000FU, uint16_t  ))    /*  Concatenation of Thermometer-Encoded Masks for the 16-bit SensorStrobe Channels */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_IC2                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_IC2_IC2                      0            /*  RTC Input Capture Channel 2 */
#define BITM_RTC_IC2_IC2                     (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  RTC Input Capture Channel 2 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_IC3                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_IC3_IC3                      0            /*  RTC Input Capture Channel 3 */
#define BITM_RTC_IC3_IC3                     (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  RTC Input Capture Channel 3 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_IC4                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_IC4_IC4                      0            /*  RTC Input Capture Channel 4 */
#define BITM_RTC_IC4_IC4                     (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  RTC Input Capture Channel 4 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SS1                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SS1_SS1                      0            /*  SensorStrobe Channel 1 */
#define BITM_RTC_SS1_SS1                     (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  SensorStrobe Channel 1 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SS2                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SS2_SS2                      0            /*  SensorStrobe Channel 2 */
#define BITM_RTC_SS2_SS2                     (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  SensorStrobe Channel 2 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SS3                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SS3_SS3                      0            /*  SensorStrobe Channel 3 */
#define BITM_RTC_SS3_SS3                     (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  SensorStrobe Channel 3 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SS4                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SS4_SS4                      0            /*  SensorStrobe Channel 4 */
#define BITM_RTC_SS4_SS4                     (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  SensorStrobe Channel 4 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SR4                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SR4_WSYNCSSMSKOT            15            /*  Synchronization Status of Posted Reads Writes to Mask for SensorStrobe Channels on Time Control Register */
#define BITP_RTC_SR4_RSYNCIC4                14            /*  Synchronization Status of Posted Reads of RTC Input Channel 4 */
#define BITP_RTC_SR4_RSYNCIC3                13            /*  Synchronization Status of Posted Reads of RTC Input Channel 3 */
#define BITP_RTC_SR4_RSYNCIC2                12            /*  Synchronization Status of Posted Reads of RTC Input Channel 2 */
#define BITP_RTC_SR4_RSYNCIC0                10            /*  Synchronization Status of Posted Reads of RTC Input Channel 0 */
#define BITP_RTC_SR4_WSYNCSS4                 9            /*  Synchronization Status of Posted Writes to SensorStrobe Channel 4 */
#define BITP_RTC_SR4_WSYNCSS3                 8            /*  Synchronization Status of Posted Writes to SensorStrobe Channel 3 */
#define BITP_RTC_SR4_WSYNCSS2                 7            /*  Synchronization Status of Posted Writes to SensorStrobe Channel 2 */
#define BITP_RTC_SR4_WSYNCSS1                 6            /*  Synchronization Status of Posted Writes to SensorStrobe Channel 1 */
#define BITP_RTC_SR4_WSYNCSSMSK               4            /*  Synchronization Status of Posted Writes to Masks for SensorStrobe Channel Register */
#define BITP_RTC_SR4_WSYNCCR4SS               3            /*  Synchronization Status of Posted Writes to RTC Control 4 for Configuring SensorStrobe Channel Register */
#define BITP_RTC_SR4_WSYNCCR3SS               2            /*  Synchronization Status of Posted Writes to RTC Control 3 for Configuring SensorStrobe Channel Register */
#define BITP_RTC_SR4_WSYNCCR2IC               1            /*  Synchronization Status of Posted Writes to RTC Control 2 for Configuring Input Capture Channels Register */
#define BITP_RTC_SR4_WSYNCSR3                 0            /*  Synchronisation Status of Posted Writes to SR3 */
#define BITM_RTC_SR4_WSYNCSSMSKOT            (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  Synchronization Status of Posted Reads Writes to Mask for SensorStrobe Channels on Time Control Register */
#define BITM_RTC_SR4_RSYNCIC4                (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  Synchronization Status of Posted Reads of RTC Input Channel 4 */
#define BITM_RTC_SR4_RSYNCIC3                (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  Synchronization Status of Posted Reads of RTC Input Channel 3 */
#define BITM_RTC_SR4_RSYNCIC2                (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  Synchronization Status of Posted Reads of RTC Input Channel 2 */
#define BITM_RTC_SR4_RSYNCIC0                (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Synchronization Status of Posted Reads of RTC Input Channel 0 */
#define BITM_RTC_SR4_WSYNCSS4                (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  Synchronization Status of Posted Writes to SensorStrobe Channel 4 */
#define BITM_RTC_SR4_WSYNCSS3                (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Synchronization Status of Posted Writes to SensorStrobe Channel 3 */
#define BITM_RTC_SR4_WSYNCSS2                (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  Synchronization Status of Posted Writes to SensorStrobe Channel 2 */
#define BITM_RTC_SR4_WSYNCSS1                (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  Synchronization Status of Posted Writes to SensorStrobe Channel 1 */
#define BITM_RTC_SR4_WSYNCSSMSK              (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Synchronization Status of Posted Writes to Masks for SensorStrobe Channel Register */
#define BITM_RTC_SR4_WSYNCCR4SS              (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Synchronization Status of Posted Writes to RTC Control 4 for Configuring SensorStrobe Channel Register */
#define BITM_RTC_SR4_WSYNCCR3SS              (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Synchronization Status of Posted Writes to RTC Control 3 for Configuring SensorStrobe Channel Register */
#define BITM_RTC_SR4_WSYNCCR2IC              (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Synchronization Status of Posted Writes to RTC Control 2 for Configuring Input Capture Channels Register */
#define BITM_RTC_SR4_WSYNCSR3                (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Synchronisation Status of Posted Writes to SR3 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SR5                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SR5_WPENDSSMSKOT            15            /*  Pending Status of Posted Writes to RTC Masks for SensorStrobe Channel Register */
#define BITP_RTC_SR5_RPENDIC4                14            /*  Pending Status of Posted Reads of IC4 */
#define BITP_RTC_SR5_RPENDIC3                13            /*  Pending Status of Posted Reads of IC3 */
#define BITP_RTC_SR5_RPENDIC2                12            /*  Pending Status of Posted Reads of IC2 */
#define BITP_RTC_SR5_RPENDIC0                10            /*  Pending Status of Posted Reads of Input Capture Channel 0 */
#define BITP_RTC_SR5_WPENDSS4                 9            /*  Pending Status of Posted Writes to SensorStrobe Channel 4 */
#define BITP_RTC_SR5_WPENDSS3                 8            /*  Pending Status of Posted Writes to SensorStrobe Channel 3 */
#define BITP_RTC_SR5_WPENDSS2                 7            /*  Pending Status of Posted Writes to SensorStrobe Channel 2 */
#define BITP_RTC_SR5_WPENDSS1                 6            /*  Pending Status of Posted Writes to SensorStrobe Channel 1 */
#define BITP_RTC_SR5_WPENDSSMSK               4            /*  Pending Status of Posted Writes to RTC Masks for SensorStrobe Channel Register */
#define BITP_RTC_SR5_WPENDCR4SS               3            /*  Pending Status of Posted Writes to RTC Control 4 for Configuring SensorStrobe Channel Register */
#define BITP_RTC_SR5_WPENDCR3SS               2            /*  Pending Status of Posted Writes to RTC Control 3 for Configuring SensorStrobe Channel Register */
#define BITP_RTC_SR5_WPENDCR2IC               1            /*  Pending Status of Posted Writes to RTC Control 2 for Configuring Input Capture Channels Register */
#define BITP_RTC_SR5_WPENDSR3                 0            /*  Pending Status of Posted Clearances of Interrupt Sources in RTC Status 3 Register */
#define BITM_RTC_SR5_WPENDSSMSKOT            (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  Pending Status of Posted Writes to RTC Masks for SensorStrobe Channel Register */
#define BITM_RTC_SR5_RPENDIC4                (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  Pending Status of Posted Reads of IC4 */
#define BITM_RTC_SR5_RPENDIC3                (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  Pending Status of Posted Reads of IC3 */
#define BITM_RTC_SR5_RPENDIC2                (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  Pending Status of Posted Reads of IC2 */
#define BITM_RTC_SR5_RPENDIC0                (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Pending Status of Posted Reads of Input Capture Channel 0 */
#define BITM_RTC_SR5_WPENDSS4                (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  Pending Status of Posted Writes to SensorStrobe Channel 4 */
#define BITM_RTC_SR5_WPENDSS3                (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Pending Status of Posted Writes to SensorStrobe Channel 3 */
#define BITM_RTC_SR5_WPENDSS2                (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  Pending Status of Posted Writes to SensorStrobe Channel 2 */
#define BITM_RTC_SR5_WPENDSS1                (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  Pending Status of Posted Writes to SensorStrobe Channel 1 */
#define BITM_RTC_SR5_WPENDSSMSK              (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Pending Status of Posted Writes to RTC Masks for SensorStrobe Channel Register */
#define BITM_RTC_SR5_WPENDCR4SS              (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Pending Status of Posted Writes to RTC Control 4 for Configuring SensorStrobe Channel Register */
#define BITM_RTC_SR5_WPENDCR3SS              (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Pending Status of Posted Writes to RTC Control 3 for Configuring SensorStrobe Channel Register */
#define BITM_RTC_SR5_WPENDCR2IC              (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Pending Status of Posted Writes to RTC Control 2 for Configuring Input Capture Channels Register */
#define BITM_RTC_SR5_WPENDSR3                (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Pending Status of Posted Clearances of Interrupt Sources in RTC Status 3 Register */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SR6                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SR6_FRZCNTPTR                9            /*  Pointer for the Triple-Read Sequence of FRZCNT */
#define BITP_RTC_SR6_IC0SNAP                  8            /*  Confirmation That RTC Snapshot 0, 1, 2 Registers Reflect the Value of Input-Capture Channel RTC Input Capture Channel 0 */
#define BITP_RTC_SR6_IC4UNR                   4            /*  Sticky Unread Status of the Input Capture Channel 4 */
#define BITP_RTC_SR6_IC3UNR                   3            /*  Sticky Unread Status of the Input Capture Channel 3 */
#define BITP_RTC_SR6_IC2UNR                   2            /*  Sticky Unread Status of the Input Capture Channel 2 */
#define BITP_RTC_SR6_IC0UNR                   0            /*  Sticky Unread Status of the Input Capture Channel 0 */
#define BITM_RTC_SR6_FRZCNTPTR               (_ADI_MSK_3(0x00000600,0x00000600U, uint16_t  ))    /*  Pointer for the Triple-Read Sequence of FRZCNT */
#define BITM_RTC_SR6_IC0SNAP                 (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Confirmation That RTC Snapshot 0, 1, 2 Registers Reflect the Value of Input-Capture Channel RTC Input Capture Channel 0 */
#define BITM_RTC_SR6_IC4UNR                  (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Sticky Unread Status of the Input Capture Channel 4 */
#define BITM_RTC_SR6_IC3UNR                  (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Sticky Unread Status of the Input Capture Channel 3 */
#define BITM_RTC_SR6_IC2UNR                  (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Sticky Unread Status of the Input Capture Channel 2 */
#define BITM_RTC_SR6_IC0UNR                  (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Sticky Unread Status of the Input Capture Channel 0 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SS1TGT                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SS1TGT_SS1TGT                0            /*  Current Target Value for the SensorStrobe Channel 1 */
#define BITM_RTC_SS1TGT_SS1TGT               (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Current Target Value for the SensorStrobe Channel 1 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_FRZCNT                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_FRZCNT_FRZCNT                0            /*  RTC Freeze Count. Coherent, Triple 16-Bit Read of the 47-Bit RTC Count */
#define BITM_RTC_FRZCNT_FRZCNT               (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  RTC Freeze Count. Coherent, Triple 16-Bit Read of the 47-Bit RTC Count */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SS2TGT                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SS2TGT_SS2TGT                0            /*  Current, Cumulative Target Time for SensorStrobe Channel 2, Taking Account of Any Auto-reloading */
#define BITM_RTC_SS2TGT_SS2TGT               (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Current, Cumulative Target Time for SensorStrobe Channel 2, Taking Account of Any Auto-reloading */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SS3TGT                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SS3TGT_SS3TGT                0            /*  Current, Cumulative Target Time for SensorStrobe Channel 3, Taking Account of Any Auto-reloading */
#define BITM_RTC_SS3TGT_SS3TGT               (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Current, Cumulative Target Time for SensorStrobe Channel 3, Taking Account of Any Auto-reloading */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SS1LOWDUR                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SS1LOWDUR_SS1LOWDUR          0            /*  Low Duration for SensorStrobe Channel 1. */
#define BITM_RTC_SS1LOWDUR_SS1LOWDUR         (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Low Duration for SensorStrobe Channel 1. */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SS2LOWDUR                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SS2LOWDUR_SS2LOWDUR          0            /*  Low Duration for SensorStrobe Channel 2. */
#define BITM_RTC_SS2LOWDUR_SS2LOWDUR         (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Low Duration for SensorStrobe Channel 2. */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SS3LOWDUR                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SS3LOWDUR_SS3LOWDUR          0            /*  Low Duration for SensorStrobe Channel 3. */
#define BITM_RTC_SS3LOWDUR_SS3LOWDUR         (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Low Duration for SensorStrobe Channel 3. */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SS1HIGHDUR                       Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SS1HIGHDUR_SS1HIGHDUR        0            /*  High Duration for SensorStrobe Channel 1. */
#define BITM_RTC_SS1HIGHDUR_SS1HIGHDUR       (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  High Duration for SensorStrobe Channel 1. */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SS2HIGHDUR                       Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SS2HIGHDUR_SS2HIGHDUR        0            /*  High Duration for SensorStrobe Channel 2. */
#define BITM_RTC_SS2HIGHDUR_SS2HIGHDUR       (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  High Duration for SensorStrobe Channel 2. */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SS3HIGHDUR                       Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SS3HIGHDUR_SS3HIGHDUR        0            /*  High Duration for SensorStrobe Channel 3. */
#define BITM_RTC_SS3HIGHDUR_SS3HIGHDUR       (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  High Duration for SensorStrobe Channel 3. */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SSMSKOT                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SSMSKOT_SS4MSKOT            12            /*  SensorStrobe Channel 4 on Time Control */
#define BITP_RTC_SSMSKOT_SS3MSKOT             8            /*  SensorStrobe Channel 3 on Time Control */
#define BITP_RTC_SSMSKOT_SS2MSKOT             4            /*  SensorStrobe Channel 2 on Time Control */
#define BITP_RTC_SSMSKOT_SS1MSKOT             0            /*  Concatenation of Thermometer-encoded Masks for the 16-bit SensorStrobe Channels */
#define BITM_RTC_SSMSKOT_SS4MSKOT            (_ADI_MSK_3(0x0000F000,0x0000F000U, uint16_t  ))    /*  SensorStrobe Channel 4 on Time Control */
#define BITM_RTC_SSMSKOT_SS3MSKOT            (_ADI_MSK_3(0x00000F00,0x00000F00U, uint16_t  ))    /*  SensorStrobe Channel 3 on Time Control */
#define BITM_RTC_SSMSKOT_SS2MSKOT            (_ADI_MSK_3(0x000000F0,0x000000F0U, uint16_t  ))    /*  SensorStrobe Channel 2 on Time Control */
#define BITM_RTC_SSMSKOT_SS1MSKOT            (_ADI_MSK_3(0x0000000F,0x0000000FU, uint16_t  ))    /*  Concatenation of Thermometer-encoded Masks for the 16-bit SensorStrobe Channels */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_CR5SSS                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_CR5SSS_SS3SMPMTCHIRQEN      11            /*  Sample Activity Interrupt Enable for SensorStrobe Channel 3 */
#define BITP_RTC_CR5SSS_SS3SMPEN              8            /*  GPIO Input Sample Enable for SensorStrobe Channel 3 */
#define BITP_RTC_CR5SSS_SS2SMPMTCHIRQEN       7            /*  Sample Activity Interrupt Enable for SensorStrobe Channel 2 */
#define BITP_RTC_CR5SSS_SS2SMPEN              4            /*  GPIO Input Sample Enable for SensorStrobe Channel 2 */
#define BITP_RTC_CR5SSS_SS1SMPMTCHIRQEN       3            /*  Sample Activity Interrupt Enable for SensorStrobe Channel 1 */
#define BITP_RTC_CR5SSS_SS1SMPEN              0            /*  GPIO Input Sample Enable for SensorStrobe Channel 1 */
#define BITM_RTC_CR5SSS_SS3SMPMTCHIRQEN      (_ADI_MSK_3(0x00000800,0x00000800U, uint16_t  ))    /*  Sample Activity Interrupt Enable for SensorStrobe Channel 3 */
#define BITM_RTC_CR5SSS_SS3SMPEN             (_ADI_MSK_3(0x00000700,0x00000700U, uint16_t  ))    /*  GPIO Input Sample Enable for SensorStrobe Channel 3 */
#define BITM_RTC_CR5SSS_SS2SMPMTCHIRQEN      (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  Sample Activity Interrupt Enable for SensorStrobe Channel 2 */
#define BITM_RTC_CR5SSS_SS2SMPEN             (_ADI_MSK_3(0x00000070,0x00000070U, uint16_t  ))    /*  GPIO Input Sample Enable for SensorStrobe Channel 2 */
#define BITM_RTC_CR5SSS_SS1SMPMTCHIRQEN      (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Sample Activity Interrupt Enable for SensorStrobe Channel 1 */
#define BITM_RTC_CR5SSS_SS1SMPEN             (_ADI_MSK_3(0x00000007,0x00000007U, uint16_t  ))    /*  GPIO Input Sample Enable for SensorStrobe Channel 1 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_CR6SSS                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_CR6SSS_SS3SMPONRE           10            /*  GPIO Sample Around Rising Edge of SensorStrobe Channel 3 */
#define BITP_RTC_CR6SSS_SS3SMPONFE            8            /*  GPIO Sample Around Falling Edge of SensorStrobe Channel 3 */
#define BITP_RTC_CR6SSS_SS2SMPONRE            6            /*  GPIO Sample Around Rising Edge of SensorStrobe Channel 2 */
#define BITP_RTC_CR6SSS_SS2SMPONFE            4            /*  GPIO Sample Around Falling Edge of SensorStrobe Channel 2 */
#define BITP_RTC_CR6SSS_SS1SMPONRE            2            /*  GPIO Sample Around Rising Edge of SensorStrobe Channel 1 */
#define BITP_RTC_CR6SSS_SS1SMPONFE            0            /*  GPIO Sample Around Falling Edge of SensorStrobe Channel 1 */
#define BITM_RTC_CR6SSS_SS3SMPONRE           (_ADI_MSK_3(0x00000C00,0x00000C00U, uint16_t  ))    /*  GPIO Sample Around Rising Edge of SensorStrobe Channel 3 */
#define BITM_RTC_CR6SSS_SS3SMPONFE           (_ADI_MSK_3(0x00000300,0x00000300U, uint16_t  ))    /*  GPIO Sample Around Falling Edge of SensorStrobe Channel 3 */
#define BITM_RTC_CR6SSS_SS2SMPONRE           (_ADI_MSK_3(0x000000C0,0x000000C0U, uint16_t  ))    /*  GPIO Sample Around Rising Edge of SensorStrobe Channel 2 */
#define BITM_RTC_CR6SSS_SS2SMPONFE           (_ADI_MSK_3(0x00000030,0x00000030U, uint16_t  ))    /*  GPIO Sample Around Falling Edge of SensorStrobe Channel 2 */
#define BITM_RTC_CR6SSS_SS1SMPONRE           (_ADI_MSK_3(0x0000000C,0x0000000CU, uint16_t  ))    /*  GPIO Sample Around Rising Edge of SensorStrobe Channel 1 */
#define BITM_RTC_CR6SSS_SS1SMPONFE           (_ADI_MSK_3(0x00000003,0x00000003U, uint16_t  ))    /*  GPIO Sample Around Falling Edge of SensorStrobe Channel 1 */
#define ENUM_RTC_CR6SSS_SS3NORES             (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  SS3SMPONRE: No sampling of input around rising edge */
#define ENUM_RTC_CR6SSS_SS3BRES              (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  SS3SMPONRE: Input sampled one clock cycle before rising edge of the SensorStrobe channel 3 */
#define ENUM_RTC_CR6SSS_SS3RES               (_ADI_MSK_3(0x00000800,0x00000800U, uint16_t  ))    /*  SS3SMPONRE: Input sampled at rising edge of the SensorStrobe channel 3 */
#define ENUM_RTC_CR6SSS_SS3ARES              (_ADI_MSK_3(0x00000C00,0x00000C00U, uint16_t  ))    /*  SS3SMPONRE: Input sampled one clock cycle after rising edge of the SensorStrobe channel 3 */
#define ENUM_RTC_CR6SSS_SS3NOFES             (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  SS3SMPONFE: No sampling of input around falling edge */
#define ENUM_RTC_CR6SSS_SS3BFES              (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  SS3SMPONFE: Input sampled one clock cycle before falling edge of the SensorStrobe channel 3 */
#define ENUM_RTC_CR6SSS_SS3FES               (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  SS3SMPONFE: Input sampled at falling edge of the SensorStrobe channel 3 */
#define ENUM_RTC_CR6SSS_SS3AFES              (_ADI_MSK_3(0x00000300,0x00000300U, uint16_t  ))    /*  SS3SMPONFE: Input sampled one clock cycle after falling edge of the SensorStrobe channel 3 */
#define ENUM_RTC_CR6SSS_SS2NORES             (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  SS2SMPONRE: No sampling of input around rising edge */
#define ENUM_RTC_CR6SSS_SS2BRES              (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  SS2SMPONRE: Input sampled one clock cycle before rising edge of the SensorStrobe channel 2 */
#define ENUM_RTC_CR6SSS_SS2RES               (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  SS2SMPONRE: Input sampled at rising edge of the SensorStrobe channel 2 */
#define ENUM_RTC_CR6SSS_SS2ARES              (_ADI_MSK_3(0x000000C0,0x000000C0U, uint16_t  ))    /*  SS2SMPONRE: Input sampled one clock cycle after rising edge of the SensorStrobe channel 2 */
#define ENUM_RTC_CR6SSS_SS2NOFES             (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  SS2SMPONFE: No sampling of input around falling edge */
#define ENUM_RTC_CR6SSS_SS2BFES              (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  SS2SMPONFE: Input sampled one clock cycle before falling edge of the SensorStrobe channel 2 */
#define ENUM_RTC_CR6SSS_SS2FES               (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  SS2SMPONFE: Input sampled at falling edge of the SensorStrobe channel 2 */
#define ENUM_RTC_CR6SSS_SS2AFES              (_ADI_MSK_3(0x00000030,0x00000030U, uint16_t  ))    /*  SS2SMPONFE: Input sampled one clock cycle after falling edge of the SensorStrobe channel 2 */
#define ENUM_RTC_CR6SSS_SS1NORES             (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  SS1SMPONRE: No sampling of input around rising edge */
#define ENUM_RTC_CR6SSS_SS1BRES              (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  SS1SMPONRE: Input sampled one clock cycle before rising edge of the SensorStrobe channel 1 */
#define ENUM_RTC_CR6SSS_SS1RES               (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  SS1SMPONRE: Input sampled at rising edge of the SensorStrobe channel 1 */
#define ENUM_RTC_CR6SSS_SS1ARES              (_ADI_MSK_3(0x0000000C,0x0000000CU, uint16_t  ))    /*  SS1SMPONRE: Input sampled one clock cycle after rising edge of the SensorStrobe channel 1 */
#define ENUM_RTC_CR6SSS_SS1NOFES             (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  SS1SMPONFE: No sampling of input around falling edge */
#define ENUM_RTC_CR6SSS_SS1BFES              (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  SS1SMPONFE: Input sampled one clock cycle before falling edge of the SensorStrobe channel 1 */
#define ENUM_RTC_CR6SSS_SS1FES               (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  SS1SMPONFE: Input sampled at falling edge of the SensorStrobe channel 1 */
#define ENUM_RTC_CR6SSS_SS1AFES              (_ADI_MSK_3(0x00000003,0x00000003U, uint16_t  ))    /*  SS1SMPONFE: Input sampled one clock cycle after falling edge of the SensorStrobe channel 1 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_CR7SSS                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_CR7SSS_SS3SMPPTRN           13            /*  Sample Activity Selection for SensorStrobe Channel 3 */
#define BITP_RTC_CR7SSS_SS3SMPEXP            10            /*  Expected GPIO Sample for SensorStrobe Channel 3 */
#define BITP_RTC_CR7SSS_SS2SMPPTRN            8            /*  Sample Activity Selection for SensorStrobe Channel 2 */
#define BITP_RTC_CR7SSS_SS2SMPEXP             5            /*  Expected GPIO Sample for SensorStrobe Channel 2 */
#define BITP_RTC_CR7SSS_SS1SMPPTRN            3            /*  Sample Activity Selection for SensorStrobe Channel 1 */
#define BITP_RTC_CR7SSS_SS1SMPEXP             0            /*  Expected GPIO Sample for SensorStrobe Channel 1 */
#define BITM_RTC_CR7SSS_SS3SMPPTRN           (_ADI_MSK_3(0x00006000,0x00006000U, uint16_t  ))    /*  Sample Activity Selection for SensorStrobe Channel 3 */
#define BITM_RTC_CR7SSS_SS3SMPEXP            (_ADI_MSK_3(0x00001C00,0x00001C00U, uint16_t  ))    /*  Expected GPIO Sample for SensorStrobe Channel 3 */
#define BITM_RTC_CR7SSS_SS2SMPPTRN           (_ADI_MSK_3(0x00000300,0x00000300U, uint16_t  ))    /*  Sample Activity Selection for SensorStrobe Channel 2 */
#define BITM_RTC_CR7SSS_SS2SMPEXP            (_ADI_MSK_3(0x000000E0,0x000000E0U, uint16_t  ))    /*  Expected GPIO Sample for SensorStrobe Channel 2 */
#define BITM_RTC_CR7SSS_SS1SMPPTRN           (_ADI_MSK_3(0x00000018,0x00000018U, uint16_t  ))    /*  Sample Activity Selection for SensorStrobe Channel 1 */
#define BITM_RTC_CR7SSS_SS1SMPEXP            (_ADI_MSK_3(0x00000007,0x00000007U, uint16_t  ))    /*  Expected GPIO Sample for SensorStrobe Channel 1 */
#define ENUM_RTC_CR7SSS_SS3SMPCHNG           (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  SS3SMPPTRN: Current GPIO sample is not same as previous sample */
#define ENUM_RTC_CR7SSS_SS3SMPSAME           (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  SS3SMPPTRN: Current GPIO sample is same as previous sample */
#define ENUM_RTC_CR7SSS_SS3SMPMTCH           (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  SS3SMPPTRN: Current GPIO sample is same as expected sample */
#define ENUM_RTC_CR7SSS_SS3SMPNOMTCH         (_ADI_MSK_3(0x00006000,0x00006000U, uint16_t  ))    /*  SS3SMPPTRN: Current GPIO sample is not same as expected sample */
#define ENUM_RTC_CR7SSS_SS2SMPCHNG           (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  SS2SMPPTRN: Current GPIO sample is not same as previous sample */
#define ENUM_RTC_CR7SSS_SS2SMPSAME           (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  SS2SMPPTRN: Current GPIO sample is same as previous sample */
#define ENUM_RTC_CR7SSS_SS2SMPMTCH           (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  SS2SMPPTRN: Current GPIO sample is same as expected sample */
#define ENUM_RTC_CR7SSS_SS2SMPNOMTCH         (_ADI_MSK_3(0x00000300,0x00000300U, uint16_t  ))    /*  SS2SMPPTRN: Current GPIO sample is not same as expected sample */
#define ENUM_RTC_CR7SSS_SS1SMPCHNG           (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  SS1SMPPTRN: Current GPIO sample is not same as previous sample */
#define ENUM_RTC_CR7SSS_SS1SMPSAME           (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  SS1SMPPTRN: Current GPIO sample is same as previous sample */
#define ENUM_RTC_CR7SSS_SS1SMPMTCH           (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  SS1SMPPTRN: Current GPIO sample is same as expected sample */
#define ENUM_RTC_CR7SSS_SS1SMPNOMTCH         (_ADI_MSK_3(0x00000018,0x00000018U, uint16_t  ))    /*  SS1SMPPTRN: Current GPIO sample is not same as expected sample */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SR7                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SR7_SS4OUT                  15            /*  Output Value for SensorStrobe Channel 4 */
#define BITP_RTC_SR7_SS3OUT                  14            /*  Output Value for SensorStrobe Channel 3 */
#define BITP_RTC_SR7_SS2OUT                  13            /*  Output Value for SensorStrobe Channel 2 */
#define BITP_RTC_SR7_SS1OUT                  12            /*  Output Value for SensorStrobe Channel 1 */
#define BITP_RTC_SR7_SS3SMPMTCHIRQ           11            /*  Sticky Status of GPIO Sample Pattern Match for SensorStrobe Channel 3 */
#define BITP_RTC_SR7_SS3SMP                   8            /*  Latest GPIO Sample for SensorStrobe Channel 3 */
#define BITP_RTC_SR7_SS2SMPMTCHIRQ            7            /*  Sticky Status of GPIO Sample Pattern Match for SensorStrobe Channel 2 */
#define BITP_RTC_SR7_SS2SMP                   4            /*  Latest GPIO Sample for SensorStrobe Channel 2 */
#define BITP_RTC_SR7_SS1SMPMTCHIRQ            3            /*  Sticky Status of GPIO Sample Pattern Match for SensorStrobe Channel 1 */
#define BITP_RTC_SR7_SS1SMP                   0            /*  Latest GPIO Sample for SensorStrobe Channel 1 */
#define BITM_RTC_SR7_SS4OUT                  (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  Output Value for SensorStrobe Channel 4 */
#define BITM_RTC_SR7_SS3OUT                  (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  Output Value for SensorStrobe Channel 3 */
#define BITM_RTC_SR7_SS2OUT                  (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  Output Value for SensorStrobe Channel 2 */
#define BITM_RTC_SR7_SS1OUT                  (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  Output Value for SensorStrobe Channel 1 */
#define BITM_RTC_SR7_SS3SMPMTCHIRQ           (_ADI_MSK_3(0x00000800,0x00000800U, uint16_t  ))    /*  Sticky Status of GPIO Sample Pattern Match for SensorStrobe Channel 3 */
#define BITM_RTC_SR7_SS3SMP                  (_ADI_MSK_3(0x00000700,0x00000700U, uint16_t  ))    /*  Latest GPIO Sample for SensorStrobe Channel 3 */
#define BITM_RTC_SR7_SS2SMPMTCHIRQ           (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  Sticky Status of GPIO Sample Pattern Match for SensorStrobe Channel 2 */
#define BITM_RTC_SR7_SS2SMP                  (_ADI_MSK_3(0x00000070,0x00000070U, uint16_t  ))    /*  Latest GPIO Sample for SensorStrobe Channel 2 */
#define BITM_RTC_SR7_SS1SMPMTCHIRQ           (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Sticky Status of GPIO Sample Pattern Match for SensorStrobe Channel 1 */
#define BITM_RTC_SR7_SS1SMP                  (_ADI_MSK_3(0x00000007,0x00000007U, uint16_t  ))    /*  Latest GPIO Sample for SensorStrobe Channel 1 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SR8                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SR8_WSYNCGPMUX1             13            /*  Synchronisation Status of Posted Writes to GPIO Pin Mux Control Register 1 */
#define BITP_RTC_SR8_WSYNCGPMUX0             12            /*  Synchronisation Status of Posted Writes to GPIO Pin Mux Control Register 0 */
#define BITP_RTC_SR8_WSYNCSR7                11            /*  Synchronisation Status of Posted Writes to Status 7 Register */
#define BITP_RTC_SR8_WSYNCCR7SSS             10            /*  Synchronisation Status of Posted Writes to Control 7 for Configuring SensorStrobe Channel Register */
#define BITP_RTC_SR8_WSYNCCR6SSS              9            /*  Synchronisation Status of Posted Writes to Control 6 for Configuring SensorStrobe Channel Register */
#define BITP_RTC_SR8_WSYNCCR5SSS              8            /*  Synchronisation Status of Posted Writes to Control 5 for Configuring SensorStrobe Channel Register */
#define BITP_RTC_SR8_WSYNCSS3HIGHDUR          6            /*  Synchronisation Status of Posted Writes to SensorStrobe Channel 3 High Duration Register */
#define BITP_RTC_SR8_WSYNCSS2HIGHDUR          5            /*  Synchronisation Status of Posted Writes to SensorStrobe Channel 2 High Duration Register */
#define BITP_RTC_SR8_WSYNCSS1HIGHDUR          4            /*  Synchronisation Status of Posted Writes to SensorStrobe Channel 1 High Duration Register */
#define BITP_RTC_SR8_WSYNCSS3LOWDUR           2            /*  Synchronisation Status of Posted Writes to SensorStrobe Channel 3 Low Duration Register */
#define BITP_RTC_SR8_WSYNCSS2LOWDUR           1            /*  Synchronisation Status of Posted Writes to SensorStrobe Channel 2 Low Duration Register */
#define BITP_RTC_SR8_WSYNCSS1LOWDUR           0            /*  Synchronisation Status of Posted Writes to SensorStrobe Channel 1 Low Duration Register */
#define BITM_RTC_SR8_WSYNCGPMUX1             (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  Synchronisation Status of Posted Writes to GPIO Pin Mux Control Register 1 */
#define BITM_RTC_SR8_WSYNCGPMUX0             (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  Synchronisation Status of Posted Writes to GPIO Pin Mux Control Register 0 */
#define BITM_RTC_SR8_WSYNCSR7                (_ADI_MSK_3(0x00000800,0x00000800U, uint16_t  ))    /*  Synchronisation Status of Posted Writes to Status 7 Register */
#define BITM_RTC_SR8_WSYNCCR7SSS             (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Synchronisation Status of Posted Writes to Control 7 for Configuring SensorStrobe Channel Register */
#define BITM_RTC_SR8_WSYNCCR6SSS             (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  Synchronisation Status of Posted Writes to Control 6 for Configuring SensorStrobe Channel Register */
#define BITM_RTC_SR8_WSYNCCR5SSS             (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Synchronisation Status of Posted Writes to Control 5 for Configuring SensorStrobe Channel Register */
#define BITM_RTC_SR8_WSYNCSS3HIGHDUR         (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  Synchronisation Status of Posted Writes to SensorStrobe Channel 3 High Duration Register */
#define BITM_RTC_SR8_WSYNCSS2HIGHDUR         (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  Synchronisation Status of Posted Writes to SensorStrobe Channel 2 High Duration Register */
#define BITM_RTC_SR8_WSYNCSS1HIGHDUR         (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Synchronisation Status of Posted Writes to SensorStrobe Channel 1 High Duration Register */
#define BITM_RTC_SR8_WSYNCSS3LOWDUR          (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Synchronisation Status of Posted Writes to SensorStrobe Channel 3 Low Duration Register */
#define BITM_RTC_SR8_WSYNCSS2LOWDUR          (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Synchronisation Status of Posted Writes to SensorStrobe Channel 2 Low Duration Register */
#define BITM_RTC_SR8_WSYNCSS1LOWDUR          (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Synchronisation Status of Posted Writes to SensorStrobe Channel 1 Low Duration Register */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_SR9                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_SR9_WPENDGPMUX1             13            /*  Pending Status of Posted Writes to  GPMUX1 */
#define BITP_RTC_SR9_WPENDGPMUX0             12            /*  Pending Status of Posted Writes to GPMUX0 */
#define BITP_RTC_SR9_WPENDSR7                11            /*  Pending Status of Posted Writes to SR7 */
#define BITP_RTC_SR9_WPENDCR7SSS             10            /*  Pending Status of Posted Writes to Control 7 for Configuring SensorStrobe Channel Register */
#define BITP_RTC_SR9_WPENDCR6SSS              9            /*  Pending Status of Posted Writes to Control 6 for Configuring SensorStrobe Channel Register */
#define BITP_RTC_SR9_WPENDCR5SSS              8            /*  Pending Status of Posted Writes to Control 5 for Configuring SensorStrobe Channel Register */
#define BITP_RTC_SR9_WPENDSS3HIGHDUR          6            /*  Pending Status of Posted Writes to SensortStrobe Channel 3 High Duration Register */
#define BITP_RTC_SR9_WPENDSS2HIGHDUR          5            /*  Pending Status of Posted Writes to SensortStrobe Channel 2 High Duration Register */
#define BITP_RTC_SR9_WPENDSS1HIGHDUR          4            /*  Pending Status of Posted Writes to SensortStrobe Channel 1 High Duration Register */
#define BITP_RTC_SR9_WPENDSS3LOWDUR           2            /*  Pending Status of Posted Writes to SensortStrobe Channel 3 Low Duration Register */
#define BITP_RTC_SR9_WPENDSS2LOWDUR           1            /*  Pending Status of Posted Writes to SensortStrobe Channel 2 Low Duration Register */
#define BITP_RTC_SR9_WPENDSS1LOWDUR           0            /*  Pending Status of Posted Writes to SensortStrobe Channel 1 Low Duration Register */
#define BITM_RTC_SR9_WPENDGPMUX1             (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  Pending Status of Posted Writes to  GPMUX1 */
#define BITM_RTC_SR9_WPENDGPMUX0             (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  Pending Status of Posted Writes to GPMUX0 */
#define BITM_RTC_SR9_WPENDSR7                (_ADI_MSK_3(0x00000800,0x00000800U, uint16_t  ))    /*  Pending Status of Posted Writes to SR7 */
#define BITM_RTC_SR9_WPENDCR7SSS             (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Pending Status of Posted Writes to Control 7 for Configuring SensorStrobe Channel Register */
#define BITM_RTC_SR9_WPENDCR6SSS             (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  Pending Status of Posted Writes to Control 6 for Configuring SensorStrobe Channel Register */
#define BITM_RTC_SR9_WPENDCR5SSS             (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Pending Status of Posted Writes to Control 5 for Configuring SensorStrobe Channel Register */
#define BITM_RTC_SR9_WPENDSS3HIGHDUR         (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  Pending Status of Posted Writes to SensortStrobe Channel 3 High Duration Register */
#define BITM_RTC_SR9_WPENDSS2HIGHDUR         (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  Pending Status of Posted Writes to SensortStrobe Channel 2 High Duration Register */
#define BITM_RTC_SR9_WPENDSS1HIGHDUR         (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Pending Status of Posted Writes to SensortStrobe Channel 1 High Duration Register */
#define BITM_RTC_SR9_WPENDSS3LOWDUR          (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Pending Status of Posted Writes to SensortStrobe Channel 3 Low Duration Register */
#define BITM_RTC_SR9_WPENDSS2LOWDUR          (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Pending Status of Posted Writes to SensortStrobe Channel 2 Low Duration Register */
#define BITM_RTC_SR9_WPENDSS1LOWDUR          (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Pending Status of Posted Writes to SensortStrobe Channel 1 Low Duration Register */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_GPMUX0                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_GPMUX0_SS2GPIN1SEL          12            /*  GPIO Mux Selection for SensorStrobe Channel 2 Input 1 */
#define BITP_RTC_GPMUX0_SS2GPIN0SEL           9            /*  GPIO Mux Selection for SensorStrobe Channel 2 Input 0 */
#define BITP_RTC_GPMUX0_SS1GPIN2SEL           6            /*  GPIO Mux Selection for SensorStrobe Channel 1 Input 2 */
#define BITP_RTC_GPMUX0_SS1GPIN1SEL           3            /*  GPIO Mux Selection for SensorStrobe Channel 1 Input 1 */
#define BITP_RTC_GPMUX0_SS1GPIN0SEL           0            /*  GPIO Mux Selection for SensorStrobe Channel 1 Input0 */
#define BITM_RTC_GPMUX0_SS2GPIN1SEL          (_ADI_MSK_3(0x00007000,0x00007000U, uint16_t  ))    /*  GPIO Mux Selection for SensorStrobe Channel 2 Input 1 */
#define BITM_RTC_GPMUX0_SS2GPIN0SEL          (_ADI_MSK_3(0x00000E00,0x00000E00U, uint16_t  ))    /*  GPIO Mux Selection for SensorStrobe Channel 2 Input 0 */
#define BITM_RTC_GPMUX0_SS1GPIN2SEL          (_ADI_MSK_3(0x000001C0,0x000001C0U, uint16_t  ))    /*  GPIO Mux Selection for SensorStrobe Channel 1 Input 2 */
#define BITM_RTC_GPMUX0_SS1GPIN1SEL          (_ADI_MSK_3(0x00000038,0x00000038U, uint16_t  ))    /*  GPIO Mux Selection for SensorStrobe Channel 1 Input 1 */
#define BITM_RTC_GPMUX0_SS1GPIN0SEL          (_ADI_MSK_3(0x00000007,0x00000007U, uint16_t  ))    /*  GPIO Mux Selection for SensorStrobe Channel 1 Input0 */

/* -------------------------------------------------------------------------------------------------------------------------
          RTC_GPMUX1                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RTC_GPMUX1_SS3DIFFOUT           15            /*  Differential SensorStrobe Out Option for SensorStrobe Channel 3 */
#define BITP_RTC_GPMUX1_SS1DIFFOUT           14            /*  Differential SensorStrobe Out Option for SensorStrobe Channel 1 */
#define BITP_RTC_GPMUX1_SS3GPIN2SEL           9            /*  GPIO Mux Selection for SensorStrobe Channel 3 Input 2 */
#define BITP_RTC_GPMUX1_SS3GPIN1SEL           6            /*  GPIO Mux Selection for SensorStrobe Channel 3 Input 1 */
#define BITP_RTC_GPMUX1_SS3GPIN0SEL           3            /*  GPIO Mux Selection for SensorStrobe Channel 3 Input 0 */
#define BITP_RTC_GPMUX1_SS2GPIN2SEL           0            /*  GPIO Mux Selection for SensorStrobe Channel 2 Input 2 */
#define BITM_RTC_GPMUX1_SS3DIFFOUT           (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  Differential SensorStrobe Out Option for SensorStrobe Channel 3 */
#define BITM_RTC_GPMUX1_SS1DIFFOUT           (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  Differential SensorStrobe Out Option for SensorStrobe Channel 1 */
#define BITM_RTC_GPMUX1_SS3GPIN2SEL          (_ADI_MSK_3(0x00000E00,0x00000E00U, uint16_t  ))    /*  GPIO Mux Selection for SensorStrobe Channel 3 Input 2 */
#define BITM_RTC_GPMUX1_SS3GPIN1SEL          (_ADI_MSK_3(0x000001C0,0x000001C0U, uint16_t  ))    /*  GPIO Mux Selection for SensorStrobe Channel 3 Input 1 */
#define BITM_RTC_GPMUX1_SS3GPIN0SEL          (_ADI_MSK_3(0x00000038,0x00000038U, uint16_t  ))    /*  GPIO Mux Selection for SensorStrobe Channel 3 Input 0 */
#define BITM_RTC_GPMUX1_SS2GPIN2SEL          (_ADI_MSK_3(0x00000007,0x00000007U, uint16_t  ))    /*  GPIO Mux Selection for SensorStrobe Channel 2 Input 2 */


/* ============================================================================================================================
        System Identification and Debug Enable
   ============================================================================================================================ */

/* ============================================================================================================================
        SYS
   ============================================================================================================================ */
#define REG_SYS_ADIID                        0x40002020            /*  SYS ADI Identification */
#define REG_SYS_CHIPID                       0x40002024            /*  SYS Chip Identifier */
#define REG_SYS_SWDEN                        0x40002040            /*  SYS Serial Wire Debug Enable */

/* ============================================================================================================================
        SYS Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          SYS_ADIID                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SYS_ADIID_VALUE                  0            /*  Reads a fixed value of 0x4144 to indicate to debuggers that they are connected to an Analog Devices implemented Cortex based part */
#define BITM_SYS_ADIID_VALUE                 (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Reads a fixed value of 0x4144 to indicate to debuggers that they are connected to an Analog Devices implemented Cortex based part */

/* -------------------------------------------------------------------------------------------------------------------------
          SYS_CHIPID                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SYS_CHIPID_PARTID                4            /*  Part identifier */
#define BITP_SYS_CHIPID_REV                   0            /*  Silicon revision */
#define BITM_SYS_CHIPID_PARTID               (_ADI_MSK_3(0x0000FFF0,0x0000FFF0U, uint16_t  ))    /*  Part identifier */
#define BITM_SYS_CHIPID_REV                  (_ADI_MSK_3(0x0000000F,0x0000000FU, uint16_t  ))    /*  Silicon revision */

/* -------------------------------------------------------------------------------------------------------------------------
          SYS_SWDEN                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SYS_SWDEN_VALUE                  0            /*  To enable SWD interface */
#define BITM_SYS_SWDEN_VALUE                 (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  To enable SWD interface */


/* ============================================================================================================================
        Watchdog Timer
   ============================================================================================================================ */

/* ============================================================================================================================
        WDT0
   ============================================================================================================================ */
#define REG_WDT0_LOAD                        0x40002C00            /*  WDT0 Load Value */
#define REG_WDT0_CCNT                        0x40002C04            /*  WDT0 Current Count Value */
#define REG_WDT0_CTL                         0x40002C08            /*  WDT0 Control */
#define REG_WDT0_RESTART                     0x40002C0C            /*  WDT0 Clear Interrupt */
#define REG_WDT0_STAT                        0x40002C18            /*  WDT0 Status */

/* ============================================================================================================================
        WDT Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          WDT_LOAD                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_WDT_LOAD_VALUE                   0            /*  Load Value */
#define BITM_WDT_LOAD_VALUE                  (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Load Value */

/* -------------------------------------------------------------------------------------------------------------------------
          WDT_CCNT                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_WDT_CCNT_VALUE                   0            /*  Current Count Value */
#define BITM_WDT_CCNT_VALUE                  (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Current Count Value */

/* -------------------------------------------------------------------------------------------------------------------------
          WDT_CTL                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_WDT_CTL_SPARE                    7            /*  Unused Spare Bit */
#define BITP_WDT_CTL_MODE                     6            /*  Timer Mode */
#define BITP_WDT_CTL_EN                       5            /*  Timer Enable */
#define BITP_WDT_CTL_PRE                      2            /*  Prescaler */
#define BITP_WDT_CTL_IRQ                      1            /*  Timer Interrupt */
#define BITM_WDT_CTL_SPARE                   (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  Unused Spare Bit */
#define BITM_WDT_CTL_MODE                    (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  Timer Mode */
#define BITM_WDT_CTL_EN                      (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  Timer Enable */
#define BITM_WDT_CTL_PRE                     (_ADI_MSK_3(0x0000000C,0x0000000CU, uint16_t  ))    /*  Prescaler */
#define BITM_WDT_CTL_IRQ                     (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Timer Interrupt */
#define ENUM_WDT_CTL_FREE_RUN                (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  MODE: Free running mode */
#define ENUM_WDT_CTL_PERIODIC                (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  MODE: Periodic mode */
#define ENUM_WDT_CTL_WDT_DIS                 (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  EN: WDT not enabled */
#define ENUM_WDT_CTL_WDT_EN                  (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  EN: WDT enabled */
#define ENUM_WDT_CTL_DIV1                    (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  PRE: Source clock/1 */
#define ENUM_WDT_CTL_DIV16                   (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  PRE: Source clock/16 */
#define ENUM_WDT_CTL_DIV256                  (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  PRE: Source clock/256 (default) */
#define ENUM_WDT_CTL_RST                     (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  IRQ: WDT asserts reset when timed out */
#define ENUM_WDT_CTL_INT                     (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  IRQ: WDT generates interrupt when timed out */

/* -------------------------------------------------------------------------------------------------------------------------
          WDT_RESTART                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_WDT_RESTART_CLRWORD              0            /*  Clear Watchdog */
#define BITM_WDT_RESTART_CLRWORD             (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Clear Watchdog */

/* -------------------------------------------------------------------------------------------------------------------------
          WDT_STAT                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_WDT_STAT_RSTCTL                  5            /*  Reset Control Register Written and Locked */
#define BITP_WDT_STAT_LOCKED                  4            /*  Lock Status Bit */
#define BITP_WDT_STAT_COUNTING                3            /*  Control Register Write Sync in Progress */
#define BITP_WDT_STAT_LOADING                 2            /*  Load Register Write Sync in Progress */
#define BITP_WDT_STAT_CLRIRQ                  1            /*  Clear Interrupt Register Write Sync in Progress */
#define BITP_WDT_STAT_IRQ                     0            /*  WDT Interrupt */
#define BITM_WDT_STAT_RSTCTL                 (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  Reset Control Register Written and Locked */
#define BITM_WDT_STAT_LOCKED                 (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Lock Status Bit */
#define BITM_WDT_STAT_COUNTING               (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Control Register Write Sync in Progress */
#define BITM_WDT_STAT_LOADING                (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Load Register Write Sync in Progress */
#define BITM_WDT_STAT_CLRIRQ                 (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Clear Interrupt Register Write Sync in Progress */
#define BITM_WDT_STAT_IRQ                    (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  WDT Interrupt */


/* ============================================================================================================================
        I2C Master/Slave
   ============================================================================================================================ */

/* ============================================================================================================================
        I2C0
   ============================================================================================================================ */
#define REG_I2C0_MCTL                        0x40003000            /*  I2C0 Master Control */
#define REG_I2C0_MSTAT                       0x40003004            /*  I2C0 Master Status */
#define REG_I2C0_MRX                         0x40003008            /*  I2C0 Master Receive Data */
#define REG_I2C0_MTX                         0x4000300C            /*  I2C0 Master Transmit Data */
#define REG_I2C0_MRXCNT                      0x40003010            /*  I2C0 Master Receive Data Count */
#define REG_I2C0_MCRXCNT                     0x40003014            /*  I2C0 Master Current Receive Data Count */
#define REG_I2C0_ADDR1                       0x40003018            /*  I2C0 Master Address Byte 1 */
#define REG_I2C0_ADDR2                       0x4000301C            /*  I2C0 Master Address Byte 2 */
#define REG_I2C0_BYT                         0x40003020            /*  I2C0 Start Byte */
#define REG_I2C0_DIV                         0x40003024            /*  I2C0 Serial Clock Period Divisor */
#define REG_I2C0_SCTL                        0x40003028            /*  I2C0 Slave Control */
#define REG_I2C0_SSTAT                       0x4000302C            /*  I2C0 Slave I2C Status/Error/IRQ */
#define REG_I2C0_SRX                         0x40003030            /*  I2C0 Slave Receive */
#define REG_I2C0_STX                         0x40003034            /*  I2C0 Slave Transmit */
#define REG_I2C0_ALT                         0x40003038            /*  I2C0 Hardware General Call ID */
#define REG_I2C0_ID0                         0x4000303C            /*  I2C0 First Slave Address Device ID */
#define REG_I2C0_ID1                         0x40003040            /*  I2C0 Second Slave Address Device ID */
#define REG_I2C0_ID2                         0x40003044            /*  I2C0 Third Slave Address Device ID */
#define REG_I2C0_ID3                         0x40003048            /*  I2C0 Fourth Slave Address Device ID */
#define REG_I2C0_STAT                        0x4000304C            /*  I2C0 Master and Slave FIFO Status */
#define REG_I2C0_SHCTL                       0x40003050            /*  I2C0 Shared Control */
#define REG_I2C0_TCTL                        0x40003054            /*  I2C0 Timing Control Register */
#define REG_I2C0_ASTRETCH_SCL                0x40003058            /*  I2C0 Automatic Stretch SCL */

/* ============================================================================================================================
        I2C Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          I2C_MCTL                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_MCTL_STOPBUSCLR             13            /*  Prestop Bus Clear */
#define BITP_I2C_MCTL_BUSCLR                 12            /*  Bus-Clear Enable */
#define BITP_I2C_MCTL_MTXDMA                 11            /*  Enable Master Tx DMA Request */
#define BITP_I2C_MCTL_MRXDMA                 10            /*  Enable Master Rx DMA Request */
#define BITP_I2C_MCTL_MXMITDEC                9            /*  Decrement Master Tx FIFO Status When a Byte Txed */
#define BITP_I2C_MCTL_IENCMP                  8            /*  Transaction Completed (or Stop Detected) Interrupt Enable */
#define BITP_I2C_MCTL_IENACK                  7            /*  ACK Not Received Interrupt Enable */
#define BITP_I2C_MCTL_IENALOST                6            /*  Arbitration Lost Interrupt Enable */
#define BITP_I2C_MCTL_IENMTX                  5            /*  Transmit Request Interrupt Enable */
#define BITP_I2C_MCTL_IENMRX                  4            /*  Receive Request Interrupt Enable */
#define BITP_I2C_MCTL_STRETCHSCL              3            /*  Stretch SCL Enable */
#define BITP_I2C_MCTL_LOOPBACK                2            /*  Internal Loopback Enable */
#define BITP_I2C_MCTL_COMPLETE                1            /*  Start Back-off Disable */
#define BITP_I2C_MCTL_MASEN                   0            /*  Master Enable */
#define BITM_I2C_MCTL_STOPBUSCLR             (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  Prestop Bus Clear */
#define BITM_I2C_MCTL_BUSCLR                 (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  Bus-Clear Enable */
#define BITM_I2C_MCTL_MTXDMA                 (_ADI_MSK_3(0x00000800,0x00000800U, uint16_t  ))    /*  Enable Master Tx DMA Request */
#define BITM_I2C_MCTL_MRXDMA                 (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Enable Master Rx DMA Request */
#define BITM_I2C_MCTL_MXMITDEC               (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  Decrement Master Tx FIFO Status When a Byte Txed */
#define BITM_I2C_MCTL_IENCMP                 (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Transaction Completed (or Stop Detected) Interrupt Enable */
#define BITM_I2C_MCTL_IENACK                 (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  ACK Not Received Interrupt Enable */
#define BITM_I2C_MCTL_IENALOST               (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  Arbitration Lost Interrupt Enable */
#define BITM_I2C_MCTL_IENMTX                 (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  Transmit Request Interrupt Enable */
#define BITM_I2C_MCTL_IENMRX                 (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Receive Request Interrupt Enable */
#define BITM_I2C_MCTL_STRETCHSCL             (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Stretch SCL Enable */
#define BITM_I2C_MCTL_LOOPBACK               (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Internal Loopback Enable */
#define BITM_I2C_MCTL_COMPLETE               (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Start Back-off Disable */
#define BITM_I2C_MCTL_MASEN                  (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Master Enable */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_MSTAT                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_MSTAT_SCLFILT               14            /*  State of SCL Line */
#define BITP_I2C_MSTAT_SDAFILT               13            /*  State of SDA Line */
#define BITP_I2C_MSTAT_MTXUNDR               12            /*  Master Transmit Underflow */
#define BITP_I2C_MSTAT_MSTOP                 11            /*  STOP Driven by This I2C Master */
#define BITP_I2C_MSTAT_LINEBUSY              10            /*  Line is Busy */
#define BITP_I2C_MSTAT_MRXOVR                 9            /*  Master Receive FIFO Overflow */
#define BITP_I2C_MSTAT_TCOMP                  8            /*  Transaction Complete or Stop Detected */
#define BITP_I2C_MSTAT_NACKDATA               7            /*  ACK Not Received in Response to Data Write */
#define BITP_I2C_MSTAT_MBUSY                  6            /*  Master Busy */
#define BITP_I2C_MSTAT_ALOST                  5            /*  Arbitration Lost */
#define BITP_I2C_MSTAT_NACKADDR               4            /*  ACK Not Received in Response to an Address */
#define BITP_I2C_MSTAT_MRXREQ                 3            /*  Master Receive Request */
#define BITP_I2C_MSTAT_MTXREQ                 2            /*  Master Transmit Request/Clear Master Transmit Interrupt */
#define BITP_I2C_MSTAT_MTXF                   0            /*  Master Transmit FIFO Status */
#define BITM_I2C_MSTAT_SCLFILT               (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  State of SCL Line */
#define BITM_I2C_MSTAT_SDAFILT               (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  State of SDA Line */
#define BITM_I2C_MSTAT_MTXUNDR               (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  Master Transmit Underflow */
#define BITM_I2C_MSTAT_MSTOP                 (_ADI_MSK_3(0x00000800,0x00000800U, uint16_t  ))    /*  STOP Driven by This I2C Master */
#define BITM_I2C_MSTAT_LINEBUSY              (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Line is Busy */
#define BITM_I2C_MSTAT_MRXOVR                (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  Master Receive FIFO Overflow */
#define BITM_I2C_MSTAT_TCOMP                 (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Transaction Complete or Stop Detected */
#define BITM_I2C_MSTAT_NACKDATA              (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  ACK Not Received in Response to Data Write */
#define BITM_I2C_MSTAT_MBUSY                 (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  Master Busy */
#define BITM_I2C_MSTAT_ALOST                 (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  Arbitration Lost */
#define BITM_I2C_MSTAT_NACKADDR              (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  ACK Not Received in Response to an Address */
#define BITM_I2C_MSTAT_MRXREQ                (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Master Receive Request */
#define BITM_I2C_MSTAT_MTXREQ                (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Master Transmit Request/Clear Master Transmit Interrupt */
#define BITM_I2C_MSTAT_MTXF                  (_ADI_MSK_3(0x00000003,0x00000003U, uint16_t  ))    /*  Master Transmit FIFO Status */
#define ENUM_I2C_MSTAT_FIFO_EMPTY            (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  MTXF: FIFO Empty. */
#define ENUM_I2C_MSTAT_FIFO_1BYTE            (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  MTXF: 1 byte in FIFO. */
#define ENUM_I2C_MSTAT_FIFO_FULL             (_ADI_MSK_3(0x00000003,0x00000003U, uint16_t  ))    /*  MTXF: FIFO Full. */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_MRX                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_MRX_VALUE                    0            /*  Master Receive Register */
#define BITM_I2C_MRX_VALUE                   (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Master Receive Register */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_MTX                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_MTX_VALUE                    0            /*  Master Transmit Register */
#define BITM_I2C_MTX_VALUE                   (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Master Transmit Register */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_MRXCNT                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_MRXCNT_EXTEND                8            /*  Extended Read */
#define BITP_I2C_MRXCNT_VALUE                 0            /*  Receive Count */
#define BITM_I2C_MRXCNT_EXTEND               (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Extended Read */
#define BITM_I2C_MRXCNT_VALUE                (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Receive Count */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_MCRXCNT                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_MCRXCNT_VALUE                0            /*  Current Receive Count */
#define BITM_I2C_MCRXCNT_VALUE               (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Current Receive Count */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_ADDR1                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_ADDR1_VALUE                  0            /*  Address Byte 1 */
#define BITM_I2C_ADDR1_VALUE                 (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Address Byte 1 */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_ADDR2                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_ADDR2_VALUE                  0            /*  Address Byte 2 */
#define BITM_I2C_ADDR2_VALUE                 (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Address Byte 2 */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_BYT                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_BYT_SBYTE                    0            /*  Start Byte */
#define BITM_I2C_BYT_SBYTE                   (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Start Byte */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_DIV                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_DIV_HIGH                     8            /*  Serial Clock High Time */
#define BITP_I2C_DIV_LOW                      0            /*  Serial Clock Low Time */
#define BITM_I2C_DIV_HIGH                    (_ADI_MSK_3(0x0000FF00,0x0000FF00U, uint16_t  ))    /*  Serial Clock High Time */
#define BITM_I2C_DIV_LOW                     (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Serial Clock Low Time */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_SCTL                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_SCTL_STXDMA                 14            /*  Enable Slave Tx DMA Request */
#define BITP_I2C_SCTL_SRXDMA                 13            /*  Enable Slave Rx DMA Request */
#define BITP_I2C_SCTL_IENREPST               12            /*  Repeated Start Interrupt Enable */
#define BITP_I2C_SCTL_STXDEC                 11            /*  Decrement Slave Tx FIFO Status When a Byte is Txed */
#define BITP_I2C_SCTL_IENSTX                 10            /*  Slave Transmit Request Interrupt Enable */
#define BITP_I2C_SCTL_IENSRX                  9            /*  Slave Receive Request Interrupt Enable */
#define BITP_I2C_SCTL_IENSTOP                 8            /*  Stop Condition Detected Interrupt Enable */
#define BITP_I2C_SCTL_NACK                    7            /*  NACK Next Communication */
#define BITP_I2C_SCTL_EARLYTXR                5            /*  Early Transmit Request Mode */
#define BITP_I2C_SCTL_GCSBCLR                 4            /*  General Call Status Bit Clear */
#define BITP_I2C_SCTL_HGCEN                   3            /*  Hardware General Call Enable */
#define BITP_I2C_SCTL_GCEN                    2            /*  General Call Enable */
#define BITP_I2C_SCTL_ADR10EN                 1            /*  Enabled 10-bit Addressing */
#define BITP_I2C_SCTL_SLVEN                   0            /*  Slave Enable */
#define BITM_I2C_SCTL_STXDMA                 (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  Enable Slave Tx DMA Request */
#define BITM_I2C_SCTL_SRXDMA                 (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  Enable Slave Rx DMA Request */
#define BITM_I2C_SCTL_IENREPST               (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  Repeated Start Interrupt Enable */
#define BITM_I2C_SCTL_STXDEC                 (_ADI_MSK_3(0x00000800,0x00000800U, uint16_t  ))    /*  Decrement Slave Tx FIFO Status When a Byte is Txed */
#define BITM_I2C_SCTL_IENSTX                 (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Slave Transmit Request Interrupt Enable */
#define BITM_I2C_SCTL_IENSRX                 (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  Slave Receive Request Interrupt Enable */
#define BITM_I2C_SCTL_IENSTOP                (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Stop Condition Detected Interrupt Enable */
#define BITM_I2C_SCTL_NACK                   (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  NACK Next Communication */
#define BITM_I2C_SCTL_EARLYTXR               (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  Early Transmit Request Mode */
#define BITM_I2C_SCTL_GCSBCLR                (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  General Call Status Bit Clear */
#define BITM_I2C_SCTL_HGCEN                  (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Hardware General Call Enable */
#define BITM_I2C_SCTL_GCEN                   (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  General Call Enable */
#define BITM_I2C_SCTL_ADR10EN                (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Enabled 10-bit Addressing */
#define BITM_I2C_SCTL_SLVEN                  (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Slave Enable */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_SSTAT                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_SSTAT_START                 14            /*  Start and Matching Address */
#define BITP_I2C_SSTAT_REPSTART              13            /*  Repeated Start and Matching Address */
#define BITP_I2C_SSTAT_IDMAT                 11            /*  Device ID Matched */
#define BITP_I2C_SSTAT_STOP                  10            /*  Stop After Start and Matching Address */
#define BITP_I2C_SSTAT_GCID                   8            /*  General ID */
#define BITP_I2C_SSTAT_GCINT                  7            /*  General Call Interrupt */
#define BITP_I2C_SSTAT_SBUSY                  6            /*  Slave Busy */
#define BITP_I2C_SSTAT_NOACK                  5            /*  ACK Not Generated by the Slave */
#define BITP_I2C_SSTAT_SRXOVR                 4            /*  Slave Receive FIFO Overflow */
#define BITP_I2C_SSTAT_SRXREQ                 3            /*  Slave Receive Request */
#define BITP_I2C_SSTAT_STXREQ                 2            /*  Slave Transmit Request/Slave Transmit Interrupt */
#define BITP_I2C_SSTAT_STXUNDR                1            /*  Slave Transmit FIFO Underflow */
#define BITP_I2C_SSTAT_STXFSEREQ              0            /*  Slave Tx FIFO Status or Early Request */
#define BITM_I2C_SSTAT_START                 (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  Start and Matching Address */
#define BITM_I2C_SSTAT_REPSTART              (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  Repeated Start and Matching Address */
#define BITM_I2C_SSTAT_IDMAT                 (_ADI_MSK_3(0x00001800,0x00001800U, uint16_t  ))    /*  Device ID Matched */
#define BITM_I2C_SSTAT_STOP                  (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Stop After Start and Matching Address */
#define BITM_I2C_SSTAT_GCID                  (_ADI_MSK_3(0x00000300,0x00000300U, uint16_t  ))    /*  General ID */
#define BITM_I2C_SSTAT_GCINT                 (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  General Call Interrupt */
#define BITM_I2C_SSTAT_SBUSY                 (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  Slave Busy */
#define BITM_I2C_SSTAT_NOACK                 (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  ACK Not Generated by the Slave */
#define BITM_I2C_SSTAT_SRXOVR                (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Slave Receive FIFO Overflow */
#define BITM_I2C_SSTAT_SRXREQ                (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Slave Receive Request */
#define BITM_I2C_SSTAT_STXREQ                (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Slave Transmit Request/Slave Transmit Interrupt */
#define BITM_I2C_SSTAT_STXUNDR               (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Slave Transmit FIFO Underflow */
#define BITM_I2C_SSTAT_STXFSEREQ             (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Slave Tx FIFO Status or Early Request */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_SRX                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_SRX_VALUE                    0            /*  Slave Receive Register */
#define BITM_I2C_SRX_VALUE                   (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Slave Receive Register */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_STX                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_STX_VALUE                    0            /*  Slave Transmit Register */
#define BITM_I2C_STX_VALUE                   (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Slave Transmit Register */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_ALT                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_ALT_ID                       0            /*  Slave Alt */
#define BITM_I2C_ALT_ID                      (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Slave Alt */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_ID0                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_ID0_VALUE                    0            /*  Slave Device ID 0 */
#define BITM_I2C_ID0_VALUE                   (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Slave Device ID 0 */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_ID1                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_ID1_VALUE                    0            /*  Slave Device ID 1 */
#define BITM_I2C_ID1_VALUE                   (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Slave Device ID 1 */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_ID2                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_ID2_VALUE                    0            /*  Slave Device ID 2 */
#define BITM_I2C_ID2_VALUE                   (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Slave Device ID 2 */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_ID3                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_ID3_VALUE                    0            /*  Slave Device ID 3 */
#define BITM_I2C_ID3_VALUE                   (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Slave Device ID 3 */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_STAT                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_STAT_MFLUSH                  9            /*  Flush the Master Transmit FIFO */
#define BITP_I2C_STAT_SFLUSH                  8            /*  Flush the Slave Transmit FIFO */
#define BITP_I2C_STAT_MRXF                    6            /*  Master Receive FIFO Status */
#define BITP_I2C_STAT_MTXF                    4            /*  Master Transmit FIFO Status */
#define BITP_I2C_STAT_SRXF                    2            /*  Slave Receive FIFO Status */
#define BITP_I2C_STAT_STXF                    0            /*  Slave Transmit FIFO Status */
#define BITM_I2C_STAT_MFLUSH                 (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  Flush the Master Transmit FIFO */
#define BITM_I2C_STAT_SFLUSH                 (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Flush the Slave Transmit FIFO */
#define BITM_I2C_STAT_MRXF                   (_ADI_MSK_3(0x000000C0,0x000000C0U, uint16_t  ))    /*  Master Receive FIFO Status */
#define BITM_I2C_STAT_MTXF                   (_ADI_MSK_3(0x00000030,0x00000030U, uint16_t  ))    /*  Master Transmit FIFO Status */
#define BITM_I2C_STAT_SRXF                   (_ADI_MSK_3(0x0000000C,0x0000000CU, uint16_t  ))    /*  Slave Receive FIFO Status */
#define BITM_I2C_STAT_STXF                   (_ADI_MSK_3(0x00000003,0x00000003U, uint16_t  ))    /*  Slave Transmit FIFO Status */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_SHCTL                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_SHCTL_RST                    0            /*  Reset START STOP Detect Circuit */
#define BITM_I2C_SHCTL_RST                   (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Reset START STOP Detect Circuit */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_TCTL                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_TCTL_FILTEROFF               8            /*  Input Filter Control */
#define BITP_I2C_TCTL_THDATIN                 0            /*  Data in Hold Start */
#define BITM_I2C_TCTL_FILTEROFF              (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Input Filter Control */
#define BITM_I2C_TCTL_THDATIN                (_ADI_MSK_3(0x0000001F,0x0000001FU, uint16_t  ))    /*  Data in Hold Start */

/* -------------------------------------------------------------------------------------------------------------------------
          I2C_ASTRETCH_SCL                     Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_I2C_ASTRETCH_SCL_SLVTMO          9            /*  Slave Automatic Stretch Timeout */
#define BITP_I2C_ASTRETCH_SCL_MSTTMO          8            /*  Master Automatic Stretch Timeout */
#define BITP_I2C_ASTRETCH_SCL_SLV             4            /*  Slave Automatic Stretch Mode */
#define BITP_I2C_ASTRETCH_SCL_MST             0            /*  Master Automatic Stretch Mode */
#define BITM_I2C_ASTRETCH_SCL_SLVTMO         (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  Slave Automatic Stretch Timeout */
#define BITM_I2C_ASTRETCH_SCL_MSTTMO         (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Master Automatic Stretch Timeout */
#define BITM_I2C_ASTRETCH_SCL_SLV            (_ADI_MSK_3(0x000000F0,0x000000F0U, uint16_t  ))    /*  Slave Automatic Stretch Mode */
#define BITM_I2C_ASTRETCH_SCL_MST            (_ADI_MSK_3(0x0000000F,0x0000000FU, uint16_t  ))    /*  Master Automatic Stretch Mode */


/* ============================================================================================================================
        Serial Peripheral Interface
   ============================================================================================================================ */

/* ============================================================================================================================
        SPI0
   ============================================================================================================================ */
#define REG_SPI0_STAT                        0x40004000            /*  SPI0 Status */
#define REG_SPI0_RX                          0x40004004            /*  SPI0 Receive */
#define REG_SPI0_TX                          0x40004008            /*  SPI0 Transmit */
#define REG_SPI0_DIV                         0x4000400C            /*  SPI0 SPI Baud Rate Selection */
#define REG_SPI0_CTL                         0x40004010            /*  SPI0 SPI Configuration */
#define REG_SPI0_IEN                         0x40004014            /*  SPI0 SPI Interrupts Enable */
#define REG_SPI0_CNT                         0x40004018            /*  SPI0 Transfer Byte Count */
#define REG_SPI0_DMA                         0x4000401C            /*  SPI0 SPI DMA Enable */
#define REG_SPI0_FIFO_STAT                   0x40004020            /*  SPI0 FIFO Status */
#define REG_SPI0_RD_CTL                      0x40004024            /*  SPI0 Read Control */
#define REG_SPI0_FLOW_CTL                    0x40004028            /*  SPI0 Flow Control */
#define REG_SPI0_WAIT_TMR                    0x4000402C            /*  SPI0 Wait Timer for Flow Control */
#define REG_SPI0_CS_CTL                      0x40004030            /*  SPI0 Chip Select Control for Multi-slave Connections */
#define REG_SPI0_CS_OVERRIDE                 0x40004034            /*  SPI0 Chip Select Override */

/* ============================================================================================================================
        SPI1
   ============================================================================================================================ */
#define REG_SPI1_STAT                        0x40004400            /*  SPI1 Status */
#define REG_SPI1_RX                          0x40004404            /*  SPI1 Receive */
#define REG_SPI1_TX                          0x40004408            /*  SPI1 Transmit */
#define REG_SPI1_DIV                         0x4000440C            /*  SPI1 SPI Baud Rate Selection */
#define REG_SPI1_CTL                         0x40004410            /*  SPI1 SPI Configuration */
#define REG_SPI1_IEN                         0x40004414            /*  SPI1 SPI Interrupts Enable */
#define REG_SPI1_CNT                         0x40004418            /*  SPI1 Transfer Byte Count */
#define REG_SPI1_DMA                         0x4000441C            /*  SPI1 SPI DMA Enable */
#define REG_SPI1_FIFO_STAT                   0x40004420            /*  SPI1 FIFO Status */
#define REG_SPI1_RD_CTL                      0x40004424            /*  SPI1 Read Control */
#define REG_SPI1_FLOW_CTL                    0x40004428            /*  SPI1 Flow Control */
#define REG_SPI1_WAIT_TMR                    0x4000442C            /*  SPI1 Wait Timer for Flow Control */
#define REG_SPI1_CS_CTL                      0x40004430            /*  SPI1 Chip Select Control for Multi-slave Connections */
#define REG_SPI1_CS_OVERRIDE                 0x40004434            /*  SPI1 Chip Select Override */

/* ============================================================================================================================
        SPI2
   ============================================================================================================================ */
#define REG_SPI2_STAT                        0x40024000            /*  SPI2 Status */
#define REG_SPI2_RX                          0x40024004            /*  SPI2 Receive */
#define REG_SPI2_TX                          0x40024008            /*  SPI2 Transmit */
#define REG_SPI2_DIV                         0x4002400C            /*  SPI2 SPI Baud Rate Selection */
#define REG_SPI2_CTL                         0x40024010            /*  SPI2 SPI Configuration */
#define REG_SPI2_IEN                         0x40024014            /*  SPI2 SPI Interrupts Enable */
#define REG_SPI2_CNT                         0x40024018            /*  SPI2 Transfer Byte Count */
#define REG_SPI2_DMA                         0x4002401C            /*  SPI2 SPI DMA Enable */
#define REG_SPI2_FIFO_STAT                   0x40024020            /*  SPI2 FIFO Status */
#define REG_SPI2_RD_CTL                      0x40024024            /*  SPI2 Read Control */
#define REG_SPI2_FLOW_CTL                    0x40024028            /*  SPI2 Flow Control */
#define REG_SPI2_WAIT_TMR                    0x4002402C            /*  SPI2 Wait Timer for Flow Control */
#define REG_SPI2_CS_CTL                      0x40024030            /*  SPI2 Chip Select Control for Multi-slave Connections */
#define REG_SPI2_CS_OVERRIDE                 0x40024034            /*  SPI2 Chip Select Override */

/* ============================================================================================================================
        SPI Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          SPI_STAT                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPI_STAT_RDY                    15            /*  Detected an Edge on Ready Indicator for Flow Control */
#define BITP_SPI_STAT_CSFALL                 14            /*  Detected a Falling Edge on CS, in Slave CON Mode */
#define BITP_SPI_STAT_CSRISE                 13            /*  Detected a Rising Edge on CS, in Slave CON Mode */
#define BITP_SPI_STAT_CSERR                  12            /*  Detected a CS Error Condition in Slave Mode */
#define BITP_SPI_STAT_CS                     11            /*  CS Status */
#define BITP_SPI_STAT_RXOVR                   7            /*  SPI Rx FIFO Overflow */
#define BITP_SPI_STAT_RXIRQ                   6            /*  SPI Rx IRQ */
#define BITP_SPI_STAT_TXIRQ                   5            /*  SPI Tx IRQ */
#define BITP_SPI_STAT_TXUNDR                  4            /*  SPI Tx FIFO Underflow */
#define BITP_SPI_STAT_TXDONE                  3            /*  SPI Tx Done in Read Command Mode */
#define BITP_SPI_STAT_TXEMPTY                 2            /*  SPI Tx FIFO Empty Interrupt */
#define BITP_SPI_STAT_XFRDONE                 1            /*  SPI Transfer Completion */
#define BITP_SPI_STAT_IRQ                     0            /*  SPI Interrupt Status */
#define BITM_SPI_STAT_RDY                    (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  Detected an Edge on Ready Indicator for Flow Control */
#define BITM_SPI_STAT_CSFALL                 (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  Detected a Falling Edge on CS, in Slave CON Mode */
#define BITM_SPI_STAT_CSRISE                 (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  Detected a Rising Edge on CS, in Slave CON Mode */
#define BITM_SPI_STAT_CSERR                  (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  Detected a CS Error Condition in Slave Mode */
#define BITM_SPI_STAT_CS                     (_ADI_MSK_3(0x00000800,0x00000800U, uint16_t  ))    /*  CS Status */
#define BITM_SPI_STAT_RXOVR                  (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  SPI Rx FIFO Overflow */
#define BITM_SPI_STAT_RXIRQ                  (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  SPI Rx IRQ */
#define BITM_SPI_STAT_TXIRQ                  (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  SPI Tx IRQ */
#define BITM_SPI_STAT_TXUNDR                 (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  SPI Tx FIFO Underflow */
#define BITM_SPI_STAT_TXDONE                 (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  SPI Tx Done in Read Command Mode */
#define BITM_SPI_STAT_TXEMPTY                (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  SPI Tx FIFO Empty Interrupt */
#define BITM_SPI_STAT_XFRDONE                (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  SPI Transfer Completion */
#define BITM_SPI_STAT_IRQ                    (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  SPI Interrupt Status */

/* -------------------------------------------------------------------------------------------------------------------------
          SPI_RX                               Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPI_RX_BYTE2                     8            /*  8-bit Receive Buffer, Used Only in DMA Modes */
#define BITP_SPI_RX_BYTE1                     0            /*  8-bit Receive Buffer */
#define BITM_SPI_RX_BYTE2                    (_ADI_MSK_3(0x0000FF00,0x0000FF00U, uint16_t  ))    /*  8-bit Receive Buffer, Used Only in DMA Modes */
#define BITM_SPI_RX_BYTE1                    (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  8-bit Receive Buffer */

/* -------------------------------------------------------------------------------------------------------------------------
          SPI_TX                               Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPI_TX_BYTE2                     8            /*  8-bit Transmit Buffer, Used Only in DMA Modes */
#define BITP_SPI_TX_BYTE1                     0            /*  8-bit Transmit Buffer */
#define BITM_SPI_TX_BYTE2                    (_ADI_MSK_3(0x0000FF00,0x0000FF00U, uint16_t  ))    /*  8-bit Transmit Buffer, Used Only in DMA Modes */
#define BITM_SPI_TX_BYTE1                    (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  8-bit Transmit Buffer */

/* -------------------------------------------------------------------------------------------------------------------------
          SPI_DIV                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPI_DIV_VALUE                    0            /*  SPI Clock Divider */
#define BITM_SPI_DIV_VALUE                   (_ADI_MSK_3(0x0000003F,0x0000003FU, uint16_t  ))    /*  SPI Clock Divider */

/* -------------------------------------------------------------------------------------------------------------------------
          SPI_CTL                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPI_CTL_CSRST                   14            /*  Reset Mode for CS Error Bit */
#define BITP_SPI_CTL_TFLUSH                  13            /*  SPI Tx FIFO Flush Enable */
#define BITP_SPI_CTL_RFLUSH                  12            /*  SPI Rx FIFO Flush Enable */
#define BITP_SPI_CTL_CON                     11            /*  Continuous Transfer Enable */
#define BITP_SPI_CTL_LOOPBACK                10            /*  Loopback Enable */
#define BITP_SPI_CTL_OEN                      9            /*  Slave MISO Output Enable */
#define BITP_SPI_CTL_RXOF                     8            /*  Rx Overflow Overwrite Enable */
#define BITP_SPI_CTL_ZEN                      7            /*  Transmit Zeros Enable */
#define BITP_SPI_CTL_TIM                      6            /*  SPI Transfer and Interrupt Mode */
#define BITP_SPI_CTL_LSB                      5            /*  LSB First Transfer Enable */
#define BITP_SPI_CTL_WOM                      4            /*  SPI Wired-OR Mode */
#define BITP_SPI_CTL_CPOL                     3            /*  Serial Clock Polarity */
#define BITP_SPI_CTL_CPHA                     2            /*  Serial Clock Phase Mode */
#define BITP_SPI_CTL_MASEN                    1            /*  Master Mode Enable */
#define BITP_SPI_CTL_SPIEN                    0            /*  SPI Enable */
#define BITM_SPI_CTL_CSRST                   (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  Reset Mode for CS Error Bit */
#define BITM_SPI_CTL_TFLUSH                  (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  SPI Tx FIFO Flush Enable */
#define BITM_SPI_CTL_RFLUSH                  (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  SPI Rx FIFO Flush Enable */
#define BITM_SPI_CTL_CON                     (_ADI_MSK_3(0x00000800,0x00000800U, uint16_t  ))    /*  Continuous Transfer Enable */
#define BITM_SPI_CTL_LOOPBACK                (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Loopback Enable */
#define BITM_SPI_CTL_OEN                     (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  Slave MISO Output Enable */
#define BITM_SPI_CTL_RXOF                    (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Rx Overflow Overwrite Enable */
#define BITM_SPI_CTL_ZEN                     (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  Transmit Zeros Enable */
#define BITM_SPI_CTL_TIM                     (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  SPI Transfer and Interrupt Mode */
#define BITM_SPI_CTL_LSB                     (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  LSB First Transfer Enable */
#define BITM_SPI_CTL_WOM                     (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  SPI Wired-OR Mode */
#define BITM_SPI_CTL_CPOL                    (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Serial Clock Polarity */
#define BITM_SPI_CTL_CPHA                    (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Serial Clock Phase Mode */
#define BITM_SPI_CTL_MASEN                   (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Master Mode Enable */
#define BITM_SPI_CTL_SPIEN                   (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  SPI Enable */

/* -------------------------------------------------------------------------------------------------------------------------
          SPI_IEN                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPI_IEN_TXEMPTY                 14            /*  Tx FIFO Empty Interrupt Enable */
#define BITP_SPI_IEN_XFRDONE                 13            /*  SPI Transfer Completion Interrupt Enable */
#define BITP_SPI_IEN_TXDONE                  12            /*  SPI Transmit Done Interrupt Enable */
#define BITP_SPI_IEN_RDY                     11            /*  Ready Signal Edge Interrupt Enable */
#define BITP_SPI_IEN_RXOVR                   10            /*  Rx Overflow Interrupt Enable */
#define BITP_SPI_IEN_TXUNDR                   9            /*  Tx Underflow Interrupt Enable */
#define BITP_SPI_IEN_CS                       8            /*  Enable Interrupt on Every CS Edge in Slave CON Mode */
#define BITP_SPI_IEN_IRQMODE                  0            /*  SPI IRQ Mode Bits */
#define BITM_SPI_IEN_TXEMPTY                 (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  Tx FIFO Empty Interrupt Enable */
#define BITM_SPI_IEN_XFRDONE                 (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  SPI Transfer Completion Interrupt Enable */
#define BITM_SPI_IEN_TXDONE                  (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  SPI Transmit Done Interrupt Enable */
#define BITM_SPI_IEN_RDY                     (_ADI_MSK_3(0x00000800,0x00000800U, uint16_t  ))    /*  Ready Signal Edge Interrupt Enable */
#define BITM_SPI_IEN_RXOVR                   (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Rx Overflow Interrupt Enable */
#define BITM_SPI_IEN_TXUNDR                  (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  Tx Underflow Interrupt Enable */
#define BITM_SPI_IEN_CS                      (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Enable Interrupt on Every CS Edge in Slave CON Mode */
#define BITM_SPI_IEN_IRQMODE                 (_ADI_MSK_3(0x00000007,0x00000007U, uint16_t  ))    /*  SPI IRQ Mode Bits */

/* -------------------------------------------------------------------------------------------------------------------------
          SPI_CNT                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPI_CNT_FRAMECONT               15            /*  Continue Frame */
#define BITP_SPI_CNT_VALUE                    0            /*  Transfer Byte Count */
#define BITM_SPI_CNT_FRAMECONT               (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  Continue Frame */
#define BITM_SPI_CNT_VALUE                   (_ADI_MSK_3(0x00003FFF,0x00003FFFU, uint16_t  ))    /*  Transfer Byte Count */

/* -------------------------------------------------------------------------------------------------------------------------
          SPI_DMA                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPI_DMA_RXEN                     2            /*  Enable Receive DMA Request */
#define BITP_SPI_DMA_TXEN                     1            /*  Enable Transmit DMA Request */
#define BITP_SPI_DMA_EN                       0            /*  Enable DMA for Data Transfer */
#define BITM_SPI_DMA_RXEN                    (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Enable Receive DMA Request */
#define BITM_SPI_DMA_TXEN                    (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Enable Transmit DMA Request */
#define BITM_SPI_DMA_EN                      (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Enable DMA for Data Transfer */

/* -------------------------------------------------------------------------------------------------------------------------
          SPI_FIFO_STAT                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPI_FIFO_STAT_RX                 8            /*  SPI Rx FIFO Dtatus */
#define BITP_SPI_FIFO_STAT_TX                 0            /*  SPI Tx FIFO Status */
#define BITM_SPI_FIFO_STAT_RX                (_ADI_MSK_3(0x00000F00,0x00000F00U, uint16_t  ))    /*  SPI Rx FIFO Dtatus */
#define BITM_SPI_FIFO_STAT_TX                (_ADI_MSK_3(0x0000000F,0x0000000FU, uint16_t  ))    /*  SPI Tx FIFO Status */

/* -------------------------------------------------------------------------------------------------------------------------
          SPI_RD_CTL                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPI_RD_CTL_THREEPIN              8            /*  Three Pin SPI Mode */
#define BITP_SPI_RD_CTL_TXBYTES               2            /*  Transmit Byte Count - 1 (Read Command) */
#define BITP_SPI_RD_CTL_OVERLAP               1            /*  Tx/Rx Overlap Mode */
#define BITP_SPI_RD_CTL_CMDEN                 0            /*  Read Command Enable */
#define BITM_SPI_RD_CTL_THREEPIN             (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Three Pin SPI Mode */
#define BITM_SPI_RD_CTL_TXBYTES              (_ADI_MSK_3(0x0000003C,0x0000003CU, uint16_t  ))    /*  Transmit Byte Count - 1 (Read Command) */
#define BITM_SPI_RD_CTL_OVERLAP              (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Tx/Rx Overlap Mode */
#define BITM_SPI_RD_CTL_CMDEN                (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Read Command Enable */

/* -------------------------------------------------------------------------------------------------------------------------
          SPI_FLOW_CTL                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPI_FLOW_CTL_RDBURSTSZ           6            /*  Read Data Burst Size - 1 */
#define BITP_SPI_FLOW_CTL_RDYPOL              4            /*  Polarity of RDY/MISO Line */
#define BITP_SPI_FLOW_CTL_MODE                0            /*  Flow Control Mode */
#define BITM_SPI_FLOW_CTL_RDBURSTSZ          (_ADI_MSK_3(0x0000FFC0,0x0000FFC0U, uint16_t  ))    /*  Read Data Burst Size - 1 */
#define BITM_SPI_FLOW_CTL_RDYPOL             (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Polarity of RDY/MISO Line */
#define BITM_SPI_FLOW_CTL_MODE               (_ADI_MSK_3(0x00000003,0x00000003U, uint16_t  ))    /*  Flow Control Mode */

/* -------------------------------------------------------------------------------------------------------------------------
          SPI_WAIT_TMR                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPI_WAIT_TMR_VALUE               0            /*  Wait Timer */
#define BITM_SPI_WAIT_TMR_VALUE              (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Wait Timer */

/* -------------------------------------------------------------------------------------------------------------------------
          SPI_CS_CTL                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPI_CS_CTL_SEL                   0            /*  Chip Select Control */
#define BITM_SPI_CS_CTL_SEL                  (_ADI_MSK_3(0x0000000F,0x0000000FU, uint16_t  ))    /*  Chip Select Control */

/* -------------------------------------------------------------------------------------------------------------------------
          SPI_CS_OVERRIDE                      Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPI_CS_OVERRIDE_CTL              0            /*  CS Override Control */
#define BITM_SPI_CS_OVERRIDE_CTL             (_ADI_MSK_3(0x00000003,0x00000003U, uint16_t  ))    /*  CS Override Control */


/* ============================================================================================================================
        
   ============================================================================================================================ */

/* ============================================================================================================================
        UART0
   ============================================================================================================================ */
#define REG_UART0_RX                         0x40005000            /*  UART0 Receive Buffer Register */
#define REG_UART0_TX                         0x40005000            /*  UART0 Transmit Holding Register */
#define REG_UART0_IEN                        0x40005004            /*  UART0 Interrupt Enable */
#define REG_UART0_IIR                        0x40005008            /*  UART0 Interrupt ID */
#define REG_UART0_LCR                        0x4000500C            /*  UART0 Line Control */
#define REG_UART0_MCR                        0x40005010            /*  UART0 Modem Control */
#define REG_UART0_LSR                        0x40005014            /*  UART0 Line Status */
#define REG_UART0_MSR                        0x40005018            /*  UART0 Modem Status */
#define REG_UART0_SCR                        0x4000501C            /*  UART0 Scratch Buffer */
#define REG_UART0_FCR                        0x40005020            /*  UART0 FIFO Control */
#define REG_UART0_FBR                        0x40005024            /*  UART0 Fractional Baud Rate */
#define REG_UART0_DIV                        0x40005028            /*  UART0 Baud Rate Divider */
#define REG_UART0_LCR2                       0x4000502C            /*  UART0 Second Line Control */
#define REG_UART0_CTL                        0x40005030            /*  UART0 UART Control Register */
#define REG_UART0_RFC                        0x40005034            /*  UART0 RX FIFO Byte Count */
#define REG_UART0_TFC                        0x40005038            /*  UART0 TX FIFO Byte Count */
#define REG_UART0_RSC                        0x4000503C            /*  UART0 RS485 Half-duplex Control */
#define REG_UART0_ACR                        0x40005040            /*  UART0 Auto Baud Control */
#define REG_UART0_ASRL                       0x40005044            /*  UART0 Auto Baud Status (Low) */
#define REG_UART0_ASRH                       0x40005048            /*  UART0 Auto Baud Status (High) */

/* ============================================================================================================================
        UART1
   ============================================================================================================================ */
#define REG_UART1_RX                         0x40005400            /*  UART1 Receive Buffer Register */
#define REG_UART1_TX                         0x40005400            /*  UART1 Transmit Holding Register */
#define REG_UART1_IEN                        0x40005404            /*  UART1 Interrupt Enable */
#define REG_UART1_IIR                        0x40005408            /*  UART1 Interrupt ID */
#define REG_UART1_LCR                        0x4000540C            /*  UART1 Line Control */
#define REG_UART1_MCR                        0x40005410            /*  UART1 Modem Control */
#define REG_UART1_LSR                        0x40005414            /*  UART1 Line Status */
#define REG_UART1_MSR                        0x40005418            /*  UART1 Modem Status */
#define REG_UART1_SCR                        0x4000541C            /*  UART1 Scratch Buffer */
#define REG_UART1_FCR                        0x40005420            /*  UART1 FIFO Control */
#define REG_UART1_FBR                        0x40005424            /*  UART1 Fractional Baud Rate */
#define REG_UART1_DIV                        0x40005428            /*  UART1 Baud Rate Divider */
#define REG_UART1_LCR2                       0x4000542C            /*  UART1 Second Line Control */
#define REG_UART1_CTL                        0x40005430            /*  UART1 UART Control Register */
#define REG_UART1_RFC                        0x40005434            /*  UART1 RX FIFO Byte Count */
#define REG_UART1_TFC                        0x40005438            /*  UART1 TX FIFO Byte Count */
#define REG_UART1_RSC                        0x4000543C            /*  UART1 RS485 Half-duplex Control */
#define REG_UART1_ACR                        0x40005440            /*  UART1 Auto Baud Control */
#define REG_UART1_ASRL                       0x40005444            /*  UART1 Auto Baud Status (Low) */
#define REG_UART1_ASRH                       0x40005448            /*  UART1 Auto Baud Status (High) */

/* ============================================================================================================================
        UART Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          UART_RX                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_UART_RX_RBR                      0            /*  Receive Buffer Register */
#define BITM_UART_RX_RBR                     (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Receive Buffer Register */

/* -------------------------------------------------------------------------------------------------------------------------
          UART_TX                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_UART_TX_THR                      0            /*  Transmit Holding Register */
#define BITM_UART_TX_THR                     (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Transmit Holding Register */

/* -------------------------------------------------------------------------------------------------------------------------
          UART_IEN                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_UART_IEN_EDMAR                   5            /*  DMA Requests in Receive Mode */
#define BITP_UART_IEN_EDMAT                   4            /*  DMA Requests in Transmit Mode */
#define BITP_UART_IEN_EDSSI                   3            /*  Modem Status Interrupt */
#define BITP_UART_IEN_ELSI                    2            /*  Rx Status Interrupt */
#define BITP_UART_IEN_ETBEI                   1            /*  Transmit Buffer Empty Interrupt */
#define BITP_UART_IEN_ERBFI                   0            /*  Receive Buffer Full Interrupt */
#define BITM_UART_IEN_EDMAR                  (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  DMA Requests in Receive Mode */
#define BITM_UART_IEN_EDMAT                  (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  DMA Requests in Transmit Mode */
#define BITM_UART_IEN_EDSSI                  (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Modem Status Interrupt */
#define BITM_UART_IEN_ELSI                   (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Rx Status Interrupt */
#define BITM_UART_IEN_ETBEI                  (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Transmit Buffer Empty Interrupt */
#define BITM_UART_IEN_ERBFI                  (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Receive Buffer Full Interrupt */

/* -------------------------------------------------------------------------------------------------------------------------
          UART_IIR                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_UART_IIR_FEND                    6            /*  FIFO Enabled */
#define BITP_UART_IIR_STAT                    1            /*  Interrupt Status */
#define BITP_UART_IIR_NIRQ                    0            /*  Interrupt Flag */
#define BITM_UART_IIR_FEND                   (_ADI_MSK_3(0x000000C0,0x000000C0U, uint16_t  ))    /*  FIFO Enabled */
#define BITM_UART_IIR_STAT                   (_ADI_MSK_3(0x0000000E,0x0000000EU, uint16_t  ))    /*  Interrupt Status */
#define BITM_UART_IIR_NIRQ                   (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Interrupt Flag */
#define ENUM_UART_IIR_STAT_EDSSI             (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  STAT: Modem status interrupt (Read MSR register to clear) */
#define ENUM_UART_IIR_STAT_ETBEI             (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  STAT: Transmit buffer empty interrupt (Write to Tx register or read IIR register to clear) */
#define ENUM_UART_IIR_STAT_ERBFI             (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  STAT: Receive buffer full interrupt (Read Rx register to clear) */
#define ENUM_UART_IIR_STAT_RLSI              (_ADI_MSK_3(0x00000006,0x00000006U, uint16_t  ))    /*  STAT: Receive line status interrupt (Read LSR register to clear) */
#define ENUM_UART_IIR_STAT_RFTOI             (_ADI_MSK_3(0x0000000C,0x0000000CU, uint16_t  ))    /*  STAT: Receive FIFO time-out interrupt (Read Rx register to clear) */

/* -------------------------------------------------------------------------------------------------------------------------
          UART_LCR                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_UART_LCR_BRK                     6            /*  Set Break */
#define BITP_UART_LCR_SP                      5            /*  Stick Parity */
#define BITP_UART_LCR_EPS                     4            /*  Parity Select */
#define BITP_UART_LCR_PEN                     3            /*  Parity Enable */
#define BITP_UART_LCR_STOP                    2            /*  Stop Bit */
#define BITP_UART_LCR_WLS                     0            /*  Word Length Select */
#define BITM_UART_LCR_BRK                    (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  Set Break */
#define BITM_UART_LCR_SP                     (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  Stick Parity */
#define BITM_UART_LCR_EPS                    (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Parity Select */
#define BITM_UART_LCR_PEN                    (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Parity Enable */
#define BITM_UART_LCR_STOP                   (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Stop Bit */
#define BITM_UART_LCR_WLS                    (_ADI_MSK_3(0x00000003,0x00000003U, uint16_t  ))    /*  Word Length Select */
#define ENUM_UART_LCR_PAR_NOTFORCED          (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  SP: Parity will not be forced based on Parity Select and Parity Enable bits. */
#define ENUM_UART_LCR_PAR_FORCED             (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  SP: Parity forced based on Parity Select and Parity Enable bits. */

/* -------------------------------------------------------------------------------------------------------------------------
          UART_MCR                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_UART_MCR_LOOPBACK                4            /*  Loopback Mode */
#define BITP_UART_MCR_OUT2                    3            /*  Output 2 */
#define BITP_UART_MCR_OUT1                    2            /*  Output 1 */
#define BITP_UART_MCR_RTS                     1            /*  Request to Send */
#define BITP_UART_MCR_DTR                     0            /*  Data Terminal Ready */
#define BITM_UART_MCR_LOOPBACK               (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Loopback Mode */
#define BITM_UART_MCR_OUT2                   (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Output 2 */
#define BITM_UART_MCR_OUT1                   (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Output 1 */
#define BITM_UART_MCR_RTS                    (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Request to Send */
#define BITM_UART_MCR_DTR                    (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Data Terminal Ready */

/* -------------------------------------------------------------------------------------------------------------------------
          UART_LSR                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_UART_LSR_FIFOERR                 7            /*  Rx FIFO Parity Error/Frame Error/Break Indication */
#define BITP_UART_LSR_TEMT                    6            /*  Transmit and Shift Register Empty Status */
#define BITP_UART_LSR_THRE                    5            /*  Transmit Register Empty */
#define BITP_UART_LSR_BI                      4            /*  Break Indicator */
#define BITP_UART_LSR_FE                      3            /*  Framing Error */
#define BITP_UART_LSR_PE                      2            /*  Parity Error */
#define BITP_UART_LSR_OE                      1            /*  Overrun Error */
#define BITP_UART_LSR_DR                      0            /*  Data Ready */
#define BITM_UART_LSR_FIFOERR                (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  Rx FIFO Parity Error/Frame Error/Break Indication */
#define BITM_UART_LSR_TEMT                   (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  Transmit and Shift Register Empty Status */
#define BITM_UART_LSR_THRE                   (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  Transmit Register Empty */
#define BITM_UART_LSR_BI                     (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Break Indicator */
#define BITM_UART_LSR_FE                     (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Framing Error */
#define BITM_UART_LSR_PE                     (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Parity Error */
#define BITM_UART_LSR_OE                     (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Overrun Error */
#define BITM_UART_LSR_DR                     (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Data Ready */

/* -------------------------------------------------------------------------------------------------------------------------
          UART_MSR                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_UART_MSR_DCD                     7            /*  Data Carrier Detect */
#define BITP_UART_MSR_RI                      6            /*  Ring Indicator */
#define BITP_UART_MSR_DSR                     5            /*  Data Set Ready */
#define BITP_UART_MSR_CTS                     4            /*  Clear to Send */
#define BITP_UART_MSR_DDCD                    3            /*  Delta DCD */
#define BITP_UART_MSR_TERI                    2            /*  Trailing Edge RI */
#define BITP_UART_MSR_DDSR                    1            /*  Delta DSR */
#define BITP_UART_MSR_DCTS                    0            /*  Delta CTS */
#define BITM_UART_MSR_DCD                    (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  Data Carrier Detect */
#define BITM_UART_MSR_RI                     (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  Ring Indicator */
#define BITM_UART_MSR_DSR                    (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  Data Set Ready */
#define BITM_UART_MSR_CTS                    (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Clear to Send */
#define BITM_UART_MSR_DDCD                   (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Delta DCD */
#define BITM_UART_MSR_TERI                   (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Trailing Edge RI */
#define BITM_UART_MSR_DDSR                   (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Delta DSR */
#define BITM_UART_MSR_DCTS                   (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Delta CTS */

/* -------------------------------------------------------------------------------------------------------------------------
          UART_SCR                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_UART_SCR_SCR                     0            /*  Scratch */
#define BITM_UART_SCR_SCR                    (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Scratch */

/* -------------------------------------------------------------------------------------------------------------------------
          UART_FCR                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_UART_FCR_RFTRIG                  6            /*  Rx FIFO Trigger Level */
#define BITP_UART_FCR_FDMAMD                  3            /*  FIFO DMA Mode */
#define BITP_UART_FCR_TFCLR                   2            /*  Clear Tx FIFO */
#define BITP_UART_FCR_RFCLR                   1            /*  Clear Rx FIFO */
#define BITP_UART_FCR_FIFOEN                  0            /*  FIFO Enable as to Work in 16550 Mode */
#define BITM_UART_FCR_RFTRIG                 (_ADI_MSK_3(0x000000C0,0x000000C0U, uint16_t  ))    /*  Rx FIFO Trigger Level */
#define BITM_UART_FCR_FDMAMD                 (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  FIFO DMA Mode */
#define BITM_UART_FCR_TFCLR                  (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Clear Tx FIFO */
#define BITM_UART_FCR_RFCLR                  (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Clear Rx FIFO */
#define BITM_UART_FCR_FIFOEN                 (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  FIFO Enable as to Work in 16550 Mode */
#define ENUM_UART_FCR_MODE0                  (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  FDMAMD: In DMA mode 0, RX DMA request will be asserted whenever there's data in RBR or RX FIFO and de-assert whenever RBR or RX FIFO is empty; TX DMA request will be asserted whenever THR or TX FIFO is empty and de-assert whenever data written to. */
#define ENUM_UART_FCR_MODE1                  (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  FDMAMD: in DMA mode 1, RX DMA request will be asserted whenever RX FIFO trig level or time out reached and de-assert thereafter when RX FIFO is empty; TX DMA request will be asserted whenever TX FIFO is empty and de-assert thereafter when TX FIFO is completely filled up full. */

/* -------------------------------------------------------------------------------------------------------------------------
          UART_FBR                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_UART_FBR_FBEN                   15            /*  Fractional Baud Rate Generator Enable */
#define BITP_UART_FBR_DIVM                   11            /*  Fractional Baud Rate M Divide Bits 1 to 3 */
#define BITP_UART_FBR_DIVN                    0            /*  Fractional Baud Rate N Divide Bits 0 to 2047 */
#define BITM_UART_FBR_FBEN                   (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  Fractional Baud Rate Generator Enable */
#define BITM_UART_FBR_DIVM                   (_ADI_MSK_3(0x00001800,0x00001800U, uint16_t  ))    /*  Fractional Baud Rate M Divide Bits 1 to 3 */
#define BITM_UART_FBR_DIVN                   (_ADI_MSK_3(0x000007FF,0x000007FFU, uint16_t  ))    /*  Fractional Baud Rate N Divide Bits 0 to 2047 */

/* -------------------------------------------------------------------------------------------------------------------------
          UART_DIV                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_UART_DIV_DIV                     0            /*  Baud Rate Divider */
#define BITM_UART_DIV_DIV                    (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Baud Rate Divider */

/* -------------------------------------------------------------------------------------------------------------------------
          UART_LCR2                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_UART_LCR2_OSR                    0            /*  Over Sample Rate */
#define BITM_UART_LCR2_OSR                   (_ADI_MSK_3(0x00000003,0x00000003U, uint16_t  ))    /*  Over Sample Rate */

/* -------------------------------------------------------------------------------------------------------------------------
          UART_CTL                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_UART_CTL_REV                     8            /*  UART Revision ID */
#define BITP_UART_CTL_RXINV                   4            /*  Invert Receiver Line */
#define BITP_UART_CTL_FORCECLK                1            /*  Force UCLK on */
#define BITM_UART_CTL_REV                    (_ADI_MSK_3(0x0000FF00,0x0000FF00U, uint16_t  ))    /*  UART Revision ID */
#define BITM_UART_CTL_RXINV                  (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Invert Receiver Line */
#define BITM_UART_CTL_FORCECLK               (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Force UCLK on */
#define ENUM_UART_CTL_NOTINV_RX              (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  RXINV: Don't invert receiver line (idling high). */
#define ENUM_UART_CTL_INV_RX                 (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  RXINV: Invert receiver line (idling low). */

/* -------------------------------------------------------------------------------------------------------------------------
          UART_RFC                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_UART_RFC_RFC                     0            /*  Current Rx FIFO Data Bytes */
#define BITM_UART_RFC_RFC                    (_ADI_MSK_3(0x0000001F,0x0000001FU, uint16_t  ))    /*  Current Rx FIFO Data Bytes */

/* -------------------------------------------------------------------------------------------------------------------------
          UART_TFC                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_UART_TFC_TFC                     0            /*  Current Tx FIFO Data Bytes */
#define BITM_UART_TFC_TFC                    (_ADI_MSK_3(0x0000001F,0x0000001FU, uint16_t  ))    /*  Current Tx FIFO Data Bytes */

/* -------------------------------------------------------------------------------------------------------------------------
          UART_RSC                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_UART_RSC_DISTX                   3            /*  Hold off Tx When Receiving */
#define BITP_UART_RSC_DISRX                   2            /*  Disable Rx When Transmitting */
#define BITP_UART_RSC_OENSP                   1            /*  SOUT_EN De-assert Before Full Stop Bit(s) */
#define BITP_UART_RSC_OENP                    0            /*  SOUT_EN Polarity */
#define BITM_UART_RSC_DISTX                  (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Hold off Tx When Receiving */
#define BITM_UART_RSC_DISRX                  (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Disable Rx When Transmitting */
#define BITM_UART_RSC_OENSP                  (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  SOUT_EN De-assert Before Full Stop Bit(s) */
#define BITM_UART_RSC_OENP                   (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  SOUT_EN Polarity */

/* -------------------------------------------------------------------------------------------------------------------------
          UART_ACR                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_UART_ACR_EEC                     8            /*  Ending Edge Count */
#define BITP_UART_ACR_SEC                     4            /*  Starting Edge Count */
#define BITP_UART_ACR_TOIEN                   2            /*  Enable Time-out Interrupt */
#define BITP_UART_ACR_DNIEN                   1            /*  Enable Done Interrupt */
#define BITP_UART_ACR_ABE                     0            /*  Auto Baud Enable */
#define BITM_UART_ACR_EEC                    (_ADI_MSK_3(0x00000F00,0x00000F00U, uint16_t  ))    /*  Ending Edge Count */
#define BITM_UART_ACR_SEC                    (_ADI_MSK_3(0x00000070,0x00000070U, uint16_t  ))    /*  Starting Edge Count */
#define BITM_UART_ACR_TOIEN                  (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Enable Time-out Interrupt */
#define BITM_UART_ACR_DNIEN                  (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Enable Done Interrupt */
#define BITM_UART_ACR_ABE                    (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Auto Baud Enable */

/* -------------------------------------------------------------------------------------------------------------------------
          UART_ASRL                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_UART_ASRL_CNT                    4            /*  Auto Baud Counter Value */
#define BITP_UART_ASRL_NEETO                  3            /*  Timed Out Due to No Valid Ending Edge Found */
#define BITP_UART_ASRL_NSETO                  2            /*  Timed Out Due to No Valid Start Edge Found */
#define BITP_UART_ASRL_BRKTO                  1            /*  Timed Out Due to Long Time Break Condition */
#define BITP_UART_ASRL_DONE                   0            /*  Auto Baud Done Successfully */
#define BITM_UART_ASRL_CNT                   (_ADI_MSK_3(0x0000FFF0,0x0000FFF0U, uint16_t  ))    /*  Auto Baud Counter Value */
#define BITM_UART_ASRL_NEETO                 (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Timed Out Due to No Valid Ending Edge Found */
#define BITM_UART_ASRL_NSETO                 (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Timed Out Due to No Valid Start Edge Found */
#define BITM_UART_ASRL_BRKTO                 (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Timed Out Due to Long Time Break Condition */
#define BITM_UART_ASRL_DONE                  (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Auto Baud Done Successfully */

/* -------------------------------------------------------------------------------------------------------------------------
          UART_ASRH                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_UART_ASRH_CNT                    0            /*  Auto Baud Counter Value */
#define BITM_UART_ASRH_CNT                   (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Auto Baud Counter Value */


/* ============================================================================================================================
        Beeper Driver
   ============================================================================================================================ */

/* ============================================================================================================================
        BEEP0
   ============================================================================================================================ */
#define REG_BEEP0_CFG                        0x40005C00            /*  BEEP0 Beeper Configuration */
#define REG_BEEP0_STAT                       0x40005C04            /*  BEEP0 Beeper Status */
#define REG_BEEP0_TONEA                      0x40005C08            /*  BEEP0 Tone A Data */
#define REG_BEEP0_TONEB                      0x40005C0C            /*  BEEP0 Tone B Data */

/* ============================================================================================================================
        BEEP Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          BEEP_CFG                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_BEEP_CFG_SEQATENDIRQ            15            /*  Sequence End IRQ */
#define BITP_BEEP_CFG_SEQNEARENDIRQ          14            /*  Sequence 1 Cycle from End IRQ */
#define BITP_BEEP_CFG_BENDIRQ                13            /*  Tone B End IRQ */
#define BITP_BEEP_CFG_BSTARTIRQ              12            /*  Tone B Start IRQ */
#define BITP_BEEP_CFG_AENDIRQ                11            /*  Tone A End IRQ */
#define BITP_BEEP_CFG_ASTARTIRQ              10            /*  Tone A Start IRQ */
#define BITP_BEEP_CFG_EN                      8            /*  Beeper Enable */
#define BITP_BEEP_CFG_SEQREPEAT               0            /*  Beeper Sequence Repeat Value */
#define BITM_BEEP_CFG_SEQATENDIRQ            (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  Sequence End IRQ */
#define BITM_BEEP_CFG_SEQNEARENDIRQ          (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  Sequence 1 Cycle from End IRQ */
#define BITM_BEEP_CFG_BENDIRQ                (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  Tone B End IRQ */
#define BITM_BEEP_CFG_BSTARTIRQ              (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  Tone B Start IRQ */
#define BITM_BEEP_CFG_AENDIRQ                (_ADI_MSK_3(0x00000800,0x00000800U, uint16_t  ))    /*  Tone A End IRQ */
#define BITM_BEEP_CFG_ASTARTIRQ              (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Tone A Start IRQ */
#define BITM_BEEP_CFG_EN                     (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Beeper Enable */
#define BITM_BEEP_CFG_SEQREPEAT              (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Beeper Sequence Repeat Value */

/* -------------------------------------------------------------------------------------------------------------------------
          BEEP_STAT                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_BEEP_STAT_SEQENDED              15            /*  Sequencer Has Ended */
#define BITP_BEEP_STAT_SEQNEAREND            14            /*  Sequencer Last Tone-pair Has Started */
#define BITP_BEEP_STAT_BENDED                13            /*  Tone B Has Ended */
#define BITP_BEEP_STAT_BSTARTED              12            /*  Tone B Has Started */
#define BITP_BEEP_STAT_AENDED                11            /*  Tone A Has Ended */
#define BITP_BEEP_STAT_ASTARTED              10            /*  Tone A Has Started */
#define BITP_BEEP_STAT_BUSY                   8            /*  Beeper is Busy */
#define BITP_BEEP_STAT_SEQREMAIN              0            /*  Remaining Tone-pair Iterations to Play in Sequence Mode */
#define BITM_BEEP_STAT_SEQENDED              (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  Sequencer Has Ended */
#define BITM_BEEP_STAT_SEQNEAREND            (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  Sequencer Last Tone-pair Has Started */
#define BITM_BEEP_STAT_BENDED                (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  Tone B Has Ended */
#define BITM_BEEP_STAT_BSTARTED              (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  Tone B Has Started */
#define BITM_BEEP_STAT_AENDED                (_ADI_MSK_3(0x00000800,0x00000800U, uint16_t  ))    /*  Tone A Has Ended */
#define BITM_BEEP_STAT_ASTARTED              (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Tone A Has Started */
#define BITM_BEEP_STAT_BUSY                  (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Beeper is Busy */
#define BITM_BEEP_STAT_SEQREMAIN             (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Remaining Tone-pair Iterations to Play in Sequence Mode */

/* -------------------------------------------------------------------------------------------------------------------------
          BEEP_TONEA                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_BEEP_TONEA_DIS                  15            /*  Output Disable */
#define BITP_BEEP_TONEA_FREQ                  8            /*  Tone Frequency */
#define BITP_BEEP_TONEA_DUR                   0            /*  Tone Duration */
#define BITM_BEEP_TONEA_DIS                  (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  Output Disable */
#define BITM_BEEP_TONEA_FREQ                 (_ADI_MSK_3(0x00007F00,0x00007F00U, uint16_t  ))    /*  Tone Frequency */
#define BITM_BEEP_TONEA_DUR                  (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Tone Duration */

/* -------------------------------------------------------------------------------------------------------------------------
          BEEP_TONEB                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_BEEP_TONEB_DIS                  15            /*  Output Disable */
#define BITP_BEEP_TONEB_FREQ                  8            /*  Tone Frequency */
#define BITP_BEEP_TONEB_DUR                   0            /*  Tone Duration */
#define BITM_BEEP_TONEB_DIS                  (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  Output Disable */
#define BITM_BEEP_TONEB_FREQ                 (_ADI_MSK_3(0x00007F00,0x00007F00U, uint16_t  ))    /*  Tone Frequency */
#define BITM_BEEP_TONEB_DUR                  (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Tone Duration */


/* ============================================================================================================================
        
   ============================================================================================================================ */

/* ============================================================================================================================
        ADC0
   ============================================================================================================================ */
#define REG_ADC0_CFG                         0x40007000            /*  ADC0 ADC Configuration */
#define REG_ADC0_PWRUP                       0x40007004            /*  ADC0 ADC Power-up Time */
#define REG_ADC0_CAL_WORD                    0x40007008            /*  ADC0 Calibration Word */
#define REG_ADC0_CNV_CFG                     0x4000700C            /*  ADC0 ADC Conversion Configuration */
#define REG_ADC0_CNV_TIME                    0x40007010            /*  ADC0 ADC Conversion Time */
#define REG_ADC0_AVG_CFG                     0x40007014            /*  ADC0 Averaging Configuration */
#define REG_ADC0_IRQ_EN                      0x40007020            /*  ADC0 Interrupt Enable */
#define REG_ADC0_STAT                        0x40007024            /*  ADC0 ADC Status */
#define REG_ADC0_OVF                         0x40007028            /*  ADC0 Overflow of Output Registers */
#define REG_ADC0_ALERT                       0x4000702C            /*  ADC0 Alert Indication */
#define REG_ADC0_CH0_OUT                     0x40007030            /*  ADC0 Conversion Result Channel 0 */
#define REG_ADC0_CH1_OUT                     0x40007034            /*  ADC0 Conversion Result Channel 1 */
#define REG_ADC0_CH2_OUT                     0x40007038            /*  ADC0 Conversion Result Channel 2 */
#define REG_ADC0_CH3_OUT                     0x4000703C            /*  ADC0 Conversion Result Channel 3 */
#define REG_ADC0_CH4_OUT                     0x40007040            /*  ADC0 Conversion Result Channel 4 */
#define REG_ADC0_CH5_OUT                     0x40007044            /*  ADC0 Conversion Result Channel 5 */
#define REG_ADC0_CH6_OUT                     0x40007048            /*  ADC0 Conversion Result Channel 6 */
#define REG_ADC0_CH7_OUT                     0x4000704C            /*  ADC0 Conversion Result Channel 7 */
#define REG_ADC0_BAT_OUT                     0x40007050            /*  ADC0 Battery Monitoring Result */
#define REG_ADC0_TMP_OUT                     0x40007054            /*  ADC0 Temperature Result */
#define REG_ADC0_TMP2_OUT                    0x40007058            /*  ADC0 Temperature Result 2 */
#define REG_ADC0_DMA_OUT                     0x4000705C            /*  ADC0 DMA Output Register */
#define REG_ADC0_LIM0_LO                     0x40007060            /*  ADC0 Channel 0 Low Limit */
#define REG_ADC0_LIM0_HI                     0x40007064            /*  ADC0 Channel 0 High Limit */
#define REG_ADC0_HYS0                        0x40007068            /*  ADC0 Channel 0 Hysteresis */
#define REG_ADC0_LIM1_LO                     0x40007070            /*  ADC0 Channel 1 Low Limit */
#define REG_ADC0_LIM1_HI                     0x40007074            /*  ADC0 Channel 1 High Limit */
#define REG_ADC0_HYS1                        0x40007078            /*  ADC0 Channel 1 Hysteresis */
#define REG_ADC0_LIM2_LO                     0x40007080            /*  ADC0 Channel 2 Low Limit */
#define REG_ADC0_LIM2_HI                     0x40007084            /*  ADC0 Channel 2 High Limit */
#define REG_ADC0_HYS2                        0x40007088            /*  ADC0 Channel 2 Hysteresis */
#define REG_ADC0_LIM3_LO                     0x40007090            /*  ADC0 Channel 3 Low Limit */
#define REG_ADC0_LIM3_HI                     0x40007094            /*  ADC0 Channel 3 High Limit */
#define REG_ADC0_HYS3                        0x40007098            /*  ADC0 Channel 3 Hysteresis */
#define REG_ADC0_CFG1                        0x400070C0            /*  ADC0 Reference Buffer Low Power Mode */

/* ============================================================================================================================
        ADC Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          ADC_CFG                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_CFG_VREFVBAT_DEL            10            /*  Set to 1 after minimum delay of 700 us from VREFBAT field being set to 1 */
#define BITP_ADC_CFG_FAST_DISCH               9            /*  For fast switchover of Vref from 2.5 V to 1.25 V */
#define BITP_ADC_CFG_TMPEN                    8            /*  To power up temperature sensor */
#define BITP_ADC_CFG_SINKEN                   7            /*  To enable additional 50 uA sink current capability @1.25 V, 100 uA current capability @2.5 V */
#define BITP_ADC_CFG_RST                      6            /*  Resets internal buffers and registers when high */
#define BITP_ADC_CFG_STARTCAL                 5            /*  To start a new offset calibration cycle */
#define BITP_ADC_CFG_EN                       4            /*  To enable ADC subsystem */
#define BITP_ADC_CFG_VREFVBAT                 3            /*  VRef VBAT */
#define BITP_ADC_CFG_REFBUFEN                 2            /*  To enable internal reference buffer */
#define BITP_ADC_CFG_VREFSEL                  1            /*  To select Vref as 1.25 V or 2.5 V */
#define BITP_ADC_CFG_PWRUP                    0            /*  Powering up ADC */
#define BITM_ADC_CFG_VREFVBAT_DEL            (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Set to 1 after minimum delay of 700 us from VREFBAT field being set to 1 */
#define BITM_ADC_CFG_FAST_DISCH              (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  For fast switchover of Vref from 2.5 V to 1.25 V */
#define BITM_ADC_CFG_TMPEN                   (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  To power up temperature sensor */
#define BITM_ADC_CFG_SINKEN                  (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  To enable additional 50 uA sink current capability @1.25 V, 100 uA current capability @2.5 V */
#define BITM_ADC_CFG_RST                     (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  Resets internal buffers and registers when high */
#define BITM_ADC_CFG_STARTCAL                (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  To start a new offset calibration cycle */
#define BITM_ADC_CFG_EN                      (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  To enable ADC subsystem */
#define BITM_ADC_CFG_VREFVBAT                (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  VRef VBAT */
#define BITM_ADC_CFG_REFBUFEN                (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  To enable internal reference buffer */
#define BITM_ADC_CFG_VREFSEL                 (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  To select Vref as 1.25 V or 2.5 V */
#define BITM_ADC_CFG_PWRUP                   (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Powering up ADC */
#define ENUM_ADC_CFG_EXT_REF                 (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  REFBUFEN: External reference is used */
#define ENUM_ADC_CFG_BUF_REF                 (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  REFBUFEN: Reference buffer is enabled */
#define ENUM_ADC_CFG_V_2P5                   (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  VREFSEL: Vref = 2.5 V */
#define ENUM_ADC_CFG_V_1P25                  (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  VREFSEL: Vref = 1.25 V */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_PWRUP                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_PWRUP_WAIT                   0            /*  Program this count to generate 20us wait time with respect to the PCLK frequency */
#define BITM_ADC_PWRUP_WAIT                  (_ADI_MSK_3(0x000007FF,0x000007FFU, uint16_t  ))    /*  Program this count to generate 20us wait time with respect to the PCLK frequency */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_CAL_WORD                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_CAL_WORD_VALUE               0            /*  Offset calibration word */
#define BITM_ADC_CAL_WORD_VALUE              (_ADI_MSK_3(0x0000007F,0x0000007FU, uint16_t  ))    /*  Offset calibration word */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_CNV_CFG                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_CNV_CFG_MULTI               15            /*  Set to start multiple conversions */
#define BITP_ADC_CNV_CFG_SINGLE              14            /*  Set to start single conversion */
#define BITP_ADC_CNV_CFG_DMAEN               13            /*  To enable DMA channel */
#define BITP_ADC_CNV_CFG_AUTOMODE            12            /*  To enable auto mode */
#define BITP_ADC_CNV_CFG_TMP2                10            /*  To select temperature measurement 2 */
#define BITP_ADC_CNV_CFG_TMP                  9            /*  To select temperature measurement 1 */
#define BITP_ADC_CNV_CFG_BAT                  8            /*  To enable battery monitoring */
#define BITP_ADC_CNV_CFG_SEL                  0            /*  To select channel(s) to convert */
#define BITM_ADC_CNV_CFG_MULTI               (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  Set to start multiple conversions */
#define BITM_ADC_CNV_CFG_SINGLE              (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  Set to start single conversion */
#define BITM_ADC_CNV_CFG_DMAEN               (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  To enable DMA channel */
#define BITM_ADC_CNV_CFG_AUTOMODE            (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  To enable auto mode */
#define BITM_ADC_CNV_CFG_TMP2                (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  To select temperature measurement 2 */
#define BITM_ADC_CNV_CFG_TMP                 (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  To select temperature measurement 1 */
#define BITM_ADC_CNV_CFG_BAT                 (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  To enable battery monitoring */
#define BITM_ADC_CNV_CFG_SEL                 (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  To select channel(s) to convert */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_CNV_TIME                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_CNV_TIME_DLY                 8            /*  Delay between two consecutive conversions in terms of number of ACLK cycles */
#define BITP_ADC_CNV_TIME_SAMPTIME            0            /*  Number of clock cycles (ACLK) required for sampling */
#define BITM_ADC_CNV_TIME_DLY                (_ADI_MSK_3(0x0000FF00,0x0000FF00U, uint16_t  ))    /*  Delay between two consecutive conversions in terms of number of ACLK cycles */
#define BITM_ADC_CNV_TIME_SAMPTIME           (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Number of clock cycles (ACLK) required for sampling */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_AVG_CFG                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_AVG_CFG_EN                  15            /*  To enable averaging on Channels enabled in enable register */
#define BITP_ADC_AVG_CFG_OS                  14            /*  Enable oversampling */
#define BITP_ADC_AVG_CFG_FACTOR               0            /*  Program averaging factor for averaging enabled channels (1-256) */
#define BITM_ADC_AVG_CFG_EN                  (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  To enable averaging on Channels enabled in enable register */
#define BITM_ADC_AVG_CFG_OS                  (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  Enable oversampling */
#define BITM_ADC_AVG_CFG_FACTOR              (_ADI_MSK_3(0x000000FF,0x000000FFU, uint16_t  ))    /*  Program averaging factor for averaging enabled channels (1-256) */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_IRQ_EN                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_IRQ_EN_RDY                  13            /*  Set to enable interrupt when ADC is ready to convert */
#define BITP_ADC_IRQ_EN_ALERT                12            /*  Set to enable interrupt on crossing lower or higher limit */
#define BITP_ADC_IRQ_EN_OVF                  11            /*  Set to enable interrupt in case of overflow */
#define BITP_ADC_IRQ_EN_CALDONE              10            /*  Set it to enable interrupt for calibration done */
#define BITP_ADC_IRQ_EN_CNVDONE               0            /*  Set it to enable interrupt after conversion is done */
#define BITM_ADC_IRQ_EN_RDY                  (_ADI_MSK_3(0x00002000,0x00002000U, uint16_t  ))    /*  Set to enable interrupt when ADC is ready to convert */
#define BITM_ADC_IRQ_EN_ALERT                (_ADI_MSK_3(0x00001000,0x00001000U, uint16_t  ))    /*  Set to enable interrupt on crossing lower or higher limit */
#define BITM_ADC_IRQ_EN_OVF                  (_ADI_MSK_3(0x00000800,0x00000800U, uint16_t  ))    /*  Set to enable interrupt in case of overflow */
#define BITM_ADC_IRQ_EN_CALDONE              (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Set it to enable interrupt for calibration done */
#define BITM_ADC_IRQ_EN_CNVDONE              (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Set it to enable interrupt after conversion is done */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_STAT                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_STAT_RDY                    15            /*  Indicates ADC is ready to start converting, when using external reference buffer */
#define BITP_ADC_STAT_CALDONE                14            /*  Indicates calibration is done */
#define BITP_ADC_STAT_TMP2DONE               10            /*  Indicates conversion is done for temperature sensing 2 */
#define BITP_ADC_STAT_TMPDONE                 9            /*  Indicates conversion is done for temperature sensing */
#define BITP_ADC_STAT_BATDONE                 8            /*  Indicates conversion done for battery monitoring */
#define BITP_ADC_STAT_DONE7                   7            /*  Indicates conversion done on Channel 7 */
#define BITP_ADC_STAT_DONE6                   6            /*  Indicates conversion done on Channel 6 */
#define BITP_ADC_STAT_DONE5                   5            /*  Indicates conversion done on Channel 5 */
#define BITP_ADC_STAT_DONE4                   4            /*  Indicates conversion done on Channel 4 */
#define BITP_ADC_STAT_DONE3                   3            /*  Indicates conversion done on Channel 3 */
#define BITP_ADC_STAT_DONE2                   2            /*  Indicates conversion done on Channel 2 */
#define BITP_ADC_STAT_DONE1                   1            /*  Indicates conversion done on Channel 1 */
#define BITP_ADC_STAT_DONE0                   0            /*  Indicates conversion done on Channel 0 */
#define BITM_ADC_STAT_RDY                    (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  Indicates ADC is ready to start converting, when using external reference buffer */
#define BITM_ADC_STAT_CALDONE                (_ADI_MSK_3(0x00004000,0x00004000U, uint16_t  ))    /*  Indicates calibration is done */
#define BITM_ADC_STAT_TMP2DONE               (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Indicates conversion is done for temperature sensing 2 */
#define BITM_ADC_STAT_TMPDONE                (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  Indicates conversion is done for temperature sensing */
#define BITM_ADC_STAT_BATDONE                (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Indicates conversion done for battery monitoring */
#define BITM_ADC_STAT_DONE7                  (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  Indicates conversion done on Channel 7 */
#define BITM_ADC_STAT_DONE6                  (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  Indicates conversion done on Channel 6 */
#define BITM_ADC_STAT_DONE5                  (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  Indicates conversion done on Channel 5 */
#define BITM_ADC_STAT_DONE4                  (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Indicates conversion done on Channel 4 */
#define BITM_ADC_STAT_DONE3                  (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Indicates conversion done on Channel 3 */
#define BITM_ADC_STAT_DONE2                  (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Indicates conversion done on Channel 2 */
#define BITM_ADC_STAT_DONE1                  (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Indicates conversion done on Channel 1 */
#define BITM_ADC_STAT_DONE0                  (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Indicates conversion done on Channel 0 */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_OVF                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_OVF_TMP2                    10            /*  Indicates overflow in temperature 2 output register */
#define BITP_ADC_OVF_TMP                      9            /*  Indicates overflow in temperature output register */
#define BITP_ADC_OVF_BAT                      8            /*  Indicates overflow in battery monitoring output register */
#define BITP_ADC_OVF_CH7                      7            /*  Indicates overflow in channel 7 output register */
#define BITP_ADC_OVF_CH6                      6            /*  Indicates overflow in channel 6 output register */
#define BITP_ADC_OVF_CH5                      5            /*  Indicates overflow in channel 5 output register */
#define BITP_ADC_OVF_CH4                      4            /*  Indicates overflow in channel 4 output register */
#define BITP_ADC_OVF_CH3                      3            /*  Indicates overflow in channel 3 output register */
#define BITP_ADC_OVF_CH2                      2            /*  Indicates overflow in channel 2 output register */
#define BITP_ADC_OVF_CH1                      1            /*  Indicates overflow in channel 1 output register */
#define BITP_ADC_OVF_CH0                      0            /*  Indicates overflow in channel 0 output register */
#define BITM_ADC_OVF_TMP2                    (_ADI_MSK_3(0x00000400,0x00000400U, uint16_t  ))    /*  Indicates overflow in temperature 2 output register */
#define BITM_ADC_OVF_TMP                     (_ADI_MSK_3(0x00000200,0x00000200U, uint16_t  ))    /*  Indicates overflow in temperature output register */
#define BITM_ADC_OVF_BAT                     (_ADI_MSK_3(0x00000100,0x00000100U, uint16_t  ))    /*  Indicates overflow in battery monitoring output register */
#define BITM_ADC_OVF_CH7                     (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  Indicates overflow in channel 7 output register */
#define BITM_ADC_OVF_CH6                     (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  Indicates overflow in channel 6 output register */
#define BITM_ADC_OVF_CH5                     (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  Indicates overflow in channel 5 output register */
#define BITM_ADC_OVF_CH4                     (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Indicates overflow in channel 4 output register */
#define BITM_ADC_OVF_CH3                     (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Indicates overflow in channel 3 output register */
#define BITM_ADC_OVF_CH2                     (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Indicates overflow in channel 2 output register */
#define BITM_ADC_OVF_CH1                     (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Indicates overflow in channel 1 output register */
#define BITM_ADC_OVF_CH0                     (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Indicates overflow in channel 0 output register */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_ALERT                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ALERT_LO3                    7            /*  Channel 3 Low alert status */
#define BITP_ADC_ALERT_HI3                    6            /*  Channel 3 High alert status */
#define BITP_ADC_ALERT_LO2                    5            /*  Channel 2 Low alert status */
#define BITP_ADC_ALERT_HI2                    4            /*  Channel 2 High alert status */
#define BITP_ADC_ALERT_LO1                    3            /*  Channel 1 Low alert status */
#define BITP_ADC_ALERT_HI1                    2            /*  Channel 1 High alert status */
#define BITP_ADC_ALERT_LO0                    1            /*  Channel 0 Low alert status */
#define BITP_ADC_ALERT_HI0                    0            /*  Channel 0 High alert status */
#define BITM_ADC_ALERT_LO3                   (_ADI_MSK_3(0x00000080,0x00000080U, uint16_t  ))    /*  Channel 3 Low alert status */
#define BITM_ADC_ALERT_HI3                   (_ADI_MSK_3(0x00000040,0x00000040U, uint16_t  ))    /*  Channel 3 High alert status */
#define BITM_ADC_ALERT_LO2                   (_ADI_MSK_3(0x00000020,0x00000020U, uint16_t  ))    /*  Channel 2 Low alert status */
#define BITM_ADC_ALERT_HI2                   (_ADI_MSK_3(0x00000010,0x00000010U, uint16_t  ))    /*  Channel 2 High alert status */
#define BITM_ADC_ALERT_LO1                   (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Channel 1 Low alert status */
#define BITM_ADC_ALERT_HI1                   (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  Channel 1 High alert status */
#define BITM_ADC_ALERT_LO0                   (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Channel 0 Low alert status */
#define BITM_ADC_ALERT_HI0                   (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Channel 0 High alert status */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_CH0_OUT                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_CH0_OUT_RESULT               0            /*  Conversion result of channel 0 is stored here */
#define BITM_ADC_CH0_OUT_RESULT              (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Conversion result of channel 0 is stored here */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_CH1_OUT                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_CH1_OUT_RESULT               0            /*  Conversion result of channel 1 is stored here */
#define BITM_ADC_CH1_OUT_RESULT              (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Conversion result of channel 1 is stored here */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_CH2_OUT                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_CH2_OUT_RESULT               0            /*  Conversion result of channel 2 is stored here */
#define BITM_ADC_CH2_OUT_RESULT              (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Conversion result of channel 2 is stored here */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_CH3_OUT                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_CH3_OUT_RESULT               0            /*  Conversion result of channel 3 is stored here */
#define BITM_ADC_CH3_OUT_RESULT              (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Conversion result of channel 3 is stored here */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_CH4_OUT                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_CH4_OUT_RESULT               0            /*  Conversion result of channel 4 is stored here */
#define BITM_ADC_CH4_OUT_RESULT              (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Conversion result of channel 4 is stored here */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_CH5_OUT                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_CH5_OUT_RESULT               0            /*  Conversion result of channel 5 is stored here */
#define BITM_ADC_CH5_OUT_RESULT              (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Conversion result of channel 5 is stored here */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_CH6_OUT                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_CH6_OUT_RESULT               0            /*  Conversion result of channel 6 is stored here */
#define BITM_ADC_CH6_OUT_RESULT              (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Conversion result of channel 6 is stored here */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_CH7_OUT                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_CH7_OUT_RESULT               0            /*  Conversion result of channel 7 is stored here */
#define BITM_ADC_CH7_OUT_RESULT              (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Conversion result of channel 7 is stored here */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_BAT_OUT                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_BAT_OUT_RESULT               0            /*  Conversion result of battery monitoring is stored here */
#define BITM_ADC_BAT_OUT_RESULT              (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Conversion result of battery monitoring is stored here */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_TMP_OUT                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_TMP_OUT_RESULT               0            /*  Conversion result of Temperature measurement 1 is stored here */
#define BITM_ADC_TMP_OUT_RESULT              (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Conversion result of Temperature measurement 1 is stored here */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_TMP2_OUT                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_TMP2_OUT_RESULT              0            /*  Conversion result of Temperature measurement 2 is stored here */
#define BITM_ADC_TMP2_OUT_RESULT             (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Conversion result of Temperature measurement 2 is stored here */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_DMA_OUT                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_DMA_OUT_RESULT               0            /*  Register to store conversion result for DMA */
#define BITM_ADC_DMA_OUT_RESULT              (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Register to store conversion result for DMA */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_LIM0_LO                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_LIM0_LO_EN                  15            /*  To enable low limit comparison on Channel 0 */
#define BITP_ADC_LIM0_LO_VALUE                0            /*  Low limit value for channel 0 */
#define BITM_ADC_LIM0_LO_EN                  (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  To enable low limit comparison on Channel 0 */
#define BITM_ADC_LIM0_LO_VALUE               (_ADI_MSK_3(0x00000FFF,0x00000FFFU, uint16_t  ))    /*  Low limit value for channel 0 */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_LIM0_HI                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_LIM0_HI_EN                  15            /*  To enable high limit comparison on Channel 0 */
#define BITP_ADC_LIM0_HI_VALUE                0            /*  High limit value for channel 0 */
#define BITM_ADC_LIM0_HI_EN                  (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  To enable high limit comparison on Channel 0 */
#define BITM_ADC_LIM0_HI_VALUE               (_ADI_MSK_3(0x00000FFF,0x00000FFFU, uint16_t  ))    /*  High limit value for channel 0 */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_HYS0                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_HYS0_EN                     15            /*  To enable hysteresis for comparison on Channel 0 */
#define BITP_ADC_HYS0_MONCYC                 12            /*  Program number of conversion cycles to monitor channel 0 before raising alert */
#define BITP_ADC_HYS0_VALUE                   0            /*  Hysteresis value for Channel 0 */
#define BITM_ADC_HYS0_EN                     (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  To enable hysteresis for comparison on Channel 0 */
#define BITM_ADC_HYS0_MONCYC                 (_ADI_MSK_3(0x00007000,0x00007000U, uint16_t  ))    /*  Program number of conversion cycles to monitor channel 0 before raising alert */
#define BITM_ADC_HYS0_VALUE                  (_ADI_MSK_3(0x000001FF,0x000001FFU, uint16_t  ))    /*  Hysteresis value for Channel 0 */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_LIM1_LO                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_LIM1_LO_EN                  15            /*  To enable low limit comparison on Channel 1 */
#define BITP_ADC_LIM1_LO_VALUE                0            /*  Low limit value for channel 1 */
#define BITM_ADC_LIM1_LO_EN                  (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  To enable low limit comparison on Channel 1 */
#define BITM_ADC_LIM1_LO_VALUE               (_ADI_MSK_3(0x00000FFF,0x00000FFFU, uint16_t  ))    /*  Low limit value for channel 1 */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_LIM1_HI                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_LIM1_HI_EN                  15            /*  To enable high limit comparison on Channel 1 */
#define BITP_ADC_LIM1_HI_VALUE                0            /*  High limit value for channel 1 */
#define BITM_ADC_LIM1_HI_EN                  (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  To enable high limit comparison on Channel 1 */
#define BITM_ADC_LIM1_HI_VALUE               (_ADI_MSK_3(0x00000FFF,0x00000FFFU, uint16_t  ))    /*  High limit value for channel 1 */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_HYS1                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_HYS1_EN                     15            /*  To enable hysteresis for comparison on Channel 1 */
#define BITP_ADC_HYS1_MONCYC                 12            /*  Program number of conversion cycles to monitor channel 1 before raising alert */
#define BITP_ADC_HYS1_VALUE                   0            /*  Hysteresis value for Channel 1 */
#define BITM_ADC_HYS1_EN                     (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  To enable hysteresis for comparison on Channel 1 */
#define BITM_ADC_HYS1_MONCYC                 (_ADI_MSK_3(0x00007000,0x00007000U, uint16_t  ))    /*  Program number of conversion cycles to monitor channel 1 before raising alert */
#define BITM_ADC_HYS1_VALUE                  (_ADI_MSK_3(0x000001FF,0x000001FFU, uint16_t  ))    /*  Hysteresis value for Channel 1 */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_LIM2_LO                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_LIM2_LO_EN                  15            /*  To enable low limit comparison on Channel 2 */
#define BITP_ADC_LIM2_LO_VALUE                0            /*  Low limit value for channel 2 */
#define BITM_ADC_LIM2_LO_EN                  (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  To enable low limit comparison on Channel 2 */
#define BITM_ADC_LIM2_LO_VALUE               (_ADI_MSK_3(0x00000FFF,0x00000FFFU, uint16_t  ))    /*  Low limit value for channel 2 */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_LIM2_HI                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_LIM2_HI_EN                  15            /*  To enable high limit comparison on Channel 2 */
#define BITP_ADC_LIM2_HI_VALUE                0            /*  High limit value for channel 2 */
#define BITM_ADC_LIM2_HI_EN                  (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  To enable high limit comparison on Channel 2 */
#define BITM_ADC_LIM2_HI_VALUE               (_ADI_MSK_3(0x00000FFF,0x00000FFFU, uint16_t  ))    /*  High limit value for channel 2 */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_HYS2                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_HYS2_EN                     15            /*  To enable hysteresis for comparison on Channel 2 */
#define BITP_ADC_HYS2_MONCYC                 12            /*  Program number of conversion cycles to monitor channel 2 before raising alert */
#define BITP_ADC_HYS2_VALUE                   0            /*  Hysteresis value for Channel 2 */
#define BITM_ADC_HYS2_EN                     (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  To enable hysteresis for comparison on Channel 2 */
#define BITM_ADC_HYS2_MONCYC                 (_ADI_MSK_3(0x00007000,0x00007000U, uint16_t  ))    /*  Program number of conversion cycles to monitor channel 2 before raising alert */
#define BITM_ADC_HYS2_VALUE                  (_ADI_MSK_3(0x000001FF,0x000001FFU, uint16_t  ))    /*  Hysteresis value for Channel 2 */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_LIM3_LO                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_LIM3_LO_EN                  15            /*  To enable low limit comparison on Channel 3 */
#define BITP_ADC_LIM3_LO_VALUE                0            /*  Low limit value for channel 3 */
#define BITM_ADC_LIM3_LO_EN                  (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  To enable low limit comparison on Channel 3 */
#define BITM_ADC_LIM3_LO_VALUE               (_ADI_MSK_3(0x00000FFF,0x00000FFFU, uint16_t  ))    /*  Low limit value for channel 3 */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_LIM3_HI                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_LIM3_HI_EN                  15            /*  To enable high limit comparison on Channel 3 */
#define BITP_ADC_LIM3_HI_VALUE                0            /*  High limit value for channel 3 */
#define BITM_ADC_LIM3_HI_EN                  (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  To enable high limit comparison on Channel 3 */
#define BITM_ADC_LIM3_HI_VALUE               (_ADI_MSK_3(0x00000FFF,0x00000FFFU, uint16_t  ))    /*  High limit value for channel 3 */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_HYS3                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_HYS3_EN                     15            /*  To enable hysteresis for comparison on Channel 3 */
#define BITP_ADC_HYS3_MONCYC                 12            /*  Program number of conversion cycles to monitor channel 3 before raising alert */
#define BITP_ADC_HYS3_VALUE                   0            /*  Hysteresis value for Channel 3 */
#define BITM_ADC_HYS3_EN                     (_ADI_MSK_3(0x00008000,0x00008000U, uint16_t  ))    /*  To enable hysteresis for comparison on Channel 3 */
#define BITM_ADC_HYS3_MONCYC                 (_ADI_MSK_3(0x00007000,0x00007000U, uint16_t  ))    /*  Program number of conversion cycles to monitor channel 3 before raising alert */
#define BITM_ADC_HYS3_VALUE                  (_ADI_MSK_3(0x000001FF,0x000001FFU, uint16_t  ))    /*  Hysteresis value for Channel 3 */

/* -------------------------------------------------------------------------------------------------------------------------
          ADC_CFG1                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_ADC_CFG1_RBUFLP                  0            /*  Enable low power mode for reference buffer */
#define BITM_ADC_CFG1_RBUFLP                 (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Enable low power mode for reference buffer */


/* ============================================================================================================================
        DMA
   ============================================================================================================================ */

/* ============================================================================================================================
        DMA0
   ============================================================================================================================ */
#define REG_DMA0_STAT                        0x40010000            /*  DMA0 DMA Status */
#define REG_DMA0_CFG                         0x40010004            /*  DMA0 DMA Configuration */
#define REG_DMA0_PDBPTR                      0x40010008            /*  DMA0 DMA Channel Primary Control Database Pointer */
#define REG_DMA0_ADBPTR                      0x4001000C            /*  DMA0 DMA Channel Alternate Control Database Pointer */
#define REG_DMA0_SWREQ                       0x40010014            /*  DMA0 DMA Channel Software Request */
#define REG_DMA0_RMSK_SET                    0x40010020            /*  DMA0 DMA Channel Request Mask Set */
#define REG_DMA0_RMSK_CLR                    0x40010024            /*  DMA0 DMA Channel Request Mask Clear */
#define REG_DMA0_EN_SET                      0x40010028            /*  DMA0 DMA Channel Enable Set */
#define REG_DMA0_EN_CLR                      0x4001002C            /*  DMA0 DMA Channel Enable Clear */
#define REG_DMA0_ALT_SET                     0x40010030            /*  DMA0 DMA Channel Primary Alternate Set */
#define REG_DMA0_ALT_CLR                     0x40010034            /*  DMA0 DMA Channel Primary Alternate Clear */
#define REG_DMA0_PRI_SET                     0x40010038            /*  DMA0 DMA Channel Priority Set */
#define REG_DMA0_PRI_CLR                     0x4001003C            /*  DMA0 DMA Channel Priority Clear */
#define REG_DMA0_ERRCHNL_CLR                 0x40010048            /*  DMA0 DMA per Channel Error Clear */
#define REG_DMA0_ERR_CLR                     0x4001004C            /*  DMA0 DMA Bus Error Clear */
#define REG_DMA0_INVALIDDESC_CLR             0x40010050            /*  DMA0 DMA per Channel Invalid Descriptor Clear */
#define REG_DMA0_BS_SET                      0x40010800            /*  DMA0 DMA Channel Bytes Swap Enable Set */
#define REG_DMA0_BS_CLR                      0x40010804            /*  DMA0 DMA Channel Bytes Swap Enable Clear */
#define REG_DMA0_SRCADDR_SET                 0x40010810            /*  DMA0 DMA Channel Source Address Decrement Enable Set */
#define REG_DMA0_SRCADDR_CLR                 0x40010814            /*  DMA0 DMA Channel Source Address Decrement Enable Clear */
#define REG_DMA0_DSTADDR_SET                 0x40010818            /*  DMA0 DMA Channel Destination Address Decrement Enable Set */
#define REG_DMA0_DSTADDR_CLR                 0x4001081C            /*  DMA0 DMA Channel Destination Address Decrement Enable Clear */
#define REG_DMA0_REVID                       0x40010FE0            /*  DMA0 DMA Controller Revision ID */

/* ============================================================================================================================
        DMA Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          DMA_STAT                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_STAT_CHANM1                 16            /*  Number of Available DMA Channels Minus 1 */
#define BITP_DMA_STAT_MEN                     0            /*  Enable Status of the Controller */
#define BITM_DMA_STAT_CHANM1                 (_ADI_MSK_3(0x001F0000,0x001F0000UL, uint32_t  ))    /*  Number of Available DMA Channels Minus 1 */
#define BITM_DMA_STAT_MEN                    (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Enable Status of the Controller */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_CFG                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_CFG_MEN                      0            /*  Controller Enable */
#define BITM_DMA_CFG_MEN                     (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Controller Enable */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_PDBPTR                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_PDBPTR_ADDR                  0            /*  Pointer to the Base Address of the Primary Data Structure */
#define BITM_DMA_PDBPTR_ADDR                 (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Pointer to the Base Address of the Primary Data Structure */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_ADBPTR                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_ADBPTR_ADDR                  0            /*  Base Address of the Alternate Data Structure */
#define BITM_DMA_ADBPTR_ADDR                 (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Base Address of the Alternate Data Structure */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_SWREQ                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_SWREQ_CHAN                   0            /*  Generate Software Request */
#define BITM_DMA_SWREQ_CHAN                  (_ADI_MSK_3(0x07FFFFFF,0x07FFFFFFUL, uint32_t  ))    /*  Generate Software Request */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_RMSK_SET                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_RMSK_SET_CHAN                0            /*  Mask Requests from DMA Channels */
#define BITM_DMA_RMSK_SET_CHAN               (_ADI_MSK_3(0x07FFFFFF,0x07FFFFFFUL, uint32_t  ))    /*  Mask Requests from DMA Channels */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_RMSK_CLR                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_RMSK_CLR_CHAN                0            /*  Clear Request Mask Set Bits */
#define BITM_DMA_RMSK_CLR_CHAN               (_ADI_MSK_3(0x07FFFFFF,0x07FFFFFFUL, uint32_t  ))    /*  Clear Request Mask Set Bits */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_EN_SET                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_EN_SET_CHAN                  0            /*  Enable DMA Channels */
#define BITM_DMA_EN_SET_CHAN                 (_ADI_MSK_3(0x07FFFFFF,0x07FFFFFFUL, uint32_t  ))    /*  Enable DMA Channels */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_EN_CLR                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_EN_CLR_CHAN                  0            /*  Disable DMA Channels */
#define BITM_DMA_EN_CLR_CHAN                 (_ADI_MSK_3(0x07FFFFFF,0x07FFFFFFUL, uint32_t  ))    /*  Disable DMA Channels */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_ALT_SET                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_ALT_SET_CHAN                 0            /*  Control Structure Status / Select Alternate Structure */
#define BITM_DMA_ALT_SET_CHAN                (_ADI_MSK_3(0x07FFFFFF,0x07FFFFFFUL, uint32_t  ))    /*  Control Structure Status / Select Alternate Structure */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_ALT_CLR                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_ALT_CLR_CHAN                 0            /*  Select Primary Data Structure */
#define BITM_DMA_ALT_CLR_CHAN                (_ADI_MSK_3(0x07FFFFFF,0x07FFFFFFUL, uint32_t  ))    /*  Select Primary Data Structure */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_PRI_SET                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_PRI_SET_CHAN                 0            /*  Configure Channel for High Priority */
#define BITM_DMA_PRI_SET_CHAN                (_ADI_MSK_3(0x07FFFFFF,0x07FFFFFFUL, uint32_t  ))    /*  Configure Channel for High Priority */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_PRI_CLR                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_PRI_CLR_CHPRICLR             0            /*  Configure Channel for Default Priority Level */
#define BITM_DMA_PRI_CLR_CHPRICLR            (_ADI_MSK_3(0x07FFFFFF,0x07FFFFFFUL, uint32_t  ))    /*  Configure Channel for Default Priority Level */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_ERRCHNL_CLR                      Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_ERRCHNL_CLR_CHAN             0            /*  Per Channel Bus Error Status/Clear */
#define BITM_DMA_ERRCHNL_CLR_CHAN            (_ADI_MSK_3(0x07FFFFFF,0x07FFFFFFUL, uint32_t  ))    /*  Per Channel Bus Error Status/Clear */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_ERR_CLR                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_ERR_CLR_CHAN                 0            /*  Bus Error Status */
#define BITM_DMA_ERR_CLR_CHAN                (_ADI_MSK_3(0x07FFFFFF,0x07FFFFFFUL, uint32_t  ))    /*  Bus Error Status */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_INVALIDDESC_CLR                  Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_INVALIDDESC_CLR_CHAN         0            /*  Per Channel Invalid Descriptor Status/Clear */
#define BITM_DMA_INVALIDDESC_CLR_CHAN        (_ADI_MSK_3(0x07FFFFFF,0x07FFFFFFUL, uint32_t  ))    /*  Per Channel Invalid Descriptor Status/Clear */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_BS_SET                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_BS_SET_CHAN                  0            /*  Byte Swap Status */
#define BITM_DMA_BS_SET_CHAN                 (_ADI_MSK_3(0x07FFFFFF,0x07FFFFFFUL, uint32_t  ))    /*  Byte Swap Status */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_BS_CLR                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_BS_CLR_CHAN                  0            /*  Disable Byte Swap */
#define BITM_DMA_BS_CLR_CHAN                 (_ADI_MSK_3(0x07FFFFFF,0x07FFFFFFUL, uint32_t  ))    /*  Disable Byte Swap */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_SRCADDR_SET                      Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_SRCADDR_SET_CHAN             0            /*  Source Address Decrement Status */
#define BITM_DMA_SRCADDR_SET_CHAN            (_ADI_MSK_3(0x07FFFFFF,0x07FFFFFFUL, uint32_t  ))    /*  Source Address Decrement Status */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_SRCADDR_CLR                      Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_SRCADDR_CLR_CHAN             0            /*  Disable Source Address Decrement */
#define BITM_DMA_SRCADDR_CLR_CHAN            (_ADI_MSK_3(0x07FFFFFF,0x07FFFFFFUL, uint32_t  ))    /*  Disable Source Address Decrement */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_DSTADDR_SET                      Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_DSTADDR_SET_CHAN             0            /*  Destination Address Decrement Status */
#define BITM_DMA_DSTADDR_SET_CHAN            (_ADI_MSK_3(0x07FFFFFF,0x07FFFFFFUL, uint32_t  ))    /*  Destination Address Decrement Status */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_DSTADDR_CLR                      Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_DSTADDR_CLR_CHAN             0            /*  Disable Destination Address Decrement */
#define BITM_DMA_DSTADDR_CLR_CHAN            (_ADI_MSK_3(0x07FFFFFF,0x07FFFFFFUL, uint32_t  ))    /*  Disable Destination Address Decrement */

/* -------------------------------------------------------------------------------------------------------------------------
          DMA_REVID                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_DMA_REVID_VALUE                  0            /*  DMA Controller Revision ID */
#define BITM_DMA_REVID_VALUE                 (_ADI_MSK_3(0x000000FF,0x000000FFUL, uint32_t  ))    /*  DMA Controller Revision ID */


/* ============================================================================================================================
        Flash Controller
   ============================================================================================================================ */

/* ============================================================================================================================
        FLCC0
   ============================================================================================================================ */
#define REG_FLCC0_STAT                       0x40018000            /*  FLCC0 Status */
#define REG_FLCC0_IEN                        0x40018004            /*  FLCC0 Interrupt Enable */
#define REG_FLCC0_CMD                        0x40018008            /*  FLCC0 Command */
#define REG_FLCC0_KH_ADDR                    0x4001800C            /*  FLCC0 Write Address */
#define REG_FLCC0_KH_DATA0                   0x40018010            /*  FLCC0 Write Lower Data */
#define REG_FLCC0_KH_DATA1                   0x40018014            /*  FLCC0 Write Upper Data */
#define REG_FLCC0_PAGE_ADDR0                 0x40018018            /*  FLCC0 Lower Page Address */
#define REG_FLCC0_PAGE_ADDR1                 0x4001801C            /*  FLCC0 Upper Page Address */
#define REG_FLCC0_KEY                        0x40018020            /*  FLCC0 Key */
#define REG_FLCC0_WR_ABORT_ADDR              0x40018024            /*  FLCC0 Write Abort Address */
#define REG_FLCC0_WRPROT                     0x40018028            /*  FLCC0 Write Protection */
#define REG_FLCC0_SIGNATURE                  0x4001802C            /*  FLCC0 Signature */
#define REG_FLCC0_UCFG                       0x40018030            /*  FLCC0 User Configuration */
#define REG_FLCC0_TIME_PARAM0                0x40018034            /*  FLCC0 Time Parameter 0 */
#define REG_FLCC0_TIME_PARAM1                0x40018038            /*  FLCC0 Time Parameter 1 */
#define REG_FLCC0_ABORT_EN_LO                0x4001803C            /*  FLCC0 IRQ Abort Enable (Lower Bits) */
#define REG_FLCC0_ABORT_EN_HI                0x40018040            /*  FLCC0 IRQ Abort Enable (Upper Bits) */
#define REG_FLCC0_ECC_CFG                    0x40018044            /*  FLCC0 ECC Configuration */
#define REG_FLCC0_ECC_ADDR                   0x40018048            /*  FLCC0 ECC Status (Address) */
#define REG_FLCC0_POR_SEC                    0x40018050            /*  FLCC0 Flash Security */
#define REG_FLCC0_VOL_CFG                    0x40018054            /*  FLCC0 Volatile Flash Configuration */

/* ============================================================================================================================
        FLCC Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_STAT                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_STAT_ACCESS_MODE           31            /*  Access Mode */
#define BITP_FLCC_STAT_CACHESRAMPERR         29            /*  SRAM parity errors in Cache Controller */
#define BITP_FLCC_STAT_ECCDCODE              27            /*  DCode AHB Bus Error ECC status */
#define BITP_FLCC_STAT_ECCICODE              25            /*  ICode AHB Bus Error ECC status */
#define BITP_FLCC_STAT_ECCERRCNT             17            /*  ECC correction counter */
#define BITP_FLCC_STAT_ECCINFOSIGN           15            /*  ECC status of flash initialization */
#define BITP_FLCC_STAT_INIT                  14            /*  Flash controller initialization in progress */
#define BITP_FLCC_STAT_SIGNERR               13            /*  Signature check failure during initialization */
#define BITP_FLCC_STAT_OVERLAP               11            /*  Overlapping Command */
#define BITP_FLCC_STAT_ECCRDERR               9            /*  ECC IRQ cause */
#define BITP_FLCC_STAT_ECCERRCMD              7            /*  ECC errors detected during user issued SIGN command */
#define BITP_FLCC_STAT_SLEEPING               6            /*  Flash array is in low power (sleep) mode */
#define BITP_FLCC_STAT_CMDFAIL                4            /*  Provides information on command failures */
#define BITP_FLCC_STAT_WRALCOMP               3            /*  Write almost complete */
#define BITP_FLCC_STAT_CMDCOMP                2            /*  Command complete */
#define BITP_FLCC_STAT_WRCLOSE                1            /*  WRITE registers are closed */
#define BITP_FLCC_STAT_CMDBUSY                0            /*  Command busy */
#define BITM_FLCC_STAT_ACCESS_MODE           (_ADI_MSK_3(0x80000000,0x80000000UL, uint32_t  ))    /*  Access Mode */
#define BITM_FLCC_STAT_CACHESRAMPERR         (_ADI_MSK_3(0x20000000,0x20000000UL, uint32_t  ))    /*  SRAM parity errors in Cache Controller */
#define BITM_FLCC_STAT_ECCDCODE              (_ADI_MSK_3(0x18000000,0x18000000UL, uint32_t  ))    /*  DCode AHB Bus Error ECC status */
#define BITM_FLCC_STAT_ECCICODE              (_ADI_MSK_3(0x06000000,0x06000000UL, uint32_t  ))    /*  ICode AHB Bus Error ECC status */
#define BITM_FLCC_STAT_ECCERRCNT             (_ADI_MSK_3(0x000E0000,0x000E0000UL, uint32_t  ))    /*  ECC correction counter */
#define BITM_FLCC_STAT_ECCINFOSIGN           (_ADI_MSK_3(0x00018000,0x00018000UL, uint32_t  ))    /*  ECC status of flash initialization */
#define BITM_FLCC_STAT_INIT                  (_ADI_MSK_3(0x00004000,0x00004000UL, uint32_t  ))    /*  Flash controller initialization in progress */
#define BITM_FLCC_STAT_SIGNERR               (_ADI_MSK_3(0x00002000,0x00002000UL, uint32_t  ))    /*  Signature check failure during initialization */
#define BITM_FLCC_STAT_OVERLAP               (_ADI_MSK_3(0x00000800,0x00000800UL, uint32_t  ))    /*  Overlapping Command */
#define BITM_FLCC_STAT_ECCRDERR              (_ADI_MSK_3(0x00000600,0x00000600UL, uint32_t  ))    /*  ECC IRQ cause */
#define BITM_FLCC_STAT_ECCERRCMD             (_ADI_MSK_3(0x00000180,0x00000180UL, uint32_t  ))    /*  ECC errors detected during user issued SIGN command */
#define BITM_FLCC_STAT_SLEEPING              (_ADI_MSK_3(0x00000040,0x00000040UL, uint32_t  ))    /*  Flash array is in low power (sleep) mode */
#define BITM_FLCC_STAT_CMDFAIL               (_ADI_MSK_3(0x00000030,0x00000030UL, uint32_t  ))    /*  Provides information on command failures */
#define BITM_FLCC_STAT_WRALCOMP              (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  Write almost complete */
#define BITM_FLCC_STAT_CMDCOMP               (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  Command complete */
#define BITM_FLCC_STAT_WRCLOSE               (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  WRITE registers are closed */
#define BITM_FLCC_STAT_CMDBUSY               (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Command busy */
#define ENUM_FLCC_STAT_DIRECT                (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  ACCESS_MODE: Flash controller is currently in Direct Access mode; user access to all registers is enabled */
#define ENUM_FLCC_STAT_INDIRECT              (_ADI_MSK_3(0x80000000,0x80000000UL, uint32_t  ))    /*  ACCESS_MODE: Flash Controller is currently in Indirect Access mode; user access to registers is limited to read-only access of the status register. Full register access will be restored when the Cryptographic module releases control of the flash controller (crypto completes the ongoing operation within the protected key storage region) */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_IEN                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_IEN_ECC_ERROR               6            /*  Control whether to generate bus errors, interrupts, or neither in response to 2-bit ECC Error events */
#define BITP_FLCC_IEN_ECC_CORRECT             4            /*  Control whether to generate bus errors, interrupts, or neither in response to 1-bit ECC Correction events */
#define BITP_FLCC_IEN_CMDFAIL                 2            /*  Command fail interrupt enable */
#define BITP_FLCC_IEN_WRALCMPLT               1            /*  Write almost complete interrupt enable */
#define BITP_FLCC_IEN_CMDCMPLT                0            /*  Command complete interrupt enable */
#define BITM_FLCC_IEN_ECC_ERROR              (_ADI_MSK_3(0x000000C0,0x000000C0UL, uint32_t  ))    /*  Control whether to generate bus errors, interrupts, or neither in response to 2-bit ECC Error events */
#define BITM_FLCC_IEN_ECC_CORRECT            (_ADI_MSK_3(0x00000030,0x00000030UL, uint32_t  ))    /*  Control whether to generate bus errors, interrupts, or neither in response to 1-bit ECC Correction events */
#define BITM_FLCC_IEN_CMDFAIL                (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  Command fail interrupt enable */
#define BITM_FLCC_IEN_WRALCMPLT              (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Write almost complete interrupt enable */
#define BITM_FLCC_IEN_CMDCMPLT               (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Command complete interrupt enable */
#define ENUM_FLCC_IEN_NONE_ERR               (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  ECC_ERROR: Do not generate a response to ECC events */
#define ENUM_FLCC_IEN_BUS_ERR_ERR            (_ADI_MSK_3(0x00000040,0x00000040UL, uint32_t  ))    /*  ECC_ERROR: Generate Bus Errors in response to ECC events */
#define ENUM_FLCC_IEN_IRQ_ERR                (_ADI_MSK_3(0x00000080,0x00000080UL, uint32_t  ))    /*  ECC_ERROR: Generate IRQs in response to ECC events */
#define ENUM_FLCC_IEN_NONE_COR               (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  ECC_CORRECT: Do not generate a response to ECC events */
#define ENUM_FLCC_IEN_BUS_ERR_COR            (_ADI_MSK_3(0x00000010,0x00000010UL, uint32_t  ))    /*  ECC_CORRECT: Generate Bus Errors in response to ECC events */
#define ENUM_FLCC_IEN_IRQ_COR                (_ADI_MSK_3(0x00000020,0x00000020UL, uint32_t  ))    /*  ECC_CORRECT: Generate IRQs in response to ECC events */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_CMD                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_CMD_VALUE                   0            /*  Commands */
#define BITM_FLCC_CMD_VALUE                  (_ADI_MSK_3(0x0000000F,0x0000000FUL, uint32_t  ))    /*  Commands */
#define ENUM_FLCC_CMD_IDLE                   (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  VALUE: IDLE */
#define ENUM_FLCC_CMD_ABORT                  (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  VALUE: ABORT */
#define ENUM_FLCC_CMD_SLEEP                  (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  VALUE: Requests flash to enter Sleep mode */
#define ENUM_FLCC_CMD_SIGN                   (_ADI_MSK_3(0x00000003,0x00000003UL, uint32_t  ))    /*  VALUE: SIGN */
#define ENUM_FLCC_CMD_WRITE                  (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  VALUE: WRITE */
#define ENUM_FLCC_CMD_BLANK_CHECK            (_ADI_MSK_3(0x00000005,0x00000005UL, uint32_t  ))    /*  VALUE: Checks all of User Space; fails if any bits in user space are cleared */
#define ENUM_FLCC_CMD_ERASEPAGE              (_ADI_MSK_3(0x00000006,0x00000006UL, uint32_t  ))    /*  VALUE: ERASEPAGE */
#define ENUM_FLCC_CMD_MASSERASE              (_ADI_MSK_3(0x00000007,0x00000007UL, uint32_t  ))    /*  VALUE: MASSERASE */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_KH_ADDR                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_KH_ADDR_VALUE               3            /*  Address to be written on a WRITE command */
#define BITM_FLCC_KH_ADDR_VALUE              (_ADI_MSK_3(0x000FFFF8,0x000FFFF8UL, uint32_t  ))    /*  Address to be written on a WRITE command */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_KH_DATA0                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_KH_DATA0_VALUE              0            /*  Lower half of 64-bit dual word data to be written on a Write command */
#define BITM_FLCC_KH_DATA0_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Lower half of 64-bit dual word data to be written on a Write command */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_KH_DATA1                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_KH_DATA1_VALUE              0            /*  Upper half of 64-bit dual word data to be written on a Write command */
#define BITM_FLCC_KH_DATA1_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Upper half of 64-bit dual word data to be written on a Write command */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_PAGE_ADDR0                      Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_PAGE_ADDR0_VALUE           10            /*  Lower address bits of the page address */
#define BITM_FLCC_PAGE_ADDR0_VALUE           (_ADI_MSK_3(0x000FFC00,0x000FFC00UL, uint32_t  ))    /*  Lower address bits of the page address */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_PAGE_ADDR1                      Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_PAGE_ADDR1_VALUE           10            /*  Upper address bits of the page address */
#define BITM_FLCC_PAGE_ADDR1_VALUE           (_ADI_MSK_3(0x000FFC00,0x000FFC00UL, uint32_t  ))    /*  Upper address bits of the page address */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_KEY                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_KEY_VALUE                   0            /*  Key register */
#define BITM_FLCC_KEY_VALUE                  (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Key register */
#define ENUM_FLCC_KEY_USERKEY                (_ADI_MSK_3(0x676C7565,0x676C7565UL, uint32_t  ))    /*  VALUE: USERKEY */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_WR_ABORT_ADDR                   Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_WR_ABORT_ADDR_VALUE         0            /*  Address of recently aborted write command */
#define BITM_FLCC_WR_ABORT_ADDR_VALUE        (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Address of recently aborted write command */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_WRPROT                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_WRPROT_WORD                 0            /*  Clear bits to write protect related groups of user space pages */
#define BITM_FLCC_WRPROT_WORD                (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Clear bits to write protect related groups of user space pages */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_SIGNATURE                       Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_SIGNATURE_VALUE             0            /*  Read signature */
#define BITM_FLCC_SIGNATURE_VALUE            (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Read signature */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_UCFG                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_UCFG_AUTOINCEN              1            /*  Auto Address Increment for Key Hole Access */
#define BITP_FLCC_UCFG_KHDMAEN                0            /*  Key hole DMA enable */
#define BITM_FLCC_UCFG_AUTOINCEN             (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Auto Address Increment for Key Hole Access */
#define BITM_FLCC_UCFG_KHDMAEN               (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Key hole DMA enable */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_TIME_PARAM0                     Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_TIME_PARAM0_TNVH1          28            /*  NVSTR Hold time during Mass Erase */
#define BITP_FLCC_TIME_PARAM0_TERASE         24            /*  Erase Time */
#define BITP_FLCC_TIME_PARAM0_TRCV           20            /*  Recovery time */
#define BITP_FLCC_TIME_PARAM0_TNVH           16            /*  NVSTR Hold time */
#define BITP_FLCC_TIME_PARAM0_TPROG          12            /*  Program time */
#define BITP_FLCC_TIME_PARAM0_TPGS            8            /*  NVSTR to Program setup time */
#define BITP_FLCC_TIME_PARAM0_TNVS            4            /*  PROG/ERASE to NVSTR setup time */
#define BITP_FLCC_TIME_PARAM0_DIVREFCLK       0            /*  Divide Reference Clock (by 2) */
#define BITM_FLCC_TIME_PARAM0_TNVH1          (_ADI_MSK_3(0xF0000000,0xF0000000UL, uint32_t  ))    /*  NVSTR Hold time during Mass Erase */
#define BITM_FLCC_TIME_PARAM0_TERASE         (_ADI_MSK_3(0x0F000000,0x0F000000UL, uint32_t  ))    /*  Erase Time */
#define BITM_FLCC_TIME_PARAM0_TRCV           (_ADI_MSK_3(0x00F00000,0x00F00000UL, uint32_t  ))    /*  Recovery time */
#define BITM_FLCC_TIME_PARAM0_TNVH           (_ADI_MSK_3(0x000F0000,0x000F0000UL, uint32_t  ))    /*  NVSTR Hold time */
#define BITM_FLCC_TIME_PARAM0_TPROG          (_ADI_MSK_3(0x0000F000,0x0000F000UL, uint32_t  ))    /*  Program time */
#define BITM_FLCC_TIME_PARAM0_TPGS           (_ADI_MSK_3(0x00000F00,0x00000F00UL, uint32_t  ))    /*  NVSTR to Program setup time */
#define BITM_FLCC_TIME_PARAM0_TNVS           (_ADI_MSK_3(0x000000F0,0x000000F0UL, uint32_t  ))    /*  PROG/ERASE to NVSTR setup time */
#define BITM_FLCC_TIME_PARAM0_DIVREFCLK      (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Divide Reference Clock (by 2) */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_TIME_PARAM1                     Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_TIME_PARAM1_CURWAITSTATES   8            /*  Current wait states [2:0] */
#define BITP_FLCC_TIME_PARAM1_WAITSTATES      4            /*  Number of wait states to access flash */
#define BITP_FLCC_TIME_PARAM1_TWK             0            /*  Wake up time */
#define BITM_FLCC_TIME_PARAM1_CURWAITSTATES  (_ADI_MSK_3(0x00000700,0x00000700UL, uint32_t  ))    /*  Current wait states [2:0] */
#define BITM_FLCC_TIME_PARAM1_WAITSTATES     (_ADI_MSK_3(0x00000070,0x00000070UL, uint32_t  ))    /*  Number of wait states to access flash */
#define BITM_FLCC_TIME_PARAM1_TWK            (_ADI_MSK_3(0x0000000F,0x0000000FUL, uint32_t  ))    /*  Wake up time */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_ABORT_EN_LO                     Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_ABORT_EN_LO_VALUE           0            /*  Sys IRQ Abort Enable */
#define BITM_FLCC_ABORT_EN_LO_VALUE          (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Sys IRQ Abort Enable */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_ABORT_EN_HI                     Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_ABORT_EN_HI_VALUE           0            /*  Sys IRQ Abort Enable */
#define BITM_FLCC_ABORT_EN_HI_VALUE          (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Sys IRQ Abort Enable */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_ECC_CFG                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_ECC_CFG_PTR                 8            /*  ECC start page pointer */
#define BITP_FLCC_ECC_CFG_INFOEN              1            /*  Info space ECC Enable bit */
#define BITP_FLCC_ECC_CFG_EN                  0            /*  ECC Enable */
#define BITM_FLCC_ECC_CFG_PTR                (_ADI_MSK_3(0xFFFFFF00,0xFFFFFF00UL, uint32_t  ))    /*  ECC start page pointer */
#define BITM_FLCC_ECC_CFG_INFOEN             (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Info space ECC Enable bit */
#define BITM_FLCC_ECC_CFG_EN                 (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  ECC Enable */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_ECC_ADDR                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_ECC_ADDR_VALUE              0            /*  This register has the address for which ECC error is detected */
#define BITM_FLCC_ECC_ADDR_VALUE             (_ADI_MSK_3(0x000FFFFF,0x000FFFFFUL, uint32_t  ))    /*  This register has the address for which ECC error is detected */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_POR_SEC                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_POR_SEC_SECURE              0            /*  Set this bit to prevent read or write access to User Space (sticky when set) */
#define BITM_FLCC_POR_SEC_SECURE             (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Set this bit to prevent read or write access to User Space (sticky when set) */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_VOL_CFG                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_VOL_CFG_INFO_REMAP          0            /*  Alias the info space to the base address of user space */
#define BITM_FLCC_VOL_CFG_INFO_REMAP         (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Alias the info space to the base address of user space */


/* ============================================================================================================================
        Cache Controller
   ============================================================================================================================ */

/* ============================================================================================================================
        FLCC0_CACHE
   ============================================================================================================================ */
#define REG_FLCC0_CACHE_STAT                 0x40018058            /*  FLCC0_CACHE Cache Status Register */
#define REG_FLCC0_CACHE_SETUP                0x4001805C            /*  FLCC0_CACHE Cache Setup Register */
#define REG_FLCC0_CACHE_KEY                  0x40018060            /*  FLCC0_CACHE Cache Key Register */

/* ============================================================================================================================
        FLCC_CACHE Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_CACHE_STAT                      Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_CACHE_STAT_ICEN             0            /*  If this bit is set,  I-Cache is enabled */
#define BITM_FLCC_CACHE_STAT_ICEN            (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  If this bit is set,  I-Cache is enabled */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_CACHE_SETUP                     Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_CACHE_SETUP_LCKIC           1            /*  If this bit is set, I-Cache contents are locked */
#define BITP_FLCC_CACHE_SETUP_ICEN            0            /*  If this bit set, I-Cache is enabled for AHB accesses */
#define BITM_FLCC_CACHE_SETUP_LCKIC          (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  If this bit is set, I-Cache contents are locked */
#define BITM_FLCC_CACHE_SETUP_ICEN           (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  If this bit set, I-Cache is enabled for AHB accesses */

/* -------------------------------------------------------------------------------------------------------------------------
          FLCC_CACHE_KEY                       Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_FLCC_CACHE_KEY_VALUE             0            /*  Cache Key */
#define BITM_FLCC_CACHE_KEY_VALUE            (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Cache Key */


/* ============================================================================================================================
        
   ============================================================================================================================ */

/* ============================================================================================================================
        GPIO0
   ============================================================================================================================ */
#define REG_GPIO0_CFG                        0x40020000            /*  GPIO0 Port  Configuration */
#define REG_GPIO0_OEN                        0x40020004            /*  GPIO0 Port Output Enable */
#define REG_GPIO0_PE                         0x40020008            /*  GPIO0 Port Output Pull-up/Pull-down Enable */
#define REG_GPIO0_IEN                        0x4002000C            /*  GPIO0 Port  Input Path Enable */
#define REG_GPIO0_IN                         0x40020010            /*  GPIO0 Port  Registered Data Input */
#define REG_GPIO0_OUT                        0x40020014            /*  GPIO0 Port Data Output */
#define REG_GPIO0_SET                        0x40020018            /*  GPIO0 Port Data Out Set */
#define REG_GPIO0_CLR                        0x4002001C            /*  GPIO0 Port Data Out Clear */
#define REG_GPIO0_TGL                        0x40020020            /*  GPIO0 Port Pin Toggle */
#define REG_GPIO0_POL                        0x40020024            /*  GPIO0 Port Interrupt Polarity */
#define REG_GPIO0_IENA                       0x40020028            /*  GPIO0 Port Interrupt A Enable */
#define REG_GPIO0_IENB                       0x4002002C            /*  GPIO0 Port Interrupt B Enable */
#define REG_GPIO0_INT                        0x40020030            /*  GPIO0 Port Interrupt Status */
#define REG_GPIO0_DS                         0x40020034            /*  GPIO0 Port Drive Strength Select */

/* ============================================================================================================================
        GPIO1
   ============================================================================================================================ */
#define REG_GPIO1_CFG                        0x40020040            /*  GPIO1 Port  Configuration */
#define REG_GPIO1_OEN                        0x40020044            /*  GPIO1 Port Output Enable */
#define REG_GPIO1_PE                         0x40020048            /*  GPIO1 Port Output Pull-up/Pull-down Enable */
#define REG_GPIO1_IEN                        0x4002004C            /*  GPIO1 Port  Input Path Enable */
#define REG_GPIO1_IN                         0x40020050            /*  GPIO1 Port  Registered Data Input */
#define REG_GPIO1_OUT                        0x40020054            /*  GPIO1 Port Data Output */
#define REG_GPIO1_SET                        0x40020058            /*  GPIO1 Port Data Out Set */
#define REG_GPIO1_CLR                        0x4002005C            /*  GPIO1 Port Data Out Clear */
#define REG_GPIO1_TGL                        0x40020060            /*  GPIO1 Port Pin Toggle */
#define REG_GPIO1_POL                        0x40020064            /*  GPIO1 Port Interrupt Polarity */
#define REG_GPIO1_IENA                       0x40020068            /*  GPIO1 Port Interrupt A Enable */
#define REG_GPIO1_IENB                       0x4002006C            /*  GPIO1 Port Interrupt B Enable */
#define REG_GPIO1_INT                        0x40020070            /*  GPIO1 Port Interrupt Status */
#define REG_GPIO1_DS                         0x40020074            /*  GPIO1 Port Drive Strength Select */

/* ============================================================================================================================
        GPIO2
   ============================================================================================================================ */
#define REG_GPIO2_CFG                        0x40020080            /*  GPIO2 Port  Configuration */
#define REG_GPIO2_OEN                        0x40020084            /*  GPIO2 Port Output Enable */
#define REG_GPIO2_PE                         0x40020088            /*  GPIO2 Port Output Pull-up/Pull-down Enable */
#define REG_GPIO2_IEN                        0x4002008C            /*  GPIO2 Port  Input Path Enable */
#define REG_GPIO2_IN                         0x40020090            /*  GPIO2 Port  Registered Data Input */
#define REG_GPIO2_OUT                        0x40020094            /*  GPIO2 Port Data Output */
#define REG_GPIO2_SET                        0x40020098            /*  GPIO2 Port Data Out Set */
#define REG_GPIO2_CLR                        0x4002009C            /*  GPIO2 Port Data Out Clear */
#define REG_GPIO2_TGL                        0x400200A0            /*  GPIO2 Port Pin Toggle */
#define REG_GPIO2_POL                        0x400200A4            /*  GPIO2 Port Interrupt Polarity */
#define REG_GPIO2_IENA                       0x400200A8            /*  GPIO2 Port Interrupt A Enable */
#define REG_GPIO2_IENB                       0x400200AC            /*  GPIO2 Port Interrupt B Enable */
#define REG_GPIO2_INT                        0x400200B0            /*  GPIO2 Port Interrupt Status */
#define REG_GPIO2_DS                         0x400200B4            /*  GPIO2 Port Drive Strength Select */

/* ============================================================================================================================
        GPIO3
   ============================================================================================================================ */
#define REG_GPIO3_CFG                        0x400200C0            /*  GPIO3 Port  Configuration */
#define REG_GPIO3_OEN                        0x400200C4            /*  GPIO3 Port Output Enable */
#define REG_GPIO3_PE                         0x400200C8            /*  GPIO3 Port Output Pull-up/Pull-down Enable */
#define REG_GPIO3_IEN                        0x400200CC            /*  GPIO3 Port  Input Path Enable */
#define REG_GPIO3_IN                         0x400200D0            /*  GPIO3 Port  Registered Data Input */
#define REG_GPIO3_OUT                        0x400200D4            /*  GPIO3 Port Data Output */
#define REG_GPIO3_SET                        0x400200D8            /*  GPIO3 Port Data Out Set */
#define REG_GPIO3_CLR                        0x400200DC            /*  GPIO3 Port Data Out Clear */
#define REG_GPIO3_TGL                        0x400200E0            /*  GPIO3 Port Pin Toggle */
#define REG_GPIO3_POL                        0x400200E4            /*  GPIO3 Port Interrupt Polarity */
#define REG_GPIO3_IENA                       0x400200E8            /*  GPIO3 Port Interrupt A Enable */
#define REG_GPIO3_IENB                       0x400200EC            /*  GPIO3 Port Interrupt B Enable */
#define REG_GPIO3_INT                        0x400200F0            /*  GPIO3 Port Interrupt Status */
#define REG_GPIO3_DS                         0x400200F4            /*  GPIO3 Port Drive Strength Select */

/* ============================================================================================================================
        GPIO Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          GPIO_CFG                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_CFG_PIN15                  30            /*  Pin 15  configuration bits */
#define BITP_GPIO_CFG_PIN14                  28            /*  Pin 14  configuration bits */
#define BITP_GPIO_CFG_PIN13                  26            /*  Pin 13  configuration bits */
#define BITP_GPIO_CFG_PIN12                  24            /*  Pin 12  configuration bits */
#define BITP_GPIO_CFG_PIN11                  22            /*  Pin 11  configuration bits */
#define BITP_GPIO_CFG_PIN10                  20            /*  Pin 10  configuration bits */
#define BITP_GPIO_CFG_PIN09                  18            /*  Pin 9 configuration bits */
#define BITP_GPIO_CFG_PIN08                  16            /*  Pin 8 configuration bits */
#define BITP_GPIO_CFG_PIN07                  14            /*  Pin 7 configuration bits */
#define BITP_GPIO_CFG_PIN06                  12            /*  Pin 6 configuration bits */
#define BITP_GPIO_CFG_PIN05                  10            /*  Pin 5 configuration bits */
#define BITP_GPIO_CFG_PIN04                   8            /*  Pin 4 configuration bits */
#define BITP_GPIO_CFG_PIN03                   6            /*  Pin 3 configuration bits */
#define BITP_GPIO_CFG_PIN02                   4            /*  Pin 2 configuration bits */
#define BITP_GPIO_CFG_PIN01                   2            /*  Pin 1 configuration bits */
#define BITP_GPIO_CFG_PIN00                   0            /*  Pin 0 configuration bits */
#define BITM_GPIO_CFG_PIN15                  (_ADI_MSK_3(0xC0000000,0xC0000000UL, uint32_t  ))    /*  Pin 15  configuration bits */
#define BITM_GPIO_CFG_PIN14                  (_ADI_MSK_3(0x30000000,0x30000000UL, uint32_t  ))    /*  Pin 14  configuration bits */
#define BITM_GPIO_CFG_PIN13                  (_ADI_MSK_3(0x0C000000,0x0C000000UL, uint32_t  ))    /*  Pin 13  configuration bits */
#define BITM_GPIO_CFG_PIN12                  (_ADI_MSK_3(0x03000000,0x03000000UL, uint32_t  ))    /*  Pin 12  configuration bits */
#define BITM_GPIO_CFG_PIN11                  (_ADI_MSK_3(0x00C00000,0x00C00000UL, uint32_t  ))    /*  Pin 11  configuration bits */
#define BITM_GPIO_CFG_PIN10                  (_ADI_MSK_3(0x00300000,0x00300000UL, uint32_t  ))    /*  Pin 10  configuration bits */
#define BITM_GPIO_CFG_PIN09                  (_ADI_MSK_3(0x000C0000,0x000C0000UL, uint32_t  ))    /*  Pin 9 configuration bits */
#define BITM_GPIO_CFG_PIN08                  (_ADI_MSK_3(0x00030000,0x00030000UL, uint32_t  ))    /*  Pin 8 configuration bits */
#define BITM_GPIO_CFG_PIN07                  (_ADI_MSK_3(0x0000C000,0x0000C000UL, uint32_t  ))    /*  Pin 7 configuration bits */
#define BITM_GPIO_CFG_PIN06                  (_ADI_MSK_3(0x00003000,0x00003000UL, uint32_t  ))    /*  Pin 6 configuration bits */
#define BITM_GPIO_CFG_PIN05                  (_ADI_MSK_3(0x00000C00,0x00000C00UL, uint32_t  ))    /*  Pin 5 configuration bits */
#define BITM_GPIO_CFG_PIN04                  (_ADI_MSK_3(0x00000300,0x00000300UL, uint32_t  ))    /*  Pin 4 configuration bits */
#define BITM_GPIO_CFG_PIN03                  (_ADI_MSK_3(0x000000C0,0x000000C0UL, uint32_t  ))    /*  Pin 3 configuration bits */
#define BITM_GPIO_CFG_PIN02                  (_ADI_MSK_3(0x00000030,0x00000030UL, uint32_t  ))    /*  Pin 2 configuration bits */
#define BITM_GPIO_CFG_PIN01                  (_ADI_MSK_3(0x0000000C,0x0000000CUL, uint32_t  ))    /*  Pin 1 configuration bits */
#define BITM_GPIO_CFG_PIN00                  (_ADI_MSK_3(0x00000003,0x00000003UL, uint32_t  ))    /*  Pin 0 configuration bits */

/* -------------------------------------------------------------------------------------------------------------------------
          GPIO_OEN                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_OEN_VALUE                   0            /*  Pin Output Drive enable */
#define BITM_GPIO_OEN_VALUE                  (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Pin Output Drive enable */

/* -------------------------------------------------------------------------------------------------------------------------
          GPIO_PE                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_PE_VALUE                    0            /*  Pin Pull enable */
#define BITM_GPIO_PE_VALUE                   (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Pin Pull enable */

/* -------------------------------------------------------------------------------------------------------------------------
          GPIO_IEN                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_IEN_VALUE                   0            /*  Input path enable */
#define BITM_GPIO_IEN_VALUE                  (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Input path enable */

/* -------------------------------------------------------------------------------------------------------------------------
          GPIO_IN                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_IN_VALUE                    0            /*  Registered data input */
#define BITM_GPIO_IN_VALUE                   (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Registered data input */

/* -------------------------------------------------------------------------------------------------------------------------
          GPIO_OUT                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_OUT_VALUE                   0            /*  Data out */
#define BITM_GPIO_OUT_VALUE                  (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Data out */

/* -------------------------------------------------------------------------------------------------------------------------
          GPIO_SET                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_SET_VALUE                   0            /*  Set the output HIGH for the pin */
#define BITM_GPIO_SET_VALUE                  (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Set the output HIGH for the pin */

/* -------------------------------------------------------------------------------------------------------------------------
          GPIO_CLR                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_CLR_VALUE                   0            /*  Set the output low  for the port pin */
#define BITM_GPIO_CLR_VALUE                  (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Set the output low  for the port pin */

/* -------------------------------------------------------------------------------------------------------------------------
          GPIO_TGL                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_TGL_VALUE                   0            /*  Toggle the output of the port pin */
#define BITM_GPIO_TGL_VALUE                  (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Toggle the output of the port pin */

/* -------------------------------------------------------------------------------------------------------------------------
          GPIO_POL                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_POL_VALUE                   0            /*  Interrupt polarity */
#define BITM_GPIO_POL_VALUE                  (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Interrupt polarity */

/* -------------------------------------------------------------------------------------------------------------------------
          GPIO_IENA                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_IENA_VALUE                  0            /*  Interrupt A enable */
#define BITM_GPIO_IENA_VALUE                 (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Interrupt A enable */

/* -------------------------------------------------------------------------------------------------------------------------
          GPIO_IENB                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_IENB_VALUE                  0            /*  Interrupt B enable */
#define BITM_GPIO_IENB_VALUE                 (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Interrupt B enable */

/* -------------------------------------------------------------------------------------------------------------------------
          GPIO_INT                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_INT_VALUE                   0            /*  Interrupt Status */
#define BITM_GPIO_INT_VALUE                  (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Interrupt Status */

/* -------------------------------------------------------------------------------------------------------------------------
          GPIO_DS                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_DS_VALUE                    0            /*  Drive strength select */
#define BITM_GPIO_DS_VALUE                   (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Drive strength select */


/* ============================================================================================================================
        Serial Port
   ============================================================================================================================ */

/* ============================================================================================================================
        SPORT0
   ============================================================================================================================ */
#define REG_SPORT0_CTL_A                     0x40038000            /*  SPORT0 Half SPORT 'A' Control Register */
#define REG_SPORT0_DIV_A                     0x40038004            /*  SPORT0 Half SPORT 'A' Divisor Register */
#define REG_SPORT0_IEN_A                     0x40038008            /*  SPORT0 Half SPORT A's Interrupt Enable register */
#define REG_SPORT0_STAT_A                    0x4003800C            /*  SPORT0 Half SPORT 'A' Status register */
#define REG_SPORT0_NUMTRAN_A                 0x40038010            /*  SPORT0 Half SPORT A Number of transfers register */
#define REG_SPORT0_CNVT_A                    0x40038014            /*  SPORT0 Half SPORT 'A' CNV width */
#define REG_SPORT0_TX_A                      0x40038020            /*  SPORT0 Half SPORT 'A' Tx Buffer Register */
#define REG_SPORT0_RX_A                      0x40038028            /*  SPORT0 Half SPORT 'A' Rx Buffer Register */
#define REG_SPORT0_CTL_B                     0x40038040            /*  SPORT0 Half SPORT 'B' Control Register */
#define REG_SPORT0_DIV_B                     0x40038044            /*  SPORT0 Half SPORT 'B' Divisor Register */
#define REG_SPORT0_IEN_B                     0x40038048            /*  SPORT0 Half SPORT B's Interrupt Enable register */
#define REG_SPORT0_STAT_B                    0x4003804C            /*  SPORT0 Half SPORT 'B' Status register */
#define REG_SPORT0_NUMTRAN_B                 0x40038050            /*  SPORT0 Half SPORT B Number of transfers register */
#define REG_SPORT0_CNVT_B                    0x40038054            /*  SPORT0 Half SPORT 'B' CNV width register */
#define REG_SPORT0_TX_B                      0x40038060            /*  SPORT0 Half SPORT 'B' Tx Buffer Register */
#define REG_SPORT0_RX_B                      0x40038068            /*  SPORT0 Half SPORT 'B' Rx Buffer Register */

/* ============================================================================================================================
        SPORT Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          SPORT_CTL_A                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPORT_CTL_A_DMAEN               26            /*  DMA Enable */
#define BITP_SPORT_CTL_A_SPTRAN              25            /*  Serial Port Transfer Direction */
#define BITP_SPORT_CTL_A_GCLKEN              21            /*  Gated Clock Enable */
#define BITP_SPORT_CTL_A_FSERRMODE           20            /*  Frame Sync Error Operation */
#define BITP_SPORT_CTL_A_PACK                18            /*  Packing Enable */
#define BITP_SPORT_CTL_A_LAFS                17            /*  Late Frame Sync */
#define BITP_SPORT_CTL_A_LFS                 16            /*  Active-Low Frame Sync */
#define BITP_SPORT_CTL_A_DIFS                15            /*  Data-Independent Frame Sync */
#define BITP_SPORT_CTL_A_IFS                 14            /*  Internal Frame Sync */
#define BITP_SPORT_CTL_A_FSR                 13            /*  Frame Sync Required */
#define BITP_SPORT_CTL_A_CKRE                12            /*  Clock Rising Edge */
#define BITP_SPORT_CTL_A_OPMODE              11            /*  Operation mode */
#define BITP_SPORT_CTL_A_ICLK                10            /*  Internal Clock */
#define BITP_SPORT_CTL_A_SLEN                 4            /*  Serial Word Length */
#define BITP_SPORT_CTL_A_LSBF                 3            /*  Least-Significant Bit First */
#define BITP_SPORT_CTL_A_CKMUXSEL             2            /*  Clock Multiplexer Select */
#define BITP_SPORT_CTL_A_FSMUXSEL             1            /*  Frame Sync Multiplexer Select */
#define BITP_SPORT_CTL_A_SPEN                 0            /*  Serial Port Enable */
#define BITM_SPORT_CTL_A_DMAEN               (_ADI_MSK_3(0x04000000,0x04000000UL, uint32_t  ))    /*  DMA Enable */
#define BITM_SPORT_CTL_A_SPTRAN              (_ADI_MSK_3(0x02000000,0x02000000UL, uint32_t  ))    /*  Serial Port Transfer Direction */
#define BITM_SPORT_CTL_A_GCLKEN              (_ADI_MSK_3(0x00200000,0x00200000UL, uint32_t  ))    /*  Gated Clock Enable */
#define BITM_SPORT_CTL_A_FSERRMODE           (_ADI_MSK_3(0x00100000,0x00100000UL, uint32_t  ))    /*  Frame Sync Error Operation */
#define BITM_SPORT_CTL_A_PACK                (_ADI_MSK_3(0x000C0000,0x000C0000UL, uint32_t  ))    /*  Packing Enable */
#define BITM_SPORT_CTL_A_LAFS                (_ADI_MSK_3(0x00020000,0x00020000UL, uint32_t  ))    /*  Late Frame Sync */
#define BITM_SPORT_CTL_A_LFS                 (_ADI_MSK_3(0x00010000,0x00010000UL, uint32_t  ))    /*  Active-Low Frame Sync */
#define BITM_SPORT_CTL_A_DIFS                (_ADI_MSK_3(0x00008000,0x00008000UL, uint32_t  ))    /*  Data-Independent Frame Sync */
#define BITM_SPORT_CTL_A_IFS                 (_ADI_MSK_3(0x00004000,0x00004000UL, uint32_t  ))    /*  Internal Frame Sync */
#define BITM_SPORT_CTL_A_FSR                 (_ADI_MSK_3(0x00002000,0x00002000UL, uint32_t  ))    /*  Frame Sync Required */
#define BITM_SPORT_CTL_A_CKRE                (_ADI_MSK_3(0x00001000,0x00001000UL, uint32_t  ))    /*  Clock Rising Edge */
#define BITM_SPORT_CTL_A_OPMODE              (_ADI_MSK_3(0x00000800,0x00000800UL, uint32_t  ))    /*  Operation mode */
#define BITM_SPORT_CTL_A_ICLK                (_ADI_MSK_3(0x00000400,0x00000400UL, uint32_t  ))    /*  Internal Clock */
#define BITM_SPORT_CTL_A_SLEN                (_ADI_MSK_3(0x000001F0,0x000001F0UL, uint32_t  ))    /*  Serial Word Length */
#define BITM_SPORT_CTL_A_LSBF                (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  Least-Significant Bit First */
#define BITM_SPORT_CTL_A_CKMUXSEL            (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  Clock Multiplexer Select */
#define BITM_SPORT_CTL_A_FSMUXSEL            (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Frame Sync Multiplexer Select */
#define BITM_SPORT_CTL_A_SPEN                (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Serial Port Enable */
#define ENUM_SPORT_CTL_A_CTL_RX              (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  SPTRAN: Receive */
#define ENUM_SPORT_CTL_A_CTL_TX              (_ADI_MSK_3(0x02000000,0x02000000UL, uint32_t  ))    /*  SPTRAN: Transmit */
#define ENUM_SPORT_CTL_A_CTL_GCLK_DIS        (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  GCLKEN: Disable */
#define ENUM_SPORT_CTL_A_CTL_GCLK_EN         (_ADI_MSK_3(0x00200000,0x00200000UL, uint32_t  ))    /*  GCLKEN: Enable */
#define ENUM_SPORT_CTL_A_CTL_PACK_DIS        (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  PACK: Disable */
#define ENUM_SPORT_CTL_A_CTL_PACK_8BIT       (_ADI_MSK_3(0x00040000,0x00040000UL, uint32_t  ))    /*  PACK: 8-bit packing enable */
#define ENUM_SPORT_CTL_A_CTL_PACK_16BIT      (_ADI_MSK_3(0x00080000,0x00080000UL, uint32_t  ))    /*  PACK: 16-bit packing enable */
#define ENUM_SPORT_CTL_A_CTL_EARLY_FS        (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  LAFS: Early frame sync */
#define ENUM_SPORT_CTL_A_CTL_LATE_FS         (_ADI_MSK_3(0x00020000,0x00020000UL, uint32_t  ))    /*  LAFS: Late frame sync */
#define ENUM_SPORT_CTL_A_CTL_FS_LO           (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  LFS: Active high frame sync */
#define ENUM_SPORT_CTL_A_CTL_FS_HI           (_ADI_MSK_3(0x00010000,0x00010000UL, uint32_t  ))    /*  LFS: Active low frame sync */
#define ENUM_SPORT_CTL_A_CTL_DATA_DEP_FS     (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  DIFS: Data-dependent frame sync */
#define ENUM_SPORT_CTL_A_CTL_DATA_INDP_FS    (_ADI_MSK_3(0x00008000,0x00008000UL, uint32_t  ))    /*  DIFS: Data-independent frame sync */
#define ENUM_SPORT_CTL_A_CTL_EXTERNAL_FS     (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  IFS: External frame sync */
#define ENUM_SPORT_CTL_A_CTL_INTERNAL_FS     (_ADI_MSK_3(0x00004000,0x00004000UL, uint32_t  ))    /*  IFS: Internal frame sync */
#define ENUM_SPORT_CTL_A_CTL_FS_NOT_REQ      (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  FSR: No frame sync required */
#define ENUM_SPORT_CTL_A_CTL_FS_REQ          (_ADI_MSK_3(0x00002000,0x00002000UL, uint32_t  ))    /*  FSR: Frame sync required */
#define ENUM_SPORT_CTL_A_CTL_CLK_FALL_EDGE   (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  CKRE: Clock falling edge */
#define ENUM_SPORT_CTL_A_CTL_CLK_RISE_EDGE   (_ADI_MSK_3(0x00001000,0x00001000UL, uint32_t  ))    /*  CKRE: Clock rising edge */
#define ENUM_SPORT_CTL_A_CTL_SERIAL          (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  OPMODE: DSP standard */
#define ENUM_SPORT_CTL_A_CTL_TIMER_EN_MODE   (_ADI_MSK_3(0x00000800,0x00000800UL, uint32_t  ))    /*  OPMODE: Timer_enable mode */
#define ENUM_SPORT_CTL_A_CTL_EXTERNAL_CLK    (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  ICLK: External clock */
#define ENUM_SPORT_CTL_A_CTL_INTERNAL_CLK    (_ADI_MSK_3(0x00000400,0x00000400UL, uint32_t  ))    /*  ICLK: Internal clock */
#define ENUM_SPORT_CTL_A_CTL_MSB_FIRST       (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  LSBF: MSB first sent/received */
#define ENUM_SPORT_CTL_A_CTL_LSB_FIRST       (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  LSBF: LSB first sent/received */
#define ENUM_SPORT_CTL_A_CTL_CLK_MUX_DIS     (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  CKMUXSEL: Disable serial clock multiplexing */
#define ENUM_SPORT_CTL_A_CTL_CLK_MUX_EN      (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  CKMUXSEL: Enable serial clock multiplexing */
#define ENUM_SPORT_CTL_A_CTL_FS_MUX_DIS      (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  FSMUXSEL: Disable frame sync multiplexing */
#define ENUM_SPORT_CTL_A_CTL_FS_MUX_EN       (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  FSMUXSEL: Enable frame sync multiplexing */
#define ENUM_SPORT_CTL_A_CTL_DIS             (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  SPEN: Disable */
#define ENUM_SPORT_CTL_A_CTL_EN              (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  SPEN: Enable */

/* -------------------------------------------------------------------------------------------------------------------------
          SPORT_DIV_A                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPORT_DIV_A_FSDIV               16            /*  Frame Sync Divisor */
#define BITP_SPORT_DIV_A_CLKDIV               0            /*  Clock Divisor */
#define BITM_SPORT_DIV_A_FSDIV               (_ADI_MSK_3(0x00FF0000,0x00FF0000UL, uint32_t  ))    /*  Frame Sync Divisor */
#define BITM_SPORT_DIV_A_CLKDIV              (_ADI_MSK_3(0x0000FFFF,0x0000FFFFUL, uint32_t  ))    /*  Clock Divisor */

/* -------------------------------------------------------------------------------------------------------------------------
          SPORT_IEN_A                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPORT_IEN_A_SYSDATERR            4            /*  Data error for system writes or reads */
#define BITP_SPORT_IEN_A_DATA                 3            /*  Data request interrupt to the core */
#define BITP_SPORT_IEN_A_FSERRMSK             2            /*  Frame Sync Error (Interrupt) Mask */
#define BITP_SPORT_IEN_A_DERRMSK              1            /*  Data Error (Interrupt) Mask */
#define BITP_SPORT_IEN_A_TF                   0            /*  Transfer Finish Interrupt Enable */
#define BITM_SPORT_IEN_A_SYSDATERR           (_ADI_MSK_3(0x00000010,0x00000010UL, uint32_t  ))    /*  Data error for system writes or reads */
#define BITM_SPORT_IEN_A_DATA                (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  Data request interrupt to the core */
#define BITM_SPORT_IEN_A_FSERRMSK            (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  Frame Sync Error (Interrupt) Mask */
#define BITM_SPORT_IEN_A_DERRMSK             (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Data Error (Interrupt) Mask */
#define BITM_SPORT_IEN_A_TF                  (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Transfer Finish Interrupt Enable */
#define ENUM_SPORT_IEN_A_CTL_TXFIN_DIS       (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  TF: Transfer finish Interrupt is disabled */
#define ENUM_SPORT_IEN_A_CTL_TXFIN_EN        (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  TF: Transfer Finish Interrupt is Enabled */

/* -------------------------------------------------------------------------------------------------------------------------
          SPORT_STAT_A                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPORT_STAT_A_DXS                 8            /*  Data Transfer Buffer Status */
#define BITP_SPORT_STAT_A_SYSDATERR           4            /*  System Data Error Status */
#define BITP_SPORT_STAT_A_DATA                3            /*  Data Buffer status */
#define BITP_SPORT_STAT_A_FSERR               2            /*  Frame Sync Error Status */
#define BITP_SPORT_STAT_A_DERR                1            /*  Data Error Status */
#define BITP_SPORT_STAT_A_TFI                 0            /*  Transmit Finish Interrupt Status */
#define BITM_SPORT_STAT_A_DXS                (_ADI_MSK_3(0x00000300,0x00000300UL, uint32_t  ))    /*  Data Transfer Buffer Status */
#define BITM_SPORT_STAT_A_SYSDATERR          (_ADI_MSK_3(0x00000010,0x00000010UL, uint32_t  ))    /*  System Data Error Status */
#define BITM_SPORT_STAT_A_DATA               (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  Data Buffer status */
#define BITM_SPORT_STAT_A_FSERR              (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  Frame Sync Error Status */
#define BITM_SPORT_STAT_A_DERR               (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Data Error Status */
#define BITM_SPORT_STAT_A_TFI                (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Transmit Finish Interrupt Status */
#define ENUM_SPORT_STAT_A_CTL_EMPTY          (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  DXS: Empty */
#define ENUM_SPORT_STAT_A_CTL_PART_FULL      (_ADI_MSK_3(0x00000200,0x00000200UL, uint32_t  ))    /*  DXS: Partially full */
#define ENUM_SPORT_STAT_A_CTL_FULL           (_ADI_MSK_3(0x00000300,0x00000300UL, uint32_t  ))    /*  DXS: Full */

/* -------------------------------------------------------------------------------------------------------------------------
          SPORT_NUMTRAN_A                      Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPORT_NUMTRAN_A_VALUE            0            /*  Number of transfers (Half SPORT A) */
#define BITM_SPORT_NUMTRAN_A_VALUE           (_ADI_MSK_3(0x00000FFF,0x00000FFFUL, uint32_t  ))    /*  Number of transfers (Half SPORT A) */

/* -------------------------------------------------------------------------------------------------------------------------
          SPORT_CNVT_A                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPORT_CNVT_A_CNVT2FS            16            /*  CNV to FS duration: Half SPORT A */
#define BITP_SPORT_CNVT_A_POL                 8            /*  Polarity of the CNV signal */
#define BITP_SPORT_CNVT_A_WID                 0            /*  CNV signal width: Half SPORT A */
#define BITM_SPORT_CNVT_A_CNVT2FS            (_ADI_MSK_3(0x00FF0000,0x00FF0000UL, uint32_t  ))    /*  CNV to FS duration: Half SPORT A */
#define BITM_SPORT_CNVT_A_POL                (_ADI_MSK_3(0x00000100,0x00000100UL, uint32_t  ))    /*  Polarity of the CNV signal */
#define BITM_SPORT_CNVT_A_WID                (_ADI_MSK_3(0x0000000F,0x0000000FUL, uint32_t  ))    /*  CNV signal width: Half SPORT A */

/* -------------------------------------------------------------------------------------------------------------------------
          SPORT_TX_A                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPORT_TX_A_VALUE                 0            /*  Transmit Buffer */
#define BITM_SPORT_TX_A_VALUE                (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Transmit Buffer */

/* -------------------------------------------------------------------------------------------------------------------------
          SPORT_RX_A                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPORT_RX_A_VALUE                 0            /*  Receive Buffer */
#define BITM_SPORT_RX_A_VALUE                (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Receive Buffer */

/* -------------------------------------------------------------------------------------------------------------------------
          SPORT_CTL_B                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPORT_CTL_B_DMAEN               26            /*  DMA Enable */
#define BITP_SPORT_CTL_B_SPTRAN              25            /*  Serial Port Transfer Direction */
#define BITP_SPORT_CTL_B_GCLKEN              21            /*  Gated Clock Enable */
#define BITP_SPORT_CTL_B_FSERRMODE           20            /*  Frame Sync Error Operation */
#define BITP_SPORT_CTL_B_PACK                18            /*  Packing Enable */
#define BITP_SPORT_CTL_B_LAFS                17            /*  Late Frame Sync */
#define BITP_SPORT_CTL_B_LFS                 16            /*  Active-Low Frame Sync */
#define BITP_SPORT_CTL_B_DIFS                15            /*  Data-Independent Frame Sync */
#define BITP_SPORT_CTL_B_IFS                 14            /*  Internal Frame Sync */
#define BITP_SPORT_CTL_B_FSR                 13            /*  Frame Sync Required */
#define BITP_SPORT_CTL_B_CKRE                12            /*  Clock Rising Edge */
#define BITP_SPORT_CTL_B_OPMODE              11            /*  Operation mode */
#define BITP_SPORT_CTL_B_ICLK                10            /*  Internal Clock */
#define BITP_SPORT_CTL_B_SLEN                 4            /*  Serial Word Length */
#define BITP_SPORT_CTL_B_LSBF                 3            /*  Least-Significant Bit First */
#define BITP_SPORT_CTL_B_SPEN                 0            /*  Serial Port Enable */
#define BITM_SPORT_CTL_B_DMAEN               (_ADI_MSK_3(0x04000000,0x04000000UL, uint32_t  ))    /*  DMA Enable */
#define BITM_SPORT_CTL_B_SPTRAN              (_ADI_MSK_3(0x02000000,0x02000000UL, uint32_t  ))    /*  Serial Port Transfer Direction */
#define BITM_SPORT_CTL_B_GCLKEN              (_ADI_MSK_3(0x00200000,0x00200000UL, uint32_t  ))    /*  Gated Clock Enable */
#define BITM_SPORT_CTL_B_FSERRMODE           (_ADI_MSK_3(0x00100000,0x00100000UL, uint32_t  ))    /*  Frame Sync Error Operation */
#define BITM_SPORT_CTL_B_PACK                (_ADI_MSK_3(0x000C0000,0x000C0000UL, uint32_t  ))    /*  Packing Enable */
#define BITM_SPORT_CTL_B_LAFS                (_ADI_MSK_3(0x00020000,0x00020000UL, uint32_t  ))    /*  Late Frame Sync */
#define BITM_SPORT_CTL_B_LFS                 (_ADI_MSK_3(0x00010000,0x00010000UL, uint32_t  ))    /*  Active-Low Frame Sync */
#define BITM_SPORT_CTL_B_DIFS                (_ADI_MSK_3(0x00008000,0x00008000UL, uint32_t  ))    /*  Data-Independent Frame Sync */
#define BITM_SPORT_CTL_B_IFS                 (_ADI_MSK_3(0x00004000,0x00004000UL, uint32_t  ))    /*  Internal Frame Sync */
#define BITM_SPORT_CTL_B_FSR                 (_ADI_MSK_3(0x00002000,0x00002000UL, uint32_t  ))    /*  Frame Sync Required */
#define BITM_SPORT_CTL_B_CKRE                (_ADI_MSK_3(0x00001000,0x00001000UL, uint32_t  ))    /*  Clock Rising Edge */
#define BITM_SPORT_CTL_B_OPMODE              (_ADI_MSK_3(0x00000800,0x00000800UL, uint32_t  ))    /*  Operation mode */
#define BITM_SPORT_CTL_B_ICLK                (_ADI_MSK_3(0x00000400,0x00000400UL, uint32_t  ))    /*  Internal Clock */
#define BITM_SPORT_CTL_B_SLEN                (_ADI_MSK_3(0x000001F0,0x000001F0UL, uint32_t  ))    /*  Serial Word Length */
#define BITM_SPORT_CTL_B_LSBF                (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  Least-Significant Bit First */
#define BITM_SPORT_CTL_B_SPEN                (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Serial Port Enable */
#define ENUM_SPORT_CTL_B_CTL_PACK_DIS        (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  PACK: Disable */
#define ENUM_SPORT_CTL_B_CTL_PACK_8BIT       (_ADI_MSK_3(0x00040000,0x00040000UL, uint32_t  ))    /*  PACK: 8-bit packing enable */
#define ENUM_SPORT_CTL_B_CTL_PACK_16BIT      (_ADI_MSK_3(0x00080000,0x00080000UL, uint32_t  ))    /*  PACK: 16-bit packing enable */

/* -------------------------------------------------------------------------------------------------------------------------
          SPORT_DIV_B                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPORT_DIV_B_FSDIV               16            /*  Frame Sync Divisor */
#define BITP_SPORT_DIV_B_CLKDIV               0            /*  Clock Divisor */
#define BITM_SPORT_DIV_B_FSDIV               (_ADI_MSK_3(0x00FF0000,0x00FF0000UL, uint32_t  ))    /*  Frame Sync Divisor */
#define BITM_SPORT_DIV_B_CLKDIV              (_ADI_MSK_3(0x0000FFFF,0x0000FFFFUL, uint32_t  ))    /*  Clock Divisor */

/* -------------------------------------------------------------------------------------------------------------------------
          SPORT_IEN_B                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPORT_IEN_B_SYSDATERR            4            /*  Data error for system writes or reads */
#define BITP_SPORT_IEN_B_DATA                 3            /*  Data request interrupt to the core */
#define BITP_SPORT_IEN_B_FSERRMSK             2            /*  Frame Sync Error (Interrupt) Mask */
#define BITP_SPORT_IEN_B_DERRMSK              1            /*  Data Error (Interrupt) Mask */
#define BITP_SPORT_IEN_B_TF                   0            /*  Transmit Finish Interrupt Enable */
#define BITM_SPORT_IEN_B_SYSDATERR           (_ADI_MSK_3(0x00000010,0x00000010UL, uint32_t  ))    /*  Data error for system writes or reads */
#define BITM_SPORT_IEN_B_DATA                (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  Data request interrupt to the core */
#define BITM_SPORT_IEN_B_FSERRMSK            (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  Frame Sync Error (Interrupt) Mask */
#define BITM_SPORT_IEN_B_DERRMSK             (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Data Error (Interrupt) Mask */
#define BITM_SPORT_IEN_B_TF                  (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Transmit Finish Interrupt Enable */
#define ENUM_SPORT_IEN_B_CTL_TXFIN_DIS       (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  TF: Transfer Finish Interrupt is disabled */
#define ENUM_SPORT_IEN_B_CTL_TXFIN_EN        (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  TF: Transfer Finish Interrupt is Enabled */

/* -------------------------------------------------------------------------------------------------------------------------
          SPORT_STAT_B                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPORT_STAT_B_DXS                 8            /*  Data Transfer Buffer Status */
#define BITP_SPORT_STAT_B_SYSDATERR           4            /*  System Data Error Status */
#define BITP_SPORT_STAT_B_DATA                3            /*  Data Buffer status */
#define BITP_SPORT_STAT_B_FSERR               2            /*  Frame Sync Error Status */
#define BITP_SPORT_STAT_B_DERR                1            /*  Data Error Status */
#define BITP_SPORT_STAT_B_TFI                 0            /*  Transmit Finish Interrupt Status */
#define BITM_SPORT_STAT_B_DXS                (_ADI_MSK_3(0x00000300,0x00000300UL, uint32_t  ))    /*  Data Transfer Buffer Status */
#define BITM_SPORT_STAT_B_SYSDATERR          (_ADI_MSK_3(0x00000010,0x00000010UL, uint32_t  ))    /*  System Data Error Status */
#define BITM_SPORT_STAT_B_DATA               (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  Data Buffer status */
#define BITM_SPORT_STAT_B_FSERR              (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  Frame Sync Error Status */
#define BITM_SPORT_STAT_B_DERR               (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Data Error Status */
#define BITM_SPORT_STAT_B_TFI                (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Transmit Finish Interrupt Status */
#define ENUM_SPORT_STAT_B_CTL_EMPTY          (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  DXS: Empty */
#define ENUM_SPORT_STAT_B_CTL_PART_FULL      (_ADI_MSK_3(0x00000200,0x00000200UL, uint32_t  ))    /*  DXS: Partially full */
#define ENUM_SPORT_STAT_B_CTL_FULL           (_ADI_MSK_3(0x00000300,0x00000300UL, uint32_t  ))    /*  DXS: Full */

/* -------------------------------------------------------------------------------------------------------------------------
          SPORT_NUMTRAN_B                      Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPORT_NUMTRAN_B_VALUE            0            /*  Number of transfers (Half SPORT A) */
#define BITM_SPORT_NUMTRAN_B_VALUE           (_ADI_MSK_3(0x00000FFF,0x00000FFFUL, uint32_t  ))    /*  Number of transfers (Half SPORT A) */

/* -------------------------------------------------------------------------------------------------------------------------
          SPORT_CNVT_B                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPORT_CNVT_B_CNVT2FS            16            /*  CNV to FS duration: Half SPORT B */
#define BITP_SPORT_CNVT_B_POL                 8            /*  Polarity of the CNV signal */
#define BITP_SPORT_CNVT_B_WID                 0            /*  CNV signal width: Half SPORT B */
#define BITM_SPORT_CNVT_B_CNVT2FS            (_ADI_MSK_3(0x00FF0000,0x00FF0000UL, uint32_t  ))    /*  CNV to FS duration: Half SPORT B */
#define BITM_SPORT_CNVT_B_POL                (_ADI_MSK_3(0x00000100,0x00000100UL, uint32_t  ))    /*  Polarity of the CNV signal */
#define BITM_SPORT_CNVT_B_WID                (_ADI_MSK_3(0x0000000F,0x0000000FUL, uint32_t  ))    /*  CNV signal width: Half SPORT B */

/* -------------------------------------------------------------------------------------------------------------------------
          SPORT_TX_B                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPORT_TX_B_VALUE                 0            /*  Transmit Buffer */
#define BITM_SPORT_TX_B_VALUE                (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Transmit Buffer */

/* -------------------------------------------------------------------------------------------------------------------------
          SPORT_RX_B                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_SPORT_RX_B_VALUE                 0            /*  Receive Buffer */
#define BITM_SPORT_RX_B_VALUE                (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Receive Buffer */


/* ============================================================================================================================
        CRC Accelerator
   ============================================================================================================================ */

/* ============================================================================================================================
        CRC0
   ============================================================================================================================ */
#define REG_CRC0_CTL                         0x40040000            /*  CRC0 CRC Control */
#define REG_CRC0_IPDATA                      0x40040004            /*  CRC0 Input Data Word */
#define REG_CRC0_RESULT                      0x40040008            /*  CRC0 CRC Result */
#define REG_CRC0_POLY                        0x4004000C            /*  CRC0 Programmable CRC Polynomial */
#define REG_CRC0_IPBITS0                     0x40040010            /*  CRC0 Input Data Bits */
#define REG_CRC0_IPBITS1                     0x40040011            /*  CRC0 Input Data Bits */
#define REG_CRC0_IPBITS2                     0x40040012            /*  CRC0 Input Data Bits */
#define REG_CRC0_IPBITS3                     0x40040013            /*  CRC0 Input Data Bits */
#define REG_CRC0_IPBITS4                     0x40040014            /*  CRC0 Input Data Bits */
#define REG_CRC0_IPBITS5                     0x40040015            /*  CRC0 Input Data Bits */
#define REG_CRC0_IPBITS6                     0x40040016            /*  CRC0 Input Data Bits */
#define REG_CRC0_IPBITS7                     0x40040017            /*  CRC0 Input Data Bits */
#define REG_CRC0_IPBITSn(i)                  (REG_CRC0_IPBITS0 + ((i) * 1))
#define REG_CRC0_IPBITSn_COUNT               8
#define REG_CRC0_IPBYTE                      0x40040010            /*  CRC0 Input Data Byte */

/* ============================================================================================================================
        CRC Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          CRC_CTL                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRC_CTL_REVID                   28            /*  Revision ID */
#define BITP_CRC_CTL_W16SWP                   4            /*  Word16 Swap */
#define BITP_CRC_CTL_BYTMIRR                  3            /*  Byte Mirroring */
#define BITP_CRC_CTL_BITMIRR                  2            /*  Bit Mirroring */
#define BITP_CRC_CTL_LSBFIRST                 1            /*  LSB First Calculation Order */
#define BITP_CRC_CTL_EN                       0            /*  CRC Peripheral Enable */
#define BITM_CRC_CTL_REVID                   (_ADI_MSK_3(0xF0000000,0xF0000000UL, uint32_t  ))    /*  Revision ID */
#define BITM_CRC_CTL_W16SWP                  (_ADI_MSK_3(0x00000010,0x00000010UL, uint32_t  ))    /*  Word16 Swap */
#define BITM_CRC_CTL_BYTMIRR                 (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  Byte Mirroring */
#define BITM_CRC_CTL_BITMIRR                 (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  Bit Mirroring */
#define BITM_CRC_CTL_LSBFIRST                (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  LSB First Calculation Order */
#define BITM_CRC_CTL_EN                      (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  CRC Peripheral Enable */
#define ENUM_CRC_CTL_W16SP_DIS               (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  W16SWP: Word16 Swap disabled */
#define ENUM_CRC_CTL_W16SP_EN                (_ADI_MSK_3(0x00000010,0x00000010UL, uint32_t  ))    /*  W16SWP: Word16 Swap enabled */
#define ENUM_CRC_CTL_BYTEMIR_DIS             (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  BYTMIRR: Byte Mirroring is disabled */
#define ENUM_CRC_CTL_BYTEMIR_EN              (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  BYTMIRR: Byte Mirroring is enabled */
#define ENUM_CRC_CTL_BITMIRR_DIS             (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  BITMIRR: Bit Mirroring is disabled */
#define ENUM_CRC_CTL_BITMIRR_EN              (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  BITMIRR: Bit Mirroring is enabled */
#define ENUM_CRC_CTL_MSB_FIRST               (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  LSBFIRST: MSB First CRC calculation is done */
#define ENUM_CRC_CTL_LSB_FIRST               (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  LSBFIRST: LSB First CRC calculation is done */
#define ENUM_CRC_CTL_CRC_DIS                 (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  EN: CRC peripheral is disabled */
#define ENUM_CRC_CTL_CRC_EN                  (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  EN: CRC peripheral is enabled */

/* -------------------------------------------------------------------------------------------------------------------------
          CRC_IPDATA                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRC_IPDATA_VALUE                 0            /*  Data Input */
#define BITM_CRC_IPDATA_VALUE                (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Data Input */

/* -------------------------------------------------------------------------------------------------------------------------
          CRC_RESULT                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRC_RESULT_VALUE                 0            /*  CRC Residue */
#define BITM_CRC_RESULT_VALUE                (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  CRC Residue */

/* -------------------------------------------------------------------------------------------------------------------------
          CRC_POLY                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRC_POLY_VALUE                   0            /*  CRC Reduction Polynomial */
#define BITM_CRC_POLY_VALUE                  (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  CRC Reduction Polynomial */

/* -------------------------------------------------------------------------------------------------------------------------
          CRC_IPBITS[n]                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRC_IPBITS_DATA_BITS             0            /*  Input Data Bits */
#define BITM_CRC_IPBITS_DATA_BITS            (_ADI_MSK_3(0x000000FF,0x000000FFU, uint8_t   ))    /*  Input Data Bits */

/* -------------------------------------------------------------------------------------------------------------------------
          CRC_IPBYTE                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRC_IPBYTE_DATA_BYTE             0            /*  Input Data Byte */
#define BITM_CRC_IPBYTE_DATA_BYTE            (_ADI_MSK_3(0x000000FF,0x000000FFU, uint8_t   ))    /*  Input Data Byte */


/* ============================================================================================================================
        Random Number Generator
   ============================================================================================================================ */

/* ============================================================================================================================
        RNG0
   ============================================================================================================================ */
#define REG_RNG0_CTL                         0x40040400            /*  RNG0 RNG Control Register */
#define REG_RNG0_LEN                         0x40040404            /*  RNG0 RNG Sample Length Register */
#define REG_RNG0_STAT                        0x40040408            /*  RNG0 RNG Status Register */
#define REG_RNG0_DATA                        0x4004040C            /*  RNG0 RNG Data Register */
#define REG_RNG0_OSCCNT                      0x40040410            /*  RNG0 Oscillator Count */
#define REG_RNG0_OSCDIFF0                    0x40040414            /*  RNG0 Oscillator Difference */
#define REG_RNG0_OSCDIFF1                    0x40040415            /*  RNG0 Oscillator Difference */
#define REG_RNG0_OSCDIFF2                    0x40040416            /*  RNG0 Oscillator Difference */
#define REG_RNG0_OSCDIFF3                    0x40040417            /*  RNG0 Oscillator Difference */
#define REG_RNG0_OSCDIFFn(i)                 (REG_RNG0_OSCDIFF0 + ((i) * 1))
#define REG_RNG0_OSCDIFFn_COUNT              4

/* ============================================================================================================================
        RNG Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          RNG_CTL                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RNG_CTL_SINGLE                   3            /*  Generate a Single Number */
#define BITP_RNG_CTL_EN                       0            /*  RNG Enable */
#define BITM_RNG_CTL_SINGLE                  (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  Generate a Single Number */
#define BITM_RNG_CTL_EN                      (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  RNG Enable */
#define ENUM_RNG_CTL_WORD                    (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  SINGLE: Buffer Word */
#define ENUM_RNG_CTL_SINGLE                  (_ADI_MSK_3(0x00000008,0x00000008U, uint16_t  ))    /*  SINGLE: Single Byte */
#define ENUM_RNG_CTL_DISABLE                 (_ADI_MSK_3(0x00000000,0x00000000U, uint16_t  ))    /*  EN: Disable the RNG */
#define ENUM_RNG_CTL_ENABLE                  (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  EN: Enable the RNG */

/* -------------------------------------------------------------------------------------------------------------------------
          RNG_LEN                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RNG_LEN_PRESCALE                12            /*  Prescaler for the Sample Counter */
#define BITP_RNG_LEN_RELOAD                   0            /*  Reload Value for the Sample Counter */
#define BITM_RNG_LEN_PRESCALE                (_ADI_MSK_3(0x0000F000,0x0000F000U, uint16_t  ))    /*  Prescaler for the Sample Counter */
#define BITM_RNG_LEN_RELOAD                  (_ADI_MSK_3(0x00000FFF,0x00000FFFU, uint16_t  ))    /*  Reload Value for the Sample Counter */

/* -------------------------------------------------------------------------------------------------------------------------
          RNG_STAT                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RNG_STAT_STUCK                   1            /*  Sampled Data Stuck High or Low */
#define BITP_RNG_STAT_RNRDY                   0            /*  Random Number Ready */
#define BITM_RNG_STAT_STUCK                  (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Sampled Data Stuck High or Low */
#define BITM_RNG_STAT_RNRDY                  (_ADI_MSK_3(0x00000001,0x00000001U, uint16_t  ))    /*  Random Number Ready */

/* -------------------------------------------------------------------------------------------------------------------------
          RNG_DATA                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RNG_DATA_BUFF                    8            /*  Buffer for RNG Data */
#define BITP_RNG_DATA_VALUE                   0            /*  Value of the CRC Accumulator */
#define BITM_RNG_DATA_BUFF                   (_ADI_MSK_3(0xFFFFFF00,0xFFFFFF00UL, uint32_t  ))    /*  Buffer for RNG Data */
#define BITM_RNG_DATA_VALUE                  (_ADI_MSK_3(0x000000FF,0x000000FFUL, uint32_t  ))    /*  Value of the CRC Accumulator */

/* -------------------------------------------------------------------------------------------------------------------------
          RNG_OSCCNT                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RNG_OSCCNT_VALUE                 0            /*  Oscillator Count */
#define BITM_RNG_OSCCNT_VALUE                (_ADI_MSK_3(0x0FFFFFFF,0x0FFFFFFFUL, uint32_t  ))    /*  Oscillator Count */

/* -------------------------------------------------------------------------------------------------------------------------
          RNG_OSCDIFF[n]                       Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_RNG_OSCDIFF_DELTA                0            /*  Oscillator Count Difference */
#define BITM_RNG_OSCDIFF_DELTA               (_ADI_MSK_3(0x000000FF,0x000000FF, int8_t    ))    /*  Oscillator Count Difference */


/* ============================================================================================================================
        Register Map for the Crypto Block
   ============================================================================================================================ */

/* ============================================================================================================================
        CRYPT0
   ============================================================================================================================ */
#define REG_CRYPT0_CFG                       0x40044000            /*  CRYPT0 Configuration Register */
#define REG_CRYPT0_DATALEN                   0x40044004            /*  CRYPT0 Payload Data Length */
#define REG_CRYPT0_PREFIXLEN                 0x40044008            /*  CRYPT0 Authentication Data Length */
#define REG_CRYPT0_INTEN                     0x4004400C            /*  CRYPT0 Interrupt Enable Register */
#define REG_CRYPT0_STAT                      0x40044010            /*  CRYPT0 Status Register */
#define REG_CRYPT0_INBUF                     0x40044014            /*  CRYPT0 Input Buffer */
#define REG_CRYPT0_OUTBUF                    0x40044018            /*  CRYPT0 Output Buffer */
#define REG_CRYPT0_NONCE0                    0x4004401C            /*  CRYPT0 Nonce Bits [31:0] */
#define REG_CRYPT0_NONCE1                    0x40044020            /*  CRYPT0 Nonce Bits [63:32] */
#define REG_CRYPT0_NONCE2                    0x40044024            /*  CRYPT0 Nonce Bits [95:64] */
#define REG_CRYPT0_NONCE3                    0x40044028            /*  CRYPT0 Nonce Bits [127:96] */
#define REG_CRYPT0_AESKEY0                   0x4004402C            /*  CRYPT0 AES Key Bits [31:0] */
#define REG_CRYPT0_AESKEY1                   0x40044030            /*  CRYPT0 AES Key Bits [63:32] */
#define REG_CRYPT0_AESKEY2                   0x40044034            /*  CRYPT0 AES Key Bits [95:64] */
#define REG_CRYPT0_AESKEY3                   0x40044038            /*  CRYPT0 AES Key Bits [127:96] */
#define REG_CRYPT0_AESKEY4                   0x4004403C            /*  CRYPT0 AES Key Bits [159:128] */
#define REG_CRYPT0_AESKEY5                   0x40044040            /*  CRYPT0 AES Key Bits [191:160] */
#define REG_CRYPT0_AESKEY6                   0x40044044            /*  CRYPT0 AES Key Bits [223:192] */
#define REG_CRYPT0_AESKEY7                   0x40044048            /*  CRYPT0 AES Key Bits [255:224] */
#define REG_CRYPT0_CNTRINIT                  0x4004404C            /*  CRYPT0 Counter Initialization Vector */
#define REG_CRYPT0_SHAH0                     0x40044050            /*  CRYPT0 SHA Bits [31:0] */
#define REG_CRYPT0_SHAH1                     0x40044054            /*  CRYPT0 SHA Bits [63:32] */
#define REG_CRYPT0_SHAH2                     0x40044058            /*  CRYPT0 SHA Bits [95:64] */
#define REG_CRYPT0_SHAH3                     0x4004405C            /*  CRYPT0 SHA Bits [127:96] */
#define REG_CRYPT0_SHAH4                     0x40044060            /*  CRYPT0 SHA Bits [159:128] */
#define REG_CRYPT0_SHAH5                     0x40044064            /*  CRYPT0 SHA Bits [191:160] */
#define REG_CRYPT0_SHAH6                     0x40044068            /*  CRYPT0 SHA Bits [223:192] */
#define REG_CRYPT0_SHAH7                     0x4004406C            /*  CRYPT0 SHA Bits [255:224] */
#define REG_CRYPT0_SHA_LAST_WORD             0x40044070            /*  CRYPT0 SHA Last Word and Valid Bits Information */
#define REG_CRYPT0_CCM_NUM_VALID_BYTES       0x40044074            /*  CRYPT0 NUM_VALID_BYTES */
#define REG_CRYPT0_PRKSTORCFG                0x40044078            /*  CRYPT0 PRKSTOR Configuration */
#define REG_CRYPT0_KUW0                      0x40044080            /*  CRYPT0 Key Wrap Unwrap Register 0 */
#define REG_CRYPT0_KUW1                      0x40044084            /*  CRYPT0 Key Wrap Unwrap Register 1 */
#define REG_CRYPT0_KUW2                      0x40044088            /*  CRYPT0 Key Wrap Unwrap Register 2 */
#define REG_CRYPT0_KUW3                      0x4004408C            /*  CRYPT0 Key Wrap Unwrap Register 3 */
#define REG_CRYPT0_KUW4                      0x40044090            /*  CRYPT0 Key Wrap Unwrap Register 4 */
#define REG_CRYPT0_KUW5                      0x40044094            /*  CRYPT0 Key Wrap Unwrap Register 5 */
#define REG_CRYPT0_KUW6                      0x40044098            /*  CRYPT0 Key Wrap Unwrap Register 6 */
#define REG_CRYPT0_KUW7                      0x4004409C            /*  CRYPT0 Key Wrap Unwrap Register 7 */
#define REG_CRYPT0_KUW8                      0x400440A0            /*  CRYPT0 Key Wrap Unwrap Register 8 */
#define REG_CRYPT0_KUW9                      0x400440A4            /*  CRYPT0 Key Wrap Unwrap Register 9 */
#define REG_CRYPT0_KUW10                     0x400440A8            /*  CRYPT0 Key Wrap Unwrap Register 10 */
#define REG_CRYPT0_KUW11                     0x400440AC            /*  CRYPT0 Key Wrap Unwrap Register 11 */
#define REG_CRYPT0_KUW12                     0x400440B0            /*  CRYPT0 Key Wrap Unwrap Register 12 */
#define REG_CRYPT0_KUW13                     0x400440B4            /*  CRYPT0 Key Wrap Unwrap Register 13 */
#define REG_CRYPT0_KUW14                     0x400440B8            /*  CRYPT0 Key Wrap Unwrap Register 14 */
#define REG_CRYPT0_KUW15                     0x400440BC            /*  CRYPT0 Key Wrap Unwrap Register 15 */
#define REG_CRYPT0_KUWVALSTR1                0x400440C0            /*  CRYPT0 Key Wrap Unwrap Validation String [63:32] */
#define REG_CRYPT0_KUWVALSTR2                0x400440C4            /*  CRYPT0 Key Wrap Unwrap Validation String [31:0] */

/* ============================================================================================================================
        CRYPT Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_CFG                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_CFG_REVID                 28            /*  Rev ID for Crypto */
#define BITP_CRYPT_CFG_SHAINIT               26            /*  Restarts SHA Computation */
#define BITP_CRYPT_CFG_SHA256EN              25            /*  Enable SHA-256 Operation */
#define BITP_CRYPT_CFG_HMACEN                21            /*  HMAC Enable */
#define BITP_CRYPT_CFG_CMACEN                20            /*  Enable CMAC Mode Operation */
#define BITP_CRYPT_CFG_CCMEN                 19            /*  Enable CCM/CCM* Mode Operation */
#define BITP_CRYPT_CFG_CBCEN                 18            /*  Enable CBC Mode Operation */
#define BITP_CRYPT_CFG_CTREN                 17            /*  Enable CTR Mode Operation */
#define BITP_CRYPT_CFG_ECBEN                 16            /*  Enable ECB Mode Operation */
#define BITP_CRYPT_CFG_PRKSTOREN             15            /*  Enable PRKSTOR Commands */
#define BITP_CRYPT_CFG_KEY_BYTESWAP          14            /*  Use Key Unwrap Before HMAC */
#define BITP_CRYPT_CFG_SHA_BYTESWAP          13            /*  Enable Key Wrap */
#define BITP_CRYPT_CFG_AES_BYTESWAP          12            /*  Byteswap for AES Input */
#define BITP_CRYPT_CFG_KUWKEYLEN             10            /*  Key Length  Key Wrap Unwrap */
#define BITP_CRYPT_CFG_AESKEYLEN              8            /*  Select Key Length for AES Cipher */
#define BITP_CRYPT_CFG_OUTFLUSH               5            /*  Output Buffer Flush */
#define BITP_CRYPT_CFG_INFLUSH                4            /*  Input Buffer Flush */
#define BITP_CRYPT_CFG_OUTDMAEN               3            /*  Enable DMA Channel Request for Output Buffer */
#define BITP_CRYPT_CFG_INDMAEN                2            /*  Enable DMA Channel Request for Input Buffer */
#define BITP_CRYPT_CFG_ENCR                   1            /*  Encrypt or Decrypt */
#define BITP_CRYPT_CFG_BLKEN                  0            /*  Enable Bit for Crypto Block */
#define BITM_CRYPT_CFG_REVID                 (_ADI_MSK_3(0xF0000000,0xF0000000UL, uint32_t  ))    /*  Rev ID for Crypto */
#define BITM_CRYPT_CFG_SHAINIT               (_ADI_MSK_3(0x04000000,0x04000000UL, uint32_t  ))    /*  Restarts SHA Computation */
#define BITM_CRYPT_CFG_SHA256EN              (_ADI_MSK_3(0x02000000,0x02000000UL, uint32_t  ))    /*  Enable SHA-256 Operation */
#define BITM_CRYPT_CFG_HMACEN                (_ADI_MSK_3(0x00200000,0x00200000UL, uint32_t  ))    /*  HMAC Enable */
#define BITM_CRYPT_CFG_CMACEN                (_ADI_MSK_3(0x00100000,0x00100000UL, uint32_t  ))    /*  Enable CMAC Mode Operation */
#define BITM_CRYPT_CFG_CCMEN                 (_ADI_MSK_3(0x00080000,0x00080000UL, uint32_t  ))    /*  Enable CCM/CCM* Mode Operation */
#define BITM_CRYPT_CFG_CBCEN                 (_ADI_MSK_3(0x00040000,0x00040000UL, uint32_t  ))    /*  Enable CBC Mode Operation */
#define BITM_CRYPT_CFG_CTREN                 (_ADI_MSK_3(0x00020000,0x00020000UL, uint32_t  ))    /*  Enable CTR Mode Operation */
#define BITM_CRYPT_CFG_ECBEN                 (_ADI_MSK_3(0x00010000,0x00010000UL, uint32_t  ))    /*  Enable ECB Mode Operation */
#define BITM_CRYPT_CFG_PRKSTOREN             (_ADI_MSK_3(0x00008000,0x00008000UL, uint32_t  ))    /*  Enable PRKSTOR Commands */
#define BITM_CRYPT_CFG_KEY_BYTESWAP          (_ADI_MSK_3(0x00004000,0x00004000UL, uint32_t  ))    /*  Use Key Unwrap Before HMAC */
#define BITM_CRYPT_CFG_SHA_BYTESWAP          (_ADI_MSK_3(0x00002000,0x00002000UL, uint32_t  ))    /*  Enable Key Wrap */
#define BITM_CRYPT_CFG_AES_BYTESWAP          (_ADI_MSK_3(0x00001000,0x00001000UL, uint32_t  ))    /*  Byteswap for AES Input */
#define BITM_CRYPT_CFG_KUWKEYLEN             (_ADI_MSK_3(0x00000C00,0x00000C00UL, uint32_t  ))    /*  Key Length  Key Wrap Unwrap */
#define BITM_CRYPT_CFG_AESKEYLEN             (_ADI_MSK_3(0x00000300,0x00000300UL, uint32_t  ))    /*  Select Key Length for AES Cipher */
#define BITM_CRYPT_CFG_OUTFLUSH              (_ADI_MSK_3(0x00000020,0x00000020UL, uint32_t  ))    /*  Output Buffer Flush */
#define BITM_CRYPT_CFG_INFLUSH               (_ADI_MSK_3(0x00000010,0x00000010UL, uint32_t  ))    /*  Input Buffer Flush */
#define BITM_CRYPT_CFG_OUTDMAEN              (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  Enable DMA Channel Request for Output Buffer */
#define BITM_CRYPT_CFG_INDMAEN               (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  Enable DMA Channel Request for Input Buffer */
#define BITM_CRYPT_CFG_ENCR                  (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Encrypt or Decrypt */
#define BITM_CRYPT_CFG_BLKEN                 (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Enable Bit for Crypto Block */
#define ENUM_CRYPT_CFG_LEN128                (_ADI_MSK_3(0x00000400,0x00000400UL, uint32_t  ))    /*  KUWKeyLen: The key size of KUW key is 128 bits */
#define ENUM_CRYPT_CFG_LEN256                (_ADI_MSK_3(0x00000800,0x00000800UL, uint32_t  ))    /*  KUWKeyLen: The key size of KUW key is 256 bits */
#define ENUM_CRYPT_CFG_LEN512                (_ADI_MSK_3(0x00000C00,0x00000C00UL, uint32_t  ))    /*  KUWKeyLen: The key size of KUW key is 512 bits */
#define ENUM_CRYPT_CFG_AESKEYLEN128          (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  AESKEYLEN: Uses 128-bit long key */
#define ENUM_CRYPT_CFG_AESKEYLEN256          (_ADI_MSK_3(0x00000200,0x00000200UL, uint32_t  ))    /*  AESKEYLEN: Uses 256-bit long key */
#define ENUM_CRYPT_CFG_DMA_DISABLE_OUTBUF    (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  OUTDMAEN: Disable DMA Requesting for Output Buffer */
#define ENUM_CRYPT_CFG_DMA_ENABLE_OUTBUF     (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  OUTDMAEN: Enable DMA Requesting for Output Buffer */
#define ENUM_CRYPT_CFG_DMA_DISABLE_INBUF     (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  INDMAEN: Disable DMA Requesting for Input Buffer */
#define ENUM_CRYPT_CFG_DMA_ENABLE_INBUF      (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  INDMAEN: Enable DMA Requesting for Input Buffer */
#define ENUM_CRYPT_CFG_ENABLE                (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  BLKEN: Enable Crypto Block */
#define ENUM_CRYPT_CFG_DISABLE               (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  BLKEN: Disable Crypto Block */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_DATALEN                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_DATALEN_VALUE              0            /*  Length of Payload Data */
#define BITM_CRYPT_DATALEN_VALUE             (_ADI_MSK_3(0x000FFFFF,0x000FFFFFUL, uint32_t  ))    /*  Length of Payload Data */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_PREFIXLEN                      Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_PREFIXLEN_VALUE            0            /*  Length of Associated Data */
#define BITM_CRYPT_PREFIXLEN_VALUE           (_ADI_MSK_3(0x0000FFFF,0x0000FFFFUL, uint32_t  ))    /*  Length of Associated Data */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_INTEN                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_INTEN_PRKSTRCMDONEEN       8            /*  PRKSTOR CMD DONE INTEN */
#define BITP_CRYPT_INTEN_HMACMSGRDYEN         7            /*  Status Bit for HMAC Message Input Ready */
#define BITP_CRYPT_INTEN_HMACDONEEN           6            /*  Interrupt Enable for HMAC Done */
#define BITP_CRYPT_INTEN_SHADONEN             5            /*  Enable SHA_Done Interrupt */
#define BITP_CRYPT_INTEN_INOVREN              2            /*  Enable Input Overflow Interrupt */
#define BITP_CRYPT_INTEN_OUTRDYEN             1            /*  Enables the Output Ready Interrupt */
#define BITP_CRYPT_INTEN_INRDYEN              0            /*  Enable Input Ready Interrupt */
#define BITM_CRYPT_INTEN_PRKSTRCMDONEEN      (_ADI_MSK_3(0x00000100,0x00000100UL, uint32_t  ))    /*  PRKSTOR CMD DONE INTEN */
#define BITM_CRYPT_INTEN_HMACMSGRDYEN        (_ADI_MSK_3(0x00000080,0x00000080UL, uint32_t  ))    /*  Status Bit for HMAC Message Input Ready */
#define BITM_CRYPT_INTEN_HMACDONEEN          (_ADI_MSK_3(0x00000040,0x00000040UL, uint32_t  ))    /*  Interrupt Enable for HMAC Done */
#define BITM_CRYPT_INTEN_SHADONEN            (_ADI_MSK_3(0x00000020,0x00000020UL, uint32_t  ))    /*  Enable SHA_Done Interrupt */
#define BITM_CRYPT_INTEN_INOVREN             (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  Enable Input Overflow Interrupt */
#define BITM_CRYPT_INTEN_OUTRDYEN            (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Enables the Output Ready Interrupt */
#define BITM_CRYPT_INTEN_INRDYEN             (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Enable Input Ready Interrupt */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_STAT                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_STAT_PRKSTOR_BUSY         31            /*  Indicates PrKSTOR is Busy */
#define BITP_CRYPT_STAT_CMD_ISSUED           27            /*  Last Command Issued to PrKStor; */
#define BITP_CRYPT_STAT_PRKSTOR_RET_STATUS   25            /*  ECC Errors in the PRKSTOR_RETRIEVE Command */
#define BITP_CRYPT_STAT_PRKSTOR_CMD_FAIL     24            /*  Indicates Last Command Issued Failed */
#define BITP_CRYPT_STAT_PRKSTOR_CMD_DONE     23            /*  Indicates Command Done for PrKStor */
#define BITP_CRYPT_STAT_HMACMSGRDY           15            /*  Status Bit Indicates HMAC is Message Ready */
#define BITP_CRYPT_STAT_HMACDONE             14            /*  Status Bit Indicates HMAC Done */
#define BITP_CRYPT_STAT_HMACBUSY             13            /*  Status Bit Indicates HMAC Busy */
#define BITP_CRYPT_STAT_OUTWORDS             10            /*  Number of Words in the Output Buffer */
#define BITP_CRYPT_STAT_INWORDS               7            /*  Number of Words in the Input Buffer */
#define BITP_CRYPT_STAT_SHABUSY               6            /*  SHA Busy. in Computation */
#define BITP_CRYPT_STAT_SHADONE               5            /*  SHA Computation Complete */
#define BITP_CRYPT_STAT_INOVR                 2            /*  Overflow in the Input Buffer */
#define BITP_CRYPT_STAT_OUTRDY                1            /*  Output Data Ready */
#define BITP_CRYPT_STAT_INRDY                 0            /*  Input Buffer Status */
#define BITM_CRYPT_STAT_PRKSTOR_BUSY         (_ADI_MSK_3(0x80000000,0x80000000UL, uint32_t  ))    /*  Indicates PrKSTOR is Busy */
#define BITM_CRYPT_STAT_CMD_ISSUED           (_ADI_MSK_3(0x78000000,0x78000000UL, uint32_t  ))    /*  Last Command Issued to PrKStor; */
#define BITM_CRYPT_STAT_PRKSTOR_RET_STATUS   (_ADI_MSK_3(0x06000000,0x06000000UL, uint32_t  ))    /*  ECC Errors in the PRKSTOR_RETRIEVE Command */
#define BITM_CRYPT_STAT_PRKSTOR_CMD_FAIL     (_ADI_MSK_3(0x01000000,0x01000000UL, uint32_t  ))    /*  Indicates Last Command Issued Failed */
#define BITM_CRYPT_STAT_PRKSTOR_CMD_DONE     (_ADI_MSK_3(0x00800000,0x00800000UL, uint32_t  ))    /*  Indicates Command Done for PrKStor */
#define BITM_CRYPT_STAT_HMACMSGRDY           (_ADI_MSK_3(0x00008000,0x00008000UL, uint32_t  ))    /*  Status Bit Indicates HMAC is Message Ready */
#define BITM_CRYPT_STAT_HMACDONE             (_ADI_MSK_3(0x00004000,0x00004000UL, uint32_t  ))    /*  Status Bit Indicates HMAC Done */
#define BITM_CRYPT_STAT_HMACBUSY             (_ADI_MSK_3(0x00002000,0x00002000UL, uint32_t  ))    /*  Status Bit Indicates HMAC Busy */
#define BITM_CRYPT_STAT_OUTWORDS             (_ADI_MSK_3(0x00001C00,0x00001C00UL, uint32_t  ))    /*  Number of Words in the Output Buffer */
#define BITM_CRYPT_STAT_INWORDS              (_ADI_MSK_3(0x00000380,0x00000380UL, uint32_t  ))    /*  Number of Words in the Input Buffer */
#define BITM_CRYPT_STAT_SHABUSY              (_ADI_MSK_3(0x00000040,0x00000040UL, uint32_t  ))    /*  SHA Busy. in Computation */
#define BITM_CRYPT_STAT_SHADONE              (_ADI_MSK_3(0x00000020,0x00000020UL, uint32_t  ))    /*  SHA Computation Complete */
#define BITM_CRYPT_STAT_INOVR                (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  Overflow in the Input Buffer */
#define BITM_CRYPT_STAT_OUTRDY               (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Output Data Ready */
#define BITM_CRYPT_STAT_INRDY                (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Input Buffer Status */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_INBUF                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_INBUF_VALUE                0            /*  Input Buffer */
#define BITM_CRYPT_INBUF_VALUE               (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Input Buffer */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_OUTBUF                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_OUTBUF_VALUE               0            /*  Output Buffer */
#define BITM_CRYPT_OUTBUF_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Output Buffer */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_NONCE0                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_NONCE0_VALUE               0            /*  Word 0: Nonce Bits [31:0] */
#define BITM_CRYPT_NONCE0_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Word 0: Nonce Bits [31:0] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_NONCE1                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_NONCE1_VALUE               0            /*  Word 1: Nonce Bits [63:32] */
#define BITM_CRYPT_NONCE1_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Word 1: Nonce Bits [63:32] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_NONCE2                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_NONCE2_VALUE               0            /*  Word 2: Nonce Bits [95:64] */
#define BITM_CRYPT_NONCE2_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Word 2: Nonce Bits [95:64] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_NONCE3                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_NONCE3_VALUE               0            /*  Word 3: Nonce Bits [127:96] */
#define BITM_CRYPT_NONCE3_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Word 3: Nonce Bits [127:96] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_AESKEY0                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_AESKEY0_VALUE              0            /*  Key: Bytes [3:0] */
#define BITM_CRYPT_AESKEY0_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Key: Bytes [3:0] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_AESKEY1                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_AESKEY1_VALUE              0            /*  Key: Bytes [7:4] */
#define BITM_CRYPT_AESKEY1_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Key: Bytes [7:4] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_AESKEY2                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_AESKEY2_VALUE              0            /*  Key: Bytes [11:8] */
#define BITM_CRYPT_AESKEY2_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Key: Bytes [11:8] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_AESKEY3                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_AESKEY3_VALUE              0            /*  Key: Bytes [15:12] */
#define BITM_CRYPT_AESKEY3_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Key: Bytes [15:12] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_AESKEY4                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_AESKEY4_VALUE              0            /*  Key: Bytes [19:16] */
#define BITM_CRYPT_AESKEY4_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Key: Bytes [19:16] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_AESKEY5                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_AESKEY5_VALUE              0            /*  Key: Bytes [23:20] */
#define BITM_CRYPT_AESKEY5_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Key: Bytes [23:20] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_AESKEY6                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_AESKEY6_VALUE              0            /*  Key: Bytes [27:24] */
#define BITM_CRYPT_AESKEY6_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Key: Bytes [27:24] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_AESKEY7                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_AESKEY7_VALUE              0            /*  Key: Bytes [31:28] */
#define BITM_CRYPT_AESKEY7_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Key: Bytes [31:28] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_CNTRINIT                       Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_CNTRINIT_VALUE             0            /*  Counter Initialization Value */
#define BITM_CRYPT_CNTRINIT_VALUE            (_ADI_MSK_3(0x000FFFFF,0x000FFFFFUL, uint32_t  ))    /*  Counter Initialization Value */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_SHAH0                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_SHAH0_SHAHASH0             0            /*  Word 0: SHA Hash */
#define BITM_CRYPT_SHAH0_SHAHASH0            (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Word 0: SHA Hash */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_SHAH1                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_SHAH1_SHAHASH1             0            /*  Word 1: SHA Hash */
#define BITM_CRYPT_SHAH1_SHAHASH1            (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Word 1: SHA Hash */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_SHAH2                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_SHAH2_SHAHASH2             0            /*  Word 2: SHA Hash */
#define BITM_CRYPT_SHAH2_SHAHASH2            (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Word 2: SHA Hash */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_SHAH3                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_SHAH3_SHAHASH3             0            /*  Word 3: SHA Hash */
#define BITM_CRYPT_SHAH3_SHAHASH3            (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Word 3: SHA Hash */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_SHAH4                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_SHAH4_SHAHASH4             0            /*  Word 4: SHA Hash */
#define BITM_CRYPT_SHAH4_SHAHASH4            (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Word 4: SHA Hash */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_SHAH5                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_SHAH5_SHAHASH5             0            /*  Word 5: SHA Hash */
#define BITM_CRYPT_SHAH5_SHAHASH5            (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Word 5: SHA Hash */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_SHAH6                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_SHAH6_SHAHASH6             0            /*  Word 6: SHA Hash */
#define BITM_CRYPT_SHAH6_SHAHASH6            (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Word 6: SHA Hash */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_SHAH7                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_SHAH7_SHAHASH7             0            /*  Word 7: SHA Hash */
#define BITM_CRYPT_SHAH7_SHAHASH7            (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Word 7: SHA Hash */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_SHA_LAST_WORD                  Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_SHA_LAST_WORD_O_BITS_VALID  1            /*  Bits Valid in SHA Last Word Input */
#define BITP_CRYPT_SHA_LAST_WORD_O_LAST_WORD  0            /*  Last SHA Input Word */
#define BITM_CRYPT_SHA_LAST_WORD_O_BITS_VALID (_ADI_MSK_3(0x0000003E,0x0000003EUL, uint32_t  ))    /*  Bits Valid in SHA Last Word Input */
#define BITM_CRYPT_SHA_LAST_WORD_O_LAST_WORD (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Last SHA Input Word */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_CCM_NUM_VALID_BYTES            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_CCM_NUM_VALID_BYTES_NUM_VALID_BYTES  0            /*  Number of Valid Bytes in CCM Last Data */
#define BITM_CRYPT_CCM_NUM_VALID_BYTES_NUM_VALID_BYTES (_ADI_MSK_3(0x0000000F,0x0000000FUL, uint32_t  ))    /*  Number of Valid Bytes in CCM Last Data */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_PRKSTORCFG                     Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_PRKSTORCFG_CMD             7            /*  Command Input for PRKSTOR */
#define BITP_CRYPT_PRKSTORCFG_KEY_INDEX       0            /*  Index of Key in PRKSTOR */
#define BITM_CRYPT_PRKSTORCFG_CMD            (_ADI_MSK_3(0x00000780,0x00000780UL, uint32_t  ))    /*  Command Input for PRKSTOR */
#define BITM_CRYPT_PRKSTORCFG_KEY_INDEX      (_ADI_MSK_3(0x0000007F,0x0000007FUL, uint32_t  ))    /*  Index of Key in PRKSTOR */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_KUW0                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_KUW0_KUW0                  0            /*  KUW [31:0] */
#define BITM_CRYPT_KUW0_KUW0                 (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  KUW [31:0] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_KUW1                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_KUW1_KUW1                  0            /*  KUW [63:32] */
#define BITM_CRYPT_KUW1_KUW1                 (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  KUW [63:32] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_KUW2                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_KUW2_KUW2                  0            /*  KUW [95:64] */
#define BITM_CRYPT_KUW2_KUW2                 (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  KUW [95:64] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_KUW3                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_KUW3_KUW3                  0            /*  KUW [127:96] */
#define BITM_CRYPT_KUW3_KUW3                 (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  KUW [127:96] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_KUW4                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_KUW4_KUW4                  0            /*  KUW [159:128] */
#define BITM_CRYPT_KUW4_KUW4                 (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  KUW [159:128] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_KUW5                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_KUW5_KUW5                  0            /*  KUW [191:160] */
#define BITM_CRYPT_KUW5_KUW5                 (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  KUW [191:160] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_KUW6                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_KUW6_KUW6                  0            /*  KUW [223:192] */
#define BITM_CRYPT_KUW6_KUW6                 (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  KUW [223:192] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_KUW7                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_KUW7_KUW7                  0            /*  KUW [255:224] */
#define BITM_CRYPT_KUW7_KUW7                 (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  KUW [255:224] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_KUW8                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_KUW8_KUW8                  0            /*  KUW [287:256] */
#define BITM_CRYPT_KUW8_KUW8                 (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  KUW [287:256] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_KUW9                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_KUW9_KUW9                  0            /*  KUW [319:288] */
#define BITM_CRYPT_KUW9_KUW9                 (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  KUW [319:288] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_KUW10                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_KUW10_KUW10                0            /*  KUW [351:320] */
#define BITM_CRYPT_KUW10_KUW10               (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  KUW [351:320] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_KUW11                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_KUW11_KUW11                0            /*  KUW [383:352] */
#define BITM_CRYPT_KUW11_KUW11               (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  KUW [383:352] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_KUW12                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_KUW12_KUW12                0            /*  KUW [415:384] */
#define BITM_CRYPT_KUW12_KUW12               (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  KUW [415:384] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_KUW13                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_KUW13_KUW13                0            /*  KUW [447:416] */
#define BITM_CRYPT_KUW13_KUW13               (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  KUW [447:416] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_KUW14                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_KUW14_KUW14                0            /*  KUW [479:448] */
#define BITM_CRYPT_KUW14_KUW14               (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  KUW [479:448] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_KUW15                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_KUW15_KUW15                0            /*  KUW [511:480] */
#define BITM_CRYPT_KUW15_KUW15               (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  KUW [511:480] */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_KUWVALSTR1                     Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_KUWVALSTR1_INITALVALUE0    0            /*  Initial Value */
#define BITM_CRYPT_KUWVALSTR1_INITALVALUE0   (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Initial Value */

/* -------------------------------------------------------------------------------------------------------------------------
          CRYPT_KUWVALSTR2                     Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CRYPT_KUWVALSTR2_INITIALVALUE1   0            /*  Initial Value */
#define BITM_CRYPT_KUWVALSTR2_INITIALVALUE1  (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Initial Value */


/* ============================================================================================================================
        Power Management Registers
   ============================================================================================================================ */

/* ============================================================================================================================
        PMG0
   ============================================================================================================================ */
#define REG_PMG0_IEN                         0x4004C000            /*  PMG0 Power Supply Monitor Interrupt Enable */
#define REG_PMG0_PSM_STAT                    0x4004C004            /*  PMG0 Power Supply Monitor Status */
#define REG_PMG0_PWRMOD                      0x4004C008            /*  PMG0 Power Mode Register */
#define REG_PMG0_PWRKEY                      0x4004C00C            /*  PMG0 Key Protection for PWRMOD and  SRAMRET */
#define REG_PMG0_SHDN_STAT                   0x4004C010            /*  PMG0 Shutdown Status Register */
#define REG_PMG0_SRAMRET                     0x4004C014            /*  PMG0 Control for Retention SRAM in Hibernate Mode */
#define REG_PMG0_TRIM                        0x4004C038            /*  PMG0 Trimming Bits */
#define REG_PMG0_RST_STAT                    0x4004C040            /*  PMG0 Reset Status */
#define REG_PMG0_CTL1                        0x4004C044            /*  PMG0 HPBUCK Control */

/* ============================================================================================================================
        PMG Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          PMG_IEN                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_PMG_IEN_IENBAT                  10            /*  Interrupt enable for VBAT range */
#define BITP_PMG_IEN_RANGEBAT                 8            /*  Battery Monitor Range */
#define BITP_PMG_IEN_VREGOVR                  2            /*  Enable Interrupt when VREG over-voltage (above 1.32 V) */
#define BITP_PMG_IEN_VREGUNDR                 1            /*  Enable Interrupt when VREG under-voltage (below 1 V) */
#define BITP_PMG_IEN_VBAT                     0            /*  Enable Interrupt for VBAT */
#define BITM_PMG_IEN_IENBAT                  (_ADI_MSK_3(0x00000400,0x00000400UL, uint32_t  ))    /*  Interrupt enable for VBAT range */
#define BITM_PMG_IEN_RANGEBAT                (_ADI_MSK_3(0x00000300,0x00000300UL, uint32_t  ))    /*  Battery Monitor Range */
#define BITM_PMG_IEN_VREGOVR                 (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  Enable Interrupt when VREG over-voltage (above 1.32 V) */
#define BITM_PMG_IEN_VREGUNDR                (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Enable Interrupt when VREG under-voltage (below 1 V) */
#define BITM_PMG_IEN_VBAT                    (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Enable Interrupt for VBAT */
#define ENUM_PMG_IEN_REGION1                 (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  RANGEBAT: Configure to generate interrupt if VBAT in Region1 */
#define ENUM_PMG_IEN_REGION2                 (_ADI_MSK_3(0x00000100,0x00000100UL, uint32_t  ))    /*  RANGEBAT: Configure to generate interrupt if VBAT in Region2 */
#define ENUM_PMG_IEN_REGION3                 (_ADI_MSK_3(0x00000200,0x00000200UL, uint32_t  ))    /*  RANGEBAT: Configure to generate interrupt if VBAT in Region3 */
#define ENUM_PMG_IEN_NA                      (_ADI_MSK_3(0x00000300,0x00000300UL, uint32_t  ))    /*  RANGEBAT: NA */

/* -------------------------------------------------------------------------------------------------------------------------
          PMG_PSM_STAT                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_PMG_PSM_STAT_RORANGE3           15            /*  VBAT range3 */
#define BITP_PMG_PSM_STAT_RORANGE2           14            /*  VBAT range2 */
#define BITP_PMG_PSM_STAT_RORANGE1           13            /*  VBAT range1 */
#define BITP_PMG_PSM_STAT_RANGE3             10            /*  VBAT range3 */
#define BITP_PMG_PSM_STAT_RANGE2              9            /*  VBAT range2 */
#define BITP_PMG_PSM_STAT_RANGE1              8            /*  VBAT range1 */
#define BITP_PMG_PSM_STAT_WICENACK            7            /*  WIC Enable Acknowledge from Cortex */
#define BITP_PMG_PSM_STAT_VREGOVR             2            /*  Status bit for alarm indicating Over Voltage for VREG */
#define BITP_PMG_PSM_STAT_VREGUNDR            1            /*  Status bit for Alarm indicating VREG is below 1 V */
#define BITP_PMG_PSM_STAT_VBATUNDR            0            /*  Status bit indicating an Alarm that battery is below 1.8 V */
#define BITM_PMG_PSM_STAT_RORANGE3           (_ADI_MSK_3(0x00008000,0x00008000UL, uint32_t  ))    /*  VBAT range3 */
#define BITM_PMG_PSM_STAT_RORANGE2           (_ADI_MSK_3(0x00004000,0x00004000UL, uint32_t  ))    /*  VBAT range2 */
#define BITM_PMG_PSM_STAT_RORANGE1           (_ADI_MSK_3(0x00002000,0x00002000UL, uint32_t  ))    /*  VBAT range1 */
#define BITM_PMG_PSM_STAT_RANGE3             (_ADI_MSK_3(0x00000400,0x00000400UL, uint32_t  ))    /*  VBAT range3 */
#define BITM_PMG_PSM_STAT_RANGE2             (_ADI_MSK_3(0x00000200,0x00000200UL, uint32_t  ))    /*  VBAT range2 */
#define BITM_PMG_PSM_STAT_RANGE1             (_ADI_MSK_3(0x00000100,0x00000100UL, uint32_t  ))    /*  VBAT range1 */
#define BITM_PMG_PSM_STAT_WICENACK           (_ADI_MSK_3(0x00000080,0x00000080UL, uint32_t  ))    /*  WIC Enable Acknowledge from Cortex */
#define BITM_PMG_PSM_STAT_VREGOVR            (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  Status bit for alarm indicating Over Voltage for VREG */
#define BITM_PMG_PSM_STAT_VREGUNDR           (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Status bit for Alarm indicating VREG is below 1 V */
#define BITM_PMG_PSM_STAT_VBATUNDR           (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Status bit indicating an Alarm that battery is below 1.8 V */
#define ENUM_PMG_PSM_STAT_BATSTAT1           (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  RORANGE1: VBAT NOT in the range specified */
#define ENUM_PMG_PSM_STAT_BATSTAT0           (_ADI_MSK_3(0x00002000,0x00002000UL, uint32_t  ))    /*  RORANGE1: VBAT in the range specified */

/* -------------------------------------------------------------------------------------------------------------------------
          PMG_PWRMOD                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_PMG_PWRMOD_MONVBATN              3            /*  Monitor VBAT during Hibernate Mode. Monitors VBAT by default */
#define BITP_PMG_PWRMOD_MODE                  0            /*  Power Mode Bits */
#define BITM_PMG_PWRMOD_MONVBATN             (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  Monitor VBAT during Hibernate Mode. Monitors VBAT by default */
#define BITM_PMG_PWRMOD_MODE                 (_ADI_MSK_3(0x00000003,0x00000003UL, uint32_t  ))    /*  Power Mode Bits */
#define ENUM_PMG_PWRMOD_FLEXI                (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  MODE: Flexi Mode */
#define ENUM_PMG_PWRMOD_HIBERNATE            (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  MODE: Hibernate Mode */
#define ENUM_PMG_PWRMOD_SHUTDOWN             (_ADI_MSK_3(0x00000003,0x00000003UL, uint32_t  ))    /*  MODE: Shutdown Mode */

/* -------------------------------------------------------------------------------------------------------------------------
          PMG_PWRKEY                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_PMG_PWRKEY_VALUE                 0            /*  Power Control Key */
#define BITM_PMG_PWRKEY_VALUE                (_ADI_MSK_3(0x0000FFFF,0x0000FFFFUL, uint32_t  ))    /*  Power Control Key */

/* -------------------------------------------------------------------------------------------------------------------------
          PMG_SHDN_STAT                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_PMG_SHDN_STAT_RTC                3            /*  Interrupt from RTC */
#define BITP_PMG_SHDN_STAT_EXTINT2            2            /*  Interrupt from External Interrupt 2 */
#define BITP_PMG_SHDN_STAT_EXTINT1            1            /*  Interrupt from External Interrupt 1 */
#define BITP_PMG_SHDN_STAT_EXTINT0            0            /*  Interrupt from External Interrupt 0 */
#define BITM_PMG_SHDN_STAT_RTC               (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  Interrupt from RTC */
#define BITM_PMG_SHDN_STAT_EXTINT2           (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  Interrupt from External Interrupt 2 */
#define BITM_PMG_SHDN_STAT_EXTINT1           (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Interrupt from External Interrupt 1 */
#define BITM_PMG_SHDN_STAT_EXTINT0           (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Interrupt from External Interrupt 0 */

/* -------------------------------------------------------------------------------------------------------------------------
          PMG_SRAMRET                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_PMG_SRAMRET_HIBERNATE_SRAM_LOAD_MODE 23            /*  Hibernate mode SRAM load mode control */
#define BITP_PMG_SRAMRET_RET4                 9            /*  Enable retention bank 6 and bank 7 (32 KB) */
#define BITP_PMG_SRAMRET_RET3                 8            /*  Enable retention bank 5 (32 KB) */
#define BITP_PMG_SRAMRET_RET2                 1            /*  Enable retention bank 3 and bank 4 (32 KB) */
#define BITP_PMG_SRAMRET_RET1                 0            /*  Enable retention bank 1 (12 KB) */
#define BITM_PMG_SRAMRET_HIBERNATE_SRAM_LOAD_MODE (_ADI_MSK_3(0x00800000,0x00800000UL, uint32_t  ))    /*  Hibernate mode SRAM load mode control */
#define BITM_PMG_SRAMRET_RET4                (_ADI_MSK_3(0x00000200,0x00000200UL, uint32_t  ))    /*  Enable retention bank 6 and bank 7 (32 KB) */
#define BITM_PMG_SRAMRET_RET3                (_ADI_MSK_3(0x00000100,0x00000100UL, uint32_t  ))    /*  Enable retention bank 5 (32 KB) */
#define BITM_PMG_SRAMRET_RET2                (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Enable retention bank 3 and bank 4 (32 KB) */
#define BITM_PMG_SRAMRET_RET1                (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Enable retention bank 1 (12 KB) */

/* -------------------------------------------------------------------------------------------------------------------------
          PMG_TRIM                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_PMG_TRIM_HIBERNATE_LOAD_MODE    29            /*  Hibernate mode load mode control */
#define BITM_PMG_TRIM_HIBERNATE_LOAD_MODE    (_ADI_MSK_3(0xE0000000,0xE0000000UL, uint32_t  ))    /*  Hibernate mode load mode control */
#define ENUM_PMG_TRIM_HIGH_LOAD              (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  hibernate_load_mode: High hibernate load */
#define ENUM_PMG_TRIM_LOW_LOAD               (_ADI_MSK_3(0xE0000000,0xE0000000UL, uint32_t  ))    /*  hibernate_load_mode: Low hibernate load */

/* -------------------------------------------------------------------------------------------------------------------------
          PMG_RST_STAT                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_PMG_RST_STAT_PORSRC              4            /*  Power on reset Source */
#define BITP_PMG_RST_STAT_SWRST               3            /*  Software reset */
#define BITP_PMG_RST_STAT_WDRST               2            /*  Watchdog timeout */
#define BITP_PMG_RST_STAT_EXTRST              1            /*  External reset */
#define BITP_PMG_RST_STAT_POR                 0            /*  Power-on reset */
#define BITM_PMG_RST_STAT_PORSRC             (_ADI_MSK_3(0x00000030,0x00000030UL, uint32_t  ))    /*  Power on reset Source */
#define BITM_PMG_RST_STAT_SWRST              (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  Software reset */
#define BITM_PMG_RST_STAT_WDRST              (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  Watchdog timeout */
#define BITM_PMG_RST_STAT_EXTRST             (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  External reset */
#define BITM_PMG_RST_STAT_POR                (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Power-on reset */
#define ENUM_PMG_RST_STAT_FAILSAFE_HV        (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  PORSRC: POR triggered because VBAT drops below Fail Safe */
#define ENUM_PMG_RST_STAT_RST_VBAT           (_ADI_MSK_3(0x00000010,0x00000010UL, uint32_t  ))    /*  PORSRC: POR trigger because VBAT supply (VBAT < 1.7 V) */
#define ENUM_PMG_RST_STAT_RST_VREG           (_ADI_MSK_3(0x00000020,0x00000020UL, uint32_t  ))    /*  PORSRC: POR triggered because VDD supply (VDD < 1.08 V) */
#define ENUM_PMG_RST_STAT_FAILSAFE_LV        (_ADI_MSK_3(0x00000030,0x00000030UL, uint32_t  ))    /*  PORSRC: POR triggered because VREG drops below Fail Safe */

/* -------------------------------------------------------------------------------------------------------------------------
          PMG_CTL1                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_PMG_CTL1_HPBUCK_LOWPWR_MODE      2            /*  HP Buck low power mode */
#define BITP_PMG_CTL1_HPBUCK_LD_MODE          1            /*  HP Buck load mode */
#define BITP_PMG_CTL1_HPBUCKEN                0            /*  Enable HP Buck */
#define BITM_PMG_CTL1_HPBUCK_LOWPWR_MODE     (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  HP Buck low power mode */
#define BITM_PMG_CTL1_HPBUCK_LD_MODE         (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  HP Buck load mode */
#define BITM_PMG_CTL1_HPBUCKEN               (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Enable HP Buck */
#define ENUM_PMG_CTL1_LOWPWRDISABLE          (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  HPBUCK_LOWPWR_MODE: HPBUCK Low power mode is disabled */
#define ENUM_PMG_CTL1_LOWPWRENABLE           (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  HPBUCK_LOWPWR_MODE: HPBUCK Low power mode is enabled */
#define ENUM_PMG_CTL1_HPBUCKLOWLOAD          (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  HPBUCK_LD_MODE: HPBUCK Low load mode is enabled */
#define ENUM_PMG_CTL1_HPBUCKHIGHLOAD         (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  HPBUCK_LD_MODE: HPBUCK High load mode is enabled */


/* ============================================================================================================================
        External interrupt configuration
   ============================================================================================================================ */

/* ============================================================================================================================
        XINT0
   ============================================================================================================================ */
#define REG_XINT0_CFG0                       0x4004C080            /*  XINT0 External Interrupt configuration */
#define REG_XINT0_EXT_STAT                   0x4004C084            /*  XINT0 External Wakeup Interrupt Status register */
#define REG_XINT0_CLR                        0x4004C090            /*  XINT0 External Interrupt clear */
#define REG_XINT0_NMICLR                     0x4004C094            /*  XINT0 Non-maskable interrupt clear */

/* ============================================================================================================================
        XINT Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          XINT_CFG0                            Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_XINT_CFG0_UART_RX_MDE           21            /*  External Interrupt using UART_RX wakeup Mode registers */
#define BITP_XINT_CFG0_UART_RX_EN            20            /*  External Interrupt using SIP_UPDATE enable bit */
#define BITP_XINT_CFG0_IRQ3EN                15            /*  External Interrupt 3 enable bit */
#define BITP_XINT_CFG0_IRQ3MDE               12            /*  External Interrupt 3 Mode registers */
#define BITP_XINT_CFG0_IRQ2EN                11            /*  External Interrupt 2 Enable bit */
#define BITP_XINT_CFG0_IRQ2MDE                8            /*  External Interrupt 2 Mode registers */
#define BITP_XINT_CFG0_IRQ1EN                 7            /*  External Interrupt 1 Enable bit */
#define BITP_XINT_CFG0_IRQ1MDE                4            /*  External Interrupt 1 Mode registers */
#define BITP_XINT_CFG0_IRQ0EN                 3            /*  External Interrupt 0 Enable bit */
#define BITP_XINT_CFG0_IRQ0MDE                0            /*  External Interrupt 0 Mode registers */
#define BITM_XINT_CFG0_UART_RX_MDE           (_ADI_MSK_3(0x00E00000,0x00E00000UL, uint32_t  ))    /*  External Interrupt using UART_RX wakeup Mode registers */
#define BITM_XINT_CFG0_UART_RX_EN            (_ADI_MSK_3(0x00100000,0x00100000UL, uint32_t  ))    /*  External Interrupt using SIP_UPDATE enable bit */
#define BITM_XINT_CFG0_IRQ3EN                (_ADI_MSK_3(0x00008000,0x00008000UL, uint32_t  ))    /*  External Interrupt 3 enable bit */
#define BITM_XINT_CFG0_IRQ3MDE               (_ADI_MSK_3(0x00007000,0x00007000UL, uint32_t  ))    /*  External Interrupt 3 Mode registers */
#define BITM_XINT_CFG0_IRQ2EN                (_ADI_MSK_3(0x00000800,0x00000800UL, uint32_t  ))    /*  External Interrupt 2 Enable bit */
#define BITM_XINT_CFG0_IRQ2MDE               (_ADI_MSK_3(0x00000700,0x00000700UL, uint32_t  ))    /*  External Interrupt 2 Mode registers */
#define BITM_XINT_CFG0_IRQ1EN                (_ADI_MSK_3(0x00000080,0x00000080UL, uint32_t  ))    /*  External Interrupt 1 Enable bit */
#define BITM_XINT_CFG0_IRQ1MDE               (_ADI_MSK_3(0x00000070,0x00000070UL, uint32_t  ))    /*  External Interrupt 1 Mode registers */
#define BITM_XINT_CFG0_IRQ0EN                (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  External Interrupt 0 Enable bit */
#define BITM_XINT_CFG0_IRQ0MDE               (_ADI_MSK_3(0x00000007,0x00000007UL, uint32_t  ))    /*  External Interrupt 0 Mode registers */

/* -------------------------------------------------------------------------------------------------------------------------
          XINT_EXT_STAT                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_XINT_EXT_STAT_STAT_UART_RXWKUP   5            /*  Interrupt status bit for UART RX WAKEUP interrupt */
#define BITP_XINT_EXT_STAT_STAT_EXTINT3       3            /*  Interrupt status bit for External Interrupt 3 */
#define BITP_XINT_EXT_STAT_STAT_EXTINT2       2            /*  Interrupt status bit for External Interrupt 2 */
#define BITP_XINT_EXT_STAT_STAT_EXTINT1       1            /*  Interrupt status bit for External Interrupt 1 */
#define BITP_XINT_EXT_STAT_STAT_EXTINT0       0            /*  Interrupt status bit for External Interrupt 0 */
#define BITM_XINT_EXT_STAT_STAT_UART_RXWKUP  (_ADI_MSK_3(0x00000020,0x00000020UL, uint32_t  ))    /*  Interrupt status bit for UART RX WAKEUP interrupt */
#define BITM_XINT_EXT_STAT_STAT_EXTINT3      (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  Interrupt status bit for External Interrupt 3 */
#define BITM_XINT_EXT_STAT_STAT_EXTINT2      (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  Interrupt status bit for External Interrupt 2 */
#define BITM_XINT_EXT_STAT_STAT_EXTINT1      (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Interrupt status bit for External Interrupt 1 */
#define BITM_XINT_EXT_STAT_STAT_EXTINT0      (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Interrupt status bit for External Interrupt 0 */

/* -------------------------------------------------------------------------------------------------------------------------
          XINT_CLR                             Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_XINT_CLR_UART_RX_CLR             5            /*  External interrupt Clear for UART_RX WAKEUP interrupt */
#define BITP_XINT_CLR_IRQ3                    3            /*  External interrupt 3 */
#define BITP_XINT_CLR_IRQ2                    2            /*  External interrupt 2 */
#define BITP_XINT_CLR_IRQ1                    1            /*  External interrupt 1 */
#define BITP_XINT_CLR_IRQ0                    0            /*  External interrupt 0 */
#define BITM_XINT_CLR_UART_RX_CLR            (_ADI_MSK_3(0x00000020,0x00000020UL, uint32_t  ))    /*  External interrupt Clear for UART_RX WAKEUP interrupt */
#define BITM_XINT_CLR_IRQ3                   (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  External interrupt 3 */
#define BITM_XINT_CLR_IRQ2                   (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  External interrupt 2 */
#define BITM_XINT_CLR_IRQ1                   (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  External interrupt 1 */
#define BITM_XINT_CLR_IRQ0                   (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  External interrupt 0 */

/* -------------------------------------------------------------------------------------------------------------------------
          XINT_NMICLR                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_XINT_NMICLR_CLR                  0            /*  NMI clear */
#define BITM_XINT_NMICLR_CLR                 (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  NMI clear */


/* ============================================================================================================================
        Clocking registers
   ============================================================================================================================ */

/* ============================================================================================================================
        CLKG0_OSC
   ============================================================================================================================ */
#define REG_CLKG0_OSC_KEY                    0x4004C10C            /*  CLKG0_OSC Key Protection for OSCCTRL */
#define REG_CLKG0_OSC_CTL                    0x4004C110            /*  CLKG0_OSC Oscillator Control */

/* ============================================================================================================================
        CLKG_OSC Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          CLKG_OSC_KEY                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CLKG_OSC_KEY_VALUE               0            /*  Oscillator key */
#define BITM_CLKG_OSC_KEY_VALUE              (_ADI_MSK_3(0x0000FFFF,0x0000FFFFUL, uint32_t  ))    /*  Oscillator key */

/* -------------------------------------------------------------------------------------------------------------------------
          CLKG_OSC_CTL                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CLKG_OSC_CTL_LFX_FAIL_STA       31            /*  LF XTAL (crystal clock) Not Stable */
#define BITP_CLKG_OSC_CTL_ROOT_FAIL_STA      30            /*  Root clock (crystal clock) Not Stable */
#define BITP_CLKG_OSC_CTL_ROOT_AUTSW_STA     22            /*  Status of automatic switching of the Root clock to HFOSC upon detection of Root clock failure */
#define BITP_CLKG_OSC_CTL_ROOT_AUTSW_EN      21            /*  Enables automatic Switching of the Root clock  to HFOSC on Root clock Failure */
#define BITP_CLKG_OSC_CTL_ROOT_MON_EN        20            /*  ROOT clock monitor and Clock FAIL interrupt enable */
#define BITP_CLKG_OSC_CTL_LFX_ROBUST_LD      15            /*  LFXTAL Robust Mode Load select */
#define BITP_CLKG_OSC_CTL_LFX_ROBUST_EN      14            /*  LFXTAL Mode select */
#define BITP_CLKG_OSC_CTL_LFX_AUTSW_STA      13            /*  Status of automatic switching of the LF Mux to LFOSC upon detection of LFXTAL failure */
#define BITP_CLKG_OSC_CTL_LFX_AUTSW_EN       12            /*  Enables automatic Switching of the LF Mux to LFOSC on LFXTAL Failure */
#define BITP_CLKG_OSC_CTL_HFX_OK             11            /*  Status of HFXTAL oscillator */
#define BITP_CLKG_OSC_CTL_LFX_OK             10            /*  Status of LFXTAL oscillator */
#define BITP_CLKG_OSC_CTL_HFOSC_OK            9            /*  Status of HFOSC oscillator */
#define BITP_CLKG_OSC_CTL_LFOSC_OK            8            /*  Status of LFOSC oscillator */
#define BITP_CLKG_OSC_CTL_LFX_MON_EN          5            /*  LFXTAL clock monitor and Clock FAIL interrupt enable */
#define BITP_CLKG_OSC_CTL_LFX_BYP             4            /*  Low frequency crystal oscillator Bypass */
#define BITP_CLKG_OSC_CTL_HFX_EN              3            /*  High frequency crystal oscillator enable */
#define BITP_CLKG_OSC_CTL_LFX_EN              2            /*  Low frequency crystal oscillator enable */
#define BITP_CLKG_OSC_CTL_HFOSC_EN            1            /*  High frequency internal oscillator enable */
#define BITP_CLKG_OSC_CTL_LFCLK_MUX           0            /*  32 kHz clock select mux */
#define BITM_CLKG_OSC_CTL_LFX_FAIL_STA       (_ADI_MSK_3(0x80000000,0x80000000UL, uint32_t  ))    /*  LF XTAL (crystal clock) Not Stable */
#define BITM_CLKG_OSC_CTL_ROOT_FAIL_STA      (_ADI_MSK_3(0x40000000,0x40000000UL, uint32_t  ))    /*  Root clock (crystal clock) Not Stable */
#define BITM_CLKG_OSC_CTL_ROOT_AUTSW_STA     (_ADI_MSK_3(0x00400000,0x00400000UL, uint32_t  ))    /*  Status of automatic switching of the Root clock to HFOSC upon detection of Root clock failure */
#define BITM_CLKG_OSC_CTL_ROOT_AUTSW_EN      (_ADI_MSK_3(0x00200000,0x00200000UL, uint32_t  ))    /*  Enables automatic Switching of the Root clock  to HFOSC on Root clock Failure */
#define BITM_CLKG_OSC_CTL_ROOT_MON_EN        (_ADI_MSK_3(0x00100000,0x00100000UL, uint32_t  ))    /*  ROOT clock monitor and Clock FAIL interrupt enable */
#define BITM_CLKG_OSC_CTL_LFX_ROBUST_LD      (_ADI_MSK_3(0x00018000,0x00018000UL, uint32_t  ))    /*  LFXTAL Robust Mode Load select */
#define BITM_CLKG_OSC_CTL_LFX_ROBUST_EN      (_ADI_MSK_3(0x00004000,0x00004000UL, uint32_t  ))    /*  LFXTAL Mode select */
#define BITM_CLKG_OSC_CTL_LFX_AUTSW_STA      (_ADI_MSK_3(0x00002000,0x00002000UL, uint32_t  ))    /*  Status of automatic switching of the LF Mux to LFOSC upon detection of LFXTAL failure */
#define BITM_CLKG_OSC_CTL_LFX_AUTSW_EN       (_ADI_MSK_3(0x00001000,0x00001000UL, uint32_t  ))    /*  Enables automatic Switching of the LF Mux to LFOSC on LFXTAL Failure */
#define BITM_CLKG_OSC_CTL_HFX_OK             (_ADI_MSK_3(0x00000800,0x00000800UL, uint32_t  ))    /*  Status of HFXTAL oscillator */
#define BITM_CLKG_OSC_CTL_LFX_OK             (_ADI_MSK_3(0x00000400,0x00000400UL, uint32_t  ))    /*  Status of LFXTAL oscillator */
#define BITM_CLKG_OSC_CTL_HFOSC_OK           (_ADI_MSK_3(0x00000200,0x00000200UL, uint32_t  ))    /*  Status of HFOSC oscillator */
#define BITM_CLKG_OSC_CTL_LFOSC_OK           (_ADI_MSK_3(0x00000100,0x00000100UL, uint32_t  ))    /*  Status of LFOSC oscillator */
#define BITM_CLKG_OSC_CTL_LFX_MON_EN         (_ADI_MSK_3(0x00000020,0x00000020UL, uint32_t  ))    /*  LFXTAL clock monitor and Clock FAIL interrupt enable */
#define BITM_CLKG_OSC_CTL_LFX_BYP            (_ADI_MSK_3(0x00000010,0x00000010UL, uint32_t  ))    /*  Low frequency crystal oscillator Bypass */
#define BITM_CLKG_OSC_CTL_HFX_EN             (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  High frequency crystal oscillator enable */
#define BITM_CLKG_OSC_CTL_LFX_EN             (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  Low frequency crystal oscillator enable */
#define BITM_CLKG_OSC_CTL_HFOSC_EN           (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  High frequency internal oscillator enable */
#define BITM_CLKG_OSC_CTL_LFCLK_MUX          (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  32 kHz clock select mux */


/* ============================================================================================================================
        Power Management Registers
   ============================================================================================================================ */

/* ============================================================================================================================
        PMG0_TST
   ============================================================================================================================ */
#define REG_PMG0_TST_SRAM_CTL                0x4004C260            /*  PMG0_TST Control for SRAM Parity and Instruction SRAM */
#define REG_PMG0_TST_SRAM_INITSTAT           0x4004C264            /*  PMG0_TST Initialization Status Register */
#define REG_PMG0_TST_CLR_LATCH_GPIOS         0x4004C268            /*  PMG0_TST Clear GPIO After Shutdown Mode */
#define REG_PMG0_TST_SCRPAD_IMG              0x4004C26C            /*  PMG0_TST Scratch Pad Image */
#define REG_PMG0_TST_SCRPAD_3V_RD            0x4004C270            /*  PMG0_TST Scratch Pad Saved in Battery Domain */
#define REG_PMG0_TST_FAST_SHT_WAKEUP         0x4004C274            /*  PMG0_TST Fast Shutdown Wake-up Enable */

/* ============================================================================================================================
        PMG_TST Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          PMG_TST_SRAM_CTL                     Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_PMG_TST_SRAM_CTL_INSTREN        31            /*  Enables 32 KB instruction SRAM */
#define BITP_PMG_TST_SRAM_CTL_PENBNK7        23            /*  Enable parity check */
#define BITP_PMG_TST_SRAM_CTL_PENBNK6        22            /*  Enable parity check */
#define BITP_PMG_TST_SRAM_CTL_PENBNK5        21            /*  Enable parity check */
#define BITP_PMG_TST_SRAM_CTL_PENBNK4        20            /*  Enable parity check */
#define BITP_PMG_TST_SRAM_CTL_PENBNK3        19            /*  Enable parity check */
#define BITP_PMG_TST_SRAM_CTL_PENBNK2        18            /*  Enable parity check */
#define BITP_PMG_TST_SRAM_CTL_PENBNK1        17            /*  Enable parity check */
#define BITP_PMG_TST_SRAM_CTL_PENBNK0        16            /*  Enable parity check */
#define BITP_PMG_TST_SRAM_CTL_ABTINIT        15            /*  Abort current initialization. Self-cleared */
#define BITP_PMG_TST_SRAM_CTL_AUTOINIT       14            /*  Automatic initialization on wake up from hibernate mode */
#define BITP_PMG_TST_SRAM_CTL_STARTINIT      13            /*  Write one to trigger initialization. Self-cleared */
#define BITP_PMG_TST_SRAM_CTL_BNK7EN          7            /*  Enable initialization */
#define BITP_PMG_TST_SRAM_CTL_BNK2EN          2            /*  Enable initialization */
#define BITP_PMG_TST_SRAM_CTL_BNK1EN          1            /*  Enable initialization */
#define BITM_PMG_TST_SRAM_CTL_INSTREN        (_ADI_MSK_3(0x80000000,0x80000000UL, uint32_t  ))    /*  Enables 32 KB instruction SRAM */
#define BITM_PMG_TST_SRAM_CTL_PENBNK7        (_ADI_MSK_3(0x00800000,0x00800000UL, uint32_t  ))    /*  Enable parity check */
#define BITM_PMG_TST_SRAM_CTL_PENBNK6        (_ADI_MSK_3(0x00400000,0x00400000UL, uint32_t  ))    /*  Enable parity check */
#define BITM_PMG_TST_SRAM_CTL_PENBNK5        (_ADI_MSK_3(0x00200000,0x00200000UL, uint32_t  ))    /*  Enable parity check */
#define BITM_PMG_TST_SRAM_CTL_PENBNK4        (_ADI_MSK_3(0x00100000,0x00100000UL, uint32_t  ))    /*  Enable parity check */
#define BITM_PMG_TST_SRAM_CTL_PENBNK3        (_ADI_MSK_3(0x00080000,0x00080000UL, uint32_t  ))    /*  Enable parity check */
#define BITM_PMG_TST_SRAM_CTL_PENBNK2        (_ADI_MSK_3(0x00040000,0x00040000UL, uint32_t  ))    /*  Enable parity check */
#define BITM_PMG_TST_SRAM_CTL_PENBNK1        (_ADI_MSK_3(0x00020000,0x00020000UL, uint32_t  ))    /*  Enable parity check */
#define BITM_PMG_TST_SRAM_CTL_PENBNK0        (_ADI_MSK_3(0x00010000,0x00010000UL, uint32_t  ))    /*  Enable parity check */
#define BITM_PMG_TST_SRAM_CTL_ABTINIT        (_ADI_MSK_3(0x00008000,0x00008000UL, uint32_t  ))    /*  Abort current initialization. Self-cleared */
#define BITM_PMG_TST_SRAM_CTL_AUTOINIT       (_ADI_MSK_3(0x00004000,0x00004000UL, uint32_t  ))    /*  Automatic initialization on wake up from hibernate mode */
#define BITM_PMG_TST_SRAM_CTL_STARTINIT      (_ADI_MSK_3(0x00002000,0x00002000UL, uint32_t  ))    /*  Write one to trigger initialization. Self-cleared */
#define BITM_PMG_TST_SRAM_CTL_BNK7EN         (_ADI_MSK_3(0x00000080,0x00000080UL, uint32_t  ))    /*  Enable initialization */
#define BITM_PMG_TST_SRAM_CTL_BNK2EN         (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  Enable initialization */
#define BITM_PMG_TST_SRAM_CTL_BNK1EN         (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Enable initialization */

/* -------------------------------------------------------------------------------------------------------------------------
          PMG_TST_SRAM_INITSTAT                Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_PMG_TST_SRAM_INITSTAT_BNK7DONE   7            /*  Bank 7 initialization status */
#define BITP_PMG_TST_SRAM_INITSTAT_BNK6DONE   6            /*  Bank 6 initialization status */
#define BITP_PMG_TST_SRAM_INITSTAT_BNK5DONE   5            /*  Bank 5 initialization status */
#define BITP_PMG_TST_SRAM_INITSTAT_BNK4DONE   4            /*  Bank 4 initialization status */
#define BITP_PMG_TST_SRAM_INITSTAT_BNK3DONE   3            /*  Bank 3 initialization status */
#define BITP_PMG_TST_SRAM_INITSTAT_BNK2DONE   2            /*  Bank 2 initialization status */
#define BITP_PMG_TST_SRAM_INITSTAT_BNK1DONE   1            /*  Bank 1 initialization status */
#define BITP_PMG_TST_SRAM_INITSTAT_BNK0DONE   0            /*  Bank 0 initialization status */
#define BITM_PMG_TST_SRAM_INITSTAT_BNK7DONE  (_ADI_MSK_3(0x00000080,0x00000080UL, uint32_t  ))    /*  Bank 7 initialization status */
#define BITM_PMG_TST_SRAM_INITSTAT_BNK6DONE  (_ADI_MSK_3(0x00000040,0x00000040UL, uint32_t  ))    /*  Bank 6 initialization status */
#define BITM_PMG_TST_SRAM_INITSTAT_BNK5DONE  (_ADI_MSK_3(0x00000020,0x00000020UL, uint32_t  ))    /*  Bank 5 initialization status */
#define BITM_PMG_TST_SRAM_INITSTAT_BNK4DONE  (_ADI_MSK_3(0x00000010,0x00000010UL, uint32_t  ))    /*  Bank 4 initialization status */
#define BITM_PMG_TST_SRAM_INITSTAT_BNK3DONE  (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  Bank 3 initialization status */
#define BITM_PMG_TST_SRAM_INITSTAT_BNK2DONE  (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  Bank 2 initialization status */
#define BITM_PMG_TST_SRAM_INITSTAT_BNK1DONE  (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Bank 1 initialization status */
#define BITM_PMG_TST_SRAM_INITSTAT_BNK0DONE  (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Bank 0 initialization status */
#define ENUM_PMG_TST_SRAM_INITSTAT_NO_BANK7_INIT (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  BNK7DONE: Bank 7 not initialized */
#define ENUM_PMG_TST_SRAM_INITSTAT_BANK7_INIT (_ADI_MSK_3(0x00000080,0x00000080UL, uint32_t  ))    /*  BNK7DONE: Bank 7 initialized */
#define ENUM_PMG_TST_SRAM_INITSTAT_NO_BANK6_INIT (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  BNK6DONE: Bank 6 not initialized */
#define ENUM_PMG_TST_SRAM_INITSTAT_BANK6_INIT (_ADI_MSK_3(0x00000040,0x00000040UL, uint32_t  ))    /*  BNK6DONE: Bank 6 initialized */
#define ENUM_PMG_TST_SRAM_INITSTAT_NO_BANK5_INIT (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  BNK5DONE: Bank 5 not initialized */
#define ENUM_PMG_TST_SRAM_INITSTAT_BANK5_INIT (_ADI_MSK_3(0x00000020,0x00000020UL, uint32_t  ))    /*  BNK5DONE: Bank 5 initialized */
#define ENUM_PMG_TST_SRAM_INITSTAT_NO_BANK4_INIT (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  BNK4DONE: Bank 4 not initialized */
#define ENUM_PMG_TST_SRAM_INITSTAT_BANK4_INIT (_ADI_MSK_3(0x00000010,0x00000010UL, uint32_t  ))    /*  BNK4DONE: Bank 4 initialized */
#define ENUM_PMG_TST_SRAM_INITSTAT_NO_BANK3_INIT (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  BNK3DONE: Bank 3 not initialized */
#define ENUM_PMG_TST_SRAM_INITSTAT_BANK3_INIT (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  BNK3DONE: Bank 3 initialized */
#define ENUM_PMG_TST_SRAM_INITSTAT_NO_BANK2_INIT (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  BNK2DONE: Bank 2 not initialized */
#define ENUM_PMG_TST_SRAM_INITSTAT_BANK2_INIT (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  BNK2DONE: Bank 2 initialized */
#define ENUM_PMG_TST_SRAM_INITSTAT_NO_BANK1_INIT (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  BNK1DONE: Bank 1 not initialized */
#define ENUM_PMG_TST_SRAM_INITSTAT_BANK1_INIT (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  BNK1DONE: Bank 1 initialized */
#define ENUM_PMG_TST_SRAM_INITSTAT_NO_BANK0_INIT (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  BNK0DONE: Bank 0 not initialized */
#define ENUM_PMG_TST_SRAM_INITSTAT_BANK0_INIT (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  BNK0DONE: Bank 0 initialized */

/* -------------------------------------------------------------------------------------------------------------------------
          PMG_TST_CLR_LATCH_GPIOS              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_PMG_TST_CLR_LATCH_GPIOS_VALUE    0            /*  Writing 0x58FA creates a pulse to clear the latches for the GPIOs */
#define BITM_PMG_TST_CLR_LATCH_GPIOS_VALUE   (_ADI_MSK_3(0x0000FFFF,0x0000FFFFU, uint16_t  ))    /*  Writing 0x58FA creates a pulse to clear the latches for the GPIOs */

/* -------------------------------------------------------------------------------------------------------------------------
          PMG_TST_SCRPAD_IMG                   Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_PMG_TST_SCRPAD_IMG_DATA          0            /*  Value written to this register is saved in 3 V when going to shutdown */
#define BITM_PMG_TST_SCRPAD_IMG_DATA         (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Value written to this register is saved in 3 V when going to shutdown */

/* -------------------------------------------------------------------------------------------------------------------------
          PMG_TST_SCRPAD_3V_RD                 Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_PMG_TST_SCRPAD_3V_RD_DATA        0            /*  Reading the scratch pad stored in shutdown mode */
#define BITM_PMG_TST_SCRPAD_3V_RD_DATA       (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Reading the scratch pad stored in shutdown mode */

/* -------------------------------------------------------------------------------------------------------------------------
          PMG_TST_FAST_SHT_WAKEUP              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_PMG_TST_FAST_SHT_WAKEUP_FAST_SHT_WAKEUP  0            /*  Enables fast shutdown wake-up */
#define BITM_PMG_TST_FAST_SHT_WAKEUP_FAST_SHT_WAKEUP (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Enables fast shutdown wake-up */


/* ============================================================================================================================
        Clocking registers
   ============================================================================================================================ */

/* ============================================================================================================================
        CLKG0_CLK
   ============================================================================================================================ */
#define REG_CLKG0_CLK_CTL0                   0x4004C300            /*  CLKG0_CLK Misc Clock Settings */
#define REG_CLKG0_CLK_CTL1                   0x4004C304            /*  CLKG0_CLK Clock Dividers */
#define REG_CLKG0_CLK_CTL2                   0x4004C308            /*  CLKG0_CLK HF Oscillator Divided Clock Select */
#define REG_CLKG0_CLK_CTL3                   0x4004C30C            /*  CLKG0_CLK System PLL */
#define REG_CLKG0_CLK_CTL5                   0x4004C314            /*  CLKG0_CLK User Clock Gating Control */
#define REG_CLKG0_CLK_STAT0                  0x4004C318            /*  CLKG0_CLK Clocking Status */

/* ============================================================================================================================
        CLKG_CLK Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          CLKG_CLK_CTL0                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CLKG_CLK_CTL0_HFXTALIE          15            /*  High frequency crystal interrupt enable */
#define BITP_CLKG_CLK_CTL0_LFXTALIE          14            /*  Low frequency crystal interrupt enable */
#define BITP_CLKG_CLK_CTL0_PLL_IPSEL         11            /*  SPLL source select mux */
#define BITP_CLKG_CLK_CTL0_RCLKMUX            8            /*  Flash reference clock and HPBUCK clock source mux */
#define BITP_CLKG_CLK_CTL0_CLKOUT             3            /*  GPIO clock out select */
#define BITP_CLKG_CLK_CTL0_CLKMUX             0            /*  Clock mux select */
#define BITM_CLKG_CLK_CTL0_HFXTALIE          (_ADI_MSK_3(0x00008000,0x00008000UL, uint32_t  ))    /*  High frequency crystal interrupt enable */
#define BITM_CLKG_CLK_CTL0_LFXTALIE          (_ADI_MSK_3(0x00004000,0x00004000UL, uint32_t  ))    /*  Low frequency crystal interrupt enable */
#define BITM_CLKG_CLK_CTL0_PLL_IPSEL         (_ADI_MSK_3(0x00001800,0x00001800UL, uint32_t  ))    /*  SPLL source select mux */
#define BITM_CLKG_CLK_CTL0_RCLKMUX           (_ADI_MSK_3(0x00000300,0x00000300UL, uint32_t  ))    /*  Flash reference clock and HPBUCK clock source mux */
#define BITM_CLKG_CLK_CTL0_CLKOUT            (_ADI_MSK_3(0x00000078,0x00000078UL, uint32_t  ))    /*  GPIO clock out select */
#define BITM_CLKG_CLK_CTL0_CLKMUX            (_ADI_MSK_3(0x00000003,0x00000003UL, uint32_t  ))    /*  Clock mux select */

/* -------------------------------------------------------------------------------------------------------------------------
          CLKG_CLK_CTL1                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CLKG_CLK_CTL1_ACLKDIVCNT        16            /*  ACLK Divide Count */
#define BITP_CLKG_CLK_CTL1_PCLKDIVCNT         8            /*  PCLK divide count */
#define BITP_CLKG_CLK_CTL1_HCLKDIVCNT         0            /*  HCLK divide count */
#define BITM_CLKG_CLK_CTL1_ACLKDIVCNT        (_ADI_MSK_3(0x01FF0000,0x01FF0000UL, uint32_t  ))    /*  ACLK Divide Count */
#define BITM_CLKG_CLK_CTL1_PCLKDIVCNT        (_ADI_MSK_3(0x00003F00,0x00003F00UL, uint32_t  ))    /*  PCLK divide count */
#define BITM_CLKG_CLK_CTL1_HCLKDIVCNT        (_ADI_MSK_3(0x0000003F,0x0000003FUL, uint32_t  ))    /*  HCLK divide count */

/* -------------------------------------------------------------------------------------------------------------------------
          CLKG_CLK_CTL2                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CLKG_CLK_CTL2_HFOSCDIVCLKSEL     1            /*  HF Oscillator divided clock select */
#define BITP_CLKG_CLK_CTL2_HFOSCAUTODIV_EN    0            /*  HF Oscillator auto divide by one clock selection during wakeup from Flexi power mode */
#define BITM_CLKG_CLK_CTL2_HFOSCDIVCLKSEL    (_ADI_MSK_3(0x0000000E,0x0000000EUL, uint32_t  ))    /*  HF Oscillator divided clock select */
#define BITM_CLKG_CLK_CTL2_HFOSCAUTODIV_EN   (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  HF Oscillator auto divide by one clock selection during wakeup from Flexi power mode */

/* -------------------------------------------------------------------------------------------------------------------------
          CLKG_CLK_CTL3                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CLKG_CLK_CTL3_SPLLMUL2          16            /*  System PLL multiply by 2 */
#define BITP_CLKG_CLK_CTL3_SPLLMSEL          11            /*  System PLL M Divider */
#define BITP_CLKG_CLK_CTL3_SPLLIE            10            /*  System PLL interrupt enable */
#define BITP_CLKG_CLK_CTL3_SPLLEN             9            /*  System PLL enable */
#define BITP_CLKG_CLK_CTL3_SPLLDIV2           8            /*  System PLL division by 2 */
#define BITP_CLKG_CLK_CTL3_SPLLNSEL           0            /*  System PLL N multiplier */
#define BITM_CLKG_CLK_CTL3_SPLLMUL2          (_ADI_MSK_3(0x00010000,0x00010000UL, uint32_t  ))    /*  System PLL multiply by 2 */
#define BITM_CLKG_CLK_CTL3_SPLLMSEL          (_ADI_MSK_3(0x00007800,0x00007800UL, uint32_t  ))    /*  System PLL M Divider */
#define BITM_CLKG_CLK_CTL3_SPLLIE            (_ADI_MSK_3(0x00000400,0x00000400UL, uint32_t  ))    /*  System PLL interrupt enable */
#define BITM_CLKG_CLK_CTL3_SPLLEN            (_ADI_MSK_3(0x00000200,0x00000200UL, uint32_t  ))    /*  System PLL enable */
#define BITM_CLKG_CLK_CTL3_SPLLDIV2          (_ADI_MSK_3(0x00000100,0x00000100UL, uint32_t  ))    /*  System PLL division by 2 */
#define BITM_CLKG_CLK_CTL3_SPLLNSEL          (_ADI_MSK_3(0x0000001F,0x0000001FUL, uint32_t  ))    /*  System PLL N multiplier */

/* -------------------------------------------------------------------------------------------------------------------------
          CLKG_CLK_CTL5                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CLKG_CLK_CTL5_TMRRGBCLKOFF       6            /*  Timer RGB user control */
#define BITP_CLKG_CLK_CTL5_PERCLKOFF          5            /*  This bit is used to disable all clocks connected to all peripherals */
#define BITP_CLKG_CLK_CTL5_GPIOCLKOFF         4            /*  GPIO clock control */
#define BITP_CLKG_CLK_CTL5_UCLKI2COFF         3            /*  I2C clock user control */
#define BITP_CLKG_CLK_CTL5_GPTCLK2OFF         2            /*  GP Timer 2 user control */
#define BITP_CLKG_CLK_CTL5_GPTCLK1OFF         1            /*  GP Timer 1 user control */
#define BITP_CLKG_CLK_CTL5_GPTCLK0OFF         0            /*  GP Timer 0 user control */
#define BITM_CLKG_CLK_CTL5_TMRRGBCLKOFF      (_ADI_MSK_3(0x00000040,0x00000040UL, uint32_t  ))    /*  Timer RGB user control */
#define BITM_CLKG_CLK_CTL5_PERCLKOFF         (_ADI_MSK_3(0x00000020,0x00000020UL, uint32_t  ))    /*  This bit is used to disable all clocks connected to all peripherals */
#define BITM_CLKG_CLK_CTL5_GPIOCLKOFF        (_ADI_MSK_3(0x00000010,0x00000010UL, uint32_t  ))    /*  GPIO clock control */
#define BITM_CLKG_CLK_CTL5_UCLKI2COFF        (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  I2C clock user control */
#define BITM_CLKG_CLK_CTL5_GPTCLK2OFF        (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  GP Timer 2 user control */
#define BITM_CLKG_CLK_CTL5_GPTCLK1OFF        (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  GP Timer 1 user control */
#define BITM_CLKG_CLK_CTL5_GPTCLK0OFF        (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  GP Timer 0 user control */

/* -------------------------------------------------------------------------------------------------------------------------
          CLKG_CLK_STAT0                       Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_CLKG_CLK_STAT0_HFXTALNOK        14            /*  HF crystal not stable */
#define BITP_CLKG_CLK_STAT0_HFXTALOK         13            /*  HF crystal stable */
#define BITP_CLKG_CLK_STAT0_HFXTAL           12            /*  HF crystal status */
#define BITP_CLKG_CLK_STAT0_LFXTALNOK        10            /*  LF crystal not stable */
#define BITP_CLKG_CLK_STAT0_LFXTALOK          9            /*  LF crystal stable */
#define BITP_CLKG_CLK_STAT0_LFXTAL            8            /*  LF crystal status */
#define BITP_CLKG_CLK_STAT0_SPLLUNLK          2            /*  System PLL unlock */
#define BITP_CLKG_CLK_STAT0_SPLLLK            1            /*  System PLL lock */
#define BITP_CLKG_CLK_STAT0_SPLL              0            /*  System PLL status */
#define BITM_CLKG_CLK_STAT0_HFXTALNOK        (_ADI_MSK_3(0x00004000,0x00004000UL, uint32_t  ))    /*  HF crystal not stable */
#define BITM_CLKG_CLK_STAT0_HFXTALOK         (_ADI_MSK_3(0x00002000,0x00002000UL, uint32_t  ))    /*  HF crystal stable */
#define BITM_CLKG_CLK_STAT0_HFXTAL           (_ADI_MSK_3(0x00001000,0x00001000UL, uint32_t  ))    /*  HF crystal status */
#define BITM_CLKG_CLK_STAT0_LFXTALNOK        (_ADI_MSK_3(0x00000400,0x00000400UL, uint32_t  ))    /*  LF crystal not stable */
#define BITM_CLKG_CLK_STAT0_LFXTALOK         (_ADI_MSK_3(0x00000200,0x00000200UL, uint32_t  ))    /*  LF crystal stable */
#define BITM_CLKG_CLK_STAT0_LFXTAL           (_ADI_MSK_3(0x00000100,0x00000100UL, uint32_t  ))    /*  LF crystal status */
#define BITM_CLKG_CLK_STAT0_SPLLUNLK         (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  System PLL unlock */
#define BITM_CLKG_CLK_STAT0_SPLLLK           (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  System PLL lock */
#define BITM_CLKG_CLK_STAT0_SPLL             (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  System PLL status */


/* ============================================================================================================================
        Bus matrix
   ============================================================================================================================ */

/* ============================================================================================================================
        BUSM0
   ============================================================================================================================ */
#define REG_BUSM0_ARBIT0                     0x4004C800            /*  BUSM0 Arbitration Priority Configuration for FLASH and SRAM0 */
#define REG_BUSM0_ARBIT1                     0x4004C804            /*  BUSM0 Arbitration Priority Configuration for SRAM1 and SIP */
#define REG_BUSM0_ARBIT2                     0x4004C808            /*  BUSM0 Arbitration Priority Configuration for APB32 and APB16 */
#define REG_BUSM0_ARBIT3                     0x4004C80C            /*  BUSM0 Arbitration Priority Configuration for APB16 priority for core and for DMA1 */
#define REG_BUSM0_ARBIT4                     0x4004C814            /*  BUSM0 Arbitration Priority Configuration for SRAM1 and SIP */

/* ============================================================================================================================
        BUSM Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          BUSM_ARBIT0                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_BUSM_ARBIT0_SRAM0_DMA0          20            /*  SRAM0 priority for DMA0 */
#define BITP_BUSM_ARBIT0_SRAM0_SBUS          18            /*  SRAM0 priority for SBUS */
#define BITP_BUSM_ARBIT0_SRAM0_DCODE         16            /*  SRAM0 priority for Dcode */
#define BITP_BUSM_ARBIT0_FLSH_DMA0            4            /*  Flash priority for DMA0 */
#define BITP_BUSM_ARBIT0_FLSH_SBUS            2            /*  Flash priority for SBUS */
#define BITP_BUSM_ARBIT0_FLSH_DCODE           0            /*  Flash priority for DCODE */
#define BITM_BUSM_ARBIT0_SRAM0_DMA0          (_ADI_MSK_3(0x00300000,0x00300000UL, uint32_t  ))    /*  SRAM0 priority for DMA0 */
#define BITM_BUSM_ARBIT0_SRAM0_SBUS          (_ADI_MSK_3(0x000C0000,0x000C0000UL, uint32_t  ))    /*  SRAM0 priority for SBUS */
#define BITM_BUSM_ARBIT0_SRAM0_DCODE         (_ADI_MSK_3(0x00030000,0x00030000UL, uint32_t  ))    /*  SRAM0 priority for Dcode */
#define BITM_BUSM_ARBIT0_FLSH_DMA0           (_ADI_MSK_3(0x00000030,0x00000030UL, uint32_t  ))    /*  Flash priority for DMA0 */
#define BITM_BUSM_ARBIT0_FLSH_SBUS           (_ADI_MSK_3(0x0000000C,0x0000000CUL, uint32_t  ))    /*  Flash priority for SBUS */
#define BITM_BUSM_ARBIT0_FLSH_DCODE          (_ADI_MSK_3(0x00000003,0x00000003UL, uint32_t  ))    /*  Flash priority for DCODE */

/* -------------------------------------------------------------------------------------------------------------------------
          BUSM_ARBIT1                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_BUSM_ARBIT1_SIP_DMA0            20            /*  SIP priority for DMA0 */
#define BITP_BUSM_ARBIT1_SIP_SBUS            18            /*  SIP priority for SBUS */
#define BITP_BUSM_ARBIT1_SIP_DCODE           16            /*  SIP priority for DCODE */
#define BITP_BUSM_ARBIT1_SRAM1_DMA0           4            /*  SRAM1 priority for DMA0 */
#define BITP_BUSM_ARBIT1_SRAM1_SBUS           2            /*  SRAM1 priority for SBUS */
#define BITP_BUSM_ARBIT1_SRAM1_DCODE          0            /*  SRAM1 priority for Dcode */
#define BITM_BUSM_ARBIT1_SIP_DMA0            (_ADI_MSK_3(0x00300000,0x00300000UL, uint32_t  ))    /*  SIP priority for DMA0 */
#define BITM_BUSM_ARBIT1_SIP_SBUS            (_ADI_MSK_3(0x000C0000,0x000C0000UL, uint32_t  ))    /*  SIP priority for SBUS */
#define BITM_BUSM_ARBIT1_SIP_DCODE           (_ADI_MSK_3(0x00030000,0x00030000UL, uint32_t  ))    /*  SIP priority for DCODE */
#define BITM_BUSM_ARBIT1_SRAM1_DMA0          (_ADI_MSK_3(0x00000030,0x00000030UL, uint32_t  ))    /*  SRAM1 priority for DMA0 */
#define BITM_BUSM_ARBIT1_SRAM1_SBUS          (_ADI_MSK_3(0x0000000C,0x0000000CUL, uint32_t  ))    /*  SRAM1 priority for SBUS */
#define BITM_BUSM_ARBIT1_SRAM1_DCODE         (_ADI_MSK_3(0x00000003,0x00000003UL, uint32_t  ))    /*  SRAM1 priority for Dcode */

/* -------------------------------------------------------------------------------------------------------------------------
          BUSM_ARBIT2                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_BUSM_ARBIT2_APB16_DMA0          20            /*  APB16 priority for DMA0 */
#define BITP_BUSM_ARBIT2_APB16_SBUS          18            /*  APB16 priority for SBUS */
#define BITP_BUSM_ARBIT2_APB16_DCODE         16            /*  APB16 priority for DCODE */
#define BITP_BUSM_ARBIT2_APB32_DMA0           4            /*  APB32 priority for DMA0 */
#define BITP_BUSM_ARBIT2_APB32_SBUS           2            /*  APB32 priority for SBUS */
#define BITP_BUSM_ARBIT2_APB32_DCODE          0            /*  APB32 priority for DCODE */
#define BITM_BUSM_ARBIT2_APB16_DMA0          (_ADI_MSK_3(0x00300000,0x00300000UL, uint32_t  ))    /*  APB16 priority for DMA0 */
#define BITM_BUSM_ARBIT2_APB16_SBUS          (_ADI_MSK_3(0x000C0000,0x000C0000UL, uint32_t  ))    /*  APB16 priority for SBUS */
#define BITM_BUSM_ARBIT2_APB16_DCODE         (_ADI_MSK_3(0x00030000,0x00030000UL, uint32_t  ))    /*  APB16 priority for DCODE */
#define BITM_BUSM_ARBIT2_APB32_DMA0          (_ADI_MSK_3(0x00000030,0x00000030UL, uint32_t  ))    /*  APB32 priority for DMA0 */
#define BITM_BUSM_ARBIT2_APB32_SBUS          (_ADI_MSK_3(0x0000000C,0x0000000CUL, uint32_t  ))    /*  APB32 priority for SBUS */
#define BITM_BUSM_ARBIT2_APB32_DCODE         (_ADI_MSK_3(0x00000003,0x00000003UL, uint32_t  ))    /*  APB32 priority for DCODE */

/* -------------------------------------------------------------------------------------------------------------------------
          BUSM_ARBIT3                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_BUSM_ARBIT3_APB16_4DMA_DMA1     17            /*  APB16 for dma priority for DMA1 */
#define BITP_BUSM_ARBIT3_APB16_4DMA_CORE     16            /*  APB16 for dma priority for CORE */
#define BITP_BUSM_ARBIT3_APB16_DMA1           1            /*  APB16 priority for DMA1 */
#define BITP_BUSM_ARBIT3_APB16_CORE           0            /*  APB16 priority for CORE */
#define BITM_BUSM_ARBIT3_APB16_4DMA_DMA1     (_ADI_MSK_3(0x00020000,0x00020000UL, uint32_t  ))    /*  APB16 for dma priority for DMA1 */
#define BITM_BUSM_ARBIT3_APB16_4DMA_CORE     (_ADI_MSK_3(0x00010000,0x00010000UL, uint32_t  ))    /*  APB16 for dma priority for CORE */
#define BITM_BUSM_ARBIT3_APB16_DMA1          (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  APB16 priority for DMA1 */
#define BITM_BUSM_ARBIT3_APB16_CORE          (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  APB16 priority for CORE */

/* -------------------------------------------------------------------------------------------------------------------------
          BUSM_ARBIT4                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_BUSM_ARBIT4_SRAM2_DMA0           4            /*  SRAM2 priority for DMA0 */
#define BITP_BUSM_ARBIT4_SRAM2_SBUS           2            /*  SRAM2 priority for SBUS */
#define BITP_BUSM_ARBIT4_SRAM2_DCODE          0            /*  SRAM2 priority for Dcode */
#define BITM_BUSM_ARBIT4_SRAM2_DMA0          (_ADI_MSK_3(0x00000030,0x00000030UL, uint32_t  ))    /*  SRAM2 priority for DMA0 */
#define BITM_BUSM_ARBIT4_SRAM2_SBUS          (_ADI_MSK_3(0x0000000C,0x0000000CUL, uint32_t  ))    /*  SRAM2 priority for SBUS */
#define BITM_BUSM_ARBIT4_SRAM2_DCODE         (_ADI_MSK_3(0x00000003,0x00000003UL, uint32_t  ))    /*  SRAM2 priority for Dcode */


/* ============================================================================================================================
        Parallel Test Interface
   ============================================================================================================================ */

/* ============================================================================================================================
        PTI0
   ============================================================================================================================ */
#define REG_PTI0_RST_ISR_STARTADDR           0x4004CD00            /*  PTI0 Reset ISR Start Address */
#define REG_PTI0_RST_STACK_PTR               0x4004CD04            /*  PTI0 Reset Stack Pointer */
#define REG_PTI0_CTL                         0x4004CD08            /*  PTI0 Parallel Test Interface Control Register */

/* ============================================================================================================================
        PTI Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          PTI_RST_ISR_STARTADDR                Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_PTI_RST_ISR_STARTADDR_VALUE      0
#define BITM_PTI_RST_ISR_STARTADDR_VALUE     (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))

/* -------------------------------------------------------------------------------------------------------------------------
          PTI_RST_STACK_PTR                    Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_PTI_RST_STACK_PTR_VALUE          0
#define BITM_PTI_RST_STACK_PTR_VALUE         (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))

/* -------------------------------------------------------------------------------------------------------------------------
          PTI_CTL                              Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_PTI_CTL_EN                       0
#define BITM_PTI_CTL_EN                      (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))


/* ============================================================================================================================
        Cortex-M3 Interrupt Controller
   ============================================================================================================================ */

/* ============================================================================================================================
        NVIC0
   ============================================================================================================================ */
#define REG_NVIC0_INTNUM                     0xE000E004            /*  NVIC0 Interrupt Control Type */
#define REG_NVIC0_STKSTA                     0xE000E010            /*  NVIC0 Systick Control and Status */
#define REG_NVIC0_STKLD                      0xE000E014            /*  NVIC0 Systick Reload Value */
#define REG_NVIC0_STKVAL                     0xE000E018            /*  NVIC0 Systick Current Value */
#define REG_NVIC0_STKCAL                     0xE000E01C            /*  NVIC0 Systick Calibration Value */
#define REG_NVIC0_INTSETE0                   0xE000E100            /*  NVIC0 IRQ0..31 Set_Enable */
#define REG_NVIC0_INTSETE1                   0xE000E104            /*  NVIC0 IRQ32..63 Set_Enable */
#define REG_NVIC0_INTCLRE0                   0xE000E180            /*  NVIC0 IRQ0..31 Clear_Enable */
#define REG_NVIC0_INTCLRE1                   0xE000E184            /*  NVIC0 IRQ32..63 Clear_Enable */
#define REG_NVIC0_INTSETP0                   0xE000E200            /*  NVIC0 IRQ0..31 Set_Pending */
#define REG_NVIC0_INTSETP1                   0xE000E204            /*  NVIC0 IRQ32..63 Set_Pending */
#define REG_NVIC0_INTCLRP0                   0xE000E280            /*  NVIC0 IRQ0..31 Clear_Pending */
#define REG_NVIC0_INTCLRP1                   0xE000E284            /*  NVIC0 IRQ32..63 Clear_Pending */
#define REG_NVIC0_INTACT0                    0xE000E300            /*  NVIC0 IRQ0..31 Active Bit */
#define REG_NVIC0_INTACT1                    0xE000E304            /*  NVIC0 IRQ32..63 Active Bit */
#define REG_NVIC0_INTPRI0                    0xE000E400            /*  NVIC0 IRQ0..3 Priority */
#define REG_NVIC0_INTPRI1                    0xE000E404            /*  NVIC0 IRQ4..7 Priority */
#define REG_NVIC0_INTPRI2                    0xE000E408            /*  NVIC0 IRQ8..11 Priority */
#define REG_NVIC0_INTPRI3                    0xE000E40C            /*  NVIC0 IRQ12..15 Priority */
#define REG_NVIC0_INTPRI4                    0xE000E410            /*  NVIC0 IRQ16..19 Priority */
#define REG_NVIC0_INTPRI5                    0xE000E414            /*  NVIC0 IRQ20..23 Priority */
#define REG_NVIC0_INTPRI6                    0xE000E418            /*  NVIC0 IRQ24..27 Priority */
#define REG_NVIC0_INTPRI7                    0xE000E41C            /*  NVIC0 IRQ28..31 Priority */
#define REG_NVIC0_INTPRI8                    0xE000E420            /*  NVIC0 IRQ32..35 Priority */
#define REG_NVIC0_INTPRI9                    0xE000E424            /*  NVIC0 IRQ36..39 Priority */
#define REG_NVIC0_INTPRI10                   0xE000E428            /*  NVIC0 IRQ40..43 Priority */
#define REG_NVIC0_INTCPID                    0xE000ED00            /*  NVIC0 CPUID Base */
#define REG_NVIC0_INTSTA                     0xE000ED04            /*  NVIC0 Interrupt Control State */
#define REG_NVIC0_INTVEC                     0xE000ED08            /*  NVIC0 Vector Table Offset */
#define REG_NVIC0_INTAIRC                    0xE000ED0C            /*  NVIC0 Application Interrupt/Reset Control */
#define REG_NVIC0_INTCON0                    0xE000ED10            /*  NVIC0 System Control */
#define REG_NVIC0_INTCON1                    0xE000ED14            /*  NVIC0 Configuration Control */
#define REG_NVIC0_INTSHPRIO0                 0xE000ED18            /*  NVIC0 System Handlers 4-7 Priority */
#define REG_NVIC0_INTSHPRIO1                 0xE000ED1C            /*  NVIC0 System Handlers 8-11 Priority */
#define REG_NVIC0_INTSHPRIO3                 0xE000ED20            /*  NVIC0 System Handlers 12-15 Priority */
#define REG_NVIC0_INTSHCSR                   0xE000ED24            /*  NVIC0 System Handler Control and State */
#define REG_NVIC0_INTCFSR                    0xE000ED28            /*  NVIC0 Configurable Fault Status */
#define REG_NVIC0_INTHFSR                    0xE000ED2C            /*  NVIC0 Hard Fault Status */
#define REG_NVIC0_INTDFSR                    0xE000ED30            /*  NVIC0 Debug Fault Status */
#define REG_NVIC0_INTMMAR                    0xE000ED34            /*  NVIC0 Mem Manage Address */
#define REG_NVIC0_INTBFAR                    0xE000ED38            /*  NVIC0 Bus Fault Address */
#define REG_NVIC0_INTAFSR                    0xE000ED3C            /*  NVIC0 Auxiliary Fault Status */
#define REG_NVIC0_INTPFR0                    0xE000ED40            /*  NVIC0 Processor Feature Register 0 */
#define REG_NVIC0_INTPFR1                    0xE000ED44            /*  NVIC0 Processor Feature Register 1 */
#define REG_NVIC0_INTDFR0                    0xE000ED48            /*  NVIC0 Debug Feature Register 0 */
#define REG_NVIC0_INTAFR0                    0xE000ED4C            /*  NVIC0 Auxiliary Feature Register 0 */
#define REG_NVIC0_INTMMFR0                   0xE000ED50            /*  NVIC0 Memory Model Feature Register 0 */
#define REG_NVIC0_INTMMFR1                   0xE000ED54            /*  NVIC0 Memory Model Feature Register 1 */
#define REG_NVIC0_INTMMFR2                   0xE000ED58            /*  NVIC0 Memory Model Feature Register 2 */
#define REG_NVIC0_INTMMFR3                   0xE000ED5C            /*  NVIC0 Memory Model Feature Register 3 */
#define REG_NVIC0_INTISAR0                   0xE000ED60            /*  NVIC0 ISA Feature Register 0 */
#define REG_NVIC0_INTISAR1                   0xE000ED64            /*  NVIC0 ISA Feature Register 1 */
#define REG_NVIC0_INTISAR2                   0xE000ED68            /*  NVIC0 ISA Feature Register 2 */
#define REG_NVIC0_INTISAR3                   0xE000ED6C            /*  NVIC0 ISA Feature Register 3 */
#define REG_NVIC0_INTISAR4                   0xE000ED70            /*  NVIC0 ISA Feature Register 4 */
#define REG_NVIC0_INTTRGI                    0xE000EF00            /*  NVIC0 Software Trigger Interrupt Register */
#define REG_NVIC0_INTPID4                    0xE000EFD0            /*  NVIC0 Peripheral Identification Register 4 */
#define REG_NVIC0_INTPID5                    0xE000EFD4            /*  NVIC0 Peripheral Identification Register 5 */
#define REG_NVIC0_INTPID6                    0xE000EFD8            /*  NVIC0 Peripheral Identification Register 6 */
#define REG_NVIC0_INTPID7                    0xE000EFDC            /*  NVIC0 Peripheral Identification Register 7 */
#define REG_NVIC0_INTPID0                    0xE000EFE0            /*  NVIC0 Peripheral Identification Bits7:0 */
#define REG_NVIC0_INTPID1                    0xE000EFE4            /*  NVIC0 Peripheral Identification Bits15:8 */
#define REG_NVIC0_INTPID2                    0xE000EFE8            /*  NVIC0 Peripheral Identification Bits16:23 */
#define REG_NVIC0_INTPID3                    0xE000EFEC            /*  NVIC0 Peripheral Identification Bits24:31 */
#define REG_NVIC0_INTCID0                    0xE000EFF0            /*  NVIC0 Component Identification Bits7:0 */
#define REG_NVIC0_INTCID1                    0xE000EFF4            /*  NVIC0 Component Identification Bits15:8 */
#define REG_NVIC0_INTCID2                    0xE000EFF8            /*  NVIC0 Component Identification Bits16:23 */
#define REG_NVIC0_INTCID3                    0xE000EFFC            /*  NVIC0 Component Identification Bits24:31 */

/* ============================================================================================================================
        NVIC Register BitMasks, Positions & Enumerations 
   ============================================================================================================================ */
/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTNUM                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTNUM_VALUE                0            /*  Interrupt Control Type */
#define BITM_NVIC_INTNUM_VALUE               (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Interrupt Control Type */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_STKSTA                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_STKSTA_VALUE                0            /*  Systick Control and Status */
#define BITM_NVIC_STKSTA_VALUE               (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Systick Control and Status */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_STKLD                           Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_STKLD_VALUE                 0            /*  Systick Reload Value */
#define BITM_NVIC_STKLD_VALUE                (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Systick Reload Value */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_STKVAL                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_STKVAL_VALUE                0            /*  Systick Current Value */
#define BITM_NVIC_STKVAL_VALUE               (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Systick Current Value */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_STKCAL                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_STKCAL_VALUE                0            /*  Systick Calibration Value */
#define BITM_NVIC_STKCAL_VALUE               (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Systick Calibration Value */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTSETE0                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTSETE0_VALUE              0            /*  IRQ0..31 Set_Enable */
#define BITM_NVIC_INTSETE0_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  IRQ0..31 Set_Enable */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTSETE1                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTSETE1_VALUE              0            /*  IRQ32..63 Set_Enable */
#define BITM_NVIC_INTSETE1_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  IRQ32..63 Set_Enable */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTCLRE0                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTCLRE0_VALUE              0            /*  IRQ0..31 Clear_Enable */
#define BITM_NVIC_INTCLRE0_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  IRQ0..31 Clear_Enable */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTCLRE1                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTCLRE1_VALUE              0            /*  IRQ32..63 Clear_Enable */
#define BITM_NVIC_INTCLRE1_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  IRQ32..63 Clear_Enable */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTSETP0                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTSETP0_VALUE              0            /*  IRQ0..31 Set_Pending */
#define BITM_NVIC_INTSETP0_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  IRQ0..31 Set_Pending */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTSETP1                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTSETP1_VALUE              0            /*  IRQ32..63 Set_Pending */
#define BITM_NVIC_INTSETP1_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  IRQ32..63 Set_Pending */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTCLRP0                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTCLRP0_VALUE              0            /*  IRQ0..31 Clear_Pending */
#define BITM_NVIC_INTCLRP0_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  IRQ0..31 Clear_Pending */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTCLRP1                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTCLRP1_VALUE              0            /*  IRQ32..63 Clear_Pending */
#define BITM_NVIC_INTCLRP1_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  IRQ32..63 Clear_Pending */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTACT0                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTACT0_VALUE               0            /*  IRQ0..31 Active Bit */
#define BITM_NVIC_INTACT0_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  IRQ0..31 Active Bit */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTACT1                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTACT1_VALUE               0            /*  IRQ32..63 Active Bit */
#define BITM_NVIC_INTACT1_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  IRQ32..63 Active Bit */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTPRI0                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTPRI0_VALUE               0            /*  IRQ0..3 Priority */
#define BITM_NVIC_INTPRI0_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  IRQ0..3 Priority */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTPRI1                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTPRI1_VALUE               0            /*  IRQ4..7 Priority */
#define BITM_NVIC_INTPRI1_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  IRQ4..7 Priority */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTPRI2                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTPRI2_VALUE               0            /*  IRQ8..11 Priority */
#define BITM_NVIC_INTPRI2_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  IRQ8..11 Priority */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTPRI3                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTPRI3_VALUE               0            /*  IRQ12..15 Priority */
#define BITM_NVIC_INTPRI3_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  IRQ12..15 Priority */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTPRI4                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTPRI4_VALUE               0            /*  IRQ16..19 Priority */
#define BITM_NVIC_INTPRI4_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  IRQ16..19 Priority */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTPRI5                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTPRI5_VALUE               0            /*  IRQ20..23 Priority */
#define BITM_NVIC_INTPRI5_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  IRQ20..23 Priority */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTPRI6                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTPRI6_VALUE               0            /*  IRQ24..27 Priority */
#define BITM_NVIC_INTPRI6_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  IRQ24..27 Priority */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTPRI7                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTPRI7_VALUE               0            /*  IRQ28..31 Priority */
#define BITM_NVIC_INTPRI7_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  IRQ28..31 Priority */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTPRI8                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTPRI8_VALUE               0            /*  IRQ32..35 Priority */
#define BITM_NVIC_INTPRI8_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  IRQ32..35 Priority */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTPRI9                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTPRI9_VALUE               0            /*  IRQ36..39 Priority */
#define BITM_NVIC_INTPRI9_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  IRQ36..39 Priority */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTPRI10                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTPRI10_VALUE              0            /*  IRQ40..43 Priority */
#define BITM_NVIC_INTPRI10_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  IRQ40..43 Priority */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTCPID                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTCPID_VALUE               0            /*  CPUID Base */
#define BITM_NVIC_INTCPID_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  CPUID Base */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTSTA                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTSTA_VALUE                0            /*  Interrupt Control State */
#define BITM_NVIC_INTSTA_VALUE               (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Interrupt Control State */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTVEC                          Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTVEC_VALUE                0            /*  Vector Table Offset */
#define BITM_NVIC_INTVEC_VALUE               (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Vector Table Offset */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTAIRC                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTAIRC_VALUE               0            /*  Application Interrupt/Reset Control */
#define BITM_NVIC_INTAIRC_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Application Interrupt/Reset Control */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTCON0                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTCON0_SLEEPDEEP           2            /*  deep sleep flag for HIBERNATE mode */
#define BITP_NVIC_INTCON0_SLEEPONEXIT         1            /*  Sleeps the core on exit from an ISR */
#define BITM_NVIC_INTCON0_SLEEPDEEP          (_ADI_MSK_3(0x00000004,0x00000004U, uint16_t  ))    /*  deep sleep flag for HIBERNATE mode */
#define BITM_NVIC_INTCON0_SLEEPONEXIT        (_ADI_MSK_3(0x00000002,0x00000002U, uint16_t  ))    /*  Sleeps the core on exit from an ISR */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTCON1                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTCON1_VALUE               0            /*  Configuration Control */
#define BITM_NVIC_INTCON1_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Configuration Control */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTSHPRIO0                      Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTSHPRIO0_VALUE            0            /*  System Handlers 4-7 Priority */
#define BITM_NVIC_INTSHPRIO0_VALUE           (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  System Handlers 4-7 Priority */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTSHPRIO1                      Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTSHPRIO1_VALUE            0            /*  System Handlers 8-11 Priority */
#define BITM_NVIC_INTSHPRIO1_VALUE           (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  System Handlers 8-11 Priority */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTSHPRIO3                      Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTSHPRIO3_VALUE            0            /*  System Handlers 12-15 Priority */
#define BITM_NVIC_INTSHPRIO3_VALUE           (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  System Handlers 12-15 Priority */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTSHCSR                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTSHCSR_VALUE              0            /*  System Handler Control and State */
#define BITM_NVIC_INTSHCSR_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  System Handler Control and State */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTCFSR                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTCFSR_VALUE               0            /*  Configurable Fault Status */
#define BITM_NVIC_INTCFSR_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Configurable Fault Status */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTHFSR                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTHFSR_VALUE               0            /*  Hard Fault Status */
#define BITM_NVIC_INTHFSR_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Hard Fault Status */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTDFSR                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTDFSR_VALUE               0            /*  Debug Fault Status */
#define BITM_NVIC_INTDFSR_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Debug Fault Status */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTMMAR                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTMMAR_VALUE               0            /*  Mem Manage Address */
#define BITM_NVIC_INTMMAR_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Mem Manage Address */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTBFAR                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTBFAR_VALUE               0            /*  Bus Fault Address */
#define BITM_NVIC_INTBFAR_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Bus Fault Address */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTAFSR                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTAFSR_VALUE               0            /*  Auxiliary Fault Status */
#define BITM_NVIC_INTAFSR_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Auxiliary Fault Status */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTPFR0                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTPFR0_VALUE               0            /*  Processor Feature Register 0 */
#define BITM_NVIC_INTPFR0_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Processor Feature Register 0 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTPFR1                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTPFR1_VALUE               0            /*  Processor Feature Register 1 */
#define BITM_NVIC_INTPFR1_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Processor Feature Register 1 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTDFR0                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTDFR0_VALUE               0            /*  Debug Feature Register 0 */
#define BITM_NVIC_INTDFR0_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Debug Feature Register 0 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTAFR0                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTAFR0_VALUE               0            /*  Auxiliary Feature Register 0 */
#define BITM_NVIC_INTAFR0_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Auxiliary Feature Register 0 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTMMFR0                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTMMFR0_VALUE              0            /*  Memory Model Feature Register 0 */
#define BITM_NVIC_INTMMFR0_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Memory Model Feature Register 0 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTMMFR1                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTMMFR1_VALUE              0            /*  Memory Model Feature Register 1 */
#define BITM_NVIC_INTMMFR1_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Memory Model Feature Register 1 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTMMFR2                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTMMFR2_VALUE              0            /*  Memory Model Feature Register 2 */
#define BITM_NVIC_INTMMFR2_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Memory Model Feature Register 2 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTMMFR3                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTMMFR3_VALUE              0            /*  Memory Model Feature Register 3 */
#define BITM_NVIC_INTMMFR3_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Memory Model Feature Register 3 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTISAR0                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTISAR0_VALUE              0            /*  ISA Feature Register 0 */
#define BITM_NVIC_INTISAR0_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  ISA Feature Register 0 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTISAR1                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTISAR1_VALUE              0            /*  ISA Feature Register 1 */
#define BITM_NVIC_INTISAR1_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  ISA Feature Register 1 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTISAR2                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTISAR2_VALUE              0            /*  ISA Feature Register 2 */
#define BITM_NVIC_INTISAR2_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  ISA Feature Register 2 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTISAR3                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTISAR3_VALUE              0            /*  ISA Feature Register 3 */
#define BITM_NVIC_INTISAR3_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  ISA Feature Register 3 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTISAR4                        Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTISAR4_VALUE              0            /*  ISA Feature Register 4 */
#define BITM_NVIC_INTISAR4_VALUE             (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  ISA Feature Register 4 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTTRGI                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTTRGI_VALUE               0            /*  Software Trigger Interrupt Register */
#define BITM_NVIC_INTTRGI_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Software Trigger Interrupt Register */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTPID4                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTPID4_VALUE               0            /*  Peripheral Identification Register 4 */
#define BITM_NVIC_INTPID4_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Peripheral Identification Register 4 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTPID5                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTPID5_VALUE               0            /*  Peripheral Identification Register 5 */
#define BITM_NVIC_INTPID5_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Peripheral Identification Register 5 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTPID6                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTPID6_VALUE               0            /*  Peripheral Identification Register 6 */
#define BITM_NVIC_INTPID6_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Peripheral Identification Register 6 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTPID7                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTPID7_VALUE               0            /*  Peripheral Identification Register 7 */
#define BITM_NVIC_INTPID7_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Peripheral Identification Register 7 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTPID0                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTPID0_VALUE               0            /*  Peripheral Identification Bits7:0 */
#define BITM_NVIC_INTPID0_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Peripheral Identification Bits7:0 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTPID1                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTPID1_VALUE               0            /*  Peripheral Identification Bits15:8 */
#define BITM_NVIC_INTPID1_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Peripheral Identification Bits15:8 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTPID2                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTPID2_VALUE               0            /*  Peripheral Identification Bits16:23 */
#define BITM_NVIC_INTPID2_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Peripheral Identification Bits16:23 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTPID3                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTPID3_VALUE               0            /*  Peripheral Identification Bits24:31 */
#define BITM_NVIC_INTPID3_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Peripheral Identification Bits24:31 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTCID0                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTCID0_VALUE               0            /*  Component Identification Bits7:0 */
#define BITM_NVIC_INTCID0_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Component Identification Bits7:0 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTCID1                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTCID1_VALUE               0            /*  Component Identification Bits15:8 */
#define BITM_NVIC_INTCID1_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Component Identification Bits15:8 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTCID2                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTCID2_VALUE               0            /*  Component Identification Bits16:23 */
#define BITM_NVIC_INTCID2_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Component Identification Bits16:23 */

/* -------------------------------------------------------------------------------------------------------------------------
          NVIC_INTCID3                         Pos/Masks         Description
   ------------------------------------------------------------------------------------------------------------------------- */
#define BITP_NVIC_INTCID3_VALUE               0            /*  Component Identification Bits24:31 */
#define BITM_NVIC_INTCID3_VALUE              (_ADI_MSK_3(0xFFFFFFFF,0xFFFFFFFFUL, uint32_t  ))    /*  Component Identification Bits24:31 */

/* ====================================================================================================
 *    Interrupt Definitions
 * ==================================================================================================== */
#define INTR_RESET                           (-15)		/* Cortex-M4 Reset */
#define INTR_NonMaskableInt                  (-14)		/* Cortex-M4 Non-maskable Interrupt */
#define INTR_HardFault                       (-13)		/* Cortex-M4 Hardware Fault */
#define INTR_MemoryManagement                (-12)		/* Cortex-M4 Memory Management Interrupt */
#define INTR_BusFault                        (-11)		/* Cortex-M4 Bus Fault */
#define INTR_UsageFault                      (-10)		/* Cortex-M4 Usage Fault */
#define INTR_SVCall                          ( -5)		/* Cortex-M4 SVCall Interrupt */
#define INTR_DebugMonitor                    ( -4)		/* Cortex-M4 Debug Monitor */
#define INTR_PendSV                          ( -2)		/* Cortex-M4 PendSV Interrupt */
#define INTR_SysTick                         ( -1)		/* Cortex-M4 SysTick Interrupt */
#define INTR_RTC1_EVT                          0		/* Event */
#define INTR_XINT_EVT0                         1		/* External Wakeup Interrupt n */
#define INTR_XINT_EVT1                         2		/* External Wakeup Interrupt n */
#define INTR_XINT_EVT2                         3		/* External Wakeup Interrupt n */
#define INTR_XINT_EVT3                         4		/* External Wakeup Interrupt n */
#define INTR_WDT_EXP                           5		/* Expiration */
#define INTR_PMG0_VREG_OVR                     6		/* Voltage Regulator (VREG) Overvoltage */
#define INTR_PMG0_BATT_RANGE                   7		/* Battery Voltage (VBAT) Out of Range */
#define INTR_RTC0_EVT                          8		/* Event */
#define INTR_SYS_GPIO_INTA                     9		/* GPIO Interrupt A */
#define INTR_SYS_GPIO_INTB                    10		/* GPIO Interrupt B */
#define INTR_TMR0_EVT                         11		/* Event */
#define INTR_TMR1_EVT                         12		/* Event */
#define INTR_FLCC_EVT                         13		/* Event */
#define INTR_UART0_EVT                        14		/* UART0 Event */
#define INTR_SPI0_EVT                         15		/* Event */
#define INTR_SPI2_EVT                         16		/* Event */
#define INTR_I2C_SLV_EVT                      17		/* Slave Event */
#define INTR_I2C_MST_EVT                      18		/* Master Event */
#define INTR_DMA_CHAN_ERR                     19		/* Channel Error */
#define INTR_DMA0_CH0_DONE                    20		/* Channel 0 Done */
#define INTR_DMA0_CH1_DONE                    21		/* Channel 1 Done */
#define INTR_DMA0_CH2_DONE                    22		/* Channel 2 Done */
#define INTR_DMA0_CH3_DONE                    23		/* Channel 3 Done */
#define INTR_DMA0_CH4_DONE                    24		/* Channel 4 Done */
#define INTR_DMA0_CH5_DONE                    25		/* Channel 5 Done */
#define INTR_DMA0_CH6_DONE                    26		/* Channel 6 Done */
#define INTR_DMA0_CH7_DONE                    27		/* Channel 7 Done */
#define INTR_DMA0_CH8_DONE                    28		/* Channel 8 Done */
#define INTR_DMA0_CH9_DONE                    29		/* Channel 9 Done */
#define INTR_DMA0_CH10_DONE                   30		/* Channel 10 Done */
#define INTR_DMA0_CH11_DONE                   31		/* Channel 11 Done */
#define INTR_DMA0_CH12_DONE                   32		/* Channel 12 Done */
#define INTR_DMA0_CH13_DONE                   33		/* Channel 13 Done */
#define INTR_DMA0_CH14_DONE                   34		/* Channel 14 Done */
#define INTR_DMA0_CH15_DONE                   35		/* Channel 15 Done */
#define INTR_SPORT_A_EVT                      36		/* Channel A Event */
#define INTR_SPORT_B_EVT                      37		/* Channel B Event */
#define INTR_CRYPT_EVT                        38		/* Event */
#define INTR_DMA0_CH24_DONE                   39		/* Channel 24 Done */
#define INTR_TMR2_EVT                         40		/* Event */
#define INTR_CLKG_XTAL_OSC_EVT                41		/* Crystal Oscillator Event */
#define INTR_SPI1_EVT                         42		/* Event */
#define INTR_CLKG_PLL_EVT                     43		/* PLL Event */
#define INTR_RNG0_EVT                         44		/* Event */
#define INTR_BEEP_EVT                         45		/* Event */
#define INTR_ADC0_EVT                         46		/* Event */
#define INTR_DMA0_CH16_DONE                   56		/* Channel 16 Done */
#define INTR_DMA0_CH17_DONE                   57		/* Channel 17 Done */
#define INTR_DMA0_CH18_DONE                   58		/* Channel 18 Done */
#define INTR_DMA0_CH19_DONE                   59		/* Channel 19 Done */
#define INTR_DMA0_CH20_DONE                   60		/* Channel 20 Done */
#define INTR_DMA0_CH21_DONE                   61		/* Channel 21 Done */
#define INTR_DMA0_CH22_DONE                   62		/* Channel 22 Done */
#define INTR_DMA0_CH23_DONE                   63		/* Channel 23 Done */
#define INTR_UART1_EVT                        66		/* Event */
#define INTR_DMA0_CH25_DONE                   67		/* Channel 25 Done */
#define INTR_DMA0_CH26_DONE                   68		/* Channel 26 Done */
#define INTR_TMR_RGB_EVT                      69		/* Event */
#define INTR_CLKG_ROOTCLK_ERR                 71		/* Root Clock Error */


#if defined (_MISRA_RULES)
#pragma diag(pop)
#endif /* _MISRA_RULES */

#endif	/* end ifndef _DEF_ADUCM4050_H */
