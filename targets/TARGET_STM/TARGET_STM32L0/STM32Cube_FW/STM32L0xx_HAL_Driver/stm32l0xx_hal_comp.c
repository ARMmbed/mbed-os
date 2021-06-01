/**
  ******************************************************************************
  * @file    stm32l0xx_hal_comp.c
  * @author  MCD Application Team
  * @brief   COMP HAL module driver.
  *          This file provides firmware functions to manage the following 
  *          functionalities of the COMP peripheral:
  *           + Initialization and de-initialization functions
  *           + Start/Stop operation functions in polling mode
  *           + Start/Stop operation functions in interrupt mode (through EXTI interrupt)
  *           + Peripheral control functions
  *           + Peripheral state functions
  *         
  @verbatim
================================================================================
          ##### COMP Peripheral features #####
================================================================================
           
  [..]       
      The STM32L0xx device family integrates two analog comparators instances
      COMP1 and COMP2:
      (#) The COMP input minus (inverting input) and input plus (non inverting input)
          can be set to internal references or to GPIO pins
          (refer to GPIO list in reference manual).
  
      (#) The COMP output level is available using HAL_COMP_GetOutputLevel()
          and can be redirected to other peripherals: GPIO pins (in mode
          alternate functions for comparator), timers.
          (refer to GPIO list in reference manual).
  
      (#) Pairs of comparators instances can be combined in window mode
          (2 consecutive instances odd and even COMP<x> and COMP<x+1>).
  
      (#) The comparators have interrupt capability through the EXTI controller
          with wake-up from sleep and stop modes:
          (++) COMP1 is internally connected to EXTI Line 21
          (++) COMP2 is internally connected to EXTI Line 22

          From the corresponding IRQ handler, the right interrupt source can be retrieved
          using macro __HAL_COMP_COMP1_EXTI_GET_FLAG() and __HAL_COMP_COMP2_EXTI_GET_FLAG().

            ##### How to use this driver #####
================================================================================
  [..]
      This driver provides functions to configure and program the comparator instances
      of STM32L0xx devices.

      To use the comparator, perform the following steps:
      
      (#)  Initialize the COMP low level resources by implementing the HAL_COMP_MspInit():
      (++) Configure the GPIO connected to comparator inputs plus and minus in analog mode
           using HAL_GPIO_Init().
      (++) If needed, configure the GPIO connected to comparator output in alternate function mode
           using HAL_GPIO_Init().
      (++) If required enable the COMP interrupt by configuring and enabling EXTI line in Interrupt mode and 
           selecting the desired sensitivity level using HAL_GPIO_Init() function. After that enable the comparator
           interrupt vector using HAL_NVIC_EnableIRQ() function.
      
      (#) Configure the comparator using HAL_COMP_Init() function:
      (++) Select the input minus (inverting input)
      (++) Select the input plus (non-inverting input)
      (++) Select the output polarity  
      (++) Select the power mode
      (++) Select the window mode
      
      -@@- HAL_COMP_Init() calls internally __HAL_RCC_SYSCFG_CLK_ENABLE()
          to enable internal control clock of the comparators.
          However, this is a legacy strategy. In future STM32 families,
          COMP clock enable must be implemented by user in "HAL_COMP_MspInit()".
          Therefore, for compatibility anticipation, it is recommended to 
          implement __HAL_RCC_SYSCFG_CLK_ENABLE() in "HAL_COMP_MspInit()".
      
      (#) Reconfiguration on-the-fly of comparator can be done by calling again
          function HAL_COMP_Init() with new input structure parameters values.
      
      (#) Enable the comparator using HAL_COMP_Start() function.
      
      (#) Use HAL_COMP_TriggerCallback() or HAL_COMP_GetOutputLevel() functions
          to manage comparator outputs (events and output level).
      
      (#) Disable the comparator using HAL_COMP_Stop() function.
      
      (#) De-initialize the comparator using HAL_COMP_DeInit() function.
      
      (#) For safety purpose, comparator configuration can be locked using HAL_COMP_Lock() function.
          The only way to unlock the comparator is a device hardware reset.
  
    *** Callback registration ***
    =============================================
    [..]

     The compilation flag USE_HAL_COMP_REGISTER_CALLBACKS, when set to 1,
     allows the user to configure dynamically the driver callbacks.
     Use Functions @ref HAL_COMP_RegisterCallback()
     to register an interrupt callback.
    [..]

     Function @ref HAL_COMP_RegisterCallback() allows to register following callbacks:
       (+) TriggerCallback       : callback for COMP trigger.
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
       (+) TriggerCallback       : callback for COMP trigger.
       (+) MspInitCallback       : callback for Msp Init.
       (+) MspDeInitCallback     : callback for Msp DeInit.
     [..]

     By default, after the @ref HAL_COMP_Init() and when the state is @ref HAL_COMP_STATE_RESET
     all callbacks are set to the corresponding weak functions:
     example @ref HAL_COMP_TriggerCallback().
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

  Table 1. COMP inputs and output for STM32L0xx devices
  +---------------------------------------------------------+
  |                |                |   COMP1   |   COMP2   |
  |----------------|----------------|-----------|-----------|
  |                | IO1            |    PA1    |    PA3    |
  | Input plus     | IO2            |    ---    |    PA4    |
  |                | IO3            |    ---    |    PB5    |
  |                | IO4            |    ---    |    PB6    |
  |                | IO5            |    ---    |    PB7    |
  |----------------|----------------|-----------------------|
  |                | 1/4 VrefInt    |    ---    | Available |
  |                | 1/2 VrefInt    |    ---    | Available |
  |                | 3/4 VrefInt    |    ---    | Available |
  | Input minus    | VrefInt        | Available | Available |
  |                | DAC1 channel 1 | Available | Available |
  |                | DAC1 channel 2 | Available | Available |
  |                | IO1            |    PA0    |    PA2    |
  |                | IO2            |    PA5    |    PA5    |
  |                | IO3            |    ---    |    PB3    |
  +---------------------------------------------------------+
  | Output         |                |  PA0  (1) |  PA2  (1) |
  |                |                |  PA6  (1) |  PA7  (1) |
  |                |                |  PA11 (1) |  PA12 (1) |
  |                |                |  LPTIM    |  LPTIM    |
  |                |                |  TIM  (2) |  TIM  (2) |
  +-----------------------------------------------------------+
  (1) GPIO must be set to alternate function for comparator
  (2) Comparators output to timers is set in timers instances.

  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright(c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************  
  */

#if !defined (STM32L010xB) && !defined (STM32L010x8) && !defined (STM32L010x6) && !defined (STM32L010x4)
/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/** @addtogroup STM32L0xx_HAL_Driver
  * @{
  */

/** @defgroup COMP COMP
  * @brief COMP HAL module driver
  * @{
  */

#ifdef HAL_COMP_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup COMP_Private_Constants
  * @{
  */

/* Delay for COMP startup time.                                               */
/* Note: Delay required to reach propagation delay specification.             */
/* Literal set to maximum value (refer to device datasheet,                   */
/* parameter "tSTART").                                                       */
/* Unit: us                                                                   */
#define COMP_DELAY_STARTUP_US             (25U)  /*!< Delay for COMP startup time */

/* Delay for COMP voltage scaler stabilization time (voltage from VrefInt,    */
/* delay based on VrefInt startup time).                                      */
/* Literal set to maximum value (refer to device datasheet,                   */
/* parameter "TVREFINT").                                                     */
/* Unit: us                                                                   */
#define COMP_DELAY_VOLTAGE_SCALER_STAB_US (3000U)  /*!< Delay for COMP voltage scaler stabilization time */

#define COMP_OUTPUT_LEVEL_BITOFFSET_POS  (30U)

#define C_REV_ID_A              0x1000U /* Cut1.0 */
#define C_REV_ID_Z              0x1008U /* Cut1.1 */
#define C_REV_ID_Y              0x1003U /* Cut1.2 */

#define C_DEV_ID_L073           0x447U
#define C_DEV_ID_L053           0x417U

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup COMP_Exported_Functions COMP Exported Functions
  * @{
  */

/** @defgroup COMP_Exported_Functions_Group1 Initialization/de-initialization functions 
 *  @brief    Initialization and de-initialization functions. 
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
  * @brief  Initialize the COMP according to the specified
  *         parameters in the COMP_InitTypeDef and initialize the associated handle.
  * @note   If the selected comparator is locked, initialization can't be performed.
  *         To unlock the configuration, perform a system reset.
  * @note   When the LPTIM connection is enabled, the following pins LPTIM_IN1(PB5, PC0)
            and LPTIM_IN2(PB7, PC2) should not be configured in alternate function.
  * @param  hcomp  COMP handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_COMP_Init(COMP_HandleTypeDef *hcomp)
{
  uint32_t tmp_csr = 0U;
  uint32_t exti_line = 0U;
  uint32_t comp_voltage_scaler_not_initialized = 0U;
  __IO uint32_t wait_loop_index = 0U;
  HAL_StatusTypeDef status = HAL_OK;
  
  /* Check the COMP handle allocation and lock status */
  if((hcomp == NULL) || (__HAL_COMP_IS_LOCKED(hcomp)))
  {
    status = HAL_ERROR;
  }
  else
  {
    /* Check the parameters */
    assert_param(IS_COMP_ALL_INSTANCE(hcomp->Instance));
    assert_param(IS_COMP_INPUT_PLUS(hcomp->Instance, hcomp->Init.NonInvertingInput));
    assert_param(IS_COMP_INPUT_MINUS(hcomp->Instance, hcomp->Init.InvertingInput));
    assert_param(IS_COMP_OUTPUTPOL(hcomp->Init.OutputPol));
    assert_param(IS_COMP_POWERMODE(hcomp->Init.Mode));
    assert_param(IS_COMP_TRIGGERMODE(hcomp->Init.TriggerMode));
    assert_param(IS_COMP_WINDOWMODE(hcomp->Init.WindowMode));
    
    if(hcomp->State == HAL_COMP_STATE_RESET)
    {
      /* Allocate lock resource and initialize it */
      hcomp->Lock = HAL_UNLOCKED;
      
      /* Init SYSCFG and the low level hardware to access comparators */
      /* Note: HAL_COMP_Init() calls __HAL_RCC_SYSCFG_CLK_ENABLE()            */
      /*       to enable internal control clock of the comparators.           */
      /*       However, this is a legacy strategy. In future STM32 families,  */
      /*       COMP clock enable must be implemented by user                  */
      /*       in "HAL_COMP_MspInit()".                                       */
      /*       Therefore, for compatibility anticipation, it is recommended   */
      /*       to implement __HAL_RCC_SYSCFG_CLK_ENABLE()                     */
      /*       in "HAL_COMP_MspInit()".                                       */
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
      /* Init the low level hardware */
      HAL_COMP_MspInit(hcomp);
#endif /* USE_HAL_COMP_REGISTER_CALLBACKS */
    }
    
    /* Set COMP parameters */
    tmp_csr = (hcomp->Init.InvertingInput   |
               hcomp->Init.OutputPol         );
    
    /* Configuration specific to comparator instance: COMP2 */
    if ((hcomp->Instance) == COMP2)
    {
      /* Comparator input plus configuration is available on COMP2 only */
      /* Comparator power mode configuration is available on COMP2 only */
      tmp_csr |= (hcomp->Init.NonInvertingInput |
                  hcomp->Init.Mode               );
      
      /* COMP2 specificity: when using VrefInt or subdivision of VrefInt,     */
      /* specific path must be enabled.                                       */
      if((hcomp->Init.InvertingInput == COMP_INPUT_MINUS_VREFINT)    ||
         (hcomp->Init.InvertingInput == COMP_INPUT_MINUS_1_4VREFINT) ||
         (hcomp->Init.InvertingInput == COMP_INPUT_MINUS_1_2VREFINT) ||
         (hcomp->Init.InvertingInput == COMP_INPUT_MINUS_3_4VREFINT)   )
      {
        /* Memorize voltage scaler state before initialization */
        comp_voltage_scaler_not_initialized = (READ_BIT(SYSCFG->CFGR3, SYSCFG_CFGR3_ENBUFLP_VREFINT_COMP) == 0U);
        
        SET_BIT(SYSCFG->CFGR3, SYSCFG_CFGR3_EN_VREFINT           |
                               SYSCFG_CFGR3_ENBUFLP_VREFINT_COMP  );
        
        /* Delay for COMP scaler bridge voltage stabilization */
        /* Apply the delay if voltage scaler bridge is enabled for the first time */
        if (comp_voltage_scaler_not_initialized != 0U)
        {
          /* Wait loop initialization and execution */
          /* Note: Variable divided by 2 to compensate partially              */
          /*       CPU processing cycles.                                     */
          wait_loop_index = (COMP_DELAY_VOLTAGE_SCALER_STAB_US * (SystemCoreClock / (1000000U * 2U)));
          while(wait_loop_index != 0U)
          {
            wait_loop_index--;
          }
        }
      }
    }
    
    /* Set comparator output connection to LPTIM */
    if (hcomp->Init.LPTIMConnection != COMP_LPTIMCONNECTION_DISABLED)
    {
      /* LPTIM connexion requested on COMP1 */
      if ((hcomp->Instance) == COMP1)
      {
        /* Note : COMP1 can be connected to the input 1 of LPTIM if requested */
        assert_param(IS_COMP1_LPTIMCONNECTION(hcomp->Init.LPTIMConnection));
        
        /* Note: Compatibility with previous driver version using             */
        /* generic literal COMP_LPTIMCONNECTION_ENABLED corresponding         */
        /* to LPTIM input 1 for COMP1.                                        */
          tmp_csr |= (COMP_CSR_COMP1LPTIM1IN1);
        }
      else
      {
        /* Note : COMP2 can be connected to input 1 or input 2 of LPTIM if requested */
        assert_param(IS_COMP2_LPTIMCONNECTION(hcomp->Init.LPTIMConnection));
        
        switch (hcomp->Init.LPTIMConnection)
        {
        case  COMP_LPTIMCONNECTION_IN1_ENABLED :
          tmp_csr |= (COMP_CSR_COMP2LPTIM1IN1);
          break;
        case  COMP_LPTIMCONNECTION_IN2_ENABLED :
        default :
          /* Note: Default case for compatibility with previous driver version*/
          /* using generic literal COMP_LPTIMCONNECTION_ENABLED corresponding */
          /* to LPTIM input 2 for COMP2.                                      */
          
          /* Check the MCU_ID in order to allow or not the COMP2 connection to LPTIM input 2 */
          if (((HAL_GetDEVID() == C_DEV_ID_L073) && (HAL_GetREVID() == C_REV_ID_A))
                            ||
              ((HAL_GetDEVID() == C_DEV_ID_L053) && (HAL_GetREVID() == C_REV_ID_A))
                            ||
              ((HAL_GetDEVID() == C_DEV_ID_L053) && (HAL_GetREVID() == C_REV_ID_Z)))
          {
            assert_param(IS_COMP2_LPTIMCONNECTION_RESTRICTED(hcomp->Init.LPTIMConnection));
            
            /* Error: On the selected device, COMP2 cannot be connected to LPTIM input 2 */
            status = HAL_ERROR;
          }
          else
          {
            tmp_csr |= (COMP_CSR_COMP2LPTIM1IN2);
          }
          break;
        }
      }
    }
    
    /* Update comparator register */
    if ((hcomp->Instance) == COMP1)
    {
      MODIFY_REG(hcomp->Instance->CSR,
                 COMP_CSR_COMP1INNSEL     | COMP_CSR_COMP1WM       |
                 COMP_CSR_COMP1LPTIM1IN1  | COMP_CSR_COMP1POLARITY  ,
                 tmp_csr
                );
    }
    else /* Instance == COMP2 */
    {
      MODIFY_REG(hcomp->Instance->CSR,
                 COMP_CSR_COMP2SPEED     | COMP_CSR_COMP2INNSEL    |
                 COMP_CSR_COMP2INPSEL    | COMP_CSR_COMP2POLARITY  |
                 COMP_CSR_COMP2LPTIM1IN2 | COMP_CSR_COMP2LPTIM1IN1  ,
                 tmp_csr
                );
    }
    
    /* Set window mode */
    /* Note: Window mode bit is located into 1 out of the 2 pairs of COMP     */
    /*       instances. Therefore, this function can update another COMP      */
    /*       instance that the one currently selected.                        */
    if(hcomp->Init.WindowMode == COMP_WINDOWMODE_COMP1_INPUT_PLUS_COMMON)
    {
      SET_BIT(COMP12_COMMON->CSR, COMP_CSR_WINMODE);
    }
    else
    {
      CLEAR_BIT(COMP12_COMMON->CSR, COMP_CSR_WINMODE);
    }
    
    /* Get the EXTI line corresponding to the selected COMP instance */
    exti_line = COMP_GET_EXTI_LINE(hcomp->Instance);
    
    /* Manage EXTI settings */
    if((hcomp->Init.TriggerMode & (COMP_EXTI_IT | COMP_EXTI_EVENT)) != RESET)
    {
      /* Configure EXTI rising edge */
      if((hcomp->Init.TriggerMode & COMP_EXTI_RISING) != RESET)
      {
        SET_BIT(EXTI->RTSR, exti_line);
      }
      else
      {
        CLEAR_BIT(EXTI->RTSR, exti_line);
      }
      
      /* Configure EXTI falling edge */
      if((hcomp->Init.TriggerMode & COMP_EXTI_FALLING) != RESET)
      {
        SET_BIT(EXTI->FTSR, exti_line);
      }
      else
      {
        CLEAR_BIT(EXTI->FTSR, exti_line);
      }
      
      /* Clear COMP EXTI pending bit (if any) */
      WRITE_REG(EXTI->PR, exti_line);
      
      /* Configure EXTI event mode */
      if((hcomp->Init.TriggerMode & COMP_EXTI_EVENT) != RESET)
      {
        SET_BIT(EXTI->EMR, exti_line);
      }
      else
      {
        CLEAR_BIT(EXTI->EMR, exti_line);
      }
      
      /* Configure EXTI interrupt mode */
      if((hcomp->Init.TriggerMode & COMP_EXTI_IT) != RESET)
      {
        SET_BIT(EXTI->IMR, exti_line);
      }
      else
      {
        CLEAR_BIT(EXTI->IMR, exti_line);
      }
    }
    else
    {
      /* Disable EXTI event mode */
      CLEAR_BIT(EXTI->EMR, exti_line);
      
      /* Disable EXTI interrupt mode */
      CLEAR_BIT(EXTI->IMR, exti_line);
    }
    
    /* Set HAL COMP handle state */
    /* Note: Transition from state reset to state ready,                      */
    /*       otherwise (coming from state ready or busy) no state update.     */
    if (hcomp->State == HAL_COMP_STATE_RESET)
    {
      hcomp->State = HAL_COMP_STATE_READY;
    }
  }
  
  return status;
}

/**
  * @brief  DeInitialize the COMP peripheral.
  * @note   Deinitialization cannot be performed if the COMP configuration is locked.
  *         To unlock the configuration, perform a system reset.
  * @param  hcomp  COMP handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_COMP_DeInit(COMP_HandleTypeDef *hcomp)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  /* Check the COMP handle allocation and lock status */
  if((hcomp == NULL) || (__HAL_COMP_IS_LOCKED(hcomp)))
  {
    status = HAL_ERROR;
  }
  else
  {
    /* Check the parameter */
    assert_param(IS_COMP_ALL_INSTANCE(hcomp->Instance));
    
    /* Set COMP_CSR register to reset value */
    WRITE_REG(hcomp->Instance->CSR, 0x00000000U);
    
#if (USE_HAL_COMP_REGISTER_CALLBACKS == 1)
    if (hcomp->MspDeInitCallback == NULL)
    {
      hcomp->MspDeInitCallback = HAL_COMP_MspDeInit; /* Legacy weak MspDeInit  */
    }
    
    /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
    hcomp->MspDeInitCallback(hcomp);
#else
    /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
    HAL_COMP_MspDeInit(hcomp);
#endif /* USE_HAL_COMP_REGISTER_CALLBACKS */
    
    /* Set HAL COMP handle state */
    hcomp->State = HAL_COMP_STATE_RESET;
    
    /* Release Lock */
    __HAL_UNLOCK(hcomp);
  }
  
  return status;
}

/**
  * @brief  Initialize the COMP MSP.
  * @param  hcomp  COMP handle
  * @retval None
  */
__weak void HAL_COMP_MspInit(COMP_HandleTypeDef *hcomp)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcomp);
  
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_COMP_MspInit could be implemented in the user file
   */
}

/**
  * @brief  DeInitialize the COMP MSP.
  * @param  hcomp  COMP handle
  * @retval None
  */
__weak void HAL_COMP_MspDeInit(COMP_HandleTypeDef *hcomp)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcomp);
  
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_COMP_MspDeInit could be implemented in the user file
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

/** @defgroup COMP_Exported_Functions_Group2 Start-Stop operation functions 
 *  @brief   Start-Stop operation functions. 
 *
@verbatim   
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================  
    [..]  This section provides functions allowing to:
      (+) Start a comparator instance.
      (+) Stop a comparator instance.

@endverbatim
  * @{
  */

/**
  * @brief  Start the comparator.
  * @param  hcomp  COMP handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_COMP_Start(COMP_HandleTypeDef *hcomp)
{
  __IO uint32_t wait_loop_index = 0U;
  HAL_StatusTypeDef status = HAL_OK;
  
  /* Check the COMP handle allocation and lock status */
  if((hcomp == NULL) || (__HAL_COMP_IS_LOCKED(hcomp)))
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
      SET_BIT(hcomp->Instance->CSR, COMP_CSR_COMPxEN);
      
      /* Set HAL COMP handle state */
      hcomp->State = HAL_COMP_STATE_BUSY;
      
      /* Delay for COMP startup time */
      /* Wait loop initialization and execution */
      /* Note: Variable divided by 2 to compensate partially                  */
      /*       CPU processing cycles.                                         */
      wait_loop_index = (COMP_DELAY_STARTUP_US * (SystemCoreClock / (1000000U * 2U)));
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
  * @brief  Stop the comparator.
  * @param  hcomp  COMP handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_COMP_Stop(COMP_HandleTypeDef *hcomp)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  /* Check the COMP handle allocation and lock status */
  if((hcomp == NULL) || (__HAL_COMP_IS_LOCKED(hcomp)))
  {
    status = HAL_ERROR;
  }
  else
  {
    /* Check the parameter */
    assert_param(IS_COMP_ALL_INSTANCE(hcomp->Instance));
    
    if((hcomp->State == HAL_COMP_STATE_BUSY)  ||
       (hcomp->State == HAL_COMP_STATE_READY)   )
    {
      /* Disable the selected comparator */
      CLEAR_BIT(hcomp->Instance->CSR, COMP_CSR_COMPxEN);

      /* Set HAL COMP handle state */
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
  * @brief  Comparator IRQ handler.
  * @param  hcomp  COMP handle
  * @retval None
  */
void HAL_COMP_IRQHandler(COMP_HandleTypeDef *hcomp)
{
  /* Get the EXTI line corresponding to the selected COMP instance */
  uint32_t exti_line = COMP_GET_EXTI_LINE(hcomp->Instance);
  
  /* Check COMP EXTI flag */
  if(READ_BIT(EXTI->PR, exti_line) != RESET)
  {
    /* Check whether comparator is in independent or window mode */
    if(READ_BIT(COMP12_COMMON->CSR, COMP_CSR_WINMODE) != 0)
    {
      /* Clear COMP EXTI line pending bit of the pair of comparators          */
      /* in window mode.                                                      */
      /* Note: Pair of comparators in window mode can both trig IRQ when      */
      /*       input voltage is changing from "out of window" area            */
      /*       (low or high ) to the other "out of window" area (high or low).*/
      /*       Both flags must be cleared to call comparator trigger          */
      /*       callback is called once.                                       */
      WRITE_REG(EXTI->PR, (COMP_EXTI_LINE_COMP1 | COMP_EXTI_LINE_COMP2));
    }
    else
    {
      /* Clear COMP EXTI line pending bit */
      WRITE_REG(EXTI->PR, exti_line);
    }
    
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
 *  @brief   Management functions.
 *
@verbatim   
 ===============================================================================
                      ##### Peripheral Control functions #####
 ===============================================================================  
    [..]
    This subsection provides a set of functions allowing to control the comparators. 

@endverbatim
  * @{
  */

/**
  * @brief  Lock the selected comparator configuration.
  * @note   A system reset is required to unlock the comparator configuration.
  * @note   Locking the comparator from reset state is possible
  *         if __HAL_RCC_SYSCFG_CLK_ENABLE() is being called before.
  * @param  hcomp  COMP handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_COMP_Lock(COMP_HandleTypeDef *hcomp)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  /* Check the COMP handle allocation and lock status */
  if((hcomp == NULL) || (__HAL_COMP_IS_LOCKED(hcomp)))
  {
    status = HAL_ERROR;
  }
  else
  {
    /* Check the parameter */
    assert_param(IS_COMP_ALL_INSTANCE(hcomp->Instance));
    
    /* Set HAL COMP handle state */
    hcomp->State = ((HAL_COMP_StateTypeDef)(hcomp->State | COMP_STATE_BITFIELD_LOCK));
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
  *           - Comparator output is low when the input plus is at a lower
  *             voltage than the input minus
  *           - Comparator output is high when the input plus is at a higher
  *             voltage than the input minus
  *         If the polarity is inverted:
  *           - Comparator output is high when the input plus is at a lower
  *             voltage than the input minus
  *           - Comparator output is low when the input plus is at a higher
  *             voltage than the input minus
  * @param  hcomp  COMP handle
  * @retval Returns the selected comparator output level: 
  *         @arg @ref COMP_OUTPUT_LEVEL_LOW
  *         @arg @ref COMP_OUTPUT_LEVEL_HIGH
  *       
  */
uint32_t HAL_COMP_GetOutputLevel(COMP_HandleTypeDef *hcomp)
{
  /* Check the parameter */
  assert_param(IS_COMP_ALL_INSTANCE(hcomp->Instance));
  
  return (uint32_t)(READ_BIT(hcomp->Instance->CSR, COMP_CSR_COMPxOUTVALUE)
                    >> COMP_OUTPUT_LEVEL_BITOFFSET_POS);
}

/**
  * @brief  Comparator trigger callback.
  * @param  hcomp  COMP handle
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
 *  @brief   Peripheral State functions. 
 *
@verbatim   
 ===============================================================================
                      ##### Peripheral State functions #####
 ===============================================================================  
    [..]
    This subsection permit to get in run-time the status of the peripheral.

@endverbatim
  * @{
  */

/**
  * @brief  Return the COMP handle state.
  * @param  hcomp  COMP handle
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

  /* Return HAL COMP handle state */
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

#endif /* HAL_COMP_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

#endif /* !defined (STM32L010xB) && !defined (STM32L010x8) && !defined (STM32L010x6) && !defined (STM32L010x4) */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
