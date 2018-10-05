/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2016, STMicroelectronics
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

// =====
// Note: Commented lines are alternative possibilities which are not used per default.
//       If you change them, you will have also to modify the corresponding xxx_api.c file
//       for pwmout, analogin, analogout, ...
// =====

//*** ADC ***

const PinMap PinMap_ADC[] = {
    {ADC00,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0,  6, 0)}, // IN6
    {ADC01,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0,  3, 0)}, // IN3
    {ADC02,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG_ADC_CONTROL, GPIO_NOPULL, 0, 13, 0)}, // IN13
    {NC,    NC,    0}
};

const PinMap PinMap_ADC_Internal[] = {
    {NC,    NC,    0}
};

//*** DAC ***

const PinMap PinMap_DAC[] = {
    {NC,   NC,    0}
};

//*** I2C ***

const PinMap PinMap_I2C_SDA[] = {
    {I2C0_SDA, I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)},
    {I2C1_SDA, I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)},
    {NC,    NC,    0}
};

const PinMap PinMap_I2C_SCL[] = {
    {I2C0_SCL, I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)},
    {I2C1_SCL, I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)},
    {NC,    NC,    0}
};

//*** PWM ***

// Warning: TIM5 cannot be used because already used by the us_ticker.
const PinMap PinMap_PWM[] = {
    {PWM0_OUT, PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 3, 0)},  // TIM3_CH3
    {PWM1_OUT, PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3, 4, 0)},  // TIM3_CH4
    {NC,    NC,     0}
};

//*** SERIAL ***

const PinMap PinMap_UART_TX[] = {
    {UART2_TX, UART_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)},
    {UART_TXD, UART_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)},
    {LPUART1_TX,  LPUART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_LPUART1)},
    {UART1_TX,  UART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)},
    {NC,    NC,     0}
};

const PinMap PinMap_UART_RX[] = {
    {UART2_RX, UART_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)},
    {UART_RXD, UART_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)},
    {LPUART1_RX,  LPUART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_LPUART1)},
    {UART1_RX, UART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1)},
    {NC,    NC,     0}
};

const PinMap PinMap_UART_RTS[] = {
    {UART_RTS, UART_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)},
    {NC,    NC,     0}
};

const PinMap PinMap_UART_CTS[] = {
    {UART_CTS, UART_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)},
    {NC,    NC,     0}
};

//*** SPI ***

const PinMap PinMap_SPI_MOSI[] = {
    {SPI_MOSI, SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)}, // ARDUINO D11
    {NC,    NC,    0}
};

const PinMap PinMap_SPI_MISO[] = {
    {SPI_MISO, SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)}, // ARDUINO D12
    {NC,    NC,    0}
};

const PinMap PinMap_SPI_SCLK[] = {
    {SPI_SCK, SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)}, // ARDUINO D13
    {NC,    NC,    0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {SPI_CS0, SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)},
    {NC,    NC,    0}
};

const PinMap PinMap_CAN_RD[] = {
    {NC,    NC,    0}
};

const PinMap PinMap_CAN_TD[] = {
    {NC,    NC,    0}
};
