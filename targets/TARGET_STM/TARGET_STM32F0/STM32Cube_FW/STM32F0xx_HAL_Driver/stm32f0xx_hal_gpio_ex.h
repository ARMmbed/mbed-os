/**
  ******************************************************************************
  * @file    stm32f0xx_hal_gpio_ex.h
  * @author  MCD Application Team
  * @brief   Header file of GPIO HAL Extension module.
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
#ifndef __STM32F0xx_HAL_GPIO_EX_H
#define __STM32F0xx_HAL_GPIO_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal_def.h"

/** @addtogroup STM32F0xx_HAL_Driver
  * @{
  */

/** @defgroup GPIOEx GPIOEx
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
  
#if defined (STM32F030x6)
/*------------------------- STM32F030x6---------------------------*/ 
/* AF 0 */
#define GPIO_AF0_EVENTOUT     ((uint8_t)0x00U)  /*!< AF0: EVENTOUT Alternate Function mapping  */
#define GPIO_AF0_MCO          ((uint8_t)0x00U)  /*!< AF0: MCO Alternate Function mapping       */
#define GPIO_AF0_SPI1         ((uint8_t)0x00U)  /*!< AF0: SPI1 Alternate Function mapping      */
#define GPIO_AF0_TIM17        ((uint8_t)0x00U)  /*!< AF0: TIM17 Alternate Function mapping     */
#define GPIO_AF0_SWDIO        ((uint8_t)0x00U)  /*!< AF0: SWDIO Alternate Function mapping     */
#define GPIO_AF0_SWCLK        ((uint8_t)0x00U)  /*!< AF0: SWCLK Alternate Function mapping     */
#define GPIO_AF0_TIM14        ((uint8_t)0x00U)  /*!< AF0: TIM14 Alternate Function mapping     */
#define GPIO_AF0_USART1       ((uint8_t)0x00U)  /*!< AF0: USART1 Alternate Function mapping    */
#define GPIO_AF0_IR           ((uint8_t)0x00U)  /*!< AF0: IR Alternate Function mapping        */
#define GPIO_AF0_TIM3         ((uint8_t)0x00U)  /*!< AF0: TIM3 Alternate Function mapping      */

/* AF 1 */
#define GPIO_AF1_TIM3         ((uint8_t)0x01U)  /*!< AF1: TIM3 Alternate Function mapping      */
#define GPIO_AF1_USART1       ((uint8_t)0x01U)  /*!< AF1: USART1 Alternate Function mapping    */
#define GPIO_AF1_EVENTOUT     ((uint8_t)0x01U)  /*!< AF1: EVENTOUT Alternate Function mapping  */
#define GPIO_AF1_I2C1         ((uint8_t)0x01U)  /*!< AF1: I2C1 Alternate Function mapping      */
#define GPIO_AF1_IR           ((uint8_t)0x01U)  /*!< AF1: IR Alternate Function mapping        */

/* AF 2 */
#define GPIO_AF2_TIM1         ((uint8_t)0x02U)  /*!< AF2: TIM1 Alternate Function mapping      */
#define GPIO_AF2_TIM16        ((uint8_t)0x02U)  /*!< AF2: TIM16 Alternate Function mapping     */
#define GPIO_AF2_TIM17        ((uint8_t)0x02U)  /*!< AF2: TIM17 Alternate Function mapping     */
#define GPIO_AF2_EVENTOUT     ((uint8_t)0x02U)  /*!< AF2: EVENTOUT Alternate Function mapping  */

/* AF 3 */
#define GPIO_AF3_EVENTOUT     ((uint8_t)0x03U)  /*!< AF3: EVENTOUT Alternate Function mapping  */
#define GPIO_AF3_I2C1         ((uint8_t)0x03U)  /*!< AF3: I2C1 Alternate Function mapping      */

/* AF 4 */
#define GPIO_AF4_TIM14        ((uint8_t)0x04U)  /*!< AF4: TIM14 Alternate Function mapping     */
#define GPIO_AF4_I2C1         ((uint8_t)0x04U)  /*!< AF4: I2C1 Alternate Function mapping      */

/* AF 5 */
#define GPIO_AF5_TIM16        ((uint8_t)0x05U)  /*!< AF5: TIM16 Alternate Function mapping     */
#define GPIO_AF5_TIM17        ((uint8_t)0x05U)  /*!< AF5: TIM17 Alternate Function mapping     */

/* AF 6 */
#define GPIO_AF6_EVENTOUT     ((uint8_t)0x06U)  /*!< AF6: EVENTOUT Alternate Function mapping  */

#define IS_GPIO_AF(AF)        ((AF) <= (uint8_t)0x06U)

#endif /* STM32F030x6 */

/*---------------------------------- STM32F030x8 -------------------------------------------*/
#if defined (STM32F030x8)
/* AF 0 */
#define GPIO_AF0_EVENTOUT     ((uint8_t)0x00U)  /*!< AF0: EVENTOUT Alternate Function mapping  */
#define GPIO_AF0_MCO          ((uint8_t)0x00U)  /*!< AF0: MCO Alternate Function mapping       */
#define GPIO_AF0_SPI1         ((uint8_t)0x00U)  /*!< AF0: SPI1 Alternate Function mapping      */
#define GPIO_AF0_SPI2         ((uint8_t)0x00U)  /*!< AF0: SPI2 Alternate Function mapping      */
#define GPIO_AF0_TIM15        ((uint8_t)0x00U)  /*!< AF0: TIM15 Alternate Function mapping     */
#define GPIO_AF0_TIM17        ((uint8_t)0x00U)  /*!< AF0: TIM17 Alternate Function mapping     */
#define GPIO_AF0_SWDIO        ((uint8_t)0x00U)  /*!< AF0: SWDIO Alternate Function mapping     */
#define GPIO_AF0_SWCLK        ((uint8_t)0x00U)  /*!< AF0: SWCLK Alternate Function mapping     */
#define GPIO_AF0_TIM14        ((uint8_t)0x00U)  /*!< AF0: TIM14 Alternate Function mapping     */
#define GPIO_AF0_USART1       ((uint8_t)0x00U)  /*!< AF0: USART1 Alternate Function mapping    */
#define GPIO_AF0_IR           ((uint8_t)0x00U)  /*!< AF0: IR Alternate Function mapping        */
#define GPIO_AF0_TIM3         ((uint8_t)0x00U)  /*!< AF0: TIM3 Alternate Function mapping      */

/* AF 1 */
#define GPIO_AF1_TIM3         ((uint8_t)0x01U)  /*!< AF1: TIM3 Alternate Function mapping      */
#define GPIO_AF1_TIM15        ((uint8_t)0x01U)  /*!< AF1: TIM15 Alternate Function mapping     */
#define GPIO_AF1_USART1       ((uint8_t)0x01U)  /*!< AF1: USART1 Alternate Function mapping    */
#define GPIO_AF1_USART2       ((uint8_t)0x01U)  /*!< AF1: USART2 Alternate Function mapping    */
#define GPIO_AF1_EVENTOUT     ((uint8_t)0x01U)  /*!< AF1: EVENTOUT Alternate Function mapping  */
#define GPIO_AF1_I2C1         ((uint8_t)0x01U)  /*!< AF1: I2C1 Alternate Function mapping      */
#define GPIO_AF1_I2C2         ((uint8_t)0x01U)  /*!< AF1: I2C2 Alternate Function mapping      */
#define GPIO_AF1_IR           ((uint8_t)0x01U)  /*!< AF1: IR Alternate Function mapping        */

/* AF 2 */
#define GPIO_AF2_TIM1         ((uint8_t)0x02U)  /*!< AF2: TIM1 Alternate Function mapping      */
#define GPIO_AF2_TIM16        ((uint8_t)0x02U)  /*!< AF2: TIM16 Alternate Function mapping     */
#define GPIO_AF2_TIM17        ((uint8_t)0x02U)  /*!< AF2: TIM17 Alternate Function mapping     */
#define GPIO_AF2_EVENTOUT     ((uint8_t)0x02U)  /*!< AF2: EVENTOUT Alternate Function mapping  */

/* AF 3 */
#define GPIO_AF3_EVENTOUT     ((uint8_t)0x03U)  /*!< AF3: EVENTOUT Alternate Function mapping  */
#define GPIO_AF3_I2C1         ((uint8_t)0x03U)  /*!< AF3: I2C1 Alternate Function mapping      */
#define GPIO_AF3_TIM15        ((uint8_t)0x03U)  /*!< AF3: TIM15 Alternate Function mapping     */

/* AF 4 */
#define GPIO_AF4_TIM14        ((uint8_t)0x04U)  /*!< AF4: TIM14 Alternate Function mapping     */

/* AF 5 */
#define GPIO_AF5_TIM16        ((uint8_t)0x05U)  /*!< AF5: TIM16 Alternate Function mapping     */
#define GPIO_AF5_TIM17        ((uint8_t)0x05U)  /*!< AF5: TIM17 Alternate Function mapping     */

/* AF 6 */
#define GPIO_AF6_EVENTOUT     ((uint8_t)0x06U)  /*!< AF6: EVENTOUT Alternate Function mapping  */

#define IS_GPIO_AF(AF)        ((AF) <= (uint8_t)0x06U)

#endif /* STM32F030x8 */

#if defined (STM32F031x6) || defined (STM32F038xx)
/*--------------------------- STM32F031x6/STM32F038xx ---------------------------*/
/* AF 0 */
#define GPIO_AF0_EVENTOUT     ((uint8_t)0x00U)  /*!< AF0: EVENTOUT Alternate Function mapping  */
#define GPIO_AF0_MCO          ((uint8_t)0x00U)  /*!< AF0: MCO Alternate Function mapping       */
#define GPIO_AF0_SPI1         ((uint8_t)0x00U)  /*!< AF0: SPI1/I2S1 Alternate Function mapping */
#define GPIO_AF0_TIM17        ((uint8_t)0x00U)  /*!< AF0: TIM17 Alternate Function mapping     */
#define GPIO_AF0_SWDAT        ((uint8_t)0x00U)  /*!< AF0: SWDAT Alternate Function mapping     */
#define GPIO_AF0_SWCLK        ((uint8_t)0x00U)  /*!< AF0: SWCLK Alternate Function mapping     */
#define GPIO_AF0_TIM14        ((uint8_t)0x00U)  /*!< AF0: TIM14 Alternate Function mapping     */
#define GPIO_AF0_USART1       ((uint8_t)0x00U)  /*!< AF0: USART1 Alternate Function mapping    */
#define GPIO_AF0_IR           ((uint8_t)0x00U)  /*!< AF0: IR Alternate Function mapping        */

/* AF 1 */
#define GPIO_AF1_TIM3         ((uint8_t)0x01U)  /*!< AF1: TIM3 Alternate Function mapping      */
#define GPIO_AF1_USART1       ((uint8_t)0x01U)  /*!< AF1: USART1 Alternate Function mapping    */
#define GPIO_AF1_IR           ((uint8_t)0x01U)  /*!< AF1: IR Alternate Function mapping        */
#define GPIO_AF1_EVENTOUT     ((uint8_t)0x01U)  /*!< AF1: EVENTOUT Alternate Function mapping  */
#define GPIO_AF1_I2C1         ((uint8_t)0x01U)  /*!< AF1: I2C1 Alternate Function mapping      */

/* AF 2 */
#define GPIO_AF2_TIM1         ((uint8_t)0x02U)  /*!< AF2: TIM1 Alternate Function mapping      */
#define GPIO_AF2_TIM2         ((uint8_t)0x02U)  /*!< AF2: TIM2 Alternate Function mapping      */
#define GPIO_AF2_TIM16        ((uint8_t)0x02U)  /*!< AF2: TIM16 Alternate Function mapping     */
#define GPIO_AF2_TIM17        ((uint8_t)0x02U)  /*!< AF2: TIM17 Alternate Function mapping     */
#define GPIO_AF2_EVENTOUT     ((uint8_t)0x02U)  /*!< AF2: EVENTOUT Alternate Function mapping  */

/* AF 3 */
#define GPIO_AF3_EVENTOUT     ((uint8_t)0x03U)  /*!< AF3: EVENTOUT Alternate Function mapping  */
#define GPIO_AF3_I2C1         ((uint8_t)0x03U)  /*!< AF3: I2C1 Alternate Function mapping      */

/* AF 4 */
#define GPIO_AF4_TIM14        ((uint8_t)0x04U)  /*!< AF4: TIM14 Alternate Function mapping     */
#define GPIO_AF4_I2C1         ((uint8_t)0x04U)  /*!< AF4: I2C1 Alternate Function mapping      */

/* AF 5 */
#define GPIO_AF5_TIM16        ((uint8_t)0x05U)  /*!< AF5: TIM16 Alternate Function mapping     */
#define GPIO_AF5_TIM17        ((uint8_t)0x05U)  /*!< AF5: TIM17 Alternate Function mapping     */

/* AF 6 */
#define GPIO_AF6_EVENTOUT     ((uint8_t)0x06U)  /*!< AF6: EVENTOUT Alternate Function mapping  */

#define IS_GPIO_AF(AF)        ((AF) <= (uint8_t)0x06U)

#endif /* STM32F031x6 || STM32F038xx */

#if defined (STM32F051x8) || defined (STM32F058xx)
/*--------------------------- STM32F051x8/STM32F058xx---------------------------*/
/* AF 0 */
#define GPIO_AF0_EVENTOUT     ((uint8_t)0x00U)  /*!< AF0: EVENTOUT Alternate Function mapping  */
#define GPIO_AF0_MCO          ((uint8_t)0x00U)  /*!< AF0: MCO Alternate Function mapping       */
#define GPIO_AF0_SPI1         ((uint8_t)0x00U)  /*!< AF0: SPI1/I2S1 Alternate Function mapping */
#define GPIO_AF0_SPI2         ((uint8_t)0x00U)  /*!< AF0: SPI2 Alternate Function mapping      */
#define GPIO_AF0_TIM15        ((uint8_t)0x00U)  /*!< AF0: TIM15 Alternate Function mapping     */
#define GPIO_AF0_TIM17        ((uint8_t)0x00U)  /*!< AF0: TIM17 Alternate Function mapping     */
#define GPIO_AF0_SWDIO        ((uint8_t)0x00U)  /*!< AF0: SWDIO Alternate Function mapping     */
#define GPIO_AF0_SWCLK        ((uint8_t)0x00U)  /*!< AF0: SWCLK Alternate Function mapping     */
#define GPIO_AF0_TIM14        ((uint8_t)0x00U)  /*!< AF0: TIM14 Alternate Function mapping     */
#define GPIO_AF0_USART1       ((uint8_t)0x00U)  /*!< AF0: USART1 Alternate Function mapping    */
#define GPIO_AF0_IR           ((uint8_t)0x00U)  /*!< AF0: IR Alternate Function mapping        */
#define GPIO_AF0_CEC          ((uint8_t)0x00U)  /*!< AF0: CEC Alternate Function mapping       */

/* AF 1 */
#define GPIO_AF1_TIM3         ((uint8_t)0x01U)  /*!< AF1: TIM3 Alternate Function mapping      */
#define GPIO_AF1_TIM15        ((uint8_t)0x01U)  /*!< AF1: TIM15 Alternate Function mapping     */
#define GPIO_AF1_USART1       ((uint8_t)0x01U)  /*!< AF1: USART1 Alternate Function mapping    */
#define GPIO_AF1_USART2       ((uint8_t)0x01U)  /*!< AF1: USART2 Alternate Function mapping    */
#define GPIO_AF1_EVENTOUT     ((uint8_t)0x01U)  /*!< AF1: EVENTOUT Alternate Function mapping  */
#define GPIO_AF1_I2C1         ((uint8_t)0x01U)  /*!< AF1: I2C1 Alternate Function mapping      */
#define GPIO_AF1_I2C2         ((uint8_t)0x01U)  /*!< AF1: I2C2 Alternate Function mapping      */
#define GPIO_AF1_IR           ((uint8_t)0x01U)  /*!< AF1: IR Alternate Function mapping        */
#define GPIO_AF1_CEC          ((uint8_t)0x01U)  /*!< AF1: CEC Alternate Function mapping       */

/* AF 2 */
#define GPIO_AF2_TIM1         ((uint8_t)0x02U)  /*!< AF2: TIM1 Alternate Function mapping      */
#define GPIO_AF2_TIM2         ((uint8_t)0x02U)  /*!< AF2: TIM2 Alternate Function mapping      */
#define GPIO_AF2_TIM16        ((uint8_t)0x02U)  /*!< AF2: TIM16 Alternate Function mapping     */
#define GPIO_AF2_TIM17        ((uint8_t)0x02U)  /*!< AF2: TIM17 Alternate Function mapping     */
#define GPIO_AF2_EVENTOUT     ((uint8_t)0x02U)  /*!< AF2: EVENTOUT Alternate Function mapping  */

/* AF 3 */
#define GPIO_AF3_EVENTOUT     ((uint8_t)0x03U)  /*!< AF3: EVENTOUT Alternate Function mapping  */
#define GPIO_AF3_I2C1         ((uint8_t)0x03U)  /*!< AF3: I2C1 Alternate Function mapping      */
#define GPIO_AF3_TIM15        ((uint8_t)0x03U)  /*!< AF3: TIM15 Alternate Function mapping     */
#define GPIO_AF3_TSC          ((uint8_t)0x03U)  /*!< AF3: TSC Alternate Function mapping       */

/* AF 4 */
#define GPIO_AF4_TIM14        ((uint8_t)0x04U)  /*!< AF4: TIM14 Alternate Function mapping     */

/* AF 5 */
#define GPIO_AF5_TIM16        ((uint8_t)0x05U)  /*!< AF5: TIM16 Alternate Function mapping     */
#define GPIO_AF5_TIM17        ((uint8_t)0x05U)  /*!< AF5: TIM17 Alternate Function mapping     */

/* AF 6 */
#define GPIO_AF6_EVENTOUT     ((uint8_t)0x06U)  /*!< AF6: EVENTOUT Alternate Function mapping  */

/* AF 7 */
#define GPIO_AF7_COMP1        ((uint8_t)0x07U)  /*!< AF7: COMP1 Alternate Function mapping     */
#define GPIO_AF7_COMP2        ((uint8_t)0x07U)  /*!< AF7: COMP2 Alternate Function mapping     */

#define IS_GPIO_AF(AF)        ((AF) <= (uint8_t)0x07U)

#endif /* STM32F051x8/STM32F058xx */

#if defined (STM32F071xB)
/*--------------------------- STM32F071xB ---------------------------*/
/* AF 0 */ 
#define GPIO_AF0_EVENTOUT     ((uint8_t)0x00U)  /*!< AF0: AEVENTOUT Alternate Function mapping */
#define GPIO_AF0_SWDIO        ((uint8_t)0x00U)  /*!< AF0: SWDIO Alternate Function mapping     */
#define GPIO_AF0_SWCLK        ((uint8_t)0x00U)  /*!< AF0: SWCLK Alternate Function mapping     */
#define GPIO_AF0_MCO          ((uint8_t)0x00U)  /*!< AF0: MCO Alternate Function mapping       */
#define GPIO_AF0_CEC          ((uint8_t)0x00U)  /*!< AF0: CEC Alternate Function mapping       */
#define GPIO_AF0_CRS          ((uint8_t)0x00U)  /*!< AF0: CRS Alternate Function mapping       */
#define GPIO_AF0_IR           ((uint8_t)0x00U)  /*!< AF0: IR Alternate Function mapping        */
#define GPIO_AF0_SPI1         ((uint8_t)0x00U)  /*!< AF0: SPI1/I2S1 Alternate Function mapping */
#define GPIO_AF0_SPI2         ((uint8_t)0x00U)  /*!< AF0: SPI2/I2S2 Alternate Function mapping */
#define GPIO_AF0_TIM1         ((uint8_t)0x00U)  /*!< AF0: TIM1 Alternate Function mapping      */
#define GPIO_AF0_TIM3         ((uint8_t)0x00U)  /*!< AF0: TIM3 Alternate Function mapping      */
#define GPIO_AF0_TIM14        ((uint8_t)0x00U)  /*!< AF0: TIM14 Alternate Function mapping     */
#define GPIO_AF0_TIM15        ((uint8_t)0x00U)  /*!< AF0: TIM15 Alternate Function mapping     */
#define GPIO_AF0_TIM16        ((uint8_t)0x00U)  /*!< AF0: TIM16 Alternate Function mapping     */
#define GPIO_AF0_TIM17        ((uint8_t)0x00U)  /*!< AF0: TIM17 Alternate Function mapping     */
#define GPIO_AF0_TSC          ((uint8_t)0x00U)  /*!< AF0: TSC Alternate Function mapping       */
#define GPIO_AF0_USART1       ((uint8_t)0x00U)  /*!< AF0: USART1 Alternate Function mapping    */
#define GPIO_AF0_USART2       ((uint8_t)0x00U)  /*!< AF0: USART2 Alternate Function mapping    */
#define GPIO_AF0_USART3       ((uint8_t)0x00U)  /*!< AF0: USART3 Alternate Function mapping    */
#define GPIO_AF0_USART4       ((uint8_t)0x00U)  /*!< AF0: USART4 Alternate Function mapping    */

/* AF 1 */
#define GPIO_AF1_TIM3         ((uint8_t)0x01U)  /*!< AF1: TIM3 Alternate Function mapping      */
#define GPIO_AF1_TIM15        ((uint8_t)0x01U)  /*!< AF1: TIM15 Alternate Function mapping     */
#define GPIO_AF1_USART1       ((uint8_t)0x01U)  /*!< AF1: USART1 Alternate Function mapping    */
#define GPIO_AF1_USART2       ((uint8_t)0x01U)  /*!< AF1: USART2 Alternate Function mapping    */
#define GPIO_AF1_USART3       ((uint8_t)0x01U)  /*!< AF1: USART3 Alternate Function mapping    */
#define GPIO_AF1_IR           ((uint8_t)0x01U)  /*!< AF1: IR Alternate Function mapping        */
#define GPIO_AF1_CEC          ((uint8_t)0x01U)  /*!< AF1: CEC Alternate Function mapping       */
#define GPIO_AF1_EVENTOUT     ((uint8_t)0x01U)  /*!< AF1: EVENTOUT Alternate Function mapping  */
#define GPIO_AF1_I2C1         ((uint8_t)0x01U)  /*!< AF1: I2C1 Alternate Function mapping      */
#define GPIO_AF1_I2C2         ((uint8_t)0x01U)  /*!< AF1: I2C2 Alternate Function mapping      */
#define GPIO_AF1_TSC          ((uint8_t)0x01U)  /*!< AF1: TSC Alternate Function mapping       */
#define GPIO_AF1_SPI1         ((uint8_t)0x01U)  /*!< AF1: SPI1 Alternate Function mapping      */
#define GPIO_AF1_SPI2         ((uint8_t)0x01U)  /*!< AF1: SPI2 Alternate Function mapping      */

/* AF 2 */
#define GPIO_AF2_TIM1         ((uint8_t)0x02U)  /*!< AF2: TIM1 Alternate Function mapping      */
#define GPIO_AF2_TIM2         ((uint8_t)0x02U)  /*!< AF2: TIM2 Alternate Function mapping      */
#define GPIO_AF2_TIM16        ((uint8_t)0x02U)  /*!< AF2: TIM16 Alternate Function mapping     */
#define GPIO_AF2_TIM17        ((uint8_t)0x02U)  /*!< AF2: TIM17 Alternate Function mapping     */
#define GPIO_AF2_EVENTOUT     ((uint8_t)0x02U)  /*!< AF2: EVENTOUT Alternate Function mapping  */

/* AF 3 */
#define GPIO_AF3_EVENTOUT     ((uint8_t)0x03U)  /*!< AF3: EVENTOUT Alternate Function mapping  */
#define GPIO_AF3_TSC          ((uint8_t)0x03U)  /*!< AF3: TSC Alternate Function mapping       */
#define GPIO_AF3_TIM15        ((uint8_t)0x03U)  /*!< AF3: TIM15 Alternate Function mapping     */
#define GPIO_AF3_I2C1         ((uint8_t)0x03U)  /*!< AF3: I2C1 Alternate Function mapping      */

/* AF 4 */
#define GPIO_AF4_TIM14        ((uint8_t)0x04U)  /*!< AF4: TIM14 Alternate Function mapping     */
#define GPIO_AF4_USART4       ((uint8_t)0x04U)  /*!< AF4: USART4 Alternate Function mapping    */
#define GPIO_AF4_USART3       ((uint8_t)0x04U)  /*!< AF4: USART3 Alternate Function mapping    */
#define GPIO_AF4_CRS          ((uint8_t)0x04U)  /*!< AF4: CRS Alternate Function mapping       */

/* AF 5 */
#define GPIO_AF5_TIM15        ((uint8_t)0x05U)  /*!< AF5: TIM15 Alternate Function mapping     */
#define GPIO_AF5_TIM16        ((uint8_t)0x05U)  /*!< AF5: TIM16 Alternate Function mapping     */
#define GPIO_AF5_TIM17        ((uint8_t)0x05U)  /*!< AF5: TIM17 Alternate Function mapping     */
#define GPIO_AF5_SPI2         ((uint8_t)0x05U)  /*!< AF5: SPI2 Alternate Function mapping      */
#define GPIO_AF5_I2C2         ((uint8_t)0x05U)  /*!< AF5: I2C2 Alternate Function mapping      */

/* AF 6 */
#define GPIO_AF6_EVENTOUT     ((uint8_t)0x06U)  /*!< AF6: EVENTOUT Alternate Function mapping  */

/* AF 7 */
#define GPIO_AF7_COMP1        ((uint8_t)0x07U)  /*!< AF7: COMP1 Alternate Function mapping     */
#define GPIO_AF7_COMP2        ((uint8_t)0x07U)  /*!< AF7: COMP2 Alternate Function mapping     */

#define IS_GPIO_AF(AF)        ((AF) <= (uint8_t)0x07U)

#endif /* STM32F071xB */


#if defined(STM32F091xC) || defined(STM32F098xx)
/*--------------------------- STM32F091xC || STM32F098xx ------------------------------*/
/* AF 0 */
#define GPIO_AF0_EVENTOUT     ((uint8_t)0x00U)  /*!< AF0: EVENTOUT Alternate Function mapping  */
#define GPIO_AF0_SWDIO        ((uint8_t)0x00U)  /*!< AF0: SWDIO Alternate Function mapping     */
#define GPIO_AF0_SWCLK        ((uint8_t)0x00U)  /*!< AF0: SWCLK Alternate Function mapping     */
#define GPIO_AF0_MCO          ((uint8_t)0x00U)  /*!< AF0: MCO Alternate Function mapping       */
#define GPIO_AF0_CEC          ((uint8_t)0x00U)  /*!< AF0: CEC Alternate Function mapping       */
#define GPIO_AF0_CRS          ((uint8_t)0x00U)  /*!< AF0: CRS Alternate Function mapping       */
#define GPIO_AF0_IR           ((uint8_t)0x00U)  /*!< AF0: IR Alternate Function mapping        */
#define GPIO_AF0_SPI1         ((uint8_t)0x00U)  /*!< AF0: SPI1/I2S1 Alternate Function mapping */
#define GPIO_AF0_SPI2         ((uint8_t)0x00U)  /*!< AF0: SPI2/I2S2 Alternate Function mapping */
#define GPIO_AF0_TIM1         ((uint8_t)0x00U)  /*!< AF0: TIM1 Alternate Function mapping      */
#define GPIO_AF0_TIM3         ((uint8_t)0x00U)  /*!< AF0: TIM3 Alternate Function mapping      */
#define GPIO_AF0_TIM14        ((uint8_t)0x00U)  /*!< AF0: TIM14 Alternate Function mapping     */
#define GPIO_AF0_TIM15        ((uint8_t)0x00U)  /*!< AF0: TIM15 Alternate Function mapping     */
#define GPIO_AF0_TIM16        ((uint8_t)0x00U)  /*!< AF0: TIM16 Alternate Function mapping     */
#define GPIO_AF0_TIM17        ((uint8_t)0x00U)  /*!< AF0: TIM17 Alternate Function mapping     */
#define GPIO_AF0_TSC          ((uint8_t)0x00U)  /*!< AF0: TSC Alternate Function mapping       */
#define GPIO_AF0_USART1       ((uint8_t)0x00U)  /*!< AF0: USART1 Alternate Function mapping    */
#define GPIO_AF0_USART2       ((uint8_t)0x00U)  /*!< AF0: USART2 Alternate Function mapping    */
#define GPIO_AF0_USART3       ((uint8_t)0x00U)  /*!< AF0: USART3 Alternate Function mapping    */
#define GPIO_AF0_USART4       ((uint8_t)0x00U)  /*!< AF0: USART4 Alternate Function mapping    */
#define GPIO_AF0_USART8       ((uint8_t)0x00U)  /*!< AF0: USART8 Alternate Function mapping    */
#define GPIO_AF0_CAN          ((uint8_t)0x00U)  /*!< AF0: CAN Alternate Function mapping       */

/* AF 1 */
#define GPIO_AF1_TIM3         ((uint8_t)0x01U)  /*!< AF1: TIM3 Alternate Function mapping      */
#define GPIO_AF1_TIM15        ((uint8_t)0x01U)  /*!< AF1: TIM15 Alternate Function mapping     */
#define GPIO_AF1_USART1       ((uint8_t)0x01U)  /*!< AF1: USART1 Alternate Function mapping    */
#define GPIO_AF1_USART2       ((uint8_t)0x01U)  /*!< AF1: USART2 Alternate Function mapping    */
#define GPIO_AF1_USART3       ((uint8_t)0x01U)  /*!< AF1: USART3 Alternate Function mapping    */
#define GPIO_AF1_USART4       ((uint8_t)0x01U)  /*!< AF1: USART4 Alternate Function mapping    */
#define GPIO_AF1_USART5       ((uint8_t)0x01U)  /*!< AF1: USART5 Alternate Function mapping    */
#define GPIO_AF1_USART6       ((uint8_t)0x01U)  /*!< AF1: USART6 Alternate Function mapping    */
#define GPIO_AF1_USART7       ((uint8_t)0x01U)  /*!< AF1: USART7 Alternate Function mapping    */
#define GPIO_AF1_USART8       ((uint8_t)0x01U)  /*!< AF1: USART8 Alternate Function mapping    */
#define GPIO_AF1_IR           ((uint8_t)0x01U)  /*!< AF1: IR Alternate Function mapping        */
#define GPIO_AF1_CEC          ((uint8_t)0x01U)  /*!< AF1: CEC Alternate Function mapping       */
#define GPIO_AF1_EVENTOUT     ((uint8_t)0x01U)  /*!< AF1: EVENTOUT Alternate Function mapping  */
#define GPIO_AF1_I2C1         ((uint8_t)0x01U)  /*!< AF1: I2C1 Alternate Function mapping      */
#define GPIO_AF1_I2C2         ((uint8_t)0x01U)  /*!< AF1: I2C2 Alternate Function mapping      */
#define GPIO_AF1_TSC          ((uint8_t)0x01U)  /*!< AF1: TSC Alternate Function mapping       */
#define GPIO_AF1_SPI1         ((uint8_t)0x01U)  /*!< AF1: SPI1 Alternate Function mapping      */
#define GPIO_AF1_SPI2         ((uint8_t)0x01U)  /*!< AF1: SPI2 Alternate Function mapping      */

/* AF 2 */
#define GPIO_AF2_TIM1         ((uint8_t)0x02U)  /*!< AF2: TIM1 Alternate Function mapping      */
#define GPIO_AF2_TIM2         ((uint8_t)0x02U)  /*!< AF2: TIM2 Alternate Function mapping      */
#define GPIO_AF2_TIM16        ((uint8_t)0x02U)  /*!< AF2: TIM16 Alternate Function mapping     */
#define GPIO_AF2_TIM17        ((uint8_t)0x02U)  /*!< AF2: TIM17 Alternate Function mapping     */
#define GPIO_AF2_EVENTOUT     ((uint8_t)0x02U)  /*!< AF2: EVENTOUT Alternate Function mapping  */
#define GPIO_AF2_USART5       ((uint8_t)0x02U)  /*!< AF2: USART5 Alternate Function mapping    */
#define GPIO_AF2_USART6       ((uint8_t)0x02U)  /*!< AF2: USART6 Alternate Function mapping    */
#define GPIO_AF2_USART7       ((uint8_t)0x02U)  /*!< AF2: USART7 Alternate Function mapping    */
#define GPIO_AF2_USART8       ((uint8_t)0x02U)  /*!< AF2: USART8 Alternate Function mapping    */

/* AF 3 */
#define GPIO_AF3_EVENTOUT     ((uint8_t)0x03U)  /*!< AF3: EVENTOUT Alternate Function mapping  */
#define GPIO_AF3_TSC          ((uint8_t)0x03U)  /*!< AF3: TSC Alternate Function mapping       */
#define GPIO_AF3_TIM15        ((uint8_t)0x03U)  /*!< AF3: TIM15 Alternate Function mapping     */
#define GPIO_AF3_I2C1         ((uint8_t)0x03U)  /*!< AF3: I2C1 Alternate Function mapping      */

/* AF 4 */
#define GPIO_AF4_TIM14        ((uint8_t)0x04U)  /*!< AF4: TIM14 Alternate Function mapping     */
#define GPIO_AF4_USART4       ((uint8_t)0x04U)  /*!< AF4: USART4 Alternate Function mapping    */
#define GPIO_AF4_USART3       ((uint8_t)0x04U)  /*!< AF4: USART3 Alternate Function mapping    */
#define GPIO_AF4_CRS          ((uint8_t)0x04U)  /*!< AF4: CRS Alternate Function mapping       */
#define GPIO_AF4_CAN          ((uint8_t)0x04U)  /*!< AF4: CAN Alternate Function mapping       */
#define GPIO_AF4_I2C1         ((uint8_t)0x04U)  /*!< AF4: I2C1 Alternate Function mapping      */
#define GPIO_AF4_USART5       ((uint8_t)0x04U)  /*!< AF4: USART5 Alternate Function mapping    */

/* AF 5 */
#define GPIO_AF5_TIM15        ((uint8_t)0x05U)  /*!< AF5: TIM15 Alternate Function mapping     */
#define GPIO_AF5_TIM16        ((uint8_t)0x05U)  /*!< AF5: TIM16 Alternate Function mapping     */
#define GPIO_AF5_TIM17        ((uint8_t)0x05U)  /*!< AF5: TIM17 Alternate Function mapping     */
#define GPIO_AF5_SPI2         ((uint8_t)0x05U)  /*!< AF5: SPI2 Alternate Function mapping      */
#define GPIO_AF5_I2C2         ((uint8_t)0x05U)  /*!< AF5: I2C2 Alternate Function mapping      */
#define GPIO_AF5_MCO          ((uint8_t)0x05U)  /*!< AF5: MCO Alternate Function mapping       */
#define GPIO_AF5_USART6       ((uint8_t)0x05U)  /*!< AF5: USART6 Alternate Function mapping    */

/* AF 6 */
#define GPIO_AF6_EVENTOUT     ((uint8_t)0x06U)  /*!< AF6: EVENTOUT Alternate Function mapping  */

/* AF 7 */
#define GPIO_AF7_COMP1        ((uint8_t)0x07U)  /*!< AF7: COMP1 Alternate Function mapping     */
#define GPIO_AF7_COMP2        ((uint8_t)0x07U)  /*!< AF7: COMP2 Alternate Function mapping     */

#define IS_GPIO_AF(AF)        ((AF) <= (uint8_t)0x07U)

#endif /* STM32F091xC  || STM32F098xx */

#if defined(STM32F030xC)
/*--------------------------- STM32F030xC ----------------------------------------------------*/
/* AF 0 */
#define GPIO_AF0_EVENTOUT     ((uint8_t)0x00U)  /*!< AF0: EVENTOUT Alternate Function mapping  */
#define GPIO_AF0_SWDIO        ((uint8_t)0x00U)  /*!< AF0: SWDIO Alternate Function mapping     */
#define GPIO_AF0_SWCLK        ((uint8_t)0x00U)  /*!< AF0: SWCLK Alternate Function mapping     */
#define GPIO_AF0_MCO          ((uint8_t)0x00U)  /*!< AF0: MCO Alternate Function mapping       */
#define GPIO_AF0_IR           ((uint8_t)0x00U)  /*!< AF0: IR Alternate Function mapping        */
#define GPIO_AF0_SPI1         ((uint8_t)0x00U)  /*!< AF0: SPI1 Alternate Function mapping      */
#define GPIO_AF0_SPI2         ((uint8_t)0x00U)  /*!< AF0: SPI2 Alternate Function mapping      */
#define GPIO_AF0_TIM3         ((uint8_t)0x00U)  /*!< AF0: TIM3 Alternate Function mapping      */
#define GPIO_AF0_TIM14        ((uint8_t)0x00U)  /*!< AF0: TIM14 Alternate Function mapping     */
#define GPIO_AF0_TIM15        ((uint8_t)0x00U)  /*!< AF0: TIM15 Alternate Function mapping     */
#define GPIO_AF0_TIM17        ((uint8_t)0x00U)  /*!< AF0: TIM17 Alternate Function mapping     */
#define GPIO_AF0_USART1       ((uint8_t)0x00U)  /*!< AF0: USART1 Alternate Function mapping    */
#define GPIO_AF0_USART4       ((uint8_t)0x00U)  /*!< AF0: USART4 Alternate Function mapping    */

/* AF 1 */
#define GPIO_AF1_TIM3         ((uint8_t)0x01U)  /*!< AF1: TIM3 Alternate Function mapping      */
#define GPIO_AF1_TIM15        ((uint8_t)0x01U)  /*!< AF1: TIM15 Alternate Function mapping     */
#define GPIO_AF1_USART1       ((uint8_t)0x01U)  /*!< AF1: USART1 Alternate Function mapping    */
#define GPIO_AF1_USART2       ((uint8_t)0x01U)  /*!< AF1: USART2 Alternate Function mapping    */
#define GPIO_AF1_USART3       ((uint8_t)0x01U)  /*!< AF1: USART3 Alternate Function mapping    */
#define GPIO_AF1_IR           ((uint8_t)0x01U)  /*!< AF1: IR Alternate Function mapping        */
#define GPIO_AF1_EVENTOUT     ((uint8_t)0x01U)  /*!< AF1: EVENTOUT Alternate Function mapping  */
#define GPIO_AF1_I2C1         ((uint8_t)0x01U)  /*!< AF1: I2C1 Alternate Function mapping      */
#define GPIO_AF1_I2C2         ((uint8_t)0x01U)  /*!< AF1: I2C2 Alternate Function mapping      */
#define GPIO_AF1_SPI2         ((uint8_t)0x01U)  /*!< AF1: SPI2 Alternate Function mapping      */

/* AF 2 */
#define GPIO_AF2_TIM1         ((uint8_t)0x02U)  /*!< AF2: TIM1 Alternate Function mapping      */
#define GPIO_AF2_TIM16        ((uint8_t)0x02U)  /*!< AF2: TIM16 Alternate Function mapping     */
#define GPIO_AF2_TIM17        ((uint8_t)0x02U)  /*!< AF2: TIM17 Alternate Function mapping     */
#define GPIO_AF2_EVENTOUT     ((uint8_t)0x02U)  /*!< AF2: EVENTOUT Alternate Function mapping  */
#define GPIO_AF2_USART5       ((uint8_t)0x02U)  /*!< AF2: USART5 Alternate Function mapping    */
#define GPIO_AF2_USART6       ((uint8_t)0x02U)  /*!< AF2: USART6 Alternate Function mapping    */

/* AF 3 */
#define GPIO_AF3_EVENTOUT     ((uint8_t)0x03U)  /*!< AF3: EVENTOUT Alternate Function mapping  */
#define GPIO_AF3_TIM15        ((uint8_t)0x03U)  /*!< AF3: TIM15 Alternate Function mapping     */
#define GPIO_AF3_I2C1         ((uint8_t)0x03U)  /*!< AF3: I2C1 Alternate Function mapping      */

/* AF 4 */
#define GPIO_AF4_TIM14        ((uint8_t)0x04U)  /*!< AF4: TIM14 Alternate Function mapping     */
#define GPIO_AF4_USART4       ((uint8_t)0x04U)  /*!< AF4: USART4 Alternate Function mapping    */
#define GPIO_AF4_USART3       ((uint8_t)0x04U)  /*!< AF4: USART3 Alternate Function mapping    */
#define GPIO_AF4_I2C1         ((uint8_t)0x04U)  /*!< AF4: I2C1 Alternate Function mapping      */
#define GPIO_AF4_USART5       ((uint8_t)0x04U)  /*!< AF4: USART5 Alternate Function mapping    */

/* AF 5 */
#define GPIO_AF5_TIM15        ((uint8_t)0x05U)  /*!< AF5: TIM15 Alternate Function mapping     */
#define GPIO_AF5_TIM16        ((uint8_t)0x05U)  /*!< AF5: TIM16 Alternate Function mapping     */
#define GPIO_AF5_TIM17        ((uint8_t)0x05U)  /*!< AF5: TIM17 Alternate Function mapping     */
#define GPIO_AF5_SPI2         ((uint8_t)0x05U)  /*!< AF5: SPI2 Alternate Function mapping      */
#define GPIO_AF5_I2C2         ((uint8_t)0x05U)  /*!< AF5: I2C2 Alternate Function mapping      */
#define GPIO_AF5_MCO          ((uint8_t)0x05U)  /*!< AF5: MCO Alternate Function mapping       */
#define GPIO_AF5_USART6       ((uint8_t)0x05U)  /*!< AF5: USART6 Alternate Function mapping    */

/* AF 6 */
#define GPIO_AF6_EVENTOUT     ((uint8_t)0x06U)  /*!< AF6: EVENTOUT Alternate Function mapping  */

#define IS_GPIO_AF(AF)        ((AF) <= (uint8_t)0x06U)

#endif /* STM32F030xC */

#if defined (STM32F072xB) || defined (STM32F078xx)
/*--------------------------- STM32F072xB/STM32F078xx ---------------------------*/
/* AF 0 */
#define GPIO_AF0_EVENTOUT     ((uint8_t)0x00U)  /*!< AF0: EVENTOUT Alternate Function mapping  */
#define GPIO_AF0_SWDIO        ((uint8_t)0x00U)  /*!< AF0: SWDIO Alternate Function mapping     */
#define GPIO_AF0_SWCLK        ((uint8_t)0x00U)  /*!< AF0: SWCLK Alternate Function mapping     */
#define GPIO_AF0_MCO          ((uint8_t)0x00U)  /*!< AF0: MCO Alternate Function mapping       */
#define GPIO_AF0_CEC          ((uint8_t)0x00U)  /*!< AF0: CEC Alternate Function mapping       */
#define GPIO_AF0_CRS          ((uint8_t)0x00U)  /*!< AF0: CRS Alternate Function mapping       */
#define GPIO_AF0_IR           ((uint8_t)0x00U)  /*!< AF0: IR Alternate Function mapping        */
#define GPIO_AF0_SPI1         ((uint8_t)0x00U)  /*!< AF0: SPI1/I2S1 Alternate Function mapping */
#define GPIO_AF0_SPI2         ((uint8_t)0x00U)  /*!< AF0: SPI2/I2S2 Alternate Function mapping */
#define GPIO_AF0_TIM1         ((uint8_t)0x00U)  /*!< AF0: TIM1 Alternate Function mapping      */
#define GPIO_AF0_TIM3         ((uint8_t)0x00U)  /*!< AF0: TIM3 Alternate Function mapping      */
#define GPIO_AF0_TIM14        ((uint8_t)0x00U)  /*!< AF0: TIM14 Alternate Function mapping     */
#define GPIO_AF0_TIM15        ((uint8_t)0x00U)  /*!< AF0: TIM15 Alternate Function mapping     */
#define GPIO_AF0_TIM16        ((uint8_t)0x00U)  /*!< AF0: TIM16 Alternate Function mapping     */
#define GPIO_AF0_TIM17        ((uint8_t)0x00U)  /*!< AF0: TIM17 Alternate Function mapping     */
#define GPIO_AF0_TSC          ((uint8_t)0x00U)  /*!< AF0: TSC Alternate Function mapping       */
#define GPIO_AF0_USART1       ((uint8_t)0x00U)  /*!< AF0: USART1 Alternate Function mapping    */
#define GPIO_AF0_USART2       ((uint8_t)0x00U)  /*!< AF0: USART2 Alternate Function mapping    */
#define GPIO_AF0_USART3       ((uint8_t)0x00U)  /*!< AF0: USART2 Alternate Function mapping    */
#define GPIO_AF0_USART4       ((uint8_t)0x00U)  /*!< AF0: USART4 Alternate Function mapping    */
#define GPIO_AF0_CAN          ((uint8_t)0x00U)  /*!< AF0: CAN Alternate Function mapping       */

/* AF 1 */
#define GPIO_AF1_TIM3         ((uint8_t)0x01U)  /*!< AF1: TIM3 Alternate Function mapping      */
#define GPIO_AF1_TIM15        ((uint8_t)0x01U)  /*!< AF1: TIM15 Alternate Function mapping     */
#define GPIO_AF1_USART1       ((uint8_t)0x01U)  /*!< AF1: USART1 Alternate Function mapping    */
#define GPIO_AF1_USART2       ((uint8_t)0x01U)  /*!< AF1: USART2 Alternate Function mapping    */
#define GPIO_AF1_USART3       ((uint8_t)0x01U)  /*!< AF1: USART3 Alternate Function mapping    */
#define GPIO_AF1_IR           ((uint8_t)0x01U)  /*!< AF1: IR Alternate Function mapping        */
#define GPIO_AF1_CEC          ((uint8_t)0x01U)  /*!< AF1: CEC Alternate Function mapping       */
#define GPIO_AF1_EVENTOUT     ((uint8_t)0x01U)  /*!< AF1: EVENTOUT Alternate Function mapping  */
#define GPIO_AF1_I2C1         ((uint8_t)0x01U)  /*!< AF1: I2C1 Alternate Function mapping      */
#define GPIO_AF1_I2C2         ((uint8_t)0x01U)  /*!< AF1: I2C1 Alternate Function mapping      */
#define GPIO_AF1_TSC          ((uint8_t)0x01U)  /*!< AF1: I2C1 Alternate Function mapping      */
#define GPIO_AF1_SPI1         ((uint8_t)0x01U)  /*!< AF1: SPI1 Alternate Function mapping      */
#define GPIO_AF1_SPI2         ((uint8_t)0x01U)  /*!< AF1: SPI2 Alternate Function mapping      */

/* AF 2 */
#define GPIO_AF2_TIM1         ((uint8_t)0x02U)  /*!< AF2: TIM1 Alternate Function mapping      */
#define GPIO_AF2_TIM2         ((uint8_t)0x02U)  /*!< AF2: TIM2 Alternate Function mapping      */
#define GPIO_AF2_TIM16        ((uint8_t)0x02U)  /*!< AF2: TIM16 Alternate Function mapping     */
#define GPIO_AF2_TIM17        ((uint8_t)0x02U)  /*!< AF2: TIM17 Alternate Function mapping     */
#define GPIO_AF2_EVENTOUT     ((uint8_t)0x02U)  /*!< AF2: EVENTOUT Alternate Function mapping  */
#define GPIO_AF2_USB          ((uint8_t)0x02U)  /*!< AF2: USB Alternate Function mapping       */

/* AF 3 */
#define GPIO_AF3_EVENTOUT     ((uint8_t)0x03U)  /*!< AF3: EVENTOUT Alternate Function mapping  */
#define GPIO_AF3_TSC          ((uint8_t)0x03U)  /*!< AF3: TSC Alternate Function mapping       */
#define GPIO_AF3_TIM15        ((uint8_t)0x03U)  /*!< AF3: TIM15 Alternate Function mapping     */
#define GPIO_AF3_I2C1         ((uint8_t)0x03U)  /*!< AF3: I2C1 Alternate Function mapping      */

/* AF 4 */
#define GPIO_AF4_TIM14        ((uint8_t)0x04U)  /*!< AF4: TIM14 Alternate Function mapping     */
#define GPIO_AF4_USART4       ((uint8_t)0x04U)  /*!< AF4: USART4 Alternate Function mapping    */
#define GPIO_AF4_USART3       ((uint8_t)0x04U)  /*!< AF4: USART3 Alternate Function mapping    */
#define GPIO_AF4_CRS          ((uint8_t)0x04U)  /*!< AF4: CRS Alternate Function mapping       */
#define GPIO_AF4_CAN          ((uint8_t)0x04U)  /*!< AF4: CAN Alternate Function mapping       */

/* AF 5 */
#define GPIO_AF5_TIM15        ((uint8_t)0x05U)  /*!< AF5: TIM15 Alternate Function mapping     */
#define GPIO_AF5_TIM16        ((uint8_t)0x05U)  /*!< AF5: TIM16 Alternate Function mapping     */
#define GPIO_AF5_TIM17        ((uint8_t)0x05U)  /*!< AF5: TIM17 Alternate Function mapping     */
#define GPIO_AF5_SPI2         ((uint8_t)0x05U)  /*!< AF5: SPI2 Alternate Function mapping      */
#define GPIO_AF5_I2C2         ((uint8_t)0x05U)  /*!< AF5: I2C2 Alternate Function mapping      */

/* AF 6 */
#define GPIO_AF6_EVENTOUT     ((uint8_t)0x06U)  /*!< AF6: EVENTOUT Alternate Function mapping  */

/* AF 7 */
#define GPIO_AF7_COMP1        ((uint8_t)0x07U)  /*!< AF7: COMP1 Alternate Function mapping     */
#define GPIO_AF7_COMP2        ((uint8_t)0x07U)  /*!< AF7: COMP2 Alternate Function mapping     */

#define IS_GPIO_AF(AF)        ((AF) <= (uint8_t)0x07U)

#endif /* STM32F072xB || STM32F078xx */

#if defined (STM32F070xB)
/*---------------------------------- STM32F070xB ---------------------------------------------*/
/* AF 0 */
#define GPIO_AF0_EVENTOUT     ((uint8_t)0x00U)  /*!< AF0: EVENTOUT Alternate Function mapping  */
#define GPIO_AF0_SWDIO        ((uint8_t)0x00U)  /*!< AF0: SWDIO Alternate Function mapping     */
#define GPIO_AF0_SWCLK        ((uint8_t)0x00U)  /*!< AF0: SWCLK Alternate Function mapping     */
#define GPIO_AF0_MCO          ((uint8_t)0x00U)  /*!< AF0: MCO Alternate Function mapping       */
#define GPIO_AF0_IR           ((uint8_t)0x00U)  /*!< AF0: IR Alternate Function mapping        */
#define GPIO_AF0_SPI1         ((uint8_t)0x00U)  /*!< AF0: SPI1 Alternate Function mapping      */
#define GPIO_AF0_SPI2         ((uint8_t)0x00U)  /*!< AF0: SPI2 Alternate Function mapping      */
#define GPIO_AF0_TIM3         ((uint8_t)0x00U)  /*!< AF0: TIM3 Alternate Function mapping      */
#define GPIO_AF0_TIM14        ((uint8_t)0x00U)  /*!< AF0: TIM14 Alternate Function mapping     */
#define GPIO_AF0_TIM15        ((uint8_t)0x00U)  /*!< AF0: TIM15 Alternate Function mapping     */
#define GPIO_AF0_TIM17        ((uint8_t)0x00U)  /*!< AF0: TIM17 Alternate Function mapping     */
#define GPIO_AF0_USART1       ((uint8_t)0x00U)  /*!< AF0: USART1 Alternate Function mapping    */
#define GPIO_AF0_USART4       ((uint8_t)0x00U)  /*!< AF0: USART4 Alternate Function mapping    */

/* AF 1 */
#define GPIO_AF1_TIM3         ((uint8_t)0x01U)  /*!< AF1: TIM3 Alternate Function mapping      */
#define GPIO_AF1_TIM15        ((uint8_t)0x01U)  /*!< AF1: TIM15 Alternate Function mapping     */
#define GPIO_AF1_USART1       ((uint8_t)0x01U)  /*!< AF1: USART1 Alternate Function mapping    */
#define GPIO_AF1_USART2       ((uint8_t)0x01U)  /*!< AF1: USART2 Alternate Function mapping    */
#define GPIO_AF1_USART3       ((uint8_t)0x01U)  /*!< AF1: USART4 Alternate Function mapping    */
#define GPIO_AF1_IR           ((uint8_t)0x01U)  /*!< AF1: IR Alternate Function mapping        */
#define GPIO_AF1_EVENTOUT     ((uint8_t)0x01U)  /*!< AF1: EVENTOUT Alternate Function mapping  */
#define GPIO_AF1_I2C1         ((uint8_t)0x01U)  /*!< AF1: I2C1 Alternate Function mapping      */
#define GPIO_AF1_I2C2         ((uint8_t)0x01U)  /*!< AF1: I2C1 Alternate Function mapping      */
#define GPIO_AF1_SPI2         ((uint8_t)0x01U)  /*!< AF1: SPI2 Alternate Function mapping      */

/* AF 2 */
#define GPIO_AF2_TIM1         ((uint8_t)0x02U)  /*!< AF2: TIM1 Alternate Function mapping      */
#define GPIO_AF2_TIM16        ((uint8_t)0x02U)  /*!< AF2: TIM16 Alternate Function mapping     */
#define GPIO_AF2_TIM17        ((uint8_t)0x02U)  /*!< AF2: TIM17 Alternate Function mapping     */
#define GPIO_AF2_EVENTOUT     ((uint8_t)0x02U)  /*!< AF2: EVENTOUT Alternate Function mapping  */
#define GPIO_AF2_USB          ((uint8_t)0x02U)  /*!< AF2: USB Alternate Function mapping       */

/* AF 3 */
#define GPIO_AF3_EVENTOUT     ((uint8_t)0x03U)  /*!< AF3: EVENTOUT Alternate Function mapping  */
#define GPIO_AF3_I2C1         ((uint8_t)0x03U)  /*!< AF3: I2C1 Alternate Function mapping      */
#define GPIO_AF3_TIM15        ((uint8_t)0x03U)  /*!< AF3: TIM15 Alternate Function mapping     */

/* AF 4 */
#define GPIO_AF4_TIM14        ((uint8_t)0x04U)  /*!< AF4: TIM14 Alternate Function mapping     */
#define GPIO_AF4_USART4       ((uint8_t)0x04U)  /*!< AF4: USART4 Alternate Function mapping    */
#define GPIO_AF4_USART3       ((uint8_t)0x04U)  /*!< AF4: USART3 Alternate Function mapping    */

/* AF 5 */
#define GPIO_AF5_TIM15        ((uint8_t)0x05U)  /*!< AF5: TIM15 Alternate Function mapping     */
#define GPIO_AF5_TIM16        ((uint8_t)0x05U)  /*!< AF5: TIM16 Alternate Function mapping     */
#define GPIO_AF5_TIM17        ((uint8_t)0x05U)  /*!< AF5: TIM17 Alternate Function mapping     */
#define GPIO_AF5_SPI2         ((uint8_t)0x05U)  /*!< AF5: SPI2 Alternate Function mapping      */
#define GPIO_AF5_I2C2         ((uint8_t)0x05U)  /*!< AF5: I2C2 Alternate Function mapping      */

/* AF 6 */
#define GPIO_AF6_EVENTOUT     ((uint8_t)0x06U)  /*!< AF6: EVENTOUT Alternate Function mapping  */

#define IS_GPIO_AF(AF)        ((AF) <= (uint8_t)0x06U)
   
#endif /* STM32F070xB */

#if defined (STM32F042x6) || defined (STM32F048xx)
/*--------------------------- STM32F042x6/STM32F048xx ---------------------------*/
/* AF 0 */
#define GPIO_AF0_EVENTOUT     ((uint8_t)0x00U)  /*!< AF0: EVENTOUT Alternate Function mapping  */
#define GPIO_AF0_CEC          ((uint8_t)0x00U)  /*!< AF0: CEC Alternate Function mapping       */
#define GPIO_AF0_CRS          ((uint8_t)0x00U)  /*!< AF0: CRS Alternate Function mapping       */
#define GPIO_AF0_IR           ((uint8_t)0x00U)  /*!< AF0: IR Alternate Function mapping        */
#define GPIO_AF0_MCO          ((uint8_t)0x00U)  /*!< AF0: MCO Alternate Function mapping       */
#define GPIO_AF0_SPI1         ((uint8_t)0x00U)  /*!< AF0: SPI1/I2S1 Alternate Function mapping */
#define GPIO_AF0_SPI2         ((uint8_t)0x00U)  /*!< AF0: SPI2/I2S2 Alternate Function mapping */
#define GPIO_AF0_SWDIO        ((uint8_t)0x00U)  /*!< AF0: SWDIO Alternate Function mapping     */
#define GPIO_AF0_SWCLK        ((uint8_t)0x00U)  /*!< AF0: SWCLK Alternate Function mapping     */
#define GPIO_AF0_TIM14        ((uint8_t)0x00U)  /*!< AF0: TIM14 Alternate Function mapping     */
#define GPIO_AF0_TIM17        ((uint8_t)0x00U)  /*!< AF0: TIM17 Alternate Function mapping     */
#define GPIO_AF0_USART1       ((uint8_t)0x00U)  /*!< AF0: USART1 Alternate Function mapping    */

/* AF 1 */
#define GPIO_AF1_CEC          ((uint8_t)0x01U)  /*!< AF1: CEC Alternate Function mapping       */
#define GPIO_AF1_EVENTOUT     ((uint8_t)0x01U)  /*!< AF1: EVENTOUT Alternate Function mapping  */
#define GPIO_AF1_I2C1         ((uint8_t)0x01U)  /*!< AF1: I2C1 Alternate Function mapping      */
#define GPIO_AF1_IR           ((uint8_t)0x01U)  /*!< AF1: IR Alternate Function mapping        */
#define GPIO_AF1_USART1       ((uint8_t)0x01U)  /*!< AF1: USART1 Alternate Function mapping    */
#define GPIO_AF1_USART2       ((uint8_t)0x01U)  /*!< AF1: USART2 Alternate Function mapping    */
#define GPIO_AF1_TIM3         ((uint8_t)0x01U)  /*!< AF1: TIM3 Alternate Function mapping      */

/* AF 2 */
#define GPIO_AF2_EVENTOUT     ((uint8_t)0x02U)  /*!< AF2: EVENTOUT Alternate Function mapping  */
#define GPIO_AF2_TIM1         ((uint8_t)0x02U)  /*!< AF2: TIM1 Alternate Function mapping      */
#define GPIO_AF2_TIM2         ((uint8_t)0x02U)  /*!< AF2: TIM2 Alternate Function mapping      */
#define GPIO_AF2_TIM16        ((uint8_t)0x02U)  /*!< AF2: TIM16 Alternate Function mapping     */
#define GPIO_AF2_TIM17        ((uint8_t)0x02U)  /*!< AF2: TIM17 Alternate Function mapping     */
#define GPIO_AF2_USB          ((uint8_t)0x02U)  /*!< AF2: USB Alternate Function mapping       */

/* AF 3 */
#define GPIO_AF3_EVENTOUT     ((uint8_t)0x03U)  /*!< AF3: EVENTOUT Alternate Function mapping  */
#define GPIO_AF3_I2C1         ((uint8_t)0x03U)  /*!< AF3: I2C1 Alternate Function mapping      */
#define GPIO_AF3_TSC          ((uint8_t)0x03U)  /*!< AF3: TSC Alternate Function mapping       */

/* AF 4 */
#define GPIO_AF4_TIM14        ((uint8_t)0x04U)  /*!< AF4: TIM14 Alternate Function mapping     */
#define GPIO_AF4_CAN          ((uint8_t)0x04U)  /*!< AF4: CAN Alternate Function mapping       */
#define GPIO_AF4_CRS          ((uint8_t)0x04U)  /*!< AF4: CRS Alternate Function mapping       */
#define GPIO_AF4_I2C1         ((uint8_t)0x04U)  /*!< AF4: I2C1 Alternate Function mapping      */

/* AF 5 */ 
#define GPIO_AF5_MCO          ((uint8_t)0x05U)  /*!< AF5: MCO Alternate Function mapping       */
#define GPIO_AF5_I2C1         ((uint8_t)0x05U)  /*!< AF5: I2C1 Alternate Function mapping      */
#define GPIO_AF5_I2C2         ((uint8_t)0x05U)  /*!< AF5: I2C2 Alternate Function mapping      */
#define GPIO_AF5_SPI2         ((uint8_t)0x05U)  /*!< AF5: SPI2 Alternate Function mapping      */
#define GPIO_AF5_TIM16        ((uint8_t)0x05U)  /*!< AF5: TIM16 Alternate Function mapping     */
#define GPIO_AF5_TIM17        ((uint8_t)0x05U)  /*!< AF5: TIM17 Alternate Function mapping     */
#define GPIO_AF5_USB          ((uint8_t)0x05U)  /*!< AF5: USB Alternate Function mapping       */

/* AF 6 */ 
#define GPIO_AF6_EVENTOUT     ((uint8_t)0x06U)  /*!< AF6: EVENTOUT Alternate Function mapping  */

#define IS_GPIO_AF(AF)        ((AF) <= (uint8_t)0x06U)

#endif /* STM32F042x6 || STM32F048xx */

#if defined (STM32F070x6)
/*--------------------------------------- STM32F070x6 ----------------------------------------*/
/* AF 0 */
#define GPIO_AF0_EVENTOUT     ((uint8_t)0x00U)  /*!< AF0: EVENTOUT Alternate Function mapping  */
#define GPIO_AF0_IR           ((uint8_t)0x00U)  /*!< AF0: IR Alternate Function mapping        */
#define GPIO_AF0_MCO          ((uint8_t)0x00U)  /*!< AF0: MCO Alternate Function mapping       */
#define GPIO_AF0_SPI1         ((uint8_t)0x00U)  /*!< AF0: SPI1 Alternate Function mapping      */
#define GPIO_AF0_SWDIO        ((uint8_t)0x00U)  /*!< AF0: SWDIO Alternate Function mapping     */
#define GPIO_AF0_SWCLK        ((uint8_t)0x00U)  /*!< AF0: SWCLK Alternate Function mapping     */
#define GPIO_AF0_TIM14        ((uint8_t)0x00U)  /*!< AF0: TIM14 Alternate Function mapping     */
#define GPIO_AF0_TIM17        ((uint8_t)0x00U)  /*!< AF0: TIM17 Alternate Function mapping     */
#define GPIO_AF0_USART1       ((uint8_t)0x00U)  /*!< AF0: USART1 Alternate Function mapping    */

/* AF 1 */
#define GPIO_AF1_EVENTOUT     ((uint8_t)0x01U)  /*!< AF1: EVENTOUT Alternate Function mapping  */
#define GPIO_AF1_I2C1         ((uint8_t)0x01U)  /*!< AF1: I2C1 Alternate Function mapping      */
#define GPIO_AF1_IR           ((uint8_t)0x01U)  /*!< AF1: IR Alternate Function mapping        */
#define GPIO_AF1_USART1       ((uint8_t)0x01U)  /*!< AF1: USART1 Alternate Function mapping    */
#define GPIO_AF1_USART2       ((uint8_t)0x01U)  /*!< AF1: USART2 Alternate Function mapping    */
#define GPIO_AF1_TIM3         ((uint8_t)0x01U)  /*!< AF1: TIM3 Alternate Function mapping      */

/* AF 2 */
#define GPIO_AF2_EVENTOUT     ((uint8_t)0x02U)  /*!< AF2: EVENTOUT Alternate Function mapping  */
#define GPIO_AF2_TIM1         ((uint8_t)0x02U)  /*!< AF2: TIM1 Alternate Function mapping      */
#define GPIO_AF2_TIM16        ((uint8_t)0x02U)  /*!< AF2: TIM16 Alternate Function mapping     */
#define GPIO_AF2_TIM17        ((uint8_t)0x02U)  /*!< AF2: TIM17 Alternate Function mapping     */
#define GPIO_AF2_USB          ((uint8_t)0x02U)  /*!< AF2: USB Alternate Function mapping       */

/* AF 3 */
#define GPIO_AF3_EVENTOUT     ((uint8_t)0x03U)  /*!< AF3: EVENTOUT Alternate Function mapping  */
#define GPIO_AF3_I2C1         ((uint8_t)0x03U)  /*!< AF3: I2C1 Alternate Function mapping      */

/* AF 4 */
#define GPIO_AF4_TIM14        ((uint8_t)0x04U)  /*!< AF4: TIM14 Alternate Function mapping     */
#define GPIO_AF4_I2C1         ((uint8_t)0x04U)  /*!< AF4: I2C1 Alternate Function mapping      */

/* AF 5 */ 
#define GPIO_AF5_MCO          ((uint8_t)0x05U)  /*!< AF5: MCO Alternate Function mapping       */
#define GPIO_AF5_I2C1         ((uint8_t)0x05U)  /*!< AF5: I2C1 Alternate Function mapping      */
#define GPIO_AF5_TIM16        ((uint8_t)0x05U)  /*!< AF5: TIM16 Alternate Function mapping     */
#define GPIO_AF5_TIM17        ((uint8_t)0x05U)  /*!< AF5: TIM17 Alternate Function mapping     */
#define GPIO_AF5_USB          ((uint8_t)0x05U)  /*!< AF5: USB Alternate Function mapping       */

/* AF 6 */ 
#define GPIO_AF6_EVENTOUT     ((uint8_t)0x06U)  /*!< AF6: EVENTOUT Alternate Function mapping  */

#define IS_GPIO_AF(AF)        ((AF) <= (uint8_t)0x06U)

#endif /* STM32F070x6 */
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

/** @defgroup GPIOEx_Get_Port_Index GPIOEx_Get Port Index
* @{
  */
#if defined(GPIOD) && defined(GPIOE)
#define GPIO_GET_INDEX(__GPIOx__)    (((__GPIOx__) == (GPIOA))? 0U :\
                                      ((__GPIOx__) == (GPIOB))? 1U :\
                                      ((__GPIOx__) == (GPIOC))? 2U :\
                                      ((__GPIOx__) == (GPIOD))? 3U :\
                                      ((__GPIOx__) == (GPIOE))? 4U : 5U)
#endif

#if defined(GPIOD) && !defined(GPIOE)
#define GPIO_GET_INDEX(__GPIOx__)    (((__GPIOx__) == (GPIOA))? 0U :\
                                      ((__GPIOx__) == (GPIOB))? 1U :\
                                      ((__GPIOx__) == (GPIOC))? 2U :\
                                      ((__GPIOx__) == (GPIOD))? 3U : 5U)
#endif

#if !defined(GPIOD) && defined(GPIOE)
#define GPIO_GET_INDEX(__GPIOx__)    (((__GPIOx__) == (GPIOA))? 0U :\
                                      ((__GPIOx__) == (GPIOB))? 1U :\
                                      ((__GPIOx__) == (GPIOC))? 2U :\
                                      ((__GPIOx__) == (GPIOE))? 4U : 5U)
#endif

#if !defined(GPIOD) && !defined(GPIOE)
#define GPIO_GET_INDEX(__GPIOx__)    (((__GPIOx__) == (GPIOA))? 0U :\
                                      ((__GPIOx__) == (GPIOB))? 1U :\
                                      ((__GPIOx__) == (GPIOC))? 2U : 5U)
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

#endif /* __STM32F0xx_HAL_GPIO_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

