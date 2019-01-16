/** 
 *******************************************************************************
 * @file    txz_uart_include.h
 * @brief   This file provides internal common definition.
 * @version V1.0.0.0
 * $Date:: 2017-07-21 15:39:36 #$
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 * (C)Copyright TOSHIBA MICROELECTRONICS CORPORATION 2017 All rights reserved
 *******************************************************************************
 */
/*------------------------------------------------------------------------------*/
/*  Define to prevent recursive inclusion                                       */
/*------------------------------------------------------------------------------*/
#ifndef __UART_INCLUDE_H
#define __UART_INCLUDE_H

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
 *  @addtogroup UART
 *  @{
 */
/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UART_Private_define
 *  @{
 */

/** 
 *  @defgroup UART_NullPointer  Null Pointer
 *  @brief    Null Pointer.
 *  @{
 */
#define UART_NULL        ((void *)0)
/**
 *  @}
 */ /* End of group UART_NullPointer */

/** 
 *  @defgroup UART_ParameterResult  Parameter Check Result
 *  @brief    Whether the parameter is specified or not.
 *  @{
 */
#define UART_PARAM_OK             ((int32_t)1)              /*!< Parameter is valid(specified).         */
#define UART_PARAM_NG             ((int32_t)0)              /*!< Parameter is invalid(not specified).   */
/**
 *  @}
 */ /* End of group UART_ParameterResult */

/** 
 *  @defgroup UARTxSWRST  UARTxSWRST Register
 *  @brief    UARTxSWRST Register Definition.
 *  @details  Detail.
 *  | Bit   | Bit Symbol |
 *  | :---  | :---       |
 *  | 31-8  | -          |
 *  | 7     | SWRSTF     |
 *  | 6:2   | -          |
 *  | 1:0   | SWRST      |
 *  @{
 */
/* SWRSTF */
#define UARTxSWRST_SWRSTF_MASK    ((uint32_t)0x00000080)    /*!< SWRSTF :Mask.                            */
#define UARTxSWRST_SWRSTF_IDLE    ((uint32_t)0x00000000)    /*!< SWRSTF :Not be "Software Reset".         */
#define UARTxSWRST_SWRSTF_RUN     ((uint32_t)0x00000080)    /*!< SWRSTF :During "Software Reset".         */
/* SWRST */
#define UARTxSWRST_SWRST_10       ((uint32_t)0x00000002)    /*!< SWRST  :"10"                             */
#define UARTxSWRST_SWRST_01       ((uint32_t)0x00000001)    /*!< SWRST  :"01"                             */
/**
 *  @}
 */ /* End of group UARTxSWRST */

/** 
 *  @defgroup UARTxCR0  UARTxCR0 Register
 *  @brief    UARTxCR0 Register Definition.
 *  @details  Detail.
 *  | Bit   | Bit Symbol |
 *  | :---  | :---       |
 *  | 31-19 | -          |
 *  | 18    | HBSST      |
 *  | 17    | HBSMD      |
 *  | 16    | HBSEN      |
 *  | 15    | LPB        |
 *  | 14-12 | NF[2:0]    |
 *  | 11    | -          |
 *  | 10    | CTSE       |
 *  | 9     | RTSE       |
 *  | 8     | WU         |
 *  | 7     | -          |
 *  | 6     | IV         |
 *  | 5     | DIR        |
 *  | 4     | SBLEN      |
 *  | 3     | EVEN       |
 *  | 2     | PE         |
 *  | 1-0   | SM[1:0]    |
 *  @{
 */
/* HBSST */
#define UARTxCR0_HBSST_MASK       ((uint32_t)0x00040000)    /*!< HBSST  :Mask.         */
/* HBSMD */
#define UARTxCR0_HBSMD_MASK       ((uint32_t)0x00020000)    /*!< HBSMD  :Mask.         */
/* HBSEN */
#define UARTxCR0_HBSEN_MASK       ((uint32_t)0x00010000)    /*!< HBSEN  :Mask.         */
#define UARTxCR0_HBSEN_DISABLE    ((uint32_t)0x00000000)    /*!< HBSEN  :Disable.      */
#define UARTxCR0_HBSEN_ENABLE     ((uint32_t)0x00010000)    /*!< HBSEN  :Enable.       */
/* LPB   */
#define UARTxCR0_LPB_MASK         ((uint32_t)0x00008000)    /*!< LPB    :Mask.         */
#define UARTxCR0_LPB_DISABLE      ((uint32_t)0x00000000)    /*!< LPB    :Disable.      */
#define UARTxCR0_LPB_ENABLE       ((uint32_t)0x00008000)    /*!< LPB    :Enable.       */
/* WU    */
#define UARTxCR0_WU_MASK          ((uint32_t)0x00000100)    /*!< WU     :Mask.         */
#define UARTxCR0_WU_DISABLE       ((uint32_t)0x00000000)    /*!< WU     :Disable.      */
#define UARTxCR0_WU_ENABLE        ((uint32_t)0x00000100)    /*!< WU     :Enable.       */
/**
 *  @}
 */ /* End of group UARTxCR0 */

/** 
 *  @defgroup UARTxCR1  UARTxCR1 Register
 *  @brief    UARTxCR1 Register Definition.
 *  @details  Detail.
 *  | Bit   | Bit Symbol |
 *  | :---  | :---       |
 *  | 31-15 | -          |
 *  | 14-12 | TIL[2:0]   |
 *  | 11    | -          |
 *  | 10-8  | RIL[2:0]   |
 *  | 7     | INTTXFE    |
 *  | 6     | INTTXWE    |
 *  | 5     | INTRXFE    |
 *  | 4     | INTRXWE    |
 *  | 3     | -          |
 *  | 2     | INTERR     |
 *  | 1     | DMATE      |
 *  | 0     | DMARE      |
 *  @{
 */
/* RIL   */
#define UARTxCR1_RIL_MASK         ((uint32_t)0x00000700)    /*!< RIL    :Mask.         */
/* DMATE */
#define UARTxCR1_DMATE_MASK       ((uint32_t)0x00000002)    /*!< DMATE  :Mask.         */
#define UARTxCR1_DMATE_DISABLE    ((uint32_t)0x00000000)    /*!< DMATE  :Disable.      */
#define UARTxCR1_DMATE_ENABLE     ((uint32_t)0x00000002)    /*!< DMATE  :Enable.       */
/* DMARE */
#define UARTxCR1_DMARE_MASK       ((uint32_t)0x00000001)    /*!< DMARE  :Mask.         */
#define UARTxCR1_DMARE_DISABLE    ((uint32_t)0x00000000)    /*!< DMARE  :Disable.      */
#define UARTxCR1_DMARE_ENABLE     ((uint32_t)0x00000001)    /*!< DMARE  :Enable.       */
/**
 *  @}
 */ /* End of group UARTxCR1 */

/** 
 *  @defgroup UARTxTRANS  UARTxTRANS Register
 *  @brief    UARTxTRANS Register Definition.
 *  @details  Detail.
 *  | Bit   | Bit Symbol |
 *  | :---  | :---       |
 *  | 31-4  | -          |
 *  | 3     | BK         |
 *  | 2     | TXTRG      |
 *  | 1     | TXE        |
 *  | 0     | RXE        |
 *  @{
 */
/* BK */
#define UARTxTRANS_BK_MASK        ((uint32_t)0x00000008)    /*!< BK     :Mask          */
#define UARTxTRANS_BK_STOP        ((uint32_t)0x00000000)    /*!< BK     :Stop          */
#define UARTxTRANS_BK_SEND        ((uint32_t)0x00000008)    /*!< BK     :Send          */
/* TXTRG */
#define UARTxTRANS_TXTRG_MASK     ((uint32_t)0x00000004)    /*!< TXTRG  :Mask          */
#define UARTxTRANS_TXTRG_DISABLE  ((uint32_t)0x00000000)    /*!< TXTRG  :Disable       */
#define UARTxTRANS_TXTRG_ENABLE   ((uint32_t)0x00000004)    /*!< TXTRG  :Enable        */
/* TXE */
#define UARTxTRANS_TXE_MASK       ((uint32_t)0x00000002)    /*!< TXE    :Mask          */
#define UARTxTRANS_TXE_DISABLE    ((uint32_t)0x00000000)    /*!< TXE    :Disable       */
#define UARTxTRANS_TXE_ENABLE     ((uint32_t)0x00000002)    /*!< TXE    :Enable        */
/* RXE */
#define UARTxTRANS_RXE_MASK       ((uint32_t)0x00000001)    /*!< RXE    :Mask          */
#define UARTxTRANS_RXE_DISABLE    ((uint32_t)0x00000000)    /*!< RXE    :Disable       */
#define UARTxTRANS_RXE_ENABLE     ((uint32_t)0x00000001)    /*!< RXE    :Enable        */
/* TXE,RXE */
#define UARTxTRANS_TXE_RXE_MASK   ((uint32_t)0x00000003)    /*!< TXE/RXE:Mask          */
/**
 *  @}
 */ /* End of group UARTxTRANS */

/** 
 *  @defgroup UARTxDR  UARTxDR Register
 *  @brief    UARTxDR Register Definition.
 *  @details  Detail.
 *  | Bit   | Bit Symbol |
 *  | :---  | :---       |
 *  | 31-19 | -          |
 *  | 18    | PERR       |
 *  | 17    | FERR       |
 *  | 16    | BERR       |
 *  | 15:9  | -          |
 *  | 8:0   | DR         |
 *  @{
 */
/* DR */
#define UARTxDR_DR_9BIT_MASK      ((uint32_t)0x000001FF)    /*!< DR     :Mask for 9bit */
#define UARTxDR_DR_8BIT_MASK      ((uint32_t)0x000000FF)    /*!< DR     :Mask for 8bit */
#define UARTxDR_DR_7BIT_MASK      ((uint32_t)0x0000007F)    /*!< DR     :Mask for 7bit */
/**
 *  @}
 */ /* End of group UARTxDR */

/** 
 *  @defgroup UARTxSR  UARTxSR Register
 *  @brief    UARTxSR Register Definition.
 *  @details  Detail.
 *  | Bit   | Bit Symbol |
 *  | :---  | :---       |
 *  | 31    | SUE        |
 *  | 30:16 | -          |
 *  | 15    | TXRUN      |
 *  | 14    | TXEND      |
 *  | 13    | TXFF       |
 *  | 12    | -          |
 *  | 11:8  | TLVL       |
 *  | 7     | RXRUN      |
 *  | 6     | RXEND      |
 *  | 5     | RXFF       |
 *  | 4     | -          |
 *  | 3:0   | RLVL       |
 *  @{
 */
/* SUE */
#define UARTxSR_SUE_MASK          ((uint32_t)0x80000000)    /*!< SUE   :Mask.               */
/* TXEND */
#define UARTxSR_TXEND_MASK        ((uint32_t)0x00004000)    /*!< TEXND :Mask.               */
#define UARTxSR_TXEND_R_END       ((uint32_t)0x00004000)    /*!< TXEND :[read]  Transfer done. */
#define UARTxSR_TXEND_W_CLEAR     ((uint32_t)0x00004000)    /*!< TXEND :[write] Clear Flag. */
/* TXFF */
#define UARTxSR_TXFF_MASK         ((uint32_t)0x00002000)    /*!< TXFF  :Mask.               */
#define UARTxSR_TXFF_R_REACHED    ((uint32_t)0x00002000)    /*!< TXFF  :[read]  Reached the transfer level. */
#define UARTxSR_TXFF_W_CLEAR      ((uint32_t)0x00002000)    /*!< TXFF  :[write] Clear Flag. */
/* TLVL */
#define UARTxSR_TLVL_MASK         ((uint32_t)0x00000F00)    /*!< TLVL  :Mask.               */
/* RXEND */
#define UARTxSR_RXEND_MASK        ((uint32_t)0x00000040)    /*!< RXEND :Mask.               */
#define UARTxSR_RXEND_R_END       ((uint32_t)0x00000040)    /*!< RXEND :[read]  Receive done. */
#define UARTxSR_RXEND_W_CLEAR     ((uint32_t)0x00000040)    /*!< RXEND :[write] Clear Flag. */
/* RXFF */
#define UARTxSR_RXFF_MASK         ((uint32_t)0x00000020)    /*!< RXFF  :Mask.               */
#define UARTxSR_RXFF_R_REACHED    ((uint32_t)0x00000020)    /*!< RXFF  :[read]  Receive done. */
#define UARTxSR_RXFF_W_CLEAR      ((uint32_t)0x00000020)    /*!< RXFF  :[write] Clear Flag. */
/* RLVL */
#define UARTxSR_RLVL_MASK         ((uint32_t)0x0000000F)    /*!< RLVL  :Mask.               */
/**
 *  @}
 */ /* End of group UARTxSR */

/** 
 *  @defgroup UARTxFIFOCLR  UARTxFIFOCLR Register
 *  @brief    UARTxFIFOCLR Register Definition.
 *  @details  Detail.
 *  | Bit   | Bit Symbol |
 *  | :---  | :---       |
 *  | 31-2  | -          |
 *  | 1     | TFCLR      |
 *  | 0     | RFCLR      |
 *  @{
 */
/* TFCLR */
#define UARTxFIFOCLR_TFCLR_CLEAR  ((uint32_t)0x00000002)    /*!< TFCLR  :Clear the transmit buff.         */
/* RFCLR */
#define UARTxFIFOCLR_RFCLR_CLEAR  ((uint32_t)0x00000001)    /*!< RFCLR  :Clear the receive buff.          */
/**
 *  @}
 */ /* End of group UARTxFIFOCLR */

/** 
 *  @defgroup UARTxERR  UARTxERR Register
 *  @brief    UARTxERR Register Definition.
 *  @details  Detail.
 *  | Bit   | Bit Symbol |
 *  | :---  | :---       |
 *  | 31-5  | -          |
 *  | 4     | TRGERR     |
 *  | 3     | OVRERR     |
 *  | 2     | PERR       |
 *  | 1     | FERR       |
 *  | 0     | BERR       |
 *  @{
 */
/* TRGERR */
#define UARTxERR_TRGERR_MASK      ((uint32_t)0x00000010)    /*!< TRGERR :Mask.               */
#define UARTxERR_TRGERR_R_NO_ERR  ((uint32_t)0x00000000)    /*!< TRGERR :[read]  No Error.   */
#define UARTxERR_TRGERR_R_ERR     ((uint32_t)0x00000010)    /*!< TRGERR :[read]  Error.      */
#define UARTxERR_TRGERR_W_CLEAR   ((uint32_t)0x00000010)    /*!< TRGERR :[write] Clear Flag. */
/* OVRERR */
#define UARTxERR_OVRERR_MASK      ((uint32_t)0x00000008)    /*!< OVRERR :Mask.               */
#define UARTxERR_OVRERR_R_NO_ERR  ((uint32_t)0x00000000)    /*!< OVRERR :[read]  No Error.   */
#define UARTxERR_OVRERR_R_ERR     ((uint32_t)0x00000008)    /*!< OVRERR :[read]  Error.      */
#define UARTxERR_OVRERR_W_CLEAR   ((uint32_t)0x00000008)    /*!< OVRERR :[write] Clear Flag. */
/* PERR */
#define UARTxERR_PERR_MASK        ((uint32_t)0x00000004)    /*!< PERR   :Mask.               */
#define UARTxERR_PERR_R_NO_ERR    ((uint32_t)0x00000000)    /*!< PERR   :[read]  No Error.   */
#define UARTxERR_PERR_R_ERR       ((uint32_t)0x00000004)    /*!< PERR   :[read]  Error.      */
#define UARTxERR_PERR_W_CLEAR     ((uint32_t)0x00000004)    /*!< PERR   :[write] Clear Flag. */
/* FERR */
#define UARTxERR_FERR_MASK        ((uint32_t)0x00000002)    /*!< FERR   :Mask.               */
#define UARTxERR_FERR_R_NO_ERR    ((uint32_t)0x00000000)    /*!< FERR   :[read]  No Error.   */
#define UARTxERR_FERR_R_ERR       ((uint32_t)0x00000002)    /*!< FERR   :[read]  Error.      */
#define UARTxERR_FERR_W_CLEAR     ((uint32_t)0x00000002)    /*!< FERR   :[write] Clear Flag. */
/* BERR */
#define UARTxERR_BERR_MASK        ((uint32_t)0x00000001)    /*!< BERR   :Mask.               */
#define UARTxERR_BERR_R_NO_ERR    ((uint32_t)0x00000000)    /*!< BERR   :[read]  No Error.   */
#define UARTxERR_BERR_R_ERR       ((uint32_t)0x00000001)    /*!< BERR   :[read]  Error.      */
#define UARTxERR_BERR_W_CLEAR     ((uint32_t)0x00000001)    /*!< BERR   :[write] Clear Flag. */
/**
 *  @}
 */ /* End of group UARTxERR */

/** 
 *  @}
 */ /* End of group UART_Private_define */


/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UART_Private_define
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group UART_Private_define */


/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UART_Private_typedef
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group UART_Private_typedef */

/*------------------------------------------------------------------------------*/
/*  Inline Functions                                                             */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup UART_Private_fuctions
 *  @{
 */
__STATIC_INLINE void disable_UARTxTRANS_TXE(TSB_UART_TypeDef *p_instance);
__STATIC_INLINE void enable_UARTxTRANS_TXE(TSB_UART_TypeDef *p_instance);
__STATIC_INLINE void disable_UARTxTRANS_RXE(TSB_UART_TypeDef *p_instance);
__STATIC_INLINE void enable_UARTxTRANS_RXE(TSB_UART_TypeDef *p_instance);
/*--------------------------------------------------*/
/** 
  * @brief  Disable UARTxTRANS TXE.
  * @param  p_instance: Instance address.
  * @retval -
  * @note   Bitband Access
  */
/*--------------------------------------------------*/
__STATIC_INLINE void disable_UARTxTRANS_TXE(TSB_UART_TypeDef *p_instance)
{
#ifdef DEBUG
    if ((uint32_t)p_instance >= (uint32_t)PERI_BASE)
    {
        (*((__IO uint32_t *)BITBAND_PERI(&p_instance->TRANS,1))) = 0;
    }
#else
    (*((__IO uint32_t *)BITBAND_PERI(&p_instance->TRANS,1))) = 0;
#endif
}

/*--------------------------------------------------*/
/** 
  * @brief  Enable UARTxTRANS TXE.
  * @param  p_instance: Instance address.
  * @retval -
  * @note   Bitband Access
  */
/*--------------------------------------------------*/
__STATIC_INLINE void enable_UARTxTRANS_TXE(TSB_UART_TypeDef *p_instance)
{
#ifdef DEBUG
    if ((uint32_t)p_instance >= (uint32_t)PERI_BASE)
    {
        (*((__IO uint32_t *)BITBAND_PERI(&p_instance->TRANS,1))) = 1;
    }
#else
    (*((__IO uint32_t *)BITBAND_PERI(&p_instance->TRANS,1))) = 1;
#endif
}

/*--------------------------------------------------*/
/** 
  * @brief  Disable UARTxTRANS RXE.
  * @param  p_instance: Instance address.
  * @retval -
  * @note   Bitband Access
  */
/*--------------------------------------------------*/
__STATIC_INLINE void disable_UARTxTRANS_RXE(TSB_UART_TypeDef *p_instance)
{
#ifdef DEBUG
    if ((uint32_t)p_instance >= (uint32_t)PERI_BASE)
    {
        (*((__IO uint32_t *)BITBAND_PERI(&p_instance->TRANS,0))) = 0;
    }
#else
    (*((__IO uint32_t *)BITBAND_PERI(&p_instance->TRANS,0))) = 0;
#endif
}

/*--------------------------------------------------*/
/** 
  * @brief  Enable UARTxTRANS RXE.
  * @param  p_instance: Instance address.
  * @retval -
  * @note   Bitband Access
  */
/*--------------------------------------------------*/
__STATIC_INLINE void enable_UARTxTRANS_RXE(TSB_UART_TypeDef *p_instance)
{
#ifdef DEBUG
    if ((uint32_t)p_instance >= (uint32_t)PERI_BASE)
    {
        (*((__IO uint32_t *)BITBAND_PERI(&p_instance->TRANS,0))) = 1;
    }
#else
    (*((__IO uint32_t *)BITBAND_PERI(&p_instance->TRANS,0))) = 1;
#endif
}


/**
 *  @}
 */ /* End of group UART_Private_functions */

/**
 *  @}
 */ /* End of group UART */

/**
 *  @}
 */ /* End of group Periph_Driver */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __UART_EX_H */


