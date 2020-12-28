/**
  ******************************************************************************
  * @file    stm32f0xx_hal_pwr_ex.c
  * @author  MCD Application Team
  * @brief   Extended PWR HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Power Controller (PWR) peripheral:
  *           + Extended Initialization and de-initialization functions
  *           + Extended Peripheral Control functions
  *         
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/** @addtogroup STM32F0xx_HAL_Driver
  * @{
  */

/** @defgroup PWREx PWREx
  * @brief    PWREx HAL module driver
  * @{
  */

#ifdef HAL_PWR_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup PWREx_Private_Constants PWREx Private Constants
  * @{
  */
#define PVD_MODE_IT               (0x00010000U)
#define PVD_MODE_EVT              (0x00020000U)
#define PVD_RISING_EDGE           (0x00000001U)
#define PVD_FALLING_EDGE          (0x00000002U)
/**
  * @}
  */
 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

/** @defgroup PWREx_Exported_Functions PWREx Exported Functions
  * @{
  */

/** @defgroup PWREx_Exported_Functions_Group1 Peripheral Extended Control Functions
  *  @brief   Extended Peripheral Control functions
  *
@verbatim

 ===============================================================================
                 ##### Peripheral extended control functions #####
 ===============================================================================
    
    *** PVD configuration ***
    =========================
    [..]
      (+) The PVD is used to monitor the VDD power supply by comparing it to a
          threshold selected by the PVD Level (PLS[2:0] bits in the PWR_CR).
      (+) A PVDO flag is available to indicate if VDD/VDDA is higher or lower
          than the PVD threshold. This event is internally connected to the EXTI
          line16 and can generate an interrupt if enabled. This is done through
          HAL_PWR_ConfigPVD(), HAL_PWR_EnablePVD() functions.
      (+) The PVD is stopped in Standby mode.
      -@- PVD is not available on STM32F030x4/x6/x8

    *** VDDIO2 Monitor Configuration ***
    ====================================
    [..]
      (+) VDDIO2 monitor is used to monitor the VDDIO2 power supply by comparing it 
          to VREFInt Voltage
      (+) This monitor is internally connected to the EXTI line31
          and can generate an interrupt if enabled. This is done through
          HAL_PWREx_EnableVddio2Monitor() function.
      -@- VDDIO2 is available on STM32F07x/09x/04x
                    
@endverbatim
  * @{
  */

#if defined (STM32F031x6) || defined (STM32F051x8) || \
    defined (STM32F071xB) || defined (STM32F091xC) || \
    defined (STM32F042x6) || defined (STM32F072xB)
/**
  * @brief Configures the voltage threshold detected by the Power Voltage Detector(PVD).
  * @param sConfigPVD pointer to an PWR_PVDTypeDef structure that contains the configuration
  *        information for the PVD.
  * @note Refer to the electrical characteristics of your device datasheet for
  *         more details about the voltage threshold corresponding to each
  *         detection level.
  * @retval None
  */
void HAL_PWR_ConfigPVD(PWR_PVDTypeDef *sConfigPVD)
{
  /* Check the parameters */
  assert_param(IS_PWR_PVD_LEVEL(sConfigPVD->PVDLevel));
  assert_param(IS_PWR_PVD_MODE(sConfigPVD->Mode));

  /* Set PLS[7:5] bits according to PVDLevel value */
  MODIFY_REG(PWR->CR, PWR_CR_PLS, sConfigPVD->PVDLevel);
  
  /* Clear any previous config. Keep it clear if no event or IT mode is selected */
  __HAL_PWR_PVD_EXTI_DISABLE_EVENT();
  __HAL_PWR_PVD_EXTI_DISABLE_IT();
  __HAL_PWR_PVD_EXTI_DISABLE_RISING_EDGE();__HAL_PWR_PVD_EXTI_DISABLE_FALLING_EDGE();

  /* Configure interrupt mode */
  if((sConfigPVD->Mode & PVD_MODE_IT) == PVD_MODE_IT)
  {
    __HAL_PWR_PVD_EXTI_ENABLE_IT();
  }
  
  /* Configure event mode */
  if((sConfigPVD->Mode & PVD_MODE_EVT) == PVD_MODE_EVT)
  {
    __HAL_PWR_PVD_EXTI_ENABLE_EVENT();
  }
  
  /* Configure the edge */
  if((sConfigPVD->Mode & PVD_RISING_EDGE) == PVD_RISING_EDGE)
  {
    __HAL_PWR_PVD_EXTI_ENABLE_RISING_EDGE();
  }
  
  if((sConfigPVD->Mode & PVD_FALLING_EDGE) == PVD_FALLING_EDGE)
  {
    __HAL_PWR_PVD_EXTI_ENABLE_FALLING_EDGE();
  }
}

/**
  * @brief Enables the Power Voltage Detector(PVD).
  * @retval None
  */
void HAL_PWR_EnablePVD(void)
{
  PWR->CR |= (uint32_t)PWR_CR_PVDE;
}

/**
  * @brief Disables the Power Voltage Detector(PVD).
  * @retval None
  */
void HAL_PWR_DisablePVD(void)
{
  PWR->CR &= ~((uint32_t)PWR_CR_PVDE);
}

/**
  * @brief This function handles the PWR PVD interrupt request.
  * @note This API should be called under the  PVD_IRQHandler() or PVD_VDDIO2_IRQHandler().
  * @retval None
  */
void HAL_PWR_PVD_IRQHandler(void)
{
  /* Check PWR exti flag */
  if(__HAL_PWR_PVD_EXTI_GET_FLAG() != RESET)
  {
    /* PWR PVD interrupt user callback */
    HAL_PWR_PVDCallback();

    /* Clear PWR Exti pending bit */
    __HAL_PWR_PVD_EXTI_CLEAR_FLAG();
  }
}

/**
  * @brief PWR PVD interrupt callback
  * @retval None
  */
__weak void HAL_PWR_PVDCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_PWR_PVDCallback could be implemented in the user file
   */
}

#endif /* defined (STM32F031x6) || defined (STM32F051x8) || */
       /* defined (STM32F071xB) || defined (STM32F091xC) || */
       /* defined (STM32F042x6) || defined (STM32F072xB)    */

#if defined (STM32F042x6) || defined (STM32F048xx) || \
    defined (STM32F071xB) || defined (STM32F072xB) || defined (STM32F078xx) || \
    defined (STM32F091xC) || defined (STM32F098xx)
/**
  * @brief Enable VDDIO2 monitor: enable Exti 31 and falling edge detection.
  * @note If Exti 31 is enable correlty and VDDIO2 voltage goes below Vrefint,
          an interrupt is generated Irq line 1.
          NVIS has to be enable by user.
  * @retval None
  */
void HAL_PWREx_EnableVddio2Monitor(void)
{
  __HAL_PWR_VDDIO2_EXTI_ENABLE_IT();
  __HAL_PWR_VDDIO2_EXTI_ENABLE_FALLING_EDGE();
}

/**
  * @brief Disable the Vddio2 Monitor.
  * @retval None
  */
void HAL_PWREx_DisableVddio2Monitor(void)
{
  __HAL_PWR_VDDIO2_EXTI_DISABLE_IT();
  __HAL_PWR_VDDIO2_EXTI_DISABLE_FALLING_EDGE();

}

/**
  * @brief This function handles the PWR Vddio2 monitor interrupt request.
  * @note This API should be called under the VDDIO2_IRQHandler() PVD_VDDIO2_IRQHandler().
  * @retval None
  */
void HAL_PWREx_Vddio2Monitor_IRQHandler(void)
{
  /* Check PWR exti flag */
  if(__HAL_PWR_VDDIO2_EXTI_GET_FLAG() != RESET)
  {
    /* PWR Vddio2 monitor interrupt user callback */
    HAL_PWREx_Vddio2MonitorCallback();

    /* Clear PWR Exti pending bit */
    __HAL_PWR_VDDIO2_EXTI_CLEAR_FLAG();
  }
}

/**
  * @brief PWR Vddio2 Monitor interrupt callback
  * @retval None
  */
__weak void HAL_PWREx_Vddio2MonitorCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_PWREx_Vddio2MonitorCallback could be implemented in the user file
   */
}

#endif /* defined (STM32F042x6) || defined (STM32F048xx) || \
          defined (STM32F071xB) || defined (STM32F072xB) || defined (STM32F078xx) || \
          defined (STM32F091xC) || defined (STM32F098xx) */

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_PWR_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
