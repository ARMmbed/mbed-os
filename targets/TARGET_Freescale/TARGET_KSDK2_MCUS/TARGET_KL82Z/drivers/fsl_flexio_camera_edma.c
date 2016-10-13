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

#include "fsl_flexio_camera_edma.h"
#include "fsl_dmamux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*<! Structure definition for camera_edma_private_handle_t. The structure is private. */
typedef struct _flexio_camera_edma_private_handle
{
    FLEXIO_CAMERA_Type *base;
    flexio_camera_edma_handle_t *handle;
} flexio_camera_edma_private_handle_t;

/* CAMERA EDMA transfer handle. */
enum _flexio_camera_edma_tansfer_states
{
    kFLEXIO_CAMERA_RxIdle, /* RX idle. */
    kFLEXIO_CAMERA_RxBusy  /* RX busy. */
};

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*< @brief user configurable flexio camera handle count. */
#define FLEXIO_CAMERA_HANDLE_COUNT 1

/*<! Private handle only used for internally. */
static flexio_camera_edma_private_handle_t s_edmaPrivateHandle[FLEXIO_CAMERA_HANDLE_COUNT];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief FLEXIO CAMERA EDMA receive finished callback function.
 *
 * This function is called when FLEXIO CAMERA EDMA receive finished. It disables the CAMERA
 * RX EDMA request and sends @ref kStatus_FLEXIO_CAMERA_RxIdle to CAMERA callback.
 *
 * @param handle The EDMA handle.
 * @param param Callback function parameter.
 */
static void FLEXIO_CAMERA_TransferReceiveEDMACallback(edma_handle_t *handle,
                                                      void *param,
                                                      bool transferDone,
                                                      uint32_t tcds);

/*******************************************************************************
 * Code
 ******************************************************************************/

static void FLEXIO_CAMERA_TransferReceiveEDMACallback(edma_handle_t *handle,
                                                      void *param,
                                                      bool transferDone,
                                                      uint32_t tcds)
{
    flexio_camera_edma_private_handle_t *cameraPrivateHandle = (flexio_camera_edma_private_handle_t *)param;

    /* Avoid the warning for unused variables. */
    handle = handle;
    tcds = tcds;

    if (transferDone)
    {
        FLEXIO_CAMERA_TransferAbortReceiveEDMA(cameraPrivateHandle->base, cameraPrivateHandle->handle);

        if (cameraPrivateHandle->handle->callback)
        {
            cameraPrivateHandle->handle->callback(cameraPrivateHandle->base, cameraPrivateHandle->handle,
                                                  kStatus_FLEXIO_CAMERA_RxIdle, cameraPrivateHandle->handle->userData);
        }
    }
}
status_t FLEXIO_CAMERA_TransferCreateHandleEDMA(FLEXIO_CAMERA_Type *base,
                                                flexio_camera_edma_handle_t *handle,
                                                flexio_camera_edma_transfer_callback_t callback,
                                                void *userData,
                                                edma_handle_t *rxEdmaHandle)
{
    assert(handle);

    uint8_t index = 0;

    /* Find the an empty handle pointer to store the handle. */
    for (index = 0; index < FLEXIO_CAMERA_HANDLE_COUNT; index++)
    {
        if (s_edmaPrivateHandle[index].base == NULL)
        {
            s_edmaPrivateHandle[index].base = base;
            s_edmaPrivateHandle[index].handle = handle;
            break;
        }
    }

    if (index == FLEXIO_CAMERA_HANDLE_COUNT)
    {
        return kStatus_OutOfRange;
    }

    s_edmaPrivateHandle[index].base = base;
    s_edmaPrivateHandle[index].handle = handle;

    memset(handle, 0, sizeof(*handle));

    handle->rxState = kFLEXIO_CAMERA_RxIdle;
    handle->rxEdmaHandle = rxEdmaHandle;

    handle->callback = callback;
    handle->userData = userData;

    /* Configure RX. */
    if (rxEdmaHandle)
    {
        EDMA_SetCallback(handle->rxEdmaHandle, FLEXIO_CAMERA_TransferReceiveEDMACallback, &s_edmaPrivateHandle);
    }

    return kStatus_Success;
}

status_t FLEXIO_CAMERA_TransferReceiveEDMA(FLEXIO_CAMERA_Type *base,
                                           flexio_camera_edma_handle_t *handle,
                                           flexio_camera_transfer_t *xfer)
{
    assert(handle->rxEdmaHandle);

    edma_transfer_config_t xferConfig;
    status_t status;

    /* If previous RX not finished. */
    if (kFLEXIO_CAMERA_RxBusy == handle->rxState)
    {
        status = kStatus_FLEXIO_CAMERA_RxBusy;
    }
    else
    {
        handle->rxState = kFLEXIO_CAMERA_RxBusy;

        /* Prepare transfer. */
        EDMA_PrepareTransfer(&xferConfig, (void *)FLEXIO_CAMERA_GetRxBufferAddress(base), 32, (void *)xfer->dataAddress,
                             32, 32, xfer->dataNum, kEDMA_PeripheralToMemory);

        /* Submit transfer. */
        EDMA_SubmitTransfer(handle->rxEdmaHandle, &xferConfig);
        EDMA_StartTransfer(handle->rxEdmaHandle);
        /* Enable CAMERA RX EDMA. */
        FLEXIO_CAMERA_EnableRxDMA(base, true);
        status = kStatus_Success;
    }

    return status;
}

void FLEXIO_CAMERA_TransferAbortReceiveEDMA(FLEXIO_CAMERA_Type *base, flexio_camera_edma_handle_t *handle)
{
    assert(handle->rxEdmaHandle);

    /* Disable CAMERA RX EDMA. */
    FLEXIO_CAMERA_EnableRxDMA(base, false);

    /* Stop transfer. */
    EDMA_StopTransfer(handle->rxEdmaHandle);

    handle->rxState = kFLEXIO_CAMERA_RxIdle;
}

status_t FLEXIO_CAMERA_TransferGetReceiveCountEDMA(FLEXIO_CAMERA_Type *base,
                                                   flexio_camera_edma_handle_t *handle,
                                                   size_t *count)
{
    assert(handle->rxEdmaHandle);

    if (!count)
    {
        return kStatus_InvalidArgument;
    }

    if (kFLEXIO_CAMERA_RxBusy == handle->rxState)
    {
        *count = (handle->rxSize - EDMA_GetRemainingBytes(handle->rxEdmaHandle->base, handle->rxEdmaHandle->channel));
    }
    else
    {
        *count = handle->rxSize;
    }

    return kStatus_Success;
}
