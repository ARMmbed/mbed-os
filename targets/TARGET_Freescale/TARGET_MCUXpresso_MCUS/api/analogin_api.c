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
#include "fsl_adc16.h"
#include "PeripheralPins.h"

/* Array of ADC peripheral base address. */
static ADC_Type *const adc_addrs[] = ADC_BASE_PTRS;

#define MAX_FADC 6000000

#if STATIC_PINMAP_READY
#define ANALOGIN_INIT_DIRECT analogin_init_direct
void analogin_init_direct(analogin_t *obj, const PinMap *pinmap)
#else
#define ANALOGIN_INIT_DIRECT _analogin_init_direct
static void _analogin_init_direct(analogin_t *obj, const PinMap *pinmap)
#endif
{
    obj->adc = (ADCName)pinmap->peripheral;
    MBED_ASSERT(obj->adc != (ADCName)NC);

    uint32_t instance = obj->adc >> ADC_INSTANCE_SHIFT;
    uint32_t bus_clock;
    adc16_config_t adc16_config;

    bus_clock = CLOCK_GetFreq(kCLOCK_BusClk);
    uint32_t clkdiv;
    for (clkdiv = 0; clkdiv < 4; clkdiv++) {
        if ((bus_clock >> clkdiv) <= MAX_FADC) {
            break;
        }
    }
    if (clkdiv == 4) {
        clkdiv = 0x3; //Set max div
    }

    ADC16_GetDefaultConfig(&adc16_config);
    adc16_config.clockSource = kADC16_ClockSourceAlt0;
    adc16_config.clockDivider = (adc16_clock_divider_t)clkdiv;
    adc16_config.resolution = kADC16_ResolutionSE16Bit;
    ADC16_Init(adc_addrs[instance], &adc16_config);
    ADC16_EnableHardwareTrigger(adc_addrs[instance], false);
    ADC16_SetHardwareAverage(adc_addrs[instance], kADC16_HardwareAverageCount4);
    pin_function(pinmap->pin, pinmap->function);
    pin_mode(pinmap->pin, PullNone);
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
    adc16_channel_config_t adc16_channel_config;

    adc16_channel_config.channelNumber = obj->adc & 0xF;
    adc16_channel_config.enableInterruptOnConversionCompleted = false;

#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adc16_channel_config.enableDifferentialConversion = false;
#endif

    ADC16_SetChannelMuxMode(adc_addrs[instance],
                            obj->adc & (1 << ADC_B_CHANNEL_SHIFT) ? kADC16_ChannelMuxB : kADC16_ChannelMuxA);

    /*
     * When in software trigger mode, each conversion would be launched once calling the "ADC16_ChannelConfigure()"
     * function, which works like writing a conversion command and executing it.
     */
    ADC16_SetChannelConfig(adc_addrs[instance], 0, &adc16_channel_config);
    while (0U == (kADC16_ChannelConversionDoneFlag &
                  ADC16_GetChannelStatusFlags(adc_addrs[instance], 0))) {
    }
    return ADC16_GetChannelConversionValue(adc_addrs[instance], 0);
}

float analogin_read(analogin_t *obj)
{
    uint16_t value = analogin_read_u16(obj);
    return (float)value * (1.0f / (float)0xFFFF);
}

const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}

#endif
