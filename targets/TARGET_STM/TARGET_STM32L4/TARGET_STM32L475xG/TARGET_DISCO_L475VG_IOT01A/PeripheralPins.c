/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2017, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

#include "PeripheralPins.h"

//==============================================================================
// Notes
//
// - The pins mentionned Px_y_ALTz are alternative possibilities which use other
//   HW peripheral instances. You can use them the same way as any other "normal"
//   pin (i.e. PwmOut pwm(PA_7_ALT0);). These pins are not displayed on the board
//   pinout image on mbed.org.
//
// - The pins which are connected to other components present on the board have
//   the comment "Connected to xxx". The pin function may not work properly in this
//   case. These pins may not be displayed on the board pinout image on mbed.org.
//   Please read the board reference manual and schematic for more information.
//
//==============================================================================

//*** ADC ***

const PinMap PinMap_ADC[] = {
    {PA_0,      ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 5, 0)}, // ARDUINO D1
    {PA_0_ALT0, ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 5, 0)}, // ARDUINO D1
    {PA_1,      ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 6, 0)}, // ARDUINO D0
    {PA_1_ALT0, ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 6, 0)}, // ARDUINO D0
    {PA_2,      ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 7, 0)}, // ARDUINO D10
    {PA_2_ALT0, ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 7, 0)}, // ARDUINO D10
    {PA_3,      ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 8, 0)}, // ARDUINO D4
    {PA_3_ALT0, ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 8, 0)}, // ARDUINO D4
    {PA_4,      ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 9, 0)}, // ARDUINO D7
    {PA_4_ALT0, ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 9, 0)}, // ARDUINO D7
    {PA_5,      ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 10, 0)}, // ARDUINO D13 - Connected to LED1
    {PA_5_ALT0, ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 10, 0)}, // ARDUINO D13 - Connected to LED1
    {PA_6,      ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 11, 0)}, // ARDUINO D12
    {PA_6_ALT0, ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 11, 0)}, // ARDUINO D12
    {PA_7,      ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 12, 0)}, // ARDUINO D11
    {PA_7_ALT0, ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 12, 0)}, // ARDUINO D11
    {PB_0,      ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 15, 0)}, // ARDUINO D3
    {PB_0_ALT0, ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 15, 0)}, // ARDUINO D3
    {PB_1,      ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 16, 0)}, // ARDUINO D6
    {PB_1_ALT0, ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 16, 0)}, // ARDUINO D6
    {PC_0,      ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 1, 0)}, // ARDUINO A5
    {PC_0_ALT0, ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 1, 0)}, // ARDUINO A5
    {PC_0_ALT1, ADC_3, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 1, 0)}, // ARDUINO A5
    {PC_1,      ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 2, 0)}, // ARDUINO A4
    {PC_1_ALT0, ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 2, 0)}, // ARDUINO A4
    {PC_1_ALT1, ADC_3, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 2, 0)}, // ARDUINO A4
    {PC_2,      ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 3, 0)}, // ARDUINO A3
    {PC_2_ALT0, ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 3, 0)}, // ARDUINO A3
    {PC_2_ALT1, ADC_3, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 3, 0)}, // ARDUINO A3
    {PC_3,      ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 4, 0)}, // ARDUINO A2
    {PC_3_ALT0, ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 4, 0)}, // ARDUINO A2
    {PC_3_ALT1, ADC_3, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 4, 0)}, // ARDUINO A2
    {PC_4,      ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 13, 0)}, // ARDUINO A1
    {PC_4_ALT0, ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 13, 0)}, // ARDUINO A1
    {PC_5,      ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 14, 0)}, // ARDUINO A0
    {PC_5_ALT0, ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 14, 0)}, // ARDUINO A0
    {NC,        NC,    0}
};

const PinMap PinMap_ADC_Internal[] = {
    {ADC_TEMP, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 17, 0)},
    {ADC_VREF, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0,  0, 0)},
    {ADC_VBAT, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 18, 0)},
    {NC,       NC,    0}
};

//*** DAC ***

const PinMap PinMap_DAC[] = {
    {PA_4, DAC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 1, 0)}, // ARDUINO D7
    {PA_5, DAC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 2, 0)}, // ARDUINO D13 - Connected to LED1
    {NC,   NC,    0}
};

//*** I2C ***

const PinMap PinMap_I2C_SDA[] = {
//    {PB_7,  I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)}, // Pin not available on any connector
    {PB_9,  I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)}, // ARDUINO D14
    {PB_11, I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)}, // Pin not available on any connector
    {PB_14, I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)}, // Connected to LED2
    {PC_1,  I2C_3, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C3)}, // ARDUINO A4
    {NC,    NC,    0}
};

const PinMap PinMap_I2C_SCL[] = {
//    {PB_6,  I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)}, // Pin not available on any connector
    {PB_8,  I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)}, // ARDUINO D15
    {PB_10, I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)}, // Pin not available on any connector
    {PB_13, I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)}, // Pin not available on any connector
    {PC_0,  I2C_3, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C3)}, // ARDUINO A5
    {NC,    NC,    0}
};

//*** PWM ***

// Warning: Pins using PWM_5 cannot be used as TIMER5 is already used by the us_ticker.
const PinMap PinMap_PWM[] = {
    {PA_0,       PWM_2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 1, 0)}, // ARDUINO D1
//  {PA_0,       PWM_5,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM5, 1, 0)}, // TIM5_CH1 ARDUINO D1
    {PA_1,       PWM_2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 2, 0)}, // TIM2_CH2 ARDUINO D0
    {PA_1_ALT0,  PWM_15, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_TIM15, 1, 1)}, // TIM15_CH1N ARDUINO D0
//  {PA_1,       PWM_5,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM5, 2, 0)}, // TIM5_CH2 ARDUINO D0
    {PA_2,       PWM_2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 3, 0)}, // TIM2_CH3 ARDUINO D10
    {PA_2_ALT0,  PWM_15, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_TIM15, 1, 0)}, // TIM15_CH1 ARDUINO D10
//  {PA_2,       PWM_5,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM5, 3, 0)}, // TIM5_CH3 ARDUINO D10
    {PA_3,       PWM_2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 4, 0)}, // TIM2_CH4 ARDUINO D4
    {PA_3_ALT0,  PWM_15, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_TIM15, 2, 0)}, // TIM15_CH2 ARDUINO D4
//  {PA_3,       PWM_5,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM5, 4, 0)}, // TIM5_CH4 ARDUINO D4
    {PA_5,       PWM_2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 1, 0)}, // TIM2_CH1 ARDUINO D13 - Connected to LED1
    {PA_5_ALT0,  PWM_8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 1, 1)}, // TIM8_CH1N ARDUINO D13 - Connected to LED1
    {PA_6,       PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 1, 0)}, // TIM3_CH1 ARDUINO D12
    {PA_6_ALT0,  PWM_16, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_TIM16, 1, 0)}, // TIM16_CH1 ARDUINO D12
    {PA_7,       PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 1, 1)}, // TIM1_CH1N ARDUINO D11
    {PA_7_ALT0,  PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 2, 0)}, // TIM3_CH2 ARDUINO D11
    {PA_7_ALT1,  PWM_8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 1, 1)}, // TIM8_CH1N ARDUINO D11
    {PA_7_ALT2,  PWM_17, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_TIM17, 1, 0)}, // TIM17_CH1 ARDUINO D11
    {PA_8,       PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 1, 0)}, // TIM1_CH1 Pin not available on any connector
    {PA_9,       PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 2, 0)}, // TIM1_CH2 Pin not available on any connector
    {PA_10,      PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 3, 0)}, // TIM1_CH3 Pin not available on any connector
    {PA_11,      PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 4, 0)}, // TIM1_CH4 Pin not available on any connector
    {PA_15,      PWM_2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 1, 0)}, // TIM2_CH1 ARDUINO D9
    {PB_0,       PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 3, 0)}, // TIM3_CH3 ARDUINO D3
    {PB_0_ALT0,  PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 2, 1)}, // TIM1_CH2N ARDUINO D3
    {PB_0_ALT1,  PWM_8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 2, 1)}, // TIM8_CH2N ARDUINO D3
    {PB_1,       PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 4, 0)}, // TIM3_CH4 ARDUINO D6
    {PB_1_ALT0,  PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 3, 1)}, // TIM1_CH3N ARDUINO D6
    {PB_1_ALT1,  PWM_8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 3, 1)}, // TIM8_CH3N ARDUINO D6
    {PB_3,       PWM_2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 2, 0)}, // TIM2_CH2 Pin not available on any connector
    {PB_4,       PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 1, 0)}, // TIM3_CH1 ARDUINO D5
    {PB_5,       PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 2, 0)}, // TIM3_CH2 Pin not available on any connector
//    {PB_6,       PWM_4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 1, 0)}, // TIM4_CH1 Pin not available on any connector
//    {PB_6_ALT0,  PWM_16, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_TIM16, 1, 1)}, // TIM16_CH1N Pin not available on any connector
//    {PB_7,       PWM_4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 2, 0)}, // TIM4_CH2 Pin not available on any connector
//    {PB_7_ALT0,  PWM_17, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_TIM17, 1, 1)}, // TIM17_CH1N Pin not available on any connector
    {PB_8,       PWM_4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 3, 0)}, // TIM4_CH3 ARDUINO D15
    {PB_8_ALT0,  PWM_16, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_TIM16, 1, 0)}, // TIM16_CH1 ARDUINO D15
    {PB_9,       PWM_4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 4, 0)}, // TIM4_CH4 ARDUINO D14
    {PB_9_ALT0,  PWM_17, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_TIM17, 1, 0)}, // TIM17_CH1 ARDUINO D14
    {PB_10,      PWM_2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 3, 0)}, // TIM2_CH3 Pin not available on any connector
    {PB_11,      PWM_2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2, 4, 0)}, // TIM2_CH4 Pin not available on any connector
    {PB_13,      PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 1, 1)}, // TIM1_CH1N Pin not available on any connector
    {PB_13_ALT0, PWM_15, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_TIM15, 1, 1)}, // TIM15_CH1N Pin not available on any connector
    {PB_14,      PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 2, 1)}, // TIM1_CH2N - Connected to LED2
    {PB_14_ALT0, PWM_8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 2, 1)}, // TIM8_CH2N - Connected to LED2
    {PB_14_ALT1, PWM_15, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_TIM15, 1, 0)}, // TIM15_CH1 - Connected to LED2
    {PB_15,      PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 3, 1)}, // TIM1_CH3N Pin not available on any connector
    {PB_15_ALT0, PWM_8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 3, 1)}, // TIM8_CH3N Pin not available on any connector
    {PB_15_ALT1, PWM_15, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_TIM15, 2, 0)}, // TIM15_CH2 Pin not available on any connector
    {PC_6,       PWM_8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 1, 0)}, // TIM8_CH1 Pin not available on any connector
    {PC_6_ALT0,  PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 1, 0)}, // TIM3_CH1 Pin not available on any connector
    {PC_7,       PWM_8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 2, 0)}, // TIM8_CH2 Pin not available on any connector
    {PC_7_ALT0,  PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 2, 0)}, // TIM3_CH2 Pin not available on any connector
    {PC_8,       PWM_8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 3, 0)}, // TIM8_CH3 Pin not available on any connector
    {PC_8_ALT0,  PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 3, 0)}, // TIM3_CH3 Pin not available on any connector
    {PC_9,       PWM_8,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF3_TIM8, 4, 0)}, // TIM8_CH4 - Connected to LED3/LED4
    {PC_9_ALT0,  PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 4, 0)}, // TIM3_CH4 - Connected to LED3/LED4
    {PD_12,      PWM_4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 1, 0)}, // TIM4_CH1 Pin not available on any connector
    {PD_13,      PWM_4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 2, 0)}, // TIM4_CH2 Pin not available on any connector
    {PD_14,      PWM_4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 3, 0)}, // TIM4_CH3 ARDUINO D2
    {PD_15,      PWM_4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4, 4, 0)}, // TIM4_CH4 Pin not available on any connector
    {PE_0,       PWM_16, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_TIM16, 1, 0)}, // TIM16_CH1 Pin not available on any connector
    {PE_1,       PWM_17, STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_TIM17, 1, 0)}, // TIM17_CH1 Pin not available on any connector
    {PE_3,       PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 1, 0)}, // TIM3_CH1 Pin not available on any connector
    {PE_4,       PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 2, 0)}, // TIM3_CH2 Pin not available on any connector
    {PE_5,       PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 3, 0)}, // TIM3_CH3 Pin not available on any connector
    {PE_6,       PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 4, 0)}, // TIM3_CH4 Pin not available on any connector
    {PE_8,       PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 1, 1)}, // TIM1_CH1N Pin not available on any connector
    {PE_9,       PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 1, 0)}, // TIM1_CH1 Pin not available on any connector
    {PE_10,      PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 2, 1)}, // TIM1_CH2N Pin not available on any connector
    {PE_11,      PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 2, 0)}, // TIM1_CH2 Pin not available on any connector
    {PE_12,      PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 3, 1)}, // TIM1_CH3N Pin not available on any connector
    {PE_13,      PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 3, 0)}, // TIM1_CH3 Pin not available on any connector
    {PE_14,      PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1, 4, 0)}, // TIM1_CH4 Pin not available on any connector
    {NC,         NC,     0}
};

//*** SERIAL ***

const PinMap PinMap_UART_TX[] = {
    {PA_0,       UART_4,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART4)}, // ARDUINO D1
    {PA_2,       UART_2,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)}, // ARDUINO D10
    {PA_9,       UART_1,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)}, // Pin not available on any connector
    {PB_6,       UART_1,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)}, // Pin not available on any connector - Connected to STDIO_UART_TX
    {PB_10,      UART_3,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)}, // Pin not available on any connector
    {PB_11,      LPUART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_LPUART1)}, // Pin not available on any connector
    {PC_1,       LPUART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_LPUART1)}, // ARDUINO A4
    {PC_4,       UART_3,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)},  // ARDUINO A1
    {PC_10,      UART_4,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART4)}, // Pin not available on any connector
    {PC_10_ALT0, UART_3,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)}, // Pin not available on any connector
    {PC_12,      UART_5,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART5)}, // Pin not available on any connector
    {PD_5,       UART_2,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)}, // PMOD 1
    {PD_8,       UART_3,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)}, // Pin not available on any connector
    {NC,         NC,       0}
};

const PinMap PinMap_UART_RX[] = {
    {PA_1,       UART_4,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART4)}, // ARDUINO D0
    {PA_3,       UART_2,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)}, // ARDUINO D4
    {PA_10,      UART_1,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)}, // Pin not available on any connector
    {PB_7,       UART_1,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)}, // Pin not available on any connector - Connected to STDIO_UART_RX
    {PB_10,      LPUART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_LPUART1)}, // Pin not available on any connector
    {PB_11,      UART_3,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)}, // Pin not available on any connector
    {PC_0,       LPUART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_LPUART1)}, // ARDUINO A5
    {PC_5,       UART_3,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)},  // ARDUINO A0
    {PC_11,      UART_4,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART4)}, // Pin not available on any connector
    {PC_11_ALT0, UART_3,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)}, // Pin not available on any connector
    {PD_2,       UART_5,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART5)}, // PMOD 7
    {PD_6,       UART_2,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)}, // Pin not available on any connector
    {PD_9,       UART_3,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)}, // Pin not available on any connector
    {NC,         NC,       0}
};

const PinMap PinMap_UART_RTS[] = {
    {PA_1,  UART_2,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)}, // ARDUINO D0
    {PA_12, UART_1,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)}, // Pin not available on any connector
    {PA_15, UART_4,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART4)}, // ARDUINO D9
    {PB_1,  UART_3,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)}, // ARDUINO D6
    {PB_3,  UART_1,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)}, // Pin not available on any connector
    {PB_4,  UART_5,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART5)}, // ARDUINO D5
    {PB_12, LPUART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_LPUART1)}, // Pin not available on any connector
    {PB_14, UART_3,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)}, // Connected to LED2
    {PD_2,  UART_3,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)}, // PMOD 7
    {PD_4,  UART_2,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)}, // PMOD 2
    {PD_12, UART_3,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)}, // Pin not available on any connector
    {NC,    NC,       0}
};

const PinMap PinMap_UART_CTS[] = {
    {PA_0,       UART_2,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)}, // ARDUINO D1
    {PA_6,       UART_3,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)}, // ARDUINO D12
    {PA_11,      UART_1,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)}, // Pin not available on any connector
    {PB_4,       UART_1,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)}, // ARDUINO D5
    {PB_5,       UART_5,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART5)}, // Pin not available on any connector
//    {PB_7,       UART_4,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART4)}, // Pin not available on any connector
    {PB_13,      LPUART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_LPUART1)}, // Pin not available on any connector
    {PB_13_ALT0, UART_3,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)}, // Pin not available on any connector
    {PD_3,       UART_2,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)}, // PMOD 3
    {PD_11,      UART_3,   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)}, // Pin not available on any connector
    {NC,         NC,       0}
};

//*** SPI ***

const PinMap PinMap_SPI_MOSI[] = {
    {PA_7,      SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)}, // ARDUINO D11
    {PB_5,      SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)}, // Pin not available on any connector
    {PB_5_ALT0, SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)}, // Pin not available on any connector
    {PB_15,     SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)}, // Pin not available on any connector
    {PC_3,      SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)}, // ARDUINO A2
    {PC_12,     SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)}, // Pin not available on any connector
    {PD_4,      SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)}, // PMOD 2
    {PE_15,     SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)}, // Pin not available on any connector
    {NC,        NC,    0}
};

const PinMap PinMap_SPI_MISO[] = {
    {PA_6,      SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)}, // ARDUINO D12
    {PB_4,      SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)}, // ARDUINO D5
    {PB_4_ALT0, SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)}, // ARDUINO D5
    {PB_14,     SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)}, // Connected to LED2
    {PC_2,      SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)}, // ARDUINO A3
    {PC_11,     SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)}, // Pin not available on any connector
    {PD_3,      SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)}, // PMOD 3
    {PE_14,     SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)}, // Pin not available on any connector
    {NC,        NC,    0}
};

const PinMap PinMap_SPI_SCLK[] = {
    {PA_5,      SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)}, // ARDUINO D13 - Connected to LED1
    {PB_3,      SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)}, // Pin not available on any connector
    {PB_3_ALT0, SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)}, // Pin not available on any connector
    {PB_10,     SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)}, // Pin not available on any connector
    {PB_13,     SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)}, // Pin not available on any connector
    {PC_10,     SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)}, // Pin not available on any connector
    {PD_1,      SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)}, // PMOD 4
    {PE_13,     SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)}, // Pin not available on any connector
    {NC,        NC,    0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {PA_4,       SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)}, // ARDUINO D7
    {PA_4_ALT0,  SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)}, // ARDUINO D7
    {PA_15,      SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)}, // ARDUINO D9
    {PA_15_ALT0, SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3)}, // ARDUINO D9
    {PB_9,       SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)}, // ARDUINO D14
    {PB_12,      SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)}, // Pin not available on any connector
    {PD_0,       SPI_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2)}, // PMOD 8
    {PE_12,      SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)}, // Pin not available on any connector
    {NC,         NC,    0}
};

//*** CAN ***

const PinMap PinMap_CAN_RD[] = {
    {PA_11, CAN_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF9_CAN1)}, // Pin not available on any connector
    {PB_8,  CAN_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF9_CAN1)}, // ARDUINO D15
    {PD_0,  CAN_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF9_CAN1)}, // PMOD 8
    {NC,    NC,    0}
};

const PinMap PinMap_CAN_TD[] = {
    {PA_12, CAN_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF9_CAN1)}, // Pin not available on any connector
    {PB_9,  CAN_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF9_CAN1)}, // ARDUINO D14
    {PD_1,  CAN_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF9_CAN1)}, // PMOD 4
    {NC,    NC,    0}
};
