/**
  ******************************************************************************
  * @file    stm32f3xx_hal_rcc.c
  * @author  MCD Application Team
  * @brief   RCC HAL module driver.
  *          This file provides firmware functions to manage the following 
  *          functionalities of the Reset and Clock Control (RCC) peripheral:
  *           + Initialization and de-initialization functions
  *           + Peripheral Control functions
  *       
  @verbatim                
  ==============================================================================
                      ##### RCC specific features #####
  ==============================================================================
    [..]  
      After reset the device is running from Internal High Speed oscillator
      (HSI 8MHz) with Flash 0 wait state, Flash prefetch buffer is enabled, 
      and all peripherals are off except internal SRAM, Flash and JTAG.
      (+) There is no prescaler on High speed (AHB) and Low speed (APB) buses;
          all peripherals mapped on these buses are running at HSI speed.
      (+) The clock for all peripherals is switched off, except the SRAM and FLASH.
      (+) All GPIOs are in input floating state, except the JTAG pins which
          are assigned to be used for debug purpose.
    [..] Once the device started from reset, the user application has to:
      (+) Configure the clock source to be used to drive the System clock
          (if the application needs higher frequency/performance)
      (+) Configure the System clock frequency and Flash settings  
      (+) Configure the AHB and APB buses prescalers
      (+) Enable the clock for the peripheral(s) to be used
      (+) Configure the clock source(s) for peripherals whose clocks are not
          derived from the System clock (RTC, ADC, I2C, I2S, TIM, USB FS)

                      ##### RCC Limitations #####
  ==============================================================================
    [..]  
      A delay between an RCC peripheral clock enable and the effective peripheral 
      enabling should be taken into account in order to manage the peripheral read/write 
      from/to registers.
      (+) This delay depends on the peripheral mapping.
        (++) AHB & APB peripherals, 1 dummy read is necessary

    [..]  
      Workarounds:
      (#) For AHB & APB peripherals, a dummy read to the peripheral register has been
          inserted in each __HAL_RCC_PPP_CLK_ENABLE() macro.

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

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/** @addtogroup STM32F3xx_HAL_Driver
  * @{
  */

/** @defgroup RCC RCC
* @brief RCC HAL module driver
  * @{
  */

#ifdef HAL_RCC_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup RCC_Private_Constants RCC Private Constants
 * @{
 */
/* Bits position in  in the CFGR register */
#define RCC_CFGR_HPRE_BITNUMBER           POSITION_VAL(RCC_CFGR_HPRE)
#define RCC_CFGR_PPRE1_BITNUMBER          POSITION_VAL(RCC_CFGR_PPRE1)
#define RCC_CFGR_PPRE2_BITNUMBER          POSITION_VAL(RCC_CFGR_PPRE2)
/**
  * @}
  */
/* Private macro -------------------------------------------------------------*/
/** @defgroup RCC_Private_Macros RCC Private Macros
  * @{
  */

#define MCO1_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define MCO1_GPIO_PORT        GPIOA
#define MCO1_PIN              GPIO_PIN_8

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup RCC_Private_Variables RCC Private Variables
  * @{
  */
const uint8_t aPLLMULFactorTable[16] = { 2U,  3U,  4U,  5U,  6U,  7U,  8U,  9U,
                                       10U, 11U, 12U, 13U, 14U, 15U, 16U, 16U};
const uint8_t aPredivFactorTable[16] = { 1U, 2U,  3U,  4U,  5U,  6U,  7U,  8U,
                                         9U,10U, 11U, 12U, 13U, 14U, 15U, 16U};
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

/** @defgroup RCC_Exported_Functions RCC Exported Functions
  * @{
  */

/** @defgroup RCC_Exported_Functions_Group1 Initialization and de-initialization functions 
  *  @brief    Initialization and Configuration functions 
  *
  @verbatim    
  ===============================================================================
           ##### Initialization and de-initialization functions #####
  ===============================================================================
    [..]
      This section provides functions allowing to configure the internal/external oscillators
      (HSE, HSI, LSE, LSI, PLL, CSS and MCO) and the System buses clocks (SYSCLK, AHB, APB1
      and APB2).

    [..] Internal/external clock and PLL configuration
      (#) HSI (high-speed internal), 8 MHz factory-trimmed RC used directly or through
          the PLL as System clock source.
          The HSI clock can be used also to clock the USART and I2C peripherals.

      (#) LSI (low-speed internal), ~40 KHz low consumption RC used as IWDG and/or RTC
          clock source.

      (#) HSE (high-speed external), 4 to 32 MHz crystal oscillator used directly or
          through the PLL as System clock source. Can be used also as RTC clock source.

      (#) LSE (low-speed external), 32 KHz oscillator used as RTC clock source.   

      (#) PLL (clocked by HSI or HSE), featuring different output clocks:
        (++) The first output is used to generate the high speed system clock (up to 72 MHz)
        (++) The second output is used to generate the clock for the USB FS (48 MHz)
        (++) The third output may be used to generate the clock for the ADC peripherals (up to 72 MHz)
        (++) The fourth output may be used to generate the clock for the TIM peripherals (144 MHz)

      (#) CSS (Clock security system), once enable using the macro __HAL_RCC_CSS_ENABLE()
          and if a HSE clock failure occurs(HSE used directly or through PLL as System 
          clock source), the System clocks automatically switched to HSI and an interrupt
          is generated if enabled. The interrupt is linked to the Cortex-M4 NMI 
          (Non-Maskable Interrupt) exception vector.   

      (#) MCO (microcontroller clock output), used to output SYSCLK, HSI, HSE, LSI, LSE or PLL
          clock (divided by 2) output on pin (such as PA8 pin).

    [..] System, AHB and APB buses clocks configuration
      (#) Several clock sources can be used to drive the System clock (SYSCLK): HSI,
          HSE and PLL.
          The AHB clock (HCLK) is derived from System clock through configurable
          prescaler and used to clock the CPU, memory and peripherals mapped
          on AHB bus (DMA, GPIO...). APB1 (PCLK1) and APB2 (PCLK2) clocks are derived
          from AHB clock through configurable prescalers and used to clock
          the peripherals mapped on these buses. You can use
          "@ref HAL_RCC_GetSysClockFreq()" function to retrieve the frequencies of these clocks.

      (#) All the peripheral clocks are derived from the System clock (SYSCLK) except:
        (++) The FLASH program/erase clock  which is always HSI 8MHz clock.
        (++) The USB 48 MHz clock which is derived from the PLL VCO clock.
        (++) The USART clock which can be derived as well from HSI 8MHz, LSI or LSE.
        (++) The I2C clock which can be derived as well from HSI 8MHz clock.
        (++) The ADC clock which is derived from PLL output.
        (++) The RTC clock which is derived from the LSE, LSI or 1 MHz HSE_RTC
             (HSE divided by a programmable prescaler). The System clock (SYSCLK)
             frequency must be higher or equal to the RTC clock frequency.
        (++) IWDG clock which is always the LSI clock.

         (#) For the STM32F3xx devices, the maximum frequency of the SYSCLK, HCLK, PCLK1 and PCLK2 is 72 MHz,
             Depending on the SYSCLK frequency, the flash latency should be adapted accordingly.

         (#) After reset, the System clock source is the HSI (8 MHz) with 0 WS and
             prefetch is disabled.
  @endverbatim
  * @{
  */
  
/*
  Additional consideration on the SYSCLK based on Latency settings:
        +-----------------------------------------------+
        | Latency       | SYSCLK clock frequency (MHz)  |
        |---------------|-------------------------------|
        |0WS(1CPU cycle)|       0 < SYSCLK <= 24        |
        |---------------|-------------------------------|
        |1WS(2CPU cycle)|      24 < SYSCLK <= 48        |
        |---------------|-------------------------------|
        |2WS(3CPU cycle)|      48 < SYSCLK <= 72        |
        +-----------------------------------------------+
  */

/**
  * @brief  Resets the RCC clock configuration to the default reset state.
  * @note   The default reset state of the clock configuration is given below:
  *            - HSI ON and used as system clock source
  *            - HSE and PLL OFF
  *            - AHB, APB1 and APB2 prescaler set to 1.
  *            - CSS and MCO1 OFF
  *            - All interrupts disabled
  * @note   This function does not modify the configuration of the
  *            - Peripheral clocks
  *            - LSI, LSE and RTC clocks
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCC_DeInit(void)
{
  uint32_t tickstart = 0;

  /* Set HSION bit */
  SET_BIT(RCC->CR, RCC_CR_HSION);

  /* Insure HSIRDY bit is set before writing default HSITRIM value */
  /* Get start tick */
  tickstart = HAL_GetTick();

  /* Wait till HSI is ready */
  while(READ_BIT(RCC->CR, RCC_CR_HSIRDY) == RESET)
  {
    if((HAL_GetTick() - tickstart) > HSI_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* Set HSITRIM default value */
  MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, RCC_CR_HSITRIM_4);

  /* Reset SW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0] and MCOSEL[2:0] bits */
  CLEAR_BIT(RCC->CFGR, RCC_CFGR_SW | RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2 | RCC_CFGR_MCO);

  /* Insure HSI selected as system clock source */
  /* Get start tick */
  tickstart = HAL_GetTick();

  /* Wait till system clock source is ready */
  while(READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
  {
    if((HAL_GetTick() - tickstart) > CLOCKSWITCH_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* Update the SystemCoreClock global variable for HSI as system clock source */
  SystemCoreClock = HSI_VALUE;

  /* Configure the source of time base considering new system clock settings  */
  if(HAL_InitTick(uwTickPrio) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Reset HSEON, CSSON, PLLON bits */
  CLEAR_BIT(RCC->CR, RCC_CR_PLLON | RCC_CR_CSSON | RCC_CR_HSEON);

  /* Reset HSEBYP bit */
  CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);

  /* Insure PLLRDY is reset */
  /* Get start tick */
  tickstart = HAL_GetTick();
  while(READ_BIT(RCC->CR, RCC_CR_PLLRDY) != 0U)
  {
    if((HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* Reset CFGR register */
  CLEAR_REG(RCC->CFGR);

  /* Reset CFGR2 register */
  CLEAR_REG(RCC->CFGR2);

  /* Reset CFGR3 register */
  CLEAR_REG(RCC->CFGR3);

  /* Clear all interrupt flags */
  SET_BIT(RCC->CIR, RCC_CIR_LSIRDYC | RCC_CIR_LSERDYC | RCC_CIR_HSIRDYC | RCC_CIR_HSERDYC | RCC_CIR_PLLRDYC | RCC_CIR_CSSC);

  /* Disable all interrupts */
  CLEAR_REG(RCC->CIR);

  /* Reset all CSR flags */
  __HAL_RCC_CLEAR_RESET_FLAGS();

  return HAL_OK;
}

/**
  * @brief  Initializes the RCC Oscillators according to the specified parameters in the
  *         RCC_OscInitTypeDef.
  * @param  RCC_OscInitStruct pointer to an RCC_OscInitTypeDef structure that
  *         contains the configuration information for the RCC Oscillators.
  * @note   The PLL is not disabled when used as system clock.
  * @note   Transitions LSE Bypass to LSE On and LSE On to LSE Bypass are not
  *         supported by this macro. User should request a transition to LSE Off
  *         first and then LSE On or LSE Bypass.
  * @note   Transition HSE Bypass to HSE On and HSE On to HSE Bypass are not
  *         supported by this macro. User should request a transition to HSE Off
  *         first and then HSE On or HSE Bypass.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef  *RCC_OscInitStruct)
{
  uint32_t tickstart;
  uint32_t pll_config;
#if defined(RCC_CFGR_PLLSRC_HSI_PREDIV)
  uint32_t pll_config2;
#endif /* RCC_CFGR_PLLSRC_HSI_PREDIV */

  /* Check Null pointer */
  if(RCC_OscInitStruct == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_RCC_OSCILLATORTYPE(RCC_OscInitStruct->OscillatorType));

  /*------------------------------- HSE Configuration ------------------------*/ 
  if(((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_HSE) == RCC_OSCILLATORTYPE_HSE)
  {
    /* Check the parameters */
    assert_param(IS_RCC_HSE(RCC_OscInitStruct->HSEState));

    /* When the HSE is used as system clock or clock source for PLL in these cases it is not allowed to be disabled */
    if((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_HSE) 
       || ((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_PLLCLK) && (__HAL_RCC_GET_PLL_OSCSOURCE() == RCC_PLLSOURCE_HSE)))
    {
      if((__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) != RESET) && (RCC_OscInitStruct->HSEState == RCC_HSE_OFF))
      {
        return HAL_ERROR;
      }
    }
    else
    {
      /* Set the new HSE configuration ---------------------------------------*/
      __HAL_RCC_HSE_CONFIG(RCC_OscInitStruct->HSEState);
      
#if defined(RCC_CFGR_PLLSRC_HSI_DIV2)
      /* Configure the HSE predivision factor --------------------------------*/
      __HAL_RCC_HSE_PREDIV_CONFIG(RCC_OscInitStruct->HSEPredivValue);
#endif /* RCC_CFGR_PLLSRC_HSI_DIV2 */

       /* Check the HSE State */
      if(RCC_OscInitStruct->HSEState != RCC_HSE_OFF)
      {
        /* Get Start Tick */
        tickstart = HAL_GetTick();
        
        /* Wait till HSE is ready */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) == RESET)
        {
          if((HAL_GetTick() - tickstart ) > HSE_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
      else
      {
        /* Get Start Tick */
        tickstart = HAL_GetTick();
        
        /* Wait till HSE is disabled */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) != RESET)
        {
           if((HAL_GetTick() - tickstart ) > HSE_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
    }
  }
  /*----------------------------- HSI Configuration --------------------------*/ 
  if(((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_HSI) == RCC_OSCILLATORTYPE_HSI)
  {
    /* Check the parameters */
    assert_param(IS_RCC_HSI(RCC_OscInitStruct->HSIState));
    assert_param(IS_RCC_CALIBRATION_VALUE(RCC_OscInitStruct->HSICalibrationValue));
    
    /* Check if HSI is used as system clock or as PLL source when PLL is selected as system clock */ 
    if((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_HSI) 
       || ((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_PLLCLK) && (__HAL_RCC_GET_PLL_OSCSOURCE() == RCC_PLLSOURCE_HSI)))
    {
      /* When HSI is used as system clock it will not disabled */
      if((__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) != RESET) && (RCC_OscInitStruct->HSIState != RCC_HSI_ON))
      {
        return HAL_ERROR;
      }
      /* Otherwise, just the calibration is allowed */
      else
      {
        /* Adjusts the Internal High Speed oscillator (HSI) calibration value.*/
        __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(RCC_OscInitStruct->HSICalibrationValue);
      }
    }
    else
    {
      /* Check the HSI State */
      if(RCC_OscInitStruct->HSIState != RCC_HSI_OFF)
      {
       /* Enable the Internal High Speed oscillator (HSI). */
        __HAL_RCC_HSI_ENABLE();
        
        /* Get Start Tick */
        tickstart = HAL_GetTick();
        
        /* Wait till HSI is ready */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) == RESET)
        {
          if((HAL_GetTick() - tickstart ) > HSI_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
                
        /* Adjusts the Internal High Speed oscillator (HSI) calibration value.*/
        __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(RCC_OscInitStruct->HSICalibrationValue);
      }
      else
      {
        /* Disable the Internal High Speed oscillator (HSI). */
        __HAL_RCC_HSI_DISABLE();
        
        /* Get Start Tick */
        tickstart = HAL_GetTick();
        
        /* Wait till HSI is disabled */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) != RESET)
        {
          if((HAL_GetTick() - tickstart ) > HSI_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
    }
  }
  /*------------------------------ LSI Configuration -------------------------*/ 
  if(((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_LSI) == RCC_OSCILLATORTYPE_LSI)
  {
    /* Check the parameters */
    assert_param(IS_RCC_LSI(RCC_OscInitStruct->LSIState));
    
    /* Check the LSI State */
    if(RCC_OscInitStruct->LSIState != RCC_LSI_OFF)
    {
      /* Enable the Internal Low Speed oscillator (LSI). */
      __HAL_RCC_LSI_ENABLE();
      
      /* Get Start Tick */
      tickstart = HAL_GetTick();
      
      /* Wait till LSI is ready */  
      while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) == RESET)
      {
        if((HAL_GetTick() - tickstart ) > LSI_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }
    else
    {
      /* Disable the Internal Low Speed oscillator (LSI). */
      __HAL_RCC_LSI_DISABLE();
      
      /* Get Start Tick */
      tickstart = HAL_GetTick();
      
      /* Wait till LSI is disabled */  
      while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) != RESET)
      {
        if((HAL_GetTick() - tickstart ) > LSI_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }
  }
  /*------------------------------ LSE Configuration -------------------------*/ 
  if(((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_LSE) == RCC_OSCILLATORTYPE_LSE)
  {
    FlagStatus       pwrclkchanged = RESET;
    
    /* Check the parameters */
    assert_param(IS_RCC_LSE(RCC_OscInitStruct->LSEState));

    /* Update LSE configuration in Backup Domain control register    */
    /* Requires to enable write access to Backup Domain of necessary */
    if(__HAL_RCC_PWR_IS_CLK_DISABLED())
    {
      __HAL_RCC_PWR_CLK_ENABLE();
      pwrclkchanged = SET;
    }
    
    if(HAL_IS_BIT_CLR(PWR->CR, PWR_CR_DBP))
    {
      /* Enable write access to Backup domain */
      SET_BIT(PWR->CR, PWR_CR_DBP);
      
      /* Wait for Backup domain Write protection disable */
      tickstart = HAL_GetTick();

      while(HAL_IS_BIT_CLR(PWR->CR, PWR_CR_DBP))
      {
        if((HAL_GetTick() - tickstart) > RCC_DBP_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }

    /* Set the new LSE configuration -----------------------------------------*/
    __HAL_RCC_LSE_CONFIG(RCC_OscInitStruct->LSEState);
    /* Check the LSE State */
    if(RCC_OscInitStruct->LSEState != RCC_LSE_OFF)
    {
      /* Get Start Tick */
      tickstart = HAL_GetTick();
      
      /* Wait till LSE is ready */  
      while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET)
      {
        if((HAL_GetTick() - tickstart ) > RCC_LSE_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }
    else
    {
      /* Get Start Tick */
      tickstart = HAL_GetTick();
      
      /* Wait till LSE is disabled */  
      while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) != RESET)
      {
        if((HAL_GetTick() - tickstart ) > RCC_LSE_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }

    /* Require to disable power clock if necessary */
    if(pwrclkchanged == SET)
    {
      __HAL_RCC_PWR_CLK_DISABLE();
    }
  }

  /*-------------------------------- PLL Configuration -----------------------*/
  /* Check the parameters */
  assert_param(IS_RCC_PLL(RCC_OscInitStruct->PLL.PLLState));
  if ((RCC_OscInitStruct->PLL.PLLState) != RCC_PLL_NONE)
  {
    /* Check if the PLL is used as system clock or not */
    if(__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_PLLCLK)
    { 
      if((RCC_OscInitStruct->PLL.PLLState) == RCC_PLL_ON)
      {
        /* Check the parameters */
        assert_param(IS_RCC_PLLSOURCE(RCC_OscInitStruct->PLL.PLLSource));
        assert_param(IS_RCC_PLL_MUL(RCC_OscInitStruct->PLL.PLLMUL));
#if   defined(RCC_CFGR_PLLSRC_HSI_PREDIV)
        assert_param(IS_RCC_PREDIV(RCC_OscInitStruct->PLL.PREDIV));
#endif
  
        /* Disable the main PLL. */
        __HAL_RCC_PLL_DISABLE();
        
        /* Get Start Tick */
        tickstart = HAL_GetTick();
        
        /* Wait till PLL is disabled */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY)  != RESET)
        {
          if((HAL_GetTick() - tickstart ) > PLL_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }

#if defined(RCC_CFGR_PLLSRC_HSI_PREDIV)
        /* Configure the main PLL clock source, predivider and multiplication factor. */
        __HAL_RCC_PLL_CONFIG(RCC_OscInitStruct->PLL.PLLSource,
                             RCC_OscInitStruct->PLL.PREDIV,
                             RCC_OscInitStruct->PLL.PLLMUL);
#else
      /* Configure the main PLL clock source and multiplication factor. */
      __HAL_RCC_PLL_CONFIG(RCC_OscInitStruct->PLL.PLLSource,
                           RCC_OscInitStruct->PLL.PLLMUL);
#endif /* RCC_CFGR_PLLSRC_HSI_PREDIV */
        /* Enable the main PLL. */
        __HAL_RCC_PLL_ENABLE();
        
        /* Get Start Tick */
        tickstart = HAL_GetTick();
        
        /* Wait till PLL is ready */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY)  == RESET)
        {
          if((HAL_GetTick() - tickstart ) > PLL_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
      else
      {
        /* Disable the main PLL. */
        __HAL_RCC_PLL_DISABLE();
 
        /* Get Start Tick */
        tickstart = HAL_GetTick();
        
        /* Wait till PLL is disabled */  
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY)  != RESET)
        {
          if((HAL_GetTick() - tickstart ) > PLL_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
    }
    else
    {
      /* Check if there is a request to disable the PLL used as System clock source */
      if((RCC_OscInitStruct->PLL.PLLState) == RCC_PLL_OFF)
      {
        return HAL_ERROR;
      }
      else
      {
        /* Do not return HAL_ERROR if request repeats the current configuration */
        pll_config = RCC->CFGR;
#if defined(RCC_CFGR_PLLSRC_HSI_PREDIV)
        pll_config2 = RCC->CFGR2;
        if((READ_BIT(pll_config, RCC_CFGR_PLLSRC)   != RCC_OscInitStruct->PLL.PLLSource) ||      
           (READ_BIT(pll_config, RCC_CFGR_PLLMUL)   != RCC_OscInitStruct->PLL.PLLMUL)    ||      
           (READ_BIT(pll_config2, RCC_CFGR2_PREDIV)  != RCC_OscInitStruct->PLL.PREDIV))     
#else
        if((READ_BIT(pll_config, RCC_CFGR_PLLSRC)   != RCC_OscInitStruct->PLL.PLLSource) ||      
           (READ_BIT(pll_config, RCC_CFGR_PLLMUL)   != RCC_OscInitStruct->PLL.PLLMUL))
#endif
        {
          return HAL_ERROR;
        }
      }
    }
  }

  return HAL_OK;
}

/**
  * @brief  Initializes the CPU, AHB and APB buses clocks according to the specified 
  *         parameters in the RCC_ClkInitStruct.
  * @param  RCC_ClkInitStruct pointer to an RCC_OscInitTypeDef structure that
  *         contains the configuration information for the RCC peripheral.
  * @param  FLatency FLASH Latency                   
  *          The value of this parameter depend on device used within the same series
  * @note   The SystemCoreClock CMSIS variable is used to store System Clock Frequency 
  *         and updated by @ref HAL_RCC_GetHCLKFreq() function called within this function
  *
  * @note   The HSI is used (enabled by hardware) as system clock source after
  *         start-up from Reset, wake-up from STOP and STANDBY mode, or in case
  *         of failure of the HSE used directly or indirectly as system clock
  *         (if the Clock Security System CSS is enabled).
  *           
  * @note   A switch from one clock source to another occurs only if the target
  *         clock source is ready (clock stable after start-up delay or PLL locked). 
  *         If a clock source which is not yet ready is selected, the switch will
  *         occur when the clock source will be ready. 
  *         You can use @ref HAL_RCC_GetClockConfig() function to know which clock is
  *         currently used as system clock source.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef  *RCC_ClkInitStruct, uint32_t FLatency)
{
  uint32_t tickstart = 0U;

  /* Check Null pointer */
  if(RCC_ClkInitStruct == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_RCC_CLOCKTYPE(RCC_ClkInitStruct->ClockType));
  assert_param(IS_FLASH_LATENCY(FLatency));

  /* To correctly read data from FLASH memory, the number of wait states (LATENCY) 
  must be correctly programmed according to the frequency of the CPU clock 
    (HCLK) of the device. */

  /* Increasing the number of wait states because of higher CPU frequency */
  if(FLatency > __HAL_FLASH_GET_LATENCY())
  {    
    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    __HAL_FLASH_SET_LATENCY(FLatency);
    
    /* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    if(__HAL_FLASH_GET_LATENCY() != FLatency)
    {
      return HAL_ERROR;
    }
  }

  /*-------------------------- HCLK Configuration --------------------------*/
  if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_HCLK) == RCC_CLOCKTYPE_HCLK)
  {
    assert_param(IS_RCC_HCLK(RCC_ClkInitStruct->AHBCLKDivider));
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_ClkInitStruct->AHBCLKDivider);
  }

  /*------------------------- SYSCLK Configuration ---------------------------*/ 
  if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_SYSCLK) == RCC_CLOCKTYPE_SYSCLK)
  {    
    assert_param(IS_RCC_SYSCLKSOURCE(RCC_ClkInitStruct->SYSCLKSource));
    
    /* HSE is selected as System Clock Source */
    if(RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_HSE)
    {
      /* Check the HSE ready flag */  
      if(__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) == RESET)
      {
        return HAL_ERROR;
      }
    }
    /* PLL is selected as System Clock Source */
    else if(RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_PLLCLK)
    {
      /* Check the PLL ready flag */  
      if(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET)
      {
        return HAL_ERROR;
      }
    }
    /* HSI is selected as System Clock Source */
    else
    {
      /* Check the HSI ready flag */  
      if(__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) == RESET)
      {
        return HAL_ERROR;
      }
    }

    __HAL_RCC_SYSCLK_CONFIG(RCC_ClkInitStruct->SYSCLKSource);

    /* Get Start Tick */
    tickstart = HAL_GetTick();
    
    while (__HAL_RCC_GET_SYSCLK_SOURCE() != (RCC_ClkInitStruct->SYSCLKSource << RCC_CFGR_SWS_Pos))
    {
      if ((HAL_GetTick() - tickstart) > CLOCKSWITCH_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }
  }
  /* Decreasing the number of wait states because of lower CPU frequency */
  if(FLatency < __HAL_FLASH_GET_LATENCY())
  {    
    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    __HAL_FLASH_SET_LATENCY(FLatency);
    
    /* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    if(__HAL_FLASH_GET_LATENCY() != FLatency)
    {
      return HAL_ERROR;
    }
  }    

  /*-------------------------- PCLK1 Configuration ---------------------------*/ 
  if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK1) == RCC_CLOCKTYPE_PCLK1)
  {
    assert_param(IS_RCC_PCLK(RCC_ClkInitStruct->APB1CLKDivider));
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_ClkInitStruct->APB1CLKDivider);
  }
  
  /*-------------------------- PCLK2 Configuration ---------------------------*/ 
  if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK2) == RCC_CLOCKTYPE_PCLK2)
  {
    assert_param(IS_RCC_PCLK(RCC_ClkInitStruct->APB2CLKDivider));
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, ((RCC_ClkInitStruct->APB2CLKDivider) << 3U));
  }
 
  /* Update the SystemCoreClock global variable */
  SystemCoreClock = HAL_RCC_GetSysClockFreq() >> AHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE)>> RCC_CFGR_HPRE_BITNUMBER];

  /* Configure the source of time base considering new system clocks settings*/
  HAL_InitTick (uwTickPrio);
  
  return HAL_OK;
}

/**
  * @}
  */

/** @defgroup RCC_Exported_Functions_Group2 Peripheral Control functions
  *  @brief   RCC clocks control functions
  *
  @verbatim   
  ===============================================================================
                  ##### Peripheral Control functions #####
  ===============================================================================  
    [..]
    This subsection provides a set of functions allowing to control the RCC Clocks 
    frequencies.

  @endverbatim
  * @{
  */

#if defined(RCC_CFGR_MCOPRE)
/**
  * @brief  Selects the clock source to output on MCO pin.
  * @note   MCO pin should be configured in alternate function mode.
  * @param  RCC_MCOx specifies the output direction for the clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_MCO1 Clock source to output on MCO1 pin(PA8).
  * @param  RCC_MCOSource specifies the clock source to output.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_MCO1SOURCE_NOCLOCK     No clock selected
  *            @arg @ref RCC_MCO1SOURCE_SYSCLK      System Clock selected as MCO clock
  *            @arg @ref RCC_MCO1SOURCE_HSI         HSI selected as MCO clock
  *            @arg @ref RCC_MCO1SOURCE_HSE         HSE selected as MCO clock
  *            @arg @ref RCC_MCO1SOURCE_LSI         LSI selected as MCO clock
  *            @arg @ref RCC_MCO1SOURCE_LSE         LSE selected as MCO clock
  *            @arg @ref RCC_MCO1SOURCE_PLLCLK      PLLCLK selected as MCO clock
  *            @arg @ref RCC_MCO1SOURCE_PLLCLK_DIV2 PLLCLK Divided by 2 selected as MCO clock
  * @param  RCC_MCODiv specifies the MCO DIV.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_MCODIV_1   no division applied to MCO clock
  *            @arg @ref RCC_MCODIV_2   division by 2 applied to MCO clock
  *            @arg @ref RCC_MCODIV_4   division by 4 applied to MCO clock
  *            @arg @ref RCC_MCODIV_8   division by 8 applied to MCO clock
  *            @arg @ref RCC_MCODIV_16  division by 16 applied to MCO clock
  *            @arg @ref RCC_MCODIV_32  division by 32 applied to MCO clock
  *            @arg @ref RCC_MCODIV_64  division by 64 applied to MCO clock
  *            @arg @ref RCC_MCODIV_128 division by 128 applied to MCO clock
  * @retval None
  */
#else
/**
  * @brief  Selects the clock source to output on MCO pin.
  * @note   MCO pin should be configured in alternate function mode.
  * @param  RCC_MCOx specifies the output direction for the clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_MCO1 Clock source to output on MCO1 pin(PA8).
  * @param  RCC_MCOSource specifies the clock source to output.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_MCO1SOURCE_NOCLOCK     No clock selected as MCO clock
  *            @arg @ref RCC_MCO1SOURCE_SYSCLK      System clock selected as MCO clock
  *            @arg @ref RCC_MCO1SOURCE_HSI         HSI selected as MCO clock
  *            @arg @ref RCC_MCO1SOURCE_HSE         HSE selected as MCO clock
  *            @arg @ref RCC_MCO1SOURCE_LSI         LSI selected as MCO clock
  *            @arg @ref RCC_MCO1SOURCE_LSE         LSE selected as MCO clock
  *            @arg @ref RCC_MCO1SOURCE_PLLCLK_DIV2 PLLCLK Divided by 2 selected as MCO clock
  * @param  RCC_MCODiv specifies the MCO DIV.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_MCODIV_1 no division applied to MCO clock
  * @retval None
  */
#endif
void HAL_RCC_MCOConfig(uint32_t RCC_MCOx, uint32_t RCC_MCOSource, uint32_t RCC_MCODiv)
{
  GPIO_InitTypeDef gpio;

  /* Check the parameters */
  assert_param(IS_RCC_MCO(RCC_MCOx));
  assert_param(IS_RCC_MCODIV(RCC_MCODiv));
  assert_param(IS_RCC_MCO1SOURCE(RCC_MCOSource));
  
  /* Configure the MCO1 pin in alternate function mode */
  gpio.Mode      = GPIO_MODE_AF_PP;
  gpio.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio.Pull      = GPIO_NOPULL;
  gpio.Pin       = MCO1_PIN;
  gpio.Alternate = GPIO_AF0_MCO;

  /* MCO1 Clock Enable */
  MCO1_CLK_ENABLE();
  
  HAL_GPIO_Init(MCO1_GPIO_PORT, &gpio);
  
  /* Configure the MCO clock source */
  __HAL_RCC_MCO1_CONFIG(RCC_MCOSource, RCC_MCODiv);
}

/**
  * @brief  Enables the Clock Security System.
  * @note   If a failure is detected on the HSE oscillator clock, this oscillator
  *         is automatically disabled and an interrupt is generated to inform the
  *         software about the failure (Clock Security System Interrupt, CSSI),
  *         allowing the MCU to perform rescue operations. The CSSI is linked to 
  *         the Cortex-M4 NMI (Non-Maskable Interrupt) exception vector.  
  * @retval None
  */
void HAL_RCC_EnableCSS(void)
{
  *(__IO uint32_t *) RCC_CR_CSSON_BB = (uint32_t)ENABLE;
}

/**
  * @brief  Disables the Clock Security System.
  * @retval None
  */
void HAL_RCC_DisableCSS(void)
{
  *(__IO uint32_t *) RCC_CR_CSSON_BB = (uint32_t)DISABLE;
}

/**
  * @brief  Returns the SYSCLK frequency     
  * @note   The system frequency computed by this function is not the real 
  *         frequency in the chip. It is calculated based on the predefined 
  *         constant and the selected clock source:
  * @note     If SYSCLK source is HSI, function returns values based on HSI_VALUE(*)
  * @note     If SYSCLK source is HSE, function returns a value based on HSE_VALUE
  *           divided by PREDIV factor(**)
  * @note     If SYSCLK source is PLL, function returns a value based on HSE_VALUE
  *           divided by PREDIV factor(**) or HSI_VALUE(*) multiplied by the PLL factor.
  * @note     (*) HSI_VALUE is a constant defined in stm32f3xx_hal_conf.h file (default value
  *               8 MHz) but the real value may vary depending on the variations
  *               in voltage and temperature.
  * @note     (**) HSE_VALUE is a constant defined in stm32f3xx_hal_conf.h file (default value
  *                8 MHz), user has to ensure that HSE_VALUE is same as the real
  *                frequency of the crystal used. Otherwise, this function may
  *                have wrong result.
  *                  
  * @note   The result of this function could be not correct when using fractional
  *         value for HSE crystal.
  *           
  * @note   This function can be used by the user application to compute the 
  *         baud-rate for the communication peripherals or configure other parameters.
  *           
  * @note   Each time SYSCLK changes, this function must be called to update the
  *         right SYSCLK value. Otherwise, any configuration based on this function will be incorrect.
  *         
  * @retval SYSCLK frequency
  */
uint32_t HAL_RCC_GetSysClockFreq(void)
{
  uint32_t tmpreg = 0U, prediv = 0U, pllclk = 0U, pllmul = 0U;
  uint32_t sysclockfreq = 0U;
  
  tmpreg = RCC->CFGR;
  
  /* Get SYSCLK source -------------------------------------------------------*/
  switch (tmpreg & RCC_CFGR_SWS)
  {
    case RCC_SYSCLKSOURCE_STATUS_HSE:  /* HSE used as system clock */
    {
      sysclockfreq = HSE_VALUE;
      break;
    }
    case RCC_SYSCLKSOURCE_STATUS_PLLCLK:  /* PLL used as system clock */
    {
      pllmul = aPLLMULFactorTable[(uint32_t)(tmpreg & RCC_CFGR_PLLMUL) >> POSITION_VAL(RCC_CFGR_PLLMUL)];
      prediv = aPredivFactorTable[(uint32_t)(RCC->CFGR2 & RCC_CFGR2_PREDIV) >> POSITION_VAL(RCC_CFGR2_PREDIV)];
#if defined(RCC_CFGR_PLLSRC_HSI_DIV2)
      if ((tmpreg & RCC_CFGR_PLLSRC) != RCC_PLLSOURCE_HSI)
      {
        /* HSE used as PLL clock source : PLLCLK = HSE/PREDIV * PLLMUL */
        pllclk = (uint32_t)((uint64_t) HSE_VALUE / (uint64_t) (prediv)) * ((uint64_t) pllmul);
      }
      else
      {
        /* HSI used as PLL clock source : PLLCLK = HSI/2 * PLLMUL */
        pllclk = (uint32_t)((uint64_t) (HSI_VALUE >> 1U) * ((uint64_t) pllmul));
      }
#else
      if ((tmpreg & RCC_CFGR_PLLSRC_HSE_PREDIV) == RCC_CFGR_PLLSRC_HSE_PREDIV)
      {
        /* HSE used as PLL clock source : PLLCLK = HSE/PREDIV * PLLMUL */
        pllclk = (uint32_t)((uint64_t) HSE_VALUE / (uint64_t) (prediv)) * ((uint64_t) pllmul);
      }
      else
      {
        /* HSI used as PLL clock source : PLLCLK = HSI/PREDIV * PLLMUL */
        pllclk = (uint32_t)((uint64_t) HSI_VALUE / (uint64_t) (prediv)) * ((uint64_t) pllmul);
      }
#endif /* RCC_CFGR_PLLSRC_HSI_DIV2 */
      sysclockfreq = pllclk;
      break;
    }
    case RCC_SYSCLKSOURCE_STATUS_HSI:  /* HSI used as system clock source */
    default: /* HSI used as system clock */
    {
      sysclockfreq = HSI_VALUE;
      break;
    }
  }
  return sysclockfreq;
}

/**
  * @brief  Returns the HCLK frequency     
  * @note   Each time HCLK changes, this function must be called to update the
  *         right HCLK value. Otherwise, any configuration based on this function will be incorrect.
  * 
  * @note   The SystemCoreClock CMSIS variable is used to store System Clock Frequency 
  *         and updated within this function
  * @retval HCLK frequency
  */
uint32_t HAL_RCC_GetHCLKFreq(void)
{
  return SystemCoreClock;
}

/**
  * @brief  Returns the PCLK1 frequency     
  * @note   Each time PCLK1 changes, this function must be called to update the
  *         right PCLK1 value. Otherwise, any configuration based on this function will be incorrect.
  * @retval PCLK1 frequency
  */
uint32_t HAL_RCC_GetPCLK1Freq(void)
{
  /* Get HCLK source and Compute PCLK1 frequency ---------------------------*/
  return (HAL_RCC_GetHCLKFreq() >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_BITNUMBER]);
}    

/**
  * @brief  Returns the PCLK2 frequency     
  * @note   Each time PCLK2 changes, this function must be called to update the
  *         right PCLK2 value. Otherwise, any configuration based on this function will be incorrect.
  * @retval PCLK2 frequency
  */
uint32_t HAL_RCC_GetPCLK2Freq(void)
{
  /* Get HCLK source and Compute PCLK2 frequency ---------------------------*/
  return (HAL_RCC_GetHCLKFreq()>> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_BITNUMBER]);
} 

/**
  * @brief  Configures the RCC_OscInitStruct according to the internal 
  * RCC configuration registers.
  * @param  RCC_OscInitStruct pointer to an RCC_OscInitTypeDef structure that 
  * will be configured.
  * @retval None
  */
void HAL_RCC_GetOscConfig(RCC_OscInitTypeDef  *RCC_OscInitStruct)
{
  /* Check the parameters */
  assert_param(RCC_OscInitStruct != NULL);

  /* Set all possible values for the Oscillator type parameter ---------------*/
  RCC_OscInitStruct->OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI  \
                  | RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_LSI;


  /* Get the HSE configuration -----------------------------------------------*/
  if((RCC->CR &RCC_CR_HSEBYP) == RCC_CR_HSEBYP)
  {
    RCC_OscInitStruct->HSEState = RCC_HSE_BYPASS;
  }
  else if((RCC->CR &RCC_CR_HSEON) == RCC_CR_HSEON)
  {
    RCC_OscInitStruct->HSEState = RCC_HSE_ON;
  }
  else
  {
    RCC_OscInitStruct->HSEState = RCC_HSE_OFF;
  }
#if defined(RCC_CFGR_PLLSRC_HSI_DIV2)
  RCC_OscInitStruct->HSEPredivValue = __HAL_RCC_HSE_GET_PREDIV();
#endif

  /* Get the HSI configuration -----------------------------------------------*/
  if((RCC->CR &RCC_CR_HSION) == RCC_CR_HSION)
  {
    RCC_OscInitStruct->HSIState = RCC_HSI_ON;
  }
  else
  {
    RCC_OscInitStruct->HSIState = RCC_HSI_OFF;
  }
  
  RCC_OscInitStruct->HSICalibrationValue = (uint32_t)((RCC->CR & RCC_CR_HSITRIM) >> POSITION_VAL(RCC_CR_HSITRIM));
  
  /* Get the LSE configuration -----------------------------------------------*/
  if((RCC->BDCR &RCC_BDCR_LSEBYP) == RCC_BDCR_LSEBYP)
  {
    RCC_OscInitStruct->LSEState = RCC_LSE_BYPASS;
  }
  else if((RCC->BDCR &RCC_BDCR_LSEON) == RCC_BDCR_LSEON)
  {
    RCC_OscInitStruct->LSEState = RCC_LSE_ON;
  }
  else
  {
    RCC_OscInitStruct->LSEState = RCC_LSE_OFF;
  }
  
  /* Get the LSI configuration -----------------------------------------------*/
  if((RCC->CSR &RCC_CSR_LSION) == RCC_CSR_LSION)
  {
    RCC_OscInitStruct->LSIState = RCC_LSI_ON;
  }
  else
  {
    RCC_OscInitStruct->LSIState = RCC_LSI_OFF;
  }
  

  /* Get the PLL configuration -----------------------------------------------*/
  if((RCC->CR &RCC_CR_PLLON) == RCC_CR_PLLON)
  {
    RCC_OscInitStruct->PLL.PLLState = RCC_PLL_ON;
  }
  else
  {
    RCC_OscInitStruct->PLL.PLLState = RCC_PLL_OFF;
  }
  RCC_OscInitStruct->PLL.PLLSource = (uint32_t)(RCC->CFGR & RCC_CFGR_PLLSRC);
  RCC_OscInitStruct->PLL.PLLMUL = (uint32_t)(RCC->CFGR & RCC_CFGR_PLLMUL);
#if defined(RCC_CFGR_PLLSRC_HSI_PREDIV)
  RCC_OscInitStruct->PLL.PREDIV = (uint32_t)(RCC->CFGR2 & RCC_CFGR2_PREDIV);
#endif /* RCC_CFGR_PLLSRC_HSI_PREDIV */
}

/**
  * @brief  Get the RCC_ClkInitStruct according to the internal 
  * RCC configuration registers.
  * @param  RCC_ClkInitStruct pointer to an RCC_ClkInitTypeDef structure that 
  * contains the current clock configuration.
  * @param  pFLatency Pointer on the Flash Latency.
  * @retval None
  */
void HAL_RCC_GetClockConfig(RCC_ClkInitTypeDef  *RCC_ClkInitStruct, uint32_t *pFLatency)
{
  /* Check the parameters */
  assert_param(RCC_ClkInitStruct != NULL);
  assert_param(pFLatency != NULL);

  /* Set all possible values for the Clock type parameter --------------------*/
  RCC_ClkInitStruct->ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  
  /* Get the SYSCLK configuration --------------------------------------------*/ 
  RCC_ClkInitStruct->SYSCLKSource = (uint32_t)(RCC->CFGR & RCC_CFGR_SW);
  
  /* Get the HCLK configuration ----------------------------------------------*/ 
  RCC_ClkInitStruct->AHBCLKDivider = (uint32_t)(RCC->CFGR & RCC_CFGR_HPRE); 
  
  /* Get the APB1 configuration ----------------------------------------------*/ 
  RCC_ClkInitStruct->APB1CLKDivider = (uint32_t)(RCC->CFGR & RCC_CFGR_PPRE1);   
  
  /* Get the APB2 configuration ----------------------------------------------*/ 
  RCC_ClkInitStruct->APB2CLKDivider = (uint32_t)((RCC->CFGR & RCC_CFGR_PPRE2) >> 3U);
  
  /* Get the Flash Wait State (Latency) configuration ------------------------*/   
  *pFLatency = (uint32_t)(FLASH->ACR & FLASH_ACR_LATENCY); 
}

/**
  * @brief This function handles the RCC CSS interrupt request.
  * @note This API should be called under the NMI_Handler().
  * @retval None
  */
void HAL_RCC_NMI_IRQHandler(void)
{
  /* Check RCC CSSF flag  */
  if(__HAL_RCC_GET_IT(RCC_IT_CSS))
  {
    /* RCC Clock Security System interrupt user callback */
    HAL_RCC_CSSCallback();
    
    /* Clear RCC CSS pending bit */
    __HAL_RCC_CLEAR_IT(RCC_IT_CSS);
  }
}

/**
  * @brief  RCC Clock Security System interrupt callback
  * @retval none
  */
__weak void HAL_RCC_CSSCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
    the HAL_RCC_CSSCallback could be implemented in the user file
    */ 
}

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_RCC_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
