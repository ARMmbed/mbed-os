/**
  ******************************************************************************
  * @file    stm32u5xx_hal_gpio_ex.h
  * @author  MCD Application Team
  * @brief   Header file of GPIO HAL Extended module.
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
#ifndef STM32U5xx_HAL_GPIO_EX_H
#define STM32U5xx_HAL_GPIO_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal_def.h"

/** @addtogroup STM32U5xx_HAL_Driver
  * @{
  */

/** @defgroup GPIOEx GPIOEx
  * @brief GPIO Extended HAL module driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup LPGPIO_MapTypeDef GPIO/Ex Exported Types
  * @{
  */
typedef struct
{
  GPIO_TypeDef *GPIO_PORT;
  uint32_t Pin_Pos;
} LPGPIO_MapTypeDef;
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup GPIOEx_Exported_Constants GPIOEx Exported Constants
  * @{
  */

/** @defgroup GPIOEx_Alternate_function_selection GPIOEx Alternate function selection
  * @{
  */

#if (defined(STM32U575xx) || defined(STM32U585xx))
/*--------------STM32U575xx/STM32U585xx---------------------------*/
/**
  * @brief   AF 0 selection
  */
#define GPIO_AF0_RTC_50Hz      ((uint8_t)0x00)  /* RTC_50Hz Alternate Function mapping                       */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /* MCO (MCO1 and MCO2) Alternate Function mapping            */
#define GPIO_AF0_SWJ           ((uint8_t)0x00)  /* SWJ (SWD and JTAG) Alternate Function mapping             */
#define GPIO_AF0_TRACE         ((uint8_t)0x00)  /* TRACE Alternate Function mapping                          */
#define GPIO_AF0_LPTIM1        ((uint8_t)0x00)  /* LPTIM1 Alternate Function mapping                         */
#define GPIO_AF0_CSLEEP        ((uint8_t)0x00)  /* CSLEEP Alternate Function mapping                         */
#define GPIO_AF0_CSTOP         ((uint8_t)0x00)  /* CSTOP Alternate Function mapping                          */
#define GPIO_AF0_SRDSTOP       ((uint8_t)0x00)  /* SRDSTOP Alternate Function mapping                        */
#define GPIO_AF0_CRS           ((uint8_t)0x00)  /* CRS Alternate Function mapping                            */

/**
  * @brief   AF 1 selection
  */
#define GPIO_AF1_TIM1          ((uint8_t)0x01)  /* TIM1 Alternate Function mapping */
#define GPIO_AF1_TIM2          ((uint8_t)0x01)  /* TIM2 Alternate Function mapping */
#define GPIO_AF1_TIM5          ((uint8_t)0x01)  /* TIM5 Alternate Function mapping */
#define GPIO_AF1_TIM8          ((uint8_t)0x01)  /* TIM8 Alternate Function mapping */
#define GPIO_AF1_LPTIM1        ((uint8_t)0x01)  /* LPTIM1 Alternate Function mapping */
#define GPIO_AF1_IR            ((uint8_t)0x01)  /* IR Alternate Function mapping */

/**
  * @brief   AF 2 selection
  */
#define GPIO_AF2_TIM1          ((uint8_t)0x02)  /* TIM1 Alternate Function mapping */
#define GPIO_AF2_TIM2          ((uint8_t)0x02)  /* TIM2 Alternate Function mapping */
#define GPIO_AF2_TIM3          ((uint8_t)0x02)  /* TIM3 Alternate Function mapping */
#define GPIO_AF2_TIM4          ((uint8_t)0x02)  /* TIM4 Alternate Function mapping */
#define GPIO_AF2_TIM5          ((uint8_t)0x02)  /* TIM5 Alternate Function mapping */
#define GPIO_AF2_LPTIM2        ((uint8_t)0x02)  /* LPTIM2 Alternate Function mapping */
#define GPIO_AF2_LPTIM3        ((uint8_t)0x02)  /* LPTIM3 Alternate Function mapping */

/**
  * @brief   AF 3 selection
  */
#define GPIO_AF3_I2C4          ((uint8_t)0x03)  /* I2C4 Alternate Function mapping  */
#define GPIO_AF3_OCTOSPI1      ((uint8_t)0x03)  /* OCTOSPI1 Alternate Function mapping  */
#define GPIO_AF3_SAI1          ((uint8_t)0x03)  /* SAI1 Alternate Function mapping  */
#define GPIO_AF3_SPI2          ((uint8_t)0x03)  /* SPI2 Alternate Function mapping  */
#define GPIO_AF3_TIM8          ((uint8_t)0x03)  /* TIM8 Alternate Function mapping  */
#define GPIO_AF3_TIM8_COMP1    ((uint8_t)0x03)  /* TIM8/COMP1 Break in Alternate Function mapping  */
#define GPIO_AF3_TIM8_COMP2    ((uint8_t)0x03)  /* TIM8/COMP2 Break in Alternate Function mapping  */
#define GPIO_AF3_TIM1          ((uint8_t)0x03)  /* TIM1 Alternate Function mapping  */
#define GPIO_AF3_TIM1_COMP1    ((uint8_t)0x03)  /* TIM1/COMP1 Break in Alternate Function mapping  */
#define GPIO_AF3_TIM1_COMP2    ((uint8_t)0x03)  /* TIM1/COMP2 Break in Alternate Function mapping  */
#define GPIO_AF3_USART2        ((uint8_t)0x03)  /* USART2 Alternate Function mapping  */
#define GPIO_AF3_ADF1          ((uint8_t)0x03)  /* ADF1 Alternate Function mapping  */

/**
  * @brief   AF 4 selection
  */
#define GPIO_AF4_I2C1          ((uint8_t)0x04)  /* I2C1 Alternate Function mapping   */
#define GPIO_AF4_I2C2          ((uint8_t)0x04)  /* I2C2 Alternate Function mapping   */
#define GPIO_AF4_I2C3          ((uint8_t)0x04)  /* I2C3 Alternate Function mapping   */
#define GPIO_AF4_I2C4          ((uint8_t)0x04)  /* I2C4 Alternate Function mapping   */
#define GPIO_AF4_USART1        ((uint8_t)0x04)  /* USART1 Alternate Function mapping   */
#define GPIO_AF4_PSSI          ((uint8_t)0x04)  /* PSSI Alternate Function mapping   */
#define GPIO_AF4_DCMI          ((uint8_t)0x04)  /* DCMI Alternate Function mapping   */
#define GPIO_AF4_LPTIM3        ((uint8_t)0x04)  /* LPTIM3 Alternate Function mapping   */

/**
  * @brief   AF 5 selection
  */
#define GPIO_AF5_DFSDM1        ((uint8_t)0x05)  /* DFSDM1 Alternate Function mapping   */
#define GPIO_AF5_I2C4          ((uint8_t)0x05)  /* I2C4 Alternate Function mapping   */
#define GPIO_AF5_PSSI          ((uint8_t)0x05)  /* PSSI Alternate Function mapping   */
#define GPIO_AF5_OCTOSPI1      ((uint8_t)0x05)  /* OCTOSPI1 Alternate Function mapping  */
#define GPIO_AF5_OCTOSPI2      ((uint8_t)0x05)  /* OCTOSPI2 Alternate Function mapping  */
#define GPIO_AF5_SPI1          ((uint8_t)0x05)  /* SPI1 Alternate Function mapping   */
#define GPIO_AF5_SPI2          ((uint8_t)0x05)  /* SPI2 Alternate Function mapping   */
#define GPIO_AF5_SPI3          ((uint8_t)0x05)  /* SPI3 Alternate Function mapping   */
#define GPIO_AF5_DCMI          ((uint8_t)0x05)  /* DCMI Alternate Function mapping   */
#define GPIO_AF5_MDF1          ((uint8_t)0x05)  /* MDF1 Alternate Function mapping   */

/**
  * @brief   AF 6 selection
  */
#define GPIO_AF6_OCTOSPI1      ((uint8_t)0x06)  /* OCTOSPI1 Alternate Function mapping  */
#define GPIO_AF6_OCTOSPI2      ((uint8_t)0x06)  /* OCTOSPI2 Alternate Function mapping  */
#define GPIO_AF6_MDF1          ((uint8_t)0x06)  /* MDF1 Alternate Function mapping  */
#define GPIO_AF6_SPI3          ((uint8_t)0x06)  /* SPI3 Alternate Function mapping  */

/**
  * @brief   AF 7 selection
  */
#define GPIO_AF7_USART1        ((uint8_t)0x07)  /* USART1 Alternate Function mapping */
#define GPIO_AF7_USART2        ((uint8_t)0x07)  /* USART2 Alternate Function mapping */
#define GPIO_AF7_USART3        ((uint8_t)0x07)  /* USART3 Alternate Function mapping */

/**
  * @brief   AF 8 selection
  */
#define GPIO_AF8_LPUART1       ((uint8_t)0x08)  /* LPUART1 Alternate Function mapping */
#define GPIO_AF8_UART4         ((uint8_t)0x08)  /* UART4 Alternate Function mapping   */
#define GPIO_AF8_UART5         ((uint8_t)0x08)  /* UART5 Alternate Function mapping   */
#define GPIO_AF8_SDMMC1        ((uint8_t)0x08)  /* SDMMC1 Alternate Function mapping  */

/**
  * @brief   AF 9 selection
  */
#define GPIO_AF9_FDCAN1        ((uint8_t)0x09)  /* FDCAN1 Alternate Function mapping */
#define GPIO_AF9_TSC           ((uint8_t)0x09)  /* TSC Alternate Function mapping   */

/**
  * @brief   AF 10 selection
  */
#define GPIO_AF10_DCMI         ((uint8_t)0x0A)  /* DCMI Alternate Function mapping */
#define GPIO_AF10_PSSI         ((uint8_t)0x0A)  /* PSSI Alternate Function mapping */
#define GPIO_AF10_USB          ((uint8_t)0x0A)  /* USB Alternate Function mapping */
#define GPIO_AF10_OCTOSPI1     ((uint8_t)0x0A)  /* OCTOSPI1 Alternate Function mapping */
#define GPIO_AF10_OCTOSPI2     ((uint8_t)0x0A)  /* OCTOSPI2 Alternate Function mapping */
#define GPIO_AF10_CRS          ((uint8_t)0x0A)  /* CRS Alternate Function mapping */

/**
  * @brief   AF 11 selection
  */
#define GPIO_AF11_UCPD1         ((uint8_t)0x0B)    /*!< UCPD1 Alternate Function mapping  */
#define GPIO_AF11_SDMMC2        ((uint8_t)0x0B)    /*!< SDMMC2 Alternate Function mapping */
#define GPIO_AF11_LPGPIO        ((uint8_t)0x0B)    /*!< LPGPIO Alternate Function mapping */
#define GPIO_AF11_FMC_NBL1      ((uint8_t)0x0B)    /*!< FMC_NBL1 Alternate Function mapping */

/**
  * @brief   AF 12 selection
  */
#define GPIO_AF12_COMP1        ((uint8_t)0x0C)  /* COMP1 Alternate Function mapping   */
#define GPIO_AF12_COMP2        ((uint8_t)0x0C)  /* COMP2 Alternate Function mapping   */
#define GPIO_AF12_FMC          ((uint8_t)0x0C)  /* FMC Alternate Function mapping     */
#define GPIO_AF12_TIM1_COMP1   ((uint8_t)0x0C)  /* TIM1/COMP1 Break in Alternate Function mapping  */
#define GPIO_AF12_TIM1_COMP2   ((uint8_t)0x0C)  /* TIM1/COMP2 Break in Alternate Function mapping  */
#define GPIO_AF12_TIM8_COMP2   ((uint8_t)0x0C)  /* TIM8/COMP2 Break in Alternate Function mapping  */
#define GPIO_AF12_SDMMC1       ((uint8_t)0x0C)  /* SDMMC1 Alternate Function mapping  */
#define GPIO_AF12_SDMMC2       ((uint8_t)0x0C)  /* SDMMC2 Alternate Function mapping  */

/**
  * @brief   AF 13 selection
  */
#define GPIO_AF13_SAI1         ((uint8_t)0x0D)  /* SAI1 Alternate Function mapping */
#define GPIO_AF13_SAI2         ((uint8_t)0x0D)  /* SAI2 Alternate Function mapping */
#define GPIO_AF13_TIM8_COMP1   ((uint8_t)0x0D)  /* TIM8/COMP1 Break in Alternate Function mapping  */
#define GPIO_AF13_LPTIM4       ((uint8_t)0x0D)  /* LPTIM4 Alternate Function mapping  */
#define GPIO_AF13_LPTIM2       ((uint8_t)0x0D)  /* LPTIM2 Alternate Function mapping  */

/**
  * @brief   AF 14 selection
  */
#define GPIO_AF14_LPTIM2       ((uint8_t)0x0E)  /* LPTIM2 Alternate Function mapping */
#define GPIO_AF14_LPTIM3       ((uint8_t)0x0E)  /* LPTIM3 Alternate Function mapping */
#define GPIO_AF14_TIM2         ((uint8_t)0x0E)  /* TIM2 Alternate Function mapping */
#define GPIO_AF14_TIM8_COMP2   ((uint8_t)0x0E)  /* TIM8/COMP2 Break in Alternate Function mapping  */
#define GPIO_AF14_TIM15        ((uint8_t)0x0E)  /* TIM15 Alternate Function mapping */
#define GPIO_AF14_TIM15_COMP1  ((uint8_t)0x0E)  /* TIM15/COMP1 Alternate Function mapping */
#define GPIO_AF14_TIM16        ((uint8_t)0x0E)  /* TIM16 Alternate Function mapping */
#define GPIO_AF14_TIM16_COMP1  ((uint8_t)0x0E)  /* TIM16/COMP1 Alternate Function mapping */
#define GPIO_AF14_TIM17        ((uint8_t)0x0E)  /* TIM17 Alternate Function mapping */
#define GPIO_AF14_TIM17_COMP1  ((uint8_t)0x0E)  /* TIM17/COMP1 Alternate Function mapping */
#define GPIO_AF14_SDMMC2       ((uint8_t)0x0E)  /* SDMMC2 Alternate Function mapping  */


/**
  * @brief   AF 15 selection
  */
#define GPIO_AF15_EVENTOUT     ((uint8_t)0x0F)  /* EVENTOUT Alternate Function mapping */

#define IS_GPIO_AF(AF)         ((AF) <= (uint8_t)0x0F)

#endif /* (defined(STM32U575xx) || defined(STM32U585xx)) */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup GPIOEx_Exported_Macros GPIOEx Exported Macros
  * @{
  */

/** @defgroup GPIOEx_Get_Port_Index GPIOEx Get Port Index
  * @{
  */
#if (defined(STM32U575xx) || defined(STM32U585xx))

#define GPIO_GET_INDEX(__GPIOx__)           (((uint32_t )(__GPIOx__) & (~GPIOA_BASE)) >> 10)

#endif /* (defined(STM32U575xx) || defined(STM32U585xx)) */
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32U5xx_HAL_GPIO_EX_H */
