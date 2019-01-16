/**
 *******************************************************************************
 * @file    txz_hal.c
 * @brief   This file provides API functions for driver common part.
 * @version V1.0.0.0
 * $Date:: 2017-08-09 11:01:04 #$
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 * (C)Copyright TOSHIBA MICROELECTRONICS CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

#ifdef __cplusplus
 extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txz_hal.h"

#if defined(__HAL_H)
/**
 *  @addtogroup Periph_Driver
 *  @{
 */

/** 
 *  @addtogroup HAL
 *  @{
 */

/*------------------------------------------------------------------------------*/
/*  Macro Function                                                              */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup HAL_Private_macro HAL Private Macro
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group HAL_Private_macro */


/*------------------------------------------------------------------------------*/
/*  Configuration                                                               */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup HAL_Private_define HAL Private Define
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group HAL_Private_define */


/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup HAL_Private_define HAL Private Define
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group HAL_Private_define */


/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup HAL_Private_define HAL Private Define
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group HAL_Private_define */


/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup HAL_Private_typedef HAL Private Typedef
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group HAL_Private_typedef */


/*------------------------------------------------------------------------------*/
/*  Private Function                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup HAL_Private_fuctions HAL Private Fuctions
 *  @{
 */

static uint32_t tick;

/**
 *  @}
 */ /* End of group HAL_Private_functions */

/*------------------------------------------------------------------------------*/
/*  Public Function                                                             */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup HAL_Exported_functions
 *  @{
 */
/*--------------------------------------------------*/
/**
  * @brief     Increment a tick value.
  * @param     -
  * @retval    -
  * @note      Please call by user.
  * @note      In the sample, this variable is incremented each 1ms timer interrupt.
  */
/*--------------------------------------------------*/
void hal_inc_tick(void)
{
    tick++;
}

/*--------------------------------------------------*/
/** 
  * @brief  Provides a tick value.
  * @param  -
  * @return Tick value.
  * @retval -
  * @note   -
  */
/*--------------------------------------------------*/
uint32_t hal_get_tick(void)
{
    return(tick);
}

/**
 *  @}
 */ /* End of group HAL_Exported_functions */

/**
 *  @} 
 */ /* End of group HAL */

/**
 *  @} 
 */ /* End of group Periph_Driver */

#endif /* defined(__HAL_H)  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

