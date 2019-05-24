/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2016-2019, STMicroelectronics
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause-Clear
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
#include "mbed_toolchain.h"

 // =============================================================================
// Notes:
// * Commented lines are alternative possibilities which are not used per default.
//   If you change them, you will have also to modify the corresponding xxx_api.c file
//   for pwmout, analogin, analogout, ...
// * Only the pins that are placed on the Arduino connector are described.
// =============================================================================

 //*** ADC ***

 MBED_WEAK const PinMap PinMap_ADC[] = {
    {PA_0,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 0, 0)}, // ADC1_IN0  - Grove1_SIG2
    {PA_1,  ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 1, 0)}, // ADC1_IN1  - Grove1_SIG1
    {PA_6,  ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 6, 0)}, // ADC2_IN6  - Grove2_SIG2
    {PA_7,  ADC_2, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 7, 0)}, // ADC2_IN7  - Grove2_SIG1
    {NC,   NC,    0}
};

 MBED_WEAK const PinMap PinMap_ADC_Internal[] = {
    {ADC_TEMP, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 16, 0)}, // See in analogin_api.c the correct ADC channel used
    {ADC_VREF, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 17, 0)}, // See in analogin_api.c the correct ADC channel used
    {ADC_VBAT, ADC_1, STM_PIN_DATA_EXT(STM_MODE_ANALOG, GPIO_NOPULL, 0, 18, 0)}, // See in analogin_api.c the correct ADC channel used
    {NC,    NC,    0}
};

 //*** DAC ***

 MBED_WEAK const PinMap PinMap_DAC[] = {
    {NC,   NC,    0}
};

 //*** I2C ***

 MBED_WEAK const PinMap PinMap_I2C_SDA[] = {
    {PB_9,  I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)}, //   - Grove4_SIG2
    {PD_13, I2C_4, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C4)}, //   - Grove3_SIG2
    {NC,    NC,    0}
};

 MBED_WEAK const PinMap PinMap_I2C_SCL[] = {
    {PB_8,  I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)}, //    - Grove4_SIG1
    {PD_12, I2C_4, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C4)}, //    - Grove3_SIG1
    {NC,    NC,    0}
};

 //*** PWM ***

 MBED_WEAK const PinMap PinMap_PWM[] = {
    {PA_0,  PWM_2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2,  1, 0)}, // TIM2_CH1 - Grove1_SIG2
    {PA_1,  PWM_2,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM2,  2, 0)}, // TIM2_CH2 - Grove1_SIG1
    {PA_6,  PWM_3,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM3,  1, 0)}, // TIM3_CH1 -Grove2_SIG2
    {PA_7,  PWM_1,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF1_TIM1,  1, 1)}, // TIM1_CH1N -Grove2_SIG2
    {PB_8,  PWM_4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4,  3, 0)}, // TIM4_CH3 -Grove4_SIG1
    {PB_9,  PWM_4,  STM_PIN_DATA_EXT(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF2_TIM4,  4, 0)}, // TIM4_CH4 -Grove4_SIG2
    {NC,    NC,    0}
};

 //*** SERIAL ***

 MBED_WEAK const PinMap PinMap_UART_TX[] = {
    {PA_0,  UART_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART4)},  // - Grove1_SIG2
    {PA_2,  UART_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)}, // - esp32 tx
    {PB_14, UART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_USART1)}, // - esp32 tx (log)
    {PD_8,  UART_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)}, // STDIO_TX
    {NC,    NC,     0}
};

 MBED_WEAK const PinMap PinMap_UART_RX[] = {
    {PA_1,  UART_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_UART4)},  // - Grove1_SIG1
    {PA_3,  UART_2, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2)}, // - esp32 rx
    {PB_15, UART_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF4_USART1)}, // - esp32 rx (log)
    {PD_9,  UART_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART3)}, // STDIO_RX
    {NC,    NC,     0}
};

 MBED_WEAK const PinMap PinMap_UART_RTS[] = {
    {NC,    NC,     0}
};

 MBED_WEAK const PinMap PinMap_UART_CTS[] = {
    {NC,    NC,     0}
};

 //*** SPI ***

 MBED_WEAK const PinMap PinMap_SPI_MOSI[] = {
    {PC_12, SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF6_SPI3)}, // - SD_SPI3_MOSI(ReservedPin)
    {PE_14, SPI_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF5_SPI4)}, // - FLASH_SPI4_MOSI
    {NC,    NC,    0}
};

 MBED_WEAK const PinMap PinMap_SPI_MISO[] = {
    {PC_11, SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF6_SPI3)}, // - SD_SPI3_MISO/QSPI_CS(ReservedPin)
    {PE_13, SPI_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF5_SPI4)}, // - FLASH_SPI4_MISO
    {NC,    NC,    0}
};

 MBED_WEAK const PinMap PinMap_SPI_SCLK[] = {
    {PC_10, SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF6_SPI3)}, // - SD_SPI3_SCK
    {PE_12, SPI_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF5_SPI4)}, // FLASH_SPI4_SCK
    {NC,    NC,    0}
};

 MBED_WEAK const PinMap PinMap_SPI_SSEL[] = {
    {PA_15, SPI_3, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF6_SPI3)}, // - SD_SPI3_SSEL
    {PE_11, SPI_4, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF5_SPI4)}, // - FLASH_SPI4_SSEL
    {NC,    NC,    0}
};

 //*** CAN ***

 MBED_WEAK const PinMap PinMap_CAN_RD[] = {
    {PB_8,  CAN_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF9_CAN1)}, // - Grove4_SIG1
    {NC,    NC,    0}
};

 MBED_WEAK const PinMap PinMap_CAN_TD[] = {
    {PB_9,  CAN_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF9_CAN1)}, // - Grove4_SIG2
    {NC,    NC,    0}
};
