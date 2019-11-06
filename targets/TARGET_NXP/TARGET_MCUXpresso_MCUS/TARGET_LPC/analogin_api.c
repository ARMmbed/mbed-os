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

#if DEVICE_ANALOGIN && !defined(NXP_LPADC)

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
#define MAX_ADC_CLOCK 80000000

#if STATIC_PINMAP_READY
#define ANALOGIN_INIT_DIRECT analogin_init_direct
void analogin_init_direct(analogin_t *obj, const PinMap *pinmap)
#else
#define ANALOGIN_INIT_DIRECT _analogin_init_direct
static void _analogin_init_direct(analogin_t *obj, const PinMap *pinmap)
#endif
{
    uint32_t clkval;
    uint32_t clkdiv = 1;

    obj->adc = (ADCName)pinmap->peripheral;
    MBED_ASSERT(obj->adc != (ADCName)NC);

    uint32_t instance = obj->adc >> ADC_INSTANCE_SHIFT;
    adc_config_t adc_config;
    uint32_t reg;
    uint32_t pin_number = pinmap->pin & 0x1F;
    uint8_t port_number = pinmap->pin / 32;

    ADC_ClockPower_Configuration();

    /* Ensure the ADC clock derived from the system clock is less than 80MHz */
    clkval = CLOCK_GetFreq(kCLOCK_BusClk);
    while ((clkval / clkdiv) > MAX_ADC_CLOCK) {
        clkdiv++;
    }

    /* Calibration after power up. */
    if (!(ADC_DoSelfCalibration(adc_addrs[instance]))) {
        /* Calibration failed */
        return;
    }

    ADC_GetDefaultConfig(&adc_config);
    adc_config.clockDividerNumber = (clkdiv - 1);

    ADC_Init(adc_addrs[instance], &adc_config);
    pin_function(pinmap->pin, pinmap->function);
    pin_mode(pinmap->pin, PullNone);

    /* Clear the DIGIMODE & MODE bits */
    reg = IOCON->PIO[port_number][pin_number] & ~(IOCON_PIO_DIGIMODE_MASK | IOCON_PIO_MODE_MASK);
    IOCON->PIO[port_number][pin_number] = reg;
}

void analogin_init(analogin_t *obj, PinName pin)
{
    int peripheral = (int)pinmap_peripheral(pin, PinMap_ADC);
    int function = (int)pinmap_find_function(pin, PinMap_ADC);

    const PinMap static_pinmap = {pin, peripheral, function};

    ANALOGIN_INIT_DIRECT(obj, &static_pinmap);
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    uint32_t instance = obj->adc >> ADC_INSTANCE_SHIFT;
    uint32_t channel = obj->adc & 0xFF;
    adc_conv_seq_config_t adcConvSeqConfigStruct;
    adc_result_info_t adcResultInfoStruct;

    adcConvSeqConfigStruct.channelMask = (1U << channel);
    adcConvSeqConfigStruct.triggerMask = 0U;
    adcConvSeqConfigStruct.triggerPolarity = kADC_TriggerPolarityPositiveEdge;
    adcConvSeqConfigStruct.enableSingleStep = false;
    adcConvSeqConfigStruct.enableSyncBypass = false;
    adcConvSeqConfigStruct.interruptMode = kADC_InterruptForEachSequence;

    ADC_SetConvSeqAConfig(adc_addrs[instance], &adcConvSeqConfigStruct);
    ADC_EnableConvSeqA(adc_addrs[instance], true);
    ADC_DoSoftwareTriggerConvSeqA(adc_addrs[instance]);

    /* Wait for the converter to be done. */
    while (!ADC_GetChannelConversionResult(adc_addrs[instance], channel, &adcResultInfoStruct)) {
    }

    /* The ADC has 12 bit resolution. We shift in 4 0s */
    /* from the right to make it a 16 bit number as expected */
    return adcResultInfoStruct.result << 4;
}

float analogin_read(analogin_t *obj)
{
    uint16_t value = analogin_read_u16(obj);
    return (float)value * (1.0f / (float)0xFFF0);
}

const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}

#endif
