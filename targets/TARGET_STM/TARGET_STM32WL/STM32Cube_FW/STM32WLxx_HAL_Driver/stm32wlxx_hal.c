/**
  ******************************************************************************
  * @file    stm32wlxx_hal.c
  * @author  MCD Application Team
  * @brief   HAL module driver.
  *          This is the common part of the HAL initialization
  *
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
    [..]
    The common HAL driver contains a set of generic and common APIs that can be
    used by the PPP peripheral drivers and the user to start using the HAL.
    [..]
    The HAL contains two APIs' categories:
         (+) Common HAL APIs
         (+) Services HAL APIs

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32wlxx_hal.h"

/** @addtogroup STM32WLxx_HAL_Driver
  * @{
  */

/** @addtogroup HAL
  * @brief HAL module driver
  * @{
  */

#ifdef HAL_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/** @defgroup HAL_Private_Constants HAL Private Constants
  * @{
  */
/**
 * @brief STM32WLxx HAL Driver version number
   */
#define __STM32WLxx_HAL_VERSION_MAIN   (0x01U) /*!< [31:24] main version */
#define __STM32WLxx_HAL_VERSION_SUB1   (0x01U) /*!< [23:16] sub1 version */
#define __STM32WLxx_HAL_VERSION_SUB2   (0x00U) /*!< [15:8]  sub2 version */
#define __STM32WLxx_HAL_VERSION_RC     (0x00U) /*!< [7:0]  release candidate */
#define __STM32WLxx_HAL_VERSION         ((__STM32WLxx_HAL_VERSION_MAIN << 24U)\
                                        |(__STM32WLxx_HAL_VERSION_SUB1 << 16U)\
                                        |(__STM32WLxx_HAL_VERSION_SUB2 << 8U )\
                                        |(__STM32WLxx_HAL_VERSION_RC))

#define VREFBUF_TIMEOUT_VALUE     10U   /* 10 ms */

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
/** @defgroup HAL_Exported_Variables HAL Exported Variables
  * @{
  */
__IO uint32_t uwTick;
uint32_t uwTickPrio   = (1UL << __NVIC_PRIO_BITS); /* Invalid PRIO */
HAL_TickFreqTypeDef uwTickFreq = HAL_TICK_FREQ_DEFAULT;  /* 1KHz */
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @addtogroup HAL_Exported_Functions
  * @{
  */

/** @addtogroup HAL_Exported_Functions_Group1
 *  @brief    HAL Initialization and Configuration functions
 *
@verbatim
 ===============================================================================
           ##### HAL Initialization and Configuration functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Initialize the Flash interface the NVIC allocation and initial time base
          clock configuration.
      (+) De-initialize common part of the HAL.
      (+) Configure the time base source to have 1ms time base with a dedicated
          Tick interrupt priority.
        (++) SysTick timer is used by default as source of time base, but user
             can eventually implement his proper time base source (a general purpose
             timer for example or other time source), keeping in mind that Time base
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
             handled in milliseconds basis.
        (++) Time base configuration function (HAL_InitTick ()) is called automatically
             at the beginning of the program after reset by HAL_Init() or at any time
             when clock is configured, by HAL_RCC_ClockConfig().
        (++) Source of time base is configured  to generate interrupts at regular
             time intervals. Care must be taken if HAL_Delay() is called from a
             peripheral ISR process, the Tick interrupt line must have higher priority
            (numerically lower) than the peripheral interrupt. Otherwise the caller
            ISR process will be blocked.
       (++) functions affecting time base configurations are declared as __weak
             to make  override possible  in case of other  implementations in user file.
@endverbatim
  * @{
  */

/**
  * @brief  This function is used to initialize the HAL Library; it must be the first
  *         instruction to be executed in the main program (before to call any other
  *         HAL function), it performs the following:
  *           Configure the Flash prefetch, instruction and Data caches.
  *           Configures the SysTick to generate an interrupt each 1 millisecond,
  *           which is clocked by the MSI (at this stage, the clock is not yet
  *           configured and thus the system is running from the internal MSI at 4 MHz).
  *           Set NVIC Group Priority to 4.
  *           Calls the HAL_MspInit() callback function defined in user file
  *           "stm32wlxx_hal_msp.c" to do the global low level hardware initialization
  *
  * @note   SysTick is used as time base for the HAL_Delay() function, the application
  *         need to ensure that the SysTick time base is always set to 1 millisecond
  *         to have correct HAL operation.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_Init(void)
{
  HAL_StatusTypeDef  status = HAL_OK;
  /* Configure Flash prefetch, Instruction cache, Data cache */
  /* Default configuration at reset is:                      */
  /* - Prefetch disabled                                     */
  /* - Instruction cache enabled                             */
  /* - Data cache enabled                                    */
#if (INSTRUCTION_CACHE_ENABLE == 0U)
  __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();
#endif /* INSTRUCTION_CACHE_ENABLE */

#ifdef CORE_CM0PLUS
#else
#if (DATA_CACHE_ENABLE == 0U)
  __HAL_FLASH_DATA_CACHE_DISABLE();
#endif /* DATA_CACHE_ENABLE */
#endif

#if (PREFETCH_ENABLE != 0U)
  __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
#endif /* PREFETCH_ENABLE */

#ifdef CORE_CM0PLUS
#else
  /* Set Interrupt Group Priority */
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
#endif

  /* Update the SystemCoreClock global variable */
#if defined(DUAL_CORE) && defined(CORE_CM0PLUS)
  SystemCoreClock = HAL_RCC_GetHCLK2Freq();
#else
  SystemCoreClock = HAL_RCC_GetHCLKFreq();
#endif

  /* Use SysTick as time base source and configure 1ms tick (default clock after Reset is MSI) */
  if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK)
  {
    status = HAL_ERROR;
  }
  else
  {
    /* Init the low level hardware */
    HAL_MspInit();
  }

  /* Return function status */
  return status;
}

/**
  * @brief  This function de-Initializes common part of the HAL and stops the source of time base.
  * @note   This function is optional.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DeInit(void)
{
  /* Reset of all peripherals */
  __HAL_RCC_APB1_FORCE_RESET();
  __HAL_RCC_APB1_RELEASE_RESET();

  __HAL_RCC_APB2_FORCE_RESET();
  __HAL_RCC_APB2_RELEASE_RESET();

  __HAL_RCC_APB3_FORCE_RESET();
  __HAL_RCC_APB3_RELEASE_RESET();

  __HAL_RCC_AHB1_FORCE_RESET();
  __HAL_RCC_AHB1_RELEASE_RESET();

  __HAL_RCC_AHB2_FORCE_RESET();
  __HAL_RCC_AHB2_RELEASE_RESET();

  __HAL_RCC_AHB3_FORCE_RESET();
  __HAL_RCC_AHB3_RELEASE_RESET();

  /* De-Init the low level hardware */
  HAL_MspDeInit();

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Initialize the MSP.
  * @retval None
  */
__weak void HAL_MspInit(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_MspInit could be implemented in the user file
   */
}

/**
  * @brief  DeInitializes the MSP.
  * @retval None
  */
__weak void HAL_MspDeInit(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_MspDeInit could be implemented in the user file
   */
}

/**
  * @brief This function configures the source of the time base:
  *        The time source is configured  to have 1ms time base with a dedicated
  *        Tick interrupt priority.
  * @note This function is called  automatically at the beginning of program after
  *       reset by HAL_Init() or at any time when clock is reconfigured  by HAL_RCC_ClockConfig().
  * @note In the default implementation, SysTick timer is the source of time base.
  *       It is used to generate interrupts at regular time intervals.
  *       Care must be taken if HAL_Delay() is called from a peripheral ISR process,
  *       The SysTick interrupt must have higher priority (numerically lower)
  *       than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
  *       The function is declared as __weak  to be overwritten  in case of other
  *       implementation  in user file.
  * @param TickPriority Tick interrupt priority.
  * @retval HAL status
  */
__weak HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  HAL_StatusTypeDef  status = HAL_OK;

  /* Check uwTickFreq for MisraC 2012 (even if uwTickFreq is a enum type that don't take the value zero)*/
  if ((uint32_t)uwTickFreq != 0U)
  {
    /*Configure the SysTick to have interrupt in 1ms time basis*/
#ifdef CORE_CM0PLUS
    if (HAL_SYSTICK_Config(HAL_RCC_GetHCLK2Freq() / (1000U / (uint32_t)uwTickFreq)) == 0U)
#else
    if (HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / (1000U / (uint32_t)uwTickFreq)) == 0U)
#endif
    {
      /* Configure the SysTick IRQ priority */
      if (TickPriority < (1UL << __NVIC_PRIO_BITS))
      {
        HAL_NVIC_SetPriority(SysTick_IRQn, TickPriority, 0U);
        uwTickPrio = TickPriority;
      }
      else
      {
        status = HAL_ERROR;
      }
    }
    else
    {
      status = HAL_ERROR;
    }
  }
  else
  {
    status = HAL_ERROR;
  }

  /* Return function status */
  return status;
}

/**
  * @}
  */

/** @addtogroup HAL_Exported_Functions_Group2
 *  @brief    HAL Control functions
 *
@verbatim
 ===============================================================================
                      ##### HAL Control functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Provide a tick value in millisecond
      (+) Provide a blocking delay in millisecond
      (+) Suspend the time base source interrupt
      (+) Resume the time base source interrupt
      (+) Get the HAL API driver version
      (+) Get the device revision identifier
      (+) Get the device identifier
      (+) Get the unique device identifier

@endverbatim
  * @{
  */

/**
  * @brief This function is called to increment  a global variable "uwTick"
  *        used as application time base.
  * @note In the default implementation, this variable is incremented each 1ms
  *       in SysTick ISR.
 * @note This function is declared as __weak to be overwritten in case of other
  *      implementations in user file.
  * @retval None
  */
__weak void HAL_IncTick(void)
{
  uwTick += (uint32_t)uwTickFreq;
}

/**
  * @brief Provides a tick value in millisecond.
  * @note This function is declared as __weak to be overwritten in case of other
  *       implementations in user file.
  * @retval tick value
  */
__weak uint32_t HAL_GetTick(void)
{
  return uwTick;
}

/**
  * @brief This function returns a tick priority.
  * @retval tick priority
  */
uint32_t HAL_GetTickPrio(void)
{
  return uwTickPrio;
}

/**
  * @brief Set new tick Freq.
  * @retval Status
  */
HAL_StatusTypeDef HAL_SetTickFreq(HAL_TickFreqTypeDef Freq)
{
  HAL_StatusTypeDef status  = HAL_OK;
  HAL_TickFreqTypeDef prevTickFreq;
  assert_param(IS_TICKFREQ(Freq));

  if (uwTickFreq != Freq)
  {
    /* Back up uwTickFreq frequency */
    prevTickFreq = uwTickFreq;

    /* Update uwTickFreq global variable used by HAL_InitTick() */
    uwTickFreq = Freq;

    /* Apply the new tick Freq */
    status = HAL_InitTick(uwTickPrio);

    if (status != HAL_OK)
    {
      /* Restore previous tick frequency */
      uwTickFreq = prevTickFreq;
    }
  }

  return status;
}

/**
  * @brief Return tick frequency.
  * @retval tick period in Hz
  */
HAL_TickFreqTypeDef HAL_GetTickFreq(void)
{
  return uwTickFreq;
}

/**
  * @brief This function provides minimum delay (in milliseconds) based
  *        on variable incremented.
  * @note In the default implementation , SysTick timer is the source of time base.
  *       It is used to generate interrupts at regular time intervals where uwTick
  *       is incremented.
  * @note This function is declared as __weak to be overwritten in case of other
  *       implementations in user file.
  * @param Delay  specifies the delay time length, in milliseconds.
  * @retval None
  */
__weak void HAL_Delay(uint32_t Delay)
{
  uint32_t tickstart = HAL_GetTick();
  uint32_t wait = Delay;

  /* Add a freq to guarantee minimum wait */
  if (wait < HAL_MAX_DELAY)
  {
    wait += (uint32_t)(uwTickFreq);
  }

  while ((HAL_GetTick() - tickstart) < wait)
  {
  }
}


/**
  * @brief Suspend Tick increment.
  * @note In the default implementation , SysTick timer is the source of time base. It is
  *       used to generate interrupts at regular time intervals. Once HAL_SuspendTick()
  *       is called, the SysTick interrupt will be disabled and so Tick increment
  *       is suspended.
  * @note This function is declared as __weak to be overwritten in case of other
  *       implementations in user file.
  * @retval None
  */
__weak void HAL_SuspendTick(void)
{
  /* Disable SysTick Interrupt */
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
}

/**
  * @brief Resume Tick increment.
  * @note In the default implementation , SysTick timer is the source of time base. It is
  *       used to generate interrupts at regular time intervals. Once HAL_ResumeTick()
  *       is called, the SysTick interrupt will be enabled and so Tick increment
  *       is resumed.
  * @note This function is declared as __weak to be overwritten in case of other
  *       implementations in user file.
  * @retval None
  */
__weak void HAL_ResumeTick(void)
{
  /* Enable SysTick Interrupt */
  SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
}

/**
  * @brief  Returns the HAL revision
  * @retval version : 0xXYZR (8bits for each decimal, R for RC)
  */
uint32_t HAL_GetHalVersion(void)
{
  return __STM32WLxx_HAL_VERSION;
}

#if defined(CORE_CM0PLUS)
#else
/**
  * @brief  Returns the device revision identifier.
  * @retval Device revision identifier
  */
uint32_t HAL_GetREVID(void)
{
  return (LL_DBGMCU_GetRevisionID());
}

/**
  * @brief  Returns the device identifier.
  * @retval Device identifier
  */
uint32_t HAL_GetDEVID(void)
{
  return (LL_DBGMCU_GetDeviceID());
}
#endif

/**
  * @brief  Return the first word of the unique device identifier (UID based on 96 bits)
  * @retval Device identifier
  */
uint32_t HAL_GetUIDw0(void)
{
  return (READ_REG(*((uint32_t *)UID_BASE)));
}

/**
  * @brief  Return the second word of the unique device identifier (UID based on 96 bits)
  * @retval Device identifier
  */
uint32_t HAL_GetUIDw1(void)
{
  return (READ_REG(*((uint32_t *)(UID_BASE + 4U))));
}

/**
  * @brief  Return the third word of the unique device identifier (UID based on 96 bits)
  * @retval Device identifier
  */
uint32_t HAL_GetUIDw2(void)
{
  return (READ_REG(*((uint32_t *)(UID_BASE + 8U))));
}

/**
  * @}
  */

/** @addtogroup HAL_Exported_Functions_Group3
 *  @brief    HAL Debug functions
 *
@verbatim
 ===============================================================================
                      ##### HAL Debug functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Enable/Disable Debug module during SLEEP mode
      (+) Enable/Disable Debug module during STOP mode
      (+) Enable/Disable Debug module during STANDBY mode

@endverbatim
  * @{
  */

#if defined(CORE_CM0PLUS)
#else
/**
  * @brief  Enable the CPU1 Debug Module during SLEEP mode
  * @retval None
  */
void HAL_DBGMCU_EnableDBGSleepMode(void)
{
  LL_DBGMCU_EnableDBGSleepMode();
}

/**
  * @brief  Disable the CPU1 Debug Module during SLEEP mode
  * @retval None
  */
void HAL_DBGMCU_DisableDBGSleepMode(void)
{
  LL_DBGMCU_DisableDBGSleepMode();
}

/**
  * @brief  Enable the Debug Module during STOP mode
  * @note  This functionality does not influence CPU2 operation, CPU2 cannot be debugged
  *        in Stop mode even when this bit is enabled
  * @retval None
  */
void HAL_DBGMCU_EnableDBGStopMode(void)
{
  LL_DBGMCU_EnableDBGStopMode();
}

/**
  * @brief  Disable the CPU1 Debug Module during STOP mode
  * @retval None
  */
void HAL_DBGMCU_DisableDBGStopMode(void)
{
  LL_DBGMCU_DisableDBGStopMode();
}

/**
  * @brief  Enable the Debug Module during STANDBY mode
  * @note  This functionality does not influence CPU2 operation, CPU2 cannot be debugged
  *        in Standby mode even when this bit is enabled
  * @retval None
  */
void HAL_DBGMCU_EnableDBGStandbyMode(void)
{
  LL_DBGMCU_EnableDBGStandbyMode();
}

/**
  * @brief  Disable the CPU1 Debug Module during STANDBY mode
  * @retval None
  */
void HAL_DBGMCU_DisableDBGStandbyMode(void)
{
  LL_DBGMCU_DisableDBGStandbyMode();
}
#endif

/**
  * @}
  */

/** @defgroup HAL_Exported_Functions_Group4 HAL System Configuration functions
 *  @brief    HAL System Configuration functions
 *
@verbatim
 ===============================================================================
                 ##### HAL system configuration functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Start a hardware SRAM2 erase operation
      (+) Configure the Voltage reference buffer
      (+) Enable/Disable the Voltage reference buffer
      (+) Enable/Disable the I/O analog switch voltage booster

@endverbatim
  * @{
  */

/**
  * @brief  Start a hardware SRAM2 erase operation.
  * @note   As long as SRAM2 is not erased the SRAM2ER bit will be set.
  *         This bit is automatically reset at the end of the SRAM2 erase operation.
  * @retval None
  */
void HAL_SYSCFG_SRAM2Erase(void)
{
  /* unlock the write protection of the SRAM2ER bit */
  __HAL_SYSCFG_SRAM2_WRP_UNLOCK();
  /* Starts a hardware SRAM2 erase operation*/
  __HAL_SYSCFG_SRAM2_ERASE();
}

/**
  * @brief Configure the internal voltage reference buffer voltage scale.
  * @param VoltageScaling  specifies the output voltage to achieve
  *          This parameter can be one of the following values:
  *            @arg @ref SYSCFG_VREFBUF_VOLTAGE_SCALE0 : VREF_OUT1 around 2.048 V.
  *                                                This requires VDDA equal to or higher than 2.4 V.
  *            @arg @ref SYSCFG_VREFBUF_VOLTAGE_SCALE1 : VREF_OUT1 around 2.5 V.
  *                                                This requires VDDA equal to or higher than 2.8 V.
  * @note   Retrieve the TrimmingValue from factory located at
  *         VREFBUF_SC0_CAL_ADDR or VREFBUF_SC1_CAL_ADDR addresses.
  * @retval None
  */
void HAL_SYSCFG_VREFBUF_VoltageScalingConfig(uint32_t VoltageScaling)
{
  uint32_t TrimmingValue;

  /* Check the parameters */
  assert_param(IS_SYSCFG_VREFBUF_VOLTAGE_SCALE(VoltageScaling));
  
  LL_VREFBUF_SetVoltageScaling(VoltageScaling);
  
  /* Restrieve Calibration data and store them into trimming field */
  if (VoltageScaling == SYSCFG_VREFBUF_VOLTAGE_SCALE0)
  {
    TrimmingValue = ((uint32_t) *VREFBUF_SC0_CAL_ADDR) & 0x3FU;
  }
  else
  {
    TrimmingValue = ((uint32_t) *VREFBUF_SC1_CAL_ADDR) & 0x3FU;
  }
  assert_param(IS_SYSCFG_VREFBUF_TRIMMING(TrimmingValue));

  HAL_SYSCFG_VREFBUF_TrimmingConfig(TrimmingValue);
}

/**
  * @brief Configure the internal voltage reference buffer high impedance mode.
  * @param Mode  specifies the high impedance mode
  *          This parameter can be one of the following values:
  *            @arg @ref SYSCFG_VREFBUF_HIGH_IMPEDANCE_DISABLE : VREF+ pin is internally connect to VREFINT output.
  *            @arg @ref SYSCFG_VREFBUF_HIGH_IMPEDANCE_ENABLE : VREF+ pin is high impedance.
  * @retval HAL_OK/HAL_TIMEOUT
  */
void HAL_SYSCFG_VREFBUF_HighImpedanceConfig(uint32_t Mode)
{

  /* Check the parameters */
  assert_param(IS_SYSCFG_VREFBUF_HIGH_IMPEDANCE(Mode));

  MODIFY_REG(VREFBUF->CSR, VREFBUF_CSR_HIZ, Mode);
}

/**
  * @brief Tune the Internal Voltage Reference buffer (VREFBUF).
  * @note  Each VrefBuf voltage scale is calibrated in production for each device,
  *        data stored in flash memory.
  *        Function @ref HAL_SYSCFG_VREFBUF_VoltageScalingConfig retrieves and 
  *        applies this calibration data as trimming value at each scale change.
  *        Therefore, optionally, function @ref HAL_SYSCFG_VREFBUF_TrimmingConfig
  *        can be used in a second time to fine tune the trimming.
  * @param TrimmingValue specifies trimming code for VREFBUF calibration
  *          This parameter can be a number between Min_Data = 0x00 and Max_Data = 0x3F
  * @retval None
  */
void HAL_SYSCFG_VREFBUF_TrimmingConfig(uint32_t TrimmingValue)
{
  /* Check the parameters */
  assert_param(IS_SYSCFG_VREFBUF_TRIMMING(TrimmingValue));

  LL_VREFBUF_SetTrimming(TrimmingValue);

}

/**
  * @brief  Enable the Internal Voltage Reference buffer (VREFBUF).
  * @retval HAL_OK/HAL_TIMEOUT
  */
HAL_StatusTypeDef HAL_SYSCFG_EnableVREFBUF(void)
{
  uint32_t tickstart;

  LL_VREFBUF_Enable();

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Wait for VRR bit  */
  while (READ_BIT(VREFBUF->CSR, VREFBUF_CSR_VRR) == 0U)
  {
    if ((HAL_GetTick() - tickstart) > VREFBUF_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  return HAL_OK;
}

/**
  * @brief  Disable the Internal Voltage Reference buffer (VREFBUF).
  *
  * @retval None
  */
void HAL_SYSCFG_DisableVREFBUF(void)
{
  LL_VREFBUF_Disable();
}

/**
  * @brief  Enable the I/O analog switch voltage booster
  *
  * @retval None
  */
void HAL_SYSCFG_EnableIOAnalogSwitchBooster(void)
{
  LL_SYSCFG_EnableAnalogBooster();
}

/**
  * @brief  Disable the I/O analog switch voltage booster
  *
  * @retval None
  */
void HAL_SYSCFG_DisableIOAnalogSwitchBooster(void)
{
  LL_SYSCFG_DisableAnalogBooster();
}

#if defined(DUAL_CORE)
/**
  * @brief  Enable Additional Interrupt Mask
  * @note   This interface is an additional interrupt masking interface
  *         up to the EXTI interface
  * @param  Interrupt Pointer to a SYSCFG_InterruptTypeDef structure that contains
  *                   the Interrupt Mask configuration
  * @retval None
  */
void HAL_SYSCFG_EnableIT(SYSCFG_InterruptTypeDef *Interrupt)
{
  uint32_t InterruptMask1 = (Interrupt->InterruptMask1 & ~HAL_SYSCFG_GRP1_RESERVED);
  uint32_t InterruptMask2 = (Interrupt->InterruptMask2 & ~HAL_SYSCFG_GRP2_RESERVED);

  /* Check the parameters */
  assert_param(IS_SYSCFG_IM_GRP1(Interrupt->InterruptMask1));
  assert_param(IS_SYSCFG_IM_GRP2(Interrupt->InterruptMask2));

#if defined(CORE_CM0PLUS)
  LL_C2_SYSCFG_GRP1_EnableIT(InterruptMask1);
  LL_C2_SYSCFG_GRP2_EnableIT(InterruptMask2);
#else
  LL_SYSCFG_GRP1_EnableIT(InterruptMask1);
  LL_SYSCFG_GRP2_EnableIT(InterruptMask2);
#endif
}

/**
  * @brief  Disable Additional Interrupt Mask
  * @note   This interface is an additional interrupt masking interface
  *         up to the EXTI interface
  * @param  Interrupt Pointer to a SYSCFG_InterruptTypeDef structure that contains
  *                   the Interrupt Mask configuration
  * @retval None
  */
void HAL_SYSCFG_DisableIT(SYSCFG_InterruptTypeDef *Interrupt)
{
  uint32_t InterruptMask1 = (Interrupt->InterruptMask1 & ~HAL_SYSCFG_GRP1_RESERVED);
  uint32_t InterruptMask2 = (Interrupt->InterruptMask2 & ~HAL_SYSCFG_GRP2_RESERVED);

  /* Check the parameters */
  assert_param(IS_SYSCFG_IM_GRP1(Interrupt->InterruptMask1));
  assert_param(IS_SYSCFG_IM_GRP2(Interrupt->InterruptMask2));

#if defined(CORE_CM0PLUS)
  LL_C2_SYSCFG_GRP1_DisableIT(InterruptMask1);
  LL_C2_SYSCFG_GRP2_DisableIT(InterruptMask2);
#else
  LL_SYSCFG_GRP1_DisableIT(InterruptMask1);
  LL_SYSCFG_GRP2_DisableIT(InterruptMask2);
#endif
}
#endif /* DUAL_CORE */
/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
