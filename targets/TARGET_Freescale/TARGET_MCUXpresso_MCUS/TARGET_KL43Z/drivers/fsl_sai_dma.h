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
#ifndef _FSL_SAI_DMA_H_
#define _FSL_SAI_DMA_H_

#include "fsl_sai.h"
#include "fsl_dma.h"

/*!
 * @addtogroup sai_dma
 * @{
 */

/*! @file */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct _sai_dma_handle sai_dma_handle_t;

/*! @brief Define SAI DMA callback */
typedef void (*sai_dma_callback_t)(I2S_Type *base, sai_dma_handle_t *handle, status_t status, void *userData);

/*! @brief SAI DMA transfer handle, users should not touch the content of the handle.*/
struct _sai_dma_handle
{
    dma_handle_t *dmaHandle;                      /*!< DMA handler for SAI send */
    uint8_t bytesPerFrame;                        /*!< Bytes in a frame */
    uint8_t channel;                              /*!< Which Data channel SAI use */
    uint32_t state;                               /*!< SAI DMA transfer internal state */
    sai_dma_callback_t callback;                  /*!< Callback for users while transfer finish or error occured */
    void *userData;                               /*!< User callback parameter */
    sai_transfer_t saiQueue[SAI_XFER_QUEUE_SIZE]; /*!< Transfer queue storing queued transfer. */
    size_t transferSize[SAI_XFER_QUEUE_SIZE];     /*!< Data bytes need to transfer */
    volatile uint8_t queueUser;                   /*!< Index for user to queue transfer. */
    volatile uint8_t queueDriver;                 /*!< Index for driver to get the transfer data and size */
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
 * @brief Initializes the SAI master DMA handle.
 *
 * This function initializes the SAI master DMA handle, which can be used for other SAI master transactional APIs.
 * Usually, for a specified SAI instance, call this API once to get the initialized handle.
 *
 * @param base SAI base pointer.
 * @param handle SAI DMA handle pointer.
 * @param base SAI peripheral base address.
 * @param callback Pointer to user callback function.
 * @param userData User parameter passed to the callback function.
 * @param dmaHandle DMA handle pointer, this handle shall be static allocated by users.
 */
void SAI_TransferTxCreateHandleDMA(
    I2S_Type *base, sai_dma_handle_t *handle, sai_dma_callback_t callback, void *userData, dma_handle_t *dmaHandle);

/*!
 * @brief Initializes the SAI slave DMA handle.
 *
 * This function initializes the SAI slave DMA handle, which can be used for other SAI master transactional APIs.
 * Usually, for a specified SAI instance, call this API once to get the initialized handle.
 *
 * @param base SAI base pointer.
 * @param handle SAI DMA handle pointer.
 * @param base SAI peripheral base address.
 * @param callback Pointer to user callback function.
 * @param userData User parameter passed to the callback function.
 * @param dmaHandle DMA handle pointer, this handle shall be static allocated by users.
 */
void SAI_TransferRxCreateHandleDMA(
    I2S_Type *base, sai_dma_handle_t *handle, sai_dma_callback_t callback, void *userData, dma_handle_t *dmaHandle);

/*!
 * @brief Configures the SAI Tx audio format.
 *
 * The audio format can be changed at run-time. This function configures the sample rate and audio data
 * format to be transferred. This function also sets the eDMA parameter according to the format.
 *
 * @param base SAI base pointer.
 * @param handle SAI DMA handle pointer.
 * @param format Pointer to SAI audio data format structure.
 * @param mclkSourceClockHz SAI master clock source frequency in Hz.
 * @param bclkSourceClockHz SAI bit clock source frequency in Hz. If bit clock source is master.
 * clock, this value should equals to masterClockHz in format.
 * @retval kStatus_Success Audio format set successfully.
 * @retval kStatus_InvalidArgument The input arguments is invalid.
*/
void SAI_TransferTxSetFormatDMA(I2S_Type *base,
                                sai_dma_handle_t *handle,
                                sai_transfer_format_t *format,
                                uint32_t mclkSourceClockHz,
                                uint32_t bclkSourceClockHz);

/*!
 * @brief Configures the SAI Rx audio format.
 *
 * The audio format can be changed at run-time. This function configures the sample rate and audio data
 * format to be transferred. This function also sets EDMA parameter according to format.
 *
 * @param base SAI base pointer.
 * @param handle SAI DMA handle pointer.
 * @param format Pointer to SAI audio data format structure.
 * @param mclkSourceClockHz SAI master clock source frequency in Hz.
 * @param bclkSourceClockHz SAI bit clock source frequency in Hz. If bit clock source is master.
 * clock, this value should equals to masterClockHz in format.
 * @retval kStatus_Success Audio format set successfully.
 * @retval kStatus_InvalidArgument The input arguments is invalid.
*/
void SAI_TransferRxSetFormatDMA(I2S_Type *base,
                                sai_dma_handle_t *handle,
                                sai_transfer_format_t *format,
                                uint32_t mclkSourceClockHz,
                                uint32_t bclkSourceClockHz);

/*!
 * @brief Performs a non-blocking SAI transfer using DMA.
 *
 * @note This interface returns immediately after the transfer initiates. Call
 * the SAI_GetTransferStatus to poll the transfer status to check whether the SAI transfer finished.
 *
 * @param base SAI base pointer.
 * @param handle SAI DMA handle pointer.
 * @param xfer Pointer to DMA transfer structure.
 * @retval kStatus_Success Successfully start the data receive.
 * @retval kStatus_SAI_TxBusy Previous receive still not finished.
 * @retval kStatus_InvalidArgument The input parameter is invalid.
 */
status_t SAI_TransferSendDMA(I2S_Type *base, sai_dma_handle_t *handle, sai_transfer_t *xfer);

/*!
 * @brief Performs a non-blocking SAI transfer using DMA.
 *
 * @note This interface returns immediately after transfer initiates. Call
 * SAI_GetTransferStatus to poll the transfer status to check whether the SAI transfer is finished.
 *
 * @param base SAI base pointer
 * @param handle SAI DMA handle pointer.
 * @param xfer Pointer to DMA transfer structure.
 * @retval kStatus_Success Successfully start the data receive.
 * @retval kStatus_SAI_RxBusy Previous receive still not finished.
 * @retval kStatus_InvalidArgument The input parameter is invalid.
 */
status_t SAI_TransferReceiveDMA(I2S_Type *base, sai_dma_handle_t *handle, sai_transfer_t *xfer);

/*!
 * @brief Aborts a SAI transfer using DMA.
 *
 * @param base SAI base pointer.
 * @param handle SAI DMA handle pointer.
 */
void SAI_TransferAbortSendDMA(I2S_Type *base, sai_dma_handle_t *handle);

/*!
 * @brief Aborts a SAI transfer using DMA.
 *
 * @param base SAI base pointer.
 * @param handle SAI DMA handle pointer.
 */
void SAI_TransferAbortReceiveDMA(I2S_Type *base, sai_dma_handle_t *handle);

/*!
 * @brief Gets byte count sent by SAI.
 *
 * @param base SAI base pointer.
 * @param handle SAI DMA handle pointer.
 * @param count Bytes count sent by SAI.
 * @retval kStatus_Success Succeed get the transfer count.
 * @retval kStatus_NoTransferInProgress There is not a non-blocking transaction currently in progress.
 */
status_t SAI_TransferGetSendCountDMA(I2S_Type *base, sai_dma_handle_t *handle, size_t *count);

/*!
 * @brief Gets byte count received by SAI.
 *
 * @param base SAI base pointer.
 * @param handle SAI DMA handle pointer.
 * @param count Bytes count received by SAI.
 * @retval kStatus_Success Succeed get the transfer count.
 * @retval kStatus_NoTransferInProgress There is not a non-blocking transaction currently in progress.
 */
status_t SAI_TransferGetReceiveCountDMA(I2S_Type *base, sai_dma_handle_t *handle, size_t *count);

/*! @} */

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif
