/**
  ******************************************************************************
  * @file    stm32f2xx_hal_gpio_ex.h
  * @author  MCD Application Team
  * @brief   Header file of GPIO HAL Extension module.
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
#ifndef __STM32F2xx_HAL_GPIO_EX_H
#define __STM32F2xx_HAL_GPIO_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal_def.h"

/** @addtogroup STM32F2xx_HAL_Driver
  * @{
  */

/** @defgroup GPIOEx GPIOEx
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** @defgroup GPIOEx_Exported_Constants GPIO Exported Constants
  * @{
  */ 
  
/** @defgroup GPIO_Alternate_function_selection GPIO Alternate function selection 
  * @{
  */

/** 
  * @brief   AF 0 selection  
  */ 
#define GPIO_AF0_RTC_50Hz      ((uint8_t)0x00)  /* RTC_50Hz Alternate Function mapping                       */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /* MCO (MCO1 and MCO2) Alternate Function mapping            */
#define GPIO_AF0_TAMPER        ((uint8_t)0x00)  /* TAMPER (TAMPER_1 and TAMPER_2) Alternate Function mapping */
#define GPIO_AF0_SWJ           ((uint8_t)0x00)  /* SWJ (SWD and JTAG) Alternate Function mapping             */
#define GPIO_AF0_TRACE         ((uint8_t)0x00)  /* TRACE Alternate Function mapping                          */

/** 
  * @brief   AF 1 selection  
  */ 
#define GPIO_AF1_TIM1          ((uint8_t)0x01)  /* TIM1 Alternate Function mapping */
#define GPIO_AF1_TIM2          ((uint8_t)0x01)  /* TIM2 Alternate Function mapping */

/** 
  * @brief   AF 2 selection  
  */ 
#define GPIO_AF2_TIM3          ((uint8_t)0x02)  /* TIM3 Alternate Function mapping */
#define GPIO_AF2_TIM4          ((uint8_t)0x02)  /* TIM4 Alternate Function mapping */
#define GPIO_AF2_TIM5          ((uint8_t)0x02)  /* TIM5 Alternate Function mapping */

/** 
  * @brief   AF 3 selection  
  */ 
#define GPIO_AF3_TIM8          ((uint8_t)0x03)  /* TIM8 Alternate Function mapping  */
#define GPIO_AF3_TIM9          ((uint8_t)0x03)  /* TIM9 Alternate Function mapping  */
#define GPIO_AF3_TIM10         ((uint8_t)0x03)  /* TIM10 Alternate Function mapping */
#define GPIO_AF3_TIM11         ((uint8_t)0x03)  /* TIM11 Alternate Function mapping */

/** 
  * @brief   AF 4 selection  
  */ 
#define GPIO_AF4_I2C1          ((uint8_t)0x04)  /* I2C1 Alternate Function mapping */
#define GPIO_AF4_I2C2          ((uint8_t)0x04)  /* I2C2 Alternate Function mapping */
#define GPIO_AF4_I2C3          ((uint8_t)0x04)  /* I2C3 Alternate Function mapping */

/** 
  * @brief   AF 5 selection  
  */ 
#define GPIO_AF5_SPI1          ((uint8_t)0x05)  /* SPI1 Alternate Function mapping        */
#define GPIO_AF5_SPI2          ((uint8_t)0x05)  /* SPI2/I2S2 Alternate Function mapping   */
/** 
  * @brief   AF 6 selection  
  */ 
#define GPIO_AF6_SPI3          ((uint8_t)0x06)  /* SPI3/I2S3 Alternate Function mapping  */

/** 
  * @brief   AF 7 selection  
  */ 
#define GPIO_AF7_USART1        ((uint8_t)0x07)  /* USART1 Alternate Function mapping     */
#define GPIO_AF7_USART2        ((uint8_t)0x07)  /* USART2 Alternate Function mapping     */
#define GPIO_AF7_USART3        ((uint8_t)0x07)  /* USART3 Alternate Function mapping     */

/** 
  * @brief   AF 8 selection  
  */ 
#define GPIO_AF8_UART4         ((uint8_t)0x08)  /* UART4 Alternate Function mapping  */
#define GPIO_AF8_UART5         ((uint8_t)0x08)  /* UART5 Alternate Function mapping  */
#define GPIO_AF8_USART6        ((uint8_t)0x08)  /* USART6 Alternate Function mapping */

/** 
  * @brief   AF 9 selection 
  */ 
#define GPIO_AF9_CAN1          ((uint8_t)0x09)  /* CAN1 Alternate Function mapping  */
#define GPIO_AF9_CAN2          ((uint8_t)0x09)  /* CAN2 Alternate Function mapping  */
#define GPIO_AF9_TIM12         ((uint8_t)0x09)  /* TIM12 Alternate Function mapping */
#define GPIO_AF9_TIM13         ((uint8_t)0x09)  /* TIM13 Alternate Function mapping */
#define GPIO_AF9_TIM14         ((uint8_t)0x09)  /* TIM14 Alternate Function mapping */

/** 
  * @brief   AF 10 selection  
  */ 
#define GPIO_AF10_OTG_FS        ((uint8_t)0x0A)  /* OTG_FS Alternate Function mapping */
#define GPIO_AF10_OTG_HS        ((uint8_t)0x0A)  /* OTG_HS Alternate Function mapping */

/** 
  * @brief   AF 11 selection  
  */ 
#if defined(STM32F207xx) || defined(STM32F217xx)
#define GPIO_AF11_ETH           ((uint8_t)0x0B)  /* ETHERNET Alternate Function mapping */
#endif /* STM32F207xx || STM32F217xx */

/** 
  * @brief   AF 12 selection  
  */ 
#define GPIO_AF12_FSMC          ((uint8_t)0x0C)  /* FSMC Alternate Function mapping                     */
#define GPIO_AF12_OTG_HS_FS     ((uint8_t)0x0C)  /* OTG HS configured in FS, Alternate Function mapping */
#define GPIO_AF12_SDIO          ((uint8_t)0x0C)  /* SDIO Alternate Function mapping                     */

/** 
  * @brief   AF 13 selection  
  */ 
#if defined(STM32F207xx) || defined(STM32F217xx)
#define GPIO_AF13_DCMI          ((uint8_t)0x0D)  /* DCMI Alternate Function mapping */
#endif /* STM32F207xx || STM32F217xx */

/** 
  * @brief   AF 15 selection  
  */ 
#define GPIO_AF15_EVENTOUT      ((uint8_t)0x0F)  /* EVENTOUT Alternate Function mapping */

/**
  * @}
  */ 

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup GPIOEx_Exported_Macros GPIO Exported Macros
  * @{
  */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/ 
/** @defgroup GPIOEx_Exported_Functions GPIO Exported Functions
  * @{
  */
/**
  * @}
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup GPIOEx_Private_Constants GPIO Private Constants
  * @{
  */
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup GPIOEx_Private_Macros GPIO Private Macros
  * @{
  */
/** @defgroup GPIOEx_Get_Port_Index GPIO Get Port Index
  * @{
  */
#define GPIO_GET_INDEX(__GPIOx__)    (uint8_t)(((__GPIOx__) == (GPIOA))? 0U :\
                                               ((__GPIOx__) == (GPIOB))? 1U :\
                                               ((__GPIOx__) == (GPIOC))? 2U :\
                                               ((__GPIOx__) == (GPIOD))? 3U :\
                                               ((__GPIOx__) == (GPIOE))? 4U :\
                                               ((__GPIOx__) == (GPIOF))? 5U :\
                                               ((__GPIOx__) == (GPIOG))? 6U :\
                                               ((__GPIOx__) == (GPIOH))? 7U :\
                                               ((__GPIOx__) == (GPIOI))? 8U : 9U)
/**
  * @}
  */

/** @defgroup GPIOEx_IS_Alternat_function_selection GPIO Check Alternate Function
  * @{
  */  
#if defined(STM32F207xx) || defined(STM32F217xx)
 
#define IS_GPIO_AF(AF)   (((AF) == GPIO_AF0_RTC_50Hz)   || ((AF) == GPIO_AF9_TIM14)      || \
                          ((AF) == GPIO_AF0_MCO)        || ((AF) == GPIO_AF0_TAMPER)     || \
                          ((AF) == GPIO_AF0_SWJ)        || ((AF) == GPIO_AF0_TRACE)      || \
                          ((AF) == GPIO_AF1_TIM1)       || ((AF) == GPIO_AF1_TIM2)       || \
                          ((AF) == GPIO_AF2_TIM3)       || ((AF) == GPIO_AF2_TIM4)       || \
                          ((AF) == GPIO_AF2_TIM5)       || ((AF) == GPIO_AF3_TIM8)       || \
                          ((AF) == GPIO_AF4_I2C1)       || ((AF) == GPIO_AF4_I2C2)       || \
                          ((AF) == GPIO_AF4_I2C3)       || ((AF) == GPIO_AF5_SPI1)       || \
                          ((AF) == GPIO_AF5_SPI2)       || ((AF) == GPIO_AF9_TIM13)      || \
                          ((AF) == GPIO_AF6_SPI3)       || ((AF) == GPIO_AF9_TIM12)      || \
                          ((AF) == GPIO_AF7_USART1)     || ((AF) == GPIO_AF7_USART2)     || \
                          ((AF) == GPIO_AF7_USART3)     || ((AF) == GPIO_AF8_UART4)      || \
                          ((AF) == GPIO_AF8_UART5)      || ((AF) == GPIO_AF8_USART6)     || \
                          ((AF) == GPIO_AF9_CAN1)       || ((AF) == GPIO_AF9_CAN2)       || \
                          ((AF) == GPIO_AF10_OTG_FS)    || ((AF) == GPIO_AF10_OTG_HS)    || \
                          ((AF) == GPIO_AF11_ETH)       || ((AF) == GPIO_AF12_OTG_HS_FS) || \
                          ((AF) == GPIO_AF12_SDIO)      || ((AF) == GPIO_AF13_DCMI)      || \
                          ((AF) == GPIO_AF12_FSMC)      || ((AF) == GPIO_AF15_EVENTOUT))
#else /* STM32F207xx || STM32F217xx */
#define IS_GPIO_AF(AF)   (((AF) == GPIO_AF0_RTC_50Hz)   || ((AF) == GPIO_AF9_TIM14)      || \
                          ((AF) == GPIO_AF0_MCO)        || ((AF) == GPIO_AF0_TAMPER)     || \
                          ((AF) == GPIO_AF0_SWJ)        || ((AF) == GPIO_AF0_TRACE)      || \
                          ((AF) == GPIO_AF1_TIM1)       || ((AF) == GPIO_AF1_TIM2)       || \
                          ((AF) == GPIO_AF2_TIM3)       || ((AF) == GPIO_AF2_TIM4)       || \
                          ((AF) == GPIO_AF2_TIM5)       || ((AF) == GPIO_AF3_TIM8)       || \
                          ((AF) == GPIO_AF4_I2C1)       || ((AF) == GPIO_AF4_I2C2)       || \
                          ((AF) == GPIO_AF4_I2C3)       || ((AF) == GPIO_AF5_SPI1)       || \
                          ((AF) == GPIO_AF5_SPI2)       || ((AF) == GPIO_AF9_TIM13)      || \
                          ((AF) == GPIO_AF6_SPI3)       || ((AF) == GPIO_AF9_TIM12)      || \
                          ((AF) == GPIO_AF7_USART1)     || ((AF) == GPIO_AF7_USART2)     || \
                          ((AF) == GPIO_AF7_USART3)     || ((AF) == GPIO_AF8_UART4)      || \
                          ((AF) == GPIO_AF8_UART5)      || ((AF) == GPIO_AF8_USART6)     || \
                          ((AF) == GPIO_AF9_CAN1)       || ((AF) == GPIO_AF9_CAN2)       || \
                          ((AF) == GPIO_AF10_OTG_FS)    || ((AF) == GPIO_AF10_OTG_HS)    || \
                          ((AF) == GPIO_AF12_OTG_HS_FS) || ((AF) == GPIO_AF12_SDIO)      || \
                          ((AF) == GPIO_AF12_FSMC)      || ((AF) == GPIO_AF15_EVENTOUT))
#endif /* STM32F207xx || STM32F217xx */

/**
  * @}
  */ 

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup GPIOEx_Private_Functions GPIO Private Functions
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
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32F2xx_HAL_GPIO_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
