/**
  ******************************************************************************
  * @file    stm32l152xe.h
  * @author  MCD Application Team
  * @version V2.1.3
  * @date    04-March-2016
  * @brief   CMSIS Cortex-M3 Device Peripheral Access Layer Header File. 
  *          This file contains all the peripheral register's definitions, bits 
  *          definitions and memory mapping for STM32L1xx devices.            
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

/** @addtogroup stm32l152xe
  * @{
  */
    
#ifndef __STM32L152xE_H
#define __STM32L152xE_H

#ifdef __cplusplus
 extern "C" {
#endif 
  

  /** @addtogroup Configuration_section_for_CMSIS
  * @{
  */
/**
  * @brief Configuration of the Cortex-M3 Processor and Core Peripherals 
 */
#define __CM3_REV                 0x200U /*!< Cortex-M3 Revision r2p0                  */
#define __MPU_PRESENT             1U     /*!< STM32L1xx provides MPU                          */
#define __NVIC_PRIO_BITS          4U     /*!< STM32L1xx uses 4 Bits for the Priority Levels    */
#define __Vendor_SysTickConfig    0U     /*!< Set to 1 if different SysTick Config is used */
 
/**
  * @}
  */
   
/** @addtogroup Peripheral_interrupt_number_definition
  * @{
  */

/**
 * @brief STM32L1xx Interrupt Number Definition, according to the selected device 
 *        in @ref Library_configuration_section 
 */

 /*!< Interrupt Number Definition */
typedef enum
{
/******  Cortex-M3 Processor Exceptions Numbers ******************************************************/
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                                */
  HardFault_IRQn              = -13,    /*!< 3 Cortex-M3 Hard Fault Interrupt                        */
  MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M3 Memory Management Interrupt                 */
  BusFault_IRQn               = -11,    /*!< 5 Cortex-M3 Bus Fault Interrupt                         */
  UsageFault_IRQn             = -10,    /*!< 6 Cortex-M3 Usage Fault Interrupt                       */
  SVC_IRQn                    = -5,     /*!< 11 Cortex-M3 SV Call Interrupt                          */
  DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M3 Debug Monitor Interrupt                    */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M3 Pend SV Interrupt                          */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M3 System Tick Interrupt                      */

/******  STM32L specific Interrupt Numbers ***********************************************************/
  WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                               */
  PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt               */
  TAMPER_STAMP_IRQn           = 2,      /*!< Tamper and TimeStamp interrupts through the EXTI line   */
  RTC_WKUP_IRQn               = 3,      /*!< RTC Wakeup Timer through EXTI Line Interrupt            */
  FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                                  */
  RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                    */
  EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                    */
  EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                    */
  EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                    */
  EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                    */
  EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                                    */
  DMA1_Channel1_IRQn          = 11,     /*!< DMA1 Channel 1 global Interrupt                         */
  DMA1_Channel2_IRQn          = 12,     /*!< DMA1 Channel 2 global Interrupt                         */
  DMA1_Channel3_IRQn          = 13,     /*!< DMA1 Channel 3 global Interrupt                         */
  DMA1_Channel4_IRQn          = 14,     /*!< DMA1 Channel 4 global Interrupt                         */
  DMA1_Channel5_IRQn          = 15,     /*!< DMA1 Channel 5 global Interrupt                         */
  DMA1_Channel6_IRQn          = 16,     /*!< DMA1 Channel 6 global Interrupt                         */
  DMA1_Channel7_IRQn          = 17,     /*!< DMA1 Channel 7 global Interrupt                         */
  ADC1_IRQn                   = 18,     /*!< ADC1 global Interrupt                                   */
  USB_HP_IRQn                 = 19,     /*!< USB High Priority Interrupt                             */
  USB_LP_IRQn                 = 20,     /*!< USB Low Priority Interrupt                              */
  DAC_IRQn                    = 21,     /*!< DAC Interrupt                                           */
  COMP_IRQn                   = 22,     /*!< Comparator through EXTI Line Interrupt                  */
  EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                           */
  LCD_IRQn                    = 24,     /*!< LCD Interrupt                                           */
  TIM9_IRQn                   = 25,     /*!< TIM9 global Interrupt                                   */
  TIM10_IRQn                  = 26,     /*!< TIM10 global Interrupt                                  */
  TIM11_IRQn                  = 27,     /*!< TIM11 global Interrupt                                  */
  TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                   */
  TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                   */
  TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                   */
  I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                    */
  I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                    */
  I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                    */
  I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                    */
  SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                   */
  SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                   */
  USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                                 */
  USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                                 */
  USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                                 */
  EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                         */
  RTC_Alarm_IRQn              = 41,     /*!< RTC Alarm through EXTI Line Interrupt                   */
  USB_FS_WKUP_IRQn            = 42,     /*!< USB FS WakeUp from suspend through EXTI Line Interrupt  */
  TIM6_IRQn                   = 43,     /*!< TIM6 global Interrupt                                   */
  TIM7_IRQn                   = 44,     /*!< TIM7 global Interrupt                                   */
  TIM5_IRQn                   = 46,     /*!< TIM5 global Interrupt                                   */
  SPI3_IRQn                   = 47,     /*!< SPI3 global Interrupt                                   */
  UART4_IRQn                  = 48,     /*!< UART4 global Interrupt                                  */
  UART5_IRQn                  = 49,     /*!< UART5 global Interrupt                                  */
  DMA2_Channel1_IRQn          = 50,     /*!< DMA2 Channel 1 global Interrupt                         */
  DMA2_Channel2_IRQn          = 51,     /*!< DMA2 Channel 2 global Interrupt                         */
  DMA2_Channel3_IRQn          = 52,     /*!< DMA2 Channel 3 global Interrupt                         */
  DMA2_Channel4_IRQn          = 53,     /*!< DMA2 Channel 4 global Interrupt                         */
  DMA2_Channel5_IRQn          = 54,     /*!< DMA2 Channel 5 global Interrupt                         */
  COMP_ACQ_IRQn               = 56      /*!< Comparator Channel Acquisition global Interrupt         */
} IRQn_Type;

/**
  * @}
  */

#include "core_cm3.h"
#include "system_stm32l1xx.h"
#include <stdint.h>

/** @addtogroup Peripheral_registers_structures
  * @{
  */   

/** 
  * @brief Analog to Digital Converter
  */

typedef struct
{
  __IO uint32_t SR;           /*!< ADC status register,                         Address offset: 0x00 */
  __IO uint32_t CR1;          /*!< ADC control register 1,                      Address offset: 0x04 */
  __IO uint32_t CR2;          /*!< ADC control register 2,                      Address offset: 0x08 */
  __IO uint32_t SMPR1;        /*!< ADC sample time register 1,                  Address offset: 0x0C */
  __IO uint32_t SMPR2;        /*!< ADC sample time register 2,                  Address offset: 0x10 */
  __IO uint32_t SMPR3;        /*!< ADC sample time register 3,                  Address offset: 0x14 */
  __IO uint32_t JOFR1;        /*!< ADC injected channel data offset register 1, Address offset: 0x18 */
  __IO uint32_t JOFR2;        /*!< ADC injected channel data offset register 2, Address offset: 0x1C */
  __IO uint32_t JOFR3;        /*!< ADC injected channel data offset register 3, Address offset: 0x20 */
  __IO uint32_t JOFR4;        /*!< ADC injected channel data offset register 4, Address offset: 0x24 */
  __IO uint32_t HTR;          /*!< ADC watchdog higher threshold register,      Address offset: 0x28 */
  __IO uint32_t LTR;          /*!< ADC watchdog lower threshold register,       Address offset: 0x2C */
  __IO uint32_t SQR1;         /*!< ADC regular sequence register 1,             Address offset: 0x30 */
  __IO uint32_t SQR2;         /*!< ADC regular sequence register 2,             Address offset: 0x34 */
  __IO uint32_t SQR3;         /*!< ADC regular sequence register 3,             Address offset: 0x38 */
  __IO uint32_t SQR4;         /*!< ADC regular sequence register 4,             Address offset: 0x3C */
  __IO uint32_t SQR5;         /*!< ADC regular sequence register 5,             Address offset: 0x40 */
  __IO uint32_t JSQR;         /*!< ADC injected sequence register,              Address offset: 0x44 */
  __IO uint32_t JDR1;         /*!< ADC injected data register 1,                Address offset: 0x48 */
  __IO uint32_t JDR2;         /*!< ADC injected data register 2,                Address offset: 0x4C */
  __IO uint32_t JDR3;         /*!< ADC injected data register 3,                Address offset: 0x50 */
  __IO uint32_t JDR4;         /*!< ADC injected data register 4,                Address offset: 0x54 */
  __IO uint32_t DR;           /*!< ADC regular data register,                   Address offset: 0x58 */
  __IO uint32_t SMPR0;        /*!< ADC sample time register 0,                  Address offset: 0x5C */
} ADC_TypeDef;

typedef struct
{
  __IO uint32_t CSR;          /*!< ADC common status register,                  Address offset: ADC1 base address + 0x300 */
  __IO uint32_t CCR;          /*!< ADC common control register,                 Address offset: ADC1 base address + 0x304 */
} ADC_Common_TypeDef;

/** 
  * @brief Comparator 
  */

typedef struct
{
  __IO uint32_t CSR;          /*!< COMP comparator control and status register, Address offset: 0x00 */
} COMP_TypeDef;

/** 
  * @brief CRC calculation unit
  */

typedef struct
{
  __IO uint32_t DR;           /*!< CRC Data register,                           Address offset: 0x00 */
  __IO uint8_t  IDR;          /*!< CRC Independent data register,               Address offset: 0x04 */
  uint8_t       RESERVED0;    /*!< Reserved,                                    Address offset: 0x05 */
  uint16_t      RESERVED1;    /*!< Reserved,                                    Address offset: 0x06 */ 
  __IO uint32_t CR;           /*!< CRC Control register,                        Address offset: 0x08 */ 
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
  __IO uint32_t DHR12R2;      /*!< DAC channel2 12-bit right aligned data holding register,  Address offset: 0x14 */
  __IO uint32_t DHR12L2;      /*!< DAC channel2 12-bit left aligned data holding register,   Address offset: 0x18 */
  __IO uint32_t DHR8R2;       /*!< DAC channel2 8-bit right-aligned data holding register,   Address offset: 0x1C */
  __IO uint32_t DHR12RD;      /*!< Dual DAC 12-bit right-aligned data holding register,      Address offset: 0x20 */
  __IO uint32_t DHR12LD;      /*!< DUAL DAC 12-bit left aligned data holding register,       Address offset: 0x24 */
  __IO uint32_t DHR8RD;       /*!< DUAL DAC 8-bit right aligned data holding register,       Address offset: 0x28 */
  __IO uint32_t DOR1;         /*!< DAC channel1 data output register,                        Address offset: 0x2C */
  __IO uint32_t DOR2;         /*!< DAC channel2 data output register,                        Address offset: 0x30 */
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
  __IO uint32_t ACR;          /*!< Access control register,                     Address offset: 0x00 */
  __IO uint32_t PECR;         /*!< Program/erase control register,              Address offset: 0x04 */
  __IO uint32_t PDKEYR;       /*!< Power down key register,                     Address offset: 0x08 */
  __IO uint32_t PEKEYR;       /*!< Program/erase key register,                  Address offset: 0x0c */
  __IO uint32_t PRGKEYR;      /*!< Program memory key register,                 Address offset: 0x10 */
  __IO uint32_t OPTKEYR;      /*!< Option byte key register,                    Address offset: 0x14 */
  __IO uint32_t SR;           /*!< Status register,                             Address offset: 0x18 */
  __IO uint32_t OBR;          /*!< Option byte register,                        Address offset: 0x1c */
  __IO uint32_t WRPR1;        /*!< Write protection register 1,                 Address offset: 0x20 */
  uint32_t   RESERVED[23];    /*!< Reserved,                                    Address offset: 0x24 */
  __IO uint32_t WRPR2;        /*!< Write protection register 2,                 Address offset: 0x80 */
  __IO uint32_t WRPR3;        /*!< Write protection register 3,                 Address offset: 0x84 */
  __IO uint32_t WRPR4;        /*!< Write protection register 4,                 Address offset: 0x88 */
} FLASH_TypeDef;

/** 
  * @brief Option Bytes Registers
  */
typedef struct
{
  __IO uint32_t RDP;              /*!< Read protection register,               Address offset: 0x00 */
  __IO uint32_t USER;             /*!< user register,                          Address offset: 0x04 */
  __IO uint32_t WRP01;            /*!< write protection register 0 1,          Address offset: 0x08 */
  __IO uint32_t WRP23;            /*!< write protection register 2 3,          Address offset: 0x0C */
  __IO uint32_t WRP45;            /*!< write protection register 4 5,          Address offset: 0x10 */
  __IO uint32_t WRP67;            /*!< write protection register 6 7,          Address offset: 0x14 */
  __IO uint32_t WRP89;            /*!< write protection register 8 9,          Address offset: 0x18 */
  __IO uint32_t WRP1011;          /*!< write protection register 10 11,        Address offset: 0x1C */
  uint32_t   RESERVED[24];        /*!< Reserved,                                    0x20 -> 0x7C    */
  __IO uint32_t WRP1213;          /*!< write protection register 12 13,        Address offset: 0x80 */
  __IO uint32_t WRP1415;          /*!< write protection register 14 15,        Address offset: 0x84 */
} OB_TypeDef;

/** 
  * @brief Operational Amplifier (OPAMP)
  */
typedef struct
{
  __IO uint32_t CSR;          /*!< OPAMP control/status register,                     Address offset: 0x00 */
  __IO uint32_t OTR;          /*!< OPAMP offset trimming register for normal mode,    Address offset: 0x04 */ 
  __IO uint32_t LPOTR;        /*!< OPAMP offset trimming register for low power mode, Address offset: 0x08 */
} OPAMP_TypeDef;

/** 
  * @brief General Purpose IO
  */

typedef struct
{
  __IO uint32_t MODER;        /*!< GPIO port mode register,                     Address offset: 0x00      */
  __IO uint32_t OTYPER;       /*!< GPIO port output type register,              Address offset: 0x04      */
  __IO uint32_t OSPEEDR;      /*!< GPIO port output speed register,             Address offset: 0x08      */
  __IO uint32_t PUPDR;        /*!< GPIO port pull-up/pull-down register,        Address offset: 0x0C      */
  __IO uint32_t IDR;          /*!< GPIO port input data register,               Address offset: 0x10      */
  __IO uint32_t ODR;          /*!< GPIO port output data register,              Address offset: 0x14      */
  __IO uint32_t BSRR;         /*!< GPIO port bit set/reset registerBSRR,        Address offset: 0x18      */
  __IO uint32_t LCKR;         /*!< GPIO port configuration lock register,       Address offset: 0x1C      */
  __IO uint32_t AFR[2];       /*!< GPIO alternate function register,            Address offset: 0x20-0x24 */
  __IO uint32_t BRR;          /*!< GPIO bit reset register,                     Address offset: 0x28      */
} GPIO_TypeDef;

/** 
  * @brief SysTem Configuration
  */

typedef struct
{
  __IO uint32_t MEMRMP;       /*!< SYSCFG memory remap register,                      Address offset: 0x00      */
  __IO uint32_t PMC;          /*!< SYSCFG peripheral mode configuration register,     Address offset: 0x04      */
  __IO uint32_t EXTICR[4];    /*!< SYSCFG external interrupt configuration registers, Address offset: 0x08-0x14 */
} SYSCFG_TypeDef;

/** 
  * @brief Inter-integrated Circuit Interface
  */

typedef struct
{
  __IO uint32_t CR1;          /*!< I2C Control register 1,                      Address offset: 0x00 */
  __IO uint32_t CR2;          /*!< I2C Control register 2,                      Address offset: 0x04 */
  __IO uint32_t OAR1;         /*!< I2C Own address register 1,                  Address offset: 0x08 */
  __IO uint32_t OAR2;         /*!< I2C Own address register 2,                  Address offset: 0x0C */
  __IO uint32_t DR;           /*!< I2C Data register,                           Address offset: 0x10 */
  __IO uint32_t SR1;          /*!< I2C Status register 1,                       Address offset: 0x14 */
  __IO uint32_t SR2;          /*!< I2C Status register 2,                       Address offset: 0x18 */
  __IO uint32_t CCR;          /*!< I2C Clock control register,                  Address offset: 0x1C */
  __IO uint32_t TRISE;        /*!< I2C TRISE register,                          Address offset: 0x20 */
} I2C_TypeDef;

/** 
  * @brief Independent WATCHDOG
  */

typedef struct
{
  __IO uint32_t KR;           /*!< Key register,                                Address offset: 0x00 */
  __IO uint32_t PR;           /*!< Prescaler register,                          Address offset: 0x04 */
  __IO uint32_t RLR;          /*!< Reload register,                             Address offset: 0x08 */
  __IO uint32_t SR;           /*!< Status register,                             Address offset: 0x0C */
} IWDG_TypeDef;

/** 
  * @brief LCD
  */

typedef struct
{
  __IO uint32_t CR;        /*!< LCD control register,                           Address offset: 0x00 */
  __IO uint32_t FCR;       /*!< LCD frame control register,                     Address offset: 0x04 */
  __IO uint32_t SR;        /*!< LCD status register,                            Address offset: 0x08 */
  __IO uint32_t CLR;       /*!< LCD clear register,                             Address offset: 0x0C */
  uint32_t RESERVED;       /*!< Reserved,                                       Address offset: 0x10 */
  __IO uint32_t RAM[16];   /*!< LCD display memory,                             Address offset: 0x14-0x50 */
} LCD_TypeDef;

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
  __IO uint32_t ICSCR;         /*!< RCC Internal clock sources calibration register,              Address offset: 0x04 */
  __IO uint32_t CFGR;          /*!< RCC Clock configuration register,                             Address offset: 0x08 */
  __IO uint32_t CIR;           /*!< RCC Clock interrupt register,                                 Address offset: 0x0C */
  __IO uint32_t AHBRSTR;       /*!< RCC AHB peripheral reset register,                            Address offset: 0x10 */
  __IO uint32_t APB2RSTR;      /*!< RCC APB2 peripheral reset register,                           Address offset: 0x14 */
  __IO uint32_t APB1RSTR;      /*!< RCC APB1 peripheral reset register,                           Address offset: 0x18 */
  __IO uint32_t AHBENR;        /*!< RCC AHB peripheral clock enable register,                     Address offset: 0x1C */
  __IO uint32_t APB2ENR;       /*!< RCC APB2 peripheral clock enable register,                    Address offset: 0x20 */
  __IO uint32_t APB1ENR;       /*!< RCC APB1 peripheral clock enable register,                    Address offset: 0x24 */
  __IO uint32_t AHBLPENR;      /*!< RCC AHB peripheral clock enable in low power mode register,   Address offset: 0x28 */
  __IO uint32_t APB2LPENR;     /*!< RCC APB2 peripheral clock enable in low power mode register,  Address offset: 0x2C */
  __IO uint32_t APB1LPENR;     /*!< RCC APB1 peripheral clock enable in low power mode register,  Address offset: 0x30 */
  __IO uint32_t CSR;           /*!< RCC Control/status register,                                  Address offset: 0x34 */
} RCC_TypeDef;

/** 
  * @brief Routing Interface 
  */

typedef struct
{
  __IO uint32_t ICR;        /*!< RI input capture register,                     Address offset: 0x00 */
  __IO uint32_t ASCR1;      /*!< RI analog switches control register,       Address offset: 0x04 */
  __IO uint32_t ASCR2;      /*!< RI analog switch control register 2,        Address offset: 0x08 */
  __IO uint32_t HYSCR1;     /*!< RI hysteresis control register,                Address offset: 0x0C */
  __IO uint32_t HYSCR2;     /*!< RI Hysteresis control register,               Address offset: 0x10 */
  __IO uint32_t HYSCR3;     /*!< RI Hysteresis control register,               Address offset: 0x14 */
  __IO uint32_t HYSCR4;     /*!< RI Hysteresis control register,               Address offset: 0x18 */
  __IO uint32_t ASMR1;      /*!< RI Analog switch mode register 1,         Address offset: 0x1C */
  __IO uint32_t CMR1;       /*!< RI Channel mask register 1,                   Address offset: 0x20 */
  __IO uint32_t CICR1;      /*!< RI Channel Iden for capture register 1,  Address offset: 0x24 */
  __IO uint32_t ASMR2;      /*!< RI Analog switch mode register 2,         Address offset: 0x28 */
  __IO uint32_t CMR2;       /*!< RI Channel mask register 2,                   Address offset: 0x2C */
  __IO uint32_t CICR2;      /*!< RI Channel Iden for capture register 2,  Address offset: 0x30 */
  __IO uint32_t ASMR3;      /*!< RI Analog switch mode register 3,         Address offset: 0x34 */
  __IO uint32_t CMR3;       /*!< RI Channel mask register 3,                   Address offset: 0x38 */
  __IO uint32_t CICR3;      /*!< RI Channel Iden for capture register 3,  Address offset: 0x3C */
  __IO uint32_t ASMR4;      /*!< RI Analog switch mode register 4,         Address offset: 0x40 */
  __IO uint32_t CMR4;       /*!< RI Channel mask register 4,                   Address offset: 0x44 */
  __IO uint32_t CICR4;      /*!< RI Channel Iden for capture register 4,  Address offset: 0x48 */
  __IO uint32_t ASMR5;      /*!< RI Analog switch mode register 5,         Address offset: 0x4C */
  __IO uint32_t CMR5;       /*!< RI Channel mask register 5,                   Address offset: 0x50 */
  __IO uint32_t CICR5;      /*!< RI Channel Iden for capture register 5,  Address offset: 0x54 */
} RI_TypeDef;

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
  __IO uint32_t CALIBR;     /*!< RTC calibration register,                                  Address offset: 0x18 */
  __IO uint32_t ALRMAR;     /*!< RTC alarm A register,                                      Address offset: 0x1C */
  __IO uint32_t ALRMBR;     /*!< RTC alarm B register,                                      Address offset: 0x20 */
  __IO uint32_t WPR;        /*!< RTC write protection register,                             Address offset: 0x24 */
  __IO uint32_t SSR;        /*!< RTC sub second register,                                   Address offset: 0x28 */
  __IO uint32_t SHIFTR;     /*!< RTC shift control register,                                Address offset: 0x2C */
  __IO uint32_t TSTR;       /*!< RTC time stamp time register,                              Address offset: 0x30 */
  __IO uint32_t TSDR;       /*!< RTC time stamp date register,                              Address offset: 0x34 */
  __IO uint32_t TSSSR;      /*!< RTC time-stamp sub second register,                        Address offset: 0x38 */
  __IO uint32_t CALR;       /*!< RRTC calibration register,                                 Address offset: 0x3C */
  __IO uint32_t TAFCR;      /*!< RTC tamper and alternate function configuration register,  Address offset: 0x40 */
  __IO uint32_t ALRMASSR;   /*!< RTC alarm A sub second register,                           Address offset: 0x44 */
  __IO uint32_t ALRMBSSR;   /*!< RTC alarm B sub second register,                           Address offset: 0x48 */
  uint32_t RESERVED7;       /*!< Reserved, 0x4C                                                                  */
  __IO uint32_t BKP0R;      /*!< RTC backup register 0,                                     Address offset: 0x50 */
  __IO uint32_t BKP1R;      /*!< RTC backup register 1,                                     Address offset: 0x54 */
  __IO uint32_t BKP2R;      /*!< RTC backup register 2,                                     Address offset: 0x58 */
  __IO uint32_t BKP3R;      /*!< RTC backup register 3,                                     Address offset: 0x5C */
  __IO uint32_t BKP4R;      /*!< RTC backup register 4,                                     Address offset: 0x60 */
  __IO uint32_t BKP5R;      /*!< RTC backup register 5,                                     Address offset: 0x64 */
  __IO uint32_t BKP6R;      /*!< RTC backup register 6,                                     Address offset: 0x68 */
  __IO uint32_t BKP7R;      /*!< RTC backup register 7,                                     Address offset: 0x6C */
  __IO uint32_t BKP8R;      /*!< RTC backup register 8,                                     Address offset: 0x70 */
  __IO uint32_t BKP9R;      /*!< RTC backup register 9,                                     Address offset: 0x74 */
  __IO uint32_t BKP10R;     /*!< RTC backup register 10,                                    Address offset: 0x78 */
  __IO uint32_t BKP11R;     /*!< RTC backup register 11,                                    Address offset: 0x7C */
  __IO uint32_t BKP12R;     /*!< RTC backup register 12,                                    Address offset: 0x80 */
  __IO uint32_t BKP13R;     /*!< RTC backup register 13,                                    Address offset: 0x84 */
  __IO uint32_t BKP14R;     /*!< RTC backup register 14,                                    Address offset: 0x88 */
  __IO uint32_t BKP15R;     /*!< RTC backup register 15,                                    Address offset: 0x8C */
  __IO uint32_t BKP16R;     /*!< RTC backup register 16,                                    Address offset: 0x90 */
  __IO uint32_t BKP17R;     /*!< RTC backup register 17,                                    Address offset: 0x94 */
  __IO uint32_t BKP18R;     /*!< RTC backup register 18,                                    Address offset: 0x98 */
  __IO uint32_t BKP19R;     /*!< RTC backup register 19,                                    Address offset: 0x9C */
  __IO uint32_t BKP20R;     /*!< RTC backup register 20,                                    Address offset: 0xA0 */
  __IO uint32_t BKP21R;     /*!< RTC backup register 21,                                    Address offset: 0xA4 */
  __IO uint32_t BKP22R;     /*!< RTC backup register 22,                                    Address offset: 0xA8 */
  __IO uint32_t BKP23R;     /*!< RTC backup register 23,                                    Address offset: 0xAC */
  __IO uint32_t BKP24R;     /*!< RTC backup register 24,                                    Address offset: 0xB0 */
  __IO uint32_t BKP25R;     /*!< RTC backup register 25,                                    Address offset: 0xB4 */
  __IO uint32_t BKP26R;     /*!< RTC backup register 26,                                    Address offset: 0xB8 */
  __IO uint32_t BKP27R;     /*!< RTC backup register 27,                                    Address offset: 0xBC */
  __IO uint32_t BKP28R;     /*!< RTC backup register 28,                                    Address offset: 0xC0 */
  __IO uint32_t BKP29R;     /*!< RTC backup register 29,                                    Address offset: 0xC4 */
  __IO uint32_t BKP30R;     /*!< RTC backup register 30,                                    Address offset: 0xC8 */
  __IO uint32_t BKP31R;     /*!< RTC backup register 31,                                    Address offset: 0xCC */
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
  uint32_t      RESERVED12;   /*!< Reserved, 0x30                                            */    
  __IO uint32_t CCR1;         /*!< TIM capture/compare register 1,      Address offset: 0x34 */    
  __IO uint32_t CCR2;         /*!< TIM capture/compare register 2,      Address offset: 0x38 */    
  __IO uint32_t CCR3;         /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  __IO uint32_t CCR4;         /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  uint32_t      RESERVED17;   /*!< Reserved, 0x44                                            */ 
  __IO uint32_t DCR;          /*!< TIM DMA control register,            Address offset: 0x48 */
  __IO uint32_t DMAR;         /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  __IO uint32_t OR;           /*!< TIM option register,                 Address offset: 0x50 */
} TIM_TypeDef;
/** 
  * @brief Universal Synchronous Asynchronous Receiver Transmitter
  */
 
typedef struct
{
  __IO uint32_t SR;         /*!< USART Status register,                   Address offset: 0x00 */
  __IO uint32_t DR;         /*!< USART Data register,                     Address offset: 0x04 */
  __IO uint32_t BRR;        /*!< USART Baud rate register,                Address offset: 0x08 */
  __IO uint32_t CR1;        /*!< USART Control register 1,                Address offset: 0x0C */
  __IO uint32_t CR2;        /*!< USART Control register 2,                Address offset: 0x10 */
  __IO uint32_t CR3;        /*!< USART Control register 3,                Address offset: 0x14 */
  __IO uint32_t GTPR;       /*!< USART Guard time and prescaler register, Address offset: 0x18 */
} USART_TypeDef;

/** 
  * @brief Universal Serial Bus Full Speed Device
  */
  
typedef struct
{
  __IO uint16_t EP0R;            /*!< USB Endpoint 0 register,                Address offset: 0x00 */ 
  __IO uint16_t RESERVED0;       /*!< Reserved */     
  __IO uint16_t EP1R;            /*!< USB Endpoint 1 register,                Address offset: 0x04 */
  __IO uint16_t RESERVED1;       /*!< Reserved */       
  __IO uint16_t EP2R;            /*!< USB Endpoint 2 register,                Address offset: 0x08 */
  __IO uint16_t RESERVED2;       /*!< Reserved */       
  __IO uint16_t EP3R;            /*!< USB Endpoint 3 register,                Address offset: 0x0C */ 
  __IO uint16_t RESERVED3;       /*!< Reserved */       
  __IO uint16_t EP4R;            /*!< USB Endpoint 4 register,                Address offset: 0x10 */
  __IO uint16_t RESERVED4;       /*!< Reserved */       
  __IO uint16_t EP5R;            /*!< USB Endpoint 5 register,                Address offset: 0x14 */
  __IO uint16_t RESERVED5;       /*!< Reserved */       
  __IO uint16_t EP6R;            /*!< USB Endpoint 6 register,                Address offset: 0x18 */
  __IO uint16_t RESERVED6;       /*!< Reserved */       
  __IO uint16_t EP7R;            /*!< USB Endpoint 7 register,                Address offset: 0x1C */
  __IO uint16_t RESERVED7[17];   /*!< Reserved */     
  __IO uint16_t CNTR;            /*!< Control register,                       Address offset: 0x40 */
  __IO uint16_t RESERVED8;       /*!< Reserved */       
  __IO uint16_t ISTR;            /*!< Interrupt status register,              Address offset: 0x44 */
  __IO uint16_t RESERVED9;       /*!< Reserved */       
  __IO uint16_t FNR;             /*!< Frame number register,                  Address offset: 0x48 */
  __IO uint16_t RESERVEDA;       /*!< Reserved */       
  __IO uint16_t DADDR;           /*!< Device address register,                Address offset: 0x4C */
  __IO uint16_t RESERVEDB;       /*!< Reserved */       
  __IO uint16_t BTABLE;          /*!< Buffer Table address register,          Address offset: 0x50 */
  __IO uint16_t RESERVEDC;       /*!< Reserved */       
} USB_TypeDef;

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
  * @brief Universal Serial Bus Full Speed Device
  */
/** 
  * @}
  */
  
/** @addtogroup Peripheral_memory_map
  * @{
  */

#define FLASH_BASE            ((uint32_t)0x08000000U)              /*!< FLASH base address in the alias region */
#define FLASH_EEPROM_BASE     ((uint32_t)(FLASH_BASE + 0x80000U))  /*!< FLASH EEPROM base address in the alias region */
#define SRAM_BASE             ((uint32_t)0x20000000U)              /*!< SRAM base address in the alias region */
#define PERIPH_BASE           ((uint32_t)0x40000000U)              /*!< Peripheral base address in the alias region */
#define SRAM_BB_BASE          ((uint32_t)0x22000000U)              /*!< SRAM base address in the bit-band region */
#define PERIPH_BB_BASE        ((uint32_t)0x42000000U)              /*!< Peripheral base address in the bit-band region */
#define FLASH_BANK2_BASE      ((uint32_t)0x08040000U)              /*!< FLASH BANK2 base address in the alias region */
#define FLASH_BANK1_END       ((uint32_t)0x0803FFFFU)              /*!< Program end FLASH BANK1 address  */
#define FLASH_BANK2_END       ((uint32_t)0x0807FFFFU)              /*!< Program end FLASH BANK2 address  */
#define FLASH_EEPROM_END      ((uint32_t)0x08083FFFU)              /*!< FLASH EEPROM end address (16KB) */

/*!< Peripheral memory map */
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000U)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x00020000U)

/*!< APB1 peripherals */
#define TIM2_BASE             (APB1PERIPH_BASE + 0x00000000U)
#define TIM3_BASE             (APB1PERIPH_BASE + 0x00000400U)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x00000800U)
#define TIM5_BASE             (APB1PERIPH_BASE + 0x00000C00U)
#define TIM6_BASE             (APB1PERIPH_BASE + 0x00001000U)
#define TIM7_BASE             (APB1PERIPH_BASE + 0x00001400U)
#define LCD_BASE              (APB1PERIPH_BASE + 0x00002400U)
#define RTC_BASE              (APB1PERIPH_BASE + 0x00002800U)
#define WWDG_BASE             (APB1PERIPH_BASE + 0x00002C00U)
#define IWDG_BASE             (APB1PERIPH_BASE + 0x00003000U)
#define SPI2_BASE             (APB1PERIPH_BASE + 0x00003800U)
#define SPI3_BASE             (APB1PERIPH_BASE + 0x00003C00U)
#define USART2_BASE           (APB1PERIPH_BASE + 0x00004400U)
#define USART3_BASE           (APB1PERIPH_BASE + 0x00004800U)
#define UART4_BASE            (APB1PERIPH_BASE + 0x00004C00U)
#define UART5_BASE            (APB1PERIPH_BASE + 0x00005000U)
#define I2C1_BASE             (APB1PERIPH_BASE + 0x00005400U)
#define I2C2_BASE             (APB1PERIPH_BASE + 0x00005800U)

/* USB device FS */
#define USB_BASE              (APB1PERIPH_BASE + 0x00005C00U) /*!< USB_IP Peripheral Registers base address */
#define USB_PMAADDR           (APB1PERIPH_BASE + 0x00006000U) /*!< USB_IP Packet Memory Area base address */

/* USB device FS SRAM */
#define PWR_BASE              (APB1PERIPH_BASE + 0x00007000U)
#define DAC_BASE              (APB1PERIPH_BASE + 0x00007400U)
#define COMP_BASE             (APB1PERIPH_BASE + 0x00007C00U)
#define RI_BASE               (APB1PERIPH_BASE + 0x00007C04U)
#define OPAMP_BASE            (APB1PERIPH_BASE + 0x00007C5CU)

/*!< APB2 peripherals */
#define SYSCFG_BASE           (APB2PERIPH_BASE + 0x00000000U)
#define EXTI_BASE             (APB2PERIPH_BASE + 0x00000400U)
#define TIM9_BASE             (APB2PERIPH_BASE + 0x00000800U)
#define TIM10_BASE            (APB2PERIPH_BASE + 0x00000C00U)
#define TIM11_BASE            (APB2PERIPH_BASE + 0x00001000U)
#define ADC1_BASE             (APB2PERIPH_BASE + 0x00002400U)
#define ADC_BASE              (APB2PERIPH_BASE + 0x00002700U)
#define SPI1_BASE             (APB2PERIPH_BASE + 0x00003000U)
#define USART1_BASE           (APB2PERIPH_BASE + 0x00003800U)

/*!< AHB peripherals */
#define GPIOA_BASE            (AHBPERIPH_BASE + 0x00000000U)
#define GPIOB_BASE            (AHBPERIPH_BASE + 0x00000400U)
#define GPIOC_BASE            (AHBPERIPH_BASE + 0x00000800U)
#define GPIOD_BASE            (AHBPERIPH_BASE + 0x00000C00U)
#define GPIOE_BASE            (AHBPERIPH_BASE + 0x00001000U)
#define GPIOH_BASE            (AHBPERIPH_BASE + 0x00001400U)
#define GPIOF_BASE            (AHBPERIPH_BASE + 0x00001800U)
#define GPIOG_BASE            (AHBPERIPH_BASE + 0x00001C00U)
#define CRC_BASE              (AHBPERIPH_BASE + 0x00003000U)
#define RCC_BASE              (AHBPERIPH_BASE + 0x00003800U)
#define FLASH_R_BASE          (AHBPERIPH_BASE + 0x00003C00U) /*!< FLASH registers base address */
#define OB_BASE               ((uint32_t)0x1FF80000U)        /*!< FLASH Option Bytes base address */
#define FLASHSIZE_BASE        ((uint32_t)0x1FF800CCU)        /*!< FLASH Size register base address for Cat.3, Cat.4, Cat.5 and Cat.6 devices */
#define UID_BASE              ((uint32_t)0x1FF800D0U)        /*!< Unique device ID register base address for Cat.3, Cat.4, Cat.5 and Cat.6 devices */
#define DMA1_BASE             (AHBPERIPH_BASE + 0x00006000U)
#define DMA1_Channel1_BASE    (DMA1_BASE + 0x00000008U)
#define DMA1_Channel2_BASE    (DMA1_BASE + 0x0000001CU)
#define DMA1_Channel3_BASE    (DMA1_BASE + 0x00000030U)
#define DMA1_Channel4_BASE    (DMA1_BASE + 0x00000044U)
#define DMA1_Channel5_BASE    (DMA1_BASE + 0x00000058U)
#define DMA1_Channel6_BASE    (DMA1_BASE + 0x0000006CU)
#define DMA1_Channel7_BASE    (DMA1_BASE + 0x00000080U)
#define DMA2_BASE             (AHBPERIPH_BASE + 0x00006400U)
#define DMA2_Channel1_BASE    (DMA2_BASE + 0x00000008U)
#define DMA2_Channel2_BASE    (DMA2_BASE + 0x0000001CU)
#define DMA2_Channel3_BASE    (DMA2_BASE + 0x00000030U)
#define DMA2_Channel4_BASE    (DMA2_BASE + 0x00000044U)
#define DMA2_Channel5_BASE    (DMA2_BASE + 0x00000058U)
#define DBGMCU_BASE           ((uint32_t)0xE0042000U)     /*!< Debug MCU registers base address */

/**
  * @}
  */
  
/** @addtogroup Peripheral_declaration
  * @{
  */  

#define TIM2                ((TIM_TypeDef *) TIM2_BASE)
#define TIM3                ((TIM_TypeDef *) TIM3_BASE)
#define TIM4                ((TIM_TypeDef *) TIM4_BASE)
#define TIM5                ((TIM_TypeDef *) TIM5_BASE)
#define TIM6                ((TIM_TypeDef *) TIM6_BASE)
#define TIM7                ((TIM_TypeDef *) TIM7_BASE)
#define LCD                 ((LCD_TypeDef *) LCD_BASE)
#define RTC                 ((RTC_TypeDef *) RTC_BASE)
#define WWDG                ((WWDG_TypeDef *) WWDG_BASE)
#define IWDG                ((IWDG_TypeDef *) IWDG_BASE)
#define SPI2                ((SPI_TypeDef *) SPI2_BASE)
#define SPI3                ((SPI_TypeDef *) SPI3_BASE)
#define USART2              ((USART_TypeDef *) USART2_BASE)
#define USART3              ((USART_TypeDef *) USART3_BASE)
#define UART4               ((USART_TypeDef *) UART4_BASE)
#define UART5               ((USART_TypeDef *) UART5_BASE)
#define I2C1                ((I2C_TypeDef *) I2C1_BASE)
#define I2C2                ((I2C_TypeDef *) I2C2_BASE)
/* USB device FS */
#define USB                   ((USB_TypeDef *) USB_BASE)
/* USB device FS SRAM */
#define PWR                 ((PWR_TypeDef *) PWR_BASE)

#define DAC1                ((DAC_TypeDef *) DAC_BASE)
/* Legacy defines */
#define DAC                 DAC1

#define COMP                ((COMP_TypeDef *) COMP_BASE)
#define COMP1               ((COMP_TypeDef *) COMP_BASE)
#define COMP2               ((COMP_TypeDef *) (COMP_BASE + 0x00000001U))
#define RI                  ((RI_TypeDef *) RI_BASE)
#define OPAMP               ((OPAMP_TypeDef *) OPAMP_BASE)
#define OPAMP1              ((OPAMP_TypeDef *) OPAMP_BASE)
#define OPAMP2              ((OPAMP_TypeDef *) (OPAMP_BASE + 0x00000001U))
#define SYSCFG              ((SYSCFG_TypeDef *) SYSCFG_BASE)
#define EXTI                ((EXTI_TypeDef *) EXTI_BASE)
#define TIM9                ((TIM_TypeDef *) TIM9_BASE)
#define TIM10               ((TIM_TypeDef *) TIM10_BASE)
#define TIM11               ((TIM_TypeDef *) TIM11_BASE)

#define ADC1                ((ADC_TypeDef *) ADC1_BASE)
#define ADC1_COMMON         ((ADC_Common_TypeDef *) ADC_BASE)
/* Legacy defines */
#define ADC                 ADC1_COMMON

#define SPI1                ((SPI_TypeDef *) SPI1_BASE)
#define USART1              ((USART_TypeDef *) USART1_BASE)
#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD               ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE               ((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOH               ((GPIO_TypeDef *) GPIOH_BASE)
#define GPIOF               ((GPIO_TypeDef *) GPIOF_BASE)
#define GPIOG               ((GPIO_TypeDef *) GPIOG_BASE)
#define CRC                 ((CRC_TypeDef *) CRC_BASE)
#define RCC                 ((RCC_TypeDef *) RCC_BASE)
#define FLASH               ((FLASH_TypeDef *) FLASH_R_BASE)
#define OB                  ((OB_TypeDef *) OB_BASE) 
#define DMA1                ((DMA_TypeDef *) DMA1_BASE)
#define DMA1_Channel1       ((DMA_Channel_TypeDef *) DMA1_Channel1_BASE)
#define DMA1_Channel2       ((DMA_Channel_TypeDef *) DMA1_Channel2_BASE)
#define DMA1_Channel3       ((DMA_Channel_TypeDef *) DMA1_Channel3_BASE)
#define DMA1_Channel4       ((DMA_Channel_TypeDef *) DMA1_Channel4_BASE)
#define DMA1_Channel5       ((DMA_Channel_TypeDef *) DMA1_Channel5_BASE)
#define DMA1_Channel6       ((DMA_Channel_TypeDef *) DMA1_Channel6_BASE)
#define DMA1_Channel7       ((DMA_Channel_TypeDef *) DMA1_Channel7_BASE)
#define DMA2                ((DMA_TypeDef *) DMA2_BASE)
#define DMA2_Channel1       ((DMA_Channel_TypeDef *) DMA2_Channel1_BASE)
#define DMA2_Channel2       ((DMA_Channel_TypeDef *) DMA2_Channel2_BASE)
#define DMA2_Channel3       ((DMA_Channel_TypeDef *) DMA2_Channel3_BASE)
#define DMA2_Channel4       ((DMA_Channel_TypeDef *) DMA2_Channel4_BASE)
#define DMA2_Channel5       ((DMA_Channel_TypeDef *) DMA2_Channel5_BASE)
#define DBGMCU              ((DBGMCU_TypeDef *) DBGMCU_BASE)

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

/********************  Bit definition for ADC_SR register  ********************/
#define  ADC_SR_AWD                          ((uint32_t)0x00000001U)        /*!< ADC analog watchdog 1 flag */
#define  ADC_SR_EOCS                         ((uint32_t)0x00000002U)        /*!< ADC group regular end of unitary conversion or end of sequence conversions flag */
#define  ADC_SR_JEOS                         ((uint32_t)0x00000004U)        /*!< ADC group injected end of sequence conversions flag */
#define  ADC_SR_JSTRT                        ((uint32_t)0x00000008U)        /*!< ADC group injected conversion start flag */
#define  ADC_SR_STRT                         ((uint32_t)0x00000010U)        /*!< ADC group regular conversion start flag */
#define  ADC_SR_OVR                          ((uint32_t)0x00000020U)        /*!< ADC group regular overrun flag */
#define  ADC_SR_ADONS                        ((uint32_t)0x00000040U)        /*!< ADC ready flag */
#define  ADC_SR_RCNR                         ((uint32_t)0x00000100U)        /*!< ADC group regular not ready flag */
#define  ADC_SR_JCNR                         ((uint32_t)0x00000200U)        /*!< ADC group injected not ready flag */

/* Legacy defines */
#define  ADC_SR_EOC                          (ADC_SR_EOCS)
#define  ADC_SR_JEOC                         (ADC_SR_JEOS)

/*******************  Bit definition for ADC_CR1 register  ********************/
#define  ADC_CR1_AWDCH                       ((uint32_t)0x0000001FU)        /*!< ADC analog watchdog 1 monitored channel selection */
#define  ADC_CR1_AWDCH_0                     ((uint32_t)0x00000001U)        /*!< Bit 0 */
#define  ADC_CR1_AWDCH_1                     ((uint32_t)0x00000002U)        /*!< Bit 1 */
#define  ADC_CR1_AWDCH_2                     ((uint32_t)0x00000004U)        /*!< Bit 2 */
#define  ADC_CR1_AWDCH_3                     ((uint32_t)0x00000008U)        /*!< Bit 3 */
#define  ADC_CR1_AWDCH_4                     ((uint32_t)0x00000010U)        /*!< Bit 4 */

#define  ADC_CR1_EOCSIE                      ((uint32_t)0x00000020U)        /*!< ADC group regular end of unitary conversion or end of sequence conversions interrupt */
#define  ADC_CR1_AWDIE                       ((uint32_t)0x00000040U)        /*!< ADC analog watchdog 1 interrupt */
#define  ADC_CR1_JEOSIE                      ((uint32_t)0x00000080U)        /*!< ADC group injected end of sequence conversions interrupt */
#define  ADC_CR1_SCAN                        ((uint32_t)0x00000100U)        /*!< ADC scan mode */
#define  ADC_CR1_AWDSGL                      ((uint32_t)0x00000200U)        /*!< ADC analog watchdog 1 monitoring a single channel or all channels */
#define  ADC_CR1_JAUTO                       ((uint32_t)0x00000400U)        /*!< ADC group injected automatic trigger mode */
#define  ADC_CR1_DISCEN                      ((uint32_t)0x00000800U)        /*!< ADC group regular sequencer discontinuous mode */
#define  ADC_CR1_JDISCEN                     ((uint32_t)0x00001000U)        /*!< ADC group injected sequencer discontinuous mode */

#define  ADC_CR1_DISCNUM                     ((uint32_t)0x0000E000U)        /*!< ADC group regular sequencer discontinuous number of ranks */
#define  ADC_CR1_DISCNUM_0                   ((uint32_t)0x00002000U)        /*!< Bit 0 */
#define  ADC_CR1_DISCNUM_1                   ((uint32_t)0x00004000U)        /*!< Bit 1 */
#define  ADC_CR1_DISCNUM_2                   ((uint32_t)0x00008000U)        /*!< Bit 2 */

#define  ADC_CR1_PDD                         ((uint32_t)0x00010000U)        /*!< ADC power down during auto delay phase */
#define  ADC_CR1_PDI                         ((uint32_t)0x00020000U)        /*!< ADC power down during idle phase */

#define  ADC_CR1_JAWDEN                      ((uint32_t)0x00400000U)        /*!< ADC analog watchdog 1 enable on scope ADC group injected */
#define  ADC_CR1_AWDEN                       ((uint32_t)0x00800000U)        /*!< ADC analog watchdog 1 enable on scope ADC group regular */

#define  ADC_CR1_RES                         ((uint32_t)0x03000000U)        /*!< ADC resolution */
#define  ADC_CR1_RES_0                       ((uint32_t)0x01000000U)        /*!< Bit 0 */
#define  ADC_CR1_RES_1                       ((uint32_t)0x02000000U)        /*!< Bit 1 */

#define  ADC_CR1_OVRIE                       ((uint32_t)0x04000000U)        /*!< ADC group regular overrun interrupt */

/* Legacy defines */
#define  ADC_CR1_EOCIE                       (ADC_CR1_EOCSIE)
#define  ADC_CR1_JEOCIE                      (ADC_CR1_JEOSIE)

/*******************  Bit definition for ADC_CR2 register  ********************/
#define  ADC_CR2_ADON                        ((uint32_t)0x00000001U)        /*!< ADC enable */
#define  ADC_CR2_CONT                        ((uint32_t)0x00000002U)        /*!< ADC group regular continuous conversion mode */
#define  ADC_CR2_CFG                         ((uint32_t)0x00000004U)        /*!< ADC channels bank selection */

#define  ADC_CR2_DELS                        ((uint32_t)0x00000070U)        /*!< ADC auto delay selection */
#define  ADC_CR2_DELS_0                      ((uint32_t)0x00000010U)        /*!< Bit 0 */
#define  ADC_CR2_DELS_1                      ((uint32_t)0x00000020U)        /*!< Bit 1 */
#define  ADC_CR2_DELS_2                      ((uint32_t)0x00000040U)        /*!< Bit 2 */

#define  ADC_CR2_DMA                         ((uint32_t)0x00000100U)        /*!< ADC DMA transfer enable */
#define  ADC_CR2_DDS                         ((uint32_t)0x00000200U)        /*!< ADC DMA transfer configuration */
#define  ADC_CR2_EOCS                        ((uint32_t)0x00000400U)        /*!< ADC end of unitary or end of sequence conversions selection */
#define  ADC_CR2_ALIGN                       ((uint32_t)0x00000800U)        /*!< ADC data alignement */

#define  ADC_CR2_JEXTSEL                     ((uint32_t)0x000F0000U)        /*!< ADC group injected external trigger source */
#define  ADC_CR2_JEXTSEL_0                   ((uint32_t)0x00010000U)        /*!< Bit 0 */
#define  ADC_CR2_JEXTSEL_1                   ((uint32_t)0x00020000U)        /*!< Bit 1 */
#define  ADC_CR2_JEXTSEL_2                   ((uint32_t)0x00040000U)        /*!< Bit 2 */
#define  ADC_CR2_JEXTSEL_3                   ((uint32_t)0x00080000U)        /*!< Bit 3 */

#define  ADC_CR2_JEXTEN                      ((uint32_t)0x00300000U)        /*!< ADC group injected external trigger polarity */
#define  ADC_CR2_JEXTEN_0                    ((uint32_t)0x00100000U)        /*!< Bit 0 */
#define  ADC_CR2_JEXTEN_1                    ((uint32_t)0x00200000U)        /*!< Bit 1 */

#define  ADC_CR2_JSWSTART                    ((uint32_t)0x00400000U)        /*!< ADC group injected conversion start */

#define  ADC_CR2_EXTSEL                      ((uint32_t)0x0F000000U)        /*!< ADC group regular external trigger source */
#define  ADC_CR2_EXTSEL_0                    ((uint32_t)0x01000000U)        /*!< Bit 0 */
#define  ADC_CR2_EXTSEL_1                    ((uint32_t)0x02000000U)        /*!< Bit 1 */
#define  ADC_CR2_EXTSEL_2                    ((uint32_t)0x04000000U)        /*!< Bit 2 */
#define  ADC_CR2_EXTSEL_3                    ((uint32_t)0x08000000U)        /*!< Bit 3 */

#define  ADC_CR2_EXTEN                       ((uint32_t)0x30000000U)        /*!< ADC group regular external trigger polarity */
#define  ADC_CR2_EXTEN_0                     ((uint32_t)0x10000000U)        /*!< Bit 0 */
#define  ADC_CR2_EXTEN_1                     ((uint32_t)0x20000000U)        /*!< Bit 1 */

#define  ADC_CR2_SWSTART                     ((uint32_t)0x40000000U)        /*!< ADC group regular conversion start */

/******************  Bit definition for ADC_SMPR1 register  *******************/
#define  ADC_SMPR1_SMP20                     ((uint32_t)0x00000007U)        /*!< ADC channel 20 sampling time selection */
#define  ADC_SMPR1_SMP20_0                   ((uint32_t)0x00000001U)        /*!< Bit 0 */
#define  ADC_SMPR1_SMP20_1                   ((uint32_t)0x00000002U)        /*!< Bit 1 */
#define  ADC_SMPR1_SMP20_2                   ((uint32_t)0x00000004U)        /*!< Bit 2 */

#define  ADC_SMPR1_SMP21                     ((uint32_t)0x00000038U)        /*!< ADC channel 21 sampling time selection */
#define  ADC_SMPR1_SMP21_0                   ((uint32_t)0x00000008U)        /*!< Bit 0 */
#define  ADC_SMPR1_SMP21_1                   ((uint32_t)0x00000010U)        /*!< Bit 1 */
#define  ADC_SMPR1_SMP21_2                   ((uint32_t)0x00000020U)        /*!< Bit 2 */

#define  ADC_SMPR1_SMP22                     ((uint32_t)0x000001C0U)        /*!< ADC channel 22 sampling time selection */
#define  ADC_SMPR1_SMP22_0                   ((uint32_t)0x00000040U)        /*!< Bit 0 */
#define  ADC_SMPR1_SMP22_1                   ((uint32_t)0x00000080U)        /*!< Bit 1 */
#define  ADC_SMPR1_SMP22_2                   ((uint32_t)0x00000100U)        /*!< Bit 2 */

#define  ADC_SMPR1_SMP23                     ((uint32_t)0x00000E00U)        /*!< ADC channel 23 sampling time selection */
#define  ADC_SMPR1_SMP23_0                   ((uint32_t)0x00000200U)        /*!< Bit 0 */
#define  ADC_SMPR1_SMP23_1                   ((uint32_t)0x00000400U)        /*!< Bit 1 */
#define  ADC_SMPR1_SMP23_2                   ((uint32_t)0x00000800U)        /*!< Bit 2 */

#define  ADC_SMPR1_SMP24                     ((uint32_t)0x00007000U)        /*!< ADC channel 24 sampling time selection */
#define  ADC_SMPR1_SMP24_0                   ((uint32_t)0x00001000U)        /*!< Bit 0 */
#define  ADC_SMPR1_SMP24_1                   ((uint32_t)0x00002000U)        /*!< Bit 1 */
#define  ADC_SMPR1_SMP24_2                   ((uint32_t)0x00004000U)        /*!< Bit 2 */

#define  ADC_SMPR1_SMP25                     ((uint32_t)0x00038000U)        /*!< ADC channel 25 sampling time selection */
#define  ADC_SMPR1_SMP25_0                   ((uint32_t)0x00008000U)        /*!< Bit 0 */
#define  ADC_SMPR1_SMP25_1                   ((uint32_t)0x00010000U)        /*!< Bit 1 */
#define  ADC_SMPR1_SMP25_2                   ((uint32_t)0x00020000U)        /*!< Bit 2 */

#define  ADC_SMPR1_SMP26                     ((uint32_t)0x001C0000U)        /*!< ADC channel 26 sampling time selection */
#define  ADC_SMPR1_SMP26_0                   ((uint32_t)0x00040000U)        /*!< Bit 0 */
#define  ADC_SMPR1_SMP26_1                   ((uint32_t)0x00080000U)        /*!< Bit 1 */
#define  ADC_SMPR1_SMP26_2                   ((uint32_t)0x00100000U)        /*!< Bit 2 */

#define  ADC_SMPR1_SMP27                     ((uint32_t)0x00E00000U)        /*!< ADC channel 27 sampling time selection */
#define  ADC_SMPR1_SMP27_0                   ((uint32_t)0x00200000U)        /*!< Bit 0 */
#define  ADC_SMPR1_SMP27_1                   ((uint32_t)0x00400000U)        /*!< Bit 1 */
#define  ADC_SMPR1_SMP27_2                   ((uint32_t)0x00800000U)        /*!< Bit 2 */

#define  ADC_SMPR1_SMP28                     ((uint32_t)0x07000000U)        /*!< ADC channel 28 sampling time selection */
#define  ADC_SMPR1_SMP28_0                   ((uint32_t)0x01000000U)        /*!< Bit 0 */
#define  ADC_SMPR1_SMP28_1                   ((uint32_t)0x02000000U)        /*!< Bit 1 */
#define  ADC_SMPR1_SMP28_2                   ((uint32_t)0x04000000U)        /*!< Bit 2 */

#define  ADC_SMPR1_SMP29                     ((uint32_t)0x38000000U)        /*!< ADC channel 29 sampling time selection */
#define  ADC_SMPR1_SMP29_0                   ((uint32_t)0x08000000U)        /*!< Bit 0 */
#define  ADC_SMPR1_SMP29_1                   ((uint32_t)0x10000000U)        /*!< Bit 1 */
#define  ADC_SMPR1_SMP29_2                   ((uint32_t)0x20000000U)        /*!< Bit 2 */

/******************  Bit definition for ADC_SMPR2 register  *******************/
#define  ADC_SMPR2_SMP10                     ((uint32_t)0x00000007U)        /*!< ADC channel 10 sampling time selection */
#define  ADC_SMPR2_SMP10_0                   ((uint32_t)0x00000001U)        /*!< Bit 0 */
#define  ADC_SMPR2_SMP10_1                   ((uint32_t)0x00000002U)        /*!< Bit 1 */
#define  ADC_SMPR2_SMP10_2                   ((uint32_t)0x00000004U)        /*!< Bit 2 */

#define  ADC_SMPR2_SMP11                     ((uint32_t)0x00000038U)        /*!< ADC channel 11 sampling time selection */
#define  ADC_SMPR2_SMP11_0                   ((uint32_t)0x00000008U)        /*!< Bit 0 */
#define  ADC_SMPR2_SMP11_1                   ((uint32_t)0x00000010U)        /*!< Bit 1 */
#define  ADC_SMPR2_SMP11_2                   ((uint32_t)0x00000020U)        /*!< Bit 2 */

#define  ADC_SMPR2_SMP12                     ((uint32_t)0x000001C0U)        /*!< ADC channel 12 sampling time selection */
#define  ADC_SMPR2_SMP12_0                   ((uint32_t)0x00000040U)        /*!< Bit 0 */
#define  ADC_SMPR2_SMP12_1                   ((uint32_t)0x00000080U)        /*!< Bit 1 */
#define  ADC_SMPR2_SMP12_2                   ((uint32_t)0x00000100U)        /*!< Bit 2 */

#define  ADC_SMPR2_SMP13                     ((uint32_t)0x00000E00U)        /*!< ADC channel 13 sampling time selection */
#define  ADC_SMPR2_SMP13_0                   ((uint32_t)0x00000200U)        /*!< Bit 0 */
#define  ADC_SMPR2_SMP13_1                   ((uint32_t)0x00000400U)        /*!< Bit 1 */
#define  ADC_SMPR2_SMP13_2                   ((uint32_t)0x00000800U)        /*!< Bit 2 */

#define  ADC_SMPR2_SMP14                     ((uint32_t)0x00007000U)        /*!< ADC channel 14 sampling time selection */
#define  ADC_SMPR2_SMP14_0                   ((uint32_t)0x00001000U)        /*!< Bit 0 */
#define  ADC_SMPR2_SMP14_1                   ((uint32_t)0x00002000U)        /*!< Bit 1 */
#define  ADC_SMPR2_SMP14_2                   ((uint32_t)0x00004000U)        /*!< Bit 2 */

#define  ADC_SMPR2_SMP15                     ((uint32_t)0x00038000U)        /*!< ADC channel 5 sampling time selection */
#define  ADC_SMPR2_SMP15_0                   ((uint32_t)0x00008000U)        /*!< Bit 0 */
#define  ADC_SMPR2_SMP15_1                   ((uint32_t)0x00010000U)        /*!< Bit 1 */
#define  ADC_SMPR2_SMP15_2                   ((uint32_t)0x00020000U)        /*!< Bit 2 */

#define  ADC_SMPR2_SMP16                     ((uint32_t)0x001C0000U)        /*!< ADC channel 16 sampling time selection */
#define  ADC_SMPR2_SMP16_0                   ((uint32_t)0x00040000U)        /*!< Bit 0 */
#define  ADC_SMPR2_SMP16_1                   ((uint32_t)0x00080000U)        /*!< Bit 1 */
#define  ADC_SMPR2_SMP16_2                   ((uint32_t)0x00100000U)        /*!< Bit 2 */

#define  ADC_SMPR2_SMP17                     ((uint32_t)0x00E00000U)        /*!< ADC channel 17 sampling time selection */
#define  ADC_SMPR2_SMP17_0                   ((uint32_t)0x00200000U)        /*!< Bit 0 */
#define  ADC_SMPR2_SMP17_1                   ((uint32_t)0x00400000U)        /*!< Bit 1 */
#define  ADC_SMPR2_SMP17_2                   ((uint32_t)0x00800000U)        /*!< Bit 2 */

#define  ADC_SMPR2_SMP18                     ((uint32_t)0x07000000U)        /*!< ADC channel 18 sampling time selection */
#define  ADC_SMPR2_SMP18_0                   ((uint32_t)0x01000000U)        /*!< Bit 0 */
#define  ADC_SMPR2_SMP18_1                   ((uint32_t)0x02000000U)        /*!< Bit 1 */
#define  ADC_SMPR2_SMP18_2                   ((uint32_t)0x04000000U)        /*!< Bit 2 */

#define  ADC_SMPR2_SMP19                     ((uint32_t)0x38000000U)        /*!< ADC channel 19 sampling time selection */
#define  ADC_SMPR2_SMP19_0                   ((uint32_t)0x08000000U)        /*!< Bit 0 */
#define  ADC_SMPR2_SMP19_1                   ((uint32_t)0x10000000U)        /*!< Bit 1 */
#define  ADC_SMPR2_SMP19_2                   ((uint32_t)0x20000000U)        /*!< Bit 2 */

/******************  Bit definition for ADC_SMPR3 register  *******************/
#define  ADC_SMPR3_SMP0                      ((uint32_t)0x00000007U)        /*!< ADC channel 0 sampling time selection */
#define  ADC_SMPR3_SMP0_0                    ((uint32_t)0x00000001U)        /*!< Bit 0 */
#define  ADC_SMPR3_SMP0_1                    ((uint32_t)0x00000002U)        /*!< Bit 1 */
#define  ADC_SMPR3_SMP0_2                    ((uint32_t)0x00000004U)        /*!< Bit 2 */
 
#define  ADC_SMPR3_SMP1                      ((uint32_t)0x00000038U)        /*!< ADC channel 1 sampling time selection */
#define  ADC_SMPR3_SMP1_0                    ((uint32_t)0x00000008U)        /*!< Bit 0 */
#define  ADC_SMPR3_SMP1_1                    ((uint32_t)0x00000010U)        /*!< Bit 1 */
#define  ADC_SMPR3_SMP1_2                    ((uint32_t)0x00000020U)        /*!< Bit 2 */

#define  ADC_SMPR3_SMP2                      ((uint32_t)0x000001C0U)        /*!< ADC channel 2 sampling time selection */
#define  ADC_SMPR3_SMP2_0                    ((uint32_t)0x00000040U)        /*!< Bit 0 */
#define  ADC_SMPR3_SMP2_1                    ((uint32_t)0x00000080U)        /*!< Bit 1 */
#define  ADC_SMPR3_SMP2_2                    ((uint32_t)0x00000100U)        /*!< Bit 2 */

#define  ADC_SMPR3_SMP3                      ((uint32_t)0x00000E00U)        /*!< ADC channel 3 sampling time selection */
#define  ADC_SMPR3_SMP3_0                    ((uint32_t)0x00000200U)        /*!< Bit 0 */
#define  ADC_SMPR3_SMP3_1                    ((uint32_t)0x00000400U)        /*!< Bit 1 */
#define  ADC_SMPR3_SMP3_2                    ((uint32_t)0x00000800U)        /*!< Bit 2 */

#define  ADC_SMPR3_SMP4                      ((uint32_t)0x00007000U)        /*!< ADC channel 4 sampling time selection */
#define  ADC_SMPR3_SMP4_0                    ((uint32_t)0x00001000U)        /*!< Bit 0 */
#define  ADC_SMPR3_SMP4_1                    ((uint32_t)0x00002000U)        /*!< Bit 1 */
#define  ADC_SMPR3_SMP4_2                    ((uint32_t)0x00004000U)        /*!< Bit 2 */

#define  ADC_SMPR3_SMP5                      ((uint32_t)0x00038000U)        /*!< ADC channel 5 sampling time selection */
#define  ADC_SMPR3_SMP5_0                    ((uint32_t)0x00008000U)        /*!< Bit 0 */
#define  ADC_SMPR3_SMP5_1                    ((uint32_t)0x00010000U)        /*!< Bit 1 */
#define  ADC_SMPR3_SMP5_2                    ((uint32_t)0x00020000U)        /*!< Bit 2 */

#define  ADC_SMPR3_SMP6                      ((uint32_t)0x001C0000U)        /*!< ADC channel 6 sampling time selection */
#define  ADC_SMPR3_SMP6_0                    ((uint32_t)0x00040000U)        /*!< Bit 0 */
#define  ADC_SMPR3_SMP6_1                    ((uint32_t)0x00080000U)        /*!< Bit 1 */
#define  ADC_SMPR3_SMP6_2                    ((uint32_t)0x00100000U)        /*!< Bit 2 */

#define  ADC_SMPR3_SMP7                      ((uint32_t)0x00E00000U)        /*!< ADC channel 7 sampling time selection */
#define  ADC_SMPR3_SMP7_0                    ((uint32_t)0x00200000U)        /*!< Bit 0 */
#define  ADC_SMPR3_SMP7_1                    ((uint32_t)0x00400000U)        /*!< Bit 1 */
#define  ADC_SMPR3_SMP7_2                    ((uint32_t)0x00800000U)        /*!< Bit 2 */

#define  ADC_SMPR3_SMP8                      ((uint32_t)0x07000000U)        /*!< ADC channel 8 sampling time selection */
#define  ADC_SMPR3_SMP8_0                    ((uint32_t)0x01000000U)        /*!< Bit 0 */
#define  ADC_SMPR3_SMP8_1                    ((uint32_t)0x02000000U)        /*!< Bit 1 */
#define  ADC_SMPR3_SMP8_2                    ((uint32_t)0x04000000U)        /*!< Bit 2 */

#define  ADC_SMPR3_SMP9                      ((uint32_t)0x38000000U)        /*!< ADC channel 9 sampling time selection */
#define  ADC_SMPR3_SMP9_0                    ((uint32_t)0x08000000U)        /*!< Bit 0 */
#define  ADC_SMPR3_SMP9_1                    ((uint32_t)0x10000000U)        /*!< Bit 1 */
#define  ADC_SMPR3_SMP9_2                    ((uint32_t)0x20000000U)        /*!< Bit 2 */

/******************  Bit definition for ADC_JOFR1 register  *******************/
#define  ADC_JOFR1_JOFFSET1                  ((uint32_t)0x00000FFFU)        /*!< ADC group injected sequencer rank 1 offset value */

/******************  Bit definition for ADC_JOFR2 register  *******************/
#define  ADC_JOFR2_JOFFSET2                  ((uint32_t)0x00000FFFU)        /*!< ADC group injected sequencer rank 2 offset value */

/******************  Bit definition for ADC_JOFR3 register  *******************/
#define  ADC_JOFR3_JOFFSET3                  ((uint32_t)0x00000FFFU)        /*!< ADC group injected sequencer rank 3 offset value */

/******************  Bit definition for ADC_JOFR4 register  *******************/
#define  ADC_JOFR4_JOFFSET4                  ((uint32_t)0x00000FFFU)        /*!< ADC group injected sequencer rank 4 offset value */

/*******************  Bit definition for ADC_HTR register  ********************/
#define  ADC_HTR_HT                          ((uint32_t)0x00000FFFU)        /*!< ADC analog watchdog 1 threshold high */

/*******************  Bit definition for ADC_LTR register  ********************/
#define  ADC_LTR_LT                          ((uint32_t)0x00000FFFU)        /*!< ADC analog watchdog 1 threshold low */

/*******************  Bit definition for ADC_SQR1 register  *******************/
#define  ADC_SQR1_L                          ((uint32_t)0x01F00000U)        /*!< ADC group regular sequencer scan length */
#define  ADC_SQR1_L_0                        ((uint32_t)0x00100000U)        /*!< Bit 0 */
#define  ADC_SQR1_L_1                        ((uint32_t)0x00200000U)        /*!< Bit 1 */
#define  ADC_SQR1_L_2                        ((uint32_t)0x00400000U)        /*!< Bit 2 */
#define  ADC_SQR1_L_3                        ((uint32_t)0x00800000U)        /*!< Bit 3 */
#define  ADC_SQR1_L_4                        ((uint32_t)0x01000000U)        /*!< Bit 4 */

#define  ADC_SQR1_SQ28                       ((uint32_t)0x000F8000U)        /*!< ADC group regular sequencer rank 28 */
#define  ADC_SQR1_SQ28_0                     ((uint32_t)0x00008000U)        /*!< Bit 0 */
#define  ADC_SQR1_SQ28_1                     ((uint32_t)0x00010000U)        /*!< Bit 1 */
#define  ADC_SQR1_SQ28_2                     ((uint32_t)0x00020000U)        /*!< Bit 2 */
#define  ADC_SQR1_SQ28_3                     ((uint32_t)0x00040000U)        /*!< Bit 3 */
#define  ADC_SQR1_SQ28_4                     ((uint32_t)0x00080000U)        /*!< Bit 4 */

#define  ADC_SQR1_SQ27                       ((uint32_t)0x00007C00U)        /*!< ADC group regular sequencer rank 27 */
#define  ADC_SQR1_SQ27_0                     ((uint32_t)0x00000400U)        /*!< Bit 0 */
#define  ADC_SQR1_SQ27_1                     ((uint32_t)0x00000800U)        /*!< Bit 1 */
#define  ADC_SQR1_SQ27_2                     ((uint32_t)0x00001000U)        /*!< Bit 2 */
#define  ADC_SQR1_SQ27_3                     ((uint32_t)0x00002000U)        /*!< Bit 3 */
#define  ADC_SQR1_SQ27_4                     ((uint32_t)0x00004000U)        /*!< Bit 4 */

#define  ADC_SQR1_SQ26                       ((uint32_t)0x000003E0U)        /*!< ADC group regular sequencer rank 26 */
#define  ADC_SQR1_SQ26_0                     ((uint32_t)0x00000020U)        /*!< Bit 0 */
#define  ADC_SQR1_SQ26_1                     ((uint32_t)0x00000040U)        /*!< Bit 1 */
#define  ADC_SQR1_SQ26_2                     ((uint32_t)0x00000080U)        /*!< Bit 2 */
#define  ADC_SQR1_SQ26_3                     ((uint32_t)0x00000100U)        /*!< Bit 3 */
#define  ADC_SQR1_SQ26_4                     ((uint32_t)0x00000200U)        /*!< Bit 4 */

#define  ADC_SQR1_SQ25                       ((uint32_t)0x0000001FU)        /*!< ADC group regular sequencer rank 25 */
#define  ADC_SQR1_SQ25_0                     ((uint32_t)0x00000001U)        /*!< Bit 0 */
#define  ADC_SQR1_SQ25_1                     ((uint32_t)0x00000002U)        /*!< Bit 1 */
#define  ADC_SQR1_SQ25_2                     ((uint32_t)0x00000004U)        /*!< Bit 2 */
#define  ADC_SQR1_SQ25_3                     ((uint32_t)0x00000008U)        /*!< Bit 3 */
#define  ADC_SQR1_SQ25_4                     ((uint32_t)0x00000010U)        /*!< Bit 4 */

/*******************  Bit definition for ADC_SQR2 register  *******************/
#define  ADC_SQR2_SQ19                       ((uint32_t)0x0000001FU)        /*!< ADC group regular sequencer rank 19 */
#define  ADC_SQR2_SQ19_0                     ((uint32_t)0x00000001U)        /*!< Bit 0 */
#define  ADC_SQR2_SQ19_1                     ((uint32_t)0x00000002U)        /*!< Bit 1 */
#define  ADC_SQR2_SQ19_2                     ((uint32_t)0x00000004U)        /*!< Bit 2 */
#define  ADC_SQR2_SQ19_3                     ((uint32_t)0x00000008U)        /*!< Bit 3 */
#define  ADC_SQR2_SQ19_4                     ((uint32_t)0x00000010U)        /*!< Bit 4 */

#define  ADC_SQR2_SQ20                       ((uint32_t)0x000003E0U)        /*!< ADC group regular sequencer rank 20 */
#define  ADC_SQR2_SQ20_0                     ((uint32_t)0x00000020U)        /*!< Bit 0 */
#define  ADC_SQR2_SQ20_1                     ((uint32_t)0x00000040U)        /*!< Bit 1 */
#define  ADC_SQR2_SQ20_2                     ((uint32_t)0x00000080U)        /*!< Bit 2 */
#define  ADC_SQR2_SQ20_3                     ((uint32_t)0x00000100U)        /*!< Bit 3 */
#define  ADC_SQR2_SQ20_4                     ((uint32_t)0x00000200U)        /*!< Bit 4 */

#define  ADC_SQR2_SQ21                       ((uint32_t)0x00007C00U)        /*!< ADC group regular sequencer rank 21 */
#define  ADC_SQR2_SQ21_0                     ((uint32_t)0x00000400U)        /*!< Bit 0 */
#define  ADC_SQR2_SQ21_1                     ((uint32_t)0x00000800U)        /*!< Bit 1 */
#define  ADC_SQR2_SQ21_2                     ((uint32_t)0x00001000U)        /*!< Bit 2 */
#define  ADC_SQR2_SQ21_3                     ((uint32_t)0x00002000U)        /*!< Bit 3 */
#define  ADC_SQR2_SQ21_4                     ((uint32_t)0x00004000U)        /*!< Bit 4 */

#define  ADC_SQR2_SQ22                       ((uint32_t)0x000F8000U)        /*!< ADC group regular sequencer rank 22 */
#define  ADC_SQR2_SQ22_0                     ((uint32_t)0x00008000U)        /*!< Bit 0 */
#define  ADC_SQR2_SQ22_1                     ((uint32_t)0x00010000U)        /*!< Bit 1 */
#define  ADC_SQR2_SQ22_2                     ((uint32_t)0x00020000U)        /*!< Bit 2 */
#define  ADC_SQR2_SQ22_3                     ((uint32_t)0x00040000U)        /*!< Bit 3 */
#define  ADC_SQR2_SQ22_4                     ((uint32_t)0x00080000U)        /*!< Bit 4 */

#define  ADC_SQR2_SQ23                       ((uint32_t)0x01F00000U)        /*!< ADC group regular sequencer rank 23 */
#define  ADC_SQR2_SQ23_0                     ((uint32_t)0x00100000U)        /*!< Bit 0 */
#define  ADC_SQR2_SQ23_1                     ((uint32_t)0x00200000U)        /*!< Bit 1 */
#define  ADC_SQR2_SQ23_2                     ((uint32_t)0x00400000U)        /*!< Bit 2 */
#define  ADC_SQR2_SQ23_3                     ((uint32_t)0x00800000U)        /*!< Bit 3 */
#define  ADC_SQR2_SQ23_4                     ((uint32_t)0x01000000U)        /*!< Bit 4 */

#define  ADC_SQR2_SQ24                       ((uint32_t)0x3E000000U)        /*!< ADC group regular sequencer rank 24 */
#define  ADC_SQR2_SQ24_0                     ((uint32_t)0x02000000U)        /*!< Bit 0 */
#define  ADC_SQR2_SQ24_1                     ((uint32_t)0x04000000U)        /*!< Bit 1 */
#define  ADC_SQR2_SQ24_2                     ((uint32_t)0x08000000U)        /*!< Bit 2 */
#define  ADC_SQR2_SQ24_3                     ((uint32_t)0x10000000U)        /*!< Bit 3 */
#define  ADC_SQR2_SQ24_4                     ((uint32_t)0x20000000U)        /*!< Bit 4 */

/*******************  Bit definition for ADC_SQR3 register  *******************/
#define  ADC_SQR3_SQ13                       ((uint32_t)0x0000001FU)        /*!< ADC group regular sequencer rank 13 */
#define  ADC_SQR3_SQ13_0                     ((uint32_t)0x00000001U)        /*!< Bit 0 */
#define  ADC_SQR3_SQ13_1                     ((uint32_t)0x00000002U)        /*!< Bit 1 */
#define  ADC_SQR3_SQ13_2                     ((uint32_t)0x00000004U)        /*!< Bit 2 */
#define  ADC_SQR3_SQ13_3                     ((uint32_t)0x00000008U)        /*!< Bit 3 */
#define  ADC_SQR3_SQ13_4                     ((uint32_t)0x00000010U)        /*!< Bit 4 */

#define  ADC_SQR3_SQ14                       ((uint32_t)0x000003E0U)        /*!< ADC group regular sequencer rank 14 */
#define  ADC_SQR3_SQ14_0                     ((uint32_t)0x00000020U)        /*!< Bit 0 */
#define  ADC_SQR3_SQ14_1                     ((uint32_t)0x00000040U)        /*!< Bit 1 */
#define  ADC_SQR3_SQ14_2                     ((uint32_t)0x00000080U)        /*!< Bit 2 */
#define  ADC_SQR3_SQ14_3                     ((uint32_t)0x00000100U)        /*!< Bit 3 */
#define  ADC_SQR3_SQ14_4                     ((uint32_t)0x00000200U)        /*!< Bit 4 */

#define  ADC_SQR3_SQ15                       ((uint32_t)0x00007C00U)        /*!< ADC group regular sequencer rank 15 */
#define  ADC_SQR3_SQ15_0                     ((uint32_t)0x00000400U)        /*!< Bit 0 */
#define  ADC_SQR3_SQ15_1                     ((uint32_t)0x00000800U)        /*!< Bit 1 */
#define  ADC_SQR3_SQ15_2                     ((uint32_t)0x00001000U)        /*!< Bit 2 */
#define  ADC_SQR3_SQ15_3                     ((uint32_t)0x00002000U)        /*!< Bit 3 */
#define  ADC_SQR3_SQ15_4                     ((uint32_t)0x00004000U)        /*!< Bit 4 */

#define  ADC_SQR3_SQ16                       ((uint32_t)0x000F8000U)        /*!< ADC group regular sequencer rank 16 */
#define  ADC_SQR3_SQ16_0                     ((uint32_t)0x00008000U)        /*!< Bit 0 */
#define  ADC_SQR3_SQ16_1                     ((uint32_t)0x00010000U)        /*!< Bit 1 */
#define  ADC_SQR3_SQ16_2                     ((uint32_t)0x00020000U)        /*!< Bit 2 */
#define  ADC_SQR3_SQ16_3                     ((uint32_t)0x00040000U)        /*!< Bit 3 */
#define  ADC_SQR3_SQ16_4                     ((uint32_t)0x00080000U)        /*!< Bit 4 */

#define  ADC_SQR3_SQ17                       ((uint32_t)0x01F00000U)        /*!< ADC group regular sequencer rank 17 */
#define  ADC_SQR3_SQ17_0                     ((uint32_t)0x00100000U)        /*!< Bit 0 */
#define  ADC_SQR3_SQ17_1                     ((uint32_t)0x00200000U)        /*!< Bit 1 */
#define  ADC_SQR3_SQ17_2                     ((uint32_t)0x00400000U)        /*!< Bit 2 */
#define  ADC_SQR3_SQ17_3                     ((uint32_t)0x00800000U)        /*!< Bit 3 */
#define  ADC_SQR3_SQ17_4                     ((uint32_t)0x01000000U)        /*!< Bit 4 */

#define  ADC_SQR3_SQ18                       ((uint32_t)0x3E000000U)        /*!< ADC group regular sequencer rank 18 */
#define  ADC_SQR3_SQ18_0                     ((uint32_t)0x02000000U)        /*!< Bit 0 */
#define  ADC_SQR3_SQ18_1                     ((uint32_t)0x04000000U)        /*!< Bit 1 */
#define  ADC_SQR3_SQ18_2                     ((uint32_t)0x08000000U)        /*!< Bit 2 */
#define  ADC_SQR3_SQ18_3                     ((uint32_t)0x10000000U)        /*!< Bit 3 */
#define  ADC_SQR3_SQ18_4                     ((uint32_t)0x20000000U)        /*!< Bit 4 */

/*******************  Bit definition for ADC_SQR4 register  *******************/
#define  ADC_SQR4_SQ7                        ((uint32_t)0x0000001FU)        /*!< ADC group regular sequencer rank 7 */
#define  ADC_SQR4_SQ7_0                      ((uint32_t)0x00000001U)        /*!< Bit 0 */
#define  ADC_SQR4_SQ7_1                      ((uint32_t)0x00000002U)        /*!< Bit 1 */
#define  ADC_SQR4_SQ7_2                      ((uint32_t)0x00000004U)        /*!< Bit 2 */
#define  ADC_SQR4_SQ7_3                      ((uint32_t)0x00000008U)        /*!< Bit 3 */
#define  ADC_SQR4_SQ7_4                      ((uint32_t)0x00000010U)        /*!< Bit 4 */

#define  ADC_SQR4_SQ8                        ((uint32_t)0x000003E0U)        /*!< ADC group regular sequencer rank 8 */
#define  ADC_SQR4_SQ8_0                      ((uint32_t)0x00000020U)        /*!< Bit 0 */
#define  ADC_SQR4_SQ8_1                      ((uint32_t)0x00000040U)        /*!< Bit 1 */
#define  ADC_SQR4_SQ8_2                      ((uint32_t)0x00000080U)        /*!< Bit 2 */
#define  ADC_SQR4_SQ8_3                      ((uint32_t)0x00000100U)        /*!< Bit 3 */
#define  ADC_SQR4_SQ8_4                      ((uint32_t)0x00000200U)        /*!< Bit 4 */

#define  ADC_SQR4_SQ9                        ((uint32_t)0x00007C00U)        /*!< ADC group regular sequencer rank 9 */
#define  ADC_SQR4_SQ9_0                      ((uint32_t)0x00000400U)        /*!< Bit 0 */
#define  ADC_SQR4_SQ9_1                      ((uint32_t)0x00000800U)        /*!< Bit 1 */
#define  ADC_SQR4_SQ9_2                      ((uint32_t)0x00001000U)        /*!< Bit 2 */
#define  ADC_SQR4_SQ9_3                      ((uint32_t)0x00002000U)        /*!< Bit 3 */
#define  ADC_SQR4_SQ9_4                      ((uint32_t)0x00004000U)        /*!< Bit 4 */

#define  ADC_SQR4_SQ10                        ((uint32_t)0x000F8000U)        /*!< ADC group regular sequencer rank 10 */
#define  ADC_SQR4_SQ10_0                      ((uint32_t)0x00008000U)        /*!< Bit 0 */
#define  ADC_SQR4_SQ10_1                      ((uint32_t)0x00010000U)        /*!< Bit 1 */
#define  ADC_SQR4_SQ10_2                      ((uint32_t)0x00020000U)        /*!< Bit 2 */
#define  ADC_SQR4_SQ10_3                      ((uint32_t)0x00040000U)        /*!< Bit 3 */
#define  ADC_SQR4_SQ10_4                      ((uint32_t)0x00080000U)        /*!< Bit 4 */

#define  ADC_SQR4_SQ11                        ((uint32_t)0x01F00000U)        /*!< ADC group regular sequencer rank 11 */
#define  ADC_SQR4_SQ11_0                      ((uint32_t)0x00100000U)        /*!< Bit 0 */
#define  ADC_SQR4_SQ11_1                      ((uint32_t)0x00200000U)        /*!< Bit 1 */
#define  ADC_SQR4_SQ11_2                      ((uint32_t)0x00400000U)        /*!< Bit 2 */
#define  ADC_SQR4_SQ11_3                      ((uint32_t)0x00800000U)        /*!< Bit 3 */
#define  ADC_SQR4_SQ11_4                      ((uint32_t)0x01000000U)        /*!< Bit 4 */

#define  ADC_SQR4_SQ12                        ((uint32_t)0x3E000000U)        /*!< ADC group regular sequencer rank 12 */
#define  ADC_SQR4_SQ12_0                      ((uint32_t)0x02000000U)        /*!< Bit 0 */
#define  ADC_SQR4_SQ12_1                      ((uint32_t)0x04000000U)        /*!< Bit 1 */
#define  ADC_SQR4_SQ12_2                      ((uint32_t)0x08000000U)        /*!< Bit 2 */
#define  ADC_SQR4_SQ12_3                      ((uint32_t)0x10000000U)        /*!< Bit 3 */
#define  ADC_SQR4_SQ12_4                      ((uint32_t)0x20000000U)        /*!< Bit 4 */

/*******************  Bit definition for ADC_SQR5 register  *******************/
#define  ADC_SQR5_SQ1                        ((uint32_t)0x0000001FU)        /*!< ADC group regular sequencer rank 1 */
#define  ADC_SQR5_SQ1_0                      ((uint32_t)0x00000001U)        /*!< Bit 0 */
#define  ADC_SQR5_SQ1_1                      ((uint32_t)0x00000002U)        /*!< Bit 1 */
#define  ADC_SQR5_SQ1_2                      ((uint32_t)0x00000004U)        /*!< Bit 2 */
#define  ADC_SQR5_SQ1_3                      ((uint32_t)0x00000008U)        /*!< Bit 3 */
#define  ADC_SQR5_SQ1_4                      ((uint32_t)0x00000010U)        /*!< Bit 4 */

#define  ADC_SQR5_SQ2                        ((uint32_t)0x000003E0U)        /*!< ADC group regular sequencer rank 2 */
#define  ADC_SQR5_SQ2_0                      ((uint32_t)0x00000020U)        /*!< Bit 0 */
#define  ADC_SQR5_SQ2_1                      ((uint32_t)0x00000040U)        /*!< Bit 1 */
#define  ADC_SQR5_SQ2_2                      ((uint32_t)0x00000080U)        /*!< Bit 2 */
#define  ADC_SQR5_SQ2_3                      ((uint32_t)0x00000100U)        /*!< Bit 3 */
#define  ADC_SQR5_SQ2_4                      ((uint32_t)0x00000200U)        /*!< Bit 4 */

#define  ADC_SQR5_SQ3                        ((uint32_t)0x00007C00U)        /*!< ADC group regular sequencer rank 3 */
#define  ADC_SQR5_SQ3_0                      ((uint32_t)0x00000400U)        /*!< Bit 0 */
#define  ADC_SQR5_SQ3_1                      ((uint32_t)0x00000800U)        /*!< Bit 1 */
#define  ADC_SQR5_SQ3_2                      ((uint32_t)0x00001000U)        /*!< Bit 2 */
#define  ADC_SQR5_SQ3_3                      ((uint32_t)0x00002000U)        /*!< Bit 3 */
#define  ADC_SQR5_SQ3_4                      ((uint32_t)0x00004000U)        /*!< Bit 4 */

#define  ADC_SQR5_SQ4                        ((uint32_t)0x000F8000U)        /*!< ADC group regular sequencer rank 4 */
#define  ADC_SQR5_SQ4_0                      ((uint32_t)0x00008000U)        /*!< Bit 0 */
#define  ADC_SQR5_SQ4_1                      ((uint32_t)0x00010000U)        /*!< Bit 1 */
#define  ADC_SQR5_SQ4_2                      ((uint32_t)0x00020000U)        /*!< Bit 2 */
#define  ADC_SQR5_SQ4_3                      ((uint32_t)0x00040000U)        /*!< Bit 3 */
#define  ADC_SQR5_SQ4_4                      ((uint32_t)0x00080000U)        /*!< Bit 4 */

#define  ADC_SQR5_SQ5                        ((uint32_t)0x01F00000U)        /*!< ADC group regular sequencer rank 5 */
#define  ADC_SQR5_SQ5_0                      ((uint32_t)0x00100000U)        /*!< Bit 0 */
#define  ADC_SQR5_SQ5_1                      ((uint32_t)0x00200000U)        /*!< Bit 1 */
#define  ADC_SQR5_SQ5_2                      ((uint32_t)0x00400000U)        /*!< Bit 2 */
#define  ADC_SQR5_SQ5_3                      ((uint32_t)0x00800000U)        /*!< Bit 3 */
#define  ADC_SQR5_SQ5_4                      ((uint32_t)0x01000000U)        /*!< Bit 4 */

#define  ADC_SQR5_SQ6                        ((uint32_t)0x3E000000U)        /*!< ADC group regular sequencer rank 6 */
#define  ADC_SQR5_SQ6_0                      ((uint32_t)0x02000000U)        /*!< Bit 0 */
#define  ADC_SQR5_SQ6_1                      ((uint32_t)0x04000000U)        /*!< Bit 1 */
#define  ADC_SQR5_SQ6_2                      ((uint32_t)0x08000000U)        /*!< Bit 2 */
#define  ADC_SQR5_SQ6_3                      ((uint32_t)0x10000000U)        /*!< Bit 3 */
#define  ADC_SQR5_SQ6_4                      ((uint32_t)0x20000000U)        /*!< Bit 4 */


/*******************  Bit definition for ADC_JSQR register  *******************/
#define  ADC_JSQR_JSQ1                       ((uint32_t)0x0000001FU)        /*!< ADC group injected sequencer rank 1 */
#define  ADC_JSQR_JSQ1_0                     ((uint32_t)0x00000001U)        /*!< Bit 0 */
#define  ADC_JSQR_JSQ1_1                     ((uint32_t)0x00000002U)        /*!< Bit 1 */
#define  ADC_JSQR_JSQ1_2                     ((uint32_t)0x00000004U)        /*!< Bit 2 */
#define  ADC_JSQR_JSQ1_3                     ((uint32_t)0x00000008U)        /*!< Bit 3 */
#define  ADC_JSQR_JSQ1_4                     ((uint32_t)0x00000010U)        /*!< Bit 4 */

#define  ADC_JSQR_JSQ2                       ((uint32_t)0x000003E0U)        /*!< ADC group injected sequencer rank 2 */
#define  ADC_JSQR_JSQ2_0                     ((uint32_t)0x00000020U)        /*!< Bit 0 */
#define  ADC_JSQR_JSQ2_1                     ((uint32_t)0x00000040U)        /*!< Bit 1 */
#define  ADC_JSQR_JSQ2_2                     ((uint32_t)0x00000080U)        /*!< Bit 2 */
#define  ADC_JSQR_JSQ2_3                     ((uint32_t)0x00000100U)        /*!< Bit 3 */
#define  ADC_JSQR_JSQ2_4                     ((uint32_t)0x00000200U)        /*!< Bit 4 */

#define  ADC_JSQR_JSQ3                       ((uint32_t)0x00007C00U)        /*!< ADC group injected sequencer rank 3 */
#define  ADC_JSQR_JSQ3_0                     ((uint32_t)0x00000400U)        /*!< Bit 0 */
#define  ADC_JSQR_JSQ3_1                     ((uint32_t)0x00000800U)        /*!< Bit 1 */
#define  ADC_JSQR_JSQ3_2                     ((uint32_t)0x00001000U)        /*!< Bit 2 */
#define  ADC_JSQR_JSQ3_3                     ((uint32_t)0x00002000U)        /*!< Bit 3 */
#define  ADC_JSQR_JSQ3_4                     ((uint32_t)0x00004000U)        /*!< Bit 4 */

#define  ADC_JSQR_JSQ4                       ((uint32_t)0x000F8000U)        /*!< ADC group injected sequencer rank 4 */
#define  ADC_JSQR_JSQ4_0                     ((uint32_t)0x00008000U)        /*!< Bit 0 */
#define  ADC_JSQR_JSQ4_1                     ((uint32_t)0x00010000U)        /*!< Bit 1 */
#define  ADC_JSQR_JSQ4_2                     ((uint32_t)0x00020000U)        /*!< Bit 2 */
#define  ADC_JSQR_JSQ4_3                     ((uint32_t)0x00040000U)        /*!< Bit 3 */
#define  ADC_JSQR_JSQ4_4                     ((uint32_t)0x00080000U)        /*!< Bit 4 */

#define  ADC_JSQR_JL                         ((uint32_t)0x00300000U)        /*!< ADC group injected sequencer scan length */
#define  ADC_JSQR_JL_0                       ((uint32_t)0x00100000U)        /*!< Bit 0 */
#define  ADC_JSQR_JL_1                       ((uint32_t)0x00200000U)        /*!< Bit 1 */

/*******************  Bit definition for ADC_JDR1 register  *******************/
#define  ADC_JDR1_JDATA                      ((uint32_t)0x0000FFFFU)        /*!< ADC group injected sequencer rank 1 conversion data */

/*******************  Bit definition for ADC_JDR2 register  *******************/
#define  ADC_JDR2_JDATA                      ((uint32_t)0x0000FFFFU)        /*!< ADC group injected sequencer rank 2 conversion data */

/*******************  Bit definition for ADC_JDR3 register  *******************/
#define  ADC_JDR3_JDATA                      ((uint32_t)0x0000FFFFU)        /*!< ADC group injected sequencer rank 3 conversion data */

/*******************  Bit definition for ADC_JDR4 register  *******************/
#define  ADC_JDR4_JDATA                      ((uint32_t)0x0000FFFFU)        /*!< ADC group injected sequencer rank 4 conversion data */

/********************  Bit definition for ADC_DR register  ********************/
#define  ADC_DR_DATA                         ((uint32_t)0x0000FFFFU)        /*!< ADC group regular conversion data */

/******************  Bit definition for ADC_SMPR0 register  *******************/
#define  ADC_SMPR0_SMP30                     ((uint32_t)0x00000007U)        /*!< ADC channel 30 sampling time selection */
#define  ADC_SMPR0_SMP30_0                   ((uint32_t)0x00000001U)        /*!< Bit 0 */
#define  ADC_SMPR0_SMP30_1                   ((uint32_t)0x00000002U)        /*!< Bit 1 */
#define  ADC_SMPR0_SMP30_2                   ((uint32_t)0x00000004U)        /*!< Bit 2 */
 
#define  ADC_SMPR0_SMP31                     ((uint32_t)0x00000038U)        /*!< ADC channel 31 sampling time selection */
#define  ADC_SMPR0_SMP31_0                   ((uint32_t)0x00000008U)        /*!< Bit 0 */
#define  ADC_SMPR0_SMP31_1                   ((uint32_t)0x00000010U)        /*!< Bit 1 */
#define  ADC_SMPR0_SMP31_2                   ((uint32_t)0x00000020U)        /*!< Bit 2 */

/*******************  Bit definition for ADC_CSR register  ********************/
#define  ADC_CSR_AWD1                        ((uint32_t)0x00000001U)        /*!< ADC multimode master analog watchdog 1 flag */
#define  ADC_CSR_EOCS1                       ((uint32_t)0x00000002U)        /*!< ADC multimode master group regular end of unitary conversion or end of sequence conversions flag */
#define  ADC_CSR_JEOS1                       ((uint32_t)0x00000004U)        /*!< ADC multimode master group injected end of sequence conversions flag */
#define  ADC_CSR_JSTRT1                      ((uint32_t)0x00000008U)        /*!< ADC multimode master group injected conversion start flag */
#define  ADC_CSR_STRT1                       ((uint32_t)0x00000010U)        /*!< ADC multimode master group regular conversion start flag */
#define  ADC_CSR_OVR1                        ((uint32_t)0x00000020U)        /*!< ADC multimode master group regular overrun flag */
#define  ADC_CSR_ADONS1                      ((uint32_t)0x00000040U)        /*!< ADC multimode master ready flag */

/* Legacy defines */
#define  ADC_CSR_EOC1                        (ADC_CSR_EOCS1)
#define  ADC_CSR_JEOC1                       (ADC_CSR_JEOS1)

/*******************  Bit definition for ADC_CCR register  ********************/
#define  ADC_CCR_ADCPRE                      ((uint32_t)0x00030000U)        /*!< ADC clock source asynchronous prescaler */
#define  ADC_CCR_ADCPRE_0                    ((uint32_t)0x00010000U)        /*!< Bit 0 */
#define  ADC_CCR_ADCPRE_1                    ((uint32_t)0x00020000U)        /*!< Bit 1 */ 
#define  ADC_CCR_TSVREFE                     ((uint32_t)0x00800000U)        /*!< ADC internal path to VrefInt and temperature sensor enable */

/******************************************************************************/
/*                                                                            */
/*                      Analog Comparators (COMP)                             */
/*                                                                            */
/******************************************************************************/

/******************  Bit definition for COMP_CSR register  ********************/
#define  COMP_CSR_10KPU                      ((uint32_t)0x00000001U)        /*!< 10K pull-up resistor */
#define  COMP_CSR_400KPU                     ((uint32_t)0x00000002U)        /*!< 400K pull-up resistor */
#define  COMP_CSR_10KPD                      ((uint32_t)0x00000004U)        /*!< 10K pull-down resistor */
#define  COMP_CSR_400KPD                     ((uint32_t)0x00000008U)        /*!< 400K pull-down resistor */
#define  COMP_CSR_CMP1EN                     ((uint32_t)0x00000010U)        /*!< Comparator 1 enable */
#define  COMP_CSR_SW1                        ((uint32_t)0x00000020U)        /*!< SW1 analog switch enable */
#define  COMP_CSR_CMP1OUT                    ((uint32_t)0x00000080U)        /*!< Comparator 1 output */

#define  COMP_CSR_SPEED                      ((uint32_t)0x00001000U)        /*!< Comparator 2 speed */
#define  COMP_CSR_CMP2OUT                    ((uint32_t)0x00002000U)        /*!< Comparator 2 ouput */
#define  COMP_CSR_VREFOUTEN                  ((uint32_t)0x00010000U)        /*!< Comparator Vref Enable */
#define  COMP_CSR_WNDWE                      ((uint32_t)0x00020000U)        /*!< Window mode enable */
#define  COMP_CSR_INSEL                      ((uint32_t)0x001C0000U)        /*!< INSEL[2:0] Inversion input Selection */
#define  COMP_CSR_INSEL_0                    ((uint32_t)0x00040000U)        /*!< Bit 0 */
#define  COMP_CSR_INSEL_1                    ((uint32_t)0x00080000U)        /*!< Bit 1 */
#define  COMP_CSR_INSEL_2                    ((uint32_t)0x00100000U)        /*!< Bit 2 */
#define  COMP_CSR_OUTSEL                     ((uint32_t)0x00E00000U)        /*!< OUTSEL[2:0] comparator 2 output redirection */
#define  COMP_CSR_OUTSEL_0                   ((uint32_t)0x00200000U)        /*!< Bit 0 */
#define  COMP_CSR_OUTSEL_1                   ((uint32_t)0x00400000U)        /*!< Bit 1 */
#define  COMP_CSR_OUTSEL_2                   ((uint32_t)0x00800000U)        /*!< Bit 2 */

#define  COMP_CSR_FCH3                       ((uint32_t)0x04000000U)        /*!< Bit 26 */
#define  COMP_CSR_FCH8                       ((uint32_t)0x08000000U)        /*!< Bit 27 */
#define  COMP_CSR_RCH13                      ((uint32_t)0x10000000U)        /*!< Bit 28 */

#define  COMP_CSR_CAIE                       ((uint32_t)0x20000000U)        /*!< Bit 29 */
#define  COMP_CSR_CAIF                       ((uint32_t)0x40000000U)        /*!< Bit 30 */
#define  COMP_CSR_TSUSP                      ((uint32_t)0x80000000U)        /*!< Bit 31 */

/******************************************************************************/
/*                                                                            */
/*                         Operational Amplifier (OPAMP)                      */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for OPAMP_CSR register  ******************/
#define OPAMP_CSR_OPA1PD                     ((uint32_t)0x00000001U)        /*!< OPAMP1 disable */
#define OPAMP_CSR_S3SEL1                     ((uint32_t)0x00000002U)        /*!< Switch 3 for OPAMP1 Enable */
#define OPAMP_CSR_S4SEL1                     ((uint32_t)0x00000004U)        /*!< Switch 4 for OPAMP1 Enable */
#define OPAMP_CSR_S5SEL1                     ((uint32_t)0x00000008U)        /*!< Switch 5 for OPAMP1 Enable */
#define OPAMP_CSR_S6SEL1                     ((uint32_t)0x00000010U)        /*!< Switch 6 for OPAMP1 Enable */
#define OPAMP_CSR_OPA1CAL_L                  ((uint32_t)0x00000020U)        /*!< OPAMP1 Offset calibration for P differential pair */
#define OPAMP_CSR_OPA1CAL_H                  ((uint32_t)0x00000040U)        /*!< OPAMP1 Offset calibration for N differential pair */
#define OPAMP_CSR_OPA1LPM                    ((uint32_t)0x00000080U)        /*!< OPAMP1 Low power enable */
#define OPAMP_CSR_OPA2PD                     ((uint32_t)0x00000100U)        /*!< OPAMP2 disable */
#define OPAMP_CSR_S3SEL2                     ((uint32_t)0x00000200U)        /*!< Switch 3 for OPAMP2 Enable */
#define OPAMP_CSR_S4SEL2                     ((uint32_t)0x00000400U)        /*!< Switch 4 for OPAMP2 Enable */
#define OPAMP_CSR_S5SEL2                     ((uint32_t)0x00000800U)        /*!< Switch 5 for OPAMP2 Enable */
#define OPAMP_CSR_S6SEL2                     ((uint32_t)0x00001000U)        /*!< Switch 6 for OPAMP2 Enable */
#define OPAMP_CSR_OPA2CAL_L                  ((uint32_t)0x00002000U)        /*!< OPAMP2 Offset calibration for P differential pair */
#define OPAMP_CSR_OPA2CAL_H                  ((uint32_t)0x00004000U)        /*!< OPAMP2 Offset calibration for N differential pair */
#define OPAMP_CSR_OPA2LPM                    ((uint32_t)0x00008000U)        /*!< OPAMP2 Low power enable */
#define OPAMP_CSR_ANAWSEL1                   ((uint32_t)0x01000000U)        /*!< Switch ANA Enable for OPAMP1 */ 
#define OPAMP_CSR_ANAWSEL2                   ((uint32_t)0x02000000U)        /*!< Switch ANA Enable for OPAMP2 */
#define OPAMP_CSR_S7SEL2                     ((uint32_t)0x08000000U)        /*!< Switch 7 for OPAMP2 Enable */
#define OPAMP_CSR_AOP_RANGE                  ((uint32_t)0x10000000U)        /*!< Power range selection */
#define OPAMP_CSR_OPA1CALOUT                 ((uint32_t)0x20000000U)        /*!< OPAMP1 calibration output */
#define OPAMP_CSR_OPA2CALOUT                 ((uint32_t)0x40000000U)        /*!< OPAMP2 calibration output */

/*******************  Bit definition for OPAMP_OTR register  ******************/
#define OPAMP_OTR_AO1_OPT_OFFSET_TRIM_LOW    ((uint32_t)0x0000001FU)        /*!< Offset trim for transistors differential pair PMOS of OPAMP1 */
#define OPAMP_OTR_AO1_OPT_OFFSET_TRIM_HIGH   ((uint32_t)0x000003E0U)        /*!< Offset trim for transistors differential pair NMOS of OPAMP1 */
#define OPAMP_OTR_AO2_OPT_OFFSET_TRIM_LOW    ((uint32_t)0x00007C00U)        /*!< Offset trim for transistors differential pair PMOS of OPAMP2 */
#define OPAMP_OTR_AO2_OPT_OFFSET_TRIM_HIGH   ((uint32_t)0x000F8000U)        /*!< Offset trim for transistors differential pair NMOS of OPAMP2 */
#define OPAMP_OTR_OT_USER                    ((uint32_t)0x80000000U)        /*!< Switch to OPAMP offset user trimmed values */

/*******************  Bit definition for OPAMP_LPOTR register  ****************/
#define OPAMP_OTR_AO1_OPT_OFFSET_TRIM_LP_LOW  ((uint32_t)0x0000001FU)        /*!< Offset trim for transistors differential pair PMOS of OPAMP1 */
#define OPAMP_OTR_AO1_OPT_OFFSET_TRIM_LP_HIGH ((uint32_t)0x000003E0U)        /*!< Offset trim for transistors differential pair NMOS of OPAMP1 */
#define OPAMP_OTR_AO2_OPT_OFFSET_TRIM_LP_LOW  ((uint32_t)0x00007C00U)        /*!< Offset trim for transistors differential pair PMOS of OPAMP2 */
#define OPAMP_OTR_AO2_OPT_OFFSET_TRIM_LP_HIGH ((uint32_t)0x000F8000U)        /*!< Offset trim for transistors differential pair NMOS of OPAMP2 */

/******************************************************************************/
/*                                                                            */
/*                       CRC calculation unit (CRC)                           */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for CRC_DR register  *********************/
#define  CRC_DR_DR                           ((uint32_t)0xFFFFFFFFU)        /*!< Data register bits */

/*******************  Bit definition for CRC_IDR register  ********************/
#define  CRC_IDR_IDR                         ((uint32_t)0xFF)              /*!< General-purpose 8-bit data register bits */

/********************  Bit definition for CRC_CR register  ********************/
#define  CRC_CR_RESET                        ((uint32_t)0x00000001U)        /*!< RESET bit */

/******************************************************************************/
/*                                                                            */
/*                    Digital to Analog Converter (DAC)                       */
/*                                                                            */
/******************************************************************************/

/********************  Bit definition for DAC_CR register  ********************/
#define  DAC_CR_EN1                          ((uint32_t)0x00000001U)        /*!<DAC channel1 enable */
#define  DAC_CR_BOFF1                        ((uint32_t)0x00000002U)        /*!<DAC channel1 output buffer disable */
#define  DAC_CR_TEN1                         ((uint32_t)0x00000004U)        /*!<DAC channel1 Trigger enable */

#define  DAC_CR_TSEL1                        ((uint32_t)0x00000038U)        /*!<TSEL1[2:0] (DAC channel1 Trigger selection) */
#define  DAC_CR_TSEL1_0                      ((uint32_t)0x00000008U)        /*!<Bit 0 */
#define  DAC_CR_TSEL1_1                      ((uint32_t)0x00000010U)        /*!<Bit 1 */
#define  DAC_CR_TSEL1_2                      ((uint32_t)0x00000020U)        /*!<Bit 2 */

#define  DAC_CR_WAVE1                        ((uint32_t)0x000000C0U)        /*!<WAVE1[1:0] (DAC channel1 noise/triangle wave generation enable) */
#define  DAC_CR_WAVE1_0                      ((uint32_t)0x00000040U)        /*!<Bit 0 */
#define  DAC_CR_WAVE1_1                      ((uint32_t)0x00000080U)        /*!<Bit 1 */

#define  DAC_CR_MAMP1                        ((uint32_t)0x00000F00U)        /*!<MAMP1[3:0] (DAC channel1 Mask/Amplitude selector) */
#define  DAC_CR_MAMP1_0                      ((uint32_t)0x00000100U)        /*!<Bit 0 */
#define  DAC_CR_MAMP1_1                      ((uint32_t)0x00000200U)        /*!<Bit 1 */
#define  DAC_CR_MAMP1_2                      ((uint32_t)0x00000400U)        /*!<Bit 2 */
#define  DAC_CR_MAMP1_3                      ((uint32_t)0x00000800U)        /*!<Bit 3 */

#define  DAC_CR_DMAEN1                       ((uint32_t)0x00001000U)        /*!<DAC channel1 DMA enable */
#define  DAC_CR_DMAUDRIE1                    ((uint32_t)0x00002000U)        /*!<DAC channel1 DMA Interrupt enable */
#define  DAC_CR_EN2                          ((uint32_t)0x00010000U)        /*!<DAC channel2 enable */
#define  DAC_CR_BOFF2                        ((uint32_t)0x00020000U)        /*!<DAC channel2 output buffer disable */
#define  DAC_CR_TEN2                         ((uint32_t)0x00040000U)        /*!<DAC channel2 Trigger enable */

#define  DAC_CR_TSEL2                        ((uint32_t)0x00380000U)        /*!<TSEL2[2:0] (DAC channel2 Trigger selection) */
#define  DAC_CR_TSEL2_0                      ((uint32_t)0x00080000U)        /*!<Bit 0 */
#define  DAC_CR_TSEL2_1                      ((uint32_t)0x00100000U)        /*!<Bit 1 */
#define  DAC_CR_TSEL2_2                      ((uint32_t)0x00200000U)        /*!<Bit 2 */

#define  DAC_CR_WAVE2                        ((uint32_t)0x00C00000U)        /*!<WAVE2[1:0] (DAC channel2 noise/triangle wave generation enable) */
#define  DAC_CR_WAVE2_0                      ((uint32_t)0x00400000U)        /*!<Bit 0 */
#define  DAC_CR_WAVE2_1                      ((uint32_t)0x00800000U)        /*!<Bit 1 */

#define  DAC_CR_MAMP2                        ((uint32_t)0x0F000000U)        /*!<MAMP2[3:0] (DAC channel2 Mask/Amplitude selector) */
#define  DAC_CR_MAMP2_0                      ((uint32_t)0x01000000U)        /*!<Bit 0 */
#define  DAC_CR_MAMP2_1                      ((uint32_t)0x02000000U)        /*!<Bit 1 */
#define  DAC_CR_MAMP2_2                      ((uint32_t)0x04000000U)        /*!<Bit 2 */
#define  DAC_CR_MAMP2_3                      ((uint32_t)0x08000000U)        /*!<Bit 3 */

#define  DAC_CR_DMAEN2                       ((uint32_t)0x10000000U)        /*!<DAC channel2 DMA enabled */
#define  DAC_CR_DMAUDRIE2                    ((uint32_t)0x20000000U)        /*!<DAC channel2 DMA underrun interrupt enable */
/*****************  Bit definition for DAC_SWTRIGR register  ******************/
#define  DAC_SWTRIGR_SWTRIG1                 ((uint32_t)0x00000001U)        /*!<DAC channel1 software trigger */
#define  DAC_SWTRIGR_SWTRIG2                 ((uint32_t)0x00000002U)        /*!<DAC channel2 software trigger */

/*****************  Bit definition for DAC_DHR12R1 register  ******************/
#define  DAC_DHR12R1_DACC1DHR                ((uint32_t)0x00000FFFU)        /*!<DAC channel1 12-bit Right aligned data */

/*****************  Bit definition for DAC_DHR12L1 register  ******************/
#define  DAC_DHR12L1_DACC1DHR                ((uint32_t)0x0000FFF0U)        /*!<DAC channel1 12-bit Left aligned data */

/******************  Bit definition for DAC_DHR8R1 register  ******************/
#define  DAC_DHR8R1_DACC1DHR                 ((uint32_t)0x000000FFU)        /*!<DAC channel1 8-bit Right aligned data */

/*****************  Bit definition for DAC_DHR12R2 register  ******************/
#define  DAC_DHR12R2_DACC2DHR                ((uint32_t)0x00000FFFU)        /*!<DAC channel2 12-bit Right aligned data */

/*****************  Bit definition for DAC_DHR12L2 register  ******************/
#define  DAC_DHR12L2_DACC2DHR                ((uint32_t)0x0000FFF0U)        /*!<DAC channel2 12-bit Left aligned data */

/******************  Bit definition for DAC_DHR8R2 register  ******************/
#define  DAC_DHR8R2_DACC2DHR                 ((uint32_t)0x000000FFU)        /*!<DAC channel2 8-bit Right aligned data */

/*****************  Bit definition for DAC_DHR12RD register  ******************/
#define  DAC_DHR12RD_DACC1DHR                ((uint32_t)0x00000FFFU)        /*!<DAC channel1 12-bit Right aligned data */
#define  DAC_DHR12RD_DACC2DHR                ((uint32_t)0x0FFF0000U)        /*!<DAC channel2 12-bit Right aligned data */

/*****************  Bit definition for DAC_DHR12LD register  ******************/
#define  DAC_DHR12LD_DACC1DHR                ((uint32_t)0x0000FFF0U)        /*!<DAC channel1 12-bit Left aligned data */
#define  DAC_DHR12LD_DACC2DHR                ((uint32_t)0xFFF00000U)        /*!<DAC channel2 12-bit Left aligned data */

/******************  Bit definition for DAC_DHR8RD register  ******************/
#define  DAC_DHR8RD_DACC1DHR                 ((uint32_t)0x000000FFU)        /*!<DAC channel1 8-bit Right aligned data */
#define  DAC_DHR8RD_DACC2DHR                 ((uint32_t)0x0000FF00U)        /*!<DAC channel2 8-bit Right aligned data */

/*******************  Bit definition for DAC_DOR1 register  *******************/
#define  DAC_DOR1_DACC1DOR                   ((uint32_t)0x00000FFFU)        /*!<DAC channel1 data output */

/*******************  Bit definition for DAC_DOR2 register  *******************/
#define  DAC_DOR2_DACC2DOR                   ((uint32_t)0x00000FFF)         /*!<DAC channel2 data output */

/********************  Bit definition for DAC_SR register  ********************/
#define  DAC_SR_DMAUDR1                      ((uint32_t)0x00002000U)        /*!<DAC channel1 DMA underrun flag */
#define  DAC_SR_DMAUDR2                      ((uint32_t)0x20000000U)        /*!<DAC channel2 DMA underrun flag */

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
#define  DBGMCU_CR_DBG_SLEEP                 ((uint32_t)0x00000001U)        /*!< Debug Sleep Mode */
#define  DBGMCU_CR_DBG_STOP                  ((uint32_t)0x00000002U)        /*!< Debug Stop Mode */
#define  DBGMCU_CR_DBG_STANDBY               ((uint32_t)0x00000004U)        /*!< Debug Standby mode */
#define  DBGMCU_CR_TRACE_IOEN                ((uint32_t)0x00000020U)        /*!< Trace Pin Assignment Control */

#define  DBGMCU_CR_TRACE_MODE                ((uint32_t)0x000000C0U)        /*!< TRACE_MODE[1:0] bits (Trace Pin Assignment Control) */
#define  DBGMCU_CR_TRACE_MODE_0              ((uint32_t)0x00000040U)        /*!< Bit 0 */
#define  DBGMCU_CR_TRACE_MODE_1              ((uint32_t)0x00000080U)        /*!< Bit 1 */

/******************  Bit definition for DBGMCU_APB1_FZ register  **************/

#define  DBGMCU_APB1_FZ_DBG_TIM2_STOP             ((uint32_t)0x00000001U)   /*!< TIM2 counter stopped when core is halted */
#define  DBGMCU_APB1_FZ_DBG_TIM3_STOP             ((uint32_t)0x00000002U)   /*!< TIM3 counter stopped when core is halted */
#define  DBGMCU_APB1_FZ_DBG_TIM4_STOP             ((uint32_t)0x00000004U)   /*!< TIM4 counter stopped when core is halted */
#define  DBGMCU_APB1_FZ_DBG_TIM5_STOP             ((uint32_t)0x00000008U)   /*!< TIM5 counter stopped when core is halted */
#define  DBGMCU_APB1_FZ_DBG_TIM6_STOP             ((uint32_t)0x00000010U)   /*!< TIM6 counter stopped when core is halted */
#define  DBGMCU_APB1_FZ_DBG_TIM7_STOP             ((uint32_t)0x00000020U)   /*!< TIM7 counter stopped when core is halted */
#define  DBGMCU_APB1_FZ_DBG_RTC_STOP              ((uint32_t)0x00000400U)   /*!< RTC Counter stopped when Core is halted */
#define  DBGMCU_APB1_FZ_DBG_WWDG_STOP             ((uint32_t)0x00000800U)   /*!< Debug Window Watchdog stopped when Core is halted */
#define  DBGMCU_APB1_FZ_DBG_IWDG_STOP             ((uint32_t)0x00001000U)   /*!< Debug Independent Watchdog stopped when Core is halted */
#define  DBGMCU_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT    ((uint32_t)0x00200000U)   /*!< SMBUS timeout mode stopped when Core is halted */
#define  DBGMCU_APB1_FZ_DBG_I2C2_SMBUS_TIMEOUT    ((uint32_t)0x00400000U)   /*!< SMBUS timeout mode stopped when Core is halted */

/******************  Bit definition for DBGMCU_APB2_FZ register  **************/

#define  DBGMCU_APB2_FZ_DBG_TIM9_STOP             ((uint32_t)0x00000004U)   /*!< TIM9 counter stopped when core is halted */
#define  DBGMCU_APB2_FZ_DBG_TIM10_STOP            ((uint32_t)0x00000008U)   /*!< TIM10 counter stopped when core is halted */
#define  DBGMCU_APB2_FZ_DBG_TIM11_STOP            ((uint32_t)0x00000010U)   /*!< TIM11 counter stopped when core is halted */

/******************************************************************************/
/*                                                                            */
/*                           DMA Controller (DMA)                             */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for DMA_ISR register  ********************/
#define  DMA_ISR_GIF1                        ((uint32_t)0x00000001U)        /*!< Channel 1 Global interrupt flag */
#define  DMA_ISR_TCIF1                       ((uint32_t)0x00000002U)        /*!< Channel 1 Transfer Complete flag */
#define  DMA_ISR_HTIF1                       ((uint32_t)0x00000004U)        /*!< Channel 1 Half Transfer flag */
#define  DMA_ISR_TEIF1                       ((uint32_t)0x00000008U)        /*!< Channel 1 Transfer Error flag */
#define  DMA_ISR_GIF2                        ((uint32_t)0x00000010U)        /*!< Channel 2 Global interrupt flag */
#define  DMA_ISR_TCIF2                       ((uint32_t)0x00000020U)        /*!< Channel 2 Transfer Complete flag */
#define  DMA_ISR_HTIF2                       ((uint32_t)0x00000040U)        /*!< Channel 2 Half Transfer flag */
#define  DMA_ISR_TEIF2                       ((uint32_t)0x00000080U)        /*!< Channel 2 Transfer Error flag */
#define  DMA_ISR_GIF3                        ((uint32_t)0x00000100U)        /*!< Channel 3 Global interrupt flag */
#define  DMA_ISR_TCIF3                       ((uint32_t)0x00000200U)        /*!< Channel 3 Transfer Complete flag */
#define  DMA_ISR_HTIF3                       ((uint32_t)0x00000400U)        /*!< Channel 3 Half Transfer flag */
#define  DMA_ISR_TEIF3                       ((uint32_t)0x00000800U)        /*!< Channel 3 Transfer Error flag */
#define  DMA_ISR_GIF4                        ((uint32_t)0x00001000U)        /*!< Channel 4 Global interrupt flag */
#define  DMA_ISR_TCIF4                       ((uint32_t)0x00002000U)        /*!< Channel 4 Transfer Complete flag */
#define  DMA_ISR_HTIF4                       ((uint32_t)0x00004000U)        /*!< Channel 4 Half Transfer flag */
#define  DMA_ISR_TEIF4                       ((uint32_t)0x00008000U)        /*!< Channel 4 Transfer Error flag */
#define  DMA_ISR_GIF5                        ((uint32_t)0x00010000U)        /*!< Channel 5 Global interrupt flag */
#define  DMA_ISR_TCIF5                       ((uint32_t)0x00020000U)        /*!< Channel 5 Transfer Complete flag */
#define  DMA_ISR_HTIF5                       ((uint32_t)0x00040000U)        /*!< Channel 5 Half Transfer flag */
#define  DMA_ISR_TEIF5                       ((uint32_t)0x00080000U)        /*!< Channel 5 Transfer Error flag */
#define  DMA_ISR_GIF6                        ((uint32_t)0x00100000U)        /*!< Channel 6 Global interrupt flag */
#define  DMA_ISR_TCIF6                       ((uint32_t)0x00200000U)        /*!< Channel 6 Transfer Complete flag */
#define  DMA_ISR_HTIF6                       ((uint32_t)0x00400000U)        /*!< Channel 6 Half Transfer flag */
#define  DMA_ISR_TEIF6                       ((uint32_t)0x00800000U)        /*!< Channel 6 Transfer Error flag */
#define  DMA_ISR_GIF7                        ((uint32_t)0x01000000U)        /*!< Channel 7 Global interrupt flag */
#define  DMA_ISR_TCIF7                       ((uint32_t)0x02000000U)        /*!< Channel 7 Transfer Complete flag */
#define  DMA_ISR_HTIF7                       ((uint32_t)0x04000000U)        /*!< Channel 7 Half Transfer flag */
#define  DMA_ISR_TEIF7                       ((uint32_t)0x08000000U)        /*!< Channel 7 Transfer Error flag */

/*******************  Bit definition for DMA_IFCR register  *******************/
#define  DMA_IFCR_CGIF1                      ((uint32_t)0x00000001U)        /*!< Channel 1 Global interrupt clear */
#define  DMA_IFCR_CTCIF1                     ((uint32_t)0x00000002U)        /*!< Channel 1 Transfer Complete clear */
#define  DMA_IFCR_CHTIF1                     ((uint32_t)0x00000004U)        /*!< Channel 1 Half Transfer clear */
#define  DMA_IFCR_CTEIF1                     ((uint32_t)0x00000008U)        /*!< Channel 1 Transfer Error clear */
#define  DMA_IFCR_CGIF2                      ((uint32_t)0x00000010U)        /*!< Channel 2 Global interrupt clear */
#define  DMA_IFCR_CTCIF2                     ((uint32_t)0x00000020U)        /*!< Channel 2 Transfer Complete clear */
#define  DMA_IFCR_CHTIF2                     ((uint32_t)0x00000040U)        /*!< Channel 2 Half Transfer clear */
#define  DMA_IFCR_CTEIF2                     ((uint32_t)0x00000080U)        /*!< Channel 2 Transfer Error clear */
#define  DMA_IFCR_CGIF3                      ((uint32_t)0x00000100U)        /*!< Channel 3 Global interrupt clear */
#define  DMA_IFCR_CTCIF3                     ((uint32_t)0x00000200U)        /*!< Channel 3 Transfer Complete clear */
#define  DMA_IFCR_CHTIF3                     ((uint32_t)0x00000400U)        /*!< Channel 3 Half Transfer clear */
#define  DMA_IFCR_CTEIF3                     ((uint32_t)0x00000800U)        /*!< Channel 3 Transfer Error clear */
#define  DMA_IFCR_CGIF4                      ((uint32_t)0x00001000U)        /*!< Channel 4 Global interrupt clear */
#define  DMA_IFCR_CTCIF4                     ((uint32_t)0x00002000U)        /*!< Channel 4 Transfer Complete clear */
#define  DMA_IFCR_CHTIF4                     ((uint32_t)0x00004000U)        /*!< Channel 4 Half Transfer clear */
#define  DMA_IFCR_CTEIF4                     ((uint32_t)0x00008000U)        /*!< Channel 4 Transfer Error clear */
#define  DMA_IFCR_CGIF5                      ((uint32_t)0x00010000U)        /*!< Channel 5 Global interrupt clear */
#define  DMA_IFCR_CTCIF5                     ((uint32_t)0x00020000U)        /*!< Channel 5 Transfer Complete clear */
#define  DMA_IFCR_CHTIF5                     ((uint32_t)0x00040000U)        /*!< Channel 5 Half Transfer clear */
#define  DMA_IFCR_CTEIF5                     ((uint32_t)0x00080000U)        /*!< Channel 5 Transfer Error clear */
#define  DMA_IFCR_CGIF6                      ((uint32_t)0x00100000U)        /*!< Channel 6 Global interrupt clear */
#define  DMA_IFCR_CTCIF6                     ((uint32_t)0x00200000U)        /*!< Channel 6 Transfer Complete clear */
#define  DMA_IFCR_CHTIF6                     ((uint32_t)0x00400000U)        /*!< Channel 6 Half Transfer clear */
#define  DMA_IFCR_CTEIF6                     ((uint32_t)0x00800000U)        /*!< Channel 6 Transfer Error clear */
#define  DMA_IFCR_CGIF7                      ((uint32_t)0x01000000U)        /*!< Channel 7 Global interrupt clear */
#define  DMA_IFCR_CTCIF7                     ((uint32_t)0x02000000U)        /*!< Channel 7 Transfer Complete clear */
#define  DMA_IFCR_CHTIF7                     ((uint32_t)0x04000000U)        /*!< Channel 7 Half Transfer clear */
#define  DMA_IFCR_CTEIF7                     ((uint32_t)0x08000000U)        /*!< Channel 7 Transfer Error clear */

/*******************  Bit definition for DMA_CCR register  *******************/
#define  DMA_CCR_EN                         ((uint32_t)0x00000001U)            /*!< Channel enable*/
#define  DMA_CCR_TCIE                       ((uint32_t)0x00000002U)            /*!< Transfer complete interrupt enable */
#define  DMA_CCR_HTIE                       ((uint32_t)0x00000004U)            /*!< Half Transfer interrupt enable */
#define  DMA_CCR_TEIE                       ((uint32_t)0x00000008U)            /*!< Transfer error interrupt enable */
#define  DMA_CCR_DIR                        ((uint32_t)0x00000010U)            /*!< Data transfer direction */
#define  DMA_CCR_CIRC                       ((uint32_t)0x00000020U)            /*!< Circular mode */
#define  DMA_CCR_PINC                       ((uint32_t)0x00000040U)            /*!< Peripheral increment mode */
#define  DMA_CCR_MINC                       ((uint32_t)0x00000080U)            /*!< Memory increment mode */

#define  DMA_CCR_PSIZE                      ((uint32_t)0x00000300U)            /*!< PSIZE[1:0] bits (Peripheral size) */
#define  DMA_CCR_PSIZE_0                    ((uint32_t)0x00000100U)            /*!< Bit 0 */
#define  DMA_CCR_PSIZE_1                    ((uint32_t)0x00000200U)            /*!< Bit 1 */

#define  DMA_CCR_MSIZE                      ((uint32_t)0x00000C00U)            /*!< MSIZE[1:0] bits (Memory size) */
#define  DMA_CCR_MSIZE_0                    ((uint32_t)0x00000400U)            /*!< Bit 0 */
#define  DMA_CCR_MSIZE_1                    ((uint32_t)0x00000800U)            /*!< Bit 1 */

#define  DMA_CCR_PL                         ((uint32_t)0x00003000U)            /*!< PL[1:0] bits(Channel Priority level) */
#define  DMA_CCR_PL_0                       ((uint32_t)0x00001000U)            /*!< Bit 0 */
#define  DMA_CCR_PL_1                       ((uint32_t)0x00002000U)            /*!< Bit 1 */

#define  DMA_CCR_MEM2MEM                    ((uint32_t)0x00004000U)            /*!< Memory to memory mode */

/******************  Bit definition generic for DMA_CNDTR register  *******************/
#define  DMA_CNDTR_NDT                       ((uint32_t)0x0000FFFFU)            /*!< Number of data to Transfer */

/******************  Bit definition for DMA_CNDTR1 register  ******************/
#define  DMA_CNDTR1_NDT                      ((uint32_t)0x0000FFFFU)            /*!< Number of data to Transfer */

/******************  Bit definition for DMA_CNDTR2 register  ******************/
#define  DMA_CNDTR2_NDT                      ((uint32_t)0x0000FFFFU)            /*!< Number of data to Transfer */

/******************  Bit definition for DMA_CNDTR3 register  ******************/
#define  DMA_CNDTR3_NDT                      ((uint32_t)0x0000FFFFU)            /*!< Number of data to Transfer */

/******************  Bit definition for DMA_CNDTR4 register  ******************/
#define  DMA_CNDTR4_NDT                      ((uint32_t)0x0000FFFFU)            /*!< Number of data to Transfer */

/******************  Bit definition for DMA_CNDTR5 register  ******************/
#define  DMA_CNDTR5_NDT                      ((uint32_t)0x0000FFFFU)            /*!< Number of data to Transfer */

/******************  Bit definition for DMA_CNDTR6 register  ******************/
#define  DMA_CNDTR6_NDT                      ((uint32_t)0x0000FFFFU)            /*!< Number of data to Transfer */

/******************  Bit definition for DMA_CNDTR7 register  ******************/
#define  DMA_CNDTR7_NDT                      ((uint32_t)0x0000FFFFU)            /*!< Number of data to Transfer */

/******************  Bit definition generic for DMA_CPAR register  ********************/
#define  DMA_CPAR_PA                         ((uint32_t)0xFFFFFFFFU)           /*!< Peripheral Address */

/******************  Bit definition for DMA_CPAR1 register  *******************/
#define  DMA_CPAR1_PA                        ((uint32_t)0xFFFFFFFFU)            /*!< Peripheral Address */

/******************  Bit definition for DMA_CPAR2 register  *******************/
#define  DMA_CPAR2_PA                        ((uint32_t)0xFFFFFFFFU)            /*!< Peripheral Address */

/******************  Bit definition for DMA_CPAR3 register  *******************/
#define  DMA_CPAR3_PA                        ((uint32_t)0xFFFFFFFFU)        /*!< Peripheral Address */


/******************  Bit definition for DMA_CPAR4 register  *******************/
#define  DMA_CPAR4_PA                        ((uint32_t)0xFFFFFFFFU)        /*!< Peripheral Address */

/******************  Bit definition for DMA_CPAR5 register  *******************/
#define  DMA_CPAR5_PA                        ((uint32_t)0xFFFFFFFFU)        /*!< Peripheral Address */

/******************  Bit definition for DMA_CPAR6 register  *******************/
#define  DMA_CPAR6_PA                        ((uint32_t)0xFFFFFFFFU)        /*!< Peripheral Address */


/******************  Bit definition for DMA_CPAR7 register  *******************/
#define  DMA_CPAR7_PA                        ((uint32_t)0xFFFFFFFFU)        /*!< Peripheral Address */

/******************  Bit definition generic for DMA_CMAR register  ********************/
#define  DMA_CMAR_MA                         ((uint32_t)0xFFFFFFFFU)        /*!< Memory Address */

/******************  Bit definition for DMA_CMAR1 register  *******************/
#define  DMA_CMAR1_MA                        ((uint32_t)0xFFFFFFFFU)        /*!< Memory Address */

/******************  Bit definition for DMA_CMAR2 register  *******************/
#define  DMA_CMAR2_MA                        ((uint32_t)0xFFFFFFFFU)        /*!< Memory Address */

/******************  Bit definition for DMA_CMAR3 register  *******************/
#define  DMA_CMAR3_MA                        ((uint32_t)0xFFFFFFFFU)        /*!< Memory Address */


/******************  Bit definition for DMA_CMAR4 register  *******************/
#define  DMA_CMAR4_MA                        ((uint32_t)0xFFFFFFFFU)        /*!< Memory Address */

/******************  Bit definition for DMA_CMAR5 register  *******************/
#define  DMA_CMAR5_MA                        ((uint32_t)0xFFFFFFFFU)        /*!< Memory Address */

/******************  Bit definition for DMA_CMAR6 register  *******************/
#define  DMA_CMAR6_MA                        ((uint32_t)0xFFFFFFFFU)        /*!< Memory Address */

/******************  Bit definition for DMA_CMAR7 register  *******************/
#define  DMA_CMAR7_MA                        ((uint32_t)0xFFFFFFFFU)        /*!< Memory Address */

/******************************************************************************/
/*                                                                            */
/*                  External Interrupt/Event Controller (EXTI)                */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for EXTI_IMR register  *******************/
#define  EXTI_IMR_MR0                        ((uint32_t)0x00000001U)        /*!< Interrupt Mask on line 0 */
#define  EXTI_IMR_MR1                        ((uint32_t)0x00000002U)        /*!< Interrupt Mask on line 1 */
#define  EXTI_IMR_MR2                        ((uint32_t)0x00000004U)        /*!< Interrupt Mask on line 2 */
#define  EXTI_IMR_MR3                        ((uint32_t)0x00000008U)        /*!< Interrupt Mask on line 3 */
#define  EXTI_IMR_MR4                        ((uint32_t)0x00000010U)        /*!< Interrupt Mask on line 4 */
#define  EXTI_IMR_MR5                        ((uint32_t)0x00000020U)        /*!< Interrupt Mask on line 5 */
#define  EXTI_IMR_MR6                        ((uint32_t)0x00000040U)        /*!< Interrupt Mask on line 6 */
#define  EXTI_IMR_MR7                        ((uint32_t)0x00000080U)        /*!< Interrupt Mask on line 7 */
#define  EXTI_IMR_MR8                        ((uint32_t)0x00000100U)        /*!< Interrupt Mask on line 8 */
#define  EXTI_IMR_MR9                        ((uint32_t)0x00000200U)        /*!< Interrupt Mask on line 9 */
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
#define  EXTI_IMR_MR20                       ((uint32_t)0x00100000U)        /*!< Interrupt Mask on line 20 */
#define  EXTI_IMR_MR21                       ((uint32_t)0x00200000U)        /*!< Interrupt Mask on line 21 */
#define  EXTI_IMR_MR22                       ((uint32_t)0x00400000U)        /*!< Interrupt Mask on line 22 */
#define  EXTI_IMR_MR23                       ((uint32_t)0x00800000U)        /*!< Interrupt Mask on line 23 */

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
#define  EXTI_IMR_IM20 EXTI_IMR_MR20
#define  EXTI_IMR_IM21 EXTI_IMR_MR21
#define  EXTI_IMR_IM22 EXTI_IMR_MR22
/* Category 3, 4 & 5 */
#define  EXTI_IMR_IM23 EXTI_IMR_MR23
#define EXTI_IMR_IM ((uint32_t)0x00FFFFFFU) /*!< Interrupt Mask All */

/*******************  Bit definition for EXTI_EMR register  *******************/
#define  EXTI_EMR_MR0                        ((uint32_t)0x00000001U)        /*!< Event Mask on line 0 */
#define  EXTI_EMR_MR1                        ((uint32_t)0x00000002U)        /*!< Event Mask on line 1 */
#define  EXTI_EMR_MR2                        ((uint32_t)0x00000004U)        /*!< Event Mask on line 2 */
#define  EXTI_EMR_MR3                        ((uint32_t)0x00000008U)        /*!< Event Mask on line 3 */
#define  EXTI_EMR_MR4                        ((uint32_t)0x00000010U)        /*!< Event Mask on line 4 */
#define  EXTI_EMR_MR5                        ((uint32_t)0x00000020U)        /*!< Event Mask on line 5 */
#define  EXTI_EMR_MR6                        ((uint32_t)0x00000040U)        /*!< Event Mask on line 6 */
#define  EXTI_EMR_MR7                        ((uint32_t)0x00000080U)        /*!< Event Mask on line 7 */
#define  EXTI_EMR_MR8                        ((uint32_t)0x00000100U)        /*!< Event Mask on line 8 */
#define  EXTI_EMR_MR9                        ((uint32_t)0x00000200U)        /*!< Event Mask on line 9 */
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
#define  EXTI_EMR_MR20                       ((uint32_t)0x00100000U)        /*!< Event Mask on line 20 */
#define  EXTI_EMR_MR21                       ((uint32_t)0x00200000U)        /*!< Event Mask on line 21 */
#define  EXTI_EMR_MR22                       ((uint32_t)0x00400000U)        /*!< Event Mask on line 22 */
#define  EXTI_EMR_MR23                       ((uint32_t)0x00800000U)        /*!< Event Mask on line 23 */

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
#define  EXTI_EMR_EM20 EXTI_EMR_MR20
#define  EXTI_EMR_EM21 EXTI_EMR_MR21
#define  EXTI_EMR_EM22 EXTI_EMR_MR22
#define  EXTI_EMR_EM23 EXTI_EMR_MR23

/******************  Bit definition for EXTI_RTSR register  *******************/
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
#define  EXTI_RTSR_TR18                      ((uint32_t)0x00040000U)        /*!< Rising trigger event configuration bit of line 18 */
#define  EXTI_RTSR_TR19                      ((uint32_t)0x00080000U)        /*!< Rising trigger event configuration bit of line 19 */
#define  EXTI_RTSR_TR20                      ((uint32_t)0x00100000U)        /*!< Rising trigger event configuration bit of line 20 */
#define  EXTI_RTSR_TR21                      ((uint32_t)0x00200000U)        /*!< Rising trigger event configuration bit of line 21 */
#define  EXTI_RTSR_TR22                      ((uint32_t)0x00400000U)        /*!< Rising trigger event configuration bit of line 22 */
#define  EXTI_RTSR_TR23                      ((uint32_t)0x00800000U)        /*!< Rising trigger event configuration bit of line 23 */

/* References Defines */
#define  EXTI_RTSR_RT0 EXTI_RTSR_TR0
#define  EXTI_RTSR_RT1 EXTI_RTSR_TR1
#define  EXTI_RTSR_RT2 EXTI_RTSR_TR2
#define  EXTI_RTSR_RT3 EXTI_RTSR_TR3
#define  EXTI_RTSR_RT4 EXTI_RTSR_TR4
#define  EXTI_RTSR_RT5 EXTI_RTSR_TR5
#define  EXTI_RTSR_RT6 EXTI_RTSR_TR6
#define  EXTI_RTSR_RT7 EXTI_RTSR_TR7
#define  EXTI_RTSR_RT8 EXTI_RTSR_TR8
#define  EXTI_RTSR_RT9 EXTI_RTSR_TR9
#define  EXTI_RTSR_RT10 EXTI_RTSR_TR10
#define  EXTI_RTSR_RT11 EXTI_RTSR_TR11
#define  EXTI_RTSR_RT12 EXTI_RTSR_TR12
#define  EXTI_RTSR_RT13 EXTI_RTSR_TR13
#define  EXTI_RTSR_RT14 EXTI_RTSR_TR14
#define  EXTI_RTSR_RT15 EXTI_RTSR_TR15
#define  EXTI_RTSR_RT16 EXTI_RTSR_TR16
#define  EXTI_RTSR_RT17 EXTI_RTSR_TR17
#define  EXTI_RTSR_RT18 EXTI_RTSR_TR18
#define  EXTI_RTSR_RT19 EXTI_RTSR_TR19
#define  EXTI_RTSR_RT20 EXTI_RTSR_TR20
#define  EXTI_RTSR_RT21 EXTI_RTSR_TR21
#define  EXTI_RTSR_RT22 EXTI_RTSR_TR22
#define  EXTI_RTSR_RT23 EXTI_RTSR_TR23

/******************  Bit definition for EXTI_FTSR register  *******************/
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
#define  EXTI_FTSR_TR18                      ((uint32_t)0x00040000U)        /*!< Falling trigger event configuration bit of line 18 */
#define  EXTI_FTSR_TR19                      ((uint32_t)0x00080000U)        /*!< Falling trigger event configuration bit of line 19 */
#define  EXTI_FTSR_TR20                      ((uint32_t)0x00100000U)        /*!< Falling trigger event configuration bit of line 20 */
#define  EXTI_FTSR_TR21                      ((uint32_t)0x00200000U)        /*!< Falling trigger event configuration bit of line 21 */
#define  EXTI_FTSR_TR22                      ((uint32_t)0x00400000U)        /*!< Falling trigger event configuration bit of line 22 */
#define  EXTI_FTSR_TR23                      ((uint32_t)0x00800000U)        /*!< Falling trigger event configuration bit of line 23 */

/* References Defines */
#define  EXTI_FTSR_FT0 EXTI_FTSR_TR0
#define  EXTI_FTSR_FT1 EXTI_FTSR_TR1
#define  EXTI_FTSR_FT2 EXTI_FTSR_TR2
#define  EXTI_FTSR_FT3 EXTI_FTSR_TR3
#define  EXTI_FTSR_FT4 EXTI_FTSR_TR4
#define  EXTI_FTSR_FT5 EXTI_FTSR_TR5
#define  EXTI_FTSR_FT6 EXTI_FTSR_TR6
#define  EXTI_FTSR_FT7 EXTI_FTSR_TR7
#define  EXTI_FTSR_FT8 EXTI_FTSR_TR8
#define  EXTI_FTSR_FT9 EXTI_FTSR_TR9
#define  EXTI_FTSR_FT10 EXTI_FTSR_TR10
#define  EXTI_FTSR_FT11 EXTI_FTSR_TR11
#define  EXTI_FTSR_FT12 EXTI_FTSR_TR12
#define  EXTI_FTSR_FT13 EXTI_FTSR_TR13
#define  EXTI_FTSR_FT14 EXTI_FTSR_TR14
#define  EXTI_FTSR_FT15 EXTI_FTSR_TR15
#define  EXTI_FTSR_FT16 EXTI_FTSR_TR16
#define  EXTI_FTSR_FT17 EXTI_FTSR_TR17
#define  EXTI_FTSR_FT18 EXTI_FTSR_TR18
#define  EXTI_FTSR_FT19 EXTI_FTSR_TR19
#define  EXTI_FTSR_FT20 EXTI_FTSR_TR20
#define  EXTI_FTSR_FT21 EXTI_FTSR_TR21
#define  EXTI_FTSR_FT22 EXTI_FTSR_TR22
#define  EXTI_FTSR_FT23 EXTI_FTSR_TR23

/******************  Bit definition for EXTI_SWIER register  ******************/
#define  EXTI_SWIER_SWIER0                   ((uint32_t)0x00000001U)        /*!< Software Interrupt on line 0 */
#define  EXTI_SWIER_SWIER1                   ((uint32_t)0x00000002U)        /*!< Software Interrupt on line 1 */
#define  EXTI_SWIER_SWIER2                   ((uint32_t)0x00000004U)        /*!< Software Interrupt on line 2 */
#define  EXTI_SWIER_SWIER3                   ((uint32_t)0x00000008U)        /*!< Software Interrupt on line 3 */
#define  EXTI_SWIER_SWIER4                   ((uint32_t)0x00000010U)        /*!< Software Interrupt on line 4 */
#define  EXTI_SWIER_SWIER5                   ((uint32_t)0x00000020U)        /*!< Software Interrupt on line 5 */
#define  EXTI_SWIER_SWIER6                   ((uint32_t)0x00000040U)        /*!< Software Interrupt on line 6 */
#define  EXTI_SWIER_SWIER7                   ((uint32_t)0x00000080U)        /*!< Software Interrupt on line 7 */
#define  EXTI_SWIER_SWIER8                   ((uint32_t)0x00000100U)        /*!< Software Interrupt on line 8 */
#define  EXTI_SWIER_SWIER9                   ((uint32_t)0x00000200U)        /*!< Software Interrupt on line 9 */
#define  EXTI_SWIER_SWIER10                  ((uint32_t)0x00000400U)        /*!< Software Interrupt on line 10 */
#define  EXTI_SWIER_SWIER11                  ((uint32_t)0x00000800U)        /*!< Software Interrupt on line 11 */
#define  EXTI_SWIER_SWIER12                  ((uint32_t)0x00001000U)        /*!< Software Interrupt on line 12 */
#define  EXTI_SWIER_SWIER13                  ((uint32_t)0x00002000U)        /*!< Software Interrupt on line 13 */
#define  EXTI_SWIER_SWIER14                  ((uint32_t)0x00004000U)        /*!< Software Interrupt on line 14 */
#define  EXTI_SWIER_SWIER15                  ((uint32_t)0x00008000U)        /*!< Software Interrupt on line 15 */
#define  EXTI_SWIER_SWIER16                  ((uint32_t)0x00010000U)        /*!< Software Interrupt on line 16 */
#define  EXTI_SWIER_SWIER17                  ((uint32_t)0x00020000U)        /*!< Software Interrupt on line 17 */
#define  EXTI_SWIER_SWIER18                  ((uint32_t)0x00040000U)        /*!< Software Interrupt on line 18 */
#define  EXTI_SWIER_SWIER19                  ((uint32_t)0x00080000U)        /*!< Software Interrupt on line 19 */
#define  EXTI_SWIER_SWIER20                  ((uint32_t)0x00100000U)        /*!< Software Interrupt on line 20 */
#define  EXTI_SWIER_SWIER21                  ((uint32_t)0x00200000U)        /*!< Software Interrupt on line 21 */
#define  EXTI_SWIER_SWIER22                  ((uint32_t)0x00400000U)        /*!< Software Interrupt on line 22 */
#define  EXTI_SWIER_SWIER23                  ((uint32_t)0x00800000U)        /*!< Software Interrupt on line 23 */

/* References Defines */
#define  EXTI_SWIER_SWI0 EXTI_SWIER_SWIER0
#define  EXTI_SWIER_SWI1 EXTI_SWIER_SWIER1
#define  EXTI_SWIER_SWI2 EXTI_SWIER_SWIER2
#define  EXTI_SWIER_SWI3 EXTI_SWIER_SWIER3
#define  EXTI_SWIER_SWI4 EXTI_SWIER_SWIER4
#define  EXTI_SWIER_SWI5 EXTI_SWIER_SWIER5
#define  EXTI_SWIER_SWI6 EXTI_SWIER_SWIER6
#define  EXTI_SWIER_SWI7 EXTI_SWIER_SWIER7
#define  EXTI_SWIER_SWI8 EXTI_SWIER_SWIER8
#define  EXTI_SWIER_SWI9 EXTI_SWIER_SWIER9
#define  EXTI_SWIER_SWI10 EXTI_SWIER_SWIER10
#define  EXTI_SWIER_SWI11 EXTI_SWIER_SWIER11
#define  EXTI_SWIER_SWI12 EXTI_SWIER_SWIER12
#define  EXTI_SWIER_SWI13 EXTI_SWIER_SWIER13
#define  EXTI_SWIER_SWI14 EXTI_SWIER_SWIER14
#define  EXTI_SWIER_SWI15 EXTI_SWIER_SWIER15
#define  EXTI_SWIER_SWI16 EXTI_SWIER_SWIER16
#define  EXTI_SWIER_SWI17 EXTI_SWIER_SWIER17
#define  EXTI_SWIER_SWI18 EXTI_SWIER_SWIER18
#define  EXTI_SWIER_SWI19 EXTI_SWIER_SWIER19
#define  EXTI_SWIER_SWI20 EXTI_SWIER_SWIER20
#define  EXTI_SWIER_SWI21 EXTI_SWIER_SWIER21
#define  EXTI_SWIER_SWI22 EXTI_SWIER_SWIER22
#define  EXTI_SWIER_SWI23 EXTI_SWIER_SWIER23

/*******************  Bit definition for EXTI_PR register  ********************/
#define  EXTI_PR_PR0                         ((uint32_t)0x00000001U)        /*!< Pending bit for line 0 */
#define  EXTI_PR_PR1                         ((uint32_t)0x00000002U)        /*!< Pending bit for line 1 */
#define  EXTI_PR_PR2                         ((uint32_t)0x00000004U)        /*!< Pending bit for line 2 */
#define  EXTI_PR_PR3                         ((uint32_t)0x00000008U)        /*!< Pending bit for line 3 */
#define  EXTI_PR_PR4                         ((uint32_t)0x00000010U)        /*!< Pending bit for line 4 */
#define  EXTI_PR_PR5                         ((uint32_t)0x00000020U)        /*!< Pending bit for line 5 */
#define  EXTI_PR_PR6                         ((uint32_t)0x00000040U)        /*!< Pending bit for line 6 */
#define  EXTI_PR_PR7                         ((uint32_t)0x00000080U)        /*!< Pending bit for line 7 */
#define  EXTI_PR_PR8                         ((uint32_t)0x00000100U)        /*!< Pending bit for line 8 */
#define  EXTI_PR_PR9                         ((uint32_t)0x00000200U)        /*!< Pending bit for line 9 */
#define  EXTI_PR_PR10                        ((uint32_t)0x00000400U)        /*!< Pending bit for line 10 */
#define  EXTI_PR_PR11                        ((uint32_t)0x00000800U)        /*!< Pending bit for line 11 */
#define  EXTI_PR_PR12                        ((uint32_t)0x00001000U)        /*!< Pending bit for line 12 */
#define  EXTI_PR_PR13                        ((uint32_t)0x00002000U)        /*!< Pending bit for line 13 */
#define  EXTI_PR_PR14                        ((uint32_t)0x00004000U)        /*!< Pending bit for line 14 */
#define  EXTI_PR_PR15                        ((uint32_t)0x00008000U)        /*!< Pending bit for line 15 */
#define  EXTI_PR_PR16                        ((uint32_t)0x00010000U)        /*!< Pending bit for line 16 */
#define  EXTI_PR_PR17                        ((uint32_t)0x00020000U)        /*!< Pending bit for line 17 */
#define  EXTI_PR_PR18                        ((uint32_t)0x00040000U)        /*!< Pending bit for line 18 */
#define  EXTI_PR_PR19                        ((uint32_t)0x00080000U)        /*!< Pending bit for line 19 */
#define  EXTI_PR_PR20                        ((uint32_t)0x00100000U)        /*!< Pending bit for line 20 */
#define  EXTI_PR_PR21                        ((uint32_t)0x00200000U)        /*!< Pending bit for line 21 */
#define  EXTI_PR_PR22                        ((uint32_t)0x00400000U)        /*!< Pending bit for line 22 */
#define  EXTI_PR_PR23                        ((uint32_t)0x00800000U)        /*!< Pending bit for line 23 */

/* References Defines */
#define  EXTI_PR_PIF0 EXTI_PR_PR0
#define  EXTI_PR_PIF1 EXTI_PR_PR1
#define  EXTI_PR_PIF2 EXTI_PR_PR2
#define  EXTI_PR_PIF3 EXTI_PR_PR3
#define  EXTI_PR_PIF4 EXTI_PR_PR4
#define  EXTI_PR_PIF5 EXTI_PR_PR5
#define  EXTI_PR_PIF6 EXTI_PR_PR6
#define  EXTI_PR_PIF7 EXTI_PR_PR7
#define  EXTI_PR_PIF8 EXTI_PR_PR8
#define  EXTI_PR_PIF9 EXTI_PR_PR9
#define  EXTI_PR_PIF10 EXTI_PR_PR10
#define  EXTI_PR_PIF11 EXTI_PR_PR11
#define  EXTI_PR_PIF12 EXTI_PR_PR12
#define  EXTI_PR_PIF13 EXTI_PR_PR13
#define  EXTI_PR_PIF14 EXTI_PR_PR14
#define  EXTI_PR_PIF15 EXTI_PR_PR15
#define  EXTI_PR_PIF16 EXTI_PR_PR16
#define  EXTI_PR_PIF17 EXTI_PR_PR17
#define  EXTI_PR_PIF18 EXTI_PR_PR18
#define  EXTI_PR_PIF19 EXTI_PR_PR19
#define  EXTI_PR_PIF20 EXTI_PR_PR20
#define  EXTI_PR_PIF21 EXTI_PR_PR21
#define  EXTI_PR_PIF22 EXTI_PR_PR22
#define  EXTI_PR_PIF23 EXTI_PR_PR23

/******************************************************************************/
/*                                                                            */
/*                FLASH, DATA EEPROM and Option Bytes Registers               */
/*                        (FLASH, DATA_EEPROM, OB)                            */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for FLASH_ACR register  ******************/
#define  FLASH_ACR_LATENCY                   ((uint32_t)0x00000001U)        /*!< Latency */
#define  FLASH_ACR_PRFTEN                    ((uint32_t)0x00000002U)        /*!< Prefetch Buffer Enable */
#define  FLASH_ACR_ACC64                     ((uint32_t)0x00000004U)        /*!< Access 64 bits */
#define  FLASH_ACR_SLEEP_PD                  ((uint32_t)0x00000008U)        /*!< Flash mode during sleep mode */
#define  FLASH_ACR_RUN_PD                    ((uint32_t)0x00000010U)        /*!< Flash mode during RUN mode */

/*******************  Bit definition for FLASH_PECR register  ******************/
#define FLASH_PECR_PELOCK                    ((uint32_t)0x00000001U)        /*!< FLASH_PECR and Flash data Lock */
#define FLASH_PECR_PRGLOCK                   ((uint32_t)0x00000002U)        /*!< Program matrix Lock */
#define FLASH_PECR_OPTLOCK                   ((uint32_t)0x00000004U)        /*!< Option byte matrix Lock */
#define FLASH_PECR_PROG                      ((uint32_t)0x00000008U)        /*!< Program matrix selection */
#define FLASH_PECR_DATA                      ((uint32_t)0x00000010U)        /*!< Data matrix selection */
#define FLASH_PECR_FTDW                      ((uint32_t)0x00000100U)        /*!< Fixed Time Data write for Word/Half Word/Byte programming */
#define FLASH_PECR_ERASE                     ((uint32_t)0x00000200U)        /*!< Page erasing mode */
#define FLASH_PECR_FPRG                      ((uint32_t)0x00000400U)        /*!< Fast Page/Half Page programming mode */
#define FLASH_PECR_PARALLBANK                ((uint32_t)0x00008000U)        /*!< Parallel Bank mode */
#define FLASH_PECR_EOPIE                     ((uint32_t)0x00010000U)        /*!< End of programming interrupt */ 
#define FLASH_PECR_ERRIE                     ((uint32_t)0x00020000U)        /*!< Error interrupt */ 
#define FLASH_PECR_OBL_LAUNCH                ((uint32_t)0x00040000U)        /*!< Launch the option byte loading */ 

/******************  Bit definition for FLASH_PDKEYR register  ******************/
#define  FLASH_PDKEYR_PDKEYR                 ((uint32_t)0xFFFFFFFFU)       /*!< FLASH_PEC and data matrix Key */

/******************  Bit definition for FLASH_PEKEYR register  ******************/
#define  FLASH_PEKEYR_PEKEYR                 ((uint32_t)0xFFFFFFFFU)       /*!< FLASH_PEC and data matrix Key */

/******************  Bit definition for FLASH_PRGKEYR register  ******************/
#define  FLASH_PRGKEYR_PRGKEYR               ((uint32_t)0xFFFFFFFFU)        /*!< Program matrix Key */

/******************  Bit definition for FLASH_OPTKEYR register  ******************/
#define  FLASH_OPTKEYR_OPTKEYR               ((uint32_t)0xFFFFFFFFU)        /*!< Option bytes matrix Key */

/******************  Bit definition for FLASH_SR register  *******************/
#define  FLASH_SR_BSY                        ((uint32_t)0x00000001U)        /*!< Busy */
#define  FLASH_SR_EOP                        ((uint32_t)0x00000002U)        /*!< End Of Programming*/
#define  FLASH_SR_ENDHV                       ((uint32_t)0x00000004U)        /*!< End of high voltage */
#define  FLASH_SR_READY                      ((uint32_t)0x00000008U)        /*!< Flash ready after low power mode */

#define  FLASH_SR_WRPERR                     ((uint32_t)0x00000100U)        /*!< Write protected error */
#define  FLASH_SR_PGAERR                     ((uint32_t)0x00000200U)        /*!< Programming Alignment Error */
#define  FLASH_SR_SIZERR                     ((uint32_t)0x00000400U)        /*!< Size error */
#define  FLASH_SR_OPTVERR                    ((uint32_t)0x00000800U)        /*!< Option validity error */
#define  FLASH_SR_OPTVERRUSR                 ((uint32_t)0x00001000U)        /*!< Option User validity error */

/******************  Bit definition for FLASH_OBR register  *******************/
#define  FLASH_OBR_RDPRT                     ((uint32_t)0x000000FFU)        /*!< Read Protection */
#define  FLASH_OBR_BOR_LEV                   ((uint32_t)0x000F0000U)        /*!< BOR_LEV[3:0] Brown Out Reset Threshold Level*/
#define  FLASH_OBR_USER                      ((uint32_t)0x00F00000U)        /*!< User Option Bytes */
#define  FLASH_OBR_IWDG_SW                   ((uint32_t)0x00100000U)        /*!< IWDG_SW */
#define  FLASH_OBR_nRST_STOP                 ((uint32_t)0x00200000U)        /*!< nRST_STOP */
#define  FLASH_OBR_nRST_STDBY                ((uint32_t)0x00400000U)        /*!< nRST_STDBY */
#define  FLASH_OBR_nRST_BFB2                 ((uint32_t)0x00800000U)        /*!< BFB2 */

/******************  Bit definition for FLASH_WRPR register  ******************/
#define  FLASH_WRPR1_WRP                      ((uint32_t)0xFFFFFFFFU)        /*!< Write Protect sectors 0  to 31  */
#define  FLASH_WRPR2_WRP                      ((uint32_t)0xFFFFFFFFU)        /*!< Write Protect sectors 32 to 63  */             
#define  FLASH_WRPR3_WRP                      ((uint32_t)0xFFFFFFFFU)        /*!< Write Protect sectors 64 to 95  */ 
#define  FLASH_WRPR4_WRP                      ((uint32_t)0xFFFFFFFFU)        /*!< Write Protect sectors 96 to 127 */ 

/******************************************************************************/
/*                                                                            */
/*                            General Purpose I/O                             */
/*                                                                            */
/******************************************************************************/
/******************  Bits definition for GPIO_MODER register  *****************/
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

/******************  Bits definition for GPIO_OTYPER register  ****************/
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

/******************  Bits definition for GPIO_OSPEEDR register  ***************/
#define GPIO_OSPEEDER_OSPEEDR0     ((uint32_t)0x00000003U)
#define GPIO_OSPEEDER_OSPEEDR0_0   ((uint32_t)0x00000001U)
#define GPIO_OSPEEDER_OSPEEDR0_1   ((uint32_t)0x00000002U)

#define GPIO_OSPEEDER_OSPEEDR1     ((uint32_t)0x0000000CU)
#define GPIO_OSPEEDER_OSPEEDR1_0   ((uint32_t)0x00000004U)
#define GPIO_OSPEEDER_OSPEEDR1_1   ((uint32_t)0x00000008U)

#define GPIO_OSPEEDER_OSPEEDR2     ((uint32_t)0x00000030U)
#define GPIO_OSPEEDER_OSPEEDR2_0   ((uint32_t)0x00000010U)
#define GPIO_OSPEEDER_OSPEEDR2_1   ((uint32_t)0x00000020U)

#define GPIO_OSPEEDER_OSPEEDR3     ((uint32_t)0x000000C0U)
#define GPIO_OSPEEDER_OSPEEDR3_0   ((uint32_t)0x00000040U)
#define GPIO_OSPEEDER_OSPEEDR3_1   ((uint32_t)0x00000080U)

#define GPIO_OSPEEDER_OSPEEDR4     ((uint32_t)0x00000300U)
#define GPIO_OSPEEDER_OSPEEDR4_0   ((uint32_t)0x00000100U)
#define GPIO_OSPEEDER_OSPEEDR4_1   ((uint32_t)0x00000200U)

#define GPIO_OSPEEDER_OSPEEDR5     ((uint32_t)0x00000C00U)
#define GPIO_OSPEEDER_OSPEEDR5_0   ((uint32_t)0x00000400U)
#define GPIO_OSPEEDER_OSPEEDR5_1   ((uint32_t)0x00000800U)

#define GPIO_OSPEEDER_OSPEEDR6     ((uint32_t)0x00003000U)
#define GPIO_OSPEEDER_OSPEEDR6_0   ((uint32_t)0x00001000U)
#define GPIO_OSPEEDER_OSPEEDR6_1   ((uint32_t)0x00002000U)

#define GPIO_OSPEEDER_OSPEEDR7     ((uint32_t)0x0000C000U)
#define GPIO_OSPEEDER_OSPEEDR7_0   ((uint32_t)0x00004000U)
#define GPIO_OSPEEDER_OSPEEDR7_1   ((uint32_t)0x00008000U)

#define GPIO_OSPEEDER_OSPEEDR8     ((uint32_t)0x00030000U)
#define GPIO_OSPEEDER_OSPEEDR8_0   ((uint32_t)0x00010000U)
#define GPIO_OSPEEDER_OSPEEDR8_1   ((uint32_t)0x00020000U)

#define GPIO_OSPEEDER_OSPEEDR9     ((uint32_t)0x000C0000U)
#define GPIO_OSPEEDER_OSPEEDR9_0   ((uint32_t)0x00040000U)
#define GPIO_OSPEEDER_OSPEEDR9_1   ((uint32_t)0x00080000U)

#define GPIO_OSPEEDER_OSPEEDR10    ((uint32_t)0x00300000U)
#define GPIO_OSPEEDER_OSPEEDR10_0  ((uint32_t)0x00100000U)
#define GPIO_OSPEEDER_OSPEEDR10_1  ((uint32_t)0x00200000U)

#define GPIO_OSPEEDER_OSPEEDR11    ((uint32_t)0x00C00000U)
#define GPIO_OSPEEDER_OSPEEDR11_0  ((uint32_t)0x00400000U)
#define GPIO_OSPEEDER_OSPEEDR11_1  ((uint32_t)0x00800000U)

#define GPIO_OSPEEDER_OSPEEDR12    ((uint32_t)0x03000000U)
#define GPIO_OSPEEDER_OSPEEDR12_0  ((uint32_t)0x01000000U)
#define GPIO_OSPEEDER_OSPEEDR12_1  ((uint32_t)0x02000000U)

#define GPIO_OSPEEDER_OSPEEDR13    ((uint32_t)0x0C000000U)
#define GPIO_OSPEEDER_OSPEEDR13_0  ((uint32_t)0x04000000U)
#define GPIO_OSPEEDER_OSPEEDR13_1  ((uint32_t)0x08000000U)

#define GPIO_OSPEEDER_OSPEEDR14    ((uint32_t)0x30000000U)
#define GPIO_OSPEEDER_OSPEEDR14_0  ((uint32_t)0x10000000U)
#define GPIO_OSPEEDER_OSPEEDR14_1  ((uint32_t)0x20000000U)

#define GPIO_OSPEEDER_OSPEEDR15    ((uint32_t)0xC0000000U)
#define GPIO_OSPEEDER_OSPEEDR15_0  ((uint32_t)0x40000000U)
#define GPIO_OSPEEDER_OSPEEDR15_1  ((uint32_t)0x80000000U)

/******************  Bits definition for GPIO_PUPDR register  *****************/
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

/******************  Bits definition for GPIO_IDR register  *******************/
#define GPIO_IDR_IDR_0                       ((uint32_t)0x00000001U)
#define GPIO_IDR_IDR_1                       ((uint32_t)0x00000002U)
#define GPIO_IDR_IDR_2                       ((uint32_t)0x00000004U)
#define GPIO_IDR_IDR_3                       ((uint32_t)0x00000008U)
#define GPIO_IDR_IDR_4                       ((uint32_t)0x00000010U)
#define GPIO_IDR_IDR_5                       ((uint32_t)0x00000020U)
#define GPIO_IDR_IDR_6                       ((uint32_t)0x00000040U)
#define GPIO_IDR_IDR_7                       ((uint32_t)0x00000080U)
#define GPIO_IDR_IDR_8                       ((uint32_t)0x00000100U)
#define GPIO_IDR_IDR_9                       ((uint32_t)0x00000200U)
#define GPIO_IDR_IDR_10                      ((uint32_t)0x00000400U)
#define GPIO_IDR_IDR_11                      ((uint32_t)0x00000800U)
#define GPIO_IDR_IDR_12                      ((uint32_t)0x00001000U)
#define GPIO_IDR_IDR_13                      ((uint32_t)0x00002000U)
#define GPIO_IDR_IDR_14                      ((uint32_t)0x00004000U)
#define GPIO_IDR_IDR_15                      ((uint32_t)0x00008000U)

/******************  Bits definition for GPIO_ODR register  *******************/
#define GPIO_ODR_ODR_0                       ((uint32_t)0x00000001U)
#define GPIO_ODR_ODR_1                       ((uint32_t)0x00000002U)
#define GPIO_ODR_ODR_2                       ((uint32_t)0x00000004U)
#define GPIO_ODR_ODR_3                       ((uint32_t)0x00000008U)
#define GPIO_ODR_ODR_4                       ((uint32_t)0x00000010U)
#define GPIO_ODR_ODR_5                       ((uint32_t)0x00000020U)
#define GPIO_ODR_ODR_6                       ((uint32_t)0x00000040U)
#define GPIO_ODR_ODR_7                       ((uint32_t)0x00000080U)
#define GPIO_ODR_ODR_8                       ((uint32_t)0x00000100U)
#define GPIO_ODR_ODR_9                       ((uint32_t)0x00000200U)
#define GPIO_ODR_ODR_10                      ((uint32_t)0x00000400U)
#define GPIO_ODR_ODR_11                      ((uint32_t)0x00000800U)
#define GPIO_ODR_ODR_12                      ((uint32_t)0x00001000U)
#define GPIO_ODR_ODR_13                      ((uint32_t)0x00002000U)
#define GPIO_ODR_ODR_14                      ((uint32_t)0x00004000U)
#define GPIO_ODR_ODR_15                      ((uint32_t)0x00008000U)

/******************  Bits definition for GPIO_BSRR register  ******************/
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

/*******************  Bit definition for I2C_CR1 register  ********************/
#define  I2C_CR1_PE                          ((uint32_t)0x00000001U)            /*!< Peripheral Enable */
#define  I2C_CR1_SMBUS                       ((uint32_t)0x00000002U)            /*!< SMBus Mode */
#define  I2C_CR1_SMBTYPE                     ((uint32_t)0x00000008U)            /*!< SMBus Type */
#define  I2C_CR1_ENARP                       ((uint32_t)0x00000010U)            /*!< ARP Enable */
#define  I2C_CR1_ENPEC                       ((uint32_t)0x00000020U)            /*!< PEC Enable */
#define  I2C_CR1_ENGC                        ((uint32_t)0x00000040U)            /*!< General Call Enable */
#define  I2C_CR1_NOSTRETCH                   ((uint32_t)0x00000080U)            /*!< Clock Stretching Disable (Slave mode) */
#define  I2C_CR1_START                       ((uint32_t)0x00000100U)            /*!< Start Generation */
#define  I2C_CR1_STOP                        ((uint32_t)0x00000200U)            /*!< Stop Generation */
#define  I2C_CR1_ACK                         ((uint32_t)0x00000400U)            /*!< Acknowledge Enable */
#define  I2C_CR1_POS                         ((uint32_t)0x00000800U)            /*!< Acknowledge/PEC Position (for data reception) */
#define  I2C_CR1_PEC                         ((uint32_t)0x00001000U)            /*!< Packet Error Checking */
#define  I2C_CR1_ALERT                       ((uint32_t)0x00002000U)            /*!< SMBus Alert */
#define  I2C_CR1_SWRST                       ((uint32_t)0x00008000U)            /*!< Software Reset */

/*******************  Bit definition for I2C_CR2 register  ********************/
#define  I2C_CR2_FREQ                        ((uint32_t)0x0000003FU)            /*!< FREQ[5:0] bits (Peripheral Clock Frequency) */
#define  I2C_CR2_FREQ_0                      ((uint32_t)0x00000001U)            /*!< Bit 0 */
#define  I2C_CR2_FREQ_1                      ((uint32_t)0x00000002U)            /*!< Bit 1 */
#define  I2C_CR2_FREQ_2                      ((uint32_t)0x00000004U)            /*!< Bit 2 */
#define  I2C_CR2_FREQ_3                      ((uint32_t)0x00000008U)            /*!< Bit 3 */
#define  I2C_CR2_FREQ_4                      ((uint32_t)0x00000010U)            /*!< Bit 4 */
#define  I2C_CR2_FREQ_5                      ((uint32_t)0x00000020U)            /*!< Bit 5 */

#define  I2C_CR2_ITERREN                     ((uint32_t)0x00000100U)            /*!< Error Interrupt Enable */
#define  I2C_CR2_ITEVTEN                     ((uint32_t)0x00000200U)            /*!< Event Interrupt Enable */
#define  I2C_CR2_ITBUFEN                     ((uint32_t)0x00000400U)            /*!< Buffer Interrupt Enable */
#define  I2C_CR2_DMAEN                       ((uint32_t)0x00000800U)            /*!< DMA Requests Enable */
#define  I2C_CR2_LAST                        ((uint32_t)0x00001000U)            /*!< DMA Last Transfer */

/*******************  Bit definition for I2C_OAR1 register  *******************/
#define  I2C_OAR1_ADD1_7                     ((uint32_t)0x000000FEU)            /*!< Interface Address */
#define  I2C_OAR1_ADD8_9                     ((uint32_t)0x00000300U)            /*!< Interface Address */

#define  I2C_OAR1_ADD0                       ((uint32_t)0x00000001U)            /*!< Bit 0 */
#define  I2C_OAR1_ADD1                       ((uint32_t)0x00000002U)            /*!< Bit 1 */
#define  I2C_OAR1_ADD2                       ((uint32_t)0x00000004U)            /*!< Bit 2 */
#define  I2C_OAR1_ADD3                       ((uint32_t)0x00000008U)            /*!< Bit 3 */
#define  I2C_OAR1_ADD4                       ((uint32_t)0x00000010U)            /*!< Bit 4 */
#define  I2C_OAR1_ADD5                       ((uint32_t)0x00000020U)            /*!< Bit 5 */
#define  I2C_OAR1_ADD6                       ((uint32_t)0x00000040U)            /*!< Bit 6 */
#define  I2C_OAR1_ADD7                       ((uint32_t)0x00000080U)            /*!< Bit 7 */
#define  I2C_OAR1_ADD8                       ((uint32_t)0x00000100U)            /*!< Bit 8 */
#define  I2C_OAR1_ADD9                       ((uint32_t)0x00000200U)            /*!< Bit 9 */

#define  I2C_OAR1_ADDMODE                    ((uint32_t)0x00008000U)            /*!< Addressing Mode (Slave mode) */

/*******************  Bit definition for I2C_OAR2 register  *******************/
#define  I2C_OAR2_ENDUAL                     ((uint32_t)0x00000001U)               /*!< Dual addressing mode enable */
#define  I2C_OAR2_ADD2                       ((uint32_t)0x000000FEU)               /*!< Interface address */

/********************  Bit definition for I2C_DR register  ********************/
#define  I2C_DR_DR                           ((uint32_t)0x000000FFU)               /*!< 8-bit Data Register */

/*******************  Bit definition for I2C_SR1 register  ********************/
#define  I2C_SR1_SB                          ((uint32_t)0x00000001U)            /*!< Start Bit (Master mode) */
#define  I2C_SR1_ADDR                        ((uint32_t)0x00000002U)            /*!< Address sent (master mode)/matched (slave mode) */
#define  I2C_SR1_BTF                         ((uint32_t)0x00000004U)            /*!< Byte Transfer Finished */
#define  I2C_SR1_ADD10                       ((uint32_t)0x00000008U)            /*!< 10-bit header sent (Master mode) */
#define  I2C_SR1_STOPF                       ((uint32_t)0x00000010U)            /*!< Stop detection (Slave mode) */
#define  I2C_SR1_RXNE                        ((uint32_t)0x00000040U)            /*!< Data Register not Empty (receivers) */
#define  I2C_SR1_TXE                         ((uint32_t)0x00000080U)            /*!< Data Register Empty (transmitters) */
#define  I2C_SR1_BERR                        ((uint32_t)0x00000100U)            /*!< Bus Error */
#define  I2C_SR1_ARLO                        ((uint32_t)0x00000200U)            /*!< Arbitration Lost (master mode) */
#define  I2C_SR1_AF                          ((uint32_t)0x00000400U)            /*!< Acknowledge Failure */
#define  I2C_SR1_OVR                         ((uint32_t)0x00000800U)            /*!< Overrun/Underrun */
#define  I2C_SR1_PECERR                      ((uint32_t)0x00001000U)            /*!< PEC Error in reception */
#define  I2C_SR1_TIMEOUT                     ((uint32_t)0x00004000U)            /*!< Timeout or Tlow Error */
#define  I2C_SR1_SMBALERT                    ((uint32_t)0x00008000U)            /*!< SMBus Alert */

/*******************  Bit definition for I2C_SR2 register  ********************/
#define  I2C_SR2_MSL                         ((uint32_t)0x00000001U)            /*!< Master/Slave */
#define  I2C_SR2_BUSY                        ((uint32_t)0x00000002U)            /*!< Bus Busy */
#define  I2C_SR2_TRA                         ((uint32_t)0x00000004U)            /*!< Transmitter/Receiver */
#define  I2C_SR2_GENCALL                     ((uint32_t)0x00000010U)            /*!< General Call Address (Slave mode) */
#define  I2C_SR2_SMBDEFAULT                  ((uint32_t)0x00000020U)            /*!< SMBus Device Default Address (Slave mode) */
#define  I2C_SR2_SMBHOST                     ((uint32_t)0x00000040U)            /*!< SMBus Host Header (Slave mode) */
#define  I2C_SR2_DUALF                       ((uint32_t)0x00000080U)            /*!< Dual Flag (Slave mode) */
#define  I2C_SR2_PEC                         ((uint32_t)0x0000FF00U)            /*!< Packet Error Checking Register */

/*******************  Bit definition for I2C_CCR register  ********************/
#define  I2C_CCR_CCR                         ((uint32_t)0x00000FFFU)            /*!< Clock Control Register in Fast/Standard mode (Master mode) */
#define  I2C_CCR_DUTY                        ((uint32_t)0x00004000U)            /*!< Fast Mode Duty Cycle */
#define  I2C_CCR_FS                          ((uint32_t)0x00008000U)            /*!< I2C Master Mode Selection */

/******************  Bit definition for I2C_TRISE register  *******************/
#define  I2C_TRISE_TRISE                     ((uint32_t)0x0000003FU)               /*!< Maximum Rise Time in Fast/Standard mode (Master mode) */

/******************************************************************************/
/*                                                                            */
/*                        Independent WATCHDOG (IWDG)                         */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for IWDG_KR register  ********************/
#define  IWDG_KR_KEY                         ((uint32_t)0x0000FFFFU)            /*!< Key value (write only, read 0000h) */

/*******************  Bit definition for IWDG_PR register  ********************/
#define  IWDG_PR_PR                          ((uint32_t)0x00000007U)               /*!< PR[2:0] (Prescaler divider) */
#define  IWDG_PR_PR_0                        ((uint32_t)0x00000001U)               /*!< Bit 0 */
#define  IWDG_PR_PR_1                        ((uint32_t)0x00000002U)               /*!< Bit 1 */
#define  IWDG_PR_PR_2                        ((uint32_t)0x00000004U)               /*!< Bit 2 */

/*******************  Bit definition for IWDG_RLR register  *******************/
#define  IWDG_RLR_RL                         ((uint32_t)0x00000FFFU)            /*!< Watchdog counter reload value */

/*******************  Bit definition for IWDG_SR register  ********************/
#define  IWDG_SR_PVU                         ((uint32_t)0x00000001U)               /*!< Watchdog prescaler value update */
#define  IWDG_SR_RVU                         ((uint32_t)0x00000002U)               /*!< Watchdog counter reload value update */

/******************************************************************************/
/*                                                                            */
/*                          LCD Controller (LCD)                              */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for LCD_CR register  *********************/
#define LCD_CR_LCDEN               ((uint32_t)0x00000001U)     /*!< LCD Enable Bit */
#define LCD_CR_VSEL                ((uint32_t)0x00000002U)     /*!< Voltage source selector Bit */

#define LCD_CR_DUTY                ((uint32_t)0x0000001CU)     /*!< DUTY[2:0] bits (Duty selector) */
#define LCD_CR_DUTY_0              ((uint32_t)0x00000004U)     /*!< Duty selector Bit 0 */
#define LCD_CR_DUTY_1              ((uint32_t)0x00000008U)     /*!< Duty selector Bit 1 */
#define LCD_CR_DUTY_2              ((uint32_t)0x00000010U)     /*!< Duty selector Bit 2 */

#define LCD_CR_BIAS                ((uint32_t)0x00000060U)     /*!< BIAS[1:0] bits (Bias selector) */
#define LCD_CR_BIAS_0              ((uint32_t)0x00000020U)     /*!< Bias selector Bit 0 */
#define LCD_CR_BIAS_1              ((uint32_t)0x00000040U)     /*!< Bias selector Bit 1 */

#define LCD_CR_MUX_SEG             ((uint32_t)0x00000080U)     /*!< Mux Segment Enable Bit */

/*******************  Bit definition for LCD_FCR register  ********************/
#define LCD_FCR_HD                 ((uint32_t)0x00000001U)     /*!< High Drive Enable Bit */
#define LCD_FCR_SOFIE              ((uint32_t)0x00000002U)     /*!< Start of Frame Interrupt Enable Bit */
#define LCD_FCR_UDDIE              ((uint32_t)0x00000008U)     /*!< Update Display Done Interrupt Enable Bit */

#define LCD_FCR_PON                ((uint32_t)0x00000070U)     /*!< PON[2:0] bits (Puls ON Duration) */
#define LCD_FCR_PON_0              ((uint32_t)0x00000010U)     /*!< Bit 0 */
#define LCD_FCR_PON_1              ((uint32_t)0x00000020U)     /*!< Bit 1 */
#define LCD_FCR_PON_2              ((uint32_t)0x00000040U)     /*!< Bit 2 */

#define LCD_FCR_DEAD               ((uint32_t)0x00000380U)     /*!< DEAD[2:0] bits (DEAD Time) */
#define LCD_FCR_DEAD_0             ((uint32_t)0x00000080U)     /*!< Bit 0 */
#define LCD_FCR_DEAD_1             ((uint32_t)0x00000100U)     /*!< Bit 1 */
#define LCD_FCR_DEAD_2             ((uint32_t)0x00000200U)     /*!< Bit 2 */

#define LCD_FCR_CC                 ((uint32_t)0x00001C00U)     /*!< CC[2:0] bits (Contrast Control) */
#define LCD_FCR_CC_0               ((uint32_t)0x00000400U)     /*!< Bit 0 */
#define LCD_FCR_CC_1               ((uint32_t)0x00000800U)     /*!< Bit 1 */
#define LCD_FCR_CC_2               ((uint32_t)0x00001000U)     /*!< Bit 2 */

#define LCD_FCR_BLINKF             ((uint32_t)0x0000E000U)     /*!< BLINKF[2:0] bits (Blink Frequency) */
#define LCD_FCR_BLINKF_0           ((uint32_t)0x00002000U)     /*!< Bit 0 */
#define LCD_FCR_BLINKF_1           ((uint32_t)0x00004000U)     /*!< Bit 1 */
#define LCD_FCR_BLINKF_2           ((uint32_t)0x00008000U)     /*!< Bit 2 */

#define LCD_FCR_BLINK              ((uint32_t)0x00030000U)     /*!< BLINK[1:0] bits (Blink Enable) */
#define LCD_FCR_BLINK_0            ((uint32_t)0x00010000U)     /*!< Bit 0 */
#define LCD_FCR_BLINK_1            ((uint32_t)0x00020000U)     /*!< Bit 1 */

#define LCD_FCR_DIV                ((uint32_t)0x003C0000U)     /*!< DIV[3:0] bits (Divider) */
#define LCD_FCR_PS                 ((uint32_t)0x03C00000U)     /*!< PS[3:0] bits (Prescaler) */

/*******************  Bit definition for LCD_SR register  *********************/
#define LCD_SR_ENS                 ((uint32_t)0x00000001U)     /*!< LCD Enabled Bit */
#define LCD_SR_SOF                 ((uint32_t)0x00000002U)     /*!< Start Of Frame Flag Bit */
#define LCD_SR_UDR                 ((uint32_t)0x00000004U)     /*!< Update Display Request Bit */
#define LCD_SR_UDD                 ((uint32_t)0x00000008U)     /*!< Update Display Done Flag Bit */
#define LCD_SR_RDY                 ((uint32_t)0x00000010U)     /*!< Ready Flag Bit */
#define LCD_SR_FCRSR               ((uint32_t)0x00000020U)     /*!< LCD FCR Register Synchronization Flag Bit */

/*******************  Bit definition for LCD_CLR register  ********************/
#define LCD_CLR_SOFC               ((uint32_t)0x00000002U)     /*!< Start Of Frame Flag Clear Bit */
#define LCD_CLR_UDDC               ((uint32_t)0x00000008U)     /*!< Update Display Done Flag Clear Bit */

/*******************  Bit definition for LCD_RAM register  ********************/
#define LCD_RAM_SEGMENT_DATA       ((uint32_t)0xFFFFFFFFU)     /*!< Segment Data Bits */

/******************************************************************************/
/*                                                                            */
/*                          Power Control (PWR)                               */
/*                                                                            */
/******************************************************************************/

#define PWR_PVD_SUPPORT                       /*!< PWR feature available only on specific devices: Power Voltage Detection feature */

/********************  Bit definition for PWR_CR register  ********************/
#define  PWR_CR_LPSDSR                       ((uint32_t)0x00000001U)     /*!< Low-power deepsleep/sleep/low power run */
#define  PWR_CR_PDDS                         ((uint32_t)0x00000002U)     /*!< Power Down Deepsleep */
#define  PWR_CR_CWUF                         ((uint32_t)0x00000004U)     /*!< Clear Wakeup Flag */
#define  PWR_CR_CSBF                         ((uint32_t)0x00000008U)     /*!< Clear Standby Flag */
#define  PWR_CR_PVDE                         ((uint32_t)0x00000010U)     /*!< Power Voltage Detector Enable */

#define  PWR_CR_PLS                          ((uint32_t)0x000000E0U)     /*!< PLS[2:0] bits (PVD Level Selection) */
#define  PWR_CR_PLS_0                        ((uint32_t)0x00000020U)     /*!< Bit 0 */
#define  PWR_CR_PLS_1                        ((uint32_t)0x00000040U)     /*!< Bit 1 */
#define  PWR_CR_PLS_2                        ((uint32_t)0x00000080U)     /*!< Bit 2 */

/*!< PVD level configuration */
#define  PWR_CR_PLS_LEV0                     ((uint32_t)0x00000000U)     /*!< PVD level 0 */
#define  PWR_CR_PLS_LEV1                     ((uint32_t)0x00000020U)     /*!< PVD level 1 */
#define  PWR_CR_PLS_LEV2                     ((uint32_t)0x00000040U)     /*!< PVD level 2 */
#define  PWR_CR_PLS_LEV3                     ((uint32_t)0x00000060U)     /*!< PVD level 3 */
#define  PWR_CR_PLS_LEV4                     ((uint32_t)0x00000080U)     /*!< PVD level 4 */
#define  PWR_CR_PLS_LEV5                     ((uint32_t)0x000000A0U)     /*!< PVD level 5 */
#define  PWR_CR_PLS_LEV6                     ((uint32_t)0x000000C0U)     /*!< PVD level 6 */
#define  PWR_CR_PLS_LEV7                     ((uint32_t)0x000000E0U)     /*!< PVD level 7 */

#define  PWR_CR_DBP                          ((uint32_t)0x00000100U)     /*!< Disable Backup Domain write protection */
#define  PWR_CR_ULP                          ((uint32_t)0x00000200U)     /*!< Ultra Low Power mode */
#define  PWR_CR_FWU                          ((uint32_t)0x00000400U)     /*!< Fast wakeup */

#define  PWR_CR_VOS                          ((uint32_t)0x00001800U)     /*!< VOS[1:0] bits (Voltage scaling range selection) */
#define  PWR_CR_VOS_0                        ((uint32_t)0x00000800U)     /*!< Bit 0 */
#define  PWR_CR_VOS_1                        ((uint32_t)0x00001000U)     /*!< Bit 1 */
#define  PWR_CR_LPRUN                        ((uint32_t)0x00004000U)     /*!< Low power run mode */

/*******************  Bit definition for PWR_CSR register  ********************/
#define  PWR_CSR_WUF                         ((uint32_t)0x00000001U)     /*!< Wakeup Flag */
#define  PWR_CSR_SBF                         ((uint32_t)0x00000002U)     /*!< Standby Flag */
#define  PWR_CSR_PVDO                        ((uint32_t)0x00000004U)     /*!< PVD Output */
#define  PWR_CSR_VREFINTRDYF                 ((uint32_t)0x00000008U)     /*!< Internal voltage reference (VREFINT) ready flag */
#define  PWR_CSR_VOSF                        ((uint32_t)0x00000010U)     /*!< Voltage Scaling select flag */
#define  PWR_CSR_REGLPF                      ((uint32_t)0x00000020U)     /*!< Regulator LP flag */

#define  PWR_CSR_EWUP1                       ((uint32_t)0x00000100U)     /*!< Enable WKUP pin 1 */
#define  PWR_CSR_EWUP2                       ((uint32_t)0x00000200U)     /*!< Enable WKUP pin 2 */
#define  PWR_CSR_EWUP3                       ((uint32_t)0x00000400U)     /*!< Enable WKUP pin 3 */

/******************************************************************************/
/*                                                                            */
/*                      Reset and Clock Control (RCC)                         */
/*                                                                            */
/******************************************************************************/
/*
* @brief Specific device feature definitions  (not present on all devices in the STM32F0 family)
*/
#define RCC_LSECSS_SUPPORT          /*!< LSE CSS feature support */

/********************  Bit definition for RCC_CR register  ********************/
#define  RCC_CR_HSION                        ((uint32_t)0x00000001U)        /*!< Internal High Speed clock enable */
#define  RCC_CR_HSIRDY                       ((uint32_t)0x00000002U)        /*!< Internal High Speed clock ready flag */

#define  RCC_CR_MSION                        ((uint32_t)0x00000100U)        /*!< Internal Multi Speed clock enable */
#define  RCC_CR_MSIRDY                       ((uint32_t)0x00000200U)        /*!< Internal Multi Speed clock ready flag */

#define  RCC_CR_HSEON                        ((uint32_t)0x00010000U)        /*!< External High Speed clock enable */
#define  RCC_CR_HSERDY                       ((uint32_t)0x00020000U)        /*!< External High Speed clock ready flag */
#define  RCC_CR_HSEBYP                       ((uint32_t)0x00040000U)        /*!< External High Speed clock Bypass */

#define  RCC_CR_PLLON                        ((uint32_t)0x01000000U)        /*!< PLL enable */
#define  RCC_CR_PLLRDY                       ((uint32_t)0x02000000U)        /*!< PLL clock ready flag */
#define  RCC_CR_CSSON                        ((uint32_t)0x10000000U)        /*!< Clock Security System enable */

#define  RCC_CR_RTCPRE                       ((uint32_t)0x60000000U)        /*!< RTC/LCD Prescaler */
#define  RCC_CR_RTCPRE_0                     ((uint32_t)0x20000000U)        /*!< Bit0 */
#define  RCC_CR_RTCPRE_1                     ((uint32_t)0x40000000U)        /*!< Bit1 */

/********************  Bit definition for RCC_ICSCR register  *****************/
#define  RCC_ICSCR_HSICAL                    ((uint32_t)0x000000FFU)        /*!< Internal High Speed clock Calibration */
#define  RCC_ICSCR_HSITRIM                   ((uint32_t)0x00001F00U)        /*!< Internal High Speed clock trimming */

#define  RCC_ICSCR_MSIRANGE                  ((uint32_t)0x0000E000U)        /*!< Internal Multi Speed clock Range */
#define  RCC_ICSCR_MSIRANGE_0                ((uint32_t)0x00000000U)        /*!< Internal Multi Speed clock Range 65.536 KHz */
#define  RCC_ICSCR_MSIRANGE_1                ((uint32_t)0x00002000U)        /*!< Internal Multi Speed clock Range 131.072 KHz */
#define  RCC_ICSCR_MSIRANGE_2                ((uint32_t)0x00004000U)        /*!< Internal Multi Speed clock Range 262.144 KHz */
#define  RCC_ICSCR_MSIRANGE_3                ((uint32_t)0x00006000U)        /*!< Internal Multi Speed clock Range 524.288 KHz */
#define  RCC_ICSCR_MSIRANGE_4                ((uint32_t)0x00008000U)        /*!< Internal Multi Speed clock Range 1.048 MHz */
#define  RCC_ICSCR_MSIRANGE_5                ((uint32_t)0x0000A000U)        /*!< Internal Multi Speed clock Range 2.097 MHz */
#define  RCC_ICSCR_MSIRANGE_6                ((uint32_t)0x0000C000U)        /*!< Internal Multi Speed clock Range 4.194 MHz */
#define  RCC_ICSCR_MSICAL                    ((uint32_t)0x00FF0000U)        /*!< Internal Multi Speed clock Calibration */
#define  RCC_ICSCR_MSITRIM                   ((uint32_t)0xFF000000U)        /*!< Internal Multi Speed clock trimming */

/********************  Bit definition for RCC_CFGR register  ******************/
#define  RCC_CFGR_SW                         ((uint32_t)0x00000003U)        /*!< SW[1:0] bits (System clock Switch) */
#define  RCC_CFGR_SW_0                       ((uint32_t)0x00000001U)        /*!< Bit 0 */
#define  RCC_CFGR_SW_1                       ((uint32_t)0x00000002U)        /*!< Bit 1 */

/*!< SW configuration */
#define  RCC_CFGR_SW_MSI                     ((uint32_t)0x00000000U)        /*!< MSI selected as system clock */
#define  RCC_CFGR_SW_HSI                     ((uint32_t)0x00000001U)        /*!< HSI selected as system clock */
#define  RCC_CFGR_SW_HSE                     ((uint32_t)0x00000002U)        /*!< HSE selected as system clock */
#define  RCC_CFGR_SW_PLL                     ((uint32_t)0x00000003U)        /*!< PLL selected as system clock */

#define  RCC_CFGR_SWS                        ((uint32_t)0x0000000CU)        /*!< SWS[1:0] bits (System Clock Switch Status) */
#define  RCC_CFGR_SWS_0                      ((uint32_t)0x00000004U)        /*!< Bit 0 */
#define  RCC_CFGR_SWS_1                      ((uint32_t)0x00000008U)        /*!< Bit 1 */

/*!< SWS configuration */
#define  RCC_CFGR_SWS_MSI                    ((uint32_t)0x00000000U)        /*!< MSI oscillator used as system clock */
#define  RCC_CFGR_SWS_HSI                    ((uint32_t)0x00000004U)        /*!< HSI oscillator used as system clock */
#define  RCC_CFGR_SWS_HSE                    ((uint32_t)0x00000008U)        /*!< HSE oscillator used as system clock */
#define  RCC_CFGR_SWS_PLL                    ((uint32_t)0x0000000CU)        /*!< PLL used as system clock */

#define  RCC_CFGR_HPRE                       ((uint32_t)0x000000F0U)        /*!< HPRE[3:0] bits (AHB prescaler) */
#define  RCC_CFGR_HPRE_0                     ((uint32_t)0x00000010U)        /*!< Bit 0 */
#define  RCC_CFGR_HPRE_1                     ((uint32_t)0x00000020U)        /*!< Bit 1 */
#define  RCC_CFGR_HPRE_2                     ((uint32_t)0x00000040U)        /*!< Bit 2 */
#define  RCC_CFGR_HPRE_3                     ((uint32_t)0x00000080U)        /*!< Bit 3 */

/*!< HPRE configuration */
#define  RCC_CFGR_HPRE_DIV1                  ((uint32_t)0x00000000U)        /*!< SYSCLK not divided */
#define  RCC_CFGR_HPRE_DIV2                  ((uint32_t)0x00000080U)        /*!< SYSCLK divided by 2 */
#define  RCC_CFGR_HPRE_DIV4                  ((uint32_t)0x00000090U)        /*!< SYSCLK divided by 4 */
#define  RCC_CFGR_HPRE_DIV8                  ((uint32_t)0x000000A0U)        /*!< SYSCLK divided by 8 */
#define  RCC_CFGR_HPRE_DIV16                 ((uint32_t)0x000000B0U)        /*!< SYSCLK divided by 16 */
#define  RCC_CFGR_HPRE_DIV64                 ((uint32_t)0x000000C0U)        /*!< SYSCLK divided by 64 */
#define  RCC_CFGR_HPRE_DIV128                ((uint32_t)0x000000D0U)        /*!< SYSCLK divided by 128 */
#define  RCC_CFGR_HPRE_DIV256                ((uint32_t)0x000000E0U)        /*!< SYSCLK divided by 256 */
#define  RCC_CFGR_HPRE_DIV512                ((uint32_t)0x000000F0U)        /*!< SYSCLK divided by 512 */

#define  RCC_CFGR_PPRE1                      ((uint32_t)0x00000700U)        /*!< PRE1[2:0] bits (APB1 prescaler) */
#define  RCC_CFGR_PPRE1_0                    ((uint32_t)0x00000100U)        /*!< Bit 0 */
#define  RCC_CFGR_PPRE1_1                    ((uint32_t)0x00000200U)        /*!< Bit 1 */
#define  RCC_CFGR_PPRE1_2                    ((uint32_t)0x00000400U)        /*!< Bit 2 */

/*!< PPRE1 configuration */
#define  RCC_CFGR_PPRE1_DIV1                 ((uint32_t)0x00000000U)        /*!< HCLK not divided */
#define  RCC_CFGR_PPRE1_DIV2                 ((uint32_t)0x00000400U)        /*!< HCLK divided by 2 */
#define  RCC_CFGR_PPRE1_DIV4                 ((uint32_t)0x00000500U)        /*!< HCLK divided by 4 */
#define  RCC_CFGR_PPRE1_DIV8                 ((uint32_t)0x00000600U)        /*!< HCLK divided by 8 */
#define  RCC_CFGR_PPRE1_DIV16                ((uint32_t)0x00000700U)        /*!< HCLK divided by 16 */

#define  RCC_CFGR_PPRE2                      ((uint32_t)0x00003800U)        /*!< PRE2[2:0] bits (APB2 prescaler) */
#define  RCC_CFGR_PPRE2_0                    ((uint32_t)0x00000800U)        /*!< Bit 0 */
#define  RCC_CFGR_PPRE2_1                    ((uint32_t)0x00001000U)        /*!< Bit 1 */
#define  RCC_CFGR_PPRE2_2                    ((uint32_t)0x00002000U)        /*!< Bit 2 */

/*!< PPRE2 configuration */
#define  RCC_CFGR_PPRE2_DIV1                 ((uint32_t)0x00000000U)        /*!< HCLK not divided */
#define  RCC_CFGR_PPRE2_DIV2                 ((uint32_t)0x00002000U)        /*!< HCLK divided by 2 */
#define  RCC_CFGR_PPRE2_DIV4                 ((uint32_t)0x00002800U)        /*!< HCLK divided by 4 */
#define  RCC_CFGR_PPRE2_DIV8                 ((uint32_t)0x00003000U)        /*!< HCLK divided by 8 */
#define  RCC_CFGR_PPRE2_DIV16                ((uint32_t)0x00003800U)        /*!< HCLK divided by 16 */

/*!< PLL entry clock source*/
#define  RCC_CFGR_PLLSRC                     ((uint32_t)0x00010000U)        /*!< PLL entry clock source */

#define  RCC_CFGR_PLLSRC_HSI                 ((uint32_t)0x00000000U)        /*!< HSI as PLL entry clock source */
#define  RCC_CFGR_PLLSRC_HSE                 ((uint32_t)0x00010000U)        /*!< HSE as PLL entry clock source */


/*!< PLLMUL configuration */
#define  RCC_CFGR_PLLMUL                     ((uint32_t)0x003C0000U)        /*!< PLLMUL[3:0] bits (PLL multiplication factor) */
#define  RCC_CFGR_PLLMUL_0                   ((uint32_t)0x00040000U)        /*!< Bit 0 */
#define  RCC_CFGR_PLLMUL_1                   ((uint32_t)0x00080000U)        /*!< Bit 1 */
#define  RCC_CFGR_PLLMUL_2                   ((uint32_t)0x00100000U)        /*!< Bit 2 */
#define  RCC_CFGR_PLLMUL_3                   ((uint32_t)0x00200000U)        /*!< Bit 3 */

/*!< PLLMUL configuration */
#define  RCC_CFGR_PLLMUL3                    ((uint32_t)0x00000000U)        /*!< PLL input clock * 3 */
#define  RCC_CFGR_PLLMUL4                    ((uint32_t)0x00040000U)        /*!< PLL input clock * 4 */
#define  RCC_CFGR_PLLMUL6                    ((uint32_t)0x00080000U)        /*!< PLL input clock * 6 */
#define  RCC_CFGR_PLLMUL8                    ((uint32_t)0x000C0000U)        /*!< PLL input clock * 8 */
#define  RCC_CFGR_PLLMUL12                   ((uint32_t)0x00100000U)        /*!< PLL input clock * 12 */
#define  RCC_CFGR_PLLMUL16                   ((uint32_t)0x00140000U)        /*!< PLL input clock * 16 */
#define  RCC_CFGR_PLLMUL24                   ((uint32_t)0x00180000U)        /*!< PLL input clock * 24 */
#define  RCC_CFGR_PLLMUL32                   ((uint32_t)0x001C0000U)        /*!< PLL input clock * 32 */
#define  RCC_CFGR_PLLMUL48                   ((uint32_t)0x00200000U)        /*!< PLL input clock * 48 */

/*!< PLLDIV configuration */
#define  RCC_CFGR_PLLDIV                     ((uint32_t)0x00C00000U)        /*!< PLLDIV[1:0] bits (PLL Output Division) */
#define  RCC_CFGR_PLLDIV_0                   ((uint32_t)0x00400000U)        /*!< Bit0 */
#define  RCC_CFGR_PLLDIV_1                   ((uint32_t)0x00800000U)        /*!< Bit1 */


/*!< PLLDIV configuration */
#define  RCC_CFGR_PLLDIV1                    ((uint32_t)0x00000000U)        /*!< PLL clock output = CKVCO / 1 */
#define  RCC_CFGR_PLLDIV2                    ((uint32_t)0x00400000U)        /*!< PLL clock output = CKVCO / 2 */
#define  RCC_CFGR_PLLDIV3                    ((uint32_t)0x00800000U)        /*!< PLL clock output = CKVCO / 3 */
#define  RCC_CFGR_PLLDIV4                    ((uint32_t)0x00C00000U)        /*!< PLL clock output = CKVCO / 4 */


#define  RCC_CFGR_MCOSEL                     ((uint32_t)0x07000000U)        /*!< MCO[2:0] bits (Microcontroller Clock Output) */
#define  RCC_CFGR_MCOSEL_0                   ((uint32_t)0x01000000U)        /*!< Bit 0 */
#define  RCC_CFGR_MCOSEL_1                   ((uint32_t)0x02000000U)        /*!< Bit 1 */
#define  RCC_CFGR_MCOSEL_2                   ((uint32_t)0x04000000U)        /*!< Bit 2 */

/*!< MCO configuration */
#define  RCC_CFGR_MCOSEL_NOCLOCK             ((uint32_t)0x00000000U)        /*!< No clock */
#define  RCC_CFGR_MCOSEL_SYSCLK              ((uint32_t)0x01000000U)        /*!< System clock selected */
#define  RCC_CFGR_MCOSEL_HSI                 ((uint32_t)0x02000000U)        /*!< Internal 16 MHz RC oscillator clock selected */
#define  RCC_CFGR_MCOSEL_MSI                 ((uint32_t)0x03000000U)        /*!< Internal Medium Speed RC oscillator clock selected */
#define  RCC_CFGR_MCOSEL_HSE                 ((uint32_t)0x04000000U)        /*!< External 1-25 MHz oscillator clock selected */
#define  RCC_CFGR_MCOSEL_PLL                 ((uint32_t)0x05000000U)        /*!< PLL clock divided */
#define  RCC_CFGR_MCOSEL_LSI                 ((uint32_t)0x06000000U)        /*!< LSI selected */
#define  RCC_CFGR_MCOSEL_LSE                 ((uint32_t)0x07000000U)        /*!< LSE selected */

#define  RCC_CFGR_MCOPRE                     ((uint32_t)0x70000000U)        /*!< MCOPRE[2:0] bits (Microcontroller Clock Output Prescaler) */
#define  RCC_CFGR_MCOPRE_0                   ((uint32_t)0x10000000U)        /*!<Bit 0 */
#define  RCC_CFGR_MCOPRE_1                   ((uint32_t)0x20000000U)        /*!<Bit 1 */
#define  RCC_CFGR_MCOPRE_2                   ((uint32_t)0x40000000U)        /*!<Bit 2 */

/*!< MCO Prescaler configuration */ 
#define  RCC_CFGR_MCOPRE_DIV1                ((uint32_t)0x00000000U)        /*!< MCO is divided by 1 */
#define  RCC_CFGR_MCOPRE_DIV2                ((uint32_t)0x10000000U)        /*!< MCO is divided by 2 */
#define  RCC_CFGR_MCOPRE_DIV4                ((uint32_t)0x20000000U)        /*!< MCO is divided by 4 */
#define  RCC_CFGR_MCOPRE_DIV8                ((uint32_t)0x30000000U)        /*!< MCO is divided by 8 */
#define  RCC_CFGR_MCOPRE_DIV16               ((uint32_t)0x40000000U)        /*!< MCO is divided by 16 */
 
/* Legacy aliases */
#define  RCC_CFGR_MCO_DIV1                  RCC_CFGR_MCOPRE_DIV1
#define  RCC_CFGR_MCO_DIV2                  RCC_CFGR_MCOPRE_DIV2
#define  RCC_CFGR_MCO_DIV4                  RCC_CFGR_MCOPRE_DIV4
#define  RCC_CFGR_MCO_DIV8                  RCC_CFGR_MCOPRE_DIV8
#define  RCC_CFGR_MCO_DIV16                 RCC_CFGR_MCOPRE_DIV16
#define  RCC_CFGR_MCO_NOCLOCK               RCC_CFGR_MCOSEL_NOCLOCK
#define  RCC_CFGR_MCO_SYSCLK                RCC_CFGR_MCOSEL_SYSCLK
#define  RCC_CFGR_MCO_HSI                   RCC_CFGR_MCOSEL_HSI
#define  RCC_CFGR_MCO_MSI                   RCC_CFGR_MCOSEL_MSI
#define  RCC_CFGR_MCO_HSE                   RCC_CFGR_MCOSEL_HSE
#define  RCC_CFGR_MCO_PLL                   RCC_CFGR_MCOSEL_PLL
#define  RCC_CFGR_MCO_LSI                   RCC_CFGR_MCOSEL_LSI
#define  RCC_CFGR_MCO_LSE                   RCC_CFGR_MCOSEL_LSE

/*!<******************  Bit definition for RCC_CIR register  ********************/
#define  RCC_CIR_LSIRDYF                     ((uint32_t)0x00000001U)        /*!< LSI Ready Interrupt flag */
#define  RCC_CIR_LSERDYF                     ((uint32_t)0x00000002U)        /*!< LSE Ready Interrupt flag */
#define  RCC_CIR_HSIRDYF                     ((uint32_t)0x00000004U)        /*!< HSI Ready Interrupt flag */
#define  RCC_CIR_HSERDYF                     ((uint32_t)0x00000008U)        /*!< HSE Ready Interrupt flag */
#define  RCC_CIR_PLLRDYF                     ((uint32_t)0x00000010U)        /*!< PLL Ready Interrupt flag */
#define  RCC_CIR_MSIRDYF                     ((uint32_t)0x00000020U)        /*!< MSI Ready Interrupt flag */
#define  RCC_CIR_LSECSSF                     ((uint32_t)0x00000040U)        /*!< LSE CSS Interrupt flag */
#define  RCC_CIR_CSSF                        ((uint32_t)0x00000080U)        /*!< Clock Security System Interrupt flag */

#define  RCC_CIR_LSIRDYIE                    ((uint32_t)0x00000100U)        /*!< LSI Ready Interrupt Enable */
#define  RCC_CIR_LSERDYIE                    ((uint32_t)0x00000200U)        /*!< LSE Ready Interrupt Enable */
#define  RCC_CIR_HSIRDYIE                    ((uint32_t)0x00000400U)        /*!< HSI Ready Interrupt Enable */
#define  RCC_CIR_HSERDYIE                    ((uint32_t)0x00000800U)        /*!< HSE Ready Interrupt Enable */
#define  RCC_CIR_PLLRDYIE                    ((uint32_t)0x00001000U)        /*!< PLL Ready Interrupt Enable */
#define  RCC_CIR_MSIRDYIE                    ((uint32_t)0x00002000U)        /*!< MSI Ready Interrupt Enable */
#define  RCC_CIR_LSECSSIE                    ((uint32_t)0x00004000U)        /*!< LSE CSS Interrupt Enable */

#define  RCC_CIR_LSIRDYC                     ((uint32_t)0x00010000U)        /*!< LSI Ready Interrupt Clear */
#define  RCC_CIR_LSERDYC                     ((uint32_t)0x00020000U)        /*!< LSE Ready Interrupt Clear */
#define  RCC_CIR_HSIRDYC                     ((uint32_t)0x00040000U)        /*!< HSI Ready Interrupt Clear */
#define  RCC_CIR_HSERDYC                     ((uint32_t)0x00080000U)        /*!< HSE Ready Interrupt Clear */
#define  RCC_CIR_PLLRDYC                     ((uint32_t)0x00100000U)        /*!< PLL Ready Interrupt Clear */
#define  RCC_CIR_MSIRDYC                     ((uint32_t)0x00200000U)        /*!< MSI Ready Interrupt Clear */
#define  RCC_CIR_LSECSSC                     ((uint32_t)0x00400000U)        /*!< LSE CSS Interrupt Clear */
#define  RCC_CIR_CSSC                        ((uint32_t)0x00800000U)        /*!< Clock Security System Interrupt Clear */

/*****************  Bit definition for RCC_AHBRSTR register  ******************/
#define  RCC_AHBRSTR_GPIOARST                ((uint32_t)0x00000001U)        /*!< GPIO port A reset */
#define  RCC_AHBRSTR_GPIOBRST                ((uint32_t)0x00000002U)        /*!< GPIO port B reset */
#define  RCC_AHBRSTR_GPIOCRST                ((uint32_t)0x00000004U)        /*!< GPIO port C reset */
#define  RCC_AHBRSTR_GPIODRST                ((uint32_t)0x00000008U)        /*!< GPIO port D reset */
#define  RCC_AHBRSTR_GPIOERST                ((uint32_t)0x00000010U)        /*!< GPIO port E reset */
#define  RCC_AHBRSTR_GPIOHRST                ((uint32_t)0x00000020U)        /*!< GPIO port H reset */
#define  RCC_AHBRSTR_GPIOFRST                ((uint32_t)0x00000040U)        /*!< GPIO port F reset */
#define  RCC_AHBRSTR_GPIOGRST                ((uint32_t)0x00000080U)        /*!< GPIO port G reset */
#define  RCC_AHBRSTR_CRCRST                  ((uint32_t)0x00001000U)        /*!< CRC reset */
#define  RCC_AHBRSTR_FLITFRST                ((uint32_t)0x00008000U)        /*!< FLITF reset */
#define  RCC_AHBRSTR_DMA1RST                 ((uint32_t)0x01000000U)        /*!< DMA1 reset */
#define  RCC_AHBRSTR_DMA2RST                 ((uint32_t)0x02000000U)        /*!< DMA2 reset */
 
/*****************  Bit definition for RCC_APB2RSTR register  *****************/
#define  RCC_APB2RSTR_SYSCFGRST              ((uint32_t)0x00000001U)        /*!< System Configuration SYSCFG reset */
#define  RCC_APB2RSTR_TIM9RST                ((uint32_t)0x00000004U)        /*!< TIM9 reset */
#define  RCC_APB2RSTR_TIM10RST               ((uint32_t)0x00000008U)        /*!< TIM10 reset */
#define  RCC_APB2RSTR_TIM11RST               ((uint32_t)0x00000010U)        /*!< TIM11 reset */
#define  RCC_APB2RSTR_ADC1RST                ((uint32_t)0x00000200U)        /*!< ADC1 reset */
#define  RCC_APB2RSTR_SPI1RST                ((uint32_t)0x00001000U)        /*!< SPI1 reset */
#define  RCC_APB2RSTR_USART1RST              ((uint32_t)0x00004000U)        /*!< USART1 reset */

/*****************  Bit definition for RCC_APB1RSTR register  *****************/
#define  RCC_APB1RSTR_TIM2RST                ((uint32_t)0x00000001U)        /*!< Timer 2 reset */
#define  RCC_APB1RSTR_TIM3RST                ((uint32_t)0x00000002U)        /*!< Timer 3 reset */
#define  RCC_APB1RSTR_TIM4RST                ((uint32_t)0x00000004U)        /*!< Timer 4 reset */
#define  RCC_APB1RSTR_TIM5RST                ((uint32_t)0x00000008U)        /*!< Timer 5 reset */
#define  RCC_APB1RSTR_TIM6RST                ((uint32_t)0x00000010U)        /*!< Timer 6 reset */
#define  RCC_APB1RSTR_TIM7RST                ((uint32_t)0x00000020U)        /*!< Timer 7 reset */
#define  RCC_APB1RSTR_LCDRST                 ((uint32_t)0x00000200U)        /*!< LCD reset */
#define  RCC_APB1RSTR_WWDGRST                ((uint32_t)0x00000800U)        /*!< Window Watchdog reset */
#define  RCC_APB1RSTR_SPI2RST                ((uint32_t)0x00004000U)        /*!< SPI 2 reset */
#define  RCC_APB1RSTR_SPI3RST                ((uint32_t)0x00008000U)        /*!< SPI 3 reset */
#define  RCC_APB1RSTR_USART2RST              ((uint32_t)0x00020000U)        /*!< USART 2 reset */
#define  RCC_APB1RSTR_USART3RST              ((uint32_t)0x00040000U)        /*!< USART 3 reset */
#define  RCC_APB1RSTR_UART4RST               ((uint32_t)0x00080000U)        /*!< UART 4 reset */
#define  RCC_APB1RSTR_UART5RST               ((uint32_t)0x00100000U)        /*!< UART 5 reset */
#define  RCC_APB1RSTR_I2C1RST                ((uint32_t)0x00200000U)        /*!< I2C 1 reset */
#define  RCC_APB1RSTR_I2C2RST                ((uint32_t)0x00400000U)        /*!< I2C 2 reset */
#define  RCC_APB1RSTR_USBRST                 ((uint32_t)0x00800000U)        /*!< USB reset */
#define  RCC_APB1RSTR_PWRRST                 ((uint32_t)0x10000000U)        /*!< Power interface reset */
#define  RCC_APB1RSTR_DACRST                 ((uint32_t)0x20000000U)        /*!< DAC interface reset */
#define  RCC_APB1RSTR_COMPRST                ((uint32_t)0x80000000U)        /*!< Comparator interface reset */

/******************  Bit definition for RCC_AHBENR register  ******************/
#define  RCC_AHBENR_GPIOAEN                  ((uint32_t)0x00000001U)        /*!< GPIO port A clock enable */
#define  RCC_AHBENR_GPIOBEN                  ((uint32_t)0x00000002U)        /*!< GPIO port B clock enable */
#define  RCC_AHBENR_GPIOCEN                  ((uint32_t)0x00000004U)        /*!< GPIO port C clock enable */
#define  RCC_AHBENR_GPIODEN                  ((uint32_t)0x00000008U)        /*!< GPIO port D clock enable */
#define  RCC_AHBENR_GPIOEEN                  ((uint32_t)0x00000010U)        /*!< GPIO port E clock enable */
#define  RCC_AHBENR_GPIOHEN                  ((uint32_t)0x00000020U)        /*!< GPIO port H clock enable */
#define  RCC_AHBENR_GPIOFEN                  ((uint32_t)0x00000040U)        /*!< GPIO port F clock enable */
#define  RCC_AHBENR_GPIOGEN                  ((uint32_t)0x00000080U)        /*!< GPIO port G clock enable */
#define  RCC_AHBENR_CRCEN                    ((uint32_t)0x00001000U)        /*!< CRC clock enable */
#define  RCC_AHBENR_FLITFEN                  ((uint32_t)0x00008000U)        /*!< FLITF clock enable (has effect only when
                                                                                the Flash memory is in power down mode) */
#define  RCC_AHBENR_DMA1EN                   ((uint32_t)0x01000000U)        /*!< DMA1 clock enable */
#define  RCC_AHBENR_DMA2EN                   ((uint32_t)0x02000000U)        /*!< DMA2 clock enable */

/******************  Bit definition for RCC_APB2ENR register  *****************/
#define  RCC_APB2ENR_SYSCFGEN                ((uint32_t)0x00000001U)         /*!< System Configuration SYSCFG clock enable */
#define  RCC_APB2ENR_TIM9EN                  ((uint32_t)0x00000004U)         /*!< TIM9 interface clock enable */
#define  RCC_APB2ENR_TIM10EN                 ((uint32_t)0x00000008U)         /*!< TIM10 interface clock enable */
#define  RCC_APB2ENR_TIM11EN                 ((uint32_t)0x00000010U)         /*!< TIM11 Timer clock enable */
#define  RCC_APB2ENR_ADC1EN                  ((uint32_t)0x00000200U)         /*!< ADC1 clock enable */
#define  RCC_APB2ENR_SPI1EN                  ((uint32_t)0x00001000U)         /*!< SPI1 clock enable */
#define  RCC_APB2ENR_USART1EN                ((uint32_t)0x00004000U)         /*!< USART1 clock enable */

/*****************  Bit definition for RCC_APB1ENR register  ******************/
#define  RCC_APB1ENR_TIM2EN                  ((uint32_t)0x00000001U)        /*!< Timer 2 clock enabled*/
#define  RCC_APB1ENR_TIM3EN                  ((uint32_t)0x00000002U)        /*!< Timer 3 clock enable */
#define  RCC_APB1ENR_TIM4EN                  ((uint32_t)0x00000004U)        /*!< Timer 4 clock enable */
#define  RCC_APB1ENR_TIM5EN                  ((uint32_t)0x00000008U)        /*!< Timer 5 clock enable */
#define  RCC_APB1ENR_TIM6EN                  ((uint32_t)0x00000010U)        /*!< Timer 6 clock enable */
#define  RCC_APB1ENR_TIM7EN                  ((uint32_t)0x00000020U)        /*!< Timer 7 clock enable */
#define  RCC_APB1ENR_LCDEN                   ((uint32_t)0x00000200U)        /*!< LCD clock enable */
#define  RCC_APB1ENR_WWDGEN                  ((uint32_t)0x00000800U)        /*!< Window Watchdog clock enable */
#define  RCC_APB1ENR_SPI2EN                  ((uint32_t)0x00004000U)        /*!< SPI 2 clock enable */
#define  RCC_APB1ENR_SPI3EN                  ((uint32_t)0x00008000U)        /*!< SPI 3 clock enable */
#define  RCC_APB1ENR_USART2EN                ((uint32_t)0x00020000U)        /*!< USART 2 clock enable */
#define  RCC_APB1ENR_USART3EN                ((uint32_t)0x00040000U)        /*!< USART 3 clock enable */
#define  RCC_APB1ENR_UART4EN                 ((uint32_t)0x00080000U)        /*!< UART 4 clock enable */
#define  RCC_APB1ENR_UART5EN                 ((uint32_t)0x00100000U)        /*!< UART 5 clock enable */
#define  RCC_APB1ENR_I2C1EN                  ((uint32_t)0x00200000U)        /*!< I2C 1 clock enable */
#define  RCC_APB1ENR_I2C2EN                  ((uint32_t)0x00400000U)        /*!< I2C 2 clock enable */
#define  RCC_APB1ENR_USBEN                   ((uint32_t)0x00800000U)        /*!< USB clock enable */
#define  RCC_APB1ENR_PWREN                   ((uint32_t)0x10000000U)        /*!< Power interface clock enable */
#define  RCC_APB1ENR_DACEN                   ((uint32_t)0x20000000U)        /*!< DAC interface clock enable */
#define  RCC_APB1ENR_COMPEN                  ((uint32_t)0x80000000U)        /*!< Comparator interface clock enable */

/******************  Bit definition for RCC_AHBLPENR register  ****************/
#define  RCC_AHBLPENR_GPIOALPEN              ((uint32_t)0x00000001U)        /*!< GPIO port A clock enabled in sleep mode */
#define  RCC_AHBLPENR_GPIOBLPEN              ((uint32_t)0x00000002U)        /*!< GPIO port B clock enabled in sleep mode */
#define  RCC_AHBLPENR_GPIOCLPEN              ((uint32_t)0x00000004U)        /*!< GPIO port C clock enabled in sleep mode */
#define  RCC_AHBLPENR_GPIODLPEN              ((uint32_t)0x00000008U)        /*!< GPIO port D clock enabled in sleep mode */
#define  RCC_AHBLPENR_GPIOELPEN              ((uint32_t)0x00000010U)        /*!< GPIO port E clock enabled in sleep mode */
#define  RCC_AHBLPENR_GPIOHLPEN              ((uint32_t)0x00000020U)        /*!< GPIO port H clock enabled in sleep mode */
#define  RCC_AHBLPENR_GPIOFLPEN              ((uint32_t)0x00000040U)        /*!< GPIO port F clock enabled in sleep mode */
#define  RCC_AHBLPENR_GPIOGLPEN              ((uint32_t)0x00000080U)        /*!< GPIO port G clock enabled in sleep mode */
#define  RCC_AHBLPENR_CRCLPEN                ((uint32_t)0x00001000U)        /*!< CRC clock enabled in sleep mode */
#define  RCC_AHBLPENR_FLITFLPEN              ((uint32_t)0x00008000U)        /*!< Flash Interface clock enabled in sleep mode
                                                                                (has effect only when the Flash memory is
                                                                                 in power down mode) */
#define  RCC_AHBLPENR_SRAMLPEN               ((uint32_t)0x00010000U)        /*!< SRAM clock enabled in sleep mode */
#define  RCC_AHBLPENR_DMA1LPEN               ((uint32_t)0x01000000U)        /*!< DMA1 clock enabled in sleep mode */
#define  RCC_AHBLPENR_DMA2LPEN               ((uint32_t)0x02000000U)        /*!< DMA2 clock enabled in sleep mode */

/******************  Bit definition for RCC_APB2LPENR register  ***************/
#define  RCC_APB2LPENR_SYSCFGLPEN            ((uint32_t)0x00000001U)         /*!< System Configuration SYSCFG clock enabled in sleep mode */
#define  RCC_APB2LPENR_TIM9LPEN              ((uint32_t)0x00000004U)         /*!< TIM9 interface clock enabled in sleep mode */
#define  RCC_APB2LPENR_TIM10LPEN             ((uint32_t)0x00000008U)         /*!< TIM10 interface clock enabled in sleep mode */
#define  RCC_APB2LPENR_TIM11LPEN             ((uint32_t)0x00000010U)         /*!< TIM11 Timer clock enabled in sleep mode */
#define  RCC_APB2LPENR_ADC1LPEN              ((uint32_t)0x00000200U)         /*!< ADC1 clock enabled in sleep mode */
#define  RCC_APB2LPENR_SPI1LPEN              ((uint32_t)0x00001000U)         /*!< SPI1 clock enabled in sleep mode */
#define  RCC_APB2LPENR_USART1LPEN            ((uint32_t)0x00004000U)         /*!< USART1 clock enabled in sleep mode */

/*****************  Bit definition for RCC_APB1LPENR register  ****************/
#define  RCC_APB1LPENR_TIM2LPEN              ((uint32_t)0x00000001U)        /*!< Timer 2 clock enabled in sleep mode */
#define  RCC_APB1LPENR_TIM3LPEN              ((uint32_t)0x00000002U)        /*!< Timer 3 clock enabled in sleep mode */
#define  RCC_APB1LPENR_TIM4LPEN              ((uint32_t)0x00000004U)        /*!< Timer 4 clock enabled in sleep mode */
#define  RCC_APB1LPENR_TIM5LPEN              ((uint32_t)0x00000008U)        /*!< Timer 5 clock enabled in sleep mode */
#define  RCC_APB1LPENR_TIM6LPEN              ((uint32_t)0x00000010U)        /*!< Timer 6 clock enabled in sleep mode */
#define  RCC_APB1LPENR_TIM7LPEN              ((uint32_t)0x00000020U)        /*!< Timer 7 clock enabled in sleep mode */
#define  RCC_APB1LPENR_LCDLPEN               ((uint32_t)0x00000200U)        /*!< LCD clock enabled in sleep mode */
#define  RCC_APB1LPENR_WWDGLPEN              ((uint32_t)0x00000800U)        /*!< Window Watchdog clock enabled in sleep mode */
#define  RCC_APB1LPENR_SPI2LPEN              ((uint32_t)0x00004000U)        /*!< SPI 2 clock enabled in sleep mode */
#define  RCC_APB1LPENR_SPI3LPEN              ((uint32_t)0x00008000U)        /*!< SPI 3 clock enabled in sleep mode */
#define  RCC_APB1LPENR_USART2LPEN            ((uint32_t)0x00020000U)        /*!< USART 2 clock enabled in sleep mode */
#define  RCC_APB1LPENR_USART3LPEN            ((uint32_t)0x00040000U)        /*!< USART 3 clock enabled in sleep mode */
#define  RCC_APB1LPENR_UART4LPEN             ((uint32_t)0x00080000U)        /*!< UART 4 clock enabled in sleep mode */
#define  RCC_APB1LPENR_UART5LPEN             ((uint32_t)0x00100000U)        /*!< UART 5 clock enabled in sleep mode */
#define  RCC_APB1LPENR_I2C1LPEN              ((uint32_t)0x00200000U)        /*!< I2C 1 clock enabled in sleep mode */
#define  RCC_APB1LPENR_I2C2LPEN              ((uint32_t)0x00400000U)        /*!< I2C 2 clock enabled in sleep mode */
#define  RCC_APB1LPENR_USBLPEN               ((uint32_t)0x00800000U)        /*!< USB clock enabled in sleep mode */
#define  RCC_APB1LPENR_PWRLPEN               ((uint32_t)0x10000000U)        /*!< Power interface clock enabled in sleep mode */
#define  RCC_APB1LPENR_DACLPEN               ((uint32_t)0x20000000U)        /*!< DAC interface clock enabled in sleep mode */
#define  RCC_APB1LPENR_COMPLPEN              ((uint32_t)0x80000000U)        /*!< Comparator interface clock enabled in sleep mode*/

/*******************  Bit definition for RCC_CSR register  ********************/
#define  RCC_CSR_LSION                      ((uint32_t)0x00000001U)        /*!< Internal Low Speed oscillator enable */
#define  RCC_CSR_LSIRDY                     ((uint32_t)0x00000002U)        /*!< Internal Low Speed oscillator Ready */

#define  RCC_CSR_LSEON                      ((uint32_t)0x00000100U)        /*!< External Low Speed oscillator enable */
#define  RCC_CSR_LSERDY                     ((uint32_t)0x00000200U)        /*!< External Low Speed oscillator Ready */
#define  RCC_CSR_LSEBYP                     ((uint32_t)0x00000400U)        /*!< External Low Speed oscillator Bypass */

#define  RCC_CSR_LSECSSON                   ((uint32_t)0x00000800U)        /*!< External Low Speed oscillator CSS Enable */
#define  RCC_CSR_LSECSSD                    ((uint32_t)0x00001000U)        /*!< External Low Speed oscillator CSS Detected */

#define  RCC_CSR_RTCSEL                     ((uint32_t)0x00030000U)        /*!< RTCSEL[1:0] bits (RTC clock source selection) */
#define  RCC_CSR_RTCSEL_0                   ((uint32_t)0x00010000U)        /*!< Bit 0 */
#define  RCC_CSR_RTCSEL_1                   ((uint32_t)0x00020000U)        /*!< Bit 1 */

/*!< RTC congiguration */
#define  RCC_CSR_RTCSEL_NOCLOCK             ((uint32_t)0x00000000U)        /*!< No clock */
#define  RCC_CSR_RTCSEL_LSE                 ((uint32_t)0x00010000U)        /*!< LSE oscillator clock used as RTC clock */
#define  RCC_CSR_RTCSEL_LSI                 ((uint32_t)0x00020000U)        /*!< LSI oscillator clock used as RTC clock */
#define  RCC_CSR_RTCSEL_HSE                 ((uint32_t)0x00030000U)        /*!< HSE oscillator clock divided by 2, 4, 8 or 16 by RTCPRE used as RTC clock */

#define  RCC_CSR_RTCEN                      ((uint32_t)0x00400000U)        /*!< RTC clock enable */
#define  RCC_CSR_RTCRST                     ((uint32_t)0x00800000U)        /*!< RTC reset  */
 
#define  RCC_CSR_RMVF                       ((uint32_t)0x01000000U)        /*!< Remove reset flag */
#define  RCC_CSR_OBLRSTF                    ((uint32_t)0x02000000U)        /*!< Option Bytes Loader reset flag */
#define  RCC_CSR_PINRSTF                    ((uint32_t)0x04000000U)        /*!< PIN reset flag */
#define  RCC_CSR_PORRSTF                    ((uint32_t)0x08000000U)        /*!< POR/PDR reset flag */
#define  RCC_CSR_SFTRSTF                    ((uint32_t)0x10000000U)        /*!< Software Reset flag */
#define  RCC_CSR_IWDGRSTF                   ((uint32_t)0x20000000U)        /*!< Independent Watchdog reset flag */
#define  RCC_CSR_WWDGRSTF                   ((uint32_t)0x40000000U)        /*!< Window watchdog reset flag */
#define  RCC_CSR_LPWRRSTF                   ((uint32_t)0x80000000U)        /*!< Low-Power reset flag */

/******************************************************************************/
/*                                                                            */
/*                           Real-Time Clock (RTC)                            */
/*                                                                            */
/******************************************************************************/
/*
* @brief Specific device feature definitions  (not present on all devices in the STM32F0 family)
*/
#define RTC_TAMPER1_SUPPORT       /*!< TAMPER 1 feature support */
#define RTC_TAMPER2_SUPPORT       /*!< TAMPER 2 feature support */
#define RTC_TAMPER3_SUPPORT       /*!< TAMPER 3 feature support */
#define RTC_BACKUP_SUPPORT        /*!< BACKUP register feature support */
#define RTC_WAKEUP_SUPPORT        /*!< WAKEUP feature support */
#define RTC_SMOOTHCALIB_SUPPORT   /*!< Smooth digital calibration feature support */
#define RTC_SUBSECOND_SUPPORT     /*!< Sub-second feature support */

/********************  Bits definition for RTC_TR register  *******************/
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

/********************  Bits definition for RTC_DR register  *******************/
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

/********************  Bits definition for RTC_CR register  *******************/
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
#define RTC_CR_WUTIE                         ((uint32_t)0x00004000U)
#define RTC_CR_ALRBIE                        ((uint32_t)0x00002000U)
#define RTC_CR_ALRAIE                        ((uint32_t)0x00001000U)
#define RTC_CR_TSE                           ((uint32_t)0x00000800U)
#define RTC_CR_WUTE                          ((uint32_t)0x00000400U)
#define RTC_CR_ALRBE                         ((uint32_t)0x00000200U)
#define RTC_CR_ALRAE                         ((uint32_t)0x00000100U)
#define RTC_CR_DCE                           ((uint32_t)0x00000080U)
#define RTC_CR_FMT                           ((uint32_t)0x00000040U)
#define RTC_CR_BYPSHAD                       ((uint32_t)0x00000020U)
#define RTC_CR_REFCKON                       ((uint32_t)0x00000010U)
#define RTC_CR_TSEDGE                        ((uint32_t)0x00000008U)
#define RTC_CR_WUCKSEL                       ((uint32_t)0x00000007U)
#define RTC_CR_WUCKSEL_0                     ((uint32_t)0x00000001U)
#define RTC_CR_WUCKSEL_1                     ((uint32_t)0x00000002U)
#define RTC_CR_WUCKSEL_2                     ((uint32_t)0x00000004U)

/********************  Bits definition for RTC_ISR register  ******************/
#define RTC_ISR_RECALPF                      ((uint32_t)0x00010000U)
#define RTC_ISR_TAMP3F                       ((uint32_t)0x00008000U)
#define RTC_ISR_TAMP2F                       ((uint32_t)0x00004000U)
#define RTC_ISR_TAMP1F                       ((uint32_t)0x00002000U)
#define RTC_ISR_TSOVF                        ((uint32_t)0x00001000U)
#define RTC_ISR_TSF                          ((uint32_t)0x00000800U)
#define RTC_ISR_WUTF                         ((uint32_t)0x00000400U)
#define RTC_ISR_ALRBF                        ((uint32_t)0x00000200U)
#define RTC_ISR_ALRAF                        ((uint32_t)0x00000100U)
#define RTC_ISR_INIT                         ((uint32_t)0x00000080U)
#define RTC_ISR_INITF                        ((uint32_t)0x00000040U)
#define RTC_ISR_RSF                          ((uint32_t)0x00000020U)
#define RTC_ISR_INITS                        ((uint32_t)0x00000010U)
#define RTC_ISR_SHPF                         ((uint32_t)0x00000008U)
#define RTC_ISR_WUTWF                        ((uint32_t)0x00000004U)
#define RTC_ISR_ALRBWF                       ((uint32_t)0x00000002U)
#define RTC_ISR_ALRAWF                       ((uint32_t)0x00000001U)

/********************  Bits definition for RTC_PRER register  *****************/
#define RTC_PRER_PREDIV_A                    ((uint32_t)0x007F0000U)
#define RTC_PRER_PREDIV_S                    ((uint32_t)0x00007FFFU)

/********************  Bits definition for RTC_WUTR register  *****************/
#define RTC_WUTR_WUT                         ((uint32_t)0x0000FFFFU)

/********************  Bits definition for RTC_CALIBR register  ***************/
#define RTC_CALIBR_DCS                       ((uint32_t)0x00000080U)
#define RTC_CALIBR_DC                        ((uint32_t)0x0000001FU)

/********************  Bits definition for RTC_ALRMAR register  ***************/
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

/********************  Bits definition for RTC_ALRMBR register  ***************/
#define RTC_ALRMBR_MSK4                      ((uint32_t)0x80000000U)
#define RTC_ALRMBR_WDSEL                     ((uint32_t)0x40000000U)
#define RTC_ALRMBR_DT                        ((uint32_t)0x30000000U)
#define RTC_ALRMBR_DT_0                      ((uint32_t)0x10000000U)
#define RTC_ALRMBR_DT_1                      ((uint32_t)0x20000000U)
#define RTC_ALRMBR_DU                        ((uint32_t)0x0F000000U)
#define RTC_ALRMBR_DU_0                      ((uint32_t)0x01000000U)
#define RTC_ALRMBR_DU_1                      ((uint32_t)0x02000000U)
#define RTC_ALRMBR_DU_2                      ((uint32_t)0x04000000U)
#define RTC_ALRMBR_DU_3                      ((uint32_t)0x08000000U)
#define RTC_ALRMBR_MSK3                      ((uint32_t)0x00800000U)
#define RTC_ALRMBR_PM                        ((uint32_t)0x00400000U)
#define RTC_ALRMBR_HT                        ((uint32_t)0x00300000U)
#define RTC_ALRMBR_HT_0                      ((uint32_t)0x00100000U)
#define RTC_ALRMBR_HT_1                      ((uint32_t)0x00200000U)
#define RTC_ALRMBR_HU                        ((uint32_t)0x000F0000U)
#define RTC_ALRMBR_HU_0                      ((uint32_t)0x00010000U)
#define RTC_ALRMBR_HU_1                      ((uint32_t)0x00020000U)
#define RTC_ALRMBR_HU_2                      ((uint32_t)0x00040000U)
#define RTC_ALRMBR_HU_3                      ((uint32_t)0x00080000U)
#define RTC_ALRMBR_MSK2                      ((uint32_t)0x00008000U)
#define RTC_ALRMBR_MNT                       ((uint32_t)0x00007000U)
#define RTC_ALRMBR_MNT_0                     ((uint32_t)0x00001000U)
#define RTC_ALRMBR_MNT_1                     ((uint32_t)0x00002000U)
#define RTC_ALRMBR_MNT_2                     ((uint32_t)0x00004000U)
#define RTC_ALRMBR_MNU                       ((uint32_t)0x00000F00U)
#define RTC_ALRMBR_MNU_0                     ((uint32_t)0x00000100U)
#define RTC_ALRMBR_MNU_1                     ((uint32_t)0x00000200U)
#define RTC_ALRMBR_MNU_2                     ((uint32_t)0x00000400U)
#define RTC_ALRMBR_MNU_3                     ((uint32_t)0x00000800U)
#define RTC_ALRMBR_MSK1                      ((uint32_t)0x00000080U)
#define RTC_ALRMBR_ST                        ((uint32_t)0x00000070U)
#define RTC_ALRMBR_ST_0                      ((uint32_t)0x00000010U)
#define RTC_ALRMBR_ST_1                      ((uint32_t)0x00000020U)
#define RTC_ALRMBR_ST_2                      ((uint32_t)0x00000040U)
#define RTC_ALRMBR_SU                        ((uint32_t)0x0000000FU)
#define RTC_ALRMBR_SU_0                      ((uint32_t)0x00000001U)
#define RTC_ALRMBR_SU_1                      ((uint32_t)0x00000002U)
#define RTC_ALRMBR_SU_2                      ((uint32_t)0x00000004U)
#define RTC_ALRMBR_SU_3                      ((uint32_t)0x00000008U)

/********************  Bits definition for RTC_WPR register  ******************/
#define RTC_WPR_KEY                          ((uint32_t)0x000000FFU)

/********************  Bits definition for RTC_SSR register  ******************/
#define RTC_SSR_SS                           ((uint32_t)0x0000FFFFU)

/********************  Bits definition for RTC_SHIFTR register  ***************/
#define RTC_SHIFTR_SUBFS                     ((uint32_t)0x00007FFFU)
#define RTC_SHIFTR_ADD1S                     ((uint32_t)0x80000000U)

/********************  Bits definition for RTC_TSTR register  *****************/
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

/********************  Bits definition for RTC_TSDR register  *****************/
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

/********************  Bits definition for RTC_TSSSR register  ****************/
#define RTC_TSSSR_SS                         ((uint32_t)0x0000FFFFU)

/********************  Bits definition for RTC_CAL register  *****************/
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

/********************  Bits definition for RTC_TAFCR register  ****************/
#define RTC_TAFCR_ALARMOUTTYPE               ((uint32_t)0x00040000U)
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
#define RTC_TAFCR_TAMP3TRG                   ((uint32_t)0x00000040U)
#define RTC_TAFCR_TAMP3E                     ((uint32_t)0x00000020U)
#define RTC_TAFCR_TAMP2TRG                   ((uint32_t)0x00000010U)
#define RTC_TAFCR_TAMP2E                     ((uint32_t)0x00000008U)
#define RTC_TAFCR_TAMPIE                     ((uint32_t)0x00000004U)
#define RTC_TAFCR_TAMP1TRG                   ((uint32_t)0x00000002U)
#define RTC_TAFCR_TAMP1E                     ((uint32_t)0x00000001U)

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

/********************  Bits definition for RTC_BKP0R register  ****************/
#define RTC_BKP0R                            ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP1R register  ****************/
#define RTC_BKP1R                            ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP2R register  ****************/
#define RTC_BKP2R                            ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP3R register  ****************/
#define RTC_BKP3R                            ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP4R register  ****************/
#define RTC_BKP4R                            ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP5R register  ****************/
#define RTC_BKP5R                            ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP6R register  ****************/
#define RTC_BKP6R                            ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP7R register  ****************/
#define RTC_BKP7R                            ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP8R register  ****************/
#define RTC_BKP8R                            ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP9R register  ****************/
#define RTC_BKP9R                            ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP10R register  ***************/
#define RTC_BKP10R                           ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP11R register  ***************/
#define RTC_BKP11R                           ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP12R register  ***************/
#define RTC_BKP12R                           ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP13R register  ***************/
#define RTC_BKP13R                           ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP14R register  ***************/
#define RTC_BKP14R                           ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP15R register  ***************/
#define RTC_BKP15R                           ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP16R register  ***************/
#define RTC_BKP16R                           ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP17R register  ***************/
#define RTC_BKP17R                           ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP18R register  ***************/
#define RTC_BKP18R                           ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP19R register  ***************/
#define RTC_BKP19R                           ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP20R register  ***************/
#define RTC_BKP20R                           ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP21R register  ***************/
#define RTC_BKP21R                           ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP22R register  ***************/
#define RTC_BKP22R                           ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP23R register  ***************/
#define RTC_BKP23R                           ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP24R register  ***************/
#define RTC_BKP24R                           ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP25R register  ***************/
#define RTC_BKP25R                           ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP26R register  ***************/
#define RTC_BKP26R                           ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP27R register  ***************/
#define RTC_BKP27R                           ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP28R register  ***************/
#define RTC_BKP28R                           ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP29R register  ***************/
#define RTC_BKP29R                           ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP30R register  ***************/
#define RTC_BKP30R                           ((uint32_t)0xFFFFFFFFU)

/********************  Bits definition for RTC_BKP31R register  ***************/
#define RTC_BKP31R                           ((uint32_t)0xFFFFFFFFU)

/******************** Number of backup registers ******************************/
#define RTC_BKP_NUMBER 32

/******************************************************************************/
/*                                                                            */
/*                     Serial Peripheral Interface (SPI)                      */
/*                                                                            */
/******************************************************************************/

/*
 * @brief Specific device feature definitions (not present on all devices in the STM32F3 family)
 */
#define SPI_I2S_SUPPORT    

/*******************  Bit definition for SPI_CR1 register  ********************/
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
#define  SPI_CR1_DFF                         ((uint32_t)0x00000800U)            /*!< Data Frame Format */
#define  SPI_CR1_CRCNEXT                     ((uint32_t)0x00001000U)            /*!< Transmit CRC next */
#define  SPI_CR1_CRCEN                       ((uint32_t)0x00002000U)            /*!< Hardware CRC calculation enable */
#define  SPI_CR1_BIDIOE                      ((uint32_t)0x00004000U)            /*!< Output enable in bidirectional mode */
#define  SPI_CR1_BIDIMODE                    ((uint32_t)0x00008000U)            /*!< Bidirectional data mode enable */

/*******************  Bit definition for SPI_CR2 register  ********************/
#define  SPI_CR2_RXDMAEN                     ((uint32_t)0x00000001U)               /*!< Rx Buffer DMA Enable */
#define  SPI_CR2_TXDMAEN                     ((uint32_t)0x00000002U)               /*!< Tx Buffer DMA Enable */
#define  SPI_CR2_SSOE                        ((uint32_t)0x00000004U)               /*!< SS Output Enable */
#define  SPI_CR2_FRF                         ((uint32_t)0x00000010U)               /*!< Frame format */
#define  SPI_CR2_ERRIE                       ((uint32_t)0x00000020U)               /*!< Error Interrupt Enable */
#define  SPI_CR2_RXNEIE                      ((uint32_t)0x00000040U)               /*!< RX buffer Not Empty Interrupt Enable */
#define  SPI_CR2_TXEIE                       ((uint32_t)0x00000080U)               /*!< Tx buffer Empty Interrupt Enable */

/********************  Bit definition for SPI_SR register  ********************/
#define  SPI_SR_RXNE                         ((uint32_t)0x00000001U)               /*!< Receive buffer Not Empty */
#define  SPI_SR_TXE                          ((uint32_t)0x00000002U)               /*!< Transmit buffer Empty */
#define  SPI_SR_CHSIDE                       ((uint32_t)0x00000004U)               /*!< Channel side */
#define  SPI_SR_UDR                          ((uint32_t)0x00000008U)               /*!< Underrun flag */
#define  SPI_SR_CRCERR                       ((uint32_t)0x00000010U)               /*!< CRC Error flag */
#define  SPI_SR_MODF                         ((uint32_t)0x00000020U)               /*!< Mode fault */
#define  SPI_SR_OVR                          ((uint32_t)0x00000040U)               /*!< Overrun flag */
#define  SPI_SR_BSY                          ((uint32_t)0x00000080U)               /*!< Busy flag */
#define  SPI_SR_FRE                          ((uint32_t)0x00000100U)               /*!<Frame format error flag  */

/********************  Bit definition for SPI_DR register  ********************/
#define  SPI_DR_DR                           ((uint32_t)0x0000FFFFU)            /*!< Data Register */

/*******************  Bit definition for SPI_CRCPR register  ******************/
#define  SPI_CRCPR_CRCPOLY                   ((uint32_t)0x0000FFFFU)            /*!< CRC polynomial register */

/******************  Bit definition for SPI_RXCRCR register  ******************/
#define  SPI_RXCRCR_RXCRC                    ((uint32_t)0x0000FFFFU)            /*!< Rx CRC Register */

/******************  Bit definition for SPI_TXCRCR register  ******************/
#define  SPI_TXCRCR_TXCRC                    ((uint32_t)0x0000FFFFU)            /*!< Tx CRC Register */

/******************  Bit definition for SPI_I2SCFGR register  *****************/
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

/******************  Bit definition for SPI_I2SPR register  *******************/
#define  SPI_I2SPR_I2SDIV                    ((uint32_t)0x000000FFU)            /*!<I2S Linear prescaler */
#define  SPI_I2SPR_ODD                       ((uint32_t)0x00000100U)            /*!<Odd factor for the prescaler */
#define  SPI_I2SPR_MCKOE                     ((uint32_t)0x00000200U)            /*!<Master Clock Output Enable */

/******************************************************************************/
/*                                                                            */
/*                       System Configuration (SYSCFG)                        */
/*                                                                            */
/******************************************************************************/
/*****************  Bit definition for SYSCFG_MEMRMP register  ****************/
#define SYSCFG_MEMRMP_MEM_MODE          ((uint32_t)0x00000003U) /*!< SYSCFG_Memory Remap Config */
#define SYSCFG_MEMRMP_MEM_MODE_0        ((uint32_t)0x00000001U) /*!< Bit 0 */
#define SYSCFG_MEMRMP_MEM_MODE_1        ((uint32_t)0x00000002U) /*!< Bit 1 */
#define SYSCFG_MEMRMP_BOOT_MODE         ((uint32_t)0x00000300U) /*!< Boot mode Config */
#define SYSCFG_MEMRMP_BOOT_MODE_0       ((uint32_t)0x00000100U) /*!< Bit 0 */
#define SYSCFG_MEMRMP_BOOT_MODE_1       ((uint32_t)0x00000200U) /*!< Bit 1 */

/*****************  Bit definition for SYSCFG_PMC register  *******************/
#define SYSCFG_PMC_USB_PU               ((uint32_t)0x00000001U) /*!< SYSCFG PMC */
#define SYSCFG_PMC_LCD_CAPA             ((uint32_t)0x0000003EU) /*!< LCD_CAPA decoupling capacitance connection */
#define SYSCFG_PMC_LCD_CAPA_0           ((uint32_t)0x00000002U) /*!< Bit 0 */
#define SYSCFG_PMC_LCD_CAPA_1           ((uint32_t)0x00000004U) /*!< Bit 1 */
#define SYSCFG_PMC_LCD_CAPA_2           ((uint32_t)0x00000008U) /*!< Bit 2 */
#define SYSCFG_PMC_LCD_CAPA_3           ((uint32_t)0x00000010U) /*!< Bit 3 */
#define SYSCFG_PMC_LCD_CAPA_4           ((uint32_t)0x00000020U) /*!< Bit 4 */

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
#define SYSCFG_EXTICR1_EXTI0_PD         ((uint32_t)0x00000003U) /*!< PD[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PE         ((uint32_t)0x00000004U) /*!< PE[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PH         ((uint32_t)0x00000005U) /*!< PH[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PF         ((uint32_t)0x00000006U) /*!< PF[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PG         ((uint32_t)0x00000007U) /*!< PG[0] pin */

/** 
  * @brief  EXTI1 configuration  
  */ 
#define SYSCFG_EXTICR1_EXTI1_PA         ((uint32_t)0x00000000U) /*!< PA[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PB         ((uint32_t)0x00000010U) /*!< PB[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PC         ((uint32_t)0x00000020U) /*!< PC[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PD         ((uint32_t)0x00000030U) /*!< PD[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PE         ((uint32_t)0x00000040U) /*!< PE[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PH         ((uint32_t)0x00000050U) /*!< PH[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PF         ((uint32_t)0x00000060U) /*!< PF[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PG         ((uint32_t)0x00000070U) /*!< PG[1] pin */

/** 
  * @brief  EXTI2 configuration  
  */ 
#define SYSCFG_EXTICR1_EXTI2_PA         ((uint32_t)0x00000000U) /*!< PA[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PB         ((uint32_t)0x00000100U) /*!< PB[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PC         ((uint32_t)0x00000200U) /*!< PC[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PD         ((uint32_t)0x00000300U) /*!< PD[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PE         ((uint32_t)0x00000400U) /*!< PE[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PH         ((uint32_t)0x00000500U) /*!< PH[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PF         ((uint32_t)0x00000600U) /*!< PF[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PG         ((uint32_t)0x00000700U) /*!< PG[2] pin */

/** 
  * @brief  EXTI3 configuration  
  */ 
#define SYSCFG_EXTICR1_EXTI3_PA         ((uint32_t)0x00000000U) /*!< PA[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PB         ((uint32_t)0x00001000U) /*!< PB[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PC         ((uint32_t)0x00002000U) /*!< PC[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PD         ((uint32_t)0x00003000U) /*!< PD[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PE         ((uint32_t)0x00004000U) /*!< PE[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PF         ((uint32_t)0x00003000U) /*!< PF[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PG         ((uint32_t)0x00004000U) /*!< PG[3] pin */

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
#define SYSCFG_EXTICR2_EXTI4_PD         ((uint32_t)0x00000003U) /*!< PD[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PE         ((uint32_t)0x00000004U) /*!< PE[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PF         ((uint32_t)0x00000006U) /*!< PF[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PG         ((uint32_t)0x00000007U) /*!< PG[4] pin */

/** 
  * @brief  EXTI5 configuration  
  */ 
#define SYSCFG_EXTICR2_EXTI5_PA         ((uint32_t)0x00000000U) /*!< PA[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PB         ((uint32_t)0x00000010U) /*!< PB[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PC         ((uint32_t)0x00000020U) /*!< PC[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PD         ((uint32_t)0x00000030U) /*!< PD[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PE         ((uint32_t)0x00000040U) /*!< PE[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PF         ((uint32_t)0x00000060U) /*!< PF[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PG         ((uint32_t)0x00000070U) /*!< PG[5] pin */

/** 
  * @brief  EXTI6 configuration  
  */ 
#define SYSCFG_EXTICR2_EXTI6_PA         ((uint32_t)0x00000000U) /*!< PA[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PB         ((uint32_t)0x00000100U) /*!< PB[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PC         ((uint32_t)0x00000200U) /*!< PC[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PD         ((uint32_t)0x00000300U) /*!< PD[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PE         ((uint32_t)0x00000400U) /*!< PE[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PF         ((uint32_t)0x00000600U) /*!< PF[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PG         ((uint32_t)0x00000700U) /*!< PG[6] pin */

/** 
  * @brief  EXTI7 configuration  
  */ 
#define SYSCFG_EXTICR2_EXTI7_PA         ((uint32_t)0x00000000U) /*!< PA[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PB         ((uint32_t)0x00001000U) /*!< PB[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PC         ((uint32_t)0x00002000U) /*!< PC[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PD         ((uint32_t)0x00003000U) /*!< PD[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PE         ((uint32_t)0x00004000U) /*!< PE[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PF         ((uint32_t)0x00006000U) /*!< PF[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PG         ((uint32_t)0x00007000U) /*!< PG[7] pin */

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
#define SYSCFG_EXTICR3_EXTI8_PD         ((uint32_t)0x00000003U) /*!< PD[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PE         ((uint32_t)0x00000004U) /*!< PE[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PF         ((uint32_t)0x00000006U) /*!< PF[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PG         ((uint32_t)0x00000007U) /*!< PG[8] pin */

/** 
  * @brief  EXTI9 configuration  
  */ 
#define SYSCFG_EXTICR3_EXTI9_PA         ((uint32_t)0x00000000U) /*!< PA[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PB         ((uint32_t)0x00000010U) /*!< PB[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PC         ((uint32_t)0x00000020U) /*!< PC[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PD         ((uint32_t)0x00000030U) /*!< PD[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PE         ((uint32_t)0x00000040U) /*!< PE[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PF         ((uint32_t)0x00000060U) /*!< PF[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PG         ((uint32_t)0x00000070U) /*!< PG[9] pin */

/** 
  * @brief  EXTI10 configuration  
  */ 
#define SYSCFG_EXTICR3_EXTI10_PA        ((uint32_t)0x00000000U) /*!< PA[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PB        ((uint32_t)0x00000100U) /*!< PB[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PC        ((uint32_t)0x00000200U) /*!< PC[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PD        ((uint32_t)0x00000300U) /*!< PD[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PE        ((uint32_t)0x00000400U) /*!< PE[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PF        ((uint32_t)0x00000600U) /*!< PF[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PG        ((uint32_t)0x00000700U) /*!< PG[10] pin */

/** 
  * @brief  EXTI11 configuration  
  */ 
#define SYSCFG_EXTICR3_EXTI11_PA        ((uint32_t)0x00000000U) /*!< PA[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PB        ((uint32_t)0x00001000U) /*!< PB[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PC        ((uint32_t)0x00002000U) /*!< PC[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PD        ((uint32_t)0x00003000U) /*!< PD[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PE        ((uint32_t)0x00004000U) /*!< PE[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PF        ((uint32_t)0x00006000U) /*!< PF[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PG        ((uint32_t)0x00007000U) /*!< PG[11] pin */

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
#define SYSCFG_EXTICR4_EXTI12_PD        ((uint32_t)0x00000003U) /*!< PD[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PE        ((uint32_t)0x00000004U) /*!< PE[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PF        ((uint32_t)0x00000006U) /*!< PF[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PG        ((uint32_t)0x00000007U) /*!< PG[12] pin */

/** 
  * @brief  EXTI13 configuration  
  */ 
#define SYSCFG_EXTICR4_EXTI13_PA        ((uint32_t)0x00000000U) /*!< PA[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PB        ((uint32_t)0x00000010U) /*!< PB[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PC        ((uint32_t)0x00000020U) /*!< PC[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PD        ((uint32_t)0x00000030U) /*!< PD[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PE        ((uint32_t)0x00000040U) /*!< PE[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PF        ((uint32_t)0x00000060U) /*!< PF[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PG        ((uint32_t)0x00000070U) /*!< PG[13] pin */

/** 
  * @brief  EXTI14 configuration  
  */ 
#define SYSCFG_EXTICR4_EXTI14_PA        ((uint32_t)0x00000000U) /*!< PA[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PB        ((uint32_t)0x00000100U) /*!< PB[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PC        ((uint32_t)0x00000200U) /*!< PC[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PD        ((uint32_t)0x00000300U) /*!< PD[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PE        ((uint32_t)0x00000400U) /*!< PE[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PF        ((uint32_t)0x00000600U) /*!< PF[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PG        ((uint32_t)0x00000700U) /*!< PG[14] pin */

/** 
  * @brief  EXTI15 configuration  
  */ 
#define SYSCFG_EXTICR4_EXTI15_PA        ((uint32_t)0x00000000U) /*!< PA[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PB        ((uint32_t)0x00001000U) /*!< PB[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PC        ((uint32_t)0x00002000U) /*!< PC[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PD        ((uint32_t)0x00003000U) /*!< PD[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PE        ((uint32_t)0x00004000U) /*!< PE[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PF        ((uint32_t)0x00006000U) /*!< PF[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PG        ((uint32_t)0x00007000U) /*!< PG[15] pin */
 
/******************************************************************************/
/*                                                                            */
/*                       Routing Interface (RI)                               */
/*                                                                            */
/******************************************************************************/

/********************  Bit definition for RI_ICR register  ********************/
#define  RI_ICR_IC1OS                    ((uint32_t)0x0000000FU) /*!< IC1OS[3:0] bits (Input Capture 1 select bits) */
#define  RI_ICR_IC1OS_0                  ((uint32_t)0x00000001U) /*!< Bit 0 */
#define  RI_ICR_IC1OS_1                  ((uint32_t)0x00000002U) /*!< Bit 1 */
#define  RI_ICR_IC1OS_2                  ((uint32_t)0x00000004U) /*!< Bit 2 */
#define  RI_ICR_IC1OS_3                  ((uint32_t)0x00000008U) /*!< Bit 3 */

#define  RI_ICR_IC2OS                    ((uint32_t)0x000000F0U) /*!< IC2OS[3:0] bits (Input Capture 2 select bits) */
#define  RI_ICR_IC2OS_0                  ((uint32_t)0x00000010U) /*!< Bit 0 */
#define  RI_ICR_IC2OS_1                  ((uint32_t)0x00000020U) /*!< Bit 1 */
#define  RI_ICR_IC2OS_2                  ((uint32_t)0x00000040U) /*!< Bit 2 */
#define  RI_ICR_IC2OS_3                  ((uint32_t)0x00000080U) /*!< Bit 3 */

#define  RI_ICR_IC3OS                    ((uint32_t)0x00000F00U) /*!< IC3OS[3:0] bits (Input Capture 3 select bits) */
#define  RI_ICR_IC3OS_0                  ((uint32_t)0x00000100U) /*!< Bit 0 */
#define  RI_ICR_IC3OS_1                  ((uint32_t)0x00000200U) /*!< Bit 1 */
#define  RI_ICR_IC3OS_2                  ((uint32_t)0x00000400U) /*!< Bit 2 */
#define  RI_ICR_IC3OS_3                  ((uint32_t)0x00000800U) /*!< Bit 3 */

#define  RI_ICR_IC4OS                    ((uint32_t)0x0000F000U) /*!< IC4OS[3:0] bits (Input Capture 4 select bits) */
#define  RI_ICR_IC4OS_0                  ((uint32_t)0x00001000U) /*!< Bit 0 */
#define  RI_ICR_IC4OS_1                  ((uint32_t)0x00002000U) /*!< Bit 1 */
#define  RI_ICR_IC4OS_2                  ((uint32_t)0x00004000U) /*!< Bit 2 */
#define  RI_ICR_IC4OS_3                  ((uint32_t)0x00008000U) /*!< Bit 3 */

#define  RI_ICR_TIM                     ((uint32_t)0x00030000U) /*!< TIM[3:0] bits (Timers select bits) */
#define  RI_ICR_TIM_0                   ((uint32_t)0x00010000U) /*!< Bit 0 */
#define  RI_ICR_TIM_1                   ((uint32_t)0x00020000U) /*!< Bit 1 */

#define  RI_ICR_IC1                     ((uint32_t)0x00040000U) /*!< Input capture 1 */
#define  RI_ICR_IC2                     ((uint32_t)0x00080000U) /*!< Input capture 2 */
#define  RI_ICR_IC3                     ((uint32_t)0x00100000U) /*!< Input capture 3 */
#define  RI_ICR_IC4                     ((uint32_t)0x00200000U) /*!< Input capture 4 */

/********************  Bit definition for RI_ASCR1 register  ********************/
#define  RI_ASCR1_CH                    ((uint32_t)0x7BFDFFFFU) /*!< AS_CH[25:18] & AS_CH[15:0] bits ( Analog switches selection bits) */
#define  RI_ASCR1_CH_0                  ((uint32_t)0x00000001U) /*!< Bit 0 */
#define  RI_ASCR1_CH_1                  ((uint32_t)0x00000002U) /*!< Bit 1 */
#define  RI_ASCR1_CH_2                  ((uint32_t)0x00000004U) /*!< Bit 2 */
#define  RI_ASCR1_CH_3                  ((uint32_t)0x00000008U) /*!< Bit 3 */
#define  RI_ASCR1_CH_4                  ((uint32_t)0x00000010U) /*!< Bit 4 */
#define  RI_ASCR1_CH_5                  ((uint32_t)0x00000020U) /*!< Bit 5 */
#define  RI_ASCR1_CH_6                  ((uint32_t)0x00000040U) /*!< Bit 6 */
#define  RI_ASCR1_CH_7                  ((uint32_t)0x00000080U) /*!< Bit 7 */
#define  RI_ASCR1_CH_8                  ((uint32_t)0x00000100U) /*!< Bit 8 */
#define  RI_ASCR1_CH_9                  ((uint32_t)0x00000200U) /*!< Bit 9 */
#define  RI_ASCR1_CH_10                 ((uint32_t)0x00000400U) /*!< Bit 10 */
#define  RI_ASCR1_CH_11                 ((uint32_t)0x00000800U) /*!< Bit 11 */
#define  RI_ASCR1_CH_12                 ((uint32_t)0x00001000U) /*!< Bit 12 */
#define  RI_ASCR1_CH_13                 ((uint32_t)0x00002000U) /*!< Bit 13 */
#define  RI_ASCR1_CH_14                 ((uint32_t)0x00004000U) /*!< Bit 14 */
#define  RI_ASCR1_CH_15                 ((uint32_t)0x00008000U) /*!< Bit 15 */
#define  RI_ASCR1_CH_31                 ((uint32_t)0x00010000U) /*!< Bit 16 */
#define  RI_ASCR1_CH_18                 ((uint32_t)0x00040000U) /*!< Bit 18 */
#define  RI_ASCR1_CH_19                 ((uint32_t)0x00080000U) /*!< Bit 19 */
#define  RI_ASCR1_CH_20                 ((uint32_t)0x00100000U) /*!< Bit 20 */
#define  RI_ASCR1_CH_21                 ((uint32_t)0x00200000U) /*!< Bit 21 */
#define  RI_ASCR1_CH_22                 ((uint32_t)0x00400000U) /*!< Bit 22 */
#define  RI_ASCR1_CH_23                 ((uint32_t)0x00800000U) /*!< Bit 23 */
#define  RI_ASCR1_CH_24                 ((uint32_t)0x01000000U) /*!< Bit 24 */
#define  RI_ASCR1_CH_25                 ((uint32_t)0x02000000U) /*!< Bit 25 */
#define  RI_ASCR1_VCOMP                 ((uint32_t)0x04000000U) /*!< ADC analog switch selection for internal node to COMP1 */
#define  RI_ASCR1_CH_27                 ((uint32_t)0x08000000U) /*!< Bit 27 */
#define  RI_ASCR1_CH_28                 ((uint32_t)0x10000000U) /*!< Bit 28 */
#define  RI_ASCR1_CH_29                 ((uint32_t)0x20000000U) /*!< Bit 29 */
#define  RI_ASCR1_CH_30                 ((uint32_t)0x40000000U) /*!< Bit 30 */
#define  RI_ASCR1_SCM                   ((uint32_t)0x80000000U) /*!< I/O Switch control mode */

/********************  Bit definition for RI_ASCR2 register  ********************/
#define  RI_ASCR2_GR10_1                ((uint32_t)0x00000001U) /*!< GR10-1 selection bit */
#define  RI_ASCR2_GR10_2                ((uint32_t)0x00000002U) /*!< GR10-2 selection bit */
#define  RI_ASCR2_GR10_3                ((uint32_t)0x00000004U) /*!< GR10-3 selection bit */
#define  RI_ASCR2_GR10_4                ((uint32_t)0x00000008U) /*!< GR10-4 selection bit */
#define  RI_ASCR2_GR6_1                 ((uint32_t)0x00000010U) /*!< GR6-1 selection bit */
#define  RI_ASCR2_GR6_2                 ((uint32_t)0x00000020U) /*!< GR6-2 selection bit */
#define  RI_ASCR2_GR5_1                 ((uint32_t)0x00000040U) /*!< GR5-1 selection bit */
#define  RI_ASCR2_GR5_2                 ((uint32_t)0x00000080U) /*!< GR5-2 selection bit */
#define  RI_ASCR2_GR5_3                 ((uint32_t)0x00000100U) /*!< GR5-3 selection bit */
#define  RI_ASCR2_GR4_1                 ((uint32_t)0x00000200U) /*!< GR4-1 selection bit */
#define  RI_ASCR2_GR4_2                 ((uint32_t)0x00000400U) /*!< GR4-2 selection bit */
#define  RI_ASCR2_GR4_3                 ((uint32_t)0x00000800U) /*!< GR4-3 selection bit */
#define  RI_ASCR2_GR4_4                 ((uint32_t)0x00008000U) /*!< GR4-4 selection bit */
#define  RI_ASCR2_CH0b                  ((uint32_t)0x00010000U) /*!< CH0b selection bit */
#define  RI_ASCR2_CH1b                  ((uint32_t)0x00020000U) /*!< CH1b selection bit */
#define  RI_ASCR2_CH2b                  ((uint32_t)0x00040000U) /*!< CH2b selection bit */
#define  RI_ASCR2_CH3b                  ((uint32_t)0x00080000U) /*!< CH3b selection bit */
#define  RI_ASCR2_CH6b                  ((uint32_t)0x00100000U) /*!< CH6b selection bit */
#define  RI_ASCR2_CH7b                  ((uint32_t)0x00200000U) /*!< CH7b selection bit */
#define  RI_ASCR2_CH8b                  ((uint32_t)0x00400000U) /*!< CH8b selection bit */
#define  RI_ASCR2_CH9b                  ((uint32_t)0x00800000U) /*!< CH9b selection bit */
#define  RI_ASCR2_CH10b                 ((uint32_t)0x01000000U) /*!< CH10b selection bit */
#define  RI_ASCR2_CH11b                 ((uint32_t)0x02000000U) /*!< CH11b selection bit */
#define  RI_ASCR2_CH12b                 ((uint32_t)0x04000000U) /*!< CH12b selection bit */
#define  RI_ASCR2_GR6_3                 ((uint32_t)0x08000000U) /*!< GR6-3 selection bit */
#define  RI_ASCR2_GR6_4                 ((uint32_t)0x10000000U) /*!< GR6-4 selection bit */

/********************  Bit definition for RI_HYSCR1 register  ********************/
#define  RI_HYSCR1_PA                   ((uint32_t)0x0000FFFFU) /*!< PA[15:0] Port A Hysteresis selection */
#define  RI_HYSCR1_PA_0                 ((uint32_t)0x00000001U) /*!< Bit 0 */
#define  RI_HYSCR1_PA_1                 ((uint32_t)0x00000002U) /*!< Bit 1 */
#define  RI_HYSCR1_PA_2                 ((uint32_t)0x00000004U) /*!< Bit 2 */
#define  RI_HYSCR1_PA_3                 ((uint32_t)0x00000008U) /*!< Bit 3 */
#define  RI_HYSCR1_PA_4                 ((uint32_t)0x00000010U) /*!< Bit 4 */
#define  RI_HYSCR1_PA_5                 ((uint32_t)0x00000020U) /*!< Bit 5 */
#define  RI_HYSCR1_PA_6                 ((uint32_t)0x00000040U) /*!< Bit 6 */
#define  RI_HYSCR1_PA_7                 ((uint32_t)0x00000080U) /*!< Bit 7 */
#define  RI_HYSCR1_PA_8                 ((uint32_t)0x00000100U) /*!< Bit 8 */
#define  RI_HYSCR1_PA_9                 ((uint32_t)0x00000200U) /*!< Bit 9 */
#define  RI_HYSCR1_PA_10                ((uint32_t)0x00000400U) /*!< Bit 10 */
#define  RI_HYSCR1_PA_11                ((uint32_t)0x00000800U) /*!< Bit 11 */
#define  RI_HYSCR1_PA_12                ((uint32_t)0x00001000U) /*!< Bit 12 */
#define  RI_HYSCR1_PA_13                ((uint32_t)0x00002000U) /*!< Bit 13 */
#define  RI_HYSCR1_PA_14                ((uint32_t)0x00004000U) /*!< Bit 14 */
#define  RI_HYSCR1_PA_15                ((uint32_t)0x00008000U) /*!< Bit 15 */

#define  RI_HYSCR1_PB                   ((uint32_t)0xFFFF0000U) /*!< PB[15:0] Port B Hysteresis selection */
#define  RI_HYSCR1_PB_0                 ((uint32_t)0x00010000U) /*!< Bit 0 */
#define  RI_HYSCR1_PB_1                 ((uint32_t)0x00020000U) /*!< Bit 1 */
#define  RI_HYSCR1_PB_2                 ((uint32_t)0x00040000U) /*!< Bit 2 */
#define  RI_HYSCR1_PB_3                 ((uint32_t)0x00080000U) /*!< Bit 3 */
#define  RI_HYSCR1_PB_4                 ((uint32_t)0x00100000U) /*!< Bit 4 */
#define  RI_HYSCR1_PB_5                 ((uint32_t)0x00200000U) /*!< Bit 5 */
#define  RI_HYSCR1_PB_6                 ((uint32_t)0x00400000U) /*!< Bit 6 */
#define  RI_HYSCR1_PB_7                 ((uint32_t)0x00800000U) /*!< Bit 7 */
#define  RI_HYSCR1_PB_8                 ((uint32_t)0x01000000U) /*!< Bit 8 */
#define  RI_HYSCR1_PB_9                 ((uint32_t)0x02000000U) /*!< Bit 9 */
#define  RI_HYSCR1_PB_10                ((uint32_t)0x04000000U) /*!< Bit 10 */
#define  RI_HYSCR1_PB_11                ((uint32_t)0x08000000U) /*!< Bit 11 */
#define  RI_HYSCR1_PB_12                ((uint32_t)0x10000000U) /*!< Bit 12 */
#define  RI_HYSCR1_PB_13                ((uint32_t)0x20000000U) /*!< Bit 13 */
#define  RI_HYSCR1_PB_14                ((uint32_t)0x40000000U) /*!< Bit 14 */
#define  RI_HYSCR1_PB_15                ((uint32_t)0x80000000U) /*!< Bit 15 */

/********************  Bit definition for RI_HYSCR2 register  ********************/
#define  RI_HYSCR2_PC                   ((uint32_t)0x0000FFFFU) /*!< PC[15:0] Port C Hysteresis selection */
#define  RI_HYSCR2_PC_0                 ((uint32_t)0x00000001U) /*!< Bit 0 */
#define  RI_HYSCR2_PC_1                 ((uint32_t)0x00000002U) /*!< Bit 1 */
#define  RI_HYSCR2_PC_2                 ((uint32_t)0x00000004U) /*!< Bit 2 */
#define  RI_HYSCR2_PC_3                 ((uint32_t)0x00000008U) /*!< Bit 3 */
#define  RI_HYSCR2_PC_4                 ((uint32_t)0x00000010U) /*!< Bit 4 */
#define  RI_HYSCR2_PC_5                 ((uint32_t)0x00000020U) /*!< Bit 5 */
#define  RI_HYSCR2_PC_6                 ((uint32_t)0x00000040U) /*!< Bit 6 */
#define  RI_HYSCR2_PC_7                 ((uint32_t)0x00000080U) /*!< Bit 7 */
#define  RI_HYSCR2_PC_8                 ((uint32_t)0x00000100U) /*!< Bit 8 */
#define  RI_HYSCR2_PC_9                 ((uint32_t)0x00000200U) /*!< Bit 9 */
#define  RI_HYSCR2_PC_10                ((uint32_t)0x00000400U) /*!< Bit 10 */
#define  RI_HYSCR2_PC_11                ((uint32_t)0x00000800U) /*!< Bit 11 */
#define  RI_HYSCR2_PC_12                ((uint32_t)0x00001000U) /*!< Bit 12 */
#define  RI_HYSCR2_PC_13                ((uint32_t)0x00002000U) /*!< Bit 13 */
#define  RI_HYSCR2_PC_14                ((uint32_t)0x00004000U) /*!< Bit 14 */
#define  RI_HYSCR2_PC_15                ((uint32_t)0x00008000U) /*!< Bit 15 */

#define  RI_HYSCR2_PD                   ((uint32_t)0xFFFF0000U) /*!< PD[15:0] Port D Hysteresis selection */
#define  RI_HYSCR2_PD_0                 ((uint32_t)0x00010000U) /*!< Bit 0 */
#define  RI_HYSCR2_PD_1                 ((uint32_t)0x00020000U) /*!< Bit 1 */
#define  RI_HYSCR2_PD_2                 ((uint32_t)0x00040000U) /*!< Bit 2 */
#define  RI_HYSCR2_PD_3                 ((uint32_t)0x00080000U) /*!< Bit 3 */
#define  RI_HYSCR2_PD_4                 ((uint32_t)0x00100000U) /*!< Bit 4 */
#define  RI_HYSCR2_PD_5                 ((uint32_t)0x00200000U) /*!< Bit 5 */
#define  RI_HYSCR2_PD_6                 ((uint32_t)0x00400000U) /*!< Bit 6 */
#define  RI_HYSCR2_PD_7                 ((uint32_t)0x00800000U) /*!< Bit 7 */
#define  RI_HYSCR2_PD_8                 ((uint32_t)0x01000000U) /*!< Bit 8 */
#define  RI_HYSCR2_PD_9                 ((uint32_t)0x02000000U) /*!< Bit 9 */
#define  RI_HYSCR2_PD_10                ((uint32_t)0x04000000U) /*!< Bit 10 */
#define  RI_HYSCR2_PD_11                ((uint32_t)0x08000000U) /*!< Bit 11 */
#define  RI_HYSCR2_PD_12                ((uint32_t)0x10000000U) /*!< Bit 12 */
#define  RI_HYSCR2_PD_13                ((uint32_t)0x20000000U) /*!< Bit 13 */
#define  RI_HYSCR2_PD_14                ((uint32_t)0x40000000U) /*!< Bit 14 */
#define  RI_HYSCR2_PD_15                ((uint32_t)0x80000000U) /*!< Bit 15 */

/********************  Bit definition for RI_HYSCR3 register  ********************/
#define  RI_HYSCR3_PE                   ((uint32_t)0x0000FFFFU) /*!< PE[15:0] Port E Hysteresis selection */
#define  RI_HYSCR3_PE_0                 ((uint32_t)0x00000001U) /*!< Bit 0 */
#define  RI_HYSCR3_PE_1                 ((uint32_t)0x00000002U) /*!< Bit 1 */
#define  RI_HYSCR3_PE_2                 ((uint32_t)0x00000004U) /*!< Bit 2 */
#define  RI_HYSCR3_PE_3                 ((uint32_t)0x00000008U) /*!< Bit 3 */
#define  RI_HYSCR3_PE_4                 ((uint32_t)0x00000010U) /*!< Bit 4 */
#define  RI_HYSCR3_PE_5                 ((uint32_t)0x00000020U) /*!< Bit 5 */
#define  RI_HYSCR3_PE_6                 ((uint32_t)0x00000040U) /*!< Bit 6 */
#define  RI_HYSCR3_PE_7                 ((uint32_t)0x00000080U) /*!< Bit 7 */
#define  RI_HYSCR3_PE_8                 ((uint32_t)0x00000100U) /*!< Bit 8 */
#define  RI_HYSCR3_PE_9                 ((uint32_t)0x00000200U) /*!< Bit 9 */
#define  RI_HYSCR3_PE_10                ((uint32_t)0x00000400U) /*!< Bit 10 */
#define  RI_HYSCR3_PE_11                ((uint32_t)0x00000800U) /*!< Bit 11 */
#define  RI_HYSCR3_PE_12                ((uint32_t)0x00001000U) /*!< Bit 12 */
#define  RI_HYSCR3_PE_13                ((uint32_t)0x00002000U) /*!< Bit 13 */
#define  RI_HYSCR3_PE_14                ((uint32_t)0x00004000U) /*!< Bit 14 */
#define  RI_HYSCR3_PE_15                ((uint32_t)0x00008000U) /*!< Bit 15 */
#define  RI_HYSCR3_PF                   ((uint32_t)0xFFFF0000U) /*!< PF[15:0] Port F Hysteresis selection */
#define  RI_HYSCR3_PF_0                 ((uint32_t)0x00010000U) /*!< Bit 0 */
#define  RI_HYSCR3_PF_1                 ((uint32_t)0x00020000U) /*!< Bit 1 */
#define  RI_HYSCR3_PF_2                 ((uint32_t)0x00040000U) /*!< Bit 2 */
#define  RI_HYSCR3_PF_3                 ((uint32_t)0x00080000U) /*!< Bit 3 */
#define  RI_HYSCR3_PF_4                 ((uint32_t)0x00100000U) /*!< Bit 4 */
#define  RI_HYSCR3_PF_5                 ((uint32_t)0x00200000U) /*!< Bit 5 */
#define  RI_HYSCR3_PF_6                 ((uint32_t)0x00400000U) /*!< Bit 6 */
#define  RI_HYSCR3_PF_7                 ((uint32_t)0x00800000U) /*!< Bit 7 */
#define  RI_HYSCR3_PF_8                 ((uint32_t)0x01000000U) /*!< Bit 8 */
#define  RI_HYSCR3_PF_9                 ((uint32_t)0x02000000U) /*!< Bit 9 */
#define  RI_HYSCR3_PF_10                ((uint32_t)0x04000000U) /*!< Bit 10 */
#define  RI_HYSCR3_PF_11                ((uint32_t)0x08000000U) /*!< Bit 11 */
#define  RI_HYSCR3_PF_12                ((uint32_t)0x10000000U) /*!< Bit 12 */
#define  RI_HYSCR3_PF_13                ((uint32_t)0x20000000U) /*!< Bit 13 */
#define  RI_HYSCR3_PF_14                ((uint32_t)0x40000000U) /*!< Bit 14 */
#define  RI_HYSCR3_PF_15                ((uint32_t)0x80000000U) /*!< Bit 15 */

/********************  Bit definition for RI_HYSCR4 register  ********************/
#define  RI_HYSCR4_PG                   ((uint32_t)0x0000FFFFU) /*!< PG[15:0] Port G Hysteresis selection */
#define  RI_HYSCR4_PG_0                 ((uint32_t)0x00000001U) /*!< Bit 0 */
#define  RI_HYSCR4_PG_1                 ((uint32_t)0x00000002U) /*!< Bit 1 */
#define  RI_HYSCR4_PG_2                 ((uint32_t)0x00000004U) /*!< Bit 2 */
#define  RI_HYSCR4_PG_3                 ((uint32_t)0x00000008U) /*!< Bit 3 */
#define  RI_HYSCR4_PG_4                 ((uint32_t)0x00000010U) /*!< Bit 4 */
#define  RI_HYSCR4_PG_5                 ((uint32_t)0x00000020U) /*!< Bit 5 */
#define  RI_HYSCR4_PG_6                 ((uint32_t)0x00000040U) /*!< Bit 6 */
#define  RI_HYSCR4_PG_7                 ((uint32_t)0x00000080U) /*!< Bit 7 */
#define  RI_HYSCR4_PG_8                 ((uint32_t)0x00000100U) /*!< Bit 8 */
#define  RI_HYSCR4_PG_9                 ((uint32_t)0x00000200U) /*!< Bit 9 */
#define  RI_HYSCR4_PG_10                ((uint32_t)0x00000400U) /*!< Bit 10 */
#define  RI_HYSCR4_PG_11                ((uint32_t)0x00000800U) /*!< Bit 11 */
#define  RI_HYSCR4_PG_12                ((uint32_t)0x00001000U) /*!< Bit 12 */
#define  RI_HYSCR4_PG_13                ((uint32_t)0x00002000U) /*!< Bit 13 */
#define  RI_HYSCR4_PG_14                ((uint32_t)0x00004000U) /*!< Bit 14 */
#define  RI_HYSCR4_PG_15                ((uint32_t)0x00008000U) /*!< Bit 15 */

/********************  Bit definition for RI_ASMR1 register  ********************/
#define  RI_ASMR1_PA                   ((uint32_t)0x0000FFFFU) /*!< PA[15:0] Port A selection*/
#define  RI_ASMR1_PA_0                 ((uint32_t)0x00000001U) /*!< Bit 0 */
#define  RI_ASMR1_PA_1                 ((uint32_t)0x00000002U) /*!< Bit 1 */
#define  RI_ASMR1_PA_2                 ((uint32_t)0x00000004U) /*!< Bit 2 */
#define  RI_ASMR1_PA_3                 ((uint32_t)0x00000008U) /*!< Bit 3 */
#define  RI_ASMR1_PA_4                 ((uint32_t)0x00000010U) /*!< Bit 4 */
#define  RI_ASMR1_PA_5                 ((uint32_t)0x00000020U) /*!< Bit 5 */
#define  RI_ASMR1_PA_6                 ((uint32_t)0x00000040U) /*!< Bit 6 */
#define  RI_ASMR1_PA_7                 ((uint32_t)0x00000080U) /*!< Bit 7 */
#define  RI_ASMR1_PA_8                 ((uint32_t)0x00000100U) /*!< Bit 8 */
#define  RI_ASMR1_PA_9                 ((uint32_t)0x00000200U) /*!< Bit 9 */
#define  RI_ASMR1_PA_10                ((uint32_t)0x00000400U) /*!< Bit 10 */
#define  RI_ASMR1_PA_11                ((uint32_t)0x00000800U) /*!< Bit 11 */
#define  RI_ASMR1_PA_12                ((uint32_t)0x00001000U) /*!< Bit 12 */
#define  RI_ASMR1_PA_13                ((uint32_t)0x00002000U) /*!< Bit 13 */
#define  RI_ASMR1_PA_14                ((uint32_t)0x00004000U) /*!< Bit 14 */
#define  RI_ASMR1_PA_15                ((uint32_t)0x00008000U) /*!< Bit 15 */

/********************  Bit definition for RI_CMR1 register  ********************/
#define  RI_CMR1_PA                    ((uint32_t)0x0000FFFFU) /*!< PA[15:0] Port A selection*/
#define  RI_CMR1_PA_0                  ((uint32_t)0x00000001U) /*!< Bit 0 */
#define  RI_CMR1_PA_1                  ((uint32_t)0x00000002U) /*!< Bit 1 */
#define  RI_CMR1_PA_2                  ((uint32_t)0x00000004U) /*!< Bit 2 */
#define  RI_CMR1_PA_3                  ((uint32_t)0x00000008U) /*!< Bit 3 */
#define  RI_CMR1_PA_4                  ((uint32_t)0x00000010U) /*!< Bit 4 */
#define  RI_CMR1_PA_5                  ((uint32_t)0x00000020U) /*!< Bit 5 */
#define  RI_CMR1_PA_6                  ((uint32_t)0x00000040U) /*!< Bit 6 */
#define  RI_CMR1_PA_7                  ((uint32_t)0x00000080U) /*!< Bit 7 */
#define  RI_CMR1_PA_8                  ((uint32_t)0x00000100U) /*!< Bit 8 */
#define  RI_CMR1_PA_9                  ((uint32_t)0x00000200U) /*!< Bit 9 */
#define  RI_CMR1_PA_10                 ((uint32_t)0x00000400U) /*!< Bit 10 */
#define  RI_CMR1_PA_11                 ((uint32_t)0x00000800U) /*!< Bit 11 */
#define  RI_CMR1_PA_12                 ((uint32_t)0x00001000U) /*!< Bit 12 */
#define  RI_CMR1_PA_13                 ((uint32_t)0x00002000U) /*!< Bit 13 */
#define  RI_CMR1_PA_14                 ((uint32_t)0x00004000U) /*!< Bit 14 */
#define  RI_CMR1_PA_15                 ((uint32_t)0x00008000U) /*!< Bit 15 */

/********************  Bit definition for RI_CICR1 register  ********************/
#define  RI_CICR1_PA                   ((uint32_t)0x0000FFFFU) /*!< PA[15:0] Port A selection*/
#define  RI_CICR1_PA_0                 ((uint32_t)0x00000001U) /*!< Bit 0 */
#define  RI_CICR1_PA_1                 ((uint32_t)0x00000002U) /*!< Bit 1 */
#define  RI_CICR1_PA_2                 ((uint32_t)0x00000004U) /*!< Bit 2 */
#define  RI_CICR1_PA_3                 ((uint32_t)0x00000008U) /*!< Bit 3 */
#define  RI_CICR1_PA_4                 ((uint32_t)0x00000010U) /*!< Bit 4 */
#define  RI_CICR1_PA_5                 ((uint32_t)0x00000020U) /*!< Bit 5 */
#define  RI_CICR1_PA_6                 ((uint32_t)0x00000040U) /*!< Bit 6 */
#define  RI_CICR1_PA_7                 ((uint32_t)0x00000080U) /*!< Bit 7 */
#define  RI_CICR1_PA_8                 ((uint32_t)0x00000100U) /*!< Bit 8 */
#define  RI_CICR1_PA_9                 ((uint32_t)0x00000200U) /*!< Bit 9 */
#define  RI_CICR1_PA_10                ((uint32_t)0x00000400U) /*!< Bit 10 */
#define  RI_CICR1_PA_11                ((uint32_t)0x00000800U) /*!< Bit 11 */
#define  RI_CICR1_PA_12                ((uint32_t)0x00001000U) /*!< Bit 12 */
#define  RI_CICR1_PA_13                ((uint32_t)0x00002000U) /*!< Bit 13 */
#define  RI_CICR1_PA_14                ((uint32_t)0x00004000U) /*!< Bit 14 */
#define  RI_CICR1_PA_15                ((uint32_t)0x00008000U) /*!< Bit 15 */

/********************  Bit definition for RI_ASMR2 register  ********************/
#define  RI_ASMR2_PB                   ((uint32_t)0x0000FFFFU) /*!< PB[15:0] Port B selection */
#define  RI_ASMR2_PB_0                 ((uint32_t)0x00000001U) /*!< Bit 0 */
#define  RI_ASMR2_PB_1                 ((uint32_t)0x00000002U) /*!< Bit 1 */
#define  RI_ASMR2_PB_2                 ((uint32_t)0x00000004U) /*!< Bit 2 */
#define  RI_ASMR2_PB_3                 ((uint32_t)0x00000008U) /*!< Bit 3 */
#define  RI_ASMR2_PB_4                 ((uint32_t)0x00000010U) /*!< Bit 4 */
#define  RI_ASMR2_PB_5                 ((uint32_t)0x00000020U) /*!< Bit 5 */
#define  RI_ASMR2_PB_6                 ((uint32_t)0x00000040U) /*!< Bit 6 */
#define  RI_ASMR2_PB_7                 ((uint32_t)0x00000080U) /*!< Bit 7 */
#define  RI_ASMR2_PB_8                 ((uint32_t)0x00000100U) /*!< Bit 8 */
#define  RI_ASMR2_PB_9                 ((uint32_t)0x00000200U) /*!< Bit 9 */
#define  RI_ASMR2_PB_10                ((uint32_t)0x00000400U) /*!< Bit 10 */
#define  RI_ASMR2_PB_11                ((uint32_t)0x00000800U) /*!< Bit 11 */
#define  RI_ASMR2_PB_12                ((uint32_t)0x00001000U) /*!< Bit 12 */
#define  RI_ASMR2_PB_13                ((uint32_t)0x00002000U) /*!< Bit 13 */
#define  RI_ASMR2_PB_14                ((uint32_t)0x00004000U) /*!< Bit 14 */
#define  RI_ASMR2_PB_15                ((uint32_t)0x00008000U) /*!< Bit 15 */

/********************  Bit definition for RI_CMR2 register  ********************/
#define  RI_CMR2_PB                    ((uint32_t)0x0000FFFFU) /*!< PB[15:0] Port B selection */
#define  RI_CMR2_PB_0                  ((uint32_t)0x00000001U) /*!< Bit 0 */
#define  RI_CMR2_PB_1                  ((uint32_t)0x00000002U) /*!< Bit 1 */
#define  RI_CMR2_PB_2                  ((uint32_t)0x00000004U) /*!< Bit 2 */
#define  RI_CMR2_PB_3                  ((uint32_t)0x00000008U) /*!< Bit 3 */
#define  RI_CMR2_PB_4                  ((uint32_t)0x00000010U) /*!< Bit 4 */
#define  RI_CMR2_PB_5                  ((uint32_t)0x00000020U) /*!< Bit 5 */
#define  RI_CMR2_PB_6                  ((uint32_t)0x00000040U) /*!< Bit 6 */
#define  RI_CMR2_PB_7                  ((uint32_t)0x00000080U) /*!< Bit 7 */
#define  RI_CMR2_PB_8                  ((uint32_t)0x00000100U) /*!< Bit 8 */
#define  RI_CMR2_PB_9                  ((uint32_t)0x00000200U) /*!< Bit 9 */
#define  RI_CMR2_PB_10                 ((uint32_t)0x00000400U) /*!< Bit 10 */
#define  RI_CMR2_PB_11                 ((uint32_t)0x00000800U) /*!< Bit 11 */
#define  RI_CMR2_PB_12                 ((uint32_t)0x00001000U) /*!< Bit 12 */
#define  RI_CMR2_PB_13                 ((uint32_t)0x00002000U) /*!< Bit 13 */
#define  RI_CMR2_PB_14                 ((uint32_t)0x00004000U) /*!< Bit 14 */
#define  RI_CMR2_PB_15                 ((uint32_t)0x00008000U) /*!< Bit 15 */

/********************  Bit definition for RI_CICR2 register  ********************/
#define  RI_CICR2_PB                   ((uint32_t)0x0000FFFFU) /*!< PB[15:0] Port B selection */
#define  RI_CICR2_PB_0                 ((uint32_t)0x00000001U) /*!< Bit 0 */
#define  RI_CICR2_PB_1                 ((uint32_t)0x00000002U) /*!< Bit 1 */
#define  RI_CICR2_PB_2                 ((uint32_t)0x00000004U) /*!< Bit 2 */
#define  RI_CICR2_PB_3                 ((uint32_t)0x00000008U) /*!< Bit 3 */
#define  RI_CICR2_PB_4                 ((uint32_t)0x00000010U) /*!< Bit 4 */
#define  RI_CICR2_PB_5                 ((uint32_t)0x00000020U) /*!< Bit 5 */
#define  RI_CICR2_PB_6                 ((uint32_t)0x00000040U) /*!< Bit 6 */
#define  RI_CICR2_PB_7                 ((uint32_t)0x00000080U) /*!< Bit 7 */
#define  RI_CICR2_PB_8                 ((uint32_t)0x00000100U) /*!< Bit 8 */
#define  RI_CICR2_PB_9                 ((uint32_t)0x00000200U) /*!< Bit 9 */
#define  RI_CICR2_PB_10                ((uint32_t)0x00000400U) /*!< Bit 10 */
#define  RI_CICR2_PB_11                ((uint32_t)0x00000800U) /*!< Bit 11 */
#define  RI_CICR2_PB_12                ((uint32_t)0x00001000U) /*!< Bit 12 */
#define  RI_CICR2_PB_13                ((uint32_t)0x00002000U) /*!< Bit 13 */
#define  RI_CICR2_PB_14                ((uint32_t)0x00004000U) /*!< Bit 14 */
#define  RI_CICR2_PB_15                ((uint32_t)0x00008000U) /*!< Bit 15 */

/********************  Bit definition for RI_ASMR3 register  ********************/
#define  RI_ASMR3_PC                   ((uint32_t)0x0000FFFFU) /*!< PC[15:0] Port C selection */
#define  RI_ASMR3_PC_0                 ((uint32_t)0x00000001U) /*!< Bit 0 */
#define  RI_ASMR3_PC_1                 ((uint32_t)0x00000002U) /*!< Bit 1 */
#define  RI_ASMR3_PC_2                 ((uint32_t)0x00000004U) /*!< Bit 2 */
#define  RI_ASMR3_PC_3                 ((uint32_t)0x00000008U) /*!< Bit 3 */
#define  RI_ASMR3_PC_4                 ((uint32_t)0x00000010U) /*!< Bit 4 */
#define  RI_ASMR3_PC_5                 ((uint32_t)0x00000020U) /*!< Bit 5 */
#define  RI_ASMR3_PC_6                 ((uint32_t)0x00000040U) /*!< Bit 6 */
#define  RI_ASMR3_PC_7                 ((uint32_t)0x00000080U) /*!< Bit 7 */
#define  RI_ASMR3_PC_8                 ((uint32_t)0x00000100U) /*!< Bit 8 */
#define  RI_ASMR3_PC_9                 ((uint32_t)0x00000200U) /*!< Bit 9 */
#define  RI_ASMR3_PC_10                ((uint32_t)0x00000400U) /*!< Bit 10 */
#define  RI_ASMR3_PC_11                ((uint32_t)0x00000800U) /*!< Bit 11 */
#define  RI_ASMR3_PC_12                ((uint32_t)0x00001000U) /*!< Bit 12 */
#define  RI_ASMR3_PC_13                ((uint32_t)0x00002000U) /*!< Bit 13 */
#define  RI_ASMR3_PC_14                ((uint32_t)0x00004000U) /*!< Bit 14 */
#define  RI_ASMR3_PC_15                ((uint32_t)0x00008000U) /*!< Bit 15 */

/********************  Bit definition for RI_CMR3 register  ********************/
#define  RI_CMR3_PC                    ((uint32_t)0x0000FFFFU) /*!< PC[15:0] Port C selection */
#define  RI_CMR3_PC_0                  ((uint32_t)0x00000001U) /*!< Bit 0 */
#define  RI_CMR3_PC_1                  ((uint32_t)0x00000002U) /*!< Bit 1 */
#define  RI_CMR3_PC_2                  ((uint32_t)0x00000004U) /*!< Bit 2 */
#define  RI_CMR3_PC_3                  ((uint32_t)0x00000008U) /*!< Bit 3 */
#define  RI_CMR3_PC_4                  ((uint32_t)0x00000010U) /*!< Bit 4 */
#define  RI_CMR3_PC_5                  ((uint32_t)0x00000020U) /*!< Bit 5 */
#define  RI_CMR3_PC_6                  ((uint32_t)0x00000040U) /*!< Bit 6 */
#define  RI_CMR3_PC_7                  ((uint32_t)0x00000080U) /*!< Bit 7 */
#define  RI_CMR3_PC_8                  ((uint32_t)0x00000100U) /*!< Bit 8 */
#define  RI_CMR3_PC_9                  ((uint32_t)0x00000200U) /*!< Bit 9 */
#define  RI_CMR3_PC_10                 ((uint32_t)0x00000400U) /*!< Bit 10 */
#define  RI_CMR3_PC_11                 ((uint32_t)0x00000800U) /*!< Bit 11 */
#define  RI_CMR3_PC_12                 ((uint32_t)0x00001000U) /*!< Bit 12 */
#define  RI_CMR3_PC_13                 ((uint32_t)0x00002000U) /*!< Bit 13 */
#define  RI_CMR3_PC_14                 ((uint32_t)0x00004000U) /*!< Bit 14 */
#define  RI_CMR3_PC_15                 ((uint32_t)0x00008000U) /*!< Bit 15 */

/********************  Bit definition for RI_CICR3 register  ********************/
#define  RI_CICR3_PC                   ((uint32_t)0x0000FFFFU) /*!< PC[15:0] Port C selection */
#define  RI_CICR3_PC_0                 ((uint32_t)0x00000001U) /*!< Bit 0 */
#define  RI_CICR3_PC_1                 ((uint32_t)0x00000002U) /*!< Bit 1 */
#define  RI_CICR3_PC_2                 ((uint32_t)0x00000004U) /*!< Bit 2 */
#define  RI_CICR3_PC_3                 ((uint32_t)0x00000008U) /*!< Bit 3 */
#define  RI_CICR3_PC_4                 ((uint32_t)0x00000010U) /*!< Bit 4 */
#define  RI_CICR3_PC_5                 ((uint32_t)0x00000020U) /*!< Bit 5 */
#define  RI_CICR3_PC_6                 ((uint32_t)0x00000040U) /*!< Bit 6 */
#define  RI_CICR3_PC_7                 ((uint32_t)0x00000080U) /*!< Bit 7 */
#define  RI_CICR3_PC_8                 ((uint32_t)0x00000100U) /*!< Bit 8 */
#define  RI_CICR3_PC_9                 ((uint32_t)0x00000200U) /*!< Bit 9 */
#define  RI_CICR3_PC_10                ((uint32_t)0x00000400U) /*!< Bit 10 */
#define  RI_CICR3_PC_11                ((uint32_t)0x00000800U) /*!< Bit 11 */
#define  RI_CICR3_PC_12                ((uint32_t)0x00001000U) /*!< Bit 12 */
#define  RI_CICR3_PC_13                ((uint32_t)0x00002000U) /*!< Bit 13 */
#define  RI_CICR3_PC_14                ((uint32_t)0x00004000U) /*!< Bit 14 */
#define  RI_CICR3_PC_15                ((uint32_t)0x00008000U) /*!< Bit 15 */

/********************  Bit definition for RI_ASMR4 register  ********************/
#define  RI_ASMR4_PF                   ((uint32_t)0x0000FFFFU) /*!< PF[15:0] Port F selection */
#define  RI_ASMR4_PF_0                 ((uint32_t)0x00000001U) /*!< Bit 0 */
#define  RI_ASMR4_PF_1                 ((uint32_t)0x00000002U) /*!< Bit 1 */
#define  RI_ASMR4_PF_2                 ((uint32_t)0x00000004U) /*!< Bit 2 */
#define  RI_ASMR4_PF_3                 ((uint32_t)0x00000008U) /*!< Bit 3 */
#define  RI_ASMR4_PF_4                 ((uint32_t)0x00000010U) /*!< Bit 4 */
#define  RI_ASMR4_PF_5                 ((uint32_t)0x00000020U) /*!< Bit 5 */
#define  RI_ASMR4_PF_6                 ((uint32_t)0x00000040U) /*!< Bit 6 */
#define  RI_ASMR4_PF_7                 ((uint32_t)0x00000080U) /*!< Bit 7 */
#define  RI_ASMR4_PF_8                 ((uint32_t)0x00000100U) /*!< Bit 8 */
#define  RI_ASMR4_PF_9                 ((uint32_t)0x00000200U) /*!< Bit 9 */
#define  RI_ASMR4_PF_10                ((uint32_t)0x00000400U) /*!< Bit 10 */
#define  RI_ASMR4_PF_11                ((uint32_t)0x00000800U) /*!< Bit 11 */
#define  RI_ASMR4_PF_12                ((uint32_t)0x00001000U) /*!< Bit 12 */
#define  RI_ASMR4_PF_13                ((uint32_t)0x00002000U) /*!< Bit 13 */
#define  RI_ASMR4_PF_14                ((uint32_t)0x00004000U) /*!< Bit 14 */
#define  RI_ASMR4_PF_15                ((uint32_t)0x00008000U) /*!< Bit 15 */

/********************  Bit definition for RI_CMR4 register  ********************/
#define  RI_CMR4_PF                    ((uint32_t)0x0000FFFFU) /*!< PF[15:0] Port F selection */
#define  RI_CMR4_PF_0                  ((uint32_t)0x00000001U) /*!< Bit 0 */
#define  RI_CMR4_PF_1                  ((uint32_t)0x00000002U) /*!< Bit 1 */
#define  RI_CMR4_PF_2                  ((uint32_t)0x00000004U) /*!< Bit 2 */
#define  RI_CMR4_PF_3                  ((uint32_t)0x00000008U) /*!< Bit 3 */
#define  RI_CMR4_PF_4                  ((uint32_t)0x00000010U) /*!< Bit 4 */
#define  RI_CMR4_PF_5                  ((uint32_t)0x00000020U) /*!< Bit 5 */
#define  RI_CMR4_PF_6                  ((uint32_t)0x00000040U) /*!< Bit 6 */
#define  RI_CMR4_PF_7                  ((uint32_t)0x00000080U) /*!< Bit 7 */
#define  RI_CMR4_PF_8                  ((uint32_t)0x00000100U) /*!< Bit 8 */
#define  RI_CMR4_PF_9                  ((uint32_t)0x00000200U) /*!< Bit 9 */
#define  RI_CMR4_PF_10                 ((uint32_t)0x00000400U) /*!< Bit 10 */
#define  RI_CMR4_PF_11                 ((uint32_t)0x00000800U) /*!< Bit 11 */
#define  RI_CMR4_PF_12                 ((uint32_t)0x00001000U) /*!< Bit 12 */
#define  RI_CMR4_PF_13                 ((uint32_t)0x00002000U) /*!< Bit 13 */
#define  RI_CMR4_PF_14                 ((uint32_t)0x00004000U) /*!< Bit 14 */
#define  RI_CMR4_PF_15                 ((uint32_t)0x00008000U) /*!< Bit 15 */

/********************  Bit definition for RI_CICR4 register  ********************/
#define  RI_CICR4_PF                   ((uint32_t)0x0000FFFFU) /*!< PF[15:0] Port F selection */
#define  RI_CICR4_PF_0                 ((uint32_t)0x00000001U) /*!< Bit 0 */
#define  RI_CICR4_PF_1                 ((uint32_t)0x00000002U) /*!< Bit 1 */
#define  RI_CICR4_PF_2                 ((uint32_t)0x00000004U) /*!< Bit 2 */
#define  RI_CICR4_PF_3                 ((uint32_t)0x00000008U) /*!< Bit 3 */
#define  RI_CICR4_PF_4                 ((uint32_t)0x00000010U) /*!< Bit 4 */
#define  RI_CICR4_PF_5                 ((uint32_t)0x00000020U) /*!< Bit 5 */
#define  RI_CICR4_PF_6                 ((uint32_t)0x00000040U) /*!< Bit 6 */
#define  RI_CICR4_PF_7                 ((uint32_t)0x00000080U) /*!< Bit 7 */
#define  RI_CICR4_PF_8                 ((uint32_t)0x00000100U) /*!< Bit 8 */
#define  RI_CICR4_PF_9                 ((uint32_t)0x00000200U) /*!< Bit 9 */
#define  RI_CICR4_PF_10                ((uint32_t)0x00000400U) /*!< Bit 10 */
#define  RI_CICR4_PF_11                ((uint32_t)0x00000800U) /*!< Bit 11 */
#define  RI_CICR4_PF_12                ((uint32_t)0x00001000U) /*!< Bit 12 */
#define  RI_CICR4_PF_13                ((uint32_t)0x00002000U) /*!< Bit 13 */
#define  RI_CICR4_PF_14                ((uint32_t)0x00004000U) /*!< Bit 14 */
#define  RI_CICR4_PF_15                ((uint32_t)0x00008000U) /*!< Bit 15 */

/********************  Bit definition for RI_ASMR5 register  ********************/
#define  RI_ASMR5_PG                   ((uint32_t)0x0000FFFFU) /*!< PG[15:0] Port G selection */
#define  RI_ASMR5_PG_0                 ((uint32_t)0x00000001U) /*!< Bit 0 */
#define  RI_ASMR5_PG_1                 ((uint32_t)0x00000002U) /*!< Bit 1 */
#define  RI_ASMR5_PG_2                 ((uint32_t)0x00000004U) /*!< Bit 2 */
#define  RI_ASMR5_PG_3                 ((uint32_t)0x00000008U) /*!< Bit 3 */
#define  RI_ASMR5_PG_4                 ((uint32_t)0x00000010U) /*!< Bit 4 */
#define  RI_ASMR5_PG_5                 ((uint32_t)0x00000020U) /*!< Bit 5 */
#define  RI_ASMR5_PG_6                 ((uint32_t)0x00000040U) /*!< Bit 6 */
#define  RI_ASMR5_PG_7                 ((uint32_t)0x00000080U) /*!< Bit 7 */
#define  RI_ASMR5_PG_8                 ((uint32_t)0x00000100U) /*!< Bit 8 */
#define  RI_ASMR5_PG_9                 ((uint32_t)0x00000200U) /*!< Bit 9 */
#define  RI_ASMR5_PG_10                ((uint32_t)0x00000400U) /*!< Bit 10 */
#define  RI_ASMR5_PG_11                ((uint32_t)0x00000800U) /*!< Bit 11 */
#define  RI_ASMR5_PG_12                ((uint32_t)0x00001000U) /*!< Bit 12 */
#define  RI_ASMR5_PG_13                ((uint32_t)0x00002000U) /*!< Bit 13 */
#define  RI_ASMR5_PG_14                ((uint32_t)0x00004000U) /*!< Bit 14 */
#define  RI_ASMR5_PG_15                ((uint32_t)0x00008000U) /*!< Bit 15 */

/********************  Bit definition for RI_CMR5 register  ********************/
#define  RI_CMR5_PG                    ((uint32_t)0x0000FFFFU) /*!< PG[15:0] Port G selection */
#define  RI_CMR5_PG_0                  ((uint32_t)0x00000001U) /*!< Bit 0 */
#define  RI_CMR5_PG_1                  ((uint32_t)0x00000002U) /*!< Bit 1 */
#define  RI_CMR5_PG_2                  ((uint32_t)0x00000004U) /*!< Bit 2 */
#define  RI_CMR5_PG_3                  ((uint32_t)0x00000008U) /*!< Bit 3 */
#define  RI_CMR5_PG_4                  ((uint32_t)0x00000010U) /*!< Bit 4 */
#define  RI_CMR5_PG_5                  ((uint32_t)0x00000020U) /*!< Bit 5 */
#define  RI_CMR5_PG_6                  ((uint32_t)0x00000040U) /*!< Bit 6 */
#define  RI_CMR5_PG_7                  ((uint32_t)0x00000080U) /*!< Bit 7 */
#define  RI_CMR5_PG_8                  ((uint32_t)0x00000100U) /*!< Bit 8 */
#define  RI_CMR5_PG_9                  ((uint32_t)0x00000200U) /*!< Bit 9 */
#define  RI_CMR5_PG_10                 ((uint32_t)0x00000400U) /*!< Bit 10 */
#define  RI_CMR5_PG_11                 ((uint32_t)0x00000800U) /*!< Bit 11 */
#define  RI_CMR5_PG_12                 ((uint32_t)0x00001000U) /*!< Bit 12 */
#define  RI_CMR5_PG_13                 ((uint32_t)0x00002000U) /*!< Bit 13 */
#define  RI_CMR5_PG_14                 ((uint32_t)0x00004000U) /*!< Bit 14 */
#define  RI_CMR5_PG_15                 ((uint32_t)0x00008000U) /*!< Bit 15 */

/********************  Bit definition for RI_CICR5 register  ********************/
#define  RI_CICR5_PG                   ((uint32_t)0x0000FFFFU) /*!< PG[15:0] Port G selection */
#define  RI_CICR5_PG_0                 ((uint32_t)0x00000001U) /*!< Bit 0 */
#define  RI_CICR5_PG_1                 ((uint32_t)0x00000002U) /*!< Bit 1 */
#define  RI_CICR5_PG_2                 ((uint32_t)0x00000004U) /*!< Bit 2 */
#define  RI_CICR5_PG_3                 ((uint32_t)0x00000008U) /*!< Bit 3 */
#define  RI_CICR5_PG_4                 ((uint32_t)0x00000010U) /*!< Bit 4 */
#define  RI_CICR5_PG_5                 ((uint32_t)0x00000020U) /*!< Bit 5 */
#define  RI_CICR5_PG_6                 ((uint32_t)0x00000040U) /*!< Bit 6 */
#define  RI_CICR5_PG_7                 ((uint32_t)0x00000080U) /*!< Bit 7 */
#define  RI_CICR5_PG_8                 ((uint32_t)0x00000100U) /*!< Bit 8 */
#define  RI_CICR5_PG_9                 ((uint32_t)0x00000200U) /*!< Bit 9 */
#define  RI_CICR5_PG_10                ((uint32_t)0x00000400U) /*!< Bit 10 */
#define  RI_CICR5_PG_11                ((uint32_t)0x00000800U) /*!< Bit 11 */
#define  RI_CICR5_PG_12                ((uint32_t)0x00001000U) /*!< Bit 12 */
#define  RI_CICR5_PG_13                ((uint32_t)0x00002000U) /*!< Bit 13 */
#define  RI_CICR5_PG_14                ((uint32_t)0x00004000U) /*!< Bit 14 */
#define  RI_CICR5_PG_15                ((uint32_t)0x00008000U) /*!< Bit 15 */

/******************************************************************************/
/*                                                                            */
/*                               Timers (TIM)                                 */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for TIM_CR1 register  ********************/
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

/*******************  Bit definition for TIM_CR2 register  ********************/
#define  TIM_CR2_CCDS                        ((uint32_t)0x00000008U)            /*!<Capture/Compare DMA Selection */

#define  TIM_CR2_MMS                         ((uint32_t)0x00000070U)            /*!<MMS[2:0] bits (Master Mode Selection) */
#define  TIM_CR2_MMS_0                       ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define  TIM_CR2_MMS_1                       ((uint32_t)0x00000020U)            /*!<Bit 1 */
#define  TIM_CR2_MMS_2                       ((uint32_t)0x00000040U)            /*!<Bit 2 */

#define  TIM_CR2_TI1S                        ((uint32_t)0x00000080U)            /*!<TI1 Selection */

/*******************  Bit definition for TIM_SMCR register  *******************/
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

/*******************  Bit definition for TIM_DIER register  *******************/
#define  TIM_DIER_UIE                        ((uint32_t)0x00000001U)            /*!<Update interrupt enable */
#define  TIM_DIER_CC1IE                      ((uint32_t)0x00000002U)            /*!<Capture/Compare 1 interrupt enable */
#define  TIM_DIER_CC2IE                      ((uint32_t)0x00000004U)            /*!<Capture/Compare 2 interrupt enable */
#define  TIM_DIER_CC3IE                      ((uint32_t)0x00000008U)            /*!<Capture/Compare 3 interrupt enable */
#define  TIM_DIER_CC4IE                      ((uint32_t)0x00000010U)            /*!<Capture/Compare 4 interrupt enable */
#define  TIM_DIER_TIE                        ((uint32_t)0x00000040U)            /*!<Trigger interrupt enable */
#define  TIM_DIER_UDE                        ((uint32_t)0x00000100U)            /*!<Update DMA request enable */
#define  TIM_DIER_CC1DE                      ((uint32_t)0x00000200U)            /*!<Capture/Compare 1 DMA request enable */
#define  TIM_DIER_CC2DE                      ((uint32_t)0x00000400U)            /*!<Capture/Compare 2 DMA request enable */
#define  TIM_DIER_CC3DE                      ((uint32_t)0x00000800U)            /*!<Capture/Compare 3 DMA request enable */
#define  TIM_DIER_CC4DE                      ((uint32_t)0x00001000U)            /*!<Capture/Compare 4 DMA request enable */
#define  TIM_DIER_COMDE                      ((uint16_t)0x2000U)            /*!<COM DMA request enable */
#define  TIM_DIER_TDE                        ((uint32_t)0x00004000U)            /*!<Trigger DMA request enable */

/********************  Bit definition for TIM_SR register  ********************/
#define  TIM_SR_UIF                          ((uint32_t)0x00000001U)            /*!<Update interrupt Flag */
#define  TIM_SR_CC1IF                        ((uint32_t)0x00000002U)            /*!<Capture/Compare 1 interrupt Flag */
#define  TIM_SR_CC2IF                        ((uint32_t)0x00000004U)            /*!<Capture/Compare 2 interrupt Flag */
#define  TIM_SR_CC3IF                        ((uint32_t)0x00000008U)            /*!<Capture/Compare 3 interrupt Flag */
#define  TIM_SR_CC4IF                        ((uint32_t)0x00000010U)            /*!<Capture/Compare 4 interrupt Flag */
#define  TIM_SR_TIF                          ((uint32_t)0x00000040U)            /*!<Trigger interrupt Flag */
#define  TIM_SR_CC1OF                        ((uint32_t)0x00000200U)            /*!<Capture/Compare 1 Overcapture Flag */
#define  TIM_SR_CC2OF                        ((uint32_t)0x00000400U)            /*!<Capture/Compare 2 Overcapture Flag */
#define  TIM_SR_CC3OF                        ((uint32_t)0x00000800U)            /*!<Capture/Compare 3 Overcapture Flag */
#define  TIM_SR_CC4OF                        ((uint32_t)0x00001000U)            /*!<Capture/Compare 4 Overcapture Flag */

/*******************  Bit definition for TIM_EGR register  ********************/
#define  TIM_EGR_UG                          ((uint32_t)0x00000001U)               /*!<Update Generation */
#define  TIM_EGR_CC1G                        ((uint32_t)0x00000002U)               /*!<Capture/Compare 1 Generation */
#define  TIM_EGR_CC2G                        ((uint32_t)0x00000004U)               /*!<Capture/Compare 2 Generation */
#define  TIM_EGR_CC3G                        ((uint32_t)0x00000008U)               /*!<Capture/Compare 3 Generation */
#define  TIM_EGR_CC4G                        ((uint32_t)0x00000010U)               /*!<Capture/Compare 4 Generation */
#define  TIM_EGR_TG                          ((uint32_t)0x00000040U)               /*!<Trigger Generation */
                   
/******************  Bit definition for TIM_CCMR1 register  *******************/
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

/*----------------------------------------------------------------------------*/

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

/******************  Bit definition for TIM_CCMR2 register  *******************/
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

/*----------------------------------------------------------------------------*/

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

/*******************  Bit definition for TIM_CCER register  *******************/
#define  TIM_CCER_CC1E                       ((uint32_t)0x00000001U)            /*!<Capture/Compare 1 output enable */
#define  TIM_CCER_CC1P                       ((uint32_t)0x00000002U)            /*!<Capture/Compare 1 output Polarity */
#define  TIM_CCER_CC1NP                      ((uint32_t)0x00000008U)            /*!<Capture/Compare 1 Complementary output Polarity */
#define  TIM_CCER_CC2E                       ((uint32_t)0x00000010U)            /*!<Capture/Compare 2 output enable */
#define  TIM_CCER_CC2P                       ((uint32_t)0x00000020U)            /*!<Capture/Compare 2 output Polarity */
#define  TIM_CCER_CC2NP                      ((uint32_t)0x00000080U)            /*!<Capture/Compare 2 Complementary output Polarity */
#define  TIM_CCER_CC3E                       ((uint32_t)0x00000100U)            /*!<Capture/Compare 3 output enable */
#define  TIM_CCER_CC3P                       ((uint32_t)0x00000200U)            /*!<Capture/Compare 3 output Polarity */
#define  TIM_CCER_CC3NP                      ((uint32_t)0x00000800U)            /*!<Capture/Compare 3 Complementary output Polarity */
#define  TIM_CCER_CC4E                       ((uint32_t)0x00001000U)            /*!<Capture/Compare 4 output enable */
#define  TIM_CCER_CC4P                       ((uint32_t)0x00002000U)            /*!<Capture/Compare 4 output Polarity */
#define  TIM_CCER_CC4NP                      ((uint32_t)0x00008000U)            /*!<Capture/Compare 4 Complementary output Polarity */

/*******************  Bit definition for TIM_CNT register  ********************/
#define  TIM_CNT_CNT                         ((uint32_t)0x0000FFFFU)            /*!<Counter Value */

/*******************  Bit definition for TIM_PSC register  ********************/
#define  TIM_PSC_PSC                         ((uint32_t)0x0000FFFFU)            /*!<Prescaler Value */

/*******************  Bit definition for TIM_ARR register  ********************/
#define  TIM_ARR_ARR                         ((uint32_t)0x0000FFFFU)            /*!<actual auto-reload Value */
           
/*******************  Bit definition for TIM_CCR1 register  *******************/
#define  TIM_CCR1_CCR1                       ((uint32_t)0x0000FFFFU)            /*!<Capture/Compare 1 Value */

/*******************  Bit definition for TIM_CCR2 register  *******************/
#define  TIM_CCR2_CCR2                       ((uint32_t)0x0000FFFFU)            /*!<Capture/Compare 2 Value */

/*******************  Bit definition for TIM_CCR3 register  *******************/
#define  TIM_CCR3_CCR3                       ((uint32_t)0x0000FFFFU)            /*!<Capture/Compare 3 Value */

/*******************  Bit definition for TIM_CCR4 register  *******************/
#define  TIM_CCR4_CCR4                       ((uint32_t)0x0000FFFFU)            /*!<Capture/Compare 4 Value */

/*******************  Bit definition for TIM_DCR register  ********************/
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

/*******************  Bit definition for TIM_DMAR register  *******************/
#define  TIM_DMAR_DMAB                       ((uint32_t)0x0000FFFFU)            /*!<DMA register for burst accesses */

/*******************  Bit definition for TIM_OR register  *********************/
#define  TIM_OR_TI1RMP                       ((uint32_t)0x00000003U)            /*!<TI1_RMP[1:0] bits (TIM Input 1 remap) */
#define  TIM_OR_TI1RMP_0                     ((uint32_t)0x00000001U)            /*!<Bit 0 */
#define  TIM_OR_TI1RMP_1                     ((uint32_t)0x00000002U)            /*!<Bit 1 */

#define  TIM_OR_ETR_RMP                      ((uint32_t)0x00000004U)            /*!<ETR_RMP bit (TIM10/11 ETR remap)*/
#define  TIM_OR_TI1_RMP_RI                   ((uint32_t)0x00000008U)            /*!<TI1_RMP_RI bit (TIM10/11 Input 1 remap for Routing interface) */

/*----------------------------------------------------------------------------*/
#define  TIM9_OR_ITR1_RMP                    ((uint32_t)0x00000004U)            /*!<ITR1_RMP bit (TIM9 Internal trigger 1 remap) */

/*----------------------------------------------------------------------------*/
#define  TIM2_OR_ITR1_RMP                    ((uint32_t)0x00000001U)            /*!<ITR1_RMP bit (TIM2 Internal trigger 1 remap) */

/*----------------------------------------------------------------------------*/
#define  TIM3_OR_ITR2_RMP                    ((uint32_t)0x00000001U)            /*!<ITR2_RMP bit (TIM3 Internal trigger 2 remap) */

/*----------------------------------------------------------------------------*/


/******************************************************************************/
/*                                                                            */
/*      Universal Synchronous Asynchronous Receiver Transmitter (USART)       */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for USART_SR register  *******************/
#define  USART_SR_PE                         ((uint32_t)0x00000001U)            /*!< Parity Error */
#define  USART_SR_FE                         ((uint32_t)0x00000002U)            /*!< Framing Error */
#define  USART_SR_NE                         ((uint32_t)0x00000004U)            /*!< Noise Error Flag */
#define  USART_SR_ORE                        ((uint32_t)0x00000008U)            /*!< OverRun Error */
#define  USART_SR_IDLE                       ((uint32_t)0x00000010U)            /*!< IDLE line detected */
#define  USART_SR_RXNE                       ((uint32_t)0x00000020U)            /*!< Read Data Register Not Empty */
#define  USART_SR_TC                         ((uint32_t)0x00000040U)            /*!< Transmission Complete */
#define  USART_SR_TXE                        ((uint32_t)0x00000080U)            /*!< Transmit Data Register Empty */
#define  USART_SR_LBD                        ((uint32_t)0x00000100U)            /*!< LIN Break Detection Flag */
#define  USART_SR_CTS                        ((uint32_t)0x00000200U)            /*!< CTS Flag */

/*******************  Bit definition for USART_DR register  *******************/
#define  USART_DR_DR                         ((uint32_t)0x000001FFU)            /*!< Data value */

/******************  Bit definition for USART_BRR register  *******************/
#define  USART_BRR_DIV_FRACTION              ((uint32_t)0x0000000FU)            /*!< Fraction of USARTDIV */
#define  USART_BRR_DIV_MANTISSA              ((uint32_t)0x0000FFF0U)            /*!< Mantissa of USARTDIV */

/******************  Bit definition for USART_CR1 register  *******************/
#define  USART_CR1_SBK                       ((uint32_t)0x00000001U)            /*!< Send Break */
#define  USART_CR1_RWU                       ((uint32_t)0x00000002U)            /*!< Receiver wakeup */
#define  USART_CR1_RE                        ((uint32_t)0x00000004U)            /*!< Receiver Enable */
#define  USART_CR1_TE                        ((uint32_t)0x00000008U)            /*!< Transmitter Enable */
#define  USART_CR1_IDLEIE                    ((uint32_t)0x00000010U)            /*!< IDLE Interrupt Enable */
#define  USART_CR1_RXNEIE                    ((uint32_t)0x00000020U)            /*!< RXNE Interrupt Enable */
#define  USART_CR1_TCIE                      ((uint32_t)0x00000040U)            /*!< Transmission Complete Interrupt Enable */
#define  USART_CR1_TXEIE                     ((uint32_t)0x00000080U)            /*!< PE Interrupt Enable */
#define  USART_CR1_PEIE                      ((uint32_t)0x00000100U)            /*!< PE Interrupt Enable */
#define  USART_CR1_PS                        ((uint32_t)0x00000200U)            /*!< Parity Selection */
#define  USART_CR1_PCE                       ((uint32_t)0x00000400U)            /*!< Parity Control Enable */
#define  USART_CR1_WAKE                      ((uint32_t)0x00000800U)            /*!< Wakeup method */
#define  USART_CR1_M                         ((uint32_t)0x00001000U)            /*!< Word length */
#define  USART_CR1_UE                        ((uint32_t)0x00002000U)            /*!< USART Enable */
#define  USART_CR1_OVER8                     ((uint32_t)0x00008000U)            /*!< Oversampling by 8-bit mode */

/******************  Bit definition for USART_CR2 register  *******************/
#define  USART_CR2_ADD                       ((uint32_t)0x0000000FU)            /*!< Address of the USART node */
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

/******************  Bit definition for USART_CR3 register  *******************/
#define  USART_CR3_EIE                       ((uint32_t)0x00000001U)            /*!< Error Interrupt Enable */
#define  USART_CR3_IREN                      ((uint32_t)0x00000002U)            /*!< IrDA mode Enable */
#define  USART_CR3_IRLP                      ((uint32_t)0x00000004U)            /*!< IrDA Low-Power */
#define  USART_CR3_HDSEL                     ((uint32_t)0x00000008U)            /*!< Half-Duplex Selection */
#define  USART_CR3_NACK                      ((uint32_t)0x00000010U)            /*!< Smartcard NACK enable */
#define  USART_CR3_SCEN                      ((uint32_t)0x00000020U)            /*!< Smartcard mode enable */
#define  USART_CR3_DMAR                      ((uint32_t)0x00000040U)            /*!< DMA Enable Receiver */
#define  USART_CR3_DMAT                      ((uint32_t)0x00000080U)            /*!< DMA Enable Transmitter */
#define  USART_CR3_RTSE                      ((uint32_t)0x00000100U)            /*!< RTS Enable */
#define  USART_CR3_CTSE                      ((uint32_t)0x00000200U)            /*!< CTS Enable */
#define  USART_CR3_CTSIE                     ((uint32_t)0x00000400U)            /*!< CTS Interrupt Enable */
#define  USART_CR3_ONEBIT                    ((uint32_t)0x00000800U)            /*!< One sample bit method enable */

/******************  Bit definition for USART_GTPR register  ******************/
#define  USART_GTPR_PSC                      ((uint32_t)0x000000FFU)            /*!< PSC[7:0] bits (Prescaler value) */
#define  USART_GTPR_PSC_0                    ((uint32_t)0x00000001U)            /*!< Bit 0 */
#define  USART_GTPR_PSC_1                    ((uint32_t)0x00000002U)            /*!< Bit 1 */
#define  USART_GTPR_PSC_2                    ((uint32_t)0x00000004U)            /*!< Bit 2 */
#define  USART_GTPR_PSC_3                    ((uint32_t)0x00000008U)            /*!< Bit 3 */
#define  USART_GTPR_PSC_4                    ((uint32_t)0x00000010U)            /*!< Bit 4 */
#define  USART_GTPR_PSC_5                    ((uint32_t)0x00000020U)            /*!< Bit 5 */
#define  USART_GTPR_PSC_6                    ((uint32_t)0x00000040U)            /*!< Bit 6 */
#define  USART_GTPR_PSC_7                    ((uint32_t)0x00000080U)            /*!< Bit 7 */

#define  USART_GTPR_GT                       ((uint32_t)0x0000FF00U)            /*!< Guard time value */

/******************************************************************************/
/*                                                                            */
/*                     Universal Serial Bus (USB)                             */
/*                                                                            */
/******************************************************************************/

/*!<Endpoint-specific registers */

#define  USB_EP0R                              USB_BASE                        /*!< endpoint 0 register address */
#define  USB_EP1R                             (USB_BASE + 0x00000004U)         /*!< endpoint 1 register address */
#define  USB_EP2R                             (USB_BASE + 0x00000008U)         /*!< endpoint 2 register address */
#define  USB_EP3R                             (USB_BASE + 0x0000000CU)         /*!< endpoint 3 register address */
#define  USB_EP4R                             (USB_BASE + 0x00000010U)         /*!< endpoint 4 register address */
#define  USB_EP5R                             (USB_BASE + 0x00000014U)         /*!< endpoint 5 register address */
#define  USB_EP6R                             (USB_BASE + 0x00000018U)         /*!< endpoint 6 register address */
#define  USB_EP7R                             (USB_BASE + 0x0000001CU)         /*!< endpoint 7 register address */

/* bit positions */ 
#define  USB_EP_CTR_RX                        ((uint32_t)0x00008000U)           /*!<  EndPoint Correct TRansfer RX */
#define  USB_EP_DTOG_RX                       ((uint32_t)0x00004000U)           /*!<  EndPoint Data TOGGLE RX */
#define  USB_EPRX_STAT                        ((uint32_t)0x00003000U)           /*!<  EndPoint RX STATus bit field */
#define  USB_EP_SETUP                         ((uint32_t)0x00000800U)           /*!<  EndPoint SETUP */
#define  USB_EP_T_FIELD                       ((uint32_t)0x00000600U)           /*!<  EndPoint TYPE */
#define  USB_EP_KIND                          ((uint32_t)0x00000100U)           /*!<  EndPoint KIND */
#define  USB_EP_CTR_TX                        ((uint32_t)0x00000080U)           /*!<  EndPoint Correct TRansfer TX */
#define  USB_EP_DTOG_TX                       ((uint32_t)0x00000040U)           /*!<  EndPoint Data TOGGLE TX */
#define  USB_EPTX_STAT                        ((uint32_t)0x00000030U)           /*!<  EndPoint TX STATus bit field */
#define  USB_EPADDR_FIELD                     ((uint32_t)0x0000000FU)           /*!<  EndPoint ADDRess FIELD */

/* EndPoint REGister MASK (no toggle fields) */
#define  USB_EPREG_MASK     (USB_EP_CTR_RX|USB_EP_SETUP|USB_EP_T_FIELD|USB_EP_KIND|USB_EP_CTR_TX|USB_EPADDR_FIELD)
                                                                               /*!< EP_TYPE[1:0] EndPoint TYPE */
#define  USB_EP_TYPE_MASK                     ((uint32_t)0x00000600U)           /*!< EndPoint TYPE Mask */
#define  USB_EP_BULK                          ((uint32_t)0x00000000U)           /*!< EndPoint BULK */
#define  USB_EP_CONTROL                       ((uint32_t)0x00000200U)           /*!< EndPoint CONTROL */
#define  USB_EP_ISOCHRONOUS                   ((uint32_t)0x00000400U)           /*!< EndPoint ISOCHRONOUS */
#define  USB_EP_INTERRUPT                     ((uint32_t)0x00000600U)           /*!< EndPoint INTERRUPT */
#define  USB_EP_T_MASK      (~USB_EP_T_FIELD & USB_EPREG_MASK)
                                                                 
#define  USB_EPKIND_MASK    (~USB_EP_KIND & USB_EPREG_MASK)            /*!< EP_KIND EndPoint KIND */
                                                                               /*!< STAT_TX[1:0] STATus for TX transfer */
#define  USB_EP_TX_DIS                        ((uint32_t)0x00000000U)           /*!< EndPoint TX DISabled */
#define  USB_EP_TX_STALL                      ((uint32_t)0x00000010U)           /*!< EndPoint TX STALLed */
#define  USB_EP_TX_NAK                        ((uint32_t)0x00000020U)           /*!< EndPoint TX NAKed */
#define  USB_EP_TX_VALID                      ((uint32_t)0x00000030U)           /*!< EndPoint TX VALID */
#define  USB_EPTX_DTOG1                       ((uint32_t)0x00000010U)           /*!< EndPoint TX Data TOGgle bit1 */
#define  USB_EPTX_DTOG2                       ((uint32_t)0x00000020U)           /*!< EndPoint TX Data TOGgle bit2 */
#define  USB_EPTX_DTOGMASK  (USB_EPTX_STAT|USB_EPREG_MASK)
                                                                               /*!< STAT_RX[1:0] STATus for RX transfer */
#define  USB_EP_RX_DIS                        ((uint32_t)0x00000000U)           /*!< EndPoint RX DISabled */
#define  USB_EP_RX_STALL                      ((uint32_t)0x00001000U)           /*!< EndPoint RX STALLed */
#define  USB_EP_RX_NAK                        ((uint32_t)0x00002000U)           /*!< EndPoint RX NAKed */
#define  USB_EP_RX_VALID                      ((uint32_t)0x00003000U)           /*!< EndPoint RX VALID */
#define  USB_EPRX_DTOG1                       ((uint32_t)0x00001000U)           /*!< EndPoint RX Data TOGgle bit1 */
#define  USB_EPRX_DTOG2                       ((uint32_t)0x00002000U)           /*!< EndPoint RX Data TOGgle bit1 */
#define  USB_EPRX_DTOGMASK  (USB_EPRX_STAT|USB_EPREG_MASK)

/*******************  Bit definition for USB_EP0R register  *******************/
#define  USB_EP0R_EA                         ((uint32_t)0x0000000FU)            /*!<Endpoint Address */

#define  USB_EP0R_STAT_TX                    ((uint32_t)0x00000030U)            /*!<STAT_TX[1:0] bits (Status bits, for transmission transfers) */
#define  USB_EP0R_STAT_TX_0                  ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define  USB_EP0R_STAT_TX_1                  ((uint32_t)0x00000020U)            /*!<Bit 1 */

#define  USB_EP0R_DTOG_TX                    ((uint32_t)0x00000040U)            /*!<Data Toggle, for transmission transfers */
#define  USB_EP0R_CTR_TX                     ((uint32_t)0x00000080U)            /*!<Correct Transfer for transmission */
#define  USB_EP0R_EP_KIND                    ((uint32_t)0x00000100U)            /*!<Endpoint Kind */

#define  USB_EP0R_EP_TYPE                    ((uint32_t)0x00000600U)            /*!<EP_TYPE[1:0] bits (Endpoint type) */
#define  USB_EP0R_EP_TYPE_0                  ((uint32_t)0x00000200U)            /*!<Bit 0 */
#define  USB_EP0R_EP_TYPE_1                  ((uint32_t)0x00000400U)            /*!<Bit 1 */

#define  USB_EP0R_SETUP                      ((uint32_t)0x00000800U)            /*!<Setup transaction completed */

#define  USB_EP0R_STAT_RX                    ((uint32_t)0x00003000U)            /*!<STAT_RX[1:0] bits (Status bits, for reception transfers) */
#define  USB_EP0R_STAT_RX_0                  ((uint32_t)0x00001000U)            /*!<Bit 0 */
#define  USB_EP0R_STAT_RX_1                  ((uint32_t)0x00002000U)            /*!<Bit 1 */

#define  USB_EP0R_DTOG_RX                    ((uint32_t)0x00004000U)            /*!<Data Toggle, for reception transfers */
#define  USB_EP0R_CTR_RX                     ((uint32_t)0x00008000U)            /*!<Correct Transfer for reception */

/*******************  Bit definition for USB_EP1R register  *******************/
#define  USB_EP1R_EA                         ((uint32_t)0x0000000FU)            /*!<Endpoint Address */

#define  USB_EP1R_STAT_TX                    ((uint32_t)0x00000030U)            /*!<STAT_TX[1:0] bits (Status bits, for transmission transfers) */
#define  USB_EP1R_STAT_TX_0                  ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define  USB_EP1R_STAT_TX_1                  ((uint32_t)0x00000020U)            /*!<Bit 1 */

#define  USB_EP1R_DTOG_TX                    ((uint32_t)0x00000040U)            /*!<Data Toggle, for transmission transfers */
#define  USB_EP1R_CTR_TX                     ((uint32_t)0x00000080U)            /*!<Correct Transfer for transmission */
#define  USB_EP1R_EP_KIND                    ((uint32_t)0x00000100U)            /*!<Endpoint Kind */

#define  USB_EP1R_EP_TYPE                    ((uint32_t)0x00000600U)            /*!<EP_TYPE[1:0] bits (Endpoint type) */
#define  USB_EP1R_EP_TYPE_0                  ((uint32_t)0x00000200U)            /*!<Bit 0 */
#define  USB_EP1R_EP_TYPE_1                  ((uint32_t)0x00000400U)            /*!<Bit 1 */

#define  USB_EP1R_SETUP                      ((uint32_t)0x00000800U)            /*!<Setup transaction completed */

#define  USB_EP1R_STAT_RX                    ((uint32_t)0x00003000U)            /*!<STAT_RX[1:0] bits (Status bits, for reception transfers) */
#define  USB_EP1R_STAT_RX_0                  ((uint32_t)0x00001000U)            /*!<Bit 0 */
#define  USB_EP1R_STAT_RX_1                  ((uint32_t)0x00002000U)            /*!<Bit 1 */

#define  USB_EP1R_DTOG_RX                    ((uint32_t)0x00004000U)            /*!<Data Toggle, for reception transfers */
#define  USB_EP1R_CTR_RX                     ((uint32_t)0x00008000U)            /*!<Correct Transfer for reception */

/*******************  Bit definition for USB_EP2R register  *******************/
#define  USB_EP2R_EA                         ((uint32_t)0x0000000FU)            /*!<Endpoint Address */

#define  USB_EP2R_STAT_TX                    ((uint32_t)0x00000030U)            /*!<STAT_TX[1:0] bits (Status bits, for transmission transfers) */
#define  USB_EP2R_STAT_TX_0                  ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define  USB_EP2R_STAT_TX_1                  ((uint32_t)0x00000020U)            /*!<Bit 1 */

#define  USB_EP2R_DTOG_TX                    ((uint32_t)0x00000040U)            /*!<Data Toggle, for transmission transfers */
#define  USB_EP2R_CTR_TX                     ((uint32_t)0x00000080U)            /*!<Correct Transfer for transmission */
#define  USB_EP2R_EP_KIND                    ((uint32_t)0x00000100U)            /*!<Endpoint Kind */

#define  USB_EP2R_EP_TYPE                    ((uint32_t)0x00000600U)            /*!<EP_TYPE[1:0] bits (Endpoint type) */
#define  USB_EP2R_EP_TYPE_0                  ((uint32_t)0x00000200U)            /*!<Bit 0 */
#define  USB_EP2R_EP_TYPE_1                  ((uint32_t)0x00000400U)            /*!<Bit 1 */

#define  USB_EP2R_SETUP                      ((uint32_t)0x00000800U)            /*!<Setup transaction completed */

#define  USB_EP2R_STAT_RX                    ((uint32_t)0x00003000U)            /*!<STAT_RX[1:0] bits (Status bits, for reception transfers) */
#define  USB_EP2R_STAT_RX_0                  ((uint32_t)0x00001000U)            /*!<Bit 0 */
#define  USB_EP2R_STAT_RX_1                  ((uint32_t)0x00002000U)            /*!<Bit 1 */

#define  USB_EP2R_DTOG_RX                    ((uint32_t)0x00004000U)            /*!<Data Toggle, for reception transfers */
#define  USB_EP2R_CTR_RX                     ((uint32_t)0x00008000U)            /*!<Correct Transfer for reception */

/*******************  Bit definition for USB_EP3R register  *******************/
#define  USB_EP3R_EA                         ((uint32_t)0x0000000FU)            /*!<Endpoint Address */

#define  USB_EP3R_STAT_TX                    ((uint32_t)0x00000030U)            /*!<STAT_TX[1:0] bits (Status bits, for transmission transfers) */
#define  USB_EP3R_STAT_TX_0                  ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define  USB_EP3R_STAT_TX_1                  ((uint32_t)0x00000020U)            /*!<Bit 1 */

#define  USB_EP3R_DTOG_TX                    ((uint32_t)0x00000040U)            /*!<Data Toggle, for transmission transfers */
#define  USB_EP3R_CTR_TX                     ((uint32_t)0x00000080U)            /*!<Correct Transfer for transmission */
#define  USB_EP3R_EP_KIND                    ((uint32_t)0x00000100U)            /*!<Endpoint Kind */

#define  USB_EP3R_EP_TYPE                    ((uint32_t)0x00000600U)            /*!<EP_TYPE[1:0] bits (Endpoint type) */
#define  USB_EP3R_EP_TYPE_0                  ((uint32_t)0x00000200U)            /*!<Bit 0 */
#define  USB_EP3R_EP_TYPE_1                  ((uint32_t)0x00000400U)            /*!<Bit 1 */

#define  USB_EP3R_SETUP                      ((uint32_t)0x00000800U)            /*!<Setup transaction completed */

#define  USB_EP3R_STAT_RX                    ((uint32_t)0x00003000U)            /*!<STAT_RX[1:0] bits (Status bits, for reception transfers) */
#define  USB_EP3R_STAT_RX_0                  ((uint32_t)0x00001000U)            /*!<Bit 0 */
#define  USB_EP3R_STAT_RX_1                  ((uint32_t)0x00002000U)            /*!<Bit 1 */

#define  USB_EP3R_DTOG_RX                    ((uint32_t)0x00004000U)            /*!<Data Toggle, for reception transfers */
#define  USB_EP3R_CTR_RX                     ((uint32_t)0x00008000U)            /*!<Correct Transfer for reception */

/*******************  Bit definition for USB_EP4R register  *******************/
#define  USB_EP4R_EA                         ((uint32_t)0x0000000FU)            /*!<Endpoint Address */

#define  USB_EP4R_STAT_TX                    ((uint32_t)0x00000030U)            /*!<STAT_TX[1:0] bits (Status bits, for transmission transfers) */
#define  USB_EP4R_STAT_TX_0                  ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define  USB_EP4R_STAT_TX_1                  ((uint32_t)0x00000020U)            /*!<Bit 1 */

#define  USB_EP4R_DTOG_TX                    ((uint32_t)0x00000040U)            /*!<Data Toggle, for transmission transfers */
#define  USB_EP4R_CTR_TX                     ((uint32_t)0x00000080U)            /*!<Correct Transfer for transmission */
#define  USB_EP4R_EP_KIND                    ((uint32_t)0x00000100U)            /*!<Endpoint Kind */

#define  USB_EP4R_EP_TYPE                    ((uint32_t)0x00000600U)            /*!<EP_TYPE[1:0] bits (Endpoint type) */
#define  USB_EP4R_EP_TYPE_0                  ((uint32_t)0x00000200U)            /*!<Bit 0 */
#define  USB_EP4R_EP_TYPE_1                  ((uint32_t)0x00000400U)            /*!<Bit 1 */

#define  USB_EP4R_SETUP                      ((uint32_t)0x00000800U)            /*!<Setup transaction completed */

#define  USB_EP4R_STAT_RX                    ((uint32_t)0x00003000U)            /*!<STAT_RX[1:0] bits (Status bits, for reception transfers) */
#define  USB_EP4R_STAT_RX_0                  ((uint32_t)0x00001000U)            /*!<Bit 0 */
#define  USB_EP4R_STAT_RX_1                  ((uint32_t)0x00002000U)            /*!<Bit 1 */

#define  USB_EP4R_DTOG_RX                    ((uint32_t)0x00004000U)            /*!<Data Toggle, for reception transfers */
#define  USB_EP4R_CTR_RX                     ((uint32_t)0x00008000U)            /*!<Correct Transfer for reception */

/*******************  Bit definition for USB_EP5R register  *******************/
#define  USB_EP5R_EA                         ((uint32_t)0x0000000FU)            /*!<Endpoint Address */

#define  USB_EP5R_STAT_TX                    ((uint32_t)0x00000030U)            /*!<STAT_TX[1:0] bits (Status bits, for transmission transfers) */
#define  USB_EP5R_STAT_TX_0                  ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define  USB_EP5R_STAT_TX_1                  ((uint32_t)0x00000020U)            /*!<Bit 1 */

#define  USB_EP5R_DTOG_TX                    ((uint32_t)0x00000040U)            /*!<Data Toggle, for transmission transfers */
#define  USB_EP5R_CTR_TX                     ((uint32_t)0x00000080U)            /*!<Correct Transfer for transmission */
#define  USB_EP5R_EP_KIND                    ((uint32_t)0x00000100U)            /*!<Endpoint Kind */

#define  USB_EP5R_EP_TYPE                    ((uint32_t)0x00000600U)            /*!<EP_TYPE[1:0] bits (Endpoint type) */
#define  USB_EP5R_EP_TYPE_0                  ((uint32_t)0x00000200U)            /*!<Bit 0 */
#define  USB_EP5R_EP_TYPE_1                  ((uint32_t)0x00000400U)            /*!<Bit 1 */

#define  USB_EP5R_SETUP                      ((uint32_t)0x00000800U)            /*!<Setup transaction completed */

#define  USB_EP5R_STAT_RX                    ((uint32_t)0x00003000U)            /*!<STAT_RX[1:0] bits (Status bits, for reception transfers) */
#define  USB_EP5R_STAT_RX_0                  ((uint32_t)0x00001000U)            /*!<Bit 0 */
#define  USB_EP5R_STAT_RX_1                  ((uint32_t)0x00002000U)            /*!<Bit 1 */

#define  USB_EP5R_DTOG_RX                    ((uint32_t)0x00004000U)            /*!<Data Toggle, for reception transfers */
#define  USB_EP5R_CTR_RX                     ((uint32_t)0x00008000U)            /*!<Correct Transfer for reception */

/*******************  Bit definition for USB_EP6R register  *******************/
#define  USB_EP6R_EA                         ((uint32_t)0x0000000FU)            /*!<Endpoint Address */

#define  USB_EP6R_STAT_TX                    ((uint32_t)0x00000030U)            /*!<STAT_TX[1:0] bits (Status bits, for transmission transfers) */
#define  USB_EP6R_STAT_TX_0                  ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define  USB_EP6R_STAT_TX_1                  ((uint32_t)0x00000020U)            /*!<Bit 1 */

#define  USB_EP6R_DTOG_TX                    ((uint32_t)0x00000040U)            /*!<Data Toggle, for transmission transfers */
#define  USB_EP6R_CTR_TX                     ((uint32_t)0x00000080U)            /*!<Correct Transfer for transmission */
#define  USB_EP6R_EP_KIND                    ((uint32_t)0x00000100U)            /*!<Endpoint Kind */

#define  USB_EP6R_EP_TYPE                    ((uint32_t)0x00000600U)            /*!<EP_TYPE[1:0] bits (Endpoint type) */
#define  USB_EP6R_EP_TYPE_0                  ((uint32_t)0x00000200U)            /*!<Bit 0 */
#define  USB_EP6R_EP_TYPE_1                  ((uint32_t)0x00000400U)            /*!<Bit 1 */

#define  USB_EP6R_SETUP                      ((uint32_t)0x00000800U)            /*!<Setup transaction completed */

#define  USB_EP6R_STAT_RX                    ((uint32_t)0x00003000U)            /*!<STAT_RX[1:0] bits (Status bits, for reception transfers) */
#define  USB_EP6R_STAT_RX_0                  ((uint32_t)0x00001000U)            /*!<Bit 0 */
#define  USB_EP6R_STAT_RX_1                  ((uint32_t)0x00002000U)            /*!<Bit 1 */

#define  USB_EP6R_DTOG_RX                    ((uint32_t)0x00004000U)            /*!<Data Toggle, for reception transfers */
#define  USB_EP6R_CTR_RX                     ((uint32_t)0x00008000U)            /*!<Correct Transfer for reception */

/*******************  Bit definition for USB_EP7R register  *******************/
#define  USB_EP7R_EA                         ((uint32_t)0x0000000FU)            /*!<Endpoint Address */

#define  USB_EP7R_STAT_TX                    ((uint32_t)0x00000030U)            /*!<STAT_TX[1:0] bits (Status bits, for transmission transfers) */
#define  USB_EP7R_STAT_TX_0                  ((uint32_t)0x00000010U)            /*!<Bit 0 */
#define  USB_EP7R_STAT_TX_1                  ((uint32_t)0x00000020U)            /*!<Bit 1 */

#define  USB_EP7R_DTOG_TX                    ((uint32_t)0x00000040U)            /*!<Data Toggle, for transmission transfers */
#define  USB_EP7R_CTR_TX                     ((uint32_t)0x00000080U)            /*!<Correct Transfer for transmission */
#define  USB_EP7R_EP_KIND                    ((uint32_t)0x00000100U)            /*!<Endpoint Kind */

#define  USB_EP7R_EP_TYPE                    ((uint32_t)0x00000600U)            /*!<EP_TYPE[1:0] bits (Endpoint type) */
#define  USB_EP7R_EP_TYPE_0                  ((uint32_t)0x00000200U)            /*!<Bit 0 */
#define  USB_EP7R_EP_TYPE_1                  ((uint32_t)0x00000400U)            /*!<Bit 1 */

#define  USB_EP7R_SETUP                      ((uint32_t)0x00000800U)            /*!<Setup transaction completed */

#define  USB_EP7R_STAT_RX                    ((uint32_t)0x00003000U)            /*!<STAT_RX[1:0] bits (Status bits, for reception transfers) */
#define  USB_EP7R_STAT_RX_0                  ((uint32_t)0x00001000U)            /*!<Bit 0 */
#define  USB_EP7R_STAT_RX_1                  ((uint32_t)0x00002000U)            /*!<Bit 1 */

#define  USB_EP7R_DTOG_RX                    ((uint32_t)0x00004000U)            /*!<Data Toggle, for reception transfers */
#define  USB_EP7R_CTR_RX                     ((uint32_t)0x00008000U)            /*!<Correct Transfer for reception */

/*!<Common registers */

#define  USB_CNTR                             (USB_BASE + 0x00000040U)          /*!< Control register */
#define  USB_ISTR                             (USB_BASE + 0x00000044U)          /*!< Interrupt status register */
#define  USB_FNR                              (USB_BASE + 0x00000048U)          /*!< Frame number register */
#define  USB_DADDR                            (USB_BASE + 0x0000004CU)          /*!< Device address register */
#define  USB_BTABLE                           (USB_BASE + 0x00000050U)          /*!< Buffer Table address register */



/*******************  Bit definition for USB_CNTR register  *******************/
#define  USB_CNTR_FRES                       ((uint32_t)0x00000001U)            /*!<Force USB Reset */
#define  USB_CNTR_PDWN                       ((uint32_t)0x00000002U)            /*!<Power down */
#define  USB_CNTR_LPMODE                     ((uint32_t)0x00000004U)            /*!<Low-power mode */
#define  USB_CNTR_FSUSP                      ((uint32_t)0x00000008U)            /*!<Force suspend */
#define  USB_CNTR_RESUME                     ((uint32_t)0x00000010U)            /*!<Resume request */
#define  USB_CNTR_ESOFM                      ((uint32_t)0x00000100U)            /*!<Expected Start Of Frame Interrupt Mask */
#define  USB_CNTR_SOFM                       ((uint32_t)0x00000200U)            /*!<Start Of Frame Interrupt Mask */
#define  USB_CNTR_RESETM                     ((uint32_t)0x00000400U)            /*!<RESET Interrupt Mask */
#define  USB_CNTR_SUSPM                      ((uint32_t)0x00000800U)            /*!<Suspend mode Interrupt Mask */
#define  USB_CNTR_WKUPM                      ((uint32_t)0x00001000U)            /*!<Wakeup Interrupt Mask */
#define  USB_CNTR_ERRM                       ((uint32_t)0x00002000U)            /*!<Error Interrupt Mask */
#define  USB_CNTR_PMAOVRM                    ((uint32_t)0x00004000U)            /*!<Packet Memory Area Over / Underrun Interrupt Mask */
#define  USB_CNTR_CTRM                       ((uint32_t)0x00008000U)            /*!<Correct Transfer Interrupt Mask */

/*******************  Bit definition for USB_ISTR register  *******************/
#define  USB_ISTR_EP_ID                      ((uint32_t)0x0000000FU)            /*!<Endpoint Identifier */
#define  USB_ISTR_DIR                        ((uint32_t)0x00000010U)            /*!<Direction of transaction */
#define  USB_ISTR_ESOF                       ((uint32_t)0x00000100U)            /*!<Expected Start Of Frame */
#define  USB_ISTR_SOF                        ((uint32_t)0x00000200U)            /*!<Start Of Frame */
#define  USB_ISTR_RESET                      ((uint32_t)0x00000400U)            /*!<USB RESET request */
#define  USB_ISTR_SUSP                       ((uint32_t)0x00000800U)            /*!<Suspend mode request */
#define  USB_ISTR_WKUP                       ((uint32_t)0x00001000U)            /*!<Wake up */
#define  USB_ISTR_ERR                        ((uint32_t)0x00002000U)            /*!<Error */
#define  USB_ISTR_PMAOVR                     ((uint32_t)0x00004000U)            /*!<Packet Memory Area Over / Underrun */
#define  USB_ISTR_CTR                        ((uint32_t)0x00008000U)            /*!<Correct Transfer */

#define  USB_CLR_CTR                          (~USB_ISTR_CTR)                  /*!< clear Correct TRansfer bit */
#define  USB_CLR_PMAOVRM                      (~USB_ISTR_PMAOVR)               /*!< clear DMA OVeR/underrun bit*/
#define  USB_CLR_ERR                          (~USB_ISTR_ERR)                  /*!< clear ERRor bit */
#define  USB_CLR_WKUP                         (~USB_ISTR_WKUP)                 /*!< clear WaKe UP bit */
#define  USB_CLR_SUSP                         (~USB_ISTR_SUSP)                 /*!< clear SUSPend bit */
#define  USB_CLR_RESET                        (~USB_ISTR_RESET)                /*!< clear RESET bit */
#define  USB_CLR_SOF                          (~USB_ISTR_SOF)                  /*!< clear Start Of Frame bit */
#define  USB_CLR_ESOF                         (~USB_ISTR_ESOF)                 /*!< clear Expected Start Of Frame bit */


/*******************  Bit definition for USB_FNR register  ********************/
#define  USB_FNR_FN                          ((uint32_t)0x000007FFU)            /*!<Frame Number */
#define  USB_FNR_LSOF                        ((uint32_t)0x00001800U)            /*!<Lost SOF */
#define  USB_FNR_LCK                         ((uint32_t)0x00002000U)            /*!<Locked */
#define  USB_FNR_RXDM                        ((uint32_t)0x00004000U)            /*!<Receive Data - Line Status */
#define  USB_FNR_RXDP                        ((uint32_t)0x00008000U)            /*!<Receive Data + Line Status */

/******************  Bit definition for USB_DADDR register  *******************/
#define  USB_DADDR_ADD                       ((uint32_t)0x0000007FU)               /*!<ADD[6:0] bits (Device Address) */
#define  USB_DADDR_ADD0                      ((uint32_t)0x00000001U)               /*!<Bit 0 */
#define  USB_DADDR_ADD1                      ((uint32_t)0x00000002U)               /*!<Bit 1 */
#define  USB_DADDR_ADD2                      ((uint32_t)0x00000004U)               /*!<Bit 2 */
#define  USB_DADDR_ADD3                      ((uint32_t)0x00000008U)               /*!<Bit 3 */
#define  USB_DADDR_ADD4                      ((uint32_t)0x00000010U)               /*!<Bit 4 */
#define  USB_DADDR_ADD5                      ((uint32_t)0x00000020U)               /*!<Bit 5 */
#define  USB_DADDR_ADD6                      ((uint32_t)0x00000040U)               /*!<Bit 6 */

#define  USB_DADDR_EF                        ((uint32_t)0x00000080U)               /*!<Enable Function */

/******************  Bit definition for USB_BTABLE register  ******************/    
#define  USB_BTABLE_BTABLE                   ((uint32_t)0x0000FFF8U)            /*!<Buffer Table */

/*!< Buffer descriptor table */
/*****************  Bit definition for USB_ADDR0_TX register  *****************/
#define  USB_ADDR0_TX_ADDR0_TX               ((uint32_t)0x0000FFFEU)            /*!< Transmission Buffer Address 0 */

/*****************  Bit definition for USB_ADDR1_TX register  *****************/
#define  USB_ADDR1_TX_ADDR1_TX               ((uint32_t)0x0000FFFEU)            /*!< Transmission Buffer Address 1 */

/*****************  Bit definition for USB_ADDR2_TX register  *****************/
#define  USB_ADDR2_TX_ADDR2_TX               ((uint32_t)0x0000FFFEU)            /*!< Transmission Buffer Address 2 */

/*****************  Bit definition for USB_ADDR3_TX register  *****************/
#define  USB_ADDR3_TX_ADDR3_TX               ((uint32_t)0x0000FFFEU)            /*!< Transmission Buffer Address 3 */

/*****************  Bit definition for USB_ADDR4_TX register  *****************/
#define  USB_ADDR4_TX_ADDR4_TX               ((uint32_t)0x0000FFFEU)            /*!< Transmission Buffer Address 4 */

/*****************  Bit definition for USB_ADDR5_TX register  *****************/
#define  USB_ADDR5_TX_ADDR5_TX               ((uint32_t)0x0000FFFEU)            /*!< Transmission Buffer Address 5 */

/*****************  Bit definition for USB_ADDR6_TX register  *****************/
#define  USB_ADDR6_TX_ADDR6_TX               ((uint32_t)0x0000FFFEU)            /*!< Transmission Buffer Address 6 */

/*****************  Bit definition for USB_ADDR7_TX register  *****************/
#define  USB_ADDR7_TX_ADDR7_TX               ((uint32_t)0x0000FFFEU)            /*!< Transmission Buffer Address 7 */

/*----------------------------------------------------------------------------*/

/*****************  Bit definition for USB_COUNT0_TX register  ****************/
#define  USB_COUNT0_TX_COUNT0_TX             ((uint32_t)0x000003FFU)            /*!< Transmission Byte Count 0 */

/*****************  Bit definition for USB_COUNT1_TX register  ****************/
#define  USB_COUNT1_TX_COUNT1_TX             ((uint32_t)0x000003FFU)            /*!< Transmission Byte Count 1 */

/*****************  Bit definition for USB_COUNT2_TX register  ****************/
#define  USB_COUNT2_TX_COUNT2_TX             ((uint32_t)0x000003FFU)            /*!< Transmission Byte Count 2 */

/*****************  Bit definition for USB_COUNT3_TX register  ****************/
#define  USB_COUNT3_TX_COUNT3_TX             ((uint32_t)0x000003FFU)            /*!< Transmission Byte Count 3 */

/*****************  Bit definition for USB_COUNT4_TX register  ****************/
#define  USB_COUNT4_TX_COUNT4_TX             ((uint32_t)0x000003FFU)            /*!< Transmission Byte Count 4 */

/*****************  Bit definition for USB_COUNT5_TX register  ****************/
#define  USB_COUNT5_TX_COUNT5_TX             ((uint32_t)0x000003FFU)            /*!< Transmission Byte Count 5 */

/*****************  Bit definition for USB_COUNT6_TX register  ****************/
#define  USB_COUNT6_TX_COUNT6_TX             ((uint32_t)0x000003FFU)            /*!< Transmission Byte Count 6 */

/*****************  Bit definition for USB_COUNT7_TX register  ****************/
#define  USB_COUNT7_TX_COUNT7_TX             ((uint32_t)0x000003FFU)            /*!< Transmission Byte Count 7 */

/*----------------------------------------------------------------------------*/

/****************  Bit definition for USB_COUNT0_TX_0 register  ***************/
#define  USB_COUNT0_TX_0_COUNT0_TX_0         ((uint32_t)0x000003FFU)        /*!< Transmission Byte Count 0 (low) */

/****************  Bit definition for USB_COUNT0_TX_1 register  ***************/
#define  USB_COUNT0_TX_1_COUNT0_TX_1         ((uint32_t)0x03FF0000U)        /*!< Transmission Byte Count 0 (high) */

/****************  Bit definition for USB_COUNT1_TX_0 register  ***************/
#define  USB_COUNT1_TX_0_COUNT1_TX_0          ((uint32_t)0x000003FFU)        /*!< Transmission Byte Count 1 (low) */

/****************  Bit definition for USB_COUNT1_TX_1 register  ***************/
#define  USB_COUNT1_TX_1_COUNT1_TX_1          ((uint32_t)0x03FF0000U)        /*!< Transmission Byte Count 1 (high) */

/****************  Bit definition for USB_COUNT2_TX_0 register  ***************/
#define  USB_COUNT2_TX_0_COUNT2_TX_0         ((uint32_t)0x000003FFU)        /*!< Transmission Byte Count 2 (low) */

/****************  Bit definition for USB_COUNT2_TX_1 register  ***************/
#define  USB_COUNT2_TX_1_COUNT2_TX_1         ((uint32_t)0x03FF0000U)        /*!< Transmission Byte Count 2 (high) */

/****************  Bit definition for USB_COUNT3_TX_0 register  ***************/
#define  USB_COUNT3_TX_0_COUNT3_TX_0         ((uint32_t)0x00000000U03FF)        /*!< Transmission Byte Count 3 (low) */

/****************  Bit definition for USB_COUNT3_TX_1 register  ***************/
#define  USB_COUNT3_TX_1_COUNT3_TX_1         ((uint32_t)0x000003FFU0000)        /*!< Transmission Byte Count 3 (high) */

/****************  Bit definition for USB_COUNT4_TX_0 register  ***************/
#define  USB_COUNT4_TX_0_COUNT4_TX_0         ((uint32_t)0x000003FFU)        /*!< Transmission Byte Count 4 (low) */

/****************  Bit definition for USB_COUNT4_TX_1 register  ***************/
#define  USB_COUNT4_TX_1_COUNT4_TX_1         ((uint32_t)0x03FF0000U)        /*!< Transmission Byte Count 4 (high) */

/****************  Bit definition for USB_COUNT5_TX_0 register  ***************/
#define  USB_COUNT5_TX_0_COUNT5_TX_0         ((uint32_t)0x000003FFU)        /*!< Transmission Byte Count 5 (low) */

/****************  Bit definition for USB_COUNT5_TX_1 register  ***************/
#define  USB_COUNT5_TX_1_COUNT5_TX_1         ((uint32_t)0x03FF0000U)        /*!< Transmission Byte Count 5 (high) */

/****************  Bit definition for USB_COUNT6_TX_0 register  ***************/
#define  USB_COUNT6_TX_0_COUNT6_TX_0         ((uint32_t)0x000003FFU)        /*!< Transmission Byte Count 6 (low) */

/****************  Bit definition for USB_COUNT6_TX_1 register  ***************/
#define  USB_COUNT6_TX_1_COUNT6_TX_1         ((uint32_t)0x03FF0000U)        /*!< Transmission Byte Count 6 (high) */

/****************  Bit definition for USB_COUNT7_TX_0 register  ***************/
#define  USB_COUNT7_TX_0_COUNT7_TX_0         ((uint32_t)0x000003FFU)        /*!< Transmission Byte Count 7 (low) */

/****************  Bit definition for USB_COUNT7_TX_1 register  ***************/
#define  USB_COUNT7_TX_1_COUNT7_TX_1         ((uint32_t)0x03FF0000U)        /*!< Transmission Byte Count 7 (high) */

/*----------------------------------------------------------------------------*/

/*****************  Bit definition for USB_ADDR0_RX register  *****************/
#define  USB_ADDR0_RX_ADDR0_RX               ((uint32_t)0x0000FFFEU)            /*!< Reception Buffer Address 0 */

/*****************  Bit definition for USB_ADDR1_RX register  *****************/
#define  USB_ADDR1_RX_ADDR1_RX               ((uint32_t)0x0000FFFEU)            /*!< Reception Buffer Address 1 */

/*****************  Bit definition for USB_ADDR2_RX register  *****************/
#define  USB_ADDR2_RX_ADDR2_RX               ((uint32_t)0x0000FFFEU)            /*!< Reception Buffer Address 2 */

/*****************  Bit definition for USB_ADDR3_RX register  *****************/
#define  USB_ADDR3_RX_ADDR3_RX               ((uint32_t)0x0000FFFEU)            /*!< Reception Buffer Address 3 */

/*****************  Bit definition for USB_ADDR4_RX register  *****************/
#define  USB_ADDR4_RX_ADDR4_RX               ((uint32_t)0x0000FFFEU)            /*!< Reception Buffer Address 4 */

/*****************  Bit definition for USB_ADDR5_RX register  *****************/
#define  USB_ADDR5_RX_ADDR5_RX               ((uint32_t)0x0000FFFEU)            /*!< Reception Buffer Address 5 */

/*****************  Bit definition for USB_ADDR6_RX register  *****************/
#define  USB_ADDR6_RX_ADDR6_RX               ((uint32_t)0x0000FFFEU)            /*!< Reception Buffer Address 6 */

/*****************  Bit definition for USB_ADDR7_RX register  *****************/
#define  USB_ADDR7_RX_ADDR7_RX               ((uint32_t)0x0000FFFEU)            /*!< Reception Buffer Address 7 */

/*----------------------------------------------------------------------------*/

/*****************  Bit definition for USB_COUNT0_RX register  ****************/
#define  USB_COUNT0_RX_COUNT0_RX             ((uint32_t)0x000003FFU)            /*!< Reception Byte Count */

#define  USB_COUNT0_RX_NUM_BLOCK             ((uint32_t)0x00007C00U)            /*!< NUM_BLOCK[4:0] bits (Number of blocks) */
#define  USB_COUNT0_RX_NUM_BLOCK_0           ((uint32_t)0x00000400U)            /*!< Bit 0 */
#define  USB_COUNT0_RX_NUM_BLOCK_1           ((uint32_t)0x00000800U)            /*!< Bit 1 */
#define  USB_COUNT0_RX_NUM_BLOCK_2           ((uint32_t)0x00001000U)            /*!< Bit 2 */
#define  USB_COUNT0_RX_NUM_BLOCK_3           ((uint32_t)0x00002000U)            /*!< Bit 3 */
#define  USB_COUNT0_RX_NUM_BLOCK_4           ((uint32_t)0x00004000U)            /*!< Bit 4 */

#define  USB_COUNT0_RX_BLSIZE                ((uint32_t)0x00008000U)            /*!< BLock SIZE */

/*****************  Bit definition for USB_COUNT1_RX register  ****************/
#define  USB_COUNT1_RX_COUNT1_RX             ((uint32_t)0x000003FFU)            /*!< Reception Byte Count */

#define  USB_COUNT1_RX_NUM_BLOCK             ((uint32_t)0x00007C00U)            /*!< NUM_BLOCK[4:0] bits (Number of blocks) */
#define  USB_COUNT1_RX_NUM_BLOCK_0           ((uint32_t)0x00000400U)            /*!< Bit 0 */
#define  USB_COUNT1_RX_NUM_BLOCK_1           ((uint32_t)0x00000800U)            /*!< Bit 1 */
#define  USB_COUNT1_RX_NUM_BLOCK_2           ((uint32_t)0x00001000U)            /*!< Bit 2 */
#define  USB_COUNT1_RX_NUM_BLOCK_3           ((uint32_t)0x00002000U)            /*!< Bit 3 */
#define  USB_COUNT1_RX_NUM_BLOCK_4           ((uint32_t)0x00004000U)            /*!< Bit 4 */

#define  USB_COUNT1_RX_BLSIZE                ((uint32_t)0x00008000U)            /*!< BLock SIZE */

/*****************  Bit definition for USB_COUNT2_RX register  ****************/
#define  USB_COUNT2_RX_COUNT2_RX             ((uint32_t)0x000003FFU)            /*!< Reception Byte Count */

#define  USB_COUNT2_RX_NUM_BLOCK             ((uint32_t)0x00007C00U)            /*!< NUM_BLOCK[4:0] bits (Number of blocks) */
#define  USB_COUNT2_RX_NUM_BLOCK_0           ((uint32_t)0x00000400U)            /*!< Bit 0 */
#define  USB_COUNT2_RX_NUM_BLOCK_1           ((uint32_t)0x00000800U)            /*!< Bit 1 */
#define  USB_COUNT2_RX_NUM_BLOCK_2           ((uint32_t)0x00001000U)            /*!< Bit 2 */
#define  USB_COUNT2_RX_NUM_BLOCK_3           ((uint32_t)0x00002000U)            /*!< Bit 3 */
#define  USB_COUNT2_RX_NUM_BLOCK_4           ((uint32_t)0x00004000U)            /*!< Bit 4 */

#define  USB_COUNT2_RX_BLSIZE                ((uint32_t)0x00008000U)            /*!< BLock SIZE */

/*****************  Bit definition for USB_COUNT3_RX register  ****************/
#define  USB_COUNT3_RX_COUNT3_RX             ((uint32_t)0x000003FFU)            /*!< Reception Byte Count */

#define  USB_COUNT3_RX_NUM_BLOCK             ((uint32_t)0x00007C00U)            /*!< NUM_BLOCK[4:0] bits (Number of blocks) */
#define  USB_COUNT3_RX_NUM_BLOCK_0           ((uint32_t)0x00000400U)            /*!< Bit 0 */
#define  USB_COUNT3_RX_NUM_BLOCK_1           ((uint32_t)0x00000800U)            /*!< Bit 1 */
#define  USB_COUNT3_RX_NUM_BLOCK_2           ((uint32_t)0x00001000U)            /*!< Bit 2 */
#define  USB_COUNT3_RX_NUM_BLOCK_3           ((uint32_t)0x00002000U)            /*!< Bit 3 */
#define  USB_COUNT3_RX_NUM_BLOCK_4           ((uint32_t)0x00004000U)            /*!< Bit 4 */

#define  USB_COUNT3_RX_BLSIZE                ((uint32_t)0x00008000U)            /*!< BLock SIZE */

/*****************  Bit definition for USB_COUNT4_RX register  ****************/
#define  USB_COUNT4_RX_COUNT4_RX             ((uint32_t)0x000003FFU)            /*!< Reception Byte Count */

#define  USB_COUNT4_RX_NUM_BLOCK             ((uint32_t)0x00007C00U)            /*!< NUM_BLOCK[4:0] bits (Number of blocks) */
#define  USB_COUNT4_RX_NUM_BLOCK_0           ((uint32_t)0x00000400U)            /*!< Bit 0 */
#define  USB_COUNT4_RX_NUM_BLOCK_1           ((uint32_t)0x00000800U)            /*!< Bit 1 */
#define  USB_COUNT4_RX_NUM_BLOCK_2           ((uint32_t)0x00001000U)            /*!< Bit 2 */
#define  USB_COUNT4_RX_NUM_BLOCK_3           ((uint32_t)0x00002000U)            /*!< Bit 3 */
#define  USB_COUNT4_RX_NUM_BLOCK_4           ((uint32_t)0x00004000U)            /*!< Bit 4 */

#define  USB_COUNT4_RX_BLSIZE                ((uint32_t)0x00008000U)            /*!< BLock SIZE */

/*****************  Bit definition for USB_COUNT5_RX register  ****************/
#define  USB_COUNT5_RX_COUNT5_RX             ((uint32_t)0x000003FFU)            /*!< Reception Byte Count */

#define  USB_COUNT5_RX_NUM_BLOCK             ((uint32_t)0x00007C00U)            /*!< NUM_BLOCK[4:0] bits (Number of blocks) */
#define  USB_COUNT5_RX_NUM_BLOCK_0           ((uint32_t)0x00000400U)            /*!< Bit 0 */
#define  USB_COUNT5_RX_NUM_BLOCK_1           ((uint32_t)0x00000800U)            /*!< Bit 1 */
#define  USB_COUNT5_RX_NUM_BLOCK_2           ((uint32_t)0x00001000U)            /*!< Bit 2 */
#define  USB_COUNT5_RX_NUM_BLOCK_3           ((uint32_t)0x00002000U)            /*!< Bit 3 */
#define  USB_COUNT5_RX_NUM_BLOCK_4           ((uint32_t)0x00004000U)            /*!< Bit 4 */

#define  USB_COUNT5_RX_BLSIZE                ((uint32_t)0x00008000U)            /*!< BLock SIZE */

/*****************  Bit definition for USB_COUNT6_RX register  ****************/
#define  USB_COUNT6_RX_COUNT6_RX             ((uint32_t)0x000003FFU)            /*!< Reception Byte Count */

#define  USB_COUNT6_RX_NUM_BLOCK             ((uint32_t)0x00007C00U)            /*!< NUM_BLOCK[4:0] bits (Number of blocks) */
#define  USB_COUNT6_RX_NUM_BLOCK_0           ((uint32_t)0x00000400U)            /*!< Bit 0 */
#define  USB_COUNT6_RX_NUM_BLOCK_1           ((uint32_t)0x00000800U)            /*!< Bit 1 */
#define  USB_COUNT6_RX_NUM_BLOCK_2           ((uint32_t)0x00001000U)            /*!< Bit 2 */
#define  USB_COUNT6_RX_NUM_BLOCK_3           ((uint32_t)0x00002000U)            /*!< Bit 3 */
#define  USB_COUNT6_RX_NUM_BLOCK_4           ((uint32_t)0x00004000U)            /*!< Bit 4 */

#define  USB_COUNT6_RX_BLSIZE                ((uint32_t)0x00008000U)            /*!< BLock SIZE */

/*****************  Bit definition for USB_COUNT7_RX register  ****************/
#define  USB_COUNT7_RX_COUNT7_RX             ((uint32_t)0x000003FFU)            /*!< Reception Byte Count */

#define  USB_COUNT7_RX_NUM_BLOCK             ((uint32_t)0x00007C00U)            /*!< NUM_BLOCK[4:0] bits (Number of blocks) */
#define  USB_COUNT7_RX_NUM_BLOCK_0           ((uint32_t)0x00000400U)            /*!< Bit 0 */
#define  USB_COUNT7_RX_NUM_BLOCK_1           ((uint32_t)0x00000800U)            /*!< Bit 1 */
#define  USB_COUNT7_RX_NUM_BLOCK_2           ((uint32_t)0x00001000U)            /*!< Bit 2 */
#define  USB_COUNT7_RX_NUM_BLOCK_3           ((uint32_t)0x00002000U)            /*!< Bit 3 */
#define  USB_COUNT7_RX_NUM_BLOCK_4           ((uint32_t)0x00004000U)            /*!< Bit 4 */

#define  USB_COUNT7_RX_BLSIZE                ((uint32_t)0x00008000U)            /*!< BLock SIZE */

/*----------------------------------------------------------------------------*/

/****************  Bit definition for USB_COUNT0_RX_0 register  ***************/
#define  USB_COUNT0_RX_0_COUNT0_RX_0         ((uint32_t)0x000003FFU)        /*!< Reception Byte Count (low) */

#define  USB_COUNT0_RX_0_NUM_BLOCK_0         ((uint32_t)0x00007C00U)        /*!< NUM_BLOCK_0[4:0] bits (Number of blocks) (low) */
#define  USB_COUNT0_RX_0_NUM_BLOCK_0_0       ((uint32_t)0x00000400U)        /*!< Bit 0 */
#define  USB_COUNT0_RX_0_NUM_BLOCK_0_1       ((uint32_t)0x00000800U)        /*!< Bit 1 */
#define  USB_COUNT0_RX_0_NUM_BLOCK_0_2       ((uint32_t)0x00001000U)        /*!< Bit 2 */
#define  USB_COUNT0_RX_0_NUM_BLOCK_0_3       ((uint32_t)0x00002000U)        /*!< Bit 3 */
#define  USB_COUNT0_RX_0_NUM_BLOCK_0_4       ((uint32_t)0x00004000U)        /*!< Bit 4 */

#define  USB_COUNT0_RX_0_BLSIZE_0            ((uint32_t)0x00008000U)        /*!< BLock SIZE (low) */

/****************  Bit definition for USB_COUNT0_RX_1 register  ***************/
#define  USB_COUNT0_RX_1_COUNT0_RX_1         ((uint32_t)0x03FF0000U)        /*!< Reception Byte Count (high) */

#define  USB_COUNT0_RX_1_NUM_BLOCK_1         ((uint32_t)0x7C000000U)        /*!< NUM_BLOCK_1[4:0] bits (Number of blocks) (high) */
#define  USB_COUNT0_RX_1_NUM_BLOCK_1_0       ((uint32_t)0x04000000U)        /*!< Bit 1 */
#define  USB_COUNT0_RX_1_NUM_BLOCK_1_1       ((uint32_t)0x08000000U)        /*!< Bit 1 */
#define  USB_COUNT0_RX_1_NUM_BLOCK_1_2       ((uint32_t)0x10000000U)        /*!< Bit 2 */
#define  USB_COUNT0_RX_1_NUM_BLOCK_1_3       ((uint32_t)0x20000000U)        /*!< Bit 3 */
#define  USB_COUNT0_RX_1_NUM_BLOCK_1_4       ((uint32_t)0x40000000U)        /*!< Bit 4 */

#define  USB_COUNT0_RX_1_BLSIZE_1            ((uint32_t)0x80000000U)        /*!< BLock SIZE (high) */

/****************  Bit definition for USB_COUNT1_RX_0 register  ***************/
#define  USB_COUNT1_RX_0_COUNT1_RX_0         ((uint32_t)0x000003FFU)        /*!< Reception Byte Count (low) */

#define  USB_COUNT1_RX_0_NUM_BLOCK_0         ((uint32_t)0x00007C00U)        /*!< NUM_BLOCK_0[4:0] bits (Number of blocks) (low) */
#define  USB_COUNT1_RX_0_NUM_BLOCK_0_0       ((uint32_t)0x00000400U)        /*!< Bit 0 */
#define  USB_COUNT1_RX_0_NUM_BLOCK_0_1       ((uint32_t)0x00000800U)        /*!< Bit 1 */
#define  USB_COUNT1_RX_0_NUM_BLOCK_0_2       ((uint32_t)0x00001000U)        /*!< Bit 2 */
#define  USB_COUNT1_RX_0_NUM_BLOCK_0_3       ((uint32_t)0x00002000U)        /*!< Bit 3 */
#define  USB_COUNT1_RX_0_NUM_BLOCK_0_4       ((uint32_t)0x00004000U)        /*!< Bit 4 */

#define  USB_COUNT1_RX_0_BLSIZE_0            ((uint32_t)0x00008000U)        /*!< BLock SIZE (low) */

/****************  Bit definition for USB_COUNT1_RX_1 register  ***************/
#define  USB_COUNT1_RX_1_COUNT1_RX_1         ((uint32_t)0x03FF0000U)        /*!< Reception Byte Count (high) */

#define  USB_COUNT1_RX_1_NUM_BLOCK_1         ((uint32_t)0x7C000000U)        /*!< NUM_BLOCK_1[4:0] bits (Number of blocks) (high) */
#define  USB_COUNT1_RX_1_NUM_BLOCK_1_0       ((uint32_t)0x04000000U)        /*!< Bit 0 */
#define  USB_COUNT1_RX_1_NUM_BLOCK_1_1       ((uint32_t)0x08000000U)        /*!< Bit 1 */
#define  USB_COUNT1_RX_1_NUM_BLOCK_1_2       ((uint32_t)0x10000000U)        /*!< Bit 2 */
#define  USB_COUNT1_RX_1_NUM_BLOCK_1_3       ((uint32_t)0x20000000U)        /*!< Bit 3 */
#define  USB_COUNT1_RX_1_NUM_BLOCK_1_4       ((uint32_t)0x40000000U)        /*!< Bit 4 */

#define  USB_COUNT1_RX_1_BLSIZE_1            ((uint32_t)0x80000000U)        /*!< BLock SIZE (high) */

/****************  Bit definition for USB_COUNT2_RX_0 register  ***************/
#define  USB_COUNT2_RX_0_COUNT2_RX_0         ((uint32_t)0x000003FFU)        /*!< Reception Byte Count (low) */

#define  USB_COUNT2_RX_0_NUM_BLOCK_0         ((uint32_t)0x00007C00U)        /*!< NUM_BLOCK_0[4:0] bits (Number of blocks) (low) */
#define  USB_COUNT2_RX_0_NUM_BLOCK_0_0       ((uint32_t)0x00000400U)        /*!< Bit 0 */
#define  USB_COUNT2_RX_0_NUM_BLOCK_0_1       ((uint32_t)0x00000800U)        /*!< Bit 1 */
#define  USB_COUNT2_RX_0_NUM_BLOCK_0_2       ((uint32_t)0x00001000U)        /*!< Bit 2 */
#define  USB_COUNT2_RX_0_NUM_BLOCK_0_3       ((uint32_t)0x00002000U)        /*!< Bit 3 */
#define  USB_COUNT2_RX_0_NUM_BLOCK_0_4       ((uint32_t)0x00004000U)        /*!< Bit 4 */

#define  USB_COUNT2_RX_0_BLSIZE_0            ((uint32_t)0x00008000U)        /*!< BLock SIZE (low) */

/****************  Bit definition for USB_COUNT2_RX_1 register  ***************/
#define  USB_COUNT2_RX_1_COUNT2_RX_1         ((uint32_t)0x03FF0000U)        /*!< Reception Byte Count (high) */

#define  USB_COUNT2_RX_1_NUM_BLOCK_1         ((uint32_t)0x7C000000U)        /*!< NUM_BLOCK_1[4:0] bits (Number of blocks) (high) */
#define  USB_COUNT2_RX_1_NUM_BLOCK_1_0       ((uint32_t)0x04000000U)        /*!< Bit 0 */
#define  USB_COUNT2_RX_1_NUM_BLOCK_1_1       ((uint32_t)0x08000000U)        /*!< Bit 1 */
#define  USB_COUNT2_RX_1_NUM_BLOCK_1_2       ((uint32_t)0x10000000U)        /*!< Bit 2 */
#define  USB_COUNT2_RX_1_NUM_BLOCK_1_3       ((uint32_t)0x20000000U)        /*!< Bit 3 */
#define  USB_COUNT2_RX_1_NUM_BLOCK_1_4       ((uint32_t)0x40000000U)        /*!< Bit 4 */

#define  USB_COUNT2_RX_1_BLSIZE_1            ((uint32_t)0x80000000U)        /*!< BLock SIZE (high) */

/****************  Bit definition for USB_COUNT3_RX_0 register  ***************/
#define  USB_COUNT3_RX_0_COUNT3_RX_0         ((uint32_t)0x000003FFU)        /*!< Reception Byte Count (low) */

#define  USB_COUNT3_RX_0_NUM_BLOCK_0         ((uint32_t)0x00007C00U)        /*!< NUM_BLOCK_0[4:0] bits (Number of blocks) (low) */
#define  USB_COUNT3_RX_0_NUM_BLOCK_0_0       ((uint32_t)0x00000400U)        /*!< Bit 0 */
#define  USB_COUNT3_RX_0_NUM_BLOCK_0_1       ((uint32_t)0x00000800U)        /*!< Bit 1 */
#define  USB_COUNT3_RX_0_NUM_BLOCK_0_2       ((uint32_t)0x00001000U)        /*!< Bit 2 */
#define  USB_COUNT3_RX_0_NUM_BLOCK_0_3       ((uint32_t)0x00002000U)        /*!< Bit 3 */
#define  USB_COUNT3_RX_0_NUM_BLOCK_0_4       ((uint32_t)0x00004000U)        /*!< Bit 4 */

#define  USB_COUNT3_RX_0_BLSIZE_0            ((uint32_t)0x00008000U)        /*!< BLock SIZE (low) */

/****************  Bit definition for USB_COUNT3_RX_1 register  ***************/
#define  USB_COUNT3_RX_1_COUNT3_RX_1         ((uint32_t)0x03FF0000U)        /*!< Reception Byte Count (high) */

#define  USB_COUNT3_RX_1_NUM_BLOCK_1         ((uint32_t)0x7C000000U)        /*!< NUM_BLOCK_1[4:0] bits (Number of blocks) (high) */
#define  USB_COUNT3_RX_1_NUM_BLOCK_1_0       ((uint32_t)0x04000000U)        /*!< Bit 0 */
#define  USB_COUNT3_RX_1_NUM_BLOCK_1_1       ((uint32_t)0x08000000U)        /*!< Bit 1 */
#define  USB_COUNT3_RX_1_NUM_BLOCK_1_2       ((uint32_t)0x10000000U)        /*!< Bit 2 */
#define  USB_COUNT3_RX_1_NUM_BLOCK_1_3       ((uint32_t)0x20000000U)        /*!< Bit 3 */
#define  USB_COUNT3_RX_1_NUM_BLOCK_1_4       ((uint32_t)0x40000000U)        /*!< Bit 4 */

#define  USB_COUNT3_RX_1_BLSIZE_1            ((uint32_t)0x80000000U)        /*!< BLock SIZE (high) */

/****************  Bit definition for USB_COUNT4_RX_0 register  ***************/
#define  USB_COUNT4_RX_0_COUNT4_RX_0         ((uint32_t)0x000003FFU)        /*!< Reception Byte Count (low) */

#define  USB_COUNT4_RX_0_NUM_BLOCK_0         ((uint32_t)0x00007C00U)        /*!< NUM_BLOCK_0[4:0] bits (Number of blocks) (low) */
#define  USB_COUNT4_RX_0_NUM_BLOCK_0_0      ((uint32_t)0x00000400U)        /*!< Bit 0 */
#define  USB_COUNT4_RX_0_NUM_BLOCK_0_1      ((uint32_t)0x00000800U)        /*!< Bit 1 */
#define  USB_COUNT4_RX_0_NUM_BLOCK_0_2      ((uint32_t)0x00001000U)        /*!< Bit 2 */
#define  USB_COUNT4_RX_0_NUM_BLOCK_0_3      ((uint32_t)0x00002000U)        /*!< Bit 3 */
#define  USB_COUNT4_RX_0_NUM_BLOCK_0_4      ((uint32_t)0x00004000U)        /*!< Bit 4 */

#define  USB_COUNT4_RX_0_BLSIZE_0            ((uint32_t)0x00008000U)        /*!< BLock SIZE (low) */

/****************  Bit definition for USB_COUNT4_RX_1 register  ***************/
#define  USB_COUNT4_RX_1_COUNT4_RX_1         ((uint32_t)0x03FF0000U)        /*!< Reception Byte Count (high) */

#define  USB_COUNT4_RX_1_NUM_BLOCK_1         ((uint32_t)0x7C000000U)        /*!< NUM_BLOCK_1[4:0] bits (Number of blocks) (high) */
#define  USB_COUNT4_RX_1_NUM_BLOCK_1_0       ((uint32_t)0x04000000U)        /*!< Bit 0 */
#define  USB_COUNT4_RX_1_NUM_BLOCK_1_1       ((uint32_t)0x08000000U)        /*!< Bit 1 */
#define  USB_COUNT4_RX_1_NUM_BLOCK_1_2       ((uint32_t)0x10000000U)        /*!< Bit 2 */
#define  USB_COUNT4_RX_1_NUM_BLOCK_1_3       ((uint32_t)0x20000000U)        /*!< Bit 3 */
#define  USB_COUNT4_RX_1_NUM_BLOCK_1_4       ((uint32_t)0x40000000U)        /*!< Bit 4 */

#define  USB_COUNT4_RX_1_BLSIZE_1            ((uint32_t)0x80000000U)        /*!< BLock SIZE (high) */

/****************  Bit definition for USB_COUNT5_RX_0 register  ***************/
#define  USB_COUNT5_RX_0_COUNT5_RX_0         ((uint32_t)0x000003FFU)        /*!< Reception Byte Count (low) */

#define  USB_COUNT5_RX_0_NUM_BLOCK_0         ((uint32_t)0x00007C00U)        /*!< NUM_BLOCK_0[4:0] bits (Number of blocks) (low) */
#define  USB_COUNT5_RX_0_NUM_BLOCK_0_0       ((uint32_t)0x00000400U)        /*!< Bit 0 */
#define  USB_COUNT5_RX_0_NUM_BLOCK_0_1       ((uint32_t)0x00000800U)        /*!< Bit 1 */
#define  USB_COUNT5_RX_0_NUM_BLOCK_0_2       ((uint32_t)0x00001000U)        /*!< Bit 2 */
#define  USB_COUNT5_RX_0_NUM_BLOCK_0_3       ((uint32_t)0x00002000U)        /*!< Bit 3 */
#define  USB_COUNT5_RX_0_NUM_BLOCK_0_4       ((uint32_t)0x00004000U)        /*!< Bit 4 */

#define  USB_COUNT5_RX_0_BLSIZE_0            ((uint32_t)0x00008000U)        /*!< BLock SIZE (low) */

/****************  Bit definition for USB_COUNT5_RX_1 register  ***************/
#define  USB_COUNT5_RX_1_COUNT5_RX_1         ((uint32_t)0x03FF0000U)        /*!< Reception Byte Count (high) */

#define  USB_COUNT5_RX_1_NUM_BLOCK_1         ((uint32_t)0x7C000000U)        /*!< NUM_BLOCK_1[4:0] bits (Number of blocks) (high) */
#define  USB_COUNT5_RX_1_NUM_BLOCK_1_0       ((uint32_t)0x04000000U)        /*!< Bit 0 */
#define  USB_COUNT5_RX_1_NUM_BLOCK_1_1       ((uint32_t)0x08000000U)        /*!< Bit 1 */
#define  USB_COUNT5_RX_1_NUM_BLOCK_1_2       ((uint32_t)0x10000000U)        /*!< Bit 2 */
#define  USB_COUNT5_RX_1_NUM_BLOCK_1_3       ((uint32_t)0x20000000U)        /*!< Bit 3 */
#define  USB_COUNT5_RX_1_NUM_BLOCK_1_4       ((uint32_t)0x40000000U)        /*!< Bit 4 */

#define  USB_COUNT5_RX_1_BLSIZE_1            ((uint32_t)0x80000000U)        /*!< BLock SIZE (high) */

/***************  Bit definition for USB_COUNT6_RX_0  register  ***************/
#define  USB_COUNT6_RX_0_COUNT6_RX_0         ((uint32_t)0x000003FFU)        /*!< Reception Byte Count (low) */

#define  USB_COUNT6_RX_0_NUM_BLOCK_0         ((uint32_t)0x00007C00U)        /*!< NUM_BLOCK_0[4:0] bits (Number of blocks) (low) */
#define  USB_COUNT6_RX_0_NUM_BLOCK_0_0       ((uint32_t)0x00000400U)        /*!< Bit 0 */
#define  USB_COUNT6_RX_0_NUM_BLOCK_0_1       ((uint32_t)0x00000800U)        /*!< Bit 1 */
#define  USB_COUNT6_RX_0_NUM_BLOCK_0_2       ((uint32_t)0x00001000U)        /*!< Bit 2 */
#define  USB_COUNT6_RX_0_NUM_BLOCK_0_3       ((uint32_t)0x00002000U)        /*!< Bit 3 */
#define  USB_COUNT6_RX_0_NUM_BLOCK_0_4       ((uint32_t)0x00004000U)        /*!< Bit 4 */

#define  USB_COUNT6_RX_0_BLSIZE_0            ((uint32_t)0x00008000U)        /*!< BLock SIZE (low) */

/****************  Bit definition for USB_COUNT6_RX_1 register  ***************/
#define  USB_COUNT6_RX_1_COUNT6_RX_1         ((uint32_t)0x03FF0000U)        /*!< Reception Byte Count (high) */

#define  USB_COUNT6_RX_1_NUM_BLOCK_1         ((uint32_t)0x7C000000U)        /*!< NUM_BLOCK_1[4:0] bits (Number of blocks) (high) */
#define  USB_COUNT6_RX_1_NUM_BLOCK_1_0       ((uint32_t)0x04000000U)        /*!< Bit 0 */
#define  USB_COUNT6_RX_1_NUM_BLOCK_1_1       ((uint32_t)0x08000000U)        /*!< Bit 1 */
#define  USB_COUNT6_RX_1_NUM_BLOCK_1_2       ((uint32_t)0x10000000U)        /*!< Bit 2 */
#define  USB_COUNT6_RX_1_NUM_BLOCK_1_3       ((uint32_t)0x20000000U)        /*!< Bit 3 */
#define  USB_COUNT6_RX_1_NUM_BLOCK_1_4       ((uint32_t)0x40000000U)        /*!< Bit 4 */

#define  USB_COUNT6_RX_1_BLSIZE_1            ((uint32_t)0x80000000U)        /*!< BLock SIZE (high) */

/***************  Bit definition for USB_COUNT7_RX_0 register  ****************/
#define  USB_COUNT7_RX_0_COUNT7_RX_0         ((uint32_t)0x000003FFU)        /*!< Reception Byte Count (low) */

#define  USB_COUNT7_RX_0_NUM_BLOCK_0         ((uint32_t)0x00007C00U)        /*!< NUM_BLOCK_0[4:0] bits (Number of blocks) (low) */
#define  USB_COUNT7_RX_0_NUM_BLOCK_0_0       ((uint32_t)0x00000400U)        /*!< Bit 0 */
#define  USB_COUNT7_RX_0_NUM_BLOCK_0_1       ((uint32_t)0x00000800U)        /*!< Bit 1 */
#define  USB_COUNT7_RX_0_NUM_BLOCK_0_2       ((uint32_t)0x00001000U)        /*!< Bit 2 */
#define  USB_COUNT7_RX_0_NUM_BLOCK_0_3       ((uint32_t)0x00002000U)        /*!< Bit 3 */
#define  USB_COUNT7_RX_0_NUM_BLOCK_0_4       ((uint32_t)0x00004000U)        /*!< Bit 4 */

#define  USB_COUNT7_RX_0_BLSIZE_0            ((uint32_t)0x00008000U)        /*!< BLock SIZE (low) */

/***************  Bit definition for USB_COUNT7_RX_1 register  ****************/
#define  USB_COUNT7_RX_1_COUNT7_RX_1         ((uint32_t)0x03FF0000U)        /*!< Reception Byte Count (high) */

#define  USB_COUNT7_RX_1_NUM_BLOCK_1         ((uint32_t)0x7C000000U)        /*!< NUM_BLOCK_1[4:0] bits (Number of blocks) (high) */
#define  USB_COUNT7_RX_1_NUM_BLOCK_1_0       ((uint32_t)0x04000000U)        /*!< Bit 0 */
#define  USB_COUNT7_RX_1_NUM_BLOCK_1_1       ((uint32_t)0x08000000U)        /*!< Bit 1 */
#define  USB_COUNT7_RX_1_NUM_BLOCK_1_2       ((uint32_t)0x10000000U)        /*!< Bit 2 */
#define  USB_COUNT7_RX_1_NUM_BLOCK_1_3       ((uint32_t)0x20000000U)        /*!< Bit 3 */
#define  USB_COUNT7_RX_1_NUM_BLOCK_1_4       ((uint32_t)0x40000000U)        /*!< Bit 4 */

#define  USB_COUNT7_RX_1_BLSIZE_1            ((uint32_t)0x80000000U)        /*!< BLock SIZE (high) */

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

/******************************************************************************/
/*                                                                            */
/*                        SystemTick (SysTick)                                */
/*                                                                            */
/******************************************************************************/

/*****************  Bit definition for SysTick_CTRL register  *****************/
#define  SysTick_CTRL_ENABLE                 ((uint32_t)0x00000001U)        /*!< Counter enable */
#define  SysTick_CTRL_TICKINT                ((uint32_t)0x00000002U)        /*!< Counting down to 0 pends the SysTick handler */
#define  SysTick_CTRL_CLKSOURCE              ((uint32_t)0x00000004U)        /*!< Clock source */
#define  SysTick_CTRL_COUNTFLAG              ((uint32_t)0x00010000U)        /*!< Count Flag */

/*****************  Bit definition for SysTick_LOAD register  *****************/
#define  SysTick_LOAD_RELOAD                 ((uint32_t)0x00FFFFFFU)        /*!< Value to load into the SysTick Current Value Register when the counter reaches 0 */

/*****************  Bit definition for SysTick_VAL register  ******************/
#define  SysTick_VAL_CURRENT                 ((uint32_t)0x00FFFFFFU)        /*!< Current value at the time the register is accessed */

/*****************  Bit definition for SysTick_CALIB register  ****************/
#define  SysTick_CALIB_TENMS                 ((uint32_t)0x00FFFFFFU)        /*!< Reload value to use for 10ms timing */
#define  SysTick_CALIB_SKEW                  ((uint32_t)0x40000000U)        /*!< Calibration value is not exactly 10 ms */
#define  SysTick_CALIB_NOREF                 ((uint32_t)0x80000000U)        /*!< The reference clock is not provided */

/******************************************************************************/
/*                                                                            */
/*               Nested Vectored Interrupt Controller (NVIC)                  */
/*                                                                            */
/******************************************************************************/

/******************  Bit definition for NVIC_ISER register  *******************/
#define  NVIC_ISER_SETENA                    ((uint32_t)0xFFFFFFFFU)        /*!< Interrupt set enable bits */
#define  NVIC_ISER_SETENA_0                  ((uint32_t)0x00000001U)        /*!< bit 0 */
#define  NVIC_ISER_SETENA_1                  ((uint32_t)0x00000002U)        /*!< bit 1 */
#define  NVIC_ISER_SETENA_2                  ((uint32_t)0x00000004U)        /*!< bit 2 */
#define  NVIC_ISER_SETENA_3                  ((uint32_t)0x00000008U)        /*!< bit 3 */
#define  NVIC_ISER_SETENA_4                  ((uint32_t)0x00000010U)        /*!< bit 4 */
#define  NVIC_ISER_SETENA_5                  ((uint32_t)0x00000020U)        /*!< bit 5 */
#define  NVIC_ISER_SETENA_6                  ((uint32_t)0x00000040U)        /*!< bit 6 */
#define  NVIC_ISER_SETENA_7                  ((uint32_t)0x00000080U)        /*!< bit 7 */
#define  NVIC_ISER_SETENA_8                  ((uint32_t)0x00000100U)        /*!< bit 8 */
#define  NVIC_ISER_SETENA_9                  ((uint32_t)0x00000200U)        /*!< bit 9 */
#define  NVIC_ISER_SETENA_10                 ((uint32_t)0x00000400U)        /*!< bit 10 */
#define  NVIC_ISER_SETENA_11                 ((uint32_t)0x00000800U)        /*!< bit 11 */
#define  NVIC_ISER_SETENA_12                 ((uint32_t)0x00001000U)        /*!< bit 12 */
#define  NVIC_ISER_SETENA_13                 ((uint32_t)0x00002000U)        /*!< bit 13 */
#define  NVIC_ISER_SETENA_14                 ((uint32_t)0x00004000U)        /*!< bit 14 */
#define  NVIC_ISER_SETENA_15                 ((uint32_t)0x00008000U)        /*!< bit 15 */
#define  NVIC_ISER_SETENA_16                 ((uint32_t)0x00010000U)        /*!< bit 16 */
#define  NVIC_ISER_SETENA_17                 ((uint32_t)0x00020000U)        /*!< bit 17 */
#define  NVIC_ISER_SETENA_18                 ((uint32_t)0x00040000U)        /*!< bit 18 */
#define  NVIC_ISER_SETENA_19                 ((uint32_t)0x00080000U)        /*!< bit 19 */
#define  NVIC_ISER_SETENA_20                 ((uint32_t)0x00100000U)        /*!< bit 20 */
#define  NVIC_ISER_SETENA_21                 ((uint32_t)0x00200000U)        /*!< bit 21 */
#define  NVIC_ISER_SETENA_22                 ((uint32_t)0x00400000U)        /*!< bit 22 */
#define  NVIC_ISER_SETENA_23                 ((uint32_t)0x00800000U)        /*!< bit 23 */
#define  NVIC_ISER_SETENA_24                 ((uint32_t)0x01000000U)        /*!< bit 24 */
#define  NVIC_ISER_SETENA_25                 ((uint32_t)0x02000000U)        /*!< bit 25 */
#define  NVIC_ISER_SETENA_26                 ((uint32_t)0x04000000U)        /*!< bit 26 */
#define  NVIC_ISER_SETENA_27                 ((uint32_t)0x08000000U)        /*!< bit 27 */
#define  NVIC_ISER_SETENA_28                 ((uint32_t)0x10000000U)        /*!< bit 28 */
#define  NVIC_ISER_SETENA_29                 ((uint32_t)0x20000000U)        /*!< bit 29 */
#define  NVIC_ISER_SETENA_30                 ((uint32_t)0x40000000U)        /*!< bit 30 */
#define  NVIC_ISER_SETENA_31                 ((uint32_t)0x80000000U)        /*!< bit 31 */

/******************  Bit definition for NVIC_ICER register  *******************/
#define  NVIC_ICER_CLRENA                   ((uint32_t)0xFFFFFFFFU)        /*!< Interrupt clear-enable bits */
#define  NVIC_ICER_CLRENA_0                  ((uint32_t)0x00000001U)        /*!< bit 0 */
#define  NVIC_ICER_CLRENA_1                  ((uint32_t)0x00000002U)        /*!< bit 1 */
#define  NVIC_ICER_CLRENA_2                  ((uint32_t)0x00000004U)        /*!< bit 2 */
#define  NVIC_ICER_CLRENA_3                  ((uint32_t)0x00000008U)        /*!< bit 3 */
#define  NVIC_ICER_CLRENA_4                  ((uint32_t)0x00000010U)        /*!< bit 4 */
#define  NVIC_ICER_CLRENA_5                  ((uint32_t)0x00000020U)        /*!< bit 5 */
#define  NVIC_ICER_CLRENA_6                  ((uint32_t)0x00000040U)        /*!< bit 6 */
#define  NVIC_ICER_CLRENA_7                  ((uint32_t)0x00000080U)        /*!< bit 7 */
#define  NVIC_ICER_CLRENA_8                  ((uint32_t)0x00000100U)        /*!< bit 8 */
#define  NVIC_ICER_CLRENA_9                  ((uint32_t)0x00000200U)        /*!< bit 9 */
#define  NVIC_ICER_CLRENA_10                 ((uint32_t)0x00000400U)        /*!< bit 10 */
#define  NVIC_ICER_CLRENA_11                 ((uint32_t)0x00000800U)        /*!< bit 11 */
#define  NVIC_ICER_CLRENA_12                 ((uint32_t)0x00001000U)        /*!< bit 12 */
#define  NVIC_ICER_CLRENA_13                 ((uint32_t)0x00002000U)        /*!< bit 13 */
#define  NVIC_ICER_CLRENA_14                 ((uint32_t)0x00004000U)        /*!< bit 14 */
#define  NVIC_ICER_CLRENA_15                 ((uint32_t)0x00008000U)        /*!< bit 15 */
#define  NVIC_ICER_CLRENA_16                 ((uint32_t)0x00010000U)        /*!< bit 16 */
#define  NVIC_ICER_CLRENA_17                 ((uint32_t)0x00020000U)        /*!< bit 17 */
#define  NVIC_ICER_CLRENA_18                 ((uint32_t)0x00040000U)        /*!< bit 18 */
#define  NVIC_ICER_CLRENA_19                 ((uint32_t)0x00080000U)        /*!< bit 19 */
#define  NVIC_ICER_CLRENA_20                 ((uint32_t)0x00100000U)        /*!< bit 20 */
#define  NVIC_ICER_CLRENA_21                 ((uint32_t)0x00200000U)        /*!< bit 21 */
#define  NVIC_ICER_CLRENA_22                 ((uint32_t)0x00400000U)        /*!< bit 22 */
#define  NVIC_ICER_CLRENA_23                 ((uint32_t)0x00800000U)        /*!< bit 23 */
#define  NVIC_ICER_CLRENA_24                 ((uint32_t)0x01000000U)        /*!< bit 24 */
#define  NVIC_ICER_CLRENA_25                 ((uint32_t)0x02000000U)        /*!< bit 25 */
#define  NVIC_ICER_CLRENA_26                 ((uint32_t)0x04000000U)        /*!< bit 26 */
#define  NVIC_ICER_CLRENA_27                 ((uint32_t)0x08000000U)        /*!< bit 27 */
#define  NVIC_ICER_CLRENA_28                 ((uint32_t)0x10000000U)        /*!< bit 28 */
#define  NVIC_ICER_CLRENA_29                 ((uint32_t)0x20000000U)        /*!< bit 29 */
#define  NVIC_ICER_CLRENA_30                 ((uint32_t)0x40000000U)        /*!< bit 30 */
#define  NVIC_ICER_CLRENA_31                 ((uint32_t)0x80000000U)        /*!< bit 31 */

/******************  Bit definition for NVIC_ISPR register  *******************/
#define  NVIC_ISPR_SETPEND                   ((uint32_t)0xFFFFFFFFU)        /*!< Interrupt set-pending bits */
#define  NVIC_ISPR_SETPEND_0                 ((uint32_t)0x00000001U)        /*!< bit 0 */
#define  NVIC_ISPR_SETPEND_1                 ((uint32_t)0x00000002U)        /*!< bit 1 */
#define  NVIC_ISPR_SETPEND_2                 ((uint32_t)0x00000004U)        /*!< bit 2 */
#define  NVIC_ISPR_SETPEND_3                 ((uint32_t)0x00000008U)        /*!< bit 3 */
#define  NVIC_ISPR_SETPEND_4                 ((uint32_t)0x00000010U)        /*!< bit 4 */
#define  NVIC_ISPR_SETPEND_5                 ((uint32_t)0x00000020U)        /*!< bit 5 */
#define  NVIC_ISPR_SETPEND_6                 ((uint32_t)0x00000040U)        /*!< bit 6 */
#define  NVIC_ISPR_SETPEND_7                 ((uint32_t)0x00000080U)        /*!< bit 7 */
#define  NVIC_ISPR_SETPEND_8                 ((uint32_t)0x00000100U)        /*!< bit 8 */
#define  NVIC_ISPR_SETPEND_9                 ((uint32_t)0x00000200U)        /*!< bit 9 */
#define  NVIC_ISPR_SETPEND_10                ((uint32_t)0x00000400U)        /*!< bit 10 */
#define  NVIC_ISPR_SETPEND_11                ((uint32_t)0x00000800U)        /*!< bit 11 */
#define  NVIC_ISPR_SETPEND_12                ((uint32_t)0x00001000U)        /*!< bit 12 */
#define  NVIC_ISPR_SETPEND_13                ((uint32_t)0x00002000U)        /*!< bit 13 */
#define  NVIC_ISPR_SETPEND_14                ((uint32_t)0x00004000U)        /*!< bit 14 */
#define  NVIC_ISPR_SETPEND_15                ((uint32_t)0x00008000U)        /*!< bit 15 */
#define  NVIC_ISPR_SETPEND_16                ((uint32_t)0x00010000U)        /*!< bit 16 */
#define  NVIC_ISPR_SETPEND_17                ((uint32_t)0x00020000U)        /*!< bit 17 */
#define  NVIC_ISPR_SETPEND_18                ((uint32_t)0x00040000U)        /*!< bit 18 */
#define  NVIC_ISPR_SETPEND_19                ((uint32_t)0x00080000U)        /*!< bit 19 */
#define  NVIC_ISPR_SETPEND_20                ((uint32_t)0x00100000U)        /*!< bit 20 */
#define  NVIC_ISPR_SETPEND_21                ((uint32_t)0x00200000U)        /*!< bit 21 */
#define  NVIC_ISPR_SETPEND_22                ((uint32_t)0x00400000U)        /*!< bit 22 */
#define  NVIC_ISPR_SETPEND_23                ((uint32_t)0x00800000U)        /*!< bit 23 */
#define  NVIC_ISPR_SETPEND_24                ((uint32_t)0x01000000U)        /*!< bit 24 */
#define  NVIC_ISPR_SETPEND_25                ((uint32_t)0x02000000U)        /*!< bit 25 */
#define  NVIC_ISPR_SETPEND_26                ((uint32_t)0x04000000U)        /*!< bit 26 */
#define  NVIC_ISPR_SETPEND_27                ((uint32_t)0x08000000U)        /*!< bit 27 */
#define  NVIC_ISPR_SETPEND_28                ((uint32_t)0x10000000U)        /*!< bit 28 */
#define  NVIC_ISPR_SETPEND_29                ((uint32_t)0x20000000U)        /*!< bit 29 */
#define  NVIC_ISPR_SETPEND_30                ((uint32_t)0x40000000U)        /*!< bit 30 */
#define  NVIC_ISPR_SETPEND_31                ((uint32_t)0x80000000U)        /*!< bit 31 */

/******************  Bit definition for NVIC_ICPR register  *******************/
#define  NVIC_ICPR_CLRPEND                   ((uint32_t)0xFFFFFFFFU)        /*!< Interrupt clear-pending bits */
#define  NVIC_ICPR_CLRPEND_0                 ((uint32_t)0x00000001U)        /*!< bit 0 */
#define  NVIC_ICPR_CLRPEND_1                 ((uint32_t)0x00000002U)        /*!< bit 1 */
#define  NVIC_ICPR_CLRPEND_2                 ((uint32_t)0x00000004U)        /*!< bit 2 */
#define  NVIC_ICPR_CLRPEND_3                 ((uint32_t)0x00000008U)        /*!< bit 3 */
#define  NVIC_ICPR_CLRPEND_4                 ((uint32_t)0x00000010U)        /*!< bit 4 */
#define  NVIC_ICPR_CLRPEND_5                 ((uint32_t)0x00000020U)        /*!< bit 5 */
#define  NVIC_ICPR_CLRPEND_6                 ((uint32_t)0x00000040U)        /*!< bit 6 */
#define  NVIC_ICPR_CLRPEND_7                 ((uint32_t)0x00000080U)        /*!< bit 7 */
#define  NVIC_ICPR_CLRPEND_8                 ((uint32_t)0x00000100U)        /*!< bit 8 */
#define  NVIC_ICPR_CLRPEND_9                 ((uint32_t)0x00000200U)        /*!< bit 9 */
#define  NVIC_ICPR_CLRPEND_10                ((uint32_t)0x00000400U)        /*!< bit 10 */
#define  NVIC_ICPR_CLRPEND_11                ((uint32_t)0x00000800U)        /*!< bit 11 */
#define  NVIC_ICPR_CLRPEND_12                ((uint32_t)0x00001000U)        /*!< bit 12 */
#define  NVIC_ICPR_CLRPEND_13                ((uint32_t)0x00002000U)        /*!< bit 13 */
#define  NVIC_ICPR_CLRPEND_14                ((uint32_t)0x00004000U)        /*!< bit 14 */
#define  NVIC_ICPR_CLRPEND_15                ((uint32_t)0x00008000U)        /*!< bit 15 */
#define  NVIC_ICPR_CLRPEND_16                ((uint32_t)0x00010000U)        /*!< bit 16 */
#define  NVIC_ICPR_CLRPEND_17                ((uint32_t)0x00020000U)        /*!< bit 17 */
#define  NVIC_ICPR_CLRPEND_18                ((uint32_t)0x00040000U)        /*!< bit 18 */
#define  NVIC_ICPR_CLRPEND_19                ((uint32_t)0x00080000U)        /*!< bit 19 */
#define  NVIC_ICPR_CLRPEND_20                ((uint32_t)0x00100000U)        /*!< bit 20 */
#define  NVIC_ICPR_CLRPEND_21                ((uint32_t)0x00200000U)        /*!< bit 21 */
#define  NVIC_ICPR_CLRPEND_22                ((uint32_t)0x00400000U)        /*!< bit 22 */
#define  NVIC_ICPR_CLRPEND_23                ((uint32_t)0x00800000U)        /*!< bit 23 */
#define  NVIC_ICPR_CLRPEND_24                ((uint32_t)0x01000000U)        /*!< bit 24 */
#define  NVIC_ICPR_CLRPEND_25                ((uint32_t)0x02000000U)        /*!< bit 25 */
#define  NVIC_ICPR_CLRPEND_26                ((uint32_t)0x04000000U)        /*!< bit 26 */
#define  NVIC_ICPR_CLRPEND_27                ((uint32_t)0x08000000U)        /*!< bit 27 */
#define  NVIC_ICPR_CLRPEND_28                ((uint32_t)0x10000000U)        /*!< bit 28 */
#define  NVIC_ICPR_CLRPEND_29                ((uint32_t)0x20000000U)        /*!< bit 29 */
#define  NVIC_ICPR_CLRPEND_30                ((uint32_t)0x40000000U)        /*!< bit 30 */
#define  NVIC_ICPR_CLRPEND_31                ((uint32_t)0x80000000U)        /*!< bit 31 */

/******************  Bit definition for NVIC_IABR register  *******************/
#define  NVIC_IABR_ACTIVE                    ((uint32_t)0xFFFFFFFFU)        /*!< Interrupt active flags */
#define  NVIC_IABR_ACTIVE_0                  ((uint32_t)0x00000001U)        /*!< bit 0 */
#define  NVIC_IABR_ACTIVE_1                  ((uint32_t)0x00000002U)        /*!< bit 1 */
#define  NVIC_IABR_ACTIVE_2                  ((uint32_t)0x00000004U)        /*!< bit 2 */
#define  NVIC_IABR_ACTIVE_3                  ((uint32_t)0x00000008U)        /*!< bit 3 */
#define  NVIC_IABR_ACTIVE_4                  ((uint32_t)0x00000010U)        /*!< bit 4 */
#define  NVIC_IABR_ACTIVE_5                  ((uint32_t)0x00000020U)        /*!< bit 5 */
#define  NVIC_IABR_ACTIVE_6                  ((uint32_t)0x00000040U)        /*!< bit 6 */
#define  NVIC_IABR_ACTIVE_7                  ((uint32_t)0x00000080U)        /*!< bit 7 */
#define  NVIC_IABR_ACTIVE_8                  ((uint32_t)0x00000100U)        /*!< bit 8 */
#define  NVIC_IABR_ACTIVE_9                  ((uint32_t)0x00000200U)        /*!< bit 9 */
#define  NVIC_IABR_ACTIVE_10                 ((uint32_t)0x00000400U)        /*!< bit 10 */
#define  NVIC_IABR_ACTIVE_11                 ((uint32_t)0x00000800U)        /*!< bit 11 */
#define  NVIC_IABR_ACTIVE_12                 ((uint32_t)0x00001000U)        /*!< bit 12 */
#define  NVIC_IABR_ACTIVE_13                 ((uint32_t)0x00002000U)        /*!< bit 13 */
#define  NVIC_IABR_ACTIVE_14                 ((uint32_t)0x00004000U)        /*!< bit 14 */
#define  NVIC_IABR_ACTIVE_15                 ((uint32_t)0x00008000U)        /*!< bit 15 */
#define  NVIC_IABR_ACTIVE_16                 ((uint32_t)0x00010000U)        /*!< bit 16 */
#define  NVIC_IABR_ACTIVE_17                 ((uint32_t)0x00020000U)        /*!< bit 17 */
#define  NVIC_IABR_ACTIVE_18                 ((uint32_t)0x00040000U)        /*!< bit 18 */
#define  NVIC_IABR_ACTIVE_19                 ((uint32_t)0x00080000U)        /*!< bit 19 */
#define  NVIC_IABR_ACTIVE_20                 ((uint32_t)0x00100000U)        /*!< bit 20 */
#define  NVIC_IABR_ACTIVE_21                 ((uint32_t)0x00200000U)        /*!< bit 21 */
#define  NVIC_IABR_ACTIVE_22                 ((uint32_t)0x00400000U)        /*!< bit 22 */
#define  NVIC_IABR_ACTIVE_23                 ((uint32_t)0x00800000U)        /*!< bit 23 */
#define  NVIC_IABR_ACTIVE_24                 ((uint32_t)0x01000000U)        /*!< bit 24 */
#define  NVIC_IABR_ACTIVE_25                 ((uint32_t)0x02000000U)        /*!< bit 25 */
#define  NVIC_IABR_ACTIVE_26                 ((uint32_t)0x04000000U)        /*!< bit 26 */
#define  NVIC_IABR_ACTIVE_27                 ((uint32_t)0x08000000U)        /*!< bit 27 */
#define  NVIC_IABR_ACTIVE_28                 ((uint32_t)0x10000000U)        /*!< bit 28 */
#define  NVIC_IABR_ACTIVE_29                 ((uint32_t)0x20000000U)        /*!< bit 29 */
#define  NVIC_IABR_ACTIVE_30                 ((uint32_t)0x40000000U)        /*!< bit 30 */
#define  NVIC_IABR_ACTIVE_31                 ((uint32_t)0x80000000U)        /*!< bit 31 */

/******************  Bit definition for NVIC_PRI0 register  *******************/
#define  NVIC_IPR0_PRI_0                     ((uint32_t)0x000000FFU)        /*!< Priority of interrupt 0 */
#define  NVIC_IPR0_PRI_1                     ((uint32_t)0x0000FF00U)        /*!< Priority of interrupt 1 */
#define  NVIC_IPR0_PRI_2                     ((uint32_t)0x00FF0000U)        /*!< Priority of interrupt 2 */
#define  NVIC_IPR0_PRI_3                     ((uint32_t)0xFF000000U)        /*!< Priority of interrupt 3 */

/******************  Bit definition for NVIC_PRI1 register  *******************/
#define  NVIC_IPR1_PRI_4                     ((uint32_t)0x000000FFU)        /*!< Priority of interrupt 4 */
#define  NVIC_IPR1_PRI_5                     ((uint32_t)0x0000FF00U)        /*!< Priority of interrupt 5 */
#define  NVIC_IPR1_PRI_6                     ((uint32_t)0x00FF0000U)        /*!< Priority of interrupt 6 */
#define  NVIC_IPR1_PRI_7                     ((uint32_t)0xFF000000U)        /*!< Priority of interrupt 7 */

/******************  Bit definition for NVIC_PRI2 register  *******************/
#define  NVIC_IPR2_PRI_8                     ((uint32_t)0x000000FFU)        /*!< Priority of interrupt 8 */
#define  NVIC_IPR2_PRI_9                     ((uint32_t)0x0000FF00U)        /*!< Priority of interrupt 9 */
#define  NVIC_IPR2_PRI_10                    ((uint32_t)0x00FF0000U)        /*!< Priority of interrupt 10 */
#define  NVIC_IPR2_PRI_11                    ((uint32_t)0xFF000000U)        /*!< Priority of interrupt 11 */

/******************  Bit definition for NVIC_PRI3 register  *******************/
#define  NVIC_IPR3_PRI_12                    ((uint32_t)0x000000FFU)        /*!< Priority of interrupt 12 */
#define  NVIC_IPR3_PRI_13                    ((uint32_t)0x0000FF00U)        /*!< Priority of interrupt 13 */
#define  NVIC_IPR3_PRI_14                    ((uint32_t)0x00FF0000U)        /*!< Priority of interrupt 14 */
#define  NVIC_IPR3_PRI_15                    ((uint32_t)0xFF000000U)        /*!< Priority of interrupt 15 */

/******************  Bit definition for NVIC_PRI4 register  *******************/
#define  NVIC_IPR4_PRI_16                    ((uint32_t)0x000000FFU)        /*!< Priority of interrupt 16 */
#define  NVIC_IPR4_PRI_17                    ((uint32_t)0x0000FF00U)        /*!< Priority of interrupt 17 */
#define  NVIC_IPR4_PRI_18                    ((uint32_t)0x00FF0000U)        /*!< Priority of interrupt 18 */
#define  NVIC_IPR4_PRI_19                    ((uint32_t)0xFF000000U)        /*!< Priority of interrupt 19 */

/******************  Bit definition for NVIC_PRI5 register  *******************/
#define  NVIC_IPR5_PRI_20                    ((uint32_t)0x000000FFU)        /*!< Priority of interrupt 20 */
#define  NVIC_IPR5_PRI_21                    ((uint32_t)0x0000FF00U)        /*!< Priority of interrupt 21 */
#define  NVIC_IPR5_PRI_22                    ((uint32_t)0x00FF0000U)        /*!< Priority of interrupt 22 */
#define  NVIC_IPR5_PRI_23                    ((uint32_t)0xFF000000U)        /*!< Priority of interrupt 23 */

/******************  Bit definition for NVIC_PRI6 register  *******************/
#define  NVIC_IPR6_PRI_24                    ((uint32_t)0x000000FFU)        /*!< Priority of interrupt 24 */
#define  NVIC_IPR6_PRI_25                    ((uint32_t)0x0000FF00U)        /*!< Priority of interrupt 25 */
#define  NVIC_IPR6_PRI_26                    ((uint32_t)0x00FF0000U)        /*!< Priority of interrupt 26 */
#define  NVIC_IPR6_PRI_27                    ((uint32_t)0xFF000000U)        /*!< Priority of interrupt 27 */

/******************  Bit definition for NVIC_PRI7 register  *******************/
#define  NVIC_IPR7_PRI_28                    ((uint32_t)0x000000FFU)        /*!< Priority of interrupt 28 */
#define  NVIC_IPR7_PRI_29                    ((uint32_t)0x0000FF00U)        /*!< Priority of interrupt 29 */
#define  NVIC_IPR7_PRI_30                    ((uint32_t)0x00FF0000U)        /*!< Priority of interrupt 30 */
#define  NVIC_IPR7_PRI_31                    ((uint32_t)0xFF000000U)        /*!< Priority of interrupt 31 */

/******************  Bit definition for SCB_CPUID register  *******************/
#define  SCB_CPUID_REVISION                  ((uint32_t)0x0000000FU)        /*!< Implementation defined revision number */
#define  SCB_CPUID_PARTNO                    ((uint32_t)0x0000FFF0U)        /*!< Number of processor within family */
#define  SCB_CPUID_Constant                  ((uint32_t)0x000F0000U)        /*!< Reads as 0x0F */
#define  SCB_CPUID_VARIANT                   ((uint32_t)0x00F00000U)        /*!< Implementation defined variant number */
#define  SCB_CPUID_IMPLEMENTER               ((uint32_t)0xFF000000U)        /*!< Implementer code. ARM is 0x41 */

/*******************  Bit definition for SCB_ICSR register  *******************/
#define  SCB_ICSR_VECTACTIVE                 ((uint32_t)0x000001FFU)        /*!< Active ISR number field */
#define  SCB_ICSR_RETTOBASE                  ((uint32_t)0x00000800U)        /*!< All active exceptions minus the IPSR_current_exception yields the empty set */
#define  SCB_ICSR_VECTPENDING                ((uint32_t)0x003FF000U)        /*!< Pending ISR number field */
#define  SCB_ICSR_ISRPENDING                 ((uint32_t)0x00400000U)        /*!< Interrupt pending flag */
#define  SCB_ICSR_ISRPREEMPT                 ((uint32_t)0x00800000U)        /*!< It indicates that a pending interrupt becomes active in the next running cycle */
#define  SCB_ICSR_PENDSTCLR                  ((uint32_t)0x02000000U)        /*!< Clear pending SysTick bit */
#define  SCB_ICSR_PENDSTSET                  ((uint32_t)0x04000000U)        /*!< Set pending SysTick bit */
#define  SCB_ICSR_PENDSVCLR                  ((uint32_t)0x08000000U)        /*!< Clear pending pendSV bit */
#define  SCB_ICSR_PENDSVSET                  ((uint32_t)0x10000000U)        /*!< Set pending pendSV bit */
#define  SCB_ICSR_NMIPENDSET                 ((uint32_t)0x80000000U)        /*!< Set pending NMI bit */

/*******************  Bit definition for SCB_VTOR register  *******************/
#define  SCB_VTOR_TBLOFF                     ((uint32_t)0x1FFFFF80U)        /*!< Vector table base offset field */
#define  SCB_VTOR_TBLBASE                    ((uint32_t)0x20000000U)        /*!< Table base in code(0) or RAM(1) */

/*!<*****************  Bit definition for SCB_AIRCR register  *******************/
#define  SCB_AIRCR_VECTRESET                 ((uint32_t)0x00000001U)        /*!< System Reset bit */
#define  SCB_AIRCR_VECTCLRACTIVE             ((uint32_t)0x00000002U)        /*!< Clear active vector bit */
#define  SCB_AIRCR_SYSRESETREQ               ((uint32_t)0x00000004U)        /*!< Requests chip control logic to generate a reset */

#define  SCB_AIRCR_PRIGROUP                  ((uint32_t)0x00000700U)        /*!< PRIGROUP[2:0] bits (Priority group) */
#define  SCB_AIRCR_PRIGROUP_0                ((uint32_t)0x00000100U)        /*!< Bit 0 */
#define  SCB_AIRCR_PRIGROUP_1                ((uint32_t)0x00000200U)        /*!< Bit 1 */
#define  SCB_AIRCR_PRIGROUP_2                ((uint32_t)0x00000400U)        /*!< Bit 2  */

/* prority group configuration */
#define  SCB_AIRCR_PRIGROUP0                 ((uint32_t)0x00000000U)        /*!< Priority group=0 (7 bits of pre-emption priority, 1 bit of subpriority) */
#define  SCB_AIRCR_PRIGROUP1                 ((uint32_t)0x00000100U)        /*!< Priority group=1 (6 bits of pre-emption priority, 2 bits of subpriority) */
#define  SCB_AIRCR_PRIGROUP2                 ((uint32_t)0x00000200U)        /*!< Priority group=2 (5 bits of pre-emption priority, 3 bits of subpriority) */
#define  SCB_AIRCR_PRIGROUP3                 ((uint32_t)0x00000300U)        /*!< Priority group=3 (4 bits of pre-emption priority, 4 bits of subpriority) */
#define  SCB_AIRCR_PRIGROUP4                 ((uint32_t)0x00000400U)        /*!< Priority group=4 (3 bits of pre-emption priority, 5 bits of subpriority) */
#define  SCB_AIRCR_PRIGROUP5                 ((uint32_t)0x00000500U)        /*!< Priority group=5 (2 bits of pre-emption priority, 6 bits of subpriority) */
#define  SCB_AIRCR_PRIGROUP6                 ((uint32_t)0x00000600U)        /*!< Priority group=6 (1 bit of pre-emption priority, 7 bits of subpriority) */
#define  SCB_AIRCR_PRIGROUP7                 ((uint32_t)0x00000700U)        /*!< Priority group=7 (no pre-emption priority, 8 bits of subpriority) */

#define  SCB_AIRCR_ENDIANESS                 ((uint32_t)0x00008000U)        /*!< Data endianness bit */
#define  SCB_AIRCR_VECTKEY                   ((uint32_t)0xFFFF0000U)        /*!< Register key (VECTKEY) - Reads as 0xFA05 (VECTKEYSTAT) */

/*******************  Bit definition for SCB_SCR register  ********************/
#define  SCB_SCR_SLEEPONEXIT                 ((uint32_t)0x00000002U)               /*!< Sleep on exit bit */
#define  SCB_SCR_SLEEPDEEP                   ((uint32_t)0x00000004U)               /*!< Sleep deep bit */
#define  SCB_SCR_SEVONPEND                   ((uint32_t)0x00000010U)               /*!< Wake up from WFE */

/********************  Bit definition for SCB_CCR register  *******************/
#define  SCB_CCR_NONBASETHRDENA              ((uint32_t)0x00000001U)            /*!< Thread mode can be entered from any level in Handler mode by controlled return value */
#define  SCB_CCR_USERSETMPEND                ((uint32_t)0x00000002U)            /*!< Enables user code to write the Software Trigger Interrupt register to trigger (pend) a Main exception */
#define  SCB_CCR_UNALIGN_TRP                 ((uint32_t)0x00000008U)            /*!< Trap for unaligned access */
#define  SCB_CCR_DIV_0_TRP                   ((uint32_t)0x00000010U)            /*!< Trap on Divide by 0 */
#define  SCB_CCR_BFHFNMIGN                   ((uint32_t)0x00000100U)            /*!< Handlers running at priority -1 and -2 */
#define  SCB_CCR_STKALIGN                    ((uint32_t)0x00000200U)            /*!< On exception entry, the SP used prior to the exception is adjusted to be 8-byte aligned */

/*******************  Bit definition for SCB_SHPR register ********************/
#define  SCB_SHPR_PRI_N                      ((uint32_t)0x000000FFU)        /*!< Priority of system handler 4,8, and 12. Mem Manage, reserved and Debug Monitor */
#define  SCB_SHPR_PRI_N1                     ((uint32_t)0x0000FF00U)        /*!< Priority of system handler 5,9, and 13. Bus Fault, reserved and reserved */
#define  SCB_SHPR_PRI_N2                     ((uint32_t)0x00FF0000U)        /*!< Priority of system handler 6,10, and 14. Usage Fault, reserved and PendSV */
#define  SCB_SHPR_PRI_N3                     ((uint32_t)0xFF000000U)        /*!< Priority of system handler 7,11, and 15. Reserved, SVCall and SysTick */

/******************  Bit definition for SCB_SHCSR register  *******************/
#define  SCB_SHCSR_MEMFAULTACT               ((uint32_t)0x00000001U)        /*!< MemManage is active */
#define  SCB_SHCSR_BUSFAULTACT               ((uint32_t)0x00000002U)        /*!< BusFault is active */
#define  SCB_SHCSR_USGFAULTACT               ((uint32_t)0x00000008U)        /*!< UsageFault is active */
#define  SCB_SHCSR_SVCALLACT                 ((uint32_t)0x00000080U)        /*!< SVCall is active */
#define  SCB_SHCSR_MONITORACT                ((uint32_t)0x00000100U)        /*!< Monitor is active */
#define  SCB_SHCSR_PENDSVACT                 ((uint32_t)0x00000400U)        /*!< PendSV is active */
#define  SCB_SHCSR_SYSTICKACT                ((uint32_t)0x00000800U)        /*!< SysTick is active */
#define  SCB_SHCSR_USGFAULTPENDED            ((uint32_t)0x00001000U)        /*!< Usage Fault is pended */
#define  SCB_SHCSR_MEMFAULTPENDED            ((uint32_t)0x00002000U)        /*!< MemManage is pended */
#define  SCB_SHCSR_BUSFAULTPENDED            ((uint32_t)0x00004000U)        /*!< Bus Fault is pended */
#define  SCB_SHCSR_SVCALLPENDED              ((uint32_t)0x00008000U)        /*!< SVCall is pended */
#define  SCB_SHCSR_MEMFAULTENA               ((uint32_t)0x00010000U)        /*!< MemManage enable */
#define  SCB_SHCSR_BUSFAULTENA               ((uint32_t)0x00020000U)        /*!< Bus Fault enable */
#define  SCB_SHCSR_USGFAULTENA               ((uint32_t)0x00040000U)        /*!< UsageFault enable */

/*******************  Bit definition for SCB_CFSR register  *******************/
/*!< MFSR */
#define  SCB_CFSR_IACCVIOL                   ((uint32_t)0x00000001U)        /*!< Instruction access violation */
#define  SCB_CFSR_DACCVIOL                   ((uint32_t)0x00000002U)        /*!< Data access violation */
#define  SCB_CFSR_MUNSTKERR                  ((uint32_t)0x00000008U)        /*!< Unstacking error */
#define  SCB_CFSR_MSTKERR                    ((uint32_t)0x00000010U)        /*!< Stacking error */
#define  SCB_CFSR_MMARVALID                  ((uint32_t)0x00000080U)        /*!< Memory Manage Address Register address valid flag */
/*!< BFSR */
#define  SCB_CFSR_IBUSERR                    ((uint32_t)0x00000100U)        /*!< Instruction bus error flag */
#define  SCB_CFSR_PRECISERR                  ((uint32_t)0x00000200U)        /*!< Precise data bus error */
#define  SCB_CFSR_IMPRECISERR                ((uint32_t)0x00000400U)        /*!< Imprecise data bus error */
#define  SCB_CFSR_UNSTKERR                   ((uint32_t)0x00000800U)        /*!< Unstacking error */
#define  SCB_CFSR_STKERR                     ((uint32_t)0x00001000U)        /*!< Stacking error */
#define  SCB_CFSR_BFARVALID                  ((uint32_t)0x00008000U)        /*!< Bus Fault Address Register address valid flag */
/*!< UFSR */
#define  SCB_CFSR_UNDEFINSTR                 ((uint32_t)0x00010000U)        /*!< The processor attempt to excecute an undefined instruction */
#define  SCB_CFSR_INVSTATE                   ((uint32_t)0x00020000U)        /*!< Invalid combination of EPSR and instruction */
#define  SCB_CFSR_INVPC                      ((uint32_t)0x00040000U)        /*!< Attempt to load EXC_RETURN into pc illegally */
#define  SCB_CFSR_NOCP                       ((uint32_t)0x00080000U)        /*!< Attempt to use a coprocessor instruction */
#define  SCB_CFSR_UNALIGNED                  ((uint32_t)0x01000000U)        /*!< Fault occurs when there is an attempt to make an unaligned memory access */
#define  SCB_CFSR_DIVBYZERO                  ((uint32_t)0x02000000U)        /*!< Fault occurs when SDIV or DIV instruction is used with a divisor of 0 */

/*******************  Bit definition for SCB_HFSR register  *******************/
#define  SCB_HFSR_VECTTBL                    ((uint32_t)0x00000002U)        /*!< Fault occures because of vector table read on exception processing */
#define  SCB_HFSR_FORCED                     ((uint32_t)0x40000000U)        /*!< Hard Fault activated when a configurable Fault was received and cannot activate */
#define  SCB_HFSR_DEBUGEVT                   ((uint32_t)0x80000000U)        /*!< Fault related to debug */

/*******************  Bit definition for SCB_DFSR register  *******************/
#define  SCB_DFSR_HALTED                     ((uint32_t)0x00000001U)               /*!< Halt request flag */
#define  SCB_DFSR_BKPT                       ((uint32_t)0x00000002U)               /*!< BKPT flag */
#define  SCB_DFSR_DWTTRAP                    ((uint32_t)0x00000004U)               /*!< Data Watchpoint and Trace (DWT) flag */
#define  SCB_DFSR_VCATCH                     ((uint32_t)0x00000008U)               /*!< Vector catch flag */
#define  SCB_DFSR_EXTERNAL                   ((uint32_t)0x00000010U)               /*!< External debug request flag */

/*******************  Bit definition for SCB_MMFAR register  ******************/
#define  SCB_MMFAR_ADDRESS                   ((uint32_t)0xFFFFFFFFU)        /*!< Mem Manage fault address field */

/*******************  Bit definition for SCB_BFAR register  *******************/
#define  SCB_BFAR_ADDRESS                    ((uint32_t)0xFFFFFFFFU)        /*!< Bus fault address field */

/*******************  Bit definition for SCB_afsr register  *******************/
#define  SCB_AFSR_IMPDEF                     ((uint32_t)0xFFFFFFFFU)        /*!< Implementation defined */
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

#define IS_ADC_COMMON_INSTANCE(INSTANCE) ((INSTANCE) == ADC1_COMMON)

/******************************** COMP Instances ******************************/
#define IS_COMP_ALL_INSTANCE(INSTANCE) (((INSTANCE) == COMP1) || \
                                        ((INSTANCE) == COMP2))

/****************************** CRC Instances *********************************/
#define IS_CRC_ALL_INSTANCE(INSTANCE) ((INSTANCE) == CRC)

/****************************** DAC Instances *********************************/
#define IS_DAC_ALL_INSTANCE(INSTANCE) ((INSTANCE) == DAC)

/****************************** DMA Instances *********************************/
#define IS_DMA_ALL_INSTANCE(INSTANCE) (((INSTANCE) == DMA1_Channel1) || \
                                       ((INSTANCE) == DMA1_Channel2) || \
                                       ((INSTANCE) == DMA1_Channel3) || \
                                       ((INSTANCE) == DMA1_Channel4) || \
                                       ((INSTANCE) == DMA1_Channel5) || \
                                       ((INSTANCE) == DMA1_Channel6) || \
                                       ((INSTANCE) == DMA1_Channel7) || \
                                       ((INSTANCE) == DMA2_Channel1) || \
                                       ((INSTANCE) == DMA2_Channel2) || \
                                       ((INSTANCE) == DMA2_Channel3) || \
                                       ((INSTANCE) == DMA2_Channel4) || \
                                       ((INSTANCE) == DMA2_Channel5))

/******************************* GPIO Instances *******************************/
#define IS_GPIO_ALL_INSTANCE(INSTANCE) (((INSTANCE) == GPIOA) || \
                                        ((INSTANCE) == GPIOB) || \
                                        ((INSTANCE) == GPIOC) || \
                                        ((INSTANCE) == GPIOD) || \
                                        ((INSTANCE) == GPIOE) || \
                                        ((INSTANCE) == GPIOF) || \
                                        ((INSTANCE) == GPIOG) || \
                                        ((INSTANCE) == GPIOH))

/**************************** GPIO Alternate Function Instances ***************/
#define IS_GPIO_AF_INSTANCE(INSTANCE) IS_GPIO_ALL_INSTANCE(INSTANCE)

/**************************** GPIO Lock Instances *****************************/
/* On L1, all GPIO Bank support the Lock mechanism */
#define IS_GPIO_LOCK_INSTANCE(INSTANCE) IS_GPIO_ALL_INSTANCE(INSTANCE)

/******************************** I2C Instances *******************************/
#define IS_I2C_ALL_INSTANCE(INSTANCE) (((INSTANCE) == I2C1) || \
                                       ((INSTANCE) == I2C2))

#define IS_I2S_ALL_INSTANCE(INSTANCE) (((INSTANCE) == SPI2) || \
                                       ((INSTANCE) == SPI3))
/****************************** IWDG Instances ********************************/
#define IS_IWDG_ALL_INSTANCE(INSTANCE)  ((INSTANCE) == IWDG)

/****************************** OPAMP Instances *******************************/
#define IS_OPAMP_ALL_INSTANCE(INSTANCE) (((INSTANCE) == OPAMP1) || \
                                         ((INSTANCE) == OPAMP2))
                                         
/****************************** RTC Instances *********************************/
#define IS_RTC_ALL_INSTANCE(INSTANCE)  ((INSTANCE) == RTC)

/******************************** SPI Instances *******************************/
#define IS_SPI_ALL_INSTANCE(INSTANCE) (((INSTANCE) == SPI1) || \
                                       ((INSTANCE) == SPI2) || \
                                       ((INSTANCE) == SPI3))

/****************************** TIM Instances *********************************/ 
#define IS_TIM_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)    || \
                                   ((INSTANCE) == TIM3)    || \
                                   ((INSTANCE) == TIM4)    || \
                                   ((INSTANCE) == TIM5)    || \
                                   ((INSTANCE) == TIM6)    || \
                                   ((INSTANCE) == TIM7)    || \
                                   ((INSTANCE) == TIM9)    || \
                                   ((INSTANCE) == TIM10)   || \
                                   ((INSTANCE) == TIM11))

#define IS_TIM_CC1_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)  || \
                                       ((INSTANCE) == TIM3)  || \
                                       ((INSTANCE) == TIM4)  || \
                                       ((INSTANCE) == TIM5)  || \
                                       ((INSTANCE) == TIM9)  || \
                                       ((INSTANCE) == TIM10) || \
                                       ((INSTANCE) == TIM11))

#define IS_TIM_CC2_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)  || \
                                       ((INSTANCE) == TIM3)  || \
                                       ((INSTANCE) == TIM4)  || \
                                       ((INSTANCE) == TIM5)  || \
                                       ((INSTANCE) == TIM9))

#define IS_TIM_CC3_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)  || \
                                       ((INSTANCE) == TIM3)  || \
                                       ((INSTANCE) == TIM4)  || \
                                       ((INSTANCE) == TIM5))

#define IS_TIM_CC4_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)  || \
                                       ((INSTANCE) == TIM3)  || \
                                       ((INSTANCE) == TIM4)  || \
                                       ((INSTANCE) == TIM5))

#define IS_TIM_CLOCKSOURCE_ETRMODE1_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)  || \
                                                        ((INSTANCE) == TIM3)  || \
                                                        ((INSTANCE) == TIM4)  || \
                                                        ((INSTANCE) == TIM5)  || \
                                                        ((INSTANCE) == TIM9))

#define IS_TIM_CLOCKSOURCE_ETRMODE2_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)  || \
                                                        ((INSTANCE) == TIM3)  || \
                                                        ((INSTANCE) == TIM4)  || \
                                                        ((INSTANCE) == TIM5)  || \
                                                        ((INSTANCE) == TIM9)  || \
                                                        ((INSTANCE) == TIM10) || \
                                                        ((INSTANCE) == TIM11))

#define IS_TIM_CLOCKSOURCE_TIX_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)  || \
                                                   ((INSTANCE) == TIM3)  || \
                                                   ((INSTANCE) == TIM4)  || \
                                                   ((INSTANCE) == TIM5)  || \
                                                   ((INSTANCE) == TIM9))

#define IS_TIM_CLOCKSOURCE_ITRX_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)  || \
                                                    ((INSTANCE) == TIM3)  || \
                                                    ((INSTANCE) == TIM4)  || \
                                                    ((INSTANCE) == TIM5)  || \
                                                    ((INSTANCE) == TIM9))

#define IS_TIM_OCXREF_CLEAR_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)  || \
                                                ((INSTANCE) == TIM3)  || \
                                                ((INSTANCE) == TIM4))

#define IS_TIM_XOR_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)  || \
                                       ((INSTANCE) == TIM3)  || \
                                       ((INSTANCE) == TIM4)  || \
                                       ((INSTANCE) == TIM5))

#define IS_TIM_MASTER_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)  || \
                                          ((INSTANCE) == TIM3)  || \
                                          ((INSTANCE) == TIM4)  || \
                                          ((INSTANCE) == TIM5)  || \
                                          ((INSTANCE) == TIM6)  || \
                                          ((INSTANCE) == TIM7)  || \
                                          ((INSTANCE) == TIM9))

#define IS_TIM_SLAVE_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)  || \
                                         ((INSTANCE) == TIM3)  || \
                                         ((INSTANCE) == TIM4)  || \
                                         ((INSTANCE) == TIM5)  || \
                                         ((INSTANCE) == TIM9))

#define IS_TIM_32B_COUNTER_INSTANCE(INSTANCE) ((INSTANCE) == TIM5)

#define IS_TIM_DMABURST_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)  || \
                                            ((INSTANCE) == TIM3)  || \
                                            ((INSTANCE) == TIM4)  || \
                                            ((INSTANCE) == TIM5))

#define IS_TIM_CCX_INSTANCE(INSTANCE, CHANNEL) \
    ((((INSTANCE) == TIM2) &&                   \
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
    (((INSTANCE) == TIM4) &&                   \
     (((CHANNEL) == TIM_CHANNEL_1) ||          \
      ((CHANNEL) == TIM_CHANNEL_2) ||          \
      ((CHANNEL) == TIM_CHANNEL_3) ||          \
      ((CHANNEL) == TIM_CHANNEL_4)))           \
    ||                                         \
    (((INSTANCE) == TIM5) &&                   \
     (((CHANNEL) == TIM_CHANNEL_1) ||          \
      ((CHANNEL) == TIM_CHANNEL_2) ||          \
      ((CHANNEL) == TIM_CHANNEL_3) ||          \
      ((CHANNEL) == TIM_CHANNEL_4)))           \
    ||                                         \
    (((INSTANCE) == TIM9) &&                  \
     (((CHANNEL) == TIM_CHANNEL_1) ||          \
      ((CHANNEL) == TIM_CHANNEL_2)))           \
    ||                                         \
    (((INSTANCE) == TIM10) &&                  \
     (((CHANNEL) == TIM_CHANNEL_1)))           \
    ||                                         \
    (((INSTANCE) == TIM11) &&                  \
     (((CHANNEL) == TIM_CHANNEL_1))))

#define IS_TIM_CLOCK_DIVISION_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)  || \
                                                  ((INSTANCE) == TIM3)  || \
                                                  ((INSTANCE) == TIM4)  || \
                                                  ((INSTANCE) == TIM5)  || \
                                                  ((INSTANCE) == TIM9)  || \
                                                  ((INSTANCE) == TIM10) || \
                                                  ((INSTANCE) == TIM11))

#define IS_TIM_DMA_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)    || \
                                       ((INSTANCE) == TIM3)    || \
                                       ((INSTANCE) == TIM4)    || \
                                       ((INSTANCE) == TIM5)    || \
                                       ((INSTANCE) == TIM6)    || \
                                       ((INSTANCE) == TIM7))
    
#define IS_TIM_DMA_CC_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)  || \
                                          ((INSTANCE) == TIM3)  || \
                                          ((INSTANCE) == TIM4)  || \
                                          ((INSTANCE) == TIM5))

#define IS_TIM_COUNTER_MODE_SELECT_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)    || \
                                                       ((INSTANCE) == TIM3)    || \
                                                       ((INSTANCE) == TIM4)    || \
                                                       ((INSTANCE) == TIM5)    || \
                                                       ((INSTANCE) == TIM9))

#define IS_TIM_ENCODER_INTERFACE_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)  || \
                                                     ((INSTANCE) == TIM3)  || \
                                                     ((INSTANCE) == TIM4)  || \
                                                     ((INSTANCE) == TIM5))

#define IS_TIM_REMAP_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)    || \
                                         ((INSTANCE) == TIM3)    || \
                                         ((INSTANCE) == TIM9)    || \
                                         ((INSTANCE) == TIM10)   || \
                                         ((INSTANCE) == TIM11))

/******************** USART Instances : Synchronous mode **********************/                                           
#define IS_USART_INSTANCE(INSTANCE) (((INSTANCE) == USART1) || \
                                     ((INSTANCE) == USART2) || \
                                     ((INSTANCE) == USART3))

/******************** UART Instances : Asynchronous mode **********************/
#define IS_UART_INSTANCE(INSTANCE) (((INSTANCE) == USART1) || \
                                    ((INSTANCE) == USART2) || \
                                    ((INSTANCE) == USART3) || \
                                    ((INSTANCE) == UART4)  || \
                                    ((INSTANCE) == UART5))

/******************** UART Instances : Half-Duplex mode **********************/
#define IS_UART_HALFDUPLEX_INSTANCE(INSTANCE)   (((INSTANCE) == USART1) || \
                                                 ((INSTANCE) == USART2) || \
                                                 ((INSTANCE) == USART3) || \
                                                 ((INSTANCE) == UART4)  || \
                                                 ((INSTANCE) == UART5))                                       

/******************** UART Instances : LIN mode **********************/
#define IS_UART_LIN_INSTANCE(INSTANCE)   (((INSTANCE) == USART1) || \
                                          ((INSTANCE) == USART2) || \
                                          ((INSTANCE) == USART3) || \
                                          ((INSTANCE) == UART4)  || \
                                          ((INSTANCE) == UART5)) 

/****************** UART Instances : Hardware Flow control ********************/                                    
#define IS_UART_HWFLOW_INSTANCE(INSTANCE) (((INSTANCE) == USART1) || \
                                           ((INSTANCE) == USART2) || \
                                           ((INSTANCE) == USART3))

/********************* UART Instances : Smard card mode ***********************/
#define IS_SMARTCARD_INSTANCE(INSTANCE) (((INSTANCE) == USART1) || \
                                         ((INSTANCE) == USART2) || \
                                         ((INSTANCE) == USART3))

/*********************** UART Instances : IRDA mode ***************************/
#define IS_IRDA_INSTANCE(INSTANCE) (((INSTANCE) == USART1) || \
                                    ((INSTANCE) == USART2) || \
                                    ((INSTANCE) == USART3) || \
                                    ((INSTANCE) == UART4)  || \
                                    ((INSTANCE) == UART5))

/***************** UART Instances : Multi-Processor mode **********************/
#define IS_UART_MULTIPROCESSOR_INSTANCE(INSTANCE)   (((INSTANCE) == USART1) || \
                                                     ((INSTANCE) == USART2) || \
                                                     ((INSTANCE) == USART3) || \
                                                     ((INSTANCE) == UART4)  || \
                                                     ((INSTANCE) == UART5)) 

/****************************** WWDG Instances ********************************/
#define IS_WWDG_ALL_INSTANCE(INSTANCE)  ((INSTANCE) == WWDG)


/****************************** LCD Instances ********************************/
#define IS_LCD_ALL_INSTANCE(INSTANCE) ((INSTANCE) == LCD)

/****************************** USB Instances ********************************/
#define IS_USB_ALL_INSTANCE(INSTANCE) ((INSTANCE) == USB)

/**
  * @}
  */

/******************************************************************************/
/*  For a painless codes migration between the STM32L1xx device product       */
/*  lines, the aliases defined below are put in place to overcome the         */
/*  differences in the interrupt handlers and IRQn definitions.               */
/*  No need to update developed interrupt code when moving across             */ 
/*  product lines within the same STM32L1 Family                              */
/******************************************************************************/

/* Aliases for __IRQn */

/* Aliases for __IRQHandler */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STM32L152xE_H */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
