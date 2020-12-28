/**
  ******************************************************************************
  * @file    stm32f3xx_hal.h
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
#ifndef __STM32F3xx_HAL_H
#define __STM32F3xx_HAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal_conf.h"

/** @addtogroup STM32F3xx_HAL_Driver
  * @{
  */

/** @addtogroup HAL
  * @{
  */ 

/* Private macros ------------------------------------------------------------*/
/** @addtogroup HAL_Private_Macros
  * @{
  */
#define IS_SYSCFG_FASTMODEPLUS(__PIN__) ((((__PIN__) & SYSCFG_FASTMODEPLUS_PB6)  == SYSCFG_FASTMODEPLUS_PB6)  || \
                                         (((__PIN__) & SYSCFG_FASTMODEPLUS_PB7)  == SYSCFG_FASTMODEPLUS_PB7)  || \
                                         (((__PIN__) & SYSCFG_FASTMODEPLUS_PB8)  == SYSCFG_FASTMODEPLUS_PB8)  || \
                                         (((__PIN__) & SYSCFG_FASTMODEPLUS_PB9)  == SYSCFG_FASTMODEPLUS_PB9))
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

/**
  * @}
  */
/** @defgroup HAL_Exported_Constants HAL Exported Constants
  * @{
  */
/** @defgroup SYSCFG_BitAddress_AliasRegion SYSCFG registers bit address in the alias region
  * @brief SYSCFG registers bit address in the alias region
  * @{
  */
/* ------------ SYSCFG registers bit address in the alias region -------------*/
#define SYSCFG_OFFSET                (SYSCFG_BASE - PERIPH_BASE)
/* --- CFGR2 Register ---*/
/* Alias word address of BYP_ADDR_PAR bit */
#define CFGR2_OFFSET                 (SYSCFG_OFFSET + 0x18U)
#define BYPADDRPAR_BitNumber          0x04U
#define CFGR2_BYPADDRPAR_BB          (PERIPH_BB_BASE + (CFGR2_OFFSET * 32U) + (BYPADDRPAR_BitNumber * 4U))
/**
  * @}
  */

#if defined(SYSCFG_CFGR1_DMA_RMP)
/** @defgroup HAL_DMA_Remapping HAL DMA Remapping
  *        Elements values convention: 0xXXYYYYYY
  *           - YYYYYY  : Position in the register
  *           - XX  : Register index
  *                 - 00: CFGR1 register in SYSCFG
  *                 - 01: CFGR3 register in SYSCFG (not available on STM32F373xC/STM32F378xx devices)
  * @{
  */
#define HAL_REMAPDMA_ADC24_DMA2_CH34         (0x00000100U) /*!< ADC24 DMA remap (STM32F303xB/C/E, STM32F358xx and STM32F398xx devices)
                                                                          1: Remap (ADC24 DMA requests mapped on DMA2 channels 3 and 4) */
#define HAL_REMAPDMA_TIM16_DMA1_CH6          (0x00000800U) /*!< TIM16 DMA request remap
                                                                         1: Remap (TIM16_CH1 and TIM16_UP DMA requests mapped on DMA1 channel 6) */
#define HAL_REMAPDMA_TIM17_DMA1_CH7          (0x00001000U) /*!< TIM17 DMA request remap
                                                                         1: Remap (TIM17_CH1 and TIM17_UP DMA requests mapped on DMA1 channel 7) */
#define HAL_REMAPDMA_TIM6_DAC1_CH1_DMA1_CH3  (0x00002000U) /*!< TIM6 and DAC channel1 DMA remap (STM32F303xB/C/E, STM32F358xx and STM32F398xx devices)
                                                                         1: Remap (TIM6_UP and DAC_CH1 DMA requests mapped on DMA1 channel 3) */
#define HAL_REMAPDMA_TIM7_DAC1_CH2_DMA1_CH4  (0x00004000U) /*!< TIM7 and DAC channel2 DMA remap (STM32F303xB/C/E, STM32F358xx and STM32F398xx devices)
                                                                         1: Remap (TIM7_UP and DAC_CH2 DMA requests mapped on DMA1 channel 4) */
#define HAL_REMAPDMA_DAC2_CH1_DMA1_CH5       (0x00008000U) /*!< DAC2 channel1 DMA remap (STM32F303x4/6/8 devices only)
                                                                         1: Remap (DAC2_CH1 DMA requests mapped on DMA1 channel 5) */
#define HAL_REMAPDMA_TIM18_DAC2_CH1_DMA1_CH5 (0x00008000U) /*!< DAC2 channel1 DMA remap (STM32F303x4/6/8 devices only)
                                                                         1: Remap (DAC2_CH1 DMA requests mapped on DMA1 channel 5) */
#if defined(SYSCFG_CFGR3_DMA_RMP)
#if !defined(HAL_REMAP_CFGR3_MASK) 
#define HAL_REMAP_CFGR3_MASK                 (0x01000000U)
#endif

#define HAL_REMAPDMA_SPI1_RX_DMA1_CH2        (0x01000003U) /*!< SPI1_RX DMA remap (STM32F303x4/6/8 devices only)
                                                                         11: Map on DMA1 channel 2 */
#define HAL_REMAPDMA_SPI1_RX_DMA1_CH4        (0x01000001U) /*!< SPI1_RX DMA remap (STM32F303x4/6/8 devices only)
                                                                         01: Map on DMA1 channel 4 */
#define HAL_REMAPDMA_SPI1_RX_DMA1_CH6        (0x01000002U) /*!< SPI1_RX DMA remap (STM32F303x4/6/8 devices only)
                                                                         10: Map on DMA1 channel 6 */
#define HAL_REMAPDMA_SPI1_TX_DMA1_CH3        (0x0100000CU) /*!< SPI1_TX DMA remap (STM32F303x4/6/8 devices only)
                                                                         11: Map on DMA1 channel 3 */
#define HAL_REMAPDMA_SPI1_TX_DMA1_CH5        (0x01000004U) /*!< SPI1_TX DMA remap (STM32F303x4/6/8 devices only)
                                                                         01: Map on DMA1 channel 5 */
#define HAL_REMAPDMA_SPI1_TX_DMA1_CH7        (0x01000008U) /*!< SPI1_TX DMA remap (STM32F303x4/6/8 devices only)
                                                                         10: Map on DMA1 channel 7 */
#define HAL_REMAPDMA_I2C1_RX_DMA1_CH7        (0x01000030U) /*!< I2C1_RX DMA remap (STM32F303x4/6/8 devices only)
                                                                         11: Map on DMA1 channel 7 */
#define HAL_REMAPDMA_I2C1_RX_DMA1_CH3        (0x01000010U) /*!< I2C1_RX DMA remap (STM32F303x4/6/8 devices only)
                                                                         01: Map on DMA1 channel 3 */
#define HAL_REMAPDMA_I2C1_RX_DMA1_CH5        (0x01000020U) /*!< I2C1_RX DMA remap (STM32F303x4/6/8 devices only)
                                                                         10: Map on DMA1 channel 5 */
#define HAL_REMAPDMA_I2C1_TX_DMA1_CH6        (0x010000C0U) /*!< I2C1_TX DMA remap (STM32F303x4/6/8 devices only)
                                                                         11: Map on DMA1 channel 6 */
#define HAL_REMAPDMA_I2C1_TX_DMA1_CH2        (0x01000040U) /*!< I2C1_TX DMA remap (STM32F303x4/6/8 devices only)
                                                                         01: Map on DMA1 channel 2 */
#define HAL_REMAPDMA_I2C1_TX_DMA1_CH4        (0x01000080U) /*!< I2C1_TX DMA remap (STM32F303x4/6/8 devices only)
                                                                         10: Map on DMA1 channel 4 */
#define HAL_REMAPDMA_ADC2_DMA1_CH2           (0x01000100U) /*!< ADC2 DMA remap
                                                                         x0: No remap (ADC2 on DMA2)
                                                                         10: Map on DMA1 channel 2 */
#define HAL_REMAPDMA_ADC2_DMA1_CH4           (0x01000300U) /*!< ADC2 DMA remap
                                                                         11: Map on DMA1 channel 4 */
#endif /* SYSCFG_CFGR3_DMA_RMP */

#if defined(SYSCFG_CFGR3_DMA_RMP)
#define IS_DMA_REMAP(RMP) ((((RMP) & HAL_REMAPDMA_ADC24_DMA2_CH34)         == HAL_REMAPDMA_ADC24_DMA2_CH34)         || \
                              (((RMP) & HAL_REMAPDMA_TIM16_DMA1_CH6)          == HAL_REMAPDMA_TIM16_DMA1_CH6)          || \
                              (((RMP) & HAL_REMAPDMA_TIM17_DMA1_CH7)          == HAL_REMAPDMA_TIM17_DMA1_CH7)          || \
                              (((RMP) & HAL_REMAPDMA_TIM6_DAC1_CH1_DMA1_CH3)  == HAL_REMAPDMA_TIM6_DAC1_CH1_DMA1_CH3)  || \
                              (((RMP) & HAL_REMAPDMA_TIM7_DAC1_CH2_DMA1_CH4)  == HAL_REMAPDMA_TIM7_DAC1_CH2_DMA1_CH4)  || \
                              (((RMP) & HAL_REMAPDMA_DAC2_CH1_DMA1_CH5)       == HAL_REMAPDMA_DAC2_CH1_DMA1_CH5)       || \
                              (((RMP) & HAL_REMAPDMA_TIM18_DAC2_CH1_DMA1_CH5) == HAL_REMAPDMA_TIM18_DAC2_CH1_DMA1_CH5) || \
                              (((RMP) & HAL_REMAPDMA_SPI1_RX_DMA1_CH2)        == HAL_REMAPDMA_SPI1_RX_DMA1_CH2)  || \
                              (((RMP) & HAL_REMAPDMA_SPI1_RX_DMA1_CH4)        == HAL_REMAPDMA_SPI1_RX_DMA1_CH4)  || \
                              (((RMP) & HAL_REMAPDMA_SPI1_RX_DMA1_CH6)        == HAL_REMAPDMA_SPI1_RX_DMA1_CH6)  || \
                              (((RMP) & HAL_REMAPDMA_SPI1_TX_DMA1_CH3)        == HAL_REMAPDMA_SPI1_TX_DMA1_CH3)  || \
                              (((RMP) & HAL_REMAPDMA_SPI1_TX_DMA1_CH5)        == HAL_REMAPDMA_SPI1_TX_DMA1_CH5)  || \
                              (((RMP) & HAL_REMAPDMA_SPI1_TX_DMA1_CH7)        == HAL_REMAPDMA_SPI1_TX_DMA1_CH7)  || \
                              (((RMP) & HAL_REMAPDMA_I2C1_RX_DMA1_CH7)        == HAL_REMAPDMA_I2C1_RX_DMA1_CH7)  || \
                              (((RMP) & HAL_REMAPDMA_I2C1_RX_DMA1_CH3)        == HAL_REMAPDMA_I2C1_RX_DMA1_CH3)  || \
                              (((RMP) & HAL_REMAPDMA_I2C1_RX_DMA1_CH5)        == HAL_REMAPDMA_I2C1_RX_DMA1_CH5)  || \
                              (((RMP) & HAL_REMAPDMA_I2C1_TX_DMA1_CH6)        == HAL_REMAPDMA_I2C1_TX_DMA1_CH6)  || \
                              (((RMP) & HAL_REMAPDMA_I2C1_TX_DMA1_CH2)        == HAL_REMAPDMA_I2C1_TX_DMA1_CH2)  || \
                              (((RMP) & HAL_REMAPDMA_I2C1_TX_DMA1_CH4)        == HAL_REMAPDMA_I2C1_TX_DMA1_CH4)  || \
                              (((RMP) & HAL_REMAPDMA_ADC2_DMA1_CH2)           == HAL_REMAPDMA_ADC2_DMA1_CH2)     || \
                              (((RMP) & HAL_REMAPDMA_ADC2_DMA1_CH4)           == HAL_REMAPDMA_ADC2_DMA1_CH4))
#else
#define IS_DMA_REMAP(RMP) ((((RMP) & HAL_REMAPDMA_ADC24_DMA2_CH34)         == HAL_REMAPDMA_ADC24_DMA2_CH34)         || \
                              (((RMP) & HAL_REMAPDMA_TIM16_DMA1_CH6)          == HAL_REMAPDMA_TIM16_DMA1_CH6)          || \
                              (((RMP) & HAL_REMAPDMA_TIM17_DMA1_CH7)          == HAL_REMAPDMA_TIM17_DMA1_CH7)          || \
                              (((RMP) & HAL_REMAPDMA_TIM6_DAC1_CH1_DMA1_CH3)  == HAL_REMAPDMA_TIM6_DAC1_CH1_DMA1_CH3)  || \
                              (((RMP) & HAL_REMAPDMA_TIM7_DAC1_CH2_DMA1_CH4)  == HAL_REMAPDMA_TIM7_DAC1_CH2_DMA1_CH4)  || \
                              (((RMP) & HAL_REMAPDMA_DAC2_CH1_DMA1_CH5)       == HAL_REMAPDMA_DAC2_CH1_DMA1_CH5)       || \
                              (((RMP) & HAL_REMAPDMA_TIM18_DAC2_CH1_DMA1_CH5) == HAL_REMAPDMA_TIM18_DAC2_CH1_DMA1_CH5))
#endif /* SYSCFG_CFGR3_DMA_RMP && SYSCFG_CFGR1_DMA_RMP*/
/**
  * @}
  */
#endif /* SYSCFG_CFGR1_DMA_RMP */

/** @defgroup HAL_Trigger_Remapping HAL Trigger Remapping
  *        Elements values convention: 0xXXYYYYYY
  *           - YYYYYY  : Position in the register
  *           - XX  : Register index
  *                 - 00: CFGR1 register in SYSCFG
  *                 - 01: CFGR3 register in SYSCFG
  * @{
  */
#define HAL_REMAPTRIGGER_DAC1_TRIG         (0x00000080U)  /*!< DAC trigger remap (when TSEL = 001 on STM32F303xB/C and STM32F358xx devices)
                                                                        0: No remap (DAC trigger is TIM8_TRGO)
                                                                        1: Remap (DAC trigger is TIM3_TRGO) */
#define HAL_REMAPTRIGGER_TIM1_ITR3         (0x00000040U)  /*!< TIM1 ITR3 trigger remap
                                                                        0: No remap
                                                                        1: Remap (TIM1_TRG3 = TIM17_OC) */
#if defined(SYSCFG_CFGR3_TRIGGER_RMP)
#if !defined(HAL_REMAP_CFGR3_MASK) 
#define HAL_REMAP_CFGR3_MASK               (0x01000000U)
#endif
#define HAL_REMAPTRIGGER_DAC1_TRIG3        (0x01010000U)  /*!< DAC1_CH1 / DAC1_CH2 Trigger remap
                                                                        0: Remap (DAC trigger is TIM15_TRGO)
                                                                        1: Remap (DAC trigger is HRTIM1_DAC1_TRIG1) */
#define HAL_REMAPTRIGGER_DAC1_TRIG5        (0x01020000U)  /*!< DAC1_CH1 / DAC1_CH2 Trigger remap
                                                                        0: No remap
                                                                        1: Remap (DAC trigger is HRTIM1_DAC1_TRIG2) */
#define IS_HAL_REMAPTRIGGER(RMP) ((((RMP) & HAL_REMAPTRIGGER_DAC1)       == HAL_REMAPTRIGGER_DAC1)       || \
                                  (((RMP) & HAL_REMAPTRIGGER_TIM1_ITR3)  == HAL_REMAPTRIGGER_TIM1_ITR3)  || \
                                  (((RMP) & HAL_REMAPTRIGGER_DAC1_TRIG3) == HAL_REMAPTRIGGER_DAC1_TRIG3) || \
                                  (((RMP) & HAL_REMAPTRIGGER_DAC1_TRIG5) == HAL_REMAPTRIGGER_DAC1_TRIG5))
#else
#define IS_HAL_REMAPTRIGGER(RMP) ((((RMP) & HAL_REMAPTRIGGER_DAC1)       == HAL_REMAPTRIGGER_DAC1)       || \
                                  (((RMP) & HAL_REMAPTRIGGER_TIM1_ITR3)  == HAL_REMAPTRIGGER_TIM1_ITR3))
#endif /* SYSCFG_CFGR3_TRIGGER_RMP */
/**
  * @}
  */

#if defined (STM32F302xE)
/** @defgroup HAL_ADC_Trigger_Remapping HAL ADC Trigger Remapping
  * @{
  */
#define HAL_REMAPADCTRIGGER_ADC12_EXT2        SYSCFG_CFGR4_ADC12_EXT2_RMP   /*!< Input trigger of ADC12 regular channel EXT2
                                                                                 0: No remap (TIM1_CC3)
                                                                                 1: Remap (TIM20_TRGO) */
#define HAL_REMAPADCTRIGGER_ADC12_EXT3        SYSCFG_CFGR4_ADC12_EXT3_RMP   /*!< Input trigger of ADC12 regular channel EXT3
                                                                                 0: No remap (TIM2_CC2)
                                                                                 1: Remap (TIM20_TRGO2) */
#define HAL_REMAPADCTRIGGER_ADC12_EXT5        SYSCFG_CFGR4_ADC12_EXT5_RMP   /*!< Input trigger of ADC12 regular channel EXT5
                                                                                 0: No remap (TIM4_CC4)
                                                                                 1: Remap (TIM20_CC1) */
#define HAL_REMAPADCTRIGGER_ADC12_EXT13       SYSCFG_CFGR4_ADC12_EXT13_RMP  /*!< Input trigger of ADC12 regular channel EXT13
                                                                                 0: No remap (TIM6_TRGO)
                                                                                 1: Remap (TIM20_CC2) */
#define HAL_REMAPADCTRIGGER_ADC12_EXT15       SYSCFG_CFGR4_ADC12_EXT15_RMP  /*!< Input trigger of ADC12 regular channel EXT15
                                                                                 0: No remap (TIM3_CC4)
                                                                                 1: Remap (TIM20_CC3) */
#define HAL_REMAPADCTRIGGER_ADC12_JEXT3       SYSCFG_CFGR4_ADC12_JEXT3_RMP  /*!< Input trigger of ADC12 injected channel JEXT3
                                                                                 0: No remap (TIM2_CC1)
                                                                                 1: Remap (TIM20_TRGO) */
#define HAL_REMAPADCTRIGGER_ADC12_JEXT6       SYSCFG_CFGR4_ADC12_JEXT6_RMP  /*!< Input trigger of ADC12 injected channel JEXT6
                                                                                 0: No remap (EXTI line 15)
                                                                                 1: Remap (TIM20_TRGO2) */
#define HAL_REMAPADCTRIGGER_ADC12_JEXT13      SYSCFG_CFGR4_ADC12_JEXT13_RMP  /*!< Input trigger of ADC12 injected channel JEXT13
                                                                                 0: No remap (TIM3_CC1)
                                                                                 1: Remap (TIM20_CC4) */

#define IS_HAL_REMAPADCTRIGGER(RMP)  ((((RMP) & HAL_REMAPADCTRIGGER_ADC12_EXT2)   == HAL_REMAPADCTRIGGER_ADC12_EXT2)   || \
                                      (((RMP) & HAL_REMAPADCTRIGGER_ADC12_EXT3)   == HAL_REMAPADCTRIGGER_ADC12_EXT3)   || \
                                      (((RMP) & HAL_REMAPADCTRIGGER_ADC12_EXT5)   == HAL_REMAPADCTRIGGER_ADC12_EXT5)   || \
                                      (((RMP) & HAL_REMAPADCTRIGGER_ADC12_EXT13)  == HAL_REMAPADCTRIGGER_ADC12_EXT13)  || \
                                      (((RMP) & HAL_REMAPADCTRIGGER_ADC12_EXT15)  == HAL_REMAPADCTRIGGER_ADC12_EXT15)  || \
                                      (((RMP) & HAL_REMAPADCTRIGGER_ADC12_JEXT3)  == HAL_REMAPADCTRIGGER_ADC12_JEXT3)  || \
                                      (((RMP) & HAL_REMAPADCTRIGGER_ADC12_JEXT6)  == HAL_REMAPADCTRIGGER_ADC12_JEXT6)  || \
                                      (((RMP) & HAL_REMAPADCTRIGGER_ADC12_JEXT13) == HAL_REMAPADCTRIGGER_ADC12_JEXT13))
/**
  * @}
  */
#endif /* STM32F302xE */

#if defined (STM32F303xE) || defined (STM32F398xx)
/** @defgroup HAL_ADC_Trigger_Remapping HAL ADC Trigger Remapping
  * @{
  */
#define HAL_REMAPADCTRIGGER_ADC12_EXT2        SYSCFG_CFGR4_ADC12_EXT2_RMP   /*!< Input trigger of ADC12 regular channel EXT2
                                                                                 0: No remap (TIM1_CC3)
                                                                                 1: Remap (TIM20_TRGO) */
#define HAL_REMAPADCTRIGGER_ADC12_EXT3        SYSCFG_CFGR4_ADC12_EXT3_RMP   /*!< Input trigger of ADC12 regular channel EXT3
                                                                                 0: No remap (TIM2_CC2)
                                                                                 1: Remap (TIM20_TRGO2) */
#define HAL_REMAPADCTRIGGER_ADC12_EXT5        SYSCFG_CFGR4_ADC12_EXT5_RMP   /*!< Input trigger of ADC12 regular channel EXT5
                                                                                 0: No remap (TIM4_CC4)
                                                                                 1: Remap (TIM20_CC1) */
#define HAL_REMAPADCTRIGGER_ADC12_EXT13       SYSCFG_CFGR4_ADC12_EXT13_RMP  /*!< Input trigger of ADC12 regular channel EXT13
                                                                                 0: No remap (TIM6_TRGO)
                                                                                 1: Remap (TIM20_CC2) */
#define HAL_REMAPADCTRIGGER_ADC12_EXT15       SYSCFG_CFGR4_ADC12_EXT15_RMP  /*!< Input trigger of ADC12 regular channel EXT15
                                                                                 0: No remap (TIM3_CC4)
                                                                                 1: Remap (TIM20_CC3) */
#define HAL_REMAPADCTRIGGER_ADC12_JEXT3       SYSCFG_CFGR4_ADC12_JEXT3_RMP  /*!< Input trigger of ADC12 injected channel JEXT3
                                                                                 0: No remap (TIM2_CC1)
                                                                                 1: Remap (TIM20_TRGO) */
#define HAL_REMAPADCTRIGGER_ADC12_JEXT6       SYSCFG_CFGR4_ADC12_JEXT6_RMP  /*!< Input trigger of ADC12 injected channel JEXT6
                                                                                 0: No remap (EXTI line 15)
                                                                                 1: Remap (TIM20_TRGO2) */
#define HAL_REMAPADCTRIGGER_ADC12_JEXT13      SYSCFG_CFGR4_ADC12_JEXT13_RMP  /*!< Input trigger of ADC12 injected channel JEXT13
                                                                                 0: No remap (TIM3_CC1)
                                                                                 1: Remap (TIM20_CC4) */
#define HAL_REMAPADCTRIGGER_ADC34_EXT5        SYSCFG_CFGR4_ADC34_EXT5_RMP   /*!< Input trigger of ADC34 regular channel EXT5
                                                                                 0: No remap (EXTI line 2)
                                                                                 1: Remap (TIM20_TRGO) */
#define HAL_REMAPADCTRIGGER_ADC34_EXT6        SYSCFG_CFGR4_ADC34_EXT6_RMP   /*!< Input trigger of ADC34 regular channel EXT6
                                                                                 0: No remap (TIM4_CC1)
                                                                                 1: Remap (TIM20_TRGO2) */
#define HAL_REMAPADCTRIGGER_ADC34_EXT15       SYSCFG_CFGR4_ADC34_EXT15_RMP  /*!< Input trigger of ADC34 regular channel EXT15
                                                                                 0: No remap (TIM2_CC1)
                                                                                 1: Remap (TIM20_CC1) */
#define HAL_REMAPADCTRIGGER_ADC34_JEXT5       SYSCFG_CFGR4_ADC34_JEXT5_RMP  /*!< Input trigger of ADC34 injected channel JEXT5
                                                                                 0: No remap (TIM4_CC3)
                                                                                 1: Remap (TIM20_TRGO) */
#define HAL_REMAPADCTRIGGER_ADC34_JEXT11      SYSCFG_CFGR4_ADC34_JEXT11_RMP /*!< Input trigger of ADC34 injected channel JEXT11
                                                                                 0: No remap (TIM1_CC3)
                                                                                 1: Remap (TIM20_TRGO2) */
#define HAL_REMAPADCTRIGGER_ADC34_JEXT14      SYSCFG_CFGR4_ADC34_JEXT14_RMP /*!< Input trigger of ADC34 injected channel JEXT14
                                                                                 0: No remap (TIM7_TRGO)
                                                                                 1: Remap (TIM20_CC2) */

#define IS_HAL_REMAPADCTRIGGER(RMP)  ((((RMP) & HAL_REMAPADCTRIGGER_ADC12_EXT2)   == HAL_REMAPADCTRIGGER_ADC12_EXT2)   || \
                                      (((RMP) & HAL_REMAPADCTRIGGER_ADC12_EXT3)   == HAL_REMAPADCTRIGGER_ADC12_EXT3)   || \
                                      (((RMP) & HAL_REMAPADCTRIGGER_ADC12_EXT5)   == HAL_REMAPADCTRIGGER_ADC12_EXT5)   || \
                                      (((RMP) & HAL_REMAPADCTRIGGER_ADC12_EXT13)  == HAL_REMAPADCTRIGGER_ADC12_EXT13)  || \
                                      (((RMP) & HAL_REMAPADCTRIGGER_ADC12_EXT15)  == HAL_REMAPADCTRIGGER_ADC12_EXT15)  || \
                                      (((RMP) & HAL_REMAPADCTRIGGER_ADC12_JEXT3)  == HAL_REMAPADCTRIGGER_ADC12_JEXT3)  || \
                                      (((RMP) & HAL_REMAPADCTRIGGER_ADC12_JEXT6)  == HAL_REMAPADCTRIGGER_ADC12_JEXT6)  || \
                                      (((RMP) & HAL_REMAPADCTRIGGER_ADC12_JEXT13) == HAL_REMAPADCTRIGGER_ADC12_JEXT13) || \
                                      (((RMP) & HAL_REMAPADCTRIGGER_ADC34_EXT5)   == HAL_REMAPADCTRIGGER_ADC34_EXT5)   || \
                                      (((RMP) & HAL_REMAPADCTRIGGER_ADC34_EXT6)   == HAL_REMAPADCTRIGGER_ADC34_EXT6)   || \
                                      (((RMP) & HAL_REMAPADCTRIGGER_ADC34_EXT15)  == HAL_REMAPADCTRIGGER_ADC34_EXT15)  || \
                                      (((RMP) & HAL_REMAPADCTRIGGER_ADC34_JEXT5)  == HAL_REMAPADCTRIGGER_ADC34_JEXT5)  || \
                                      (((RMP) & HAL_REMAPADCTRIGGER_ADC34_JEXT11) == HAL_REMAPADCTRIGGER_ADC34_JEXT11) || \
                                      (((RMP) & HAL_REMAPADCTRIGGER_ADC34_JEXT14) == HAL_REMAPADCTRIGGER_ADC34_JEXT14))
/**
  * @}
  */
#endif /* STM32F303xE || STM32F398xx */

/** @defgroup SYSCFG_FastModePlus_GPIO Fast-mode Plus on GPIO
  * @{
  */

/** @brief  Fast-mode Plus driving capability on a specific GPIO
  */  
#if defined(SYSCFG_CFGR1_I2C_PB6_FMP)
#define SYSCFG_FASTMODEPLUS_PB6    ((uint32_t)SYSCFG_CFGR1_I2C_PB6_FMP)  /*!< Enable Fast-mode Plus on PB6  */
#endif /* SYSCFG_CFGR1_I2C_PB6_FMP */

#if defined(SYSCFG_CFGR1_I2C_PB7_FMP)
#define SYSCFG_FASTMODEPLUS_PB7    ((uint32_t)SYSCFG_CFGR1_I2C_PB7_FMP)  /*!< Enable Fast-mode Plus on PB7  */
#endif /* SYSCFG_CFGR1_I2C_PB7_FMP */

#if defined(SYSCFG_CFGR1_I2C_PB8_FMP)
#define SYSCFG_FASTMODEPLUS_PB8    ((uint32_t)SYSCFG_CFGR1_I2C_PB8_FMP)  /*!< Enable Fast-mode Plus on PB8  */
#endif /* SYSCFG_CFGR1_I2C_PB8_FMP */

#if defined(SYSCFG_CFGR1_I2C_PB9_FMP)
#define SYSCFG_FASTMODEPLUS_PB9    ((uint32_t)SYSCFG_CFGR1_I2C_PB9_FMP)  /*!< Enable Fast-mode Plus on PB9  */
#endif /* SYSCFG_CFGR1_I2C_PB9_FMP */
/**
  * @}
  */

#if defined(SYSCFG_RCR_PAGE0)
/* CCM-SRAM defined */
/** @defgroup HAL_Page_Write_Protection HAL CCM RAM page write protection
  * @{
  */
#define HAL_SYSCFG_WP_PAGE0                    (SYSCFG_RCR_PAGE0)  /*!< ICODE SRAM Write protection page 0 */
#define HAL_SYSCFG_WP_PAGE1                    (SYSCFG_RCR_PAGE1)  /*!< ICODE SRAM Write protection page 1 */
#define HAL_SYSCFG_WP_PAGE2                    (SYSCFG_RCR_PAGE2)  /*!< ICODE SRAM Write protection page 2 */
#define HAL_SYSCFG_WP_PAGE3                    (SYSCFG_RCR_PAGE3)  /*!< ICODE SRAM Write protection page 3 */
#if defined(SYSCFG_RCR_PAGE4)
/* More than 4KB CCM-SRAM defined */
#define HAL_SYSCFG_WP_PAGE4                    (SYSCFG_RCR_PAGE4)  /*!< ICODE SRAM Write protection page 4 */
#define HAL_SYSCFG_WP_PAGE5                    (SYSCFG_RCR_PAGE5)  /*!< ICODE SRAM Write protection page 5 */
#define HAL_SYSCFG_WP_PAGE6                    (SYSCFG_RCR_PAGE6)  /*!< ICODE SRAM Write protection page 6 */
#define HAL_SYSCFG_WP_PAGE7                    (SYSCFG_RCR_PAGE7)  /*!< ICODE SRAM Write protection page 7 */
#endif /* SYSCFG_RCR_PAGE4 */
#if defined(SYSCFG_RCR_PAGE8)
#define HAL_SYSCFG_WP_PAGE8                    (SYSCFG_RCR_PAGE8)  /*!< ICODE SRAM Write protection page 8 */
#define HAL_SYSCFG_WP_PAGE9                    (SYSCFG_RCR_PAGE9)  /*!< ICODE SRAM Write protection page 9 */
#define HAL_SYSCFG_WP_PAGE10                   (SYSCFG_RCR_PAGE10) /*!< ICODE SRAM Write protection page 10 */
#define HAL_SYSCFG_WP_PAGE11                   (SYSCFG_RCR_PAGE11) /*!< ICODE SRAM Write protection page 11 */
#define HAL_SYSCFG_WP_PAGE12                   (SYSCFG_RCR_PAGE12) /*!< ICODE SRAM Write protection page 12 */
#define HAL_SYSCFG_WP_PAGE13                   (SYSCFG_RCR_PAGE13) /*!< ICODE SRAM Write protection page 13 */
#define HAL_SYSCFG_WP_PAGE14                   (SYSCFG_RCR_PAGE14) /*!< ICODE SRAM Write protection page 14 */
#define HAL_SYSCFG_WP_PAGE15                   (SYSCFG_RCR_PAGE15) /*!< ICODE SRAM Write protection page 15 */
#endif /* SYSCFG_RCR_PAGE8 */

#if defined(SYSCFG_RCR_PAGE8)
#define IS_HAL_SYSCFG_WP_PAGE(__PAGE__)        (((__PAGE__) > 0U) && ((__PAGE__) <= 0xFFFFU))
#elif defined(SYSCFG_RCR_PAGE4)
#define IS_HAL_SYSCFG_WP_PAGE(__PAGE__)        (((__PAGE__) > 0U) && ((__PAGE__) <= 0x00FFU))
#else
#define IS_HAL_SYSCFG_WP_PAGE(__PAGE__)        (((__PAGE__) > 0U) && ((__PAGE__) <= 0x000FU))
#endif /* SYSCFG_RCR_PAGE8 */      
/**
  * @}
  */
#endif /* SYSCFG_RCR_PAGE0 */

/** @defgroup HAL_SYSCFG_Interrupts HAL SYSCFG Interrupts
  * @{
  */
#define HAL_SYSCFG_IT_FPU_IOC                  (SYSCFG_CFGR1_FPU_IE_0)  /*!< Floating Point Unit Invalid operation Interrupt */
#define HAL_SYSCFG_IT_FPU_DZC                  (SYSCFG_CFGR1_FPU_IE_1)  /*!< Floating Point Unit Divide-by-zero Interrupt */
#define HAL_SYSCFG_IT_FPU_UFC                  (SYSCFG_CFGR1_FPU_IE_2)  /*!< Floating Point Unit Underflow Interrupt */
#define HAL_SYSCFG_IT_FPU_OFC                  (SYSCFG_CFGR1_FPU_IE_3)  /*!< Floating Point Unit Overflow Interrupt */
#define HAL_SYSCFG_IT_FPU_IDC                  (SYSCFG_CFGR1_FPU_IE_4)  /*!< Floating Point Unit Input denormal Interrupt */
#define HAL_SYSCFG_IT_FPU_IXC                  (SYSCFG_CFGR1_FPU_IE_5)  /*!< Floating Point Unit Inexact Interrupt */

#define IS_HAL_SYSCFG_INTERRUPT(__INTERRUPT__) ((((__INTERRUPT__) & SYSCFG_CFGR1_FPU_IE_0) == SYSCFG_CFGR1_FPU_IE_0) || \
                                                (((__INTERRUPT__) & SYSCFG_CFGR1_FPU_IE_1) == SYSCFG_CFGR1_FPU_IE_1) || \
                                                (((__INTERRUPT__) & SYSCFG_CFGR1_FPU_IE_2) == SYSCFG_CFGR1_FPU_IE_2) || \
                                                (((__INTERRUPT__) & SYSCFG_CFGR1_FPU_IE_3) == SYSCFG_CFGR1_FPU_IE_3) || \
                                                (((__INTERRUPT__) & SYSCFG_CFGR1_FPU_IE_4) == SYSCFG_CFGR1_FPU_IE_4) || \
                                                (((__INTERRUPT__) & SYSCFG_CFGR1_FPU_IE_5) == SYSCFG_CFGR1_FPU_IE_5))

/**
  * @}
  */
  
/**
 * @}
 */ 

/* Exported macros -----------------------------------------------------------*/
/** @defgroup HAL_Exported_Macros HAL Exported Macros
  * @{
  */

/** @defgroup Debug_MCU_APB1_Freeze Freeze/Unfreeze APB1 Peripherals in Debug mode
  * @{
  */
#if defined(DBGMCU_APB1_FZ_DBG_TIM2_STOP)
#define __HAL_DBGMCU_FREEZE_TIM2()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM2_STOP))
#define __HAL_DBGMCU_UNFREEZE_TIM2()         (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM2_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_TIM2_STOP */

#if defined(DBGMCU_APB1_FZ_DBG_TIM3_STOP)
#define __HAL_DBGMCU_FREEZE_TIM3()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM3_STOP))
#define __HAL_DBGMCU_UNFREEZE_TIM3()         (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM3_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_TIM3_STOP */

#if defined(DBGMCU_APB1_FZ_DBG_TIM4_STOP)
#define __HAL_DBGMCU_FREEZE_TIM4()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM4_STOP))
#define __HAL_DBGMCU_UNFREEZE_TIM4()         (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM4_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_TIM4_STOP */

#if defined(DBGMCU_APB1_FZ_DBG_TIM5_STOP)
#define __HAL_DBGMCU_FREEZE_TIM5()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM5_STOP))
#define __HAL_DBGMCU_UNFREEZE_TIM5()         (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM5_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_TIM5_STOP */

#if defined(DBGMCU_APB1_FZ_DBG_TIM6_STOP)
#define __HAL_DBGMCU_FREEZE_TIM6()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM6_STOP))
#define __HAL_DBGMCU_UNFREEZE_TIM6()         (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM6_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_TIM6_STOP */

#if defined(DBGMCU_APB1_FZ_DBG_TIM7_STOP)
#define __HAL_DBGMCU_FREEZE_TIM7()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM7_STOP))
#define __HAL_DBGMCU_UNFREEZE_TIM7()         (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM7_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_TIM7_STOP */

#if defined(DBGMCU_APB1_FZ_DBG_TIM12_STOP)
#define __HAL_DBGMCU_FREEZE_TIM12()          (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM12_STOP))
#define __HAL_DBGMCU_UNFREEZE_TIM12()        (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM12_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_TIM12_STOP */

#if defined(DBGMCU_APB1_FZ_DBG_TIM13_STOP)
#define __HAL_DBGMCU_FREEZE_TIM13()          (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM13_STOP))
#define __HAL_DBGMCU_UNFREEZE_TIM13()        (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM13_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_TIM13_STOP */

#if defined(DBGMCU_APB1_FZ_DBG_TIM14_STOP)
#define __HAL_DBGMCU_FREEZE_TIM14()          (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM14_STOP))
#define __HAL_DBGMCU_UNFREEZE_TIM14()        (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM14_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_TIM14_STOP */

#if defined(DBGMCU_APB1_FZ_DBG_TIM18_STOP)
#define __HAL_FREEZE_TIM18_DBGMCU()          (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM18_STOP))
#define __HAL_UNFREEZE_TIM18_DBGMCU()        (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM18_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_TIM14_STOP */

#if defined(DBGMCU_APB1_FZ_DBG_RTC_STOP)
#define __HAL_DBGMCU_FREEZE_RTC()            (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_RTC_STOP))
#define __HAL_DBGMCU_UNFREEZE_RTC()          (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_RTC_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_RTC_STOP */

#if defined(DBGMCU_APB1_FZ_DBG_WWDG_STOP)
#define __HAL_DBGMCU_FREEZE_WWDG()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_WWDG_STOP))
#define __HAL_DBGMCU_UNFREEZE_WWDG()         (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_WWDG_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_WWDG_STOP */

#if defined(DBGMCU_APB1_FZ_DBG_IWDG_STOP)
#define __HAL_DBGMCU_FREEZE_IWDG()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_IWDG_STOP))
#define __HAL_DBGMCU_UNFREEZE_IWDG()         (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_IWDG_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_IWDG_STOP */

#if defined(DBGMCU_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT)
#define __HAL_DBGMCU_FREEZE_I2C1_TIMEOUT()   (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT))
#define __HAL_DBGMCU_UNFREEZE_I2C1_TIMEOUT() (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT))
#endif /* DBGMCU_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT */

#if defined(DBGMCU_APB1_FZ_DBG_I2C2_SMBUS_TIMEOUT)
#define __HAL_DBGMCU_FREEZE_I2C2_TIMEOUT()   (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_I2C2_SMBUS_TIMEOUT))
#define __HAL_DBGMCU_UNFREEZE_I2C2_TIMEOUT() (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_I2C2_SMBUS_TIMEOUT))
#endif /* DBGMCU_APB1_FZ_DBG_I2C2_SMBUS_TIMEOUT */

#if defined(DBGMCU_APB1_FZ_DBG_I2C3_SMBUS_TIMEOUT)
#define __HAL_DBGMCU_FREEZE_I2C3_TIMEOUT()   (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_I2C3_SMBUS_TIMEOUT))
#define __HAL_DBGMCU_UNFREEZE_I2C3_TIMEOUT() (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_I2C3_SMBUS_TIMEOUT))
#endif /* DBGMCU_APB1_FZ_DBG_I2C3_SMBUS_TIMEOUT */

#if defined(DBGMCU_APB1_FZ_DBG_CAN_STOP)
#define __HAL_FREEZE_CAN_DBGMCU()            (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_CAN_STOP))
#define __HAL_UNFREEZE_CAN_DBGMCU()          (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_CAN_STOP))
#endif /* DBGMCU_APB1_FZ_DBG_CAN_STOP */
/**
 * @}
 */
 
/** @defgroup Debug_MCU_APB2_Freeze Freeze/Unfreeze APB2 Peripherals in Debug mode
  * @{
  */
#if defined(DBGMCU_APB2_FZ_DBG_TIM1_STOP)
#define __HAL_DBGMCU_FREEZE_TIM1()           (DBGMCU->APB2FZ |= (DBGMCU_APB2_FZ_DBG_TIM1_STOP))
#define __HAL_DBGMCU_UNFREEZE_TIM1()           (DBGMCU->APB2FZ &= ~(DBGMCU_APB2_FZ_DBG_TIM1_STOP))
#endif /* DBGMCU_APB2_FZ_DBG_TIM1_STOP */

#if defined(DBGMCU_APB2_FZ_DBG_TIM8_STOP)
#define __HAL_DBGMCU_FREEZE_TIM8()           (DBGMCU->APB2FZ |= (DBGMCU_APB2_FZ_DBG_TIM8_STOP))
#define __HAL_DBGMCU_UNFREEZE_TIM8()         (DBGMCU->APB2FZ &= ~(DBGMCU_APB2_FZ_DBG_TIM8_STOP))
#endif /* DBGMCU_APB2_FZ_DBG_TIM8_STOP */

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

#if defined(DBGMCU_APB2_FZ_DBG_TIM19_STOP)
#define __HAL_FREEZE_TIM19_DBGMCU()          (DBGMCU->APB2FZ |= (DBGMCU_APB2_FZ_DBG_TIM19_STOP))
#define __HAL_UNFREEZE_TIM19_DBGMCU()        (DBGMCU->APB2FZ &= ~(DBGMCU_APB2_FZ_DBG_TIM19_STOP))
#endif /* DBGMCU_APB2_FZ_DBG_TIM19_STOP */

#if defined(DBGMCU_APB2_FZ_DBG_TIM20_STOP)
#define __HAL_FREEZE_TIM20_DBGMCU()          (DBGMCU->APB2FZ |= (DBGMCU_APB2_FZ_DBG_TIM20_STOP))
#define __HAL_UNFREEZE_TIM20_DBGMCU()        (DBGMCU->APB2FZ &= ~(DBGMCU_APB2_FZ_DBG_TIM20_STOP))
#endif /* DBGMCU_APB2_FZ_DBG_TIM20_STOP */

#if defined(DBGMCU_APB2_FZ_DBG_HRTIM1_STOP)
#define __HAL_FREEZE_HRTIM1_DBGMCU()          (DBGMCU->APB2FZ |= (DBGMCU_APB2_FZ_DBG_HRTIM1_STOP))
#define __HAL_UNFREEZE_HRTIM1_DBGMCU()        (DBGMCU->APB2FZ &= ~(DBGMCU_APB2_FZ_DBG_HRTIM1_STOP))
#endif /* DBGMCU_APB2_FZ_DBG_HRTIM1_STOP */
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
                                            }while(0U)
#endif /* SYSCFG_CFGR1_MEM_MODE_0 */

#if defined(SYSCFG_CFGR1_MEM_MODE_0) && defined(SYSCFG_CFGR1_MEM_MODE_1)
/** @brief  Embedded SRAM mapped at 0x00000000
  */
#define __HAL_SYSCFG_REMAPMEMORY_SRAM()         do {SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_MEM_MODE); \
                                             SYSCFG->CFGR1 |= (SYSCFG_CFGR1_MEM_MODE_0 | SYSCFG_CFGR1_MEM_MODE_1); \
                                            }while(0U)
#endif /* SYSCFG_CFGR1_MEM_MODE_0 && SYSCFG_CFGR1_MEM_MODE_1 */

#if defined(SYSCFG_CFGR1_MEM_MODE_2)
#define __HAL_SYSCFG_FMC_BANK()         do {SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_MEM_MODE); \
                                     SYSCFG->CFGR1 |= (SYSCFG_CFGR1_MEM_MODE_2); \
                                    }while(0U)
#endif /* SYSCFG_CFGR1_MEM_MODE_2 */
/**
 * @}
 */
 
/** @defgroup Encoder_Mode Encoder Mode
  * @{
  */
#if defined(SYSCFG_CFGR1_ENCODER_MODE)
/** @brief  No Encoder mode
  */
#define __HAL_REMAPENCODER_NONE()        (SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_ENCODER_MODE))
#endif /* SYSCFG_CFGR1_ENCODER_MODE */

#if defined(SYSCFG_CFGR1_ENCODER_MODE_0)
/** @brief  Encoder mode : TIM2 IC1 and TIM2 IC2 are connected to TIM15 IC1 and TIM15 IC2 respectively
  */
#define __HAL_REMAPENCODER_TIM2()        do {SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_ENCODER_MODE); \
                                             SYSCFG->CFGR1 |= SYSCFG_CFGR1_ENCODER_MODE_0;  \
                                            }while(0U)
#endif /* SYSCFG_CFGR1_ENCODER_MODE_0 */

#if defined(SYSCFG_CFGR1_ENCODER_MODE_1)
/** @brief  Encoder mode : TIM3 IC1 and TIM3 IC2 are connected to TIM15 IC1 and TIM15 IC2 respectively
  */
#define __HAL_REMAPENCODER_TIM3()        do {SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_ENCODER_MODE); \
                                             SYSCFG->CFGR1 |= SYSCFG_CFGR1_ENCODER_MODE_1;  \
                                            }while(0U)
#endif /* SYSCFG_CFGR1_ENCODER_MODE_1 */

#if defined(SYSCFG_CFGR1_ENCODER_MODE_0) && defined(SYSCFG_CFGR1_ENCODER_MODE_1)
/** @brief  Encoder mode : TIM4 IC1 and TIM4 IC2 are connected to TIM15 IC1 and TIM15 IC2 (STM32F303xB/C and STM32F358xx devices)
  */
#define __HAL_REMAPENCODER_TIM4()        do {SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_ENCODER_MODE); \
                                             SYSCFG->CFGR1 |= (SYSCFG_CFGR1_ENCODER_MODE_0 | SYSCFG_CFGR1_ENCODER_MODE_1);  \
                                            }while(0U)
#endif /* SYSCFG_CFGR1_ENCODER_MODE_0 && SYSCFG_CFGR1_ENCODER_MODE_1 */
/**
 * @}
 */
 
/** @defgroup DMA_Remap_Enable DMA Remap Enable
  * @{
  */
#if defined(SYSCFG_CFGR3_DMA_RMP) && defined(SYSCFG_CFGR1_DMA_RMP)
/** @brief  DMA remapping enable/disable macros
  * @param __DMA_REMAP__ This parameter can be a value of @ref HAL_DMA_Remapping
  */
#define __HAL_DMA_REMAP_CHANNEL_ENABLE(__DMA_REMAP__)   do {assert_param(IS_DMA_REMAP((__DMA_REMAP__)));                  \
                                                           (((__DMA_REMAP__) & HAL_REMAP_CFGR3_MASK) ?                      \
                                                             (SYSCFG->CFGR3 |= ((__DMA_REMAP__) & ~HAL_REMAP_CFGR3_MASK)) : \
                                                             (SYSCFG->CFGR1 |= (__DMA_REMAP__)));                           \
                                                         }while(0U)
#define __HAL_DMA_REMAP_CHANNEL_DISABLE(__DMA_REMAP__)  do {assert_param(IS_DMA_REMAP((__DMA_REMAP__)));                  \
                                                           (((__DMA_REMAP__) & HAL_REMAP_CFGR3_MASK) ?                      \
                                                             (SYSCFG->CFGR3 &= (~(__DMA_REMAP__) | HAL_REMAP_CFGR3_MASK)) : \
                                                             (SYSCFG->CFGR1 &= ~(__DMA_REMAP__)));                          \
                                                         }while(0U)
#elif defined(SYSCFG_CFGR1_DMA_RMP)
/** @brief  DMA remapping enable/disable macros
  * @param __DMA_REMAP__ This parameter can be a value of @ref HAL_DMA_Remapping
  */
#define __HAL_DMA_REMAP_CHANNEL_ENABLE(__DMA_REMAP__)   do {assert_param(IS_DMA_REMAP((__DMA_REMAP__)));                  \
                                                           SYSCFG->CFGR1 |= (__DMA_REMAP__);                                \
                                                         }while(0U)
#define __HAL_DMA_REMAP_CHANNEL_DISABLE(__DMA_REMAP__)  do {assert_param(IS_DMA_REMAP((__DMA_REMAP__)));                  \
                                                           SYSCFG->CFGR1 &= ~(__DMA_REMAP__);                               \
                                                         }while(0U)
#endif /* SYSCFG_CFGR3_DMA_RMP || SYSCFG_CFGR1_DMA_RMP */
/**
 * @}
 */
 
/** @defgroup FastModePlus_GPIO Fast-mode Plus on GPIO
  * @{
  */
/** @brief  Fast-mode Plus driving capability enable/disable macros
  * @param __FASTMODEPLUS__ This parameter can be a value of @ref SYSCFG_FastModePlus_GPIO values.
  *                          That you can find above these macros.
  */
#define __HAL_SYSCFG_FASTMODEPLUS_ENABLE(__FASTMODEPLUS__)  do {assert_param(IS_SYSCFG_FASTMODEPLUS((__FASTMODEPLUS__)));\
                                                                SET_BIT(SYSCFG->CFGR1, (__FASTMODEPLUS__));\
                                                               }while(0U)

#define __HAL_SYSCFG_FASTMODEPLUS_DISABLE(__FASTMODEPLUS__) do {assert_param(IS_SYSCFG_FASTMODEPLUS((__FASTMODEPLUS__)));\
                                                                CLEAR_BIT(SYSCFG->CFGR1, (__FASTMODEPLUS__));\
                                                               }while(0U)
/**
 * @}
 */

/** @defgroup Floating_Point_Unit_Interrupts_Enable Floating Point Unit Interrupts Enable
  * @{
  */
/** @brief  SYSCFG interrupt enable/disable macros
  * @param __INTERRUPT__ This parameter can be a value of @ref HAL_SYSCFG_Interrupts
  */
#define __HAL_SYSCFG_INTERRUPT_ENABLE(__INTERRUPT__)        do {assert_param(IS_HAL_SYSCFG_INTERRUPT((__INTERRUPT__))); \
                                                                SYSCFG->CFGR1 |= (__INTERRUPT__);                       \
                                                               }while(0U)

#define __HAL_SYSCFG_INTERRUPT_DISABLE(__INTERRUPT__)       do {assert_param(IS_HAL_SYSCFG_INTERRUPT((__INTERRUPT__))); \
                                                                SYSCFG->CFGR1 &= ~(__INTERRUPT__);                      \
                                                               }while(0U)
/**
 * @}
 */
 
#if defined(SYSCFG_CFGR1_USB_IT_RMP)
/** @defgroup USB_Interrupt_Remap USB Interrupt Remap
  * @{
  */ 
/** @brief  USB interrupt remapping enable/disable macros
  */
#define __HAL_REMAPINTERRUPT_USB_ENABLE()              (SYSCFG->CFGR1 |= (SYSCFG_CFGR1_USB_IT_RMP))
#define __HAL_REMAPINTERRUPT_USB_DISABLE()             (SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_USB_IT_RMP))
/**
 * @}
 */
#endif /* SYSCFG_CFGR1_USB_IT_RMP */
 
#if defined(SYSCFG_CFGR1_VBAT)
/** @defgroup VBAT_Monitoring_Enable VBAT Monitoring Enable
  * @{
  */  
/** @brief  SYSCFG interrupt enable/disable macros
  */
#define __HAL_SYSCFG_VBAT_MONITORING_ENABLE()          (SYSCFG->CFGR1 |= (SYSCFG_CFGR1_VBAT))
#define __HAL_SYSCFG_VBAT_MONITORING_DISABLE()         (SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_VBAT))
/**
 * @}
 */
#endif /* SYSCFG_CFGR1_VBAT */
 
#if defined(SYSCFG_CFGR2_LOCKUP_LOCK)
/** @defgroup Cortex_Lockup_Enable Cortex Lockup Enable
  * @{
  */
/** @brief  SYSCFG Break Lockup lock
  *         Enables and locks the connection of Cortex-M4 LOCKUP (Hardfault) output to TIM1/15/16/17 Break input
  * @note   The selected configuration is locked and can be unlocked by system reset
  */
#define __HAL_SYSCFG_BREAK_LOCKUP_LOCK()   do {SYSCFG->CFGR2 &= ~(SYSCFG_CFGR2_LOCKUP_LOCK); \
                                               SYSCFG->CFGR2 |= SYSCFG_CFGR2_LOCKUP_LOCK;    \
                                              }while(0U)
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
                                              }while(0U)
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
                                                }while(0U)
/**
 * @}
 */
#endif /* SYSCFG_CFGR2_SRAM_PARITY_LOCK */
 
/** @defgroup Trigger_Remapping_Enable Trigger Remapping Enable
  * @{
  */
#if defined(SYSCFG_CFGR3_TRIGGER_RMP)
/** @brief  Trigger remapping enable/disable macros
  * @param __TRIGGER_REMAP__ This parameter can be a value of @ref HAL_Trigger_Remapping
  */
#define __HAL_REMAPTRIGGER_ENABLE(__TRIGGER_REMAP__)   do {assert_param(IS_HAL_REMAPTRIGGER((__TRIGGER_REMAP__)));             \
                                                           (((__TRIGGER_REMAP__) & HAL_REMAP_CFGR3_MASK) ?                     \
                                                             (SYSCFG->CFGR3 |= ((__TRIGGER_REMAP__) & ~HAL_REMAP_CFGR3_MASK)) : \
                                                             (SYSCFG->CFGR1 |= (__TRIGGER_REMAP__)));                           \
                                                         }while(0U)
#define __HAL_REMAPTRIGGER_DISABLE(__TRIGGER_REMAP__)  do {assert_param(IS_HAL_REMAPTRIGGER((__TRIGGER_REMAP__)));             \
                                                           (((__TRIGGER_REMAP__) & HAL_REMAP_CFGR3_MASK) ?                     \
                                                             (SYSCFG->CFGR3 &= (~(__TRIGGER_REMAP__) | HAL_REMAP_CFGR3_MASK)) : \
                                                             (SYSCFG->CFGR1 &= ~(__TRIGGER_REMAP__)));                          \
                                                         }while(0U)
#else
/** @brief  Trigger remapping enable/disable macros
  * @param __TRIGGER_REMAP__ This parameter can be a value of @ref HAL_Trigger_Remapping
  */
#define __HAL_REMAPTRIGGER_ENABLE(__TRIGGER_REMAP__)   do {assert_param(IS_HAL_REMAPTRIGGER((__TRIGGER_REMAP__)));             \
                                                           (SYSCFG->CFGR1 |= (__TRIGGER_REMAP__));                           \
                                                         }while(0U)
#define __HAL_REMAPTRIGGER_DISABLE(__TRIGGER_REMAP__)  do {assert_param(IS_HAL_REMAPTRIGGER((__TRIGGER_REMAP__)));             \
                                                           (SYSCFG->CFGR1 &= ~(__TRIGGER_REMAP__));                          \
                                                         }while(0U)
#endif /* SYSCFG_CFGR3_TRIGGER_RMP */
/**
 * @}
 */
 
#if defined (STM32F302xE) || defined (STM32F303xE) || defined (STM32F398xx)
/** @defgroup ADC_Trigger_Remapping_Enable ADC Trigger Remapping Enable
  * @{
  */
/** @brief  ADC trigger remapping enable/disable macros
  * @param __ADCTRIGGER_REMAP__ This parameter can be a value of @ref HAL_ADC_Trigger_Remapping
  */
#define __HAL_REMAPADCTRIGGER_ENABLE(__ADCTRIGGER_REMAP__)   do {assert_param(IS_HAL_REMAPADCTRIGGER((__ADCTRIGGER_REMAP__)));   \
                                                             (SYSCFG->CFGR4 |= (__ADCTRIGGER_REMAP__));                          \
                                                         }while(0U)
#define __HAL_REMAPADCTRIGGER_DISABLE(__ADCTRIGGER_REMAP__)  do {assert_param(IS_HAL_REMAPADCTRIGGER((__ADCTRIGGER_REMAP__)));   \
                                                             (SYSCFG->CFGR4 &= ~(__ADCTRIGGER_REMAP__));                         \
                                                         }while(0U)
/**
 * @}
 */
#endif /* STM32F302xE || STM32F303xE || STM32F398xx */
                                                           
#if defined(SYSCFG_CFGR2_BYP_ADDR_PAR)
/** @defgroup RAM_Parity_Check_Disable RAM Parity Check Disable
  * @{
  */
/**
  * @brief  Parity check on RAM disable macro
  * @note   Disabling the parity check on RAM locks the configuration bit.
  *         To re-enable the parity check on RAM perform a system reset.
  */
#define __HAL_SYSCFG_RAM_PARITYCHECK_DISABLE()         (*(__IO uint32_t *) CFGR2_BYPADDRPAR_BB = 0x00000001U)
/**
 * @}
 */
#endif /* SYSCFG_CFGR2_BYP_ADDR_PAR */
 
#if defined(SYSCFG_RCR_PAGE0)
/** @defgroup CCM_RAM_Page_Write_Protection_Enable CCM RAM page write protection enable
  * @{
  */
/** @brief  CCM RAM page write protection enable macro
  * @param __PAGE_WP__ This parameter can be a value of @ref HAL_Page_Write_Protection
  * @note   write protection can only be disabled by a system reset
  */
#define __HAL_SYSCFG_SRAM_WRP_ENABLE(__PAGE_WP__)      do {assert_param(IS_HAL_SYSCFG_WP_PAGE((__PAGE_WP__))); \
                                                           SYSCFG->RCR |= (__PAGE_WP__);                       \
                                                          }while(0U)
/**
 * @}
 */
#endif /* SYSCFG_RCR_PAGE0 */
 
/**
 * @}
 */ 
/* Private macro -------------------------------------------------------------*/
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
/** @addtogroup HAL_Exported_Functions HAL Exported Functions
  * @{
  */

/** @addtogroup HAL_Exported_Functions_Group1 Initialization and de-initialization Functions 
 *  @brief    Initialization and de-initialization functions
 * @{
 */
/* Initialization and de-initialization functions  ******************************/
HAL_StatusTypeDef HAL_Init(void);
HAL_StatusTypeDef HAL_DeInit(void);
void HAL_MspInit(void);
void HAL_MspDeInit(void);
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
 
/** @addtogroup HAL_Exported_Functions_Group2 HAL Control functions 
 *  @brief    HAL Control functions
 * @{
 */
/* Peripheral Control functions  ************************************************/
void     HAL_IncTick(void);
void     HAL_Delay(uint32_t Delay);
void     HAL_SuspendTick(void);
void     HAL_ResumeTick(void);
uint32_t HAL_GetTick(void);
uint32_t HAL_GetTickPrio(void);
HAL_StatusTypeDef HAL_SetTickFreq(HAL_TickFreqTypeDef Freq);
HAL_TickFreqTypeDef HAL_GetTickFreq(void);
uint32_t HAL_GetHalVersion(void);
uint32_t HAL_GetREVID(void);
uint32_t HAL_GetDEVID(void);
uint32_t HAL_GetUIDw0(void);
uint32_t HAL_GetUIDw1(void);
uint32_t HAL_GetUIDw2(void);
void     HAL_DBGMCU_EnableDBGSleepMode(void);
void     HAL_DBGMCU_DisableDBGSleepMode(void);
void     HAL_DBGMCU_EnableDBGStopMode(void);
void     HAL_DBGMCU_DisableDBGStopMode(void);
void     HAL_DBGMCU_EnableDBGStandbyMode(void);
void     HAL_DBGMCU_DisableDBGStandbyMode(void);
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

#endif /* __STM32F3xx_HAL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
