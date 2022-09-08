/**
 *******************************************************************************
 * @file    txz_flash.h
 * @brief   This file provides all the functions prototypes for driver common part.
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
#ifndef __FLASH_H
#define __FLASH_H

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
 *  @defgroup FLASH FLASH
 *  @brief    FLASH Driver.
 *  @{
 */
/*------------------------------------------------------------------------------*/
/*  Macro Function                                                              */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup FLASH_Exported_macro FLASH Exported Macro
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group FLASH_Exported_macro */


/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup FLASH_Exported_define FLASH Exported Define
 *  @{
 */
/* no define */

/**
 *  @}
 */ /* End of group FLASH_Exported_define */


/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup FLASH_Exported_define FLASH Exported Define
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group FLASH_Exported_define */


/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup FLASH_Exported_typedef FLASH Exported Typedef
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group FLASH_Exported_typedef */

/*------------------------------------------------------------------------------*/
/*  Functions                                                                   */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup FLASH_Exported_functions FLASH Exported Functions
 *  @{
 */

void fc_read_clock_set(uint32_t sysclock);

/**
 *  @}
 */ /* End of group FLASH_Exported_functions */

/**
 *  @}
 */ /* End of group FLASH */

/**
 *  @}
 */ /* End of group Periph_Driver */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __FLASH_H */


