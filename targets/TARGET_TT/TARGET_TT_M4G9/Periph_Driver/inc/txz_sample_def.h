/**
 *******************************************************************************
 * @file    txz_sample_def.h
 * @brief   This file includes header files for sample application.
 * @version V1.0.0.3
 * $Date:: 2017-10-26 13:33:43 #$
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 * (C)Copyright TOSHIBA MICROELECTRONICS CORPORATION 2017 All rights reserved
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
