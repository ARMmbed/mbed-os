/**
 ******************************************************************************
 * @file adc_sar.h
 * @internal
 * @author ON Semiconductor
 * $Rev: 3426 $
 * $Date: 2015-06-15 16:46:35 +0530 (Mon, 15 Jun 2015) $
 * @brief Definitions and API for the SAR ADC driver.
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
 * </p>
 */

#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

#include "adc_sar_map.h"

#if DEVICE_ANALOGIN

#ifdef __cplusplus
extern "C" {
#endif

/* ADC register bits */
#define ADC_CONTROL_MODE_BIT_POS        0
#define ADC_CONTROL_MEASTYPE_BIT_POS    3
#define ADC_CONTROL_INPUTSCALE_BIT_POS  4
#define ADC_CONTROL_MEAS_CH_BIT_POS     8
#define ADC_CONTROL_REF_CH_BIT_POS      12
#define ADC_PRESCALE_VAL_BIT_POS        0
#define ADC_PRESCALE_EN_BIT_POS         8
#define ADC_DELAY_SAMPLE_RATE_BIT_POS   0
#define ADC_DELAY_WARMUP_BIT_POS        16
#define ADC_DELAY_SAMPLE_TIME_BIT_POS   24

typedef enum {
    ADC_CHANNEL0 = 0,
    ADC_CHANNEL1,
    ADC_CHANNEL2,
    ADC_CHANNEL3,
    ADC_TEMPSENSR = 6,
    ADC_BATTERY
} Type_RefCh_ConvCh;

typedef enum {
    ADC_RELATIVE_MEAS = 0,
    ADC_ABSOLUTE_MEAS
} Type_Meastype;

typedef enum {
    ADC_SINGLE_SAMPLE = 0,
    ADC_CONTINUOUS_SAMPLE
} Type_Mode;

typedef enum {
    ADC_INT_DISABLE = 0,
    ADC_INT_ENABLE
} Type_Intrpt;

typedef enum {
    ADC_IP_SCALE_1_0 = 0,
    ADC_IP_SCALE_0_6923,
    ADC_IP_SCALE_0_5294,
    ADC_IP_SCALE_0_4286,
    ADC_IP_SCALE_0_3600,
    ADC_IP_SCALE_0_3103,
    ADC_IP_SCALE_0_2728,
    ADC_IP_SCALE_0_2432
} Ip_Scale_Type;

void fAdcHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* DEVICE_ANALOGIN */

#endif /* ADC_DRIVER_H_ */
