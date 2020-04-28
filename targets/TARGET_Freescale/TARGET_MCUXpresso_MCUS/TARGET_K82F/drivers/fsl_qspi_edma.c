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

#include "fsl_qspi_edma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*<! Structure definition for qspi_edma_private_handle_t. The structure is private. */
typedef struct _qspi_edma_private_handle
{
    QuadSPI_Type *base;
    qspi_edma_handle_t *handle;
} qspi_edma_private_handle_t;

/* QSPI EDMA transfer handle. */
enum _qspi_edma_tansfer_states
{
    kQSPI_Idle,   /* TX idle. */
    kQSPI_BusBusy /* RX busy. */
};

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*<! Private handle only used for internally. */
static qspi_edma_private_handle_t s_edmaPrivateHandle[FSL_FEATURE_SOC_QuadSPI_COUNT][2];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief QSPI EDMA send finished callback function.
 *
 * This function is called when QSPI EDMA send finished. It disables the QSPI
 * TX EDMA request and sends @ref kStatus_QSPI_TxIdle to QSPI callback.
 *
 * @param handle The EDMA handle.
 * @param param Callback function parameter.
 */
static void QSPI_SendEDMACallback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds);

/*!
 * @brief QSPI EDMA receive finished callback function.
 *
 * This function is called when QSPI EDMA receive finished. It disables the QSPI
 * RX EDMA request and sends @ref kStatus_QSPI_RxIdle to QSPI callback.
 *
 * @param handle The EDMA handle.
 * @param param Callback function parameter.
 */
static void QSPI_ReceiveEDMACallback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds);

/*!
 * @brief Get the QSPI instance from peripheral base address.
 *
 * @param base QSPI peripheral base address.
 * @return QSPI instance.
 */
extern uint32_t QSPI_GetInstance(QuadSPI_Type *base);

/*******************************************************************************
 * Code
 ******************************************************************************/

static void QSPI_SendEDMACallback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    qspi_edma_private_handle_t *qspiPrivateHandle = (qspi_edma_private_handle_t *)param;

    /* Avoid the warning for unused variables. */
    handle = handle;
    tcds = tcds;

    if (transferDone)
    {
        QSPI_TransferAbortSendEDMA(qspiPrivateHandle->base, qspiPrivateHandle->handle);

        if (qspiPrivateHandle->handle->callback)
        {
            qspiPrivateHandle->handle->callback(qspiPrivateHandle->base, qspiPrivateHandle->handle, kStatus_QSPI_Idle,
                                                qspiPrivateHandle->handle->userData);
        }
    }
}

static void QSPI_ReceiveEDMACallback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    qspi_edma_private_handle_t *qspiPrivateHandle = (qspi_edma_private_handle_t *)param;

    /* Avoid warning for unused parameters. */
    handle = handle;
    tcds = tcds;

    if (transferDone)
    {
        /* Disable transfer. */
        QSPI_TransferAbortReceiveEDMA(qspiPrivateHandle->base, qspiPrivateHandle->handle);

        if (qspiPrivateHandle->handle->callback)
        {
            qspiPrivateHandle->handle->callback(qspiPrivateHandle->base, qspiPrivateHandle->handle, kStatus_QSPI_Idle,
                                                qspiPrivateHandle->handle->userData);
        }
    }
}

void QSPI_TransferTxCreateHandleEDMA(QuadSPI_Type *base,
                                     qspi_edma_handle_t *handle,
                                     qspi_edma_callback_t callback,
                                     void *userData,
                                     edma_handle_t *dmaHandle)
{
    assert(handle);

    uint32_t instance = QSPI_GetInstance(base);

    s_edmaPrivateHandle[instance][0].base = base;
    s_edmaPrivateHandle[instance][0].handle = handle;

    memset(handle, 0, sizeof(*handle));

    handle->state = kQSPI_Idle;
    handle->dmaHandle = dmaHandle;

    handle->callback = callback;
    handle->userData = userData;

    /* Get the watermark value */
    handle->count = base->TBCT + 1;

    /* Configure TX edma callback */
    EDMA_SetCallback(handle->dmaHandle, QSPI_SendEDMACallback, &s_edmaPrivateHandle[instance][0]);
}

void QSPI_TransferRxCreateHandleEDMA(QuadSPI_Type *base,
                                     qspi_edma_handle_t *handle,
                                     qspi_edma_callback_t callback,
                                     void *userData,
                                     edma_handle_t *dmaHandle)
{
    assert(handle);

    uint32_t instance = QSPI_GetInstance(base);

    s_edmaPrivateHandle[instance][1].base = base;
    s_edmaPrivateHandle[instance][1].handle = handle;

    memset(handle, 0, sizeof(*handle));

    handle->state = kQSPI_Idle;
    handle->dmaHandle = dmaHandle;

    handle->callback = callback;
    handle->userData = userData;

    /* Get the watermark value */
    handle->count = (base->RBCT & QuadSPI_RBCT_WMRK_MASK) + 1;

    /* Configure RX edma callback */
    EDMA_SetCallback(handle->dmaHandle, QSPI_ReceiveEDMACallback, &s_edmaPrivateHandle[instance][1]);
}

status_t QSPI_TransferSendEDMA(QuadSPI_Type *base, qspi_edma_handle_t *handle, qspi_transfer_t *xfer)
{
    assert(handle && (handle->dmaHandle));

    edma_transfer_config_t xferConfig;
    status_t status;

    /* If previous TX not finished. */
    if (kQSPI_BusBusy == handle->state)
    {
        status = kStatus_QSPI_Busy;
    }
    else
    {
        handle->state = kQSPI_BusBusy;

        /* Prepare transfer. */
        EDMA_PrepareTransfer(&xferConfig, xfer->data, sizeof(uint32_t), (void *)QSPI_GetTxDataRegisterAddress(base),
                             sizeof(uint32_t), (sizeof(uint32_t) * handle->count), xfer->dataSize,
                             kEDMA_MemoryToPeripheral);

        /* Store the initially configured eDMA minor byte transfer count into the QSPI handle */
        handle->nbytes = (sizeof(uint32_t) * handle->count);

        /* Submit transfer. */
        EDMA_SubmitTransfer(handle->dmaHandle, &xferConfig);
        EDMA_StartTransfer(handle->dmaHandle);

        /* Enable QSPI TX EDMA. */
        QSPI_EnableDMA(base, kQSPI_TxBufferFillDMAEnable, true);

        status = kStatus_Success;
    }

    return status;
}

status_t QSPI_TransferReceiveEDMA(QuadSPI_Type *base, qspi_edma_handle_t *handle, qspi_transfer_t *xfer)
{
    assert(handle && (handle->dmaHandle));

    edma_transfer_config_t xferConfig;
    status_t status;

    /* If previous TX not finished. */
    if (kQSPI_BusBusy == handle->state)
    {
        status = kStatus_QSPI_Busy;
    }
    else
    {
        handle->state = kQSPI_BusBusy;

        /* Prepare transfer. */
        EDMA_PrepareTransfer(&xferConfig, (void *)QSPI_GetRxDataRegisterAddress(base), sizeof(uint32_t), xfer->data,
                             sizeof(uint32_t), (sizeof(uint32_t) * handle->count), xfer->dataSize,
                             kEDMA_MemoryToMemory);

        /* Store the initially configured eDMA minor byte transfer count into the QSPI handle */
        handle->nbytes = (sizeof(uint32_t) * handle->count);
        /* Submit transfer. */
        EDMA_SubmitTransfer(handle->dmaHandle, &xferConfig);
        handle->dmaHandle->base->TCD[handle->dmaHandle->channel].ATTR |= DMA_ATTR_SMOD(0x5U);
        EDMA_StartTransfer(handle->dmaHandle);

        /* Enable QSPI TX EDMA. */
        QSPI_EnableDMA(base, kQSPI_RxBufferDrainDMAEnable, true);

        status = kStatus_Success;
    }

    return status;
}

void QSPI_TransferAbortSendEDMA(QuadSPI_Type *base, qspi_edma_handle_t *handle)
{
    assert(handle && (handle->dmaHandle));

    /* Disable QSPI TX EDMA. */
    QSPI_EnableDMA(base, kQSPI_TxBufferFillDMAEnable, false);

    /* Stop transfer. */
    EDMA_AbortTransfer(handle->dmaHandle);

    handle->state = kQSPI_Idle;
}

void QSPI_TransferAbortReceiveEDMA(QuadSPI_Type *base, qspi_edma_handle_t *handle)
{
    assert(handle && (handle->dmaHandle));

    /* Disable QSPI RX EDMA. */
    QSPI_EnableDMA(base, kQSPI_RxBufferDrainDMAEnable, false);

    /* Stop transfer. */
    EDMA_AbortTransfer(handle->dmaHandle);

    handle->state = kQSPI_Idle;
}

status_t QSPI_TransferGetSendCountEDMA(QuadSPI_Type *base, qspi_edma_handle_t *handle, size_t *count)
{
    assert(handle);

    status_t status = kStatus_Success;

    if (handle->state != kQSPI_BusBusy)
    {
        status = kStatus_NoTransferInProgress;
    }
    else
    {
        *count = (handle->transferSize -
                  (uint32_t)handle->nbytes *
                      EDMA_GetRemainingMajorLoopCount(handle->dmaHandle->base, handle->dmaHandle->channel));
    }

    return status;
}

status_t QSPI_TransferGetReceiveCountEDMA(QuadSPI_Type *base, qspi_edma_handle_t *handle, size_t *count)
{
    assert(handle);

    status_t status = kStatus_Success;

    if (handle->state != kQSPI_BusBusy)
    {
        status = kStatus_NoTransferInProgress;
    }
    else
    {
        *count = (handle->transferSize -
                  (uint32_t)handle->nbytes *
                      EDMA_GetRemainingMajorLoopCount(handle->dmaHandle->base, handle->dmaHandle->channel));
    }

    return status;
}
