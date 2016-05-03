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

#include "fsl_flexcan_edma.h"
#include "fsl_dmamux.h"

/*******************************************************************************
 * Definitons
 ******************************************************************************/

/*<! Structure definition for flexcan_edma_private_handle_t. The structure is private. */
typedef struct _flexcan_edma_private_handle
{
    CAN_Type *base;
    flexcan_edma_handle_t *handle;
} flexcan_edma_private_handle_t;

/* FlexCAN EDMA transfer handle. */
enum _flexcan_edma_tansfer_state
{
    KFLEXCAN_RxFifoIdle = 0U, /* Rx Fifo idle. */
    KFLEXCAN_RxFifoBusy = 1U, /* Rx Fifo busy. */
};

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*<! Private handle only used for internally. */
static flexcan_edma_private_handle_t s_edmaPrivateHandle[FSL_FEATURE_SOC_FLEXCAN_COUNT];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief FlexCAN EDMA receive finished callback function.
 *
 * This function is called when FlexCAN Rx FIFO EDMA receive finished.
 * It disables the FlexCAN Rx FIFO EDMA request and sends
 * @ref kStatus_FLEXCAN_RxFifoIdle to FlexCAN EDMA callback.
 *
 * @param handle The EDMA handle.
 * @param param Callback function parameter.
 */
static void FLEXCAN_ReceiveFifoEDMACallback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds);

/*!
 * @brief Get the FlexCAN instance from peripheral base address.
 *
 * @param base FlexCAN peripheral base address.
 * @return FlexCAN instance.
 */
extern uint32_t FLEXCAN_GetInstance(CAN_Type *base);

/*******************************************************************************
 * Code
 ******************************************************************************/

static void FLEXCAN_ReceiveFifoEDMACallback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    handle = handle;
    tcds = tcds;

    flexcan_edma_private_handle_t *flexcanPrivateHandle = (flexcan_edma_private_handle_t *)param;

    if (transferDone)
    {
        /* Disable transfer. */
        FLEXCAN_TransferAbortReceiveFifoEDMA(flexcanPrivateHandle->base, flexcanPrivateHandle->handle);

        if (flexcanPrivateHandle->handle->callback)
        {
            flexcanPrivateHandle->handle->callback(flexcanPrivateHandle->base, flexcanPrivateHandle->handle,
                                                   kStatus_FLEXCAN_RxFifoIdle, flexcanPrivateHandle->handle->userData);
        }
    }
}

void FLEXCAN_TransferCreateHandleEDMA(CAN_Type *base,
                                      flexcan_edma_handle_t *handle,
                                      flexcan_edma_transfer_callback_t callback,
                                      void *userData,
                                      edma_handle_t *rxFifoEdmaHandle)
{
    assert(handle);

    uint32_t instance = FLEXCAN_GetInstance(base);
    s_edmaPrivateHandle[instance].base = base;
    s_edmaPrivateHandle[instance].handle = handle;

    memset(handle, 0, sizeof(flexcan_edma_handle_t));

    handle->rxFifoState = KFLEXCAN_RxFifoIdle;
    handle->rxFifoEdmaHandle = rxFifoEdmaHandle;

    /* Register Callback. */
    handle->callback = callback;
    handle->userData = userData;

    /* Configure Rx FIFO DMA. */
    EDMA_SetCallback(handle->rxFifoEdmaHandle, FLEXCAN_ReceiveFifoEDMACallback, &s_edmaPrivateHandle[instance]);
}

status_t FLEXCAN_TransferReceiveFifoEDMA(CAN_Type *base, flexcan_edma_handle_t *handle, flexcan_fifo_transfer_t *xfer)
{
    assert(handle->rxFifoEdmaHandle);

    edma_transfer_config_t dmaXferConfig;
    status_t status;

    /* If previous Rx FIFO receive not finished. */
    if (KFLEXCAN_RxFifoBusy == handle->rxFifoState)
    {
        status = kStatus_FLEXCAN_RxFifoBusy;
    }
    else
    {
        handle->rxFifoState = KFLEXCAN_RxFifoBusy;

        /* Prepare transfer. */
        EDMA_PrepareTransfer(&dmaXferConfig, (void *)FLEXCAN_GetRxFifoHeadAddr(base), sizeof(flexcan_frame_t),
                             (void *)xfer->frame, sizeof(uint32_t), sizeof(flexcan_frame_t), sizeof(flexcan_frame_t),
                             kEDMA_PeripheralToMemory);
        /* Submit transfer. */
        EDMA_SubmitTransfer(handle->rxFifoEdmaHandle, &dmaXferConfig);
        EDMA_StartTransfer(handle->rxFifoEdmaHandle);

        /* Enable FlexCAN Rx FIFO EDMA. */
        FLEXCAN_EnableRxFifoDMA(base, true);

        status = kStatus_Success;
    }

    return status;
}

void FLEXCAN_TransferAbortReceiveFifoEDMA(CAN_Type *base, flexcan_edma_handle_t *handle)
{
    assert(handle->rxFifoEdmaHandle);

    /* Disable FlexCAN Rx FIFO EDMA. */
    FLEXCAN_EnableRxFifoDMA(base, false);

    /* Stop transfer. */
    EDMA_AbortTransfer(handle->rxFifoEdmaHandle);

    handle->rxFifoState = KFLEXCAN_RxFifoIdle;
}
