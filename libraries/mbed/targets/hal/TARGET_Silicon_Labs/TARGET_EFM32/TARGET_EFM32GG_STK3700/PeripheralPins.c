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

#include "PeripheralPins.h"

/************ADC***************/
const PinMap PinMap_ADC[] = {
    {PD_0, ADC0_PD0, 0},
    {PD_1, ADC0_PD1, 0},
    {PD_2, ADC0_PD2, 0},
    {PD_3, ADC0_PD3, 0},
    {PD_4, ADC0_PD4, 0},
    {PD_5, ADC0_PD5, 0},
    {PD_6, ADC0_PD6, 0},
    {PD_7, ADC0_PD7, 0},
    {NC  , NC   , 0}
};

/************DAC***************/
const PinMap PinMap_DAC[] = {
    {PB_11, DAC0_CH0, 0},
    {PB_12, DAC0_CH1, 0},
    {NC  , NC   , 0}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
    {PA_12, PWM_CH0, 0},
    {PA_13, PWM_CH1, 0},
    {PA_14, PWM_CH2, 0},
    {NC  , NC   , 0}
};
