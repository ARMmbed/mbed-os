/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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

#include "fsl_dma.h"
#include "fsl_i2s_dma.h"
#include "fsl_flexcomm.h"
#include <string.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DMA_MAX_TRANSFER_BYTES (DMA_MAX_TRANSFER_COUNT * sizeof(uint32_t))
#define DMA_DESCRIPTORS (2U)
#define ENQUEUED_BYTES_BUFFER_SIZE (DMA_DESCRIPTORS + 1U)
#define I2S_FIFO_DEPTH (8U)

/*<! @brief Structure for statically allocated private data. */
typedef struct _i2s_dma_private_handle
{
    I2S_Type *base;           /*!< I2S base address */
    i2s_dma_handle_t *handle; /*!< I2S handle */
    volatile uint16_t
        enqueuedBytes[ENQUEUED_BYTES_BUFFER_SIZE]; /*!< Number of bytes being transferred by DMA descriptors */
    volatile uint8_t enqueuedBytesStart;           /*!< First item in enqueuedBytes (for reading) */
    volatile uint8_t enqueuedBytesEnd;             /*!< Last item in enqueuedBytes (for adding) */
    volatile bool initialDescriptor;               /*!< Initial DMA descriptor transfer not finished yet */
    volatile uint8_t
        dmaDescriptorsUsed; /*!< Number of DMA descriptors with valid data (in queue, excluding initial descriptor) */
    volatile uint8_t descriptor;      /*!< Index of next descriptor to be configured with data */
    volatile uint8_t queueDescriptor; /*!< Queue index of buffer to be actually consumed by DMA */
    volatile i2s_transfer_t descriptorQueue[I2S_NUM_BUFFERS]; /*!< Transfer data as queued to descriptors for DMA */
} i2s_dma_private_handle_t;

/*! @brief I2S DMA transfer private state. */
enum _i2s_dma_state
{
    kI2S_DmaStateIdle = 0x0U, /*!< I2S is in idle state */
    kI2S_DmaStateTx,          /*!< I2S is busy transmitting data */
    kI2S_DmaStateRx,          /*!< I2S is busy receiving data */
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static status_t I2S_EnqueueUserBuffer(I2S_Type *base, i2s_dma_handle_t *handle, i2s_transfer_t transfer);
static uint32_t I2S_GetInstance(I2S_Type *base);
static void I2S_TxEnableDMA(I2S_Type *base, bool enable);
static void I2S_RxEnableDMA(I2S_Type *base, bool enable);
static status_t I2S_StartTransferDMA(I2S_Type *base, i2s_dma_handle_t *handle);
static status_t I2S_AddTransferDMA(I2S_Type *base, i2s_dma_handle_t *handle, const uint16_t maxSize);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*<! @brief DMA transfer descriptors. */
#if defined(__ICCARM__)
#pragma data_alignment = 16
static dma_descriptor_t s_DmaDescriptors[DMA_DESCRIPTORS * FSL_FEATURE_SOC_I2S_COUNT];
#elif defined(__CC_ARM)
__attribute__((aligned(16))) static dma_descriptor_t s_DmaDescriptors[DMA_DESCRIPTORS * FSL_FEATURE_SOC_I2S_COUNT];
#elif defined(__GNUC__)
__attribute__((aligned(16))) static dma_descriptor_t s_DmaDescriptors[DMA_DESCRIPTORS * FSL_FEATURE_SOC_I2S_COUNT];
#endif

/*<! @brief Buffer with dummy TX data. */
#if defined(__ICCARM__)
#pragma data_alignment = 4
static uint32_t s_DummyBufferTx = 0U;
#elif defined(__CC_ARM)
__attribute__((aligned(4))) static uint32_t s_DummyBufferTx = 0U;
#elif defined(__GNUC__)
__attribute__((aligned(4))) static uint32_t s_DummyBufferTx = 0U;
#endif

/*<! @brief Buffer to fill with RX data. */
#if defined(__ICCARM__)
#pragma data_alignment = 4
static uint32_t s_DummyBufferRx = 0U;
#elif defined(__CC_ARM)
__attribute__((aligned(4))) static uint32_t s_DummyBufferRx = 0U;
#elif defined(__GNUC__)
__attribute__((aligned(4))) static uint32_t s_DummyBufferRx = 0U;
#endif

/*<! @brief Private array of data associated with available I2S peripherals. */
static i2s_dma_private_handle_t s_DmaPrivateHandle[FSL_FEATURE_SOC_I2S_COUNT];

/*<! @brief Base addresses of available I2S peripherals. */
static const uint32_t s_I2sBaseAddrs[FSL_FEATURE_SOC_I2S_COUNT] = I2S_BASE_ADDRS;

/*******************************************************************************
 * Code
 ******************************************************************************/

static status_t I2S_EnqueueUserBuffer(I2S_Type *base, i2s_dma_handle_t *handle, i2s_transfer_t transfer)
{
    uint32_t instance = I2S_GetInstance(base);
    i2s_dma_private_handle_t *privateHandle = &(s_DmaPrivateHandle[instance]);

    /* Validate input data and tranfer buffer */

    assert(handle);
    if (!handle)
    {
        return kStatus_InvalidArgument;
    }

    assert((((uint32_t)transfer.data) % 4U) == 0U);
    if ((((uint32_t)transfer.data) % 4U) != 0U)
    {
        /* Data not 4-bytes aligned */
        return kStatus_InvalidArgument;
    }

    assert(transfer.dataSize != 0U);
    if (transfer.dataSize == 0U)
    {
        /* No data to send or receive */
        return kStatus_InvalidArgument;
    }

    assert((transfer.dataSize % 4U) == 0U);
    if ((transfer.dataSize % 4U) != 0U)
    {
        /* Data length not multiply of 4 bytes */
        return kStatus_InvalidArgument;
    }

    if (handle->i2sQueue[handle->queueUser].dataSize)
    {
        /* Previously prepared buffers not processed yet, reject request */
        return kStatus_I2S_Busy;
    }

    /* Enqueue data */
    privateHandle->descriptorQueue[handle->queueUser].data = transfer.data;
    privateHandle->descriptorQueue[handle->queueUser].dataSize = transfer.dataSize;
    handle->i2sQueue[handle->queueUser].data = transfer.data;
    handle->i2sQueue[handle->queueUser].dataSize = transfer.dataSize;
    handle->queueUser = (handle->queueUser + 1U) % I2S_NUM_BUFFERS;

    return kStatus_Success;
}

static uint32_t I2S_GetInstance(I2S_Type *base)
{
    uint32_t i;

    for (i = 0U; i < FSL_FEATURE_SOC_I2S_COUNT; i++)
    {
        if ((uint32_t)base == s_I2sBaseAddrs[i])
        {
            return i;
        }
    }

    assert(false);
    return 0U;
}

void I2S_TxTransferCreateHandleDMA(I2S_Type *base,
                                   i2s_dma_handle_t *handle,
                                   dma_handle_t *dmaHandle,
                                   i2s_dma_transfer_callback_t callback,
                                   void *userData)
{
    assert(handle);
    assert(dmaHandle);

    uint32_t instance = I2S_GetInstance(base);
    i2s_dma_private_handle_t *privateHandle = &(s_DmaPrivateHandle[instance]);

    memset(handle, 0U, sizeof(*handle));
    handle->state = kI2S_DmaStateIdle;
    handle->dmaHandle = dmaHandle;
    handle->completionCallback = callback;
    handle->userData = userData;

    memset(privateHandle, 0U, sizeof(*privateHandle));
    privateHandle->base = base;
    privateHandle->handle = handle;
    privateHandle->initialDescriptor = false;

    DMA_SetCallback(dmaHandle, I2S_DMACallback, privateHandle);
}

status_t I2S_TxTransferSendDMA(I2S_Type *base, i2s_dma_handle_t *handle, i2s_transfer_t transfer)
{
    /* Enqueue transfer buffer */
    status_t status = I2S_EnqueueUserBuffer(base, handle, transfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Initialize DMA transfer */
    if (handle->state == kI2S_DmaStateIdle)
    {
        handle->state = kI2S_DmaStateTx;
        return I2S_StartTransferDMA(base, handle);
    }

    return kStatus_Success;
}

void I2S_TransferAbortDMA(I2S_Type *base, i2s_dma_handle_t *handle)
{
    assert(handle);
    assert(handle->dmaHandle);

    uint32_t instance = I2S_GetInstance(base);
    i2s_dma_private_handle_t *privateHandle = &(s_DmaPrivateHandle[instance]);

    /* Abort operation */
    DMA_AbortTransfer(handle->dmaHandle);
    if (handle->state == kI2S_DmaStateTx)
    {
        I2S_TxEnableDMA(base, false);
    }
    else
    {
        I2S_RxEnableDMA(base, false);
    }
    I2S_Disable(base);

    /* Reset state */
    handle->state = kI2S_DmaStateIdle;

    /* Clear transfer queue */
    memset((void *)&(handle->i2sQueue), 0U, sizeof(handle->i2sQueue));
    handle->queueDriver = 0U;
    handle->queueUser = 0U;

    /* Clear internal state */
    memset((void *)&(privateHandle->descriptorQueue), 0U, sizeof(privateHandle->descriptorQueue));
    memset((void *)&(privateHandle->enqueuedBytes), 0U, sizeof(privateHandle->enqueuedBytes));
    privateHandle->enqueuedBytesStart = 0U;
    privateHandle->enqueuedBytesEnd = 0U;
    privateHandle->initialDescriptor = false;
    privateHandle->dmaDescriptorsUsed = 0U;
    privateHandle->descriptor = 0U;
    privateHandle->queueDescriptor = 0U;
}

void I2S_RxTransferCreateHandleDMA(I2S_Type *base,
                                   i2s_dma_handle_t *handle,
                                   dma_handle_t *dmaHandle,
                                   i2s_dma_transfer_callback_t callback,
                                   void *userData)
{
    I2S_TxTransferCreateHandleDMA(base, handle, dmaHandle, callback, userData);
}

status_t I2S_RxTransferReceiveDMA(I2S_Type *base, i2s_dma_handle_t *handle, i2s_transfer_t transfer)
{
    /* Enqueue transfer buffer */
    status_t status = I2S_EnqueueUserBuffer(base, handle, transfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Initialize DMA transfer */
    if (handle->state == kI2S_DmaStateIdle)
    {
        handle->state = kI2S_DmaStateRx;
        return I2S_StartTransferDMA(base, handle);
    }

    return kStatus_Success;
}

static void I2S_TxEnableDMA(I2S_Type *base, bool enable)
{
    if (enable)
    {
        base->FIFOCFG |= I2S_FIFOCFG_DMATX_MASK;
    }
    else
    {
        base->FIFOCFG &= (~I2S_FIFOCFG_DMATX_MASK);
        base->FIFOCFG |= I2S_FIFOCFG_EMPTYTX_MASK;
    }
}

static void I2S_RxEnableDMA(I2S_Type *base, bool enable)
{
    if (enable)
    {
        base->FIFOCFG |= I2S_FIFOCFG_DMARX_MASK;
    }
    else
    {
        base->FIFOCFG &= (~I2S_FIFOCFG_DMARX_MASK);
        base->FIFOCFG |= I2S_FIFOCFG_EMPTYRX_MASK;
    }
}

static status_t I2S_StartTransferDMA(I2S_Type *base, i2s_dma_handle_t *handle)
{
    status_t status;
    dma_transfer_config_t xferConfig = {0};
    i2s_dma_private_handle_t *privateHandle;
    i2s_transfer_t volatile *transfer;
    uint16_t transferBytes;
    uint32_t instance;
    uint32_t i;

    instance = I2S_GetInstance(base);
    privateHandle = &(s_DmaPrivateHandle[instance]);
    transfer = &(privateHandle->descriptorQueue[privateHandle->queueDescriptor]);

    /*
     * Divide first buffer between initial DMA descriptor and chained DMA descriptors.
     * This allows to enqueue more data before entire buffer is processed
     * and thus could prevent audio drop-outs due to requeing.
     * But user is expected to enqueue more data in order for this to work.
     */
    transferBytes = (transfer->dataSize / (DMA_DESCRIPTORS + 1U));

    if (transferBytes > DMA_MAX_TRANSFER_BYTES)
    {
        transferBytes = DMA_MAX_TRANSFER_BYTES;
    }

    if ((transferBytes % 4U) != 0U)
    {
        transferBytes -= (transferBytes % 4U);
    }

    assert(transferBytes > 0U);
    if (transferBytes == 0U)
    {
        /*
         * TODO buffer too small to be divided for multiple descriptors
         * return invalid argument status for now
         */
        return kStatus_InvalidArgument;
    }

    /* Prepare transfer of data via initial DMA transfer descriptor */
    DMA_PrepareTransfer(
        &xferConfig, (handle->state == kI2S_DmaStateTx) ? (void *)transfer->data : (void *)&(base->FIFORD),
        (handle->state == kI2S_DmaStateTx) ? (void *)&(base->FIFOWR) : (void *)transfer->data, sizeof(uint32_t),
        transferBytes, (handle->state == kI2S_DmaStateTx) ? kDMA_MemoryToPeripheral : kDMA_PeripheralToMemory,
        (void *)&(s_DmaDescriptors[(instance * FSL_FEATURE_SOC_I2S_COUNT) + 0U]));

    privateHandle->initialDescriptor = true; /* For IRQ handler to know that it will be fired for first descriptor */
    privateHandle->enqueuedBytes[privateHandle->enqueuedBytesEnd] = transferBytes;
    privateHandle->enqueuedBytesEnd = (privateHandle->enqueuedBytesEnd + 1U) % ENQUEUED_BYTES_BUFFER_SIZE;

    transfer->dataSize -= transferBytes;
    transfer->data += transferBytes;
    if (transfer->dataSize == 0U)
    {
        transfer->data = NULL;
        privateHandle->queueDescriptor = (privateHandle->queueDescriptor + 1U) % I2S_NUM_BUFFERS;
    }

    /* Prepare chained descriptors to transfer 2nd and 3rd part of initial data */
    for (i = 0; i < DMA_DESCRIPTORS; i++)
    {
        if (i == (DMA_DESCRIPTORS - 1))
        {
            /* Last buffer will take the rest of data (because of possible truncating by division) */
            transferBytes = transfer->dataSize;
            if (transferBytes > DMA_MAX_TRANSFER_BYTES)
            {
                transferBytes = DMA_MAX_TRANSFER_BYTES;
            }
        }

        I2S_AddTransferDMA(base, handle, transferBytes);
    }

    /* Submit and start initial DMA transfer */

    if (handle->state == kI2S_DmaStateTx)
    {
        I2S_TxEnableDMA(base, true);
    }
    else
    {
        I2S_RxEnableDMA(base, true);
    }

    status = DMA_SubmitTransfer(handle->dmaHandle, &xferConfig);
    if (status != kStatus_Success)
    {
        return status;
    }

    DMA_StartTransfer(handle->dmaHandle);

    I2S_Enable(base);
    return kStatus_Success;
}

static status_t I2S_AddTransferDMA(I2S_Type *base, i2s_dma_handle_t *handle, const uint16_t maxSize)
{
    dma_xfercfg_t xfercfg;
    i2s_transfer_t volatile *transfer;
    uint16_t transferBytes;
    uint32_t instance;
    i2s_dma_private_handle_t *privateHandle;
    dma_descriptor_t *descriptor;
    dma_descriptor_t *nextDescriptor;

    instance = I2S_GetInstance(base);
    privateHandle = &(s_DmaPrivateHandle[instance]);

    if (privateHandle->dmaDescriptorsUsed >= DMA_DESCRIPTORS)
    {
        /* No unprocessed DMA transfer descriptor */
        return kStatus_I2S_Busy;
    }

    transfer = &(privateHandle->descriptorQueue[privateHandle->queueDescriptor]);

    /* Determine currently configured descriptor and the other which it will link to */
    descriptor = &(s_DmaDescriptors[(instance * FSL_FEATURE_SOC_I2S_COUNT) + privateHandle->descriptor]);
    privateHandle->descriptor = (privateHandle->descriptor + 1U) % DMA_DESCRIPTORS;
    nextDescriptor = &(s_DmaDescriptors[(instance * FSL_FEATURE_SOC_I2S_COUNT) + privateHandle->descriptor]);

    if (transfer->dataSize == 0U)
    {
        /* Currently nothing to enqueue, use dummy buffer instead */

        xfercfg.valid = false;
        xfercfg.reload = true;
        xfercfg.swtrig = false;
        xfercfg.clrtrig = false;
        xfercfg.intA = true;
        xfercfg.intB = false;
        xfercfg.byteWidth = sizeof(uint32_t);
        xfercfg.srcInc = 0U;
        xfercfg.dstInc = 0U;
        xfercfg.transferCount = I2S_FIFO_DEPTH + 1U;

        DMA_CreateDescriptor(descriptor, &xfercfg,
                             (handle->state == kI2S_DmaStateTx) ? (void *)&s_DummyBufferTx : (void *)&(base->FIFORD),
                             (handle->state == kI2S_DmaStateTx) ? (void *)&(base->FIFOWR) : (void *)&s_DummyBufferRx,
                             (void *)nextDescriptor);

        /* TODO use API for this once it is available in DMA driver */
        descriptor->xfercfg |= DMA_CHANNEL_XFERCFG_CFGVALID(1U);
        handle->dmaHandle->base->COMMON->SETVALID = (1U << (handle->dmaHandle->channel));

        return kStatus_Success;
    }
    else if (transfer->dataSize > maxSize)
    {
        /* Take part of data - DMA transfer limitation */
        transferBytes = maxSize;
    }
    else
    {
        /* Entire buffer can fit into a single descriptor */
        transferBytes = transfer->dataSize;
        privateHandle->queueDescriptor = (privateHandle->queueDescriptor + 1U) % I2S_NUM_BUFFERS;
    }

    privateHandle->enqueuedBytes[privateHandle->enqueuedBytesEnd] = transferBytes;
    privateHandle->enqueuedBytesEnd = (privateHandle->enqueuedBytesEnd + 1U) % ENQUEUED_BYTES_BUFFER_SIZE;

    /* Configure descriptor */

    xfercfg.valid = false;
    xfercfg.reload = true;
    xfercfg.swtrig = false;
    xfercfg.clrtrig = false;
    xfercfg.intA = true;
    xfercfg.intB = false;
    xfercfg.byteWidth = sizeof(uint32_t);
    xfercfg.srcInc = (handle->state == kI2S_DmaStateTx) ? 1U : 0U;
    xfercfg.dstInc = (handle->state == kI2S_DmaStateTx) ? 0U : 1U;
    xfercfg.transferCount = transferBytes / sizeof(uint32_t);

    DMA_CreateDescriptor(
        descriptor, &xfercfg, (handle->state == kI2S_DmaStateTx) ? (void *)transfer->data : (void *)&(base->FIFORD),
        (handle->state == kI2S_DmaStateTx) ? (void *)&(base->FIFOWR) : (void *)transfer->data, (void *)nextDescriptor);

    /* TODO use API for this once it is available in DMA driver */
    descriptor->xfercfg |= DMA_CHANNEL_XFERCFG_CFGVALID(1U);
    handle->dmaHandle->base->COMMON->SETVALID = (1U << (handle->dmaHandle->channel));

    /* Advance internal state */
    privateHandle->dmaDescriptorsUsed++;
    transfer->dataSize -= transferBytes;
    transfer->data += transferBytes;

    return kStatus_Success;
}

void I2S_DMACallback(dma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds)
{
    i2s_dma_private_handle_t *privateHandle = (i2s_dma_private_handle_t *)userData;
    i2s_dma_handle_t *i2sHandle = privateHandle->handle;
    I2S_Type *base = privateHandle->base;

    if (!transferDone)
    {
        return;
    }

    if (i2sHandle->i2sQueue[i2sHandle->queueDriver].dataSize == 0U)
    {
        /*
         * All user buffers processed before, dataSize already zero,
         * this means at least one dummy buffer sent/received
         * and no new buffer enqueued by user meanwhile.
         */

        I2S_TransferAbortDMA(base, i2sHandle);

        /* Notify user about completion of the final buffer */
        if (i2sHandle->completionCallback)
        {
            (i2sHandle->completionCallback)(base, i2sHandle, kStatus_I2S_Done, i2sHandle->userData);
        }

        return;
    }

    if (privateHandle->initialDescriptor)
    {
        /* Initial descriptor finished, decrease amount of data to be processed */

        assert(privateHandle->enqueuedBytes[privateHandle->enqueuedBytesStart] > 0U);

        i2sHandle->i2sQueue[i2sHandle->queueDriver].dataSize -=
            privateHandle->enqueuedBytes[privateHandle->enqueuedBytesStart];
        i2sHandle->i2sQueue[i2sHandle->queueDriver].data +=
            privateHandle->enqueuedBytes[privateHandle->enqueuedBytesStart];
        privateHandle->enqueuedBytes[privateHandle->enqueuedBytesStart] = 0U;
        privateHandle->enqueuedBytesStart = (privateHandle->enqueuedBytesStart + 1U) % ENQUEUED_BYTES_BUFFER_SIZE;

        privateHandle->initialDescriptor = false;
    }
    else
    {
        if (privateHandle->dmaDescriptorsUsed > 0U)
        {
            /* Finished one of chained descriptors, decrease amount of data to be processed */

            assert(privateHandle->enqueuedBytes[privateHandle->enqueuedBytesStart] > 0U);

            i2sHandle->i2sQueue[i2sHandle->queueDriver].dataSize -=
                privateHandle->enqueuedBytes[privateHandle->enqueuedBytesStart];
            i2sHandle->i2sQueue[i2sHandle->queueDriver].data +=
                privateHandle->enqueuedBytes[privateHandle->enqueuedBytesStart];
            privateHandle->enqueuedBytes[privateHandle->enqueuedBytesStart] = 0U;
            privateHandle->enqueuedBytesStart = (privateHandle->enqueuedBytesStart + 1U) % ENQUEUED_BYTES_BUFFER_SIZE;

            privateHandle->dmaDescriptorsUsed--;
        }
    }

    if (i2sHandle->i2sQueue[i2sHandle->queueDriver].dataSize == 0U)
    {
        /* Entire user buffer sent or received - advance to next one */
        i2sHandle->i2sQueue[i2sHandle->queueDriver].data = NULL;
        i2sHandle->queueDriver = (i2sHandle->queueDriver + 1U) % I2S_NUM_BUFFERS;

        /* Notify user about buffer completion */
        if (i2sHandle->completionCallback)
        {
            (i2sHandle->completionCallback)(base, i2sHandle, kStatus_I2S_BufferComplete, i2sHandle->userData);
        }
    }

    /* Enqueue new chunk of data to DMA (if any) */
    I2S_AddTransferDMA(base, i2sHandle, DMA_MAX_TRANSFER_BYTES);
}
