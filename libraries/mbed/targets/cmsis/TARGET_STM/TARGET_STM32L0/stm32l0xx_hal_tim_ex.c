/**
  ******************************************************************************
  * @file    stm32l0xx_hal_tim_ex.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-June-2014
  * @brief   TIM HAL module driver.
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Timer (TIM) peripheral:
  *           + Time Hall Sensor Interface Initialization
  *           + Time Hall Sensor Interface Start
  *           + Time Master and Slave synchronization configuration
  @verbatim 
================================================================================
          ##### TIM specific features integration #####
================================================================================
           
    [..] The Timer features include: 
         (#) 16-bit up, down, up/down auto-reload counter.
         (#) 16-bit programmable prescaler allowing dividing (also on the fly) the counter clock
             frequency either by any factor between 1 and 65536.
         (#) Up to 4 independent channels for:
           Input Capture
           Output Compare
           PWM generation (Edge and Center-aligned Mode)
           One-pulse mode output
         (#) Synchronization circuit to control the timer with external signals and to interconnect
            several timers together.
         (#) Supports incremental (quadrature) encoder and hall-sensor circuitry for positioning
           purposes               
   
            ##### How to use this driver #####
================================================================================
    [..]
     (#) Enable the TIM interface clock using 
         __TIMx_CLK_ENABLE(); 
       
     (#) TIM pins configuration
          (++) Enable the clock for the TIM GPIOs using the following function:
              __GPIOx_CLK_ENABLE();   
          (++) Configure these TIM pins in Alternate function mode using HAL_GPIO_Init();  

     (#) The external Clock can be configured, if needed (the default clock is the internal clock from the APBx), 
         using the following function:
         HAL_TIM_ConfigClockSource, the clock configuration should be done before any start function.
  
     (#) Configure the TIM in the desired operating mode using one of the 
         configuration function of this driver:
          (++) HAL_TIMEx_MasterConfigSynchronization() to configure the peripheral in master mode.

     (#) Remap the Timer I/O using HAL_TIMEx_RemapConfig() API.

  
  @endverbatim
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
#include "stm32l0xx_hal.h"

/** @addtogroup STM32L0xx_HAL_Driver
  * @{
  */

/** @defgroup TIMEx 
  * @brief TIMEx HAL module driver
  * @{
  */

#ifdef HAL_TIM_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/   
/* Private functions ---------------------------------------------------------*/

/** @defgroup TIMEx_Private_Functions
  * @{
  */


/** @defgroup TIMEx_Group1 Peripheral Control functions
 *  @brief   	Peripheral Control functions 
 *
@verbatim   
 ===============================================================================
             ##### Peripheral Control functions #####
 ===============================================================================  
    [..]  This section provides functions allowing to:
      (+) Configure Master and the Slave synchronization.
      
@endverbatim
  * @{
  */

/**
  * @brief  Configures the TIM in master mode.
  * @param  htim: TIM handle.   
  * @param  sMasterConfig: pointer to a TIM_MasterConfigTypeDef structure that
  *         contains the selected trigger output (TRGO) and the Master/Slave 
  *         mode. 
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_MasterConfigSynchronization(TIM_HandleTypeDef *htim, TIM_MasterConfigTypeDef * sMasterConfig)
{
  /* Check the parameters */
  assert_param(IS_TIM_MASTER_INSTANCE(htim->Instance));
  assert_param(IS_TIM_TRGO_SOURCE(sMasterConfig->MasterOutputTrigger));
  assert_param(IS_TIM_MSM_STATE(sMasterConfig->MasterSlaveMode));
  
  __HAL_LOCK(htim);
  
  /* Change the handler state */
  htim->State = HAL_TIM_STATE_BUSY;

  /* Reset the MMS Bits */
  htim->Instance->CR2 &= ~TIM_CR2_MMS;
  /* Select the TRGO source */
  htim->Instance->CR2 |= sMasterConfig->MasterOutputTrigger;

  /* Reset the MSM Bit */
  htim->Instance->SMCR &= ~TIM_SMCR_MSM;
  /* Set or Reset the MSM Bit */
  htim->Instance->SMCR |= sMasterConfig->MasterSlaveMode;

  htim->State = HAL_TIM_STATE_READY;
  
  __HAL_UNLOCK(htim);
  
  return HAL_OK;
}  

/**
  * @brief  Configures the TIM2, TIM21 and TIM22 Remapping input capabilities.
  * @param  htim: pointer to a TIM_HandleTypeDef structure that contains
  *                the configuration information for TIM module.
  * @param  TIM_Remap: specifies the TIM input remapping source.
  *          This parameter can be one of the following values:
  *            @arg TIM2_ETR_GPIO: TIM2  ETR is connected to GPIO (default)
  *            @arg TIM2_ETR_HSI48: TIM2  ETR is connected to HSI48
  *            @arg TIM2_ETR_LSE: TIM2  ETR is connected to LSE
  *            @arg TIM2_ETR_COMP2_OUT: TIM2  ETR is connected to COMP2 output
  *            @arg TIM2_ETR_COMP1_OUT: TIM2  ETR is connected to COMP1 output
  *            @arg TIM2_TI4_GPIO1: TIM2  TI4 is connected to GPIO1(default)
  *            @arg TIM2_TI4_COMP1: TIM2  TI4 is connected to COMP1
  *            @arg TIM2_TI4_COMP2: TIM2  TI4 is connected to COMP2
  *            @arg TIM2_TI4_GPIO2: TIM2  TI4 is connected to GPIO2
  *            @arg TIM21_ETR_GPIO: TIM21 ETR is connected to GPIO(default)
  *            @arg TIM21_ETR_COMP2_OUT: TIM21 ETR is connected to COMP2 output
  *            @arg TIM21_ETR_COMP1_OUT: TIM21 ETR is connected to COMP1 output
  *            @arg TIM21_ETR_LSE: TIM21 ETR is connected to LSE
  *            @arg TIM21_TI1_MCO: TIM21 TI1 is connected to MCO
  *            @arg TIM21_TI1_RTC_WKUT_IT: TIM21 TI1 is connected to RTC WAKEUP interrupt
  *            @arg TIM21_TI1_HSE_RTC: TIM21 TI1 is connected to HSE_RTC 
  *            @arg TIM21_TI1_MSI: TIM21 TI1 is connected to MSI clock
  *            @arg TIM21_TI1_LSE: TIM21 TI1 is connected to LSE
  *            @arg TIM21_TI1_LSI: TIM21 TI1 is connected to LSI  
  *            @arg TIM21_TI1_COMP1_OUT: TIM21 TI1 is connected to COMP1_OUT
  *            @arg TIM21_TI1_GPIO: TIM21 TI1 is connected to GPIO(default)
  *            @arg TIM21_TI2_GPIO: TIM21 TI2 is connected to GPIO(default)
  *            @arg TIM21_TI2_COMP2_OUT: TIM21 TI2 is connected to COMP2 output
  *            @arg TIM22_ETR_LSE: TIM22 ETR is connected to LSE
  *            @arg TIM22_ETR_COMP2_OUT: TIM22 ETR is connected to COMP2 output
  *            @arg TIM22_ETR_COMP1_OUT: TIM22 ETR is connected to COMP1 output
  *            @arg TIM22_ETR_GPIO: TIM22 ETR is connected to GPIO(default)  
  *            @arg TIM22_TI1_GPIO1: TIM22 TI1 is connected to GPIO(default)
  *            @arg TIM22_TI1_COMP2_OUT: TIM22 TI1 is connected to COMP2 output 
  *            @arg TIM22_TI1_COMP1_OUT: TIM22 TI1 is connected to COMP1 output
  *            @arg TIM22_TI1_GPIO2: TIM22 TI1 is connected to GPIO 
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIMEx_RemapConfig(TIM_HandleTypeDef *htim, uint32_t Remap)
{
  __HAL_LOCK(htim);
    
  /* Check parameters */
  assert_param(IS_TIM_REMAP_INSTANCE(htim->Instance));
  assert_param(IS_TIM_REMAP(Remap));
  
  /* Change the handler state */
  htim->State = HAL_TIM_STATE_BUSY;
  
  /* Set the Timer remapping configuration */
  htim->Instance->OR &=  (uint32_t)(Remap >> 16);
  htim->Instance->OR |=  Remap;
  
  /* Change the handler state */
  htim->State = HAL_TIM_STATE_READY;
  
  __HAL_UNLOCK(htim);  
  
  return HAL_OK;
}

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_TIM_MODULE_ENABLED */
/**
  * @}
  */ 

/**
  * @}
  */ 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
