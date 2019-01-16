/** 
 *******************************************************************************
 * @file    txz_hal.h
 * @brief   This file provides all the functions prototypes for driver common part.
 * @version V1.0.0.0
 * $Date:: 2017-08-09 11:01:04 #$
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 * (C)Copyright TOSHIBA MICROELECTRONICS CORPORATION 2017 All rights reserved
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


