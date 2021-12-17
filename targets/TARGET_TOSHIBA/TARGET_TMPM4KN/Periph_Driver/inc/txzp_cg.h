/**
 *******************************************************************************
 * @file    txzp_cg.h
 * @brief   This file provides all the functions prototypes for CG driver.
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
/*------------------------------------------------------------------------------*/
/*  Define to prevent recursive inclusion                                       */
/*------------------------------------------------------------------------------*/
#ifndef __CG_H
#define __CG_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txzp_driver_def.h"

/**
 *  @addtogroup Periph_Driver
 *  @{
 */

/**
 *  @defgroup CG CG
 *  @brief    CG Driver.
 *  @{
 */

/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup CG_Exported_define CG Exported Define
 *  @{
 */
/**
 *  @defgroup CG_CGOSCCR_OSCF  fosc high-speed oscillator Select Status.
 *  @brief    fosc high-speed oscillator Select Status.
 *  @{
 */
#define CG_CGOSCCR_OSCF_IHOSC           ((uint32_t)0x00000000)    /*!< IHOSC    */
#define CG_CGOSCCR_OSCF_EHOSC           ((uint32_t)0x00000200)    /*!< EHOSC    */
/**
 *  @}
 */ /* End of group CG_CGOSCCR_OSCF */
/**
 *  @}
 */ /* End of group CG_Exported_define */


/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup CG_Exported_define CG Exported Define
 *  @{
 */
/* no define */
/**
 *  @}
 */ /* End of group CG_Exported_define */

/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup CG_Exported_typedef CG Exported Typedef
 *  @{
 */
/**
 * @brief  CG member.
*/
/*----------------------------------*/
typedef struct {
    TSB_CG_TypeDef       *p_instance;     /*!< Registers base address.           */
} cg_t;

/**
 *  @}
 */ /* End of group CG_Exported_typedef */

/*------------------------------------------------------------------------------*/
/*  Functions                                                                   */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup CG_Exported_functions CG Exported Functions
 *  @{
 */
uint32_t    cg_get_mphyt0(cg_t *p_obj);
/**
 *  @}
 */ /* End of group CG_Exported_functions */

/**
 *  @}
 */ /* End of group CG */

/**
 *  @}
 */ /* End of group Periph_Driver */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __CG_H */


