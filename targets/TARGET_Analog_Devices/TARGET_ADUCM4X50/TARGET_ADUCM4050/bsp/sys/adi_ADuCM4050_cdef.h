/* ================================================================================

     Project      :   ADuCM4050
     File         :   ADuCM4050_cdef.h
     Description  :   C MMR Pointer Definitions

     Date         :   Feb 7, 2017

     Copyright (c) 2014-2017 Analog Devices, Inc.  All Rights Reserved.
     This software is proprietary and confidential to Analog Devices, Inc. and
     its licensors.

     This file was auto-generated. Do not make local changes to this file.

   ================================================================================ */

#ifndef _ADUCM4050_CDEF_H
#define _ADUCM4050_CDEF_H

#if defined(_LANGUAGE_C) || (defined(__GNUC__) && !defined(__ASSEMBLER__))
#include <stdint.h>
#endif /* _LANGUAGE_C */

/* pickup register bitfield and bit masks */
#include "adi_ADuCM4050.h"


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
#pragma diag(push)
#pragma diag(suppress:misra_rule_5_1:"Allow names over 32 character limit")
#pragma diag(suppress:misra_rule_19_7:"ADI header allows function-like macros")
#pragma diag(suppress:misra_rule_19_13:"ADI headers can use the # and ## preprocessor operators")
#endif /* _MISRA_RULES */


/*  =================================================================================
 *      General Purpose Timer (TMR0)
 *  ================================================================================= */
#define pREG_TMR0_LOAD                   ((__IO     uint16_t  *) REG_TMR0_LOAD)                   /*  16-bit Load Value */
#define pREG_TMR0_CURCNT                 ((__I __C  uint16_t  *) REG_TMR0_CURCNT)                 /*  16-bit Timer Value */
#define pREG_TMR0_CTL                    ((__IO     uint16_t  *) REG_TMR0_CTL)                    /*  Control */
#define pREG_TMR0_CLRINT                 ((__O      uint16_t  *) REG_TMR0_CLRINT)                 /*  Clear Interrupt */
#define pREG_TMR0_CAPTURE                ((__I __C  uint16_t  *) REG_TMR0_CAPTURE)                /*  Capture */
#define pREG_TMR0_ALOAD                  ((__IO     uint16_t  *) REG_TMR0_ALOAD)                  /*  16-bit Load Value, Asynchronous */
#define pREG_TMR0_ACURCNT                ((__I __C  uint16_t  *) REG_TMR0_ACURCNT)                /*  16-bit Timer Value, Asynchronous */
#define pREG_TMR0_STAT                   ((__I __C  uint16_t  *) REG_TMR0_STAT)                   /*  Status */
#define pREG_TMR0_PWMCTL                 ((__IO     uint16_t  *) REG_TMR0_PWMCTL)                 /*  PWM Control Register */
#define pREG_TMR0_PWMMATCH               ((__IO     uint16_t  *) REG_TMR0_PWMMATCH)               /*  PWM Match Value */
#define pREG_TMR0_EVENTSELECT            ((__IO     uint16_t  *) REG_TMR0_EVENTSELECT)            /*  Timer Event Selection Register */

/*  =================================================================================
 *      General Purpose Timer (TMR1)
 *  ================================================================================= */
#define pREG_TMR1_LOAD                   ((__IO     uint16_t  *) REG_TMR1_LOAD)                   /*  16-bit Load Value */
#define pREG_TMR1_CURCNT                 ((__I __C  uint16_t  *) REG_TMR1_CURCNT)                 /*  16-bit Timer Value */
#define pREG_TMR1_CTL                    ((__IO     uint16_t  *) REG_TMR1_CTL)                    /*  Control */
#define pREG_TMR1_CLRINT                 ((__O      uint16_t  *) REG_TMR1_CLRINT)                 /*  Clear Interrupt */
#define pREG_TMR1_CAPTURE                ((__I __C  uint16_t  *) REG_TMR1_CAPTURE)                /*  Capture */
#define pREG_TMR1_ALOAD                  ((__IO     uint16_t  *) REG_TMR1_ALOAD)                  /*  16-bit Load Value, Asynchronous */
#define pREG_TMR1_ACURCNT                ((__I __C  uint16_t  *) REG_TMR1_ACURCNT)                /*  16-bit Timer Value, Asynchronous */
#define pREG_TMR1_STAT                   ((__I __C  uint16_t  *) REG_TMR1_STAT)                   /*  Status */
#define pREG_TMR1_PWMCTL                 ((__IO     uint16_t  *) REG_TMR1_PWMCTL)                 /*  PWM Control Register */
#define pREG_TMR1_PWMMATCH               ((__IO     uint16_t  *) REG_TMR1_PWMMATCH)               /*  PWM Match Value */
#define pREG_TMR1_EVENTSELECT            ((__IO     uint16_t  *) REG_TMR1_EVENTSELECT)            /*  Timer Event Selection Register */

/*  =================================================================================
 *      General Purpose Timer (TMR2)
 *  ================================================================================= */
#define pREG_TMR2_LOAD                   ((__IO     uint16_t  *) REG_TMR2_LOAD)                   /*  16-bit Load Value */
#define pREG_TMR2_CURCNT                 ((__I __C  uint16_t  *) REG_TMR2_CURCNT)                 /*  16-bit Timer Value */
#define pREG_TMR2_CTL                    ((__IO     uint16_t  *) REG_TMR2_CTL)                    /*  Control */
#define pREG_TMR2_CLRINT                 ((__O      uint16_t  *) REG_TMR2_CLRINT)                 /*  Clear Interrupt */
#define pREG_TMR2_CAPTURE                ((__I __C  uint16_t  *) REG_TMR2_CAPTURE)                /*  Capture */
#define pREG_TMR2_ALOAD                  ((__IO     uint16_t  *) REG_TMR2_ALOAD)                  /*  16-bit Load Value, Asynchronous */
#define pREG_TMR2_ACURCNT                ((__I __C  uint16_t  *) REG_TMR2_ACURCNT)                /*  16-bit Timer Value, Asynchronous */
#define pREG_TMR2_STAT                   ((__I __C  uint16_t  *) REG_TMR2_STAT)                   /*  Status */
#define pREG_TMR2_PWMCTL                 ((__IO     uint16_t  *) REG_TMR2_PWMCTL)                 /*  PWM Control Register */
#define pREG_TMR2_PWMMATCH               ((__IO     uint16_t  *) REG_TMR2_PWMMATCH)               /*  PWM Match Value */
#define pREG_TMR2_EVENTSELECT            ((__IO     uint16_t  *) REG_TMR2_EVENTSELECT)            /*  Timer Event Selection Register */

/*  =================================================================================
 *      Timer_RGB with 3 PWM outputs (TMR_RGB)
 *  ================================================================================= */
#define pREG_TMR_RGB_LOAD                ((__IO     uint16_t  *) REG_TMR_RGB_LOAD)                /*  16-bit load value */
#define pREG_TMR_RGB_CURCNT              ((__I __C  uint16_t  *) REG_TMR_RGB_CURCNT)              /*  16-bit timer value */
#define pREG_TMR_RGB_CTL                 ((__IO     uint16_t  *) REG_TMR_RGB_CTL)                 /*  Control */
#define pREG_TMR_RGB_CLRINT              ((__O      uint16_t  *) REG_TMR_RGB_CLRINT)              /*  Clear interrupt */
#define pREG_TMR_RGB_CAPTURE             ((__I __C  uint16_t  *) REG_TMR_RGB_CAPTURE)             /*  Capture */
#define pREG_TMR_RGB_ALOAD               ((__IO     uint16_t  *) REG_TMR_RGB_ALOAD)               /*  16-bit load value, asynchronous */
#define pREG_TMR_RGB_ACURCNT             ((__I __C  uint16_t  *) REG_TMR_RGB_ACURCNT)             /*  16-bit timer value, asynchronous */
#define pREG_TMR_RGB_STAT                ((__I __C  uint16_t  *) REG_TMR_RGB_STAT)                /*  Status */
#define pREG_TMR_RGB_PWM0CTL             ((__IO     uint16_t  *) REG_TMR_RGB_PWM0CTL)             /*  PWM0 Control Register */
#define pREG_TMR_RGB_PWM0MATCH           ((__IO     uint16_t  *) REG_TMR_RGB_PWM0MATCH)           /*  PWM0 Match Value */
#define pREG_TMR_RGB_EVENTSELECT         ((__IO     uint16_t  *) REG_TMR_RGB_EVENTSELECT)         /*  Timer Event selection Register */
#define pREG_TMR_RGB_PWM1CTL             ((__IO     uint16_t  *) REG_TMR_RGB_PWM1CTL)             /*  PWM1 Control Register */
#define pREG_TMR_RGB_PWM1MATCH           ((__IO     uint16_t  *) REG_TMR_RGB_PWM1MATCH)           /*  PWM1 Match Value */
#define pREG_TMR_RGB_PWM2CTL             ((__IO     uint16_t  *) REG_TMR_RGB_PWM2CTL)             /*  PWM2 Control Register */
#define pREG_TMR_RGB_PWM2MATCH           ((__IO     uint16_t  *) REG_TMR_RGB_PWM2MATCH)           /*  PWM2 Match Value */

/*  =================================================================================
 *      Real-Time Clock (RTC0)
 *  ================================================================================= */
#define pREG_RTC0_CR0                    ((__IO     uint16_t  *) REG_RTC0_CR0)                    /*  RTC Control 0 */
#define pREG_RTC0_SR0                    ((__IO     uint16_t  *) REG_RTC0_SR0)                    /*  RTC Status 0 */
#define pREG_RTC0_SR1                    ((__I __C  uint16_t  *) REG_RTC0_SR1)                    /*  RTC Status 1 */
#define pREG_RTC0_CNT0                   ((__IO     uint16_t  *) REG_RTC0_CNT0)                   /*  RTC Count 0 */
#define pREG_RTC0_CNT1                   ((__IO     uint16_t  *) REG_RTC0_CNT1)                   /*  RTC Count 1 */
#define pREG_RTC0_ALM0                   ((__IO     uint16_t  *) REG_RTC0_ALM0)                   /*  RTC Alarm 0 */
#define pREG_RTC0_ALM1                   ((__IO     uint16_t  *) REG_RTC0_ALM1)                   /*  RTC Alarm 1 */
#define pREG_RTC0_TRM                    ((__IO     uint16_t  *) REG_RTC0_TRM)                    /*  RTC Trim */
#define pREG_RTC0_GWY                    ((__O      uint16_t  *) REG_RTC0_GWY)                    /*  RTC Gateway */
#define pREG_RTC0_CR1                    ((__IO     uint16_t  *) REG_RTC0_CR1)                    /*  RTC Control 1 */
#define pREG_RTC0_SR2                    ((__IO     uint16_t  *) REG_RTC0_SR2)                    /*  RTC Status 2 */
#define pREG_RTC0_SNAP0                  ((__I __C  uint16_t  *) REG_RTC0_SNAP0)                  /*  RTC Snapshot 0 */
#define pREG_RTC0_SNAP1                  ((__I __C  uint16_t  *) REG_RTC0_SNAP1)                  /*  RTC Snapshot 1 */
#define pREG_RTC0_SNAP2                  ((__I __C  uint16_t  *) REG_RTC0_SNAP2)                  /*  RTC Snapshot 2 */
#define pREG_RTC0_MOD                    ((__I __C  uint16_t  *) REG_RTC0_MOD)                    /*  RTC Modulo */
#define pREG_RTC0_CNT2                   ((__I __C  uint16_t  *) REG_RTC0_CNT2)                   /*  RTC Count 2 */
#define pREG_RTC0_ALM2                   ((__IO     uint16_t  *) REG_RTC0_ALM2)                   /*  RTC Alarm 2 */
#define pREG_RTC0_SR3                    ((__IO     uint16_t  *) REG_RTC0_SR3)                    /*  RTC Status 3 */
#define pREG_RTC0_CR2IC                  ((__IO     uint16_t  *) REG_RTC0_CR2IC)                  /*  RTC Control 2 for Configuring Input Capture Channels */
#define pREG_RTC0_CR3SS                  ((__IO     uint16_t  *) REG_RTC0_CR3SS)                  /*  RTC Control 3 for Configuring SensorStrobe Channel */
#define pREG_RTC0_CR4SS                  ((__IO     uint16_t  *) REG_RTC0_CR4SS)                  /*  RTC Control 4 for Configuring SensorStrobe Channel */
#define pREG_RTC0_SSMSK                  ((__IO     uint16_t  *) REG_RTC0_SSMSK)                  /*  RTC Mask for SensorStrobe Channel */
#define pREG_RTC0_IC2                    ((__I __C  uint16_t  *) REG_RTC0_IC2)                    /*  RTC Input Capture Channel 2 */
#define pREG_RTC0_IC3                    ((__I __C  uint16_t  *) REG_RTC0_IC3)                    /*  RTC Input Capture Channel 3 */
#define pREG_RTC0_IC4                    ((__I __C  uint16_t  *) REG_RTC0_IC4)                    /*  RTC Input Capture Channel 4 */
#define pREG_RTC0_SS1                    ((__IO     uint16_t  *) REG_RTC0_SS1)                    /*  RTC SensorStrobe Channel 1 */
#define pREG_RTC0_SS2                    ((__IO     uint16_t  *) REG_RTC0_SS2)                    /*  RTC SensorStrobe Channel 2 */
#define pREG_RTC0_SS3                    ((__IO     uint16_t  *) REG_RTC0_SS3)                    /*  RTC SensorStrobe Channel 3 */
#define pREG_RTC0_SS4                    ((__IO     uint16_t  *) REG_RTC0_SS4)                    /*  RTC SensorStrobe Channel 4 */
#define pREG_RTC0_SR4                    ((__I __C  uint16_t  *) REG_RTC0_SR4)                    /*  RTC Status 4 */
#define pREG_RTC0_SR5                    ((__I __C  uint16_t  *) REG_RTC0_SR5)                    /*  RTC Status 5 */
#define pREG_RTC0_SR6                    ((__I __C  uint16_t  *) REG_RTC0_SR6)                    /*  RTC Status 6 */
#define pREG_RTC0_SS1TGT                 ((__I __C  uint16_t  *) REG_RTC0_SS1TGT)                 /*  RTC SensorStrobe Channel 1 Target */
#define pREG_RTC0_FRZCNT                 ((__I __C  uint16_t  *) REG_RTC0_FRZCNT)                 /*  RTC Freeze Count */
#define pREG_RTC0_SS2TGT                 ((__I __C  uint16_t  *) REG_RTC0_SS2TGT)                 /*  RTC SensorStrobe Channel 2 Target */
#define pREG_RTC0_SS3TGT                 ((__I __C  uint16_t  *) REG_RTC0_SS3TGT)                 /*  RTC SensorStrobe Channel 3 Target */
#define pREG_RTC0_SS1LOWDUR              ((__IO     uint16_t  *) REG_RTC0_SS1LOWDUR)              /*  RTC Auto-Reload Low Duration for SensorStrobe Channel 1 */
#define pREG_RTC0_SS2LOWDUR              ((__IO     uint16_t  *) REG_RTC0_SS2LOWDUR)              /*  RTC Auto-Reload Low Duration for SensorStrobe Channel 2 */
#define pREG_RTC0_SS3LOWDUR              ((__IO     uint16_t  *) REG_RTC0_SS3LOWDUR)              /*  RTC Auto-Reload Low Duration for SensorStrobe Channel 3 */
#define pREG_RTC0_SS1HIGHDUR             ((__IO     uint16_t  *) REG_RTC0_SS1HIGHDUR)             /*  RTC Auto-Reload High Duration for SensorStrobe Channel 1 */
#define pREG_RTC0_SS2HIGHDUR             ((__IO     uint16_t  *) REG_RTC0_SS2HIGHDUR)             /*  RTC Auto-Reload High Duration for SensorStrobe Channel 2 */
#define pREG_RTC0_SS3HIGHDUR             ((__IO     uint16_t  *) REG_RTC0_SS3HIGHDUR)             /*  RTC Auto-Reload High Duration for SensorStrobe Channel 3 */
#define pREG_RTC0_SSMSKOT                ((__IO     uint16_t  *) REG_RTC0_SSMSKOT)                /*  RTC Masks for SensorStrobe Channels on Time Control */
#define pREG_RTC0_CR5SSS                 ((__IO     uint16_t  *) REG_RTC0_CR5SSS)                 /*  RTC Control 5 for Configuring SensorStrobe Channel GPIO Sampling */
#define pREG_RTC0_CR6SSS                 ((__IO     uint16_t  *) REG_RTC0_CR6SSS)                 /*  RTC Control 6 for Configuring SensorStrobe Channel GPIO Sampling Edge */
#define pREG_RTC0_CR7SSS                 ((__IO     uint16_t  *) REG_RTC0_CR7SSS)                 /*  RTC Control 7 for Configuring SensorStrobe Channel GPIO Sampling Activity */
#define pREG_RTC0_SR7                    ((__IO     uint16_t  *) REG_RTC0_SR7)                    /*  RTC Status 7 */
#define pREG_RTC0_SR8                    ((__I __C  uint16_t  *) REG_RTC0_SR8)                    /*  RTC Status 8 */
#define pREG_RTC0_SR9                    ((__I __C  uint16_t  *) REG_RTC0_SR9)                    /*  RTC Status 9 */
#define pREG_RTC0_GPMUX0                 ((__IO     uint16_t  *) REG_RTC0_GPMUX0)                 /*  RTC GPIO Pin Mux Control Register 0 */
#define pREG_RTC0_GPMUX1                 ((__IO     uint16_t  *) REG_RTC0_GPMUX1)                 /*  RTC GPIO Pin Mux Control Register 1 */

/*  =================================================================================
 *      Real-Time Clock (RTC1)
 *  ================================================================================= */
#define pREG_RTC1_CR0                    ((__IO     uint16_t  *) REG_RTC1_CR0)                    /*  RTC Control 0 */
#define pREG_RTC1_SR0                    ((__IO     uint16_t  *) REG_RTC1_SR0)                    /*  RTC Status 0 */
#define pREG_RTC1_SR1                    ((__I __C  uint16_t  *) REG_RTC1_SR1)                    /*  RTC Status 1 */
#define pREG_RTC1_CNT0                   ((__IO     uint16_t  *) REG_RTC1_CNT0)                   /*  RTC Count 0 */
#define pREG_RTC1_CNT1                   ((__IO     uint16_t  *) REG_RTC1_CNT1)                   /*  RTC Count 1 */
#define pREG_RTC1_ALM0                   ((__IO     uint16_t  *) REG_RTC1_ALM0)                   /*  RTC Alarm 0 */
#define pREG_RTC1_ALM1                   ((__IO     uint16_t  *) REG_RTC1_ALM1)                   /*  RTC Alarm 1 */
#define pREG_RTC1_TRM                    ((__IO     uint16_t  *) REG_RTC1_TRM)                    /*  RTC Trim */
#define pREG_RTC1_GWY                    ((__O      uint16_t  *) REG_RTC1_GWY)                    /*  RTC Gateway */
#define pREG_RTC1_CR1                    ((__IO     uint16_t  *) REG_RTC1_CR1)                    /*  RTC Control 1 */
#define pREG_RTC1_SR2                    ((__IO     uint16_t  *) REG_RTC1_SR2)                    /*  RTC Status 2 */
#define pREG_RTC1_SNAP0                  ((__I __C  uint16_t  *) REG_RTC1_SNAP0)                  /*  RTC Snapshot 0 */
#define pREG_RTC1_SNAP1                  ((__I __C  uint16_t  *) REG_RTC1_SNAP1)                  /*  RTC Snapshot 1 */
#define pREG_RTC1_SNAP2                  ((__I __C  uint16_t  *) REG_RTC1_SNAP2)                  /*  RTC Snapshot 2 */
#define pREG_RTC1_MOD                    ((__I __C  uint16_t  *) REG_RTC1_MOD)                    /*  RTC Modulo */
#define pREG_RTC1_CNT2                   ((__I __C  uint16_t  *) REG_RTC1_CNT2)                   /*  RTC Count 2 */
#define pREG_RTC1_ALM2                   ((__IO     uint16_t  *) REG_RTC1_ALM2)                   /*  RTC Alarm 2 */
#define pREG_RTC1_SR3                    ((__IO     uint16_t  *) REG_RTC1_SR3)                    /*  RTC Status 3 */
#define pREG_RTC1_CR2IC                  ((__IO     uint16_t  *) REG_RTC1_CR2IC)                  /*  RTC Control 2 for Configuring Input Capture Channels */
#define pREG_RTC1_CR3SS                  ((__IO     uint16_t  *) REG_RTC1_CR3SS)                  /*  RTC Control 3 for Configuring SensorStrobe Channel */
#define pREG_RTC1_CR4SS                  ((__IO     uint16_t  *) REG_RTC1_CR4SS)                  /*  RTC Control 4 for Configuring SensorStrobe Channel */
#define pREG_RTC1_SSMSK                  ((__IO     uint16_t  *) REG_RTC1_SSMSK)                  /*  RTC Mask for SensorStrobe Channel */
#define pREG_RTC1_IC2                    ((__I __C  uint16_t  *) REG_RTC1_IC2)                    /*  RTC Input Capture Channel 2 */
#define pREG_RTC1_IC3                    ((__I __C  uint16_t  *) REG_RTC1_IC3)                    /*  RTC Input Capture Channel 3 */
#define pREG_RTC1_IC4                    ((__I __C  uint16_t  *) REG_RTC1_IC4)                    /*  RTC Input Capture Channel 4 */
#define pREG_RTC1_SS1                    ((__IO     uint16_t  *) REG_RTC1_SS1)                    /*  RTC SensorStrobe Channel 1 */
#define pREG_RTC1_SS2                    ((__IO     uint16_t  *) REG_RTC1_SS2)                    /*  RTC SensorStrobe Channel 2 */
#define pREG_RTC1_SS3                    ((__IO     uint16_t  *) REG_RTC1_SS3)                    /*  RTC SensorStrobe Channel 3 */
#define pREG_RTC1_SS4                    ((__IO     uint16_t  *) REG_RTC1_SS4)                    /*  RTC SensorStrobe Channel 4 */
#define pREG_RTC1_SR4                    ((__I __C  uint16_t  *) REG_RTC1_SR4)                    /*  RTC Status 4 */
#define pREG_RTC1_SR5                    ((__I __C  uint16_t  *) REG_RTC1_SR5)                    /*  RTC Status 5 */
#define pREG_RTC1_SR6                    ((__I __C  uint16_t  *) REG_RTC1_SR6)                    /*  RTC Status 6 */
#define pREG_RTC1_SS1TGT                 ((__I __C  uint16_t  *) REG_RTC1_SS1TGT)                 /*  RTC SensorStrobe Channel 1 Target */
#define pREG_RTC1_FRZCNT                 ((__I __C  uint16_t  *) REG_RTC1_FRZCNT)                 /*  RTC Freeze Count */
#define pREG_RTC1_SS2TGT                 ((__I __C  uint16_t  *) REG_RTC1_SS2TGT)                 /*  RTC SensorStrobe Channel 2 Target */
#define pREG_RTC1_SS3TGT                 ((__I __C  uint16_t  *) REG_RTC1_SS3TGT)                 /*  RTC SensorStrobe Channel 3 Target */
#define pREG_RTC1_SS1LOWDUR              ((__IO     uint16_t  *) REG_RTC1_SS1LOWDUR)              /*  RTC Auto-Reload Low Duration for SensorStrobe Channel 1 */
#define pREG_RTC1_SS2LOWDUR              ((__IO     uint16_t  *) REG_RTC1_SS2LOWDUR)              /*  RTC Auto-Reload Low Duration for SensorStrobe Channel 2 */
#define pREG_RTC1_SS3LOWDUR              ((__IO     uint16_t  *) REG_RTC1_SS3LOWDUR)              /*  RTC Auto-Reload Low Duration for SensorStrobe Channel 3 */
#define pREG_RTC1_SS1HIGHDUR             ((__IO     uint16_t  *) REG_RTC1_SS1HIGHDUR)             /*  RTC Auto-Reload High Duration for SensorStrobe Channel 1 */
#define pREG_RTC1_SS2HIGHDUR             ((__IO     uint16_t  *) REG_RTC1_SS2HIGHDUR)             /*  RTC Auto-Reload High Duration for SensorStrobe Channel 2 */
#define pREG_RTC1_SS3HIGHDUR             ((__IO     uint16_t  *) REG_RTC1_SS3HIGHDUR)             /*  RTC Auto-Reload High Duration for SensorStrobe Channel 3 */
#define pREG_RTC1_SSMSKOT                ((__IO     uint16_t  *) REG_RTC1_SSMSKOT)                /*  RTC Masks for SensorStrobe Channels on Time Control */
#define pREG_RTC1_CR5SSS                 ((__IO     uint16_t  *) REG_RTC1_CR5SSS)                 /*  RTC Control 5 for Configuring SensorStrobe Channel GPIO Sampling */
#define pREG_RTC1_CR6SSS                 ((__IO     uint16_t  *) REG_RTC1_CR6SSS)                 /*  RTC Control 6 for Configuring SensorStrobe Channel GPIO Sampling Edge */
#define pREG_RTC1_CR7SSS                 ((__IO     uint16_t  *) REG_RTC1_CR7SSS)                 /*  RTC Control 7 for Configuring SensorStrobe Channel GPIO Sampling Activity */
#define pREG_RTC1_SR7                    ((__IO     uint16_t  *) REG_RTC1_SR7)                    /*  RTC Status 7 */
#define pREG_RTC1_SR8                    ((__I __C  uint16_t  *) REG_RTC1_SR8)                    /*  RTC Status 8 */
#define pREG_RTC1_SR9                    ((__I __C  uint16_t  *) REG_RTC1_SR9)                    /*  RTC Status 9 */
#define pREG_RTC1_GPMUX0                 ((__IO     uint16_t  *) REG_RTC1_GPMUX0)                 /*  RTC GPIO Pin Mux Control Register 0 */
#define pREG_RTC1_GPMUX1                 ((__IO     uint16_t  *) REG_RTC1_GPMUX1)                 /*  RTC GPIO Pin Mux Control Register 1 */

/*  =================================================================================
 *      System Identification and Debug Enable (SYS)
 *  ================================================================================= */
#define pREG_SYS_ADIID                   ((__I __C  uint16_t  *) REG_SYS_ADIID)                   /*  ADI Identification */
#define pREG_SYS_CHIPID                  ((__I __C  uint16_t  *) REG_SYS_CHIPID)                  /*  Chip Identifier */
#define pREG_SYS_SWDEN                   ((__O      uint16_t  *) REG_SYS_SWDEN)                   /*  Serial Wire Debug Enable */

/*  =================================================================================
 *      Watchdog Timer (WDT0)
 *  ================================================================================= */
#define pREG_WDT0_LOAD                   ((__IO     uint16_t  *) REG_WDT0_LOAD)                   /*  Load Value */
#define pREG_WDT0_CCNT                   ((__I __C  uint16_t  *) REG_WDT0_CCNT)                   /*  Current Count Value */
#define pREG_WDT0_CTL                    ((__IO     uint16_t  *) REG_WDT0_CTL)                    /*  Control */
#define pREG_WDT0_RESTART                ((__O      uint16_t  *) REG_WDT0_RESTART)                /*  Clear Interrupt */
#define pREG_WDT0_STAT                   ((__I __C  uint16_t  *) REG_WDT0_STAT)                   /*  Status */

/*  =================================================================================
 *      I2C Master/Slave (I2C0)
 *  ================================================================================= */
#define pREG_I2C0_MCTL                   ((__IO     uint16_t  *) REG_I2C0_MCTL)                   /*  Master Control */
#define pREG_I2C0_MSTAT                  ((__IO     uint16_t  *) REG_I2C0_MSTAT)                  /*  Master Status */
#define pREG_I2C0_MRX                    ((__I __C  uint16_t  *) REG_I2C0_MRX)                    /*  Master Receive Data */
#define pREG_I2C0_MTX                    ((__IO     uint16_t  *) REG_I2C0_MTX)                    /*  Master Transmit Data */
#define pREG_I2C0_MRXCNT                 ((__IO     uint16_t  *) REG_I2C0_MRXCNT)                 /*  Master Receive Data Count */
#define pREG_I2C0_MCRXCNT                ((__I __C  uint16_t  *) REG_I2C0_MCRXCNT)                /*  Master Current Receive Data Count */
#define pREG_I2C0_ADDR1                  ((__IO     uint16_t  *) REG_I2C0_ADDR1)                  /*  Master Address Byte 1 */
#define pREG_I2C0_ADDR2                  ((__IO     uint16_t  *) REG_I2C0_ADDR2)                  /*  Master Address Byte 2 */
#define pREG_I2C0_BYT                    ((__IO     uint16_t  *) REG_I2C0_BYT)                    /*  Start Byte */
#define pREG_I2C0_DIV                    ((__IO     uint16_t  *) REG_I2C0_DIV)                    /*  Serial Clock Period Divisor */
#define pREG_I2C0_SCTL                   ((__IO     uint16_t  *) REG_I2C0_SCTL)                   /*  Slave Control */
#define pREG_I2C0_SSTAT                  ((__IO     uint16_t  *) REG_I2C0_SSTAT)                  /*  Slave I2C Status/Error/IRQ */
#define pREG_I2C0_SRX                    ((__I __C  uint16_t  *) REG_I2C0_SRX)                    /*  Slave Receive */
#define pREG_I2C0_STX                    ((__IO     uint16_t  *) REG_I2C0_STX)                    /*  Slave Transmit */
#define pREG_I2C0_ALT                    ((__IO     uint16_t  *) REG_I2C0_ALT)                    /*  Hardware General Call ID */
#define pREG_I2C0_ID0                    ((__IO     uint16_t  *) REG_I2C0_ID0)                    /*  First Slave Address Device ID */
#define pREG_I2C0_ID1                    ((__IO     uint16_t  *) REG_I2C0_ID1)                    /*  Second Slave Address Device ID */
#define pREG_I2C0_ID2                    ((__IO     uint16_t  *) REG_I2C0_ID2)                    /*  Third Slave Address Device ID */
#define pREG_I2C0_ID3                    ((__IO     uint16_t  *) REG_I2C0_ID3)                    /*  Fourth Slave Address Device ID */
#define pREG_I2C0_STAT                   ((__IO     uint16_t  *) REG_I2C0_STAT)                   /*  Master and Slave FIFO Status */
#define pREG_I2C0_SHCTL                  ((__O      uint16_t  *) REG_I2C0_SHCTL)                  /*  Shared Control */
#define pREG_I2C0_TCTL                   ((__IO     uint16_t  *) REG_I2C0_TCTL)                   /*  Timing Control Register */
#define pREG_I2C0_ASTRETCH_SCL           ((__IO     uint16_t  *) REG_I2C0_ASTRETCH_SCL)           /*  Automatic Stretch SCL */

/*  =================================================================================
 *      Serial Peripheral Interface (SPI0)
 *  ================================================================================= */
#define pREG_SPI0_STAT                   ((__IO     uint16_t  *) REG_SPI0_STAT)                   /*  Status */
#define pREG_SPI0_RX                     ((__I __C  uint16_t  *) REG_SPI0_RX)                     /*  Receive */
#define pREG_SPI0_TX                     ((__O      uint16_t  *) REG_SPI0_TX)                     /*  Transmit */
#define pREG_SPI0_DIV                    ((__IO     uint16_t  *) REG_SPI0_DIV)                    /*  SPI Baud Rate Selection */
#define pREG_SPI0_CTL                    ((__IO     uint16_t  *) REG_SPI0_CTL)                    /*  SPI Configuration */
#define pREG_SPI0_IEN                    ((__IO     uint16_t  *) REG_SPI0_IEN)                    /*  SPI Interrupts Enable */
#define pREG_SPI0_CNT                    ((__IO     uint16_t  *) REG_SPI0_CNT)                    /*  Transfer Byte Count */
#define pREG_SPI0_DMA                    ((__IO     uint16_t  *) REG_SPI0_DMA)                    /*  SPI DMA Enable */
#define pREG_SPI0_FIFO_STAT              ((__I __C  uint16_t  *) REG_SPI0_FIFO_STAT)              /*  FIFO Status */
#define pREG_SPI0_RD_CTL                 ((__IO     uint16_t  *) REG_SPI0_RD_CTL)                 /*  Read Control */
#define pREG_SPI0_FLOW_CTL               ((__IO     uint16_t  *) REG_SPI0_FLOW_CTL)               /*  Flow Control */
#define pREG_SPI0_WAIT_TMR               ((__IO     uint16_t  *) REG_SPI0_WAIT_TMR)               /*  Wait Timer for Flow Control */
#define pREG_SPI0_CS_CTL                 ((__IO     uint16_t  *) REG_SPI0_CS_CTL)                 /*  Chip Select Control for Multi-slave Connections */
#define pREG_SPI0_CS_OVERRIDE            ((__IO     uint16_t  *) REG_SPI0_CS_OVERRIDE)            /*  Chip Select Override */

/*  =================================================================================
 *      Serial Peripheral Interface (SPI1)
 *  ================================================================================= */
#define pREG_SPI1_STAT                   ((__IO     uint16_t  *) REG_SPI1_STAT)                   /*  Status */
#define pREG_SPI1_RX                     ((__I __C  uint16_t  *) REG_SPI1_RX)                     /*  Receive */
#define pREG_SPI1_TX                     ((__O      uint16_t  *) REG_SPI1_TX)                     /*  Transmit */
#define pREG_SPI1_DIV                    ((__IO     uint16_t  *) REG_SPI1_DIV)                    /*  SPI Baud Rate Selection */
#define pREG_SPI1_CTL                    ((__IO     uint16_t  *) REG_SPI1_CTL)                    /*  SPI Configuration */
#define pREG_SPI1_IEN                    ((__IO     uint16_t  *) REG_SPI1_IEN)                    /*  SPI Interrupts Enable */
#define pREG_SPI1_CNT                    ((__IO     uint16_t  *) REG_SPI1_CNT)                    /*  Transfer Byte Count */
#define pREG_SPI1_DMA                    ((__IO     uint16_t  *) REG_SPI1_DMA)                    /*  SPI DMA Enable */
#define pREG_SPI1_FIFO_STAT              ((__I __C  uint16_t  *) REG_SPI1_FIFO_STAT)              /*  FIFO Status */
#define pREG_SPI1_RD_CTL                 ((__IO     uint16_t  *) REG_SPI1_RD_CTL)                 /*  Read Control */
#define pREG_SPI1_FLOW_CTL               ((__IO     uint16_t  *) REG_SPI1_FLOW_CTL)               /*  Flow Control */
#define pREG_SPI1_WAIT_TMR               ((__IO     uint16_t  *) REG_SPI1_WAIT_TMR)               /*  Wait Timer for Flow Control */
#define pREG_SPI1_CS_CTL                 ((__IO     uint16_t  *) REG_SPI1_CS_CTL)                 /*  Chip Select Control for Multi-slave Connections */
#define pREG_SPI1_CS_OVERRIDE            ((__IO     uint16_t  *) REG_SPI1_CS_OVERRIDE)            /*  Chip Select Override */

/*  =================================================================================
 *      Serial Peripheral Interface (SPI2)
 *  ================================================================================= */
#define pREG_SPI2_STAT                   ((__IO     uint16_t  *) REG_SPI2_STAT)                   /*  Status */
#define pREG_SPI2_RX                     ((__I __C  uint16_t  *) REG_SPI2_RX)                     /*  Receive */
#define pREG_SPI2_TX                     ((__O      uint16_t  *) REG_SPI2_TX)                     /*  Transmit */
#define pREG_SPI2_DIV                    ((__IO     uint16_t  *) REG_SPI2_DIV)                    /*  SPI Baud Rate Selection */
#define pREG_SPI2_CTL                    ((__IO     uint16_t  *) REG_SPI2_CTL)                    /*  SPI Configuration */
#define pREG_SPI2_IEN                    ((__IO     uint16_t  *) REG_SPI2_IEN)                    /*  SPI Interrupts Enable */
#define pREG_SPI2_CNT                    ((__IO     uint16_t  *) REG_SPI2_CNT)                    /*  Transfer Byte Count */
#define pREG_SPI2_DMA                    ((__IO     uint16_t  *) REG_SPI2_DMA)                    /*  SPI DMA Enable */
#define pREG_SPI2_FIFO_STAT              ((__I __C  uint16_t  *) REG_SPI2_FIFO_STAT)              /*  FIFO Status */
#define pREG_SPI2_RD_CTL                 ((__IO     uint16_t  *) REG_SPI2_RD_CTL)                 /*  Read Control */
#define pREG_SPI2_FLOW_CTL               ((__IO     uint16_t  *) REG_SPI2_FLOW_CTL)               /*  Flow Control */
#define pREG_SPI2_WAIT_TMR               ((__IO     uint16_t  *) REG_SPI2_WAIT_TMR)               /*  Wait Timer for Flow Control */
#define pREG_SPI2_CS_CTL                 ((__IO     uint16_t  *) REG_SPI2_CS_CTL)                 /*  Chip Select Control for Multi-slave Connections */
#define pREG_SPI2_CS_OVERRIDE            ((__IO     uint16_t  *) REG_SPI2_CS_OVERRIDE)            /*  Chip Select Override */

/*  =================================================================================
 *       (UART0)
 *  ================================================================================= */
#define pREG_UART0_TX                    ((__O      uint16_t  *) REG_UART0_TX)                    /*  Transmit Holding Register */
#define pREG_UART0_RX                    ((__I __C  uint16_t  *) REG_UART0_RX)                    /*  Receive Buffer Register */
#define pREG_UART0_IEN                   ((__IO     uint16_t  *) REG_UART0_IEN)                   /*  Interrupt Enable */
#define pREG_UART0_IIR                   ((__I __C  uint16_t  *) REG_UART0_IIR)                   /*  Interrupt ID */
#define pREG_UART0_LCR                   ((__IO     uint16_t  *) REG_UART0_LCR)                   /*  Line Control */
#define pREG_UART0_MCR                   ((__IO     uint16_t  *) REG_UART0_MCR)                   /*  Modem Control */
#define pREG_UART0_LSR                   ((__I __C  uint16_t  *) REG_UART0_LSR)                   /*  Line Status */
#define pREG_UART0_MSR                   ((__I __C  uint16_t  *) REG_UART0_MSR)                   /*  Modem Status */
#define pREG_UART0_SCR                   ((__IO     uint16_t  *) REG_UART0_SCR)                   /*  Scratch Buffer */
#define pREG_UART0_FCR                   ((__IO     uint16_t  *) REG_UART0_FCR)                   /*  FIFO Control */
#define pREG_UART0_FBR                   ((__IO     uint16_t  *) REG_UART0_FBR)                   /*  Fractional Baud Rate */
#define pREG_UART0_DIV                   ((__IO     uint16_t  *) REG_UART0_DIV)                   /*  Baud Rate Divider */
#define pREG_UART0_LCR2                  ((__IO     uint16_t  *) REG_UART0_LCR2)                  /*  Second Line Control */
#define pREG_UART0_CTL                   ((__IO     uint16_t  *) REG_UART0_CTL)                   /*  UART Control Register */
#define pREG_UART0_RFC                   ((__I __C  uint16_t  *) REG_UART0_RFC)                   /*  RX FIFO Byte Count */
#define pREG_UART0_TFC                   ((__I __C  uint16_t  *) REG_UART0_TFC)                   /*  TX FIFO Byte Count */
#define pREG_UART0_RSC                   ((__IO     uint16_t  *) REG_UART0_RSC)                   /*  RS485 Half-duplex Control */
#define pREG_UART0_ACR                   ((__IO     uint16_t  *) REG_UART0_ACR)                   /*  Auto Baud Control */
#define pREG_UART0_ASRL                  ((__I __C  uint16_t  *) REG_UART0_ASRL)                  /*  Auto Baud Status (Low) */
#define pREG_UART0_ASRH                  ((__I __C  uint16_t  *) REG_UART0_ASRH)                  /*  Auto Baud Status (High) */

/*  =================================================================================
 *       (UART1)
 *  ================================================================================= */
#define pREG_UART1_RX                    ((__I __C  uint16_t  *) REG_UART1_RX)                    /*  Receive Buffer Register */
#define pREG_UART1_TX                    ((__O      uint16_t  *) REG_UART1_TX)                    /*  Transmit Holding Register */
#define pREG_UART1_IEN                   ((__IO     uint16_t  *) REG_UART1_IEN)                   /*  Interrupt Enable */
#define pREG_UART1_IIR                   ((__I __C  uint16_t  *) REG_UART1_IIR)                   /*  Interrupt ID */
#define pREG_UART1_LCR                   ((__IO     uint16_t  *) REG_UART1_LCR)                   /*  Line Control */
#define pREG_UART1_MCR                   ((__IO     uint16_t  *) REG_UART1_MCR)                   /*  Modem Control */
#define pREG_UART1_LSR                   ((__I __C  uint16_t  *) REG_UART1_LSR)                   /*  Line Status */
#define pREG_UART1_MSR                   ((__I __C  uint16_t  *) REG_UART1_MSR)                   /*  Modem Status */
#define pREG_UART1_SCR                   ((__IO     uint16_t  *) REG_UART1_SCR)                   /*  Scratch Buffer */
#define pREG_UART1_FCR                   ((__IO     uint16_t  *) REG_UART1_FCR)                   /*  FIFO Control */
#define pREG_UART1_FBR                   ((__IO     uint16_t  *) REG_UART1_FBR)                   /*  Fractional Baud Rate */
#define pREG_UART1_DIV                   ((__IO     uint16_t  *) REG_UART1_DIV)                   /*  Baud Rate Divider */
#define pREG_UART1_LCR2                  ((__IO     uint16_t  *) REG_UART1_LCR2)                  /*  Second Line Control */
#define pREG_UART1_CTL                   ((__IO     uint16_t  *) REG_UART1_CTL)                   /*  UART Control Register */
#define pREG_UART1_RFC                   ((__I __C  uint16_t  *) REG_UART1_RFC)                   /*  RX FIFO Byte Count */
#define pREG_UART1_TFC                   ((__I __C  uint16_t  *) REG_UART1_TFC)                   /*  TX FIFO Byte Count */
#define pREG_UART1_RSC                   ((__IO     uint16_t  *) REG_UART1_RSC)                   /*  RS485 Half-duplex Control */
#define pREG_UART1_ACR                   ((__IO     uint16_t  *) REG_UART1_ACR)                   /*  Auto Baud Control */
#define pREG_UART1_ASRL                  ((__I __C  uint16_t  *) REG_UART1_ASRL)                  /*  Auto Baud Status (Low) */
#define pREG_UART1_ASRH                  ((__I __C  uint16_t  *) REG_UART1_ASRH)                  /*  Auto Baud Status (High) */

/*  =================================================================================
 *      Beeper Driver (BEEP0)
 *  ================================================================================= */
#define pREG_BEEP0_CFG                   ((__IO     uint16_t  *) REG_BEEP0_CFG)                   /*  Beeper Configuration */
#define pREG_BEEP0_STAT                  ((__IO     uint16_t  *) REG_BEEP0_STAT)                  /*  Beeper Status */
#define pREG_BEEP0_TONEA                 ((__IO     uint16_t  *) REG_BEEP0_TONEA)                 /*  Tone A Data */
#define pREG_BEEP0_TONEB                 ((__IO     uint16_t  *) REG_BEEP0_TONEB)                 /*  Tone B Data */

/*  =================================================================================
 *       (ADC0)
 *  ================================================================================= */
#define pREG_ADC0_CFG                    ((__IO     uint16_t  *) REG_ADC0_CFG)                    /*  ADC Configuration */
#define pREG_ADC0_PWRUP                  ((__IO     uint16_t  *) REG_ADC0_PWRUP)                  /*  ADC Power-up Time */
#define pREG_ADC0_CAL_WORD               ((__IO     uint16_t  *) REG_ADC0_CAL_WORD)               /*  Calibration Word */
#define pREG_ADC0_CNV_CFG                ((__IO     uint16_t  *) REG_ADC0_CNV_CFG)                /*  ADC Conversion Configuration */
#define pREG_ADC0_CNV_TIME               ((__IO     uint16_t  *) REG_ADC0_CNV_TIME)               /*  ADC Conversion Time */
#define pREG_ADC0_AVG_CFG                ((__IO     uint16_t  *) REG_ADC0_AVG_CFG)                /*  Averaging Configuration */
#define pREG_ADC0_IRQ_EN                 ((__IO     uint16_t  *) REG_ADC0_IRQ_EN)                 /*  Interrupt Enable */
#define pREG_ADC0_STAT                   ((__IO     uint16_t  *) REG_ADC0_STAT)                   /*  ADC Status */
#define pREG_ADC0_OVF                    ((__IO     uint16_t  *) REG_ADC0_OVF)                    /*  Overflow of Output Registers */
#define pREG_ADC0_ALERT                  ((__IO     uint16_t  *) REG_ADC0_ALERT)                  /*  Alert Indication */
#define pREG_ADC0_CH0_OUT                ((__I __C  uint16_t  *) REG_ADC0_CH0_OUT)                /*  Conversion Result Channel 0 */
#define pREG_ADC0_CH1_OUT                ((__I __C  uint16_t  *) REG_ADC0_CH1_OUT)                /*  Conversion Result Channel 1 */
#define pREG_ADC0_CH2_OUT                ((__I __C  uint16_t  *) REG_ADC0_CH2_OUT)                /*  Conversion Result Channel 2 */
#define pREG_ADC0_CH3_OUT                ((__I __C  uint16_t  *) REG_ADC0_CH3_OUT)                /*  Conversion Result Channel 3 */
#define pREG_ADC0_CH4_OUT                ((__I __C  uint16_t  *) REG_ADC0_CH4_OUT)                /*  Conversion Result Channel 4 */
#define pREG_ADC0_CH5_OUT                ((__I __C  uint16_t  *) REG_ADC0_CH5_OUT)                /*  Conversion Result Channel 5 */
#define pREG_ADC0_CH6_OUT                ((__I __C  uint16_t  *) REG_ADC0_CH6_OUT)                /*  Conversion Result Channel 6 */
#define pREG_ADC0_CH7_OUT                ((__I __C  uint16_t  *) REG_ADC0_CH7_OUT)                /*  Conversion Result Channel 7 */
#define pREG_ADC0_BAT_OUT                ((__I __C  uint16_t  *) REG_ADC0_BAT_OUT)                /*  Battery Monitoring Result */
#define pREG_ADC0_TMP_OUT                ((__I __C  uint16_t  *) REG_ADC0_TMP_OUT)                /*  Temperature Result */
#define pREG_ADC0_TMP2_OUT               ((__I __C  uint16_t  *) REG_ADC0_TMP2_OUT)               /*  Temperature Result 2 */
#define pREG_ADC0_DMA_OUT                ((__I __C  uint16_t  *) REG_ADC0_DMA_OUT)                /*  DMA Output Register */
#define pREG_ADC0_LIM0_LO                ((__IO     uint16_t  *) REG_ADC0_LIM0_LO)                /*  Channel 0 Low Limit */
#define pREG_ADC0_LIM0_HI                ((__IO     uint16_t  *) REG_ADC0_LIM0_HI)                /*  Channel 0 High Limit */
#define pREG_ADC0_HYS0                   ((__IO     uint16_t  *) REG_ADC0_HYS0)                   /*  Channel 0 Hysteresis */
#define pREG_ADC0_LIM1_LO                ((__IO     uint16_t  *) REG_ADC0_LIM1_LO)                /*  Channel 1 Low Limit */
#define pREG_ADC0_LIM1_HI                ((__IO     uint16_t  *) REG_ADC0_LIM1_HI)                /*  Channel 1 High Limit */
#define pREG_ADC0_HYS1                   ((__IO     uint16_t  *) REG_ADC0_HYS1)                   /*  Channel 1 Hysteresis */
#define pREG_ADC0_LIM2_LO                ((__IO     uint16_t  *) REG_ADC0_LIM2_LO)                /*  Channel 2 Low Limit */
#define pREG_ADC0_LIM2_HI                ((__IO     uint16_t  *) REG_ADC0_LIM2_HI)                /*  Channel 2 High Limit */
#define pREG_ADC0_HYS2                   ((__IO     uint16_t  *) REG_ADC0_HYS2)                   /*  Channel 2 Hysteresis */
#define pREG_ADC0_LIM3_LO                ((__IO     uint16_t  *) REG_ADC0_LIM3_LO)                /*  Channel 3 Low Limit */
#define pREG_ADC0_LIM3_HI                ((__IO     uint16_t  *) REG_ADC0_LIM3_HI)                /*  Channel 3 High Limit */
#define pREG_ADC0_HYS3                   ((__IO     uint16_t  *) REG_ADC0_HYS3)                   /*  Channel 3 Hysteresis */
#define pREG_ADC0_CFG1                   ((__IO     uint16_t  *) REG_ADC0_CFG1)                   /*  Reference Buffer Low Power Mode */

/*  =================================================================================
 *      DMA (DMA0)
 *  ================================================================================= */
#define pREG_DMA0_STAT                   ((__I __C  uint32_t  *) REG_DMA0_STAT)                   /*  DMA Status */
#define pREG_DMA0_CFG                    ((__O      uint32_t  *) REG_DMA0_CFG)                    /*  DMA Configuration */
#define pREG_DMA0_PDBPTR                 ((__IO     uint32_t  *) REG_DMA0_PDBPTR)                 /*  DMA Channel Primary Control Database Pointer */
#define pREG_DMA0_ADBPTR                 ((__I __C  uint32_t  *) REG_DMA0_ADBPTR)                 /*  DMA Channel Alternate Control Database Pointer */
#define pREG_DMA0_SWREQ                  ((__O      uint32_t  *) REG_DMA0_SWREQ)                  /*  DMA Channel Software Request */
#define pREG_DMA0_RMSK_SET               ((__IO     uint32_t  *) REG_DMA0_RMSK_SET)               /*  DMA Channel Request Mask Set */
#define pREG_DMA0_RMSK_CLR               ((__O      uint32_t  *) REG_DMA0_RMSK_CLR)               /*  DMA Channel Request Mask Clear */
#define pREG_DMA0_EN_SET                 ((__IO     uint32_t  *) REG_DMA0_EN_SET)                 /*  DMA Channel Enable Set */
#define pREG_DMA0_EN_CLR                 ((__O      uint32_t  *) REG_DMA0_EN_CLR)                 /*  DMA Channel Enable Clear */
#define pREG_DMA0_ALT_SET                ((__IO     uint32_t  *) REG_DMA0_ALT_SET)                /*  DMA Channel Primary Alternate Set */
#define pREG_DMA0_ALT_CLR                ((__O      uint32_t  *) REG_DMA0_ALT_CLR)                /*  DMA Channel Primary Alternate Clear */
#define pREG_DMA0_PRI_SET                ((__O      uint32_t  *) REG_DMA0_PRI_SET)                /*  DMA Channel Priority Set */
#define pREG_DMA0_PRI_CLR                ((__O      uint32_t  *) REG_DMA0_PRI_CLR)                /*  DMA Channel Priority Clear */
#define pREG_DMA0_ERRCHNL_CLR            ((__IO     uint32_t  *) REG_DMA0_ERRCHNL_CLR)            /*  DMA per Channel Error Clear */
#define pREG_DMA0_ERR_CLR                ((__IO     uint32_t  *) REG_DMA0_ERR_CLR)                /*  DMA Bus Error Clear */
#define pREG_DMA0_INVALIDDESC_CLR        ((__IO     uint32_t  *) REG_DMA0_INVALIDDESC_CLR)        /*  DMA per Channel Invalid Descriptor Clear */
#define pREG_DMA0_BS_SET                 ((__IO     uint32_t  *) REG_DMA0_BS_SET)                 /*  DMA Channel Bytes Swap Enable Set */
#define pREG_DMA0_BS_CLR                 ((__O      uint32_t  *) REG_DMA0_BS_CLR)                 /*  DMA Channel Bytes Swap Enable Clear */
#define pREG_DMA0_SRCADDR_SET            ((__IO     uint32_t  *) REG_DMA0_SRCADDR_SET)            /*  DMA Channel Source Address Decrement Enable Set */
#define pREG_DMA0_SRCADDR_CLR            ((__O      uint32_t  *) REG_DMA0_SRCADDR_CLR)            /*  DMA Channel Source Address Decrement Enable Clear */
#define pREG_DMA0_DSTADDR_SET            ((__IO     uint32_t  *) REG_DMA0_DSTADDR_SET)            /*  DMA Channel Destination Address Decrement Enable Set */
#define pREG_DMA0_DSTADDR_CLR            ((__O      uint32_t  *) REG_DMA0_DSTADDR_CLR)            /*  DMA Channel Destination Address Decrement Enable Clear */
#define pREG_DMA0_REVID                  ((__I __C  uint32_t  *) REG_DMA0_REVID)                  /*  DMA Controller Revision ID */

/*  =================================================================================
 *      Flash Controller (FLCC0)
 *  ================================================================================= */
#define pREG_FLCC0_STAT                  ((__IO     uint32_t  *) REG_FLCC0_STAT)                  /*  Status */
#define pREG_FLCC0_IEN                   ((__IO     uint32_t  *) REG_FLCC0_IEN)                   /*  Interrupt Enable */
#define pREG_FLCC0_CMD                   ((__IO     uint32_t  *) REG_FLCC0_CMD)                   /*  Command */
#define pREG_FLCC0_KH_ADDR               ((__IO     uint32_t  *) REG_FLCC0_KH_ADDR)               /*  Write Address */
#define pREG_FLCC0_KH_DATA0              ((__IO     uint32_t  *) REG_FLCC0_KH_DATA0)              /*  Write Lower Data */
#define pREG_FLCC0_KH_DATA1              ((__IO     uint32_t  *) REG_FLCC0_KH_DATA1)              /*  Write Upper Data */
#define pREG_FLCC0_PAGE_ADDR0            ((__IO     uint32_t  *) REG_FLCC0_PAGE_ADDR0)            /*  Lower Page Address */
#define pREG_FLCC0_PAGE_ADDR1            ((__IO     uint32_t  *) REG_FLCC0_PAGE_ADDR1)            /*  Upper Page Address */
#define pREG_FLCC0_KEY                   ((__O      uint32_t  *) REG_FLCC0_KEY)                   /*  Key */
#define pREG_FLCC0_WR_ABORT_ADDR         ((__I __C  uint32_t  *) REG_FLCC0_WR_ABORT_ADDR)         /*  Write Abort Address */
#define pREG_FLCC0_WRPROT                ((__IO     uint32_t  *) REG_FLCC0_WRPROT)                /*  Write Protection */
#define pREG_FLCC0_SIGNATURE             ((__I __C  uint32_t  *) REG_FLCC0_SIGNATURE)             /*  Signature */
#define pREG_FLCC0_UCFG                  ((__IO     uint32_t  *) REG_FLCC0_UCFG)                  /*  User Configuration */
#define pREG_FLCC0_TIME_PARAM0           ((__IO     uint32_t  *) REG_FLCC0_TIME_PARAM0)           /*  Time Parameter 0 */
#define pREG_FLCC0_TIME_PARAM1           ((__IO     uint32_t  *) REG_FLCC0_TIME_PARAM1)           /*  Time Parameter 1 */
#define pREG_FLCC0_ABORT_EN_LO           ((__IO     uint32_t  *) REG_FLCC0_ABORT_EN_LO)           /*  IRQ Abort Enable (Lower Bits) */
#define pREG_FLCC0_ABORT_EN_HI           ((__IO     uint32_t  *) REG_FLCC0_ABORT_EN_HI)           /*  IRQ Abort Enable (Upper Bits) */
#define pREG_FLCC0_ECC_CFG               ((__IO     uint32_t  *) REG_FLCC0_ECC_CFG)               /*  ECC Configuration */
#define pREG_FLCC0_ECC_ADDR              ((__I __C  uint32_t  *) REG_FLCC0_ECC_ADDR)              /*  ECC Status (Address) */
#define pREG_FLCC0_POR_SEC               ((__IO     uint32_t  *) REG_FLCC0_POR_SEC)               /*  Flash Security */
#define pREG_FLCC0_VOL_CFG               ((__IO     uint32_t  *) REG_FLCC0_VOL_CFG)               /*  Volatile Flash Configuration */

/*  =================================================================================
 *      Cache Controller (FLCC0_CACHE)
 *  ================================================================================= */
#define pREG_FLCC0_CACHE_STAT            ((__I __C  uint32_t  *) REG_FLCC0_CACHE_STAT)            /*  Cache Status Register */
#define pREG_FLCC0_CACHE_SETUP           ((__IO     uint32_t  *) REG_FLCC0_CACHE_SETUP)           /*  Cache Setup Register */
#define pREG_FLCC0_CACHE_KEY             ((__O      uint32_t  *) REG_FLCC0_CACHE_KEY)             /*  Cache Key Register */

/*  =================================================================================
 *       (GPIO0)
 *  ================================================================================= */
#define pREG_GPIO0_CFG                   ((__IO     uint32_t  *) REG_GPIO0_CFG)                   /*  Port  Configuration */
#define pREG_GPIO0_OEN                   ((__IO     uint16_t  *) REG_GPIO0_OEN)                   /*  Port Output Enable */
#define pREG_GPIO0_PE                    ((__IO     uint16_t  *) REG_GPIO0_PE)                    /*  Port Output Pull-up/Pull-down Enable */
#define pREG_GPIO0_IEN                   ((__IO     uint16_t  *) REG_GPIO0_IEN)                   /*  Port  Input Path Enable */
#define pREG_GPIO0_IN                    ((__I __C  uint16_t  *) REG_GPIO0_IN)                    /*  Port  Registered Data Input */
#define pREG_GPIO0_OUT                   ((__IO     uint16_t  *) REG_GPIO0_OUT)                   /*  Port Data Output */
#define pREG_GPIO0_SET                   ((__O      uint16_t  *) REG_GPIO0_SET)                   /*  Port Data Out Set */
#define pREG_GPIO0_CLR                   ((__O      uint16_t  *) REG_GPIO0_CLR)                   /*  Port Data Out Clear */
#define pREG_GPIO0_TGL                   ((__O      uint16_t  *) REG_GPIO0_TGL)                   /*  Port Pin Toggle */
#define pREG_GPIO0_POL                   ((__IO     uint16_t  *) REG_GPIO0_POL)                   /*  Port Interrupt Polarity */
#define pREG_GPIO0_IENA                  ((__IO     uint16_t  *) REG_GPIO0_IENA)                  /*  Port Interrupt A Enable */
#define pREG_GPIO0_IENB                  ((__IO     uint16_t  *) REG_GPIO0_IENB)                  /*  Port Interrupt B Enable */
#define pREG_GPIO0_INT                   ((__IO     uint16_t  *) REG_GPIO0_INT)                   /*  Port Interrupt Status */
#define pREG_GPIO0_DS                    ((__IO     uint16_t  *) REG_GPIO0_DS)                    /*  Port Drive Strength Select */

/*  =================================================================================
 *       (GPIO1)
 *  ================================================================================= */
#define pREG_GPIO1_CFG                   ((__IO     uint32_t  *) REG_GPIO1_CFG)                   /*  Port  Configuration */
#define pREG_GPIO1_OEN                   ((__IO     uint16_t  *) REG_GPIO1_OEN)                   /*  Port Output Enable */
#define pREG_GPIO1_PE                    ((__IO     uint16_t  *) REG_GPIO1_PE)                    /*  Port Output Pull-up/Pull-down Enable */
#define pREG_GPIO1_IEN                   ((__IO     uint16_t  *) REG_GPIO1_IEN)                   /*  Port  Input Path Enable */
#define pREG_GPIO1_IN                    ((__I __C  uint16_t  *) REG_GPIO1_IN)                    /*  Port  Registered Data Input */
#define pREG_GPIO1_OUT                   ((__IO     uint16_t  *) REG_GPIO1_OUT)                   /*  Port Data Output */
#define pREG_GPIO1_SET                   ((__O      uint16_t  *) REG_GPIO1_SET)                   /*  Port Data Out Set */
#define pREG_GPIO1_CLR                   ((__O      uint16_t  *) REG_GPIO1_CLR)                   /*  Port Data Out Clear */
#define pREG_GPIO1_TGL                   ((__O      uint16_t  *) REG_GPIO1_TGL)                   /*  Port Pin Toggle */
#define pREG_GPIO1_POL                   ((__IO     uint16_t  *) REG_GPIO1_POL)                   /*  Port Interrupt Polarity */
#define pREG_GPIO1_IENA                  ((__IO     uint16_t  *) REG_GPIO1_IENA)                  /*  Port Interrupt A Enable */
#define pREG_GPIO1_IENB                  ((__IO     uint16_t  *) REG_GPIO1_IENB)                  /*  Port Interrupt B Enable */
#define pREG_GPIO1_INT                   ((__IO     uint16_t  *) REG_GPIO1_INT)                   /*  Port Interrupt Status */
#define pREG_GPIO1_DS                    ((__IO     uint16_t  *) REG_GPIO1_DS)                    /*  Port Drive Strength Select */

/*  =================================================================================
 *       (GPIO2)
 *  ================================================================================= */
#define pREG_GPIO2_CFG                   ((__IO     uint32_t  *) REG_GPIO2_CFG)                   /*  Port  Configuration */
#define pREG_GPIO2_OEN                   ((__IO     uint16_t  *) REG_GPIO2_OEN)                   /*  Port Output Enable */
#define pREG_GPIO2_PE                    ((__IO     uint16_t  *) REG_GPIO2_PE)                    /*  Port Output Pull-up/Pull-down Enable */
#define pREG_GPIO2_IEN                   ((__IO     uint16_t  *) REG_GPIO2_IEN)                   /*  Port  Input Path Enable */
#define pREG_GPIO2_IN                    ((__I __C  uint16_t  *) REG_GPIO2_IN)                    /*  Port  Registered Data Input */
#define pREG_GPIO2_OUT                   ((__IO     uint16_t  *) REG_GPIO2_OUT)                   /*  Port Data Output */
#define pREG_GPIO2_SET                   ((__O      uint16_t  *) REG_GPIO2_SET)                   /*  Port Data Out Set */
#define pREG_GPIO2_CLR                   ((__O      uint16_t  *) REG_GPIO2_CLR)                   /*  Port Data Out Clear */
#define pREG_GPIO2_TGL                   ((__O      uint16_t  *) REG_GPIO2_TGL)                   /*  Port Pin Toggle */
#define pREG_GPIO2_POL                   ((__IO     uint16_t  *) REG_GPIO2_POL)                   /*  Port Interrupt Polarity */
#define pREG_GPIO2_IENA                  ((__IO     uint16_t  *) REG_GPIO2_IENA)                  /*  Port Interrupt A Enable */
#define pREG_GPIO2_IENB                  ((__IO     uint16_t  *) REG_GPIO2_IENB)                  /*  Port Interrupt B Enable */
#define pREG_GPIO2_INT                   ((__IO     uint16_t  *) REG_GPIO2_INT)                   /*  Port Interrupt Status */
#define pREG_GPIO2_DS                    ((__IO     uint16_t  *) REG_GPIO2_DS)                    /*  Port Drive Strength Select */

/*  =================================================================================
 *       (GPIO3)
 *  ================================================================================= */
#define pREG_GPIO3_CFG                   ((__IO     uint32_t  *) REG_GPIO3_CFG)                   /*  Port  Configuration */
#define pREG_GPIO3_OEN                   ((__IO     uint16_t  *) REG_GPIO3_OEN)                   /*  Port Output Enable */
#define pREG_GPIO3_PE                    ((__IO     uint16_t  *) REG_GPIO3_PE)                    /*  Port Output Pull-up/Pull-down Enable */
#define pREG_GPIO3_IEN                   ((__IO     uint16_t  *) REG_GPIO3_IEN)                   /*  Port  Input Path Enable */
#define pREG_GPIO3_IN                    ((__I __C  uint16_t  *) REG_GPIO3_IN)                    /*  Port  Registered Data Input */
#define pREG_GPIO3_OUT                   ((__IO     uint16_t  *) REG_GPIO3_OUT)                   /*  Port Data Output */
#define pREG_GPIO3_SET                   ((__O      uint16_t  *) REG_GPIO3_SET)                   /*  Port Data Out Set */
#define pREG_GPIO3_CLR                   ((__O      uint16_t  *) REG_GPIO3_CLR)                   /*  Port Data Out Clear */
#define pREG_GPIO3_TGL                   ((__O      uint16_t  *) REG_GPIO3_TGL)                   /*  Port Pin Toggle */
#define pREG_GPIO3_POL                   ((__IO     uint16_t  *) REG_GPIO3_POL)                   /*  Port Interrupt Polarity */
#define pREG_GPIO3_IENA                  ((__IO     uint16_t  *) REG_GPIO3_IENA)                  /*  Port Interrupt A Enable */
#define pREG_GPIO3_IENB                  ((__IO     uint16_t  *) REG_GPIO3_IENB)                  /*  Port Interrupt B Enable */
#define pREG_GPIO3_INT                   ((__IO     uint16_t  *) REG_GPIO3_INT)                   /*  Port Interrupt Status */
#define pREG_GPIO3_DS                    ((__IO     uint16_t  *) REG_GPIO3_DS)                    /*  Port Drive Strength Select */

/*  =================================================================================
 *      Serial Port (SPORT0)
 *  ================================================================================= */
#define pREG_SPORT0_CTL_A                ((__IO     uint32_t  *) REG_SPORT0_CTL_A)                /*  Half SPORT 'A' Control Register */
#define pREG_SPORT0_DIV_A                ((__IO     uint32_t  *) REG_SPORT0_DIV_A)                /*  Half SPORT 'A' Divisor Register */
#define pREG_SPORT0_IEN_A                ((__IO     uint32_t  *) REG_SPORT0_IEN_A)                /*  Half SPORT A's Interrupt Enable register */
#define pREG_SPORT0_STAT_A               ((__IO     uint32_t  *) REG_SPORT0_STAT_A)               /*  Half SPORT 'A' Status register */
#define pREG_SPORT0_NUMTRAN_A            ((__IO     uint32_t  *) REG_SPORT0_NUMTRAN_A)            /*  Half SPORT A Number of transfers register */
#define pREG_SPORT0_CNVT_A               ((__IO     uint32_t  *) REG_SPORT0_CNVT_A)               /*  Half SPORT 'A' CNV width */
#define pREG_SPORT0_TX_A                 ((__O      uint32_t  *) REG_SPORT0_TX_A)                 /*  Half SPORT 'A' Tx Buffer Register */
#define pREG_SPORT0_RX_A                 ((__I __C  uint32_t  *) REG_SPORT0_RX_A)                 /*  Half SPORT 'A' Rx Buffer Register */
#define pREG_SPORT0_CTL_B                ((__IO     uint32_t  *) REG_SPORT0_CTL_B)                /*  Half SPORT 'B' Control Register */
#define pREG_SPORT0_DIV_B                ((__IO     uint32_t  *) REG_SPORT0_DIV_B)                /*  Half SPORT 'B' Divisor Register */
#define pREG_SPORT0_IEN_B                ((__IO     uint32_t  *) REG_SPORT0_IEN_B)                /*  Half SPORT B's Interrupt Enable register */
#define pREG_SPORT0_STAT_B               ((__IO     uint32_t  *) REG_SPORT0_STAT_B)               /*  Half SPORT 'B' Status register */
#define pREG_SPORT0_NUMTRAN_B            ((__IO     uint32_t  *) REG_SPORT0_NUMTRAN_B)            /*  Half SPORT B Number of transfers register */
#define pREG_SPORT0_CNVT_B               ((__IO     uint32_t  *) REG_SPORT0_CNVT_B)               /*  Half SPORT 'B' CNV width register */
#define pREG_SPORT0_TX_B                 ((__O      uint32_t  *) REG_SPORT0_TX_B)                 /*  Half SPORT 'B' Tx Buffer Register */
#define pREG_SPORT0_RX_B                 ((__I __C  uint32_t  *) REG_SPORT0_RX_B)                 /*  Half SPORT 'B' Rx Buffer Register */

/*  =================================================================================
 *      CRC Accelerator (CRC0)
 *  ================================================================================= */
#define pREG_CRC0_CTL                    ((__IO     uint32_t  *) REG_CRC0_CTL)                    /*  CRC Control */
#define pREG_CRC0_IPDATA                 ((__O      uint32_t  *) REG_CRC0_IPDATA)                 /*  Input Data Word */
#define pREG_CRC0_RESULT                 ((__IO     uint32_t  *) REG_CRC0_RESULT)                 /*  CRC Result */
#define pREG_CRC0_POLY                   ((__IO     uint32_t  *) REG_CRC0_POLY)                   /*  Programmable CRC Polynomial */
#define pREG_CRC0_IPBYTE                 ((__O      uint8_t   *) REG_CRC0_IPBYTE)                 /*  Input Data Byte */
#define pREG_CRC0_IPBITS0                ((__O      uint8_t   *) REG_CRC0_IPBITS0)                /*  Input Data Bits */
#define pREG_CRC0_IPBITS1                ((__O      uint8_t   *) REG_CRC0_IPBITS1)                /*  Input Data Bits */
#define pREG_CRC0_IPBITS2                ((__O      uint8_t   *) REG_CRC0_IPBITS2)                /*  Input Data Bits */
#define pREG_CRC0_IPBITS3                ((__O      uint8_t   *) REG_CRC0_IPBITS3)                /*  Input Data Bits */
#define pREG_CRC0_IPBITS4                ((__O      uint8_t   *) REG_CRC0_IPBITS4)                /*  Input Data Bits */
#define pREG_CRC0_IPBITS5                ((__O      uint8_t   *) REG_CRC0_IPBITS5)                /*  Input Data Bits */
#define pREG_CRC0_IPBITS6                ((__O      uint8_t   *) REG_CRC0_IPBITS6)                /*  Input Data Bits */
#define pREG_CRC0_IPBITS7                ((__O      uint8_t   *) REG_CRC0_IPBITS7)                /*  Input Data Bits */

/*  =================================================================================
 *      Random Number Generator (RNG0)
 *  ================================================================================= */
#define pREG_RNG0_CTL                    ((__IO     uint16_t  *) REG_RNG0_CTL)                    /*  RNG Control Register */
#define pREG_RNG0_LEN                    ((__IO     uint16_t  *) REG_RNG0_LEN)                    /*  RNG Sample Length Register */
#define pREG_RNG0_STAT                   ((__IO     uint16_t  *) REG_RNG0_STAT)                   /*  RNG Status Register */
#define pREG_RNG0_DATA                   ((__I __C  uint32_t  *) REG_RNG0_DATA)                   /*  RNG Data Register */
#define pREG_RNG0_OSCCNT                 ((__I __C  uint32_t  *) REG_RNG0_OSCCNT)                 /*  Oscillator Count */
#define pREG_RNG0_OSCDIFF0               ((__I __C  int8_t    *) REG_RNG0_OSCDIFF0)               /*  Oscillator Difference */
#define pREG_RNG0_OSCDIFF1               ((__I __C  int8_t    *) REG_RNG0_OSCDIFF1)               /*  Oscillator Difference */
#define pREG_RNG0_OSCDIFF2               ((__I __C  int8_t    *) REG_RNG0_OSCDIFF2)               /*  Oscillator Difference */
#define pREG_RNG0_OSCDIFF3               ((__I __C  int8_t    *) REG_RNG0_OSCDIFF3)               /*  Oscillator Difference */

/*  =================================================================================
 *      Register Map for the Crypto Block (CRYPT0)
 *  ================================================================================= */
#define pREG_CRYPT0_CFG                  ((__IO     uint32_t  *) REG_CRYPT0_CFG)                  /*  Configuration Register */
#define pREG_CRYPT0_DATALEN              ((__IO     uint32_t  *) REG_CRYPT0_DATALEN)              /*  Payload Data Length */
#define pREG_CRYPT0_PREFIXLEN            ((__IO     uint32_t  *) REG_CRYPT0_PREFIXLEN)            /*  Authentication Data Length */
#define pREG_CRYPT0_INTEN                ((__IO     uint32_t  *) REG_CRYPT0_INTEN)                /*  Interrupt Enable Register */
#define pREG_CRYPT0_STAT                 ((__IO     uint32_t  *) REG_CRYPT0_STAT)                 /*  Status Register */
#define pREG_CRYPT0_INBUF                ((__O      uint32_t  *) REG_CRYPT0_INBUF)                /*  Input Buffer */
#define pREG_CRYPT0_OUTBUF               ((__I __C  uint32_t  *) REG_CRYPT0_OUTBUF)               /*  Output Buffer */
#define pREG_CRYPT0_NONCE0               ((__IO     uint32_t  *) REG_CRYPT0_NONCE0)               /*  Nonce Bits [31:0] */
#define pREG_CRYPT0_NONCE1               ((__IO     uint32_t  *) REG_CRYPT0_NONCE1)               /*  Nonce Bits [63:32] */
#define pREG_CRYPT0_NONCE2               ((__IO     uint32_t  *) REG_CRYPT0_NONCE2)               /*  Nonce Bits [95:64] */
#define pREG_CRYPT0_NONCE3               ((__IO     uint32_t  *) REG_CRYPT0_NONCE3)               /*  Nonce Bits [127:96] */
#define pREG_CRYPT0_AESKEY0              ((__O      uint32_t  *) REG_CRYPT0_AESKEY0)              /*  AES Key Bits [31:0] */
#define pREG_CRYPT0_AESKEY1              ((__O      uint32_t  *) REG_CRYPT0_AESKEY1)              /*  AES Key Bits [63:32] */
#define pREG_CRYPT0_AESKEY2              ((__O      uint32_t  *) REG_CRYPT0_AESKEY2)              /*  AES Key Bits [95:64] */
#define pREG_CRYPT0_AESKEY3              ((__O      uint32_t  *) REG_CRYPT0_AESKEY3)              /*  AES Key Bits [127:96] */
#define pREG_CRYPT0_AESKEY4              ((__O      uint32_t  *) REG_CRYPT0_AESKEY4)              /*  AES Key Bits [159:128] */
#define pREG_CRYPT0_AESKEY5              ((__O      uint32_t  *) REG_CRYPT0_AESKEY5)              /*  AES Key Bits [191:160] */
#define pREG_CRYPT0_AESKEY6              ((__O      uint32_t  *) REG_CRYPT0_AESKEY6)              /*  AES Key Bits [223:192] */
#define pREG_CRYPT0_AESKEY7              ((__O      uint32_t  *) REG_CRYPT0_AESKEY7)              /*  AES Key Bits [255:224] */
#define pREG_CRYPT0_CNTRINIT             ((__IO     uint32_t  *) REG_CRYPT0_CNTRINIT)             /*  Counter Initialization Vector */
#define pREG_CRYPT0_SHAH0                ((__IO     uint32_t  *) REG_CRYPT0_SHAH0)                /*  SHA Bits [31:0] */
#define pREG_CRYPT0_SHAH1                ((__IO     uint32_t  *) REG_CRYPT0_SHAH1)                /*  SHA Bits [63:32] */
#define pREG_CRYPT0_SHAH2                ((__IO     uint32_t  *) REG_CRYPT0_SHAH2)                /*  SHA Bits [95:64] */
#define pREG_CRYPT0_SHAH3                ((__IO     uint32_t  *) REG_CRYPT0_SHAH3)                /*  SHA Bits [127:96] */
#define pREG_CRYPT0_SHAH4                ((__IO     uint32_t  *) REG_CRYPT0_SHAH4)                /*  SHA Bits [159:128] */
#define pREG_CRYPT0_SHAH5                ((__IO     uint32_t  *) REG_CRYPT0_SHAH5)                /*  SHA Bits [191:160] */
#define pREG_CRYPT0_SHAH6                ((__IO     uint32_t  *) REG_CRYPT0_SHAH6)                /*  SHA Bits [223:192] */
#define pREG_CRYPT0_SHAH7                ((__IO     uint32_t  *) REG_CRYPT0_SHAH7)                /*  SHA Bits [255:224] */
#define pREG_CRYPT0_SHA_LAST_WORD        ((__IO     uint32_t  *) REG_CRYPT0_SHA_LAST_WORD)        /*  SHA Last Word and Valid Bits Information */
#define pREG_CRYPT0_CCM_NUM_VALID_BYTES  ((__IO     uint32_t  *) REG_CRYPT0_CCM_NUM_VALID_BYTES)  /*  NUM_VALID_BYTES */
#define pREG_CRYPT0_PRKSTORCFG           ((__IO     uint32_t  *) REG_CRYPT0_PRKSTORCFG)           /*  PRKSTOR Configuration */
#define pREG_CRYPT0_KUW0                 ((__O      uint32_t  *) REG_CRYPT0_KUW0)                 /*  Key Wrap Unwrap Register 0 */
#define pREG_CRYPT0_KUW1                 ((__O      uint32_t  *) REG_CRYPT0_KUW1)                 /*  Key Wrap Unwrap Register 1 */
#define pREG_CRYPT0_KUW2                 ((__O      uint32_t  *) REG_CRYPT0_KUW2)                 /*  Key Wrap Unwrap Register 2 */
#define pREG_CRYPT0_KUW3                 ((__O      uint32_t  *) REG_CRYPT0_KUW3)                 /*  Key Wrap Unwrap Register 3 */
#define pREG_CRYPT0_KUW4                 ((__O      uint32_t  *) REG_CRYPT0_KUW4)                 /*  Key Wrap Unwrap Register 4 */
#define pREG_CRYPT0_KUW5                 ((__O      uint32_t  *) REG_CRYPT0_KUW5)                 /*  Key Wrap Unwrap Register 5 */
#define pREG_CRYPT0_KUW6                 ((__O      uint32_t  *) REG_CRYPT0_KUW6)                 /*  Key Wrap Unwrap Register 6 */
#define pREG_CRYPT0_KUW7                 ((__O      uint32_t  *) REG_CRYPT0_KUW7)                 /*  Key Wrap Unwrap Register 7 */
#define pREG_CRYPT0_KUW8                 ((__O      uint32_t  *) REG_CRYPT0_KUW8)                 /*  Key Wrap Unwrap Register 8 */
#define pREG_CRYPT0_KUW9                 ((__O      uint32_t  *) REG_CRYPT0_KUW9)                 /*  Key Wrap Unwrap Register 9 */
#define pREG_CRYPT0_KUW10                ((__O      uint32_t  *) REG_CRYPT0_KUW10)                /*  Key Wrap Unwrap Register 10 */
#define pREG_CRYPT0_KUW11                ((__O      uint32_t  *) REG_CRYPT0_KUW11)                /*  Key Wrap Unwrap Register 11 */
#define pREG_CRYPT0_KUW12                ((__O      uint32_t  *) REG_CRYPT0_KUW12)                /*  Key Wrap Unwrap Register 12 */
#define pREG_CRYPT0_KUW13                ((__O      uint32_t  *) REG_CRYPT0_KUW13)                /*  Key Wrap Unwrap Register 13 */
#define pREG_CRYPT0_KUW14                ((__O      uint32_t  *) REG_CRYPT0_KUW14)                /*  Key Wrap Unwrap Register 14 */
#define pREG_CRYPT0_KUW15                ((__O      uint32_t  *) REG_CRYPT0_KUW15)                /*  Key Wrap Unwrap Register 15 */
#define pREG_CRYPT0_KUWVALSTR1           ((__O      uint32_t  *) REG_CRYPT0_KUWVALSTR1)           /*  Key Wrap Unwrap Validation String [63:32] */
#define pREG_CRYPT0_KUWVALSTR2           ((__O      uint32_t  *) REG_CRYPT0_KUWVALSTR2)           /*  Key Wrap Unwrap Validation String [31:0] */

/*  =================================================================================
 *      Power Management  (PMG0)
 *  ================================================================================= */
#define pREG_PMG0_IEN                    ((__IO     uint32_t  *) REG_PMG0_IEN)                    /*  Power Supply Monitor Interrupt Enable */
#define pREG_PMG0_PSM_STAT               ((__IO     uint32_t  *) REG_PMG0_PSM_STAT)               /*  Power Supply Monitor Status */
#define pREG_PMG0_PWRMOD                 ((__IO     uint32_t  *) REG_PMG0_PWRMOD)                 /*  Power Mode Register */
#define pREG_PMG0_PWRKEY                 ((__O      uint32_t  *) REG_PMG0_PWRKEY)                 /*  Key Protection for PWRMOD and  SRAMRET */
#define pREG_PMG0_SHDN_STAT              ((__I __C  uint32_t  *) REG_PMG0_SHDN_STAT)              /*  Shutdown Status Register */
#define pREG_PMG0_SRAMRET                ((__IO     uint32_t  *) REG_PMG0_SRAMRET)                /*  Control for Retention SRAM in Hibernate Mode */
#define pREG_PMG0_TRIM                   ((__IO     uint32_t  *) REG_PMG0_TRIM)                   /*  Trimming Bits */
#define pREG_PMG0_RST_STAT               ((__IO     uint32_t  *) REG_PMG0_RST_STAT)               /*  Reset Status */
#define pREG_PMG0_CTL1                   ((__IO     uint32_t  *) REG_PMG0_CTL1)                   /*  HPBUCK Control */

/*  =================================================================================
 *      External interrupt configuration (XINT0)
 *  ================================================================================= */
#define pREG_XINT0_CFG0                  ((__IO     uint32_t  *) REG_XINT0_CFG0)                  /*  External Interrupt configuration */
#define pREG_XINT0_EXT_STAT              ((__I __C  uint32_t  *) REG_XINT0_EXT_STAT)              /*  External Wakeup Interrupt Status register */
#define pREG_XINT0_CLR                   ((__IO     uint32_t  *) REG_XINT0_CLR)                   /*  External Interrupt clear */
#define pREG_XINT0_NMICLR                ((__IO     uint32_t  *) REG_XINT0_NMICLR)                /*  Non-maskable interrupt clear */

/*  =================================================================================
 *      Clocking  (CLKG0_OSC)
 *  ================================================================================= */
#define pREG_CLKG0_OSC_KEY               ((__O      uint32_t  *) REG_CLKG0_OSC_KEY)               /*  Key Protection for OSCCTRL */
#define pREG_CLKG0_OSC_CTL               ((__IO     uint32_t  *) REG_CLKG0_OSC_CTL)               /*  Oscillator Control */

/*  =================================================================================
 *      Power Management  (PMG0_TST)
 *  ================================================================================= */
#define pREG_PMG0_TST_SRAM_CTL           ((__IO     uint32_t  *) REG_PMG0_TST_SRAM_CTL)           /*  Control for SRAM Parity and Instruction SRAM */
#define pREG_PMG0_TST_SRAM_INITSTAT      ((__I __C  uint32_t  *) REG_PMG0_TST_SRAM_INITSTAT)      /*  Initialization Status Register */
#define pREG_PMG0_TST_CLR_LATCH_GPIOS    ((__O      uint16_t  *) REG_PMG0_TST_CLR_LATCH_GPIOS)    /*  Clear GPIO After Shutdown Mode */
#define pREG_PMG0_TST_SCRPAD_IMG         ((__IO     uint32_t  *) REG_PMG0_TST_SCRPAD_IMG)         /*  Scratch Pad Image */
#define pREG_PMG0_TST_SCRPAD_3V_RD       ((__I __C  uint32_t  *) REG_PMG0_TST_SCRPAD_3V_RD)       /*  Scratch Pad Saved in Battery Domain */
#define pREG_PMG0_TST_FAST_SHT_WAKEUP    ((__IO     uint32_t  *) REG_PMG0_TST_FAST_SHT_WAKEUP)    /*  Fast Shutdown Wake-up Enable */

/*  =================================================================================
 *      Clocking  (CLKG0_CLK)
 *  ================================================================================= */
#define pREG_CLKG0_CLK_CTL0              ((__IO     uint32_t  *) REG_CLKG0_CLK_CTL0)              /*  Misc Clock Settings */
#define pREG_CLKG0_CLK_CTL1              ((__IO     uint32_t  *) REG_CLKG0_CLK_CTL1)              /*  Clock Dividers */
#define pREG_CLKG0_CLK_CTL2              ((__IO     uint32_t  *) REG_CLKG0_CLK_CTL2)              /*  HF Oscillator Divided Clock Select */
#define pREG_CLKG0_CLK_CTL3              ((__IO     uint32_t  *) REG_CLKG0_CLK_CTL3)              /*  System PLL */
#define pREG_CLKG0_CLK_CTL5              ((__IO     uint32_t  *) REG_CLKG0_CLK_CTL5)              /*  User Clock Gating Control */
#define pREG_CLKG0_CLK_STAT0             ((__IO     uint32_t  *) REG_CLKG0_CLK_STAT0)             /*  Clocking Status */

/*  =================================================================================
 *      Bus matrix (BUSM0)
 *  ================================================================================= */
#define pREG_BUSM0_ARBIT0                ((__IO     uint32_t  *) REG_BUSM0_ARBIT0)                /*  Arbitration Priority Configuration for FLASH and SRAM0 */
#define pREG_BUSM0_ARBIT1                ((__IO     uint32_t  *) REG_BUSM0_ARBIT1)                /*  Arbitration Priority Configuration for SRAM1 and SIP */
#define pREG_BUSM0_ARBIT2                ((__IO     uint32_t  *) REG_BUSM0_ARBIT2)                /*  Arbitration Priority Configuration for APB32 and APB16 */
#define pREG_BUSM0_ARBIT3                ((__IO     uint32_t  *) REG_BUSM0_ARBIT3)                /*  Arbitration Priority Configuration for APB16 priority for core and for DMA1 */
#define pREG_BUSM0_ARBIT4                ((__IO     uint32_t  *) REG_BUSM0_ARBIT4)                /*  Arbitration Priority Configuration for SRAM1 and SIP */

/*  =================================================================================
 *      Parallel Test Interface (PTI0)
 *  ================================================================================= */
#define pREG_PTI0_RST_ISR_STARTADDR      ((__IO     uint32_t  *) REG_PTI0_RST_ISR_STARTADDR)      /*  Reset ISR Start Address */
#define pREG_PTI0_RST_STACK_PTR          ((__IO     uint32_t  *) REG_PTI0_RST_STACK_PTR)          /*  Reset Stack Pointer */
#define pREG_PTI0_CTL                    ((__IO     uint32_t  *) REG_PTI0_CTL)                    /*  Parallel Test Interface Control Register */

/*  =================================================================================
 *      Cortex-M3 Interrupt Controller (NVIC0)
 *  ================================================================================= */
#define pREG_NVIC0_INTNUM                ((__IO     uint32_t  *) REG_NVIC0_INTNUM)                /*  Interrupt Control Type */
#define pREG_NVIC0_STKSTA                ((__IO     uint32_t  *) REG_NVIC0_STKSTA)                /*  Systick Control and Status */
#define pREG_NVIC0_STKLD                 ((__IO     uint32_t  *) REG_NVIC0_STKLD)                 /*  Systick Reload Value */
#define pREG_NVIC0_STKVAL                ((__IO     uint32_t  *) REG_NVIC0_STKVAL)                /*  Systick Current Value */
#define pREG_NVIC0_STKCAL                ((__IO     uint32_t  *) REG_NVIC0_STKCAL)                /*  Systick Calibration Value */
#define pREG_NVIC0_INTSETE0              ((__IO     uint32_t  *) REG_NVIC0_INTSETE0)              /*  IRQ0..31 Set_Enable */
#define pREG_NVIC0_INTSETE1              ((__IO     uint32_t  *) REG_NVIC0_INTSETE1)              /*  IRQ32..63 Set_Enable */
#define pREG_NVIC0_INTCLRE0              ((__IO     uint32_t  *) REG_NVIC0_INTCLRE0)              /*  IRQ0..31 Clear_Enable */
#define pREG_NVIC0_INTCLRE1              ((__IO     uint32_t  *) REG_NVIC0_INTCLRE1)              /*  IRQ32..63 Clear_Enable */
#define pREG_NVIC0_INTSETP0              ((__IO     uint32_t  *) REG_NVIC0_INTSETP0)              /*  IRQ0..31 Set_Pending */
#define pREG_NVIC0_INTSETP1              ((__IO     uint32_t  *) REG_NVIC0_INTSETP1)              /*  IRQ32..63 Set_Pending */
#define pREG_NVIC0_INTCLRP0              ((__IO     uint32_t  *) REG_NVIC0_INTCLRP0)              /*  IRQ0..31 Clear_Pending */
#define pREG_NVIC0_INTCLRP1              ((__IO     uint32_t  *) REG_NVIC0_INTCLRP1)              /*  IRQ32..63 Clear_Pending */
#define pREG_NVIC0_INTACT0               ((__IO     uint32_t  *) REG_NVIC0_INTACT0)               /*  IRQ0..31 Active Bit */
#define pREG_NVIC0_INTACT1               ((__IO     uint32_t  *) REG_NVIC0_INTACT1)               /*  IRQ32..63 Active Bit */
#define pREG_NVIC0_INTPRI0               ((__IO     uint32_t  *) REG_NVIC0_INTPRI0)               /*  IRQ0..3 Priority */
#define pREG_NVIC0_INTPRI1               ((__IO     uint32_t  *) REG_NVIC0_INTPRI1)               /*  IRQ4..7 Priority */
#define pREG_NVIC0_INTPRI2               ((__IO     uint32_t  *) REG_NVIC0_INTPRI2)               /*  IRQ8..11 Priority */
#define pREG_NVIC0_INTPRI3               ((__IO     uint32_t  *) REG_NVIC0_INTPRI3)               /*  IRQ12..15 Priority */
#define pREG_NVIC0_INTPRI4               ((__IO     uint32_t  *) REG_NVIC0_INTPRI4)               /*  IRQ16..19 Priority */
#define pREG_NVIC0_INTPRI5               ((__IO     uint32_t  *) REG_NVIC0_INTPRI5)               /*  IRQ20..23 Priority */
#define pREG_NVIC0_INTPRI6               ((__IO     uint32_t  *) REG_NVIC0_INTPRI6)               /*  IRQ24..27 Priority */
#define pREG_NVIC0_INTPRI7               ((__IO     uint32_t  *) REG_NVIC0_INTPRI7)               /*  IRQ28..31 Priority */
#define pREG_NVIC0_INTPRI8               ((__IO     uint32_t  *) REG_NVIC0_INTPRI8)               /*  IRQ32..35 Priority */
#define pREG_NVIC0_INTPRI9               ((__IO     uint32_t  *) REG_NVIC0_INTPRI9)               /*  IRQ36..39 Priority */
#define pREG_NVIC0_INTPRI10              ((__IO     uint32_t  *) REG_NVIC0_INTPRI10)              /*  IRQ40..43 Priority */
#define pREG_NVIC0_INTCPID               ((__IO     uint32_t  *) REG_NVIC0_INTCPID)               /*  CPUID Base */
#define pREG_NVIC0_INTSTA                ((__IO     uint32_t  *) REG_NVIC0_INTSTA)                /*  Interrupt Control State */
#define pREG_NVIC0_INTVEC                ((__IO     uint32_t  *) REG_NVIC0_INTVEC)                /*  Vector Table Offset */
#define pREG_NVIC0_INTAIRC               ((__IO     uint32_t  *) REG_NVIC0_INTAIRC)               /*  Application Interrupt/Reset Control */
#define pREG_NVIC0_INTCON0               ((__IO     uint16_t  *) REG_NVIC0_INTCON0)               /*  System Control */
#define pREG_NVIC0_INTCON1               ((__IO     uint32_t  *) REG_NVIC0_INTCON1)               /*  Configuration Control */
#define pREG_NVIC0_INTSHPRIO0            ((__IO     uint32_t  *) REG_NVIC0_INTSHPRIO0)            /*  System Handlers 4-7 Priority */
#define pREG_NVIC0_INTSHPRIO1            ((__IO     uint32_t  *) REG_NVIC0_INTSHPRIO1)            /*  System Handlers 8-11 Priority */
#define pREG_NVIC0_INTSHPRIO3            ((__IO     uint32_t  *) REG_NVIC0_INTSHPRIO3)            /*  System Handlers 12-15 Priority */
#define pREG_NVIC0_INTSHCSR              ((__IO     uint32_t  *) REG_NVIC0_INTSHCSR)              /*  System Handler Control and State */
#define pREG_NVIC0_INTCFSR               ((__IO     uint32_t  *) REG_NVIC0_INTCFSR)               /*  Configurable Fault Status */
#define pREG_NVIC0_INTHFSR               ((__IO     uint32_t  *) REG_NVIC0_INTHFSR)               /*  Hard Fault Status */
#define pREG_NVIC0_INTDFSR               ((__IO     uint32_t  *) REG_NVIC0_INTDFSR)               /*  Debug Fault Status */
#define pREG_NVIC0_INTMMAR               ((__IO     uint32_t  *) REG_NVIC0_INTMMAR)               /*  Mem Manage Address */
#define pREG_NVIC0_INTBFAR               ((__IO     uint32_t  *) REG_NVIC0_INTBFAR)               /*  Bus Fault Address */
#define pREG_NVIC0_INTAFSR               ((__IO     uint32_t  *) REG_NVIC0_INTAFSR)               /*  Auxiliary Fault Status */
#define pREG_NVIC0_INTPFR0               ((__IO     uint32_t  *) REG_NVIC0_INTPFR0)               /*  Processor Feature Register 0 */
#define pREG_NVIC0_INTPFR1               ((__IO     uint32_t  *) REG_NVIC0_INTPFR1)               /*  Processor Feature Register 1 */
#define pREG_NVIC0_INTDFR0               ((__IO     uint32_t  *) REG_NVIC0_INTDFR0)               /*  Debug Feature Register 0 */
#define pREG_NVIC0_INTAFR0               ((__IO     uint32_t  *) REG_NVIC0_INTAFR0)               /*  Auxiliary Feature Register 0 */
#define pREG_NVIC0_INTMMFR0              ((__IO     uint32_t  *) REG_NVIC0_INTMMFR0)              /*  Memory Model Feature Register 0 */
#define pREG_NVIC0_INTMMFR1              ((__IO     uint32_t  *) REG_NVIC0_INTMMFR1)              /*  Memory Model Feature Register 1 */
#define pREG_NVIC0_INTMMFR2              ((__IO     uint32_t  *) REG_NVIC0_INTMMFR2)              /*  Memory Model Feature Register 2 */
#define pREG_NVIC0_INTMMFR3              ((__IO     uint32_t  *) REG_NVIC0_INTMMFR3)              /*  Memory Model Feature Register 3 */
#define pREG_NVIC0_INTISAR0              ((__IO     uint32_t  *) REG_NVIC0_INTISAR0)              /*  ISA Feature Register 0 */
#define pREG_NVIC0_INTISAR1              ((__IO     uint32_t  *) REG_NVIC0_INTISAR1)              /*  ISA Feature Register 1 */
#define pREG_NVIC0_INTISAR2              ((__IO     uint32_t  *) REG_NVIC0_INTISAR2)              /*  ISA Feature Register 2 */
#define pREG_NVIC0_INTISAR3              ((__IO     uint32_t  *) REG_NVIC0_INTISAR3)              /*  ISA Feature Register 3 */
#define pREG_NVIC0_INTISAR4              ((__IO     uint32_t  *) REG_NVIC0_INTISAR4)              /*  ISA Feature Register 4 */
#define pREG_NVIC0_INTTRGI               ((__IO     uint32_t  *) REG_NVIC0_INTTRGI)               /*  Software Trigger Interrupt Register */
#define pREG_NVIC0_INTPID4               ((__IO     uint32_t  *) REG_NVIC0_INTPID4)               /*  Peripheral Identification Register 4 */
#define pREG_NVIC0_INTPID5               ((__IO     uint32_t  *) REG_NVIC0_INTPID5)               /*  Peripheral Identification Register 5 */
#define pREG_NVIC0_INTPID6               ((__IO     uint32_t  *) REG_NVIC0_INTPID6)               /*  Peripheral Identification Register 6 */
#define pREG_NVIC0_INTPID7               ((__IO     uint32_t  *) REG_NVIC0_INTPID7)               /*  Peripheral Identification Register 7 */
#define pREG_NVIC0_INTPID0               ((__IO     uint32_t  *) REG_NVIC0_INTPID0)               /*  Peripheral Identification Bits7:0 */
#define pREG_NVIC0_INTPID1               ((__IO     uint32_t  *) REG_NVIC0_INTPID1)               /*  Peripheral Identification Bits15:8 */
#define pREG_NVIC0_INTPID2               ((__IO     uint32_t  *) REG_NVIC0_INTPID2)               /*  Peripheral Identification Bits16:23 */
#define pREG_NVIC0_INTPID3               ((__IO     uint32_t  *) REG_NVIC0_INTPID3)               /*  Peripheral Identification Bits24:31 */
#define pREG_NVIC0_INTCID0               ((__IO     uint32_t  *) REG_NVIC0_INTCID0)               /*  Component Identification Bits7:0 */
#define pREG_NVIC0_INTCID1               ((__IO     uint32_t  *) REG_NVIC0_INTCID1)               /*  Component Identification Bits15:8 */
#define pREG_NVIC0_INTCID2               ((__IO     uint32_t  *) REG_NVIC0_INTCID2)               /*  Component Identification Bits16:23 */
#define pREG_NVIC0_INTCID3               ((__IO     uint32_t  *) REG_NVIC0_INTCID3)               /*  Component Identification Bits24:31 */

#if defined (_MISRA_RULES)
#pragma diag(pop)
#endif /* _MISRA_RULES */


#endif

