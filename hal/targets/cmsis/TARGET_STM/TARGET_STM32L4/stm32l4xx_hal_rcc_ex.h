/**
  ******************************************************************************
  * @file    stm32l4xx_hal_rcc_ex.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    26-June-2015
  * @brief   Header file of RCC HAL Extended module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
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
#ifndef __STM32L4xx_HAL_RCC_EX_H
#define __STM32L4xx_HAL_RCC_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal_def.h"

/** @addtogroup STM32L4xx_HAL_Driver
  * @{
  */

/** @addtogroup RCCEx
  * @{
  */

/* Exported types ------------------------------------------------------------*/ 

/** @defgroup RCCEx_Exported_Types RCCEx Exported Types
  * @{
  */

/**
  * @brief  PLLSAI1 Clock structure definition
  */
typedef struct
{

  uint32_t PLLSAI1N;         /*!< PLLSAI1N: specifies the multiplication factor for PLLSAI1 VCO output clock.
                                  This parameter must be a number between 8 and 86. */

  uint32_t PLLSAI1P;         /*!< PLLSAI1P: specifies the division factor for SAI clock.
                                  This parameter must be a value of @ref RCC_PLLP_Clock_Divider */

  uint32_t PLLSAI1Q;         /*!< PLLSAI1Q: specifies the division factor for USB/RNG/SDMMC1 clock.
                                  This parameter must be a value of @ref RCC_PLLQ_Clock_Divider */

  uint32_t PLLSAI1R;         /*!< PLLSAI1R: specifies the division factor for ADC clock.
                                  This parameter must be a value of @ref RCC_PLLR_Clock_Divider */

  uint32_t PLLSAI1ClockOut;  /*!< PLLSAIClockOut: specifies PLLSAI1 output clock to be enabled.
                                  This parameter must be a value of @ref RCC_PLLSAI1_Clock_Output */
}RCC_PLLSAI1InitTypeDef;

/**
  * @brief  PLLSAI2 Clock structure definition
  */
typedef struct
{

   uint32_t PLLSAI2N;        /*!< PLLSAI2N: specifies the multiplication factor for PLLSAI2 VCO output clock.
                                  This parameter must be a number between 8 and 86. */

  uint32_t PLLSAI2P;         /*!< PLLSAI2P: specifies the division factor for SAI clock.
                                  This parameter must be a value of @ref RCC_PLLP_Clock_Divider */

  uint32_t PLLSAI2R;         /*!< PLLSAI2R: specifies the division factor for ADC clock.
                                  This parameter must be a value of @ref RCC_PLLR_Clock_Divider */

  uint32_t PLLSAI2ClockOut;  /*!< PLLSAIClockOut: specifies PLLSAI2 output clock to be enabled.
                                  This parameter must be a value of @ref RCC_PLLSAI2_Clock_Output */
}RCC_PLLSAI2InitTypeDef;

/**
  * @brief  RCC extended clocks structure definition
  */
typedef struct
{
  uint32_t PeriphClockSelection;   /*!< The Extended Clock to be configured.
                                        This parameter can be a value of @ref RCCEx_Periph_Clock_Selection */

  RCC_PLLSAI1InitTypeDef PLLSAI1;  /*!< PLLSAI1 structure parameters.
                                        This parameter will be used only when PLLSAI1 is selected as Clock Source for SAI1, USB/RNG/SDMMC1 or ADC */

  RCC_PLLSAI2InitTypeDef PLLSAI2;  /*!< PLLSAI2 structure parameters.
                                        This parameter will be used only when PLLSAI2 is selected as Clock Source for SAI2 or ADC */

  uint32_t Usart1ClockSelection;   /*!< Specifies USART1 clock source.
                                        This parameter can be a value of @ref RCCEx_USART1_Clock_Source */

  uint32_t Usart2ClockSelection;   /*!< Specifies USART2 clock source.
                                        This parameter can be a value of @ref RCCEx_USART2_Clock_Source */

  uint32_t Usart3ClockSelection;   /*!< Specifies USART3 clock source.
                                        This parameter can be a value of @ref RCCEx_USART3_Clock_Source */

  uint32_t Uart4ClockSelection;    /*!< Specifies UART4 clock source.
                                        This parameter can be a value of @ref RCCEx_UART4_Clock_Source */

  uint32_t Uart5ClockSelection;    /*!< Specifies UART5 clock source.
                                        This parameter can be a value of @ref RCCEx_UART5_Clock_Source */

  uint32_t Lpuart1ClockSelection;  /*!< Specifies LPUART1 clock source.
                                        This parameter can be a value of @ref RCCEx_LPUART1_Clock_Source */

  uint32_t I2c1ClockSelection;     /*!< Specifies I2C1 clock source.
                                        This parameter can be a value of @ref RCCEx_I2C1_Clock_Source */

  uint32_t I2c2ClockSelection;     /*!< Specifies I2C2 clock source.
                                        This parameter can be a value of @ref RCCEx_I2C2_Clock_Source */

  uint32_t I2c3ClockSelection;     /*!< Specifies I2C3 clock source.
                                        This parameter can be a value of @ref RCCEx_I2C3_Clock_Source */

  uint32_t Lptim1ClockSelection;   /*!< Specifies LPTIM1 clock source.
                                        This parameter can be a value of @ref RCCEx_LPTIM1_Clock_Source */

  uint32_t Lptim2ClockSelection;   /*!< Specifies LPTIM2 clock source.
                                        This parameter can be a value of @ref RCCEx_LPTIM2_Clock_Source */

  uint32_t Sai1ClockSelection;     /*!< Specifies SAI1 clock source.
                                        This parameter can be a value of @ref RCCEx_SAI1_Clock_Source */

  uint32_t Sai2ClockSelection;     /*!< Specifies SAI2 clock source.
                                        This parameter can be a value of @ref RCCEx_SAI2_Clock_Source */

#if defined(STM32L475xx) || defined(STM32L476xx) || defined(STM32L485xx) || defined(STM32L486xx)

  uint32_t UsbClockSelection;      /*!< Specifies USB clock source (warning: same source for SDMMC1 and RNG).
                                        This parameter can be a value of @ref RCCEx_USB_Clock_Source */

#endif /* STM32L475xx || STM32L476xx || STM32L485xx || STM32L486xx */

  uint32_t Sdmmc1ClockSelection;   /*!< Specifies SDMMC1 clock source (warning: same source for USB and RNG).
                                        This parameter can be a value of @ref RCCEx_SDMMC1_Clock_Source */

  uint32_t RngClockSelection;      /*!< Specifies RNG clock source (warning: same source for USB and SDMMC1).
                                        This parameter can be a value of @ref RCCEx_RNG_Clock_Source */

  uint32_t AdcClockSelection;      /*!< Specifies ADC interface clock source.
                                        This parameter can be a value of @ref RCCEx_ADC_Clock_Source */

  uint32_t Swpmi1ClockSelection;   /*!< Specifies SWPMI1 clock source.
                                        This parameter can be a value of @ref RCCEx_SWPMI1_Clock_Source */

  uint32_t DfsdmClockSelection;    /*!< Specifies DFSDM clock source.
                                        This parameter can be a value of @ref RCCEx_DFSDM_Clock_Source */

  uint32_t RTCClockSelection;      /*!< Specifies RTC clock source.
                                        This parameter can be a value of @ref RCC_RTC_Clock_Source */
}RCC_PeriphCLKInitTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup RCCEx_Exported_Constants RCCEx Exported Constants
  * @{
  */

/** @defgroup RCCEx_LSCO_Clock_Source Low Speed Clock Source
  * @{
  */
#define RCC_LSCOSOURCE_LSI             (uint32_t)0x00000000  /*!< LSI selection for low speed clock output */
#define RCC_LSCOSOURCE_LSE             RCC_BDCR_LSCOSEL      /*!< LSE selection for low speed clock output */
/**
  * @}
  */

/** @defgroup RCCEx_Periph_Clock_Selection Periph Clock Selection
  * @{
  */
#define RCC_PERIPHCLK_USART1           ((uint32_t)0x00000001)
#define RCC_PERIPHCLK_USART2           ((uint32_t)0x00000002)
#define RCC_PERIPHCLK_USART3           ((uint32_t)0x00000004)
#define RCC_PERIPHCLK_UART4            ((uint32_t)0x00000008)
#define RCC_PERIPHCLK_UART5            ((uint32_t)0x00000010)
#define RCC_PERIPHCLK_LPUART1          ((uint32_t)0x00000020)
#define RCC_PERIPHCLK_I2C1             ((uint32_t)0x00000040)
#define RCC_PERIPHCLK_I2C2             ((uint32_t)0x00000080)
#define RCC_PERIPHCLK_I2C3             ((uint32_t)0x00000100)
#define RCC_PERIPHCLK_LPTIM1           ((uint32_t)0x00000200)
#define RCC_PERIPHCLK_LPTIM2           ((uint32_t)0x00000400)
#define RCC_PERIPHCLK_SAI1             ((uint32_t)0x00000800)
#define RCC_PERIPHCLK_SAI2             ((uint32_t)0x00001000)
#if defined(STM32L475xx) || defined(STM32L476xx) || defined(STM32L485xx) || defined(STM32L486xx)
#define RCC_PERIPHCLK_USB              ((uint32_t)0x00002000)
#endif /* STM32L475xx || STM32L476xx || STM32L485xx || STM32L486xx */
#define RCC_PERIPHCLK_ADC              ((uint32_t)0x00004000)
#define RCC_PERIPHCLK_SWPMI1           ((uint32_t)0x00008000)
#define RCC_PERIPHCLK_DFSDM            ((uint32_t)0x00010000)
#define RCC_PERIPHCLK_RTC              ((uint32_t)0x00020000)
#define RCC_PERIPHCLK_RNG              ((uint32_t)0x00040000)
#define RCC_PERIPHCLK_SDMMC1           ((uint32_t)0x00080000)
/**
  * @}
  */


/** @defgroup RCCEx_USART1_Clock_Source USART1 Clock Source
  * @{
  */
#define RCC_USART1CLKSOURCE_PCLK2      ((uint32_t)0x00000000)
#define RCC_USART1CLKSOURCE_SYSCLK     RCC_CCIPR_USART1SEL_0
#define RCC_USART1CLKSOURCE_HSI        RCC_CCIPR_USART1SEL_1
#define RCC_USART1CLKSOURCE_LSE        (RCC_CCIPR_USART1SEL_0 | RCC_CCIPR_USART1SEL_1)
/**
  * @}
  */

/** @defgroup RCCEx_USART2_Clock_Source USART2 Clock Source
  * @{
  */
#define RCC_USART2CLKSOURCE_PCLK1      ((uint32_t)0x00000000)
#define RCC_USART2CLKSOURCE_SYSCLK     RCC_CCIPR_USART2SEL_0
#define RCC_USART2CLKSOURCE_HSI        RCC_CCIPR_USART2SEL_1
#define RCC_USART2CLKSOURCE_LSE        (RCC_CCIPR_USART2SEL_0 | RCC_CCIPR_USART2SEL_1)
/**
  * @}
  */

/** @defgroup RCCEx_USART3_Clock_Source USART3 Clock Source
  * @{
  */
#define RCC_USART3CLKSOURCE_PCLK1      ((uint32_t)0x00000000)
#define RCC_USART3CLKSOURCE_SYSCLK     RCC_CCIPR_USART3SEL_0
#define RCC_USART3CLKSOURCE_HSI        RCC_CCIPR_USART3SEL_1
#define RCC_USART3CLKSOURCE_LSE        (RCC_CCIPR_USART3SEL_0 | RCC_CCIPR_USART3SEL_1)
/**
  * @}
  */

/** @defgroup RCCEx_UART4_Clock_Source UART4 Clock Source
  * @{
  */
#define RCC_UART4CLKSOURCE_PCLK1       ((uint32_t)0x00000000)
#define RCC_UART4CLKSOURCE_SYSCLK      RCC_CCIPR_UART4SEL_0
#define RCC_UART4CLKSOURCE_HSI         RCC_CCIPR_UART4SEL_1
#define RCC_UART4CLKSOURCE_LSE         (RCC_CCIPR_UART4SEL_0 | RCC_CCIPR_UART4SEL_1)
/**
  * @}
  */

/** @defgroup RCCEx_UART5_Clock_Source UART5 Clock Source
  * @{
  */
#define RCC_UART5CLKSOURCE_PCLK1       ((uint32_t)0x00000000)
#define RCC_UART5CLKSOURCE_SYSCLK      RCC_CCIPR_UART5SEL_0
#define RCC_UART5CLKSOURCE_HSI         RCC_CCIPR_UART5SEL_1
#define RCC_UART5CLKSOURCE_LSE         (RCC_CCIPR_UART5SEL_0 | RCC_CCIPR_UART5SEL_1)
/**
  * @}
  */

/** @defgroup RCCEx_LPUART1_Clock_Source LPUART1 Clock Source
  * @{
  */
#define RCC_LPUART1CLKSOURCE_PCLK1     ((uint32_t)0x00000000)
#define RCC_LPUART1CLKSOURCE_SYSCLK    RCC_CCIPR_LPUART1SEL_0
#define RCC_LPUART1CLKSOURCE_HSI       RCC_CCIPR_LPUART1SEL_1
#define RCC_LPUART1CLKSOURCE_LSE       (RCC_CCIPR_LPUART1SEL_0 | RCC_CCIPR_LPUART1SEL_1)
/**
  * @}
  */

/** @defgroup RCCEx_I2C1_Clock_Source I2C1 Clock Source
  * @{
  */
#define RCC_I2C1CLKSOURCE_PCLK1        ((uint32_t)0x00000000)
#define RCC_I2C1CLKSOURCE_SYSCLK       RCC_CCIPR_I2C1SEL_0
#define RCC_I2C1CLKSOURCE_HSI          RCC_CCIPR_I2C1SEL_1
/**
  * @}
  */

/** @defgroup RCCEx_I2C2_Clock_Source I2C2 Clock Source
  * @{
  */
#define RCC_I2C2CLKSOURCE_PCLK1        ((uint32_t)0x00000000)
#define RCC_I2C2CLKSOURCE_SYSCLK       RCC_CCIPR_I2C2SEL_0
#define RCC_I2C2CLKSOURCE_HSI          RCC_CCIPR_I2C2SEL_1
/**
  * @}
  */

/** @defgroup RCCEx_I2C3_Clock_Source I2C3 Clock Source
  * @{
  */
#define RCC_I2C3CLKSOURCE_PCLK1        ((uint32_t)0x00000000)
#define RCC_I2C3CLKSOURCE_SYSCLK       RCC_CCIPR_I2C3SEL_0
#define RCC_I2C3CLKSOURCE_HSI          RCC_CCIPR_I2C3SEL_1
/**
  * @}
  */

/** @defgroup RCCEx_SAI1_Clock_Source SAI1 Clock Source
  * @{
  */
#define RCC_SAI1CLKSOURCE_PLLSAI1      ((uint32_t)0x00000000)
#define RCC_SAI1CLKSOURCE_PLLSAI2      RCC_CCIPR_SAI1SEL_0
#define RCC_SAI1CLKSOURCE_PLL          RCC_CCIPR_SAI1SEL_1
#define RCC_SAI1CLKSOURCE_PIN          RCC_CCIPR_SAI1SEL
/**
  * @}
  */

/** @defgroup RCCEx_SAI2_Clock_Source SAI2 Clock Source
  * @{
  */
#define RCC_SAI2CLKSOURCE_PLLSAI1      ((uint32_t)0x00000000)
#define RCC_SAI2CLKSOURCE_PLLSAI2      RCC_CCIPR_SAI2SEL_0
#define RCC_SAI2CLKSOURCE_PLL          RCC_CCIPR_SAI2SEL_1
#define RCC_SAI2CLKSOURCE_PIN          RCC_CCIPR_SAI2SEL
/**
  * @}
  */

/** @defgroup RCCEx_LPTIM1_Clock_Source LPTIM1 Clock Source
  * @{
  */
#define RCC_LPTIM1CLKSOURCE_PCLK       ((uint32_t)0x00000000)
#define RCC_LPTIM1CLKSOURCE_LSI        RCC_CCIPR_LPTIM1SEL_0
#define RCC_LPTIM1CLKSOURCE_HSI        RCC_CCIPR_LPTIM1SEL_1
#define RCC_LPTIM1CLKSOURCE_LSE        RCC_CCIPR_LPTIM1SEL
/**
  * @}
  */

/** @defgroup RCCEx_LPTIM2_Clock_Source LPTIM2 Clock Source
  * @{
  */
#define RCC_LPTIM2CLKSOURCE_PCLK       ((uint32_t)0x00000000)
#define RCC_LPTIM2CLKSOURCE_LSI        RCC_CCIPR_LPTIM2SEL_0
#define RCC_LPTIM2CLKSOURCE_HSI        RCC_CCIPR_LPTIM2SEL_1
#define RCC_LPTIM2CLKSOURCE_LSE        RCC_CCIPR_LPTIM2SEL
/**
  * @}
  */

/** @defgroup RCCEx_SDMMC1_Clock_Source SDMMC1 Clock Source
  * @{
  */
#define RCC_SDMMC1CLKSOURCE_NONE       ((uint32_t)0x00000000)
#define RCC_SDMMC1CLKSOURCE_PLLSAI1    RCC_CCIPR_CLK48SEL_0
#define RCC_SDMMC1CLKSOURCE_PLL        RCC_CCIPR_CLK48SEL_1
#define RCC_SDMMC1CLKSOURCE_MSI        RCC_CCIPR_CLK48SEL
/**
  * @}
  */

/** @defgroup RCCEx_RNG_Clock_Source RNG Clock Source
  * @{
  */
#define RCC_RNGCLKSOURCE_NONE          ((uint32_t)0x00000000)
#define RCC_RNGCLKSOURCE_PLLSAI1       RCC_CCIPR_CLK48SEL_0
#define RCC_RNGCLKSOURCE_PLL           RCC_CCIPR_CLK48SEL_1
#define RCC_RNGCLKSOURCE_MSI           RCC_CCIPR_CLK48SEL
/**
  * @}
  */

#if defined(STM32L475xx) || defined(STM32L476xx) || defined(STM32L485xx) || defined(STM32L486xx)
/** @defgroup RCCEx_USB_Clock_Source USB Clock Source
  * @{
  */
#define RCC_USBCLKSOURCE_NONE          ((uint32_t)0x00000000)
#define RCC_USBCLKSOURCE_PLLSAI1       RCC_CCIPR_CLK48SEL_0
#define RCC_USBCLKSOURCE_PLL           RCC_CCIPR_CLK48SEL_1
#define RCC_USBCLKSOURCE_MSI           RCC_CCIPR_CLK48SEL
/**
  * @}
  */
#endif /* STM32L475xx || STM32L476xx || STM32L485xx || STM32L486xx */

/** @defgroup RCCEx_ADC_Clock_Source ADC Clock Source
  * @{
  */
#define RCC_ADCCLKSOURCE_NONE        ((uint32_t)0x00000000)
#define RCC_ADCCLKSOURCE_PLLSAI1      RCC_CCIPR_ADCSEL_0
#define RCC_ADCCLKSOURCE_PLLSAI2      RCC_CCIPR_ADCSEL_1
#define RCC_ADCCLKSOURCE_SYSCLK       RCC_CCIPR_ADCSEL
/**
  * @}
  */

/** @defgroup RCCEx_SWPMI1_Clock_Source SWPMI1 Clock Source
  * @{
  */
#define RCC_SWPMI1CLKSOURCE_PCLK       ((uint32_t)0x00000000)
#define RCC_SWPMI1CLKSOURCE_HSI        RCC_CCIPR_SWPMI1SEL
/**
  * @}
  */

/** @defgroup RCCEx_DFSDM_Clock_Source DFSDM Clock Source
  * @{
  */
#define RCC_DFSDMCLKSOURCE_PCLK        ((uint32_t)0x00000000)
#define RCC_DFSDMCLKSOURCE_SYSCLK      RCC_CCIPR_DFSDMSEL
/**
  * @}
  */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup RCCEx_Exported_Macros RCCEx Exported Macros
 * @{
 */

/** @defgroup RCCEx_Peripheral_Clock_Enable_Disable AHB2 Peripheral Clock Enable Disable
  * @brief  Enable or disable the AHB2 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{
  */

#if defined(STM32L475xx) || defined(STM32L476xx) || defined(STM32L485xx) || defined(STM32L486xx)
#define __HAL_RCC_USB_OTG_FS_CLK_ENABLE()      do { \
                                                 __IO uint32_t tmpreg; \
                                                 SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN); \
                                                 /* Delay after an RCC peripheral clock enabling */ \
                                                 tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN); \
                                                 UNUSED(tmpreg); \
                                               } while(0)

#define __HAL_RCC_USB_OTG_FS_CLK_DISABLE()     CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN);
#endif /* STM32L475xx || STM32L476xx || STM32L485xx || STM32L486xx */

                                                 
#if defined(STM32L485xx) || defined(STM32L486xx)
#define __HAL_RCC_AES_CLK_ENABLE()             do { \
                                                 __IO uint32_t tmpreg; \
                                                 SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_AESEN); \
                                                 /* Delay after an RCC peripheral clock enabling */ \
                                                 tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_AESEN); \
                                                 UNUSED(tmpreg); \
                                               } while(0)

#define __HAL_RCC_AES_CLK_DISABLE()            CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_AESEN);
#endif /* STM32L485xx || STM32L486xx */

/**
  * @}
  */

/** @defgroup RCCEx_APB1_Clock_Enable_Disable APB1 Peripheral Clock Enable Disable
  * @brief  Enable or disable the APB1 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{
  */
#if defined(STM32L476xx) || defined(STM32L486xx)
#define __HAL_RCC_LCD_CLK_ENABLE()             do { \
                                                 __IO uint32_t tmpreg; \
                                                 SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_LCDEN); \
                                                 /* Delay after an RCC peripheral clock enabling */ \
                                                 tmpreg = READ_BIT(RCC->APB1ENR1, RCC_APB1ENR1_LCDEN); \
                                                 UNUSED(tmpreg); \
                                               } while(0)

#define __HAL_RCC_LCD_CLK_DISABLE()            CLEAR_BIT(RCC->APB1ENR1, RCC_APB1ENR1_LCDEN);
#endif /* STM32L476xx || STM32L486xx */

/**
  * @}
  */

/** @defgroup RCCEx_Peripheral_Clock_Enable_Disable_Status AHB2 Peripheral Clock Enable Disable Status
  * @brief  Macros to get the status of the AHB2 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{
  */

#if defined(STM32L475xx) || defined(STM32L476xx) || defined(STM32L486xx)  
#define __HAL_RCC_USB_OTG_FS_IS_CLK_ENABLED()    (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN) != RESET)

#define __HAL_RCC_USB_OTG_FS_IS_CLK_DISABLED()   (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN) == RESET)
#endif /* STM32L475xx || STM32L476xx || STM32L486xx */

                                                 
#if defined(STM32L485xx) || defined(STM32L486xx)
#define __HAL_RCC_AES_IS_CLK_ENABLED()           (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_AESEN) != RESET)

#define __HAL_RCC_AES_IS_CLK_DISABLED()          (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_AESEN) == RESET)
#endif /* STM32L485xx || STM32L486xx */

/**
  * @}
  */

/** @defgroup RCCEx_APB1_Clock_Enable_Disable_Status APB1 Peripheral Clock Enable Disable Status
  * @brief  Macros to get the status of the APB1 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{
  */
#if defined(STM32L476xx) || defined(STM32L486xx)
#define __HAL_RCC_LCD_IS_CLK_ENABLED()           (READ_BIT(RCC->APB1ENR1, RCC_APB1ENR1_LCDEN) != RESET)

#define __HAL_RCC_LCD_IS_CLK_DISABLED()          (READ_BIT(RCC->APB1ENR1, RCC_APB1ENR1_LCDEN) == RESET)
#endif /* STM32L476xx || STM32L486xx */

/**
  * @}
  */

/** @defgroup RCCEx_AHB2_Force_Release_Reset AHB2 Peripheral Force Release Reset
  * @brief  Force or release AHB2 peripheral reset.
  * @{
  */

#if defined(STM32L475xx) || defined(STM32L476xx) || defined(STM32L485xx) || defined(STM32L486xx)
#define __HAL_RCC_USB_OTG_FS_FORCE_RESET()     SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_OTGFSRST)

#define __HAL_RCC_USB_OTG_FS_RELEASE_RESET()   CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_OTGFSRST)
#endif /* STM32L475xx || STM32L476xx || STM32L485xx || STM32L486xx */

#if defined(STM32L485xx) || defined(STM32L486xx)
#define __HAL_RCC_AES_FORCE_RESET()            SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_AESRST)

#define __HAL_RCC_AES_RELEASE_RESET()          CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_AESRST)
#endif /* STM32L485xx || STM32L486xx */

/**
  * @}
  */


/** @defgroup RCCEx_APB1_Force_Release_Reset APB1 Peripheral Force Release Reset
  * @brief  Force or release APB1 peripheral reset.
  * @{
  */
#if defined(STM32L476xx) || defined(STM32L486xx)
#define __HAL_RCC_LCD_FORCE_RESET()            SET_BIT(RCC->APB1RSTR1, RCC_APB1RSTR1_LCDRST)

#define __HAL_RCC_LCD_RELEASE_RESET()          CLEAR_BIT(RCC->APB1RSTR1, RCC_APB1RSTR1_LCDRST)
#endif /* STM32L476xx || STM32L486xx */

/**
  * @}
  */


/** @defgroup RCCEx_AHB2_Clock_Sleep_Enable_Disable AHB2 Peripheral Clock Sleep Enable Disable
  * @brief  Enable or disable the AHB2 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  * @{
  */

#if defined(STM32L475xx) || defined(STM32L476xx) || defined(STM32L485xx) || defined(STM32L486xx)
#define __HAL_RCC_USB_OTG_FS_CLK_SLEEP_ENABLE()  SET_BIT(RCC->AHB2SMENR, RCC_AHB2SMENR_OTGFSSMEN)

#define __HAL_RCC_USB_OTG_FS_CLK_SLEEP_DISABLE() CLEAR_BIT(RCC->AHB2SMENR, RCC_AHB2SMENR_OTGFSSMEN)
#endif /* STM32L475xx || STM32L476xx || STM32L485xx || STM32L486xx */

#if defined(STM32L485xx) || defined(STM32L486xx)
#define __HAL_RCC_AES_CLK_SLEEP_ENABLE()       SET_BIT(RCC->AHB2SMENR, RCC_AHB2SMENR_AESSMEN)

#define __HAL_RCC_AES_CLK_SLEEP_DISABLE()      CLEAR_BIT(RCC->AHB2SMENR, RCC_AHB2SMENR_AESSMEN)
#endif /* STM32L485xx || STM32L486xx */

/**
  * @}
  */


/** @defgroup RCCEx_APB1_Clock_Sleep_Enable_Disable APB1 Peripheral Clock Sleep Enable Disable
  * @brief  Enable or disable the APB1 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  * @{
  */

#if defined(STM32L476xx) || defined(STM32L486xx)
#define __HAL_RCC_LCD_CLK_SLEEP_ENABLE()       SET_BIT(RCC->APB1SMENR1, RCC_APB1SMENR1_LCDSMEN)

#define __HAL_RCC_LCD_CLK_SLEEP_DISABLE()      CLEAR_BIT(RCC->APB1SMENR1, RCC_APB1SMENR1_LCDSMEN)
#endif /* STM32L476xx || STM32L486xx */

/**
  * @}
  */

/** @defgroup RCCEx_AHB2_Clock_Sleep_Enable_Disable_Status AHB2 Peripheral Clock Sleep Enable Disable Status
  * @brief  Macros to get the status of the AHB2 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  * @{
  */

#if defined(STM32L475xx) || defined(STM32L476xx) || defined(STM32L485xx) || defined(STM32L486xx)
#define __HAL_RCC_USB_OTG_FS_IS_CLK_SLEEP_ENABLED()  (READ_BIT(RCC->AHB2SMENR, RCC_AHB2SMENR_OTGFSSMEN) != RESET)

#define __HAL_RCC_USB_OTG_FS_IS_CLK_SLEEP_DISABLED() (READ_BIT(RCC->AHB2SMENR, RCC_AHB2SMENR_OTGFSSMEN) == RESET)
#endif /* STM32L475xx || STM32L476xx || STM32L485xx || STM32L486xx */

#if defined(STM32L485xx) || defined(STM32L486xx)
#define __HAL_RCC_AES_IS_CLK_SLEEP_ENABLED()         (READ_BIT(RCC->AHB2SMENR, RCC_AHB2SMENR_AESSMEN) != RESET)

#define __HAL_RCC_AES_IS_CLK_SLEEP_DISABLED()        (READ_BIT(RCC->AHB2SMENR, RCC_AHB2SMENR_AESSMEN) == RESET)
#endif /* STM32L485xx || STM32L486xx */

/**
  * @}
  */


/** @defgroup RCCEx_APB1_Clock_Sleep_Enable_Disable_Status APB1 Peripheral Clock Sleep Enable Disable Status
  * @brief  Macros to get the status of the APB1 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  * @{
  */

#if defined(STM32L476xx) || defined(STM32L486xx)
#define __HAL_RCC_LCD_IS_CLK_SLEEP_ENABLED()       (READ_BIT(RCC->APB1SMENR1, RCC_APB1SMENR1_LCDSMEN) != RESET)

#define __HAL_RCC_LCD_IS_CLK_SLEEP_DISABLED()      (READ_BIT(RCC->APB1SMENR1, RCC_APB1SMENR1_LCDSMEN) == RESET)
#endif /* STM32L476xx || STM32L486xx */

/**
  * @}
  */

/**
  * @brief  Macro to configure the PLLSAI1 clock multiplication and division factors.
  *
  * @note   This function must be used only when the PLLSAI1 is disabled.
  * @note   PLLSAI1 clock source is common with the main PLL (configured through
  *         __HAL_RCC_PLL_CONFIG() macro)
  *
  * @param  __PLLSAI1N__: specifies the multiplication factor for PLLSAI1 VCO output clock.
  *         This parameter must be a number between 8 and 86.
  * @note   You have to set the PLLSAI1N parameter correctly to ensure that the VCO
  *         output frequency is between 64 and 344 MHz.
  *         PLLSAI1 clock frequency = f(PLLSAI1) multiplied by PLLSAI1N
  *
  * @param  __PLLSAI1P__: specifies the division factor for SAI clock.
  *         This parameter must be a number in the range (7 or 17).
  *         SAI1 clock frequency = f(PLLSAI1) / PLLSAI1P
  *
  * @param  __PLLSAI1Q__: specifies the division factor for USB/RNG/SDMMC1 clock.
  *         This parameter must be in the range (2, 4, 6 or 8).
  *         USB/RNG/SDMMC1 clock frequency = f(PLLSAI1) / PLLSAI1Q
  *
  * @param  __PLLSAI1R__: specifies the division factor for SAR ADC clock.
  *         This parameter must be in the range (2, 4, 6 or 8).
  *         ADC clock frequency = f(PLLSAI1) / PLLSAI1R
  *
  * @retval None
  */
#define __HAL_RCC_PLLSAI1_CONFIG(__PLLSAI1N__, __PLLSAI1P__, __PLLSAI1Q__, __PLLSAI1R__) \
                  WRITE_REG(RCC->PLLSAI1CFGR, ((__PLLSAI1N__) << 8U) | (((__PLLSAI1P__) >> 4U) << 17U) | \
                   ((((__PLLSAI1Q__) >> 1U) - 1) << 21U) | ((((__PLLSAI1R__) >> 1U) - 1) << 25U))
                    
/**
  * @brief  Macro to configure the PLLSAI1 clock multiplication factor N.
  *
  * @note   This function must be used only when the PLLSAI1 is disabled.
  * @note   PLLSAI1 clock source is common with the main PLL (configured through
  *         __HAL_RCC_PLL_CONFIG() macro)
  *
  * @param  __PLLSAI1N__: specifies the multiplication factor for PLLSAI1 VCO output clock.
  *          This parameter must be a number between 8 and 86.
  * @note   You have to set the PLLSAI1N parameter correctly to ensure that the VCO
  *         output frequency is between 64 and 344 MHz.
  *         Use to set PLLSAI1 clock frequency = f(PLLSAI1) multiplied by PLLSAI1N
  *
  * @retval None
  */
#define __HAL_RCC_PLLSAI1_MULN_CONFIG(__PLLSAI1N__) \
                  MODIFY_REG(RCC->PLLSAI1CFGR, RCC_PLLSAI1CFGR_PLLSAI1N, (__PLLSAI1N__) << 8U)

/** @brief  Macro to configure the PLLSAI1 clock division factor P.
  *
  * @note   This function must be used only when the PLLSAI1 is disabled.
  * @note   PLLSAI1 clock source is common with the main PLL (configured through
  *         __HAL_RCC_PLL_CONFIG() macro)
  *
  * @param  __PLLSAI1P__: specifies the division factor for SAI clock.
  *         This parameter must be a number in the range (7 or 17).
  *         Use to set SAI1 clock frequency = f(PLLSAI1) / PLLSAI1P
  *
  * @retval None
  */
#define __HAL_RCC_PLLSAI1_DIVP_CONFIG(__PLLSAI1P__) \
                  MODIFY_REG(RCC->PLLSAI1CFGR, RCC_PLLSAI1CFGR_PLLSAI1P, ((__PLLSAI1P__) >> 4U) << 17U)

/** @brief  Macro to configure the PLLSAI1 clock division factor Q.
  *
  * @note   This function must be used only when the PLLSAI1 is disabled.
  * @note   PLLSAI1 clock source is common with the main PLL (configured through
  *         __HAL_RCC_PLL_CONFIG() macro)
  *
  * @param  __PLLSAI1Q__: specifies the division factor for USB/RNG/SDMMC1 clock.
  *         This parameter must be in the range (2, 4, 6 or 8).
  *         Use to set USB/RNG/SDMMC1 clock frequency = f(PLLSAI1) / PLLSAI1Q
  *
  * @retval None
  */
#define __HAL_RCC_PLLSAI1_DIVQ_CONFIG(__PLLSAI1Q__) \
                  MODIFY_REG(RCC->PLLSAI1CFGR, RCC_PLLSAI1CFGR_PLLSAI1Q, (((__PLLSAI1Q__) >> 1U) - 1) << 21U)

/** @brief  Macro to configure the PLLSAI1 clock division factor R.
  *
  * @note   This function must be used only when the PLLSAI1 is disabled.
  * @note   PLLSAI1 clock source is common with the main PLL (configured through
  *         __HAL_RCC_PLL_CONFIG() macro)
  *
  * @param  __PLLSAI1R__: specifies the division factor for ADC clock.
  *         This parameter must be in the range (2, 4, 6 or 8)
  *         Use to set ADC clock frequency = f(PLLSAI1) / PLLSAI1R
  *
  * @retval None
  */
#define __HAL_RCC_PLLSAI1_DIVR_CONFIG(__PLLSAI1R__) \
                  MODIFY_REG(RCC->PLLSAI1CFGR, RCC_PLLSAI1CFGR_PLLSAI1R, (((__PLLSAI1R__) >> 1U) - 1) << 25U)

/**
  * @brief  Macros to enable or disable the PLLSAI1.
  * @note   The PLLSAI1 is disabled by hardware when entering STOP and STANDBY modes.
  * @retval None
  */

#define __HAL_RCC_PLLSAI1_ENABLE()  SET_BIT(RCC->CR, RCC_CR_PLLSAI1ON)

#define __HAL_RCC_PLLSAI1_DISABLE() CLEAR_BIT(RCC->CR, RCC_CR_PLLSAI1ON)

/**
  * @brief  Macros to enable or disable each clock output (PLLSAI1_SAI1, PLLSAI1_USB2 and PLLSAI1_ADC1).
  * @note   Enabling and disabling those clocks can be done without the need to stop the PLL.
  *         This is mainly used to save Power.
  * @param  __PLLSAI1_CLOCKOUT__: specifies the PLLSAI1 clock to be output.
  *         This parameter can be one or a combination of the following values:
  *            @arg RCC_PLLSAI1_SAI1CLK: This clock is used to generate an accurate clock to achieve
  *                                   high-quality audio performance on SAI interface in case.
  *            @arg RCC_PLLSAI1_USB2CLK: This clock is used to generate the clock for the USB OTG FS (48 MHz),
  *                                   the random number generator (<=48 MHz) and the SDIO (<= 48 MHz).
  *            @arg RCC_PLLSAI1_ADC1CLK: Clock used to clock ADC peripheral.
  * @retval None
  */

#define __HAL_RCC_PLLSAI1CLKOUT_ENABLE(__PLLSAI1_CLOCKOUT__)   SET_BIT(RCC->PLLSAI1CFGR, (__PLLSAI1_CLOCKOUT__))

#define __HAL_RCC_PLLSAI1CLKOUT_DISABLE(__PLLSAI1_CLOCKOUT__)  CLEAR_BIT(RCC->PLLSAI1CFGR, (__PLLSAI1_CLOCKOUT__))

/**
  * @brief  Macro to get clock output enable status (PLLSAI1_SAI1, PLLSAI1_USB2 and PLLSAI1_ADC1).
  * @param  __PLLSAI1_CLOCKOUT__: specifies the PLLSAI1 clock to be output.
  *         This parameter can be one of the following values:
  *            @arg RCC_PLLSAI1_SAI1CLK: This clock is used to generate an accurate clock to achieve
  *                                   high-quality audio performance on SAI interface in case.
  *            @arg RCC_PLLSAI1_USB2CLK: This clock is used to generate the clock for the USB OTG FS (48 MHz),
  *                                   the random number generator (<=48 MHz) and the SDIO (<= 48 MHz).
  *            @arg RCC_PLLSAI1_ADC1CLK: Clock used to clock ADC peripheral.
  * @retval SET / RESET
  */
#define __HAL_RCC_GET_PLLSAI1CLKOUT_CONFIG(__PLLSAI1_CLOCKOUT__)  READ_BIT(RCC->PLLSAI1CFGR, (__PLLSAI1_CLOCKOUT__))

/**
  * @brief  Macro to configure the PLLSAI2 clock multiplication and division factors.
  *
  * @note   This function must be used only when the PLLSAI2 is disabled.
  * @note   PLLSAI2 clock source is common with the main PLL (configured through
  *         __HAL_RCC_PLL_CONFIG() macro)
  *
  * @param  __PLLSAI2N__: specifies the multiplication factor for PLLSAI2 VCO output clock.
  *          This parameter must be a number between 8 and 86.
  * @note   You have to set the PLLSAI2N parameter correctly to ensure that the VCO
  *         output frequency is between 64 and 344 MHz.
  *
  * @param  __PLLSAI2P__: specifies the division factor for SAI clock.
  *          This parameter must be a number in the range (7 or 17).
  *
  *
  * @param  __PLLSAI2R__: specifies the division factor for SAR ADC clock.
  *          This parameter must be in the range (2, 4, 6 or 8)
  *
  * @retval None
  */

#define __HAL_RCC_PLLSAI2_CONFIG(__PLLSAI2N__, __PLLSAI2P__, __PLLSAI2R__) \
                  WRITE_REG(RCC->PLLSAI2CFGR, ((__PLLSAI2N__) << 8U) | (((__PLLSAI2P__) >> 4U) << 17U) | \
                   ((((__PLLSAI2R__) >> 1U) - 1) << 25U))

/**
  * @brief  Macro to configure the PLLSAI2 clock multiplication factor N.
  *
  * @note   This function must be used only when the PLLSAI2 is disabled.
  * @note   PLLSAI2 clock source is common with the main PLL (configured through
  *         __HAL_RCC_PLL_CONFIG() macro)
  *
  * @param  __PLLSAI2N__: specifies the multiplication factor for PLLSAI2 VCO output clock.
  *          This parameter must be a number between 8 and 86.
  * @note   You have to set the PLLSAI2N parameter correctly to ensure that the VCO
  *         output frequency is between 64 and 344 MHz.
  *         PLLSAI1 clock frequency = f(PLLSAI1) multiplied by PLLSAI2N
  *
  * @retval None
  */
#define __HAL_RCC_PLLSAI2_MULN_CONFIG(__PLLSAI2N__) \
                  MODIFY_REG(RCC->PLLSAI2CFGR, RCC_PLLSAI2CFGR_PLLSAI2N, (__PLLSAI2N__) << 8U)

/** @brief  Macro to configure the PLLSAI2 clock division factor P.
  *
  * @note   This function must be used only when the PLLSAI2 is disabled.
  * @note   PLLSAI2 clock source is common with the main PLL (configured through
  *         __HAL_RCC_PLL_CONFIG() macro)
  *
  * @param  __PLLSAI2P__: specifies the division factor.
  *         This parameter must be a number in the range (7 or 17).
  *         Use to set SAI2 clock frequency = f(PLLSAI2) / __PLLSAI2P__
  *
  * @retval None
  */
#define __HAL_RCC_PLLSAI2_DIVP_CONFIG(__PLLSAI2P__) \
                  MODIFY_REG(RCC->PLLSAI2CFGR, RCC_PLLSAI2CFGR_PLLSAI2P, ((__PLLSAI2P__) >> 4U) << 17U)

/** @brief  Macro to configure the PLLSAI2 clock division factor R.
  *
  * @note   This function must be used only when the PLLSAI2 is disabled.
  * @note   PLLSAI1 clock source is common with the main PLL (configured through
  *         __HAL_RCC_PLL_CONFIG() macro)
  *
  * @param  __PLLSAI2R__: specifies the division factor.
  *         This parameter must be in the range (2, 4, 6 or 8).
  *         Use to set ADC clock frequency = f(PLLSAI2) / __PLLSAI2Q__
  *
  * @retval None
  */
#define __HAL_RCC_PLLSAI2_DIVR_CONFIG(__PLLSAI2R__) \
                  MODIFY_REG(RCC->PLLSAI2CFGR, RCC_PLLSAI2CFGR_PLLSAI2R, (((__PLLSAI2R__) >> 1U) - 1) << 25U)

/**
  * @brief  Macros to enable or disable the PLLSAI2.
  * @note   The PLLSAI2 is disabled by hardware when entering STOP and STANDBY modes.
  * @retval None
  */

#define __HAL_RCC_PLLSAI2_ENABLE()  SET_BIT(RCC->CR, RCC_CR_PLLSAI2ON)

#define __HAL_RCC_PLLSAI2_DISABLE() CLEAR_BIT(RCC->CR, RCC_CR_PLLSAI2ON)

/**
  * @brief  Macros to enable or disable each clock output (PLLSAI2_SAI2 and PLLSAI2_ADC2).
  * @note   Enabling and disabling those clocks can be done without the need to stop the PLL.
  *         This is mainly used to save Power.
  * @param  __PLLSAI2_CLOCKOUT__: specifies the PLLSAI2 clock to be output.
  *         This parameter can be one or a combination of the following values:
  *            @arg RCC_PLLSAI2_SAI2CLK: This clock is used to generate an accurate clock to achieve
  *                                   high-quality audio performance on SAI interface in case.
  *            @arg RCC_PLLSAI2_ADC2CLK: Clock used to clock ADC peripheral.
  * @retval None
  */

#define __HAL_RCC_PLLSAI2CLKOUT_ENABLE(__PLLSAI2_CLOCKOUT__)  SET_BIT(RCC->PLLSAI2CFGR, (__PLLSAI2_CLOCKOUT__))

#define __HAL_RCC_PLLSAI2CLKOUT_DISABLE(__PLLSAI2_CLOCKOUT__) CLEAR_BIT(RCC->PLLSAI2CFGR, (__PLLSAI2_CLOCKOUT__))

/**
  * @brief  Macro to get clock output enable status (PLLSAI2_SAI2 and PLLSAI2_ADC2).
  * @param  __PLLSAI2_CLOCKOUT__: specifies the PLLSAI2 clock to be output.
  *          This parameter can be one of the following values:
  *            @arg RCC_PLLSAI2_SAI2CLK: This clock is used to generate an accurate clock to achieve
  *                                   high-quality audio performance on SAI interface in case.
  *            @arg RCC_PLLSAI2_ADC2CLK: Clock used to clock ADC peripheral.
  * @retval SET / RESET
  */
#define __HAL_RCC_GET_PLLSAI2CLKOUT_CONFIG(__PLLSAI2_CLOCKOUT__)  READ_BIT(RCC->PLLSAI2CFGR, (__PLLSAI2_CLOCKOUT__))

/**
  * @brief  Macro to configure the SAI1 clock source.
  * @param  __SAI1_CLKSOURCE__: defines the SAI1 clock source. This clock is derived
  *         from the PLLSAI1, system PLL or external clock (through a dedicated pin).
  *          This parameter can be one of the following values:
  *             @arg RCC_SAI1CLKSOURCE_PLLSAI1: SAI1 clock = PLLSAI1 "P" clock (PLLSAI1CLK)
  *             @arg RCC_SAI1CLKSOURCE_PLLSAI2: SAI1 clock = PLLSAI2 "P" clock (PLLSAI2CLK)
  *             @arg RCC_SAI1CLKSOURCE_PLL: SAI1 clock  = PLL "P" clock (PLLSAI3CLK)
  *             @arg RCC_SAI1CLKSOURCE_PIN: SAI1 clock = External Clock (SAI1_EXTCLK)
  * @retval None
  */
#define __HAL_RCC_SAI1_CONFIG(__SAI1_CLKSOURCE__)\
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_SAI1SEL, (uint32_t)(__SAI1_CLKSOURCE__))

/** @brief  Macro to get the SAI1 clock source.
  * @retval The clock source can be one of the following values:
  *             @arg RCC_SAI1CLKSOURCE_PLLSAI1: SAI1 clock = PLLSAI1 "P" clock (PLLSAI1CLK)
  *             @arg RCC_SAI1CLKSOURCE_PLLSAI2: SAI1 clock = PLLSAI2 "P" clock (PLLSAI2CLK)
  *             @arg RCC_SAI1CLKSOURCE_PLL: SAI1 clock  = PLL "P" clock (PLLSAI3CLK)
  *             @arg RCC_SAI1CLKSOURCE_PIN: SAI1 clock = External Clock (SAI1_EXTCLK)
  */
#define __HAL_RCC_GET_SAI1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_SAI1SEL)))

/**
  * @brief  Macro to configure the SAI2 clock source.
  * @param  __SAI2_CLKSOURCE__: defines the SAI2 clock source. This clock is derived
  *         from the PLLSAI2, system PLL or external clock (through a dedicated pin).
  *          This parameter can be one of the following values:
  *             @arg RCC_SAI2CLKSOURCE_PLLSAI1: SAI2 clock = PLLSAI1 "P" clock (PLLSAI1CLK)
  *             @arg RCC_SAI2CLKSOURCE_PLLSAI2: SAI2 clock = PLLSAI2 "P" clock (PLLSAI2CLK)
  *             @arg RCC_SAI2CLKSOURCE_PLL: SAI2 clock  = PLL "P" clock (PLLSAI3CLK)
  *             @arg RCC_SAI2CLKSOURCE_PIN: SAI2 clock = External Clock (SAI2_EXTCLK)
  * @retval None
  */
#define __HAL_RCC_SAI2_CONFIG(__SAI2_CLKSOURCE__ )\
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_SAI2SEL, (uint32_t)(__SAI2_CLKSOURCE__))

/** @brief  Macro to get the SAI2 clock source.
  * @retval The clock source can be one of the following values:
  *             @arg RCC_SAI2CLKSOURCE_PLLSAI1: SAI2 clock = PLLSAI1 "P" clock (PLLSAI1CLK)
  *             @arg RCC_SAI2CLKSOURCE_PLLSAI2: SAI2 clock = PLLSAI2 "P" clock (PLLSAI2CLK)
  *             @arg RCC_SAI2CLKSOURCE_PLL: SAI2 clock  = PLL "P" clock (PLLSAI3CLK)
  *             @arg RCC_SAI2CLKSOURCE_PIN: SAI2 clock = External Clock (SAI2_EXTCLK)
  */
#define __HAL_RCC_GET_SAI2_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_SAI2SEL)))

/** @brief  Macro to configure the I2C1 clock (I2C1CLK).
  *
  * @param  __I2C1_CLKSOURCE__: specifies the I2C1 clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_I2C1CLKSOURCE_PCLK1: PCLK1 selected as I2C1 clock
  *            @arg RCC_I2C1CLKSOURCE_HSI: HSI selected as I2C1 clock
  *            @arg RCC_I2C1CLKSOURCE_SYSCLK: System Clock selected as I2C1 clock
  * @retval None
  */
#define __HAL_RCC_I2C1_CONFIG(__I2C1_CLKSOURCE__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_I2C1SEL, (uint32_t)(__I2C1_CLKSOURCE__))

/** @brief  Macro to get the I2C1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_I2C1CLKSOURCE_PCLK1: PCLK1 selected as I2C1 clock
  *            @arg RCC_I2C1CLKSOURCE_HSI: HSI selected as I2C1 clock
  *            @arg RCC_I2C1CLKSOURCE_SYSCLK: System Clock selected as I2C1 clock
  */
#define __HAL_RCC_GET_I2C1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_I2C1SEL)))

/** @brief  Macro to configure the I2C2 clock (I2C2CLK).
  *
  * @param  __I2C2_CLKSOURCE__: specifies the I2C2 clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_I2C2CLKSOURCE_PCLK1: PCLK1 selected as I2C2 clock
  *            @arg RCC_I2C2CLKSOURCE_HSI: HSI selected as I2C2 clock
  *            @arg RCC_I2C2CLKSOURCE_SYSCLK: System Clock selected as I2C2 clock
  * @retval None
  */
#define __HAL_RCC_I2C2_CONFIG(__I2C2_CLKSOURCE__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_I2C2SEL, (uint32_t)(__I2C2_CLKSOURCE__))

/** @brief  Macro to get the I2C2 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_I2C2CLKSOURCE_PCLK1: PCLK1 selected as I2C2 clock
  *            @arg RCC_I2C2CLKSOURCE_HSI: HSI selected as I2C2 clock
  *            @arg RCC_I2C2CLKSOURCE_SYSCLK: System Clock selected as I2C2 clock
  */
#define __HAL_RCC_GET_I2C2_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_I2C2SEL)))

/** @brief  Macro to configure the I2C3 clock (I2C3CLK).
  *
  * @param  __I2C3_CLKSOURCE__: specifies the I2C3 clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_I2C3CLKSOURCE_PCLK1: PCLK1 selected as I2C3 clock
  *            @arg RCC_I2C3CLKSOURCE_HSI: HSI selected as I2C3 clock
  *            @arg RCC_I2C3CLKSOURCE_SYSCLK: System Clock selected as I2C3 clock
  * @retval None
  */
#define __HAL_RCC_I2C3_CONFIG(__I2C3_CLKSOURCE__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_I2C3SEL, (uint32_t)(__I2C3_CLKSOURCE__))

/** @brief  Macro to get the I2C3 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_I2C3CLKSOURCE_PCLK1: PCLK1 selected as I2C3 clock
  *            @arg RCC_I2C3CLKSOURCE_HSI: HSI selected as I2C3 clock
  *            @arg RCC_I2C3CLKSOURCE_SYSCLK: System Clock selected as I2C3 clock
  */
#define __HAL_RCC_GET_I2C3_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_I2C3SEL)))

/** @brief  Macro to configure the USART1 clock (USART1CLK).
  *
  * @param  __USART1_CLKSOURCE__: specifies the USART1 clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_USART1CLKSOURCE_PCLK2: PCLK2 selected as USART1 clock
  *            @arg RCC_USART1CLKSOURCE_HSI: HSI selected as USART1 clock
  *            @arg RCC_USART1CLKSOURCE_SYSCLK: System Clock selected as USART1 clock
  *            @arg RCC_USART1CLKSOURCE_LSE: LSE selected as USART1 clock
  * @retval None
  */
#define __HAL_RCC_USART1_CONFIG(__USART1_CLKSOURCE__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_USART1SEL, (uint32_t)(__USART1_CLKSOURCE__))

/** @brief  Macro to get the USART1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_USART1CLKSOURCE_PCLK2: PCLK2 selected as USART1 clock
  *            @arg RCC_USART1CLKSOURCE_HSI: HSI selected as USART1 clock
  *            @arg RCC_USART1CLKSOURCE_SYSCLK: System Clock selected as USART1 clock
  *            @arg RCC_USART1CLKSOURCE_LSE: LSE selected as USART1 clock
  */
#define __HAL_RCC_GET_USART1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_USART1SEL)))

/** @brief  Macro to configure the USART2 clock (USART2CLK).
  *
  * @param  __USART2_CLKSOURCE__: specifies the USART2 clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_USART2CLKSOURCE_PCLK1: PCLK1 selected as USART2 clock
  *            @arg RCC_USART2CLKSOURCE_HSI: HSI selected as USART2 clock
  *            @arg RCC_USART2CLKSOURCE_SYSCLK: System Clock selected as USART2 clock
  *            @arg RCC_USART2CLKSOURCE_LSE: LSE selected as USART2 clock
  * @retval None
  */
#define __HAL_RCC_USART2_CONFIG(__USART2_CLKSOURCE__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_USART2SEL, (uint32_t)(__USART2_CLKSOURCE__))

/** @brief  Macro to get the USART2 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_USART2CLKSOURCE_PCLK1: PCLK1 selected as USART2 clock
  *            @arg RCC_USART2CLKSOURCE_HSI: HSI selected as USART2 clock
  *            @arg RCC_USART2CLKSOURCE_SYSCLK: System Clock selected as USART2 clock
  *            @arg RCC_USART2CLKSOURCE_LSE: LSE selected as USART2 clock
  */
#define __HAL_RCC_GET_USART2_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_USART2SEL)))

/** @brief  Macro to configure the USART3 clock (USART3CLK).
  *
  * @param  __USART3_CLKSOURCE__: specifies the USART3 clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_USART3CLKSOURCE_PCLK1: PCLK1 selected as USART3 clock
  *            @arg RCC_USART3CLKSOURCE_HSI: HSI selected as USART3 clock
  *            @arg RCC_USART3CLKSOURCE_SYSCLK: System Clock selected as USART3 clock
  *            @arg RCC_USART3CLKSOURCE_LSE: LSE selected as USART3 clock
  * @retval None
  */
#define __HAL_RCC_USART3_CONFIG(__USART3_CLKSOURCE__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_USART3SEL, (uint32_t)(__USART3_CLKSOURCE__))

/** @brief  Macro to get the USART3 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_USART3CLKSOURCE_PCLK1: PCLK1 selected as USART3 clock
  *            @arg RCC_USART3CLKSOURCE_HSI: HSI selected as USART3 clock
  *            @arg RCC_USART3CLKSOURCE_SYSCLK: System Clock selected as USART3 clock
  *            @arg RCC_USART3CLKSOURCE_LSE: LSE selected as USART3 clock
  */
#define __HAL_RCC_GET_USART3_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_USART3SEL)))

 /** @brief  Macro to configure the UART4 clock (UART4CLK).
  *
  * @param  __UART4_CLKSOURCE__: specifies the UART4 clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_UART4CLKSOURCE_PCLK1: PCLK1 selected as UART4 clock
  *            @arg RCC_UART4CLKSOURCE_HSI: HSI selected as UART4 clock
  *            @arg RCC_UART4CLKSOURCE_SYSCLK: System Clock selected as UART4 clock
  *            @arg RCC_UART4CLKSOURCE_LSE: LSE selected as UART4 clock
  * @retval None
  */
#define __HAL_RCC_UART4_CONFIG(__UART4_CLKSOURCE__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_UART4SEL, (uint32_t)(__UART4_CLKSOURCE__))

/** @brief  Macro to get the UART4 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_UART4CLKSOURCE_PCLK1: PCLK1 selected as UART4 clock
  *            @arg RCC_UART4CLKSOURCE_HSI: HSI selected as UART4 clock
  *            @arg RCC_UART4CLKSOURCE_SYSCLK: System Clock selected as UART4 clock
  *            @arg RCC_UART4CLKSOURCE_LSE: LSE selected as UART4 clock
  */
#define __HAL_RCC_GET_UART4_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_UART4SEL)))

 /** @brief  Macro to configure the UART5 clock (UART5CLK).
  *
  * @param  __UART5_CLKSOURCE__: specifies the UART5 clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_UART5CLKSOURCE_PCLK1: PCLK1 selected as UART5 clock
  *            @arg RCC_UART5CLKSOURCE_HSI: HSI selected as UART5 clock
  *            @arg RCC_UART5CLKSOURCE_SYSCLK: System Clock selected as UART5 clock
  *            @arg RCC_UART5CLKSOURCE_LSE: LSE selected as UART5 clock
  * @retval None
  */
#define __HAL_RCC_UART5_CONFIG(__UART5_CLKSOURCE__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_UART5SEL, (uint32_t)(__UART5_CLKSOURCE__))

/** @brief  Macro to get the UART5 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_UART5CLKSOURCE_PCLK1: PCLK1 selected as UART5 clock
  *            @arg RCC_UART5CLKSOURCE_HSI: HSI selected as UART5 clock
  *            @arg RCC_UART5CLKSOURCE_SYSCLK: System Clock selected as UART5 clock
  *            @arg RCC_UART5CLKSOURCE_LSE: LSE selected as UART5 clock
  */
#define __HAL_RCC_GET_UART5_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_UART5SEL)))

/** @brief  Macro to configure the LPUART1 clock (LPUART1CLK).
  *
  * @param  __LPUART1_CLKSOURCE__: specifies the LPUART1 clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_LPUART1CLKSOURCE_PCLK1: PCLK1 selected as LPUART1 clock
  *            @arg RCC_LPUART1CLKSOURCE_HSI: HSI selected as LPUART1 clock
  *            @arg RCC_LPUART1CLKSOURCE_SYSCLK: System Clock selected as LPUART1 clock
  *            @arg RCC_LPUART1CLKSOURCE_LSE: LSE selected as LPUART1 clock
  * @retval None
  */
#define __HAL_RCC_LPUART1_CONFIG(__LPUART1_CLKSOURCE__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_LPUART1SEL, (uint32_t)(__LPUART1_CLKSOURCE__))

/** @brief  Macro to get the LPUART1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_LPUART1CLKSOURCE_PCLK1: PCLK1 selected as LPUART1 clock
  *            @arg RCC_LPUART1CLKSOURCE_HSI: HSI selected as LPUART1 clock
  *            @arg RCC_LPUART1CLKSOURCE_SYSCLK: System Clock selected as LPUART1 clock
  *            @arg RCC_LPUART1CLKSOURCE_LSE: LSE selected as LPUART1 clock
  */
#define __HAL_RCC_GET_LPUART1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_LPUART1SEL)))

/** @brief  Macro to configure the LPTIM1 clock (LPTIM1CLK).
  *
  * @param  __LPTIM1_CLKSOURCE__: specifies the LPTIM1 clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_LPTIM1CLKSOURCE_PCLK: PCLK selected as LPTIM1 clock
  *            @arg RCC_LPTIM1CLKSOURCE_LSI : HSI  selected as LPTIM1 clock
  *            @arg RCC_LPTIM1CLKSOURCE_HSI : LSI  selected as LPTIM1 clock
  *            @arg RCC_LPTIM1CLKSOURCE_LSE : LSE  selected as LPTIM1 clock
  * @retval None
  */
#define __HAL_RCC_LPTIM1_CONFIG(__LPTIM1_CLKSOURCE__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_LPTIM1SEL, (uint32_t)(__LPTIM1_CLKSOURCE__))

/** @brief  Macro to get the LPTIM1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_LPTIM1CLKSOURCE_PCLK: PCLK selected as LPUART1 clock
  *            @arg RCC_LPTIM1CLKSOURCE_LSI : HSI selected as LPUART1 clock
  *            @arg RCC_LPTIM1CLKSOURCE_HSI : System Clock selected as LPUART1 clock
  *            @arg RCC_LPTIM1CLKSOURCE_LSE : LSE selected as LPUART1 clock
  */
#define __HAL_RCC_GET_LPTIM1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_LPTIM1SEL)))

/** @brief  Macro to configure the LPTIM2 clock (LPTIM2CLK).
  *
  * @param  __LPTIM2_CLKSOURCE__: specifies the LPTIM2 clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_LPTIM2CLKSOURCE_PCLK: PCLK selected as LPTIM2 clock
  *            @arg RCC_LPTIM2CLKSOURCE_LSI : HSI  selected as LPTIM2 clock
  *            @arg RCC_LPTIM2CLKSOURCE_HSI : LSI  selected as LPTIM2 clock
  *            @arg RCC_LPTIM2CLKSOURCE_LSE : LSE  selected as LPTIM2 clock
  * @retval None
  */
#define __HAL_RCC_LPTIM2_CONFIG(__LPTIM2_CLKSOURCE__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_LPTIM2SEL, (uint32_t)(__LPTIM2_CLKSOURCE__))

/** @brief  Macro to get the LPTIM2 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_LPTIM2CLKSOURCE_PCLK: PCLK selected as LPUART1 clock
  *            @arg RCC_LPTIM2CLKSOURCE_LSI : HSI selected as LPUART1 clock
  *            @arg RCC_LPTIM2CLKSOURCE_HSI : System Clock selected as LPUART1 clock
  *            @arg RCC_LPTIM2CLKSOURCE_LSE : LSE selected as LPUART1 clock
  */
#define __HAL_RCC_GET_LPTIM2_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_LPTIM2SEL)))

/** @brief  Macro to configure the SDMMC1 clock.
  *
  * @note  USB, RNG and SDMMC1 peripherals share the same 48MHz clock source.
  *
  * @param  __SDMMC1_CLKSOURCE__: specifies the SDMMC1 clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_SDMMC1CLKSOURCE_NONE:  No clock selected as SDMMC1 clock
  *            @arg RCC_SDMMC1CLKSOURCE_MSI:  MSI selected as SDMMC1 clock
  *            @arg RCC_SDMMC1CLKSOURCE_PLLSAI1: PLLSAI1 Clock selected as SDMMC1 clock
  *            @arg RCC_SDMMC1CLKSOURCE_PLL: PLL Clock selected as SDMMC1 clock
  * @retval None
  */
#define __HAL_RCC_SDMMC1_CONFIG(__SDMMC1_CLKSOURCE__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_CLK48SEL, (uint32_t)(__SDMMC1_CLKSOURCE__))

/** @brief  Macro to get the SDMMC1 clock.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_SDMMC1CLKSOURCE_NONE:  No clock selected as SDMMC1 clock
  *            @arg RCC_SDMMC1CLKSOURCE_MSI:  MSI selected as SDMMC1 clock
  *            @arg RCC_SDMMC1CLKSOURCE_PLLSAI1: PLLSAI1 "Q" clock (PLL48M2CLK) selected as SDMMC1 clock
  *            @arg RCC_SDMMC1CLKSOURCE_PLL: PLL "Q" clock (PLL48M1CLK) selected as SDMMC1 clock
  */
#define __HAL_RCC_GET_SDMMC1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_CLK48SEL)))

/** @brief  Macro to configure the RNG clock.
  *
  * @note  USB, RNG and SDMMC1 peripherals share the same 48MHz clock source.
  *
  * @param  __RNG_CLKSOURCE__: specifies the RNG clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_RNGCLKSOURCE_NONE:  No clock selected as RNG clock
  *            @arg RCC_RNGCLKSOURCE_MSI:  MSI selected as RNG clock
  *            @arg RCC_RNGCLKSOURCE_PLLSAI1: PLLSAI1 Clock selected as RNG clock
  *            @arg RCC_RNGCLKSOURCE_PLL: PLL Clock selected as RNG clock
  * @retval None
  */
#define __HAL_RCC_RNG_CONFIG(__RNG_CLKSOURCE__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_CLK48SEL, (uint32_t)(__RNG_CLKSOURCE__))

/** @brief  Macro to get the RNG clock.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_RNGCLKSOURCE_NONE:  No clock selected as RNG clock
  *            @arg RCC_RNGCLKSOURCE_MSI:  MSI selected as RNG clock
  *            @arg RCC_RNGCLKSOURCE_PLLSAI1: PLLSAI1 "Q" clock (PLL48M2CLK) selected as RNG clock
  *            @arg RCC_RNGCLKSOURCE_PLL: PLL "Q" clock (PLL48M1CLK) selected as RNG clock
  */
#define __HAL_RCC_GET_RNG_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_CLK48SEL)))

#if defined(STM32L475xx) || defined(STM32L476xx) || defined(STM32L485xx) || defined(STM32L486xx)
/** @brief  Macro to configure the USB clock (USBCLK).
  *
  * @note  USB, RNG and SDMMC1 peripherals share the same 48MHz clock source.
  *
  * @param  __USB_CLKSOURCE__: specifies the USB clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_USBCLKSOURCE_NONE:  No clock selected as 48MHz clock
  *            @arg RCC_USBCLKSOURCE_MSI:  MSI selected as USB clock
  *            @arg RCC_USBCLKSOURCE_PLLSAI1: PLLSAI1 "Q" clock (PLL48M2CLK) selected as USB clock
  *            @arg RCC_USBCLKSOURCE_PLL: PLL "Q" clock (PLL48M1CLK) selected as USB clock
  * @retval None
  */
#define __HAL_RCC_USB_CONFIG(__USB_CLKSOURCE__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_CLK48SEL, (uint32_t)(__USB_CLKSOURCE__))

/** @brief  Macro to get the USB clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_USBCLKSOURCE_NONE:  No clock selected as 48MHz clock
  *            @arg RCC_USBCLKSOURCE_MSI:  MSI selected as USB clock
  *            @arg RCC_USBCLKSOURCE_PLLSAI1: PLLSAI1 "Q" clock (PLL48M2CLK) selected as USB clock
  *            @arg RCC_USBCLKSOURCE_PLL: PLL "Q" clock (PLL48M1CLK) selected as USB clock
  */
#define __HAL_RCC_GET_USB_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_CLK48SEL)))

#endif /* STM32L475xx || STM32L476xx || STM32L485xx || STM32L486xx */

/** @brief  Macro to configure the ADC interface clock.
  * @param  __ADC_CLKSOURCE__: specifies the ADC digital interface clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_ADCCLKSOURCE_PLLSAI1:  PLLSAI1 Clock selected as ADC clock
  *            @arg RCC_ADCCLKSOURCE_PLLSAI2: PLLSAI2 Clock selected as ADC clock
  *            @arg RCC_ADCCLKSOURCE_SYSCLK: System Clock selected as ADC clock
  * @retval None
  */
#define __HAL_RCC_ADC_CONFIG(__ADC_CLKSOURCE__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_ADCSEL, (uint32_t)(__ADC_CLKSOURCE__))

/** @brief  Macro to get the ADC clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_ADCCLKSOURCE_PLLSAI1:  PLLSAI1 Clock selected as ADC clock
  *            @arg RCC_ADCCLKSOURCE_PLLSAI2: PLLSAI2 Clock selected as ADC clock
  *            @arg RCC_ADCCLKSOURCE_SYSCLK: System Clock selected as ADC clock
  */
#define __HAL_RCC_GET_ADC_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_ADCSEL)))

 /** @brief  Macro to configure the SWPMI1 clock.
   * @param  __SWPMI1_CLKSOURCE__: specifies the SWPMI1 clock source.
   *         This parameter can be one of the following values:
   *            @arg RCC_SWPMI1CLKSOURCE_PCLK:  PCLK Clock selected as SWPMI1 clock
   *            @arg RCC_SWPMI1CLKSOURCE_HSI: HSI Clock selected as SWPMI1 clock
   * @retval None
   */
#define __HAL_RCC_SWPMI1_CONFIG(__SWPMI1_CLKSOURCE__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_SWPMI1SEL, (uint32_t)(__SWPMI1_CLKSOURCE__))

/** @brief  Macro to get the SWPMI1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_SWPMI1CLKSOURCE_PCLK:  PCLK Clock selected as SWPMI1 clock
  *            @arg RCC_SWPMI1CLKSOURCE_HSI:   HSI Clock selected as SWPMI1 clock
  */
#define __HAL_RCC_GET_SWPMI1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_SWPMI1SEL)))

 /** @brief  Macro to configure the DFSDM clock.
  * @param  __DFSDM_CLKSOURCE__: specifies the DFSDM clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_DFSDMCLKSOURCE_PCLK:  PCLK Clock selected as DFSDM clock
  *            @arg RCC_DFSDMCLKSOURCE_HSI:   HSI Clock selected as DFSDM clock
  * @retval None
  */
#define __HAL_RCC_DFSDM_CONFIG(__DFSDM_CLKSOURCE__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_DFSDMSEL, (uint32_t)(__DFSDM_CLKSOURCE__))

/** @brief  Macro to get the DFSDM clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_DFSDMCLKSOURCE_PCLK:  PCLK Clock selected as DFSDM clock
  *            @arg RCC_DFSDMCLKSOURCE_HSI:   HSI Clock selected as DFSDM clock
  */
#define __HAL_RCC_GET_DFSDM_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_DFSDMSEL)))


/** @defgroup RCCEx_Flags_Interrupts_Management Flags Interrupts Management
  * @brief macros to manage the specified RCC Flags and interrupts.
  * @{
  */

/** @brief Enable PLLSAI1RDY interrupt.
  * @retval None
  */
#define __HAL_RCC_PLLSAI1_ENABLE_IT()  SET_BIT(RCC->CIER, RCC_CIER_PLLSAI1RDYIE)

/** @brief Disable PLLSAI1RDY interrupt.
  * @retval None
  */
#define __HAL_RCC_PLLSAI1_DISABLE_IT() CLEAR_BIT(RCC->CIER, RCC_CIER_PLLSAI1RDYIE)

/** @brief Clear the PLLSAI1RDY interrupt pending bit.
  * @retval None
  */
#define __HAL_RCC_PLLSAI1_CLEAR_IT()   WRITE_REG(RCC->CICR, RCC_CICR_PLLSAI1RDYC)

/** @brief Check whether PLLSAI1RDY interrupt has occurred or not.
  * @retval TRUE or FALSE.
  */
#define __HAL_RCC_PLLSAI1_GET_IT_SOURCE()     (READ_BIT(RCC->CIFR, RCC_CIFR_PLLSAI1RDYF) == RCC_CIFR_PLLSAI1RDYF)

/** @brief  Check whether the PLLSAI1RDY flag is set or not.
  * @retval TRUE or FALSE.
  */
#define __HAL_RCC_PLLSAI1_GET_FLAG()   (READ_BIT(RCC->CR, RCC_CR_PLLSAI1RDY) == (RCC_CR_PLLSAI1RDY))

/** @brief Enable PLLSAI2RDY interrupt.
  * @retval None
  */
#define __HAL_RCC_PLLSAI2_ENABLE_IT()  SET_BIT(RCC->CIER, RCC_CIER_PLLSAI2RDYIE)

/** @brief Disable PLLSAI2RDY interrupt.
  * @retval None
  */
#define __HAL_RCC_PLLSAI2_DISABLE_IT() CLEAR_BIT(RCC->CIER, RCC_CIER_PLLSAI2RDYIE)

/** @brief Clear the PLLSAI2RDY interrupt pending bit.
  * @retval None
  */
#define __HAL_RCC_PLLSAI2_CLEAR_IT()   WRITE_REG(RCC->CICR, RCC_CICR_PLLSAI2RDYC)

/** @brief Check whether the PLLSAI2RDY interrupt has occurred or not.
  * @retval TRUE or FALSE.
  */
#define __HAL_RCC_PLLSAI2_GET_IT_SOURCE()     (READ_BIT(RCC->CIFR, RCC_CIFR_PLLSAI2RDYF) == RCC_CIFR_PLLSAI2RDYF)

/** @brief  Check whether the PLLSAI2RDY flag is set or not.
  * @retval TRUE or FALSE.
  */
#define __HAL_RCC_PLLSAI2_GET_FLAG()   (READ_BIT(RCC->CR, RCC_CR_PLLSAI2RDY) == (RCC_CR_PLLSAI2RDY))

/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup RCCEx_Exported_Functions
  * @{
  */

/** @addtogroup RCCEx_Exported_Functions_Group1
  * @{
  */

HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit);
void              HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit);
uint32_t          HAL_RCCEx_GetPeriphCLKFreq(uint32_t PeriphClk);

/**
  * @}
  */

/** @addtogroup RCCEx_Exported_Functions_Group2
  * @{
  */

HAL_StatusTypeDef HAL_RCCEx_EnablePLLSAI1(RCC_PLLSAI1InitTypeDef  *PLLSAI1Init);
HAL_StatusTypeDef HAL_RCCEx_DisablePLLSAI1(void);
HAL_StatusTypeDef HAL_RCCEx_EnablePLLSAI2(RCC_PLLSAI2InitTypeDef  *PLLSAI2Init);
HAL_StatusTypeDef HAL_RCCEx_DisablePLLSAI2(void);
void              HAL_RCCEx_WakeUpStopCLKConfig(uint32_t WakeUpClk);
void              HAL_RCCEx_StandbyMSIRangeConfig(uint32_t MSIRange);
void              HAL_RCCEx_EnableLSECSS(void);
void              HAL_RCCEx_DisableLSECSS(void);
void              HAL_RCCEx_EnableLSCO(uint32_t LSCOSource);
void              HAL_RCCEx_DisableLSCO(void);
void              HAL_RCCEx_EnableMSIPLLMode(void);
void              HAL_RCCEx_DisableMSIPLLMode(void);

/**
  * @}
  */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @addtogroup RCCEx_Private_Macros
  * @{
  */

#define IS_RCC_LSCOSOURCE(__SOURCE__) (((__SOURCE__) == RCC_LSCOSOURCE_LSI) || \
                                       ((__SOURCE__) == RCC_LSCOSOURCE_LSE))

#if defined(STM32L471xx)

#define IS_RCC_PERIPHCLOCK(__SELECTION__)  \
               ((((__SELECTION__) & RCC_PERIPHCLK_USART1)  == RCC_PERIPHCLK_USART1)  || \
                (((__SELECTION__) & RCC_PERIPHCLK_USART2)  == RCC_PERIPHCLK_USART2)  || \
                (((__SELECTION__) & RCC_PERIPHCLK_USART3)  == RCC_PERIPHCLK_USART3)  || \
                (((__SELECTION__) & RCC_PERIPHCLK_UART4)   == RCC_PERIPHCLK_UART4)   || \
                (((__SELECTION__) & RCC_PERIPHCLK_UART5)   == RCC_PERIPHCLK_UART5)   || \
                (((__SELECTION__) & RCC_PERIPHCLK_LPUART1) == RCC_PERIPHCLK_LPUART1) || \
                (((__SELECTION__) & RCC_PERIPHCLK_I2C1)    == RCC_PERIPHCLK_I2C1)    || \
                (((__SELECTION__) & RCC_PERIPHCLK_I2C2)    == RCC_PERIPHCLK_I2C2)    || \
                (((__SELECTION__) & RCC_PERIPHCLK_I2C3)    == RCC_PERIPHCLK_I2C3)    || \
                (((__SELECTION__) & RCC_PERIPHCLK_LPTIM1)  == RCC_PERIPHCLK_LPTIM1)  || \
                (((__SELECTION__) & RCC_PERIPHCLK_LPTIM2)  == RCC_PERIPHCLK_LPTIM2)  || \
                (((__SELECTION__) & RCC_PERIPHCLK_SAI1)    == RCC_PERIPHCLK_SAI1)    || \
                (((__SELECTION__) & RCC_PERIPHCLK_SAI2)    == RCC_PERIPHCLK_SAI2)    || \
                (((__SELECTION__) & RCC_PERIPHCLK_ADC)     == RCC_PERIPHCLK_ADC)     || \
                (((__SELECTION__) & RCC_PERIPHCLK_SWPMI1)  == RCC_PERIPHCLK_SWPMI1)  || \
                (((__SELECTION__) & RCC_PERIPHCLK_DFSDM)   == RCC_PERIPHCLK_DFSDM)   || \
                (((__SELECTION__) & RCC_PERIPHCLK_RTC)     == RCC_PERIPHCLK_RTC)     || \
                (((__SELECTION__) & RCC_PERIPHCLK_RNG)     == RCC_PERIPHCLK_RNG)     || \
                (((__SELECTION__) & RCC_PERIPHCLK_SDMMC1)  == RCC_PERIPHCLK_SDMMC1))

#else /* STM32L475xx || STM32L476xx || STM32L485xx || STM32L486xx */

#define IS_RCC_PERIPHCLOCK(__SELECTION__)  \
               ((((__SELECTION__) & RCC_PERIPHCLK_USART1)  == RCC_PERIPHCLK_USART1)  || \
                (((__SELECTION__) & RCC_PERIPHCLK_USART2)  == RCC_PERIPHCLK_USART2)  || \
                (((__SELECTION__) & RCC_PERIPHCLK_USART3)  == RCC_PERIPHCLK_USART3)  || \
                (((__SELECTION__) & RCC_PERIPHCLK_UART4)   == RCC_PERIPHCLK_UART4)   || \
                (((__SELECTION__) & RCC_PERIPHCLK_UART5)   == RCC_PERIPHCLK_UART5)   || \
                (((__SELECTION__) & RCC_PERIPHCLK_LPUART1) == RCC_PERIPHCLK_LPUART1) || \
                (((__SELECTION__) & RCC_PERIPHCLK_I2C1)    == RCC_PERIPHCLK_I2C1)    || \
                (((__SELECTION__) & RCC_PERIPHCLK_I2C2)    == RCC_PERIPHCLK_I2C2)    || \
                (((__SELECTION__) & RCC_PERIPHCLK_I2C3)    == RCC_PERIPHCLK_I2C3)    || \
                (((__SELECTION__) & RCC_PERIPHCLK_LPTIM1)  == RCC_PERIPHCLK_LPTIM1)  || \
                (((__SELECTION__) & RCC_PERIPHCLK_LPTIM2)  == RCC_PERIPHCLK_LPTIM2)  || \
                (((__SELECTION__) & RCC_PERIPHCLK_SAI1)    == RCC_PERIPHCLK_SAI1)    || \
                (((__SELECTION__) & RCC_PERIPHCLK_SAI2)    == RCC_PERIPHCLK_SAI2)    || \
                (((__SELECTION__) & RCC_PERIPHCLK_USB)     == RCC_PERIPHCLK_USB)     || \
                (((__SELECTION__) & RCC_PERIPHCLK_ADC)     == RCC_PERIPHCLK_ADC)     || \
                (((__SELECTION__) & RCC_PERIPHCLK_SWPMI1)  == RCC_PERIPHCLK_SWPMI1)  || \
                (((__SELECTION__) & RCC_PERIPHCLK_DFSDM)   == RCC_PERIPHCLK_DFSDM)   || \
                (((__SELECTION__) & RCC_PERIPHCLK_RTC)     == RCC_PERIPHCLK_RTC)     || \
                (((__SELECTION__) & RCC_PERIPHCLK_RNG)     == RCC_PERIPHCLK_RNG)     || \
                (((__SELECTION__) & RCC_PERIPHCLK_SDMMC1)  == RCC_PERIPHCLK_SDMMC1))

#endif /* STM32L471xx */

#define IS_RCC_USART1CLKSOURCE(__SOURCE__)  \
               (((__SOURCE__) == RCC_USART1CLKSOURCE_PCLK2)  || \
                ((__SOURCE__) == RCC_USART1CLKSOURCE_SYSCLK) || \
                ((__SOURCE__) == RCC_USART1CLKSOURCE_LSE)    || \
                ((__SOURCE__) == RCC_USART1CLKSOURCE_HSI))

#define IS_RCC_USART2CLKSOURCE(__SOURCE__)  \
               (((__SOURCE__) == RCC_USART2CLKSOURCE_PCLK1)  || \
                ((__SOURCE__) == RCC_USART2CLKSOURCE_SYSCLK) || \
                ((__SOURCE__) == RCC_USART2CLKSOURCE_LSE)    || \
                ((__SOURCE__) == RCC_USART2CLKSOURCE_HSI))

#define IS_RCC_USART3CLKSOURCE(__SOURCE__)  \
               (((__SOURCE__) == RCC_USART3CLKSOURCE_PCLK1)  || \
                ((__SOURCE__) == RCC_USART3CLKSOURCE_SYSCLK) || \
                ((__SOURCE__) == RCC_USART3CLKSOURCE_LSE)    || \
                ((__SOURCE__) == RCC_USART3CLKSOURCE_HSI))

#define IS_RCC_UART4CLKSOURCE(__SOURCE__)  \
               (((__SOURCE__) == RCC_UART4CLKSOURCE_PCLK1)  || \
                ((__SOURCE__) == RCC_UART4CLKSOURCE_SYSCLK) || \
                ((__SOURCE__) == RCC_UART4CLKSOURCE_LSE)    || \
                ((__SOURCE__) == RCC_UART4CLKSOURCE_HSI))

#define IS_RCC_UART5CLKSOURCE(__SOURCE__)  \
               (((__SOURCE__) == RCC_UART5CLKSOURCE_PCLK1)  || \
                ((__SOURCE__) == RCC_UART5CLKSOURCE_SYSCLK) || \
                ((__SOURCE__) == RCC_UART5CLKSOURCE_LSE)    || \
                ((__SOURCE__) == RCC_UART5CLKSOURCE_HSI))

#define IS_RCC_LPUART1CLKSOURCE(__SOURCE__)  \
               (((__SOURCE__) == RCC_LPUART1CLKSOURCE_PCLK1)  || \
                ((__SOURCE__) == RCC_LPUART1CLKSOURCE_SYSCLK) || \
                ((__SOURCE__) == RCC_LPUART1CLKSOURCE_LSE)    || \
                ((__SOURCE__) == RCC_LPUART1CLKSOURCE_HSI))

#define IS_RCC_I2C1CLKSOURCE(__SOURCE__)   \
               (((__SOURCE__) == RCC_I2C1CLKSOURCE_PCLK1) || \
                ((__SOURCE__) == RCC_I2C1CLKSOURCE_SYSCLK)|| \
                ((__SOURCE__) == RCC_I2C1CLKSOURCE_HSI))

#define IS_RCC_I2C2CLKSOURCE(__SOURCE__)   \
               (((__SOURCE__) == RCC_I2C2CLKSOURCE_PCLK1) || \
                ((__SOURCE__) == RCC_I2C2CLKSOURCE_SYSCLK)|| \
                ((__SOURCE__) == RCC_I2C2CLKSOURCE_HSI))

#define IS_RCC_I2C3CLKSOURCE(__SOURCE__)   \
               (((__SOURCE__) == RCC_I2C3CLKSOURCE_PCLK1) || \
                ((__SOURCE__) == RCC_I2C3CLKSOURCE_SYSCLK)|| \
                ((__SOURCE__) == RCC_I2C3CLKSOURCE_HSI))

#define IS_RCC_SAI1CLK(__SOURCE__)   \
               (((__SOURCE__) == RCC_SAI1CLKSOURCE_PLLSAI1) || \
                ((__SOURCE__) == RCC_SAI1CLKSOURCE_PLLSAI2) || \
                ((__SOURCE__) == RCC_SAI1CLKSOURCE_PLL)     || \
                ((__SOURCE__) == RCC_SAI1CLKSOURCE_PIN))

#define IS_RCC_SAI2CLK(__SOURCE__)   \
               (((__SOURCE__) == RCC_SAI2CLKSOURCE_PLLSAI2) || \
                ((__SOURCE__) == RCC_SAI2CLKSOURCE_PLLSAI2) || \
                ((__SOURCE__) == RCC_SAI2CLKSOURCE_PLL)     || \
                ((__SOURCE__) == RCC_SAI2CLKSOURCE_PIN))

#define IS_RCC_LPTIM1CLK(__SOURCE__)  \
               (((__SOURCE__) == RCC_LPTIM1CLKSOURCE_PCLK) || \
                ((__SOURCE__) == RCC_LPTIM1CLKSOURCE_LSI)  || \
                ((__SOURCE__) == RCC_LPTIM1CLKSOURCE_HSI)  || \
                ((__SOURCE__) == RCC_LPTIM1CLKSOURCE_LSE))

#define IS_RCC_LPTIM2CLK(__SOURCE__)  \
               (((__SOURCE__) == RCC_LPTIM2CLKSOURCE_PCLK) || \
                ((__SOURCE__) == RCC_LPTIM2CLKSOURCE_LSI)  || \
                ((__SOURCE__) == RCC_LPTIM2CLKSOURCE_HSI)  || \
                ((__SOURCE__) == RCC_LPTIM2CLKSOURCE_LSE))

#define IS_RCC_SDMMC1CLKSOURCE(__SOURCE__)  \
               (((__SOURCE__) == RCC_SDMMC1CLKSOURCE_NONE)    || \
                ((__SOURCE__) == RCC_SDMMC1CLKSOURCE_PLLSAI1) || \
                ((__SOURCE__) == RCC_SDMMC1CLKSOURCE_PLL)     || \
                ((__SOURCE__) == RCC_SDMMC1CLKSOURCE_MSI))

#define IS_RCC_RNGCLKSOURCE(__SOURCE__)  \
               (((__SOURCE__) == RCC_RNGCLKSOURCE_NONE)    || \
                ((__SOURCE__) == RCC_RNGCLKSOURCE_PLLSAI1) || \
                ((__SOURCE__) == RCC_RNGCLKSOURCE_PLL)     || \
                ((__SOURCE__) == RCC_RNGCLKSOURCE_MSI))

#if defined(STM32L475xx) || defined(STM32L476xx) || defined(STM32L485xx) || defined(STM32L486xx)
#define IS_RCC_USBCLKSOURCE(__SOURCE__)  \
               (((__SOURCE__) == RCC_USBCLKSOURCE_NONE)    || \
                ((__SOURCE__) == RCC_USBCLKSOURCE_PLLSAI1) || \
                ((__SOURCE__) == RCC_USBCLKSOURCE_PLL)     || \
                ((__SOURCE__) == RCC_USBCLKSOURCE_MSI))
#endif /* STM32L475xx || STM32L476xx || STM32L485xx || STM32L486xx */

#define IS_RCC_ADCCLKSOURCE(__SOURCE__)  \
               (((__SOURCE__) == RCC_ADCCLKSOURCE_NONE)    || \
                ((__SOURCE__) == RCC_ADCCLKSOURCE_PLLSAI1) || \
                ((__SOURCE__) == RCC_ADCCLKSOURCE_PLLSAI2) || \
                ((__SOURCE__) == RCC_ADCCLKSOURCE_SYSCLK))

#define IS_RCC_SWPMI1CLKSOURCE(__SOURCE__)  \
               (((__SOURCE__) == RCC_SWPMI1CLKSOURCE_PCLK) || \
                ((__SOURCE__) == RCC_SWPMI1CLKSOURCE_HSI))

#define IS_RCC_DFSDMCLKSOURCE(__SOURCE__)  \
               (((__SOURCE__) == RCC_DFSDMCLKSOURCE_PCLK) || \
                ((__SOURCE__) == RCC_DFSDMCLKSOURCE_SYSCLK))


#define IS_RCC_PLLSAI1N_VALUE(__VALUE__)   ((8 <= (__VALUE__)) && ((__VALUE__) <= 86))

#define IS_RCC_PLLSAI1P_VALUE(__VALUE__)   (((__VALUE__) == 7) || ((__VALUE__) == 17))

#define IS_RCC_PLLSAI1Q_VALUE(__VALUE__)   (((__VALUE__) == 2 ) || ((__VALUE__) == 4) || \
                                            ((__VALUE__) == 6)  || ((__VALUE__) == 8))

#define IS_RCC_PLLSAI1R_VALUE(__VALUE__)   (((__VALUE__) == 2 ) || ((__VALUE__) == 4) || \
                                            ((__VALUE__) == 6)  || ((__VALUE__) == 8))

#define IS_RCC_PLLSAI2N_VALUE(__VALUE__)   ((8 <= (__VALUE__)) && ((__VALUE__) <= 86))

#define IS_RCC_PLLSAI2P_VALUE(__VALUE__)   (((__VALUE__) == 7) || ((__VALUE__) == 17))

#define IS_RCC_PLLSAI2R_VALUE(__VALUE__)   (((__VALUE__) == 2 ) || ((__VALUE__) == 4) || \
                                            ((__VALUE__) == 6)  || ((__VALUE__) == 8))

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

#endif /* __STM32L4xx_HAL_RCC_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
