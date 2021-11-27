/*******************************************************************************
 * Copyright (c) Maxim Integrated Products, Inc., All Rights Reserved.
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
    { P0_9,  I2C_0, 1 },
    { P0_3,  I2C_1, 1 },
    { NC, 	 NC,	0 }
};

const PinMap PinMap_I2C_SCL[] = {
    { P0_8,  I2C_0, 1 },
    { P0_2,	 I2C_1, 1 },
	{ NC,    NC,    0 }
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {P0_4, 	UART_0, 2},
    {P0_10, UART_1, 2},
    {P0_0, 	UART_1, 3},
    {P0_6, 	UART_1, 3},
    {NC,    NC,     0}
};

const PinMap PinMap_UART_RX[] = {
	{P0_5, 	UART_0, 2},
	{P0_11, UART_1,	2},
	{P0_1, 	UART_1, 3},
	{P0_7, 	UART_1, 3},
    {NC,    NC,     0}
};

const PinMap PinMap_UART_CTS[] = {
	{P0_6, 	UART_0, 2},
	{P0_12, UART_1, 2},
    {NC,    NC,     0}
};

const PinMap PinMap_UART_RTS[] = {
	{P0_7, 	UART_0, 2},
	{P0_13, UART_1, 2},
    {NC,    NC,     0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    { P0_6,  SPI_0, 1 },
    { P0_12, SPI_1, 1 },
    { P0_2,  SPI_1, 2 },
    { NC,    NC,    0 }
};

const PinMap PinMap_SPI_MOSI[] = {
    { P0_5,  SPI_0, 1 },
    { P0_11, SPI_1, 1 },
    { P0_1,  SPI_1, 2 },
    { NC,    NC,    0 }
};

const PinMap PinMap_SPI_MISO[] = {
    { P0_4,  SPI_0, 1 },
    { P0_10, SPI_1, 1 },
    { P0_0,  SPI_1, 2 },
    { NC,    NC,    0 }
};

const PinMap PinMap_SPI_SSEL[] = {
    { P0_7,  SPI_0, 1 },
    { P0_13, SPI_1, 1 },
    { P0_3,  SPI_1, 2 },
    { NC,    NC,    0 }
};

