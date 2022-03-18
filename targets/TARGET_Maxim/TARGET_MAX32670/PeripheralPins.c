/*******************************************************************************
 * Copyright (c) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

#include "device.h"
#include "PeripheralPins.h"

/*
 * To select a peripheral function on Maxim microcontrollers, multiple
 * configurations must be made. The mbed PinMap structure only includes one
 * data member to hold this information. To extend the configuration storage,
 * the "function" data member is used as a pointer to a pin_function_t
 * structure. This structure is defined in objects.h. The definitions below
 * include the creation of the pin_function_t structures and the assignment of
 * the pointers to the "function" data members.
 */

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    { P0_7 , I2C_0, 1 }, // I2C0_SDA
    { P0_13, I2C_1, 1 }, // I2C1_SDA
    { P0_19, I2C_2, 1 }, // I2C2_SDA
    { NC,    NC,    0 }
};

const PinMap PinMap_I2C_SCL[] = {
    { P0_6,  I2C_0, 1 }, // I2C0_SCL
    { P0_12, I2C_1, 1 }, // I2C1_SCL
    { P0_18, I2C_2, 1 }, // I2C2_SCL
    { NC,    NC,    0 }
};


/************UART***************/
const PinMap PinMap_UART_TX[] = {
    { P0_9 ,  UART_0, 1 }, // UART0A_TX
    { P0_25,  UART_0, 2 }, // UART0B_TX
    { P0_29,  UART_1, 1 }, // UART1A_TX
    { P0_3,   UART_1, 2 }, // UART1B_TX
    { P0_15,  UART_2, 2 }, // UART2B_TX
    { P0_27,  UART_3, 1 }, // LPUART0_TX
    { NC,     NC,     0 }
};

const PinMap PinMap_UART_RX[] = {
    { P0_8,   UART_0, 1 },  // UART0A_RX
    { P0_24,  UART_0, 2 },  // UART0B_RX
    { P0_28,  UART_1, 1 },  // UART1A_RX
    { P0_2,   UART_1, 2 },  // UART1B_RX
    { P0_14,  UART_2, 2 },  // UART2B_RX
    { P0_26,  UART_3, 1 },  // LPUART0_RX
    { NC,     NC,     0 }
};

const PinMap PinMap_UART_CTS[] = {
    { P0_10,  UART_0, 1 }, // UART0A_CTS
    { P0_26,  UART_0, 2 }, // UART0B_CTS
    { P0_30,  UART_1, 1 }, // UART1A_CTS
    { P0_4,   UART_1, 2 }, // UART1B_CTS
    { P0_16,  UART_2, 2 }, // UART2B_CTS
    { P0_24,  UART_3, 1 }, // LPUART0_CTS
    { NC,     NC,     0 }
};

const PinMap PinMap_UART_RTS[] = {
    { P0_11,  UART_0, 1 }, // UART0A_RTS
    { P0_27,  UART_0, 2 }, // UART0B_RTS
    { P0_31,  UART_1, 1 }, // UART1A_RTS
    { P0_5,   UART_1, 2 }, // UART1B_RTS
    { P0_17,  UART_2, 2 }, // UART2B_RTS
    { P0_25,  UART_3, 1 }, // LPUART0_RTS
    { NC,     NC,     0 }
};


/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    { P0_4,   SPI_0, 1 }, // SPI0_SCK
    { P0_16,  SPI_1, 1 }, // SPI1_SCK
    { P1_3,   SPI_2, 1 }, // SPI2_SCK
    { NC,     NC,    0 }
};

const PinMap PinMap_SPI_MOSI[] = {
    { P0_3,   SPI_0, 1 }, // SPI0_MOSI
    { P0_15,  SPI_1, 1 }, // SPI1_MOSI
    { P1_2,   SPI_2, 1 }, // SPI2_MOSI
    { NC,     NC,    0 }
};

const PinMap PinMap_SPI_MISO[] = {
    { P0_2,   SPI_0, 1 }, // SPI0_MISO
    { P0_14,  SPI_1, 1 }, // SPI1_MISO
    { P1_1,   SPI_2, 1 }, // SPI2_MISO
    { NC,     NC,    0 }
};

const PinMap PinMap_SPI_SSEL[] = {
    { P0_5,   SPI_0, 1 }, // SPI0_SS
    { P0_17,  SPI_1, 1 }, // SPI1_SS
    { P1_4,   SPI_2, 1 }, // SPI2_SS
    { NC,     NC,    0 }
};
