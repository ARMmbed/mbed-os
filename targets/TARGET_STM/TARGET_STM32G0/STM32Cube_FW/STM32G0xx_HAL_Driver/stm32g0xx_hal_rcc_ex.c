/**
  ******************************************************************************
  * @file    stm32g0xx_hal_rcc_ex.c
  * @author  MCD Application Team
  * @brief   Extended RCC HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities RCC extended peripheral:
  *           + Extended Peripheral Control functions
  *           + Extended Clock management functions
  *           + Extended Clock Recovery System Control functions
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

/** @addtogroup STM32G0xx_HAL_Driver
  * @{
  */

/** @defgroup RCCEx RCCEx
  * @brief RCC Extended HAL module driver
  * @{
  */

#ifdef HAL_RCC_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/** @defgroup RCCEx_Private_Constants RCCEx Private Constants
  * @{
  */
#define PLL_TIMEOUT_VALUE        100U /* 100 ms (minimum Tick + 1)  */

#define LSCO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define LSCO_GPIO_PORT        GPIOA
#define LSCO_PIN              GPIO_PIN_2
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup RCCEx_Exported_Functions RCCEx Exported Functions
  * @{
  */

/** @defgroup RCCEx_Exported_Functions_Group1 Extended Peripheral Control functions
  * @brief  Extended Peripheral Control functions
  *
@verbatim
 ===============================================================================
                ##### Extended Peripheral Control functions  #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the RCC Clocks
    frequencies.
    [..]
    (@) Important note: Care must be taken when @ref HAL_RCCEx_PeriphCLKConfig() is used to
        select the RTC clock source; in this case the Backup domain will be reset in
        order to modify the RTC Clock source, as consequence RTC registers (including
        the backup registers) and RCC_BDCR register are set to their reset values.

@endverbatim
  * @{
  */
/**
  * @brief  Initialize the RCC extended peripherals clocks according to the specified
  *         parameters in the @ref RCC_PeriphCLKInitTypeDef.
  * @param  PeriphClkInit  pointer to a @ref RCC_PeriphCLKInitTypeDef structure that
  *         contains a field PeriphClockSelection which can be a combination of the following values:
  *            @arg @ref RCC_PERIPHCLK_RTC     RTC peripheral clock
  *            @arg @ref RCC_PERIPHCLK_ADC     ADC peripheral clock
  *            @arg @ref RCC_PERIPHCLK_I2C1    I2C1 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_I2C2    I2C2 peripheral clock (2)
  *            @arg @ref RCC_PERIPHCLK_I2S1    I2S1 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_I2S2    I2S2 peripheral clock (1)
  *            @arg @ref RCC_PERIPHCLK_USART1  USART1 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_CEC     CEC peripheral clock     (1)
  *            @arg @ref RCC_PERIPHCLK_LPTIM1  LPTIM1 peripheral clock  (1)
  *            @arg @ref RCC_PERIPHCLK_LPTIM2  LPTIM2 peripheral clock  (1)
  *            @arg @ref RCC_PERIPHCLK_LPUART1 LPUART1 peripheral clock (1)
  *            @arg @ref RCC_PERIPHCLK_LPUART2 LPUART2 peripheral clock (1)
  *            @arg @ref RCC_PERIPHCLK_RNG     RNG peripheral clock     (1)
  *            @arg @ref RCC_PERIPHCLK_TIM1    TIM1 peripheral clock    (1)(2)
  *            @arg @ref RCC_PERIPHCLK_TIM15   TIM15 peripheral clock   (1)(2)
  *            @arg @ref RCC_PERIPHCLK_USART2  USART2 peripheral clock     (2)
  *            @arg @ref RCC_PERIPHCLK_USART3  USART3 peripheral clock     (2)
  *            @arg @ref RCC_PERIPHCLK_FDCAN   FDCAN peripheral clock   (1)
  *            @arg @ref RCC_PERIPHCLK_USB     USB peripheral clock     (1)
  *
  * @note   (1) Peripherals are not available on all devices
  * @note   (2) Peripherals clock selection is not available on all devices
  * @note   Care must be taken when @ref HAL_RCCEx_PeriphCLKConfig() is used to select
  *         the RTC clock source: in this case the access to Backup domain is enabled.
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)
{
  uint32_t tmpregister;
  uint32_t tickstart;
  HAL_StatusTypeDef ret    = HAL_OK;   /* Intermediate status */
  HAL_StatusTypeDef status = HAL_OK;   /* Final status */

  /* Check the parameters */
  assert_param(IS_RCC_PERIPHCLOCK(PeriphClkInit->PeriphClockSelection));

  /*-------------------------- RTC clock source configuration ----------------------*/
  if ((PeriphClkInit->PeriphClockSelection & RCC_PERIPHCLK_RTC) == RCC_PERIPHCLK_RTC)
  {
    FlagStatus       pwrclkchanged = RESET;

    /* Check for RTC Parameters used to output RTCCLK */
    assert_param(IS_RCC_RTCCLKSOURCE(PeriphClkInit->RTCClockSelection));

    /* Enable Power Clock */
    if (__HAL_RCC_PWR_IS_CLK_DISABLED())
    {
      __HAL_RCC_PWR_CLK_ENABLE();
      pwrclkchanged = SET;
    }

    /* Enable write access to Backup domain */
    SET_BIT(PWR->CR1, PWR_CR1_DBP);

    /* Wait for Backup domain Write protection disable */
    tickstart = HAL_GetTick();

    while ((PWR->CR1 & PWR_CR1_DBP) == 0U)
    {
      if ((HAL_GetTick() - tickstart) > RCC_DBP_TIMEOUT_VALUE)
      {
        ret = HAL_TIMEOUT;
        break;
      }
    }

    if (ret == HAL_OK)
    {
      /* Reset the Backup domain only if the RTC Clock source selection is modified from default */
      tmpregister = READ_BIT(RCC->BDCR, RCC_BDCR_RTCSEL);

      /* Reset the Backup domain only if the RTC Clock source selection is modified */
      if ((tmpregister != RCC_RTCCLKSOURCE_NONE) && (tmpregister != PeriphClkInit->RTCClockSelection))
      {
        /* Store the content of BDCR register before the reset of Backup Domain */
        tmpregister = READ_BIT(RCC->BDCR, ~(RCC_BDCR_RTCSEL));
        /* RTC Clock selection can be changed only if the Backup Domain is reset */
        __HAL_RCC_BACKUPRESET_FORCE();
        __HAL_RCC_BACKUPRESET_RELEASE();
        /* Restore the Content of BDCR register */
        RCC->BDCR = tmpregister;
      }

      /* Wait for LSE reactivation if LSE was enable prior to Backup Domain reset */
      if (HAL_IS_BIT_SET(tmpregister, RCC_BDCR_LSEON))
      {
        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till LSE is ready */
        while (READ_BIT(RCC->BDCR, RCC_BDCR_LSERDY) == 0U)
        {
          if ((HAL_GetTick() - tickstart) > RCC_LSE_TIMEOUT_VALUE)
          {
            ret = HAL_TIMEOUT;
            break;
          }
        }
      }

      if (ret == HAL_OK)
      {
        /* Apply new RTC clock source selection */
        __HAL_RCC_RTC_CONFIG(PeriphClkInit->RTCClockSelection);
      }
      else
      {
        /* set overall return value */
        status = ret;
      }
    }
    else
    {
      /* set overall return value */
      status = ret;
    }

    /* Restore clock configuration if changed */
    if (pwrclkchanged == SET)
    {
      __HAL_RCC_PWR_CLK_DISABLE();
    }
  }

  /*-------------------------- USART1 clock source configuration -------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART1) == RCC_PERIPHCLK_USART1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART1CLKSOURCE(PeriphClkInit->Usart1ClockSelection));

    /* Configure the USART1 clock source */
    __HAL_RCC_USART1_CONFIG(PeriphClkInit->Usart1ClockSelection);
  }

#if defined(RCC_CCIPR_USART2SEL)
  /*-------------------------- USART2 clock source configuration -------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART2) == RCC_PERIPHCLK_USART2)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART2CLKSOURCE(PeriphClkInit->Usart2ClockSelection));

    /* Configure the USART2 clock source */
    __HAL_RCC_USART2_CONFIG(PeriphClkInit->Usart2ClockSelection);
  }
#endif /* RCC_CCIPR_USART2SEL */

#if defined(RCC_CCIPR_USART3SEL)
  /*-------------------------- USART3 clock source configuration -------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART3) == RCC_PERIPHCLK_USART3)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART3CLKSOURCE(PeriphClkInit->Usart3ClockSelection));

    /* Configure the USART3 clock source */
    __HAL_RCC_USART3_CONFIG(PeriphClkInit->Usart3ClockSelection);
  }
#endif /* RCC_CCIPR_USART3SEL */

#if defined(LPUART1)
  /*-------------------------- LPUART1 clock source configuration ------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LPUART1) == RCC_PERIPHCLK_LPUART1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_LPUART1CLKSOURCE(PeriphClkInit->Lpuart1ClockSelection));

    /* Configure the LPUART1 clock source */
    __HAL_RCC_LPUART1_CONFIG(PeriphClkInit->Lpuart1ClockSelection);
  }
#endif /* LPUART1 */

#if defined(LPUART2)
  /*-------------------------- LPUART2 clock source configuration ------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LPUART2) == RCC_PERIPHCLK_LPUART2)
  {
    /* Check the parameters */
    assert_param(IS_RCC_LPUART2CLKSOURCE(PeriphClkInit->Lpuart2ClockSelection));

    /* Configure the LPUART clock source */
    __HAL_RCC_LPUART2_CONFIG(PeriphClkInit->Lpuart2ClockSelection);
  }
#endif /* LPUART2 */

#if defined(RCC_CCIPR_LPTIM1SEL)
  /*-------------------------- LPTIM1 clock source configuration -------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LPTIM1) == (RCC_PERIPHCLK_LPTIM1))
  {
    assert_param(IS_RCC_LPTIM1CLKSOURCE(PeriphClkInit->Lptim1ClockSelection));
    __HAL_RCC_LPTIM1_CONFIG(PeriphClkInit->Lptim1ClockSelection);
  }
#endif /* RCC_CCIPR_LPTIM1SEL */

#if defined(RCC_CCIPR_LPTIM2SEL)
  /*-------------------------- LPTIM2 clock source configuration -------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LPTIM2) == (RCC_PERIPHCLK_LPTIM2))
  {
    assert_param(IS_RCC_LPTIM2CLKSOURCE(PeriphClkInit->Lptim2ClockSelection));
    __HAL_RCC_LPTIM2_CONFIG(PeriphClkInit->Lptim2ClockSelection);
  }
#endif /* RCC_CCIPR_LPTIM2SEL */

  /*-------------------------- I2C1 clock source configuration ---------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2C1) == RCC_PERIPHCLK_I2C1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2C1CLKSOURCE(PeriphClkInit->I2c1ClockSelection));

    /* Configure the I2C1 clock source */
    __HAL_RCC_I2C1_CONFIG(PeriphClkInit->I2c1ClockSelection);
  }

#if defined(RCC_CCIPR_I2C2SEL)
  /*-------------------------- I2C2 clock source configuration ---------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2C2) == RCC_PERIPHCLK_I2C2)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2C2CLKSOURCE(PeriphClkInit->I2c2ClockSelection));

    /* Configure the I2C2 clock source */
    __HAL_RCC_I2C2_CONFIG(PeriphClkInit->I2c2ClockSelection);
  }
#endif /* (RCC_CCIPR_I2C2SEL */

#if defined(RNG)
  /*-------------------------- RNG clock source configuration ----------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_RNG) == (RCC_PERIPHCLK_RNG))
  {
    assert_param(IS_RCC_RNGCLKSOURCE(PeriphClkInit->RngClockSelection));
    __HAL_RCC_RNG_CONFIG(PeriphClkInit->RngClockSelection);

    if (PeriphClkInit->RngClockSelection == RCC_RNGCLKSOURCE_PLL)
    {
      /* Enable PLLQCLK output */
      __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLLQCLK);
    }
  }
#endif /* RNG */
  /*-------------------------- ADC clock source configuration ----------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_ADC) == RCC_PERIPHCLK_ADC)
  {
    /* Check the parameters */
    assert_param(IS_RCC_ADCCLKSOURCE(PeriphClkInit->AdcClockSelection));

    /* Configure the ADC interface clock source */
    __HAL_RCC_ADC_CONFIG(PeriphClkInit->AdcClockSelection);

    if (PeriphClkInit->AdcClockSelection == RCC_ADCCLKSOURCE_PLLADC)
    {
      /* Enable PLLPCLK output */
      __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLLPCLK);
    }
  }

#if defined(CEC)
  /*-------------------------- CEC clock source configuration ---------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_CEC) == RCC_PERIPHCLK_CEC)
  {
    /* Check the parameters */
    assert_param(IS_RCC_CECCLKSOURCE(PeriphClkInit->CecClockSelection));

    /* Configure the CEC clock source */
    __HAL_RCC_CEC_CONFIG(PeriphClkInit->CecClockSelection);
  }
#endif /* CEC */

#if defined(RCC_CCIPR_TIM1SEL)
  /*-------------------------- TIM1 clock source configuration ---------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_TIM1) == RCC_PERIPHCLK_TIM1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_TIM1CLKSOURCE(PeriphClkInit->Tim1ClockSelection));

    /* Configure the TIM1 clock source */
    __HAL_RCC_TIM1_CONFIG(PeriphClkInit->Tim1ClockSelection);

    if (PeriphClkInit->Tim1ClockSelection == RCC_TIM1CLKSOURCE_PLL)
    {
      /* Enable PLLQCLK output */
      __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLLQCLK);
    }
  }
#endif /* RCC_CCIPR_TIM1SEL */

#if defined(RCC_CCIPR_TIM15SEL)
  /*-------------------------- TIM15 clock source configuration ---------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_TIM15) == RCC_PERIPHCLK_TIM15)
  {
    /* Check the parameters */
    assert_param(IS_RCC_TIM15CLKSOURCE(PeriphClkInit->Tim15ClockSelection));

    /* Configure the TIM15 clock source */
    __HAL_RCC_TIM15_CONFIG(PeriphClkInit->Tim15ClockSelection);

    if (PeriphClkInit->Tim15ClockSelection == RCC_TIM15CLKSOURCE_PLL)
    {
      /* Enable PLLQCLK output */
      __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLLQCLK);
    }
  }
#endif /* RCC_CCIPR_TIM15SEL */

  /*-------------------------- I2S1 clock source configuration ---------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S1) == RCC_PERIPHCLK_I2S1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2S1CLKSOURCE(PeriphClkInit->I2s1ClockSelection));

    /* Configure the I2S1 clock source */
    __HAL_RCC_I2S1_CONFIG(PeriphClkInit->I2s1ClockSelection);

    if (PeriphClkInit->I2s1ClockSelection == RCC_I2S1CLKSOURCE_PLL)
    {
      /* Enable PLLPCLK output */
      __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLLPCLK);
    }
  }

#if defined(RCC_CCIPR2_I2S2SEL)
  /*-------------------------- I2S2 clock source configuration ---------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S2) == RCC_PERIPHCLK_I2S2)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2S2CLKSOURCE(PeriphClkInit->I2s2ClockSelection));

    /* Configure the I2S2 clock source */
    __HAL_RCC_I2S2_CONFIG(PeriphClkInit->I2s2ClockSelection);

    if (PeriphClkInit->I2s2ClockSelection == RCC_I2S2CLKSOURCE_PLL)
    {
      /* Enable PLLPCLK output */
      __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLLPCLK);
    }
  }
#endif /* RCC_CCIPR2_I2S2SEL */

#if defined(STM32G0C1xx) || defined(STM32G0B1xx)  || defined(STM32G0B0xx) 
  /*-------------------------- USB clock source configuration ---------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USB) == RCC_PERIPHCLK_USB)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USBCLKSOURCE(PeriphClkInit->UsbClockSelection));

    /* Configure the USB clock source */
    __HAL_RCC_USB_CONFIG(PeriphClkInit->UsbClockSelection);

    if (PeriphClkInit->UsbClockSelection == RCC_USBCLKSOURCE_PLL)
    {
      /* Enable PLLQCLK output */
      __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLLQCLK);
    }
  }
#endif /* STM32G0C1xx || STM32G0B1xx || STM32G0B0xx */

#if defined(FDCAN1) || defined(FDCAN2)
  /*-------------------------- FDCAN clock source configuration ---------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_FDCAN) == RCC_PERIPHCLK_FDCAN)
  {
    /* Check the parameters */
    assert_param(IS_RCC_FDCANCLKSOURCE(PeriphClkInit->FdcanClockSelection));

    /* Configure the FDCAN clock source */
    __HAL_RCC_FDCAN_CONFIG(PeriphClkInit->FdcanClockSelection);

    if (PeriphClkInit->FdcanClockSelection == RCC_FDCANCLKSOURCE_PLL)
    {
      /* Enable PLLQCLK output */
      __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLLQCLK);
    }
  }
#endif /* FDCAN1 || FDCAN2 */

  return status;
}

/**
  * @brief  Get the RCC_ClkInitStruct according to the internal RCC configuration registers.
  * @param  PeriphClkInit pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         returns the configuration information for the Extended Peripherals
  *         clocks: I2C1, I2S1, USART1, RTC, ADC,
  *         LPTIM1 (1), LPTIM2 (1), TIM1 (2), TIM15 (1)(2), USART2 (2), LPUART1 (1), CEC (1) and RNG (1)
  * @note (1) Peripheral is not available on all devices
  * @note (2) Peripheral clock selection is not available on all devices
  * @retval None
  */
void HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)
{
  /* Set all possible values for the extended clock type parameter------------*/
  PeriphClkInit->PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_I2C1 | RCC_PERIPHCLK_I2S1 | \
                                        RCC_PERIPHCLK_ADC     | RCC_PERIPHCLK_RTC ;

#if defined(RCC_CCIPR_LPTIM1SEL) && defined(RCC_CCIPR_LPTIM2SEL)
  PeriphClkInit->PeriphClockSelection |=  RCC_PERIPHCLK_LPTIM2  | RCC_PERIPHCLK_LPTIM1;
#endif /* RCC_CCIPR_LPTIM1SEL && RCC_CCIPR_LPTIM2SEL */
#if defined(RCC_CCIPR_RNGSEL)
  PeriphClkInit->PeriphClockSelection |=  RCC_PERIPHCLK_RNG;
#endif /* RCC_CCIPR_RNGSEL */
#if defined(RCC_CCIPR_LPUART1SEL)
  PeriphClkInit->PeriphClockSelection |=  RCC_PERIPHCLK_LPUART1;
#endif /* RCC_CCIPR_LPUART1SEL */
#if defined(RCC_CCIPR_LPUART2SEL)
  PeriphClkInit->PeriphClockSelection |=  RCC_PERIPHCLK_LPUART2;
#endif /* RCC_CCIPR_LPUART2SEL */
#if defined(RCC_CCIPR_CECSEL)
  PeriphClkInit->PeriphClockSelection |=  RCC_PERIPHCLK_CEC;
#endif /* RCC_CCIPR_CECSEL */
#if defined(RCC_CCIPR_TIM1SEL)
  PeriphClkInit->PeriphClockSelection |=  RCC_PERIPHCLK_TIM1;
#endif /* RCC_CCIPR_TIM1SEL */
#if defined(RCC_CCIPR_TIM15SEL)
  PeriphClkInit->PeriphClockSelection |=  RCC_PERIPHCLK_TIM15;
#endif /* RCC_CCIPR_TIM15SEL */
#if defined(RCC_CCIPR_USART2SEL)
  PeriphClkInit->PeriphClockSelection |=  RCC_PERIPHCLK_USART2;
#endif /* RCC_CCIPR_USART2SEL */
#if defined(RCC_CCIPR_USART3SEL)
  PeriphClkInit->PeriphClockSelection |=  RCC_PERIPHCLK_USART3;
#endif /* RCC_CCIPR_USART3SEL */
#if defined(RCC_CCIPR_I2C2SEL)
  PeriphClkInit->PeriphClockSelection |=  RCC_PERIPHCLK_I2C2;
#endif /* RCC_CCIPR_I2C2SEL */
#if defined(RCC_CCIPR2_I2S2SEL)
  PeriphClkInit->PeriphClockSelection |=  RCC_PERIPHCLK_I2S2;
#endif /* RCC_CCIPR2_I2S2SEL */
#if defined(RCC_CCIPR2_USBSEL)
  PeriphClkInit->PeriphClockSelection |=  RCC_PERIPHCLK_USB;
#endif /* RCC_CCIPR2_USBSEL */
#if defined(RCC_CCIPR2_FDCANSEL)
  PeriphClkInit->PeriphClockSelection |=  RCC_PERIPHCLK_FDCAN;
#endif /* RCC_CCIPR_FDCANSEL */
  /* Get the USART1 clock source ---------------------------------------------*/
  PeriphClkInit->Usart1ClockSelection  = __HAL_RCC_GET_USART1_SOURCE();
#if defined(RCC_CCIPR_USART2SEL)
  /* Get the USART2 clock source ---------------------------------------------*/
  PeriphClkInit->Usart2ClockSelection  = __HAL_RCC_GET_USART2_SOURCE();
#endif /* RCC_CCIPR_USART2SEL */
#if defined(RCC_CCIPR_USART3SEL)
  /* Get the USART3 clock source ---------------------------------------------*/
  PeriphClkInit->Usart3ClockSelection  = __HAL_RCC_GET_USART3_SOURCE();
#endif /* RCC_CCIPR_USART3SEL */
#if defined(RCC_CCIPR_LPUART1SEL)
  /* Get the LPUART1 clock source --------------------------------------------*/
  PeriphClkInit->Lpuart1ClockSelection = __HAL_RCC_GET_LPUART1_SOURCE();
#endif /* RCC_CCIPR_LPUART1SEL */
#if defined(RCC_CCIPR_LPUART2SEL)
  /* Get the LPUART2 clock source --------------------------------------------*/
  PeriphClkInit->Lpuart2ClockSelection = __HAL_RCC_GET_LPUART2_SOURCE();
#endif /* RCC_CCIPR_LPUART2SEL */
  /* Get the I2C1 clock source -----------------------------------------------*/
  PeriphClkInit->I2c1ClockSelection    = __HAL_RCC_GET_I2C1_SOURCE();
#if defined(RCC_CCIPR_I2C2SEL)
  /* Get the I2C2 clock source -----------------------------------------------*/
  PeriphClkInit->I2c2ClockSelection    = __HAL_RCC_GET_I2C2_SOURCE();
#endif /* RCC_CCIPR_I2C2SEL */
#if defined(RCC_CCIPR_LPTIM1SEL)
  /* Get the LPTIM1 clock source ---------------------------------------------*/
  PeriphClkInit->Lptim1ClockSelection  = __HAL_RCC_GET_LPTIM1_SOURCE();
#endif /* RCC_CCIPR_LPTIM1SEL */
#if defined(RCC_CCIPR_LPTIM2SEL)
  /* Get the LPTIM2 clock source ---------------------------------------------*/
  PeriphClkInit->Lptim2ClockSelection  = __HAL_RCC_GET_LPTIM2_SOURCE();
#endif /* RCC_CCIPR_LPTIM2SEL */
#if defined(RCC_CCIPR_TIM1SEL)
  /* Get the TIM1 clock source ---------------------------------------------*/
  PeriphClkInit->Tim1ClockSelection  = __HAL_RCC_GET_TIM1_SOURCE();
#endif /* RCC_CCIPR_TIM1SEL */
#if defined(RCC_CCIPR_TIM15SEL)
  /* Get the TIM15 clock source ---------------------------------------------*/
  PeriphClkInit->Tim15ClockSelection  = __HAL_RCC_GET_TIM15_SOURCE();
#endif /* RCC_CCIPR_TIM15SEL */
  /* Get the RTC clock source ------------------------------------------------*/
  PeriphClkInit->RTCClockSelection     = __HAL_RCC_GET_RTC_SOURCE();
#if defined(RCC_CCIPR_RNGSEL)
  /* Get the RNG clock source ------------------------------------------------*/
  PeriphClkInit->RngClockSelection     = __HAL_RCC_GET_RNG_SOURCE();
#endif  /* RCC_CCIPR_RNGSEL */
  /* Get the ADC clock source -----------------------------------------------*/
  PeriphClkInit->AdcClockSelection     = __HAL_RCC_GET_ADC_SOURCE();
#if defined(RCC_CCIPR_CECSEL)
  /* Get the CEC clock source -----------------------------------------------*/
  PeriphClkInit->CecClockSelection     = __HAL_RCC_GET_CEC_SOURCE();
#endif  /* RCC_CCIPR_CECSEL */
#if defined(RCC_CCIPR2_USBSEL)
  /* Get the USB clock source -----------------------------------------------*/
  PeriphClkInit->UsbClockSelection     = __HAL_RCC_GET_USB_SOURCE();
#endif  /* RCC_CCIPR2_USBSEL */
#if defined(RCC_CCIPR2_FDCANSEL)
  /* Get the FDCAN clock source -----------------------------------------------*/
  PeriphClkInit->FdcanClockSelection     = __HAL_RCC_GET_FDCAN_SOURCE();
#endif  /* RCC_CCIPR2_FDCANSEL */
  /* Get the I2S1 clock source -----------------------------------------------*/
  PeriphClkInit->I2s1ClockSelection    = __HAL_RCC_GET_I2S1_SOURCE();
#if defined(RCC_CCIPR2_I2S2SEL)
  /* Get the I2S2 clock source -----------------------------------------------*/
  PeriphClkInit->I2s2ClockSelection    = __HAL_RCC_GET_I2S2_SOURCE();
#endif /* RCC_CCIPR2_I2S2SEL */
}

/**
  * @brief  Return the peripheral clock frequency for peripherals with clock source from PLL
  * @note   Return 0 if peripheral clock identifier not managed by this API
  * @param  PeriphClk  Peripheral clock identifier
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_PERIPHCLK_RTC     RTC peripheral clock
  *            @arg @ref RCC_PERIPHCLK_ADC     ADC peripheral clock
  *            @arg @ref RCC_PERIPHCLK_I2C1    I2C1 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_I2C2    I2C2 peripheral clock (1)
  *            @arg @ref RCC_PERIPHCLK_I2S1    I2S1 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_I2S2    I2S2 peripheral clock (1)
  *            @arg @ref RCC_PERIPHCLK_USART1  USART1 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_USART2  USART2 peripheral clock (1)(2)
  *            @arg @ref RCC_PERIPHCLK_USART3  USART3 peripheral clock (1)
  *            @arg @ref RCC_PERIPHCLK_RNG     RNG peripheral clock    (1)
  *            @arg @ref RCC_PERIPHCLK_TIM15   TIM15 peripheral clock  (1)(2)
  *            @arg @ref RCC_PERIPHCLK_TIM1    TIM1 peripheral clock   (1)(2)
  *            @arg @ref RCC_PERIPHCLK_LPTIM1  LPTIM1 peripheral clock (1)
  *            @arg @ref RCC_PERIPHCLK_LPTIM2  LPTIM2 peripheral clock (1)
  *            @arg @ref RCC_PERIPHCLK_LPUART1 LPUART1 peripheral clock(1)
  *            @arg @ref RCC_PERIPHCLK_LPUART2 LPUART2 peripheral clock(1)
  *            @arg @ref RCC_PERIPHCLK_CEC     CEC peripheral clock    (1)
  *            @arg @ref RCC_PERIPHCLK_FDCAN    FDCAN peripheral clock (1)
  *            @arg @ref RCC_PERIPHCLK_USB      USB peripheral clock   (1)
  * @note   (1) Peripheral not available on all devices
  * @note   (2) Peripheral Clock configuration not available on all devices
  * @retval Frequency in Hz
  */
uint32_t HAL_RCCEx_GetPeriphCLKFreq(uint32_t PeriphClk)
{
  uint32_t frequency = 0U;
  uint32_t srcclk;
  uint32_t pllvco;
  uint32_t plln;
#if defined(RCC_CCIPR_RNGSEL)
  uint32_t rngclk;
  uint32_t rngdiv;
#endif /* RCC_CCIPR_RNGSEL */
  /* Check the parameters */
  assert_param(IS_RCC_PERIPHCLOCK(PeriphClk));

  if (PeriphClk == RCC_PERIPHCLK_RTC)
  {
    /* Get the current RTC source */
    srcclk = __HAL_RCC_GET_RTC_SOURCE();

    /* Check if LSE is ready and if RTC clock selection is LSE */
    if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_RTCCLKSOURCE_LSE))
    {
      frequency = LSE_VALUE;
    }
    /* Check if LSI is ready and if RTC clock selection is LSI */
    else if ((HAL_IS_BIT_SET(RCC->CSR, RCC_CSR_LSIRDY)) && (srcclk == RCC_RTCCLKSOURCE_LSI))
    {
      frequency = LSI_VALUE;
    }
    /* Check if HSE is ready  and if RTC clock selection is HSI_DIV32*/
    else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY)) && (srcclk == RCC_RTCCLKSOURCE_HSE_DIV32))
    {
      frequency = HSE_VALUE / 32U;
    }
    /* Clock not enabled for RTC*/
    else
    {
      /* Nothing to do as frequency already initialized to 0U */
    }
  }
  else
  {
    /* Other external peripheral clock source than RTC */

    /* Compute PLL clock input */
    if (__HAL_RCC_GET_PLL_OSCSOURCE() == RCC_PLLSOURCE_HSI)  /* HSI ? */
    {
      pllvco = HSI_VALUE;
    }
    else if (__HAL_RCC_GET_PLL_OSCSOURCE() == RCC_PLLSOURCE_HSE)  /* HSE ? */
    {
      pllvco = HSE_VALUE;
    }
    else /* No source */
    {
      pllvco = 0U;
    }

    /* f(PLL Source) / PLLM */
    pllvco = (pllvco / ((READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLM) >> RCC_PLLCFGR_PLLM_Pos) + 1U));

    switch (PeriphClk)
    {
#if defined(RCC_CCIPR_RNGSEL)
      case RCC_PERIPHCLK_RNG:

        srcclk = READ_BIT(RCC->CCIPR, RCC_CCIPR_RNGSEL);
        if (srcclk == RCC_RNGCLKSOURCE_HSI_DIV8)  /* HSI_DIV8 ? */
        {
          rngclk = HSI_VALUE / 8U;
        }
        else if (srcclk == RCC_RNGCLKSOURCE_PLL) /* PLL ? */
        {
          /* f(PLLQ) = f(VCO input) * PLLN / PLLQ */
          plln = READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos;
          rngclk = (pllvco * plln) / ((READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ) >> RCC_PLLCFGR_PLLQ_Pos) + 1U);
        }
        else if (srcclk == RCC_RNGCLKSOURCE_SYSCLK) /* SYSCLK ? */
        {
          rngclk = HAL_RCC_GetSysClockFreq();
        }
        else /* No clock source */
        {
          rngclk = 0U;
        }

        rngdiv = (1UL << ((READ_BIT(RCC->CCIPR, RCC_CCIPR_RNGDIV)) >> RCC_CCIPR_RNGDIV_Pos));
        frequency = (rngclk / rngdiv);

        break;
#endif  /* RCC_CCIPR_RNGSEL */
      case RCC_PERIPHCLK_USART1:
        /* Get the current USART1 source */
        srcclk = __HAL_RCC_GET_USART1_SOURCE();

        if (srcclk == RCC_USART1CLKSOURCE_PCLK1)            /* PCLK1 ? */
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else if (srcclk == RCC_USART1CLKSOURCE_SYSCLK)     /* SYSCLK ? */
        {
          frequency = HAL_RCC_GetSysClockFreq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_USART1CLKSOURCE_HSI))
        {
          frequency = HSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_USART1CLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        /* Clock not enabled for USART1 */
        else
        {
          /* Nothing to do as frequency already initialized to 0U */
        }
        break;
#if defined(RCC_CCIPR_USART2SEL)
      case RCC_PERIPHCLK_USART2:
        /* Get the current USART2 source */
        srcclk = __HAL_RCC_GET_USART2_SOURCE();

        if (srcclk == RCC_USART2CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else if (srcclk == RCC_USART2CLKSOURCE_SYSCLK)
        {
          frequency = HAL_RCC_GetSysClockFreq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_USART2CLKSOURCE_HSI))
        {
          frequency = HSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_USART2CLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        /* Clock not enabled for USART2 */
        else
        {
          /* Nothing to do as frequency already initialized to 0U */
        }
        break;
#endif /* RCC_CCIPR_USART2SEL */

#if defined(RCC_CCIPR_USART3SEL)
      case RCC_PERIPHCLK_USART3:
        /* Get the current USART3 source */
        srcclk = __HAL_RCC_GET_USART3_SOURCE();

        if (srcclk == RCC_USART3CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else if (srcclk == RCC_USART3CLKSOURCE_SYSCLK)
        {
          frequency = HAL_RCC_GetSysClockFreq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_USART3CLKSOURCE_HSI))
        {
          frequency = HSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_USART3CLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        /* Clock not enabled for USART3 */
        else
        {
          /* Nothing to do as frequency already initialized to 0U */
        }
        break;
#endif /* RCC_CCIPR_USART3SEL */

#if defined(RCC_CCIPR_CECSEL)
      case RCC_PERIPHCLK_CEC:
        /* Get the current CEC source */
        srcclk = __HAL_RCC_GET_CEC_SOURCE();

        if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_CECCLKSOURCE_HSI_DIV488))
        {
          frequency = (HSI_VALUE / 488U);
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_CECCLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        /* Clock not enabled for CEC */
        else
        {
          /* Nothing to do as frequency already initialized to 0U */
        }
        break;
#endif /* RCC_CCIPR_CECSEL */

#if defined(RCC_CCIPR_LPUART1SEL)
      case RCC_PERIPHCLK_LPUART1:
        /* Get the current LPUART1 source */
        srcclk = __HAL_RCC_GET_LPUART1_SOURCE();

        if (srcclk == RCC_LPUART1CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else if (srcclk == RCC_LPUART1CLKSOURCE_SYSCLK)
        {
          frequency = HAL_RCC_GetSysClockFreq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_LPUART1CLKSOURCE_HSI))
        {
          frequency = HSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_LPUART1CLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        /* Clock not enabled for LPUART1 */
        else
        {
          /* Nothing to do as frequency already initialized to 0U */
        }
        break;
#endif /* RCC_CCIPR_LPUART1SEL */

#if defined(RCC_CCIPR_LPUART2SEL)
      case RCC_PERIPHCLK_LPUART2:
        /* Get the current LPUART2 source */
        srcclk = __HAL_RCC_GET_LPUART2_SOURCE();

        if (srcclk == RCC_LPUART2CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else if (srcclk == RCC_LPUART2CLKSOURCE_SYSCLK)
        {
          frequency = HAL_RCC_GetSysClockFreq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_LPUART2CLKSOURCE_HSI))
        {
          frequency = HSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_LPUART2CLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        /* Clock not enabled for LPUART2 */
        else
        {
          /* Nothing to do as frequency already initialized to 0U */
        }
        break;
#endif /* RCC_CCIPR_LPUART2SEL */

      case RCC_PERIPHCLK_ADC:

        srcclk = __HAL_RCC_GET_ADC_SOURCE();

        if (srcclk == RCC_ADCCLKSOURCE_SYSCLK)
        {
          frequency = HAL_RCC_GetSysClockFreq();
        }
        else if (srcclk == RCC_ADCCLKSOURCE_HSI)
        {
          frequency = HSI_VALUE;
        }
        else if (srcclk == RCC_ADCCLKSOURCE_PLLADC)
        {
          if (__HAL_RCC_GET_PLLCLKOUT_CONFIG(RCC_PLLPCLK) != 0U)
          {
            /* f(PLLP) = f(VCO input) * PLLN / PLLP */
            plln = READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos;
            frequency = (pllvco * plln) / ((READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLP) >> RCC_PLLCFGR_PLLP_Pos) + 1U);
          }
        }
        /* Clock not enabled for ADC */
        else
        {
          /* Nothing to do as frequency already initialized to 0U */
        }
        break;

      case RCC_PERIPHCLK_I2C1:
        /* Get the current I2C1 source */
        srcclk = __HAL_RCC_GET_I2C1_SOURCE();

        if (srcclk == RCC_I2C1CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else if (srcclk == RCC_I2C1CLKSOURCE_SYSCLK)
        {
          frequency = HAL_RCC_GetSysClockFreq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_I2C1CLKSOURCE_HSI))
        {
          frequency = HSI_VALUE;
        }
        /* Clock not enabled for I2C1 */
        else
        {
          /* Nothing to do as frequency already initialized to 0U */
        }
        break;

#if defined(RCC_CCIPR_I2C2SEL)
      case RCC_PERIPHCLK_I2C2:
        /* Get the current I2C2 source */
        srcclk = __HAL_RCC_GET_I2C2_SOURCE();

        if (srcclk == RCC_I2C2CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else if (srcclk == RCC_I2C2CLKSOURCE_SYSCLK)
        {
          frequency = HAL_RCC_GetSysClockFreq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_I2C2CLKSOURCE_HSI))
        {
          frequency = HSI_VALUE;
        }
        /* Clock not enabled for I2C2 */
        else
        {
          /* Nothing to do as frequency already initialized to 0U */
        }
        break;
#endif /* RCC_CCIPR_I2C2SEL */

      case RCC_PERIPHCLK_I2S1:
        /* Get the current I2S1 source */
        srcclk = __HAL_RCC_GET_I2S1_SOURCE();

        if (srcclk == RCC_I2S1CLKSOURCE_PLL)
        {
          if (__HAL_RCC_GET_PLLCLKOUT_CONFIG(RCC_PLLPCLK) != 0U)
          {
            /* f(PLLP) = f(VCO input) * PLLN / PLLP */
            plln = READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos;
            frequency = (pllvco * plln) / ((READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLP) >> RCC_PLLCFGR_PLLP_Pos) + 1U);
          }
        }
        else if (srcclk == RCC_I2S1CLKSOURCE_SYSCLK)
        {
          frequency = HAL_RCC_GetSysClockFreq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_I2S1CLKSOURCE_HSI))
        {
          frequency = HSI_VALUE;
        }
        else if (srcclk == RCC_I2S1CLKSOURCE_EXT)
        {
          /* External clock used.*/
          frequency = EXTERNAL_I2S1_CLOCK_VALUE;
        }
        /* Clock not enabled for I2S1 */
        else
        {
          /* Nothing to do as frequency already initialized to 0U */
        }
        break;

#if defined(RCC_CCIPR2_I2S2SEL)
      case RCC_PERIPHCLK_I2S2:
        /* Get the current I2S2 source */
        srcclk = __HAL_RCC_GET_I2S2_SOURCE();

        if (srcclk == RCC_I2S2CLKSOURCE_PLL)
        {
          if (__HAL_RCC_GET_PLLCLKOUT_CONFIG(RCC_PLLPCLK) != 0U)
          {
            /* f(PLLP) = f(VCO input) * PLLN / PLLP */
            plln = READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos;
            frequency = (pllvco * plln) / ((READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLP) >> RCC_PLLCFGR_PLLP_Pos) + 1U);
          }
        }
        else if (srcclk == RCC_I2S2CLKSOURCE_SYSCLK)
        {
          frequency = HAL_RCC_GetSysClockFreq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_I2S2CLKSOURCE_HSI))
        {
          frequency = HSI_VALUE;
        }
        else if (srcclk == RCC_I2S2CLKSOURCE_EXT)
        {
          /* External clock used.*/
          frequency = EXTERNAL_I2S2_CLOCK_VALUE;
        }
        /* Clock not enabled for I2S2 */
        else
        {
          /* Nothing to do as frequency already initialized to 0U */
        }
        break;
#endif /* RCC_CCIPR2_I2S2SEL */

#if defined(RCC_CCIPR_LPTIM1SEL)
      case RCC_PERIPHCLK_LPTIM1:
        /* Get the current LPTIM1 source */
        srcclk = __HAL_RCC_GET_LPTIM1_SOURCE();

        if (srcclk == RCC_LPTIM1CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CSR, RCC_CSR_LSIRDY)) && (srcclk == RCC_LPTIM1CLKSOURCE_LSI))
        {
          frequency = LSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_LPTIM1CLKSOURCE_HSI))
        {
          frequency = HSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_LPTIM1CLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        /* Clock not enabled for LPTIM1 */
        else
        {
          /* Nothing to do as frequency already initialized to 0U */
        }
        break;
#endif /* RCC_CCIPR_LPTIM1SEL */

#if defined(RCC_CCIPR_LPTIM2SEL)
      case RCC_PERIPHCLK_LPTIM2:
        /* Get the current LPTIM2 source */
        srcclk = __HAL_RCC_GET_LPTIM2_SOURCE();

        if (srcclk == RCC_LPTIM2CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CSR, RCC_CSR_LSIRDY)) && (srcclk == RCC_LPTIM2CLKSOURCE_LSI))
        {
          frequency = LSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_LPTIM2CLKSOURCE_HSI))
        {
          frequency = HSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_LPTIM2CLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        /* Clock not enabled for LPTIM2 */
        else
        {
          /* Nothing to do as frequency already initialized to 0U */
        }
        break;
#endif /* RCC_CCIPR_LPTIM2SEL */

#if defined(RCC_CCIPR_TIM1SEL)
      case RCC_PERIPHCLK_TIM1:

        srcclk = READ_BIT(RCC->CCIPR, RCC_CCIPR_TIM1SEL);

        if (srcclk == RCC_TIM1CLKSOURCE_PLL) /* PLL ? */
        {
          if (__HAL_RCC_GET_PLLCLKOUT_CONFIG(RCC_PLLQCLK) != 0U)
          {
            /* f(PLLQ) = f(VCO input) * PLLN / PLLQ */
            plln = READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos;
            frequency = (pllvco * plln) / ((READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ) >> RCC_PLLCFGR_PLLQ_Pos) + 1U);
          }
        }
        else if (srcclk == RCC_TIM1CLKSOURCE_PCLK1) /* PCLK1 ? */
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else /* No clock source */
        {
          /* Nothing to do as frequency already initialized to 0U */
        }
        break;
#endif /* RCC_CCIPR_TIM1SEL */

#if defined(RCC_CCIPR_TIM15SEL)
      case RCC_PERIPHCLK_TIM15:

        srcclk = READ_BIT(RCC->CCIPR, RCC_CCIPR_TIM15SEL);

        if (srcclk == RCC_TIM15CLKSOURCE_PLL) /* PLL ? */
        {
          if (__HAL_RCC_GET_PLLCLKOUT_CONFIG(RCC_PLLQCLK) != 0U)
          {
            /* f(PLLQ) = f(VCO input) * PLLN / PLLQ */
            plln = READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos;
            frequency = (pllvco * plln) / ((READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ) >> RCC_PLLCFGR_PLLQ_Pos) + 1U);
          }
        }
        else if (srcclk == RCC_TIM15CLKSOURCE_PCLK1) /* PCLK1 ? */
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else /* No clock source */
        {
          /* Nothing to do as frequency already initialized to 0U */
        }
        break;
#endif /* RCC_CCIPR_TIM15SEL */

#if defined(RCC_CCIPR2_USBSEL)
      case RCC_PERIPHCLK_USB:

        srcclk = READ_BIT(RCC->CCIPR2, RCC_CCIPR2_USBSEL);

        if (srcclk == RCC_USBCLKSOURCE_PLL) /* PLL ? */
        {
          if (__HAL_RCC_GET_PLLCLKOUT_CONFIG(RCC_PLLQCLK) != 0U)
          {
            /* f(PLLQ) = f(VCO input) * PLLN / PLLQ */
            plln = READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos;
            frequency = (pllvco * plln) / ((READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ) >> RCC_PLLCFGR_PLLQ_Pos) + 1U);
          }
        }
#if defined(RCC_HSI48_SUPPORT)
        else if (srcclk == RCC_USBCLKSOURCE_HSI48) /* HSI48 ? */
        {
          if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSI48RDY)) && (srcclk == RCC_USBCLKSOURCE_HSI48))
          {
            frequency = HSI48_VALUE;
          }
        }
#endif /* RCC_HSI48_SUPPORT */
        else if (srcclk == RCC_USBCLKSOURCE_HSE)
        {
          if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY)) && (srcclk == RCC_USBCLKSOURCE_HSE))
          {
            frequency = HSE_VALUE;
          }
        }
        else /* No clock source */
        {
          /* Nothing to do as frequency already initialized to 0U */
        }
        break;
#endif /* RCC_CCIPR2_USBSEL */

#if defined(RCC_CCIPR2_FDCANSEL)
      case RCC_PERIPHCLK_FDCAN:

        srcclk = READ_BIT(RCC->CCIPR2, RCC_CCIPR2_FDCANSEL);

        if (srcclk == RCC_FDCANCLKSOURCE_PLL) /* PLL ? */
        {
          if (__HAL_RCC_GET_PLLCLKOUT_CONFIG(RCC_PLLQCLK) != 0U)
          {
            /* f(PLLQ) = f(VCO input) * PLLN / PLLQ */
            plln = READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos;
            frequency = (pllvco * plln) / ((READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ) >> RCC_PLLCFGR_PLLQ_Pos) + 1U);
          }
        }
        else if (srcclk == RCC_FDCANCLKSOURCE_PCLK1) /* PCLK1 ? */
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY)) && (srcclk == RCC_FDCANCLKSOURCE_HSE))
        {
          frequency = HSE_VALUE;
        }
        else /* No clock source */
        {
          /* Nothing to do as frequency already initialized to 0U */
        }
        break;
#endif /* RCC_CCIPR2_FDCANSEL */

      default:
        break;
    }
  }

  return (frequency);
}

/**
  * @}
  */

/** @defgroup RCCEx_Exported_Functions_Group2 Extended Clock management functions
  *  @brief  Extended Clock management functions
  *
@verbatim
 ===============================================================================
                ##### Extended clock management functions  #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the
    activation or deactivation of LSE CSS, Low speed clock output and
    clock after wake-up from STOP mode.
@endverbatim
  * @{
  */

/**
  * @brief  Select the Low Speed clock source to output on LSCO pin (PA2).
  * @param  LSCOSource  specifies the Low Speed clock source to output.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_LSCOSOURCE_LSI  LSI clock selected as LSCO source
  *            @arg @ref RCC_LSCOSOURCE_LSE  LSE clock selected as LSCO source
  * @retval None
  */
void HAL_RCCEx_EnableLSCO(uint32_t LSCOSource)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  FlagStatus       pwrclkchanged = RESET;
  FlagStatus       backupchanged = RESET;

  /* Check the parameters */
  assert_param(IS_RCC_LSCOSOURCE(LSCOSource));

  /* LSCO Pin Clock Enable */
  LSCO_CLK_ENABLE();

  /* Configure the LSCO pin in analog mode */
  GPIO_InitStruct.Pin = LSCO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LSCO_GPIO_PORT, &GPIO_InitStruct);

  /* Update LSCOSEL clock source in Backup Domain control register */
  if (__HAL_RCC_PWR_IS_CLK_DISABLED())
  {
    __HAL_RCC_PWR_CLK_ENABLE();
    pwrclkchanged = SET;
  }
  if (HAL_IS_BIT_CLR(PWR->CR1, PWR_CR1_DBP))
  {
    HAL_PWR_EnableBkUpAccess();
    backupchanged = SET;
  }

  MODIFY_REG(RCC->BDCR, RCC_BDCR_LSCOSEL | RCC_BDCR_LSCOEN, LSCOSource | RCC_BDCR_LSCOEN);

  if (backupchanged == SET)
  {
    HAL_PWR_DisableBkUpAccess();
  }
  if (pwrclkchanged == SET)
  {
    __HAL_RCC_PWR_CLK_DISABLE();
  }
}

/**
  * @brief  Disable the Low Speed clock output.
  * @retval None
  */
void HAL_RCCEx_DisableLSCO(void)
{
  FlagStatus       pwrclkchanged = RESET;
  FlagStatus       backupchanged = RESET;

  /* Update LSCOEN bit in Backup Domain control register */
  if (__HAL_RCC_PWR_IS_CLK_DISABLED())
  {
    __HAL_RCC_PWR_CLK_ENABLE();
    pwrclkchanged = SET;
  }
  if (HAL_IS_BIT_CLR(PWR->CR1, PWR_CR1_DBP))
  {
    /* Enable access to the backup domain */
    HAL_PWR_EnableBkUpAccess();
    backupchanged = SET;
  }

  CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSCOEN);

  /* Restore previous configuration */
  if (backupchanged == SET)
  {
    /* Disable access to the backup domain */
    HAL_PWR_DisableBkUpAccess();
  }
  if (pwrclkchanged == SET)
  {
    __HAL_RCC_PWR_CLK_DISABLE();
  }
}

/**
  * @}
  */

#if defined(CRS)

/** @defgroup RCCEx_Exported_Functions_Group3 Extended Clock Recovery System Control functions
  * @brief  Extended Clock Recovery System Control functions
  *
@verbatim
 ===============================================================================
                ##### Extended Clock Recovery System Control functions  #####
 ===============================================================================
    [..]
      For devices with Clock Recovery System feature (CRS), RCC Extension HAL driver can be used as follows:

      (#) In System clock config, HSI48 needs to be enabled

      (#) Enable CRS clock in IP MSP init which will use CRS functions

      (#) Call CRS functions as follows:
          (##) Prepare synchronization configuration necessary for HSI48 calibration
              (+++) Default values can be set for frequency Error Measurement (reload and error limit)
                        and also HSI48 oscillator smooth trimming.
              (+++) Macro __HAL_RCC_CRS_RELOADVALUE_CALCULATE can be also used to calculate
                        directly reload value with target and sychronization frequencies values
          (##) Call function HAL_RCCEx_CRSConfig which
              (+++) Resets CRS registers to their default values.
              (+++) Configures CRS registers with synchronization configuration
              (+++) Enables automatic calibration and frequency error counter feature
           Note: When using USB LPM (Link Power Management) and the device is in Sleep mode, the
           periodic USB SOF will not be generated by the host. No SYNC signal will therefore be
           provided to the CRS to calibrate the HSI48 on the run. To guarantee the required clock
           precision after waking up from Sleep mode, the LSE or reference clock on the GPIOs
           should be used as SYNC signal.

          (##) A polling function is provided to wait for complete synchronization
              (+++) Call function HAL_RCCEx_CRSWaitSynchronization()
              (+++) According to CRS status, user can decide to adjust again the calibration or continue
                        application if synchronization is OK

      (#) User can retrieve information related to synchronization in calling function
            HAL_RCCEx_CRSGetSynchronizationInfo()

      (#) Regarding synchronization status and synchronization information, user can try a new calibration
           in changing synchronization configuration and call again HAL_RCCEx_CRSConfig.
           Note: When the SYNC event is detected during the downcounting phase (before reaching the zero value),
           it means that the actual frequency is lower than the target (and so, that the TRIM value should be
           incremented), while when it is detected during the upcounting phase it means that the actual frequency
           is higher (and that the TRIM value should be decremented).

      (#) In interrupt mode, user can resort to the available macros (__HAL_RCC_CRS_XXX_IT). Interrupts will go
          through CRS Handler (CRS_IRQn/CRS_IRQHandler)
              (++) Call function HAL_RCCEx_CRSConfig()
              (++) Enable CRS_IRQn (thanks to NVIC functions)
              (++) Enable CRS interrupt (__HAL_RCC_CRS_ENABLE_IT)
              (++) Implement CRS status management in the following user callbacks called from
                   HAL_RCCEx_CRS_IRQHandler():
                   (+++) HAL_RCCEx_CRS_SyncOkCallback()
                   (+++) HAL_RCCEx_CRS_SyncWarnCallback()
                   (+++) HAL_RCCEx_CRS_ExpectedSyncCallback()
                   (+++) HAL_RCCEx_CRS_ErrorCallback()

      (#) To force a SYNC EVENT, user can use the function HAL_RCCEx_CRSSoftwareSynchronizationGenerate().
          This function can be called before calling HAL_RCCEx_CRSConfig (for instance in Systick handler)

@endverbatim
  * @{
  */

/**
  * @brief  Start automatic synchronization for polling mode
  * @param  pInit Pointer on RCC_CRSInitTypeDef structure
  * @retval None
  */
void HAL_RCCEx_CRSConfig(RCC_CRSInitTypeDef *pInit)
{
  uint32_t value;  /* no init needed */

  /* Check the parameters */
  assert_param(IS_RCC_CRS_SYNC_DIV(pInit->Prescaler));
  assert_param(IS_RCC_CRS_SYNC_SOURCE(pInit->Source));
  assert_param(IS_RCC_CRS_SYNC_POLARITY(pInit->Polarity));
  assert_param(IS_RCC_CRS_RELOADVALUE(pInit->ReloadValue));
  assert_param(IS_RCC_CRS_ERRORLIMIT(pInit->ErrorLimitValue));
  assert_param(IS_RCC_CRS_HSI48CALIBRATION(pInit->HSI48CalibrationValue));

  /* CONFIGURATION */

  /* Before configuration, reset CRS registers to their default values*/
  __HAL_RCC_CRS_FORCE_RESET();
  __HAL_RCC_CRS_RELEASE_RESET();

  /* Set the SYNCDIV[2:0] bits according to Prescaler value */
  /* Set the SYNCSRC[1:0] bits according to Source value */
  /* Set the SYNCSPOL bit according to Polarity value */
  value = (pInit->Prescaler | pInit->Source | pInit->Polarity);
  /* Set the RELOAD[15:0] bits according to ReloadValue value */
  value |= pInit->ReloadValue;
  /* Set the FELIM[7:0] bits according to ErrorLimitValue value */
  value |= (pInit->ErrorLimitValue << CRS_CFGR_FELIM_Pos);
  WRITE_REG(CRS->CFGR, value);

  /* Adjust HSI48 oscillator smooth trimming */
  /* Set the TRIM[6:0] bits according to RCC_CRS_HSI48CalibrationValue value */
  MODIFY_REG(CRS->CR, CRS_CR_TRIM, (pInit->HSI48CalibrationValue << CRS_CR_TRIM_Pos));

  /* START AUTOMATIC SYNCHRONIZATION*/

  /* Enable Automatic trimming & Frequency error counter */
  SET_BIT(CRS->CR, CRS_CR_AUTOTRIMEN | CRS_CR_CEN);
}

/**
  * @brief  Generate the software synchronization event
  * @retval None
  */
void HAL_RCCEx_CRSSoftwareSynchronizationGenerate(void)
{
  SET_BIT(CRS->CR, CRS_CR_SWSYNC);
}

/**
  * @brief  Return synchronization info
  * @param  pSynchroInfo Pointer on RCC_CRSSynchroInfoTypeDef structure
  * @retval None
  */
void HAL_RCCEx_CRSGetSynchronizationInfo(RCC_CRSSynchroInfoTypeDef *pSynchroInfo)
{
  /* Check the parameter */
  assert_param(pSynchroInfo != (void *)NULL);

  /* Get the reload value */
  pSynchroInfo->ReloadValue = (READ_BIT(CRS->CFGR, CRS_CFGR_RELOAD));

  /* Get HSI48 oscillator smooth trimming */
  pSynchroInfo->HSI48CalibrationValue = (READ_BIT(CRS->CR, CRS_CR_TRIM) >> CRS_CR_TRIM_Pos);

  /* Get Frequency error capture */
  pSynchroInfo->FreqErrorCapture = (READ_BIT(CRS->ISR, CRS_ISR_FECAP) >> CRS_ISR_FECAP_Pos);

  /* Get Frequency error direction */
  pSynchroInfo->FreqErrorDirection = (READ_BIT(CRS->ISR, CRS_ISR_FEDIR));
}

/**
  * @brief Wait for CRS Synchronization status.
  * @param Timeout  Duration of the timeout
  * @note  Timeout is based on the maximum time to receive a SYNC event based on synchronization
  *        frequency.
  * @note    If Timeout set to HAL_MAX_DELAY, HAL_TIMEOUT will be never returned.
  * @retval Combination of Synchronization status
  *          This parameter can be a combination of the following values:
  *            @arg @ref RCC_CRS_TIMEOUT
  *            @arg @ref RCC_CRS_SYNCOK
  *            @arg @ref RCC_CRS_SYNCWARN
  *            @arg @ref RCC_CRS_SYNCERR
  *            @arg @ref RCC_CRS_SYNCMISS
  *            @arg @ref RCC_CRS_TRIMOVF
  */
uint32_t HAL_RCCEx_CRSWaitSynchronization(uint32_t Timeout)
{
  uint32_t crsstatus = RCC_CRS_NONE;
  uint32_t tickstart;

  /* Get timeout */
  tickstart = HAL_GetTick();

  /* Wait for CRS flag or timeout detection */
  do
  {
    if (Timeout != HAL_MAX_DELAY)
    {
      if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))
      {
        crsstatus = RCC_CRS_TIMEOUT;
      }
    }
    /* Check CRS SYNCOK flag  */
    if (__HAL_RCC_CRS_GET_FLAG(RCC_CRS_FLAG_SYNCOK))
    {
      /* CRS SYNC event OK */
      crsstatus |= RCC_CRS_SYNCOK;

      /* Clear CRS SYNC event OK bit */
      __HAL_RCC_CRS_CLEAR_FLAG(RCC_CRS_FLAG_SYNCOK);
    }

    /* Check CRS SYNCWARN flag  */
    if (__HAL_RCC_CRS_GET_FLAG(RCC_CRS_FLAG_SYNCWARN))
    {
      /* CRS SYNC warning */
      crsstatus |= RCC_CRS_SYNCWARN;

      /* Clear CRS SYNCWARN bit */
      __HAL_RCC_CRS_CLEAR_FLAG(RCC_CRS_FLAG_SYNCWARN);
    }

    /* Check CRS TRIM overflow flag  */
    if (__HAL_RCC_CRS_GET_FLAG(RCC_CRS_FLAG_TRIMOVF))
    {
      /* CRS SYNC Error */
      crsstatus |= RCC_CRS_TRIMOVF;

      /* Clear CRS Error bit */
      __HAL_RCC_CRS_CLEAR_FLAG(RCC_CRS_FLAG_TRIMOVF);
    }

    /* Check CRS Error flag  */
    if (__HAL_RCC_CRS_GET_FLAG(RCC_CRS_FLAG_SYNCERR))
    {
      /* CRS SYNC Error */
      crsstatus |= RCC_CRS_SYNCERR;

      /* Clear CRS Error bit */
      __HAL_RCC_CRS_CLEAR_FLAG(RCC_CRS_FLAG_SYNCERR);
    }

    /* Check CRS SYNC Missed flag  */
    if (__HAL_RCC_CRS_GET_FLAG(RCC_CRS_FLAG_SYNCMISS))
    {
      /* CRS SYNC Missed */
      crsstatus |= RCC_CRS_SYNCMISS;

      /* Clear CRS SYNC Missed bit */
      __HAL_RCC_CRS_CLEAR_FLAG(RCC_CRS_FLAG_SYNCMISS);
    }

    /* Check CRS Expected SYNC flag  */
    if (__HAL_RCC_CRS_GET_FLAG(RCC_CRS_FLAG_ESYNC))
    {
      /* frequency error counter reached a zero value */
      __HAL_RCC_CRS_CLEAR_FLAG(RCC_CRS_FLAG_ESYNC);
    }
  }
  while (RCC_CRS_NONE == crsstatus);

  return crsstatus;
}

/**
  * @brief Handle the Clock Recovery System interrupt request.
  * @retval None
  */
void HAL_RCCEx_CRS_IRQHandler(void)
{
  uint32_t crserror = RCC_CRS_NONE;
  /* Get current IT flags and IT sources values */
  uint32_t itflags = READ_REG(CRS->ISR);
  uint32_t itsources = READ_REG(CRS->CR);

  /* Check CRS SYNCOK flag  */
  if (((itflags & RCC_CRS_FLAG_SYNCOK) != 0U) && ((itsources & RCC_CRS_IT_SYNCOK) != 0U))
  {
    /* Clear CRS SYNC event OK flag */
    WRITE_REG(CRS->ICR, CRS_ICR_SYNCOKC);

    /* user callback */
    HAL_RCCEx_CRS_SyncOkCallback();
  }
  /* Check CRS SYNCWARN flag  */
  else if (((itflags & RCC_CRS_FLAG_SYNCWARN) != 0U) && ((itsources & RCC_CRS_IT_SYNCWARN) != 0U))
  {
    /* Clear CRS SYNCWARN flag */
    WRITE_REG(CRS->ICR, CRS_ICR_SYNCWARNC);

    /* user callback */
    HAL_RCCEx_CRS_SyncWarnCallback();
  }
  /* Check CRS Expected SYNC flag  */
  else if (((itflags & RCC_CRS_FLAG_ESYNC) != 0U) && ((itsources & RCC_CRS_IT_ESYNC) != 0U))
  {
    /* frequency error counter reached a zero value */
    WRITE_REG(CRS->ICR, CRS_ICR_ESYNCC);

    /* user callback */
    HAL_RCCEx_CRS_ExpectedSyncCallback();
  }
  /* Check CRS Error flags  */
  else
  {
    if (((itflags & RCC_CRS_FLAG_ERR) != 0U) && ((itsources & RCC_CRS_IT_ERR) != 0U))
    {
      if ((itflags & RCC_CRS_FLAG_SYNCERR) != 0U)
      {
        crserror |= RCC_CRS_SYNCERR;
      }
      if ((itflags & RCC_CRS_FLAG_SYNCMISS) != 0U)
      {
        crserror |= RCC_CRS_SYNCMISS;
      }
      if ((itflags & RCC_CRS_FLAG_TRIMOVF) != 0U)
      {
        crserror |= RCC_CRS_TRIMOVF;
      }

      /* Clear CRS Error flags */
      WRITE_REG(CRS->ICR, CRS_ICR_ERRC);

      /* user error callback */
      HAL_RCCEx_CRS_ErrorCallback(crserror);
    }
  }
}

/**
  * @brief  RCCEx Clock Recovery System SYNCOK interrupt callback.
  * @retval none
  */
__weak void HAL_RCCEx_CRS_SyncOkCallback(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the @ref HAL_RCCEx_CRS_SyncOkCallback should be implemented in the user file
   */
}

/**
  * @brief  RCCEx Clock Recovery System SYNCWARN interrupt callback.
  * @retval none
  */
__weak void HAL_RCCEx_CRS_SyncWarnCallback(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the @ref HAL_RCCEx_CRS_SyncWarnCallback should be implemented in the user file
   */
}

/**
  * @brief  RCCEx Clock Recovery System Expected SYNC interrupt callback.
  * @retval none
  */
__weak void HAL_RCCEx_CRS_ExpectedSyncCallback(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the @ref HAL_RCCEx_CRS_ExpectedSyncCallback should be implemented in the user file
   */
}

/**
  * @brief  RCCEx Clock Recovery System Error interrupt callback.
  * @param  Error Combination of Error status.
  *         This parameter can be a combination of the following values:
  *           @arg @ref RCC_CRS_SYNCERR
  *           @arg @ref RCC_CRS_SYNCMISS
  *           @arg @ref RCC_CRS_TRIMOVF
  * @retval none
  */
__weak void HAL_RCCEx_CRS_ErrorCallback(uint32_t Error)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Error);

  /* NOTE : This function should not be modified, when the callback is needed,
            the @ref HAL_RCCEx_CRS_ErrorCallback should be implemented in the user file
   */
}

/**
  * @}
  */

#endif /* CRS */

/**
  * @}
  */


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
