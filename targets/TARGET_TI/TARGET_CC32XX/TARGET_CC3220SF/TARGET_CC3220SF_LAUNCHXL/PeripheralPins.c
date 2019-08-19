/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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

/************GPIO***************/
const PinMap PinMap_GPIO[] = {
    {PIN_50, CC3220SF_GPIOA0_BASE, 0}, //GPIO_00 (PM/Dig Mux)
    {PIN_55, CC3220SF_GPIOA0_BASE, 1}, //GPIO_01
    {PIN_57, CC3220SF_GPIOA0_BASE, 2}, //GPIO_02 (Dig/ADC Mux)
    {PIN_58, CC3220SF_GPIOA0_BASE, 3}, //GPIO_03 (Dig/ADC Mux)
    {PIN_59, CC3220SF_GPIOA0_BASE, 4}, //GPIO_04 (Dig/ADC Mux)
    {PIN_60, CC3220SF_GPIOA0_BASE, 5}, //GPIO_05 (Dig/ADC Mux)
    {PIN_61, CC3220SF_GPIOA0_BASE, 6}, //GPIO_06
    {PIN_62, CC3220SF_GPIOA0_BASE, 7}, //GPIO_07

    {PIN_63, CC3220SF_GPIOA1_BASE, 8}, //GPIO_08
    {PIN_64, CC3220SF_GPIOA1_BASE, 9}, //GPIO_09
    {PIN_01, CC3220SF_GPIOA1_BASE, 10}, //GPIO_10
    {PIN_02, CC3220SF_GPIOA1_BASE, 11}, //GPIO_11
    {PIN_03, CC3220SF_GPIOA1_BASE, 12}, //GPIO_12
    {PIN_04, CC3220SF_GPIOA1_BASE, 13}, //GPIO_13
    {PIN_05, CC3220SF_GPIOA1_BASE, 14}, //GPIO_14
    {PIN_06, CC3220SF_GPIOA1_BASE, 15}, //GPIO_15

    {PIN_07, CC3220SF_GPIOA2_BASE, 16}, //GPIO_16
    {PIN_08, CC3220SF_GPIOA2_BASE, 17}, //GPIO_17
    //this is only here for reference
    {PIN_XX, CC3220SF_GPIOA2_BASE, 18}, //GPIO_18 (Reserved) No package pin associate with this GPIO
    {PIN_XX, CC3220SF_GPIOA2_BASE, 19}, //GPIO_19 (Reserved) No package pin associate with this GPIO
    {PIN_XX, CC3220SF_GPIOA2_BASE, 20}, //GPIO_20 (Reserved) No package pin associate with this GPIO
    {PIN_XX, CC3220SF_GPIOA2_BASE, 21}, //GPIO_21 (Reserved) No package pin associate with this GPIO
    {PIN_15, CC3220SF_GPIOA2_BASE, 22}, //GPIO_22
    {PIN_16, CC3220SF_GPIOA2_BASE, 23}, //GPIO_23

    {PIN_17, CC3220SF_GPIOA3_BASE, 24}, //GPIO_24
    // pin 21 is one of three that must have a passive pullup or pulldown resistor
    // on board to configure the chip hardware power-up mode. Because of this,
    // if this pin is used for digital functions, it must be output only.
    {PIN_21, CC3220SF_GPIOA3_BASE, 25}, //GPIO_25
    //this is only here for reference
    {PIN_XX, CC3220SF_GPIOA3_BASE, 26}, //GPIO_26 (Restricted Use; Antenna Selection 1 Only) No package pin associate with this GPIO
    {PIN_XX, CC3220SF_GPIOA3_BASE, 27}, //GPIO_27 (Restricted Use; Antenna Selection 1 Only) No package pin associate with this GPIO
    {PIN_18, CC3220SF_GPIOA3_BASE, 28}, //GPIO_28
    {PIN_20, CC3220SF_GPIOA3_BASE, 29}, //GPIO_29
    {PIN_53, CC3220SF_GPIOA3_BASE, 30}, //GPIO_30 (PM/Dig Mux)
    {PIN_45, CC3220SF_GPIOA3_BASE, 31}, //GPIO_31 (PM/Dig Mux)
    {NC, NC, 0}
};

/************PWM***************/
#if DEVICE_PWMOUT
const PinMap PinMap_PWM[] = {
    {PIN_01, PWM_1, 3},
    {PIN_02, PWM_2, 3},
    {PIN_17, PWM_3, 5},
    {PIN_19, PWM_4, 8},
    {PIN_21, PWM_5, 9},
    {PIN_64, PWM_6, 3},
    {NC, NC, 0}
};
#endif

/************UART***************/
#if DEVICE_SERIAL
const PinMap PinMap_UART_TX[] = {
    {PIN_01, UART_1, 7},
    {PIN_03, UART_0, 7},
    {PIN_07, UART_1, 5},
    {PIN_16, UART_1, 2},
    {PIN_53, UART_0, 9},
    {PIN_55, UART_0, 3},
    {PIN_55, UART_1, 6},
    {PIN_58, UART_1, 6},
    {PIN_62, UART_0, 11},
    {NC, NC, 0}
};

const PinMap PinMap_UART_RX[] = {
    {PIN_02, UART_1, 7},
    {PIN_04, UART_0, 7},
    {PIN_08, UART_1, 5},
    {PIN_17, UART_1, 2},
    {PIN_45, UART_0, 9},
    {PIN_45, UART_1, 2},
    {PIN_57, UART_0, 3},
    {PIN_57, UART_1, 6},
    {PIN_59, UART_1, 6},
    {NC, NC, 0}
};
#if DEVICE_SERIAL_FC
const PinMap PinMap_UART_RTS[] = {
    {PIN_50, UART_0, 3},
    {PIN_50, UART_1, 10},
    {PIN_52, UART_0, 6},
    {PIN_61, UART_0, 5},
    {PIN_62, UART_0, 10},
    {PIN_62, UART_1, 3},
    {NC, NC, 0}
};

const PinMap PinMap_UART_CTS[] = {
    {PIN_50, UART_0, 12},
    {PIN_61, UART_0, 6},
    {PIN_61, UART_1, 3},
    {NC, NC, 0}
};
#endif
#endif

#if DEVICE_ANALOGIN
const PinMap PinMap_ADC[] = {
    {PIN_57, ADC0_0, 0},
    {PIN_58, ADC0_1, 0},
    {PIN_59, ADC0_2, 0},
    {PIN_60, ADC0_3, 0},
    {NC, NC, 0}
};
#endif

#if DEVICE_SPI
/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {PIN_05, GSPI, 7},
    {PIN_45, GSPI, 7},
    {NC, NC, 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {PIN_07, GSPI, 7},
    {PIN_52, GSPI, 7},
    {NC, NC, 0}
};

const PinMap PinMap_SPI_MISO[] = {
    {PIN_06, GSPI, 7},
    {PIN_53, GSPI, 7},
    {NC, NC, 0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {PIN_08, GSPI, 7},
    {PIN_50, GSPI, 7},
    {NC, NC, 0}
};
#endif
