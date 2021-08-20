/**
  ******************************************************************************
  * @file    stm32l1xx_ll_pwr.h
  * @author  MCD Application Team
  * @brief   Header file of PWR LL module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L1xx_LL_PWR_H
#define __STM32L1xx_LL_PWR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

/** @addtogroup STM32L1xx_LL_Driver
  * @{
  */

#if defined(PWR)

/** @defgroup PWR_LL PWR
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup PWR_LL_Exported_Constants PWR Exported Constants
  * @{
  */

/** @defgroup PWR_LL_EC_CLEAR_FLAG Clear Flags Defines
  * @brief    Flags defines which can be used with LL_PWR_WriteReg function
  * @{
  */
#define LL_PWR_CR_CSBF                     PWR_CR_CSBF            /*!< Clear standby flag */
#define LL_PWR_CR_CWUF                     PWR_CR_CWUF            /*!< Clear wakeup flag */
/**
  * @}
  */

/** @defgroup PWR_LL_EC_GET_FLAG Get Flags Defines
  * @brief    Flags defines which can be used with LL_PWR_ReadReg function
  * @{
  */
#define LL_PWR_CSR_WUF                     PWR_CSR_WUF            /*!< Wakeup flag */
#define LL_PWR_CSR_SBF                     PWR_CSR_SBF            /*!< Standby flag */
#if defined(PWR_PVD_SUPPORT)
#define LL_PWR_CSR_PVDO                    PWR_CSR_PVDO           /*!< Power voltage detector output flag */
#endif /* PWR_PVD_SUPPORT */
#if defined(PWR_CSR_VREFINTRDYF)
#define LL_PWR_CSR_VREFINTRDYF             PWR_CSR_VREFINTRDYF    /*!< VREFINT ready flag */
#endif /* PWR_CSR_VREFINTRDYF */
#define LL_PWR_CSR_VOS                     PWR_CSR_VOSF           /*!< Voltage scaling select flag */
#define LL_PWR_CSR_REGLPF                  PWR_CSR_REGLPF         /*!< Regulator low power flag */
#define LL_PWR_CSR_EWUP1                   PWR_CSR_EWUP1          /*!< Enable WKUP pin 1 */
#define LL_PWR_CSR_EWUP2                   PWR_CSR_EWUP2          /*!< Enable WKUP pin 2 */
#if defined(PWR_CSR_EWUP3)
#define LL_PWR_CSR_EWUP3                   PWR_CSR_EWUP3          /*!< Enable WKUP pin 3 */
#endif /* PWR_CSR_EWUP3 */
/**
  * @}
  */

/** @defgroup PWR_LL_EC_REGU_VOLTAGE Regulator Voltage
  * @{
  */
#define LL_PWR_REGU_VOLTAGE_SCALE1         (PWR_CR_VOS_0)                   /*!< 1.8V (range 1) */
#define LL_PWR_REGU_VOLTAGE_SCALE2         (PWR_CR_VOS_1)                   /*!< 1.5V (range 2) */
#define LL_PWR_REGU_VOLTAGE_SCALE3         (PWR_CR_VOS_0 | PWR_CR_VOS_1)    /*!< 1.2V (range 3) */
/**
  * @}
  */

/** @defgroup PWR_LL_EC_MODE_PWR Mode Power
  * @{
  */
#define LL_PWR_MODE_STOP                      0x00000000U                    /*!< Enter Stop mode when the CPU enters deepsleep */
#define LL_PWR_MODE_STANDBY                   (PWR_CR_PDDS)                  /*!< Enter Standby mode when the CPU enters deepsleep */
/**
  * @}
  */

/** @defgroup PWR_LL_EC_REGU_MODE_LP_MODES  Regulator Mode In Low Power Modes
  * @{
  */
#define LL_PWR_REGU_LPMODES_MAIN           0x00000000U        /*!< Voltage Regulator in main mode during deepsleep/sleep/low-power run mode */
#define LL_PWR_REGU_LPMODES_LOW_POWER      (PWR_CR_LPSDSR)    /*!< Voltage Regulator in low-power mode during deepsleep/sleep/low-power run mode */
/**
  * @}
  */
#if defined(PWR_CR_LPDS)
/** @defgroup PWR_LL_EC_REGU_MODE_DS_MODE  Regulator Mode In Deep Sleep Mode
 * @{
 */
#define LL_PWR_REGU_DSMODE_MAIN        0x00000000U           /*!< Voltage Regulator in main mode during deepsleep mode */
#define LL_PWR_REGU_DSMODE_LOW_POWER   (PWR_CR_LPDS)         /*!< Voltage Regulator in low-power mode during deepsleep mode */
/**
  * @}
  */
#endif /* PWR_CR_LPDS */

#if defined(PWR_PVD_SUPPORT)
/** @defgroup PWR_LL_EC_PVDLEVEL Power Voltage Detector Level
  * @{
  */
#define LL_PWR_PVDLEVEL_0                  (PWR_CR_PLS_LEV0)      /*!< Voltage threshold detected by PVD 1.9 V */
#define LL_PWR_PVDLEVEL_1                  (PWR_CR_PLS_LEV1)      /*!< Voltage threshold detected by PVD 2.1 V */
#define LL_PWR_PVDLEVEL_2                  (PWR_CR_PLS_LEV2)      /*!< Voltage threshold detected by PVD 2.3 V */
#define LL_PWR_PVDLEVEL_3                  (PWR_CR_PLS_LEV3)      /*!< Voltage threshold detected by PVD 2.5 V */
#define LL_PWR_PVDLEVEL_4                  (PWR_CR_PLS_LEV4)      /*!< Voltage threshold detected by PVD 2.7 V */
#define LL_PWR_PVDLEVEL_5                  (PWR_CR_PLS_LEV5)      /*!< Voltage threshold detected by PVD 2.9 V */
#define LL_PWR_PVDLEVEL_6                  (PWR_CR_PLS_LEV6)      /*!< Voltage threshold detected by PVD 3.1 V */
#define LL_PWR_PVDLEVEL_7                  (PWR_CR_PLS_LEV7)      /*!< External input analog voltage   (Compare internally to VREFINT) */
/**
  * @}
  */
#endif /* PWR_PVD_SUPPORT */
/** @defgroup PWR_LL_EC_WAKEUP_PIN  Wakeup Pins
  * @{
  */
#define LL_PWR_WAKEUP_PIN1                 (PWR_CSR_EWUP1)        /*!< WKUP pin 1 : PA0 */
#define LL_PWR_WAKEUP_PIN2                 (PWR_CSR_EWUP2)        /*!< WKUP pin 2 : PC13 */
#if defined(PWR_CSR_EWUP3)
#define LL_PWR_WAKEUP_PIN3                 (PWR_CSR_EWUP3)        /*!< WKUP pin 3 : PE6 or PA2 according to device */
#endif /* PWR_CSR_EWUP3 */
/**
  * @}
  */

/**
  * @}
  */


/* Exported macro ------------------------------------------------------------*/
/** @defgroup PWR_LL_Exported_Macros PWR Exported Macros
  * @{
  */

/** @defgroup PWR_LL_EM_WRITE_READ Common write and read registers Macros
  * @{
  */

/**
  * @brief  Write a value in PWR register
  * @param  __REG__ Register to be written
  * @param  __VALUE__ Value to be written in the register
  * @retval None
  */
#define LL_PWR_WriteReg(__REG__, __VALUE__) WRITE_REG(PWR->__REG__, (__VALUE__))

/**
  * @brief  Read a value in PWR register
  * @param  __REG__ Register to be read
  * @retval Register value
  */
#define LL_PWR_ReadReg(__REG__) READ_REG(PWR->__REG__)
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup PWR_LL_Exported_Functions PWR Exported Functions
  * @{
  */

/** @defgroup PWR_LL_EF_Configuration Configuration
  * @{
  */
/**
  * @brief  Switch the Regulator from main mode to low-power mode
  * @rmtoll CR    LPRUN       LL_PWR_EnableLowPowerRunMode
  * @note   Remind to set the Regulator to low power before enabling
  *         LowPower run mode (bit @ref LL_PWR_REGU_LPMODES_LOW_POWER).
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableLowPowerRunMode(void)
{
  SET_BIT(PWR->CR, PWR_CR_LPRUN);
}

/**
  * @brief  Switch the Regulator from low-power mode to main mode
  * @rmtoll CR    LPRUN       LL_PWR_DisableLowPowerRunMode
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableLowPowerRunMode(void)
{
  CLEAR_BIT(PWR->CR, PWR_CR_LPRUN);
}

/**
  * @brief  Check if the Regulator is in low-power mode
  * @rmtoll CR    LPRUN       LL_PWR_IsEnabledLowPowerRunMode
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledLowPowerRunMode(void)
{
  return ((READ_BIT(PWR->CR, PWR_CR_LPRUN) == PWR_CR_LPRUN) ? 1UL : 0UL);
}

/**
  * @brief  Set voltage Regulator to low-power and switch from
  *         run main mode to run low-power mode.
  * @rmtoll CR    LPSDSR       LL_PWR_EnterLowPowerRunMode\n
  *         CR    LPRUN        LL_PWR_EnterLowPowerRunMode
  * @note   This "high level" function is introduced to provide functional
  *         compatibility with other families. Notice that the two registers
  *         have to be written sequentially, so this function is not atomic.
  *         To assure atomicity you can call separately the following functions:
  *         - @ref LL_PWR_SetRegulModeLP(@ref LL_PWR_REGU_LPMODES_LOW_POWER);
  *         - @ref LL_PWR_EnableLowPowerRunMode();
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnterLowPowerRunMode(void)
{
  SET_BIT(PWR->CR, PWR_CR_LPSDSR); /* => LL_PWR_SetRegulModeLP(LL_PWR_REGU_LPMODES_LOW_POWER) */
  SET_BIT(PWR->CR, PWR_CR_LPRUN);  /* => LL_PWR_EnableLowPowerRunMode() */
}

/**
  * @brief  Set voltage Regulator to main and switch from
  *         run main mode to low-power mode.
  * @rmtoll CR    LPSDSR       LL_PWR_ExitLowPowerRunMode\n
  *         CR    LPRUN        LL_PWR_ExitLowPowerRunMode
  * @note   This "high level" function is introduced to provide functional
  *         compatibility with other families. Notice that the two registers
  *         have to be written sequentially, so this function is not atomic.
  *         To assure atomicity you can call separately the following functions:
  *         - @ref LL_PWR_DisableLowPowerRunMode();
  *         - @ref LL_PWR_SetRegulModeLP(@ref LL_PWR_REGU_LPMODES_MAIN);
  * @retval None
  */
__STATIC_INLINE void LL_PWR_ExitLowPowerRunMode(void)
{
  CLEAR_BIT(PWR->CR, PWR_CR_LPRUN);   /* => LL_PWR_DisableLowPowerRunMode() */
  CLEAR_BIT(PWR->CR, PWR_CR_LPSDSR);  /* => LL_PWR_SetRegulModeLP(LL_PWR_REGU_LPMODES_MAIN) */
}
/**
  * @brief  Set the main internal Regulator output voltage
  * @rmtoll CR    VOS       LL_PWR_SetRegulVoltageScaling
  * @param  VoltageScaling This parameter can be one of the following values:
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SCALE1
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SCALE2
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SCALE3
  * @retval None
  */
__STATIC_INLINE void LL_PWR_SetRegulVoltageScaling(uint32_t VoltageScaling)
{
  MODIFY_REG(PWR->CR, PWR_CR_VOS, VoltageScaling);
}

/**
  * @brief  Get the main internal Regulator output voltage
  * @rmtoll CR    VOS       LL_PWR_GetRegulVoltageScaling
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SCALE1
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SCALE2
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SCALE3
  */
__STATIC_INLINE uint32_t LL_PWR_GetRegulVoltageScaling(void)
{
  return (uint32_t)(READ_BIT(PWR->CR, PWR_CR_VOS));
}

/**
  * @brief  Enable access to the backup domain
  * @rmtoll CR    DBP       LL_PWR_EnableBkUpAccess
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableBkUpAccess(void)
{
  SET_BIT(PWR->CR, PWR_CR_DBP);
}

/**
  * @brief  Disable access to the backup domain
  * @rmtoll CR    DBP       LL_PWR_DisableBkUpAccess
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableBkUpAccess(void)
{
  CLEAR_BIT(PWR->CR, PWR_CR_DBP);
}

/**
  * @brief  Check if the backup domain is enabled
  * @rmtoll CR    DBP       LL_PWR_IsEnabledBkUpAccess
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledBkUpAccess(void)
{
  return ((READ_BIT(PWR->CR, PWR_CR_DBP) == PWR_CR_DBP) ? 1UL : 0UL);
}

/**
  * @brief  Set voltage Regulator mode during low power modes
  * @rmtoll CR    LPSDSR       LL_PWR_SetRegulModeLP
  * @param  RegulMode This parameter can be one of the following values:
  *         @arg @ref LL_PWR_REGU_LPMODES_MAIN
  *         @arg @ref LL_PWR_REGU_LPMODES_LOW_POWER
  * @retval None
  */
__STATIC_INLINE void LL_PWR_SetRegulModeLP(uint32_t RegulMode)
{
  MODIFY_REG(PWR->CR, PWR_CR_LPSDSR, RegulMode);
}

/**
  * @brief  Get voltage Regulator mode during low power modes
  * @rmtoll CR    LPSDSR       LL_PWR_GetRegulModeLP
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_PWR_REGU_LPMODES_MAIN
  *         @arg @ref LL_PWR_REGU_LPMODES_LOW_POWER
  */
__STATIC_INLINE uint32_t LL_PWR_GetRegulModeLP(void)
{
  return (uint32_t)(READ_BIT(PWR->CR, PWR_CR_LPSDSR));
}

#if defined(PWR_CR_LPDS)
/**
  * @brief  Set voltage Regulator mode during deep sleep mode
  * @rmtoll CR    LPDS         LL_PWR_SetRegulModeDS
  * @param  RegulMode This parameter can be one of the following values:
  *         @arg @ref LL_PWR_REGU_DSMODE_MAIN
  *         @arg @ref LL_PWR_REGU_DSMODE_LOW_POWER
  * @retval None
  */
__STATIC_INLINE void LL_PWR_SetRegulModeDS(uint32_t RegulMode)
{
  MODIFY_REG(PWR->CR, PWR_CR_LPDS, RegulMode);
}

/**
  * @brief  Get voltage Regulator mode during deep sleep mode
  * @rmtoll CR    LPDS         LL_PWR_GetRegulModeDS
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_PWR_REGU_DSMODE_MAIN
  *         @arg @ref LL_PWR_REGU_DSMODE_LOW_POWER
  */
__STATIC_INLINE uint32_t LL_PWR_GetRegulModeDS(void)
{
  return (uint32_t)(READ_BIT(PWR->CR, PWR_CR_LPDS));
}
#endif /* PWR_CR_LPDS */

/**
  * @brief  Set Power Down mode when CPU enters deepsleep
  * @rmtoll CR    PDDS         LL_PWR_SetPowerMode
  * @param  PDMode This parameter can be one of the following values:
  *         @arg @ref LL_PWR_MODE_STOP
  *         @arg @ref LL_PWR_MODE_STANDBY
  * @note   Set the Regulator to low power (bit @ref LL_PWR_REGU_LPMODES_LOW_POWER)
  *         before setting MODE_STOP. If the Regulator remains in "main mode",
  *         it consumes more power without providing any additional feature.
  *         In MODE_STANDBY the Regulator is automatically off.
  * @retval None
  */
__STATIC_INLINE void LL_PWR_SetPowerMode(uint32_t PDMode)
{
  MODIFY_REG(PWR->CR, PWR_CR_PDDS, PDMode);
}

/**
  * @brief  Get Power Down mode when CPU enters deepsleep
  * @rmtoll CR    PDDS         LL_PWR_GetPowerMode
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_PWR_MODE_STOP
  *         @arg @ref LL_PWR_MODE_STANDBY
  */
__STATIC_INLINE uint32_t LL_PWR_GetPowerMode(void)
{
  return (uint32_t)(READ_BIT(PWR->CR, PWR_CR_PDDS));
}

#if defined(PWR_PVD_SUPPORT)
/**
  * @brief  Configure the voltage threshold detected by the Power Voltage Detector
  * @rmtoll CR    PLS       LL_PWR_SetPVDLevel
  * @param  PVDLevel This parameter can be one of the following values:
  *         @arg @ref LL_PWR_PVDLEVEL_0
  *         @arg @ref LL_PWR_PVDLEVEL_1
  *         @arg @ref LL_PWR_PVDLEVEL_2
  *         @arg @ref LL_PWR_PVDLEVEL_3
  *         @arg @ref LL_PWR_PVDLEVEL_4
  *         @arg @ref LL_PWR_PVDLEVEL_5
  *         @arg @ref LL_PWR_PVDLEVEL_6
  *         @arg @ref LL_PWR_PVDLEVEL_7
  * @retval None
  */
__STATIC_INLINE void LL_PWR_SetPVDLevel(uint32_t PVDLevel)
{
  MODIFY_REG(PWR->CR, PWR_CR_PLS, PVDLevel);
}

/**
  * @brief  Get the voltage threshold detection
  * @rmtoll CR    PLS       LL_PWR_GetPVDLevel
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_PWR_PVDLEVEL_0
  *         @arg @ref LL_PWR_PVDLEVEL_1
  *         @arg @ref LL_PWR_PVDLEVEL_2
  *         @arg @ref LL_PWR_PVDLEVEL_3
  *         @arg @ref LL_PWR_PVDLEVEL_4
  *         @arg @ref LL_PWR_PVDLEVEL_5
  *         @arg @ref LL_PWR_PVDLEVEL_6
  *         @arg @ref LL_PWR_PVDLEVEL_7
  */
__STATIC_INLINE uint32_t LL_PWR_GetPVDLevel(void)
{
  return (uint32_t)(READ_BIT(PWR->CR, PWR_CR_PLS));
}

/**
  * @brief  Enable Power Voltage Detector
  * @rmtoll CR    PVDE       LL_PWR_EnablePVD
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnablePVD(void)
{
  SET_BIT(PWR->CR, PWR_CR_PVDE);
}

/**
  * @brief  Disable Power Voltage Detector
  * @rmtoll CR    PVDE       LL_PWR_DisablePVD
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisablePVD(void)
{
  CLEAR_BIT(PWR->CR, PWR_CR_PVDE);
}

/**
  * @brief  Check if Power Voltage Detector is enabled
  * @rmtoll CR    PVDE       LL_PWR_IsEnabledPVD
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledPVD(void)
{
  return ((READ_BIT(PWR->CR, PWR_CR_PVDE) == PWR_CR_PVDE) ? 1UL : 0UL);
}
#endif /* PWR_PVD_SUPPORT */

/**
  * @brief  Enable the WakeUp PINx functionality
  * @rmtoll CSR   EWUP1       LL_PWR_EnableWakeUpPin\n
  * @rmtoll CSR   EWUP2       LL_PWR_EnableWakeUpPin\n
  * @rmtoll CSR   EWUP3       LL_PWR_EnableWakeUpPin
  * @param  WakeUpPin This parameter can be one of the following values:
  *         @arg @ref LL_PWR_WAKEUP_PIN1
  *         @arg @ref LL_PWR_WAKEUP_PIN2
  *         @arg @ref LL_PWR_WAKEUP_PIN3 (*)
  *
  *         (*) not available on all devices
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableWakeUpPin(uint32_t WakeUpPin)
{
  SET_BIT(PWR->CSR, WakeUpPin);
}

/**
  * @brief  Disable the WakeUp PINx functionality
  * @rmtoll CSR   EWUP1       LL_PWR_DisableWakeUpPin\n
  * @rmtoll CSR   EWUP2       LL_PWR_DisableWakeUpPin\n
  * @rmtoll CSR   EWUP3       LL_PWR_DisableWakeUpPin
  * @param  WakeUpPin This parameter can be one of the following values:
  *         @arg @ref LL_PWR_WAKEUP_PIN1
  *         @arg @ref LL_PWR_WAKEUP_PIN2
  *         @arg @ref LL_PWR_WAKEUP_PIN3 (*)
  *
  *         (*) not available on all devices
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableWakeUpPin(uint32_t WakeUpPin)
{
  CLEAR_BIT(PWR->CSR, WakeUpPin);
}

/**
  * @brief  Check if the WakeUp PINx functionality is enabled
  * @rmtoll CSR   EWUP1       LL_PWR_IsEnabledWakeUpPin\n
  * @rmtoll CSR   EWUP2       LL_PWR_IsEnabledWakeUpPin\n
  * @rmtoll CSR   EWUP3       LL_PWR_IsEnabledWakeUpPin
  * @param  WakeUpPin This parameter can be one of the following values:
  *         @arg @ref LL_PWR_WAKEUP_PIN1
  *         @arg @ref LL_PWR_WAKEUP_PIN2
  *         @arg @ref LL_PWR_WAKEUP_PIN3 (*)
  *
  *         (*) not available on all devices
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledWakeUpPin(uint32_t WakeUpPin)
{
  return ((READ_BIT(PWR->CSR, WakeUpPin) == WakeUpPin) ? 1UL : 0UL);
}

/**
  * @brief  Enable ultra low-power mode by enabling VREFINT switch off in low-power modes
  * @rmtoll CR    ULP       LL_PWR_EnableUltraLowPower
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableUltraLowPower(void)
{
  SET_BIT(PWR->CR, PWR_CR_ULP);
}

/**
  * @brief  Disable ultra low-power mode by disabling VREFINT switch off in low-power modes
  * @rmtoll CR    ULP       LL_PWR_DisableUltraLowPower
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableUltraLowPower(void)
{
  CLEAR_BIT(PWR->CR, PWR_CR_ULP);
}

/**
  * @brief  Check if ultra low-power mode is enabled by checking if VREFINT switch off in low-power modes is enabled
  * @rmtoll CR    ULP       LL_PWR_IsEnabledUltraLowPower
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledUltraLowPower(void)
{
  return ((READ_BIT(PWR->CR, PWR_CR_ULP) == PWR_CR_ULP) ? 1UL : 0UL);
}

/**
  * @brief  Enable fast wakeup by ignoring VREFINT startup time when exiting from low-power mode
  * @rmtoll CR    FWU       LL_PWR_EnableFastWakeUp
  * @note   Works in conjunction with ultra low power mode.
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableFastWakeUp(void)
{
  SET_BIT(PWR->CR, PWR_CR_FWU);
}

/**
  * @brief  Disable fast wakeup by waiting VREFINT startup time when exiting from low-power mode
  * @rmtoll CR    FWU       LL_PWR_DisableFastWakeUp
  * @note   Works in conjunction with ultra low power mode.
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableFastWakeUp(void)
{
  CLEAR_BIT(PWR->CR, PWR_CR_FWU);
}

/**
  * @brief  Check if fast wakeup is enabled by checking if VREFINT startup time when exiting from low-power mode is ignored
  * @rmtoll CR    FWU       LL_PWR_IsEnabledFastWakeUp
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledFastWakeUp(void)
{
  return ((READ_BIT(PWR->CR, PWR_CR_FWU) == PWR_CR_FWU) ? 1UL : 0UL);
}


/**
  * @}
  */

/** @defgroup PWR_LL_EF_FLAG_Management FLAG_Management
  * @{
  */

/**
  * @brief  Get Wake-up Flag
  * @rmtoll CSR   WUF       LL_PWR_IsActiveFlag_WU
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_WU(void)
{
  return ((READ_BIT(PWR->CSR, PWR_CSR_WUF) == PWR_CSR_WUF) ? 1UL : 0UL);
}

/**
  * @brief  Get Standby Flag
  * @rmtoll CSR   SBF       LL_PWR_IsActiveFlag_SB
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_SB(void)
{
  return ((READ_BIT(PWR->CSR, PWR_CSR_SBF) == PWR_CSR_SBF) ? 1UL : 0UL);
}

#if defined(PWR_PVD_SUPPORT)
/**
  * @brief  Indicate whether VDD voltage is below the selected PVD threshold
  * @rmtoll CSR   PVDO       LL_PWR_IsActiveFlag_PVDO
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_PVDO(void)
{
  return ((READ_BIT(PWR->CSR, PWR_CSR_PVDO) == PWR_CSR_PVDO) ? 1UL : 0UL);
}
#endif /* PWR_PVD_SUPPORT */

#if defined(PWR_CSR_VREFINTRDYF)
/**
  * @brief  Get Internal Reference VrefInt Flag
  * @rmtoll CSR   VREFINTRDYF       LL_PWR_IsActiveFlag_VREFINTRDY
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_VREFINTRDY(void)
{
  return ((READ_BIT(PWR->CSR, PWR_CSR_VREFINTRDYF) == PWR_CSR_VREFINTRDYF) ? 1UL : 0UL);
}
#endif /* PWR_CSR_VREFINTRDYF */
/**
  * @brief  Indicate whether the Regulator is ready in the selected voltage range or if its output voltage is still changing to the required voltage level
  * @rmtoll CSR   VOSF       LL_PWR_IsActiveFlag_VOS
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_VOS(void)
{
  return ((READ_BIT(PWR->CSR, PWR_CSR_VOSF) == PWR_CSR_VOSF) ? 1UL : 0UL);
}
/**
  * @brief Indicate whether the Regulator is ready in main mode or is in low-power mode
  * @rmtoll CSR   REGLPF       LL_PWR_IsActiveFlag_REGLPF
  * @note Take care, return value "0" means the Regulator is ready.  Return value "1" means the output voltage range is still changing.
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_REGLPF(void)
{
  return ((READ_BIT(PWR->CSR, PWR_CSR_REGLPF) == PWR_CSR_REGLPF) ? 1UL : 0UL);
}
/**
  * @brief  Clear Standby Flag
  * @rmtoll CR   CSBF       LL_PWR_ClearFlag_SB
  * @retval None
  */
__STATIC_INLINE void LL_PWR_ClearFlag_SB(void)
{
  SET_BIT(PWR->CR, PWR_CR_CSBF);
}

/**
  * @brief  Clear Wake-up Flags
  * @rmtoll CR   CWUF       LL_PWR_ClearFlag_WU
  * @retval None
  */
__STATIC_INLINE void LL_PWR_ClearFlag_WU(void)
{
  SET_BIT(PWR->CR, PWR_CR_CWUF);
}

/**
  * @}
  */

#if defined(USE_FULL_LL_DRIVER)
/** @defgroup PWR_LL_EF_Init De-initialization function
  * @{
  */
ErrorStatus LL_PWR_DeInit(void);
/**
  * @}
  */
#endif /* USE_FULL_LL_DRIVER */

/** @defgroup PWR_LL_EF_Legacy_Functions PWR legacy functions name
  * @{
  */
/* Old functions name kept for legacy purpose, to be replaced by the          */
/* current functions name.                                                    */
#define LL_PWR_IsActiveFlag_VOSF  LL_PWR_IsActiveFlag_VOS
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* defined(PWR) */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32L1xx_LL_PWR_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
