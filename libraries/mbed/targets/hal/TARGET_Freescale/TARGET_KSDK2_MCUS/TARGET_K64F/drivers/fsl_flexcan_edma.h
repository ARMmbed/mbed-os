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
#ifndef _FSL_FLEXCAN_EDMA_H_
#define _FSL_FLEXCAN_EDMA_H_

#include "fsl_flexcan.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"

/*!
 * @addtogroup flexcan_edma_driver
 * @{
 */

/*! @file */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Forward declaration of the handle typedef. */
typedef struct _flexcan_edma_handle flexcan_edma_handle_t;

/*! @brief FlexCAN transfer callback function. */
typedef void (*flexcan_edma_transfer_callback_t)(CAN_Type *base,
                                                 flexcan_edma_handle_t *handle,
                                                 status_t status,
                                                 void *userData);

/*!
* @brief FlexCAN eDMA handle
*/
struct _flexcan_edma_handle
{
    flexcan_edma_transfer_callback_t callback; /*!< Callback function. */
    void *userData;                            /*!< FlexCAN callback function parameter.*/
    edma_handle_t *rxFifoEdmaHandle;           /*!< The EDMA Rx FIFO channel used. */
    volatile uint8_t rxFifoState;              /*!< Rx FIFO transfer state. */
};

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name eDMA transactional
 * @{
 */

/*!
 * @brief Initializes the FlexCAN handle, which is used in transactional functions.
 *
 * @param base FlexCAN peripheral base address.
 * @param handle Pointer to flexcan_edma_handle_t structure.
 * @param callback The callback function.
 * @param userData The parameter of the callback function.
 * @param rxFifoEdmaHandle User-requested DMA handle for Rx FIFO DMA transfer.
 */
void FLEXCAN_TransferCreateHandleEDMA(CAN_Type *base,
                                      flexcan_edma_handle_t *handle,
                                      flexcan_edma_transfer_callback_t callback,
                                      void *userData,
                                      edma_handle_t *rxFifoEdmaHandle);

/*!
 * @brief Receives the CAN Message from the Rx FIFO using eDMA.
 *
 * This function receives the CAN Message using eDMA. This is a non-blocking function, which returns
 * right away. After the CAN Message is received, the receive callback function is called.
 *
 * @param base FlexCAN peripheral base address.
 * @param handle Pointer to flexcan_edma_handle_t structure.
 * @param xfer FlexCAN Rx FIFO EDMA transfer structure, see #flexcan_fifo_transfer_t.
 * @retval kStatus_Success if succeed, others failed.
 * @retval kStatus_FLEXCAN_RxFifoBusy Previous transfer ongoing.
 */
status_t FLEXCAN_TransferReceiveFifoEDMA(CAN_Type *base, flexcan_edma_handle_t *handle, flexcan_fifo_transfer_t *xfer);

/*!
 * @brief Aborts the receive process which used eDMA.
 *
 * This function aborts the receive process which used eDMA.
 *
 * @param base FlexCAN peripheral base address.
 * @param handle Pointer to flexcan_edma_handle_t structure.
 */
void FLEXCAN_TransferAbortReceiveFifoEDMA(CAN_Type *base, flexcan_edma_handle_t *handle);

/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FSL_FLEXCAN_EDMA_H_ */
