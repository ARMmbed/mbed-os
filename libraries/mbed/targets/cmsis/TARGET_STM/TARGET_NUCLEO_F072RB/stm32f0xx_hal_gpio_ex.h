/**
  ******************************************************************************
  * @file    stm32f0xx_hal_gpio_ex.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    28-May-2014
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

/** @addtogroup GPIOEx
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/** @defgroup GPIOEx_Exported_Constants GPIOEx_Exported_Constants
  * @{
  */ 
  
/** @defgroup GPIOEx_Alternate_function_selection GPIOEx_Alternate_function_selection
  * @{
  */
  
#if defined (STM32F030x6)
/*------------------------- STM32F030x6---------------------------*/ 
/** 
  * @brief   AF 0 selection  
  */ 
#define GPIO_AF0_EVENTOUT      ((uint8_t)0x00)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /* MCO Alternate Function mapping      */
#define GPIO_AF0_SPI1          ((uint8_t)0x00)  /* SPI1 Alternate Function mapping     */
#define GPIO_AF0_TIM17         ((uint8_t)0x00)  /* TIM17 Alternate Function mapping    */
#define GPIO_AF0_SWDIO         ((uint8_t)0x00)  /* SWDIO Alternate Function mapping    */
#define GPIO_AF0_SWCLK         ((uint8_t)0x00)  /* SWCLK Alternate Function mapping    */
#define GPIO_AF0_TIM14         ((uint8_t)0x00)  /* TIM14 Alternate Function mapping    */
#define GPIO_AF0_USART1        ((uint8_t)0x00)  /* USART1 Alternate Function mapping */
#define GPIO_AF0_IR            ((uint8_t)0x00)  /* IR Alternate Function mapping */

/** 
  * @brief   AF 1 selection  
  */ 
#define GPIO_AF1_TIM3           ((uint8_t)0x01)  /* TIM3 Alternate Function mapping */
#define GPIO_AF1_USART1         ((uint8_t)0x01)  /* USART1 Alternate Function mapping */
#define GPIO_AF1_EVENTOUT       ((uint8_t)0x01)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF1_I2C1           ((uint8_t)0x01)  /* I2C1 Alternate Function mapping */

/** 
  * @brief   AF 2 selection  
  */ 
#define GPIO_AF2_TIM1           ((uint8_t)0x02)  /* TIM1 Alternate Function mapping */
#define GPIO_AF2_TIM16          ((uint8_t)0x02)  /* TIM16 Alternate Function mapping */
#define GPIO_AF2_TIM17          ((uint8_t)0x02)  /* TIM17 Alternate Function mapping */
#define GPIO_AF2_EVENTOUT       ((uint8_t)0x02)  /* EVENTOUT Alternate Function mapping */

/** 
  * @brief   AF 3 selection  
  */ 
#define GPIO_AF3_EVENTOUT       ((uint8_t)0x03)  /* EVENTOUT Alternate Function mapping  */
#define GPIO_AF3_I2C1           ((uint8_t)0x03)  /* I2C1 Alternate Function mapping  */

/** 
  * @brief   AF 4 selection  
  */ 
#define GPIO_AF4_TIM14          ((uint8_t)0x04)  /* TIM14 Alternate Function mapping */
#define GPIO_AF4_I2C1           ((uint8_t)0x04)  /* I2C1 Alternate Function mapping */

/** 
  * @brief   AF 5 selection  
  */ 
#define GPIO_AF5_TIM16          ((uint8_t)0x05)  /* TIM16 Alternate Function mapping      */
#define GPIO_AF5_TIM17          ((uint8_t)0x05)  /* TIM17 Alternate Function mapping */

/** 
  * @brief   AF 6 selection  
  */ 
#define GPIO_AF6_EVENTOUT       ((uint8_t)0x06)  /* EVENTOUT Alternate Function mapping */

#define IS_GPIO_AF(AF)          ((AF) <= (uint8_t)0x06)

#endif /* STM32F030x6 */

/*---------------------------------- STM32F030x8 -------------------------------------------*/
#if defined (STM32F030x8)
/** 
  * @brief   AF 0 selection  
  */ 
#define GPIO_AF0_EVENTOUT      ((uint8_t)0x00)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /* MCO Alternate Function mapping      */
#define GPIO_AF0_SPI1          ((uint8_t)0x00)  /* SPI1 Alternate Function mapping     */
#define GPIO_AF0_SPI2          ((uint8_t)0x00)  /* SPI2 Alternate Function mapping     */
#define GPIO_AF0_TIM15         ((uint8_t)0x00)  /* TIM15 Alternate Function mapping    */
#define GPIO_AF0_TIM17         ((uint8_t)0x00)  /* TIM17 Alternate Function mapping    */
#define GPIO_AF0_SWDIO         ((uint8_t)0x00)  /* SWDIO Alternate Function mapping    */
#define GPIO_AF0_SWCLK         ((uint8_t)0x00)  /* SWCLK Alternate Function mapping    */
#define GPIO_AF0_TIM14         ((uint8_t)0x00)  /* TIM14 Alternate Function mapping    */
#define GPIO_AF0_USART1        ((uint8_t)0x00)  /* USART1 Alternate Function mapping */
#define GPIO_AF0_IR            ((uint8_t)0x00)  /* IR Alternate Function mapping */

/** 
  * @brief   AF 1 selection  
  */ 
#define GPIO_AF1_TIM3           ((uint8_t)0x01)  /* TIM3 Alternate Function mapping */
#define GPIO_AF1_TIM15          ((uint8_t)0x01)  /* TIM15 Alternate Function mapping */
#define GPIO_AF1_USART1         ((uint8_t)0x01)  /* USART1 Alternate Function mapping */
#define GPIO_AF1_USART2         ((uint8_t)0x01)  /* USART2 Alternate Function mapping */
#define GPIO_AF1_EVENTOUT       ((uint8_t)0x01)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF1_I2C1           ((uint8_t)0x01)  /* I2C1 Alternate Function mapping */
#define GPIO_AF1_I2C2           ((uint8_t)0x01)  /* I2C2 Alternate Function mapping */
#define GPIO_AF1_IR             ((uint8_t)0x01)  /* IR Alternate Function mapping */

/** 
  * @brief   AF 2 selection
  */ 
#define GPIO_AF2_TIM1           ((uint8_t)0x02)  /* TIM1 Alternate Function mapping */
#define GPIO_AF2_TIM16          ((uint8_t)0x02)  /* TIM16 Alternate Function mapping */
#define GPIO_AF2_TIM17          ((uint8_t)0x02)  /* TIM17 Alternate Function mapping */
#define GPIO_AF2_EVENTOUT       ((uint8_t)0x02)  /* EVENTOUT Alternate Function mapping */

/** 
  * @brief   AF 3 selection  
  */ 
#define GPIO_AF3_EVENTOUT       ((uint8_t)0x03)  /* EVENTOUT Alternate Function mapping  */
#define GPIO_AF3_I2C1           ((uint8_t)0x03)  /* I2C1 Alternate Function mapping      */
#define GPIO_AF3_TIM15          ((uint8_t)0x03)  /* TIM15 Alternate Function mapping     */

/** 
  * @brief   AF 4 selection  
  */ 
#define GPIO_AF4_TIM14          ((uint8_t)0x04)  /* TIM14 Alternate Function mapping */

/** 
  * @brief   AF 5 selection  
  */ 
#define GPIO_AF5_TIM16          ((uint8_t)0x05)  /* TIM16 Alternate Function mapping      */
#define GPIO_AF5_TIM17          ((uint8_t)0x05)  /* TIM17 Alternate Function mapping */

/** 
  * @brief   AF 6 selection  
  */ 
#define GPIO_AF6_EVENTOUT       ((uint8_t)0x06)  /* EVENTOUT Alternate Function mapping */

#define IS_GPIO_AF(AF)          ((AF) <= (uint8_t)0x06)

#endif /* STM32F030x8 */

#if defined (STM32F031x6) || defined (STM32F038xx)
/*--------------------------- STM32F031x6/STM32F038xx ---------------------------*/
/** 
  * @brief   AF 0 selection  
  */ 
#define GPIO_AF0_EVENTOUT      ((uint8_t)0x00)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /* MCO Alternate Function mapping      */
#define GPIO_AF0_SPI1          ((uint8_t)0x00)  /* SPI1/I2S1 Alternate Function mapping     */
#define GPIO_AF0_TIM17         ((uint8_t)0x00)  /* TIM17 Alternate Function mapping    */
#define GPIO_AF0_SWDAT         ((uint8_t)0x00)  /* SWDAT Alternate Function mapping    */
#define GPIO_AF0_SWCLK         ((uint8_t)0x00)  /* SWCLK Alternate Function mapping    */
#define GPIO_AF0_TIM14         ((uint8_t)0x00)  /* TIM14 Alternate Function mapping    */
#define GPIO_AF0_USART1        ((uint8_t)0x00)  /* USART1 Alternate Function mapping    */
#define GPIO_AF0_IR            ((uint8_t)0x00)  /* IR Alternate Function mapping */

/** 
  * @brief   AF 1 selection  
  */ 
#define GPIO_AF1_TIM3           ((uint8_t)0x01)  /* TIM3 Alternate Function mapping */
#define GPIO_AF1_USART1         ((uint8_t)0x01)  /* USART1 Alternate Function mapping */
#define GPIO_AF1_IR             ((uint8_t)0x01)  /* IR Alternate Function mapping */
#define GPIO_AF1_EVENTOUT       ((uint8_t)0x01)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF1_I2C1           ((uint8_t)0x01)  /* I2C1 Alternate Function mapping */

/** 
  * @brief   AF 2 selection
  */ 
#define GPIO_AF2_TIM1           ((uint8_t)0x02)  /* TIM1 Alternate Function mapping */
#define GPIO_AF2_TIM2           ((uint8_t)0x02)  /* TIM2 Alternate Function mapping */
#define GPIO_AF2_TIM16          ((uint8_t)0x02)  /* TIM16 Alternate Function mapping */
#define GPIO_AF2_TIM17          ((uint8_t)0x02)  /* TIM17 Alternate Function mapping */
#define GPIO_AF2_EVENTOUT       ((uint8_t)0x02)  /* EVENTOUT Alternate Function mapping */

/** 
  * @brief   AF 3 selection  
  */ 
#define GPIO_AF3_EVENTOUT       ((uint8_t)0x03)  /* EVENTOUT Alternate Function mapping  */
#define GPIO_AF3_I2C1           ((uint8_t)0x03)  /* I2C1 Alternate Function mapping      */

/** 
  * @brief   AF 4 selection  
  */ 
#define GPIO_AF4_TIM14          ((uint8_t)0x04)  /* TIM14 Alternate Function mapping */
#define GPIO_AF4_I2C1           ((uint8_t)0x04)  /* I2C1 Alternate Function mapping */

/** 
  * @brief   AF 5 selection  
  */ 
#define GPIO_AF5_TIM16          ((uint8_t)0x05)  /* TIM16 Alternate Function mapping      */
#define GPIO_AF5_TIM17          ((uint8_t)0x05)  /* TIM17 Alternate Function mapping */

/** 
  * @brief   AF 6 selection  
  */ 
#define GPIO_AF6_EVENTOUT       ((uint8_t)0x06)  /* EVENTOUT Alternate Function mapping */

#define IS_GPIO_AF(AF)          ((AF) <= (uint8_t)0x06)

#endif /* STM32F031x6 || STM32F038xx */

#if defined (STM32F051x8) || defined (STM32F058xx)
/*--------------------------- STM32F051x8/STM32F058xx---------------------------*/
/** 
  * @brief   AF 0 selection  
  */ 
#define GPIO_AF0_EVENTOUT      ((uint8_t)0x00)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /* MCO Alternate Function mapping      */
#define GPIO_AF0_SPI1          ((uint8_t)0x00)  /* SPI1/I2S1 Alternate Function mapping     */
#define GPIO_AF0_SPI2          ((uint8_t)0x00)  /* SPI2 Alternate Function mapping     */
#define GPIO_AF0_TIM15         ((uint8_t)0x00)  /* TIM15 Alternate Function mapping    */
#define GPIO_AF0_TIM17         ((uint8_t)0x00)  /* TIM17 Alternate Function mapping    */
#define GPIO_AF0_SWDIO         ((uint8_t)0x00)  /* SWDIO Alternate Function mapping    */
#define GPIO_AF0_SWCLK         ((uint8_t)0x00)  /* SWCLK Alternate Function mapping    */
#define GPIO_AF0_TIM14         ((uint8_t)0x00)  /* TIM14 Alternate Function mapping    */
#define GPIO_AF0_USART1        ((uint8_t)0x00)  /* USART1 Alternate Function mapping */
#define GPIO_AF0_IR            ((uint8_t)0x00)  /* IR Alternate Function mapping */
#define GPIO_AF0_CEC           ((uint8_t)0x00)  /* CEC Alternate Function mapping */

/** 
  * @brief   AF 1 selection  
  */ 
#define GPIO_AF1_TIM3           ((uint8_t)0x01)  /* TIM3 Alternate Function mapping */
#define GPIO_AF1_TIM15          ((uint8_t)0x01)  /* TIM15 Alternate Function mapping */
#define GPIO_AF1_USART1         ((uint8_t)0x01)  /* USART1 Alternate Function mapping */
#define GPIO_AF1_USART2         ((uint8_t)0x01)  /* USART2 Alternate Function mapping */
#define GPIO_AF1_EVENTOUT       ((uint8_t)0x01)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF1_I2C1           ((uint8_t)0x01)  /* I2C1 Alternate Function mapping */
#define GPIO_AF1_I2C2           ((uint8_t)0x01)  /* I2C2 Alternate Function mapping */
#define GPIO_AF1_IR             ((uint8_t)0x01)  /* IR Alternate Function mapping */
#define GPIO_AF1_CEC            ((uint8_t)0x01)  /* CEC Alternate Function mapping */

/** 
  * @brief   AF 2 selection
  */ 
#define GPIO_AF2_TIM1           ((uint8_t)0x02)  /* TIM1 Alternate Function mapping */
#define GPIO_AF2_TIM2           ((uint8_t)0x02)  /* TIM2 Alternate Function mapping */
#define GPIO_AF2_TIM16          ((uint8_t)0x02)  /* TIM16 Alternate Function mapping */
#define GPIO_AF2_TIM17          ((uint8_t)0x02)  /* TIM17 Alternate Function mapping */
#define GPIO_AF2_EVENTOUT       ((uint8_t)0x02)  /* EVENTOUT Alternate Function mapping */

/** 
  * @brief   AF 3 selection  
  */ 
#define GPIO_AF3_EVENTOUT      ((uint8_t)0x03)  /* EVENTOUT Alternate Function mapping  */
#define GPIO_AF3_I2C1          ((uint8_t)0x03)  /* I2C1 Alternate Function mapping      */
#define GPIO_AF3_TIM15         ((uint8_t)0x03)  /* TIM15 Alternate Function mapping     */
#define GPIO_AF3_TSC           ((uint8_t)0x03)  /* TSC Alternate Function mapping       */

/** 
  * @brief   AF 4 selection  
  */ 
#define GPIO_AF4_TIM14         ((uint8_t)0x04)  /* TIM14 Alternate Function mapping */

/** 
  * @brief   AF 5 selection  
  */ 
#define GPIO_AF5_TIM16         ((uint8_t)0x05)  /* TIM16 Alternate Function mapping */
#define GPIO_AF5_TIM17         ((uint8_t)0x05)  /* TIM17 Alternate Function mapping */

/** 
  * @brief   AF 6 selection  
  */ 
#define GPIO_AF6_EVENTOUT      ((uint8_t)0x06)  /* EVENTOUT Alternate Function mapping */

/** 
  * @brief   AF 7 selection  
  */ 
#define GPIO_AF7_COMP1         ((uint8_t)0x07)  /* COMP1 Alternate Function mapping */
#define GPIO_AF7_COMP2         ((uint8_t)0x07)  /* COMP2 Alternate Function mapping */

#define IS_GPIO_AF(AF)         ((AF) <= (uint8_t)0x07)

#endif /* STM32F051x8/STM32F058xx */

#if defined (STM32F071xB)
/*--------------------------- STM32F071xB ---------------------------*/
/** 
  * @brief   AF 0 selection  
  */ 
#define GPIO_AF0_EVENTOUT      ((uint8_t)0x00)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF0_SWDIO         ((uint8_t)0x00)  /* SWDIO Alternate Function mapping    */
#define GPIO_AF0_SWCLK         ((uint8_t)0x00)  /* SWCLK Alternate Function mapping    */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /* MCO Alternate Function mapping      */
#define GPIO_AF0_CEC           ((uint8_t)0x00)  /* CEC Alternate Function mapping      */
#define GPIO_AF0_CRS           ((uint8_t)0x00)  /* CRS Alternate Function mapping   */
#define GPIO_AF0_IR            ((uint8_t)0x00)  /* IR Alternate Function mapping      */
#define GPIO_AF0_SPI1          ((uint8_t)0x00)  /* SPI1/I2S1 Alternate Function mapping */
#define GPIO_AF0_SPI2          ((uint8_t)0x00)  /* SPI2/I2S2 Alternate Function mapping */
#define GPIO_AF0_TIM1          ((uint8_t)0x00)  /* TIM1 Alternate Function mapping    */
#define GPIO_AF0_TIM3          ((uint8_t)0x00)  /* TIM3 Alternate Function mapping    */
#define GPIO_AF0_TIM14         ((uint8_t)0x00)  /* TIM14 Alternate Function mapping    */
#define GPIO_AF0_TIM15         ((uint8_t)0x00)  /* TIM15 Alternate Function mapping    */
#define GPIO_AF0_TIM16         ((uint8_t)0x00)  /* TIM16 Alternate Function mapping    */
#define GPIO_AF0_TIM17         ((uint8_t)0x00)  /* TIM17 Alternate Function mapping    */
#define GPIO_AF0_TSC           ((uint8_t)0x00)  /* TSC Alternate Function mapping   */
#define GPIO_AF0_USART1        ((uint8_t)0x00)  /* USART1 Alternate Function mapping   */
#define GPIO_AF0_USART2        ((uint8_t)0x00)  /* USART2 Alternate Function mapping   */
#define GPIO_AF0_USART3        ((uint8_t)0x00)  /* USART3 Alternate Function mapping   */
#define GPIO_AF0_USART4        ((uint8_t)0x00)  /* USART4 Alternate Function mapping   */

/** 
  * @brief   AF 1 selection  
  */ 
#define GPIO_AF1_TIM3           ((uint8_t)0x01)  /* TIM3 Alternate Function mapping */
#define GPIO_AF1_TIM15          ((uint8_t)0x01)  /* TIM15 Alternate Function mapping */
#define GPIO_AF1_USART1         ((uint8_t)0x01)  /* USART1 Alternate Function mapping */
#define GPIO_AF1_USART2         ((uint8_t)0x01)  /* USART2 Alternate Function mapping */
#define GPIO_AF1_USART3         ((uint8_t)0x01)  /* USART3 Alternate Function mapping */
#define GPIO_AF1_IR             ((uint8_t)0x01)  /* IR Alternate Function mapping */
#define GPIO_AF1_CEC            ((uint8_t)0x01)  /* CEC Alternate Function mapping */
#define GPIO_AF1_EVENTOUT       ((uint8_t)0x01)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF1_I2C1           ((uint8_t)0x01)  /* I2C1 Alternate Function mapping */
#define GPIO_AF1_I2C2           ((uint8_t)0x01)  /* I2C2 Alternate Function mapping */
#define GPIO_AF1_TSC            ((uint8_t)0x01)  /* TSC Alternate Function mapping */
#define GPIO_AF1_SPI1           ((uint8_t)0x01)  /* SPI1 Alternate Function mapping */
#define GPIO_AF1_SPI2           ((uint8_t)0x01)  /* SPI2 Alternate Function mapping */

/** 
  * @brief   AF 2 selection
  */ 
#define GPIO_AF2_TIM1           ((uint8_t)0x02)  /* TIM1 Alternate Function mapping */
#define GPIO_AF2_TIM2           ((uint8_t)0x02)  /* TIM2 Alternate Function mapping */
#define GPIO_AF2_TIM16          ((uint8_t)0x02)  /* TIM16 Alternate Function mapping */
#define GPIO_AF2_TIM17          ((uint8_t)0x02)  /* TIM17 Alternate Function mapping */
#define GPIO_AF2_EVENTOUT       ((uint8_t)0x02)  /* EVENTOUT Alternate Function mapping */

/** 
  * @brief   AF 3 selection  
  */ 
#define GPIO_AF3_EVENTOUT      ((uint8_t)0x03)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF3_TSC           ((uint8_t)0x03)  /* TSC Alternate Function mapping */
#define GPIO_AF3_TIM15         ((uint8_t)0x03)  /* TIM15 Alternate Function mapping */
#define GPIO_AF3_I2C1          ((uint8_t)0x03)  /* I2C1 Alternate Function mapping */

/** 
  * @brief   AF 4 selection  
  */ 
#define GPIO_AF4_TIM14          ((uint8_t)0x04)  /* TIM14 Alternate Function mapping */
#define GPIO_AF4_USART4         ((uint8_t)0x04)  /* USART4 Alternate Function mapping */
#define GPIO_AF4_USART3         ((uint8_t)0x04)  /* USART3 Alternate Function mapping */
#define GPIO_AF4_CRS            ((uint8_t)0x04)  /* CRS Alternate Function mapping */

/** 
  * @brief   AF 5 selection  
  */ 
#define GPIO_AF5_TIM15          ((uint8_t)0x05)  /* TIM15 Alternate Function mapping */
#define GPIO_AF5_TIM16          ((uint8_t)0x05)  /* TIM16 Alternate Function mapping */
#define GPIO_AF5_TIM17          ((uint8_t)0x05)  /* TIM17 Alternate Function mapping */
#define GPIO_AF5_SPI2           ((uint8_t)0x05)  /* SPI2 Alternate Function mapping */
#define GPIO_AF5_I2C2           ((uint8_t)0x05)  /* I2C2 Alternate Function mapping */

/** 
  * @brief   AF 6 selection  
  */ 
#define GPIO_AF6_EVENTOUT       ((uint8_t)0x06)  /* EVENTOUT Alternate Function mapping */

/** 
  * @brief   AF 7 selection  
  */ 
#define GPIO_AF7_COMP1          ((uint8_t)0x07)  /* COMP1 Alternate Function mapping */
#define GPIO_AF7_COMP2          ((uint8_t)0x07)  /* COMP2 Alternate Function mapping */

#define IS_GPIO_AF(AF)          ((AF) <= (uint8_t)0x07)

#endif /* STM32F071xB */


#if defined (STM32F072xB) || defined (STM32F078xx)
/*--------------------------- STM32F072xB/STM32F078xx ---------------------------*/
/** 
  * @brief   AF 0 selection  
  */ 
#define GPIO_AF0_EVENTOUT      ((uint8_t)0x00)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF0_SWDIO         ((uint8_t)0x00)  /* SWDIO Alternate Function mapping    */
#define GPIO_AF0_SWCLK         ((uint8_t)0x00)  /* SWCLK Alternate Function mapping    */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /* MCO Alternate Function mapping      */
#define GPIO_AF0_CEC           ((uint8_t)0x00)  /* CEC Alternate Function mapping      */
#define GPIO_AF0_CRS           ((uint8_t)0x00)  /* CRS Alternate Function mapping   */
#define GPIO_AF0_IR            ((uint8_t)0x00)  /* IR Alternate Function mapping      */
#define GPIO_AF0_SPI1          ((uint8_t)0x00)  /* SPI1/I2S1 Alternate Function mapping */
#define GPIO_AF0_SPI2          ((uint8_t)0x00)  /* SPI2/I2S2 Alternate Function mapping */
#define GPIO_AF0_TIM1          ((uint8_t)0x00)  /* TIM1 Alternate Function mapping    */
#define GPIO_AF0_TIM3          ((uint8_t)0x00)  /* TIM3 Alternate Function mapping    */
#define GPIO_AF0_TIM14         ((uint8_t)0x00)  /* TIM14 Alternate Function mapping    */
#define GPIO_AF0_TIM15         ((uint8_t)0x00)  /* TIM15 Alternate Function mapping    */
#define GPIO_AF0_TIM16         ((uint8_t)0x00)  /* TIM16 Alternate Function mapping    */
#define GPIO_AF0_TIM17         ((uint8_t)0x00)  /* TIM17 Alternate Function mapping    */
#define GPIO_AF0_TSC           ((uint8_t)0x00)  /* TSC Alternate Function mapping   */
#define GPIO_AF0_USART1        ((uint8_t)0x00)  /* USART1 Alternate Function mapping   */
#define GPIO_AF0_USART2        ((uint8_t)0x00)  /* USART2 Alternate Function mapping   */
#define GPIO_AF0_USART3        ((uint8_t)0x00)  /* USART2 Alternate Function mapping   */
#define GPIO_AF0_USART4        ((uint8_t)0x00)  /* USART4 Alternate Function mapping   */
#define GPIO_AF0_CAN           ((uint8_t)0x00)  /* CAN Alternate Function mapping   */

/** 
  * @brief   AF 1 selection  
  */ 
#define GPIO_AF1_TIM3           ((uint8_t)0x01)  /* TIM3 Alternate Function mapping */
#define GPIO_AF1_TIM15          ((uint8_t)0x01)  /* TIM15 Alternate Function mapping */
#define GPIO_AF1_USART1         ((uint8_t)0x01)  /* USART1 Alternate Function mapping */
#define GPIO_AF1_USART2         ((uint8_t)0x01)  /* USART2 Alternate Function mapping */
#define GPIO_AF1_USART3         ((uint8_t)0x01)  /* USART3 Alternate Function mapping */
#define GPIO_AF1_IR             ((uint8_t)0x01)  /* IR Alternate Function mapping */
#define GPIO_AF1_CEC            ((uint8_t)0x01)  /* CEC Alternate Function mapping */
#define GPIO_AF1_EVENTOUT       ((uint8_t)0x01)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF1_I2C1           ((uint8_t)0x01)  /* I2C1 Alternate Function mapping */
#define GPIO_AF1_I2C2           ((uint8_t)0x01)  /* I2C1 Alternate Function mapping */
#define GPIO_AF1_TSC            ((uint8_t)0x01)  /* I2C1 Alternate Function mapping */
#define GPIO_AF1_SPI1           ((uint8_t)0x01)  /* SPI1 Alternate Function mapping */
#define GPIO_AF1_SPI2           ((uint8_t)0x01)  /* SPI2 Alternate Function mapping */

/** 
  * @brief   AF 2 selection
  */ 
#define GPIO_AF2_TIM1           ((uint8_t)0x02)  /* TIM1 Alternate Function mapping */
#define GPIO_AF2_TIM2           ((uint8_t)0x02)  /* TIM2 Alternate Function mapping */
#define GPIO_AF2_TIM16          ((uint8_t)0x02)  /* TIM16 Alternate Function mapping */
#define GPIO_AF2_TIM17          ((uint8_t)0x02)  /* TIM17 Alternate Function mapping */
#define GPIO_AF2_EVENTOUT       ((uint8_t)0x02)  /* EVENTOUT Alternate Function mapping */

#define GPIO_AF2_USB            ((uint8_t)0x02)  /* USB Alternate Function mapping */

/** 
  * @brief   AF 3 selection  
  */ 
#define GPIO_AF3_EVENTOUT      ((uint8_t)0x03)  /* EVENTOUT Alternate Function mapping  */
#define GPIO_AF3_TSC           ((uint8_t)0x03)  /* TSC Alternate Function mapping  */
#define GPIO_AF3_TIM15         ((uint8_t)0x03)  /* TIM15 Alternate Function mapping  */
#define GPIO_AF3_I2C1          ((uint8_t)0x03)  /* I2C1 Alternate Function mapping  */

/** 
  * @brief   AF 4 selection  
  */ 
#define GPIO_AF4_TIM14          ((uint8_t)0x04)  /* TIM14 Alternate Function mapping */
#define GPIO_AF4_USART4         ((uint8_t)0x04)  /* USART4 Alternate Function mapping */
#define GPIO_AF4_USART3         ((uint8_t)0x04)  /* USART3 Alternate Function mapping */
#define GPIO_AF4_CRS            ((uint8_t)0x04)  /* CRS Alternate Function mapping */

#define GPIO_AF4_CAN            ((uint8_t)0x04)  /* CAN Alternate Function mapping */

/** 
  * @brief   AF 5 selection  
  */ 
#define GPIO_AF5_TIM15          ((uint8_t)0x05)  /* TIM15 Alternate Function mapping */
#define GPIO_AF5_TIM16          ((uint8_t)0x05)  /* TIM16 Alternate Function mapping */
#define GPIO_AF5_TIM17          ((uint8_t)0x05)  /* TIM17 Alternate Function mapping */
#define GPIO_AF5_SPI2           ((uint8_t)0x05)  /* SPI2 Alternate Function mapping */
#define GPIO_AF5_I2C2           ((uint8_t)0x05)  /* I2C2 Alternate Function mapping */

/** 
  * @brief   AF 6 selection  
  */ 
#define GPIO_AF6_EVENTOUT       ((uint8_t)0x06)  /* EVENTOUT Alternate Function mapping */

/** 
  * @brief   AF 7 selection  
  */ 
#define GPIO_AF7_COMP1          ((uint8_t)0x07)  /* COMP1 Alternate Function mapping */
#define GPIO_AF7_COMP2          ((uint8_t)0x07)  /* COMP2 Alternate Function mapping */

#define IS_GPIO_AF(AF)          ((AF) <= (uint8_t)0x07)

#endif /* STM32F072xB || STM32F078xx */

#if defined (STM32F042x6) || defined (STM32F048xx)
/*--------------------------- STM32F042x6/STM32F048xx ---------------------------*/
/** 
  * @brief   AF 0 selection  on PA, PB, PF
  */ 
#define GPIO_AF0_EVENTOUT      ((uint8_t)0x00)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF0_CEC           ((uint8_t)0x00)  /* CEC Alternate Function mapping      */
#define GPIO_AF0_CRS           ((uint8_t)0x00)  /* CRS Alternate Function mapping      */
#define GPIO_AF0_IR            ((uint8_t)0x00)  /* IR Alternate Function mapping      */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /* MCO Alternate Function mapping      */
#define GPIO_AF0_SPI1          ((uint8_t)0x00)  /* SPI1/I2S1 Alternate Function mapping */
#define GPIO_AF0_SPI2          ((uint8_t)0x00)  /* SPI2/I2S2 Alternate Function mapping */
#define GPIO_AF0_SWDIO         ((uint8_t)0x00)  /* SWDIO Alternate Function mapping    */
#define GPIO_AF0_SWCLK         ((uint8_t)0x00)  /* SWCLK Alternate Function mapping    */
#define GPIO_AF0_TIM14         ((uint8_t)0x00)  /* TIM14 Alternate Function mapping    */
#define GPIO_AF0_TIM17         ((uint8_t)0x00)  /* TIM17 Alternate Function mapping    */
#define GPIO_AF0_USART1        ((uint8_t)0x00)  /* USART1 Alternate Function mapping   */

/** 
  * @brief   AF 1 selection  on PA, PB, PF
  */ 
#define GPIO_AF1_CEC            ((uint8_t)0x01)  /* CEC Alternate Function mapping */
#define GPIO_AF1_EVENTOUT       ((uint8_t)0x01)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF1_I2C1           ((uint8_t)0x01)  /* I2C1 Alternate Function mapping */
#define GPIO_AF1_IR             ((uint8_t)0x01)  /* IR Alternate Function mapping */
#define GPIO_AF1_USART1         ((uint8_t)0x01)  /* USART1 Alternate Function mapping */
#define GPIO_AF1_USART2         ((uint8_t)0x01)  /* USART2 Alternate Function mapping */
#define GPIO_AF1_TIM3           ((uint8_t)0x01)  /* TIM3 Alternate Function mapping */

/** 
  * @brief   AF 2 selection on PA, PB
  */ 
#define GPIO_AF2_EVENTOUT       ((uint8_t)0x02)  /* EVENTOUT Alternate Function mapping */
#define GPIO_AF2_TIM1           ((uint8_t)0x02)  /* TIM1 Alternate Function mapping */
#define GPIO_AF2_TIM2           ((uint8_t)0x02)  /* TIM2 Alternate Function mapping */
#define GPIO_AF2_TIM16          ((uint8_t)0x02)  /* TIM16 Alternate Function mapping */
#define GPIO_AF2_TIM17          ((uint8_t)0x02)  /* TIM17 Alternate Function mapping */
#define GPIO_AF2_USB            ((uint8_t)0x02)  /* USB Alternate Function mapping */

/** 
  * @brief   AF 3 selection  on PA, PB
  */ 
#define GPIO_AF3_EVENTOUT      ((uint8_t)0x03)  /* EVENTOUT Alternate Function mapping  */
#define GPIO_AF3_I2C1          ((uint8_t)0x03)  /* I2C1 Alternate Function mapping  */
#define GPIO_AF3_TSC           ((uint8_t)0x03)  /* TSC Alternate Function mapping  */

/** 
  * @brief   AF 4 selection  on PA, PB
  */ 
#define GPIO_AF4_TIM14          ((uint8_t)0x04)  /* TIM14 Alternate Function mapping */
#define GPIO_AF4_CAN            ((uint8_t)0x04)  /* CAN Alternate Function mapping */
#define GPIO_AF4_CRS            ((uint8_t)0x04)  /* CRS Alternate Function mapping */
#define GPIO_AF4_I2C1           ((uint8_t)0x04)  /* I2C1 Alternate Function mapping */

/** 
  * @brief   AF 5 selection  on PA, PB
  */ 
#define GPIO_AF5_MCO            ((uint8_t)0x05)  /* MCO Alternate Function mapping */
#define GPIO_AF5_I2C1           ((uint8_t)0x05)  /* I2C1 Alternate Function mapping */
#define GPIO_AF5_I2C2           ((uint8_t)0x05)  /* I2C2 Alternate Function mapping */
#define GPIO_AF5_SPI2           ((uint8_t)0x05)  /* SPI2 Alternate Function mapping */
#define GPIO_AF5_TIM16          ((uint8_t)0x05)  /* TIM16 Alternate Function mapping */
#define GPIO_AF5_TIM17          ((uint8_t)0x05)  /* TIM17 Alternate Function mapping */
#define GPIO_AF5_USB            ((uint8_t)0x05)  /* USB Alternate Function mapping */

/** 
  * @brief   AF 6 selection  on PA
  */ 
#define GPIO_AF6_EVENTOUT       ((uint8_t)0x06)  /* EVENTOUT Alternate Function mapping */

#define IS_GPIO_AF(AF)          ((AF) <= (uint8_t)0x06)

#endif /* STM32F042x6 || STM32F048xx */

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

#endif /* __STM32F0xx_HAL_GPIO_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
