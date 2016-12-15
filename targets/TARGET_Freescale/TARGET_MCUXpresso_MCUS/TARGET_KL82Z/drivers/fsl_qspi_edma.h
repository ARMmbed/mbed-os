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

#ifndef _FSL_QSPI_EDMA_H_
#define _FSL_QSPI_EDMA_H_

#include "fsl_qspi.h"
#include "fsl_edma.h"

/*!
 * @addtogroup qspi_edma
 * @{
 */


/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct _qspi_edma_handle qspi_edma_handle_t;

/*! @brief QSPI eDMA transfer callback function for finish and error */
typedef void (*qspi_edma_callback_t)(QuadSPI_Type *base, qspi_edma_handle_t *handle, status_t status, void *userData);

/*! @brief QSPI DMA transfer handle, users should not touch the content of the handle.*/
struct _qspi_edma_handle
{
    edma_handle_t *dmaHandle;      /*!< eDMA handler for QSPI send */
    size_t transferSize;           /*!< Bytes need to transfer. */
    uint8_t count;                 /*!< The transfer data count in a DMA request */
    uint32_t state;                /*!< Internal state for QSPI eDMA transfer */
    qspi_edma_callback_t callback; /*!< Callback for users while transfer finish or error occurred */
    void *userData;                /*!< User callback parameter */
};

/*******************************************************************************
 * APIs
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name eDMA Transactional
 * @{
 */

/*!
 * @brief Initializes the QSPI handle for send which is used in transactional functions and set the callback.
 *
 * @param base QSPI peripheral base address
 * @param handle Pointer to qspi_edma_handle_t structure
 * @param callback QSPI callback, NULL means no callback.
 * @param userData User callback function data.
 * @param rxDmaHandle User requested eDMA handle for eDMA transfer
 */
void QSPI_TransferTxCreateHandleEDMA(QuadSPI_Type *base,
                                     qspi_edma_handle_t *handle,
                                     qspi_edma_callback_t callback,
                                     void *userData,
                                     edma_handle_t *dmaHandle);

/*!
 * @brief Initializes the QSPI handle for receive which is used in transactional functions and set the callback.
 *
 * @param base QSPI peripheral base address
 * @param handle Pointer to qspi_edma_handle_t structure
 * @param callback QSPI callback, NULL means no callback.
 * @param userData User callback function data.
 * @param rxDmaHandle User requested eDMA handle for eDMA transfer
 */
void QSPI_TransferRxCreateHandleEDMA(QuadSPI_Type *base,
                                     qspi_edma_handle_t *handle,
                                     qspi_edma_callback_t callback,
                                     void *userData,
                                     edma_handle_t *dmaHandle);

/*!
 * @brief Transfers QSPI data using an eDMA non-blocking method.
 *
 * This function writes data to the QSPI transmit FIFO. This function is non-blocking.
 * @param base Pointer to QuadSPI Type.
 * @param handle Pointer to qspi_edma_handle_t structure
 * @param xfer QSPI transfer structure.
 */
status_t QSPI_TransferSendEDMA(QuadSPI_Type *base, qspi_edma_handle_t *handle, qspi_transfer_t *xfer);

/*!
 * @brief Receives data using an eDMA non-blocking method.
 *
 * This function receive data from the QSPI receive buffer/FIFO. This function is non-blocking.
 * @param base Pointer to QuadSPI Type.
 * @param handle Pointer to qspi_edma_handle_t structure
 * @param xfer QSPI transfer structure.
 */
status_t QSPI_TransferReceiveEDMA(QuadSPI_Type *base, qspi_edma_handle_t *handle, qspi_transfer_t *xfer);

/*!
 * @brief Aborts the sent data using eDMA.
 *
 * This function aborts the sent data using eDMA.
 *
 * @param base QSPI peripheral base address.
 * @param handle Pointer to qspi_edma_handle_t structure
 */
void QSPI_TransferAbortSendEDMA(QuadSPI_Type *base, qspi_edma_handle_t *handle);

/*!
 * @brief Aborts the receive data using eDMA.
 *
 * This function abort receive data which using eDMA.
 *
 * @param base QSPI peripheral base address.
 * @param handle Pointer to qspi_edma_handle_t structure
 */
void QSPI_TransferAbortReceiveEDMA(QuadSPI_Type *base, qspi_edma_handle_t *handle);

/*!
 * @brief Gets the transferred counts of send.
 *
 * @param base Pointer to QuadSPI Type.
 * @param handle Pointer to qspi_edma_handle_t structure.
 * @param count Bytes sent.
 * @retval kStatus_Success Succeed get the transfer count.
 * @retval kStatus_NoTransferInProgress There is not a non-blocking transaction currently in progress.
 */
status_t QSPI_TransferGetSendCountEDMA(QuadSPI_Type *base, qspi_edma_handle_t *handle, size_t *count);

/*!
 * @brief Gets the status of the receive transfer.
 *
 * @param base Pointer to QuadSPI Type.
 * @param handle Pointer to qspi_edma_handle_t structure
 * @param count Bytes received.
 * @retval kStatus_Success Succeed get the transfer count.
 * @retval kStatus_NoTransferInProgress There is not a non-blocking transaction currently in progress.
 */
status_t QSPI_TransferGetReceiveCountEDMA(QuadSPI_Type *base, qspi_edma_handle_t *handle, size_t *count);

/* @} */

#if defined(__cplusplus)
}
#endif

/* @} */

#endif /* _FSL_QSPI_EDMA_H_ */
