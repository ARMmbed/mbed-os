/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_flexio_spi_dma.h"

/*******************************************************************************
 * Definitons
 ******************************************************************************/

/*<! Structure definition for spi_dma_private_handle_t. The structure is private. */
typedef struct _flexio_spi_master_dma_private_handle
{
    FLEXIO_SPI_Type *base;
    flexio_spi_master_dma_handle_t *handle;
} flexio_spi_master_dma_private_handle_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief DMA callback function for FLEXIO SPI send transfer.
 *
 * @param handle DMA handle pointer.
 * @param param Callback function parameter.
 */
static void FLEXIO_SPI_TxDMACallback(dma_handle_t *handle, void *param);

/*!
 * @brief DMA callback function for FLEXIO SPI receive transfer.
 *
 * @param handle DMA handle pointer.
 * @param param Callback function parameter.
 */
static void FLEXIO_SPI_RxDMACallback(dma_handle_t *handle, void *param);

/*!
 * @brief EDMA config for FLEXIO SPI transfer.
 *
 * @param base pointer to FLEXIO_SPI_Type structure.
 * @param handle pointer to flexio_spi_master_dma_handle_t structure to store the transfer state.
 * @param xfer Pointer to flexio spi transfer structure.
 */
static void FLEXIO_SPI_DMAConfig(FLEXIO_SPI_Type *base,
                                 flexio_spi_master_dma_handle_t *handle,
                                 flexio_spi_transfer_t *xfer);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Dummy data used to send */
static const uint16_t s_dummyData = FLEXIO_SPI_DUMMYDATA;

/*< @brief user configurable flexio spi handle count. */
#define FLEXIO_SPI_HANDLE_COUNT 2

/*<! Private handle only used for internally. */
static flexio_spi_master_dma_private_handle_t s_dmaPrivateHandle[FLEXIO_SPI_HANDLE_COUNT];

/*******************************************************************************
* Code
******************************************************************************/

static void FLEXIO_SPI_TxDMACallback(dma_handle_t *handle, void *param)
{
    flexio_spi_master_dma_private_handle_t *spiPrivateHandle = (flexio_spi_master_dma_private_handle_t *)param;

    /* Disable Tx DMA. */
    FLEXIO_SPI_EnableDMA(spiPrivateHandle->base, kFLEXIO_SPI_TxDmaEnable, false);

    /* Disable interrupt. */
    DMA_DisableInterrupts(handle->base, handle->channel);

    /* change the state. */
    spiPrivateHandle->handle->txInProgress = false;

    /* All finished, call the callback. */
    if ((spiPrivateHandle->handle->txInProgress == false) && (spiPrivateHandle->handle->rxInProgress == false))
    {
        if (spiPrivateHandle->handle->callback)
        {
            (spiPrivateHandle->handle->callback)(spiPrivateHandle->base, spiPrivateHandle->handle, kStatus_Success,
                                                 spiPrivateHandle->handle->userData);
        }
    }
}

static void FLEXIO_SPI_RxDMACallback(dma_handle_t *handle, void *param)
{
    flexio_spi_master_dma_private_handle_t *spiPrivateHandle = (flexio_spi_master_dma_private_handle_t *)param;

    /* Disable Rx DMA. */
    FLEXIO_SPI_EnableDMA(spiPrivateHandle->base, kFLEXIO_SPI_RxDmaEnable, false);

    /* Disable interrupt. */
    DMA_DisableInterrupts(handle->base, handle->channel);

    /* change the state. */
    spiPrivateHandle->handle->rxInProgress = false;

    /* All finished, call the callback. */
    if ((spiPrivateHandle->handle->txInProgress == false) && (spiPrivateHandle->handle->rxInProgress == false))
    {
        if (spiPrivateHandle->handle->callback)
        {
            (spiPrivateHandle->handle->callback)(spiPrivateHandle->base, spiPrivateHandle->handle, kStatus_Success,
                                                 spiPrivateHandle->handle->userData);
        }
    }
}

static void FLEXIO_SPI_DMAConfig(FLEXIO_SPI_Type *base,
                                 flexio_spi_master_dma_handle_t *handle,
                                 flexio_spi_transfer_t *xfer)
{
    dma_transfer_config_t xferConfig;
    flexio_spi_shift_direction_t direction;
    uint8_t bytesPerFrame;

    /* Configure the values in handle. */
    switch (xfer->flags)
    {
        case kFLEXIO_SPI_8bitMsb:
            bytesPerFrame = 1;
            direction = kFLEXIO_SPI_MsbFirst;
            break;
        case kFLEXIO_SPI_8bitLsb:
            bytesPerFrame = 1;
            direction = kFLEXIO_SPI_LsbFirst;
            break;
        case kFLEXIO_SPI_16bitMsb:
            bytesPerFrame = 2;
            direction = kFLEXIO_SPI_MsbFirst;
            break;
        case kFLEXIO_SPI_16bitLsb:
            bytesPerFrame = 2;
            direction = kFLEXIO_SPI_LsbFirst;
            break;
        default:
            bytesPerFrame = 1;
            direction = kFLEXIO_SPI_MsbFirst;
            assert(true);
            break;
    }

    /* Save total transfer size. */
    handle->transferSize = xfer->dataSize;

    /* Configure tx transfer DMA. */
    xferConfig.destAddr = FLEXIO_SPI_GetTxDataRegisterAddress(base, direction);
    xferConfig.enableDestIncrement = false;
    if (bytesPerFrame == 1U)
    {
        xferConfig.srcSize = kDMA_Transfersize8bits;
        xferConfig.destSize = kDMA_Transfersize8bits;
    }
    else
    {
        if (direction == kFLEXIO_SPI_MsbFirst)
        {
            xferConfig.destAddr -= 1U;
        }
        xferConfig.srcSize = kDMA_Transfersize16bits;
        xferConfig.destSize = kDMA_Transfersize16bits;
    }

    /* Configure DMA channel. */
    if (xfer->txData)
    {
        xferConfig.enableSrcIncrement = true;
        xferConfig.srcAddr = (uint32_t)(xfer->txData);
    }
    else
    {
        /* Disable the source increasement and source set to dummyData. */
        xferConfig.enableSrcIncrement = false;
        xferConfig.srcAddr = (uint32_t)(&s_dummyData);
    }

    xferConfig.transferSize = xfer->dataSize;

    if (handle->txHandle)
    {
        DMA_SubmitTransfer(handle->txHandle, &xferConfig, kDMA_EnableInterrupt);
    }

    /* Configure tx transfer DMA. */
    if (xfer->rxData)
    {
        xferConfig.srcAddr = FLEXIO_SPI_GetRxDataRegisterAddress(base, direction);
        xferConfig.enableSrcIncrement = false;
        xferConfig.destAddr = (uint32_t)(xfer->rxData);
        xferConfig.enableDestIncrement = true;
        DMA_SubmitTransfer(handle->rxHandle, &xferConfig, kDMA_EnableInterrupt);
        handle->rxInProgress = true;
        FLEXIO_SPI_EnableDMA(base, kFLEXIO_SPI_RxDmaEnable, true);
        DMA_StartTransfer(handle->rxHandle);
    }

    /* Always start Tx transfer. */
    if (handle->txHandle)
    {
        handle->txInProgress = true;
        FLEXIO_SPI_EnableDMA(base, kFLEXIO_SPI_TxDmaEnable, true);
        DMA_StartTransfer(handle->txHandle);
    }
}

status_t FLEXIO_SPI_MasterTransferCreateHandleDMA(FLEXIO_SPI_Type *base,
                                                  flexio_spi_master_dma_handle_t *handle,
                                                  flexio_spi_master_dma_transfer_callback_t callback,
                                                  void *userData,
                                                  dma_handle_t *txHandle,
                                                  dma_handle_t *rxHandle)
{
    assert(handle);

    uint8_t index = 0;

    /* Find the an empty handle pointer to store the handle. */
    for (index = 0; index < FLEXIO_SPI_HANDLE_COUNT; index++)
    {
        if (s_dmaPrivateHandle[index].base == NULL)
        {
            s_dmaPrivateHandle[index].base = base;
            s_dmaPrivateHandle[index].handle = handle;
            break;
        }
    }

    if (index == FLEXIO_SPI_HANDLE_COUNT)
    {
        return kStatus_OutOfRange;
    }

    /* Set spi base to handle. */
    handle->txHandle = txHandle;
    handle->rxHandle = rxHandle;

    /* Register callback and userData. */
    handle->callback = callback;
    handle->userData = userData;

    /* Set SPI state to idle. */
    handle->txInProgress = false;
    handle->rxInProgress = false;

    /* Install callback for Tx/Rx dma channel. */
    if (handle->txHandle)
    {
        DMA_SetCallback(handle->txHandle, FLEXIO_SPI_TxDMACallback, &s_dmaPrivateHandle[index]);
    }
    if (handle->rxHandle)
    {
        DMA_SetCallback(handle->rxHandle, FLEXIO_SPI_RxDMACallback, &s_dmaPrivateHandle[index]);
    }

    return kStatus_Success;
}

status_t FLEXIO_SPI_MasterTransferDMA(FLEXIO_SPI_Type *base,
                                      flexio_spi_master_dma_handle_t *handle,
                                      flexio_spi_transfer_t *xfer)
{
    assert(handle);
    assert(xfer);

    uint32_t dataMode = 0;
    uint16_t timerCmp = base->flexioBase->TIMCMP[base->timerIndex[0]];

    timerCmp &= 0x00FFU;

    /* Check if the device is busy. */
    if ((handle->txInProgress) || (handle->rxInProgress))
    {
        return kStatus_FLEXIO_SPI_Busy;
    }

    /* Check if input parameter invalid. */
    if (((xfer->txData == NULL) && (xfer->rxData == NULL)) || (xfer->dataSize == 0U))
    {
        return kStatus_InvalidArgument;
    }

    /* configure data mode. */
    if ((xfer->flags == kFLEXIO_SPI_8bitMsb) || (xfer->flags == kFLEXIO_SPI_8bitLsb))
    {
        dataMode = (8 * 2 - 1U) << 8U;
    }
    else if ((xfer->flags == kFLEXIO_SPI_16bitMsb) || (xfer->flags == kFLEXIO_SPI_16bitLsb))
    {
        dataMode = (16 * 2 - 1U) << 8U;
    }
    else
    {
        dataMode = 8 * 2 - 1U;
    }

    dataMode |= timerCmp;

    base->flexioBase->TIMCMP[base->timerIndex[0]] = dataMode;

    FLEXIO_SPI_DMAConfig(base, handle, xfer);

    return kStatus_Success;
}

status_t FLEXIO_SPI_MasterTransferGetCountDMA(FLEXIO_SPI_Type *base,
                                              flexio_spi_master_dma_handle_t *handle,
                                              size_t *count)
{
    assert(handle);

    if (!count)
    {
        return kStatus_InvalidArgument;
    }

    if (handle->rxInProgress)
    {
        *count = (handle->transferSize - DMA_GetRemainingBytes(handle->rxHandle->base, handle->rxHandle->channel));
    }
    else
    {
        *count = (handle->transferSize - DMA_GetRemainingBytes(handle->txHandle->base, handle->txHandle->channel));
    }

    return kStatus_Success;
}

void FLEXIO_SPI_MasterTransferAbortDMA(FLEXIO_SPI_Type *base, flexio_spi_master_dma_handle_t *handle)
{
    assert(handle);

    /* Disable dma. */
    DMA_AbortTransfer(handle->txHandle);
    DMA_AbortTransfer(handle->rxHandle);

    /* Disable DMA enable bit. */
    FLEXIO_SPI_EnableDMA(base, kFLEXIO_SPI_DmaAllEnable, false);

    /* Set the handle state. */
    handle->txInProgress = false;
    handle->rxInProgress = false;
}

status_t FLEXIO_SPI_SlaveTransferDMA(FLEXIO_SPI_Type *base,
                                     flexio_spi_slave_dma_handle_t *handle,
                                     flexio_spi_transfer_t *xfer)
{
    assert(handle);
    assert(xfer);

    uint32_t dataMode = 0;

    /* Check if the device is busy. */
    if ((handle->txInProgress) || (handle->rxInProgress))
    {
        return kStatus_FLEXIO_SPI_Busy;
    }

    /* Check if input parameter invalid. */
    if (((xfer->txData == NULL) && (xfer->rxData == NULL)) || (xfer->dataSize == 0U))
    {
        return kStatus_InvalidArgument;
    }

    /* configure data mode. */
    if ((xfer->flags == kFLEXIO_SPI_8bitMsb) || (xfer->flags == kFLEXIO_SPI_8bitLsb))
    {
        dataMode = 8 * 2 - 1U;
    }
    else if ((xfer->flags == kFLEXIO_SPI_16bitMsb) || (xfer->flags == kFLEXIO_SPI_16bitLsb))
    {
        dataMode = 16 * 2 - 1U;
    }
    else
    {
        dataMode = 8 * 2 - 1U;
    }

    base->flexioBase->TIMCMP[base->timerIndex[0]] = dataMode;

    FLEXIO_SPI_DMAConfig(base, handle, xfer);

    return kStatus_Success;
}
