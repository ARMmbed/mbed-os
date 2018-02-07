/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed_assert.h"
#include "analogin_api.h"

#if DEVICE_ANALOGIN

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralNames.h"
#include "fsl_adc.h"
#include "fsl_power.h"
#include "PeripheralPins.h"

/* Array of ADC peripheral base address. */
static ADC_Type *const adc_addrs[] = ADC_BASE_PTRS;
extern void ADC_ClockPower_Configuration(void);

#define MAX_FADC 6000000

void analogin_init(analogin_t *obj, PinName pin)
{
    obj->adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);
    MBED_ASSERT(obj->adc != (ADCName)NC);

    uint32_t instance = obj->adc >> ADC_INSTANCE_SHIFT;
    adc_config_t adc_config;
    uint32_t reg;
    uint32_t pin_number = pin & 0x1F;
    uint8_t port_number = pin / 32;

    /* Clear the DIGIMODE bit */
    reg = IOCON->PIO[port_number][pin_number] & ~IOCON_PIO_DIGIMODE_MASK;
    IOCON->PIO[port_number][pin_number] = reg;

    ADC_ClockPower_Configuration();

    /* Calibration after power up. */
    if (!(ADC_DoSelfCalibration(adc_addrs[instance]))) {
        /* Calibration failed */
        return;
    }

    ADC_GetDefaultConfig(&adc_config);
    adc_config.clockDividerNumber = 1;

    ADC_Init(adc_addrs[instance], &adc_config);
    pinmap_pinout(pin, PinMap_ADC);
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    uint32_t instance = obj->adc >> ADC_INSTANCE_SHIFT;
    uint32_t channel = obj->adc & 0xF;
    adc_conv_seq_config_t adcConvSeqConfigStruct;
    adc_result_info_t adcResultInfoStruct;

    adcConvSeqConfigStruct.channelMask = (1U << channel);
    adcConvSeqConfigStruct.triggerMask = 0U;
    adcConvSeqConfigStruct.triggerPolarity = kADC_TriggerPolarityNegativeEdge;
    adcConvSeqConfigStruct.enableSingleStep = false;
    adcConvSeqConfigStruct.enableSyncBypass = false;
    adcConvSeqConfigStruct.interruptMode = kADC_InterruptForEachSequence;

    ADC_SetConvSeqAConfig(adc_addrs[instance], &adcConvSeqConfigStruct);
    ADC_EnableConvSeqA(adc_addrs[instance], true);
    ADC_DoSoftwareTriggerConvSeqA(adc_addrs[instance]);

    /* Wait for the converter to be done. */
    while (!ADC_GetChannelConversionResult(adc_addrs[instance], channel, &adcResultInfoStruct)) {
    }

    return adcResultInfoStruct.result;
}

float analogin_read(analogin_t *obj)
{
    uint16_t value = analogin_read_u16(obj);
    return (float)value * (1.0f / (float)0xFFFF);
}

#endif
