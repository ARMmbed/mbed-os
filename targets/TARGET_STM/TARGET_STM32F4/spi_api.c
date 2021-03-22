/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2021 STMicroelectronics.
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
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"

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
#if defined SPI4_BASE
        case SPI_4:
#endif
#if defined SPI5_BASE
        case SPI_5:
#endif
#if defined SPI6_BASE
        case SPI_6:
#endif
            /* SPI_1, SPI_4, SPI_5 and SPI_6. Source CLK is PCKL2 */
            spi_hz = HAL_RCC_GetPCLK2Freq();
            break;
        case SPI_2:
#if defined SPI3_BASE
        case SPI_3:
#endif
            /* SPI_2 and SPI_3. Source CLK is PCKL1 */
            spi_hz = HAL_RCC_GetPCLK1Freq();
            break;
        default:
            error("CLK: SPI instance not set");
            break;
    }
    return spi_hz;
}

#endif
