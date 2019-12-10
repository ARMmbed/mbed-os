/**
 *******************************************************************************
 * @file    txz_driver_def.h
 * @brief   All common macro and definition for TXZ peripheral drivers
 * @version V1.0.0.0
 * $Date:: 2017-07-21 15:39:36 #$
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
#ifndef __TXZ_DRIVER_DEF_H
#define __TXZ_DRIVER_DEF_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup Periph_Driver Peripheral Driver
  * @{
  */

/** @defgroup TXZ_DRIVER_DEF TXZ DRIVER DEF
  * @brief All common macro and definition for TXZ peripheral drivers
  * @{
  */

/** @defgroup Device_Header_Included Device Header Included
  * @brief Include the Device header file of a Target.
  * @{
  */
#include "TMPM4G9.h"  /*!< TMPM4Gx Group Header file. */
/**
 *  @} 
 */ /* End of group Device_Header */


/** @defgroup TXZ_Exported_typedef TXZ Exported typedef
  * @{
  */
typedef enum {
    TXZ_SUCCESS = 0U,
    TXZ_ERROR = 1U
} TXZ_Result;

typedef enum {
    TXZ_BUSY = 0U,
    TXZ_DONE = 1U
} TXZ_WorkState;

typedef enum {
    TXZ_DISABLE = 0U,
    TXZ_ENABLE = 1U
} TXZ_FunctionalState;
/**
 *  @} 
 */ /* End of group TXZ_Exported_typedef */

/** @defgroup TXZ_Exported_macro TXZ Exported macro
  * @{
  */
#define IS_TXZ_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

#define IS_POINTER_NOT_NULL(param) ((void*)(param)!=(void*)0)

/**
  * @brief To report the name of the source file and source line number where the
  *        assert_param error has occurred, "DEBUG" must be defined. And detailed
  *        definition of assert_failed() is needed to be implemented, which can be
  *        done, for example, in the main.c file.
  */
#ifdef DEBUG
void assert_failed(char *file, int32_t line);
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((char *)__FILE__, __LINE__))
#else
#define assert_param(expr)
#endif /* DEBUG */
/**
 *  @} 
 */ /* End of group TXZ_Exported_macro */

/**
 *  @} 
 */ /* End of group Periph_Driver */

/**
 *  @} 
 */ /* End of group TXZ_DRIVER_DEF */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TXZ_DRIVER_DEF_H */
