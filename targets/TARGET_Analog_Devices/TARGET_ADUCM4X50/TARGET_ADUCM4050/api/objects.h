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

#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "cmsis.h"
#include "PeripheralNames.h"
#include "PinNames.h"
#include "gpio_object.h"
#include "adi_rng.h"

#include "adi_i2c.h"
#include "adi_spi.h"
#include "adi_adc.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gpio_irq_s {
    unsigned int id;
    PinName pinname;
};

struct sleep_s {
    int temp;
};

struct serial_s {
    int index;
};

struct trng_s {
    ADI_RNG_HANDLE  RNGhDevice;
};

#define BUILD_I2C_MI_DYNAMIC
struct i2c_s {
    uint32_t        instance;
    uint32_t        error;
    ADI_I2C_HANDLE  *pI2C_Handle;
#if defined(BUILD_I2C_MI_DYNAMIC)
    ADI_I2C_HANDLE  I2C_Handle;
    uint8_t         I2C_Mem[ADI_I2C_MEMORY_SIZE];
#endif
};

#define BUILD_SPI_MI_DYNAMIC
struct spi_s {
    uint32_t        instance;
    uint32_t        error;
    ADI_SPI_HANDLE  *pSPI_Handle;
#if defined(BUILD_SPI_MI_DYNAMIC)
    ADI_SPI_HANDLE  SPI_Handle;
    uint8_t         SPI_Mem[ADI_SPI_MEMORY_SIZE];
#endif
};

#include "gpio_object.h"

struct analogin_s {
    ADI_ADC_HANDLE  hDevice;
    ADI_ADC_BUFFER  UserBuffer;
    uint8_t         DelayCycles;
    uint8_t         SampleCycles;
};



#ifdef __cplusplus
}
#endif

#endif
