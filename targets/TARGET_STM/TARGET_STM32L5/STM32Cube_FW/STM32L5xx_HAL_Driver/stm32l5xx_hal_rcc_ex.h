/**
  ******************************************************************************
  * @file    stm32l5xx_hal_rcc_ex.h
  * @author  MCD Application Team
  * @brief   Header file of RCC HAL Extended module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#ifndef STM32L5xx_HAL_RCC_EX_H
#define STM32L5xx_HAL_RCC_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l5xx_hal_def.h"

/** @addtogroup STM32L5xx_HAL_Driver
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

  uint32_t PLLSAI1Source;    /*!< PLLSAI1Source: PLLSAI1 entry clock source.
                                  This parameter must be a value of @ref RCC_PLLSAI1_Clock_Source */

  uint32_t PLLSAI1M;         /*!< PLLSAI1M: specifies the division factor for PLLSAI1 input clock.
                                  This parameter must be a number between Min_Data = 1 and Max_Data = 16 */

  uint32_t PLLSAI1N;         /*!< PLLSAI1N: specifies the multiplication factor for PLLSAI1 VCO output clock.
                                  This parameter must be a number between 8 and 86 or 127 depending on devices. */

  uint32_t PLLSAI1P;         /*!< PLLSAI1P: specifies the division factor for SAI clock.
                                  This parameter must be a value of @ref RCC_PLLP_Clock_Divider */

  uint32_t PLLSAI1Q;         /*!< PLLSAI1Q: specifies the division factor for USB/RNG/SDMMC1 clock.
                                  This parameter must be a value of @ref RCC_PLLQ_Clock_Divider */

  uint32_t PLLSAI1R;         /*!< PLLSAI1R: specifies the division factor for ADC clock.
                                  This parameter must be a value of @ref RCC_PLLR_Clock_Divider */

  uint32_t PLLSAI1ClockOut;  /*!< PLLSAIClockOut: specifies PLLSAI1 output clock to be enabled.
                                  This parameter must be a value of @ref RCC_PLLSAI1_Clock_Output */
} RCC_PLLSAI1InitTypeDef;

/**
  * @brief  PLLSAI2 Clock structure definition
  */
typedef struct
{

  uint32_t PLLSAI2Source;    /*!< PLLSAI2Source: PLLSAI2 entry clock source.
                                  This parameter must be a value of @ref RCC_PLLSAI2_Clock_Source */

  uint32_t PLLSAI2M;         /*!< PLLSAI2M: specifies the division factor for PLLSAI2 input clock.
                                  This parameter must be a number between Min_Data = 1 and Max_Data = 16 */

  uint32_t PLLSAI2N;         /*!< PLLSAI2N: specifies the multiplication factor for PLLSAI2 VCO output clock.
                                  This parameter must be a number between 8 and 86 or 127 depending on devices. */

  uint32_t PLLSAI2P;         /*!< PLLSAI2P: specifies the division factor for SAI clock.
                                  This parameter must be a value of @ref RCC_PLLP_Clock_Divider */

  uint32_t PLLSAI2ClockOut;  /*!< PLLSAIClockOut: specifies PLLSAI2 output clock to be enabled.
                                  This parameter must be a value of @ref RCC_PLLSAI2_Clock_Output */
} RCC_PLLSAI2InitTypeDef;

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

  uint32_t I2c4ClockSelection;     /*!< Specifies I2C4 clock source.
                                        This parameter can be a value of @ref RCCEx_I2C4_Clock_Source */

  uint32_t Lptim1ClockSelection;   /*!< Specifies LPTIM1 clock source.
                                        This parameter can be a value of @ref RCCEx_LPTIM1_Clock_Source */

  uint32_t Lptim2ClockSelection;   /*!< Specifies LPTIM2 clock source.
                                        This parameter can be a value of @ref RCCEx_LPTIM2_Clock_Source */

  uint32_t Lptim3ClockSelection;   /*!< Specifies LPTIM3 clock source.
                                        This parameter can be a value of @ref RCCEx_LPTIM3_Clock_Source */

  uint32_t FdcanClockSelection;     /*!< Specifies FDCAN kernel clock source.
                                        This parameter can be a value of @ref RCCEx_FDCAN_Clock_Source */

  uint32_t Sai1ClockSelection;     /*!< Specifies SAI1 clock source.
                                        This parameter can be a value of @ref RCCEx_SAI1_Clock_Source */

  uint32_t Sai2ClockSelection;     /*!< Specifies SAI2 clock source.
                                        This parameter can be a value of @ref RCCEx_SAI2_Clock_Source */

#if defined(USB)

  uint32_t UsbClockSelection;      /*!< Specifies USB clock source (warning: same source for SDMMC1 and RNG).
                                        This parameter can be a value of @ref RCCEx_USB_Clock_Source */

#endif /* USB */

  uint32_t Sdmmc1ClockSelection;   /*!< Specifies SDMMC1 clock source (warning: same source for USB and RNG).
                                        This parameter can be a value of @ref RCCEx_SDMMC1_Clock_Source */

  uint32_t RngClockSelection;      /*!< Specifies RNG clock source (warning: same source for USB and SDMMC1).
                                        This parameter can be a value of @ref RCCEx_RNG_Clock_Source */

  uint32_t AdcClockSelection;      /*!< Specifies ADC interface clock source.
                                        This parameter can be a value of @ref RCCEx_ADC_Clock_Source */

  uint32_t Dfsdm1ClockSelection;   /*!< Specifies DFSDM1 clock source.
                                        This parameter can be a value of @ref RCCEx_DFSDM1_Clock_Source */

  uint32_t Dfsdm1AudioClockSelection; /*!< Specifies DFSDM1 audio clock source.
                                        This parameter can be a value of @ref RCCEx_DFSDM1_Audio_Clock_Source */

  uint32_t OspiClockSelection;     /*!< Specifies OctoSPI clock source.
                                        This parameter can be a value of @ref RCCEx_OSPI_Clock_Source */

  uint32_t RTCClockSelection;      /*!< Specifies RTC clock source.
                                        This parameter can be a value of @ref RCC_RTC_Clock_Source */
} RCC_PeriphCLKInitTypeDef;


#if defined(CRS)

/**
  * @brief RCC_CRS Init structure definition
  */
typedef struct
{
  uint32_t Prescaler;             /*!< Specifies the division factor of the SYNC signal.
                                     This parameter can be a value of @ref RCCEx_CRS_SynchroDivider */

  uint32_t Source;                /*!< Specifies the SYNC signal source.
                                     This parameter can be a value of @ref RCCEx_CRS_SynchroSource */

  uint32_t Polarity;              /*!< Specifies the input polarity for the SYNC signal source.
                                     This parameter can be a value of @ref RCCEx_CRS_SynchroPolarity */

  uint32_t ReloadValue;           /*!< Specifies the value to be loaded in the frequency error counter with each SYNC event.
                                      It can be calculated in using macro __HAL_RCC_CRS_RELOADVALUE_CALCULATE(__FTARGET__, __FSYNC__)
                                     This parameter must be a number between 0 and 0xFFFF or a value of @ref RCCEx_CRS_ReloadValueDefault .*/

  uint32_t ErrorLimitValue;       /*!< Specifies the value to be used to evaluate the captured frequency error value.
                                     This parameter must be a number between 0 and 0xFF or a value of @ref RCCEx_CRS_ErrorLimitDefault */

  uint32_t HSI48CalibrationValue; /*!< Specifies a user-programmable trimming value to the HSI48 oscillator.
                                     This parameter must be a number between 0 and 0x7F or a value of @ref RCCEx_CRS_HSI48CalibrationDefault */

} RCC_CRSInitTypeDef;

/**
  * @brief RCC_CRS Synchronization structure definition
  */
typedef struct
{
  uint32_t ReloadValue;           /*!< Specifies the value loaded in the Counter reload value.
                                     This parameter must be a number between 0 and 0xFFFF */

  uint32_t HSI48CalibrationValue; /*!< Specifies value loaded in HSI48 oscillator smooth trimming.
                                     This parameter must be a number between 0 and 0x7F or a value of @ref RCCEx_CRS_HSI48CalibrationDefault */

  uint32_t FreqErrorCapture;      /*!< Specifies the value loaded in the .FECAP, the frequency error counter
                                                                    value latched in the time of the last SYNC event.
                                    This parameter must be a number between 0 and 0xFFFF */

  uint32_t FreqErrorDirection;    /*!< Specifies the value loaded in the .FEDIR, the counting direction of the
                                                                    frequency error counter latched in the time of the last SYNC event.
                                                                    It shows whether the actual frequency is below or above the target.
                                    This parameter must be a value of @ref RCCEx_CRS_FreqErrorDirection*/

} RCC_CRSSynchroInfoTypeDef;

#endif /* CRS */
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
#define RCC_LSCOSOURCE_LSI             0U                  /*!< LSI selection for low speed clock output */
#define RCC_LSCOSOURCE_LSE             RCC_BDCR_LSCOSEL    /*!< LSE selection for low speed clock output */
/**
  * @}
  */

/** @defgroup RCCEx_Periph_Clock_Selection Periph Clock Selection
  * @{
  */
#define RCC_PERIPHCLK_USART1           0x00000001U
#define RCC_PERIPHCLK_USART2           0x00000002U
#define RCC_PERIPHCLK_USART3           0x00000004U
#define RCC_PERIPHCLK_UART4            0x00000008U
#define RCC_PERIPHCLK_UART5            0x00000010U
#define RCC_PERIPHCLK_LPUART1          0x00000020U
#define RCC_PERIPHCLK_I2C1             0x00000040U
#define RCC_PERIPHCLK_I2C2             0x00000080U
#define RCC_PERIPHCLK_I2C3             0x00000100U
#define RCC_PERIPHCLK_LPTIM1           0x00000200U
#define RCC_PERIPHCLK_LPTIM2           0x00000400U
#define RCC_PERIPHCLK_SAI1             0x00000800U
#define RCC_PERIPHCLK_SAI2             0x00001000U
#if defined(USB)
#define RCC_PERIPHCLK_USB              0x00002000U
#endif /* USB */
#define RCC_PERIPHCLK_ADC              0x00004000U
#define RCC_PERIPHCLK_DFSDM1           0x00010000U
#define RCC_PERIPHCLK_RTC              0x00020000U
#define RCC_PERIPHCLK_RNG              0x00040000U
#define RCC_PERIPHCLK_SDMMC1           0x00080000U
#define RCC_PERIPHCLK_I2C4             0x00100000U
#define RCC_PERIPHCLK_DFSDM1AUDIO      0x00200000U
#define RCC_PERIPHCLK_LPTIM3           0x00400000U
#define RCC_PERIPHCLK_OSPI             0x01000000U
#define RCC_PERIPHCLK_FDCAN            0x02000000U
/**
  * @}
  */


/** @defgroup RCCEx_USART1_Clock_Source USART1 Clock Source
  * @{
  */
#define RCC_USART1CLKSOURCE_PCLK2      0U                     /*!< PCLK2 */
#define RCC_USART1CLKSOURCE_SYSCLK     RCC_CCIPR1_USART1SEL_0 /*!< System clock */
#define RCC_USART1CLKSOURCE_HSI        RCC_CCIPR1_USART1SEL_1 /*!< HSI */
#define RCC_USART1CLKSOURCE_LSE        (RCC_CCIPR1_USART1SEL_0 | RCC_CCIPR1_USART1SEL_1) /*!< LSE */
/**
  * @}
  */

/** @defgroup RCCEx_USART2_Clock_Source USART2 Clock Source
  * @{
  */
#define RCC_USART2CLKSOURCE_PCLK1      0U                     /*!< PCLK1 */
#define RCC_USART2CLKSOURCE_SYSCLK     RCC_CCIPR1_USART2SEL_0 /*!< System clock */
#define RCC_USART2CLKSOURCE_HSI        RCC_CCIPR1_USART2SEL_1 /*!< HSI */
#define RCC_USART2CLKSOURCE_LSE        (RCC_CCIPR1_USART2SEL_0 | RCC_CCIPR1_USART2SEL_1) /*!< LSE */
/**
  * @}
  */

/** @defgroup RCCEx_USART3_Clock_Source USART3 Clock Source
  * @{
  */
#define RCC_USART3CLKSOURCE_PCLK1      0U                     /*!< PCLK1 */
#define RCC_USART3CLKSOURCE_SYSCLK     RCC_CCIPR1_USART3SEL_0 /*!< System clock */
#define RCC_USART3CLKSOURCE_HSI        RCC_CCIPR1_USART3SEL_1 /*!< HSI */
#define RCC_USART3CLKSOURCE_LSE        (RCC_CCIPR1_USART3SEL_0 | RCC_CCIPR1_USART3SEL_1) /*!< LSE */
/**
  * @}
  */

/** @defgroup RCCEx_UART4_Clock_Source UART4 Clock Source
  * @{
  */
#define RCC_UART4CLKSOURCE_PCLK1       0U                    /*!< PCLK1 */
#define RCC_UART4CLKSOURCE_SYSCLK      RCC_CCIPR1_UART4SEL_0 /*!< System clock */
#define RCC_UART4CLKSOURCE_HSI         RCC_CCIPR1_UART4SEL_1 /*!< HSI */
#define RCC_UART4CLKSOURCE_LSE         (RCC_CCIPR1_UART4SEL_0 | RCC_CCIPR1_UART4SEL_1) /*!< LSE */
/**
  * @}
  */

/** @defgroup RCCEx_UART5_Clock_Source UART5 Clock Source
  * @{
  */
#define RCC_UART5CLKSOURCE_PCLK1       0U                    /*!< PCLK1 */
#define RCC_UART5CLKSOURCE_SYSCLK      RCC_CCIPR1_UART5SEL_0 /*!< System clock */
#define RCC_UART5CLKSOURCE_HSI         RCC_CCIPR1_UART5SEL_1 /*!< HSI */
#define RCC_UART5CLKSOURCE_LSE         (RCC_CCIPR1_UART5SEL_0 | RCC_CCIPR1_UART5SEL_1) /*!< LSE */
/**
  * @}
  */

/** @defgroup RCCEx_LPUART1_Clock_Source LPUART1 Clock Source
  * @{
  */
#define RCC_LPUART1CLKSOURCE_PCLK1     0U                      /*!< PCLK1 */
#define RCC_LPUART1CLKSOURCE_SYSCLK    RCC_CCIPR1_LPUART1SEL_0 /*!< System clock */
#define RCC_LPUART1CLKSOURCE_HSI       RCC_CCIPR1_LPUART1SEL_1 /*!< HSI */
#define RCC_LPUART1CLKSOURCE_LSE       (RCC_CCIPR1_LPUART1SEL_0 | RCC_CCIPR1_LPUART1SEL_1) /*!< LSE */
/**
  * @}
  */

/** @defgroup RCCEx_I2C1_Clock_Source I2C1 Clock Source
  * @{
  */
#define RCC_I2C1CLKSOURCE_PCLK1        0U                   /*!< PCLK1 */
#define RCC_I2C1CLKSOURCE_SYSCLK       RCC_CCIPR1_I2C1SEL_0 /*!< System clock */
#define RCC_I2C1CLKSOURCE_HSI          RCC_CCIPR1_I2C1SEL_1 /*!< HSI */
/**
  * @}
  */

/** @defgroup RCCEx_I2C2_Clock_Source I2C2 Clock Source
  * @{
  */
#define RCC_I2C2CLKSOURCE_PCLK1        0U                   /*!< PCLK1 */
#define RCC_I2C2CLKSOURCE_SYSCLK       RCC_CCIPR1_I2C2SEL_0 /*!< System clock */
#define RCC_I2C2CLKSOURCE_HSI          RCC_CCIPR1_I2C2SEL_1 /*!< HSI */
/**
  * @}
  */

/** @defgroup RCCEx_I2C3_Clock_Source I2C3 Clock Source
  * @{
  */
#define RCC_I2C3CLKSOURCE_PCLK1        0U                   /*!< PCLK1 */
#define RCC_I2C3CLKSOURCE_SYSCLK       RCC_CCIPR1_I2C3SEL_0 /*!< System clock */
#define RCC_I2C3CLKSOURCE_HSI          RCC_CCIPR1_I2C3SEL_1 /*!< HSI */
/**
  * @}
  */

/** @defgroup RCCEx_I2C4_Clock_Source I2C4 Clock Source
  * @{
  */
#define RCC_I2C4CLKSOURCE_PCLK1        0U                   /*!< PCLK1 */
#define RCC_I2C4CLKSOURCE_SYSCLK       RCC_CCIPR2_I2C4SEL_0 /*!< System clock */
#define RCC_I2C4CLKSOURCE_HSI          RCC_CCIPR2_I2C4SEL_1 /*!< HSI */
/**
  * @}
  */

/** @defgroup RCCEx_SAI1_Clock_Source SAI1 Clock Source
  * @{
  */
#define RCC_SAI1CLKSOURCE_PLLSAI1      0U                   /*!< PLLSAI1 "P" clock (PLLSAI1CLK) */
#define RCC_SAI1CLKSOURCE_PLLSAI2      RCC_CCIPR2_SAI1SEL_0 /*!< PLLSAI2 "P" clock (PLLSAI2CLK) */
#define RCC_SAI1CLKSOURCE_PLL          RCC_CCIPR2_SAI1SEL_1 /*!< PLL "P" clock (PLLSAI3CLK) */
#define RCC_SAI1CLKSOURCE_PIN          (RCC_CCIPR2_SAI1SEL_1 | RCC_CCIPR2_SAI1SEL_0) /*!< External clock SAI1_EXTCLK */
#define RCC_SAI1CLKSOURCE_HSI          RCC_CCIPR2_SAI1SEL_2 /*!< HSI */
/**
  * @}
  */

/** @defgroup RCCEx_SAI2_Clock_Source SAI2 Clock Source
  * @{
  */
#define RCC_SAI2CLKSOURCE_PLLSAI1      0U                   /*!< PLLSAI1 "P" clock (PLLSAI1CLK) */
#define RCC_SAI2CLKSOURCE_PLLSAI2      RCC_CCIPR2_SAI2SEL_0 /*!< PLLSAI2 "P" clock (PLLSAI2CLK) */
#define RCC_SAI2CLKSOURCE_PLL          RCC_CCIPR2_SAI2SEL_1 /*!< PLL "P" clock (PLLSAI3CLK) */
#define RCC_SAI2CLKSOURCE_PIN          (RCC_CCIPR2_SAI2SEL_1 | RCC_CCIPR2_SAI2SEL_0) /*!< External clock SAI1_EXTCLK */
#define RCC_SAI2CLKSOURCE_HSI          RCC_CCIPR2_SAI2SEL_2 /*!< HSI */
/**
  * @}
  */

/** @defgroup RCCEx_LPTIM1_Clock_Source LPTIM1 Clock Source
  * @{
  */
#define RCC_LPTIM1CLKSOURCE_PCLK1      0U                     /*!< PCLK1 */
#define RCC_LPTIM1CLKSOURCE_LSI        RCC_CCIPR1_LPTIM1SEL_0 /*!< LSI */
#define RCC_LPTIM1CLKSOURCE_HSI        RCC_CCIPR1_LPTIM1SEL_1 /*!< HSI */
#define RCC_LPTIM1CLKSOURCE_LSE        RCC_CCIPR1_LPTIM1SEL   /*!< LSE */
/**
  * @}
  */

/** @defgroup RCCEx_LPTIM2_Clock_Source LPTIM2 Clock Source
  * @{
  */
#define RCC_LPTIM2CLKSOURCE_PCLK1      0U                     /*!< PCLK1 */
#define RCC_LPTIM2CLKSOURCE_LSI        RCC_CCIPR1_LPTIM2SEL_0 /*!< LSI */
#define RCC_LPTIM2CLKSOURCE_HSI        RCC_CCIPR1_LPTIM2SEL_1 /*!< HSI */
#define RCC_LPTIM2CLKSOURCE_LSE        RCC_CCIPR1_LPTIM2SEL   /*!< LSE */
/**
  * @}
  */

/** @defgroup RCCEx_LPTIM3_Clock_Source LPTIM3 Clock Source
  * @{
  */
#define RCC_LPTIM3CLKSOURCE_PCLK1      0U                     /*!< PCLK1 */
#define RCC_LPTIM3CLKSOURCE_LSI        RCC_CCIPR1_LPTIM3SEL_0 /*!< LSI */
#define RCC_LPTIM3CLKSOURCE_HSI        RCC_CCIPR1_LPTIM3SEL_1 /*!< HSI */
#define RCC_LPTIM3CLKSOURCE_LSE        RCC_CCIPR1_LPTIM3SEL   /*!< LSE */
/**
  * @}
  */

/** @defgroup RCCEx_FDCAN_Clock_Source FDCAN Kernel Clock Source
  * @{
  */
#define RCC_FDCANCLKSOURCE_HSE         0U                    /*!< LSE */
#define RCC_FDCANCLKSOURCE_PLL         RCC_CCIPR1_FDCANSEL_0 /*!< PLL "Q" clock (PLL48M1CLK) */
#define RCC_FDCANCLKSOURCE_PLLSAI1     RCC_CCIPR1_FDCANSEL_1 /*!< PLLSAI1 "P" clock (PLLSAI1CLK) */
/**
  * @}
  */

/** @defgroup RCCEx_SDMMC1_Clock_Source SDMMC1 Clock Source
  * @{
  */
#define RCC_SDMMC1CLKSOURCE_HSI48      0U                     /*!< HSI48 via internal multiplexer */
#define RCC_SDMMC1CLKSOURCE_PLLSAI1    RCC_CCIPR1_CLK48MSEL_0 /*!< PLLSAI1 "Q" clock (PLL48M2CLK) via internal multiplexer */
#define RCC_SDMMC1CLKSOURCE_PLL        RCC_CCIPR1_CLK48MSEL_1 /*!< PLL "Q" clock (PLL48M1CLK) via internal multiplexer */
#define RCC_SDMMC1CLKSOURCE_MSI        RCC_CCIPR1_CLK48MSEL   /*!< MSI via internal multiplexer */
#define RCC_SDMMC1CLKSOURCE_PLLP       RCC_CCIPR2_SDMMCSEL    /*!< PLL "Q" clock (PLLSAI3CLK) */
/**
  * @}
  */

/** @defgroup RCCEx_RNG_Clock_Source RNG Clock Source
  * @{
  */
#define RCC_RNGCLKSOURCE_HSI48         0U                     /*!< HSI48 */
#define RCC_RNGCLKSOURCE_PLLSAI1       RCC_CCIPR1_CLK48MSEL_0 /*!< PLLSAI1 "Q" clock (PLL48M2CLK) */
#define RCC_RNGCLKSOURCE_PLL           RCC_CCIPR1_CLK48MSEL_1 /*!< PLL "Q" clock (PLL48M1CLK) */
#define RCC_RNGCLKSOURCE_MSI           RCC_CCIPR1_CLK48MSEL   /*!< MSI */
/**
  * @}
  */

#if defined(USB)
/** @defgroup RCCEx_USB_Clock_Source USB Clock Source
  * @{
  */
#define RCC_USBCLKSOURCE_HSI48         0U                     /*!< HSI48 */
#define RCC_USBCLKSOURCE_PLLSAI1       RCC_CCIPR1_CLK48MSEL_0 /*!< PLLSAI1 "Q" clock (PLL48M2CLK) */
#define RCC_USBCLKSOURCE_PLL           RCC_CCIPR1_CLK48MSEL_1 /*!< PLL "Q" clock (PLL48M1CLK) */
#define RCC_USBCLKSOURCE_MSI           RCC_CCIPR1_CLK48MSEL   /*!< MSI */
/**
  * @}
  */
#endif /* USB */

/** @defgroup RCCEx_ADC_Clock_Source ADC Clock Source
  * @{
  */
#define RCC_ADCCLKSOURCE_NONE          0U                  /*!< No clock */
#define RCC_ADCCLKSOURCE_PLLSAI1       RCC_CCIPR1_ADCSEL_0 /*!< PLLSAI "R" clock (PLLADC1CLK) */
#define RCC_ADCCLKSOURCE_SYSCLK        RCC_CCIPR1_ADCSEL   /*!< System clock */
/**
  * @}
  */

/** @defgroup RCCEx_DFSDM1_Clock_Source DFSDM1 Clock Source
  * @{
  */
#define RCC_DFSDM1CLKSOURCE_PCLK2      0U                  /*!< PCLK2 */
#define RCC_DFSDM1CLKSOURCE_SYSCLK     RCC_CCIPR2_DFSDMSEL /*!< System clock */
/**
  * @}
  */

/** @defgroup RCCEx_DFSDM1_Audio_Clock_Source DFSDM1 Audio Clock Source
  * @{
  */
#define RCC_DFSDM1AUDIOCLKSOURCE_SAI1  0U                     /*!< SAI1 clock */
#define RCC_DFSDM1AUDIOCLKSOURCE_HSI   RCC_CCIPR2_ADFSDMSEL_0 /*!< HSI */
#define RCC_DFSDM1AUDIOCLKSOURCE_MSI   RCC_CCIPR2_ADFSDMSEL_1 /*!< MSI */
/**
  * @}
  */

/** @defgroup RCCEx_OSPI_Clock_Source OctoSPI Clock Source
  * @{
  */
#define RCC_OSPICLKSOURCE_SYSCLK       0U                   /*!< System clock */
#define RCC_OSPICLKSOURCE_MSI          RCC_CCIPR2_OSPISEL_0 /*!< MSI */
#define RCC_OSPICLKSOURCE_PLL          RCC_CCIPR2_OSPISEL_1 /*!< PLL "Q" clock (PLL48M1CLK) */
/**
  * @}
  */


/** @defgroup RCCEx_SecureMode RCCEx Secure Mode
  * @note Only available when system implements security (TZEN=1)
  * @{
  */
#define RCC_SECURE_NONE                0U                      /*!< No security on RCC resources (default) */
#define RCC_SECURE_ALL                 0x1FFFU                 /*!< Security on all RCC resources          */

#define RCC_SECURE_HSI                 RCC_SECCFGR_HSISEC      /*!< HSI clock configuration security */
#define RCC_SECURE_HSE                 RCC_SECCFGR_HSESEC      /*!< HSE clock configuration security */
#define RCC_SECURE_MSI                 RCC_SECCFGR_MSISEC      /*!< MSI clock configuration security */
#define RCC_SECURE_LSI                 RCC_SECCFGR_LSISEC      /*!< LSI clock configuration security */
#define RCC_SECURE_SYSCLK              RCC_SECCFGR_SYSCLKSEC   /*!< SYSCLK clock; STOPWUCK and MCO output configuration security */
#define RCC_SECURE_PRESCALERS          RCC_SECCFGR_PRESCSEC    /*!< AHBx/APBx prescaler configuration security */
#define RCC_SECURE_PLL                 RCC_SECCFGR_PLLSEC      /*!< main PLL clock configuration security */
#define RCC_SECURE_PLLSAI1             RCC_SECCFGR_PLLSAI1SEC  /*!< PLLSAI1 clock configuration security */
#define RCC_SECURE_PLLSAI2             RCC_SECCFGR_PLLSAI2SEC  /*!< PLLSAI2 clock configuration security */
#define RCC_SECURE_CLK48M              RCC_SECCFGR_CLK48MSEC   /*!< 48MHz clock source selection security */
#define RCC_SECURE_HSI48               RCC_SECCFGR_HSI48SEC    /*!< HSI48 clock configuration security */
#define RCC_SECURE_RESET_FLAGS         RCC_SECCFGR_RMVFSEC     /*!< Remove reset flag security */
/**
  * @}
  */

#if defined(CRS)

/** @defgroup RCCEx_CRS_Status RCCEx CRS Status
  * @{
  */
#define RCC_CRS_NONE                   0x00000000U
#define RCC_CRS_TIMEOUT                0x00000001U
#define RCC_CRS_SYNCOK                 0x00000002U
#define RCC_CRS_SYNCWARN               0x00000004U
#define RCC_CRS_SYNCERR                0x00000008U
#define RCC_CRS_SYNCMISS               0x00000010U
#define RCC_CRS_TRIMOVF                0x00000020U
/**
  * @}
  */

/** @defgroup RCCEx_CRS_SynchroSource RCCEx CRS SynchroSource
  * @{
  */
#define RCC_CRS_SYNC_SOURCE_GPIO       0U                  /*!< Synchro Signal source GPIO */
#define RCC_CRS_SYNC_SOURCE_LSE        CRS_CFGR_SYNCSRC_0  /*!< Synchro Signal source LSE */
#define RCC_CRS_SYNC_SOURCE_USB        CRS_CFGR_SYNCSRC_1  /*!< Synchro Signal source USB SOF (default)*/
/**
  * @}
  */

/** @defgroup RCCEx_CRS_SynchroDivider RCCEx CRS SynchroDivider
  * @{
  */
#define RCC_CRS_SYNC_DIV1              0U                                        /*!< Synchro Signal not divided (default) */
#define RCC_CRS_SYNC_DIV2              CRS_CFGR_SYNCDIV_0                        /*!< Synchro Signal divided by 2 */
#define RCC_CRS_SYNC_DIV4              CRS_CFGR_SYNCDIV_1                        /*!< Synchro Signal divided by 4 */
#define RCC_CRS_SYNC_DIV8              (CRS_CFGR_SYNCDIV_1 | CRS_CFGR_SYNCDIV_0) /*!< Synchro Signal divided by 8 */
#define RCC_CRS_SYNC_DIV16             CRS_CFGR_SYNCDIV_2                        /*!< Synchro Signal divided by 16 */
#define RCC_CRS_SYNC_DIV32             (CRS_CFGR_SYNCDIV_2 | CRS_CFGR_SYNCDIV_0) /*!< Synchro Signal divided by 32 */
#define RCC_CRS_SYNC_DIV64             (CRS_CFGR_SYNCDIV_2 | CRS_CFGR_SYNCDIV_1) /*!< Synchro Signal divided by 64 */
#define RCC_CRS_SYNC_DIV128            CRS_CFGR_SYNCDIV                          /*!< Synchro Signal divided by 128 */
/**
  * @}
  */

/** @defgroup RCCEx_CRS_SynchroPolarity RCCEx CRS SynchroPolarity
  * @{
  */
#define RCC_CRS_SYNC_POLARITY_RISING   0U                  /*!< Synchro Active on rising edge (default) */
#define RCC_CRS_SYNC_POLARITY_FALLING  CRS_CFGR_SYNCPOL    /*!< Synchro Active on falling edge */
/**
  * @}
  */

/** @defgroup RCCEx_CRS_ReloadValueDefault RCCEx CRS ReloadValueDefault
  * @{
  */
#define RCC_CRS_RELOADVALUE_DEFAULT    0x0000BB7FU         /*!< The reset value of the RELOAD field corresponds
                                                                to a target frequency of 48 MHz and a synchronization signal frequency of 1 kHz (SOF signal from USB). */
/**
  * @}
  */

/** @defgroup RCCEx_CRS_ErrorLimitDefault RCCEx CRS ErrorLimitDefault
  * @{
  */
#define RCC_CRS_ERRORLIMIT_DEFAULT     0x00000022U         /*!< Default Frequency error limit */
/**
  * @}
  */

/** @defgroup RCCEx_CRS_HSI48CalibrationDefault RCCEx CRS HSI48CalibrationDefault
  * @{
  */
#define RCC_CRS_HSI48CALIBRATION_DEFAULT 0x00000040U       /*!< The default value is 64, which corresponds to the middle of the trimming interval.
                                                                The trimming step is specified in the product datasheet. A higher TRIM value corresponds
                                                                to a higher output frequency */
/**
  * @}
  */

/** @defgroup RCCEx_CRS_FreqErrorDirection RCCEx CRS FreqErrorDirection
  * @{
  */
#define RCC_CRS_FREQERRORDIR_UP        0U                  /*!< Upcounting direction, the actual frequency is above the target */
#define RCC_CRS_FREQERRORDIR_DOWN      CRS_ISR_FEDIR       /*!< Downcounting direction, the actual frequency is below the target */
/**
  * @}
  */

/** @defgroup RCCEx_CRS_Interrupt_Sources RCCEx CRS Interrupt Sources
  * @{
  */
#define RCC_CRS_IT_SYNCOK              CRS_CR_SYNCOKIE     /*!< SYNC event OK */
#define RCC_CRS_IT_SYNCWARN            CRS_CR_SYNCWARNIE   /*!< SYNC warning */
#define RCC_CRS_IT_ERR                 CRS_CR_ERRIE        /*!< Error */
#define RCC_CRS_IT_ESYNC               CRS_CR_ESYNCIE      /*!< Expected SYNC */
#define RCC_CRS_IT_SYNCERR             CRS_CR_ERRIE        /*!< SYNC error */
#define RCC_CRS_IT_SYNCMISS            CRS_CR_ERRIE        /*!< SYNC missed */
#define RCC_CRS_IT_TRIMOVF             CRS_CR_ERRIE        /*!< Trimming overflow or underflow */

/**
  * @}
  */

/** @defgroup RCCEx_CRS_Flags RCCEx CRS Flags
  * @{
  */
#define RCC_CRS_FLAG_SYNCOK            CRS_ISR_SYNCOKF     /*!< SYNC event OK flag     */
#define RCC_CRS_FLAG_SYNCWARN          CRS_ISR_SYNCWARNF   /*!< SYNC warning flag      */
#define RCC_CRS_FLAG_ERR               CRS_ISR_ERRF        /*!< Error flag        */
#define RCC_CRS_FLAG_ESYNC             CRS_ISR_ESYNCF      /*!< Expected SYNC flag     */
#define RCC_CRS_FLAG_SYNCERR           CRS_ISR_SYNCERR     /*!< SYNC error */
#define RCC_CRS_FLAG_SYNCMISS          CRS_ISR_SYNCMISS    /*!< SYNC missed*/
#define RCC_CRS_FLAG_TRIMOVF           CRS_ISR_TRIMOVF     /*!< Trimming overflow or underflow */

/**
  * @}
  */

#endif /* CRS */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup RCCEx_Exported_Macros RCCEx Exported Macros
  * @{
  */


/**
  * @brief  Macro to configure the PLLSAI1 clock multiplication and division factors.
  *
  * @note   This macro must be used only when the PLLSAI1 is disabled.
  * @note   This macro preserves the PLLSAI1's output clocks enable state.
  *
  * @param  __PLLSAI1SOURCE__ specifies the PLLSAI1 entry clock source.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_PLLSOURCE_NONE  No clock selected as PLLSAI1 clock entry
  *            @arg @ref RCC_PLLSOURCE_MSI  MSI oscillator clock selected as PLLSAI1 clock entry
  *            @arg @ref RCC_PLLSOURCE_HSI  HSI oscillator clock selected as PLLSAI1 clock entry
  *            @arg @ref RCC_PLLSOURCE_HSE  HSE oscillator clock selected as PLLSAI1 clock entry
  *
  * @param  __PLLSAI1M__ specifies the division factor of PLLSAI1 input clock.
  *         This parameter must be a number between Min_Data = 1 and Max_Data = 16.
  *
  * @param  __PLLSAI1N__ specifies the multiplication factor for PLLSAI1 VCO output clock.
  *         This parameter must be a number between Min_Data = 8 and Max_Data = 86.
  * @note   You have to set the PLLSAI1N parameter correctly to ensure that the VCO
  *         output frequency is between 64 and 344 MHz.
  *         PLLSAI1 clock frequency = f(PLLSAI1) multiplied by PLLSAI1N
  *
  * @param  __PLLSAI1P__ specifies the division factor for SAI clock.
  *         This parameter must be a number between Min_Data = 2 to Max_Data = 31.
  *         SAI1 clock frequency = f(PLLSAI1) / PLLSAI1P
  *
  * @param  __PLLSAI1Q__ specifies the division factor for USB/RNG/SDMMC1 clock.
  *         This parameter must be in the range (2, 4, 6 or 8).
  *         USB/RNG/SDMMC1 clock frequency = f(PLLSAI1) / PLLSAI1Q
  *
  * @param  __PLLSAI1R__ specifies the division factor for SAR ADC clock.
  *         This parameter must be in the range (2, 4, 6 or 8).
  *         ADC clock frequency = f(PLLSAI1) / PLLSAI1R
  *
  * @retval None
  */
#define __HAL_RCC_PLLSAI1_CONFIG(__PLLSAI1SOURCE__, __PLLSAI1M__, __PLLSAI1N__, __PLLSAI1P__, __PLLSAI1Q__, __PLLSAI1R__) \
  MODIFY_REG(RCC->PLLSAI1CFGR, \
             (RCC_PLLSAI1CFGR_PLLSAI1SRC | RCC_PLLSAI1CFGR_PLLSAI1M | RCC_PLLSAI1CFGR_PLLSAI1N | \
              RCC_PLLSAI1CFGR_PLLSAI1P | RCC_PLLSAI1CFGR_PLLSAI1Q | RCC_PLLSAI1CFGR_PLLSAI1R | \
              RCC_PLLSAI1CFGR_PLLSAI1PDIV), \
             ((__PLLSAI1SOURCE__) | \
              (((__PLLSAI1M__) - 1U) << RCC_PLLSAI1CFGR_PLLSAI1M_Pos) | \
              ((__PLLSAI1N__) << RCC_PLLSAI1CFGR_PLLSAI1N_Pos) | \
              ((((__PLLSAI1Q__) >> 1U) - 1U) << RCC_PLLSAI1CFGR_PLLSAI1Q_Pos) | \
              ((((__PLLSAI1R__) >> 1U) - 1U) << RCC_PLLSAI1CFGR_PLLSAI1R_Pos) | \
              ((__PLLSAI1P__) << RCC_PLLSAI1CFGR_PLLSAI1PDIV_Pos)))

/**
  * @brief  Macro to configure the PLLSAI1 clock multiplication factor N.
  *
  * @note   This function must be used only when the PLLSAI1 is disabled.
  * @note   PLLSAI1 clock source is common with the main PLL (configured through
  *         __HAL_RCC_PLL_CONFIG() macro)
  *
  * @param  __PLLSAI1N__ specifies the multiplication factor for PLLSAI1 VCO output clock.
  *          This parameter must be a number between 8 and 86.
  * @note   You have to set the PLLSAI1N parameter correctly to ensure that the VCO
  *         output frequency is between 64 and 344 MHz.
  *         Use to set PLLSAI1 clock frequency = f(PLLSAI1) multiplied by PLLSAI1N
  *
  * @retval None
  */
#define __HAL_RCC_PLLSAI1_MULN_CONFIG(__PLLSAI1N__) \
  MODIFY_REG(RCC->PLLSAI1CFGR, RCC_PLLSAI1CFGR_PLLSAI1N, (__PLLSAI1N__) << RCC_PLLSAI1CFGR_PLLSAI1N_Pos)

/** @brief  Macro to configure the PLLSAI1 input clock division factor M.
  *
  * @note   This function must be used only when the PLLSAI1 is disabled.
  * @note   PLLSAI1 clock source is common with the main PLL (configured through
  *         __HAL_RCC_PLL_CONFIG() macro)
  *
  * @param  __PLLSAI1M__ specifies the division factor for PLLSAI1 clock.
  *         This parameter must be a number between Min_Data = 1 and Max_Data = 16.
  *
  * @retval None
  */
#define __HAL_RCC_PLLSAI1_DIVM_CONFIG(__PLLSAI1M__) \
  MODIFY_REG(RCC->PLLSAI1CFGR, RCC_PLLSAI1CFGR_PLLSAI1M, ((__PLLSAI1M__) - 1U) << RCC_PLLSAI1CFGR_PLLSAI1M_Pos)

/** @brief  Macro to configure the PLLSAI1 clock division factor P.
  *
  * @note   This function must be used only when the PLLSAI1 is disabled.
  * @note   PLLSAI1 clock source is common with the main PLL (configured through
  *         __HAL_RCC_PLL_CONFIG() macro)
  *
  * @param  __PLLSAI1P__ specifies the division factor for SAI clock.
  *         This parameter must be a number in the range (2 to 31).
  *         Use to set SAI1 clock frequency = f(PLLSAI1) / PLLSAI1P
  *
  * @retval None
  */
#define __HAL_RCC_PLLSAI1_DIVP_CONFIG(__PLLSAI1P__) \
  MODIFY_REG(RCC->PLLSAI1CFGR, RCC_PLLSAI1CFGR_PLLSAI1PDIV, (__PLLSAI1P__) << RCC_PLLSAI1CFGR_PLLSAI1PDIV_Pos)

/** @brief  Macro to configure the PLLSAI1 clock division factor Q.
  *
  * @note   This function must be used only when the PLLSAI1 is disabled.
  * @note   PLLSAI1 clock source is common with the main PLL (configured through
  *         __HAL_RCC_PLL_CONFIG() macro)
  *
  * @param  __PLLSAI1Q__ specifies the division factor for USB/RNG/SDMMC1 clock.
  *         This parameter must be in the range (2, 4, 6 or 8).
  *         Use to set USB/RNG/SDMMC1 clock frequency = f(PLLSAI1) / PLLSAI1Q
  *
  * @retval None
  */
#define __HAL_RCC_PLLSAI1_DIVQ_CONFIG(__PLLSAI1Q__) \
  MODIFY_REG(RCC->PLLSAI1CFGR, RCC_PLLSAI1CFGR_PLLSAI1Q, (((__PLLSAI1Q__) >> 1U) - 1U) << RCC_PLLSAI1CFGR_PLLSAI1Q_Pos)

/** @brief  Macro to configure the PLLSAI1 clock division factor R.
  *
  * @note   This function must be used only when the PLLSAI1 is disabled.
  * @note   PLLSAI1 clock source is common with the main PLL (configured through
  *         __HAL_RCC_PLL_CONFIG() macro)
  *
  * @param  __PLLSAI1R__ specifies the division factor for ADC clock.
  *         This parameter must be in the range (2, 4, 6 or 8)
  *         Use to set ADC clock frequency = f(PLLSAI1) / PLLSAI1R
  *
  * @retval None
  */
#define __HAL_RCC_PLLSAI1_DIVR_CONFIG(__PLLSAI1R__) \
  MODIFY_REG(RCC->PLLSAI1CFGR, RCC_PLLSAI1CFGR_PLLSAI1R, (((__PLLSAI1R__) >> 1U) - 1U) << RCC_PLLSAI1CFGR_PLLSAI1R_Pos)

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
  * @param  __PLLSAI1_CLOCKOUT__ specifies the PLLSAI1 clock to be output.
  *         This parameter can be one or a combination of the following values:
  *            @arg @ref RCC_PLLSAI1_SAI1CLK  This clock is used to generate an accurate clock to achieve
  *                                   high-quality audio performance on SAI interface in case.
  *            @arg @ref RCC_PLLSAI1_48M2CLK  This clock is used to generate the clock for the USB FS (48 MHz),
  *                                   the random number generator (<=48 MHz) and the SDIO (<= 48 MHz).
  *            @arg @ref RCC_PLLSAI1_ADC1CLK  Clock used to clock ADC peripheral.
  * @retval None
  */

#define __HAL_RCC_PLLSAI1CLKOUT_ENABLE(__PLLSAI1_CLOCKOUT__)   SET_BIT(RCC->PLLSAI1CFGR, (__PLLSAI1_CLOCKOUT__))

#define __HAL_RCC_PLLSAI1CLKOUT_DISABLE(__PLLSAI1_CLOCKOUT__)  CLEAR_BIT(RCC->PLLSAI1CFGR, (__PLLSAI1_CLOCKOUT__))

/**
  * @brief  Macro to get clock output enable status (PLLSAI1_SAI1, PLLSAI1_USB2 and PLLSAI1_ADC1).
  * @param  __PLLSAI1_CLOCKOUT__ specifies the PLLSAI1 clock to be output.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_PLLSAI1_SAI1CLK  This clock is used to generate an accurate clock to achieve
  *                                   high-quality audio performance on SAI interface in case.
  *            @arg @ref RCC_PLLSAI1_48M2CLK  This clock is used to generate the clock for the USB FS (48 MHz),
  *                                   the random number generator (<=48 MHz) and the SDIO (<= 48 MHz).
  *            @arg @ref RCC_PLLSAI1_ADC1CLK  Clock used to clock ADC peripheral.
  * @retval SET / RESET
  */
#define __HAL_RCC_GET_PLLSAI1CLKOUT_CONFIG(__PLLSAI1_CLOCKOUT__)  READ_BIT(RCC->PLLSAI1CFGR, (__PLLSAI1_CLOCKOUT__))

/**
  * @brief  Macro to configure the PLLSAI2 clock multiplication and division factors.
  *
  * @note   This macro must be used only when the PLLSAIS is disabled.
  * @note   This macro preserves the PLLSAI2's output clocks enable state.
  *
  * @param  __PLLSAI2SOURCE__ specifies the PLLSAI1 entry clock source.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_PLLSOURCE_NONE  No clock selected as PLLSAI2 clock entry
  *            @arg @ref RCC_PLLSOURCE_MSI  MSI oscillator clock selected as PLLSAI2 clock entry
  *            @arg @ref RCC_PLLSOURCE_HSI  HSI oscillator clock selected as PLLSAI2 clock entry
  *            @arg @ref RCC_PLLSOURCE_HSE  HSE oscillator clock selected as PLLSAI2 clock entry
  *
  * @param  __PLLSAI2M__ specifies the division factor of PLLSAI2 input clock.
  *         This parameter must be a number between Min_Data = 1 and Max_Data = 16.
  *
  * @param  __PLLSAI2N__ specifies the multiplication factor for PLLSAI2 VCO output clock.
  *          This parameter must be a number between Min_Data = 8 and Max_Data = 86.
  * @note   You have to set the PLLSAI2N parameter correctly to ensure that the VCO
  *         output frequency is between 64 and 344 MHz.
  *
  * @param  __PLLSAI2P__ specifies the division factor for SAI clock.
  *         This parameter must be a number between Min_Data = 2 and Max_Data = 31.
  *         SAI2 clock frequency = f(PLLSAI2) / PLLSAI2P
  *
  * @retval None
  */
#define __HAL_RCC_PLLSAI2_CONFIG(__PLLSAI2SOURCE__, __PLLSAI2M__, __PLLSAI2N__, __PLLSAI2P__) \
  MODIFY_REG(RCC->PLLSAI2CFGR, \
             (RCC_PLLSAI2CFGR_PLLSAI2SRC | RCC_PLLSAI2CFGR_PLLSAI2M | RCC_PLLSAI2CFGR_PLLSAI2N | \
              RCC_PLLSAI2CFGR_PLLSAI2P | RCC_PLLSAI2CFGR_PLLSAI2PDIV), \
             ((__PLLSAI2SOURCE__) | \
              (((__PLLSAI2M__) - 1U) << RCC_PLLSAI2CFGR_PLLSAI2M_Pos) | \
              ((__PLLSAI2N__) << RCC_PLLSAI2CFGR_PLLSAI2N_Pos) | \
              ((__PLLSAI2P__) << RCC_PLLSAI2CFGR_PLLSAI2PDIV_Pos)))

/**
  * @brief  Macro to configure the PLLSAI2 clock multiplication factor N.
  *
  * @note   This function must be used only when the PLLSAI2 is disabled.
  * @note   PLLSAI2 clock source is common with the main PLL (configured through
  *         __HAL_RCC_PLL_CONFIG() macro)
  *
  * @param  __PLLSAI2N__ specifies the multiplication factor for PLLSAI2 VCO output clock.
  *          This parameter must be a number between 8 and 86.
  * @note   You have to set the PLLSAI2N parameter correctly to ensure that the VCO
  *         output frequency is between 64 and 344 MHz.
  *         PLLSAI1 clock frequency = f(PLLSAI1) multiplied by PLLSAI2N
  *
  * @retval None
  */
#define __HAL_RCC_PLLSAI2_MULN_CONFIG(__PLLSAI2N__) \
  MODIFY_REG(RCC->PLLSAI2CFGR, RCC_PLLSAI2CFGR_PLLSAI2N, (__PLLSAI2N__) << RCC_PLLSAI2CFGR_PLLSAI2N_Pos)

/** @brief  Macro to configure the PLLSAI2 input clock division factor M.
  *
  * @note   This function must be used only when the PLLSAI2 is disabled.
  * @note   PLLSAI2 clock source is common with the main PLL (configured through
  *         __HAL_RCC_PLL_CONFIG() macro)
  *
  * @param  __PLLSAI2M__ specifies the division factor for PLLSAI2 clock.
  *         This parameter must be a number between Min_Data = 1 and Max_Data = 16.
  *
  * @retval None
  */

#define __HAL_RCC_PLLSAI2_DIVM_CONFIG(__PLLSAI2M__) \
  MODIFY_REG(RCC->PLLSAI2CFGR, RCC_PLLSAI2CFGR_PLLSAI2M,  ((__PLLSAI2M__) - 1U) << RCC_PLLSAI2CFGR_PLLSAI2M_Pos)

/** @brief  Macro to configure the PLLSAI2 clock division factor P.
  *
  * @note   This function must be used only when the PLLSAI2 is disabled.
  * @note   PLLSAI2 clock source is common with the main PLL (configured through
  *         __HAL_RCC_PLL_CONFIG() macro)
  *
  * @param  __PLLSAI2P__ specifies the division factor.
  *         This parameter must be a number in the range (7 or 17).
  *         Use to set SAI2 clock frequency = f(PLLSAI2) / __PLLSAI2P__
  *
  * @retval None
  */
#define __HAL_RCC_PLLSAI2_DIVP_CONFIG(__PLLSAI2P__) \
  MODIFY_REG(RCC->PLLSAI2CFGR, RCC_PLLSAI2CFGR_PLLSAI2P, ((__PLLSAI2P__) >> 4U) << RCC_PLLSAI2CFGR_PLLSAI2P_Pos)

/**
  * @brief  Macros to enable or disable the PLLSAI2.
  * @note   The PLLSAI2 is disabled by hardware when entering STOP and STANDBY modes.
  * @retval None
  */

#define __HAL_RCC_PLLSAI2_ENABLE()  SET_BIT(RCC->CR, RCC_CR_PLLSAI2ON)

#define __HAL_RCC_PLLSAI2_DISABLE() CLEAR_BIT(RCC->CR, RCC_CR_PLLSAI2ON)

/**
  * @brief  Macros to enable or disable each clock output (PLLSAI2_SAI2).
  * @note   Enabling and disabling those clocks can be done without the need to stop the PLL.
  *         This is mainly used to save Power.
  * @param  __PLLSAI2_CLOCKOUT__ specifies the PLLSAI2 clock to be output.
  *         This parameter can be one or a combination of the following values:
  *            @arg @ref RCC_PLLSAI2_SAI2CLK  This clock is used to generate an accurate clock to achieve
  *                                   high-quality audio performance on SAI interface in case.
  * @retval None
  */

#define __HAL_RCC_PLLSAI2CLKOUT_ENABLE(__PLLSAI2_CLOCKOUT__)  SET_BIT(RCC->PLLSAI2CFGR, (__PLLSAI2_CLOCKOUT__))

#define __HAL_RCC_PLLSAI2CLKOUT_DISABLE(__PLLSAI2_CLOCKOUT__) CLEAR_BIT(RCC->PLLSAI2CFGR, (__PLLSAI2_CLOCKOUT__))

/**
  * @brief  Macro to get clock output enable status (PLLSAI2_SAI2).
  * @param  __PLLSAI2_CLOCKOUT__ specifies the PLLSAI2 clock to be output.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_PLLSAI2_SAI2CLK  This clock is used to generate an accurate clock to achieve
  *                                   high-quality audio performance on SAI interface in case.
  * @retval SET / RESET
  */
#define __HAL_RCC_GET_PLLSAI2CLKOUT_CONFIG(__PLLSAI2_CLOCKOUT__)  READ_BIT(RCC->PLLSAI2CFGR, (__PLLSAI2_CLOCKOUT__))

/**
  * @brief  Macro to configure the SAI1 clock source.
  * @param  __SAI1_CLKSOURCE__ defines the SAI1 clock source. This clock is derived
  *         from the PLLSAI1, system PLL or external clock (through a dedicated pin).
  *          This parameter can be one of the following values:
  *             @arg @ref RCC_SAI1CLKSOURCE_PLLSAI1  SAI1 clock = PLLSAI1 "P" clock (PLLSAI1CLK)
  *             @arg @ref RCC_SAI1CLKSOURCE_PLLSAI2  SAI1 clock = PLLSAI2 "P" clock (PLLSAI2CLK)
  *             @arg @ref RCC_SAI1CLKSOURCE_PLL  SAI1 clock  = PLL "P" clock (PLLSAI3CLK if PLLSAI2 exists, else PLLSAI2CLK)
  *             @arg @ref RCC_SAI1CLKSOURCE_PIN  SAI1 clock = External Clock (SAI1_EXTCLK)
  *             @arg @ref RCC_SAI1CLKSOURCE_HSI  SAI1 clock = HSI16
  *
  * @note  HSI16 is automatically set as SAI1 clock source when PLL are disabled for devices without PLLSAI2.
  *
  * @retval None
  */
#define __HAL_RCC_SAI1_CONFIG(__SAI1_CLKSOURCE__)\
  MODIFY_REG(RCC->CCIPR2, RCC_CCIPR2_SAI1SEL, (uint32_t)(__SAI1_CLKSOURCE__))

/** @brief  Macro to get the SAI1 clock source.
  * @retval The clock source can be one of the following values:
  *             @arg @ref RCC_SAI1CLKSOURCE_PLLSAI1  SAI1 clock = PLLSAI1 "P" clock (PLLSAI1CLK)
  *             @arg @ref RCC_SAI1CLKSOURCE_PLLSAI2  SAI1 clock = PLLSAI2 "P" clock (PLLSAI2CLK)
  *             @arg @ref RCC_SAI1CLKSOURCE_PLL  SAI1 clock  = PLL "P" clock (PLLSAI3CLK if PLLSAI2 exists, else PLLSAI2CLK)
  *             @arg @ref RCC_SAI1CLKSOURCE_PIN  SAI1 clock = External Clock (SAI1_EXTCLK)
  *             @arg @ref RCC_SAI1CLKSOURCE_HSI  SAI1 clock = HSI16
  *
  * @note  Despite returned values RCC_SAI1CLKSOURCE_PLLSAI1 or RCC_SAI1CLKSOURCE_PLL, HSI16 is automatically set as SAI1
  *        clock source when PLLs are disabled for devices without PLLSAI2.
  *
  */
#define __HAL_RCC_GET_SAI1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR2, RCC_CCIPR2_SAI1SEL)))

/**
  * @brief  Macro to configure the SAI2 clock source.
  * @param  __SAI2_CLKSOURCE__ defines the SAI2 clock source. This clock is derived
  *         from the PLLSAI2, system PLL or external clock (through a dedicated pin).
  *          This parameter can be one of the following values:
  *             @arg @ref RCC_SAI2CLKSOURCE_PLLSAI1  SAI2 clock = PLLSAI1 "P" clock (PLLSAI1CLK)
  *             @arg @ref RCC_SAI2CLKSOURCE_PLLSAI2  SAI2 clock = PLLSAI2 "P" clock (PLLSAI2CLK)
  *             @arg @ref RCC_SAI2CLKSOURCE_PLL  SAI2 clock  = PLL "P" clock (PLLSAI3CLK)
  *             @arg @ref RCC_SAI2CLKSOURCE_PIN  SAI2 clock = External Clock (SAI2_EXTCLK)
  *             @arg @ref RCC_SAI2CLKSOURCE_HSI  SAI2 clock = HSI16
  *
  * @retval None
  */
#define __HAL_RCC_SAI2_CONFIG(__SAI2_CLKSOURCE__ )\
  MODIFY_REG(RCC->CCIPR2, RCC_CCIPR2_SAI2SEL, (uint32_t)(__SAI2_CLKSOURCE__))

/** @brief  Macro to get the SAI2 clock source.
  * @retval The clock source can be one of the following values:
  *             @arg @ref RCC_SAI2CLKSOURCE_PLLSAI1  SAI2 clock = PLLSAI1 "P" clock (PLLSAI1CLK)
  *             @arg @ref RCC_SAI2CLKSOURCE_PLLSAI2  SAI2 clock = PLLSAI2 "P" clock (PLLSAI2CLK)
  *             @arg @ref RCC_SAI2CLKSOURCE_PLL  SAI2 clock  = PLL "P" clock (PLLSAI3CLK)
  *             @arg @ref RCC_SAI2CLKSOURCE_PIN  SAI2 clock = External Clock (SAI2_EXTCLK)
  *             @arg @ref RCC_SAI2CLKSOURCE_HSI  SAI2 clock = HSI16
  */
#define __HAL_RCC_GET_SAI2_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR2, RCC_CCIPR2_SAI2SEL)))

/** @brief  Macro to configure the I2C1 clock (I2C1CLK).
  *
  * @param  __I2C1_CLKSOURCE__ specifies the I2C1 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_I2C1CLKSOURCE_PCLK1  PCLK1 selected as I2C1 clock
  *            @arg @ref RCC_I2C1CLKSOURCE_HSI  HSI selected as I2C1 clock
  *            @arg @ref RCC_I2C1CLKSOURCE_SYSCLK  System Clock selected as I2C1 clock
  * @retval None
  */
#define __HAL_RCC_I2C1_CONFIG(__I2C1_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_I2C1SEL, (uint32_t)(__I2C1_CLKSOURCE__))

/** @brief  Macro to get the I2C1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_I2C1CLKSOURCE_PCLK1  PCLK1 selected as I2C1 clock
  *            @arg @ref RCC_I2C1CLKSOURCE_HSI  HSI selected as I2C1 clock
  *            @arg @ref RCC_I2C1CLKSOURCE_SYSCLK  System Clock selected as I2C1 clock
  */
#define __HAL_RCC_GET_I2C1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_I2C1SEL)))

/** @brief  Macro to configure the I2C2 clock (I2C2CLK).
  *
  * @param  __I2C2_CLKSOURCE__ specifies the I2C2 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_I2C2CLKSOURCE_PCLK1  PCLK1 selected as I2C2 clock
  *            @arg @ref RCC_I2C2CLKSOURCE_HSI  HSI selected as I2C2 clock
  *            @arg @ref RCC_I2C2CLKSOURCE_SYSCLK  System Clock selected as I2C2 clock
  * @retval None
  */
#define __HAL_RCC_I2C2_CONFIG(__I2C2_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_I2C2SEL, (uint32_t)(__I2C2_CLKSOURCE__))

/** @brief  Macro to get the I2C2 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_I2C2CLKSOURCE_PCLK1  PCLK1 selected as I2C2 clock
  *            @arg @ref RCC_I2C2CLKSOURCE_HSI  HSI selected as I2C2 clock
  *            @arg @ref RCC_I2C2CLKSOURCE_SYSCLK  System Clock selected as I2C2 clock
  */
#define __HAL_RCC_GET_I2C2_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_I2C2SEL)))

/** @brief  Macro to configure the I2C3 clock (I2C3CLK).
  *
  * @param  __I2C3_CLKSOURCE__ specifies the I2C3 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_I2C3CLKSOURCE_PCLK1  PCLK1 selected as I2C3 clock
  *            @arg @ref RCC_I2C3CLKSOURCE_HSI  HSI selected as I2C3 clock
  *            @arg @ref RCC_I2C3CLKSOURCE_SYSCLK  System Clock selected as I2C3 clock
  * @retval None
  */
#define __HAL_RCC_I2C3_CONFIG(__I2C3_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_I2C3SEL, (uint32_t)(__I2C3_CLKSOURCE__))

/** @brief  Macro to get the I2C3 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_I2C3CLKSOURCE_PCLK1  PCLK1 selected as I2C3 clock
  *            @arg @ref RCC_I2C3CLKSOURCE_HSI  HSI selected as I2C3 clock
  *            @arg @ref RCC_I2C3CLKSOURCE_SYSCLK  System Clock selected as I2C3 clock
  */
#define __HAL_RCC_GET_I2C3_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_I2C3SEL)))

/** @brief  Macro to configure the I2C4 clock (I2C4CLK).
  *
  * @param  __I2C4_CLKSOURCE__ specifies the I2C4 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_I2C4CLKSOURCE_PCLK1  PCLK1 selected as I2C4 clock
  *            @arg @ref RCC_I2C4CLKSOURCE_HSI  HSI selected as I2C4 clock
  *            @arg @ref RCC_I2C4CLKSOURCE_SYSCLK  System Clock selected as I2C4 clock
  * @retval None
  */
#define __HAL_RCC_I2C4_CONFIG(__I2C4_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR2, RCC_CCIPR2_I2C4SEL, (uint32_t)(__I2C4_CLKSOURCE__))

/** @brief  Macro to get the I2C4 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_I2C4CLKSOURCE_PCLK1  PCLK1 selected as I2C4 clock
  *            @arg @ref RCC_I2C4CLKSOURCE_HSI  HSI selected as I2C4 clock
  *            @arg @ref RCC_I2C4CLKSOURCE_SYSCLK  System Clock selected as I2C4 clock
  */
#define __HAL_RCC_GET_I2C4_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR2, RCC_CCIPR2_I2C4SEL)))


/** @brief  Macro to configure the USART1 clock (USART1CLK).
  *
  * @param  __USART1_CLKSOURCE__ specifies the USART1 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_USART1CLKSOURCE_PCLK2  PCLK2 selected as USART1 clock
  *            @arg @ref RCC_USART1CLKSOURCE_HSI  HSI selected as USART1 clock
  *            @arg @ref RCC_USART1CLKSOURCE_SYSCLK  System Clock selected as USART1 clock
  *            @arg @ref RCC_USART1CLKSOURCE_LSE  SE selected as USART1 clock
  * @retval None
  */
#define __HAL_RCC_USART1_CONFIG(__USART1_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_USART1SEL, (uint32_t)(__USART1_CLKSOURCE__))

/** @brief  Macro to get the USART1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_USART1CLKSOURCE_PCLK2  PCLK2 selected as USART1 clock
  *            @arg @ref RCC_USART1CLKSOURCE_HSI  HSI selected as USART1 clock
  *            @arg @ref RCC_USART1CLKSOURCE_SYSCLK  System Clock selected as USART1 clock
  *            @arg @ref RCC_USART1CLKSOURCE_LSE  LSE selected as USART1 clock
  */
#define __HAL_RCC_GET_USART1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_USART1SEL)))

/** @brief  Macro to configure the USART2 clock (USART2CLK).
  *
  * @param  __USART2_CLKSOURCE__ specifies the USART2 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_USART2CLKSOURCE_PCLK1  PCLK1 selected as USART2 clock
  *            @arg @ref RCC_USART2CLKSOURCE_HSI  HSI selected as USART2 clock
  *            @arg @ref RCC_USART2CLKSOURCE_SYSCLK  System Clock selected as USART2 clock
  *            @arg @ref RCC_USART2CLKSOURCE_LSE  LSE selected as USART2 clock
  * @retval None
  */
#define __HAL_RCC_USART2_CONFIG(__USART2_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_USART2SEL, (uint32_t)(__USART2_CLKSOURCE__))

/** @brief  Macro to get the USART2 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_USART2CLKSOURCE_PCLK1  PCLK1 selected as USART2 clock
  *            @arg @ref RCC_USART2CLKSOURCE_HSI  HSI selected as USART2 clock
  *            @arg @ref RCC_USART2CLKSOURCE_SYSCLK  System Clock selected as USART2 clock
  *            @arg @ref RCC_USART2CLKSOURCE_LSE  LSE selected as USART2 clock
  */
#define __HAL_RCC_GET_USART2_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_USART2SEL)))

/** @brief  Macro to configure the USART3 clock (USART3CLK).
  *
  * @param  __USART3_CLKSOURCE__ specifies the USART3 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_USART3CLKSOURCE_PCLK1  PCLK1 selected as USART3 clock
  *            @arg @ref RCC_USART3CLKSOURCE_HSI  HSI selected as USART3 clock
  *            @arg @ref RCC_USART3CLKSOURCE_SYSCLK  System Clock selected as USART3 clock
  *            @arg @ref RCC_USART3CLKSOURCE_LSE  LSE selected as USART3 clock
  * @retval None
  */
#define __HAL_RCC_USART3_CONFIG(__USART3_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_USART3SEL, (uint32_t)(__USART3_CLKSOURCE__))

/** @brief  Macro to get the USART3 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_USART3CLKSOURCE_PCLK1  PCLK1 selected as USART3 clock
  *            @arg @ref RCC_USART3CLKSOURCE_HSI  HSI selected as USART3 clock
  *            @arg @ref RCC_USART3CLKSOURCE_SYSCLK  System Clock selected as USART3 clock
  *            @arg @ref RCC_USART3CLKSOURCE_LSE  LSE selected as USART3 clock
  */
#define __HAL_RCC_GET_USART3_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_USART3SEL)))

/** @brief  Macro to configure the UART4 clock (UART4CLK).
  *
  * @param  __UART4_CLKSOURCE__ specifies the UART4 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_UART4CLKSOURCE_PCLK1  PCLK1 selected as UART4 clock
  *            @arg @ref RCC_UART4CLKSOURCE_HSI  HSI selected as UART4 clock
  *            @arg @ref RCC_UART4CLKSOURCE_SYSCLK  System Clock selected as UART4 clock
  *            @arg @ref RCC_UART4CLKSOURCE_LSE  LSE selected as UART4 clock
  * @retval None
  */
#define __HAL_RCC_UART4_CONFIG(__UART4_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_UART4SEL, (uint32_t)(__UART4_CLKSOURCE__))

/** @brief  Macro to get the UART4 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_UART4CLKSOURCE_PCLK1  PCLK1 selected as UART4 clock
  *            @arg @ref RCC_UART4CLKSOURCE_HSI  HSI selected as UART4 clock
  *            @arg @ref RCC_UART4CLKSOURCE_SYSCLK  System Clock selected as UART4 clock
  *            @arg @ref RCC_UART4CLKSOURCE_LSE  LSE selected as UART4 clock
  */
#define __HAL_RCC_GET_UART4_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_UART4SEL)))

/** @brief  Macro to configure the UART5 clock (UART5CLK).
  *
  * @param  __UART5_CLKSOURCE__ specifies the UART5 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_UART5CLKSOURCE_PCLK1  PCLK1 selected as UART5 clock
  *            @arg @ref RCC_UART5CLKSOURCE_HSI  HSI selected as UART5 clock
  *            @arg @ref RCC_UART5CLKSOURCE_SYSCLK  System Clock selected as UART5 clock
  *            @arg @ref RCC_UART5CLKSOURCE_LSE  LSE selected as UART5 clock
  * @retval None
  */
#define __HAL_RCC_UART5_CONFIG(__UART5_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_UART5SEL, (uint32_t)(__UART5_CLKSOURCE__))

/** @brief  Macro to get the UART5 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_UART5CLKSOURCE_PCLK1  PCLK1 selected as UART5 clock
  *            @arg @ref RCC_UART5CLKSOURCE_HSI  HSI selected as UART5 clock
  *            @arg @ref RCC_UART5CLKSOURCE_SYSCLK  System Clock selected as UART5 clock
  *            @arg @ref RCC_UART5CLKSOURCE_LSE  LSE selected as UART5 clock
  */
#define __HAL_RCC_GET_UART5_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_UART5SEL)))

/** @brief  Macro to configure the LPUART1 clock (LPUART1CLK).
  *
  * @param  __LPUART1_CLKSOURCE__ specifies the LPUART1 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_LPUART1CLKSOURCE_PCLK1  PCLK1 selected as LPUART1 clock
  *            @arg @ref RCC_LPUART1CLKSOURCE_HSI  HSI selected as LPUART1 clock
  *            @arg @ref RCC_LPUART1CLKSOURCE_SYSCLK  System Clock selected as LPUART1 clock
  *            @arg @ref RCC_LPUART1CLKSOURCE_LSE  LSE selected as LPUART1 clock
  * @retval None
  */
#define __HAL_RCC_LPUART1_CONFIG(__LPUART1_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_LPUART1SEL, (uint32_t)(__LPUART1_CLKSOURCE__))

/** @brief  Macro to get the LPUART1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_LPUART1CLKSOURCE_PCLK1  PCLK1 selected as LPUART1 clock
  *            @arg @ref RCC_LPUART1CLKSOURCE_HSI  HSI selected as LPUART1 clock
  *            @arg @ref RCC_LPUART1CLKSOURCE_SYSCLK  System Clock selected as LPUART1 clock
  *            @arg @ref RCC_LPUART1CLKSOURCE_LSE  LSE selected as LPUART1 clock
  */
#define __HAL_RCC_GET_LPUART1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_LPUART1SEL)))

/** @brief  Macro to configure the LPTIM1 clock (LPTIM1CLK).
  *
  * @param  __LPTIM1_CLKSOURCE__ specifies the LPTIM1 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_LPTIM1CLKSOURCE_PCLK1  PCLK1 selected as LPTIM1 clock
  *            @arg @ref RCC_LPTIM1CLKSOURCE_LSI  LSI selected as LPTIM1 clock
  *            @arg @ref RCC_LPTIM1CLKSOURCE_HSI  HSI selected as LPTIM1 clock
  *            @arg @ref RCC_LPTIM1CLKSOURCE_LSE  LSE selected as LPTIM1 clock
  * @retval None
  */
#define __HAL_RCC_LPTIM1_CONFIG(__LPTIM1_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_LPTIM1SEL, (uint32_t)(__LPTIM1_CLKSOURCE__))

/** @brief  Macro to get the LPTIM1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_LPTIM1CLKSOURCE_PCLK1  PCLK1 selected as LPTIM1 clock
  *            @arg @ref RCC_LPTIM1CLKSOURCE_LSI  LSI selected as LPTIM1 clock
  *            @arg @ref RCC_LPTIM1CLKSOURCE_HSI  HSI selected as LPTIM1 clock
  *            @arg @ref RCC_LPTIM1CLKSOURCE_LSE  LSE selected as LPTIM1 clock
  */
#define __HAL_RCC_GET_LPTIM1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_LPTIM1SEL)))

/** @brief  Macro to configure the LPTIM2 clock (LPTIM2CLK).
  *
  * @param  __LPTIM2_CLKSOURCE__ specifies the LPTIM2 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_LPTIM2CLKSOURCE_PCLK1  PCLK1 selected as LPTIM2 clock
  *            @arg @ref RCC_LPTIM2CLKSOURCE_LSI  LSI selected as LPTIM2 clock
  *            @arg @ref RCC_LPTIM2CLKSOURCE_HSI  HSI selected as LPTIM2 clock
  *            @arg @ref RCC_LPTIM2CLKSOURCE_LSE  LSE selected as LPTIM2 clock
  * @retval None
  */
#define __HAL_RCC_LPTIM2_CONFIG(__LPTIM2_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_LPTIM2SEL, (uint32_t)(__LPTIM2_CLKSOURCE__))

/** @brief  Macro to get the LPTIM2 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_LPTIM2CLKSOURCE_PCLK1  PCLK1 selected as LPTIM2 clock
  *            @arg @ref RCC_LPTIM2CLKSOURCE_LSI  HSI selected as LPTIM2 clock
  *            @arg @ref RCC_LPTIM2CLKSOURCE_HSI  HSI selected as LPTIM2 clock
  *            @arg @ref RCC_LPTIM2CLKSOURCE_LSE  LSE selected as LPTIM2 clock
  */
#define __HAL_RCC_GET_LPTIM2_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_LPTIM2SEL)))

/** @brief  Macro to configure the LPTIM3 clock (LPTIM3CLK).
  *
  * @param  __LPTIM3_CLKSOURCE__ specifies the LPTIM3 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_LPTIM3CLKSOURCE_PCLK1  PCLK1 selected as LPTIM3 clock
  *            @arg @ref RCC_LPTIM3CLKSOURCE_LSI  LSI selected as LPTIM3 clock
  *            @arg @ref RCC_LPTIM3CLKSOURCE_HSI  HSI selected as LPTIM3 clock
  *            @arg @ref RCC_LPTIM3CLKSOURCE_LSE  LSE selected as LPTIM3 clock
  * @retval None
  */
#define __HAL_RCC_LPTIM3_CONFIG(__LPTIM3_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_LPTIM3SEL, (uint32_t)(__LPTIM3_CLKSOURCE__))

/** @brief  Macro to get the LPTIM3 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_LPTIM3CLKSOURCE_PCLK1  PCLK1 selected as LPTIM3 clock
  *            @arg @ref RCC_LPTIM3CLKSOURCE_LSI  LSI selected as LPTIM3 clock
  *            @arg @ref RCC_LPTIM3CLKSOURCE_HSI  HSI selected as LPTIM3 clock
  *            @arg @ref RCC_LPTIM3CLKSOURCE_LSE  LSE selected as LPTIM3 clock
  */
#define __HAL_RCC_GET_LPTIM3_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_LPTIM3SEL)))

/** @brief  Macro to configure the FDCAN kernel clock (FDCANCLK).
  *
  * @param  __FDCAN_CLKSOURCE__ specifies the FDCAN kernel clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_FDCANCLKSOURCE_HSE  HSE selected as FDCAN kernel clock
  *            @arg @ref RCC_FDCANCLKSOURCE_PLL  PLL Clock selected as FDCAN kernel clock
  *            @arg @ref RCC_FDCANCLKSOURCE_PLLSAI1  PLLSAI1 Clock selected as FDCAN kernel clock
  * @retval None
  */
#define __HAL_RCC_FDCAN_CONFIG(__FDCAN_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_FDCANSEL, (uint32_t)(__FDCAN_CLKSOURCE__))

/** @brief  Macro to get the FDCAN clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_FDCANCLKSOURCE_HSE  HSE selected as FDCAN kernel clock
  *            @arg @ref RCC_FDCANCLKSOURCE_PLL  PLL Clock selected as FDCAN kernel clock
  *            @arg @ref RCC_FDCANCLKSOURCE_PLLSAI1  PLLSAI1 Clock selected as FDCAN kernel clock
  */
#define __HAL_RCC_GET_FDCAN_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_FDCANSEL)))

/** @brief  Macro to configure the SDMMC1 clock.
  *
  * @param  __SDMMC1_CLKSOURCE__ specifies the SDMMC1 clock source.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_SDMMC1CLKSOURCE_HSI48  HSI48 selected as SDMMC1 clock via internal multiplexer
  *            @arg @ref RCC_SDMMC1CLKSOURCE_PLLSAI1  PLLSAI1 "Q" clock (PLL48M2CLK) selected as SDMMC1 clock via internal multiplexer
  *            @arg @ref RCC_SDMMC1CLKSOURCE_PLL  PLL "Q" Clock selected as SDMMC1 clock via internal multiplexer
  *            @arg @ref RCC_SDMMC1CLKSOURCE_MSI  MSI selected as SDMMC1 clock via internal multiplexer
  *            @arg @ref RCC_SDMMC1CLKSOURCE_PLLP  PLL "P" Clock selected as SDMMC1 clock
  * @retval None
  */
#define __HAL_RCC_SDMMC1_CONFIG(__SDMMC1_CLKSOURCE__) \
  do \
  {  \
    if((__SDMMC1_CLKSOURCE__) == RCC_SDMMC1CLKSOURCE_PLLP) \
    { \
      SET_BIT(RCC->CCIPR2, RCC_CCIPR2_SDMMCSEL); \
    } \
    else \
    { \
      MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_CLK48MSEL, (uint32_t)(__SDMMC1_CLKSOURCE__)); \
      CLEAR_BIT(RCC->CCIPR2, RCC_CCIPR2_SDMMCSEL); \
    } \
  } while(0)

/** @brief  Macro to get the SDMMC1 clock.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_SDMMC1CLKSOURCE_HSI48  HSI48 selected as SDMMC1 clock via internal multiplexer
  *            @arg @ref RCC_SDMMC1CLKSOURCE_PLLSAI1  PLLSAI1 "Q" clock (PLL48M2CLK) selected as SDMMC1 clock via internal multiplexer
  *            @arg @ref RCC_SDMMC1CLKSOURCE_PLL  PLL "Q" clock (PLL48M1CLK) selected as SDMMC1 clock via internal multiplexer
  *            @arg @ref RCC_SDMMC1CLKSOURCE_MSI  MSI selected as SDMMC1 clock via internal multiplexer
  *            @arg @ref RCC_SDMMC1CLKSOURCE_PLLP  PLL "P" clock (PLLSAI3CLK) selected as SDMMC1 clock
  */
#define __HAL_RCC_GET_SDMMC1_SOURCE() \
  ((READ_BIT(RCC->CCIPR2, RCC_CCIPR2_SDMMCSEL) != 0U) ? RCC_SDMMC1CLKSOURCE_PLLP : ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_CLK48MSEL))))

/** @brief  Macro to configure the RNG clock.
  *
  * @note  USB, RNG and SDMMC1 peripherals share the same 48MHz clock source.
  *
  * @param  __RNG_CLKSOURCE__ specifies the RNG clock source.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_RNGCLKSOURCE_HSI48  HSI48 selected as RNG clock
  *            @arg @ref RCC_RNGCLKSOURCE_MSI  MSI selected as RNG clock
  *            @arg @ref RCC_RNGCLKSOURCE_PLLSAI1  PLLSAI1 Clock selected as RNG clock
  *            @arg @ref RCC_RNGCLKSOURCE_PLL  PLL Clock selected as RNG clock
  * @retval None
  */
#define __HAL_RCC_RNG_CONFIG(__RNG_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_CLK48MSEL, (uint32_t)(__RNG_CLKSOURCE__))

/** @brief  Macro to get the RNG clock.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_RNGCLKSOURCE_HSI48  HSI48 selected as RNG clock
  *            @arg @ref RCC_RNGCLKSOURCE_MSI  MSI selected as RNG clock
  *            @arg @ref RCC_RNGCLKSOURCE_PLLSAI1  PLLSAI1 "Q" clock (PLL48M2CLK) selected as RNG clock
  *            @arg @ref RCC_RNGCLKSOURCE_PLL  PLL "Q" clock (PLL48M1CLK) selected as RNG clock
  */
#define __HAL_RCC_GET_RNG_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_CLK48MSEL)))

#if defined(USB)

/** @brief  Macro to configure the USB clock (USBCLK).
  *
  * @note  USB, RNG and SDMMC1 peripherals share the same 48MHz clock source.
  *
  * @param  __USB_CLKSOURCE__ specifies the USB clock source.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_USBCLKSOURCE_HSI48  HSI48 selected as 48MHz clock
  *            @arg @ref RCC_USBCLKSOURCE_MSI  MSI selected as USB clock
  *            @arg @ref RCC_USBCLKSOURCE_PLLSAI1  PLLSAI1 "Q" clock (PLL48M2CLK) selected as USB clock
  *            @arg @ref RCC_USBCLKSOURCE_PLL  PLL "Q" clock (PLL48M1CLK) selected as USB clock
  * @retval None
  */
#define __HAL_RCC_USB_CONFIG(__USB_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_CLK48MSEL, (uint32_t)(__USB_CLKSOURCE__))

/** @brief  Macro to get the USB clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_USBCLKSOURCE_HSI48  HSI48 selected as 48MHz clock
  *            @arg @ref RCC_USBCLKSOURCE_MSI  MSI selected as USB clock
  *            @arg @ref RCC_USBCLKSOURCE_PLLSAI1  PLLSAI1 "Q" clock (PLL48M2CLK) selected as USB clock
  *            @arg @ref RCC_USBCLKSOURCE_PLL  PLL "Q" clock (PLL48M1CLK) selected as USB clock
  */
#define __HAL_RCC_GET_USB_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_CLK48MSEL)))

#endif /* USB */

/** @brief  Macro to configure the ADC interface clock.
  * @param  __ADC_CLKSOURCE__ specifies the ADC digital interface clock source.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_ADCCLKSOURCE_NONE  No clock selected as ADC clock
  *            @arg @ref RCC_ADCCLKSOURCE_PLLSAI1  PLLSAI1 Clock selected as ADC clock
  *            @arg @ref RCC_ADCCLKSOURCE_SYSCLK  System Clock selected as ADC clock
  * @retval None
  */
#define __HAL_RCC_ADC_CONFIG(__ADC_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_ADCSEL, (uint32_t)(__ADC_CLKSOURCE__))

/** @brief  Macro to get the ADC clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_ADCCLKSOURCE_NONE  No clock selected as ADC clock
  *            @arg @ref RCC_ADCCLKSOURCE_PLLSAI1  PLLSAI1 Clock selected as ADC clock
  *            @arg @ref RCC_ADCCLKSOURCE_SYSCLK  System Clock selected as ADC clock
  */
#define __HAL_RCC_GET_ADC_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_ADCSEL)))

/** @brief  Macro to configure the DFSDM1 clock.
  * @param  __DFSDM1_CLKSOURCE__ specifies the DFSDM1 clock source.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_DFSDM1CLKSOURCE_PCLK2  PCLK2 Clock selected as DFSDM1 clock
  *            @arg @ref RCC_DFSDM1CLKSOURCE_SYSCLK  System Clock selected as DFSDM1 clock
  * @retval None
  */
#define __HAL_RCC_DFSDM1_CONFIG(__DFSDM1_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR2, RCC_CCIPR2_DFSDMSEL, (uint32_t)(__DFSDM1_CLKSOURCE__))

/** @brief  Macro to get the DFSDM1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_DFSDM1CLKSOURCE_PCLK2  PCLK2 Clock selected as DFSDM1 clock
  *            @arg @ref RCC_DFSDM1CLKSOURCE_SYSCLK  System Clock selected as DFSDM1 clock
  */
#define __HAL_RCC_GET_DFSDM1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR2, RCC_CCIPR2_DFSDMSEL)))

/** @brief  Macro to configure the DFSDM1 audio clock.
  * @param  __DFSDM1AUDIO_CLKSOURCE__ specifies the DFSDM1 audio clock source.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_DFSDM1AUDIOCLKSOURCE_SAI1  SAI1 clock selected as DFSDM1 audio clock
  *            @arg @ref RCC_DFSDM1AUDIOCLKSOURCE_HSI   HSI clock selected as DFSDM1 audio clock
  *            @arg @ref RCC_DFSDM1AUDIOCLKSOURCE_MSI   MSI clock selected as DFSDM1 audio clock
  * @retval None
  */
#define __HAL_RCC_DFSDM1AUDIO_CONFIG(__DFSDM1AUDIO_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR2, RCC_CCIPR2_ADFSDMSEL, (uint32_t)(__DFSDM1AUDIO_CLKSOURCE__))

/** @brief  Macro to get the DFSDM1 audio clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_DFSDM1AUDIOCLKSOURCE_SAI1  SAI1 clock selected as DFSDM1 audio clock
  *            @arg @ref RCC_DFSDM1AUDIOCLKSOURCE_HSI   HSI clock selected as DFSDM1 audio clock
  *            @arg @ref RCC_DFSDM1AUDIOCLKSOURCE_MSI   MSI clock selected as DFSDM1 audio clock
  */
#define __HAL_RCC_GET_DFSDM1AUDIO_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR2, RCC_CCIPR2_ADFSDMSEL)))

/** @brief  Macro to configure the OctoSPI clock.
  * @param  __OSPI_CLKSOURCE__ specifies the OctoSPI clock source.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_OSPICLKSOURCE_SYSCLK  System Clock selected as OctoSPI clock
  *            @arg @ref RCC_OSPICLKSOURCE_MSI     MSI clock selected as OctoSPI clock
  *            @arg @ref RCC_OSPICLKSOURCE_PLL     PLL Q divider clock selected as OctoSPI clock
  * @retval None
  */
#define __HAL_RCC_OSPI_CONFIG(__OSPI_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR2, RCC_CCIPR2_OSPISEL, (uint32_t)(__OSPI_CLKSOURCE__))

/** @brief  Macro to get the OctoSPI clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_OSPICLKSOURCE_SYSCLK  System Clock selected as OctoSPI clock
  *            @arg @ref RCC_OSPICLKSOURCE_MSI     MSI clock selected as OctoSPI clock
  *            @arg @ref RCC_OSPICLKSOURCE_PLL     PLL Q divider clock selected as OctoSPI clock
  */
#define __HAL_RCC_GET_OSPI_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR2, RCC_CCIPR2_OSPISEL)))

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


/** @defgroup RCCEx_Security_Extensions RCCEx Security Extensions
  * @{
  */
/* Clock/System Security status */
#define __HAL_RCC_HSI_IS_SECURITY_ENABLED()      (READ_BIT(RCC->SECSR, RCC_SECSR_HSISECF) != 0U)

#define __HAL_RCC_HSE_IS_SECURITY_ENABLED()      (READ_BIT(RCC->SECSR, RCC_SECSR_HSESECF) != 0U)

#define __HAL_RCC_MSI_IS_SECURITY_ENABLED()      (READ_BIT(RCC->SECSR, RCC_SECSR_MSISECF) != 0U)

#define __HAL_RCC_LSI_IS_SECURITY_ENABLED()      (READ_BIT(RCC->SECSR, RCC_SECSR_LSISECF) != 0U)

#define __HAL_RCC_LSE_IS_SECURITY_ENABLED()      (READ_BIT(RCC->SECSR, RCC_SECSR_LSESECF) != 0U)

#define __HAL_RCC_HSI48_IS_SECURITY_ENABLED()    (READ_BIT(RCC->SECSR, RCC_SECSR_HSI48SECF) != 0U)

#define __HAL_RCC_SYSCLK_IS_SECURITY_ENABLED()   (READ_BIT(RCC->SECSR, RCC_SECSR_CKSYSSECF) != 0U)

#define __HAL_RCC_PRESCALER_IS_SECURITY_ENABLED() (READ_BIT(RCC->SECSR, RCC_SECSR_PRESCSECF) != 0U)

#define __HAL_RCC_PLL_IS_SECURITY_ENABLED()      (READ_BIT(RCC->SECSR, RCC_SECSR_PLLSECF) != 0U)

#define __HAL_RCC_PLLSAI1_IS_SECURITY_ENABLED()  (READ_BIT(RCC->SECSR, RCC_SECSR_PLLSAI1SECF) != 0U)

#define __HAL_RCC_PLLSAI2_IS_SECURITY_ENABLED()  (READ_BIT(RCC->SECSR, RCC_SECSR_PLLSAI2SECF) != 0U)

#define __HAL_RCC_CLK48_IS_SECURITY_ENABLED()    (READ_BIT(RCC->SECSR, RCC_SECSR_SEL48SECF) != 0U)

#define __HAL_RCC_RMVF_IS_SECURITY_ENABLED()     (READ_BIT(RCC->SECSR, RCC_SECSR_RMVFSECF) != 0U)

/* Peripheral Security status */
#define __HAL_RCC_DMA1_IS_SECURITY_ENABLED()     (READ_BIT(RCC->AHB1SECSR, RCC_AHB1SECSR_DMA1SECF) != 0U)

#define __HAL_RCC_DMA2_IS_SECURITY_ENABLED()     (READ_BIT(RCC->AHB1SECSR, RCC_AHB1SECSR_DMA2SECF) != 0U)

#define __HAL_RCC_DMAMUX1_IS_SECURITY_ENABLED()  (READ_BIT(RCC->AHB1SECSR, RCC_AHB1SECSR_DMAMUX1SECF) != 0U)

#define __HAL_RCC_FLASH_IS_SECURITY_ENABLED()    (READ_BIT(RCC->AHB1SECSR, RCC_AHB1SECSR_FLASHSECF) != 0U)

#define __HAL_RCC_SRAM1_IS_SECURITY_ENABLED()    (READ_BIT(RCC->AHB1SECSR, RCC_AHB1SECSR_SRAM1SECF) != 0U)

#define __HAL_RCC_CRC_IS_SECURITY_ENABLED()      (READ_BIT(RCC->AHB1SECSR, RCC_AHB1SECSR_CRCSECF) != 0U)

#define __HAL_RCC_TSC_IS_SECURITY_ENABLED()      (READ_BIT(RCC->AHB1SECSR, RCC_AHB1SECSR_TSCSECF) != 0U)

#define __HAL_RCC_GTZC_IS_SECURITY_ENABLED()     (READ_BIT(RCC->AHB1SECSR, RCC_AHB1SECSR_GTZCSECF) != 0U)

#define __HAL_RCC_ICACHE_IS_SECURITY_ENABLED()   (READ_BIT(RCC->AHB1SECSR, RCC_AHB1SECSR_ICACHESECF) != 0U)

#define __HAL_RCC_GPIOA_IS_SECURITY_ENABLED()    (READ_BIT(RCC->AHB2SECSR, RCC_AHB2SECSR_GPIOASECF) != 0U)

#define __HAL_RCC_GPIOB_IS_SECURITY_ENABLED()    (READ_BIT(RCC->AHB2SECSR, RCC_AHB2SECSR_GPIOBSECF) != 0U)

#define __HAL_RCC_GPIOC_IS_SECURITY_ENABLED()    (READ_BIT(RCC->AHB2SECSR, RCC_AHB2SECSR_GPIOCSECF) != 0U)

#define __HAL_RCC_GPIOD_IS_SECURITY_ENABLED()    (READ_BIT(RCC->AHB2SECSR, RCC_AHB2SECSR_GPIODSECF) != 0U)

#define __HAL_RCC_GPIOE_IS_SECURITY_ENABLED()    (READ_BIT(RCC->AHB2SECSR, RCC_AHB2SECSR_GPIOESECF) != 0U)

#define __HAL_RCC_GPIOF_IS_SECURITY_ENABLED()    (READ_BIT(RCC->AHB2SECSR, RCC_AHB2SECSR_GPIOFSECF) != 0U)

#define __HAL_RCC_GPIOG_IS_SECURITY_ENABLED()    (READ_BIT(RCC->AHB2SECSR, RCC_AHB2SECSR_GPIOGSECF) != 0U)

#define __HAL_RCC_GPIOH_IS_SECURITY_ENABLED()    (READ_BIT(RCC->AHB2SECSR, RCC_AHB2SECSR_GPIOHSECF) != 0U)

#define __HAL_RCC_SRAM2_IS_SECURITY_ENABLED()    (READ_BIT(RCC->AHB2SECSR, RCC_AHB2SECSR_SRAM2SECF) != 0U)

#define __HAL_RCC_ADC_IS_SECURITY_ENABLED()      (READ_BIT(RCC->AHB2SECSR, RCC_AHB2SECSR_ADCSECF) != 0U)

#define __HAL_RCC_AES_IS_SECURITY_ENABLED()      (READ_BIT(RCC->AHB2SECSR, RCC_AHB2SECSR_AESSECF) != 0U)

#define __HAL_RCC_HASH_IS_SECURITY_ENABLED()     (READ_BIT(RCC->AHB2SECSR, RCC_AHB2SECSR_HASHSECF) != 0U)

#define __HAL_RCC_RNG_IS_SECURITY_ENABLED()      (READ_BIT(RCC->AHB2SECSR, RCC_AHB2SECSR_RNGSECF) != 0U)

#define __HAL_RCC_PKA_IS_SECURITY_ENABLED()      (READ_BIT(RCC->AHB2SECSR, RCC_AHB2SECSR_PKASECF) != 0U)

#define __HAL_RCC_OTFDEC1_IS_SECURITY_ENABLED()  (READ_BIT(RCC->AHB2SECSR, RCC_AHB2SECSR_OTFDEC1SECF) != 0U)

#define __HAL_RCC_SDMMC1_IS_SECURITY_ENABLED()   (READ_BIT(RCC->AHB2SECSR, RCC_AHB2SECSR_SDMMC1SECF) != 0U)

#define __HAL_RCC_FMC_IS_SECURITY_ENABLED()      (READ_BIT(RCC->AHB3SECSR, RCC_AHB3SECSR_FMCSECF) != 0U)

#define __HAL_RCC_OSPI1_IS_SECURITY_ENABLED()    (READ_BIT(RCC->AHB3SECSR, RCC_AHB3SECSR_OSPI1SECF) != 0U)

#define __HAL_RCC_TIM2_IS_SECURITY_ENABLED()     (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_TIM2SECF) != 0U)

#define __HAL_RCC_TIM3_IS_SECURITY_ENABLED()     (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_TIM3SECF) != 0U)

#define __HAL_RCC_TIM4_IS_SECURITY_ENABLED()     (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_TIM4SECF) != 0U)

#define __HAL_RCC_TIM5_IS_SECURITY_ENABLED()     (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_TIM5SECF) != 0U)

#define __HAL_RCC_TIM6_IS_SECURITY_ENABLED()     (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_TIM6SECF) != 0U)

#define __HAL_RCC_TIM7_IS_SECURITY_ENABLED()     (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_TIM7SECF) != 0U)

#define __HAL_RCC_RTCAPB_IS_SECURITY_ENABLED()   (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_RTCAPBSECF) != 0U)

#define __HAL_RCC_WWDG_IS_SECURITY_ENABLED()     (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_WWDGSECF) != 0U)

#define __HAL_RCC_SPI2_IS_SECURITY_ENABLED()     (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_SPI2SECF) != 0U)

#define __HAL_RCC_SPI3_IS_SECURITY_ENABLED()     (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_SPI3SECF) != 0U)

#define __HAL_RCC_USART2_IS_SECURITY_ENABLED()   (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_USART2SECF) != 0U)

#define __HAL_RCC_USART3_IS_SECURITY_ENABLED()   (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_USART3SECF) != 0U)

#define __HAL_RCC_UART4_IS_SECURITY_ENABLED()    (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_UART4SECF) != 0U)

#define __HAL_RCC_UART5_IS_SECURITY_ENABLED()    (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_UART5SECF) != 0U)

#define __HAL_RCC_I2C1_IS_SECURITY_ENABLED()     (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_I2C1SECF) != 0U)

#define __HAL_RCC_I2C2_IS_SECURITY_ENABLED()     (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_I2C2SECF) != 0U)

#define __HAL_RCC_I2C3_IS_SECURITY_ENABLED()     (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_I2C3SECF) != 0U)

#define __HAL_RCC_CRS_IS_SECURITY_ENABLED()      (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_CRSSECF) != 0U)

#define __HAL_RCC_PWR_IS_SECURITY_ENABLED()      (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_PWRSECF) != 0U)

#define __HAL_RCC_DAC1_IS_SECURITY_ENABLED()     (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_DAC1SECF) != 0U)

#define __HAL_RCC_OPAMP_IS_SECURITY_ENABLED()    (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_OPAMPSECF) != 0U)

#define __HAL_RCC_LPTIM1_IS_SECURITY_ENABLED()   (READ_BIT(RCC->APB1SECSR1, RCC_APB1SECSR1_LPTIM1SECF) != 0U)

#define __HAL_RCC_LPUART1_IS_SECURITY_ENABLED()  (READ_BIT(RCC->APB1SECSR2, RCC_APB1SECSR2_LPUART1SECF) != 0U)

#define __HAL_RCC_I2C4_IS_SECURITY_ENABLED()     (READ_BIT(RCC->APB1SECSR2, RCC_APB1SECSR2_I2C4SECF) != 0U)

#define __HAL_RCC_LPTIM2_IS_SECURITY_ENABLED()   (READ_BIT(RCC->APB1SECSR2, RCC_APB1SECSR2_LPTIM2SECF) != 0U)

#define __HAL_RCC_LPTIM3_IS_SECURITY_ENABLED()   (READ_BIT(RCC->APB1SECSR2, RCC_APB1SECSR2_LPTIM3SECF) != 0U)

#define __HAL_RCC_FDCAN1_IS_SECURITY_ENABLED()   (READ_BIT(RCC->APB1SECSR2, RCC_APB1SECSR2_FDCAN1SECF) != 0U)

#if defined(USB)
#define __HAL_RCC_USB_IS_SECURITY_ENABLED()      (READ_BIT(RCC->APB1SECSR2, RCC_APB1SECSR2_USBFSSECF) != 0U)
#endif /* USB */

#define __HAL_RCC_UCPD1_IS_SECURITY_ENABLED()    (READ_BIT(RCC->APB1SECSR2, RCC_APB1SECSR2_UCPD1SECF) != 0U)

#define __HAL_RCC_SYSCFG_IS_SECURITY_ENABLED()   (READ_BIT(RCC->APB2SECSR, RCC_APB2SECSR_SYSCFGSECF) != 0U)

#define __HAL_RCC_TIM1_IS_SECURITY_ENABLED()     (READ_BIT(RCC->APB2SECSR, RCC_APB2SECSR_TIM1SECF) != 0U)

#define __HAL_RCC_SPI1_IS_SECURITY_ENABLED()     (READ_BIT(RCC->APB2SECSR, RCC_APB2SECSR_SPI1SECF) != 0U)

#define __HAL_RCC_TIM8_IS_SECURITY_ENABLED()     (READ_BIT(RCC->APB2SECSR, RCC_APB2SECSR_TIM8SECF) != 0U)

#define __HAL_RCC_USART1_IS_SECURITY_ENABLED()   (READ_BIT(RCC->APB2SECSR, RCC_APB2SECSR_USART1SECF) != 0U)

#define __HAL_RCC_TIM15_IS_SECURITY_ENABLED()    (READ_BIT(RCC->APB2SECSR, RCC_APB2SECSR_TIM15SECF) != 0U)

#define __HAL_RCC_TIM16_IS_SECURITY_ENABLED()    (READ_BIT(RCC->APB2SECSR, RCC_APB2SECSR_TIM16SECF) != 0U)

#define __HAL_RCC_TIM17_IS_SECURITY_ENABLED()    (READ_BIT(RCC->APB2SECSR, RCC_APB2SECSR_TIM17SECF) != 0U)

#define __HAL_RCC_SAI1_IS_SECURITY_ENABLED()     (READ_BIT(RCC->APB2SECSR, RCC_APB2SECSR_SAI1SECF) != 0U)

#define __HAL_RCC_SAI2_IS_SECURITY_ENABLED()     (READ_BIT(RCC->APB2SECSR, RCC_APB2SECSR_SAI2SECF) != 0U)

#define __HAL_RCC_DFSDM1_IS_SECURITY_ENABLED()   (READ_BIT(RCC->APB2SECSR, RCC_APB2SECSR_DFSDM1SECF) != 0U)
/**
  * @}
  */


#if defined(CRS)

/** @defgroup RCCEx_CRS_Extended_Features RCCEx CRS Extended Features
  * @{
  */

/**
  * @brief  Enable the specified CRS interrupts.
  * @param  __INTERRUPT__ specifies the CRS interrupt sources to be enabled.
  *          This parameter can be any combination of the following values:
  *              @arg @ref RCC_CRS_IT_SYNCOK  SYNC event OK interrupt
  *              @arg @ref RCC_CRS_IT_SYNCWARN  SYNC warning interrupt
  *              @arg @ref RCC_CRS_IT_ERR  Synchronization or trimming error interrupt
  *              @arg @ref RCC_CRS_IT_ESYNC  Expected SYNC interrupt
  * @retval None
  */
#define __HAL_RCC_CRS_ENABLE_IT(__INTERRUPT__)   SET_BIT(CRS->CR, (__INTERRUPT__))

/**
  * @brief  Disable the specified CRS interrupts.
  * @param  __INTERRUPT__ specifies the CRS interrupt sources to be disabled.
  *          This parameter can be any combination of the following values:
  *              @arg @ref RCC_CRS_IT_SYNCOK  SYNC event OK interrupt
  *              @arg @ref RCC_CRS_IT_SYNCWARN  SYNC warning interrupt
  *              @arg @ref RCC_CRS_IT_ERR  Synchronization or trimming error interrupt
  *              @arg @ref RCC_CRS_IT_ESYNC  Expected SYNC interrupt
  * @retval None
  */
#define __HAL_RCC_CRS_DISABLE_IT(__INTERRUPT__)  CLEAR_BIT(CRS->CR, (__INTERRUPT__))

/** @brief  Check whether the CRS interrupt has occurred or not.
  * @param  __INTERRUPT__ specifies the CRS interrupt source to check.
  *         This parameter can be one of the following values:
  *              @arg @ref RCC_CRS_IT_SYNCOK  SYNC event OK interrupt
  *              @arg @ref RCC_CRS_IT_SYNCWARN  SYNC warning interrupt
  *              @arg @ref RCC_CRS_IT_ERR  Synchronization or trimming error interrupt
  *              @arg @ref RCC_CRS_IT_ESYNC  Expected SYNC interrupt
  * @retval The new state of __INTERRUPT__ (0 or 1).
  */
#define __HAL_RCC_CRS_GET_IT_SOURCE(__INTERRUPT__)  ((READ_BIT(CRS->CR, (__INTERRUPT__)) != 0U) ? 1U : 0U)

/** @brief  Clear the CRS interrupt pending bits
  * @param  __INTERRUPT__ specifies the interrupt pending bit to clear.
  *         This parameter can be any combination of the following values:
  *              @arg @ref RCC_CRS_IT_SYNCOK  SYNC event OK interrupt
  *              @arg @ref RCC_CRS_IT_SYNCWARN  SYNC warning interrupt
  *              @arg @ref RCC_CRS_IT_ERR  Synchronization or trimming error interrupt
  *              @arg @ref RCC_CRS_IT_ESYNC  Expected SYNC interrupt
  *              @arg @ref RCC_CRS_IT_TRIMOVF  Trimming overflow or underflow interrupt
  *              @arg @ref RCC_CRS_IT_SYNCERR  SYNC error interrupt
  *              @arg @ref RCC_CRS_IT_SYNCMISS  SYNC missed interrupt
  */
/* CRS IT Error Mask */
#define  RCC_CRS_IT_ERROR_MASK                 ((uint32_t)(RCC_CRS_IT_TRIMOVF\
                                                           | RCC_CRS_IT_SYNCERR | RCC_CRS_IT_SYNCMISS))

#define __HAL_RCC_CRS_CLEAR_IT(__INTERRUPT__)  do { \
                                                    if(((__INTERRUPT__) & RCC_CRS_IT_ERROR_MASK) != 0U) \
                                                    { \
                                                      WRITE_REG(CRS->ICR, CRS_ICR_ERRC | ((__INTERRUPT__) & ~RCC_CRS_IT_ERROR_MASK)); \
                                                    } \
                                                    else \
                                                    { \
                                                      WRITE_REG(CRS->ICR, (__INTERRUPT__)); \
                                                    } \
                                                  } while(0)

/**
  * @brief  Check whether the specified CRS flag is set or not.
  * @param  __FLAG__ specifies the flag to check.
  *          This parameter can be one of the following values:
  *              @arg @ref RCC_CRS_FLAG_SYNCOK  SYNC event OK
  *              @arg @ref RCC_CRS_FLAG_SYNCWARN  SYNC warning
  *              @arg @ref RCC_CRS_FLAG_ERR  Error
  *              @arg @ref RCC_CRS_FLAG_ESYNC  Expected SYNC
  *              @arg @ref RCC_CRS_FLAG_TRIMOVF  Trimming overflow or underflow
  *              @arg @ref RCC_CRS_FLAG_SYNCERR  SYNC error
  *              @arg @ref RCC_CRS_FLAG_SYNCMISS  SYNC missed
  * @retval The new state of _FLAG_ (TRUE or FALSE).
  */
#define __HAL_RCC_CRS_GET_FLAG(__FLAG__)  (READ_BIT(CRS->ISR, (__FLAG__)) == (__FLAG__))

/**
  * @brief  Clear the CRS specified FLAG.
  * @param __FLAG__ specifies the flag to clear.
  *          This parameter can be one of the following values:
  *              @arg @ref RCC_CRS_FLAG_SYNCOK  SYNC event OK
  *              @arg @ref RCC_CRS_FLAG_SYNCWARN  SYNC warning
  *              @arg @ref RCC_CRS_FLAG_ERR  Error
  *              @arg @ref RCC_CRS_FLAG_ESYNC  Expected SYNC
  *              @arg @ref RCC_CRS_FLAG_TRIMOVF  Trimming overflow or underflow
  *              @arg @ref RCC_CRS_FLAG_SYNCERR  SYNC error
  *              @arg @ref RCC_CRS_FLAG_SYNCMISS  SYNC missed
  * @note RCC_CRS_FLAG_ERR clears RCC_CRS_FLAG_TRIMOVF, RCC_CRS_FLAG_SYNCERR, RCC_CRS_FLAG_SYNCMISS and consequently RCC_CRS_FLAG_ERR
  * @retval None
  */

/* CRS Flag Error Mask */
#define RCC_CRS_FLAG_ERROR_MASK                ((uint32_t)(RCC_CRS_FLAG_TRIMOVF\
                                                           | RCC_CRS_FLAG_SYNCERR | RCC_CRS_FLAG_SYNCMISS))

#define __HAL_RCC_CRS_CLEAR_FLAG(__FLAG__)     do { \
                                                    if(((__FLAG__) & RCC_CRS_FLAG_ERROR_MASK) != 0U) \
                                                    { \
                                                      WRITE_REG(CRS->ICR, CRS_ICR_ERRC | ((__FLAG__) & ~RCC_CRS_FLAG_ERROR_MASK)); \
                                                    } \
                                                    else \
                                                    { \
                                                      WRITE_REG(CRS->ICR, (__FLAG__)); \
                                                    } \
                                                  } while(0)

/**
  * @brief  Enable the oscillator clock for frequency error counter.
  * @note   when the CEN bit is set the CRS_CFGR register becomes write-protected.
  * @retval None
  */
#define __HAL_RCC_CRS_FREQ_ERROR_COUNTER_ENABLE()  SET_BIT(CRS->CR, CRS_CR_CEN)

/**
  * @brief  Disable the oscillator clock for frequency error counter.
  * @retval None
  */
#define __HAL_RCC_CRS_FREQ_ERROR_COUNTER_DISABLE() CLEAR_BIT(CRS->CR, CRS_CR_CEN)

/**
  * @brief  Enable the automatic hardware adjustment of TRIM bits.
  * @note   When the AUTOTRIMEN bit is set the CRS_CFGR register becomes write-protected.
  * @retval None
  */
#define __HAL_RCC_CRS_AUTOMATIC_CALIB_ENABLE()     SET_BIT(CRS->CR, CRS_CR_AUTOTRIMEN)

/**
  * @brief  Enable or disable the automatic hardware adjustment of TRIM bits.
  * @retval None
  */
#define __HAL_RCC_CRS_AUTOMATIC_CALIB_DISABLE()    CLEAR_BIT(CRS->CR, CRS_CR_AUTOTRIMEN)

/**
  * @brief  Macro to calculate reload value to be set in CRS register according to target and sync frequencies
  * @note   The RELOAD value should be selected according to the ratio between the target frequency and the frequency
  *             of the synchronization source after prescaling. It is then decreased by one in order to
  *             reach the expected synchronization on the zero value. The formula is the following:
  *             RELOAD = (fTARGET / fSYNC) -1
  * @param  __FTARGET__ Target frequency (value in Hz).
  * @param  __FSYNC__ Synchronization signal frequency (value in Hz).
  * @retval None
  */
#define __HAL_RCC_CRS_RELOADVALUE_CALCULATE(__FTARGET__, __FSYNC__)  (((__FTARGET__) / (__FSYNC__)) - 1U)

/**
  * @}
  */

#endif /* CRS */

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
void              HAL_RCCEx_LSECSS_IRQHandler(void);
void              HAL_RCCEx_LSECSS_Callback(void);
void              HAL_RCCEx_EnableLSCO(uint32_t LSCOSource);
void              HAL_RCCEx_DisableLSCO(void);
void              HAL_RCCEx_EnableMSIPLLMode(void);
void              HAL_RCCEx_DisableMSIPLLMode(void);

/**
  * @}
  */

#if defined(CRS)

/** @addtogroup RCCEx_Exported_Functions_Group3
  * @{
  */

void              HAL_RCCEx_CRSConfig(RCC_CRSInitTypeDef *pInit);
void              HAL_RCCEx_CRSSoftwareSynchronizationGenerate(void);
void              HAL_RCCEx_CRSGetSynchronizationInfo(RCC_CRSSynchroInfoTypeDef *pSynchroInfo);
uint32_t          HAL_RCCEx_CRSWaitSynchronization(uint32_t Timeout);
void              HAL_RCCEx_CRS_IRQHandler(void);
void              HAL_RCCEx_CRS_SyncOkCallback(void);
void              HAL_RCCEx_CRS_SyncWarnCallback(void);
void              HAL_RCCEx_CRS_ExpectedSyncCallback(void);
void              HAL_RCCEx_CRS_ErrorCallback(uint32_t Error);

/**
  * @}
  */

#endif /* CRS */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @addtogroup RCCEx_Private_Macros
  * @{
  */

#define IS_RCC_LSCOSOURCE(__SOURCE__) (((__SOURCE__) == RCC_LSCOSOURCE_LSI) || \
                                       ((__SOURCE__) == RCC_LSCOSOURCE_LSE))

#define IS_RCC_PERIPHCLOCK(__SELECTION__)  \
  ((((__SELECTION__) & RCC_PERIPHCLK_USART1)      == RCC_PERIPHCLK_USART1)  || \
   (((__SELECTION__) & RCC_PERIPHCLK_USART2)      == RCC_PERIPHCLK_USART2)  || \
   (((__SELECTION__) & RCC_PERIPHCLK_USART3)      == RCC_PERIPHCLK_USART3)  || \
   (((__SELECTION__) & RCC_PERIPHCLK_UART4)       == RCC_PERIPHCLK_UART4)   || \
   (((__SELECTION__) & RCC_PERIPHCLK_UART5)       == RCC_PERIPHCLK_UART5)   || \
   (((__SELECTION__) & RCC_PERIPHCLK_LPUART1)     == RCC_PERIPHCLK_LPUART1) || \
   (((__SELECTION__) & RCC_PERIPHCLK_I2C1)        == RCC_PERIPHCLK_I2C1)    || \
   (((__SELECTION__) & RCC_PERIPHCLK_I2C2)        == RCC_PERIPHCLK_I2C2)    || \
   (((__SELECTION__) & RCC_PERIPHCLK_I2C3)        == RCC_PERIPHCLK_I2C3)    || \
   (((__SELECTION__) & RCC_PERIPHCLK_I2C4)        == RCC_PERIPHCLK_I2C4)    || \
   (((__SELECTION__) & RCC_PERIPHCLK_LPTIM1)      == RCC_PERIPHCLK_LPTIM1)  || \
   (((__SELECTION__) & RCC_PERIPHCLK_LPTIM2)      == RCC_PERIPHCLK_LPTIM2)  || \
   (((__SELECTION__) & RCC_PERIPHCLK_LPTIM3)      == RCC_PERIPHCLK_LPTIM3)  || \
   (((__SELECTION__) & RCC_PERIPHCLK_SAI1)        == RCC_PERIPHCLK_SAI1)    || \
   (((__SELECTION__) & RCC_PERIPHCLK_SAI2)        == RCC_PERIPHCLK_SAI2)    || \
   (((__SELECTION__) & RCC_PERIPHCLK_USB)         == RCC_PERIPHCLK_USB)     || \
   (((__SELECTION__) & RCC_PERIPHCLK_ADC)         == RCC_PERIPHCLK_ADC)     || \
   (((__SELECTION__) & RCC_PERIPHCLK_DFSDM1)      == RCC_PERIPHCLK_DFSDM1)  || \
   (((__SELECTION__) & RCC_PERIPHCLK_DFSDM1AUDIO) == RCC_PERIPHCLK_DFSDM1AUDIO) || \
   (((__SELECTION__) & RCC_PERIPHCLK_RTC)         == RCC_PERIPHCLK_RTC)     || \
   (((__SELECTION__) & RCC_PERIPHCLK_RNG)         == RCC_PERIPHCLK_RNG)     || \
   (((__SELECTION__) & RCC_PERIPHCLK_SDMMC1)      == RCC_PERIPHCLK_SDMMC1)  || \
   (((__SELECTION__) & RCC_PERIPHCLK_OSPI)        == RCC_PERIPHCLK_OSPI)    || \
   (((__SELECTION__) & RCC_PERIPHCLK_FDCAN)       == RCC_PERIPHCLK_FDCAN))

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

#define IS_RCC_I2C4CLKSOURCE(__SOURCE__)   \
  (((__SOURCE__) == RCC_I2C4CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_I2C4CLKSOURCE_SYSCLK)|| \
   ((__SOURCE__) == RCC_I2C4CLKSOURCE_HSI))

#define IS_RCC_SAI1CLK(__SOURCE__)   \
  (((__SOURCE__) == RCC_SAI1CLKSOURCE_PLLSAI1) || \
   ((__SOURCE__) == RCC_SAI1CLKSOURCE_PLLSAI2) || \
   ((__SOURCE__) == RCC_SAI1CLKSOURCE_PLL)     || \
   ((__SOURCE__) == RCC_SAI1CLKSOURCE_PIN)     || \
   ((__SOURCE__) == RCC_SAI1CLKSOURCE_HSI))

#define IS_RCC_SAI2CLK(__SOURCE__)   \
  (((__SOURCE__) == RCC_SAI2CLKSOURCE_PLLSAI1) || \
   ((__SOURCE__) == RCC_SAI2CLKSOURCE_PLLSAI2) || \
   ((__SOURCE__) == RCC_SAI2CLKSOURCE_PLL)     || \
   ((__SOURCE__) == RCC_SAI2CLKSOURCE_PIN)     || \
   ((__SOURCE__) == RCC_SAI2CLKSOURCE_HSI))

#define IS_RCC_LPTIM1CLK(__SOURCE__)  \
  (((__SOURCE__) == RCC_LPTIM1CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_LPTIM1CLKSOURCE_LSI)   || \
   ((__SOURCE__) == RCC_LPTIM1CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_LPTIM1CLKSOURCE_LSE))

#define IS_RCC_LPTIM2CLK(__SOURCE__)  \
  (((__SOURCE__) == RCC_LPTIM2CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_LPTIM2CLKSOURCE_LSI)   || \
   ((__SOURCE__) == RCC_LPTIM2CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_LPTIM2CLKSOURCE_LSE))

#define IS_RCC_LPTIM3CLK(__SOURCE__)  \
  (((__SOURCE__) == RCC_LPTIM3CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_LPTIM3CLKSOURCE_LSI)   || \
   ((__SOURCE__) == RCC_LPTIM3CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_LPTIM3CLKSOURCE_LSE))

#define IS_RCC_FDCANCLK(__SOURCE__)  \
  (((__SOURCE__) == RCC_FDCANCLKSOURCE_HSE)   || \
   ((__SOURCE__) == RCC_FDCANCLKSOURCE_PLL)   || \
   ((__SOURCE__) == RCC_FDCANCLKSOURCE_PLLSAI1))

#define IS_RCC_SDMMC1CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_SDMMC1CLKSOURCE_HSI48)   || \
   ((__SOURCE__) == RCC_SDMMC1CLKSOURCE_PLLSAI1) || \
   ((__SOURCE__) == RCC_SDMMC1CLKSOURCE_PLL)     || \
   ((__SOURCE__) == RCC_SDMMC1CLKSOURCE_MSI)     || \
   ((__SOURCE__) == RCC_SDMMC1CLKSOURCE_PLLP))

#define IS_RCC_RNGCLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_RNGCLKSOURCE_HSI48)   || \
   ((__SOURCE__) == RCC_RNGCLKSOURCE_PLLSAI1) || \
   ((__SOURCE__) == RCC_RNGCLKSOURCE_PLL)     || \
   ((__SOURCE__) == RCC_RNGCLKSOURCE_MSI))

#if  defined(USB)

#define IS_RCC_USBCLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_USBCLKSOURCE_HSI48)   || \
   ((__SOURCE__) == RCC_USBCLKSOURCE_PLLSAI1) || \
   ((__SOURCE__) == RCC_USBCLKSOURCE_PLL)     || \
   ((__SOURCE__) == RCC_USBCLKSOURCE_MSI))

#endif /* USB */

#define IS_RCC_ADCCLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_ADCCLKSOURCE_NONE)    || \
   ((__SOURCE__) == RCC_ADCCLKSOURCE_PLLSAI1) || \
   ((__SOURCE__) == RCC_ADCCLKSOURCE_SYSCLK))

#define IS_RCC_DFSDM1CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_DFSDM1CLKSOURCE_PCLK2) || \
   ((__SOURCE__) == RCC_DFSDM1CLKSOURCE_SYSCLK))

#define IS_RCC_DFSDM1AUDIOCLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_DFSDM1AUDIOCLKSOURCE_SAI1) || \
   ((__SOURCE__) == RCC_DFSDM1AUDIOCLKSOURCE_HSI) || \
   ((__SOURCE__) == RCC_DFSDM1AUDIOCLKSOURCE_MSI))

#define IS_RCC_OSPICLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_OSPICLKSOURCE_SYSCLK) || \
   ((__SOURCE__) == RCC_OSPICLKSOURCE_MSI) || \
   ((__SOURCE__) == RCC_OSPICLKSOURCE_PLL))

#define IS_RCC_PLLSAI1SOURCE(__VALUE__)    IS_RCC_PLLSOURCE(__VALUE__)

#define IS_RCC_PLLSAI1M_VALUE(__VALUE__)   ((1U <= (__VALUE__)) && ((__VALUE__) <= 16U))

#define IS_RCC_PLLSAI1N_VALUE(__VALUE__)   ((8U <= (__VALUE__)) && ((__VALUE__) <= 86U))

#define IS_RCC_PLLSAI1P_VALUE(__VALUE__)   (((__VALUE__) >= 2U) && ((__VALUE__) <= 31U))

#define IS_RCC_PLLSAI1Q_VALUE(__VALUE__)   (((__VALUE__) == 2U) || ((__VALUE__) == 4U) || \
                                            ((__VALUE__) == 6U) || ((__VALUE__) == 8U))

#define IS_RCC_PLLSAI1R_VALUE(__VALUE__)   (((__VALUE__) == 2U) || ((__VALUE__) == 4U) || \
                                            ((__VALUE__) == 6U) || ((__VALUE__) == 8U))

#define IS_RCC_PLLSAI2SOURCE(__VALUE__)    IS_RCC_PLLSOURCE(__VALUE__)

#define IS_RCC_PLLSAI2M_VALUE(__VALUE__)   ((1U <= (__VALUE__)) && ((__VALUE__) <= 16U))

#define IS_RCC_PLLSAI2N_VALUE(__VALUE__)   ((8U <= (__VALUE__)) && ((__VALUE__) <= 86U))

#define IS_RCC_PLLSAI2P_VALUE(__VALUE__)   (((__VALUE__) >= 2U) && ((__VALUE__) <= 31U))

#if defined(CRS)

#define IS_RCC_CRS_SYNC_SOURCE(__SOURCE__) (((__SOURCE__) == RCC_CRS_SYNC_SOURCE_GPIO) || \
                                            ((__SOURCE__) == RCC_CRS_SYNC_SOURCE_LSE)  || \
                                            ((__SOURCE__) == RCC_CRS_SYNC_SOURCE_USB))

#define IS_RCC_CRS_SYNC_DIV(__DIV__)       (((__DIV__) == RCC_CRS_SYNC_DIV1)  || ((__DIV__) == RCC_CRS_SYNC_DIV2)  || \
                                            ((__DIV__) == RCC_CRS_SYNC_DIV4)  || ((__DIV__) == RCC_CRS_SYNC_DIV8)  || \
                                            ((__DIV__) == RCC_CRS_SYNC_DIV16) || ((__DIV__) == RCC_CRS_SYNC_DIV32) || \
                                            ((__DIV__) == RCC_CRS_SYNC_DIV64) || ((__DIV__) == RCC_CRS_SYNC_DIV128))

#define IS_RCC_CRS_SYNC_POLARITY(__POLARITY__) (((__POLARITY__) == RCC_CRS_SYNC_POLARITY_RISING) || \
                                                ((__POLARITY__) == RCC_CRS_SYNC_POLARITY_FALLING))

#define IS_RCC_CRS_RELOADVALUE(__VALUE__)  (((__VALUE__) <= 0xFFFFU))

#define IS_RCC_CRS_ERRORLIMIT(__VALUE__)   (((__VALUE__) <= 0xFFU))

#define IS_RCC_CRS_HSI48CALIBRATION(__VALUE__) (((__VALUE__) <= 0x7FU))

#define IS_RCC_CRS_FREQERRORDIR(__DIR__)   (((__DIR__) == RCC_CRS_FREQERRORDIR_UP) || \
                                            ((__DIR__) == RCC_CRS_FREQERRORDIR_DOWN))

#endif /* CRS */

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
#endif

#endif /* STM32L5xx_HAL_RCC_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
