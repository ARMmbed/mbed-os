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
    /* SPI_1, SPI_2. Source CLK is PCKL1 */
    return HAL_RCC_GetPCLK1Freq();
}

#endif
