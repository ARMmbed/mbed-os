
/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

/************IRQ***************/
enum {
    IRQ0,IRQ1,
    IRQ2,IRQ3,
    IRQ4,IRQ5,
    IRQ6,IRQ7,
} IRQNo;
const PinMap PinMap_IRQ[] = {
    {P6_7,  IRQ0, 4},
    {P1_8,  IRQ0, 2},
    {P1_4,  IRQ0, 2},
    {P6_8,  IRQ1, 4},
    {P1_9,  IRQ1, 2},
    {P1_5,  IRQ1, 2},
    {P2_9,  IRQ1, 4},
    {P6_9,  IRQ2, 4},
    {P1_10, IRQ2, 2},
    {P1_6,  IRQ2, 2},
    {P2_7,  IRQ2, 4},
    {P5_8,  IRQ3, 4},
    {P6_10, IRQ3, 4},
    {P7_3,  IRQ3, 4},
    {P7_8,  IRQ3, 4},
    {P1_11, IRQ3, 2},
    {P1_7,  IRQ3, 2},
    {P5_9,  IRQ4, 4},
    {P7_2,  IRQ4, 4},
    {P0_0,  IRQ4, 3},
    {P1_12, IRQ4, 2},
    {P1_0,  IRQ4, 2},
    {P5_10, IRQ5, 4},
    {P7_7,  IRQ5, 4},
    {P7_9,  IRQ5, 4},
    {P2_2,  IRQ5, 4},
    {P1_13, IRQ5, 2},
    {P1_1,  IRQ5, 2},
    {P5_11, IRQ6, 4},
    {P8_6,  IRQ6, 3},
    {P7_6,  IRQ6, 4},
    {P2_1,  IRQ6, 4},
    {P1_14, IRQ6, 2},
    {P1_2,  IRQ6, 2},
    {P8_7,  IRQ7, 3},
    {P2_0,  IRQ7, 4},
    {P0_2,  IRQ7, 3},
    {P1_15, IRQ7, 2},
    {P1_3,  IRQ7, 2},
    {NC,    NC,   0}
};

/************PINMAP***************/
const PinFunc PIPC_0_tbl[] = {
//   pin      func     pm
    {P2_8   , 3      , -1}, /* TIOC0A */
    {P8_4   , 5      , -1}, /* TIOC0A */ /* for 208QFP */
    {P8_5   , 5      , -1}, /* TIOC0B */ /* for 208QFP */
    {P8_6   , 5      , -1}, /* TIOC0C */ /* for 208QFP */
    {P2_2   , 3      , -1}, /* TIOC0C */
    {P8_7   , 5      , -1}, /* TIOC0D */ /* for 208QFP */
    {P2_3   , 3      , -1}, /* TIOC0D */
    {P7_9   , 6      , -1}, /* TIOC1A */
    {P9_2   , 5      , -1}, /* TIOC1A */ /* for 208QFP */
    {P2_7   , 3      , -1}, /* TIOC1A */
    {P9_3   , 5      , -1}, /* TIOC1B */ /* for 208QFP */
    {P4_0   , 3      , -1}, /* TIOC1B */
    {P5_14  , 4      , -1}, /* TIOC2A */
    {P7_0   , 5      , -1}, /* TIOC2A */
    {P9_4   , 5      , -1}, /* TIOC2A */
    {P2_6   , 3      , -1}, /* TIOC2A */
    {P9_5   , 5      , -1}, /* TIOC2B */ /* for 208QFP */
    {P4_1   , 3      , -1}, /* TIOC2B */
    {P6_7   , 5      , -1}, /* TIOC3A */
    {P2_5   , 3      , -1}, /* TIOC3A */
    {P3_11  , 3      , -1}, /* TIOC3A */
    {P6_8   , 5      , -1}, /* TIOC3B */
    {P3_10  , 3      , -1}, /* TIOC3B */
    {P6_9   , 5      , -1}, /* TIOC3C */
    {P3_12  , 3      , -1}, /* TIOC3C */
    {P6_10  , 5      , -1}, /* TIOC3D */
    {P3_13  , 3      , -1}, /* TIOC3D */
    {P5_8   , 3      , -1}, /* TIOC4A */
    {P2_4   , 3      , -1}, /* TIOC4A */
    {P5_9   , 3      , -1}, /* TIOC4B */
    {P5_10  , 3      , -1}, /* TIOC4C */
    {P5_11  , 3      , -1}, /* TIOC4D */
    {P6_10  , 3      , 0 }, /* SSITxD0 */
    {P5_6   , 3      , 0 }, /* SSITxD1 */
    {P7_10  , 2      , 0 }, /* SSITxD1 */
    {P8_2   , 3      , 0 }, /* SSITxD1 */ /* for 208QFP */
    {P6_6   , 3      , 0 }, /* SSITxD3 */
    {P2_8   , 2      , 0 }, /* SSITxD3 */
    {P6_0   , 5      , 0 }, /* WDTOVF */
    {NC     , 0      , -1}
};

/************ADC***************/
const PinMap PinMap_ADC[] = {
    {P1_8,  AN0, 1},
    {P1_9,  AN1, 1},
    {P1_10, AN2, 1},
    {P1_11, AN3, 1},
    {P1_12, AN4, 1},
    {P1_13, AN5, 1},
    {P1_14, AN6, 1},
    {P1_15, AN7, 1},
    {NC   , NC , 0}
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    {P1_1  , I2C_0, 1},
    {P1_3  , I2C_1, 1},
    {P1_5  , I2C_2, 1},
    {P1_7  , I2C_3, 1},
    {NC    , NC   , 0}
};

const PinMap PinMap_I2C_SCL[] = {
    {P1_0  , I2C_0, 1},
    {P1_2  , I2C_1, 1},
    {P1_4  , I2C_2, 1},
    {P1_6  , I2C_3, 1},
    {NC    , NC,    0}
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {P6_15 , UART0, 5},
    {P7_5  , UART0, 4},
    {P7_8  , UART0, 5},
    {P9_5  , UART0, 4}, /* for 208QFP */
    {P9_0  , UART1, 4}, /* for 208QFP */
    {P3_15 , UART1, 5},
    {P5_7  , UART2, 5},
    {P8_10 , UART2, 4}, /* for 208QFP */
    {P7_1  , UART2, 4},
    {P7_11 , UART3, 5},
    {P3_9  , UART3, 5},
    {P8_2  , UART3, 4},
    {P5_15 , UART4, 3},
    {P8_13 , UART4, 4},
    {NC    , NC   , 0}
};

const PinMap PinMap_UART_RX[] = {
    {P2_15 , UART0, 6},
    {P0_0  , UART0, 2},
    {P1_8  , UART0, 3},
    {P9_4  , UART0, 4}, /* for 208QFP */
    {P8_15 , UART1, 4}, /* for 208QFP */
    {P0_2  , UART1, 2},
    {P1_9  , UART1, 3},
    {P8_9  , UART2, 4}, /* for 208QFP */
    {P0_1  , UART2, 2},
    {P1_10 , UART2, 3},
    {P1_7  , UART2, 3},
    {P7_10 , UART3, 5},
    {P0_3  , UART3, 2},
    {P1_11 , UART3, 3},
    {P3_10 , UART3, 5},
    {P8_1  , UART3, 4}, /* for 208QFP */
    {P5_14 , UART4, 3},
    {P8_12 , UART4, 4}, /* for 208QFP */
    {P8_12 , UART4, 4}, /* for 208QFP */
    {NC    , NC   , 0}
};

const PinMap PinMap_UART_CTS[] = {
    {P6_13 , UART0, 5},
    {P9_1  , UART0, 4}, /* for 208QFP */
    {P8_6  , UART1, 4}, /* for 208QFP */
    {P4_0  , UART1, 5},
    {P7_6  , UART2, 5},
    {P8_4  , UART2, 4}, /* for 208QFP */
    {NC    , NC   , 0}
};

const PinMap PinMap_UART_RTS[] = {
    {P6_12 , UART0, 5},
    {P9_2  , UART0, 4}, /* for 208QFP */
    {P8_7  , UART1, 4}, /* for 208QFP */
    {P4_1  , UART1, 5},
    {P7_7  , UART2, 5},
    {P8_5  , UART2, 4}, /* for 208QFP */
    {NC    , NC   , 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {P6_0  , SPI_0, 3},
    {P6_12 , SPI_1, 3},
    {P5_4  , SPI_2, 2},
    {P9_2  , SPI_2, 1}, /* for 208QFP */
    {NC    , NC   , 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {P6_2  , SPI_0, 3},
    {P6_14 , SPI_1, 3},
    {P5_6  , SPI_2, 2},
    {P9_4  , SPI_2, 1}, /* for 208QFP */
    {NC    , NC   , 0}
};

const PinMap PinMap_SPI_MISO[] = {
    {P6_3  , SPI_0, 3},
    {P6_15 , SPI_1, 3},
    {P5_7  , SPI_2, 2},
    {P9_5  , SPI_2, 1}, /* for 208QFP */
    {NC    , NC   , 0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {P6_1  , SPI_0, 3},
    {P6_13 , SPI_1, 3},
    {P5_5  , SPI_2, 2},
    {P9_3  , SPI_2, 1}, /* for 208QFP */
    {NC    , NC   , 0}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
    {P2_8  , PWM_TIOC0A, 3},
    {P8_4  , PWM_TIOC0A, 5}, /* for 208QFP */
    {P8_6  , PWM_TIOC0C, 5}, /* for 208QFP */
    {P2_2  , PWM_TIOC0C, 3},
    {P7_9  , PWM_TIOC1A, 6},
    {P9_2  , PWM_TIOC1A, 5}, /* for 208QFP */
    {P2_7  , PWM_TIOC1A, 3},
    {P9_3  , PWM_TIOC1B, 5}, /* for 208QFP */
    {P4_0  , PWM_TIOC1B, 3},
    {P2_6  , PWM_TIOC2A, 3},
    {P5_14 , PWM_TIOC2A, 4},
    {P7_0  , PWM_TIOC2A, 5},
    {P9_4  , PWM_TIOC2A, 5}, /* for 208QFP */
    {P9_5  , PWM_TIOC2B, 5}, /* for 208QFP */
    {P4_1  , PWM_TIOC2B, 3},
    {P6_7  , PWM_TIOC3A, 5},
    {P2_5  , PWM_TIOC3A, 3},
    {P3_11 , PWM_TIOC3A, 3},
    {P6_9  , PWM_TIOC3C, 5},
    {P3_12 , PWM_TIOC3C, 3},
    {P5_8  , PWM_TIOC4A, 3},
    {P2_4  , PWM_TIOC4A, 3},
    {P5_10 , PWM_TIOC4C, 3},
    {NC    , NC        , 0}
};

/************CAN***************/
const PinMap PinMap_CAN_RD[] = {
    {P5_8  , CAN_0, 2},
    {P7_2  , CAN_0, 5},
    {P2_9  , CAN_0, 5},
    {P8_10 , CAN_1, 3},
    {P2_2  , CAN_1, 2},
    {NC    , NC   , 0}
};

const PinMap PinMap_CAN_TD[] = {
    {P5_9  , CAN_0, 2},
    {P7_3  , CAN_0, 5},
    {P2_8  , CAN_0, 5},
    {P8_9  , CAN_1, 3},
    {P2_3  , CAN_1, 2},
    {NC    , NC   , 0}
};

