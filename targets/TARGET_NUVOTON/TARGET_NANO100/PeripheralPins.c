/* mbed Microcontroller Library
 * Copyright (c) 2015-2017 Nuvoton
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

//*** ADC ***

const PinMap PinMap_ADC[] = {
    {PA_0, ADC_0_0, SYS_PA_L_MFP_PA0_MFP_ADC_CH0},
    {PA_1, ADC_0_1, SYS_PA_L_MFP_PA1_MFP_ADC_CH1},
    {PA_2, ADC_0_2, SYS_PA_L_MFP_PA2_MFP_ADC_CH2},
    {PA_3, ADC_0_3, SYS_PA_L_MFP_PA3_MFP_ADC_CH3},
    {PA_4, ADC_0_4, SYS_PA_L_MFP_PA4_MFP_ADC_CH4},
    {PA_5, ADC_0_5, SYS_PA_L_MFP_PA5_MFP_ADC_CH5},
    {PA_6, ADC_0_6, SYS_PA_L_MFP_PA6_MFP_ADC_CH6},
    {PA_7, ADC_0_7, SYS_PA_L_MFP_PA7_MFP_ADC_CH7},
    {PD_0, ADC_0_8, SYS_PD_L_MFP_PD0_MFP_ADC_CH8},
    {PD_1, ADC_0_9, SYS_PD_L_MFP_PD1_MFP_ADC_CH9},
    {PD_2, ADC_0_10, SYS_PD_L_MFP_PD2_MFP_ADC_CH10},
    {PD_3, ADC_0_11, SYS_PD_L_MFP_PD3_MFP_ADC_CH11},
    
    {NC,   NC,    0}
};

//*** DAC ***

const PinMap PinMap_DAC[] = {
    {PC_6, DAC_0_0, SYS_PC_L_MFP_PC6_MFP_DA_OUT0},
    {PC_7, DAC_0_1, SYS_PC_L_MFP_PC7_MFP_DA_OUT1},

    {NC,   NC,    0}
};

//*** I2C ***

const PinMap PinMap_I2C_SDA[] = {
    {PA_8, I2C_0, SYS_PA_H_MFP_PA8_MFP_I2C0_SDA},
    {PA_10, I2C_1, SYS_PA_H_MFP_PA10_MFP_I2C1_SDA},
    {PA_12, I2C_0, SYS_PA_H_MFP_PA12_MFP_I2C0_SDA},
    {PA_4, I2C_0, SYS_PA_L_MFP_PA4_MFP_I2C0_SDA},
    {PC_8, I2C_1, SYS_PC_H_MFP_PC8_MFP_I2C1_SDA},
    {PC_12, I2C_0, SYS_PC_H_MFP_PC12_MFP_I2C0_SDA},
    {PF_4, I2C_0, SYS_PF_L_MFP_PF4_MFP_I2C0_SDA},
    
    {NC,    NC,    0}
};

const PinMap PinMap_I2C_SCL[] = {
    {PA_5, I2C_0, SYS_PA_L_MFP_PA5_MFP_I2C0_SCL},
    {PA_9, I2C_0, SYS_PA_H_MFP_PA9_MFP_I2C0_SCL},
    {PA_11, I2C_1, SYS_PA_H_MFP_PA11_MFP_I2C1_SCL},
    {PA_13, I2C_0, SYS_PA_H_MFP_PA13_MFP_I2C0_SCL},
    {PC_9, I2C_1, SYS_PC_H_MFP_PC9_MFP_I2C1_SCL},
    {PC_13, I2C_0, SYS_PC_H_MFP_PC13_MFP_I2C0_SCL},
    {PF_5, I2C_0, SYS_PF_L_MFP_PF5_MFP_I2C0_SCL},
    
    {NC,    NC,    0}
};

//*** PWM ***

const PinMap PinMap_PWM[] = {
    {PA_6, PWM_0_3, SYS_PA_L_MFP_PA6_MFP_PWM0_CH3},
    {PA_7, PWM_0_2, SYS_PA_L_MFP_PA7_MFP_PWM0_CH2},
    {PA_12, PWM_0_0, SYS_PA_H_MFP_PA12_MFP_PWM0_CH0},
    {PA_13, PWM_0_1, SYS_PA_H_MFP_PA13_MFP_PWM0_CH1},
    {PA_14, PWM_0_2, SYS_PA_H_MFP_PA14_MFP_PWM0_CH2},
    {PA_15, PWM_0_3, SYS_PA_H_MFP_PA15_MFP_PWM0_CH3},
    {PB_11, PWM_1_0, SYS_PB_H_MFP_PB11_MFP_PWM1_CH0},
    {PC_6, PWM_0_0, SYS_PC_L_MFP_PC6_MFP_PWM0_CH0},
    {PC_7, PWM_0_1, SYS_PC_L_MFP_PC7_MFP_PWM0_CH1},
    {PC_12, PWM_1_0, SYS_PC_H_MFP_PC12_MFP_PWM1_CH0},
    {PC_13, PWM_1_1, SYS_PC_H_MFP_PC13_MFP_PWM1_CH1},
    {PC_14, PWM_1_3, SYS_PC_H_MFP_PC14_MFP_PWM1_CH3},
    {PC_15, PWM_1_2, SYS_PC_H_MFP_PC15_MFP_PWM1_CH2},
    {PE_0, PWM_1_2, SYS_PE_L_MFP_PE0_MFP_PWM1_CH2},
    {PE_1, PWM_1_3, SYS_PE_L_MFP_PE1_MFP_PWM1_CH3},
    {PE_5, PWM_1_1, SYS_PE_L_MFP_PE5_MFP_PWM1_CH1},

    {NC,    NC,    0}
};

//*** SERIAL ***

const PinMap PinMap_UART_TX[] = {
    {PA_3, UART_1, SYS_PA_L_MFP_PA3_MFP_UART1_TX},
    {PA_15, UART_0, SYS_PA_H_MFP_PA15_MFP_UART0_TX},
    {PB_1, UART_0, SYS_PB_L_MFP_PB1_MFP_UART0_TX},
    {PB_5, UART_1, SYS_PB_L_MFP_PB5_MFP_UART1_TX},
    {PC_11, UART_1, SYS_PC_H_MFP_PC11_MFP_UART1_TX},
    {PD_1, UART_1, SYS_PD_L_MFP_PD1_MFP_UART1_TX},
    {PE_10, UART_1, SYS_PE_H_MFP_PE10_MFP_UART1_TX},
    
    {NC,    NC,     0}
};

const PinMap PinMap_UART_RX[] = {
    {PA_2, UART_1, SYS_PA_L_MFP_PA2_MFP_UART1_RX},
    {PA_14, UART_0, SYS_PA_H_MFP_PA14_MFP_UART0_RX},
    {PB_0, UART_0, SYS_PB_L_MFP_PB0_MFP_UART0_RX},
    {PB_4, UART_1, SYS_PB_L_MFP_PB4_MFP_UART1_RX},
    {PC_10, UART_1, SYS_PC_H_MFP_PC10_MFP_UART1_RX},
    {PD_0, UART_1, SYS_PD_L_MFP_PD0_MFP_UART1_RX},
    {PE_9, UART_1, SYS_PE_H_MFP_PE9_MFP_UART1_RX},
    
    {NC,    NC,     0}
};

const PinMap PinMap_UART_RTS[] = {
    {PB_2, UART_0, SYS_PB_L_MFP_PB2_MFP_UART0_RTS},
    {PB_6, UART_1, SYS_PB_L_MFP_PB6_MFP_UART1_RTS},
    {PD_2, UART_1, SYS_PD_L_MFP_PD2_MFP_UART1_RTS},
    {PE_11, UART_1, SYS_PE_H_MFP_PE11_MFP_UART1_RTS},
    
    {NC,    NC,     0}
};

const PinMap PinMap_UART_CTS[] = {
    {PB_3, UART_0, SYS_PB_L_MFP_PB3_MFP_UART0_CTS},
    {PB_7, UART_1, SYS_PB_L_MFP_PB7_MFP_UART1_CTS},
    {PD_3, UART_1, SYS_PD_L_MFP_PD3_MFP_UART1_CTS},
    {PE_12, UART_1, SYS_PE_H_MFP_PE12_MFP_UART1_CTS},
    
    {NC,    NC,     0}
};

//*** SPI ***

const PinMap PinMap_SPI_MOSI[] = {
    {PA_11, SPI_2, SYS_PA_H_MFP_PA11_MFP_SPI2_MOSI0},
    {PB_0, SPI_1, SYS_PB_L_MFP_PB0_MFP_SPI1_MOSI0},
    {PB_7, SPI_2, SYS_PB_L_MFP_PB7_MFP_SPI2_MOSI0},
    {PB_10, SPI_0, SYS_PB_H_MFP_PB10_MFP_SPI0_MOSI0},
    {PC_3, SPI_0, SYS_PC_L_MFP_PC3_MFP_SPI0_MOSI0},
    {PC_11, SPI_1, SYS_PC_H_MFP_PC11_MFP_SPI1_MOSI0},
    {PD_3, SPI_2, SYS_PD_L_MFP_PD3_MFP_SPI2_MOSI0},
    {PE_4, SPI_0, SYS_PE_L_MFP_PE4_MFP_SPI0_MOSI0},
    
    {NC,    NC,    0}
};

const PinMap PinMap_SPI_MISO[] = {
    {PA_10, SPI_2, SYS_PA_H_MFP_PA10_MFP_SPI2_MISO0},
    {PB_1, SPI_1, SYS_PB_L_MFP_PB1_MFP_SPI1_MISO0},
    {PB_6, SPI_2, SYS_PB_L_MFP_PB6_MFP_SPI2_MISO0},
    {PB_11, SPI_0, SYS_PB_H_MFP_PB11_MFP_SPI0_MISO0},
    {PC_2, SPI_0, SYS_PC_L_MFP_PC2_MFP_SPI0_MISO0},
    {PC_10, SPI_1, SYS_PC_H_MFP_PC10_MFP_SPI1_MISO0},
    {PD_2, SPI_2, SYS_PD_L_MFP_PD2_MFP_SPI2_MISO0},
    {PE_3, SPI_0, SYS_PE_L_MFP_PE3_MFP_SPI0_MISO0},
    
    {NC,    NC,    0}
};

const PinMap PinMap_SPI_SCLK[] = {
    {PA_9, SPI_2, SYS_PA_H_MFP_PA9_MFP_SPI2_SCLK},
    {PB_2, SPI_1, SYS_PB_L_MFP_PB2_MFP_SPI1_SCLK},
    {PB_5, SPI_2, SYS_PB_L_MFP_PB5_MFP_SPI2_SCLK},
    {PC_1, SPI_0, SYS_PC_L_MFP_PC1_MFP_SPI0_SCLK},
    {PC_9, SPI_1, SYS_PC_H_MFP_PC9_MFP_SPI1_SCLK},
    {PD_1, SPI_2, SYS_PD_L_MFP_PD1_MFP_SPI2_SCLK},
    {PE_2, SPI_0, SYS_PE_L_MFP_PE2_MFP_SPI0_SCLK},
    
    {NC,    NC,    0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {PA_8, SPI_2, SYS_PA_H_MFP_PA8_MFP_SPI2_SS0},
    {PB_3, SPI_1, SYS_PB_L_MFP_PB3_MFP_SPI1_SS0},
    {PB_4, SPI_2, SYS_PB_L_MFP_PB4_MFP_SPI2_SS0},
    {PB_9, SPI_1, SYS_PB_H_MFP_PB9_MFP_SPI1_SS1},
    {PB_10, SPI_0, SYS_PB_H_MFP_PB10_MFP_SPI0_SS1},
    {PB_14, SPI_2, SYS_PB_H_MFP_PB14_MFP_SPI2_SS1},
    {PC_0, SPI_0, SYS_PC_L_MFP_PC0_MFP_SPI0_SS0},
    {PC_8, SPI_1, SYS_PC_H_MFP_PC8_MFP_SPI1_SS0},
    {PD_0, SPI_2, SYS_PD_L_MFP_PD0_MFP_SPI2_SS0},
    {PE_1, SPI_0, SYS_PE_L_MFP_PE1_MFP_SPI0_SS0},
    
    {NC,    NC,    0}
};
