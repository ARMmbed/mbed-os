/**
  ******************************************************************************
  * @file    stm32l1xx_hal_rtc_ex.h
  * @author  MCD Application Team
  * @brief   Header file of RTC HAL Extended module.
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
#ifndef __STM32L1xx_HAL_RTC_EX_H
#define __STM32L1xx_HAL_RTC_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_hal_def.h"

/** @addtogroup STM32L1xx_HAL_Driver
  * @{
  */

/** @defgroup RTCEx RTCEx
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup RTCEx_Exported_Types RTCEx Exported Types
  * @{
  */

/**
  * @brief  RTC Tamper structure definition
  */
typedef struct
{
  uint32_t Tamper;                      /*!< Specifies the Tamper Pin.
                                             This parameter can be a value of @ref  RTCEx_Tamper_Pins_Definitions */

  uint32_t Trigger;                     /*!< Specifies the Tamper Trigger.
                                             This parameter can be a value of @ref  RTCEx_Tamper_Trigger_Definitions */

#if defined(STM32L100xBA) || defined (STM32L151xBA) || defined (STM32L152xBA) || defined(STM32L100xC) || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC) || defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
  uint32_t Filter;                      /*!< Specifies the RTC Filter Tamper.
                                             This parameter can be a value of @ref RTCEx_Tamper_Filter_Definitions */

  uint32_t SamplingFrequency;           /*!< Specifies the sampling frequency.
                                             This parameter can be a value of @ref RTCEx_Tamper_Sampling_Frequencies_Definitions */

  uint32_t PrechargeDuration;           /*!< Specifies the Precharge Duration .
                                             This parameter can be a value of @ref RTCEx_Tamper_Pin_Precharge_Duration_Definitions */

  uint32_t TamperPullUp;                /*!< Specifies the Tamper PullUp .
                                             This parameter can be a value of @ref RTCEx_Tamper_Pull_Up_Definitions */

  uint32_t TimeStampOnTamperDetection;  /*!< Specifies the TimeStampOnTamperDetection.
                                             This parameter can be a value of @ref RTCEx_Tamper_TimeStampOnTamperDetection_Definitions */
#endif /* STM32L100xBA || STM32L151xBA || STM32L152xBA || STM32L100xC || STM32L151xC || STM32L152xC || STM32L162xC || STM32L151xCA || STM32L151xD || STM32L152xCA || STM32L152xD || STM32L162xCA || STM32L162xD || STM32L151xE || STM32L151xDX || STM32L152xE || STM32L152xDX || STM32L162xE || STM32L162xDX */
} RTC_TamperTypeDef;
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup RTCEx_Exported_Constants RTCEx Exported Constants
  * @{
  */

/** @defgroup RTCEx_Output_selection_Definition RTCEx Output Selection Definition
  * @{
  */
#define RTC_TR_RESERVED_MASK    (0x007F7F7FU)
#define RTC_DR_RESERVED_MASK    (0x00FFFF3FU)
#define RTC_INIT_MASK           (0xFFFFFFFFU)
#define RTC_RSF_MASK            (0xFFFFFF5FU)

#if defined(STM32L100xBA) || defined (STM32L151xBA) || defined (STM32L152xBA) || defined(STM32L100xC) || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC) || defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
#define RTC_FLAGS_MASK          ((uint32_t)(RTC_FLAG_ALRAWF | RTC_FLAG_ALRBWF | RTC_FLAG_WUTWF | \
                                            RTC_FLAG_SHPF | RTC_FLAG_INITS | RTC_FLAG_RSF | \
                                            RTC_FLAG_INITF | RTC_FLAG_ALRAF | RTC_FLAG_ALRBF | \
                                            RTC_FLAG_WUTF | RTC_FLAG_TSF | RTC_FLAG_TSOVF | \
                                            RTC_FLAG_TAMP1F | RTC_FLAG_TAMP2F | RTC_FLAG_TAMP3F | \
                                            RTC_FLAG_RECALPF))
#else
#define RTC_FLAGS_MASK          ((uint32_t)(RTC_FLAG_ALRAWF | RTC_FLAG_ALRBWF | RTC_FLAG_WUTWF | \
                                            RTC_FLAG_SHPF | RTC_FLAG_INITS | RTC_FLAG_RSF | \
                                            RTC_FLAG_INITF | RTC_FLAG_ALRAF | RTC_FLAG_ALRBF | \
                                            RTC_FLAG_WUTF | RTC_FLAG_TSF | RTC_FLAG_TSOVF | \
                                            RTC_FLAG_TAMP1F | \
                                            RTC_FLAG_RECALPF))

#endif /* STM32L100xBA || STM32L151xBA || STM32L152xBA || STM32L100xC || STM32L151xC || STM32L152xC || STM32L162xC || STM32L151xCA || STM32L151xD || STM32L152xCA || STM32L152xD || STM32L162xCA || STM32L162xD || STM32L151xE || STM32L151xDX || STM32L152xE || STM32L152xDX || STM32L162xE || STM32L162xDX */
/**
  * @}
  */

/** @defgroup RTCEx_Backup_Registers_Definition RTCEx Backup Registers Definition
  * @{
  */
#if defined(STM32L100xBA) || defined (STM32L151xBA) || defined (STM32L152xBA) || defined(STM32L100xC) || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC) || defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
#define IS_RTC_SYNCH_PREDIV(PREDIV)    ((PREDIV) <= 0x7FFFU)
#elif defined(STM32L100xB) || defined (STM32L151xB) || defined (STM32L152xB)
#define IS_RTC_SYNCH_PREDIV(PREDIV)    ((PREDIV) <= 0x1FFFU)
#endif /* STM32L100xBA || STM32L151xBA || STM32L152xBA || STM32L100xC || STM32L151xC || STM32L152xC || STM32L162xC || STM32L151xCA || STM32L151xD || STM32L152xCA || STM32L152xD || STM32L162xCA || STM32L162xD || STM32L151xE || STM32L151xDX || STM32L152xE || STM32L152xDX || STM32L162xE || STM32L162xDX */
/**
  * @}
  */

/** @defgroup RTC_Interrupts_Definitions Interrupts Definitions
  * @{
  */
#define RTC_IT_TS                         ((uint32_t)RTC_CR_TSIE)
#define RTC_IT_WUT                        ((uint32_t)RTC_CR_WUTIE)
#define RTC_IT_ALRB                       ((uint32_t)RTC_CR_ALRBIE)
#define RTC_IT_ALRA                       ((uint32_t)RTC_CR_ALRAIE)
#define RTC_IT_TAMP1                      ((uint32_t)(RTC_TAFCR_TAMPIE | RTC_TAFCR_TAMP1E))
#if defined(STM32L100xBA) || defined (STM32L151xBA) || defined (STM32L152xBA) || defined(STM32L100xC) || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC) || defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
#define RTC_IT_TAMP2                      ((uint32_t)(RTC_TAFCR_TAMPIE | RTC_TAFCR_TAMP2E))
#define RTC_IT_TAMP3                      ((uint32_t)(RTC_TAFCR_TAMPIE | RTC_TAFCR_TAMP3E))
#endif /* STM32L100xBA || STM32L151xBA || STM32L152xBA || STM32L100xC || STM32L151xC || STM32L152xC || STM32L162xC || STM32L151xCA || STM32L151xD || STM32L152xCA || STM32L152xD || STM32L162xCA || STM32L162xD || STM32L151xE || STM32L151xDX || STM32L152xE || STM32L152xDX || STM32L162xE || STM32L162xDX */
/**
  * @}
  */

/** @defgroup RTC_Flags_Definitions Flags Definitions
  * @{
  */
#define RTC_FLAG_RECALPF                  ((uint32_t)RTC_ISR_RECALPF)
#if defined(STM32L100xBA) || defined (STM32L151xBA) || defined (STM32L152xBA) || defined(STM32L100xC) || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC) || defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
#define RTC_FLAG_TAMP3F                   ((uint32_t)RTC_ISR_TAMP3F)
#define RTC_FLAG_TAMP2F                   ((uint32_t)RTC_ISR_TAMP2F)
#endif /* STM32L100xBA || STM32L151xBA || STM32L152xBA || STM32L100xC || STM32L151xC || STM32L152xC || STM32L162xC || STM32L151xCA || STM32L151xD || STM32L152xCA || STM32L152xD || STM32L162xCA || STM32L162xD || STM32L151xE || STM32L151xDX || STM32L152xE || STM32L152xDX || STM32L162xE || STM32L162xDX */
#define RTC_FLAG_TAMP1F                   ((uint32_t)RTC_ISR_TAMP1F)
#define RTC_FLAG_TSOVF                    ((uint32_t)RTC_ISR_TSOVF)
#define RTC_FLAG_TSF                      ((uint32_t)RTC_ISR_TSF)
#define RTC_FLAG_WUTF                     ((uint32_t)RTC_ISR_WUTF)
#define RTC_FLAG_ALRBF                    ((uint32_t)RTC_ISR_ALRBF)
#define RTC_FLAG_ALRAF                    ((uint32_t)RTC_ISR_ALRAF)
#define RTC_FLAG_INITF                    ((uint32_t)RTC_ISR_INITF)
#define RTC_FLAG_RSF                      ((uint32_t)RTC_ISR_RSF)
#define RTC_FLAG_INITS                    ((uint32_t)RTC_ISR_INITS)
#if defined(STM32L100xBA) || defined (STM32L151xBA) || defined (STM32L152xBA) || defined(STM32L100xC) || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC) || defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
#define RTC_FLAG_SHPF                     ((uint32_t)RTC_ISR_SHPF)
#endif /* STM32L100xBA || STM32L151xBA || STM32L152xBA || STM32L100xC || STM32L151xC || STM32L152xC || STM32L162xC || STM32L151xCA || STM32L151xD || STM32L152xCA || STM32L152xD || STM32L162xCA || STM32L162xD || STM32L151xE || STM32L151xDX || STM32L152xE || STM32L152xDX || STM32L162xE || STM32L162xDX */
#define RTC_FLAG_WUTWF                    ((uint32_t)RTC_ISR_WUTWF)
#define RTC_FLAG_ALRBWF                   ((uint32_t)RTC_ISR_ALRBWF)
#define RTC_FLAG_ALRAWF                   ((uint32_t)RTC_ISR_ALRAWF)
/**
  * @}
  */

/** @defgroup RTCEx_Output_selection_Definitions Output selection Definitions
  * @{
  */
#define RTC_OUTPUT_DISABLE             (0x00000000U)
#define RTC_OUTPUT_ALARMA              (0x00200000U)
#define RTC_OUTPUT_ALARMB              (0x00400000U)
#define RTC_OUTPUT_WAKEUP              (0x00600000U)

#define IS_RTC_OUTPUT(OUTPUT) (((OUTPUT) == RTC_OUTPUT_DISABLE) || \
                               ((OUTPUT) == RTC_OUTPUT_ALARMA)  || \
                               ((OUTPUT) == RTC_OUTPUT_ALARMB)  || \
                               ((OUTPUT) == RTC_OUTPUT_WAKEUP))
/**
  * @}
  */

/** @defgroup RTCEx_Backup_Registers_Definitions Backup Registers Definitions
  * @{
  */
#if RTC_BKP_NUMBER > 0
#define RTC_BKP_DR0                       (0x00000000U)
#define RTC_BKP_DR1                       (0x00000001U)
#define RTC_BKP_DR2                       (0x00000002U)
#define RTC_BKP_DR3                       (0x00000003U)
#define RTC_BKP_DR4                       (0x00000004U)
#endif /* RTC_BKP_NUMBER > 0 */

#if RTC_BKP_NUMBER > 5
#define RTC_BKP_DR5                       (0x00000005U)
#define RTC_BKP_DR6                       (0x00000006U)
#define RTC_BKP_DR7                       (0x00000007U)
#define RTC_BKP_DR8                       (0x00000008U)
#define RTC_BKP_DR9                       (0x00000009U)
#define RTC_BKP_DR10                      (0x0000000AU)
#define RTC_BKP_DR11                      (0x0000000BU)
#define RTC_BKP_DR12                      (0x0000000CU)
#define RTC_BKP_DR13                      (0x0000000DU)
#define RTC_BKP_DR14                      (0x0000000EU)
#define RTC_BKP_DR15                      (0x0000000FU)
#define RTC_BKP_DR16                      (0x00000010U)
#define RTC_BKP_DR17                      (0x00000011U)
#define RTC_BKP_DR18                      (0x00000012U)
#define RTC_BKP_DR19                      (0x00000013U)
#endif /* RTC_BKP_NUMBER > 5 */

#if RTC_BKP_NUMBER > 20
#define RTC_BKP_DR20                      (0x00000014U)
#define RTC_BKP_DR21                      (0x00000015U)
#define RTC_BKP_DR22                      (0x00000016U)
#define RTC_BKP_DR23                      (0x00000017U)
#define RTC_BKP_DR24                      (0x00000018U)
#define RTC_BKP_DR25                      (0x00000019U)
#define RTC_BKP_DR26                      (0x0000001AU)
#define RTC_BKP_DR27                      (0x0000001BU)
#define RTC_BKP_DR28                      (0x0000001CU)
#define RTC_BKP_DR29                      (0x0000001DU)
#define RTC_BKP_DR30                      (0x0000001EU)
#define RTC_BKP_DR31                      (0x0000001FU)
#endif /* RTC_BKP_NUMBER > 20 */

#define IS_RTC_BKP(BKP)                   ((BKP) < (uint32_t) RTC_BKP_NUMBER)
/**
  * @}
  */

/** @defgroup RTCEx_Time_Stamp_Edges_Definitions Time Stamp Edges Definitions
  * @{
  */
#define RTC_TIMESTAMPEDGE_RISING          (0x00000000U)
#define RTC_TIMESTAMPEDGE_FALLING         (0x00000008U)

#define IS_TIMESTAMP_EDGE(EDGE) (((EDGE) == RTC_TIMESTAMPEDGE_RISING) || \
                                 ((EDGE) == RTC_TIMESTAMPEDGE_FALLING))
/**
  * @}
  */

/** @defgroup RTCEx_Tamper_Pins_Definitions Tamper Pins Definitions
  * @{
  */
#define RTC_TAMPER_1                    RTC_TAFCR_TAMP1E
#if defined(STM32L100xBA) || defined (STM32L151xBA) || defined (STM32L152xBA) || defined(STM32L100xC) || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC) || defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
#define RTC_TAMPER_2                    RTC_TAFCR_TAMP2E
#define RTC_TAMPER_3                    RTC_TAFCR_TAMP3E
#endif /* STM32L100xBA || STM32L151xBA || STM32L152xBA || STM32L100xC || STM32L151xC || STM32L152xC || STM32L162xC || STM32L151xCA || STM32L151xD || STM32L152xCA || STM32L152xD || STM32L162xCA || STM32L162xD || STM32L151xE || STM32L151xDX || STM32L152xE || STM32L152xDX || STM32L162xE || STM32L162xDX */

#if defined(STM32L100xBA) || defined (STM32L151xBA) || defined (STM32L152xBA) || defined(STM32L100xC) || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC) || defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
#define IS_RTC_TAMPER(TAMPER) (((~(RTC_TAMPER_1|RTC_TAMPER_2|RTC_TAMPER_3) & (TAMPER)) == (uint32_t)RESET) && ((TAMPER) != (uint32_t)RESET))
#else
#define IS_RTC_TAMPER(TAMPER) ((TAMPER) == RTC_TAMPER_1)
#endif /* STM32L100xBA || STM32L151xBA || STM32L152xBA || STM32L100xC || STM32L151xC || STM32L152xC || STM32L162xC || STM32L151xCA || STM32L151xD || STM32L152xCA || STM32L152xD || STM32L162xCA || STM32L162xD || STM32L151xE || STM32L151xDX || STM32L152xE || STM32L152xDX || STM32L162xE || STM32L162xDX */
/**
  * @}
  */

/** @defgroup RTCEx_Tamper_Trigger_Definitions Tamper Trigger Definitions
  * @{
  */
#define RTC_TAMPERTRIGGER_RISINGEDGE       (0x00000000U)
#define RTC_TAMPERTRIGGER_FALLINGEDGE      (0x00000002U)
#if defined(STM32L100xBA) || defined (STM32L151xBA) || defined (STM32L152xBA) || defined(STM32L100xC) || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC) || defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
#define RTC_TAMPERTRIGGER_LOWLEVEL         RTC_TAMPERTRIGGER_RISINGEDGE
#define RTC_TAMPERTRIGGER_HIGHLEVEL        RTC_TAMPERTRIGGER_FALLINGEDGE
#endif /* STM32L100xBA || STM32L151xBA || STM32L152xBA || STM32L100xC || STM32L151xC || STM32L152xC || STM32L162xC || STM32L151xCA || STM32L151xD || STM32L152xCA || STM32L152xD || STM32L162xCA || STM32L162xD || STM32L151xE || STM32L151xDX || STM32L152xE || STM32L152xDX || STM32L162xE || STM32L162xDX */

#if defined(STM32L100xBA) || defined (STM32L151xBA) || defined (STM32L152xBA) || defined(STM32L100xC) || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC) || defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
#define IS_RTC_TAMPER_TRIGGER(TRIGGER) (((TRIGGER) == RTC_TAMPERTRIGGER_RISINGEDGE) || \
                                        ((TRIGGER) == RTC_TAMPERTRIGGER_FALLINGEDGE) || \
                                        ((TRIGGER) == RTC_TAMPERTRIGGER_LOWLEVEL) || \
                                        ((TRIGGER) == RTC_TAMPERTRIGGER_HIGHLEVEL))
#elif defined(STM32L100xB) || defined (STM32L151xB) || defined (STM32L152xB)
#define IS_RTC_TAMPER_TRIGGER(TRIGGER) (((TRIGGER) == RTC_TAMPERTRIGGER_RISINGEDGE) || \
                                        ((TRIGGER) == RTC_TAMPERTRIGGER_FALLINGEDGE))
#endif /* STM32L100xBA || STM32L151xBA || STM32L152xBA || STM32L100xC || STM32L151xC || STM32L152xC || STM32L162xC || STM32L151xCA || STM32L151xD || STM32L152xCA || STM32L152xD || STM32L162xCA || STM32L162xD || STM32L151xE || STM32L151xDX || STM32L152xE || STM32L152xDX || STM32L162xE || STM32L162xDX */
/**
  * @}
  */

#if defined(STM32L100xBA) || defined (STM32L151xBA) || defined (STM32L152xBA) || defined(STM32L100xC) || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC) || defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
/** @defgroup RTCEx_Tamper_Filter_Definitions RTCex Tamper Filter Definitions
  * @{
  */
#define RTC_TAMPERFILTER_DISABLE   (0x00000000U)  /*!< Tamper filter is disabled */

#define RTC_TAMPERFILTER_2SAMPLE   (0x00000800U)  /*!< Tamper is activated after 2 
                                                                consecutive samples at the active level */
#define RTC_TAMPERFILTER_4SAMPLE   (0x00001000U)  /*!< Tamper is activated after 4 
                                                                consecutive samples at the active level */
#define RTC_TAMPERFILTER_8SAMPLE   (0x00001800U)  /*!< Tamper is activated after 8 
                                                                consecutive samples at the active level. */

#define IS_RTC_TAMPER_FILTER(FILTER)  (((FILTER) == RTC_TAMPERFILTER_DISABLE) || \
                                   ((FILTER) == RTC_TAMPERFILTER_2SAMPLE) || \
                                   ((FILTER) == RTC_TAMPERFILTER_4SAMPLE) || \
                                   ((FILTER) == RTC_TAMPERFILTER_8SAMPLE))
/**
  * @}
  */

/** @defgroup RTCEx_Tamper_Sampling_Frequencies_Definitions RTCEx Tamper Sampling Frequencies Definitions
  * @{
  */
#define RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV32768  (0x00000000U)  /*!< Each of the tamper inputs are sampled
                                                                             with a frequency =  RTCCLK / 32768 */
#define RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV16384  (0x00000100U)  /*!< Each of the tamper inputs are sampled
                                                                             with a frequency =  RTCCLK / 16384 */
#define RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV8192   (0x00000200U)  /*!< Each of the tamper inputs are sampled
                                                                             with a frequency =  RTCCLK / 8192  */
#define RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV4096   (0x00000300U)  /*!< Each of the tamper inputs are sampled
                                                                             with a frequency =  RTCCLK / 4096  */
#define RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV2048   (0x00000400U)  /*!< Each of the tamper inputs are sampled
                                                                             with a frequency =  RTCCLK / 2048  */
#define RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV1024   (0x00000500U)  /*!< Each of the tamper inputs are sampled
                                                                             with a frequency =  RTCCLK / 1024  */
#define RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV512    (0x00000600U)  /*!< Each of the tamper inputs are sampled
                                                                             with a frequency =  RTCCLK / 512   */
#define RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV256    (0x00000700U)  /*!< Each of the tamper inputs are sampled
                                                                             with a frequency =  RTCCLK / 256   */

#define IS_RTC_TAMPER_SAMPLING_FREQ(FREQ) (((FREQ) == RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV32768)|| \
                                       ((FREQ) == RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV16384)|| \
                                       ((FREQ) == RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV8192) || \
                                       ((FREQ) == RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV4096) || \
                                       ((FREQ) == RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV2048) || \
                                       ((FREQ) == RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV1024) || \
                                       ((FREQ) == RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV512)  || \
                                       ((FREQ) == RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV256))
/**
  * @}
  */

/** @defgroup RTCEx_Tamper_Pin_Precharge_Duration_Definitions RTCEx Tamper Pin Precharge Duration Definitions
  * @{
  */
#define RTC_TAMPERPRECHARGEDURATION_1RTCCLK (0x00000000U)  /*!< Tamper pins are pre-charged before
                                                                         sampling during 1 RTCCLK cycle */
#define RTC_TAMPERPRECHARGEDURATION_2RTCCLK (0x00002000U)  /*!< Tamper pins are pre-charged before
                                                                         sampling during 2 RTCCLK cycles */
#define RTC_TAMPERPRECHARGEDURATION_4RTCCLK (0x00004000U)  /*!< Tamper pins are pre-charged before
                                                                         sampling during 4 RTCCLK cycles */
#define RTC_TAMPERPRECHARGEDURATION_8RTCCLK (0x00006000U)  /*!< Tamper pins are pre-charged before
                                                                         sampling during 8 RTCCLK cycles */

#define IS_RTC_TAMPER_PRECHARGE_DURATION(DURATION) (((DURATION) == RTC_TAMPERPRECHARGEDURATION_1RTCCLK) || \
                                                ((DURATION) == RTC_TAMPERPRECHARGEDURATION_2RTCCLK) || \
                                                ((DURATION) == RTC_TAMPERPRECHARGEDURATION_4RTCCLK) || \
                                                ((DURATION) == RTC_TAMPERPRECHARGEDURATION_8RTCCLK))
/**
  * @}
  */

/** @defgroup RTCEx_Tamper_TimeStampOnTamperDetection_Definitions TimeStampOnTamperDetection Definitions
  * @{
  */
#define RTC_TIMESTAMPONTAMPERDETECTION_ENABLE  ((uint32_t)RTC_TAFCR_TAMPTS)  /*!< TimeStamp on Tamper Detection event saved */
#define RTC_TIMESTAMPONTAMPERDETECTION_DISABLE (0x00000000U)        /*!< TimeStamp on Tamper Detection event is not saved */

#define IS_RTC_TAMPER_TIMESTAMPONTAMPER_DETECTION(DETECTION) (((DETECTION) == RTC_TIMESTAMPONTAMPERDETECTION_ENABLE) || \
                                                          ((DETECTION) == RTC_TIMESTAMPONTAMPERDETECTION_DISABLE))
/**
  * @}
  */

/** @defgroup RTCEx_Tamper_Pull_Up_Definitions Tamper Pull-Up Definitions
  * @{
  */
#define RTC_TAMPER_PULLUP_ENABLE  (0x00000000U)            /*!< TimeStamp on Tamper Detection event saved */
#define RTC_TAMPER_PULLUP_DISABLE (RTC_TAFCR_TAMPPUDIS)   /*!< TimeStamp on Tamper Detection event is not saved */

#define IS_RTC_TAMPER_PULLUP_STATE(STATE) (((STATE) == RTC_TAMPER_PULLUP_ENABLE) || \
                                       ((STATE) == RTC_TAMPER_PULLUP_DISABLE))
/**
  * @}
  */
#endif /* STM32L100xBA || STM32L151xBA || STM32L152xBA || STM32L100xC || STM32L151xC || STM32L152xC || STM32L162xC || STM32L151xCA || STM32L151xD || STM32L152xCA || STM32L152xD || STM32L162xCA || STM32L162xD || STM32L151xE || STM32L151xDX || STM32L152xE || STM32L152xDX || STM32L162xE || STM32L162xDX */

/** @defgroup RTCEx_Wakeup_Timer_Definitions Wakeup Timer Definitions
  * @{
  */
#define RTC_WAKEUPCLOCK_RTCCLK_DIV16        (0x00000000U)
#define RTC_WAKEUPCLOCK_RTCCLK_DIV8         (0x00000001U)
#define RTC_WAKEUPCLOCK_RTCCLK_DIV4         (0x00000002U)
#define RTC_WAKEUPCLOCK_RTCCLK_DIV2         (0x00000003U)
#define RTC_WAKEUPCLOCK_CK_SPRE_16BITS      (0x00000004U)
#define RTC_WAKEUPCLOCK_CK_SPRE_17BITS      (0x00000006U)

#define IS_RTC_WAKEUP_CLOCK(CLOCK) (((CLOCK) == RTC_WAKEUPCLOCK_RTCCLK_DIV16)   || \
                                ((CLOCK) == RTC_WAKEUPCLOCK_RTCCLK_DIV8)    || \
                                ((CLOCK) == RTC_WAKEUPCLOCK_RTCCLK_DIV4)    || \
                                ((CLOCK) == RTC_WAKEUPCLOCK_RTCCLK_DIV2)    || \
                                ((CLOCK) == RTC_WAKEUPCLOCK_CK_SPRE_16BITS) || \
                                ((CLOCK) == RTC_WAKEUPCLOCK_CK_SPRE_17BITS))

#define IS_RTC_WAKEUP_COUNTER(COUNTER)  ((COUNTER) <= 0xFFFF)
/**
  * @}
  */

/** @defgroup RTCEx_Digital_Calibration_Definitions Digital Calibration Definitions
  * @{
  */
#define RTC_CALIBSIGN_POSITIVE            (0x00000000U)
#define RTC_CALIBSIGN_NEGATIVE            (0x00000080U)

#define IS_RTC_CALIB_SIGN(SIGN) (((SIGN) == RTC_CALIBSIGN_POSITIVE) || \
                                 ((SIGN) == RTC_CALIBSIGN_NEGATIVE))

#define IS_RTC_CALIB_VALUE(VALUE) ((VALUE) < 0x20)
/**
  * @}
  */

/** @defgroup RTCEx_Smooth_Calib_Period_Definitions Smooth Calib Period Definitions
  * @{
  */
#define RTC_SMOOTHCALIB_PERIOD_32SEC   (0x00000000U) /*!<  If RTCCLK = 32768 Hz, Smooth calibation
                                                                    period is 32s,  else 2exp20 RTCCLK seconds */
#define RTC_SMOOTHCALIB_PERIOD_16SEC   (0x00002000U) /*!<  If RTCCLK = 32768 Hz, Smooth calibation
                                                                    period is 16s, else 2exp19 RTCCLK seconds */
#define RTC_SMOOTHCALIB_PERIOD_8SEC    (0x00004000U) /*!<  If RTCCLK = 32768 Hz, Smooth calibation
                                                                    period is 8s, else 2exp18 RTCCLK seconds */

#define IS_RTC_SMOOTH_CALIB_PERIOD(PERIOD) (((PERIOD) == RTC_SMOOTHCALIB_PERIOD_32SEC) || \
                                            ((PERIOD) == RTC_SMOOTHCALIB_PERIOD_16SEC) || \
                                            ((PERIOD) == RTC_SMOOTHCALIB_PERIOD_8SEC))
/**
  * @}
  */

/** @defgroup RTCEx_Smooth_Calib_Plus_Pulses_Definitions Smooth Calib Plus Pulses Definitions
  * @{
  */
#define RTC_SMOOTHCALIB_PLUSPULSES_SET    (0x00008000U) /*!<  The number of RTCCLK pulses added
                                                                       during a X -second window = Y - CALM[8:0]
                                                                       with Y = 512, 256, 128 when X = 32, 16, 8 */
#define RTC_SMOOTHCALIB_PLUSPULSES_RESET  (0x00000000U) /*!<  The number of RTCCLK pulses subbstited
                                                                       during a 32-second window =   CALM[8:0] */

#define IS_RTC_SMOOTH_CALIB_PLUS(PLUS) (((PLUS) == RTC_SMOOTHCALIB_PLUSPULSES_SET) || \
                                        ((PLUS) == RTC_SMOOTHCALIB_PLUSPULSES_RESET))
/**
  * @}
  */

/** @defgroup RTCEx_Smooth_Calib_Minus_Pulses_Definitions Smooth Calib Minus Pulses Definitions
  * @{
  */
#define  IS_RTC_SMOOTH_CALIB_MINUS(VALUE) ((VALUE) <= 0x000001FFU)
/**
  * @}
  */

/** @defgroup RTCEx_Add_1_Second_Parameter_Definitions Add 1 Second Parameter Definitions
  * @{
  */
#define RTC_SHIFTADD1S_RESET      (0x00000000U)
#define RTC_SHIFTADD1S_SET        (0x80000000U)

#define IS_RTC_SHIFT_ADD1S(SEL) (((SEL) == RTC_SHIFTADD1S_RESET) || \
                                 ((SEL) == RTC_SHIFTADD1S_SET))
/**
  * @}
  */

/** @defgroup RTCEx_Substract_Fraction_Of_Second_Value Substract Fraction Of Second Value
  * @{
  */
#define IS_RTC_SHIFT_SUBFS(FS) ((FS) <= 0x00007FFFU)
/**
  * @}
  */

/** @defgroup RTCEx_Calib_Output_Selection_Definitions Calib Output Selection Definitions
  * @{
  */
#define RTC_CALIBOUTPUT_512HZ            (0x00000000U)
#define RTC_CALIBOUTPUT_1HZ              (0x00080000U)

#define IS_RTC_CALIB_OUTPUT(OUTPUT)  (((OUTPUT) == RTC_CALIBOUTPUT_512HZ) || \
                                      ((OUTPUT) == RTC_CALIBOUTPUT_1HZ))
/**
  * @}
  */

#if defined(STM32L100xBA) || defined (STM32L151xBA) || defined (STM32L152xBA) || defined(STM32L100xC) || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC) || defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
/** @defgroup RTC_Alarm_Sub_Seconds_Value Alarm Sub Seconds Value
  * @{
  */
#define IS_RTC_ALARM_SUB_SECOND_VALUE(VALUE) ((VALUE) <= 0x00007FFFU)
/**
  * @}
  */

/** @defgroup RTC_Alarm_Sub_Seconds_Masks_Definitions Alarm Sub Seconds Masks Definitions
  * @{
  */
#define RTC_ALARMSUBSECONDMASK_ALL         (0x00000000U)  /*!< All Alarm SS fields are masked. 
                                                                        There is no comparison on sub seconds 
                                                                        for Alarm */
#define RTC_ALARMSUBSECONDMASK_SS14_1      (0x01000000U)  /*!< SS[14:1] are don't care in Alarm 
                                                                        comparison. Only SS[0] is compared.    */
#define RTC_ALARMSUBSECONDMASK_SS14_2      (0x02000000U)  /*!< SS[14:2] are don't care in Alarm 
                                                                        comparison. Only SS[1:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_3      (0x03000000U)  /*!< SS[14:3] are don't care in Alarm 
                                                                        comparison. Only SS[2:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_4      (0x04000000U)  /*!< SS[14:4] are don't care in Alarm 
                                                                        comparison. Only SS[3:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_5      (0x05000000U)  /*!< SS[14:5] are don't care in Alarm 
                                                                        comparison. Only SS[4:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_6      (0x06000000U)  /*!< SS[14:6] are don't care in Alarm 
                                                                        comparison. Only SS[5:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_7      (0x07000000U)  /*!< SS[14:7] are don't care in Alarm 
                                                                        comparison. Only SS[6:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_8      (0x08000000U)  /*!< SS[14:8] are don't care in Alarm 
                                                                        comparison. Only SS[7:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_9      (0x09000000U)  /*!< SS[14:9] are don't care in Alarm 
                                                                        comparison. Only SS[8:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_10     (0x0A000000U)  /*!< SS[14:10] are don't care in Alarm 
                                                                        comparison. Only SS[9:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_11     (0x0B000000U)  /*!< SS[14:11] are don't care in Alarm 
                                                                        comparison. Only SS[10:0] are compared */
#define RTC_ALARMSUBSECONDMASK_SS14_12     (0x0C000000U)  /*!< SS[14:12] are don't care in Alarm 
                                                                        comparison.Only SS[11:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_13     (0x0D000000U)  /*!< SS[14:13] are don't care in Alarm 
                                                                        comparison. Only SS[12:0] are compared */
#define RTC_ALARMSUBSECONDMASK_SS14        (0x0E000000U)  /*!< SS[14] is don't care in Alarm 
                                                                        comparison.Only SS[13:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_NONE        (0x0F000000U)  /*!< SS[14:0] are compared and must match 
                                                                        to activate alarm. */

#define IS_RTC_ALARM_SUB_SECOND_MASK(MASK)   (((MASK) == RTC_ALARMSUBSECONDMASK_ALL) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_1) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_2) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_3) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_4) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_5) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_6) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_7) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_8) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_9) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_10) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_11) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_12) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14_13) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_SS14) || \
                                              ((MASK) == RTC_ALARMSUBSECONDMASK_NONE))
/**
  * @}
  */
#endif /* STM32L100xBA || STM32L151xBA || STM32L152xBA || STM32L100xC || STM32L151xC || STM32L152xC || STM32L162xC || STM32L151xCA || STM32L151xD || STM32L152xCA || STM32L152xD || STM32L162xCA || STM32L162xD || STM32L151xE || STM32L151xDX || STM32L152xE || STM32L152xDX || STM32L162xE || STM32L162xDX */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup RTCEx_Exported_Macros RTCEx Exported Macros
  * @{
  */

/* ---------------------------------WAKEUPTIMER---------------------------------*/
/** @defgroup RTCEx_WakeUp_Timer RTC WakeUp Timer
  * @{
  */
/**
  * @brief Enable the RTC WakeUp Timer peripheral.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_WAKEUPTIMER_ENABLE(__HANDLE__)                      ((__HANDLE__)->Instance->CR |= (RTC_CR_WUTE))

/**
  * @brief Enable the RTC TimeStamp peripheral.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_TIMESTAMP_ENABLE(__HANDLE__)                        ((__HANDLE__)->Instance->CR |= (RTC_CR_TSE))

/**
  * @brief Disable the RTC WakeUp Timer peripheral.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_WAKEUPTIMER_DISABLE(__HANDLE__)                     ((__HANDLE__)->Instance->CR &= ~(RTC_CR_WUTE))

/**
  * @brief Disable the RTC TimeStamp peripheral.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_TIMESTAMP_DISABLE(__HANDLE__)                       ((__HANDLE__)->Instance->CR &= ~(RTC_CR_TSE))

/**
  * @brief  Enable the Coarse calibration process.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_COARSE_CALIB_ENABLE(__HANDLE__)                       ((__HANDLE__)->Instance->CR |= (RTC_CR_DCE))

/**
  * @brief  Disable the Coarse calibration process.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_COARSE_CALIB_DISABLE(__HANDLE__)                      ((__HANDLE__)->Instance->CR &= ~(RTC_CR_DCE))

/**
  * @brief  Enable the RTC calibration output.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_CALIBRATION_OUTPUT_ENABLE(__HANDLE__)                 ((__HANDLE__)->Instance->CR |= (RTC_CR_COE))

/**
  * @brief  Disable the calibration output.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_CALIBRATION_OUTPUT_DISABLE(__HANDLE__)                ((__HANDLE__)->Instance->CR &= ~(RTC_CR_COE))

/**
  * @brief  Enable the clock reference detection.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_CLOCKREF_DETECTION_ENABLE(__HANDLE__)                 ((__HANDLE__)->Instance->CR |= (RTC_CR_REFCKON))

/**
  * @brief  Disable the clock reference detection.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_CLOCKREF_DETECTION_DISABLE(__HANDLE__)                ((__HANDLE__)->Instance->CR &= ~(RTC_CR_REFCKON))

/**
  * @brief  Enable the RTC TimeStamp interrupt.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __INTERRUPT__ specifies the RTC TimeStamp interrupt sources to be enabled or disabled.
  *         This parameter can be:
  *             @arg RTC_IT_TS: TimeStamp interrupt
  * @retval None
  */
#define __HAL_RTC_TIMESTAMP_ENABLE_IT(__HANDLE__, __INTERRUPT__)      ((__HANDLE__)->Instance->CR |= (__INTERRUPT__))

/**
  * @brief  Enable the RTC WakeUpTimer interrupt.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __INTERRUPT__ specifies the RTC WakeUpTimer interrupt sources to be enabled or disabled.
  *         This parameter can be:
  *            @arg RTC_IT_WUT:  WakeUpTimer A interrupt
  * @retval None
  */
#define __HAL_RTC_WAKEUPTIMER_ENABLE_IT(__HANDLE__, __INTERRUPT__)    ((__HANDLE__)->Instance->CR |= (__INTERRUPT__))

/**
  * @brief  Disable the RTC TimeStamp interrupt.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __INTERRUPT__ specifies the RTC TimeStamp interrupt sources to be enabled or disabled.
  *          This parameter can be:
  *             @arg RTC_IT_TS: TimeStamp interrupt
  * @retval None
  */
#define __HAL_RTC_TIMESTAMP_DISABLE_IT(__HANDLE__, __INTERRUPT__)     ((__HANDLE__)->Instance->CR &= ~(__INTERRUPT__))

/**
  * @brief  Disable the RTC WakeUpTimer interrupt.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __INTERRUPT__ specifies the RTC WakeUpTimer interrupt sources to be enabled or disabled.
  *         This parameter can be:
  *            @arg RTC_IT_WUT:  WakeUpTimer A interrupt
  * @retval None
  */
#define __HAL_RTC_WAKEUPTIMER_DISABLE_IT(__HANDLE__, __INTERRUPT__)   ((__HANDLE__)->Instance->CR &= ~(__INTERRUPT__))

/**
  * @brief  Enable the RTC Tamper1 input detection.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_TAMPER1_ENABLE(__HANDLE__)                         SET_BIT((__HANDLE__)->Instance->TAFCR, RTC_TAFCR_TAMP1E)

/**
  * @brief  Disable the RTC Tamper1 input detection.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_TAMPER1_DISABLE(__HANDLE__)                        CLEAR_BIT((__HANDLE__)->Instance->TAFCR, RTC_TAFCR_TAMP1E)

#if defined(STM32L100xBA) || defined (STM32L151xBA) || defined (STM32L152xBA) || defined(STM32L100xC)\
  || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC) || defined(STM32L151xCA)\
  || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA)\
  || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
/**
  * @brief  Enable the RTC Tamper2 input detection.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_TAMPER2_ENABLE(__HANDLE__)                         SET_BIT((__HANDLE__)->Instance->TAFCR, RTC_TAFCR_TAMP2E)

/**
  * @brief  Disable the RTC Tamper2 input detection.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_TAMPER2_DISABLE(__HANDLE__)                        CLEAR_BIT((__HANDLE__)->Instance->TAFCR, RTC_TAFCR_TAMP2E)

/**
  * @brief  Enable the RTC Tamper3 input detection.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_TAMPER3_ENABLE(__HANDLE__)                         SET_BIT((__HANDLE__)->Instance->TAFCR, RTC_TAFCR_TAMP3E)

/**
  * @brief  Disable the RTC Tamper3 input detection.
  * @param  __HANDLE__ specifies the RTC handle.
  * @retval None
  */
#define __HAL_RTC_TAMPER3_DISABLE(__HANDLE__)                        CLEAR_BIT((__HANDLE__)->Instance->TAFCR, RTC_TAFCR_TAMP3E)


/**
  * @brief  Check whether the specified RTC Tamper interrupt has occurred or not.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __INTERRUPT__ specifies the RTC Tamper interrupt to check.
  *         This parameter can be:
  *            @arg  RTC_IT_TAMP1: Tamper1 interrupt
  *            @arg  RTC_IT_TAMP2: Tamper2 interrupt
  *            @arg  RTC_IT_TAMP3: Tamper3 interrupt
  * @retval None
  */
#define __HAL_RTC_TAMPER_GET_IT(__HANDLE__, __INTERRUPT__) (((__INTERRUPT__) == RTC_IT_TAMP1) ? (((((__HANDLE__)->Instance->ISR) & RTC_ISR_TAMP1F) != RESET) ? SET : RESET) : \
                                                           ((__INTERRUPT__) == RTC_IT_TAMP2) ? (((((__HANDLE__)->Instance->ISR) & RTC_ISR_TAMP2F) != RESET) ? SET : RESET) : \
                                                           (((((__HANDLE__)->Instance->ISR) & RTC_ISR_TAMP3F) != RESET) ? SET : RESET))
#else
/**
  * @brief  Check whether the specified RTC Tamper interrupt has occurred or not.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __INTERRUPT__ specifies the RTC Tamper interrupt to check.
  *         This parameter can be:
  *            @arg  RTC_IT_TAMP1: Tamper1 interrupt
  * @retval None
  */
#define __HAL_RTC_TAMPER_GET_IT(__HANDLE__, __INTERRUPT__) (((((__HANDLE__)->Instance->ISR) & RTC_ISR_TAMP1F) != RESET) ? SET : RESET)

#endif

/**
  * @brief  Enable the RTC Tamper interrupt.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __INTERRUPT__ specifies the RTC Tamper interrupt sources to be enabled
  *          This parameter can be any combination of the following values:
  *            @arg  RTC_IT_TAMP1: Tamper1 interrupt
  *            @arg  RTC_IT_TAMP2: Tamper2 interrupt (*)
  *            @arg  RTC_IT_TAMP3: Tamper3 interrupt (*)
  * @note   (*) Available only on devices STM32L100xBA, STM32L151xBA, STM32L152xBA, STM32L100xC,
  *         STM32L151xC, STM32L152xC, STM32L162xC, STM32L151xCA, STM32L151xD, STM32L152xCA,
  *         STM32L152xD, STM32L162xCA, STM32L162xD, STM32L151xE, STM32L152xE, STM32L162xE
  *         STM32L151xDX, STM32L152xDX, STM32L162xDX
  * @retval None
  */
#define __HAL_RTC_TAMPER_ENABLE_IT(__HANDLE__, __INTERRUPT__)         SET_BIT(((__HANDLE__)->Instance->TAFCR), RTC_TAFCR_TAMPIE)

/**
  * @brief  Disable the RTC Tamper interrupt.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __INTERRUPT__ specifies the RTC Tamper interrupt sources to be disabled.
  *         This parameter can be any combination of the following values:
  *            @arg  RTC_IT_TAMP1: Tamper1 interrupt
  *            @arg  RTC_IT_TAMP2: Tamper2 interrupt (*)
  *            @arg  RTC_IT_TAMP3: Tamper3 interrupt (*)
  * @note   (*) Available only on devices STM32L100xBA, STM32L151xBA, STM32L152xBA, STM32L100xC,
  *         STM32L151xC, STM32L152xC, STM32L162xC, STM32L151xCA, STM32L151xD, STM32L152xCA,
  *         STM32L152xD, STM32L162xCA, STM32L162xD, STM32L151xE, STM32L152xE, STM32L162xE
  *         STM32L151xDX, STM32L152xDX, STM32L162xDX
  * @retval None
  */
#define __HAL_RTC_TAMPER_DISABLE_IT(__HANDLE__, __INTERRUPT__)        CLEAR_BIT(((__HANDLE__)->Instance->TAFCR), RTC_TAFCR_TAMPIE)

/**
  * @brief  Check whether the specified RTC Tamper interrupt has been enabled or not.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __INTERRUPT__ specifies the RTC Tamper interrupt source to check.
  *         This parameter can be:
  *            @arg  RTC_IT_TAMP1: Tamper1 interrupt
  *            @arg  RTC_IT_TAMP2: Tamper2 interrupt (*)
  *            @arg  RTC_IT_TAMP3: Tamper3 interrupt (*)
  * @note   (*) Available only on devices STM32L100xBA, STM32L151xBA, STM32L152xBA, STM32L100xC,
  *         STM32L151xC, STM32L152xC, STM32L162xC, STM32L151xCA, STM32L151xD, STM32L152xCA,
  *         STM32L152xD, STM32L162xCA, STM32L162xD, STM32L151xE, STM32L152xE, STM32L162xE
  *         STM32L151xDX, STM32L152xDX, STM32L162xDX
  * @retval None
  */
#define __HAL_RTC_TAMPER_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__)    (((((__HANDLE__)->Instance->TAFCR) & RTC_TAFCR_TAMPIE) != RESET) ? SET : RESET)

/**
  * @brief  Check whether the specified RTC WakeUpTimer interrupt has occurred or not.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __FLAG__ specifies the RTC WakeUpTimer interrupt sources to be enabled or disabled.
  *         This parameter can be:
  *            @arg RTC_IT_WUT:  WakeUpTimer A interrupt
  * @retval None
  */
#define __HAL_RTC_WAKEUPTIMER_GET_IT(__HANDLE__, __FLAG__)            (((((__HANDLE__)->Instance->ISR) & ((__FLAG__)>> 4)) != RESET)? SET : RESET)

/**
  * @brief  Check whether the specified RTC Wake Up timer interrupt has been enabled or not.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __INTERRUPT__ specifies the RTC Wake Up timer interrupt sources to check.
  *         This parameter can be:
  *            @arg RTC_IT_WUT:  WakeUpTimer interrupt
  * @retval None
  */
#define __HAL_RTC_WAKEUPTIMER_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__)   (((((__HANDLE__)->Instance->CR) & (__INTERRUPT__)) != RESET) ? SET : RESET)

/**
  * @brief  Check whether the specified RTC TimeStamp interrupt has occurred or not.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __INTERRUPT__ specifies the RTC TimeStamp interrupt sources to be enabled or disabled.
  *         This parameter can be:
  *            @arg RTC_IT_TS: TimeStamp interrupt
  * @retval None
  */
#define __HAL_RTC_TIMESTAMP_GET_IT(__HANDLE__, __INTERRUPT__)              (((((__HANDLE__)->Instance->ISR) & ((__INTERRUPT__)>> 4)) != RESET)? SET : RESET)

/**
  * @brief  Check whether the specified RTC Time Stamp interrupt has been enabled or not.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __INTERRUPT__ specifies the RTC Time Stamp interrupt source to check.
  *         This parameter can be:
  *            @arg RTC_IT_TS: TimeStamp interrupt
  * @retval None
  */
#define __HAL_RTC_TIMESTAMP_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__)     (((((__HANDLE__)->Instance->CR) & (__INTERRUPT__)) != RESET) ? SET : RESET)

/**
  * @brief  Get the selected RTC TimeStamp's flag status.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __FLAG__ specifies the RTC TimeStamp Flag sources to be enabled or disabled.
  *         This parameter can be:
  *            @arg RTC_FLAG_TSF
  *            @arg RTC_FLAG_TSOVF
  * @retval None
  */
#define __HAL_RTC_TIMESTAMP_GET_FLAG(__HANDLE__, __FLAG__)            (((((__HANDLE__)->Instance->ISR) & (__FLAG__)) != RESET)? SET : RESET)

/**
  * @brief  Get the selected RTC WakeUpTimer's flag status.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __FLAG__ specifies the RTC WakeUpTimer Flag sources to be enabled or disabled.
  *         This parameter can be:
  *            @arg RTC_FLAG_WUTF
  *            @arg RTC_FLAG_WUTWF
  * @retval None
  */
#define __HAL_RTC_WAKEUPTIMER_GET_FLAG(__HANDLE__, __FLAG__)          (((((__HANDLE__)->Instance->ISR) & (__FLAG__)) != RESET)? SET : RESET)

/**
  * @brief  Get the selected RTC Tamper's flag status.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __FLAG__ specifies the RTC Tamper Flag sources to be enabled or disabled.
  *         This parameter can be:
  *            @arg RTC_FLAG_TAMP1F
  *            @arg RTC_FLAG_TAMP2F  (*)
  *            @arg RTC_FLAG_TAMP3F  (*)
  * @note   (*) Available only on devices STM32L100xBA, STM32L151xBA, STM32L152xBA, STM32L100xC,
  *         STM32L151xC, STM32L152xC, STM32L162xC, STM32L151xCA, STM32L151xD, STM32L152xCA,
  *         STM32L152xD, STM32L162xCA, STM32L162xD, STM32L151xE, STM32L152xE, STM32L162xE
  *         STM32L151xDX, STM32L152xDX, STM32L162xDX
  * @retval None
  */
#define __HAL_RTC_TAMPER_GET_FLAG(__HANDLE__, __FLAG__)               (((((__HANDLE__)->Instance->ISR) & (__FLAG__)) != RESET)? SET : RESET)

#if defined(STM32L100xBA) || defined (STM32L151xBA) || defined (STM32L152xBA) || defined(STM32L100xC)\
  || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC) || defined(STM32L151xCA)\
  || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA)\
  || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
/**
  * @brief  Get the selected RTC shift operation's flag status.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __FLAG__ specifies the RTC shift operation Flag is pending or not.
  *         This parameter can be:
  *            @arg RTC_FLAG_SHPF
  * @retval None
  */
#define __HAL_RTC_SHIFT_GET_FLAG(__HANDLE__, __FLAG__)                (((((__HANDLE__)->Instance->ISR) & (__FLAG__)) != RESET)? SET : RESET)
#endif /* STM32L100xBA || STM32L151xBA || STM32L152xBA || STM32L100xC || STM32L151xC || STM32L152xC || STM32L162xC || STM32L151xCA || STM32L151xD || STM32L152xCA || STM32L152xD || STM32L162xCA || STM32L162xD || STM32L151xE || STM32L151xDX || STM32L152xE || STM32L152xDX || STM32L162xE || STM32L162xDX */

/**
  * @brief  Clear the RTC Time Stamp's pending flags.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __FLAG__ specifies the RTC Alarm Flag sources to be enabled or disabled.
  *         This parameter can be:
  *            @arg RTC_FLAG_TSF
  * @retval None
  */
#define __HAL_RTC_TIMESTAMP_CLEAR_FLAG(__HANDLE__, __FLAG__)          ((__HANDLE__)->Instance->ISR) = (~((__FLAG__) | RTC_ISR_INIT)|((__HANDLE__)->Instance->ISR & RTC_ISR_INIT))

/**
  * @brief  Clear the RTC Tamper's pending flags.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __FLAG__ specifies the RTC Tamper Flag sources to be enabled or disabled.
  *         This parameter can be:
  *            @arg RTC_FLAG_TAMP1F
  *            @arg RTC_FLAG_TAMP2F  (*)
  *            @arg RTC_FLAG_TAMP3F  (*)
  * @note   (*) Available only on devices STM32L100xBA, STM32L151xBA, STM32L152xBA, STM32L100xC,
  *         STM32L151xC, STM32L152xC, STM32L162xC, STM32L151xCA, STM32L151xD, STM32L152xCA,
  *         STM32L152xD, STM32L162xCA, STM32L162xD, STM32L151xE, STM32L152xE, STM32L162xE
  *         STM32L151xDX, STM32L152xDX, STM32L162xDX
  * @retval None
  */
#define __HAL_RTC_TAMPER_CLEAR_FLAG(__HANDLE__, __FLAG__)      ((__HANDLE__)->Instance->ISR) = (~((__FLAG__) | RTC_ISR_INIT)|((__HANDLE__)->Instance->ISR & RTC_ISR_INIT))

/**
  * @brief  Clear the RTC Wake Up timer's pending flags.
  * @param  __HANDLE__ specifies the RTC handle.
  * @param  __FLAG__ specifies the RTC WakeUpTimer Flag to clear.
  *         This parameter can be:
  *            @arg RTC_FLAG_WUTF
  * @retval None
  */
#define __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(__HANDLE__, __FLAG__) ((__HANDLE__)->Instance->ISR) = (~((__FLAG__) | RTC_ISR_INIT)|((__HANDLE__)->Instance->ISR & RTC_ISR_INIT))

/**
  * @brief  Enable interrupt on the RTC WakeUp Timer associated Exti line.
  * @retval None
  */
#define __HAL_RTC_WAKEUPTIMER_EXTI_ENABLE_IT()       (EXTI->IMR |= RTC_EXTI_LINE_WAKEUPTIMER_EVENT)

/**
  * @brief  Disable interrupt on the RTC WakeUp Timer associated Exti line.
  * @retval None
  */
#define __HAL_RTC_WAKEUPTIMER_EXTI_DISABLE_IT()      (EXTI->IMR &= ~(RTC_EXTI_LINE_WAKEUPTIMER_EVENT))

/**
  * @brief  Enable event on the RTC WakeUp Timer associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_WAKEUPTIMER_EXTI_ENABLE_EVENT()    (EXTI->EMR |= RTC_EXTI_LINE_WAKEUPTIMER_EVENT)

/**
  * @brief  Disable event on the RTC WakeUp Timer associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_WAKEUPTIMER_EXTI_DISABLE_EVENT()   (EXTI->EMR &= ~(RTC_EXTI_LINE_WAKEUPTIMER_EVENT))

/**
  * @brief  Enable falling edge trigger on the RTC WakeUp Timer associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_WAKEUPTIMER_EXTI_ENABLE_FALLING_EDGE()   (EXTI->FTSR |= RTC_EXTI_LINE_WAKEUPTIMER_EVENT)

/**
  * @brief  Disable falling edge trigger on the RTC WakeUp Timer associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_WAKEUPTIMER_EXTI_DISABLE_FALLING_EDGE()  (EXTI->FTSR &= ~(RTC_EXTI_LINE_WAKEUPTIMER_EVENT))

/**
  * @brief  Enable rising edge trigger on the RTC WakeUp Timer associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_WAKEUPTIMER_EXTI_ENABLE_RISING_EDGE()    (EXTI->RTSR |= RTC_EXTI_LINE_WAKEUPTIMER_EVENT)

/**
  * @brief  Disable rising edge trigger on the RTC WakeUp Timer associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_WAKEUPTIMER_EXTI_DISABLE_RISING_EDGE()   (EXTI->RTSR &= ~(RTC_EXTI_LINE_WAKEUPTIMER_EVENT))

/**
  * @brief  Enable rising & falling edge trigger on the RTC WakeUp Timer associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_WAKEUPTIMER_EXTI_ENABLE_RISING_FALLING_EDGE() do { __HAL_RTC_WAKEUPTIMER_EXTI_ENABLE_RISING_EDGE();__HAL_RTC_WAKEUPTIMER_EXTI_ENABLE_FALLING_EDGE(); } while(0);

/**
  * @brief  Disable rising & falling edge trigger on the RTC WakeUp Timer associated Exti line.
  * This parameter can be:
  * @retval None.
  */
#define __HAL_RTC_WAKEUPTIMER_EXTI_DISABLE_RISING_FALLING_EDGE() do { __HAL_RTC_WAKEUPTIMER_EXTI_DISABLE_RISING_EDGE();__HAL_RTC_WAKEUPTIMER_EXTI_DISABLE_FALLING_EDGE(); } while(0);

/**
  * @brief Check whether the RTC WakeUp Timer associated Exti line interrupt flag is set or not.
  * @retval Line Status.
  */
#define __HAL_RTC_WAKEUPTIMER_EXTI_GET_FLAG()              (EXTI->PR & RTC_EXTI_LINE_WAKEUPTIMER_EVENT)

/**
  * @brief Clear the RTC WakeUp Timer associated Exti line flag.
  * @retval None.
  */
#define __HAL_RTC_WAKEUPTIMER_EXTI_CLEAR_FLAG()            (EXTI->PR = RTC_EXTI_LINE_WAKEUPTIMER_EVENT)

/**
  * @brief Generate a Software interrupt on the RTC WakeUp Timer associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_WAKEUPTIMER_EXTI_GENERATE_SWIT()         (EXTI->SWIER |= RTC_EXTI_LINE_WAKEUPTIMER_EVENT)

/**
  * @brief  Enable interrupt on the RTC Tamper and Timestamp associated Exti line.
  * @retval None
  */
#define __HAL_RTC_TAMPER_TIMESTAMP_EXTI_ENABLE_IT()        (EXTI->IMR |= RTC_EXTI_LINE_TAMPER_TIMESTAMP_EVENT)

/**
  * @brief  Disable interrupt on the RTC Tamper and Timestamp associated Exti line.
  * @retval None
  */
#define __HAL_RTC_TAMPER_TIMESTAMP_EXTI_DISABLE_IT()       (EXTI->IMR &= ~(RTC_EXTI_LINE_TAMPER_TIMESTAMP_EVENT))

/**
  * @brief  Enable event on the RTC Tamper and Timestamp associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_TAMPER_TIMESTAMP_EXTI_ENABLE_EVENT()    (EXTI->EMR |= RTC_EXTI_LINE_TAMPER_TIMESTAMP_EVENT)

/**
  * @brief  Disable event on the RTC Tamper and Timestamp associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_TAMPER_TIMESTAMP_EXTI_DISABLE_EVENT()   (EXTI->EMR &= ~(RTC_EXTI_LINE_TAMPER_TIMESTAMP_EVENT))

/**
  * @brief  Enable falling edge trigger on the RTC Tamper and Timestamp associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_TAMPER_TIMESTAMP_EXTI_ENABLE_FALLING_EDGE()   (EXTI->FTSR |= RTC_EXTI_LINE_TAMPER_TIMESTAMP_EVENT)

/**
  * @brief  Disable falling edge trigger on the RTC Tamper and Timestamp associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_TAMPER_TIMESTAMP_EXTI_DISABLE_FALLING_EDGE()  (EXTI->FTSR &= ~(RTC_EXTI_LINE_TAMPER_TIMESTAMP_EVENT))

/**
  * @brief  Enable rising edge trigger on the RTC Tamper and Timestamp associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_TAMPER_TIMESTAMP_EXTI_ENABLE_RISING_EDGE()    (EXTI->RTSR |= RTC_EXTI_LINE_TAMPER_TIMESTAMP_EVENT)

/**
  * @brief  Disable rising edge trigger on the RTC Tamper and Timestamp associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_TAMPER_TIMESTAMP_EXTI_DISABLE_RISING_EDGE()   (EXTI->RTSR &= ~(RTC_EXTI_LINE_TAMPER_TIMESTAMP_EVENT))

/**
  * @brief  Enable rising & falling edge trigger on the RTC Tamper and Timestamp associated Exti line.
  * @retval None.
  */
#define __HAL_RTC_TAMPER_TIMESTAMP_EXTI_ENABLE_RISING_FALLING_EDGE() do { __HAL_RTC_TAMPER_TIMESTAMP_EXTI_ENABLE_RISING_EDGE();__HAL_RTC_TAMPER_TIMESTAMP_EXTI_ENABLE_FALLING_EDGE(); } while(0);

/**
  * @brief  Disable rising & falling edge trigger on the RTC Tamper and Timestamp associated Exti line.
  * This parameter can be:
  * @retval None.
  */
#define __HAL_RTC_TAMPER_TIMESTAMP_EXTI_DISABLE_RISING_FALLING_EDGE() do { __HAL_RTC_TAMPER_TIMESTAMP_EXTI_DISABLE_RISING_EDGE();__HAL_RTC_TAMPER_TIMESTAMP_EXTI_DISABLE_FALLING_EDGE(); } while(0);

/**
  * @brief Check whether the RTC Tamper and Timestamp associated Exti line interrupt flag is set or not.
  * @retval Line Status.
  */
#define __HAL_RTC_TAMPER_TIMESTAMP_EXTI_GET_FLAG()         (EXTI->PR & RTC_EXTI_LINE_TAMPER_TIMESTAMP_EVENT)

/**
  * @brief Clear the RTC Tamper and Timestamp associated Exti line flag.
  * @retval None.
  */
#define __HAL_RTC_TAMPER_TIMESTAMP_EXTI_CLEAR_FLAG()       (EXTI->PR = RTC_EXTI_LINE_TAMPER_TIMESTAMP_EVENT)

/**
  * @brief Generate a Software interrupt on the RTC Tamper and Timestamp associated Exti line
  * @retval None.
  */
#define __HAL_RTC_TAMPER_TIMESTAMP_EXTI_GENERATE_SWIT()    (EXTI->SWIER |= RTC_EXTI_LINE_TAMPER_TIMESTAMP_EVENT)

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup RTCEx_Exported_Functions RTCEx Exported Functions
  * @{
  */

/* RTC TimeStamp and Tamper functions *****************************************/
/** @defgroup RTCEx_Exported_Functions_Group1 Extended RTC TimeStamp and Tamper functions
  * @{
  */
HAL_StatusTypeDef HAL_RTCEx_SetTimeStamp(RTC_HandleTypeDef *hrtc, uint32_t TimeStampEdge);
HAL_StatusTypeDef HAL_RTCEx_SetTimeStamp_IT(RTC_HandleTypeDef *hrtc, uint32_t TimeStampEdge);
HAL_StatusTypeDef HAL_RTCEx_DeactivateTimeStamp(RTC_HandleTypeDef *hrtc);
HAL_StatusTypeDef HAL_RTCEx_GetTimeStamp(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTimeStamp, RTC_DateTypeDef *sTimeStampDate, uint32_t Format);
HAL_StatusTypeDef HAL_RTCEx_SetTamper(RTC_HandleTypeDef *hrtc, RTC_TamperTypeDef *sTamper);
HAL_StatusTypeDef HAL_RTCEx_SetTamper_IT(RTC_HandleTypeDef *hrtc, RTC_TamperTypeDef *sTamper);
HAL_StatusTypeDef HAL_RTCEx_DeactivateTamper(RTC_HandleTypeDef *hrtc, uint32_t Tamper);
void              HAL_RTCEx_TamperTimeStampIRQHandler(RTC_HandleTypeDef *hrtc);

void              HAL_RTCEx_Tamper1EventCallback(RTC_HandleTypeDef *hrtc);
#if defined(STM32L100xBA) || defined (STM32L151xBA) || defined (STM32L152xBA) || defined(STM32L100xC) || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC) || defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
void              HAL_RTCEx_Tamper2EventCallback(RTC_HandleTypeDef *hrtc);
void              HAL_RTCEx_Tamper3EventCallback(RTC_HandleTypeDef *hrtc);
#endif /* STM32L100xBA || STM32L151xBA || STM32L152xBA || STM32L100xC || STM32L151xC || STM32L152xC || STM32L162xC || STM32L151xCA || STM32L151xD || STM32L152xCA || STM32L152xD || STM32L162xCA || STM32L162xD || STM32L151xE || STM32L151xDX || STM32L152xE || STM32L152xDX || STM32L162xE || STM32L162xDX */
void              HAL_RTCEx_TimeStampEventCallback(RTC_HandleTypeDef *hrtc);
HAL_StatusTypeDef HAL_RTCEx_PollForTimeStampEvent(RTC_HandleTypeDef *hrtc, uint32_t Timeout);
HAL_StatusTypeDef HAL_RTCEx_PollForTamper1Event(RTC_HandleTypeDef *hrtc, uint32_t Timeout);
#if defined(STM32L100xBA) || defined (STM32L151xBA) || defined (STM32L152xBA) || defined(STM32L100xC) || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC) || defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
HAL_StatusTypeDef HAL_RTCEx_PollForTamper2Event(RTC_HandleTypeDef *hrtc, uint32_t Timeout);
HAL_StatusTypeDef HAL_RTCEx_PollForTamper3Event(RTC_HandleTypeDef *hrtc, uint32_t Timeout);
#endif /* STM32L100xBA || STM32L151xBA || STM32L152xBA || STM32L100xC || STM32L151xC || STM32L152xC || STM32L162xC || STM32L151xCA || STM32L151xD || STM32L152xCA || STM32L152xD || STM32L162xCA || STM32L162xD || STM32L151xE || STM32L151xDX || STM32L152xE || STM32L152xDX || STM32L162xE || STM32L162xDX */
/**
  * @}
  */

/* RTC Wake-up functions ******************************************************/
/** @defgroup RTCEx_Exported_Functions_Group2 Extended RTC Wake-up functions
  * @{
  */
HAL_StatusTypeDef HAL_RTCEx_SetWakeUpTimer(RTC_HandleTypeDef *hrtc, uint32_t WakeUpCounter, uint32_t WakeUpClock);
HAL_StatusTypeDef HAL_RTCEx_SetWakeUpTimer_IT(RTC_HandleTypeDef *hrtc, uint32_t WakeUpCounter, uint32_t WakeUpClock);
HAL_StatusTypeDef HAL_RTCEx_DeactivateWakeUpTimer(RTC_HandleTypeDef *hrtc);
uint32_t          HAL_RTCEx_GetWakeUpTimer(RTC_HandleTypeDef *hrtc);
void              HAL_RTCEx_WakeUpTimerIRQHandler(RTC_HandleTypeDef *hrtc);
void              HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc);
HAL_StatusTypeDef HAL_RTCEx_PollForWakeUpTimerEvent(RTC_HandleTypeDef *hrtc, uint32_t Timeout);
/**
  * @}
  */

/* Extended Control functions ************************************************/
/** @defgroup RTCEx_Exported_Functions_Group3 Extended Peripheral Control functions
  * @{
  */
void              HAL_RTCEx_BKUPWrite(RTC_HandleTypeDef *hrtc, uint32_t BackupRegister, uint32_t Data);
uint32_t          HAL_RTCEx_BKUPRead(RTC_HandleTypeDef *hrtc, uint32_t BackupRegister);

HAL_StatusTypeDef HAL_RTCEx_SetCoarseCalib(RTC_HandleTypeDef *hrtc, uint32_t CalibSign, uint32_t Value);
HAL_StatusTypeDef HAL_RTCEx_DeactivateCoarseCalib(RTC_HandleTypeDef *hrtc);
#if defined(STM32L100xBA) || defined (STM32L151xBA) || defined (STM32L152xBA) || defined(STM32L100xC) || defined (STM32L151xC) || defined (STM32L152xC) || defined (STM32L162xC) || defined(STM32L151xCA) || defined (STM32L151xD) || defined (STM32L152xCA) || defined (STM32L152xD) || defined (STM32L162xCA) || defined (STM32L162xD) || defined(STM32L151xE) || defined(STM32L151xDX) || defined (STM32L152xE) || defined (STM32L152xDX) || defined (STM32L162xE) || defined (STM32L162xDX)
HAL_StatusTypeDef HAL_RTCEx_SetSmoothCalib(RTC_HandleTypeDef *hrtc, uint32_t SmoothCalibPeriod, uint32_t SmoothCalibPlusPulses, uint32_t SmoothCalibMinusPulsesValue);
HAL_StatusTypeDef HAL_RTCEx_SetSynchroShift(RTC_HandleTypeDef *hrtc, uint32_t ShiftAdd1S, uint32_t ShiftSubFS);
HAL_StatusTypeDef HAL_RTCEx_SetCalibrationOutPut(RTC_HandleTypeDef *hrtc, uint32_t CalibOutput);
#else
HAL_StatusTypeDef HAL_RTCEx_SetCalibrationOutPut(RTC_HandleTypeDef *hrtc);
#endif /* STM32L100xBA || STM32L151xBA || STM32L152xBA || STM32L100xC || STM32L151xC || STM32L152xC || STM32L162xC || STM32L151xCA || STM32L151xD || STM32L152xCA || STM32L152xD || STM32L162xCA || STM32L162xD || STM32L151xE || STM32L151xDX || STM32L152xE || STM32L152xDX || STM32L162xE || STM32L162xDX */
HAL_StatusTypeDef HAL_RTCEx_DeactivateCalibrationOutPut(RTC_HandleTypeDef *hrtc);
HAL_StatusTypeDef HAL_RTCEx_SetRefClock(RTC_HandleTypeDef *hrtc);
HAL_StatusTypeDef HAL_RTCEx_DeactivateRefClock(RTC_HandleTypeDef *hrtc);
HAL_StatusTypeDef HAL_RTCEx_EnableBypassShadow(RTC_HandleTypeDef *hrtc);
HAL_StatusTypeDef HAL_RTCEx_DisableBypassShadow(RTC_HandleTypeDef *hrtc);
/**
  * @}
  */

/* Extended RTC features functions *******************************************/
/** @defgroup RTCEx_Exported_Functions_Group4 Extended features functions
  * @{
  */
void              HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc);
HAL_StatusTypeDef HAL_RTCEx_PollForAlarmBEvent(RTC_HandleTypeDef *hrtc, uint32_t Timeout);
/**
  * @}
  */

/**
  * @}
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup RTCEx_Private_Constants RTCEx Private Constants
  * @{
  */
#define RTC_EXTI_LINE_TAMPER_TIMESTAMP_EVENT  (0x00080000U)  /*!< External interrupt line 19 Connected to the RTC Tamper and Time Stamp events */
#define RTC_EXTI_LINE_WAKEUPTIMER_EVENT       (0x00100000U)  /*!< External interrupt line 20 Connected to the RTC Wakeup event */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup RTCEx_Private_Macros RTCEx Private Macros
  * @{
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

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32L1xx_HAL_RTC_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
