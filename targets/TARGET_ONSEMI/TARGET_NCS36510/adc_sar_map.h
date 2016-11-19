/**
 ******************************************************************************
 * @file adc_sar_map.h
 * @brief ADC HW register map
 * @internal
 * @author ON Semiconductor
 * $Rev: 3422 $
 * $Date: 2015-06-09 11:01:43 +0530 (Tue, 09 Jun 2015) $
 ******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup adc_sar
 *
 * @details
 * <p>
 * ADC HW register map description
 * </p>
 *
 */

#ifndef ADC_MAP_H_
#define ADC_MAP_H_


#include "architecture.h"

/* ADC Control HW Structure Overlay */
typedef struct {
    union {
        struct {
            __IO uint32_t MODE :1;             /** 1= Continuous Conversion 0= Single Shot  */
            __IO uint32_t START_CONV :1;       /** 1= Start Conversion 0= No effect*/
            __IO uint32_t ABORT :1;            /** 1= Aborts the Continuous Conversion mode 0=No effect */
            __IO uint32_t MEASUREMENT_TYPE :1; /** 1= Absolute 0= Differential */
            __IO uint32_t INPUT_SCALE :3;      /** 000 – 1.0     001 – 0.6923 010 – 0.5294 011 – 0.4286 100 – 0.3600 101 – 0.3103  110 – 0.2728 111 – 0.2432 */
            __I  uint32_t BIT7:1;              /** NA Always read backs 0*/
            __IO uint32_t CONV_CH :3;          /** Selects 1 or 8 channels to do a conversion on. 000 – A[0] 000 – A[1] 010 – A[2] 011 – A[3] 100 – N/A 101 – N/A 110 – Temperature sensor      111 – Battery  */
            __I uint32_t  NA :1;               /** NA */
            __IO uint32_t REF_CH :3;           /** Selects 1 to 8 channels for reference channel  000 – A[0]  000 – A[1]   010 – A[2]  011 – A[3]  100 – N/A  101 – N/A 110 – Temperature sensor  111 – Battery */
        } BITS;
        __IO uint32_t WORD;
    } CONTROL;
    union {
        struct {
            __IO uint32_t SAMPLE_RATE :16; /** Sets the sample rate in units of PCLKperiod * (Prescale + 1).  */
            __IO uint32_t WARMUP_TIME :8;  /** Sets the warm-up time in units of PCLKperiod * (Prescale + 1)  */
            __IO uint32_t SAMPLE_TIME :8;  /** Sample Time. Sets the measure time in units of PCLKperiod * (Prescale + 1).*/
        } BITS;
        __IO uint32_t WORD;
    } DELAY;
    __I uint32_t DATA;
    __IO uint32_t IR;
    union {
        struct {
            __IO uint32_t PRESC_VAL  :8;   /**Set the pre-scalar value. The SAR ADC nominally runs at 4MHz, so this value should be programmed to 32 Mhz/4mhz -1=7 */
            __IO uint32_t PRESC_EN   :1;   /** 1= enables PreScalar 0= Disable Prescalar */
//            __IO uint32_t PHASE_CTRL :1; /** 1 = Phase 2 is delayed two 32MHz clock from phase 1. 0= Phase 2 is delayed one 32MHz clock from phase 1. */
        } BITS;
        __IO uint32_t WORD;
    } PRESCALE;
    __I uint32_t STATUS;
} ADCReg_t,*AdcReg_pt;

#endif /* ADC_MAP_H_ */
