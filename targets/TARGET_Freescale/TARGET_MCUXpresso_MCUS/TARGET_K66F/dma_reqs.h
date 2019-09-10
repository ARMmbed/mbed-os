/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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

#ifndef _FSL_DMA_REQS_H_
#define _FSL_DMA_REQS_H_

#include "fsl_common.h"

/* Array for DSPI DMA TX requests */
#define SPI_DMA_TX_REQUEST_NUMBERS                                      \
    {                                                                   \
        kDmaRequestMux0SPI0Tx, kDmaRequestMux0SPI1Tx \
    }

/* Array for DSPI DMA RX requests */
#define SPI_DMA_RX_REQUEST_NUMBERS                                      \
    {                                                                   \
        kDmaRequestMux0SPI0Rx, kDmaRequestMux0SPI1Rx \
    }

/* Array for UART DMA TX requests */
#define UART_DMA_TX_REQUEST_NUMBERS                                            \
    {                                                                            \
        kDmaRequestMux0UART0Tx, kDmaRequestMux0UART1Tx, kDmaRequestMux0UART2Tx,  \
        kDmaRequestMux0UART3Tx, kDmaRequestMux0UART4       \
    }

/* Array for UART DMA RX requests */
#define UART_DMA_RX_REQUEST_NUMBERS                                            \
    {                                                                            \
        kDmaRequestMux0UART0Rx, kDmaRequestMux0UART1Rx, kDmaRequestMux0UART2Rx,  \
        kDmaRequestMux0UART3Rx, kDmaRequestMux0UART4       \
    }

#endif /* _FSL_DMA_REQS_H_ */
