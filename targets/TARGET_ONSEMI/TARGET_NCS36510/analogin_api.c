/**
 ******************************************************************************
 * @file adc_sar.c
 * @brief Implementation of a SAR ADC driver
 * @internal
 * @author ON Semiconductor
 * $Rev:
 * $Date:
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
 *
 */

#include "device.h"
#include "analogin_api.h"
#include "PeripheralPins.h"
#include "mbed_assert.h"
#include "clock.h"
#include "adc_sar.h"

#if DEVICE_ANALOGIN

/**
 * \defgroup hal_analogin Analogin hal functions
 * @{
 */

/** Initialize the analogin peripheral
 *
 * Configures the pin used by analogin.
 * @param obj The analogin object to initialize
 * @param pin The analogin pin name
 */
void analogin_init(analogin_t *obj, PinName pin)
{
    CLOCK_ENABLE(CLOCK_ADC);
    ADCName adc;
    uint8_t adc_pin = 0;

    adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);
    MBED_ASSERT(adc != (ADCName)NC);

    obj->adcReg = (AdcReg_pt)adc;
    obj->pin = pin;
    obj->pinFlag = 1;
    obj->ADC_Offset_Value = TRIMREG->ADC_OFFSET_TRIM;
    
    switch (pin) {
        case A0:
            adc_pin=0;
            break;
        case A1:
            adc_pin = 1;
            break;
        case A2:
            adc_pin = 2;
            break;
        case A3:
            adc_pin = 3;
            break;
        default:
            break;
    }

    /* If no config parameters are passed on; assume default value */
    if (obj->adcConf == Null) {
        /* Single sample, absolute conversion, scale = 1 */
        obj->adcReg->CONTROL.WORD = ((0 << ADC_CONTROL_MODE_BIT_POS) |
                                     (1 << ADC_CONTROL_MEASTYPE_BIT_POS) |
                                     (6 << ADC_CONTROL_INPUTSCALE_BIT_POS) |
                                     (((uint8_t)adc_pin) << ADC_CONTROL_MEAS_CH_BIT_POS));

        /* Prescaler enabled; set to 7 */
        obj->adcReg->PRESCALE.WORD = ((True << ADC_PRESCALE_EN_BIT_POS) |
                                      (7 << ADC_PRESCALE_VAL_BIT_POS)); /* ADC clock = 32MHz/(Prescale + 1) */

        obj->adcReg->DELAY.WORD = ((0x34 << ADC_DELAY_SAMPLE_RATE_BIT_POS) | /** 25 uS Sets the sample rate in units of PCLKperiod *  (Prescale + 1). */
                                   (0x05 << ADC_DELAY_WARMUP_BIT_POS) | /** 12.5 uS Sets the measure time in units of PCLKperiod * (Prescale + 1). */
                                   (0x1A << ADC_DELAY_SAMPLE_TIME_BIT_POS)); /** 2.5 uS Sets the warm-up time in units of PCLKperiod * (Prescale + 1). */

        obj->adcReg->IR = 0; /** No interrupt generated */
    } else {
        obj->adcConf->convCh = adc_pin;

        /* ADC register settings */
        if((obj->adcConf->measurementType) == ADC_RELATIVE_MEAS) {
            obj->adcReg->CONTROL.WORD = ((obj->adcConf->mode << ADC_CONTROL_MODE_BIT_POS) |
                                         (obj->adcConf->measurementType << ADC_CONTROL_MEASTYPE_BIT_POS) |
                                         (obj->adcConf->inputScale << ADC_CONTROL_INPUTSCALE_BIT_POS) |
                                         (obj->adcConf->convCh << ADC_CONTROL_MEAS_CH_BIT_POS) |
                                         (obj->adcConf->referenceCh << ADC_CONTROL_REF_CH_BIT_POS));
        } else {
            obj->adcReg->CONTROL.WORD = ((obj->adcConf->mode << ADC_CONTROL_MODE_BIT_POS) |
                                         (obj->adcConf->measurementType << ADC_CONTROL_MEASTYPE_BIT_POS) |
                                         (obj->adcConf->inputScale << ADC_CONTROL_INPUTSCALE_BIT_POS) |
                                         (obj->adcConf->convCh << ADC_CONTROL_MEAS_CH_BIT_POS));
        }

        obj->adcReg->PRESCALE.WORD = ((True << ADC_PRESCALE_EN_BIT_POS) |
                                      (obj->adcConf->PrescaleVal << ADC_PRESCALE_VAL_BIT_POS)); /* ADC clock = 32MHz/(Prescale + 1) */

        obj->adcReg->DELAY.WORD = ((obj->adcConf->samplingRate << ADC_DELAY_SAMPLE_RATE_BIT_POS) | /** 25 uS Sets the sample rate in units of PCLKperiod *  (Prescale + 1). */
                                   (obj->adcConf->WarmUpTime << ADC_DELAY_WARMUP_BIT_POS) | /** 12.5 uS Sets the measure time in units of PCLKperiod * (Prescale + 1). */
                                   (obj->adcConf->samplingTime << ADC_DELAY_SAMPLE_TIME_BIT_POS)); /** 2.5 uS Sets the warm-up time in units of PCLKperiod * (Prescale + 1). */

        obj->adcReg->IR = obj->adcConf->interruptConfig; /**  Interrupt setting */
    }

    /* Enable interrupt */
    NVIC_ClearPendingIRQ(Adc_IRQn);
    NVIC_EnableIRQ(Adc_IRQn);
}

/** Read the input voltage, represented as a float in the range [0.0, 1.0]
 *
 * @param obj The analogin object
 * @return A floating value representing the current input voltage
 */
float analogin_read(analogin_t *obj)
{
    float retVal = 0.0;
    uint16_t value = analogin_read_u16(obj);
    retVal = (float)value * (1.0f / (float)0x03FF);
    return(retVal);
}

/** Read the value from analogin pin, represented as an unsigned 16bit value
 *
 * @param obj The analogin object
 * @return An unsigned 16bit value representing the current input voltage
 */
uint16_t analogin_read_u16(analogin_t *obj)
{
    uint16_t adcData = 0;
    uint8_t adc_pin = 0;

    CLOCK_ENABLE(CLOCK_ADC);

    if (obj->pinFlag) {
        switch (obj->pin) {
            case A0:
                adc_pin=0;
                break;
            case A1:
                adc_pin = 1;
                break;
            case A2:
                adc_pin = 2;
                break;
            case A3:
                adc_pin = 3;
                break;
            default:
                break;
        }

        /* Re initialize the pin configured for ADC read */
        obj->adcReg->CONTROL.BITS.CONV_CH = adc_pin;
    }

    obj->adcReg->CONTROL.BITS.START_CONV=1;    /* Start The Conversion */

    while((uint32_t)(obj->adcReg->STATUS)!=(uint32_t)1) {
    }
    adcData =(uint16_t)(obj->adcReg->DATA);
    
    /* Offset the ADC data with trim value */
    if (obj->ADC_Offset_Value != 0xFFFFFFFF) {
      
        if(adcData >= obj->ADC_Offset_Value) {
            adcData -= obj->ADC_Offset_Value;
        }
    }

    return(adcData);
}

#endif // DEVICE_ANALOGIN

