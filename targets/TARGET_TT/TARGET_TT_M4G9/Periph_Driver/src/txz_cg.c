/**
 *******************************************************************************
 * @file    txz_cg.c
 * @brief   This file provides API functions for CG driver.
 * @version V1.0.0.1
 * $Date:: 2018-04-04 18:04:44 #$
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
#include "txz_cg.h"

#if defined(__CG_H)
/**
 *  @addtogroup Periph_Driver
 *  @{
 */

/** 
 *  @addtogroup CG
 *  @{
 */
/*------------------------------------------------------------------------------*/
/*  Configuration                                                               */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup CG_Private_define CG Private Define
 *  @{
 */
/* no define */
/** 
 *  @}
 */ /* End of group CG_Private_define */


/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup CG_Private_define CG Private Define
 *  @{
 */
#define CG_FSYS_MASK      ((uint32_t)0x00070000)    /*!< CG FSYS mask */

#define CG_FSYS_1         ((uint32_t)0x00000000)    /*!< CG fc register value */
#define CG_FSYS_2         ((uint32_t)0x00010000)    /*!< CG fc/2 register value */
#define CG_FSYS_4         ((uint32_t)0x00020000)    /*!< CG fc/4 register value */
#define CG_FSYS_8         ((uint32_t)0x00030000)    /*!< CG fc/8 register value */
#define CG_FSYS_16        ((uint32_t)0x00040000)    /*!< CG fc/16 register value */

#define CG_FSYS_1_MUL     ((uint32_t)0x00000001)    /*!< CG fc multiplication value */
#define CG_FSYS_2_MUL     ((uint32_t)0x00000002)    /*!< CG fc/2 multiplication value */
#define CG_FSYS_4_MUL     ((uint32_t)0x00000004)    /*!< CG fc/4 multiplication value */
#define CG_FSYS_8_MUL     ((uint32_t)0x00000008)    /*!< CG fc/8 multiplication value */
#define CG_FSYS_16_MUL    ((uint32_t)0x00000010)    /*!< CG fc/16 multiplication value */

#define CG_PRCK_MASK     ((uint32_t)0x00000F00)    /*!< CG PRCK mask */

#define CG_PRCK_1        ((uint32_t)0x00000000)    /*!< CG ƒÓT0 fc register value */
#define CG_PRCK_2        ((uint32_t)0x00000100)    /*!< CG ƒÓT0 fc/2 register value */
#define CG_PRCK_4        ((uint32_t)0x00000200)    /*!< CG ƒÓT0 fc/4 register value */
#define CG_PRCK_8        ((uint32_t)0x00000300)    /*!< CG ƒÓT0 fc/8 register value */
#define CG_PRCK_16       ((uint32_t)0x00000400)    /*!< CG ƒÓT0 fc/16 register value */
#define CG_PRCK_32       ((uint32_t)0x00000500)    /*!< CG ƒÓT0 fc/32 register value */
#define CG_PRCK_64       ((uint32_t)0x00000600)    /*!< CG ƒÓT0 fc/64 register value */
#define CG_PRCK_128      ((uint32_t)0x00000700)    /*!< CG ƒÓT0 fc/128 register value */
#define CG_PRCK_256      ((uint32_t)0x00000800)    /*!< CG ƒÓT0 fc/256 register value */
#define CG_PRCK_512      ((uint32_t)0x00000900)    /*!< CG ƒÓT0 fc/512 register value */

#define CG_PRCKST_MASK     ((uint32_t)0x0F000000)    /*!< CG PRCKST mask */

#define CG_PRCKST_1        ((uint32_t)0x00000000)    /*!< CG ƒÓT0 fc register status */
#define CG_PRCKST_2        ((uint32_t)0x01000000)    /*!< CG ƒÓT0 fc/2 register status */
#define CG_PRCKST_4        ((uint32_t)0x02000000)    /*!< CG ƒÓT0 fc/4 register status */
#define CG_PRCKST_8        ((uint32_t)0x03000000)    /*!< CG ƒÓT0 fc/8 register status */
#define CG_PRCKST_16       ((uint32_t)0x04000000)    /*!< CG ƒÓT0 fc/16 register status */
#define CG_PRCKST_32       ((uint32_t)0x05000000)    /*!< CG ƒÓT0 fc/32 register status */
#define CG_PRCKST_64       ((uint32_t)0x06000000)    /*!< CG ƒÓT0 fc/64 register status */
#define CG_PRCKST_128      ((uint32_t)0x07000000)    /*!< CG ƒÓT0 fc/128 register status */
#define CG_PRCKST_256      ((uint32_t)0x08000000)    /*!< CG ƒÓT0 fc/256 register status */
#define CG_PRCKST_512      ((uint32_t)0x09000000)    /*!< CG ƒÓT0 fc/512 register status */

#define CG_PRCK_1_DIV     ((uint32_t)0x00000001)    /*!< CG ƒÓT0 fc division value */
#define CG_PRCK_2_DIV     ((uint32_t)0x00000002)    /*!< CG ƒÓT0 fc/2 division value */
#define CG_PRCK_4_DIV     ((uint32_t)0x00000004)    /*!< CG ƒÓT0 fc/4 division value */
#define CG_PRCK_8_DIV     ((uint32_t)0x00000008)    /*!< CG ƒÓT0 fc/8 division value */
#define CG_PRCK_16_DIV    ((uint32_t)0x00000010)    /*!< CG ƒÓT0 fc/16 division value */
#define CG_PRCK_32_DIV    ((uint32_t)0x00000020)    /*!< CG ƒÓT0 fc/32 division value */
#define CG_PRCK_64_DIV    ((uint32_t)0x00000040)    /*!< CG ƒÓT0 fc/64 division value */
#define CG_PRCK_128_DIV   ((uint32_t)0x00000080)    /*!< CG ƒÓT0 fc/128 division value */
#define CG_PRCK_256_DIV   ((uint32_t)0x00000100)    /*!< CG ƒÓT0 fc/256 division value */
#define CG_PRCK_512_DIV   ((uint32_t)0x00000200)    /*!< CG ƒÓT0 fc/512 division value */


#define CG_MCKSELPST_MASK ((uint32_t)0xC0000000)    /*!< CG MCKSEL mask */

#define CG_MCKSELPST_1    ((uint32_t)0x00000000)    /*!<  CG ƒÓT0 fc/PRCK value */
#define CG_MCKSELPST_2    ((uint32_t)0x40000000)    /*!<  CG ƒÓT0 fc/PRCK/2 value */
#define CG_MCKSELPST_4    ((uint32_t)0x80000000)    /*!<  CG ƒÓT0 fc/PRCK/4 value */

#define CG_FSYSM_1_DIV    ((uint32_t)0x00000001)    /*!< CG fsysm ƒÓT0   division value */
#define CG_FSYSM_2_DIV    ((uint32_t)0x00000002)    /*!< CG fsysm ƒÓT0/2 division value */
#define CG_FSYSM_4_DIV    ((uint32_t)0x00000004)    /*!< CG fsysm ƒÓT0/4 division value */

#define CG_IHOSC_DISABLE  ((uint32_t)0x00000000)    /*!< Internal high-speed oscillator disable */
#define CG_IHOSC_ENABLE   ((uint32_t)0x00000001)    /*!< Internal high-speed oscillator enable */

#define CG_IHOSC1EN       ((uint32_t)0x00000000)    /*!< CG OSCCR bit0 */

#define CG_MCKSELGST_MASK ((uint32_t)0x00C00000)	/*!< CG MCKSELGST mask  */
#define CG_MCKSELGST_1    ((uint32_t)0x00000000)    /*!<  CG fsysm fc/PRCK value */
#define CG_MCKSELGST_2    ((uint32_t)0x00400000)    /*!<  CG fsysm fc/PRCK/2 value */
#define CG_MCKSELGST_4    ((uint32_t)0x00800000)    /*!<  CG fsysm fc/PRCK/4 value */


/** 
 *  @}
 */ /* End of group CG_Private_define */


/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup CG_Private_define CG Private Define
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group CG_Private_define */


/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup CG_Private_typedef CG Private Typedef
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group CG_Private_typedef */


/*------------------------------------------------------------------------------*/
/*  Private Function                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup CG_Private_fuctions CG Private Fuctions
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group CG_Private_functions */


/*------------------------------------------------------------------------------*/
/*  Public Function                                                             */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup CG_Exported_functions CG Exported Functions
 *  @{
 */

/*--------------------------------------------------*/
/**
  * @brief     Update Middle PrescalerClock according register values.
  * @param     p_obj       :CG object.
  * @retval    Middle PrescalerClock Frequency.
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
uint32_t cg_get_fsysm(cg_t *p_obj)
{
	uint32_t result = 0U;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the CG_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */

    /* System core clock update */
    SystemCoreClockUpdate();

    switch (p_obj->p_instance->SYSCR & CG_MCKSELGST_MASK)
    {
    case CG_MCKSELGST_1:   /* fsysm -> fc/PRCK */
        result = SystemCoreClock / CG_FSYSM_1_DIV;
        break;
    case CG_MCKSELGST_2:   /* fsysm -> fc/PRCK/2 */
        result = SystemCoreClock / CG_FSYSM_2_DIV;
        break;
    case CG_MCKSELGST_4:   /* fsysm -> fc/PRCK/4 */
        result = SystemCoreClock / CG_FSYSM_4_DIV;
        break;
    default:
        result = 0U;
        break;
    }   
    return (result);

}

/*--------------------------------------------------*/
/**
  * @brief     Update PrescalerClock according register values.
  * @param     p_obj       :CG object.
  * @retval    PrescalerClock Frequency.
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
uint32_t cg_get_phyt0(cg_t *p_obj)
{
    uint32_t result = 0U;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the CG_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */

    /* System core clock update */
    SystemCoreClockUpdate();

    /* Get Gear status. */
    switch (p_obj->p_instance->SYSCR & CG_FSYS_MASK)
    {
    case CG_FSYS_1:             /* Gear -> fc */
        result = SystemCoreClock * CG_FSYS_1_MUL;
        break;
    case CG_FSYS_2:             /* Gear -> fc/2 */
        result = SystemCoreClock * CG_FSYS_2_MUL;
        break;
    case CG_FSYS_4:             /* Gear -> fc/4 */
        result = SystemCoreClock * CG_FSYS_4_MUL;
        break;
    case CG_FSYS_8:             /* Gear -> fc/8 */
        result = SystemCoreClock * CG_FSYS_8_MUL;
        break;
    case CG_FSYS_16:            /* Gear -> fc/16 */
        result = SystemCoreClock * CG_FSYS_16_MUL;
        break;
    default:
        result = 0U;
        break;
    }

    switch (p_obj->p_instance->SYSCR & CG_PRCKST_MASK)
    {
    case CG_PRCKST_1:             /* ƒÓT0 -> fc */
        result /= CG_PRCK_1_DIV;
        break;
    case CG_PRCKST_2:             /* ƒÓT0 -> fc/2 */
        result /= CG_PRCK_2_DIV;
        break;
    case CG_PRCKST_4:             /* ƒÓT0 -> fc/4 */
        result /= CG_PRCK_4_DIV;
        break;
    case CG_PRCKST_8:             /* ƒÓT0 -> fc/8 */
        result /= CG_PRCK_8_DIV;
        break;
    case CG_PRCKST_16:            /* ƒÓT0 -> fc/16 */
        result /= CG_PRCK_16_DIV;
        break;
    case CG_PRCKST_32:            /* ƒÓT0 -> fc/32 */
        result /= CG_PRCK_32_DIV;
        break;
    case CG_PRCKST_64:            /* ƒÓT0 -> fc/64 */
        result /= CG_PRCK_64_DIV;
        break;
    case CG_PRCKST_128:           /* ƒÓT0 -> fc/128 */
        result /= CG_PRCK_128_DIV;
        break;
    case CG_PRCKST_256:           /* ƒÓT0 -> fc/256 */
        result /= CG_PRCK_256_DIV;
        break;
    case CG_PRCKST_512:           /* ƒÓT0 -> fc/512 */
        result /= CG_PRCK_512_DIV;
        break;
    default:
        result = 0U;
        break;
    }

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Update Middle PrescalerClock according register values.
  * @param     p_obj       :CG object.
  * @retval    Middle PrescalerClock Frequency.
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
uint32_t cg_get_mphyt0(cg_t *p_obj)
{
    uint32_t result = 0U;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the CG_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */

    /* System core clock update */
    SystemCoreClockUpdate();

    /* Get Gear status. */
    switch (p_obj->p_instance->SYSCR & CG_FSYS_MASK)
    {
    case CG_FSYS_1:             /* Gear -> fc */
        result = SystemCoreClock * CG_FSYS_1_MUL;
        break;
    case CG_FSYS_2:             /* Gear -> fc/2 */
        result = SystemCoreClock * CG_FSYS_2_MUL;
        break;
    case CG_FSYS_4:             /* Gear -> fc/4 */
        result = SystemCoreClock * CG_FSYS_4_MUL;
        break;
    case CG_FSYS_8:             /* Gear -> fc/8 */
        result = SystemCoreClock * CG_FSYS_8_MUL;
        break;
    case CG_FSYS_16:            /* Gear -> fc/16 */
        result = SystemCoreClock * CG_FSYS_16_MUL;
        break;
    default:
        result = 0U;
        break;
    }
    switch (p_obj->p_instance->SYSCR & CG_PRCKST_MASK)
    {
    case CG_PRCKST_1:             /* ƒÓT0 -> fc */
        result /= CG_PRCK_1_DIV;
        break;
    case CG_PRCKST_2:             /* ƒÓT0 -> fc/2 */
        result /= CG_PRCK_2_DIV;
        break;
    case CG_PRCKST_4:             /* ƒÓT0 -> fc/4 */
        result /= CG_PRCK_4_DIV;
        break;
    case CG_PRCKST_8:             /* ƒÓT0 -> fc/8 */
        result /= CG_PRCK_8_DIV;
        break;
    case CG_PRCKST_16:            /* ƒÓT0 -> fc/16 */
        result /= CG_PRCK_16_DIV;
        break;
    case CG_PRCKST_32:            /* ƒÓT0 -> fc/32 */
        result /= CG_PRCK_32_DIV;
        break;
    case CG_PRCKST_64:            /* ƒÓT0 -> fc/64 */
        result /= CG_PRCK_64_DIV;
        break;
    case CG_PRCKST_128:           /* ƒÓT0 -> fc/128 */
        result /= CG_PRCK_128_DIV;
        break;
    case CG_PRCKST_256:           /* ƒÓT0 -> fc/256 */
        result /= CG_PRCK_256_DIV;
        break;
    case CG_PRCKST_512:           /* ƒÓT0 -> fc/512 */
        result /= CG_PRCK_512_DIV;
        break;
    default:
        result = 0U;
        break;
    }
    
    switch (p_obj->p_instance->SYSCR & CG_MCKSELPST_MASK)
    {
    case CG_MCKSELPST_1:   /* ƒÓT0 -> fc/PRCK */
        result /= CG_FSYSM_1_DIV;
        break;
    case CG_MCKSELPST_2:   /* ƒÓT0 -> fc/PRCK/2 */
        result /= CG_FSYSM_2_DIV;
        break;
    case CG_MCKSELPST_4:   /* ƒÓT0 -> fc/PRCK/4 */
        result /= CG_FSYSM_4_DIV;
        break;
    default:
        result = 0U;
        break;
    }   
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Set Internal high-speed oscillator enable.
  * @param     p_obj       :CG object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result cg_ihosc_enable(cg_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the CG_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */

    /* Internal high-speed oscillator is enable. */
    (*((__IO uint32_t *)BITBAND_PERI(&p_obj->p_instance->OSCCR,CG_IHOSC1EN))) = CG_IHOSC_ENABLE;

    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Set Internal high-speed oscillator disable.
  * @param     p_obj       :CG object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR   :Failure.
  * @note      -
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result cg_ihosc_disable(cg_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the CG_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
#endif /* #ifdef DEBUG */

    /* Internal high-speed oscillator is disable. */
    (*((__IO uint32_t *)BITBAND_PERI(&p_obj->p_instance->OSCCR,CG_IHOSC1EN))) = CG_IHOSC_DISABLE;

    return (result);
}
/**
 *  @}
 */ /* End of group CG_Exported_functions */

/**
 *  @} 
 */ /* End of group CG */

/**
 *  @} 
 */ /* End of group Periph_Driver */

#endif /* defined(__CG_H)  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

