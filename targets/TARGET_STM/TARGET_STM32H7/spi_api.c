/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2020 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include "mbed_assert.h"
#include "mbed_error.h"
#include "spi_api.h"

#if DEVICE_SPI

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "mbed_error.h"
#include "spi_device.h"

#if DEVICE_SPI_ASYNCH
#define SPI_S(obj)    (( struct spi_s *)(&(obj->spi)))
#else
#define SPI_S(obj)    (( struct spi_s *)(obj))
#endif

/*
 * Only the frequency is managed in the family specific part
 * the rest of SPI management is common to all STM32 families
 */
int spi_get_clock_freq(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);
    int spi_hz = 0;

    /* Get source clock depending on SPI instance */
    switch ((int)spiobj->spi) {
        case SPI_1:
        case SPI_2:
        case SPI_3:
            spi_hz = LL_RCC_GetSPIClockFreq(LL_RCC_SPI123_CLKSOURCE);
            break;
        case SPI_4:
        case SPI_5:
            spi_hz = LL_RCC_GetSPIClockFreq(LL_RCC_SPI45_CLKSOURCE);
            break;
        case SPI_6:
            spi_hz = LL_RCC_GetSPIClockFreq(LL_RCC_SPI6_CLKSOURCE);
            break;
        default:
            error("CLK: SPI instance not set");
            break;
    }
    if (spi_hz == LL_RCC_PERIPH_FREQUENCY_NO) {
        error("spi_hz not found\n");
    }
    return spi_hz;
}

#endif
