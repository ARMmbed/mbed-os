/**
  ******************************************************************************
  * @file    stm32f0xx_hal_comp.c
  * @author  MCD Application Team
  * @brief   COMP HAL module driver.
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
      The STM32F0xx device family integrates up to 2 analog comparators COMP1 and COMP2:
      (+) The non inverting input and inverting input can be set to GPIO pins.
  
      (+) The COMP output is available using HAL_COMP_GetOutputLevel() 
          and can be set on GPIO pins.
  
      (+) The COMP output can be redirected to embedded timers (TIM1, TIM2 and TIM3).
  
      (+) The comparators COMP1 and COMP2 can be combined in window mode.
  
      (+) The comparators have interrupt capability with wake-up
          from Sleep and Stop modes (through the EXTI controller):
          (++) COMP1 is internally connected to EXTI Line 21
          (++) COMP2 is internally connected to EXTI Line 22

      (+) From the corresponding IRQ handler, the right interrupt source can be retrieved with the 
          macros __HAL_COMP_COMP1_EXTI_GET_FLAG() and __HAL_COMP_COMP2_EXTI_GET_FLAG().


            ##### How to use this driver #####
================================================================================
  [..]
      This driver provides functions to configure and program the Comparators of STM32F05x, STM32F07x and STM32F09x devices.

      To use the comparator, perform the following steps:
  
      (#) Fill in the HAL_COMP_MspInit() to
      (++) Configure the comparator input in analog mode using HAL_GPIO_Init()
      (++) Configure the comparator output in alternate function mode using HAL_GPIO_Init() to map the comparator 
           output to the GPIO pin
      (++) If required enable the COMP interrupt by configuring and enabling EXTI line in Interrupt mode and 
           selecting the desired sensitivity level using HAL_GPIO_Init() function. After that enable the comparator
           interrupt vector using HAL_NVIC_EnableIRQ() function.
  
      (#) Configure the comparator using HAL_COMP_Init() function:
      (++) Select the inverting input (input minus)
      (++) Select the non inverting input (input plus)
      (++) Select the output polarity  
      (++) Select the output redirection
      (++) Select the hysteresis level
      (++) Select the power mode
      (++) Select the event/interrupt mode
      (++) Select the window mode
  
      -@@- HAL_COMP_Init() calls internally __HAL_RCC_SYSCFG_CLK_ENABLE() in order
          to access the comparator(s) registers.

      (#) Enable the comparator using HAL_COMP_Start() function or HAL_COMP_Start_IT() function for interrupt mode.
    
      (#) Use HAL_COMP_TriggerCallback() and/or HAL_COMP_GetOutputLevel() functions
          to manage comparator outputs (event/interrupt triggered and output level).

      (#) Disable the comparator using HAL_COMP_Stop() or HAL_COMP_Stop_IT() 
          function.

      (#) De-initialize the comparator using HAL_COMP_DeInit() function.

      (#) For safety purposes comparator(s) can be locked using HAL_COMP_Lock() function.
          Only a MCU reset can reset that protection.

    *** Callback registration ***
    =============================================
    [..]

     The compilation flag USE_HAL_COMP_REGISTER_CALLBACKS, when set to 1,
     allows the user to configure dynamically the driver callbacks.
     Use Functions @ref HAL_COMP_RegisterCallback()
     to register an interrupt callback.
    [..]

     Function @ref HAL_COMP_RegisterCallback() allows to register following callbacks:
       (+) OperationCpltCallback : callback for End of operation.
       (+) ErrorCallback         : callback for error detection.
       (+) MspInitCallback       : callback for Msp Init.
       (+) MspDeInitCallback     : callback for Msp DeInit.
     This function takes as parameters the HAL peripheral handle, the Callback ID
     and a pointer to the user callback function.
    [..]

     Use function @ref HAL_COMP_UnRegisterCallback to reset a callback to the default
     weak function.
    [..]

     @ref HAL_COMP_UnRegisterCallback takes as parameters the HAL peripheral handle,
     and the Callback ID.
     This function allows to reset following callbacks:
       (+) OperationCpltCallback : callback for End of operation.
       (+) ErrorCallback         : callback for error detection.
       (+) MspInitCallback       : callback for Msp Init.
       (+) MspDeInitCallback     : callback for Msp DeInit.
     [..]

     By default, after the @ref HAL_COMP_Init() and when the state is @ref HAL_COMP_STATE_RESET
     all callbacks are set to the corresponding weak functions:
     examples @ref HAL_COMP_OperationCpltCallback(), @ref HAL_COMP_ErrorCallback().
     Exception done for MspInit and MspDeInit functions that are
     reset to the legacy weak functions in the @ref HAL_COMP_Init()/ @ref HAL_COMP_DeInit() only when
     these callbacks are null (not registered beforehand).
    [..]

     If MspInit or MspDeInit are not null, the @ref HAL_COMP_Init()/ @ref HAL_COMP_DeInit()
     keep and use the user MspInit/MspDeInit callbacks (registered beforehand) whatever the state.
     [..]

     Callbacks can be registered/unregistered in @ref HAL_COMP_STATE_READY state only.
     Exception done MspInit/MspDeInit functions that can be registered/unregistered
     in @ref HAL_COMP_STATE_READY or @ref HAL_COMP_STATE_RESET state,
     thus registered (user) MspInit/DeInit callbacks can be used during the Init/DeInit.
    [..]

     Then, the user first registers the MspInit/MspDeInit user callbacks
     using @ref HAL_COMP_RegisterCallback() before calling @ref HAL_COMP_DeInit()
     or @ref HAL_COMP_Init() function.
     [..]

     When the compilation flag USE_HAL_COMP_REGISTER_CALLBACKS is set to 0 or
     not defined, the callback registration feature is not available and all callbacks
     are set to the corresponding weak functions.

  @endverbatim
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

/*
    Additional Tables:

    Table 1. COMP Inputs for the STM32F05x, STM32F07x and STM32F09x devices
    +--------------------------------------------------+    
    |                 |                | COMP1 | COMP2 |
    |-----------------|----------------|---------------|
    |                 | 1/4 VREFINT    |  OK   |  OK   |
    |                 | 1/2 VREFINT    |  OK   |  OK   |
    |                 | 3/4 VREFINT    |  OK   |  OK   |
    | Inverting Input | VREFINT        |  OK   |  OK   |
    |                 | DAC1 OUT (PA4) |  OK   |  OK   |
    |                 | DAC2 OUT (PA5) |  OK   |  OK   |
    |                 | IO1            |  PA0  |  PA2  |
    |-----------------|----------------|-------|-------|
    |  Non Inverting  |                |  PA1  |  PA3  |
    |    Input        |                |       |       |
    +--------------------------------------------------+  

    Table 2. COMP Outputs for the STM32F05x, STM32F07x and STM32F09x devices
    +---------------+     
    | COMP1 | COMP2 |
    |-------|-------|
    |  PA0  |  PA2  |
    |  PA6  |  PA7  |
    |  PA11 |  PA12 |
    +---------------+

    Table 3. COMP Outputs redirection to embedded timers for the STM32F05x, STM32F07x and STM32F09x devices
    +---------------------------------+     
    |     COMP1      |     COMP2      |
    |----------------|----------------|
    |  TIM1 BKIN     |  TIM1 BKIN     |
    |                |                |
    |  TIM1 OCREFCLR |  TIM1 OCREFCLR |
    |                |                |
    |  TIM1 IC1      |  TIM1 IC1      |
    |                |                |
    |  TIM2 IC4      |  TIM2 IC4      |
    |                |                |
    |  TIM2 OCREFCLR |  TIM2 OCREFCLR |
    |                |                |
    |  TIM3 IC1      |  TIM3 IC1      |
    |                |                |
    |  TIM3 OCREFCLR |  TIM3 OCREFCLR |
    +---------------------------------+

*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

#ifdef HAL_COMP_MODULE_ENABLED

#if defined (COMP1) || defined (COMP2)

/** @addtogroup STM32F0xx_HAL_Driver
  * @{
  */

/** @defgroup COMP COMP
  * @brief COMP HAL module driver
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/** @defgroup COMP_Private_Constants COMP Private Constants
  * @{
  */

/* Delay for COMP startup time.                                               */
/* Note: Delay required to reach propagation delay specification.             */
/* Literal set to maximum value (refer to device datasheet,                   */
/* parameter "tSTART").                                                       */
/* Unit: us                                                                   */
#define COMP_DELAY_STARTUP_US           (60U)  /*!< Delay for COMP startup time */

/* CSR register reset value */ 
#define COMP_CSR_RESET_VALUE            (0x00000000U)
/* CSR register masks */ 
#define COMP_CSR_RESET_PARAMETERS_MASK   (0x00003FFFU)
#define COMP_CSR_UPDATE_PARAMETERS_MASK  (0x00003FFEU)
/* CSR COMPx non inverting input mask */ 
#define COMP_CSR_COMPxNONINSEL_MASK      ((uint16_t)COMP_CSR_COMP1SW1)
/* CSR COMP2 shift */ 
#define COMP_CSR_COMP1_SHIFT             0U
#define COMP_CSR_COMP2_SHIFT             16U
/**
  * @}
  */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup COMP_Exported_Functions COMP Exported Functions
  * @{
  */

/** @defgroup COMP_Exported_Functions_Group1 Initialization/de-initialization functions 
 *  @brief    Initialization and Configuration functions 
 *
@verbatim    
 ===============================================================================
              ##### Initialization and Configuration functions #####
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
  * @param  hcomp COMP handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_COMP_Init(COMP_HandleTypeDef *hcomp)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t regshift = COMP_CSR_COMP1_SHIFT;
  
  /* Check the COMP handle allocation and lock status */
  if((hcomp == NULL) || ((hcomp->State & COMP_STATE_BIT_LOCK) != RESET))
  {
    status = HAL_ERROR;
  }
  else
  {
    /* Check the parameter */
    assert_param(IS_COMP_ALL_INSTANCE(hcomp->Instance));
    assert_param(IS_COMP_INVERTINGINPUT(hcomp->Init.InvertingInput));
    assert_param(IS_COMP_NONINVERTINGINPUT(hcomp->Init.NonInvertingInput));
    assert_param(IS_COMP_OUTPUT(hcomp->Init.Output));
    assert_param(IS_COMP_OUTPUTPOL(hcomp->Init.OutputPol));
    assert_param(IS_COMP_HYSTERESIS(hcomp->Init.Hysteresis));
    assert_param(IS_COMP_MODE(hcomp->Init.Mode));
    
    if(hcomp->Init.NonInvertingInput == COMP_NONINVERTINGINPUT_DAC1SWITCHCLOSED)
    {
      assert_param(IS_COMP_DAC1SWITCH_INSTANCE(hcomp->Instance));
    }
  
    if(hcomp->Init.WindowMode != COMP_WINDOWMODE_DISABLE)
    {
      assert_param(IS_COMP_WINDOWMODE_INSTANCE(hcomp->Instance));
    }

    /* Init SYSCFG and the low level hardware to access comparators */
    __HAL_RCC_SYSCFG_CLK_ENABLE();  
    
#if (USE_HAL_COMP_REGISTER_CALLBACKS == 1)
    /* Init the COMP Callback settings */
    hcomp->TriggerCallback = HAL_COMP_TriggerCallback; /* Legacy weak callback */

    if (hcomp->MspInitCallback == NULL)
    {
      hcomp->MspInitCallback = HAL_COMP_MspInit; /* Legacy weak MspInit  */
    }

    /* Init the low level hardware */
    hcomp->MspInitCallback(hcomp);
#else
    /* Init the low level hardware : SYSCFG to access comparators */
    HAL_COMP_MspInit(hcomp);
#endif /* USE_HAL_COMP_REGISTER_CALLBACKS */

    if(hcomp->State == HAL_COMP_STATE_RESET)
    {
      /* Allocate lock resource and initialize it */
      hcomp->Lock = HAL_UNLOCKED;
    }

    /* Change COMP peripheral state */
    hcomp->State = HAL_COMP_STATE_BUSY;

    /* Set COMP parameters */
    /*     Set COMPxINSEL bits according to hcomp->Init.InvertingInput value        */
    /*     Set COMPxOUTSEL bits according to hcomp->Init.Output value               */
    /*     Set COMPxPOL bit according to hcomp->Init.OutputPol value                */
    /*     Set COMPxHYST bits according to hcomp->Init.Hysteresis value             */
    /*     Set COMPxMODE bits according to hcomp->Init.Mode value                   */
    if(hcomp->Instance == COMP2)
    {
      regshift = COMP_CSR_COMP2_SHIFT;
    }
    MODIFY_REG(COMP->CSR, 
               (COMP_CSR_COMPxINSEL  | COMP_CSR_COMPxNONINSEL_MASK | \
                COMP_CSR_COMPxOUTSEL | COMP_CSR_COMPxPOL           | \
                COMP_CSR_COMPxHYST   | COMP_CSR_COMPxMODE) << regshift,
               (hcomp->Init.InvertingInput    | \
                hcomp->Init.NonInvertingInput | \
                hcomp->Init.Output            | \
                hcomp->Init.OutputPol         | \
                hcomp->Init.Hysteresis        | \
                hcomp->Init.Mode) << regshift);   
    
    if(hcomp->Init.WindowMode != COMP_WINDOWMODE_DISABLE)
    {
      COMP->CSR |= COMP_CSR_WNDWEN;
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
  * @param  hcomp COMP handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_COMP_DeInit(COMP_HandleTypeDef *hcomp)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t regshift = COMP_CSR_COMP1_SHIFT;

  /* Check the COMP handle allocation and lock status */
  if((hcomp == NULL) || ((hcomp->State & COMP_STATE_BIT_LOCK) != RESET))
  {
    status = HAL_ERROR;
  }
  else
  {
    /* Check the parameter */
    assert_param(IS_COMP_ALL_INSTANCE(hcomp->Instance));

    /* Set COMP_CSR register to reset value for the corresponding COMP instance */
    if(hcomp->Instance == COMP2)
    {
      regshift = COMP_CSR_COMP2_SHIFT;
    }
    MODIFY_REG(COMP->CSR, 
               COMP_CSR_RESET_PARAMETERS_MASK << regshift, 
               COMP_CSR_RESET_VALUE << regshift);

#if (USE_HAL_COMP_REGISTER_CALLBACKS == 1)
    if (hcomp->MspDeInitCallback == NULL)
    {
      hcomp->MspDeInitCallback = HAL_COMP_MspDeInit; /* Legacy weak MspDeInit  */
    }

    /* DeInit the low level hardware: SYSCFG, GPIO, CLOCK and NVIC */
    hcomp->MspDeInitCallback(hcomp);
#else
    /* DeInit the low level hardware: SYSCFG, GPIO, CLOCK and NVIC */
    HAL_COMP_MspDeInit(hcomp);
#endif /* USE_HAL_COMP_REGISTER_CALLBACKS */

    hcomp->State = HAL_COMP_STATE_RESET;
    
    /* Release Lock */
    __HAL_UNLOCK(hcomp);
  }
  
  return status;
}

/**
  * @brief  Initializes the COMP MSP.
  * @param  hcomp COMP handle
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
  * @param  hcomp COMP handle
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

#if (USE_HAL_COMP_REGISTER_CALLBACKS == 1)
/**
  * @brief  Register a User COMP Callback
  *         To be used instead of the weak predefined callback
  * @param  hcomp Pointer to a COMP_HandleTypeDef structure that contains
  *                the configuration information for the specified COMP.
  * @param  CallbackID ID of the callback to be registered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_COMP_TRIGGER_CB_ID Trigger callback ID
  *          @arg @ref HAL_COMP_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_COMP_MSPDEINIT_CB_ID MspDeInit callback ID
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_COMP_RegisterCallback(COMP_HandleTypeDef *hcomp, HAL_COMP_CallbackIDTypeDef CallbackID, pCOMP_CallbackTypeDef pCallback)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (pCallback == NULL)
  {
    /* Update the error code */
    hcomp->ErrorCode |= HAL_COMP_ERROR_INVALID_CALLBACK;

    return HAL_ERROR;
  }

  if (HAL_COMP_STATE_READY == hcomp->State)
  {
    switch (CallbackID)
    {
    case HAL_COMP_TRIGGER_CB_ID :
      hcomp->TriggerCallback = pCallback;
      break;

    case HAL_COMP_MSPINIT_CB_ID :
      hcomp->MspInitCallback = pCallback;
      break;

    case HAL_COMP_MSPDEINIT_CB_ID :
      hcomp->MspDeInitCallback = pCallback;
      break;

    default :
      /* Update the error code */
      hcomp->ErrorCode |= HAL_COMP_ERROR_INVALID_CALLBACK;
      
      /* Return error status */
      status = HAL_ERROR;
      break;
    }
  }
  else if (HAL_COMP_STATE_RESET == hcomp->State)
  {
    switch (CallbackID)
    {
    case HAL_COMP_MSPINIT_CB_ID :
      hcomp->MspInitCallback = pCallback;
      break;

    case HAL_COMP_MSPDEINIT_CB_ID :
      hcomp->MspDeInitCallback = pCallback;
      break;

    default :
      /* Update the error code */
      hcomp->ErrorCode |= HAL_COMP_ERROR_INVALID_CALLBACK;

      /* Return error status */
      status = HAL_ERROR;
      break;
    }
  }
  else
  {
    /* Update the error code */
    hcomp->ErrorCode |= HAL_COMP_ERROR_INVALID_CALLBACK;

    /* Return error status */
    status =  HAL_ERROR;
  }

  return status;
}

/**
  * @brief  Unregister a COMP Callback
  *         COMP callback is redirected to the weak predefined callback
  * @param  hcomp Pointer to a COMP_HandleTypeDef structure that contains
  *                the configuration information for the specified COMP.
  * @param  CallbackID ID of the callback to be unregistered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_COMP_TRIGGER_CB_ID Trigger callback ID
  *          @arg @ref HAL_COMP_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_COMP_MSPDEINIT_CB_ID MspDeInit callback ID
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_COMP_UnRegisterCallback(COMP_HandleTypeDef *hcomp, HAL_COMP_CallbackIDTypeDef CallbackID)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (HAL_COMP_STATE_READY == hcomp->State)
  {
    switch (CallbackID)
    {
    case HAL_COMP_TRIGGER_CB_ID :
      hcomp->TriggerCallback = HAL_COMP_TriggerCallback;         /* Legacy weak callback */
      break;

    case HAL_COMP_MSPINIT_CB_ID :
      hcomp->MspInitCallback = HAL_COMP_MspInit;                 /* Legacy weak MspInit */
      break;

    case HAL_COMP_MSPDEINIT_CB_ID :
      hcomp->MspDeInitCallback = HAL_COMP_MspDeInit;             /* Legacy weak MspDeInit */
      break;

    default :
      /* Update the error code */
      hcomp->ErrorCode |= HAL_COMP_ERROR_INVALID_CALLBACK;

      /* Return error status */
      status =  HAL_ERROR;
      break;
    }
  }
  else if (HAL_COMP_STATE_RESET == hcomp->State)
  {
    switch (CallbackID)
    {
    case HAL_COMP_MSPINIT_CB_ID :
      hcomp->MspInitCallback = HAL_COMP_MspInit;                 /* Legacy weak MspInit */
      break;

    case HAL_COMP_MSPDEINIT_CB_ID :
      hcomp->MspDeInitCallback = HAL_COMP_MspDeInit;             /* Legacy weak MspDeInit */
      break;

    default :
      /* Update the error code */
      hcomp->ErrorCode |= HAL_COMP_ERROR_INVALID_CALLBACK;

      /* Return error status */
      status =  HAL_ERROR;
      break;
    }
  }
  else
  {
    /* Update the error code */
    hcomp->ErrorCode |= HAL_COMP_ERROR_INVALID_CALLBACK;
    
    /* Return error status */
    status =  HAL_ERROR;
  }

  return status;
}

#endif /* USE_HAL_COMP_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup COMP_Exported_Functions_Group2 I/O operation functions 
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
  * @param  hcomp COMP handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_COMP_Start(COMP_HandleTypeDef *hcomp)
{ 
  uint32_t wait_loop_index = 0U;
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t regshift = COMP_CSR_COMP1_SHIFT;
  
  /* Check the COMP handle allocation and lock status */
  if((hcomp == NULL) || ((hcomp->State & COMP_STATE_BIT_LOCK) != RESET))
  {
    status = HAL_ERROR;
  }
  else
  {
    /* Check the parameter */
    assert_param(IS_COMP_ALL_INSTANCE(hcomp->Instance));

    if(hcomp->State == HAL_COMP_STATE_READY)
    {
      /* Enable the selected comparator */
      if(hcomp->Instance == COMP2)
      {
        regshift = COMP_CSR_COMP2_SHIFT;
      }
      SET_BIT(COMP->CSR, COMP_CSR_COMPxEN << regshift);
      
      /* Set HAL COMP handle state */
      hcomp->State = HAL_COMP_STATE_BUSY;
      
      /* Delay for COMP startup time */
      wait_loop_index = (COMP_DELAY_STARTUP_US * (SystemCoreClock / 1000000U));
      while(wait_loop_index != 0U)
      {
        wait_loop_index--;
      }    
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
  * @param  hcomp COMP handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_COMP_Stop(COMP_HandleTypeDef *hcomp)
{ 
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t regshift = COMP_CSR_COMP1_SHIFT;
  
  /* Check the COMP handle allocation and lock status */
  if((hcomp == NULL) || ((hcomp->State & COMP_STATE_BIT_LOCK) != RESET))
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
      if(hcomp->Instance == COMP2)
      {
        regshift = COMP_CSR_COMP2_SHIFT;
      }
      CLEAR_BIT(COMP->CSR, COMP_CSR_COMPxEN << regshift);

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
  * @param  hcomp COMP handle
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_COMP_Start_IT(COMP_HandleTypeDef *hcomp)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t extiline = 0U;
  
  /* Check the parameter */
  assert_param(IS_COMP_TRIGGERMODE(hcomp->Init.TriggerMode));

  status = HAL_COMP_Start(hcomp);
  if(status == HAL_OK)
  {
    /* Check the Exti Line output configuration */
    extiline = COMP_GET_EXTI_LINE(hcomp->Instance);
    /* Configure the rising edge */
    if((hcomp->Init.TriggerMode & COMP_TRIGGERMODE_IT_RISING) != RESET)
    {
      SET_BIT(EXTI->RTSR, extiline);
    }
    else
    {
      CLEAR_BIT(EXTI->RTSR, extiline);
    }
    /* Configure the falling edge */
    if((hcomp->Init.TriggerMode & COMP_TRIGGERMODE_IT_FALLING) != RESET)
    {
      SET_BIT(EXTI->FTSR, extiline);
    }
    else
    {
      CLEAR_BIT(EXTI->FTSR, extiline);
    }

    /* Clear COMP EXTI pending bit */
    WRITE_REG(EXTI->PR, extiline);
    
    /* Enable Exti interrupt mode */
    SET_BIT(EXTI->IMR, extiline);
  }

  return status;
}

/**
  * @brief  Disable the interrupt and Stop the comparator 
  * @param  hcomp COMP handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_COMP_Stop_IT(COMP_HandleTypeDef *hcomp)
{ 
  HAL_StatusTypeDef status = HAL_OK;
  
  /* Disable the Exti Line interrupt mode */
  CLEAR_BIT(EXTI->IMR, COMP_GET_EXTI_LINE(hcomp->Instance));
  
  status = HAL_COMP_Stop(hcomp);
  
  return status;
}

/**
  * @brief  Comparator IRQ Handler 
  * @param  hcomp COMP handle
  * @retval HAL status
  */
void HAL_COMP_IRQHandler(COMP_HandleTypeDef *hcomp)
{
  uint32_t extiline = COMP_GET_EXTI_LINE(hcomp->Instance);
  
  /* Check COMP Exti flag */
  if(READ_BIT(EXTI->PR, extiline) != RESET)
  {
    /* Clear COMP Exti pending bit */
    WRITE_REG(EXTI->PR, extiline);

    /* COMP trigger callback */
#if (USE_HAL_COMP_REGISTER_CALLBACKS == 1)
    hcomp->TriggerCallback(hcomp);
#else
    HAL_COMP_TriggerCallback(hcomp);
#endif /* USE_HAL_COMP_REGISTER_CALLBACKS */  
  }
}

/**
  * @}
  */

/** @defgroup COMP_Exported_Functions_Group3 Peripheral Control functions 
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
  * @param  hcomp COMP handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_COMP_Lock(COMP_HandleTypeDef *hcomp)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t regshift = COMP_CSR_COMP1_SHIFT;

  /* Check the COMP handle allocation and lock status */
  if((hcomp == NULL) || ((hcomp->State & COMP_STATE_BIT_LOCK) != RESET))
  {
    status = HAL_ERROR;
  }
  else
  {
    /* Check the parameter */
    assert_param(IS_COMP_ALL_INSTANCE(hcomp->Instance));

    /* Set lock flag */
    hcomp->State |= COMP_STATE_BIT_LOCK;

    /* Set the lock bit corresponding to selected comparator */
    if(hcomp->Instance == COMP2)
    {
      regshift = COMP_CSR_COMP2_SHIFT;
    }
    SET_BIT(COMP->CSR, COMP_CSR_COMPxLOCK << regshift);
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
  * @param  hcomp COMP handle
  * @retval Returns the selected comparator output level: COMP_OUTPUTLEVEL_LOW or COMP_OUTPUTLEVEL_HIGH.
  *       
  */
uint32_t HAL_COMP_GetOutputLevel(COMP_HandleTypeDef *hcomp)
{
  uint32_t level=0;
  uint32_t regshift = COMP_CSR_COMP1_SHIFT;
  
  /* Check the parameter */
  assert_param(IS_COMP_ALL_INSTANCE(hcomp->Instance));
  
  if(hcomp->Instance == COMP2)
  {
    regshift = COMP_CSR_COMP2_SHIFT;
  }
  level = READ_BIT(COMP->CSR, COMP_CSR_COMPxOUT << regshift);
  
  if(level != 0U)
  {
    return(COMP_OUTPUTLEVEL_HIGH);
  }
  return(COMP_OUTPUTLEVEL_LOW);
}

/**
  * @brief  Comparator trigger callback.
  * @param  hcomp COMP handle
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

/** @defgroup COMP_Exported_Functions_Group4 Peripheral State functions 
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
  * @param  hcomp COMP handle
  * @retval HAL state
  */
uint32_t HAL_COMP_GetState(COMP_HandleTypeDef *hcomp)
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
  * @brief  Return the COMP error code.
  * @param hcomp COMP handle
  * @retval COMP error code
  */
uint32_t HAL_COMP_GetError(COMP_HandleTypeDef *hcomp)
{
  /* Check the parameters */
  assert_param(IS_COMP_ALL_INSTANCE(hcomp->Instance));

  return hcomp->ErrorCode;
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

/**
  * @}
  */

#endif /* COMP1 || COMP2 */

#endif /* HAL_COMP_MODULE_ENABLED */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
