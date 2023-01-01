/**
 *******************************************************************************
 * @file    txz_fuart_ex.h
 * @brief   This file provides all the functions prototypes for FUART driver.
 * @brief   Extended functionality.
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
#ifndef __FUART_EX_H
#define __FUART_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txz_driver_def.h"
#include "txz_fuart.h"

/**
 *  @addtogroup Periph_Driver
 *  @{
 */

/**
 *  @addtogroup FUART
 *  @{
 */
/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup FUART_Exported_define
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group FUART_Exported_define */


/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup FUART_Exported_define
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group FUART_Exported_define */


/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup FUART_Exported_typedef
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group FUART_Exported_typedef */


/*------------------------------------------------------------------------------*/
/*  Functions                                                                   */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup FUART_Exported_functions
 *  @{
 */
TXZ_Result fuart_send_break(fuart_t *p_obj);
TXZ_Result fuart_stop_break(fuart_t *p_obj);
/**
 *  @}
 */ /* End of group FUART_Exported_functions */

/**
 *  @}
 */ /* End of group FUART */

/**
 *  @}
 */ /* End of group Periph_Driver */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __FUART_EX_H */


