/**
  ******************************************************************************
  * @file    stm32l0xx_hal_rcc_ex.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-June-2014
  * @brief   Header file of RCC HAL Extension module.
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
#ifndef __STM32L0xx_HAL_RCC_EX_H
#define __STM32L0xx_HAL_RCC_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal_def.h"

/** @addtogroup STM32L0xx_HAL_Driver
  * @{
  */

/** @addtogroup RCCEx
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/ 
/** 
  * @brief  RCC extended clocks structure definition  
  */
#if !defined(STM32L051xx) && !defined(STM32L061xx) 
typedef struct
{
  uint32_t PeriphClockSelection;   /*!< The Extended Clock to be configured.
                                        This parameter can be a value of @ref RCCEx_Periph_Clock_Selection */
  uint32_t Usart1ClockSelection;   /*!< USART1 clock source      
                                        This parameter can be a value of @ref RCCEx_USART1_Clock_Source */
                                   
  uint32_t Usart2ClockSelection;   /*!< USART2 clock source      
                                        This parameter can be a value of @ref RCCEx_USART2_Clock_Source */
                                   
  uint32_t Lpuart1ClockSelection;  /*!< LPUART1 clock source      
                                        This parameter can be a value of @ref RCCEx_LPUART_Clock_Source */
                                   
  uint32_t I2c1ClockSelection;     /*!< I2C1 clock source      
                                        This parameter can be a value of @ref RCCEx_I2C1_Clock_Source */
                                   
  uint32_t RTCClockSelection;      /*!< Specifies RTC Clock Prescalers Selection
                                        This parameter can be a value of @ref RCCEx_RTC_Clock_Source */
                                                                         
  uint32_t UsbClockSelection;      /*!< Specifies USB and RNG Clock  Selection
                                        This parameter can be a value of @ref RCCEx_USB_Clock_Source */

  uint32_t LptimClockSelection;    /*!< LPTIM1 clock source
                                        This parameter can be a value of @ref RCCEx_LPTIM1_Clock_Source */
  
}RCC_PeriphCLKInitTypeDef;
#endif /* !(STM32L051xx) && !(STM32L061xx) */

#if defined(STM32L051xx) || defined(STM32L061xx) 
typedef struct
{
  uint32_t PeriphClockSelection;   /*!< The Extended Clock to be configured.
                                        This parameter can be a value of @ref RCCEx_Periph_Clock_Selection */
  uint32_t Usart1ClockSelection;   /*!< USART1 clock source      
                                        This parameter can be a value of @ref RCCEx_USART1_Clock_Source */
                                   
  uint32_t Usart2ClockSelection;   /*!< USART2 clock source      
                                        This parameter can be a value of @ref RCCEx_USART2_Clock_Source */
                                   
  uint32_t Lpuart1ClockSelection;  /*!< LPUART1 clock source      
                                        This parameter can be a value of @ref RCCEx_LPUART1_Clock_Source */
                                   
  uint32_t I2c1ClockSelection;     /*!< I2C1 clock source      
                                        This parameter can be a value of @ref RCCEx_I2C1_Clock_Source */
                                   
  uint32_t RTCClockSelection;      /*!< Specifies RTC Clock Prescalers Selection
                                        This parameter can be a value of @ref RCCEx_RTC_Clock_Source */
                                                                         
  uint32_t LptimClockSelection;    /*!< LPTIM1 clock source
                                        This parameter can be a value of @ref RCCEx_LPTIM1_Clock_Source */
  
}RCC_PeriphCLKInitTypeDef;
#endif /* STM32L051xx || STM32L061xx */

#if !defined(STM32L051xx) && !defined(STM32L061xx) 
/** 
  * @brief  RCC CRS Status structures definition  
  */  
typedef enum 
{
  RCC_CRS_NONE      = 0x00,
  RCC_CRS_TIMEOUT   = 0x01,
  RCC_CRS_SYNCOK    = 0x02,
  RCC_CRS_SYNCWARM  = 0x04,
  RCC_CRS_SYNCERR   = 0x08,
  RCC_CRS_SYNCMISS  = 0x10,
  RCC_CRS_TRIMOV    = 0x20
} RCC_CRSStatusTypeDef;

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
                                      It can be calculated in using macro __HAL_RCC_CRS_CALCULATE_RELOADVALUE(_FTARGET_, _FSYNC_)
                                     This parameter must be a number between 0 and 0xFFFF or a value of @ref RCCEx_CRS_ReloadValueDefault .*/

  uint32_t ErrorLimitValue;       /*!< Specifies the value to be used to evaluate the captured frequency error value.
                                     This parameter must be a number between 0 and 0xFF or a value of @ref RCCEx_CRS_ErrorLimitDefault */

  uint32_t HSI48CalibrationValue; /*!< Specifies a user-programmable trimming value to the HSI48 oscillator.
                                     This parameter must be a number between 0 and 0x3F or a value of @ref RCCEx_CRS_HSI48CalibrationDefault */
   
}RCC_CRSInitTypeDef;

/** 
  * @brief RCC_CRS Synchronization structure definition  
  */
typedef struct
{
  uint32_t ReloadValue;           /*!< Specifies the value loaded in the Counter reload value.
                                     This parameter must be a number between 0 and 0xFFFF*/

  uint32_t HSI48CalibrationValue; /*!< Specifies value loaded in HSI48 oscillator smooth trimming.
                                     This parameter must be a number between 0 and 0x3F */
   
  uint32_t FreqErrorCapture;      /*!< Specifies the value loaded in the .FECAP, the frequency error counter 
                                       value latched in the time of the last SYNC event.
                                    This parameter must be a number between 0 and 0xFFFF */
                                    
  uint32_t FreqErrorDirection;    /*!< Specifies the value loaded in the .FEDIR, the counting direction of the 
                                                                    frequency error counter latched in the time of the last SYNC event. 
                                                                    It shows whether the actual frequency is below or above the target.
                                    This parameter must be a value of @ref RCCEx_CRS_FreqErrorDirection*/

}RCC_CRSSynchroInfoTypeDef;
#endif /* !(STM32L051xx ) && !(STM32L061xx ) */
 
/* Exported constants --------------------------------------------------------*/
/** @defgroup RCCEx_Exported_Constants
  * @{
  */

/** @defgroup RCCEx_Periph_Clock_Selection
  * @{
  */
#if !defined(STM32L051xx) && !defined(STM32L061xx) 
#define RCC_PERIPHCLK_USART1           ((uint32_t)0x00000001)
#define RCC_PERIPHCLK_USART2           ((uint32_t)0x00000002)
#define RCC_PERIPHCLK_LPUART1          ((uint32_t)0x00000004)
#define RCC_PERIPHCLK_I2C1             ((uint32_t)0x00000008)
#define RCC_PERIPHCLK_I2C2             ((uint32_t)0x00000010)
#define RCC_PERIPHCLK_RTC              ((uint32_t)0x00000020)
#define RCC_PERIPHCLK_USB              ((uint32_t)0x00000040)
#define RCC_PERIPHCLK_LPTIM1           ((uint32_t)0x00000080)


#define IS_RCC_PERIPHCLK(CLK) ((CLK) <= (RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_LPUART1 | \
                                         RCC_PERIPHCLK_I2C1   | RCC_PERIPHCLK_I2C2 | RCC_PERIPHCLK_RTC       |  \
                                         RCC_PERIPHCLK_USB | RCC_PERIPHCLK_LPTIM1))
#endif /* !(STM32L051xx) && !(STM32L061xx) */

#if defined(STM32L051xx) || defined(STM32L061xx) 
#define RCC_PERIPHCLK_USART1           ((uint32_t)0x00000001)
#define RCC_PERIPHCLK_USART2           ((uint32_t)0x00000002)
#define RCC_PERIPHCLK_LPUART1          ((uint32_t)0x00000004)
#define RCC_PERIPHCLK_I2C1             ((uint32_t)0x00000008)
#define RCC_PERIPHCLK_I2C2             ((uint32_t)0x00000010)
#define RCC_PERIPHCLK_RTC              ((uint32_t)0x00000020)
#define RCC_PERIPHCLK_LPTIM1           ((uint32_t)0x00000080)


#define IS_RCC_PERIPHCLK(CLK) ((CLK) <= (RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_LPUART1 | \
                                         RCC_PERIPHCLK_I2C1   | RCC_PERIPHCLK_I2C2 | RCC_PERIPHCLK_RTC       |  \
                                         RCC_PERIPHCLK_LPTIM1))
#endif /* !(STM32L051xx) && !(STM32L061xx) */
/**
  * @}
  */
  
/** @defgroup RCCEx_USART1_Clock_Source
  * @{
  */
#define RCC_USART1CLKSOURCE_PCLK2        ((uint32_t)0x00000000) 
#define RCC_USART1CLKSOURCE_SYSCLK       RCC_CCIPR_USART1SEL_0
#define RCC_USART1CLKSOURCE_HSI          RCC_CCIPR_USART1SEL_1
#define RCC_USART1CLKSOURCE_LSE          (RCC_CCIPR_USART1SEL_0 | RCC_CCIPR_USART1SEL_1)
#define IS_RCC_USART1CLKSOURCE(SOURCE)  (((SOURCE) == RCC_USART1CLKSOURCE_PCLK2)  || \
                                         ((SOURCE) == RCC_USART1CLKSOURCE_SYSCLK) || \
                                         ((SOURCE) == RCC_USART1CLKSOURCE_LSE)    || \
                                         ((SOURCE) == RCC_USART1CLKSOURCE_HSI))
/**
  * @}
  */

/** @defgroup RCCEx_USART2_Clock_Source
  * @{
  */
#define RCC_USART2CLKSOURCE_PCLK1        ((uint32_t)0x00000000) 
#define RCC_USART2CLKSOURCE_SYSCLK       RCC_CCIPR_USART2SEL_0
#define RCC_USART2CLKSOURCE_HSI          RCC_CCIPR_USART2SEL_1
#define RCC_USART2CLKSOURCE_LSE          (RCC_CCIPR_USART2SEL_0 | RCC_CCIPR_USART2SEL_1)
#define IS_RCC_USART2CLKSOURCE(SOURCE)  (((SOURCE) == RCC_USART2CLKSOURCE_PCLK1)  || \
                                         ((SOURCE) == RCC_USART2CLKSOURCE_SYSCLK) || \
                                         ((SOURCE) == RCC_USART2CLKSOURCE_LSE)    || \
                                         ((SOURCE) == RCC_USART2CLKSOURCE_HSI))
/**
  * @}
  */

/** @defgroup RCCEx_LPUART_Clock_Source
  * @{
  */
#define RCC_LPUART1CLKSOURCE_PCLK1        ((uint32_t)0x00000000) 
#define RCC_LPUART1CLKSOURCE_SYSCLK       RCC_CCIPR_LPUART1SEL_0
#define RCC_LPUART1CLKSOURCE_HSI          RCC_CCIPR_LPUART1SEL_1
#define RCC_LPUART1CLKSOURCE_LSE          (RCC_CCIPR_LPUART1SEL_0 | RCC_CCIPR_LPUART1SEL_1)
#define IS_RCC_LPUART1CLKSOURCE(SOURCE)  (((SOURCE) == RCC_LPUART1CLKSOURCE_PCLK1)  || \
                                         ((SOURCE) == RCC_LPUART1CLKSOURCE_SYSCLK) || \
                                         ((SOURCE) == RCC_LPUART1CLKSOURCE_LSE)    || \
                                         ((SOURCE) == RCC_LPUART1CLKSOURCE_HSI))
/**
  * @}
  */

/** @defgroup RCCEx_I2C1_Clock_Source
  * @{
  */
#define RCC_I2C1CLKSOURCE_PCLK1          ((uint32_t)0x00000000) 
#define RCC_I2C1CLKSOURCE_SYSCLK         RCC_CCIPR_I2C1SEL_0
#define RCC_I2C1CLKSOURCE_HSI            RCC_CCIPR_I2C1SEL_1
#define IS_RCC_I2C1CLKSOURCE(SOURCE)  (((SOURCE) == RCC_I2C1CLKSOURCE_PCLK1) || \
                                       ((SOURCE) == RCC_I2C1CLKSOURCE_SYSCLK)|| \
                                       ((SOURCE) == RCC_I2C1CLKSOURCE_HSI))
/**
  * @}
  */

/** @defgroup RCCEx_TIM_PRescaler_Selection
  * @{
  */
#define RCC_TIMPRES_DESACTIVATED        ((uint8_t)0x00)
#define RCC_TIMPRES_ACTIVATED           ((uint8_t)0x01)
/**
  * @}
  */

#if !defined(STM32L051xx) && !defined(STM32L061xx)
/** @defgroup RCCEx_USB_Clock_Source
  * @{
  */
#define RCC_USBCLKSOURCE_HSI48           RCC_CCIPR_HSI48SEL
#define RCC_USBCLKSOURCE_PLLCLK          ((uint32_t)0x00000000)

#define IS_RCC_USBCLKSOURCE(SOURCE)  (((SOURCE) == RCC_USBCLKSOURCE_HSI48) || \
                                      ((SOURCE) == RCC_USBCLKSOURCE_PLLCLK))
/**
  * @}
  */
  
/** @defgroup RCCEx_RNG_Clock_Source
  * @{
  */
#define RCC_RNGCLKSOURCE_HSI48           RCC_CCIPR_HSI48SEL
#define RCC_RNGCLKSOURCE_PLLCLK          ((uint32_t)0x00000000)

#define IS_RCC_RNGCLKSOURCE(SOURCE)  (((SOURCE) == RCC_RNGCLKSOURCE_HSI48) || \
                                      ((SOURCE) == RCC_RNGCLKSOURCE_PLLCLK))
/**
  * @}
  */  

/** @defgroup RCCEx_HSI48M_Clock_Source 
  * @{
  */

#define RCC_HSI48M_PLL                 ((uint32_t)0x00000000)
#define RCC_HSI48M_RC48                 RCC_CCIPR_HSI48SEL

#define IS_RCC_HSI48MCLKSOURCE(HSI48MCLK) (((HSI48MCLK) == RCC_HSI48M_PLL) || ((HSI48MCLK) == RCC_HSI48M_RC48))

/**
  * @}
  */
#endif /* !(STM32L051xx ) && !(STM32L061xx ) */ 

/** @defgroup RCCEx_LPTIM1_Clock_Source
  * @{
  */
#define RCC_LPTIM1CLKSOURCE_PCLK        ((uint32_t)0x00000000)
#define RCC_LPTIM1CLKSOURCE_LSI         RCC_CCIPR_LPTIM1SEL_0
#define RCC_LPTIM1CLKSOURCE_HSI         RCC_CCIPR_LPTIM1SEL_1
#define RCC_LPTIM1CLKSOURCE_LSE         RCC_CCIPR_LPTIM1SEL

#define IS_RCC_LPTIMCLK(LPTIMCLK)     (((LPTIMCLK) == RCC_LPTIM1CLKSOURCE_PCLK) || \
                                       ((LPTIMCLK) == RCC_LPTIM1CLKSOURCE_LSI)  || \
                                       ((LPTIMCLK) == RCC_LPTIM1CLKSOURCE_HSI)  || \
                                       ((LPTIMCLK) == RCC_LPTIM1CLKSOURCE_LSE))
/**
  * @}
  */

/** @defgroup RCCEx_StopWakeUp_Clock
  * @{
  */

#define RCC_StopWakeUpClock_MSI                ((uint32_t)0x00)
#define RCC_StopWakeUpClock_HSI                RCC_CFGR_STOPWUCK

#define IS_RCC_STOPWAKEUP_CLOCK(SOURCE) (((SOURCE) == RCC_StopWakeUpClock_MSI) || \
                                         ((SOURCE) == RCC_StopWakeUpClock_HSI))
/**
  * @}
  */ 

/** @defgroup RCCEx_LSEDrive_Configuration
  * @{
  */

#define RCC_LSEDRIVE_LOW                 ((uint32_t)0x00000000)
#define RCC_LSEDRIVE_MEDIUMLOW           RCC_CSR_LSEDRV_0
#define RCC_LSEDRIVE_MEDIUMHIGH          RCC_CSR_LSEDRV_1
#define RCC_LSEDRIVE_HIGH                RCC_CSR_LSEDRV
#define IS_RCC_LSE_DRIVE(DRIVE) (((DRIVE) == RCC_LSEDRIVE_LOW) || ((DRIVE) == RCC_LSEDRIVE_MEDIUMLOW) || \
                                 ((DRIVE) == RCC_LSEDRIVE_MEDIUMHIGH) || ((DRIVE) == RCC_LSEDRIVE_HIGH))
/**
  * @}
  */  

#if !defined(STM32L051xx) && !defined(STM32L061xx)
/** @defgroup RCCEx_CRS_SynchroSource
  * @{
  */
#define RCC_CRS_SYNC_SOURCE_GPIO       ((uint32_t)0x00)        /*!< Synchro Signal source GPIO */
#define RCC_CRS_SYNC_SOURCE_LSE        CRS_CFGR_SYNCSRC_0      /*!< Synchro Signal source LSE */
#define RCC_CRS_SYNC_SOURCE_USB        CRS_CFGR_SYNCSRC_1      /*!< Synchro Signal source USB SOF (default)*/
  
#define IS_RCC_CRS_SYNC_SOURCE(_SOURCE_) (((_SOURCE_) == RCC_CRS_SYNC_SOURCE_GPIO) || \
                                      ((_SOURCE_) == RCC_CRS_SYNC_SOURCE_LSE) ||\
                                      ((_SOURCE_) == RCC_CRS_SYNC_SOURCE_USB))
/**
  * @}
  */

/** @defgroup RCCEx_CRS_SynchroDivider
  * @{
  */
#define RCC_CRS_SYNC_DIV1        ((uint32_t)0x00)                          /*!< Synchro Signal not divided (default) */
#define RCC_CRS_SYNC_DIV2        CRS_CFGR_SYNCDIV_0                        /*!< Synchro Signal divided by 2 */
#define RCC_CRS_SYNC_DIV4        CRS_CFGR_SYNCDIV_1                        /*!< Synchro Signal divided by 4 */
#define RCC_CRS_SYNC_DIV8        (CRS_CFGR_SYNCDIV_1 | CRS_CFGR_SYNCDIV_0) /*!< Synchro Signal divided by 8 */
#define RCC_CRS_SYNC_DIV16       CRS_CFGR_SYNCDIV_2                        /*!< Synchro Signal divided by 16 */
#define RCC_CRS_SYNC_DIV32       (CRS_CFGR_SYNCDIV_2 | CRS_CFGR_SYNCDIV_0) /*!< Synchro Signal divided by 32 */
#define RCC_CRS_SYNC_DIV64       (CRS_CFGR_SYNCDIV_2 | CRS_CFGR_SYNCDIV_1) /*!< Synchro Signal divided by 64 */
#define RCC_CRS_SYNC_DIV128      CRS_CFGR_SYNCDIV                          /*!< Synchro Signal divided by 128 */
  
#define IS_RCC_CRS_SYNC_DIV(_DIV_) (((_DIV_) == RCC_CRS_SYNC_DIV1) || ((_DIV_) == RCC_CRS_SYNC_DIV2)   ||\
                                ((_DIV_) == RCC_CRS_SYNC_DIV4) || ((_DIV_) == RCC_CRS_SYNC_DIV8)   || \
                                ((_DIV_) == RCC_CRS_SYNC_DIV16) || ((_DIV_) == RCC_CRS_SYNC_DIV32) || \
                                ((_DIV_) == RCC_CRS_SYNC_DIV64) || ((_DIV_) == RCC_CRS_SYNC_DIV128))
/**
  * @}
  */

/** @defgroup RCCEx_CRS_SynchroPolarity
  * @{
  */
#define RCC_CRS_SYNC_POLARITY_RISING        ((uint32_t)0x00)      /*!< Synchro Active on rising edge (default) */
#define RCC_CRS_SYNC_POLARITY_FALLING       CRS_CFGR_SYNCPOL      /*!< Synchro Active on falling edge */
  
#define IS_RCC_CRS_SYNC_POLARITY(_POLARITY_) (((_POLARITY_) == RCC_CRS_SYNC_POLARITY_RISING) || \
                                      ((_POLARITY_) == RCC_CRS_SYNC_POLARITY_FALLING))
/**
  * @}
  */
  
/** @defgroup RCCEx_CRS_ReloadValueDefault
  * @{
  */
#define RCC_CRS_RELOADVALUE_DEFAULT         ((uint32_t)0xBB7F)      /*!< The reset value of the RELOAD field corresponds 
                                                              to a target frequency of 48 MHz and a synchronization signal frequency of 1 kHz (SOF signal from USB). */
    
#define IS_RCC_CRS_RELOADVALUE(_VALUE_) (((_VALUE_) <= 0xFFFF))
/**
  * @}
  */
  
/** @defgroup RCCEx_CRS_ErrorLimitDefault
  * @{
  */
#define RCC_CRS_ERRORLIMIT_DEFAULT          ((uint32_t)0x22)      /*!< Default Frequency error limit */
    
#define IS_RCC_CRS_ERRORLIMIT(_VALUE_) (((_VALUE_) <= 0xFF))
/**
  * @}
  */

/** @defgroup RCCEx_CRS_HSI48CalibrationDefault
  * @{
  */
#define RCC_CRS_HSI48CALIBRATION_DEFAULT    ((uint32_t)0x20)      /*!< The default value is 32, which corresponds to the middle of the trimming interval. 
                                                                The trimming step is around 67 kHz between two consecutive TRIM steps. A higher TRIM value
                                                                corresponds to a higher output frequency */
    
#define IS_RCC_CRS_HSI48CALIBRATION(_VALUE_) (((_VALUE_) <= 0x3F))
/**
  * @}
  */

/** @defgroup RCCEx_CRS_FreqErrorDirection
  * @{
  */
#define RCC_CRS_FREQERRORDIR_UP             ((uint32_t)0x00)          /*!< Upcounting direction, the actual frequency is above the target */
#define RCC_CRS_FREQERRORDIR_DOWN           ((uint32_t)CRS_ISR_FEDIR) /*!< Downcounting direction, the actual frequency is below the target */
    
#define IS_RCC_CRS_FREQERRORDIR(_DIR_) (((_DIR_) == RCC_CRS_FREQERRORDIR_UP) || \
                                      ((_DIR_) == RCC_CRS_FREQERRORDIR_DOWN))
/**
  * @}
  */

/** @defgroup RCCEx_CRS_Interrupt_Sources
  * @{
  */
#define RCC_CRS_IT_SYNCOK             CRS_ISR_SYNCOKF    /*!< SYNC event OK */
#define RCC_CRS_IT_SYNCWARN           CRS_ISR_SYNCWARNF  /*!< SYNC warning */
#define RCC_CRS_IT_ERR                CRS_ISR_ERRF       /*!< error */
#define RCC_CRS_IT_ESYNC              CRS_ISR_ESYNCF     /*!< Expected SYNC */
#define RCC_CRS_IT_TRIMOVF            CRS_ISR_TRIMOVF    /*!< Trimming overflow or underflow */
#define RCC_CRS_IT_SYNCERR            CRS_ISR_SYNCERR    /*!< SYNC error */
#define RCC_CRS_IT_SYNCMISS           CRS_ISR_SYNCMISS    /*!< SYNC missed*/

/**
  * @}
  */
  
/** @defgroup RCCEx_CRS_Flags
  * @{
  */
#define RCC_CRS_FLAG_SYNCOK             CRS_ISR_SYNCOKF     /* SYNC event OK flag     */
#define RCC_CRS_FLAG_SYNCWARN           CRS_ISR_SYNCWARNF   /* SYNC warning flag      */
#define RCC_CRS_FLAG_ERR                CRS_ISR_ERRF        /* Error flag        */
#define RCC_CRS_FLAG_ESYNC              CRS_ISR_ESYNCF      /* Expected SYNC flag     */
#define RCC_CRS_FLAG_TRIMOVF            CRS_ISR_TRIMOVF     /*!< Trimming overflow or underflow */
#define RCC_CRS_FLAG_SYNCERR            CRS_ISR_SYNCERR     /*!< SYNC error */
#define RCC_CRS_FLAG_SYNCMISS           CRS_ISR_SYNCMISS    /*!< SYNC missed*/

/**
  * @}
  */

#endif /* !(STM32L051xx ) && !(STM32L061xx ) */  
/**
  * @}
  */ 

/* Exported macro ------------------------------------------------------------*/
/** @defgroup RCCEx_Exported_Macros
  * @{
  */

/** @brief  Enable or disable the AHB peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  */

#if defined(STM32L062xx) || defined(STM32L063xx)
#define __CRYP_CLK_ENABLE()          (RCC->AHBENR |= (RCC_AHBENR_CRYPEN))
#define __CRYP_CLK_DISABLE()         (RCC->AHBENR  &= ~ (RCC_AHBENR_CRYPEN))
#endif /* STM32L062xx || STM32L063xx  */

#if !defined(STM32L051xx) && !defined(STM32L061xx) 
#define __TSC_CLK_ENABLE()             (RCC->AHBENR |= (RCC_AHBENR_TSCEN))
#define __TSC_CLK_DISABLE()            (RCC->AHBENR  &= ~ (RCC_AHBENR_TSCEN))

#define __RNG_CLK_ENABLE()            (RCC->AHBENR |= (RCC_AHBENR_RNGEN))
#define __RNG_CLK_DISABLE()           (RCC->AHBENR  &= ~ (RCC_AHBENR_RNGEN))
#endif /* !(STM32L051xx ) && !(STM32L061xx ) */

/** @brief  Enable or disable the APB1 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.   
  */

#if !defined(STM32L051xx) && !defined(STM32L061xx) 
#define __USB_CLK_ENABLE()         (RCC->APB1ENR |= (RCC_APB1ENR_USBEN))
#define __USB_CLK_DISABLE()        (RCC->APB1ENR &= ~ (RCC_APB1ENR_USBEN))
#endif /* !(STM32L051xx ) && !(STM32L061xx ) */

#if !defined(STM32L051xx) && !defined(STM32L061xx)
#define __CRS_CLK_ENABLE()     (RCC->APB1ENR |= (RCC_APB1ENR_CRSEN))
#define __CRS_CLK_DISABLE()    (RCC->APB1ENR &= ~(RCC_APB1ENR_CRSEN))
#endif /* !(STM32L051xx ) && !(STM32L061xx ) */
       

#if defined(STM32L053xx) || defined(STM32L063xx)
#define __LCD_CLK_ENABLE()            (RCC->APB1ENR |= (RCC_APB1ENR_LCDEN))
#define __LCD_CLK_DISABLE()           (RCC->APB1ENR &= ~ (RCC_APB1ENR_LCDEN))
#endif /* STM32L053xx || STM32L063xx  */

#if defined(STM32L053xx) || defined(STM32L063xx) || \
    defined(STM32L052xx) || defined(STM32L062xx) || \
    defined(STM32L051xx) || defined(STM32L061xx)
#define __TIM2_CLK_ENABLE()    (RCC->APB1ENR |= (RCC_APB1ENR_TIM2EN))
#define __TIM6_CLK_ENABLE()    (RCC->APB1ENR |= (RCC_APB1ENR_TIM6EN))
#define __SPI2_CLK_ENABLE()    (RCC->APB1ENR |= (RCC_APB1ENR_SPI2EN))
#define __USART2_CLK_ENABLE()  (RCC->APB1ENR |= (RCC_APB1ENR_USART2EN))
#define __LPUART1_CLK_ENABLE()  (RCC->APB1ENR |= (RCC_APB1ENR_LPUART1EN))
#define __I2C1_CLK_ENABLE()    (RCC->APB1ENR |= (RCC_APB1ENR_I2C1EN))
#define __I2C2_CLK_ENABLE()    (RCC->APB1ENR |= (RCC_APB1ENR_I2C2EN))
#define __DAC_CLK_ENABLE()     (RCC->APB1ENR |= (RCC_APB1ENR_DACEN))
#define __LPTIM1_CLK_ENABLE()   (RCC->APB1ENR |= (RCC_APB1ENR_LPTIM1EN))

#define __TIM2_CLK_DISABLE()    (RCC->APB1ENR &= ~ (RCC_APB1ENR_TIM2EN))
#define __TIM6_CLK_DISABLE()    (RCC->APB1ENR &= ~ (RCC_APB1ENR_TIM6EN))
#define __SPI2_CLK_DISABLE()    (RCC->APB1ENR &= ~ (RCC_APB1ENR_SPI2EN))
#define __USART2_CLK_DISABLE()  (RCC->APB1ENR &= ~ (RCC_APB1ENR_USART2EN))
#define __LPUART1_CLK_DISABLE()  (RCC->APB1ENR &= ~ (RCC_APB1ENR_LPUART1EN))
#define __I2C1_CLK_DISABLE()    (RCC->APB1ENR &= ~ (RCC_APB1ENR_I2C1EN))
#define __I2C2_CLK_DISABLE()    (RCC->APB1ENR &= ~ (RCC_APB1ENR_I2C2EN))
#define __DAC_CLK_DISABLE()     (RCC->APB1ENR &= ~ (RCC_APB1ENR_DACEN))
#define __LPTIM1_CLK_DISABLE()   (RCC->APB1ENR &= ~ (RCC_APB1ENR_LPTIM1EN))
#endif /* STM32L051xx  || STM32L061xx  || */
       /* STM32L052xx  || STM32L062xx  || */
       /* STM32L053xx  || STM32L063xx  || */
       
/** @brief  Enable or disable the APB2 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before 
  *         using it.   
  */

#if defined(STM32L053xx) || defined(STM32L063xx) || \
    defined(STM32L052xx) || defined(STM32L062xx) || \
    defined(STM32L051xx) || defined(STM32L061xx)
#define __TIM21_CLK_ENABLE()    (RCC->APB2ENR |= (RCC_APB2ENR_TIM21EN))
#define __TIM22_CLK_ENABLE()    (RCC->APB2ENR |= (RCC_APB2ENR_TIM22EN))
#define __FIREWALL_CLK_ENABLE() (RCC->APB2ENR |= (RCC_APB2ENR_MIFIEN))
#define __ADC1_CLK_ENABLE()     (RCC->APB2ENR |= (RCC_APB2ENR_ADC1EN))
#define __SPI1_CLK_ENABLE()     (RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN))
#define __USART1_CLK_ENABLE()   (RCC->APB2ENR |= (RCC_APB2ENR_USART1EN))

#define __TIM21_CLK_DISABLE()    (RCC->APB2ENR &= ~  (RCC_APB2ENR_TIM21EN))
#define __TIM22_CLK_DISABLE()    (RCC->APB2ENR &= ~  (RCC_APB2ENR_TIM22EN))
#define __FIREWALL_CLK_DISABLE() (RCC->APB2ENR &= ~  (RCC_APB2ENR_MIFIEN))
#define __ADC1_CLK_DISABLE()     (RCC->APB2ENR &= ~  (RCC_APB2ENR_ADC1EN))
#define __SPI1_CLK_DISABLE()     (RCC->APB2ENR &= ~  (RCC_APB2ENR_SPI1EN))
#define __USART1_CLK_DISABLE()   (RCC->APB2ENR &= ~  (RCC_APB2ENR_USART1EN))
#endif /* STM32L051xx  || STM32L061xx  || */
       /* STM32L052xx  || STM32L062xx  || */
       /* STM32L053xx  || STM32L063xx  || */

/** @brief  Force or release AHB peripheral reset.
  */  
#if defined(STM32L062xx) || defined(STM32L063xx)
#define __CRYP_FORCE_RESET()    (RCC->AHBRSTR |= (RCC_AHBRSTR_CRYPRST))
#define __CRYP_RELEASE_RESET()   (RCC->AHBRSTR &= ~ (RCC_AHBRSTR_CRYPRST))
#endif /* STM32L062xx || STM32L063xx  */

#if !defined(STM32L051xx) && !defined(STM32L061xx)
#define __TSC_FORCE_RESET()        (RCC->AHBRSTR |= (RCC_AHBRSTR_TSCRST))
#define __TSC_RELEASE_RESET()      (RCC->AHBRSTR &= ~ (RCC_AHBRSTR_TSCRST))
#define __RNG_FORCE_RESET()        (RCC->AHBRSTR |= (RCC_AHBRSTR_RNGRST))
#define __RNG_RELEASE_RESET()      (RCC->AHBRSTR &= ~ (RCC_AHBRSTR_RNGRST))
#endif /* !(STM32L051xx ) && !(STM32L061xx ) */

/** @brief  Force or release APB1 peripheral reset.
  */  
#if defined(STM32L053xx) || defined(STM32L063xx) || \
    defined(STM32L052xx) || defined(STM32L062xx) || \
    defined(STM32L051xx) || defined(STM32L061xx)
#define __TIM2_FORCE_RESET()     (RCC->APB1RSTR |= (RCC_APB1RSTR_TIM2RST))
#define __TIM6_FORCE_RESET()     (RCC->APB1RSTR |= (RCC_APB1RSTR_TIM6RST))
#define __LPTIM1_FORCE_RESET()   (RCC->APB1RSTR |= (RCC_APB1RSTR_LPTIM1RST))
#define __I2C1_FORCE_RESET()     (RCC->APB1RSTR |= (RCC_APB1RSTR_I2C1RST))
#define __I2C2_FORCE_RESET()     (RCC->APB1RSTR |= (RCC_APB1RSTR_I2C2RST))
#define __USART2_FORCE_RESET()   (RCC->APB1RSTR |= (RCC_APB1RSTR_USART2RST))
#define __LPUART1_FORCE_RESET()  (RCC->APB1RSTR |= (RCC_APB1RSTR_LPUART1RST))
#define __SPI2_FORCE_RESET()     (RCC->APB1RSTR |= (RCC_APB1RSTR_SPI2RST))
#define __DAC_FORCE_RESET()      (RCC->APB1RSTR |= (RCC_APB1RSTR_DACRST))

#define __TIM2_RELEASE_RESET()     (RCC->APB1RSTR &= ~ (RCC_APB1RSTR_TIM2RST))
#define __TIM6_RELEASE_RESET()     (RCC->APB1RSTR &= ~ (RCC_APB1RSTR_TIM6RST))
#define __LPTIM1_RELEASE_RESET()   (RCC->APB1RSTR &= ~ (RCC_APB1RSTR_LPTIM1RST))
#define __I2C1_RELEASE_RESET()     (RCC->APB1RSTR &= ~ (RCC_APB1RSTR_I2C1RST))
#define __I2C2_RELEASE_RESET()     (RCC->APB1RSTR &= ~ (RCC_APB1RSTR_I2C2RST))
#define __USART2_RELEASE_RESET()   (RCC->APB1RSTR &= ~ (RCC_APB1RSTR_USART2RST))
#define __LPUART1_RELEASE_RESET()  (RCC->APB1RSTR &= ~ (RCC_APB1RSTR_LPUART1RST))
#define __SPI2_RELEASE_RESET()     (RCC->APB1RSTR &= ~ (RCC_APB1RSTR_SPI2RST))
#define __DAC_RELEASE_RESET()      (RCC->APB1RSTR &= ~ (RCC_APB1RSTR_DACRST))
#endif /* STM32L051xx  || STM32L061xx  || */
       /* STM32L052xx  || STM32L062xx  || */
       /* STM32L053xx  || STM32L063xx  || */
       
#if !defined(STM32L051xx) && !defined(STM32L061xx)
#define __USB_FORCE_RESET()        (RCC->APB1RSTR |= (RCC_APB1RSTR_USBRST))
#define __USB_RELEASE_RESET()      (RCC->APB1RSTR &= ~ (RCC_APB1RSTR_USBRST))
#endif /* !(STM32L051xx ) && !(STM32L061xx ) */

#if !defined(STM32L051xx) && !defined(STM32L061xx)
#define __CRS_FORCE_RESET()      (RCC->APB1RSTR |= (RCC_APB1RSTR_CRSRST))
#define __CRS_RELEASE_RESET()    (RCC->APB1RSTR &= ~(RCC_APB1RSTR_CRSRST))
#endif /* !(STM32L051xx ) && !(STM32L061xx ) */

#if defined(STM32L053xx) || defined(STM32L063xx)
#define __LCD_FORCE_RESET()           (RCC->APB1RSTR |= (RCC_APB1RSTR_LCDRST))
#define __LCD_RELEASE_RESET()         (RCC->APB1RSTR &= ~ (RCC_APB1RSTR_LCDRST))
#endif /* STM32L053xx || STM32L063xx  */

/** @brief  Force or release APB2 peripheral reset.
  */ 
#if defined(STM32L053xx) || defined(STM32L063xx) || \
    defined(STM32L052xx) || defined(STM32L062xx) || \
    defined(STM32L051xx) || defined(STM32L061xx)
#define __USART1_FORCE_RESET()     (RCC->APB2RSTR |= (RCC_APB2RSTR_USART1RST))
#define __ADC1_FORCE_RESET()       (RCC->APB2RSTR |= (RCC_APB2RSTR_ADC1RST))
#define __SPI1_FORCE_RESET()       (RCC->APB2RSTR |= (RCC_APB2RSTR_SPI1RST))
#define __TIM21_FORCE_RESET()      (RCC->APB2RSTR |= (RCC_APB2RSTR_TIM21RST))
#define __TIM22_FORCE_RESET()      (RCC->APB2RSTR |= (RCC_APB2RSTR_TIM22RST))

#define __USART1_RELEASE_RESET()     (RCC->APB2RSTR &= ~ (RCC_APB2RSTR_USART1RST))
#define __ADC1_RELEASE_RESET()       (RCC->APB2RSTR &= ~ (RCC_APB2RSTR_ADC1RST))
#define __SPI1_RELEASE_RESET()       (RCC->APB2RSTR &= ~ (RCC_APB2RSTR_SPI1RST))
#define __TIM21_RELEASE_RESET()      (RCC->APB2RSTR &= ~ (RCC_APB2RSTR_TIM21RST))
#define __TIM22_RELEASE_RESET()      (RCC->APB2RSTR &= ~ (RCC_APB2RSTR_TIM22RST))
#endif /* STM32L051xx  || STM32L061xx  || */
       /* STM32L052xx  || STM32L062xx  || */
       /* STM32L053xx  || STM32L063xx  || */

/** @brief  Enable or disable the AHB peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */

#if !defined(STM32L051xx) && !defined(STM32L061xx)
#define __TSC_CLK_SLEEP_ENABLE()           (RCC->AHBSMENR |= (RCC_AHBSMENR_TSCSMEN))
#define __RNG_CLK_SLEEP_ENABLE()           (RCC->AHBSMENR |= (RCC_AHBSMENR_RNGSMEN))
#define __TSC_CLK_SLEEP_DISABLE()          (RCC->AHBSMENR  &= ~ (RCC_AHBSMENR_TSCSMEN))
#define __RNG_CLK_SLEEP_DISABLE()          (RCC->AHBSMENR  &= ~ (RCC_AHBSMENR_RNGSMEN))
#endif /* !(STM32L051xx ) && !(STM32L061xx ) */
       
#if defined(STM32L062xx) || defined(STM32L063xx)
#define __CRYP_CLK_SLEEP_ENABLE()          (RCC->AHBLPENR |= (RCC_AHBSMENR_CRYPSMEN))
#define __CRYP_CLK_SLEEP_DISABLE()         (RCC->AHBLPENR  &= ~ (RCC_AHBSMENR_CRYPSMEN))
#endif /* STM32L062xx || STM32L063xx  */


/** @brief  Enable or disable the APB1 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */

#if defined(STM32L053xx) || defined(STM32L063xx) || \
    defined(STM32L052xx) || defined(STM32L062xx) || \
    defined(STM32L051xx) || defined(STM32L061xx)
#define __TIM2_CLK_SLEEP_ENABLE()    (RCC->APB1SMENR |= (RCC_APB1SMENR_TIM2SMEN))
#define __TIM6_CLK_SLEEP_ENABLE()    (RCC->APB1SMENR |= (RCC_APB1SMENR_TIM6SMEN))
#define __SPI2_CLK_SLEEP_ENABLE()    (RCC->APB1SMENR |= (RCC_APB1SMENR_SPI2SMEN))
#define __USART2_CLK_SLEEP_ENABLE()  (RCC->APB1SMENR |= (RCC_APB1SMENR_USART2SMEN))
#define __LPUART1_CLK_SLEEP_ENABLE()  (RCC->APB1SMENR |= (RCC_APB1SMENR_LPUART1SMEN))
#define __I2C1_CLK_SLEEP_ENABLE()    (RCC->APB1SMENR |= (RCC_APB1SMENR_I2C1SMEN))
#define __I2C2_CLK_SLEEP_ENABLE()    (RCC->APB1SMENR |= (RCC_APB1SMENR_I2C2SMEN))
#define __DAC_CLK_SLEEP_ENABLE()     (RCC->APB1SMENR |= (RCC_APB1SMENR_DACSMEN))
#define __LPTIM1_CLK_SLEEP_ENABLE()   (RCC->APB1SMENR |= (RCC_APB1SMENR_LPTIM1SMEN))

#define __TIM2_CLK_SLEEP_DISABLE()    (RCC->APB1SMENR &= ~ (RCC_APB1SMENR_TIM2SMEN))
#define __TIM6_CLK_SLEEP_DISABLE()    (RCC->APB1SMENR &= ~ (RCC_APB1SMENR_TIM6SMEN))
#define __SPI2_CLK_SLEEP_DISABLE()    (RCC->APB1SMENR &= ~ (RCC_APB1SMENR_SPI2SMEN))
#define __USART2_CLK_SLEEP_DISABLE()  (RCC->APB1SMENR &= ~ (RCC_APB1SMENR_USART2SMEN))
#define __LPUART1_CLK_SLEEP_DISABLE()  (RCC->APB1SMENR &= ~ (RCC_APB1SMENR_LPUART1SMEN))
#define __I2C1_CLK_SLEEP_DISABLE()    (RCC->APB1SMENR &= ~ (RCC_APB1SMENR_I2C1SMEN))
#define __I2C2_CLK_SLEEP_DISABLE()    (RCC->APB1SMENR &= ~ (RCC_APB1SMENR_I2C2SMEN))
#define __DAC_CLK_SLEEP_DISABLE()     (RCC->APB1SMENR &= ~ (RCC_APB1SMENR_DACSMEN))
#define __LPTIM1_CLK_SLEEP_DISABLE()   (RCC->APB1SMENR &= ~ (RCC_APB1SMENR_LPTIM1SMEN))
#endif /* STM32L051xx  || STM32L061xx  || */
       /* STM32L052xx  || STM32L062xx  || */
       /* STM32L053xx  || STM32L063xx  || */
       
#if !defined(STM32L051xx) && !defined(STM32L061xx)
#define __USB_CLK_SLEEP_ENABLE()   (RCC->APB1SMENR |= (RCC_APB1SMENR_USBSMEN))
#define __USB_CLK_SLEEP_DISABLE()  (RCC->APB1SMENR &= ~ (RCC_APB1SMENR_USBSMEN))

#define __CRS_CLK_SLEEP_ENABLE()   (RCC->APB1SMENR |= (RCC_APB1SMENR_CRSSMEN))
#define __CRS_CLK_SLEEP_DISABLE()  (RCC->APB1SMENR &= ~ (RCC_APB1SMENR_CRSSMEN))
#endif /* !(STM32L051xx ) && !(STM32L061xx ) */

#if defined(STM32L053xx) || defined(STM32L063xx)
#define __LCD_CLK_SLEEP_ENABLE()      (RCC->APB1SMENR |= (RCC_APB1SMENR_LCDSMEN))
#define __LCD_CLK_SLEEP_DISABLE()     (RCC->APB1SMENR &= ~ (RCC_APB1SMENR_LCDSMEN))
#endif /* STM32L053xx || STM32L063xx  */

/** @brief  Enable or disable the APB2 peripheral clock during Low Power (Sleep) mode.
  * @note   Peripheral clock gating in SLEEP mode can be used to further reduce
  *         power consumption.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  */
#if defined(STM32L053xx) || defined(STM32L063xx) || \
    defined(STM32L052xx) || defined(STM32L062xx) || \
    defined(STM32L051xx) || defined(STM32L061xx)
#define __TIM21_CLK_SLEEP_ENABLE()    (RCC->APB2SMENR |= (RCC_APB2SMENR_TIM21SMEN))
#define __TIM22_CLK_SLEEP_ENABLE()    (RCC->APB2SMENR |= (RCC_APB2SMENR_TIM22SMEN))
#define __ADC1_CLK_SLEEP_ENABLE()     (RCC->APB2SMENR |= (RCC_APB2SMENR_ADC1SMEN))
#define __SPI1_CLK_SLEEP_ENABLE()     (RCC->APB2SMENR |= (RCC_APB2SMENR_SPI1SMEN))
#define __USART1_CLK_SLEEP_ENABLE()   (RCC->APB2SMENR |= (RCC_APB2SMENR_USART1SMEN))

#define __TIM21_CLK_SLEEP_DISABLE()    (RCC->APB2SMENR &= ~  (RCC_APB2SMENR_TIM21SMEN))
#define __TIM22_CLK_SLEEP_DISABLE()    (RCC->APB2SMENR &= ~  (RCC_APB2SMENR_TIM22SMEN))
#define __ADC1_CLK_SLEEP_DISABLE()     (RCC->APB2SMENR &= ~  (RCC_APB2SMENR_ADC1SMEN))
#define __SPI1_CLK_SLEEP_DISABLE()     (RCC->APB2SMENR &= ~  (RCC_APB2SMENR_SPI1SMEN))
#define __USART1_CLK_SLEEP_DISABLE()   (RCC->APB2SMENR &= ~  (RCC_APB2SMENR_USART1SMEN))
#endif /* STM32L051xx  || STM32L061xx  || */
       /* STM32L052xx  || STM32L062xx  || */
       /* STM32L053xx  || STM32L063xx  || */

/** @brief macro to configure the I2C1 clock (I2C1CLK).
  *
  * @param  __I2C1CLKSource__: specifies the I2C1 clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_I2C1CLKSOURCE_PCLK1: PCLK1 selected as I2C1 clock  
  *            @arg RCC_I2C1CLKSOURCE_HSI: HSI selected as I2C1 clock
  *            @arg RCC_I2C1CLKSOURCE_SYSCLK: System Clock selected as I2C1 clock 
  */
#define __HAL_RCC_I2C1_CONFIG(__I2C1CLKSource__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_I2C1SEL, (uint32_t)(__I2C1CLKSource__))

/** @brief  macro to get the I2C1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_I2C1CLKSOURCE_PCLK1: PCLK1 selected as I2C1 clock  
  *            @arg RCC_I2C1CLKSOURCE_HSI: HSI selected as I2C1 clock
  *            @arg RCC_I2C1CLKSOURCE_SYSCLK: System Clock selected as I2C1 clock
  */
#define __HAL_RCC_GET_I2C1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_I2C1SEL)))

/** @brief macro to configure the USART1 clock (USART1CLK).
  *
  * @param  __USART1CLKSource__: specifies the USART1 clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_USART1CLKSOURCE_PCLK2: PCLK2 selected as USART1 clock
  *            @arg RCC_USART1CLKSOURCE_HSI: HSI selected as USART1 clock
  *            @arg RCC_USART1CLKSOURCE_SYSCLK: System Clock selected as USART1 clock
  *            @arg RCC_USART1CLKSOURCE_LSE: LSE selected as USART1 clock
  */
#define __HAL_RCC_USART1_CONFIG(__USART1CLKSource__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_USART1SEL, (uint32_t)(__USART1CLKSource__))

/** @brief  macro to get the USART1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_USART1CLKSOURCE_PCLK2: PCLK2 selected as USART1 clock
  *            @arg RCC_USART1CLKSOURCE_HSI: HSI selected as USART1 clock
  *            @arg RCC_USART1CLKSOURCE_SYSCLK: System Clock selected as USART1 clock
  *            @arg RCC_USART1CLKSOURCE_LSE: LSE selected as USART1 clock
  */
#define __HAL_RCC_GET_USART1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_USART1SEL)))

/** @brief macro to configure the USART2 clock (USART2CLK).
  *
  * @param  __USART2CLKSource__: specifies the USART2 clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_USART2CLKSOURCE_PCLK1: PCLK1 selected as USART2 clock
  *            @arg RCC_USART2CLKSOURCE_HSI: HSI selected as USART2 clock
  *            @arg RCC_USART2CLKSOURCE_SYSCLK: System Clock selected as USART2 clock
  *            @arg RCC_USART2CLKSOURCE_LSE: LSE selected as USART2 clock
  */
#define __HAL_RCC_USART2_CONFIG(__USART2CLKSource__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_USART2SEL, (uint32_t)(__USART2CLKSource__))

/** @brief  macro to get the USART2 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_USART2CLKSOURCE_PCLK1: PCLK1 selected as USART2 clock
  *            @arg RCC_USART2CLKSOURCE_HSI: HSI selected as USART2 clock
  *            @arg RCC_USART2CLKSOURCE_SYSCLK: System Clock selected as USART2 clock
  *            @arg RCC_USART2CLKSOURCE_LSE: LSE selected as USART2 clock
  */
#define __HAL_RCC_GET_USART2_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_USART2SEL)))

/** @brief macro to configure the LPUART1 clock (LPUART1CLK).
  *
  * @param  __LPUART1CLKSource__: specifies the LPUART1 clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_LPUART1CLKSOURCE_PCLK1: PCLK1 selected as LPUART1 clock
  *            @arg RCC_LPUART1CLKSOURCE_HSI: HSI selected as LPUART1 clock
  *            @arg RCC_LPUART1CLKSOURCE_SYSCLK: System Clock selected as LPUART1 clock
  *            @arg RCC_LPUART1CLKSOURCE_LSE: LSE selected as LPUART1 clock
  */
#define __HAL_RCC_LPUART1_CONFIG(__LPUART1CLKSource__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_LPUART1SEL, (uint32_t)(__LPUART1CLKSource__))

/** @brief  macro to get the LPUART1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_LPUART1CLKSOURCE_PCLK1: PCLK1 selected as LPUART1 clock
  *            @arg RCC_LPUART1CLKSOURCE_HSI: HSI selected as LPUART1 clock
  *            @arg RCC_LPUART1CLKSOURCE_SYSCLK: System Clock selected as LPUART1 clock
  *            @arg RCC_LPUART1CLKSOURCE_LSE: LSE selected as LPUART1 clock
  */
#define __HAL_RCC_GET_LPUART1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_LPUART1SEL)))

/** @brief macro to configure the LPTIM1 clock (LPTIM1CLK).
  *
  * @param  __LPTIM1CLKSource__: specifies the LPTIM1 clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_LPTIM1CLKSOURCE_PCLK: PCLK selected as LPTIM1 clock
  *            @arg RCC_LPTIM1CLKSOURCE_LSI : HSI  selected as LPTIM1 clock
  *            @arg RCC_LPTIM1CLKSOURCE_HSI : LSI  selected as LPTIM1 clock
  *            @arg RCC_LPTIM1CLKSOURCE_LSE : LSE  selected as LPTIM1 clock
  */
#define __HAL_RCC_LPTIM1_CONFIG(__LPTIM1CLKSource__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_LPTIM1SEL, (uint32_t)(__LPTIM1CLKSource__))

/** @brief  macro to get the LPTIM1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_LPTIM1CLKSOURCE_PCLK: PCLK selected as LPUART1 clock
  *            @arg RCC_LPTIM1CLKSOURCE_LSI : HSI selected as LPUART1 clock
  *            @arg RCC_LPTIM1CLKSOURCE_HSI : System Clock selected as LPUART1 clock
  *            @arg RCC_LPTIM1CLKSOURCE_LSE : LSE selected as LPUART1 clock
  */
#define __HAL_RCC_GET_LPTIM1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_LPTIM1SEL)))

#if !defined(STM32L051xx) && !defined(STM32L061xx)
/** @brief  Macro to configure the USB clock (USBCLK).
  * @param  __USBCLKSource__: specifies the USB clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_USBCLKSOURCE_HSI48:  HSI48 selected as USB clock
  *            @arg RCC_USBCLKSOURCE_PLLCLK: PLL Clock selected as USB clock
  */
#define __HAL_RCC_USB_CONFIG(__USBCLKSource__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_HSI48SEL, (uint32_t)(__USBCLKSource__))

/** @brief  Macro to get the USB clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_USBCLKSOURCE_HSI48:  HSI48 selected as USB clock
  *            @arg RCC_USBCLKSOURCE_PLLCLK: PLL Clock selected as USB clock
  */
#define __HAL_RCC_GET_USB_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_HSI48SEL)))

/** @brief  Macro to configure the RNG clock (RNGCLK).
  * @param  __RNGCLKSource__: specifies the USB clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_RNGCLKSOURCE_HSI48:  HSI48 selected as RNG clock
  *            @arg RCC_RNGCLKSOURCE_PLLCLK: PLL Clock selected as RNG clock
  */
#define __HAL_RCC_RNG_CONFIG(__RNGCLKSource__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_HSI48SEL, (uint32_t)(__RNGCLKSource__))

/** @brief  Macro to get the RNG clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_RNGCLKSOURCE_HSI48:  HSI48 selected as RNG clock
  *            @arg RCC_RNGCLKSOURCE_PLLCLK: PLL Clock selected as RNG clock
  */
#define __HAL_RCC_GET_RNG_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_HSI48SEL)))

/** @brief macro to select the HSI48M clock source 
  * @note   This macro can be replaced by either __HAL_RCC_RNG_CONFIG or
  *         __HAL_RCC_USB_CONFIG to configure respectively RNG or UBS clock sources.
  *
  * @param  __HSI48MCLKSource__: specifies the HSI48M clock source dedicated for 
  *          USB an RNG peripherals.                 
  *          This parameter can be one of the following values:
  *            @arg RCC_HSI48M_PLL: A dedicated 48MHZ PLL output.
  *            @arg RCC_HSI48M_RC48: 48MHZ issued from internal HSI48 oscillator. 
  */
#define __HAL_RCC_HSI48M_CONFIG(__HSI48MCLKSource__) \
                  MODIFY_REG(RCC->CCIPR, RCC_CCIPR_HSI48SEL, (uint32_t)(__HSI48MCLKSource__))                    

/** @brief  macro to get the HSI48M clock source.
  * @note   This macro can be replaced by either __HAL_RCC_GET_RNG_SOURCE or
  *         __HAL_RCC_GET_USB_SOURCE to get respectively RNG or UBS clock sources.
  * @retval The clock source can be one of the following values:
  *           @arg RCC_HSI48M_PLL: A dedicated 48MHZ PLL output.
  *            @arg RCC_HSI48M_RC48: 48MHZ issued from internal HSI48 oscillator. 
  */
#define __HAL_RCC_GET_HSI48M_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR, RCC_CCIPR_HSI48SEL)))       
#endif /* !(STM32L051xx ) && !(STM32L061xx ) */

/**
  * @brief    Macros to enable or disable the force of the Internal High Speed oscillator (HSI)
  *           in STOP mode to be quickly available as kernel clock for USART and I2C.
  * @note     The Enable of this function has not effect on the HSION bit.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
#define __HAL_RCC_HSISTOP_ENABLE()  SET_BIT(RCC->CR, RCC_CR_HSIKERON)
#define __HAL_RCC_HSISTOP_DISABLE() CLEAR_BIT(RCC->CR, RCC_CR_HSIKERON)                   

/**
  * @brief  Macro to configures the External Low Speed oscillator (LSE) drive capability.
  * @param  RCC_LSEDrive: specifies the new state of the LSE drive capability.
  *          This parameter can be one of the following values:
  *            @arg RCC_LSEDRIVE_LOW: LSE oscillator low drive capability.
  *            @arg RCC_LSEDRIVE_MEDIUMLOW: LSE oscillator medium low drive capability.
  *            @arg RCC_LSEDRIVE_MEDIUMHIGH: LSE oscillator medium high drive capability.
  *            @arg RCC_LSEDRIVE_HIGH: LSE oscillator high drive capability.
  * @retval None
  */ 
#define __HAL_RCC_LSEDRIVE_CONFIG(__RCC_LSEDrive__) (MODIFY_REG(RCC->CSR,\
        RCC_CSR_LSEDRV, (uint32_t)(__RCC_LSEDrive__) ))

/**
  * @brief  Macro to configures the wake up from stop clock.
  * @param  RCC_STOPWUCLK: specifies the clock source used after wake up from stop 
  *   This parameter can be one of the following values:
  *     @arg RCC_StopWakeUpClock_MSI:    MSI selected as system clock source
  *     @arg RCC_StopWakeUpClock_HSI:    HSI selected as system clock source
  * @retval None
  */
#define __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(__RCC_STOPWUCLK__) (MODIFY_REG(RCC->CFGR,\
        RCC_CFGR_STOPWUCK, (uint32_t)(__RCC_STOPWUCLK__) ))
 
#if !defined(STM32L051xx) && !defined(STM32L061xx)
/**
  * @brief  Enables the specified CRS interrupts.
  * @param  __INTERRUPT__: specifies the CRS interrupt sources to be enabled.
  *          This parameter can be any combination of the following values:
  *              @arg RCC_CRS_IT_SYNCOK
  *              @arg RCC_CRS_IT_SYNCWARN
  *              @arg RCC_CRS_IT_ERR
  *              @arg RCC_CRS_IT_ESYNC
  * @retval None
  */
#define __HAL_RCC_CRS_ENABLE_IT(__INTERRUPT__)   (CRS->CR |= (__INTERRUPT__))

/**
  * @brief  Disables the specified CRS interrupts.
  * @param  __INTERRUPT__: specifies the CRS interrupt sources to be disabled.
  *          This parameter can be any combination of the following values:
  *              @arg RCC_CRS_IT_SYNCOK
  *              @arg RCC_CRS_IT_SYNCWARN
  *              @arg RCC_CRS_IT_ERR
  *              @arg RCC_CRS_IT_ESYNC
  * @retval None
  */
#define __HAL_RCC_CRS_DISABLE_IT(__INTERRUPT__)  (CRS->CR &= ~(__INTERRUPT__))

/** @brief  Check the CRS's interrupt has occurred or not.
  * @param  __INTERRUPT__: specifies the CRS interrupt source to check.
  *         This parameter can be one of the following values:
  *              @arg RCC_CRS_IT_SYNCOK
  *              @arg RCC_CRS_IT_SYNCWARN
  *              @arg RCC_CRS_IT_ERR
  *              @arg RCC_CRS_IT_ESYNC
  * @retval The new state of __INTERRUPT__ (SET or RESET).
  */
#define __HAL_RCC_CRS_GET_IT_SOURCE(__INTERRUPT__)     ((CRS->CR & (__INTERRUPT__))? SET : RESET)

/** @brief  Clear the CRS's interrupt pending bits
  *         bits to clear the selected interrupt pending bits.
  * @param  __INTERRUPT__: specifies the interrupt pending bit to clear.
  *         This parameter can be any combination of the following values:
  *              @arg RCC_CRS_IT_SYNCOK
  *              @arg RCC_CRS_IT_SYNCWARN
  *              @arg RCC_CRS_IT_ERR
  *              @arg RCC_CRS_IT_ESYNC
  *              @arg RCC_CRS_IT_TRIMOVF
  *              @arg RCC_CRS_IT_SYNCERR
  *              @arg RCC_CRS_IT_SYNCMISS
  */
/* CRS IT Error Mask */
#define  RCC_CRS_IT_ERROR_MASK                 ((uint32_t)(RCC_CRS_IT_TRIMOVF | RCC_CRS_IT_SYNCERR | RCC_CRS_IT_SYNCMISS))

#define __HAL_RCC_CRS_CLEAR_IT(__INTERRUPT__)   ((((__INTERRUPT__) &  RCC_CRS_IT_ERROR_MASK)!= 0) ? (CRS->ICR |= CRS_ICR_ERRC) : \
                                            (CRS->ICR = (__INTERRUPT__)))

/**
  * @brief  Checks whether the specified CRS flag is set or not.
  * @param  _FLAG_: specifies the flag to check.
  *          This parameter can be one of the following values:
  *              @arg RCC_CRS_FLAG_SYNCOK
  *              @arg RCC_CRS_FLAG_SYNCWARN
  *              @arg RCC_CRS_FLAG_ERR
  *              @arg RCC_CRS_FLAG_ESYNC
  *              @arg RCC_CRS_FLAG_TRIMOVF
  *              @arg RCC_CRS_FLAG_SYNCERR
  *              @arg RCC_CRS_FLAG_SYNCMISS
  * @retval The new state of _FLAG_ (TRUE or FALSE).
  */
#define __HAL_RCC_CRS_GET_FLAG(_FLAG_)  ((CRS->ISR & (_FLAG_)) == (_FLAG_))

/**
  * @brief  Clears the CRS specified FLAG.
  * @param _FLAG_: specifies the flag to clear.
  *          This parameter can be one of the following values:
  *              @arg RCC_CRS_FLAG_SYNCOK
  *              @arg RCC_CRS_FLAG_SYNCWARN
  *              @arg RCC_CRS_FLAG_ERR
  *              @arg RCC_CRS_FLAG_ESYNC
  *              @arg RCC_CRS_FLAG_TRIMOVF
  *              @arg RCC_CRS_FLAG_SYNCERR
  *              @arg RCC_CRS_FLAG_SYNCMISS
  * @retval None
  */

/* CRS Flag Error Mask */
#define RCC_CRS_FLAG_ERROR_MASK                 ((uint32_t)(RCC_CRS_FLAG_TRIMOVF | RCC_CRS_FLAG_SYNCERR | RCC_CRS_FLAG_SYNCMISS))

#define __HAL_RCC_CRS_CLEAR_FLAG(__FLAG__)   ((((__FLAG__) & RCC_CRS_FLAG_ERROR_MASK)!= 0) ? (CRS->ICR |= CRS_ICR_ERRC) : \
                                            (CRS->ICR = (__FLAG__)))


/**
  * @brief  Enables the oscillator clock for frequency error counter.
  * @note   when the CEN bit is set the CRS_CFGR register becomes write-protected.
  * @param  None
  * @retval None
  */
#define __HAL_RCC_CRS_ENABLE_FREQ_ERROR_COUNTER() (CRS->CR |= CRS_CR_CEN)

/**
  * @brief  Disables the oscillator clock for frequency error counter.
  * @param  None
  * @retval None
  */
#define __HAL_RCC_CRS_DISABLE_FREQ_ERROR_COUNTER()  (CRS->CR &= ~CRS_CR_CEN)

/**
  * @brief  Enables the automatic hardware adjustment of TRIM bits.
  * @note   When the AUTOTRIMEN bit is set the CRS_CFGR register becomes write-protected.
  * @param  None
  * @retval None
  */
#define __HAL_RCC_CRS_ENABLE_AUTOMATIC_CALIB()  (CRS->CR |= CRS_CR_AUTOTRIMEN)

/**
  * @brief  Enables or disables the automatic hardware adjustment of TRIM bits.
  * @param  None
  * @retval None
  */
#define __HAL_RCC_CRS_DISABLE_AUTOMATIC_CALIB()  (CRS->CR &= ~CRS_CR_AUTOTRIMEN)

/**
  * @brief  Macro to calculate reload value to be set in CRS register according to target and sync frequencies
  * @note   The RELOAD value should be selected according to the ratio between the target frequency and the frequency 
  *             of the synchronization source after prescaling. It is then decreased by one in order to 
  *             reach the expected synchronization on the zero value. The formula is the following:
  *             RELOAD = (fTARGET / fSYNC) -1
  * @param  _FTARGET_ Target frequency (value in Hz)
  * @param  _FSYNC_ Synchronization signal frequency (value in Hz)
  * @retval None
  */
#define __HAL_RCC_CRS_CALCULATE_RELOADVALUE(_FTARGET_, _FSYNC_)  (((_FTARGET_) / (_FSYNC_)) - 1)

#endif /* !(STM32L051xx) && !(STM32L061xx) */

/**
  * @}
  */

        
/* Exported functions --------------------------------------------------------*/
HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit);
void HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit);
void HAL_RCCEx_EnableLSECSS(void);
void HAL_RCCEx_DisableLSECSS(void);
#if !defined(STM32L051xx) && !defined(STM32L061xx)
void                  HAL_RCCEx_CRSConfig(RCC_CRSInitTypeDef *pInit);
void                  HAL_RCCEx_CRSSoftwareSynchronizationGenerate(void);
void                  HAL_RCCEx_CRSGetSynchronizationInfo(RCC_CRSSynchroInfoTypeDef *pSynchroInfo);
RCC_CRSStatusTypeDef  HAL_RCCEx_CRSWaitSynchronization(uint32_t Timeout);
#endif /* !(STM32L051xx) && !(STM32L061xx) */
       
/**
  * @}
  */ 

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32L0xx_HAL_RCC_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
