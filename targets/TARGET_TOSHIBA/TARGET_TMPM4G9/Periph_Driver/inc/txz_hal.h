/** 
 *******************************************************************************
 * @file    txz_hal.h
 * @brief   This file provides all the functions prototypes for driver common part.
 * @version V1.0.0.0
 * $Date:: 2017-08-09 11:01:04 #$
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
/*------------------------------------------------------------------------------*/
/*  Define to prevent recursive inclusion                                       */
/*------------------------------------------------------------------------------*/
#ifndef __HAL_H
#define __HAL_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txz_driver_def.h"

/**
 *  @addtogroup Periph_Driver
 *  @{
 */

/** 
 *  @defgroup HAL HAL
 *  @brief    HAL Driver.
 *  @{
 */
/*------------------------------------------------------------------------------*/
/*  Macro Function                                                              */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup HAL_Exported_macro HAL Exported Macro
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group HAL_Exported_macro */


/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup HAL_Exported_define HAL Exported Define
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group HAL_Exported_define */


/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup HAL_Exported_define HAL Exported Define
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group HAL_Exported_define */


/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup HAL_Exported_typedef HAL Exported Typedef
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group HAL_Exported_typedef */

/*------------------------------------------------------------------------------*/
/*  Functions                                                                   */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup HAL_Exported_functions HAL Exported Functions
 *  @{
 */

void hal_inc_tick(void);
uint32_t hal_get_tick(void);


/**
 *  @}
 */ /* End of group HAL_Exported_functions */

/**
 *  @}
 */ /* End of group HAL */

/**
 *  @}
 */ /* End of group Periph_Driver */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __HAL_H */


