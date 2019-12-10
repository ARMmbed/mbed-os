/**
 *******************************************************************************
 * @file    txz_hal.c
 * @brief   This file provides API functions for driver common part.
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

