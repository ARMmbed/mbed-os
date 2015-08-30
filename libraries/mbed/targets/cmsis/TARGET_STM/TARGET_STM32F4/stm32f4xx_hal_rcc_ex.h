/**
  ******************************************************************************
  * @file    stm32f4xx_hal_rcc_ex.h
  * @author  MCD Application Team
  * @version V1.3.2
  * @date    26-June-2015
  * @brief   Header file of RCC HAL Extension module.
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
#ifndef __STM32F4xx_HAL_RCC_EX_H
#define __STM32F4xx_HAL_RCC_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"

/** @addtogroup STM32F4xx_HAL_Driver
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
  * @brief  RCC PLL configuration structure definition
  */
typedef struct
{
  uint32_t PLLState;   /*!< The new state of the PLL.
                            This parameter can be a value of @ref RCC_PLL_Config                      */

  uint32_t PLLSource;  /*!< RCC_PLLSource: PLL entry clock source.
                            This parameter must be a value of @ref RCC_PLL_Clock_Source               */           

  uint32_t PLLM;       /*!< PLLM: Division factor for PLL VCO input clock.
                            This parameter must be a number between Min_Data = 0 and Max_Data = 63    */        

  uint32_t PLLN;       /*!< PLLN: Multiplication factor for PLL VCO output clock.
                            This parameter must be a number between Min_Data = 192 and Max_Data = 432 */

  uint32_t PLLP;       /*!< PLLP: Division factor for main system clock (SYSCLK).
                            This parameter must be a value of @ref RCC_PLLP_Clock_Divider             */

  uint32_t PLLQ;       /*!< PLLQ: Division factor for OTG FS, SDIO and RNG clocks.
                            This parameter must be a number between Min_Data = 4 and Max_Data = 15    */
#if defined(STM32F446xx)
  uint32_t PLLR;       /*!< PLLR: PLL division factor for I2S, SAI, SYSTEM, SPDIFRX clocks.
                            This parameter is only available in STM32F446xx devices. 
                            This parameter must be a number between Min_Data = 2 and Max_Data = 7     */
#endif /* STM32F446xx */ 
}RCC_PLLInitTypeDef;

#if defined(STM32F446xx)
/** 
  * @brief  PLLI2S Clock structure definition  
  */
typedef struct
{
  uint32_t PLLI2SM;    /*!< Specifies division factor for PLL VCO input clock.
                            This parameter must be a number between Min_Data = 2 and Max_Data = 63       */  

  uint32_t PLLI2SN;    /*!< Specifies the multiplication factor for PLLI2S VCO output clock.
                            This parameter must be a number between Min_Data = 192 and Max_Data = 432    */

  uint32_t PLLI2SP;    /*!< Specifies division factor for SPDIFRX Clock.
                            This parameter must be a value of @ref RCCEx_PLLI2SP_Clock_Divider           */

  uint32_t PLLI2SQ;    /*!< Specifies the division factor for SAI clock.
                            This parameter must be a number between Min_Data = 2 and Max_Data = 15. 
                            This parameter will be used only when PLLI2S is selected as Clock Source SAI */ 
                           
  uint32_t PLLI2SR;    /*!< Specifies the division factor for I2S clock.
                            This parameter must be a number between Min_Data = 2 and Max_Data = 7. 
                            This parameter will be used only when PLLI2S is selected as Clock Source I2S */
}RCC_PLLI2SInitTypeDef;

/** 
  * @brief  PLLSAI Clock structure definition  
  */
typedef struct
{
  uint32_t PLLSAIM;    /*!< Spcifies division factor for PLL VCO input clock.
                            This parameter must be a number between Min_Data = 2 and Max_Data = 63       */ 

  uint32_t PLLSAIN;    /*!< Specifies the multiplication factor for PLLI2S VCO output clock.
                            This parameter must be a number between Min_Data = 192 and Max_Data = 432    */ 

  uint32_t PLLSAIP;    /*!< Specifies division factor for OTG FS, SDIO and RNG clocks.
                            This parameter must be a value of @ref RCCEx_PLLSAIP_Clock_Divider           */
                                                             
  uint32_t PLLSAIQ;    /*!< Specifies the division factor for SAI clock.
                            This parameter must be a number between Min_Data = 2 and Max_Data = 15.
                            This parameter will be used only when PLLSAI is selected as Clock Source SAI */
}RCC_PLLSAIInitTypeDef;
/** 
  * @brief  RCC extended clocks structure definition  
  */
typedef struct
{
  uint32_t PeriphClockSelection; /*!< The Extended Clock to be configured.
                                      This parameter can be a value of @ref RCCEx_Periph_Clock_Selection */

  RCC_PLLI2SInitTypeDef PLLI2S;  /*!< PLL I2S structure parameters. 
                                      This parameter will be used only when PLLI2S is selected as Clock Source I2S or SAI */

  RCC_PLLSAIInitTypeDef PLLSAI;  /*!< PLL SAI structure parameters. 
                                      This parameter will be used only when PLLI2S is selected as Clock Source SAI or LTDC */

  uint32_t PLLI2SDivQ;           /*!< Specifies the PLLI2S division factor for SAI1 clock.
                                      This parameter must be a number between Min_Data = 1 and Max_Data = 32
                                      This parameter will be used only when PLLI2S is selected as Clock Source SAI */

  uint32_t PLLSAIDivQ;           /*!< Specifies the PLLI2S division factor for SAI1 clock.
                                      This parameter must be a number between Min_Data = 1 and Max_Data = 32
                                      This parameter will be used only when PLLSAI is selected as Clock Source SAI */

  uint32_t Sai1ClockSelection;    /*!< Specifies SAI1 Clock Source Selection. 
                                      This parameter can be a value of @ref RCCEx_SAI1_Clock_Source */

  uint32_t Sai2ClockSelection;    /*!< Specifies SAI2 Clock Source Selection. 
                                      This parameter can be a value of @ref RCCEx_SAI2_Clock_Source */
                                      
  uint32_t I2sApb1ClockSelection;    /*!< Specifies I2S APB1 Clock Source Selection. 
                                      This parameter can be a value of @ref RCCEx_I2SAPB1_Clock_Source */

  uint32_t I2sApb2ClockSelection;    /*!< Specifies I2S APB2 Clock Source Selection. 
                                      This parameter can be a value of @ref RCCEx_I2SAPB2_Clock_Source */

  uint32_t RTCClockSelection;      /*!< Specifies RTC Clock Source Selection. 
                                      This parameter can be a value of @ref RCC_RTC_Clock_Source */

  uint32_t SdioClockSelection;    /*!< Specifies SDIO Clock Source Selection. 
                                      This parameter can be a value of @ref RCCEx_SDIO_Clock_Source */

  uint32_t CecClockSelection;      /*!< Specifies CEC Clock Source Selection. 
                                      This parameter can be a value of @ref RCCEx_CEC_Clock_Source */

  uint32_t Fmpi2c1ClockSelection;  /*!< Specifies FMPI2C1 Clock Source Selection. 
                                      This parameter can be a value of @ref RCCEx_FMPI2C1_Clock_Source */

  uint32_t SpdifClockSelection;    /*!< Specifies SPDIFRX Clock Source Selection. 
                                      This parameter can be a value of @ref RCCEx_SPDIFRX_Clock_Source */

  uint32_t Clk48ClockSelection;     /*!< Specifies CK48 Clock Selection this clock used OTG FS, SDIO and RNG clocks. 
                                      This parameter can be a value of @ref RCCEx_CK48_Clock_Source */
  
  uint8_t TIMPresSelection;      /*!< Specifies TIM Clock Source Selection. 
                                      This parameter can be a value of @ref RCCEx_TIM_PRescaler_Selection */
}RCC_PeriphCLKInitTypeDef;
#endif /* STM32F446xx */   

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)

/** 
  * @brief  PLLI2S Clock structure definition  
  */
typedef struct
{
  uint32_t PLLI2SN;    /*!< Specifies the multiplication factor for PLLI2S VCO output clock.
                            This parameter must be a number between Min_Data = 192 and Max_Data = 432.
                            This parameter will be used only when PLLI2S is selected as Clock Source I2S or SAI */

  uint32_t PLLI2SR;    /*!< Specifies the division factor for I2S clock.
                            This parameter must be a number between Min_Data = 2 and Max_Data = 7. 
                            This parameter will be used only when PLLI2S is selected as Clock Source I2S or SAI */

  uint32_t PLLI2SQ;    /*!< Specifies the division factor for SAI1 clock.
                            This parameter must be a number between Min_Data = 2 and Max_Data = 15. 
                            This parameter will be used only when PLLI2S is selected as Clock Source SAI */
}RCC_PLLI2SInitTypeDef;

/** 
  * @brief  PLLSAI Clock structure definition  
  */
typedef struct
{
  uint32_t PLLSAIN;    /*!< Specifies the multiplication factor for PLLI2S VCO output clock.
                            This parameter must be a number between Min_Data = 192 and Max_Data = 432.
                            This parameter will be used only when PLLSAI is selected as Clock Source SAI or LTDC */ 
                                 
  uint32_t PLLSAIQ;    /*!< Specifies the division factor for SAI1 clock.
                            This parameter must be a number between Min_Data = 2 and Max_Data = 15.
                            This parameter will be used only when PLLSAI is selected as Clock Source SAI or LTDC */
                              
  uint32_t PLLSAIR;    /*!< specifies the division factor for LTDC clock
                            This parameter must be a number between Min_Data = 2 and Max_Data = 7.
                            This parameter will be used only when PLLSAI is selected as Clock Source LTDC */

}RCC_PLLSAIInitTypeDef;
/** 
  * @brief  RCC extended clocks structure definition  
  */
typedef struct
{
  uint32_t PeriphClockSelection; /*!< The Extended Clock to be configured.
                                      This parameter can be a value of @ref RCCEx_Periph_Clock_Selection */

  RCC_PLLI2SInitTypeDef PLLI2S;  /*!< PLL I2S structure parameters. 
                                      This parameter will be used only when PLLI2S is selected as Clock Source I2S or SAI */

  RCC_PLLSAIInitTypeDef PLLSAI;  /*!< PLL SAI structure parameters. 
                                      This parameter will be used only when PLLI2S is selected as Clock Source SAI or LTDC */

  uint32_t PLLI2SDivQ;           /*!< Specifies the PLLI2S division factor for SAI1 clock.
                                      This parameter must be a number between Min_Data = 1 and Max_Data = 32
                                      This parameter will be used only when PLLI2S is selected as Clock Source SAI */

  uint32_t PLLSAIDivQ;           /*!< Specifies the PLLI2S division factor for SAI1 clock.
                                      This parameter must be a number between Min_Data = 1 and Max_Data = 32
                                      This parameter will be used only when PLLSAI is selected as Clock Source SAI */

  uint32_t PLLSAIDivR;           /*!< Specifies the PLLSAI division factor for LTDC clock.
                                      This parameter must be one value of @ref RCCEx_PLLSAI_DIVR */

  uint32_t RTCClockSelection;      /*!< Specifies RTC Clock Prescalers Selection. 
                                      This parameter can be a value of @ref RCC_RTC_Clock_Source */

  uint8_t TIMPresSelection;      /*!< Specifies TIM Clock Prescalers Selection. 
                                      This parameter can be a value of @ref RCCEx_TIM_PRescaler_Selection */

}RCC_PeriphCLKInitTypeDef;
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx */

#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx)|| defined(STM32F417xx) ||\
    defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F411xE)
/** 
  * @brief  PLLI2S Clock structure definition  
  */
typedef struct
{
#if defined(STM32F411xE) 
  uint32_t PLLI2SM;    /*!< PLLM: Division factor for PLLI2S VCO input clock.
                            This parameter must be a number between Min_Data = 2 and Max_Data = 62  */
#endif /* STM32F411xE */
                                
  uint32_t PLLI2SN;    /*!< Specifies the multiplication factor for PLLI2S VCO output clock.
                            This parameter must be a number between Min_Data = 192 and Max_Data = 432
                            This parameter will be used only when PLLI2S is selected as Clock Source I2S or SAI */

  uint32_t PLLI2SR;    /*!< Specifies the division factor for I2S clock.
                            This parameter must be a number between Min_Data = 2 and Max_Data = 7. 
                            This parameter will be used only when PLLI2S is selected as Clock Source I2S or SAI */

}RCC_PLLI2SInitTypeDef;

 
/** 
  * @brief  RCC extended clocks structure definition  
  */
typedef struct
{
  uint32_t PeriphClockSelection; /*!< The Extended Clock to be configured.
                                      This parameter can be a value of @ref RCCEx_Periph_Clock_Selection */

  RCC_PLLI2SInitTypeDef PLLI2S;  /*!< PLL I2S structure parameters.
                                      This parameter will be used only when PLLI2S is selected as Clock Source I2S or SAI */

  uint32_t RTCClockSelection;      /*!< Specifies RTC Clock Prescalers Selection.
                                       This parameter can be a value of @ref RCC_RTC_Clock_Source */                                   

}RCC_PeriphCLKInitTypeDef;
#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx || STM32F401xC || STM32F401xE || STM32F411xE */
/**
  * @}
  */ 

/* Exported constants --------------------------------------------------------*/
/** @defgroup RCCEx_Exported_Constants RCCEx Exported Constants
  * @{
  */

/** @defgroup RCCEx_Periph_Clock_Selection RCC Periph Clock Selection
  * @{
  */
/*------------------------------- Peripheral Clock source for STM32F446xx -----------------------------*/
#if defined(STM32F446xx)
#define RCC_PERIPHCLK_I2S_APB1        ((uint32_t)0x00000001)
#define RCC_PERIPHCLK_I2S_APB2        ((uint32_t)0x00000002)
#define RCC_PERIPHCLK_SAI1            ((uint32_t)0x00000004)
#define RCC_PERIPHCLK_SAI2            ((uint32_t)0x00000008)
#define RCC_PERIPHCLK_TIM             ((uint32_t)0x00000010)
#define RCC_PERIPHCLK_RTC             ((uint32_t)0x00000020)
#define RCC_PERIPHCLK_CEC             ((uint32_t)0x00000040)
#define RCC_PERIPHCLK_FMPI2C1         ((uint32_t)0x00000080)
#define RCC_PERIPHCLK_CK48            ((uint32_t)0x00000100)
#define RCC_PERIPHCLK_SDIO            ((uint32_t)0x00000200)
#define RCC_PERIPHCLK_SPDIFRX         ((uint32_t)0x00000400)
#define RCC_PERIPHCLK_PLLI2S          ((uint32_t)0x00000800)
#endif /* STM32F446xx */
/*-----------------------------------------------------------------------------------------------------*/

/*--------------- Peripheral Clock source for STM32F42xxx/STM32F43xxx ---------------------*/
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)
#define RCC_PERIPHCLK_I2S             ((uint32_t)0x00000001)
#define RCC_PERIPHCLK_SAI_PLLI2S      ((uint32_t)0x00000002)
#define RCC_PERIPHCLK_SAI_PLLSAI      ((uint32_t)0x00000004)
#define RCC_PERIPHCLK_LTDC            ((uint32_t)0x00000008)
#define RCC_PERIPHCLK_TIM             ((uint32_t)0x00000010)
#define RCC_PERIPHCLK_RTC             ((uint32_t)0x00000020)
#define RCC_PERIPHCLK_PLLI2S          ((uint32_t)0x00000040)
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx */
/*-----------------------------------------------------------------------------------------------------*/

/*------------------------ Peripheral Clock source for STM32F40xxx/STM32F41xxx ------------------------*/
#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx)|| defined(STM32F417xx) ||\
    defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F411xE) 
#define RCC_PERIPHCLK_I2S             ((uint32_t)0x00000001)
#define RCC_PERIPHCLK_RTC             ((uint32_t)0x00000002)
#define RCC_PERIPHCLK_PLLI2S          ((uint32_t)0x00000004)
#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx || STM32F401xC || STM32F401xE || STM32F411xE */
/*-----------------------------------------------------------------------------------------------------*/
/**
  * @}
  */

/** @defgroup RCCEx_PLLSAI_DIVR RCC PLLSAI DIVR
  * @{
  */
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || defined(STM32F446xx)
#define RCC_PLLSAIDIVR_2                ((uint32_t)0x00000000)
#define RCC_PLLSAIDIVR_4                ((uint32_t)0x00010000)
#define RCC_PLLSAIDIVR_8                ((uint32_t)0x00020000)
#define RCC_PLLSAIDIVR_16               ((uint32_t)0x00030000)
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F446xx */
/**
  * @}
  */

/** @defgroup RCCEx_PLLI2SP_Clock_Divider RCC PLLI2SP Clock Divider
  * @{
  */
#if defined(STM32F446xx)
#define RCC_PLLI2SP_DIV2                  ((uint32_t)0x00000002)
#define RCC_PLLI2SP_DIV4                  ((uint32_t)0x00000004)
#define RCC_PLLI2SP_DIV6                  ((uint32_t)0x00000006)
#define RCC_PLLI2SP_DIV8                  ((uint32_t)0x00000008)
#endif /* STM32F446xx */
/**
  * @}
  */

/** @defgroup RCCEx_PLLSAIP_Clock_Divider RCC PLLSAIP Clock Divider
  * @{
  */
#if defined(STM32F446xx)
#define RCC_PLLSAIP_DIV2                  ((uint32_t)0x00000002)
#define RCC_PLLSAIP_DIV4                  ((uint32_t)0x00000004)
#define RCC_PLLSAIP_DIV6                  ((uint32_t)0x00000006)
#define RCC_PLLSAIP_DIV8                  ((uint32_t)0x00000008)
#endif /* STM32F446xx */
/**
  * @}
  */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)
/** @defgroup RCCEx_SAI_BlockA_Clock_Source  RCC SAI BlockA Clock Source
  * @{
  */
#define RCC_SAIACLKSOURCE_PLLSAI             ((uint32_t)0x00000000)
#define RCC_SAIACLKSOURCE_PLLI2S             ((uint32_t)0x00100000)
#define RCC_SAIACLKSOURCE_EXT                ((uint32_t)0x00200000)
/**
  * @}
  */ 

/** @defgroup RCCEx_SAI_BlockB_Clock_Source  RCC SAI BlockB Clock Source
  * @{
  */
#define RCC_SAIBCLKSOURCE_PLLSAI             ((uint32_t)0x00000000)
#define RCC_SAIBCLKSOURCE_PLLI2S             ((uint32_t)0x00400000)
#define RCC_SAIBCLKSOURCE_EXT                ((uint32_t)0x00800000)
/**
  * @}
  */ 
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx */

#if defined(STM32F446xx)
/** @defgroup RCCEx_SAI1_Clock_Source RCC SAI1 Clock Source 
  * @{
  */
#define RCC_SAI1CLKSOURCE_PLLSAI             ((uint32_t)0x00000000)
#define RCC_SAI1CLKSOURCE_PLLI2S             ((uint32_t)RCC_DCKCFGR_SAI1SRC_0)
#define RCC_SAI1CLKSOURCE_PLLR               ((uint32_t)RCC_DCKCFGR_SAI1SRC_1)
#define RCC_SAI1CLKSOURCE_EXT                ((uint32_t)RCC_DCKCFGR_SAI1SRC)
/**
  * @}
  */ 

/** @defgroup RCCEx_SAI2_Clock_Source  RCC SAI2 Clock Source
  * @{
  */
#define RCC_SAI2CLKSOURCE_PLLSAI             ((uint32_t)0x00000000)
#define RCC_SAI2CLKSOURCE_PLLI2S             ((uint32_t)RCC_DCKCFGR_SAI2SRC_0)
#define RCC_SAI2CLKSOURCE_PLLR               ((uint32_t)RCC_DCKCFGR_SAI2SRC_1)
#define RCC_SAI2CLKSOURCE_PLLSRC             ((uint32_t)RCC_DCKCFGR_SAI2SRC)
/**
  * @}
  */

/** @defgroup RCCEx_I2SAPB1_Clock_Source  RCC I2S APB1 Clock Source
  * @{
  */
#define RCC_I2SAPB1CLKSOURCE_PLLI2S          ((uint32_t)0x00000000)
#define RCC_I2SAPB1CLKSOURCE_EXT             ((uint32_t)RCC_DCKCFGR_I2S1SRC_0)
#define RCC_I2SAPB1CLKSOURCE_PLLR            ((uint32_t)RCC_DCKCFGR_I2S1SRC_1)
#define RCC_I2SAPB1CLKSOURCE_PLLSRC          ((uint32_t)RCC_DCKCFGR_I2S1SRC)
/**
  * @}
  */ 

/** @defgroup RCCEx_I2SAPB2_Clock_Source  RCC I2S APB2 Clock Source
  * @{
  */
#define RCC_I2SAPB2CLKSOURCE_PLLI2S          ((uint32_t)0x00000000)
#define RCC_I2SAPB2CLKSOURCE_EXT             ((uint32_t)RCC_DCKCFGR_I2S2SRC_0)
#define RCC_I2SAPB2CLKSOURCE_PLLR            ((uint32_t)RCC_DCKCFGR_I2S2SRC_1)
#define RCC_I2SAPB2CLKSOURCE_PLLSRC          ((uint32_t)RCC_DCKCFGR_I2S2SRC)
/**
  * @}
  */

/** @defgroup RCCEx_FMPI2C1_Clock_Source  RCC FMPI2C1 Clock Source
  * @{
  */
#define RCC_FMPI2C1CLKSOURCE_APB                ((uint32_t)0x00000000)
#define RCC_FMPI2C1CLKSOURCE_SYSCLK             ((uint32_t)RCC_DCKCFGR2_FMPI2C1SEL_0)
#define RCC_FMPI2C1CLKSOURCE_HSI                ((uint32_t)RCC_DCKCFGR2_FMPI2C1SEL_1)
/**
  * @}
  */

/** @defgroup RCCEx_CEC_Clock_Source  RCC CEC Clock Source
  * @{
  */
#define RCC_CECCLKSOURCE_HSI                ((uint32_t)0x00000000)
#define RCC_CECCLKSOURCE_LSE                ((uint32_t)RCC_DCKCFGR2_CECSEL)
/**
  * @}
  */

/** @defgroup RCCEx_CK48_Clock_Source  RCC CK48 Clock Source
  * @{
  */
#define RCC_CK48CLKSOURCE_PLLQ              ((uint32_t)0x00000000)
#define RCC_CK48CLKSOURCE_PLLSAIP           ((uint32_t)RCC_DCKCFGR2_CK48MSEL)
/**
  * @}
  */

/** @defgroup RCCEx_SDIO_Clock_Source  RCC SDIO Clock Source
  * @{
  */
#define RCC_SDIOCLKSOURCE_CK48              ((uint32_t)0x00000000)
#define RCC_SDIOCLKSOURCE_SYSCLK            ((uint32_t)RCC_DCKCFGR2_SDIOSEL)
/**
  * @}
  */

/** @defgroup RCCEx_SPDIFRX_Clock_Source   RCC SPDIFRX Clock Source
  * @{
  */
#define RCC_SPDIFRXCLKSOURCE_PLLR             ((uint32_t)0x00000000)
#define RCC_SPDIFRXCLKSOURCE_PLLI2SP          ((uint32_t)RCC_DCKCFGR2_SPDIFRXSEL)
/**
  * @}
  */
   
#endif /* STM32F446xx */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx)|| defined(STM32F439xx) ||\
    defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F411xE) || defined(STM32F446xx)
/** @defgroup RCCEx_TIM_PRescaler_Selection  RCC TIM PRescaler Selection
  * @{
  */
#define RCC_TIMPRES_DESACTIVATED        ((uint8_t)0x00)
#define RCC_TIMPRES_ACTIVATED           ((uint8_t)0x01)
/**
  * @}
  */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx) || STM32F401xC || STM32F401xE || STM32F411xE || STM32F446xx */

#if defined(STM32F411xE) || defined(STM32F446xx)
/** @defgroup RCCEx_LSE_Dual_Mode_Selection  RCC LSE Dual Mode Selection
  * @{
  */
#define RCC_LSE_LOWPOWER_MODE           ((uint8_t)0x00)
#define RCC_LSE_HIGHDRIVE_MODE          ((uint8_t)0x01)
/**
  * @}
  */
#endif /* STM32F411xE || STM32F446xx */

/**
  * @}
  */
     
/* Exported macro ------------------------------------------------------------*/
/** @defgroup RCCEx_Exported_Macros RCCEx Exported Macros
  * @{
  */
/*------------------------------- STM32F42xxx/STM32F43xxx ----------------------------------*/
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx)|| defined(STM32F439xx)
/** @brief  Enables or disables the AHB1 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.
  */
#define __HAL_RCC_GPIOI_CLK_ENABLE()    do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOIEN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOIEN);\
                                        UNUSED(tmpreg); \
                                        } while(0)
#define __HAL_RCC_GPIOF_CLK_ENABLE()    do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOFEN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOFEN);\
                                        UNUSED(tmpreg); \
                                        } while(0)
#define __HAL_RCC_GPIOG_CLK_ENABLE()    do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOGEN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOGEN);\
                                        UNUSED(tmpreg); \
                                        } while(0)
#define __HAL_RCC_GPIOJ_CLK_ENABLE()    do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOJEN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOJEN);\
                                        UNUSED(tmpreg); \
                                        } while(0)
#define __HAL_RCC_GPIOK_CLK_ENABLE()    do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOKEN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOKEN);\
                                        UNUSED(tmpreg); \
                                        } while(0)
#define __HAL_RCC_DMA2D_CLK_ENABLE()    do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA2DEN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA2DEN);\
                                        UNUSED(tmpreg); \
                                        } while(0)
#define __HAL_RCC_ETHMAC_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHMACEN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHMACEN);\
                                        UNUSED(tmpreg); \
                                        } while(0)
#define __HAL_RCC_ETHMACTX_CLK_ENABLE() do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHMACTXEN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHMACTXEN);\
                                        UNUSED(tmpreg); \
                                        } while(0)
#define __HAL_RCC_ETHMACRX_CLK_ENABLE() do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHMACRXEN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHMACRXEN);\
                                         UNUSED(tmpreg); \
                                         } while(0)
#define __HAL_RCC_ETHMACPTP_CLK_ENABLE() do { \
                                         __IO uint32_t tmpreg; \
                                         SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHMACPTPEN);\
                                         /* Delay after an RCC peripheral clock enabling */ \
                                         tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHMACPTPEN);\
                                         UNUSED(tmpreg); \
                                         } while(0)
#define __HAL_RCC_USB_OTG_HS_CLK_ENABLE() do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_OTGHSEN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_OTGHSEN);\
                                        UNUSED(tmpreg); \
                                        } while(0)
#define __HAL_RCC_USB_OTG_HS_ULPI_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_OTGHSULPIEN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_OTGHSULPIEN);\
                                        UNUSED(tmpreg); \
                                        } while(0)

#define __HAL_RCC_GPIOF_CLK_DISABLE()           (RCC->AHB1ENR &= ~(RCC_AHB1ENR_GPIOFEN))
#define __HAL_RCC_GPIOG_CLK_DISABLE()           (RCC->AHB1ENR &= ~(RCC_AHB1ENR_GPIOGEN))
#define __HAL_RCC_GPIOI_CLK_DISABLE()           (RCC->AHB1ENR &= ~(RCC_AHB1ENR_GPIOIEN))
#define __HAL_RCC_GPIOJ_CLK_DISABLE()           (RCC->AHB1ENR &= ~(RCC_AHB1ENR_GPIOJEN))
#define __HAL_RCC_GPIOK_CLK_DISABLE()           (RCC->AHB1ENR &= ~(RCC_AHB1ENR_GPIOKEN))
#define __HAL_RCC_DMA2D_CLK_DISABLE()           (RCC->AHB1ENR &= ~(RCC_AHB1ENR_DMA2DEN))
#define __HAL_RCC_ETHMAC_CLK_DISABLE()          (RCC->AHB1ENR &= ~(RCC_AHB1ENR_ETHMACEN))
#define __HAL_RCC_ETHMACTX_CLK_DISABLE()        (RCC->AHB1ENR &= ~(RCC_AHB1ENR_ETHMACTXEN))
#define __HAL_RCC_ETHMACRX_CLK_DISABLE()        (RCC->AHB1ENR &= ~(RCC_AHB1ENR_ETHMACRXEN))
#define __HAL_RCC_ETHMACPTP_CLK_DISABLE()       (RCC->AHB1ENR &= ~(RCC_AHB1ENR_ETHMACPTPEN))
#define __HAL_RCC_USB_OTG_HS_CLK_DISABLE()      (RCC->AHB1ENR &= ~(RCC_AHB1ENR_OTGHSEN))
#define __HAL_RCC_USB_OTG_HS_ULPI_CLK_DISABLE() (RCC->AHB1ENR &= ~(RCC_AHB1ENR_OTGHSULPIEN))

/**
  * @brief  Enable ETHERNET clock.
  */
#define __HAL_RCC_ETH_CLK_ENABLE() do {                                     \
                                        __HAL_RCC_ETHMAC_CLK_ENABLE();      \
                                        __HAL_RCC_ETHMACTX_CLK_ENABLE();    \
                                        __HAL_RCC_ETHMACRX_CLK_ENABLE();    \
                                      } while(0)
/**
  * @brief  Disable ETHERNET clock.
  */
#define __HAL_RCC_ETH_CLK_DISABLE()  do {                                      \
                                          __HAL_RCC_ETHMACTX_CLK_DISABLE();    \
                                          __HAL_RCC_ETHMACRX_CLK_DISABLE();    \
                                          __HAL_RCC_ETHMAC_CLK_DISABLE();      \
                                        } while(0)
                                     
/** @brief  Enable or disable the AHB2 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.
  */
 
#define __HAL_RCC_DCMI_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_DCMIEN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_DCMIEN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_DCMI_CLK_DISABLE()  (RCC->AHB2ENR &= ~(RCC_AHB2ENR_DCMIEN))

#if defined(STM32F437xx)|| defined(STM32F439xx)
#define __HAL_RCC_CRYP_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_CRYPEN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_CRYPEN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_HASH_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_HASHEN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_HASHEN);\
                                      UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCC_CRYP_CLK_DISABLE()  (RCC->AHB2ENR &= ~(RCC_AHB2ENR_CRYPEN))
#define __HAL_RCC_HASH_CLK_DISABLE()  (RCC->AHB2ENR &= ~(RCC_AHB2ENR_HASHEN))
#endif /* STM32F437xx || STM32F439xx */

/** @brief  Enables or disables the AHB3 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it. 
  */
#define __HAL_RCC_FMC_CLK_ENABLE()    do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->AHB3ENR, RCC_AHB3ENR_FMCEN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->AHB3ENR, RCC_AHB3ENR_FMCEN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_FMC_CLK_DISABLE()  (RCC->AHB3ENR &= ~(RCC_AHB3ENR_FMCEN))

/** @brief  Enable or disable the Low Speed APB (APB1) peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it. 
  */
#define __HAL_RCC_TIM6_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM6EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM6EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_TIM7_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM7EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM7EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_TIM12_CLK_ENABLE()  do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM12EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM12EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_TIM13_CLK_ENABLE()  do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM13EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM13EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_TIM14_CLK_ENABLE()  do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM14EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM14EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_TIM14_CLK_ENABLE()  do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM14EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM14EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_USART3_CLK_ENABLE() do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART3EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_USART3EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_UART4_CLK_ENABLE()  do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_UART4EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_UART4EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_UART5_CLK_ENABLE()  do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_UART5EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_UART5EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_CAN1_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_CAN1EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_CAN1EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_CAN2_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_CAN2EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_CAN2EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_DAC_CLK_ENABLE()    do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_DACEN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_DACEN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_UART7_CLK_ENABLE()  do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_UART7EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_UART7EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_UART8_CLK_ENABLE()  do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_UART8EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_UART8EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCC_TIM6_CLK_DISABLE()   (RCC->APB1ENR &= ~(RCC_APB1ENR_TIM6EN))
#define __HAL_RCC_TIM7_CLK_DISABLE()   (RCC->APB1ENR &= ~(RCC_APB1ENR_TIM7EN))
#define __HAL_RCC_TIM12_CLK_DISABLE()  (RCC->APB1ENR &= ~(RCC_APB1ENR_TIM12EN))
#define __HAL_RCC_TIM13_CLK_DISABLE()  (RCC->APB1ENR &= ~(RCC_APB1ENR_TIM13EN))
#define __HAL_RCC_TIM14_CLK_DISABLE()  (RCC->APB1ENR &= ~(RCC_APB1ENR_TIM14EN))
#define __HAL_RCC_USART3_CLK_DISABLE() (RCC->APB1ENR &= ~(RCC_APB1ENR_USART3EN))
#define __HAL_RCC_UART4_CLK_DISABLE()  (RCC->APB1ENR &= ~(RCC_APB1ENR_UART4EN))
#define __HAL_RCC_UART5_CLK_DISABLE()  (RCC->APB1ENR &= ~(RCC_APB1ENR_UART5EN))
#define __HAL_RCC_CAN1_CLK_DISABLE()   (RCC->APB1ENR &= ~(RCC_APB1ENR_CAN1EN))
#define __HAL_RCC_CAN2_CLK_DISABLE()   (RCC->APB1ENR &= ~(RCC_APB1ENR_CAN2EN))
#define __HAL_RCC_DAC_CLK_DISABLE()    (RCC->APB1ENR &= ~(RCC_APB1ENR_DACEN))
#define __HAL_RCC_UART7_CLK_DISABLE()  (RCC->APB1ENR &= ~(RCC_APB1ENR_UART7EN))
#define __HAL_RCC_UART8_CLK_DISABLE()  (RCC->APB1ENR &= ~(RCC_APB1ENR_UART8EN))

/** @brief  Enable or disable the High Speed APB (APB2) peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.
  */
#define __HAL_RCC_TIM8_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM8EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM8EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_ADC2_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC2EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC2EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_ADC3_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC3EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC3EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_SPI5_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI5EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI5EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_SPI6_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI6EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI6EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_SAI1_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SAI1EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SAI1EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCC_TIM8_CLK_DISABLE() (RCC->APB2ENR &= ~(RCC_APB2ENR_TIM8EN))
#define __HAL_RCC_ADC2_CLK_DISABLE() (RCC->APB2ENR &= ~(RCC_APB2ENR_ADC2EN))
#define __HAL_RCC_ADC3_CLK_DISABLE() (RCC->APB2ENR &= ~(RCC_APB2ENR_ADC3EN))
#define __HAL_RCC_SPI5_CLK_DISABLE() (RCC->APB2ENR &= ~(RCC_APB2ENR_SPI5EN))
#define __HAL_RCC_SPI6_CLK_DISABLE() (RCC->APB2ENR &= ~(RCC_APB2ENR_SPI6EN))
#define __HAL_RCC_SAI1_CLK_DISABLE() (RCC->APB2ENR &= ~(RCC_APB2ENR_SAI1EN))

#if defined(STM32F429xx)|| defined(STM32F439xx)
#define __HAL_RCC_LTDC_CLK_ENABLE()  (RCC->APB2ENR |= (RCC_APB2ENR_LTDCEN))

#define __HAL_RCC_LTDC_CLK_DISABLE() (RCC->APB2ENR &= ~(RCC_APB2ENR_LTDCEN))
#endif /* STM32F429xx || STM32F439xx */

/** @brief  Force or release AHB1 peripheral reset.
  */  
#define __HAL_RCC_GPIOF_FORCE_RESET()    (RCC->AHB1RSTR |= (RCC_AHB1RSTR_GPIOFRST))
#define __HAL_RCC_GPIOG_FORCE_RESET()    (RCC->AHB1RSTR |= (RCC_AHB1RSTR_GPIOGRST))
#define __HAL_RCC_GPIOI_FORCE_RESET()    (RCC->AHB1RSTR |= (RCC_AHB1RSTR_GPIOIRST))
#define __HAL_RCC_ETHMAC_FORCE_RESET()   (RCC->AHB1RSTR |= (RCC_AHB1RSTR_ETHMACRST))
#define __HAL_RCC_USB_OTG_HS_FORCE_RESET()    (RCC->AHB1RSTR |= (RCC_AHB1RSTR_OTGHRST))
#define __HAL_RCC_GPIOJ_FORCE_RESET()    (RCC->AHB1RSTR |= (RCC_AHB1RSTR_GPIOJRST))
#define __HAL_RCC_GPIOK_FORCE_RESET()    (RCC->AHB1RSTR |= (RCC_AHB1RSTR_GPIOKRST))
#define __HAL_RCC_DMA2D_FORCE_RESET()    (RCC->AHB1RSTR |= (RCC_AHB1RSTR_DMA2DRST))

#define __HAL_RCC_GPIOF_RELEASE_RESET()  (RCC->AHB1RSTR &= ~(RCC_AHB1RSTR_GPIOFRST))
#define __HAL_RCC_GPIOG_RELEASE_RESET()  (RCC->AHB1RSTR &= ~(RCC_AHB1RSTR_GPIOGRST))
#define __HAL_RCC_GPIOI_RELEASE_RESET()  (RCC->AHB1RSTR &= ~(RCC_AHB1RSTR_GPIOIRST))
#define __HAL_RCC_ETHMAC_RELEASE_RESET() (RCC->AHB1RSTR &= ~(RCC_AHB1RSTR_ETHMACRST))
#define __HAL_RCC_USB_OTG_HS_RELEASE_RESET()  (RCC->AHB1RSTR &= ~(RCC_AHB1RSTR_OTGHRST))
#define __HAL_RCC_GPIOJ_RELEASE_RESET()  (RCC->AHB1RSTR &= ~(RCC_AHB1RSTR_GPIOJRST))
#define __HAL_RCC_GPIOK_RELEASE_RESET()  (RCC->AHB1RSTR &= ~(RCC_AHB1RSTR_GPIOKRST))
#define __HAL_RCC_DMA2D_RELEASE_RESET()  (RCC->AHB1RSTR &= ~(RCC_AHB1RSTR_DMA2DRST))
 
/** @brief  Force or release AHB2 peripheral reset.
  */
#define __HAL_RCC_DCMI_FORCE_RESET()   (RCC->AHB2RSTR |= (RCC_AHB2RSTR_DCMIRST))
#define __HAL_RCC_DCMI_RELEASE_RESET() (RCC->AHB2RSTR &= ~(RCC_AHB2RSTR_DCMIRST))

#if defined(STM32F437xx)|| defined(STM32F439xx)
#define __HAL_RCC_CRYP_FORCE_RESET()   (RCC->AHB2RSTR |= (RCC_AHB2RSTR_CRYPRST))
#define __HAL_RCC_HASH_FORCE_RESET()   (RCC->AHB2RSTR |= (RCC_AHB2RSTR_HASHRST))

#define __HAL_RCC_CRYP_RELEASE_RESET() (RCC->AHB2RSTR &= ~(RCC_AHB2RSTR_CRYPRST))
#define __HAL_RCC_HASH_RELEASE_RESET() (RCC->AHB2RSTR &= ~(RCC_AHB2RSTR_HASHRST))
#endif /* STM32F437xx || STM32F439xx */

/** @brief  Force or release AHB3 peripheral reset
  */ 
#define __HAL_RCC_FMC_FORCE_RESET()    (RCC->AHB3RSTR |= (RCC_AHB3RSTR_FMCRST))
#define __HAL_RCC_FMC_RELEASE_RESET()  (RCC->AHB3RSTR &= ~(RCC_AHB3RSTR_FMCRST))
 
/** @brief  Force or release APB1 peripheral reset.
  */ 
#define __HAL_RCC_TIM6_FORCE_RESET()     (RCC->APB1RSTR |= (RCC_APB1RSTR_TIM6RST))
#define __HAL_RCC_TIM7_FORCE_RESET()     (RCC->APB1RSTR |= (RCC_APB1RSTR_TIM7RST))
#define __HAL_RCC_TIM12_FORCE_RESET()    (RCC->APB1RSTR |= (RCC_APB1RSTR_TIM12RST))
#define __HAL_RCC_TIM13_FORCE_RESET()    (RCC->APB1RSTR |= (RCC_APB1RSTR_TIM13RST))
#define __HAL_RCC_TIM14_FORCE_RESET()    (RCC->APB1RSTR |= (RCC_APB1RSTR_TIM14RST))
#define __HAL_RCC_USART3_FORCE_RESET()   (RCC->APB1RSTR |= (RCC_APB1RSTR_USART3RST))
#define __HAL_RCC_UART4_FORCE_RESET()    (RCC->APB1RSTR |= (RCC_APB1RSTR_UART4RST))
#define __HAL_RCC_UART5_FORCE_RESET()    (RCC->APB1RSTR |= (RCC_APB1RSTR_UART5RST))
#define __HAL_RCC_CAN1_FORCE_RESET()     (RCC->APB1RSTR |= (RCC_APB1RSTR_CAN1RST))
#define __HAL_RCC_CAN2_FORCE_RESET()     (RCC->APB1RSTR |= (RCC_APB1RSTR_CAN2RST))
#define __HAL_RCC_DAC_FORCE_RESET()      (RCC->APB1RSTR |= (RCC_APB1RSTR_DACRST))
#define __HAL_RCC_UART7_FORCE_RESET()    (RCC->APB1RSTR |= (RCC_APB1RSTR_UART7RST))
#define __HAL_RCC_UART8_FORCE_RESET()    (RCC->APB1RSTR |= (RCC_APB1RSTR_UART8RST))

#define __HAL_RCC_TIM6_RELEASE_RESET()   (RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM6RST))
#define __HAL_RCC_TIM7_RELEASE_RESET()   (RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM7RST))
#define __HAL_RCC_TIM12_RELEASE_RESET()  (RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM12RST))
#define __HAL_RCC_TIM13_RELEASE_RESET()  (RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM13RST))
#define __HAL_RCC_TIM14_RELEASE_RESET()  (RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM14RST))
#define __HAL_RCC_USART3_RELEASE_RESET() (RCC->APB1RSTR &= ~(RCC_APB1RSTR_USART3RST))
#define __HAL_RCC_UART4_RELEASE_RESET()  (RCC->APB1RSTR &= ~(RCC_APB1RSTR_UART4RST))
#define __HAL_RCC_UART5_RELEASE_RESET()  (RCC->APB1RSTR &= ~(RCC_APB1RSTR_UART5RST))
#define __HAL_RCC_CAN1_RELEASE_RESET()   (RCC->APB1RSTR &= ~(RCC_APB1RSTR_CAN1RST))
#define __HAL_RCC_CAN2_RELEASE_RESET()   (RCC->APB1RSTR &= ~(RCC_APB1RSTR_CAN2RST))
#define __HAL_RCC_DAC_RELEASE_RESET()    (RCC->APB1RSTR &= ~(RCC_APB1RSTR_DACRST))
#define __HAL_RCC_UART7_RELEASE_RESET()  (RCC->APB1RSTR &= ~(RCC_APB1RSTR_UART7RST))
#define __HAL_RCC_UART8_RELEASE_RESET()  (RCC->APB1RSTR &= ~(RCC_APB1RSTR_UART8RST))

/** @brief  Force or release APB2 peripheral reset.
  */
#define __HAL_RCC_TIM8_FORCE_RESET()   (RCC->APB2RSTR |= (RCC_APB2RSTR_TIM8RST))
#define __HAL_RCC_SPI5_FORCE_RESET()   (RCC->APB2RSTR |= (RCC_APB2RSTR_SPI5RST))
#define __HAL_RCC_SPI6_FORCE_RESET()   (RCC->APB2RSTR |= (RCC_APB2RSTR_SPI6RST))
#define __HAL_RCC_SAI1_FORCE_RESET()   (RCC->APB2RSTR |= (RCC_APB2RSTR_SAI1RST))

#define __HAL_RCC_TIM8_RELEASE_RESET() (RCC->APB2RSTR &= ~(RCC_APB2RSTR_TIM8RST))
#define __HAL_RCC_SPI5_RELEASE_RESET() (RCC->APB2RSTR &= ~(RCC_APB2RSTR_SPI5RST))
#define __HAL_RCC_SPI6_RELEASE_RESET() (RCC->APB2RSTR &= ~(RCC_APB2RSTR_SPI6RST))
#define __HAL_RCC_SAI1_RELEASE_RESET() (RCC->APB2RSTR &= ~(RCC_APB2RSTR_SAI1RST))

#if defined(STM32F429xx)|| defined(STM32F439xx)
#define __HAL_RCC_LTDC_FORCE_RESET()   (RCC->APB2RSTR |= (RCC_APB2RSTR_LTDCRST))
#define __HAL_RCC_LTDC_RELEASE_RESET() (RCC->APB2RSTR &= ~(RCC_APB2RSTR_LTDCRST))
#endif /* STM32F429xx|| STM32F439xx */

/** @brief  Enable or disable the AHB1 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */ 
#define __HAL_RCC_GPIOF_CLK_SLEEP_ENABLE()      (RCC->AHB1LPENR |= (RCC_AHB1LPENR_GPIOFLPEN))
#define __HAL_RCC_GPIOG_CLK_SLEEP_ENABLE()      (RCC->AHB1LPENR |= (RCC_AHB1LPENR_GPIOGLPEN))
#define __HAL_RCC_GPIOI_CLK_SLEEP_ENABLE()      (RCC->AHB1LPENR |= (RCC_AHB1LPENR_GPIOILPEN))
#define __HAL_RCC_SRAM2_CLK_SLEEP_ENABLE()      (RCC->AHB1LPENR |= (RCC_AHB1LPENR_SRAM2LPEN))
#define __HAL_RCC_ETHMAC_CLK_SLEEP_ENABLE()     (RCC->AHB1LPENR |= (RCC_AHB1LPENR_ETHMACLPEN))
#define __HAL_RCC_ETHMACTX_CLK_SLEEP_ENABLE()   (RCC->AHB1LPENR |= (RCC_AHB1LPENR_ETHMACTXLPEN))
#define __HAL_RCC_ETHMACRX_CLK_SLEEP_ENABLE()   (RCC->AHB1LPENR |= (RCC_AHB1LPENR_ETHMACRXLPEN))
#define __HAL_RCC_ETHMACPTP_CLK_SLEEP_ENABLE()  (RCC->AHB1LPENR |= (RCC_AHB1LPENR_ETHMACPTPLPEN))
#define __HAL_RCC_USB_OTG_HS_CLK_SLEEP_ENABLE()      (RCC->AHB1LPENR |= (RCC_AHB1LPENR_OTGHSLPEN))
#define __HAL_RCC_USB_OTG_HS_ULPI_CLK_SLEEP_ENABLE()  (RCC->AHB1LPENR |= (RCC_AHB1LPENR_OTGHSULPILPEN))
#define __HAL_RCC_GPIOJ_CLK_SLEEP_ENABLE()      (RCC->AHB1LPENR |= (RCC_AHB1LPENR_GPIOJLPEN))
#define __HAL_RCC_GPIOK_CLK_SLEEP_ENABLE()      (RCC->AHB1LPENR |= (RCC_AHB1LPENR_GPIOKLPEN))
#define __HAL_RCC_SRAM3_CLK_SLEEP_ENABLE()      (RCC->AHB1LPENR |= (RCC_AHB1LPENR_SRAM3LPEN))
#define __HAL_RCC_DMA2D_CLK_SLEEP_ENABLE()      (RCC->AHB1LPENR |= (RCC_AHB1LPENR_DMA2DLPEN))

#define __HAL_RCC_GPIOF_CLK_SLEEP_DISABLE()     (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_GPIOFLPEN))
#define __HAL_RCC_GPIOG_CLK_SLEEP_DISABLE()     (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_GPIOGLPEN))
#define __HAL_RCC_GPIOI_CLK_SLEEP_DISABLE()     (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_GPIOILPEN))
#define __HAL_RCC_SRAM2_CLK_SLEEP_DISABLE()     (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_SRAM2LPEN))
#define __HAL_RCC_ETHMAC_CLK_SLEEP_DISABLE()    (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_ETHMACLPEN))
#define __HAL_RCC_ETHMACTX_CLK_SLEEP_DISABLE()  (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_ETHMACTXLPEN))
#define __HAL_RCC_ETHMACRX_CLK_SLEEP_DISABLE()  (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_ETHMACRXLPEN))
#define __HAL_RCC_ETHMACPTP_CLK_SLEEP_DISABLE() (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_ETHMACPTPLPEN))
#define __HAL_RCC_USB_OTG_HS_CLK_SLEEP_DISABLE()     (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_OTGHSLPEN))
#define __HAL_RCC_USB_OTG_HS_ULPI_CLK_SLEEP_DISABLE() (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_OTGHSULPILPEN))
#define __HAL_RCC_GPIOJ_CLK_SLEEP_DISABLE()     (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_GPIOJLPEN))
#define __HAL_RCC_GPIOK_CLK_SLEEP_DISABLE()     (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_GPIOKLPEN))
#define __HAL_RCC_DMA2D_CLK_SLEEP_DISABLE()     (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_DMA2DLPEN))

/** @brief  Enable or disable the AHB2 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */
#define __HAL_RCC_DCMI_CLK_SLEEP_ENABLE()  (RCC->AHB2LPENR |= (RCC_AHB2LPENR_DCMILPEN))
#define __HAL_RCC_DCMI_CLK_SLEEP_DISABLE() (RCC->AHB2LPENR &= ~(RCC_AHB2LPENR_DCMILPEN))

#if defined(STM32F437xx)|| defined(STM32F439xx)
#define __HAL_RCC_CRYP_CLK_SLEEP_ENABLE()  (RCC->AHB2LPENR |= (RCC_AHB2LPENR_CRYPLPEN))
#define __HAL_RCC_HASH_CLK_SLEEP_ENABLE()  (RCC->AHB2LPENR |= (RCC_AHB2LPENR_HASHLPEN))

#define __HAL_RCC_CRYP_CLK_SLEEP_DISABLE() (RCC->AHB2LPENR &= ~(RCC_AHB2LPENR_CRYPLPEN))
#define __HAL_RCC_HASH_CLK_SLEEP_DISABLE() (RCC->AHB2LPENR &= ~(RCC_AHB2LPENR_HASHLPEN))
#endif /* STM32F437xx || STM32F439xx */

/** @brief  Enable or disable the AHB3 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */
#define __HAL_RCC_FMC_CLK_SLEEP_ENABLE()  (RCC->AHB3LPENR |= (RCC_AHB3LPENR_FMCLPEN))
#define __HAL_RCC_FMC_CLK_SLEEP_DISABLE() (RCC->AHB3LPENR &= ~(RCC_AHB3LPENR_FMCLPEN))

/** @brief  Enable or disable the APB1 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */  
#define __HAL_RCC_TIM6_CLK_SLEEP_ENABLE()    (RCC->APB1LPENR |= (RCC_APB1LPENR_TIM6LPEN))
#define __HAL_RCC_TIM7_CLK_SLEEP_ENABLE()    (RCC->APB1LPENR |= (RCC_APB1LPENR_TIM7LPEN))
#define __HAL_RCC_TIM12_CLK_SLEEP_ENABLE()   (RCC->APB1LPENR |= (RCC_APB1LPENR_TIM12LPEN))
#define __HAL_RCC_TIM13_CLK_SLEEP_ENABLE()   (RCC->APB1LPENR |= (RCC_APB1LPENR_TIM13LPEN))
#define __HAL_RCC_TIM14_CLK_SLEEP_ENABLE()   (RCC->APB1LPENR |= (RCC_APB1LPENR_TIM14LPEN))
#define __HAL_RCC_USART3_CLK_SLEEP_ENABLE()  (RCC->APB1LPENR |= (RCC_APB1LPENR_USART3LPEN))
#define __HAL_RCC_UART4_CLK_SLEEP_ENABLE()   (RCC->APB1LPENR |= (RCC_APB1LPENR_UART4LPEN))
#define __HAL_RCC_UART5_CLK_SLEEP_ENABLE()   (RCC->APB1LPENR |= (RCC_APB1LPENR_UART5LPEN))
#define __HAL_RCC_CAN1_CLK_SLEEP_ENABLE()    (RCC->APB1LPENR |= (RCC_APB1LPENR_CAN1LPEN))
#define __HAL_RCC_CAN2_CLK_SLEEP_ENABLE()    (RCC->APB1LPENR |= (RCC_APB1LPENR_CAN2LPEN))
#define __HAL_RCC_DAC_CLK_SLEEP_ENABLE()     (RCC->APB1LPENR |= (RCC_APB1LPENR_DACLPEN))
#define __HAL_RCC_UART7_CLK_SLEEP_ENABLE()   (RCC->APB1LPENR |= (RCC_APB1LPENR_UART7LPEN))
#define __HAL_RCC_UART8_CLK_SLEEP_ENABLE()   (RCC->APB1LPENR |= (RCC_APB1LPENR_UART8LPEN))

#define __HAL_RCC_TIM6_CLK_SLEEP_DISABLE()   (RCC->APB1LPENR &= ~(RCC_APB1LPENR_TIM6LPEN))
#define __HAL_RCC_TIM7_CLK_SLEEP_DISABLE()   (RCC->APB1LPENR &= ~(RCC_APB1LPENR_TIM7LPEN))
#define __HAL_RCC_TIM12_CLK_SLEEP_DISABLE()  (RCC->APB1LPENR &= ~(RCC_APB1LPENR_TIM12LPEN))
#define __HAL_RCC_TIM13_CLK_SLEEP_DISABLE()  (RCC->APB1LPENR &= ~(RCC_APB1LPENR_TIM13LPEN))
#define __HAL_RCC_TIM14_CLK_SLEEP_DISABLE()  (RCC->APB1LPENR &= ~(RCC_APB1LPENR_TIM14LPEN))
#define __HAL_RCC_USART3_CLK_SLEEP_DISABLE() (RCC->APB1LPENR &= ~(RCC_APB1LPENR_USART3LPEN))
#define __HAL_RCC_UART4_CLK_SLEEP_DISABLE()  (RCC->APB1LPENR &= ~(RCC_APB1LPENR_UART4LPEN))
#define __HAL_RCC_UART5_CLK_SLEEP_DISABLE()  (RCC->APB1LPENR &= ~(RCC_APB1LPENR_UART5LPEN))
#define __HAL_RCC_CAN1_CLK_SLEEP_DISABLE()   (RCC->APB1LPENR &= ~(RCC_APB1LPENR_CAN1LPEN))
#define __HAL_RCC_CAN2_CLK_SLEEP_DISABLE()   (RCC->APB1LPENR &= ~(RCC_APB1LPENR_CAN2LPEN))
#define __HAL_RCC_DAC_CLK_SLEEP_DISABLE()    (RCC->APB1LPENR &= ~(RCC_APB1LPENR_DACLPEN))
#define __HAL_RCC_UART7_CLK_SLEEP_DISABLE()   (RCC->APB1LPENR &= ~(RCC_APB1LPENR_UART7LPEN))
#define __HAL_RCC_UART8_CLK_SLEEP_DISABLE()   (RCC->APB1LPENR &= ~(RCC_APB1LPENR_UART8LPEN))

/** @brief  Enable or disable the APB2 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */ 
#define __HAL_RCC_TIM8_CLK_SLEEP_ENABLE() (RCC->APB2LPENR |= (RCC_APB2LPENR_TIM8LPEN))
#define __HAL_RCC_ADC2_CLK_SLEEP_ENABLE() (RCC->APB2LPENR |= (RCC_APB2LPENR_ADC2LPEN))
#define __HAL_RCC_ADC3_CLK_SLEEP_ENABLE() (RCC->APB2LPENR |= (RCC_APB2LPENR_ADC3LPEN))
#define __HAL_RCC_SPI5_CLK_SLEEP_ENABLE() (RCC->APB2LPENR |= (RCC_APB2LPENR_SPI5LPEN))
#define __HAL_RCC_SPI6_CLK_SLEEP_ENABLE() (RCC->APB2LPENR |= (RCC_APB2LPENR_SPI6LPEN))
#define __HAL_RCC_SAI1_CLK_SLEEP_ENABLE() (RCC->APB2LPENR |= (RCC_APB2LPENR_SAI1LPEN))

#define __HAL_RCC_TIM8_CLK_SLEEP_DISABLE() (RCC->APB2LPENR &= ~(RCC_APB2LPENR_TIM8LPEN))
#define __HAL_RCC_ADC2_CLK_SLEEP_DISABLE() (RCC->APB2LPENR &= ~(RCC_APB2LPENR_ADC2LPEN))
#define __HAL_RCC_ADC3_CLK_SLEEP_DISABLE() (RCC->APB2LPENR &= ~(RCC_APB2LPENR_ADC3LPEN))
#define __HAL_RCC_SPI5_CLK_SLEEP_DISABLE() (RCC->APB2LPENR &= ~(RCC_APB2LPENR_SPI5LPEN))
#define __HAL_RCC_SPI6_CLK_SLEEP_DISABLE() (RCC->APB2LPENR &= ~(RCC_APB2LPENR_SPI6LPEN))
#define __HAL_RCC_SAI1_CLK_SLEEP_DISABLE() (RCC->APB2LPENR &= ~(RCC_APB2LPENR_SAI1LPEN))

#if defined(STM32F429xx)|| defined(STM32F439xx)
#define __HAL_RCC_LTDC_CLK_SLEEP_ENABLE() (RCC->APB2LPENR |= (RCC_APB2LPENR_LTDCLPEN))

#define __HAL_RCC_LTDC_CLK_SLEEP_DISABLE() (RCC->APB2LPENR &= ~(RCC_APB2LPENR_LTDCLPEN))
#endif /* STM32F429xx || STM32F439xx */

#endif /* STM32F427xx || STM32F437xx || STM32F429xx|| STM32F439xx */
/*---------------------------------------------------------------------------------------------*/

/*----------------------------------- STM32F40xxx/STM32F41xxx----------------------------------*/
#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx)|| defined(STM32F417xx)
/** @brief  Enables or disables the AHB1 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.
  */
#define __HAL_RCC_GPIOI_CLK_ENABLE()   do { \
                                       __IO uint32_t tmpreg; \
                                       SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOIEN);\
                                       /* Delay after an RCC peripheral clock enabling */ \
                                       tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOIEN);\
                                       UNUSED(tmpreg); \
                                       } while(0)
#define __HAL_RCC_GPIOF_CLK_ENABLE()   do { \
                                       __IO uint32_t tmpreg; \
                                       SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOFEN);\
                                       /* Delay after an RCC peripheral clock enabling */ \
                                       tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOFEN);\
                                       UNUSED(tmpreg); \
                                       } while(0)
#define __HAL_RCC_GPIOG_CLK_ENABLE()   do { \
                                       __IO uint32_t tmpreg; \
                                       SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOGEN);\
                                       /* Delay after an RCC peripheral clock enabling */ \
                                       tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOGEN);\
                                       UNUSED(tmpreg); \
                                       } while(0)
#define __HAL_RCC_USB_OTG_HS_CLK_ENABLE()   do { \
                                       __IO uint32_t tmpreg; \
                                       SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_OTGHSEN);\
                                       /* Delay after an RCC peripheral clock enabling */ \
                                       tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_OTGHSEN);\
                                       UNUSED(tmpreg); \
                                       } while(0)
#define __HAL_RCC_USB_OTG_HS_ULPI_CLK_ENABLE()   do { \
                                       __IO uint32_t tmpreg; \
                                       SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_OTGHSULPIEN);\
                                       /* Delay after an RCC peripheral clock enabling */ \
                                       tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_OTGHSULPIEN);\
                                       UNUSED(tmpreg); \
                                       } while(0)

#define __HAL_RCC_GPIOF_CLK_DISABLE()           (RCC->AHB1ENR &= ~(RCC_AHB1ENR_GPIOFEN))
#define __HAL_RCC_GPIOG_CLK_DISABLE()           (RCC->AHB1ENR &= ~(RCC_AHB1ENR_GPIOGEN))
#define __HAL_RCC_GPIOI_CLK_DISABLE()           (RCC->AHB1ENR &= ~(RCC_AHB1ENR_GPIOIEN))
#define __HAL_RCC_USB_OTG_HS_CLK_DISABLE()      (RCC->AHB1ENR &= ~(RCC_AHB1ENR_OTGHSEN))
#define __HAL_RCC_USB_OTG_HS_ULPI_CLK_DISABLE() (RCC->AHB1ENR &= ~(RCC_AHB1ENR_OTGHSULPIEN))

#if defined(STM32F407xx)|| defined(STM32F417xx)
/**
  * @brief  Enable ETHERNET clock.
  */
#define __HAL_RCC_ETHMAC_CLK_ENABLE()  do { \
                                       __IO uint32_t tmpreg; \
                                       SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHMACEN);\
                                       /* Delay after an RCC peripheral clock enabling */ \
                                       tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHMACEN);\
                                       UNUSED(tmpreg); \
                                       } while(0)
#define __HAL_RCC_ETHMACTX_CLK_ENABLE() do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHMACTXEN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHMACTXEN);\
                                        UNUSED(tmpreg); \
                                        } while(0)
#define __HAL_RCC_ETHMACRX_CLK_ENABLE() do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHMACRXEN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHMACRXEN);\
                                        UNUSED(tmpreg); \
                                        } while(0)
#define __HAL_RCC_ETHMACPTP_CLK_ENABLE() do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHMACPTPEN);\
                                        /* Delay after an RCC peripheral clock enabling */ \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHMACPTPEN);\
                                        UNUSED(tmpreg); \
                                        } while(0)
#define __HAL_RCC_ETH_CLK_ENABLE()      do {                            \
                                        __HAL_RCC_ETHMAC_CLK_ENABLE();      \
                                        __HAL_RCC_ETHMACTX_CLK_ENABLE();    \
                                        __HAL_RCC_ETHMACRX_CLK_ENABLE();    \
                                        } while(0)

/**
  * @brief  Disable ETHERNET clock.
  */
#define __HAL_RCC_ETHMAC_CLK_DISABLE()    (RCC->AHB1ENR &= ~(RCC_AHB1ENR_ETHMACEN))
#define __HAL_RCC_ETHMACTX_CLK_DISABLE()  (RCC->AHB1ENR &= ~(RCC_AHB1ENR_ETHMACTXEN))
#define __HAL_RCC_ETHMACRX_CLK_DISABLE()  (RCC->AHB1ENR &= ~(RCC_AHB1ENR_ETHMACRXEN))
#define __HAL_RCC_ETHMACPTP_CLK_DISABLE() (RCC->AHB1ENR &= ~(RCC_AHB1ENR_ETHMACPTPEN))  
#define __HAL_RCC_ETH_CLK_DISABLE()       do {                             \
                                      __HAL_RCC_ETHMACTX_CLK_DISABLE();    \
                                      __HAL_RCC_ETHMACRX_CLK_DISABLE();    \
                                      __HAL_RCC_ETHMAC_CLK_DISABLE();      \
                                     } while(0)
#endif /* STM32F407xx || STM32F417xx */
 
/** @brief  Enable or disable the AHB2 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.
  */
#if defined(STM32F407xx)|| defined(STM32F417xx) 
#define __HAL_RCC_DCMI_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_DCMIEN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_DCMIEN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_DCMI_CLK_DISABLE()  (RCC->AHB2ENR &= ~(RCC_AHB2ENR_DCMIEN))
#endif /* STM32F407xx || STM32F417xx */

#if defined(STM32F415xx) || defined(STM32F417xx)
#define __HAL_RCC_CRYP_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_CRYPEN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_CRYPEN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_HASH_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_HASHEN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_HASHEN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_CRYP_CLK_DISABLE()  (RCC->AHB2ENR &= ~(RCC_AHB2ENR_CRYPEN))
#define __HAL_RCC_HASH_CLK_DISABLE()  (RCC->AHB2ENR &= ~(RCC_AHB2ENR_HASHEN))
#endif /* STM32F415xx || STM32F417xx */

/** @brief  Enables or disables the AHB3 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it. 
  */
#define __HAL_RCC_FSMC_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->AHB3ENR, RCC_AHB3ENR_FSMCEN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->AHB3ENR, RCC_AHB3ENR_FSMCEN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_FSMC_CLK_DISABLE() (RCC->AHB3ENR &= ~(RCC_AHB3ENR_FSMCEN))

/** @brief  Enable or disable the Low Speed APB (APB1) peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it. 
  */
#define __HAL_RCC_TIM6_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM6EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM6EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_TIM7_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM7EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM7EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_TIM12_CLK_ENABLE()  do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM12EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM12EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_TIM13_CLK_ENABLE()  do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM13EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM13EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_TIM14_CLK_ENABLE()  do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM14EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM14EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_USART3_CLK_ENABLE() do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART3EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_USART3EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_UART4_CLK_ENABLE()  do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_UART4EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_UART4EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_UART5_CLK_ENABLE()  do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_UART5EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_UART5EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_CAN1_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_CAN1EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_CAN1EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_CAN2_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_CAN2EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_CAN2EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_DAC_CLK_ENABLE()    do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_DACEN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_DACEN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
									  
#define __HAL_RCC_TIM6_CLK_DISABLE()   (RCC->APB1ENR &= ~(RCC_APB1ENR_TIM6EN))
#define __HAL_RCC_TIM7_CLK_DISABLE()   (RCC->APB1ENR &= ~(RCC_APB1ENR_TIM7EN))
#define __HAL_RCC_TIM12_CLK_DISABLE()  (RCC->APB1ENR &= ~(RCC_APB1ENR_TIM12EN))
#define __HAL_RCC_TIM13_CLK_DISABLE()  (RCC->APB1ENR &= ~(RCC_APB1ENR_TIM13EN))
#define __HAL_RCC_TIM14_CLK_DISABLE()  (RCC->APB1ENR &= ~(RCC_APB1ENR_TIM14EN))
#define __HAL_RCC_USART3_CLK_DISABLE() (RCC->APB1ENR &= ~(RCC_APB1ENR_USART3EN))
#define __HAL_RCC_UART4_CLK_DISABLE()  (RCC->APB1ENR &= ~(RCC_APB1ENR_UART4EN))
#define __HAL_RCC_UART5_CLK_DISABLE()  (RCC->APB1ENR &= ~(RCC_APB1ENR_UART5EN))
#define __HAL_RCC_CAN1_CLK_DISABLE()   (RCC->APB1ENR &= ~(RCC_APB1ENR_CAN1EN))
#define __HAL_RCC_CAN2_CLK_DISABLE()   (RCC->APB1ENR &= ~(RCC_APB1ENR_CAN2EN))
#define __HAL_RCC_DAC_CLK_DISABLE()    (RCC->APB1ENR &= ~(RCC_APB1ENR_DACEN))

/** @brief  Enable or disable the High Speed APB (APB2) peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.
  */
#define __HAL_RCC_TIM8_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM8EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM8EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_ADC2_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC2EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC2EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_ADC3_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC3EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC3EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCC_TIM8_CLK_DISABLE() (RCC->APB2ENR &= ~(RCC_APB2ENR_TIM8EN))
#define __HAL_RCC_ADC2_CLK_DISABLE() (RCC->APB2ENR &= ~(RCC_APB2ENR_ADC2EN))
#define __HAL_RCC_ADC3_CLK_DISABLE() (RCC->APB2ENR &= ~(RCC_APB2ENR_ADC3EN))

/** @brief  Force or release AHB1 peripheral reset.
  */  
#define __HAL_RCC_GPIOF_FORCE_RESET()    (RCC->AHB1RSTR |= (RCC_AHB1RSTR_GPIOFRST))
#define __HAL_RCC_GPIOG_FORCE_RESET()    (RCC->AHB1RSTR |= (RCC_AHB1RSTR_GPIOGRST))
#define __HAL_RCC_GPIOI_FORCE_RESET()    (RCC->AHB1RSTR |= (RCC_AHB1RSTR_GPIOIRST))
#define __HAL_RCC_ETHMAC_FORCE_RESET()   (RCC->AHB1RSTR |= (RCC_AHB1RSTR_ETHMACRST))
#define __HAL_RCC_USB_OTG_HS_FORCE_RESET()    (RCC->AHB1RSTR |= (RCC_AHB1RSTR_OTGHRST))

#define __HAL_RCC_GPIOF_RELEASE_RESET()  (RCC->AHB1RSTR &= ~(RCC_AHB1RSTR_GPIOFRST))
#define __HAL_RCC_GPIOG_RELEASE_RESET()  (RCC->AHB1RSTR &= ~(RCC_AHB1RSTR_GPIOGRST))
#define __HAL_RCC_GPIOI_RELEASE_RESET()  (RCC->AHB1RSTR &= ~(RCC_AHB1RSTR_GPIOIRST))
#define __HAL_RCC_ETHMAC_RELEASE_RESET() (RCC->AHB1RSTR &= ~(RCC_AHB1RSTR_ETHMACRST))
#define __HAL_RCC_USB_OTG_HS_RELEASE_RESET()  (RCC->AHB1RSTR &= ~(RCC_AHB1RSTR_OTGHRST))

/** @brief  Force or release AHB2 peripheral reset.
  */
#if defined(STM32F407xx)|| defined(STM32F417xx)  
#define __HAL_RCC_DCMI_FORCE_RESET()   (RCC->AHB2RSTR |= (RCC_AHB2RSTR_DCMIRST))
#define __HAL_RCC_DCMI_RELEASE_RESET() (RCC->AHB2RSTR &= ~(RCC_AHB2RSTR_DCMIRST))
#endif /* STM32F407xx || STM32F417xx */

#if defined(STM32F415xx) || defined(STM32F417xx) 
#define __HAL_RCC_CRYP_FORCE_RESET()   (RCC->AHB2RSTR |= (RCC_AHB2RSTR_CRYPRST))
#define __HAL_RCC_HASH_FORCE_RESET()   (RCC->AHB2RSTR |= (RCC_AHB2RSTR_HASHRST))

#define __HAL_RCC_CRYP_RELEASE_RESET() (RCC->AHB2RSTR &= ~(RCC_AHB2RSTR_CRYPRST))
#define __HAL_RCC_HASH_RELEASE_RESET() (RCC->AHB2RSTR &= ~(RCC_AHB2RSTR_HASHRST))

#endif /* STM32F415xx || STM32F417xx */

/** @brief  Force or release AHB3 peripheral reset
  */
#define __HAL_RCC_FSMC_FORCE_RESET()   (RCC->AHB3RSTR |= (RCC_AHB3RSTR_FSMCRST))
#define __HAL_RCC_FSMC_RELEASE_RESET() (RCC->AHB3RSTR &= ~(RCC_AHB3RSTR_FSMCRST))

/** @brief  Force or release APB1 peripheral reset.
  */ 
#define __HAL_RCC_TIM6_FORCE_RESET()     (RCC->APB1RSTR |= (RCC_APB1RSTR_TIM6RST))
#define __HAL_RCC_TIM7_FORCE_RESET()     (RCC->APB1RSTR |= (RCC_APB1RSTR_TIM7RST))
#define __HAL_RCC_TIM12_FORCE_RESET()    (RCC->APB1RSTR |= (RCC_APB1RSTR_TIM12RST))
#define __HAL_RCC_TIM13_FORCE_RESET()    (RCC->APB1RSTR |= (RCC_APB1RSTR_TIM13RST))
#define __HAL_RCC_TIM14_FORCE_RESET()    (RCC->APB1RSTR |= (RCC_APB1RSTR_TIM14RST))
#define __HAL_RCC_USART3_FORCE_RESET()   (RCC->APB1RSTR |= (RCC_APB1RSTR_USART3RST))
#define __HAL_RCC_UART4_FORCE_RESET()    (RCC->APB1RSTR |= (RCC_APB1RSTR_UART4RST))
#define __HAL_RCC_UART5_FORCE_RESET()    (RCC->APB1RSTR |= (RCC_APB1RSTR_UART5RST))
#define __HAL_RCC_CAN1_FORCE_RESET()     (RCC->APB1RSTR |= (RCC_APB1RSTR_CAN1RST))
#define __HAL_RCC_CAN2_FORCE_RESET()     (RCC->APB1RSTR |= (RCC_APB1RSTR_CAN2RST))
#define __HAL_RCC_DAC_FORCE_RESET()      (RCC->APB1RSTR |= (RCC_APB1RSTR_DACRST))

#define __HAL_RCC_TIM6_RELEASE_RESET()   (RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM6RST))
#define __HAL_RCC_TIM7_RELEASE_RESET()   (RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM7RST))
#define __HAL_RCC_TIM12_RELEASE_RESET()  (RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM12RST))
#define __HAL_RCC_TIM13_RELEASE_RESET()  (RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM13RST))
#define __HAL_RCC_TIM14_RELEASE_RESET()  (RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM14RST))
#define __HAL_RCC_USART3_RELEASE_RESET() (RCC->APB1RSTR &= ~(RCC_APB1RSTR_USART3RST))
#define __HAL_RCC_UART4_RELEASE_RESET()  (RCC->APB1RSTR &= ~(RCC_APB1RSTR_UART4RST))
#define __HAL_RCC_UART5_RELEASE_RESET()  (RCC->APB1RSTR &= ~(RCC_APB1RSTR_UART5RST))
#define __HAL_RCC_CAN1_RELEASE_RESET()   (RCC->APB1RSTR &= ~(RCC_APB1RSTR_CAN1RST))
#define __HAL_RCC_CAN2_RELEASE_RESET()   (RCC->APB1RSTR &= ~(RCC_APB1RSTR_CAN2RST))
#define __HAL_RCC_DAC_RELEASE_RESET()    (RCC->APB1RSTR &= ~(RCC_APB1RSTR_DACRST))

/** @brief  Force or release APB2 peripheral reset.
  */
#define __HAL_RCC_TIM8_FORCE_RESET()   (RCC->APB2RSTR |= (RCC_APB2RSTR_TIM8RST))
#define __HAL_RCC_TIM8_RELEASE_RESET() (RCC->APB2RSTR &= ~(RCC_APB2RSTR_TIM8RST))

/** @brief  Enable or disable the AHB1 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */   
#define __HAL_RCC_GPIOF_CLK_SLEEP_ENABLE()      (RCC->AHB1LPENR |= (RCC_AHB1LPENR_GPIOFLPEN))
#define __HAL_RCC_GPIOG_CLK_SLEEP_ENABLE()      (RCC->AHB1LPENR |= (RCC_AHB1LPENR_GPIOGLPEN))
#define __HAL_RCC_GPIOI_CLK_SLEEP_ENABLE()      (RCC->AHB1LPENR |= (RCC_AHB1LPENR_GPIOILPEN))
#define __HAL_RCC_SRAM2_CLK_SLEEP_ENABLE()      (RCC->AHB1LPENR |= (RCC_AHB1LPENR_SRAM2LPEN))
#define __HAL_RCC_ETHMAC_CLK_SLEEP_ENABLE()     (RCC->AHB1LPENR |= (RCC_AHB1LPENR_ETHMACLPEN))
#define __HAL_RCC_ETHMACTX_CLK_SLEEP_ENABLE()   (RCC->AHB1LPENR |= (RCC_AHB1LPENR_ETHMACTXLPEN))
#define __HAL_RCC_ETHMACRX_CLK_SLEEP_ENABLE()   (RCC->AHB1LPENR |= (RCC_AHB1LPENR_ETHMACRXLPEN))
#define __HAL_RCC_ETHMACPTP_CLK_SLEEP_ENABLE()  (RCC->AHB1LPENR |= (RCC_AHB1LPENR_ETHMACPTPLPEN))
#define __HAL_RCC_USB_OTG_HS_CLK_SLEEP_ENABLE()      (RCC->AHB1LPENR |= (RCC_AHB1LPENR_OTGHSLPEN))
#define __HAL_RCC_USB_OTG_HS_ULPI_CLK_SLEEP_ENABLE()  (RCC->AHB1LPENR |= (RCC_AHB1LPENR_OTGHSULPILPEN))

#define __HAL_RCC_GPIOF_CLK_SLEEP_DISABLE()     (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_GPIOFLPEN))
#define __HAL_RCC_GPIOG_CLK_SLEEP_DISABLE()     (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_GPIOGLPEN))
#define __HAL_RCC_GPIOI_CLK_SLEEP_DISABLE()     (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_GPIOILPEN))
#define __HAL_RCC_SRAM2_CLK_SLEEP_DISABLE()     (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_SRAM2LPEN))
#define __HAL_RCC_ETHMAC_CLK_SLEEP_DISABLE()    (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_ETHMACLPEN))
#define __HAL_RCC_ETHMACTX_CLK_SLEEP_DISABLE()  (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_ETHMACTXLPEN))
#define __HAL_RCC_ETHMACRX_CLK_SLEEP_DISABLE()  (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_ETHMACRXLPEN))
#define __HAL_RCC_ETHMACPTP_CLK_SLEEP_DISABLE() (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_ETHMACPTPLPEN))
#define __HAL_RCC_USB_OTG_HS_CLK_SLEEP_DISABLE()     (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_OTGHSLPEN))
#define __HAL_RCC_USB_OTG_HS_ULPI_CLK_SLEEP_DISABLE() (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_OTGHSULPILPEN))

/** @brief  Enable or disable the AHB2 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */
#if defined(STM32F407xx)|| defined(STM32F417xx) 
#define __HAL_RCC_DCMI_CLK_SLEEP_ENABLE()  (RCC->AHB2LPENR |= (RCC_AHB2LPENR_DCMILPEN))
#define __HAL_RCC_DCMI_CLK_SLEEP_DISABLE() (RCC->AHB2LPENR &= ~(RCC_AHB2LPENR_DCMILPEN))
#endif /* STM32F407xx || STM32F417xx */

#if defined(STM32F415xx) || defined(STM32F417xx) 
#define __HAL_RCC_CRYP_CLK_SLEEP_ENABLE()  (RCC->AHB2LPENR |= (RCC_AHB2LPENR_CRYPLPEN))
#define __HAL_RCC_HASH_CLK_SLEEP_ENABLE()  (RCC->AHB2LPENR |= (RCC_AHB2LPENR_HASHLPEN))

#define __HAL_RCC_CRYP_CLK_SLEEP_DISABLE() (RCC->AHB2LPENR &= ~(RCC_AHB2LPENR_CRYPLPEN))
#define __HAL_RCC_HASH_CLK_SLEEP_DISABLE() (RCC->AHB2LPENR &= ~(RCC_AHB2LPENR_HASHLPEN))
#endif /* STM32F415xx || STM32F417xx */

/** @brief  Enable or disable the AHB3 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */
#define __HAL_RCC_FSMC_CLK_SLEEP_ENABLE()  (RCC->AHB3LPENR |= (RCC_AHB3LPENR_FSMCLPEN))
#define __HAL_RCC_FSMC_CLK_SLEEP_DISABLE() (RCC->AHB3LPENR &= ~(RCC_AHB3LPENR_FSMCLPEN))

/** @brief  Enable or disable the APB1 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */
#define __HAL_RCC_TIM6_CLK_SLEEP_ENABLE()    (RCC->APB1LPENR |= (RCC_APB1LPENR_TIM6LPEN))
#define __HAL_RCC_TIM7_CLK_SLEEP_ENABLE()    (RCC->APB1LPENR |= (RCC_APB1LPENR_TIM7LPEN))
#define __HAL_RCC_TIM12_CLK_SLEEP_ENABLE()   (RCC->APB1LPENR |= (RCC_APB1LPENR_TIM12LPEN))
#define __HAL_RCC_TIM13_CLK_SLEEP_ENABLE()   (RCC->APB1LPENR |= (RCC_APB1LPENR_TIM13LPEN))
#define __HAL_RCC_TIM14_CLK_SLEEP_ENABLE()   (RCC->APB1LPENR |= (RCC_APB1LPENR_TIM14LPEN))
#define __HAL_RCC_USART3_CLK_SLEEP_ENABLE()  (RCC->APB1LPENR |= (RCC_APB1LPENR_USART3LPEN))
#define __HAL_RCC_UART4_CLK_SLEEP_ENABLE()   (RCC->APB1LPENR |= (RCC_APB1LPENR_UART4LPEN))
#define __HAL_RCC_UART5_CLK_SLEEP_ENABLE()   (RCC->APB1LPENR |= (RCC_APB1LPENR_UART5LPEN))
#define __HAL_RCC_CAN1_CLK_SLEEP_ENABLE()    (RCC->APB1LPENR |= (RCC_APB1LPENR_CAN1LPEN))
#define __HAL_RCC_CAN2_CLK_SLEEP_ENABLE()    (RCC->APB1LPENR |= (RCC_APB1LPENR_CAN2LPEN))
#define __HAL_RCC_DAC_CLK_SLEEP_ENABLE()     (RCC->APB1LPENR |= (RCC_APB1LPENR_DACLPEN))

#define __HAL_RCC_TIM6_CLK_SLEEP_DISABLE()   (RCC->APB1LPENR &= ~(RCC_APB1LPENR_TIM6LPEN))
#define __HAL_RCC_TIM7_CLK_SLEEP_DISABLE()   (RCC->APB1LPENR &= ~(RCC_APB1LPENR_TIM7LPEN))
#define __HAL_RCC_TIM12_CLK_SLEEP_DISABLE()  (RCC->APB1LPENR &= ~(RCC_APB1LPENR_TIM12LPEN))
#define __HAL_RCC_TIM13_CLK_SLEEP_DISABLE()  (RCC->APB1LPENR &= ~(RCC_APB1LPENR_TIM13LPEN))
#define __HAL_RCC_TIM14_CLK_SLEEP_DISABLE()  (RCC->APB1LPENR &= ~(RCC_APB1LPENR_TIM14LPEN))
#define __HAL_RCC_USART3_CLK_SLEEP_DISABLE() (RCC->APB1LPENR &= ~(RCC_APB1LPENR_USART3LPEN))
#define __HAL_RCC_UART4_CLK_SLEEP_DISABLE()  (RCC->APB1LPENR &= ~(RCC_APB1LPENR_UART4LPEN))
#define __HAL_RCC_UART5_CLK_SLEEP_DISABLE()  (RCC->APB1LPENR &= ~(RCC_APB1LPENR_UART5LPEN))
#define __HAL_RCC_CAN1_CLK_SLEEP_DISABLE()   (RCC->APB1LPENR &= ~(RCC_APB1LPENR_CAN1LPEN))
#define __HAL_RCC_CAN2_CLK_SLEEP_DISABLE()   (RCC->APB1LPENR &= ~(RCC_APB1LPENR_CAN2LPEN))
#define __HAL_RCC_DAC_CLK_SLEEP_DISABLE()    (RCC->APB1LPENR &= ~(RCC_APB1LPENR_DACLPEN))

/** @brief  Enable or disable the APB2 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */ 
#define __HAL_RCC_TIM8_CLK_SLEEP_ENABLE() (RCC->APB2LPENR |= (RCC_APB2LPENR_TIM8LPEN))
#define __HAL_RCC_ADC2_CLK_SLEEP_ENABLE() (RCC->APB2LPENR |= (RCC_APB2LPENR_ADC2LPEN))
#define __HAL_RCC_ADC3_CLK_SLEEP_ENABLE() (RCC->APB2LPENR |= (RCC_APB2LPENR_ADC3LPEN))

#define __HAL_RCC_TIM8_CLK_SLEEP_DISABLE() (RCC->APB2LPENR &= ~(RCC_APB2LPENR_TIM8LPEN))
#define __HAL_RCC_ADC2_CLK_SLEEP_DISABLE() (RCC->APB2LPENR &= ~(RCC_APB2LPENR_ADC2LPEN))
#define __HAL_RCC_ADC3_CLK_SLEEP_DISABLE() (RCC->APB2LPENR &= ~(RCC_APB2LPENR_ADC3LPEN))
#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx */
/*---------------------------------------------------------------------------------------------*/

/*------------------------------------------ STM32F411xx --------------------------------------*/
#if defined(STM32F411xE)
/** @brief  Enable or disable the High Speed APB (APB2) peripheral clock.
  */
#define __HAL_RCC_SPI5_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI5EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI5EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_SPI5_CLK_DISABLE() (RCC->APB2ENR &= ~(RCC_APB2ENR_SPI5EN))
                                       
/** @brief  Force or release APB2 peripheral reset.
  */
#define __HAL_RCC_SPI5_FORCE_RESET()   (RCC->APB2RSTR |= (RCC_APB2RSTR_SPI5RST))
#define __HAL_RCC_SPI5_RELEASE_RESET() (RCC->APB2RSTR &= ~(RCC_APB2RSTR_SPI5RST))                                    

/** @brief  Enable or disable the APB2 peripheral clock during Low Power (Sleep) mode.
  */ 
#define __HAL_RCC_SPI5_CLK_SLEEP_ENABLE() (RCC->APB2LPENR |= (RCC_APB2LPENR_SPI5LPEN))
#define __HAL_RCC_SPI5_CLK_SLEEP_DISABLE() (RCC->APB2LPENR &= ~(RCC_APB2LPENR_SPI5LPEN))

#endif /* STM32F411xE */
/*---------------------------------------------------------------------------------------------*/

/*----------------------------------------- STM32F446xx ---------------------------------------*/
#if defined(STM32F446xx)
/** @brief  Enables or disables the AHB1 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.
  */
#define __HAL_RCC_GPIOF_CLK_ENABLE()  do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOFEN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOFEN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_GPIOG_CLK_ENABLE()  do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOGEN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOGEN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_USB_OTG_HS_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_OTGHSEN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_OTGHSEN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_USB_OTG_HS_ULPI_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_OTGHSULPIEN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_OTGHSULPIEN);\
                                      UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCC_GPIOF_CLK_DISABLE()           (RCC->AHB1ENR &= ~(RCC_AHB1ENR_GPIOFEN))
#define __HAL_RCC_GPIOG_CLK_DISABLE()           (RCC->AHB1ENR &= ~(RCC_AHB1ENR_GPIOGEN))
#define __HAL_RCC_USB_OTG_HS_CLK_DISABLE()      (RCC->AHB1ENR &= ~(RCC_AHB1ENR_OTGHSEN))
#define __HAL_RCC_USB_OTG_HS_ULPI_CLK_DISABLE() (RCC->AHB1ENR &= ~(RCC_AHB1ENR_OTGHSULPIEN))
                                     
/** @brief  Enable or disable the AHB2 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.
  */
#define __HAL_RCC_DCMI_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_DCMIEN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_DCMIEN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_DCMI_CLK_DISABLE()  (RCC->AHB2ENR &= ~(RCC_AHB2ENR_DCMIEN))

/** @brief  Enables or disables the AHB3 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it. 
  */
#define __HAL_RCC_FMC_CLK_ENABLE()    do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->AHB3ENR, RCC_AHB3ENR_FMCEN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->AHB3ENR, RCC_AHB3ENR_FMCEN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_QSPI_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->AHB3ENR, RCC_AHB3ENR_QSPIEN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->AHB3ENR, RCC_AHB3ENR_QSPIEN);\
                                      UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCC_FMC_CLK_DISABLE()    (RCC->AHB3ENR &= ~(RCC_AHB3ENR_FMCEN))
#define __HAL_RCC_QSPI_CLK_DISABLE()   (RCC->AHB3ENR &= ~(RCC_AHB3ENR_QSPIEN))

/** @brief  Enable or disable the Low Speed APB (APB1) peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it. 
  */
#define __HAL_RCC_TIM6_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM6EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM6EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_TIM7_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM7EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM7EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_TIM12_CLK_ENABLE()  do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM12EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM12EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_TIM13_CLK_ENABLE()  do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM13EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM13EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_TIM14_CLK_ENABLE()  do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM14EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM14EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_SPDIFRX_CLK_ENABLE() do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_SPDIFRXEN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_SPDIFRXEN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_USART3_CLK_ENABLE() do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART3EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_USART3EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_UART4_CLK_ENABLE()  do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_UART4EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_UART4EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_UART5_CLK_ENABLE()  do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_UART5EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_UART5EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_FMPI2C1_CLK_ENABLE() do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_FMPI2C1EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_FMPI2C1EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_CAN1_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_CAN1EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_CAN1EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_CAN2_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_CAN2EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_CAN2EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_CEC_CLK_ENABLE()    do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_CECEN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_CECEN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_DAC_CLK_ENABLE()    do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB1ENR, RCC_APB1ENR_DACEN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_DACEN);\
                                      UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCC_TIM6_CLK_DISABLE()    (RCC->APB1ENR &= ~(RCC_APB1ENR_TIM6EN))
#define __HAL_RCC_TIM7_CLK_DISABLE()    (RCC->APB1ENR &= ~(RCC_APB1ENR_TIM7EN))
#define __HAL_RCC_TIM12_CLK_DISABLE()   (RCC->APB1ENR &= ~(RCC_APB1ENR_TIM12EN))
#define __HAL_RCC_TIM13_CLK_DISABLE()   (RCC->APB1ENR &= ~(RCC_APB1ENR_TIM13EN))
#define __HAL_RCC_TIM14_CLK_DISABLE()   (RCC->APB1ENR &= ~(RCC_APB1ENR_TIM14EN))
#define __HAL_RCC_SPDIFRX_CLK_DISABLE()   (RCC->APB1ENR &= ~(RCC_APB1ENR_SPDIFRXEN))
#define __HAL_RCC_USART3_CLK_DISABLE()  (RCC->APB1ENR &= ~(RCC_APB1ENR_USART3EN))
#define __HAL_RCC_UART4_CLK_DISABLE()   (RCC->APB1ENR &= ~(RCC_APB1ENR_UART4EN))
#define __HAL_RCC_UART5_CLK_DISABLE()   (RCC->APB1ENR &= ~(RCC_APB1ENR_UART5EN))
#define __HAL_RCC_FMPI2C1_CLK_DISABLE() (RCC->APB1ENR &= ~(RCC_APB1ENR_FMPI2C1EN))
#define __HAL_RCC_CAN1_CLK_DISABLE()    (RCC->APB1ENR &= ~(RCC_APB1ENR_CAN1EN))
#define __HAL_RCC_CAN2_CLK_DISABLE()    (RCC->APB1ENR &= ~(RCC_APB1ENR_CAN2EN))
#define __HAL_RCC_CEC_CLK_DISABLE()     (RCC->APB1ENR &= ~(RCC_APB1ENR_CECEN))
#define __HAL_RCC_DAC_CLK_DISABLE()     (RCC->APB1ENR &= ~(RCC_APB1ENR_DACEN))

/** @brief  Enable or disable the High Speed APB (APB2) peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.
  */
#define __HAL_RCC_TIM8_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM8EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM8EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_ADC2_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC2EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC2EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_ADC3_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC3EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC3EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_SAI1_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SAI1EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SAI1EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)
#define __HAL_RCC_SAI2_CLK_ENABLE()   do { \
                                      __IO uint32_t tmpreg; \
                                      SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SAI2EN);\
                                      /* Delay after an RCC peripheral clock enabling */ \
                                      tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SAI2EN);\
                                      UNUSED(tmpreg); \
                                      } while(0)

#define __HAL_RCC_TIM8_CLK_DISABLE() (RCC->APB2ENR &= ~(RCC_APB2ENR_TIM8EN))
#define __HAL_RCC_ADC2_CLK_DISABLE() (RCC->APB2ENR &= ~(RCC_APB2ENR_ADC2EN))
#define __HAL_RCC_ADC3_CLK_DISABLE() (RCC->APB2ENR &= ~(RCC_APB2ENR_ADC3EN))
#define __HAL_RCC_SAI1_CLK_DISABLE() (RCC->APB2ENR &= ~(RCC_APB2ENR_SAI1EN))
#define __HAL_RCC_SAI2_CLK_DISABLE() (RCC->APB2ENR &= ~(RCC_APB2ENR_SAI2EN))

/** @brief  Force or release AHB1 peripheral reset.
  */  
#define __HAL_RCC_GPIOF_FORCE_RESET()    (RCC->AHB1RSTR |= (RCC_AHB1RSTR_GPIOFRST))
#define __HAL_RCC_GPIOG_FORCE_RESET()    (RCC->AHB1RSTR |= (RCC_AHB1RSTR_GPIOGRST))
#define __HAL_RCC_USB_OTG_HS_FORCE_RESET()    (RCC->AHB1RSTR |= (RCC_AHB1RSTR_OTGHRST))

#define __HAL_RCC_GPIOF_RELEASE_RESET()  (RCC->AHB1RSTR &= ~(RCC_AHB1RSTR_GPIOFRST))
#define __HAL_RCC_GPIOG_RELEASE_RESET()  (RCC->AHB1RSTR &= ~(RCC_AHB1RSTR_GPIOGRST))
#define __HAL_RCC_USB_OTG_HS_RELEASE_RESET()  (RCC->AHB1RSTR &= ~(RCC_AHB1RSTR_OTGHRST))
 
/** @brief  Force or release AHB2 peripheral reset.
  */
#define __HAL_RCC_DCMI_FORCE_RESET()   (RCC->AHB2RSTR |= (RCC_AHB2RSTR_DCMIRST))
#define __HAL_RCC_DCMI_RELEASE_RESET() (RCC->AHB2RSTR &= ~(RCC_AHB2RSTR_DCMIRST))

/** @brief  Force or release AHB3 peripheral reset
  */ 
#define __HAL_RCC_FMC_FORCE_RESET()    (RCC->AHB3RSTR |= (RCC_AHB3RSTR_FMCRST))
#define __HAL_RCC_QSPI_FORCE_RESET()   (RCC->AHB3RSTR |= (RCC_AHB3RSTR_QSPIRST))

#define __HAL_RCC_FMC_RELEASE_RESET()    (RCC->AHB3RSTR &= ~(RCC_AHB3RSTR_FMCRST))
#define __HAL_RCC_QSPI_RELEASE_RESET()   (RCC->AHB3RSTR &= ~(RCC_AHB3RSTR_QSPIRST))
 
/** @brief  Force or release APB1 peripheral reset.
  */ 
#define __HAL_RCC_TIM6_FORCE_RESET()     (RCC->APB1RSTR |= (RCC_APB1RSTR_TIM6RST))
#define __HAL_RCC_TIM7_FORCE_RESET()     (RCC->APB1RSTR |= (RCC_APB1RSTR_TIM7RST))
#define __HAL_RCC_TIM12_FORCE_RESET()    (RCC->APB1RSTR |= (RCC_APB1RSTR_TIM12RST))
#define __HAL_RCC_TIM13_FORCE_RESET()    (RCC->APB1RSTR |= (RCC_APB1RSTR_TIM13RST))
#define __HAL_RCC_TIM14_FORCE_RESET()    (RCC->APB1RSTR |= (RCC_APB1RSTR_TIM14RST))
#define __HAL_RCC_SPDIFRX_FORCE_RESET()    (RCC->APB1RSTR |= (RCC_APB1RSTR_SPDIFRXRST))
#define __HAL_RCC_USART3_FORCE_RESET()   (RCC->APB1RSTR |= (RCC_APB1RSTR_USART3RST))
#define __HAL_RCC_UART4_FORCE_RESET()    (RCC->APB1RSTR |= (RCC_APB1RSTR_UART4RST))
#define __HAL_RCC_UART5_FORCE_RESET()    (RCC->APB1RSTR |= (RCC_APB1RSTR_UART5RST))
#define __HAL_RCC_FMPI2C1_FORCE_RESET()  (RCC->APB1RSTR |= (RCC_APB1RSTR_FMPI2C1RST))
#define __HAL_RCC_CAN1_FORCE_RESET()     (RCC->APB1RSTR |= (RCC_APB1RSTR_CAN1RST))
#define __HAL_RCC_CAN2_FORCE_RESET()     (RCC->APB1RSTR |= (RCC_APB1RSTR_CAN2RST))
#define __HAL_RCC_CEC_FORCE_RESET()      (RCC->APB1RSTR |= (RCC_APB1RSTR_CECRST))
#define __HAL_RCC_DAC_FORCE_RESET()      (RCC->APB1RSTR |= (RCC_APB1RSTR_DACRST))

#define __HAL_RCC_TIM6_RELEASE_RESET()   (RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM6RST))
#define __HAL_RCC_TIM7_RELEASE_RESET()   (RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM7RST))
#define __HAL_RCC_TIM12_RELEASE_RESET()  (RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM12RST))
#define __HAL_RCC_TIM13_RELEASE_RESET()  (RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM13RST))
#define __HAL_RCC_TIM14_RELEASE_RESET()  (RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM14RST))
#define __HAL_RCC_SPDIFRX_RELEASE_RESET()  (RCC->APB1RSTR &= ~(RCC_APB1RSTR_SPDIFRXRST))
#define __HAL_RCC_USART3_RELEASE_RESET() (RCC->APB1RSTR &= ~(RCC_APB1RSTR_USART3RST))
#define __HAL_RCC_UART4_RELEASE_RESET()  (RCC->APB1RSTR &= ~(RCC_APB1RSTR_UART4RST))
#define __HAL_RCC_UART5_RELEASE_RESET()  (RCC->APB1RSTR &= ~(RCC_APB1RSTR_UART5RST))
#define __HAL_RCC_FMPI2C1_RELEASE_RESET() (RCC->APB1RSTR &= ~(RCC_APB1RSTR_FMPI2C1RST))
#define __HAL_RCC_CAN1_RELEASE_RESET()   (RCC->APB1RSTR &= ~(RCC_APB1RSTR_CAN1RST))
#define __HAL_RCC_CAN2_RELEASE_RESET()   (RCC->APB1RSTR &= ~(RCC_APB1RSTR_CAN2RST))
#define __HAL_RCC_CEC_RELEASE_RESET()    (RCC->APB1RSTR &= ~(RCC_APB1RSTR_CECRST))
#define __HAL_RCC_DAC_RELEASE_RESET()    (RCC->APB1RSTR &= ~(RCC_APB1RSTR_DACRST))

/** @brief  Force or release APB2 peripheral reset.
  */
#define __HAL_RCC_TIM8_FORCE_RESET()   (RCC->APB2RSTR |= (RCC_APB2RSTR_TIM8RST))
#define __HAL_RCC_SAI1_FORCE_RESET()   (RCC->APB2RSTR |= (RCC_APB2RSTR_SAI1RST)) 
#define __HAL_RCC_SAI2_FORCE_RESET()   (RCC->APB2RSTR |= (RCC_APB2RSTR_SAI2RST))

#define __HAL_RCC_TIM8_RELEASE_RESET() (RCC->APB2RSTR &= ~(RCC_APB2RSTR_TIM8RST))
#define __HAL_RCC_SAI1_RELEASE_RESET() (RCC->APB2RSTR &= ~(RCC_APB2RSTR_SAI1RST))
#define __HAL_RCC_SAI2_RELEASE_RESET() (RCC->APB2RSTR &= ~(RCC_APB2RSTR_SAI2RST)) 

/** @brief  Enable or disable the AHB1 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */ 
#define __HAL_RCC_GPIOF_CLK_SLEEP_ENABLE()      (RCC->AHB1LPENR |= (RCC_AHB1LPENR_GPIOFLPEN))
#define __HAL_RCC_GPIOG_CLK_SLEEP_ENABLE()      (RCC->AHB1LPENR |= (RCC_AHB1LPENR_GPIOGLPEN))
#define __HAL_RCC_SRAM2_CLK_SLEEP_ENABLE()      (RCC->AHB1LPENR |= (RCC_AHB1LPENR_SRAM2LPEN))
#define __HAL_RCC_USB_OTG_HS_CLK_SLEEP_ENABLE()      (RCC->AHB1LPENR |= (RCC_AHB1LPENR_OTGHSLPEN))
#define __HAL_RCC_USB_OTG_HS_ULPI_CLK_SLEEP_ENABLE()  (RCC->AHB1LPENR |= (RCC_AHB1LPENR_OTGHSULPILPEN))

#define __HAL_RCC_GPIOF_CLK_SLEEP_DISABLE()     (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_GPIOFLPEN))
#define __HAL_RCC_GPIOG_CLK_SLEEP_DISABLE()     (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_GPIOGLPEN))
#define __HAL_RCC_SRAM2_CLK_SLEEP_DISABLE()     (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_SRAM2LPEN))
#define __HAL_RCC_USB_OTG_HS_CLK_SLEEP_DISABLE()     (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_OTGHSLPEN))
#define __HAL_RCC_USB_OTG_HS_ULPI_CLK_SLEEP_DISABLE() (RCC->AHB1LPENR &= ~(RCC_AHB1LPENR_OTGHSULPILPEN))
/** @brief  Enable or disable the AHB2 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */
#define __HAL_RCC_DCMI_CLK_SLEEP_ENABLE()  (RCC->AHB2LPENR |= (RCC_AHB2LPENR_DCMILPEN))
#define __HAL_RCC_DCMI_CLK_SLEEP_DISABLE() (RCC->AHB2LPENR &= ~(RCC_AHB2LPENR_DCMILPEN))

/** @brief  Enable or disable the AHB3 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */
#define __HAL_RCC_FMC_CLK_SLEEP_ENABLE()   (RCC->AHB3LPENR |= (RCC_AHB3LPENR_FMCLPEN))
#define __HAL_RCC_QSPI_CLK_SLEEP_ENABLE()  (RCC->AHB3LPENR |= (RCC_AHB3LPENR_QSPILPEN))

#define __HAL_RCC_FMC_CLK_SLEEP_DISABLE()   (RCC->AHB3LPENR &= ~(RCC_AHB3LPENR_FMCLPEN))
#define __HAL_RCC_QSPI_CLK_SLEEP_DISABLE()  (RCC->AHB3LPENR &= ~(RCC_AHB3LPENR_QSPILPEN))

/** @brief  Enable or disable the APB1 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */  
#define __HAL_RCC_TIM6_CLK_SLEEP_ENABLE()    (RCC->APB1LPENR |= (RCC_APB1LPENR_TIM6LPEN))
#define __HAL_RCC_TIM7_CLK_SLEEP_ENABLE()    (RCC->APB1LPENR |= (RCC_APB1LPENR_TIM7LPEN))
#define __HAL_RCC_TIM12_CLK_SLEEP_ENABLE()   (RCC->APB1LPENR |= (RCC_APB1LPENR_TIM12LPEN))
#define __HAL_RCC_TIM13_CLK_SLEEP_ENABLE()   (RCC->APB1LPENR |= (RCC_APB1LPENR_TIM13LPEN))
#define __HAL_RCC_TIM14_CLK_SLEEP_ENABLE()   (RCC->APB1LPENR |= (RCC_APB1LPENR_TIM14LPEN))
#define __HAL_RCC_SPDIFRX_CLK_SLEEP_ENABLE()   (RCC->APB1LPENR |= (RCC_APB1LPENR_SPDIFRXLPEN))
#define __HAL_RCC_USART3_CLK_SLEEP_ENABLE()  (RCC->APB1LPENR |= (RCC_APB1LPENR_USART3LPEN))
#define __HAL_RCC_UART4_CLK_SLEEP_ENABLE()   (RCC->APB1LPENR |= (RCC_APB1LPENR_UART4LPEN))
#define __HAL_RCC_UART5_CLK_SLEEP_ENABLE()   (RCC->APB1LPENR |= (RCC_APB1LPENR_UART5LPEN))
#define __HAL_RCC_FMPI2C1_CLK_SLEEP_ENABLE() (RCC->APB1LPENR |= (RCC_APB1LPENR_FMPI2C1LPEN))
#define __HAL_RCC_CAN1_CLK_SLEEP_ENABLE()    (RCC->APB1LPENR |= (RCC_APB1LPENR_CAN1LPEN))
#define __HAL_RCC_CAN2_CLK_SLEEP_ENABLE()    (RCC->APB1LPENR |= (RCC_APB1LPENR_CAN2LPEN))
#define __HAL_RCC_CEC_CLK_SLEEP_ENABLE()     (RCC->APB1LPENR |= (RCC_APB1LPENR_CECLPEN))
#define __HAL_RCC_DAC_CLK_SLEEP_ENABLE()     (RCC->APB1LPENR |= (RCC_APB1LPENR_DACLPEN))

#define __HAL_RCC_TIM6_CLK_SLEEP_DISABLE()   (RCC->APB1LPENR &= ~(RCC_APB1LPENR_TIM6LPEN))
#define __HAL_RCC_TIM7_CLK_SLEEP_DISABLE()   (RCC->APB1LPENR &= ~(RCC_APB1LPENR_TIM7LPEN))
#define __HAL_RCC_TIM12_CLK_SLEEP_DISABLE()  (RCC->APB1LPENR &= ~(RCC_APB1LPENR_TIM12LPEN))
#define __HAL_RCC_TIM13_CLK_SLEEP_DISABLE()  (RCC->APB1LPENR &= ~(RCC_APB1LPENR_TIM13LPEN))
#define __HAL_RCC_TIM14_CLK_SLEEP_DISABLE()  (RCC->APB1LPENR &= ~(RCC_APB1LPENR_TIM14LPEN))
#define __HAL_RCC_SPDIFRX_CLK_SLEEP_DISABLE()  (RCC->APB1LPENR &= ~(RCC_APB1LPENR_SPDIFRXLPEN))
#define __HAL_RCC_USART3_CLK_SLEEP_DISABLE() (RCC->APB1LPENR &= ~(RCC_APB1LPENR_USART3LPEN))
#define __HAL_RCC_UART4_CLK_SLEEP_DISABLE()  (RCC->APB1LPENR &= ~(RCC_APB1LPENR_UART4LPEN))
#define __HAL_RCC_UART5_CLK_SLEEP_DISABLE()  (RCC->APB1LPENR &= ~(RCC_APB1LPENR_UART5LPEN))
#define __HAL_RCC_FMPI2C1_CLK_SLEEP_DISABLE() (RCC->APB1LPENR &= ~(RCC_APB1LPENR_FMPI2C1LPEN))
#define __HAL_RCC_CAN1_CLK_SLEEP_DISABLE()   (RCC->APB1LPENR &= ~(RCC_APB1LPENR_CAN1LPEN))
#define __HAL_RCC_CAN2_CLK_SLEEP_DISABLE()   (RCC->APB1LPENR &= ~(RCC_APB1LPENR_CAN2LPEN))
#define __HAL_RCC_CEC_CLK_SLEEP_DISABLE()    (RCC->APB1LPENR &= ~(RCC_APB1LPENR_CECLPEN))
#define __HAL_RCC_DAC_CLK_SLEEP_DISABLE()    (RCC->APB1LPENR &= ~(RCC_APB1LPENR_DACLPEN))

/** @brief  Enable or disable the APB2 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */ 
#define __HAL_RCC_TIM8_CLK_SLEEP_ENABLE() (RCC->APB2LPENR |= (RCC_APB2LPENR_TIM8LPEN))
#define __HAL_RCC_ADC2_CLK_SLEEP_ENABLE() (RCC->APB2LPENR |= (RCC_APB2LPENR_ADC2LPEN))
#define __HAL_RCC_ADC3_CLK_SLEEP_ENABLE() (RCC->APB2LPENR |= (RCC_APB2LPENR_ADC3LPEN))
#define __HAL_RCC_SAI1_CLK_SLEEP_ENABLE() (RCC->APB2LPENR |= (RCC_APB2LPENR_SAI1LPEN))
#define __HAL_RCC_SAI2_CLK_SLEEP_ENABLE() (RCC->APB2LPENR |= (RCC_APB2LPENR_SAI2LPEN))

#define __HAL_RCC_TIM8_CLK_SLEEP_DISABLE() (RCC->APB2LPENR &= ~(RCC_APB2LPENR_TIM8LPEN))
#define __HAL_RCC_ADC2_CLK_SLEEP_DISABLE() (RCC->APB2LPENR &= ~(RCC_APB2LPENR_ADC2LPEN))
#define __HAL_RCC_ADC3_CLK_SLEEP_DISABLE() (RCC->APB2LPENR &= ~(RCC_APB2LPENR_ADC3LPEN))
#define __HAL_RCC_SAI1_CLK_SLEEP_DISABLE() (RCC->APB2LPENR &= ~(RCC_APB2LPENR_SAI1LPEN))
#define __HAL_RCC_SAI2_CLK_SLEEP_DISABLE() (RCC->APB2LPENR &= ~(RCC_APB2LPENR_SAI2LPEN))

#endif /* STM32F446xx */
/*------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------- PLL Configuration ----------------------------------------*/
#if defined(STM32F446xx)
/** @brief  Macro to configure the main PLL clock source, multiplication and division factors.
  * @note   This function must be used only when the main PLL is disabled.
  * @param  __RCC_PLLSource__: specifies the PLL entry clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_PLLSOURCE_HSI: HSI oscillator clock selected as PLL clock entry
  *            @arg RCC_PLLSOURCE_HSE: HSE oscillator clock selected as PLL clock entry
  * @note   This clock source (RCC_PLLSource) is common for the main PLL and PLLI2S.  
  * @param  __PLLM__: specifies the division factor for PLL VCO input clock
  *         This parameter must be a number between Min_Data = 2 and Max_Data = 63.
  * @note   You have to set the PLLM parameter correctly to ensure that the VCO input
  *         frequency ranges from 1 to 2 MHz. It is recommended to select a frequency
  *         of 2 MHz to limit PLL jitter.
  * @param  __PLLN__: specifies the multiplication factor for PLL VCO output clock
  *         This parameter must be a number between Min_Data = 192 and Max_Data = 432.
  * @note   You have to set the PLLN parameter correctly to ensure that the VCO
  *         output frequency is between 192 and 432 MHz.
  *   
  * @param  __PLLP__: specifies the division factor for main system clock (SYSCLK)
  *         This parameter must be a number in the range {2, 4, 6, or 8}.
  *           
  * @param  __PLLQ__: specifies the division factor for OTG FS, SDIO and RNG clocks
  *         This parameter must be a number between Min_Data = 2 and Max_Data = 15.        
  * @note   If the USB OTG FS is used in your application, you have to set the
  *         PLLQ parameter correctly to have 48 MHz clock for the USB. However,
  *         the SDIO and RNG need a frequency lower than or equal to 48 MHz to work
  *         correctly.
  *     
  * @param  __PLLR__: PLL division factor for I2S, SAI, SYSTEM, SPDIFRX clocks.
  *         This parameter must be a number between Min_Data = 2 and Max_Data = 7.
  * @note   This parameter is only available in STM32F446xx devices.
  *      
  */
#define __HAL_RCC_PLL_CONFIG(__RCC_PLLSource__, __PLLM__, __PLLN__, __PLLP__, __PLLQ__,__PLLR__)  \
                            (RCC->PLLCFGR = ((__RCC_PLLSource__) | (__PLLM__)                   | \
                            ((__PLLN__) << POSITION_VAL(RCC_PLLCFGR_PLLN))                      | \
                            ((((__PLLP__) >> 1) -1) << POSITION_VAL(RCC_PLLCFGR_PLLP))          | \
                            ((__PLLQ__) << POSITION_VAL(RCC_PLLCFGR_PLLQ))                      | \
                            ((__PLLR__) << POSITION_VAL(RCC_PLLCFGR_PLLR))))
#else
/** @brief  Macro to configure the main PLL clock source, multiplication and division factors.
  * @note   This function must be used only when the main PLL is disabled.
  * @param  __RCC_PLLSource__: specifies the PLL entry clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_PLLSOURCE_HSI: HSI oscillator clock selected as PLL clock entry
  *            @arg RCC_PLLSOURCE_HSE: HSE oscillator clock selected as PLL clock entry
  * @note   This clock source (RCC_PLLSource) is common for the main PLL and PLLI2S.  
  * @param  __PLLM__: specifies the division factor for PLL VCO input clock
  *         This parameter must be a number between Min_Data = 2 and Max_Data = 63.
  * @note   You have to set the PLLM parameter correctly to ensure that the VCO input
  *         frequency ranges from 1 to 2 MHz. It is recommended to select a frequency
  *         of 2 MHz to limit PLL jitter.
  * @param  __PLLN__: specifies the multiplication factor for PLL VCO output clock
  *         This parameter must be a number between Min_Data = 192 and Max_Data = 432.
  * @note   You have to set the PLLN parameter correctly to ensure that the VCO
  *         output frequency is between 192 and 432 MHz.
  *   
  * @param  __PLLP__: specifies the division factor for main system clock (SYSCLK)
  *         This parameter must be a number in the range {2, 4, 6, or 8}.
  *           
  * @param  __PLLQ__: specifies the division factor for OTG FS, SDIO and RNG clocks
  *         This parameter must be a number between Min_Data = 2 and Max_Data = 15.        
  * @note   If the USB OTG FS is used in your application, you have to set the
  *         PLLQ parameter correctly to have 48 MHz clock for the USB. However,
  *         the SDIO and RNG need a frequency lower than or equal to 48 MHz to work
  *         correctly.
  *      
  */
#define __HAL_RCC_PLL_CONFIG(__RCC_PLLSource__, __PLLM__, __PLLN__, __PLLP__, __PLLQ__)     \
                            (RCC->PLLCFGR = (0x20000000 | (__RCC_PLLSource__) | (__PLLM__)| \
                            ((__PLLN__) << POSITION_VAL(RCC_PLLCFGR_PLLN))                | \
                            ((((__PLLP__) >> 1) -1) << POSITION_VAL(RCC_PLLCFGR_PLLP))    | \
                            ((__PLLQ__) << POSITION_VAL(RCC_PLLCFGR_PLLQ))))
 #endif /* STM32F446xx */
/*-------------------------------------------------------------------------------------------------------*/

/*------------------------------------------- PLLI2S Configuration --------------------------------------*/
#if defined(STM32F446xx)
/** @brief  Macro to configure the PLLI2S clock multiplication and division factors .
  * @note   This macro must be used only when the PLLI2S is disabled.
  * @note   PLLI2S clock source is common with the main PLL (configured in 
  *         HAL_RCC_ClockConfig() API).
  * @param  __PLLI2SM__: specifies the division factor for PLLI2S VCO input clock
  *         This parameter must be a number between Min_Data = 2 and Max_Data = 63.
  * @note   You have to set the PLLI2SM parameter correctly to ensure that the VCO input
  *         frequency ranges from 1 to 2 MHz. It is recommended to select a frequency
  *         of 1 MHz to limit PLLI2S jitter.
  * @note   The PLLI2SM parameter is only used with STM32F411xE and STM32F446xx Devices
  *             
  * @param  __PLLI2SN__: specifies the multiplication factor for PLLI2S VCO output clock
  *         This parameter must be a number between Min_Data = 192 and Max_Data = 432.
  * @note   You have to set the PLLI2SN parameter correctly to ensure that the VCO 
  *         output frequency is between Min_Data = 192 and Max_Data = 432 MHz.
  *
  * @param  __PLLI2SP__: specifies division factor for SPDIFRX Clock.
  *         This parameter must be a number in the range {2, 4, 6, or 8}.
  * @note   the PLLI2SP parameter is only available with STM32F446xx Devices
  *                 
  * @param  __PLLI2SR__: specifies the division factor for I2S clock
  *         This parameter must be a number between Min_Data = 2 and Max_Data = 7.
  * @note   You have to set the PLLI2SR parameter correctly to not exceed 192 MHz
  *         on the I2S clock frequency.
  *   
  * @param  __PLLI2SQ__: specifies the division factor for SAI clock
  *         This parameter must be a number between Min_Data = 2 and Max_Data = 15.
  * @note   the PLLI2SQ parameter is only available with STM32F427/437/429x/439xx Devices
  *      
  */
#define __HAL_RCC_PLLI2S_CONFIG(__PLLI2SM__, __PLLI2SN__, __PLLI2SP__, __PLLI2SQ__, __PLLI2SR__) \
                               (RCC->PLLI2SCFGR = ((__PLLI2SM__)                                                       |\
                                                   ((__PLLI2SN__) << POSITION_VAL(RCC_PLLI2SCFGR_PLLI2SN))             |\
                                                   ((((__PLLI2SP__) >> 1) -1) << POSITION_VAL(RCC_PLLI2SCFGR_PLLI2SP)) |\
                                                   ((__PLLI2SQ__) << POSITION_VAL(RCC_PLLI2SCFGR_PLLI2SQ))             |\
                                                   ((__PLLI2SR__) << POSITION_VAL(RCC_PLLI2SCFGR_PLLI2SR))))
#else
/** @brief  Macro to configure the PLLI2S clock multiplication and division factors .
  * @note   This macro must be used only when the PLLI2S is disabled.
  * @note   PLLI2S clock source is common with the main PLL (configured in 
  *         HAL_RCC_ClockConfig() API).
  * @param  __PLLI2SN__: specifies the multiplication factor for PLLI2S VCO output clock
  *         This parameter must be a number between Min_Data = 192 and Max_Data = 432.
  * @note   You have to set the PLLI2SN parameter correctly to ensure that the VCO 
  *         output frequency is between Min_Data = 192 and Max_Data = 432 MHz.
  * @param  __PLLI2SR__: specifies the division factor for I2S clock
  *         This parameter must be a number between Min_Data = 2 and Max_Data = 7.
  * @note   You have to set the PLLI2SR parameter correctly to not exceed 192 MHz
  *         on the I2S clock frequency.
  *      
  */
#define __HAL_RCC_PLLI2S_CONFIG(__PLLI2SN__, __PLLI2SR__)                                                     \
                               (RCC->PLLI2SCFGR = (((__PLLI2SN__) << POSITION_VAL(RCC_PLLI2SCFGR_PLLI2SN))  | \
                               ((__PLLI2SR__) << POSITION_VAL(RCC_PLLI2SCFGR_PLLI2SR))))
#endif /* STM32F446xx */

#if defined(STM32F411xE)
                           
/** @brief  Macro to configure the PLLI2S clock multiplication and division factors .
  * @note   This macro must be used only when the PLLI2S is disabled.
  * @note   This macro must be used only when the PLLI2S is disabled.
  * @note   PLLI2S clock source is common with the main PLL (configured in 
  *         HAL_RCC_ClockConfig() API).
  * @param  __PLLI2SM__: specifies the division factor for PLLI2S VCO input clock
  *         This parameter must be a number between Min_Data = 2 and Max_Data = 63.
  * @note   The PLLI2SM parameter is only used with STM32F411xE Devices
  * @note   You have to set the PLLI2SM parameter correctly to ensure that the VCO input
  *         frequency ranges from 1 to 2 MHz. It is recommended to select a frequency
  *         of 2 MHz to limit PLLI2S jitter.    
  * @param  __PLLI2SN__: specifies the multiplication factor for PLLI2S VCO output clock
  *         This parameter must be a number between Min_Data = 192 and Max_Data = 432.
  * @note   You have to set the PLLI2SN parameter correctly to ensure that the VCO 
  *         output frequency is between Min_Data = 192 and Max_Data = 432 MHz.
  * @param  __PLLI2SR__: specifies the division factor for I2S clock
  *         This parameter must be a number between Min_Data = 2 and Max_Data = 7.
  * @note   You have to set the PLLI2SR parameter correctly to not exceed 192 MHz
  *         on the I2S clock frequency.
  */
#define __HAL_RCC_PLLI2S_I2SCLK_CONFIG(__PLLI2SM__, __PLLI2SN__, __PLLI2SR__) (RCC->PLLI2SCFGR = ((__PLLI2SM__)                                                       |\
                                                                                                  ((__PLLI2SN__) << POSITION_VAL(RCC_PLLI2SCFGR_PLLI2SN))             |\
                                                                                                  ((__PLLI2SR__) << POSITION_VAL(RCC_PLLI2SCFGR_PLLI2SR))))
#endif /* STM32F411xE */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)
/** @brief  Macro used by the SAI HAL driver to configure the PLLI2S clock multiplication and division factors.
  * @note   This macro must be used only when the PLLI2S is disabled.
  * @note   PLLI2S clock source is common with the main PLL (configured in 
  *         HAL_RCC_ClockConfig() API)             
  * @param  __PLLI2SN__: specifies the multiplication factor for PLLI2S VCO output clock.
  *         This parameter must be a number between Min_Data = 192 and Max_Data = 432.
  * @note   You have to set the PLLI2SN parameter correctly to ensure that the VCO 
  *         output frequency is between Min_Data = 192 and Max_Data = 432 MHz.
  * @param  __PLLI2SQ__: specifies the division factor for SAI1 clock.
  *         This parameter must be a number between Min_Data = 2 and Max_Data = 15. 
  * @note   the PLLI2SQ parameter is only available with STM32F427xx/437xx/429xx/439xx Devices
  *         and can be configured using the __HAL_RCC_PLLI2S_PLLSAICLK_CONFIG() macro
  * @param  __PLLI2SR__: specifies the division factor for I2S clock
  *         This parameter must be a number between Min_Data = 2 and Max_Data = 7.
  * @note   You have to set the PLLI2SR parameter correctly to not exceed 192 MHz
  *         on the I2S clock frequency.
  */
#define __HAL_RCC_PLLI2S_SAICLK_CONFIG(__PLLI2SN__, __PLLI2SQ__, __PLLI2SR__) (RCC->PLLI2SCFGR = ((__PLLI2SN__) << 6)  |\
                                                                                                 ((__PLLI2SQ__) << 24) |\
                                                                                                 ((__PLLI2SR__) << 28))
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx */   
/*----------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------- PLLSAI Configuration ---------------------------------------*/     
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || defined(STM32F446xx)
/** @brief Macros to Enable or Disable the PLLISAI. 
  * @note  The PLLSAI is only available with STM32F429x/439x Devices.     
  * @note  The PLLSAI is disabled by hardware when entering STOP and STANDBY modes. 
  */
#define __HAL_RCC_PLLSAI_ENABLE() (*(__IO uint32_t *) RCC_CR_PLLSAION_BB = ENABLE)
#define __HAL_RCC_PLLSAI_DISABLE() (*(__IO uint32_t *) RCC_CR_PLLSAION_BB = DISABLE)

#if defined(STM32F446xx)
/** @brief  Macro to configure the PLLSAI clock multiplication and division factors.
  *
  * @param  __PLLSAIM__: specifies the division factor for PLLSAI VCO input clock
  *         This parameter must be a number between Min_Data = 2 and Max_Data = 63.
  * @note   You have to set the PLLSAIM parameter correctly to ensure that the VCO input
  *         frequency ranges from 1 to 2 MHz. It is recommended to select a frequency
  *         of 1 MHz to limit PLLI2S jitter.
  * @note   The PLLSAIM parameter is only used with STM32F446xx Devices
  *             
  * @param  __PLLSAIN__: specifies the multiplication factor for PLLSAI VCO output clock.
  *         This parameter must be a number between Min_Data = 192 and Max_Data = 432.
  * @note   You have to set the PLLSAIN parameter correctly to ensure that the VCO 
  *         output frequency is between Min_Data = 192 and Max_Data = 432 MHz.
  *
  * @param  __PLLSAIP__: specifies division factor for OTG FS, SDIO and RNG clocks.
  *         This parameter must be a number in the range {2, 4, 6, or 8}.
  * @note   the PLLSAIP parameter is only available with STM32F446xx Devices
  *                 
  * @param  __PLLSAIQ__: specifies the division factor for SAI clock
  *         This parameter must be a number between Min_Data = 2 and Max_Data = 15.
  *           
  * @param  __PLLSAIR__: specifies the division factor for LTDC clock
  *         This parameter must be a number between Min_Data = 2 and Max_Data = 7.
  * @note   the PLLI2SR parameter is only available with STM32F427/437/429/439xx Devices  
  */
#define __HAL_RCC_PLLSAI_CONFIG(__PLLSAIM__, __PLLSAIN__, __PLLSAIP__, __PLLSAIQ__, __PLLSAIR__)     \
                               (RCC->PLLSAICFGR = ((__PLLSAIM__)                                   | \
                               ((__PLLSAIN__) << POSITION_VAL(RCC_PLLSAICFGR_PLLSAIN))             | \
                               ((((__PLLSAIP__) >> 1) -1) << POSITION_VAL(RCC_PLLSAICFGR_PLLSAIP)) | \
                               ((__PLLSAIQ__) << POSITION_VAL(RCC_PLLSAICFGR_PLLSAIQ)))) 
#endif /* STM32F446xx */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)
/** @brief  Macro to configure the PLLSAI clock multiplication and division factors.
  *             
  * @param  __PLLSAIN__: specifies the multiplication factor for PLLSAI VCO output clock.
  *         This parameter must be a number between Min_Data = 192 and Max_Data = 432.
  * @note   You have to set the PLLSAIN parameter correctly to ensure that the VCO 
  *         output frequency is between Min_Data = 192 and Max_Data = 432 MHz.
  *
  * @param  __PLLSAIQ__: specifies the division factor for SAI clock
  *         This parameter must be a number between Min_Data = 2 and Max_Data = 15.
  *           
  * @param  __PLLSAIR__: specifies the division factor for LTDC clock
  *         This parameter must be a number between Min_Data = 2 and Max_Data = 7.
  * @note   the PLLI2SR parameter is only available with STM32F427/437/429/439xx Devices  
  */
#define __HAL_RCC_PLLSAI_CONFIG(__PLLSAIN__, __PLLSAIQ__, __PLLSAIR__)                                        \
                               (RCC->PLLSAICFGR = (((__PLLSAIN__) << POSITION_VAL(RCC_PLLSAICFGR_PLLSAIN))  | \
                               ((__PLLSAIQ__) << POSITION_VAL(RCC_PLLSAICFGR_PLLSAIQ))                      | \
                               ((__PLLSAIR__) << POSITION_VAL(RCC_PLLSAICFGR_PLLSAIR))))
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx */

#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F446xx */
/*----------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------- PLLSAI/PLLI2S Dividers Configuration ---------------------------------------*/
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)  || defined(STM32F446xx)
/** @brief  Macro to configure the SAI clock Divider coming from PLLI2S.    
  * @note   This function must be called before enabling the PLLI2S.          
  * @param  __PLLI2SDivQ__: specifies the PLLI2S division factor for SAI1 clock .
  *          This parameter must be a number between 1 and 32.
  *          SAI1 clock frequency = f(PLLI2SQ) / __PLLI2SDivQ__ 
  */
#define __HAL_RCC_PLLI2S_PLLSAICLKDIVQ_CONFIG(__PLLI2SDivQ__) (MODIFY_REG(RCC->DCKCFGR, RCC_DCKCFGR_PLLI2SDIVQ, (__PLLI2SDivQ__)-1))

/** @brief  Macro to configure the SAI clock Divider coming from PLLSAI.
  * @note   This function must be called before enabling the PLLSAI.
  * @param  __PLLSAIDivQ__: specifies the PLLSAI division factor for SAI1 clock .
  *         This parameter must be a number between Min_Data = 1 and Max_Data = 32.
  *         SAI1 clock frequency = f(PLLSAIQ) / __PLLSAIDivQ__  
  */
#define __HAL_RCC_PLLSAI_PLLSAICLKDIVQ_CONFIG(__PLLSAIDivQ__) (MODIFY_REG(RCC->DCKCFGR, RCC_DCKCFGR_PLLSAIDIVQ, ((__PLLSAIDivQ__)-1)<<8))
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F446xx */
                                 
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)
/** @brief  Macro to configure the LTDC clock Divider coming from PLLSAI.
  * 
  * @note   The LTDC peripheral is only available with STM32F427/437/429/439xx Devices.   
  * @note   This function must be called before enabling the PLLSAI. 
  * @param  __PLLSAIDivR__: specifies the PLLSAI division factor for LTDC clock .
  *          This parameter must be a number between Min_Data = 2 and Max_Data = 16.
  *          LTDC clock frequency = f(PLLSAIR) / __PLLSAIDivR__ 
  */   
#define __HAL_RCC_PLLSAI_PLLSAICLKDIVR_CONFIG(__PLLSAIDivR__) (MODIFY_REG(RCC->DCKCFGR, RCC_DCKCFGR_PLLSAIDIVR, (__PLLSAIDivR__)))
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx */
/*-----------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------- Peripheral Clock selection -----------------------------------------------------*/
#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx) ||\
    defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) ||\
    defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F411xE)
/** @brief  Macro to configure the I2S clock source (I2SCLK).
  * @note   This function must be called before enabling the I2S APB clock.
  * @param  __SOURCE__: specifies the I2S clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_I2SCLKSOURCE_PLLI2S: PLLI2S clock used as I2S clock source.
  *            @arg RCC_I2SCLKSOURCE_EXT: External clock mapped on the I2S_CKIN pin
  *                                       used as I2S clock source.
  */
#define __HAL_RCC_I2S_CONFIG(__SOURCE__) (*(__IO uint32_t *) RCC_CFGR_I2SSRC_BB = (__SOURCE__))
#endif /* STM32F40xxx || STM32F41xxx || STM32F42xxx || STM32F43xxx */
                                 
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)
                                 
/** @brief  Macro to configure SAI1BlockA clock source selection.
  * @note   The SAI peripheral is only available with STM32F427/437/429/439xx Devices.      
  * @note   This function must be called before enabling PLLSAI, PLLI2S and  
  *         the SAI clock.
  * @param  __SOURCE__: specifies the SAI Block A clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_SAIACLKSOURCE_PLLI2S: PLLI2S_Q clock divided by PLLI2SDIVQ used 
  *                                           as SAI1 Block A clock. 
  *            @arg RCC_SAIACLKSOURCE_PLLSAI: PLLISAI_Q clock divided by PLLSAIDIVQ used 
  *                                           as SAI1 Block A clock.
  *            @arg RCC_SAIACLKSOURCE_Ext: External clock mapped on the I2S_CKIN pin
  *                                        used as SAI1 Block A clock.
  */
#define __HAL_RCC_SAI_BLOCKACLKSOURCE_CONFIG(__SOURCE__) (MODIFY_REG(RCC->DCKCFGR, RCC_DCKCFGR_SAI1ASRC, (__SOURCE__)))

/** @brief  Macro to configure SAI1BlockB clock source selection.
  * @note   The SAI peripheral is only available with STM32F427/437/429/439xx Devices.     
  * @note   This function must be called before enabling PLLSAI, PLLI2S and  
  *         the SAI clock.
  * @param  __SOURCE__: specifies the SAI Block B clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_SAIBCLKSOURCE_PLLI2S: PLLI2S_Q clock divided by PLLI2SDIVQ used 
  *                                           as SAI1 Block B clock. 
  *            @arg RCC_SAIBCLKSOURCE_PLLSAI: PLLISAI_Q clock divided by PLLSAIDIVQ used 
  *                                           as SAI1 Block B clock. 
  *            @arg RCC_SAIBCLKSOURCE_Ext: External clock mapped on the I2S_CKIN pin
  *                                        used as SAI1 Block B clock.
  */
#define __HAL_RCC_SAI_BLOCKBCLKSOURCE_CONFIG(__SOURCE__) (MODIFY_REG(RCC->DCKCFGR, RCC_DCKCFGR_SAI1BSRC, (__SOURCE__)))
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx */

#if defined(STM32F446xx)
/** @brief  Macro to configure SAI1 clock source selection.
  * @note   This configuration is only available with STM32F446xx Devices.      
  * @note   This function must be called before enabling PLL, PLLSAI, PLLI2S and  
  *         the SAI clock.
  * @param  __SOURCE__: specifies the SAI1 clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_SAI1CLKSOURCE_PLLI2S: PLLI2S_Q clock divided by PLLI2SDIVQ used as SAI1 clock. 
  *            @arg RCC_SAI1CLKSOURCE_PLLSAI: PLLISAI_Q clock divided by PLLSAIDIVQ used as SAI1 clock.
  *            @arg RCC_SAI1CLKSOURCE_PLLR: PLL VCO Output divided by PLLR used as SAI1 clock.  
  *            @arg RCC_SAI1CLKSOURCE_EXT: External clock mapped on the I2S_CKIN pin used as SAI1 clock.
  */
#define __HAL_RCC_SAI1_CONFIG(__SOURCE__) (MODIFY_REG(RCC->DCKCFGR, RCC_DCKCFGR_SAI1SRC, (__SOURCE__)))

/** @brief  Macro to Get SAI1 clock source selection.
  * @note   This configuration is only available with STM32F446xx Devices.      
  * @retval The clock source can be one of the following values:
  *            @arg RCC_SAI1CLKSOURCE_PLLI2S: PLLI2S_Q clock divided by PLLI2SDIVQ used as SAI1 clock. 
  *            @arg RCC_SAI1CLKSOURCE_PLLSAI: PLLISAI_Q clock divided by PLLSAIDIVQ used as SAI1 clock.
  *            @arg RCC_SAI1CLKSOURCE_PLLR: PLL VCO Output divided by PLLR used as SAI1 clock.  
  *            @arg RCC_SAI1CLKSOURCE_EXT: External clock mapped on the I2S_CKIN pin used as SAI1 clock.
  */
#define __HAL_RCC_GET_SAI1_SOURCE() (READ_BIT(RCC->DCKCFGR, RCC_DCKCFGR_SAI1SRC))

/** @brief  Macro to configure SAI2 clock source selection.
  * @note   This configuration is only available with STM32F446xx Devices.      
  * @note   This function must be called before enabling PLL, PLLSAI, PLLI2S and  
  *         the SAI clock.
  * @param  __SOURCE__: specifies the SAI2 clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_SAI2CLKSOURCE_PLLI2S: PLLI2S_Q clock divided by PLLI2SDIVQ used as SAI2 clock. 
  *            @arg RCC_SAI2CLKSOURCE_PLLSAI: PLLISAI_Q clock divided by PLLSAIDIVQ used as SAI2 clock.
  *            @arg RCC_SAI2CLKSOURCE_PLLR: PLL VCO Output divided by PLLR used as SAI2 clock.  
  *            @arg RCC_SAI2CLKSOURCE_PLLSRC: HSI or HSE depending from PLL Source clock used as SAI2 clock.
  */
#define __HAL_RCC_SAI2_CONFIG(__SOURCE__) (MODIFY_REG(RCC->DCKCFGR, RCC_DCKCFGR_SAI2SRC, (__SOURCE__)))

/** @brief  Macro to Get SAI2 clock source selection.
  * @note   This configuration is only available with STM32F446xx Devices.      
  * @retval The clock source can be one of the following values:
  *            @arg RCC_SAI2CLKSOURCE_PLLI2S: PLLI2S_Q clock divided by PLLI2SDIVQ used as SAI2 clock. 
  *            @arg RCC_SAI2CLKSOURCE_PLLSAI: PLLISAI_Q clock divided by PLLSAIDIVQ used as SAI2 clock.
  *            @arg RCC_SAI2CLKSOURCE_PLLR: PLL VCO Output divided by PLLR used as SAI2 clock.  
  *            @arg RCC_SAI2CLKSOURCE_PLLSRC: HSI or HSE depending from PLL Source clock used as SAI2 clock.
  */
#define __HAL_RCC_GET_SAI2_SOURCE() (READ_BIT(RCC->DCKCFGR, RCC_DCKCFGR_SAI2SRC))

/** @brief  Macro to configure I2S APB1 clock source selection.
  * @note   This configuration is only available with STM32F446xx Devices.      
  * @note   This function must be called before enabling PLL, PLLI2S and the I2S clock.
  * @param  __SOURCE__: specifies the I2S APB1 clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_I2SAPB1CLKSOURCE_PLLI2S: PLLI2S VCO output clock divided by PLLI2SR used as I2S clock. 
  *            @arg RCC_I2SAPB1CLKSOURCE_EXT: External clock mapped on the I2S_CKIN pin used as SAI1 clock.
  *            @arg RCC_I2SAPB1CLKSOURCE_PLLR: PLL VCO Output divided by PLLR used as SAI1 clock.  
  *            @arg RCC_I2SAPB1CLKSOURCE_PLLSRC: HSI or HSE depending from PLL source Clock.
  */
#define __HAL_RCC_I2S_APB1_CONFIG(__SOURCE__) (MODIFY_REG(RCC->DCKCFGR, RCC_DCKCFGR_I2S1SRC, (__SOURCE__)))

/** @brief  Macro to Get I2S APB1 clock source selection.
  * @note   This configuration is only available with STM32F446xx Devices.      
  * @retval The clock source can be one of the following values:
  *            @arg RCC_I2SAPB1CLKSOURCE_PLLI2S: PLLI2S VCO output clock divided by PLLI2SR used as I2S clock. 
  *            @arg RCC_I2SAPB1CLKSOURCE_EXT: External clock mapped on the I2S_CKIN pin used as SAI1 clock.
  *            @arg RCC_I2SAPB1CLKSOURCE_PLLR: PLL VCO Output divided by PLLR used as SAI1 clock.  
  *            @arg RCC_I2SAPB1CLKSOURCE_PLLSRC: HSI or HSE depending from PLL source Clock.
  */
#define __HAL_RCC_GET_I2S_APB1_SOURCE() (READ_BIT(RCC->DCKCFGR, RCC_DCKCFGR_I2S1SRC))

/** @brief  Macro to configure I2S APB2 clock source selection.
  * @note   This configuration is only available with STM32F446xx Devices.      
  * @note   This function must be called before enabling PLL, PLLI2S and the I2S clock.
  * @param  __SOURCE__: specifies the SAI Block A clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_I2SAPB2CLKSOURCE_PLLI2S: PLLI2S VCO output clock divided by PLLI2SR used as I2S clock. 
  *            @arg RCC_I2SAPB2CLKSOURCE_EXT: External clock mapped on the I2S_CKIN pin used as SAI1 clock.
  *            @arg RCC_I2SAPB2CLKSOURCE_PLLR: PLL VCO Output divided by PLLR used as SAI1 clock.  
  *            @arg RCC_I2SAPB2CLKSOURCE_PLLSRC: HSI or HSE depending from PLL source Clock.
  */
#define __HAL_RCC_I2S_APB2_CONFIG(__SOURCE__) (MODIFY_REG(RCC->DCKCFGR, RCC_DCKCFGR_I2S2SRC, (__SOURCE__)))

/** @brief  Macro to Get I2S APB2 clock source selection.
  * @note   This configuration is only available with STM32F446xx Devices.      
  * @retval The clock source can be one of the following values:
  *            @arg RCC_I2SAPB2CLKSOURCE_PLLI2S: PLLI2S VCO output clock divided by PLLI2SR used as I2S clock. 
  *            @arg RCC_I2SAPB2CLKSOURCE_EXT: External clock mapped on the I2S_CKIN pin used as SAI1 clock.
  *            @arg RCC_I2SAPB2CLKSOURCE_PLLR: PLL VCO Output divided by PLLR used as SAI1 clock.  
  *            @arg RCC_I2SAPB2CLKSOURCE_PLLSRC: HSI or HSE depending from PLL source Clock.
  */
#define __HAL_RCC_GET_I2S_APB2_SOURCE() (READ_BIT(RCC->DCKCFGR, RCC_DCKCFGR_I2S2SRC))

/** @brief  Macro to configure the CEC clock.
  * @param  __SOURCE__: specifies the CEC clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_CECCLKSOURCE_HSI: HSI selected as CEC clock
  *            @arg RCC_CECCLKSOURCE_LSE: LSE selected as CEC clock
  */
#define __HAL_RCC_CEC_CONFIG(__SOURCE__) (MODIFY_REG(RCC->DCKCFGR2, RCC_DCKCFGR2_CECSEL, (uint32_t)(__SOURCE__)))

/** @brief  Macro to Get the CEC clock.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_CECCLKSOURCE_HSI488: HSI selected as CEC clock
  *            @arg RCC_CECCLKSOURCE_LSE: LSE selected as CEC clock
  */
#define __HAL_RCC_GET_CEC_SOURCE() (READ_BIT(RCC->DCKCFGR2, RCC_DCKCFGR2_CECSEL))
                     
/** @brief  Macro to configure the FMPI2C1 clock.
  * @param  __SOURCE__: specifies the FMPI2C1 clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_FMPI2C1CLKSOURCE_APB: APB selected as CEC clock
  *            @arg RCC_FMPI2C1CLKSOURCE_SYSCLK: SYS clock selected as CEC clock
  *            @arg RCC_FMPI2C1CLKSOURCE_HSI: HSI selected as CEC clock
  */
#define __HAL_RCC_FMPI2C1_CONFIG(__SOURCE__) (MODIFY_REG(RCC->DCKCFGR2, RCC_DCKCFGR2_FMPI2C1SEL, (uint32_t)(__SOURCE__)))

/** @brief  Macro to Get the FMPI2C1 clock.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_FMPI2C1CLKSOURCE_APB: APB selected as CEC clock
  *            @arg RCC_FMPI2C1CLKSOURCE_SYSCLK: SYS clock selected as CEC clock
  *            @arg RCC_FMPI2C1CLKSOURCE_HSI: HSI selected as CEC clock
  */
#define __HAL_RCC_GET_FMPI2C1_SOURCE() (READ_BIT(RCC->DCKCFGR2, RCC_DCKCFGR2_FMPI2C1SEL))

/** @brief  Macro to configure the CLK48 clock.
  * @param  __SOURCE__: specifies the CK48 clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_CK48CLKSOURCE_PLLQ: PLL VCO Output divided by PLLQ used as CK48 clock. 
  *            @arg RCC_CK48CLKSOURCE_PLLSAIP: PLLSAI VCO Output divided by PLLSAIP used as CK48 clock. 
  */
#define __HAL_RCC_CLK48_CONFIG(__SOURCE__) (MODIFY_REG(RCC->DCKCFGR2, RCC_DCKCFGR2_CK48MSEL, (uint32_t)(__SOURCE__)))

/** @brief  Macro to Get the CLK48 clock.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_CK48CLKSOURCE_PLLQ: PLL VCO Output divided by PLLQ used as CK48 clock. 
  *            @arg RCC_CK48CLKSOURCE_PLLSAIP: PLLSAI VCO Output divided by PLLSAIP used as CK48 clock. 
  */
#define __HAL_RCC_GET_CLK48_SOURCE() (READ_BIT(RCC->DCKCFGR2, RCC_DCKCFGR2_CK48MSEL))

/** @brief  Macro to configure the SDIO clock.
  * @param  __SOURCE__: specifies the SDIO clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_SDIOCLKSOURCE_CK48: CK48 output used as SDIO clock. 
  *            @arg RCC_SDIOCLKSOURCE_SYSCLK: System clock output used as SDIO clock. 
  */
#define __HAL_RCC_SDIO_CONFIG(__SOURCE__) (MODIFY_REG(RCC->DCKCFGR2, RCC_DCKCFGR2_SDIOSEL, (uint32_t)(__SOURCE__)))

/** @brief  Macro to Get the SDIO clock.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_SDIOCLKSOURCE_CK48: CK48 output used as SDIO clock. 
  *            @arg RCC_SDIOCLKSOURCE_SYSCLK: System clock output used as SDIO clock. 
  */
#define __HAL_RCC_GET_SDIO_SOURCE() (READ_BIT(RCC->DCKCFGR2, RCC_DCKCFGR2_SDIOSEL))

/** @brief  Macro to configure the SPDIFRX clock.
  * @param  __SOURCE__: specifies the SPDIFRX clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_SPDIFRXCLKSOURCE_PLLR: PLL VCO Output divided by PLLR used as SPDIFRX clock.  
  *            @arg RCC_SPDIFRXCLKSOURCE_PLLI2SP: PLLI2S VCO Output divided by PLLI2SP used as SPDIFRX clock. 
  */
#define __HAL_RCC_SPDIFRX_CONFIG(__SOURCE__) (MODIFY_REG(RCC->DCKCFGR2, RCC_DCKCFGR2_SPDIFRXSEL, (uint32_t)(__SOURCE__)))

/** @brief  Macro to Get the SPDIFRX clock.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_SPDIFRXCLKSOURCE_PLLR: PLL VCO Output divided by PLLR used as SPDIFRX clock.  
  *            @arg RCC_SPDIFRXCLKSOURCE_PLLI2SP: PLLI2S VCO Output divided by PLLI2SP used as SPDIFRX clock. 
  */
#define __HAL_RCC_GET_SPDIFRX_SOURCE() (READ_BIT(RCC->DCKCFGR2, RCC_DCKCFGR2_SPDIFRXSEL))
#endif /* STM32F446xx */

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx)|| defined(STM32F439xx) ||\
    defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F411xE) || defined(STM32F446xx)
                           
/** @brief  Macro to configure the Timers clocks prescalers 
  * @note   This feature is only available with STM32F429x/439x Devices.  
  * @param  __PRESC__ : specifies the Timers clocks prescalers selection
  *         This parameter can be one of the following values:
  *            @arg RCC_TIMPRES_DESACTIVATED: The Timers kernels clocks prescaler is 
  *                 equal to HPRE if PPREx is corresponding to division by 1 or 2, 
  *                 else it is equal to [(HPRE * PPREx) / 2] if PPREx is corresponding to 
  *                 division by 4 or more.       
  *            @arg RCC_TIMPRES_ACTIVATED: The Timers kernels clocks prescaler is 
  *                 equal to HPRE if PPREx is corresponding to division by 1, 2 or 4, 
  *                 else it is equal to [(HPRE * PPREx) / 4] if PPREx is corresponding 
  *                 to division by 8 or more.
  */     
#define __HAL_RCC_TIMCLKPRESCALER(__PRESC__) (*(__IO uint32_t *) RCC_DCKCFGR_TIMPRE_BB = (__PRESC__))

#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx) || STM32F401xC || STM32F401xE || STM32F411xE || STM32F446xx */

/*-------------------------------------------------------------------------------------------------------------------*/

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || defined(STM32F446xx)
/** @brief Enable PLLSAI_RDY interrupt.
  */
#define __HAL_RCC_PLLSAI_ENABLE_IT() (RCC->CIR |= (RCC_CIR_PLLSAIRDYIE))

/** @brief Disable PLLSAI_RDY interrupt.
  */
#define __HAL_RCC_PLLSAI_DISABLE_IT() (RCC->CIR &= ~(RCC_CIR_PLLSAIRDYIE))

/** @brief Clear the PLLSAI RDY interrupt pending bits.
  */
#define __HAL_RCC_PLLSAI_CLEAR_IT() (RCC->CIR |= (RCC_CIR_PLLSAIRDYF))

/** @brief Check the PLLSAI RDY interrupt has occurred or not.
  * @retval The new state (TRUE or FALSE).
  */
#define __HAL_RCC_PLLSAI_GET_IT() ((RCC->CIR & (RCC_CIR_PLLSAIRDYIE)) == (RCC_CIR_PLLSAIRDYIE))

/** @brief  Check PLLSAI RDY flag is set or not.
  * @retval The new state (TRUE or FALSE).
  */
#define __HAL_RCC_PLLSAI_GET_FLAG() ((RCC->CR & (RCC_CR_PLLSAIRDY)) == (RCC_CR_PLLSAIRDY))

#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F446xx */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup RCCEx_Exported_Functions
  *  @{
  */

/** @addtogroup RCCEx_Exported_Functions_Group1
  *  @{
  */
HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit);
void HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit);

#if defined(STM32F446xx)
uint32_t HAL_RCCEx_GetPeriphCLKFreq(uint32_t PeriphClk);
#endif /* STM32F446xx */

#if defined(STM32F411xE) || defined(STM32F446xx)
void HAL_RCCEx_SelectLSEMode(uint8_t Mode);
#endif /* STM32F411xE || STM32F446xx */
/**
  * @}
  */ 

/**
  * @}
  */
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup RCCEx_Private_Constants RCCEx Private Constants
  * @{
  */

/** @defgroup RCCEx_BitAddress_AliasRegion RCC BitAddress AliasRegion
  * @brief RCC registers bit address in the alias region
  * @{
  */
/* --- CR Register ---*/  
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || defined(STM32F446xx)  
/* Alias word address of PLLSAION bit */
#define RCC_PLLSAION_BIT_NUMBER        0x1C
#define RCC_CR_PLLSAION_BB            (PERIPH_BB_BASE + (RCC_CR_OFFSET * 32) + (RCC_PLLSAION_BIT_NUMBER * 4))

/* --- DCKCFGR Register ---*/
/* Alias word address of TIMPRE bit */
#define RCC_DCKCFGR_OFFSET            (RCC_OFFSET + 0x8C)
#define RCC_TIMPRE_BIT_NUMBER          0x18
#define RCC_DCKCFGR_TIMPRE_BB         (PERIPH_BB_BASE + (RCC_DCKCFGR_OFFSET * 32) + (RCC_TIMPRE_BIT_NUMBER * 4))
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F446xx */

#define PLL_TIMEOUT_VALUE          ((uint32_t)100)  /* 100 ms */

/**
  * @}
  */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @addtogroup RCCEx_Private_Macros RCCEx Private Macros
  * @{
  */
/** @defgroup RCCEx_IS_RCC_Definitions RCC Private macros to check input parameters
  * @{
  */
 
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx)|| defined(STM32F439xx)
#define IS_RCC_PERIPHCLOCK(SELECTION) ((1 <= (SELECTION)) && ((SELECTION) <= 0x0000002F))
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx */

#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx)|| defined(STM32F417xx) ||\
    defined(STM32F401xC) || defined(STM32F401xE) || defined(STM32F411xE) 
#define IS_RCC_PERIPHCLOCK(SELECTION) ((1 <= (SELECTION)) && ((SELECTION) <= 0x00000003))
#endif /* STM32F405xx || STM32F415xx || STM32F407xx || STM32F417xx || STM32F401xC || STM32F401xE || STM32F411xE */

#if defined(STM32F446xx)
#define IS_RCC_PERIPHCLOCK(SELECTION) ((1 <= (SELECTION)) && ((SELECTION) <= 0x000007FF))
#endif /* STM32F446xx */

#define IS_RCC_PLLI2SN_VALUE(VALUE) ((192 <= (VALUE)) && ((VALUE) <= 432))
#define IS_RCC_PLLI2SR_VALUE(VALUE) ((2 <= (VALUE)) && ((VALUE) <= 7))  


#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx)|| defined(STM32F439xx) || defined(STM32F446xx)
#define IS_RCC_PLLI2SQ_VALUE(VALUE)     ((2 <= (VALUE)) && ((VALUE) <= 15))

#define IS_RCC_PLLSAIN_VALUE(VALUE)     ((49 <= (VALUE)) && ((VALUE) <= 432))

#define IS_RCC_PLLSAIQ_VALUE(VALUE)     ((2 <= (VALUE)) && ((VALUE) <= 15))

#define IS_RCC_PLLSAIR_VALUE(VALUE)     ((2 <= (VALUE)) && ((VALUE) <= 7))

#define IS_RCC_PLLSAI_DIVQ_VALUE(VALUE) ((1 <= (VALUE)) && ((VALUE) <= 32))

#define IS_RCC_PLLI2S_DIVQ_VALUE(VALUE) ((1 <= (VALUE)) && ((VALUE) <= 32))

#define IS_RCC_PLLSAI_DIVR_VALUE(VALUE) (((VALUE) == RCC_PLLSAIDIVR_2)  ||\
                                         ((VALUE) == RCC_PLLSAIDIVR_4)  ||\
                                         ((VALUE) == RCC_PLLSAIDIVR_8)  ||\
                                         ((VALUE) == RCC_PLLSAIDIVR_16))
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx */

#if defined(STM32F446xx) || defined(STM32F411xE) 
#define IS_RCC_PLLI2SM_VALUE(VALUE)     ((VALUE) <= 63)
 
#define IS_RCC_LSE_MODE(MODE)           (((MODE) == RCC_LSE_LOWPOWER_MODE) ||\
                                         ((MODE) == RCC_LSE_HIGHDRIVE_MODE))
#endif /* STM32F446xx || STM32F411xE */

#if defined(STM32F446xx)
#define IS_RCC_PLLR_VALUE(VALUE) ((2 <= (VALUE)) && ((VALUE) <= 7))
  
#define IS_RCC_PLLI2SP_VALUE(VALUE)       (((VALUE) == RCC_PLLI2SP_DIV2) ||\
                                           ((VALUE) == RCC_PLLI2SP_DIV4) ||\
                                           ((VALUE) == RCC_PLLI2SP_DIV6) ||\
                                           ((VALUE) == RCC_PLLI2SP_DIV8))

#define IS_RCC_PLLSAIM_VALUE(VALUE)       ((VALUE) <= 63)
  
#define IS_RCC_PLLSAIP_VALUE(VALUE)       (((VALUE) == RCC_PLLSAIP_DIV2) ||\
                                           ((VALUE) == RCC_PLLSAIP_DIV4) ||\
                                           ((VALUE) == RCC_PLLSAIP_DIV6) ||\
                                           ((VALUE) == RCC_PLLSAIP_DIV8))

#define IS_RCC_SAI1CLKSOURCE(SOURCE)      (((SOURCE) == RCC_SAI1CLKSOURCE_PLLSAI) ||\
                                           ((SOURCE) == RCC_SAI1CLKSOURCE_PLLI2S) ||\
                                           ((SOURCE) == RCC_SAI1CLKSOURCE_PLLR)   ||\
                                           ((SOURCE) == RCC_SAI1CLKSOURCE_EXT))

#define IS_RCC_SAI2CLKSOURCE(SOURCE)      (((SOURCE) == RCC_SAI2CLKSOURCE_PLLSAI) ||\
                                           ((SOURCE) == RCC_SAI2CLKSOURCE_PLLI2S) ||\
                                           ((SOURCE) == RCC_SAI2CLKSOURCE_PLLR)   ||\
                                           ((SOURCE) == RCC_SAI2CLKSOURCE_PLLSRC))
 
#define IS_RCC_I2SAPB1CLKSOURCE(SOURCE)   (((SOURCE) == RCC_I2SAPB1CLKSOURCE_PLLI2S) ||\
                                           ((SOURCE) == RCC_I2SAPB1CLKSOURCE_EXT)    ||\
                                           ((SOURCE) == RCC_I2SAPB1CLKSOURCE_PLLR)   ||\
                                           ((SOURCE) == RCC_I2SAPB1CLKSOURCE_PLLSRC))
                                              
 #define IS_RCC_I2SAPB2CLKSOURCE(SOURCE)  (((SOURCE) == RCC_I2SAPB2CLKSOURCE_PLLI2S) ||\
                                           ((SOURCE) == RCC_I2SAPB2CLKSOURCE_EXT)    ||\
                                           ((SOURCE) == RCC_I2SAPB2CLKSOURCE_PLLR)   ||\
                                           ((SOURCE) == RCC_I2SAPB2CLKSOURCE_PLLSRC))

#define IS_RCC_FMPI2C1CLKSOURCE(SOURCE)   (((SOURCE) == RCC_FMPI2C1CLKSOURCE_APB)    ||\
                                           ((SOURCE) == RCC_FMPI2C1CLKSOURCE_SYSCLK) ||\
                                           ((SOURCE) == RCC_FMPI2C1CLKSOURCE_HSI))

#define IS_RCC_CECCLKSOURCE(SOURCE)       (((SOURCE) == RCC_CECCLKSOURCE_HSI)   ||\
                                           ((SOURCE) == RCC_CECCLKSOURCE_LSE))

#define IS_RCC_CK48CLKSOURCE(SOURCE)      (((SOURCE) == RCC_CK48CLKSOURCE_PLLQ) ||\
                                           ((SOURCE) == RCC_CK48CLKSOURCE_PLLSAIP))

#define IS_RCC_SDIOCLKSOURCE(SOURCE)      (((SOURCE) == RCC_SDIOCLKSOURCE_CK48) ||\
                                           ((SOURCE) == RCC_SDIOCLKSOURCE_SYSCLK))

#define IS_RCC_SPDIFRXCLKSOURCE(SOURCE)     (((SOURCE) == RCC_SPDIFRXCLKSOURCE_PLLR) ||\
                                           ((SOURCE) == RCC_SPDIFRXCLKSOURCE_PLLI2SP))  
#endif /* STM32F446xx */ 

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

#endif /* __STM32F4xx_HAL_RCC_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
