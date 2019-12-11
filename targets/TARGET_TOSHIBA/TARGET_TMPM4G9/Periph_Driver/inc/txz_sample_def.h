/**
 *******************************************************************************
 * @file    txz_sample_def.h
 * @brief   This file includes header files for sample application.
 * @version V1.0.0.3
 * $Date:: 2017-10-26 13:33:43 #$
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright(C) 2019, Toshiba Electronic Device Solutions Corporation
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of Nuvoton Technology Corp. nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TXZ_SAMPLE_DEF_H
#define __TXZ_SAMPLE_DEF_H

#include "txz_driver_def.h"
#include "txz_hal.h"
#if defined(TMPM4G9)
    #include "txz_cg.h"
    #include "txz_gpio.h"
#else
    #include "txz_cg.h"
    #include "txz_gpio.h"
    #include "txz_dma.h"
#endif
#include "txz_uart.h"
#include "txz_t32a.h"
#endif  /* __TXZ_SAMPLE_DEF_H */
