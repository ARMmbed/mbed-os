/** 
 *******************************************************************************
 * @file    txz_cg.h
 * @brief   This file provides all the functions prototypes for CG driver.
 * @version V1.0.0.0
 * $Date:: 2018-04-02 19:31:41 #$
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 * (C)Copyright TOSHIBA MICROELECTRONICS CORPORATION 2017 All rights reserved
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
typedef struct
{
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
uint32_t 	cg_get_fsysm(cg_t *p_obj);
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


