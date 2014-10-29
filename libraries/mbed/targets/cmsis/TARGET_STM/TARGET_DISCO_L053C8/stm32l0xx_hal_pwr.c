/**
  ******************************************************************************
  * @file    stm32l0xx_hal_pwr.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-June-2014
  * @brief   PWR HAL module driver.
  *
  *          This file provides firmware functions to manage the following
  *          functionalities of the Power Controller (PWR) peripheral:
  *           + Initialization/de-initialization functions
  *           + Peripheral Control functions 
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
#include "stm32l0xx_hal.h"

/** @addtogroup STM32L0xx_HAL_Driver
  * @{
  */

/** @defgroup PWR
  * @brief PWR HAL module driver
  * @{
  */

#ifdef HAL_PWR_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup PWR_Private_Functions
  * @{
  */

/** @defgroup PWR_Group1 Initialization and De-initialization functions 
  *  @brief Initialization and Configuration functions
  *
@verbatim
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]
      After reset, the backup domain (RTC registers, RTC backup data
      registers) is protected against possible unwanted
      write accesses.
      To enable access to the RTC Domain and RTC registers, proceed as follows:
        (+) Enable the Power Controller (PWR) APB1 interface clock using the
            __PWR_CLK_ENABLE() macro.
        (+) Enable access to RTC domain using the HAL_PWR_EnableBkUpAccess() function.

@endverbatim
  * @{
  */

/**
  * @brief Deinitializes the HAL PWR peripheral registers to their default reset values.
  * @param None
  * @retval None
  */
void HAL_PWR_DeInit(void)
{
  __PWR_FORCE_RESET();
  __PWR_RELEASE_RESET();
}

/**
  * @brief Enables access to the backup domain (RTC registers, RTC
  *         backup data registers ).
  * @note   If the HSE divided by 2, 4, 8 or 16 is used as the RTC clock, the
  *         Backup Domain Access should be kept enabled.
  * @param None
  * @retval None
  */
void HAL_PWR_EnableBkUpAccess(void)
{
  /* Enable access to RTC and backup registers */
  PWR->CR |= PWR_CR_DBP;
}

/**
  * @brief Disables access to the backup domain (RTC registers, RTC
  *         backup data registers).
  * @note   If the HSE divided by 2, 4, 8 or 16 is used as the RTC clock, the
  *         Backup Domain Access should be kept enabled.
  * @param None
  * @retval None
  */
void HAL_PWR_DisableBkUpAccess(void)
{
  /* Disable access to RTC and backup registers */
  PWR->CR &= (uint32_t)~((uint32_t)PWR_CR_DBP);
}

/**
  * @}
  */

/** @defgroup PWR_Group2 Peripheral Control functions 
  *  @brief Low Power modes configuration functions
  *
@verbatim

 ===============================================================================
                 ##### Peripheral Control functions #####
 ===============================================================================
     
    *** PVD configuration ***
    =========================
    [..]
      (+) The PVD is used to monitor the VDD power supply by comparing it to a
          threshold selected by the PVD Level (PLS[2:0] bits in the PWR_CR).
      (+) The PVD can use an external input analog voltage (PVD_IN) which is compared 
      internally to VREFINT. The PVD_IN (PB7) has to be configured in Analog mode 
      when PWR_PVDLevel_7 is selected (PLS[2:0] = 111).

      (+) A PVDO flag is available to indicate if VDD/VDDA is higher or lower
          than the PVD threshold. This event is internally connected to the EXTI
          line16 and can generate an interrupt if enabled. This is done through
          __HAL_PVD_EXTI_ENABLE_IT() macro.
      (+) The PVD is stopped in Standby mode.

    *** WakeUp pin configuration ***
    ================================
    [..]
      (+) WakeUp pin is used to wake up the system from Standby mode. This pin is
          forced in input pull-down configuration and is active on rising edges.
      (+) There are two WakeUp pins:
          WakeUp Pin 1 on PA.00.
          WakeUp Pin 2 on PC.13.

    [..]
    *** Main and Backup Regulators configuration ***
    ================================================

      (+) The main internal regulator can be configured to have a tradeoff between
          performance and power consumption when the device does not operate at
          the maximum frequency. This is done through __HAL_PWR_VOLTAGESCALING_CONFIG()
          macro which configure VOS bit in PWR_CR register:
        (++) When this bit is set (Regulator voltage output Scale 1 mode selected)
             the System frequency can go up to 32 MHz.
        (++) When this bit is reset (Regulator voltage output Scale 2 mode selected)
             the System frequency can go up to 16 MHz.
        (++) When this bit is reset (Regulator voltage output Scale 3 mode selected)
             the System frequency can go up to 4.2 MHz.
              
        Refer to the datasheets for more details.

    *** Low Power modes configuration ***
    =====================================
     [..]
      The device features 5 low-power modes:
      (+) Low power run mode: regulator in low power mode, limited clock frequency, 
        limited number of peripherals running.
      (+) Sleep mode: Cortex-M0+ core stopped, peripherals kept running.
      (+) Low power sleep mode: Cortex-M0+ core stopped, limited clock frequency, 
         limited number of peripherals running, regulator in low power mode.
      (+) Stop mode: All clocks are stopped, regulator running, regulator in low power mode.
      (+) Standby mode: VCORE domain powered off
  
   *** Low power run mode ***
   =========================
    [..]
       To further reduce the consumption when the system is in Run mode, the regulator can be
        configured in low power mode. In this mode, the system frequency should not exceed
        MSI frequency range1.
        In Low power run mode, all I/O pins keep the same state as in Run mode.
  
      (+) Entry:
        (++) VCORE in range2
        (++) Decrease the system frequency tonot exceed the frequency of MSI frequency range1.
        (++) The regulator is forced in low power mode using the HAL_PWREx_EnableLowPowerRunMode()
             function.
      (+) Exit:
        (++) The regulator is forced in Main regulator mode using the HAL_PWREx_DisableLowPowerRunMode()
              function.
        (++) Increase the system frequency if needed.
  
   *** Sleep mode ***
   ==================
    [..]
      (+) Entry:
          The Sleep mode is entered by using the HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFx)
              functions with
          (++) PWR_SLEEPENTRY_WFI: enter SLEEP mode with WFI instruction
          (++) PWR_SLEEPENTRY_WFE: enter SLEEP mode with WFE instruction
     
      (+) Exit:
        (++) Any peripheral interrupt acknowledged by the nested vectored interrupt
              controller (NVIC) can wake up the device from Sleep mode.

   *** Low power sleep mode ***
   ============================
    [..]
      (+) Entry:
          The Low power sleep mode is entered by using the HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFx)
              functions with
          (++) PWR_SLEEPENTRY_WFI: enter SLEEP mode with WFI instruction
          (++) PWR_SLEEPENTRY_WFE: enter SLEEP mode with WFE instruction
       (+) The Flash memory can be switched off by using the control bits (SLEEP_PD in the FLASH_ACR register. 
             This reduces power consumption but increases the wake-up time.
         
      (+) Exit:
        (++) If the WFI instruction was used to enter Low power sleep mode, any peripheral interrupt
              acknowledged by the nested vectored interrupt controller (NVIC) can wake up the device
              from Low power sleep mode. If the WFE instruction was used to enter Low power sleep mode,
              the MCU exits Sleep mode as soon as an event occurs. 
                
   *** Stop mode ***
   =================
    [..]
      The Stop mode is based on the Cortex-M0+ deepsleep mode combined with peripheral
      clock gating. The voltage regulator can be configured either in normal or low-power mode.
      In Stop mode, all clocks in the VCORE domain are stopped, the PLL, the MSI, the HSI and
      the HSE RC oscillators are disabled. Internal SRAM and register contents are preserved.
      To get the lowest consumption in Stop mode, the internal Flash memory also enters low
      power mode. When the Flash memory is in power-down mode, an additional startup delay is
      incurred when waking up from Stop mode.
      To minimize the consumption In Stop mode, VREFINT, the BOR, PVD, and temperature
      sensor can be switched off before entering Stop mode. They can be switched on again by
      software after exiting Stop mode using the ULP bit in the PWR_CR register.
      In Stop mode, all I/O pins keep the same state as in Run mode.

      (+) Entry:
           The Stop mode is entered using the HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI )
             function with:
          (++) Main regulator ON.
          (++) Low Power regulator ON.
          (++) PWR_SLEEPENTRY_WFI: enter SLEEP mode with WFI instruction
          (++) PWR_SLEEPENTRY_WFE: enter SLEEP mode with WFE instruction
      (+) Exit:
        (++) By issuing an interrupt or a wakeup event, the MSI or HSI16 RC
             oscillator is selected as system clock depending the bit STOPWUCK in the RCC_CFGR
             register

   *** Standby mode ***
   ====================
     [..]
      The Standby mode allows to achieve the lowest power consumption. It is based on the
      Cortex-M0+ deepsleep mode, with the voltage regulator disabled. The VCORE domain is
      consequently powered off. The PLL, the MSI, the HSI oscillator and the HSE oscillator are
      also switched off. SRAM and register contents are lost except for the RTC registers, RTC
      backup registers and Standby circuitry.
      
      To minimize the consumption In Standby mode, VREFINT, the BOR, PVD, and temperature
       sensor can be switched off before entering the Standby mode. They can be switched 
       on again by software after exiting the Standby mode.
       function.
      
      (+) Entry:
        (++) The Standby mode is entered using the HAL_PWR_EnterSTANDBYMode() function.
      (+) Exit:
        (++) WKUP pin rising edge, RTC alarm (Alarm A and Alarm B), RTC wakeup,
             tamper event, time-stamp event, external reset in NRST pin, IWDG reset.

   *** Auto-wakeup (AWU) from low-power mode ***
   =============================================
    [..]
      The MCU can be woken up from low-power mode by an RTC Alarm event, an RTC 
      Wakeup event, a tamper event, a time-stamp event, or a comparator event, 
      without depending on an external interrupt (Auto-wakeup mode).

    (+) RTC auto-wakeup (AWU) from the Stop mode
        (++) To wake up from the Stop mode with an RTC alarm event, it is necessary to:
             (+++) Configure the EXTI Line 17 to be sensitive to rising edges (Interrupt 
                   or Event modes) using the EXTI_Init() function.
             (+++) Enable the RTC Alarm Interrupt using the RTC_ITConfig() function
             (+++) Configure the RTC to generate the RTC alarm using the RTC_SetAlarm() 
                   and RTC_AlarmCmd() functions.
        (++) To wake up from the Stop mode with an RTC Tamper or time stamp event, it 
             is necessary to:
             (+++) Configure the EXTI Line 19 to be sensitive to rising edges (Interrupt 
                   or Event modes) using the EXTI_Init() function.
             (+++) Enable the RTC Tamper or time stamp Interrupt using the RTC_ITConfig() 
                   function.
             (+++) Configure the RTC to detect the tamper or time stamp event using the
                   RTC_TimeStampConfig(), RTC_TamperTriggerConfig() and RTC_TamperCmd()
                   functions.
        (++) To wake up from the Stop mode with an RTC WakeUp event, it is necessary to:
             (+++) Configure the EXTI Line 20 to be sensitive to rising edges (Interrupt 
                   or Event modes) using the EXTI_Init() function.
             (+++) Enable the RTC WakeUp Interrupt using the RTC_ITConfig() function.
             (+++) Configure the RTC to generate the RTC WakeUp event using the RTC_WakeUpClockConfig(), 
                   RTC_SetWakeUpCounter() and RTC_WakeUpCmd() functions.

    (+) RTC auto-wakeup (AWU) from the Standby mode
        (++) To wake up from the Standby mode with an RTC alarm event, it is necessary to:
             (+++) Enable the RTC Alarm Interrupt using the RTC_ITConfig() function.
             (+++) Configure the RTC to generate the RTC alarm using the RTC_SetAlarm() 
                   and RTC_AlarmCmd() functions.
        (++) To wake up from the Standby mode with an RTC Tamper or time stamp event, it 
             is necessary to:
             (+++) Enable the RTC Tamper or time stamp Interrupt using the RTC_ITConfig() 
                   function.
             (+++) Configure the RTC to detect the tamper or time stamp event using the
                   RTC_TimeStampConfig(), RTC_TamperTriggerConfig() and RTC_TamperCmd()
                   functions.
        (++) To wake up from the Standby mode with an RTC WakeUp event, it is necessary to:
             (+++) Enable the RTC WakeUp Interrupt using the RTC_ITConfig() function
             (+++) Configure the RTC to generate the RTC WakeUp event using the RTC_WakeUpClockConfig(), 
                   RTC_SetWakeUpCounter() and RTC_WakeUpCmd() functions.

    (+) Comparator auto-wakeup (AWU) from the Stop mode
        (++) To wake up from the Stop mode with an comparator 1 or comparator 2 wakeup
             event, it is necessary to:
             (+++) Configure the EXTI Line 21 for comparator 1 or EXTI Line 22 for comparator 2 
                   to be sensitive to to the selected edges (falling, rising or falling 
                   and rising) (Interrupt or Event modes) using the EXTI_Init() function.
             (+++) Configure the comparator to generate the event.      
        
@endverbatim
  * @{
  */

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
 uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_PWR_PVD_LEVEL(sConfigPVD->PVDLevel));

  tmpreg = PWR->CR;

  /* Clear PLS[7:5] bits */
  tmpreg &= ~ (uint32_t)PWR_CR_PLS;

  /* Set PLS[7:5] bits according to PVDLevel value */
  tmpreg |= sConfigPVD->PVDLevel;

  /* Store the new value */
  PWR->CR = tmpreg;

  /* Configure the EXTI 16 interrupt */
  if((sConfigPVD->Mode == PWR_MODE_IT_RISING_FALLING) ||\
     (sConfigPVD->Mode == PWR_MODE_IT_FALLING) ||\
     (sConfigPVD->Mode == PWR_MODE_IT_RISING)) 
  {
    __HAL_PVD_EXTI_ENABLE_IT(PWR_EXTI_LINE_PVD);
  }
  
  /* Clear the edge trigger  for the EXTI Line 16 (PVD) */
  EXTI->RTSR &= ~EXTI_RTSR_TR16;
  EXTI->FTSR &= ~EXTI_FTSR_TR16;

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
  /* Enable the power voltage detector */
  PWR->CR |= PWR_CR_PVDE;
}

/**
  * @brief Disables the Power Voltage Detector(PVD).
  * @param None
  * @retval None
  */
void HAL_PWR_DisablePVD(void)
{
  /* Disable the power voltage detector */
  PWR->CR &= (uint32_t)~((uint32_t)PWR_CR_PVDE);
}

/**
  * @brief Enables the WakeUp PINx functionality.
  * @param WakeUpPinx: Specifies the Power Wake-Up pin to enable.
  *         This parameter can be one of the following values:
  *           @arg PWR_WAKEUP_PIN1
  *           @arg PWR_WAKEUP_PIN2
  * @retval None
  */
void HAL_PWR_EnableWakeUpPin(uint32_t WakeUpPinx)
{
  /* Check the parameter */
  assert_param(IS_PWR_WAKEUP_PIN(WakeUpPinx));
  /* Enable the EWUPx pin */
  PWR->CSR |= WakeUpPinx;
}

/**
  * @brief Disables the WakeUp PINx functionality.
  * @param WakeUpPinx: Specifies the Power Wake-Up pin to disable.
  *         This parameter can be one of the following values:
  *           @arg PWR_WAKEUP_PIN1
  *           @arg PWR_WAKEUP_PIN2  
  * @retval None
  */
void HAL_PWR_DisableWakeUpPin(uint32_t WakeUpPinx)
{
  /* Check the parameter */
  assert_param(IS_PWR_WAKEUP_PIN(WakeUpPinx));
  /* Disable the EWUPx pin */
  PWR->CSR &= ~WakeUpPinx;
}

/**
  * @brief Enters Sleep mode.
  * @note In Sleep mode, all I/O pins keep the same state as in Run mode.
  * @param Regulator: Specifies the regulator state in SLEEP mode.
  *          This parameter can be one of the following values:
  *            @arg PWR_MAINREGULATOR_ON: SLEEP mode with regulator ON
  *            @arg PWR_LOWPOWERREGULATOR_ON: SLEEP mode with low power regulator ON
  * @param SLEEPEntry: Specifies if SLEEP mode is entered with WFI or WFE instruction.
  *           When WFI entry is used, tick interrupt have to be disabled if not desired as 
  *           the interrupt wake up source.
  *           This parameter can be one of the following values:
  *            @arg PWR_SLEEPENTRY_WFI: enter SLEEP mode with WFI instruction
  *            @arg PWR_SLEEPENTRY_WFE: enter SLEEP mode with WFE instruction
  * @retval None
  */
void HAL_PWR_EnterSLEEPMode(uint32_t Regulator, uint8_t SLEEPEntry)
{
   uint32_t tmpreg = 0;
  /* Check the parameters */
  assert_param(IS_PWR_REGULATOR(Regulator));
  assert_param(IS_PWR_SLEEP_ENTRY(SLEEPEntry));

   /* Select the regulator state in Sleep mode ---------------------------------*/
  tmpreg = PWR->CR;
  /* Clear PDDS and LPDS bits */
  tmpreg &= (uint32_t)~(PWR_CR_PDDS | PWR_CR_LPSDSR);

 /* Set LPSDSR bit according to PWR_Regulator value */
  tmpreg |= Regulator;

  /* Store the new value */
  PWR->CR = tmpreg;
  
  /* Clear SLEEPDEEP bit of Cortex System Control Register */
  SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);

  /* Select SLEEP mode entry -------------------------------------------------*/
  if(SLEEPEntry == PWR_SLEEPENTRY_WFI)
  {
    /* Request Wait For Interrupt */
    __WFI();
  }
  else
  {
    /* Request Wait For Event */
    __SEV();
    __WFE();
    __WFE();
  }
}

/**
  * @brief Enters Stop mode. 
  * @note In Stop mode, all I/O pins keep the same state as in Run mode.
  * @note When exiting Stop mode by issuing an interrupt or a wakeup event,
  *        MSI or HSI16 RCoscillator is selected as system clock depending 
  *        the bit STOPWUCK in the RCC_CFGR register.
  * @note When the voltage regulator operates in low power mode, an additional
  *         startup delay is incurred when waking up from Stop mode. 
  *         By keeping the internal regulator ON during Stop mode, the consumption
  *         is higher although the startup time is reduced.    
  * @param Regulator: Specifies the regulator state in Stop mode.
  *          This parameter can be one of the following values:
  *            @arg PWR_MAINREGULATOR_ON: Stop mode with regulator ON
  *            @arg PWR_LOWPOWERREGULATOR_ON: Stop mode with low power regulator ON
  * @param STOPEntry: Specifies if Stop mode in entered with WFI or WFE instruction.
  *          This parameter can be one of the following values:
  *            @arg PWR_STOPENTRY_WFI: Enter Stop mode with WFI instruction
  *            @arg PWR_STOPENTRY_WFE: Enter Stop mode with WFE instruction   
  * @retval None
  */
void HAL_PWR_EnterSTOPMode(uint32_t Regulator, uint8_t STOPEntry)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_PWR_REGULATOR(Regulator));
  assert_param(IS_PWR_STOP_ENTRY(STOPEntry));

  /* Select the regulator state in Stop mode ---------------------------------*/
  tmpreg = PWR->CR;
  /* Clear PDDS and LPDS bits */
  tmpreg &= (uint32_t)~(PWR_CR_PDDS | PWR_CR_LPSDSR);

 /* Set LPSDSR bit according to PWR_Regulator value */
  tmpreg |= Regulator;

  /* Store the new value */
  PWR->CR = tmpreg;

  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

  /* Select Stop mode entry --------------------------------------------------*/
  if(STOPEntry == PWR_STOPENTRY_WFI)
  {
    /* Request Wait For Interrupt */
    __WFI();
  }
  else
  {
    /* Request Wait For Event */
    __SEV();
    __WFE();
    __WFE();
  }
 
  /* Reset SLEEPDEEP bit of Cortex System Control Register */
  SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);
}

/**
  * @brief Enters Standby mode.
  * @note In Standby mode, all I/O pins are high impedance except for:
  *          - Reset pad (still available) 
  *          - RTC_AF1 pin (PC13) if configured for tamper, time-stamp, RTC
  *            Alarm out, or RTC clock calibration out.
  *          - RTC_AF2 pin (PC13) if configured for tamper.
  *          - WKUP pin 1 (PA0) if enabled.
  *          - WKUP pin 2 (PC13) if enabled.
  * @param None
  * @retval None
  */
void HAL_PWR_EnterSTANDBYMode(void)
{
  /* Select Standby mode */
  PWR->CR |= PWR_CR_PDDS;

  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

  /* This option is used to ensure that store operations are completed */
#if defined ( __CC_ARM)
  __force_stores();
#endif
  /* Request Wait For Interrupt */
  __WFI();
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
  * @brief  PWR PVD interrupt callback
  * @param  None 
  * @retval None
  */
__weak void HAL_PWR_PVDCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_PWR_PVDCallback could be implemented in the user file
   */ 
}

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
