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

#ifndef _FSL_PERIPHERAL_CLOCK_H_
#define _FSL_PERIPHERAL_CLOCK_H_

#include "fsl_clock.h"

/* Array for LPUART module clocks */
#define LPUART_CLOCK_FREQS                         \
    {                                              \
        kCLOCK_Osc0ErClk, kCLOCK_Osc0ErClk, kCLOCK_Osc0ErClk, kCLOCK_Osc0ErClk, kCLOCK_Osc0ErClk   \
    }

/* Array for I2C module clocks */
#define I2C_CLOCK_FREQS                                 \
    {                                              \
        I2C0_CLK_SRC, I2C1_CLK_SRC, I2C2_CLK_SRC   \
    }

/* Array for DSPI module clocks */
#define SPI_CLOCK_FREQS                                    \
    {                                                 \
        DSPI0_CLK_SRC, DSPI1_CLK_SRC, DSPI2_CLK_SRC   \
    }

#endif /* _FSL_PERIPHERAL_CLOCK_H_ */
