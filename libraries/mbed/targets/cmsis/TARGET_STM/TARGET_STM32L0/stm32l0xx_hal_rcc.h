/**
  ******************************************************************************
  * @file    stm32l0xx_hal_rcc.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-June-2014
  * @brief   Header file of RCC HAL module.
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
#ifndef __STM32L0xx_HAL_RCC_H
#define __STM32L0xx_HAL_RCC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal_def.h"

/** @addtogroup STM32L0xx_HAL_Driver
  * @{
  */

/** @addtogroup RCC
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  RCC PLL configuration structure definition
  */
typedef struct
{
  uint32_t PLLState;   /*!< The new state of the PLL.
                            This parameter can be a value of @ref RCC_PLL_Config */

  uint32_t PLLSource;  /*!< RCC_PLLSource: PLL entry clock source.
                            This parameter must be a value of @ref RCC_PLL_Clock_Source */

  uint32_t PLLMUL;       /*!< PLLMUL: Multiplication factor for PLL VCO output clock
                              This parameter must of @ref RCC_PLLMultiplication_Factor */

  uint32_t PLLDIV;       /*!< PLLDIV: Division factor for main system clock (SYSCLK)
                            This parameter must be a value of @ref RCC_PLLDivider_Factor */

}RCC_PLLInitTypeDef;

/**
  * @brief  RCC Internal/External Oscillator (HSE, HSI, LSE and LSI) configuration structure definition  
  */
typedef struct
{
  uint32_t OscillatorType;       /*!< The oscillators to be configured.
                                      This parameter can be a value of @ref RCC_Oscillator_Type */

  uint32_t HSEState;             /*!< The new state of the HSE.
                                      This parameter can be a value of @ref RCC_HSE_Config */

  uint32_t LSEState;             /*!< The new state of the LSE.
                                      This parameter can be a value of @ref RCC_LSE_Config */

  uint32_t HSIState;             /*!< The new state of the HSI.
                                      This parameter can be a value of @ref RCC_HSI_Config */

  uint32_t HSICalibrationValue;  /*!< The calibration trimming value.
                                      This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x1F */

  uint32_t LSIState;             /*!< The new state of the LSI.
                                      This parameter can be a value of @ref RCC_LSI_Config */

  uint32_t HSI48State;             /*!< The new state of the HSI48.
                                      This parameter can be a value of @ref RCC_HSI48_Config */

  uint32_t MSIState;             /*!< The new state of the MSI.
                                      This parameter can be a value of @ref RCC_MSI_Config */

  uint32_t MSICalibrationValue;  /*!< The calibration trimming value.
                                      This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF */

  uint32_t MSIClockRange;         /*!< The MSI  frequency  range.
                                      This parameter can be a value of @ref RCC_MSI_Clock_Range  */

  RCC_PLLInitTypeDef PLL;        /*!< PLL structure parameters */

}RCC_OscInitTypeDef;

/**
  * @brief  RCC System, AHB and APB busses clock configuration structure definition  
  */
typedef struct
{
  uint32_t ClockType;             /*!< The clock to be configured.
                                       This parameter can be a value of @ref RCC_System_Clock_Type */

  uint32_t SYSCLKSource;          /*!< The clock source (SYSCLKS) used as system clock.
                                       This parameter can be a value of @ref RCC_System_Clock_Source */

  uint32_t AHBCLKDivider;         /*!< The AHB clock (HCLK) divider. This clock is derived from the system clock (SYSCLK).
                                       This parameter can be a value of @ref RCC_AHB_Clock_Source */

  uint32_t APB1CLKDivider;        /*!< The APB1 clock (PCLK1) divider. This clock is derived from the AHB clock (HCLK).
                                       This parameter can be a value of @ref RCC_APB1_APB2_Clock_Source */

  uint32_t APB2CLKDivider;        /*!< The APB2 clock (PCLK2) divider. This clock is derived from the AHB clock (HCLK).
                                       This parameter can be a value of @ref RCC_APB1_APB2_Clock_Source */

}RCC_ClkInitTypeDef;

/* Exported constants --------------------------------------------------------*/
/** @defgroup RCC_Exported_Constants
  * @{
  */

/** @defgroup RCC_BitAddress_AliasRegion
  * @brief RCC registers bit address in the alias region
  * @{
  */
#define RCC_OFFSET                (RCC_BASE - PERIPH_BASE)
/* --- CR Register ---*/
/* Alias word address of HSION bit */
#define RCC_CR_OFFSET             (RCC_OFFSET + 0x00)
/* --- CFGR Register ---*/
/* Alias word address of I2SSRC bit */
#define RCC_CFGR_OFFSET           (RCC_OFFSET + 0x08)
/* --- CSR Register ---*/
#define RCC_CSR_OFFSET            (RCC_OFFSET + 0x74)

/* CR register byte 3 (Bits[23:16]) base address */
#define CR_BYTE2_ADDRESS          ((uint32_t)0x40023802)

/* CIER register byte 0 (Bits[0:8]) base address */
#define CIER_BYTE0_ADDRESS         ((uint32_t)(RCC_BASE + 0x10 + 0x00))

#define LSE_TIMEOUT_VALUE          LSE_STARTUP_TIMEOUT
#define DBP_TIMEOUT_VALUE          ((uint32_t)100)  /* 100 ms */

/**
  * @}
  */

/** @defgroup RCC_Oscillator_Type
  * @{
  */
#define RCC_OSCILLATORTYPE_NONE            ((uint32_t)0x00000000)
#define RCC_OSCILLATORTYPE_HSE             ((uint32_t)0x00000001)
#define RCC_OSCILLATORTYPE_HSI             ((uint32_t)0x00000002)
#define RCC_OSCILLATORTYPE_LSE             ((uint32_t)0x00000004)
#define RCC_OSCILLATORTYPE_LSI             ((uint32_t)0x00000008)
#define RCC_OSCILLATORTYPE_MSI             ((uint32_t)0x00000010)
#define RCC_OSCILLATORTYPE_HSI48           ((uint32_t)0x00000020)

#define IS_RCC_OSCILLATORTYPE(OSCILLATOR) ((OSCILLATOR) <= 0x3F)

/**
  * @}
  */

/** @defgroup RCC_HSE_Config
  * @{
  */
#define RCC_HSE_OFF                     ((uint32_t)0x00000000)
#define RCC_HSE_ON                      RCC_CR_HSEON
#define RCC_HSE_BYPASS                  ((uint32_t)(RCC_CR_HSEBYP | RCC_CR_HSEON))

#define IS_RCC_HSE(HSE) (((HSE) == RCC_HSE_OFF) || ((HSE) == RCC_HSE_ON) || \
                         ((HSE) == RCC_HSE_BYPASS))
/**
  * @}
  */

/** @defgroup RCC_LSE_Config
  * @{
  */
#define RCC_LSE_OFF                      ((uint32_t)0x00000000)
#define RCC_LSE_ON                       RCC_CSR_LSEON
#define RCC_LSE_BYPASS                   ((uint32_t)(RCC_CSR_LSEBYP | RCC_CSR_LSEON))

#define IS_RCC_LSE(LSE) (((LSE) == RCC_LSE_OFF) || ((LSE) == RCC_LSE_ON) || \
                         ((LSE) == RCC_LSE_BYPASS))
/**
  * @}
  */

/** @defgroup RCC_HSI_Config
  * @{
  */
#define RCC_HSI_OFF                      ((uint8_t)0x00)
#define RCC_HSI_ON                       ((uint8_t)0x01)
#define RCC_HSI_DIV4                     ((uint8_t)0x09)
#define IS_RCC_HSI(HSI) (((HSI) == RCC_HSI_OFF) || ((HSI) == RCC_HSI_ON) || \
                         ((HSI) == RCC_HSI_DIV4))

/**
  * @}
  */ 

/** @defgroup RCC_MSI_Clock_Range
  * @{
  */

#define RCC_MSIRANGE_0                   RCC_ICSCR_MSIRANGE_0 /*!< MSI = 65.536 KHz  */
#define RCC_MSIRANGE_1                   RCC_ICSCR_MSIRANGE_1 /*!< MSI = 131.072 KHz */
#define RCC_MSIRANGE_2                   RCC_ICSCR_MSIRANGE_2 /*!< MSI = 262.144 KHz */
#define RCC_MSIRANGE_3                   RCC_ICSCR_MSIRANGE_3 /*!< MSI = 524.288 KHz */
#define RCC_MSIRANGE_4                   RCC_ICSCR_MSIRANGE_4 /*!< MSI = 1.048 MHz   */
#define RCC_MSIRANGE_5                   RCC_ICSCR_MSIRANGE_5 /*!< MSI = 2.097 MHz   */
#define RCC_MSIRANGE_6                   RCC_ICSCR_MSIRANGE_6 /*!< MSI = 4.194 MHz   */

#define IS_RCC_MSI_CLOCK_RANGE(RANGE) (((RANGE) == RCC_MSIRANGE_0) || \
                                       ((RANGE) == RCC_MSIRANGE_1) || \
                                       ((RANGE) == RCC_MSIRANGE_2) || \
                                       ((RANGE) == RCC_MSIRANGE_3) || \
                                       ((RANGE) == RCC_MSIRANGE_4) || \
                                       ((RANGE) == RCC_MSIRANGE_5) || \
                                       ((RANGE) == RCC_MSIRANGE_6))

/**
  * @}
  */ 

/** @defgroup RCC_LSI_Config
  * @{
  */
#define RCC_LSI_OFF                      ((uint8_t)0x00)
#define RCC_LSI_ON                       ((uint8_t)0x01)

#define IS_RCC_LSI(LSI) (((LSI) == RCC_LSI_OFF) || ((LSI) == RCC_LSI_ON))
/**
  * @}
  */

    
/** @defgroup RCC_MSI_Config
  * @{
  */
#define RCC_MSI_OFF                      ((uint8_t)0x00)
#define RCC_MSI_ON                       ((uint8_t)0x01)

#define IS_RCC_MSI(MSI) (((MSI) == RCC_MSI_OFF) || ((MSI) == RCC_MSI_ON))
/**
  * @}
  */

/** @defgroup RCC_HSI48_Config
  * @{
  */
#define RCC_HSI48_OFF                      ((uint8_t)0x00)
#define RCC_HSI48_ON                       ((uint8_t)0x01)

#define IS_RCC_HSI48(HSI48) (((HSI48) == RCC_HSI48_OFF) || ((HSI48) == RCC_HSI48_ON))
/**
  * @}
  */

/** @defgroup RCC_PLL_Config
  * @{
  */
#define RCC_PLL_NONE                      ((uint8_t)0x00)
#define RCC_PLL_OFF                       ((uint8_t)0x01)
#define RCC_PLL_ON                        ((uint8_t)0x02)

#define IS_RCC_PLL(PLL) (((PLL) == RCC_PLL_NONE) ||((PLL) == RCC_PLL_OFF) || ((PLL) == RCC_PLL_ON))
/**
  * @}
  */

/** @defgroup RCC_PLL_Clock_Source
  * @{
  */
#define RCC_PLLSOURCE_HSI                RCC_CFGR_PLLSRC_HSI
#define RCC_PLLSOURCE_HSE                RCC_CFGR_PLLSRC_HSE

#define IS_RCC_PLLSOURCE(SOURCE) (((SOURCE) == RCC_PLLSOURCE_HSI) || \
                                  ((SOURCE) == RCC_PLLSOURCE_HSE))

/**
  * @}
  */

/** @defgroup RCC_PLLMultiplication_Factor
  * @{
  */

#define RCC_PLLMUL_3                     RCC_CFGR_PLLMUL3
#define RCC_PLLMUL_4                     RCC_CFGR_PLLMUL4
#define RCC_PLLMUL_6                     RCC_CFGR_PLLMUL6
#define RCC_PLLMUL_8                     RCC_CFGR_PLLMUL8
#define RCC_PLLMUL_12                    RCC_CFGR_PLLMUL12
#define RCC_PLLMUL_16                    RCC_CFGR_PLLMUL16
#define RCC_PLLMUL_24                    RCC_CFGR_PLLMUL24
#define RCC_PLLMUL_32                    RCC_CFGR_PLLMUL32
#define RCC_PLLMUL_48                    RCC_CFGR_PLLMUL48
#define IS_RCC_PLL_MUL(MUL) (((MUL) == RCC_PLLMUL_3) || ((MUL) == RCC_PLLMUL_4) || \
                             ((MUL) == RCC_PLLMUL_6) || ((MUL) == RCC_PLLMUL_8) || \
                             ((MUL) == RCC_PLLMUL_12) || ((MUL) == RCC_PLLMUL_16) || \
                             ((MUL) == RCC_PLLMUL_24) || ((MUL) == RCC_PLLMUL_32) || \
                             ((MUL) == RCC_PLLMUL_48))
/**
  * @}
  */

/** @defgroup RCC_PLLDivider_Factor
  * @{
  */

#define RCC_PLLDIV_2                     RCC_CFGR_PLLDIV2
#define RCC_PLLDIV_3                     RCC_CFGR_PLLDIV3
#define RCC_PLLDIV_4                     RCC_CFGR_PLLDIV4
#define IS_RCC_PLL_DIV(DIV) (((DIV) == RCC_PLLDIV_2) || ((DIV) == RCC_PLLDIV_3) || \
                             ((DIV) == RCC_PLLDIV_4))
/**
  * @}
  */

/** @defgroup RCC_System_Clock_Type
  * @{
  */
#define RCC_CLOCKTYPE_SYSCLK             ((uint32_t)0x00000001)
#define RCC_CLOCKTYPE_HCLK               ((uint32_t)0x00000002)
#define RCC_CLOCKTYPE_PCLK1              ((uint32_t)0x00000004)
#define RCC_CLOCKTYPE_PCLK2              ((uint32_t)0x00000008)

#define IS_RCC_CLOCKTYPE(CLK) ((1 <= (CLK)) && ((CLK) <= 15))
/**
  * @}
  */
  
/** @defgroup RCC_System_Clock_Source
  * @{
  */
#define RCC_SYSCLKSOURCE_MSI             RCC_CFGR_SW_MSI
#define RCC_SYSCLKSOURCE_HSI             RCC_CFGR_SW_HSI
#define RCC_SYSCLKSOURCE_HSE             RCC_CFGR_SW_HSE
#define RCC_SYSCLKSOURCE_PLLCLK          RCC_CFGR_SW_PLL

#define IS_RCC_SYSCLKSOURCE(SOURCE) (((SOURCE) == RCC_SYSCLKSOURCE_HSI) || \
                                     ((SOURCE) == RCC_SYSCLKSOURCE_HSE) || \
                                     ((SOURCE) == RCC_SYSCLKSOURCE_MSI) || \
                                     ((SOURCE) == RCC_SYSCLKSOURCE_PLLCLK))
/**
  * @}
  */ 

/** @defgroup RCC_AHB_Clock_Source
  * @{
  */
#define RCC_SYSCLK_DIV1                  RCC_CFGR_HPRE_DIV1
#define RCC_SYSCLK_DIV2                  RCC_CFGR_HPRE_DIV2
#define RCC_SYSCLK_DIV4                  RCC_CFGR_HPRE_DIV4
#define RCC_SYSCLK_DIV8                  RCC_CFGR_HPRE_DIV8
#define RCC_SYSCLK_DIV16                 RCC_CFGR_HPRE_DIV16
#define RCC_SYSCLK_DIV64                 RCC_CFGR_HPRE_DIV64
#define RCC_SYSCLK_DIV128                RCC_CFGR_HPRE_DIV128
#define RCC_SYSCLK_DIV256                RCC_CFGR_HPRE_DIV256
#define RCC_SYSCLK_DIV512                RCC_CFGR_HPRE_DIV512

#define IS_RCC_HCLK(HCLK) (((HCLK) == RCC_SYSCLK_DIV1)   || ((HCLK) == RCC_SYSCLK_DIV2)   || \
                           ((HCLK) == RCC_SYSCLK_DIV4)   || ((HCLK) == RCC_SYSCLK_DIV8)   || \
                           ((HCLK) == RCC_SYSCLK_DIV16)  || ((HCLK) == RCC_SYSCLK_DIV64)  || \
                           ((HCLK) == RCC_SYSCLK_DIV128) || ((HCLK) == RCC_SYSCLK_DIV256) || \
                           ((HCLK) == RCC_SYSCLK_DIV512))
/**
  * @}
  */ 
  
/** @defgroup RCC_APB1_APB2_Clock_Source
  * @{
  */
#define RCC_HCLK_DIV1                    RCC_CFGR_PPRE1_DIV1
#define RCC_HCLK_DIV2                    RCC_CFGR_PPRE1_DIV2
#define RCC_HCLK_DIV4                    RCC_CFGR_PPRE1_DIV4
#define RCC_HCLK_DIV8                    RCC_CFGR_PPRE1_DIV8
#define RCC_HCLK_DIV16                   RCC_CFGR_PPRE1_DIV16

#define IS_RCC_PCLK(PCLK) (((PCLK) == RCC_HCLK_DIV1) || ((PCLK) == RCC_HCLK_DIV2) || \
                           ((PCLK) == RCC_HCLK_DIV4) || ((PCLK) == RCC_HCLK_DIV8) || \
                           ((PCLK) == RCC_HCLK_DIV16))
/**
  * @}
  */ 

/** @defgroup RCC_RTC_Clock_Source
  * @{
  */
#define RCC_RTCCLKSOURCE_LSE             RCC_CSR_RTCSEL_LSE
#define RCC_RTCCLKSOURCE_LSI             RCC_CSR_RTCSEL_LSI
#define RCC_RTCCLKSOURCE_HSE_DIV2        RCC_CSR_RTCSEL_HSE
#define RCC_RTCCLKSOURCE_HSE_DIV4        ((uint32_t)RCC_CSR_RTCSEL_HSE | RCC_CR_RTCPRE_0)
#define RCC_RTCCLKSOURCE_HSE_DIV8        ((uint32_t)RCC_CSR_RTCSEL_HSE | RCC_CR_RTCPRE_1)
#define RCC_RTCCLKSOURCE_HSE_DIV16       ((uint32_t)RCC_CSR_RTCSEL_HSE | RCC_CR_RTCPRE)
#define IS_RCC_RTCCLK_SOURCE(SOURCE) (((SOURCE) == RCC_RTCCLKSOURCE_LSE) || \
                                      ((SOURCE) == RCC_RTCCLKSOURCE_LSI) || \
                                      ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV2) || \
                                      ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV4) || \
                                      ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV8) || \
                                      ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV16))
/**
  * @}
  */

/** @defgroup RCC_MCO_Clock_Source
  * @{
  */
#define RCC_MCO1SOURCE_NOCLOCK            ((uint8_t)0x00)
#define RCC_MCO1SOURCE_SYSCLK             ((uint8_t)0x01)
#define RCC_MCO1SOURCE_HSI                ((uint8_t)0x02)
#define RCC_MCO1SOURCE_MSI                ((uint8_t)0x03)
#define RCC_MCO1SOURCE_HSE                ((uint8_t)0x04)
#define RCC_MCO1SOURCE_PLLCLK             ((uint8_t)0x05)
#define RCC_MCO1SOURCE_LSI                ((uint8_t)0x06)
#define RCC_MCO1SOURCE_LSE                ((uint8_t)0x07)
#define RCC_MCO1SOURCE_HSI48              ((uint8_t)0x08)

#define IS_RCC_MCO1SOURCE(SOURCE) (((SOURCE) == RCC_MCO1SOURCE_NOCLOCK) || ((SOURCE) == RCC_MCO1SOURCE_SYSCLK) || \
                                   ((SOURCE) == RCC_MCO1SOURCE_HSI)  || ((SOURCE) == RCC_MCO1SOURCE_MSI) || \
                                   ((SOURCE) == RCC_MCO1SOURCE_HSE)  || ((SOURCE) == RCC_MCO1SOURCE_PLLCLK) || \
                                   ((SOURCE) == RCC_MCO1SOURCE_LSI) || ((SOURCE) == RCC_MCO1SOURCE_LSE) || \
                                   ((SOURCE) == RCC_MCO1SOURCE_HSI48))
/**
  * @}
  */

/** @defgroup RCC_MCOPrescaler
  * @{
  */

#define RCC_MCODIV_1            RCC_CFGR_MCO_PRE_1
#define RCC_MCODIV_2            RCC_CFGR_MCO_PRE_2
#define RCC_MCODIV_4            RCC_CFGR_MCO_PRE_4
#define RCC_MCODIV_8            RCC_CFGR_MCO_PRE_8
#define RCC_MCODIV_16           RCC_CFGR_MCO_PRE_16

#define IS_RCC_MCODIV(DIV) (((DIV) == RCC_MCODIV_1)  || \
                            ((DIV) == RCC_MCODIV_2)  || \
                            ((DIV) == RCC_MCODIV_4)  || \
                            ((DIV) == RCC_MCODIV_8)  || \
                            ((DIV) == RCC_MCODIV_16))
/**
  * @}
  */  

/** @defgroup RCC_MCO_Index
  * @{
  */
#define RCC_MCO1                         ((uint32_t)0x00000000)
#define RCC_MCO2                         ((uint32_t)0x00000001)

#define IS_RCC_MCO(MCOx) (((MCOx) == RCC_MCO1) || ((MCOx) == RCC_MCO2))
/**
  * @}
  */

/** @defgroup RCC_Interrupt
  * @{
  */
#define RCC_IT_LSIRDY                    RCC_CIFR_LSIRDYF
#define RCC_IT_LSERDY                    RCC_CIFR_LSERDYF
#define RCC_IT_HSIRDY                    RCC_CIFR_HSIRDYF
#define RCC_IT_HSERDY                    RCC_CIFR_HSERDYF
#define RCC_IT_PLLRDY                    RCC_CIFR_PLLRDYF
#define RCC_IT_MSIRDY                    RCC_CIFR_MSIRDYF
#define RCC_IT_HSI48RDY                  RCC_CIFR_HSI48RDYF
#define RCC_IT_LSECSS                    RCC_CIFR_LSECSSF
#define RCC_IT_CSS                       RCC_CIFR_CSSF

#define IS_RCC_IT(IT) (((IT) == RCC_IT_LSIRDY) || ((IT) == RCC_IT_LSERDY) || \
                           ((IT) == RCC_IT_HSIRDY) || ((IT) == RCC_IT_HSERDY) || \
                           ((IT) == RCC_IT_PLLRDY) || ((IT) == RCC_IT_MSIRDY) || \
                           ((IT) == RCC_IT_HSI48RDY)  || ((IT) == RCC_IT_LSECSS))

#define IS_RCC_GET_IT(IT) (((IT) == RCC_IT_LSIRDY) || ((IT) == RCC_IT_LSERDY) || \
                           ((IT) == RCC_IT_HSIRDY) || ((IT) == RCC_IT_HSERDY) || \
                           ((IT) == RCC_IT_PLLRDY) || ((IT) == RCC_IT_MSIRDY) || \
                           ((IT) == RCC_IT_CSS)  || ((IT) == RCC_IT_HSI48RDY) || \
                           ((IT) == RCC_IT_LSECSS))

#define IS_RCC_CLEAR_IT(IT) (((IT) == RCC_IT_LSIRDY) || ((IT) == RCC_IT_LSERDY) || \
                           ((IT) == RCC_IT_HSIRDY) || ((IT) == RCC_IT_HSERDY) || \
                           ((IT) == RCC_IT_PLLRDY) || ((IT) == RCC_IT_MSIRDY) || \
                           ((IT) == RCC_IT_CSS)  || ((IT) == RCC_IT_HSI48RDY) || \
                           ((IT) == RCC_IT_LSECSS))

/**
  * @}
  */
  
/** @defgroup RCC_Flag
  *        Elements values convention: 0XXYYYYYb
  *           - YYYYY  : Flag position in the register
  *           - 0XX  : Register index
  *                 - 01: CR register
  *                 - 10: CSR register
  *                 - 11: CRRCR register
  * @{
  */
/* Flags in the CR register */
#define RCC_FLAG_HSIRDY                  ((uint8_t)0x22)
#define RCC_FLAG_HSIDIV                  ((uint8_t)0x24)
#define RCC_FLAG_MSIRDY                  ((uint8_t)0x29)
#define RCC_FLAG_HSERDY                  ((uint8_t)0x31)
#define RCC_FLAG_PLLRDY                  ((uint8_t)0x39)

/* Flags in the CSR register */
#define RCC_FLAG_LSERDY                  ((uint8_t)0x49)
#define RCC_FLAG_LSECSS                  ((uint8_t)0x4E)
#define RCC_FLAG_LSIRDY                  ((uint8_t)0x41)
#define RCC_FLAG_FIREWALLRST             ((uint8_t)0x58)
#define RCC_FLAG_OBLRST                  ((uint8_t)0x59)
#define RCC_FLAG_PINRST                  ((uint8_t)0x5A)
#define RCC_FLAG_PORRST                  ((uint8_t)0x5B)
#define RCC_FLAG_SFTRST                  ((uint8_t)0x5C)
#define RCC_FLAG_IWDGRST                 ((uint8_t)0x5D)
#define RCC_FLAG_WWDGRST                 ((uint8_t)0x5E)
#define RCC_FLAG_LPWRRST                 ((uint8_t)0x5F)

/* Flags in the CRRCR register */
#define RCC_FLAG_HSI48RDY                ((uint8_t)0x61)



#define IS_RCC_CALIBRATION_VALUE(VALUE) ((VALUE) <= 0x1F)
#define IS_RCC_MSICALIBRATION_VALUE(VALUE) ((VALUE) <= 0xFF)
/**
  * @}
  */ 

/**
  * @}
  */   
/* Exported macro ------------------------------------------------------------*/
/** @defgroup RCC_Exported macro
 * @{
 */

/** @brief  Enable or disable the AHB peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.
  */
#define __DMA1_CLK_ENABLE()          (RCC->AHBENR |= (RCC_AHBENR_DMA1EN))
#define __MIF_CLK_ENABLE()           (RCC->AHBENR |= (RCC_AHBENR_MIFEN))
#define __CRC_CLK_ENABLE()           (RCC->AHBENR |= (RCC_AHBENR_CRCEN))


#define __DMA1_CLK_DISABLE()          (RCC->AHBENR  &= ~ (RCC_AHBENR_DMA1EN))
#define __MIF_CLK_DISABLE()           (RCC->AHBENR  &= ~ (RCC_AHBENR_MIFEN))
#define __CRC_CLK_DISABLE()           (RCC->AHBENR  &= ~ (RCC_AHBENR_CRCEN))


/** @brief  Enable or disable the IOPORT peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.
  */
#define __GPIOA_CLK_ENABLE()         (RCC->IOPENR |= (RCC_IOPENR_GPIOAEN))
#define __GPIOB_CLK_ENABLE()         (RCC->IOPENR |= (RCC_IOPENR_GPIOBEN))
#define __GPIOC_CLK_ENABLE()         (RCC->IOPENR |= (RCC_IOPENR_GPIOCEN))
#define __GPIOD_CLK_ENABLE()         (RCC->IOPENR |= (RCC_IOPENR_GPIODEN))
#define __GPIOH_CLK_ENABLE()         (RCC->IOPENR |= (RCC_IOPENR_GPIOHEN))

#define __GPIOA_CLK_DISABLE()        (RCC->IOPENR &= ~(RCC_IOPENR_GPIOAEN))
#define __GPIOB_CLK_DISABLE()        (RCC->IOPENR &= ~(RCC_IOPENR_GPIOBEN))
#define __GPIOC_CLK_DISABLE()        (RCC->IOPENR &= ~(RCC_IOPENR_GPIOCEN))
#define __GPIOD_CLK_DISABLE()        (RCC->IOPENR &= ~(RCC_IOPENR_GPIODEN))
#define __GPIOH_CLK_DISABLE()        (RCC->IOPENR &= ~(RCC_IOPENR_GPIOHEN))


/** @brief  Enable or disable the Low Speed APB (APB1) peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  */
#define __WWDG_CLK_ENABLE()    (RCC->APB1ENR |= (RCC_APB1ENR_WWDGEN))
#define __PWR_CLK_ENABLE()     (RCC->APB1ENR |= (RCC_APB1ENR_PWREN))

#define __WWDG_CLK_DISABLE()    (RCC->APB1ENR &= ~ (RCC_APB1ENR_WWDGEN))
#define __PWR_CLK_DISABLE()     (RCC->APB1ENR &= ~ (RCC_APB1ENR_PWREN))

/** @brief  Enable or disable the High Speed APB (APB2) peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.
  */
#define __SYSCFG_CLK_ENABLE()   (RCC->APB2ENR |= (RCC_APB2ENR_SYSCFGEN))
#define __DBGMCU_CLK_ENABLE()   (RCC->APB2ENR |= (RCC_APB2ENR_DBGMCUEN))

#define __SYSCFG_CLK_DISABLE()   (RCC->APB2ENR &= ~  (RCC_APB2ENR_SYSCFGEN))
#define __DBGMCU_CLK_DISABLE()   (RCC->APB2ENR &= ~  (RCC_APB2ENR_DBGMCUEN))

/** @brief  Force or release AHB peripheral reset.
  */
#define __AHB_FORCE_RESET()     (RCC->AHBRSTR = 0xFFFFFFFF)
#define __DMA1_FORCE_RESET()    (RCC->AHBRSTR |= (RCC_AHBRSTR_DMA1RST))
#define __MIF_FORCE_RESET()     (RCC->AHBRSTR |= (RCC_AHBRSTR_MIFRST))
#define __CRC_FORCE_RESET()     (RCC->AHBRSTR |= (RCC_AHBRSTR_CRCRST))

#define __AHB_RELEASE_RESET()     (RCC->AHBRSTR = 0x00)
#define __CRC_RELEASE_RESET()     (RCC->AHBRSTR &= ~ (RCC_AHBRSTR_CRCRST))
#define __DMA1_RELEASE_RESET()    (RCC->AHBRSTR &= ~ (RCC_AHBRSTR_DMA1RST))
#define __MIF_RELEASE_RESET()     (RCC->AHBRSTR &= ~ (RCC_AHBRSTR_MIFRST))


/** @brief  Force or release IOPORT peripheral reset.
  */
#define __IOP_FORCE_RESET()     (RCC->IOPRSTR = 0xFFFFFFFF) 
#define __GPIOA_FORCE_RESET()   (RCC->IOPRSTR |= (RCC_IOPRSTR_GPIOARST))
#define __GPIOB_FORCE_RESET()   (RCC->IOPRSTR |= (RCC_IOPRSTR_GPIOBRST))
#define __GPIOC_FORCE_RESET()   (RCC->IOPRSTR |= (RCC_IOPRSTR_GPIOCRST))
#define __GPIOD_FORCE_RESET()   (RCC->IOPRSTR |= (RCC_IOPRSTR_GPIODRST))
#define __GPIOH_FORCE_RESET()   (RCC->IOPRSTR |= (RCC_IOPRSTR_GPIOHRST))

#define __IOP_RELEASE_RESET()   (RCC->IOPRSTR = 0x00) 
#define __GPIOA_RELEASE_RESET() (RCC->IOPRSTR &= ~(RCC_IOPRSTR_GPIOARST))
#define __GPIOB_RELEASE_RESET() (RCC->IOPRSTR &= ~(RCC_IOPRSTR_GPIOBRST))
#define __GPIOC_RELEASE_RESET() (RCC->IOPRSTR &= ~(RCC_IOPRSTR_GPIOCRST))
#define __GPIOD_RELEASE_RESET() (RCC->IOPRSTR &= ~(RCC_IOPRSTR_GPIODRST))
#define __GPIOH_RELEASE_RESET() (RCC->IOPRSTR &= ~(RCC_IOPRSTR_GPIOHRST))

/** @brief  Force or release APB1 peripheral reset.
  */
#define __APB1_FORCE_RESET()     (RCC->APB1RSTR = 0xFFFFFFFF)  
#define __WWDG_FORCE_RESET()     (RCC->APB1RSTR |= (RCC_APB1RSTR_WWDGRST))
#define __PWR_FORCE_RESET()      (RCC->APB1RSTR |= (RCC_APB1RSTR_PWRRST))

#define __APB1_RELEASE_RESET()     (RCC->APB1RSTR = 0x00)
#define __WWDG_RELEASE_RESET()     (RCC->APB1RSTR &= ~ (RCC_APB1RSTR_WWDGRST))
#define __PWR_RELEASE_RESET()      (RCC->APB1RSTR &= ~ (RCC_APB1RSTR_PWRRST))

/** @brief  Force or release APB2 peripheral reset.
  */
#define __APB2_FORCE_RESET()       (RCC->APB2RSTR = 0xFFFFFFFF)  
#define __DBGMCU_FORCE_RESET()     (RCC->APB2RSTR |= (RCC_APB2RSTR_DBGMCURST))
#define __SYSCFG_FORCE_RESET()     (RCC->APB2RSTR |= (RCC_APB2RSTR_SYSCFGRST))

#define __APB2_RELEASE_RESET()       (RCC->APB2RSTR = 0x00)
#define __DBGMCU_RELEASE_RESET()     (RCC->APB2RSTR &= ~ (RCC_APB2RSTR_DBGMCURST))
#define __SYSCFG_RELEASE_RESET()     (RCC->APB2RSTR &= ~ (RCC_APB2RSTR_SYSCFGRST))

/** @brief  Enable or disable the AHB peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */
#define __CRC_CLK_SLEEP_ENABLE()           (RCC->AHBSMENR |= (RCC_AHBSMENR_CRCSMEN))
#define __MIF_CLK_SLEEP_ENABLE()           (RCC->AHBSMENR |= (RCC_AHBSMENR_MIFSMEN))
#define __SRAM_CLK_SLEEP_ENABLE()          (RCC->AHBSMENR |= (RCC_AHBSMENR_SRAMSMEN))
#define __DMA1_CLK_SLEEP_ENABLE()          (RCC->AHBSMENR |= (RCC_AHBSMENR_DMA1SMEN))

#define __CRC_CLK_SLEEP_DISABLE()           (RCC->AHBSMENR  &= ~ (RCC_AHBSMENR_CRCSMEN))
#define __MIF_CLK_SLEEP_DISABLE()           (RCC->AHBSMENR  &= ~ (RCC_AHBSMENR_MIFSMEN))
#define __SRAM_CLK_SLEEP_DISABLE()          (RCC->AHBSMENR  &= ~ (RCC_AHBSMENR_SRAMSMEN))
#define __DMA1_CLK_SLEEP_DISABLE()          (RCC->AHBSMENR  &= ~ (RCC_AHBSMENR_DMA1SMEN))

/** @brief  Enable or disable the IOPORT peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */

#define __GPIOA_CLK_SLEEP_ENABLE()         (RCC->IOPSMENR |= (RCC_IOPSMENR_GPIOASMEN))
#define __GPIOB_CLK_SLEEP_ENABLE()         (RCC->IOPSMENR |= (RCC_IOPSMENR_GPIOBSMEN))
#define __GPIOC_CLK_SLEEP_ENABLE()         (RCC->IOPSMENR |= (RCC_IOPSMENR_GPIOCSMEN))
#define __GPIOD_CLK_SLEEP_ENABLE()         (RCC->IOPSMENR |= (RCC_IOPSMENR_GPIODSMEN))
#define __GPIOH_CLK_SLEEP_ENABLE()         (RCC->IOPSMENR |= (RCC_IOPSMENR_GPIOHSMEN))

#define __GPIOA_CLK_SLEEP_DISABLE()        (RCC->IOPSMENR &= ~(RCC_IOPSMENR_GPIOASMEN))
#define __GPIOB_CLK_SLEEP_DISABLE()        (RCC->IOPSMENR &= ~(RCC_IOPSMENR_GPIOBSMEN))
#define __GPIOC_CLK_SLEEP_DISABLE()        (RCC->IOPSMENR &= ~(RCC_IOPSMENR_GPIOCSMEN))
#define __GPIOD_CLK_SLEEP_DISABLE()        (RCC->IOPSMENR &= ~(RCC_IOPSMENR_GPIODSMEN))
#define __GPIOH_CLK_SLEEP_DISABLE()        (RCC->IOPSMENR &= ~(RCC_IOPSMENR_GPIOHSMEN))

/** @brief  Enable or disable the APB1 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */
#define __WWDG_CLK_SLEEP_ENABLE()    (RCC->APB1SMENR |= (RCC_APB1SMENR_WWDGSMEN))
#define __PWR_CLK_SLEEP_ENABLE()     (RCC->APB1SMENR |= (RCC_APB1SMENR_PWRSMEN))

#define __WWDG_CLK_SLEEP_DISABLE()    (RCC->APB1SMENR &= ~ (RCC_APB1SMENR_WWDGSMEN))
#define __PWR_CLK_SLEEP_DISABLE()     (RCC->APB1SMENR &= ~ (RCC_APB1SMENR_PWRSMEN))

/** @brief  Enable or disable the APB2 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */
#define __SYSCFG_CLK_SLEEP_ENABLE()   (RCC->APB2SMENR |= (RCC_APB2SMENR_SYSCFGSMEN))
#define __DBGMCU_CLK_SLEEP_ENABLE()   (RCC->APB2SMENR |= (RCC_APB2SMENR_DBGMCUSMEN))

#define __SYSCFG_CLK_SLEEP_DISABLE()   (RCC->APB2SMENR &= ~  (RCC_APB2SMENR_SYSCFGSMEN))
#define __DBGMCU_CLK_SLEEP_DISABLE()   (RCC->APB2SMENR &= ~  (RCC_APB2SMENR_DBGMCUSMEN))

/** @brief  Macro to enable or disable the Internal High Speed oscillator (HSI).
  * @note     After enabling the HSI, the application software should wait on 
  *           HSIRDY flag to be set indicating that HSI clock is stable and can
  *           be used to clock the PLL and/or system clock.
  * @note     HSI can not be stopped if it is used directly or through the PLL
  *           as system clock. In this case, you have to select another source 
  *           of the system clock then stop the HSI.
  * @note     The HSI is stopped by hardware when entering STOP and STANDBY modes. 
  * @param    __STATE__: specifies the new state of the HSI.
  *           This parameter can be one of the following values:
  *            @arg RCC_HSI_OFF: turn OFF the HSI oscillator
  *            @arg RCC_HSI_ON: turn ON the HSI oscillator
  *            @arg RCC_HSI_DIV4: turn ON the HSI oscillator and divide it by 4
  * @note   When the HSI is stopped, HSIRDY flag goes low after 6 HSI oscillator
  *         clock cycles. 
  */
#define __HAL_RCC_HSI_CONFIG(__STATE__) \
                  MODIFY_REG(RCC->CR, RCC_CR_HSION|RCC_CR_HSIDIVEN, (uint32_t)(__STATE__))

/** @brief  Macros to enable or disable the Internal High Speed oscillator (HSI).
  * @note   The HSI is stopped by hardware when entering STOP and STANDBY modes.
  *         It is used (enabled by hardware) as system clock source after startup
  *         from Reset, wakeup from STOP and STANDBY mode, or in case of failure
  *         of the HSE used directly or indirectly as system clock (if the Clock
  *         Security System CSS is enabled).
  * @note   HSI can not be stopped if it is used as system clock source. In this case,
  *         you have to select another source of the system clock then stop the HSI.  
  * @note   After enabling the HSI, the application software should wait on HSIRDY
  *         flag to be set indicating that HSI clock is stable and can be used as
  *         system clock source.
  * @note   When the HSI is stopped, HSIRDY flag goes low after 6 HSI oscillator
  *         clock cycles.  
  */
#define __HAL_RCC_HSI_ENABLE()  SET_BIT(RCC->CR, RCC_CR_HSION)
#define __HAL_RCC_HSI_DISABLE() CLEAR_BIT(RCC->CR, RCC_CR_HSION)

/**
  * @brief  Macros to enable or disable the Internal Multi Speed oscillator (MSI).
  * @note     The MSI is stopped by hardware when entering STOP and STANDBY modes.
  *           It is used (enabled by hardware) as system clock source after
  *           startup from Reset, wakeup from STOP and STANDBY mode, or in case
  *           of failure of the HSE used directly or indirectly as system clock
  *           (if the Clock Security System CSS is enabled).
  * @note     MSI can not be stopped if it is used as system clock source.
  *           In this case, you have to select another source of the system
  *           clock then stop the MSI.
  * @note     After enabling the MSI, the application software should wait on
  *           MSIRDY flag to be set indicating that MSI clock is stable and can
  *           be used as system clock source.
  * @note   When the MSI is stopped, MSIRDY flag goes low after 6 MSI oscillator
  *         clock cycles.
  */
#define __HAL_RCC_MSI_ENABLE()  SET_BIT(RCC->CR, RCC_CR_MSION)
#define __HAL_RCC_MSI_DISABLE() CLEAR_BIT(RCC->CR, RCC_CR_MSION)

/**
  * @brief  Macro to enable or disable the Internal High Speed oscillator for USB (HSI48).
  * @note   After enabling the HSI48, the application software should wait on 
  *         HSI48RDY flag to be set indicating that HSI48 clock is stable and can
  *         be used to clock the USB.
  * @note   The HSI48 is stopped by hardware when entering STOP and STANDBY modes.
  */
#define __HAL_RCC_HSI48_ENABLE()  do { SET_BIT(RCC->CRRCR, RCC_CRRCR_HSI48ON);   \
                                                    RCC->APB2ENR |=  RCC_APB2ENR_SYSCFGEN; \
                                                    SYSCFG->CFGR3 |= (SYSCFG_CFGR3_ENREF_HSI48 | SYSCFG_CFGR3_EN_VREFINT);  \
                                                   } while (0)
#define __HAL_RCC_HSI48_DISABLE()  do { CLEAR_BIT(RCC->CRRCR, RCC_CRRCR_HSI48ON);   \
                                                    SYSCFG->CFGR3 &= (uint32_t)~((uint32_t)(SYSCFG_CFGR3_ENREF_HSI48 | SYSCFG_CFGR3_EN_VREFINT));  \
                                                   } while (0)

/** @brief  Macro to adjust the Internal High Speed oscillator (HSI) calibration value.
  * @note   The calibration is used to compensate for the variations in voltage
  *         and temperature that influence the frequency of the internal HSI RC.
  * @param  __HSICalibrationValue__: specifies the calibration trimming value.
  *         This parameter must be a number between 0 and 0x1F.
  */
#define __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(__HSICalibrationValue__) (MODIFY_REG(RCC->ICSCR,\
        RCC_ICSCR_HSITRIM, (uint32_t)(__HSICalibrationValue__) << 8))

/** @brief  Macro Adjusts the Internal Multi Speed oscillator (MSI) calibration value.
  * @note   The calibration is used to compensate for the variations in voltage
  *         and temperature that influence the frequency of the internal MSI RC.
  *         Refer to the Application Note AN3300 for more details on how to  
  *         calibrate the MSI.
  * @param  __MSICalibrationValue__: specifies the calibration trimming value.
  *         This parameter must be a number between 0 and 0xFF.
  */
#define __HAL_RCC_MSI_CALIBRATIONVALUE_ADJUST(__MSICalibrationValue__) (MODIFY_REG(RCC->ICSCR,\
        RCC_ICSCR_MSITRIM, (uint32_t)(__MSICalibrationValue__) << 24))

/**
  * @brief  Macro to configures the Internal Multi Speed oscillator (MSI) clock range.
  * @note     After restart from Reset or wakeup from STANDBY, the MSI clock is 
  *           around 2.097 MHz. The MSI clock does not change after wake-up from
  *           STOP mode.
  * @note    The MSI clock range can be modified on the fly.
  * @param  RCC_MSIRange: specifies the MSI Clock range.
  *   This parameter must be one of the following values:
  *     @arg RCC_MSIRANGE_0: MSI clock is around 65.536 KHz
  *     @arg RCC_MSIRANGE_1: MSI clock is around 131.072 KHz
  *     @arg RCC_MSIRANGE_2: MSI clock is around 262.144 KHz
  *     @arg RCC_MSIRANGE_3: MSI clock is around 524.288 KHz
  *     @arg RCC_MSIRANGE_4: MSI clock is around 1.048 MHz
  *     @arg RCC_MSIRANGE_5: MSI clock is around 2.097 MHz (default after Reset or wake-up from STANDBY)
  *     @arg RCC_MSIRANGE_6: MSI clock is around 4.194 MHz
  */
#define __HAL_RCC_MSI_RANGE_CONFIG(__RCC_MSIRange__) (MODIFY_REG(RCC->ICSCR,\
        RCC_ICSCR_MSIRANGE, (uint32_t)(__RCC_MSIRange__) ))

/** @brief  Macros to enable or disable the Internal Low Speed oscillator (LSI).
  * @note   After enabling the LSI, the application software should wait on 
  *         LSIRDY flag to be set indicating that LSI clock is stable and can
  *         be used to clock the IWDG and/or the RTC.
  * @note   LSI can not be disabled if the IWDG is running.
  * @note   When the LSI is stopped, LSIRDY flag goes low after 6 LSI oscillator
  *         clock cycles. 
  */
#define __HAL_RCC_LSI_ENABLE() SET_BIT(RCC->CSR, RCC_CSR_LSION)
#define __HAL_RCC_LSI_DISABLE() CLEAR_BIT(RCC->CSR, RCC_CSR_LSION)

/**
  * @brief  Macro to configure the External High Speed oscillator (HSE).
  * @note   After enabling the HSE (RCC_HSE_ON or RCC_HSE_Bypass), the application
  *         software should wait on HSERDY flag to be set indicating that HSE clock
  *         is stable and can be used to clock the PLL and/or system clock.
  * @note   HSE state can not be changed if it is used directly or through the
  *         PLL as system clock. In this case, you have to select another source
  *         of the system clock then change the HSE state (ex. disable it).
  * @note   The HSE is stopped by hardware when entering STOP and STANDBY modes.  
  * @note   This function reset the CSSON bit, so if the clock security system(CSS)
  *         was previously enabled you have to enable it again after calling this
  *         function.    
  * @param  __STATE__: specifies the new state of the HSE.
  *         This parameter can be one of the following values:
  *            @arg RCC_HSE_OFF: turn OFF the HSE oscillator, HSERDY flag goes low after
  *                              6 HSE oscillator clock cycles.
  *            @arg RCC_HSE_ON: turn ON the HSE oscillator.
  *            @arg RCC_HSE_BYPASS: HSE oscillator bypassed with external clock.
  */
#define __HAL_RCC_HSE_CONFIG(__STATE__) \
                  MODIFY_REG(RCC->CR, RCC_CR_HSEON|RCC_CR_HSEBYP, (uint32_t)(__STATE__))

/**
  * @brief  Macro to configure the External Low Speed oscillator (LSE).
  * @note   As the LSE is in the Backup domain and write access is denied to
  *         this domain after reset, you have to enable write access using 
  *         HAL_PWR_EnableBkUpAccess() function before to configure the LSE
  *         (to be done once after reset).  
  * @note   After enabling the LSE (RCC_LSE_ON or RCC_LSE_BYPASS), the application
  *         software should wait on LSERDY flag to be set indicating that LSE clock
  *         is stable and can be used to clock the RTC.
  * @param  __STATE__: specifies the new state of the LSE.
  *         This parameter can be one of the following values:
  *            @arg RCC_LSE_OFF: turn OFF the LSE oscillator, LSERDY flag goes low after
  *                              6 LSE oscillator clock cycles.
  *            @arg RCC_LSE_ON: turn ON the LSE oscillator.
  *            @arg RCC_LSE_BYPASS: LSE oscillator bypassed with external clock.
  */
#define __HAL_RCC_LSE_CONFIG(__STATE__) \
                  MODIFY_REG(RCC->CSR, RCC_CSR_LSEON|RCC_CSR_LSEBYP, (uint32_t)(__STATE__))

/** @brief  Macros to enable or disable the the RTC clock.
  * @note   These macros must be used only after the RTC clock source was selected.
  */
#define __HAL_RCC_RTC_ENABLE() SET_BIT(RCC->CSR, RCC_CSR_RTCEN)
#define __HAL_RCC_RTC_DISABLE() CLEAR_BIT(RCC->CSR, RCC_CSR_RTCEN)

/**
  * @brief  Configures  or  Get the RTC and LCD clock (RTCCLK / LCDCLK).
  * @note     As the RTC clock configuration bits are in the RTC domain and write
  *           access is denied to this domain after reset, you have to enable write
  *           access using PWR_RTCAccessCmd(ENABLE) function before to configure
  *           the RTC clock source (to be done once after reset).    
  * @note     Once the RTC clock is configured it can't be changed unless the RTC
  *           is reset using RCC_RTCResetCmd function, or by a Power On Reset (POR)
  * @note     The RTC clock (RTCCLK) is used also to clock the LCD (LCDCLK).
  *
  * @param  RCC_RTCCLKSource: specifies the RTC clock source.
  *   This parameter can be one of the following values:
  *     @arg RCC_RTCCLKSOURCE_LSE: LSE selected as RTC clock
  *     @arg RCC_RTCCLKSOURCE_LSI: LSI selected as RTC clock
  *     @arg RCC_RTCCLKSOURCE_HSE_DIV2: HSE divided by 2 selected as RTC clock
  *     @arg RCC_RTCCLKSOURCE_HSE_DIV4: HSE divided by 4 selected as RTC clock
  *     @arg RCC_RTCCLKSOURCE_HSE_DIV8: HSE divided by 8 selected as RTC clock
  *     @arg RCC_RTCCLKSOURCE_HSE_DIV16: HSE divided by 16 selected as RTC clock
  *
  * @note     If the LSE or LSI is used as RTC clock source, the RTC continues to
  *           work in STOP and STANDBY modes, and can be used as wakeup source.
  *           However, when the HSE clock is used as RTC clock source, the RTC
  *           cannot be used in STOP and STANDBY modes.          
  * @note     The maximum input clock frequency for RTC is 1MHz (when using HSE as
  *           RTC clock source).
  */
#define __HAL_RCC_RTC_CLKPRESCALER(__RTCCLKSource__) (((__RTCCLKSource__) & RCC_CSR_RTCSEL) == RCC_CSR_RTCSEL) ?    \
                                                 MODIFY_REG(RCC->CR, RCC_CR_RTCPRE, ((__RTCCLKSource__) & 0xFFFCFFFF)) : CLEAR_BIT(RCC->CR, RCC_CR_RTCPRE)

#define __HAL_RCC_RTC_CONFIG(__RTCCLKSource__) do { __HAL_RCC_RTC_CLKPRESCALER(__RTCCLKSource__);    \
                                                   MODIFY_REG( RCC->CSR, RCC_CSR_RTCSEL, (uint32_t)(__RTCCLKSource__));  \
                                                   } while (0)

#define  __HAL_RCC_GET_RTC_SOURCE() ((uint32_t)(READ_BIT(RCC->CSR, RCC_CSR_RTCSEL)))

/** @brief  Macros to force or release the Backup domain reset.
  * @note   This function resets the RTC peripheral (including the backup registers)
  *         and the RTC clock source selection in RCC_CSR register.
  * @note   The BKPSRAM is not affected by this reset.   
  */
#define __HAL_RCC_BACKUPRESET_FORCE()  SET_BIT(RCC->CSR, RCC_CSR_RTCRST) 
#define __HAL_RCC_BACKUPRESET_RELEASE() CLEAR_BIT(RCC->CSR, RCC_CSR_RTCRST) 

/** @brief  Macros to enable or disable the main PLL.
  * @note   After enabling the main PLL, the application software should wait on 
  *         PLLRDY flag to be set indicating that PLL clock is stable and can
  *         be used as system clock source.
  * @note   The main PLL can not be disabled if it is used as system clock source
  * @note   The main PLL is disabled by hardware when entering STOP and STANDBY modes.
  */
#define __HAL_RCC_PLL_ENABLE() SET_BIT(RCC->CR, RCC_CR_PLLON)
#define __HAL_RCC_PLL_DISABLE() CLEAR_BIT(RCC->CR, RCC_CR_PLLON)

/** @brief  Macro to configure the main PLL clock source, multiplication and division factors.
  * @note   This function must be used only when the main PLL is disabled.
  * @param  __RCC_PLLSource__: specifies the PLL entry clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_PLLSOURCE_HSI: HSI oscillator clock selected as PLL clock entry
  *            @arg RCC_PLLSOURCE_HSE: HSE oscillator clock selected as PLL clock entry
  * @param  __PLLMUL__: specifies the multiplication factor to generate the PLL VCO clock
  *         This parameter must be one of the following values:
  *            @arg RCC_CFGR_PLLMUL3: PLLVCO = PLL clock entry x 3
  *            @arg RCC_CFGR_PLLMUL4: PLLVCO = PLL clock entry x 4
  *            @arg RCC_CFGR_PLLMUL6: PLLVCO = PLL clock entry x 6
  *            @arg RCC_CFGR_PLLMUL8: PLLVCO = PLL clock entry x 8
  *            @arg RCC_CFGR_PLLMUL12: PLLVCO = PLL clock entry x 12
  *            @arg RCC_CFGR_PLLMUL16: PLLVCO = PLL clock entry x 16
  *            @arg RCC_CFGR_PLLMUL24: PLLVCO = PLL clock entry x 24  
  *            @arg RCC_CFGR_PLLMUL32: PLLVCO = PLL clock entry x 32
  *            @arg RCC_CFGR_PLLMUL48: PLLVCO = PLL clock entry x 48
  * @note   The PLL VCO clock frequency must not exceed 96 MHz when the product is in
  *         Range 1, 48 MHz when the product is in Range 2 and 24 MHz when the product is
  *         in Range 3.
  * @param  __PLLDIV__: specifies the PLL output clock division from PLL VCO clock
  *         This parameter must be one of the following values:
  *            @arg RCC_PLLDIV_2: PLL clock output = PLLVCO / 2
  *            @arg RCC_PLLDIV_3: PLL clock output = PLLVCO / 3
  *            @arg RCC_PLLDIV_4: PLL clock output = PLLVCO / 4
  */

#define __HAL_RCC_PLL_CONFIG(__RCC_PLLSource__ , __PLLMUL__ ,__PLLDIV__ ) \
            MODIFY_REG(RCC->CFGR, RCC_CFGR_PLLMUL | RCC_CFGR_PLLDIV | RCC_CFGR_PLLSRC, (uint32_t)((__PLLMUL__)| (__PLLDIV__)| (__RCC_PLLSource__)))

/** @brief  Macro to get the clock source used as system clock.
  * @retval The clock source used as system clock. The returned value can be one
  *         of the following:
  *              - RCC_CFGR_SWS_HSI: HSI used as system clock.
  *              - RCC_CFGR_SWS_HSE: HSE used as system clock.
  *              - RCC_CFGR_SWS_PLL: PLL used as system clock.
  */
#define __HAL_RCC_GET_SYSCLK_SOURCE() ((uint32_t)(RCC->CFGR & RCC_CFGR_SWS))

/** @brief  Macro to get the oscillator used as PLL clock source.
  * @retval The oscillator used as PLL clock source. The returned value can be one
  *         of the following:
  *              - RCC_PLLSOURCE_HSI: HSI oscillator is used as PLL clock source.
  *              - RCC_PLLSOURCE_HSE: HSE oscillator is used as PLL clock source.
  */
#define __HAL_RCC_GET_PLL_OSCSOURCE() ((uint32_t)(RCC->CFGR & RCC_CFGR_PLLSRC))

/** @defgroup RCC_Flags_Interrupts_Management
  * @brief macros to manage the specified RCC Flags and interrupts.
  * @{
  */

/** @brief  Enable RCC interrupt (Perform Byte access to RCC_CIER[0:7] bits to enable
  *         the selected interrupts).
  * @note   The CSS interrupt doesn't have an enable bit; once the CSS is enabled
  *         and if the HSE clock fails, the CSS interrupt occurs and an NMI is
  *         automatically generated. The NMI will be executed indefinitely, and 
  *         since NMI has higher priority than any other IRQ (and main program)
  *         the application will be stacked in the NMI ISR unless the CSS interrupt
  *         pending bit is cleared.            
  * @param  __INTERRUPT__: specifies the RCC interrupt sources to be enabled.
  *         This parameter can be any combination of the following values:
  *     @arg RCC_IT_LSIRDY: LSI ready interrupt
  *     @arg RCC_IT_LSERDY: LSE ready interrupt
  *     @arg RCC_IT_HSIRDY: HSI ready interrupt
  *     @arg RCC_IT_HSERDY: HSE ready interrupt
  *     @arg RCC_IT_PLLRDY: PLL ready interrupt
  *     @arg RCC_IT_MSIRDY: MSI ready interrupt
  *     @arg RCC_IT_LSECSS: LSE CSS interrupt  
  */
#define __HAL_RCC_ENABLE_IT(__INTERRUPT__) (*(__IO uint8_t *) CIER_BYTE0_ADDRESS |= (__INTERRUPT__))

/** @brief Disable RCC interrupt (Perform Byte access to RCC_CIER[0:7] bits to disable 
  *        the selected interrupts).
  * @note   The CSS interrupt doesn't have an enable bit; once the CSS is enabled
  *         and if the HSE clock fails, the CSS interrupt occurs and an NMI is
  *         automatically generated. The NMI will be executed indefinitely, and 
  *         since NMI has higher priority than any other IRQ (and main program)
  *         the application will be stacked in the NMI ISR unless the CSS interrupt
  *         pending bit is cleared.
  * @param  __INTERRUPT__: specifies the RCC interrupt sources to be disabled.
  *         This parameter can be any combination of the following values:
  *     @arg RCC_IT_LSIRDY: LSI ready interrupt
  *     @arg RCC_IT_LSERDY: LSE ready interrupt
  *     @arg RCC_IT_HSIRDY: HSI ready interrupt
  *     @arg RCC_IT_HSERDY: HSE ready interrupt
  *     @arg RCC_IT_PLLRDY: PLL ready interrupt
  *     @arg RCC_IT_MSIRDY: MSI ready interrupt
  *     @arg RCC_IT_LSECSS: LSE CSS interrupt  
  */
#define __HAL_RCC_DISABLE_IT(__INTERRUPT__) (*(__IO uint8_t *) CIER_BYTE0_ADDRESS &= ~(__INTERRUPT__))

/** @brief  Clear the RCC's interrupt pending bits (Perform Byte access to RCC_CIR[23:16]
  *         bits to clear the selected interrupt pending bits.
  * @param  __INTERRUPT__: specifies the interrupt pending bit to clear.
  *         This parameter can be any combination of the following values:
  *     @arg RCC_IT_LSIRDY: LSI ready interrupt
  *     @arg RCC_IT_LSERDY: LSE ready interrupt
  *     @arg RCC_IT_HSIRDY: HSI ready interrupt
  *     @arg RCC_IT_HSERDY: HSE ready interrupt
  *     @arg RCC_IT_PLLRDY: PLL ready interrupt
  *     @arg RCC_IT_MSIRDY: MSI ready interrupt 
  *     @arg RCC_IT_LSECSS: LSE CSS interrupt
  *     @arg RCC_IT_CSS: Clock Security System interrupt
  */
 #define __HAL_RCC_CLEAR_IT(__INTERRUPT__) (RCC->CICR = (__INTERRUPT__))

/** @brief  Check the RCC's interrupt has occurred or not.
  * @param  __INTERRUPT__: specifies the RCC interrupt source to check.
  *         This parameter can be one of the following values:
  *     @arg RCC_IT_LSIRDY: LSI ready interrupt
  *     @arg RCC_IT_LSERDY: LSE ready interrupt  
  *     @arg RCC_IT_HSIRDY: HSI ready interrupt
  *     @arg RCC_IT_HSERDY: HSE ready interrupt
  *     @arg RCC_IT_PLLRDY: PLL ready interrupt
  *     @arg RCC_IT_MSIRDY: MSI ready interrupt
  *     @arg RCC_IT_LSECSS: LSE CSS interrupt 
  *     @arg RCC_IT_CSS: Clock Security System interrupt
  * @retval The new state of __INTERRUPT__ (TRUE or FALSE).
  */
#define __HAL_RCC_GET_IT(__INTERRUPT__) ((RCC->CIFR & (__INTERRUPT__)) == (__INTERRUPT__))

/** @brief Set RMVF bit to clear the reset flags.
  *         The reset flags are: RCC_FLAG_OBLRST, RCC_FLAG_PINRST, RCC_FLAG_PORRST, 
  *         RCC_FLAG_SFTRST, RCC_FLAG_IWDGRST, RCC_FLAG_WWDGRST, RCC_FLAG_LPWRRST.
  */
#define __HAL_RCC_CLEAR_RESET_FLAGS() (RCC->CSR |= RCC_CSR_RMVF)

/** @brief  Check RCC flag is set or not.
  * @param  __FLAG__: specifies the flag to check.
  *         This parameter can be one of the following values:
  *     @arg RCC_FLAG_HSIRDY: HSI oscillator clock ready
  *     @arg RCC_FLAG_MSIRDY: MSI oscillator clock ready  
  *     @arg RCC_FLAG_HSERDY: HSE oscillator clock ready
  *     @arg RCC_FLAG_PLLRDY: PLL clock ready
  *     @arg RCC_FLAG_LSECSS: LSE oscillator clock CSS detected  
  *     @arg RCC_FLAG_LSERDY: LSE oscillator clock ready
  *     @arg RCC_FLAG_LSIRDY: LSI oscillator clock ready
  *     @arg RCC_FLAG_OBLRST: Option Byte Loader (OBL) reset 
  *     @arg RCC_FLAG_PINRST: Pin reset
  *     @arg RCC_FLAG_PORRST: POR/PDR reset
  *     @arg RCC_FLAG_SFTRST: Software reset
  *     @arg RCC_FLAG_IWDGRST: Independent Watchdog reset
  *     @arg RCC_FLAG_WWDGRST: Window Watchdog reset
  *     @arg RCC_FLAG_LPWRRST: Low Power reset
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  */
#define RCC_FLAG_MASK  ((uint8_t)0x1F)
#define __HAL_RCC_GET_FLAG(__FLAG__) (((((((__FLAG__) >> 5) == 1)? RCC->CR :((((__FLAG__) >> 5) == 2) ? RCC->CSR :((((__FLAG__) >> 5) == 3)? \
              RCC->CRRCR :RCC->CIFR))) & ((uint32_t)1 << ((__FLAG__) & RCC_FLAG_MASK))) != 0 ) ? 1 : 0 )  

/**
  * @}
  */

#define __RCC_PLLSRC() ((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> POSITION_VAL(RCC_PLLCFGR_PLLSRC))
/**
  * @}
  */

/* Include RCC HAL Extension module */
#include "stm32l0xx_hal_rcc_ex.h"

/* Exported functions --------------------------------------------------------*/
/* Initialization and de-initialization methods  ******************************/
void HAL_RCC_DeInit(void);
HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef *RCC_OscInitStruct);
HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef *RCC_ClkInitStruct, uint32_t FLatency);

/* Peripheral Control methods  ************************************************/
void     HAL_RCC_MCOConfig(uint32_t RCC_MCOx, uint32_t RCC_MCOSource, uint32_t RCC_MCODiv);
void     HAL_RCC_EnableCSS(void);
uint32_t HAL_RCC_GetSysClockFreq(void);
uint32_t HAL_RCC_GetHCLKFreq(void);
uint32_t HAL_RCC_GetPCLK1Freq(void);
uint32_t HAL_RCC_GetPCLK2Freq(void);
void     HAL_RCC_GetOscConfig(RCC_OscInitTypeDef *RCC_OscInitStruct);
void     HAL_RCC_GetClockConfig(RCC_ClkInitTypeDef *RCC_ClkInitStruct, uint32_t *pFLatency);

/* CSS NMI IRQ handler */
void HAL_RCC_NMI_IRQHandler(void);

/* User Callbacks in non blocking mode (IT mode) */ 
void HAL_RCC_CCSCallback(void);

/**
  * @}
  */ 

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32L0xx_HAL_RCC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
