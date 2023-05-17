/**
  ******************************************************************************
  * @file    stm32u5xx_hal_pwr_ex.h
  * @author  MCD Application Team
  * @brief   Header file of PWR HAL Extended module.
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
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32U5xx_HAL_PWR_EX_H
#define STM32U5xx_HAL_PWR_EX_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal_def.h"

/** @addtogroup STM32U5xx_HAL_Driver
  * @{
  */

/** @addtogroup PWREx
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup PWREx_Exported_Types PWR Extended Exported Types
  * @{
  */

/**
  * @brief  PWR PVM configuration structure definition
  */
typedef struct
{
  uint32_t PVMType;   /*!< Specifies which voltage is monitored.
                           This parameter can be a value of
                           @ref PWREx_PVM_Type.                               */

  uint32_t Mode;      /*!< Specifies the operating mode for the selected pins.
                           This parameter can be a value of
                           @ref PWREx_PVM_Mode.                               */
} PWR_PVMTypeDef;
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup PWREx_Exported_Constants PWR Extended Exported Constants
  * @{
  */

/** @defgroup PWREx_PVM_Type PWR Extended Voltage Monitoring Type
  * @{
  */
#define PWR_UVM   PWR_SVMCR_UVMEN   /*!< Independent USB voltage monitor (VDDUSB versus 1.2 V)    */
#define PWR_IO2VM PWR_SVMCR_IO2VMEN /*!< Independent VDDIO2 voltage monitor (VDDIO2 versus 0.9 V) */
#define PWR_AVM1  PWR_SVMCR_AVM1EN  /*!< Independent VDDA voltage monitor (VDDA versus 1.6 V)     */
#define PWR_AVM2  PWR_SVMCR_AVM2EN  /*!< Independent VDDA voltage monitor (VDDA versus 1.8 V)     */
/**
  * @}
  */

/** @defgroup PWREx_PVM_Mode PWR Extended PVM Interrupt and Event Mode
  * @{
  */
#define PWR_PVM_MODE_NORMAL               (0x00U) /*!< Basic Mode is used                                                 */
#define PWR_PVM_MODE_IT_RISING            (0x05U) /*!< External Interrupt Mode with Rising edge trigger detection         */
#define PWR_PVM_MODE_IT_FALLING           (0x06U) /*!< External Interrupt Mode with Falling edge trigger detection        */
#define PWR_PVM_MODE_IT_RISING_FALLING    (0x07U) /*!< External Interrupt Mode with Rising/Falling edge trigger detection */
#define PWR_PVM_MODE_EVENT_RISING         (0x09U) /*!< Event Mode with Rising edge trigger detection                      */
#define PWR_PVM_MODE_EVENT_FALLING        (0x0AU) /*!< Event Mode with Falling edge trigger detection                     */
#define PWR_PVM_MODE_EVENT_RISING_FALLING (0x0BU) /*!< Event Mode with Rising/Falling edge trigger detection              */
/**
  * @}
  */

/** @defgroup PWREx_SRD_State PWREx SRD Domain State
  * @{
  */
#define PWR_SRD_DOMAIN_STOP (0U)           /*!< SRD in Stop mode when system goes to Stop 0/1/2 mode */
#define PWR_SRD_DOMAIN_RUN  PWR_CR2_SRDRUN /*!< SRD in Run mode when system goes to Stop 0/1/2 mode  */
/**
  * @}
  */

/** @defgroup PWREx_RAM_Contents_Stop_Retention PWR Extended RAM Contents Stop Retention
  * @{
  */
/* SRAM1 pages retention defines */
#define PWR_SRAM1_PAGE1_STOP      (SRAM1_ID  | PAGE01_ID)        /*!< SRAM1 page 1 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM1_PAGE2_STOP      (SRAM1_ID  | PAGE02_ID)        /*!< SRAM1 page 2 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM1_PAGE3_STOP      (SRAM1_ID  | PAGE03_ID)        /*!< SRAM1 page 3 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#if defined (PWR_CR4_SRAM1PDS4)
#define PWR_SRAM1_PAGE4_STOP      (SRAM1_ID  | PAGE04_ID)        /*!< SRAM1 page 4 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM1_PAGE5_STOP      (SRAM1_ID  | PAGE05_ID)        /*!< SRAM1 page 5 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM1_PAGE6_STOP      (SRAM1_ID  | PAGE06_ID)        /*!< SRAM1 page 6 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM1_PAGE7_STOP      (SRAM1_ID  | PAGE07_ID)        /*!< SRAM1 page 7 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM1_PAGE8_STOP      (SRAM1_ID  | PAGE08_ID)        /*!< SRAM1 page 8 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM1_PAGE9_STOP      (SRAM1_ID  | PAGE09_ID)        /*!< SRAM1 page 9 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM1_PAGE10_STOP     (SRAM1_ID  | PAGE10_ID)        /*!< SRAM1 page 10 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)   */
#define PWR_SRAM1_PAGE11_STOP     (SRAM1_ID  | PAGE11_ID)        /*!< SRAM1 page 11 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)   */
#define PWR_SRAM1_PAGE12_STOP     (SRAM1_ID  | PAGE12_ID)        /*!< SRAM1 page 12 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)   */
#define PWR_SRAM1_FULL_STOP       (SRAM1_ID  | 0x0FFFU)          /*!< SRAM1 all pages retention in Stop modes (Stop 0, 1, 2, 3)         */
#else
#define PWR_SRAM1_FULL_STOP       (SRAM1_ID  | 0x07U)            /*!< SRAM1 all pages retention in Stop modes (Stop 0, 1, 2, 3)         */
#endif /* defined (PWR_CR4_SRAM1PDS4) */

/* SRAM2 pages retention defines */
#define PWR_SRAM2_PAGE1_STOP      (SRAM2_ID  | PAGE01_ID)        /*!< SRAM2 page 1 (8 KB) retention in Stop modes  (Stop 0, 1, 2)       */
#define PWR_SRAM2_PAGE2_STOP      (SRAM2_ID  | PAGE02_ID)        /*!< SRAM2 page 2 (54 KB) retention in Stop modes (Stop 0, 1, 2)       */
#define PWR_SRAM2_FULL_STOP       (SRAM2_ID  | 0x03U)            /*!< SRAM2 all pages retention in Stop modes      (Stop 0, 1, 2)       */

#if defined (PWR_CR2_SRAM3PDS1)
/* SRAM3 pages retention defines */
#define PWR_SRAM3_PAGE1_STOP      (SRAM3_ID  | PAGE01_ID)        /*!< SRAM3 page 1 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM3_PAGE2_STOP      (SRAM3_ID  | PAGE02_ID)        /*!< SRAM3 page 2 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM3_PAGE3_STOP      (SRAM3_ID  | PAGE03_ID)        /*!< SRAM3 page 3 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM3_PAGE4_STOP      (SRAM3_ID  | PAGE04_ID)        /*!< SRAM3 page 4 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM3_PAGE5_STOP      (SRAM3_ID  | PAGE05_ID)        /*!< SRAM3 page 5 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM3_PAGE6_STOP      (SRAM3_ID  | PAGE06_ID)        /*!< SRAM3 page 6 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM3_PAGE7_STOP      (SRAM3_ID  | PAGE07_ID)        /*!< SRAM3 page 7 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM3_PAGE8_STOP      (SRAM3_ID  | PAGE08_ID)        /*!< SRAM3 page 8 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#if defined (PWR_CR4_SRAM3PDS9)
#define PWR_SRAM3_PAGE9_STOP      (SRAM3_ID  | PAGE09_ID)        /*!< SRAM3 page 9 (64 KB) retention in Stop modes  (Stop 0, 1, 2, 3)   */
#define PWR_SRAM3_PAGE10_STOP     (SRAM3_ID  | PAGE10_ID)        /*!< SRAM3 page 10 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)   */
#define PWR_SRAM3_PAGE11_STOP     (SRAM3_ID  | PAGE11_ID)        /*!< SRAM3 page 11 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)   */
#define PWR_SRAM3_PAGE12_STOP     (SRAM3_ID  | PAGE12_ID)        /*!< SRAM3 page 12 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)   */
#define PWR_SRAM3_PAGE13_STOP     (SRAM3_ID  | PAGE13_ID)        /*!< SRAM3 page 13 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)   */
#define PWR_SRAM3_FULL_STOP       (SRAM3_ID  | 0x1FFFU)          /*!< SRAM3 all pages retention in Stop modes       (Stop 0, 1, 2, 3)   */
#else
#define PWR_SRAM3_FULL_STOP       (SRAM3_ID  | 0xFFU)            /*!< SRAM3 all pages retention in Stop modes       (Stop 0, 1, 2, 3)   */
#endif /* defined (PWR_CR4_SRAM3PDS9) */
#endif /* PWR_CR2_SRAM3PDS1 */

/* SRAM4 page retention defines */
#define PWR_SRAM4_FULL_STOP       (SRAM4_ID  | PAGE01_ID)        /*!< SRAM4 retention in Stop modes (Stop 0, 1, 2, 3)                   */

#if defined (PWR_CR4_SRAM5PDS1)
/* SRAM5 pages retention defines */
#define PWR_SRAM5_PAGE1_STOP      (SRAM5_ID  | PAGE01_ID)        /*!< SRAM5 page 1 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM5_PAGE2_STOP      (SRAM5_ID  | PAGE02_ID)        /*!< SRAM5 page 2 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM5_PAGE3_STOP      (SRAM5_ID  | PAGE03_ID)        /*!< SRAM5 page 3 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM5_PAGE4_STOP      (SRAM5_ID  | PAGE04_ID)        /*!< SRAM5 page 4 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM5_PAGE5_STOP      (SRAM5_ID  | PAGE05_ID)        /*!< SRAM5 page 5 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM5_PAGE6_STOP      (SRAM5_ID  | PAGE06_ID)        /*!< SRAM5 page 6 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM5_PAGE7_STOP      (SRAM5_ID  | PAGE07_ID)        /*!< SRAM5 page 7 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM5_PAGE8_STOP      (SRAM5_ID  | PAGE08_ID)        /*!< SRAM5 page 8 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM5_PAGE9_STOP      (SRAM5_ID  | PAGE09_ID)        /*!< SRAM5 page 9 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM5_PAGE10_STOP     (SRAM5_ID  | PAGE10_ID)        /*!< SRAM5 page 10 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)   */
#define PWR_SRAM5_PAGE11_STOP     (SRAM5_ID  | PAGE11_ID)        /*!< SRAM5 page 11 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)   */
#define PWR_SRAM5_PAGE12_STOP     (SRAM5_ID  | PAGE12_ID)        /*!< SRAM5 page 12 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)   */
#define PWR_SRAM5_PAGE13_STOP     (SRAM5_ID  | PAGE13_ID)        /*!< SRAM5 page 13 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)   */
#define PWR_SRAM5_FULL_STOP       (SRAM5_ID  | 0x1FFFU)          /*!< SRAM5 all pages retention in Stop modes      (Stop 0, 1, 2, 3)    */
#endif /* defined (PWR_CR4_SRAM5PDS1) */

#if defined (PWR_CR5_SRAM6PDS1)
/* SRAM5 pages retention defines */
#define PWR_SRAM6_PAGE1_STOP      (SRAM6_ID  | PAGE01_ID)        /*!< SRAM6 page 1 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM6_PAGE2_STOP      (SRAM6_ID  | PAGE02_ID)        /*!< SRAM6 page 2 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM6_PAGE3_STOP      (SRAM6_ID  | PAGE03_ID)        /*!< SRAM6 page 3 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM6_PAGE4_STOP      (SRAM6_ID  | PAGE04_ID)        /*!< SRAM6 page 4 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM6_PAGE5_STOP      (SRAM6_ID  | PAGE05_ID)        /*!< SRAM6 page 5 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM6_PAGE6_STOP      (SRAM6_ID  | PAGE06_ID)        /*!< SRAM6 page 6 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM6_PAGE7_STOP      (SRAM6_ID  | PAGE07_ID)        /*!< SRAM6 page 7 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM6_PAGE8_STOP      (SRAM6_ID  | PAGE08_ID)        /*!< SRAM6 page 8 (64 KB) retention in Stop modes (Stop 0, 1, 2, 3)    */
#define PWR_SRAM6_FULL_STOP       (SRAM6_ID  | 0xFFU)            /*!< SRAM6 all pages retention in Stop modes      (Stop 0, 1, 2, 3)    */
#endif /* defined (PWR_CR5_SRAM6PDS1) */

/* Cache RAMs retention defines */
#define PWR_ICACHE_FULL_STOP      (ICACHERAM_ID  | PAGE01_ID)        /*!< ICACHE page retention in Stop modes (Stop 0, 1, 2, 3)             */
#define PWR_DCACHE1_FULL_STOP     (DCACHE1RAM_ID | PAGE01_ID)        /*!< DCACHE1 page retention in Stop modes (Stop 0, 1, 2, 3)            */
#if defined (PWR_CR2_DC2RAMPDS)
#define PWR_DCACHE2_FULL_STOP     (DCACHE2RAM_ID | PAGE01_ID)        /*!< DCACHE2 page retention in Stop modes (Stop 0, 1, 2, 3)            */
#endif /* defined (PWR_CR2_DC2RAMPDS) */

#if defined (PWR_CR2_DMA2DRAMPDS)
/* DMA2D RAM retention defines */
#define PWR_DMA2DRAM_FULL_STOP    (DMA2DRAM_ID   | PAGE01_ID)        /*!< DMA2D RAM retention in Stop modes (Stop 0, 1, 2, 3)               */
#endif /* PWR_CR2_DMA2DRAMPDS */

/* FMAC, FDCAN and USB RAMs retention defines */
#define PWR_PERIPHRAM_FULL_STOP   (PERIPHRAM_ID  | PAGE01_ID)        /*!< FMAC, FDCAN and USB RAM retention in Stop modes (Stop 0, 1, 2, 3) */

/* PKA32 RAM retention defines */
#define PWR_PKA32RAM_FULL_STOP    (PKARAM_ID     | PAGE01_ID)        /*!< PKA32 RAM retention in Stop modes (Stop 0, 1, 2, 3)               */

#if defined (PWR_CR2_GPRAMPDS)
/* Graphic peripherals RAM retention defines */
#define PWR_GRAPHICPRAM_FULL_STOP (GRAPHIPRAM_ID | PAGE01_ID)        /*!< LTDC, GFXMMU retention in Stop modes (Stop 0, 1, 2, 3)            */
#endif /* defined (PWR_CR2_GPRAMPDS) */

#if defined (PWR_CR2_DSIRAMPDS)
/* DSI RAM retention defines */
#define PWR_DSIRAM_FULL_STOP      (DSIRAM_ID     | PAGE01_ID)        /*!< DSI RAM retention in Stop modes (Stop 0, 1, 2, 3)                 */
#endif /* defined (PWR_CR2_DSIRAMPDS) */

#if defined (PWR_CR2_JPEGRAMPDS)
/* JPEG RAM retention defines */
#define PWR_JPEGRAM_FULL_STOP      (JPEGRAM_ID     | PAGE01_ID)      /*!< JPEG RAM retention in Stop modes (Stop 0, 1, 2, 3)                 */
#endif /* defined (PWR_CR2_JPEGRAMPDS) */
/**
  * @}
  */

/** @defgroup PWREx_SRAM2_Contents_Standby_Retention PWR Extended SRAM2 Contents Standby Retention
  * @note  For some products of the U5 family (please see the Reference Manual),
  *        the SRAM2 content is preserved based on the same defines in Stop 3 mode.
  * @{
  */
#define PWR_SRAM2_PAGE1_STANDBY PWR_CR1_RRSB1                   /*!< SRAM2 page 1 (8 KB) retention in Stop 3 and Standby modes  */
#define PWR_SRAM2_PAGE2_STANDBY PWR_CR1_RRSB2                   /*!< SRAM2 page 2 (54 KB) retention in Stop 3 and Standby modes */
#define PWR_SRAM2_FULL_STANDBY  (PWR_CR1_RRSB1 | PWR_CR1_RRSB2) /*!< SRAM2 all pages retention in Stop 3 and Standby modes      */
/**
  * @}
  */

/** @defgroup PWREx_SRAMx_Contents_Run_Retention PWR Extended SRAM Contents Run Retention
  * @{
  */
#define PWR_SRAM1_FULL_RUN PWR_CR1_SRAM1PD /*!< SRAM1 full retention in Run mode */
#define PWR_SRAM2_FULL_RUN PWR_CR1_SRAM2PD /*!< SRAM2 full retention in Run mode */
#if defined (PWR_CR1_SRAM3PD)
#define PWR_SRAM3_FULL_RUN PWR_CR1_SRAM3PD /*!< SRAM3 full retention in Run mode */
#endif /* PWR_CR1_SRAM3PD */
#define PWR_SRAM4_FULL_RUN PWR_CR1_SRAM4PD /*!< SRAM4 full retention in Run mode */
#if defined (PWR_CR1_SRAM5PD)
#define PWR_SRAM5_FULL_RUN PWR_CR1_SRAM5PD /*!< SRAM5 full retention in Run mode */
#endif /* defined (PWR_CR1_SRAM5PD) */
#if defined (PWR_CR1_SRAM6PD)
#define PWR_SRAM6_FULL_RUN PWR_CR1_SRAM6PD /*!< SRAM6 full retention in Run mode */
#endif /* defined (PWR_CR1_SRAM6PD) */
/**
  * @}
  */

/** @defgroup PWREx_Supply_Configuration PWR Extended Supply Configuration
  * @{
  */
#define PWR_LDO_SUPPLY  (0U)             /*!< LDO supply  */
#define PWR_SMPS_SUPPLY (PWR_CR3_REGSEL) /*!< SMPS supply */
/**
  * @}
  */

/** @defgroup PWREx_Regulator_Voltage_Scale PWR Extended Regulator Voltage Scale
  * @{
  */
#define PWR_REGULATOR_VOLTAGE_SCALE1 (PWR_VOSR_VOS_0 | PWR_VOSR_VOS_1) /*!< Voltage scaling range 1 */
#define PWR_REGULATOR_VOLTAGE_SCALE2 PWR_VOSR_VOS_1                    /*!< Voltage scaling range 2 */
#define PWR_REGULATOR_VOLTAGE_SCALE3 PWR_VOSR_VOS_0                    /*!< Voltage scaling range 3 */
#define PWR_REGULATOR_VOLTAGE_SCALE4 (0U)                              /*!< Voltage scaling range 4 */
/**
  * @}
  */

/** @defgroup PWREx_VBAT_Battery_Charging_Selection PWR Extended Battery Charging Resistor Selection
  * @{
  */
#define PWR_BATTERY_CHARGING_RESISTOR_5   (0U)           /*!< VBAT charging through a 5 kOhms resistor   */
#define PWR_BATTERY_CHARGING_RESISTOR_1_5 PWR_BDCR2_VBRS /*!< VBAT charging through a 1.5 kOhms resistor */
/**
  * @}
  */

/** @defgroup PWREx_GPIO_Port PWR Extended GPIO Port
  * @{
  */
#define PWR_GPIO_A (0x00U) /*!< GPIO port A */
#define PWR_GPIO_B (0x01U) /*!< GPIO port B */
#define PWR_GPIO_C (0x02U) /*!< GPIO port C */
#define PWR_GPIO_D (0x03U) /*!< GPIO port D */
#define PWR_GPIO_E (0x04U) /*!< GPIO port E */
#if defined (PWR_PUCRF_PU0)
#define PWR_GPIO_F (0x05U) /*!< GPIO port F */
#endif /* PWR_PUCRF_PU0 */
#define PWR_GPIO_G (0x06U) /*!< GPIO port G */
#define PWR_GPIO_H (0x07U) /*!< GPIO port H */
#if defined (PWR_PUCRI_PU0)
#define PWR_GPIO_I (0x08U) /*!< GPIO port I */
#endif /* PWR_PUCRI_PU0 */
#if defined (PWR_PUCRJ_PU0)
#define PWR_GPIO_J (0x09U) /*!< GPIO port J */
#endif /* defined (PWR_PUCRJ_PU0) */
/**
  * @}
  */

/** @defgroup PWREx_GPIO_Pin_Mask PWR Extended GPIO Pin Mask
  * @{
  */
#define PWR_GPIO_BIT_0  (0x0001U) /*!< GPIO port I/O pin 0  */
#define PWR_GPIO_BIT_1  (0x0002U) /*!< GPIO port I/O pin 1  */
#define PWR_GPIO_BIT_2  (0x0004U) /*!< GPIO port I/O pin 2  */
#define PWR_GPIO_BIT_3  (0x0008U) /*!< GPIO port I/O pin 3  */
#define PWR_GPIO_BIT_4  (0x0010U) /*!< GPIO port I/O pin 4  */
#define PWR_GPIO_BIT_5  (0x0020U) /*!< GPIO port I/O pin 5  */
#define PWR_GPIO_BIT_6  (0x0040U) /*!< GPIO port I/O pin 6  */
#define PWR_GPIO_BIT_7  (0x0080U) /*!< GPIO port I/O pin 7  */
#define PWR_GPIO_BIT_8  (0x0100U) /*!< GPIO port I/O pin 8  */
#define PWR_GPIO_BIT_9  (0x0200U) /*!< GPIO port I/O pin 9  */
#define PWR_GPIO_BIT_10 (0x0400U) /*!< GPIO port I/O pin 10 */
#define PWR_GPIO_BIT_11 (0x0800U) /*!< GPIO port I/O pin 11 */
#define PWR_GPIO_BIT_12 (0x1000U) /*!< GPIO port I/O pin 12 */
#define PWR_GPIO_BIT_13 (0x2000U) /*!< GPIO port I/O pin 13 */
#define PWR_GPIO_BIT_14 (0x4000U) /*!< GPIO port I/O pin 14 */
#define PWR_GPIO_BIT_15 (0x8000U) /*!< GPIO port I/O pin 15 */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/

/** @defgroup PWREx_Exported_Macros PWR Extended Exported Macros
  * @{
  */

/**
  * @brief  Enable the UVM Extended Interrupt Line.
  * @retval None.
  */
#define __HAL_PWR_UVM_EXTI_ENABLE_IT() SET_BIT(EXTI->IMR1, PWR_EXTI_LINE_UVM)

/**
  * @brief  Disable the UVM Extended Interrupt Line.
  * @retval None.
  */
#define __HAL_PWR_UVM_EXTI_DISABLE_IT() CLEAR_BIT(EXTI->IMR1, PWR_EXTI_LINE_UVM)

/**
  * @brief  Enable the UVM Event Line.
  * @retval None.
  */
#define __HAL_PWR_UVM_EXTI_ENABLE_EVENT() SET_BIT(EXTI->EMR1, PWR_EXTI_LINE_UVM)

/**
  * @brief  Disable the UVM Event Line.
  * @retval None.
  */
#define __HAL_PWR_UVM_EXTI_DISABLE_EVENT() CLEAR_BIT(EXTI->EMR1, PWR_EXTI_LINE_UVM)

/**
  * @brief  Enable the UVM Extended Interrupt Rising Trigger.
  * @retval None.
  */
#define __HAL_PWR_UVM_EXTI_ENABLE_RISING_EDGE() SET_BIT(EXTI->RTSR1, PWR_EXTI_LINE_UVM)

/**
  * @brief  Disable the UVM Extended Interrupt Rising Trigger.
  * @retval None.
  */
#define __HAL_PWR_UVM_EXTI_DISABLE_RISING_EDGE() CLEAR_BIT(EXTI->RTSR1, PWR_EXTI_LINE_UVM)

/**
  * @brief  Enable the UVM Extended Interrupt Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_UVM_EXTI_ENABLE_FALLING_EDGE() SET_BIT(EXTI->FTSR1, PWR_EXTI_LINE_UVM)

/**
  * @brief  Disable the UVM Extended Interrupt Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_UVM_EXTI_DISABLE_FALLING_EDGE() CLEAR_BIT(EXTI->FTSR1, PWR_EXTI_LINE_UVM)

/**
  * @brief  Enable the UVM Extended Interrupt Rising & Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_UVM_EXTI_ENABLE_RISING_FALLING_EDGE() \
  do                                                    \
  {                                                     \
    __HAL_PWR_UVM_EXTI_ENABLE_RISING_EDGE();            \
    __HAL_PWR_UVM_EXTI_ENABLE_FALLING_EDGE();           \
  } while(0)

/**
  * @brief  Disable the UVM Extended Interrupt Rising & Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_UVM_EXTI_DISABLE_RISING_FALLING_EDGE() \
  do                                                     \
  {                                                      \
    __HAL_PWR_UVM_EXTI_DISABLE_RISING_EDGE();            \
    __HAL_PWR_UVM_EXTI_DISABLE_FALLING_EDGE();           \
  } while(0)

/**
  * @brief  Generate a Software Interrupt on UVM EXTI Line.
  * @retval None
  */
#define __HAL_PWR_UVM_EXTI_GENERATE_SWIT() SET_BIT(EXTI->SWIER1, PWR_EXTI_LINE_UVM)

/**
  * @brief  Check whether the specified UVM EXTI flag is set or not.
  * @retval EXTI UVM Line Status.
  */
#define __HAL_PWR_UVM_EXTI_GET_FLAG() ((EXTI->RPR1 | EXTI->FPR1) & PWR_EXTI_LINE_UVM)

/**
  * @brief  Clear the UVM EXTI flag.
  * @retval None.
  */
#define __HAL_PWR_UVM_EXTI_CLEAR_FLAG()       \
  do                                          \
  {                                           \
    WRITE_REG(EXTI->RPR1, PWR_EXTI_LINE_UVM); \
    WRITE_REG(EXTI->FPR1, PWR_EXTI_LINE_UVM); \
  } while(0)

/**
  * @brief  Enable the IO2VM Extended Interrupt Line.
  * @retval None.
  */
#define __HAL_PWR_IO2VM_EXTI_ENABLE_IT() SET_BIT(EXTI->IMR1, PWR_EXTI_LINE_IO2VM)

/**
  * @brief  Disable the IO2VM Extended Interrupt Line.
  * @retval None.
  */
#define __HAL_PWR_IO2VM_EXTI_DISABLE_IT() CLEAR_BIT(EXTI->IMR1, PWR_EXTI_LINE_IO2VM)

/**
  * @brief  Enable the IO2VM Event Line.
  * @retval None.
  */
#define __HAL_PWR_IO2VM_EXTI_ENABLE_EVENT() SET_BIT(EXTI->EMR1, PWR_EXTI_LINE_IO2VM)

/**
  * @brief  Disable the IO2VM Event Line.
  * @retval None.
  */
#define __HAL_PWR_IO2VM_EXTI_DISABLE_EVENT() CLEAR_BIT(EXTI->EMR1, PWR_EXTI_LINE_IO2VM)

/**
  * @brief  Enable the IO2VM Extended Interrupt Rising Trigger.
  * @retval None.
  */
#define __HAL_PWR_IO2VM_EXTI_ENABLE_RISING_EDGE() SET_BIT(EXTI->RTSR1, PWR_EXTI_LINE_IO2VM)

/**
  * @brief  Disable the IO2VM Extended Interrupt Rising Trigger.
  * @retval None.
  */
#define __HAL_PWR_IO2VM_EXTI_DISABLE_RISING_EDGE() CLEAR_BIT(EXTI->RTSR1, PWR_EXTI_LINE_IO2VM)

/**
  * @brief  Enable the IO2VM Extended Interrupt Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_IO2VM_EXTI_ENABLE_FALLING_EDGE() SET_BIT(EXTI->FTSR1, PWR_EXTI_LINE_IO2VM)

/**
  * @brief  Disable the IO2VM Extended Interrupt Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_IO2VM_EXTI_DISABLE_FALLING_EDGE() CLEAR_BIT(EXTI->FTSR1, PWR_EXTI_LINE_IO2VM)

/**
  * @brief  Enable the IO2VM Extended Interrupt Rising & Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_IO2VM_EXTI_ENABLE_RISING_FALLING_EDGE() \
  do                                                      \
  {                                                       \
    __HAL_PWR_IO2VM_EXTI_ENABLE_RISING_EDGE();            \
    __HAL_PWR_IO2VM_EXTI_ENABLE_FALLING_EDGE();           \
  } while(0)

/**
  * @brief  Disable the IO2VM Extended Interrupt Rising & Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_IO2VM_EXTI_DISABLE_RISING_FALLING_EDGE() \
  do                                                       \
  {                                                        \
    __HAL_PWR_IO2VM_EXTI_DISABLE_RISING_EDGE();            \
    __HAL_PWR_IO2VM_EXTI_DISABLE_FALLING_EDGE();           \
  } while(0)

/**
  * @brief  Generate a Software Interrupt on IO2VM EXTI Line.
  * @retval None
  */
#define __HAL_PWR_IO2VM_EXTI_GENERATE_SWIT() SET_BIT(EXTI->SWIER1, PWR_EXTI_LINE_IO2VM)

/**
  * @brief  Check whether the specified IO2VM EXTI flag is set or not.
  * @retval EXTI IO2VM Line Status.
  */
#define __HAL_PWR_IO2VM_EXTI_GET_FLAG() ((EXTI->RPR1 | EXTI->FPR1) & PWR_EXTI_LINE_IO2VM)

/**
  * @brief  Clear the IO2VM EXTI flag.
  * @retval None.
  */
#define __HAL_PWR_IO2VM_EXTI_CLEAR_FLAG()       \
  do                                            \
  {                                             \
    WRITE_REG(EXTI->RPR1, PWR_EXTI_LINE_IO2VM); \
    WRITE_REG(EXTI->FPR1, PWR_EXTI_LINE_IO2VM); \
  } while(0)

/**
  * @brief  Enable the AVM1 Extended Interrupt Line.
  * @retval None.
  */
#define __HAL_PWR_AVM1_EXTI_ENABLE_IT() SET_BIT(EXTI->IMR1, PWR_EXTI_LINE_AVM1)

/**
  * @brief  Disable the AVM1 Extended Interrupt Line.
  * @retval None.
  */
#define __HAL_PWR_AVM1_EXTI_DISABLE_IT() CLEAR_BIT(EXTI->IMR1, PWR_EXTI_LINE_AVM1)

/**
  * @brief  Enable the AVM1 Event Line.
  * @retval None.
  */
#define __HAL_PWR_AVM1_EXTI_ENABLE_EVENT() SET_BIT(EXTI->EMR1, PWR_EXTI_LINE_AVM1)

/**
  * @brief  Disable the AVM1 Event Line.
  * @retval None.
  */
#define __HAL_PWR_AVM1_EXTI_DISABLE_EVENT() CLEAR_BIT(EXTI->EMR1, PWR_EXTI_LINE_AVM1)

/**
  * @brief  Enable the AVM1 Extended Interrupt Rising Trigger.
  * @retval None.
  */
#define __HAL_PWR_AVM1_EXTI_ENABLE_RISING_EDGE() SET_BIT(EXTI->RTSR1, PWR_EXTI_LINE_AVM1)

/**
  * @brief  Disable the AVM1 Extended Interrupt Rising Trigger.
  * @retval None.
  */
#define __HAL_PWR_AVM1_EXTI_DISABLE_RISING_EDGE() CLEAR_BIT(EXTI->RTSR1, PWR_EXTI_LINE_AVM1)

/**
  * @brief  Enable the AVM1 Extended Interrupt Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_AVM1_EXTI_ENABLE_FALLING_EDGE() SET_BIT(EXTI->FTSR1, PWR_EXTI_LINE_AVM1)

/**
  * @brief  Disable the AVM1 Extended Interrupt Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_AVM1_EXTI_DISABLE_FALLING_EDGE() CLEAR_BIT(EXTI->FTSR1, PWR_EXTI_LINE_AVM1)

/**
  * @brief  Enable the AVM1 Extended Interrupt Rising & Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_AVM1_EXTI_ENABLE_RISING_FALLING_EDGE() \
  do                                                     \
  {                                                      \
    __HAL_PWR_AVM1_EXTI_ENABLE_RISING_EDGE();            \
    __HAL_PWR_AVM1_EXTI_ENABLE_FALLING_EDGE();           \
  } while(0)

/**
  * @brief  Disable the AVM1 Extended Interrupt Rising & Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_AVM1_EXTI_DISABLE_RISING_FALLING_EDGE() \
  do                                                      \
  {                                                       \
    __HAL_PWR_AVM1_EXTI_DISABLE_RISING_EDGE();            \
    __HAL_PWR_AVM1_EXTI_DISABLE_FALLING_EDGE();           \
  } while(0)

/**
  * @brief  Generate a Software Interrupt on AVM1 EXTI Line.
  * @retval None
  */
#define __HAL_PWR_AVM1_EXTI_GENERATE_SWIT() SET_BIT(EXTI->SWIER1, PWR_EXTI_LINE_AVM1)

/**
  * @brief  Check whether the specified AVM1 EXTI flag is set or not.
  * @retval EXTI AVM1 Line Status.
  */
#define __HAL_PWR_AVM1_EXTI_GET_FLAG() ((EXTI->RPR1 | EXTI->FPR1) & PWR_EXTI_LINE_AVM1)

/**
  * @brief  Clear the AVM1 EXTI flag.
  * @retval None.
  */
#define __HAL_PWR_AVM1_EXTI_CLEAR_FLAG()       \
  do                                           \
  {                                            \
    WRITE_REG(EXTI->RPR1, PWR_EXTI_LINE_AVM1); \
    WRITE_REG(EXTI->FPR1, PWR_EXTI_LINE_AVM1); \
  } while(0)

/**
  * @brief  Enable the AVM2 Extended Interrupt Line.
  * @retval None.
  */
#define __HAL_PWR_AVM2_EXTI_ENABLE_IT() SET_BIT(EXTI->IMR1, PWR_EXTI_LINE_AVM2)

/**
  * @brief  Disable the AVM2 Extended Interrupt Line.
  * @retval None.
  */
#define __HAL_PWR_AVM2_EXTI_DISABLE_IT() CLEAR_BIT(EXTI->IMR1, PWR_EXTI_LINE_AVM2)

/**
  * @brief  Enable the AVM2 Event Line.
  * @retval None.
  */
#define __HAL_PWR_AVM2_EXTI_ENABLE_EVENT() SET_BIT(EXTI->EMR1, PWR_EXTI_LINE_AVM2)

/**
  * @brief  Disable the AVM2 Event Line.
  * @retval None.
  */
#define __HAL_PWR_AVM2_EXTI_DISABLE_EVENT() CLEAR_BIT(EXTI->EMR1, PWR_EXTI_LINE_AVM2)

/**
  * @brief  Enable the AVM2 Extended Interrupt Rising Trigger.
  * @retval None.
  */
#define __HAL_PWR_AVM2_EXTI_ENABLE_RISING_EDGE() SET_BIT(EXTI->RTSR1, PWR_EXTI_LINE_AVM2)

/**
  * @brief  Disable the AVM2 Extended Interrupt Rising Trigger.
  * @retval None.
  */
#define __HAL_PWR_AVM2_EXTI_DISABLE_RISING_EDGE() CLEAR_BIT(EXTI->RTSR1, PWR_EXTI_LINE_AVM2)

/**
  * @brief  Enable the AVM2 Extended Interrupt Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_AVM2_EXTI_ENABLE_FALLING_EDGE() SET_BIT(EXTI->FTSR1, PWR_EXTI_LINE_AVM2)

/**
  * @brief  Disable the AVM2 Extended Interrupt Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_AVM2_EXTI_DISABLE_FALLING_EDGE() CLEAR_BIT(EXTI->FTSR1, PWR_EXTI_LINE_AVM2)

/**
  * @brief  Enable the AVM2 Extended Interrupt Rising & Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_AVM2_EXTI_ENABLE_RISING_FALLING_EDGE() \
  do                                                     \
  {                                                      \
    __HAL_PWR_AVM2_EXTI_ENABLE_RISING_EDGE();            \
    __HAL_PWR_AVM2_EXTI_ENABLE_FALLING_EDGE();           \
  } while(0)

/**
  * @brief  Disable the AVM2 Extended Interrupt Rising & Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_AVM2_EXTI_DISABLE_RISING_FALLING_EDGE() \
  do                                                      \
  {                                                       \
    __HAL_PWR_AVM2_EXTI_DISABLE_RISING_EDGE();            \
    __HAL_PWR_AVM2_EXTI_DISABLE_FALLING_EDGE();           \
  } while(0)

/**
  * @brief  Generate a Software Interrupt on AVM2 EXTI Line.
  * @retval None
  */
#define __HAL_PWR_AVM2_EXTI_GENERATE_SWIT() SET_BIT(EXTI->SWIER1, PWR_EXTI_LINE_AVM2)

/**
  * @brief  Check whether the specified AVM2 EXTI flag is set or not.
  * @retval EXTI AVM2 Line Status.
  */
#define __HAL_PWR_AVM2_EXTI_GET_FLAG() ((EXTI->RPR1 | EXTI->FPR1) & PWR_EXTI_LINE_AVM2)

/**
  * @brief  Clear the AVM2 EXTI flag.
  * @retval None.
  */
#define __HAL_PWR_AVM2_EXTI_CLEAR_FLAG()       \
  do                                           \
  {                                            \
    WRITE_REG(EXTI->RPR1, PWR_EXTI_LINE_AVM2); \
    WRITE_REG(EXTI->FPR1, PWR_EXTI_LINE_AVM2); \
  } while(0)

/**
  * @brief Configure the main internal regulator output voltage.
  * @note  This macro is similar to HAL_PWREx_ControlVoltageScaling() API but
  *        doesn't check whether or not VOSREADY flag is set. User may resort
  *        to __HAL_PWR_GET_FLAG() macro to check VOSF bit state.
  * @param  __REGULATOR__ : Specifies the regulator output voltage to achieve a
  *                         tradeoff between performance and power consumption.
  *                         This parameter can be one of the following values :
  *                         @arg @ref PWR_REGULATOR_VOLTAGE_SCALE1 : Regulator voltage output scale 1.
  *                                                                  Provides a typical output voltage at 1.2 V.
  *                                                                  Used when system clock frequency is up to 160 MHz.
  *                         @arg @ref PWR_REGULATOR_VOLTAGE_SCALE2 : Regulator voltage output scale 2.
  *                                                                  Provides a typical output voltage at 1.1 V.
  *                                                                  Used when system clock frequency is up to 100 MHz.
  *                         @arg @ref PWR_REGULATOR_VOLTAGE_SCALE3 : Regulator voltage output scale 3.
  *                                                                  Provides a typical output voltage at 1.0 V.
  *                                                                  Used when system clock frequency is up to 50 MHz.
  *                         @arg @ref PWR_REGULATOR_VOLTAGE_SCALE4 : Regulator voltage output scale 4.
  *                                                                  Provides a typical output voltage at 0.9 V.
  *                                                                  Used when system clock frequency is up to 24 MHz.
  * @retval None.
  */
#define __HAL_PWR_VOLTAGESCALING_CONFIG(__REGULATOR__)    \
  do                                                      \
  {                                                       \
    __IO uint32_t tmpreg;                                 \
    MODIFY_REG(PWR->VOSR, PWR_VOSR_VOS, (__REGULATOR__)); \
    tmpreg = READ_BIT(PWR->VOSR, PWR_VOSR_VOS);           \
    UNUSED(tmpreg);                                       \
  } while(0)
/**
  * @}
  */

/* Private constants ---------------------------------------------------------*/

/** @addtogroup PWREx_Private_Constants PWR Extended Private Constants
  * @{
  */
/* PVM extended interrupts and event lines defines */
#define PWR_EXTI_LINE_UVM   (0x00080000UL) /* UVM EXTI Line   */
#define PWR_EXTI_LINE_IO2VM (0x00100000UL) /* IO2VM EXTI Line */
#define PWR_EXTI_LINE_AVM1  (0x00200000UL) /* AVM1 EXTI Line  */
#define PWR_EXTI_LINE_AVM2  (0x00400000UL) /* AVM2 EXTI Line  */

/* SRAM retention IDs */
#define SRAM_ID_MASK  (0xFFFFUL << 16U)
#define SRAM1_ID      (0x01UL   << 16U)
#define SRAM2_ID      (0x01UL   << 17U)
#if defined (PWR_CR2_SRAM3PDS1)
#define SRAM3_ID      (0x01UL   << 18U)
#endif /* PWR_CR2_SRAM3PDS1 */
#define SRAM4_ID      (0x01UL   << 19U)
#define ICACHERAM_ID  (0x01UL   << 20U)
#define DCACHE1RAM_ID (0x01UL   << 21U)
#if defined (PWR_CR2_DMA2DRAMPDS)
#define DMA2DRAM_ID   (0x01UL   << 22U)
#endif /* PWR_CR2_DMA2DRAMPDS */
#define PERIPHRAM_ID  (0x01UL   << 23U)
#define PKARAM_ID     (0x01UL   << 24U)
#if defined (PWR_CR2_DC2RAMPDS)
#define DCACHE2RAM_ID (0x01UL   << 25U)
#endif /* defined (PWR_CR2_DC2RAMPDS) */
#if defined (PWR_CR2_GPRAMPDS)
#define GRAPHIPRAM_ID (0x01UL   << 26U)
#endif /* defined (PWR_CR2_GPRAMPDS) */
#if defined (PWR_CR2_DSIRAMPDS)
#define DSIRAM_ID     (0x01UL   << 27U)
#endif /* defined (PWR_CR2_DSIRAMPDS) */
#if defined (PWR_CR4_SRAM5PDS1)
#define SRAM5_ID      (0x01UL   << 28U)
#endif /* defined (PWR_CR4_SRAM5PDS1) */
#if defined (PWR_CR5_SRAM6PDS1)
#define SRAM6_ID      (0x01UL   << 29U)
#endif /* defined (PWR_CR5_SRAM6PDS1) */
#if defined (PWR_CR2_JPEGRAMPDS)
#define JPEGRAM_ID    (0x01UL   << 30U)
#endif /* defined (PWR_CR2_JPEGRAMPDS)*/

/* SRAM page retention IDs */
#define PAGE01_ID (0x01UL << 0U)
#define PAGE02_ID (0x01UL << 1U)
#define PAGE03_ID (0x01UL << 2U)
#define PAGE04_ID (0x01UL << 3U)
#define PAGE05_ID (0x01UL << 4U)
#define PAGE06_ID (0x01UL << 5U)
#define PAGE07_ID (0x01UL << 6U)
#define PAGE08_ID (0x01UL << 7U)
#define PAGE09_ID (0x01UL << 8U)
#define PAGE10_ID (0x01UL << 9U)
#define PAGE11_ID (0x01UL << 10U)
#define PAGE12_ID (0x01UL << 11U)
#define PAGE13_ID (0x01UL << 12U)

/* All available RAM retention in Run mode define */
#if defined (PWR_CR1_SRAM6PD)
#define PWR_ALL_RAM_RUN_MASK (PWR_SRAM1_FULL_RUN | PWR_SRAM2_FULL_RUN | \
                              PWR_SRAM3_FULL_RUN | PWR_SRAM4_FULL_RUN | \
                              PWR_SRAM5_FULL_RUN | PWR_SRAM6_FULL_RUN)
#elif defined (PWR_CR1_SRAM5PD)
#define PWR_ALL_RAM_RUN_MASK (PWR_SRAM1_FULL_RUN | PWR_SRAM2_FULL_RUN | \
                              PWR_SRAM3_FULL_RUN | PWR_SRAM4_FULL_RUN | \
                              PWR_SRAM5_FULL_RUN)
#elif defined (PWR_CR2_SRAM3PDS1)
#define PWR_ALL_RAM_RUN_MASK (PWR_SRAM1_FULL_RUN | PWR_SRAM2_FULL_RUN | \
                              PWR_SRAM3_FULL_RUN | PWR_SRAM4_FULL_RUN)
#else
#define PWR_ALL_RAM_RUN_MASK (PWR_SRAM1_FULL_RUN | PWR_SRAM2_FULL_RUN | \
                              PWR_SRAM4_FULL_RUN)
#endif /* defined (PWR_CR1_SRAM5PD) */
/**
  * @}
  */

/* Private macros --------------------------------------------------------*/

/** @addtogroup PWREx_Private_Macros PWR Extended Private Macros
  * @{
  */
/* PVM type check macro */
#define IS_PWR_PVM_TYPE(TYPE)   \
  (((TYPE) == PWR_UVM )       ||\
   ((TYPE) == PWR_IO2VM)      ||\
   ((TYPE) == PWR_AVM1)       ||\
   ((TYPE) == PWR_AVM2))

/* PVM mode check macro */
#define IS_PWR_PVM_MODE(MODE)                   \
  (((MODE) == PWR_PVM_MODE_NORMAL)            ||\
   ((MODE) == PWR_PVM_MODE_IT_RISING)         ||\
   ((MODE) == PWR_PVM_MODE_IT_FALLING)        ||\
   ((MODE) == PWR_PVM_MODE_IT_RISING_FALLING) ||\
   ((MODE) == PWR_PVM_MODE_EVENT_RISING)      ||\
   ((MODE) == PWR_PVM_MODE_EVENT_FALLING)     ||\
   ((MODE) == PWR_PVM_MODE_EVENT_RISING_FALLING))

/* SRD state check macro */
#define IS_PWR_SRD_STATE(SRD_STATE)       \
  (((SRD_STATE) == PWR_SRD_DOMAIN_STOP) ||\
   ((SRD_STATE) == PWR_SRD_DOMAIN_RUN))

/* Supply selection check macro */
#define IS_PWR_SUPPLY(PWR_SOURCE)     \
  (((PWR_SOURCE) == PWR_LDO_SUPPLY) ||\
   ((PWR_SOURCE) == PWR_SMPS_SUPPLY))

/* Voltage scaling range check macro */
#define IS_PWR_VOLTAGE_SCALING_RANGE(RANGE)    \
  (((RANGE) == PWR_REGULATOR_VOLTAGE_SCALE1) ||\
   ((RANGE) == PWR_REGULATOR_VOLTAGE_SCALE2) ||\
   ((RANGE) == PWR_REGULATOR_VOLTAGE_SCALE3) ||\
   ((RANGE) == PWR_REGULATOR_VOLTAGE_SCALE4))

/* Battery charging resistor selection check macro */
#define IS_PWR_BATTERY_RESISTOR_SELECT(RESISTOR)     \
  (((RESISTOR) == PWR_BATTERY_CHARGING_RESISTOR_5) ||\
   ((RESISTOR) == PWR_BATTERY_CHARGING_RESISTOR_1_5))

/* GPIO port check macro */
#if defined (PWR_PUCRJ_PU0)
#define IS_PWR_GPIO_PORT(GPIO_PORT) \
  (((GPIO_PORT) == PWR_GPIO_A)      ||\
   ((GPIO_PORT) == PWR_GPIO_B)      ||\
   ((GPIO_PORT) == PWR_GPIO_C)      ||\
   ((GPIO_PORT) == PWR_GPIO_D)      ||\
   ((GPIO_PORT) == PWR_GPIO_E)      ||\
   ((GPIO_PORT) == PWR_GPIO_F)      ||\
   ((GPIO_PORT) == PWR_GPIO_G)      ||\
   ((GPIO_PORT) == PWR_GPIO_H)      ||\
   ((GPIO_PORT) == PWR_GPIO_I)      ||\
   ((GPIO_PORT) == PWR_GPIO_J))
#elif defined (PWR_PUCRF_PU0) && defined (PWR_PUCRI_PU0)
#define IS_PWR_GPIO_PORT(GPIO_PORT) \
  (((GPIO_PORT) == PWR_GPIO_A)      ||\
   ((GPIO_PORT) == PWR_GPIO_B)      ||\
   ((GPIO_PORT) == PWR_GPIO_C)      ||\
   ((GPIO_PORT) == PWR_GPIO_D)      ||\
   ((GPIO_PORT) == PWR_GPIO_E)      ||\
   ((GPIO_PORT) == PWR_GPIO_F)      ||\
   ((GPIO_PORT) == PWR_GPIO_G)      ||\
   ((GPIO_PORT) == PWR_GPIO_H)      ||\
   ((GPIO_PORT) == PWR_GPIO_I))
#else
#define IS_PWR_GPIO_PORT(GPIO_PORT) \
  (((GPIO_PORT) == PWR_GPIO_A)      ||\
   ((GPIO_PORT) == PWR_GPIO_B)      ||\
   ((GPIO_PORT) == PWR_GPIO_C)      ||\
   ((GPIO_PORT) == PWR_GPIO_D)      ||\
   ((GPIO_PORT) == PWR_GPIO_E)      ||\
   ((GPIO_PORT) == PWR_GPIO_G)      ||\
   ((GPIO_PORT) == PWR_GPIO_H))
#endif /* defined (PWR_PUCRJ_PU0) */

/* GPIO pin mask check macro */
#define IS_PWR_GPIO_PIN_MASK(BIT_MASK) \
  ((((BIT_MASK) & GPIO_PIN_MASK) != 0U) && ((BIT_MASK) <= GPIO_PIN_MASK))

/* SRAM2 retention in Standby mode check macro */
#define IS_PWR_SRAM2_STANDBY_RETENTION(CONTENT)       \
  (((CONTENT) == PWR_SRAM2_PAGE1_STANDBY) ||\
   ((CONTENT) == PWR_SRAM2_PAGE2_STANDBY) ||\
   ((CONTENT) == PWR_SRAM2_FULL_STANDBY))

/* RAMs retention in Stop mode check macros */
#define IS_PWR_SRAM1_STOP_RETENTION(RAMCONTENT) \
  ((((RAMCONTENT) & (~PWR_SRAM1_FULL_STOP)) == 0U) && ((RAMCONTENT) != 0U))

#define IS_PWR_SRAM2_STOP_RETENTION(RAMCONTENT) \
  ((((RAMCONTENT) & (~PWR_SRAM2_FULL_STOP)) == 0U) && ((RAMCONTENT) != 0U))

#if defined (PWR_CR2_SRAM3PDS1)
#define IS_PWR_SRAM3_STOP_RETENTION(RAMCONTENT) \
  ((((RAMCONTENT) & (~PWR_SRAM3_FULL_STOP)) == 0U) && ((RAMCONTENT) != 0U))
#endif /* PWR_CR2_SRAM3PDS1 */

#define IS_PWR_SRAM4_STOP_RETENTION(RAMCONTENT) \
  ((((RAMCONTENT) & (~PWR_SRAM4_FULL_STOP)) == 0U) && ((RAMCONTENT) != 0U))

#if defined (PWR_CR4_SRAM5PDS1)
#define IS_PWR_SRAM5_STOP_RETENTION(RAMCONTENT) \
  ((((RAMCONTENT) & (~PWR_SRAM5_FULL_STOP)) == 0U) && ((RAMCONTENT) != 0U))
#endif /* defined (PWR_CR4_SRAM5PDS1) */

#if defined (PWR_CR5_SRAM6PDS1)
#define IS_PWR_SRAM6_STOP_RETENTION(RAMCONTENT) \
  ((((RAMCONTENT) & (~PWR_SRAM6_FULL_STOP)) == 0U) && ((RAMCONTENT) != 0U))
#endif /* defined (PWR_CR5_SRAM6PDS1) */

#define IS_PWR_ICACHE_STOP_RETENTION(RAMCONTENT) \
  ((((RAMCONTENT) & (~PWR_ICACHE_FULL_STOP)) == 0U) && ((RAMCONTENT) != 0U))

#define IS_PWR_DCACHE1_STOP_RETENTION(RAMCONTENT) \
  ((((RAMCONTENT) & (~PWR_DCACHE1_FULL_STOP)) == 0U) && ((RAMCONTENT) != 0U))

#if defined (PWR_CR2_DC2RAMPDS)
#define IS_PWR_DCACHE2_STOP_RETENTION(RAMCONTENT) \
  ((((RAMCONTENT) & (~PWR_DCACHE2_FULL_STOP)) == 0U) && ((RAMCONTENT) != 0U))
#endif /* defined (PWR_CR2_DC2RAMPDS) */

#if defined (PWR_CR2_DMA2DRAMPDS)
#define IS_PWR_DMA2DRAM_STOP_RETENTION(RAMCONTENT) \
  ((((RAMCONTENT) & (~PWR_DMA2DRAM_FULL_STOP)) == 0U) && ((RAMCONTENT) != 0U))
#endif /* PWR_CR2_DMA2DRAMPDS */

#define IS_PWR_PERIPHRAM_STOP_RETENTION(RAMCONTENT) \
  ((((RAMCONTENT) & (~PWR_PERIPHRAM_FULL_STOP)) == 0U) && ((RAMCONTENT) != 0U))

#define IS_PWR_PKA32RAM_STOP_RETENTION(RAMCONTENT) \
  ((((RAMCONTENT) & (~PWR_PKA32RAM_FULL_STOP)) == 0U) && ((RAMCONTENT) != 0U))

#if defined (PWR_CR2_GPRAMPDS)
#define IS_PWR_GRAPHICPRAM_STOP_RETENTION(RAMCONTENT) \
  ((((RAMCONTENT) & (~PWR_GRAPHICPRAM_FULL_STOP)) == 0U) && ((RAMCONTENT) != 0U))
#endif /* defined (PWR_CR2_GPRAMPDS) */

#if defined (PWR_CR2_DSIRAMPDS)
#define IS_PWR_DSIRAM_STOP_RETENTION(RAMCONTENT) \
  ((((RAMCONTENT) & (~PWR_DSIRAM_FULL_STOP)) == 0U) && ((RAMCONTENT) != 0U))
#endif /* defined (PWR_CR2_DSIRAMPDS) */

#if defined (PWR_CR2_JPEGRAMPDS)
#define IS_PWR_JPEGRAM_STOP_RETENTION(RAMCONTENT) \
  ((((RAMCONTENT) & (~PWR_JPEGRAM_FULL_STOP)) == 0U) && ((RAMCONTENT) != 0U))
#endif /* defined (PWR_CR2_DSIRAMPDS) */

/* RAMs retention in Run mode check macro */
#define IS_PWR_RAM_RUN_RETENTION(RAMCONTENT) \
  ((((RAMCONTENT) & (~PWR_ALL_RAM_RUN_MASK)) == 0U) && ((RAMCONTENT) != 0U))
/**
  * @}
  */

/** @addtogroup PWREx_Exported_Functions PWR Extended Exported Functions
  * @{
  */

/** @addtogroup PWREx_Exported_Functions_Group1 Power Supply Control Functions
  * @{
  */
HAL_StatusTypeDef HAL_PWREx_ControlVoltageScaling(uint32_t VoltageScaling);
uint32_t          HAL_PWREx_GetVoltageRange(void);
HAL_StatusTypeDef HAL_PWREx_ConfigSupply(uint32_t SupplySource);
uint32_t          HAL_PWREx_GetSupplyConfig(void);
void              HAL_PWREx_EnableFastSoftStart(void);
void              HAL_PWREx_DisableFastSoftStart(void);
/**
  * @}
  */

/** @addtogroup PWREx_Exported_Functions_Group2 Low Power Control Functions
  * @{
  */
void HAL_PWREx_EnterSTOP1Mode(uint8_t STOPEntry);
void HAL_PWREx_EnterSTOP2Mode(uint8_t STOPEntry);
void HAL_PWREx_EnterSTOP3Mode(uint8_t STOPEntry);
void HAL_PWREx_EnterSHUTDOWNMode(void);
void HAL_PWREx_ConfigSRDDomain(uint32_t SRDState);
void HAL_PWREx_EnableUltraLowPowerMode(void);
void HAL_PWREx_DisableUltraLowPowerMode(void);
void HAL_PWREx_S3WU_IRQHandler(uint32_t WakeUpPin);
void HAL_PWREx_S3WUCallback(uint32_t WakeUpPin);
/**
  * @}
  */

/** @addtogroup PWREx_Exported_Functions_Group3 Voltage Monitoring Functions
  * @{
  */
void              HAL_PWREx_EnableBatteryCharging(uint32_t ResistorValue);
void              HAL_PWREx_DisableBatteryCharging(void);
void              HAL_PWREx_EnableVddUSB(void);
void              HAL_PWREx_DisableVddUSB(void);
void              HAL_PWREx_EnableVddIO2(void);
void              HAL_PWREx_DisableVddIO2(void);
void              HAL_PWREx_EnableVddA(void);
void              HAL_PWREx_DisableVddA(void);
void              HAL_PWREx_EnableUVM(void);
void              HAL_PWREx_DisableUVM(void);
void              HAL_PWREx_EnableIO2VM(void);
void              HAL_PWREx_DisableIO2VM(void);
void              HAL_PWREx_EnableAVM1(void);
void              HAL_PWREx_DisableAVM1(void);
void              HAL_PWREx_EnableAVM2(void);
void              HAL_PWREx_DisableAVM2(void);
#if defined (PWR_VOSR_USBPWREN)
HAL_StatusTypeDef HAL_PWREx_EnableUSBHSTranceiverSupply(void);
void              HAL_PWREx_DisableUSBHSTranceiverSupply(void);
#endif /* defined (PWR_VOSR_USBPWREN) */
#if defined (PWR_CR1_FORCE_USBPWR)
void              HAL_PWREx_EnableOTGHSPHYLowPowerRetention(void);
void              HAL_PWREx_DisableOTGHSPHYLowPowerRetention(void);
#endif /* defined (PWR_CR1_FORCE_USBPWR) */
#if defined (PWR_VOSR_VDD11USBDIS)
void              HAL_PWREx_EnableVDD11USB(void);
void              HAL_PWREx_DisableVDD11USB(void);
#endif /* defined (PWR_VOSR_VDD11USBDIS) */
HAL_StatusTypeDef HAL_PWREx_ConfigPVM(PWR_PVMTypeDef *pConfigPVM);
void              HAL_PWREx_EnableMonitoring(void);
void              HAL_PWREx_DisableMonitoring(void);
void              HAL_PWREx_EnableUCPDStandbyMode(void);
void              HAL_PWREx_DisableUCPDStandbyMode(void);
void              HAL_PWREx_EnableUCPDDeadBattery(void);
void              HAL_PWREx_DisableUCPDDeadBattery(void);
void              HAL_PWREx_PVD_PVM_IRQHandler(void);
void              HAL_PWREx_UVMCallback(void);
void              HAL_PWREx_IO2VMCallback(void);
void              HAL_PWREx_AVM1Callback(void);
void              HAL_PWREx_AVM2Callback(void);
/**
  * @}
  */

/** @addtogroup PWREx_Exported_Functions_Group4 Memories Retention Functions
  * @{
  */
void              HAL_PWREx_EnableSRAM2ContentStandbyRetention(uint32_t SRAM2Pages);
void              HAL_PWREx_DisableSRAM2ContentStandbyRetention(uint32_t SRAM2Pages);
void              HAL_PWREx_EnableRAMsContentStopRetention(uint32_t RAMSelection);
void              HAL_PWREx_DisableRAMsContentStopRetention(uint32_t RAMSelection);
void              HAL_PWREx_EnableRAMsContentRunRetention(uint32_t RAMSelection);
void              HAL_PWREx_DisableRAMsContentRunRetention(uint32_t RAMSelection);
void              HAL_PWREx_EnableFlashFastWakeUp(void);
void              HAL_PWREx_DisableFlashFastWakeUp(void);
void              HAL_PWREx_EnableSRAM4FastWakeUp(void);
void              HAL_PWREx_DisableSRAM4FastWakeUp(void);
HAL_StatusTypeDef HAL_PWREx_EnableBkupRAMRetention(void);
void              HAL_PWREx_DisableBkupRAMRetention(void);
/**
  * @}
  */

/** @addtogroup PWREx_Exported_Functions_Group5 I/O Pull-Up Pull-Down Configuration Functions
  * @{
  */
void              HAL_PWREx_EnablePullUpPullDownConfig(void);
void              HAL_PWREx_DisablePullUpPullDownConfig(void);
HAL_StatusTypeDef HAL_PWREx_EnableGPIOPullUp(uint32_t GPIO_Port, uint32_t GPIO_Pin);
HAL_StatusTypeDef HAL_PWREx_DisableGPIOPullUp(uint32_t GPIO_Port, uint32_t GPIO_Pin);
HAL_StatusTypeDef HAL_PWREx_EnableGPIOPullDown(uint32_t GPIO_Port, uint32_t GPIO_Pin);
HAL_StatusTypeDef HAL_PWREx_DisableGPIOPullDown(uint32_t GPIO_Port, uint32_t GPIO_Pin);
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
#endif /* __cplusplus */


#endif /* STM32U5xx_HAL_PWR_EX_H */
