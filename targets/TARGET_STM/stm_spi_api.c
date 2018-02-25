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

#if DEVICE_SPI_ASYNCH
    #define SPI_INST(obj)    ((SPI_TypeDef *)(obj->spi.spi))
#else
    #define SPI_INST(obj)    ((SPI_TypeDef *)(obj->spi))
#endif

#if DEVICE_SPI_ASYNCH
    #define SPI_S(obj)    (( struct spi_s *)(&(obj->spi)))
#else
    #define SPI_S(obj)    (( struct spi_s *)(obj))
#endif

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

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    // Determine the SPI to use
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    SPIName spi_ssel = (SPIName)pinmap_peripheral(ssel, PinMap_SPI_SSEL);

    SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
    SPIName spi_cntl = (SPIName)pinmap_merge(spi_sclk, spi_ssel);

    spiobj->spi = (SPIName)pinmap_merge(spi_data, spi_cntl);
    MBED_ASSERT(spiobj->spi != (SPIName)NC);

#if defined SPI1_BASE
    // Enable SPI clock
    if (spiobj->spi == SPI_1) {
        __HAL_RCC_SPI1_CLK_ENABLE();
        spiobj->spiIRQ = SPI1_IRQn;
    }
#endif

#if defined SPI2_BASE
    if (spiobj->spi == SPI_2) {
        __HAL_RCC_SPI2_CLK_ENABLE();
        spiobj->spiIRQ = SPI2_IRQn;
    }
#endif

#if defined SPI3_BASE
    if (spiobj->spi == SPI_3) {
        __HAL_RCC_SPI3_CLK_ENABLE();
        spiobj->spiIRQ = SPI3_IRQn;
    }
#endif

#if defined SPI4_BASE
    if (spiobj->spi == SPI_4) {
        __HAL_RCC_SPI4_CLK_ENABLE();
        spiobj->spiIRQ = SPI4_IRQn;
    }
#endif

#if defined SPI5_BASE
    if (spiobj->spi == SPI_5) {
        __HAL_RCC_SPI5_CLK_ENABLE();
        spiobj->spiIRQ = SPI5_IRQn;
    }
#endif

#if defined SPI6_BASE
    if (spiobj->spi == SPI_6) {
        __HAL_RCC_SPI6_CLK_ENABLE();
        spiobj->spiIRQ = SPI6_IRQn;
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
    } else {
        handle->Init.NSS = SPI_NSS_SOFT;
    }

    /* Fill default value */
    handle->Instance = SPI_INST(obj);
    handle->Init.Mode              = SPI_MODE_MASTER;
    handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;

    if (miso != NC) {
        handle->Init.Direction     = SPI_DIRECTION_2LINES;
    } else {
       handle->Init.Direction      = SPI_DIRECTION_1LINE;
    }

    handle->Init.CLKPhase          = SPI_PHASE_1EDGE;
    handle->Init.CLKPolarity       = SPI_POLARITY_LOW;
    handle->Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    handle->Init.CRCPolynomial     = 7;
    handle->Init.DataSize          = SPI_DATASIZE_8BIT;
    handle->Init.FirstBit          = SPI_FIRSTBIT_MSB;
    handle->Init.TIMode            = SPI_TIMODE_DISABLE;

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
        __HAL_RCC_SPI1_FORCE_RESET();
        __HAL_RCC_SPI1_RELEASE_RESET();
        __HAL_RCC_SPI1_CLK_DISABLE();
    }
#endif
#if defined SPI2_BASE
    if (spiobj->spi == SPI_2) {
        __HAL_RCC_SPI2_FORCE_RESET();
        __HAL_RCC_SPI2_RELEASE_RESET();
        __HAL_RCC_SPI2_CLK_DISABLE();
    }
#endif

#if defined SPI3_BASE
    if (spiobj->spi == SPI_3) {
        __HAL_RCC_SPI3_FORCE_RESET();
        __HAL_RCC_SPI3_RELEASE_RESET();
        __HAL_RCC_SPI3_CLK_DISABLE();
    }
#endif

#if defined SPI4_BASE
    if (spiobj->spi == SPI_4) {
        __HAL_RCC_SPI4_FORCE_RESET();
        __HAL_RCC_SPI4_RELEASE_RESET();
        __HAL_RCC_SPI4_CLK_DISABLE();
    }
#endif

#if defined SPI5_BASE
    if (spiobj->spi == SPI_5) {
        __HAL_RCC_SPI5_FORCE_RESET();
        __HAL_RCC_SPI5_RELEASE_RESET();
        __HAL_RCC_SPI5_CLK_DISABLE();
    }
#endif

#if defined SPI6_BASE
    if (spiobj->spi == SPI_6) {
        __HAL_RCC_SPI6_FORCE_RESET();
        __HAL_RCC_SPI6_RELEASE_RESET();
        __HAL_RCC_SPI6_CLK_DISABLE();
    }
#endif

    // Configure GPIOs
    pin_function(spiobj->pin_miso, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
    pin_function(spiobj->pin_mosi, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
    pin_function(spiobj->pin_sclk, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
    if (handle->Init.NSS != SPI_NSS_SOFT) {
        pin_function(spiobj->pin_ssel, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
    }
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    DEBUG_PRINTF("spi_format, bits:%d, mode:%d, slave?:%d\r\n", bits, mode, slave);

    // Save new values
    handle->Init.DataSize          = (bits == 16) ? SPI_DATASIZE_16BIT : SPI_DATASIZE_8BIT;

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

    if (handle->Init.NSS != SPI_NSS_SOFT) {
        handle->Init.NSS = (slave) ? SPI_NSS_HARD_INPUT : SPI_NSS_HARD_OUTPUT;
    }

    handle->Init.Mode = (slave) ? SPI_MODE_SLAVE : SPI_MODE_MASTER;

    if (slave && (handle->Init.Direction == SPI_DIRECTION_1LINE)) {
        /*  SPI slave implemtation in MBED does not support the 3 wires SPI.
         *  (e.g. when MISO is not connected). So we're forcing slave in
         *  2LINES mode. As MISO is not connected, slave will only read
         *  from master, and cannot write to it. Inform user.
         */
        debug("3 wires SPI slave not supported - slave will only read\r\n");
        handle->Init.Direction = SPI_DIRECTION_2LINES;
    }

    init_spi(obj);
}

/*
 * Only the IP clock input is family dependant so it computed
 * separately in spi_get_clock_freq
 */
extern int spi_get_clock_freq(spi_t *obj);

static const uint16_t baudrate_prescaler_table[] =	{SPI_BAUDRATEPRESCALER_2,
                                                    SPI_BAUDRATEPRESCALER_4,
                                                    SPI_BAUDRATEPRESCALER_8,
                                                    SPI_BAUDRATEPRESCALER_16,
                                                    SPI_BAUDRATEPRESCALER_32,
                                                    SPI_BAUDRATEPRESCALER_64,
                                                    SPI_BAUDRATEPRESCALER_128,
                                                    SPI_BAUDRATEPRESCALER_256};

void spi_frequency(spi_t *obj, int hz) {
    struct spi_s *spiobj = SPI_S(obj);
    int spi_hz = 0;
    uint8_t prescaler_rank = 0;
    uint8_t last_index = (sizeof(baudrate_prescaler_table)/sizeof(baudrate_prescaler_table[0])) - 1;
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
        DEBUG_PRINTF("WARNING: lowest SPI freq (%d)  higher than requested (%d)\r\n", spi_hz, hz);
    }

    DEBUG_PRINTF("spi_frequency, request:%d, select:%d\r\n", hz, spi_hz);

    init_spi(obj);
}

static inline int ssp_readable(spi_t *obj)
{
    int status;
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    // Check if data is received
    status = ((__HAL_SPI_GET_FLAG(handle, SPI_FLAG_RXNE) != RESET) ? 1 : 0);
    return status;
}

static inline int ssp_writeable(spi_t *obj)
{
    int status;
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    // Check if data is transmitted
    status = ((__HAL_SPI_GET_FLAG(handle, SPI_FLAG_TXE) != RESET) ? 1 : 0);
    return status;
}

static inline int ssp_busy(spi_t *obj)
{
    int status;
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);
    status = ((__HAL_SPI_GET_FLAG(handle, SPI_FLAG_BSY) != RESET) ? 1 : 0);
    return status;
}

int spi_master_write(spi_t *obj, int value)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);

    if (handle->Init.Direction == SPI_DIRECTION_1LINE) {
        return HAL_SPI_Transmit(handle, (uint8_t*)&value, 1, TIMEOUT_1_BYTE);
    }

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
     *  infinitely here in case of faulty device for insatnce,
     *  but this will increase performances significantly
     */

    /* Wait TXE flag to transmit data */
    while (!LL_SPI_IsActiveFlag_TXE(SPI_INST(obj)));

    if (handle->Init.DataSize == SPI_DATASIZE_16BIT) {
        LL_SPI_TransmitData16(SPI_INST(obj), value);
    } else {
        LL_SPI_TransmitData8(SPI_INST(obj), (uint8_t) value);
    }

    /* Then wait RXE flag before reading */
    while (!LL_SPI_IsActiveFlag_RXNE(SPI_INST(obj)));

    if (handle->Init.DataSize == SPI_DATASIZE_16BIT) {
        return LL_SPI_ReceiveData16(SPI_INST(obj));
    } else {
        return LL_SPI_ReceiveData8(SPI_INST(obj));
    }
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char write_fill)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);
    int total = (tx_length > rx_length) ? tx_length : rx_length;
    int i = 0;
    if (handle->Init.Direction == SPI_DIRECTION_2LINES) {
        for (i = 0; i < total; i++) {
            char out = (i < tx_length) ? tx_buffer[i] : write_fill;
            char in = spi_master_write(obj, out);
            if (i < rx_length) {
                rx_buffer[i] = in;
            }
        }
    } else {
        /* In case of 1 WIRE only, first handle TX, then Rx */
        if (tx_length != 0) {
            if (HAL_OK != HAL_SPI_Transmit(handle, (uint8_t*)tx_buffer, tx_length, tx_length*TIMEOUT_1_BYTE)) {
                /*  report an error */
                total = 0;
            }
        }
        if (rx_length != 0) {
            if (HAL_OK != HAL_SPI_Receive(handle, (uint8_t*)rx_buffer, rx_length, rx_length*TIMEOUT_1_BYTE)) {
                /*  report an error */
                total = 0;
            }
        }
    }

    return total;
}

int spi_slave_receive(spi_t *obj)
{
    return ((ssp_readable(obj) && !ssp_busy(obj)) ? 1 : 0);
};

int spi_slave_read(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);
    while (!ssp_readable(obj));
    if (handle->Init.DataSize == SPI_DATASIZE_16BIT) {
        return LL_SPI_ReceiveData16(SPI_INST(obj));
    } else {
        return LL_SPI_ReceiveData8(SPI_INST(obj));
    }
}

void spi_slave_write(spi_t *obj, int value)
{
    SPI_TypeDef *spi = SPI_INST(obj);
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);
    while (!ssp_writeable(obj));
    if (handle->Init.DataSize == SPI_DATASIZE_8BIT) {
        // Force 8-bit access to the data register
        uint8_t *p_spi_dr = 0;
        p_spi_dr = (uint8_t *) & (spi->DR);
        *p_spi_dr = (uint8_t)value;
    } else { // SPI_DATASIZE_16BIT
        spi->DR = (uint16_t)value;
    }
}

int spi_busy(spi_t *obj)
{
    return ssp_busy(obj);
}

#ifdef DEVICE_SPI_ASYNCH
typedef enum {
    SPI_TRANSFER_TYPE_NONE = 0,
    SPI_TRANSFER_TYPE_TX = 1,
    SPI_TRANSFER_TYPE_RX = 2,
    SPI_TRANSFER_TYPE_TXRX = 3,
} transfer_type_t;


/// @returns the number of bytes transferred, or `0` if nothing transferred
static int spi_master_start_asynch_transfer(spi_t *obj, transfer_type_t transfer_type, const void *tx, void *rx, size_t length)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);
    bool is16bit = (handle->Init.DataSize == SPI_DATASIZE_16BIT);
    // the HAL expects number of transfers instead of number of bytes
    // so for 16 bit transfer width the count needs to be halved
    size_t words;

    DEBUG_PRINTF("SPI inst=0x%8X Start: %u, %u\r\n", (int)handle->Instance, transfer_type, length);

    obj->spi.transfer_type = transfer_type;

    if (is16bit) {
        words = length / 2;
    } else {
        words = length;
    }

    // enable the interrupt
    IRQn_Type irq_n = spiobj->spiIRQ;
    NVIC_DisableIRQ(irq_n);
    NVIC_ClearPendingIRQ(irq_n);
    NVIC_SetPriority(irq_n, 1);
    NVIC_EnableIRQ(irq_n);

    // flush FIFO
#if defined(SPI_FLAG_FRLVL) // STM32F0 STM32F3 STM32F7 STM32L4
    HAL_SPIEx_FlushRxFifo(handle);
#endif

    // enable the right hal transfer
    int rc = 0;
    switch(transfer_type) {
        case SPI_TRANSFER_TYPE_TXRX:
            rc = HAL_SPI_TransmitReceive_IT(handle, (uint8_t*)tx, (uint8_t*)rx, words);
            break;
        case SPI_TRANSFER_TYPE_TX:
            rc = HAL_SPI_Transmit_IT(handle, (uint8_t*)tx, words);
            break;
        case SPI_TRANSFER_TYPE_RX:
            // the receive function also "transmits" the receive buffer so in order
            // to guarantee that 0xff is on the line, we explicitly memset it here
            memset(rx, SPI_FILL_WORD, length);
            rc = HAL_SPI_Receive_IT(handle, (uint8_t*)rx, words);
            break;
        default:
            length = 0;
    }

    if (rc) {
        DEBUG_PRINTF("SPI: RC=%u\n", rc);
        length = 0;
    }

    return length;
}

// asynchronous API
void spi_master_transfer(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint8_t bit_width, uint32_t handler, uint32_t event, DMAUsage hint)
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
    if (!use_tx && !use_rx)
        return;

    // copy the buffers to the SPI object
    obj->tx_buff.buffer = (void *) tx;
    obj->tx_buff.length = tx_length;
    obj->tx_buff.pos = 0;
    obj->tx_buff.width = is16bit ? 16 : 8;

    obj->rx_buff.buffer = rx;
    obj->rx_buff.length = rx_length;
    obj->rx_buff.pos = 0;
    obj->rx_buff.width = obj->tx_buff.width;

    obj->spi.event = event;

    DEBUG_PRINTF("SPI: Transfer: %u, %u\n", tx_length, rx_length);

    // register the thunking handler
    IRQn_Type irq_n = spiobj->spiIRQ;
    NVIC_SetVector(irq_n, (uint32_t)handler);

    // enable the right hal transfer
    if (use_tx && use_rx) {
        // we cannot manage different rx / tx sizes, let's use smaller one
        size_t size = (tx_length < rx_length)? tx_length : rx_length;
        if(tx_length != rx_length) {
            DEBUG_PRINTF("SPI: Full duplex transfer only 1 size: %d\n", size);
            obj->tx_buff.length = size;
            obj->rx_buff.length = size;
        }
        spi_master_start_asynch_transfer(obj, SPI_TRANSFER_TYPE_TXRX, tx, rx, size);
    } else if (use_tx) {
        spi_master_start_asynch_transfer(obj, SPI_TRANSFER_TYPE_TX, tx, NULL, tx_length);
    } else if (use_rx) {
        spi_master_start_asynch_transfer(obj, SPI_TRANSFER_TYPE_RX, NULL, rx, rx_length);
    }
}

inline uint32_t spi_irq_handler_asynch(spi_t *obj)
{
    int event = 0;

    // call the CubeF4 handler, this will update the handle
    HAL_SPI_IRQHandler(&obj->spi.handle);

    if (obj->spi.handle.State == HAL_SPI_STATE_READY) {
        // When HAL SPI is back to READY state, check if there was an error
        int error = obj->spi.handle.ErrorCode;
        if(error != HAL_SPI_ERROR_NONE) {
            // something went wrong and the transfer has definitely completed
            event = SPI_EVENT_ERROR | SPI_EVENT_INTERNAL_TRANSFER_COMPLETE;

            if (error & HAL_SPI_ERROR_OVR) {
                // buffer overrun
                event |= SPI_EVENT_RX_OVERFLOW;
            }
        } else {
            // else we're done
            event = SPI_EVENT_COMPLETE | SPI_EVENT_INTERNAL_TRANSFER_COMPLETE;
       }
       // enable the interrupt
       NVIC_DisableIRQ(obj->spi.spiIRQ);
       NVIC_ClearPendingIRQ(obj->spi.spiIRQ);
    }


    return (event & (obj->spi.event | SPI_EVENT_INTERNAL_TRANSFER_COMPLETE));
}

uint8_t spi_active(spi_t *obj)
{
    struct spi_s *spiobj = SPI_S(obj);
    SPI_HandleTypeDef *handle = &(spiobj->handle);
    HAL_SPI_StateTypeDef state = HAL_SPI_GetState(handle);

    switch(state) {
        case HAL_SPI_STATE_RESET:
        case HAL_SPI_STATE_READY:
        case HAL_SPI_STATE_ERROR:
            return 0;
        default:
            return 1;
    }
}

void spi_abort_asynch(spi_t *obj)
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
