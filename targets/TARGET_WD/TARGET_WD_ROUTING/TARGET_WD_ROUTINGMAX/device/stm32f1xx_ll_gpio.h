/**
  ******************************************************************************
  * @file    stm32f1xx_ll_gpio.h
  * @author  MCD Application Team
  * @version $VERSION$
  * @date    $DATE$
  * @brief   Header file of GPIO LL module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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
#ifndef __STM32F1xx_LL_GPIO_H
#define __STM32F1xx_LL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx.h"

/** @addtogroup STM32F1xx_LL_Driver
  * @{
  */

#if defined (GPIOA) || defined (GPIOB) || defined (GPIOC) || defined (GPIOD) || defined (GPIOE) || defined (GPIOF) || defined (GPIOG)

/** @defgroup GPIO_LL GPIO
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/** @defgroup GPIO_LL_Private_Constants GPIO Private Constants
  * @{
  */
/**
  * @}
  */


/* Private macros ------------------------------------------------------------*/
#if defined(USE_FULL_LL_DRIVER)
/** @defgroup GPIO_LL_Private_Macros GPIO Private Macros
  * @{
  */

/**
  * @}
  */
#endif /*USE_FULL_LL_DRIVER*/

/* Exported types ------------------------------------------------------------*/
#if defined(USE_FULL_LL_DRIVER)
/** @defgroup GPIO_LL_ES_INIT GPIO Exported Init structures
  * @{
  */

/**
  * @brief LL GPIO Init Structure definition
  */
typedef struct
{
  uint32_t Pin;          /*!< Specifies the GPIO pins to be configured.
                              This parameter can be any value of @ref GPIO_LL_EC_PIN */

  uint32_t Mode;         /*!< Specifies the operating mode for the selected pins.
                              This parameter can be a value of @ref GPIO_LL_EC_MODE.

                              GPIO HW configuration can be modified afterwards using unitary function @ref LL_GPIO_SetPinMode().*/

  uint32_t Speed;        /*!< Specifies the speed for the selected pins.
                              This parameter can be a value of @ref GPIO_LL_EC_SPEED.

                              GPIO HW configuration can be modified afterwards using unitary function @ref LL_GPIO_SetPinSpeed().*/

  uint32_t OutputType;   /*!< Specifies the operating output type for the selected pins.
                              This parameter can be a value of @ref GPIO_LL_EC_OUTPUT.

                              GPIO HW configuration can be modified afterwards using unitary function @ref LL_GPIO_SetPinOutputType().*/

  uint32_t Pull;         /*!< Specifies the operating Pull-up/Pull down for the selected pins.
                              This parameter can be a value of @ref GPIO_LL_EC_PULL.

                              GPIO HW configuration can be modified afterwards using unitary function @ref LL_GPIO_SetPinPull().*/
}LL_GPIO_InitTypeDef;

/**
  * @}
  */
#endif /* USE_FULL_LL_DRIVER */

/* Exported constants --------------------------------------------------------*/
/** @defgroup GPIO_LL_Exported_Constants GPIO Exported Constants
  * @{
  */

/** @defgroup GPIO_LL_EC_PIN PIN
  * @{
  */
#define LL_GPIO_PIN_0         (GPIO_BSRR_BS0 << 8)  | 0x00000001U  /*!< Select pin 0 */
#define LL_GPIO_PIN_1         (GPIO_BSRR_BS1 << 8)  | 0x00000002U  /*!< Select pin 1 */
#define LL_GPIO_PIN_2         (GPIO_BSRR_BS2 << 8)  | 0x00000004U  /*!< Select pin 2 */
#define LL_GPIO_PIN_3         (GPIO_BSRR_BS3 << 8)  | 0x00000008U  /*!< Select pin 3 */
#define LL_GPIO_PIN_4         (GPIO_BSRR_BS4 << 8)  | 0x00000010U  /*!< Select pin 4 */
#define LL_GPIO_PIN_5         (GPIO_BSRR_BS5 << 8)  | 0x00000020U  /*!< Select pin 5 */
#define LL_GPIO_PIN_6         (GPIO_BSRR_BS6 << 8)  | 0x00000040U  /*!< Select pin 6 */
#define LL_GPIO_PIN_7         (GPIO_BSRR_BS7 << 8)  | 0x00000080U  /*!< Select pin 7 */
#define LL_GPIO_PIN_8         (GPIO_BSRR_BS8 << 8)  | 0x04000001U  /*!< Select pin 8 */
#define LL_GPIO_PIN_9         (GPIO_BSRR_BS9 << 8)  | 0x04000002U  /*!< Select pin 9 */
#define LL_GPIO_PIN_10        (GPIO_BSRR_BS10 << 8) | 0x04000004U  /*!< Select pin 10 */
#define LL_GPIO_PIN_11        (GPIO_BSRR_BS11 << 8) | 0x04000008U  /*!< Select pin 11 */
#define LL_GPIO_PIN_12        (GPIO_BSRR_BS12 << 8) | 0x04000010U  /*!< Select pin 12 */
#define LL_GPIO_PIN_13        (GPIO_BSRR_BS13 << 8) | 0x04000020U  /*!< Select pin 13 */
#define LL_GPIO_PIN_14        (GPIO_BSRR_BS14 << 8) | 0x04000040U  /*!< Select pin 14 */
#define LL_GPIO_PIN_15        (GPIO_BSRR_BS15 << 8) | 0x04000080U  /*!< Select pin 15 */
#define LL_GPIO_PIN_ALL       (LL_GPIO_PIN_0 | LL_GPIO_PIN_1  | LL_GPIO_PIN_2  | \
                              LL_GPIO_PIN_3  | LL_GPIO_PIN_4  | LL_GPIO_PIN_5  | \
                              LL_GPIO_PIN_6  | LL_GPIO_PIN_7  | LL_GPIO_PIN_8  | \
                              LL_GPIO_PIN_9  | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | \
                              LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | \
                              LL_GPIO_PIN_15) /*!< Select all pins */
/**
  * @}
  */

/** @defgroup GPIO_LL_EC_MODE Mode
  * @{
  */
#define LL_GPIO_MODE_ANALOG                0x00000000U   /*!< Select analog mode */
#define LL_GPIO_MODE_FLOATING              GPIO_CRL_CNF0_0   /*!< Select floating mode */
#define LL_GPIO_MODE_INPUT                 GPIO_CRL_CNF0_1   /*!< Select input mode */
#define LL_GPIO_MODE_OUTPUT                GPIO_CRL_MODE0_0   /*!< Select general purpose output mode */
#define LL_GPIO_MODE_ALTERNATE             (GPIO_CRL_CNF0_1 | GPIO_CRL_MODE0_0) /*!< Select alternate function mode */
/**
  * @}
  */

/** @defgroup GPIO_LL_EC_OUTPUT Output Type
  * @{
  */
#define LL_GPIO_OUTPUT_PUSHPULL            0x00000000U /*!< Select push-pull as output type */
#define LL_GPIO_OUTPUT_OPENDRAIN           GPIO_CRL_CNF0_0 /*!< Select open-drain as output type */
/**
  * @}
  */

/** @defgroup GPIO_LL_EC_SPEED Output Speed
  * @{
  */
#define LL_GPIO_MODE_OUTPUT_10MHz      GPIO_CRL_MODE0_0            /*!< Select Output mode, max speed 10 MHz */
#define LL_GPIO_MODE_OUTPUT_2MHz       GPIO_CRL_MODE0_1            /*!< Select Output mode, max speed 20 MHz */
#define LL_GPIO_MODE_OUTPUT_50MHz      GPIO_CRL_MODE0              /*!< Select Output mode, max speed 50 MHz */
/**
  * @}
  */
  
#define LL_GPIO_SPEED_FREQ_LOW           LL_GPIO_MODE_OUTPUT_2MHz    /*!< Select I/O low output speed    */
#define LL_GPIO_SPEED_FREQ_MEDIUM        LL_GPIO_MODE_OUTPUT_10MHz   /*!< Select I/O medium output speed */
#define LL_GPIO_SPEED_FREQ_HIGH          LL_GPIO_MODE_OUTPUT_50MHz   /*!< Select I/O high output speed   */

/** @defgroup GPIO_LL_EC_PULL Pull Up Pull Down
  * @{
  */
#define LL_GPIO_PULL_DOWN                  0x00000000U    /*!< Select I/O pull down */
#define LL_GPIO_PULL_UP                    GPIO_ODR_ODR0  /*!< Select I/O pull up */

/**
  * @}
  */
  
/** @defgroup GPIO_LL_EVENTOUT_PIN EVENTOUT Pin
  * @{
  */

#define LL_GPIO_AFIO_EVENTOUT_PIN_0      AFIO_EVCR_PIN_PX0  /*!< EVENTOUT on pin 0 */
#define LL_GPIO_AFIO_EVENTOUT_PIN_1      AFIO_EVCR_PIN_PX1  /*!< EVENTOUT on pin 1 */
#define LL_GPIO_AFIO_EVENTOUT_PIN_2      AFIO_EVCR_PIN_PX2  /*!< EVENTOUT on pin 2 */
#define LL_GPIO_AFIO_EVENTOUT_PIN_3      AFIO_EVCR_PIN_PX3  /*!< EVENTOUT on pin 3 */
#define LL_GPIO_AFIO_EVENTOUT_PIN_4      AFIO_EVCR_PIN_PX4  /*!< EVENTOUT on pin 4 */
#define LL_GPIO_AFIO_EVENTOUT_PIN_5      AFIO_EVCR_PIN_PX5  /*!< EVENTOUT on pin 5 */
#define LL_GPIO_AFIO_EVENTOUT_PIN_6      AFIO_EVCR_PIN_PX6  /*!< EVENTOUT on pin 6 */
#define LL_GPIO_AFIO_EVENTOUT_PIN_7      AFIO_EVCR_PIN_PX7  /*!< EVENTOUT on pin 7 */
#define LL_GPIO_AFIO_EVENTOUT_PIN_8      AFIO_EVCR_PIN_PX8  /*!< EVENTOUT on pin 8 */
#define LL_GPIO_AFIO_EVENTOUT_PIN_9      AFIO_EVCR_PIN_PX9  /*!< EVENTOUT on pin 9 */
#define LL_GPIO_AFIO_EVENTOUT_PIN_10     AFIO_EVCR_PIN_PX10 /*!< EVENTOUT on pin 10 */
#define LL_GPIO_AFIO_EVENTOUT_PIN_11     AFIO_EVCR_PIN_PX11 /*!< EVENTOUT on pin 11 */
#define LL_GPIO_AFIO_EVENTOUT_PIN_12     AFIO_EVCR_PIN_PX12 /*!< EVENTOUT on pin 12 */
#define LL_GPIO_AFIO_EVENTOUT_PIN_13     AFIO_EVCR_PIN_PX13 /*!< EVENTOUT on pin 13 */
#define LL_GPIO_AFIO_EVENTOUT_PIN_14     AFIO_EVCR_PIN_PX14 /*!< EVENTOUT on pin 14 */
#define LL_GPIO_AFIO_EVENTOUT_PIN_15     AFIO_EVCR_PIN_PX15 /*!< EVENTOUT on pin 15 */

/**
  * @}
  */

/** @defgroup GPIO_LL_EVENTOUT_PORT EVENTOUT Port
  * @{
  */

#define LL_GPIO_AFIO_EVENTOUT_PORT_A     AFIO_EVCR_PORT_PA /*!< EVENTOUT on port A */
#define LL_GPIO_AFIO_EVENTOUT_PORT_B     AFIO_EVCR_PORT_PB /*!< EVENTOUT on port B */
#define LL_GPIO_AFIO_EVENTOUT_PORT_C     AFIO_EVCR_PORT_PC /*!< EVENTOUT on port C */
#define LL_GPIO_AFIO_EVENTOUT_PORT_D     AFIO_EVCR_PORT_PD /*!< EVENTOUT on port D */
#define LL_GPIO_AFIO_EVENTOUT_PORT_E     AFIO_EVCR_PORT_PE /*!< EVENTOUT on port E */

/**
  * @}
  */

/** @defgroup GPIO_LL_EC_EXTI_PORT GPIO EXTI PORT
  * @{
  */
#define LL_GPIO_AFIO_EXTI_PORTA               (uint32_t)0               /*!< EXTI PORT A */
#define LL_GPIO_AFIO_EXTI_PORTB               (uint32_t)1               /*!< EXTI PORT B */
#define LL_GPIO_AFIO_EXTI_PORTC               (uint32_t)2               /*!< EXTI PORT C */
#define LL_GPIO_AFIO_EXTI_PORTD               (uint32_t)3               /*!< EXTI PORT D */
#define LL_GPIO_AFIO_EXTI_PORTE               (uint32_t)4               /*!< EXTI PORT E */
#define LL_GPIO_AFIO_EXTI_PORTF               (uint32_t)5               /*!< EXTI PORT F */
#define LL_GPIO_AFIO_EXTI_PORTG               (uint32_t)6               /*!< EXTI PORT G */
/**
  * @}
  */

/** @defgroup GPIO_LL_EC_EXTI_LINE GPIO EXTI LINE
  * @{
  */
#define LL_GPIO_AFIO_EXTI_LINE0               (uint32_t)(0x000FU << 16 | 0)  /*!< EXTI_POSITION_0  | EXTICR[0] */
#define LL_GPIO_AFIO_EXTI_LINE1               (uint32_t)(0x00F0U << 16 | 0)  /*!< EXTI_POSITION_4  | EXTICR[0] */
#define LL_GPIO_AFIO_EXTI_LINE2               (uint32_t)(0x0F00U << 16 | 0)  /*!< EXTI_POSITION_8  | EXTICR[0] */
#define LL_GPIO_AFIO_EXTI_LINE3               (uint32_t)(0xF000U << 16 | 0)  /*!< EXTI_POSITION_12 | EXTICR[0] */
#define LL_GPIO_AFIO_EXTI_LINE4               (uint32_t)(0x000FU << 16 | 1)  /*!< EXTI_POSITION_0  | EXTICR[1] */
#define LL_GPIO_AFIO_EXTI_LINE5               (uint32_t)(0x00F0U << 16 | 1)  /*!< EXTI_POSITION_4  | EXTICR[1] */
#define LL_GPIO_AFIO_EXTI_LINE6               (uint32_t)(0x0F00U << 16 | 1)  /*!< EXTI_POSITION_8  | EXTICR[1] */
#define LL_GPIO_AFIO_EXTI_LINE7               (uint32_t)(0xF000U << 16 | 1)  /*!< EXTI_POSITION_12 | EXTICR[1] */
#define LL_GPIO_AFIO_EXTI_LINE8               (uint32_t)(0x000FU << 16 | 2)  /*!< EXTI_POSITION_0  | EXTICR[2] */
#define LL_GPIO_AFIO_EXTI_LINE9               (uint32_t)(0x00F0U << 16 | 2)  /*!< EXTI_POSITION_4  | EXTICR[2] */
#define LL_GPIO_AFIO_EXTI_LINE10              (uint32_t)(0x0F00U << 16 | 2)  /*!< EXTI_POSITION_8  | EXTICR[2] */
#define LL_GPIO_AFIO_EXTI_LINE11              (uint32_t)(0xF000U << 16 | 2)  /*!< EXTI_POSITION_12 | EXTICR[2] */
#define LL_GPIO_AFIO_EXTI_LINE12              (uint32_t)(0x000FU << 16 | 3)  /*!< EXTI_POSITION_0  | EXTICR[3] */
#define LL_GPIO_AFIO_EXTI_LINE13              (uint32_t)(0x00F0U << 16 | 3)  /*!< EXTI_POSITION_4  | EXTICR[3] */
#define LL_GPIO_AFIO_EXTI_LINE14              (uint32_t)(0x0F00U << 16 | 3)  /*!< EXTI_POSITION_8  | EXTICR[3] */
#define LL_GPIO_AFIO_EXTI_LINE15              (uint32_t)(0xF000U << 16 | 3)  /*!< EXTI_POSITION_12 | EXTICR[3] */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup GPIO_LL_Exported_Macros GPIO Exported Macros
  * @{
  */

/** @defgroup GPIO_LL_EM_WRITE_READ Common Write and read registers Macros
  * @{
  */

/**
  * @brief  Write a value in GPIO register
  * @param  __INSTANCE__ GPIO Instance
  * @param  __REG__ Register to be written
  * @param  __VALUE__ Value to be written in the register
  * @retval None
  */
#define LL_GPIO_WriteReg(__INSTANCE__, __REG__, __VALUE__) WRITE_REG(__INSTANCE__->__REG__, (__VALUE__))

/**
  * @brief  Read a value in GPIO register
  * @param  __INSTANCE__ GPIO Instance
  * @param  __REG__ Register to be read
  * @retval Register value
  */
#define LL_GPIO_ReadReg(__INSTANCE__, __REG__) READ_REG(__INSTANCE__->__REG__)
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup GPIO_LL_Exported_Functions GPIO Exported Functions
  * @{
  */

/** @defgroup GPIO_LL_EF_Port_Configuration Port Configuration
  * @{
  */

/**
  * @brief  Configure gpio mode for a dedicated pin on dedicated port.
  * @note   I/O mode can be Analog, Floating input, Input with pull-up/pull-down, General purpose Output,
  *         Alternate function Output.
  * @note   Warning: only one pin can be passed as parameter.
  * @rmtoll CRL        CNFy         LL_GPIO_SetPinMode
  * @rmtoll CRL        MODEy        LL_GPIO_SetPinMode
  * @rmtoll CRH        CNFy         LL_GPIO_SetPinMode
  * @rmtoll CRH        MODEy        LL_GPIO_SetPinMode
  * @param  GPIOx GPIO Port
  * @param  Pin This parameter can be one of the following values:
  *         @arg @ref LL_GPIO_PIN_0
  *         @arg @ref LL_GPIO_PIN_1
  *         @arg @ref LL_GPIO_PIN_2
  *         @arg @ref LL_GPIO_PIN_3
  *         @arg @ref LL_GPIO_PIN_4
  *         @arg @ref LL_GPIO_PIN_5
  *         @arg @ref LL_GPIO_PIN_6
  *         @arg @ref LL_GPIO_PIN_7
  *         @arg @ref LL_GPIO_PIN_8
  *         @arg @ref LL_GPIO_PIN_9
  *         @arg @ref LL_GPIO_PIN_10
  *         @arg @ref LL_GPIO_PIN_11
  *         @arg @ref LL_GPIO_PIN_12
  *         @arg @ref LL_GPIO_PIN_13
  *         @arg @ref LL_GPIO_PIN_14
  *         @arg @ref LL_GPIO_PIN_15
  * @param  Mode This parameter can be one of the following values:
  *         @arg @ref LL_GPIO_MODE_ANALOG
  *         @arg @ref LL_GPIO_MODE_FLOATING
  *         @arg @ref LL_GPIO_MODE_INPUT
  *         @arg @ref LL_GPIO_MODE_OUTPUT
  *         @arg @ref LL_GPIO_MODE_ALTERNATE
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_SetPinMode(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode)
{
  register uint32_t *pReg = (uint32_t *)((uint32_t)((uint32_t)(&GPIOx->CRL) + (Pin>>24)));
  MODIFY_REG(*pReg, ((GPIO_CRL_CNF0|GPIO_CRL_MODE0) << (POSITION_VAL(Pin) * 4U)), (Mode << (POSITION_VAL(Pin) * 4U)));
}

/**
  * @brief  Return gpio mode for a dedicated pin on dedicated port.
  * @note   I/O mode can be Analog, Floating input, Input with pull-up/pull-down, General purpose Output,
  *         Alternate function Output.
  * @note   Warning: only one pin can be passed as parameter.
  * @rmtoll CRL        CNFy         LL_GPIO_GetPinMode
  * @rmtoll CRL        MODEy        LL_GPIO_GetPinMode
  * @rmtoll CRH        CNFy         LL_GPIO_GetPinMode
  * @rmtoll CRH        MODEy        LL_GPIO_GetPinMode
  * @param  GPIOx GPIO Port
  * @param  Pin This parameter can be one of the following values:
  *         @arg @ref LL_GPIO_PIN_0
  *         @arg @ref LL_GPIO_PIN_1
  *         @arg @ref LL_GPIO_PIN_2
  *         @arg @ref LL_GPIO_PIN_3
  *         @arg @ref LL_GPIO_PIN_4
  *         @arg @ref LL_GPIO_PIN_5
  *         @arg @ref LL_GPIO_PIN_6
  *         @arg @ref LL_GPIO_PIN_7
  *         @arg @ref LL_GPIO_PIN_8
  *         @arg @ref LL_GPIO_PIN_9
  *         @arg @ref LL_GPIO_PIN_10
  *         @arg @ref LL_GPIO_PIN_11
  *         @arg @ref LL_GPIO_PIN_12
  *         @arg @ref LL_GPIO_PIN_13
  *         @arg @ref LL_GPIO_PIN_14
  *         @arg @ref LL_GPIO_PIN_15
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_GPIO_MODE_ANALOG
  *         @arg @ref LL_GPIO_MODE_FLOATING
  *         @arg @ref LL_GPIO_MODE_INPUT
  *         @arg @ref LL_GPIO_MODE_OUTPUT
  *         @arg @ref LL_GPIO_MODE_ALTERNATE
  */
__STATIC_INLINE uint32_t LL_GPIO_GetPinMode(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
  register uint32_t *pReg = (uint32_t *)((uint32_t)((uint32_t)(&GPIOx->CRL) + (Pin>>24)));
  return (uint32_t)(READ_BIT(*pReg,
                             ((GPIO_CRL_CNF0|GPIO_CRL_MODE0) << (POSITION_VAL(Pin) * 4U))) >> (POSITION_VAL(Pin) * 4U));
}

/**
  * @brief  Configure gpio speed for a dedicated pin on dedicated port.
  * @note   I/O speed can be Low, Medium or Fast speed.
  * @note   Warning: only one pin can be passed as parameter.
  * @note   Refer to datasheet for frequency specifications and the power
  *         supply and load conditions for each speed.
  * @rmtoll CRL      MODEy       LL_GPIO_SetPinSpeed
  * @rmtoll CRH      MODEy       LL_GPIO_SetPinSpeed
  * @param  GPIOx GPIO Port
  * @param  Pin This parameter can be one of the following values:
  *         @arg @ref LL_GPIO_PIN_0
  *         @arg @ref LL_GPIO_PIN_1
  *         @arg @ref LL_GPIO_PIN_2
  *         @arg @ref LL_GPIO_PIN_3
  *         @arg @ref LL_GPIO_PIN_4
  *         @arg @ref LL_GPIO_PIN_5
  *         @arg @ref LL_GPIO_PIN_6
  *         @arg @ref LL_GPIO_PIN_7
  *         @arg @ref LL_GPIO_PIN_8
  *         @arg @ref LL_GPIO_PIN_9
  *         @arg @ref LL_GPIO_PIN_10
  *         @arg @ref LL_GPIO_PIN_11
  *         @arg @ref LL_GPIO_PIN_12
  *         @arg @ref LL_GPIO_PIN_13
  *         @arg @ref LL_GPIO_PIN_14
  *         @arg @ref LL_GPIO_PIN_15
  * @param  Speed This parameter can be one of the following values:
  *         @arg @ref LL_GPIO_SPEED_FREQ_LOW
  *         @arg @ref LL_GPIO_SPEED_FREQ_MEDIUM
  *         @arg @ref LL_GPIO_SPEED_FREQ_HIGH
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_SetPinSpeed(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t  Speed)
{
  register uint32_t *pReg = (uint32_t *)((uint32_t)((uint32_t)(&GPIOx->CRL) + (Pin>>24)));
  MODIFY_REG(*pReg, (GPIO_CRL_MODE0 << (POSITION_VAL(Pin) * 4U)),
             (Speed << (POSITION_VAL(Pin) * 4U)));
}

/**
  * @brief  Return gpio speed for a dedicated pin on dedicated port.
  * @note   I/O speed can be Low, Medium, Fast or High speed.
  * @note   Warning: only one pin can be passed as parameter.
  * @note   Refer to datasheet for frequency specifications and the power
  *         supply and load conditions for each speed.
  * @rmtoll CRL      MODEy       LL_GPIO_GetPinSpeed
  * @rmtoll CRH      MODEy       LL_GPIO_GetPinSpeed
  * @param  GPIOx GPIO Port
  * @param  Pin This parameter can be one of the following values:
  *         @arg @ref LL_GPIO_PIN_0
  *         @arg @ref LL_GPIO_PIN_1
  *         @arg @ref LL_GPIO_PIN_2
  *         @arg @ref LL_GPIO_PIN_3
  *         @arg @ref LL_GPIO_PIN_4
  *         @arg @ref LL_GPIO_PIN_5
  *         @arg @ref LL_GPIO_PIN_6
  *         @arg @ref LL_GPIO_PIN_7
  *         @arg @ref LL_GPIO_PIN_8
  *         @arg @ref LL_GPIO_PIN_9
  *         @arg @ref LL_GPIO_PIN_10
  *         @arg @ref LL_GPIO_PIN_11
  *         @arg @ref LL_GPIO_PIN_12
  *         @arg @ref LL_GPIO_PIN_13
  *         @arg @ref LL_GPIO_PIN_14
  *         @arg @ref LL_GPIO_PIN_15
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_GPIO_SPEED_FREQ_LOW
  *         @arg @ref LL_GPIO_SPEED_FREQ_MEDIUM
  *         @arg @ref LL_GPIO_SPEED_FREQ_HIGH
  */
__STATIC_INLINE uint32_t LL_GPIO_GetPinSpeed(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
  register uint32_t *pReg = (uint32_t *)((uint32_t)((uint32_t)(&GPIOx->CRL) + (Pin>>24)));
  return (uint32_t)(READ_BIT(*pReg,
                             (GPIO_CRL_MODE0 << (POSITION_VAL(Pin) * 4U))) >> (POSITION_VAL(Pin) * 4U));
}

/**
  * @brief  Configure gpio output type for several pins on dedicated port.
  * @note   Output type as to be set when gpio pin is in output or
  *         alternate modes. Possible type are Push-pull or Open-drain.
  * @rmtoll CRL       MODEy           LL_GPIO_SetPinOutputType
  * @rmtoll CRH       MODEy           LL_GPIO_SetPinOutputType
  * @param  GPIOx GPIO Port
  * @param  Pin This parameter can be a combination of the following values:
  *         @arg @ref LL_GPIO_PIN_0
  *         @arg @ref LL_GPIO_PIN_1
  *         @arg @ref LL_GPIO_PIN_2
  *         @arg @ref LL_GPIO_PIN_3
  *         @arg @ref LL_GPIO_PIN_4
  *         @arg @ref LL_GPIO_PIN_5
  *         @arg @ref LL_GPIO_PIN_6
  *         @arg @ref LL_GPIO_PIN_7
  *         @arg @ref LL_GPIO_PIN_8
  *         @arg @ref LL_GPIO_PIN_9
  *         @arg @ref LL_GPIO_PIN_10
  *         @arg @ref LL_GPIO_PIN_11
  *         @arg @ref LL_GPIO_PIN_12
  *         @arg @ref LL_GPIO_PIN_13
  *         @arg @ref LL_GPIO_PIN_14
  *         @arg @ref LL_GPIO_PIN_15
  *         @arg @ref LL_GPIO_PIN_ALL
  * @param  OutputType This parameter can be one of the following values:
  *         @arg @ref LL_GPIO_OUTPUT_PUSHPULL
  *         @arg @ref LL_GPIO_OUTPUT_OPENDRAIN
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_SetPinOutputType(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t OutputType)
{
  register uint32_t *pReg = (uint32_t *)((uint32_t)((uint32_t)(&GPIOx->CRL) + (Pin>>24)));
  MODIFY_REG(*pReg, (GPIO_CRL_CNF0_0 << (POSITION_VAL(Pin) * 4U)),
             (OutputType << (POSITION_VAL(Pin) * 4U)));
}

/**
  * @brief  Return gpio output type for several pins on dedicated port.
  * @note   Output type as to be set when gpio pin is in output or
  *         alternate modes. Possible type are Push-pull or Open-drain.
  * @note   Warning: only one pin can be passed as parameter.
  * @rmtoll CRL       MODEy       LL_GPIO_GetPinOutputType
  * @rmtoll CRH       MODEy       LL_GPIO_GetPinOutputType
  * @param  GPIOx GPIO Port
  * @param  Pin This parameter can be one of the following values:
  *         @arg @ref LL_GPIO_PIN_0
  *         @arg @ref LL_GPIO_PIN_1
  *         @arg @ref LL_GPIO_PIN_2
  *         @arg @ref LL_GPIO_PIN_3
  *         @arg @ref LL_GPIO_PIN_4
  *         @arg @ref LL_GPIO_PIN_5
  *         @arg @ref LL_GPIO_PIN_6
  *         @arg @ref LL_GPIO_PIN_7
  *         @arg @ref LL_GPIO_PIN_8
  *         @arg @ref LL_GPIO_PIN_9
  *         @arg @ref LL_GPIO_PIN_10
  *         @arg @ref LL_GPIO_PIN_11
  *         @arg @ref LL_GPIO_PIN_12
  *         @arg @ref LL_GPIO_PIN_13
  *         @arg @ref LL_GPIO_PIN_14
  *         @arg @ref LL_GPIO_PIN_15
  *         @arg @ref LL_GPIO_PIN_ALL
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_GPIO_OUTPUT_PUSHPULL
  *         @arg @ref LL_GPIO_OUTPUT_OPENDRAIN
  */
__STATIC_INLINE uint32_t LL_GPIO_GetPinOutputType(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
  register uint32_t *pReg = (uint32_t *)((uint32_t)((uint32_t)(&GPIOx->CRL) + (Pin>>24)));
  return (uint32_t)(READ_BIT(*pReg,
                            (GPIO_CRL_CNF0_0 << (POSITION_VAL(Pin) * 4U))) >> (POSITION_VAL(Pin) * 4U));

}

/**
  * @brief  Configure gpio pull-up or pull-down for a dedicated pin on a dedicated port.
  * @note   Warning: only one pin can be passed as parameter.
  * @rmtoll ODR        ODR         LL_GPIO_SetPinPull
  * @param  GPIOx GPIO Port
  * @param  Pin This parameter can be one of the following values:
  *         @arg @ref LL_GPIO_PIN_0
  *         @arg @ref LL_GPIO_PIN_1
  *         @arg @ref LL_GPIO_PIN_2
  *         @arg @ref LL_GPIO_PIN_3
  *         @arg @ref LL_GPIO_PIN_4
  *         @arg @ref LL_GPIO_PIN_5
  *         @arg @ref LL_GPIO_PIN_6
  *         @arg @ref LL_GPIO_PIN_7
  *         @arg @ref LL_GPIO_PIN_8
  *         @arg @ref LL_GPIO_PIN_9
  *         @arg @ref LL_GPIO_PIN_10
  *         @arg @ref LL_GPIO_PIN_11
  *         @arg @ref LL_GPIO_PIN_12
  *         @arg @ref LL_GPIO_PIN_13
  *         @arg @ref LL_GPIO_PIN_14
  *         @arg @ref LL_GPIO_PIN_15
  * @param  Pull This parameter can be one of the following values:
  *         @arg @ref LL_GPIO_PULL_DOWN
  *         @arg @ref LL_GPIO_PULL_UP
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_SetPinPull(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Pull)
{
  MODIFY_REG(GPIOx->ODR, (Pin>>8) , Pull << (POSITION_VAL(Pin>>8)));
}

/**
  * @brief  Return gpio pull-up or pull-down for a dedicated pin on a dedicated port
  * @note   Warning: only one pin can be passed as parameter.
  * @rmtoll ODR        ODR         LL_GPIO_GetPinPull
  * @param  GPIOx GPIO Port
  * @param  Pin This parameter can be one of the following values:
  *         @arg @ref LL_GPIO_PIN_0
  *         @arg @ref LL_GPIO_PIN_1
  *         @arg @ref LL_GPIO_PIN_2
  *         @arg @ref LL_GPIO_PIN_3
  *         @arg @ref LL_GPIO_PIN_4
  *         @arg @ref LL_GPIO_PIN_5
  *         @arg @ref LL_GPIO_PIN_6
  *         @arg @ref LL_GPIO_PIN_7
  *         @arg @ref LL_GPIO_PIN_8
  *         @arg @ref LL_GPIO_PIN_9
  *         @arg @ref LL_GPIO_PIN_10
  *         @arg @ref LL_GPIO_PIN_11
  *         @arg @ref LL_GPIO_PIN_12
  *         @arg @ref LL_GPIO_PIN_13
  *         @arg @ref LL_GPIO_PIN_14
  *         @arg @ref LL_GPIO_PIN_15
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_GPIO_PULL_DOWN
  *         @arg @ref LL_GPIO_PULL_UP
  */
__STATIC_INLINE uint32_t LL_GPIO_GetPinPull(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
  return (uint32_t)(READ_BIT(GPIOx->ODR,
                             (GPIO_ODR_ODR0 << (POSITION_VAL(Pin>>8)))) >> (POSITION_VAL(Pin>>8)));
}

/**
  * @brief  Lock configuration of several pins for a dedicated port.
  * @note   When the lock sequence has been applied on a port bit, the
  *         value of this port bit can no longer be modified until the
  *         next reset.
  * @note   Each lock bit freezes a specific configuration register
  *         (control and alternate function registers).
  * @rmtoll LCKR         LCKK          LL_GPIO_LockPin
  * @param  GPIOx GPIO Port
  * @param  PinMask This parameter can be a combination of the following values:
  *         @arg @ref LL_GPIO_PIN_0
  *         @arg @ref LL_GPIO_PIN_1
  *         @arg @ref LL_GPIO_PIN_2
  *         @arg @ref LL_GPIO_PIN_3
  *         @arg @ref LL_GPIO_PIN_4
  *         @arg @ref LL_GPIO_PIN_5
  *         @arg @ref LL_GPIO_PIN_6
  *         @arg @ref LL_GPIO_PIN_7
  *         @arg @ref LL_GPIO_PIN_8
  *         @arg @ref LL_GPIO_PIN_9
  *         @arg @ref LL_GPIO_PIN_10
  *         @arg @ref LL_GPIO_PIN_11
  *         @arg @ref LL_GPIO_PIN_12
  *         @arg @ref LL_GPIO_PIN_13
  *         @arg @ref LL_GPIO_PIN_14
  *         @arg @ref LL_GPIO_PIN_15
  *         @arg @ref LL_GPIO_PIN_ALL
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_LockPin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  __IO uint32_t temp;
  WRITE_REG(GPIOx->LCKR, GPIO_LCKR_LCKK | ((PinMask >> 8) & 0x0000FFFFU));
  WRITE_REG(GPIOx->LCKR, ((PinMask >>8 ) & 0x0000FFFFU));
  WRITE_REG(GPIOx->LCKR, GPIO_LCKR_LCKK | ((PinMask>>8) & 0x0000FFFFU));
  temp = READ_REG(GPIOx->LCKR);
  (void) temp;
}

/**
  * @brief  Return 1 if all pins passed as parameter, of a dedicated port, are locked. else Return 0.
  * @rmtoll LCKR         LCKy          LL_GPIO_IsPinLocked
  * @param  GPIOx GPIO Port
  * @param  PinMask This parameter can be a combination of the following values:
  *         @arg @ref LL_GPIO_PIN_0
  *         @arg @ref LL_GPIO_PIN_1
  *         @arg @ref LL_GPIO_PIN_2
  *         @arg @ref LL_GPIO_PIN_3
  *         @arg @ref LL_GPIO_PIN_4
  *         @arg @ref LL_GPIO_PIN_5
  *         @arg @ref LL_GPIO_PIN_6
  *         @arg @ref LL_GPIO_PIN_7
  *         @arg @ref LL_GPIO_PIN_8
  *         @arg @ref LL_GPIO_PIN_9
  *         @arg @ref LL_GPIO_PIN_10
  *         @arg @ref LL_GPIO_PIN_11
  *         @arg @ref LL_GPIO_PIN_12
  *         @arg @ref LL_GPIO_PIN_13
  *         @arg @ref LL_GPIO_PIN_14
  *         @arg @ref LL_GPIO_PIN_15
  *         @arg @ref LL_GPIO_PIN_ALL
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_GPIO_IsPinLocked(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  return (READ_BIT(GPIOx->LCKR, ((PinMask >> 8 ) & 0x0000FFFFU)) == ((PinMask >>8 ) & 0x0000FFFFU));
}

/**
  * @brief  Return 1 if one of the pin of a dedicated port is locked. else return 0.
  * @rmtoll LCKR         LCKK          LL_GPIO_IsAnyPinLocked
  * @param  GPIOx GPIO Port
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_GPIO_IsAnyPinLocked(GPIO_TypeDef *GPIOx)
{
  return (READ_BIT(GPIOx->LCKR, GPIO_LCKR_LCKK) == (GPIO_LCKR_LCKK));
}

/**
  * @}
  */

/** @defgroup GPIO_LL_EF_Data_Access Data Access
  * @{
  */

/**
  * @brief  Return full input data register value for a dedicated port.
  * @rmtoll IDR          IDy           LL_GPIO_ReadInputPort
  * @param  GPIOx GPIO Port
  * @retval Input data register value of port
  */
__STATIC_INLINE uint32_t LL_GPIO_ReadInputPort(GPIO_TypeDef *GPIOx)
{
  return (uint32_t)(READ_REG(GPIOx->IDR));
}

/**
  * @brief  Return if input data level for several pins of dedicated port is high or low.
  * @rmtoll IDR          IDy           LL_GPIO_IsInputPinSet
  * @param  GPIOx GPIO Port
  * @param  PinMask This parameter can be a combination of the following values:
  *         @arg @ref LL_GPIO_PIN_0
  *         @arg @ref LL_GPIO_PIN_1
  *         @arg @ref LL_GPIO_PIN_2
  *         @arg @ref LL_GPIO_PIN_3
  *         @arg @ref LL_GPIO_PIN_4
  *         @arg @ref LL_GPIO_PIN_5
  *         @arg @ref LL_GPIO_PIN_6
  *         @arg @ref LL_GPIO_PIN_7
  *         @arg @ref LL_GPIO_PIN_8
  *         @arg @ref LL_GPIO_PIN_9
  *         @arg @ref LL_GPIO_PIN_10
  *         @arg @ref LL_GPIO_PIN_11
  *         @arg @ref LL_GPIO_PIN_12
  *         @arg @ref LL_GPIO_PIN_13
  *         @arg @ref LL_GPIO_PIN_14
  *         @arg @ref LL_GPIO_PIN_15
  *         @arg @ref LL_GPIO_PIN_ALL
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_GPIO_IsInputPinSet(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  return (READ_BIT(GPIOx->IDR, (PinMask >> 8 ) & 0x0000FFFFU) == ((PinMask >> 8 ) & 0x0000FFFFU));
}

/**
  * @brief  Write output data register for the port.
  * @rmtoll ODR          ODy           LL_GPIO_WriteOutputPort
  * @param  GPIOx GPIO Port
  * @param  PortValue Level value for each pin of the port
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_WriteOutputPort(GPIO_TypeDef *GPIOx, uint32_t PortValue)
{
  WRITE_REG(GPIOx->ODR, PortValue);
}

/**
  * @brief  Return full output data register value for a dedicated port.
  * @rmtoll ODR          ODy           LL_GPIO_ReadOutputPort
  * @param  GPIOx GPIO Port
  * @retval Output data register value of port
  */
__STATIC_INLINE uint32_t LL_GPIO_ReadOutputPort(GPIO_TypeDef *GPIOx)
{
  return (uint32_t)(READ_REG(GPIOx->ODR));
}

/**
  * @brief  Return if input data level for several pins of dedicated port is high or low.
  * @rmtoll ODR          ODy           LL_GPIO_IsOutputPinSet
  * @param  GPIOx GPIO Port
  * @param  PinMask This parameter can be a combination of the following values:
  *         @arg @ref LL_GPIO_PIN_0
  *         @arg @ref LL_GPIO_PIN_1
  *         @arg @ref LL_GPIO_PIN_2
  *         @arg @ref LL_GPIO_PIN_3
  *         @arg @ref LL_GPIO_PIN_4
  *         @arg @ref LL_GPIO_PIN_5
  *         @arg @ref LL_GPIO_PIN_6
  *         @arg @ref LL_GPIO_PIN_7
  *         @arg @ref LL_GPIO_PIN_8
  *         @arg @ref LL_GPIO_PIN_9
  *         @arg @ref LL_GPIO_PIN_10
  *         @arg @ref LL_GPIO_PIN_11
  *         @arg @ref LL_GPIO_PIN_12
  *         @arg @ref LL_GPIO_PIN_13
  *         @arg @ref LL_GPIO_PIN_14
  *         @arg @ref LL_GPIO_PIN_15
  *         @arg @ref LL_GPIO_PIN_ALL
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_GPIO_IsOutputPinSet(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  return (READ_BIT(GPIOx->ODR, (PinMask >> 8 ) & 0x0000FFFFU) == ((PinMask >> 8 ) & 0x0000FFFFU));
}

/**
  * @brief  Set several pins to high level on dedicated gpio port.
  * @rmtoll BSRR         BSy           LL_GPIO_SetOutputPin
  * @param  GPIOx GPIO Port
  * @param  PinMask This parameter can be a combination of the following values:
  *         @arg @ref LL_GPIO_PIN_0
  *         @arg @ref LL_GPIO_PIN_1
  *         @arg @ref LL_GPIO_PIN_2
  *         @arg @ref LL_GPIO_PIN_3
  *         @arg @ref LL_GPIO_PIN_4
  *         @arg @ref LL_GPIO_PIN_5
  *         @arg @ref LL_GPIO_PIN_6
  *         @arg @ref LL_GPIO_PIN_7
  *         @arg @ref LL_GPIO_PIN_8
  *         @arg @ref LL_GPIO_PIN_9
  *         @arg @ref LL_GPIO_PIN_10
  *         @arg @ref LL_GPIO_PIN_11
  *         @arg @ref LL_GPIO_PIN_12
  *         @arg @ref LL_GPIO_PIN_13
  *         @arg @ref LL_GPIO_PIN_14
  *         @arg @ref LL_GPIO_PIN_15
  *         @arg @ref LL_GPIO_PIN_ALL
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_SetOutputPin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  WRITE_REG(GPIOx->BSRR, (PinMask >> 8) & 0x0000FFFFU);
}

/**
  * @brief  Set several pins to low level on dedicated gpio port.
  * @rmtoll BRR          BRy           LL_GPIO_ResetOutputPin
  * @param  GPIOx GPIO Port
  * @param  PinMask This parameter can be a combination of the following values:
  *         @arg @ref LL_GPIO_PIN_0
  *         @arg @ref LL_GPIO_PIN_1
  *         @arg @ref LL_GPIO_PIN_2
  *         @arg @ref LL_GPIO_PIN_3
  *         @arg @ref LL_GPIO_PIN_4
  *         @arg @ref LL_GPIO_PIN_5
  *         @arg @ref LL_GPIO_PIN_6
  *         @arg @ref LL_GPIO_PIN_7
  *         @arg @ref LL_GPIO_PIN_8
  *         @arg @ref LL_GPIO_PIN_9
  *         @arg @ref LL_GPIO_PIN_10
  *         @arg @ref LL_GPIO_PIN_11
  *         @arg @ref LL_GPIO_PIN_12
  *         @arg @ref LL_GPIO_PIN_13
  *         @arg @ref LL_GPIO_PIN_14
  *         @arg @ref LL_GPIO_PIN_15
  *         @arg @ref LL_GPIO_PIN_ALL
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_ResetOutputPin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  WRITE_REG(GPIOx->BRR, (PinMask >> 8 ) & 0x0000FFFFU);
}

/**
  * @brief  Toggle data value for several pin of dedicated port.
  * @rmtoll ODR          ODy           LL_GPIO_TogglePin
  * @param  GPIOx GPIO Port
  * @param  PinMask This parameter can be a combination of the following values:
  *         @arg @ref LL_GPIO_PIN_0
  *         @arg @ref LL_GPIO_PIN_1
  *         @arg @ref LL_GPIO_PIN_2
  *         @arg @ref LL_GPIO_PIN_3
  *         @arg @ref LL_GPIO_PIN_4
  *         @arg @ref LL_GPIO_PIN_5
  *         @arg @ref LL_GPIO_PIN_6
  *         @arg @ref LL_GPIO_PIN_7
  *         @arg @ref LL_GPIO_PIN_8
  *         @arg @ref LL_GPIO_PIN_9
  *         @arg @ref LL_GPIO_PIN_10
  *         @arg @ref LL_GPIO_PIN_11
  *         @arg @ref LL_GPIO_PIN_12
  *         @arg @ref LL_GPIO_PIN_13
  *         @arg @ref LL_GPIO_PIN_14
  *         @arg @ref LL_GPIO_PIN_15
  *         @arg @ref LL_GPIO_PIN_ALL
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  WRITE_REG(GPIOx->ODR, READ_REG(GPIOx->ODR) ^ ((PinMask >> 8 ) & 0x0000FFFFU));
}

/**
  * @}
  */

/** @defgroup GPIO_AFIO_AF_REMAPPING Alternate Function Remapping
  * @brief This section propose definition to remap the alternate function to some other port/pins.
  * @{
  */

/**
  * @brief  Enable the remapping of SPI1 alternate function NSS, SCK, MISO and MOSI.
  * @rmtoll MAPR          SPI1_REMAP           LL_GPIO_AFIO_REMAP_SPI1_ENABLE
  * @note   ENABLE: Remap (NSS/PA15, SCK/PB3, MISO/PB4, MOSI/PB5)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_SPI1_ENABLE(void)
{
  SET_BIT(AFIO->MAPR, AFIO_MAPR_SPI1_REMAP);
}

/**
  * @brief Disable the remapping of SPI1 alternate function NSS, SCK, MISO and MOSI.
  * @rmtoll MAPR          SPI1_REMAP           LL_GPIO_AFIO_REMAP_SPI1_DISABLE
  * @note  DISABLE: No remap (NSS/PA4,  SCK/PA5, MISO/PA6, MOSI/PA7)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_SPI1_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR, AFIO_MAPR_SPI1_REMAP);
}

/**
  * @brief Enable the remapping of I2C1 alternate function SCL and SDA.
  * @rmtoll MAPR          I2C1_REMAP           LL_GPIO_AFIO_REMAP_I2C1_ENABLE
  * @note  ENABLE: Remap     (SCL/PB8, SDA/PB9)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_I2C1_ENABLE(void)
{
  SET_BIT(AFIO->MAPR, AFIO_MAPR_I2C1_REMAP);
}

/**
  * @brief Disable the remapping of I2C1 alternate function SCL and SDA.
  * @rmtoll MAPR          I2C1_REMAP           LL_GPIO_AFIO_REMAP_I2C1_DISABLE
  * @note  DISABLE: No remap (SCL/PB6, SDA/PB7)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_I2C1_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR, AFIO_MAPR_I2C1_REMAP);
}

/**
  * @brief Enable the remapping of USART1 alternate function TX and RX.
  * @rmtoll MAPR          USART1_REMAP           LL_GPIO_AFIO_REMAP_USART1_ENABLE
  * @note  ENABLE: Remap     (TX/PB6, RX/PB7)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_USART1_ENABLE(void)
{
  SET_BIT(AFIO->MAPR, AFIO_MAPR_USART1_REMAP);
}

/**
  * @brief Disable the remapping of USART1 alternate function TX and RX.
  * @rmtoll MAPR          USART1_REMAP           LL_GPIO_AFIO_REMAP_USART1_DISABLE
  * @note  DISABLE: No remap (TX/PA9, RX/PA10)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_USART1_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR, AFIO_MAPR_USART1_REMAP);
}

/**
  * @brief Enable the remapping of USART2 alternate function CTS, RTS, CK, TX and RX.
  * @rmtoll MAPR          USART2_REMAP           LL_GPIO_AFIO_REMAP_USART2_ENABLE
  * @note  ENABLE: Remap     (CTS/PD3, RTS/PD4, TX/PD5, RX/PD6, CK/PD7)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_USART2_ENABLE(void)
{
  SET_BIT(AFIO->MAPR, AFIO_MAPR_USART2_REMAP);
}

/**
  * @brief Disable the remapping of USART2 alternate function CTS, RTS, CK, TX and RX.
  * @rmtoll MAPR          USART2_REMAP           LL_GPIO_AFIO_REMAP_USART2_DISABLE
  * @note  DISABLE: No remap (CTS/PA0, RTS/PA1, TX/PA2, RX/PA3, CK/PA4)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_USART2_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR, AFIO_MAPR_USART2_REMAP);
}

#if defined (AFIO_MAPR_USART3_REMAP)
/**
  * @brief Enable the remapping of USART3 alternate function CTS, RTS, CK, TX and RX.
  * @rmtoll MAPR          USART3_REMAP           LL_GPIO_AFIO_REMAP_USART3_ENABLE
  * @note  ENABLE: Full remap     (TX/PD8,  RX/PD9,  CK/PD10, CTS/PD11, RTS/PD12)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_USART3_ENABLE(void)
{
  MODIFY_REG(AFIO->MAPR, AFIO_MAPR_USART3_REMAP, AFIO_MAPR_USART3_REMAP_FULLREMAP);
}

/**
  * @brief Enable the remapping of USART3 alternate function CTS, RTS, CK, TX and RX.
  * @rmtoll MAPR          USART3_REMAP           LL_GPIO_AFIO_REMAP_USART3_PARTIAL
  * @note  PARTIAL: Partial remap (TX/PC10, RX/PC11, CK/PC12, CTS/PB13, RTS/PB14)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_USART3_PARTIAL(void)
{
  MODIFY_REG(AFIO->MAPR, AFIO_MAPR_USART3_REMAP, AFIO_MAPR_USART3_REMAP_PARTIALREMAP);
}

/**
  * @brief Disable the remapping of USART3 alternate function CTS, RTS, CK, TX and RX.
  * @rmtoll MAPR          USART3_REMAP           LL_GPIO_AFIO_REMAP_USART3_DISABLE
  * @note  DISABLE: No remap      (TX/PB10, RX/PB11, CK/PB12, CTS/PB13, RTS/PB14)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_USART3_DISABLE(void)
{
  MODIFY_REG(AFIO->MAPR, AFIO_MAPR_USART3_REMAP, AFIO_MAPR_USART3_REMAP_NOREMAP);
}
#endif

/**
  * @brief Enable the remapping of TIM1 alternate function channels 1 to 4, 1N to 3N, external trigger (ETR) and Break input (BKIN)
  * @rmtoll MAPR          TIM1_REMAP           LL_GPIO_AFIO_REMAP_TIM1_ENABLE
  * @note  ENABLE: Full remap     (ETR/PE7,  CH1/PE9, CH2/PE11, CH3/PE13, CH4/PE14, BKIN/PE15, CH1N/PE8,  CH2N/PE10, CH3N/PE12)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM1_ENABLE(void)
{
  MODIFY_REG(AFIO->MAPR, AFIO_MAPR_TIM1_REMAP, AFIO_MAPR_TIM1_REMAP_FULLREMAP);
}

/**
  * @brief Enable the remapping of TIM1 alternate function channels 1 to 4, 1N to 3N, external trigger (ETR) and Break input (BKIN)
  * @rmtoll MAPR          TIM1_REMAP           LL_GPIO_AFIO_REMAP_TIM1_PARTIAL
  * @note  PARTIAL: Partial remap (ETR/PA12, CH1/PA8, CH2/PA9,  CH3/PA10, CH4/PA11, BKIN/PA6,  CH1N/PA7,  CH2N/PB0,  CH3N/PB1)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM1_PARTIAL(void)
{
  MODIFY_REG(AFIO->MAPR, AFIO_MAPR_TIM1_REMAP, AFIO_MAPR_TIM1_REMAP_PARTIALREMAP);
}

/**
  * @brief Disable the remapping of TIM1 alternate function channels 1 to 4, 1N to 3N, external trigger (ETR) and Break input (BKIN)
  * @rmtoll MAPR          TIM1_REMAP           LL_GPIO_AFIO_REMAP_TIM1_DISABLE
  * @note  DISABLE: No remap      (ETR/PA12, CH1/PA8, CH2/PA9,  CH3/PA10, CH4/PA11, BKIN/PB12, CH1N/PB13, CH2N/PB14, CH3N/PB15)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM1_DISABLE(void)
{
  MODIFY_REG(AFIO->MAPR, AFIO_MAPR_TIM1_REMAP, AFIO_MAPR_TIM1_REMAP_NOREMAP);
}

/**
  * @brief Enable the remapping of TIM2 alternate function channels 1 to 4 and external trigger (ETR)
  * @rmtoll MAPR          TIM2_REMAP           LL_GPIO_AFIO_REMAP_TIM2_ENABLE
  * @note  ENABLE: Full remap       (CH1/ETR/PA15, CH2/PB3, CH3/PB10, CH4/PB11)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM2_ENABLE(void)
{
  MODIFY_REG(AFIO->MAPR, AFIO_MAPR_TIM2_REMAP, AFIO_MAPR_TIM2_REMAP_FULLREMAP);
}

/**
  * @brief Enable the remapping of TIM2 alternate function channels 1 to 4 and external trigger (ETR)
  * @rmtoll MAPR          TIM2_REMAP           LL_GPIO_AFIO_REMAP_TIM2_PARTIAL_2
  * @note  PARTIAL_2: Partial remap (CH1/ETR/PA0,  CH2/PA1, CH3/PB10, CH4/PB11)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM2_PARTIAL_2(void)
{
  MODIFY_REG(AFIO->MAPR, AFIO_MAPR_TIM2_REMAP, AFIO_MAPR_TIM2_REMAP_PARTIALREMAP2);
}

/**
  * @brief Enable the remapping of TIM2 alternate function channels 1 to 4 and external trigger (ETR)
  * @rmtoll MAPR          TIM2_REMAP           LL_GPIO_AFIO_REMAP_TIM2_PARTIAL_1
  * @note  PARTIAL_1: Partial remap (CH1/ETR/PA15, CH2/PB3, CH3/PA2,  CH4/PA3)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM2_PARTIAL_1(void)
{
  MODIFY_REG(AFIO->MAPR, AFIO_MAPR_TIM2_REMAP, AFIO_MAPR_TIM2_REMAP_PARTIALREMAP1);
}

/**
  * @brief Disable the remapping of TIM2 alternate function channels 1 to 4 and external trigger (ETR)
  * @rmtoll MAPR          TIM2_REMAP           LL_GPIO_AFIO_REMAP_TIM2_PARTIAL_1
  * @note  DISABLE: No remap        (CH1/ETR/PA0,  CH2/PA1, CH3/PA2,  CH4/PA3)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM2_DISABLE(void)
{
  MODIFY_REG(AFIO->MAPR, AFIO_MAPR_TIM2_REMAP, AFIO_MAPR_TIM2_REMAP_NOREMAP);
}

/**
  * @brief Enable the remapping of TIM3 alternate function channels 1 to 4
  * @rmtoll MAPR          TIM3_REMAP           LL_GPIO_AFIO_REMAP_TIM3_ENABLE
  * @note  ENABLE: Full remap     (CH1/PC6, CH2/PC7, CH3/PC8, CH4/PC9)
  * @note  TIM3_ETR on PE0 is not re-mapped.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM3_ENABLE(void)
{
  MODIFY_REG(AFIO->MAPR, AFIO_MAPR_TIM3_REMAP, AFIO_MAPR_TIM3_REMAP_FULLREMAP);
}

/**
  * @brief Enable the remapping of TIM3 alternate function channels 1 to 4
  * @rmtoll MAPR          TIM3_REMAP           LL_GPIO_AFIO_REMAP_TIM3_PARTIAL
  * @note  PARTIAL: Partial remap (CH1/PB4, CH2/PB5, CH3/PB0, CH4/PB1)
  * @note  TIM3_ETR on PE0 is not re-mapped.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM3_PARTIAL(void)
{
  MODIFY_REG(AFIO->MAPR, AFIO_MAPR_TIM3_REMAP, AFIO_MAPR_TIM3_REMAP_PARTIALREMAP);
}

/**
  * @brief Disable the remapping of TIM3 alternate function channels 1 to 4
  * @rmtoll MAPR          TIM3_REMAP           LL_GPIO_AFIO_REMAP_TIM3_DISABLE
  * @note  DISABLE: No remap      (CH1/PA6, CH2/PA7, CH3/PB0, CH4/PB1)
  * @note  TIM3_ETR on PE0 is not re-mapped.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM3_DISABLE(void)
{
  MODIFY_REG(AFIO->MAPR, AFIO_MAPR_TIM3_REMAP, AFIO_MAPR_TIM3_REMAP_NOREMAP);
}

#if defined(AFIO_MAPR_TIM4_REMAP)
/**
  * @brief Enable the remapping of TIM4 alternate function channels 1 to 4.
  * @rmtoll MAPR          TIM4_REMAP           LL_GPIO_AFIO_REMAP_TIM4_ENABLE
  * @note  ENABLE: Full remap (TIM4_CH1/PD12, TIM4_CH2/PD13, TIM4_CH3/PD14, TIM4_CH4/PD15)
  * @note  TIM4_ETR on PE0 is not re-mapped.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM4_ENABLE(void)
{
  SET_BIT(AFIO->MAPR, AFIO_MAPR_TIM4_REMAP);
}
/**
  * @brief Disable the remapping of TIM4 alternate function channels 1 to 4.
  * @rmtoll MAPR          TIM4_REMAP           LL_GPIO_AFIO_REMAP_TIM4_DISABLE
  * @note  DISABLE: No remap  (TIM4_CH1/PB6,  TIM4_CH2/PB7,  TIM4_CH3/PB8,  TIM4_CH4/PB9)
  * @note  TIM4_ETR on PE0 is not re-mapped.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM4_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR, AFIO_MAPR_TIM4_REMAP);
}
#endif

#if defined(AFIO_MAPR_CAN_REMAP_REMAP1)

/**
  * @brief Enable or disable the remapping of CAN alternate function CAN_RX and CAN_TX in devices with a single CAN interface.
  * @rmtoll MAPR          CAN_REMAP           LL_GPIO_AFIO_REMAP_CAN1_1
  * @note  CASE 1: CAN_RX mapped to PA11, CAN_TX mapped to PA12
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_CAN1_1(void)
{
  MODIFY_REG(AFIO->MAPR, AFIO_MAPR_CAN_REMAP, AFIO_MAPR_CAN_REMAP_REMAP1);
}

/**
  * @brief Enable or disable the remapping of CAN alternate function CAN_RX and CAN_TX in devices with a single CAN interface.
  * @rmtoll MAPR          CAN_REMAP           LL_GPIO_AFIO_REMAP_CAN1_2
  * @note  CASE 2: CAN_RX mapped to PB8,  CAN_TX mapped to PB9 (not available on 36-pin package)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_CAN1_2(void)
{
  MODIFY_REG(AFIO->MAPR, AFIO_MAPR_CAN_REMAP, AFIO_MAPR_CAN_REMAP_REMAP2);
}

/**
  * @brief Enable or disable the remapping of CAN alternate function CAN_RX and CAN_TX in devices with a single CAN interface.
  * @rmtoll MAPR          CAN_REMAP           LL_GPIO_AFIO_REMAP_CAN1_3
  * @note  CASE 3: CAN_RX mapped to PD0,  CAN_TX mapped to PD1
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_CAN1_3(void)
{
  MODIFY_REG(AFIO->MAPR, AFIO_MAPR_CAN_REMAP, AFIO_MAPR_CAN_REMAP_REMAP3);
}
#endif

/**
  * @brief Enable the remapping of PD0 and PD1. When the HSE oscillator is not used
  *        (application running on internal 8 MHz RC) PD0 and PD1 can be mapped on OSC_IN and
  *        OSC_OUT. This is available only on 36, 48 and 64 pins packages (PD0 and PD1 are available
  *        on 100-pin and 144-pin packages, no need for remapping).
  * @rmtoll MAPR          PD01_REMAP           LL_GPIO_AFIO_REMAP_PD01_ENABLE
  * @note  ENABLE: PD0 remapped on OSC_IN, PD1 remapped on OSC_OUT.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_PD01_ENABLE(void)
{
  SET_BIT(AFIO->MAPR, AFIO_MAPR_PD01_REMAP);
}

/**
  * @brief Disable the remapping of PD0 and PD1. When the HSE oscillator is not used
  *        (application running on internal 8 MHz RC) PD0 and PD1 can be mapped on OSC_IN and
  *        OSC_OUT. This is available only on 36, 48 and 64 pins packages (PD0 and PD1 are available
  *        on 100-pin and 144-pin packages, no need for remapping).
  * @rmtoll MAPR          PD01_REMAP           LL_GPIO_AFIO_REMAP_PD01_DISABLE
  * @note  DISABLE: No remapping of PD0 and PD1
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_PD01_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR, AFIO_MAPR_PD01_REMAP);
}

#if defined(AFIO_MAPR_TIM5CH4_IREMAP)
/**
  * @brief Enable the remapping of TIM5CH4.
  * @rmtoll MAPR          TIM5CH4_IREMAP           LL_GPIO_AFIO_REMAP_TIM5CH4_ENABLE
  * @note  ENABLE: LSI internal clock is connected to TIM5_CH4 input for calibration purpose.
  * @note  This function is available only in high density value line devices.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM5CH4_ENABLE(void)
{
  SET_BIT(AFIO->MAPR, AFIO_MAPR_TIM5CH4_IREMAP);
}

/**
  * @brief Disable the remapping of TIM5CH4.
  * @rmtoll MAPR          TIM5CH4_IREMAP           LL_GPIO_AFIO_REMAP_TIM5CH4_DISABLE
  * @note  DISABLE: TIM5_CH4 is connected to PA3
  * @note  This function is available only in high density value line devices.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM5CH4_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR, AFIO_MAPR_TIM5CH4_IREMAP);
}
#endif

#if defined(AFIO_MAPR_ETH_REMAP)
/**
  * @brief Enable the remapping of Ethernet MAC connections with the PHY.
  * @rmtoll MAPR          ETH_REMAP           LL_GPIO_AFIO_REMAP_ETH_ENABLE
  * @note  ENABLE: Remap     (RX_DV-CRS_DV/PD8, RXD0/PD9, RXD1/PD10, RXD2/PD11, RXD3/PD12)
  * @note  This bit is available only in connectivity line devices and is reserved otherwise.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_ETH_ENABLE(void)
{
  SET_BIT(AFIO->MAPR, AFIO_MAPR_ETH_REMAP);
}

/**
  * @brief Disable the remapping of Ethernet MAC connections with the PHY.
  * @rmtoll MAPR          ETH_REMAP           LL_GPIO_AFIO_REMAP_ETH_DISABLE
  * @note  DISABLE: No remap (RX_DV-CRS_DV/PA7, RXD0/PC4, RXD1/PC5,  RXD2/PB0,  RXD3/PB1)
  * @note  This bit is available only in connectivity line devices and is reserved otherwise.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_ETH_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR, AFIO_MAPR_ETH_REMAP);
}
#endif

#if defined(AFIO_MAPR_CAN2_REMAP)

/**
  * @brief Enable the remapping of CAN2 alternate function CAN2_RX and CAN2_TX.
  * @rmtoll MAPR          CAN2_REMAP           LL_GPIO_AFIO_REMAP_CAN2_ENABLE
  * @note  ENABLE: Remap     (CAN2_RX/PB5,  CAN2_TX/PB6)
  * @note  This bit is available only in connectivity line devices and is reserved otherwise.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_CAN2_ENABLE(void)
{
  SET_BIT(AFIO->MAPR, AFIO_MAPR_CAN2_REMAP);
}
/**
  * @brief Disable the remapping of CAN2 alternate function CAN2_RX and CAN2_TX.
  * @rmtoll MAPR          CAN2_REMAP           LL_GPIO_AFIO_REMAP_CAN2_DISABLE
  * @note  DISABLE: No remap (CAN2_RX/PB12, CAN2_TX/PB13)
  * @note  This bit is available only in connectivity line devices and is reserved otherwise.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_CAN2_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR, AFIO_MAPR_CAN2_REMAP);
}
#endif

#if defined(AFIO_MAPR_MII_RMII_SEL)
/**
  * @brief Configures the Ethernet MAC internally for use with an external MII or RMII PHY.
  * @rmtoll MAPR          MII_RMII_SEL           LL_GPIO_AFIO_REMAP_ETH_RMII
  * @note  ETH_RMII: Configure Ethernet MAC for connection with an RMII PHY
  * @note  This bit is available only in connectivity line devices and is reserved otherwise.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_ETH_RMII(void)
{
  SET_BIT(AFIO->MAPR, AFIO_MAPR_MII_RMII_SEL);
}

/**
  * @brief Configures the Ethernet MAC internally for use with an external MII or RMII PHY.
  * @rmtoll MAPR          MII_RMII_SEL           LL_GPIO_AFIO_REMAP_ETH_MII
  * @note  ETH_MII: Configure Ethernet MAC for connection with an MII PHY
  * @note  This bit is available only in connectivity line devices and is reserved otherwise.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_ETH_MII(void)
{
  CLEAR_BIT(AFIO->MAPR, AFIO_MAPR_MII_RMII_SEL);
}
#endif

#if defined(AFIO_MAPR_ADC1_ETRGINJ_REMAP)
/**
  * @brief Enable the remapping of ADC1_ETRGINJ (ADC 1 External trigger injected conversion).
  * @rmtoll MAPR          ADC1_ETRGINJ_REMAP           LL_GPIO_AFIO_REMAP_ADC1_ETRGINJ_ENABLE
  * @note  ENABLE: ADC1 External Event injected conversion is connected to TIM8 Channel4.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_ADC1_ETRGINJ_ENABLE(void)
{
  SET_BIT(AFIO->MAPR, AFIO_MAPR_ADC1_ETRGINJ_REMAP);
}

/**
  * @brief Disable the remapping of ADC1_ETRGINJ (ADC 1 External trigger injected conversion).
  * @rmtoll MAPR          ADC1_ETRGINJ_REMAP           LL_GPIO_AFIO_REMAP_ADC1_ETRGINJ_DISABLE
  * @note  DISABLE: ADC1 External trigger injected conversion is connected to EXTI15
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_ADC1_ETRGINJ_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR, AFIO_MAPR_ADC1_ETRGINJ_REMAP);
}
#endif

#if defined(AFIO_MAPR_ADC1_ETRGREG_REMAP)
/**
  * @brief Enable the remapping of ADC1_ETRGREG (ADC 1 External trigger regular conversion).
  * @rmtoll MAPR          ADC1_ETRGREG_REMAP           LL_GPIO_AFIO_REMAP_ADC1_ETRGREG_ENABLE
  * @note  ENABLE: ADC1 External Event regular conversion is connected to TIM8 TRG0.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_ADC1_ETRGREG_ENABLE(void)
{
  SET_BIT(AFIO->MAPR, AFIO_MAPR_ADC1_ETRGREG_REMAP);
}

/**
  * @brief Disable the remapping of ADC1_ETRGREG (ADC 1 External trigger regular conversion).
  * @rmtoll MAPR          ADC1_ETRGREG_REMAP           LL_GPIO_AFIO_REMAP_ADC1_ETRGREG_DISABLE
  * @note  DISABLE: ADC1 External trigger regular conversion is connected to EXTI11
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_ADC1_ETRGREG_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR, AFIO_MAPR_ADC1_ETRGREG_REMAP);
}
#endif

#if defined(AFIO_MAPR_ADC2_ETRGINJ_REMAP)

/**
  * @brief Enable the remapping of ADC2_ETRGREG (ADC 2 External trigger injected conversion).
  * @rmtoll MAPR          ADC2_ETRGINJ_REMAP           LL_GPIO_AFIO_REMAP_ADC2_ETRGINJ_ENABLE
  * @note  ENABLE: ADC2 External Event injected conversion is connected to TIM8 Channel4.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_ADC2_ETRGINJ_ENABLE(void)
{
  SET_BIT(AFIO->MAPR, AFIO_MAPR_ADC2_ETRGINJ_REMAP);
}

/**
  * @brief Disable the remapping of ADC2_ETRGREG (ADC 2 External trigger injected conversion).
  * @rmtoll MAPR          ADC2_ETRGINJ_REMAP           LL_GPIO_AFIO_REMAP_ADC2_ETRGINJ_DISABLE
  * @note  DISABLE: ADC2 External trigger injected conversion is connected to EXTI15
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_ADC2_ETRGINJ_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR, AFIO_MAPR_ADC2_ETRGINJ_REMAP);
}
#endif

#if defined (AFIO_MAPR_ADC2_ETRGREG_REMAP)

/**
  * @brief Enable the remapping of ADC2_ETRGREG (ADC 2 External trigger regular conversion).
  * @rmtoll MAPR          ADC2_ETRGREG_REMAP           LL_GPIO_AFIO_REMAP_ADC2_ETRGREG_ENABLE
  * @note  ENABLE: ADC2 External Event regular conversion is connected to TIM8 TRG0.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_ADC2_ETRGREG_ENABLE(void)
{
  SET_BIT(AFIO->MAPR, AFIO_MAPR_ADC2_ETRGREG_REMAP);
}

/**
  * @brief Disable the remapping of ADC2_ETRGREG (ADC 2 External trigger regular conversion).
  * @rmtoll MAPR          ADC2_ETRGREG_REMAP           LL_GPIO_AFIO_REMAP_ADC2_ETRGREG_DISABLE
  * @note  DISABLE: ADC2 External trigger regular conversion is connected to EXTI11
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_ADC2_ETRGREG_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR, AFIO_MAPR_ADC2_ETRGREG_REMAP);
}
#endif

/**
  * @brief Enable the Serial wire JTAG configuration
  * @rmtoll MAPR          SWJ_CFG           LL_GPIO_AFIO_REMAP_SWJ_ENABLE
  * @note  ENABLE: Full SWJ (JTAG-DP + SW-DP): Reset State
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_SWJ_ENABLE(void)
{
  MODIFY_REG(AFIO->MAPR, AFIO_MAPR_SWJ_CFG, AFIO_MAPR_SWJ_CFG_RESET);
}

/**
  * @brief Enable the Serial wire JTAG configuration
  * @rmtoll MAPR          SWJ_CFG           LL_GPIO_AFIO_REMAP_SWJ_NONJTRST
  * @note  NONJTRST: Full SWJ (JTAG-DP + SW-DP) but without NJTRST
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_SWJ_NONJTRST(void)
{
  MODIFY_REG(AFIO->MAPR, AFIO_MAPR_SWJ_CFG, AFIO_MAPR_SWJ_CFG_NOJNTRST);
}

/**
  * @brief Enable the Serial wire JTAG configuration
  * @rmtoll MAPR          SWJ_CFG           LL_GPIO_AFIO_REMAP_SWJ_NOJTAG
  * @note  NOJTAG: JTAG-DP Disabled and SW-DP Enabled
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_SWJ_NOJTAG(void)
{
  MODIFY_REG(AFIO->MAPR, AFIO_MAPR_SWJ_CFG, AFIO_MAPR_SWJ_CFG_JTAGDISABLE);
}

/**
  * @brief Disable the Serial wire JTAG configuration
  * @rmtoll MAPR          SWJ_CFG           LL_GPIO_AFIO_REMAP_SWJ_DISABLE
  * @note  DISABLE: JTAG-DP Disabled and SW-DP Disabled
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_SWJ_DISABLE(void)
{
  MODIFY_REG(AFIO->MAPR, AFIO_MAPR_SWJ_CFG, AFIO_MAPR_SWJ_CFG_DISABLE);
}

#if defined(AFIO_MAPR_SPI3_REMAP)

/**
  * @brief Enable the remapping of SPI3 alternate functions SPI3_NSS/I2S3_WS, SPI3_SCK/I2S3_CK, SPI3_MISO, SPI3_MOSI/I2S3_SD.
  * @rmtoll MAPR          SPI3_REMAP           LL_GPIO_AFIO_REMAP_SPI3_ENABLE
  * @note  ENABLE: Remap     (SPI3_NSS-I2S3_WS/PA4,  SPI3_SCK-I2S3_CK/PC10, SPI3_MISO/PC11, SPI3_MOSI-I2S3_SD/PC12)
  * @note  This bit is available only in connectivity line devices and is reserved otherwise.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_SPI3_ENABLE(void)
{
  SET_BIT(AFIO->MAPR, AFIO_MAPR_SPI3_REMAP);
}

/**
  * @brief Disable the remapping of SPI3 alternate functions SPI3_NSS/I2S3_WS, SPI3_SCK/I2S3_CK, SPI3_MISO, SPI3_MOSI/I2S3_SD.
  * @rmtoll MAPR          SPI3_REMAP           LL_GPIO_AFIO_REMAP_SPI3_DISABLE
  * @note  DISABLE: No remap (SPI3_NSS-I2S3_WS/PA15, SPI3_SCK-I2S3_CK/PB3,  SPI3_MISO/PB4,  SPI3_MOSI-I2S3_SD/PB5).
  * @note  This bit is available only in connectivity line devices and is reserved otherwise.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_SPI3_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR, AFIO_MAPR_SPI3_REMAP);
}
#endif

#if defined(AFIO_MAPR_TIM2ITR1_IREMAP)

/**
  * @brief Control of TIM2_ITR1 internal mapping.
  * @rmtoll MAPR          TIM2ITR1_IREMAP           LL_GPIO_AFIO_TIM2ITR1_TO_USB
  * @note  TO_USB: Connect USB OTG SOF (Start of Frame) output to TIM2_ITR1 for calibration purposes.
  * @note  This bit is available only in connectivity line devices and is reserved otherwise.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_TIM2ITR1_TO_USB(void)
{
  SET_BIT(AFIO->MAPR, AFIO_MAPR_TIM2ITR1_IREMAP);
}

/**
  * @brief Control of TIM2_ITR1 internal mapping.
  * @rmtoll MAPR          TIM2ITR1_IREMAP           LL_GPIO_AFIO_TIM2ITR1_TO_ETH
  * @note  TO_ETH: Connect TIM2_ITR1 internally to the Ethernet PTP output for calibration purposes.
  * @note  This bit is available only in connectivity line devices and is reserved otherwise.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_TIM2ITR1_TO_ETH(void)
{
  CLEAR_BIT(AFIO->MAPR, AFIO_MAPR_TIM2ITR1_IREMAP);
}
#endif

#if defined(AFIO_MAPR_PTP_PPS_REMAP)

/**
  * @brief Enable the remapping of ADC2_ETRGREG (ADC 2 External trigger regular conversion).
  * @rmtoll MAPR          PTP_PPS_REMAP           LL_GPIO_AFIO_ETH_PTP_PPS_ENABLE
  * @note  ENABLE: PTP_PPS is output on PB5 pin.
  * @note  This bit is available only in connectivity line devices and is reserved otherwise.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_ETH_PTP_PPS_ENABLE(void)
{
  SET_BIT(AFIO->MAPR, AFIO_MAPR_PTP_PPS_REMAP);
}

/**
  * @brief Disable the remapping of ADC2_ETRGREG (ADC 2 External trigger regular conversion).
  * @rmtoll MAPR          PTP_PPS_REMAP           LL_GPIO_AFIO_ETH_PTP_PPS_DISABLE
  * @note  DISABLE: PTP_PPS not output on PB5 pin.
  * @note  This bit is available only in connectivity line devices and is reserved otherwise.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_ETH_PTP_PPS_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR, AFIO_MAPR_PTP_PPS_REMAP);
}
#endif

#if defined(AFIO_MAPR2_TIM9_REMAP)

/**
  * @brief Enable the remapping of TIM9_CH1 and TIM9_CH2.
  * @rmtoll MAPR          TIM9_REMAP           LL_GPIO_AFIO_REMAP_TIM9_ENABLE
  * @note  ENABLE: Remap     (TIM9_CH1 on PE5 and TIM9_CH2 on PE6).
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM9_ENABLE(void)
{
  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM9_REMAP);
}

/**
  * @brief Disable the remapping of TIM9_CH1 and TIM9_CH2.
  * @rmtoll MAPR          TIM9_REMAP           LL_GPIO_AFIO_REMAP_TIM9_ENABLE
  * @note  DISABLE: No remap (TIM9_CH1 on PA2 and TIM9_CH2 on PA3).
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM9_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM9_REMAP);
}
#endif

#if defined(AFIO_MAPR2_TIM10_REMAP)

/**
  * @brief Enable the remapping of TIM10_CH1.
  * @rmtoll MAPR          TIM10_REMAP           LL_GPIO_AFIO_REMAP_TIM10_ENABLE
  * @note  ENABLE: Remap     (TIM10_CH1 on PF6).
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM10_ENABLE(void)
{
  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM10_REMAP);
}

/**
  * @brief Disable the remapping of TIM10_CH1.
  * @rmtoll MAPR          TIM10_REMAP           LL_GPIO_AFIO_REMAP_TIM10_DISABLE
  * @note  DISABLE: No remap (TIM10_CH1 on PB8).
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM10_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM10_REMAP);
}
#endif

#if defined(AFIO_MAPR2_TIM11_REMAP)
/**
  * @brief Enable the remapping of TIM11_CH1.
  * @rmtoll MAPR          TIM11_REMAP           LL_GPIO_AFIO_REMAP_TIM11_ENABLE
  * @note  ENABLE: Remap     (TIM11_CH1 on PF7).
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM11_ENABLE(void)
{
  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM11_REMAP);
}

/**
  * @brief Disable the remapping of TIM11_CH1.
  * @rmtoll MAPR          TIM11_REMAP           LL_GPIO_AFIO_REMAP_TIM11_DISABLE
  * @note  DISABLE: No remap (TIM11_CH1 on PB9).
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM11_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM11_REMAP);
}
#endif

#if defined(AFIO_MAPR2_TIM13_REMAP)

/**
  * @brief Enable the remapping of TIM13_CH1.
  * @rmtoll MAPR          TIM13_REMAP           LL_GPIO_AFIO_REMAP_TIM13_ENABLE
  * @note  ENABLE: Remap     STM32F100:(TIM13_CH1 on PF8). Others:(TIM13_CH1 on PB0).
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM13_ENABLE(void)
{
  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM13_REMAP);
}

/**
  * @brief Disable the remapping of TIM13_CH1.
  * @rmtoll MAPR          TIM13_REMAP           LL_GPIO_AFIO_REMAP_TIM13_DISABLE
  * @note  DISABLE: No remap STM32F100:(TIM13_CH1 on PA6). Others:(TIM13_CH1 on PC8).
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM13_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM13_REMAP);
}
#endif

#if defined(AFIO_MAPR2_TIM14_REMAP)

/**
  * @brief Enable the remapping of TIM14_CH1.
  * @rmtoll MAPR          TIM14_REMAP           LL_GPIO_AFIO_REMAP_TIM14_ENABLE
  * @note  ENABLE: Remap     STM32F100:(TIM14_CH1 on PB1). Others:(TIM14_CH1 on PF9).
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM14_ENABLE(void)
{
  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM14_REMAP);
}

/**
  * @brief Disable the remapping of TIM14_CH1.
  * @rmtoll MAPR          TIM14_REMAP           LL_GPIO_AFIO_REMAP_TIM14_DISABLE
  * @note  DISABLE: No remap STM32F100:(TIM14_CH1 on PC9). Others:(TIM14_CH1 on PA7).
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM14_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM14_REMAP);
}
#endif

#if defined(AFIO_MAPR2_FSMC_NADV_REMAP)

/**
  * @brief Controls the use of the optional FSMC_NADV signal.
  * @rmtoll MAPR          FSMC_NADV           LL_GPIO_AFIO_FSMCNADV_DISCONNECTED
  * @note  DISCONNECTED: The NADV signal is not connected. The I/O pin can be used by another peripheral.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_FSMCNADV_DISCONNECTED(void)
{
  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_FSMC_NADV_REMAP);
}

/**
  * @brief Controls the use of the optional FSMC_NADV signal.
  * @rmtoll MAPR          FSMC_NADV           LL_GPIO_AFIO_FSMCNADV_CONNECTED
  * @note  CONNECTED: The NADV signal is connected to the output (default).
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_FSMCNADV_CONNECTED(void)
{
  CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_FSMC_NADV_REMAP);
}
#endif

#if defined(AFIO_MAPR2_TIM15_REMAP)

/**
  * @brief Enable the remapping of TIM15_CH1 and TIM15_CH2.
  * @rmtoll MAPR          TIM15_REMAP           LL_GPIO_AFIO_REMAP_TIM15_ENABLE
  * @note  ENABLE: Remap     (TIM15_CH1 on PB14 and TIM15_CH2 on PB15).
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM15_ENABLE(void)
{
  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM15_REMAP);
}
/**
  * @brief Disable the remapping of TIM15_CH1 and TIM15_CH2.
  * @rmtoll MAPR          TIM15_REMAP           LL_GPIO_AFIO_REMAP_TIM15_DISABLE
  * @note  DISABLE: No remap (TIM15_CH1 on PA2  and TIM15_CH2 on PA3).
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM15_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM15_REMAP);
}
#endif

#if defined(AFIO_MAPR2_TIM16_REMAP)

/**
  * @brief Enable the remapping of TIM16_CH1.
  * @rmtoll MAPR          TIM16_REMAP           LL_GPIO_AFIO_REMAP_TIM16_ENABLE
  * @note  ENABLE: Remap     (TIM16_CH1 on PA6).
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM16_ENABLE(void)
{
  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM16_REMAP);
}

/**
  * @brief Disable the remapping of TIM16_CH1.
  * @rmtoll MAPR          TIM16_REMAP           LL_GPIO_AFIO_REMAP_TIM16_DISABLE
  * @note  DISABLE: No remap (TIM16_CH1 on PB8).
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM16_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM16_REMAP);
}
#endif

#if defined(AFIO_MAPR2_TIM17_REMAP)

/**
  * @brief Enable the remapping of TIM17_CH1.
  * @rmtoll MAPR          TIM17_REMAP           LL_GPIO_AFIO_REMAP_TIM17_ENABLE
  * @note  ENABLE: Remap     (TIM17_CH1 on PA7).
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM17_ENABLE(void)
{
  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM17_REMAP);
}

/**
  * @brief Disable the remapping of TIM17_CH1.
  * @rmtoll MAPR          TIM17_REMAP           LL_GPIO_AFIO_REMAP_TIM17_DISABLE
  * @note  DISABLE: No remap (TIM17_CH1 on PB9).
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM17_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM17_REMAP);
}
#endif

#if defined(AFIO_MAPR2_CEC_REMAP)

/**
  * @brief Enable the remapping of CEC.
  * @rmtoll MAPR          CEC_REMAP           LL_GPIO_AFIO_REMAP_CEC_ENABLE
  * @note  ENABLE: Remap     (CEC on PB10).
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_CEC_ENABLE(void)
{
  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_CEC_REMAP);
}

/**
  * @brief Disable the remapping of CEC.
  * @rmtoll MAPR          CEC_REMAP           LL_GPIO_AFIO_REMAP_CEC_DISABLE
  * @note  DISABLE: No remap (CEC on PB8).
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_CEC_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_CEC_REMAP);
}
#endif

#if defined(AFIO_MAPR2_TIM1_DMA_REMAP)

/**
  * @brief Controls the mapping of the TIM1_CH1 TIM1_CH2 DMA requests onto the DMA1 channels.
  * @rmtoll MAPR          TIM1_DMA_REMAP           LL_GPIO_AFIO_REMAP_TIM1DMA_ENABLE
  * @note  ENABLE: Remap (TIM1_CH1 DMA request/DMA1 Channel6, TIM1_CH2 DMA request/DMA1 Channel6)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM1DMA_ENABLE(void)
{
  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM1_DMA_REMAP);
}

/**
  * @brief Controls the mapping of the TIM1_CH1 TIM1_CH2 DMA requests onto the DMA1 channels.
  * @rmtoll MAPR          TIM1_DMA_REMAP           LL_GPIO_AFIO_REMAP_TIM1DMA_DISABLE
  * @note  DISABLE: No remap (TIM1_CH1 DMA request/DMA1 Channel2, TIM1_CH2 DMA request/DMA1 Channel3).
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM1DMA_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM1_DMA_REMAP);
}
#endif

#if defined(AFIO_MAPR2_TIM67_DAC_DMA_REMAP)

/**
  * @brief Controls the mapping of the TIM6_DAC1 and TIM7_DAC2 DMA requests onto the DMA1 channels.
  * @rmtoll MAPR          TIM76_DAC_DMA_REMAP           LL_GPIO_AFIO_REMAP_TIM67DACDMA_ENABLE
  * @note  ENABLE: Remap (TIM6_DAC1 DMA request/DMA1 Channel3, TIM7_DAC2 DMA request/DMA1 Channel4)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM67DACDMA_ENABLE(void)
{
  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM67_DAC_DMA_REMAP);
}

/**
  * @brief Controls the mapping of the TIM6_DAC1 and TIM7_DAC2 DMA requests onto the DMA1 channels.
  * @rmtoll MAPR          TIM76_DAC_DMA_REMAP           LL_GPIO_AFIO_REMAP_TIM67DACDMA_DISABLE
  * @note  DISABLE: No remap (TIM6_DAC1 DMA request/DMA2 Channel3, TIM7_DAC2 DMA request/DMA2 Channel4)
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM67DACDMA_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM67_DAC_DMA_REMAP);
}
#endif

#if defined(AFIO_MAPR2_TIM12_REMAP)

/**
  * @brief Enable the remapping of TIM12_CH1 and TIM12_CH2.
  * @rmtoll MAPR          TIM12_REMAP           LL_GPIO_AFIO_REMAP_TIM12_ENABLE
  * @note  ENABLE: Remap     (TIM12_CH1 on PB12 and TIM12_CH2 on PB13).
  * @note  This bit is available only in high density value line devices.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM12_ENABLE(void)
{
  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM12_REMAP);
}

/**
  * @brief Disable the remapping of TIM12_CH1 and TIM12_CH2.
  * @rmtoll MAPR          TIM12_REMAP           LL_GPIO_AFIO_REMAP_TIM12_DISABLE
  * @note  DISABLE: No remap (TIM12_CH1 on PC4  and TIM12_CH2 on PC5).
  * @note  This bit is available only in high density value line devices.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_TIM12_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM12_REMAP);
}
#endif

#if defined(AFIO_MAPR2_MISC_REMAP)

/**
  * @brief Miscellaneous features remapping.
  *        This bit is set and cleared by software. It controls miscellaneous features.
  *        The DMA2 channel 5 interrupt position in the vector table.
  *        The timer selection for DAC trigger 3 (TSEL[2:0] = 011, for more details refer to the DAC_CR register).
  * @rmtoll MAPR          MISC_REMAP           LL_GPIO_AFIO_REMAP_MISC_ENABLE
  * @note  ENABLE: DMA2 channel 5 interrupt is mapped separately at position 60 and TIM15 TRGO event is
  *        selected as DAC Trigger 3, TIM15 triggers TIM1/3.
  * @note  This bit is available only in high density value line devices.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_MISC_ENABLE(void)
{
  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_MISC_REMAP);
}

/**
  * @brief Miscellaneous features remapping.
  *        This bit is set and cleared by software. It controls miscellaneous features.
  *        The DMA2 channel 5 interrupt position in the vector table.
  *        The timer selection for DAC trigger 3 (TSEL[2:0] = 011, for more details refer to the DAC_CR register).
  * @rmtoll MAPR          MISC_REMAP           LL_GPIO_AFIO_REMAP_MISC_DISABLE
  * @note  DISABLE: DMA2 channel 5 interrupt is mapped with DMA2 channel 4 at position 59, TIM5 TRGO
  *        event is selected as DAC Trigger 3, TIM5 triggers TIM1/3.
  * @note  This bit is available only in high density value line devices.
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_REMAP_MISC_DISABLE(void)
{
  CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_MISC_REMAP);
}
#endif

/**
  * @}
  */

/** @defgroup GPIO_AFIO_LL_EVENTOUT Output Event configuration
  * @brief This section propose definition to Configure EVENTOUT Cortex feature .
  * @{
  */

/**
  * @brief  Configures the port and pin on which the EVENTOUT Cortex signal will be connected.
  * @rmtoll EVCR          PORT           LL_GPIO_AFIO_ConfigEventout\n
  *         EVCR          PIN            LL_GPIO_AFIO_ConfigEventout
  * @param  LL_GPIO_PortSource This parameter can be one of the following values:
  *      @arg @ref LL_GPIO_AFIO_EVENTOUT_PORT_A
  *      @arg @ref LL_GPIO_AFIO_EVENTOUT_PORT_B
  *      @arg @ref LL_GPIO_AFIO_EVENTOUT_PORT_C
  *      @arg @ref LL_GPIO_AFIO_EVENTOUT_PORT_D
  *      @arg @ref LL_GPIO_AFIO_EVENTOUT_PORT_E
  * @param  LL_GPIO_PinSource This parameter can be one of the following values:
  *      @arg @ref LL_GPIO_AFIO_EVENTOUT_PIN_0
  *      @arg @ref LL_GPIO_AFIO_EVENTOUT_PIN_1
  *      @arg @ref LL_GPIO_AFIO_EVENTOUT_PIN_2
  *      @arg @ref LL_GPIO_AFIO_EVENTOUT_PIN_3
  *      @arg @ref LL_GPIO_AFIO_EVENTOUT_PIN_4
  *      @arg @ref LL_GPIO_AFIO_EVENTOUT_PIN_5
  *      @arg @ref LL_GPIO_AFIO_EVENTOUT_PIN_6
  *      @arg @ref LL_GPIO_AFIO_EVENTOUT_PIN_7
  *      @arg @ref LL_GPIO_AFIO_EVENTOUT_PIN_8
  *      @arg @ref LL_GPIO_AFIO_EVENTOUT_PIN_9
  *      @arg @ref LL_GPIO_AFIO_EVENTOUT_PIN_10
  *      @arg @ref LL_GPIO_AFIO_EVENTOUT_PIN_11
  *      @arg @ref LL_GPIO_AFIO_EVENTOUT_PIN_12
  *      @arg @ref LL_GPIO_AFIO_EVENTOUT_PIN_13
  *      @arg @ref LL_GPIO_AFIO_EVENTOUT_PIN_14
  *      @arg @ref LL_GPIO_AFIO_EVENTOUT_PIN_15
  * @retval None
*/
__STATIC_INLINE void LL_GPIO_AFIO_ConfigEventout(uint32_t LL_GPIO_PortSource, uint32_t LL_GPIO_PinSource)
{
  MODIFY_REG(AFIO->EVCR, (AFIO_EVCR_PORT) | (AFIO_EVCR_PIN), (LL_GPIO_PortSource) | (LL_GPIO_PinSource));
}

/**
  * @brief  Enables the Event Output.
  * @rmtoll EVCR          EVOE           LL_GPIO_AFIO_EnableEventout
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_EnableEventout(void)
{
  SET_BIT(AFIO->EVCR, AFIO_EVCR_EVOE);
}

/**
  * @brief  Disables the Event Output.
  * @rmtoll EVCR          EVOE           LL_GPIO_AFIO_DisableEventout
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_DisableEventout(void)
{
  CLEAR_BIT(AFIO->EVCR, AFIO_EVCR_EVOE);
}

/**
  * @}
  */
/** @defgroup GPIO_AFIO_LL_EXTI EXTI external interrupt
  * @brief This section Configure source input for the EXTI external interrupt .
  * @{
  */

/**
  * @brief  Configure source input for the EXTI external interrupt.
  * @rmtoll AFIO_EXTICR1      EXTIx         LL_GPIO_AFIO_SetEXTISource\n
  *         AFIO_EXTICR2      EXTIx         LL_GPIO_AFIO_SetEXTISource\n
  *         AFIO_EXTICR3      EXTIx         LL_GPIO_AFIO_SetEXTISource\n
  *         AFIO_EXTICR4      EXTIx         LL_GPIO_AFIO_SetEXTISource
  * @param  Port This parameter can be one of the following values:
  *         @arg @ref LL_GPIO_AFIO_EXTI_PORTA
  *         @arg @ref LL_GPIO_AFIO_EXTI_PORTB
  *         @arg @ref LL_GPIO_AFIO_EXTI_PORTC
  *         @arg @ref LL_GPIO_AFIO_EXTI_PORTD
  *         @arg @ref LL_GPIO_AFIO_EXTI_PORTE
  *         @arg @ref LL_GPIO_AFIO_EXTI_PORTF
  *         @arg @ref LL_GPIO_AFIO_EXTI_PORTG
  * @param  Line This parameter can be one of the following values:
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE0
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE1
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE2
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE3
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE4
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE5
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE6
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE7
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE8
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE9
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE10
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE11
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE12
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE13
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE14
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE15
  * @retval None
  */
__STATIC_INLINE void LL_GPIO_AFIO_SetEXTISource(uint32_t Port, uint32_t Line)
{
  MODIFY_REG(AFIO->EXTICR[Line & 0xFF], (Line >> 16), Port << POSITION_VAL((Line >> 16)));
}

/**
  * @brief  Get the configured defined for specific EXTI Line
  * @rmtoll AFIO_EXTICR1      EXTIx         LL_GPIO_AFIO_GetEXTISource\n
  *         AFIO_EXTICR2      EXTIx         LL_GPIO_AFIO_GetEXTISource\n
  *         AFIO_EXTICR3      EXTIx         LL_GPIO_AFIO_GetEXTISource\n
  *         AFIO_EXTICR4      EXTIx         LL_GPIO_AFIO_GetEXTISource
  * @param  Line This parameter can be one of the following values:
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE0
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE1
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE2
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE3
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE4
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE5
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE6
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE7
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE8
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE9
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE10
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE11
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE12
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE13
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE14
  *         @arg @ref LL_GPIO_AFIO_EXTI_LINE15
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_GPIO_AFIO_EXTI_PORTA
  *         @arg @ref LL_GPIO_AFIO_EXTI_PORTB
  *         @arg @ref LL_GPIO_AFIO_EXTI_PORTC
  *         @arg @ref LL_GPIO_AFIO_EXTI_PORTD
  *         @arg @ref LL_GPIO_AFIO_EXTI_PORTE
  *         @arg @ref LL_GPIO_AFIO_EXTI_PORTF
  *         @arg @ref LL_GPIO_AFIO_EXTI_PORTG
  */
__STATIC_INLINE uint32_t LL_GPIO_AFIO_GetEXTISource(uint32_t Line)
{
  return (uint32_t)(READ_BIT(AFIO->EXTICR[Line & 0xFF], (Line >> 16)) >> POSITION_VAL(Line >> 16));
}

/**
  * @}
  */

#if defined(USE_FULL_LL_DRIVER)
/** @defgroup GPIO_LL_EF_Init Initialization and de-initialization functions
  * @{
  */

ErrorStatus LL_GPIO_DeInit(GPIO_TypeDef *GPIOx);
ErrorStatus LL_GPIO_Init(GPIO_TypeDef *GPIOx, LL_GPIO_InitTypeDef *GPIO_InitStruct);
void        LL_GPIO_StructInit(LL_GPIO_InitTypeDef *GPIO_InitStruct);

/**
  * @}
  */
#endif /* USE_FULL_LL_DRIVER */

/**
  * @}
  */

/**
  * @}
  */

#endif /* defined (GPIOA) || defined (GPIOB) || defined (GPIOC) || defined (GPIOD) || defined (GPIOE) || defined (GPIOF) || defined (GPIOG) */
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_LL_GPIO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
