/**
  ******************************************************************************
  * @file    stm32f0xx_hal.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    28-May-2014
  * @brief   This file contains all the functions prototypes for the HAL 
  *          module driver.
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

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** @defgroup HAL_DMA_Remapping
  *        Elements values convention: 0xYYYYYYYY
  *           - YYYYYYYY  : Position in the SYSCFG register CFGR1
  * @{
  */
#define HAL_REMAPDMA_ADC_DMA_CH2         ((uint32_t)SYSCFG_CFGR1_ADC_DMA_RMP) /*!< ADC DMA remap 
                                                                         0: No remap (ADC DMA requests mapped on DMA channel 1
                                                                         1: Remap (ADC DMA requests mapped on DMA channel 2 */
#define HAL_REMAPDMA_USART1_TX_DMA_CH4   ((uint32_t)SYSCFG_CFGR1_USART1TX_DMA_RMP) /*!< USART1 TX DMA remap 
                                                                         0: No remap (USART1_TX DMA request mapped on DMA channel 2
                                                                         1: Remap (USART1_TX DMA request mapped on DMA channel 4 */
#define HAL_REMAPDMA_USART1_RX_DMA_CH5   ((uint32_t)SYSCFG_CFGR1_USART1RX_DMA_RMP) /*!< USART1 RX DMA remap 
                                                                         0: No remap (USART1_RX DMA request mapped on DMA channel 3
                                                                         1: Remap (USART1_RX DMA request mapped on DMA channel 5 */
#define HAL_REMAPDMA_TIM16_DMA_CH4       ((uint32_t)SYSCFG_CFGR1_TIM16_DMA_RMP) /*!< TIM16 DMA request remap
                                                                         0: No remap (TIM16_CH1 and TIM16_UP DMA requests mapped on DMA channel 3)
                                                                         1: Remap (TIM16_CH1 and TIM16_UP DMA requests mapped on DMA channel 4) */
#define HAL_REMAPDMA_TIM17_DMA_CH2       ((uint32_t)SYSCFG_CFGR1_TIM17_DMA_RMP) /*!< TIM17 DMA request remap
                                                                         0: No remap (TIM17_CH1 and TIM17_UP DMA requests mapped on DMA channel 1
                                                                         1: Remap (TIM17_CH1 and TIM17_UP DMA requests mapped on DMA channel 2) */

#if defined (STM32F071xB) || defined (STM32F072xB) || defined (STM32F078xx)
#define HAL_REMAPDMA_TIM16_DMA_CH6       ((uint32_t)SYSCFG_CFGR1_TIM16_DMA_RMP2) /*!< TIM16 alternate DMA request remapping bit. Available on STM32F07x devices only
                                                                         0: No alternate remap (TIM16 DMA requestsmapped according to TIM16_DMA_RMP bit)
                                                                         1: Alternate remap (TIM16_CH1 and TIM16_UP DMA requests mapped on DMA channel 6) */
#define HAL_REMAPDMA_TIM17_DMA_CH7       ((uint32_t)SYSCFG_CFGR1_TIM17_DMA_RMP2) /*!< TIM17 alternate DMA request remapping bit. Available on STM32F07x devices only
                                                                         0: No alternate remap (TIM17 DMA requestsmapped according to TIM17_DMA_RMP bit)
                                                                         1: Alternate remap (TIM17_CH1 and TIM17_UP DMA requests mapped on DMA channel 7) */
#define HAL_REMAPDMA_SPI2_DMA_CH67       ((uint32_t)SYSCFG_CFGR1_SPI2_DMA_RMP) /*!< SPI2 DMA request remapping bit. Available on STM32F07x devices only.
                                                                         0: No remap (SPI2_RX and SPI2_TX DMA requests mapped on DMA channel 4 and 5 respectively)
                                                                         1: 1: Remap (SPI2_RX and SPI2_TX DMA requests mapped on DMA channel 6 and 7 respectively) */
#define HAL_REMAPDMA_USART2_DMA_CH67     ((uint32_t)SYSCFG_CFGR1_USART2_DMA_RMP) /*!< USART2 DMA request remapping bit. Available on STM32F07x devices only.
                                                                         0: No remap (USART2_RX and USART2_TX DMA requests mapped on DMA channel 5 and 4 respectively)
                                                                         1: 1: Remap (USART2_RX and USART2_TX DMA requests mapped on DMA channel 6 and 7 respectively) */
#define HAL_REMAPDMA_USART3_DMA_CH32     ((uint32_t)SYSCFG_CFGR1_USART3_DMA_RMP) /*!< USART3 DMA request remapping bit. Available on STM32F07x devices only.
                                                                         0: No remap (USART3_RX and USART3_TX DMA requests mapped on DMA channel 6 and 7 respectively)
                                                                         1: 1: Remap (USART3_RX and USART3_TX DMA requests mapped on DMA channel 3 and 2 respectively) */
#define HAL_REMAPDMA_I2C1_DMA_CH76       ((uint32_t)SYSCFG_CFGR1_I2C1_DMA_RMP) /*!< I2C1 DMA request remapping bit. Available on STM32F07x devices only.
                                                                         0: No remap (I2C1_RX and I2C1_TX DMA requests mapped on DMA channel 3 and 2 respectively)
                                                                         1: Remap (I2C1_RX and I2C1_TX DMA requests mapped on DMA channel 7 and 6 respectively) */
#define HAL_REMAPDMA_TIM1_DMA_CH6        ((uint32_t)SYSCFG_CFGR1_TIM1_DMA_RMP) /*!< TIM1 DMA request remapping bit. Available on STM32F07x devices only.
                                                                         0: No remap (TIM1_CH1, TIM1_CH2 and TIM1_CH3 DMA requests mapped on DMA channel 2, 3 and 4 respectively)
                                                                         1: Remap (TIM1_CH1, TIM1_CH2 and TIM1_CH3 DMA requests mapped on DMA channel 6 */
#define HAL_REMAPDMA_TIM2_DMA_CH7        ((uint32_t)SYSCFG_CFGR1_TIM2_DMA_RMP) /*!< TIM2 DMA request remapping bit. Available on STM32F07x devices only.
                                                                         0: No remap (TIM2_CH2 and TIM2_CH4 DMA requests mapped on DMA channel 3 and 4 respectively)
                                                                         1: Remap (TIM2_CH2 and TIM2_CH4 DMA requests mapped on DMA channel 7 */
#define HAL_REMAPDMA_TIM3_DMA_CH6        ((uint32_t)SYSCFG_CFGR1_TIM3_DMA_RMP) /*!< TIM3 DMA request remapping bit. Available on STM32F07x devices only.
                                                                         0: No remap (TIM3_CH1 and TIM3_TRIG DMA requests mapped on DMA channel 4)
                                                                         1: Remap (TIM3_CH1 and TIM3_TRIG DMA requests mapped on DMA channel 6) */
#endif

#if defined (STM32F071xB) || defined (STM32F072xB) || defined (STM32F078xx)
#define IS_HAL_REMAPDMA(RMP) (((RMP) == HAL_REMAPDMA_ADC_DMA_CH2)        || \
                              ((RMP) == HAL_REMAPDMA_USART1_TX_DMA_CH4) || \
                              ((RMP) == HAL_REMAPDMA_USART1_RX_DMA_CH5) || \
                              ((RMP) == HAL_REMAPDMA_TIM16_DMA_CH4)     || \
                              ((RMP) == HAL_REMAPDMA_TIM17_DMA_CH2)     || \
                              ((RMP) == HAL_REMAPDMA_TIM16_DMA_CH6)     || \
                              ((RMP) == HAL_REMAPDMA_TIM17_DMA_CH7)     || \
                              ((RMP) == HAL_REMAPDMA_SPI2_DMA_CH67)     || \
                              ((RMP) == HAL_REMAPDMA_USART2_DMA_CH67)   || \
                              ((RMP) == HAL_REMAPDMA_USART3_DMA_CH32)   || \
                              ((RMP) == HAL_REMAPDMA_I2C1_DMA_CH76)     || \
                              ((RMP) == HAL_REMAPDMA_TIM1_DMA_CH6)      || \
                              ((RMP) == HAL_REMAPDMA_TIM2_DMA_CH7)      || \
                              ((RMP) == HAL_REMAPDMA_TIM3_DMA_CH6))
#else
#define IS_HAL_REMAPDMA(RMP) (((RMP) == HAL_REMAPDMA_ADC_DMA_CH2)       || \
                              ((RMP) == HAL_REMAPDMA_USART1_TX_DMA_CH4) || \
                              ((RMP) == HAL_REMAPDMA_USART1_RX_DMA_CH5) || \
                              ((RMP) == HAL_REMAPDMA_TIM16_DMA_CH4)     || \
                              ((RMP) == HAL_REMAPDMA_TIM17_DMA_CH2))
#endif
/**
  * @}
  */

#if defined(STM32F042x6) || defined(STM32F048xx)
/** @defgroup HAL_Pin_Remapping
  * @{
  */
#define HAL_REMAP_PA11_PA12                 (SYSCFG_CFGR1_PA11_PA12_RMP)  /*!< PA11 and PA12 remapping bit for small packages (28 and 20 pins).
                                                                           0: No remap (pin pair PA9/10 mapped on the pins)
                                                                           1: Remap (pin pair PA11/12 mapped instead of PA9/10) */

#define IS_HAL_REMAP_PIN(RMP) ((RMP) == HAL_REMAP_PA11_PA12)
/**
  * @}
  */
#endif /* STM32F042x6 || STM32F048xx */

/** @defgroup HAL_FastModePlus_I2C
  * @{
  */
#define HAL_SYSCFG_FASTMODEPLUS_I2C_PB6     (SYSCFG_CFGR1_I2C_FMP_PB6)  /*!< Fast Mode Plus (FM+) driving capability activation on the pad
                                                                         0: PB6 pin operates in standard mode
                                                                         1: I2C FM+ mode enabled on PB6 pin, and the Speed control is bypassed */
#define HAL_SYSCFG_FASTMODEPLUS_I2C_PB7     (SYSCFG_CFGR1_I2C_FMP_PB7)  /*!< Fast Mode Plus (FM+) driving capability activation on the pad
                                                                         0: PB7 pin operates in standard mode
                                                                         1: I2C FM+ mode enabled on PB7 pin, and the Speed control is bypassed */
#define HAL_SYSCFG_FASTMODEPLUS_I2C_PB8     (SYSCFG_CFGR1_I2C_FMP_PB8)  /*!< Fast Mode Plus (FM+) driving capability activation on the pad
                                                                         0: PB8 pin operates in standard mode
                                                                         1: I2C FM+ mode enabled on PB8 pin, and the Speed control is bypassed */
#define HAL_SYSCFG_FASTMODEPLUS_I2C_PB9     (SYSCFG_CFGR1_I2C_FMP_PB9)  /*!< Fast Mode Plus (FM+) driving capability activation on the pad
                                                                         0: PB9 pin operates in standard mode
                                                                         1: I2C FM+ mode enabled on PB9 pin, and the Speed control is bypassed */
                                                                         
#if defined(STM32F030x6) || defined(STM32F031x6) || defined(STM32F038xx) || \
    defined(STM32F042x6) || defined(STM32F048xx) || \
    defined(STM32F071xB) || defined(STM32F072xB) || defined(STM32F078xx)
#define HAL_SYSCFG_FASTMODEPLUS_I2C1        (SYSCFG_CFGR1_I2C_FMP_I2C1)  /*!< I2C1 fast mode Plus driving capability activation
                                                                         0: FM+ mode is not enabled on I2C1 pins selected through AF selection bits
                                                                         1: FM+ mode is enabled on I2C1 pins selected through AF selection bits */
#endif

#if defined(STM32F071xB) || defined(STM32F072xB) || defined(STM32F078xx)
#define HAL_SYSCFG_FASTMODEPLUS_I2C2        (SYSCFG_CFGR1_I2C_FMP_I2C2)  /*!< I2C2 fast mode Plus driving capability activation
                                                                         0: FM+ mode is not enabled on I2C2 pins selected through AF selection bits
                                                                         1: FM+ mode is enabled on I2C2 pins selected through AF selection bits */
#endif

#if defined(STM32F030x6) || defined(STM32F031x6) || defined(STM32F038xx)
#define HAL_SYSCFG_FASTMODEPLUS_I2C2_PA9    (SYSCFG_CFGR1_I2C_FMP_PA9)  /*!< Fast Mode Plus (FM+) driving capability activation on the pad
                                                                         0: PA9 pin operates in standard mode
                                                                         1: FM+ mode is enabled on PA9 pin, and the Speed control is bypassed */
#define HAL_SYSCFG_FASTMODEPLUS_I2C2_PA10   (SYSCFG_CFGR1_I2C_FMP_PA10)  /*!< Fast Mode Plus (FM+) driving capability activation on the pad
                                                                         0: PA10 pin operates in standard mode
                                                                         1: FM+ mode is enabled on PA10 pin, and the Speed control is bypassed */
#endif

/**
  * @}
  */


/* Exported macros -----------------------------------------------------------*/

/** @brief  Freeze/Unfreeze Peripherals in Debug mode 
  */
#define __HAL_FREEZE_TIM3_DBGMCU()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM3_STOP))
#define __HAL_UNFREEZE_TIM3_DBGMCU()         (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM3_STOP))

#define __HAL_FREEZE_TIM14_DBGMCU()          (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM14_STOP))
#define __HAL_UNFREEZE_TIM14_DBGMCU()        (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM14_STOP))

#define __HAL_FREEZE_RTC_DBGMCU()            (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_RTC_STOP))
#define __HAL_UNFREEZE_RTC_DBGMCU()          (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_RTC_STOP))

#define __HAL_FREEZE_WWDG_DBGMCU()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_WWDG_STOP))
#define __HAL_UNFREEZE_WWDG_DBGMCU()         (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_WWDG_STOP))

#define __HAL_FREEZE_IWDG_DBGMCU()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_IWDG_STOP))
#define __HAL_UNFREEZE_IWDG_DBGMCU()         (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_IWDG_STOP))

#define __HAL_FREEZE_I2C1_TIMEOUT_DBGMCU()   (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT))
#define __HAL_UNFREEZE_I2C1_TIMEOUT_DBGMCU() (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT))

#if defined(STM32F031x6) || defined(STM32F042x6) || defined(STM32F048xx) || defined(STM32F051x8) || \
    defined(STM32F071xB) || defined(STM32F072xB) || \
    defined(STM32F038xx) || defined(STM32F058xx) || defined(STM32F078xx)
#define __HAL_FREEZE_TIM2_DBGMCU()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM2_STOP))
#define __HAL_UNFREEZE_TIM2_DBGMCU()         (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM2_STOP))
#endif /* STM32F031x6 || STM32F042x6 || STM32F048xx || STM32F051x8 || */
       /* STM32F071xB || STM32F072xB || */
       /* STM32F038xx || STM32F058xx || STM32F078xx */

#if defined(STM32F030x8) || defined(STM32F051x8) || \
    defined(STM32F071xB) || defined(STM32F072xB) || \
    defined(STM32F058xx) || defined(STM32F078xx)
#define __HAL_FREEZE_TIM6_DBGMCU()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM6_STOP))
#define __HAL_UNFREEZE_TIM6_DBGMCU()         (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM6_STOP))
#endif /* STM32F030x8 || STM32F051x8 || */
       /* STM32F071xB || STM32F072xB || */
       /* STM32F058xx || STM32F078xx */

#if defined(STM32F071xB) || defined(STM32F072xB) || defined(STM32F078xx)
#define __HAL_FREEZE_TIM7_DBGMCU()           (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_TIM7_STOP))
#define __HAL_UNFREEZE_TIM7_DBGMCU()         (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_TIM7_STOP))
#endif /* STM32F071xB || STM32F072xB || STM32F078xx */

#if defined(STM32F042x6) || defined(STM32F048xx) || defined(STM32F072xB)
#define __HAL_FREEZE_CAN_DBGMCU()            (DBGMCU->APB1FZ |= (DBGMCU_APB1_FZ_DBG_CAN1_STOP))
#define __HAL_UNFREEZE_CAN_DBGMCU()          (DBGMCU->APB1FZ &= ~(DBGMCU_APB1_FZ_DBG_CAN1_STOP))
#endif /* STM32F042x6 || STM32F072xB */


#define __HAL_FREEZE_TIM1_DBGMCU()           (DBGMCU->APB2FZ |= (DBGMCU_APB2_FZ_DBG_TIM1_STOP))
#define __HAL_UNFREEZE_TIM1_DBGMCU()         (DBGMCU->APB2FZ &= ~(DBGMCU_APB2_FZ_DBG_TIM1_STOP))

#if defined(STM32F030x8) || defined(STM32F051x8) || \
    defined(STM32F071xB) || defined(STM32F072xB) || \
    defined(STM32F058xx) || defined(STM32F078xx)
#define __HAL_FREEZE_TIM15_DBGMCU()          (DBGMCU->APB2FZ |= (DBGMCU_APB2_FZ_DBG_TIM15_STOP))
#define __HAL_UNFREEZE_TIM15_DBGMCU()        (DBGMCU->APB2FZ &= ~(DBGMCU_APB2_FZ_DBG_TIM15_STOP))
#endif /* STM32F030x8 || STM32F051x8 || */
       /* STM32F071xB || STM32F072xB || */
       /* STM32F058xx || STM32F078xx */

#define __HAL_FREEZE_TIM16_DBGMCU()          (DBGMCU->APB2FZ |= (DBGMCU_APB2_FZ_DBG_TIM16_STOP))
#define __HAL_UNFREEZE_TIM16_DBGMCU()        (DBGMCU->APB2FZ &= ~(DBGMCU_APB2_FZ_DBG_TIM16_STOP))

#define __HAL_FREEZE_TIM17_DBGMCU()          (DBGMCU->APB2FZ |= (DBGMCU_APB2_FZ_DBG_TIM17_STOP))
#define __HAL_UNFREEZE_TIM17_DBGMCU()        (DBGMCU->APB2FZ &= ~(DBGMCU_APB2_FZ_DBG_TIM17_STOP))

/** @brief  Main Flash memory mapped at 0x00000000
  */
#define __HAL_REMAPMEMORY_FLASH()        (SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_MEM_MODE))

/** @brief  System Flash memory mapped at 0x00000000
  */
#define __HAL_REMAPMEMORY_SYSTEMFLASH()  do {SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_MEM_MODE); \
                                             SYSCFG->CFGR1 |= SYSCFG_CFGR1_MEM_MODE_0;  \
                                            }while(0)

/** @brief  Embedded SRAM mapped at 0x00000000
  */
#define __HAL_REMAPMEMORY_SRAM()         do {SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_MEM_MODE); \
                                             SYSCFG->CFGR1 |= (SYSCFG_CFGR1_MEM_MODE_0 | SYSCFG_CFGR1_MEM_MODE_1); \
                                            }while(0)

/** @brief  DMA remapping enable/disable macros
  * @param __DMA_REMAP__: This parameter can be a value of @ref HAL_DMA_Remapping
  */
#define __HAL_REMAPDMA_CHANNEL_ENABLE(__DMA_REMAP__)   do {assert_param(IS_HAL_REMAPDMA((__DMA_REMAP__)));                  \
                                                           SYSCFG->CFGR1 |= (__DMA_REMAP__);                                \
                                                         }while(0)
#define __HAL_REMAPDMA_CHANNEL_DISABLE(__DMA_REMAP__)  do {assert_param(IS_HAL_REMAPDMA((__DMA_REMAP__)));                  \
                                                           SYSCFG->CFGR1 &= ~(__DMA_REMAP__);                               \
                                                         }while(0)

#if defined(STM32F042x6) || defined(STM32F048xx)
/** @brief  Pin remapping enable/disable macros
  * @param __PIN_REMAP__: This parameter can be a value of @ref HAL_Pin_Remapping
  */
#define __HAL_REMAP_PIN_ENABLE(__PIN_REMAP__)          do {assert_param(IS_HAL_REMAP_PIN((__PIN_REMAP__)));                 \
                                                           SYSCFG->CFGR1 |= (__PIN_REMAP__);                                \
                                                         }while(0)
#define __HAL_REMAP_PIN_DISABLE(__PIN_REMAP__)         do {assert_param(IS_HAL_REMAP_PIN((__DMA_REMAP__)));                 \
                                                           SYSCFG->CFGR1 &= ~(__PIN_REMAP__);                               \
                                                         }while(0)
#endif /* STM32F042x6 || STM32F048xx */

/** @brief  Fast mode Plus driving capability enable/disable macros
  * @param __FASTMODEPLUS__: This parameter can be a value of @ref HAL_FastModePlus_I2C
  */
#define __HAL_SYSCFG_FASTMODEPLUS_ENABLE(__FASTMODEPLUS__)  do {assert_param(IS_HAL_SYSCFG_FASTMODEPLUS_CONFIG((__FASTMODEPLUS__))); \
                                                                SYSCFG->CFGR1 |= (__FASTMODEPLUS__);                                 \
                                                               }while(0)

#define __HAL_SYSCFG_FASTMODEPLUS_DISABLE(__FASTMODEPLUS__) do {assert_param(IS_HAL_SYSCFG_FASTMODEPLUS_CONFIG((__FASTMODEPLUS__))); \
                                                                SYSCFG->CFGR1 &= ~(__FASTMODEPLUS__);                                \
                                                               }while(0)

/** @brief  SYSCFG Break Lockup lock
  *         Enables and locks the connection of Cortex-M0 LOCKUP (Hardfault) output to TIM1/15/16/17 Break input
  * @note   The selected configuration is locked and can be unlocked by system reset
  */
#define __HAL_SYSCFG_BREAK_LOCKUP_LOCK()   do {SYSCFG->CFGR2 &= ~(SYSCFG_CFGR2_LOCKUP_LOCK); \
                                               SYSCFG->CFGR2 |= SYSCFG_CFGR2_LOCKUP_LOCK;    \
                                              }while(0)

#if defined(STM32F031x6) || defined(STM32F042x6) || defined(STM32F048xx) || defined(STM32F051x8) || \
    defined(STM32F071xB) || defined(STM32F072xB)
/** @brief  SYSCFG Break PVD lock
  *         Enables and locks the PVD connection with Timer1/8/15/16/17 Break Input, , as well as the PVDE and PLS[2:0] in the PWR_CR register
  * @note   The selected configuration is locked and can be unlocked by system reset
  */
#define __HAL_SYSCFG_BREAK_PVD_LOCK()      do {SYSCFG->CFGR2 &= ~(SYSCFG_CFGR2_PVD_LOCK); \
                                               SYSCFG->CFGR2 |= SYSCFG_CFGR2_PVD_LOCK;    \
                                              }while(0)
#endif /* STM32F031x6 || STM32F042x6 || STM32F048xx || STM32F051x8) || */
       /* STM32F071xB || STM32F072xB */

/** @brief  SYSCFG Break SRAM PARITY lock
  *         Enables and locks the SRAM_PARITY error signal with Break Input of TIMER1/8/15/16/17
  * @note   The selected configuration is locked and can be unlocked by system reset
  */
#define __HAL_SYSCFG_BREAK_SRAMPARITY_LOCK() do {SYSCFG->CFGR2 &= ~(SYSCFG_CFGR2_SRAM_PARITY_LOCK); \
                                                 SYSCFG->CFGR2 |= SYSCFG_CFGR2_SRAM_PARITY_LOCK;    \
                                                }while(0)

/**
  * @brief  Parity check on RAM disable macro
  * @note   Disabling the parity check on RAM locks the configuration bit.
  *         To re-enable the parity check on RAM perform a system reset.
  */
#define __HAL_SYSCFG_RAM_PARITYCHECK_DISABLE()   (SYSCFG->CFGR2 |= SYSCFG_CFGR2_SRAM_PEF)


/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions  ****************************/
HAL_StatusTypeDef HAL_Init(void);
HAL_StatusTypeDef HAL_DeInit(void);
void              HAL_MspInit(void);
void              HAL_MspDeInit(void);
HAL_StatusTypeDef HAL_InitTick (uint32_t TickPriority);

/* Peripheral Control functions  **********************************************/
void              HAL_IncTick(void);
void              HAL_Delay(__IO uint32_t Delay);
uint32_t          HAL_GetTick(void);
void              HAL_SuspendTick(void);
void              HAL_ResumeTick(void);
uint32_t          HAL_GetHalVersion(void);
uint32_t          HAL_GetREVID(void);
uint32_t          HAL_GetDEVID(void);

void              HAL_EnableDBGStopMode(void);
void              HAL_DisableDBGStopMode(void);
void              HAL_EnableDBGStandbyMode(void);
void              HAL_DisableDBGStandbyMode(void);

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
