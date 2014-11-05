/**
  ******************************************************************************
  * @file    stm32l0xx_hal_gpio_ex.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-June-2014
  * @brief   Header file of GPIO HAL Extension module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L0xx_HAL_GPIO_EX_H
#define __STM32L0xx_HAL_GPIO_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal_def.h"

/** @addtogroup STM32L0xx_HAL_Driver
  * @{
  */

/** @addtogroup GPIOEx
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/** @defgroup GPIOEx_Exported_Constants
  * @{
  */ 
  
/** @defgroup GPIOEx_Alternate_function_selection
  * @{
  */
  
/*------------------------- STM32L053xx/STM32L063xx---------------------------*/ 
#if defined (STM32L053xx) || defined (STM32L063xx)
/** 
  * @brief   AF 0 selection  
  */ 
#define GPIO_AF0_SPI1          ((uint8_t)0x00)  /* SPI1 Alternate Function mapping     */
#define GPIO_AF0_SPI2          ((uint8_t)0x00)  /* SPI2 Alternate Function mapping     */
#define GPIO_AF0_USART1        ((uint8_t)0x00)  /* USART1 Alternate Function mapping   */
#define GPIO_AF0_USART2        ((uint8_t)0x00)  /* USART2 Alternate Function mapping   */
#define GPIO_AF0_LPUART1       ((uint8_t)0x00)  /* LPUART1 Alternate Function mapping  */
#define GPIO_AF0_USB           ((uint8_t)0x00)  /* USB Alternate Function mapping      */
#define GPIO_AF0_LPTIM         ((uint8_t)0x00)  /* LPTIM Alternate Function mapping    */
#define GPIO_AF0_TSC           ((uint8_t)0x00)  /* TSC Alternate Function mapping      */
#define GPIO_AF0_TIM2          ((uint8_t)0x00)  /* TIM2 Alternate Function mapping     */
#define GPIO_AF0_TIM21         ((uint8_t)0x00)  /* TIM21 Alternate Function mapping    */
#define GPIO_AF0_TIM22         ((uint8_t)0x00)  /* TIM22 Alternate Function mapping    */
#define GPIO_AF0_EVENTOUT      ((uint8_t)0x00)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /* MCO Alternate Function mapping      */
#define GPIO_AF0_SWDIO         ((uint8_t)0x00)  /* SWDIO Alternate Function mapping    */
#define GPIO_AF0_SWCLK         ((uint8_t)0x00)  /* SWCLK Alternate Function mapping    */

/** 
  * @brief   AF 1 selection  
  */ 
#define GPIO_AF1_SPI1          ((uint8_t)0x01)  /* SPI1 Alternate Function mapping  */
#define GPIO_AF1_SPI2          ((uint8_t)0x01)  /* SPI2 Alternate Function mapping  */
#define GPIO_AF1_I2C1          ((uint8_t)0x01)  /* I2C1 Alternate Function mapping  */
#define GPIO_AF1_LCD           ((uint8_t)0x01)  /* LCD Alternate Function mapping   */
#define GPIO_AF1_TIM2          ((uint8_t)0x01)  /* TIM2 Alternate Function mapping  */
#define GPIO_AF1_TIM21         ((uint8_t)0x01)  /* TIM21 Alternate Function mapping */
/** 
  * @brief   AF 2 selection  
  */ 
#define GPIO_AF2_SPI2          ((uint8_t)0x02)  /* SPI2 Alternate Function mapping       */
#define GPIO_AF2_LPUART1       ((uint8_t)0x02)  /* LPUART1 Alternate Function mapping    */
#define GPIO_AF2_USB           ((uint8_t)0x02)  /* USB Alternate Function mapping        */
#define GPIO_AF2_LPTIM         ((uint8_t)0x02)  /* LPTIM Alternate Function mapping      */
#define GPIO_AF2_TIM2          ((uint8_t)0x02)  /* TIM2 Alternate Function mapping       */
#define GPIO_AF2_TIM22         ((uint8_t)0x02)  /* TIM22 Alternate Function mapping      */
#define GPIO_AF2_EVENTOUT      ((uint8_t)0x02)  /* EVENTOUT Alternate Function mapping   */
#define GPIO_AF2_RTC_50Hz      ((uint8_t)0x02)  /* RTC_OUT Alternate Function mapping    */

/** 
  * @brief   AF 3 selection  
  */ 
#define GPIO_AF3_I2C1          ((uint8_t)0x03)  /* I2C1 Alternate Function mapping     */
#define GPIO_AF3_TSC           ((uint8_t)0x03)  /* TSC  Alternate Function mapping     */
#define GPIO_AF3_EVENTOUT      ((uint8_t)0x03)  /* EVENTOUT Alternate Function mapping */

/** 
  * @brief   AF 4 selection  
  */ 
#define GPIO_AF4_I2C1            ((uint8_t)0x04)  /* I2C1 Alternate Function mapping     */
#define GPIO_AF4_USART1          ((uint8_t)0x04)  /* USART1 Alternate Function mapping   */
#define GPIO_AF4_USART2          ((uint8_t)0x04)  /* USART2 Alternate Function mapping   */
#define GPIO_AF4_LPUART1         ((uint8_t)0x04)  /* LPUART1 Alternate Function mapping  */
#define GPIO_AF4_TIM22           ((uint8_t)0x04)  /* TIM22 Alternate Function mapping    */
#define GPIO_AF4_EVENTOUT        ((uint8_t)0x04)  /* EVENTOUT Alternate Function mapping */

/** 
  * @brief   AF 5 selection  
  */ 
#define GPIO_AF5_SPI2          ((uint8_t)0x05)  /* SPI2 Alternate Function mapping     */
#define GPIO_AF5_I2C2          ((uint8_t)0x05)  /* I2C2 Alternate Function mapping     */
#define GPIO_AF5_TIM2          ((uint8_t)0x05)  /* TIM2 Alternate Function mapping     */
#define GPIO_AF5_TIM21         ((uint8_t)0x05)  /* TIM21 Alternate Function mapping    */
#define GPIO_AF5_TIM22         ((uint8_t)0x05)  /* TIM22 Alternate Function mapping    */

/** 
  * @brief   AF 6 selection  
  */ 
#define GPIO_AF6_I2C2          ((uint8_t)0x06)  /* I2C2 Alternate Function mapping      */
#define GPIO_AF6_TIM21         ((uint8_t)0x06)  /* TIM21 Alternate Function mapping     */
#define GPIO_AF6_EVENTOUT      ((uint8_t)0x06)  /* EVENTOUT Alternate Function mapping  */

/** 
  * @brief   AF 7 selection  
  */ 
#define GPIO_AF7_COMP1        ((uint8_t)0x07)  /* COMP1 Alternate Function mapping     */
#define GPIO_AF7_COMP2        ((uint8_t)0x07)  /* COMP2 Alternate Function mapping     */


#define IS_GPIO_AF(AF)   (((AF) == GPIO_AF0_SPI1)         ||     ((AF) == GPIO_AF2_SPI2)        || \
                          ((AF) == GPIO_AF0_SPI2)         ||     ((AF) == GPIO_AF2_LPUART1)     || \
                          ((AF) == GPIO_AF0_USART1)       ||     ((AF) == GPIO_AF2_USB)         || \
                          ((AF) == GPIO_AF0_USART2)       ||     ((AF) == GPIO_AF2_LPTIM)       || \
                          ((AF) == GPIO_AF0_LPUART1)      ||     ((AF) == GPIO_AF2_TIM2)        || \
                          ((AF) == GPIO_AF0_USB)          ||     ((AF) == GPIO_AF2_TIM22)       || \
                          ((AF) == GPIO_AF0_LPTIM)        ||     ((AF) == GPIO_AF2_EVENTOUT)    || \
                          ((AF) == GPIO_AF0_TSC)          ||     ((AF) == GPIO_AF2_RTC_50Hz)    || \
                          ((AF) == GPIO_AF0_TIM2)         ||     ((AF) == GPIO_AF3_I2C1)        || \
                          ((AF) == GPIO_AF0_TIM21)        ||     ((AF) == GPIO_AF3_TSC)         || \
                          ((AF) == GPIO_AF0_TIM22)        ||     ((AF) == GPIO_AF3_EVENTOUT)    || \
                          ((AF) == GPIO_AF0_EVENTOUT)     ||     ((AF) == GPIO_AF4_I2C1)        || \
                          ((AF) == GPIO_AF0_MCO)          ||     ((AF) == GPIO_AF4_USART1)      || \
                          ((AF) == GPIO_AF0_SWDIO)        ||     ((AF) == GPIO_AF0_SWCLK)       || \
                          ((AF) == GPIO_AF1_SPI1)         ||     ((AF) == GPIO_AF4_USART2)      || \
                          ((AF) == GPIO_AF1_SPI2)         ||     ((AF) == GPIO_AF4_LPUART1)     || \
                          ((AF) == GPIO_AF1_TIM2)         ||     ((AF) == GPIO_AF4_TIM22)       || \
                          ((AF) == GPIO_AF1_I2C1)         ||     ((AF) == GPIO_AF4_EVENTOUT)    || \
                          ((AF) == GPIO_AF1_LCD)          ||     ((AF) == GPIO_AF5_SPI2)        || \
                          ((AF) == GPIO_AF5_I2C2)         ||     ((AF) == GPIO_AF5_TIM2)        || \
                          ((AF) == GPIO_AF5_TIM21)        ||     ((AF) == GPIO_AF5_TIM22)       || \
                          ((AF) == GPIO_AF6_I2C2)         ||     ((AF) == GPIO_AF6_TIM21)       || \
                          ((AF) == GPIO_AF6_EVENTOUT)     ||     ((AF) == GPIO_AF7_COMP1)       || \
                          ((AF) == GPIO_AF7_COMP2)        ||     ((AF) == GPIO_AF1_TIM21))

#endif /* STM32L053xx || STM32L063xx */
/*------------------------------------------------------------------------------------------*/

/*------------------------- STM32L052xx/STM32L062xx---------------------------*/ 
#if defined (STM32L052xx) || defined (STM32L062xx)
/** 
  * @brief   AF 0 selection  
  */ 
#define GPIO_AF0_SPI1          ((uint8_t)0x00)  /* SPI1 Alternate Function mapping               */
#define GPIO_AF0_SPI2          ((uint8_t)0x00)  /* SPI2 Alternate Function mapping               */
#define GPIO_AF0_USART1        ((uint8_t)0x00)  /* USART1 Alternate Function mapping             */
#define GPIO_AF0_USART2        ((uint8_t)0x00)  /* USART2 Alternate Function mapping             */
#define GPIO_AF0_LPUART1       ((uint8_t)0x00)  /* LPUART1 Alternate Function mapping            */
#define GPIO_AF0_USB           ((uint8_t)0x00)  /* USB Alternate Function mapping                */
#define GPIO_AF0_LPTIM         ((uint8_t)0x00)  /* LPTIM Alternate Function mapping              */
#define GPIO_AF0_TSC           ((uint8_t)0x00)  /* TSC Alternate Function mapping                */
#define GPIO_AF0_TIM2          ((uint8_t)0x00)  /* TIM2 Alternate Function mapping               */
#define GPIO_AF0_TIM21         ((uint8_t)0x00)  /* TIM21 Alternate Function mapping              */
#define GPIO_AF0_TIM22         ((uint8_t)0x00)  /* TIM22 Alternate Function mapping              */
#define GPIO_AF0_EVENTOUT      ((uint8_t)0x00)  /* EVENTOUT Alternate Function mapping           */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /* MCO Alternate Function mapping                */
#define GPIO_AF0_SWDIO         ((uint8_t)0x00)  /* SWDIO Alternate Function mapping              */
#define GPIO_AF0_SWCLK         ((uint8_t)0x00)  /* SWCLK Alternate Function mapping              */


/** 
  * @brief   AF 1 selection  
  */ 
#define GPIO_AF1_SPI1          ((uint8_t)0x01)  /* SPI1 Alternate Function mapping */
#define GPIO_AF1_SPI2          ((uint8_t)0x01)  /* SPI2 Alternate Function mapping */
#define GPIO_AF1_I2C1          ((uint8_t)0x01)  /* I2C1 Alternate Function mapping */
#define GPIO_AF1_TIM2          ((uint8_t)0x01)  /* TIM2 Alternate Function mapping */
#define GPIO_AF1_TIM21         ((uint8_t)0x01)  /* TIM21 Alternate Function mapping */
/** 
  * @brief   AF 2 selection  
  */ 
#define GPIO_AF2_SPI2          ((uint8_t)0x02)  /* SPI2 Alternate Function mapping               */
#define GPIO_AF2_LPUART1       ((uint8_t)0x02)  /* LPUART1 Alternate Function mapping            */
#define GPIO_AF2_USB           ((uint8_t)0x02)  /* USB Alternate Function mapping                */
#define GPIO_AF2_LPTIM         ((uint8_t)0x02)  /* LPTIM Alternate Function mapping              */
#define GPIO_AF2_TIM2          ((uint8_t)0x02)  /* TIM2 Alternate Function mapping               */
#define GPIO_AF2_TIM22         ((uint8_t)0x02)  /* TIM22 Alternate Function mapping              */
#define GPIO_AF2_EVENTOUT      ((uint8_t)0x02)  /* EVENTOUT Alternate Function mapping           */
#define GPIO_AF2_RTC_50Hz      ((uint8_t)0x02)  /* RTC_OUT Alternate Function mapping            */

/** 
  * @brief   AF 3 selection  
  */ 
#define GPIO_AF3_I2C1          ((uint8_t)0x03)  /* I2C1 Alternate Function mapping     */
#define GPIO_AF3_TSC           ((uint8_t)0x03)  /* TSC  Alternate Function mapping     */
#define GPIO_AF3_EVENTOUT      ((uint8_t)0x03)  /* EVENTOUT Alternate Function mapping */

/** 
  * @brief   AF 4 selection  
  */ 
#define GPIO_AF4_I2C1            ((uint8_t)0x04)  /* I2C1 Alternate Function mapping     */
#define GPIO_AF4_USART1          ((uint8_t)0x04)  /* USART1 Alternate Function mapping   */
#define GPIO_AF4_USART2          ((uint8_t)0x04)  /* USART2 Alternate Function mapping   */
#define GPIO_AF4_LPUART1         ((uint8_t)0x04)  /* LPUART1 Alternate Function mapping  */
#define GPIO_AF4_TIM22           ((uint8_t)0x04)  /* TIM22 Alternate Function mapping    */
#define GPIO_AF4_EVENTOUT        ((uint8_t)0x04)  /* EVENTOUT Alternate Function mapping */

/** 
  * @brief   AF 5 selection  
  */ 
#define GPIO_AF5_SPI2          ((uint8_t)0x05)  /* SPI2 Alternate Function mapping     */
#define GPIO_AF5_I2C2          ((uint8_t)0x05)  /* I2C2 Alternate Function mapping     */
#define GPIO_AF5_TIM2          ((uint8_t)0x05)  /* TIM2 Alternate Function mapping     */
#define GPIO_AF5_TIM21         ((uint8_t)0x05)  /* TIM21 Alternate Function mapping    */
#define GPIO_AF5_TIM22         ((uint8_t)0x05)  /* TIM22 Alternate Function mapping    */

/** 
  * @brief   AF 6 selection  
  */ 
#define GPIO_AF6_I2C2          ((uint8_t)0x06)  /* I2C2 Alternate Function mapping      */
#define GPIO_AF6_TIM21         ((uint8_t)0x06)  /* TIM21 Alternate Function mapping     */
#define GPIO_AF6_EVENTOUT      ((uint8_t)0x06)  /* EVENTOUT Alternate Function mapping  */

/** 
  * @brief   AF 7 selection  
  */ 
#define GPIO_AF7_COMP1        ((uint8_t)0x07)  /* COMP1 Alternate Function mapping     */
#define GPIO_AF7_COMP2        ((uint8_t)0x07)  /* COMP2 Alternate Function mapping     */


#define IS_GPIO_AF(AF)   (((AF) == GPIO_AF0_SPI1)         ||     ((AF) == GPIO_AF2_SPI2)        || \
                          ((AF) == GPIO_AF0_SPI2)         ||     ((AF) == GPIO_AF2_LPUART1)     || \
                          ((AF) == GPIO_AF0_USART1)       ||     ((AF) == GPIO_AF2_USB)         || \
                          ((AF) == GPIO_AF0_USART2)       ||     ((AF) == GPIO_AF2_LPTIM)       || \
                          ((AF) == GPIO_AF0_LPUART1)      ||     ((AF) == GPIO_AF2_TIM2)        || \
                          ((AF) == GPIO_AF0_USB)          ||     ((AF) == GPIO_AF2_TIM22)       || \
                          ((AF) == GPIO_AF0_LPTIM)        ||     ((AF) == GPIO_AF2_EVENTOUT)    || \
                          ((AF) == GPIO_AF0_TSC)          ||     ((AF) == GPIO_AF2_RTC_50Hz)    || \
                          ((AF) == GPIO_AF0_TIM2)         ||     ((AF) == GPIO_AF3_I2C1)        || \
                          ((AF) == GPIO_AF0_TIM21)        ||     ((AF) == GPIO_AF3_TSC)         || \
                          ((AF) == GPIO_AF0_TIM22)        ||     ((AF) == GPIO_AF3_EVENTOUT)    || \
                          ((AF) == GPIO_AF0_EVENTOUT)     ||     ((AF) == GPIO_AF4_I2C1)        || \
                          ((AF) == GPIO_AF0_MCO)          ||     ((AF) == GPIO_AF4_USART1)      || \
                          ((AF) == GPIO_AF0_SWDIO)        ||     ((AF) == GPIO_AF0_SWCLK)       || \
                          ((AF) == GPIO_AF1_SPI1)         ||     ((AF) == GPIO_AF4_USART2)      || \
                          ((AF) == GPIO_AF1_SPI2)         ||     ((AF) == GPIO_AF4_LPUART1)     || \
                          ((AF) == GPIO_AF1_TIM2)         ||     ((AF) == GPIO_AF4_TIM22)       || \
                          ((AF) == GPIO_AF1_I2C1)         ||     ((AF) == GPIO_AF4_EVENTOUT)    || \
                          ((AF) == GPIO_AF6_EVENTOUT)     ||     ((AF) == GPIO_AF5_SPI2)        || \
                          ((AF) == GPIO_AF5_I2C2)         ||     ((AF) == GPIO_AF5_TIM2)        || \
                          ((AF) == GPIO_AF5_TIM21)        ||     ((AF) == GPIO_AF5_TIM22)       || \
                          ((AF) == GPIO_AF6_I2C2)         ||     ((AF) == GPIO_AF6_TIM21)       || \
                          ((AF) == GPIO_AF7_COMP2)        ||     ((AF) == GPIO_AF7_COMP1)       || \
                          ((AF) == GPIO_AF1_TIM21))

#endif /* STM32L052xx || STM32L062xx */
/*------------------------------------------------------------------------------------------*/

/*------------------------- STM32L051xx/STM32L061xx---------------------------*/ 
#if defined (STM32L051xx)|| defined (STM32L061xx)
/** 
  * @brief   AF 0 selection  
  */ 
#define GPIO_AF0_SPI1          ((uint8_t)0x00)  /* SPI1 Alternate Function mapping               */
#define GPIO_AF0_SPI2          ((uint8_t)0x00)  /* SPI2 Alternate Function mapping               */
#define GPIO_AF0_USART1        ((uint8_t)0x00)  /* USART1 Alternate Function mapping             */
#define GPIO_AF0_USART2        ((uint8_t)0x00)  /* USART2 Alternate Function mapping             */
#define GPIO_AF0_LPUART1       ((uint8_t)0x00)  /* LPUART1 Alternate Function mapping            */
#define GPIO_AF0_LPTIM         ((uint8_t)0x00)  /* LPTIM Alternate Function mapping              */
#define GPIO_AF0_TSC           ((uint8_t)0x00)  /* TSC Alternate Function mapping                */
#define GPIO_AF0_TIM2          ((uint8_t)0x00)  /* TIM2 Alternate Function mapping               */
#define GPIO_AF0_TIM21         ((uint8_t)0x00)  /* TIM21 Alternate Function mapping              */
#define GPIO_AF0_TIM22         ((uint8_t)0x00)  /* TIM22 Alternate Function mapping              */
#define GPIO_AF0_EVENTOUT      ((uint8_t)0x00)  /* EVENTOUT Alternate Function mapping           */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /* MCO Alternate Function mapping                */
#define GPIO_AF0_SWDIO         ((uint8_t)0x00)  /* SWDIO Alternate Function mapping              */
#define GPIO_AF0_SWCLK         ((uint8_t)0x00)  /* SWCLK Alternate Function mapping              */


/** 
  * @brief   AF 1 selection  
  */ 
#define GPIO_AF1_SPI1          ((uint8_t)0x01)  /* SPI1 Alternate Function mapping */
#define GPIO_AF1_SPI2          ((uint8_t)0x01)  /* SPI2 Alternate Function mapping */
#define GPIO_AF1_I2C1          ((uint8_t)0x01)  /* I2C1 Alternate Function mapping */
#define GPIO_AF1_TIM2          ((uint8_t)0x01)  /* TIM2 Alternate Function mapping */
#define GPIO_AF1_TIM21         ((uint8_t)0x01)  /* TIM21 Alternate Function mapping */
/** 
  * @brief   AF 2 selection  
  */ 
#define GPIO_AF2_SPI2          ((uint8_t)0x02)  /* SPI2 Alternate Function mapping               */
#define GPIO_AF2_LPUART1       ((uint8_t)0x02)  /* LPUART1 Alternate Function mapping            */
#define GPIO_AF2_LPTIM         ((uint8_t)0x02)  /* LPTIM Alternate Function mapping              */
#define GPIO_AF2_TIM2          ((uint8_t)0x02)  /* TIM2 Alternate Function mapping               */
#define GPIO_AF2_TIM22         ((uint8_t)0x02)  /* TIM22 Alternate Function mapping              */
#define GPIO_AF2_EVENTOUT      ((uint8_t)0x02)  /* EVENTOUT Alternate Function mapping           */
#define GPIO_AF2_RTC_50Hz      ((uint8_t)0x02)  /* RTC_OUT Alternate Function mapping            */

/** 
  * @brief   AF 3 selection  
  */ 
#define GPIO_AF3_I2C1          ((uint8_t)0x03)  /* I2C1 Alternate Function mapping     */
#define GPIO_AF3_TSC           ((uint8_t)0x03)  /* TSC  Alternate Function mapping     */
#define GPIO_AF3_EVENTOUT      ((uint8_t)0x03)  /* EVENTOUT Alternate Function mapping */


/** 
  * @brief   AF 4 selection  
  */ 
#define GPIO_AF4_I2C1            ((uint8_t)0x04)  /* I2C1 Alternate Function mapping     */
#define GPIO_AF4_USART1          ((uint8_t)0x04)  /* USART1 Alternate Function mapping   */
#define GPIO_AF4_USART2          ((uint8_t)0x04)  /* USART2 Alternate Function mapping   */
#define GPIO_AF4_LPUART1         ((uint8_t)0x04)  /* LPUART1 Alternate Function mapping  */
#define GPIO_AF4_TIM22           ((uint8_t)0x04)  /* TIM22 Alternate Function mapping    */
#define GPIO_AF4_EVENTOUT        ((uint8_t)0x04)  /* EVENTOUT Alternate Function mapping */

/** 
  * @brief   AF 5 selection  
  */ 
#define GPIO_AF5_SPI2          ((uint8_t)0x05)  /* SPI2 Alternate Function mapping     */
#define GPIO_AF5_I2C2          ((uint8_t)0x05)  /* I2C2 Alternate Function mapping     */
#define GPIO_AF5_TIM2          ((uint8_t)0x05)  /* TIM2 Alternate Function mapping     */
#define GPIO_AF5_TIM21         ((uint8_t)0x05)  /* TIM21 Alternate Function mapping    */
#define GPIO_AF5_TIM22         ((uint8_t)0x05)  /* TIM22 Alternate Function mapping    */

/** 
  * @brief   AF 6 selection  
  */ 
#define GPIO_AF6_I2C2          ((uint8_t)0x06)  /* I2C2 Alternate Function mapping      */
#define GPIO_AF6_TIM21         ((uint8_t)0x06)  /* TIM21 Alternate Function mapping     */
#define GPIO_AF6_EVENTOUT      ((uint8_t)0x06)  /* EVENTOUT Alternate Function mapping  */

/** 
  * @brief   AF 7 selection  
  */ 
#define GPIO_AF7_COMP1        ((uint8_t)0x07)  /* COMP1 Alternate Function mapping     */
#define GPIO_AF7_COMP2        ((uint8_t)0x07)  /* COMP2 Alternate Function mapping     */


#define IS_GPIO_AF(AF)   (((AF) == GPIO_AF0_SPI1)         ||     ((AF) == GPIO_AF2_SPI2)        || \
                          ((AF) == GPIO_AF0_SPI2)         ||     ((AF) == GPIO_AF2_LPUART1)     || \
                          ((AF) == GPIO_AF0_USART1)       ||     ((AF) == GPIO_AF2_TIM22)       || \
                          ((AF) == GPIO_AF0_USART2)       ||     ((AF) == GPIO_AF2_LPTIM)       || \
                          ((AF) == GPIO_AF0_LPUART1)      ||     ((AF) == GPIO_AF2_TIM2)        || \
                          ((AF) == GPIO_AF0_LPTIM)        ||     ((AF) == GPIO_AF2_EVENTOUT)    || \
                          ((AF) == GPIO_AF1_TIM21)        ||     ((AF) == GPIO_AF2_RTC_50Hz)    || \
                          ((AF) == GPIO_AF0_TIM2)         ||     ((AF) == GPIO_AF3_I2C1)        || \
                          ((AF) == GPIO_AF0_TIM21)        ||     ((AF) == GPIO_AF3_TSC)         || \
                          ((AF) == GPIO_AF0_TIM22)        ||     ((AF) == GPIO_AF3_EVENTOUT)    || \
                          ((AF) == GPIO_AF0_EVENTOUT)     ||     ((AF) == GPIO_AF4_I2C1)        || \
                          ((AF) == GPIO_AF0_MCO)          ||     ((AF) == GPIO_AF4_USART1)      || \
                          ((AF) == GPIO_AF0_SWDIO)        ||     ((AF) == GPIO_AF0_SWCLK)       || \
                          ((AF) == GPIO_AF1_SPI1)         ||     ((AF) == GPIO_AF4_USART2)      || \
                          ((AF) == GPIO_AF1_SPI2)         ||     ((AF) == GPIO_AF4_LPUART1)     || \
                          ((AF) == GPIO_AF1_TIM2)         ||     ((AF) == GPIO_AF4_TIM22)       || \
                          ((AF) == GPIO_AF1_I2C1)         ||     ((AF) == GPIO_AF4_EVENTOUT)    || \
                          ((AF) == GPIO_AF6_EVENTOUT)     ||     ((AF) == GPIO_AF5_SPI2)        || \
                          ((AF) == GPIO_AF5_I2C2)         ||     ((AF) == GPIO_AF5_TIM2)        || \
                          ((AF) == GPIO_AF5_TIM21)        ||     ((AF) == GPIO_AF5_TIM22)       || \
                          ((AF) == GPIO_AF6_I2C2)         ||     ((AF) == GPIO_AF6_TIM21)       || \
                          ((AF) == GPIO_AF7_COMP2)        ||     ((AF) == GPIO_AF7_COMP1))       
                         

#endif /* STM32L051xx/STM32L061xx*/

/* Aliases define maintained for legacy */
#define GPIO_AF0_EVENOUT      GPIO_AF0_EVENTOUT
#define GPIO_AF2_EVENOUT      GPIO_AF2_EVENTOUT
#define GPIO_AF3_EVENOUT      GPIO_AF3_EVENTOUT
#define GPIO_AF6_EVENOUT      GPIO_AF6_EVENTOUT
/*------------------------------------------------------------------------------------------*/
/**
  * @}
  */ 

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
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

#endif /* __STM32L0xx_HAL_GPIO_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
