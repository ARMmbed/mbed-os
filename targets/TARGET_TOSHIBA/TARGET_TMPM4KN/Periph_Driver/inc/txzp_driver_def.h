/**
 *******************************************************************************
 * @file    txzp_driver_def.h
 * @brief   All common macro and definition for TXZ peripheral drivers
 * @version V1.0.0
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2021
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
#include "TMPM4KNA.h"  /*!< TMPM4KNA Group Header file. */
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
