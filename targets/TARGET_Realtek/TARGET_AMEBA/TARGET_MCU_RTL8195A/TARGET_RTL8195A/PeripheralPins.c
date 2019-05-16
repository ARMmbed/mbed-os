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

// for spi_api.c
const PinMap PinMap_SSI_MOSI[] = {
    {PE_2,  RTL_PIN_PERI(SPI0, 0, S0), RTL_PIN_FUNC(SPI0, S0)},
    {PC_2,  RTL_PIN_PERI(SPI0, 0, S1), RTL_PIN_FUNC(SPI0, S1)},
    {PA_1,  RTL_PIN_PERI(SPI1, 1, S0), RTL_PIN_FUNC(SPI1, S0)},
    {PB_6,  RTL_PIN_PERI(SPI1, 1, S1), RTL_PIN_FUNC(SPI1, S1)},
    {PD_6,  RTL_PIN_PERI(SPI1, 1, S2), RTL_PIN_FUNC(SPI1, S2)},
    {PG_2,  RTL_PIN_PERI(SPI2, 2, S0), RTL_PIN_FUNC(SPI2, S0)},
    {PE_6,  RTL_PIN_PERI(SPI2, 2, S1), RTL_PIN_FUNC(SPI2, S1)},
    {PD_2,  RTL_PIN_PERI(SPI2, 2, S2), RTL_PIN_FUNC(SPI2, S2)},
    {NC,    NC,     0}
};

const PinMap PinMap_SSI_MISO[] = {
    {PE_3,  RTL_PIN_PERI(SPI0, 0, S0), RTL_PIN_FUNC(SPI0, S0)},
    {PC_3,  RTL_PIN_PERI(SPI0, 0, S1), RTL_PIN_FUNC(SPI0, S1)},
    {PA_0,  RTL_PIN_PERI(SPI1, 1, S0), RTL_PIN_FUNC(SPI1, S0)},
    {PB_7,  RTL_PIN_PERI(SPI1, 1, S1), RTL_PIN_FUNC(SPI1, S1)},
    {PD_7,  RTL_PIN_PERI(SPI1, 1, S2), RTL_PIN_FUNC(SPI1, S2)},
    {PG_3,  RTL_PIN_PERI(SPI2, 2, S0), RTL_PIN_FUNC(SPI2, S0)},
    {PE_7,  RTL_PIN_PERI(SPI2, 2, S1), RTL_PIN_FUNC(SPI2, S1)},
    {PD_3,  RTL_PIN_PERI(SPI2, 2, S2), RTL_PIN_FUNC(SPI2, S2)},
    {NC,    NC,     0}
};

// For testing only
const PinMap PinMap_SSI_SCLK[] = {
    {PC_1,  RTL_PIN_PERI(SPI0, 0, S1), /* Unused */ 0},
    {PA_2,  RTL_PIN_PERI(SPI1, 1, S0), /* Unused */ 0},
    {PA_2,  RTL_PIN_PERI(SPI1, 1, S1), /* Unused */ 0},
    {PA_2,  RTL_PIN_PERI(SPI1, 1, S2), /* Unused */ 0},
    {PD_5,  RTL_PIN_PERI(SPI1, 1, S0), /* Unused */ 0},
    {PD_5,  RTL_PIN_PERI(SPI1, 1, S1), /* Unused */ 0},
    {PD_5,  RTL_PIN_PERI(SPI1, 1, S2), /* Unused */ 0},

    {NC,    NC,     0}
};

// For testing only
const PinMap PinMap_SSI_SSEL[] = {
    {PE_5,  RTL_PIN_PERI(SPI0, 0, S2), /* Unused */ 0},
    {PC_5,  RTL_PIN_PERI(SPI0, 0, S2), /* Unused */ 0},
    {PC_4,  RTL_PIN_PERI(SPI0, 0, S1), /* Unused */ 0},
    {PC_0,  RTL_PIN_PERI(SPI0, 0, S0), /* Unused */ 0},
    {PA_4,  RTL_PIN_PERI(SPI1, 1, S0), /* Unused */ 0},
    {PD_4,  RTL_PIN_PERI(SPI1, 1, S0), /* Unused */ 0},

    {NC,    NC,     0}
};

// for serial_api.c
const PinMap PinMap_UART_TX[] = {
    {PC_3,  RTL_PIN_PERI(UART0, 0, S0), RTL_PIN_FUNC(UART0, S0)},
    {PE_0,  RTL_PIN_PERI(UART0, 0, S1), RTL_PIN_FUNC(UART0, S1)},
    {PA_7,  RTL_PIN_PERI(UART0, 0, S2), RTL_PIN_FUNC(UART0, S2)},
    {PD_3,  RTL_PIN_PERI(UART1, 1, S0), RTL_PIN_FUNC(UART1, S0)},
    {PE_4,  RTL_PIN_PERI(UART1, 1, S1), RTL_PIN_FUNC(UART1, S1)},
    {PB_5,  RTL_PIN_PERI(UART1, 1, S2), RTL_PIN_FUNC(UART1, S2)},
    {PA_4,  RTL_PIN_PERI(UART2, 2, S0), RTL_PIN_FUNC(UART2, S0)},
    {PC_9,  RTL_PIN_PERI(UART2, 2, S1), RTL_PIN_FUNC(UART2, S1)},
    {PD_7,  RTL_PIN_PERI(UART2, 2, S2), RTL_PIN_FUNC(UART2, S2)},
    {PB_0,  RTL_PIN_PERI(LOG_UART, 3, S0), RTL_PIN_FUNC(LOG_UART, S0)},
    {NC,    NC,     0}
};

const PinMap PinMap_UART_RX[] = {
    {PC_0,  RTL_PIN_PERI(UART0, 0, S0), RTL_PIN_FUNC(UART0, S0)},
    {PE_3,  RTL_PIN_PERI(UART0, 0, S1), RTL_PIN_FUNC(UART0, S1)},
    {PA_6,  RTL_PIN_PERI(UART0, 0, S2), RTL_PIN_FUNC(UART0, S2)},
    {PD_0,  RTL_PIN_PERI(UART1, 1, S0), RTL_PIN_FUNC(UART1, S0)},
    {PE_7,  RTL_PIN_PERI(UART1, 1, S1), RTL_PIN_FUNC(UART1, S1)},
    {PB_4,  RTL_PIN_PERI(UART1, 1, S2), RTL_PIN_FUNC(UART1, S2)},
    {PA_0,  RTL_PIN_PERI(UART2, 2, S0), RTL_PIN_FUNC(UART2, S0)},
    {PC_6,  RTL_PIN_PERI(UART2, 2, S1), RTL_PIN_FUNC(UART2, S1)},
    {PD_4,  RTL_PIN_PERI(UART2, 2, S2), RTL_PIN_FUNC(UART2, S2)},
    {PB_1,  RTL_PIN_PERI(LOG_UART, 3, S0), RTL_PIN_FUNC(LOG_UART, S0)},
    {NC,    NC,     0}
};

// for pwmout_api.c
const PinMap PinMap_PWM[] = {
    {PB_4,  RTL_PIN_PERI(PWM0, 0, S0), RTL_PIN_FUNC(PWM0, S0)},
    {PB_5,  RTL_PIN_PERI(PWM1, 1, S0), RTL_PIN_FUNC(PWM1, S0)},
    {PB_6,  RTL_PIN_PERI(PWM2, 2, S0), RTL_PIN_FUNC(PWM2, S0)},
    {PB_7,  RTL_PIN_PERI(PWM3, 3, S0), RTL_PIN_FUNC(PWM3, S0)},

    {PC_0,  RTL_PIN_PERI(PWM0, 0, S1), RTL_PIN_FUNC(PWM0, S1)},
    {PC_1,  RTL_PIN_PERI(PWM1, 1, S1), RTL_PIN_FUNC(PWM1, S1)},
    {PC_2,  RTL_PIN_PERI(PWM2, 2, S1), RTL_PIN_FUNC(PWM2, S1)},
    {PC_3,  RTL_PIN_PERI(PWM3, 3, S1), RTL_PIN_FUNC(PWM3, S1)},

    {PD_3,  RTL_PIN_PERI(PWM0, 0, S2), RTL_PIN_FUNC(PWM0, S2)},
    {PD_4,  RTL_PIN_PERI(PWM1, 1, S2), RTL_PIN_FUNC(PWM1, S2)},
    {PD_5,  RTL_PIN_PERI(PWM2, 2, S2), RTL_PIN_FUNC(PWM2, S2)},
    {PD_6,  RTL_PIN_PERI(PWM3, 3, S2), RTL_PIN_FUNC(PWM3, S2)},

    {PE_0,  RTL_PIN_PERI(PWM0, 0, S3), RTL_PIN_FUNC(PWM0, S3)},
    {PE_1,  RTL_PIN_PERI(PWM1, 1, S3), RTL_PIN_FUNC(PWM1, S3)},
    {PE_2,  RTL_PIN_PERI(PWM2, 2, S3), RTL_PIN_FUNC(PWM2, S3)},
    {PE_3,  RTL_PIN_PERI(PWM3, 3, S3), RTL_PIN_FUNC(PWM3, S3)},

    {NC,    NC,     0}
};

// for i2c_api.c
const PinMap PinMap_I2C_SDA[] = {
    {PD_4,  RTL_PIN_PERI(I2C0, 0, S0), RTL_PIN_FUNC(I2C0, S0)},
    {PH_1,  RTL_PIN_PERI(I2C0, 0, S1), RTL_PIN_FUNC(I2C0, S1)},
    {PC_8,  RTL_PIN_PERI(I2C0, 0, S2), RTL_PIN_FUNC(I2C0, S2)},
    {PE_7,  RTL_PIN_PERI(I2C0, 0, S3), RTL_PIN_FUNC(I2C0, S3)},
    
    {PC_4,  RTL_PIN_PERI(I2C1, 1, S0), RTL_PIN_FUNC(I2C1, S0)},
    {PH_3,  RTL_PIN_PERI(I2C1, 1, S1), RTL_PIN_FUNC(I2C1, S1)},
    {PD_7,  RTL_PIN_PERI(I2C1, 1, S2), RTL_PIN_FUNC(I2C1, S2)},

    {PB_7,  RTL_PIN_PERI(I2C2, 2, S0), RTL_PIN_FUNC(I2C2, S0)},
    {PE_1,  RTL_PIN_PERI(I2C2, 2, S1), RTL_PIN_FUNC(I2C2, S1)},
    {PC_7,  RTL_PIN_PERI(I2C2, 2, S2), RTL_PIN_FUNC(I2C2, S2)},

    {PB_3,  RTL_PIN_PERI(I2C3, 3, S0), RTL_PIN_FUNC(I2C3, S0)},
    {PE_3,  RTL_PIN_PERI(I2C3, 3, S1), RTL_PIN_FUNC(I2C3, S1)},
    {PE_5,  RTL_PIN_PERI(I2C3, 3, S2), RTL_PIN_FUNC(I2C3, S2)},
    {PD_9,  RTL_PIN_PERI(I2C3, 3, S3), RTL_PIN_FUNC(I2C3, S3)},

    {NC,    NC,     0}
};

const PinMap PinMap_I2C_SCL[] = {
    {PD_5,  RTL_PIN_PERI(I2C0, 0, S0), RTL_PIN_FUNC(I2C0, S0)},
    {PH_0,  RTL_PIN_PERI(I2C0, 0, S1), RTL_PIN_FUNC(I2C0, S1)},
    {PC_9,  RTL_PIN_PERI(I2C0, 0, S2), RTL_PIN_FUNC(I2C0, S2)},
    {PE_6,  RTL_PIN_PERI(I2C0, 0, S3), RTL_PIN_FUNC(I2C0, S3)},

    {PC_5,  RTL_PIN_PERI(I2C1, 1, S0), RTL_PIN_FUNC(I2C1, S0)},
    {PH_2,  RTL_PIN_PERI(I2C1, 1, S1), RTL_PIN_FUNC(I2C1, S1)},
    {PD_6,  RTL_PIN_PERI(I2C1, 1, S2), RTL_PIN_FUNC(I2C1, S2)},

    {PB_6,  RTL_PIN_PERI(I2C2, 2, S0), RTL_PIN_FUNC(I2C2, S0)},
    {PE_0,  RTL_PIN_PERI(I2C2, 2, S1), RTL_PIN_FUNC(I2C2, S1)},
    {PC_6,  RTL_PIN_PERI(I2C2, 2, S2), RTL_PIN_FUNC(I2C2, S2)},
    
    {PB_2,  RTL_PIN_PERI(I2C3, 3, S0), RTL_PIN_FUNC(I2C3, S0)},
    {PE_2,  RTL_PIN_PERI(I2C3, 3, S1), RTL_PIN_FUNC(I2C3, S1)},
    {PE_4,  RTL_PIN_PERI(I2C3, 3, S2), RTL_PIN_FUNC(I2C3, S2)},
    {PD_8,  RTL_PIN_PERI(I2C3, 3, S3), RTL_PIN_FUNC(I2C3, S3)},

    {NC,    NC,     0}
};

// For testing only
const PinMap PinMap_ADC[] = {
    {A1,    /* Unused */ 0, /* Unused */ 0},
    {A2,    /* Unused */ 0, /* Unused */ 0},

    {NC,    NC,     0}
};

// For testing only
const PinMap PinMap_DAC[] = {
    {DA_0,  /* Unused */ 0, /* Unused */ 0},
    {DA_1,  /* Unused */ 0, /* Unused */ 0},

    {NC,    NC,     0}
};
