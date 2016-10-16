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

/*todo: determine how function argument is used */

#include "PeripheralPins.h"

/************ADC***************/
const PinMap PinMap_ADC[] = {
    {A0,  ADC_0, 0},
    {A1,  ADC_0, 0},
    {A2,  ADC_0, 0},
    {A3,  ADC_0, 0},
    {NC   , NC       , 0}
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    {GPIO3,  I2C_0, 5},
    {GPIO4,  I2C_0, 5},
    {GPIO10, I2C_0, 5},
    {GPIO12, I2C_0, 5},
    {GPIO15, I2C_1, 5},
    {GPIO16, I2C_1, 5},
    {NC   , NC   , 0}
};

const PinMap PinMap_I2C_SCL[] = {
    {GPIO2,  I2C_0, 5},
    {GPIO5,  I2C_0, 5},
    {GPIO11, I2C_0, 5},
    {GPIO13, I2C_0, 5},
    {GPIO14, I2C_1, 5},
    {GPIO17, I2C_1, 5},
    {NC   , NC   , 0}
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {GPIO0, UART_0, 7},
    {GPIO8, UART_1, 7},
    {NC  ,  NC    , 0}
};

const PinMap PinMap_UART_RX[] = {
    {GPIO1, UART_0, 7},
    {GPIO9, UART_1, 7},
    {NC  ,  NC    , 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    /*todo: other pins are possible, need to add */
    {SPI1_SCLK_2, SPI_0, 6},
    {SPI1_SCLK_3, SPI_0, 6},
    {SPI2_SCLK,   SPI_1, 6},
    {NC,          NC,    0}
};

const PinMap PinMap_SPI_MOSI[] = {
    /*todo: other pins are possible, need to add */
    {SPI1_SDATAO_2, SPI_0, 6},
    {SPI1_SDATAO_3, SPI_0, 6},
    {SPI2_SDATAO,   SPI_1, 6},
    {NC,            NC,    0}
};

const PinMap PinMap_SPI_MISO[] = {
    /*todo: other pins are possible, need to add */
    {SPI1_SDATAI_2, SPI_0, 6},
    {SPI1_SDATAI_3, SPI_0, 6},
    {SPI2_SDATAI,   SPI_1, 6},
    {NC,            NC,    0}
};

const PinMap PinMap_SPI_SSEL[] = {
    /*todo: other pins are possible, need to add */
    /* TODO what about SSNO */
    {SPI1_SSNO0_1, SPI_0, 6},
    {SPI1_SSNO1_1, SPI_0, 6},
    {SPI1_SSNO2_1, SPI_0, 6},
    {SPI1_SSNO3_1, SPI_0, 6},
    {SPI1_SSNI_2,  SPI_0, 6},
    {SPI1_SSNO0_2, SPI_0, 6},
    {SPI1_SSNO1_2, SPI_0, 6},
    {SPI1_SSNO2_2, SPI_0, 6},
    {SPI2_SSNI,    SPI_1, 6},
    {SPI2_SSNO0,   SPI_1, 6},
    {NC,           NC,    0}
};


const PinMap PinMap_PWM[] = {

    {GPIO6 , PWM_0 , 4},
    {GPIO7 , PWM_0 , 4},
    {GPIO9 , PWM_0 , 4},
    {GPIO12 , PWM_0 , 4},
    {GPIO13 , PWM_0 , 4},
    {NC   , NC    , 0}
};
