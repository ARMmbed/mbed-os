/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2014, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
#include "spi_api.h"

#if DEVICE_SPI

#include <math.h>
#include "cmsis.h"
#include "pinmap.h"
#include "error.h"
#include "stm32f4xx_hal.h"

static const PinMap PinMap_SPI_MOSI[] = {
    {PA_7,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)},
    {NC,    NC,    0}
};

static const PinMap PinMap_SPI_MISO[] = {
    {PA_6,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)},
    {NC,    NC,    0}
};

static const PinMap PinMap_SPI_SCLK[] = {
    {PA_5,  SPI_1, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1)},
    {NC,    NC,    0}
};

// Only used in Slave mode
static const PinMap PinMap_SPI_SSEL[] = {
    {PB_6,  SPI_1, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0)}, // Generic IO, not real H/W NSS pin
    {NC,    NC,    0}
};

static SPI_HandleTypeDef SpiHandle;

static void init_spi(spi_t *obj) {
    SpiHandle.Instance = (SPI_TypeDef *)(obj->spi);
  
    __HAL_SPI_DISABLE(&SpiHandle);

    SpiHandle.Init.Mode              = obj->mode;
    SpiHandle.Init.BaudRatePrescaler = obj->br_presc;
    SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
    SpiHandle.Init.CLKPhase          = obj->cpha;
    SpiHandle.Init.CLKPolarity       = obj->cpol;
    SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLED;
    SpiHandle.Init.CRCPolynomial     = 7;
    SpiHandle.Init.DataSize          = obj->bits;
    SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    SpiHandle.Init.NSS               = obj->nss;
    SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLED;
  
    HAL_SPI_Init(&SpiHandle);

    __HAL_SPI_ENABLE(&SpiHandle);
}

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel) {
    // Determine the SPI to use
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    SPIName spi_ssel = (SPIName)pinmap_peripheral(ssel, PinMap_SPI_SSEL);
  
    SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
    SPIName spi_cntl = (SPIName)pinmap_merge(spi_sclk, spi_ssel);
  
    obj->spi = (SPIName)pinmap_merge(spi_data, spi_cntl);
  
    if (obj->spi == (SPIName)NC) {
        error("SPI error: pinout mapping failed.");
    }
    
    // Enable SPI clock
    if (obj->spi == SPI_1) {
        __SPI1_CLK_ENABLE();
    }
    
    // Configure the SPI pins
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    
    // Save new values
    obj->bits = SPI_DATASIZE_8BIT;
    obj->cpol = SPI_POLARITY_LOW;
    obj->cpha = SPI_PHASE_1EDGE;
    obj->br_presc = SPI_BAUDRATEPRESCALER_256;
    
    if (ssel == NC) { // Master
        obj->mode = SPI_MODE_MASTER;
        obj->nss = SPI_NSS_SOFT;
    }
    else { // Slave
        pinmap_pinout(ssel, PinMap_SPI_SSEL);
        obj->mode = SPI_MODE_SLAVE;
        obj->nss = SPI_NSS_HARD_INPUT;
    }

    init_spi(obj);
}

void spi_free(spi_t *obj) {
    SpiHandle.Instance = (SPI_TypeDef *)(obj->spi);
    HAL_SPI_DeInit(&SpiHandle);
}

void spi_format(spi_t *obj, int bits, int mode, int slave) {  
    // Save new values
    if (bits == 8) {
        obj->bits = SPI_DATASIZE_8BIT;
    }
    else {
        obj->bits = SPI_DATASIZE_16BIT;
    }
    
    switch (mode) {
        case 0:
          obj->cpol = SPI_POLARITY_LOW;
          obj->cpha = SPI_PHASE_1EDGE;
        break;
        case 1:
          obj->cpol = SPI_POLARITY_LOW;
          obj->cpha = SPI_PHASE_2EDGE;
        break;
        case 2:
          obj->cpol = SPI_POLARITY_HIGH;
          obj->cpha = SPI_PHASE_1EDGE;          
        break;
        default:
          obj->cpol = SPI_POLARITY_HIGH;
          obj->cpha = SPI_PHASE_2EDGE;          
        break;
    }
    
    if (slave == 0) {
        obj->mode = SPI_MODE_MASTER;
        obj->nss = SPI_NSS_SOFT;
    }
    else {
        obj->mode = SPI_MODE_SLAVE;
        obj->nss = SPI_NSS_HARD_INPUT;      
    }
    
    init_spi(obj);
}

void spi_frequency(spi_t *obj, int hz) {
    // Note: The frequencies are obtained with SPI1 clock = 84 MHz (APB2 clock)
    if (hz < 500000) {
        obj->br_presc = SPI_BAUDRATEPRESCALER_256; // 331 kHz
    }
    else if ((hz >= 500000) && (hz < 1000000)) {
        obj->br_presc = SPI_BAUDRATEPRESCALER_128; // 663 kHz
    }
    else if ((hz >= 1000000) && (hz < 2000000)) {
        obj->br_presc = SPI_BAUDRATEPRESCALER_64; // 1.3 MHz
    }
    else if ((hz >= 2000000) && (hz < 5000000)) {
        obj->br_presc = SPI_BAUDRATEPRESCALER_32; // 2.65 MHz
    }
    else if ((hz >= 5000000) && (hz < 10000000)) {
        obj->br_presc = SPI_BAUDRATEPRESCALER_16; // 5.3 MHz
    }
    else if ((hz >= 10000000) && (hz < 20000000)) {
        obj->br_presc = SPI_BAUDRATEPRESCALER_8; // 10.6 MHz
    }
    else if ((hz >= 20000000) && (hz < 40000000)) {
        obj->br_presc = SPI_BAUDRATEPRESCALER_4; // 21.2 MHz
    }
    else { // >= 40000000
        obj->br_presc = SPI_BAUDRATEPRESCALER_2; // 42 MHz
    }
    init_spi(obj);
}

static inline int ssp_readable(spi_t *obj) {
    int status;
    SpiHandle.Instance = (SPI_TypeDef *)(obj->spi);
    // Check if data is received
    status = ((__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_RXNE) != RESET) ? 1 : 0);
    return status;  
}

static inline int ssp_writeable(spi_t *obj) {
    int status;
    SpiHandle.Instance = (SPI_TypeDef *)(obj->spi);
    // Check if data is transmitted
    status = ((__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_TXE) != RESET) ? 1 : 0);
    return status;
}

static inline void ssp_write(spi_t *obj, int value) {
    SPI_TypeDef *spi = (SPI_TypeDef *)(obj->spi);
    while (!ssp_writeable(obj));
    spi->DR = (uint8_t)value;
}

static inline int ssp_read(spi_t *obj) {
    SPI_TypeDef *spi = (SPI_TypeDef *)(obj->spi);
    while (!ssp_readable(obj));
    return (int)spi->DR;
}

static inline int ssp_busy(spi_t *obj) {
    int status;
    SpiHandle.Instance = (SPI_TypeDef *)(obj->spi);
    status = ((__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_BSY) != RESET) ? 1 : 0);
    return status;
}

int spi_master_write(spi_t *obj, int value) {
    ssp_write(obj, value);
    return ssp_read(obj);
}

int spi_slave_receive(spi_t *obj) {
    return (ssp_readable(obj) && !ssp_busy(obj)) ? (1) : (0);
};

int spi_slave_read(spi_t *obj) {
    SPI_TypeDef *spi = (SPI_TypeDef *)(obj->spi);
    return (int)spi->DR;
}

void spi_slave_write(spi_t *obj, int value) {
    SPI_TypeDef *spi = (SPI_TypeDef *)(obj->spi);
    while (!ssp_writeable(obj));
    spi->DR = (uint8_t)value;
}

int spi_busy(spi_t *obj) {
    return ssp_busy(obj);
}

#endif
