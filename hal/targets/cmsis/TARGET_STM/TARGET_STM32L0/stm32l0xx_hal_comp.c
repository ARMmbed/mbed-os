/**
  ******************************************************************************
  * @file    stm32l0xx_hal_comp.c
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    8-January-2016
  * @brief   COMP HAL module driver.
  *    
  *          This file provides firmware functions to manage the following 
  *          functionalities of the COMP peripheral:
  *           + Initialization/de-initialization functions
  *           + I/O operation functions
  *           + Peripheral Control functions 
  *           + Peripheral State functions
  *         
  @verbatim
================================================================================
          ##### COMP Peripheral features #####
================================================================================
           
  [..]       
      The STM32L0xx device family integrates  2 analog comparators COMP1 and COMP2:
      (#) The non inverting input and inverting input can be set to GPIO pins
          as shown in table1. COMP Inputs below.
  
      (#) The COMP output is available using HAL_COMP_GetOutputLevel()
          and can be set on GPIO pins. Refer to table 2. COMP Outputs below.
  
      (#) The COMP output can be redirected to embedded timers (TIM2, TIM21, TIM22...) and LPTIM
          Refer to TIM and LPTIM drivers.
  
      (#) The comparators COMP1 and COMP2 can be combined in window mode and only COMP2 non inverting input can be used as non-inverting input.
  
      (#) The 2 comparators have interrupt capability with wake-up
          from Sleep and Stop modes (through the EXTI controller):
          (++) COMP1 is internally connected to EXTI Line 21
          (++) COMP2 is internally connected to EXTI Line 22

          From the corresponding IRQ handler, the right interrupt source can be retrieved with the 
          macro __HAL_COMP_EXTI_GET_FLAG(). Possible values are:
          (++) COMP_EXTI_LINE_COMP1
          (++) COMP_EXTI_LINE_COMP2


[..] Table 1. COMP Inputs for the STM32L0xx devices
 +--------------------------------------------------+     
 |                 |                | COMP1 | COMP2 | 
 |-----------------|----------------|---------------|
 |                 | 1/4 VREFINT    |  --   |  OK   |  
 |                 | 1/2 VREFINT    |  --   |  OK   |
 |                 | 3/4 VREFINT    |  --   |  OK   |
 | Inverting Input | VREFINT        |  OK   |  OK   | 
 |                 | DAC OUT (PA4)  |  OK   |  OK   |  
 |                 | IO1            |  PA0  |  PA2  |  
 |                 | IO2            |  PA5  |  PA5  |  
 |                 | IO3            |  ---  |  PB3  |  
 |-----------------|----------------|-------|-------|
 |  Non Inverting  | IO1            |  PA1  |  PA3  |  
 |    Input        | IO2            |  ---  |  PB4  |
 |                 | IO3            |  ---  |  PB5  |  
 |                 | IO4            |  ---  |  PB6  |  
 |                 | IO5            |  ---  |  PB7  |  
 +--------------------------------------------------+  
  
 [..] Table 2. COMP Outputs for the STM32L0xx devices
 +---------------+     
 | COMP1 | COMP2 | 
 |-------|-------|
 |  PA0  |  PA2  | 
 |  PA6  |  PA7  |  
 |  PA11 |  PA12 | 
 +---------------+

    
            ##### How to use this driver #####
================================================================================
  [..]
      This driver provides functions to configure and program the Comparators of all STM32L0xx devices.

      To use the comparator, perform the following steps:
  
      (#)  Initialize the COMP low level resources by implementing the HAL_COMP_MspInit().
      (++) Configure the comparator input in analog mode using HAL_GPIO_Init().
      (++) Configure the comparator output in alternate function mode using HAL_GPIO_Init() to map the comparator 
           output to the GPIO pin.
      (++) If required enable the VREFINT reference using HAL_VREFINT_Cmd() and HAL_COMP_EnableBuffer_Cmd().
      (++) If required enable the COMP interrupt by configuring and enabling EXTI line in Interrupt mode and 
           selecting the desired sensitivity level using HAL_GPIO_Init() function. After that enable the comparator
           interrupt vector using HAL_NVIC_EnableIRQ() function.
  
      (#) Configure the comparator using HAL_COMP_Init() function:
      (++) Select the inverting input
      (++) Select the non-inverting input
      (++) Select the output polarity  
      (++) Select the power mode
      (++) Select the window mode
  
      (#) Enable the comparator using HAL_COMP_Start() function
    
  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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

#ifdef HAL_COMP_MODULE_ENABLED

/** @addtogroup COMP
  * @brief COMP HAL module driver
  * @{
  */

/** @addtogroup COMP_Private
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* CSR register reset value */ 
#define COMP_CSR_RESET_VALUE             ((uint32_t)0x00000000)

#define C_REV_ID_A              0x1000 //Cut1.0
#define C_REV_ID_Z              0x1008 //Cut1.1
#define C_REV_ID_Y              0x1003 //Cut1.2

#define C_DEV_ID_L073           0x447
#define C_DEV_ID_L053           0x417
/**
  * @}
  */


/** @addtogroup COMP_Exported_Functions
  * @{
  */

/** @addtogroup COMP_Exported_Functions_Group1
 *  @brief    Initialization and Configuration functions 
 *
@verbatim    
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]  This section provides functions to initialize and de-initialize comparators 

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the COMP according to the specified
  *         parameters in the COMP_InitTypeDef and create the associated handle.
  * @note   If the selected comparator is locked, initialization can't be performed.
  *         To unlock the configuration, perform a system reset.
  * @note   When the LPTIM connection is enabled, the following pins LPTIM_IN1(PB5, PC0)
            and LPTIM_IN2(PB7, PC2) should not be configured in AF. 
  * @param  hcomp: COMP handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_COMP_Init(COMP_HandleTypeDef *hcomp)
{
 HAL_StatusTypeDef status = HAL_OK;
  
  /* Check the COMP handle allocation and lock status */
  if((hcomp == NULL) || ((hcomp->State & COMP_STATE_BIT_LOCK) != 0x00))
  {
    status = HAL_ERROR;
  }
  else
  {
    /* Check the parameter */
    assert_param(IS_COMP_ALL_INSTANCE(hcomp->Instance));
    assert_param(IS_COMP_INVERTINGINPUT(hcomp->Init.InvertingInput));
    assert_param(IS_COMP_NONINVERTINGINPUT(hcomp->Init.NonInvertingInput));
    assert_param(IS_COMP_OUTPUTPOL(hcomp->Init.OutputPol));
    assert_param(IS_COMP_MODE(hcomp->Init.Mode));
    
    if(hcomp->Init.WindowMode != COMP_WINDOWMODE_DISABLE)
    {
      assert_param(IS_COMP_WINDOWMODE_INSTANCE(hcomp->Instance));
      assert_param(IS_COMP_WINDOWMODE(hcomp->Init.WindowMode));
    }
    
    if(hcomp->State == HAL_COMP_STATE_RESET)
    {
      /* Allocate lock resource and initialize it */
      hcomp->Lock = HAL_UNLOCKED;
      /* Init SYSCFG and the low level hardware to access comparators */
     __HAL_RCC_SYSCFG_CLK_ENABLE();
      /* Init the low level hardware : SYSCFG to access comparators */
      HAL_COMP_MspInit(hcomp);
    }
    
    /* Change COMP peripheral state */
    hcomp->State = HAL_COMP_STATE_BUSY;
  
    /* Set COMP parameters                                                              */
    /*     Set COMPxINSEL bits according to hcomp->Init.InvertingInput value            */
    /*     Set COMPxNONINSEL bits according to hcomp->Init.NonInvertingInput value      */
    /*     Set COMPxLPTIMCONNECTION bits according to hcomp->Init.LPTIMConnection value */
    /*     Set COMPxPOL bit according to hcomp->Init.OutputPol value                    */
    /*     Set COMPxMODE bits according to hcomp->Init.Mode value                       */
    /*     Set COMP1WM bit according to hcomp->Init.WindowMode value                    */

    /* No LPTIM connexion requested */
    if (hcomp->Init.LPTIMConnection == COMP_LPTIMCONNECTION_DISABLED)
    {
         MODIFY_REG(hcomp->Instance->CSR, COMP_CSR_UPDATE_PARAMETERS_MASK, \
                                       hcomp->Init.InvertingInput    |  \
                                       hcomp->Init.NonInvertingInput |  \
                                       hcomp->Init.OutputPol         |  \
                                       hcomp->Init.Mode              |  \
                                       hcomp->Init.WindowMode);
    }
    else
    {
      /* LPTIM connexion requested on COMP2*/
      if ((hcomp->Instance) == COMP2)
      {
        /* Check the MCU_ID in order to allow or not the COMP2 connection to LPTIM-input2 */
        if (((HAL_GetDEVID() == C_DEV_ID_L073) && (HAL_GetREVID() == C_REV_ID_A))
                          ||
            ((HAL_GetDEVID() == C_DEV_ID_L053) && (HAL_GetREVID() == C_REV_ID_A))
                          ||
            ((HAL_GetDEVID() == C_DEV_ID_L053) && (HAL_GetREVID() == C_REV_ID_Z)))
        {
          /* Note : COMP2 can be connected only to input 1 of LPTIM if requested */
          assert_param(IS_COMP2_LPTIMCONNECTION_RESTRICTED(hcomp->Init.LPTIMConnection));
          MODIFY_REG(hcomp->Instance->CSR, COMP_CSR_UPDATE_PARAMETERS_MASK, \
                                       hcomp->Init.InvertingInput    |  \
                                       hcomp->Init.NonInvertingInput |  \
                                       COMP_CSR_COMP2LPTIM1IN1       |  \
                                       hcomp->Init.OutputPol         |  \
                                       hcomp->Init.Mode              |  \
                                       hcomp->Init.WindowMode);
        }
        else
        {
           /* Note : COMP2 can be connected to input 1 or input2  of LPTIM if requested */
          assert_param(IS_COMP2_LPTIMCONNECTION(hcomp->Init.LPTIMConnection));
          switch (hcomp->Init.LPTIMConnection)
          {
          case  COMP_LPTIMCONNECTION_IN1_ENABLED :
              MODIFY_REG(hcomp->Instance->CSR, COMP_CSR_UPDATE_PARAMETERS_MASK, \
                                         hcomp->Init.InvertingInput    |  \
                                         hcomp->Init.NonInvertingInput |  \
                                         COMP_CSR_COMP2LPTIM1IN1       |  \
                                         hcomp->Init.OutputPol         |  \
                                         hcomp->Init.Mode              |  \
                                         hcomp->Init.WindowMode);
              break;
          case  COMP_LPTIMCONNECTION_IN2_ENABLED :
              MODIFY_REG(hcomp->Instance->CSR, COMP_CSR_UPDATE_PARAMETERS_MASK, \
                                         hcomp->Init.InvertingInput    |  \
                                         hcomp->Init.NonInvertingInput |  \
                                         COMP_CSR_COMP2LPTIM1IN2       |  \
                                         hcomp->Init.OutputPol         |  \
                                         hcomp->Init.Mode              |  \
                                         hcomp->Init.WindowMode);
              break;
          default :
              MODIFY_REG(hcomp->Instance->CSR, COMP_CSR_UPDATE_PARAMETERS_MASK, \
                                         hcomp->Init.InvertingInput    |  \
                                         hcomp->Init.NonInvertingInput |  \
                                         hcomp->Init.OutputPol         |  \
                                         hcomp->Init.Mode              |  \
                                         hcomp->Init.WindowMode);
              break;
          }
        }
      }
      else
      /* LPTIM connexion requested on COMP1 */
      {
        /* Note : COMP1 can be connected to the input 1 of LPTIM if requested */
        assert_param(IS_COMP1_LPTIMCONNECTION(hcomp->Init.LPTIMConnection));
        if (hcomp->Init.LPTIMConnection == COMP_LPTIMCONNECTION_IN1_ENABLED)
            MODIFY_REG(hcomp->Instance->CSR, COMP_CSR_UPDATE_PARAMETERS_MASK, \
                                         hcomp->Init.InvertingInput    |      \
                                         hcomp->Init.NonInvertingInput |      \
                                         COMP_CSR_COMP1LPTIM1IN1       |      \
                                         hcomp->Init.OutputPol         |      \
                                         hcomp->Init.Mode              |      \
                                         hcomp->Init.WindowMode);
        else
            MODIFY_REG(hcomp->Instance->CSR, COMP_CSR_UPDATE_PARAMETERS_MASK, \
                                         hcomp->Init.InvertingInput    |      \
                                         hcomp->Init.NonInvertingInput |      \
                                         hcomp->Init.OutputPol         |      \
                                         hcomp->Init.Mode              |      \
                                         hcomp->Init.WindowMode);
      }
    }
    /* Initialize the COMP state*/
    hcomp->State = HAL_COMP_STATE_READY;

  }
  
  return status;
}

/**
  * @brief  DeInitializes the COMP peripheral 
  * @note   Deinitialization can't be performed if the COMP configuration is locked.
  *         To unlock the configuration, perform a system reset.
  * @param  hcomp: COMP handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_COMP_DeInit(COMP_HandleTypeDef *hcomp)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Check the COMP handle allocation and lock status */
  if((hcomp == NULL) || ((hcomp->State & COMP_STATE_BIT_LOCK) != 0x00))
  {
    status = HAL_ERROR;
  }
  else
  {
    /* Check the parameter */
    assert_param(IS_COMP_ALL_INSTANCE(hcomp->Instance));

    /* Set COMP_CSR register to reset value */
    WRITE_REG(hcomp->Instance->CSR, COMP_CSR_RESET_VALUE);

    /* DeInit the low level hardware: SYSCFG, GPIO, CLOCK and NVIC */
    HAL_COMP_MspDeInit(hcomp);

    hcomp->State = HAL_COMP_STATE_RESET;
    
    /* Release Lock */
    __HAL_UNLOCK(hcomp);
  }
  
  return status;
}

/**
  * @brief  Initializes the COMP MSP.
  * @param  hcomp: COMP handle
  * @retval None
  */
__weak void HAL_COMP_MspInit(COMP_HandleTypeDef *hcomp)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcomp);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_COMP_MspInit could be implenetd in the user file
   */
}

/**
  * @brief  DeInitializes COMP MSP.
  * @param  hcomp: COMP handle
  * @retval None
  */
__weak void HAL_COMP_MspDeInit(COMP_HandleTypeDef *hcomp)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcomp);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_COMP_MspDeInit could be implenetd in the user file
   */
}

/**
  * @}
  */

/** @addtogroup COMP_Exported_Functions_Group2
 *  @brief   Data transfers functions 
 *
@verbatim   
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================  
    [..]
    This subsection provides a set of functions allowing to manage the COMP data 
    transfers.

@endverbatim
  * @{
  */

/**
  * @brief  Start the comparator 
  * @param  hcomp: COMP handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_COMP_Start(COMP_HandleTypeDef *hcomp)
{ 
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t extiline = 0;
  
  /* Check the COMP handle allocation and lock status */
  if((hcomp == NULL) || ((hcomp->State & COMP_STATE_BIT_LOCK) != 0x00))
  {
    status = HAL_ERROR;
  }
  else
  {
    /* Check the parameter */
    assert_param(IS_COMP_ALL_INSTANCE(hcomp->Instance));

    if(hcomp->State == HAL_COMP_STATE_READY)
    {
        /* Check the Exti Line output configuration */
        extiline = COMP_GET_EXTI_LINE(hcomp->Instance);

        /* Configure the rising edge */
        if((hcomp->Init.TriggerMode & COMP_TRIGGERMODE_EVENT_RISING) != 0x0)
        {
           if (extiline == COMP_EXTI_LINE_COMP1)
           {
             __HAL_COMP_COMP1_EXTI_ENABLE_RISING_EDGE();
           }
           else
           {
             __HAL_COMP_COMP2_EXTI_ENABLE_RISING_EDGE();
           }
        }
        else
        {
          if (extiline == COMP_EXTI_LINE_COMP1)
          {
            __HAL_COMP_COMP1_EXTI_DISABLE_RISING_EDGE();
          }
          else
          {
            __HAL_COMP_COMP2_EXTI_DISABLE_RISING_EDGE();
          }
        }

        /* Configure the falling edge */
        if((hcomp->Init.TriggerMode & COMP_TRIGGERMODE_EVENT_FALLING) != 0x0)
        {
          if (extiline == COMP_EXTI_LINE_COMP1)
          {
            __HAL_COMP_COMP1_EXTI_ENABLE_FALLING_EDGE();
          }
          else
          {
            __HAL_COMP_COMP2_EXTI_ENABLE_FALLING_EDGE();
          }
        }
        else
        {
          if (extiline == COMP_EXTI_LINE_COMP1)
          {
            __HAL_COMP_COMP1_EXTI_DISABLE_FALLING_EDGE();
          }
          else
          {
            __HAL_COMP_COMP2_EXTI_DISABLE_FALLING_EDGE();
          }
        }

        /* Configure the COMP module */
        if (extiline == COMP_EXTI_LINE_COMP1)
        {
          /* Clear COMP Exti pending bit */
          __HAL_COMP_COMP1_EXTI_CLEAR_FLAG();
        }
        else
        {
          /* Clear COMP Exti pending bit */
          __HAL_COMP_COMP2_EXTI_CLEAR_FLAG();
        }

      /* Enable the selected comparator */
      __HAL_COMP_ENABLE(hcomp);

      hcomp->State = HAL_COMP_STATE_BUSY;      
    }
    else
    {
      status = HAL_ERROR;
    }
  }

  return status;
}

/**
  * @brief  Stop the comparator 
  * @param  hcomp: COMP handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_COMP_Stop(COMP_HandleTypeDef *hcomp)
{ 
  HAL_StatusTypeDef status = HAL_OK;
  
  /* Check the COMP handle allocation and lock status */
  if((hcomp == NULL) || ((hcomp->State & COMP_STATE_BIT_LOCK) != 0x00))
  {
    status = HAL_ERROR;
  }
  else
  {
    /* Check the parameter */
    assert_param(IS_COMP_ALL_INSTANCE(hcomp->Instance));

    if(hcomp->State == HAL_COMP_STATE_BUSY)
    {
      /* Disable the selected comparator */
      __HAL_COMP_DISABLE(hcomp);

      hcomp->State = HAL_COMP_STATE_READY;
    }
    else
    {
      status = HAL_ERROR;
    }
  }
  
  return status;
}

/**
  * @brief  Enables the interrupt and starts the comparator
  * @param  hcomp: COMP handle
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_COMP_Start_IT(COMP_HandleTypeDef *hcomp)
{ 
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t extiline = 0;
  
  /* Check the COMP handle allocation and lock status */
  if((hcomp == NULL) || ((hcomp->State & COMP_STATE_BIT_LOCK) != 0x00))
  {
    status = HAL_ERROR;
  }
  else
  {
    /* Check the parameter */
    assert_param(IS_COMP_ALL_INSTANCE(hcomp->Instance));

    if(hcomp->State == HAL_COMP_STATE_READY)
    {
        /* Check the Exti Line output configuration */
        extiline = COMP_GET_EXTI_LINE(hcomp->Instance);

        /* Configure the rising edge */
        if((hcomp->Init.TriggerMode & COMP_TRIGGERMODE_IT_RISING) != 0x0)
        {
           if (extiline == COMP_EXTI_LINE_COMP1)
           {
             __HAL_COMP_COMP1_EXTI_ENABLE_RISING_EDGE();
           }
           else
           {
             __HAL_COMP_COMP2_EXTI_ENABLE_RISING_EDGE();
           }
        }
        else
        {
          if (extiline == COMP_EXTI_LINE_COMP1)
          {
            __HAL_COMP_COMP1_EXTI_DISABLE_RISING_EDGE();
          }
          else
          {
            __HAL_COMP_COMP2_EXTI_DISABLE_RISING_EDGE();
          }
        }

        /* Configure the falling edge */
        if((hcomp->Init.TriggerMode & COMP_TRIGGERMODE_IT_FALLING) != 0x0)
        {
          if (extiline == COMP_EXTI_LINE_COMP1)
          {
            __HAL_COMP_COMP1_EXTI_ENABLE_FALLING_EDGE();
          }
          else
          {
            __HAL_COMP_COMP2_EXTI_ENABLE_FALLING_EDGE();
          }
        }
        else
        {
          if (extiline == COMP_EXTI_LINE_COMP1)
          {
            __HAL_COMP_COMP1_EXTI_DISABLE_FALLING_EDGE();
          }
          else
          {
            __HAL_COMP_COMP2_EXTI_DISABLE_FALLING_EDGE();
          }
        }

        /* Configure the COMP module */
        if (extiline == COMP_EXTI_LINE_COMP1)
        {
          /* Clear COMP Exti pending bit */
          __HAL_COMP_COMP1_EXTI_CLEAR_FLAG();
          /* Enable Exti interrupt mode */
          __HAL_COMP_COMP1_EXTI_ENABLE_IT();
        }
        else
        {
          /* Clear COMP Exti pending bit */
          __HAL_COMP_COMP2_EXTI_CLEAR_FLAG();
          /* Enable Exti interrupt mode */
          __HAL_COMP_COMP2_EXTI_ENABLE_IT();
        }

      /* Enable the selected comparator */
      __HAL_COMP_ENABLE(hcomp);

      hcomp->State = HAL_COMP_STATE_BUSY;
    }
    else
    {
      status = HAL_ERROR;
    }
  }

  return status;
}

/**
  * @brief  Disable the interrupt and Stop the comparator 
  * @param  hcomp: COMP handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_COMP_Stop_IT(COMP_HandleTypeDef *hcomp)
{  
  HAL_StatusTypeDef status = HAL_OK;
  
  if (COMP_GET_EXTI_LINE(hcomp->Instance) == COMP_EXTI_LINE_COMP1)
  {
    __HAL_COMP_COMP1_EXTI_DISABLE_IT();
  }
  if (COMP_GET_EXTI_LINE(hcomp->Instance) == COMP_EXTI_LINE_COMP2)
  {
    __HAL_COMP_COMP2_EXTI_DISABLE_IT();
  }
  
  status = HAL_COMP_Stop(hcomp);
  
  return status;
}

/**
  * @brief  Comparator IRQ Handler 
  * @param  hcomp: COMP handle
  * @retval HAL status
  */
void HAL_COMP_IRQHandler(COMP_HandleTypeDef *hcomp)
{
  /* Check which exti line is involved */
  uint32_t extiline = COMP_GET_EXTI_LINE(hcomp->Instance);

  /* Manage COMP1 Exti line */
  if (extiline == COMP_EXTI_LINE_COMP1)
  {
    if(__HAL_COMP_COMP1_EXTI_GET_FLAG() != RESET)
    {
      /* Clear COMP Exti pending bit */
      __HAL_COMP_COMP1_EXTI_CLEAR_FLAG();
      /* COMP trigger user callback */
      HAL_COMP_TriggerCallback(hcomp);
    }
  }

  /* Manage COMP2 Exti line */
  if (extiline == COMP_EXTI_LINE_COMP2)
  {
    if(__HAL_COMP_COMP2_EXTI_GET_FLAG() != RESET)
    {
      /* Clear COMP Exti pending bit */
      __HAL_COMP_COMP2_EXTI_CLEAR_FLAG();
      /* COMP trigger user callback */
      HAL_COMP_TriggerCallback(hcomp);
    }
  }
}

/**
  * @}
  */

/** @addtogroup COMP_Exported_Functions_Group3
 *  @brief   management functions 
 *
@verbatim   
 ===============================================================================
                      ##### Peripheral Control functions #####
 ===============================================================================  
    [..]
    This subsection provides a set of functions allowing to control the COMP data 
    transfers.

@endverbatim
  * @{
  */

/**
  * @brief  Lock the selected comparator configuration. 
  * @param  hcomp: COMP handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_COMP_Lock(COMP_HandleTypeDef *hcomp)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Check the COMP handle allocation and lock status */
  if((hcomp == NULL) || ((hcomp->State & COMP_STATE_BIT_LOCK) != 0x00))
  {
    status = HAL_ERROR;
  }
  else
  {
        /* Check the parameter */
    assert_param(IS_COMP_ALL_INSTANCE(hcomp->Instance));

    /* Set lock flag on state */
    switch(hcomp->State)
    {
    case HAL_COMP_STATE_BUSY:
      hcomp->State = HAL_COMP_STATE_BUSY_LOCKED;
      break;
    case HAL_COMP_STATE_READY:
      hcomp->State = HAL_COMP_STATE_READY_LOCKED;
      break;
    default:
      /* unexpected state */
      status = HAL_ERROR;
      break;
    }
  }
  
  if(status == HAL_OK)
  {
    /* Set the lock bit corresponding to selected comparator */
    __HAL_COMP_LOCK(hcomp);
  }
  
  return status; 
}

/**
  * @brief  Return the output level (high or low) of the selected comparator. 
  *         The output level depends on the selected polarity.
  *         If the polarity is not inverted:
  *           - Comparator output is low when the non-inverting input is at a lower
  *             voltage than the inverting input
  *           - Comparator output is high when the non-inverting input is at a higher
  *             voltage than the inverting input
  *         If the polarity is inverted:
  *           - Comparator output is high when the non-inverting input is at a lower
  *             voltage than the inverting input
  *           - Comparator output is low when the non-inverting input is at a higher
  *             voltage than the inverting input
  * @param  hcomp: COMP handle
  * @retval Returns the selected comparator output level: COMP_OUTPUTLEVEL_LOW or COMP_OUTPUTLEVEL_HIGH.
  *       
  */
uint32_t HAL_COMP_GetOutputLevel(COMP_HandleTypeDef *hcomp)
{
  /* Check the parameter */
  assert_param(IS_COMP_ALL_INSTANCE(hcomp->Instance));
  
  return((uint32_t)(hcomp->Instance->CSR & COMP_OUTPUTLEVEL_HIGH));
}

/**
  * @brief  Comparator callback.
  * @param  hcomp: COMP handle
  * @retval None
  */
__weak void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcomp);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_COMP_TriggerCallback should be implemented in the user file
   */
}


/**
  * @}
  */

/** @addtogroup COMP_Exported_Functions_Group4
 *  @brief   Peripheral State functions 
 *
@verbatim   
 ===============================================================================
                      ##### Peripheral State functions #####
 ===============================================================================  
    [..]
    This subsection permit to get in run-time the status of the peripheral 
    and the data flow.

@endverbatim
  * @{
  */

/**
  * @brief  Return the COMP state
  * @param  hcomp : COMP handle
  * @retval HAL state
  */
HAL_COMP_StateTypeDef HAL_COMP_GetState(COMP_HandleTypeDef *hcomp)
{
  /* Check the COMP handle allocation */
  if(hcomp == NULL)
  {
    return HAL_COMP_STATE_RESET;
  }

  /* Check the parameter */
  assert_param(IS_COMP_ALL_INSTANCE(hcomp->Instance));

  return hcomp->State;
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

#endif /* HAL_COMP_MODULE_ENABLED */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

