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
#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H
#include "em_adc.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DAC0_CH0,
    DAC0_CH1,
} DACName;

typedef enum {
    ADC0_PD0 = _ADC_SINGLECTRL_INPUTSEL_CH0,
    ADC0_PD1 = _ADC_SINGLECTRL_INPUTSEL_CH1,
    ADC0_PD2 = _ADC_SINGLECTRL_INPUTSEL_CH2,
    ADC0_PD3 = _ADC_SINGLECTRL_INPUTSEL_CH3,
    ADC0_PD4 = _ADC_SINGLECTRL_INPUTSEL_CH4,
    ADC0_PD5 = _ADC_SINGLECTRL_INPUTSEL_CH5,
    ADC0_PD6 = _ADC_SINGLECTRL_INPUTSEL_CH6,
    ADC0_PD7 = _ADC_SINGLECTRL_INPUTSEL_CH7
} ADCName;

#ifdef __cplusplus
}
#endif

#endif
