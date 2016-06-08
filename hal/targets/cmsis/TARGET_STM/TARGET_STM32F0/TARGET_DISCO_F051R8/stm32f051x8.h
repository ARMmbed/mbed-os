/**
  ******************************************************************************
  * @file    stm32f051x8.h
  * @author  MCD Application Team
  * @version V2.2.3
  * @date    29-January-2016
  * @brief   CMSIS Cortex-M0 Device Peripheral Access Layer Header File. 
  *          This file contains all the peripheral register's definitions, bits 
  *          definitions and memory mapping for STM32F0xx devices.            
  *            
  *          This file contains:
  *           - Data structures and the address mapping for all peripherals
  *           - Peripheral's registers declarations and bits definition
  *           - Macros to access peripheral’s registers hardware
  *  
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
                                                           
/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32f051x8
  * @{
  */
    
#ifndef __STM32F051x8_H
#define __STM32F051x8_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

  /** @addtogroup Configuration_section_for_CMSIS
  * @{
  */
/**
 * @brief Configuration of the Cortex-M0 Processor and Core Peripherals
 */
#define __CM0_REV                 0 /*!< Core Revision r0p0                            */
#define __MPU_PRESENT             0 /*!< STM32F0xx do not provide MPU                  */
#define __NVIC_PRIO_BITS          2 /*!< STM32F0xx uses 2 Bits for the Priority Levels */
#define __Vendor_SysTickConfig    0     /*!< Set to 1 if different SysTick Config is used */
 
/**
  * @}
  */
   
/** @addtogroup Peripheral_interrupt_number_definition
  * @{
  */

/**
 * @brief STM32F0xx Interrupt Number Definition, according to the selected device 
 *        in @ref Library_configuration_section 
 */

 /*!< Interrupt Number Definition */
typedef enum
{
/******  Cortex-M0 Processor Exceptions Numbers **************************************************************/
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                                        */
  HardFault_IRQn              = -13,    /*!< 3 Cortex-M0 Hard Fault Interrupt                                */
  SVC_IRQn                    = -5,     /*!< 11 Cortex-M0 SV Call Interrupt                                  */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M0 Pend SV Interrupt                                  */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M0 System Tick Interrupt                              */

/******  STM32F0 specific Interrupt Numbers ******************************************************************/
  WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                               */
  PVD_IRQn                    = 1,      /*!< PVD Interrupt through EXTI Lines 16                             */
  RTC_IRQn                    = 2,      /*!< RTC Interrupt through EXTI Lines 17, 19 and 20                  */
  FLASH_IRQn                  = 3,      /*!< FLASH global Interrupt                                          */
  RCC_IRQn                    = 4,      /*!< RCC global Interrupt                                            */
  EXTI0_1_IRQn                = 5,      /*!< EXTI Line 0 and 1 Interrupt                                     */
  EXTI2_3_IRQn                = 6,      /*!< EXTI Line 2 and 3 Interrupt                                     */
  EXTI4_15_IRQn               = 7,      /*!< EXTI Line 4 to 15 Interrupt                                     */
  TSC_IRQn                    = 8,      /*!< Touch Sensing Controller Interrupts                             */
  DMA1_Channel1_IRQn          = 9,      /*!< DMA1 Channel 1 Interrupt                                        */
  DMA1_Channel2_3_IRQn        = 10,     /*!< DMA1 Channel 2 and Channel 3 Interrupt                          */
  DMA1_Channel4_5_IRQn        = 11,     /*!< DMA1 Channel 4 and Channel 5 Interrupt                          */
  ADC1_COMP_IRQn              = 12,     /*!< ADC1 and COMP interrupts (ADC interrupt combined with EXTI Lines 21 and 22 */
  TIM1_BRK_UP_TRG_COM_IRQn    = 13,     /*!< TIM1 Break, Update, Trigger and Commutation Interrupt           */
  TIM1_CC_IRQn                = 14,     /*!< TIM1 Capture Compare Interrupt                                  */
  TIM2_IRQn                   = 15,     /*!< TIM2 global Interrupt                                           */
  TIM3_IRQn                   = 16,     /*!< TIM3 global Interrupt                                           */
  TIM6_DAC_IRQn               = 17,     /*!< TIM6 global and DAC channel underrun error Interrupt            */
  TIM14_IRQn                  = 19,     /*!< TIM14 global Interrupt                                          */
  TIM15_IRQn                  = 20,     /*!< TIM15 global Interrupt                                          */
  TIM16_IRQn                  = 21,     /*!< TIM16 global Interrupt                                          */
  TIM17_IRQn                  = 22,     /*!< TIM17 global Interrupt                                          */
  I2C1_IRQn                   = 23,     /*!< I2C1 Event Interrupt & EXTI Line23 Interrupt (I2C1 wakeup)      */
  I2C2_IRQn                   = 24,     /*!< I2C2 Event Interrupt                                            */
  SPI1_IRQn                   = 25,     /*!< SPI1 global Interrupt                                           */
  SPI2_IRQn                   = 26,     /*!< SPI2 global Interrupt                                           */
  USART1_IRQn                 = 27,     /*!< USART1 global Interrupt & EXTI Line25 Interrupt (USART1 wakeup) */
  USART2_IRQn                 = 28,     /*!< USART2 global Interrupt                                         */
  CEC_CAN_IRQn                = 30      /*!< CEC and CAN global Interrupts & EXTI Line27 Interrupt           */
} IRQn_Type;

/**
  * @}
  */

#include "core_cm0.h"            /* Cortex-M0 processor and core peripherals */
#include "system_stm32f0xx.h"    /* STM32F0xx System Header */
#include <stdint.h>

/** @addtogroup Peripheral_registers_structures
  * @{
  */

/**
  * @brief Analog to Digital Converter
  */

typedef struct
{
  __IO uint32_t ISR;          /*!< ADC interrupt and status register,             Address offset: 0x00 */
  __IO uint32_t IER;          /*!< ADC interrupt enable register,                 Address offset: 0x04 */
  __IO uint32_t CR;           /*!< ADC control register,                          Address offset: 0x08 */
  __IO uint32_t CFGR1;        /*!< ADC configuration register 1,                  Address offset: 0x0C */
  __IO uint32_t CFGR2;        /*!< ADC configuration register 2,                  Address offset: 0x10 */
  __IO uint32_t SMPR;         /*!< ADC sampling time register,                    Address offset: 0x14 */
       uint32_t RESERVED1;    /*!< Reserved,                                                      0x18 */
       uint32_t RESERVED2;    /*!< Reserved,                                                      0x1C */
  __IO uint32_t TR;           /*!< ADC analog watchdog 1 threshold register,      Address offset: 0x20 */
       uint32_t RESERVED3;    /*!< Reserved,                                                      0x24 */
  __IO uint32_t CHSELR;       /*!< ADC group regular sequencer register,          Address offset: 0x28 */
       uint32_t RESERVED4[5]; /*!< Reserved,                                                      0x2C */
  __IO uint32_t DR;           /*!< ADC group regular data register,               Address offset: 0x40 */
} ADC_TypeDef;

typedef struct
{
  __IO uint32_t CCR;          /*!< ADC common configuration register,             Address offset: ADC1 base address + 0x308 */
} ADC_Common_TypeDef;

/**
  * @brief HDMI-CEC
  */

typedef struct
{
  __IO uint32_t CR;           /*!< CEC control register,                                       Address offset:0x00 */
  __IO uint32_t CFGR;         /*!< CEC configuration register,                                 Address offset:0x04 */
  __IO uint32_t TXDR;         /*!< CEC Tx data register ,                                      Address offset:0x08 */
  __IO uint32_t RXDR;         /*!< CEC Rx Data Register,                                       Address offset:0x0C */
  __IO uint32_t ISR;          /*!< CEC Interrupt and Status Register,                          Address offset:0x10 */
  __IO uint32_t IER;          /*!< CEC interrupt enable register,                              Address offset:0x14 */
}CEC_TypeDef;

/** 
  * @brief Comparator 
  */

typedef struct
{
  __IO uint16_t CSR;         /*!< COMP control and status register,                                                 Address offset: 0x00 */
} COMP_TypeDef;

typedef struct
{
  __IO uint32_t CSR;         /*!< COMP control and status register, used for bits common to several COMP instances, Address offset: 0x00 */
} COMP_Common_TypeDef;

/* Legacy defines */
typedef struct
{
  __IO uint32_t CSR;         /*!< Kept for legacy purpose. Use structure 'COMP_Common_TypeDef'. */
}COMP1_2_TypeDef;

/** 
  * @brief CRC calculation unit
  */

typedef struct
{
  __IO uint32_t DR;          /*!< CRC Data register,                           Address offset: 0x00 */
  __IO uint8_t  IDR;         /*!< CRC Independent data register,               Address offset: 0x04 */
  uint8_t       RESERVED0;   /*!< Reserved,                                                    0x05 */
  uint16_t      RESERVED1;   /*!< Reserved,                                                    0x06 */
  __IO uint32_t CR;          /*!< CRC Control register,                        Address offset: 0x08 */ 
  uint32_t      RESERVED2;   /*!< Reserved,                                                    0x0C */
  __IO uint32_t INIT;        /*!< Initial CRC value register,                  Address offset: 0x10 */
  __IO uint32_t RESERVED3;   /*!< Reserved,                                                    0x14 */
} CRC_TypeDef;

/** 
  * @brief Digital to Analog Converter
  */

typedef struct
{
  __IO uint32_t CR;           /*!< DAC control register,                                     Address offset: 0x00 */
  __IO uint32_t SWTRIGR;      /*!< DAC software trigger register,                            Address offset: 0x04 */
  __IO uint32_t DHR12R1;      /*!< DAC channel1 12-bit right-aligned data holding register,  Address offset: 0x08 */
  __IO uint32_t DHR12L1;      /*!< DAC channel1 12-bit left aligned data holding register,   Address offset: 0x0C */
  __IO uint32_t DHR8R1;       /*!< DAC channel1 8-bit right aligned data holding register,   Address offset: 0x10 */
  uint32_t      RESERVED1[6]; /*!< Reserved,                                                 Address offset: 0x14 to 0x28  */
  __IO uint32_t DOR1;         /*!< DAC channel1 data output register,                        Address offset: 0x2C */
  uint32_t      RESERVED2;    /*!< Reserved,                                                 Address offset: 0x30 */
  __IO uint32_t SR;           /*!< DAC status register,                                      Address offset: 0x34 */
} DAC_TypeDef;

/** 
  * @brief Debug MCU
  */

typedef struct
{
  __IO uint32_t IDCODE;       /*!< MCU device ID code,                          Address offset: 0x00 */
  __IO uint32_t CR;           /*!< Debug MCU configuration register,            Address offset: 0x04 */
  __IO uint32_t APB1FZ;       /*!< Debug MCU APB1 freeze register,              Address offset: 0x08 */
  __IO uint32_t APB2FZ;       /*!< Debug MCU APB2 freeze register,              Address offset: 0x0C */
}DBGMCU_TypeDef;

/** 
  * @brief DMA Controller
  */

typedef struct
{
  __IO uint32_t CCR;          /*!< DMA channel x configuration register        */
  __IO uint32_t CNDTR;        /*!< DMA channel x number of data register       */
  __IO uint32_t CPAR;         /*!< DMA channel x peripheral address register   */
  __IO uint32_t CMAR;         /*!< DMA channel x memory address register       */
} DMA_Channel_TypeDef;

typedef struct
{
  __IO uint32_t ISR;          /*!< DMA interrupt status register,               Address offset: 0x00 */
  __IO uint32_t IFCR;         /*!< DMA interrupt flag clear register,           Address offset: 0x04 */
} DMA_TypeDef;

/** 
  * @brief External Interrupt/Event Controller
  */

typedef struct
{
  __IO uint32_t IMR;          /*!<EXTI Interrupt mask register,                 Address offset: 0x00 */
  __IO uint32_t EMR;          /*!<EXTI Event mask register,                     Address offset: 0x04 */
  __IO uint32_t RTSR;         /*!<EXTI Rising trigger selection register ,      Address offset: 0x08 */
  __IO uint32_t FTSR;         /*!<EXTI Falling trigger selection register,      Address offset: 0x0C */
  __IO uint32_t SWIER;        /*!<EXTI Software interrupt event register,       Address offset: 0x10 */
  __IO uint32_t PR;           /*!<EXTI Pending register,                        Address offset: 0x14 */
} EXTI_TypeDef;

/** 
  * @brief FLASH Registers
  */
typedef struct
{
  __IO uint32_t ACR;          /*!<FLASH access control register,                 Address offset: 0x00 */
  __IO uint32_t KEYR;         /*!<FLASH key register,                            Address offset: 0x04 */
  __IO uint32_t OPTKEYR;      /*!<FLASH OPT key register,                        Address offset: 0x08 */
  __IO uint32_t SR;           /*!<FLASH status register,                         Address offset: 0x0C */
  __IO uint32_t CR;           /*!<FLASH control register,                        Address offset: 0x10 */
  __IO uint32_t AR;           /*!<FLASH address register,                        Address offset: 0x14 */
  __IO uint32_t RESERVED;     /*!< Reserved,                                                     0x18 */
  __IO uint32_t OBR;          /*!<FLASH option bytes register,                   Address offset: 0x1C */
  __IO uint32_t WRPR;         /*!<FLASH option bytes register,                   Address offset: 0x20 */
} FLASH_TypeDef;

/** 
  * @brief Option Bytes Registers
  */
typedef struct
{
  __IO uint16_t RDP;          /*!< FLASH option byte Read protection,             Address offset: 0x00 */
  __IO uint16_t USER;         /*!< FLASH option byte user options,                Address offset: 0x02 */
  __IO uint16_t DATA0;        /*!< User data byte 0 (stored in FLASH_OBR[23:16]), Address offset: 0x04 */
  __IO uint16_t DATA1;        /*!< User data byte 1 (stored in FLASH_OBR[31:24]), Address offset: 0x06 */
  __IO uint16_t WRP0;         /*!< FLASH option byte write protection 0,          Address offset: 0x08 */
  __IO uint16_t WRP1;         /*!< FLASH option byte write protection 1,          Address offset: 0x0A */
} OB_TypeDef;

/** 
  * @brief General Purpose I/O
  */

typedef struct
{
  __IO uint32_t MODER;        /*!< GPIO port mode register,                     Address offset: 0x00      */
  __IO uint32_t OTYPER;       /*!< GPIO port output type register,              Address offset: 0x04      */
  __IO uint32_t OSPEEDR;      /*!< GPIO port output speed register,             Address offset: 0x08      */
  __IO uint32_t PUPDR;        /*!< GPIO port pull-up/pull-down register,        Address offset: 0x0C      */
  __IO uint32_t IDR;          /*!< GPIO port input data register,               Address offset: 0x10      */
  __IO uint32_t ODR;          /*!< GPIO port output data register,              Address offset: 0x14      */
  __IO uint32_t BSRR;         /*!< GPIO port bit set/reset register,      Address offset: 0x1A */
  __IO uint32_t LCKR;         /*!< GPIO port configuration lock register,       Address offset: 0x1C      */
  __IO uint32_t AFR[2];       /*!< GPIO alternate function low register,  Address offset: 0x20-0x24 */
  __IO uint32_t BRR;          /*!< GPIO bit reset register,                     Address offset: 0x28      */
} GPIO_TypeDef;

/** 
  * @brief SysTem Configuration
  */

typedef struct
{
  __IO uint32_t CFGR1;       /*!< SYSCFG configuration register 1,                           Address offset: 0x00 */
       uint32_t RESERVED;    /*!< Reserved,                                                                  0x04 */
  __IO uint32_t EXTICR[4];   /*!< SYSCFG external interrupt configuration register,     Address offset: 0x14-0x08 */
  __IO uint32_t CFGR2;       /*!< SYSCFG configuration register 2,                           Address offset: 0x18 */
} SYSCFG_TypeDef;

/** 
  * @brief Inter-integrated Circuit Interface
  */

typedef struct
{
  __IO uint32_t CR1;          /*!< I2C Control register 1,                      Address offset: 0x00 */
  __IO uint32_t CR2;          /*!< I2C Control register 2,                      Address offset: 0x04 */
  __IO uint32_t OAR1;     /*!< I2C Own address 1 register,        Address offset: 0x08 */
  __IO uint32_t OAR2;     /*!< I2C Own address 2 register,        Address offset: 0x0C */
  __IO uint32_t TIMINGR;  /*!< I2C Timing register,               Address offset: 0x10 */
  __IO uint32_t TIMEOUTR; /*!< I2C Timeout register,              Address offset: 0x14 */
  __IO uint32_t ISR;      /*!< I2C Interrupt and status register, Address offset: 0x18 */
  __IO uint32_t ICR;      /*!< I2C Interrupt clear register,      Address offset: 0x1C */
  __IO uint32_t PECR;     /*!< I2C PEC register,                  Address offset: 0x20 */
  __IO uint32_t RXDR;     /*!< I2C Receive data register,         Address offset: 0x24 */
  __IO uint32_t TXDR;     /*!< I2C Transmit data register,        Address offset: 0x28 */
} I2C_TypeDef;

/** 
  * @brief Independent WATCHDOG
  */

typedef struct
{
  __IO uint32_t KR;   /*!< IWDG Key register,       Address offset: 0x00 */
  __IO uint32_t PR;   /*!< IWDG Prescaler register, Address offset: 0x04 */
  __IO uint32_t RLR;  /*!< IWDG Reload register,    Address offset: 0x08 */
  __IO uint32_t SR;   /*!< IWDG Status register,    Address offset: 0x0C */
  __IO uint32_t WINR; /*!< IWDG Window register,    Address offset: 0x10 */
} IWDG_TypeDef;

/** 
  * @brief Power Control
  */

typedef struct
{
  __IO uint32_t CR;   /*!< PWR power control register,                          Address offset: 0x00 */
  __IO uint32_t CSR;  /*!< PWR power control/status register,                   Address offset: 0x04 */
} PWR_TypeDef;

/** 
  * @brief Reset and Clock Control
  */

typedef struct
{
  __IO uint32_t CR;            /*!< RCC clock control register,                                   Address offset: 0x00 */
  __IO uint32_t CFGR;       /*!< RCC clock configuration register,                            Address offset: 0x04 */
  __IO uint32_t CIR;        /*!< RCC clock interrupt register,                                Address offset: 0x08 */
  __IO uint32_t APB2RSTR;   /*!< RCC APB2 peripheral reset register,                          Address offset: 0x0C */
  __IO uint32_t APB1RSTR;   /*!< RCC APB1 peripheral reset register,                          Address offset: 0x10 */
  __IO uint32_t AHBENR;     /*!< RCC AHB peripheral clock register,                           Address offset: 0x14 */
  __IO uint32_t APB2ENR;    /*!< RCC APB2 peripheral clock enable register,                   Address offset: 0x18 */
  __IO uint32_t APB1ENR;    /*!< RCC APB1 peripheral clock enable register,                   Address offset: 0x1C */
  __IO uint32_t BDCR;       /*!< RCC Backup domain control register,                          Address offset: 0x20 */
  __IO uint32_t CSR;        /*!< RCC clock control & status register,                         Address offset: 0x24 */
  __IO uint32_t AHBRSTR;    /*!< RCC AHB peripheral reset register,                           Address offset: 0x28 */
  __IO uint32_t CFGR2;      /*!< RCC clock configuration register 2,                          Address offset: 0x2C */
  __IO uint32_t CFGR3;      /*!< RCC clock configuration register 3,                          Address offset: 0x30 */
  __IO uint32_t CR2;        /*!< RCC clock control register 2,                                Address offset: 0x34 */
} RCC_TypeDef;

/** 
  * @brief Real-Time Clock
  */
typedef struct
{
  __IO uint32_t TR;         /*!< RTC time register,                                         Address offset: 0x00 */
  __IO uint32_t DR;         /*!< RTC date register,                                         Address offset: 0x04 */
  __IO uint32_t CR;         /*!< RTC control register,                                      Address offset: 0x08 */                                                                                            
  __IO uint32_t ISR;        /*!< RTC initialization and status register,                    Address offset: 0x0C */
  __IO uint32_t PRER;       /*!< RTC prescaler register,                                    Address offset: 0x10 */
       uint32_t RESERVED1;  /*!< Reserved,                                                  Address offset: 0x14 */
       uint32_t RESERVED2;  /*!< Reserved,                                                  Address offset: 0x18 */
  __IO uint32_t ALRMAR;     /*!< RTC alarm A register,                                      Address offset: 0x1C */
       uint32_t RESERVED3;  /*!< Reserved,                                                  Address offset: 0x20 */
  __IO uint32_t WPR;        /*!< RTC write protection register,                             Address offset: 0x24 */
  __IO uint32_t SSR;        /*!< RTC sub second register,                                   Address offset: 0x28 */
  __IO uint32_t SHIFTR;     /*!< RTC shift control register,                                Address offset: 0x2C */
  __IO uint32_t TSTR;       /*!< RTC time stamp time register,                              Address offset: 0x30 */
  __IO uint32_t TSDR;       /*!< RTC time stamp date register,                              Address offset: 0x34 */
  __IO uint32_t TSSSR;      /*!< RTC time-stamp sub second register,                        Address offset: 0x38 */
  __IO uint32_t CALR;       /*!< RTC calibration register,                                  Address offset: 0x3C */
  __IO uint32_t TAFCR;      /*!< RTC tamper and alternate function configuration register,  Address offset: 0x40 */
  __IO uint32_t ALRMASSR;   /*!< RTC alarm A sub second register,                           Address offset: 0x44 */
       uint32_t RESERVED4;  /*!< Reserved,                                                  Address offset: 0x48 */
       uint32_t RESERVED5;  /*!< Reserved,                                                  Address offset: 0x4C */
  __IO uint32_t BKP0R;      /*!< RTC backup register 0,                                     Address offset: 0x50 */
  __IO uint32_t BKP1R;      /*!< RTC backup register 1,                                     Address offset: 0x54 */
  __IO uint32_t BKP2R;      /*!< RTC backup register 2,                                     Address offset: 0x58 */
  __IO uint32_t BKP3R;      /*!< RTC backup register 3,                                     Address offset: 0x5C */
  __IO uint32_t BKP4R;      /*!< RTC backup register 4,                                     Address offset: 0x60 */
} RTC_TypeDef;

/** 
  * @brief Serial Peripheral Interface
  */

typedef struct
{
  __IO uint32_t CR1;        /*!< SPI Control register 1 (not used in I2S mode),      Address offset: 0x00 */
  __IO uint32_t CR2;        /*!< SPI Control register 2,                             Address offset: 0x04 */
  __IO uint32_t SR;         /*!< SPI Status register,                                Address offset: 0x08 */
  __IO uint32_t DR;         /*!< SPI data register,                                  Address offset: 0x0C */
  __IO uint32_t CRCPR;      /*!< SPI CRC polynomial register (not used in I2S mode), Address offset: 0x10 */
  __IO uint32_t RXCRCR;     /*!< SPI Rx CRC register (not used in I2S mode),         Address offset: 0x14 */
  __IO uint32_t TXCRCR;     /*!< SPI Tx CRC register (not used in I2S mode),         Address offset: 0x18 */
  __IO uint32_t I2SCFGR;    /*!< SPI_I2S configuration register,                     Address offset: 0x1C */
  __IO uint32_t I2SPR;      /*!< SPI_I2S prescaler register,                         Address offset: 0x20 */
} SPI_TypeDef;

/** 
  * @brief TIM
  */
typedef struct
{
  __IO uint32_t CR1;          /*!< TIM control register 1,              Address offset: 0x00 */
  __IO uint32_t CR2;          /*!< TIM control register 2,              Address offset: 0x04 */
  __IO uint32_t SMCR;         /*!< TIM slave Mode Control register,     Address offset: 0x08 */
  __IO uint32_t DIER;         /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  __IO uint32_t SR;           /*!< TIM status register,                 Address offset: 0x10 */
  __IO uint32_t EGR;          /*!< TIM event generation register,       Address offset: 0x14 */
  __IO uint32_t CCMR1;        /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  __IO uint32_t CCMR2;        /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  __IO uint32_t CCER;         /*!< TIM capture/compare enable register, Address offset: 0x20 */
  __IO uint32_t CNT;          /*!< TIM counter register,                Address offset: 0x24 */
  __IO uint32_t PSC;          /*!< TIM prescaler register,              Address offset: 0x28 */
  __IO uint32_t ARR;          /*!< TIM auto-reload register,            Address offset: 0x2C */
  __IO uint32_t RCR;             /*!< TIM  repetition counter register,            Address offset: 0x30 */
  __IO uint32_t CCR1;         /*!< TIM capture/compare register 1,      Address offset: 0x34 */    
  __IO uint32_t CCR2;         /*!< TIM capture/compare register 2,      Address offset: 0x38 */    
  __IO uint32_t CCR3;         /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  __IO uint32_t CCR4;         /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  __IO uint32_t BDTR;            /*!< TIM break and dead-time register,            Address offset: 0x44 */
  __IO uint32_t DCR;          /*!< TIM DMA control register,            Address offset: 0x48 */
  __IO uint32_t DMAR;            /*!< TIM DMA address for full transfer register,  Address offset: 0x4C */
  __IO uint32_t OR;           /*!< TIM option register,                 Address offset: 0x50 */
} TIM_TypeDef;

/**
  * @brief Touch Sensing Controller (TSC)
  */
typedef struct
{
  __IO uint32_t CR;        /*!< TSC control register,                                     Address offset: 0x00 */
  __IO uint32_t IER;       /*!< TSC interrupt enable register,                            Address offset: 0x04 */
  __IO uint32_t ICR;       /*!< TSC interrupt clear register,                             Address offset: 0x08 */
  __IO uint32_t ISR;       /*!< TSC interrupt status register,                            Address offset: 0x0C */
  __IO uint32_t IOHCR;     /*!< TSC I/O hysteresis control register,                      Address offset: 0x10 */
       uint32_t RESERVED1; /*!< Reserved,                                                 Address offset: 0x14 */
  __IO uint32_t IOASCR;    /*!< TSC I/O analog switch control register,                   Address offset: 0x18 */
       uint32_t RESERVED2; /*!< Reserved,                                                 Address offset: 0x1C */
  __IO uint32_t IOSCR;     /*!< TSC I/O sampling control register,                        Address offset: 0x20 */
       uint32_t RESERVED3; /*!< Reserved,                                                 Address offset: 0x24 */
  __IO uint32_t IOCCR;     /*!< TSC I/O channel control register,                         Address offset: 0x28 */
       uint32_t RESERVED4; /*!< Reserved,                                                 Address offset: 0x2C */
  __IO uint32_t IOGCSR;    /*!< TSC I/O group control status register,                    Address offset: 0x30 */
  __IO uint32_t IOGXCR[8]; /*!< TSC I/O group x counter register,                         Address offset: 0x34-50 */
}TSC_TypeDef;

/** 
  * @brief Universal Synchronous Asynchronous Receiver Transmitter
  */
 
typedef struct
{
  __IO uint32_t CR1;    /*!< USART Control register 1,                 Address offset: 0x00 */ 
  __IO uint32_t CR2;    /*!< USART Control register 2,                 Address offset: 0x04 */ 
  __IO uint32_t CR3;    /*!< USART Control register 3,                 Address offset: 0x08 */
  __IO uint32_t BRR;    /*!< USART Baud rate register,                 Address offset: 0x0C */
  __IO uint32_t GTPR;   /*!< USART Guard time and prescaler register,  Address offset: 0x10 */
  __IO uint32_t RTOR;   /*!< USART Receiver Time Out register,         Address offset: 0x14 */  
  __IO uint32_t RQR;    /*!< USART Request register,                   Address offset: 0x18 */
  __IO uint32_t ISR;    /*!< USART Interrupt and status register,      Address offset: 0x1C */
  __IO uint32_t ICR;    /*!< USART Interrupt flag Clear register,      Address offset: 0x20 */
  __IO uint16_t RDR;    /*!< USART Receive Data register,              Address offset: 0x24 */
  uint16_t  RESERVED1;  /*!< Reserved, 0x26                                                 */
  __IO uint16_t TDR;    /*!< USART Transmit Data register,             Address offset: 0x28 */
  uint16_t  RESERVED2;  /*!< Reserved, 0x2A                                                 */
} USART_TypeDef;

/** 
  * @brief Window WATCHDOG
  */
typedef struct
{
  __IO uint32_t CR;   /*!< WWDG Control register,       Address offset: 0x00 */
  __IO uint32_t CFR;  /*!< WWDG Configuration register, Address offset: 0x04 */
  __IO uint32_t SR;   /*!< WWDG Status register,        Address offset: 0x08 */
} WWDG_TypeDef;

/** 
  * @}
  */
  
/** @addtogroup Peripheral_memory_map
  * @{
  */

#define FLASH_BASE            ((uint32_t)0x08000000U)              /*!< FLASH base address in the alias region */
#define FLASH_BANK1_END       ((uint32_t)0x0800FFFFU) /*!< FLASH END address of bank1 */
#define SRAM_BASE             ((uint32_t)0x20000000U)              /*!< SRAM base address in the alias region */
#define PERIPH_BASE           ((uint32_t)0x40000000U)              /*!< Peripheral base address in the alias region */

/*!< Peripheral memory map */
#define APBPERIPH_BASE        PERIPH_BASE
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x00020000)
#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x08000000)

/*!< APB peripherals */
#define TIM2_BASE             (APBPERIPH_BASE + 0x00000000)
#define TIM3_BASE             (APBPERIPH_BASE + 0x00000400)
#define TIM6_BASE             (APBPERIPH_BASE + 0x00001000)
#define TIM14_BASE            (APBPERIPH_BASE + 0x00002000)
#define RTC_BASE              (APBPERIPH_BASE + 0x00002800)
#define WWDG_BASE             (APBPERIPH_BASE + 0x00002C00)
#define IWDG_BASE             (APBPERIPH_BASE + 0x00003000)
#define SPI2_BASE             (APBPERIPH_BASE + 0x00003800)
#define USART2_BASE           (APBPERIPH_BASE + 0x00004400)
#define I2C1_BASE             (APBPERIPH_BASE + 0x00005400)
#define I2C2_BASE             (APBPERIPH_BASE + 0x00005800)
#define PWR_BASE              (APBPERIPH_BASE + 0x00007000)
#define DAC_BASE              (APBPERIPH_BASE + 0x00007400)

#define CEC_BASE              (APBPERIPH_BASE + 0x00007800)

#define SYSCFG_BASE           (APBPERIPH_BASE + 0x00010000)
#define COMP_BASE             (APBPERIPH_BASE + 0x0001001C)
#define EXTI_BASE             (APBPERIPH_BASE + 0x00010400)
#define ADC1_BASE             (APBPERIPH_BASE + 0x00012400)
#define ADC_BASE              (APBPERIPH_BASE + 0x00012708)
#define TIM1_BASE             (APBPERIPH_BASE + 0x00012C00)
#define SPI1_BASE             (APBPERIPH_BASE + 0x00013000)
#define USART1_BASE           (APBPERIPH_BASE + 0x00013800)
#define TIM15_BASE            (APBPERIPH_BASE + 0x00014000)
#define TIM16_BASE            (APBPERIPH_BASE + 0x00014400)
#define TIM17_BASE            (APBPERIPH_BASE + 0x00014800)
#define DBGMCU_BASE           (APBPERIPH_BASE + 0x00015800)

/*!< AHB peripherals */
#define DMA1_BASE             (AHBPERIPH_BASE + 0x00000000)
#define DMA1_Channel1_BASE    (DMA1_BASE + 0x00000008)
#define DMA1_Channel2_BASE    (DMA1_BASE + 0x0000001C)
#define DMA1_Channel3_BASE    (DMA1_BASE + 0x00000030)
#define DMA1_Channel4_BASE    (DMA1_BASE + 0x00000044)
#define DMA1_Channel5_BASE    (DMA1_BASE + 0x00000058)

#define RCC_BASE              (AHBPERIPH_BASE + 0x00001000)
#define FLASH_R_BASE          (AHBPERIPH_BASE + 0x00002000) /*!< FLASH registers base address */
#define OB_BASE               ((uint32_t)0x1FFFF800U)       /*!< FLASH Option Bytes base address */
#define FLASHSIZE_BASE        ((uint32_t)0x1FFFF7CCU)       /*!< FLASH Size register base address */
#define UID_BASE              ((uint32_t)0x1FFFF7ACU)       /*!< Unique device ID register base address */
#define CRC_BASE              (AHBPERIPH_BASE + 0x00003000)
#define TSC_BASE              (AHBPERIPH_BASE + 0x00004000)

/*!< AHB2 peripherals */
#define GPIOA_BASE            (AHB2PERIPH_BASE + 0x00000000)
#define GPIOB_BASE            (AHB2PERIPH_BASE + 0x00000400)
#define GPIOC_BASE            (AHB2PERIPH_BASE + 0x00000800)
#define GPIOD_BASE            (AHB2PERIPH_BASE + 0x00000C00)
#define GPIOF_BASE            (AHB2PERIPH_BASE + 0x00001400)

/**
  * @}
  */
  
/** @addtogroup Peripheral_declaration
  * @{
  */  

#define TIM2                ((TIM_TypeDef *) TIM2_BASE)
#define TIM3                ((TIM_TypeDef *) TIM3_BASE)
#define TIM6                ((TIM_TypeDef *) TIM6_BASE)
#define TIM14               ((TIM_TypeDef *) TIM14_BASE)
#define RTC                 ((RTC_TypeDef *) RTC_BASE)
#define WWDG                ((WWDG_TypeDef *) WWDG_BASE)
#define IWDG                ((IWDG_TypeDef *) IWDG_BASE)
#define USART2              ((USART_TypeDef *) USART2_BASE)
#define I2C1                ((I2C_TypeDef *) I2C1_BASE)
#define I2C2                ((I2C_TypeDef *) I2C2_BASE)
#define PWR                 ((PWR_TypeDef *) PWR_BASE)
#define DAC1                ((DAC_TypeDef *) DAC_BASE)
#define DAC                 ((DAC_TypeDef *) DAC_BASE) /* Kept for legacy purpose */
#define CEC                 ((CEC_TypeDef *) CEC_BASE)
#define SYSCFG              ((SYSCFG_TypeDef *) SYSCFG_BASE)
#define COMP1               ((COMP_TypeDef *) COMP_BASE)
#define COMP2               ((COMP_TypeDef *) (COMP_BASE + 0x00000002))
#define COMP12_COMMON       ((COMP_Common_TypeDef *) COMP_BASE)
#define COMP                ((COMP1_2_TypeDef *) COMP_BASE) /* Kept for legacy purpose */
#define EXTI                ((EXTI_TypeDef *) EXTI_BASE)
#define ADC1                ((ADC_TypeDef *) ADC1_BASE)
#define ADC1_COMMON         ((ADC_Common_TypeDef *) ADC_BASE)
#define ADC                 ((ADC_Common_TypeDef *) ADC_BASE) /* Kept for legacy purpose */
#define TIM1                ((TIM_TypeDef *) TIM1_BASE)
#define SPI1                ((SPI_TypeDef *) SPI1_BASE)
#define SPI2                ((SPI_TypeDef *) SPI2_BASE)
#define USART1              ((USART_TypeDef *) USART1_BASE)
#define TIM15               ((TIM_TypeDef *) TIM15_BASE)
#define TIM16               ((TIM_TypeDef *) TIM16_BASE)
#define TIM17               ((TIM_TypeDef *) TIM17_BASE)
#define DBGMCU              ((DBGMCU_TypeDef *) DBGMCU_BASE)
#define DMA1                ((DMA_TypeDef *) DMA1_BASE)
#define DMA1_Channel1       ((DMA_Channel_TypeDef *) DMA1_Channel1_BASE)
#define DMA1_Channel2       ((DMA_Channel_TypeDef *) DMA1_Channel2_BASE)
#define DMA1_Channel3       ((DMA_Channel_TypeDef *) DMA1_Channel3_BASE)
#define DMA1_Channel4       ((DMA_Channel_TypeDef *) DMA1_Channel4_BASE)
#define DMA1_Channel5       ((DMA_Channel_TypeDef *) DMA1_Channel5_BASE)
#define FLASH               ((FLASH_TypeDef *) FLASH_R_BASE)
#define OB                  ((OB_TypeDef *) OB_BASE) 
#define RCC                 ((RCC_TypeDef *) RCC_BASE)
#define CRC                 ((CRC_TypeDef *) CRC_BASE)
#define TSC                 ((TSC_TypeDef *) TSC_BASE)
#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD               ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOF               ((GPIO_TypeDef *) GPIOF_BASE)
/**
  * @}
  */

/** @addtogroup Exported_constants
  * @{
  */

  /** @addtogroup Peripheral_Registers_Bits_Definition
  * @{
  */

/******************************************************************************/
/*                         Peripheral Registers Bits Definition               */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                      Analog to Digital Converter (ADC)                     */
/*                                                                            */
/******************************************************************************/

/*
 * @brief Specific device feature definitions (not present on all devices in the STM32F0 family)
 */
#define ADC_CHANNEL_VBAT_SUPPORT                       /*!< ADC feature available only on specific devices: ADC internal channel Vbat */

/********************  Bits definition for ADC_ISR register  ******************/
#define ADC_ISR_ADRDY           ((uint32_t)0x00000001U) /*!< ADC ready flag */
#define ADC_ISR_EOSMP           ((uint32_t)0x00000002U) /*!< ADC group regular end of sampling flag */
#define ADC_ISR_EOC             ((uint32_t)0x00000004U) /*!< ADC group regular end of unitary conversion flag */
#define ADC_ISR_EOS             ((uint32_t)0x00000008U) /*!< ADC group regular end of sequence conversions flag */
#define ADC_ISR_OVR             ((uint32_t)0x00000010U) /*!< ADC group regular overrun flag */
#define ADC_ISR_AWD1            ((uint32_t)0x00000080U) /*!< ADC analog watchdog 1 flag */

/* Legacy defines */
#define ADC_ISR_AWD             (ADC_ISR_AWD1)
#define ADC_ISR_EOSEQ           (ADC_ISR_EOS)

/********************  Bits definition for ADC_IER register  ******************/
#define ADC_IER_ADRDYIE         ((uint32_t)0x00000001U) /*!< ADC ready interrupt */
#define ADC_IER_EOSMPIE         ((uint32_t)0x00000002U) /*!< ADC group regular end of sampling interrupt */
#define ADC_IER_EOCIE           ((uint32_t)0x00000004U) /*!< ADC group regular end of unitary conversion interrupt */
#define ADC_IER_EOSIE           ((uint32_t)0x00000008U) /*!< ADC group regular end of sequence conversions interrupt */
#define ADC_IER_OVRIE           ((uint32_t)0x00000010U) /*!< ADC group regular overrun interrupt */
#define ADC_IER_AWD1IE          ((uint32_t)0x00000080U) /*!< ADC analog watchdog 1 interrupt */

/* Legacy defines */
#define ADC_IER_AWDIE           (ADC_IER_AWD1IE)
#define ADC_IER_EOSEQIE         (ADC_IER_EOSIE)

/********************  Bits definition for ADC_CR register  *******************/
#define ADC_CR_ADEN             ((uint32_t)0x00000001U) /*!< ADC enable */
#define ADC_CR_ADDIS            ((uint32_t)0x00000002U) /*!< ADC disable */
#define ADC_CR_ADSTART          ((uint32_t)0x00000004U) /*!< ADC group regular conversion start */
#define ADC_CR_ADSTP            ((uint32_t)0x00000010U) /*!< ADC group regular conversion stop */
#define ADC_CR_ADCAL            ((uint32_t)0x80000000U) /*!< ADC calibration */

/*******************  Bits definition for ADC_CFGR1 register  *****************/
#define ADC_CFGR1_DMAEN         ((uint32_t)0x00000001U) /*!< ADC DMA transfer enable */
#define ADC_CFGR1_DMACFG        ((uint32_t)0x00000002U) /*!< ADC DMA transfer configuration */
#define ADC_CFGR1_SCANDIR       ((uint32_t)0x00000004U) /*!< ADC group regular sequencer scan direction */

#define ADC_CFGR1_RES           ((uint32_t)0x00000018U) /*!< ADC data resolution */
#define ADC_CFGR1_RES_0         ((uint32_t)0x00000008U) /*!< bit 0 */
#define ADC_CFGR1_RES_1         ((uint32_t)0x00000010U) /*!< bit 1 */

#define ADC_CFGR1_ALIGN         ((uint32_t)0x00000020U) /*!< ADC data alignement */

#define ADC_CFGR1_EXTSEL        ((uint32_t)0x000001C0U) /*!< ADC group regular external trigger source */
#define ADC_CFGR1_EXTSEL_0      ((uint32_t)0x00000040U) /*!< bit 0 */
#define ADC_CFGR1_EXTSEL_1      ((uint32_t)0x00000080U) /*!< bit 1 */
#define ADC_CFGR1_EXTSEL_2      ((uint32_t)0x00000100U) /*!< bit 2 */

#define ADC_CFGR1_EXTEN         ((uint32_t)0x00000C00U) /*!< ADC group regular external trigger polarity */
#define ADC_CFGR1_EXTEN_0       ((uint32_t)0x00000400U) /*!< bit 0 */
#define ADC_CFGR1_EXTEN_1       ((uint32_t)0x00000800U) /*!< bit 1 */

#define ADC_CFGR1_OVRMOD        ((uint32_t)0x00001000U) /*!< ADC group regular overrun configuration */
#define ADC_CFGR1_CONT          ((uint32_t)0x00002000U) /*!< ADC group regular continuous conversion mode */
#define ADC_CFGR1_WAIT          ((uint32_t)0x00004000U) /*!< ADC low power auto wait */
#define ADC_CFGR1_AUTOFF        ((uint32_t)0x00008000U) /*!< ADC low power auto power off */
#define ADC_CFGR1_DISCEN        ((uint32_t)0x00010000U) /*!< ADC group regular sequencer discontinuous mode */

#define ADC_CFGR1_AWD1SGL       ((uint32_t)0x00400000U) /*!< ADC analog watchdog 1 monitoring a single channel or all channels */
#define ADC_CFGR1_AWD1EN        ((uint32_t)0x00800000U) /*!< ADC analog watchdog 1 enable on scope ADC group regular */

#define ADC_CFGR1_AWD1CH        ((uint32_t)0x7C000000U) /*!< ADC analog watchdog 1 monitored channel selection */
#define ADC_CFGR1_AWD1CH_0      ((uint32_t)0x04000000U) /*!< bit 0 */
#define ADC_CFGR1_AWD1CH_1      ((uint32_t)0x08000000U) /*!< bit 1  */
#define ADC_CFGR1_AWD1CH_2      ((uint32_t)0x10000000U) /*!< bit 2  */
#define ADC_CFGR1_AWD1CH_3      ((uint32_t)0x20000000U) /*!< bit 3  */
#define ADC_CFGR1_AWD1CH_4      ((uint32_t)0x40000000U) /*!< bit 4  */

/* Legacy defines */
#define ADC_CFGR1_AUTDLY        (ADC_CFGR1_WAIT)
#define ADC_CFGR1_AWDSGL        (ADC_CFGR1_AWD1SGL)
#define ADC_CFGR1_AWDEN         (ADC_CFGR1_AWD1EN)
#define ADC_CFGR1_AWDCH         (ADC_CFGR1_AWD1CH)
#define ADC_CFGR1_AWDCH_0       (ADC_CFGR1_AWD1CH_0)
#define ADC_CFGR1_AWDCH_1       (ADC_CFGR1_AWD1CH_1)
#define ADC_CFGR1_AWDCH_2       (ADC_CFGR1_AWD1CH_2)
#define ADC_CFGR1_AWDCH_3       (ADC_CFGR1_AWD1CH_3)
#define ADC_CFGR1_AWDCH_4       (ADC_CFGR1_AWD1CH_4)

/*******************  Bits definition for ADC_CFGR2 register  *****************/
#define ADC_CFGR2_CKMODE        ((uint32_t)0xC0000000U) /*!< ADC clock source and prescaler (prescaler only for clock source synchronous) */
#define ADC_CFGR2_CKMODE_1      ((uint32_t)0x80000000U) /*!< bit 1 */
#define ADC_CFGR2_CKMODE_0      ((uint32_t)0x40000000U) /*!< bit 0 */

/* Legacy defines */
#define  ADC_CFGR2_JITOFFDIV4   (ADC_CFGR2_CKMODE_1)   /*!< ADC clocked by PCLK div4 */
#define  ADC_CFGR2_JITOFFDIV2   (ADC_CFGR2_CKMODE_0)   /*!< ADC clocked by PCLK div2 */

/******************  Bit definition for ADC_SMPR register  ********************/
#define  ADC_SMPR_SMP           ((uint32_t)0x00000007U) /*!< ADC group of channels sampling time 2 */
#define  ADC_SMPR_SMP_0         ((uint32_t)0x00000001U) /*!< bit 0 */
#define  ADC_SMPR_SMP_1         ((uint32_t)0x00000002U) /*!< bit 1 */
#define  ADC_SMPR_SMP_2         ((uint32_t)0x00000004U) /*!< bit 2 */

/* Legacy defines */
#define  ADC_SMPR1_SMPR         (ADC_SMPR_SMP)         /*!< SMP[2:0] bits (Sampling time selection) */
#define  ADC_SMPR1_SMPR_0       (ADC_SMPR_SMP_0)       /*!< bit 0 */
#define  ADC_SMPR1_SMPR_1       (ADC_SMPR_SMP_1)       /*!< bit 1 */
#define  ADC_SMPR1_SMPR_2       (ADC_SMPR_SMP_2)       /*!< bit 2 */

/*******************  Bit definition for ADC_TR register  ********************/
#define ADC_TR1_LT1             ((uint32_t)0x00000FFFU) /*!< ADC analog watchdog 1 threshold low */
#define ADC_TR1_LT1_0           ((uint32_t)0x00000001U) /*!< bit 0 */
#define ADC_TR1_LT1_1           ((uint32_t)0x00000002U) /*!< bit 1 */
#define ADC_TR1_LT1_2           ((uint32_t)0x00000004U) /*!< bit 2 */
#define ADC_TR1_LT1_3           ((uint32_t)0x00000008U) /*!< bit 3 */
#define ADC_TR1_LT1_4           ((uint32_t)0x00000010U) /*!< bit 4 */
#define ADC_TR1_LT1_5           ((uint32_t)0x00000020U) /*!< bit 5 */
#define ADC_TR1_LT1_6           ((uint32_t)0x00000040U) /*!< bit 6 */
#define ADC_TR1_LT1_7           ((uint32_t)0x00000080U) /*!< bit 7 */
#define ADC_TR1_LT1_8           ((uint32_t)0x00000100U) /*!< bit 8 */
#define ADC_TR1_LT1_9           ((uint32_t)0x00000200U) /*!< bit 9 */
#define ADC_TR1_LT1_10          ((uint32_t)0x00000400U) /*!< bit 10 */
#define ADC_TR1_LT1_11          ((uint32_t)0x00000800U) /*!< bit 11 */

#define ADC_TR1_HT1             ((uint32_t)0x0FFF0000U) /*!< ADC Analog watchdog 1 threshold high */
#define ADC_TR1_HT1_0           ((uint32_t)0x00010000U) /*!< bit 0 */
#define ADC_TR1_HT1_1           ((uint32_t)0x00020000U) /*!< bit 1 */
#define ADC_TR1_HT1_2           ((uint32_t)0x00040000U) /*!< bit 2 */
#define ADC_TR1_HT1_3           ((uint32_t)0x00080000U) /*!< bit 3 */
#define ADC_TR1_HT1_4           ((uint32_t)0x00100000U) /*!< bit 4 */
#define ADC_TR1_HT1_5           ((uint32_t)0x00200000U) /*!< bit 5 */
#define ADC_TR1_HT1_6           ((uint32_t)0x00400000U) /*!< bit 6 */
#define ADC_TR1_HT1_7           ((uint32_t)0x00800000U) /*!< bit 7 */
#define ADC_TR1_HT1_8           ((uint32_t)0x01000000U) /*!< bit 8 */
#define ADC_TR1_HT1_9           ((uint32_t)0x02000000U) /*!< bit 9 */
#define ADC_TR1_HT1_10          ((uint32_t)0x04000000U) /*!< bit 10 */
#define ADC_TR1_HT1_11          ((uint32_t)0x08000000U) /*!< bit 11 */

/* Legacy defines */
#define  ADC_TR_HT              (ADC_TR1_HT1)
#define  ADC_TR_LT              (ADC_TR1_LT1)
#define  ADC_HTR_HT             (ADC_TR1_HT1)
#define  ADC_LTR_LT             (ADC_TR1_LT1)

/******************  Bit definition for ADC_CHSELR register  ******************/
#define ADC_CHSELR_CHSEL        ((uint32_t)0x0007FFFFU) /*!< ADC group regular sequencer channels, available when ADC_CFGR1_CHSELRMOD is reset */
#define ADC_CHSELR_CHSEL18      ((uint32_t)0x00040000U) /*!< ADC group regular sequencer channel 18, available when ADC_CFGR1_CHSELRMOD is reset */
#define ADC_CHSELR_CHSEL17      ((uint32_t)0x00020000U) /*!< ADC group regular sequencer channel 17, available when ADC_CFGR1_CHSELRMOD is reset */
#define ADC_CHSELR_CHSEL16      ((uint32_t)0x00010000U) /*!< ADC group regular sequencer channel 16, available when ADC_CFGR1_CHSELRMOD is reset */
#define ADC_CHSELR_CHSEL15      ((uint32_t)0x00008000U) /*!< ADC group regular sequencer channel 15, available when ADC_CFGR1_CHSELRMOD is reset */
#define ADC_CHSELR_CHSEL14      ((uint32_t)0x00004000U) /*!< ADC group regular sequencer channel 14, available when ADC_CFGR1_CHSELRMOD is reset */
#define ADC_CHSELR_CHSEL13      ((uint32_t)0x00002000U) /*!< ADC group regular sequencer channel 13, available when ADC_CFGR1_CHSELRMOD is reset */
#define ADC_CHSELR_CHSEL12      ((uint32_t)0x00001000U) /*!< ADC group regular sequencer channel 12, available when ADC_CFGR1_CHSELRMOD is reset */
#define ADC_CHSELR_CHSEL11      ((uint32_t)0x00000800U) /*!< ADC group regular sequencer channel 11, available when ADC_CFGR1_CHSELRMOD is reset */
#define ADC_CHSELR_CHSEL10      ((uint32_t)0x00000400U) /*!< ADC group regular sequencer channel 10, available when ADC_CFGR1_CHSELRMOD is reset */
#define ADC_CHSELR_CHSEL9       ((uint32_t)0x00000200U) /*!< ADC group regular sequencer channel 9, available when ADC_CFGR1_CHSELRMOD is reset */
#define ADC_CHSELR_CHSEL8       ((uint32_t)0x00000100U) /*!< ADC group regular sequencer channel 8, available when ADC_CFGR1_CHSELRMOD is reset */
#define ADC_CHSELR_CHSEL7       ((uint32_t)0x00000080U) /*!< ADC group regular sequencer channel 7, available when ADC_CFGR1_CHSELRMOD is reset */
#define ADC_CHSELR_CHSEL6       ((uint32_t)0x00000040U) /*!< ADC group regular sequencer channel 6, available when ADC_CFGR1_CHSELRMOD is reset */
#define ADC_CHSELR_CHSEL5       ((uint32_t)0x00000020U) /*!< ADC group regular sequencer channel 5, available when ADC_CFGR1_CHSELRMOD is reset */
#define ADC_CHSELR_CHSEL4       ((uint32_t)0x00000010U) /*!< ADC group regular sequencer channel 4, available when ADC_CFGR1_CHSELRMOD is reset */
#define ADC_CHSELR_CHSEL3       ((uint32_t)0x00000008U) /*!< ADC group regular sequencer channel 3, available when ADC_CFGR1_CHSELRMOD is reset */
#define ADC_CHSELR_CHSEL2       ((uint32_t)0x00000004U) /*!< ADC group regular sequencer channel 2, available when ADC_CFGR1_CHSELRMOD is reset */
#define ADC_CHSELR_CHSEL1       ((uint32_t)0x00000002U) /*!< ADC group regular sequencer channel 1, available when ADC_CFGR1_CHSELRMOD is reset */
#define ADC_CHSELR_CHSEL0       ((uint32_t)0x00000001U) /*!< ADC group regular sequencer channel 0, available when ADC_CFGR1_CHSELRMOD is reset */

/********************  Bit definition for ADC_DR register  ********************/
#define ADC_DR_DATA             ((uint32_t)0x0000FFFFU) /*!< ADC group regular conversion data */
#define ADC_DR_DATA_0           ((uint32_t)0x00000001U) /*!< bit 0 */
#define ADC_DR_DATA_1           ((uint32_t)0x00000002U) /*!< bit 1 */
#define ADC_DR_DATA_2           ((uint32_t)0x00000004U) /*!< bit 2 */
#define ADC_DR_DATA_3           ((uint32_t)0x00000008U) /*!< bit 3 */
#define ADC_DR_DATA_4           ((uint32_t)0x00000010U) /*!< bit 4 */
#define ADC_DR_DATA_5           ((uint32_t)0x00000020U) /*!< bit 5 */
#define ADC_DR_DATA_6           ((uint32_t)0x00000040U) /*!< bit 6 */
#define ADC_DR_DATA_7           ((uint32_t)0x00000080U) /*!< bit 7 */
#define ADC_DR_DATA_8           ((uint32_t)0x00000100U) /*!< bit 8 */
#define ADC_DR_DATA_9           ((uint32_t)0x00000200U) /*!< bit 9 */
#define ADC_DR_DATA_10          ((uint32_t)0x00000400U) /*!< bit 10 */
#define ADC_DR_DATA_11          ((uint32_t)0x00000800U) /*!< bit 11 */
#define ADC_DR_DATA_12          ((uint32_t)0x00001000U) /*!< bit 12 */
#define ADC_DR_DATA_13          ((uint32_t)0x00002000U) /*!< bit 13 */
#define ADC_DR_DATA_14          ((uint32_t)0x00004000U) /*!< bit 14 */
#define ADC_DR_DATA_15          ((uint32_t)0x00008000U) /*!< bit 15 */

/*************************  ADC Common registers  *****************************/
/*******************  Bit definition for ADC_CCR register  ********************/
#define ADC_CCR_VREFEN          ((uint32_t)0x00400000U) /*!< ADC internal path to VrefInt enable */
#define ADC_CCR_TSEN            ((uint32_t)0x00800000U) /*!< ADC internal path to temperature sensor enable */

#define ADC_CCR_VBATEN          ((uint32_t)0x01000000U) /*!< ADC internal path to battery voltage enable */

/******************************************************************************/
/*                                                                            */
/*                                 HDMI-CEC (CEC)                             */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for CEC_CR register  *********************/
#define  CEC_CR_CECEN                        ((uint32_t)0x00000001U)       /*!< CEC Enable                         */
#define  CEC_CR_TXSOM                        ((uint32_t)0x00000002U)       /*!< CEC Tx Start Of Message            */
#define  CEC_CR_TXEOM                        ((uint32_t)0x00000004U)       /*!< CEC Tx End Of Message              */

/*******************  Bit definition for CEC_CFGR register  *******************/
#define  CEC_CFGR_SFT                        ((uint32_t)0x00000007U)       /*!< CEC Signal Free Time               */
#define  CEC_CFGR_RXTOL                      ((uint32_t)0x00000008U)       /*!< CEC Tolerance                      */
#define  CEC_CFGR_BRESTP                     ((uint32_t)0x00000010U)       /*!< CEC Rx Stop                        */
#define  CEC_CFGR_BREGEN                     ((uint32_t)0x00000020U)       /*!< CEC Bit Rising Error generation    */
#define  CEC_CFGR_LBPEGEN                    ((uint32_t)0x00000040U)       /*!< CEC Long Bit Period Error gener.   */
#define  CEC_CFGR_BRDNOGEN                   ((uint32_t)0x00000080U)       /*!< CEC Broadcast No Error generation  */
#define  CEC_CFGR_SFTOPT                     ((uint32_t)0x00000100U)       /*!< CEC Signal Free Time optional      */
#define  CEC_CFGR_OAR                        ((uint32_t)0x7FFF0000U)       /*!< CEC Own Address                    */
#define  CEC_CFGR_LSTN                       ((uint32_t)0x80000000U)       /*!< CEC Listen mode                    */

/*******************  Bit definition for CEC_TXDR register  *******************/
#define  CEC_TXDR_TXD                        ((uint32_t)0x000000FFU)       /*!< CEC Tx Data                        */

/*******************  Bit definition for CEC_RXDR register  *******************/
#define  CEC_TXDR_RXD                        ((uint32_t)0x000000FFU)       /*!< CEC Rx Data                        */

/*******************  Bit definition for CEC_ISR register  ********************/
#define  CEC_ISR_RXBR                        ((uint32_t)0x00000001U)       /*!< CEC Rx-Byte Received                   */
#define  CEC_ISR_RXEND                       ((uint32_t)0x00000002U)       /*!< CEC End Of Reception                   */
#define  CEC_ISR_RXOVR                       ((uint32_t)0x00000004U)       /*!< CEC Rx-Overrun                         */
#define  CEC_ISR_BRE                         ((uint32_t)0x00000008U)       /*!< CEC Rx Bit Rising Error                */
#define  CEC_ISR_SBPE                        ((uint32_t)0x00000010U)       /*!< CEC Rx Short Bit period Error          */
#define  CEC_ISR_LBPE                        ((uint32_t)0x00000020U)       /*!< CEC Rx Long Bit period Error           */
#define  CEC_ISR_RXACKE                      ((uint32_t)0x00000040U)       /*!< CEC Rx Missing Acknowledge             */
#define  CEC_ISR_ARBLST                      ((uint32_t)0x00000080U)       /*!< CEC Arbitration Lost                   */
#define  CEC_ISR_TXBR                        ((uint32_t)0x00000100U)       /*!< CEC Tx Byte Request                    */
#define  CEC_ISR_TXEND                       ((uint32_t)0x00000200U)       /*!< CEC End of Transmission                */
#define  CEC_ISR_TXUDR                       ((uint32_t)0x00000400U)       /*!< CEC Tx-Buffer Underrun                 */
#define  CEC_ISR_TXERR                       ((uint32_t)0x00000800U)       /*!< CEC Tx-Error                           */
#define  CEC_ISR_TXACKE                      ((uint32_t)0x00001000U)       /*!< CEC Tx Missing Acknowledge             */

/*******************  Bit definition for CEC_IER register  ********************/
#define  CEC_IER_RXBRIE                      ((uint32_t)0x00000001U)       /*!< CEC Rx-Byte Received IT Enable         */
#define  CEC_IER_RXENDIE                     ((uint32_t)0x00000002U)       /*!< CEC End Of Reception IT Enable         */
#define  CEC_IER_RXOVRIE                     ((uint32_t)0x00000004U)       /*!< CEC Rx-Overrun IT Enable               */
#define  CEC_IER_BREIE                       ((uint32_t)0x00000008U)       /*!< CEC Rx Bit Rising Error IT Enable      */
#define  CEC_IER_SBPEIE                      ((uint32_t)0x00000010U)       /*!< CEC Rx Short Bit period Error IT Enable*/
#define  CEC_IER_LBPEIE                      ((uint32_t)0x00000020U)       /*!< CEC Rx Long Bit period Error IT Enable */
#define  CEC_IER_RXACKEIE                    ((uint32_t)0x00000040U)       /*!< CEC Rx Missing Acknowledge IT Enable   */
#define  CEC_IER_ARBLSTIE                    ((uint32_t)0x00000080U)       /*!< CEC Arbitration Lost IT Enable         */
#define  CEC_IER_TXBRIE                      ((uint32_t)0x00000100U)       /*!< CEC Tx Byte Request  IT Enable         */
#define  CEC_IER_TXENDIE                     ((uint32_t)0x00000200U)       /*!< CEC End of Transmission IT Enable      */
#define  CEC_IER_TXUDRIE                     ((uint32_t)0x00000400U)       /*!< CEC Tx-Buffer Underrun IT Enable       */
#define  CEC_IER_TXERRIE                     ((uint32_t)0x00000800U)       /*!< CEC Tx-Error IT Enable                 */
#define  CEC_IER_TXACKEIE                    ((uint32_t)0x00001000U)       /*!< CEC Tx Missing Acknowledge IT Enable   */

/******************************************************************************/
/*                                                                            */
/*                      Analog Comparators (COMP)                             */
/*                                                                            */
/******************************************************************************/
/***********************  Bit definition for COMP_CSR register  ***************/
/* COMP1 bits definition */
#define COMP_CSR_COMP1EN               ((uint32_t)0x00000001U) /*!< COMP1 enable */
#define COMP_CSR_COMP1SW1              ((uint32_t)0x00000002U) /*!< COMP1 SW1 switch control */
#define COMP_CSR_COMP1MODE             ((uint32_t)0x0000000CU) /*!< COMP1 power mode */
#define COMP_CSR_COMP1MODE_0           ((uint32_t)0x00000004U) /*!< COMP1 power mode bit 0 */
#define COMP_CSR_COMP1MODE_1           ((uint32_t)0x00000008U) /*!< COMP1 power mode bit 1 */
#define COMP_CSR_COMP1INSEL            ((uint32_t)0x00000070U) /*!< COMP1 inverting input select */
#define COMP_CSR_COMP1INSEL_0          ((uint32_t)0x00000010U) /*!< COMP1 inverting input select bit 0 */
#define COMP_CSR_COMP1INSEL_1          ((uint32_t)0x00000020U) /*!< COMP1 inverting input select bit 1 */
#define COMP_CSR_COMP1INSEL_2          ((uint32_t)0x00000040U) /*!< COMP1 inverting input select bit 2 */
#define COMP_CSR_COMP1OUTSEL           ((uint32_t)0x00000700U) /*!< COMP1 output select */
#define COMP_CSR_COMP1OUTSEL_0         ((uint32_t)0x00000100U) /*!< COMP1 output select bit 0 */
#define COMP_CSR_COMP1OUTSEL_1         ((uint32_t)0x00000200U) /*!< COMP1 output select bit 1 */
#define COMP_CSR_COMP1OUTSEL_2         ((uint32_t)0x00000400U) /*!< COMP1 output select bit 2 */
#define COMP_CSR_COMP1POL              ((uint32_t)0x00000800U) /*!< COMP1 output polarity */
#define COMP_CSR_COMP1HYST             ((uint32_t)0x00003000U) /*!< COMP1 hysteresis */
#define COMP_CSR_COMP1HYST_0           ((uint32_t)0x00001000U) /*!< COMP1 hysteresis bit 0 */
#define COMP_CSR_COMP1HYST_1           ((uint32_t)0x00002000U) /*!< COMP1 hysteresis bit 1 */
#define COMP_CSR_COMP1OUT              ((uint32_t)0x00004000U) /*!< COMP1 output level */
#define COMP_CSR_COMP1LOCK             ((uint32_t)0x00008000U) /*!< COMP1 lock */
/* COMP2 bits definition */
#define COMP_CSR_COMP2EN               ((uint32_t)0x00010000U) /*!< COMP2 enable */
#define COMP_CSR_COMP2MODE             ((uint32_t)0x000C0000U) /*!< COMP2 power mode */
#define COMP_CSR_COMP2MODE_0           ((uint32_t)0x00040000U) /*!< COMP2 power mode bit 0 */
#define COMP_CSR_COMP2MODE_1           ((uint32_t)0x00080000U) /*!< COMP2 power mode bit 1 */
#define COMP_CSR_COMP2INSEL            ((uint32_t)0x00700000U) /*!< COMP2 inverting input select */
#define COMP_CSR_COMP2INSEL_0          ((uint32_t)0x00100000U) /*!< COMP2 inverting input select bit 0 */
#define COMP_CSR_COMP2INSEL_1          ((uint32_t)0x00200000U) /*!< COMP2 inverting input select bit 1 */
#define COMP_CSR_COMP2INSEL_2          ((uint32_t)0x00400000U) /*!< COMP2 inverting input select bit 2 */
#define COMP_CSR_WNDWEN                ((uint32_t)0x00800000U) /*!< COMPx window mode. Bit intended to be used with COMP common instance (COMP_Common_TypeDef) */
#define COMP_CSR_COMP2OUTSEL           ((uint32_t)0x07000000U) /*!< COMP2 output select */
#define COMP_CSR_COMP2OUTSEL_0         ((uint32_t)0x01000000U) /*!< COMP2 output select bit 0 */
#define COMP_CSR_COMP2OUTSEL_1         ((uint32_t)0x02000000U) /*!< COMP2 output select bit 1 */
#define COMP_CSR_COMP2OUTSEL_2         ((uint32_t)0x04000000U) /*!< COMP2 output select bit 2 */
#define COMP_CSR_COMP2POL              ((uint32_t)0x08000000U) /*!< COMP2 output polarity */
#define COMP_CSR_COMP2HYST             ((uint32_t)0x30000000U) /*!< COMP2 hysteresis */
#define COMP_CSR_COMP2HYST_0           ((uint32_t)0x10000000U) /*!< COMP2 hysteresis bit 0 */
#define COMP_CSR_COMP2HYST_1           ((uint32_t)0x20000000U) /*!< COMP2 hysteresis bit 1 */
#define COMP_CSR_COMP2OUT              ((uint32_t)0x40000000U) /*!< COMP2 output level */
#define COMP_CSR_COMP2LOCK             ((uint32_t)0x80000000U) /*!< COMP2 lock */
/* COMPx bits definition */
#define COMP_CSR_COMPxEN               ((uint32_t)0x00000001U) /*!< COMPx enable */
#define COMP_CSR_COMPxMODE             ((uint32_t)0x0000000CU) /*!< COMPx power mode */
#define COMP_CSR_COMPxMODE_0           ((uint32_t)0x00000004U) /*!< COMPx power mode bit 0 */
#define COMP_CSR_COMPxMODE_1           ((uint32_t)0x00000008U) /*!< COMPx power mode bit 1 */
#define COMP_CSR_COMPxINSEL            ((uint32_t)0x00000070U) /*!< COMPx inverting input select */
#define COMP_CSR_COMPxINSEL_0          ((uint32_t)0x00000010U) /*!< COMPx inverting input select bit 0 */
#define COMP_CSR_COMPxINSEL_1          ((uint32_t)0x00000020U) /*!< COMPx inverting input select bit 1 */
#define COMP_CSR_COMPxINSEL_2          ((uint32_t)0x00000040U) /*!< COMPx inverting input select bit 2 */
#define COMP_CSR_COMPxOUTSEL           ((uint32_t)0x00000700U) /*!< COMPx output select */
#define COMP_CSR_COMPxOUTSEL_0         ((uint32_t)0x00000100U) /*!< COMPx output select bit 0 */
#define COMP_CSR_COMPxOUTSEL_1         ((uint32_t)0x00000200U) /*!< COMPx output select bit 1 */
#define COMP_CSR_COMPxOUTSEL_2         ((uint32_t)0x00000400U) /*!< COMPx output select bit 2 */
#define COMP_CSR_COMPxPOL              ((uint32_t)0x00000800U) /*!< COMPx output polarity */
#define COMP_CSR_COMPxHYST             ((uint32_t)0x00003000U) /*!< COMPx hysteresis */
#define COMP_CSR_COMPxHYST_0           ((uint32_t)0x00001000U) /*!< COMPx hysteresis bit 0 */
#define COMP_CSR_COMPxHYST_1           ((uint32_t)0x00002000U) /*!< COMPx hysteresis bit 1 */
#define COMP_CSR_COMPxOUT              ((uint32_t)0x00004000U) /*!< COMPx output level */
#define COMP_CSR_COMPxLOCK             ((uint32_t)0x00008000U) /*!< COMPx lock */

/******************************************************************************/
/*                                                                            */
/*                       CRC calculation unit (CRC)                           */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for CRC_DR register  *********************/
#define  CRC_DR_DR                           ((uint32_t)0xFFFFFFFFU) /*!< Data register bits */

/*******************  Bit definition for CRC_IDR register  ********************/
#define  CRC_IDR_IDR                         ((uint8_t)0xFFU)        /*!< General-purpose 8-bit data register bits */

/********************  Bit definition for CRC_CR register  ********************/
#define  CRC_CR_RESET                        ((uint32_t)0x00000001U) /*!< RESET the CRC computation unit bit */
#define  CRC_CR_REV_IN                       ((uint32_t)0x00000060U) /*!< REV_IN Reverse Input Data bits */
#define  CRC_CR_REV_IN_0                     ((uint32_t)0x00000020U) /*!< REV_IN Bit 0 */
#define  CRC_CR_REV_IN_1                     ((uint32_t)0x00000040U) /*!< REV_IN Bit 1 */
#define  CRC_CR_REV_OUT                      ((uint32_t)0x00000080U) /*!< REV_OUT Reverse Output Data bits */

/*******************  Bit definition for CRC_INIT register  *******************/
#define  CRC_INIT_INIT                       ((uint32_t)0xFFFFFFFFU) /*!< Initial CRC value bits */

/******************************************************************************/
/*                                                                            */
/*                 Digital to Analog Converter (DAC)                          */
/*                                                                            */
/******************************************************************************/

/*
 * @brief Specific device feature definitions (not present on all devices in the STM32F0 family)
 */
/* Note: No specific macro feature on this device */

/********************  Bit definition for DAC_CR register  ********************/
#define  DAC_CR_EN1                          ((uint32_t)0x00000001U)        /*!< DAC channel1 enable */
#define  DAC_CR_BOFF1                        ((uint32_t)0x00000002U)        /*!< DAC channel1 output buffer disable */
#define  DAC_CR_TEN1                         ((uint32_t)0x00000004U)        /*!< DAC channel1 Trigger enable */

#define  DAC_CR_TSEL1                        ((uint32_t)0x00000038U)        /*!< TSEL1[2:0] (DAC channel1 Trigger selection) */
#define  DAC_CR_TSEL1_0                      ((uint32_t)0x00000008U)        /*!< Bit 0 */
#define  DAC_CR_TSEL1_1                      ((uint32_t)0x00000010U)        /*!< Bit 1 */
#define  DAC_CR_TSEL1_2                      ((uint32_t)0x00000020U)        /*!< Bit 2 */

#define  DAC_CR_DMAEN1                       ((uint32_t)0x00001000U)        /*!< DAC channel1 DMA enable */
#define  DAC_CR_DMAUDRIE1                    ((uint32_t)0x00002000U)        /*!< DAC channel1 DMA Underrun Interrupt enable */


/*****************  Bit definition for DAC_SWTRIGR register  ******************/
#define  DAC_SWTRIGR_SWTRIG1                 ((uint32_t)0x00000001U)        /*!< DAC channel1 software trigger */

/*****************  Bit definition for DAC_DHR12R1 register  ******************/
#define  DAC_DHR12R1_DACC1DHR                ((uint32_t)0x00000FFFU)        /*!< DAC channel1 12-bit Right aligned data */

/*****************  Bit definition for DAC_DHR12L1 register  ******************/
#define  DAC_DHR12L1_DACC1DHR                ((uint32_t)0x0000FFF0U)        /*!< DAC channel1 12-bit Left aligned data */

/******************  Bit definition for DAC_DHR8R1 register  ******************/
#define  DAC_DHR8R1_DACC1DHR                 ((uint32_t)0x000000FFU)        /*!< DAC channel1 8-bit Right aligned data */

/*******************  Bit definition for DAC_DOR1 register  *******************/
#define  DAC_DOR1_DACC1DOR                   ((uint32_t)0x00000FFFU)        /*!< DAC channel1 data output */

/********************  Bit definition for DAC_SR register  ********************/
#define  DAC_SR_DMAUDR1                      ((uint32_t)0x00002000U)        /*!< DAC channel1 DMA underrun flag */
#define  DAC_SR_DMAUDR2                      ((uint32_t)0x20000000U)        /*!< DAC channel2 DMA underrun flag  */

/******************************************************************************/
/*                                                                            */
/*                           Debug MCU (DBGMCU)                               */
/*                                                                            */
/******************************************************************************/

/****************  Bit definition for DBGMCU_IDCODE register  *****************/
#define  DBGMCU_IDCODE_DEV_ID                ((uint32_t)0x00000FFFU)        /*!< Device Identifier */

#define  DBGMCU_IDCODE_REV_ID                ((uint32_t)0xFFFF0000U)        /*!< REV_ID[15:0] bits (Revision Identifier) */
#define  DBGMCU_IDCODE_REV_ID_0              ((uint32_t)0x00010000U)        /*!< Bit 0 */
#define  DBGMCU_IDCODE_REV_ID_1              ((uint32_t)0x00020000U)        /*!< Bit 1 */
#define  DBGMCU_IDCODE_REV_ID_2              ((uint32_t)0x00040000U)        /*!< Bit 2 */
#define  DBGMCU_IDCODE_REV_ID_3              ((uint32_t)0x00080000U)        /*!< Bit 3 */
#define  DBGMCU_IDCODE_REV_ID_4              ((uint32_t)0x00100000U)        /*!< Bit 4 */
#define  DBGMCU_IDCODE_REV_ID_5              ((uint32_t)0x00200000U)        /*!< Bit 5 */
#define  DBGMCU_IDCODE_REV_ID_6              ((uint32_t)0x00400000U)        /*!< Bit 6 */
#define  DBGMCU_IDCODE_REV_ID_7              ((uint32_t)0x00800000U)        /*!< Bit 7 */
#define  DBGMCU_IDCODE_REV_ID_8              ((uint32_t)0x01000000U)        /*!< Bit 8 */
#define  DBGMCU_IDCODE_REV_ID_9              ((uint32_t)0x02000000U)        /*!< Bit 9 */
#define  DBGMCU_IDCODE_REV_ID_10             ((uint32_t)0x04000000U)        /*!< Bit 10 */
#define  DBGMCU_IDCODE_REV_ID_11             ((uint32_t)0x08000000U)        /*!< Bit 11 */
#define  DBGMCU_IDCODE_REV_ID_12             ((uint32_t)0x10000000U)        /*!< Bit 12 */
#define  DBGMCU_IDCODE_REV_ID_13             ((uint32_t)0x20000000U)        /*!< Bit 13 */
#define  DBGMCU_IDCODE_REV_ID_14             ((uint32_t)0x40000000U)        /*!< Bit 14 */
#define  DBGMCU_IDCODE_REV_ID_15             ((uint32_t)0x80000000U)        /*!< Bit 15 */

/******************  Bit definition for DBGMCU_CR register  *******************/
#define  DBGMCU_CR_DBG_STOP                  ((uint32_t)0x00000002U)        /*!< Debug Stop Mode */
#define  DBGMCU_CR_DBG_STANDBY               ((uint32_t)0x00000004U)        /*!< Debug Standby mode */

/******************  Bit definition for DBGMCU_APB1_FZ register  **************/
#define  DBGMCU_APB1_FZ_DBG_TIM2_STOP        ((uint32_t)0x00000001U)        /*!< TIM2 counter stopped when core is halted */
#define  DBGMCU_APB1_FZ_DBG_TIM3_STOP        ((uint32_t)0x00000002U)        /*!< TIM3 counter stopped when core is halted */
#define  DBGMCU_APB1_FZ_DBG_TIM6_STOP        ((uint32_t)0x00000010U)        /*!< TIM6 counter stopped when core is halted */
#define  DBGMCU_APB1_FZ_DBG_TIM14_STOP       ((uint32_t)0x00000100U)        /*!< TIM14 counter stopped when core is halted */
#define  DBGMCU_APB1_FZ_DBG_RTC_STOP         ((uint32_t)0x00000400U)        /*!< RTC Calendar frozen when core is halted */
#define  DBGMCU_APB1_FZ_DBG_WWDG_STOP        ((uint32_t)0x00000800U)        /*!< Debug Window Watchdog stopped when Core is halted */
#define  DBGMCU_APB1_FZ_DBG_IWDG_STOP        ((uint32_t)0x00001000U)        /*!< Debug Independent Watchdog stopped when Core is halted */
#define  DBGMCU_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT    ((uint32_t)0x00200000U)   /*!< I2C1 SMBUS timeout mode stopped when Core is halted */

/******************  Bit definition for DBGMCU_APB2_FZ register  **************/
#define  DBGMCU_APB2_FZ_DBG_TIM1_STOP        ((uint32_t)0x00000800U)        /*!< TIM1 counter stopped when core is halted */
#define  DBGMCU_APB2_FZ_DBG_TIM15_STOP       ((uint32_t)0x00010000U)        /*!< TIM15 counter stopped when core is halted  */
#define  DBGMCU_APB2_FZ_DBG_TIM16_STOP       ((uint32_t)0x00020000U)        /*!< TIM16 counter stopped when core is halted */
#define  DBGMCU_APB2_FZ_DBG_TIM17_STOP       ((uint32_t)0x00040000U)        /*!< TIM17 counter stopped when core is halted */

/******************************************************************************/
/*                                                                            */
/*                           DMA Controller (DMA)                             */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for DMA_ISR register  ********************/
#define  DMA_ISR_GIF1                        ((uint32_t)0x00000001U)        /*!< Channel 1 Global interrupt flag    */
#define  DMA_ISR_TCIF1                       ((uint32_t)0x00000002U)        /*!< Channel 1 Transfer Complete flag   */
#define  DMA_ISR_HTIF1                       ((uint32_t)0x00000004U)        /*!< Channel 1 Half Transfer flag       */
#define  DMA_ISR_TEIF1                       ((uint32_t)0x00000008U)        /*!< Channel 1 Transfer Error flag      */
#define  DMA_ISR_GIF2                        ((uint32_t)0x00000010U)        /*!< Channel 2 Global interrupt flag    */
#define  DMA_ISR_TCIF2                       ((uint32_t)0x00000020U)        /*!< Channel 2 Transfer Complete flag   */
#define  DMA_ISR_HTIF2                       ((uint32_t)0x00000040U)        /*!< Channel 2 Half Transfer flag       */
#define  DMA_ISR_TEIF2                       ((uint32_t)0x00000080U)        /*!< Channel 2 Transfer Error flag      */
#define  DMA_ISR_GIF3                        ((uint32_t)0x00000100U)        /*!< Channel 3 Global interrupt flag    */
#define  DMA_ISR_TCIF3                       ((uint32_t)0x00000200U)        /*!< Channel 3 Transfer Complete flag   */
#define  DMA_ISR_HTIF3                       ((uint32_t)0x00000400U)        /*!< Channel 3 Half Transfer flag       */
#define  DMA_ISR_TEIF3                       ((uint32_t)0x00000800U)        /*!< Channel 3 Transfer Error flag      */
#define  DMA_ISR_GIF4                        ((uint32_t)0x00001000U)        /*!< Channel 4 Global interrupt flag    */
#define  DMA_ISR_TCIF4                       ((uint32_t)0x00002000U)        /*!< Channel 4 Transfer Complete flag   */
#define  DMA_ISR_HTIF4                       ((uint32_t)0x00004000U)        /*!< Channel 4 Half Transfer flag       */
#define  DMA_ISR_TEIF4                       ((uint32_t)0x00008000U)        /*!< Channel 4 Transfer Error flag      */
#define  DMA_ISR_GIF5                        ((uint32_t)0x00010000U)        /*!< Channel 5 Global interrupt flag    */
#define  DMA_ISR_TCIF5                       ((uint32_t)0x00020000U)        /*!< Channel 5 Transfer Complete flag   */
#define  DMA_ISR_HTIF5                       ((uint32_t)0x00040000U)        /*!< Channel 5 Half Transfer flag       */
#define  DMA_ISR_TEIF5                       ((uint32_t)0x00080000U)        /*!< Channel 5 Transfer Error flag      */

/*******************  Bit definition for DMA_IFCR register  *******************/
#define  DMA_IFCR_CGIF1                      ((uint32_t)0x00000001U)        /*!< Channel 1 Global interrupt clear    */
#define  DMA_IFCR_CTCIF1                     ((uint32_t)0x00000002U)        /*!< Channel 1 Transfer Complete clear   */
#define  DMA_IFCR_CHTIF1                     ((uint32_t)0x00000004U)        /*!< Channel 1 Half Transfer clear       */
#define  DMA_IFCR_CTEIF1                     ((uint32_t)0x00000008U)        /*!< Channel 1 Transfer Error clear      */
#define  DMA_IFCR_CGIF2                      ((uint32_t)0x00000010U)        /*!< Channel 2 Global interrupt clear    */
#define  DMA_IFCR_CTCIF2                     ((uint32_t)0x00000020U)        /*!< Channel 2 Transfer Complete clear   */
#define  DMA_IFCR_CHTIF2                     ((uint32_t)0x00000040U)        /*!< Channel 2 Half Transfer clear       */
#define  DMA_IFCR_CTEIF2                     ((uint32_t)0x00000080U)        /*!< Channel 2 Transfer Error clear      */
#define  DMA_IFCR_CGIF3                      ((uint32_t)0x00000100U)        /*!< Channel 3 Global interrupt clear    */
#define  DMA_IFCR_CTCIF3                     ((uint32_t)0x00000200U)        /*!< Channel 3 Transfer Complete clear   */
#define  DMA_IFCR_CHTIF3                     ((uint32_t)0x00000400U)        /*!< Channel 3 Half Transfer clear       */
#define  DMA_IFCR_CTEIF3                     ((uint32_t)0x00000800U)        /*!< Channel 3 Transfer Error clear      */
#define  DMA_IFCR_CGIF4                      ((uint32_t)0x00001000U)        /*!< Channel 4 Global interrupt clear    */
#define  DMA_IFCR_CTCIF4                     ((uint32_t)0x00002000U)        /*!< Channel 4 Transfer Complete clear   */
#define  DMA_IFCR_CHTIF4                     ((uint32_t)0x00004000U)        /*!< Channel 4 Half Transfer clear       */
#define  DMA_IFCR_CTEIF4                     ((uint32_t)0x00008000U)        /*!< Channel 4 Transfer Error clear      */
#define  DMA_IFCR_CGIF5                      ((uint32_t)0x00010000U)        /*!< Channel 5 Global interrupt clear    */
#define  DMA_IFCR_CTCIF5                     ((uint32_t)0x00020000U)        /*!< Channel 5 Transfer Complete clear   */
#define  DMA_IFCR_CHTIF5                     ((uint32_t)0x00040000U)        /*!< Channel 5 Half Transfer clear       */
#define  DMA_IFCR_CTEIF5                     ((uint32_t)0x00080000U)        /*!< Channel 5 Transfer Error clear      */

/*******************  Bit definition for DMA_CCR register  ********************/
#define  DMA_CCR_EN                          ((uint32_t)0x00000001U)        /*!< Channel enable                      */
#define  DMA_CCR_TCIE                        ((uint32_t)0x00000002U)        /*!< Transfer complete interrupt enable  */
#define  DMA_CCR_HTIE                        ((uint32_t)0x00000004U)        /*!< Half Transfer interrupt enable      */
#define  DMA_CCR_TEIE                        ((uint32_t)0x00000008U)        /*!< Transfer error interrupt enable     */
#define  DMA_CCR_DIR                         ((uint32_t)0x00000010U)        /*!< Data transfer direction             */
#define  DMA_CCR_CIRC                        ((uint32_t)0x00000020U)        /*!< Circular mode                       */
#define  DMA_CCR_PINC                        ((uint32_t)0x00000040U)        /*!< Peripheral increment mode           */
#define  DMA_CCR_MINC                        ((uint32_t)0x00000080U)        /*!< Memory increment mode               */

#define  DMA_CCR_PSIZE                       ((uint32_t)0x00000300U)        /*!< PSIZE[1:0] bits (Peripheral size)   */
#define  DMA_CCR_PSIZE_0                     ((uint32_t)0x00000100U)        /*!< Bit 0                               */
#define  DMA_CCR_PSIZE_1                     ((uint32_t)0x00000200U)        /*!< Bit 1                               */

#define  DMA_CCR_MSIZE                       ((uint32_t)0x00000C00U)        /*!< MSIZE[1:0] bits (Memory size)       */
#define  DMA_CCR_MSIZE_0                     ((uint32_t)0x00000400U)        /*!< Bit 0                               */
#define  DMA_CCR_MSIZE_1                     ((uint32_t)0x00000800U)        /*!< Bit 1                               */

#define  DMA_CCR_PL                          ((uint32_t)0x00003000U)        /*!< PL[1:0] bits(Channel Priority level)*/
#define  DMA_CCR_PL_0                        ((uint32_t)0x00001000U)        /*!< Bit 0                               */
#define  DMA_CCR_PL_1                        ((uint32_t)0x00002000U)        /*!< Bit 1                               */

#define  DMA_CCR_MEM2MEM                     ((uint32_t)0x00004000U)        /*!< Memory to memory mode               */

/******************  Bit definition for DMA_CNDTR register  *******************/
#define  DMA_CNDTR_NDT                       ((uint32_t)0x0000FFFFU)        /*!< Number of data to Transfer          */

/******************  Bit definition for DMA_CPAR register  ********************/
#define  DMA_CPAR_PA                         ((uint32_t)0xFFFFFFFFU)        /*!< Peripheral Address                  */

/******************  Bit definition for DMA_CMAR register  ********************/
#define  DMA_CMAR_MA                         ((uint32_t)0xFFFFFFFFU)        /*!< Memory Address                      */

/******************************************************************************/
/*                                                                            */
/*                 External Interrupt/Event Controller (EXTI)                 */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for EXTI_IMR register  *******************/
#define  EXTI_IMR_MR0                        ((uint32_t)0x00000001U)        /*!< Interrupt Mask on line 0  */
#define  EXTI_IMR_MR1                        ((uint32_t)0x00000002U)        /*!< Interrupt Mask on line 1  */
#define  EXTI_IMR_MR2                        ((uint32_t)0x00000004U)        /*!< Interrupt Mask on line 2  */
#define  EXTI_IMR_MR3                        ((uint32_t)0x00000008U)        /*!< Interrupt Mask on line 3  */
#define  EXTI_IMR_MR4                        ((uint32_t)0x00000010U)        /*!< Interrupt Mask on line 4  */
#define  EXTI_IMR_MR5                        ((uint32_t)0x00000020U)        /*!< Interrupt Mask on line 5  */
#define  EXTI_IMR_MR6                        ((uint32_t)0x00000040U)        /*!< Interrupt Mask on line 6  */
#define  EXTI_IMR_MR7                        ((uint32_t)0x00000080U)        /*!< Interrupt Mask on line 7  */
#define  EXTI_IMR_MR8                        ((uint32_t)0x00000100U)        /*!< Interrupt Mask on line 8  */
#define  EXTI_IMR_MR9                        ((uint32_t)0x00000200U)        /*!< Interrupt Mask on line 9  */
#define  EXTI_IMR_MR10                       ((uint32_t)0x00000400U)        /*!< Interrupt Mask on line 10 */
#define  EXTI_IMR_MR11                       ((uint32_t)0x00000800U)        /*!< Interrupt Mask on line 11 */
#define  EXTI_IMR_MR12                       ((uint32_t)0x00001000U)        /*!< Interrupt Mask on line 12 */
#define  EXTI_IMR_MR13                       ((uint32_t)0x00002000U)        /*!< Interrupt Mask on line 13 */
#define  EXTI_IMR_MR14                       ((uint32_t)0x00004000U)        /*!< Interrupt Mask on line 14 */
#define  EXTI_IMR_MR15                       ((uint32_t)0x00008000U)        /*!< Interrupt Mask on line 15 */
#define  EXTI_IMR_MR16                       ((uint32_t)0x00010000U)        /*!< Interrupt Mask on line 16 */
#define  EXTI_IMR_MR17                       ((uint32_t)0x00020000U)        /*!< Interrupt Mask on line 17 */
#define  EXTI_IMR_MR18                       ((uint32_t)0x00040000U)        /*!< Interrupt Mask on line 18 */
#define  EXTI_IMR_MR19                       ((uint32_t)0x00080000U)        /*!< Interrupt Mask on line 19 */
#define  EXTI_IMR_MR21                       ((uint32_t)0x00200000U)        /*!< Interrupt Mask on line 21 */
#define  EXTI_IMR_MR22                       ((uint32_t)0x00400000U)        /*!< Interrupt Mask on line 22 */
#define  EXTI_IMR_MR23                       ((uint32_t)0x00800000U)        /*!< Interrupt Mask on line 23 */
#define  EXTI_IMR_MR25                       ((uint32_t)0x02000000U)        /*!< Interrupt Mask on line 25 */
#define  EXTI_IMR_MR27                       ((uint32_t)0x08000000U)        /*!< Interrupt Mask on line 27 */

/* References Defines */
#define  EXTI_IMR_IM0 EXTI_IMR_MR0
#define  EXTI_IMR_IM1 EXTI_IMR_MR1
#define  EXTI_IMR_IM2 EXTI_IMR_MR2
#define  EXTI_IMR_IM3 EXTI_IMR_MR3
#define  EXTI_IMR_IM4 EXTI_IMR_MR4
#define  EXTI_IMR_IM5 EXTI_IMR_MR5
#define  EXTI_IMR_IM6 EXTI_IMR_MR6
#define  EXTI_IMR_IM7 EXTI_IMR_MR7
#define  EXTI_IMR_IM8 EXTI_IMR_MR8
#define  EXTI_IMR_IM9 EXTI_IMR_MR9
#define  EXTI_IMR_IM10 EXTI_IMR_MR10
#define  EXTI_IMR_IM11 EXTI_IMR_MR11
#define  EXTI_IMR_IM12 EXTI_IMR_MR12
#define  EXTI_IMR_IM13 EXTI_IMR_MR13
#define  EXTI_IMR_IM14 EXTI_IMR_MR14
#define  EXTI_IMR_IM15 EXTI_IMR_MR15
#define  EXTI_IMR_IM16 EXTI_IMR_MR16
#define  EXTI_IMR_IM17 EXTI_IMR_MR17
#define  EXTI_IMR_IM18 EXTI_IMR_MR18
#define  EXTI_IMR_IM19 EXTI_IMR_MR19
#define  EXTI_IMR_IM21 EXTI_IMR_MR21
#define  EXTI_IMR_IM22 EXTI_IMR_MR22
#define  EXTI_IMR_IM23 EXTI_IMR_MR23
#define  EXTI_IMR_IM25 EXTI_IMR_MR25
#define  EXTI_IMR_IM27 EXTI_IMR_MR27

/******************  Bit definition for EXTI_EMR register  ********************/
#define  EXTI_EMR_MR0                        ((uint32_t)0x00000001U)        /*!< Event Mask on line 0  */
#define  EXTI_EMR_MR1                        ((uint32_t)0x00000002U)        /*!< Event Mask on line 1  */
#define  EXTI_EMR_MR2                        ((uint32_t)0x00000004U)        /*!< Event Mask on line 2  */
#define  EXTI_EMR_MR3                        ((uint32_t)0x00000008U)        /*!< Event Mask on line 3  */
#define  EXTI_EMR_MR4                        ((uint32_t)0x00000010U)        /*!< Event Mask on line 4  */
#define  EXTI_EMR_MR5                        ((uint32_t)0x00000020U)        /*!< Event Mask on line 5  */
#define  EXTI_EMR_MR6                        ((uint32_t)0x00000040U)        /*!< Event Mask on line 6  */
#define  EXTI_EMR_MR7                        ((uint32_t)0x00000080U)        /*!< Event Mask on line 7  */
#define  EXTI_EMR_MR8                        ((uint32_t)0x00000100U)        /*!< Event Mask on line 8  */
#define  EXTI_EMR_MR9                        ((uint32_t)0x00000200U)        /*!< Event Mask on line 9  */
#define  EXTI_EMR_MR10                       ((uint32_t)0x00000400U)        /*!< Event Mask on line 10 */
#define  EXTI_EMR_MR11                       ((uint32_t)0x00000800U)        /*!< Event Mask on line 11 */
#define  EXTI_EMR_MR12                       ((uint32_t)0x00001000U)        /*!< Event Mask on line 12 */
#define  EXTI_EMR_MR13                       ((uint32_t)0x00002000U)        /*!< Event Mask on line 13 */
#define  EXTI_EMR_MR14                       ((uint32_t)0x00004000U)        /*!< Event Mask on line 14 */
#define  EXTI_EMR_MR15                       ((uint32_t)0x00008000U)        /*!< Event Mask on line 15 */
#define  EXTI_EMR_MR16                       ((uint32_t)0x00010000U)        /*!< Event Mask on line 16 */
#define  EXTI_EMR_MR17                       ((uint32_t)0x00020000U)        /*!< Event Mask on line 17 */
#define  EXTI_EMR_MR18                       ((uint32_t)0x00040000U)        /*!< Event Mask on line 18 */
#define  EXTI_EMR_MR19                       ((uint32_t)0x00080000U)        /*!< Event Mask on line 19 */
#define  EXTI_EMR_MR21                       ((uint32_t)0x00200000U)        /*!< Event Mask on line 21 */
#define  EXTI_EMR_MR22                       ((uint32_t)0x00400000U)        /*!< Event Mask on line 22 */
#define  EXTI_EMR_MR23                       ((uint32_t)0x00800000U)        /*!< Event Mask on line 23 */
#define  EXTI_EMR_MR25                       ((uint32_t)0x02000000U)        /*!< Event Mask on line 25 */
#define  EXTI_EMR_MR27                       ((uint32_t)0x08000000U)        /*!< Event Mask on line 27 */

/* References Defines */
#define  EXTI_EMR_EM0 EXTI_EMR_MR0
#define  EXTI_EMR_EM1 EXTI_EMR_MR1
#define  EXTI_EMR_EM2 EXTI_EMR_MR2
#define  EXTI_EMR_EM3 EXTI_EMR_MR3
#define  EXTI_EMR_EM4 EXTI_EMR_MR4
#define  EXTI_EMR_EM5 EXTI_EMR_MR5
#define  EXTI_EMR_EM6 EXTI_EMR_MR6
#define  EXTI_EMR_EM7 EXTI_EMR_MR7
#define  EXTI_EMR_EM8 EXTI_EMR_MR8
#define  EXTI_EMR_EM9 EXTI_EMR_MR9
#define  EXTI_EMR_EM10 EXTI_EMR_MR10
#define  EXTI_EMR_EM11 EXTI_EMR_MR11
#define  EXTI_EMR_EM12 EXTI_EMR_MR12
#define  EXTI_EMR_EM13 EXTI_EMR_MR13
#define  EXTI_EMR_EM14 EXTI_EMR_MR14
#define  EXTI_EMR_EM15 EXTI_EMR_MR15
#define  EXTI_EMR_EM16 EXTI_EMR_MR16
#define  EXTI_EMR_EM17 EXTI_EMR_MR17
#define  EXTI_EMR_EM18 EXTI_EMR_MR18
#define  EXTI_EMR_EM19 EXTI_EMR_MR19
#define  EXTI_EMR_EM21 EXTI_EMR_MR21
#define  EXTI_EMR_EM22 EXTI_EMR_MR22
#define  EXTI_EMR_EM23 EXTI_EMR_MR23
#define  EXTI_EMR_EM25 EXTI_EMR_MR25
#define  EXTI_EMR_EM27 EXTI_EMR_MR27

/*******************  Bit definition for EXTI_RTSR register  ******************/
#define  EXTI_RTSR_TR0                       ((uint32_t)0x00000001U)        /*!< Rising trigger event configuration bit of line 0 */
#define  EXTI_RTSR_TR1                       ((uint32_t)0x00000002U)        /*!< Rising trigger event configuration bit of line 1 */
#define  EXTI_RTSR_TR2                       ((uint32_t)0x00000004U)        /*!< Rising trigger event configuration bit of line 2 */
#define  EXTI_RTSR_TR3                       ((uint32_t)0x00000008U)        /*!< Rising trigger event configuration bit of line 3 */
#define  EXTI_RTSR_TR4                       ((uint32_t)0x00000010U)        /*!< Rising trigger event configuration bit of line 4 */
#define  EXTI_RTSR_TR5                       ((uint32_t)0x00000020U)        /*!< Rising trigger event configuration bit of line 5 */
#define  EXTI_RTSR_TR6                       ((uint32_t)0x00000040U)        /*!< Rising trigger event configuration bit of line 6 */
#define  EXTI_RTSR_TR7                       ((uint32_t)0x00000080U)        /*!< Rising trigger event configuration bit of line 7 */
#define  EXTI_RTSR_TR8                       ((uint32_t)0x00000100U)        /*!< Rising trigger event configuration bit of line 8 */
#define  EXTI_RTSR_TR9                       ((uint32_t)0x00000200U)        /*!< Rising trigger event configuration bit of line 9 */
#define  EXTI_RTSR_TR10                      ((uint32_t)0x00000400U)        /*!< Rising trigger event configuration bit of line 10 */
#define  EXTI_RTSR_TR11                      ((uint32_t)0x00000800U)        /*!< Rising trigger event configuration bit of line 11 */
#define  EXTI_RTSR_TR12                      ((uint32_t)0x00001000U)        /*!< Rising trigger event configuration bit of line 12 */
#define  EXTI_RTSR_TR13                      ((uint32_t)0x00002000U)        /*!< Rising trigger event configuration bit of line 13 */
#define  EXTI_RTSR_TR14                      ((uint32_t)0x00004000U)        /*!< Rising trigger event configuration bit of line 14 */
#define  EXTI_RTSR_TR15                      ((uint32_t)0x00008000U)        /*!< Rising trigger event configuration bit of line 15 */
#define  EXTI_RTSR_TR16                      ((uint32_t)0x00010000U)        /*!< Rising trigger event configuration bit of line 16 */
#define  EXTI_RTSR_TR17                      ((uint32_t)0x00020000U)        /*!< Rising trigger event configuration bit of line 17 */
#define  EXTI_RTSR_TR19                      ((uint32_t)0x00080000U)        /*!< Rising trigger event configuration bit of line 19 */
#define  EXTI_RTSR_TR21                      ((uint32_t)0x00200000U)        /*!< Rising trigger event configuration bit of line 21 */
#define  EXTI_RTSR_TR22                      ((uint32_t)0x00400000U)        /*!< Rising trigger event configuration bit of line 22 */

/* References Defines */
#define EXTI_RTSR_RT0 EXTI_RTSR_TR0
#define EXTI_RTSR_RT1 EXTI_RTSR_TR1
#define EXTI_RTSR_RT2 EXTI_RTSR_TR2
#define EXTI_RTSR_RT3 EXTI_RTSR_TR3
#define EXTI_RTSR_RT4 EXTI_RTSR_TR4
#define EXTI_RTSR_RT5 EXTI_RTSR_TR5
#define EXTI_RTSR_RT6 EXTI_RTSR_TR6
#define EXTI_RTSR_RT7 EXTI_RTSR_TR7
#define EXTI_RTSR_RT8 EXTI_RTSR_TR8
#define EXTI_RTSR_RT9 EXTI_RTSR_TR9
#define EXTI_RTSR_RT10 EXTI_RTSR_TR10
#define EXTI_RTSR_RT11 EXTI_RTSR_TR11
#define EXTI_RTSR_RT12 EXTI_RTSR_TR12
#define EXTI_RTSR_RT13 EXTI_RTSR_TR13
#define EXTI_RTSR_RT14 EXTI_RTSR_TR14
#define EXTI_RTSR_RT15 EXTI_RTSR_TR15
#define EXTI_RTSR_RT16 EXTI_RTSR_TR16
#define EXTI_RTSR_RT17 EXTI_RTSR_TR17
#define EXTI_RTSR_RT19 EXTI_RTSR_TR19
#define EXTI_RTSR_RT21 EXTI_RTSR_TR21
#define EXTI_RTSR_RT22 EXTI_RTSR_TR22

/*******************  Bit definition for EXTI_FTSR register *******************/
#define  EXTI_FTSR_TR0                       ((uint32_t)0x00000001U)        /*!< Falling trigger event configuration bit of line 0 */
#define  EXTI_FTSR_TR1                       ((uint32_t)0x00000002U)        /*!< Falling trigger event configuration bit of line 1 */
#define  EXTI_FTSR_TR2                       ((uint32_t)0x00000004U)        /*!< Falling trigger event configuration bit of line 2 */
#define  EXTI_FTSR_TR3                       ((uint32_t)0x00000008U)        /*!< Falling trigger event configuration bit of line 3 */
#define  EXTI_FTSR_TR4                       ((uint32_t)0x00000010U)        /*!< Falling trigger event configuration bit of line 4 */
#define  EXTI_FTSR_TR5                       ((uint32_t)0x00000020U)        /*!< Falling trigger event configuration bit of line 5 */
#define  EXTI_FTSR_TR6                       ((uint32_t)0x00000040U)        /*!< Falling trigger event configuration bit of line 6 */
#define  EXTI_FTSR_TR7                       ((uint32_t)0x00000080U)        /*!< Falling trigger event configuration bit of line 7 */
#define  EXTI_FTSR_TR8                       ((uint32_t)0x00000100U)        /*!< Falling trigger event configuration bit of line 8 */
#define  EXTI_FTSR_TR9                       ((uint32_t)0x00000200U)        /*!< Falling trigger event configuration bit of line 9 */
#define  EXTI_FTSR_TR10                      ((uint32_t)0x00000400U)        /*!< Falling trigger event configuration bit of line 10 */
#define  EXTI_FTSR_TR11                      ((uint32_t)0x00000800U)        /*!< Falling trigger event configuration bit of line 11 */
#define  EXTI_FTSR_TR12                      ((uint32_t)0x00001000U)        /*!< Falling trigger event configuration bit of line 12 */
#define  EXTI_FTSR_TR13                      ((uint32_t)0x00002000U)        /*!< Falling trigger event configuration bit of line 13 */
#define  EXTI_FTSR_TR14                      ((uint32_t)0x00004000U)        /*!< Falling trigger event configuration bit of line 14 */
#define  EXTI_FTSR_TR15                      ((uint32_t)0x00008000U)        /*!< Falling trigger event configuration bit of line 15 */
#define  EXTI_FTSR_TR16                      ((uint32_t)0x00010000U)        /*!< Falling trigger event configuration bit of line 16 */
#define  EXTI_FTSR_TR17                      ((uint32_t)0x00020000U)        /*!< Falling trigger event configuration bit of line 17 */
#define  EXTI_FTSR_TR19                      ((uint32_t)0x00080000U)        /*!< Falling trigger event configuration bit of line 19 */
#define  EXTI_FTSR_TR21                      ((uint32_t)0x00200000U)        /*!< Falling trigger event configuration bit of line 21 */
#define  EXTI_FTSR_TR22                      ((uint32_t)0x00400000U)        /*!< Falling trigger event configuration bit of line 22 */

/* References Defines */
#define EXTI_FTSR_FT0 EXTI_FTSR_TR0
#define EXTI_FTSR_FT1 EXTI_FTSR_TR1
#define EXTI_FTSR_FT2 EXTI_FTSR_TR2
#define EXTI_FTSR_FT3 EXTI_FTSR_TR3
#define EXTI_FTSR_FT4 EXTI_FTSR_TR4
#define EXTI_FTSR_FT5 EXTI_FTSR_TR5
#define EXTI_FTSR_FT6 EXTI_FTSR_TR6
#define EXTI_FTSR_FT7 EXTI_FTSR_TR7
#define EXTI_FTSR_FT8 EXTI_FTSR_TR8
#define EXTI_FTSR_FT9 EXTI_FTSR_TR9
#define EXTI_FTSR_FT10 EXTI_FTSR_TR10
#define EXTI_FTSR_FT11 EXTI_FTSR_TR11
#define EXTI_FTSR_FT12 EXTI_FTSR_TR12
#define EXTI_FTSR_FT13 EXTI_FTSR_TR13
#define EXTI_FTSR_FT14 EXTI_FTSR_TR14
#define EXTI_FTSR_FT15 EXTI_FTSR_TR15
#define EXTI_FTSR_FT16 EXTI_FTSR_TR16
#define EXTI_FTSR_FT17 EXTI_FTSR_TR17
#define EXTI_FTSR_FT19 EXTI_FTSR_TR19
#define EXTI_FTSR_FT21 EXTI_FTSR_TR21
#define EXTI_FTSR_FT22 EXTI_FTSR_TR22

/******************* Bit definition for EXTI_SWIER register *******************/
#define  EXTI_SWIER_SWIER0                   ((uint32_t)0x00000001U)        /*!< Software Interrupt on line 0  */
#define  EXTI_SWIER_SWIER1                   ((uint32_t)0x00000002U)        /*!< Software Interrupt on line 1  */
#define  EXTI_SWIER_SWIER2                   ((uint32_t)0x00000004U)        /*!< Software Interrupt on line 2  */
#define  EXTI_SWIER_SWIER3                   ((uint32_t)0x00000008U)        /*!< Software Interrupt on line 3  */
#define  EXTI_SWIER_SWIER4                   ((uint32_t)0x00000010U)        /*!< Software Interrupt on line 4  */
#define  EXTI_SWIER_SWIER5                   ((uint32_t)0x00000020U)        /*!< Software Interrupt on line 5  */
#define  EXTI_SWIER_SWIER6                   ((uint32_t)0x00000040U)        /*!< Software Interrupt on line 6  */
#define  EXTI_SWIER_SWIER7                   ((uint32_t)0x00000080U)        /*!< Software Interrupt on line 7  */
#define  EXTI_SWIER_SWIER8                   ((uint32_t)0x00000100U)        /*!< Software Interrupt on line 8  */
#define  EXTI_SWIER_SWIER9                   ((uint32_t)0x00000200U)        /*!< Software Interrupt on line 9  */
#define  EXTI_SWIER_SWIER10                  ((uint32_t)0x00000400U)        /*!< Software Interrupt on line 10 */
#define  EXTI_SWIER_SWIER11                  ((uint32_t)0x00000800U)        /*!< Software Interrupt on line 11 */
#define  EXTI_SWIER_SWIER12                  ((uint32_t)0x00001000U)        /*!< Software Interrupt on line 12 */
#define  EXTI_SWIER_SWIER13                  ((uint32_t)0x00002000U)        /*!< Software Interrupt on line 13 */
#define  EXTI_SWIER_SWIER14                  ((uint32_t)0x00004000U)        /*!< Software Interrupt on line 14 */
#define  EXTI_SWIER_SWIER15                  ((uint32_t)0x00008000U)        /*!< Software Interrupt on line 15 */
#define  EXTI_SWIER_SWIER16                  ((uint32_t)0x00010000U)        /*!< Software Interrupt on line 16 */
#define  EXTI_SWIER_SWIER17                  ((uint32_t)0x00020000U)        /*!< Software Interrupt on line 17 */
#define  EXTI_SWIER_SWIER19                  ((uint32_t)0x00080000U)        /*!< Software Interrupt on line 19 */
#define  EXTI_SWIER_SWIER21                  ((uint32_t)0x00200000U)        /*!< Software Interrupt on line 21 */
#define  EXTI_SWIER_SWIER22                  ((uint32_t)0x00400000U)        /*!< Software Interrupt on line 22 */

/* References Defines */
#define EXTI_SWIER_SWI0 EXTI_SWIER_SWIER0
#define EXTI_SWIER_SWI1 EXTI_SWIER_SWIER1
#define EXTI_SWIER_SWI2 EXTI_SWIER_SWIER2
#define EXTI_SWIER_SWI3 EXTI_SWIER_SWIER3
#define EXTI_SWIER_SWI4 EXTI_SWIER_SWIER4
#define EXTI_SWIER_SWI5 EXTI_SWIER_SWIER5
#define EXTI_SWIER_SWI6 EXTI_SWIER_SWIER6
#define EXTI_SWIER_SWI7 EXTI_SWIER_SWIER7
#define EXTI_SWIER_SWI8 EXTI_SWIER_SWIER8
#define EXTI_SWIER_SWI9 EXTI_SWIER_SWIER9
#define EXTI_SWIER_SWI10 EXTI_SWIER_SWIER10
#define EXTI_SWIER_SWI11 EXTI_SWIER_SWIER11
#define EXTI_SWIER_SWI12 EXTI_SWIER_SWIER12
#define EXTI_SWIER_SWI13 EXTI_SWIER_SWIER13
#define EXTI_SWIER_SWI14 EXTI_SWIER_SWIER14
#define EXTI_SWIER_SWI15 EXTI_SWIER_SWIER15
#define EXTI_SWIER_SWI16 EXTI_SWIER_SWIER16
#define EXTI_SWIER_SWI17 EXTI_SWIER_SWIER17
#define EXTI_SWIER_SWI19 EXTI_SWIER_SWIER19
#define EXTI_SWIER_SWI21 EXTI_SWIER_SWIER21
#define EXTI_SWIER_SWI22 EXTI_SWIER_SWIER22

/******************  Bit definition for EXTI_PR register  *********************/
#define  EXTI_PR_PR0                         ((uint32_t)0x00000001U)        /*!< Pending bit 0  */
#define  EXTI_PR_PR1                         ((uint32_t)0x00000002U)        /*!< Pending bit 1  */
#define  EXTI_PR_PR2                         ((uint32_t)0x00000004U)        /*!< Pending bit 2  */
#define  EXTI_PR_PR3                         ((uint32_t)0x00000008U)        /*!< Pending bit 3  */
#define  EXTI_PR_PR4                         ((uint32_t)0x00000010U)        /*!< Pending bit 4  */
#define  EXTI_PR_PR5                         ((uint32_t)0x00000020U)        /*!< Pending bit 5  */
#define  EXTI_PR_PR6                         ((uint32_t)0x00000040U)        /*!< Pending bit 6  */
#define  EXTI_PR_PR7                         ((uint32_t)0x00000080U)        /*!< Pending bit 7  */
#define  EXTI_PR_PR8                         ((uint32_t)0x00000100U)        /*!< Pending bit 8  */
#define  EXTI_PR_PR9                         ((uint32_t)0x00000200U)        /*!< Pending bit 9  */
#define  EXTI_PR_PR10                        ((uint32_t)0x00000400U)        /*!< Pending bit 10 */
#define  EXTI_PR_PR11                        ((uint32_t)0x00000800U)        /*!< Pending bit 11 */
#define  EXTI_PR_PR12                        ((uint32_t)0x00001000U)        /*!< Pending bit 12 */
#define  EXTI_PR_PR13                        ((uint32_t)0x00002000U)        /*!< Pending bit 13 */
#define  EXTI_PR_PR14                        ((uint32_t)0x00004000U)        /*!< Pending bit 14 */
#define  EXTI_PR_PR15                        ((uint32_t)0x00008000U)        /*!< Pending bit 15 */
#define  EXTI_PR_PR16                        ((uint32_t)0x00010000U)        /*!< Pending bit 16 */
#define  EXTI_PR_PR17                        ((uint32_t)0x00020000U)        /*!< Pending bit 17 */
#define  EXTI_PR_PR19                        ((uint32_t)0x00080000U)        /*!< Pending bit 19 */
#define  EXTI_PR_PR21                        ((uint32_t)0x00200000U)        /*!< Pending bit 21 */
#define  EXTI_PR_PR22                        ((uint32_t)0x00400000U)        /*!< Pending bit 22 */

/* References Defines */
#define EXTI_PR_PIF0 EXTI_PR_PR0
#define EXTI_PR_PIF1 EXTI_PR_PR1
#define EXTI_PR_PIF2 EXTI_PR_PR2
#define EXTI_PR_PIF3 EXTI_PR_PR3
#define EXTI_PR_PIF4 EXTI_PR_PR4
#define EXTI_PR_PIF5 EXTI_PR_PR5
#define EXTI_PR_PIF6 EXTI_PR_PR6
#define EXTI_PR_PIF7 EXTI_PR_PR7
#define EXTI_PR_PIF8 EXTI_PR_PR8
#define EXTI_PR_PIF9 EXTI_PR_PR9
#define EXTI_PR_PIF10 EXTI_PR_PR10
#define EXTI_PR_PIF11 EXTI_PR_PR11
#define EXTI_PR_PIF12 EXTI_PR_PR12
#define EXTI_PR_PIF13 EXTI_PR_PR13
#define EXTI_PR_PIF14 EXTI_PR_PR14
#define EXTI_PR_PIF15 EXTI_PR_PR15
#define EXTI_PR_PIF16 EXTI_PR_PR16
#define EXTI_PR_PIF17 EXTI_PR_PR17
#define EXTI_PR_PIF19 EXTI_PR_PR19
#define EXTI_PR_PIF21 EXTI_PR_PR21
#define EXTI_PR_PIF22 EXTI_PR_PR22

/******************************************************************************/
/*                                                                            */
/*                      FLASH and Option Bytes Registers                      */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for FLASH_ACR register  ******************/
#define  FLASH_ACR_LATENCY                   ((uint32_t)0x00000001U)        /*!< LATENCY bit (Latency) */

#define  FLASH_ACR_PRFTBE                    ((uint32_t)0x00000010U)        /*!< Prefetch Buffer Enable */
#define  FLASH_ACR_PRFTBS                    ((uint32_t)0x00000020U)        /*!< Prefetch Buffer Status */

/******************  Bit definition for FLASH_KEYR register  ******************/
#define  FLASH_KEYR_FKEYR                    ((uint32_t)0xFFFFFFFFU)        /*!< FPEC Key */

/*****************  Bit definition for FLASH_OPTKEYR register  ****************/
#define  FLASH_OPTKEYR_OPTKEYR               ((uint32_t)0xFFFFFFFFU)        /*!< Option Byte Key */

/******************  FLASH Keys  **********************************************/
#define FLASH_KEY1                           ((uint32_t)0x45670123U)        /*!< Flash program erase key1 */
#define FLASH_KEY2                           ((uint32_t)0xCDEF89ABU)        /*!< Flash program erase key2: used with FLASH_PEKEY1
                                                                                to unlock the write access to the FPEC. */
                                                               
#define FLASH_OPTKEY1                        ((uint32_t)0x45670123U)        /*!< Flash option key1 */
#define FLASH_OPTKEY2                        ((uint32_t)0xCDEF89ABU)        /*!< Flash option key2: used with FLASH_OPTKEY1 to
                                                                                unlock the write access to the option byte block */

/******************  Bit definition for FLASH_SR register  *******************/
#define  FLASH_SR_BSY                        ((uint32_t)0x00000001U)        /*!< Busy */
#define  FLASH_SR_PGERR                      ((uint32_t)0x00000004U)        /*!< Programming Error */
#define  FLASH_SR_WRPRTERR                   ((uint32_t)0x00000010U)        /*!< Write Protection Error */
#define  FLASH_SR_EOP                        ((uint32_t)0x00000020U)        /*!< End of operation */
#define  FLASH_SR_WRPERR                     FLASH_SR_WRPRTERR             /*!< Legacy of Write Protection Error */

/*******************  Bit definition for FLASH_CR register  *******************/
#define  FLASH_CR_PG                         ((uint32_t)0x00000001U)        /*!< Programming */
#define  FLASH_CR_PER                        ((uint32_t)0x00000002U)        /*!< Page Erase */
#define  FLASH_CR_MER                        ((uint32_t)0x00000004U)        /*!< Mass Erase */
#define  FLASH_CR_OPTPG                      ((uint32_t)0x00000010U)        /*!< Option Byte Programming */
#define  FLASH_CR_OPTER                      ((uint32_t)0x00000020U)        /*!< Option Byte Erase */
#define  FLASH_CR_STRT                       ((uint32_t)0x00000040U)        /*!< Start */
#define  FLASH_CR_LOCK                       ((uint32_t)0x00000080U)        /*!< Lock */
#define  FLASH_CR_OPTWRE                     ((uint32_t)0x00000200U)        /*!< Option Bytes Write Enable */
#define  FLASH_CR_ERRIE                      ((uint32_t)0x00000400U)        /*!< Error Interrupt Enable */
#define  FLASH_CR_EOPIE                      ((uint32_t)0x00001000U)        /*!< End of operation interrupt enable */
#define  FLASH_CR_OBL_LAUNCH                 ((uint32_t)0x00002000U)        /*!< Option Bytes Loader Launch */

/*******************  Bit definition for FLASH_AR register  *******************/
#define  FLASH_AR_FAR                        ((uint32_t)0xFFFFFFFFU)        /*!< Flash Address */

/******************  Bit definition for FLASH_OBR register  *******************/
#define  FLASH_OBR_OPTERR                    ((uint32_t)0x00000001U)        /*!< Option Byte Error */
#define  FLASH_OBR_RDPRT1                    ((uint32_t)0x00000002U)        /*!< Read protection Level 1 */
#define  FLASH_OBR_RDPRT2                    ((uint32_t)0x00000004U)        /*!< Read protection Level 2 */

#define  FLASH_OBR_USER                      ((uint32_t)0x00007700U)        /*!< User Option Bytes */
#define  FLASH_OBR_IWDG_SW                   ((uint32_t)0x00000100U)        /*!< IWDG SW */
#define  FLASH_OBR_nRST_STOP                 ((uint32_t)0x00000200U)        /*!< nRST_STOP */
#define  FLASH_OBR_nRST_STDBY                ((uint32_t)0x00000400U)        /*!< nRST_STDBY */
#define  FLASH_OBR_nBOOT1                    ((uint32_t)0x00001000U)        /*!< nBOOT1 */
#define  FLASH_OBR_VDDA_MONITOR              ((uint32_t)0x00002000U)        /*!< VDDA power supply supervisor */
#define  FLASH_OBR_RAM_PARITY_CHECK          ((uint32_t)0x00004000U)        /*!< RAM parity check */
#define  FLASH_OBR_DATA0                     ((uint32_t)0x00FF0000U) /*!< Data0 */
#define  FLASH_OBR_DATA1                     ((uint32_t)0xFF000000U) /*!< Data1 */

/* Old BOOT1 bit definition, maintained for legacy purpose */
#define FLASH_OBR_BOOT1                      FLASH_OBR_nBOOT1

/* Old OBR_VDDA bit definition, maintained for legacy purpose */
#define FLASH_OBR_VDDA_ANALOG                FLASH_OBR_VDDA_MONITOR

/******************  Bit definition for FLASH_WRPR register  ******************/
#define  FLASH_WRPR_WRP                      ((uint32_t)0x0000FFFFU)        /*!< Write Protect */

/*----------------------------------------------------------------------------*/

/******************  Bit definition for OB_RDP register  **********************/
#define  OB_RDP_RDP                          ((uint32_t)0x000000FFU)        /*!< Read protection option byte */
#define  OB_RDP_nRDP                         ((uint32_t)0x0000FF00U)        /*!< Read protection complemented option byte */

/******************  Bit definition for OB_USER register  *********************/
#define  OB_USER_USER                        ((uint32_t)0x00FF0000U)        /*!< User option byte */
#define  OB_USER_nUSER                       ((uint32_t)0xFF000000U)        /*!< User complemented option byte */

/******************  Bit definition for OB_WRP0 register  *********************/
#define  OB_WRP0_WRP0                        ((uint32_t)0x000000FFU)        /*!< Flash memory write protection option bytes */
#define  OB_WRP0_nWRP0                       ((uint32_t)0x0000FF00U)        /*!< Flash memory write protection complemented option bytes */

/******************  Bit definition for OB_WRP1 register  *********************/
#define  OB_WRP1_WRP1                        ((uint32_t)0x00FF0000U)        /*!< Flash memory write protection option bytes */
#define  OB_WRP1_nWRP1                       ((uint32_t)0xFF000000U)        /*!< Flash memory write protection complemented option bytes */

/******************************************************************************/
/*                                                                            */
/*                       General Purpose IOs (GPIO)                           */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for GPIO_MODER register  *****************/
#define GPIO_MODER_MODER0          ((uint32_t)0x00000003U)
#define GPIO_MODER_MODER0_0        ((uint32_t)0x00000001U)
#define GPIO_MODER_MODER0_1        ((uint32_t)0x00000002U)
#define GPIO_MODER_MODER1          ((uint32_t)0x0000000CU)
#define GPIO_MODER_MODER1_0        ((uint32_t)0x00000004U)
#define GPIO_MODER_MODER1_1        ((uint32_t)0x00000008U)
#define GPIO_MODER_MODER2          ((uint32_t)0x00000030U)
#define GPIO_MODER_MODER2_0        ((uint32_t)0x00000010U)
#define GPIO_MODER_MODER2_1        ((uint32_t)0x00000020U)
#define GPIO_MODER_MODER3          ((uint32_t)0x000000C0U)
#define GPIO_MODER_MODER3_0        ((uint32_t)0x00000040U)
#define GPIO_MODER_MODER3_1        ((uint32_t)0x00000080U)
#define GPIO_MODER_MODER4          ((uint32_t)0x00000300U)
#define GPIO_MODER_MODER4_0        ((uint32_t)0x00000100U)
#define GPIO_MODER_MODER4_1        ((uint32_t)0x00000200U)
#define GPIO_MODER_MODER5          ((uint32_t)0x00000C00U)
#define GPIO_MODER_MODER5_0        ((uint32_t)0x00000400U)
#define GPIO_MODER_MODER5_1        ((uint32_t)0x00000800U)
#define GPIO_MODER_MODER6          ((uint32_t)0x00003000U)
#define GPIO_MODER_MODER6_0        ((uint32_t)0x00001000U)
#define GPIO_MODER_MODER6_1        ((uint32_t)0x00002000U)
#define GPIO_MODER_MODER7          ((uint32_t)0x0000C000U)
#define GPIO_MODER_MODER7_0        ((uint32_t)0x00004000U)
#define GPIO_MODER_MODER7_1        ((uint32_t)0x00008000U)
#define GPIO_MODER_MODER8          ((uint32_t)0x00030000U)
#define GPIO_MODER_MODER8_0        ((uint32_t)0x00010000U)
#define GPIO_MODER_MODER8_1        ((uint32_t)0x00020000U)
#define GPIO_MODER_MODER9          ((uint32_t)0x000C0000U)
#define GPIO_MODER_MODER9_0        ((uint32_t)0x00040000U)
#define GPIO_MODER_MODER9_1        ((uint32_t)0x00080000U)
#define GPIO_MODER_MODER10         ((uint32_t)0x00300000U)
#define GPIO_MODER_MODER10_0       ((uint32_t)0x00100000U)
#define GPIO_MODER_MODER10_1       ((uint32_t)0x00200000U)
#define GPIO_MODER_MODER11         ((uint32_t)0x00C00000U)
#define GPIO_MODER_MODER11_0       ((uint32_t)0x00400000U)
#define GPIO_MODER_MODER11_1       ((uint32_t)0x00800000U)
#define GPIO_MODER_MODER12         ((uint32_t)0x03000000U)
#define GPIO_MODER_MODER12_0       ((uint32_t)0x01000000U)
#define GPIO_MODER_MODER12_1       ((uint32_t)0x02000000U)
#define GPIO_MODER_MODER13         ((uint32_t)0x0C000000U)
#define GPIO_MODER_MODER13_0       ((uint32_t)0x04000000U)
#define GPIO_MODER_MODER13_1       ((uint32_t)0x08000000U)
#define GPIO_MODER_MODER14         ((uint32_t)0x30000000U)
#define GPIO_MODER_MODER14_0       ((uint32_t)0x10000000U)
#define GPIO_MODER_MODER14_1       ((uint32_t)0x20000000U)
#define GPIO_MODER_MODER15         ((uint32_t)0xC0000000U)
#define GPIO_MODER_MODER15_0       ((uint32_t)0x40000000U)
#define GPIO_MODER_MODER15_1       ((uint32_t)0x80000000U)

/******************  Bit definition for GPIO_OTYPER register  *****************/
#define GPIO_OTYPER_OT_0           ((uint32_t)0x00000001U)
#define GPIO_OTYPER_OT_1           ((uint32_t)0x00000002U)
#define GPIO_OTYPER_OT_2           ((uint32_t)0x00000004U)
#define GPIO_OTYPER_OT_3           ((uint32_t)0x00000008U)
#define GPIO_OTYPER_OT_4           ((uint32_t)0x00000010U)
#define GPIO_OTYPER_OT_5           ((uint32_t)0x00000020U)
#define GPIO_OTYPER_OT_6           ((uint32_t)0x00000040U)
#define GPIO_OTYPER_OT_7           ((uint32_t)0x00000080U)
#define GPIO_OTYPER_OT_8           ((uint32_t)0x00000100U)
#define GPIO_OTYPER_OT_9           ((uint32_t)0x00000200U)
#define GPIO_OTYPER_OT_10          ((uint32_t)0x00000400U)
#define GPIO_OTYPER_OT_11          ((uint32_t)0x00000800U)
#define GPIO_OTYPER_OT_12          ((uint32_t)0x00001000U)
#define GPIO_OTYPER_OT_13          ((uint32_t)0x00002000U)
#define GPIO_OTYPER_OT_14          ((uint32_t)0x00004000U)
#define GPIO_OTYPER_OT_15          ((uint32_t)0x00008000U)

/****************  Bit definition for GPIO_OSPEEDR register  ******************/
#define GPIO_OSPEEDR_OSPEEDR0     ((uint32_t)0x00000003U)
#define GPIO_OSPEEDR_OSPEEDR0_0   ((uint32_t)0x00000001U)
#define GPIO_OSPEEDR_OSPEEDR0_1   ((uint32_t)0x00000002U)
#define GPIO_OSPEEDR_OSPEEDR1     ((uint32_t)0x0000000CU)
#define GPIO_OSPEEDR_OSPEEDR1_0   ((uint32_t)0x00000004U)
#define GPIO_OSPEEDR_OSPEEDR1_1   ((uint32_t)0x00000008U)
#define GPIO_OSPEEDR_OSPEEDR2     ((uint32_t)0x00000030U)
#define GPIO_OSPEEDR_OSPEEDR2_0   ((uint32_t)0x00000010U)
#define GPIO_OSPEEDR_OSPEEDR2_1   ((uint32_t)0x00000020U)
#define GPIO_OSPEEDR_OSPEEDR3     ((uint32_t)0x000000C0U)
#define GPIO_OSPEEDR_OSPEEDR3_0   ((uint32_t)0x00000040U)
#define GPIO_OSPEEDR_OSPEEDR3_1   ((uint32_t)0x00000080U)
#define GPIO_OSPEEDR_OSPEEDR4     ((uint32_t)0x00000300U)
#define GPIO_OSPEEDR_OSPEEDR4_0   ((uint32_t)0x00000100U)
#define GPIO_OSPEEDR_OSPEEDR4_1   ((uint32_t)0x00000200U)
#define GPIO_OSPEEDR_OSPEEDR5     ((uint32_t)0x00000C00U)
#define GPIO_OSPEEDR_OSPEEDR5_0   ((uint32_t)0x00000400U)
#define GPIO_OSPEEDR_OSPEEDR5_1   ((uint32_t)0x00000800U)
#define GPIO_OSPEEDR_OSPEEDR6     ((uint32_t)0x00003000U)
#define GPIO_OSPEEDR_OSPEEDR6_0   ((uint32_t)0x00001000U)
#define GPIO_OSPEEDR_OSPEEDR6_1   ((uint32_t)0x00002000U)
#define GPIO_OSPEEDR_OSPEEDR7     ((uint32_t)0x0000C000U)
#define GPIO_OSPEEDR_OSPEEDR7_0   ((uint32_t)0x00004000U)
#define GPIO_OSPEEDR_OSPEEDR7_1   ((uint32_t)0x00008000U)
#define GPIO_OSPEEDR_OSPEEDR8     ((uint32_t)0x00030000U)
#define GPIO_OSPEEDR_OSPEEDR8_0   ((uint32_t)0x00010000U)
#define GPIO_OSPEEDR_OSPEEDR8_1   ((uint32_t)0x00020000U)
#define GPIO_OSPEEDR_OSPEEDR9     ((uint32_t)0x000C0000U)
#define GPIO_OSPEEDR_OSPEEDR9_0   ((uint32_t)0x00040000U)
#define GPIO_OSPEEDR_OSPEEDR9_1   ((uint32_t)0x00080000U)
#define GPIO_OSPEEDR_OSPEEDR10    ((uint32_t)0x00300000U)
#define GPIO_OSPEEDR_OSPEEDR10_0  ((uint32_t)0x00100000U)
#define GPIO_OSPEEDR_OSPEEDR10_1  ((uint32_t)0x00200000U)
#define GPIO_OSPEEDR_OSPEEDR11    ((uint32_t)0x00C00000U)
#define GPIO_OSPEEDR_OSPEEDR11_0  ((uint32_t)0x00400000U)
#define GPIO_OSPEEDR_OSPEEDR11_1  ((uint32_t)0x00800000U)
#define GPIO_OSPEEDR_OSPEEDR12    ((uint32_t)0x03000000U)
#define GPIO_OSPEEDR_OSPEEDR12_0  ((uint32_t)0x01000000U)
#define GPIO_OSPEEDR_OSPEEDR12_1  ((uint32_t)0x02000000U)
#define GPIO_OSPEEDR_OSPEEDR13    ((uint32_t)0x0C000000U)
#define GPIO_OSPEEDR_OSPEEDR13_0  ((uint32_t)0x04000000U)
#define GPIO_OSPEEDR_OSPEEDR13_1  ((uint32_t)0x08000000U)
#define GPIO_OSPEEDR_OSPEEDR14    ((uint32_t)0x30000000U)
#define GPIO_OSPEEDR_OSPEEDR14_0  ((uint32_t)0x10000000U)
#define GPIO_OSPEEDR_OSPEEDR14_1  ((uint32_t)0x20000000U)
#define GPIO_OSPEEDR_OSPEEDR15    ((uint32_t)0xC0000000U)
#define GPIO_OSPEEDR_OSPEEDR15_0  ((uint32_t)0x40000000U)
#define GPIO_OSPEEDR_OSPEEDR15_1  ((uint32_t)0x80000000U)

/* Old Bit definition for GPIO_OSPEEDR register maintained for legacy purpose */
#define GPIO_OSPEEDER_OSPEEDR0     GPIO_OSPEEDR_OSPEEDR0
#define GPIO_OSPEEDER_OSPEEDR0_0   GPIO_OSPEEDR_OSPEEDR0_0
#define GPIO_OSPEEDER_OSPEEDR0_1   GPIO_OSPEEDR_OSPEEDR0_1
#define GPIO_OSPEEDER_OSPEEDR1     GPIO_OSPEEDR_OSPEEDR1
#define GPIO_OSPEEDER_OSPEEDR1_0   GPIO_OSPEEDR_OSPEEDR1_0
#define GPIO_OSPEEDER_OSPEEDR1_1   GPIO_OSPEEDR_OSPEEDR1_1
#define GPIO_OSPEEDER_OSPEEDR2     GPIO_OSPEEDR_OSPEEDR2
#define GPIO_OSPEEDER_OSPEEDR2_0   GPIO_OSPEEDR_OSPEEDR2_0
#define GPIO_OSPEEDER_OSPEEDR2_1   GPIO_OSPEEDR_OSPEEDR2_1
#define GPIO_OSPEEDER_OSPEEDR3     GPIO_OSPEEDR_OSPEEDR3
#define GPIO_OSPEEDER_OSPEEDR3_0   GPIO_OSPEEDR_OSPEEDR3_0
#define GPIO_OSPEEDER_OSPEEDR3_1   GPIO_OSPEEDR_OSPEEDR3_1
#define GPIO_OSPEEDER_OSPEEDR4     GPIO_OSPEEDR_OSPEEDR4
#define GPIO_OSPEEDER_OSPEEDR4_0   GPIO_OSPEEDR_OSPEEDR4_0
#define GPIO_OSPEEDER_OSPEEDR4_1   GPIO_OSPEEDR_OSPEEDR4_1
#define GPIO_OSPEEDER_OSPEEDR5     GPIO_OSPEEDR_OSPEEDR5
#define GPIO_OSPEEDER_OSPEEDR5_0   GPIO_OSPEEDR_OSPEEDR5_0
#define GPIO_OSPEEDER_OSPEEDR5_1   GPIO_OSPEEDR_OSPEEDR5_1
#define GPIO_OSPEEDER_OSPEEDR6     GPIO_OSPEEDR_OSPEEDR6
#define GPIO_OSPEEDER_OSPEEDR6_0   GPIO_OSPEEDR_OSPEEDR6_0
#define GPIO_OSPEEDER_OSPEEDR6_1   GPIO_OSPEEDR_OSPEEDR6_1
#define GPIO_OSPEEDER_OSPEEDR7     GPIO_OSPEEDR_OSPEEDR7
#define GPIO_OSPEEDER_OSPEEDR7_0   GPIO_OSPEEDR_OSPEEDR7_0
#define GPIO_OSPEEDER_OSPEEDR7_1   GPIO_OSPEEDR_OSPEEDR7_1
#define GPIO_OSPEEDER_OSPEEDR8     GPIO_OSPEEDR_OSPEEDR8
#define GPIO_OSPEEDER_OSPEEDR8_0   GPIO_OSPEEDR_OSPEEDR8_0
#define GPIO_OSPEEDER_OSPEEDR8_1   GPIO_OSPEEDR_OSPEEDR8_1
#define GPIO_OSPEEDER_OSPEEDR9     GPIO_OSPEEDR_OSPEEDR9
#define GPIO_OSPEEDER_OSPEEDR9_0   GPIO_OSPEEDR_OSPEEDR9_0
#define GPIO_OSPEEDER_OSPEEDR9_1   GPIO_OSPEEDR_OSPEEDR9_1
#define GPIO_OSPEEDER_OSPEEDR10    GPIO_OSPEEDR_OSPEEDR10
#define GPIO_OSPEEDER_OSPEEDR10_0  GPIO_OSPEEDR_OSPEEDR10_0
#define GPIO_OSPEEDER_OSPEEDR10_1  GPIO_OSPEEDR_OSPEEDR10_1
#define GPIO_OSPEEDER_OSPEEDR11    GPIO_OSPEEDR_OSPEEDR11
#define GPIO_OSPEEDER_OSPEEDR11_0  GPIO_OSPEEDR_OSPEEDR11_0
#define GPIO_OSPEEDER_OSPEEDR11_1  GPIO_OSPEEDR_OSPEEDR11_1
#define GPIO_OSPEEDER_OSPEEDR12    GPIO_OSPEEDR_OSPEEDR12
#define GPIO_OSPEEDER_OSPEEDR12_0  GPIO_OSPEEDR_OSPEEDR12_0
#define GPIO_OSPEEDER_OSPEEDR12_1  GPIO_OSPEEDR_OSPEEDR12_1
#define GPIO_OSPEEDER_OSPEEDR13    GPIO_OSPEEDR_OSPEEDR13
#define GPIO_OSPEEDER_OSPEEDR13_0  GPIO_OSPEEDR_OSPEEDR13_0
#define GPIO_OSPEEDER_OSPEEDR13_1  GPIO_OSPEEDR_OSPEEDR13_1
#define GPIO_OSPEEDER_OSPEEDR14    GPIO_OSPEEDR_OSPEEDR14
#define GPIO_OSPEEDER_OSPEEDR14_0  GPIO_OSPEEDR_OSPEEDR14_0
#define GPIO_OSPEEDER_OSPEEDR14_1  GPIO_OSPEEDR_OSPEEDR14_1
#define GPIO_OSPEEDER_OSPEEDR15    GPIO_OSPEEDR_OSPEEDR15
#define GPIO_OSPEEDER_OSPEEDR15_0  GPIO_OSPEEDR_OSPEEDR15_0
#define GPIO_OSPEEDER_OSPEEDR15_1  GPIO_OSPEEDR_OSPEEDR15_1

/*******************  Bit definition for GPIO_PUPDR register ******************/
#define GPIO_PUPDR_PUPDR0          ((uint32_t)0x00000003U)
#define GPIO_PUPDR_PUPDR0_0        ((uint32_t)0x00000001U)
#define GPIO_PUPDR_PUPDR0_1        ((uint32_t)0x00000002U)
#define GPIO_PUPDR_PUPDR1          ((uint32_t)0x0000000CU)
#define GPIO_PUPDR_PUPDR1_0        ((uint32_t)0x00000004U)
#define GPIO_PUPDR_PUPDR1_1        ((uint32_t)0x00000008U)
#define GPIO_PUPDR_PUPDR2          ((uint32_t)0x00000030U)
#define GPIO_PUPDR_PUPDR2_0        ((uint32_t)0x00000010U)
#define GPIO_PUPDR_PUPDR2_1        ((uint32_t)0x00000020U)
#define GPIO_PUPDR_PUPDR3          ((uint32_t)0x000000C0U)
#define GPIO_PUPDR_PUPDR3_0        ((uint32_t)0x00000040U)
#define GPIO_PUPDR_PUPDR3_1        ((uint32_t)0x00000080U)
#define GPIO_PUPDR_PUPDR4          ((uint32_t)0x00000300U)
#define GPIO_PUPDR_PUPDR4_0        ((uint32_t)0x00000100U)
#define GPIO_PUPDR_PUPDR4_1        ((uint32_t)0x00000200U)
#define GPIO_PUPDR_PUPDR5          ((uint32_t)0x00000C00U)
#define GPIO_PUPDR_PUPDR5_0        ((uint32_t)0x00000400U)
#define GPIO_PUPDR_PUPDR5_1        ((uint32_t)0x00000800U)
#define GPIO_PUPDR_PUPDR6          ((uint32_t)0x00003000U)
#define GPIO_PUPDR_PUPDR6_0        ((uint32_t)0x00001000U)
#define GPIO_PUPDR_PUPDR6_1        ((uint32_t)0x00002000U)
#define GPIO_PUPDR_PUPDR7          ((uint32_t)0x0000C000U)
#define GPIO_PUPDR_PUPDR7_0        ((uint32_t)0x00004000U)
#define GPIO_PUPDR_PUPDR7_1        ((uint32_t)0x00008000U)
#define GPIO_PUPDR_PUPDR8          ((uint32_t)0x00030000U)
#define GPIO_PUPDR_PUPDR8_0        ((uint32_t)0x00010000U)
#define GPIO_PUPDR_PUPDR8_1        ((uint32_t)0x00020000U)
#define GPIO_PUPDR_PUPDR9          ((uint32_t)0x000C0000U)
#define GPIO_PUPDR_PUPDR9_0        ((uint32_t)0x00040000U)
#define GPIO_PUPDR_PUPDR9_1        ((uint32_t)0x00080000U)
#define GPIO_PUPDR_PUPDR10         ((uint32_t)0x00300000U)
#define GPIO_PUPDR_PUPDR10_0       ((uint32_t)0x00100000U)
#define GPIO_PUPDR_PUPDR10_1       ((uint32_t)0x00200000U)
#define GPIO_PUPDR_PUPDR11         ((uint32_t)0x00C00000U)
#define GPIO_PUPDR_PUPDR11_0       ((uint32_t)0x00400000U)
#define GPIO_PUPDR_PUPDR11_1       ((uint32_t)0x00800000U)
#define GPIO_PUPDR_PUPDR12         ((uint32_t)0x03000000U)
#define GPIO_PUPDR_PUPDR12_0       ((uint32_t)0x01000000U)
#define GPIO_PUPDR_PUPDR12_1       ((uint32_t)0x02000000U)
#define GPIO_PUPDR_PUPDR13         ((uint32_t)0x0C000000U)
#define GPIO_PUPDR_PUPDR13_0       ((uint32_t)0x04000000U)
#define GPIO_PUPDR_PUPDR13_1       ((uint32_t)0x08000000U)
#define GPIO_PUPDR_PUPDR14         ((uint32_t)0x30000000U)
#define GPIO_PUPDR_PUPDR14_0       ((uint32_t)0x10000000U)
#define GPIO_PUPDR_PUPDR14_1       ((uint32_t)0x20000000U)
#define GPIO_PUPDR_PUPDR15         ((uint32_t)0xC0000000U)
#define GPIO_PUPDR_PUPDR15_0       ((uint32_t)0x40000000U)
#define GPIO_PUPDR_PUPDR15_1       ((uint32_t)0x80000000U)

/*******************  Bit definition for GPIO_IDR register  *******************/
#define GPIO_IDR_0                 ((uint32_t)0x00000001U)
#define GPIO_IDR_1                 ((uint32_t)0x00000002U)
#define GPIO_IDR_2                 ((uint32_t)0x00000004U)
#define GPIO_IDR_3                 ((uint32_t)0x00000008U)
#define GPIO_IDR_4                 ((uint32_t)0x00000010U)
#define GPIO_IDR_5                 ((uint32_t)0x00000020U)
#define GPIO_IDR_6                 ((uint32_t)0x00000040U)
#define GPIO_IDR_7                 ((uint32_t)0x00000080U)
#define GPIO_IDR_8                 ((uint32_t)0x00000100U)
#define GPIO_IDR_9                 ((uint32_t)0x00000200U)
#define GPIO_IDR_10                ((uint32_t)0x00000400U)
#define GPIO_IDR_11                ((uint32_t)0x00000800U)
#define GPIO_IDR_12                ((uint32_t)0x00001000U)
#define GPIO_IDR_13                ((uint32_t)0x00002000U)
#define GPIO_IDR_14                ((uint32_t)0x00004000U)
#define GPIO_IDR_15                ((uint32_t)0x00008000U)

/******************  Bit definition for GPIO_ODR register  ********************/
#define GPIO_ODR_0                 ((uint32_t)0x00000001U)
#define GPIO_ODR_1                 ((uint32_t)0x00000002U)
#define GPIO_ODR_2                 ((uint32_t)0x00000004U)
#define GPIO_ODR_3                 ((uint32_t)0x00000008U)
#define GPIO_ODR_4                 ((uint32_t)0x00000010U)
#define GPIO_ODR_5                 ((uint32_t)0x00000020U)
#define GPIO_ODR_6                 ((uint32_t)0x00000040U)
#define GPIO_ODR_7                 ((uint32_t)0x00000080U)
#define GPIO_ODR_8                 ((uint32_t)0x00000100U)
#define GPIO_ODR_9                 ((uint32_t)0x00000200U)
#define GPIO_ODR_10                ((uint32_t)0x00000400U)
#define GPIO_ODR_11                ((uint32_t)0x00000800U)
#define GPIO_ODR_12                ((uint32_t)0x00001000U)
#define GPIO_ODR_13                ((uint32_t)0x00002000U)
#define GPIO_ODR_14                ((uint32_t)0x00004000U)
#define GPIO_ODR_15                ((uint32_t)0x00008000U)

/****************** Bit definition for GPIO_BSRR register  ********************/
#define GPIO_BSRR_BS_0             ((uint32_t)0x00000001U)
#define GPIO_BSRR_BS_1             ((uint32_t)0x00000002U)
#define GPIO_BSRR_BS_2             ((uint32_t)0x00000004U)
#define GPIO_BSRR_BS_3             ((uint32_t)0x00000008U)
#define GPIO_BSRR_BS_4             ((uint32_t)0x00000010U)
#define GPIO_BSRR_BS_5             ((uint32_t)0x00000020U)
#define GPIO_BSRR_BS_6             ((uint32_t)0x00000040U)
#define GPIO_BSRR_BS_7             ((uint32_t)0x00000080U)
#define GPIO_BSRR_BS_8             ((uint32_t)0x00000100U)
#define GPIO_BSRR_BS_9             ((uint32_t)0x00000200U)
#define GPIO_BSRR_BS_10            ((uint32_t)0x00000400U)
#define GPIO_BSRR_BS_11            ((uint32_t)0x00000800U)
#define GPIO_BSRR_BS_12            ((uint32_t)0x00001000U)
#define GPIO_BSRR_BS_13            ((uint32_t)0x00002000U)
#define GPIO_BSRR_BS_14            ((uint32_t)0x00004000U)
#define GPIO_BSRR_BS_15            ((uint32_t)0x00008000U)
#define GPIO_BSRR_BR_0             ((uint32_t)0x00010000U)
#define GPIO_BSRR_BR_1             ((uint32_t)0x00020000U)
#define GPIO_BSRR_BR_2             ((uint32_t)0x00040000U)
#define GPIO_BSRR_BR_3             ((uint32_t)0x00080000U)
#define GPIO_BSRR_BR_4             ((uint32_t)0x00100000U)
#define GPIO_BSRR_BR_5             ((uint32_t)0x00200000U)
#define GPIO_BSRR_BR_6             ((uint32_t)0x00400000U)
#define GPIO_BSRR_BR_7             ((uint32_t)0x00800000U)
#define GPIO_BSRR_BR_8             ((uint32_t)0x01000000U)
#define GPIO_BSRR_BR_9             ((uint32_t)0x02000000U)
#define GPIO_BSRR_BR_10            ((uint32_t)0x04000000U)
#define GPIO_BSRR_BR_11            ((uint32_t)0x08000000U)
#define GPIO_BSRR_BR_12            ((uint32_t)0x10000000U)
#define GPIO_BSRR_BR_13            ((uint32_t)0x20000000U)
#define GPIO_BSRR_BR_14            ((uint32_t)0x40000000U)
#define GPIO_BSRR_BR_15            ((uint32_t)0x80000000U)

/****************** Bit definition for GPIO_LCKR register  ********************/
#define GPIO_LCKR_LCK0             ((uint32_t)0x00000001U)
#define GPIO_LCKR_LCK1             ((uint32_t)0x00000002U)
#define GPIO_LCKR_LCK2             ((uint32_t)0x00000004U)
#define GPIO_LCKR_LCK3             ((uint32_t)0x00000008U)
#define GPIO_LCKR_LCK4             ((uint32_t)0x00000010U)
#define GPIO_LCKR_LCK5             ((uint32_t)0x00000020U)
#define GPIO_LCKR_LCK6             ((uint32_t)0x00000040U)
#define GPIO_LCKR_LCK7             ((uint32_t)0x00000080U)
#define GPIO_LCKR_LCK8             ((uint32_t)0x00000100U)
#define GPIO_LCKR_LCK9             ((uint32_t)0x00000200U)
#define GPIO_LCKR_LCK10            ((uint32_t)0x00000400U)
#define GPIO_LCKR_LCK11            ((uint32_t)0x00000800U)
#define GPIO_LCKR_LCK12            ((uint32_t)0x00001000U)
#define GPIO_LCKR_LCK13            ((uint32_t)0x00002000U)
#define GPIO_LCKR_LCK14            ((uint32_t)0x00004000U)
#define GPIO_LCKR_LCK15            ((uint32_t)0x00008000U)
#define GPIO_LCKR_LCKK             ((uint32_t)0x00010000U)

/****************** Bit definition for GPIO_AFRL register  ********************/
#define GPIO_AFRL_AFRL0            ((uint32_t)0x0000000FU)
#define GPIO_AFRL_AFRL1            ((uint32_t)0x000000F0U)
#define GPIO_AFRL_AFRL2            ((uint32_t)0x00000F00U)
#define GPIO_AFRL_AFRL3            ((uint32_t)0x0000F000U)
#define GPIO_AFRL_AFRL4            ((uint32_t)0x000F0000U)
#define GPIO_AFRL_AFRL5            ((uint32_t)0x00F00000U)
#define GPIO_AFRL_AFRL6            ((uint32_t)0x0F000000U)
#define GPIO_AFRL_AFRL7            ((uint32_t)0xF0000000U)

/****************** Bit definition for GPIO_AFRH register  ********************/
#define GPIO_AFRH_AFRH0            ((uint32_t)0x0000000FU)
#define GPIO_AFRH_AFRH1            ((uint32_t)0x000000F0U)
#define GPIO_AFRH_AFRH2            ((uint32_t)0x00000F00U)
#define GPIO_AFRH_AFRH3            ((uint32_t)0x0000F000U)
#define GPIO_AFRH_AFRH4            ((uint32_t)0x000F0000U)
#define GPIO_AFRH_AFRH5            ((uint32_t)0x00F00000U)
#define GPIO_AFRH_AFRH6            ((uint32_t)0x0F000000U)
#define GPIO_AFRH_AFRH7            ((uint32_t)0xF0000000U)

/****************** Bit definition for GPIO_BRR register  *********************/
#define GPIO_BRR_BR_0              ((uint32_t)0x00000001U)
#define GPIO_BRR_BR_1              ((uint32_t)0x00000002U)
#define GPIO_BRR_BR_2              ((uint32_t)0x00000004U)
#define GPIO_BRR_BR_3              ((uint32_t)0x00000008U)
#define GPIO_BRR_BR_4              ((uint32_t)0x00000010U)
#define GPIO_BRR_BR_5              ((uint32_t)0x00000020U)
#define GPIO_BRR_BR_6              ((uint32_t)0x00000040U)
#define GPIO_BRR_BR_7              ((uint32_t)0x00000080U)
#define GPIO_BRR_BR_8              ((uint32_t)0x00000100U)
#define GPIO_BRR_BR_9              ((uint32_t)0x00000200U)
#define GPIO_BRR_BR_10             ((uint32_t)0x00000400U)
#define GPIO_BRR_BR_11             ((uint32_t)0x00000800U)
#define GPIO_BRR_BR_12             ((uint32_t)0x00001000U)
#define GPIO_BRR_BR_13             ((uint32_t)0x00002000U)
#define GPIO_BRR_BR_14             ((uint32_t)0x00004000U)
#define GPIO_BRR_BR_15             ((uint32_t)0x00008000U)

/******************************************************************************/
/*                                                                            */
/*                   Inter-integrated Circuit Interface (I2C)                 */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for I2C_CR1 register  *******************/
#define  I2C_CR1_PE                          ((uint32_t)0x00000001U)        /*!< Peripheral enable */
#define  I2C_CR1_TXIE                        ((uint32_t)0x00000002U)        /*!< TX interrupt enable */
#define  I2C_CR1_RXIE                        ((uint32_t)0x00000004U)        /*!< RX interrupt enable */
#define  I2C_CR1_ADDRIE                      ((uint32_t)0x00000008U)        /*!< Address match interrupt enable */
#define  I2C_CR1_NACKIE                      ((uint32_t)0x00000010U)        /*!< NACK received interrupt enable */
#define  I2C_CR1_STOPIE                      ((uint32_t)0x00000020U)        /*!< STOP detection interrupt enable */
#define  I2C_CR1_TCIE                        ((uint32_t)0x00000040U)        /*!< Transfer complete interrupt enable */
#define  I2C_CR1_ERRIE                       ((uint32_t)0x00000080U)        /*!< Errors interrupt enable */
#define  I2C_CR1_DNF                         ((uint32_t)0x00000F00U)        /*!< Digital noise filter */
#define  I2C_CR1_ANFOFF                      ((uint32_t)0x00001000U)        /*!< Analog noise filter OFF */
#define  I2C_CR1_SWRST                       ((uint32_t)0x00002000U)        /*!< Software reset */
#define  I2C_CR1_TXDMAEN                     ((uint32_t)0x00004000U)        /*!< DMA transmission requests enable */
#define  I2C_CR1_RXDMAEN                     ((uint32_t)0x00008000U)        /*!< DMA reception requests enable */
#define  I2C_CR1_SBC                         ((uint32_t)0x00010000U)        /*!< Slave byte control */
#define  I2C_CR1_NOSTRETCH                   ((uint32_t)0x00020000U)        /*!< Clock stretching disable */
#define  I2C_CR1_WUPEN                       ((uint32_t)0x00040000U)        /*!< Wakeup from STOP enable */
#define  I2C_CR1_GCEN                        ((uint32_t)0x00080000U)        /*!< General call enable */
#define  I2C_CR1_SMBHEN                      ((uint32_t)0x00100000U)        /*!< SMBus host address enable */
#define  I2C_CR1_SMBDEN                      ((uint32_t)0x00200000U)        /*!< SMBus device default address enable */
#define  I2C_CR1_ALERTEN                     ((uint32_t)0x00400000U)        /*!< SMBus alert enable */
#define  I2C_CR1_PECEN                       ((uint32_t)0x00800000U)        /*!< PEC enable */

/******************  Bit definition for I2C_CR2 register  ********************/
#define  I2C_CR2_SADD                        ((uint32_t)0x000003FFU)        /*!< Slave address (master mode) */
#define  I2C_CR2_RD_WRN                      ((uint32_t)0x00000400U)        /*!< Transfer direction (master mode) */
#define  I2C_CR2_ADD10                       ((uint32_t)0x00000800U)        /*!< 10-bit addressing mode (master mode) */
#define  I2C_CR2_HEAD10R                     ((uint32_t)0x00001000U)        /*!< 10-bit address header only read direction (master mode) */
#define  I2C_CR2_START                       ((uint32_t)0x00002000U)        /*!< START generation */
#define  I2C_CR2_STOP                        ((uint32_t)0x00004000U)        /*!< STOP generation (master mode) */
#define  I2C_CR2_NACK                        ((uint32_t)0x00008000U)        /*!< NACK generation (slave mode) */
#define  I2C_CR2_NBYTES                      ((uint32_t)0x00FF0000U)        /*!< Number of bytes */
#define  I2C_CR2_RELOAD                      ((uint32_t)0x01000000U)        /*!< NBYTES reload mode */
#define  I2C_CR2_AUTOEND                     ((uint32_t)0x02000000U)        /*!< Automatic end mode (master mode) */
#define  I2C_CR2_PECBYTE                     ((uint32_t)0x04000000U)        /*!< Packet error checking byte */

/*******************  Bit definition for I2C_OAR1 register  ******************/
#define  I2C_OAR1_OA1                        ((uint32_t)0x000003FFU)        /*!< Interface own address 1 */
#define  I2C_OAR1_OA1MODE                    ((uint32_t)0x00000400U)        /*!< Own address 1 10-bit mode */
#define  I2C_OAR1_OA1EN                      ((uint32_t)0x00008000U)        /*!< Own address 1 enable */

/*******************  Bit definition for I2C_OAR2 register  ******************/
#define  I2C_OAR2_OA2                        ((uint32_t)0x000000FEU)        /*!< Interface own address 2 */
#define  I2C_OAR2_OA2MSK                     ((uint32_t)0x00000700U)        /*!< Own address 2 masks */
#define  I2C_OAR2_OA2NOMASK                  ((uint32_t)0x00000000U)        /*!< No mask                                        */
#define  I2C_OAR2_OA2MASK01                  ((uint32_t)0x00000100U)        /*!< OA2[1] is masked, Only OA2[7:2] are compared   */
#define  I2C_OAR2_OA2MASK02                  ((uint32_t)0x00000200U)        /*!< OA2[2:1] is masked, Only OA2[7:3] are compared */
#define  I2C_OAR2_OA2MASK03                  ((uint32_t)0x00000300U)        /*!< OA2[3:1] is masked, Only OA2[7:4] are compared */
#define  I2C_OAR2_OA2MASK04                  ((uint32_t)0x00000400U)        /*!< OA2[4:1] is masked, Only OA2[7:5] are compared */
#define  I2C_OAR2_OA2MASK05                  ((uint32_t)0x00000500U)        /*!< OA2[5:1] is masked, Only OA2[7:6] are compared */
#define  I2C_OAR2_OA2MASK06                  ((uint32_t)0x00000600U)        /*!< OA2[6:1] is masked, Only OA2[7] are compared   */
#define  I2C_OAR2_OA2MASK07                  ((uint32_t)0x00000700U)        /*!< OA2[7:1] is masked, No comparison is done      */
#define  I2C_OAR2_OA2EN                      ((uint32_t)0x00008000U)        /*!< Own address 2 enable */

/*******************  Bit definition for I2C_TIMINGR register ****************/
#define  I2C_TIMINGR_SCLL                    ((uint32_t)0x000000FFU)        /*!< SCL low period (master mode) */
#define  I2C_TIMINGR_SCLH                    ((uint32_t)0x0000FF00U)        /*!< SCL high period (master mode) */
#define  I2C_TIMINGR_SDADEL                  ((uint32_t)0x000F0000U)        /*!< Data hold time */
#define  I2C_TIMINGR_SCLDEL                  ((uint32_t)0x00F00000U)        /*!< Data setup time */
#define  I2C_TIMINGR_PRESC                   ((uint32_t)0xF0000000U)        /*!< Timings prescaler */

/******************* Bit definition for I2C_TIMEOUTR register ****************/
#define  I2C_TIMEOUTR_TIMEOUTA               ((uint32_t)0x00000FFFU)        /*!< Bus timeout A */
#define  I2C_TIMEOUTR_TIDLE                  ((uint32_t)0x00001000U)        /*!< Idle clock timeout detection */
#define  I2C_TIMEOUTR_TIMOUTEN               ((uint32_t)0x00008000U)        /*!< Clock timeout enable */
#define  I2C_TIMEOUTR_TIMEOUTB               ((uint32_t)0x0FFF0000U)        /*!< Bus timeout B*/
#define  I2C_TIMEOUTR_TEXTEN                 ((uint32_t)0x80000000U)        /*!< Extended clock timeout enable */

/******************  Bit definition for I2C_ISR register  ********************/
#define  I2C_ISR_TXE                         ((uint32_t)0x00000001U)        /*!< Transmit data register empty */
#define  I2C_ISR_TXIS                        ((uint32_t)0x00000002U)        /*!< Transmit interrupt status */
#define  I2C_ISR_RXNE                        ((uint32_t)0x00000004U)        /*!< Receive data register not empty */
#define  I2C_ISR_ADDR                        ((uint32_t)0x00000008U)        /*!< Address matched (slave mode)*/
#define  I2C_ISR_NACKF                       ((uint32_t)0x00000010U)        /*!< NACK received flag */
#define  I2C_ISR_STOPF                       ((uint32_t)0x00000020U)        /*!< STOP detection flag */
#define  I2C_ISR_TC                          ((uint32_t)0x00000040U)        /*!< Transfer complete (master mode) */
#define  I2C_ISR_TCR                         ((uint32_t)0x00000080U)        /*!< Transfer complete reload */
#define  I2C_ISR_BERR                        ((uint32_t)0x00000100U)        /*!< Bus error */
#define  I2C_ISR_ARLO                        ((uint32_t)0x00000200U)        /*!< Arbitration lost */
#define  I2C_ISR_OVR                         ((uint32_t)0x00000400U)        /*!< Overrun/Underrun */
#define  I2C_ISR_PECERR                      ((uint32_t)0x00000800U)        /*!< PEC error in reception */
#define  I2C_ISR_TIMEOUT                     ((uint32_t)0x00001000U)        /*!< Timeout or Tlow detection flag */
#define  I2C_ISR_ALERT                       ((uint32_t)0x00002000U)        /*!< SMBus alert */
#define  I2C_ISR_BUSY                        ((uint32_t)0x00008000U)        /*!< Bus busy */
#define  I2C_ISR_DIR                         ((uint32_t)0x00010000U)        /*!< Transfer direction (slave mode) */
#define  I2C_ISR_ADDCODE                     ((uint32_t)0x00FE0000U)        /*!< Address match code (slave mode) */

/******************  Bit definition for I2C_ICR register  ********************/
#define  I2C_ICR_ADDRCF                      ((uint32_t)0x00000008U)        /*!< Address matched clear flag */
#define  I2C_ICR_NACKCF                      ((uint32_t)0x00000010U)        /*!< NACK clear flag */
#define  I2C_ICR_STOPCF                      ((uint32_t)0x00000020U)        /*!< STOP detection clear flag */
#define  I2C_ICR_BERRCF                      ((uint32_t)0x00000100U)        /*!< Bus error clear flag */
#define  I2C_ICR_ARLOCF                      ((uint32_t)0x00000200U)        /*!< Arbitration lost clear flag */
#define  I2C_ICR_OVRCF                       ((uint32_t)0x00000400U)        /*!< Overrun/Underrun clear flag */
#define  I2C_ICR_PECCF                       ((uint32_t)0x00000800U)        /*!< PAC error clear flag */
#define  I2C_ICR_TIMOUTCF                    ((uint32_t)0x00001000U)        /*!< Timeout clear flag */
#define  I2C_ICR_ALERTCF                     ((uint32_t)0x00002000U)        /*!< Alert clear flag */

/******************  Bit definition for I2C_PECR register  *******************/
#define  I2C_PECR_PEC                        ((uint32_t)0x000000FFU)       /*!< PEC register */

/******************  Bit definition for I2C_RXDR register  *********************/
#define  I2C_RXDR_RXDATA                     ((uint32_t)0x000000FFU)        /*!< 8-bit receive data */

/******************  Bit definition for I2C_TXDR register  *******************/
#define  I2C_TXDR_TXDATA                     ((uint32_t)0x000000FFU)        /*!< 8-bit transmit data */

/*****************************************************************************/
/*                                                                           */
/*                        Independent WATCHDOG (IWDG)                        */
/*                                                                           */
/*****************************************************************************/
/*******************  Bit definition for IWDG_KR register  *******************/
#define  IWDG_KR_KEY                         ((uint32_t)0xFFFF)            /*!< Key value (write only, read 0000h) */

/*******************  Bit definition for IWDG_PR register  *******************/
#define  IWDG_PR_PR                          ((uint32_t)0x07)              /*!< PR[2:0] (Prescaler divider) */
#define  IWDG_PR_PR_0                        ((uint32_t)0x01)              /*!< Bit 0 */
#define  IWDG_PR_PR_1                        ((uint32_t)0x02)              /*!< Bit 1 */
#define  IWDG_PR_PR_2                        ((uint32_t)0x04)              /*!< Bit 2 */

/*******************  Bit definition for IWDG_RLR register  ******************/
#define  IWDG_RLR_RL                         ((uint32_t)0x0FFF)            /*!< Watchdog counter reload value */

/*******************  Bit definition for IWDG_SR register  *******************/
#define  IWDG_SR_PVU                         ((uint32_t)0x01)              /*!< Watchdog prescaler value update */
#define  IWDG_SR_RVU                         ((uint32_t)0x02)              /*!< Watchdog counter reload value update */
#define  IWDG_SR_WVU                         ((uint32_t)0x04)              /*!< Watchdog counter window value update */

/*******************  Bit definition for IWDG_KR register  *******************/
#define  IWDG_WINR_WIN                       ((uint32_t)0x0FFF)            /*!< Watchdog counter window value */

/*****************************************************************************/
/*                                                                           */
/*                          Power Control (PWR)                              */
/*                                                                           */
/*****************************************************************************/

#define PWR_PVD_SUPPORT                       /*!< PWR feature available only on specific devices: Power Voltage Detection feature */


/********************  Bit definition for PWR_CR register  *******************/
#define  PWR_CR_LPDS                         ((uint32_t)0x00000001U)        /*!< Low-power Deepsleep */
#define  PWR_CR_PDDS                         ((uint32_t)0x00000002U)        /*!< Power Down Deepsleep */
#define  PWR_CR_CWUF                         ((uint32_t)0x00000004U)        /*!< Clear Wakeup Flag */
#define  PWR_CR_CSBF                         ((uint32_t)0x00000008U)        /*!< Clear Standby Flag */
#define  PWR_CR_PVDE                         ((uint32_t)0x00000010U)        /*!< Power Voltage Detector Enable */

#define  PWR_CR_PLS                          ((uint32_t)0x000000E0U)        /*!< PLS[2:0] bits (PVD Level Selection) */
#define  PWR_CR_PLS_0                        ((uint32_t)0x00000020U)        /*!< Bit 0 */
#define  PWR_CR_PLS_1                        ((uint32_t)0x00000040U)        /*!< Bit 1 */
#define  PWR_CR_PLS_2                        ((uint32_t)0x00000080U)        /*!< Bit 2 */

/*!< PVD level configuration */
#define  PWR_CR_PLS_LEV0                     ((uint32_t)0x00000000U)        /*!< PVD level 0 */
#define  PWR_CR_PLS_LEV1                     ((uint32_t)0x00000020U)        /*!< PVD level 1 */
#define  PWR_CR_PLS_LEV2                     ((uint32_t)0x00000040U)        /*!< PVD level 2 */
#define  PWR_CR_PLS_LEV3                     ((uint32_t)0x00000060U)        /*!< PVD level 3 */
#define  PWR_CR_PLS_LEV4                     ((uint32_t)0x00000080U)        /*!< PVD level 4 */
#define  PWR_CR_PLS_LEV5                     ((uint32_t)0x000000A0U)        /*!< PVD level 5 */
#define  PWR_CR_PLS_LEV6                     ((uint32_t)0x000000C0U)        /*!< PVD level 6 */
#define  PWR_CR_PLS_LEV7                     ((uint32_t)0x000000E0U)        /*!< PVD level 7 */

#define  PWR_CR_DBP                          ((uint32_t)0x00000100U)        /*!< Disable Backup Domain write protection */

/*******************  Bit definition for PWR_CSR register  *******************/
#define  PWR_CSR_WUF                         ((uint32_t)0x00000001U)        /*!< Wakeup Flag */
#define  PWR_CSR_SBF                         ((uint32_t)0x00000002U)        /*!< Standby Flag */
#define  PWR_CSR_PVDO                        ((uint32_t)0x00000004U)        /*!< PVD Output */
#define  PWR_CSR_VREFINTRDYF                 ((uint32_t)0x00000008U)        /*!< Internal voltage reference (VREFINT) ready flag */

#define  PWR_CSR_EWUP1                       ((uint32_t)0x00000100U)        /*!< Enable WKUP pin 1 */
#define  PWR_CSR_EWUP2                       ((uint32_t)0x00000200U)        /*!< Enable WKUP pin 2 */

/*****************************************************************************/
/*                                                                           */
/*                         Reset and Clock Control                           */
/*                                                                           */
/*****************************************************************************/
/*
* @brief Specific device feature definitions  (not present on all devices in the STM32F0 family)
*/

/********************  Bit definition for RCC_CR register  *******************/
#define  RCC_CR_HSION                        ((uint32_t)0x00000001U)        /*!< Internal High Speed clock enable */
#define  RCC_CR_HSIRDY                       ((uint32_t)0x00000002U)        /*!< Internal High Speed clock ready flag */

#define  RCC_CR_HSITRIM                      ((uint32_t)0x000000F8U)        /*!< Internal High Speed clock trimming */
#define  RCC_CR_HSITRIM_0                    ((uint32_t)0x00000008U)        /*!<Bit 0 */
#define  RCC_CR_HSITRIM_1                    ((uint32_t)0x00000010U)        /*!<Bit 1 */
#define  RCC_CR_HSITRIM_2                    ((uint32_t)0x00000020U)        /*!<Bit 2 */
#define  RCC_CR_HSITRIM_3                    ((uint32_t)0x00000040U)        /*!<Bit 3 */
#define  RCC_CR_HSITRIM_4                    ((uint32_t)0x00000080U)        /*!<Bit 4 */

#define  RCC_CR_HSICAL                       ((uint32_t)0x0000FF00U)        /*!< Internal High Speed clock Calibration */
#define  RCC_CR_HSICAL_0                     ((uint32_t)0x00000100U)        /*!<Bit 0 */
#define  RCC_CR_HSICAL_1                     ((uint32_t)0x00000200U)        /*!<Bit 1 */
#define  RCC_CR_HSICAL_2                     ((uint32_t)0x00000400U)        /*!<Bit 2 */
#define  RCC_CR_HSICAL_3                     ((uint32_t)0x00000800U)        /*!<Bit 3 */
#define  RCC_CR_HSICAL_4                     ((uint32_t)0x00001000U)        /*!<Bit 4 */
#define  RCC_CR_HSICAL_5                     ((uint32_t)0x00002000U)        /*!<Bit 5 */
#define  RCC_CR_HSICAL_6                     ((uint32_t)0x00004000U)        /*!<Bit 6 */
#define  RCC_CR_HSICAL_7                     ((uint32_t)0x00008000U)        /*!<Bit 7 */

#define  RCC_CR_HSEON                        ((uint32_t)0x00010000U)        /*!< External High Speed clock enable */
#define  RCC_CR_HSERDY                       ((uint32_t)0x00020000U)        /*!< External High Speed clock ready flag */
#define  RCC_CR_HSEBYP                       ((uint32_t)0x00040000U)        /*!< External High Speed clock Bypass */
#define  RCC_CR_CSSON                        ((uint32_t)0x00080000U)        /*!< Clock Security System enable */
#define  RCC_CR_PLLON                        ((uint32_t)0x01000000U)        /*!< PLL enable */
#define  RCC_CR_PLLRDY                       ((uint32_t)0x02000000U)        /*!< PLL clock ready flag */

/********************  Bit definition for RCC_CFGR register  *****************/
/*!< SW configuration */
#define  RCC_CFGR_SW                         ((uint32_t)0x00000003U)        /*!< SW[1:0] bits (System clock Switch) */
#define  RCC_CFGR_SW_0                       ((uint32_t)0x00000001U)        /*!< Bit 0 */
#define  RCC_CFGR_SW_1                       ((uint32_t)0x00000002U)        /*!< Bit 1 */

#define  RCC_CFGR_SW_HSI                     ((uint32_t)0x00000000U)        /*!< HSI selected as system clock */
#define  RCC_CFGR_SW_HSE                     ((uint32_t)0x00000001U)        /*!< HSE selected as system clock */
#define  RCC_CFGR_SW_PLL                     ((uint32_t)0x00000002U)        /*!< PLL selected as system clock */

/*!< SWS configuration */
#define  RCC_CFGR_SWS                        ((uint32_t)0x0000000CU)        /*!< SWS[1:0] bits (System Clock Switch Status) */
#define  RCC_CFGR_SWS_0                      ((uint32_t)0x00000004U)        /*!< Bit 0 */
#define  RCC_CFGR_SWS_1                      ((uint32_t)0x00000008U)        /*!< Bit 1 */

#define  RCC_CFGR_SWS_HSI                    ((uint32_t)0x00000000U)        /*!< HSI oscillator used as system clock */
#define  RCC_CFGR_SWS_HSE                    ((uint32_t)0x00000004U)        /*!< HSE oscillator used as system clock */
#define  RCC_CFGR_SWS_PLL                    ((uint32_t)0x00000008U)        /*!< PLL used as system clock */

/*!< HPRE configuration */
#define  RCC_CFGR_HPRE                       ((uint32_t)0x000000F0U)        /*!< HPRE[3:0] bits (AHB prescaler) */
#define  RCC_CFGR_HPRE_0                     ((uint32_t)0x00000010U)        /*!< Bit 0 */
#define  RCC_CFGR_HPRE_1                     ((uint32_t)0x00000020U)        /*!< Bit 1 */
#define  RCC_CFGR_HPRE_2                     ((uint32_t)0x00000040U)        /*!< Bit 2 */
#define  RCC_CFGR_HPRE_3                     ((uint32_t)0x00000080U)        /*!< Bit 3 */

#define  RCC_CFGR_HPRE_DIV1                  ((uint32_t)0x00000000U)        /*!< SYSCLK not divided */
#define  RCC_CFGR_HPRE_DIV2                  ((uint32_t)0x00000080U)        /*!< SYSCLK divided by 2 */
#define  RCC_CFGR_HPRE_DIV4                  ((uint32_t)0x00000090U)        /*!< SYSCLK divided by 4 */
#define  RCC_CFGR_HPRE_DIV8                  ((uint32_t)0x000000A0U)        /*!< SYSCLK divided by 8 */
#define  RCC_CFGR_HPRE_DIV16                 ((uint32_t)0x000000B0U)        /*!< SYSCLK divided by 16 */
#define  RCC_CFGR_HPRE_DIV64                 ((uint32_t)0x000000C0U)        /*!< SYSCLK divided by 64 */
#define  RCC_CFGR_HPRE_DIV128                ((uint32_t)0x000000D0U)        /*!< SYSCLK divided by 128 */
#define  RCC_CFGR_HPRE_DIV256                ((uint32_t)0x000000E0U)        /*!< SYSCLK divided by 256 */
#define  RCC_CFGR_HPRE_DIV512                ((uint32_t)0x000000F0U)        /*!< SYSCLK divided by 512 */

/*!< PPRE configuration */
#define  RCC_CFGR_PPRE                       ((uint32_t)0x00000700U)        /*!< PRE[2:0] bits (APB prescaler) */
#define  RCC_CFGR_PPRE_0                     ((uint32_t)0x00000100U)        /*!< Bit 0 */
#define  RCC_CFGR_PPRE_1                     ((uint32_t)0x00000200U)        /*!< Bit 1 */
#define  RCC_CFGR_PPRE_2                     ((uint32_t)0x00000400U)        /*!< Bit 2 */

#define  RCC_CFGR_PPRE_DIV1                  ((uint32_t)0x00000000U)        /*!< HCLK not divided */
#define  RCC_CFGR_PPRE_DIV2                  ((uint32_t)0x00000400U)        /*!< HCLK divided by 2 */
#define  RCC_CFGR_PPRE_DIV4                  ((uint32_t)0x00000500U)        /*!< HCLK divided by 4 */
#define  RCC_CFGR_PPRE_DIV8                  ((uint32_t)0x00000600U)        /*!< HCLK divided by 8 */
#define  RCC_CFGR_PPRE_DIV16                 ((uint32_t)0x00000700U)        /*!< HCLK divided by 16 */

/*!< ADCPPRE configuration */
#define  RCC_CFGR_ADCPRE                     ((uint32_t)0x00004000U)        /*!< ADCPRE bit (ADC prescaler) */

#define  RCC_CFGR_ADCPRE_DIV2                ((uint32_t)0x00000000U)        /*!< PCLK divided by 2 */
#define  RCC_CFGR_ADCPRE_DIV4                ((uint32_t)0x00004000U)        /*!< PCLK divided by 4 */

#define  RCC_CFGR_PLLSRC                     ((uint32_t)0x00010000U)        /*!< PLL entry clock source */
#define  RCC_CFGR_PLLSRC_HSI_DIV2            ((uint32_t)0x00000000U)        /*!< HSI clock divided by 2 selected as PLL entry clock source */
#define  RCC_CFGR_PLLSRC_HSE_PREDIV          ((uint32_t)0x00010000U)        /*!< HSE/PREDIV clock selected as PLL entry clock source */

#define  RCC_CFGR_PLLXTPRE                   ((uint32_t)0x00020000U)        /*!< HSE divider for PLL entry */
#define  RCC_CFGR_PLLXTPRE_HSE_PREDIV_DIV1   ((uint32_t)0x00000000U)        /*!< HSE/PREDIV clock not divided for PLL entry */
#define  RCC_CFGR_PLLXTPRE_HSE_PREDIV_DIV2   ((uint32_t)0x00020000U)        /*!< HSE/PREDIV clock divided by 2 for PLL entry */

/*!< PLLMUL configuration */
#define  RCC_CFGR_PLLMUL                     ((uint32_t)0x003C0000U)        /*!< PLLMUL[3:0] bits (PLL multiplication factor) */
#define  RCC_CFGR_PLLMUL_0                   ((uint32_t)0x00040000U)        /*!< Bit 0 */
#define  RCC_CFGR_PLLMUL_1                   ((uint32_t)0x00080000U)        /*!< Bit 1 */
#define  RCC_CFGR_PLLMUL_2                   ((uint32_t)0x00100000U)        /*!< Bit 2 */
#define  RCC_CFGR_PLLMUL_3                   ((uint32_t)0x00200000U)        /*!< Bit 3 */

#define  RCC_CFGR_PLLMUL2                    ((uint32_t)0x00000000U)        /*!< PLL input clock*2 */
#define  RCC_CFGR_PLLMUL3                    ((uint32_t)0x00040000U)        /*!< PLL input clock*3 */
#define  RCC_CFGR_PLLMUL4                    ((uint32_t)0x00080000U)        /*!< PLL input clock*4 */
#define  RCC_CFGR_PLLMUL5                    ((uint32_t)0x000C0000U)        /*!< PLL input clock*5 */
#define  RCC_CFGR_PLLMUL6                    ((uint32_t)0x00100000U)        /*!< PLL input clock*6 */
#define  RCC_CFGR_PLLMUL7                    ((uint32_t)0x00140000U)        /*!< PLL input clock*7 */
#define  RCC_CFGR_PLLMUL8                    ((uint32_t)0x00180000U)        /*!< PLL input clock*8 */
#define  RCC_CFGR_PLLMUL9                    ((uint32_t)0x001C0000U)        /*!< PLL input clock*9 */
#define  RCC_CFGR_PLLMUL10                   ((uint32_t)0x00200000U)        /*!< PLL input clock10 */
#define  RCC_CFGR_PLLMUL11                   ((uint32_t)0x00240000U)        /*!< PLL input clock*11 */
#define  RCC_CFGR_PLLMUL12                   ((uint32_t)0x00280000U)        /*!< PLL input clock*12 */
#define  RCC_CFGR_PLLMUL13                   ((uint32_t)0x002C0000U)        /*!< PLL input clock*13 */
#define  RCC_CFGR_PLLMUL14                   ((uint32_t)0x00300000U)        /*!< PLL input clock*14 */
#define  RCC_CFGR_PLLMUL15                   ((uint32_t)0x00340000U)        /*!< PLL input clock*15 */
#define  RCC_CFGR_PLLMUL16                   ((uint32_t)0x00380000U)        /*!< PLL input clock*16 */

/*!< MCO configuration */
#define  RCC_CFGR_MCO                        ((uint32_t)0x0F000000U)        /*!< MCO[3:0] bits (Microcontroller Clock Output) */
#define  RCC_CFGR_MCO_0                      ((uint32_t)0x01000000U)        /*!< Bit 0 */
#define  RCC_CFGR_MCO_1                      ((uint32_t)0x02000000U)        /*!< Bit 1 */
#define  RCC_CFGR_MCO_2                      ((uint32_t)0x04000000U)        /*!< Bit 2 */

#define  RCC_CFGR_MCO_NOCLOCK                ((uint32_t)0x00000000U)        /*!< No clock */
#define  RCC_CFGR_MCO_HSI14                  ((uint32_t)0x01000000U)        /*!< HSI14 clock selected as MCO source */
#define  RCC_CFGR_MCO_LSI                    ((uint32_t)0x02000000U)        /*!< LSI clock selected as MCO source */
#define  RCC_CFGR_MCO_LSE                    ((uint32_t)0x03000000U)        /*!< LSE clock selected as MCO source */
#define  RCC_CFGR_MCO_SYSCLK                 ((uint32_t)0x04000000U)        /*!< System clock selected as MCO source */
#define  RCC_CFGR_MCO_HSI                    ((uint32_t)0x05000000U)        /*!< HSI clock selected as MCO source */
#define  RCC_CFGR_MCO_HSE                    ((uint32_t)0x06000000U)        /*!< HSE clock selected as MCO source  */
#define  RCC_CFGR_MCO_PLL                    ((uint32_t)0x07000000U)        /*!< PLL clock divided by 2 selected as MCO source */

/* Reference defines */
#define RCC_CFGR_MCOSEL                      RCC_CFGR_MCO
#define RCC_CFGR_MCOSEL_0                    RCC_CFGR_MCO_0
#define RCC_CFGR_MCOSEL_1                    RCC_CFGR_MCO_1
#define RCC_CFGR_MCOSEL_2                    RCC_CFGR_MCO_2
#define RCC_CFGR_MCOSEL_NOCLOCK              RCC_CFGR_MCO_NOCLOCK
#define RCC_CFGR_MCOSEL_HSI14                RCC_CFGR_MCO_HSI14
#define RCC_CFGR_MCOSEL_LSI                  RCC_CFGR_MCO_LSI
#define RCC_CFGR_MCOSEL_LSE                  RCC_CFGR_MCO_LSE
#define RCC_CFGR_MCOSEL_SYSCLK               RCC_CFGR_MCO_SYSCLK
#define RCC_CFGR_MCOSEL_HSI                  RCC_CFGR_MCO_HSI
#define RCC_CFGR_MCOSEL_HSE                  RCC_CFGR_MCO_HSE
#define RCC_CFGR_MCOSEL_PLL_DIV2             RCC_CFGR_MCO_PLL

/*!<******************  Bit definition for RCC_CIR register  *****************/
#define  RCC_CIR_LSIRDYF                     ((uint32_t)0x00000001U)        /*!< LSI Ready Interrupt flag */
#define  RCC_CIR_LSERDYF                     ((uint32_t)0x00000002U)        /*!< LSE Ready Interrupt flag */
#define  RCC_CIR_HSIRDYF                     ((uint32_t)0x00000004U)        /*!< HSI Ready Interrupt flag */
#define  RCC_CIR_HSERDYF                     ((uint32_t)0x00000008U)        /*!< HSE Ready Interrupt flag */
#define  RCC_CIR_PLLRDYF                     ((uint32_t)0x00000010U)        /*!< PLL Ready Interrupt flag */
#define  RCC_CIR_HSI14RDYF                   ((uint32_t)0x00000020U)        /*!< HSI14 Ready Interrupt flag */
#define  RCC_CIR_CSSF                        ((uint32_t)0x00000080U)        /*!< Clock Security System Interrupt flag */
#define  RCC_CIR_LSIRDYIE                    ((uint32_t)0x00000100U)        /*!< LSI Ready Interrupt Enable */
#define  RCC_CIR_LSERDYIE                    ((uint32_t)0x00000200U)        /*!< LSE Ready Interrupt Enable */
#define  RCC_CIR_HSIRDYIE                    ((uint32_t)0x00000400U)        /*!< HSI Ready Interrupt Enable */
#define  RCC_CIR_HSERDYIE                    ((uint32_t)0x00000800U)        /*!< HSE Ready Interrupt Enable */
#define  RCC_CIR_PLLRDYIE                    ((uint32_t)0x00001000U)        /*!< PLL Ready Interrupt Enable */
#define  RCC_CIR_HSI14RDYIE                  ((uint32_t)0x00002000U)        /*!< HSI14 Ready Interrupt Enable */
#define  RCC_CIR_LSIRDYC                     ((uint32_t)0x00010000U)        /*!< LSI Ready Interrupt Clear */
#define  RCC_CIR_LSERDYC                     ((uint32_t)0x00020000U)        /*!< LSE Ready Interrupt Clear */
#define  RCC_CIR_HSIRDYC                     ((uint32_t)0x00040000U)        /*!< HSI Ready Interrupt Clear */
#define  RCC_CIR_HSERDYC                     ((uint32_t)0x00080000U)        /*!< HSE Ready Interrupt Clear */
#define  RCC_CIR_PLLRDYC                     ((uint32_t)0x00100000U)        /*!< PLL Ready Interrupt Clear */
#define  RCC_CIR_HSI14RDYC                   ((uint32_t)0x00200000U)        /*!< HSI14 Ready Interrupt Clear */
#define  RCC_CIR_CSSC                        ((uint32_t)0x00800000U)        /*!< Clock Security System Interrupt Clear */

/*****************  Bit definition for RCC_APB2RSTR register  ****************/
#define  RCC_APB2RSTR_SYSCFGRST              ((uint32_t)0x00000001U)        /*!< SYSCFG clock reset */
#define  RCC_APB2RSTR_ADCRST                 ((uint32_t)0x00000200U)        /*!< ADC clock reset */
#define  RCC_APB2RSTR_TIM1RST                ((uint32_t)0x00000800U)        /*!< TIM1 clock reset */
#define  RCC_APB2RSTR_SPI1RST                ((uint32_t)0x00001000U)        /*!< SPI1 clock reset */
#define  RCC_APB2RSTR_USART1RST              ((uint32_t)0x00004000U)        /*!< USART1 clock reset */
#define  RCC_APB2RSTR_TIM15RST               ((uint32_t)0x00010000U)        /*!< TIM15 clock reset */
#define  RCC_APB2RSTR_TIM16RST               ((uint32_t)0x00020000U)        /*!< TIM16 clock reset */
#define  RCC_APB2RSTR_TIM17RST               ((uint32_t)0x00040000U)        /*!< TIM17 clock reset */
#define  RCC_APB2RSTR_DBGMCURST              ((uint32_t)0x00400000U)        /*!< DBGMCU clock reset */

/*!< Old ADC1 clock reset bit definition maintained for legacy purpose */
#define  RCC_APB2RSTR_ADC1RST                RCC_APB2RSTR_ADCRST          

/*****************  Bit definition for RCC_APB1RSTR register  ****************/
#define  RCC_APB1RSTR_TIM2RST                ((uint32_t)0x00000001U)        /*!< Timer 2 clock reset */
#define  RCC_APB1RSTR_TIM3RST                ((uint32_t)0x00000002U)        /*!< Timer 3 clock reset */
#define  RCC_APB1RSTR_TIM6RST                ((uint32_t)0x00000010U)        /*!< Timer 6 clock reset */
#define  RCC_APB1RSTR_TIM14RST               ((uint32_t)0x00000100U)        /*!< Timer 14 clock reset */
#define  RCC_APB1RSTR_WWDGRST                ((uint32_t)0x00000800U)        /*!< Window Watchdog clock reset */
#define  RCC_APB1RSTR_SPI2RST                ((uint32_t)0x00004000U)        /*!< SPI2 clock reset */
#define  RCC_APB1RSTR_USART2RST              ((uint32_t)0x00020000U)        /*!< USART 2 clock reset */
#define  RCC_APB1RSTR_I2C1RST                ((uint32_t)0x00200000U)        /*!< I2C 1 clock reset */
#define  RCC_APB1RSTR_I2C2RST                ((uint32_t)0x00400000U)        /*!< I2C 2 clock reset */
#define  RCC_APB1RSTR_PWRRST                 ((uint32_t)0x10000000U)        /*!< PWR clock reset */
#define  RCC_APB1RSTR_DACRST                 ((uint32_t)0x20000000U)        /*!< DAC clock reset */
#define  RCC_APB1RSTR_CECRST                 ((uint32_t)0x40000000U)        /*!< CEC clock reset */

/******************  Bit definition for RCC_AHBENR register  *****************/
#define  RCC_AHBENR_DMAEN                    ((uint32_t)0x00000001U)        /*!< DMA1 clock enable */
#define  RCC_AHBENR_SRAMEN                   ((uint32_t)0x00000004U)        /*!< SRAM interface clock enable */
#define  RCC_AHBENR_FLITFEN                  ((uint32_t)0x00000010U)        /*!< FLITF clock enable */
#define  RCC_AHBENR_CRCEN                    ((uint32_t)0x00000040U)        /*!< CRC clock enable */
#define  RCC_AHBENR_GPIOAEN                  ((uint32_t)0x00020000U)        /*!< GPIOA clock enable */
#define  RCC_AHBENR_GPIOBEN                  ((uint32_t)0x00040000U)        /*!< GPIOB clock enable */
#define  RCC_AHBENR_GPIOCEN                  ((uint32_t)0x00080000U)        /*!< GPIOC clock enable */
#define  RCC_AHBENR_GPIODEN                  ((uint32_t)0x00100000U)        /*!< GPIOD clock enable */
#define  RCC_AHBENR_GPIOFEN                  ((uint32_t)0x00400000U)        /*!< GPIOF clock enable */
#define  RCC_AHBENR_TSCEN                    ((uint32_t)0x01000000U)        /*!< TS controller clock enable */

/* Old Bit definition maintained for legacy purpose */
#define  RCC_AHBENR_DMA1EN                   RCC_AHBENR_DMAEN        /*!< DMA1 clock enable */
#define  RCC_AHBENR_TSEN                     RCC_AHBENR_TSCEN        /*!< TS clock enable */

/*****************  Bit definition for RCC_APB2ENR register  *****************/
#define  RCC_APB2ENR_SYSCFGCOMPEN            ((uint32_t)0x00000001U)        /*!< SYSCFG and comparator clock enable */
#define  RCC_APB2ENR_ADCEN                   ((uint32_t)0x00000200U)        /*!< ADC1 clock enable */
#define  RCC_APB2ENR_TIM1EN                  ((uint32_t)0x00000800U)        /*!< TIM1 clock enable */
#define  RCC_APB2ENR_SPI1EN                  ((uint32_t)0x00001000U)        /*!< SPI1 clock enable */
#define  RCC_APB2ENR_USART1EN                ((uint32_t)0x00004000U)        /*!< USART1 clock enable */
#define  RCC_APB2ENR_TIM15EN                 ((uint32_t)0x00010000U)        /*!< TIM15 clock enable */
#define  RCC_APB2ENR_TIM16EN                 ((uint32_t)0x00020000U)        /*!< TIM16 clock enable */
#define  RCC_APB2ENR_TIM17EN                 ((uint32_t)0x00040000U)        /*!< TIM17 clock enable */
#define  RCC_APB2ENR_DBGMCUEN                ((uint32_t)0x00400000U)        /*!< DBGMCU clock enable */

/* Old Bit definition maintained for legacy purpose */
#define  RCC_APB2ENR_SYSCFGEN                RCC_APB2ENR_SYSCFGCOMPEN        /*!< SYSCFG clock enable */
#define  RCC_APB2ENR_ADC1EN                  RCC_APB2ENR_ADCEN               /*!< ADC1 clock enable */

/*****************  Bit definition for RCC_APB1ENR register  *****************/
#define  RCC_APB1ENR_TIM2EN                  ((uint32_t)0x00000001U)        /*!< Timer 2 clock enable */
#define  RCC_APB1ENR_TIM3EN                  ((uint32_t)0x00000002U)        /*!< Timer 3 clock enable */
#define  RCC_APB1ENR_TIM6EN                  ((uint32_t)0x00000010U)        /*!< Timer 6 clock enable */
#define  RCC_APB1ENR_TIM14EN                 ((uint32_t)0x00000100U)        /*!< Timer 14 clock enable */
#define  RCC_APB1ENR_WWDGEN                  ((uint32_t)0x00000800U)        /*!< Window Watchdog clock enable */
#define  RCC_APB1ENR_SPI2EN                  ((uint32_t)0x00004000U)        /*!< SPI2 clock enable */
#define  RCC_APB1ENR_USART2EN                ((uint32_t)0x00020000U)        /*!< USART2 clock enable */
#define  RCC_APB1ENR_I2C1EN                  ((uint32_t)0x00200000U)        /*!< I2C1 clock enable */
#define  RCC_APB1ENR_I2C2EN                  ((uint32_t)0x00400000U)        /*!< I2C2 clock enable */
#define  RCC_APB1ENR_PWREN                   ((uint32_t)0x10000000U)        /*!< PWR clock enable */
#define  RCC_APB1ENR_DACEN                   ((uint32_t)0x20000000U)        /*!< DAC clock enable */
#define  RCC_APB1ENR_CECEN                   ((uint32_t)0x40000000U)        /*!< CEC clock enable */

/*******************  Bit definition for RCC_BDCR register  ******************/
#define  RCC_BDCR_LSEON                      ((uint32_t)0x00000001U)        /*!< External Low Speed oscillator enable */
#define  RCC_BDCR_LSERDY                     ((uint32_t)0x00000002U)        /*!< External Low Speed oscillator Ready */
#define  RCC_BDCR_LSEBYP                     ((uint32_t)0x00000004U)        /*!< External Low Speed oscillator Bypass */

#define  RCC_BDCR_LSEDRV                     ((uint32_t)0x00000018U)        /*!< LSEDRV[1:0] bits (LSE Osc. drive capability) */
#define  RCC_BDCR_LSEDRV_0                   ((uint32_t)0x00000008U)        /*!< Bit 0 */
#define  RCC_BDCR_LSEDRV_1                   ((uint32_t)0x00000010U)        /*!< Bit 1 */

#define  RCC_BDCR_RTCSEL                     ((uint32_t)0x00000300U)        /*!< RTCSEL[1:0] bits (RTC clock source selection) */
#define  RCC_BDCR_RTCSEL_0                   ((uint32_t)0x00000100U)        /*!< Bit 0 */
#define  RCC_BDCR_RTCSEL_1                   ((uint32_t)0x00000200U)        /*!< Bit 1 */

/*!< RTC configuration */
#define  RCC_BDCR_RTCSEL_NOCLOCK             ((uint32_t)0x00000000U)        /*!< No clock */
#define  RCC_BDCR_RTCSEL_LSE                 ((uint32_t)0x00000100U)        /*!< LSE oscillator clock used as RTC clock */
#define  RCC_BDCR_RTCSEL_LSI                 ((uint32_t)0x00000200U)        /*!< LSI oscillator clock used as RTC clock */
#define  RCC_BDCR_RTCSEL_HSE                 ((uint32_t)0x00000300U)        /*!< HSE oscillator clock divided by 128 used as RTC clock */

#define  RCC_BDCR_RTCEN                      ((uint32_t)0x00008000U)        /*!< RTC clock enable */
#define  RCC_BDCR_BDRST                      ((uint32_t)0x00010000U)        /*!< Backup domain software reset  */

/*******************  Bit definition for RCC_CSR register  *******************/
#define  RCC_CSR_LSION                       ((uint32_t)0x00000001U)        /*!< Internal Low Speed oscillator enable */
#define  RCC_CSR_LSIRDY                      ((uint32_t)0x00000002U)        /*!< Internal Low Speed oscillator Ready */
#define  RCC_CSR_V18PWRRSTF                  ((uint32_t)0x00800000U)        /*!< V1.8 power domain reset flag */
#define  RCC_CSR_RMVF                        ((uint32_t)0x01000000U)        /*!< Remove reset flag */
#define  RCC_CSR_OBLRSTF                     ((uint32_t)0x02000000U)        /*!< OBL reset flag */
#define  RCC_CSR_PINRSTF                     ((uint32_t)0x04000000U)        /*!< PIN reset flag */
#define  RCC_CSR_PORRSTF                     ((uint32_t)0x08000000U)        /*!< POR/PDR reset flag */
#define  RCC_CSR_SFTRSTF                     ((uint32_t)0x10000000U)        /*!< Software Reset flag */
#define  RCC_CSR_IWDGRSTF                    ((uint32_t)0x20000000U)        /*!< Independent Watchdog reset flag */
#define  RCC_CSR_WWDGRSTF                    ((uint32_t)0x40000000U)        /*!< Window watchdog reset flag */
#define  RCC_CSR_LPWRRSTF                    ((uint32_t)0x80000000U)        /*!< Low-Power reset flag */

/* Old Bit definition maintained for legacy purpose */
#define  RCC_CSR_OBL                         RCC_CSR_OBLRSTF        /*!< OBL reset flag */

/*******************  Bit definition for RCC_AHBRSTR register  ***************/
#define  RCC_AHBRSTR_GPIOARST                ((uint32_t)0x00020000U)         /*!< GPIOA clock reset */
#define  RCC_AHBRSTR_GPIOBRST                ((uint32_t)0x00040000U)         /*!< GPIOB clock reset */
#define  RCC_AHBRSTR_GPIOCRST                ((uint32_t)0x00080000U)         /*!< GPIOC clock reset */
#define  RCC_AHBRSTR_GPIODRST                ((uint32_t)0x00100000U)         /*!< GPIOD clock reset */
#define  RCC_AHBRSTR_GPIOFRST                ((uint32_t)0x00400000U)         /*!< GPIOF clock reset */
#define  RCC_AHBRSTR_TSCRST                  ((uint32_t)0x01000000U)         /*!< TS clock reset */

/* Old Bit definition maintained for legacy purpose */
#define  RCC_AHBRSTR_TSRST                   RCC_AHBRSTR_TSCRST         /*!< TS clock reset */

/*******************  Bit definition for RCC_CFGR2 register  *****************/
/*!< PREDIV configuration */
#define  RCC_CFGR2_PREDIV                    ((uint32_t)0x0000000FU)        /*!< PREDIV[3:0] bits */
#define  RCC_CFGR2_PREDIV_0                  ((uint32_t)0x00000001U)        /*!< Bit 0 */
#define  RCC_CFGR2_PREDIV_1                  ((uint32_t)0x00000002U)        /*!< Bit 1 */
#define  RCC_CFGR2_PREDIV_2                  ((uint32_t)0x00000004U)        /*!< Bit 2 */
#define  RCC_CFGR2_PREDIV_3                  ((uint32_t)0x00000008U)        /*!< Bit 3 */

#define  RCC_CFGR2_PREDIV_DIV1               ((uint32_t)0x00000000U)        /*!< PREDIV input clock not divided */
#define  RCC_CFGR2_PREDIV_DIV2               ((uint32_t)0x00000001U)        /*!< PREDIV input clock divided by 2 */
#define  RCC_CFGR2_PREDIV_DIV3               ((uint32_t)0x00000002U)        /*!< PREDIV input clock divided by 3 */
#define  RCC_CFGR2_PREDIV_DIV4               ((uint32_t)0x00000003U)        /*!< PREDIV input clock divided by 4 */
#define  RCC_CFGR2_PREDIV_DIV5               ((uint32_t)0x00000004U)        /*!< PREDIV input clock divided by 5 */
#define  RCC_CFGR2_PREDIV_DIV6               ((uint32_t)0x00000005U)        /*!< PREDIV input clock divided by 6 */
#define  RCC_CFGR2_PREDIV_DIV7               ((uint32_t)0x00000006U)        /*!< PREDIV input clock divided by 7 */
#define  RCC_CFGR2_PREDIV_DIV8               ((uint32_t)0x00000007U)        /*!< PREDIV input clock divided by 8 */
#define  RCC_CFGR2_PREDIV_DIV9               ((uint32_t)0x00000008U)        /*!< PREDIV input clock divided by 9 */
#define  RCC_CFGR2_PREDIV_DIV10              ((uint32_t)0x00000009U)        /*!< PREDIV input clock divided by 10 */
#define  RCC_CFGR2_PREDIV_DIV11              ((uint32_t)0x0000000AU)        /*!< PREDIV input clock divided by 11 */
#define  RCC_CFGR2_PREDIV_DIV12              ((uint32_t)0x0000000BU)        /*!< PREDIV input clock divided by 12 */
#define  RCC_CFGR2_PREDIV_DIV13              ((uint32_t)0x0000000CU)        /*!< PREDIV input clock divided by 13 */
#define  RCC_CFGR2_PREDIV_DIV14              ((uint32_t)0x0000000DU)        /*!< PREDIV input clock divided by 14 */
#define  RCC_CFGR2_PREDIV_DIV15              ((uint32_t)0x0000000EU)        /*!< PREDIV input clock divided by 15 */
#define  RCC_CFGR2_PREDIV_DIV16              ((uint32_t)0x0000000FU)        /*!< PREDIV input clock divided by 16 */

/*******************  Bit definition for RCC_CFGR3 register  *****************/
/*!< USART1 Clock source selection */
#define  RCC_CFGR3_USART1SW                  ((uint32_t)0x00000003U)        /*!< USART1SW[1:0] bits */
#define  RCC_CFGR3_USART1SW_0                ((uint32_t)0x00000001U)        /*!< Bit 0 */
#define  RCC_CFGR3_USART1SW_1                ((uint32_t)0x00000002U)        /*!< Bit 1 */

#define  RCC_CFGR3_USART1SW_PCLK             ((uint32_t)0x00000000U)        /*!< PCLK clock used as USART1 clock source */
#define  RCC_CFGR3_USART1SW_SYSCLK           ((uint32_t)0x00000001U)        /*!< System clock selected as USART1 clock source */
#define  RCC_CFGR3_USART1SW_LSE              ((uint32_t)0x00000002U)        /*!< LSE oscillator clock used as USART1 clock source */
#define  RCC_CFGR3_USART1SW_HSI              ((uint32_t)0x00000003U)        /*!< HSI oscillator clock used as USART1 clock source */

/*!< I2C1 Clock source selection */
#define  RCC_CFGR3_I2C1SW                    ((uint32_t)0x00000010U)        /*!< I2C1SW bits */ 

#define  RCC_CFGR3_I2C1SW_HSI                ((uint32_t)0x00000000U)        /*!< HSI oscillator clock used as I2C1 clock source */
#define  RCC_CFGR3_I2C1SW_SYSCLK             ((uint32_t)0x00000010U)        /*!< System clock selected as I2C1 clock source */

/*!< CEC Clock source selection */
#define  RCC_CFGR3_CECSW                     ((uint32_t)0x00000040U)        /*!< CECSW bits */ 

#define  RCC_CFGR3_CECSW_HSI_DIV244          ((uint32_t)0x00000000U)        /*!< HSI clock divided by 244 selected as HDMI CEC entry clock source */
#define  RCC_CFGR3_CECSW_LSE                 ((uint32_t)0x00000040U)        /*!< LSE clock selected as HDMI CEC entry clock source */

/*******************  Bit definition for RCC_CR2 register  *******************/
#define  RCC_CR2_HSI14ON                     ((uint32_t)0x00000001U)        /*!< Internal High Speed 14MHz clock enable */
#define  RCC_CR2_HSI14RDY                    ((uint32_t)0x00000002U)        /*!< Internal High Speed 14MHz clock ready flag */
#define  RCC_CR2_HSI14DIS                    ((uint32_t)0x00000004U)        /*!< Internal High Speed 14MHz clock disable */
#define  RCC_CR2_HSI14TRIM                   ((uint32_t)0x000000F8U)        /*!< Internal High Speed 14MHz clock trimming */
#define  RCC_CR2_HSI14CAL                    ((uint32_t)0x0000FF00U)        /*!< Internal High Speed 14MHz clock Calibration */

/*****************************************************************************/
/*                                                                           */
/*                           Real-Time Clock (RTC)                           */
/*                                                                           */
/*****************************************************************************/
/*
* @brief Specific device feature definitions  (not present on all devices in the STM32F0 family)
*/
#define RTC_TAMPER1_SUPPORT  /*!< TAMPER 1 feature support */
#define RTC_BACKUP_SUPPORT   /*!< BACKUP register feature support */

/********************  Bits definition for RTC_TR register  ******************/
#define RTC_TR_PM                            ((uint32_t)0x00400000U)
#define RTC_TR_HT                            ((uint32_t)0x00300000U)
#define RTC_TR_HT_0                          ((uint32_t)0x00100000U)
#define RTC_TR_HT_1                          ((uint32_t)0x00200000U)
#define RTC_TR_HU                            ((uint32_t)0x000F0000U)
#define RTC_TR_HU_0                          ((uint32_t)0x00010000U)
#define RTC_TR_HU_1                          ((uint32_t)0x00020000U)
#define RTC_TR_HU_2                          ((uint32_t)0x00040000U)
#define RTC_TR_HU_3                          ((uint32_t)0x00080000U)
#define RTC_TR_MNT                           ((uint32_t)0x00007000U)
#define RTC_TR_MNT_0                         ((uint32_t)0x00001000U)
#define RTC_TR_MNT_1                         ((uint32_t)0x00002000U)
#define RTC_TR_MNT_2                         ((uint32_t)0x00004000U)
#define RTC_TR_MNU                           ((uint32_t)0x00000F00U)
#define RTC_TR_MNU_0                         ((uint32_t)0x00000100U)
#define RTC_TR_MNU_1                         ((uint32_t)0x00000200U)
#define RTC_TR_MNU_2                         ((uint32_t)0x00000400U)
#define RTC_TR_MNU_3                         ((uint32_t)0x00000800U)
#define RTC_TR_ST                            ((uint32_t)0x00000070U)
#define RTC_TR_ST_0                          ((uint32_t)0x00000010U)
#define RTC_TR_ST_1                          ((uint32_t)0x00000020U)
#define RTC_TR_ST_2                          ((uint32_t)0x00000040U)
#define RTC_TR_SU                            ((uint32_t)0x0000000FU)
#define RTC_TR_SU_0                          ((uint32_t)0x00000001U)
#define RTC_TR_SU_1                          ((uint32_t)0x00000002U)
#define RTC_TR_SU_2                          ((uint32_t)0x00000004U)
#define RTC_TR_SU_3                          ((uint32_t)0x00000008U)

/********************  Bits definition for RTC_DR register  ******************/
#define RTC_DR_YT                            ((uint32_t)0x00F00000U)
#define RTC_DR_YT_0                          ((uint32_t)0x00100000U)
#define RTC_DR_YT_1                          ((uint32_t)0x00200000U)
#define RTC_DR_YT_2                          ((uint32_t)0x00400000U)
#define RTC_DR_YT_3                          ((uint32_t)0x00800000U)
#define RTC_DR_YU                            ((uint32_t)0x000F0000U)
#define RTC_DR_YU_0                          ((uint32_t)0x00010000U)
#define RTC_DR_YU_1                          ((uint32_t)0x00020000U)
#define RTC_DR_YU_2                          ((uint32_t)0x00040000U)
#define RTC_DR_YU_3                          ((uint32_t)0x00080000U)
#define RTC_DR_WDU                           ((uint32_t)0x0000E000U)
#define RTC_DR_WDU_0                         ((uint32_t)0x00002000U)
#define RTC_DR_WDU_1                         ((uint32_t)0x00004000U)
#define RTC_DR_WDU_2                         ((uint32_t)0x00008000U)
#define RTC_DR_MT                            ((uint32_t)0x00001000U)
#define RTC_DR_MU                            ((uint32_t)0x00000F00U)
#define RTC_DR_MU_0                          ((uint32_t)0x00000100U)
#define RTC_DR_MU_1                          ((uint32_t)0x00000200U)
#define RTC_DR_MU_2                          ((uint32_t)0x00000400U)
#define RTC_DR_MU_3                          ((uint32_t)0x00000800U)
#define RTC_DR_DT                            ((uint32_t)0x00000030U)
#define RTC_DR_DT_0                          ((uint32_t)0x00000010U)
#define RTC_DR_DT_1                          ((uint32_t)0x00000020U)
#define RTC_DR_DU                            ((uint32_t)0x0000000FU)
#define RTC_DR_DU_0                          ((uint32_t)0x00000001U)
#define RTC_DR_DU_1                          ((uint32_t)0x00000002U)
#define RTC_DR_DU_2                          ((uint32_t)0x00000004U)
#define RTC_DR_DU_3                          ((uint32_t)0x00000008U)

/********************  Bits definition for RTC_CR register  ******************/
#define RTC_CR_COE                           ((uint32_t)0x00800000U)
#define RTC_CR_OSEL                          ((uint32_t)0x00600000U)
#define RTC_CR_OSEL_0                        ((uint32_t)0x00200000U)
#define RTC_CR_OSEL_1                        ((uint32_t)0x00400000U)
#define RTC_CR_POL                           ((uint32_t)0x00100000U)
#define RTC_CR_COSEL                         ((uint32_t)0x00080000U)
#define RTC_CR_BCK                           ((uint32_t)0x00040000U)
#define RTC_CR_SUB1H                         ((uint32_t)0x00020000U)
#define RTC_CR_ADD1H                         ((uint32_t)0x00010000U)
#define RTC_CR_TSIE                          ((uint32_t)0x00008000U)
#define RTC_CR_ALRAIE                        ((uint32_t)0x00001000U)
#define RTC_CR_TSE                           ((uint32_t)0x00000800U)
#define RTC_CR_ALRAE                         ((uint32_t)0x00000100U)
#define RTC_CR_FMT                           ((uint32_t)0x00000040U)
#define RTC_CR_BYPSHAD                       ((uint32_t)0x00000020U)
#define RTC_CR_REFCKON                       ((uint32_t)0x00000010U)
#define RTC_CR_TSEDGE                        ((uint32_t)0x00000008U)

/********************  Bits definition for RTC_ISR register  *****************/
#define RTC_ISR_RECALPF                      ((uint32_t)0x00010000U)
#define RTC_ISR_TAMP2F                       ((uint32_t)0x00004000U)
#define RTC_ISR_TAMP1F                       ((uint32_t)0x00002000U)
#define RTC_ISR_TSOVF                        ((uint32_t)0x00001000U)
#define RTC_ISR_TSF                          ((uint32_t)0x00000800U)
#define RTC_ISR_ALRAF                        ((uint32_t)0x00000100U)
#define RTC_ISR_INIT                         ((uint32_t)0x00000080U)
#define RTC_ISR_INITF                        ((uint32_t)0x00000040U)
#define RTC_ISR_RSF                          ((uint32_t)0x00000020U)
#define RTC_ISR_INITS                        ((uint32_t)0x00000010U)
#define RTC_ISR_SHPF                         ((uint32_t)0x00000008U)
#define RTC_ISR_ALRAWF                       ((uint32_t)0x00000001U)

/********************  Bits definition for RTC_PRER register  ****************/
#define RTC_PRER_PREDIV_A                    ((uint32_t)0x007F0000U)
#define RTC_PRER_PREDIV_S                    ((uint32_t)0x00007FFFU)

/********************  Bits definition for RTC_ALRMAR register  **************/
#define RTC_ALRMAR_MSK4                      ((uint32_t)0x80000000U)
#define RTC_ALRMAR_WDSEL                     ((uint32_t)0x40000000U)
#define RTC_ALRMAR_DT                        ((uint32_t)0x30000000U)
#define RTC_ALRMAR_DT_0                      ((uint32_t)0x10000000U)
#define RTC_ALRMAR_DT_1                      ((uint32_t)0x20000000U)
#define RTC_ALRMAR_DU                        ((uint32_t)0x0F000000U)
#define RTC_ALRMAR_DU_0                      ((uint32_t)0x01000000U)
#define RTC_ALRMAR_DU_1                      ((uint32_t)0x02000000U)
#define RTC_ALRMAR_DU_2                      ((uint32_t)0x04000000U)
#define RTC_ALRMAR_DU_3                      ((uint32_t)0x08000000U)
#define RTC_ALRMAR_MSK3                      ((uint32_t)0x00800000U)
#define RTC_ALRMAR_PM                        ((uint32_t)0x00400000U)
#define RTC_ALRMAR_HT                        ((uint32_t)0x00300000U)
#define RTC_ALRMAR_HT_0                      ((uint32_t)0x00100000U)
#define RTC_ALRMAR_HT_1                      ((uint32_t)0x00200000U)
#define RTC_ALRMAR_HU                        ((uint32_t)0x000F0000U)
#define RTC_ALRMAR_HU_0                      ((uint32_t)0x00010000U)
#define RTC_ALRMAR_HU_1                      ((uint32_t)0x00020000U)
#define RTC_ALRMAR_HU_2                      ((uint32_t)0x00040000U)
#define RTC_ALRMAR_HU_3                      ((uint32_t)0x00080000U)
#define RTC_ALRMAR_MSK2                      ((uint32_t)0x00008000U)
#define RTC_ALRMAR_MNT                       ((uint32_t)0x00007000U)
#define RTC_ALRMAR_MNT_0                     ((uint32_t)0x00001000U)
#define RTC_ALRMAR_MNT_1                     ((uint32_t)0x00002000U)
#define RTC_ALRMAR_MNT_2                     ((uint32_t)0x00004000U)
#define RTC_ALRMAR_MNU                       ((uint32_t)0x00000F00U)
#define RTC_ALRMAR_MNU_0                     ((uint32_t)0x00000100U)
#define RTC_ALRMAR_MNU_1                     ((uint32_t)0x00000200U)
#define RTC_ALRMAR_MNU_2                     ((uint32_t)0x00000400U)
#define RTC_ALRMAR_MNU_3                     ((uint32_t)0x00000800U)
#define RTC_ALRMAR_MSK1                      ((uint32_t)0x00000080U)
#define RTC_ALRMAR_ST                        ((uint32_t)0x00000070U)
#define RTC_ALRMAR_ST_0                      ((uint32_t)0x00000010U)
#define RTC_ALRMAR_ST_1                      ((uint32_t)0x00000020U)
#define RTC_ALRMAR_ST_2                      ((uint32_t)0x00000040U)
#define RTC_ALRMAR_SU                        ((uint32_t)0x0000000FU)
#define RTC_ALRMAR_SU_0                      ((uint32_t)0x00000001U)
#define RTC_ALRMAR_SU_1                      ((uint32_t)0x00000002U)
#define RTC_ALRMAR_SU_2                      ((uint32_t)0x00000004U)
#define RTC_ALRMAR_SU_3                      ((uint32_t)0x00000008U)

/********************  Bits definition for RTC_WPR register  *****************/
#define RTC_WPR_KEY                          ((uint32_t)0x000000FFU)

/********************  Bits definition for RTC_SSR register  *****************/
#define RTC_SSR_SS                           ((uint32_t)0x0000FFFFU)

/********************  Bits definition for RTC_SHIFTR register  **************/
#define RTC_SHIFTR_SUBFS                     ((uint32_t)0x00007FFFU)
#define RTC_SHIFTR_ADD1S                     ((uint32_t)0x80000000U)

/********************  Bits definition for RTC_TSTR register  ****************/
#define RTC_TSTR_PM                          ((uint32_t)0x00400000U)
#define RTC_TSTR_HT                          ((uint32_t)0x00300000U)
#define RTC_TSTR_HT_0                        ((uint32_t)0x00100000U)
#define RTC_TSTR_HT_1                        ((uint32_t)0x00200000U)
#define RTC_TSTR_HU                          ((uint32_t)0x000F0000U)
#define RTC_TSTR_HU_0                        ((uint32_t)0x00010000U)
#define RTC_TSTR_HU_1                        ((uint32_t)0x00020000U)
#define RTC_TSTR_HU_2                        ((uint32_t)0x00040000U)
#define RTC_TSTR_HU_3                        ((uint32_t)0x00080000U)
#define RTC_TSTR_MNT                         ((uint32_t)0x00007000U)
#define RTC_TSTR_MNT_0                       ((uint32_t)0x00001000U)
#define RTC_TSTR_MNT_1                       ((uint32_t)0x00002000U)
#define RTC_TSTR_MNT_2                       ((uint32_t)0x00004000U)
#define RTC_TSTR_MNU                         ((uint32_t)0x00000F00U)
#define RTC_TSTR_MNU_0                       ((uint32_t)0x00000100U)
#define RTC_TSTR_MNU_1                       ((uint32_t)0x00000200U)
#define RTC_TSTR_MNU_2                       ((uint32_t)0x00000400U)
#define RTC_TSTR_MNU_3                       ((uint32_t)0x00000800U)
#define RTC_TSTR_ST                          ((uint32_t)0x00000070U)
#define RTC_TSTR_ST_0                        ((uint32_t)0x00000010U)
#define RTC_TSTR_ST_1                        ((uint32_t)0x00000020U)
#define RTC_TSTR_ST_2                        ((uint32_t)0x00000040U)
#define RTC_TSTR_SU                          ((uint32_t)0x0000000FU)
#define RTC_TSTR_SU_0                        ((uint32_t)0x00000001U)
#define RTC_TSTR_SU_1                        ((uint32_t)0x00000002U)
#define RTC_TSTR_SU_2                        ((uint32_t)0x00000004U)
#define RTC_TSTR_SU_3                        ((uint32_t)0x00000008U)

/********************  Bits definition for RTC_TSDR register  ****************/
#define RTC_TSDR_WDU                         ((uint32_t)0x0000E000U)
#define RTC_TSDR_WDU_0                       ((uint32_t)0x00002000U)
#define RTC_TSDR_WDU_1                       ((uint32_t)0x00004000U)
#define RTC_TSDR_WDU_2                       ((uint32_t)0x00008000U)
#define RTC_TSDR_MT                          ((uint32_t)0x00001000U)
#define RTC_TSDR_MU                          ((uint32_t)0x00000F00U)
#define RTC_TSDR_MU_0                        ((uint32_t)0x00000100U)
#define RTC_TSDR_MU_1                        ((uint32_t)0x00000200U)
#define RTC_TSDR_MU_2                        ((uint32_t)0x00000400U)
#define RTC_TSDR_MU_3                        ((uint32_t)0x00000800U)
#define RTC_TSDR_DT                          ((uint32_t)0x00000030U)
#define RTC_TSDR_DT_0                        ((uint32_t)0x00000010U)
#define RTC_TSDR_DT_1                        ((uint32_t)0x00000020U)
#define RTC_TSDR_DU                          ((uint32_t)0x0000000FU)
#define RTC_TSDR_DU_0                        ((uint32_t)0x00000001U)
#define RTC_TSDR_DU_1                        ((uint32_t)0x00000002U)
#define RTC_TSDR_DU_2                        ((uint32_t)0x00000004U)
#define RTC_TSDR_DU_3                        ((uint32_t)0x00000008U)

/********************  Bits definition for RTC_TSSSR register  ***************/
#define RTC_TSSSR_SS                         ((uint32_t)0x0000FFFFU)

/********************  Bits definition for RTC_CALR register  ****************/
#define RTC_CALR_CALP                        ((uint32_t)0x00008000U)
#define RTC_CALR_CALW8                       ((uint32_t)0x00004000U)
#define RTC_CALR_CALW16                      ((uint32_t)0x00002000U)
#define RTC_CALR_CALM                        ((uint32_t)0x000001FFU)
#define RTC_CALR_CALM_0                      ((uint32_t)0x00000001U)
#define RTC_CALR_CALM_1                      ((uint32_t)0x00000002U)
#define RTC_CALR_CALM_2                      ((uint32_t)0x00000004U)
#define RTC_CALR_CALM_3                      ((uint32_t)0x00000008U)
#define RTC_CALR_CALM_4                      ((uint32_t)0x00000010U)
#define RTC_CALR_CALM_5                      ((uint32_t)0x00000020U)
#define RTC_CALR_CALM_6                      ((uint32_t)0x00000040U)
#define RTC_CALR_CALM_7                      ((uint32_t)0x00000080U)
#define RTC_CALR_CALM_8                      ((uint32_t)0x00000100U)

/********************  Bits definition for RTC_TAFCR register  ***************/
#define RTC_TAFCR_PC15MODE                   ((uint32_t)0x00800000U)
#define RTC_TAFCR_PC15VALUE                  ((uint32_t)0x00400000U)
#define RTC_TAFCR_PC14MODE                   ((uint32_t)0x00200000U)
#define RTC_TAFCR_PC14VALUE                  ((uint32_t)0x00100000U)
#define RTC_TAFCR_PC13MODE                   ((uint32_t)0x00080000U)
#define RTC_TAFCR_PC13VALUE                  ((uint32_t)0x00040000U)
#define RTC_TAFCR_TAMPPUDIS                  ((uint32_t)0x00008000U)
#define RTC_TAFCR_TAMPPRCH                   ((uint32_t)0x00006000U)
#define RTC_TAFCR_TAMPPRCH_0                 ((uint32_t)0x00002000U)
#define RTC_TAFCR_TAMPPRCH_1                 ((uint32_t)0x00004000U)
#define RTC_TAFCR_TAMPFLT                    ((uint32_t)0x00001800U)
#define RTC_TAFCR_TAMPFLT_0                  ((uint32_t)0x00000800U)
#define RTC_TAFCR_TAMPFLT_1                  ((uint32_t)0x00001000U)
#define RTC_TAFCR_TAMPFREQ                   ((uint32_t)0x00000700U)
#define RTC_TAFCR_TAMPFREQ_0                 ((uint32_t)0x00000100U)
#define RTC_TAFCR_TAMPFREQ_1                 ((uint32_t)0x00000200U)
#define RTC_TAFCR_TAMPFREQ_2                 ((uint32_t)0x00000400U)
#define RTC_TAFCR_TAMPTS                     ((uint32_t)0x00000080U)
#define RTC_TAFCR_TAMP2TRG                   ((uint32_t)0x00000010U)
#define RTC_TAFCR_TAMP2E                     ((uint32_t)0x00000008U)
#define RTC_TAFCR_TAMPIE                     ((uint32_t)0x00000004U)
#define RTC_TAFCR_TAMP1TRG                   ((uint32_t)0x00000002U)
#define RTC_TAFCR_TAMP1E                     ((uint32_t)0x00000001U)

/* Reference defines */
#define RTC_TAFCR_ALARMOUTTYPE               RTC_TAFCR_PC13VALUE

/********************  Bits definition for RTC_ALRMASSR register  ************/
#define RTC_ALRMASSR_MASKSS                  ((uint32_t)0x0F000000U)
#define RTC_ALRMASSR_MASKSS_0                ((uint32_t)0x01000000U)
#define RTC_ALRMASSR_MASKSS_1                ((uint32_t)0x02000000U)
#define RTC_ALRMASSR_MASKSS_2                ((uint32_t)0x04000000U)
#define RTC_ALRMASSR_MASKSS_3                ((uint32_t)0x08000000U)
#define RTC_ALRMASSR_SS                      ((uint32_t)0x00007FFFU)

/********************  Bits definition for RTC_BKP0R register  ***************/
#define RTC_BKP0R                            ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP1R register  ***************/
#define RTC_BKP1R                            ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP2R register  ***************/
#define RTC_BKP2R                            ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP3R register  ***************/
#define RTC_BKP3R                            ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP4R register  ***************/
#define RTC_BKP4R                            ((uint32_t)0xFFFFFFFFU)

/******************** Number of backup registers ******************************/
#define RTC_BKP_NUMBER                       ((uint32_t)0x00000005U)

/*****************************************************************************/
/*                                                                           */
/*                        Serial Peripheral Interface (SPI)                  */
/*                                                                           */
/*****************************************************************************/
/*******************  Bit definition for SPI_CR1 register  *******************/
#define  SPI_CR1_CPHA                        ((uint32_t)0x00000001U)            /*!< Clock Phase */
#define  SPI_CR1_CPOL                        ((uint32_t)0x00000002U)            /*!< Clock Polarity */
#define  SPI_CR1_MSTR                        ((uint32_t)0x00000004U)            /*!< Master Selection */
#define  SPI_CR1_BR                          ((uint32_t)0x00000038U)            /*!< BR[2:0] bits (Baud Rate Control) */
#define  SPI_CR1_BR_0                        ((uint32_t)0x00000008U)            /*!< Bit 0 */
#define  SPI_CR1_BR_1                        ((uint32_t)0x00000010U)            /*!< Bit 1 */
#define  SPI_CR1_BR_2                        ((uint32_t)0x00000020U)            /*!< Bit 2 */
#define  SPI_CR1_SPE                         ((uint32_t)0x00000040U)            /*!< SPI Enable */
#define  SPI_CR1_LSBFIRST                    ((uint32_t)0x00000080U)            /*!< Frame Format */
#define  SPI_CR1_SSI                         ((uint32_t)0x00000100U)            /*!< Internal slave select */
#define  SPI_CR1_SSM                         ((uint32_t)0x00000200U)            /*!< Software slave management */
#define  SPI_CR1_RXONLY                      ((uint32_t)0x00000400U)            /*!< Receive only */
#define  SPI_CR1_CRCL                        ((uint32_t)0x00000800U)            /*!< CRC Length */
#define  SPI_CR1_CRCNEXT                     ((uint32_t)0x00001000U)            /*!< Transmit CRC next */
#define  SPI_CR1_CRCEN                       ((uint32_t)0x00002000U)            /*!< Hardware CRC calculation enable */
#define  SPI_CR1_BIDIOE                      ((uint32_t)0x00004000U)            /*!< Output enable in bidirectional mode */
#define  SPI_CR1_BIDIMODE                    ((uint32_t)0x00008000U)            /*!< Bidirectional data mode enable */

/*******************  Bit definition for SPI_CR2 register  *******************/
#define  SPI_CR2_RXDMAEN                     ((uint32_t)0x00000001U)            /*!< Rx Buffer DMA Enable */
#define  SPI_CR2_TXDMAEN                     ((uint32_t)0x00000002U)            /*!< Tx Buffer DMA Enable */
#define  SPI_CR2_SSOE                        ((uint32_t)0x00000004U)            /*!< SS Output Enable */
#define  SPI_CR2_NSSP                        ((uint32_t)0x00000008U)            /*!< NSS pulse management Enable */
#define  SPI_CR2_FRF                         ((uint32_t)0x00000010U)            /*!< Frame Format Enable */
#define  SPI_CR2_ERRIE                       ((uint32_t)0x00000020U)            /*!< Error Interrupt Enable */
#define  SPI_CR2_RXNEIE                      ((uint32_t)0x00000040U)            /*!< RX buffer Not Empty Interrupt Enable */
#define  SPI_CR2_TXEIE                       ((uint32_t)0x00000080U)            /*!< Tx buffer Empty Interrupt Enable */
#define  SPI_CR2_DS                          ((uint32_t)0x00000F00U)            /*!< DS[3:0] Data Size */
#define  SPI_CR2_DS_0                        ((uint32_t)0x00000100U)            /*!< Bit 0 */
#define  SPI_CR2_DS_1                        ((uint32_t)0x00000200U)            /*!< Bit 1 */
#define  SPI_CR2_DS_2                        ((uint32_t)0x00000400U)            /*!< Bit 2 */
#define  SPI_CR2_DS_3                        ((uint32_t)0x00000800U)            /*!< Bit 3 */
#define  SPI_CR2_FRXTH                       ((uint32_t)0x00001000U)            /*!< FIFO reception Threshold */
#define  SPI_CR2_LDMARX                      ((uint32_t)0x00002000U)            /*!< Last DMA transfer for reception */
#define  SPI_CR2_LDMATX                      ((uint32_t)0x00004000U)            /*!< Last DMA transfer for transmission */

/********************  Bit definition for SPI_SR register  *******************/
#define  SPI_SR_RXNE                         ((uint32_t)0x00000001U)            /*!< Receive buffer Not Empty */
#define  SPI_SR_TXE                          ((uint32_t)0x00000002U)            /*!< Transmit buffer Empty */
#define  SPI_SR_CHSIDE                       ((uint32_t)0x00000004U)            /*!< Channel side */
#define  SPI_SR_UDR                          ((uint32_t)0x00000008U)            /*!< Underrun flag */
#define  SPI_SR_CRCERR                       ((uint32_t)0x00000010U)            /*!< CRC Error flag */
#define  SPI_SR_MODF                         ((uint32_t)0x00000020U)            /*!< Mode fault */
#define  SPI_SR_OVR                          ((uint32_t)0x00000040U)            /*!< Overrun flag */
#define  SPI_SR_BSY                          ((uint32_t)0x00000080U)            /*!< Busy flag */
#define  SPI_SR_FRE                          ((uint32_t)0x00000100U)            /*!< TI frame format error */
#define  SPI_SR_FRLVL                        ((uint32_t)0x00000600U)            /*!< FIFO Reception Level */
#define  SPI_SR_FRLVL_0                      ((uint32_t)0x00000200U)            /*!< Bit 0 */
#define  SPI_SR_FRLVL_1                      ((uint32_t)0x00000400U)            /*!< Bit 1 */
#define  SPI_SR_FTLVL                        ((uint32_t)0x00001800U)            /*!< FIFO Transmission Level */
#define  SPI_SR_FTLVL_0                      ((uint32_t)0x00000800U)            /*!< Bit 0 */
#define  SPI_SR_FTLVL_1                      ((uint32_t)0x00001000U)            /*!< Bit 1 */  

/********************  Bit definition for SPI_DR register  *******************/
#define  SPI_DR_DR                           ((uint32_t)0xFFFFFFFFU)            /*!< Data Register */

/*******************  Bit definition for SPI_CRCPR register  *****************/
#define  SPI_CRCPR_CRCPOLY                   ((uint32_t)0xFFFFFFFFU)            /*!< CRC polynomial register */

/******************  Bit definition for SPI_RXCRCR register  *****************/
#define  SPI_RXCRCR_RXCRC                    ((uint32_t)0xFFFFFFFFU)            /*!< Rx CRC Register */

/******************  Bit definition for SPI_TXCRCR register  *****************/
#define  SPI_TXCRCR_TXCRC                    ((uint32_t)0xFFFFFFFFU)            /*!< Tx CRC Register */

/******************  Bit definition for SPI_I2SCFGR register  ****************/
#define  SPI_I2SCFGR_CHLEN                   ((uint32_t)0x00000001U)            /*!<Channel length (number of bits per audio channel) */
#define  SPI_I2SCFGR_DATLEN                  ((uint32_t)0x00000006U)            /*!<DATLEN[1:0] bits (Data length to be transferred) */
#define  SPI_I2SCFGR_DATLEN_0                ((uint32_t)0x00000002U)            /*!<Bit 0 */
#define  SPI_I2SCFGR_DATLEN_1                ((uint32_t)0x00000004U)            /*!<Bit 1 */
#define  SPI_I2SCFGR_CKPOL                   ((uint32_t)0x00000008U)            /*!<steady state clock polarity */
#define  SPI_I2SCFGR_I2SSTD                  ((uint32_t)0x00000030U)            /*!<I2SSTD[1:0] bits (I2S standard selection) */
#define  SPI_I2SCFGR_I2SSTD_0                ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define  SPI_I2SCFGR_I2SSTD_1                ((uint32_t)0x00000020U)            /*!<Bit 1 */
#define  SPI_I2SCFGR_PCMSYNC                 ((uint32_t)0x00000080U)            /*!<PCM frame synchronization */
#define  SPI_I2SCFGR_I2SCFG                  ((uint32_t)0x00000300U)            /*!<I2SCFG[1:0] bits (I2S configuration mode) */
#define  SPI_I2SCFGR_I2SCFG_0                ((uint32_t)0x00000100U)            /*!<Bit 0 */
#define  SPI_I2SCFGR_I2SCFG_1                ((uint32_t)0x00000200U)            /*!<Bit 1 */
#define  SPI_I2SCFGR_I2SE                    ((uint32_t)0x00000400U)            /*!<I2S Enable */
#define  SPI_I2SCFGR_I2SMOD                  ((uint32_t)0x00000800U)            /*!<I2S mode selection */

/******************  Bit definition for SPI_I2SPR register  ******************/
#define  SPI_I2SPR_I2SDIV                    ((uint32_t)0x000000FFU)            /*!<I2S Linear prescaler */
#define  SPI_I2SPR_ODD                       ((uint32_t)0x00000100U)            /*!<Odd factor for the prescaler */
#define  SPI_I2SPR_MCKOE                     ((uint32_t)0x00000200U)            /*!<Master Clock Output Enable */

/*****************************************************************************/
/*                                                                           */
/*                       System Configuration (SYSCFG)                       */
/*                                                                           */
/*****************************************************************************/
/*****************  Bit definition for SYSCFG_CFGR1 register  ****************/
#define SYSCFG_CFGR1_MEM_MODE               ((uint32_t)0x00000003U) /*!< SYSCFG_Memory Remap Config */
#define SYSCFG_CFGR1_MEM_MODE_0             ((uint32_t)0x00000001U) /*!< SYSCFG_Memory Remap Config Bit 0 */
#define SYSCFG_CFGR1_MEM_MODE_1             ((uint32_t)0x00000002U) /*!< SYSCFG_Memory Remap Config Bit 1 */


#define SYSCFG_CFGR1_DMA_RMP                ((uint32_t)0x00001F00U) /*!< DMA remap mask */
#define SYSCFG_CFGR1_ADC_DMA_RMP            ((uint32_t)0x00000100U) /*!< ADC DMA remap */
#define SYSCFG_CFGR1_USART1TX_DMA_RMP       ((uint32_t)0x00000200U) /*!< USART1 TX DMA remap */
#define SYSCFG_CFGR1_USART1RX_DMA_RMP       ((uint32_t)0x00000400U) /*!< USART1 RX DMA remap */
#define SYSCFG_CFGR1_TIM16_DMA_RMP          ((uint32_t)0x00000800U) /*!< Timer 16 DMA remap */
#define SYSCFG_CFGR1_TIM17_DMA_RMP          ((uint32_t)0x00001000U) /*!< Timer 17 DMA remap */

#define SYSCFG_CFGR1_I2C_FMP_PB6            ((uint32_t)0x00010000U) /*!< I2C PB6 Fast mode plus */
#define SYSCFG_CFGR1_I2C_FMP_PB7            ((uint32_t)0x00020000U) /*!< I2C PB7 Fast mode plus */
#define SYSCFG_CFGR1_I2C_FMP_PB8            ((uint32_t)0x00040000U) /*!< I2C PB8 Fast mode plus */
#define SYSCFG_CFGR1_I2C_FMP_PB9            ((uint32_t)0x00080000U) /*!< I2C PB9 Fast mode plus */

/*****************  Bit definition for SYSCFG_EXTICR1 register  **************/
#define SYSCFG_EXTICR1_EXTI0            ((uint32_t)0x0000000FU) /*!< EXTI 0 configuration */
#define SYSCFG_EXTICR1_EXTI1            ((uint32_t)0x000000F0U) /*!< EXTI 1 configuration */
#define SYSCFG_EXTICR1_EXTI2            ((uint32_t)0x00000F00U) /*!< EXTI 2 configuration */
#define SYSCFG_EXTICR1_EXTI3            ((uint32_t)0x0000F000U) /*!< EXTI 3 configuration */

/** 
  * @brief  EXTI0 configuration
  */
#define SYSCFG_EXTICR1_EXTI0_PA         ((uint32_t)0x00000000U) /*!< PA[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PB         ((uint32_t)0x00000001U) /*!< PB[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PC         ((uint32_t)0x00000002U) /*!< PC[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PD         ((uint32_t)0x00000003U) /*!< PD[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PF         ((uint32_t)0x00000005U) /*!< PF[0] pin */

/** 
  * @brief  EXTI1 configuration  
  */ 
#define SYSCFG_EXTICR1_EXTI1_PA         ((uint32_t)0x00000000U) /*!< PA[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PB         ((uint32_t)0x00000010U) /*!< PB[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PC         ((uint32_t)0x00000020U) /*!< PC[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PD         ((uint32_t)0x00000030U) /*!< PD[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PF         ((uint32_t)0x00000050U) /*!< PF[1] pin */

/** 
  * @brief  EXTI2 configuration  
  */
#define SYSCFG_EXTICR1_EXTI2_PA         ((uint32_t)0x00000000U) /*!< PA[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PB         ((uint32_t)0x00000100U) /*!< PB[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PC         ((uint32_t)0x00000200U) /*!< PC[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PD         ((uint32_t)0x00000300U) /*!< PD[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PF         ((uint32_t)0x00000500U) /*!< PF[2] pin */

/** 
  * @brief  EXTI3 configuration  
  */
#define SYSCFG_EXTICR1_EXTI3_PA         ((uint32_t)0x00000000U) /*!< PA[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PB         ((uint32_t)0x00001000U) /*!< PB[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PC         ((uint32_t)0x00002000U) /*!< PC[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PD         ((uint32_t)0x00003000U) /*!< PD[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PF         ((uint32_t)0x00005000U) /*!< PF[3] pin */

/*****************  Bit definition for SYSCFG_EXTICR2 register  **************/
#define SYSCFG_EXTICR2_EXTI4            ((uint32_t)0x0000000FU) /*!< EXTI 4 configuration */
#define SYSCFG_EXTICR2_EXTI5            ((uint32_t)0x000000F0U) /*!< EXTI 5 configuration */
#define SYSCFG_EXTICR2_EXTI6            ((uint32_t)0x00000F00U) /*!< EXTI 6 configuration */
#define SYSCFG_EXTICR2_EXTI7            ((uint32_t)0x0000F000U) /*!< EXTI 7 configuration */

/** 
  * @brief  EXTI4 configuration  
  */
#define SYSCFG_EXTICR2_EXTI4_PA         ((uint32_t)0x00000000U) /*!< PA[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PB         ((uint32_t)0x00000001U) /*!< PB[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PC         ((uint32_t)0x00000002U) /*!< PC[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PD         ((uint32_t)0x00000003U) /*!< PD[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PF         ((uint32_t)0x00000005U) /*!< PF[4] pin */

/** 
  * @brief  EXTI5 configuration  
  */
#define SYSCFG_EXTICR2_EXTI5_PA         ((uint32_t)0x00000000U) /*!< PA[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PB         ((uint32_t)0x00000010U) /*!< PB[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PC         ((uint32_t)0x00000020U) /*!< PC[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PD         ((uint32_t)0x00000030U) /*!< PD[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PF         ((uint32_t)0x00000050U) /*!< PF[5] pin */

/** 
  * @brief  EXTI6 configuration  
  */
#define SYSCFG_EXTICR2_EXTI6_PA         ((uint32_t)0x00000000U) /*!< PA[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PB         ((uint32_t)0x00000100U) /*!< PB[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PC         ((uint32_t)0x00000200U) /*!< PC[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PD         ((uint32_t)0x00000300U) /*!< PD[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PF         ((uint32_t)0x00000500U) /*!< PF[6] pin */

/** 
  * @brief  EXTI7 configuration  
  */
#define SYSCFG_EXTICR2_EXTI7_PA         ((uint32_t)0x00000000U) /*!< PA[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PB         ((uint32_t)0x00001000U) /*!< PB[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PC         ((uint32_t)0x00002000U) /*!< PC[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PD         ((uint32_t)0x00003000U) /*!< PD[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PF         ((uint32_t)0x00005000U) /*!< PF[7] pin */

/*****************  Bit definition for SYSCFG_EXTICR3 register  **************/
#define SYSCFG_EXTICR3_EXTI8            ((uint32_t)0x0000000FU) /*!< EXTI 8 configuration */
#define SYSCFG_EXTICR3_EXTI9            ((uint32_t)0x000000F0U) /*!< EXTI 9 configuration */
#define SYSCFG_EXTICR3_EXTI10           ((uint32_t)0x00000F00U) /*!< EXTI 10 configuration */
#define SYSCFG_EXTICR3_EXTI11           ((uint32_t)0x0000F000U) /*!< EXTI 11 configuration */

/** 
  * @brief  EXTI8 configuration  
  */
#define SYSCFG_EXTICR3_EXTI8_PA         ((uint32_t)0x00000000U) /*!< PA[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PB         ((uint32_t)0x00000001U) /*!< PB[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PC         ((uint32_t)0x00000002U) /*!< PC[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PD         ((uint32_t)0x00000003U) /*!< PD[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PF         ((uint32_t)0x00000005U) /*!< PF[8] pin */


/** 
  * @brief  EXTI9 configuration  
  */
#define SYSCFG_EXTICR3_EXTI9_PA         ((uint32_t)0x00000000U) /*!< PA[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PB         ((uint32_t)0x00000010U) /*!< PB[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PC         ((uint32_t)0x00000020U) /*!< PC[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PD         ((uint32_t)0x00000030U) /*!< PD[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PF         ((uint32_t)0x00000050U) /*!< PF[9] pin */

/** 
  * @brief  EXTI10 configuration  
  */
#define SYSCFG_EXTICR3_EXTI10_PA        ((uint32_t)0x00000000U) /*!< PA[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PB        ((uint32_t)0x00000100U) /*!< PB[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PC        ((uint32_t)0x00000200U) /*!< PC[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PD        ((uint32_t)0x00000300U) /*!< PD[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PF        ((uint32_t)0x00000500U) /*!< PF[10] pin */

/** 
  * @brief  EXTI11 configuration  
  */
#define SYSCFG_EXTICR3_EXTI11_PA        ((uint32_t)0x00000000U) /*!< PA[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PB        ((uint32_t)0x00001000U) /*!< PB[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PC        ((uint32_t)0x00002000U) /*!< PC[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PD        ((uint32_t)0x00003000U) /*!< PD[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PF        ((uint32_t)0x00005000U) /*!< PF[11] pin */

/*****************  Bit definition for SYSCFG_EXTICR4 register  **************/
#define SYSCFG_EXTICR4_EXTI12           ((uint32_t)0x0000000FU) /*!< EXTI 12 configuration */
#define SYSCFG_EXTICR4_EXTI13           ((uint32_t)0x000000F0U) /*!< EXTI 13 configuration */
#define SYSCFG_EXTICR4_EXTI14           ((uint32_t)0x00000F00U) /*!< EXTI 14 configuration */
#define SYSCFG_EXTICR4_EXTI15           ((uint32_t)0x0000F000U) /*!< EXTI 15 configuration */

/** 
  * @brief  EXTI12 configuration  
  */
#define SYSCFG_EXTICR4_EXTI12_PA        ((uint32_t)0x00000000U) /*!< PA[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PB        ((uint32_t)0x00000001U) /*!< PB[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PC        ((uint32_t)0x00000002U) /*!< PC[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PD        ((uint32_t)0x00000003U) /*!< PD[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PF        ((uint32_t)0x00000005U) /*!< PF[12] pin */

/** 
  * @brief  EXTI13 configuration  
  */
#define SYSCFG_EXTICR4_EXTI13_PA        ((uint32_t)0x00000000U) /*!< PA[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PB        ((uint32_t)0x00000010U) /*!< PB[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PC        ((uint32_t)0x00000020U) /*!< PC[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PD        ((uint32_t)0x00000030U) /*!< PD[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PF        ((uint32_t)0x00000050U) /*!< PF[13] pin */

/** 
  * @brief  EXTI14 configuration  
  */
#define SYSCFG_EXTICR4_EXTI14_PA        ((uint32_t)0x00000000U) /*!< PA[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PB        ((uint32_t)0x00000100U) /*!< PB[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PC        ((uint32_t)0x00000200U) /*!< PC[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PD        ((uint32_t)0x00000300U) /*!< PD[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PF        ((uint32_t)0x00000500U) /*!< PF[14] pin */

/** 
  * @brief  EXTI15 configuration  
  */
#define SYSCFG_EXTICR4_EXTI15_PA        ((uint32_t)0x00000000U) /*!< PA[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PB        ((uint32_t)0x00001000U) /*!< PB[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PC        ((uint32_t)0x00002000U) /*!< PC[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PD        ((uint32_t)0x00003000U) /*!< PD[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PF        ((uint32_t)0x00005000U) /*!< PF[15] pin */

/*****************  Bit definition for SYSCFG_CFGR2 register  ****************/
#define SYSCFG_CFGR2_LOCKUP_LOCK               ((uint32_t)0x00000001U) /*!< Enables and locks the LOCKUP (Hardfault) output of CortexM0 with Break Input of TIMER1 */
#define SYSCFG_CFGR2_SRAM_PARITY_LOCK          ((uint32_t)0x00000002U) /*!< Enables and locks the SRAM_PARITY error signal with Break Input of TIMER1 */
#define SYSCFG_CFGR2_PVD_LOCK                  ((uint32_t)0x00000004U) /*!< Enables and locks the PVD connection with Timer1 Break Input and also the PVD_EN and PVDSEL[2:0] bits of the Power Control Interface */
#define SYSCFG_CFGR2_SRAM_PEF                  ((uint32_t)0x00000100U) /*!< SRAM Parity error flag */
#define SYSCFG_CFGR2_SRAM_PE                   SYSCFG_CFGR2_SRAM_PEF  /*!< SRAM Parity error flag (define maintained for legacy purpose) */

/*****************************************************************************/
/*                                                                           */
/*                               Timers (TIM)                                */
/*                                                                           */
/*****************************************************************************/
/*******************  Bit definition for TIM_CR1 register  *******************/
#define  TIM_CR1_CEN                         ((uint32_t)0x00000001U)            /*!<Counter enable */
#define  TIM_CR1_UDIS                        ((uint32_t)0x00000002U)            /*!<Update disable */
#define  TIM_CR1_URS                         ((uint32_t)0x00000004U)            /*!<Update request source */
#define  TIM_CR1_OPM                         ((uint32_t)0x00000008U)            /*!<One pulse mode */
#define  TIM_CR1_DIR                         ((uint32_t)0x00000010U)            /*!<Direction */

#define  TIM_CR1_CMS                         ((uint32_t)0x00000060U)            /*!<CMS[1:0] bits (Center-aligned mode selection) */
#define  TIM_CR1_CMS_0                       ((uint32_t)0x00000020U)            /*!<Bit 0 */
#define  TIM_CR1_CMS_1                       ((uint32_t)0x00000040U)            /*!<Bit 1 */

#define  TIM_CR1_ARPE                        ((uint32_t)0x00000080U)            /*!<Auto-reload preload enable */

#define  TIM_CR1_CKD                         ((uint32_t)0x00000300U)            /*!<CKD[1:0] bits (clock division) */
#define  TIM_CR1_CKD_0                       ((uint32_t)0x00000100U)            /*!<Bit 0 */
#define  TIM_CR1_CKD_1                       ((uint32_t)0x00000200U)            /*!<Bit 1 */

/*******************  Bit definition for TIM_CR2 register  *******************/
#define  TIM_CR2_CCPC                        ((uint32_t)0x00000001U)            /*!<Capture/Compare Preloaded Control */
#define  TIM_CR2_CCUS                        ((uint32_t)0x00000004U)            /*!<Capture/Compare Control Update Selection */
#define  TIM_CR2_CCDS                        ((uint32_t)0x00000008U)            /*!<Capture/Compare DMA Selection */

#define  TIM_CR2_MMS                         ((uint32_t)0x00000070U)            /*!<MMS[2:0] bits (Master Mode Selection) */
#define  TIM_CR2_MMS_0                       ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define  TIM_CR2_MMS_1                       ((uint32_t)0x00000020U)            /*!<Bit 1 */
#define  TIM_CR2_MMS_2                       ((uint32_t)0x00000040U)            /*!<Bit 2 */

#define  TIM_CR2_TI1S                        ((uint32_t)0x00000080U)            /*!<TI1 Selection */
#define  TIM_CR2_OIS1                        ((uint32_t)0x00000100U)            /*!<Output Idle state 1 (OC1 output) */
#define  TIM_CR2_OIS1N                       ((uint32_t)0x00000200U)            /*!<Output Idle state 1 (OC1N output) */
#define  TIM_CR2_OIS2                        ((uint32_t)0x00000400U)            /*!<Output Idle state 2 (OC2 output) */
#define  TIM_CR2_OIS2N                       ((uint32_t)0x00000800U)            /*!<Output Idle state 2 (OC2N output) */
#define  TIM_CR2_OIS3                        ((uint32_t)0x00001000U)            /*!<Output Idle state 3 (OC3 output) */
#define  TIM_CR2_OIS3N                       ((uint32_t)0x00002000U)            /*!<Output Idle state 3 (OC3N output) */
#define  TIM_CR2_OIS4                        ((uint32_t)0x00004000U)            /*!<Output Idle state 4 (OC4 output) */

/*******************  Bit definition for TIM_SMCR register  ******************/
#define  TIM_SMCR_SMS                        ((uint32_t)0x00000007U)            /*!<SMS[2:0] bits (Slave mode selection) */
#define  TIM_SMCR_SMS_0                      ((uint32_t)0x00000001U)            /*!<Bit 0 */
#define  TIM_SMCR_SMS_1                      ((uint32_t)0x00000002U)            /*!<Bit 1 */
#define  TIM_SMCR_SMS_2                      ((uint32_t)0x00000004U)            /*!<Bit 2 */

#define  TIM_SMCR_OCCS                       ((uint32_t)0x00000008U)            /*!< OCREF clear selection */

#define  TIM_SMCR_TS                         ((uint32_t)0x00000070U)            /*!<TS[2:0] bits (Trigger selection) */
#define  TIM_SMCR_TS_0                       ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define  TIM_SMCR_TS_1                       ((uint32_t)0x00000020U)            /*!<Bit 1 */
#define  TIM_SMCR_TS_2                       ((uint32_t)0x00000040U)            /*!<Bit 2 */

#define  TIM_SMCR_MSM                        ((uint32_t)0x00000080U)            /*!<Master/slave mode */

#define  TIM_SMCR_ETF                        ((uint32_t)0x00000F00U)            /*!<ETF[3:0] bits (External trigger filter) */
#define  TIM_SMCR_ETF_0                      ((uint32_t)0x00000100U)            /*!<Bit 0 */
#define  TIM_SMCR_ETF_1                      ((uint32_t)0x00000200U)            /*!<Bit 1 */
#define  TIM_SMCR_ETF_2                      ((uint32_t)0x00000400U)            /*!<Bit 2 */
#define  TIM_SMCR_ETF_3                      ((uint32_t)0x00000800U)            /*!<Bit 3 */

#define  TIM_SMCR_ETPS                       ((uint32_t)0x00003000U)            /*!<ETPS[1:0] bits (External trigger prescaler) */
#define  TIM_SMCR_ETPS_0                     ((uint32_t)0x00001000U)            /*!<Bit 0 */
#define  TIM_SMCR_ETPS_1                     ((uint32_t)0x00002000U)            /*!<Bit 1 */

#define  TIM_SMCR_ECE                        ((uint32_t)0x00004000U)            /*!<External clock enable */
#define  TIM_SMCR_ETP                        ((uint32_t)0x00008000U)            /*!<External trigger polarity */

/*******************  Bit definition for TIM_DIER register  ******************/
#define  TIM_DIER_UIE                        ((uint32_t)0x00000001U)            /*!<Update interrupt enable */
#define  TIM_DIER_CC1IE                      ((uint32_t)0x00000002U)            /*!<Capture/Compare 1 interrupt enable */
#define  TIM_DIER_CC2IE                      ((uint32_t)0x00000004U)            /*!<Capture/Compare 2 interrupt enable */
#define  TIM_DIER_CC3IE                      ((uint32_t)0x00000008U)            /*!<Capture/Compare 3 interrupt enable */
#define  TIM_DIER_CC4IE                      ((uint32_t)0x00000010U)            /*!<Capture/Compare 4 interrupt enable */
#define  TIM_DIER_COMIE                      ((uint32_t)0x00000020U)            /*!<COM interrupt enable */
#define  TIM_DIER_TIE                        ((uint32_t)0x00000040U)            /*!<Trigger interrupt enable */
#define  TIM_DIER_BIE                        ((uint32_t)0x00000080U)            /*!<Break interrupt enable */
#define  TIM_DIER_UDE                        ((uint32_t)0x00000100U)            /*!<Update DMA request enable */
#define  TIM_DIER_CC1DE                      ((uint32_t)0x00000200U)            /*!<Capture/Compare 1 DMA request enable */
#define  TIM_DIER_CC2DE                      ((uint32_t)0x00000400U)            /*!<Capture/Compare 2 DMA request enable */
#define  TIM_DIER_CC3DE                      ((uint32_t)0x00000800U)            /*!<Capture/Compare 3 DMA request enable */
#define  TIM_DIER_CC4DE                      ((uint32_t)0x00001000U)            /*!<Capture/Compare 4 DMA request enable */
#define  TIM_DIER_COMDE                      ((uint32_t)0x00002000U)            /*!<COM DMA request enable */
#define  TIM_DIER_TDE                        ((uint32_t)0x00004000U)            /*!<Trigger DMA request enable */

/********************  Bit definition for TIM_SR register  *******************/
#define  TIM_SR_UIF                          ((uint32_t)0x00000001U)            /*!<Update interrupt Flag */
#define  TIM_SR_CC1IF                        ((uint32_t)0x00000002U)            /*!<Capture/Compare 1 interrupt Flag */
#define  TIM_SR_CC2IF                        ((uint32_t)0x00000004U)            /*!<Capture/Compare 2 interrupt Flag */
#define  TIM_SR_CC3IF                        ((uint32_t)0x00000008U)            /*!<Capture/Compare 3 interrupt Flag */
#define  TIM_SR_CC4IF                        ((uint32_t)0x00000010U)            /*!<Capture/Compare 4 interrupt Flag */
#define  TIM_SR_COMIF                        ((uint32_t)0x00000020U)            /*!<COM interrupt Flag */
#define  TIM_SR_TIF                          ((uint32_t)0x00000040U)            /*!<Trigger interrupt Flag */
#define  TIM_SR_BIF                          ((uint32_t)0x00000080U)            /*!<Break interrupt Flag */
#define  TIM_SR_CC1OF                        ((uint32_t)0x00000200U)            /*!<Capture/Compare 1 Overcapture Flag */
#define  TIM_SR_CC2OF                        ((uint32_t)0x00000400U)            /*!<Capture/Compare 2 Overcapture Flag */
#define  TIM_SR_CC3OF                        ((uint32_t)0x00000800U)            /*!<Capture/Compare 3 Overcapture Flag */
#define  TIM_SR_CC4OF                        ((uint32_t)0x00001000U)            /*!<Capture/Compare 4 Overcapture Flag */

/*******************  Bit definition for TIM_EGR register  *******************/
#define  TIM_EGR_UG                          ((uint32_t)0x00000001U)               /*!<Update Generation */
#define  TIM_EGR_CC1G                        ((uint32_t)0x00000002U)               /*!<Capture/Compare 1 Generation */
#define  TIM_EGR_CC2G                        ((uint32_t)0x00000004U)               /*!<Capture/Compare 2 Generation */
#define  TIM_EGR_CC3G                        ((uint32_t)0x00000008U)               /*!<Capture/Compare 3 Generation */
#define  TIM_EGR_CC4G                        ((uint32_t)0x00000010U)               /*!<Capture/Compare 4 Generation */
#define  TIM_EGR_COMG                        ((uint32_t)0x00000020U)               /*!<Capture/Compare Control Update Generation */
#define  TIM_EGR_TG                          ((uint32_t)0x00000040U)               /*!<Trigger Generation */
#define  TIM_EGR_BG                          ((uint32_t)0x00000080U)               /*!<Break Generation */

/******************  Bit definition for TIM_CCMR1 register  ******************/
#define  TIM_CCMR1_CC1S                      ((uint32_t)0x00000003U)            /*!<CC1S[1:0] bits (Capture/Compare 1 Selection) */
#define  TIM_CCMR1_CC1S_0                    ((uint32_t)0x00000001U)            /*!<Bit 0 */
#define  TIM_CCMR1_CC1S_1                    ((uint32_t)0x00000002U)            /*!<Bit 1 */

#define  TIM_CCMR1_OC1FE                     ((uint32_t)0x00000004U)            /*!<Output Compare 1 Fast enable */
#define  TIM_CCMR1_OC1PE                     ((uint32_t)0x00000008U)            /*!<Output Compare 1 Preload enable */

#define  TIM_CCMR1_OC1M                      ((uint32_t)0x00000070U)            /*!<OC1M[2:0] bits (Output Compare 1 Mode) */
#define  TIM_CCMR1_OC1M_0                    ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define  TIM_CCMR1_OC1M_1                    ((uint32_t)0x00000020U)            /*!<Bit 1 */
#define  TIM_CCMR1_OC1M_2                    ((uint32_t)0x00000040U)            /*!<Bit 2 */

#define  TIM_CCMR1_OC1CE                     ((uint32_t)0x00000080U)            /*!<Output Compare 1Clear Enable */

#define  TIM_CCMR1_CC2S                      ((uint32_t)0x00000300U)            /*!<CC2S[1:0] bits (Capture/Compare 2 Selection) */
#define  TIM_CCMR1_CC2S_0                    ((uint32_t)0x00000100U)            /*!<Bit 0 */
#define  TIM_CCMR1_CC2S_1                    ((uint32_t)0x00000200U)            /*!<Bit 1 */

#define  TIM_CCMR1_OC2FE                     ((uint32_t)0x00000400U)            /*!<Output Compare 2 Fast enable */
#define  TIM_CCMR1_OC2PE                     ((uint32_t)0x00000800U)            /*!<Output Compare 2 Preload enable */

#define  TIM_CCMR1_OC2M                      ((uint32_t)0x00007000U)            /*!<OC2M[2:0] bits (Output Compare 2 Mode) */
#define  TIM_CCMR1_OC2M_0                    ((uint32_t)0x00001000U)            /*!<Bit 0 */
#define  TIM_CCMR1_OC2M_1                    ((uint32_t)0x00002000U)            /*!<Bit 1 */
#define  TIM_CCMR1_OC2M_2                    ((uint32_t)0x00004000U)            /*!<Bit 2 */

#define  TIM_CCMR1_OC2CE                     ((uint32_t)0x00008000U)            /*!<Output Compare 2 Clear Enable */

/*---------------------------------------------------------------------------*/

#define  TIM_CCMR1_IC1PSC                    ((uint32_t)0x0000000CU)            /*!<IC1PSC[1:0] bits (Input Capture 1 Prescaler) */
#define  TIM_CCMR1_IC1PSC_0                  ((uint32_t)0x00000004U)            /*!<Bit 0 */
#define  TIM_CCMR1_IC1PSC_1                  ((uint32_t)0x00000008U)            /*!<Bit 1 */

#define  TIM_CCMR1_IC1F                      ((uint32_t)0x000000F0U)            /*!<IC1F[3:0] bits (Input Capture 1 Filter) */
#define  TIM_CCMR1_IC1F_0                    ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define  TIM_CCMR1_IC1F_1                    ((uint32_t)0x00000020U)            /*!<Bit 1 */
#define  TIM_CCMR1_IC1F_2                    ((uint32_t)0x00000040U)            /*!<Bit 2 */
#define  TIM_CCMR1_IC1F_3                    ((uint32_t)0x00000080U)            /*!<Bit 3 */

#define  TIM_CCMR1_IC2PSC                    ((uint32_t)0x00000C00U)            /*!<IC2PSC[1:0] bits (Input Capture 2 Prescaler) */
#define  TIM_CCMR1_IC2PSC_0                  ((uint32_t)0x00000400U)            /*!<Bit 0 */
#define  TIM_CCMR1_IC2PSC_1                  ((uint32_t)0x00000800U)            /*!<Bit 1 */

#define  TIM_CCMR1_IC2F                      ((uint32_t)0x0000F000U)            /*!<IC2F[3:0] bits (Input Capture 2 Filter) */
#define  TIM_CCMR1_IC2F_0                    ((uint32_t)0x00001000U)            /*!<Bit 0 */
#define  TIM_CCMR1_IC2F_1                    ((uint32_t)0x00002000U)            /*!<Bit 1 */
#define  TIM_CCMR1_IC2F_2                    ((uint32_t)0x00004000U)            /*!<Bit 2 */
#define  TIM_CCMR1_IC2F_3                    ((uint32_t)0x00008000U)            /*!<Bit 3 */

/******************  Bit definition for TIM_CCMR2 register  ******************/
#define  TIM_CCMR2_CC3S                      ((uint32_t)0x00000003U)            /*!<CC3S[1:0] bits (Capture/Compare 3 Selection) */
#define  TIM_CCMR2_CC3S_0                    ((uint32_t)0x00000001U)            /*!<Bit 0 */
#define  TIM_CCMR2_CC3S_1                    ((uint32_t)0x00000002U)            /*!<Bit 1 */

#define  TIM_CCMR2_OC3FE                     ((uint32_t)0x00000004U)            /*!<Output Compare 3 Fast enable */
#define  TIM_CCMR2_OC3PE                     ((uint32_t)0x00000008U)            /*!<Output Compare 3 Preload enable */

#define  TIM_CCMR2_OC3M                      ((uint32_t)0x00000070U)            /*!<OC3M[2:0] bits (Output Compare 3 Mode) */
#define  TIM_CCMR2_OC3M_0                    ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define  TIM_CCMR2_OC3M_1                    ((uint32_t)0x00000020U)            /*!<Bit 1 */
#define  TIM_CCMR2_OC3M_2                    ((uint32_t)0x00000040U)            /*!<Bit 2 */

#define  TIM_CCMR2_OC3CE                     ((uint32_t)0x00000080U)            /*!<Output Compare 3 Clear Enable */

#define  TIM_CCMR2_CC4S                      ((uint32_t)0x00000300U)            /*!<CC4S[1:0] bits (Capture/Compare 4 Selection) */
#define  TIM_CCMR2_CC4S_0                    ((uint32_t)0x00000100U)            /*!<Bit 0 */
#define  TIM_CCMR2_CC4S_1                    ((uint32_t)0x00000200U)            /*!<Bit 1 */

#define  TIM_CCMR2_OC4FE                     ((uint32_t)0x00000400U)            /*!<Output Compare 4 Fast enable */
#define  TIM_CCMR2_OC4PE                     ((uint32_t)0x00000800U)            /*!<Output Compare 4 Preload enable */

#define  TIM_CCMR2_OC4M                      ((uint32_t)0x00007000U)            /*!<OC4M[2:0] bits (Output Compare 4 Mode) */
#define  TIM_CCMR2_OC4M_0                    ((uint32_t)0x00001000U)            /*!<Bit 0 */
#define  TIM_CCMR2_OC4M_1                    ((uint32_t)0x00002000U)            /*!<Bit 1 */
#define  TIM_CCMR2_OC4M_2                    ((uint32_t)0x00004000U)            /*!<Bit 2 */

#define  TIM_CCMR2_OC4CE                     ((uint32_t)0x00008000U)            /*!<Output Compare 4 Clear Enable */

/*---------------------------------------------------------------------------*/

#define  TIM_CCMR2_IC3PSC                    ((uint32_t)0x0000000CU)            /*!<IC3PSC[1:0] bits (Input Capture 3 Prescaler) */
#define  TIM_CCMR2_IC3PSC_0                  ((uint32_t)0x00000004U)            /*!<Bit 0 */
#define  TIM_CCMR2_IC3PSC_1                  ((uint32_t)0x00000008U)            /*!<Bit 1 */

#define  TIM_CCMR2_IC3F                      ((uint32_t)0x000000F0U)            /*!<IC3F[3:0] bits (Input Capture 3 Filter) */
#define  TIM_CCMR2_IC3F_0                    ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define  TIM_CCMR2_IC3F_1                    ((uint32_t)0x00000020U)            /*!<Bit 1 */
#define  TIM_CCMR2_IC3F_2                    ((uint32_t)0x00000040U)            /*!<Bit 2 */
#define  TIM_CCMR2_IC3F_3                    ((uint32_t)0x00000080U)            /*!<Bit 3 */

#define  TIM_CCMR2_IC4PSC                    ((uint32_t)0x00000C00U)            /*!<IC4PSC[1:0] bits (Input Capture 4 Prescaler) */
#define  TIM_CCMR2_IC4PSC_0                  ((uint32_t)0x00000400U)            /*!<Bit 0 */
#define  TIM_CCMR2_IC4PSC_1                  ((uint32_t)0x00000800U)            /*!<Bit 1 */

#define  TIM_CCMR2_IC4F                      ((uint32_t)0x0000F000U)            /*!<IC4F[3:0] bits (Input Capture 4 Filter) */
#define  TIM_CCMR2_IC4F_0                    ((uint32_t)0x00001000U)            /*!<Bit 0 */
#define  TIM_CCMR2_IC4F_1                    ((uint32_t)0x00002000U)            /*!<Bit 1 */
#define  TIM_CCMR2_IC4F_2                    ((uint32_t)0x00004000U)            /*!<Bit 2 */
#define  TIM_CCMR2_IC4F_3                    ((uint32_t)0x00008000U)            /*!<Bit 3 */

/*******************  Bit definition for TIM_CCER register  ******************/
#define  TIM_CCER_CC1E                       ((uint32_t)0x00000001U)            /*!<Capture/Compare 1 output enable */
#define  TIM_CCER_CC1P                       ((uint32_t)0x00000002U)            /*!<Capture/Compare 1 output Polarity */
#define  TIM_CCER_CC1NE                      ((uint32_t)0x00000004U)            /*!<Capture/Compare 1 Complementary output enable */
#define  TIM_CCER_CC1NP                      ((uint32_t)0x00000008U)            /*!<Capture/Compare 1 Complementary output Polarity */
#define  TIM_CCER_CC2E                       ((uint32_t)0x00000010U)            /*!<Capture/Compare 2 output enable */
#define  TIM_CCER_CC2P                       ((uint32_t)0x00000020U)            /*!<Capture/Compare 2 output Polarity */
#define  TIM_CCER_CC2NE                      ((uint32_t)0x00000040U)            /*!<Capture/Compare 2 Complementary output enable */
#define  TIM_CCER_CC2NP                      ((uint32_t)0x00000080U)            /*!<Capture/Compare 2 Complementary output Polarity */
#define  TIM_CCER_CC3E                       ((uint32_t)0x00000100U)            /*!<Capture/Compare 3 output enable */
#define  TIM_CCER_CC3P                       ((uint32_t)0x00000200U)            /*!<Capture/Compare 3 output Polarity */
#define  TIM_CCER_CC3NE                      ((uint32_t)0x00000400U)            /*!<Capture/Compare 3 Complementary output enable */
#define  TIM_CCER_CC3NP                      ((uint32_t)0x00000800U)            /*!<Capture/Compare 3 Complementary output Polarity */
#define  TIM_CCER_CC4E                       ((uint32_t)0x00001000U)            /*!<Capture/Compare 4 output enable */
#define  TIM_CCER_CC4P                       ((uint32_t)0x00002000U)            /*!<Capture/Compare 4 output Polarity */
#define  TIM_CCER_CC4NP                      ((uint32_t)0x00008000U)            /*!<Capture/Compare 4 Complementary output Polarity */

/*******************  Bit definition for TIM_CNT register  *******************/
#define  TIM_CNT_CNT                         ((uint32_t)0xFFFFFFFFU)            /*!<Counter Value */

/*******************  Bit definition for TIM_PSC register  *******************/
#define  TIM_PSC_PSC                         ((uint32_t)0x0000FFFFU)            /*!<Prescaler Value */

/*******************  Bit definition for TIM_ARR register  *******************/
#define  TIM_ARR_ARR                         ((uint32_t)0xFFFFFFFFU)            /*!<actual auto-reload Value */

/*******************  Bit definition for TIM_RCR register  *******************/
#define  TIM_RCR_REP                         ((uint32_t)0x000000FFU)               /*!<Repetition Counter Value */

/*******************  Bit definition for TIM_CCR1 register  ******************/
#define  TIM_CCR1_CCR1                       ((uint32_t)0x0000FFFFU)            /*!<Capture/Compare 1 Value */

/*******************  Bit definition for TIM_CCR2 register  ******************/
#define  TIM_CCR2_CCR2                       ((uint32_t)0x0000FFFFU)            /*!<Capture/Compare 2 Value */

/*******************  Bit definition for TIM_CCR3 register  ******************/
#define  TIM_CCR3_CCR3                       ((uint32_t)0x0000FFFFU)            /*!<Capture/Compare 3 Value */

/*******************  Bit definition for TIM_CCR4 register  ******************/
#define  TIM_CCR4_CCR4                       ((uint32_t)0x0000FFFFU)            /*!<Capture/Compare 4 Value */

/*******************  Bit definition for TIM_BDTR register  ******************/
#define  TIM_BDTR_DTG                        ((uint32_t)0x000000FFU)            /*!<DTG[0:7] bits (Dead-Time Generator set-up) */
#define  TIM_BDTR_DTG_0                      ((uint32_t)0x00000001U)            /*!<Bit 0 */
#define  TIM_BDTR_DTG_1                      ((uint32_t)0x00000002U)            /*!<Bit 1 */
#define  TIM_BDTR_DTG_2                      ((uint32_t)0x00000004U)            /*!<Bit 2 */
#define  TIM_BDTR_DTG_3                      ((uint32_t)0x00000008U)            /*!<Bit 3 */
#define  TIM_BDTR_DTG_4                      ((uint32_t)0x00000010U)            /*!<Bit 4 */
#define  TIM_BDTR_DTG_5                      ((uint32_t)0x00000020U)            /*!<Bit 5 */
#define  TIM_BDTR_DTG_6                      ((uint32_t)0x00000040U)            /*!<Bit 6 */
#define  TIM_BDTR_DTG_7                      ((uint32_t)0x00000080U)            /*!<Bit 7 */

#define  TIM_BDTR_LOCK                       ((uint32_t)0x00000300U)            /*!<LOCK[1:0] bits (Lock Configuration) */
#define  TIM_BDTR_LOCK_0                     ((uint32_t)0x00000100U)            /*!<Bit 0 */
#define  TIM_BDTR_LOCK_1                     ((uint32_t)0x00000200U)            /*!<Bit 1 */

#define  TIM_BDTR_OSSI                       ((uint32_t)0x00000400U)            /*!<Off-State Selection for Idle mode */
#define  TIM_BDTR_OSSR                       ((uint32_t)0x00000800U)            /*!<Off-State Selection for Run mode */
#define  TIM_BDTR_BKE                        ((uint32_t)0x00001000U)            /*!<Break enable */
#define  TIM_BDTR_BKP                        ((uint32_t)0x00002000U)            /*!<Break Polarity */
#define  TIM_BDTR_AOE                        ((uint32_t)0x00004000U)            /*!<Automatic Output enable */
#define  TIM_BDTR_MOE                        ((uint32_t)0x00008000U)            /*!<Main Output enable */

/*******************  Bit definition for TIM_DCR register  *******************/
#define  TIM_DCR_DBA                         ((uint32_t)0x0000001FU)            /*!<DBA[4:0] bits (DMA Base Address) */
#define  TIM_DCR_DBA_0                       ((uint32_t)0x00000001U)            /*!<Bit 0 */
#define  TIM_DCR_DBA_1                       ((uint32_t)0x00000002U)            /*!<Bit 1 */
#define  TIM_DCR_DBA_2                       ((uint32_t)0x00000004U)            /*!<Bit 2 */
#define  TIM_DCR_DBA_3                       ((uint32_t)0x00000008U)            /*!<Bit 3 */
#define  TIM_DCR_DBA_4                       ((uint32_t)0x00000010U)            /*!<Bit 4 */

#define  TIM_DCR_DBL                         ((uint32_t)0x00001F00U)            /*!<DBL[4:0] bits (DMA Burst Length) */
#define  TIM_DCR_DBL_0                       ((uint32_t)0x00000100U)            /*!<Bit 0 */
#define  TIM_DCR_DBL_1                       ((uint32_t)0x00000200U)            /*!<Bit 1 */
#define  TIM_DCR_DBL_2                       ((uint32_t)0x00000400U)            /*!<Bit 2 */
#define  TIM_DCR_DBL_3                       ((uint32_t)0x00000800U)            /*!<Bit 3 */
#define  TIM_DCR_DBL_4                       ((uint32_t)0x00001000U)            /*!<Bit 4 */

/*******************  Bit definition for TIM_DMAR register  ******************/
#define  TIM_DMAR_DMAB                       ((uint32_t)0x0000FFFFU)            /*!<DMA register for burst accesses */

/*******************  Bit definition for TIM14_OR register  ********************/
#define TIM14_OR_TI1_RMP                     ((uint32_t)0x00000003U)            /*!<TI1_RMP[1:0] bits (TIM14 Input 4 remap) */
#define TIM14_OR_TI1_RMP_0                   ((uint32_t)0x00000001U)            /*!<Bit 0 */
#define TIM14_OR_TI1_RMP_1                   ((uint32_t)0x00000002U)            /*!<Bit 1 */

/******************************************************************************/
/*                                                                            */
/*                          Touch Sensing Controller (TSC)                    */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for TSC_CR register  *********************/
#define  TSC_CR_TSCE                         ((uint32_t)0x00000001U)            /*!<Touch sensing controller enable */
#define  TSC_CR_START                        ((uint32_t)0x00000002U)            /*!<Start acquisition */
#define  TSC_CR_AM                           ((uint32_t)0x00000004U)            /*!<Acquisition mode */
#define  TSC_CR_SYNCPOL                      ((uint32_t)0x00000008U)            /*!<Synchronization pin polarity */
#define  TSC_CR_IODEF                        ((uint32_t)0x00000010U)            /*!<IO default mode */

#define  TSC_CR_MCV                          ((uint32_t)0x000000E0U)            /*!<MCV[2:0] bits (Max Count Value) */
#define  TSC_CR_MCV_0                        ((uint32_t)0x00000020U)            /*!<Bit 0 */
#define  TSC_CR_MCV_1                        ((uint32_t)0x00000040U)            /*!<Bit 1 */
#define  TSC_CR_MCV_2                        ((uint32_t)0x00000080U)            /*!<Bit 2 */

#define  TSC_CR_PGPSC                        ((uint32_t)0x00007000U)            /*!<PGPSC[2:0] bits (Pulse Generator Prescaler) */
#define  TSC_CR_PGPSC_0                      ((uint32_t)0x00001000U)            /*!<Bit 0 */
#define  TSC_CR_PGPSC_1                      ((uint32_t)0x00002000U)            /*!<Bit 1 */
#define  TSC_CR_PGPSC_2                      ((uint32_t)0x00004000U)            /*!<Bit 2 */

#define  TSC_CR_SSPSC                        ((uint32_t)0x00008000U)            /*!<Spread Spectrum Prescaler */
#define  TSC_CR_SSE                          ((uint32_t)0x00010000U)            /*!<Spread Spectrum Enable */

#define  TSC_CR_SSD                          ((uint32_t)0x00FE0000U)            /*!<SSD[6:0] bits (Spread Spectrum Deviation) */
#define  TSC_CR_SSD_0                        ((uint32_t)0x00020000U)            /*!<Bit 0 */
#define  TSC_CR_SSD_1                        ((uint32_t)0x00040000U)            /*!<Bit 1 */
#define  TSC_CR_SSD_2                        ((uint32_t)0x00080000U)            /*!<Bit 2 */
#define  TSC_CR_SSD_3                        ((uint32_t)0x00100000U)            /*!<Bit 3 */
#define  TSC_CR_SSD_4                        ((uint32_t)0x00200000U)            /*!<Bit 4 */
#define  TSC_CR_SSD_5                        ((uint32_t)0x00400000U)            /*!<Bit 5 */
#define  TSC_CR_SSD_6                        ((uint32_t)0x00800000U)            /*!<Bit 6 */

#define  TSC_CR_CTPL                         ((uint32_t)0x0F000000U)            /*!<CTPL[3:0] bits (Charge Transfer pulse low) */
#define  TSC_CR_CTPL_0                       ((uint32_t)0x01000000U)            /*!<Bit 0 */
#define  TSC_CR_CTPL_1                       ((uint32_t)0x02000000U)            /*!<Bit 1 */
#define  TSC_CR_CTPL_2                       ((uint32_t)0x04000000U)            /*!<Bit 2 */
#define  TSC_CR_CTPL_3                       ((uint32_t)0x08000000U)            /*!<Bit 3 */

#define  TSC_CR_CTPH                         ((uint32_t)0xF0000000U)            /*!<CTPH[3:0] bits (Charge Transfer pulse high) */
#define  TSC_CR_CTPH_0                       ((uint32_t)0x10000000U)            /*!<Bit 0 */
#define  TSC_CR_CTPH_1                       ((uint32_t)0x20000000U)            /*!<Bit 1 */
#define  TSC_CR_CTPH_2                       ((uint32_t)0x40000000U)            /*!<Bit 2 */
#define  TSC_CR_CTPH_3                       ((uint32_t)0x80000000U)            /*!<Bit 3 */

/*******************  Bit definition for TSC_IER register  ********************/
#define  TSC_IER_EOAIE                       ((uint32_t)0x00000001U)            /*!<End of acquisition interrupt enable */
#define  TSC_IER_MCEIE                       ((uint32_t)0x00000002U)            /*!<Max count error interrupt enable */

/*******************  Bit definition for TSC_ICR register  ********************/
#define  TSC_ICR_EOAIC                       ((uint32_t)0x00000001U)            /*!<End of acquisition interrupt clear */
#define  TSC_ICR_MCEIC                       ((uint32_t)0x00000002U)            /*!<Max count error interrupt clear */

/*******************  Bit definition for TSC_ISR register  ********************/
#define  TSC_ISR_EOAF                        ((uint32_t)0x00000001U)            /*!<End of acquisition flag */
#define  TSC_ISR_MCEF                        ((uint32_t)0x00000002U)            /*!<Max count error flag */

/*******************  Bit definition for TSC_IOHCR register  ******************/
#define  TSC_IOHCR_G1_IO1                    ((uint32_t)0x00000001U)            /*!<GROUP1_IO1 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G1_IO2                    ((uint32_t)0x00000002U)            /*!<GROUP1_IO2 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G1_IO3                    ((uint32_t)0x00000004U)            /*!<GROUP1_IO3 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G1_IO4                    ((uint32_t)0x00000008U)            /*!<GROUP1_IO4 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G2_IO1                    ((uint32_t)0x00000010U)            /*!<GROUP2_IO1 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G2_IO2                    ((uint32_t)0x00000020U)            /*!<GROUP2_IO2 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G2_IO3                    ((uint32_t)0x00000040U)            /*!<GROUP2_IO3 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G2_IO4                    ((uint32_t)0x00000080U)            /*!<GROUP2_IO4 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G3_IO1                    ((uint32_t)0x00000100U)            /*!<GROUP3_IO1 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G3_IO2                    ((uint32_t)0x00000200U)            /*!<GROUP3_IO2 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G3_IO3                    ((uint32_t)0x00000400U)            /*!<GROUP3_IO3 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G3_IO4                    ((uint32_t)0x00000800U)            /*!<GROUP3_IO4 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G4_IO1                    ((uint32_t)0x00001000U)            /*!<GROUP4_IO1 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G4_IO2                    ((uint32_t)0x00002000U)            /*!<GROUP4_IO2 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G4_IO3                    ((uint32_t)0x00004000U)            /*!<GROUP4_IO3 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G4_IO4                    ((uint32_t)0x00008000U)            /*!<GROUP4_IO4 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G5_IO1                    ((uint32_t)0x00010000U)            /*!<GROUP5_IO1 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G5_IO2                    ((uint32_t)0x00020000U)            /*!<GROUP5_IO2 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G5_IO3                    ((uint32_t)0x00040000U)            /*!<GROUP5_IO3 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G5_IO4                    ((uint32_t)0x00080000U)            /*!<GROUP5_IO4 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G6_IO1                    ((uint32_t)0x00100000U)            /*!<GROUP6_IO1 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G6_IO2                    ((uint32_t)0x00200000U)            /*!<GROUP6_IO2 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G6_IO3                    ((uint32_t)0x00400000U)            /*!<GROUP6_IO3 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G6_IO4                    ((uint32_t)0x00800000U)            /*!<GROUP6_IO4 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G7_IO1                    ((uint32_t)0x01000000U)            /*!<GROUP7_IO1 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G7_IO2                    ((uint32_t)0x02000000U)            /*!<GROUP7_IO2 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G7_IO3                    ((uint32_t)0x04000000U)            /*!<GROUP7_IO3 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G7_IO4                    ((uint32_t)0x08000000U)            /*!<GROUP7_IO4 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G8_IO1                    ((uint32_t)0x10000000U)            /*!<GROUP8_IO1 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G8_IO2                    ((uint32_t)0x20000000U)            /*!<GROUP8_IO2 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G8_IO3                    ((uint32_t)0x40000000U)            /*!<GROUP8_IO3 schmitt trigger hysteresis mode */
#define  TSC_IOHCR_G8_IO4                    ((uint32_t)0x80000000U)            /*!<GROUP8_IO4 schmitt trigger hysteresis mode */

/*******************  Bit definition for TSC_IOASCR register  *****************/
#define  TSC_IOASCR_G1_IO1                   ((uint32_t)0x00000001U)            /*!<GROUP1_IO1 analog switch enable */
#define  TSC_IOASCR_G1_IO2                   ((uint32_t)0x00000002U)            /*!<GROUP1_IO2 analog switch enable */
#define  TSC_IOASCR_G1_IO3                   ((uint32_t)0x00000004U)            /*!<GROUP1_IO3 analog switch enable */
#define  TSC_IOASCR_G1_IO4                   ((uint32_t)0x00000008U)            /*!<GROUP1_IO4 analog switch enable */
#define  TSC_IOASCR_G2_IO1                   ((uint32_t)0x00000010U)            /*!<GROUP2_IO1 analog switch enable */
#define  TSC_IOASCR_G2_IO2                   ((uint32_t)0x00000020U)            /*!<GROUP2_IO2 analog switch enable */
#define  TSC_IOASCR_G2_IO3                   ((uint32_t)0x00000040U)            /*!<GROUP2_IO3 analog switch enable */
#define  TSC_IOASCR_G2_IO4                   ((uint32_t)0x00000080U)            /*!<GROUP2_IO4 analog switch enable */
#define  TSC_IOASCR_G3_IO1                   ((uint32_t)0x00000100U)            /*!<GROUP3_IO1 analog switch enable */
#define  TSC_IOASCR_G3_IO2                   ((uint32_t)0x00000200U)            /*!<GROUP3_IO2 analog switch enable */
#define  TSC_IOASCR_G3_IO3                   ((uint32_t)0x00000400U)            /*!<GROUP3_IO3 analog switch enable */
#define  TSC_IOASCR_G3_IO4                   ((uint32_t)0x00000800U)            /*!<GROUP3_IO4 analog switch enable */
#define  TSC_IOASCR_G4_IO1                   ((uint32_t)0x00001000U)            /*!<GROUP4_IO1 analog switch enable */
#define  TSC_IOASCR_G4_IO2                   ((uint32_t)0x00002000U)            /*!<GROUP4_IO2 analog switch enable */
#define  TSC_IOASCR_G4_IO3                   ((uint32_t)0x00004000U)            /*!<GROUP4_IO3 analog switch enable */
#define  TSC_IOASCR_G4_IO4                   ((uint32_t)0x00008000U)            /*!<GROUP4_IO4 analog switch enable */
#define  TSC_IOASCR_G5_IO1                   ((uint32_t)0x00010000U)            /*!<GROUP5_IO1 analog switch enable */
#define  TSC_IOASCR_G5_IO2                   ((uint32_t)0x00020000U)            /*!<GROUP5_IO2 analog switch enable */
#define  TSC_IOASCR_G5_IO3                   ((uint32_t)0x00040000U)            /*!<GROUP5_IO3 analog switch enable */
#define  TSC_IOASCR_G5_IO4                   ((uint32_t)0x00080000U)            /*!<GROUP5_IO4 analog switch enable */
#define  TSC_IOASCR_G6_IO1                   ((uint32_t)0x00100000U)            /*!<GROUP6_IO1 analog switch enable */
#define  TSC_IOASCR_G6_IO2                   ((uint32_t)0x00200000U)            /*!<GROUP6_IO2 analog switch enable */
#define  TSC_IOASCR_G6_IO3                   ((uint32_t)0x00400000U)            /*!<GROUP6_IO3 analog switch enable */
#define  TSC_IOASCR_G6_IO4                   ((uint32_t)0x00800000U)            /*!<GROUP6_IO4 analog switch enable */
#define  TSC_IOASCR_G7_IO1                   ((uint32_t)0x01000000U)            /*!<GROUP7_IO1 analog switch enable */
#define  TSC_IOASCR_G7_IO2                   ((uint32_t)0x02000000U)            /*!<GROUP7_IO2 analog switch enable */
#define  TSC_IOASCR_G7_IO3                   ((uint32_t)0x04000000U)            /*!<GROUP7_IO3 analog switch enable */
#define  TSC_IOASCR_G7_IO4                   ((uint32_t)0x08000000U)            /*!<GROUP7_IO4 analog switch enable */
#define  TSC_IOASCR_G8_IO1                   ((uint32_t)0x10000000U)            /*!<GROUP8_IO1 analog switch enable */
#define  TSC_IOASCR_G8_IO2                   ((uint32_t)0x20000000U)            /*!<GROUP8_IO2 analog switch enable */
#define  TSC_IOASCR_G8_IO3                   ((uint32_t)0x40000000U)            /*!<GROUP8_IO3 analog switch enable */
#define  TSC_IOASCR_G8_IO4                   ((uint32_t)0x80000000U)            /*!<GROUP8_IO4 analog switch enable */

/*******************  Bit definition for TSC_IOSCR register  ******************/
#define  TSC_IOSCR_G1_IO1                    ((uint32_t)0x00000001U)            /*!<GROUP1_IO1 sampling mode */
#define  TSC_IOSCR_G1_IO2                    ((uint32_t)0x00000002U)            /*!<GROUP1_IO2 sampling mode */
#define  TSC_IOSCR_G1_IO3                    ((uint32_t)0x00000004U)            /*!<GROUP1_IO3 sampling mode */
#define  TSC_IOSCR_G1_IO4                    ((uint32_t)0x00000008U)            /*!<GROUP1_IO4 sampling mode */
#define  TSC_IOSCR_G2_IO1                    ((uint32_t)0x00000010U)            /*!<GROUP2_IO1 sampling mode */
#define  TSC_IOSCR_G2_IO2                    ((uint32_t)0x00000020U)            /*!<GROUP2_IO2 sampling mode */
#define  TSC_IOSCR_G2_IO3                    ((uint32_t)0x00000040U)            /*!<GROUP2_IO3 sampling mode */
#define  TSC_IOSCR_G2_IO4                    ((uint32_t)0x00000080U)            /*!<GROUP2_IO4 sampling mode */
#define  TSC_IOSCR_G3_IO1                    ((uint32_t)0x00000100U)            /*!<GROUP3_IO1 sampling mode */
#define  TSC_IOSCR_G3_IO2                    ((uint32_t)0x00000200U)            /*!<GROUP3_IO2 sampling mode */
#define  TSC_IOSCR_G3_IO3                    ((uint32_t)0x00000400U)            /*!<GROUP3_IO3 sampling mode */
#define  TSC_IOSCR_G3_IO4                    ((uint32_t)0x00000800U)            /*!<GROUP3_IO4 sampling mode */
#define  TSC_IOSCR_G4_IO1                    ((uint32_t)0x00001000U)            /*!<GROUP4_IO1 sampling mode */
#define  TSC_IOSCR_G4_IO2                    ((uint32_t)0x00002000U)            /*!<GROUP4_IO2 sampling mode */
#define  TSC_IOSCR_G4_IO3                    ((uint32_t)0x00004000U)            /*!<GROUP4_IO3 sampling mode */
#define  TSC_IOSCR_G4_IO4                    ((uint32_t)0x00008000U)            /*!<GROUP4_IO4 sampling mode */
#define  TSC_IOSCR_G5_IO1                    ((uint32_t)0x00010000U)            /*!<GROUP5_IO1 sampling mode */
#define  TSC_IOSCR_G5_IO2                    ((uint32_t)0x00020000U)            /*!<GROUP5_IO2 sampling mode */
#define  TSC_IOSCR_G5_IO3                    ((uint32_t)0x00040000U)            /*!<GROUP5_IO3 sampling mode */
#define  TSC_IOSCR_G5_IO4                    ((uint32_t)0x00080000U)            /*!<GROUP5_IO4 sampling mode */
#define  TSC_IOSCR_G6_IO1                    ((uint32_t)0x00100000U)            /*!<GROUP6_IO1 sampling mode */
#define  TSC_IOSCR_G6_IO2                    ((uint32_t)0x00200000U)            /*!<GROUP6_IO2 sampling mode */
#define  TSC_IOSCR_G6_IO3                    ((uint32_t)0x00400000U)            /*!<GROUP6_IO3 sampling mode */
#define  TSC_IOSCR_G6_IO4                    ((uint32_t)0x00800000U)            /*!<GROUP6_IO4 sampling mode */
#define  TSC_IOSCR_G7_IO1                    ((uint32_t)0x01000000U)            /*!<GROUP7_IO1 sampling mode */
#define  TSC_IOSCR_G7_IO2                    ((uint32_t)0x02000000U)            /*!<GROUP7_IO2 sampling mode */
#define  TSC_IOSCR_G7_IO3                    ((uint32_t)0x04000000U)            /*!<GROUP7_IO3 sampling mode */
#define  TSC_IOSCR_G7_IO4                    ((uint32_t)0x08000000U)            /*!<GROUP7_IO4 sampling mode */
#define  TSC_IOSCR_G8_IO1                    ((uint32_t)0x10000000U)            /*!<GROUP8_IO1 sampling mode */
#define  TSC_IOSCR_G8_IO2                    ((uint32_t)0x20000000U)            /*!<GROUP8_IO2 sampling mode */
#define  TSC_IOSCR_G8_IO3                    ((uint32_t)0x40000000U)            /*!<GROUP8_IO3 sampling mode */
#define  TSC_IOSCR_G8_IO4                    ((uint32_t)0x80000000U)            /*!<GROUP8_IO4 sampling mode */

/*******************  Bit definition for TSC_IOCCR register  ******************/
#define  TSC_IOCCR_G1_IO1                    ((uint32_t)0x00000001U)            /*!<GROUP1_IO1 channel mode */
#define  TSC_IOCCR_G1_IO2                    ((uint32_t)0x00000002U)            /*!<GROUP1_IO2 channel mode */
#define  TSC_IOCCR_G1_IO3                    ((uint32_t)0x00000004U)            /*!<GROUP1_IO3 channel mode */
#define  TSC_IOCCR_G1_IO4                    ((uint32_t)0x00000008U)            /*!<GROUP1_IO4 channel mode */
#define  TSC_IOCCR_G2_IO1                    ((uint32_t)0x00000010U)            /*!<GROUP2_IO1 channel mode */
#define  TSC_IOCCR_G2_IO2                    ((uint32_t)0x00000020U)            /*!<GROUP2_IO2 channel mode */
#define  TSC_IOCCR_G2_IO3                    ((uint32_t)0x00000040U)            /*!<GROUP2_IO3 channel mode */
#define  TSC_IOCCR_G2_IO4                    ((uint32_t)0x00000080U)            /*!<GROUP2_IO4 channel mode */
#define  TSC_IOCCR_G3_IO1                    ((uint32_t)0x00000100U)            /*!<GROUP3_IO1 channel mode */
#define  TSC_IOCCR_G3_IO2                    ((uint32_t)0x00000200U)            /*!<GROUP3_IO2 channel mode */
#define  TSC_IOCCR_G3_IO3                    ((uint32_t)0x00000400U)            /*!<GROUP3_IO3 channel mode */
#define  TSC_IOCCR_G3_IO4                    ((uint32_t)0x00000800U)            /*!<GROUP3_IO4 channel mode */
#define  TSC_IOCCR_G4_IO1                    ((uint32_t)0x00001000U)            /*!<GROUP4_IO1 channel mode */
#define  TSC_IOCCR_G4_IO2                    ((uint32_t)0x00002000U)            /*!<GROUP4_IO2 channel mode */
#define  TSC_IOCCR_G4_IO3                    ((uint32_t)0x00004000U)            /*!<GROUP4_IO3 channel mode */
#define  TSC_IOCCR_G4_IO4                    ((uint32_t)0x00008000U)            /*!<GROUP4_IO4 channel mode */
#define  TSC_IOCCR_G5_IO1                    ((uint32_t)0x00010000U)            /*!<GROUP5_IO1 channel mode */
#define  TSC_IOCCR_G5_IO2                    ((uint32_t)0x00020000U)            /*!<GROUP5_IO2 channel mode */
#define  TSC_IOCCR_G5_IO3                    ((uint32_t)0x00040000U)            /*!<GROUP5_IO3 channel mode */
#define  TSC_IOCCR_G5_IO4                    ((uint32_t)0x00080000U)            /*!<GROUP5_IO4 channel mode */
#define  TSC_IOCCR_G6_IO1                    ((uint32_t)0x00100000U)            /*!<GROUP6_IO1 channel mode */
#define  TSC_IOCCR_G6_IO2                    ((uint32_t)0x00200000U)            /*!<GROUP6_IO2 channel mode */
#define  TSC_IOCCR_G6_IO3                    ((uint32_t)0x00400000U)            /*!<GROUP6_IO3 channel mode */
#define  TSC_IOCCR_G6_IO4                    ((uint32_t)0x00800000U)            /*!<GROUP6_IO4 channel mode */
#define  TSC_IOCCR_G7_IO1                    ((uint32_t)0x01000000U)            /*!<GROUP7_IO1 channel mode */
#define  TSC_IOCCR_G7_IO2                    ((uint32_t)0x02000000U)            /*!<GROUP7_IO2 channel mode */
#define  TSC_IOCCR_G7_IO3                    ((uint32_t)0x04000000U)            /*!<GROUP7_IO3 channel mode */
#define  TSC_IOCCR_G7_IO4                    ((uint32_t)0x08000000U)            /*!<GROUP7_IO4 channel mode */
#define  TSC_IOCCR_G8_IO1                    ((uint32_t)0x10000000U)            /*!<GROUP8_IO1 channel mode */
#define  TSC_IOCCR_G8_IO2                    ((uint32_t)0x20000000U)            /*!<GROUP8_IO2 channel mode */
#define  TSC_IOCCR_G8_IO3                    ((uint32_t)0x40000000U)            /*!<GROUP8_IO3 channel mode */
#define  TSC_IOCCR_G8_IO4                    ((uint32_t)0x80000000U)            /*!<GROUP8_IO4 channel mode */

/*******************  Bit definition for TSC_IOGCSR register  *****************/
#define  TSC_IOGCSR_G1E                      ((uint32_t)0x00000001U)            /*!<Analog IO GROUP1 enable */
#define  TSC_IOGCSR_G2E                      ((uint32_t)0x00000002U)            /*!<Analog IO GROUP2 enable */
#define  TSC_IOGCSR_G3E                      ((uint32_t)0x00000004U)            /*!<Analog IO GROUP3 enable */
#define  TSC_IOGCSR_G4E                      ((uint32_t)0x00000008U)            /*!<Analog IO GROUP4 enable */
#define  TSC_IOGCSR_G5E                      ((uint32_t)0x00000010U)            /*!<Analog IO GROUP5 enable */
#define  TSC_IOGCSR_G6E                      ((uint32_t)0x00000020U)            /*!<Analog IO GROUP6 enable */
#define  TSC_IOGCSR_G7E                      ((uint32_t)0x00000040U)            /*!<Analog IO GROUP7 enable */
#define  TSC_IOGCSR_G8E                      ((uint32_t)0x00000080U)            /*!<Analog IO GROUP8 enable */
#define  TSC_IOGCSR_G1S                      ((uint32_t)0x00010000U)            /*!<Analog IO GROUP1 status */
#define  TSC_IOGCSR_G2S                      ((uint32_t)0x00020000U)            /*!<Analog IO GROUP2 status */
#define  TSC_IOGCSR_G3S                      ((uint32_t)0x00040000U)            /*!<Analog IO GROUP3 status */
#define  TSC_IOGCSR_G4S                      ((uint32_t)0x00080000U)            /*!<Analog IO GROUP4 status */
#define  TSC_IOGCSR_G5S                      ((uint32_t)0x00100000U)            /*!<Analog IO GROUP5 status */
#define  TSC_IOGCSR_G6S                      ((uint32_t)0x00200000U)            /*!<Analog IO GROUP6 status */
#define  TSC_IOGCSR_G7S                      ((uint32_t)0x00400000U)            /*!<Analog IO GROUP7 status */
#define  TSC_IOGCSR_G8S                      ((uint32_t)0x00800000U)            /*!<Analog IO GROUP8 status */

/*******************  Bit definition for TSC_IOGXCR register  *****************/
#define  TSC_IOGXCR_CNT                      ((uint32_t)0x00003FFFU)            /*!<CNT[13:0] bits (Counter value) */

/******************************************************************************/
/*                                                                            */
/*      Universal Synchronous Asynchronous Receiver Transmitter (USART)       */
/*                                                                            */
/******************************************************************************/
/******************  Bit definition for USART_CR1 register  *******************/
#define  USART_CR1_UE                        ((uint32_t)0x00000001U)            /*!< USART Enable */
#define  USART_CR1_UESM                      ((uint32_t)0x00000002U)            /*!< USART Enable in STOP Mode */
#define  USART_CR1_RE                        ((uint32_t)0x00000004U)            /*!< Receiver Enable */
#define  USART_CR1_TE                        ((uint32_t)0x00000008U)            /*!< Transmitter Enable */
#define  USART_CR1_IDLEIE                    ((uint32_t)0x00000010U)            /*!< IDLE Interrupt Enable */
#define  USART_CR1_RXNEIE                    ((uint32_t)0x00000020U)            /*!< RXNE Interrupt Enable */
#define  USART_CR1_TCIE                      ((uint32_t)0x00000040U)            /*!< Transmission Complete Interrupt Enable */
#define  USART_CR1_TXEIE                     ((uint32_t)0x00000080U)            /*!< TXE Interrupt Enable */
#define  USART_CR1_PEIE                      ((uint32_t)0x00000100U)            /*!< PE Interrupt Enable */
#define  USART_CR1_PS                        ((uint32_t)0x00000200U)            /*!< Parity Selection */
#define  USART_CR1_PCE                       ((uint32_t)0x00000400U)            /*!< Parity Control Enable */
#define  USART_CR1_WAKE                      ((uint32_t)0x00000800U)            /*!< Receiver Wakeup method */
#define  USART_CR1_M                         ((uint32_t)0x00001000U)            /*!< Word Length */
#define  USART_CR1_MME                       ((uint32_t)0x00002000U)            /*!< Mute Mode Enable */
#define  USART_CR1_CMIE                      ((uint32_t)0x00004000U)            /*!< Character match interrupt enable */
#define  USART_CR1_OVER8                     ((uint32_t)0x00008000U)            /*!< Oversampling by 8-bit or 16-bit mode */
#define  USART_CR1_DEDT                      ((uint32_t)0x001F0000U)            /*!< DEDT[4:0] bits (Driver Enable Deassertion Time) */
#define  USART_CR1_DEDT_0                    ((uint32_t)0x00010000U)            /*!< Bit 0 */
#define  USART_CR1_DEDT_1                    ((uint32_t)0x00020000U)            /*!< Bit 1 */
#define  USART_CR1_DEDT_2                    ((uint32_t)0x00040000U)            /*!< Bit 2 */
#define  USART_CR1_DEDT_3                    ((uint32_t)0x00080000U)            /*!< Bit 3 */
#define  USART_CR1_DEDT_4                    ((uint32_t)0x00100000U)            /*!< Bit 4 */
#define  USART_CR1_DEAT                      ((uint32_t)0x03E00000U)            /*!< DEAT[4:0] bits (Driver Enable Assertion Time) */
#define  USART_CR1_DEAT_0                    ((uint32_t)0x00200000U)            /*!< Bit 0 */
#define  USART_CR1_DEAT_1                    ((uint32_t)0x00400000U)            /*!< Bit 1 */
#define  USART_CR1_DEAT_2                    ((uint32_t)0x00800000U)            /*!< Bit 2 */
#define  USART_CR1_DEAT_3                    ((uint32_t)0x01000000U)            /*!< Bit 3 */
#define  USART_CR1_DEAT_4                    ((uint32_t)0x02000000U)            /*!< Bit 4 */
#define  USART_CR1_RTOIE                     ((uint32_t)0x04000000U)            /*!< Receive Time Out interrupt enable */
#define  USART_CR1_EOBIE                     ((uint32_t)0x08000000U)            /*!< End of Block interrupt enable */

/******************  Bit definition for USART_CR2 register  *******************/
#define  USART_CR2_ADDM7                     ((uint32_t)0x00000010U)            /*!< 7-bit or 4-bit Address Detection */
#define  USART_CR2_LBDL                      ((uint32_t)0x00000020U)            /*!< LIN Break Detection Length */
#define  USART_CR2_LBDIE                     ((uint32_t)0x00000040U)            /*!< LIN Break Detection Interrupt Enable */
#define  USART_CR2_LBCL                      ((uint32_t)0x00000100U)            /*!< Last Bit Clock pulse */
#define  USART_CR2_CPHA                      ((uint32_t)0x00000200U)            /*!< Clock Phase */
#define  USART_CR2_CPOL                      ((uint32_t)0x00000400U)            /*!< Clock Polarity */
#define  USART_CR2_CLKEN                     ((uint32_t)0x00000800U)            /*!< Clock Enable */
#define  USART_CR2_STOP                      ((uint32_t)0x00003000U)            /*!< STOP[1:0] bits (STOP bits) */
#define  USART_CR2_STOP_0                    ((uint32_t)0x00001000U)            /*!< Bit 0 */
#define  USART_CR2_STOP_1                    ((uint32_t)0x00002000U)            /*!< Bit 1 */
#define  USART_CR2_LINEN                     ((uint32_t)0x00004000U)            /*!< LIN mode enable */
#define  USART_CR2_SWAP                      ((uint32_t)0x00008000U)            /*!< SWAP TX/RX pins */
#define  USART_CR2_RXINV                     ((uint32_t)0x00010000U)            /*!< RX pin active level inversion */
#define  USART_CR2_TXINV                     ((uint32_t)0x00020000U)            /*!< TX pin active level inversion */
#define  USART_CR2_DATAINV                   ((uint32_t)0x00040000U)            /*!< Binary data inversion */
#define  USART_CR2_MSBFIRST                  ((uint32_t)0x00080000U)            /*!< Most Significant Bit First */
#define  USART_CR2_ABREN                     ((uint32_t)0x00100000U)            /*!< Auto Baud-Rate Enable*/
#define  USART_CR2_ABRMODE                   ((uint32_t)0x00600000U)            /*!< ABRMOD[1:0] bits (Auto Baud-Rate Mode) */
#define  USART_CR2_ABRMODE_0                 ((uint32_t)0x00200000U)            /*!< Bit 0 */
#define  USART_CR2_ABRMODE_1                 ((uint32_t)0x00400000U)            /*!< Bit 1 */
#define  USART_CR2_RTOEN                     ((uint32_t)0x00800000U)            /*!< Receiver Time-Out enable */
#define  USART_CR2_ADD                       ((uint32_t)0xFF000000U)            /*!< Address of the USART node */

/******************  Bit definition for USART_CR3 register  *******************/
#define  USART_CR3_EIE                       ((uint32_t)0x00000001U)            /*!< Error Interrupt Enable */
#define  USART_CR3_IREN                      ((uint32_t)0x00000002U)            /*!< IrDA mode Enable */
#define  USART_CR3_IRLP                      ((uint32_t)0x00000004U)            /*!< IrDA Low-Power */
#define  USART_CR3_HDSEL                     ((uint32_t)0x00000008U)            /*!< Half-Duplex Selection */
#define  USART_CR3_NACK                      ((uint32_t)0x00000010U)            /*!< SmartCard NACK enable */
#define  USART_CR3_SCEN                      ((uint32_t)0x00000020U)            /*!< SmartCard mode enable */
#define  USART_CR3_DMAR                      ((uint32_t)0x00000040U)            /*!< DMA Enable Receiver */
#define  USART_CR3_DMAT                      ((uint32_t)0x00000080U)            /*!< DMA Enable Transmitter */
#define  USART_CR3_RTSE                      ((uint32_t)0x00000100U)            /*!< RTS Enable */
#define  USART_CR3_CTSE                      ((uint32_t)0x00000200U)            /*!< CTS Enable */
#define  USART_CR3_CTSIE                     ((uint32_t)0x00000400U)            /*!< CTS Interrupt Enable */
#define  USART_CR3_ONEBIT                    ((uint32_t)0x00000800U)            /*!< One sample bit method enable */
#define  USART_CR3_OVRDIS                    ((uint32_t)0x00001000U)            /*!< Overrun Disable */
#define  USART_CR3_DDRE                      ((uint32_t)0x00002000U)            /*!< DMA Disable on Reception Error */
#define  USART_CR3_DEM                       ((uint32_t)0x00004000U)            /*!< Driver Enable Mode */
#define  USART_CR3_DEP                       ((uint32_t)0x00008000U)            /*!< Driver Enable Polarity Selection */
#define  USART_CR3_SCARCNT                   ((uint32_t)0x000E0000U)            /*!< SCARCNT[2:0] bits (SmartCard Auto-Retry Count) */
#define  USART_CR3_SCARCNT_0                 ((uint32_t)0x00020000U)            /*!< Bit 0 */
#define  USART_CR3_SCARCNT_1                 ((uint32_t)0x00040000U)            /*!< Bit 1 */
#define  USART_CR3_SCARCNT_2                 ((uint32_t)0x00080000U)            /*!< Bit 2 */
#define  USART_CR3_WUS                       ((uint32_t)0x00300000U)            /*!< WUS[1:0] bits (Wake UP Interrupt Flag Selection) */
#define  USART_CR3_WUS_0                     ((uint32_t)0x00100000U)            /*!< Bit 0 */
#define  USART_CR3_WUS_1                     ((uint32_t)0x00200000U)            /*!< Bit 1 */
#define  USART_CR3_WUFIE                     ((uint32_t)0x00400000U)            /*!< Wake Up Interrupt Enable */

/******************  Bit definition for USART_BRR register  *******************/
#define  USART_BRR_DIV_FRACTION              ((uint32_t)0x0000000FU)                /*!< Fraction of USARTDIV */
#define  USART_BRR_DIV_MANTISSA              ((uint32_t)0x0000FFF0U)                /*!< Mantissa of USARTDIV */

/******************  Bit definition for USART_GTPR register  ******************/
#define  USART_GTPR_PSC                      ((uint32_t)0x000000FFU)                /*!< PSC[7:0] bits (Prescaler value) */
#define  USART_GTPR_GT                       ((uint32_t)0x0000FF00U)                /*!< GT[7:0] bits (Guard time value) */


/*******************  Bit definition for USART_RTOR register  *****************/
#define  USART_RTOR_RTO                      ((uint32_t)0x00FFFFFFU)            /*!< Receiver Time Out Value */
#define  USART_RTOR_BLEN                     ((uint32_t)0xFF000000U)            /*!< Block Length */

/*******************  Bit definition for USART_RQR register  ******************/
#define  USART_RQR_ABRRQ                     ((uint32_t)0x00000001U)                /*!< Auto-Baud Rate Request */
#define  USART_RQR_SBKRQ                     ((uint32_t)0x00000002U)                /*!< Send Break Request */
#define  USART_RQR_MMRQ                      ((uint32_t)0x00000004U)                /*!< Mute Mode Request */
#define  USART_RQR_RXFRQ                     ((uint32_t)0x00000008U)                /*!< Receive Data flush Request */
#define  USART_RQR_TXFRQ                     ((uint32_t)0x00000010U)                /*!< Transmit data flush Request */

/*******************  Bit definition for USART_ISR register  ******************/
#define  USART_ISR_PE                        ((uint32_t)0x00000001U)            /*!< Parity Error */
#define  USART_ISR_FE                        ((uint32_t)0x00000002U)            /*!< Framing Error */
#define  USART_ISR_NE                        ((uint32_t)0x00000004U)            /*!< Noise detected Flag */
#define  USART_ISR_ORE                       ((uint32_t)0x00000008U)            /*!< OverRun Error */
#define  USART_ISR_IDLE                      ((uint32_t)0x00000010U)            /*!< IDLE line detected */
#define  USART_ISR_RXNE                      ((uint32_t)0x00000020U)            /*!< Read Data Register Not Empty */
#define  USART_ISR_TC                        ((uint32_t)0x00000040U)            /*!< Transmission Complete */
#define  USART_ISR_TXE                       ((uint32_t)0x00000080U)            /*!< Transmit Data Register Empty */
#define  USART_ISR_LBDF                      ((uint32_t)0x00000100U)            /*!< LIN Break Detection Flag */
#define  USART_ISR_CTSIF                     ((uint32_t)0x00000200U)            /*!< CTS interrupt flag */
#define  USART_ISR_CTS                       ((uint32_t)0x00000400U)            /*!< CTS flag */
#define  USART_ISR_RTOF                      ((uint32_t)0x00000800U)            /*!< Receiver Time Out */
#define  USART_ISR_EOBF                      ((uint32_t)0x00001000U)            /*!< End Of Block Flag */
#define  USART_ISR_ABRE                      ((uint32_t)0x00004000U)            /*!< Auto-Baud Rate Error */
#define  USART_ISR_ABRF                      ((uint32_t)0x00008000U)            /*!< Auto-Baud Rate Flag */
#define  USART_ISR_BUSY                      ((uint32_t)0x00010000U)            /*!< Busy Flag */
#define  USART_ISR_CMF                       ((uint32_t)0x00020000U)            /*!< Character Match Flag */
#define  USART_ISR_SBKF                      ((uint32_t)0x00040000U)            /*!< Send Break Flag */
#define  USART_ISR_RWU                       ((uint32_t)0x00080000U)            /*!< Receive Wake Up from mute mode Flag */
#define  USART_ISR_WUF                       ((uint32_t)0x00100000U)            /*!< Wake Up from stop mode Flag */
#define  USART_ISR_TEACK                     ((uint32_t)0x00200000U)            /*!< Transmit Enable Acknowledge Flag */
#define  USART_ISR_REACK                     ((uint32_t)0x00400000U)            /*!< Receive Enable Acknowledge Flag */

/*******************  Bit definition for USART_ICR register  ******************/
#define  USART_ICR_PECF                      ((uint32_t)0x00000001U)            /*!< Parity Error Clear Flag */
#define  USART_ICR_FECF                      ((uint32_t)0x00000002U)            /*!< Framing Error Clear Flag */
#define  USART_ICR_NCF                       ((uint32_t)0x00000004U)            /*!< Noise detected Clear Flag */
#define  USART_ICR_ORECF                     ((uint32_t)0x00000008U)            /*!< OverRun Error Clear Flag */
#define  USART_ICR_IDLECF                    ((uint32_t)0x00000010U)            /*!< IDLE line detected Clear Flag */
#define  USART_ICR_TCCF                      ((uint32_t)0x00000040U)            /*!< Transmission Complete Clear Flag */
#define  USART_ICR_LBDCF                     ((uint32_t)0x00000100U)            /*!< LIN Break Detection Clear Flag */
#define  USART_ICR_CTSCF                     ((uint32_t)0x00000200U)            /*!< CTS Interrupt Clear Flag */
#define  USART_ICR_RTOCF                     ((uint32_t)0x00000800U)            /*!< Receiver Time Out Clear Flag */
#define  USART_ICR_EOBCF                     ((uint32_t)0x00001000U)            /*!< End Of Block Clear Flag */
#define  USART_ICR_CMCF                      ((uint32_t)0x00020000U)            /*!< Character Match Clear Flag */
#define  USART_ICR_WUCF                      ((uint32_t)0x00100000U)            /*!< Wake Up from stop mode Clear Flag */

/*******************  Bit definition for USART_RDR register  ******************/
#define  USART_RDR_RDR                       ((uint16_t)0x01FFU)                /*!< RDR[8:0] bits (Receive Data value) */

/*******************  Bit definition for USART_TDR register  ******************/
#define  USART_TDR_TDR                       ((uint16_t)0x01FFU)                /*!< TDR[8:0] bits (Transmit Data value) */

/******************************************************************************/
/*                                                                            */
/*                         Window WATCHDOG (WWDG)                             */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for WWDG_CR register  ********************/
#define  WWDG_CR_T                           ((uint32_t)0x0000007FU)               /*!< T[6:0] bits (7-Bit counter (MSB to LSB)) */
#define  WWDG_CR_T_0                         ((uint32_t)0x00000001U)               /*!< Bit 0 */
#define  WWDG_CR_T_1                         ((uint32_t)0x00000002U)               /*!< Bit 1 */
#define  WWDG_CR_T_2                         ((uint32_t)0x00000004U)               /*!< Bit 2 */
#define  WWDG_CR_T_3                         ((uint32_t)0x00000008U)               /*!< Bit 3 */
#define  WWDG_CR_T_4                         ((uint32_t)0x00000010U)               /*!< Bit 4 */
#define  WWDG_CR_T_5                         ((uint32_t)0x00000020U)               /*!< Bit 5 */
#define  WWDG_CR_T_6                         ((uint32_t)0x00000040U)               /*!< Bit 6 */

/* Legacy defines */
#define  WWDG_CR_T0 WWDG_CR_T_0
#define  WWDG_CR_T1 WWDG_CR_T_1
#define  WWDG_CR_T2 WWDG_CR_T_2
#define  WWDG_CR_T3 WWDG_CR_T_3
#define  WWDG_CR_T4 WWDG_CR_T_4
#define  WWDG_CR_T5 WWDG_CR_T_5
#define  WWDG_CR_T6 WWDG_CR_T_6

#define  WWDG_CR_WDGA                        ((uint32_t)0x00000080U)               /*!< Activation bit */

/*******************  Bit definition for WWDG_CFR register  *******************/
#define  WWDG_CFR_W                          ((uint32_t)0x0000007FU)               /*!< W[6:0] bits (7-bit window value) */
#define  WWDG_CFR_W_0                        ((uint32_t)0x00000001U)               /*!< Bit 0 */
#define  WWDG_CFR_W_1                        ((uint32_t)0x00000002U)               /*!< Bit 1 */
#define  WWDG_CFR_W_2                        ((uint32_t)0x00000004U)               /*!< Bit 2 */
#define  WWDG_CFR_W_3                        ((uint32_t)0x00000008U)               /*!< Bit 3 */
#define  WWDG_CFR_W_4                        ((uint32_t)0x00000010U)               /*!< Bit 4 */
#define  WWDG_CFR_W_5                        ((uint32_t)0x00000020U)               /*!< Bit 5 */
#define  WWDG_CFR_W_6                        ((uint32_t)0x00000040U)               /*!< Bit 6 */

/* Legacy defines */
#define  WWDG_CFR_W0 WWDG_CFR_W_0
#define  WWDG_CFR_W1 WWDG_CFR_W_1
#define  WWDG_CFR_W2 WWDG_CFR_W_2
#define  WWDG_CFR_W3 WWDG_CFR_W_3
#define  WWDG_CFR_W4 WWDG_CFR_W_4
#define  WWDG_CFR_W5 WWDG_CFR_W_5
#define  WWDG_CFR_W6 WWDG_CFR_W_6

#define  WWDG_CFR_WDGTB                      ((uint32_t)0x00000180U)               /*!< WDGTB[1:0] bits (Timer Base) */
#define  WWDG_CFR_WDGTB_0                    ((uint32_t)0x00000080U)               /*!< Bit 0 */
#define  WWDG_CFR_WDGTB_1                    ((uint32_t)0x00000100U)               /*!< Bit 1 */

/* Legacy defines */
#define  WWDG_CFR_WDGTB0 WWDG_CFR_WDGTB_0
#define  WWDG_CFR_WDGTB1 WWDG_CFR_WDGTB_1

#define  WWDG_CFR_EWI                        ((uint32_t)0x00000200U)               /*!< Early Wakeup Interrupt */

/*******************  Bit definition for WWDG_SR register  ********************/
#define  WWDG_SR_EWIF                        ((uint32_t)0x00000001U)               /*!< Early Wakeup Interrupt Flag */

/**
  * @}
  */

 /**
  * @}
  */


/** @addtogroup Exported_macro
  * @{
  */

/****************************** ADC Instances *********************************/
#define IS_ADC_ALL_INSTANCE(INSTANCE) ((INSTANCE) == ADC1)

#define IS_ADC_COMMON_INSTANCE(INSTANCE) ((INSTANCE) == ADC)

/****************************** COMP Instances *********************************/
#define IS_COMP_ALL_INSTANCE(INSTANCE) (((INSTANCE) == COMP1) || \
                                        ((INSTANCE) == COMP2))

#define IS_COMP_COMMON_INSTANCE(INSTANCE) ((INSTANCE) == COMP12_COMMON)

#define IS_COMP_DAC1SWITCH_INSTANCE(INSTANCE) ((INSTANCE) == COMP1)

#define IS_COMP_WINDOWMODE_INSTANCE(INSTANCE) ((INSTANCE) == COMP2)

/****************************** CEC Instances *********************************/
#define IS_CEC_ALL_INSTANCE(INSTANCE) ((INSTANCE) == CEC)

/****************************** CRC Instances *********************************/
#define IS_CRC_ALL_INSTANCE(INSTANCE) ((INSTANCE) == CRC)
                                      
/******************************* DAC Instances ********************************/
#define IS_DAC_ALL_INSTANCE(INSTANCE) ((INSTANCE) == DAC1)

/******************************* DMA Instances ********************************/
#define IS_DMA_ALL_INSTANCE(INSTANCE) (((INSTANCE) == DMA1_Channel1) || \
                                       ((INSTANCE) == DMA1_Channel2) || \
                                       ((INSTANCE) == DMA1_Channel3) || \
                                       ((INSTANCE) == DMA1_Channel4) || \
                                       ((INSTANCE) == DMA1_Channel5))

/****************************** GPIO Instances ********************************/
#define IS_GPIO_ALL_INSTANCE(INSTANCE)  (((INSTANCE) == GPIOA) || \
                                         ((INSTANCE) == GPIOB) || \
                                         ((INSTANCE) == GPIOC) || \
                                         ((INSTANCE) == GPIOD) || \
                                         ((INSTANCE) == GPIOF))
                                         
/**************************** GPIO Alternate Function Instances ***************/
#define IS_GPIO_AF_INSTANCE(INSTANCE)   (((INSTANCE) == GPIOA) || \
                                         ((INSTANCE) == GPIOB))

/****************************** GPIO Lock Instances ***************************/
#define IS_GPIO_LOCK_INSTANCE(INSTANCE) (((INSTANCE) == GPIOA) || \
                                         ((INSTANCE) == GPIOB))

/****************************** I2C Instances *********************************/
#define IS_I2C_ALL_INSTANCE(INSTANCE) (((INSTANCE) == I2C1) || \
                                       ((INSTANCE) == I2C2))

/****************************** I2S Instances *********************************/
#define IS_I2S_ALL_INSTANCE(INSTANCE) (((INSTANCE) == SPI1) || \
                                       ((INSTANCE) == SPI2))

/****************************** IWDG Instances ********************************/
#define IS_IWDG_ALL_INSTANCE(INSTANCE)  ((INSTANCE) == IWDG)

/****************************** RTC Instances *********************************/
#define IS_RTC_ALL_INSTANCE(INSTANCE)  ((INSTANCE) == RTC)

/****************************** SMBUS Instances *********************************/
#define IS_SMBUS_ALL_INSTANCE(INSTANCE) ((INSTANCE) == I2C1)

/****************************** SPI Instances *********************************/
#define IS_SPI_ALL_INSTANCE(INSTANCE) (((INSTANCE) == SPI1) || \
                                       ((INSTANCE) == SPI2))

/****************************** TIM Instances *********************************/
#define IS_TIM_INSTANCE(INSTANCE)\
  (((INSTANCE) == TIM1)    || \
   ((INSTANCE) == TIM2)    || \
   ((INSTANCE) == TIM3)    || \
   ((INSTANCE) == TIM6)    || \
   ((INSTANCE) == TIM14)   || \
   ((INSTANCE) == TIM15)   || \
   ((INSTANCE) == TIM16)   || \
   ((INSTANCE) == TIM17))

#define IS_TIM_CC1_INSTANCE(INSTANCE)\
  (((INSTANCE) == TIM1)    || \
   ((INSTANCE) == TIM2)    || \
   ((INSTANCE) == TIM3)    || \
   ((INSTANCE) == TIM14)   || \
   ((INSTANCE) == TIM15)   || \
   ((INSTANCE) == TIM16)   || \
   ((INSTANCE) == TIM17))

#define IS_TIM_CC2_INSTANCE(INSTANCE)\
  (((INSTANCE) == TIM1)    || \
   ((INSTANCE) == TIM2)    || \
   ((INSTANCE) == TIM3)    || \
   ((INSTANCE) == TIM15))

#define IS_TIM_CC3_INSTANCE(INSTANCE)\
  (((INSTANCE) == TIM1)    || \
   ((INSTANCE) == TIM2)    || \
   ((INSTANCE) == TIM3))

#define IS_TIM_CC4_INSTANCE(INSTANCE)\
  (((INSTANCE) == TIM1)    || \
   ((INSTANCE) == TIM2)    || \
   ((INSTANCE) == TIM3))

#define IS_TIM_CLOCKSOURCE_ETRMODE1_INSTANCE(INSTANCE)\
  (((INSTANCE) == TIM1)    || \
   ((INSTANCE) == TIM2)    || \
   ((INSTANCE) == TIM3))

#define IS_TIM_CLOCKSOURCE_ETRMODE2_INSTANCE(INSTANCE)\
  (((INSTANCE) == TIM1)    || \
   ((INSTANCE) == TIM2)    || \
   ((INSTANCE) == TIM3))

#define IS_TIM_CLOCKSOURCE_TIX_INSTANCE(INSTANCE)\
  (((INSTANCE) == TIM1)    || \
   ((INSTANCE) == TIM2)    || \
   ((INSTANCE) == TIM3)    || \
   ((INSTANCE) == TIM15))

#define IS_TIM_CLOCKSOURCE_ITRX_INSTANCE(INSTANCE)\
  (((INSTANCE) == TIM1)    || \
   ((INSTANCE) == TIM2)    || \
   ((INSTANCE) == TIM3)    || \
   ((INSTANCE) == TIM15))

#define IS_TIM_OCXREF_CLEAR_INSTANCE(INSTANCE)\
  (((INSTANCE) == TIM1)    || \
   ((INSTANCE) == TIM2)    || \
   ((INSTANCE) == TIM3))

#define IS_TIM_ENCODER_INTERFACE_INSTANCE(INSTANCE)\
  (((INSTANCE) == TIM1)    || \
   ((INSTANCE) == TIM2)    || \
   ((INSTANCE) == TIM3))

#define IS_TIM_HALL_INTERFACE_INSTANCE(INSTANCE)\
  (((INSTANCE) == TIM1))

#define IS_TIM_XOR_INSTANCE(INSTANCE)\
  (((INSTANCE) == TIM1)    || \
   ((INSTANCE) == TIM2)    || \
   ((INSTANCE) == TIM3))

#define IS_TIM_MASTER_INSTANCE(INSTANCE)\
  (((INSTANCE) == TIM1)    || \
   ((INSTANCE) == TIM2)    || \
   ((INSTANCE) == TIM3)    || \
   ((INSTANCE) == TIM6)    || \
   ((INSTANCE) == TIM15))

#define IS_TIM_SLAVE_INSTANCE(INSTANCE)\
  (((INSTANCE) == TIM1)    || \
   ((INSTANCE) == TIM2)    || \
   ((INSTANCE) == TIM3)    || \
   ((INSTANCE) == TIM15))

#define IS_TIM_32B_COUNTER_INSTANCE(INSTANCE)\
    ((INSTANCE) == TIM2)

#define IS_TIM_DMABURST_INSTANCE(INSTANCE)\
    (((INSTANCE) == TIM1)    || \
     ((INSTANCE) == TIM2)    || \
     ((INSTANCE) == TIM3)    || \
     ((INSTANCE) == TIM15)   || \
     ((INSTANCE) == TIM16)   || \
     ((INSTANCE) == TIM17))

#define IS_TIM_BREAK_INSTANCE(INSTANCE)\
      (((INSTANCE) == TIM1)    || \
       ((INSTANCE) == TIM15)   || \
       ((INSTANCE) == TIM16)   || \
       ((INSTANCE) == TIM17))

#define IS_TIM_CCX_INSTANCE(INSTANCE, CHANNEL) \
    ((((INSTANCE) == TIM1) &&                   \
     (((CHANNEL) == TIM_CHANNEL_1) ||          \
      ((CHANNEL) == TIM_CHANNEL_2) ||          \
      ((CHANNEL) == TIM_CHANNEL_3) ||          \
      ((CHANNEL) == TIM_CHANNEL_4)))           \
    ||                                         \
    (((INSTANCE) == TIM2) &&                   \
     (((CHANNEL) == TIM_CHANNEL_1) ||          \
      ((CHANNEL) == TIM_CHANNEL_2) ||          \
      ((CHANNEL) == TIM_CHANNEL_3) ||          \
      ((CHANNEL) == TIM_CHANNEL_4)))           \
    ||                                         \
    (((INSTANCE) == TIM3) &&                   \
     (((CHANNEL) == TIM_CHANNEL_1) ||          \
      ((CHANNEL) == TIM_CHANNEL_2) ||          \
      ((CHANNEL) == TIM_CHANNEL_3) ||          \
      ((CHANNEL) == TIM_CHANNEL_4)))           \
    ||                                         \
    (((INSTANCE) == TIM14) &&                  \
     (((CHANNEL) == TIM_CHANNEL_1)))           \
    ||                                         \
    (((INSTANCE) == TIM15) &&                  \
     (((CHANNEL) == TIM_CHANNEL_1) ||          \
      ((CHANNEL) == TIM_CHANNEL_2)))           \
    ||                                         \
    (((INSTANCE) == TIM16) &&                  \
     (((CHANNEL) == TIM_CHANNEL_1)))           \
    ||                                         \
    (((INSTANCE) == TIM17) &&                  \
     (((CHANNEL) == TIM_CHANNEL_1))))

#define IS_TIM_CCXN_INSTANCE(INSTANCE, CHANNEL) \
   ((((INSTANCE) == TIM1) &&                    \
     (((CHANNEL) == TIM_CHANNEL_1) ||           \
      ((CHANNEL) == TIM_CHANNEL_2) ||           \
      ((CHANNEL) == TIM_CHANNEL_3)))            \
    ||                                          \
    (((INSTANCE) == TIM15) &&                   \
      ((CHANNEL) == TIM_CHANNEL_1))             \
    ||                                          \
    (((INSTANCE) == TIM16) &&                   \
     ((CHANNEL) == TIM_CHANNEL_1))              \
    ||                                          \
    (((INSTANCE) == TIM17) &&                   \
     ((CHANNEL) == TIM_CHANNEL_1)))

#define IS_TIM_COUNTER_MODE_SELECT_INSTANCE(INSTANCE)\
  (((INSTANCE) == TIM1)    || \
   ((INSTANCE) == TIM2)    || \
   ((INSTANCE) == TIM3))

#define IS_TIM_REPETITION_COUNTER_INSTANCE(INSTANCE)\
  (((INSTANCE) == TIM1)    || \
   ((INSTANCE) == TIM15)   || \
   ((INSTANCE) == TIM16)   || \
   ((INSTANCE) == TIM17))

#define IS_TIM_CLOCK_DIVISION_INSTANCE(INSTANCE)\
  (((INSTANCE) == TIM1)    || \
   ((INSTANCE) == TIM2)    || \
   ((INSTANCE) == TIM3)    || \
   ((INSTANCE) == TIM14)   || \
   ((INSTANCE) == TIM15)   || \
   ((INSTANCE) == TIM16)   || \
   ((INSTANCE) == TIM17))

#define IS_TIM_DMA_INSTANCE(INSTANCE)\
  (((INSTANCE) == TIM1)    || \
   ((INSTANCE) == TIM2)    || \
   ((INSTANCE) == TIM3)    || \
   ((INSTANCE) == TIM6)    || \
   ((INSTANCE) == TIM15)   || \
   ((INSTANCE) == TIM16)   || \
   ((INSTANCE) == TIM17))
    
#define IS_TIM_DMA_CC_INSTANCE(INSTANCE)\
  (((INSTANCE) == TIM1)    || \
   ((INSTANCE) == TIM2)    || \
   ((INSTANCE) == TIM3)    || \
   ((INSTANCE) == TIM15)   || \
   ((INSTANCE) == TIM16)   || \
   ((INSTANCE) == TIM17))
    
#define IS_TIM_COMMUTATION_EVENT_INSTANCE(INSTANCE)\
  (((INSTANCE) == TIM1)    || \
   ((INSTANCE) == TIM15)   || \
   ((INSTANCE) == TIM16)   || \
   ((INSTANCE) == TIM17))

#define IS_TIM_REMAP_INSTANCE(INSTANCE)\
  ((INSTANCE) == TIM14)

/****************************** TSC Instances *********************************/
#define IS_TSC_ALL_INSTANCE(INSTANCE) ((INSTANCE) == TSC)

/*********************** UART Instances : IRDA mode ***************************/
#define IS_IRDA_INSTANCE(INSTANCE) ((INSTANCE) == USART1)

/********************* UART Instances : Smard card mode ***********************/
#define IS_SMARTCARD_INSTANCE(INSTANCE) ((INSTANCE) == USART1)

/******************** USART Instances : Synchronous mode **********************/
#define IS_USART_INSTANCE(INSTANCE) (((INSTANCE) == USART1) || \
                                     ((INSTANCE) == USART2))
                                     
/******************** USART Instances : auto Baud rate detection **************/                                     
#define IS_USART_AUTOBAUDRATE_DETECTION_INSTANCE(INSTANCE) ((INSTANCE) == USART1)
                                                                                              
/******************** UART Instances : Asynchronous mode **********************/
#define IS_UART_INSTANCE(INSTANCE)   (((INSTANCE) == USART1) || \
                                      ((INSTANCE) == USART2))

/******************** UART Instances : Half-Duplex mode **********************/
#define IS_UART_HALFDUPLEX_INSTANCE(INSTANCE)   (((INSTANCE) == USART1) || \
                                                 ((INSTANCE) == USART2))

/****************** UART Instances : Hardware Flow control ********************/
#define IS_UART_HWFLOW_INSTANCE(INSTANCE) (((INSTANCE) == USART1) || \
                                           ((INSTANCE) == USART2))

/****************** UART Instances : LIN mode ********************/
#define IS_UART_LIN_INSTANCE(INSTANCE) ((INSTANCE) == USART1)

/****************** UART Instances : wakeup from stop mode ********************/
#define IS_UART_WAKEUP_INSTANCE(INSTANCE) ((INSTANCE) == USART1)

/****************** UART Instances : Driver enable detection ********************/
#define IS_UART_DRIVER_ENABLE_INSTANCE(INSTANCE) (((INSTANCE) == USART1) || \
                                                  ((INSTANCE) == USART2))

/****************************** WWDG Instances ********************************/
#define IS_WWDG_ALL_INSTANCE(INSTANCE)  ((INSTANCE) == WWDG)

/**
  * @}
  */


/******************************************************************************/
/*  For a painless codes migration between the STM32F0xx device product       */
/*  lines, the aliases defined below are put in place to overcome the         */
/*  differences in the interrupt handlers and IRQn definitions.               */
/*  No need to update developed interrupt code when moving across             */
/*  product lines within the same STM32F0 Family                              */
/******************************************************************************/

/* Aliases for __IRQn */
#define ADC1_IRQn                  ADC1_COMP_IRQn
#define DMA1_Ch1_IRQn              DMA1_Channel1_IRQn
#define DMA1_Ch2_3_DMA2_Ch1_2_IRQn DMA1_Channel2_3_IRQn
#define DMA1_Channel4_5_6_7_IRQn   DMA1_Channel4_5_IRQn
#define DMA1_Ch4_7_DMA2_Ch3_5_IRQn DMA1_Channel4_5_IRQn
#define VDDIO2_IRQn                PVD_IRQn
#define PVD_VDDIO2_IRQn            PVD_IRQn
#define RCC_CRS_IRQn               RCC_IRQn
#define TIM6_IRQn                  TIM6_DAC_IRQn


/* Aliases for __IRQHandler */
#define ADC1_IRQHandler                  ADC1_COMP_IRQHandler
#define DMA1_Ch1_IRQHandler              DMA1_Channel1_IRQHandler
#define DMA1_Ch2_3_DMA2_Ch1_2_IRQHandler DMA1_Channel2_3_IRQHandler
#define DMA1_Channel4_5_6_7_IRQHandler   DMA1_Channel4_5_IRQHandler
#define DMA1_Ch4_7_DMA2_Ch3_5_IRQHandler DMA1_Channel4_5_IRQHandler
#define VDDIO2_IRQHandler                PVD_IRQHandler
#define PVD_VDDIO2_IRQHandler            PVD_IRQHandler
#define RCC_CRS_IRQHandler               RCC_IRQHandler
#define TIM6_IRQHandler                  TIM6_DAC_IRQHandler


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STM32F051x8_H */

/**
  * @}
  */

  /**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
