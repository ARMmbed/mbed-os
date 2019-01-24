/**
  ******************************************************************************
  * @file    misc.c
  * @author  VMA Application Team
  * @version V2.0.0
  * @date    21-March-2016
  * @brief   This file provides all the miscellaneous firmware functions (add-on
  *          to CMSIS functions).
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "../../Periph_Driver/inc/misc.h"

/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */

/** @addtogroup MISC_Peripheral  MISC Peripheral
  * @{
  */

/** @defgroup MISC_Private_TypesDefinitions Private Types Definitions
  * @{
  */

/**
  * @}
  */ 

/** @defgroup MISC_Private_Defines Provate Defines
  * @{
  */

#define AIRCR_VECTKEY_MASK    ((uint32_t)0x05FA0000)

/**
  * @}
  */

/** @defgroup MISC_Private_Macros Private Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup MISC_Private_Variables Private Variables
  * @{
  */

/**
  * @}
  */

/** @defgroup MISC_Private_FunctionPrototypes Private Function Prototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup MISC_Public_Functions Public Function
  * @{
  */

/**
  * @brief  Initializes the NVIC peripheral according to the specified
  *         parameters in the NVIC_InitStruct.
  * @param  NVIC_InitStruct: pointer to a @ref NVIC_InitTypeDef structure that contains
  *         the configuration information for the specified NVIC peripheral.
  * @retval None
  */
void NVIC_Init(NVIC_InitType* NVIC_InitStruct)
{ 
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NVIC_InitStruct->NVIC_IRQChannelCmd));
  assert_param(IS_NVIC_PREEMPTION_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority));
    
  if (NVIC_InitStruct->NVIC_IRQChannelCmd != DISABLE)
  {
    /* Compute the Corresponding IRQ Priority --------------------------------*/    
    NVIC_SetPriority((IRQn_Type)NVIC_InitStruct->NVIC_IRQChannel, NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority);
    
    /* Enable the Selected IRQ Channels --------------------------------------*/
    NVIC->ISER[NVIC_InitStruct->NVIC_IRQChannel >> 0x05] =
      (uint32_t)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);
  }
  else
  {
    /* Disable the Selected IRQ Channels -------------------------------------*/
    NVIC->ICER[NVIC_InitStruct->NVIC_IRQChannel >> 0x05] =
      (uint32_t)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);
  }
}

/**
  * @brief  Enables or disables sleep/stop mode on system level.
  * @param  NewState: functional state @ref FunctionalState
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SystemSleepCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE) {
    /** Enable Sleep/Stop mode entry on system level */
    SCB->SCR |= 1<<SCB_SCR_SLEEPDEEP_Pos;
  }
  else {
    /** Disable Sleep/Stop mode entry on system level */
    SCB->SCR &= ~(1<<SCB_SCR_SLEEPDEEP_Pos);
  }
}

void SysTick_State(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE) {
    SysTick->CTRL  |= SysTick_CTRL_ENABLE_Msk;
  }
  else {
    SysTick->CTRL  &= ~SysTick_CTRL_ENABLE_Msk;
  }
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

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
