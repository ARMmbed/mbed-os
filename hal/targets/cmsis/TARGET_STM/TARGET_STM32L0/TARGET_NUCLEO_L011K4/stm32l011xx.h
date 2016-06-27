/**
  ******************************************************************************
  * @file    stm32l011xx.h
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    8-January-2016
  * @brief   CMSIS Cortex-M0+ Device Peripheral Access Layer Header File. 
  *          This file contains all the peripheral register's definitions, bits 
  *          definitions and memory mapping for stm32l011xx devices.  
  *          
  *          This file contains:
  *           - Data structures and the address mapping for all peripherals
  *           - Peripheral's registers declarations and bits definition
  *           - Macros to access peripheral's registers hardware
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

/** @addtogroup stm32l011xx
  * @{
  */
    
#ifndef __STM32L011xx_H
#define __STM32L011xx_H

#ifdef __cplusplus
 extern "C" {
#endif 
  

/** @addtogroup Configuration_section_for_CMSIS
  * @{
  */
/**
  * @brief Configuration of the Cortex-M0+ Processor and Core Peripherals 
  */
#define __CM0PLUS_REV             0 /*!< Core Revision r0p0                            */
#define __MPU_PRESENT             0 /*!< STM32L0xx  provides no MPU                    */
#define __VTOR_PRESENT            1 /*!< Vector  Table  Register supported             */
#define __NVIC_PRIO_BITS          2 /*!< STM32L0xx uses 2 Bits for the Priority Levels */
#define __Vendor_SysTickConfig    0 /*!< Set to 1 if different SysTick Config is used  */

/**
  * @}
  */
   
/** @addtogroup Peripheral_interrupt_number_definition
  * @{
  */
   
/**
 * @brief stm32l011xx Interrupt Number Definition, according to the selected device 
 *        in @ref Library_configuration_section 
 */

/*!< Interrupt Number Definition */
typedef enum
{
/******  Cortex-M0 Processor Exceptions Numbers ******************************************************/
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                                */
  HardFault_IRQn              = -13,    /*!< 3 Cortex-M0+ Hard Fault Interrupt                       */
  SVC_IRQn                    = -5,     /*!< 11 Cortex-M0+ SV Call Interrupt                         */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M0+ Pend SV Interrupt                         */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M0+ System Tick Interrupt                     */

/******  STM32L-0 specific Interrupt Numbers *********************************************************/
  WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                               */
  PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detect Interrupt                  */
  RTC_IRQn                    = 2,      /*!< RTC through EXTI Line Interrupt                         */
  FLASH_IRQn                  = 3,      /*!< FLASH Interrupt                                         */
  RCC_IRQn                    = 4,      /*!< RCC Interrupt                                           */
  EXTI0_1_IRQn                = 5,      /*!< EXTI Line 0 and 1 Interrupts                            */
  EXTI2_3_IRQn                = 6,      /*!< EXTI Line 2 and 3 Interrupts                            */
  EXTI4_15_IRQn               = 7,      /*!< EXTI Line 4 to 15 Interrupts                            */
  DMA1_Channel1_IRQn          = 9,      /*!< DMA1 Channel 1 Interrupt                                */
  DMA1_Channel2_3_IRQn        = 10,     /*!< DMA1 Channel 2 and Channel 3 Interrupts                 */
  DMA1_Channel4_5_6_7_IRQn    = 11,     /*!< DMA1 Channel 4, Channel 5, Channel 6 and Channel 7 Interrupts */
  ADC1_COMP_IRQn              = 12,     /*!< ADC1, COMP1 and COMP2 Interrupts                        */
  LPTIM1_IRQn                 = 13,     /*!< LPTIM1 Interrupt                                        */
  TIM2_IRQn                   = 15,     /*!< TIM2 Interrupt                                          */
  TIM21_IRQn                  = 20,     /*!< TIM21 Interrupt                                         */
  I2C1_IRQn                   = 23,     /*!< I2C1 Interrupt                                          */
  SPI1_IRQn                   = 25,     /*!< SPI1 Interrupt                                          */
  USART2_IRQn                 = 28,     /*!< USART2 Interrupt                                        */
  LPUART1_IRQn                = 29,     /*!< LPUART1 Interrupt                                       */
} IRQn_Type;

/**
  * @}
  */

#include "core_cm0plus.h"
#include "system_stm32l0xx.h"
#include <stdint.h>

/** @addtogroup Peripheral_registers_structures
  * @{
  */   

/** 
  * @brief Analog to Digital Converter  
  */

typedef struct
{
  __IO uint32_t ISR;          /*!< ADC Interrupt and Status register,                          Address offset:0x00 */
  __IO uint32_t IER;          /*!< ADC Interrupt Enable register,                              Address offset:0x04 */
  __IO uint32_t CR;           /*!< ADC Control register,                                       Address offset:0x08 */
  __IO uint32_t CFGR1;        /*!< ADC Configuration register 1,                               Address offset:0x0C */
  __IO uint32_t CFGR2;        /*!< ADC Configuration register 2,                               Address offset:0x10 */
  __IO uint32_t SMPR;         /*!< ADC Sampling time register,                                 Address offset:0x14 */
  uint32_t   RESERVED1;       /*!< Reserved,                                                                  0x18 */
  uint32_t   RESERVED2;       /*!< Reserved,                                                                  0x1C */
  __IO uint32_t TR;           /*!< ADC watchdog threshold register,                            Address offset:0x20 */
  uint32_t   RESERVED3;       /*!< Reserved,                                                                  0x24 */
  __IO uint32_t CHSELR;       /*!< ADC channel selection register,                             Address offset:0x28 */
  uint32_t   RESERVED4[5];    /*!< Reserved,                                                                  0x2C */
  __IO uint32_t DR;           /*!< ADC data register,                                          Address offset:0x40 */
  uint32_t   RESERVED5[28];   /*!< Reserved,                                                           0x44 - 0xB0 */
  __IO uint32_t CALFACT;      /*!< ADC data register,                                          Address offset:0xB4 */
} ADC_TypeDef;

typedef struct
{
  __IO uint32_t CCR;
} ADC_Common_TypeDef;


/**
  * @brief Comparator 
  */

typedef struct
{
  __IO uint32_t CSR;     /*!< COMP comparator control and status register, Address offset: 0x18 */
} COMP_TypeDef;

typedef struct
{
  __IO uint32_t CSR;         /*!< COMP control and status register, used for bits common to several COMP instances, Address offset: 0x00 */
} COMP_Common_TypeDef;


/**
* @brief CRC calculation unit
*/

typedef struct
{
__IO uint32_t DR;            /*!< CRC Data register,                            Address offset: 0x00 */
__IO uint8_t IDR;            /*!< CRC Independent data register,                Address offset: 0x04 */
uint8_t RESERVED0;           /*!< Reserved,                                                     0x05 */
uint16_t RESERVED1;          /*!< Reserved,                                                     0x06 */
__IO uint32_t CR;            /*!< CRC Control register,                         Address offset: 0x08 */
uint32_t RESERVED2;          /*!< Reserved,                                                     0x0C */
__IO uint32_t INIT;          /*!< Initial CRC value register,                   Address offset: 0x10 */
__IO uint32_t POL;           /*!< CRC polynomial register,                      Address offset: 0x14 */
} CRC_TypeDef;

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
  __IO uint32_t CCR;          /*!< DMA channel x configuration register */
  __IO uint32_t CNDTR;        /*!< DMA channel x number of data register */
  __IO uint32_t CPAR;         /*!< DMA channel x peripheral address register */
  __IO uint32_t CMAR;         /*!< DMA channel x memory address register */
} DMA_Channel_TypeDef;

typedef struct
{
  __IO uint32_t ISR;          /*!< DMA interrupt status register,               Address offset: 0x00 */
  __IO uint32_t IFCR;         /*!< DMA interrupt flag clear register,           Address offset: 0x04 */
} DMA_TypeDef;                                                                  
                                                                                
typedef struct                                                                  
{                                                                               
  __IO uint32_t CSELR;        /*!< DMA channel selection register,              Address offset: 0xA8 */
} DMA_Request_TypeDef;                                                          
                                                                                
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
}EXTI_TypeDef;

/** 
  * @brief FLASH Registers
  */
typedef struct
{
  __IO uint32_t ACR;           /*!< Access control register,                     Address offset: 0x00 */
  __IO uint32_t PECR;          /*!< Program/erase control register,              Address offset: 0x04 */
  __IO uint32_t PDKEYR;        /*!< Power down key register,                     Address offset: 0x08 */
  __IO uint32_t PEKEYR;        /*!< Program/erase key register,                  Address offset: 0x0c */
  __IO uint32_t PRGKEYR;       /*!< Program memory key register,                 Address offset: 0x10 */
  __IO uint32_t OPTKEYR;       /*!< Option byte key register,                    Address offset: 0x14 */
  __IO uint32_t SR;            /*!< Status register,                             Address offset: 0x18 */
  __IO uint32_t OPTR;          /*!< Option byte register,                        Address offset: 0x1c */
  __IO uint32_t WRPR;          /*!< Write protection register,                   Address offset: 0x20 */
} FLASH_TypeDef;


/** 
  * @brief Option Bytes Registers
  */
typedef struct
{
  __IO uint32_t RDP;               /*!< Read protection register,               Address offset: 0x00 */
  __IO uint32_t USER;              /*!< user register,                          Address offset: 0x04 */
  __IO uint32_t WRP01;             /*!< write protection Bytes 0 and 1          Address offset: 0x08 */
} OB_TypeDef;
  

/** 
  * @brief General Purpose IO
  */

typedef struct
{
  __IO uint32_t MODER;        /*!< GPIO port mode register,                     Address offset: 0x00 */
  __IO uint32_t OTYPER;       /*!< GPIO port output type register,              Address offset: 0x04 */
  __IO uint32_t OSPEEDR;      /*!< GPIO port output speed register,             Address offset: 0x08 */
  __IO uint32_t PUPDR;        /*!< GPIO port pull-up/pull-down register,        Address offset: 0x0C */
  __IO uint32_t IDR;          /*!< GPIO port input data register,               Address offset: 0x10 */
  __IO uint32_t ODR;          /*!< GPIO port output data register,              Address offset: 0x14 */
  __IO uint32_t BSRR;         /*!< GPIO port bit set/reset registerBSRR,        Address offset: 0x18 */
  __IO uint32_t LCKR;         /*!< GPIO port configuration lock register,       Address offset: 0x1C */
  __IO uint32_t AFR[2];       /*!< GPIO alternate function register,            Address offset: 0x20-0x24 */
  __IO uint32_t BRR;          /*!< GPIO bit reset register,                     Address offset: 0x28 */
}GPIO_TypeDef;

/** 
  * @brief LPTIMIMER
  */
typedef struct
{
  __IO uint32_t ISR;      /*!< LPTIM Interrupt and Status register,             Address offset: 0x00 */
  __IO uint32_t ICR;      /*!< LPTIM Interrupt Clear register,                  Address offset: 0x04 */
  __IO uint32_t IER;      /*!< LPTIM Interrupt Enable register,                 Address offset: 0x08 */
  __IO uint32_t CFGR;     /*!< LPTIM Configuration register,                    Address offset: 0x0C */
  __IO uint32_t CR;       /*!< LPTIM Control register,                          Address offset: 0x10 */
  __IO uint32_t CMP;      /*!< LPTIM Compare register,                          Address offset: 0x14 */
  __IO uint32_t ARR;      /*!< LPTIM Autoreload register,                       Address offset: 0x18 */
  __IO uint32_t CNT;      /*!< LPTIM Counter register,                          Address offset: 0x1C */
} LPTIM_TypeDef;

/** 
  * @brief SysTem Configuration
  */

typedef struct
{
  __IO uint32_t CFGR1;         /*!< SYSCFG configuration register 1,                    Address offset: 0x00 */
  __IO uint32_t CFGR2;         /*!< SYSCFG configuration register 2,                    Address offset: 0x04 */
  __IO uint32_t EXTICR[4];     /*!< SYSCFG external interrupt configuration register,   Address offset: 0x14-0x08 */
       uint32_t RESERVED[2];   /*!< Reserved,                                           0x18-0x1C */
  __IO uint32_t CFGR3;         /*!< SYSCFG configuration register 3,                    Address offset: 0x20 */       
} SYSCFG_TypeDef;



/** 
  * @brief Inter-integrated Circuit Interface
  */

typedef struct
{
  __IO uint32_t CR1;      /*!< I2C Control register 1,            Address offset: 0x00 */
  __IO uint32_t CR2;      /*!< I2C Control register 2,            Address offset: 0x04 */
  __IO uint32_t OAR1;     /*!< I2C Own address 1 register,        Address offset: 0x08 */
  __IO uint32_t OAR2;     /*!< I2C Own address 2 register,        Address offset: 0x0C */
  __IO uint32_t TIMINGR;  /*!< I2C Timing register,               Address offset: 0x10 */
  __IO uint32_t TIMEOUTR; /*!< I2C Timeout register,              Address offset: 0x14 */
  __IO uint32_t ISR;      /*!< I2C Interrupt and status register, Address offset: 0x18 */
  __IO uint32_t ICR;      /*!< I2C Interrupt clear register,      Address offset: 0x1C */
  __IO uint32_t PECR;     /*!< I2C PEC register,                  Address offset: 0x20 */
  __IO uint32_t RXDR;     /*!< I2C Receive data register,         Address offset: 0x24 */
  __IO uint32_t TXDR;     /*!< I2C Transmit data register,        Address offset: 0x28 */
}I2C_TypeDef;


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
  __IO uint32_t CR;   /*!< PWR power control register,        Address offset: 0x00 */
  __IO uint32_t CSR;  /*!< PWR power control/status register, Address offset: 0x04 */
} PWR_TypeDef;

/** 
  * @brief Reset and Clock Control
  */
typedef struct
{
  __IO uint32_t CR;            /*!< RCC clock control register,                                   Address offset: 0x00 */
  __IO uint32_t ICSCR;         /*!< RCC Internal clock sources calibration register,              Address offset: 0x04 */
  __IO uint32_t CRRCR;         /*!< RCC Clock recovery RC register,                               Address offset: 0x08 */
  __IO uint32_t CFGR;          /*!< RCC Clock configuration register,                             Address offset: 0x0C */
  __IO uint32_t CIER;          /*!< RCC Clock interrupt enable register,                          Address offset: 0x10 */
  __IO uint32_t CIFR;          /*!< RCC Clock interrupt flag register,                            Address offset: 0x14 */
  __IO uint32_t CICR;          /*!< RCC Clock interrupt clear register,                           Address offset: 0x18 */
  __IO uint32_t IOPRSTR;       /*!< RCC IO port reset register,                                   Address offset: 0x1C */
  __IO uint32_t AHBRSTR;       /*!< RCC AHB peripheral reset register,                            Address offset: 0x20 */
  __IO uint32_t APB2RSTR;      /*!< RCC APB2 peripheral reset register,                           Address offset: 0x24 */
  __IO uint32_t APB1RSTR;      /*!< RCC APB1 peripheral reset register,                           Address offset: 0x28 */
  __IO uint32_t IOPENR;        /*!< RCC Clock IO port enable register,                            Address offset: 0x2C */
  __IO uint32_t AHBENR;        /*!< RCC AHB peripheral clock enable register,                     Address offset: 0x30 */
  __IO uint32_t APB2ENR;       /*!< RCC APB2 peripheral enable register,                          Address offset: 0x34 */
  __IO uint32_t APB1ENR;       /*!< RCC APB1 peripheral enable register,                          Address offset: 0x38 */
  __IO uint32_t IOPSMENR;      /*!< RCC IO port clock enable in sleep mode register,              Address offset: 0x3C */
  __IO uint32_t AHBSMENR;      /*!< RCC AHB peripheral clock enable in sleep mode register,       Address offset: 0x40 */
  __IO uint32_t APB2SMENR;     /*!< RCC APB2 peripheral clock enable in sleep mode register,      Address offset: 0x44 */
  __IO uint32_t APB1SMENR;     /*!< RCC APB1 peripheral clock enable in sleep mode register,      Address offset: 0x48 */
  __IO uint32_t CCIPR;         /*!< RCC clock configuration register,                             Address offset: 0x4C */
  __IO uint32_t CSR;           /*!< RCC Control/status register,                                  Address offset: 0x50 */
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
  __IO uint32_t WUTR;       /*!< RTC wakeup timer register,                                 Address offset: 0x14 */
       uint32_t RESERVED;   /*!< Reserved,                                                  Address offset: 0x18 */
  __IO uint32_t ALRMAR;     /*!< RTC alarm A register,                                      Address offset: 0x1C */
  __IO uint32_t ALRMBR;     /*!< RTC alarm B register,                                      Address offset: 0x20 */
  __IO uint32_t WPR;        /*!< RTC write protection register,                             Address offset: 0x24 */
  __IO uint32_t SSR;        /*!< RTC sub second register,                                   Address offset: 0x28 */
  __IO uint32_t SHIFTR;     /*!< RTC shift control register,                                Address offset: 0x2C */
  __IO uint32_t TSTR;       /*!< RTC time stamp time register,                              Address offset: 0x30 */
  __IO uint32_t TSDR;       /*!< RTC time stamp date register,                              Address offset: 0x34 */
  __IO uint32_t TSSSR;      /*!< RTC time-stamp sub second register,                        Address offset: 0x38 */
  __IO uint32_t CALR;       /*!< RTC calibration register,                                  Address offset: 0x3C */
  __IO uint32_t TAMPCR;     /*!< RTC tamper configuration register,                         Address offset: 0x40 */
  __IO uint32_t ALRMASSR;   /*!< RTC alarm A sub second register,                           Address offset: 0x44 */
  __IO uint32_t ALRMBSSR;   /*!< RTC alarm B sub second register,                           Address offset: 0x48 */
  __IO uint32_t OR;         /*!< RTC option register,                                       Address offset  0x4C */
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
  __IO uint32_t CR1;      /*!< SPI Control register 1,                              Address offset: 0x00 */
  __IO uint32_t CR2;      /*!< SPI Control register 2,                              Address offset: 0x04 */
  __IO uint32_t SR;       /*!< SPI Status register,                                 Address offset: 0x08 */
  __IO uint32_t DR;       /*!< SPI data register,                                   Address offset: 0x0C */
  __IO uint32_t CRCPR;    /*!< SPI CRC polynomial register,                         Address offset: 0x10 */
  __IO uint32_t RXCRCR;   /*!< SPI Rx CRC register,                                 Address offset: 0x14 */
  __IO uint32_t TXCRCR;   /*!< SPI Tx CRC register,                                 Address offset: 0x18 */
} SPI_TypeDef;

/** 
  * @brief TIM
  */
typedef struct
{
  __IO uint32_t CR1;       /*!< TIM control register 1,                       Address offset: 0x00 */
  __IO uint32_t CR2;       /*!< TIM control register 2,                       Address offset: 0x04 */
  __IO uint32_t SMCR;      /*!< TIM slave Mode Control register,              Address offset: 0x08 */
  __IO uint32_t DIER;      /*!< TIM DMA/interrupt enable register,            Address offset: 0x0C */
  __IO uint32_t SR;        /*!< TIM status register,                          Address offset: 0x10 */
  __IO uint32_t EGR;       /*!< TIM event generation register,                Address offset: 0x14 */
  __IO uint32_t CCMR1;     /*!< TIM  capture/compare mode register 1,         Address offset: 0x18 */
  __IO uint32_t CCMR2;     /*!< TIM  capture/compare mode register 2,         Address offset: 0x1C */
  __IO uint32_t CCER;      /*!< TIM capture/compare enable register,          Address offset: 0x20 */
  __IO uint32_t CNT;       /*!< TIM counter register,                         Address offset: 0x24 */
  __IO uint32_t PSC;       /*!< TIM prescaler register,                       Address offset: 0x28 */
  __IO uint32_t ARR;       /*!< TIM auto-reload register,                     Address offset: 0x2C */
  uint32_t      RESERVED12;/*!< Reserved                                      Address offset: 0x30 */
  __IO uint32_t CCR1;      /*!< TIM capture/compare register 1,               Address offset: 0x34 */
  __IO uint32_t CCR2;      /*!< TIM capture/compare register 2,               Address offset: 0x38 */
  __IO uint32_t CCR3;      /*!< TIM capture/compare register 3,               Address offset: 0x3C */
  __IO uint32_t CCR4;      /*!< TIM capture/compare register 4,               Address offset: 0x40 */
  uint32_t      RESERVED17;/*!< Reserved,                                     Address offset: 0x44 */
  __IO uint32_t DCR;       /*!< TIM DMA control register,                     Address offset: 0x48 */
  __IO uint32_t DMAR;      /*!< TIM DMA address for full transfer register,   Address offset: 0x4C */
  __IO uint32_t OR;        /*!< TIM option register,                          Address offset: 0x50 */
} TIM_TypeDef;

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
  __IO uint32_t RDR;    /*!< USART Receive Data register,              Address offset: 0x24 */
  __IO uint32_t TDR;    /*!< USART Transmit Data register,             Address offset: 0x28 */
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
#define FLASH_BASE             ((uint32_t)0x08000000U) /*!< FLASH base address in the alias region */
#define FLASH_END              ((uint32_t)0x08003FFFUU) /*!< FLASH end address in the alias region */
#define DATA_EEPROM_BASE       ((uint32_t)0x08080000U) /*!< DATA_EEPROM base address in the alias region */
#define DATA_EEPROM_END        ((uint32_t)0x080801FFU) /*!< DATA EEPROM end address in the alias region */
#define SRAM_BASE              ((uint32_t)0x20000000U) /*!< SRAM base address in the alias region */
#define PERIPH_BASE            ((uint32_t)0x40000000U) /*!< Peripheral base address in the alias region */

/*!< Peripheral memory map */
#define APBPERIPH_BASE        PERIPH_BASE
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x00020000)
#define IOPPERIPH_BASE        (PERIPH_BASE + 0x10000000)

#define TIM2_BASE             (APBPERIPH_BASE + 0x00000000)
#define RTC_BASE              (APBPERIPH_BASE + 0x00002800)
#define WWDG_BASE             (APBPERIPH_BASE + 0x00002C00)
#define IWDG_BASE             (APBPERIPH_BASE + 0x00003000)
#define USART2_BASE           (APBPERIPH_BASE + 0x00004400)
#define LPUART1_BASE          (APBPERIPH_BASE + 0x00004800)
#define I2C1_BASE             (APBPERIPH_BASE + 0x00005400)
#define PWR_BASE              (APBPERIPH_BASE + 0x00007000)
#define LPTIM1_BASE           (APBPERIPH_BASE + 0x00007C00)

#define SYSCFG_BASE           (APBPERIPH_BASE + 0x00010000)
#define COMP1_BASE            (APBPERIPH_BASE + 0x00010018)
#define COMP2_BASE            (APBPERIPH_BASE + 0x0001001C)
#define COMP12_COMMON       ((COMP_Common_TypeDef *) COMP1_BASE)
#define EXTI_BASE             (APBPERIPH_BASE + 0x00010400)
#define TIM21_BASE            (APBPERIPH_BASE + 0x00010800)
#define ADC1_BASE             (APBPERIPH_BASE + 0x00012400)
#define ADC_BASE              (APBPERIPH_BASE + 0x00012708)
#define SPI1_BASE             (APBPERIPH_BASE + 0x00013000)
#define DBGMCU_BASE           (APBPERIPH_BASE + 0x00015800)

#define DMA1_BASE             (AHBPERIPH_BASE + 0x00000000)
#define DMA1_Channel1_BASE    (DMA1_BASE + 0x00000008)
#define DMA1_Channel2_BASE    (DMA1_BASE + 0x0000001C)
#define DMA1_Channel3_BASE    (DMA1_BASE + 0x00000030)
#define DMA1_Channel4_BASE    (DMA1_BASE + 0x00000044)
#define DMA1_Channel5_BASE    (DMA1_BASE + 0x00000058)
#define DMA1_CSELR_BASE       (DMA1_BASE + 0x000000A8)


#define RCC_BASE              (AHBPERIPH_BASE + 0x00001000)
#define FLASH_R_BASE          (AHBPERIPH_BASE + 0x00002000) /*!< FLASH registers base address */
#define OB_BASE               ((uint32_t)0x1FF80000U)        /*!< FLASH Option Bytes base address */
#define CRC_BASE              (AHBPERIPH_BASE + 0x00003000)

#define GPIOA_BASE            (IOPPERIPH_BASE + 0x00000000)
#define GPIOB_BASE            (IOPPERIPH_BASE + 0x00000400)
#define GPIOC_BASE            (IOPPERIPH_BASE + 0x00000800)

/**
  * @}
  */
  
/** @addtogroup Peripheral_declaration
  * @{
  */  

#define TIM2                ((TIM_TypeDef *) TIM2_BASE)
#define RTC                 ((RTC_TypeDef *) RTC_BASE)
#define WWDG                ((WWDG_TypeDef *) WWDG_BASE)
#define IWDG                ((IWDG_TypeDef *) IWDG_BASE)
#define USART2              ((USART_TypeDef *) USART2_BASE)
#define LPUART1             ((USART_TypeDef *) LPUART1_BASE)
#define I2C1                ((I2C_TypeDef *) I2C1_BASE)
#define PWR                 ((PWR_TypeDef *) PWR_BASE)
#define LPTIM1              ((LPTIM_TypeDef *) LPTIM1_BASE)

#define SYSCFG              ((SYSCFG_TypeDef *) SYSCFG_BASE)
#define COMP1               ((COMP_TypeDef *) COMP1_BASE)
#define COMP2               ((COMP_TypeDef *) COMP2_BASE)
#define EXTI                ((EXTI_TypeDef *) EXTI_BASE)
#define TIM21               ((TIM_TypeDef *) TIM21_BASE)
#define ADC1                ((ADC_TypeDef *) ADC1_BASE)
#define ADC1_COMMON         ((ADC_Common_TypeDef *) ADC_BASE)
/* Legacy defines */
#define ADC                 ADC1_COMMON
#define SPI1                ((SPI_TypeDef *) SPI1_BASE)
#define DBGMCU              ((DBGMCU_TypeDef *) DBGMCU_BASE)

#define DMA1                ((DMA_TypeDef *) DMA1_BASE)
#define DMA1_Channel1       ((DMA_Channel_TypeDef *) DMA1_Channel1_BASE)
#define DMA1_Channel2       ((DMA_Channel_TypeDef *) DMA1_Channel2_BASE)
#define DMA1_Channel3       ((DMA_Channel_TypeDef *) DMA1_Channel3_BASE)
#define DMA1_Channel4       ((DMA_Channel_TypeDef *) DMA1_Channel4_BASE)
#define DMA1_Channel5       ((DMA_Channel_TypeDef *) DMA1_Channel5_BASE)
#define DMA1_CSELR          ((DMA_Request_TypeDef *) DMA1_CSELR_BASE)


#define FLASH               ((FLASH_TypeDef *) FLASH_R_BASE)
#define OB                  ((OB_TypeDef *) OB_BASE) 
#define RCC                 ((RCC_TypeDef *) RCC_BASE)
#define CRC                 ((CRC_TypeDef *) CRC_BASE)

#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)

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
/********************  Bits definition for ADC_ISR register  ******************/
#define ADC_ISR_EOCAL                       ((uint32_t)0x00000800U)     /*!< End of calibration flag */
#define ADC_ISR_AWD                         ((uint32_t)0x00000080U)     /*!< Analog watchdog flag */
#define ADC_ISR_OVR                         ((uint32_t)0x00000010U)     /*!< Overrun flag */
#define ADC_ISR_EOSEQ                       ((uint32_t)0x00000008U)     /*!< End of Sequence flag */
#define ADC_ISR_EOC                         ((uint32_t)0x00000004U)     /*!< End of Conversion */
#define ADC_ISR_EOSMP                       ((uint32_t)0x00000002U)     /*!< End of sampling flag */
#define ADC_ISR_ADRDY                       ((uint32_t)0x00000001U)     /*!< ADC Ready */

/* Old EOSEQ bit definition, maintained for legacy purpose */
#define ADC_ISR_EOS                          ADC_ISR_EOSEQ

/********************  Bits definition for ADC_IER register  ******************/
#define ADC_IER_EOCALIE                     ((uint32_t)0x00000800U)     /*!< Enf Of Calibration interrupt enable */
#define ADC_IER_AWDIE                       ((uint32_t)0x00000080U)     /*!< Analog Watchdog interrupt enable */
#define ADC_IER_OVRIE                       ((uint32_t)0x00000010U)     /*!< Overrun interrupt enable */
#define ADC_IER_EOSEQIE                     ((uint32_t)0x00000008U)     /*!< End of Sequence of conversion interrupt enable */
#define ADC_IER_EOCIE                       ((uint32_t)0x00000004U)     /*!< End of Conversion interrupt enable */
#define ADC_IER_EOSMPIE                     ((uint32_t)0x00000002U)     /*!< End of sampling interrupt enable */
#define ADC_IER_ADRDYIE                     ((uint32_t)0x00000001U)     /*!< ADC Ready interrupt enable */

/* Old EOSEQIE bit definition, maintained for legacy purpose */
#define ADC_IER_EOSIE                        ADC_IER_EOSEQIE

/********************  Bits definition for ADC_CR register  *******************/
#define ADC_CR_ADCAL                        ((uint32_t)0x80000000U)     /*!< ADC calibration */
#define ADC_CR_ADVREGEN                     ((uint32_t)0x10000000U)     /*!< ADC Voltage Regulator Enable */
#define ADC_CR_ADSTP                        ((uint32_t)0x00000010U)     /*!< ADC stop of conversion command */
#define ADC_CR_ADSTART                      ((uint32_t)0x00000004U)     /*!< ADC start of conversion */
#define ADC_CR_ADDIS                        ((uint32_t)0x00000002U)     /*!< ADC disable command */
#define ADC_CR_ADEN                         ((uint32_t)0x00000001U)     /*!< ADC enable control */ /*####   TBV  */

/*******************  Bits definition for ADC_CFGR1 register  *****************/
#define ADC_CFGR1_AWDCH                     ((uint32_t)0x7C000000U)     /*!< AWDCH[4:0] bits (Analog watchdog channel select bits) */
#define ADC_CFGR1_AWDCH_0                   ((uint32_t)0x04000000U)     /*!< Bit 0 */
#define ADC_CFGR1_AWDCH_1                   ((uint32_t)0x08000000U)     /*!< Bit 1 */
#define ADC_CFGR1_AWDCH_2                   ((uint32_t)0x10000000U)     /*!< Bit 2 */
#define ADC_CFGR1_AWDCH_3                   ((uint32_t)0x20000000U)     /*!< Bit 3 */
#define ADC_CFGR1_AWDCH_4                   ((uint32_t)0x40000000U)     /*!< Bit 4 */
#define ADC_CFGR1_AWDEN                     ((uint32_t)0x00800000U)     /*!< Analog watchdog enable on regular channels */
#define ADC_CFGR1_AWDSGL                    ((uint32_t)0x00400000U)     /*!< Enable the watchdog on a single channel or on all channels  */
#define ADC_CFGR1_DISCEN                    ((uint32_t)0x00010000U)     /*!< Discontinuous mode on regular channels */
#define ADC_CFGR1_AUTOFF                    ((uint32_t)0x00008000U)     /*!< ADC auto power off */
#define ADC_CFGR1_WAIT                      ((uint32_t)0x00004000U)     /*!< ADC wait conversion mode */
#define ADC_CFGR1_CONT                      ((uint32_t)0x00002000U)     /*!< Continuous Conversion */
#define ADC_CFGR1_OVRMOD                    ((uint32_t)0x00001000U)     /*!< Overrun mode */
#define ADC_CFGR1_EXTEN                     ((uint32_t)0x00000C00U)     /*!< EXTEN[1:0] bits (External Trigger Conversion mode for regular channels) */
#define ADC_CFGR1_EXTEN_0                   ((uint32_t)0x00000400U)     /*!< Bit 0 */
#define ADC_CFGR1_EXTEN_1                   ((uint32_t)0x00000800U)     /*!< Bit 1 */
#define ADC_CFGR1_EXTSEL                    ((uint32_t)0x000001C0U)     /*!< EXTSEL[2:0] bits (External Event Select for regular group) */
#define ADC_CFGR1_EXTSEL_0                  ((uint32_t)0x00000040U)     /*!< Bit 0 */
#define ADC_CFGR1_EXTSEL_1                  ((uint32_t)0x00000080U)     /*!< Bit 1 */
#define ADC_CFGR1_EXTSEL_2                  ((uint32_t)0x00000100U)     /*!< Bit 2 */
#define ADC_CFGR1_ALIGN                     ((uint32_t)0x00000020U)     /*!< Data Alignment */
#define ADC_CFGR1_RES                       ((uint32_t)0x00000018U)     /*!< RES[1:0] bits (Resolution) */
#define ADC_CFGR1_RES_0                     ((uint32_t)0x00000008U)     /*!< Bit 0 */
#define ADC_CFGR1_RES_1                     ((uint32_t)0x00000010U)     /*!< Bit 1 */
#define ADC_CFGR1_SCANDIR                   ((uint32_t)0x00000004U)     /*!< Sequence scan direction */
#define ADC_CFGR1_DMACFG                    ((uint32_t)0x00000002U)     /*!< Direct memory access configuration */
#define ADC_CFGR1_DMAEN                     ((uint32_t)0x00000001U)     /*!< Direct memory access enable */

/* Old WAIT bit definition, maintained for legacy purpose */
#define ADC_CFGR1_AUTDLY                    ADC_CFGR1_WAIT

/*******************  Bits definition for ADC_CFGR2 register  *****************/
#define ADC_CFGR2_TOVS                      ((uint32_t)0x80000200U)     /*!< Triggered Oversampling */
#define ADC_CFGR2_OVSS                      ((uint32_t)0x000001E0U)     /*!< OVSS [3:0] bits (Oversampling shift) */
#define ADC_CFGR2_OVSS_0                    ((uint32_t)0x00000020U)     /*!< Bit 0 */
#define ADC_CFGR2_OVSS_1                    ((uint32_t)0x00000040U)     /*!< Bit 1 */
#define ADC_CFGR2_OVSS_2                    ((uint32_t)0x00000080U)     /*!< Bit 2 */
#define ADC_CFGR2_OVSS_3                    ((uint32_t)0x00000100U)     /*!< Bit 3 */
#define ADC_CFGR2_OVSR                      ((uint32_t)0x0000001CU)     /*!< OVSR  [2:0] bits (Oversampling ratio) */
#define ADC_CFGR2_OVSR_0                    ((uint32_t)0x00000004U)     /*!< Bit 0 */
#define ADC_CFGR2_OVSR_1                    ((uint32_t)0x00000008U)     /*!< Bit 1 */
#define ADC_CFGR2_OVSR_2                    ((uint32_t)0x00000010U)     /*!< Bit 2 */
#define ADC_CFGR2_OVSE                      ((uint32_t)0x00000001U)     /*!< Oversampler Enable */
#define ADC_CFGR2_CKMODE                    ((uint32_t)0xC0000000U)     /*!< CKMODE [1:0] bits (ADC clock mode) */
#define ADC_CFGR2_CKMODE_0                  ((uint32_t)0x40000000U)     /*!< Bit 0 */
#define ADC_CFGR2_CKMODE_1                  ((uint32_t)0x80000000U)     /*!< Bit 1 */


/******************  Bit definition for ADC_SMPR register  ********************/
#define ADC_SMPR_SMP                        ((uint32_t)0x00000007U)     /*!< SMPR[2:0] bits (Sampling time selection) */
#define ADC_SMPR_SMP_0                      ((uint32_t)0x00000001U)     /*!< Bit 0 */
#define ADC_SMPR_SMP_1                      ((uint32_t)0x00000002U)     /*!< Bit 1 */
#define ADC_SMPR_SMP_2                      ((uint32_t)0x00000004U)     /*!< Bit 2 */

/* Legacy defines */
#define ADC_SMPR_SMPR                       ADC_SMPR_SMP
#define ADC_SMPR_SMPR_0                     ADC_SMPR_SMP_0
#define ADC_SMPR_SMPR_1                     ADC_SMPR_SMP_1
#define ADC_SMPR_SMPR_2                     ADC_SMPR_SMP_2

/*******************  Bit definition for ADC_TR register  ********************/
#define ADC_TR_HT                           ((uint32_t)0x0FFF0000U)     /*!< Analog watchdog high threshold */
#define ADC_TR_LT                           ((uint32_t)0x00000FFFU)     /*!< Analog watchdog low threshold */

/******************  Bit definition for ADC_CHSELR register  ******************/
#define ADC_CHSELR_CHSEL                    ((uint32_t)0x0007FFFFU)     /*!< ADC group regular sequencer channels */
#define ADC_CHSELR_CHSEL18                  ((uint32_t)0x00040000U)     /*!< Channel 18 selection */
#define ADC_CHSELR_CHSEL17                  ((uint32_t)0x00020000U)     /*!< Channel 17 selection */
#define ADC_CHSELR_CHSEL15                  ((uint32_t)0x00008000U)     /*!< Channel 15 selection */
#define ADC_CHSELR_CHSEL14                  ((uint32_t)0x00004000U)     /*!< Channel 14 selection */
#define ADC_CHSELR_CHSEL13                  ((uint32_t)0x00002000U)     /*!< Channel 13 selection */
#define ADC_CHSELR_CHSEL12                  ((uint32_t)0x00001000U)     /*!< Channel 12 selection */
#define ADC_CHSELR_CHSEL11                  ((uint32_t)0x00000800U)     /*!< Channel 11 selection */
#define ADC_CHSELR_CHSEL10                  ((uint32_t)0x00000400U)     /*!< Channel 10 selection */
#define ADC_CHSELR_CHSEL9                   ((uint32_t)0x00000200U)     /*!< Channel 9 selection */
#define ADC_CHSELR_CHSEL8                   ((uint32_t)0x00000100U)     /*!< Channel 8 selection */
#define ADC_CHSELR_CHSEL7                   ((uint32_t)0x00000080U)     /*!< Channel 7 selection */
#define ADC_CHSELR_CHSEL6                   ((uint32_t)0x00000040U)     /*!< Channel 6 selection */
#define ADC_CHSELR_CHSEL5                   ((uint32_t)0x00000020U)     /*!< Channel 5 selection */
#define ADC_CHSELR_CHSEL4                   ((uint32_t)0x00000010U)     /*!< Channel 4 selection */
#define ADC_CHSELR_CHSEL3                   ((uint32_t)0x00000008U)     /*!< Channel 3 selection */
#define ADC_CHSELR_CHSEL2                   ((uint32_t)0x00000004U)     /*!< Channel 2 selection */
#define ADC_CHSELR_CHSEL1                   ((uint32_t)0x00000002U)     /*!< Channel 1 selection */
#define ADC_CHSELR_CHSEL0                   ((uint32_t)0x00000001U)     /*!< Channel 0 selection */

/********************  Bit definition for ADC_DR register  ********************/
#define ADC_DR_DATA                         ((uint32_t)0x0000FFFFU)     /*!< Regular data */

/********************  Bit definition for ADC_CALFACT register  ********************/
#define ADC_CALFACT_CALFACT                 ((uint32_t)0x0000007FU)     /*!< Calibration factor */

/*******************  Bit definition for ADC_CCR register  ********************/
#define ADC_CCR_LFMEN                       ((uint32_t)0x02000000U)     /*!< Low Frequency Mode enable */
#define ADC_CCR_TSEN                        ((uint32_t)0x00800000U)     /*!< Temperature sensore enable */
#define ADC_CCR_VREFEN                      ((uint32_t)0x00400000U)     /*!< Vrefint enable */
#define ADC_CCR_PRESC                       ((uint32_t)0x003C0000U)     /*!< PRESC  [3:0] bits (ADC prescaler) */
#define ADC_CCR_PRESC_0                     ((uint32_t)0x00040000U)     /*!< Bit 0 */
#define ADC_CCR_PRESC_1                     ((uint32_t)0x00080000U)     /*!< Bit 1 */
#define ADC_CCR_PRESC_2                     ((uint32_t)0x00100000U)     /*!< Bit 2 */
#define ADC_CCR_PRESC_3                     ((uint32_t)0x00200000U)     /*!< Bit 3 */

/******************************************************************************/
/*                                                                            */
/*                      Analog Comparators (COMP)                             */
/*                                                                            */
/******************************************************************************/
/*************  Bit definition for COMP_CSR register (COMP1 and COMP2)  **************/
/* COMP1 bits definition */
#define COMP_CSR_COMP1EN                ((uint32_t)0x00000001U) /*!< COMP1 enable */
#define COMP_CSR_COMP1INNSEL            ((uint32_t)0x00000030U) /*!< COMP1 inverting input select */
#define COMP_CSR_COMP1INNSEL_0          ((uint32_t)0x00000010U) /*!< COMP1 inverting input select bit 0 */
#define COMP_CSR_COMP1INNSEL_1          ((uint32_t)0x00000020U) /*!< COMP1 inverting input select bit 1 */
#define COMP_CSR_COMP1WM                ((uint32_t)0x00000100U) /*!< Comparators window mode enable */
#define COMP_CSR_COMP1LPTIM1IN1         ((uint32_t)0x00001000U) /*!< COMP1 LPTIM1 IN1 connection */
#define COMP_CSR_COMP1POLARITY          ((uint32_t)0x00008000U) /*!< COMP1 output polarity */
#define COMP_CSR_COMP1VALUE             ((uint32_t)0x40000000U) /*!< COMP1 output level */
#define COMP_CSR_COMP1LOCK              ((uint32_t)0x80000000U) /*!< COMP1 lock */
/* COMP2 bits definition */
#define COMP_CSR_COMP2EN                ((uint32_t)0x00000001U) /*!< COMP2 enable */
#define COMP_CSR_COMP2SPEED             ((uint32_t)0x00000008U) /*!< COMP2 power mode */
#define COMP_CSR_COMP2INNSEL            ((uint32_t)0x00000070U) /*!< COMP2 inverting input select */
#define COMP_CSR_COMP2INNSEL_0          ((uint32_t)0x00000010U) /*!< COMP2 inverting input select bit 0 */
#define COMP_CSR_COMP2INNSEL_1          ((uint32_t)0x00000020U) /*!< COMP2 inverting input select bit 1 */
#define COMP_CSR_COMP2INNSEL_2          ((uint32_t)0x00000040U) /*!< COMP2 inverting input select bit 2 */
#define COMP_CSR_COMP2INPSEL            ((uint32_t)0x00000700U) /*!< COMPx non inverting input select */
#define COMP_CSR_COMP2INPSEL_0          ((uint32_t)0x00000100U) /*!< COMPx non inverting input select */
#define COMP_CSR_COMP2INPSEL_1          ((uint32_t)0x00000200U) /*!< COMPx non inverting input select */
#define COMP_CSR_COMP2INPSEL_2          ((uint32_t)0x00000400U) /*!< COMPx non inverting input select */
#define COMP_CSR_COMP2LPTIM1IN2         ((uint32_t)0x00001000U) /*!< COMP2 LPTIM1 IN2 connection */
#define COMP_CSR_COMP2LPTIM1IN1         ((uint32_t)0x00002000U) /*!< COMP2 LPTIM1 IN1 connection */
#define COMP_CSR_COMP2POLARITY          ((uint32_t)0x00008000U) /*!< COMP2 output polarity */
#define COMP_CSR_COMP2VALUE             ((uint32_t)0x40000000U) /*!< COMP2 output level */
#define COMP_CSR_COMP2LOCK              ((uint32_t)0x80000000U) /*!< COMP2 lock */

/**********************  Bit definition for COMP_CSR register common  ****************/
#define COMP_CSR_COMPxEN                ((uint32_t)0x00000001U) /*!< COMPx enable */
#define COMP_CSR_COMPxPOLARITY          ((uint32_t)0x00008000U) /*!< COMPx output polarity */
#define COMP_CSR_COMPxOUTVALUE          ((uint32_t)0x40000000U) /*!< COMPx output level */
#define COMP_CSR_COMPxLOCK              ((uint32_t)0x80000000U) /*!< COMPx lock */

/* Reference defines */
#define COMP_CSR_WINMODE   COMP_CSR_COMP1WM /*!< Bit intended to be used with COMP common instance (COMP_Common_TypeDef)  */

/******************************************************************************/
/*                                                                            */
/*                       CRC calculation unit (CRC)                           */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for CRC_DR register  *********************/
#define CRC_DR_DR                           ((uint32_t)0xFFFFFFFFU) /*!< Data register bits */

/*******************  Bit definition for CRC_IDR register  ********************/
#define CRC_IDR_IDR                         ((uint8_t)0xFFU) /*!< General-purpose 8-bit data register bits */

/********************  Bit definition for CRC_CR register  ********************/
#define CRC_CR_RESET                        ((uint32_t)0x00000001U) /*!< RESET the CRC computation unit bit */
#define CRC_CR_POLYSIZE                     ((uint32_t)0x00000018U) /*!< Polynomial size bits */
#define CRC_CR_POLYSIZE_0                   ((uint32_t)0x00000008U) /*!< Polynomial size bit 0 */
#define CRC_CR_POLYSIZE_1                   ((uint32_t)0x00000010U) /*!< Polynomial size bit 1 */
#define CRC_CR_REV_IN                       ((uint32_t)0x00000060U) /*!< REV_IN Reverse Input Data bits */
#define CRC_CR_REV_IN_0                     ((uint32_t)0x00000020U) /*!< Bit 0 */
#define CRC_CR_REV_IN_1                     ((uint32_t)0x00000040U) /*!< Bit 1 */
#define CRC_CR_REV_OUT                      ((uint32_t)0x00000080U) /*!< REV_OUT Reverse Output Data bits */

/*******************  Bit definition for CRC_INIT register  *******************/
#define CRC_INIT_INIT                       ((uint32_t)0xFFFFFFFFU) /*!< Initial CRC value bits */

/*******************  Bit definition for CRC_POL register  ********************/
#define CRC_POL_POL                         ((uint32_t)0xFFFFFFFFU) /*!< Coefficients of the polynomial */

/******************************************************************************/
/*                                                                            */
/*                           Debug MCU (DBGMCU)                               */
/*                                                                            */
/******************************************************************************/

/****************  Bit definition for DBGMCU_IDCODE register  *****************/
#define DBGMCU_IDCODE_DEV_ID                ((uint32_t)0x00000FFFU)        /*!< Device Identifier */

#define DBGMCU_IDCODE_REV_ID                ((uint32_t)0xFFFF0000U)        /*!< REV_ID[15:0] bits (Revision Identifier) */
#define DBGMCU_IDCODE_REV_ID_0              ((uint32_t)0x00010000U)        /*!< Bit 0 */
#define DBGMCU_IDCODE_REV_ID_1              ((uint32_t)0x00020000U)        /*!< Bit 1 */
#define DBGMCU_IDCODE_REV_ID_2              ((uint32_t)0x00040000U)        /*!< Bit 2 */
#define DBGMCU_IDCODE_REV_ID_3              ((uint32_t)0x00080000U)        /*!< Bit 3 */
#define DBGMCU_IDCODE_REV_ID_4              ((uint32_t)0x00100000U)        /*!< Bit 4 */
#define DBGMCU_IDCODE_REV_ID_5              ((uint32_t)0x00200000U)        /*!< Bit 5 */
#define DBGMCU_IDCODE_REV_ID_6              ((uint32_t)0x00400000U)        /*!< Bit 6 */
#define DBGMCU_IDCODE_REV_ID_7              ((uint32_t)0x00800000U)        /*!< Bit 7 */
#define DBGMCU_IDCODE_REV_ID_8              ((uint32_t)0x01000000U)        /*!< Bit 8 */
#define DBGMCU_IDCODE_REV_ID_9              ((uint32_t)0x02000000U)        /*!< Bit 9 */
#define DBGMCU_IDCODE_REV_ID_10             ((uint32_t)0x04000000U)        /*!< Bit 10 */
#define DBGMCU_IDCODE_REV_ID_11             ((uint32_t)0x08000000U)        /*!< Bit 11 */
#define DBGMCU_IDCODE_REV_ID_12             ((uint32_t)0x10000000U)        /*!< Bit 12 */
#define DBGMCU_IDCODE_REV_ID_13             ((uint32_t)0x20000000U)        /*!< Bit 13 */
#define DBGMCU_IDCODE_REV_ID_14             ((uint32_t)0x40000000U)        /*!< Bit 14 */
#define DBGMCU_IDCODE_REV_ID_15             ((uint32_t)0x80000000U)        /*!< Bit 15 */

/******************  Bit definition for DBGMCU_CR register  *******************/
#define DBGMCU_CR_DBG                       ((uint32_t)0x00000007U)        /*!< Debug mode mask */
#define DBGMCU_CR_DBG_SLEEP                 ((uint32_t)0x00000001U)        /*!< Debug Sleep Mode */
#define DBGMCU_CR_DBG_STOP                  ((uint32_t)0x00000002U)        /*!< Debug Stop Mode */
#define DBGMCU_CR_DBG_STANDBY               ((uint32_t)0x00000004U)        /*!< Debug Standby mode */

/******************  Bit definition for DBGMCU_APB1_FZ register  **************/
#define DBGMCU_APB1_FZ_DBG_TIM2_STOP        ((uint32_t)0x00000001U)        /*!< TIM2 counter stopped when core is halted */
#define DBGMCU_APB1_FZ_DBG_RTC_STOP         ((uint32_t)0x00000400U)        /*!< RTC Calendar frozen when core is halted */
#define DBGMCU_APB1_FZ_DBG_WWDG_STOP        ((uint32_t)0x00000800U)        /*!< Debug Window Watchdog stopped when Core is halted */
#define DBGMCU_APB1_FZ_DBG_IWDG_STOP        ((uint32_t)0x00001000U)        /*!< Debug Independent Watchdog stopped when Core is halted */
#define DBGMCU_APB1_FZ_DBG_I2C1_STOP        ((uint32_t)0x00200000U)        /*!< I2C1 SMBUS timeout mode stopped when Core is halted */
#define DBGMCU_APB1_FZ_DBG_LPTIMER_STOP     ((uint32_t)0x80000000U)        /*!< LPTIM1 counter stopped when core is halted */
/******************  Bit definition for DBGMCU_APB2_FZ register  **************/
#define DBGMCU_APB2_FZ_DBG_TIM21_STOP       ((uint32_t)0x00000004U)        /*!< TIM21 counter stopped when core is halted */

/******************************************************************************/
/*                                                                            */
/*                           DMA Controller (DMA)                             */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for DMA_ISR register  ********************/
#define DMA_ISR_GIF1                        ((uint32_t)0x00000001U)        /*!< Channel 1 Global interrupt flag    */
#define DMA_ISR_TCIF1                       ((uint32_t)0x00000002U)        /*!< Channel 1 Transfer Complete flag   */
#define DMA_ISR_HTIF1                       ((uint32_t)0x00000004U)        /*!< Channel 1 Half Transfer flag       */
#define DMA_ISR_TEIF1                       ((uint32_t)0x00000008U)        /*!< Channel 1 Transfer Error flag      */
#define DMA_ISR_GIF2                        ((uint32_t)0x00000010U)        /*!< Channel 2 Global interrupt flag    */
#define DMA_ISR_TCIF2                       ((uint32_t)0x00000020U)        /*!< Channel 2 Transfer Complete flag   */
#define DMA_ISR_HTIF2                       ((uint32_t)0x00000040U)        /*!< Channel 2 Half Transfer flag       */
#define DMA_ISR_TEIF2                       ((uint32_t)0x00000080U)        /*!< Channel 2 Transfer Error flag      */
#define DMA_ISR_GIF3                        ((uint32_t)0x00000100U)        /*!< Channel 3 Global interrupt flag    */
#define DMA_ISR_TCIF3                       ((uint32_t)0x00000200U)        /*!< Channel 3 Transfer Complete flag   */
#define DMA_ISR_HTIF3                       ((uint32_t)0x00000400U)        /*!< Channel 3 Half Transfer flag       */
#define DMA_ISR_TEIF3                       ((uint32_t)0x00000800U)        /*!< Channel 3 Transfer Error flag      */
#define DMA_ISR_GIF4                        ((uint32_t)0x00001000U)        /*!< Channel 4 Global interrupt flag    */
#define DMA_ISR_TCIF4                       ((uint32_t)0x00002000U)        /*!< Channel 4 Transfer Complete flag   */
#define DMA_ISR_HTIF4                       ((uint32_t)0x00004000U)        /*!< Channel 4 Half Transfer flag       */
#define DMA_ISR_TEIF4                       ((uint32_t)0x00008000U)        /*!< Channel 4 Transfer Error flag      */
#define DMA_ISR_GIF5                        ((uint32_t)0x00010000U)        /*!< Channel 5 Global interrupt flag    */
#define DMA_ISR_TCIF5                       ((uint32_t)0x00020000U)        /*!< Channel 5 Transfer Complete flag   */
#define DMA_ISR_HTIF5                       ((uint32_t)0x00040000U)        /*!< Channel 5 Half Transfer flag       */
#define DMA_ISR_TEIF5                       ((uint32_t)0x00080000U)        /*!< Channel 5 Transfer Error flag      */
#define DMA_ISR_GIF6                        ((uint32_t)0x00100000U)        /*!< Channel 6 Global interrupt flag */
#define DMA_ISR_TCIF6                       ((uint32_t)0x00200000U)        /*!< Channel 6 Transfer Complete flag */
#define DMA_ISR_HTIF6                       ((uint32_t)0x00400000U)        /*!< Channel 6 Half Transfer flag */
#define DMA_ISR_TEIF6                       ((uint32_t)0x00800000U)        /*!< Channel 6 Transfer Error flag */
#define DMA_ISR_GIF7                        ((uint32_t)0x01000000U)        /*!< Channel 7 Global interrupt flag */
#define DMA_ISR_TCIF7                       ((uint32_t)0x02000000U)        /*!< Channel 7 Transfer Complete flag */
#define DMA_ISR_HTIF7                       ((uint32_t)0x04000000U)        /*!< Channel 7 Half Transfer flag */
#define DMA_ISR_TEIF7                       ((uint32_t)0x08000000U)        /*!< Channel 7 Transfer Error flag */

/*******************  Bit definition for DMA_IFCR register  *******************/
#define DMA_IFCR_CGIF1                      ((uint32_t)0x00000001U)        /*!< Channel 1 Global interrupt clear    */
#define DMA_IFCR_CTCIF1                     ((uint32_t)0x00000002U)        /*!< Channel 1 Transfer Complete clear   */
#define DMA_IFCR_CHTIF1                     ((uint32_t)0x00000004U)        /*!< Channel 1 Half Transfer clear       */
#define DMA_IFCR_CTEIF1                     ((uint32_t)0x00000008U)        /*!< Channel 1 Transfer Error clear      */
#define DMA_IFCR_CGIF2                      ((uint32_t)0x00000010U)        /*!< Channel 2 Global interrupt clear    */
#define DMA_IFCR_CTCIF2                     ((uint32_t)0x00000020U)        /*!< Channel 2 Transfer Complete clear   */
#define DMA_IFCR_CHTIF2                     ((uint32_t)0x00000040U)        /*!< Channel 2 Half Transfer clear       */
#define DMA_IFCR_CTEIF2                     ((uint32_t)0x00000080U)        /*!< Channel 2 Transfer Error clear      */
#define DMA_IFCR_CGIF3                      ((uint32_t)0x00000100U)        /*!< Channel 3 Global interrupt clear    */
#define DMA_IFCR_CTCIF3                     ((uint32_t)0x00000200U)        /*!< Channel 3 Transfer Complete clear   */
#define DMA_IFCR_CHTIF3                     ((uint32_t)0x00000400U)        /*!< Channel 3 Half Transfer clear       */
#define DMA_IFCR_CTEIF3                     ((uint32_t)0x00000800U)        /*!< Channel 3 Transfer Error clear      */
#define DMA_IFCR_CGIF4                      ((uint32_t)0x00001000U)        /*!< Channel 4 Global interrupt clear    */
#define DMA_IFCR_CTCIF4                     ((uint32_t)0x00002000U)        /*!< Channel 4 Transfer Complete clear   */
#define DMA_IFCR_CHTIF4                     ((uint32_t)0x00004000U)        /*!< Channel 4 Half Transfer clear       */
#define DMA_IFCR_CTEIF4                     ((uint32_t)0x00008000U)        /*!< Channel 4 Transfer Error clear      */
#define DMA_IFCR_CGIF5                      ((uint32_t)0x00010000U)        /*!< Channel 5 Global interrupt clear    */
#define DMA_IFCR_CTCIF5                     ((uint32_t)0x00020000U)        /*!< Channel 5 Transfer Complete clear   */
#define DMA_IFCR_CHTIF5                     ((uint32_t)0x00040000U)        /*!< Channel 5 Half Transfer clear       */
#define DMA_IFCR_CTEIF5                     ((uint32_t)0x00080000U)        /*!< Channel 5 Transfer Error clear      */
#define DMA_IFCR_CGIF6                      ((uint32_t)0x00100000U)        /*!< Channel 6 Global interrupt clear */
#define DMA_IFCR_CTCIF6                     ((uint32_t)0x00200000U)        /*!< Channel 6 Transfer Complete clear */
#define DMA_IFCR_CHTIF6                     ((uint32_t)0x00400000U)        /*!< Channel 6 Half Transfer clear */
#define DMA_IFCR_CTEIF6                     ((uint32_t)0x00800000U)        /*!< Channel 6 Transfer Error clear */
#define DMA_IFCR_CGIF7                      ((uint32_t)0x01000000U)        /*!< Channel 7 Global interrupt clear */
#define DMA_IFCR_CTCIF7                     ((uint32_t)0x02000000U)        /*!< Channel 7 Transfer Complete clear */
#define DMA_IFCR_CHTIF7                     ((uint32_t)0x04000000U)        /*!< Channel 7 Half Transfer clear */
#define DMA_IFCR_CTEIF7                     ((uint32_t)0x08000000U)        /*!< Channel 7 Transfer Error clear */

/*******************  Bit definition for DMA_CCR register  ********************/
#define DMA_CCR_EN                          ((uint32_t)0x00000001U)        /*!< Channel enable                      */
#define DMA_CCR_TCIE                        ((uint32_t)0x00000002U)        /*!< Transfer complete interrupt enable  */
#define DMA_CCR_HTIE                        ((uint32_t)0x00000004U)        /*!< Half Transfer interrupt enable      */
#define DMA_CCR_TEIE                        ((uint32_t)0x00000008U)        /*!< Transfer error interrupt enable     */
#define DMA_CCR_DIR                         ((uint32_t)0x00000010U)        /*!< Data transfer direction             */
#define DMA_CCR_CIRC                        ((uint32_t)0x00000020U)        /*!< Circular mode                       */
#define DMA_CCR_PINC                        ((uint32_t)0x00000040U)        /*!< Peripheral increment mode           */
#define DMA_CCR_MINC                        ((uint32_t)0x00000080U)        /*!< Memory increment mode               */

#define DMA_CCR_PSIZE                       ((uint32_t)0x00000300U)        /*!< PSIZE[1:0] bits (Peripheral size)   */
#define DMA_CCR_PSIZE_0                     ((uint32_t)0x00000100U)        /*!< Bit 0                               */
#define DMA_CCR_PSIZE_1                     ((uint32_t)0x00000200U)        /*!< Bit 1                               */

#define DMA_CCR_MSIZE                       ((uint32_t)0x00000C00U)        /*!< MSIZE[1:0] bits (Memory size)       */
#define DMA_CCR_MSIZE_0                     ((uint32_t)0x00000400U)        /*!< Bit 0                               */
#define DMA_CCR_MSIZE_1                     ((uint32_t)0x00000800U)        /*!< Bit 1                               */

#define DMA_CCR_PL                          ((uint32_t)0x00003000U)        /*!< PL[1:0] bits(Channel Priority level)*/
#define DMA_CCR_PL_0                        ((uint32_t)0x00001000U)        /*!< Bit 0                               */
#define DMA_CCR_PL_1                        ((uint32_t)0x00002000U)        /*!< Bit 1                               */

#define DMA_CCR_MEM2MEM                     ((uint32_t)0x00004000U)        /*!< Memory to memory mode               */

/******************  Bit definition for DMA_CNDTR register  *******************/
#define DMA_CNDTR_NDT                       ((uint32_t)0x0000FFFFU)        /*!< Number of data to Transfer          */

/******************  Bit definition for DMA_CPAR register  ********************/
#define DMA_CPAR_PA                         ((uint32_t)0xFFFFFFFFU)        /*!< Peripheral Address                  */

/******************  Bit definition for DMA_CMAR register  ********************/
#define DMA_CMAR_MA                         ((uint32_t)0xFFFFFFFFU)        /*!< Memory Address                      */


/*******************  Bit definition for DMA_CSELR register  *******************/
#define DMA_CSELR_C1S                       ((uint32_t)0x0000000FU)          /*!< Channel 1 Selection */ 
#define DMA_CSELR_C2S                       ((uint32_t)0x000000F0U)          /*!< Channel 2 Selection */ 
#define DMA_CSELR_C3S                       ((uint32_t)0x00000F00U)          /*!< Channel 3 Selection */ 
#define DMA_CSELR_C4S                       ((uint32_t)0x0000F000U)          /*!< Channel 4 Selection */ 
#define DMA_CSELR_C5S                       ((uint32_t)0x000F0000U)          /*!< Channel 5 Selection */ 
#define DMA_CSELR_C6S                       ((uint32_t)0x00F00000U)          /*!< Channel 6 Selection */ 
#define DMA_CSELR_C7S                       ((uint32_t)0x0F000000U)          /*!< Channel 7 Selection */


/******************************************************************************/
/*                                                                            */
/*                 External Interrupt/Event Controller (EXTI)                 */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for EXTI_IMR register  *******************/
#define EXTI_IMR_IM0                        ((uint32_t)0x00000001U)        /*!< Interrupt Mask on line 0  */
#define EXTI_IMR_IM1                        ((uint32_t)0x00000002U)        /*!< Interrupt Mask on line 1  */
#define EXTI_IMR_IM2                        ((uint32_t)0x00000004U)        /*!< Interrupt Mask on line 2  */
#define EXTI_IMR_IM3                        ((uint32_t)0x00000008U)        /*!< Interrupt Mask on line 3  */
#define EXTI_IMR_IM4                        ((uint32_t)0x00000010U)        /*!< Interrupt Mask on line 4  */
#define EXTI_IMR_IM5                        ((uint32_t)0x00000020U)        /*!< Interrupt Mask on line 5  */
#define EXTI_IMR_IM6                        ((uint32_t)0x00000040U)        /*!< Interrupt Mask on line 6  */
#define EXTI_IMR_IM7                        ((uint32_t)0x00000080U)        /*!< Interrupt Mask on line 7  */
#define EXTI_IMR_IM8                        ((uint32_t)0x00000100U)        /*!< Interrupt Mask on line 8  */
#define EXTI_IMR_IM9                        ((uint32_t)0x00000200U)        /*!< Interrupt Mask on line 9  */
#define EXTI_IMR_IM10                       ((uint32_t)0x00000400U)        /*!< Interrupt Mask on line 10 */
#define EXTI_IMR_IM11                       ((uint32_t)0x00000800U)        /*!< Interrupt Mask on line 11 */
#define EXTI_IMR_IM12                       ((uint32_t)0x00001000U)        /*!< Interrupt Mask on line 12 */
#define EXTI_IMR_IM13                       ((uint32_t)0x00002000U)        /*!< Interrupt Mask on line 13 */
#define EXTI_IMR_IM14                       ((uint32_t)0x00004000U)        /*!< Interrupt Mask on line 14 */
#define EXTI_IMR_IM15                       ((uint32_t)0x00008000U)        /*!< Interrupt Mask on line 15 */
#define EXTI_IMR_IM16                       ((uint32_t)0x00010000U)        /*!< Interrupt Mask on line 16 */
#define EXTI_IMR_IM17                       ((uint32_t)0x00020000U)        /*!< Interrupt Mask on line 17 */
#define EXTI_IMR_IM18                       ((uint32_t)0x00040000U)        /*!< Interrupt Mask on line 18 */
#define EXTI_IMR_IM19                       ((uint32_t)0x00080000U)        /*!< Interrupt Mask on line 19 */
#define EXTI_IMR_IM20                       ((uint32_t)0x00100000U)        /*!< Interrupt Mask on line 20 */
#define EXTI_IMR_IM21                       ((uint32_t)0x00200000U)        /*!< Interrupt Mask on line 21 */
#define EXTI_IMR_IM22                       ((uint32_t)0x00400000U)        /*!< Interrupt Mask on line 22 */
#define EXTI_IMR_IM23                       ((uint32_t)0x00800000U)        /*!< Interrupt Mask on line 23 */
#define EXTI_IMR_IM25                       ((uint32_t)0x02000000U)        /*!< Interrupt Mask on line 25 */
#define EXTI_IMR_IM26                       ((uint32_t)0x04000000U)        /*!< Interrupt Mask on line 26 */
#define EXTI_IMR_IM28                       ((uint32_t)0x10000000U)        /*!< Interrupt Mask on line 28 */
#define EXTI_IMR_IM29                       ((uint32_t)0x20000000U)        /*!< Interrupt Mask on line 29 */

/******************  Bit definition for EXTI_EMR register  ********************/
#define EXTI_EMR_EM0                        ((uint32_t)0x00000001U)        /*!< Event Mask on line 0  */
#define EXTI_EMR_EM1                        ((uint32_t)0x00000002U)        /*!< Event Mask on line 1  */
#define EXTI_EMR_EM2                        ((uint32_t)0x00000004U)        /*!< Event Mask on line 2  */
#define EXTI_EMR_EM3                        ((uint32_t)0x00000008U)        /*!< Event Mask on line 3  */
#define EXTI_EMR_EM4                        ((uint32_t)0x00000010U)        /*!< Event Mask on line 4  */
#define EXTI_EMR_EM5                        ((uint32_t)0x00000020U)        /*!< Event Mask on line 5  */
#define EXTI_EMR_EM6                        ((uint32_t)0x00000040U)        /*!< Event Mask on line 6  */
#define EXTI_EMR_EM7                        ((uint32_t)0x00000080U)        /*!< Event Mask on line 7  */
#define EXTI_EMR_EM8                        ((uint32_t)0x00000100U)        /*!< Event Mask on line 8  */
#define EXTI_EMR_EM9                        ((uint32_t)0x00000200U)        /*!< Event Mask on line 9  */
#define EXTI_EMR_EM10                       ((uint32_t)0x00000400U)        /*!< Event Mask on line 10 */
#define EXTI_EMR_EM11                       ((uint32_t)0x00000800U)        /*!< Event Mask on line 11 */
#define EXTI_EMR_EM12                       ((uint32_t)0x00001000U)        /*!< Event Mask on line 12 */
#define EXTI_EMR_EM13                       ((uint32_t)0x00002000U)        /*!< Event Mask on line 13 */
#define EXTI_EMR_EM14                       ((uint32_t)0x00004000U)        /*!< Event Mask on line 14 */
#define EXTI_EMR_EM15                       ((uint32_t)0x00008000U)        /*!< Event Mask on line 15 */
#define EXTI_EMR_EM16                       ((uint32_t)0x00010000U)        /*!< Event Mask on line 16 */
#define EXTI_EMR_EM17                       ((uint32_t)0x00020000U)        /*!< Event Mask on line 17 */
#define EXTI_EMR_EM18                       ((uint32_t)0x00040000U)        /*!< Event Mask on line 18 */
#define EXTI_EMR_EM19                       ((uint32_t)0x00080000U)        /*!< Event Mask on line 19 */
#define EXTI_EMR_EM20                       ((uint32_t)0x00100000U)        /*!< Event Mask on line 20 */
#define EXTI_EMR_EM21                       ((uint32_t)0x00200000U)        /*!< Event Mask on line 21 */
#define EXTI_EMR_EM22                       ((uint32_t)0x00400000U)        /*!< Event Mask on line 22 */
#define EXTI_EMR_EM23                       ((uint32_t)0x00800000U)        /*!< Event Mask on line 23 */
#define EXTI_EMR_EM25                       ((uint32_t)0x02000000U)        /*!< Event Mask on line 25 */
#define EXTI_EMR_EM26                       ((uint32_t)0x04000000U)        /*!< Event Mask on line 26 */
#define EXTI_EMR_EM28                       ((uint32_t)0x10000000U)        /*!< Event Mask on line 28 */
#define EXTI_EMR_EM29                       ((uint32_t)0x20000000U)        /*!< Event Mask on line 29 */

/*******************  Bit definition for EXTI_RTSR register  ******************/
#define EXTI_RTSR_RT0                       ((uint32_t)0x00000001U)        /*!< Rising trigger event configuration bit of line 0 */
#define EXTI_RTSR_RT1                       ((uint32_t)0x00000002U)        /*!< Rising trigger event configuration bit of line 1 */
#define EXTI_RTSR_RT2                       ((uint32_t)0x00000004U)        /*!< Rising trigger event configuration bit of line 2 */
#define EXTI_RTSR_RT3                       ((uint32_t)0x00000008U)        /*!< Rising trigger event configuration bit of line 3 */
#define EXTI_RTSR_RT4                       ((uint32_t)0x00000010U)        /*!< Rising trigger event configuration bit of line 4 */
#define EXTI_RTSR_RT5                       ((uint32_t)0x00000020U)        /*!< Rising trigger event configuration bit of line 5 */
#define EXTI_RTSR_RT6                       ((uint32_t)0x00000040U)        /*!< Rising trigger event configuration bit of line 6 */
#define EXTI_RTSR_RT7                       ((uint32_t)0x00000080U)        /*!< Rising trigger event configuration bit of line 7 */
#define EXTI_RTSR_RT8                       ((uint32_t)0x00000100U)        /*!< Rising trigger event configuration bit of line 8 */
#define EXTI_RTSR_RT9                       ((uint32_t)0x00000200U)        /*!< Rising trigger event configuration bit of line 9 */
#define EXTI_RTSR_RT10                      ((uint32_t)0x00000400U)        /*!< Rising trigger event configuration bit of line 10 */
#define EXTI_RTSR_RT11                      ((uint32_t)0x00000800U)        /*!< Rising trigger event configuration bit of line 11 */
#define EXTI_RTSR_RT12                      ((uint32_t)0x00001000U)        /*!< Rising trigger event configuration bit of line 12 */
#define EXTI_RTSR_RT13                      ((uint32_t)0x00002000U)        /*!< Rising trigger event configuration bit of line 13 */
#define EXTI_RTSR_RT14                      ((uint32_t)0x00004000U)        /*!< Rising trigger event configuration bit of line 14 */
#define EXTI_RTSR_RT15                      ((uint32_t)0x00008000U)        /*!< Rising trigger event configuration bit of line 15 */
#define EXTI_RTSR_RT16                      ((uint32_t)0x00010000U)        /*!< Rising trigger event configuration bit of line 16 */
#define EXTI_RTSR_RT17                      ((uint32_t)0x00020000U)        /*!< Rising trigger event configuration bit of line 17 */
#define EXTI_RTSR_RT19                      ((uint32_t)0x00080000U)        /*!< Rising trigger event configuration bit of line 19 */
#define EXTI_RTSR_RT20                      ((uint32_t)0x00100000U)        /*!< Rising trigger event configuration bit of line 20 */
#define EXTI_RTSR_RT21                      ((uint32_t)0x00200000U)        /*!< Rising trigger event configuration bit of line 21 */
#define EXTI_RTSR_RT22                      ((uint32_t)0x00400000U)        /*!< Rising trigger event configuration bit of line 22 */

/* Legacy defines */
#define EXTI_RTSR_TR0                       EXTI_RTSR_RT0
#define EXTI_RTSR_TR1                       EXTI_RTSR_RT1
#define EXTI_RTSR_TR2                       EXTI_RTSR_RT2
#define EXTI_RTSR_TR3                       EXTI_RTSR_RT3
#define EXTI_RTSR_TR4                       EXTI_RTSR_RT4
#define EXTI_RTSR_TR5                       EXTI_RTSR_RT5
#define EXTI_RTSR_TR6                       EXTI_RTSR_RT6
#define EXTI_RTSR_TR7                       EXTI_RTSR_RT7
#define EXTI_RTSR_TR8                       EXTI_RTSR_RT8
#define EXTI_RTSR_TR9                       EXTI_RTSR_RT9
#define EXTI_RTSR_TR10                      EXTI_RTSR_RT10
#define EXTI_RTSR_TR11                      EXTI_RTSR_RT11
#define EXTI_RTSR_TR12                      EXTI_RTSR_RT12
#define EXTI_RTSR_TR13                      EXTI_RTSR_RT13
#define EXTI_RTSR_TR14                      EXTI_RTSR_RT14
#define EXTI_RTSR_TR15                      EXTI_RTSR_RT15
#define EXTI_RTSR_TR16                      EXTI_RTSR_RT16
#define EXTI_RTSR_TR17                      EXTI_RTSR_RT17
#define EXTI_RTSR_TR19                      EXTI_RTSR_RT19
#define EXTI_RTSR_TR20                      EXTI_RTSR_RT20
#define EXTI_RTSR_TR21                      EXTI_RTSR_RT21
#define EXTI_RTSR_TR22                      EXTI_RTSR_RT22

/*******************  Bit definition for EXTI_FTSR register *******************/
#define EXTI_FTSR_FT0                       ((uint32_t)0x00000001U)        /*!< Falling trigger event configuration bit of line 0 */
#define EXTI_FTSR_FT1                       ((uint32_t)0x00000002U)        /*!< Falling trigger event configuration bit of line 1 */
#define EXTI_FTSR_FT2                       ((uint32_t)0x00000004U)        /*!< Falling trigger event configuration bit of line 2 */
#define EXTI_FTSR_FT3                       ((uint32_t)0x00000008U)        /*!< Falling trigger event configuration bit of line 3 */
#define EXTI_FTSR_FT4                       ((uint32_t)0x00000010U)        /*!< Falling trigger event configuration bit of line 4 */
#define EXTI_FTSR_FT5                       ((uint32_t)0x00000020U)        /*!< Falling trigger event configuration bit of line 5 */
#define EXTI_FTSR_FT6                       ((uint32_t)0x00000040U)        /*!< Falling trigger event configuration bit of line 6 */
#define EXTI_FTSR_FT7                       ((uint32_t)0x00000080U)        /*!< Falling trigger event configuration bit of line 7 */
#define EXTI_FTSR_FT8                       ((uint32_t)0x00000100U)        /*!< Falling trigger event configuration bit of line 8 */
#define EXTI_FTSR_FT9                       ((uint32_t)0x00000200U)        /*!< Falling trigger event configuration bit of line 9 */
#define EXTI_FTSR_FT10                      ((uint32_t)0x00000400U)        /*!< Falling trigger event configuration bit of line 10 */
#define EXTI_FTSR_FT11                      ((uint32_t)0x00000800U)        /*!< Falling trigger event configuration bit of line 11 */
#define EXTI_FTSR_FT12                      ((uint32_t)0x00001000U)        /*!< Falling trigger event configuration bit of line 12 */
#define EXTI_FTSR_FT13                      ((uint32_t)0x00002000U)        /*!< Falling trigger event configuration bit of line 13 */
#define EXTI_FTSR_FT14                      ((uint32_t)0x00004000U)        /*!< Falling trigger event configuration bit of line 14 */
#define EXTI_FTSR_FT15                      ((uint32_t)0x00008000U)        /*!< Falling trigger event configuration bit of line 15 */
#define EXTI_FTSR_FT16                      ((uint32_t)0x00010000U)        /*!< Falling trigger event configuration bit of line 16 */
#define EXTI_FTSR_FT17                      ((uint32_t)0x00020000U)        /*!< Falling trigger event configuration bit of line 17 */
#define EXTI_FTSR_FT19                      ((uint32_t)0x00080000U)        /*!< Falling trigger event configuration bit of line 19 */
#define EXTI_FTSR_FT20                      ((uint32_t)0x00100000U)        /*!< Falling trigger event configuration bit of line 20 */
#define EXTI_FTSR_FT21                      ((uint32_t)0x00200000U)        /*!< Falling trigger event configuration bit of line 21 */
#define EXTI_FTSR_FT22                      ((uint32_t)0x00400000U)        /*!< Falling trigger event configuration bit of line 22 */

/* Legacy defines */
#define EXTI_FTSR_TR0                       EXTI_FTSR_FT0
#define EXTI_FTSR_TR1                       EXTI_FTSR_FT1
#define EXTI_FTSR_TR2                       EXTI_FTSR_FT2
#define EXTI_FTSR_TR3                       EXTI_FTSR_FT3
#define EXTI_FTSR_TR4                       EXTI_FTSR_FT4
#define EXTI_FTSR_TR5                       EXTI_FTSR_FT5
#define EXTI_FTSR_TR6                       EXTI_FTSR_FT6
#define EXTI_FTSR_TR7                       EXTI_FTSR_FT7
#define EXTI_FTSR_TR8                       EXTI_FTSR_FT8
#define EXTI_FTSR_TR9                       EXTI_FTSR_FT9
#define EXTI_FTSR_TR10                      EXTI_FTSR_FT10
#define EXTI_FTSR_TR11                      EXTI_FTSR_FT11
#define EXTI_FTSR_TR12                      EXTI_FTSR_FT12
#define EXTI_FTSR_TR13                      EXTI_FTSR_FT13
#define EXTI_FTSR_TR14                      EXTI_FTSR_FT14
#define EXTI_FTSR_TR15                      EXTI_FTSR_FT15
#define EXTI_FTSR_TR16                      EXTI_FTSR_FT16
#define EXTI_FTSR_TR17                      EXTI_FTSR_FT17
#define EXTI_FTSR_TR19                      EXTI_FTSR_FT19
#define EXTI_FTSR_TR20                      EXTI_FTSR_FT20
#define EXTI_FTSR_TR21                      EXTI_FTSR_FT21
#define EXTI_FTSR_TR22                      EXTI_FTSR_FT22

/******************* Bit definition for EXTI_SWIER register *******************/
#define EXTI_SWIER_SWI0                      ((uint32_t)0x00000001U)        /*!< Software Interrupt on line 0  */
#define EXTI_SWIER_SWI1                      ((uint32_t)0x00000002U)        /*!< Software Interrupt on line 1  */
#define EXTI_SWIER_SWI2                      ((uint32_t)0x00000004U)        /*!< Software Interrupt on line 2  */
#define EXTI_SWIER_SWI3                      ((uint32_t)0x00000008U)        /*!< Software Interrupt on line 3  */
#define EXTI_SWIER_SWI4                      ((uint32_t)0x00000010U)        /*!< Software Interrupt on line 4  */
#define EXTI_SWIER_SWI5                      ((uint32_t)0x00000020U)        /*!< Software Interrupt on line 5  */
#define EXTI_SWIER_SWI6                      ((uint32_t)0x00000040U)        /*!< Software Interrupt on line 6  */
#define EXTI_SWIER_SWI7                      ((uint32_t)0x00000080U)        /*!< Software Interrupt on line 7  */
#define EXTI_SWIER_SWI8                      ((uint32_t)0x00000100U)        /*!< Software Interrupt on line 8  */
#define EXTI_SWIER_SWI9                      ((uint32_t)0x00000200U)        /*!< Software Interrupt on line 9  */
#define EXTI_SWIER_SWI10                     ((uint32_t)0x00000400U)        /*!< Software Interrupt on line 10 */
#define EXTI_SWIER_SWI11                     ((uint32_t)0x00000800U)        /*!< Software Interrupt on line 11 */
#define EXTI_SWIER_SWI12                     ((uint32_t)0x00001000U)        /*!< Software Interrupt on line 12 */
#define EXTI_SWIER_SWI13                     ((uint32_t)0x00002000U)        /*!< Software Interrupt on line 13 */
#define EXTI_SWIER_SWI14                     ((uint32_t)0x00004000U)        /*!< Software Interrupt on line 14 */
#define EXTI_SWIER_SWI15                     ((uint32_t)0x00008000U)        /*!< Software Interrupt on line 15 */
#define EXTI_SWIER_SWI16                     ((uint32_t)0x00010000U)        /*!< Software Interrupt on line 16 */
#define EXTI_SWIER_SWI17                     ((uint32_t)0x00020000U)        /*!< Software Interrupt on line 17 */
#define EXTI_SWIER_SWI19                     ((uint32_t)0x00080000U)        /*!< Software Interrupt on line 19 */
#define EXTI_SWIER_SWI20                     ((uint32_t)0x00100000U)        /*!< Software Interrupt on line 20 */
#define EXTI_SWIER_SWI21                     ((uint32_t)0x00200000U)        /*!< Software Interrupt on line 21 */
#define EXTI_SWIER_SWI22                     ((uint32_t)0x00400000U)        /*!< Software Interrupt on line 22 */

/* Legacy defines */
#define EXTI_SWIER_SWIER0                   EXTI_SWIER_SWI0
#define EXTI_SWIER_SWIER1                   EXTI_SWIER_SWI1
#define EXTI_SWIER_SWIER2                   EXTI_SWIER_SWI2
#define EXTI_SWIER_SWIER3                   EXTI_SWIER_SWI3
#define EXTI_SWIER_SWIER4                   EXTI_SWIER_SWI4
#define EXTI_SWIER_SWIER5                   EXTI_SWIER_SWI5
#define EXTI_SWIER_SWIER6                   EXTI_SWIER_SWI6
#define EXTI_SWIER_SWIER7                   EXTI_SWIER_SWI7
#define EXTI_SWIER_SWIER8                   EXTI_SWIER_SWI8
#define EXTI_SWIER_SWIER9                   EXTI_SWIER_SWI9
#define EXTI_SWIER_SWIER10                  EXTI_SWIER_SWI10
#define EXTI_SWIER_SWIER11                  EXTI_SWIER_SWI11
#define EXTI_SWIER_SWIER12                  EXTI_SWIER_SWI12
#define EXTI_SWIER_SWIER13                  EXTI_SWIER_SWI13
#define EXTI_SWIER_SWIER14                  EXTI_SWIER_SWI14
#define EXTI_SWIER_SWIER15                  EXTI_SWIER_SWI15
#define EXTI_SWIER_SWIER16                  EXTI_SWIER_SWI16
#define EXTI_SWIER_SWIER17                  EXTI_SWIER_SWI17
#define EXTI_SWIER_SWIER19                  EXTI_SWIER_SWI19
#define EXTI_SWIER_SWIER20                  EXTI_SWIER_SWI20
#define EXTI_SWIER_SWIER21                  EXTI_SWIER_SWI21
#define EXTI_SWIER_SWIER22                  EXTI_SWIER_SWI22

/******************  Bit definition for EXTI_PR register  *********************/
#define EXTI_PR_PIF0                        ((uint32_t)0x00000001U)        /*!< Pending bit 0  */
#define EXTI_PR_PIF1                        ((uint32_t)0x00000002U)        /*!< Pending bit 1  */
#define EXTI_PR_PIF2                        ((uint32_t)0x00000004U)        /*!< Pending bit 2  */
#define EXTI_PR_PIF3                        ((uint32_t)0x00000008U)        /*!< Pending bit 3  */
#define EXTI_PR_PIF4                        ((uint32_t)0x00000010U)        /*!< Pending bit 4  */
#define EXTI_PR_PIF5                        ((uint32_t)0x00000020U)        /*!< Pending bit 5  */
#define EXTI_PR_PIF6                        ((uint32_t)0x00000040U)        /*!< Pending bit 6  */
#define EXTI_PR_PIF7                        ((uint32_t)0x00000080U)        /*!< Pending bit 7  */
#define EXTI_PR_PIF8                        ((uint32_t)0x00000100U)        /*!< Pending bit 8  */
#define EXTI_PR_PIF9                        ((uint32_t)0x00000200U)        /*!< Pending bit 9  */
#define EXTI_PR_PIF10                       ((uint32_t)0x00000400U)        /*!< Pending bit 10 */
#define EXTI_PR_PIF11                       ((uint32_t)0x00000800U)        /*!< Pending bit 11 */
#define EXTI_PR_PIF12                       ((uint32_t)0x00001000U)        /*!< Pending bit 12 */
#define EXTI_PR_PIF13                       ((uint32_t)0x00002000U)        /*!< Pending bit 13 */
#define EXTI_PR_PIF14                       ((uint32_t)0x00004000U)        /*!< Pending bit 14 */
#define EXTI_PR_PIF15                       ((uint32_t)0x00008000U)        /*!< Pending bit 15 */
#define EXTI_PR_PIF16                       ((uint32_t)0x00010000U)        /*!< Pending bit 16 */
#define EXTI_PR_PIF17                       ((uint32_t)0x00020000U)        /*!< Pending bit 17 */
#define EXTI_PR_PIF19                       ((uint32_t)0x00080000U)        /*!< Pending bit 19 */
#define EXTI_PR_PIF20                       ((uint32_t)0x00100000U)        /*!< Pending bit 20 */
#define EXTI_PR_PIF21                       ((uint32_t)0x00200000U)        /*!< Pending bit 21 */
#define EXTI_PR_PIF22                       ((uint32_t)0x00400000U)        /*!< Pending bit 22 */

/* Legacy defines */
#define EXTI_PR_PR0                         EXTI_PR_PIF0
#define EXTI_PR_PR1                         EXTI_PR_PIF1
#define EXTI_PR_PR2                         EXTI_PR_PIF2
#define EXTI_PR_PR3                         EXTI_PR_PIF3
#define EXTI_PR_PR4                         EXTI_PR_PIF4
#define EXTI_PR_PR5                         EXTI_PR_PIF5
#define EXTI_PR_PR6                         EXTI_PR_PIF6
#define EXTI_PR_PR7                         EXTI_PR_PIF7
#define EXTI_PR_PR8                         EXTI_PR_PIF8
#define EXTI_PR_PR9                         EXTI_PR_PIF9
#define EXTI_PR_PR10                        EXTI_PR_PIF10
#define EXTI_PR_PR11                        EXTI_PR_PIF11
#define EXTI_PR_PR12                        EXTI_PR_PIF12
#define EXTI_PR_PR13                        EXTI_PR_PIF13
#define EXTI_PR_PR14                        EXTI_PR_PIF14
#define EXTI_PR_PR15                        EXTI_PR_PIF15
#define EXTI_PR_PR16                        EXTI_PR_PIF16
#define EXTI_PR_PR17                        EXTI_PR_PIF17
#define EXTI_PR_PR19                        EXTI_PR_PIF19
#define EXTI_PR_PR20                        EXTI_PR_PIF20
#define EXTI_PR_PR21                        EXTI_PR_PIF21
#define EXTI_PR_PR22                        EXTI_PR_PIF22

/******************************************************************************/
/*                                                                            */
/*                      FLASH and Option Bytes Registers                      */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for FLASH_ACR register  ******************/
#define FLASH_ACR_LATENCY                   ((uint32_t)0x00000001U)        /*!< LATENCY bit (Latency) */
#define FLASH_ACR_PRFTEN                    ((uint32_t)0x00000002U)        /*!< Prefetch Buffer Enable */
#define FLASH_ACR_SLEEP_PD                  ((uint32_t)0x00000008U)        /*!< Flash mode during sleep mode */
#define FLASH_ACR_RUN_PD                    ((uint32_t)0x00000010U)        /*!< Flash mode during RUN mode */
#define FLASH_ACR_DISAB_BUF                 ((uint32_t)0x00000020U)        /*!< Disable Buffer */
#define FLASH_ACR_PRE_READ                  ((uint32_t)0x00000040U)        /*!< Pre-read data address */

/*******************  Bit definition for FLASH_PECR register  ******************/
#define FLASH_PECR_PELOCK                    ((uint32_t)0x00000001U)        /*!< FLASH_PECR and Flash data Lock */
#define FLASH_PECR_PRGLOCK                   ((uint32_t)0x00000002U)        /*!< Program matrix Lock */
#define FLASH_PECR_OPTLOCK                   ((uint32_t)0x00000004U)        /*!< Option byte matrix Lock */
#define FLASH_PECR_PROG                      ((uint32_t)0x00000008U)        /*!< Program matrix selection */
#define FLASH_PECR_DATA                      ((uint32_t)0x00000010U)        /*!< Data matrix selection */
#define FLASH_PECR_FIX                       ((uint32_t)0x00000100U)        /*!< Fixed Time Data write for Word/Half Word/Byte programming */
#define FLASH_PECR_ERASE                     ((uint32_t)0x00000200U)        /*!< Page erasing mode */
#define FLASH_PECR_FPRG                      ((uint32_t)0x00000400U)        /*!< Fast Page/Half Page programming mode */
#define FLASH_PECR_EOPIE                     ((uint32_t)0x00010000U)        /*!< End of programming interrupt */ 
#define FLASH_PECR_ERRIE                     ((uint32_t)0x00020000U)        /*!< Error interrupt */ 
#define FLASH_PECR_OBL_LAUNCH                ((uint32_t)0x00040000U)        /*!< Launch the option byte loading */
#define FLASH_PECR_HALF_ARRAY                ((uint32_t)0x00080000U)        /*!< Half array mode */

/******************  Bit definition for FLASH_PDKEYR register  ******************/
#define FLASH_PDKEYR_PDKEYR                 ((uint32_t)0xFFFFFFFFU)       /*!< FLASH_PEC and data matrix Key */

/******************  Bit definition for FLASH_PEKEYR register  ******************/
#define FLASH_PEKEYR_PEKEYR                 ((uint32_t)0xFFFFFFFFU)       /*!< FLASH_PEC and data matrix Key */

/******************  Bit definition for FLASH_PRGKEYR register  ******************/
#define FLASH_PRGKEYR_PRGKEYR               ((uint32_t)0xFFFFFFFFU)        /*!< Program matrix Key */

/******************  Bit definition for FLASH_OPTKEYR register  ******************/
#define FLASH_OPTKEYR_OPTKEYR               ((uint32_t)0xFFFFFFFFU)        /*!< Option bytes matrix Key */

/******************  Bit definition for FLASH_SR register  *******************/
#define FLASH_SR_BSY                        ((uint32_t)0x00000001U)        /*!< Busy */
#define FLASH_SR_EOP                        ((uint32_t)0x00000002U)        /*!< End Of Programming*/
#define FLASH_SR_HVOFF                      ((uint32_t)0x00000004U)        /*!< End of high voltage */
#define FLASH_SR_READY                      ((uint32_t)0x00000008U)        /*!< Flash ready after low power mode */

#define FLASH_SR_WRPERR                     ((uint32_t)0x00000100U)        /*!< Write protection error */
#define FLASH_SR_PGAERR                     ((uint32_t)0x00000200U)        /*!< Programming Alignment Error */
#define FLASH_SR_SIZERR                     ((uint32_t)0x00000400U)        /*!< Size error */
#define FLASH_SR_OPTVERR                    ((uint32_t)0x00000800U)        /*!< Option Valid error */
#define FLASH_SR_RDERR                      ((uint32_t)0x00002000U)        /*!< Read protected error */
#define FLASH_SR_NOTZEROERR                 ((uint32_t)0x00010000U)        /*!< Not Zero error */
#define FLASH_SR_FWWERR                     ((uint32_t)0x00020000U)        /*!< Write/Errase operation aborted */

/* Legacy defines */
#define FLASH_SR_FWWER                      FLASH_SR_FWWERR
#define FLASH_SR_ENHV                       FLASH_SR_HVOFF
#define FLASH_SR_ENDHV                      FLASH_SR_HVOFF

/******************  Bit definition for FLASH_OPTR register  *******************/
#define FLASH_OPTR_RDPROT                   ((uint32_t)0x000000FFU)        /*!< Read Protection */
#define FLASH_OPTR_WPRMOD                   ((uint32_t)0x00000100U)        /*!< Selection of protection mode of WPR bits */
#define FLASH_OPTR_BOR_LEV                  ((uint32_t)0x000F0000U)        /*!< BOR_LEV[3:0] Brown Out Reset Threshold Level*/
#define FLASH_OPTR_IWDG_SW                  ((uint32_t)0x00100000U)        /*!< IWDG_SW */
#define FLASH_OPTR_nRST_STOP                ((uint32_t)0x00200000U)        /*!< nRST_STOP */
#define FLASH_OPTR_nRST_STDBY               ((uint32_t)0x00400000U)        /*!< nRST_STDBY */
#define FLASH_OPTR_USER                     ((uint32_t)0x00700000U)        /*!< User Option Bytes */
#define FLASH_OPTR_BOOT1                    ((uint32_t)0x80000000U)        /*!< BOOT1 */

/******************  Bit definition for FLASH_WRPR register  ******************/
#define FLASH_WRPR_WRP                      ((uint32_t)0x0000FFFFU)        /*!< Write Protection bits */

/******************************************************************************/
/*                                                                            */
/*                       General Purpose IOs (GPIO)                           */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for GPIO_MODER register  *****************/
#define GPIO_MODER_MODE0          ((uint32_t)0x00000003U)
#define GPIO_MODER_MODE0_0        ((uint32_t)0x00000001U)
#define GPIO_MODER_MODE0_1        ((uint32_t)0x00000002U)
#define GPIO_MODER_MODE1          ((uint32_t)0x0000000CU)
#define GPIO_MODER_MODE1_0        ((uint32_t)0x00000004U)
#define GPIO_MODER_MODE1_1        ((uint32_t)0x00000008U)
#define GPIO_MODER_MODE2          ((uint32_t)0x00000030U)
#define GPIO_MODER_MODE2_0        ((uint32_t)0x00000010U)
#define GPIO_MODER_MODE2_1        ((uint32_t)0x00000020U)
#define GPIO_MODER_MODE3          ((uint32_t)0x000000C0U)
#define GPIO_MODER_MODE3_0        ((uint32_t)0x00000040U)
#define GPIO_MODER_MODE3_1        ((uint32_t)0x00000080U)
#define GPIO_MODER_MODE4          ((uint32_t)0x00000300U)
#define GPIO_MODER_MODE4_0        ((uint32_t)0x00000100U)
#define GPIO_MODER_MODE4_1        ((uint32_t)0x00000200U)
#define GPIO_MODER_MODE5          ((uint32_t)0x00000C00U)
#define GPIO_MODER_MODE5_0        ((uint32_t)0x00000400U)
#define GPIO_MODER_MODE5_1        ((uint32_t)0x00000800U)
#define GPIO_MODER_MODE6          ((uint32_t)0x00003000U)
#define GPIO_MODER_MODE6_0        ((uint32_t)0x00001000U)
#define GPIO_MODER_MODE6_1        ((uint32_t)0x00002000U)
#define GPIO_MODER_MODE7          ((uint32_t)0x0000C000U)
#define GPIO_MODER_MODE7_0        ((uint32_t)0x00004000U)
#define GPIO_MODER_MODE7_1        ((uint32_t)0x00008000U)
#define GPIO_MODER_MODE8          ((uint32_t)0x00030000U)
#define GPIO_MODER_MODE8_0        ((uint32_t)0x00010000U)
#define GPIO_MODER_MODE8_1        ((uint32_t)0x00020000U)
#define GPIO_MODER_MODE9          ((uint32_t)0x000C0000U)
#define GPIO_MODER_MODE9_0        ((uint32_t)0x00040000U)
#define GPIO_MODER_MODE9_1        ((uint32_t)0x00080000U)
#define GPIO_MODER_MODE10         ((uint32_t)0x00300000U)
#define GPIO_MODER_MODE10_0       ((uint32_t)0x00100000U)
#define GPIO_MODER_MODE10_1       ((uint32_t)0x00200000U)
#define GPIO_MODER_MODE11         ((uint32_t)0x00C00000U)
#define GPIO_MODER_MODE11_0       ((uint32_t)0x00400000U)
#define GPIO_MODER_MODE11_1       ((uint32_t)0x00800000U)
#define GPIO_MODER_MODE12         ((uint32_t)0x03000000U)
#define GPIO_MODER_MODE12_0       ((uint32_t)0x01000000U)
#define GPIO_MODER_MODE12_1       ((uint32_t)0x02000000U)
#define GPIO_MODER_MODE13         ((uint32_t)0x0C000000U)
#define GPIO_MODER_MODE13_0       ((uint32_t)0x04000000U)
#define GPIO_MODER_MODE13_1       ((uint32_t)0x08000000U)
#define GPIO_MODER_MODE14         ((uint32_t)0x30000000U)
#define GPIO_MODER_MODE14_0       ((uint32_t)0x10000000U)
#define GPIO_MODER_MODE14_1       ((uint32_t)0x20000000U)
#define GPIO_MODER_MODE15         ((uint32_t)0xC0000000U)
#define GPIO_MODER_MODE15_0       ((uint32_t)0x40000000U)
#define GPIO_MODER_MODE15_1       ((uint32_t)0x80000000U)

/******************  Bit definition for GPIO_OTYPER register  *****************/
#define GPIO_OTYPER_OT_0          ((uint32_t)0x00000001U)
#define GPIO_OTYPER_OT_1          ((uint32_t)0x00000002U)
#define GPIO_OTYPER_OT_2          ((uint32_t)0x00000004U)
#define GPIO_OTYPER_OT_3          ((uint32_t)0x00000008U)
#define GPIO_OTYPER_OT_4          ((uint32_t)0x00000010U)
#define GPIO_OTYPER_OT_5          ((uint32_t)0x00000020U)
#define GPIO_OTYPER_OT_6          ((uint32_t)0x00000040U)
#define GPIO_OTYPER_OT_7          ((uint32_t)0x00000080U)
#define GPIO_OTYPER_OT_8          ((uint32_t)0x00000100U)
#define GPIO_OTYPER_OT_9          ((uint32_t)0x00000200U)
#define GPIO_OTYPER_OT_10         ((uint32_t)0x00000400U)
#define GPIO_OTYPER_OT_11         ((uint32_t)0x00000800U)
#define GPIO_OTYPER_OT_12         ((uint32_t)0x00001000U)
#define GPIO_OTYPER_OT_13         ((uint32_t)0x00002000U)
#define GPIO_OTYPER_OT_14         ((uint32_t)0x00004000U)
#define GPIO_OTYPER_OT_15         ((uint32_t)0x00008000U)

/****************  Bit definition for GPIO_OSPEEDR register  ******************/
#define GPIO_OSPEEDER_OSPEED0     ((uint32_t)0x00000003U)
#define GPIO_OSPEEDER_OSPEED0_0   ((uint32_t)0x00000001U)
#define GPIO_OSPEEDER_OSPEED0_1   ((uint32_t)0x00000002U)
#define GPIO_OSPEEDER_OSPEED1     ((uint32_t)0x0000000CU)
#define GPIO_OSPEEDER_OSPEED1_0   ((uint32_t)0x00000004U)
#define GPIO_OSPEEDER_OSPEED1_1   ((uint32_t)0x00000008U)
#define GPIO_OSPEEDER_OSPEED2     ((uint32_t)0x00000030U)
#define GPIO_OSPEEDER_OSPEED2_0   ((uint32_t)0x00000010U)
#define GPIO_OSPEEDER_OSPEED2_1   ((uint32_t)0x00000020U)
#define GPIO_OSPEEDER_OSPEED3     ((uint32_t)0x000000C0U)
#define GPIO_OSPEEDER_OSPEED3_0   ((uint32_t)0x00000040U)
#define GPIO_OSPEEDER_OSPEED3_1   ((uint32_t)0x00000080U)
#define GPIO_OSPEEDER_OSPEED4     ((uint32_t)0x00000300U)
#define GPIO_OSPEEDER_OSPEED4_0   ((uint32_t)0x00000100U)
#define GPIO_OSPEEDER_OSPEED4_1   ((uint32_t)0x00000200U)
#define GPIO_OSPEEDER_OSPEED5     ((uint32_t)0x00000C00U)
#define GPIO_OSPEEDER_OSPEED5_0   ((uint32_t)0x00000400U)
#define GPIO_OSPEEDER_OSPEED5_1   ((uint32_t)0x00000800U)
#define GPIO_OSPEEDER_OSPEED6     ((uint32_t)0x00003000U)
#define GPIO_OSPEEDER_OSPEED6_0   ((uint32_t)0x00001000U)
#define GPIO_OSPEEDER_OSPEED6_1   ((uint32_t)0x00002000U)
#define GPIO_OSPEEDER_OSPEED7     ((uint32_t)0x0000C000U)
#define GPIO_OSPEEDER_OSPEED7_0   ((uint32_t)0x00004000U)
#define GPIO_OSPEEDER_OSPEED7_1   ((uint32_t)0x00008000U)
#define GPIO_OSPEEDER_OSPEED8     ((uint32_t)0x00030000U)
#define GPIO_OSPEEDER_OSPEED8_0   ((uint32_t)0x00010000U)
#define GPIO_OSPEEDER_OSPEED8_1   ((uint32_t)0x00020000U)
#define GPIO_OSPEEDER_OSPEED9     ((uint32_t)0x000C0000U)
#define GPIO_OSPEEDER_OSPEED9_0   ((uint32_t)0x00040000U)
#define GPIO_OSPEEDER_OSPEED9_1   ((uint32_t)0x00080000U)
#define GPIO_OSPEEDER_OSPEED10    ((uint32_t)0x00300000U)
#define GPIO_OSPEEDER_OSPEED10_0  ((uint32_t)0x00100000U)
#define GPIO_OSPEEDER_OSPEED10_1  ((uint32_t)0x00200000U)
#define GPIO_OSPEEDER_OSPEED11    ((uint32_t)0x00C00000U)
#define GPIO_OSPEEDER_OSPEED11_0  ((uint32_t)0x00400000U)
#define GPIO_OSPEEDER_OSPEED11_1  ((uint32_t)0x00800000U)
#define GPIO_OSPEEDER_OSPEED12    ((uint32_t)0x03000000U)
#define GPIO_OSPEEDER_OSPEED12_0  ((uint32_t)0x01000000U)
#define GPIO_OSPEEDER_OSPEED12_1  ((uint32_t)0x02000000U)
#define GPIO_OSPEEDER_OSPEED13    ((uint32_t)0x0C000000U)
#define GPIO_OSPEEDER_OSPEED13_0  ((uint32_t)0x04000000U)
#define GPIO_OSPEEDER_OSPEED13_1  ((uint32_t)0x08000000U)
#define GPIO_OSPEEDER_OSPEED14    ((uint32_t)0x30000000U)
#define GPIO_OSPEEDER_OSPEED14_0  ((uint32_t)0x10000000U)
#define GPIO_OSPEEDER_OSPEED14_1  ((uint32_t)0x20000000U)
#define GPIO_OSPEEDER_OSPEED15    ((uint32_t)0xC0000000U)
#define GPIO_OSPEEDER_OSPEED15_0  ((uint32_t)0x40000000U)
#define GPIO_OSPEEDER_OSPEED15_1  ((uint32_t)0x80000000U)

/*******************  Bit definition for GPIO_PUPDR register ******************/
#define GPIO_PUPDR_PUPD0          ((uint32_t)0x00000003U)
#define GPIO_PUPDR_PUPD0_0        ((uint32_t)0x00000001U)
#define GPIO_PUPDR_PUPD0_1        ((uint32_t)0x00000002U)
#define GPIO_PUPDR_PUPD1          ((uint32_t)0x0000000CU)
#define GPIO_PUPDR_PUPD1_0        ((uint32_t)0x00000004U)
#define GPIO_PUPDR_PUPD1_1        ((uint32_t)0x00000008U)
#define GPIO_PUPDR_PUPD2          ((uint32_t)0x00000030U)
#define GPIO_PUPDR_PUPD2_0        ((uint32_t)0x00000010U)
#define GPIO_PUPDR_PUPD2_1        ((uint32_t)0x00000020U)
#define GPIO_PUPDR_PUPD3          ((uint32_t)0x000000C0U)
#define GPIO_PUPDR_PUPD3_0        ((uint32_t)0x00000040U)
#define GPIO_PUPDR_PUPD3_1        ((uint32_t)0x00000080U)
#define GPIO_PUPDR_PUPD4          ((uint32_t)0x00000300U)
#define GPIO_PUPDR_PUPD4_0        ((uint32_t)0x00000100U)
#define GPIO_PUPDR_PUPD4_1        ((uint32_t)0x00000200U)
#define GPIO_PUPDR_PUPD5          ((uint32_t)0x00000C00U)
#define GPIO_PUPDR_PUPD5_0        ((uint32_t)0x00000400U)
#define GPIO_PUPDR_PUPD5_1        ((uint32_t)0x00000800U)
#define GPIO_PUPDR_PUPD6          ((uint32_t)0x00003000U)
#define GPIO_PUPDR_PUPD6_0        ((uint32_t)0x00001000U)
#define GPIO_PUPDR_PUPD6_1        ((uint32_t)0x00002000U)
#define GPIO_PUPDR_PUPD7          ((uint32_t)0x0000C000U)
#define GPIO_PUPDR_PUPD7_0        ((uint32_t)0x00004000U)
#define GPIO_PUPDR_PUPD7_1        ((uint32_t)0x00008000U)
#define GPIO_PUPDR_PUPD8          ((uint32_t)0x00030000U)
#define GPIO_PUPDR_PUPD8_0        ((uint32_t)0x00010000U)
#define GPIO_PUPDR_PUPD8_1        ((uint32_t)0x00020000U)
#define GPIO_PUPDR_PUPD9          ((uint32_t)0x000C0000U)
#define GPIO_PUPDR_PUPD9_0        ((uint32_t)0x00040000U)
#define GPIO_PUPDR_PUPD9_1        ((uint32_t)0x00080000U)
#define GPIO_PUPDR_PUPD10         ((uint32_t)0x00300000U)
#define GPIO_PUPDR_PUPD10_0       ((uint32_t)0x00100000U)
#define GPIO_PUPDR_PUPD10_1       ((uint32_t)0x00200000U)
#define GPIO_PUPDR_PUPD11         ((uint32_t)0x00C00000U)
#define GPIO_PUPDR_PUPD11_0       ((uint32_t)0x00400000U)
#define GPIO_PUPDR_PUPD11_1       ((uint32_t)0x00800000U)
#define GPIO_PUPDR_PUPD12         ((uint32_t)0x03000000U)
#define GPIO_PUPDR_PUPD12_0       ((uint32_t)0x01000000U)
#define GPIO_PUPDR_PUPD12_1       ((uint32_t)0x02000000U)
#define GPIO_PUPDR_PUPD13         ((uint32_t)0x0C000000U)
#define GPIO_PUPDR_PUPD13_0       ((uint32_t)0x04000000U)
#define GPIO_PUPDR_PUPD13_1       ((uint32_t)0x08000000U)
#define GPIO_PUPDR_PUPD14         ((uint32_t)0x30000000U)
#define GPIO_PUPDR_PUPD14_0       ((uint32_t)0x10000000U)
#define GPIO_PUPDR_PUPD14_1       ((uint32_t)0x20000000U)
#define GPIO_PUPDR_PUPD15         ((uint32_t)0xC0000000U)
#define GPIO_PUPDR_PUPD15_0       ((uint32_t)0x40000000U)
#define GPIO_PUPDR_PUPD15_1       ((uint32_t)0x80000000U)

/*******************  Bit definition for GPIO_IDR register  *******************/
#define GPIO_IDR_ID0              ((uint32_t)0x00000001U)
#define GPIO_IDR_ID1              ((uint32_t)0x00000002U)
#define GPIO_IDR_ID2              ((uint32_t)0x00000004U)
#define GPIO_IDR_ID3              ((uint32_t)0x00000008U)
#define GPIO_IDR_ID4              ((uint32_t)0x00000010U)
#define GPIO_IDR_ID5              ((uint32_t)0x00000020U)
#define GPIO_IDR_ID6              ((uint32_t)0x00000040U)
#define GPIO_IDR_ID7              ((uint32_t)0x00000080U)
#define GPIO_IDR_ID8              ((uint32_t)0x00000100U)
#define GPIO_IDR_ID9              ((uint32_t)0x00000200U)
#define GPIO_IDR_ID10             ((uint32_t)0x00000400U)
#define GPIO_IDR_ID11             ((uint32_t)0x00000800U)
#define GPIO_IDR_ID12             ((uint32_t)0x00001000U)
#define GPIO_IDR_ID13             ((uint32_t)0x00002000U)
#define GPIO_IDR_ID14             ((uint32_t)0x00004000U)
#define GPIO_IDR_ID15             ((uint32_t)0x00008000U)

/******************  Bit definition for GPIO_ODR register  ********************/
#define GPIO_ODR_OD0              ((uint32_t)0x00000001U)
#define GPIO_ODR_OD1              ((uint32_t)0x00000002U)
#define GPIO_ODR_OD2              ((uint32_t)0x00000004U)
#define GPIO_ODR_OD3              ((uint32_t)0x00000008U)
#define GPIO_ODR_OD4              ((uint32_t)0x00000010U)
#define GPIO_ODR_OD5              ((uint32_t)0x00000020U)
#define GPIO_ODR_OD6              ((uint32_t)0x00000040U)
#define GPIO_ODR_OD7              ((uint32_t)0x00000080U)
#define GPIO_ODR_OD8              ((uint32_t)0x00000100U)
#define GPIO_ODR_OD9              ((uint32_t)0x00000200U)
#define GPIO_ODR_OD10             ((uint32_t)0x00000400U)
#define GPIO_ODR_OD11             ((uint32_t)0x00000800U)
#define GPIO_ODR_OD12             ((uint32_t)0x00001000U)
#define GPIO_ODR_OD13             ((uint32_t)0x00002000U)
#define GPIO_ODR_OD14             ((uint32_t)0x00004000U)
#define GPIO_ODR_OD15             ((uint32_t)0x00008000U)

/****************** Bit definition for GPIO_BSRR register  ********************/
#define GPIO_BSRR_BS_0            ((uint32_t)0x00000001U)
#define GPIO_BSRR_BS_1            ((uint32_t)0x00000002U)
#define GPIO_BSRR_BS_2            ((uint32_t)0x00000004U)
#define GPIO_BSRR_BS_3            ((uint32_t)0x00000008U)
#define GPIO_BSRR_BS_4            ((uint32_t)0x00000010U)
#define GPIO_BSRR_BS_5            ((uint32_t)0x00000020U)
#define GPIO_BSRR_BS_6            ((uint32_t)0x00000040U)
#define GPIO_BSRR_BS_7            ((uint32_t)0x00000080U)
#define GPIO_BSRR_BS_8            ((uint32_t)0x00000100U)
#define GPIO_BSRR_BS_9            ((uint32_t)0x00000200U)
#define GPIO_BSRR_BS_10           ((uint32_t)0x00000400U)
#define GPIO_BSRR_BS_11           ((uint32_t)0x00000800U)
#define GPIO_BSRR_BS_12           ((uint32_t)0x00001000U)
#define GPIO_BSRR_BS_13           ((uint32_t)0x00002000U)
#define GPIO_BSRR_BS_14           ((uint32_t)0x00004000U)
#define GPIO_BSRR_BS_15           ((uint32_t)0x00008000U)
#define GPIO_BSRR_BR_0            ((uint32_t)0x00010000U)
#define GPIO_BSRR_BR_1            ((uint32_t)0x00020000U)
#define GPIO_BSRR_BR_2            ((uint32_t)0x00040000U)
#define GPIO_BSRR_BR_3            ((uint32_t)0x00080000U)
#define GPIO_BSRR_BR_4            ((uint32_t)0x00100000U)
#define GPIO_BSRR_BR_5            ((uint32_t)0x00200000U)
#define GPIO_BSRR_BR_6            ((uint32_t)0x00400000U)
#define GPIO_BSRR_BR_7            ((uint32_t)0x00800000U)
#define GPIO_BSRR_BR_8            ((uint32_t)0x01000000U)
#define GPIO_BSRR_BR_9            ((uint32_t)0x02000000U)
#define GPIO_BSRR_BR_10           ((uint32_t)0x04000000U)
#define GPIO_BSRR_BR_11           ((uint32_t)0x08000000U)
#define GPIO_BSRR_BR_12           ((uint32_t)0x10000000U)
#define GPIO_BSRR_BR_13           ((uint32_t)0x20000000U)
#define GPIO_BSRR_BR_14           ((uint32_t)0x40000000U)
#define GPIO_BSRR_BR_15           ((uint32_t)0x80000000U)

/****************** Bit definition for GPIO_LCKR register  ********************/
#define GPIO_LCKR_LCK0            ((uint32_t)0x00000001U)
#define GPIO_LCKR_LCK1            ((uint32_t)0x00000002U)
#define GPIO_LCKR_LCK2            ((uint32_t)0x00000004U)
#define GPIO_LCKR_LCK3            ((uint32_t)0x00000008U)
#define GPIO_LCKR_LCK4            ((uint32_t)0x00000010U)
#define GPIO_LCKR_LCK5            ((uint32_t)0x00000020U)
#define GPIO_LCKR_LCK6            ((uint32_t)0x00000040U)
#define GPIO_LCKR_LCK7            ((uint32_t)0x00000080U)
#define GPIO_LCKR_LCK8            ((uint32_t)0x00000100U)
#define GPIO_LCKR_LCK9            ((uint32_t)0x00000200U)
#define GPIO_LCKR_LCK10           ((uint32_t)0x00000400U)
#define GPIO_LCKR_LCK11           ((uint32_t)0x00000800U)
#define GPIO_LCKR_LCK12           ((uint32_t)0x00001000U)
#define GPIO_LCKR_LCK13           ((uint32_t)0x00002000U)
#define GPIO_LCKR_LCK14           ((uint32_t)0x00004000U)
#define GPIO_LCKR_LCK15           ((uint32_t)0x00008000U)
#define GPIO_LCKR_LCKK            ((uint32_t)0x00010000U)

/****************** Bit definition for GPIO_AFRL register ********************/
#define GPIO_AFRL_AFRL0           ((uint32_t)0x0000000FU)
#define GPIO_AFRL_AFRL1           ((uint32_t)0x000000F0U)
#define GPIO_AFRL_AFRL2           ((uint32_t)0x00000F00U)
#define GPIO_AFRL_AFRL3           ((uint32_t)0x0000F000U)
#define GPIO_AFRL_AFRL4           ((uint32_t)0x000F0000U)
#define GPIO_AFRL_AFRL5           ((uint32_t)0x00F00000U)
#define GPIO_AFRL_AFRL6           ((uint32_t)0x0F000000U)
#define GPIO_AFRL_AFRL7           ((uint32_t)0xF0000000U)

/****************** Bit definition for GPIO_AFRH register ********************/
#define GPIO_AFRH_AFRH0           ((uint32_t)0x0000000FU)
#define GPIO_AFRH_AFRH1           ((uint32_t)0x000000F0U)
#define GPIO_AFRH_AFRH2           ((uint32_t)0x00000F00U)
#define GPIO_AFRH_AFRH3           ((uint32_t)0x0000F000U)
#define GPIO_AFRH_AFRH4           ((uint32_t)0x000F0000U)
#define GPIO_AFRH_AFRH5           ((uint32_t)0x00F00000U)
#define GPIO_AFRH_AFRH6           ((uint32_t)0x0F000000U)
#define GPIO_AFRH_AFRH7           ((uint32_t)0xF0000000U)

/****************** Bit definition for GPIO_BRR register  *********************/
#define GPIO_BRR_BR_0             ((uint32_t)0x00000001U)
#define GPIO_BRR_BR_1             ((uint32_t)0x00000002U)
#define GPIO_BRR_BR_2             ((uint32_t)0x00000004U)
#define GPIO_BRR_BR_3             ((uint32_t)0x00000008U)
#define GPIO_BRR_BR_4             ((uint32_t)0x00000010U)
#define GPIO_BRR_BR_5             ((uint32_t)0x00000020U)
#define GPIO_BRR_BR_6             ((uint32_t)0x00000040U)
#define GPIO_BRR_BR_7             ((uint32_t)0x00000080U)
#define GPIO_BRR_BR_8             ((uint32_t)0x00000100U)
#define GPIO_BRR_BR_9             ((uint32_t)0x00000200U)
#define GPIO_BRR_BR_10            ((uint32_t)0x00000400U)
#define GPIO_BRR_BR_11            ((uint32_t)0x00000800U)
#define GPIO_BRR_BR_12            ((uint32_t)0x00001000U)
#define GPIO_BRR_BR_13            ((uint32_t)0x00002000U)
#define GPIO_BRR_BR_14            ((uint32_t)0x00004000U)
#define GPIO_BRR_BR_15            ((uint32_t)0x00008000U)

/******************************************************************************/
/*                                                                            */
/*                   Inter-integrated Circuit Interface (I2C)                 */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for I2C_CR1 register  *******************/
#define I2C_CR1_PE                          ((uint32_t)0x00000001U)        /*!< Peripheral enable */
#define I2C_CR1_TXIE                        ((uint32_t)0x00000002U)        /*!< TX interrupt enable */
#define I2C_CR1_RXIE                        ((uint32_t)0x00000004U)        /*!< RX interrupt enable */
#define I2C_CR1_ADDRIE                      ((uint32_t)0x00000008U)        /*!< Address match interrupt enable */
#define I2C_CR1_NACKIE                      ((uint32_t)0x00000010U)        /*!< NACK received interrupt enable */
#define I2C_CR1_STOPIE                      ((uint32_t)0x00000020U)        /*!< STOP detection interrupt enable */
#define I2C_CR1_TCIE                        ((uint32_t)0x00000040U)        /*!< Transfer complete interrupt enable */
#define I2C_CR1_ERRIE                       ((uint32_t)0x00000080U)        /*!< Errors interrupt enable */
#define I2C_CR1_DNF                         ((uint32_t)0x00000F00U)        /*!< Digital noise filter */
#define I2C_CR1_ANFOFF                      ((uint32_t)0x00001000U)        /*!< Analog noise filter OFF */
#define I2C_CR1_TXDMAEN                     ((uint32_t)0x00004000U)        /*!< DMA transmission requests enable */
#define I2C_CR1_RXDMAEN                     ((uint32_t)0x00008000U)        /*!< DMA reception requests enable */
#define I2C_CR1_SBC                         ((uint32_t)0x00010000U)        /*!< Slave byte control */
#define I2C_CR1_NOSTRETCH                   ((uint32_t)0x00020000U)        /*!< Clock stretching disable */
#define I2C_CR1_WUPEN                       ((uint32_t)0x00040000U)        /*!< Wakeup from STOP enable */
#define I2C_CR1_GCEN                        ((uint32_t)0x00080000U)        /*!< General call enable */
#define I2C_CR1_SMBHEN                      ((uint32_t)0x00100000U)        /*!< SMBus host address enable */
#define I2C_CR1_SMBDEN                      ((uint32_t)0x00200000U)        /*!< SMBus device default address enable */
#define I2C_CR1_ALERTEN                     ((uint32_t)0x00400000U)        /*!< SMBus alert enable */
#define I2C_CR1_PECEN                       ((uint32_t)0x00800000U)        /*!< PEC enable */

/******************  Bit definition for I2C_CR2 register  ********************/
#define I2C_CR2_SADD                        ((uint32_t)0x000003FFU)        /*!< Slave address (master mode) */
#define I2C_CR2_RD_WRN                      ((uint32_t)0x00000400U)        /*!< Transfer direction (master mode) */
#define I2C_CR2_ADD10                       ((uint32_t)0x00000800U)        /*!< 10-bit addressing mode (master mode) */
#define I2C_CR2_HEAD10R                     ((uint32_t)0x00001000U)        /*!< 10-bit address header only read direction (master mode) */
#define I2C_CR2_START                       ((uint32_t)0x00002000U)        /*!< START generation */
#define I2C_CR2_STOP                        ((uint32_t)0x00004000U)        /*!< STOP generation (master mode) */
#define I2C_CR2_NACK                        ((uint32_t)0x00008000U)        /*!< NACK generation (slave mode) */
#define I2C_CR2_NBYTES                      ((uint32_t)0x00FF0000U)        /*!< Number of bytes */
#define I2C_CR2_RELOAD                      ((uint32_t)0x01000000U)        /*!< NBYTES reload mode */
#define I2C_CR2_AUTOEND                     ((uint32_t)0x02000000U)        /*!< Automatic end mode (master mode) */
#define I2C_CR2_PECBYTE                     ((uint32_t)0x04000000U)        /*!< Packet error checking byte */

/*******************  Bit definition for I2C_OAR1 register  ******************/
#define I2C_OAR1_OA1                        ((uint32_t)0x000003FFU)        /*!< Interface own address 1 */
#define I2C_OAR1_OA1MODE                    ((uint32_t)0x00000400U)        /*!< Own address 1 10-bit mode */
#define I2C_OAR1_OA1EN                      ((uint32_t)0x00008000U)        /*!< Own address 1 enable */

/*******************  Bit definition for I2C_OAR2 register  ******************/
#define  I2C_OAR2_OA2                        ((uint32_t)0x000000FEU)        /*!< Interface own address 2                        */
#define  I2C_OAR2_OA2MSK                     ((uint32_t)0x00000700U)        /*!< Own address 2 masks                            */
#define  I2C_OAR2_OA2NOMASK                  ((uint32_t)0x00000000U)        /*!< No mask                                        */
#define  I2C_OAR2_OA2MASK01                  ((uint32_t)0x00000100U)        /*!< OA2[1] is masked, Only OA2[7:2] are compared   */
#define  I2C_OAR2_OA2MASK02                  ((uint32_t)0x00000200U)        /*!< OA2[2:1] is masked, Only OA2[7:3] are compared */
#define  I2C_OAR2_OA2MASK03                  ((uint32_t)0x00000300U)        /*!< OA2[3:1] is masked, Only OA2[7:4] are compared */
#define  I2C_OAR2_OA2MASK04                  ((uint32_t)0x00000400U)        /*!< OA2[4:1] is masked, Only OA2[7:5] are compared */
#define  I2C_OAR2_OA2MASK05                  ((uint32_t)0x00000500U)        /*!< OA2[5:1] is masked, Only OA2[7:6] are compared */
#define  I2C_OAR2_OA2MASK06                  ((uint32_t)0x00000600U)        /*!< OA2[6:1] is masked, Only OA2[7] are compared   */
#define  I2C_OAR2_OA2MASK07                  ((uint32_t)0x00000700U)        /*!< OA2[7:1] is masked, No comparison is done      */
#define  I2C_OAR2_OA2EN                      ((uint32_t)0x00008000U)        /*!< Own address 2 enable                           */

/*******************  Bit definition for I2C_TIMINGR register *******************/
#define I2C_TIMINGR_SCLL                    ((uint32_t)0x000000FFU)        /*!< SCL low period (master mode) */
#define I2C_TIMINGR_SCLH                    ((uint32_t)0x0000FF00U)        /*!< SCL high period (master mode) */
#define I2C_TIMINGR_SDADEL                  ((uint32_t)0x000F0000U)        /*!< Data hold time */
#define I2C_TIMINGR_SCLDEL                  ((uint32_t)0x00F00000U)        /*!< Data setup time */
#define I2C_TIMINGR_PRESC                   ((uint32_t)0xF0000000U)        /*!< Timings prescaler */

/******************* Bit definition for I2C_TIMEOUTR register *******************/
#define I2C_TIMEOUTR_TIMEOUTA               ((uint32_t)0x00000FFFU)        /*!< Bus timeout A */
#define I2C_TIMEOUTR_TIDLE                  ((uint32_t)0x00001000U)        /*!< Idle clock timeout detection */
#define I2C_TIMEOUTR_TIMOUTEN               ((uint32_t)0x00008000U)        /*!< Clock timeout enable */
#define I2C_TIMEOUTR_TIMEOUTB               ((uint32_t)0x0FFF0000U)        /*!< Bus timeout B*/
#define I2C_TIMEOUTR_TEXTEN                 ((uint32_t)0x80000000U)        /*!< Extended clock timeout enable */

/******************  Bit definition for I2C_ISR register  *********************/
#define I2C_ISR_TXE                         ((uint32_t)0x00000001U)        /*!< Transmit data register empty */
#define I2C_ISR_TXIS                        ((uint32_t)0x00000002U)        /*!< Transmit interrupt status */
#define I2C_ISR_RXNE                        ((uint32_t)0x00000004U)        /*!< Receive data register not empty */
#define I2C_ISR_ADDR                        ((uint32_t)0x00000008U)        /*!< Address matched (slave mode)*/
#define I2C_ISR_NACKF                       ((uint32_t)0x00000010U)        /*!< NACK received flag */
#define I2C_ISR_STOPF                       ((uint32_t)0x00000020U)        /*!< STOP detection flag */
#define I2C_ISR_TC                          ((uint32_t)0x00000040U)        /*!< Transfer complete (master mode) */
#define I2C_ISR_TCR                         ((uint32_t)0x00000080U)        /*!< Transfer complete reload */
#define I2C_ISR_BERR                        ((uint32_t)0x00000100U)        /*!< Bus error */
#define I2C_ISR_ARLO                        ((uint32_t)0x00000200U)        /*!< Arbitration lost */
#define I2C_ISR_OVR                         ((uint32_t)0x00000400U)        /*!< Overrun/Underrun */
#define I2C_ISR_PECERR                      ((uint32_t)0x00000800U)        /*!< PEC error in reception */
#define I2C_ISR_TIMEOUT                     ((uint32_t)0x00001000U)        /*!< Timeout or Tlow detection flag */
#define I2C_ISR_ALERT                       ((uint32_t)0x00002000U)        /*!< SMBus alert */
#define I2C_ISR_BUSY                        ((uint32_t)0x00008000U)        /*!< Bus busy */
#define I2C_ISR_DIR                         ((uint32_t)0x00010000U)        /*!< Transfer direction (slave mode) */
#define I2C_ISR_ADDCODE                     ((uint32_t)0x00FE0000U)        /*!< Address match code (slave mode) */

/******************  Bit definition for I2C_ICR register  *********************/
#define I2C_ICR_ADDRCF                      ((uint32_t)0x00000008U)        /*!< Address matched clear flag */
#define I2C_ICR_NACKCF                      ((uint32_t)0x00000010U)        /*!< NACK clear flag */
#define I2C_ICR_STOPCF                      ((uint32_t)0x00000020U)        /*!< STOP detection clear flag */
#define I2C_ICR_BERRCF                      ((uint32_t)0x00000100U)        /*!< Bus error clear flag */
#define I2C_ICR_ARLOCF                      ((uint32_t)0x00000200U)        /*!< Arbitration lost clear flag */
#define I2C_ICR_OVRCF                       ((uint32_t)0x00000400U)        /*!< Overrun/Underrun clear flag */
#define I2C_ICR_PECCF                       ((uint32_t)0x00000800U)        /*!< PAC error clear flag */
#define I2C_ICR_TIMOUTCF                    ((uint32_t)0x00001000U)        /*!< Timeout clear flag */
#define I2C_ICR_ALERTCF                     ((uint32_t)0x00002000U)        /*!< Alert clear flag */

/******************  Bit definition for I2C_PECR register  *********************/
#define I2C_PECR_PEC                        ((uint32_t)0x000000FFU)       /*!< PEC register */

/******************  Bit definition for I2C_RXDR register  *********************/
#define I2C_RXDR_RXDATA                     ((uint32_t)0x000000FFU)       /*!< 8-bit receive data */

/******************  Bit definition for I2C_TXDR register  *********************/
#define I2C_TXDR_TXDATA                     ((uint32_t)0x000000FFU)       /*!< 8-bit transmit data */

/******************************************************************************/
/*                                                                            */
/*                        Independent WATCHDOG (IWDG)                         */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for IWDG_KR register  ********************/
#define IWDG_KR_KEY                         ((uint32_t)0x0000FFFFU)       /*!< Key value (write only, read 0000h) */

/*******************  Bit definition for IWDG_PR register  ********************/
#define IWDG_PR_PR                          ((uint32_t)0x00000007U)       /*!< PR[2:0] (Prescaler divider) */
#define IWDG_PR_PR_0                        ((uint32_t)0x00000001U)       /*!< Bit 0 */
#define IWDG_PR_PR_1                        ((uint32_t)0x00000002U)       /*!< Bit 1 */
#define IWDG_PR_PR_2                        ((uint32_t)0x00000004U)       /*!< Bit 2 */

/*******************  Bit definition for IWDG_RLR register  *******************/
#define IWDG_RLR_RL                         ((uint32_t)0x00000FFFU)       /*!< Watchdog counter reload value */

/*******************  Bit definition for IWDG_SR register  ********************/
#define IWDG_SR_PVU                         ((uint32_t)0x00000001U)       /*!< Watchdog prescaler value update */
#define IWDG_SR_RVU                         ((uint32_t)0x00000002U)       /*!< Watchdog counter reload value update */
#define IWDG_SR_WVU                         ((uint32_t)0x00000004U)       /*!< Watchdog counter window value update */

/*******************  Bit definition for IWDG_KR register  ********************/
#define IWDG_WINR_WIN                       ((uint32_t)0x00000FFFU)       /*!< Watchdog counter window value */

/******************************************************************************/
/*                                                                            */
/*                         Low Power Timer (LPTTIM)                           */
/*                                                                            */
/******************************************************************************/
/******************  Bit definition for LPTIM_ISR register  *******************/
#define LPTIM_ISR_CMPM                         ((uint32_t)0x00000001U)            /*!< Compare match */
#define LPTIM_ISR_ARRM                         ((uint32_t)0x00000002U)            /*!< Autoreload match */
#define LPTIM_ISR_EXTTRIG                      ((uint32_t)0x00000004U)            /*!< External trigger edge event */
#define LPTIM_ISR_CMPOK                        ((uint32_t)0x00000008U)            /*!< Compare register update OK */
#define LPTIM_ISR_ARROK                        ((uint32_t)0x00000010U)            /*!< Autoreload register update OK */
#define LPTIM_ISR_UP                           ((uint32_t)0x00000020U)            /*!< Counter direction change down to up */
#define LPTIM_ISR_DOWN                         ((uint32_t)0x00000040U)            /*!< Counter direction change up to down */

/******************  Bit definition for LPTIM_ICR register  *******************/
#define LPTIM_ICR_CMPMCF                       ((uint32_t)0x00000001U)            /*!< Compare match Clear Flag */
#define LPTIM_ICR_ARRMCF                       ((uint32_t)0x00000002U)            /*!< Autoreload match Clear Flag */
#define LPTIM_ICR_EXTTRIGCF                    ((uint32_t)0x00000004U)            /*!< External trigger edge event Clear Flag */
#define LPTIM_ICR_CMPOKCF                      ((uint32_t)0x00000008U)            /*!< Compare register update OK Clear Flag */
#define LPTIM_ICR_ARROKCF                      ((uint32_t)0x00000010U)            /*!< Autoreload register update OK Clear Flag */
#define LPTIM_ICR_UPCF                         ((uint32_t)0x00000020U)            /*!< Counter direction change down to up Clear Flag */
#define LPTIM_ICR_DOWNCF                       ((uint32_t)0x00000040U)            /*!< Counter direction change up to down Clear Flag */

/******************  Bit definition for LPTIM_IER register ********************/
#define LPTIM_IER_CMPMIE                       ((uint32_t)0x00000001U)            /*!< Compare match Interrupt Enable */
#define LPTIM_IER_ARRMIE                       ((uint32_t)0x00000002U)            /*!< Autoreload match Interrupt Enable */
#define LPTIM_IER_EXTTRIGIE                    ((uint32_t)0x00000004U)            /*!< External trigger edge event Interrupt Enable */
#define LPTIM_IER_CMPOKIE                      ((uint32_t)0x00000008U)            /*!< Compare register update OK Interrupt Enable */
#define LPTIM_IER_ARROKIE                      ((uint32_t)0x00000010U)            /*!< Autoreload register update OK Interrupt Enable */
#define LPTIM_IER_UPIE                         ((uint32_t)0x00000020U)            /*!< Counter direction change down to up Interrupt Enable */
#define LPTIM_IER_DOWNIE                       ((uint32_t)0x00000040U)            /*!< Counter direction change up to down Interrupt Enable */

/******************  Bit definition for LPTIM_CFGR register *******************/
#define LPTIM_CFGR_CKSEL                       ((uint32_t)0x00000001U)             /*!< Clock selector */

#define LPTIM_CFGR_CKPOL                       ((uint32_t)0x00000006U)             /*!< CKPOL[1:0] bits (Clock polarity) */
#define LPTIM_CFGR_CKPOL_0                     ((uint32_t)0x00000002U)             /*!< Bit 0 */
#define LPTIM_CFGR_CKPOL_1                     ((uint32_t)0x00000004U)             /*!< Bit 1 */

#define LPTIM_CFGR_CKFLT                       ((uint32_t)0x00000018U)             /*!< CKFLT[1:0] bits (Configurable digital filter for external clock) */
#define LPTIM_CFGR_CKFLT_0                     ((uint32_t)0x00000008U)             /*!< Bit 0 */
#define LPTIM_CFGR_CKFLT_1                     ((uint32_t)0x00000010U)             /*!< Bit 1 */

#define LPTIM_CFGR_TRGFLT                      ((uint32_t)0x000000C0U)             /*!< TRGFLT[1:0] bits (Configurable digital filter for trigger) */
#define LPTIM_CFGR_TRGFLT_0                    ((uint32_t)0x00000040U)             /*!< Bit 0 */
#define LPTIM_CFGR_TRGFLT_1                    ((uint32_t)0x00000080U)             /*!< Bit 1 */

#define LPTIM_CFGR_PRESC                       ((uint32_t)0x00000E00U)             /*!< PRESC[2:0] bits (Clock prescaler) */
#define LPTIM_CFGR_PRESC_0                     ((uint32_t)0x00000200U)             /*!< Bit 0 */
#define LPTIM_CFGR_PRESC_1                     ((uint32_t)0x00000400U)             /*!< Bit 1 */
#define LPTIM_CFGR_PRESC_2                     ((uint32_t)0x00000800U)             /*!< Bit 2 */

#define LPTIM_CFGR_TRIGSEL                     ((uint32_t)0x0000E000U)             /*!< TRIGSEL[2:0]] bits (Trigger selector) */
#define LPTIM_CFGR_TRIGSEL_0                   ((uint32_t)0x00002000U)             /*!< Bit 0 */
#define LPTIM_CFGR_TRIGSEL_1                   ((uint32_t)0x00004000U)             /*!< Bit 1 */
#define LPTIM_CFGR_TRIGSEL_2                   ((uint32_t)0x00008000U)             /*!< Bit 2 */

#define LPTIM_CFGR_TRIGEN                      ((uint32_t)0x00060000U)             /*!< TRIGEN[1:0] bits (Trigger enable and polarity) */
#define LPTIM_CFGR_TRIGEN_0                    ((uint32_t)0x00020000U)             /*!< Bit 0 */
#define LPTIM_CFGR_TRIGEN_1                    ((uint32_t)0x00040000U)             /*!< Bit 1 */

#define LPTIM_CFGR_TIMOUT                      ((uint32_t)0x00080000U)             /*!< Timout enable */
#define LPTIM_CFGR_WAVE                        ((uint32_t)0x00100000U)             /*!< Waveform shape */
#define LPTIM_CFGR_WAVPOL                      ((uint32_t)0x00200000U)             /*!< Waveform shape polarity */
#define LPTIM_CFGR_PRELOAD                     ((uint32_t)0x00400000U)             /*!< Reg update mode */
#define LPTIM_CFGR_COUNTMODE                   ((uint32_t)0x00800000U)             /*!< Counter mode enable */
#define LPTIM_CFGR_ENC                         ((uint32_t)0x01000000U)             /*!< Encoder mode enable */

/******************  Bit definition for LPTIM_CR register  ********************/
#define LPTIM_CR_ENABLE                        ((uint32_t)0x00000001U)             /*!< LPTIMer enable */
#define LPTIM_CR_SNGSTRT                       ((uint32_t)0x00000002U)             /*!< Timer start in single mode */
#define LPTIM_CR_CNTSTRT                       ((uint32_t)0x00000004U)             /*!< Timer start in continuous mode */

/******************  Bit definition for LPTIM_CMP register  *******************/
#define LPTIM_CMP_CMP                          ((uint32_t)0x0000FFFFU)             /*!< Compare register */

/******************  Bit definition for LPTIM_ARR register  *******************/
#define LPTIM_ARR_ARR                          ((uint32_t)0x0000FFFFU)             /*!< Auto reload register */

/******************  Bit definition for LPTIM_CNT register  *******************/
#define LPTIM_CNT_CNT                          ((uint32_t)0x0000FFFFU)             /*!< Counter register */

/******************************************************************************/
/*                                                                            */
/*                          Power Control (PWR)                               */
/*                                                                            */
/******************************************************************************/

/********************  Bit definition for PWR_CR register  ********************/
#define PWR_CR_LPSDSR                       ((uint32_t)0x00000001U)     /*!< Low-power deepsleep/sleep/low power run */
#define PWR_CR_PDDS                         ((uint32_t)0x00000002U)     /*!< Power Down Deepsleep */
#define PWR_CR_CWUF                         ((uint32_t)0x00000004U)     /*!< Clear Wakeup Flag */
#define PWR_CR_CSBF                         ((uint32_t)0x00000008U)     /*!< Clear Standby Flag */
#define PWR_CR_PVDE                         ((uint32_t)0x00000010U)     /*!< Power Voltage Detector Enable */

#define PWR_CR_PLS                          ((uint32_t)0x000000E0U)     /*!< PLS[2:0] bits (PVD Level Selection) */
#define PWR_CR_PLS_0                        ((uint32_t)0x00000020U)     /*!< Bit 0 */
#define PWR_CR_PLS_1                        ((uint32_t)0x00000040U)     /*!< Bit 1 */
#define PWR_CR_PLS_2                        ((uint32_t)0x00000080U)     /*!< Bit 2 */

/*!< PVD level configuration */
#define PWR_CR_PLS_LEV0                     ((uint32_t)0x00000000U)     /*!< PVD level 0 */
#define PWR_CR_PLS_LEV1                     ((uint32_t)0x00000020U)     /*!< PVD level 1 */
#define PWR_CR_PLS_LEV2                     ((uint32_t)0x00000040U)     /*!< PVD level 2 */
#define PWR_CR_PLS_LEV3                     ((uint32_t)0x00000060U)     /*!< PVD level 3 */
#define PWR_CR_PLS_LEV4                     ((uint32_t)0x00000080U)     /*!< PVD level 4 */
#define PWR_CR_PLS_LEV5                     ((uint32_t)0x000000A0U)     /*!< PVD level 5 */
#define PWR_CR_PLS_LEV6                     ((uint32_t)0x000000C0U)     /*!< PVD level 6 */
#define PWR_CR_PLS_LEV7                     ((uint32_t)0x000000E0U)     /*!< PVD level 7 */

#define PWR_CR_DBP                          ((uint32_t)0x00000100U)     /*!< Disable Backup Domain write protection */
#define PWR_CR_ULP                          ((uint32_t)0x00000200U)     /*!< Ultra Low Power mode */
#define PWR_CR_FWU                          ((uint32_t)0x00000400U)     /*!< Fast wakeup */

#define PWR_CR_VOS                          ((uint32_t)0x00001800U)     /*!< VOS[1:0] bits (Voltage scaling range selection) */
#define PWR_CR_VOS_0                        ((uint32_t)0x00000800U)     /*!< Bit 0 */
#define PWR_CR_VOS_1                        ((uint32_t)0x00001000U)     /*!< Bit 1 */
#define PWR_CR_DSEEKOFF                     ((uint32_t)0x00002000U)     /*!< Deep Sleep mode with EEPROM kept Off */
#define PWR_CR_LPRUN                        ((uint32_t)0x00004000U)     /*!< Low power run mode */
#define PWR_CR_LPDS                         ((uint32_t)0x00010000U)     /*!< regulator in low power deepsleep mode */

/*******************  Bit definition for PWR_CSR register  ********************/
#define PWR_CSR_WUF                         ((uint32_t)0x00000001U)     /*!< Wakeup Flag */
#define PWR_CSR_SBF                         ((uint32_t)0x00000002U)     /*!< Standby Flag */
#define PWR_CSR_PVDO                        ((uint32_t)0x00000004U)     /*!< PVD Output */
#define PWR_CSR_VREFINTRDYF                 ((uint32_t)0x00000008U)     /*!< Internal voltage reference (VREFINT) ready flag */
#define PWR_CSR_VOSF                        ((uint32_t)0x00000010U)     /*!< Voltage Scaling select flag */
#define PWR_CSR_REGLPF                      ((uint32_t)0x00000020U)     /*!< Regulator LP flag */

#define PWR_CSR_EWUP1                       ((uint32_t)0x00000100U)     /*!< Enable WKUP pin 1 */
#define PWR_CSR_EWUP2                       ((uint32_t)0x00000200U)     /*!< Enable WKUP pin 2 */
#define PWR_CSR_EWUP3                       ((uint32_t)0x00000400U)     /*!< Enable WKUP pin 3 */

/******************************************************************************/
/*                                                                            */
/*                         Reset and Clock Control                            */
/*                                                                            */
/******************************************************************************/

/********************  Bit definition for RCC_CR register  ********************/
#define RCC_CR_HSION                        ((uint32_t)0x00000001U)        /*!< Internal High Speed clock enable */
#define RCC_CR_HSIKERON                     ((uint32_t)0x00000002U)        /*!< Internal High Speed clock enable for some IPs Kernel */
#define RCC_CR_HSIRDY                       ((uint32_t)0x00000004U)        /*!< Internal High Speed clock ready flag */
#define RCC_CR_HSIDIVEN                     ((uint32_t)0x00000008U)        /*!< Internal High Speed clock divider enable */
#define RCC_CR_HSIDIVF                      ((uint32_t)0x00000010U)        /*!< Internal High Speed clock divider flag */
#define RCC_CR_HSIOUTEN                     ((uint32_t)0x00000020U)        /*!< Internal High Speed clock out enable */
#define RCC_CR_MSION                        ((uint32_t)0x00000100U)        /*!< Internal Multi Speed clock enable */
#define RCC_CR_MSIRDY                       ((uint32_t)0x00000200U)        /*!< Internal Multi Speed clock ready flag */
#define RCC_CR_HSEON                        ((uint32_t)0x00010000U)        /*!< External High Speed clock enable */
#define RCC_CR_HSERDY                       ((uint32_t)0x00020000U)        /*!< External High Speed clock ready flag */
#define RCC_CR_HSEBYP                       ((uint32_t)0x00040000U)        /*!< External High Speed clock Bypass */
#define RCC_CR_RTCPRE                       ((uint32_t)0x00300000U)        /*!< RTC prescaler [1:0] bits */
#define RCC_CR_RTCPRE_0                     ((uint32_t)0x00100000U)        /*!< RTC prescaler Bit 0 */
#define RCC_CR_RTCPRE_1                     ((uint32_t)0x00200000U)        /*!< RTC prescaler Bit 1 */
#define RCC_CR_PLLON                        ((uint32_t)0x01000000U)        /*!< PLL enable */
#define RCC_CR_PLLRDY                       ((uint32_t)0x02000000U)        /*!< PLL clock ready flag */

/* Reference defines */

/********************  Bit definition for RCC_ICSCR register  *****************/
#define RCC_ICSCR_HSICAL                    ((uint32_t)0x000000FFU)        /*!< Internal High Speed clock Calibration */
#define RCC_ICSCR_HSITRIM                   ((uint32_t)0x00001F00U)        /*!< Internal High Speed clock trimming */

#define RCC_ICSCR_MSIRANGE                  ((uint32_t)0x0000E000U)        /*!< Internal Multi Speed clock Range */
#define RCC_ICSCR_MSIRANGE_0                ((uint32_t)0x00000000U)        /*!< Internal Multi Speed clock Range 65.536 KHz */
#define RCC_ICSCR_MSIRANGE_1                ((uint32_t)0x00002000U)        /*!< Internal Multi Speed clock Range 131.072 KHz */
#define RCC_ICSCR_MSIRANGE_2                ((uint32_t)0x00004000U)        /*!< Internal Multi Speed clock Range 262.144 KHz */
#define RCC_ICSCR_MSIRANGE_3                ((uint32_t)0x00006000U)        /*!< Internal Multi Speed clock Range 524.288 KHz */
#define RCC_ICSCR_MSIRANGE_4                ((uint32_t)0x00008000U)        /*!< Internal Multi Speed clock Range 1.048 MHz */
#define RCC_ICSCR_MSIRANGE_5                ((uint32_t)0x0000A000U)        /*!< Internal Multi Speed clock Range 2.097 MHz */
#define RCC_ICSCR_MSIRANGE_6                ((uint32_t)0x0000C000U)        /*!< Internal Multi Speed clock Range 4.194 MHz */
#define RCC_ICSCR_MSICAL                    ((uint32_t)0x00FF0000U)        /*!< Internal Multi Speed clock Calibration */
#define RCC_ICSCR_MSITRIM                   ((uint32_t)0xFF000000U)        /*!< Internal Multi Speed clock trimming */


/*******************  Bit definition for RCC_CFGR register  *******************/
/*!< SW configuration */
#define RCC_CFGR_SW                         ((uint32_t)0x00000003U)        /*!< SW[1:0] bits (System clock Switch) */
#define RCC_CFGR_SW_0                       ((uint32_t)0x00000001U)        /*!< Bit 0 */
#define RCC_CFGR_SW_1                       ((uint32_t)0x00000002U)        /*!< Bit 1 */

#define RCC_CFGR_SW_MSI                     ((uint32_t)0x00000000U)        /*!< MSI selected as system clock */
#define RCC_CFGR_SW_HSI                     ((uint32_t)0x00000001U)        /*!< HSI selected as system clock */
#define RCC_CFGR_SW_HSE                     ((uint32_t)0x00000002U)        /*!< HSE selected as system clock */
#define RCC_CFGR_SW_PLL                     ((uint32_t)0x00000003U)        /*!< PLL selected as system clock */

/*!< SWS configuration */
#define RCC_CFGR_SWS                        ((uint32_t)0x0000000CU)        /*!< SWS[1:0] bits (System Clock Switch Status) */
#define RCC_CFGR_SWS_0                      ((uint32_t)0x00000004U)        /*!< Bit 0 */
#define RCC_CFGR_SWS_1                      ((uint32_t)0x00000008U)        /*!< Bit 1 */

#define RCC_CFGR_SWS_MSI                    ((uint32_t)0x00000000U)        /*!< MSI oscillator used as system clock */
#define RCC_CFGR_SWS_HSI                    ((uint32_t)0x00000004U)        /*!< HSI oscillator used as system clock */
#define RCC_CFGR_SWS_HSE                    ((uint32_t)0x00000008U)        /*!< HSE oscillator used as system clock */
#define RCC_CFGR_SWS_PLL                    ((uint32_t)0x0000000CU)        /*!< PLL used as system clock */

/*!< HPRE configuration */
#define RCC_CFGR_HPRE                       ((uint32_t)0x000000F0U)        /*!< HPRE[3:0] bits (AHB prescaler) */
#define RCC_CFGR_HPRE_0                     ((uint32_t)0x00000010U)        /*!< Bit 0 */
#define RCC_CFGR_HPRE_1                     ((uint32_t)0x00000020U)        /*!< Bit 1 */
#define RCC_CFGR_HPRE_2                     ((uint32_t)0x00000040U)        /*!< Bit 2 */
#define RCC_CFGR_HPRE_3                     ((uint32_t)0x00000080U)        /*!< Bit 3 */

#define RCC_CFGR_HPRE_DIV1                  ((uint32_t)0x00000000U)        /*!< SYSCLK not divided */
#define RCC_CFGR_HPRE_DIV2                  ((uint32_t)0x00000080U)        /*!< SYSCLK divided by 2 */
#define RCC_CFGR_HPRE_DIV4                  ((uint32_t)0x00000090U)        /*!< SYSCLK divided by 4 */
#define RCC_CFGR_HPRE_DIV8                  ((uint32_t)0x000000A0U)        /*!< SYSCLK divided by 8 */
#define RCC_CFGR_HPRE_DIV16                 ((uint32_t)0x000000B0U)        /*!< SYSCLK divided by 16 */
#define RCC_CFGR_HPRE_DIV64                 ((uint32_t)0x000000C0U)        /*!< SYSCLK divided by 64 */
#define RCC_CFGR_HPRE_DIV128                ((uint32_t)0x000000D0U)        /*!< SYSCLK divided by 128 */
#define RCC_CFGR_HPRE_DIV256                ((uint32_t)0x000000E0U)        /*!< SYSCLK divided by 256 */
#define RCC_CFGR_HPRE_DIV512                ((uint32_t)0x000000F0U)        /*!< SYSCLK divided by 512 */

/*!< PPRE1 configuration */
#define RCC_CFGR_PPRE1                      ((uint32_t)0x00000700U)        /*!< PRE1[2:0] bits (APB1 prescaler) */
#define RCC_CFGR_PPRE1_0                    ((uint32_t)0x00000100U)        /*!< Bit 0 */
#define RCC_CFGR_PPRE1_1                    ((uint32_t)0x00000200U)        /*!< Bit 1 */
#define RCC_CFGR_PPRE1_2                    ((uint32_t)0x00000400U)        /*!< Bit 2 */

#define RCC_CFGR_PPRE1_DIV1                 ((uint32_t)0x00000000U)        /*!< HCLK not divided */
#define RCC_CFGR_PPRE1_DIV2                 ((uint32_t)0x00000400U)        /*!< HCLK divided by 2 */
#define RCC_CFGR_PPRE1_DIV4                 ((uint32_t)0x00000500U)        /*!< HCLK divided by 4 */
#define RCC_CFGR_PPRE1_DIV8                 ((uint32_t)0x00000600U)        /*!< HCLK divided by 8 */
#define RCC_CFGR_PPRE1_DIV16                ((uint32_t)0x00000700U)        /*!< HCLK divided by 16 */

/*!< PPRE2 configuration */
#define RCC_CFGR_PPRE2                      ((uint32_t)0x00003800U)        /*!< PRE2[2:0] bits (APB2 prescaler) */
#define RCC_CFGR_PPRE2_0                    ((uint32_t)0x00000800U)        /*!< Bit 0 */
#define RCC_CFGR_PPRE2_1                    ((uint32_t)0x00001000U)        /*!< Bit 1 */
#define RCC_CFGR_PPRE2_2                    ((uint32_t)0x00002000U)        /*!< Bit 2 */

#define RCC_CFGR_PPRE2_DIV1                 ((uint32_t)0x00000000U)        /*!< HCLK not divided */
#define RCC_CFGR_PPRE2_DIV2                 ((uint32_t)0x00002000U)        /*!< HCLK divided by 2 */
#define RCC_CFGR_PPRE2_DIV4                 ((uint32_t)0x00002800U)        /*!< HCLK divided by 4 */
#define RCC_CFGR_PPRE2_DIV8                 ((uint32_t)0x00003000U)        /*!< HCLK divided by 8 */
#define RCC_CFGR_PPRE2_DIV16                ((uint32_t)0x00003800U)        /*!< HCLK divided by 16 */

#define RCC_CFGR_STOPWUCK                   ((uint32_t)0x00008000U)        /*!< Wake Up from Stop Clock selection */

/*!< PLL entry clock source*/
#define RCC_CFGR_PLLSRC                     ((uint32_t)0x00010000U)        /*!< PLL entry clock source */

#define RCC_CFGR_PLLSRC_HSI                 ((uint32_t)0x00000000U)        /*!< HSI as PLL entry clock source */
#define RCC_CFGR_PLLSRC_HSE                 ((uint32_t)0x00010000U)        /*!< HSE as PLL entry clock source */


/*!< PLLMUL configuration */
#define RCC_CFGR_PLLMUL                     ((uint32_t)0x003C0000U)        /*!< PLLMUL[3:0] bits (PLL multiplication factor) */
#define RCC_CFGR_PLLMUL_0                   ((uint32_t)0x00040000U)        /*!< Bit 0 */
#define RCC_CFGR_PLLMUL_1                   ((uint32_t)0x00080000U)        /*!< Bit 1 */
#define RCC_CFGR_PLLMUL_2                   ((uint32_t)0x00100000U)        /*!< Bit 2 */
#define RCC_CFGR_PLLMUL_3                   ((uint32_t)0x00200000U)        /*!< Bit 3 */

#define RCC_CFGR_PLLMUL3                    ((uint32_t)0x00000000U)        /*!< PLL input clock * 3 */
#define RCC_CFGR_PLLMUL4                    ((uint32_t)0x00040000U)        /*!< PLL input clock * 4 */
#define RCC_CFGR_PLLMUL6                    ((uint32_t)0x00080000U)        /*!< PLL input clock * 6 */
#define RCC_CFGR_PLLMUL8                    ((uint32_t)0x000C0000U)        /*!< PLL input clock * 8 */
#define RCC_CFGR_PLLMUL12                   ((uint32_t)0x00100000U)        /*!< PLL input clock * 12 */
#define RCC_CFGR_PLLMUL16                   ((uint32_t)0x00140000U)        /*!< PLL input clock * 16 */
#define RCC_CFGR_PLLMUL24                   ((uint32_t)0x00180000U)        /*!< PLL input clock * 24 */
#define RCC_CFGR_PLLMUL32                   ((uint32_t)0x001C0000U)        /*!< PLL input clock * 32 */
#define RCC_CFGR_PLLMUL48                   ((uint32_t)0x00200000U)        /*!< PLL input clock * 48 */

/*!< PLLDIV configuration */
#define RCC_CFGR_PLLDIV                     ((uint32_t)0x00C00000U)        /*!< PLLDIV[1:0] bits (PLL Output Division) */
#define RCC_CFGR_PLLDIV_0                   ((uint32_t)0x00400000U)        /*!< Bit0 */
#define RCC_CFGR_PLLDIV_1                   ((uint32_t)0x00800000U)        /*!< Bit1 */

#define RCC_CFGR_PLLDIV2                    ((uint32_t)0x00400000U)        /*!< PLL clock output = CKVCO / 2 */
#define RCC_CFGR_PLLDIV3                    ((uint32_t)0x00800000U)        /*!< PLL clock output = CKVCO / 3 */
#define RCC_CFGR_PLLDIV4                    ((uint32_t)0x00C00000U)        /*!< PLL clock output = CKVCO / 4 */

/*!< MCO configuration */
#define RCC_CFGR_MCOSEL                     ((uint32_t)0x0F000000U)        /*!< MCO[3:0] bits (Microcontroller Clock Output) */
#define RCC_CFGR_MCOSEL_0                   ((uint32_t)0x01000000U)        /*!< Bit 0 */
#define RCC_CFGR_MCOSEL_1                   ((uint32_t)0x02000000U)        /*!< Bit 1 */
#define RCC_CFGR_MCOSEL_2                   ((uint32_t)0x04000000U)        /*!< Bit 2 */
#define RCC_CFGR_MCOSEL_3                   ((uint32_t)0x08000000U)        /*!< Bit 3 */

#define RCC_CFGR_MCO_NOCLOCK                ((uint32_t)0x00000000U)        /*!< No clock */
#define RCC_CFGR_MCO_SYSCLK                 ((uint32_t)0x01000000U)        /*!< System clock selected as MCO source */
#define RCC_CFGR_MCO_HSI                    ((uint32_t)0x02000000U)        /*!< Internal 16 MHz RC oscillator clock selected */
#define RCC_CFGR_MCO_MSI                    ((uint32_t)0x03000000U)        /*!< Internal Medium Speed RC oscillator clock selected */
#define RCC_CFGR_MCO_HSE                    ((uint32_t)0x04000000U)        /*!< External 1-25 MHz oscillator clock selected */
#define RCC_CFGR_MCO_PLL                    ((uint32_t)0x05000000U)        /*!< PLL clock divided */
#define RCC_CFGR_MCO_LSI                    ((uint32_t)0x06000000U)        /*!< LSI selected */
#define RCC_CFGR_MCO_LSE                    ((uint32_t)0x07000000U)        /*!< LSE selected */

#define RCC_CFGR_MCOPRE                    ((uint32_t)0x70000000U)        /*!< MCO prescaler */
#define RCC_CFGR_MCOPRE_0                  ((uint32_t)0x10000000U)        /*!< MCO is divided by 2 */
#define RCC_CFGR_MCOPRE_1                  ((uint32_t)0x20000000U)        /*!< MCO is divided by 4 */
#define RCC_CFGR_MCOPRE_2                  ((uint32_t)0x40000000U)        /*!< MCO is divided by 8 */

#define RCC_CFGR_MCOPRE_DIV1               ((uint32_t)0x00000000U)        /*!< MCO is divided by 1 */
#define RCC_CFGR_MCOPRE_DIV2               ((uint32_t)0x10000000U)        /*!< MCO is divided by 2 */
#define RCC_CFGR_MCOPRE_DIV4               ((uint32_t)0x20000000U)        /*!< MCO is divided by 4 */
#define RCC_CFGR_MCOPRE_DIV8               ((uint32_t)0x30000000U)        /*!< MCO is divided by 8 */
#define RCC_CFGR_MCOPRE_DIV16              ((uint32_t)0x40000000U)        /*!< MCO is divided by 16 */

/* Legacy defines */
#define RCC_CFGR_MCO_PRE                    RCC_CFGR_MCOPRE          /*!< MCO prescaler */
#define RCC_CFGR_MCO_PRE_1                  RCC_CFGR_MCOPRE_DIV1        /*!< MCO is divided by 1 */
#define RCC_CFGR_MCO_PRE_2                  RCC_CFGR_MCOPRE_DIV2        /*!< MCO is divided by 1 */
#define RCC_CFGR_MCO_PRE_4                  RCC_CFGR_MCOPRE_DIV4        /*!< MCO is divided by 1 */
#define RCC_CFGR_MCO_PRE_8                  RCC_CFGR_MCOPRE_DIV8        /*!< MCO is divided by 1 */
#define RCC_CFGR_MCO_PRE_16                 RCC_CFGR_MCOPRE_DIV16       /*!< MCO is divided by 1 */

/*!<******************  Bit definition for RCC_CIER register  ********************/
#define RCC_CIER_LSIRDYIE                   ((uint32_t)0x00000001U)        /*!< LSI Ready Interrupt Enable */
#define RCC_CIER_LSERDYIE                   ((uint32_t)0x00000002U)        /*!< LSE Ready Interrupt Enable */
#define RCC_CIER_HSIRDYIE                   ((uint32_t)0x00000004U)        /*!< HSI Ready Interrupt Enable */
#define RCC_CIER_HSERDYIE                   ((uint32_t)0x00000008U)        /*!< HSE Ready Interrupt Enable */
#define RCC_CIER_PLLRDYIE                   ((uint32_t)0x00000010U)        /*!< PLL Ready Interrupt Enable */
#define RCC_CIER_MSIRDYIE                   ((uint32_t)0x00000020U)        /*!< MSI Ready Interrupt Enable */
#define RCC_CIER_CSSLSE                     ((uint32_t)0x00000080U)        /*!< LSE CSS Interrupt Enable */

/* Reference defines */
#define RCC_CIER_LSECSSIE                    RCC_CIER_CSSLSE

/*!<******************  Bit definition for RCC_CIFR register  ********************/
#define RCC_CIFR_LSIRDYF                    ((uint32_t)0x00000001U)        /*!< LSI Ready Interrupt flag */
#define RCC_CIFR_LSERDYF                    ((uint32_t)0x00000002U)        /*!< LSE Ready Interrupt flag */
#define RCC_CIFR_HSIRDYF                    ((uint32_t)0x00000004U)        /*!< HSI Ready Interrupt flag */
#define RCC_CIFR_HSERDYF                    ((uint32_t)0x00000008U)        /*!< HSE Ready Interrupt flag */
#define RCC_CIFR_PLLRDYF                    ((uint32_t)0x00000010U)        /*!< PLL Ready Interrupt flag */
#define RCC_CIFR_MSIRDYF                    ((uint32_t)0x00000020U)        /*!< MSI Ready Interrupt flag */
#define RCC_CIFR_CSSLSEF                    ((uint32_t)0x00000080U)        /*!< LSE Clock Security System Interrupt flag */

/* Reference defines */
#define RCC_CIFR_LSECSSF                    RCC_CIFR_CSSLSEF

/*!<******************  Bit definition for RCC_CICR register  ********************/
#define RCC_CICR_LSIRDYC                    ((uint32_t)0x00000001U)        /*!< LSI Ready Interrupt Clear */
#define RCC_CICR_LSERDYC                    ((uint32_t)0x00000002U)        /*!< LSE Ready Interrupt Clear */
#define RCC_CICR_HSIRDYC                    ((uint32_t)0x00000004U)        /*!< HSI Ready Interrupt Clear */
#define RCC_CICR_HSERDYC                    ((uint32_t)0x00000008U)        /*!< HSE Ready Interrupt Clear */
#define RCC_CICR_PLLRDYC                    ((uint32_t)0x00000010U)        /*!< PLL Ready Interrupt Clear */
#define RCC_CICR_MSIRDYC                    ((uint32_t)0x00000020U)        /*!< MSI Ready Interrupt Clear */
#define RCC_CICR_CSSLSEC                    ((uint32_t)0x00000080U)        /*!< LSE Clock Security System Interrupt Clear */

/* Reference defines */
#define RCC_CICR_LSECSSC                    RCC_CICR_CSSLSEC
/*****************  Bit definition for RCC_IOPRSTR register  ******************/
#define RCC_IOPRSTR_IOPARST                ((uint32_t)0x00000001U)        /*!< GPIO port A reset */
#define RCC_IOPRSTR_IOPBRST                ((uint32_t)0x00000002U)        /*!< GPIO port B reset */
#define RCC_IOPRSTR_IOPCRST                ((uint32_t)0x00000004U)        /*!< GPIO port C reset */

/* Reference defines */
#define RCC_IOPRSTR_GPIOARST                RCC_IOPRSTR_IOPARST        /*!< GPIO port A reset */
#define RCC_IOPRSTR_GPIOBRST                RCC_IOPRSTR_IOPBRST        /*!< GPIO port B reset */
#define RCC_IOPRSTR_GPIOCRST                RCC_IOPRSTR_IOPCRST        /*!< GPIO port C reset */


/******************  Bit definition for RCC_AHBRST register  ******************/
#define RCC_AHBRSTR_DMARST                  ((uint32_t)0x00000001U)        /*!< DMA1 reset */
#define RCC_AHBRSTR_MIFRST                  ((uint32_t)0x00000100U)        /*!< Memory interface reset reset */
#define RCC_AHBRSTR_CRCRST                  ((uint32_t)0x00001000U)        /*!< CRC reset */

/* Reference defines */
#define RCC_AHBRSTR_DMA1RST                 RCC_AHBRSTR_DMARST            /*!< DMA1 reset */

/*****************  Bit definition for RCC_APB2RSTR register  *****************/
#define RCC_APB2RSTR_SYSCFGRST              ((uint32_t)0x00000001U)        /*!< SYSCFG clock reset */
#define RCC_APB2RSTR_TIM21RST               ((uint32_t)0x00000004U)        /*!< TIM21 clock reset */
#define RCC_APB2RSTR_ADCRST                 ((uint32_t)0x00000200U)        /*!< ADC1 clock reset */
#define RCC_APB2RSTR_SPI1RST                ((uint32_t)0x00001000U)        /*!< SPI1 clock reset */
#define RCC_APB2RSTR_DBGRST                 ((uint32_t)0x00400000U)        /*!< DBGMCU clock reset */

/* Reference defines */
#define RCC_APB2RSTR_ADC1RST                RCC_APB2RSTR_ADCRST           /*!< ADC1 clock reset */
#define RCC_APB2RSTR_DBGMCURST              RCC_APB2RSTR_DBGRST           /*!< DBGMCU clock reset */

/*****************  Bit definition for RCC_APB1RSTR register  *****************/
#define RCC_APB1RSTR_TIM2RST                ((uint32_t)0x00000001U)        /*!< Timer 2 clock reset */
#define RCC_APB1RSTR_WWDGRST                ((uint32_t)0x00000800U)        /*!< Window Watchdog clock reset */
#define RCC_APB1RSTR_USART2RST              ((uint32_t)0x00020000U)        /*!< USART 2 clock reset */
#define RCC_APB1RSTR_LPUART1RST             ((uint32_t)0x00040000U)        /*!< LPUART1 clock reset */
#define RCC_APB1RSTR_I2C1RST                ((uint32_t)0x00200000U)        /*!< I2C 1 clock reset */
#define RCC_APB1RSTR_PWRRST                 ((uint32_t)0x10000000U)        /*!< PWR clock reset */
#define RCC_APB1RSTR_LPTIM1RST              ((uint32_t)0x80000000U)        /*!< LPTIM1 clock reset */

/*****************  Bit definition for RCC_IOPENR register  ******************/
#define RCC_IOPENR_IOPAEN                  ((uint32_t)0x00000001U)        /*!< GPIO port A clock enable */
#define RCC_IOPENR_IOPBEN                  ((uint32_t)0x00000002U)        /*!< GPIO port B clock enable */
#define RCC_IOPENR_IOPCEN                  ((uint32_t)0x00000004U)        /*!< GPIO port C clock enable */

/* Reference defines */
#define RCC_IOPENR_GPIOAEN                  RCC_IOPENR_IOPAEN        /*!< GPIO port A clock enable */
#define RCC_IOPENR_GPIOBEN                  RCC_IOPENR_IOPBEN        /*!< GPIO port B clock enable */
#define RCC_IOPENR_GPIOCEN                  RCC_IOPENR_IOPCEN        /*!< GPIO port C clock enable */

/*****************  Bit definition for RCC_AHBENR register  ******************/
#define RCC_AHBENR_DMAEN                    ((uint32_t)0x00000001U)        /*!< DMA1 clock enable */
#define RCC_AHBENR_MIFEN                    ((uint32_t)0x00000100U)        /*!< NVM interface clock enable bit */
#define RCC_AHBENR_CRCEN                    ((uint32_t)0x00001000U)        /*!< CRC clock enable */

/* Reference defines */
#define RCC_AHBENR_DMA1EN                   RCC_AHBENR_DMAEN              /*!< DMA1 clock enable */

/*****************  Bit definition for RCC_APB2ENR register  ******************/
#define RCC_APB2ENR_SYSCFGEN                ((uint32_t)0x00000001U)        /*!< SYSCFG clock enable */
#define RCC_APB2ENR_TIM21EN                 ((uint32_t)0x00000004U)        /*!< TIM21 clock enable */
#define RCC_APB2ENR_FWEN                    ((uint32_t)0x00000080U)        /*!< MiFare Firewall clock enable */
#define RCC_APB2ENR_ADCEN                   ((uint32_t)0x00000200U)        /*!< ADC1 clock enable */
#define RCC_APB2ENR_SPI1EN                  ((uint32_t)0x00001000U)        /*!< SPI1 clock enable */
#define RCC_APB2ENR_DBGEN                   ((uint32_t)0x00400000U)        /*!< DBGMCU clock enable */

/* Reference defines */

#define RCC_APB2ENR_MIFIEN                  RCC_APB2ENR_FWEN              /*!< MiFare Firewall clock enable */
#define RCC_APB2ENR_ADC1EN                  RCC_APB2ENR_ADCEN             /*!< ADC1 clock enable */
#define RCC_APB2ENR_DBGMCUEN                RCC_APB2ENR_DBGEN             /*!< DBGMCU clock enable */

/*****************  Bit definition for RCC_APB1ENR register  ******************/
#define RCC_APB1ENR_TIM2EN                  ((uint32_t)0x00000001U)        /*!< Timer 2 clock enable */
#define RCC_APB1ENR_WWDGEN                  ((uint32_t)0x00000800U)        /*!< Window Watchdog clock enable */
#define RCC_APB1ENR_USART2EN                ((uint32_t)0x00020000U)        /*!< USART2 clock enable */
#define RCC_APB1ENR_LPUART1EN               ((uint32_t)0x00040000U)        /*!< LPUART1 clock enable */
#define RCC_APB1ENR_I2C1EN                  ((uint32_t)0x00200000U)        /*!< I2C1 clock enable */
#define RCC_APB1ENR_PWREN                   ((uint32_t)0x10000000U)        /*!< PWR clock enable */
#define RCC_APB1ENR_LPTIM1EN                ((uint32_t)0x80000000U)        /*!< LPTIM1 clock enable */

/******************  Bit definition for RCC_IOPSMENR register  ****************/
#define RCC_IOPSMENR_IOPASMEN              ((uint32_t)0x00000001U)        /*!< GPIO port A clock enabled in sleep mode */
#define RCC_IOPSMENR_IOPBSMEN              ((uint32_t)0x00000002U)        /*!< GPIO port B clock enabled in sleep mode */
#define RCC_IOPSMENR_IOPCSMEN              ((uint32_t)0x00000004U)        /*!< GPIO port C clock enabled in sleep mode */

/* Reference defines */
#define RCC_IOPSMENR_GPIOASMEN              RCC_IOPSMENR_IOPASMEN        /*!< GPIO port A clock enabled in sleep mode */
#define RCC_IOPSMENR_GPIOBSMEN              RCC_IOPSMENR_IOPBSMEN        /*!< GPIO port B clock enabled in sleep mode */
#define RCC_IOPSMENR_GPIOCSMEN              RCC_IOPSMENR_IOPCSMEN        /*!< GPIO port C clock enabled in sleep mode */

/*****************  Bit definition for RCC_AHBSMENR register  ******************/
#define RCC_AHBSMENR_DMASMEN                ((uint32_t)0x00000001U)        /*!< DMA1 clock enabled in sleep mode */
#define RCC_AHBSMENR_MIFSMEN                ((uint32_t)0x00000100U)        /*!< NVM interface clock enable during sleep mode */
#define RCC_AHBSMENR_SRAMSMEN               ((uint32_t)0x00000200U)        /*!< SRAM clock enabled in sleep mode */
#define RCC_AHBSMENR_CRCSMEN                ((uint32_t)0x00001000U)        /*!< CRC clock enabled in sleep mode */

/* Reference defines */
#define RCC_AHBSMENR_DMA1SMEN               RCC_AHBSMENR_DMASMEN          /*!< DMA1 clock enabled in sleep mode */

/*****************  Bit definition for RCC_APB2SMENR register  ******************/
#define RCC_APB2SMENR_SYSCFGSMEN            ((uint32_t)0x00000001U)        /*!< SYSCFG clock enabled in sleep mode */
#define RCC_APB2SMENR_TIM21SMEN             ((uint32_t)0x00000004U)        /*!< TIM21 clock enabled in sleep mode */
#define RCC_APB2SMENR_ADCSMEN               ((uint32_t)0x00000200U)        /*!< ADC1 clock enabled in sleep mode */
#define RCC_APB2SMENR_SPI1SMEN              ((uint32_t)0x00001000U)        /*!< SPI1 clock enabled in sleep mode */
#define RCC_APB2SMENR_DBGSMEN               ((uint32_t)0x00400000U)        /*!< DBGMCU clock enabled in sleep mode */

/* Reference defines */
#define RCC_APB2SMENR_ADC1SMEN              RCC_APB2SMENR_ADCSMEN         /*!< ADC1 clock enabled in sleep mode */
#define RCC_APB2SMENR_DBGMCUSMEN            RCC_APB2SMENR_DBGSMEN         /*!< DBGMCU clock enabled in sleep mode */

/*****************  Bit definition for RCC_APB1SMENR register  ******************/
#define RCC_APB1SMENR_TIM2SMEN              ((uint32_t)0x00000001U)        /*!< Timer 2 clock enabled in sleep mode */
#define RCC_APB1SMENR_WWDGSMEN              ((uint32_t)0x00000800U)        /*!< Window Watchdog clock enabled in sleep mode */
#define RCC_APB1SMENR_USART2SMEN            ((uint32_t)0x00020000U)        /*!< USART2 clock enabled in sleep mode */
#define RCC_APB1SMENR_LPUART1SMEN           ((uint32_t)0x00040000U)        /*!< LPUART1 clock enabled in sleep mode */
#define RCC_APB1SMENR_I2C1SMEN              ((uint32_t)0x00200000U)        /*!< I2C1 clock enabled in sleep mode */
#define RCC_APB1SMENR_PWRSMEN                ((uint32_t)0x10000000U)       /*!< PWR clock enabled in sleep mode */
#define RCC_APB1SMENR_LPTIM1SMEN            ((uint32_t)0x80000000U)        /*!< LPTIM1 clock enabled in sleep mode */

/*!< USART2 Clock source selection */
#define RCC_CCIPR_USART2SEL                 ((uint32_t)0x0000000CU)        /*!< USART2SEL[1:0] bits */
#define RCC_CCIPR_USART2SEL_0               ((uint32_t)0x00000004U)        /*!< Bit 0 */
#define RCC_CCIPR_USART2SEL_1               ((uint32_t)0x00000008U)        /*!< Bit 1 */

/*!< LPUART1 Clock source selection */ 
#define RCC_CCIPR_LPUART1SEL                ((uint32_t)0x0000C00)         /*!< LPUART1SEL[1:0] bits */
#define RCC_CCIPR_LPUART1SEL_0              ((uint32_t)0x0000400)         /*!< Bit 0 */
#define RCC_CCIPR_LPUART1SEL_1              ((uint32_t)0x0000800)         /*!< Bit 1 */

/*!< I2C1 Clock source selection */
#define RCC_CCIPR_I2C1SEL                   ((uint32_t)0x00003000U)        /*!< I2C1SEL [1:0] bits */
#define RCC_CCIPR_I2C1SEL_0                 ((uint32_t)0x00001000U)        /*!< Bit 0 */
#define RCC_CCIPR_I2C1SEL_1                 ((uint32_t)0x00002000U)        /*!< Bit 1 */


/*!< LPTIM1 Clock source selection */ 
#define RCC_CCIPR_LPTIM1SEL                 ((uint32_t)0x000C0000U)        /*!< LPTIM1SEL [1:0] bits */
#define RCC_CCIPR_LPTIM1SEL_0               ((uint32_t)0x00040000U)        /*!< Bit 0 */
#define RCC_CCIPR_LPTIM1SEL_1               ((uint32_t)0x00080000U)        /*!< Bit 1 */

/*******************  Bit definition for RCC_CSR register  *******************/
#define RCC_CSR_LSION                       ((uint32_t)0x00000001U)        /*!< Internal Low Speed oscillator enable */
#define RCC_CSR_LSIRDY                      ((uint32_t)0x00000002U)        /*!< Internal Low Speed oscillator Ready */

#define RCC_CSR_LSEON                       ((uint32_t)0x00000100U)        /*!< External Low Speed oscillator enable */
#define RCC_CSR_LSERDY                      ((uint32_t)0x00000200U)        /*!< External Low Speed oscillator Ready */
#define RCC_CSR_LSEBYP                      ((uint32_t)0x00000400U)        /*!< External Low Speed oscillator Bypass */
                                             
#define RCC_CSR_LSEDRV                      ((uint32_t)0x00001800U)        /*!< LSEDRV[1:0] bits (LSE Osc. drive capability) */
#define RCC_CSR_LSEDRV_0                    ((uint32_t)0x00000800U)        /*!< Bit 0 */
#define RCC_CSR_LSEDRV_1                    ((uint32_t)0x00001000U)        /*!< Bit 1 */
                                             
#define RCC_CSR_LSECSSON                    ((uint32_t)0x00002000U)        /*!< External Low Speed oscillator CSS Enable */
#define RCC_CSR_LSECSSD                     ((uint32_t)0x00004000U)        /*!< External Low Speed oscillator CSS Detected */
                                             
/*!< RTC congiguration */                    
#define RCC_CSR_RTCSEL                      ((uint32_t)0x00030000U)        /*!< RTCSEL[1:0] bits (RTC clock source selection) */
#define RCC_CSR_RTCSEL_0                    ((uint32_t)0x00010000U)        /*!< Bit 0 */
#define RCC_CSR_RTCSEL_1                    ((uint32_t)0x00020000U)        /*!< Bit 1 */
                                             
#define RCC_CSR_RTCSEL_NOCLOCK              ((uint32_t)0x00000000U)        /*!< No clock */
#define RCC_CSR_RTCSEL_LSE                  ((uint32_t)0x00010000U)        /*!< LSE oscillator clock used as RTC clock */
#define RCC_CSR_RTCSEL_LSI                  ((uint32_t)0x00020000U)        /*!< LSI oscillator clock used as RTC clock */
#define RCC_CSR_RTCSEL_HSE                  ((uint32_t)0x00030000U)        /*!< HSE oscillator clock used as RTC clock */
                                             
#define RCC_CSR_RTCEN                       ((uint32_t)0x00040000U)        /*!< RTC clock enable */
#define RCC_CSR_RTCRST                      ((uint32_t)0x00080000U)        /*!< RTC software reset  */

#define RCC_CSR_RMVF                        ((uint32_t)0x00800000U)        /*!< Remove reset flag */
#define RCC_CSR_OBLRSTF                     ((uint32_t)0x02000000U)        /*!< OBL reset flag */
#define RCC_CSR_PINRSTF                     ((uint32_t)0x04000000U)        /*!< PIN reset flag */
#define RCC_CSR_PORRSTF                     ((uint32_t)0x08000000U)        /*!< POR/PDR reset flag */
#define RCC_CSR_SFTRSTF                     ((uint32_t)0x10000000U)        /*!< Software Reset flag */
#define RCC_CSR_IWDGRSTF                    ((uint32_t)0x20000000U)        /*!< Independent Watchdog reset flag */
#define RCC_CSR_WWDGRSTF                    ((uint32_t)0x40000000U)        /*!< Window watchdog reset flag */
#define RCC_CSR_LPWRRSTF                    ((uint32_t)0x80000000U)        /*!< Low-Power reset flag */

/* Reference defines */
#define RCC_CSR_OBL                         RCC_CSR_OBLRSTF               /*!< OBL reset flag */


/******************************************************************************/
/*                                                                            */
/*                           Real-Time Clock (RTC)                            */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for RTC_TR register  *******************/
#define RTC_TR_PM                            ((uint32_t)0x00400000U)        /*!<  */
#define RTC_TR_HT                            ((uint32_t)0x00300000U)        /*!<  */
#define RTC_TR_HT_0                          ((uint32_t)0x00100000U)        /*!<  */
#define RTC_TR_HT_1                          ((uint32_t)0x00200000U)        /*!<  */
#define RTC_TR_HU                            ((uint32_t)0x000F0000U)        /*!<  */
#define RTC_TR_HU_0                          ((uint32_t)0x00010000U)        /*!<  */
#define RTC_TR_HU_1                          ((uint32_t)0x00020000U)        /*!<  */
#define RTC_TR_HU_2                          ((uint32_t)0x00040000U)        /*!<  */
#define RTC_TR_HU_3                          ((uint32_t)0x00080000U)        /*!<  */
#define RTC_TR_MNT                           ((uint32_t)0x00007000U)        /*!<  */
#define RTC_TR_MNT_0                         ((uint32_t)0x00001000U)        /*!<  */
#define RTC_TR_MNT_1                         ((uint32_t)0x00002000U)        /*!<  */
#define RTC_TR_MNT_2                         ((uint32_t)0x00004000U)        /*!<  */
#define RTC_TR_MNU                           ((uint32_t)0x00000F00U)        /*!<  */
#define RTC_TR_MNU_0                         ((uint32_t)0x00000100U)        /*!<  */
#define RTC_TR_MNU_1                         ((uint32_t)0x00000200U)        /*!<  */
#define RTC_TR_MNU_2                         ((uint32_t)0x00000400U)        /*!<  */
#define RTC_TR_MNU_3                         ((uint32_t)0x00000800U)        /*!<  */
#define RTC_TR_ST                            ((uint32_t)0x00000070U)        /*!<  */
#define RTC_TR_ST_0                          ((uint32_t)0x00000010U)        /*!<  */
#define RTC_TR_ST_1                          ((uint32_t)0x00000020U)        /*!<  */
#define RTC_TR_ST_2                          ((uint32_t)0x00000040U)        /*!<  */
#define RTC_TR_SU                            ((uint32_t)0x0000000FU)        /*!<  */
#define RTC_TR_SU_0                          ((uint32_t)0x00000001U)        /*!<  */
#define RTC_TR_SU_1                          ((uint32_t)0x00000002U)        /*!<  */
#define RTC_TR_SU_2                          ((uint32_t)0x00000004U)        /*!<  */
#define RTC_TR_SU_3                          ((uint32_t)0x00000008U)        /*!<  */

/********************  Bits definition for RTC_DR register  *******************/
#define RTC_DR_YT                            ((uint32_t)0x00F00000U)        /*!<  */
#define RTC_DR_YT_0                          ((uint32_t)0x00100000U)        /*!<  */
#define RTC_DR_YT_1                          ((uint32_t)0x00200000U)        /*!<  */
#define RTC_DR_YT_2                          ((uint32_t)0x00400000U)        /*!<  */
#define RTC_DR_YT_3                          ((uint32_t)0x00800000U)        /*!<  */
#define RTC_DR_YU                            ((uint32_t)0x000F0000U)        /*!<  */
#define RTC_DR_YU_0                          ((uint32_t)0x00010000U)        /*!<  */
#define RTC_DR_YU_1                          ((uint32_t)0x00020000U)        /*!<  */
#define RTC_DR_YU_2                          ((uint32_t)0x00040000U)        /*!<  */
#define RTC_DR_YU_3                          ((uint32_t)0x00080000U)        /*!<  */
#define RTC_DR_WDU                           ((uint32_t)0x0000E000U)        /*!<  */
#define RTC_DR_WDU_0                         ((uint32_t)0x00002000U)        /*!<  */
#define RTC_DR_WDU_1                         ((uint32_t)0x00004000U)        /*!<  */
#define RTC_DR_WDU_2                         ((uint32_t)0x00008000U)        /*!<  */
#define RTC_DR_MT                            ((uint32_t)0x00001000U)        /*!<  */
#define RTC_DR_MU                            ((uint32_t)0x00000F00U)        /*!<  */
#define RTC_DR_MU_0                          ((uint32_t)0x00000100U)        /*!<  */
#define RTC_DR_MU_1                          ((uint32_t)0x00000200U)        /*!<  */
#define RTC_DR_MU_2                          ((uint32_t)0x00000400U)        /*!<  */
#define RTC_DR_MU_3                          ((uint32_t)0x00000800U)        /*!<  */
#define RTC_DR_DT                            ((uint32_t)0x00000030U)        /*!<  */
#define RTC_DR_DT_0                          ((uint32_t)0x00000010U)        /*!<  */
#define RTC_DR_DT_1                          ((uint32_t)0x00000020U)        /*!<  */
#define RTC_DR_DU                            ((uint32_t)0x0000000FU)        /*!<  */
#define RTC_DR_DU_0                          ((uint32_t)0x00000001U)        /*!<  */
#define RTC_DR_DU_1                          ((uint32_t)0x00000002U)        /*!<  */
#define RTC_DR_DU_2                          ((uint32_t)0x00000004U)        /*!<  */
#define RTC_DR_DU_3                          ((uint32_t)0x00000008U)        /*!<  */

/********************  Bits definition for RTC_CR register  *******************/
#define RTC_CR_COE                           ((uint32_t)0x00800000U)        /*!<  */
#define RTC_CR_OSEL                          ((uint32_t)0x00600000U)        /*!<  */
#define RTC_CR_OSEL_0                        ((uint32_t)0x00200000U)        /*!<  */
#define RTC_CR_OSEL_1                        ((uint32_t)0x00400000U)        /*!<  */
#define RTC_CR_POL                           ((uint32_t)0x00100000U)        /*!<  */
#define RTC_CR_COSEL                         ((uint32_t)0x00080000U)        /*!<  */
#define RTC_CR_BCK                           ((uint32_t)0x00040000U)        /*!<  */
#define RTC_CR_SUB1H                         ((uint32_t)0x00020000U)        /*!<  */
#define RTC_CR_ADD1H                         ((uint32_t)0x00010000U)        /*!<  */
#define RTC_CR_TSIE                          ((uint32_t)0x00008000U)        /*!<  */
#define RTC_CR_WUTIE                         ((uint32_t)0x00004000U)        /*!<  */
#define RTC_CR_ALRBIE                        ((uint32_t)0x00002000U)        /*!<  */
#define RTC_CR_ALRAIE                        ((uint32_t)0x00001000U)        /*!<  */
#define RTC_CR_TSE                           ((uint32_t)0x00000800U)        /*!<  */
#define RTC_CR_WUTE                          ((uint32_t)0x00000400U)        /*!<  */
#define RTC_CR_ALRBE                         ((uint32_t)0x00000200U)        /*!<  */
#define RTC_CR_ALRAE                         ((uint32_t)0x00000100U)        /*!<  */
#define RTC_CR_FMT                           ((uint32_t)0x00000040U)        /*!<  */
#define RTC_CR_BYPSHAD                       ((uint32_t)0x00000020U)        /*!<  */
#define RTC_CR_REFCKON                       ((uint32_t)0x00000010U)        /*!<  */
#define RTC_CR_TSEDGE                        ((uint32_t)0x00000008U)        /*!<  */
#define RTC_CR_WUCKSEL                       ((uint32_t)0x00000007U)        /*!<  */
#define RTC_CR_WUCKSEL_0                     ((uint32_t)0x00000001U)        /*!<  */
#define RTC_CR_WUCKSEL_1                     ((uint32_t)0x00000002U)        /*!<  */
#define RTC_CR_WUCKSEL_2                     ((uint32_t)0x00000004U)        /*!<  */

/********************  Bits definition for RTC_ISR register  ******************/
#define RTC_ISR_RECALPF                      ((uint32_t)0x00010000U)        /*!<  */
#define RTC_ISR_TAMP3F                       ((uint32_t)0x00008000U)        /*!<  */
#define RTC_ISR_TAMP2F                       ((uint32_t)0x00004000U)        /*!<  */
#define RTC_ISR_TSOVF                        ((uint32_t)0x00001000U)        /*!<  */
#define RTC_ISR_TSF                          ((uint32_t)0x00000800U)        /*!<  */
#define RTC_ISR_WUTF                         ((uint32_t)0x00000400U)        /*!<  */
#define RTC_ISR_ALRBF                        ((uint32_t)0x00000200U)        /*!<  */
#define RTC_ISR_ALRAF                        ((uint32_t)0x00000100U)        /*!<  */
#define RTC_ISR_INIT                         ((uint32_t)0x00000080U)        /*!<  */
#define RTC_ISR_INITF                        ((uint32_t)0x00000040U)        /*!<  */
#define RTC_ISR_RSF                          ((uint32_t)0x00000020U)        /*!<  */
#define RTC_ISR_INITS                        ((uint32_t)0x00000010U)        /*!<  */
#define RTC_ISR_SHPF                         ((uint32_t)0x00000008U)        /*!<  */
#define RTC_ISR_WUTWF                        ((uint32_t)0x00000004U)        /*!<  */
#define RTC_ISR_ALRBWF                       ((uint32_t)0x00000002U)        /*!<  */
#define RTC_ISR_ALRAWF                       ((uint32_t)0x00000001U)        /*!<  */

/********************  Bits definition for RTC_PRER register  *****************/
#define RTC_PRER_PREDIV_A                    ((uint32_t)0x007F0000U)        /*!<  */
#define RTC_PRER_PREDIV_S                    ((uint32_t)0x00007FFFU)        /*!<  */

/********************  Bits definition for RTC_WUTR register  *****************/
#define RTC_WUTR_WUT                         ((uint32_t)0x0000FFFFU)

/********************  Bits definition for RTC_ALRMAR register  ***************/
#define RTC_ALRMAR_MSK4                      ((uint32_t)0x80000000U)        /*!<  */
#define RTC_ALRMAR_WDSEL                     ((uint32_t)0x40000000U)        /*!<  */
#define RTC_ALRMAR_DT                        ((uint32_t)0x30000000U)        /*!<  */
#define RTC_ALRMAR_DT_0                      ((uint32_t)0x10000000U)        /*!<  */
#define RTC_ALRMAR_DT_1                      ((uint32_t)0x20000000U)        /*!<  */
#define RTC_ALRMAR_DU                        ((uint32_t)0x0F000000U)        /*!<  */
#define RTC_ALRMAR_DU_0                      ((uint32_t)0x01000000U)        /*!<  */
#define RTC_ALRMAR_DU_1                      ((uint32_t)0x02000000U)        /*!<  */
#define RTC_ALRMAR_DU_2                      ((uint32_t)0x04000000U)        /*!<  */
#define RTC_ALRMAR_DU_3                      ((uint32_t)0x08000000U)        /*!<  */
#define RTC_ALRMAR_MSK3                      ((uint32_t)0x00800000U)        /*!<  */
#define RTC_ALRMAR_PM                        ((uint32_t)0x00400000U)        /*!<  */
#define RTC_ALRMAR_HT                        ((uint32_t)0x00300000U)        /*!<  */
#define RTC_ALRMAR_HT_0                      ((uint32_t)0x00100000U)        /*!<  */
#define RTC_ALRMAR_HT_1                      ((uint32_t)0x00200000U)        /*!<  */
#define RTC_ALRMAR_HU                        ((uint32_t)0x000F0000U)        /*!<  */
#define RTC_ALRMAR_HU_0                      ((uint32_t)0x00010000U)        /*!<  */
#define RTC_ALRMAR_HU_1                      ((uint32_t)0x00020000U)        /*!<  */
#define RTC_ALRMAR_HU_2                      ((uint32_t)0x00040000U)        /*!<  */
#define RTC_ALRMAR_HU_3                      ((uint32_t)0x00080000U)        /*!<  */
#define RTC_ALRMAR_MSK2                      ((uint32_t)0x00008000U)        /*!<  */
#define RTC_ALRMAR_MNT                       ((uint32_t)0x00007000U)        /*!<  */
#define RTC_ALRMAR_MNT_0                     ((uint32_t)0x00001000U)        /*!<  */
#define RTC_ALRMAR_MNT_1                     ((uint32_t)0x00002000U)        /*!<  */
#define RTC_ALRMAR_MNT_2                     ((uint32_t)0x00004000U)        /*!<  */
#define RTC_ALRMAR_MNU                       ((uint32_t)0x00000F00U)        /*!<  */
#define RTC_ALRMAR_MNU_0                     ((uint32_t)0x00000100U)        /*!<  */
#define RTC_ALRMAR_MNU_1                     ((uint32_t)0x00000200U)        /*!<  */
#define RTC_ALRMAR_MNU_2                     ((uint32_t)0x00000400U)        /*!<  */
#define RTC_ALRMAR_MNU_3                     ((uint32_t)0x00000800U)        /*!<  */
#define RTC_ALRMAR_MSK1                      ((uint32_t)0x00000080U)        /*!<  */
#define RTC_ALRMAR_ST                        ((uint32_t)0x00000070U)        /*!<  */
#define RTC_ALRMAR_ST_0                      ((uint32_t)0x00000010U)        /*!<  */
#define RTC_ALRMAR_ST_1                      ((uint32_t)0x00000020U)        /*!<  */
#define RTC_ALRMAR_ST_2                      ((uint32_t)0x00000040U)        /*!<  */
#define RTC_ALRMAR_SU                        ((uint32_t)0x0000000FU)        /*!<  */
#define RTC_ALRMAR_SU_0                      ((uint32_t)0x00000001U)        /*!<  */
#define RTC_ALRMAR_SU_1                      ((uint32_t)0x00000002U)        /*!<  */
#define RTC_ALRMAR_SU_2                      ((uint32_t)0x00000004U)        /*!<  */
#define RTC_ALRMAR_SU_3                      ((uint32_t)0x00000008U)        /*!<  */

/********************  Bits definition for RTC_ALRMBR register  ***************/
#define RTC_ALRMBR_MSK4                      ((uint32_t)0x80000000U)        /*!<  */
#define RTC_ALRMBR_WDSEL                     ((uint32_t)0x40000000U)        /*!<  */
#define RTC_ALRMBR_DT                        ((uint32_t)0x30000000U)        /*!<  */
#define RTC_ALRMBR_DT_0                      ((uint32_t)0x10000000U)        /*!<  */
#define RTC_ALRMBR_DT_1                      ((uint32_t)0x20000000U)        /*!<  */
#define RTC_ALRMBR_DU                        ((uint32_t)0x0F000000U)        /*!<  */
#define RTC_ALRMBR_DU_0                      ((uint32_t)0x01000000U)        /*!<  */
#define RTC_ALRMBR_DU_1                      ((uint32_t)0x02000000U)        /*!<  */
#define RTC_ALRMBR_DU_2                      ((uint32_t)0x04000000U)        /*!<  */
#define RTC_ALRMBR_DU_3                      ((uint32_t)0x08000000U)        /*!<  */
#define RTC_ALRMBR_MSK3                      ((uint32_t)0x00800000U)        /*!<  */
#define RTC_ALRMBR_PM                        ((uint32_t)0x00400000U)        /*!<  */
#define RTC_ALRMBR_HT                        ((uint32_t)0x00300000U)        /*!<  */
#define RTC_ALRMBR_HT_0                      ((uint32_t)0x00100000U)        /*!<  */
#define RTC_ALRMBR_HT_1                      ((uint32_t)0x00200000U)        /*!<  */
#define RTC_ALRMBR_HU                        ((uint32_t)0x000F0000U)        /*!<  */
#define RTC_ALRMBR_HU_0                      ((uint32_t)0x00010000U)        /*!<  */
#define RTC_ALRMBR_HU_1                      ((uint32_t)0x00020000U)        /*!<  */
#define RTC_ALRMBR_HU_2                      ((uint32_t)0x00040000U)        /*!<  */
#define RTC_ALRMBR_HU_3                      ((uint32_t)0x00080000U)        /*!<  */
#define RTC_ALRMBR_MSK2                      ((uint32_t)0x00008000U)        /*!<  */
#define RTC_ALRMBR_MNT                       ((uint32_t)0x00007000U)        /*!<  */
#define RTC_ALRMBR_MNT_0                     ((uint32_t)0x00001000U)        /*!<  */
#define RTC_ALRMBR_MNT_1                     ((uint32_t)0x00002000U)        /*!<  */
#define RTC_ALRMBR_MNT_2                     ((uint32_t)0x00004000U)        /*!<  */
#define RTC_ALRMBR_MNU                       ((uint32_t)0x00000F00U)        /*!<  */
#define RTC_ALRMBR_MNU_0                     ((uint32_t)0x00000100U)        /*!<  */
#define RTC_ALRMBR_MNU_1                     ((uint32_t)0x00000200U)        /*!<  */
#define RTC_ALRMBR_MNU_2                     ((uint32_t)0x00000400U)        /*!<  */
#define RTC_ALRMBR_MNU_3                     ((uint32_t)0x00000800U)        /*!<  */
#define RTC_ALRMBR_MSK1                      ((uint32_t)0x00000080U)        /*!<  */
#define RTC_ALRMBR_ST                        ((uint32_t)0x00000070U)        /*!<  */
#define RTC_ALRMBR_ST_0                      ((uint32_t)0x00000010U)        /*!<  */
#define RTC_ALRMBR_ST_1                      ((uint32_t)0x00000020U)        /*!<  */
#define RTC_ALRMBR_ST_2                      ((uint32_t)0x00000040U)        /*!<  */
#define RTC_ALRMBR_SU                        ((uint32_t)0x0000000FU)        /*!<  */
#define RTC_ALRMBR_SU_0                      ((uint32_t)0x00000001U)        /*!<  */
#define RTC_ALRMBR_SU_1                      ((uint32_t)0x00000002U)        /*!<  */
#define RTC_ALRMBR_SU_2                      ((uint32_t)0x00000004U)        /*!<  */
#define RTC_ALRMBR_SU_3                      ((uint32_t)0x00000008U)        /*!<  */

/********************  Bits definition for RTC_WPR register  ******************/
#define RTC_WPR_KEY                          ((uint32_t)0x000000FFU)        /*!<  */

/********************  Bits definition for RTC_SSR register  ******************/
#define RTC_SSR_SS                           ((uint32_t)0x0000FFFFU)        /*!<  */

/********************  Bits definition for RTC_SHIFTR register  ***************/
#define RTC_SHIFTR_SUBFS                     ((uint32_t)0x00007FFFU)        /*!<  */
#define RTC_SHIFTR_ADD1S                     ((uint32_t)0x80000000U)        /*!<  */

/********************  Bits definition for RTC_TSTR register  *****************/
#define RTC_TSTR_PM                          ((uint32_t)0x00400000U)        /*!<  */
#define RTC_TSTR_HT                          ((uint32_t)0x00300000U)        /*!<  */
#define RTC_TSTR_HT_0                        ((uint32_t)0x00100000U)        /*!<  */
#define RTC_TSTR_HT_1                        ((uint32_t)0x00200000U)        /*!<  */
#define RTC_TSTR_HU                          ((uint32_t)0x000F0000U)        /*!<  */
#define RTC_TSTR_HU_0                        ((uint32_t)0x00010000U)        /*!<  */
#define RTC_TSTR_HU_1                        ((uint32_t)0x00020000U)        /*!<  */
#define RTC_TSTR_HU_2                        ((uint32_t)0x00040000U)        /*!<  */
#define RTC_TSTR_HU_3                        ((uint32_t)0x00080000U)        /*!<  */
#define RTC_TSTR_MNT                         ((uint32_t)0x00007000U)        /*!<  */
#define RTC_TSTR_MNT_0                       ((uint32_t)0x00001000U)        /*!<  */
#define RTC_TSTR_MNT_1                       ((uint32_t)0x00002000U)        /*!<  */
#define RTC_TSTR_MNT_2                       ((uint32_t)0x00004000U)        /*!<  */
#define RTC_TSTR_MNU                         ((uint32_t)0x00000F00U)        /*!<  */
#define RTC_TSTR_MNU_0                       ((uint32_t)0x00000100U)        /*!<  */
#define RTC_TSTR_MNU_1                       ((uint32_t)0x00000200U)        /*!<  */
#define RTC_TSTR_MNU_2                       ((uint32_t)0x00000400U)        /*!<  */
#define RTC_TSTR_MNU_3                       ((uint32_t)0x00000800U)        /*!<  */
#define RTC_TSTR_ST                          ((uint32_t)0x00000070U)        /*!<  */
#define RTC_TSTR_ST_0                        ((uint32_t)0x00000010U)        /*!<  */
#define RTC_TSTR_ST_1                        ((uint32_t)0x00000020U)        /*!<  */
#define RTC_TSTR_ST_2                        ((uint32_t)0x00000040U)        /*!<  */
#define RTC_TSTR_SU                          ((uint32_t)0x0000000FU)        /*!<  */
#define RTC_TSTR_SU_0                        ((uint32_t)0x00000001U)        /*!<  */
#define RTC_TSTR_SU_1                        ((uint32_t)0x00000002U)        /*!<  */
#define RTC_TSTR_SU_2                        ((uint32_t)0x00000004U)        /*!<  */
#define RTC_TSTR_SU_3                        ((uint32_t)0x00000008U)        /*!<  */

/********************  Bits definition for RTC_TSDR register  *****************/
#define RTC_TSDR_WDU                         ((uint32_t)0x0000E000U)        /*!<  */
#define RTC_TSDR_WDU_0                       ((uint32_t)0x00002000U)        /*!<  */
#define RTC_TSDR_WDU_1                       ((uint32_t)0x00004000U)        /*!<  */
#define RTC_TSDR_WDU_2                       ((uint32_t)0x00008000U)        /*!<  */
#define RTC_TSDR_MT                          ((uint32_t)0x00001000U)        /*!<  */
#define RTC_TSDR_MU                          ((uint32_t)0x00000F00U)        /*!<  */
#define RTC_TSDR_MU_0                        ((uint32_t)0x00000100U)        /*!<  */
#define RTC_TSDR_MU_1                        ((uint32_t)0x00000200U)        /*!<  */
#define RTC_TSDR_MU_2                        ((uint32_t)0x00000400U)        /*!<  */
#define RTC_TSDR_MU_3                        ((uint32_t)0x00000800U)        /*!<  */
#define RTC_TSDR_DT                          ((uint32_t)0x00000030U)        /*!<  */
#define RTC_TSDR_DT_0                        ((uint32_t)0x00000010U)        /*!<  */
#define RTC_TSDR_DT_1                        ((uint32_t)0x00000020U)        /*!<  */
#define RTC_TSDR_DU                          ((uint32_t)0x0000000FU)        /*!<  */
#define RTC_TSDR_DU_0                        ((uint32_t)0x00000001U)        /*!<  */
#define RTC_TSDR_DU_1                        ((uint32_t)0x00000002U)        /*!<  */
#define RTC_TSDR_DU_2                        ((uint32_t)0x00000004U)        /*!<  */
#define RTC_TSDR_DU_3                        ((uint32_t)0x00000008U)        /*!<  */

/********************  Bits definition for RTC_TSSSR register  ****************/
#define RTC_TSSSR_SS                         ((uint32_t)0x0000FFFFU)

/********************  Bits definition for RTC_CALR register  *****************/
#define RTC_CALR_CALP                         ((uint32_t)0x00008000U)        /*!<  */
#define RTC_CALR_CALW8                        ((uint32_t)0x00004000U)        /*!<  */
#define RTC_CALR_CALW16                       ((uint32_t)0x00002000U)        /*!<  */
#define RTC_CALR_CALM                         ((uint32_t)0x000001FFU)        /*!<  */
#define RTC_CALR_CALM_0                       ((uint32_t)0x00000001U)        /*!<  */
#define RTC_CALR_CALM_1                       ((uint32_t)0x00000002U)        /*!<  */
#define RTC_CALR_CALM_2                       ((uint32_t)0x00000004U)        /*!<  */
#define RTC_CALR_CALM_3                       ((uint32_t)0x00000008U)        /*!<  */
#define RTC_CALR_CALM_4                       ((uint32_t)0x00000010U)        /*!<  */
#define RTC_CALR_CALM_5                       ((uint32_t)0x00000020U)        /*!<  */
#define RTC_CALR_CALM_6                       ((uint32_t)0x00000040U)        /*!<  */
#define RTC_CALR_CALM_7                       ((uint32_t)0x00000080U)        /*!<  */
#define RTC_CALR_CALM_8                       ((uint32_t)0x00000100U)        /*!<  */

/* Legacy defines */
#define RTC_CAL_CALP     RTC_CALR_CALP 
#define RTC_CAL_CALW8    RTC_CALR_CALW8  
#define RTC_CAL_CALW16   RTC_CALR_CALW16 
#define RTC_CAL_CALM     RTC_CALR_CALM 
#define RTC_CAL_CALM_0   RTC_CALR_CALM_0 
#define RTC_CAL_CALM_1   RTC_CALR_CALM_1 
#define RTC_CAL_CALM_2   RTC_CALR_CALM_2 
#define RTC_CAL_CALM_3   RTC_CALR_CALM_3 
#define RTC_CAL_CALM_4   RTC_CALR_CALM_4 
#define RTC_CAL_CALM_5   RTC_CALR_CALM_5 
#define RTC_CAL_CALM_6   RTC_CALR_CALM_6 
#define RTC_CAL_CALM_7   RTC_CALR_CALM_7 
#define RTC_CAL_CALM_8   RTC_CALR_CALM_8 

/********************  Bits definition for RTC_TAMPCR register  ****************/
#define RTC_TAMPCR_TAMP3MF                   ((uint32_t)0x01000000U)        /*!<  */
#define RTC_TAMPCR_TAMP3NOERASE              ((uint32_t)0x00800000U)        /*!<  */
#define RTC_TAMPCR_TAMP3IE                   ((uint32_t)0x00400000U)        /*!<  */
#define RTC_TAMPCR_TAMP2MF                   ((uint32_t)0x00200000U)        /*!<  */
#define RTC_TAMPCR_TAMP2NOERASE              ((uint32_t)0x00100000U)        /*!<  */
#define RTC_TAMPCR_TAMP2IE                   ((uint32_t)0x00080000U)        /*!<  */
#define RTC_TAMPCR_TAMPPUDIS                 ((uint32_t)0x00008000U)        /*!<  */
#define RTC_TAMPCR_TAMPPRCH                  ((uint32_t)0x00006000U)        /*!<  */
#define RTC_TAMPCR_TAMPPRCH_0                ((uint32_t)0x00002000U)        /*!<  */
#define RTC_TAMPCR_TAMPPRCH_1                ((uint32_t)0x00004000U)        /*!<  */
#define RTC_TAMPCR_TAMPFLT                   ((uint32_t)0x00001800U)        /*!<  */
#define RTC_TAMPCR_TAMPFLT_0                 ((uint32_t)0x00000800U)        /*!<  */
#define RTC_TAMPCR_TAMPFLT_1                 ((uint32_t)0x00001000U)        /*!<  */
#define RTC_TAMPCR_TAMPFREQ                  ((uint32_t)0x00000700U)        /*!<  */
#define RTC_TAMPCR_TAMPFREQ_0                ((uint32_t)0x00000100U)        /*!<  */
#define RTC_TAMPCR_TAMPFREQ_1                ((uint32_t)0x00000200U)        /*!<  */
#define RTC_TAMPCR_TAMPFREQ_2                ((uint32_t)0x00000400U)        /*!<  */
#define RTC_TAMPCR_TAMPTS                    ((uint32_t)0x00000080U)        /*!<  */
#define RTC_TAMPCR_TAMP3TRG                  ((uint32_t)0x00000040U)        /*!<  */
#define RTC_TAMPCR_TAMP3E                    ((uint32_t)0x00000020U)        /*!<  */
#define RTC_TAMPCR_TAMP2TRG                  ((uint32_t)0x00000010U)        /*!<  */
#define RTC_TAMPCR_TAMP2E                    ((uint32_t)0x00000008U)        /*!<  */
#define RTC_TAMPCR_TAMPIE                    ((uint32_t)0x00000004U)        /*!<  */

/********************  Bits definition for RTC_ALRMASSR register  *************/
#define RTC_ALRMASSR_MASKSS                  ((uint32_t)0x0F000000U)
#define RTC_ALRMASSR_MASKSS_0                ((uint32_t)0x01000000U)
#define RTC_ALRMASSR_MASKSS_1                ((uint32_t)0x02000000U)
#define RTC_ALRMASSR_MASKSS_2                ((uint32_t)0x04000000U)
#define RTC_ALRMASSR_MASKSS_3                ((uint32_t)0x08000000U)
#define RTC_ALRMASSR_SS                      ((uint32_t)0x00007FFFU)

/********************  Bits definition for RTC_ALRMBSSR register  *************/
#define RTC_ALRMBSSR_MASKSS                  ((uint32_t)0x0F000000U)
#define RTC_ALRMBSSR_MASKSS_0                ((uint32_t)0x01000000U)
#define RTC_ALRMBSSR_MASKSS_1                ((uint32_t)0x02000000U)
#define RTC_ALRMBSSR_MASKSS_2                ((uint32_t)0x04000000U)
#define RTC_ALRMBSSR_MASKSS_3                ((uint32_t)0x08000000U)
#define RTC_ALRMBSSR_SS                      ((uint32_t)0x00007FFFU)

/********************  Bits definition for RTC_OR register  ****************/
#define RTC_OR_OUT_RMP                       ((uint32_t)0x00000002U)        /*!<  */
#define RTC_OR_ALARMOUTTYPE                  ((uint32_t)0x00000001U)        /*!<  */

/* Legacy defines */
#define RTC_OR_RTC_OUT_RMP                   RTC_OR_OUT_RMP

/********************  Bits definition for RTC_BKP0R register  ****************/
#define RTC_BKP0R                            ((uint32_t)0xFFFFFFFFU)        /*!<  */

/********************  Bits definition for RTC_BKP1R register  ****************/
#define RTC_BKP1R                            ((uint32_t)0xFFFFFFFFU)        /*!<  */

/********************  Bits definition for RTC_BKP2R register  ****************/
#define RTC_BKP2R                            ((uint32_t)0xFFFFFFFFU)        /*!<  */

/********************  Bits definition for RTC_BKP3R register  ****************/
#define RTC_BKP3R                            ((uint32_t)0xFFFFFFFFU)        /*!<  */

/********************  Bits definition for RTC_BKP4R register  ****************/
#define RTC_BKP4R                            ((uint32_t)0xFFFFFFFFU)        /*!<  */

/******************** Number of backup registers ******************************/
#define RTC_BKP_NUMBER                       ((uint32_t)0x00000005U)        /*!<  */

/******************************************************************************/
/*                                                                            */
/*                        Serial Peripheral Interface (SPI)                   */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for SPI_CR1 register  ********************/
#define SPI_CR1_CPHA                        ((uint32_t)0x00000001U)            /*!< Clock Phase */
#define SPI_CR1_CPOL                        ((uint32_t)0x00000002U)            /*!< Clock Polarity */
#define SPI_CR1_MSTR                        ((uint32_t)0x00000004U)            /*!< Master Selection */
#define SPI_CR1_BR                          ((uint32_t)0x00000038U)            /*!< BR[2:0] bits (Baud Rate Control) */
#define SPI_CR1_BR_0                        ((uint32_t)0x00000008U)            /*!< Bit 0 */
#define SPI_CR1_BR_1                        ((uint32_t)0x00000010U)            /*!< Bit 1 */
#define SPI_CR1_BR_2                        ((uint32_t)0x00000020U)            /*!< Bit 2 */
#define SPI_CR1_SPE                         ((uint32_t)0x00000040U)            /*!< SPI Enable */
#define SPI_CR1_LSBFIRST                    ((uint32_t)0x00000080U)            /*!< Frame Format */
#define SPI_CR1_SSI                         ((uint32_t)0x00000100U)            /*!< Internal slave select */
#define SPI_CR1_SSM                         ((uint32_t)0x00000200U)            /*!< Software slave management */
#define SPI_CR1_RXONLY                      ((uint32_t)0x00000400U)            /*!< Receive only */
#define SPI_CR1_DFF                         ((uint32_t)0x00000800U)            /*!< Data Frame Format */
#define SPI_CR1_CRCNEXT                     ((uint32_t)0x00001000U)            /*!< Transmit CRC next */
#define SPI_CR1_CRCEN                       ((uint32_t)0x00002000U)            /*!< Hardware CRC calculation enable */
#define SPI_CR1_BIDIOE                      ((uint32_t)0x00004000U)            /*!< Output enable in bidirectional mode */
#define SPI_CR1_BIDIMODE                    ((uint32_t)0x00008000U)            /*!< Bidirectional data mode enable */

/*******************  Bit definition for SPI_CR2 register  ********************/
#define SPI_CR2_RXDMAEN                     ((uint32_t)0x00000001U)            /*!< Rx Buffer DMA Enable */
#define SPI_CR2_TXDMAEN                     ((uint32_t)0x00000002U)            /*!< Tx Buffer DMA Enable */
#define SPI_CR2_SSOE                        ((uint32_t)0x00000004U)            /*!< SS Output Enable */
#define SPI_CR2_FRF                         ((uint32_t)0x00000010U)            /*!< Frame Format Enable */
#define SPI_CR2_ERRIE                       ((uint32_t)0x00000020U)            /*!< Error Interrupt Enable */
#define SPI_CR2_RXNEIE                      ((uint32_t)0x00000040U)            /*!< RX buffer Not Empty Interrupt Enable */
#define SPI_CR2_TXEIE                       ((uint32_t)0x00000080U)            /*!< Tx buffer Empty Interrupt Enable */

/********************  Bit definition for SPI_SR register  ********************/
#define SPI_SR_RXNE                         ((uint32_t)0x00000001U)            /*!< Receive buffer Not Empty */
#define SPI_SR_TXE                          ((uint32_t)0x00000002U)            /*!< Transmit buffer Empty */
#define SPI_SR_CHSIDE                       ((uint32_t)0x00000004U)            /*!< Channel side */
#define SPI_SR_UDR                          ((uint32_t)0x00000008U)            /*!< Underrun flag */
#define SPI_SR_CRCERR                       ((uint32_t)0x00000010U)            /*!< CRC Error flag */
#define SPI_SR_MODF                         ((uint32_t)0x00000020U)            /*!< Mode fault */
#define SPI_SR_OVR                          ((uint32_t)0x00000040U)            /*!< Overrun flag */
#define SPI_SR_BSY                          ((uint32_t)0x00000080U)            /*!< Busy flag */
#define SPI_SR_FRE                          ((uint32_t)0x00000100U)            /*!< TI frame format error */  

/********************  Bit definition for SPI_DR register  ********************/
#define SPI_DR_DR                           ((uint32_t)0x0000FFFFU)            /*!< Data Register */

/*******************  Bit definition for SPI_CRCPR register  ******************/
#define SPI_CRCPR_CRCPOLY                   ((uint32_t)0x0000FFFFU)            /*!< CRC polynomial register */

/******************  Bit definition for SPI_RXCRCR register  ******************/
#define SPI_RXCRCR_RXCRC                    ((uint32_t)0x0000FFFFU)            /*!< Rx CRC Register */

/******************  Bit definition for SPI_TXCRCR register  ******************/
#define SPI_TXCRCR_TXCRC                    ((uint32_t)0x0000FFFFU)            /*!< Tx CRC Register */

/******************************************************************************/
/*                                                                            */
/*                       System Configuration (SYSCFG)                        */
/*                                                                            */
/******************************************************************************/
/*****************  Bit definition for SYSCFG_CFGR1 register  ****************/
#define SYSCFG_CFGR1_MEM_MODE               ((uint32_t)0x00000003U) /*!< SYSCFG_Memory Remap Config */
#define SYSCFG_CFGR1_MEM_MODE_0             ((uint32_t)0x00000001U) /*!< SYSCFG_Memory Remap Config Bit 0 */
#define SYSCFG_CFGR1_MEM_MODE_1             ((uint32_t)0x00000002U) /*!< SYSCFG_Memory Remap Config Bit 1 */
#define SYSCFG_CFGR1_BOOT_MODE              ((uint32_t)0x00000300U) /*!< SYSCFG_Boot mode Config */
#define SYSCFG_CFGR1_BOOT_MODE_0            ((uint32_t)0x00000100U) /*!< SYSCFG_Boot mode Config Bit 0 */
#define SYSCFG_CFGR1_BOOT_MODE_1            ((uint32_t)0x00000200U) /*!< SYSCFG_Boot mode Config Bit 1 */

/*****************  Bit definition for SYSCFG_CFGR2 register  ****************/
#define SYSCFG_CFGR2_FWDISEN                ((uint32_t)0x00000001U) /*!< Firewall disable bit */
#define SYSCFG_CFGR2_I2C_PB6_FMP            ((uint32_t)0x00000100U) /*!< I2C PB6 Fast mode plus */
#define SYSCFG_CFGR2_I2C_PB7_FMP            ((uint32_t)0x00000200U) /*!< I2C PB7 Fast mode plus */
#define SYSCFG_CFGR2_I2C_PB8_FMP            ((uint32_t)0x00000400U) /*!< I2C PB8 Fast mode plus */
#define SYSCFG_CFGR2_I2C_PB9_FMP            ((uint32_t)0x00000800U) /*!< I2C PB9 Fast mode plus */
#define SYSCFG_CFGR2_I2C1_FMP               ((uint32_t)0x00001000U) /*!< I2C1 Fast mode plus */

/*****************  Bit definition for SYSCFG_EXTICR1 register  ***************/
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
#define SYSCFG_EXTICR1_EXTI0_PH         ((uint32_t)0x00000005U) /*!< PH[0] pin */

/** 
  * @brief  EXTI1 configuration  
  */ 
#define SYSCFG_EXTICR1_EXTI1_PA         ((uint32_t)0x00000000U) /*!< PA[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PB         ((uint32_t)0x00000010U) /*!< PB[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PC         ((uint32_t)0x00000020U) /*!< PC[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PH         ((uint32_t)0x00000050U) /*!< PH[1] pin */

/** 
  * @brief  EXTI2 configuration  
  */
#define SYSCFG_EXTICR1_EXTI2_PA         ((uint32_t)0x00000000U) /*!< PA[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PB         ((uint32_t)0x00000100U) /*!< PB[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PC         ((uint32_t)0x00000200U) /*!< PC[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PD         ((uint32_t)0x00000300U) /*!< PD[2] pin */

/** 
  * @brief  EXTI3 configuration  
  */
#define SYSCFG_EXTICR1_EXTI3_PA         ((uint32_t)0x00000000U) /*!< PA[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PB         ((uint32_t)0x00001000U) /*!< PB[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PC         ((uint32_t)0x00002000U) /*!< PC[3] pin */

/*****************  Bit definition for SYSCFG_EXTICR2 register  *****************/
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

/** 
  * @brief  EXTI5 configuration  
  */
#define SYSCFG_EXTICR2_EXTI5_PA         ((uint32_t)0x00000000U) /*!< PA[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PB         ((uint32_t)0x00000010U) /*!< PB[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PC         ((uint32_t)0x00000020U) /*!< PC[5] pin */

/** 
  * @brief  EXTI6 configuration  
  */
#define SYSCFG_EXTICR2_EXTI6_PA         ((uint32_t)0x00000000U) /*!< PA[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PB         ((uint32_t)0x00000100U) /*!< PB[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PC         ((uint32_t)0x00000200U) /*!< PC[6] pin */

/** 
  * @brief  EXTI7 configuration  
  */
#define SYSCFG_EXTICR2_EXTI7_PA         ((uint32_t)0x00000000U) /*!< PA[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PB         ((uint32_t)0x00001000U) /*!< PB[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PC         ((uint32_t)0x00002000U) /*!< PC[7] pin */

/*****************  Bit definition for SYSCFG_EXTICR3 register  *****************/
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

/** 
  * @brief  EXTI9 configuration  
  */
#define SYSCFG_EXTICR3_EXTI9_PA         ((uint32_t)0x00000000U) /*!< PA[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PB         ((uint32_t)0x00000010U) /*!< PB[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PC         ((uint32_t)0x00000020U) /*!< PC[9] pin */

/** 
  * @brief  EXTI10 configuration  
  */
#define SYSCFG_EXTICR3_EXTI10_PA        ((uint32_t)0x00000000U) /*!< PA[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PB        ((uint32_t)0x00000100U) /*!< PB[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PC        ((uint32_t)0x00000200U) /*!< PC[10] pin */

/** 
  * @brief  EXTI11 configuration  
  */
#define SYSCFG_EXTICR3_EXTI11_PA        ((uint32_t)0x00000000U) /*!< PA[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PB        ((uint32_t)0x00001000U) /*!< PB[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PC        ((uint32_t)0x00002000U) /*!< PC[11] pin */

/*****************  Bit definition for SYSCFG_EXTICR4 register  *****************/
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

/** 
  * @brief  EXTI13 configuration  
  */
#define SYSCFG_EXTICR4_EXTI13_PA        ((uint32_t)0x00000000U) /*!< PA[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PB        ((uint32_t)0x00000010U) /*!< PB[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PC        ((uint32_t)0x00000020U) /*!< PC[13] pin */

/** 
  * @brief  EXTI14 configuration  
  */
#define SYSCFG_EXTICR4_EXTI14_PA        ((uint32_t)0x00000000U) /*!< PA[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PB        ((uint32_t)0x00000100U) /*!< PB[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PC        ((uint32_t)0x00000200U) /*!< PC[14] pin */

/** 
  * @brief  EXTI15 configuration  
  */
#define SYSCFG_EXTICR4_EXTI15_PA        ((uint32_t)0x00000000U) /*!< PA[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PB        ((uint32_t)0x00001000U) /*!< PB[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PC        ((uint32_t)0x00002000U) /*!< PC[15] pin */


/*****************  Bit definition for SYSCFG_CFGR3 register  ****************/
#define SYSCFG_CFGR3_EN_VREFINT               ((uint32_t)0x00000001U) /*!< Vref Enable bit*/
#define SYSCFG_CFGR3_VREF_OUT                 ((uint32_t)0x00000030U) /*!< Verf_ADC connection bit */
#define SYSCFG_CFGR3_VREF_OUT_0               ((uint32_t)0x00000010U) /*!< Bit 0 */
#define SYSCFG_CFGR3_VREF_OUT_1               ((uint32_t)0x00000020U) /*!< Bit 1 */
#define SYSCFG_CFGR3_ENBUF_VREFINT_ADC        ((uint32_t)0x00000100U) /*!< VREFINT reference for ADC enable bit */
#define SYSCFG_CFGR3_ENBUF_SENSOR_ADC         ((uint32_t)0x00000200U) /*!< Sensor reference for ADC enable bit */
#define SYSCFG_CFGR3_ENBUFLP_VREFINT_COMP     ((uint32_t)0x00001000U) /*!< VREFINT reference for comparator 2 enable bit */
#define SYSCFG_CFGR3_SENSOR_ADC_RDYF          ((uint32_t)0x08000000U) /*!< Sensor for ADC ready flag */
#define SYSCFG_CFGR3_VREFINT_ADC_RDYF         ((uint32_t)0x10000000U) /*!< VREFINT for ADC ready flag */
#define SYSCFG_CFGR3_VREFINT_COMP_RDYF        ((uint32_t)0x20000000U) /*!< VREFINT for comparator ready flag */
#define SYSCFG_CFGR3_VREFINT_RDYF             ((uint32_t)0x40000000U) /*!< VREFINT ready flag */
#define SYSCFG_CFGR3_REF_LOCK                 ((uint32_t)0x80000000U) /*!< CFGR3 lock bit */

/* Legacy defines */

#define SYSCFG_CFGR3_EN_BGAP                  SYSCFG_CFGR3_EN_VREFINT
#define SYSCFG_CFGR3_ENBUF_BGAP_ADC           SYSCFG_CFGR3_ENBUF_VREFINT_ADC
#define SYSCFG_CFGR3_ENBUFLP_BGAP_COMP        SYSCFG_CFGR3_ENBUFLP_VREFINT_COMP
#define SYSCFG_VREFINT_ADC_RDYF               SYSCFG_CFGR3_VREFINT_ADC_RDYF

/******************************************************************************/
/*                                                                            */
/*                               Timers (TIM)                                 */
/*                                                                            */
/******************************************************************************/
/*
* @brief Specific device feature definitions (not present on all devices in the STM32L0 family)
*/
#if defined (STM32L071xx) || defined (STM32L072xx) || defined (STM32L073xx) \
    || defined (STM32L081xx) || defined (STM32L082xx) || defined (STM32L083xx)
#define TIM_TIM2_REMAP_HSI_SUPPORT       /*!<Support remap HSI on TIM2 */
#define TIM_TIM2_REMAP_HSI48_SUPPORT     /*!<Support remap HSI48 on TIM2 */
#else
#define TIM_TIM2_REMAP_HSI_SUPPORT	     /*!<Support remap HSI on TIM2 */
#endif

/*******************  Bit definition for TIM_CR1 register  ********************/
#define TIM_CR1_CEN                         ((uint32_t)0x00000001U)            /*!<Counter enable */
#define TIM_CR1_UDIS                        ((uint32_t)0x00000002U)            /*!<Update disable */
#define TIM_CR1_URS                         ((uint32_t)0x00000004U)            /*!<Update request source */
#define TIM_CR1_OPM                         ((uint32_t)0x00000008U)            /*!<One pulse mode */
#define TIM_CR1_DIR                         ((uint32_t)0x00000010U)            /*!<Direction */

#define TIM_CR1_CMS                         ((uint32_t)0x00000060U)            /*!<CMS[1:0] bits (Center-aligned mode selection) */
#define TIM_CR1_CMS_0                       ((uint32_t)0x00000020U)            /*!<Bit 0 */
#define TIM_CR1_CMS_1                       ((uint32_t)0x00000040U)            /*!<Bit 1 */

#define TIM_CR1_ARPE                        ((uint32_t)0x00000080U)            /*!<Auto-reload preload enable */

#define TIM_CR1_CKD                         ((uint32_t)0x00000300U)            /*!<CKD[1:0] bits (clock division) */
#define TIM_CR1_CKD_0                       ((uint32_t)0x00000100U)            /*!<Bit 0 */
#define TIM_CR1_CKD_1                       ((uint32_t)0x00000200U)            /*!<Bit 1 */

/*******************  Bit definition for TIM_CR2 register  ********************/
#define TIM_CR2_CCDS                        ((uint32_t)0x00000008U)            /*!<Capture/Compare DMA Selection */

#define TIM_CR2_MMS                         ((uint32_t)0x00000070U)            /*!<MMS[2:0] bits (Master Mode Selection) */
#define TIM_CR2_MMS_0                       ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define TIM_CR2_MMS_1                       ((uint32_t)0x00000020U)            /*!<Bit 1 */
#define TIM_CR2_MMS_2                       ((uint32_t)0x00000040U)            /*!<Bit 2 */

#define TIM_CR2_TI1S                        ((uint32_t)0x00000080U)            /*!<TI1 Selection */

/*******************  Bit definition for TIM_SMCR register  *******************/
#define TIM_SMCR_SMS                        ((uint32_t)0x00000007U)            /*!<SMS[2:0] bits (Slave mode selection) */
#define TIM_SMCR_SMS_0                      ((uint32_t)0x00000001U)            /*!<Bit 0 */
#define TIM_SMCR_SMS_1                      ((uint32_t)0x00000002U)            /*!<Bit 1 */
#define TIM_SMCR_SMS_2                      ((uint32_t)0x00000004U)            /*!<Bit 2 */

#define TIM_SMCR_OCCS                       ((uint32_t)0x00000008U)            /*!< OCREF clear selection */

#define TIM_SMCR_TS                         ((uint32_t)0x00000070U)            /*!<TS[2:0] bits (Trigger selection) */
#define TIM_SMCR_TS_0                       ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define TIM_SMCR_TS_1                       ((uint32_t)0x00000020U)            /*!<Bit 1 */
#define TIM_SMCR_TS_2                       ((uint32_t)0x00000040U)            /*!<Bit 2 */

#define TIM_SMCR_MSM                        ((uint32_t)0x00000080U)            /*!<Master/slave mode */

#define TIM_SMCR_ETF                        ((uint32_t)0x00000F00U)            /*!<ETF[3:0] bits (External trigger filter) */
#define TIM_SMCR_ETF_0                      ((uint32_t)0x00000100U)            /*!<Bit 0 */
#define TIM_SMCR_ETF_1                      ((uint32_t)0x00000200U)            /*!<Bit 1 */
#define TIM_SMCR_ETF_2                      ((uint32_t)0x00000400U)            /*!<Bit 2 */
#define TIM_SMCR_ETF_3                      ((uint32_t)0x00000800U)            /*!<Bit 3 */

#define TIM_SMCR_ETPS                       ((uint32_t)0x00003000U)            /*!<ETPS[1:0] bits (External trigger prescaler) */
#define TIM_SMCR_ETPS_0                     ((uint32_t)0x00001000U)            /*!<Bit 0 */
#define TIM_SMCR_ETPS_1                     ((uint32_t)0x00002000U)            /*!<Bit 1 */

#define TIM_SMCR_ECE                        ((uint32_t)0x00004000U)            /*!<External clock enable */
#define TIM_SMCR_ETP                        ((uint32_t)0x00008000U)            /*!<External trigger polarity */

/*******************  Bit definition for TIM_DIER register  *******************/
#define TIM_DIER_UIE                        ((uint32_t)0x00000001U)            /*!<Update interrupt enable */
#define TIM_DIER_CC1IE                      ((uint32_t)0x00000002U)            /*!<Capture/Compare 1 interrupt enable */
#define TIM_DIER_CC2IE                      ((uint32_t)0x00000004U)            /*!<Capture/Compare 2 interrupt enable */
#define TIM_DIER_CC3IE                      ((uint32_t)0x00000008U)            /*!<Capture/Compare 3 interrupt enable */
#define TIM_DIER_CC4IE                      ((uint32_t)0x00000010U)            /*!<Capture/Compare 4 interrupt enable */
#define TIM_DIER_TIE                        ((uint32_t)0x00000040U)            /*!<Trigger interrupt enable */
#define TIM_DIER_UDE                        ((uint32_t)0x00000100U)            /*!<Update DMA request enable */
#define TIM_DIER_CC1DE                      ((uint32_t)0x00000200U)            /*!<Capture/Compare 1 DMA request enable */
#define TIM_DIER_CC2DE                      ((uint32_t)0x00000400U)            /*!<Capture/Compare 2 DMA request enable */
#define TIM_DIER_CC3DE                      ((uint32_t)0x00000800U)            /*!<Capture/Compare 3 DMA request enable */
#define TIM_DIER_CC4DE                      ((uint32_t)0x00001000U)            /*!<Capture/Compare 4 DMA request enable */
#define TIM_DIER_TDE                        ((uint32_t)0x00004000U)            /*!<Trigger DMA request enable */

/********************  Bit definition for TIM_SR register  ********************/
#define TIM_SR_UIF                          ((uint32_t)0x00000001U)            /*!<Update interrupt Flag */
#define TIM_SR_CC1IF                        ((uint32_t)0x00000002U)            /*!<Capture/Compare 1 interrupt Flag */
#define TIM_SR_CC2IF                        ((uint32_t)0x00000004U)            /*!<Capture/Compare 2 interrupt Flag */
#define TIM_SR_CC3IF                        ((uint32_t)0x00000008U)            /*!<Capture/Compare 3 interrupt Flag */
#define TIM_SR_CC4IF                        ((uint32_t)0x00000010U)            /*!<Capture/Compare 4 interrupt Flag */
#define TIM_SR_TIF                          ((uint32_t)0x00000040U)            /*!<Trigger interrupt Flag */
#define TIM_SR_CC1OF                        ((uint32_t)0x00000200U)            /*!<Capture/Compare 1 Overcapture Flag */
#define TIM_SR_CC2OF                        ((uint32_t)0x00000400U)            /*!<Capture/Compare 2 Overcapture Flag */
#define TIM_SR_CC3OF                        ((uint32_t)0x00000800U)            /*!<Capture/Compare 3 Overcapture Flag */
#define TIM_SR_CC4OF                        ((uint32_t)0x00001000U)            /*!<Capture/Compare 4 Overcapture Flag */

/*******************  Bit definition for TIM_EGR register  ********************/
#define TIM_EGR_UG                          ((uint32_t)0x00000001U)               /*!<Update Generation */
#define TIM_EGR_CC1G                        ((uint32_t)0x00000002U)               /*!<Capture/Compare 1 Generation */
#define TIM_EGR_CC2G                        ((uint32_t)0x00000004U)               /*!<Capture/Compare 2 Generation */
#define TIM_EGR_CC3G                        ((uint32_t)0x00000008U)               /*!<Capture/Compare 3 Generation */
#define TIM_EGR_CC4G                        ((uint32_t)0x00000010U)               /*!<Capture/Compare 4 Generation */
#define TIM_EGR_TG                          ((uint32_t)0x00000040U)               /*!<Trigger Generation */

/******************  Bit definition for TIM_CCMR1 register  *******************/
#define TIM_CCMR1_CC1S                      ((uint32_t)0x00000003U)            /*!<CC1S[1:0] bits (Capture/Compare 1 Selection) */
#define TIM_CCMR1_CC1S_0                    ((uint32_t)0x00000001U)            /*!<Bit 0 */
#define TIM_CCMR1_CC1S_1                    ((uint32_t)0x00000002U)            /*!<Bit 1 */

#define TIM_CCMR1_OC1FE                     ((uint32_t)0x00000004U)            /*!<Output Compare 1 Fast enable */
#define TIM_CCMR1_OC1PE                     ((uint32_t)0x00000008U)            /*!<Output Compare 1 Preload enable */

#define TIM_CCMR1_OC1M                      ((uint32_t)0x00000070U)            /*!<OC1M[2:0] bits (Output Compare 1 Mode) */
#define TIM_CCMR1_OC1M_0                    ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define TIM_CCMR1_OC1M_1                    ((uint32_t)0x00000020U)            /*!<Bit 1 */
#define TIM_CCMR1_OC1M_2                    ((uint32_t)0x00000040U)            /*!<Bit 2 */

#define TIM_CCMR1_OC1CE                     ((uint32_t)0x00000080U)            /*!<Output Compare 1Clear Enable */

#define TIM_CCMR1_CC2S                      ((uint32_t)0x00000300U)            /*!<CC2S[1:0] bits (Capture/Compare 2 Selection) */
#define TIM_CCMR1_CC2S_0                    ((uint32_t)0x00000100U)            /*!<Bit 0 */
#define TIM_CCMR1_CC2S_1                    ((uint32_t)0x00000200U)            /*!<Bit 1 */

#define TIM_CCMR1_OC2FE                     ((uint32_t)0x00000400U)            /*!<Output Compare 2 Fast enable */
#define TIM_CCMR1_OC2PE                     ((uint32_t)0x00000800U)            /*!<Output Compare 2 Preload enable */

#define TIM_CCMR1_OC2M                      ((uint32_t)0x00007000U)            /*!<OC2M[2:0] bits (Output Compare 2 Mode) */
#define TIM_CCMR1_OC2M_0                    ((uint32_t)0x00001000U)            /*!<Bit 0 */
#define TIM_CCMR1_OC2M_1                    ((uint32_t)0x00002000U)            /*!<Bit 1 */
#define TIM_CCMR1_OC2M_2                    ((uint32_t)0x00004000U)            /*!<Bit 2 */

#define TIM_CCMR1_OC2CE                     ((uint32_t)0x00008000U)            /*!<Output Compare 2 Clear Enable */

/*----------------------------------------------------------------------------*/

#define TIM_CCMR1_IC1PSC                    ((uint32_t)0x0000000CU)            /*!<IC1PSC[1:0] bits (Input Capture 1 Prescaler) */
#define TIM_CCMR1_IC1PSC_0                  ((uint32_t)0x00000004U)            /*!<Bit 0 */
#define TIM_CCMR1_IC1PSC_1                  ((uint32_t)0x00000008U)            /*!<Bit 1 */

#define TIM_CCMR1_IC1F                      ((uint32_t)0x000000F0U)            /*!<IC1F[3:0] bits (Input Capture 1 Filter) */
#define TIM_CCMR1_IC1F_0                    ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define TIM_CCMR1_IC1F_1                    ((uint32_t)0x00000020U)            /*!<Bit 1 */
#define TIM_CCMR1_IC1F_2                    ((uint32_t)0x00000040U)            /*!<Bit 2 */
#define TIM_CCMR1_IC1F_3                    ((uint32_t)0x00000080U)            /*!<Bit 3 */

#define TIM_CCMR1_IC2PSC                    ((uint32_t)0x00000C00U)            /*!<IC2PSC[1:0] bits (Input Capture 2 Prescaler) */
#define TIM_CCMR1_IC2PSC_0                  ((uint32_t)0x00000400U)            /*!<Bit 0 */
#define TIM_CCMR1_IC2PSC_1                  ((uint32_t)0x00000800U)            /*!<Bit 1 */

#define TIM_CCMR1_IC2F                      ((uint32_t)0x0000F000U)            /*!<IC2F[3:0] bits (Input Capture 2 Filter) */
#define TIM_CCMR1_IC2F_0                    ((uint32_t)0x00001000U)            /*!<Bit 0 */
#define TIM_CCMR1_IC2F_1                    ((uint32_t)0x00002000U)            /*!<Bit 1 */
#define TIM_CCMR1_IC2F_2                    ((uint32_t)0x00004000U)            /*!<Bit 2 */
#define TIM_CCMR1_IC2F_3                    ((uint32_t)0x00008000U)            /*!<Bit 3 */

/******************  Bit definition for TIM_CCMR2 register  *******************/
#define TIM_CCMR2_CC3S                      ((uint32_t)0x00000003U)            /*!<CC3S[1:0] bits (Capture/Compare 3 Selection) */
#define TIM_CCMR2_CC3S_0                    ((uint32_t)0x00000001U)            /*!<Bit 0 */
#define TIM_CCMR2_CC3S_1                    ((uint32_t)0x00000002U)            /*!<Bit 1 */

#define TIM_CCMR2_OC3FE                     ((uint32_t)0x00000004U)            /*!<Output Compare 3 Fast enable */
#define TIM_CCMR2_OC3PE                     ((uint32_t)0x00000008U)            /*!<Output Compare 3 Preload enable */

#define TIM_CCMR2_OC3M                      ((uint32_t)0x00000070U)            /*!<OC3M[2:0] bits (Output Compare 3 Mode) */
#define TIM_CCMR2_OC3M_0                    ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define TIM_CCMR2_OC3M_1                    ((uint32_t)0x00000020U)            /*!<Bit 1 */
#define TIM_CCMR2_OC3M_2                    ((uint32_t)0x00000040U)            /*!<Bit 2 */

#define TIM_CCMR2_OC3CE                     ((uint32_t)0x00000080U)            /*!<Output Compare 3 Clear Enable */

#define TIM_CCMR2_CC4S                      ((uint32_t)0x00000300U)            /*!<CC4S[1:0] bits (Capture/Compare 4 Selection) */
#define TIM_CCMR2_CC4S_0                    ((uint32_t)0x00000100U)            /*!<Bit 0 */
#define TIM_CCMR2_CC4S_1                    ((uint32_t)0x00000200U)            /*!<Bit 1 */

#define TIM_CCMR2_OC4FE                     ((uint32_t)0x00000400U)            /*!<Output Compare 4 Fast enable */
#define TIM_CCMR2_OC4PE                     ((uint32_t)0x00000800U)            /*!<Output Compare 4 Preload enable */

#define TIM_CCMR2_OC4M                      ((uint32_t)0x00007000U)            /*!<OC4M[2:0] bits (Output Compare 4 Mode) */
#define TIM_CCMR2_OC4M_0                    ((uint32_t)0x00001000U)            /*!<Bit 0 */
#define TIM_CCMR2_OC4M_1                    ((uint32_t)0x00002000U)            /*!<Bit 1 */
#define TIM_CCMR2_OC4M_2                    ((uint32_t)0x00004000U)            /*!<Bit 2 */

#define TIM_CCMR2_OC4CE                     ((uint32_t)0x00008000U)            /*!<Output Compare 4 Clear Enable */

/*----------------------------------------------------------------------------*/

#define TIM_CCMR2_IC3PSC                    ((uint32_t)0x0000000CU)            /*!<IC3PSC[1:0] bits (Input Capture 3 Prescaler) */
#define TIM_CCMR2_IC3PSC_0                  ((uint32_t)0x00000004U)            /*!<Bit 0 */
#define TIM_CCMR2_IC3PSC_1                  ((uint32_t)0x00000008U)            /*!<Bit 1 */

#define TIM_CCMR2_IC3F                      ((uint32_t)0x000000F0U)            /*!<IC3F[3:0] bits (Input Capture 3 Filter) */
#define TIM_CCMR2_IC3F_0                    ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define TIM_CCMR2_IC3F_1                    ((uint32_t)0x00000020U)            /*!<Bit 1 */
#define TIM_CCMR2_IC3F_2                    ((uint32_t)0x00000040U)            /*!<Bit 2 */
#define TIM_CCMR2_IC3F_3                    ((uint32_t)0x00000080U)            /*!<Bit 3 */

#define TIM_CCMR2_IC4PSC                    ((uint32_t)0x00000C00U)            /*!<IC4PSC[1:0] bits (Input Capture 4 Prescaler) */
#define TIM_CCMR2_IC4PSC_0                  ((uint32_t)0x00000400U)            /*!<Bit 0 */
#define TIM_CCMR2_IC4PSC_1                  ((uint32_t)0x00000800U)            /*!<Bit 1 */

#define TIM_CCMR2_IC4F                      ((uint32_t)0x0000F000U)            /*!<IC4F[3:0] bits (Input Capture 4 Filter) */
#define TIM_CCMR2_IC4F_0                    ((uint32_t)0x00001000U)            /*!<Bit 0 */
#define TIM_CCMR2_IC4F_1                    ((uint32_t)0x00002000U)            /*!<Bit 1 */
#define TIM_CCMR2_IC4F_2                    ((uint32_t)0x00004000U)            /*!<Bit 2 */
#define TIM_CCMR2_IC4F_3                    ((uint32_t)0x00008000U)            /*!<Bit 3 */

/*******************  Bit definition for TIM_CCER register  *******************/
#define TIM_CCER_CC1E                       ((uint32_t)0x00000001U)            /*!<Capture/Compare 1 output enable */
#define TIM_CCER_CC1P                       ((uint32_t)0x00000002U)            /*!<Capture/Compare 1 output Polarity */
#define TIM_CCER_CC1NP                      ((uint32_t)0x00000008U)            /*!<Capture/Compare 1 Complementary output Polarity */
#define TIM_CCER_CC2E                       ((uint32_t)0x00000010U)            /*!<Capture/Compare 2 output enable */
#define TIM_CCER_CC2P                       ((uint32_t)0x00000020U)            /*!<Capture/Compare 2 output Polarity */
#define TIM_CCER_CC2NP                      ((uint32_t)0x00000080U)            /*!<Capture/Compare 2 Complementary output Polarity */
#define TIM_CCER_CC3E                       ((uint32_t)0x00000100U)            /*!<Capture/Compare 3 output enable */
#define TIM_CCER_CC3P                       ((uint32_t)0x00000200U)            /*!<Capture/Compare 3 output Polarity */
#define TIM_CCER_CC3NP                      ((uint32_t)0x00000800U)            /*!<Capture/Compare 3 Complementary output Polarity */
#define TIM_CCER_CC4E                       ((uint32_t)0x00001000U)            /*!<Capture/Compare 4 output enable */
#define TIM_CCER_CC4P                       ((uint32_t)0x00002000U)            /*!<Capture/Compare 4 output Polarity */
#define TIM_CCER_CC4NP                      ((uint32_t)0x00008000U)            /*!<Capture/Compare 4 Complementary output Polarity */

/*******************  Bit definition for TIM_CNT register  ********************/
#define TIM_CNT_CNT                         ((uint32_t)0x0000FFFFU)            /*!<Counter Value */

/*******************  Bit definition for TIM_PSC register  ********************/
#define TIM_PSC_PSC                         ((uint32_t)0x0000FFFFU)            /*!<Prescaler Value */

/*******************  Bit definition for TIM_ARR register  ********************/
#define TIM_ARR_ARR                         ((uint32_t)0x0000FFFFU)            /*!<actual auto-reload Value */

/*******************  Bit definition for TIM_CCR1 register  *******************/
#define TIM_CCR1_CCR1                       ((uint32_t)0x0000FFFFU)            /*!<Capture/Compare 1 Value */

/*******************  Bit definition for TIM_CCR2 register  *******************/
#define TIM_CCR2_CCR2                       ((uint32_t)0x0000FFFFU)            /*!<Capture/Compare 2 Value */

/*******************  Bit definition for TIM_CCR3 register  *******************/
#define TIM_CCR3_CCR3                       ((uint32_t)0x0000FFFFU)            /*!<Capture/Compare 3 Value */

/*******************  Bit definition for TIM_CCR4 register  *******************/
#define TIM_CCR4_CCR4                       ((uint32_t)0x0000FFFFU)            /*!<Capture/Compare 4 Value */

/*******************  Bit definition for TIM_DCR register  ********************/
#define TIM_DCR_DBA                         ((uint32_t)0x0000001FU)            /*!<DBA[4:0] bits (DMA Base Address) */
#define TIM_DCR_DBA_0                       ((uint32_t)0x00000001U)            /*!<Bit 0 */
#define TIM_DCR_DBA_1                       ((uint32_t)0x00000002U)            /*!<Bit 1 */
#define TIM_DCR_DBA_2                       ((uint32_t)0x00000004U)            /*!<Bit 2 */
#define TIM_DCR_DBA_3                       ((uint32_t)0x00000008U)            /*!<Bit 3 */
#define TIM_DCR_DBA_4                       ((uint32_t)0x00000010U)            /*!<Bit 4 */

#define TIM_DCR_DBL                         ((uint32_t)0x00001F00U)            /*!<DBL[4:0] bits (DMA Burst Length) */
#define TIM_DCR_DBL_0                       ((uint32_t)0x00000100U)            /*!<Bit 0 */
#define TIM_DCR_DBL_1                       ((uint32_t)0x00000200U)            /*!<Bit 1 */
#define TIM_DCR_DBL_2                       ((uint32_t)0x00000400U)            /*!<Bit 2 */
#define TIM_DCR_DBL_3                       ((uint32_t)0x00000800U)            /*!<Bit 3 */
#define TIM_DCR_DBL_4                       ((uint32_t)0x00001000U)            /*!<Bit 4 */

/*******************  Bit definition for TIM_DMAR register  *******************/
#define TIM_DMAR_DMAB                       ((uint32_t)0x0000FFFFU)            /*!<DMA register for burst accesses */

/*******************  Bit definition for TIM_OR register  *********************/
#define TIM2_OR_ETR_RMP                     ((uint32_t)0x00000007U)            /*!<ETR_RMP[1:0] bits (TIM2 ETR remap) */
#define TIM2_OR_ETR_RMP_0                   ((uint32_t)0x00000001U)            /*!<Bit 0 */
#define TIM2_OR_ETR_RMP_1                   ((uint32_t)0x00000002U)            /*!<Bit 1 */
#define TIM2_OR_ETR_RMP_2                   ((uint32_t)0x00000004U)            /*!<Bit 2 */
#define TIM2_OR_TI4_RMP                     ((uint32_t)0x0000018)             /*!<TI4_RMP[1:0] bits (TIM2 Input 4 remap) */
#define TIM2_OR_TI4_RMP_0                   ((uint32_t)0x00000008U)            /*!<Bit 0 */
#define TIM2_OR_TI4_RMP_1                   ((uint32_t)0x00000010U)            /*!<Bit 1 */

#define TIM21_OR_ETR_RMP                    ((uint32_t)0x00000003U)            /*!<ETR_RMP[1:0] bits (TIM21 ETR remap) */
#define TIM21_OR_ETR_RMP_0                  ((uint32_t)0x00000001U)            /*!<Bit 0 */
#define TIM21_OR_ETR_RMP_1                  ((uint32_t)0x00000002U)            /*!<Bit 1 */
#define TIM21_OR_TI1_RMP                    ((uint32_t)0x0000001CU)            /*!<TI1_RMP[2:0] bits (TIM21 Input 1 remap) */
#define TIM21_OR_TI1_RMP_0                  ((uint32_t)0x00000004U)            /*!<Bit 0 */
#define TIM21_OR_TI1_RMP_1                  ((uint32_t)0x00000008U)            /*!<Bit 1 */
#define TIM21_OR_TI1_RMP_2                  ((uint32_t)0x00000010U)            /*!<Bit 2 */
#define TIM21_OR_TI2_RMP                    ((uint32_t)0x00000020U)            /*!<TI2_RMP bit (TIM21 Input 2 remap) */


/******************************************************************************/
/*                                                                            */
/*      Universal Synchronous Asynchronous Receiver Transmitter (USART)       */
/*                                                                            */
/******************************************************************************/

/*
 * @brief Specific device feature definitions (not present on all devices in the STM32L0 family)
 */
#define USART_TCBGT_SUPPORT                       /*!< Transmission completed before guard time interrupt support */

/******************  Bit definition for USART_CR1 register  *******************/
#define USART_CR1_UE                        ((uint32_t)0x00000001U)            /*!< USART Enable */
#define USART_CR1_UESM                      ((uint32_t)0x00000002U)            /*!< USART Enable in STOP Mode */
#define USART_CR1_RE                        ((uint32_t)0x00000004U)            /*!< Receiver Enable */
#define USART_CR1_TE                        ((uint32_t)0x00000008U)            /*!< Transmitter Enable */
#define USART_CR1_IDLEIE                    ((uint32_t)0x00000010U)            /*!< IDLE Interrupt Enable */
#define USART_CR1_RXNEIE                    ((uint32_t)0x00000020U)            /*!< RXNE Interrupt Enable */
#define USART_CR1_TCIE                      ((uint32_t)0x00000040U)            /*!< Transmission Complete Interrupt Enable */
#define USART_CR1_TXEIE                     ((uint32_t)0x00000080U)            /*!< TXE Interrupt Enable */
#define USART_CR1_PEIE                      ((uint32_t)0x00000100U)            /*!< PE Interrupt Enable */
#define USART_CR1_PS                        ((uint32_t)0x00000200U)            /*!< Parity Selection */
#define USART_CR1_PCE                       ((uint32_t)0x00000400U)            /*!< Parity Control Enable */
#define USART_CR1_WAKE                      ((uint32_t)0x00000800U)            /*!< Receiver Wakeup method */
#define USART_CR1_M                         ((uint32_t)0x10001000U)            /*!< Word length */
#define USART_CR1_M0                        ((uint32_t)0x00001000U)            /*!< Word length - Bit 0 */
#define USART_CR1_MME                       ((uint32_t)0x00002000U)            /*!< Mute Mode Enable */
#define USART_CR1_CMIE                      ((uint32_t)0x00004000U)            /*!< Character match interrupt enable */
#define USART_CR1_OVER8                     ((uint32_t)0x00008000U)            /*!< Oversampling by 8-bit or 16-bit mode */
#define USART_CR1_DEDT                      ((uint32_t)0x001F0000U)            /*!< DEDT[4:0] bits (Driver Enable Deassertion Time) */
#define USART_CR1_DEDT_0                    ((uint32_t)0x00010000U)            /*!< Bit 0 */
#define USART_CR1_DEDT_1                    ((uint32_t)0x00020000U)            /*!< Bit 1 */
#define USART_CR1_DEDT_2                    ((uint32_t)0x00040000U)            /*!< Bit 2 */
#define USART_CR1_DEDT_3                    ((uint32_t)0x00080000U)            /*!< Bit 3 */
#define USART_CR1_DEDT_4                    ((uint32_t)0x00100000U)            /*!< Bit 4 */
#define USART_CR1_DEAT                      ((uint32_t)0x03E00000U)            /*!< DEAT[4:0] bits (Driver Enable Assertion Time) */
#define USART_CR1_DEAT_0                    ((uint32_t)0x00200000U)            /*!< Bit 0 */
#define USART_CR1_DEAT_1                    ((uint32_t)0x00400000U)            /*!< Bit 1 */
#define USART_CR1_DEAT_2                    ((uint32_t)0x00800000U)            /*!< Bit 2 */
#define USART_CR1_DEAT_3                    ((uint32_t)0x01000000U)            /*!< Bit 3 */
#define USART_CR1_DEAT_4                    ((uint32_t)0x02000000U)            /*!< Bit 4 */
#define USART_CR1_RTOIE                     ((uint32_t)0x04000000U)            /*!< Receive Time Out interrupt enable */
#define USART_CR1_EOBIE                     ((uint32_t)0x08000000U)            /*!< End of Block interrupt enable */
#define USART_CR1_M1                        ((uint32_t)0x10000000U)            /*!< Word length - Bit 1 */
/******************  Bit definition for USART_CR2 register  *******************/
#define USART_CR2_ADDM7                     ((uint32_t)0x00000010U)            /*!< 7-bit or 4-bit Address Detection */
#define USART_CR2_LBDL                      ((uint32_t)0x00000020U)            /*!< LIN Break Detection Length */
#define USART_CR2_LBDIE                     ((uint32_t)0x00000040U)            /*!< LIN Break Detection Interrupt Enable */
#define USART_CR2_LBCL                      ((uint32_t)0x00000100U)            /*!< Last Bit Clock pulse */
#define USART_CR2_CPHA                      ((uint32_t)0x00000200U)            /*!< Clock Phase */
#define USART_CR2_CPOL                      ((uint32_t)0x00000400U)            /*!< Clock Polarity */
#define USART_CR2_CLKEN                     ((uint32_t)0x00000800U)            /*!< Clock Enable */
#define USART_CR2_STOP                      ((uint32_t)0x00003000U)            /*!< STOP[1:0] bits (STOP bits) */
#define USART_CR2_STOP_0                    ((uint32_t)0x00001000U)            /*!< Bit 0 */
#define USART_CR2_STOP_1                    ((uint32_t)0x00002000U)            /*!< Bit 1 */
#define USART_CR2_LINEN                     ((uint32_t)0x00004000U)            /*!< LIN mode enable */
#define USART_CR2_SWAP                      ((uint32_t)0x00008000U)            /*!< SWAP TX/RX pins */
#define USART_CR2_RXINV                     ((uint32_t)0x00010000U)            /*!< RX pin active level inversion */
#define USART_CR2_TXINV                     ((uint32_t)0x00020000U)            /*!< TX pin active level inversion */
#define USART_CR2_DATAINV                   ((uint32_t)0x00040000U)            /*!< Binary data inversion */
#define USART_CR2_MSBFIRST                  ((uint32_t)0x00080000U)            /*!< Most Significant Bit First */
#define USART_CR2_ABREN                     ((uint32_t)0x00100000U)            /*!< Auto Baud-Rate Enable*/
#define USART_CR2_ABRMODE                   ((uint32_t)0x00600000U)            /*!< ABRMOD[1:0] bits (Auto Baud-Rate Mode) */
#define USART_CR2_ABRMODE_0                 ((uint32_t)0x00200000U)            /*!< Bit 0 */
#define USART_CR2_ABRMODE_1                 ((uint32_t)0x00400000U)            /*!< Bit 1 */
#define USART_CR2_RTOEN                     ((uint32_t)0x00800000U)            /*!< Receiver Time-Out enable */
#define USART_CR2_ADD                       ((uint32_t)0xFF000000U)            /*!< Address of the USART node */

/******************  Bit definition for USART_CR3 register  *******************/
#define USART_CR3_EIE                       ((uint32_t)0x00000001U)            /*!< Error Interrupt Enable */
#define USART_CR3_IREN                      ((uint32_t)0x00000002U)            /*!< IrDA mode Enable */
#define USART_CR3_IRLP                      ((uint32_t)0x00000004U)            /*!< IrDA Low-Power */
#define USART_CR3_HDSEL                     ((uint32_t)0x00000008U)            /*!< Half-Duplex Selection */
#define USART_CR3_NACK                      ((uint32_t)0x00000010U)            /*!< SmartCard NACK enable */
#define USART_CR3_SCEN                      ((uint32_t)0x00000020U)            /*!< SmartCard mode enable */
#define USART_CR3_DMAR                      ((uint32_t)0x00000040U)            /*!< DMA Enable Receiver */
#define USART_CR3_DMAT                      ((uint32_t)0x00000080U)            /*!< DMA Enable Transmitter */
#define USART_CR3_RTSE                      ((uint32_t)0x00000100U)            /*!< RTS Enable */
#define USART_CR3_CTSE                      ((uint32_t)0x00000200U)            /*!< CTS Enable */
#define USART_CR3_CTSIE                     ((uint32_t)0x00000400U)            /*!< CTS Interrupt Enable */
#define USART_CR3_ONEBIT                    ((uint32_t)0x00000800U)            /*!< One sample bit method enable */
#define USART_CR3_OVRDIS                    ((uint32_t)0x00001000U)            /*!< Overrun Disable */
#define USART_CR3_DDRE                      ((uint32_t)0x00002000U)            /*!< DMA Disable on Reception Error */
#define USART_CR3_DEM                       ((uint32_t)0x00004000U)            /*!< Driver Enable Mode */
#define USART_CR3_DEP                       ((uint32_t)0x00008000U)            /*!< Driver Enable Polarity Selection */
#define USART_CR3_SCARCNT                   ((uint32_t)0x000E0000U)            /*!< SCARCNT[2:0] bits (SmartCard Auto-Retry Count) */
#define USART_CR3_SCARCNT_0                 ((uint32_t)0x00020000U)            /*!< Bit 0 */
#define USART_CR3_SCARCNT_1                 ((uint32_t)0x00040000U)            /*!< Bit 1 */
#define USART_CR3_SCARCNT_2                 ((uint32_t)0x00080000U)            /*!< Bit 2 */
#define USART_CR3_WUS                       ((uint32_t)0x00300000U)            /*!< WUS[1:0] bits (Wake UP Interrupt Flag Selection) */
#define USART_CR3_WUS_0                     ((uint32_t)0x00100000U)            /*!< Bit 0 */
#define USART_CR3_WUS_1                     ((uint32_t)0x00200000U)            /*!< Bit 1 */
#define USART_CR3_WUFIE                     ((uint32_t)0x00400000U)            /*!< Wake Up Interrupt Enable */
#define USART_CR3_UCESM                     ((uint32_t)0x00800000U)            /*!< Clock Enable in Stop mode */ 
#define USART_CR3_TCBGTIE                   ((uint32_t)0x01000000U)            /*!< Transmission Complete Before Guard Time Interrupt Enable */

/******************  Bit definition for USART_BRR register  *******************/
#define USART_BRR_DIV_FRACTION              ((uint32_t)0x0000000FU)            /*!< Fraction of USARTDIV */
#define USART_BRR_DIV_MANTISSA              ((uint32_t)0x0000FFF0U)            /*!< Mantissa of USARTDIV */

/******************  Bit definition for USART_GTPR register  ******************/
#define USART_GTPR_PSC                      ((uint32_t)0x000000FFU)            /*!< PSC[7:0] bits (Prescaler value) */
#define USART_GTPR_GT                       ((uint32_t)0x0000FF00U)            /*!< GT[7:0] bits (Guard time value) */


/*******************  Bit definition for USART_RTOR register  *****************/
#define USART_RTOR_RTO                      ((uint32_t)0x00FFFFFFU)            /*!< Receiver Time Out Value */
#define USART_RTOR_BLEN                     ((uint32_t)0xFF000000U)            /*!< Block Length */

/*******************  Bit definition for USART_RQR register  ******************/
#define USART_RQR_ABRRQ                     ((uint32_t)0x00000001U)            /*!< Auto-Baud Rate Request */
#define USART_RQR_SBKRQ                     ((uint32_t)0x00000002U)            /*!< Send Break Request */
#define USART_RQR_MMRQ                      ((uint32_t)0x00000004U)            /*!< Mute Mode Request */
#define USART_RQR_RXFRQ                     ((uint32_t)0x00000008U)            /*!< Receive Data flush Request */
#define USART_RQR_TXFRQ                     ((uint32_t)0x00000010U)            /*!< Transmit data flush Request */

/*******************  Bit definition for USART_ISR register  ******************/
#define USART_ISR_PE                        ((uint32_t)0x00000001U)            /*!< Parity Error */
#define USART_ISR_FE                        ((uint32_t)0x00000002U)            /*!< Framing Error */
#define USART_ISR_NE                        ((uint32_t)0x00000004U)            /*!< Noise detected Flag */
#define USART_ISR_ORE                       ((uint32_t)0x00000008U)            /*!< OverRun Error */
#define USART_ISR_IDLE                      ((uint32_t)0x00000010U)            /*!< IDLE line detected */
#define USART_ISR_RXNE                      ((uint32_t)0x00000020U)            /*!< Read Data Register Not Empty */
#define USART_ISR_TC                        ((uint32_t)0x00000040U)            /*!< Transmission Complete */
#define USART_ISR_TXE                       ((uint32_t)0x00000080U)            /*!< Transmit Data Register Empty */
#define USART_ISR_LBDF                      ((uint32_t)0x00000100U)            /*!< LIN Break Detection Flag */
#define USART_ISR_CTSIF                     ((uint32_t)0x00000200U)            /*!< CTS interrupt flag */
#define USART_ISR_CTS                       ((uint32_t)0x00000400U)            /*!< CTS flag */
#define USART_ISR_RTOF                      ((uint32_t)0x00000800U)            /*!< Receiver Time Out */
#define USART_ISR_EOBF                      ((uint32_t)0x00001000U)            /*!< End Of Block Flag */
#define USART_ISR_ABRE                      ((uint32_t)0x00004000U)            /*!< Auto-Baud Rate Error */
#define USART_ISR_ABRF                      ((uint32_t)0x00008000U)            /*!< Auto-Baud Rate Flag */
#define USART_ISR_BUSY                      ((uint32_t)0x00010000U)            /*!< Busy Flag */
#define USART_ISR_CMF                       ((uint32_t)0x00020000U)            /*!< Character Match Flag */
#define USART_ISR_SBKF                      ((uint32_t)0x00040000U)            /*!< Send Break Flag */
#define USART_ISR_RWU                       ((uint32_t)0x00080000U)            /*!< Receive Wake Up from mute mode Flag */
#define USART_ISR_WUF                       ((uint32_t)0x00100000U)            /*!< Wake Up from stop mode Flag */
#define USART_ISR_TEACK                     ((uint32_t)0x00200000U)            /*!< Transmit Enable Acknowledge Flag */
#define USART_ISR_REACK                     ((uint32_t)0x00400000U)            /*!< Receive Enable Acknowledge Flag */
#define USART_ISR_TCBGT                     ((uint32_t)0x02000000U)            /*!< Transmission Complete Before Guard Time Completion Flag */

/*******************  Bit definition for USART_ICR register  ******************/
#define USART_ICR_PECF                      ((uint32_t)0x00000001U)            /*!< Parity Error Clear Flag */
#define USART_ICR_FECF                      ((uint32_t)0x00000002U)            /*!< Framing Error Clear Flag */
#define USART_ICR_NCF                       ((uint32_t)0x00000004U)            /*!< Noise detected Clear Flag */
#define USART_ICR_ORECF                     ((uint32_t)0x00000008U)            /*!< OverRun Error Clear Flag */
#define USART_ICR_IDLECF                    ((uint32_t)0x00000010U)            /*!< IDLE line detected Clear Flag */
#define USART_ICR_TCCF                      ((uint32_t)0x00000040U)            /*!< Transmission Complete Clear Flag */
#define USART_ICR_TCBGTCF                   ((uint32_t)0x00000080U)            /*!< Transmission Complete Before Guard Time Clear Flag */
#define USART_ICR_LBDCF                     ((uint32_t)0x00000100U)            /*!< LIN Break Detection Clear Flag */
#define USART_ICR_CTSCF                     ((uint32_t)0x00000200U)            /*!< CTS Interrupt Clear Flag */
#define USART_ICR_RTOCF                     ((uint32_t)0x00000800U)            /*!< Receiver Time Out Clear Flag */
#define USART_ICR_EOBCF                     ((uint32_t)0x00001000U)            /*!< End Of Block Clear Flag */
#define USART_ICR_CMCF                      ((uint32_t)0x00020000U)            /*!< Character Match Clear Flag */
#define USART_ICR_WUCF                      ((uint32_t)0x00100000U)            /*!< Wake Up from stop mode Clear Flag */

/*******************  Bit definition for USART_RDR register  ******************/
#define USART_RDR_RDR                       ((uint32_t)0x000001FFU)            /*!< RDR[8:0] bits (Receive Data value) */

/*******************  Bit definition for USART_TDR register  ******************/
#define USART_TDR_TDR                       ((uint32_t)0x000001FFU)            /*!< TDR[8:0] bits (Transmit Data value) */

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
#define  WWDG_CR_T0    WWDG_CR_T_0
#define  WWDG_CR_T1    WWDG_CR_T_1
#define  WWDG_CR_T2    WWDG_CR_T_2
#define  WWDG_CR_T3    WWDG_CR_T_3
#define  WWDG_CR_T4    WWDG_CR_T_4
#define  WWDG_CR_T5    WWDG_CR_T_5
#define  WWDG_CR_T6    WWDG_CR_T_6

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
#define  WWDG_CFR_W0    WWDG_CFR_W_0
#define  WWDG_CFR_W1    WWDG_CFR_W_1
#define  WWDG_CFR_W2    WWDG_CFR_W_2
#define  WWDG_CFR_W3    WWDG_CFR_W_3
#define  WWDG_CFR_W4    WWDG_CFR_W_4
#define  WWDG_CFR_W5    WWDG_CFR_W_5
#define  WWDG_CFR_W6    WWDG_CFR_W_6

#define  WWDG_CFR_WDGTB                      ((uint32_t)0x00000180U)               /*!< WDGTB[1:0] bits (Timer Base) */
#define  WWDG_CFR_WDGTB_0                    ((uint32_t)0x00000080U)               /*!< Bit 0 */
#define  WWDG_CFR_WDGTB_1                    ((uint32_t)0x00000100U)               /*!< Bit 1 */

/* Legacy defines */
#define  WWDG_CFR_WDGTB0    WWDG_CFR_WDGTB_0
#define  WWDG_CFR_WDGTB1    WWDG_CFR_WDGTB_1

#define  WWDG_CFR_EWI                        ((uint32_t)0x00000200U)               /*!< Early Wakeup Interrupt */

/*******************  Bit definition for WWDG_SR register  ********************/
#define  WWDG_SR_EWIF                        ((uint32_t)0x00000001U)               /*!< Early Wakeup Interrupt Flag */

/**
  * @}
  */ 

/**
  * @}
  */

/** @addtogroup Exported_macros
  * @{
  */

/******************************* ADC Instances ********************************/
#define IS_ADC_ALL_INSTANCE(INSTANCE) ((INSTANCE) == ADC1)

/******************************* COMP Instances *******************************/
#define IS_COMP_ALL_INSTANCE(INSTANCE) (((INSTANCE) == COMP1) || \
                                       ((INSTANCE) == COMP2))

#define IS_COMP_COMMON_INSTANCE(INSTANCE) ((INSTANCE) == COMP12_COMMON)

/******************************* CRC Instances ********************************/
#define IS_CRC_ALL_INSTANCE(INSTANCE) ((INSTANCE) == CRC)

/******************************* DMA Instances *********************************/
#define IS_DMA_STREAM_ALL_INSTANCE(INSTANCE) (((INSTANCE) == DMA1_Stream0) || \
                                              ((INSTANCE) == DMA1_Stream1) || \
                                              ((INSTANCE) == DMA1_Stream2) || \
                                              ((INSTANCE) == DMA1_Stream3) || \
                                              ((INSTANCE) == DMA1_Stream4) || \
                                              ((INSTANCE) == DMA1_Stream5) || \
                                              ((INSTANCE) == DMA1_Stream6) || \
                                              ((INSTANCE) == DMA1_Stream7))

/******************************* GPIO Instances *******************************/
#define IS_GPIO_ALL_INSTANCE(INSTANCE) (((INSTANCE) == GPIOA) || \
                                        ((INSTANCE) == GPIOB) || \
                                        ((INSTANCE) == GPIOC))

#define IS_GPIO_AF_INSTANCE(INSTANCE)  (((INSTANCE) == GPIOA) || \
                                        ((INSTANCE) == GPIOB))

/******************************** I2C Instances *******************************/
#define IS_I2C_ALL_INSTANCE(INSTANCE) ((INSTANCE) == I2C1)



/****************************** RTC Instances *********************************/
#define IS_RTC_ALL_INSTANCE(INSTANCE)  ((INSTANCE) == RTC)

/******************************** SMBUS Instances *****************************/
#define IS_SMBUS_INSTANCE(INSTANCE)  ((INSTANCE) == I2C1)

/******************************** SPI Instances *******************************/
#define IS_SPI_ALL_INSTANCE(INSTANCE) ((INSTANCE) == SPI1)

/****************** LPTIM Instances : All supported instances *****************/
#define IS_LPTIM_INSTANCE(INSTANCE)       ((INSTANCE) == LPTIM1)

/****************** TIM Instances : All supported instances *******************/
#define IS_TIM_INSTANCE(INSTANCE)       (((INSTANCE) == TIM2)   || \
                                         ((INSTANCE) == TIM21))

/****************** TIM Instances : supporting counting mode selection ********/ 
#define IS_TIM_COUNTER_MODE_SELECT_INSTANCE(INSTANCE)       (((INSTANCE) == TIM2)   || \
                                                             ((INSTANCE) == TIM21))

/****************** TIM Instances : supporting clock division *****************/
#define IS_TIM_CLOCK_DIVISION_INSTANCE(INSTANCE)       (((INSTANCE) == TIM2)   || \
                                                        ((INSTANCE) == TIM21))

/****** TIM Instances : supporting external clock mode 1 for ETRF input *******/
#define IS_TIM_CLOCKSOURCE_ETRMODE1_INSTANCE(INSTANCE)   (((INSTANCE) == TIM2)   || \
                                                          ((INSTANCE) == TIM21))

/****** TIM Instances : supporting external clock mode 2 for ETRF input *******/
#define IS_TIM_CLOCKSOURCE_ETRMODE2_INSTANCE(INSTANCE)   (((INSTANCE) == TIM2)   || \
                                                          ((INSTANCE) == TIM21))

/****************** TIM Instances : supporting external clock mode 1 for TIX inputs*/
#define IS_TIM_CLOCKSOURCE_TIX_INSTANCE(INSTANCE)        (((INSTANCE) == TIM2)   || \
                                                          ((INSTANCE) == TIM21))

/****************** TIM Instances : supporting internal trigger inputs(ITRX) *******/
#define IS_TIM_CLOCKSOURCE_ITRX_INSTANCE(INSTANCE)       (((INSTANCE) == TIM2)   || \
                                                          ((INSTANCE) == TIM21))

/************* TIM Instances : at least 1 capture/compare channel *************/
#define IS_TIM_CC1_INSTANCE(INSTANCE)   (((INSTANCE) == TIM2)  || \
                                         ((INSTANCE) == TIM21))

/************ TIM Instances : at least 2 capture/compare channels *************/
#define IS_TIM_CC2_INSTANCE(INSTANCE)  (((INSTANCE) == TIM2)  || \
                                        ((INSTANCE) == TIM21))

/************ TIM Instances : at least 3 capture/compare channels *************/
#define IS_TIM_CC3_INSTANCE(INSTANCE)   ((INSTANCE) == TIM2)

/************ TIM Instances : at least 4 capture/compare channels *************/
#define IS_TIM_CC4_INSTANCE(INSTANCE)   ((INSTANCE) == TIM2)

/******************** TIM Instances : Advanced-control timers *****************/

/******************* TIM Instances : Timer input XOR function *****************/
#define IS_TIM_XOR_INSTANCE(INSTANCE)   ((INSTANCE) == TIM2)

/****************** TIM Instances : DMA requests generation (UDE) *************/
#define IS_TIM_DMA_INSTANCE(INSTANCE)      ((INSTANCE) == TIM2)

/************ TIM Instances : DMA requests generation (CCxDE) *****************/
#define IS_TIM_DMA_CC_INSTANCE(INSTANCE)   ((INSTANCE) == TIM2)

/************ TIM Instances : DMA requests generation (COMDE) *****************/
#define IS_TIM_CCDMA_INSTANCE(INSTANCE)    ((INSTANCE) == TIM2)

/******************** TIM Instances : DMA burst feature ***********************/
#define IS_TIM_DMABURST_INSTANCE(INSTANCE)  ((INSTANCE) == TIM2)

/****** TIM Instances : master mode available (TIMx_CR2.MMS available )********/
#define IS_TIM_MASTER_INSTANCE(INSTANCE)   (((INSTANCE) == TIM2)   || \
                                            ((INSTANCE) == TIM21))

/*********** TIM Instances : Slave mode available (TIMx_SMCR available )*******/
#define IS_TIM_SLAVE_INSTANCE(INSTANCE)    (((INSTANCE) == TIM2)  || \
                                            ((INSTANCE) == TIM21))

/********************** TIM Instances : 32 bit Counter ************************/

/***************** TIM Instances : external trigger input availabe ************/
#define IS_TIM_ETR_INSTANCE(INSTANCE)      (((INSTANCE) == TIM2)  || \
                                            ((INSTANCE) == TIM21))

/****************** TIM Instances : remapping capability **********************/
#define IS_TIM_REMAP_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)   || \
                                         ((INSTANCE) == TIM21))

/****************** TIM Instances : supporting encoder interface **************/
#define IS_TIM_ENCODER_INTERFACE_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)   || \
                                                     ((INSTANCE) == TIM21))

/******************* TIM Instances : output(s) OCXEC register *****************/
#define IS_TIM_OCXREF_CLEAR_INSTANCE(INSTANCE)  ((INSTANCE) == TIM2)
													   
/******************* TIM Instances : output(s) available **********************/
#define IS_TIM_CCX_INSTANCE(INSTANCE, CHANNEL) \
    ((((INSTANCE) == TIM2) &&                  \
     (((CHANNEL) == TIM_CHANNEL_1) ||          \
      ((CHANNEL) == TIM_CHANNEL_2) ||          \
      ((CHANNEL) == TIM_CHANNEL_3) ||          \
      ((CHANNEL) == TIM_CHANNEL_4)))           \
     ||                                        \
     (((INSTANCE) == TIM21) &&                 \
      (((CHANNEL) == TIM_CHANNEL_1) ||         \
       ((CHANNEL) == TIM_CHANNEL_2))))

/******************** UART Instances : Asynchronous mode **********************/
#define IS_UART_INSTANCE(INSTANCE) (((INSTANCE) == USART2) || \
                                    ((INSTANCE) == LPUART1))

/******************** USART Instances : Synchronous mode **********************/
#define IS_USART_INSTANCE(INSTANCE) (1 == 0)

/****************** USART Instances : Auto Baud Rate detection ****************/

#define IS_USART_AUTOBAUDRATE_DETECTION_INSTANCE(INSTANCE) (1 == 0)

/******************** UART Instances : Half-Duplex mode **********************/
#define IS_UART_HALFDUPLEX_INSTANCE(INSTANCE)   (((INSTANCE) == USART2) || \
                                                 ((INSTANCE) == LPUART1))

/******************** UART Instances : LIN mode **********************/
#define IS_UART_LIN_INSTANCE(INSTANCE)    (1==0)

/******************** UART Instances : Wake-up from Stop mode **********************/
#define IS_UART_WAKEUP_FROMSTOP_INSTANCE(INSTANCE)   ((INSTANCE) == LPUART1)
/****************** UART Instances : Hardware Flow control ********************/
#define IS_UART_HWFLOW_INSTANCE(INSTANCE) (((INSTANCE) == USART2) || \
                                           ((INSTANCE) == LPUART1))

/********************* UART Instances : Smard card mode ***********************/
#define IS_SMARTCARD_INSTANCE(INSTANCE) ((INSTANCE) == USART2)

/*********************** UART Instances : IRDA mode ***************************/
#define IS_IRDA_INSTANCE(INSTANCE) ((INSTANCE) == USART2)

/****************************** IWDG Instances ********************************/
#define IS_IWDG_ALL_INSTANCE(INSTANCE)  ((INSTANCE) == IWDG)

/****************************** WWDG Instances ********************************/
#define IS_WWDG_ALL_INSTANCE(INSTANCE)  ((INSTANCE) == WWDG)

/**
  * @}
  */

/******************************************************************************/
/*  For a painless codes migration between the STM32L0xx device product       */
/*  lines, the aliases defined below are put in place to overcome the         */
/*  differences in the interrupt handlers and IRQn definitions.               */
/*  No need to update developed interrupt code when moving across             */ 
/*  product lines within the same STM32L0 Family                              */
/******************************************************************************/

/* Aliases for __IRQn */

#define RNG_LPUART1_IRQn               LPUART1_IRQn
#define AES_LPUART1_IRQn               LPUART1_IRQn
#define AES_RNG_LPUART1_IRQn           LPUART1_IRQn
#define RCC_CRS_IRQn                   RCC_IRQn

/* Aliases for __IRQHandler */
#define RNG_LPUART1_IRQHandler         LPUART1_IRQHandler
#define AES_LPUART1_IRQHandler         LPUART1_IRQHandler
#define AES_RNG_LPUART1_IRQHandler     LPUART1_IRQHandler
#define TIM6_DAC_IRQHandler            TIM6_IRQHandler
#define RCC_CRS_IRQHandler             RCC_IRQHandler

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STM32L011xx_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
