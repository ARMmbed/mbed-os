/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
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
#include "PeripheralPins.h"
#include "mbed_error.h"

#define DEV_ADC_ACCURACY_12BIT           (0xFFF)
#define DEV_ADC_PRECISION_12TO16(val)    ((val << 4)| ((val >> 8) & (uint16_t)0x000F))
#define AND_NUMBER                       (0xFF)

FlagStatus temperature_sample_flag = RESET;

/** software delay
 *
 * @param time The time need to delay
 */
static void _delay(uint16_t time)
{
    uint16_t i;
    for (i = 0; i < time; i++) {
    }
}

/** Initialize the analogin peripheral
 *
 * Configures the pin used by analogin.
 * @param obj The analogin object to initialize
 * @param pin The analogin pin name
 */
void analogin_init(analogin_t *obj, PinName pin)
{
    uint32_t periph;

    MBED_ASSERT(obj);

    obj->adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);
    MBED_ASSERT(obj->adc != (ADCName)NC);

    uint32_t function = pinmap_function(pin, PinMap_ADC);
    MBED_ASSERT(function != (uint32_t)NC);

    obj->channel = GD_PIN_CHANNEL_GET(function);
    MBED_ASSERT(obj->channel <= ADC_CHANNEL_18);

    periph = obj->adc;
    /* save the pin for future use */
    obj->pin = pin;

    /* ADC clock enable and pin number reset*/
    switch (periph) {
        case ADC0:
            rcu_periph_clock_enable(RCU_ADC0);
            /* No need to reset pin number  */
            break;

        case ADC1:
            rcu_periph_clock_enable(RCU_ADC1);
            /* reset pin number */
            pin = (PinName)(pin & AND_NUMBER);
            break;

        case ADC2:
            rcu_periph_clock_enable(RCU_ADC2);
            /* reset pin number */
            if ((pin > MUL1) | (pin == MUL1)) {
                pin = (PinName)(pin & AND_NUMBER);
            }
            break;
    }

    /* ADC clock cannot be greater than 40MHz */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV6);

    if ((ADC_CHANNEL_16 == obj->channel)) {
        /* ADC temperature channel enable */
        adc_channel_16_to_18(ADC_TEMP_VREF_CHANNEL_SWITCH, ENABLE);
        /* set temperature sample flag */
        temperature_sample_flag = SET;
    }
    if ((ADC_CHANNEL_17 == obj->channel)) {
        /* ADC Vref channel enable */
        adc_channel_16_to_18(ADC_TEMP_VREF_CHANNEL_SWITCH, ENABLE);
    }
    if ((ADC_CHANNEL_18 == obj->channel)) {
        /* ADC Vbat channel enable */
        adc_channel_16_to_18(ADC_VBAT_CHANNEL_SWITCH, ENABLE);
    }

    /* when pin >= ADC_TEMP, it indicates that the channel has no external pins */
    if (pin < ADC_TEMP) {
        pinmap_pinout(pin, PinMap_ADC);
    }

    /* ADC configuration */
    /* ADC mode config */
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);
    /* ADC scan mode disable */
    adc_special_function_config(obj->adc, ADC_SCAN_MODE, DISABLE);
    /* ADC continuous function disable */
    adc_special_function_config(obj->adc, ADC_CONTINUOUS_MODE, DISABLE);
    /* ADC data alignment config */
    adc_data_alignment_config(obj->adc, ADC_DATAALIGN_RIGHT);
    /* ADC channel length config */
    adc_channel_length_config(obj->adc, ADC_REGULAR_CHANNEL, 1);

    adc_external_trigger_config(obj->adc, ADC_REGULAR_CHANNEL, EXTERNAL_TRIGGER_DISABLE);
    /* ADC regular channel config */
    if (temperature_sample_flag == SET) {
        /* sample temperature needs more time */
        adc_regular_channel_config(obj->adc, 0, obj->channel, ADC_SAMPLETIME_480);
        /* clear temperature sample flag */
        temperature_sample_flag = RESET;
    } else {
        adc_regular_channel_config(obj->adc, 0, obj->channel, ADC_SAMPLETIME_15);
    }

    /* ADC enable */
    adc_enable(obj->adc);
    /* wait for ADC to stabilize */
    _delay(500);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(obj->adc);
}

/** Read the value from analogin pin, represented as an unsigned 16bit value
 *
 * @param obj The analogin object
 * @return An unsigned 16bit value representing the current input voltage
 */
uint16_t analogin_read_u16(analogin_t *obj)
{
    uint16_t reval;

    adc_flag_clear(obj->adc, ADC_FLAG_EOC);
    /* start Conversion */
    adc_software_trigger_enable(obj->adc, ADC_REGULAR_CHANNEL);

    while (SET != adc_flag_get(obj->adc, ADC_FLAG_EOC)) {
    }
    /* ADC actual accuracy is 12 bits */
    reval = adc_regular_data_read(obj->adc);

    reval = DEV_ADC_PRECISION_12TO16(reval);

    return reval;
}

/** Read the input voltage, represented as a float in the range [0.0, 1.0]
 *
 * @param obj The analogin object
 * @return A floating value representing the current input voltage
 */
float analogin_read(analogin_t *obj)
{
    uint16_t reval;

    adc_flag_clear(obj->adc, ADC_FLAG_EOC);
    /* start Conversion */
    adc_software_trigger_enable(obj->adc, ADC_REGULAR_CHANNEL);
    /* wait for conversion to complete */
    while (SET != adc_flag_get(obj->adc, ADC_FLAG_EOC)) {
    }
    /* ADC actual accuracy is 12 bits */
    reval = adc_regular_data_read(obj->adc);

    return (float)reval * (1.0f / (float)DEV_ADC_ACCURACY_12BIT);
}

const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}

#endif /* DEVICE_ANALOGIN */
