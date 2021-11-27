/**
  ******************************************************************************
  * @file    stm32u5xx_ll_cortex.h
  * @author  MCD Application Team
  * @brief   Header file of CORTEX LL module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
    [..]
    The LL CORTEX driver contains a set of generic APIs that can be
    used by user:
      (+) SYSTICK configuration used by @ref LL_mDelay and @ref LL_Init1msTick
          functions
      (+) Low power mode configuration (SCB register of Cortex-MCU)
      (+) API to access to MCU info (CPUID register)
      (+) API to enable fault handler (SHCSR accesses)
      (+) API to enable and disable the MPU secure and non-secure
      (+) API to configure the region of MPU secure and non-secure
      (+) API to configure the attributes region of MPU secure and non-secure

  @endverbatim
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32U5xx_LL_CORTEX_H
#define STM32U5xx_LL_CORTEX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx.h"

/** @addtogroup STM32U5xx_LL_Driver
  * @{
  */

/** @defgroup CORTEX_LL CORTEX
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup CORTEX_LL_Exported_Constants CORTEX LL Exported Constants
  * @{
  */

/** @defgroup CORTEX_LL_EC_CLKSOURCE_HCLK SYSTICK Clock Source
  * @{
  */
#define LL_SYSTICK_CLKSOURCE_HCLK_DIV8     0x00000000U                 /*!< AHB clock divided by 8 selected as SysTick
                                                                            clock source */
#define LL_SYSTICK_CLKSOURCE_HCLK          SysTick_CTRL_CLKSOURCE_Msk  /*!< AHB clock selected as SysTick 
                                                                            clock source */
/**
  * @}
  */

/** @defgroup CORTEX_LL_EC_FAULT Handler Fault type
  * @{
  */
#define LL_HANDLER_FAULT_USG               SCB_SHCSR_USGFAULTENA_Msk              /*!< Usage fault */
#define LL_HANDLER_FAULT_BUS               SCB_SHCSR_BUSFAULTENA_Msk              /*!< Bus fault */
#define LL_HANDLER_FAULT_MEM               SCB_SHCSR_MEMFAULTENA_Msk              /*!< Memory management fault */
#define LL_HANDLER_FAULT_SECURE            SCB_SHCSR_SECUREFAULTENA_Msk           /*!< Secure fault */
/**
  * @}
  */

/** @defgroup CORTEX_LL_MPU_HFNMI_PRIVDEF_Control CORTEX LL MPU HFNMI and PRIVILEGED Access control
  * @{
  */
#define LL_MPU_CTRL_HFNMI_PRIVDEF_NONE          0U
#define LL_MPU_CTRL_HARDFAULT_NMI               2U
#define LL_MPU_CTRL_PRIVILEGED_DEFAULT          4U
#define LL_MPU_CTRL_HFNMI_PRIVDEF               6U
/**
  * @}
  */

/** @defgroup CORTEX_LL_MPU_Attributes CORTEX LL MPU Attributes
  * @{
  */
#define  LL_MPU_DEVICE_nGnRnE          0x0U  /* Device, noGather, noReorder, noEarly acknowledge. */
#define  LL_MPU_DEVICE_nGnRE           0x4U  /* Device, noGather, noReorder, Early acknowledge.   */
#define  LL_MPU_DEVICE_nGRE            0x8U  /* Device, noGather, Reorder, Early acknowledge.     */
#define  LL_MPU_DEVICE_GRE             0xCU  /* Device, Gather, Reorder, Early acknowledge.       */

#define  LL_MPU_WRITE_THROUGH          0x0U  /* Normal memory, write-through. */
#define  LL_MPU_NOT_CACHEABLE          0x4U  /* Normal memory, non-cacheable. */
#define  LL_MPU_WRITE_BACK             0x4U  /* Normal memory, write-back.    */

#define  LL_MPU_TRANSIENT              0x0U  /* Normal memory, transient.     */
#define  LL_MPU_NON_TRANSIENT          0x8U  /* Normal memory, non-transient. */

#define  LL_MPU_NO_ALLOCATE            0x0U  /* Normal memory, no allocate.         */
#define  LL_MPU_W_ALLOCATE             0x1U  /* Normal memory, write allocate.      */
#define  LL_MPU_R_ALLOCATE             0x2U  /* Normal memory, read allocate.       */
#define  LL_MPU_RW_ALLOCATE            0x3U  /* Normal memory, read/write allocate. */
/**
  * @}
  */

/** @defgroup CORTEX_LL_MPU_Region_Enable CORTEX LL MPU Region Enable
  * @{
  */
#define LL_MPU_REGION_ENABLE               1U
#define LL_MPU_REGION_DISABLE              0U
/**
  * @}
  */

/** @defgroup CORTEX_LL_MPU_Instruction_Access CORTEX LL MPU Instruction Access
  * @{
  */
#define LL_MPU_INSTRUCTION_ACCESS_ENABLE   (0U << MPU_RBAR_XN_Pos)
#define LL_MPU_INSTRUCTION_ACCESS_DISABLE  (1U << MPU_RBAR_XN_Pos)
/**
  * @}
  */

/** @defgroup CORTEX_LL_MPU_Access_Shareable CORTEX LL MPU Instruction Access Shareable
  * @{
  */
#define LL_MPU_ACCESS_NOT_SHAREABLE        (0U << MPU_RBAR_SH_Pos)
#define LL_MPU_ACCESS_OUTER_SHAREABLE      (1U << MPU_RBAR_SH_Pos)
#define LL_MPU_ACCESS_INNER_SHAREABLE      (3U << MPU_RBAR_SH_Pos)
/**
  * @}
  */

/** @defgroup CORTEX_LL_MPU_Region_Permission_Attributes CORTEX LL MPU Region Permission Attributes
  * @{
  */
#define LL_MPU_REGION_PRIV_RW              (0U << MPU_RBAR_AP_Pos)
#define LL_MPU_REGION_ALL_RW               (1U << MPU_RBAR_AP_Pos)
#define LL_MPU_REGION_PRIV_RO              (2U << MPU_RBAR_AP_Pos)
#define LL_MPU_REGION_ALL_RO               (3U << MPU_RBAR_AP_Pos)
/**
  * @}
  */

/** @defgroup CORTEX_LL_MPU_Region_Index CORTEX LL MPU Region Index
  * @{
  */
#define LL_MPU_REGION_NUMBER0              0U
#define LL_MPU_REGION_NUMBER1              1U
#define LL_MPU_REGION_NUMBER2              2U
#define LL_MPU_REGION_NUMBER3              3U
#define LL_MPU_REGION_NUMBER4              4U
#define LL_MPU_REGION_NUMBER5              5U
#define LL_MPU_REGION_NUMBER6              6U
#define LL_MPU_REGION_NUMBER7              7U
/**
  * @}
  */

/** @defgroup CORTEX_LL_MPU_Attributes_Index CORTEX LL MPU Memory Attributes Index
  * @{
  */
#define LL_MPU_ATTRIBUTES_NUMBER0          0U
#define LL_MPU_ATTRIBUTES_NUMBER1          1U
#define LL_MPU_ATTRIBUTES_NUMBER2          2U
#define LL_MPU_ATTRIBUTES_NUMBER3          3U
#define LL_MPU_ATTRIBUTES_NUMBER4          4U
#define LL_MPU_ATTRIBUTES_NUMBER5          5U
#define LL_MPU_ATTRIBUTES_NUMBER6          6U
#define LL_MPU_ATTRIBUTES_NUMBER7          7U
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @defgroup CORTEX_LL_Exported_Functions CORTEX LL Exported Functions
  * @{
  */

/** @defgroup CORTEX_LL_EF_SYSTICK SYSTICK
  * @brief CORTEX SYSTICK LL module driver
  * @{
  */

/**
  * @brief  This function checks if the Systick counter flag is active or not.
  * @note   It can be used in timeout function on application side.
  * @rmtoll STK_CTRL     COUNTFLAG     LL_SYSTICK_IsActiveCounterFlag
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SYSTICK_IsActiveCounterFlag(void)
{
  return (((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == (SysTick_CTRL_COUNTFLAG_Msk)) ? 1UL : 0UL);
}

/**
  * @brief  Configures the SysTick clock source
  * @rmtoll STK_CTRL     CLKSOURCE     LL_SYSTICK_SetClkSource
  * @param  Source This parameter can be one of the following values:
  *         @arg @ref LL_SYSTICK_CLKSOURCE_HCLK_DIV8
  *         @arg @ref LL_SYSTICK_CLKSOURCE_HCLK
  * @retval None
  */
__STATIC_INLINE void LL_SYSTICK_SetClkSource(uint32_t Source)
{
  if (Source == LL_SYSTICK_CLKSOURCE_HCLK)
  {
    SET_BIT(SysTick->CTRL, LL_SYSTICK_CLKSOURCE_HCLK);
  }
  else
  {
    CLEAR_BIT(SysTick->CTRL, LL_SYSTICK_CLKSOURCE_HCLK);
  }
}

/**
  * @brief  Get the SysTick clock source
  * @rmtoll STK_CTRL     CLKSOURCE     LL_SYSTICK_GetClkSource
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_SYSTICK_CLKSOURCE_HCLK_DIV8
  *         @arg @ref LL_SYSTICK_CLKSOURCE_HCLK
  */
__STATIC_INLINE uint32_t LL_SYSTICK_GetClkSource(void)
{
  return READ_BIT(SysTick->CTRL, LL_SYSTICK_CLKSOURCE_HCLK);
}

/**
  * @brief  Enable SysTick exception request
  * @rmtoll STK_CTRL     TICKINT       LL_SYSTICK_EnableIT
  * @retval None
  */
__STATIC_INLINE void LL_SYSTICK_EnableIT(void)
{
  SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
}

/**
  * @brief  Disable SysTick exception request
  * @rmtoll STK_CTRL     TICKINT       LL_SYSTICK_DisableIT
  * @retval None
  */
__STATIC_INLINE void LL_SYSTICK_DisableIT(void)
{
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
}

/**
  * @brief  Checks if the SYSTICK interrupt is enabled or disabled.
  * @rmtoll STK_CTRL     TICKINT       LL_SYSTICK_IsEnabledIT
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SYSTICK_IsEnabledIT(void)
{
  return ((READ_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk) == (SysTick_CTRL_TICKINT_Msk)) ? 1UL : 0UL);
}

/**
  * @}
  */

/** @defgroup CORTEX_LL_EF_LOW_POWER_MODE CORTEX LL LOW POWER MODE
  * @{
  */

/**
  * @brief  Processor uses sleep as its low power mode
  * @rmtoll SCB_SCR      SLEEPDEEP     LL_LPM_EnableSleep
  * @retval None
  */
__STATIC_INLINE void LL_LPM_EnableSleep(void)
{
  /* Clear SLEEPDEEP bit of Cortex System Control Register */
  CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
}

/**
  * @brief  Processor uses deep sleep as its low power mode
  * @rmtoll SCB_SCR      SLEEPDEEP     LL_LPM_EnableDeepSleep
  * @retval None
  */
__STATIC_INLINE void LL_LPM_EnableDeepSleep(void)
{
  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
}

/**
  * @brief  Configures sleep-on-exit when returning from Handler mode to Thread mode.
  * @note   Setting this bit to 1 enables an interrupt-driven application to avoid returning to an
  *         empty main application.
  * @rmtoll SCB_SCR      SLEEPONEXIT   LL_LPM_EnableSleepOnExit
  * @retval None
  */
__STATIC_INLINE void LL_LPM_EnableSleepOnExit(void)
{
  /* Set SLEEPONEXIT bit of Cortex System Control Register */
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPONEXIT_Msk));
}

/**
  * @brief  Do not sleep when returning to Thread mode.
  * @rmtoll SCB_SCR      SLEEPONEXIT   LL_LPM_DisableSleepOnExit
  * @retval None
  */
__STATIC_INLINE void LL_LPM_DisableSleepOnExit(void)
{
  /* Clear SLEEPONEXIT bit of Cortex System Control Register */
  CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPONEXIT_Msk));
}

/**
  * @brief  Enabled events and all interrupts, including disabled interrupts, can wakeup the
  *         processor.
  * @rmtoll SCB_SCR      SEVEONPEND    LL_LPM_EnableEventOnPend
  * @retval None
  */
__STATIC_INLINE void LL_LPM_EnableEventOnPend(void)
{
  /* Set SEVEONPEND bit of Cortex System Control Register */
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SEVONPEND_Msk));
}

/**
  * @brief  Only enabled interrupts or events can wakeup the processor, disabled interrupts are
  *         excluded
  * @rmtoll SCB_SCR      SEVEONPEND    LL_LPM_DisableEventOnPend
  * @retval None
  */
__STATIC_INLINE void LL_LPM_DisableEventOnPend(void)
{
  /* Clear SEVEONPEND bit of Cortex System Control Register */
  CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SEVONPEND_Msk));
}

/**
  * @}
  */

/** @defgroup CORTEX_LL_EF_HANDLER CORTEX LL HANDLER
  * @{
  */

/**
  * @brief  Enable a fault in System handler control register (SHCSR)
  * @rmtoll SCB_SHCSR    USGFAULTENA     LL_HANDLER_EnableFault\n
  *         SCB_SHCSR    BUSFAULTENA     LL_HANDLER_EnableFault\n
  *         SCB_SHCSR    MEMFAULTENA     LL_HANDLER_EnableFault\n
  *         SCB_SHCSR    SECUREFAULTENA  LL_HANDLER_EnableFault
  * @param  Fault This parameter can be a combination of the following values:
  *         @arg @ref LL_HANDLER_FAULT_USG
  *         @arg @ref LL_HANDLER_FAULT_BUS
  *         @arg @ref LL_HANDLER_FAULT_MEM
  *         @arg @ref LL_HANDLER_FAULT_SECURE (*)
  *
  *         (*) value applicable in secure when the system implements the security.
  * @retval None
  */
__STATIC_INLINE void LL_HANDLER_EnableFault(uint32_t Fault)
{
  /* Enable the system handler fault */
  SET_BIT(SCB->SHCSR, Fault);
}

/**
  * @brief  Disable a fault in System handler control register (SHCSR)
  * @rmtoll SCB_SHCSR    USGFAULTENA     LL_HANDLER_DisableFault\n
  *         SCB_SHCSR    BUSFAULTENA     LL_HANDLER_DisableFault\n
  *         SCB_SHCSR    MEMFAULTENA     LL_HANDLER_DisableFault\n
  *         SCB_SHCSR    SECUREFAULTENA  LL_HANDLER_DisableFault
  * @param  Fault This parameter can be a combination of the following values:
  *         @arg @ref LL_HANDLER_FAULT_USG
  *         @arg @ref LL_HANDLER_FAULT_BUS
  *         @arg @ref LL_HANDLER_FAULT_MEM
  *         @arg @ref LL_HANDLER_FAULT_SECURE (*)
  *
  *         (*) value applicable in secure when the system implements the security.
  * @retval None
  */
__STATIC_INLINE void LL_HANDLER_DisableFault(uint32_t Fault)
{
  /* Disable the system handler fault */
  CLEAR_BIT(SCB->SHCSR, Fault);
}

/**
  * @}
  */

/** @defgroup CORTEX_LL_EF_MCU_INFO CORTEX LL MCU INFO
  * @{
  */

/**
  * @brief  Get Implementer code
  * @rmtoll SCB_CPUID    IMPLEMENTER   LL_CPUID_GetImplementer
  * @retval Value should be equal to 0x41 for ARM
  */
__STATIC_INLINE uint32_t LL_CPUID_GetImplementer(void)
{
  return (uint32_t)(READ_BIT(SCB->CPUID, SCB_CPUID_IMPLEMENTER_Msk) >> SCB_CPUID_IMPLEMENTER_Pos);
}

/**
  * @brief  Get Variant number (The r value in the rnpn product revision identifier)
  * @rmtoll SCB_CPUID    VARIANT       LL_CPUID_GetVariant
  * @retval Value between 0 and 255 (0x0: revision 0)
  */
__STATIC_INLINE uint32_t LL_CPUID_GetVariant(void)
{
  return (uint32_t)(READ_BIT(SCB->CPUID, SCB_CPUID_VARIANT_Msk) >> SCB_CPUID_VARIANT_Pos);
}

/**
  * @brief  Get Constant number
  * @rmtoll SCB_CPUID    ARCHITECTURE  LL_CPUID_GetConstant
  * @retval Value should be equal to 0xF for Cortex-M33 devices
  */
__STATIC_INLINE uint32_t LL_CPUID_GetConstant(void)
{
  return (uint32_t)(READ_BIT(SCB->CPUID, SCB_CPUID_ARCHITECTURE_Msk) >> SCB_CPUID_ARCHITECTURE_Pos);
}

/**
  * @brief  Get Part number
  * @rmtoll SCB_CPUID    PARTNO        LL_CPUID_GetParNo
  * @retval Value should be equal to 0xD21 for Cortex-M33
  */
__STATIC_INLINE uint32_t LL_CPUID_GetParNo(void)
{
  return (uint32_t)(READ_BIT(SCB->CPUID, SCB_CPUID_PARTNO_Msk) >> SCB_CPUID_PARTNO_Pos);
}

/**
  * @brief  Get Revision number (The p value in the rnpn product revision identifier, indicates patch release)
  * @rmtoll SCB_CPUID    REVISION      LL_CPUID_GetRevision
  * @retval Value between 0 and 255 (0x1: patch 1)
  */
__STATIC_INLINE uint32_t LL_CPUID_GetRevision(void)
{
  return (uint32_t)(READ_BIT(SCB->CPUID, SCB_CPUID_REVISION_Msk) >> SCB_CPUID_REVISION_Pos);
}

/**
  * @}
  */

/** @defgroup CORTEX_LL_EF_MPU CORTEX LL MPU
  * @{
  */

/**
  * @brief  Enable MPU with input options
  * @rmtoll MPU_CTRL     ENABLE        LL_MPU_Enable
  * @param  MPU_Control This parameter can be one of the following values:
  *         @arg @ref LL_MPU_CTRL_HFNMI_PRIVDEF_NONE
  *         @arg @ref LL_MPU_CTRL_HARDFAULT_NMI
  *         @arg @ref LL_MPU_CTRL_PRIVILEGED_DEFAULT
  *         @arg @ref LL_MPU_CTRL_HFNMI_PRIVDEF
  * @retval None
  */
__STATIC_INLINE void LL_MPU_Enable(uint32_t MPU_Control)
{
  /* Enable the MPU*/
  MPU->CTRL = MPU_CTRL_ENABLE_Msk | MPU_Control;

  /* Ensure MPU settings take effects */
  __DSB();

  /* Sequence instruction fetches using update settings */
  __ISB();
}

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/**
  * @brief  Enable non-secure MPU with input options
  * @rmtoll MPU_CTRL     ENABLE        LL_MPU_Enable
  * @param  MPU_Control This parameter can be one of the following values:
  *         @arg @ref LL_MPU_CTRL_HFNMI_PRIVDEF_NONE
  *         @arg @ref LL_MPU_CTRL_HARDFAULT_NMI
  *         @arg @ref LL_MPU_CTRL_PRIVILEGED_DEFAULT
  *         @arg @ref LL_MPU_CTRL_HFNMI_PRIVDEF
  * @retval None
  */
__STATIC_INLINE void LL_MPU_Enable_NS(uint32_t MPU_Control)
{
  /* Enable the MPU*/
  MPU_NS->CTRL = MPU_CTRL_ENABLE_Msk | MPU_Control;

  /* Ensure MPU settings take effects */
  __DSB();

  /* Sequence instruction fetches using update settings */
  __ISB();
}
#endif /* __ARM_FEATURE_CMSE */

/**
  * @brief  Disable MPU
  * @rmtoll MPU_CTRL     ENABLE        LL_MPU_Disable
  * @retval None
  */
__STATIC_INLINE void LL_MPU_Disable(void)
{
  /* Make sure outstanding transfers are done */
  __DMB();
  /* Disable MPU*/
  WRITE_REG(MPU->CTRL, 0U);
}

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/**
  * @brief  Disable the non-secure MPU
  * @rmtoll MPU_CTRL     ENABLE        LL_MPU_Disable_NS
  * @retval None
  */
__STATIC_INLINE void LL_MPU_Disable_NS(void)
{
  /* Make sure outstanding transfers are done */
  __DMB();
  /* Disable MPU*/
  WRITE_REG(MPU_NS->CTRL, 0U);
}
#endif /* __ARM_FEATURE_CMSE */


/**
  * @brief  Check if MPU is enabled or not
  * @rmtoll MPU_CTRL     ENABLE        LL_MPU_IsEnabled
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_MPU_IsEnabled(void)
{
  return ((READ_BIT(MPU->CTRL, MPU_CTRL_ENABLE_Msk) == (MPU_CTRL_ENABLE_Msk)) ? 1UL : 0UL);
}

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/**
  * @brief  Check if non-secure MPU is enabled or not
  * @rmtoll MPU_CTRL     ENABLE        LL_MPU_IsEnabled_NS
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_MPU_IsEnabled_NS(void)
{
  return ((READ_BIT(MPU_NS->CTRL, MPU_CTRL_ENABLE_Msk) == (MPU_CTRL_ENABLE_Msk)) ? 1UL : 0UL);
}
#endif /* __ARM_FEATURE_CMSE */

/**
  * @brief  Enable a MPU region
  * @rmtoll MPU_RLAR     ENABLE        LL_MPU_EnableRegion
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @note   cortex-M33 supports 8 secure and 8 non secure regions.
  * @retval None
  */
__STATIC_INLINE void LL_MPU_EnableRegion(uint32_t Region)
{
  /* Set Region number */
  WRITE_REG(MPU->RNR, Region);

  /* Enable the MPU region */
  SET_BIT(MPU->RLAR, MPU_RLAR_EN_Msk);
}

/**
  * @brief  Check if MPU region is enabled or not
  * @rmtoll MPU_RNR     ENABLE        LL_MPU_IsEnabledRegion
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @note   cortex-M33 supports 8 secure and 8 non secure regions.
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_MPU_IsEnabled_Region(uint32_t Region)
{
  return ((READ_BIT(MPU->RNR, Region) == (Region)) ? 1UL : 0UL);
}

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/**
  * @brief  Enable a non-secure MPU region
  * @rmtoll MPU_RLAR     ENABLE        LL_MPU_EnableRegion_NS
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @note   cortex-M33 supports 8 secure and 8 non secure regions.
  * @retval None
  */
__STATIC_INLINE void LL_MPU_EnableRegion_NS(uint32_t Region)
{
  /* Set Region number */
  WRITE_REG(MPU_NS->RNR, Region);

  /* Enable the MPU region */
  SET_BIT(MPU_NS->RLAR, MPU_RLAR_EN_Msk);
}

/**
  * @brief  Check if non-secure MPU region is enabled or not
  * @rmtoll MPU_RNR     ENABLE        LL_MPU_IsEnableRegion_NS
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @note   cortex-M33 supports 8 secure and 8 non secure regions.
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_MPU_IsEnabled_Region_NS(uint32_t Region)
{
  return ((READ_BIT(MPU_NS->RNR, Region) == (Region)) ? 1UL : 0UL);
}
#endif /* __ARM_FEATURE_CMSE */

/**
  * @brief  Disable a MPU region
  * @rmtoll MPU_RNR      REGION        LL_MPU_DisableRegion\n
  *         MPU_RLAR     ENABLE        LL_MPU_DisableRegion
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @note   cortex-M33 supports 8 secure and 8 non secure regions.
  * @retval None
  */
__STATIC_INLINE void LL_MPU_DisableRegion(uint32_t Region)
{
  /* Set Region number */
  WRITE_REG(MPU->RNR, Region);

  /* Disable the MPU region */
  CLEAR_BIT(MPU->RLAR, MPU_RLAR_EN_Msk);
}

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/**
  * @brief  Disable a non-secure MPU region
  * @rmtoll MPU_RNR      REGION        LL_MPU_DisableRegion_NS\n
  *         MPU_RLAR     ENABLE        LL_MPU_DisableRegion_NS\n
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @note   cortex-M33 supports 8 secure and 8 non secure regions.
  * @retval None
  */
__STATIC_INLINE void LL_MPU_DisableRegion_NS(uint32_t Region)
{
  /* Set Region number */
  WRITE_REG(MPU_NS->RNR, Region);

  /* Disable the MPU region */
  CLEAR_BIT(MPU_NS->RLAR, MPU_RLAR_EN_Msk);
}
#endif /* __ARM_FEATURE_CMSE */

/**
  * @brief  Configure and enable a MPU region
  * @rmtoll MPU_RNR      REGION        LL_MPU_ConfigRegion\n
  *         MPU_RBAR     ADDR          LL_MPU_ConfigRegion\n
  *         MPU_RLAR     ADDR          LL_MPU_ConfigRegion\n
  *         MPU_RBAR     XN            LL_MPU_ConfigRegion\n
  *         MPU_RBAR     AP            LL_MPU_ConfigRegion\n
  *         MPU_RBAR     SH            LL_MPU_ConfigRegion\n
  *         MPU_RLAR     EN            LL_MPU_ConfigRegion\n
  *         MPU_RLAR     AttrIndx      LL_MPU_ConfigRegion\n
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @param  Attributes This parameter can be a combination of the following values:
  *         @arg @ref LL_MPU_INSTRUCTION_ACCESS_ENABLE or @ref LL_MPU_INSTRUCTION_ACCESS_DISABLE
  *         @arg @ref LL_MPU_ACCESS_NOT_SHAREABLE or @ref LL_MPU_ACCESS_OUTER_SHAREABLE
  *              or @ref LL_MPU_ACCESS_INNER_SHAREABLE
  *         @arg @ref LL_MPU_REGION_PRIV_RW or @ref LL_MPU_REGION_ALL_RW or @ref LL_MPU_REGION_PRIV_RO
  *              or @ref LL_MPU_REGION_ALL_RO
  * @param  AttrIndx This parameter can be one of the following values:
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER0
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER1
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER2
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER3
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER4
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER5
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER6
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER7
  * @param  BaseAddress Value of region base address
  * @param  LimitAddress Value of region limit address
  * @note   cortex-M33 supports 8 secure and 8 non secure regions.
  * @retval None
  */
__STATIC_INLINE void LL_MPU_ConfigRegion(uint32_t Region, uint32_t Attributes, uint32_t AttrIndx, uint32_t BaseAddress,
                                         uint32_t LimitAddress)
{
  /* Set Region number */
  WRITE_REG(MPU->RNR, Region);

  /* Set base address */
  MPU->RBAR |=  Attributes;
  /* Set base address */
  MPU->RBAR |= (BaseAddress & 0xFFFFFFE0U);

  /* Set limit address */
  MPU->RLAR |= (LimitAddress & 0xFFFFFFE0U);
  /* Configure MPU */
  MPU->RLAR |= (MPU_RLAR_EN_Msk | AttrIndx);
}

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/**
  * @brief  Configure and enable a non-secure MPU region
  * @rmtoll MPU_RNR      REGION        LL_MPU_ConfigRegion_NS\n
  *         MPU_RBAR     ADDR          LL_MPU_ConfigRegion_NS\n
  *         MPU_RLAR     ADDR          LL_MPU_ConfigRegion_NS\n
  *         MPU_RBAR     XN            LL_MPU_ConfigRegion_NS\n
  *         MPU_RBAR     AP            LL_MPU_ConfigRegion_NS\n
  *         MPU_RBAR     SH            LL_MPU_ConfigRegion_NS\n
  *         MPU_RLAR     EN            LL_MPU_ConfigRegion_NS\n
  *         MPU_RLAR     AttrIndx      LL_MPU_ConfigRegion_NS\n
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @param  Attributes This parameter can be a combination of the following values:
  *         @arg @ref LL_MPU_INSTRUCTION_ACCESS_ENABLE or @ref LL_MPU_INSTRUCTION_ACCESS_DISABLE
  *         @arg @ref LL_MPU_ACCESS_NOT_SHAREABLE or @ref LL_MPU_ACCESS_OUTER_SHAREABLE
  *              or @ref LL_MPU_ACCESS_INNER_SHAREABLE
  *         @arg @ref LL_MPU_REGION_PRIV_RW or @ref LL_MPU_REGION_ALL_RW or @ref LL_MPU_REGION_PRIV_RO
  *              or @ref LL_MPU_REGION_ALL_RO
  * @param  AttrIndx This parameter can be one of the following values:
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER0
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER1
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER2
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER3
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER4
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER5
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER6
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER7
  * @param  BaseAddress Value of region base address
  * @param  LimitAddress Value of region limit address
  * @note   cortex-M33 supports 8 secure and 8 non secure regions.
  * @retval None
  */
__STATIC_INLINE void LL_MPU_ConfigRegion_NS(uint32_t Region, uint32_t Attributes, uint32_t AttrIndx,
                                            uint32_t BaseAddress, uint32_t LimitAddress)
{
  /* Set Region number */
  WRITE_REG(MPU_NS->RNR, Region);

  /* Set base address */
  MPU_NS->RBAR |=  Attributes;

  /* Set base address */
  MPU_NS->RBAR |= (BaseAddress & 0xFFFFFFE0U);

  /* Set limit address */
  MPU_NS->RLAR |= (LimitAddress & 0xFFFFFFE0U);
  /* Configure MPU */
  MPU_NS->RLAR |= (MPU_RLAR_EN_Msk | AttrIndx);
}
#endif /* __ARM_FEATURE_CMSE */

/**
  * @brief  Configure a MPU region address range
  * @rmtoll MPU_RNR      REGION        LL_MPU_ConfigRegionAddress\n
  *         MPU_RBAR     ADDR          LL_MPU_ConfigRegionAddress\n
  *         MPU_RLAR     ADDR          LL_MPU_ConfigRegionAddress\n
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @param  BaseAddress Value of region base address
  * @param  LimitAddress Value of region limit address
  * @note   cortex-M33 supports 8 secure and 8 non secure regions.
  * @retval None
  */
__STATIC_INLINE void LL_MPU_ConfigRegionAddress(uint32_t Region, uint32_t BaseAddress, uint32_t LimitAddress)
{
  /* Set Region number */
  WRITE_REG(MPU->RNR, Region);
  /* Set base address */
  MPU->RBAR |= (BaseAddress & 0xFFFFFFE0U) ;

  /* Set limit address */
  MPU->RLAR |= (LimitAddress & 0xFFFFFFE0U);
}

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/**
  * @brief  Configure a non-secure MPU region address range
  * @rmtoll MPU_RNR      REGION        LL_MPU_ConfigRegionAddress_NS\n
  *         MPU_RBAR     ADDR          LL_MPU_ConfigRegionAddress_NS\n
  *         MPU_RLAR     ADDR          LL_MPU_ConfigRegionAddress_NS\n
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @param  BaseAddress Value of region base address
  * @param  LimitAddress Value of region limit address
  * @note   cortex-M33 supports 8 secure and 8 non secure regions.
  * @retval None
  */
__STATIC_INLINE void LL_MPU_ConfigRegionAddress_NS(uint32_t Region, uint32_t BaseAddress, uint32_t LimitAddress)
{
  /* Set Region number */
  WRITE_REG(MPU_NS->RNR, Region);
  /* Set base address */
  MPU_NS->RBAR |= (BaseAddress & 0xFFFFFFE0U);

  /* Set limit address */
  MPU_NS->RLAR |= (LimitAddress & 0xFFFFFFE0U);
}
#endif /* __ARM_FEATURE_CMSE */

/**
  * @brief  Configure a MPU attributes index
  * @rmtoll MPU_MAIR0      Attribute       LL_MPU_ConfigAttributes\n
  *         MPU_MAIR1      Attribute       LL_MPU_ConfigAttributes\n
  * @param  AttIndex This parameter can be one of the following values:
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER0
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER1
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER2
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER3
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER4
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER5
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER6
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER7
  * @param  Attributes This parameter can be a combination of @ref CORTEX_LL_MPU_Attributes
  * @retval None
  */
__STATIC_INLINE void LL_MPU_ConfigAttributes(uint32_t AttIndex, uint32_t  Attributes)
{
  if (AttIndex < LL_MPU_ATTRIBUTES_NUMBER4)
  {
    /* Program MPU_MAIR0 */
    WRITE_REG(MPU->MAIR0, (Attributes << (AttIndex * 8U)));
  }
  else
  {
    /* Program MPU_MAIR1 */
    WRITE_REG(MPU->MAIR1, (Attributes << ((AttIndex - 4U) * 8U)));
  }
}

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/**
  * @brief  Configure a non-secure MPU attributes index
  * @rmtoll MPU_MAIR0      Attribute       LL_MPU_ConfigAttributes_NS\n
  *         MPU_MAIR1      Attribute       LL_MPU_ConfigAttributes_NS\n
  * @param  AttIndex This parameter can be one of the following values:
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER0
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER1
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER2
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER3
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER4
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER5
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER6
  *         @arg @ref LL_MPU_ATTRIBUTES_NUMBER7
  * @param  Attributes This parameter can be a combination of @ref CORTEX_LL_MPU_Attributes
  * @retval None
  */
__STATIC_INLINE void LL_MPU_ConfigAttributes_NS(uint32_t AttIndex, uint32_t  Attributes)
{
  if (AttIndex < LL_MPU_ATTRIBUTES_NUMBER4)
  {
    /* Program MPU_MAIR0 */
    WRITE_REG(MPU_NS->MAIR0, (Attributes << (AttIndex * 8U)));
  }
  else
  {
    /* Program MPU_MAIR1 */
    WRITE_REG(MPU_NS->MAIR1, (Attributes << ((AttIndex - 4U) * 8U)));
  }
}
#endif /* __ARM_FEATURE_CMSE */

/**
  * @brief  Configure a MPU region limit address
  * @rmtoll MPU_RNR      REGION        LL_MPU_SetRegionLimitAddress\n
  *         MPU_RLAR     ADDR          LL_MPU_SetRegionLimitAddress\n
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @param  LimitAddress Value of region limit address
  * @note   cortex-M33 supports 8 secure and 8 non secure regions.
  * @retval None
  */
__STATIC_INLINE void LL_MPU_SetRegionLimitAddress(uint32_t Region, uint32_t LimitAddress)
{
  /* Set Region number */
  WRITE_REG(MPU->RNR, Region);
  /* Set limit address */
  MPU->RLAR |= (LimitAddress & 0xFFFFFFE0U);
}

/**
  * @brief  Get a MPU region limit address
  * @rmtoll MPU_RNR      REGION        LL_MPU_GetRegionLimitAddress\n
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @retval None
  */
__STATIC_INLINE uint32_t LL_MPU_GetRegionLimitAddress(uint32_t Region)
{
  /* Set Region number */
  WRITE_REG(MPU->RNR, Region);
  return (READ_REG(MPU->RLAR & MPU_RLAR_LIMIT_Msk));
}

/**
  * @brief  Configure a MPU region base address
  * @rmtoll MPU_RNR      REGION        LL_MPU_SetRegionBaseAddress\n
  *         MPU_RBAR     ADDR          LL_MPU_SetRegionBaseAddress\n
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @param  BaseAddress Value of region base address
  * @note   cortex-M33 supports 8 secure and 8 non secure regions.
  * @retval None
  */
__STATIC_INLINE void LL_MPU_SetRegionBaseAddress(uint32_t Region, uint32_t BaseAddress)
{
  /* Set Region number */
  WRITE_REG(MPU->RNR, Region);
  /* Set base address */
  MPU->RBAR |= (BaseAddress & 0xFFFFFFE0U);
}

/**
  * @brief  Get a MPU region base address
  * @rmtoll MPU_RNR      REGION        LL_MPU_GetRegionBaseAddress\n
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @retval None
  */
__STATIC_INLINE uint32_t LL_MPU_GetRegionBaseAddress(uint32_t Region)
{
  /* Set Region number */
  WRITE_REG(MPU->RNR, Region);
  return (READ_REG(MPU->RBAR & MPU_RBAR_BASE_Msk));
}

/**
  * @brief  Configure a MPU region access attributes and enable a region
  * @rmtoll MPU_RNR      REGION        LL_MPU_SetRegionAccess\n
  *         MPU_RBAR     XN            LL_MPU_SetRegionAccess\n
  *         MPU_RBAR     AP            LL_MPU_SetRegionAccess\n
  *         MPU_RBAR     SH            LL_MPU_SetRegionAccess\n
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @param  Attributes This parameter can be a combination of the following values:
  *         @arg @ref LL_MPU_INSTRUCTION_ACCESS_ENABLE or @ref LL_MPU_INSTRUCTION_ACCESS_DISABLE
  *         @arg @ref LL_MPU_ACCESS_NOT_SHAREABLE or @ref LL_MPU_ACCESS_OUTER_SHAREABLE
  *              or @ref LL_MPU_ACCESS_INNER_SHAREABLE
  *         @arg @ref LL_MPU_REGION_PRIV_RW or @ref LL_MPU_REGION_ALL_RW or @ref LL_MPU_REGION_PRIV_RO
  *              or @ref LL_MPU_REGION_ALL_RO
  * @note   cortex-M33 supports 8 secure and 8 non secure regions.
  * @retval None
  */
__STATIC_INLINE void LL_MPU_SetRegionAccess(uint32_t Region, uint32_t Attributes)
{
  /* Set Region number */
  WRITE_REG(MPU->RNR, Region);
  /* Set base address */
  MPU->RBAR |=  Attributes;
}

/**
  * @brief  Get a MPU region access attributes
  * @rmtoll MPU_RNR      REGION        LL_MPU_GetRegionAccess\n
  *         MPU_RBAR     XN            LL_MPU_GetRegionAccess\n
  *         MPU_RBAR     AP            LL_MPU_GetRegionAccess\n
  *         MPU_RBAR     SH            LL_MPU_GetRegionAccess\n
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @retval None
  */
__STATIC_INLINE uint32_t LL_MPU_GetRegionAccess(uint32_t Region)
{
  /* Set Region number */
  WRITE_REG(MPU->RNR, Region);
  return (READ_REG(MPU->RBAR & (MPU_RBAR_XN_Msk | MPU_RBAR_AP_Msk | MPU_RBAR_SH_Msk)));
}

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/**
  * @brief  Configure a non-secure MPU region limit address
  * @rmtoll MPU_RNR      REGION        LL_MPU_SetRegionLimitAddress_NS\n
  *         MPU_RLAR     ADDR          LL_MPU_SetRegionLimitAddress_NS\n
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @param  LimitAddress Value of region limit address
  * @note   cortex-M33 supports 8 secure and 8 non secure regions.
  * @retval None
  */
__STATIC_INLINE void LL_MPU_SetRegionLimitAddress_NS(uint32_t Region, uint32_t LimitAddress)
{
  /* Set Region number */
  WRITE_REG(MPU_NS->RNR, Region);
  /* Set limit address */
  MPU_NS->RLAR |= (LimitAddress & 0xFFFFFFE0U);
}

/**
  * @brief  Get a non-secure MPU region limit address
  * @rmtoll MPU_RNR      REGION        LL_MPU_GetRegionLimitAddress_NS\n
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @retval None
  */
__STATIC_INLINE uint32_t LL_MPU_GetRegionLimitAddress_NS(uint32_t Region)
{
  /* Set Region number */
  WRITE_REG(MPU_NS->RNR, Region);
  return (READ_REG(MPU_NS->RLAR & MPU_RLAR_LIMIT_Msk));
}

/**
  * @brief  Configure a non-secure MPU region base address
  * @rmtoll MPU_RNR      REGION        LL_MPU_SetRegionBaseAddress_NS\n
  *         MPU_RBAR     ADDR          LL_MPU_SetRegionBaseAddress_NS\n
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @param  BaseAddress Value of region base address
  * @note   cortex-M33 supports 8 secure and 8 non secure regions.
  * @retval None
  */
__STATIC_INLINE void LL_MPU_SetRegionBaseAddress_NS(uint32_t Region, uint32_t BaseAddress)
{
  /* Set Region number */
  WRITE_REG(MPU_NS->RNR, Region);
  /* Set base address */
  MPU_NS->RBAR |= (BaseAddress & 0xFFFFFFE0U);
}

/**
  * @brief  Get a non-secure MPU region base address
  * @rmtoll MPU_RNR      REGION        LL_MPU_GetRegionBaseAddress_NS\n
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @retval None
  */
__STATIC_INLINE uint32_t LL_MPU_GetRegionBaseAddress_NS(uint32_t Region)
{
  /* Set Region number */
  WRITE_REG(MPU_NS->RNR, Region);
  return (READ_REG(MPU_NS->RBAR & MPU_RBAR_BASE_Msk));
}

/**
  * @brief  Configure a non-secure MPU region access attributes and enable a region
  * @rmtoll MPU_RNR      REGION        LL_MPU_SetRegionAccess_NS\n
  *         MPU_RBAR     XN            LL_MPU_SetRegionAccess_NS\n
  *         MPU_RBAR     AP            LL_MPU_SetRegionAccess_NS\n
  *         MPU_RBAR     SH            LL_MPU_SetRegionAccess_NS\n
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @param  Attributes This parameter can be a combination of the following values:
  *         @arg @ref LL_MPU_INSTRUCTION_ACCESS_ENABLE or @ref LL_MPU_INSTRUCTION_ACCESS_DISABLE
  *         @arg @ref LL_MPU_ACCESS_NOT_SHAREABLE or @ref LL_MPU_ACCESS_OUTER_SHAREABLE
  *              or @ref LL_MPU_ACCESS_INNER_SHAREABLE
  *         @arg @ref LL_MPU_REGION_PRIV_RW or @ref LL_MPU_REGION_ALL_RW or @ref LL_MPU_REGION_PRIV_RO
  *              or @ref LL_MPU_REGION_ALL_RO
  * @note   cortex-M33 supports 8 secure and 8 non secure regions.
  * @retval None
  */
__STATIC_INLINE void LL_MPU_SetRegionAccess_NS(uint32_t Region, uint32_t Attributes)
{
  /* Set Region number */
  WRITE_REG(MPU_NS->RNR, Region);
  /* Set base address Attributes */
  MPU_NS->RBAR |=  Attributes;
}

/**
  * @brief  Get a non-secure MPU region access attributes
  * @rmtoll MPU_RNR      REGION        LL_MPU_GetRegionAccess_NS\n
  *         MPU_RBAR     XN            LL_MPU_GetRegionAccess_NS\n
  *         MPU_RBAR     AP            LL_MPU_GetRegionAccess_NS\n
  *         MPU_RBAR     SH            LL_MPU_GetRegionAccess_NS\n
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_MPU_REGION_NUMBER0
  *         @arg @ref LL_MPU_REGION_NUMBER1
  *         @arg @ref LL_MPU_REGION_NUMBER2
  *         @arg @ref LL_MPU_REGION_NUMBER3
  *         @arg @ref LL_MPU_REGION_NUMBER4
  *         @arg @ref LL_MPU_REGION_NUMBER5
  *         @arg @ref LL_MPU_REGION_NUMBER6
  *         @arg @ref LL_MPU_REGION_NUMBER7
  * @retval None
  */
__STATIC_INLINE uint32_t LL_MPU_GetRegionAccess_NS(uint32_t Region)
{
  /* Set Region number */
  WRITE_REG(MPU_NS->RNR, Region);
  return (READ_REG(MPU_NS->RBAR & (MPU_RBAR_XN_Msk | MPU_RBAR_AP_Msk | MPU_RBAR_SH_Msk)));
}
#endif /* __ARM_FEATURE_CMSE */

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

#ifdef __cplusplus
}
#endif

#endif /* STM32U5xx_LL_CORTEX_H */

