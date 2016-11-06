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
    /* Not connected */
    {PA17, ADC_0, NC}, /*NC in mux posiotn since it is a extra function and doesnt have a name*/
    {PA18, ADC_1, NC}, /*Pin is multiplexed to the extra function once the corresponding ADC channel is enabled*/
    {PA19, ADC_2, NC},
    {PA20, ADC_3, NC},
    {PB00, ADC_4, NC},
    {PB01, ADC_5, NC},
    {PB02, ADC_6, NC},
    {PB03, ADC_7, NC},
    /* Not connected */
    {NC  , NC   , NC}
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {PA03, UART_3, 0},
    {PA06, UART_2, 0},
    {PA10, UART_0, 0},
    {PA13, UART_5, 0},
    {PA28, UART_7, 1},
    {PB00, UART_6, 1},
    {PB03, UART_1, 0},
    {PB08, UART_4, 0},
    {PB10, UART_4, 0},
    {PB10, UART_6, 1},
    /* Not connected */
    {NC  , NC   , NC}
};

const PinMap PinMap_UART_RX[] = {
    {PA04, UART_3, 0},
    {PA05, UART_2, 0},
    {PA09, UART_0, 0},
    {PA12, UART_5, 0},
    {PA27, UART_7, 1},
    {PB01, UART_6, 1},
    {PB02, UART_1, 0},
    {PB09, UART_4, 0},
    {PB11, UART_4, 0},
    {PB11, UART_6, 1},
    /* Not connected */
    {NC  , NC   , NC}
};

const PinMap PinMap_UART_CTS[] = {
    {PA11, UART_5, 0},
    {PA16, UART_2, 0},
    {PA25, UART_0, 0},
    {PA28, UART_1, 0},
    {PA30, UART_7, 1},
    {PB08, UART_4, 1},
    {PB14, UART_3, 0},
    {PB14, UART_6, 1},
    /* Not connected */
    {NC  , NC   , NC}
};

const PinMap PinMap_UART_RTS[] = {
    {PA15, UART_2, 0},
    {PA26, UART_0, 0},
    {PA29, UART_1, 0},
    {PA31, UART_7, 1},
    {PB02, UART_5, 1},
    {PB09, UART_4, 1},
    {PB15, UART_3, 0},
    {PB15, UART_6, 1},
    /* Not connected */
    {NC  , NC   , NC}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {PB00, (int)SPI0, 0},
    {PA27, (int)SPI1, 0},
    {PA15, (int)SPI2, 1},
    {PA24, (int)SPI2, 1},
    {PB13, (int)SPI3, 0},
    {PB01, (int)SPI4, 0},
    {PA14, (int)SPI5, 0},
    {PB13, (int)SPI6, 1},
    {PA29, (int)SPI7, 1},
    /* Not connected */
    {NC  , (int)NC   , NC}
};

const PinMap PinMap_SPI_MOSI[] = {
    {PA10, (int)SPI0, 0},
    {PB03, (int)SPI1, 0},
    {PA06, (int)SPI2, 0},
    {PA03, (int)SPI3, 0},
    {PB08, (int)SPI4, 0},
    {PB10, (int)SPI4, 0},
    {PA13, (int)SPI5, 0},
    {PB00, (int)SPI6, 1},
    {PB10, (int)SPI6, 1},
    {PA28, (int)SPI7, 1},
    /* Not connected */
    {NC  , (int)NC   , NC}
};

const PinMap PinMap_SPI_MISO[] = {
    {PA09, (int)SPI0, 0},
    {PB02, (int)SPI1, 0},
    {PA05, (int)SPI2, 0},
    {PA04, (int)SPI3, 0},
    {PB09, (int)SPI4, 0},
    {PB11, (int)SPI4, 0},
    {PA12, (int)SPI5, 0},
    {PB01, (int)SPI6, 1},
    {PB11, (int)SPI6, 1},
    {PA27, (int)SPI7, 1},
    /* Not connected */
    {NC  , (int)NC   , NC}
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    {PA10, (int)TWI0, 0},
    {PB03, (int)TWI1, 0},
    {PA06, (int)TWI2, 0},
    {PA03, (int)TWI3, 0},
    {PB08, (int)TWI4, 0},
    {PB10, (int)TWI4, 0},
    {PB10, (int)TWI6, 1},
    {PA13, (int)TWI5, 0},
    {PB00, (int)TWI6, 1},
    {PB10, (int)TWI6, 1},
    {PA28, (int)TWI7, 1},
    /* Not connected */
    {NC  , NC   , NC}
};

const PinMap PinMap_I2C_SCL[] = {
    {PA09, (int)TWI0, 0},
    {PB02, (int)TWI1, 0},
    {PA05, (int)TWI2, 0},
    {PA04, (int)TWI3, 0},
    {PB09, (int)TWI4, 0},
    {PB11, (int)TWI4, 0},
    {PA12, (int)TWI5, 0},
    {PB01, (int)TWI6, 1},
    {PB11, (int)TWI6, 1},
    {PA27, (int)TWI7, 1},
    /* Not connected */
    {NC  , NC   , NC}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
    {PA00, (int)TC0, 1},
    {PA23, (int)TC0, 1},
    {PA21, (int)TC0, 0},
    {PA01, (int)TC0, 1},
    {PA16, (int)TC0, 1},
    {PA22, (int)TC0, 0},
    /* Not connected */
    {NC  , NC   , NC}
};

/************PWM***************/
const PinMap PinMap_PWM_IO_Line[] = {
    {PA00, 0, 0},
    {PA23, 1, 0},
    {PA21, 2, 0},
    {PA01, 0, 1},
    {PA16, 1, 1},
    {PA22, 2, 1},
    /* Not connected */
    {NC  , NC   , NC}
};





