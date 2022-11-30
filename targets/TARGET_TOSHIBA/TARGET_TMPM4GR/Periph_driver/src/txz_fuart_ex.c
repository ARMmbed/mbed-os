/**
 *******************************************************************************
 * @file    txz_fuart_ex.c
 * @brief   This file provides API functions for FUART driver.
 * @brief   Extended functionality.
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

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txz_fuart_include.h"
#include "txz_fuart_ex.h"

#if defined(__FUART_EX_H)

/**
 *  @addtogroup Periph_Driver
 *  @{
 */

/**
 *  @addtogroup UART
 *  @{
 */
/*------------------------------------------------------------------------------*/
/*  Configuration                                                               */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup FUART_Private_define
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group FUART_Private_define */


/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup FUART_Private_define
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group FUART_Private_define */


/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup FUART_Private_define
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group FUART_Private_define */


/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup FUART_Private_typedef
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group FUART_Private_typedef */


/*------------------------------------------------------------------------------*/
/*  Private Function                                                            */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup FUART_Private_fuctions
 *  @{
 */

/**
 *  @}
 */ /* End of group FUART_Private_functions */


/*------------------------------------------------------------------------------*/
/*  Public Function                                                             */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup FUART_Exported_functions
 *  @{
 */
/*--------------------------------------------------*/
/**
  * @brief     Send Break.
  * @param     p_obj   :UART object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @attention This function is not available in interrupt.
  * @attention Use after interrupt is disabled.
  */
/*--------------------------------------------------*/
TXZ_Result fuart_send_break(fuart_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef __DEBUG__
    /* Check the FUART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef __DEBUG__ */
    /*------------------------------*/
    /*  Set Break                   */
    /*------------------------------*/
    {
        uint32_t trans = p_obj->p_instance->LCR_H;

        trans &= ~FUARTxLCR_H_BRK_MASK;
        trans |=  FUARTxLCR_H_BRK_SEND;
        p_obj->p_instance->LCR_H = trans;
    }

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Stop Break.
  * @param     p_obj   :UART object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @attention This function is not available in interrupt.
  * @attention Use after interrupt is disabled.
  */
/*--------------------------------------------------*/
TXZ_Result fuart_stop_break(fuart_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef __DEBUG__
    /* Check the FUART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef __DEBUG__ */
    /*------------------------------*/
    /*  Set Break                   */
    /*------------------------------*/
    {
        uint32_t trans = p_obj->p_instance->LCR_H;

        trans &= ~FUARTxLCR_H_BRK_MASK;
        trans |=  FUARTxLCR_H_BRK_STOP;
        p_obj->p_instance->LCR_H = trans;
    }

    return (result);
}


/**
 *  @}
 */ /* End of group FUART_Exported_functions */

/**
 *  @}
 */ /* End of group FUART */

/**
 *  @}
 */ /* End of group Periph_Driver */

#endif /* defined(__UART_EX_H)  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

