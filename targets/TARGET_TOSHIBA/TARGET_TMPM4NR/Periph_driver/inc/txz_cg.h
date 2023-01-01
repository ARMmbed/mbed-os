/**
 *******************************************************************************
 * @file    txz_cg.h
 * @brief   This file provides all the functions prototypes for CG driver.
 * @version V1.0.0
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * Copyright(C) TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2021
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
#include "txz_driver_def.h"

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
/* no define */
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
uint32_t    cg_get_fsysm(cg_t *p_obj);
uint32_t    cg_get_phyt0(cg_t *p_obj);
uint32_t    cg_get_mphyt0(cg_t *p_obj);
TXZ_Result  cg_ihosc_enable(cg_t *p_obj);
TXZ_Result  cg_ihosc_disable(cg_t *p_obj);
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


