/**
  ******************************************************************************
  * @file    stm32f0xx_ll_comp.c
  * @author  MCD Application Team
  * @brief   COMP LL module driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

#if defined(USE_FULL_LL_DRIVER)

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_ll_comp.h"

#ifdef  USE_FULL_ASSERT
  #include "stm32_assert.h"
#else
  #define assert_param(expr) ((void)0U)
#endif

/** @addtogroup STM32F0xx_LL_Driver
  * @{
  */

#if defined (COMP1) || defined (COMP2)

/** @addtogroup COMP_LL COMP
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/** @addtogroup COMP_LL_Private_Macros
  * @{
  */

/* Check of parameters for configuration of COMP hierarchical scope:          */
/* COMP instance.                                                             */

#define IS_LL_COMP_POWER_MODE(__POWER_MODE__)                                  \
  (   ((__POWER_MODE__) == LL_COMP_POWERMODE_HIGHSPEED)                        \
   || ((__POWER_MODE__) == LL_COMP_POWERMODE_MEDIUMSPEED)                      \
   || ((__POWER_MODE__) == LL_COMP_POWERMODE_LOWPOWER)                         \
   || ((__POWER_MODE__) == LL_COMP_POWERMODE_ULTRALOWPOWER)                    \
  )

/* Note: On this STM32 serie, comparator input plus parameters are            */
/*       the different depending on COMP instances.                           */
#define IS_LL_COMP_INPUT_PLUS(__COMP_INSTANCE__, __INPUT_PLUS__)               \
  (((__COMP_INSTANCE__) == COMP1)                                              \
    ? (                                                                        \
          ((__INPUT_PLUS__) == LL_COMP_INPUT_PLUS_IO1)                         \
       || ((__INPUT_PLUS__) == LL_COMP_INPUT_PLUS_DAC1_CH1)                    \
      )                                                                        \
      :                                                                        \
      (                                                                        \
          ((__INPUT_PLUS__) == LL_COMP_INPUT_PLUS_IO1)                         \
      )                                                                        \
  )

/* Note: On this STM32 serie, comparator input minus parameters are           */
/*       the same on all COMP instances.                                      */
/*       However, comparator instance kept as macro parameter for             */
/*       compatibility with other STM32 families.                             */
#define IS_LL_COMP_INPUT_MINUS(__COMP_INSTANCE__, __INPUT_MINUS__)             \
  (   ((__INPUT_MINUS__) == LL_COMP_INPUT_MINUS_1_4VREFINT)                    \
   || ((__INPUT_MINUS__) == LL_COMP_INPUT_MINUS_1_2VREFINT)                    \
   || ((__INPUT_MINUS__) == LL_COMP_INPUT_MINUS_3_4VREFINT)                    \
   || ((__INPUT_MINUS__) == LL_COMP_INPUT_MINUS_VREFINT)                       \
   || ((__INPUT_MINUS__) == LL_COMP_INPUT_MINUS_DAC1_CH1)                      \
   || ((__INPUT_MINUS__) == LL_COMP_INPUT_MINUS_DAC1_CH2)                      \
   || ((__INPUT_MINUS__) == LL_COMP_INPUT_MINUS_IO1)                           \
  )

#define IS_LL_COMP_INPUT_HYSTERESIS(__INPUT_HYSTERESIS__)                      \
  (   ((__INPUT_HYSTERESIS__) == LL_COMP_HYSTERESIS_NONE)                      \
   || ((__INPUT_HYSTERESIS__) == LL_COMP_HYSTERESIS_LOW)                       \
   || ((__INPUT_HYSTERESIS__) == LL_COMP_HYSTERESIS_MEDIUM)                    \
   || ((__INPUT_HYSTERESIS__) == LL_COMP_HYSTERESIS_HIGH)                      \
  )

#define IS_LL_COMP_OUTPUT_SELECTION(__OUTPUT_SELECTION__)                      \
  (   ((__OUTPUT_SELECTION__) == LL_COMP_OUTPUT_NONE)                          \
   || ((__OUTPUT_SELECTION__) == LL_COMP_OUTPUT_TIM1_BKIN)                     \
   || ((__OUTPUT_SELECTION__) == LL_COMP_OUTPUT_TIM1_IC1)                      \
   || ((__OUTPUT_SELECTION__) == LL_COMP_OUTPUT_TIM1_OCCLR)                    \
   || ((__OUTPUT_SELECTION__) == LL_COMP_OUTPUT_TIM2_IC4)                      \
   || ((__OUTPUT_SELECTION__) == LL_COMP_OUTPUT_TIM2_OCCLR)                    \
   || ((__OUTPUT_SELECTION__) == LL_COMP_OUTPUT_TIM3_IC1)                      \
   || ((__OUTPUT_SELECTION__) == LL_COMP_OUTPUT_TIM3_OCCLR)                    \
  )

#define IS_LL_COMP_OUTPUT_POLARITY(__POLARITY__)                               \
  (   ((__POLARITY__) == LL_COMP_OUTPUTPOL_NONINVERTED)                        \
   || ((__POLARITY__) == LL_COMP_OUTPUTPOL_INVERTED)                           \
  )

/**
  * @}
  */


/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup COMP_LL_Exported_Functions
  * @{
  */

/** @addtogroup COMP_LL_EF_Init
  * @{
  */

/**
  * @brief  De-initialize registers of the selected COMP instance
  *         to their default reset values.
  * @note   If comparator is locked, de-initialization by software is
  *         not possible.
  *         The only way to unlock the comparator is a device hardware reset.
  * @param  COMPx COMP instance
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: COMP registers are de-initialized
  *          - ERROR: COMP registers are not de-initialized
  */
ErrorStatus LL_COMP_DeInit(COMP_TypeDef *COMPx)
{
  ErrorStatus status = SUCCESS;
  
  /* Check the parameters */
  assert_param(IS_COMP_ALL_INSTANCE(COMPx));
  
  /* Note: Hardware constraint (refer to description of this function):       */
  /*       COMP instance must not be locked.                                  */
  if(LL_COMP_IsLocked(COMPx) == 0U)
  {
    /* Note: Connection switch is applicable only to COMP instance COMP1,     */
    /*       therefore is COMP2 is selected the equivalent bit is             */
    /*       kept unmodified.                                                 */
    if(COMPx == COMP1)
    {
      CLEAR_BIT(COMP->CSR,
                (  COMP_CSR_COMP1MODE
                 | COMP_CSR_COMP1INSEL
                 | COMP_CSR_COMP1SW1
                 | COMP_CSR_COMP1OUTSEL
                 | COMP_CSR_COMP1HYST
                 | COMP_CSR_COMP1POL
                 | COMP_CSR_COMP1EN
                ) << __COMP_BITOFFSET_INSTANCE(COMPx)
               );
    }
    else
    {
      CLEAR_BIT(COMP->CSR,
                (  COMP_CSR_COMP1MODE
                 | COMP_CSR_COMP1INSEL
                 | COMP_CSR_COMP1OUTSEL
                 | COMP_CSR_COMP1HYST
                 | COMP_CSR_COMP1POL
                 | COMP_CSR_COMP1EN
                ) << __COMP_BITOFFSET_INSTANCE(COMPx)
               );
    }
    
  }
  else
  {
    /* Comparator instance is locked: de-initialization by software is         */
    /* not possible.                                                           */
    /* The only way to unlock the comparator is a device hardware reset.       */
    status = ERROR;
  }
  
  return status;
}

/**
  * @brief  Initialize some features of COMP instance.
  * @note   This function configures features of the selected COMP instance.
  *         Some features are also available at scope COMP common instance
  *         (common to several COMP instances).
  *         Refer to functions having argument "COMPxy_COMMON" as parameter.
  * @param  COMPx COMP instance
  * @param  COMP_InitStruct Pointer to a @ref LL_COMP_InitTypeDef structure
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: COMP registers are initialized
  *          - ERROR: COMP registers are not initialized
  */
ErrorStatus LL_COMP_Init(COMP_TypeDef *COMPx, LL_COMP_InitTypeDef *COMP_InitStruct)
{
  ErrorStatus status = SUCCESS;
  
  /* Check the parameters */
  assert_param(IS_COMP_ALL_INSTANCE(COMPx));
  assert_param(IS_LL_COMP_POWER_MODE(COMP_InitStruct->PowerMode));
  assert_param(IS_LL_COMP_INPUT_PLUS(COMPx, COMP_InitStruct->InputPlus));
  assert_param(IS_LL_COMP_INPUT_MINUS(COMPx, COMP_InitStruct->InputMinus));
  assert_param(IS_LL_COMP_INPUT_HYSTERESIS(COMP_InitStruct->InputHysteresis));
  assert_param(IS_LL_COMP_OUTPUT_SELECTION(COMP_InitStruct->OutputSelection));
  assert_param(IS_LL_COMP_OUTPUT_POLARITY(COMP_InitStruct->OutputPolarity));
  
  /* Note: Hardware constraint (refer to description of this function)        */
  /*       COMP instance must not be locked.                                  */
  if(LL_COMP_IsLocked(COMPx) == 0U)
  {
    /* Configuration of comparator instance :                                 */
    /*  - PowerMode                                                           */
    /*  - InputPlus                                                           */
    /*  - InputMinus                                                          */
    /*  - InputHysteresis                                                     */
    /*  - OutputSelection                                                     */
    /*  - OutputPolarity                                                      */
    /* Note: Connection switch is applicable only to COMP instance COMP1,     */
    /*       therefore is COMP2 is selected the equivalent bit is             */
    /*       kept unmodified.                                                 */
    if(COMPx == COMP1)
    {
      MODIFY_REG(COMP->CSR,
                 (  COMP_CSR_COMP1MODE
                  | COMP_CSR_COMP1INSEL
                  | COMP_CSR_COMP1SW1
                  | COMP_CSR_COMP1OUTSEL
                  | COMP_CSR_COMP1HYST
                  | COMP_CSR_COMP1POL
                 ) << __COMP_BITOFFSET_INSTANCE(COMPx)
                ,
                 (  COMP_InitStruct->PowerMode
                  | COMP_InitStruct->InputPlus
                  | COMP_InitStruct->InputMinus
                  | COMP_InitStruct->InputHysteresis
                  | COMP_InitStruct->OutputSelection
                  | COMP_InitStruct->OutputPolarity
                 ) << __COMP_BITOFFSET_INSTANCE(COMPx)
                );
    }
    else
    {
      MODIFY_REG(COMP->CSR,
                 (  COMP_CSR_COMP1MODE
                  | COMP_CSR_COMP1INSEL
                  | COMP_CSR_COMP1OUTSEL
                  | COMP_CSR_COMP1HYST
                  | COMP_CSR_COMP1POL
                 ) << __COMP_BITOFFSET_INSTANCE(COMPx)
                ,
                 (  COMP_InitStruct->PowerMode
                  | COMP_InitStruct->InputPlus
                  | COMP_InitStruct->InputMinus
                  | COMP_InitStruct->InputHysteresis
                  | COMP_InitStruct->OutputSelection
                  | COMP_InitStruct->OutputPolarity
                 ) << __COMP_BITOFFSET_INSTANCE(COMPx)
                );
    }
    
  }
  else
  {
    /* Initialization error: COMP instance is locked.                         */
    status = ERROR;
  }
  
  return status;
}

/**
  * @brief Set each @ref LL_COMP_InitTypeDef field to default value.
  * @param COMP_InitStruct pointer to a @ref LL_COMP_InitTypeDef structure
  *                         whose fields will be set to default values.
  * @retval None
  */
void LL_COMP_StructInit(LL_COMP_InitTypeDef *COMP_InitStruct)
{
  /* Set COMP_InitStruct fields to default values */
  COMP_InitStruct->PowerMode            = LL_COMP_POWERMODE_ULTRALOWPOWER;
  COMP_InitStruct->InputPlus            = LL_COMP_INPUT_PLUS_IO1;
  COMP_InitStruct->InputMinus           = LL_COMP_INPUT_MINUS_VREFINT;
  COMP_InitStruct->InputHysteresis      = LL_COMP_HYSTERESIS_NONE;
  COMP_InitStruct->OutputSelection      = LL_COMP_OUTPUT_NONE;
  COMP_InitStruct->OutputPolarity       = LL_COMP_OUTPUTPOL_NONINVERTED;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* COMP1 || COMP2 */

/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
