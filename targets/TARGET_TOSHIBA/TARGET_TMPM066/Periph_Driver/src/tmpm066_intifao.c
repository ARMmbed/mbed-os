/**
 *******************************************************************************
 * @file    tmpm066_intifao.c
 * @brief   This file provides API functions for INTIFAO driver 
 * @version V2.0.2.2
 * @date    2016/02/15
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "tmpm066_intifao.h"

#if defined(__TMPM066_INTIFAO_H)
/** @addtogroup TX00_Periph_Driver
  * @{
  */

/** @defgroup INTIFAO
  * @brief INTIFAO driver modules
  * @{
  */

/** @defgroup INTIFAO_Private_Defines
  * @{
  */
#define INT_NCLR_PCLR_CLEAR                  ((uint8_t)0x01)
#define INT_EN_MODE_MASK                     ((uint8_t)0xF0)
#define INT_MODE_MASK                        ((uint8_t)0x0E)

#define INTIFAO_INTFLAG1_MASK                ((uint32_t)0xFFFFFF00)

/** @} */
/* End of group INTIFAO_Private_Defines */

/** @defgroup INTIFAO_Private_FunctionPrototypes
  * @{
  */

/** @} */
/* End of group INTIFAO_Private_FunctionPrototypes */

/** @defgroup INTIFAO_Private_Functions
  * @{
  */

/** @} */
/* End of group INTIFAO_Private_Functions */

/** @defgroup INTIFAO_Exported_Functions
  * @{
  */

/**
  * @brief  Setup the INT source for releasing low power mode.
  * @param  INTSource: Select the release INT source
  *   This parameter can be one of the following values:  
  *   INTIFAO_INT_SRC_0, INTIFAO_INT_SRC_1, INTIFAO_INT_SRC_2, INTIFAO_INT_SRC_3,
  *   INTIFAO_INT_SRC_4, INTIFAO_INT_SRC_5, INTIFAO_INT_SRC_I2CS, INTIFAO_INT_SRC_USBWKUP.
  * @param  ActiveState: select the active state for release trigger
  *   This parameter can be one of the following values:
  *   INTIFAO_INT_ACTIVE_STATE_L, 
  *   INTIFAO_INT_ACTIVE_STATE_H, 
  *   INTIFAO_INT_ACTIVE_STATE_FALLING, 
  *   INTIFAO_INT_ACTIVE_STATE_RISING or 
  *   INTIFAO_INT_ACTIVE_STATE_BOTH_EDGES.
  * @param  NewState: Enable or disable this release trigger
  *   This parameter can be one of the following values:
  *   DISABLE or ENABLE
  * @retval None
  */
void INTIFAO_SetSTBYReleaseINTSrc(INTIFAO_INTSrc INTSource,
                             INTIFAO_INTActiveState ActiveState, FunctionalState NewState)
{
    uint8_t num = 0U;
    uint32_t regval;
    
    /* Check the parameters */
    assert_param(IS_INTIFAO_INT_SRC(INTSource));
    assert_param(IS_INTIFAO_INT_ACTIVE_STATE(ActiveState));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if ((INTSource == INTIFAO_INT_SRC_I2CS) || (INTSource == INTIFAO_INT_SRC_USBWKUP)) {
        assert_param(IS_INTIFAO_INT_I2CS_USBWKUP_ACTIVE_STATE(ActiveState));
    } else {
        /* Do nothing */
    }

    num = (uint32_t) INTSource;

    switch (num) {
    case 0U:                   /* STOP2INT_032 */
      regval =	TSB_INTIFAO ->STOP2INT_032;
      regval &= INT_EN_MODE_MASK;
      regval |= NewState;
      regval |= ActiveState << 1;
      TSB_INTIFAO ->STOP2INT_032 = regval;
        break;
    case 1U:                   /* STOP2INT_033 */
      regval =	TSB_INTIFAO ->STOP2INT_033;
      regval &= INT_EN_MODE_MASK;
      regval |= NewState;
      regval |= ActiveState << 1;
      TSB_INTIFAO ->STOP2INT_033 = regval;
        break;
    case 2U:                   /* STOP2INT_034 */ 
      regval =	TSB_INTIFAO ->STOP2INT_034;
      regval &= INT_EN_MODE_MASK;
      regval |= NewState;
      regval |= ActiveState << 1;
      TSB_INTIFAO ->STOP2INT_034 = regval;
        break;
    case 3U:                   /* STOP2INT_035 */
      regval =	TSB_INTIFAO ->STOP2INT_035;
      regval &= INT_EN_MODE_MASK;
      regval |= NewState;
      regval |= ActiveState << 1;
      TSB_INTIFAO ->STOP2INT_035 = regval;
        break;
    case 4U:                   /* STOP2INT_036 */
      regval =	TSB_INTIFAO ->STOP2INT_036;
      regval &= INT_EN_MODE_MASK;
      regval |= NewState;
      regval |= ActiveState << 1;
      TSB_INTIFAO ->STOP2INT_036 = regval;
        break;
    case 5U:                   /* STOP2INT_037 */
      regval =	TSB_INTIFAO ->STOP2INT_037;
      regval &= INT_EN_MODE_MASK;
      regval |= NewState;
      regval |= ActiveState << 1;
      TSB_INTIFAO ->STOP2INT_037 = regval;    
        break;
    case 6U:                   /* STOP2INT_038 */
      regval =	TSB_INTIFAO ->STOP2INT_038;
      regval &= INT_EN_MODE_MASK;
      regval |= NewState;
      regval |= ActiveState << 1;
      TSB_INTIFAO ->STOP2INT_038 = regval;   	
        break;
    case 7U:                   /* STOP2INT_039 */
      regval =	TSB_INTIFAO ->STOP2INT_039;
      regval &= INT_EN_MODE_MASK;
      regval |= NewState;
      regval |= ActiveState << 1;
      TSB_INTIFAO ->STOP2INT_039 = regval;  	
        break; 
    default:
        /* Do nothing */
        break;  
    }   
}       
        
/**
  * @brief  Get the active state of INT source standby clear request
  * @param  INTSource: Select the release INT source
  *   This parameter can be one of the following values:  
  *   INTIFAO_INT_SRC_0, INTIFAO_INT_SRC_1, INTIFAO_INT_SRC_2, INTIFAO_INT_SRC_3,
  *   INTIFAO_INT_SRC_4, INTIFAO_INT_SRC_5, INTIFAO_INT_SRC_I2CS, INTIFAO_INT_SRC_USBWKUP.
  * @retval Active state of the input INT
  *   The value returned can be one of the following values:
  *   INTIFAO_INT_ACTIVE_STATE_FALLING, INTIFAO_INT_ACTIVE_STATE_RISING,
  *   INTIFAO_INT_ACTIVE_STATE_BOTH_EDGES or INTIFAO_INT_ACTIVE_STATE_INVALID
  */
INTIFAO_INTActiveState INTIFAO_GetSTBYReleaseINTState(INTIFAO_INTSrc INTSource)
{
    INTIFAO_INTActiveState int_active_state = INTIFAO_INT_ACTIVE_STATE_INVALID;    
    uint8_t tmp = 0U;
    uint8_t num = 0U;
    uint8_t regval[8] = {0};

    /* Check the parameters */
    assert_param(IS_INTIFAO_INT_SRC(INTSource));
     
    regval[0] = TSB_INTIFAO ->STOP2INT_032 & INT_MODE_MASK;
    regval[1] = TSB_INTIFAO ->STOP2INT_033 & INT_MODE_MASK;
    regval[2] = TSB_INTIFAO ->STOP2INT_034 & INT_MODE_MASK;   
    regval[3] = TSB_INTIFAO ->STOP2INT_035 & INT_MODE_MASK;
    regval[4] = TSB_INTIFAO ->STOP2INT_036 & INT_MODE_MASK;
    regval[5] = TSB_INTIFAO ->STOP2INT_037 & INT_MODE_MASK;
    regval[6] = TSB_INTIFAO ->STOP2INT_038 & INT_MODE_MASK;   
    regval[7] = TSB_INTIFAO ->STOP2INT_039 & INT_MODE_MASK;

    num = (uint32_t) INTSource;
    tmp = regval[num];
    tmp = tmp >>1;
    switch (tmp) {
    case 3U:
        int_active_state = INTIFAO_INT_ACTIVE_STATE_RISING;
        break;
    case 2U:
        int_active_state = INTIFAO_INT_ACTIVE_STATE_FALLING;
        break;
    case 4U:
        int_active_state = INTIFAO_INT_ACTIVE_STATE_BOTH_EDGES;
        break;
    default:
        /* Do nothing */
        break;
    }

    return (int_active_state);
}

/**
  * @brief  Clears the input INT request.
  * @param  INTSource: Select the release INT source
  *   This parameter can be one of the following values:
  *   INTIFAO_INT_SRC_0, INTIFAO_INT_SRC_1, INTIFAO_INT_SRC_2, INTIFAO_INT_SRC_3,
  *   INTIFAO_INT_SRC_4, INTIFAO_INT_SRC_5, INTIFAO_INT_SRC_I2CS, INTIFAO_INT_SRC_USBWKUP.
  * @retval None
  */
void INTIFAO_ClearINTReq(INTIFAO_INTSrc INTSource)
{
    uint8_t num = 0U;

    /* Check the parameters */
    assert_param(IS_INTIFAO_INT_SRC(INTSource));
    
    num = (uint32_t) INTSource;
    switch (num) {
    case 0U:                   /* STOP2INT_032 */
      TSB_INTIFAO_STOP2INT_032_INT032PCLR = INT_NCLR_PCLR_CLEAR; 
      TSB_INTIFAO_STOP2INT_032_INT032NCLR = INT_NCLR_PCLR_CLEAR;
        break;      
     case 1U:                   /* STOP2INT_033 */     
      TSB_INTIFAO_STOP2INT_033_INT033PCLR = INT_NCLR_PCLR_CLEAR; 
      TSB_INTIFAO_STOP2INT_033_INT033NCLR = INT_NCLR_PCLR_CLEAR;
        break;   
    case 2U:                   /* STOP2INT_034 */   
      TSB_INTIFAO_STOP2INT_034_INT034PCLR = INT_NCLR_PCLR_CLEAR; 
      TSB_INTIFAO_STOP2INT_034_INT034NCLR = INT_NCLR_PCLR_CLEAR;
        break;   
    case 3U:                   /* STOP2INT_035 */  
      TSB_INTIFAO_STOP2INT_035_INT035PCLR = INT_NCLR_PCLR_CLEAR; 
      TSB_INTIFAO_STOP2INT_035_INT035NCLR = INT_NCLR_PCLR_CLEAR;
        break;  
    case 4U:                   /* STOP2INT_036 */ 
      TSB_INTIFAO_STOP2INT_036_INT036PCLR = INT_NCLR_PCLR_CLEAR; 
      TSB_INTIFAO_STOP2INT_036_INT036NCLR = INT_NCLR_PCLR_CLEAR;
        break;  
    case 5U:                   /* STOP2INT_037 */
      TSB_INTIFAO_STOP2INT_037_INT037PCLR = INT_NCLR_PCLR_CLEAR; 
      TSB_INTIFAO_STOP2INT_037_INT037NCLR = INT_NCLR_PCLR_CLEAR;
        break;  
    case 6U:                   /* STOP2INT_038 */
      TSB_INTIFAO_STOP2INT_038_INT038PCLR = INT_NCLR_PCLR_CLEAR; 
      TSB_INTIFAO_STOP2INT_038_INT038NCLR = INT_NCLR_PCLR_CLEAR;
        break;  
    case 7U:                   /* STOP2INT_039 */
      TSB_INTIFAO_STOP2INT_039_INT039PCLR = INT_NCLR_PCLR_CLEAR; 
      TSB_INTIFAO_STOP2INT_039_INT039NCLR = INT_NCLR_PCLR_CLEAR;
        break; 
    default:
        /* Do nothing */
        break;  
    } 
}

/**
  * @brief  Get the INTFLAG1.
  * @param  None
  * @retval INTFLAG1 flag
  */
INTIFAO_IntFlag1Factor INTIFAO_GetIntFlag1(void)
{
    INTIFAO_IntFlag1Factor intifao_intflag1_factor = { 0U };

    intifao_intflag1_factor.All = TSB_INTIFSD->FLAG1 & (~INTIFAO_INTFLAG1_MASK);

    return intifao_intflag1_factor;
}

/** @} */
/* End of group INTIFAO_Exported_Functions */

/** @} */
/* End of group INTIFAO */

/** @} */
/* End of group TX00_Periph_Driver */

#endif                          /* defined(__TMPM066_INTIFAO_H) */
