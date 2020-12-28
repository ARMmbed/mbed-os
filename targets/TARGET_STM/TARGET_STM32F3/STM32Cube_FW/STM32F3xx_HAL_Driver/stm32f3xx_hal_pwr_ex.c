/**
  ******************************************************************************
  * @file    stm32f3xx_hal_pwr_ex.c
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
#include "stm32f3xx_hal.h"

/** @addtogroup STM32F3xx_HAL_Driver
  * @{
  */

/** @defgroup PWREx PWREx
  * @brief    PWREx HAL module driver
  * @{
  */

#ifdef HAL_PWR_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup PWREx_Private_Constants PWR Extended Private Constants
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

/** @defgroup PWREx_Exported_Functions PWR Extended Exported Functions
  * @{
  */

/** @defgroup PWREx_Exported_Functions_Group1 Peripheral Extended Control Functions
  *  @brief   Extended Peripheral Control functions
  *
@verbatim

 ===============================================================================
                 ##### Peripheral Extended control functions #####
 ===============================================================================
    *** PVD configuration (present on all other devices than STM32F3x8 devices) ***
    =========================
    [..]
      (+) The PVD is used to monitor the VDD power supply by comparing it to a
          threshold selected by the PVD Level (PLS[2:0] bits in the PWR_CR).
      (+) A PVDO flag is available to indicate if VDD/VDDA is higher or lower
          than the PVD threshold. This event is internally connected to the EXTI
          line16 and can generate an interrupt if enabled. This is done through
          __HAL_PWR_PVD_EXTI_ENABLE_IT() macro
      (+) The PVD is stopped in Standby mode.
      -@- PVD is not available on STM32F3x8 Product Line


    *** Voltage regulator ***
    =========================
    [..]
      (+) The voltage regulator is always enabled after Reset. It works in three different
          modes.
          In Run mode, the regulator supplies full power to the 1.8V domain (core, memories
          and digital peripherals).
          In Stop mode, the regulator supplies low power to the 1.8V domain, preserving
          contents of registers and SRAM.
          In Stop mode, the regulator is powered off. The contents of the registers and SRAM
          are lost except for the Standby circuitry and the Backup Domain.
          Note: in the STM32F3x8xx devices, the voltage regulator is bypassed and the
          microcontroller must be powered from a nominal VDD = 1.8V +/-8U% voltage.


      (+) A PVDO flag is available to indicate if VDD/VDDA is higher or lower
          than the PVD threshold. This event is internally connected to the EXTI
          line16 and can generate an interrupt if enabled. This is done through
          __HAL_PWR_PVD_EXTI_ENABLE_IT() macro
      (+) The PVD is stopped in Standby mode.


    *** SDADC power configuration ***
    ================================
    [..]
      (+) On STM32F373xC/STM32F378xx devices, there are up to 
          3 SDADC instances that can be enabled/disabled.

@endverbatim
  * @{
  */

#if defined(STM32F302xE) || defined(STM32F303xE) || \
    defined(STM32F302xC) || defined(STM32F303xC) || \
    defined(STM32F303x8) || defined(STM32F334x8) || \
    defined(STM32F301x8) || defined(STM32F302x8) || \
    defined(STM32F373xC)

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
  SET_BIT(PWR->CR, PWR_CR_PVDE);  
}

/**
  * @brief Disables the Power Voltage Detector(PVD).
  * @retval None
  */
void HAL_PWR_DisablePVD(void)
{
  CLEAR_BIT(PWR->CR, PWR_CR_PVDE);  
}

/**
  * @brief This function handles the PWR PVD interrupt request.
  * @note This API should be called under the PVD_IRQHandler().
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

#endif /* STM32F302xE || STM32F303xE || */
       /* STM32F302xC || STM32F303xC || */
       /* STM32F303x8 || STM32F334x8 || */
       /* STM32F301x8 || STM32F302x8 || */
       /* STM32F373xC                   */

#if defined(STM32F373xC) || defined(STM32F378xx)

/**
  * @brief  Enables the SDADC peripheral functionaliy
  * @param  Analogx specifies the SDADC peripheral instance.
  *   This parameter can be: PWR_SDADC_ANALOG1, PWR_SDADC_ANALOG2 or PWR_SDADC_ANALOG3.
  * @retval None
  */
void HAL_PWREx_EnableSDADC(uint32_t Analogx)
{
  /* Check the parameters */
  assert_param(IS_PWR_SDADC_ANALOG(Analogx));

  /* Enable PWR clock interface for SDADC use */
  __HAL_RCC_PWR_CLK_ENABLE();
    
  PWR->CR |= Analogx;
}

/**
  * @brief  Disables the SDADC peripheral functionaliy
  * @param  Analogx specifies the SDADC peripheral instance.
  *   This parameter can be: PWR_SDADC_ANALOG1, PWR_SDADC_ANALOG2 or PWR_SDADC_ANALOG3.
  * @retval None
  */
void HAL_PWREx_DisableSDADC(uint32_t Analogx)
{
  /* Check the parameters */
  assert_param(IS_PWR_SDADC_ANALOG(Analogx));
  
  PWR->CR &= ~Analogx;
}

#endif /* STM32F373xC || STM32F378xx */

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
