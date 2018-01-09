
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
#ifdef MAX_PERI
    {P1_0,  IRQ0, 4}, {P4_8,  IRQ0, 8}, {P6_8,  IRQ0, 8}, {P7_9,  IRQ0, 8}, {P8_2,  IRQ0, 5}, {P2_14, IRQ0, 8}, {P5_8,  IRQ0, 2}, {P9_1,  IRQ0, 4},
    {P1_1,  IRQ1, 4}, {P4_9,  IRQ1, 8}, {P6_9,  IRQ1, 8}, {P7_8,  IRQ1, 8}, {P8_3,  IRQ1, 6}, {P2_15, IRQ1, 8},
    {P1_2,  IRQ2, 4}, {P4_10, IRQ2, 8}, {P6_10, IRQ2, 8}, {P7_10, IRQ2, 8}, {P1_8,  IRQ2, 3}, {P3_0,  IRQ2, 3}, {P5_9,  IRQ2, 4}, {P6_3,  IRQ2, 4},
    {P1_3,  IRQ3, 4}, {P4_11, IRQ3, 8}, {P6_11, IRQ3, 8}, {P7_11, IRQ3, 8}, {P1_9,  IRQ3, 3}, {P6_4,  IRQ3, 4},
    {P1_4,  IRQ4, 4}, {P4_12, IRQ4, 8}, {P6_12, IRQ4, 8}, {P7_12, IRQ4, 8}, {P1_10, IRQ4, 3}, {P3_3,  IRQ4, 3}, {P6_1,  IRQ4, 4},
    {P1_5,  IRQ5, 4}, {P4_13, IRQ5, 8}, {P6_13, IRQ5, 8}, {P7_13, IRQ5, 8}, {P1_11, IRQ5, 3}, {P2_0,  IRQ5, 6}, {P6_0,  IRQ5, 6}, {P8_7,  IRQ5, 4},
    {P1_6,  IRQ6, 4}, {P4_14, IRQ6, 8}, {P6_14, IRQ6, 8}, {P7_14, IRQ6, 8}, {P2_12, IRQ6, 6}, {P3_1,  IRQ6, 3}, {P3_9,  IRQ6, 8}, {P5_6,  IRQ6, 6},
    {P1_7,  IRQ7, 4}, {P4_15, IRQ7, 8}, {P6_15, IRQ7, 8}, {P6_2,  IRQ7, 4}, {P2_13, IRQ7, 8},
    {NC,    NC,     0}
#else
    {P9_1,  IRQ0, 4},
    {P7_8,  IRQ1, 8},
    {P1_2,  IRQ2, 4}, {P1_8,  IRQ2, 3}, {P3_0,  IRQ2, 3}, {P5_9,  IRQ2, 4},
    {P1_3,  IRQ3, 4}, {P1_9,  IRQ3, 3},
    {P1_4,  IRQ4, 4}, {P1_10, IRQ4, 3},
    {P1_5,  IRQ5, 4}, {P1_11, IRQ5, 3},
    {P3_1,  IRQ6, 3}, {P3_9,  IRQ6, 8}, {P5_6,  IRQ6, 6},
    {NC,    NC,     0}
#endif
};

/************PINMAP***************/
const PinFunc PIPC_0_tbl[] = {
#ifdef MAX_PERI
//   pin      func     pm
    {P4_0   , 2      , -1}, /* TIOC0A */
    {P5_0   , 6      , -1}, /* TIOC0A */
    {P7_0   , 7      , -1}, /* TIOC0A */
    {P4_1   , 2      , -1}, /* TIOC0B */
    {P5_1   , 6      , -1}, /* TIOC0B */
    {P7_1   , 7      , -1}, /* TIOC0B */
    {P4_2   , 2      , -1}, /* TIOC0C */
    {P5_5   , 6      , -1}, /* TIOC0C */
    {P7_2   , 7      , -1}, /* TIOC0C */
    {P4_3   , 2      , -1}, /* TIOC0D */
    {P5_7   , 6      , -1}, /* TIOC0D */
    {P7_3   , 7      , -1}, /* TIOC0D */
    {P2_11  , 5      , -1}, /* TIOC1A */
    {P6_0   , 5      , -1}, /* TIOC1A */
    {P7_4   , 7      , -1}, /* TIOC1A */
    {P8_8   , 5      , -1}, /* TIOC1A */
    {P9_7   , 4      , -1}, /* TIOC1A */
    {P2_12  , 8      , -1}, /* TIOC1B */
    {P5_2   , 6      , -1}, /* TIOC1B */
    {P6_1   , 5      , -1}, /* TIOC1B */
    {P7_5   , 7      , -1}, /* TIOC1B */
    {P8_9   , 5      , -1}, /* TIOC1B */
    {P2_1   , 6      , -1}, /* TIOC2A */
    {P6_2   , 6      , -1}, /* TIOC2A */
    {P7_6   , 7      , -1}, /* TIOC2A */
    {P8_14  , 4      , -1}, /* TIOC2A */
    {P2_2   , 6      , -1}, /* TIOC2B */
    {P6_3   , 6      , -1}, /* TIOC2B */
    {P7_7   , 7      , -1}, /* TIOC2B */
    {P8_15  , 4      , -1}, /* TIOC2B */
    {P3_4   , 6      , -1}, /* TIOC3A */
    {P7_8   , 7      , -1}, /* TIOC3A */
    {P8_10  , 4      , -1}, /* TIOC3A */
    {P3_5   , 6      , -1}, /* TIOC3B */
    {P7_9   , 7      , -1}, /* TIOC3B */
    {P8_11  , 4      , -1}, /* TIOC3B */
    {P3_6   , 6      , -1}, /* TIOC3C */
    {P5_3   , 6      , -1}, /* TIOC3C */
    {P7_10  , 7      , -1}, /* TIOC3C */
    {P8_12  , 4      , -1}, /* TIOC3C */
    {P3_7   , 6      , -1}, /* TIOC3D */
    {P5_4   , 6      , -1}, /* TIOC3D */
    {P7_11  , 7      , -1}, /* TIOC3D */
    {P8_13  , 4      , -1}, /* TIOC3D */
    {P3_8   , 6      , -1}, /* TIOC4A */
    {P4_4   , 3      , -1}, /* TIOC4A */
    {P7_12  , 7      , -1}, /* TIOC4A */
    {P3_9   , 6      , -1}, /* TIOC4B */
    {P4_5   , 3      , -1}, /* TIOC4B */
    {P7_13  , 7      , -1}, /* TIOC4B */
    {P3_10  , 6      , -1}, /* TIOC4C */
    {P4_6   , 3      , -1}, /* TIOC4C */
    {P7_14  , 7      , -1}, /* TIOC4C */
    {P3_11  , 6      , -1}, /* TIOC4D */
    {P4_7   , 3      , -1}, /* TIOC4D */
    {P7_15  , 7      , -1}, /* TIOC4D */
    {P5_7   , 1      , 1 }, /* TXOUT0M   */
    {P5_6   , 1      , 1 }, /* TXOUT0P   */
    {P5_5   , 1      , 1 }, /* TXOUT1M   */
    {P5_4   , 1      , 1 }, /* TXOUT1P   */
    {P5_3   , 1      , 1 }, /* TXOUT2M   */
    {P5_2   , 1      , 1 }, /* TXOUT2P   */
    {P5_1   , 1      , 1 }, /* TXCLKOUTM */
    {P5_0   , 1      , 1 }, /* TXCLKOUTP */
    {P2_11  , 4      , 0 }, /* SSITxD0 */
    {P4_7   , 5      , 0 }, /* SSITxD0 */
    {P7_4   , 6      , 0 }, /* SSITxD1 */
    {P4_15  , 6      , 0 }, /* SSITxD3 */
    {P7_11  , 2      , 0 }, /* SSITxD3 */
    {P2_7   , 4      , 0 }, /* SSITxD5 */
    {P4_11  , 5      , 0 }, /* SSITxD5 */
    {P8_10  , 8      , 0 }, /* SSITxD5 */
    {P3_7   , 8      , 0 }, /* WDTOVF */
    {NC     , 0      , -1}
#else
    //   pin      func     pm
    {P4_0   , 2      , -1}, // TIOC0A
    {P5_0   , 6      , -1}, // TIOC0A
    {P4_2   , 2      , -1}, // TIOC0C
    {P5_5   , 6      , -1}, // TIOC0C
    //
    {P8_14  , 4      , -1}, // TIOC2A
    //
    {P8_10  , 4      , -1}, // TIOC3A
    {P5_3   , 6      , -1}, // TIOC3C
    {P8_12  , 4      , -1}, // TIOC3C
    //
    {P3_8   , 6      , -1}, // TIOC4A
    {P4_4   , 3      , -1}, // TIOC4A
    {P3_10  , 6      , -1}, // TIOC4C
    {P4_6   , 3      , -1}, // TIOC4C
    //
    {P5_7   , 1      , 1 }, // TXOUT0M
    {P5_6   , 1      , 1 }, // TXOUT0P
    {P5_5   , 1      , 1 }, // TXOUT1M
    {P5_4   , 1      , 1 }, // TXOUT1P
    {P5_3   , 1      , 1 }, // TXOUT2M
    {P5_2   , 1      , 1 }, // TXOUT2P
    {P5_1   , 1      , 1 }, // TXCLKOUTM
    {P5_0   , 1      , 1 }, // TXCLKOUTP
    {P4_7   , 5      , 0 }, // SSITxD0
    {P8_10  , 8      , 0 }, // SSITxD5
    {P3_7   , 8      , 0 }, // WDTOVF
    {NC     , 0      , -1}
#endif
};

/************ADC***************/
const PinMap PinMap_ADC[] = {
#ifdef MAX_PERI
    {P1_8,  AN0, 1},
    {P1_9,  AN1, 1},
    {P1_10, AN2, 1},
    {P1_11, AN3, 1},
    {P1_12, AN4, 1},
    {P1_13, AN5, 1},
    {P1_14, AN6, 1},
    {P1_15, AN7, 1},
    {NC,    NC,  0}
#else
    {P1_8,  AN0, 1},
    {P1_9,  AN1, 1},
    {P1_10, AN2, 1},
    {P1_11, AN3, 1},
    {P1_12, AN4, 1},
    {P1_13, AN5, 1},
    {P1_15, AN7, 1},
    {NC,    NC,  0}
#endif
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    {P1_1 , I2C_0, 1},
    {P1_3 , I2C_1, 1},
    {P1_5 , I2C_2, 1},
    {P1_7 , I2C_3, 1},
    {NC   , NC   , 0}
};

const PinMap PinMap_I2C_SCL[] = {
    {P1_0 , I2C_0, 1},
    {P1_2 , I2C_1, 1},
    {P1_4 , I2C_2, 1},
    {P1_6 , I2C_3, 1},
    {NC   , NC,    0}
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
#ifdef MAX_PERI
    {P2_14 , UART0, 6},
    {P4_9  , UART0, 7},
    {P6_9  , UART0, 5},
    {P2_5  , UART1, 6},
    {P4_12 , UART1, 7},
    {P6_12 , UART1, 5},
    {P9_3  , UART1, 4},
    {P3_0  , UART2, 6},
    {P3_1  , UART2, 4},
    {P4_2  , UART2, 5},
    {P4_14 , UART2, 7},
    {P6_3  , UART2, 7},
    {P8_6  , UART2, 7},
    {P3_5  , UART3, 7},
    {P5_3  , UART3, 5},
    {P6_1  , UART3, 7},
    {P8_8  , UART3, 7},
    {P5_0  , UART4, 5},
    {P7_1  , UART4, 4},
    {P8_14 , UART4, 7},
    {P6_6  , UART5, 5},
    {P8_1  , UART5, 4},
    {P8_13 , UART5, 5},
    {P5_6  , UART6, 5},
    {P6_14 , UART6, 4},
    {P7_4  , UART7, 4},
    {NC    , NC   , 0}
#else
    {P3_0  , UART2, 6},
    {P3_1  , UART2, 4},
    {P4_2  , UART2, 5},
    {P5_3  , UART3, 5},
    {P8_8  , UART3, 7},
    {P5_0  , UART4, 5},
    {P8_14 , UART4, 7},
    {P8_13 , UART5, 5},
    {P5_6  , UART6, 5},
    {NC    , NC   , 0}
#endif
};

const PinMap PinMap_UART_RX[] = {
#ifdef MAX_PERI
    {P2_15 , UART0, 6},
    {P4_10 , UART0, 7},
    {P6_10 , UART0, 5},
    {P2_6  , UART1, 6},
    {P4_13 , UART1, 7},
    {P6_13 , UART1, 5},
    {P9_4  , UART1, 4},
    {P3_2  , UART2, 4},
    {P4_3  , UART2, 5},
    {P4_15 , UART2, 7},
    {P6_2  , UART2, 7},
    {P8_4  , UART2, 7},
    {P3_6  , UART3, 7},
    {P5_4  , UART3, 5},
    {P6_0  , UART3, 7},
    {P8_9  , UART3, 7},
    {P5_1  , UART4, 5},
    {P7_2  , UART4, 4},
    {P8_15 , UART4, 7},
    {P6_7  , UART5, 5},
    {P8_2  , UART5, 4},
    {P8_11 , UART5, 5},
    {P5_7  , UART6, 5},
    {P6_15 , UART6, 4},
    {P7_5  , UART7, 4},
    {NC    , NC   , 0}
#else
    {P3_2  , UART2, 4},
    {P4_3  , UART2, 5},
    {P5_4  , UART3, 5},
    {P8_9  , UART3, 7},
    {P5_1  , UART4, 5},
    {P8_15 , UART4, 7},
    {P8_11 , UART5, 5},
    {P5_7  , UART6, 5},
    {NC    , NC   , 0}
#endif
};

const PinMap PinMap_UART_CTS[] = {
#ifdef MAX_PERI
    {P2_3  , UART1, 6},
    {P9_5  , UART1, 4},
    {P6_3  , UART5, 5},
    {P7_15 , UART5, 4},
    {P7_6  , UART7, 4},
    {NC    , NC   , 0}
#else
    {NC    , NC   , 0}
#endif
};

const PinMap PinMap_UART_RTS[] = {
#ifdef MAX_PERI
    {P2_7  , UART1, 6},
    {P9_6  , UART1, 4},
    {P6_4  , UART5, 5},
    {P8_3  , UART5, 4},
    {P7_7  , UART7, 4},
    {NC    , NC   , 0}
#else
    {NC    , NC   , 0}
#endif
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
#ifdef MAX_PERI
    {P2_12 , SPI_0, 2},
    {P7_15 , SPI_0, 2},
    {P4_4  , SPI_1, 2},
    {P6_4  , SPI_1, 7},
    {P8_3  , SPI_2, 3},
    {P8_14 , SPI_2, 5},
    {P3_0  , SPI_3, 8},
    {P5_0  , SPI_3, 8},
    {P2_8  , SPI_4, 8},
    {P4_0  , SPI_4, 7},
    {NC    , NC   , 0}
#else
    {P4_4  , SPI_1, 2},
    {P8_14 , SPI_2, 5},
    {P5_0  , SPI_3, 8},
    {P4_0  , SPI_4, 7},
    {NC    , NC   , 0}
#endif
};

const PinMap PinMap_SPI_MOSI[] = {
#ifdef MAX_PERI
    {P2_14 , SPI_0, 2},
    {P8_1  , SPI_0, 2},
    {P4_6  , SPI_1, 2},
    {P6_6  , SPI_1, 7},
    {P8_5  , SPI_2, 3},
    {P9_0  , SPI_2, 5},
    {P3_2  , SPI_3, 8},
    {P5_2  , SPI_3, 8},
    {P2_10 , SPI_4, 8},
    {P4_2  , SPI_4, 7},
    {NC    , NC   , 0}
#else
    {P4_6  , SPI_1, 2},
    {P9_0  , SPI_2, 5},
    {P5_2  , SPI_3, 8},
    {P4_2  , SPI_4, 7},
    {NC    , NC   , 0}
#endif
};

const PinMap PinMap_SPI_MISO[] = {
#ifdef MAX_PERI
    {P2_15 , SPI_0, 2},
    {P8_2  , SPI_0, 2},
    {P4_7  , SPI_1, 2},
    {P6_7  , SPI_1, 7},
    {P8_6  , SPI_2, 3},
    {P9_1  , SPI_2, 5},
    {P3_3  , SPI_3, 8},
    {P5_3  , SPI_3, 8},
    {P2_11 , SPI_4, 8},
    {P4_3  , SPI_4, 7},
    {NC    , NC   , 0}
#else
    {P4_7  , SPI_1, 2},
    {P9_1  , SPI_2, 5},
    {P5_3  , SPI_3, 8},
    {P4_3  , SPI_4, 7},
    {NC    , NC   , 0}
#endif
};

const PinMap PinMap_SPI_SSEL[] = {
#ifdef MAX_PERI
    {P2_13 , SPI_0, 2},
    {P8_0  , SPI_0, 2},
    {P4_5  , SPI_1, 2},
    {P6_5  , SPI_1, 7},
    {P8_4  , SPI_2, 3},
    {P8_15 , SPI_2, 5},
    {P3_1  , SPI_3, 8},
    {P5_1  , SPI_3, 8},
    {P2_9  , SPI_4, 8},
    {P4_1  , SPI_4, 7},
    {NC    , NC   , 0}
#else
    {P4_5  , SPI_1, 2},
    {P8_15 , SPI_2, 5},
    {P5_1  , SPI_3, 8},
    {P4_1  , SPI_4, 7},
    {NC    , NC   , 0}
#endif
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
#ifdef MAX_PERI
    // TIOC0 A,C
    {P4_0     , PWM_TIOC0A, 2},	//TIOC0A
    {P5_0     , PWM_TIOC0A, 6},	//TIOC0A
    {P7_0     , PWM_TIOC0A, 7},	//TIOC0A
    {P4_2     , PWM_TIOC0C, 2},	//TIOC0C
    {P5_5     , PWM_TIOC0C, 6},	//TIOC0C
    {P7_2     , PWM_TIOC0C, 7},	//TIOC0C
    //TIOC1 A
    {P2_11    , PWM_TIOC1A, 5},	//TIOC1A
    {P6_0     , PWM_TIOC1A, 5},	//TIOC1A
    {P7_4     , PWM_TIOC1A, 7},	//TIOC1A
    {P8_8     , PWM_TIOC1A, 5},	//TIOC1A
    {P9_7     , PWM_TIOC1A, 4},	//TIOC1A
    //TIOC2 A
    {P2_1     , PWM_TIOC2A, 6},	//TIOC2A
    {P6_2     , PWM_TIOC2A, 6},	//TIOC2A
    {P7_6     , PWM_TIOC2A, 7},	//TIOC2A
    {P8_14    , PWM_TIOC2A, 4},	//TIOC2A
    //TIOC3 A,C
    {P3_4     , PWM_TIOC3A, 6},	//TIOC3A
    {P7_8     , PWM_TIOC3A, 7},	//TIOC3A
    {P8_10    , PWM_TIOC3A, 4},	//TIOC3A
    {P3_6     , PWM_TIOC3C, 6},	//TIOC3C
    {P7_10    , PWM_TIOC3C, 7},	//TIOC3C
    {P8_12    , PWM_TIOC3C, 4},	//TIOC3C
    //TIOC4 A,C
    {P3_8     , PWM_TIOC4A, 6},	//TIOC4A
    {P4_4     , PWM_TIOC4A, 3},	//TIOC4A
    {P7_12    , PWM_TIOC4A, 7},	//TIOC4A
    {P3_10    , PWM_TIOC4C, 6},	//TIOC4C
    {P4_6     , PWM_TIOC4C, 3},	//TIOC4C
    {P7_14    , PWM_TIOC4C, 7},	//TIOC4C
    //PWM1
    {P8_8     , PWM_PWM1A , 6},	//PWM1A
    {P8_9     , PWM_PWM1B , 6},	//PWM1B
    {P8_10    , PWM_PWM1C , 6},	//PWM1C
    {P8_11    , PWM_PWM1D , 6},	//PWM1D
    {P8_12    , PWM_PWM1E , 6},	//PWM1E
    {P8_13    , PWM_PWM1F , 6},	//PWM1F
    {P8_14    , PWM_PWM1G , 6},	//PWM1G
    {P8_15    , PWM_PWM1H , 6},	//PWM1H
    //PWM2
    {P3_0     , PWM_PWM2A , 7},	//PWM2A
    {P3_1     , PWM_PWM2B , 7},	//PWM2B
    {P3_2     , PWM_PWM2C , 7},	//PWM2C
    {P3_3     , PWM_PWM2D , 7},	//PWM2D
    {P4_4     , PWM_PWM2E , 4},	//PWM2E
    {P4_5     , PWM_PWM2F , 4},	//PWM2F
    {P4_6     , PWM_PWM2G , 4},	//PWM2G
    {P4_7     , PWM_PWM2H , 4},	//PWM2H
    {NC       , NC        , 0}
#else
    //TIOC0 A,C
    {P4_0     , PWM_TIOC0A, 2},	//TIOC0A
    {P5_0     , PWM_TIOC0A, 6},	//TIOC0A
    {P4_2     , PWM_TIOC0C, 2},	//TIOC0C
    {P5_5     , PWM_TIOC0C, 6},	//TIOC0C
    //TIOC2 A
    {P8_14    , PWM_TIOC2A, 4},	//TIOC2A
    //TIOC3 A,C
    {P8_10    , PWM_TIOC3A, 4},	//TIOC3A
    {P5_3     , PWM_TIOC3C, 6}, //TIOC3C
    {P8_12    , PWM_TIOC3C, 4},	//TIOC3C
    //TIOC4 A,C
    {P3_8     , PWM_TIOC4A, 6},	//TIOC4A
    {P4_4     , PWM_TIOC4A, 3},	//TIOC4A
    {P3_10    , PWM_TIOC4C, 6},	//TIOC4C
    {P4_6     , PWM_TIOC4C, 3},	//TIOC4C
    //PWM1
    {P8_10    , PWM_PWM1C , 6},	//PWM1C
    {P8_11    , PWM_PWM1D , 6},	//PWM1D
    {P8_12    , PWM_PWM1E , 6},	//PWM1E
    {P8_13    , PWM_PWM1F , 6},	//PWM1F
    {P8_14    , PWM_PWM1G , 6},	//PWM1G
    {P8_15    , PWM_PWM1H , 6},	//PWM1H
    //PWM2
    {P3_0     , PWM_PWM2A , 7},	//PWM2A
    {P3_1     , PWM_PWM2B , 7},	//PWM2B
    {P3_2     , PWM_PWM2C , 7},	//PWM2C
    {P4_4     , PWM_PWM2E , 4},	//PWM2E
    {P4_5     , PWM_PWM2F , 4},	//PWM2F
    {P4_6     , PWM_PWM2G , 4},	//PWM2G
    {P4_7     , PWM_PWM2H , 4},	//PWM2H
    {NC       , NC        , 0}
#endif
};

/************CAN***************/
const PinMap PinMap_CAN_RD[] = {
#ifdef MAX_PERI
    {P7_8  , CAN_0, 4},
    {P9_1  , CAN_0, 3},
    {P1_4  , CAN_1, 3},
    {P5_9  , CAN_1, 5},
    {P7_11 , CAN_1, 4},
    {P4_9  , CAN_2, 6},
    {P6_4  , CAN_2, 3},
    {P7_2  , CAN_2, 5},
    {P2_12 , CAN_3, 5},
    {P4_2  , CAN_3, 4},
    {P1_5  , CAN_4, 3},
    {P2_14 , CAN_4, 5},
    {NC    , NC   , 0}
#else
    {P9_1  , CAN_0, 3},
    {P1_4  , CAN_1, 3},
    {P5_9  , CAN_1, 5},
    {P4_2  , CAN_3, 4},
    {P1_5  , CAN_4, 3},
    {NC    , NC   , 0}
#endif
};

const PinMap PinMap_CAN_TD[] = {
#ifdef MAX_PERI
    {P7_9  , CAN_0, 4},
    {P9_0  , CAN_0, 3},
    {P5_10 , CAN_1, 5},
    {P7_10 , CAN_1, 4},
    {P4_8  , CAN_2, 6},
    {P6_5  , CAN_2, 3},
    {P7_3  , CAN_2, 5},
    {P2_13 , CAN_3, 5},
    {P4_3  , CAN_3, 4},
    {P4_11 , CAN_4, 6},
    {P8_10 , CAN_4, 5},
    {NC    , NC   , 0}
#else
    {P9_0  , CAN_0, 3},
    {P5_10 , CAN_1, 5},
    {P4_3  , CAN_3, 4},
    {P8_10 , CAN_4, 5},
    {NC    , NC   , 0}
#endif
};

