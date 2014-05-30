/**
  ******************************************************************************
  * @file    stm32f0xx_hal_pwr_ex.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    28-May-2014
  * @brief   Extended PWR HAL module driver.
  *    
  *          This file provides firmware functions to manage the following
  *          functionalities of the Power Controller (PWR) peripheral:
  *           + Extended Initialization and de-initialization function
  *           + Extended Peripheral Control function
  *         
  @verbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
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
  *
  ******************************************************************************  
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/** @addtogroup STM32F0xx_HAL_Driver
  * @{
  */

/** @defgroup PWREx
  * @brief PWR Extended HAL module driver
  * @{
  */

#ifdef HAL_PWR_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup PWREx_Private_Functions
  * @{
  */

/** @defgroup PWREx_Group1 Extended Peripheral Control functions
  *  @brief   Extended Peripheral Control functions
  *
@verbatim

 ===============================================================================
                 ##### Peripheral Control function #####
 ===============================================================================
    
    *** PVD configuration ***
    =========================
    [..]
      (+) The PVD is used to monitor the VDD power supply by comparing it to a
          threshold selected by the PVD Level (PLS[2:0] bits in the PWR_CR).
      (+) A PVDO flag is available to indicate if VDD/VDDA is higher or lower
          than the PVD threshold. This event is internally connected to the EXTI
          line16 and can generate an interrupt if enabled. This is done through
          __HAL_PVD_EXTI_ENABLE_IT() macro
      (+) The PVD is stopped in Standby mode.
          Note: PVD is not available on STM32F030x4/x6/x8
          
@endverbatim
  * @{
  */

#if defined (STM32F031x6) || defined (STM32F042x6) || defined (STM32F051x8) || \
    defined (STM32F071xB) || defined (STM32F072xB)

/**
  * @brief Configures the voltage threshold detected by the Power Voltage Detector(PVD).
  * @param sConfigPVD: pointer to an PWR_PVDTypeDef structure that contains the configuration
  *        information for the PVD.
  * @note Refer to the electrical characteristics of your device datasheet for
  *         more details about the voltage threshold corresponding to each
  *         detection level.
  * @retval None
  */
void HAL_PWR_PVDConfig(PWR_PVDTypeDef *sConfigPVD)
{
  /* Check the parameters */
  assert_param(IS_PWR_PVD_LEVEL(sConfigPVD->PVDLevel));
  assert_param(IS_PWR_PVD_MODE(sConfigPVD->Mode));

  /* Set PLS[7:5] bits according to PVDLevel value */
  MODIFY_REG(PWR->CR, PWR_CR_PLS, sConfigPVD->PVDLevel);

  /* Configure the EXTI 16 interrupt */
  if((sConfigPVD->Mode == PWR_MODE_IT_RISING_FALLING) ||\
     (sConfigPVD->Mode == PWR_MODE_IT_FALLING) ||\
     (sConfigPVD->Mode == PWR_MODE_IT_RISING))
  {
    __HAL_PVD_EXTI_ENABLE_IT(PWR_EXTI_LINE_PVD);
  }
  /* Configure the rising edge */
  if((sConfigPVD->Mode == PWR_MODE_IT_RISING_FALLING) ||\
     (sConfigPVD->Mode == PWR_MODE_IT_RISING))
  {
    EXTI->RTSR |= PWR_EXTI_LINE_PVD;
  }
  /* Configure the falling edge */
  if((sConfigPVD->Mode == PWR_MODE_IT_RISING_FALLING) ||\
     (sConfigPVD->Mode == PWR_MODE_IT_FALLING))
  {
    EXTI->FTSR |= PWR_EXTI_LINE_PVD;
  }
}

/**
  * @brief Enables the Power Voltage Detector(PVD).
  * @param None
  * @retval None
  */
void HAL_PWR_EnablePVD(void)
{
  PWR->CR |= (uint32_t)PWR_CR_PVDE;
}

/**
  * @brief Disables the Power Voltage Detector(PVD).
  * @param None
  * @retval None
  */
void HAL_PWR_DisablePVD(void)
{
  PWR->CR &= ~((uint32_t)PWR_CR_PVDE);
}

/**
  * @brief This function handles the PWR PVD interrupt request.
  * @note This API should be called under the PVD_IRQHandler().
  * @param None
  * @retval None
  */
void HAL_PWR_PVD_IRQHandler(void)
{
  /* Check PWR exti flag */
  if(__HAL_PVD_EXTI_GET_FLAG(PWR_EXTI_LINE_PVD) != RESET)
  {
    /* PWR PVD interrupt user callback */
    HAL_PWR_PVDCallback();

    /* Clear PWR Exti pending bit */
    __HAL_PVD_EXTI_CLEAR_FLAG(PWR_EXTI_LINE_PVD);
  }
}

/**
  * @brief PWR PVD interrupt callback
  * @param None
  * @retval None
  */
__weak void HAL_PWR_PVDCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_PWR_PVDCallback could be implemented in the user file
   */
}

#endif /* defined (STM32F031x6) || defined (STM32F042x6) || defined (STM32F051x8) || */
       /* defined (STM32F071xB) || defined (STM32F072xB)    */


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
