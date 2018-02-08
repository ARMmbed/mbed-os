/*******************************************************************************
 * Copyright (c) 2010-2017 Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   - Modified versions of the software must be conspicuously marked as such.
 *   - This software is licensed solely and exclusively for use with processors
 *     manufactured by or for Analog Devices, Inc.
 *   - This software may not be combined or merged with other code in any manner
 *     that would cause the software to become subject to terms and conditions
 *     which differ from those listed here.
 *   - Neither the name of Analog Devices, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *   - The use of this software may or may not infringe the patent rights of one
 *     or more patent holders.  This license does not release you from the
 *     requirement that you obtain separate licenses from these patent holders
 *     to use this software.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
 * INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
 * CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

/*
    The ADuCM4050 is made in two package variants.

    64 lead LFCSP & 72 ball WLCSP

    There are some differences for Port 2 between the two variants
    WLCSP also has Port 3.

    The #define ADUCM4050_LFCSP is used to determine which variant the code
    is built for.

    For LFCSP leave the #define in, to build for ADUCM4050_WLCSP remove.
*/
#define ADUCM4050_LFCSP

#include "PeripheralPins.h"

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {P0_10, UART_0, 1},
    {P1_15, UART_1, 2},
    {NC,  NC, 0}
};

const PinMap PinMap_UART_RX[] = {
    {P0_11, UART_0, 1},
    {P2_00, UART_1, 2},
    {NC,  NC, 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {P0_00, SPI_0, 1},
    {P1_06, SPI_1, 1},
    {P1_02, SPI_2, 1},
    {NC, NC, 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {P0_01, SPI_0, 1},
    {P1_07, SPI_1, 1},
    {P1_03, SPI_2, 1},
    {NC, NC, 0}
};

const PinMap PinMap_SPI_MISO[] = {
    {P0_02, SPI_0, 1},
    {P1_08, SPI_1, 1},
    {P1_04, SPI_2, 1},
    {NC, NC, 0}
};

#if defined(ADUCM4050_LFCSP)
const PinMap PinMap_SPI_SSEL[] = {
    {P0_03, SPI_0, 1},
    {P1_09, SPI_1, 1},
    {P2_10, SPI_2, 1},
    {NC, NC, 0}
};
#else
const PinMap PinMap_SPI_SSEL[] = {
    {P0_03, SPI_0, 1},
    {P1_09, SPI_1, 1},
    {P2_15, SPI_2, 1},
    {NC, NC, 0}
};
#endif

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    {P0_05, I2C_0, 1},
    {NC, NC, 0}
};

const PinMap PinMap_I2C_SCL[] = {
    {P0_04, I2C_0, 1},
    {NC, NC, 0}
};

/************ADC***************/
const PinMap PinMap_ADC[] = {
    {P2_03, ADC0_VIN0, 1},
    {P2_04, ADC0_VIN1, 1},
    {P2_05, ADC0_VIN2, 1},
    {P2_06, ADC0_VIN3, 1},
    {P2_07, ADC0_VIN4, 1},
    {P2_08, ADC0_VIN5, 1},
    {P2_09, ADC0_VIN6, 1},
#ifdef ADUCM4050_LFCSP
    {P2_10, ADC0_VIN7, 1},
#endif
    {NC, NC, 0}
};

/************RTC***************/
const PinMap PinMap_RTC[] = {
    {NC, OSC32KCLK, 0},
};
