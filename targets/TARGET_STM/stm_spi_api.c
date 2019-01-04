/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2015, STMicroelectronics
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
#include "mbed_assert.h"
#include "mbed_error.h"
#include "mbed_debug.h"
#include "spi_api.h"

#if DEVICE_SPI
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "spi_device.h"
#include "device.h"

#define SPI_INST(obj)    ((SPI_TypeDef *)(obj->spi))
#define SPI_S(obj)    (( struct spi_s *)(obj))

#ifndef DEBUG_STDIO
#   define DEBUG_STDIO 0
#endif

#if DEBUG_STDIO
#   include <stdio.h>
#   define DEBUG_PRINTF(...) do { printf(__VA_ARGS__); } while(0)
#else
#   define DEBUG_PRINTF(...) {}
#endif

/* Consider 10ms as the default timeout for sending/receving 1 byte */
#define TIMEOUT_1_BYTE 10

#if defined(SPI_FLAG_FRLVL) // STM32F0 STM32F3 STM32F7 STM32L4
extern HAL_StatusTypeDef HAL_SPIEx_FlushRxFifo(SPI_HandleTypeDef *hspi);
#endif

static void spi_irq_handler(spi_t *);

#if defined SPI1_BASE
spi_t *spi1 = NULL;
void spi1_irq_handler(void) {
    spi_irq_handler(spi1);
}
#endif

#if defined SPI2_BASE
spi_t *spi2 = NULL;
void spi2_irq_handler(void) {
    spi_irq_handler(spi2);
}
#endif

#if defined SPI3_BASE
spi_t *spi3 = NULL;
void spi3_irq_handler(void) {
    spi_irq_handler(spi3);
}
#endif

#if defined SPI4_BASE
spi_t *spi4 = NULL;
void spi4_irq_handler(void) {
    spi_irq_handler(spi4);
}
#endif

#if defined SPI5_BASE
spi_t *spi5 = NULL;
void spi5_irq_handler(void) {
    spi_irq_handler(spi5);
}
#endif

#if defined SPI6_BASE
spi_t *spi6 = NULL;
void spi6_irq_handler(void) {
    spi_irq_handler(spi6);
}
#endif

void spi_get_capabilities(SPIName name, PinName ssel, spi_capabilities_t *cap)
{
    cap->word_length = 0x00008080;
    cap->support_slave_mode = true;
    cap->half_duplex = true;

    cap->minimum_frequency = 200000;
    cap->maximum_frequency = 4000000;
}

void init_spi(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    __HAL_SPI_DISABLE(handle);

    DEBUG_PRINTF("init_spi: instance=0x%8X\r\n", (int)handle->Instance);
    if (HAL_SPI_Init(handle) != HAL_OK) {
        error("Cannot initialize SPI");
    }
    /* In case of standard 4 wires SPI,PI can be kept enabled all time
     * and SCK will only be generated during the write operations. But in case
     * of 3 wires, it should be only enabled during rd/wr unitary operations,
     * which is handled inside STM32 HAL layer.
     */
    if (handle->Init.Direction  == SPI_DIRECTION_2LINES) {
        __HAL_SPI_ENABLE(handle);
    }
}

SPIName spi_get_module(PinName mosi, PinName miso, PinName sclk) {
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);

    SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);

    return (SPIName)pinmap_merge(spi_data, spi_sclk);
}

void spi_init(spi_t *obj, bool is_slave, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    // Determine the SPI to use
    SPIName spi_module = spi_get_module(mosi, miso, sclk);
    SPIName spi_ssel = (SPIName)pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    if (is_slave && (ssel == NC)) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_PINMAP_INVALID), "missing slave select pin");
    }

    spiobj->spi = (SPIName)pinmap_merge(spi_module, spi_ssel);
    MBED_ASSERT(spiobj->spi != (SPIName)NC);
    void (*irq_vector)(void) = NULL;

#if defined SPI1_BASE
    // Enable SPI clock
    if (spiobj->spi == SPI_1) {
        __HAL_RCC_SPI1_CLK_ENABLE();
        spiobj->spiIRQ = SPI1_IRQn;

        MBED_ASSERT((spi1 == NULL) || (spi1 == obj));
        spi1 = obj;
        irq_vector = spi1_irq_handler;
    }
#endif

#if defined SPI2_BASE
    if (spiobj->spi == SPI_2) {
        __HAL_RCC_SPI2_CLK_ENABLE();
        spiobj->spiIRQ = SPI2_IRQn;

        MBED_ASSERT((spi2 == NULL) || (spi2 == obj));
        spi2 = obj;
        irq_vector = spi2_irq_handler;
    }
#endif

#if defined SPI3_BASE
    if (spiobj->spi == SPI_3) {
        __HAL_RCC_SPI3_CLK_ENABLE();
        spiobj->spiIRQ = SPI3_IRQn;

        MBED_ASSERT((spi3 == NULL) || (spi3 == obj));
        spi3 = obj;
        irq_vector = spi3_irq_handler;
    }
#endif

#if defined SPI4_BASE
    if (spiobj->spi == SPI_4) {
        __HAL_RCC_SPI4_CLK_ENABLE();
        spiobj->spiIRQ = SPI4_IRQn;

        MBED_ASSERT((spi4 == NULL) || (spi4 == obj));
        spi4 = obj;
        irq_vector = spi4_irq_handler;
    }
#endif

#if defined SPI5_BASE
    if (spiobj->spi == SPI_5) {
        __HAL_RCC_SPI5_CLK_ENABLE();
        spiobj->spiIRQ = SPI5_IRQn;

        MBED_ASSERT((spi5 == NULL) || (spi5 == obj));
        spi5 = obj;
        irq_vector = spi5_irq_handler;
    }
#endif

#if defined SPI6_BASE
    if (spiobj->spi == SPI_6) {
        __HAL_RCC_SPI6_CLK_ENABLE();
        spiobj->spiIRQ = SPI6_IRQn;

        MBED_ASSERT((spi6 == NULL) || (spi6 == obj));
        spi6 = obj;
        irq_vector = spi6_irq_handler;
    }
#endif

    // Configure the SPI pins
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    spiobj->pin_miso = miso;
    spiobj->pin_mosi = mosi;
    spiobj->pin_sclk = sclk;
    spiobj->pin_ssel = ssel;
    if (ssel != NC) {
        pinmap_pinout(ssel, PinMap_SPI_SSEL);
        if (is_slave) {
            handle->Init.NSS = SPI_NSS_HARD_INPUT;
        } else {
            handle->Init.NSS = SPI_NSS_HARD_OUTPUT;
        }
    } else {
        handle->Init.NSS = SPI_NSS_SOFT;
    }

    /* Fill default value */
    handle->Instance = SPI_INST(obj);
    handle->Init.Mode = is_slave ? SPI_MODE_SLAVE : SPI_MODE_MASTER;
    handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;

    if (is_slave) {
        if (mosi != NC) {
            handle->Init.Direction     = SPI_DIRECTION_2LINES;
        } else {
            handle->Init.Direction     = SPI_DIRECTION_1LINE;
        }
    } else {
        if (miso != NC) {
            handle->Init.Direction     = SPI_DIRECTION_2LINES;
        } else {
            handle->Init.Direction     = SPI_DIRECTION_1LINE;
        }
    }

    handle->Init.CLKPhase          = SPI_PHASE_1EDGE;
    handle->Init.CLKPolarity       = SPI_POLARITY_LOW;
    handle->Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    handle->Init.CRCPolynomial     = 7;
    handle->Init.DataSize          = SPI_DATASIZE_8BIT;
    handle->Init.FirstBit          = SPI_FIRSTBIT_MSB;
    handle->Init.TIMode            = SPI_TIMODE_DISABLE;

#ifdef DEVICE_SPI_ASYNCH
    IRQn_Type irq_n = spiobj->spiIRQ;
    NVIC_DisableIRQ(irq_n);
    NVIC_ClearPendingIRQ(irq_n);
    NVIC_SetVector(irq_n, (uint32_t)irq_vector);
    DEBUG_PRINTF("init_spi: vector=%d\n", irq_n);
#endif

    init_spi(obj);
}

void spi_free(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    DEBUG_PRINTF("spi_free\r\n");

    __HAL_SPI_DISABLE(handle);
    HAL_SPI_DeInit(handle);

#if defined SPI1_BASE
    // Reset SPI and disable clock
    if (spiobj->spi == SPI_1) {
        MBED_ASSERT(spi1 == obj);
        spi1 = NULL;
        __HAL_RCC_SPI1_FORCE_RESET();
        __HAL_RCC_SPI1_RELEASE_RESET();
        __HAL_RCC_SPI1_CLK_DISABLE();
    }
#endif
#if defined SPI2_BASE
    if (spiobj->spi == SPI_2) {
        MBED_ASSERT(spi2 == obj);
        spi2 = NULL;
        __HAL_RCC_SPI2_FORCE_RESET();
        __HAL_RCC_SPI2_RELEASE_RESET();
        __HAL_RCC_SPI2_CLK_DISABLE();
    }
#endif

#if defined SPI3_BASE
    if (spiobj->spi == SPI_3) {
        MBED_ASSERT(spi3 == obj);
        spi3 = NULL;
        __HAL_RCC_SPI3_FORCE_RESET();
        __HAL_RCC_SPI3_RELEASE_RESET();
        __HAL_RCC_SPI3_CLK_DISABLE();
    }
#endif

#if defined SPI4_BASE
    if (spiobj->spi == SPI_4) {
        MBED_ASSERT(spi4 == obj);
        spi4 = NULL;
        __HAL_RCC_SPI4_FORCE_RESET();
        __HAL_RCC_SPI4_RELEASE_RESET();
        __HAL_RCC_SPI4_CLK_DISABLE();
    }
#endif

#if defined SPI5_BASE
    if (spiobj->spi == SPI_5) {
        MBED_ASSERT(spi5 == obj);
        spi5 = NULL;
        __HAL_RCC_SPI5_FORCE_RESET();
        __HAL_RCC_SPI5_RELEASE_RESET();
        __HAL_RCC_SPI5_CLK_DISABLE();
    }
#endif

#if defined SPI6_BASE
    if (spiobj->spi == SPI_6) {
        MBED_ASSERT(spi6 == obj);
        spi6 = NULL;
        __HAL_RCC_SPI6_FORCE_RESET();
        __HAL_RCC_SPI6_RELEASE_RESET();
        __HAL_RCC_SPI6_CLK_DISABLE();
    }
#endif

    // Configure GPIOs
    if (spiobj->pin_miso != NC) {
        pin_function(spiobj->pin_miso, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
    }
    if (spiobj->pin_mosi != NC) {
        pin_function(spiobj->pin_mosi, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
    }
    pin_function(spiobj->pin_sclk, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
    if (handle->Init.NSS != SPI_NSS_SOFT) {
        pin_function(spiobj->pin_ssel, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
    }
}

void spi_format(spi_t *obj, uint8_t bits, spi_mode_t mode, spi_bit_ordering_t bit_ordering)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    DEBUG_PRINTF("spi_format, bits:%d, mode:%d\n", bits, mode);

    // Save new values
    handle->Init.DataSize          = (bits == 16) ? SPI_DATASIZE_16BIT : SPI_DATASIZE_8BIT;
    handle->Init.FirstBit          = (bit_ordering == SPI_BIT_ORDERING_MSB_FIRST) ? SPI_FIRSTBIT_MSB : SPI_FIRSTBIT_LSB;

    switch (mode) {
        case 0:
            handle->Init.CLKPolarity = SPI_POLARITY_LOW;
            handle->Init.CLKPhase = SPI_PHASE_1EDGE;
            break;
        case 1:
            handle->Init.CLKPolarity = SPI_POLARITY_LOW;
            handle->Init.CLKPhase = SPI_PHASE_2EDGE;
            break;
        case 2:
            handle->Init.CLKPolarity = SPI_POLARITY_HIGH;
            handle->Init.CLKPhase = SPI_PHASE_1EDGE;
            break;
        default:
            handle->Init.CLKPolarity = SPI_POLARITY_HIGH;
            handle->Init.CLKPhase = SPI_PHASE_2EDGE;
            break;
    }

    init_spi(obj);
}

/*
 * Only the IP clock input is family dependant so it computed
 * separately in spi_get_clock_freq
 */
extern int spi_get_clock_freq(spi_t *obj);

static const uint32_t baudrate_prescaler_table[] =  {SPI_BAUDRATEPRESCALER_2,
                                                     SPI_BAUDRATEPRESCALER_4,
                                                     SPI_BAUDRATEPRESCALER_8,
                                                     SPI_BAUDRATEPRESCALER_16,
                                                     SPI_BAUDRATEPRESCALER_32,
                                                     SPI_BAUDRATEPRESCALER_64,
                                                     SPI_BAUDRATEPRESCALER_128,
                                                     SPI_BAUDRATEPRESCALER_256
                                                    };

uint32_t spi_frequency(spi_t *obj, uint32_t hz)
{
    struct spi_s *spiobj = SPI_S(obj);
    uint32_t spi_hz = 0;
    uint8_t prescaler_rank = 0;
    uint8_t last_index = (sizeof(baudrate_prescaler_table) / sizeof(baudrate_prescaler_table[0])) - 1;
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    /* Calculate the spi clock for prescaler_rank 0: SPI_BAUDRATEPRESCALER_2 */
    spi_hz = spi_get_clock_freq(obj) / 2;

    /* Define pre-scaler in order to get highest available frequency below requested frequency */
    while ((spi_hz > hz) && (prescaler_rank < last_index)) {
        spi_hz = spi_hz / 2;
        prescaler_rank++;
    }

    /*  Use the best fit pre-scaler */
    handle->Init.BaudRatePrescaler = baudrate_prescaler_table[prescaler_rank];

    /*  In case maximum pre-scaler still gives too high freq, raise an error */
    if (spi_hz > hz) {
        DEBUG_PRINTF("WARNING: lowest SPI freq (%lu)  higher than requested (%lu)\n", spi_hz, hz);
    }

    DEBUG_PRINTF("spi_frequency, request:%lu, select:%lu\n", hz, spi_hz);

    init_spi(obj);

    return spi_hz;
}

static uint32_t spi_write(spi_t *obj, uint32_t value) {
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

#if defined(LL_SPI_RX_FIFO_TH_HALF)
    /*  Configure the default data size */
    if (handle->Init.DataSize == SPI_DATASIZE_16BIT) {
        LL_SPI_SetRxFIFOThreshold(SPI_INST(obj), LL_SPI_RX_FIFO_TH_HALF);
    } else {
        LL_SPI_SetRxFIFOThreshold(SPI_INST(obj), LL_SPI_RX_FIFO_TH_QUARTER);
    }
#endif

    /*  Here we're using LL which means direct registers access
     *  There is no error management, so we may end up looping
     *  infinitely here in case of faulty device for instance,
     *  but this will increase performances significantly
     */

#if TARGET_STM32H7
    /* Master transfer start */
    LL_SPI_StartMasterTransfer(SPI_INST(obj));

    /* Wait TXP flag to transmit data */
    while (!LL_SPI_IsActiveFlag_TXP(SPI_INST(obj)));
#else
    /* Wait TXE flag to transmit data */
    while (!LL_SPI_IsActiveFlag_TXE(SPI_INST(obj)));

#endif /* TARGET_STM32H7 */

    /* Transmit data */
    if (handle->Init.DataSize == SPI_DATASIZE_16BIT) {
        LL_SPI_TransmitData16(SPI_INST(obj), (uint16_t)value);
    } else {
        LL_SPI_TransmitData8(SPI_INST(obj), (uint8_t)value);
    }

#if TARGET_STM32H7
    /* Wait for RXP or end of Transfer */
    while (!LL_SPI_IsActiveFlag_RXP(SPI_INST(obj)));
#else /* TARGET_STM32H7 */
    /* Wait for RXNE flag before reading */
    while (!LL_SPI_IsActiveFlag_RXNE(SPI_INST(obj)));
#endif /* TARGET_STM32H7 */

    uint32_t out = 0;
    if (handle->Init.DataSize == SPI_DATASIZE_16BIT) {
        out = LL_SPI_ReceiveData16(SPI_INST(obj));
    } else {
        out = LL_SPI_ReceiveData8(SPI_INST(obj));
    }
    return out;
}

uint32_t spi_transfer(spi_t *obj, const void *tx_buffer, uint32_t tx_length,
                           void *rx_buffer, uint32_t rx_length, const void *write_fill)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);
    uint32_t total = (tx_length > rx_length) ? tx_length : rx_length;
    uint32_t i = 0;
    if (handle->Init.Direction == SPI_DIRECTION_2LINES) {
        for (i = 0; i < total; i++) {
            uint32_t out, in;
            switch (handle->Init.DataSize) {
                case SPI_DATASIZE_8BIT:
                    if (tx_buffer) {
                        out = (i < tx_length) ? ((uint8_t *)tx_buffer)[i] : *(uint8_t *)write_fill;
                    } else {
                        out = *(uint8_t *)write_fill;
                    }
                    in = spi_write(obj, out);
                    if (rx_buffer && i < rx_length) {
                        ((uint8_t*)rx_buffer)[i] = (uint8_t)in;
                    }
                    break;
                case SPI_DATASIZE_16BIT:
                    if (tx_buffer) {
                        out = (i < tx_length) ? ((uint16_t *)tx_buffer)[i] : *(uint16_t *)write_fill;
                    } else {
                        out = *(uint16_t *)write_fill;
                    }
                    in = spi_write(obj, out);
                    if (rx_buffer && i < rx_length) {
                        ((uint16_t*)rx_buffer)[i] = (uint16_t)in;
                    }
                    break;
                default:
                    break;
            }
        }
    } else {
        /* In case of 1 WIRE only, first handle TX, then Rx */

        total = tx_length + rx_length;

        if (handle->Init.Mode == SPI_MODE_MASTER) {
            if (tx_length != 0) {
                if (HAL_OK != HAL_SPI_Transmit(handle, (uint8_t *)tx_buffer, tx_length, tx_length * TIMEOUT_1_BYTE)) {
                    /*  report an error */
                    total = 0;
                }
            }
            if (rx_length != 0) {
                if (HAL_OK != HAL_SPI_Receive(handle, (uint8_t *)rx_buffer, rx_length, rx_length * TIMEOUT_1_BYTE)) {
                    /*  report an error */
                    total = 0;
                }
            }
        } else {
            if (rx_length != 0) {
                if (HAL_OK != HAL_SPI_Receive(handle, (uint8_t *)rx_buffer, rx_length, HAL_MAX_DELAY)) {
                    /*  report an error */
                    total = 0;
                }
            }
            if (tx_length != 0) {
                if (HAL_OK != HAL_SPI_Transmit(handle, (uint8_t *)tx_buffer, tx_length, HAL_MAX_DELAY)) {
                    /*  report an error */
                    total = 0;
                }
            }
        }
    }

    return total;
}

#ifdef DEVICE_SPI_ASYNCH
typedef enum {
    SPI_TRANSFER_TYPE_NONE = 0,
    SPI_TRANSFER_TYPE_TX = 1,
    SPI_TRANSFER_TYPE_RX = 2,
    SPI_TRANSFER_TYPE_TXRX = 3,
} transfer_type_t;

/// @returns the number of bytes transferred, or `0` if nothing transferred
static void spi_start_asynch_transfer(spi_t *obj, transfer_type_t transfer_type, const void *tx, void *rx, size_t length, const void *fill_symbol)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);
    bool is16bit = (handle->Init.DataSize == SPI_DATASIZE_16BIT);

    if (is16bit) {
        obj->fill_symbol = *(uint16_t *)fill_symbol;
    } else {
        obj->fill_symbol = *(uint8_t *)fill_symbol;
    }

    // the HAL expects number of transfers instead of number of bytes
    // so for 16 bit transfer width the count needs to be halved
    size_t words;

    DEBUG_PRINTF("SPI inst=0x%8X Start: %u, %u\n", (int)handle->Instance, transfer_type, length);

    obj->transfer_type = transfer_type;

    words = length;

    // enable the interrupt
    IRQn_Type irq_n = spiobj->spiIRQ;
    NVIC_SetPriority(irq_n, 1);
    NVIC_EnableIRQ(irq_n);

    // flush FIFO
#if defined(SPI_FLAG_FRLVL) // STM32F0 STM32F3 STM32F7 STM32L4
    HAL_SPIEx_FlushRxFifo(handle);
#endif

    // enable the right hal transfer
    int rc = 0;
    switch (transfer_type) {
        case SPI_TRANSFER_TYPE_TXRX:
            rc = HAL_SPI_TransmitReceive_IT(handle, (uint8_t *)tx, (uint8_t *)rx, words);
            break;
        case SPI_TRANSFER_TYPE_TX:
            rc = HAL_SPI_Transmit_IT(handle, (uint8_t *)tx, words);
            break;
        case SPI_TRANSFER_TYPE_RX:
            // the receive function also "transmits" the receive buffer so in order
            // to guarantee that fill sym is on the line, we explicitly memset it here
            memset(rx, *(uint8_t*)fill_symbol, length); // this only works for 8 bit symbols though
            rc = HAL_SPI_TransmitReceive_IT(handle, (uint8_t *)rx, (uint8_t *)rx, words);
            break;
        default:
            length = 0;
    }

    if (rc) {
        DEBUG_PRINTF("SPI: RC=%u\n", rc);
    }
}

static void spi_irq_handler(spi_t *obj)
{
    // call the CubeF4 handler, this will update the handle
    HAL_SPI_IRQHandler(&obj->handle);

    if (obj->handle.State == HAL_SPI_STATE_READY) {
        // When HAL SPI is back to READY state, check if there was an error
        int error = obj->handle.ErrorCode;
#ifndef MAX
        // quick & dirty max impl
#define MAX(a, b) ((a>b)?(a):(b))
#endif

        obj->tx_buff.pos += obj->handle.TxXferSize;
        obj->rx_buff.pos += obj->handle.RxXferSize;
        bool is16bit = (obj->handle.Init.DataSize == SPI_DATASIZE_16BIT);

        if (obj->tx_buff.buffer && obj->tx_buff.pos < obj->tx_buff.length) {
            uint16_t pos = obj->tx_buff.pos;
            spi_start_asynch_transfer(obj, SPI_TRANSFER_TYPE_TX,
                    ((uint8_t *)obj->tx_buff.buffer) + (pos * (is16bit?2:1)), NULL,
                    obj->tx_buff.length - obj->tx_buff.pos, &obj->fill_symbol);
            return;
        } else if (obj->rx_buff.buffer &&  obj->rx_buff.pos < obj->rx_buff.length) {
            uint16_t pos = obj->rx_buff.pos;

            spi_start_asynch_transfer(obj, SPI_TRANSFER_TYPE_RX,
                    NULL, ((uint8_t *)obj->rx_buff.buffer) + (pos * (is16bit?2:1)),
                    obj->rx_buff.length - obj->rx_buff.pos, &obj->fill_symbol);

            return;
        }

        spi_async_event_t event = {
            .transfered = MAX(obj->tx_buff.pos, obj->rx_buff.pos),
            .error = (error != HAL_SPI_ERROR_NONE)
        };
        spi_async_handler_f handler = obj->handler;
        void *ctx = obj->ctx;

        handler(obj, ctx, &event);

        obj->handler = NULL;
        obj->ctx = NULL;

        // enable the interrupt
        NVIC_DisableIRQ(obj->spiIRQ);
        NVIC_ClearPendingIRQ(obj->spiIRQ);
    }
}

// asynchronous API
bool spi_transfer_async(spi_t *obj, const void *tx, uint32_t tx_length, void *rx, uint32_t rx_length,
        const void *fill_symbol, spi_async_handler_f handler, void *ctx, DMAUsage hint)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    // TODO: DMA usage is currently ignored
    (void) hint;

    // check which use-case we have
    bool use_tx = (tx != NULL && tx_length > 0);
    bool use_rx = (rx != NULL && rx_length > 0);
    bool is16bit = (handle->Init.DataSize == SPI_DATASIZE_16BIT);

    // don't do anything, if the buffers aren't valid
    if (!use_tx && !use_rx) {
        return false;
    }

    // copy the buffers to the SPI object
    obj->tx_buff.buffer = (void *) tx;
    obj->tx_buff.length = tx_length;
    obj->tx_buff.pos = 0;
    obj->tx_buff.width = is16bit ? 16 : 8;

    obj->rx_buff.buffer = rx;
    obj->rx_buff.length = rx_length;
    obj->rx_buff.pos = 0;
    obj->rx_buff.width = obj->tx_buff.width;

    obj->handler = handler;
    obj->ctx = ctx;

    DEBUG_PRINTF("SPI: Transfer: %lu, %lu\n", tx_length, rx_length);

    if (use_tx && use_rx) {
        uint32_t size = (rx_length < tx_length)? rx_length : tx_length;
        spi_start_asynch_transfer(obj, SPI_TRANSFER_TYPE_TXRX, tx, rx, size, fill_symbol);
    } else if (use_tx) {
        spi_start_asynch_transfer(obj, SPI_TRANSFER_TYPE_TX, tx, NULL, tx_length, fill_symbol);
    } else if (use_rx) {
        spi_start_asynch_transfer(obj, SPI_TRANSFER_TYPE_RX, NULL, rx, rx_length, fill_symbol);
    }
    return true;
}

void spi_transfer_async_abort(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    // disable interrupt
    IRQn_Type irq_n = spiobj->spiIRQ;
    NVIC_ClearPendingIRQ(irq_n);
    NVIC_DisableIRQ(irq_n);

    // clean-up
    __HAL_SPI_DISABLE(handle);
    HAL_SPI_DeInit(handle);
    HAL_SPI_Init(handle);
    __HAL_SPI_ENABLE(handle);
}

#endif //DEVICE_SPI_ASYNCH

#endif
