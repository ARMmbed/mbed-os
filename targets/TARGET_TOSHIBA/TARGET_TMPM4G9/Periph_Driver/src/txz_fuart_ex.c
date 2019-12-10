/**
 *******************************************************************************
 * @file    txz_fuart_ex.c
 * @brief   This file provides API functions for FUART driver.
 * @brief   Extended functionality.
 * @version V1.0.0.0
 * $Date:: 2017-08-06 10:43:01 #$
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
#ifdef DEBUG
    /* Check the FUART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef DEBUG */
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
#ifdef DEBUG
    /* Check the FUART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif /* #ifdef DEBUG */
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

