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
#ifndef _FSL_FLEXIO_I2S_DMA_H_
#define _FSL_FLEXIO_I2S_DMA_H_

#include "fsl_flexio_i2s.h"
#include "fsl_dma.h"

/*!
 * @addtogroup flexio_dma_i2s
 * @{
 */

/*! @file */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct _flexio_i2s_dma_handle flexio_i2s_dma_handle_t;

/*! @brief FlexIO I2S DMA transfer callback function for finish and error */
typedef void (*flexio_i2s_dma_callback_t)(FLEXIO_I2S_Type *base,
                                          flexio_i2s_dma_handle_t *handle,
                                          status_t status,
                                          void *userData);

/*! @brief FlexIO I2S DMA transfer handle, users should not touch the content of the handle.*/
struct _flexio_i2s_dma_handle
{
    dma_handle_t *dmaHandle;            /*!< DMA handler for FlexIO I2S send */
    uint8_t bytesPerFrame;              /*!< Bytes in a frame */
    uint32_t state;                     /*!< Internal state for FlexIO I2S DMA transfer */
    flexio_i2s_dma_callback_t callback; /*!< Callback for users while transfer finish or error occurred */
    void *userData;                     /*!< User callback parameter */
    flexio_i2s_transfer_t queue[FLEXIO_I2S_XFER_QUEUE_SIZE]; /*!< Transfer queue storing queued transfer. */
    size_t transferSize[FLEXIO_I2S_XFER_QUEUE_SIZE];         /*!< Data bytes need to transfer */
    volatile uint8_t queueUser;                              /*!< Index for user to queue transfer. */
    volatile uint8_t queueDriver;                            /*!< Index for driver to get the transfer data and size */
};

/*******************************************************************************
 * APIs
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name DMA Transactional
 * @{
 */

/*!
 * @brief Initializes the FlexIO I2S DMA handle.
 *
 * This function initializes the FlexIO I2S master DMA handle which can be used for other FlexIO I2S master
 * transactional APIs.
 * Usually, for a specified FlexIO I2S instance, user need only call this API once to get the initialized handle.
 *
 * @param base FlexIO I2S peripheral base address.
 * @param handle FlexIO I2S DMA handle pointer.
 * @param callback FlexIO I2S DMA callback function called while finished a block.
 * @param userData User parameter for callback.
 * @param dmaHandle DMA handle for FlexIO I2S. This handle shall be a static value allocated by users.
 */
void FLEXIO_I2S_TransferTxCreateHandleDMA(FLEXIO_I2S_Type *base,
                                          flexio_i2s_dma_handle_t *handle,
                                          flexio_i2s_dma_callback_t callback,
                                          void *userData,
                                          dma_handle_t *dmaHandle);

/*!
 * @brief Initializes the FlexIO I2S Rx DMA handle.
 *
 * This function initializes the FlexIO I2S slave DMA handle which can be used for other FlexIO I2S master transactional
 * APIs.
 * Usually, for a specified FlexIO I2S instance, user need only call this API once to get the initialized handle.
 *
 * @param base FlexIO I2S peripheral base address.
 * @param handle FlexIO I2S DMA handle pointer.
 * @param callback FlexIO I2S DMA callback function called while finished a block.
 * @param userData User parameter for callback.
 * @param dmaHandle DMA handle for FlexIO I2S. This handle shall be a static value allocated by users.
 */
void FLEXIO_I2S_TransferRxCreateHandleDMA(FLEXIO_I2S_Type *base,
                                          flexio_i2s_dma_handle_t *handle,
                                          flexio_i2s_dma_callback_t callback,
                                          void *userData,
                                          dma_handle_t *dmaHandle);

/*!
 * @brief Configures the FlexIO I2S Tx audio format.
 *
 * Audio format can be changed in run-time of FlexIO I2S. This function configures the sample rate and audio data
 * format to be transferred. This function also sets DMA parameter according to format.
 *
 * @param base FlexIO I2S peripheral base address.
 * @param handle FlexIO I2S DMA handle pointer
 * @param format Pointer to FlexIO I2S audio data format structure.
 * @param srcClock_Hz FlexIO I2S clock source frequency in Hz. It should be 0 while in slave mode.
 * @retval kStatus_Success Audio format set successfully.
 * @retval kStatus_InvalidArgument The input arguments is invalid.
*/
void FLEXIO_I2S_TransferSetFormatDMA(FLEXIO_I2S_Type *base,
                                     flexio_i2s_dma_handle_t *handle,
                                     flexio_i2s_format_t *format,
                                     uint32_t srcClock_Hz);

/*!
 * @brief Performs a non-blocking FlexIO I2S transfer using DMA.
 *
 * @note This interface returns immediately after transfer initiates. Call
 * FLEXIO_I2S_GetTransferStatus to poll the transfer status and check whether FLEXIO I2S transfer finished.
 *
 * @param base FlexIO I2S peripheral base address.
 * @param handle FlexIO I2S DMA handle pointer.
 * @param xfer Pointer to DMA transfer structure.
 * @retval kStatus_Success Start a FlexIO I2S DMA send successfully.
 * @retval kStatus_InvalidArgument The input arguments is invalid.
 * @retval kStatus_TxBusy FlexIO I2S is busy sending data.
 */
status_t FLEXIO_I2S_TransferSendDMA(FLEXIO_I2S_Type *base,
                                    flexio_i2s_dma_handle_t *handle,
                                    flexio_i2s_transfer_t *xfer);

/*!
 * @brief Performs a non-blocking FlexIO I2S receive using DMA.
 *
 * @note This interface returns immediately after transfer initiates. Call
 * FLEXIO_I2S_GetReceiveRemainingBytes to poll the transfer status to check whether the FlexIO I2S transfer is finished.
 *
 * @param base FlexIO I2S peripheral base address.
 * @param handle FlexIO I2S DMA handle pointer.
 * @param xfer Pointer to DMA transfer structure.
 * @retval kStatus_Success Start a FlexIO I2S DMA receive successfully.
 * @retval kStatus_InvalidArgument The input arguments is invalid.
 * @retval kStatus_RxBusy FlexIO I2S is busy receiving data.
 */
status_t FLEXIO_I2S_TransferReceiveDMA(FLEXIO_I2S_Type *base,
                                       flexio_i2s_dma_handle_t *handle,
                                       flexio_i2s_transfer_t *xfer);

/*!
 * @brief Aborts a FlexIO I2S transfer using DMA.
 *
 * @param base FlexIO I2S peripheral base address.
 * @param handle FlexIO I2S DMA handle pointer.
 */
void FLEXIO_I2S_TransferAbortSendDMA(FLEXIO_I2S_Type *base, flexio_i2s_dma_handle_t *handle);

/*!
 * @brief Aborts a FlexIO I2S receive using DMA.
 *
 * @param base FlexIO I2S peripheral base address.
 * @param handle FlexIO I2S DMA handle pointer.
 */
void FLEXIO_I2S_TransferAbortReceiveDMA(FLEXIO_I2S_Type *base, flexio_i2s_dma_handle_t *handle);

/*!
 * @brief Gets the remaining bytes to be sent.
 *
 * @param base FlexIO I2S peripheral base address.
 * @param handle FlexIO I2S DMA handle pointer.
 * @param count Bytes sent.
 * @retval kStatus_Success Succeed get the transfer count.
 * @retval kStatus_NoTransferInProgress There is not a non-blocking transaction currently in progress.
 */
status_t FLEXIO_I2S_TransferGetSendCountDMA(FLEXIO_I2S_Type *base, flexio_i2s_dma_handle_t *handle, size_t *count);

/*!
 * @brief Gets the remaining bytes to be received.
 *
 * @param base FlexIO I2S peripheral base address.
 * @param handle FlexIO I2S DMA handle pointer.
 * @param count Bytes received.
 * @retval kStatus_Success Succeed get the transfer count.
 * @retval kStatus_NoTransferInProgress There is not a non-blocking transaction currently in progress.
 */
status_t FLEXIO_I2S_TransferGetReceiveCountDMA(FLEXIO_I2S_Type *base, flexio_i2s_dma_handle_t *handle, size_t *count);

/*! @} */

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif
