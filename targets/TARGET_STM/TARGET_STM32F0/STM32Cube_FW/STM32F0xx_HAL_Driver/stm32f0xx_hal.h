/**
  ******************************************************************************
  * @file    stm32f0xx_hal.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the HAL 
  *          module driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F0xx_HAL_H
#define __STM32F0xx_HAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal_conf.h"

/** @addtogroup STM32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup HAL
  * @{
  */ 

/* Private macros ------------------------------------------------------------*/
/** @addtogroup HAL_Private_Macros
  * @{
  */
#if defined(STM32F091xC) || defined(STM32F098xx) || defined(STM32F042x6) || defined(STM32F048xx) || \
    defined(STM32F030x6) || defined(STM32F031x6) || defined(STM32F038xx) || defined(STM32F070x6) || \
    defined(STM32F070xB) || defined(STM32F030x6)
#define IS_SYSCFG_FASTMODEPLUS(__PIN__) ((((__PIN__) & SYSCFG_FASTMODEPLUS_PA9)  == SYSCFG_FASTMODEPLUS_PA9)  || \
                                         (((__PIN__) & SYSCFG_FASTMODEPLUS_PA10) == SYSCFG_FASTMODEPLUS_PA10) || \
                                         (((__PIN__) & SYSCFG_FASTMODEPLUS_PB6)  == SYSCFG_FASTMODEPLUS_PB6)  || \
                                         (((__PIN__) & SYSCFG_FASTMODEPLUS_PB7)  == SYSCFG_FASTMODEPLUS_PB7)  || \
                                         (((__PIN__) & SYSCFG_FASTMODEPLUS_PB8)  == SYSCFG_FASTMODEPLUS_PB8)  || \
                                         (((__PIN__) & SYSCFG_FASTMODEPLUS_PB9)  == SYSCFG_FASTMODEPLUS_PB9))
#else
#define IS_SYSCFG_FASTMODEPLUS(__PIN__) ((((__PIN__) & SYSCFG_FASTMODEPLUS_PB6)  == SYSCFG_FASTMODEPLUS_PB6)  || \
                                         (((__PIN__) & SYSCFG_FASTMODEPLUS_PB7)  == SYSCFG_FASTMODEPLUS_PB7)  || \
                                         (((__PIN__) & SYSCFG_FASTMODEPLUS_PB8)  == SYSCFG_FASTMODEPLUS_PB8)  || \
                                         (((__PIN__) & SYSCFG_FASTMODEPLUS_PB9)  == SYSCFG_FASTMODEPLUS_PB9))
#endif
#if defined(SYSCFG_CFGR1_PA11_PA12_RMP)
#define IS_HAL_REMAP_PIN(RMP) ((RMP) == HAL_REMAP_PA11_PA12)
#endif /* SYSCFG_CFGR1_PA11_PA12_RMP */
#if defined(STM32F091xC) || defined(STM32F098xx)
#define IS_HAL_SYSCFG_IRDA_ENV_SEL(SEL)   (((SEL) == HAL_SYSCFG_IRDA_ENV_SEL_TIM16)   || \
                                           ((SEL) == HAL_SYSCFG_IRDA_ENV_SEL_USART1)   || \
                                           ((SEL) == HAL_SYSCFG_IRDA_ENV_SEL_USART4))
#endif /* STM32F091xC || STM32F098xx */
/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup HAL_Exported_Constants HAL Exported Constants
  * @{
  */

/** @defgroup HAL_TICK_FREQ Tick Frequency
  * @{
  */
typedef enum
{
  HAL_TICK_FREQ_10HZ         = 100U,
  HAL_TICK_FREQ_100HZ        = 10U,
  HAL_TICK_FREQ_1KHZ         = 1U,
  HAL_TICK_FREQ_DEFAULT      = HAL_TICK_FREQ_1KHZ
} HAL_TickFreqTypeDef;

/**
  * @}
  */

#if defined(SYSCFG_CFGR1_PA11_PA12_RMP)
/** @defgroup HAL_Pin_remapping HAL Pin remapping
  * @{
  */
#define HAL_REMAP_PA11_PA12                 (SYSCFG_CFGR1_PA11_PA12_RMP)  /*!< PA11 and PA12 remapping bit for small packages (28 and 20 pins).
                                                                           0: No remap (pin pair PA9/10 mapped on the pins)
                                                                           1: Remap (pin pair PA11/12 mapped instead of PA9/10) */

/**
  * @}
  */
#endif /* SYSCFG_CFGR1_PA11_PA12_RMP */

#if defined(STM32F091xC) || defined(STM32F098xx)
/** @defgroup HAL_IRDA_ENV_SEL HAL IRDA Enveloppe Selection
  * @note Applicable on STM32F09x
  * @{
  */
#define HAL_SYSCFG_IRDA_ENV_SEL_TIM16     (SYSCFG_CFGR1_IRDA_ENV_SEL_0 & SYSCFG_CFGR1_IRDA_ENV_SEL_1)    /* 00: Timer16 is selected as IRDA Modulation enveloppe source */
#define HAL_SYSCFG_IRDA_ENV_SEL_USART1    (SYSCFG_CFGR1_IRDA_ENV_SEL_0)  /* 01: USART1 is selected as IRDA Modulation enveloppe source */
#define HAL_SYSCFG_IRDA_ENV_SEL_USART4    (SYSCFG_CFGR1_IRDA_ENV_SEL_1)  /* 10: USART4 is selected as IRDA Modulation enveloppe source */

/**
  * @}
  */
#endif /* STM32F091xC || STM32F098xx */


/** @defgroup SYSCFG_FastModePlus_GPIO Fast-mode Plus on GPIO
  * @{
  */

/** @brief  Fast-mode Plus driving capability on a specific GPIO
  */  
#if defined(STM32F091xC) || defined(STM32F098xx) || defined(STM32F042x6) || defined(STM32F048xx) || \
    defined(STM32F030x6) || defined(STM32F031x6) || defined(STM32F038xx) || defined(STM32F070x6) || \
    defined(STM32F070xB) || defined(STM32F030x6)
#define SYSCFG_FASTMODEPLUS_PA9        SYSCFG_CFGR1_I2C_FMP_PA9  /*!< Enable Fast-mode Plus on PA9  */
#define SYSCFG_FASTMODEPLUS_PA10       SYSCFG_CFGR1_I2C_FMP_PA10 /*!< Enable Fast-mode Plus on PA10 */
#endif
#define SYSCFG_FASTMODEPLUS_PB6        SYSCFG_CFGR1_I2C_FMP_PB6  /*!< Enable Fast-mode Plus on PB6  */
#define SYSCFG_FASTMODEPLUS_PB7        SYSCFG_CFGR1_I2C_FMP_PB7  /*!< Enable Fast-mode Plus on PB7  */
#define SYSCFG_FASTMODEPLUS_PB8        SYSCFG_CFGR1_I2C_FMP_PB8  /*!< Enable Fast-mode Plus on PB8  */
#define SYSCFG_FASTMODEPLUS_PB9        SYSCFG_CFGR1_I2C_FMP_PB9  /*!< Enable Fast-mode Plus on PB9  */

/**
 * @}
 */


#if defined(STM32F091xC) || defined (STM32F098xx)
/** @defgroup HAL_ISR_Wrapper HAL ISR Wrapper
  * @brief ISR Wrapper
  * @note applicable on STM32F09x
  * @{
  */
#define HAL_SYSCFG_ITLINE0                           ( 0x00000000U) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE1                           ( 0x00000001U) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE2                           ( 0x00000002U) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE3                           ( 0x00000003U) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE4                           ( 0x00000004U) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE5                           ( 0x00000005U) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE6                           ( 0x00000006U) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE7                           ( 0x00000007U) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE8                           ( 0x00000008U) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE9                           ( 0x00000009U) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE10                          ( 0x0000000AU) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE11                          ( 0x0000000BU) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE12                          ( 0x0000000CU) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE13                          ( 0x0000000DU) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE14                          ( 0x0000000EU) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE15                          ( 0x0000000FU) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE16                          ( 0x00000010U) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE17                          ( 0x00000011U) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE18                          ( 0x00000012U) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE19                          ( 0x00000013U) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE20                          ( 0x00000014U) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE21                          ( 0x00000015U) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE22                          ( 0x00000016U) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE23                          ( 0x00000017U) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE24                          ( 0x00000018U) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE25                          ( 0x00000019U) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE26                          ( 0x0000001AU) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE27                          ( 0x0000001BU) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE28                          ( 0x0000001CU) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE29                          ( 0x0000001DU) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE30                          ( 0x0000001EU) /*!< Internal define for macro handling */
#define HAL_SYSCFG_ITLINE31                          ( 0x0000001FU) /*!< Internal define for macro handling */

#define HAL_ITLINE_EWDG           ((uint32_t) ((HAL_SYSCFG_ITLINE0 << 0x18U) | SYSCFG_ITLINE0_SR_EWDG)) /*!< EWDG has expired .... */
#if defined(STM32F091xC)
#define HAL_ITLINE_PVDOUT         ((uint32_t) ((HAL_SYSCFG_ITLINE1 << 0x18U) | SYSCFG_ITLINE1_SR_PVDOUT)) /*!< Power voltage detection Interrupt .... */
#endif
#define HAL_ITLINE_VDDIO2         ((uint32_t) ((HAL_SYSCFG_ITLINE1 << 0x18U) | SYSCFG_ITLINE1_SR_VDDIO2)) /*!< VDDIO2 Interrupt .... */
#define HAL_ITLINE_RTC_WAKEUP     ((uint32_t) ((HAL_SYSCFG_ITLINE2 << 0x18U) | SYSCFG_ITLINE2_SR_RTC_WAKEUP)) /*!< RTC WAKEUP -> exti[20] Interrupt */
#define HAL_ITLINE_RTC_TSTAMP     ((uint32_t) ((HAL_SYSCFG_ITLINE2 << 0x18U) | SYSCFG_ITLINE2_SR_RTC_TSTAMP)) /*!< RTC Time Stamp -> exti[19] interrupt */
#define HAL_ITLINE_RTC_ALRA       ((uint32_t) ((HAL_SYSCFG_ITLINE2 << 0x18U) | SYSCFG_ITLINE2_SR_RTC_ALRA)) /*!< RTC Alarm -> exti[17] interrupt .... */
#define HAL_ITLINE_FLASH_ITF      ((uint32_t) ((HAL_SYSCFG_ITLINE3 << 0x18U) | SYSCFG_ITLINE3_SR_FLASH_ITF)) /*!< Flash ITF Interrupt */
#define HAL_ITLINE_CRS            ((uint32_t) ((HAL_SYSCFG_ITLINE4 << 0x18U) | SYSCFG_ITLINE4_SR_CRS)) /*!< CRS Interrupt */
#define HAL_ITLINE_CLK_CTRL       ((uint32_t) ((HAL_SYSCFG_ITLINE4 << 0x18U) | SYSCFG_ITLINE4_SR_CLK_CTRL)) /*!< CLK Control Interrupt */
#define HAL_ITLINE_EXTI0          ((uint32_t) ((HAL_SYSCFG_ITLINE5 << 0x18U) | SYSCFG_ITLINE5_SR_EXTI0)) /*!< External Interrupt 0 */
#define HAL_ITLINE_EXTI1          ((uint32_t) ((HAL_SYSCFG_ITLINE5 << 0x18U) | SYSCFG_ITLINE5_SR_EXTI1)) /*!< External Interrupt 1 */
#define HAL_ITLINE_EXTI2          ((uint32_t) ((HAL_SYSCFG_ITLINE6 << 0x18U) | SYSCFG_ITLINE6_SR_EXTI2)) /*!< External Interrupt 2 */
#define HAL_ITLINE_EXTI3          ((uint32_t) ((HAL_SYSCFG_ITLINE6 << 0x18U) | SYSCFG_ITLINE6_SR_EXTI3)) /*!< External Interrupt 3 */
#define HAL_ITLINE_EXTI4          ((uint32_t) ((HAL_SYSCFG_ITLINE7 << 0x18U) | SYSCFG_ITLINE7_SR_EXTI4)) /*!< EXTI4 Interrupt */
#define HAL_ITLINE_EXTI5          ((uint32_t) ((HAL_SYSCFG_ITLINE7 << 0x18U) | SYSCFG_ITLINE7_SR_EXTI5)) /*!< EXTI5 Interrupt */
#define HAL_ITLINE_EXTI6          ((uint32_t) ((HAL_SYSCFG_ITLINE7 << 0x18U) | SYSCFG_ITLINE7_SR_EXTI6)) /*!< EXTI6 Interrupt */
#define HAL_ITLINE_EXTI7          ((uint32_t) ((HAL_SYSCFG_ITLINE7 << 0x18U) | SYSCFG_ITLINE7_SR_EXTI7)) /*!< EXTI7 Interrupt */
#define HAL_ITLINE_EXTI8          ((uint32_t) ((HAL_SYSCFG_ITLINE7 << 0x18U) | SYSCFG_ITLINE7_SR_EXTI8)) /*!< EXTI8 Interrupt */
#define HAL_ITLINE_EXTI9          ((uint32_t) ((HAL_SYSCFG_ITLINE7 << 0x18U) | SYSCFG_ITLINE7_SR_EXTI9)) /*!< EXTI9 Interrupt */
#define HAL_ITLINE_EXTI10         ((uint32_t) ((HAL_SYSCFG_ITLINE7 << 0x18U) | SYSCFG_ITLINE7_SR_EXTI10)) /*!< EXTI10 Interrupt */
#define HAL_ITLINE_EXTI11         ((uint32_t) ((HAL_SYSCFG_ITLINE7 << 0x18U) | SYSCFG_ITLINE7_SR_EXTI11)) /*!< EXTI11 Interrupt */
#define HAL_ITLINE_EXTI12         ((uint32_t) ((HAL_SYSCFG_ITLINE7 << 0x18U) | SYSCFG_ITLINE7_SR_EXTI12)) /*!< EXTI12 Interrupt */
#define HAL_ITLINE_EXTI13         ((uint32_t) ((HAL_SYSCFG_ITLINE7 << 0x18U) | SYSCFG_ITLINE7_SR_EXTI13)) /*!< EXTI13 Interrupt */
#define HAL_ITLINE_EXTI14         ((uint32_t) ((HAL_SYSCFG_ITLINE7 << 0x18U) | SYSCFG_ITLINE7_SR_EXTI14)) /*!< EXTI14 Interrupt */
#define HAL_ITLINE_EXTI15         ((uint32_t) ((HAL_SYSCFG_ITLINE7 << 0x18U) | SYSCFG_ITLINE7_SR_EXTI15)) /*!< EXTI15 Interrupt */
#define HAL_ITLINE_TSC_EOA        ((uint32_t) ((HAL_SYSCFG_ITLINE8 << 0x18U) | SYSCFG_ITLINE8_SR_TSC_EOA)) /*!< Touch control EOA Interrupt */
#define HAL_ITLINE_TSC_MCE        ((uint32_t) ((HAL_SYSCFG_ITLINE8 << 0x18U) | SYSCFG_ITLINE8_SR_TSC_MCE)) /*!< Touch control MCE Interrupt */
#define HAL_ITLINE_DMA1_CH1       ((uint32_t) ((HAL_SYSCFG_ITLINE9 << 0x18U) | SYSCFG_ITLINE9_SR_DMA1_CH1)) /*!< DMA1 Channel 1 Interrupt */
#define HAL_ITLINE_DMA1_CH2       ((uint32_t) ((HAL_SYSCFG_ITLINE10 << 0x18U) | SYSCFG_ITLINE10_SR_DMA1_CH2)) /*!< DMA1 Channel 2 Interrupt */
#define HAL_ITLINE_DMA1_CH3       ((uint32_t) ((HAL_SYSCFG_ITLINE10 << 0x18U) | SYSCFG_ITLINE10_SR_DMA1_CH3)) /*!< DMA1 Channel 3 Interrupt */
#define HAL_ITLINE_DMA2_CH1       ((uint32_t) ((HAL_SYSCFG_ITLINE10 << 0x18U) | SYSCFG_ITLINE10_SR_DMA2_CH1)) /*!< DMA2 Channel 1 Interrupt */
#define HAL_ITLINE_DMA2_CH2       ((uint32_t) ((HAL_SYSCFG_ITLINE10 << 0x18U) | SYSCFG_ITLINE10_SR_DMA2_CH2)) /*!< DMA2 Channel 2 Interrupt */
#define HAL_ITLINE_DMA1_CH4       ((uint32_t) ((HAL_SYSCFG_ITLINE11 << 0x18U) | SYSCFG_ITLINE11_SR_DMA1_CH4)) /*!< DMA1 Channel 4 Interrupt */
#define HAL_ITLINE_DMA1_CH5       ((uint32_t) ((HAL_SYSCFG_ITLINE11 << 0x18U) | SYSCFG_ITLINE11_SR_DMA1_CH5)) /*!< DMA1 Channel 5 Interrupt */
#define HAL_ITLINE_DMA1_CH6       ((uint32_t) ((HAL_SYSCFG_ITLINE11 << 0x18U) | SYSCFG_ITLINE11_SR_DMA1_CH6)) /*!< DMA1 Channel 6 Interrupt */
#define HAL_ITLINE_DMA1_CH7       ((uint32_t) ((HAL_SYSCFG_ITLINE11 << 0x18U) | SYSCFG_ITLINE11_SR_DMA1_CH7)) /*!< DMA1 Channel 7 Interrupt */
#define HAL_ITLINE_DMA2_CH3       ((uint32_t) ((HAL_SYSCFG_ITLINE11 << 0x18U) | SYSCFG_ITLINE11_SR_DMA2_CH3)) /*!< DMA2 Channel 3 Interrupt */
#define HAL_ITLINE_DMA2_CH4       ((uint32_t) ((HAL_SYSCFG_ITLINE11 << 0x18U) | SYSCFG_ITLINE11_SR_DMA2_CH4)) /*!< DMA2 Channel 4 Interrupt */
#define HAL_ITLINE_DMA2_CH5       ((uint32_t) ((HAL_SYSCFG_ITLINE11 << 0x18U) | SYSCFG_ITLINE11_SR_DMA2_CH5)) /*!< DMA2 Channel 5 Interrupt */
#define HAL_ITLINE_ADC            ((uint32_t) ((HAL_SYSCFG_ITLINE12 << 0x18U) | SYSCFG_ITLINE12_SR_ADC)) /*!< ADC Interrupt */
#define HAL_ITLINE_COMP1          ((uint32_t) ((HAL_SYSCFG_ITLINE12 << 0x18U) | SYSCFG_ITLINE12_SR_COMP1)) /*!< COMP1 Interrupt -> exti[21] */
#define HAL_ITLINE_COMP2          ((uint32_t) ((HAL_SYSCFG_ITLINE12 << 0x18U) | SYSCFG_ITLINE12_SR_COMP2)) /*!< COMP2 Interrupt -> exti[21] */
#define HAL_ITLINE_TIM1_BRK       ((uint32_t) ((HAL_SYSCFG_ITLINE13 << 0x18U) | SYSCFG_ITLINE13_SR_TIM1_BRK)) /*!< TIM1 BRK Interrupt */
#define HAL_ITLINE_TIM1_UPD       ((uint32_t) ((HAL_SYSCFG_ITLINE13 << 0x18U) | SYSCFG_ITLINE13_SR_TIM1_UPD)) /*!< TIM1 UPD Interrupt */
#define HAL_ITLINE_TIM1_TRG       ((uint32_t) ((HAL_SYSCFG_ITLINE13 << 0x18U) | SYSCFG_ITLINE13_SR_TIM1_TRG)) /*!< TIM1 TRG Interrupt */
#define HAL_ITLINE_TIM1_CCU       ((uint32_t) ((HAL_SYSCFG_ITLINE13 << 0x18U) | SYSCFG_ITLINE13_SR_TIM1_CCU)) /*!< TIM1 CCU Interrupt */
#define HAL_ITLINE_TIM1_CC        ((uint32_t) ((HAL_SYSCFG_ITLINE14 << 0x18U) | SYSCFG_ITLINE14_SR_TIM1_CC)) /*!< TIM1 CC Interrupt */
#define HAL_ITLINE_TIM2           ((uint32_t) ((HAL_SYSCFG_ITLINE15 << 0x18U) | SYSCFG_ITLINE15_SR_TIM2_GLB)) /*!< TIM2 Interrupt */
#define HAL_ITLINE_TIM3           ((uint32_t) ((HAL_SYSCFG_ITLINE16 << 0x18U) | SYSCFG_ITLINE16_SR_TIM3_GLB)) /*!< TIM3 Interrupt */
#define HAL_ITLINE_DAC            ((uint32_t) ((HAL_SYSCFG_ITLINE17 << 0x18U) | SYSCFG_ITLINE17_SR_DAC)) /*!< DAC Interrupt */
#define HAL_ITLINE_TIM6           ((uint32_t) ((HAL_SYSCFG_ITLINE17 << 0x18U) | SYSCFG_ITLINE17_SR_TIM6_GLB)) /*!< TIM6 Interrupt */
#define HAL_ITLINE_TIM7           ((uint32_t) ((HAL_SYSCFG_ITLINE18 << 0x18U) | SYSCFG_ITLINE18_SR_TIM7_GLB)) /*!< TIM7 Interrupt */
#define HAL_ITLINE_TIM14          ((uint32_t) ((HAL_SYSCFG_ITLINE19 << 0x18U) | SYSCFG_ITLINE19_SR_TIM14_GLB)) /*!< TIM14 Interrupt */
#define HAL_ITLINE_TIM15          ((uint32_t) ((HAL_SYSCFG_ITLINE20 << 0x18U) | SYSCFG_ITLINE20_SR_TIM15_GLB)) /*!< TIM15 Interrupt */
#define HAL_ITLINE_TIM16          ((uint32_t) ((HAL_SYSCFG_ITLINE21 << 0x18U) | SYSCFG_ITLINE21_SR_TIM16_GLB)) /*!< TIM16 Interrupt */
#define HAL_ITLINE_TIM17          ((uint32_t) ((HAL_SYSCFG_ITLINE22 << 0x18U) | SYSCFG_ITLINE22_SR_TIM17_GLB)) /*!< TIM17 Interrupt */
#define HAL_ITLINE_I2C1           ((uint32_t) ((HAL_SYSCFG_ITLINE23 << 0x18U) | SYSCFG_ITLINE23_SR_I2C1_GLB)) /*!< I2C1 Interrupt -> exti[23] */
#define HAL_ITLINE_I2C2           ((uint32_t) ((HAL_SYSCFG_ITLINE24 << 0x18U) | SYSCFG_ITLINE24_SR_I2C2_GLB)) /*!< I2C2 Interrupt */
#define HAL_ITLINE_SPI1           ((uint32_t) ((HAL_SYSCFG_ITLINE25 << 0x18U) | SYSCFG_ITLINE25_SR_SPI1)) /*!< I2C1 Interrupt -> exti[23] */
#define HAL_ITLINE_SPI2           ((uint32_t) ((HAL_SYSCFG_ITLINE26 << 0x18U) | SYSCFG_ITLINE26_SR_SPI2)) /*!< SPI1 Interrupt */
#define HAL_ITLINE_USART1         ((uint32_t) ((HAL_SYSCFG_ITLINE27 << 0x18U) | SYSCFG_ITLINE27_SR_USART1_GLB)) /*!< USART1 GLB Interrupt -> exti[25] */
#define HAL_ITLINE_USART2         ((uint32_t) ((HAL_SYSCFG_ITLINE28 << 0x18U) | SYSCFG_ITLINE28_SR_USART2_GLB)) /*!< USART2 GLB Interrupt -> exti[26] */
#define HAL_ITLINE_USART3         ((uint32_t) ((HAL_SYSCFG_ITLINE29 << 0x18U) | SYSCFG_ITLINE29_SR_USART3_GLB)) /*!< USART3 Interrupt .... */
#define HAL_ITLINE_USART4         ((uint32_t) ((HAL_SYSCFG_ITLINE29 << 0x18U) | SYSCFG_ITLINE29_SR_USART4_GLB)) /*!< USART4 Interrupt .... */
#define HAL_ITLINE_USART5         ((uint32_t) ((HAL_SYSCFG_ITLINE29 << 0x18U) | SYSCFG_ITLINE29_SR_USART5_GLB)) /*!< USART5 Interrupt .... */
#define HAL_ITLINE_USART6         ((uint32_t) ((HAL_SYSCFG_ITLINE29 << 0x18U) | SYSCFG_ITLINE29_SR_USART6_GLB)) /*!< USART6 Interrupt .... */
#define HAL_ITLINE_USART7         ((uint32_t) ((HAL_SYSCFG_ITLINE29 << 0x18U) | SYSCFG_ITLINE29_SR_USART7_GLB)) /*!< USART7 Interrupt .... */
#define HAL_ITLINE_USART8         ((uint32_t) ((HAL_SYSCFG_ITLINE29 << 0x18U) | SYSCFG_ITLINE29_SR_USART8_GLB)) /*!< USART8 Interrupt .... */
#define HAL_ITLINE_CAN            ((uint32_t) ((HAL_SYSCFG_ITLINE30 << 0x18U) | SYSCFG_ITLINE30_SR_CAN)) /*!< CAN Interrupt */
#define HAL_ITLINE_CEC            ((uint32_t) ((HAL_SYSCFG_ITLINE30 << 0x18U) | SYSCFG_ITLINE30_SR_CEC)) /*!< CEC Interrupt -> exti[27] */
/**
  * @}
  */
#endif /* STM32F091xC || STM32F098xx */

/**
  * @}
  */  

/* Exported macros -----------------------------------------------------------*/
/** @defgroup HAL_Exported_Macros HAL Exported Macros
  * @{  
  */

/** @defgroup HAL_Freeze_Unfreeze_Peripherals HAL Freeze Unfreeze Peripherals
  * @brief  Freeze/Unfreeze Peripherals in Debug mode 
  * @{  
  */
  
#if defined(DBGMCU_APB1_FZ_DBG_CAN_STOP)
#define __HAL_FREEZE_CAN_DBGMCU()            (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_CAN_STOP))
#define __HAL_UNFREEZE_CAN_DBGMCU()          (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_CAN_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_CAN_STOP */

#if defined(DBGMCU_APB1_FZ_DBG_RTC_STOP)
#define __HAL_DBGMCU_FREEZE_RTC()            (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_RTC_STOP))
#define __HAL_DBGMCU_UNFREEZE_RTC()          (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_RTC_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_RTC_STOP */

#if defined(DBGMCU_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT)
#define __HAL_DBGMCU_FREEZE_I2C1_TIMEOUT()   (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT))
#define __HAL_DBGMCU_UNFREEZE_I2C1_TIMEOUT() (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT))
#endif /* DBGMCU_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT */

#if defined(DBGMCU_APB1_FZ_DBG_IWDG_STOP)
#define __HAL_DBGMCU_FREEZE_IWDG()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_IWDG_STOP))
#define __HAL_DBGMCU_UNFREEZE_IWDG()         (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_IWDG_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_IWDG_STOP */

#if defined(DBGMCU_APB1_FZ_DBG_WWDG_STOP)
#define __HAL_DBGMCU_FREEZE_WWDG()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_WWDG_STOP))
#define __HAL_DBGMCU_UNFREEZE_WWDG()         (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_WWDG_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_WWDG_STOP */

#if defined(DBGMCU_APB1_FZ_DBG_TIM2_STOP)
#define __HAL_DBGMCU_FREEZE_TIM2()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM2_STOP))
#define __HAL_DBGMCU_UNFREEZE_TIM2()         (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM2_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_TIM2_STOP */

#if defined(DBGMCU_APB1_FZ_DBG_TIM3_STOP)
#define __HAL_DBGMCU_FREEZE_TIM3()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM3_STOP))
#define __HAL_DBGMCU_UNFREEZE_TIM3()         (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM3_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_TIM3_STOP */

#if defined(DBGMCU_APB1_FZ_DBG_TIM6_STOP)
#define __HAL_DBGMCU_FREEZE_TIM6()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM6_STOP))
#define __HAL_DBGMCU_UNFREEZE_TIM6()         (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM6_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_TIM6_STOP */

#if defined(DBGMCU_APB1_FZ_DBG_TIM7_STOP)
#define __HAL_DBGMCU_FREEZE_TIM7()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM7_STOP))
#define __HAL_DBGMCU_UNFREEZE_TIM7()         (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM7_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_TIM7_STOP */

#if defined(DBGMCU_APB1_FZ_DBG_TIM14_STOP)
#define __HAL_DBGMCU_FREEZE_TIM14()          (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM14_STOP))
#define __HAL_DBGMCU_UNFREEZE_TIM14()        (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM14_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_TIM14_STOP */

#if defined(DBGMCU_APB2_FZ_DBG_TIM1_STOP)
#define __HAL_DBGMCU_FREEZE_TIM1()           (DBGMCU->APB2FZ |= (DBGMCU_APB2_FZ_DBG_TIM1_STOP))
#define __HAL_DBGMCU_UNFREEZE_TIM1()         (DBGMCU->APB2FZ &= ~(DBGMCU_APB2_FZ_DBG_TIM1_STOP))
#endif /* DBGMCU_APB2_FZ_DBG_TIM1_STOP */

#if defined(DBGMCU_APB2_FZ_DBG_TIM15_STOP)
#define __HAL_DBGMCU_FREEZE_TIM15()          (DBGMCU->APB2FZ |= (DBGMCU_APB2_FZ_DBG_TIM15_STOP))
#define __HAL_DBGMCU_UNFREEZE_TIM15()        (DBGMCU->APB2FZ &= ~(DBGMCU_APB2_FZ_DBG_TIM15_STOP))
#endif /* DBGMCU_APB2_FZ_DBG_TIM15_STOP */

#if defined(DBGMCU_APB2_FZ_DBG_TIM16_STOP)
#define __HAL_DBGMCU_FREEZE_TIM16()          (DBGMCU->APB2FZ |= (DBGMCU_APB2_FZ_DBG_TIM16_STOP))
#define __HAL_DBGMCU_UNFREEZE_TIM16()        (DBGMCU->APB2FZ &= ~(DBGMCU_APB2_FZ_DBG_TIM16_STOP))
#endif /* DBGMCU_APB2_FZ_DBG_TIM16_STOP */

#if defined(DBGMCU_APB2_FZ_DBG_TIM17_STOP)
#define __HAL_DBGMCU_FREEZE_TIM17()          (DBGMCU->APB2FZ |= (DBGMCU_APB2_FZ_DBG_TIM17_STOP))
#define __HAL_DBGMCU_UNFREEZE_TIM17()        (DBGMCU->APB2FZ &= ~(DBGMCU_APB2_FZ_DBG_TIM17_STOP))
#endif /* DBGMCU_APB2_FZ_DBG_TIM17_STOP */

/**
  * @}
  */  
  
/** @defgroup Memory_Mapping_Selection Memory Mapping Selection
  * @{   
  */
#if defined(SYSCFG_CFGR1_MEM_MODE)
/** @brief  Main Flash memory mapped at 0x00000000
  */
#define __HAL_SYSCFG_REMAPMEMORY_FLASH()        (SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_MEM_MODE))
#endif /* SYSCFG_CFGR1_MEM_MODE */

#if defined(SYSCFG_CFGR1_MEM_MODE_0)
/** @brief  System Flash memory mapped at 0x00000000
  */
#define __HAL_SYSCFG_REMAPMEMORY_SYSTEMFLASH()  do {SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_MEM_MODE); \
                                             SYSCFG->CFGR1 |= SYSCFG_CFGR1_MEM_MODE_0;  \
                                            }while(0)
#endif /* SYSCFG_CFGR1_MEM_MODE_0 */

#if defined(SYSCFG_CFGR1_MEM_MODE_0) && defined(SYSCFG_CFGR1_MEM_MODE_1)
/** @brief  Embedded SRAM mapped at 0x00000000
  */
#define __HAL_SYSCFG_REMAPMEMORY_SRAM()         do {SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_MEM_MODE); \
                                             SYSCFG->CFGR1 |= (SYSCFG_CFGR1_MEM_MODE_0 | SYSCFG_CFGR1_MEM_MODE_1); \
                                            }while(0) 
#endif /* SYSCFG_CFGR1_MEM_MODE_0 && SYSCFG_CFGR1_MEM_MODE_1 */
/**
  * @}
  */ 


#if defined(SYSCFG_CFGR1_PA11_PA12_RMP)
/** @defgroup HAL_Pin_remap HAL Pin remap 
  * @brief  Pin remapping enable/disable macros
  * @param __PIN_REMAP__ This parameter can be a value of @ref HAL_Pin_remapping
  * @{   
  */
#define __HAL_REMAP_PIN_ENABLE(__PIN_REMAP__)          do {assert_param(IS_HAL_REMAP_PIN((__PIN_REMAP__)));                 \
                                                           SYSCFG->CFGR1 |= (__PIN_REMAP__);                                \
                                                         }while(0)
#define __HAL_REMAP_PIN_DISABLE(__PIN_REMAP__)         do {assert_param(IS_HAL_REMAP_PIN((__PIN_REMAP__)));                 \
                                                           SYSCFG->CFGR1 &= ~(__PIN_REMAP__);                               \
                                                         }while(0)
/**
  * @}
  */  
#endif /* SYSCFG_CFGR1_PA11_PA12_RMP */

/** @brief  Fast-mode Plus driving capability enable/disable macros
  * @param __FASTMODEPLUS__ This parameter can be a value of @ref SYSCFG_FastModePlus_GPIO values.
  *                          That you can find above these macros.
  */
#define __HAL_SYSCFG_FASTMODEPLUS_ENABLE(__FASTMODEPLUS__)  do {assert_param(IS_SYSCFG_FASTMODEPLUS((__FASTMODEPLUS__)));\
                                                                SET_BIT(SYSCFG->CFGR1, (__FASTMODEPLUS__));\
                                                               }while(0)

#define __HAL_SYSCFG_FASTMODEPLUS_DISABLE(__FASTMODEPLUS__) do {assert_param(IS_SYSCFG_FASTMODEPLUS((__FASTMODEPLUS__)));\
                                                                CLEAR_BIT(SYSCFG->CFGR1, (__FASTMODEPLUS__));\
                                                               }while(0)
#if defined(SYSCFG_CFGR2_LOCKUP_LOCK)
/** @defgroup Cortex_Lockup_Enable Cortex Lockup Enable
  * @{   
  */
/** @brief  SYSCFG Break Lockup lock
  *         Enables and locks the connection of Cortex-M0 LOCKUP (Hardfault) output to TIM1/15/16/17 Break input
  * @note   The selected configuration is locked and can be unlocked by system reset
  */
#define __HAL_SYSCFG_BREAK_LOCKUP_LOCK()   do {SYSCFG->CFGR2 &= ~(SYSCFG_CFGR2_LOCKUP_LOCK); \
                                               SYSCFG->CFGR2 |= SYSCFG_CFGR2_LOCKUP_LOCK;    \
                                              }while(0)
/**
  * @}
  */  
#endif /* SYSCFG_CFGR2_LOCKUP_LOCK */

#if defined(SYSCFG_CFGR2_PVD_LOCK)
/** @defgroup PVD_Lock_Enable PVD Lock
  * @{  
  */
/** @brief  SYSCFG Break PVD lock
  *         Enables and locks the PVD connection with Timer1/8/15/16/17 Break Input, , as well as the PVDE and PLS[2:0] in the PWR_CR register
  * @note   The selected configuration is locked and can be unlocked by system reset
  */
#define __HAL_SYSCFG_BREAK_PVD_LOCK()      do {SYSCFG->CFGR2 &= ~(SYSCFG_CFGR2_PVD_LOCK); \
                                               SYSCFG->CFGR2 |= SYSCFG_CFGR2_PVD_LOCK;    \
                                              }while(0)
/**
  * @}
  */
#endif /* SYSCFG_CFGR2_PVD_LOCK */

#if defined(SYSCFG_CFGR2_SRAM_PARITY_LOCK)
/** @defgroup SRAM_Parity_Lock SRAM Parity Lock
  * @{
  */
/** @brief  SYSCFG Break SRAM PARITY lock
  *         Enables and locks the SRAM_PARITY error signal with Break Input of TIMER1/8/15/16/17
  * @note   The selected configuration is locked and can be unlocked by system reset
  */
#define __HAL_SYSCFG_BREAK_SRAMPARITY_LOCK() do {SYSCFG->CFGR2 &= ~(SYSCFG_CFGR2_SRAM_PARITY_LOCK); \
                                                 SYSCFG->CFGR2 |= SYSCFG_CFGR2_SRAM_PARITY_LOCK;    \
                                                }while(0)
/**
  * @}
  */
#endif /* SYSCFG_CFGR2_SRAM_PARITY_LOCK */

#if defined(SYSCFG_CFGR2_SRAM_PEF)
/** @defgroup HAL_SYSCFG_Parity_check_on_RAM HAL SYSCFG Parity check on RAM
  * @brief  Parity check on RAM disable macro
  * @note   Disabling the parity check on RAM locks the configuration bit.
  *         To re-enable the parity check on RAM perform a system reset.
  * @{  
  */
#define __HAL_SYSCFG_RAM_PARITYCHECK_DISABLE()   (SYSCFG->CFGR2 |= SYSCFG_CFGR2_SRAM_PEF)
/**
  * @}
  */
#endif /* SYSCFG_CFGR2_SRAM_PEF */


#if defined(STM32F091xC) || defined (STM32F098xx)
/** @defgroup HAL_ISR_wrapper_check HAL ISR wrapper check
  * @brief  ISR wrapper check
  * @note This feature is applicable on STM32F09x  
  * @note Allow to determine interrupt source per line.
  * @{  
  */
#define __HAL_GET_PENDING_IT(__SOURCE__)       (SYSCFG->IT_LINE_SR[((__SOURCE__) >> 0x18U)] & ((__SOURCE__) & 0x00FFFFFF))
/**
  * @}
  */
#endif /* (STM32F091xC) || defined (STM32F098xx)*/

#if defined(STM32F091xC) || defined (STM32F098xx)
/** @defgroup HAL_SYSCFG_IRDA_modulation_envelope_selection HAL SYSCFG IRDA modulation envelope selection
  * @brief  selection of the modulation envelope signal macro, using bits [7:6] of SYS_CTRL(CFGR1) register
  * @note This feature is applicable on STM32F09x
  * @param __SOURCE__ This parameter can be a value of @ref HAL_IRDA_ENV_SEL
  * @{  
  */
#define __HAL_SYSCFG_IRDA_ENV_SELECTION(__SOURCE__)  do {assert_param(IS_HAL_SYSCFG_IRDA_ENV_SEL((__SOURCE__))); \
                                                         SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_IRDA_ENV_SEL); \
                                                         SYSCFG->CFGR1 |= (__SOURCE__);    \
                                                        }while(0)

#define __HAL_SYSCFG_GET_IRDA_ENV_SELECTION()  ((SYSCFG->CFGR1) & 0x000000C0)
/**
  * @}
  */
#endif /* (STM32F091xC) || defined (STM32F098xx)*/

/**
  * @}
  */

/** @defgroup HAL_Private_Macros HAL Private Macros
  * @{
  */
#define IS_TICKFREQ(FREQ) (((FREQ) == HAL_TICK_FREQ_10HZ)  || \
                           ((FREQ) == HAL_TICK_FREQ_100HZ) || \
                           ((FREQ) == HAL_TICK_FREQ_1KHZ))
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @addtogroup HAL_Exported_Functions
  * @{
  */

/** @addtogroup HAL_Exported_Functions_Group1
  * @{
  */    
/* Initialization and de-initialization functions  ******************************/
HAL_StatusTypeDef HAL_Init(void);
HAL_StatusTypeDef HAL_DeInit(void);
void              HAL_MspInit(void);
void              HAL_MspDeInit(void);
HAL_StatusTypeDef HAL_InitTick (uint32_t TickPriority);
/**
  * @}
  */

/* Exported variables ---------------------------------------------------------*/
/** @addtogroup HAL_Exported_Variables
  * @{
  */
extern __IO uint32_t uwTick;
extern uint32_t uwTickPrio;
extern HAL_TickFreqTypeDef uwTickFreq;
/**
  * @}
  */

/** @addtogroup HAL_Exported_Functions_Group2
  * @{
  */    

/* Peripheral Control functions  ************************************************/
void                HAL_IncTick(void);
void                HAL_Delay(uint32_t Delay);
uint32_t            HAL_GetTick(void);
uint32_t            HAL_GetTickPrio(void);
HAL_StatusTypeDef   HAL_SetTickFreq(HAL_TickFreqTypeDef Freq);
HAL_TickFreqTypeDef HAL_GetTickFreq(void);
void                HAL_SuspendTick(void);
void                HAL_ResumeTick(void);
uint32_t            HAL_GetHalVersion(void);
uint32_t            HAL_GetREVID(void);
uint32_t            HAL_GetDEVID(void);
uint32_t            HAL_GetUIDw0(void);
uint32_t            HAL_GetUIDw1(void);
uint32_t            HAL_GetUIDw2(void);
void                HAL_DBGMCU_EnableDBGStopMode(void);
void                HAL_DBGMCU_DisableDBGStopMode(void);
void                HAL_DBGMCU_EnableDBGStandbyMode(void);
void                HAL_DBGMCU_DisableDBGStandbyMode(void);
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

#endif /* __STM32F0xx_HAL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
