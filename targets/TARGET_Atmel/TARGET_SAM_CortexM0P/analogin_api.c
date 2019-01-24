/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "adc.h"
#include "status_codes.h"

extern uint8_t g_sys_init;
struct adc_module adc_instance;

void adc_configure_ain_pin(uint32_t pin)
{
#define PIN_INVALID_ADC_AIN    0xFFFFUL

    /* Pinmapping table for AINxx -> GPIO pin number */
    const uint32_t pinmapping[] = {
#if (SAMD20E | SAMD21E)
        PIN_PA02B_ADC_AIN0,  PIN_PA03B_ADC_AIN1,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_PA04B_ADC_AIN4,  PIN_PA05B_ADC_AIN5,
        PIN_PA06B_ADC_AIN6,  PIN_PA07B_ADC_AIN7,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_PA08B_ADC_AIN16, PIN_PA09B_ADC_AIN17,
        PIN_PA10B_ADC_AIN18, PIN_PA11B_ADC_AIN19,
#elif (SAMD20G | SAMD21G)
        PIN_PA02B_ADC_AIN0,  PIN_PA03B_ADC_AIN1,
        PIN_PB08B_ADC_AIN2,  PIN_PB09B_ADC_AIN3,
        PIN_PA04B_ADC_AIN4,  PIN_PA05B_ADC_AIN5,
        PIN_PA06B_ADC_AIN6,  PIN_PA07B_ADC_AIN7,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_PB02B_ADC_AIN10, PIN_PB03B_ADC_AIN11,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_PA08B_ADC_AIN16, PIN_PA09B_ADC_AIN17,
        PIN_PA10B_ADC_AIN18, PIN_PA11B_ADC_AIN19,
#elif (SAMD20J | SAMD21J)
        PIN_PA02B_ADC_AIN0,  PIN_PA03B_ADC_AIN1,
        PIN_PB08B_ADC_AIN2,  PIN_PB09B_ADC_AIN3,
        PIN_PA04B_ADC_AIN4,  PIN_PA05B_ADC_AIN5,
        PIN_PA06B_ADC_AIN6,  PIN_PA07B_ADC_AIN7,
        PIN_PB00B_ADC_AIN8,  PIN_PB01B_ADC_AIN9,
        PIN_PB02B_ADC_AIN10, PIN_PB03B_ADC_AIN11,
        PIN_PB04B_ADC_AIN12, PIN_PB05B_ADC_AIN13,
        PIN_PB06B_ADC_AIN14, PIN_PB07B_ADC_AIN15,
        PIN_PA08B_ADC_AIN16, PIN_PA09B_ADC_AIN17,
        PIN_PA10B_ADC_AIN18, PIN_PA11B_ADC_AIN19,
#elif SAMR21E
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_PA06B_ADC_AIN6,  PIN_PA07B_ADC_AIN7,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_PA08B_ADC_AIN16, PIN_PA09B_ADC_AIN17,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
#elif SAMR21G
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_PB08B_ADC_AIN2,  PIN_PB09B_ADC_AIN3,
        PIN_PA04B_ADC_AIN4,  PIN_PA05B_ADC_AIN5,
        PIN_PA06B_ADC_AIN6,  PIN_PA07B_ADC_AIN7,
        PIN_PB00B_ADC_AIN8,  PIN_INVALID_ADC_AIN,
        PIN_PB02B_ADC_AIN10, PIN_PB03B_ADC_AIN11,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_PA08B_ADC_AIN16, PIN_PA09B_ADC_AIN17,
        PIN_PA10B_ADC_AIN18, PIN_PA11B_ADC_AIN19,
#elif (SAMD10C | SAMD11C)
        PIN_PA02B_ADC_AIN0,  PIN_INVALID_ADC_AIN,
        PIN_PA04B_ADC_AIN2,  PIN_PA05B_ADC_AIN3,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_PA14B_ADC_AIN6,  PIN_PA15B_ADC_AIN7,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
#elif (SAMD10DS | SAMD11DS)
        PIN_PA02B_ADC_AIN0,  PIN_INVALID_ADC_AIN,
        PIN_PA04B_ADC_AIN2,  PIN_PA05B_ADC_AIN3,
        PIN_PA06B_ADC_AIN4,  PIN_PA07B_ADC_AIN5,
        PIN_PA14B_ADC_AIN6,  PIN_PA15B_ADC_AIN7,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
#elif (SAMD10DM | SAMD11DM)
        PIN_PA02B_ADC_AIN0,  PIN_PA03B_ADC_AIN1,
        PIN_PA04B_ADC_AIN2,  PIN_PA05B_ADC_AIN3,
        PIN_PA06B_ADC_AIN4,  PIN_PA07B_ADC_AIN5,
        PIN_PA14B_ADC_AIN6,  PIN_PA15B_ADC_AIN7,
        PIN_PA10B_ADC_AIN8,  PIN_PA11B_ADC_AIN9,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
#elif (SAML21E)
        PIN_PA02B_ADC_AIN0,  PIN_PA03B_ADC_AIN1,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_PA04B_ADC_AIN4,  PIN_PA05B_ADC_AIN5,
        PIN_PA06B_ADC_AIN6,  PIN_PA07B_ADC_AIN7,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_PA08B_ADC_AIN16, PIN_PA09B_ADC_AIN17,
        PIN_PA10B_ADC_AIN18, PIN_PA11B_ADC_AIN19,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
#elif (SAML21G)
        PIN_PA02B_ADC_AIN0,  PIN_PA03B_ADC_AIN1,
        PIN_PB08B_ADC_AIN2,  PIN_PB09B_ADC_AIN3,
        PIN_PA04B_ADC_AIN4,  PIN_PA05B_ADC_AIN5,
        PIN_PA06B_ADC_AIN6,  PIN_PA07B_ADC_AIN7,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_PB02B_ADC_AIN10, PIN_PB03B_ADC_AIN11,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_PA08B_ADC_AIN16, PIN_PA09B_ADC_AIN17,
        PIN_PA10B_ADC_AIN18, PIN_PA11B_ADC_AIN19,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
#elif (SAML21J)
        PIN_PA02B_ADC_AIN0,  PIN_PA03B_ADC_AIN1,
        PIN_PB08B_ADC_AIN2,  PIN_PB09B_ADC_AIN3,
        PIN_PA04B_ADC_AIN4,  PIN_PA05B_ADC_AIN5,
        PIN_PA06B_ADC_AIN6,  PIN_PA07B_ADC_AIN7,
        PIN_PB00B_ADC_AIN8,  PIN_PB01B_ADC_AIN9,
        PIN_PB02B_ADC_AIN10, PIN_PB03B_ADC_AIN11,
        PIN_PB04B_ADC_AIN12, PIN_PB05B_ADC_AIN13,
        PIN_PB06B_ADC_AIN14, PIN_PB07B_ADC_AIN15,
        PIN_PA08B_ADC_AIN16, PIN_PA09B_ADC_AIN17,
        PIN_PA10B_ADC_AIN18, PIN_PA11B_ADC_AIN19,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
        PIN_INVALID_ADC_AIN, PIN_INVALID_ADC_AIN,
#else
#  error ADC pin mappings are not defined for this device.
#endif
    };

    uint32_t pin_map_result = PIN_INVALID_ADC_AIN;

    if (pin <= ADC_EXTCHANNEL_MSB) {
        pin_map_result = pinmapping[pin >> ADC_INPUTCTRL_MUXPOS_Pos];

        Assert(pin_map_result != PIN_INVALID_ADC_AIN);

        struct system_pinmux_config config;
        system_pinmux_get_config_defaults(&config);

        /* Analog functions are all on MUX setting B */
        config.input_pull   = SYSTEM_PINMUX_PIN_PULL_NONE;
        config.mux_position = 1;

        system_pinmux_pin_set_config(pin_map_result, &config);
    }
}

void analogin_init(analogin_t *obj, PinName pin)
{
    MBED_ASSERT(obj);
    if (g_sys_init == 0) {
        system_init();
        g_sys_init = 1;
    }
    uint32_t pos_input;
    static uint8_t init_flag = 0;

    pos_input = pinmap_find_peripheral(pin, PinMap_ADC);
    MBED_ASSERT(pos_input != (uint32_t)NC);

    adc_get_config_defaults(&(obj->config_adc));
    obj->config_adc.positive_input = (enum adc_positive_input)pos_input;
    if (init_flag == 0) {  // ADC init and enable to be done only once.
        adc_init(&adc_instance, ADC, &(obj->config_adc));
        adc_enable(&adc_instance);
        init_flag = 1;
    }
    adc_configure_ain_pin(obj->config_adc.positive_input);
    adc_configure_ain_pin(obj->config_adc.negative_input);
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    MBED_ASSERT(obj);
    uint16_t result;
    adc_set_positive_input(&adc_instance, obj->config_adc.positive_input);
    adc_set_negative_input(&adc_instance, obj->config_adc.negative_input);
    adc_start_conversion(&adc_instance);
    do {
    } while(adc_read(&(adc_instance), &result) == STATUS_BUSY);   // 12 bit value

    return (uint16_t)(((uint32_t)result * 0xFFFF) / 0x0FFF);  // for normalizing to 16 bit value
}

float analogin_read(analogin_t *obj)
{
    MBED_ASSERT(obj);
    uint16_t value = analogin_read_u16(obj);
    return (float)value * (1.0f / (float)0xFFFF);
}

const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}
