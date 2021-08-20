/**
  ******************************************************************************
  * @file    stm32wlxx_ll_utils.c
  * @author  MCD Application Team
  * @brief   UTILS LL module driver.
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
#include "stm32wlxx_ll_utils.h"
#include "stm32wlxx_ll_rcc.h"
#include "stm32wlxx_ll_system.h"
#include "stm32wlxx_ll_pwr.h"
#ifdef  USE_FULL_ASSERT
#include "stm32_assert.h"
#else
#define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */

/** @addtogroup STM32WLxx_LL_Driver
  * @{
  */

/** @addtogroup UTILS_LL
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @addtogroup UTILS_LL_Private_Constants
  * @{
  */
#define UTILS_MAX_FREQUENCY_SCALE1  48000000U        /*!< Maximum frequency for system clock at power scale1, in Hz */
#define UTILS_MAX_FREQUENCY_SCALE2  16000000U        /*!< Maximum frequency for system clock at power scale2, in Hz */

/* Defines used for PLL range */
#define UTILS_PLLVCO_INPUT_MIN        2660000U       /*!< Frequency min for PLLVCO input, in Hz   */
#define UTILS_PLLVCO_INPUT_MAX       16000000U       /*!< Frequency max for PLLVCO input, in Hz   */
#define UTILS_PLLVCO_OUTPUT_MIN      96000000U       /*!< Frequency min for PLLVCO output, in Hz  */
#define UTILS_PLLVCO_OUTPUT_MAX     344000000U       /*!< Frequency max for PLLVCO output, in Hz  */

/* Defines used for HCLK2 frequency check */
#define UTILS_HCLK2_MAX             48000000U        /*!< HCLK2 frequency maximum at 48MHz */


/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @addtogroup UTILS_LL_Private_Macros
  * @{
  */
#define IS_LL_UTILS_SYSCLK_DIV(__VALUE__) (((__VALUE__) == LL_RCC_SYSCLK_DIV_1)   \
                                           || ((__VALUE__) == LL_RCC_SYSCLK_DIV_2)   \
                                           || ((__VALUE__) == LL_RCC_SYSCLK_DIV_3)   \
                                           || ((__VALUE__) == LL_RCC_SYSCLK_DIV_4)   \
                                           || ((__VALUE__) == LL_RCC_SYSCLK_DIV_5)   \
                                           || ((__VALUE__) == LL_RCC_SYSCLK_DIV_6)   \
                                           || ((__VALUE__) == LL_RCC_SYSCLK_DIV_8)   \
                                           || ((__VALUE__) == LL_RCC_SYSCLK_DIV_10)  \
                                           || ((__VALUE__) == LL_RCC_SYSCLK_DIV_16)  \
                                           || ((__VALUE__) == LL_RCC_SYSCLK_DIV_32)  \
                                           || ((__VALUE__) == LL_RCC_SYSCLK_DIV_64)  \
                                           || ((__VALUE__) == LL_RCC_SYSCLK_DIV_128) \
                                           || ((__VALUE__) == LL_RCC_SYSCLK_DIV_256) \
                                           || ((__VALUE__) == LL_RCC_SYSCLK_DIV_512))

#define IS_LL_UTILS_APB1_DIV(__VALUE__) (((__VALUE__) == LL_RCC_APB1_DIV_1) \
                                         || ((__VALUE__) == LL_RCC_APB1_DIV_2) \
                                         || ((__VALUE__) == LL_RCC_APB1_DIV_4) \
                                         || ((__VALUE__) == LL_RCC_APB1_DIV_8) \
                                         || ((__VALUE__) == LL_RCC_APB1_DIV_16))

#define IS_LL_UTILS_APB2_DIV(__VALUE__) (((__VALUE__) == LL_RCC_APB2_DIV_1) \
                                         || ((__VALUE__) == LL_RCC_APB2_DIV_2) \
                                         || ((__VALUE__) == LL_RCC_APB2_DIV_4) \
                                         || ((__VALUE__) == LL_RCC_APB2_DIV_8) \
                                         || ((__VALUE__) == LL_RCC_APB2_DIV_16))

#define IS_LL_UTILS_PLLM_VALUE(__VALUE__) (((__VALUE__) == LL_RCC_PLLM_DIV_1) \
                                           || ((__VALUE__) == LL_RCC_PLLM_DIV_2) \
                                           || ((__VALUE__) == LL_RCC_PLLM_DIV_3) \
                                           || ((__VALUE__) == LL_RCC_PLLM_DIV_4) \
                                           || ((__VALUE__) == LL_RCC_PLLM_DIV_5) \
                                           || ((__VALUE__) == LL_RCC_PLLM_DIV_6) \
                                           || ((__VALUE__) == LL_RCC_PLLM_DIV_7) \
                                           || ((__VALUE__) == LL_RCC_PLLM_DIV_8))

#define IS_LL_UTILS_PLLN_VALUE(__VALUE__) ((6U <= (__VALUE__)) && ((__VALUE__) <= 127U))

#define IS_LL_UTILS_PLLR_VALUE(__VALUE__) (((__VALUE__) == LL_RCC_PLLR_DIV_2) \
                                           || ((__VALUE__) == LL_RCC_PLLR_DIV_3) \
                                           || ((__VALUE__) == LL_RCC_PLLR_DIV_4) \
                                           || ((__VALUE__) == LL_RCC_PLLR_DIV_5) \
                                           || ((__VALUE__) == LL_RCC_PLLR_DIV_6) \
                                           || ((__VALUE__) == LL_RCC_PLLR_DIV_7) \
                                           || ((__VALUE__) == LL_RCC_PLLR_DIV_8))

#define IS_LL_UTILS_PLLVCO_INPUT(__VALUE__)  ((UTILS_PLLVCO_INPUT_MIN <= (__VALUE__))\
                                              && ((__VALUE__) <= UTILS_PLLVCO_INPUT_MAX))

#define IS_LL_UTILS_PLLVCO_OUTPUT(__VALUE__) ((UTILS_PLLVCO_OUTPUT_MIN <= (__VALUE__))\
                                              && ((__VALUE__) <= UTILS_PLLVCO_OUTPUT_MAX))

#define IS_LL_UTILS_PLL_FREQUENCY(__VALUE__) ((LL_PWR_GetRegulVoltageScaling() == LL_PWR_REGU_VOLTAGE_SCALE1) ? \
                                              ((__VALUE__) <= UTILS_MAX_FREQUENCY_SCALE1) : \
                                              ((__VALUE__) <= UTILS_MAX_FREQUENCY_SCALE2))

#define COUNTOF(a)   (sizeof(a) / sizeof(*(a)))
  /**
    * @}
    */
  /* Private function prototypes -----------------------------------------------*/
  /** @defgroup UTILS_LL_Private_Functions UTILS Private functions
    * @{
    */
  static uint32_t    UTILS_GetPLLOutputFrequency(uint32_t PLL_InputFrequency,
                                                 LL_UTILS_PLLInitTypeDef * UTILS_PLLInitStruct);
  static ErrorStatus UTILS_EnablePLLAndSwitchSystem(uint32_t SYSCLK_Frequency,
                                                    LL_UTILS_ClkInitTypeDef * UTILS_ClkInitStruct);
  static ErrorStatus UTILS_PLL_IsBusy(void);

  /**
    * @}
    */

  /* Exported functions --------------------------------------------------------*/
  /** @addtogroup UTILS_LL_Exported_Functions
    * @{
    */

  /** @addtogroup UTILS_LL_EF_DELAY
    * @{
    */

#if defined(CORE_CM0PLUS)
  /**
    * @brief  This function configures the Cortex-M SysTick source to have 1ms time base.
    * @note   When a RTOS is used, it is recommended to avoid changing the Systick
    *         configuration by calling this function, for a delay use rather osDelay RTOS service.
    * @param  HCLKFrequency HCLK frequency in Hz
    * @note   HCLK frequency can be calculated thanks to RCC helper macro or function @ref LL_RCC_GetSystemClocksFreq
    *         (HCLK2_Frequency field)
    * @retval None
    */
#else
  /**
    * @brief  This function configures the Cortex-M SysTick source to have 1ms time base.
    * @note   When a RTOS is used, it is recommended to avoid changing the Systick
    *         configuration by calling this function, for a delay use rather osDelay RTOS service.
    * @param  HCLKFrequency HCLK frequency in Hz
    * @note   HCLK frequency can be calculated thanks to RCC helper macro or function @ref LL_RCC_GetSystemClocksFreq
    *         (HCLK1_Frequency field)
    * @retval None
    */
#endif /* CORE_CM0PLUS */
  void LL_Init1msTick(uint32_t HCLKFrequency)
{
  /* Use frequency provided in argument */
  LL_InitTick(HCLKFrequency, 1000);
}


/**
  * @brief  This function provides accurate delay (in milliseconds) based
  *         on SysTick counter flag
  * @note   When a RTOS is used, it is recommended to avoid using blocking delay
  *         and use rather osDelay service.
  * @note   To respect 1ms timebase, user should call @ref LL_Init1msTick function which
  *         will configure Systick to 1ms
  * @param  Delay specifies the delay time length, in milliseconds.
  * @retval None
  */
void LL_mDelay(uint32_t Delay)
{
  __IO uint32_t  tmp = SysTick->CTRL;  /* Clear the COUNTFLAG first */
  uint32_t tmpDelay;
  /* Add this code to indicate that local variable is not used */
  ((void)tmp);
  tmpDelay  = Delay;
  /* Add a period to guaranty minimum wait */
  if (tmpDelay  < LL_MAX_DELAY)
  {
    tmpDelay ++;
  }

  while (tmpDelay  != 0U)
  {
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
    {
      tmpDelay --;
    }
  }
}

/**
  * @}
  */

/** @addtogroup UTILS_EF_SYSTEM
  *  @brief    System Configuration functions
  *
  @verbatim
 ===============================================================================
           ##### System Configuration functions #####
 ===============================================================================
    [..]
         System, HCLK1, HCLK2, HCLK3 and APB buses clocks configuration

         (+) The maximum frequency of the SYSCLK, HCLK1, HCLK3, PCLK1 and PCLK2
             is 480000000 Hz.

  @endverbatim
  @internal
             Depending on the device voltage range, the maximum frequency should be
             adapted accordingly:
             (++) HCLK3 clock frequency for STM32WL55xx device
             (++) +--------------------------------------------------------+
             (++) | Latency         |     HCLK3 clock frequency (MHz)      |
             (++) |                 |--------------------------------------|
             (++) |                 |  voltage range 1  | voltage range 2  |
             (++) |                 |       1.2 V       |     1.0 V        |
             (++) |-----------------|-------------------|------------------|
             (++) |0WS(1 CPU cycles)|   0 < HCLK3 <= 18 |  0 < HCLK3 <= 6  |
             (++) |-----------------|-------------------|------------------|
             (++) |1WS(2 CPU cycles)|  18 < HCLK3 <= 36 |  6 < HCLK3 <= 12 |
             (++) |-----------------|-------------------|------------------|
             (++) |2WS(3 CPU cycles)|  36 < HCLK3 <= 48 | 12 < HCLK3 <= 16|
             (++) +--------------------------------------------------------+
  @endinternal
  * @{
  */

#if defined(CORE_CM0PLUS)
/**
  * @brief  This function sets directly SystemCoreClock CMSIS variable.
  * @note   Variable can be calculated also through SystemCoreClockUpdate function.
  * @param  HCLKFrequency HCLK frequency in Hz (can be calculated thanks to RCC helper macro or function
            @ref LL_RCC_GetSystemClocksFreq (HCLK2_Frequency field))
  * @retval None
  */
#else
/**
  * @brief  This function sets directly SystemCoreClock CMSIS variable.
  * @note   Variable can be calculated also through SystemCoreClockUpdate function.
  * @param  HCLKFrequency HCLK frequency in Hz (can be calculated thanks to RCC helper macro or function
            @ref LL_RCC_GetSystemClocksFreq (HCLK1_Frequency field))
  * @retval None
  */
#endif /* CORE_CM0PLUS */
void LL_SetSystemCoreClock(uint32_t HCLKFrequency)
{
  /* HCLK clock frequency */
  SystemCoreClock = HCLKFrequency;
}


/**
  * @brief  Update number of Flash wait states in line with new frequency and current
            voltage range.
  * @param  HCLK3_Frequency  HCLK3 frequency
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Latency has been modified
  *          - ERROR: Latency cannot be modified
  */
ErrorStatus LL_SetFlashLatency(uint32_t HCLK3_Frequency)
{
  uint32_t timeout;
  uint32_t getlatency;
  uint32_t latency;
  uint8_t index;
  ErrorStatus status = ERROR;

  /* Array used for FLASH latency according to HCLK3 Frequency */
  /* Flash Clock source (HCLK3) range in MHz with a VCORE is range1 */
  const uint32_t UTILS_CLK_SRC_RANGE_VOS1[] = {18000000UL, 36000000UL, UTILS_MAX_FREQUENCY_SCALE1};

  /* Flash Clock source (HCLK3) range in MHz with a VCORE is range2 */
  const uint32_t UTILS_CLK_SRC_RANGE_VOS2[] = {6000000U, 12000000U, UTILS_MAX_FREQUENCY_SCALE2};

  /* Flash Latency range */
  const uint32_t UTILS_LATENCY_RANGE[] = {LL_FLASH_LATENCY_0, LL_FLASH_LATENCY_1, LL_FLASH_LATENCY_2};

  /* Frequency cannot be equal to 0 */
  if (HCLK3_Frequency != 0U)
  {
    if (LL_PWR_GetRegulVoltageScaling() == LL_PWR_REGU_VOLTAGE_SCALE1)
    {
      /* Frequency cannot be greater than a defined max clock */
      if (HCLK3_Frequency <= UTILS_MAX_FREQUENCY_SCALE1)
      {
        for (index = 0; index < COUNTOF(UTILS_CLK_SRC_RANGE_VOS1); index++)
        {
          if (HCLK3_Frequency <= UTILS_CLK_SRC_RANGE_VOS1[index])
          {
            latency = UTILS_LATENCY_RANGE[index];
            status = SUCCESS;
            break;
          }
        }
      }
    }
    else /* SCALE2 */
    {
      /* Frequency cannot be greater than a defined max clock */
      if (HCLK3_Frequency <= UTILS_MAX_FREQUENCY_SCALE2)
      {
        for (index = 0; index < COUNTOF(UTILS_CLK_SRC_RANGE_VOS2); index++)
        {
          if (HCLK3_Frequency <= UTILS_CLK_SRC_RANGE_VOS2[index])
          {
            latency = UTILS_LATENCY_RANGE[index];
            status = SUCCESS;
            break;
          }
        }
      }
    }

    if (status != ERROR)
    {
      LL_FLASH_SetLatency(latency);

      /* Check that the new number of wait states is taken into account to access the Flash
         memory by reading the FLASH_ACR register */
      timeout = 2U;
      do
      {
        /* Wait for Flash latency to be updated */
        getlatency = LL_FLASH_GetLatency();
        timeout--;
      } while ((getlatency != latency) && (timeout > 0U));

      if (getlatency != latency)
      {
        status = ERROR;
      }
    }
  }

  return status;
}


/**
  * @brief  This function configures system clock with MSI as clock source of the PLL
  * @note   The application needs to ensure that PLL configuration is valid
  * @note   The application needs to ensure that MSI range is valid.
  * @note   The application needs to ensure that BUS prescalers are valid
  * @note   Function is based on the following formula:
  *         - PLL output frequency = (((MSI frequency / PLLM) * PLLN) / PLLR)
  *         - PLLM: ensure that the VCO input frequency ranges from 2.66 to 16 MHz (PLLVCO_input = MSI frequency / PLLM)
  *         - PLLN: ensure that the VCO output frequency is between 96 and 344 MHz (PLLVCO_output = PLLVCO_input * PLLN)
  *         - PLLR: ensure that max frequency at 48000000 Hz is reached (PLLVCO_output / PLLR)
  * @param  UTILS_PLLInitStruct pointer to a @ref LL_UTILS_PLLInitTypeDef structure that contains
  *                             the configuration information for the PLL.
  * @param  UTILS_ClkInitStruct pointer to a @ref LL_UTILS_ClkInitTypeDef structure that contains
  *                             the configuration information for the BUS prescalers.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Max frequency configuration done
  *          - ERROR: Max frequency configuration not done
  */
ErrorStatus LL_PLL_ConfigSystemClock_MSI(LL_UTILS_PLLInitTypeDef *UTILS_PLLInitStruct,
            LL_UTILS_ClkInitTypeDef *UTILS_ClkInitStruct)
{
  ErrorStatus status = SUCCESS;
  uint32_t pllrfreq = 0;
  uint32_t msi_range;
#if defined(DUAL_CORE)
  uint32_t hclk2freq;
#endif /* DUAL_CORE */

  /* Check if one of the PLL is enabled */
  if (UTILS_PLL_IsBusy() == SUCCESS)
  {
    /* Get the current MSI range */
    if (LL_RCC_MSI_IsEnabledRangeSelect()  == 0U)
    {
      msi_range =  LL_RCC_MSI_GetRange();
      switch (msi_range)
      {
        case LL_RCC_MSIRANGE_0:     /* MSI = 100 KHz  */
        case LL_RCC_MSIRANGE_1:     /* MSI = 200 KHz  */
        case LL_RCC_MSIRANGE_2:     /* MSI = 400 KHz  */
        case LL_RCC_MSIRANGE_3:     /* MSI = 800 KHz  */
        case LL_RCC_MSIRANGE_4:     /* MSI = 1 MHz    */
        case LL_RCC_MSIRANGE_5:     /* MSI = 2 MHz    */
          /* PLLVCO input frequency is not in the range from 2.66 to 16 MHz*/
          status = ERROR;
          break;

        case LL_RCC_MSIRANGE_6:     /* MSI = 4 MHz    */
        case LL_RCC_MSIRANGE_7:     /* MSI = 8 MHz    */
        case LL_RCC_MSIRANGE_8:     /* MSI = 16 MHz   */
        case LL_RCC_MSIRANGE_9:     /* MSI = 24 MHz   */
        case LL_RCC_MSIRANGE_10:    /* MSI = 32 MHz   */
        case LL_RCC_MSIRANGE_11:    /* MSI = 48 MHz   */
        default:
          break;
      }
    }
    else
    {
      msi_range = LL_RCC_MSI_GetRangeAfterStandby();
      switch (msi_range)
      {
        case LL_RCC_MSISRANGE_4:    /* MSI = 1 MHz    */
        case LL_RCC_MSISRANGE_5:    /* MSI = 2 MHz    */
          /* PLLVCO input frequency is not in the range from 2.66 to 16 MHz*/
          status = ERROR;
          break;

        case LL_RCC_MSISRANGE_7:    /* MSI = 8 MHz    */
        case LL_RCC_MSISRANGE_6:    /* MSI = 4 MHz    */
        default:
          break;
      }
    }

    /* Calculate PLL output frequency */
    if (status != ERROR)
    {
      /* Calculate the new PLL output frequency & verify all PLL stages are correct (VCO input ranges,
         VCO output ranges & SYSCLK max) when assert activated */
      pllrfreq = UTILS_GetPLLOutputFrequency(__LL_RCC_CALC_MSI_FREQ(LL_RCC_MSI_IsEnabledRangeSelect(), msi_range),
                                             UTILS_PLLInitStruct);

#if defined(DUAL_CORE)
      /* Check HCLK2 frequency coherency */
      hclk2freq = __LL_RCC_CALC_HCLK2_FREQ(pllrfreq, UTILS_ClkInitStruct->CPU2CLKDivider);

      if (hclk2freq > UTILS_HCLK2_MAX)
      {
        /* HCLK2 frequency can not be higher than 48 Mhz */
        status = ERROR;
      }
#endif /* DUAL_CORE */
    }

    /* Main PLL configuration and activation */
    if (status != ERROR)
    {
      /* Enable MSI if not enabled */
      if (LL_RCC_MSI_IsReady() != 1U)
      {
        LL_RCC_MSI_Enable();
        while ((LL_RCC_MSI_IsReady() != 1U))
        {
          /* Wait for MSI ready */
        }
      }

      /* Configure PLL domain SYS */
      LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, UTILS_PLLInitStruct->PLLM, UTILS_PLLInitStruct->PLLN,
                                  UTILS_PLLInitStruct->PLLR);

      /* Enable PLL and switch system clock to PLL - latency check done internally */
      status = UTILS_EnablePLLAndSwitchSystem(pllrfreq, UTILS_ClkInitStruct);
    }
  }
  else
  {
    /* Current PLL configuration cannot be modified */
    status = ERROR;
  }

  return status;
}

/**
  * @brief  This function configures system clock at maximum frequency with HSI as clock source of the PLL
  * @note   The application needs to ensure that PLL configuration is valid
  * @note   The application needs to ensure that BUS prescalers are valid
  * @note   Function is based on the following formula:
  *         - PLL output frequency = (((HSI frequency / PLLM) * PLLN) / PLLR)
  *         - PLLM: ensure that the VCO input frequency ranges from 2.66 to 16 MHz (PLLVCO_input = HSI frequency / PLLM)
  *         - PLLN: ensure that the VCO output frequency is between 96 and 344 MHz (PLLVCO_output = PLLVCO_input * PLLN)
  *         - PLLR: ensure that max frequency at 48000000 Hz is reach (PLLVCO_output / PLLR)
  * @param  UTILS_PLLInitStruct pointer to a @ref LL_UTILS_PLLInitTypeDef structure that contains
  *                             the configuration information for the PLL.
  * @param  UTILS_ClkInitStruct pointer to a @ref LL_UTILS_ClkInitTypeDef structure that contains
  *                             the configuration information for the BUS prescalers.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Max frequency configuration done
  *          - ERROR: Max frequency configuration not done
  */
ErrorStatus LL_PLL_ConfigSystemClock_HSI(LL_UTILS_PLLInitTypeDef *UTILS_PLLInitStruct,
                                         LL_UTILS_ClkInitTypeDef *UTILS_ClkInitStruct)
{
  ErrorStatus status = SUCCESS;
  uint32_t pllrfreq;
#if defined(DUAL_CORE)
  uint32_t hclk2freq;
#endif /* DUAL_CORE */

  /* Check if one of the PLL is enabled */
  if (UTILS_PLL_IsBusy() == SUCCESS)
  {
    /* Calculate the new PLL output frequency */
    pllrfreq = UTILS_GetPLLOutputFrequency(HSI_VALUE, UTILS_PLLInitStruct);

#if defined(DUAL_CORE)
    hclk2freq = __LL_RCC_CALC_HCLK2_FREQ(pllrfreq, UTILS_ClkInitStruct->CPU2CLKDivider);

    /* Check HCLK2 frequency coherency */
    if (hclk2freq > UTILS_HCLK2_MAX)
    {
      /* HCLK2 frequency can not be higher than 48 Mhz */
      status = ERROR;
    }
#endif /* DUAL_CORE */

    if (status != ERROR)
    {
      /* Enable HSI if not enabled */
      if (LL_RCC_HSI_IsReady() != 1U)
      {
        LL_RCC_HSI_Enable();
        while (LL_RCC_HSI_IsReady() != 1U)
        {
          /* Wait for HSI ready */
        }
      }

      /* Configure PLL */
      LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, UTILS_PLLInitStruct->PLLM, UTILS_PLLInitStruct->PLLN,
                                  UTILS_PLLInitStruct->PLLR);

      /* Enable PLL and switch system clock to PLL */
      status = UTILS_EnablePLLAndSwitchSystem(pllrfreq, UTILS_ClkInitStruct);
    }
  }
  else
  {
    /* Current PLL configuration cannot be modified */
    status = ERROR;
  }

  return status;
}

/**
  * @brief  This function configures system clock with HSE as clock source of the PLL
  * @note   The application needs to ensure that PLL configuration is valid
  * @note   The application needs to ensure that BUS prescalers are valid
  * @note   Function is based on the following formula:
  *         - PLL output frequency = (((HSE frequency / PLLM) * PLLN) / PLLR)
  *         - PLLM: ensure that the VCO input frequency ranges from 2.66 to 16 MHz (PLLVCO_input = HSE frequency / PLLM)
  *         - PLLN: ensure that the VCO output frequency is between 96 and 344 MHz (PLLVCO_output = PLLVCO_input * PLLN)
  *         - PLLR: ensure that max frequency at 48000000 Hz is reached (PLLVCO_output / PLLR)
  * @param  UTILS_PLLInitStruct pointer to a @ref LL_UTILS_PLLInitTypeDef structure that contains
  *                             the configuration information for the PLL.
  * @param  UTILS_ClkInitStruct pointer to a @ref LL_UTILS_ClkInitTypeDef structure that contains
  *                             the configuration information for the BUS prescalers.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Max frequency configuration done
  *          - ERROR: Max frequency configuration not done
  */
ErrorStatus LL_PLL_ConfigSystemClock_HSE(LL_UTILS_PLLInitTypeDef *UTILS_PLLInitStruct,
                                         LL_UTILS_ClkInitTypeDef *UTILS_ClkInitStruct)
{
  ErrorStatus status = SUCCESS;
  uint32_t pllrfreq;
#if defined(DUAL_CORE)
  uint32_t hclk2freq;
#endif /* DUAL_CORE */

  /* Check if one of the PLL is enabled */
  if (UTILS_PLL_IsBusy() == SUCCESS)
  {
    /* Calculate the new PLL output frequency */
    pllrfreq = UTILS_GetPLLOutputFrequency(HSE_VALUE, UTILS_PLLInitStruct);

#if defined(DUAL_CORE)
    hclk2freq = __LL_RCC_CALC_HCLK2_FREQ(pllrfreq, UTILS_ClkInitStruct->CPU2CLKDivider);

    /* Check HCLK2 frequency coherency */
    if (hclk2freq > UTILS_HCLK2_MAX)
    {
      /* HCLK2 frequency can not be higher than 48 Mhz */
      status = ERROR;
    }
#endif /* DUAL_CORE */

    if (status != ERROR)
    {
      /* Enable HSE if not enabled */
      if (LL_RCC_HSE_IsReady() != 1U)
      {
        /* Enable HSE */
        LL_RCC_HSE_Enable();
        while (LL_RCC_HSE_IsReady() != 1U)
        {
          /* Wait for HSE ready */
        }
      }

      /* Configure PLL */
      LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, UTILS_PLLInitStruct->PLLM, UTILS_PLLInitStruct->PLLN,
                                  UTILS_PLLInitStruct->PLLR);

      /* Enable PLL and switch system clock to PLL */
      status = UTILS_EnablePLLAndSwitchSystem(pllrfreq, UTILS_ClkInitStruct);
    }
  }
  else
  {
    /* Current PLL configuration cannot be modified */
    status = ERROR;
  }

  return status;
}


/**
  * @}
  */


/**
  * @}
  */

/** @addtogroup UTILS_LL_Private_Functions
  * @{
  */

/**
  * @brief  Function to check that PLL can be modified
  * @param  PLL_InputFrequency  PLL input frequency (in Hz)
  * @param  UTILS_PLLInitStruct pointer to a @ref LL_UTILS_PLLInitTypeDef structure that contains
  *                             the configuration information for the PLL.
  * @retval PLL output frequency (in Hz)
  */
static uint32_t UTILS_GetPLLOutputFrequency(uint32_t PLL_InputFrequency, LL_UTILS_PLLInitTypeDef *UTILS_PLLInitStruct)
{
  uint32_t pllfreq;

  /* Check the parameters */
  assert_param(IS_LL_UTILS_PLLM_VALUE(UTILS_PLLInitStruct->PLLM));
  assert_param(IS_LL_UTILS_PLLN_VALUE(UTILS_PLLInitStruct->PLLN));
  assert_param(IS_LL_UTILS_PLLR_VALUE(UTILS_PLLInitStruct->PLLR));

  /* Check different PLL parameters according to RM                          */
  /*  - PLLM: ensure that the VCO input frequency ranges from 2.66 to 16 MHz.   */
  pllfreq = PLL_InputFrequency / (((UTILS_PLLInitStruct->PLLM >> RCC_PLLCFGR_PLLM_Pos) + 1U));
  assert_param(IS_LL_UTILS_PLLVCO_INPUT(pllfreq));

  /*  - PLLN: ensure that the VCO output frequency is between 96 and 344 MHz.*/
  pllfreq = pllfreq * (UTILS_PLLInitStruct->PLLN & (RCC_PLLCFGR_PLLN >> RCC_PLLCFGR_PLLN_Pos));
  assert_param(IS_LL_UTILS_PLLVCO_OUTPUT(pllfreq));

  /*  - PLLR: ensure that max frequency at 48000000 Hz is reached                   */
  pllfreq = pllfreq / ((UTILS_PLLInitStruct->PLLR >> RCC_PLLCFGR_PLLR_Pos) + 1U);
  assert_param(IS_LL_UTILS_PLL_FREQUENCY(pllfreq));

  return pllfreq;
}
/**
  * @brief  Function to check that PLL can be modified
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: PLL modification can be done
  *          - ERROR: PLL is busy
  */
static ErrorStatus UTILS_PLL_IsBusy(void)
{
  ErrorStatus status = SUCCESS;

  /* Check if PLL is busy*/
  if (LL_RCC_PLL_IsReady() != 0U)
  {
    /* PLL configuration cannot be modified */
    status = ERROR;
  }
  return status;
}

/**
  * @brief  Function to enable PLL and switch system clock to PLL
  * @param  SYSCLK_Frequency SYSCLK frequency
  * @param  UTILS_ClkInitStruct pointer to a @ref LL_UTILS_ClkInitTypeDef structure that contains
  *                             the configuration information for the BUS prescalers.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: No problem to switch system to PLL
  *          - ERROR: Problem to switch system to PLL
  */
static ErrorStatus UTILS_EnablePLLAndSwitchSystem(uint32_t SYSCLK_Frequency,
                                                  LL_UTILS_ClkInitTypeDef *UTILS_ClkInitStruct)
{
  ErrorStatus status = SUCCESS;
  uint32_t hclks_frequency_target;
  uint32_t hclks_frequency_current;
  uint32_t sysclk_current;

  assert_param(IS_LL_UTILS_SYSCLK_DIV(UTILS_ClkInitStruct->CPU1CLKDivider));
#if defined(DUAL_CORE)
  assert_param(IS_LL_UTILS_SYSCLK_DIV(UTILS_ClkInitStruct->CPU2CLKDivider));
#endif /* DUAL_CORE */
  assert_param(IS_LL_UTILS_SYSCLK_DIV(UTILS_ClkInitStruct->AHB3CLKDivider));
  assert_param(IS_LL_UTILS_APB1_DIV(UTILS_ClkInitStruct->APB1CLKDivider));
  assert_param(IS_LL_UTILS_APB2_DIV(UTILS_ClkInitStruct->APB2CLKDivider));

  /* Calculate HCLK3 frequency based on SYSCLK_Frequency target */
  hclks_frequency_target = __LL_RCC_CALC_HCLK3_FREQ(SYSCLK_Frequency, UTILS_ClkInitStruct->AHB3CLKDivider);

  /* Calculate HCLK3 frequency current */
  sysclk_current = (SystemCoreClock * AHBPrescTable[(LL_RCC_GetAHBPrescaler() & RCC_CFGR_HPRE) >>  RCC_CFGR_HPRE_Pos]);
  hclks_frequency_current = __LL_RCC_CALC_HCLK3_FREQ(sysclk_current, LL_RCC_GetAHB3Prescaler());

  /* Increasing the number of wait states because of higher CPU frequency */
  if (hclks_frequency_current < hclks_frequency_target)
  {
    /* Set FLASH latency to highest latency */
    status = LL_SetFlashLatency(hclks_frequency_target);
  }

  /* Update system clock configuration */
  if (status == SUCCESS)
  {
    /* Enable PLL */
    LL_RCC_PLL_Enable();
    LL_RCC_PLL_EnableDomain_SYS();
    while (LL_RCC_PLL_IsReady() != 1U)
    {
      /* Wait for PLL ready */
    }

    /* Sysclk activation on the main PLL */
    LL_RCC_SetAHBPrescaler(UTILS_ClkInitStruct->CPU1CLKDivider);
#if defined(DUAL_CORE)
    LL_C2_RCC_SetAHBPrescaler(UTILS_ClkInitStruct->CPU2CLKDivider);
#endif /* DUAL_CORE */
    LL_RCC_SetAHB3Prescaler(UTILS_ClkInitStruct->AHB3CLKDivider);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    {
      /* Wait for system clock switch to PLL */
    }

    /* Set APB1 & APB2 prescaler*/
    LL_RCC_SetAPB1Prescaler(UTILS_ClkInitStruct->APB1CLKDivider);
    LL_RCC_SetAPB2Prescaler(UTILS_ClkInitStruct->APB2CLKDivider);
  }

  /* Decreasing the number of wait states because of lower CPU frequency */
  if (hclks_frequency_current > hclks_frequency_target)
  {
    /* Set FLASH latency to lowest latency */
    status = LL_SetFlashLatency(hclks_frequency_target);
  }

  /* Update SystemCoreClock variable */
  if (status == SUCCESS)
  {
#if defined(CORE_CM0PLUS) && defined(DUAL_CORE)
    LL_SetSystemCoreClock(__LL_RCC_CALC_HCLK2_FREQ(SYSCLK_Frequency, UTILS_ClkInitStruct->CPU2CLKDivider));
#else
    LL_SetSystemCoreClock(__LL_RCC_CALC_HCLK1_FREQ(SYSCLK_Frequency, UTILS_ClkInitStruct->CPU1CLKDivider));
#endif /* CORE_CM0PLUS && DUAL_CORE */
  }

  return status;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
