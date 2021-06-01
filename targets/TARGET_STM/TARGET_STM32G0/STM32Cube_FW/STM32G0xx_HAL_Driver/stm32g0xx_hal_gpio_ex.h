/**
  ******************************************************************************
  * @file    stm32g0xx_hal_gpio_ex.h
  * @author  MCD Application Team
  * @brief   Header file of GPIO HAL Extended module.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32G0xx_HAL_GPIO_EX_H
#define STM32G0xx_HAL_GPIO_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal_def.h"

/** @addtogroup STM32G0xx_HAL_Driver
  * @{
  */

/** @defgroup GPIOEx GPIOEx
  * @brief GPIO Extended HAL module driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup GPIOEx_Exported_Constants GPIOEx Exported Constants
  * @{
  */

/** @defgroup GPIOEx_Alternate_function_selection GPIOEx Alternate function selection
  * @{
  */
#if defined (STM32G0C1xx) || defined (STM32G0B1xx)
/*------------------------- STM32G0C1xx / STM32G0B1xx ------------------------*/
/**
  * @brief   AF 0 selection
  */
#define GPIO_AF0_CEC           ((uint8_t)0x00)  /*!< CEC Alternate Function mapping */
#define GPIO_AF0_CRS           ((uint8_t)0x00)  /*!< CRS Alternate Function mapping */
#define GPIO_AF0_EVENTOUT      ((uint8_t)0x00)  /*!< EVENTOUT Alternate Function mapping */
#define GPIO_AF0_IR            ((uint8_t)0x00)  /*!< IR Alternate Function mapping */
#define GPIO_AF0_LPTIM1        ((uint8_t)0x00)  /*!< LPTIM1 Alternate Function mapping */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /*!< MCO (MCO1) Alternate Function mapping */
#define GPIO_AF0_OSC           ((uint8_t)0x00)  /*!< OSC (By pass and Enable) Alternate Function mapping */
#define GPIO_AF0_OSC32         ((uint8_t)0x00)  /*!< OSC32 (By pass and Enable) Alternate Function mapping */
#define GPIO_AF0_SWJ           ((uint8_t)0x00)  /*!< SWJ (SWD) Alternate Function mapping */
#define GPIO_AF0_SPI1          ((uint8_t)0x00)  /*!< SPI1 Alternate Function mapping */
#define GPIO_AF0_SPI2          ((uint8_t)0x00)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF0_TIM14         ((uint8_t)0x00)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF0_TIM15         ((uint8_t)0x00)  /*!< TIM15 Alternate Function mapping */
#define GPIO_AF0_TIM16         ((uint8_t)0x00)  /*!< TIM16 Alternate Function mapping */
#define GPIO_AF0_TIM17         ((uint8_t)0x00)  /*!< TIM17 Alternate Function mapping */
#define GPIO_AF0_USART1        ((uint8_t)0x00)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF0_USART2        ((uint8_t)0x00)  /*!< USART2 Alternate Function mapping */
#define GPIO_AF0_USART3        ((uint8_t)0x00)  /*!< USART3 Alternate Function mapping */
#define GPIO_AF0_USART4        ((uint8_t)0x00)  /*!< USART4 Alternate Function mapping */
#define GPIO_AF0_UCPD1         ((uint8_t)0x00)  /*!< UCPD1 Alternate Function mapping */
#define GPIO_AF0_UCPD2         ((uint8_t)0x00)  /*!< UCPD2 Alternate Function mapping */

/**
  * @brief   AF 1 selection
  */
#define GPIO_AF1_CEC           ((uint8_t)0x01)  /*!< CEC Alternate Function mapping */
#define GPIO_AF1_EVENTOUT      ((uint8_t)0x01)  /*!< EVENTOUT Alternate Function mapping */
#define GPIO_AF1_IR            ((uint8_t)0x01)  /*!< IR Alternate Function mapping */
#define GPIO_AF1_LPTIM2        ((uint8_t)0x01)  /*!< LPTIM2 Alternate Function mapping */
#define GPIO_AF1_LPUART1       ((uint8_t)0x01)  /*!< LPUART1 Alternate Function mapping */
#define GPIO_AF1_LPUART2       ((uint8_t)0x01)  /*!< LPUART2 Alternate Function mapping */
#define GPIO_AF1_OSC           ((uint8_t)0x01)  /*!< OSC (By pass and Enable) Alternate Function mapping */
#define GPIO_AF1_SPI1          ((uint8_t)0x01)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF1_SPI2          ((uint8_t)0x01)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF1_TIM1          ((uint8_t)0x01)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF1_TIM3          ((uint8_t)0x01)  /*!< TIM3 Alternate Function mapping */
#define GPIO_AF1_USART1        ((uint8_t)0x01)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF1_USART2        ((uint8_t)0x01)  /*!< USART2 Alternate Function mapping */
#define GPIO_AF1_USART4        ((uint8_t)0x01)  /*!< USART4 Alternate Function mapping */
#define GPIO_AF1_UCPD1         ((uint8_t)0x01)  /*!< UCPD1 Alternate Function mapping */
#define GPIO_AF1_UCPD2         ((uint8_t)0x01)  /*!< UCPD2 Alternate Function mapping */

/**
  * @brief   AF 2 selection
  */
#define GPIO_AF2_LPTIM1        ((uint8_t)0x02)  /*!< LPTIM1 Alternate Function mapping */
#define GPIO_AF2_LPTIM2        ((uint8_t)0x02)  /*!< LPTIM2 Alternate Function mapping */
#define GPIO_AF2_TIM1          ((uint8_t)0x02)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF2_TIM2          ((uint8_t)0x02)  /*!< TIM2 Alternate Function mapping */
#define GPIO_AF2_TIM4          ((uint8_t)0x02)  /*!< TIM4 Alternate Function mapping */
#define GPIO_AF2_TIM14         ((uint8_t)0x02)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF2_TIM15         ((uint8_t)0x02)  /*!< TIM15 Alternate Function mapping */
#define GPIO_AF2_TIM16         ((uint8_t)0x02)  /*!< TIM16 Alternate Function mapping */
#define GPIO_AF2_TIM17         ((uint8_t)0x02)  /*!< TIM17 Alternate Function mapping */
#define GPIO_AF2_USB           ((uint8_t)0x02)  /*!< USB Alternate Function mapping */


/**
  * @brief   AF 3 selection
  */
#define GPIO_AF3_FDCAN1        ((uint8_t)0x03)  /*!< FDCAN1 Alternate Function mapping */
#define GPIO_AF3_FDCAN2        ((uint8_t)0x03)  /*!< FDCAN2 Alternate Function mapping */
#define GPIO_AF3_LPUART2       ((uint8_t)0x03)  /*!< LPUART2 Alternate Function mapping */
#define GPIO_AF3_USART5        ((uint8_t)0x03)  /*!< USART5 Alternate Function mapping */
#define GPIO_AF3_USART6        ((uint8_t)0x03)  /*!< USART6 Alternate Function mapping */
#define GPIO_AF3_MCO2          ((uint8_t)0x03)  /*!< MCO2 Alternate Function mapping */

/**
  * @brief   AF 4 selection
  */
#define GPIO_AF4_CRS           ((uint8_t)0x04)  /*!< CRS Alternate Function mapping */
#define GPIO_AF4_SPI2          ((uint8_t)0x04)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF4_SPI3          ((uint8_t)0x04)  /*!< SPI3 Alternate Function mapping */
#define GPIO_AF4_TIM14         ((uint8_t)0x04)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF4_TIM15         ((uint8_t)0x04)  /*!< TIM15 Alternate Function mapping */
#define GPIO_AF4_USART1        ((uint8_t)0x04)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF4_USART3        ((uint8_t)0x04)  /*!< USART3 Alternate Function mapping */
#define GPIO_AF4_USART4        ((uint8_t)0x04)  /*!< USART4 Alternate Function mapping */
#define GPIO_AF4_USART6        ((uint8_t)0x04)  /*!< USART6 Alternate Function mapping */
#define GPIO_AF4_UCPD1         ((uint8_t)0x04)  /*!< UCPD1 Alternate Function mapping */
#define GPIO_AF4_UCPD2         ((uint8_t)0x04)  /*!< UCPD2 Alternate Function mapping */

/**
  * @brief   AF 5 selection
  */
#define GPIO_AF5_LPTIM1        ((uint8_t)0x05)  /*!< LPTIM1 Alternate Function mapping */
#define GPIO_AF5_LPTIM2        ((uint8_t)0x05)  /*!< LPTIM2 Alternate Function mapping */
#define GPIO_AF5_SPI1          ((uint8_t)0x05)  /*!< SPI1 Alternate Function mapping */
#define GPIO_AF5_SPI2          ((uint8_t)0x05)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF5_TIM1          ((uint8_t)0x05)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF5_TIM15         ((uint8_t)0x05)  /*!< TIM15 Alternate Function mapping */
#define GPIO_AF5_TIM16         ((uint8_t)0x05)  /*!< TIM16 Alternate Function mapping */
#define GPIO_AF5_TIM17         ((uint8_t)0x05)  /*!< TIM17 Alternate Function mapping */
#define GPIO_AF5_USART3        ((uint8_t)0x05)  /*!< USART3 Alternate Function mapping */

/**
  * @brief   AF 6 selection
  */
#define GPIO_AF6_I2C1          ((uint8_t)0x06)  /*!< I2C1 Alternate Function mapping */
#define GPIO_AF6_I2C2          ((uint8_t)0x06)  /*!< I2C2 Alternate Function mapping */
#define GPIO_AF6_I2C3          ((uint8_t)0x06)  /*!< I2C3 Alternate Function mapping */
#define GPIO_AF6_LPUART1       ((uint8_t)0x06)  /*!< LPUART1 Alternate Function mapping */
#define GPIO_AF6_UCPD1         ((uint8_t)0x06)  /*!< UCPD1 Alternate Function mapping */
#define GPIO_AF6_UCPD2         ((uint8_t)0x06)  /*!< UCPD2 Alternate Function mapping */
#define GPIO_AF6_USB           ((uint8_t)0x06)  /*!< USB Alternate Function mapping */

/**
  * @brief   AF 7 selection
  */
#define GPIO_AF7_COMP1         ((uint8_t)0x07)  /*!< COMP1 Alternate Function mapping */
#define GPIO_AF7_COMP2         ((uint8_t)0x07)  /*!< COMP2 Alternate Function mapping */
#define GPIO_AF7_COMP3         ((uint8_t)0x07)  /*!< COMP3 Alternate Function mapping */
#define GPIO_AF7_EVENTOUT      ((uint8_t)0x07)  /*!< EVENTOUT Alternate Function mapping */

/**
  * @brief   AF 8 selection
  */
#define GPIO_AF8_I2C2          ((uint8_t)0x08)  /*!< I2C2 Alternate Function mapping */
#define GPIO_AF8_USART5        ((uint8_t)0x08)  /*!< USART5 Alternate Function mapping */
#define GPIO_AF8_USART6        ((uint8_t)0x08)  /*!< USART5 Alternate Function mapping */

/**
  * @brief   AF 9 selection
  */
#define GPIO_AF9_I2C3          ((uint8_t)0x09)  /*!< I2C3 Alternate Function mapping */
#define GPIO_AF9_SPI3          ((uint8_t)0x09)  /*!< SPI3 Alternate Function mapping */
#define GPIO_AF9_TIM4          ((uint8_t)0x09)  /*!< TIM4 Alternate Function mapping */

/**
  * @brief   AF 10 selection
  */
#define GPIO_AF10_LPUART2      ((uint8_t)0x0A)  /*!< LPUART2 Alternate Function mapping */

#define IS_GPIO_AF(AF)         ((AF) <= (uint8_t)0x0A)

#endif

#if defined (STM32G0B0xx)
/*------------------------- STM32G0B0xx ------------------------*/
/**
  * @brief   AF 0 selection
  */
#define GPIO_AF0_CRS           ((uint8_t)0x00)  /*!< CRS Alternate Function mapping */
#define GPIO_AF0_EVENTOUT      ((uint8_t)0x00)  /*!< EVENTOUT Alternate Function mapping */
#define GPIO_AF0_IR            ((uint8_t)0x00)  /*!< IR Alternate Function mapping */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /*!< MCO (MCO1) Alternate Function mapping */
#define GPIO_AF0_OSC           ((uint8_t)0x00)  /*!< OSC (By pass and Enable) Alternate Function mapping */
#define GPIO_AF0_OSC32         ((uint8_t)0x00)  /*!< OSC32 (By pass and Enable) Alternate Function mapping */
#define GPIO_AF0_SWJ           ((uint8_t)0x00)  /*!< SWJ (SWD) Alternate Function mapping */
#define GPIO_AF0_SPI1          ((uint8_t)0x00)  /*!< SPI1 Alternate Function mapping */
#define GPIO_AF0_SPI2          ((uint8_t)0x00)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF0_TIM14         ((uint8_t)0x00)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF0_TIM15         ((uint8_t)0x00)  /*!< TIM15 Alternate Function mapping */
#define GPIO_AF0_TIM16         ((uint8_t)0x00)  /*!< TIM16 Alternate Function mapping */
#define GPIO_AF0_TIM17         ((uint8_t)0x00)  /*!< TIM17 Alternate Function mapping */
#define GPIO_AF0_USART1        ((uint8_t)0x00)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF0_USART2        ((uint8_t)0x00)  /*!< USART2 Alternate Function mapping */
#define GPIO_AF0_USART3        ((uint8_t)0x00)  /*!< USART3 Alternate Function mapping */
#define GPIO_AF0_USART4        ((uint8_t)0x00)  /*!< USART4 Alternate Function mapping */

/**
  * @brief   AF 1 selection
  */
#define GPIO_AF1_EVENTOUT      ((uint8_t)0x01)  /*!< EVENTOUT Alternate Function mapping */
#define GPIO_AF1_IR            ((uint8_t)0x01)  /*!< IR Alternate Function mapping */
#define GPIO_AF1_OSC           ((uint8_t)0x01)  /*!< OSC (By pass and Enable) Alternate Function mapping */
#define GPIO_AF1_SPI1          ((uint8_t)0x01)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF1_SPI2          ((uint8_t)0x01)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF1_TIM1          ((uint8_t)0x01)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF1_TIM3          ((uint8_t)0x01)  /*!< TIM3 Alternate Function mapping */
#define GPIO_AF1_USART1        ((uint8_t)0x01)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF1_USART2        ((uint8_t)0x01)  /*!< USART2 Alternate Function mapping */
#define GPIO_AF1_USART4        ((uint8_t)0x01)  /*!< USART4 Alternate Function mapping */

/**
  * @brief   AF 2 selection
  */
#define GPIO_AF2_TIM1          ((uint8_t)0x02)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF2_TIM4          ((uint8_t)0x02)  /*!< TIM4 Alternate Function mapping */
#define GPIO_AF2_TIM14         ((uint8_t)0x02)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF2_TIM15         ((uint8_t)0x02)  /*!< TIM15 Alternate Function mapping */
#define GPIO_AF2_TIM16         ((uint8_t)0x02)  /*!< TIM16 Alternate Function mapping */
#define GPIO_AF2_TIM17         ((uint8_t)0x02)  /*!< TIM17 Alternate Function mapping */
#define GPIO_AF2_USB           ((uint8_t)0x02)  /*!< USB Alternate Function mapping */


/**
  * @brief   AF 3 selection
  */
#define GPIO_AF3_USART5        ((uint8_t)0x03)  /*!< USART5 Alternate Function mapping */
#define GPIO_AF3_USART6        ((uint8_t)0x03)  /*!< USART6 Alternate Function mapping */
#define GPIO_AF3_MCO2          ((uint8_t)0x03)  /*!< MCO2 Alternate Function mapping */

/**
  * @brief   AF 4 selection
  */
#define GPIO_AF4_CRS           ((uint8_t)0x04)  /*!< CRS Alternate Function mapping */
#define GPIO_AF4_SPI2          ((uint8_t)0x04)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF4_SPI3          ((uint8_t)0x04)  /*!< SPI3 Alternate Function mapping */
#define GPIO_AF4_TIM14         ((uint8_t)0x04)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF4_TIM15         ((uint8_t)0x04)  /*!< TIM15 Alternate Function mapping */
#define GPIO_AF4_USART1        ((uint8_t)0x04)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF4_USART3        ((uint8_t)0x04)  /*!< USART3 Alternate Function mapping */
#define GPIO_AF4_USART4        ((uint8_t)0x04)  /*!< USART4 Alternate Function mapping */
#define GPIO_AF4_USART6        ((uint8_t)0x04)  /*!< USART6 Alternate Function mapping */

/**
  * @brief   AF 5 selection
  */
#define GPIO_AF5_SPI1          ((uint8_t)0x05)  /*!< SPI1 Alternate Function mapping */
#define GPIO_AF5_SPI2          ((uint8_t)0x05)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF5_TIM1          ((uint8_t)0x05)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF5_TIM15         ((uint8_t)0x05)  /*!< TIM15 Alternate Function mapping */
#define GPIO_AF5_TIM16         ((uint8_t)0x05)  /*!< TIM16 Alternate Function mapping */
#define GPIO_AF5_TIM17         ((uint8_t)0x05)  /*!< TIM17 Alternate Function mapping */
#define GPIO_AF5_USART3        ((uint8_t)0x05)  /*!< USART3 Alternate Function mapping */

/**
  * @brief   AF 6 selection
  */
#define GPIO_AF6_I2C1          ((uint8_t)0x06)  /*!< I2C1 Alternate Function mapping */
#define GPIO_AF6_I2C2          ((uint8_t)0x06)  /*!< I2C2 Alternate Function mapping */
#define GPIO_AF6_I2C3          ((uint8_t)0x06)  /*!< I2C3 Alternate Function mapping */
#define GPIO_AF6_USB           ((uint8_t)0x06)  /*!< USB Alternate Function mapping */

/**
  * @brief   AF 7 selection
  */
#define GPIO_AF7_EVENTOUT      ((uint8_t)0x07)  /*!< EVENTOUT Alternate Function mapping */

/**
  * @brief   AF 8 selection
  */
#define GPIO_AF8_I2C2          ((uint8_t)0x08)  /*!< I2C2 Alternate Function mapping */
#define GPIO_AF8_USART5        ((uint8_t)0x08)  /*!< USART5 Alternate Function mapping */
#define GPIO_AF8_USART6        ((uint8_t)0x08)  /*!< USART5 Alternate Function mapping */

/**
  * @brief   AF 9 selection
  */
#define GPIO_AF9_I2C3          ((uint8_t)0x09)  /*!< I2C3 Alternate Function mapping */
#define GPIO_AF9_SPI3          ((uint8_t)0x09)  /*!< SPI3 Alternate Function mapping */
#define GPIO_AF9_TIM4          ((uint8_t)0x09)  /*!< TIM4 Alternate Function mapping */

#define IS_GPIO_AF(AF)         ((AF) <= (uint8_t)0x09)

#endif


#if defined (STM32G081xx) || defined (STM32G071xx)
/*------------------------- STM32G081xx / STM32G071xx ------------------------*/
/**
  * @brief   AF 0 selection
  */
#define GPIO_AF0_CEC           ((uint8_t)0x00)  /*!< CEC Alternate Function mapping */
#define GPIO_AF0_EVENTOUT      ((uint8_t)0x00)  /*!< EVENTOUT Alternate Function mapping */
#define GPIO_AF0_IR            ((uint8_t)0x00)  /*!< IR Alternate Function mapping */
#define GPIO_AF0_LPTIM1        ((uint8_t)0x00)  /*!< LPTIM1 Alternate Function mapping */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /*!< MCO (MCO1) Alternate Function mapping */
#define GPIO_AF0_OSC           ((uint8_t)0x00)  /*!< OSC (By pass and Enable) Alternate Function mapping */
#define GPIO_AF0_OSC32         ((uint8_t)0x00)  /*!< OSC32 (By pass and Enable) Alternate Function mapping */
#define GPIO_AF0_SWJ           ((uint8_t)0x00)  /*!< SWJ (SWD) Alternate Function mapping */
#define GPIO_AF0_SPI1          ((uint8_t)0x00)  /*!< SPI1 Alternate Function mapping */
#define GPIO_AF0_SPI2          ((uint8_t)0x00)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF0_TIM14         ((uint8_t)0x00)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF0_USART1        ((uint8_t)0x00)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF0_USART2        ((uint8_t)0x00)  /*!< USART2 Alternate Function mapping */
#define GPIO_AF0_USART3        ((uint8_t)0x00)  /*!< USART3 Alternate Function mapping */
#define GPIO_AF0_UCPD1         ((uint8_t)0x00)  /*!< UCPD1 Alternate Function mapping */
#define GPIO_AF0_UCPD2         ((uint8_t)0x00)  /*!< UCPD2 Alternate Function mapping */

/**
  * @brief   AF 1 selection
  */
#define GPIO_AF1_CEC           ((uint8_t)0x01)  /*!< CEC Alternate Function mapping */
#define GPIO_AF1_IR            ((uint8_t)0x01)  /*!< IR Alternate Function mapping */
#define GPIO_AF1_LPUART1       ((uint8_t)0x01)  /*!< LPUART1 Alternate Function mapping */
#define GPIO_AF1_OSC           ((uint8_t)0x01)  /*!< OSC (By pass and Enable) Alternate Function mapping */
#define GPIO_AF1_SPI1          ((uint8_t)0x01)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF1_SPI2          ((uint8_t)0x01)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF1_TIM1          ((uint8_t)0x01)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF1_TIM3          ((uint8_t)0x01)  /*!< TIM3 Alternate Function mapping */
#define GPIO_AF1_USART1        ((uint8_t)0x01)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF1_USART2        ((uint8_t)0x01)  /*!< USART2 Alternate Function mapping */
#define GPIO_AF1_USART4        ((uint8_t)0x01)  /*!< USART4 Alternate Function mapping */
#define GPIO_AF1_UCPD1         ((uint8_t)0x01)  /*!< UCPD1 Alternate Function mapping */
#define GPIO_AF1_UCPD2         ((uint8_t)0x01)  /*!< UCPD2 Alternate Function mapping */

/**
  * @brief   AF 2 selection
  */
#define GPIO_AF2_LPTIM1        ((uint8_t)0x02)  /*!< LPTIM1 Alternate Function mapping */
#define GPIO_AF2_LPTIM2        ((uint8_t)0x02)  /*!< LPTIM2 Alternate Function mapping */
#define GPIO_AF2_TIM1          ((uint8_t)0x02)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF2_TIM2          ((uint8_t)0x02)  /*!< TIM2 Alternate Function mapping */
#define GPIO_AF2_TIM14         ((uint8_t)0x02)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF2_TIM15         ((uint8_t)0x02)  /*!< TIM15 Alternate Function mapping */
#define GPIO_AF2_TIM16         ((uint8_t)0x02)  /*!< TIM16 Alternate Function mapping */
#define GPIO_AF2_TIM17         ((uint8_t)0x02)  /*!< TIM17 Alternate Function mapping */

/**
  * @brief   AF 3 selection
  */
#define GPIO_AF3_UCPD1         ((uint8_t)0x03)  /*!< UCPD1 Alternate Function mapping */
#define GPIO_AF3_UCPD2         ((uint8_t)0x03)  /*!< UCPD2 Alternate Function mapping */

/**
  * @brief   AF 4 selection
  */
#define GPIO_AF4_SPI2          ((uint8_t)0x04)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF4_TIM14         ((uint8_t)0x04)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF4_TIM15         ((uint8_t)0x04)  /*!< TIM15 Alternate Function mapping */
#define GPIO_AF4_USART1        ((uint8_t)0x04)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF4_USART3        ((uint8_t)0x04)  /*!< USART3 Alternate Function mapping */
#define GPIO_AF4_USART4        ((uint8_t)0x04)  /*!< USART4 Alternate Function mapping */
#define GPIO_AF4_UCPD1         ((uint8_t)0x04)  /*!< UCPD1 Alternate Function mapping */
#define GPIO_AF4_UCPD2         ((uint8_t)0x04)  /*!< UCPD2 Alternate Function mapping */

/**
  * @brief   AF 5 selection
  */
#define GPIO_AF5_LPTIM1        ((uint8_t)0x05)  /*!< LPTIM1 Alternate Function mapping */
#define GPIO_AF5_LPTIM2        ((uint8_t)0x05)  /*!< LPTIM2 Alternate Function mapping */
#define GPIO_AF5_SPI1          ((uint8_t)0x05)  /*!< SPI1 Alternate Function mapping */
#define GPIO_AF5_SPI2          ((uint8_t)0x05)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF5_TIM1          ((uint8_t)0x05)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF5_TIM15         ((uint8_t)0x05)  /*!< TIM15 Alternate Function mapping */
#define GPIO_AF5_TIM16         ((uint8_t)0x05)  /*!< TIM16 Alternate Function mapping */
#define GPIO_AF5_TIM17         ((uint8_t)0x05)  /*!< TIM17 Alternate Function mapping */
#define GPIO_AF5_USART3        ((uint8_t)0x05)  /*!< USART3 Alternate Function mapping */

/**
  * @brief   AF 6 selection
  */
#define GPIO_AF6_I2C1          ((uint8_t)0x06)  /*!< I2C1 Alternate Function mapping */
#define GPIO_AF6_I2C2          ((uint8_t)0x06)  /*!< I2C2 Alternate Function mapping */
#define GPIO_AF6_LPUART1       ((uint8_t)0x06)  /*!< LPUART1 Alternate Function mapping */
#define GPIO_AF6_UCPD1         ((uint8_t)0x06)  /*!< UCPD1 Alternate Function mapping */
#define GPIO_AF6_UCPD2         ((uint8_t)0x06)  /*!< UCPD2 Alternate Function mapping */

/**
  * @brief   AF 7 selection
  */
#define GPIO_AF7_COMP1         ((uint8_t)0x07)  /*!< COMP1 Alternate Function mapping */
#define GPIO_AF7_COMP2         ((uint8_t)0x07)  /*!< COMP2 Alternate Function mapping */
#define GPIO_AF7_EVENTOUT      ((uint8_t)0x07)  /*!< EVENTOUT Alternate Function mapping */

#define IS_GPIO_AF(AF)         ((AF) <= (uint8_t)0x07)

#endif /* STM32G081xx || STM32G071xx */

#if defined (STM32G070xx)
/**
  * @brief   AF 0 selection
  */
#define GPIO_AF0_EVENTOUT      ((uint8_t)0x00)  /*!< EVENTOUT Alternate Function mapping */
#define GPIO_AF0_IR            ((uint8_t)0x00)  /*!< IR Alternate Function mapping */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /*!< MCO (MCO1) Alternate Function mapping */
#define GPIO_AF0_OSC           ((uint8_t)0x00)  /*!< OSC (By pass and Enable) Alternate Function mapping */
#define GPIO_AF0_OSC32         ((uint8_t)0x00)  /*!< OSC32 (By pass and Enable) Alternate Function mapping */
#define GPIO_AF0_SWJ           ((uint8_t)0x00)  /*!< SWJ (SWD) Alternate Function mapping */
#define GPIO_AF0_SPI1          ((uint8_t)0x00)  /*!< SPI1 Alternate Function mapping */
#define GPIO_AF0_SPI2          ((uint8_t)0x00)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF0_TIM14         ((uint8_t)0x00)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF0_USART1        ((uint8_t)0x00)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF0_USART2        ((uint8_t)0x00)  /*!< USART2 Alternate Function mapping */
#define GPIO_AF0_USART3        ((uint8_t)0x00)  /*!< USART3 Alternate Function mapping */

/**
  * @brief   AF 1 selection
  */
#define GPIO_AF1_IR            ((uint8_t)0x01)  /*!< IR Alternate Function mapping */
#define GPIO_AF1_OSC           ((uint8_t)0x01)  /*!< OSC (By pass and Enable) Alternate Function mapping */
#define GPIO_AF1_SPI1          ((uint8_t)0x01)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF1_SPI2          ((uint8_t)0x01)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF1_TIM1          ((uint8_t)0x01)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF1_TIM3          ((uint8_t)0x01)  /*!< TIM3 Alternate Function mapping */
#define GPIO_AF1_USART1        ((uint8_t)0x01)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF1_USART2        ((uint8_t)0x01)  /*!< USART2 Alternate Function mapping */
#define GPIO_AF1_USART4        ((uint8_t)0x01)  /*!< USART4 Alternate Function mapping */

/**
  * @brief   AF 2 selection
  */
#define GPIO_AF2_TIM1          ((uint8_t)0x02)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF2_TIM14         ((uint8_t)0x02)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF2_TIM15         ((uint8_t)0x02)  /*!< TIM15 Alternate Function mapping */
#define GPIO_AF2_TIM16         ((uint8_t)0x02)  /*!< TIM16 Alternate Function mapping */
#define GPIO_AF2_TIM17         ((uint8_t)0x02)  /*!< TIM17 Alternate Function mapping */

/**
  * @brief   AF 3 selection
  */

/**
  * @brief   AF 4 selection
  */
#define GPIO_AF4_SPI2          ((uint8_t)0x04)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF4_TIM14         ((uint8_t)0x04)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF4_TIM15         ((uint8_t)0x04)  /*!< TIM15 Alternate Function mapping */
#define GPIO_AF4_USART1        ((uint8_t)0x04)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF4_USART3        ((uint8_t)0x04)  /*!< USART3 Alternate Function mapping */
#define GPIO_AF4_USART4        ((uint8_t)0x04)  /*!< USART4 Alternate Function mapping */

/**
  * @brief   AF 5 selection
  */
#define GPIO_AF5_SPI1          ((uint8_t)0x05)  /*!< SPI1 Alternate Function mapping */
#define GPIO_AF5_SPI2          ((uint8_t)0x05)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF5_TIM1          ((uint8_t)0x05)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF5_TIM15         ((uint8_t)0x05)  /*!< TIM15 Alternate Function mapping */
#define GPIO_AF5_TIM16         ((uint8_t)0x05)  /*!< TIM16 Alternate Function mapping */
#define GPIO_AF5_TIM17         ((uint8_t)0x05)  /*!< TIM17 Alternate Function mapping */
#define GPIO_AF5_USART3        ((uint8_t)0x05)  /*!< USART3 Alternate Function mapping */

/**
  * @brief   AF 6 selection
  */
#define GPIO_AF6_I2C1          ((uint8_t)0x06)  /*!< I2C1 Alternate Function mapping */
#define GPIO_AF6_I2C2          ((uint8_t)0x06)  /*!< I2C2 Alternate Function mapping */

/**
  * @brief   AF 7 selection
  */
#define GPIO_AF7_EVENTOUT      ((uint8_t)0x07)  /*!< EVENTOUT Alternate Function mapping */

#define IS_GPIO_AF(AF)         ((AF) <= (uint8_t)0x07)

#endif /* STM32G070xx */

#if defined (STM32G051xx) || defined (STM32G061xx)
/*------------------------- STM32G061xx / STM32G051xx ------------------------*/
/**
  * @brief   AF 0 selection
  */
#define GPIO_AF0_EVENTOUT      ((uint8_t)0x00)  /*!< EVENTOUT Alternate Function mapping */
#define GPIO_AF0_IR            ((uint8_t)0x00)  /*!< IR Alternate Function mapping */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /*!< MCO (MCO1) Alternate Function mapping */
#define GPIO_AF0_OSC           ((uint8_t)0x00)  /*!< OSC (By pass and Enable) Alternate Function mapping */
#define GPIO_AF0_OSC32         ((uint8_t)0x00)  /*!< OSC32 (By pass and Enable) Alternate Function mapping */
#define GPIO_AF0_SWJ           ((uint8_t)0x00)  /*!< SWJ (SWD) Alternate Function mapping */
#define GPIO_AF0_SPI1          ((uint8_t)0x00)  /*!< SPI1 Alternate Function mapping */
#define GPIO_AF0_SPI2          ((uint8_t)0x00)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF0_TIM14         ((uint8_t)0x00)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF0_USART1        ((uint8_t)0x00)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF0_USART2        ((uint8_t)0x00)  /*!< USART2 Alternate Function mapping */

/**
  * @brief   AF 1 selection
  */
#define GPIO_AF1_IR            ((uint8_t)0x01)  /*!< IR Alternate Function mapping */
#define GPIO_AF1_LPUART1       ((uint8_t)0x01)  /*!< LPUART1 Alternate Function mapping */
#define GPIO_AF1_OSC           ((uint8_t)0x01)  /*!< OSC (By pass and Enable) Alternate Function mapping */
#define GPIO_AF1_SPI2          ((uint8_t)0x01)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF1_TIM1          ((uint8_t)0x01)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF1_TIM3          ((uint8_t)0x01)  /*!< TIM3 Alternate Function mapping */
#define GPIO_AF1_USART1        ((uint8_t)0x01)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF1_USART2        ((uint8_t)0x01)  /*!< USART2 Alternate Function mapping */

/**
  * @brief   AF 2 selection
  */
#define GPIO_AF2_TIM1          ((uint8_t)0x02)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF2_TIM2          ((uint8_t)0x02)  /*!< TIM2 Alternate Function mapping */
#define GPIO_AF2_TIM14         ((uint8_t)0x02)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF2_TIM15         ((uint8_t)0x02)  /*!< TIM15 Alternate Function mapping */
#define GPIO_AF2_TIM16         ((uint8_t)0x02)  /*!< TIM16 Alternate Function mapping */
#define GPIO_AF2_TIM17         ((uint8_t)0x02)  /*!< TIM17 Alternate Function mapping */

/**
  * @brief   AF 3 selection
  */

/**
  * @brief   AF 4 selection
  */
#define GPIO_AF4_SPI2          ((uint8_t)0x04)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF4_TIM14         ((uint8_t)0x04)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF4_TIM15         ((uint8_t)0x04)  /*!< TIM15 Alternate Function mapping */
#define GPIO_AF4_USART1        ((uint8_t)0x04)  /*!< USART1 Alternate Function mapping */

/**
  * @brief   AF 5 selection
  */
#define GPIO_AF5_LPTIM1        ((uint8_t)0x05)  /*!< LPTIM1 Alternate Function mapping */
#define GPIO_AF5_LPTIM2        ((uint8_t)0x05)  /*!< LPTIM2 Alternate Function mapping */
#define GPIO_AF5_SPI1          ((uint8_t)0x05)  /*!< SPI1 Alternate Function mapping */
#define GPIO_AF5_SPI2          ((uint8_t)0x05)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF5_TIM1          ((uint8_t)0x05)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF5_TIM15         ((uint8_t)0x05)  /*!< TIM15 Alternate Function mapping */
#define GPIO_AF5_TIM16         ((uint8_t)0x05)  /*!< TIM16 Alternate Function mapping */
#define GPIO_AF5_TIM17         ((uint8_t)0x05)  /*!< TIM17 Alternate Function mapping */

/**
  * @brief   AF 6 selection
  */
#define GPIO_AF6_I2C1          ((uint8_t)0x06)  /*!< I2C1 Alternate Function mapping */
#define GPIO_AF6_I2C2          ((uint8_t)0x06)  /*!< I2C2 Alternate Function mapping */
#define GPIO_AF6_LPUART1       ((uint8_t)0x06)  /*!< LPUART1 Alternate Function mapping */

/**
  * @brief   AF 7 selection
  */
#define GPIO_AF7_COMP1         ((uint8_t)0x07)  /*!< COMP1 Alternate Function mapping */
#define GPIO_AF7_COMP2         ((uint8_t)0x07)  /*!< COMP2 Alternate Function mapping */
#define GPIO_AF7_EVENTOUT      ((uint8_t)0x07)  /*!< EVENTOUT Alternate Function mapping */

#define IS_GPIO_AF(AF)         ((AF) <= (uint8_t)0x07)

#endif /* STM32G051xx || STM32G061xx */

#if defined (STM32G050xx)
/*------------------------- STM32G050xx --------------------------------------*/
/**
  * @brief   AF 0 selection
  */
#define GPIO_AF0_EVENTOUT      ((uint8_t)0x00)  /*!< EVENTOUT Alternate Function mapping */
#define GPIO_AF0_IR            ((uint8_t)0x00)  /*!< IR Alternate Function mapping */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /*!< MCO (MCO1) Alternate Function mapping */
#define GPIO_AF0_OSC           ((uint8_t)0x00)  /*!< OSC (By pass and Enable) Alternate Function mapping */
#define GPIO_AF0_OSC32         ((uint8_t)0x00)  /*!< OSC32 (By pass and Enable) Alternate Function mapping */
#define GPIO_AF0_SWJ           ((uint8_t)0x00)  /*!< SWJ (SWD) Alternate Function mapping */
#define GPIO_AF0_SPI1          ((uint8_t)0x00)  /*!< SPI1 Alternate Function mapping */
#define GPIO_AF0_SPI2          ((uint8_t)0x00)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF0_TIM14         ((uint8_t)0x00)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF0_USART1        ((uint8_t)0x00)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF0_USART2        ((uint8_t)0x00)  /*!< USART2 Alternate Function mapping */

/**
  * @brief   AF 1 selection
  */
#define GPIO_AF1_IR            ((uint8_t)0x01)  /*!< IR Alternate Function mapping */
#define GPIO_AF1_OSC           ((uint8_t)0x01)  /*!< OSC (By pass and Enable) Alternate Function mapping */
#define GPIO_AF1_SPI2          ((uint8_t)0x01)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF1_TIM1          ((uint8_t)0x01)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF1_TIM3          ((uint8_t)0x01)  /*!< TIM3 Alternate Function mapping */
#define GPIO_AF1_USART1        ((uint8_t)0x01)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF1_USART2        ((uint8_t)0x01)  /*!< USART2 Alternate Function mapping */

/**
  * @brief   AF 2 selection
  */
#define GPIO_AF2_TIM1          ((uint8_t)0x02)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF2_TIM14         ((uint8_t)0x02)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF2_TIM15         ((uint8_t)0x02)  /*!< TIM15 Alternate Function mapping */
#define GPIO_AF2_TIM16         ((uint8_t)0x02)  /*!< TIM16 Alternate Function mapping */
#define GPIO_AF2_TIM17         ((uint8_t)0x02)  /*!< TIM17 Alternate Function mapping */

/**
  * @brief   AF 3 selection
  */

/**
  * @brief   AF 4 selection
  */
#define GPIO_AF4_SPI2          ((uint8_t)0x04)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF4_TIM14         ((uint8_t)0x04)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF4_TIM15         ((uint8_t)0x04)  /*!< TIM15 Alternate Function mapping */
#define GPIO_AF4_USART1        ((uint8_t)0x04)  /*!< USART1 Alternate Function mapping */

/**
  * @brief   AF 5 selection
  */
#define GPIO_AF5_SPI1          ((uint8_t)0x05)  /*!< SPI1 Alternate Function mapping */
#define GPIO_AF5_SPI2          ((uint8_t)0x05)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF5_TIM1          ((uint8_t)0x05)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF5_TIM15         ((uint8_t)0x05)  /*!< TIM15 Alternate Function mapping */
#define GPIO_AF5_TIM16         ((uint8_t)0x05)  /*!< TIM16 Alternate Function mapping */
#define GPIO_AF5_TIM17         ((uint8_t)0x05)  /*!< TIM17 Alternate Function mapping */

/**
  * @brief   AF 6 selection
  */
#define GPIO_AF6_I2C1          ((uint8_t)0x06)  /*!< I2C1 Alternate Function mapping */
#define GPIO_AF6_I2C2          ((uint8_t)0x06)  /*!< I2C2 Alternate Function mapping */

/**
  * @brief   AF 7 selection
  */
#define GPIO_AF7_EVENTOUT      ((uint8_t)0x07)  /*!< EVENTOUT Alternate Function mapping */

#define IS_GPIO_AF(AF)         ((AF) <= (uint8_t)0x07)

#endif /* STM32G050xx */

#if defined (STM32G031xx) || defined (STM32G041xx)
/*------------------------- STM32G041xx / STM32G031xx ------------------------*/
/**
  * @brief   AF 0 selection
  */
#define GPIO_AF0_EVENTOUT      ((uint8_t)0x00)  /*!< EVENTOUT Alternate Function mapping */
#define GPIO_AF0_IR            ((uint8_t)0x00)  /*!< IR Alternate Function mapping */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /*!< MCO (MCO1) Alternate Function mapping */
#define GPIO_AF0_OSC           ((uint8_t)0x00)  /*!< OSC (By pass and Enable) Alternate Function mapping */
#define GPIO_AF0_OSC32         ((uint8_t)0x00)  /*!< OSC32 (By pass and Enable) Alternate Function mapping */
#define GPIO_AF0_SWJ           ((uint8_t)0x00)  /*!< SWJ (SWD) Alternate Function mapping */
#define GPIO_AF0_SPI1          ((uint8_t)0x00)  /*!< SPI1 Alternate Function mapping */
#define GPIO_AF0_SPI2          ((uint8_t)0x00)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF0_TIM14         ((uint8_t)0x00)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF0_USART1        ((uint8_t)0x00)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF0_USART2        ((uint8_t)0x00)  /*!< USART2 Alternate Function mapping */

/**
  * @brief   AF 1 selection
  */
#define GPIO_AF1_IR            ((uint8_t)0x01)  /*!< IR Alternate Function mapping */
#define GPIO_AF1_LPUART1       ((uint8_t)0x01)  /*!< LPUART1 Alternate Function mapping */
#define GPIO_AF1_OSC           ((uint8_t)0x01)  /*!< OSC (By pass and Enable) Alternate Function mapping */
#define GPIO_AF1_SPI2          ((uint8_t)0x01)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF1_TIM1          ((uint8_t)0x01)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF1_TIM3          ((uint8_t)0x01)  /*!< TIM3 Alternate Function mapping */
#define GPIO_AF1_USART1        ((uint8_t)0x01)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF1_USART2        ((uint8_t)0x01)  /*!< USART2 Alternate Function mapping */

/**
  * @brief   AF 2 selection
  */
#define GPIO_AF2_TIM1          ((uint8_t)0x02)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF2_TIM2          ((uint8_t)0x02)  /*!< TIM2 Alternate Function mapping */
#define GPIO_AF2_TIM14         ((uint8_t)0x02)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF2_TIM16         ((uint8_t)0x02)  /*!< TIM16 Alternate Function mapping */
#define GPIO_AF2_TIM17         ((uint8_t)0x02)  /*!< TIM17 Alternate Function mapping */

/**
  * @brief   AF 3 selection
  */

/**
  * @brief   AF 4 selection
  */
#define GPIO_AF4_SPI2          ((uint8_t)0x04)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF4_TIM14         ((uint8_t)0x04)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF4_USART1        ((uint8_t)0x04)  /*!< USART1 Alternate Function mapping */

/**
  * @brief   AF 5 selection
  */
#define GPIO_AF5_LPTIM1        ((uint8_t)0x05)  /*!< LPTIM1 Alternate Function mapping */
#define GPIO_AF5_LPTIM2        ((uint8_t)0x05)  /*!< LPTIM2 Alternate Function mapping */
#define GPIO_AF5_SPI1          ((uint8_t)0x05)  /*!< SPI1 Alternate Function mapping */
#define GPIO_AF5_SPI2          ((uint8_t)0x05)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF5_TIM1          ((uint8_t)0x05)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF5_TIM16         ((uint8_t)0x05)  /*!< TIM16 Alternate Function mapping */
#define GPIO_AF5_TIM17         ((uint8_t)0x05)  /*!< TIM17 Alternate Function mapping */

/**
  * @brief   AF 6 selection
  */
#define GPIO_AF6_I2C1          ((uint8_t)0x06)  /*!< I2C1 Alternate Function mapping */
#define GPIO_AF6_I2C2          ((uint8_t)0x06)  /*!< I2C2 Alternate Function mapping */
#define GPIO_AF6_LPUART1       ((uint8_t)0x06)  /*!< LPUART1 Alternate Function mapping */

/**
  * @brief   AF 7 selection
  */
#define GPIO_AF7_EVENTOUT      ((uint8_t)0x07)  /*!< EVENTOUT Alternate Function mapping */

#define IS_GPIO_AF(AF)         ((AF) <= (uint8_t)0x07)

#endif /* STM32G031xx || STM32G041xx */

#if defined (STM32G030xx)
/*------------------------- STM32G030xx --------------------------------------*/
/**
  * @brief   AF 0 selection
  */
#define GPIO_AF0_EVENTOUT      ((uint8_t)0x00)  /*!< EVENTOUT Alternate Function mapping */
#define GPIO_AF0_IR            ((uint8_t)0x00)  /*!< IR Alternate Function mapping */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /*!< MCO (MCO1) Alternate Function mapping */
#define GPIO_AF0_OSC           ((uint8_t)0x00)  /*!< OSC (By pass and Enable) Alternate Function mapping */
#define GPIO_AF0_OSC32         ((uint8_t)0x00)  /*!< OSC32 (By pass and Enable) Alternate Function mapping */
#define GPIO_AF0_SWJ           ((uint8_t)0x00)  /*!< SWJ (SWD) Alternate Function mapping */
#define GPIO_AF0_SPI1          ((uint8_t)0x00)  /*!< SPI1 Alternate Function mapping */
#define GPIO_AF0_SPI2          ((uint8_t)0x00)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF0_TIM14         ((uint8_t)0x00)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF0_USART1        ((uint8_t)0x00)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF0_USART2        ((uint8_t)0x00)  /*!< USART2 Alternate Function mapping */

/**
  * @brief   AF 1 selection
  */
#define GPIO_AF1_IR            ((uint8_t)0x01)  /*!< IR Alternate Function mapping */
#define GPIO_AF1_OSC           ((uint8_t)0x01)  /*!< OSC (By pass and Enable) Alternate Function mapping */
#define GPIO_AF1_SPI2          ((uint8_t)0x01)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF1_TIM1          ((uint8_t)0x01)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF1_TIM3          ((uint8_t)0x01)  /*!< TIM3 Alternate Function mapping */
#define GPIO_AF1_USART1        ((uint8_t)0x01)  /*!< USART1 Alternate Function mapping */
#define GPIO_AF1_USART2        ((uint8_t)0x01)  /*!< USART2 Alternate Function mapping */

/**
  * @brief   AF 2 selection
  */
#define GPIO_AF2_TIM1          ((uint8_t)0x02)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF2_TIM14         ((uint8_t)0x02)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF2_TIM16         ((uint8_t)0x02)  /*!< TIM16 Alternate Function mapping */
#define GPIO_AF2_TIM17         ((uint8_t)0x02)  /*!< TIM17 Alternate Function mapping */

/**
  * @brief   AF 3 selection
  */

/**
  * @brief   AF 4 selection
  */
#define GPIO_AF4_SPI2          ((uint8_t)0x04)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF4_TIM14         ((uint8_t)0x04)  /*!< TIM14 Alternate Function mapping */
#define GPIO_AF4_USART1        ((uint8_t)0x04)  /*!< USART1 Alternate Function mapping */

/**
  * @brief   AF 5 selection
  */
#define GPIO_AF5_SPI1          ((uint8_t)0x05)  /*!< SPI1 Alternate Function mapping */
#define GPIO_AF5_SPI2          ((uint8_t)0x05)  /*!< SPI2 Alternate Function mapping */
#define GPIO_AF5_TIM1          ((uint8_t)0x05)  /*!< TIM1 Alternate Function mapping */
#define GPIO_AF5_TIM16         ((uint8_t)0x05)  /*!< TIM16 Alternate Function mapping */
#define GPIO_AF5_TIM17         ((uint8_t)0x05)  /*!< TIM17 Alternate Function mapping */

/**
  * @brief   AF 6 selection
  */
#define GPIO_AF6_I2C1          ((uint8_t)0x06)  /*!< I2C1 Alternate Function mapping */
#define GPIO_AF6_I2C2          ((uint8_t)0x06)  /*!< I2C2 Alternate Function mapping */

/**
  * @brief   AF 7 selection
  */
#define GPIO_AF7_EVENTOUT      ((uint8_t)0x07)  /*!< EVENTOUT Alternate Function mapping */

#define IS_GPIO_AF(AF)         ((AF) <= (uint8_t)0x07)

#endif /* STM32G030xx */

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
#if defined(GPIOE)   
#define GPIO_GET_INDEX(__GPIOx__)    (((__GPIOx__) == (GPIOA))? 0uL :\
                                      ((__GPIOx__) == (GPIOB))? 1uL :\
                                      ((__GPIOx__) == (GPIOC))? 2uL :\
                                      ((__GPIOx__) == (GPIOD))? 3uL :\
                                      ((__GPIOx__) == (GPIOE))? 4uL : 5uL)
#else
#define GPIO_GET_INDEX(__GPIOx__)    (((__GPIOx__) == (GPIOA))? 0uL :\
                                      ((__GPIOx__) == (GPIOB))? 1uL :\
                                      ((__GPIOx__) == (GPIOC))? 2uL :\
                                      ((__GPIOx__) == (GPIOD))? 3uL : 5uL)
#endif

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

#endif /* STM32G0xx_HAL_GPIO_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
