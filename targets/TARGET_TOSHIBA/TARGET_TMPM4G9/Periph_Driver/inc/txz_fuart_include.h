/** 
 *******************************************************************************
 * @file    txz_fuart_include.h
 * @brief   This file provides internal common definition.
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
/*------------------------------------------------------------------------------*/
/*  Define to prevent recursive inclusion                                       */
/*------------------------------------------------------------------------------*/
#ifndef __FUART_INCLUDE_H
#define __FUART_INCLUDE_H

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
 *  @addtogroup FUART
 *  @{
 */
/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup FUART_Private_define
 *  @{
 */

/** 
 *  @defgroup FUART_NullPointer  Null Pointer
 *  @brief    Null Pointer.
 *  @{
 */
#define FUART_NULL        ((void *)0)       /*!< Null Pointer. */
/**
 *  @}
 */ /* End of group FUART_NullPointer */

/** 
 *  @defgroup FUART_ParameterResult  Parameter Check Result
 *  @brief    Whether the parameter is specified or not.
 *  @{
 */
#define FUART_PARAM_OK             ((int32_t)1)              /*!< Parameter is valid(specified).         */
#define FUART_PARAM_NG             ((int32_t)0)              /*!< Parameter is invalid(not specified).   */
/**
 *  @}
 */ /* End of group FUART_ParameterResult */

/** 
 *  @defgroup FUARTxDR  FUARTxDR Register
 *  @brief    FUARTxDR Register Definition.
 *  @{
 */
/* DR */
#define FUARTxDR_DR_8BIT_MASK      ((uint32_t)0x000000FF)    /*!< DR     :Mask for 8bit */
/**
 *  @}
 */ /* End of group FUARTxDR */

/** 
 *  @defgroup FUARTxCR  FUARTxCR Register
 *  @brief    FUARTxCR Register Definition.
 *  @{
 */
#define FUARTxCR_CTSEN_MASK                 ((uint32_t)0x00008000)    /*!< CTSEN :MASK.  */
#define FUARTxCR_RTSEN_MASK                 ((uint32_t)0x00004000)    /*!< RTSEN :MASK.   */
#define FUARTxCR_RXE_MASK                   ((uint32_t)0x00000200)    /*!< RXE :MASK.  */
#define FUARTxCR_TXE_MASK                   ((uint32_t)0x00000100)    /*!< TXE :MASK.  */
#define FUARTxCR_UARTEN_MSK                 ((uint32_t)0x00000001)    /*!< UARTEN :MASK.  */

#define FUARTxCR_CTSEN_DISABLE              ((uint32_t)0x00000000)    /*!< CTSEN :Not Available. */
#define FUARTxCR_RTSEN_DISABLE              ((uint32_t)0x00000000)    /*!< RTSEN :Not Available. */
#define FUARTxCR_RXE_DISABLE                ((uint32_t)0x00000000)    /*!< RXE :Disable. */
#define FUARTxCR_TXE_DISABLE                ((uint32_t)0x00000000)    /*!< TXE :Disable. */
#define FUARTxCR_UARTEN_DISABLE             ((uint32_t)0x00000000)    /*!< UARTEN :Disable. */

#define FUARTxCR_CTSEN_ENABLE               ((uint32_t)0x00008000)    /*!< CTSEN :Available.     */
#define FUARTxCR_RTSEN_ENABLE               ((uint32_t)0x00004000)    /*!< RTSEN :Available.     */
#define FUARTxCR_RXE_ENABLE                 ((uint32_t)0x00000200)    /*!< RXE :Enable.  */
#define FUARTxCR_TXE_ENABLE                 ((uint32_t)0x00000100)    /*!< TXE :Enable.  */
#define FUARTxCR_UARTEN_ENABLE              ((uint32_t)0x00000001)    /*!< UARTEN :Enable.  */
/**
 *  @}
 */ /* End of group FUARTxRSR */

/** 
 *  @defgroup FUARTxRSR  FUARTxRSR Register
 *  @brief    FUARTxRSR Register Definition.
 *  @{
 */
#define FUARTxRSR_OE_MASK        ((uint32_t)0x00000008)    /*!< OE     :Mask */
#define FUARTxRSR_BE_MASK        ((uint32_t)0x00000004)    /*!< BE     :Mask */
#define FUARTxRSR_PE_MASK        ((uint32_t)0x00000002)    /*!< PE     :Mask */
#define FUARTxRSR_FE_MASK        ((uint32_t)0x00000001)    /*!< FE     :Mask */

#define FUARTxRSR_OE_ERR         ((uint32_t)0x00000008)    /*!< OE     :Error */
#define FUARTxRSR_BE_ERR         ((uint32_t)0x00000004)    /*!< BE     :Error */
#define FUARTxRSR_PE_ERR         ((uint32_t)0x00000002)    /*!< PE     :Error */
#define FUARTxRSR_FE_ERR         ((uint32_t)0x00000001)    /*!< FE     :Error */
/**
 *  @}
 */ /* End of group FUARTxRSR */

/** 
 *  @defgroup FUARTxECR  FUARTxECR Register
 *  @brief    FUARTxECR Register Definition.
 *  @{
 */
/* ECR */
#define FUARTxECR_OE_MASK        ((uint32_t)0x00000008)    /*!< OE     :Mask */
#define FUARTxECR_BE_MASK        ((uint32_t)0x00000004)    /*!< BE     :Mask */
#define FUARTxECR_PE_MASK        ((uint32_t)0x00000002)    /*!< PE     :Mask */
#define FUARTxECR_FE_MASK        ((uint32_t)0x00000001)    /*!< FE     :Mask */

#define FUARTxECR_OE_CLR         ((uint32_t)0x00000008)    /*!< OE     :Clear */
#define FUARTxECR_BE_CLR         ((uint32_t)0x00000004)    /*!< BE     :Clear */
#define FUARTxECR_PE_CLR         ((uint32_t)0x00000002)    /*!< PE     :Clear */
#define FUARTxECR_FE_CLR         ((uint32_t)0x00000001)    /*!< FE     :Clear */

/**
 *  @}
 */ /* End of group FUARTxECR */

/** 
 *  @defgroup FUARTxLCR_H  FUARTxRSR Register
 *  @brief    FUARTxLCR_H  Register Definition.
 *  @{
 */
#define FUARTxLCR_H_BRK_MASK     ((uint32_t)0x00000001)    /*!< BRK      :Mask */

#define FUARTxLCR_H_BRK_SEND     ((uint32_t)0x00000001)    /*!< BRK      :Send */
#define FUARTxLCR_H_BRK_STOP     ((uint32_t)0x00000000)    /*!< BRK      :Stop */
/**
 *  @}
 */ /* End of group FUARTxLCR_H */

/** 
 *  @defgroup FUARTxRIS  FUARTxRIS Register
 *  @brief    FUARTxRIS  Register Definition.
 *  @{
 */
#define FUARTxRIS_OERIS_MASK     ((uint32_t)0x00000400)    /*!< OERIS    :Mask */
#define FUARTxRIS_BERIS_MASK     ((uint32_t)0x00000200)    /*!< BERIS    :Mask */
#define FUARTxRIS_PERIS_MASK     ((uint32_t)0x00000100)    /*!< PRRIS    :Mask */
#define FUARTxRIS_FERIS_MASK     ((uint32_t)0x00000080)    /*!< FERIS    :Mask */
#define FUARTxRIS_RTRIS_MASK     ((uint32_t)0x00000040)    /*!< RTRIS    :Mask */
#define FUARTxRIS_TXRIS_MASK     ((uint32_t)0x00000020)    /*!< TXRIS    :Mask */
#define FUARTxRIS_RXRIS_MASK     ((uint32_t)0x00000010)    /*!< RXRIS    :Mask */

#define FUARTxRIS_OERIS_REQ      ((uint32_t)0x00000400)    /*!< OERIS    :Request */
#define FUARTxRIS_BERIS_REQ      ((uint32_t)0x00000200)    /*!< BERIS    :Request */
#define FUARTxRIS_PERIS_REQ      ((uint32_t)0x00000100)    /*!< PRRIS    :Request */
#define FUARTxRIS_FERIS_REQ      ((uint32_t)0x00000080)    /*!< FERIS    :Request */
#define FUARTxRIS_RTRIS_REQ      ((uint32_t)0x00000040)    /*!< RTRIS    :Request */
#define FUARTxRIS_TXRIS_REQ      ((uint32_t)0x00000020)    /*!< TXRIS    :Request */
#define FUARTxRIS_RXRIS_REQ      ((uint32_t)0x00000010)    /*!< RXRIS    :Request */

/**
 *  @}
 */ /* End of group FUARTxRIS */

/** 
 *  @defgroup FUARTxMIS  FUARTxMIS Register
 *  @brief    FUARTxMIS  Register Definition.
 *  @{
 */
#define FUARTxMIS_OEMIS_MASK     ((uint32_t)0x00000400)    /*!< OEMIS    :Mask */
#define FUARTxMIS_BEMIS_MASK     ((uint32_t)0x00000200)    /*!< BEMIS    :Mask */
#define FUARTxMIS_PEMIS_MASK     ((uint32_t)0x00000100)    /*!< PRMIS    :Mask */
#define FUARTxMIS_FEMIS_MASK     ((uint32_t)0x00000080)    /*!< FEMIS    :Mask */
#define FUARTxMIS_RTMIS_MASK     ((uint32_t)0x00000040)    /*!< RTMIS    :Mask */
#define FUARTxMIS_TXMIS_MASK     ((uint32_t)0x00000020)    /*!< TXMIS    :Mask */
#define FUARTxMIS_RXMIS_MASK     ((uint32_t)0x00000010)    /*!< RXMIS    :Mask */

#define FUARTxMIS_OEMIS_REQ      ((uint32_t)0x00000400)    /*!< OEMIS    :Request */
#define FUARTxMIS_BEMIS_REQ      ((uint32_t)0x00000200)    /*!< BEMIS    :Request */
#define FUARTxMIS_PEMIS_REQ      ((uint32_t)0x00000100)    /*!< PRMIS    :Request */
#define FUARTxMIS_FEMIS_REQ      ((uint32_t)0x00000080)    /*!< FEMIS    :Request */
#define FUARTxMIS_RTMIS_REQ      ((uint32_t)0x00000040)    /*!< RTMIS    :Request */
#define FUARTxMIS_TXMIS_REQ      ((uint32_t)0x00000020)    /*!< TXMIS    :Request */
#define FUARTxMIS_RXMIS_REQ      ((uint32_t)0x00000010)    /*!< RXMIS    :Request */

/**
 *  @}
 */ /* End of group FUARTxMIS */

/** 
 *  @defgroup FUARTxICR  FUARTxICR Register
 *  @brief    FUARTxICR  Register Definition.
 *  @{
 */
#define FUARTxICR_OEIC_MASK     ((uint32_t)0x00000400)    /*!< OEIC    :Mask */
#define FUARTxICR_BEIC_MASK     ((uint32_t)0x00000200)    /*!< BEIC    :Mask */
#define FUARTxICR_PEIC_MASK     ((uint32_t)0x00000100)    /*!< PRIC    :Mask */
#define FUARTxICR_FEIC_MASK     ((uint32_t)0x00000080)    /*!< FEIC    :Mask */
#define FUARTxICR_RTIC_MASK     ((uint32_t)0x00000040)    /*!< RTIC    :Mask */
#define FUARTxICR_TXIC_MASK     ((uint32_t)0x00000020)    /*!< TXIC    :Mask */
#define FUARTxICR_RXIC_MASK     ((uint32_t)0x00000010)    /*!< RXIC    :Mask */

#define FUARTxICR_OEIC_CLR      ((uint32_t)0x00000400)    /*!< OEIC    :Request */
#define FUARTxICR_BEIC_CLR      ((uint32_t)0x00000200)    /*!< BEIC    :Request */
#define FUARTxICR_PEIC_CLR      ((uint32_t)0x00000100)    /*!< PRIC    :Request */
#define FUARTxICR_FEIC_CLR      ((uint32_t)0x00000080)    /*!< FEIC    :Request */
#define FUARTxICR_RTIC_CLR      ((uint32_t)0x00000040)    /*!< RTIC    :Request */
#define FUARTxICR_TXIC_CLR      ((uint32_t)0x00000020)    /*!< TXIC    :Request */
#define FUARTxICR_RXIC_CLR      ((uint32_t)0x00000010)    /*!< RXIC    :Request */

/**
 *  @}
 */ /* End of group FUARTxICR */

/** 
 *  @defgroup FUARTxDMACR  FUARTxDMACR Register
 *  @brief    FUARTxDMACR  Register Definition.
 *  @{
 */
#define FUARTxDMACR_RXDMAE_MASK    ((uint32_t)0x00000001)    /*!< RXDMAE    :Mask */
#define FUARTxDMACR_TXDMAE_MASK    ((uint32_t)0x00000002)    /*!< TXDMAE    :Mask */

#define FUARTxDMACR_RXDMAE_ENABLE  ((uint32_t)0x00000001)    /*!< RXDMAE    :Enable */
#define FUARTxDMACR_TXDMAE_ENABLE  ((uint32_t)0x00000002)    /*!< TXDMAE    :Enable */

#define FUARTxDMACR_RXDMAE_DISABLE ((uint32_t)0x00000000)    /*!< RXDMAE    :Disable */
#define FUARTxDMACR_TXDMAE_DISABLE ((uint32_t)0x00000000)    /*!< TXDMAE    :Disable */
/**
 *  @}
 */ /* End of group FUARTxDMACR */


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
/*  Inline Functions                                                             */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup FUART_Private_functions
 *  @{
 */
__STATIC_INLINE void disable_FUARTxCR_TXE(TSB_FURT_TypeDef *p_instance);
__STATIC_INLINE void enable_FUARTxCR_TXE(TSB_FURT_TypeDef *p_instance);
__STATIC_INLINE void disable_FUARTxCR_RXE(TSB_FURT_TypeDef *p_instance);
__STATIC_INLINE void enable_FUARTxCR_RXE(TSB_FURT_TypeDef *p_instance);
/*--------------------------------------------------*/
/** 
  * @brief  Disable FUARTxCR TXE.
  * @param  p_instance: Instance address.
  * @retval -
  * @note   Bitband Access
  */
/*--------------------------------------------------*/
__STATIC_INLINE void disable_FUARTxCR_TXE(TSB_FURT_TypeDef *p_instance)
{
#ifdef DEBUG
    if ((uint32_t)p_instance >= (uint32_t)PERI_BASE)
    {
        (*((__IO uint32_t *)BITBAND_PERI(&p_instance->CR,8))) = 0;
    }
#else
    (*((__IO uint32_t *)BITBAND_PERI(&p_instance->CR,8))) = 0;
#endif
}

/*--------------------------------------------------*/
/** 
  * @brief  Enable FUARTxCR TXE.
  * @param  p_instance: Instance address.
  * @retval -
  * @note   Bitband Access
  */
/*--------------------------------------------------*/
__STATIC_INLINE void enable_FUARTxCR_TXE(TSB_FURT_TypeDef *p_instance)
{
#ifdef DEBUG
    if ((uint32_t)p_instance >= (uint32_t)PERI_BASE)
    {
        (*((__IO uint32_t *)BITBAND_PERI(&p_instance->CR,8))) = 1;
    }
#else
    (*((__IO uint32_t *)BITBAND_PERI(&p_instance->CR,8))) = 1;
#endif
}

/*--------------------------------------------------*/
/** 
  * @brief  Disable FUARTxCR RXE.
  * @param  p_instance: Instance address.
  * @retval -
  * @note   Bitband Access
  */
/*--------------------------------------------------*/
__STATIC_INLINE void disable_FUARTxCR_RXE(TSB_FURT_TypeDef *p_instance)
{
#ifdef DEBUG
    if ((uint32_t)p_instance >= (uint32_t)PERI_BASE)
    {
        (*((__IO uint32_t *)BITBAND_PERI(&p_instance->CR,9))) = 0;
    }
#else
    (*((__IO uint32_t *)BITBAND_PERI(&p_instance->CR,9))) = 0;
#endif
}

/*--------------------------------------------------*/
/** 
  * @brief  Enable FUARTxCR RXE.
  * @param  p_instance: Instance address.
  * @retval -
  * @note   Bitband Access
  */
/*--------------------------------------------------*/
__STATIC_INLINE void enable_FUARTxCR_RXE(TSB_FURT_TypeDef *p_instance)
{
#ifdef DEBUG
    if ((uint32_t)p_instance >= (uint32_t)PERI_BASE)
    {
        (*((__IO uint32_t *)BITBAND_PERI(&p_instance->CR,9))) = 1;
    }
#else
    (*((__IO uint32_t *)BITBAND_PERI(&p_instance->CR,9))) = 1;
#endif
}


/**
 *  @}
 */ /* End of group FUART_Private_functions */

/**
 *  @}
 */ /* End of group FUART */

/**
 *  @}
 */ /* End of group Periph_Driver */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __UART_EX_H */


