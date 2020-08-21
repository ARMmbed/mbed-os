/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __S1SBP6A_H
#define __S1SBP6A_H

typedef enum IRQn {
    /* -------------------  Cortex-M4 Processor Exceptions Numbers  ------------------- */
    NonMaskableInt_IRQn           = -14,      /*!<  2 Non Maskable Interrupt                        */
    MemoryManagement_IRQn         = -12,      /*!<  4 Memory Management Interrupt                   */
    BusFault_IRQn                 = -11,      /*!<  5 Bus Fault Interrupt                           */
    UsageFault_IRQn               = -10,      /*!<  6 Usage Fault Interrupt                         */
    SVCall_IRQn                   = -5,       /*!< 11 SV Call Interrupt                             */
    DebugMonitor_IRQn             = -4,       /*!< 12 Debug Monitor Interrupt                       */
    PendSV_IRQn                   = -2,       /*!< 14 Pend SV Interrupt                             */
    SysTick_IRQn                  = -1,       /*!< 15 System Tick Interrupt                         */

    /* ---------------------  S5JS100 Specific Interrupt Numbers  ---------------- */
    BOR_IRQn                      = 0,        /*!< Brown Out Reset Interrupt                        */
    UARTR0_IRQn                   = 1,        /*!< UART 0 Interrupt                                 */
    UARTR1_IRQn                   = 2,        /*!< UART 1 Interrupt                                 */
    UARTR2_IRQn                   = 3,        /*!< UART 2 Interrupt                                 */
    SPI0_IRQn                     = 4,        /*!< SPI 0 Interrupt                                  */
    SPI1_IRQn                     = 5,        /*!< SPI 1 Interrupt                                  */
    SPI2_IRQn                     = 6,        /*!< SPI 2 Interrupt                                  */
    SPI3_IRQn                     = 7,        /*!< SPI 3 Interrupt                                  */
    SPI4_IRQn                     = 8,        /*!< SPI 4 Interrupt                                  */
    I2C0_IRQn                     = 9,        /*!< I2C 0 Interrupt                                  */
    I2C1_IRQn                     = 10,       /*!< I2C 1 Interrupt                                  */
    I2C2_IRQn                     = 11,       /*!< I2C 2 Interrupt                                  */
    I2C3_IRQn                     = 12,       /*!< I2C 3 Interrupt                                  */
    I2C4_IRQn                     = 13,       /*!< I2C 4 Interrupt                                  */
    TIMER0_IRQn                   = 14,       /*!< TIMER 0 Interrupt                                */
    TIMER1_IRQn                   = 15,       /*!< TIMER 1 Interrupt                                */
    TIMER2_IRQn                   = 16,       /*!< TIMER 2 Interrupt                                */
    TIMER3_IRQn                   = 17,       /*!< TIMER 3 Interrupt                                */
    TIMER4_IRQn                   = 18,       /*!< TIMER 4 Interrupt                                */
    TIMER5_IRQn                   = 19,       /*!< TIMER 5 Interrupt                                */
    TIMER6_IRQn                   = 20,       /*!< TIMER 6 Interrupt                                */
    PWM_TIMER_IRQn                = 21,       /*!< PWM TIMER Interrupt                              */
    DUAL_TIMER_IRQn               = 22,       /*!< Dual Timer Interrupt                             */
    AFECON_IRQn                   = 23,       /*!< Analog Front End Control Interrupt               */
    SRC_IRQn                      = 24,       /*!< Sample Rate Converter Interrupt                  */
    DMA_ERROR_IRQn                = 25,       /*!< DMA Error Interrupt                              */
    FLASH_IRQn                    = 26,       /*!< FLASH Interrupt                                  */
    RTC0_IRQn                     = 27,       /*!< Real-Time Counter 0 Interrupt                    */
    RTC1_IRQn                     = 28,       /*!< Real-Time Counter 1 Interrupt                    */
    RTC2_IRQn                     = 29,       /*!< Real-Time Counter 2 Interrupt                    */
    FPU_IRQn                      = 30,       /*!< Floating-point Processing Unit Interrupt         */
    SRP0_IRQn                     = 31,       /*!< SRP 0 Interrupt                                  */
    SRP1_IRQn                     = 32,       /*!< SRP 1 Interrupt                                  */
    DELAY_MON_IRQn                = 33,       /*!< Delay Monitor Interrupt                          */
    AES_IRQn                      = 34,       /*!< AES Interrupt                                    */
    EXT_WAKEUP_IRQn               = 35,       /*!< External Wakeup Interrupt                        */
    DMA0_IRQn                     = 36,       /*!< DMA Channel 0 Done Interrupt                     */
    DMA1_IRQn                     = 37,       /*!< DMA Channel 1 Done Interrupt                     */
    DMA2_IRQn                     = 38,       /*!< DMA Channel 2 Done Interrupt                     */
    DMA3_IRQn                     = 39,       /*!< DMA Channel 3 Done Interrupt                     */
    DMA4_IRQn                     = 40,       /*!< DMA Channel 4 Done Interrupt                     */
    DMA5_IRQn                     = 41,       /*!< DMA Channel 5 Done Interrupt                     */
    DMA6_IRQn                     = 42,       /*!< DMA Channel 6 Done Interrupt                     */
    DMA7_IRQn                     = 43,       /*!< DMA Channel 7 Done Interrupt                     */
    DMA8_IRQn                     = 44,       /*!< DMA Channel 8 Done Interrupt                     */
    DMA9_IRQn                     = 45,       /*!< DMA Channel 9 Done Interrupt                     */
    DMA10_IRQn                    = 46,       /*!< DMA Channel 10 Done Interrupt                    */
    DMA11_IRQn                    = 47,       /*!< DMA Channel 11 Done Interrupt                    */
    DMA12_IRQn                    = 48,       /*!< DMA Channel 12 Done Interrupt                    */
    DMA13_IRQn                    = 49,       /*!< DMA Channel 13 Done Interrupt                    */
    DMA14_IRQn                    = 50,       /*!< DMA Channel 14 Done Interrupt                    */
    DMA15_IRQn                    = 51,       /*!< DMA Channel 15 Done Interrupt                    */
    DMA16_IRQn                    = 52,       /*!< DMA Channel 16 Done Interrupt                    */
    DMA17_IRQn                    = 53,       /*!< DMA Channel 17 Done Interrupt                    */
    DMA18_IRQn                    = 54,       /*!< DMA Channel 18 Done Interrupt                    */
    DMA19_IRQn                    = 55,       /*!< DMA Channel 19 Done Interrupt                    */
    DMA20_IRQn                    = 56,       /*!< DMA Channel 20 Done Interrupt                    */
    DMA21_IRQn                    = 57,       /*!< DMA Channel 21 Done Interrupt                    */
    DMA22_IRQn                    = 58,       /*!< DMA Channel 22 Done Interrupt                    */
    DMA23_IRQn                    = 59,       /*!< DMA Channel 23 Done Interrupt                    */
    DMA24_IRQn                    = 60,       /*!< DMA Channel 24 Done Interrupt                    */
    DMA25_IRQn                    = 61,       /*!< DMA Channel 25 Done Interrupt                    */
    DMA26_IRQn                    = 62,       /*!< DMA Channel 26 Done Interrupt                    */
    DMA27_IRQn                    = 63,       /*!< DMA Channel 27 Done Interrupt                    */
    DMA28_IRQn                    = 64,       /*!< DMA Channel 28 Done Interrupt                    */
    DMA29_IRQn                    = 65,       /*!< DMA Channel 29 Done Interrupt                    */
    DMA30_IRQn                    = 66,       /*!< DMA Channel 30 Done Interrupt                    */
    DMA31_IRQn                    = 67,       /*!< DMA Channel 31 Done Interrupt                    */
    PORT0_ALL_IRQn                = 68,       /*!< Port 0 Combined Interrupt                        */
    PORT1_ALL_IRQn                = 69,       /*!< Port 1 Combined Interrupt                        */
    PORT2_ALL_IRQn                = 70,       /*!< Port 2 Combined Interrupt                        */
    PORT0_0_IRQn                  = 71,       /*!< All P0 I/O pins can be used as interrupt source. */
    PORT0_1_IRQn                  = 72,       /*!< There are 16 pins in total                       */
    PORT0_2_IRQn                  = 73,
    PORT0_3_IRQn                  = 74,
    PORT0_4_IRQn                  = 75,
    PORT0_5_IRQn                  = 76,
    PORT0_6_IRQn                  = 77,
    PORT0_7_IRQn                  = 78,
    PORT0_8_IRQn                  = 79,
    PORT0_9_IRQn                  = 80,
    PORT0_10_IRQn                 = 81,
    PORT0_11_IRQn                 = 82,
    PORT0_12_IRQn                 = 83,
    PORT0_13_IRQn                 = 84,
    PORT0_14_IRQn                 = 85,
    PORT0_15_IRQn                 = 86,
    PORT1_0_IRQn                  = 87,       /*!< All P1 I/O pins can be used as interrupt source. */
    PORT1_1_IRQn                  = 88,       /*!< There are 16 pins in total                       */
    PORT1_2_IRQn                  = 89,
    PORT1_3_IRQn                  = 90,
    PORT1_4_IRQn                  = 91,
    PORT1_5_IRQn                  = 92,
    PORT1_6_IRQn                  = 93,
    PORT1_7_IRQn                  = 94,
    PORT1_8_IRQn                  = 95,
    PORT1_9_IRQn                  = 96,
    PORT1_10_IRQn                 = 97,
    PORT1_11_IRQn                 = 98,
    PORT1_12_IRQn                 = 99,
    PORT1_13_IRQn                 = 100,
    PORT1_14_IRQn                 = 101,
    PORT1_15_IRQn                 = 102,
    PORT2_0_IRQn                  = 103,      /*!< All P2 I/O pins can be used as interrupt source. */
    PORT2_1_IRQn                  = 104,      /*!< There are 16 pins in total                       */
    PORT2_2_IRQn                  = 105,
    PORT2_3_IRQn                  = 106,
    PORT2_4_IRQn                  = 107,
    PORT2_5_IRQn                  = 108,
    PORT2_6_IRQn                  = 109,
    PORT2_7_IRQn                  = 110,
    PORT2_8_IRQn                  = 111,
    PORT2_9_IRQn                  = 112,
    PORT2_10_IRQn                 = 113,
    PORT2_11_IRQn                 = 114,
    PORT2_12_IRQn                 = 115,
    PORT2_13_IRQn                 = 116,
    PORT2_14_IRQn                 = 117,
    PORT2_15_IRQn                 = 118,
    NUM_OF_IRQn                   = 119       /* Number of IRQs */
} IRQn_Type;

/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M4 Processor and Core Peripherals */
#define __CM4_REV                 0x0001U   /*!< Core Revision r0p1                               */
#define __NVIC_PRIO_BITS          3         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */
#define __MPU_PRESENT             1         /*!< MPU present or not                               */
#define __FPU_PRESENT             1         /*!< FPU present or not                               */

#include <core_cm4.h>                       /* Cortex-M4 processor and core peripherals           */
#include "system_s1sbp6a.h"                 /* S1SBP6A System  include file                       */

/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

/*---------- Timer (TIMER) ---------------------------------------------------*/
typedef struct {
    __IOM uint32_t CTRL;                  /*!< Offset: 0x000 Control Register                                     */
    __IOM uint32_t VALUE;                 /*!< Offset: 0x004 Current Value Register                               */
    __IOM uint32_t RELOAD;                /*!< Offset: 0x008 Reload Value Register                                */
    union {
        __IM  uint32_t INTSTAT;               /*!< Offset: 0x00C Interrupt Status Register                            */
        __OM  uint32_t INTCLR;                /*!< Offset: 0x00C Interrupt Clear Register                             */
    };
} BP_TIMER_TypeDef;

/*---------- PWM Timer (PWM_TIMER) -------------------------------------------*/

typedef struct {
    __IOM uint32_t CTRL;                  /*!< Offset: 0x000 Control Register                                     */
    __IOM uint32_t VALUE;                 /*!< Offset: 0x004 Current Value Register                               */
    __IOM uint32_t RELOAD;                /*!< Offset: 0x008 Reload Value Register                                */
    union {
        __IM  uint32_t INTSTAT;               /*!< Offset: 0x00C Interrupt Status Register                            */
        __OM  uint32_t INTCLR;                /*!< Offset: 0x00C Interrupt Clear Register                             */
    };
    uint32_t RESERVED[764];
    __IOM uint32_t PWM_CTRL;              /*!< Offset: 0xC00 PWM Control Register                                 */
    __IOM uint32_t PWM_PULSE;             /*!< Offset: 0xC04 PWM Pulse Set Register                               */
} BP_PWM_TIMER_TypeDef;

/*---------- Dual Input Timer (DUAL_TIMER) -----------------------------------*/

typedef struct {
    __IOM uint32_t LOAD;                  /*!< Offset: 0x000 Timer Load Value Register                            */
    __IM  uint32_t VALUE;                 /*!< Offset: 0x004 Timer Current Value Register                         */
    __IOM uint32_t CTRL;                  /*!< Offset: 0x008 Timer Control Register                               */
    __OM  uint32_t INTCLR;                /*!< Offset: 0x00C Timer Interrupt Clear Register                       */
    __IM  uint32_t RIS;                   /*!< Offset: 0x010 Timer Raw Interrupt Status Register                  */
    __IM  uint32_t MIS;                   /*!< Offset: 0x014 Timer Masked Interrupt Status Register               */
    __IOM uint32_t BGLOAD;                /*!< Offset: 0x018 Timer Background Load Register                       */
} BP_DUAL_TIMER_TypeDef;
/*@}*/ /* end of group BP_DUAL_TIMER */

/*---------- Watchdog (WATCHDOG) ---------------------------------------------*/
typedef struct {
    __IOM uint32_t LOAD;                  /*!< Offset: 0x000 Load Register                                        */
    __IM  uint32_t VALUE;                 /*!< Offset: 0x004 Value Register                                       */
    __IOM uint32_t CTRL;                  /*!< Offset: 0x008 Control Register                                     */
    __OM  uint32_t INTCLR;                /*!< Offset: 0x00C Clear Interrupt Register                             */
    __IM  uint32_t RIS;                   /*!< Offset: 0x010 Raw Interrupt Status Register                        */
    __IM  uint32_t MIS;                   /*!< Offset: 0x014 Masked Interrupt Status Register                     */
    uint32_t RESERVED[762];
    __IOM uint32_t LOCK;                  /*!< Offset: 0xC00 Lock Register                                        */
} BP_WATCHDOG_TypeDef;

/*---------- Universal Asynchronous Receiver Transmitter (UART) --------------*/
typedef struct {
    __IOM uint32_t ULCON;                 /*!< Offset: 0x000 Line Control Register                                */
    __IOM uint32_t UCON;                  /*!< Offset: 0x004 Control Register                                     */
    __IOM uint32_t UFCON;                 /*!< Offset: 0x008 FIFO Control Register                                */
    __IOM uint32_t UMCON;                 /*!< Offset: 0x00C AFC Control Register                                 */
    __IOM uint32_t UTRSTAT;               /*!< Offset: 0x010 Tx/Rx Status Register                                */
    __IM  uint32_t UERSTAT;               /*!< Offset: 0x014 Rx Error Register                                    */
    __IM  uint32_t UFSTAT;                /*!< Offset: 0x018 FIFO Status Register                                 */
    uint32_t RESERVED;
    __OM  uint32_t UTXH;                  /*!< Offset: 0x020 Transmit Data Register                               */
    __IM  uint32_t URXH;                  /*!< Offset: 0x024 Receive Data Register                                */
    __IOM uint32_t UBRDIV;                /*!< Offset: 0x028 Baud Rate Division Value Register                    */
    __IOM uint32_t UFRACVAL;              /*!< Offset: 0x02C Baud Rate Fractional Part Value Register             */
    __IOM uint32_t UINTP;                 /*!< Offset: 0x030 Interrupt Pending Register                           */
    __IM  uint32_t UINTS;                 /*!< Offset: 0x034 Interrupt Source Register                            */
    __IOM uint32_t UINTM;                 /*!< Offset: 0x038 Interrupt Mask Register                              */
} BP_UART_TypeDef;

/*---------- Serial Peripheral Interface (SPI) -------------------------------*/

typedef struct {
    __IOM uint32_t CR0;                   /*!< Offset: 0x000 Control Register 0                                   */
    __IOM uint32_t CR1;                   /*!< Offset: 0x004 Control Register 1                                   */
    __IOM uint32_t DR;                    /*!< Offset: 0x008 Data Register                                        */
    __IM  uint32_t SR;                    /*!< Offset: 0x00C Status Register                                      */
    __IOM uint32_t CPSR;                  /*!< Offset: 0x010 Clock Pre-scale Register                             */
    __IOM uint32_t IMSC;                  /*!< Offset: 0x014 Interrupt Mask Set and Clear Register                */
    __IM  uint32_t RIS;                   /*!< Offset: 0x018 Raw Interrupt Status Register                        */
    __IM  uint32_t MIS;                   /*!< Offset: 0x01C Masked Interrupt Status Register                     */
    __OM  uint32_t ICR;                   /*!< Offset: 0x020 Interrupt Clear Register                             */
    __IOM uint32_t DMACR;                 /*!< Offset: 0x024 DMA Control Register                                 */
} BP_SPI_TypeDef;


/*---------- Inter-Integrated Circuit (I2C) ----------------------------------*/
typedef struct {
    __IOM uint32_t CTL;                   /*!< Offset: 0x000 Control register                                     */
    __OM  uint32_t FIFO_CTL;              /*!< Offset: 0x004 FIFO Control Register                                */
    __IOM uint32_t TRAILING_CTL;          /*!< Offset: 0x008 Trailing Bytes Control Register                      */
    uint32_t RESERVED1[5];
    __IOM uint32_t INT_EN;                /*!< Offset: 0x020 Interrupt Enable Register                            */
    __IOM uint32_t INT_STAT;              /*!< Offset: 0x024 Interrupt Status Register                            */
    uint32_t RESERVED2[2];
    __IM  uint32_t FIFO_STAT;             /*!< Offset: 0x030 FIFO Status Register                                 */
    __OM  uint32_t TXDATA;                /*!< Offset: 0x034 Transmit Data Buffer Register                        */
    __IM  uint32_t RXDATA;                /*!< Offset: 0x038 Received Data Buffer Register                        */
    uint32_t RESERVED3;
    __IOM uint32_t CONF;                  /*!< Offset: 0x040 Configuration Register                               */
    __IOM uint32_t AUTO_CONF;             /*!< Offset: 0x044 Configuration Register for Auto Mode                 */
    __IOM uint32_t TIMEOUT;               /*!< Offset: 0x048 Time Out Register for I2C Master                     */
    __IOM uint32_t MANUAL_CMD;            /*!< Offset: 0x04C Command Register for Manual Mode                     */
    __IM  uint32_t TRANS_STATUS;          /*!< Offset: 0x050 Transfer Status Register                             */
    __IOM uint32_t TIMING_HS1;            /*!< Offset: 0x054 Timing Register1 for High Speed Mode                 */
    __IOM uint32_t TIMING_HS2;            /*!< Offset: 0x058 Timing Register2 for High Speed Mode                 */
    __IOM uint32_t TIMING_HS3;            /*!< Offset: 0x05C Timing Register3 for High Speed Mode                 */
    __IOM uint32_t TIMING_FS1;            /*!< Offset: 0x060 Timing Register1 for Fast Speed Mode                 */
    __IOM uint32_t TIMING_FS2;            /*!< Offset: 0x064 Timing Register2 for Fast Speed Mode                 */
    __IOM uint32_t TIMING_FS3;            /*!< Offset: 0x068 Timing Register3 for Fast Speed Mode                 */
    __IOM uint32_t TIMING_SLA;            /*!< Offset: 0x06C Data Hold Timing Register for I2C Slave              */
    __IOM uint32_t ADDR;                  /*!< Offset: 0x070 Address Register                                     */
} BP_I2C_TypeDef;

/*---------- Delay Monitor ---------------------------------------------------*/

typedef struct {
    __IOM uint32_t CFG;                   /*!< Offset: 0x000 Configuration Register                               */
    __IOM uint32_t CHAINSEL_MCU;          /*!< Offset: 0x004 Chain Selection Register for MCU                     */
    __IOM uint32_t CHAINSEL_SRP;          /*!< Offset: 0x008 Chain Selection Register for SRP                     */
    __IOM uint32_t CHAINMASK_MCU;         /*!< Offset: 0x00C Chain Selection Mask Register for MCU                */
    __IOM uint32_t CHAINMASK_SRP;         /*!< Offset: 0x010 Chain Selection Mask Register for SRP                */
    __IM  uint32_t DMONQ_MCU;             /*!< Offset: 0x014 Flip-Flop Output Register for MCU                    */
    __IM  uint32_t DMONQ_SRP;             /*!< Offset: 0x018 Flip-Flop Output Register for SRP                    */
    __IM  uint32_t DMONCNT_L;             /*!< Offset: 0x01C Violation Counter Lower Register                     */
    __IM  uint32_t DMONCNT_H;             /*!< Offset: 0x020 Violation Counter Higher Register                    */
} BP_DELAY_MON_TypeDef;

/*---------- Real Time Clock -------------------------------------------------*/
typedef struct {
    __IOM uint32_t PMU_KA_DLY_CON0;           /*!< Offset: 0x000 PMU_KA_DLY_CON0                                  */
    __IOM uint32_t PMU_KA_DLY_CON1;           /*!< Offset: 0x004 PMU_KA_DLY_CON1                                  */
    __IOM uint32_t PMU_KA_LDO_CTRL;           /*!< Offset: 0x008 PMU_KA_LDO_CTRL                                  */
    __IM  uint32_t PMU_KA_LDO_POR_STATUS;     /*!< Offset: 0x00C PMU_KA_LDO_POR_STATUS                            */
    __IOM uint32_t PMU_KA_MANUAL_CTRL;        /*!< Offset: 0x010 PMU_KA_MANUAL_CTRL                               */
    __IOM uint32_t KA_AFEISO_CTRL;            /*!< Offset: 0x014 KA_AFEISO_CTRL                                   */
    __IM  uint32_t KA_SYSTEM_ON;              /*!< Offset: 0x018 KA_SYSTEM_ON                                     */
    uint32_t RESERVED;
    __IOM uint32_t CALENDAR_MODE;             /*!< Offset: 0x020 CALENDAR_MODE                                    */
    __IOM uint32_t GEN_COUNT_CLR;             /*!< Offset: 0x024 GEN_COUNT_CLR                                    */
    __IOM uint32_t LOAD_COUNT_SIG;            /*!< Offset: 0x028 LOAD_COUNT_SIG                                   */
    struct {
        __IO uint32_t SEC;                     /*!< Offset: 0x02C LOAD_COUNT_VALUE0                                */
        __IO uint32_t MIN;                     /*!< Offset: 0x030 LOAD_COUNT_VALUE1                                */
        __IO uint32_t HOUR;                    /*!< Offset: 0x034 LOAD_COUNT_VALUE2                                */
        __IO uint32_t DAY;                     /*!< Offset: 0x038 LOAD_COUNT_VALUE3                                */
        __IO uint32_t MON;                     /*!< Offset: 0x03C LOAD_COUNT_VALUE4                                */
        __IO uint32_t YEAR;                    /*!< Offset: 0x040 LOAD_COUNT_VALUE5                                */
    } BCD_W;
    struct {
        __IO uint32_t SEC;                      /*!< Offset: 0x044 CALENDAR_COUNT_VALUE0                            */
        __IO uint32_t MIN;                      /*!< Offset: 0x048 CALENDAR_COUNT_VALUE1                            */
        __IO uint32_t HOUR;                     /*!< Offset: 0x04C CALENDAR_COUNT_VALUE2                            */
        __IO uint32_t DAY;                      /*!< Offset: 0x050 CALENDAR_COUNT_VALUE3                            */
        __IO uint32_t MON;                      /*!< Offset: 0x054 CALENDAR_COUNT_VALUE4                            */
        __IO uint32_t YEAR;                     /*!< Offset: 0x058 CALENDAR_COUNT_VALUE5                            */
    } BCD_R;
    __IOM uint32_t CALENDAR_TIME_STAMP;       /*!< Offset: 0x05C CALENDAR_TIME_STAMP                              */
    __IOM uint32_t INT_ENABLE_CPU;            /*!< Offset: 0x060 INT_ENABLE_CPU                                   */
    __IOM uint32_t INT_CLEAR_CPU;             /*!< Offset: 0x064 INT_CLEAR_CPU                                    */
    __IOM uint32_t RESERVED1;
    __IOM uint32_t SYNC_INT_TIME0;            /*!< Offset: 0x06C SYNC_INT_TIME0                                   */
    __IOM uint32_t SYNC_INT_TIME1;            /*!< Offset: 0x070 SYNC_INT_TIME1                                   */
    __IM  uint32_t INT_STATUS_CPU;            /*!< Offset: 0x074 INT_STATUS_CPU                                   */
    __IOM uint32_t ALARM_ENABLE;              /*!< Offset: 0x078 ALARM_ENABLE                                     */
    __IOM uint32_t ALARM_COUNT_VALUE0;        /*!< Offset: 0x07C ALARM_COUNT_VALUE0                               */
    __IOM uint32_t ALARM_COUNT_VALUE1;        /*!< Offset: 0x080 ALARM_COUNT_VALUE1                               */
    __IOM uint32_t ALARM_COUNT_VALUE2;        /*!< Offset: 0x084 ALARM_COUNT_VALUE2                               */
    __IOM uint32_t ALARM_COUNT_VALUE3;        /*!< Offset: 0x088 ALARM_COUNT_VALUE3                               */
    __IOM uint32_t ALARM_COUNT_VALUE4;        /*!< Offset: 0x08C ALARM_COUNT_VALUE4                               */
    __IOM uint32_t ALARM_COUNT_VALUE5;        /*!< Offset: 0x090 ALARM_COUNT_VALUE5                               */
    __IM  uint32_t GENERAL_COUNT0;            /*!< Offset: 0x094 GENERAL_COUNT0                                   */
    __IM  uint32_t GENERAL_COUNT1;            /*!< Offset: 0x098 GENERAL_COUNT1                                   */
    __IOM uint32_t STOP_COUNT_ENABLE;         /*!< Offset: 0x09C STOP_COUNT_ENABLE                                */
    __IOM uint32_t STOP_COUNT_CLEAR;          /*!< Offset: 0x0A0 STOP_COUNT_CLEAR                                 */
    __IM  uint32_t STOP_COUNT_TIME_STAMP0;    /*!< Offset: 0x0A4 STOP_COUNT_TIME_STAMP0                           */
    __IM  uint32_t STOP_COUNT_TIME_STAMP1;    /*!< Offset: 0x0A8 STOP_COUNT_TIME_STAMP1                           */
    __IM  uint32_t STOP_COUNT_TIME_STAMP2;    /*!< Offset: 0x0AC STOP_COUNT_TIME_STAMP2                           */
    __IM  uint32_t STOP_COUNT_TIME_STAMP3;    /*!< Offset: 0x0B0 STOP_COUNT_TIME_STAMP3                           */
    __IOM uint32_t AUTO_COUNT_CLR;            /*!< Offset: 0x0B4 AUTO_COUNT_CLR                                   */
    __IOM uint32_t STOP_COUNT_LOAD0;          /*!< Offset: 0x0B8 STOP_COUNT_LOAD0                                 */
    __IOM uint32_t STOP_COUNT_LOAD1;          /*!< Offset: 0x0BC STOP_COUNT_LOAD1                                 */
    __IOM uint32_t STOP_COUNT_LOAD2;          /*!< Offset: 0x0C0 STOP_COUNT_LOAD2                                 */
    __IOM uint32_t STOP_COUNT_LOAD3;          /*!< Offset: 0x0C4 STOP_COUNT_LOAD3                                 */
    __IOM uint32_t GENERAL_COUNT_SNAP_SHOT;   /*!< Offset: 0x0C8 GENERAL_COUNT_SNAP_SHOT_COMMAND                  */
    __IOM uint32_t STOP_COUNT_SNAP_SHOT;      /*!< Offset: 0x0CC STOP_COUNT_SNAP_SHOT_COMMAND                     */
    __IOM uint32_t PWMCTRL;                   /*!< Offset: 0x0D0 PWMCTRL                                          */
    __IOM uint32_t PWM_PULSE0;                /*!< Offset: 0x0D4 PWM_PULSE0                                       */
    __IOM uint32_t PWM_PULSE1;                /*!< Offset: 0x0D8 PWM_PULSE1                                       */
    __IOM uint32_t PWM_PULSE2;                /*!< Offset: 0x0DC PWM_PULSE2                                       */
    __IOM uint32_t PWM_PULSE3;                /*!< Offset: 0x0E0 PWM_PULSE3                                       */
    __IM  uint32_t KA_BOR_INT;                /*!< Offset: 0x0E4 KA_BOR_INT                                       */
    __IOM uint32_t KA_BOR_INT_CLEAR;          /*!< Offset: 0x0E8 KA_BOR_INT_CLEAR                                 */
    __IOM uint32_t KA_DLY_EN;                 /*!< Offset: 0x0EC KA_DLY_EN                                        */
    __IOM uint32_t KA_32K_XTAL_CON;           /*!< Offset: 0x0F0 KA_32K_XTAL_CON                                  */
    __IOM uint32_t KA_IOPWR_CTRL;             /*!< Offset: 0x0F4 KA_IOPWR_CTRL                                    */
    __IOM uint32_t RTC_CLK_MUX;               /*!< Offset: 0x0F8 RTC_CLK_MUX                                      */
    __IOM uint32_t KA_PM_IV_EN;               /*!< Offset: 0x0FC KA_PM_IV_EN                                      */
} BP_RTC_TypeDef;

/*---------- Analog Front End Top Control ------------------------------------*/

typedef struct {
    __IOM uint32_t AFE_CLK_CTRL;          /*!< Offset: 0x000 AFE_CLK_CTRL                                         */
    __IOM uint32_t AFE_RST_CTRL;          /*!< Offset: 0x004 AFE_RST_CTRL                                         */
    __IOM uint32_t GPIO_TMUX_SEL;         /*!< Offset: 0x008 GPIO_TMUX_SEL                                        */
    __IOM uint32_t GP_DAC0;               /*!< Offset: 0x00C GP_DAC0                                              */
    __IOM uint32_t GP_DAC1;               /*!< Offset: 0x010 GP_DAC1                                              */
    __IOM uint32_t GP_ADC0;               /*!< Offset: 0x014 GP_ADC0                                              */
    __IOM uint32_t GP_ADC1;               /*!< Offset: 0x018 GP_ADC1                                              */
    __IOM uint32_t REF_CTRL;              /*!< Offset: 0x01C REF_CTRL                                             */
    __IOM uint32_t REGULATOR_CTRL0;       /*!< Offset: 0x020 REGULATOR_CTRL0                                      */
    __IOM uint32_t REGULATOR_CTRL1;       /*!< Offset: 0x024 REGULATOR_CTRL1                                      */
    uint32_t RESERVED1[4];
    __IOM uint32_t TMPX;                  /*!< Offset: 0x038 TMPX                                                 */
    uint32_t RESERVED2;
    __IOM uint32_t V11_RESERVED0;         /*!< Offset: 0x040 V11_RESERVED0                                        */
    __IOM uint32_t V11_RESERVED1;         /*!< Offset: 0x044 V11_RESERVED1                                        */
    __IOM uint32_t V11_RESERVED2;         /*!< Offset: 0x048 V11_RESERVED2                                        */
    __IOM uint32_t V11_RESERVED3;         /*!< Offset: 0x04C V11_RESERVED3                                        */
    __IOM uint32_t V3_RESERVED0;          /*!< Offset: 0x050 V3_RESERVED0                                         */
    __IOM uint32_t V3_RESERVED1;          /*!< Offset: 0x054 V3_RESERVED1                                         */
    __IOM uint32_t TRIM_SELB;             /*!< Offset: 0x058 TRIM_SELB                                            */
    __IOM uint32_t FM_SEL;                /*!< Offset: 0x05C FM_SEL                                               */
} BP_AFE_TOP_TypeDef;


/*---------- Analog Front End ADC FIFO Control ---------------------------------*/
typedef struct {
    __IOM uint32_t FIFO_INTR_MASK;            /*!< Offset: 0x000 FIFO_INTR_MASK                                   */
    __IOM uint32_t FIFO_SW_RESET;             /*!< Offset: 0x004 FIFO_SW_RESET                                    */
    __IOM uint32_t FIFO_DMA_SIZE0;            /*!< Offset: 0x008 FIFO_DMA_SIZE0                                   */
    __IOM uint32_t FIFO_READY_SIZE0;          /*!< Offset: 0x00C FIFO_READY_SIZE0                                 */
    __IOM uint32_t FIFO_ENABLE;               /*!< Offset: 0x010 FIFO_ENABLE                                      */
    __IOM uint32_t FIFO_DMA_ENABLE;           /*!< Offset: 0x014 FIFO_DMA_ENABLE                                  */
    __IOM uint32_t FIFO_INT_CLEAR;            /*!< Offset: 0x018 FIFO_INT_CLEAR                                   */
    __IM  uint32_t FIFO0_READ_DATA;           /*!< Offset: 0x01C FIFO0_READ_DATA                                  */
    __IM  uint32_t FIFO1_READ_DATA;           /*!< Offset: 0x020 FIFO1_READ_DATA                                  */
    __IM  uint32_t FIFO2_READ_DATA;           /*!< Offset: 0x024 FIFO2_READ_DATA                                  */
    __IM  uint32_t FIFO3_READ_DATA;           /*!< Offset: 0x028 FIFO3_READ_DATA                                  */
    __IM  uint32_t FIFO4_READ_DATA;           /*!< Offset: 0x02C FIFO4_READ_DATA                                  */
    __IM  uint32_t FIFO5_READ_DATA;           /*!< Offset: 0x030 FIFO5_READ_DATA                                  */
    __IM  uint32_t FIFO6_READ_DATA;           /*!< Offset: 0x034 FIFO6_READ_DATA                                  */
    __IM  uint32_t FIFO7_READ_DATA;           /*!< Offset: 0x038 FIFO7_READ_DATA                                  */
    __IM  uint32_t FIFO8_READ_DATA;           /*!< Offset: 0x03C FIFO8_READ_DATA                                  */
    uint32_t RESERVED;
    __IM  uint32_t FIFO_DATA_CNT_SAR;             /*!< Offset: 0x044 FIFO_DATA_SAR                                    */
    __IM  uint32_t FIFO_DATA_CNT_IDC;             /*!< Offset: 0x048 FIFO_DATA_IDC                                    */
    __IM  uint32_t FIFO_STATUS;               /*!< Offset: 0x04C FIFO_STATUS                                      */
    __IOM uint32_t FIFO_RCLK_MANUAL_CG_EN;    /*!< Offset: 0x050 FIFO_RCLK_MANUAL_CG_EN                           */
    __IOM uint32_t FIFO_RCLK_MANUAL_CG;       /*!< Offset: 0x054 FIFO_RCLK_MANUAL_CG                              */
    __IOM uint32_t FIFO_DMA_SIZE1;            /*!< Offset: 0x058 FIFO_DMA_SIZE1                                   */
    __IOM uint32_t FIFO_READY_SIZE1;          /*!< Offset: 0x05C FIFO_READY_SIZE1                                 */
    __IM  uint32_t FIFO_DATA_CNT_SDC;             /*!< Offset: 0x060 FIFO_DATA_SDC                                    */
    __IM  uint32_t FIFO_INT_STATUS;           /*!< Offset: 0x064 FIFO_INT_STATUS                                  */
} BP_ADC_FIFO_TypeDef;


/*---------- Analog Front End ADC Control ------------------------------------*/

typedef struct {
    __IOM uint32_t SAR_SEL;               /*!< Offset: 0x000 SAR_SEL                                              */
    __IOM uint32_t DC_COMPEN_ADC0;        /*!< Offset: 0x004 DC_COMPEN_ADC0                                       */
    __IOM uint32_t DC_COMPEN_ADC1;        /*!< Offset: 0x008 DC_COMPEN_ADC1                                       */
    __IOM uint32_t DC_COMPEN_ADC2;        /*!< Offset: 0x00C DC_COMPEN_ADC2                                       */
    uint32_t RESERVED1[3];
    __IM  uint32_t SAR_ADC0_CH0_DATA;     /*!< Offset: 0x01C SAR_ADC0_CH0_DATA                                    */
    __IM  uint32_t SAR_ADC0_CH1_DATA;     /*!< Offset: 0x020 SAR_ADC0_CH1_DATA                                    */
    __IM  uint32_t SAR_ADC1_CH0_DATA;     /*!< Offset: 0x024 SAR_ADC1_CH0_DATA                                    */
    __IM  uint32_t SAR_ADC1_CH1_DATA;     /*!< Offset: 0x028 SAR_ADC1_CH1_DATA                                    */
    __IM  uint32_t SAR_ADC2_CH0_DATA;     /*!< Offset: 0x02C SAR_ADC2_CH0_DATA                                    */
    __IM  uint32_t SAR_ADC2_CH1_DATA;     /*!< Offset: 0x030 SAR_ADC2_CH1_DATA                                    */
    __IM  uint32_t SD_ADC0_DATA;          /*!< Offset: 0x034 SD_ADC0_DATA                                         */
    __IM  uint32_t SD_ADC1_DATA;          /*!< Offset: 0x038 SD_ADC1_DATA                                         */
    __IM  uint32_t SD_ADC2_DATA;          /*!< Offset: 0x03C SD_ADC2_DATA                                         */
    uint32_t RESERVED2;
    __IOM uint32_t SAR_ADC_CON;           /*!< Offset: 0x044 SAR_ADC_CON                                          */
    __IOM uint32_t SAR_ADC_CTRL;          /*!< Offset: 0x048 SAR_ADC_CTRL                                         */
} BP_ADC_CON_TypeDef;

/*---------- Analog Front End SDADC Control ----------------------------------*/
typedef struct {
    __IOM uint32_t SDADC_CTRL1;           /*!< Offset: 0x000 SDADC_CTRL1                                          */
    __IOM uint32_t SDADC_CTRL2;           /*!< Offset: 0x004 SDADC_CTRL2                                          */
    __IOM uint32_t SDADC_CTRL3;           /*!< Offset: 0x008 SDADC_CTRL3                                          */
    __IOM uint32_t RESERVED;
    __IOM uint32_t GPADC_TEST_MODE_CTRL;  /*!< Offset: 0x010 GPADC_TEST_MODE_CTRL                                 */
    __IOM uint32_t GPADC_LDO_CTRL;        /*!< Offset: 0x014 GPADC_LDO_CTRL                                       */
} BP_SDADC_CON_TypeDef;

/*---------- Analog Front End ECG --------------------------------------------*/

typedef struct {
    __IOM uint32_t ECG_EN;                /*!< Offset: 0x000 ECG_EN                                               */
    __IOM uint32_t ECG_GAIN;              /*!< Offset: 0x004 ECG_GAIN                                             */
    __IOM uint32_t ECG_BW;                /*!< Offset: 0x008 ECG_BW                                               */
    __IOM uint32_t ECG_SET;               /*!< Offset: 0x00C ECG_SET                                              */
    __IOM uint32_t ECG_INIT;              /*!< Offset: 0x010 ECG_INIT                                             */
    __IOM uint32_t ECG_HPF;               /*!< Offset: 0x014 ECG_HPF                                              */
    __IOM uint32_t ECG_CONT;              /*!< Offset: 0x018 ECG_CONT                                             */
} BP_AFE_ECG_TypeDef;

/*---------- Analog Front End PPG --------------------------------------------*/

typedef struct {
    __IOM uint32_t PPG_GLOBALEN;          /*!< Offset: 0x000 PPG_GLOBALEN                                         */
    __IOM uint32_t PPG0_FUNCEN;           /*!< Offset: 0x004 PPG0_FUNCEN                                          */
    __IOM uint32_t PPG0_TIA;              /*!< Offset: 0x008 PPG0_TIA                                             */
    __IOM uint32_t PPG0_IDC;              /*!< Offset: 0x00C PPG0_IDC                                             */
    __IOM uint32_t PPG0_PRT;              /*!< Offset: 0x010 PPG0_PRT                                             */
    __IOM uint32_t PPG0_LEDONT;           /*!< Offset: 0x014 PPG0_LEDONT                                          */
    __IOM uint32_t PPG0_CONVT;            /*!< Offset: 0x018 PPG0_CONVT                                           */
    __IOM uint32_t PPG0_IDCRSTT;          /*!< Offset: 0x01C PPG0_IDCRSTT                                         */
    __IOM uint32_t PPG0_AFESETT;          /*!< Offset: 0x020 PPG0_AFESETT                                         */
    __IOM uint32_t PPG0_PDNTCTRL;         /*!< Offset: 0x024 PPG0_PDNTCTRL                                        */
    __IOM uint32_t PPG0_LEDTSCTRL;        /*!< Offset: 0x028 PPG0_LEDTSCTRL                                       */
    __IOM uint32_t PPG0_LEDTSCDAC;        /*!< Offset: 0x02C PPG0_LEDTSCDAC                                       */
    __IOM uint32_t PPG0_DCCCTRL;          /*!< Offset: 0x030 PPG0_DCCCTRL                                         */
    uint32_t RESERVED1[4];
    __IOM uint32_t PPG1_FUNCEN;           /*!< Offset: 0x044 PPG1_FUNCEN                                          */
    __IOM uint32_t PPG1_TIA;              /*!< Offset: 0x048 PPG1_TIA                                             */
    __IOM uint32_t PPG1_IDC;              /*!< Offset: 0x04C PPG1_IDC                                             */
    __IOM uint32_t PPG1_PRT;              /*!< Offset: 0x050 PPG1_PRT                                             */
    uint32_t RESERVED2;
    __IOM uint32_t PPG1_CONVT;            /*!< Offset: 0x058 PPG1_CONVT                                           */
    __IOM uint32_t PPG1_IDCRSTT;          /*!< Offset: 0x05C PPG1_IDCRSTT                                         */
    __IOM uint32_t PPG1_AFESETT;          /*!< Offset: 0x060 PPG1_AFESETT                                         */
    __IOM uint32_t PPG1_PDNTCTRL;         /*!< Offset: 0x064 PPG1_PDNTCTRL                                        */
    uint32_t RESERVED3[2];
    __IOM uint32_t PPG1_DCCCTRL;          /*!< Offset: 0x070 PPG1_DCCCTRL                                         */
    uint32_t RESERVED4[3];
    __IOM uint32_t PPG_GENERAL;           /*!< Offset: 0x080 PPG_GENERAL                                          */
    __IOM uint32_t PPG_AGCLEVEL0;         /*!< Offset: 0x084 PPG_AGCLEVEL0                                        */
    __IOM uint32_t PPG_AGCLEVEL1;         /*!< Offset: 0x088 PPG_AGCLEVEL1                                        */
    __IOM uint32_t PPG_AGCLEVEL2;         /*!< Offset: 0x08C PPG_AGCLEVEL2                                        */
    __IOM uint32_t PPG_AGCTIME;           /*!< Offset: 0x090 PPG_AGCTIME                                          */
    __IOM uint32_t PPG_AGCSTEP;           /*!< Offset: 0x094 PPG_AGCSTEP                                          */
    __IM  uint32_t PPG_AGCSTATUS_TS0;     /*!< Offset: 0x098 PPG_AGCSTATUS_TS0                                    */
    __IM  uint32_t PPG_AGCDCAVG_TS0;      /*!< Offset: 0x09C PPG_AGCDCAVG_TS0                                     */
    __IM  uint32_t PPG_AGCSTATUS_TS1;     /*!< Offset: 0x0A0 PPG_AGCSTATUS_TS1                                    */
    __IM  uint32_t PPG_AGCDCAVG_TS1;      /*!< Offset: 0x0A4 PPG_AGCDCAVG_TS1                                     */
    __IM  uint32_t PPG_AGCSTATUS_TS2;     /*!< Offset: 0x0A8 PPG_AGCSTATUS_TS2                                    */
    __IM  uint32_t PPG_AGCDCAVG_TS2;      /*!< Offset: 0x0AC PPG_AGCDCAVG_TS2                                     */
    __IM  uint32_t PPG_AGCSTATUS_TS3;     /*!< Offset: 0x0B0 PPG_AGCSTATUS_TS3                                    */
    __IM  uint32_t PPG_AGCDCAVG_TS3;      /*!< Offset: 0x0B4 PPG_AGCDCAVG_TS3                                     */
    __IOM uint32_t PPG_MANUAL_TEST;       /*!< Offset: 0x0B8 PPG_MANUAL_TEST                                      */
    uint32_t RESERVED5;
    __IOM uint32_t LED_FUNCEN;            /*!< Offset: 0x0C0 LED_FUNCEN                                           */
    __IOM uint32_t LED_TRIM;              /*!< Offset: 0x0C4 LED_TRIM                                             */
    __IOM uint32_t LED_CDAC0;             /*!< Offset: 0x0C8 LED_CDAC0                                            */
    __IOM uint32_t LED_CDAC1;             /*!< Offset: 0x0CC LED_CDAC1                                            */
    __IOM uint32_t LED_CDAC2;             /*!< Offset: 0x0D0 LED_CDAC2                                            */
    __IOM uint32_t LED_CDAC3;             /*!< Offset: 0x0D4 LED_CDAC3                                            */
} BP_AFE_PPG_TypeDef;

/*---------- Analog Front End BIA --------------------------------------------*/

typedef struct {
    __IOM uint32_t BIA_TX_FIL;            /*!< Offset: 0x000 BIA_TX_FIL                                           */
    __IOM uint32_t BIA_TX_FREQ;           /*!< Offset: 0x004 BIA_TX_FREQ                                          */
    __IOM uint32_t BIA_TX_AMPL;           /*!< Offset: 0x008 BIA_TX_AMPL                                          */
    __IOM uint32_t BIA_TX_CUR;            /*!< Offset: 0x00C BIA_TX_CUR                                           */
    __IOM uint32_t BIA_TX_SW;             /*!< Offset: 0x010 BIA_TX_SW                                            */
    __IOM uint32_t BIA_RX_INPUT;          /*!< Offset: 0x014 BIA_RX_INPUT                                         */
    __IOM uint32_t BIA_RX_AMP1;           /*!< Offset: 0x018 BIA_RX_AMP1                                          */
    __IOM uint32_t BIA_RX_AMP2;           /*!< Offset: 0x01C BIA_RX_AMP2                                          */
    __IOM uint32_t BIA_RX_FIL;          /*!< Offset: 0x020 BIA_RX_FIL                                           */
    __IOM uint32_t BIA_RX_AMP3;           /*!< Offset: 0x024 BIA_RX_AMP3                                          */
    __IOM uint32_t BIA_RX_COMP;           /*!< Offset: 0x028 BIA_RX_COMP                                          */
    __IOM uint32_t BIA_TMUX1;             /*!< Offset: 0x02C BIA_TMUX1                                            */
    __IOM uint32_t BIA_TMUX2;             /*!< Offset: 0x030 BIA_TMUX2                                            */
    __IOM uint32_t BIA_TMUX3;             /*!< Offset: 0x034 BIA_TMUX3                                            */
    __IOM uint32_t BIA_DCTRL1;            /*!< Offset: 0x038 BIA_DCTRL1                                           */
    __IOM uint32_t BIA_IQ_GEN;            /*!< Offset: 0x03C BIA_IQ_GEN                                           */
    __IOM uint32_t BIA_DCTRL3;            /*!< Offset: 0x040 BIA_DCTRL3                                           */
    uint32_t RESERVED;
    __IOM uint32_t BIA_TRX_EN;            /*!< Offset: 0x048 BIA_TRX_EN                                           */
    __IOM uint32_t BIA_GSR_MODE;          /*!< Offset: 0x04C BIA_GSR_MODE                                         */
    __IOM uint32_t BIA_CURRENT_CTRL;      /*!< Offset: 0x050 BIA_CURRENT_CTRL                                     */
    __IOM uint32_t BIA_IMP_CTRL;          /*!< Offset: 0x054 BIA_IMP_CTRL                                         */
    __IOM uint32_t BIA_HPF_EN;            /*!< Offset: 0x058 BIA_HPF_EN                                           */
    __IOM uint32_t BIA_TESTMUX_EN;        /*!< Offset: 0x05C BIA_TESTMUX_EN                                       */
} BP_AFE_BIA_TypeDef;

/*---------- Analog Front End GSR --------------------------------------------*/

typedef struct {
    __IOM uint32_t CTRL;                  /*!< Offset: 0x000 GSR HW Control Register 0                            */
    __IOM uint32_t SW_CTRL;               /*!< Offset: 0x004 GSR SW Control Register 1                            */
    __IOM uint32_t FIT3PCTRL;             /*!< Offset: 0x008 GSR External register fitting Register               */
    __IOM uint32_t FIT3NCTRL;             /*!< Offset: 0x00C GSR External register fitting Register               */
    //__IOM uint32_t FIT1PCTRL;             /*!< Offset: 0x010 GSR External register fitting Register               */
    //__IOM uint32_t FIT1NCTRL;             /*!< Offset: 0x014 GSR External register fitting Register               */
} BP_AFE_GSR_TypeDef;

/*------------- PL230 uDMA (uDMA) --------------------------------------------*/

typedef struct {
    __IM  uint32_t DMA_STATUS;            /*!< Offset: 0x000 DMA Status Register                                  */
    __OM  uint32_t DMA_CFG;               /*!< Offset: 0x004 DMA Configuration Register                           */
    __IOM uint32_t CTRL_BASE_PTR;         /*!< Offset: 0x008 Channel Control Data Base Pointer Register           */
    __IM  uint32_t ALT_CTRL_BASE_PTR;     /*!< Offset: 0x00C Channel Alternate Control Data Base Pointer Register */
    __IM  uint32_t DMA_WAITONREQ_STATUS;  /*!< Offset: 0x010 Channel Wait On Request Status Register              */
    __OM  uint32_t CHNL_SW_REQUEST;       /*!< Offset: 0x014 Channel Software Request Register                    */
    __IOM uint32_t CHNL_USEBURST_SET;     /*!< Offset: 0x018 Channel UseBurst Set Register                        */
    __OM  uint32_t CHNL_USEBURST_CLR;     /*!< Offset: 0x01C Channel UseBurst Clear Register                      */
    __IOM uint32_t CHNL_REQ_MASK_SET;     /*!< Offset: 0x020 Channel Request Mask Set Register                    */
    __OM  uint32_t CHNL_REQ_MASK_CLR;     /*!< Offset: 0x024 Channel Request Mask Clear Register                  */
    __IOM uint32_t CHNL_ENABLE_SET;       /*!< Offset: 0x028 Channel Enable Set Register                          */
    __OM  uint32_t CHNL_ENABLE_CLR;       /*!< Offset: 0x02C Channel Enable Clear Register                        */
    __IOM uint32_t CHNL_PRI_ALT_SET;      /*!< Offset: 0x030 Channel Primary-Alternate Set Register               */
    __OM  uint32_t CHNL_PRI_ALT_CLR;      /*!< Offset: 0x034 Channel Primary-Alternate Clear Register             */
    __IOM uint32_t CHNL_PRIORITY_SET;     /*!< Offset: 0x038 Channel Priority Set Register                        */
    __OM  uint32_t CHNL_PRIORITY_CLR;     /*!< Offset: 0x03C Channel Priority Clear Register                      */
    uint32_t RESERVED1[3];
    __IOM uint32_t ERR_CLR;               /*!< Offset: 0x04C Bus Error Clear Register                             */
    __IOM uint32_t DMA_MUX;               /*!< Offset: 0x050 DMA Channel Mux Within I2C and SPI Peripherals       */
    __IOM uint32_t DMA_NOT_MUX;           /*!< Offset: 0x054 DMA Channel Not Muxed Within I2C and SPI Peripherals */
    __IOM uint32_t DMA_REQ_GATE;          /*!< Offset: 0x058 DMA Request Gating Enable Register                   */
} BP_DMA_TypeDef;

/*---------- System Control (SYSCON) -----------------------------------------*/
typedef struct {
    __IOM uint32_t PMU_SYSCON_CTRL;               /*!< Offset: 0x000 PMU_SYSCON_CTRL                              */
    __IOM uint32_t PMU_SYSCON_DLY_CTRL;           /*!< Offset: 0x004 PMU_SYSCON_DLY_CTRL                          */
    __IOM uint32_t PMU_MCU_DLY_CTRL;              /*!< Offset: 0x008 PMU_MCU_DLY_CTRL                             */
    __IOM uint32_t PMU_SYSCON_DLY_EN;             /*!< Offset: 0x00C PMU_SYSCON_DLY_EN                            */
    __IOM uint32_t RESERVED1;
    __IOM uint32_t PMU_SYSCON_FAST_BOOT;          /*!< Offset: 0x014 PMU_SYSCON_FAST_BOOT                         */
    uint32_t RESERVED2[58];
    __IOM uint32_t MCU_CLK_CTRL;                  /*!< Offset: 0x100 MCU_CLK_CTRL                                 */
    __IOM uint32_t MCU_CLK_GATE;                  /*!< Offset: 0x104 MCU_CLK_GATE                                 */
    uint32_t RESERVED3[2];
    __IOM uint32_t PERI_CLK_CTRL;                 /*!< Offset: 0x110 PERI_CLK_CTRL                                */
    __IOM uint32_t PERI_CLK_GATE;                 /*!< Offset: 0x114 PERI_CLK_GATE                                */
    __IOM uint32_t PERI_CLK_MUX;                  /*!< Offset: 0x118 PERI_CLK_MUX                                 */
    uint32_t RESERVED4;
    __IOM uint32_t AFE_CLK_CTRL;                  /*!< Offset: 0x120 AFE_CLK_CTRL                                 */
    uint32_t RESERVED8[55];
    __IOM uint32_t PMU_SYSCON_PD_CTRL;            /*!< Offset: 0x200 PMU_SYSCON_PD_CTRL                           */
    __IM  uint32_t PMU_SYSCON_PD_STATUS;          /*!< Offset: 0x204 PMU_SYSCON_PD_STATUS                         */
    __IOM uint32_t PMU_SYSCON_RET_CTRL;           /*!< Offset: 0x208 PMU_SYSCON_RET_CTRL                          */
    __IOM uint32_t SYSCON_FLASH_MODE;             /*!< Offset: 0x20C SYSCON_FLASH_MODE                            */
    uint32_t RESERVED9[124];
    __IOM uint32_t IOCFG_GENERAL;                 /*!< Offset: 0x400 SYSCON_IOCFGG                                */
    __IOM uint32_t IOCFG[14];                     /*!< Offset: 0x404 SYSCON_IOCFG                                 */
    uint32_t RESERVED10[113];
    __IOM uint32_t PLL_CTRL0;                     /*!< Offset: 0x600 PLL_CTRL0                                    */
    __IOM uint32_t PLL_CTRL1;                     /*!< Offset: 0x604 PLL_CTRL1                                    */
    __IM  uint32_t PLL_STATUS;                    /*!< Offset: 0x608 PLL_STATUS                                   */
    __IOM uint32_t HSOSC_CTRL;                    /*!< Offset: 0x60C HSOSC_CTRL                                   */
    __IOM uint32_t LSOSC_CTRL;                    /*!< Offset: 0x610 LSOSC_CTRL                                   */
    uint32_t RESERVED11[123];
    __IOM uint32_t SRP_IRQEVE;                    /*!< Offset: 0x800 SRP_IRQEVE                                   */
    __IM  uint32_t SRP_STATUS;                    /*!< Offset: 0x804 SRP_STATUS                                   */
    uint32_t RESERVED12[62];
    __IOM uint32_t SYSCON_IO;                     /*!< Offset: 0x900 SYSCON_IO                                    */
    __IM  uint32_t SYSCON_WAKEUP_STATUS;          /*!< Offset: 0x904 SYSCON_WAKEUP_STATUS                         */
    __IOM uint32_t SYSCON_WAKEUP_STATUS_CLEAR;    /*!< Offset: 0x908 SYSCON_WAKEUP_STATUS_CLEAR                   */
    __IM  uint32_t QSPI_MONITOR1;                 /*!< Offset: 0x90C QSPI_MONITOR1                                */
    __IM  uint32_t QSPI_MONITOR2;                 /*!< Offset: 0x910 QSPI_MONITOR2                                */
    __IOM uint32_t KA_ACCESS;                     /*!< Offset: 0x914 KA_ACCESS                                    */
} BP_SYSCON_TypeDef;

/*---------- Sample Rate Converter (SRC) -------------------------------------*/

typedef struct {
    __OM  uint32_t CTRL;                  /*!< Offset: 0x000 Control Register                                     */
    __IOM uint32_t CFG;                   /*!< Offset: 0x004 Configuration Register                               */
    __IM  uint32_t STAT;                  /*!< Offset: 0x008 Status Register                                      */
    __IOM uint32_t fSTART;                /*!< Offset: 0x00C Start Position(float) Register                       */
    __IOM uint32_t fINC;                  /*!< Offset: 0x010 Increment Value(float) Register                      */
    __IOM uint32_t PB_PTR;                /*!< Offset: 0x014 Preceding Buffer Pointer Register                    */
    __IOM uint32_t PB_SIZE;               /*!< Offset: 0x018 Preceding Buffer Size Register                       */
    __IOM uint32_t SB_PTR;                /*!< Offset: 0x01C Sample Buffer Pointer Register                       */
    __IOM uint32_t SB_SIZE;               /*!< Offset: 0x020 Sample Buffer Size Register                          */
    __IOM uint32_t CB_PTR;                /*!< Offset: 0x024 Coefficient Buffer Pointer Register                  */
    __IOM uint32_t CB_SIZE;               /*!< Offset: 0x028 Coefficient Buffer Size Register                     */
    __IOM uint32_t RB_PTR;                /*!< Offset: 0x02C Result Buffer Pointer Register                       */
    __IOM uint32_t RB_SIZE;               /*!< Offset: 0x030 Result Buffer Size Register                          */
    __IOM uint32_t IN_SH;                 /*!< Offset: 0x034 Shift(in) Register                                   */
    __IOM uint32_t OUT_SH;                /*!< Offset: 0x038 Shift(out) Register                                  */
    __IOM uint32_t INT;                   /*!< Offset: 0x03C SRC Interrupt Register                               */
} BP_SRC_TypeDef;

/*---------- General Purpose Input Output (GPIO) -----------------------------*/

typedef struct {
    __IOM uint32_t DATA;                  /*!< Offset: 0x000 DATA Register                                        */
    __IOM uint32_t DATAOUT;               /*!< Offset: 0x004 Data Output Latch Register                           */
    uint32_t RESERVED1[2];
    __IOM uint32_t OUTENSET;              /*!< Offset: 0x010 Output Enable Set Register                           */
    __IOM uint32_t OUTENCLR;              /*!< Offset: 0x014 Output Enable Clear Register                         */
    __IOM uint32_t ALTFUNCSET;            /*!< Offset: 0x018 Alternate Function Set Register                      */
    __IOM uint32_t ALTFUNCCLR;            /*!< Offset: 0x01C Alternate Function Clear Register                    */
    __IOM uint32_t INTENSET;              /*!< Offset: 0x020 Interrupt Enable Set Register                        */
    __IOM uint32_t INTENCLR;              /*!< Offset: 0x024 Interrupt Enable Clear Register                      */
    __IOM uint32_t INTTYPESET;            /*!< Offset: 0x028 Interrupt Type Set Register                          */
    __IOM uint32_t INTTYPECLR;            /*!< Offset: 0x02C Interrupt Type Clear Register                        */
    __IOM uint32_t INTPOLSET;             /*!< Offset: 0x030 Interrupt Polarity Set Register                      */
    __IOM uint32_t INTPOLCLR;             /*!< Offset: 0x034 Interrupt Polarity Clear Register                    */
    union {
        __IM  uint32_t INTSTAT;               /*!< Offset: 0x038 Interrupt Status Register                            */
        __OM  uint32_t INTCLR;                /*!< Offset: 0x038 Interrupt Clear Register                             */
    };
    __IOM uint32_t ALTFUNCSEL;            /*!< Offset: 0x03C Alternate Function Set Register                      */
    uint32_t RESERVED2[240];
    __IOM uint32_t LB_MASKED[256];        /*!< Offset: 0x400 - 0x7FC Lower byte Masked Access Register            */
    __IOM uint32_t UB_MASKED[256];        /*!< Offset: 0x800 - 0xBFC Upper byte Masked Access Register            */
} BP_GPIO_TypeDef;

/*---------- Advanced Encryption Standard (AES) ------------------------------*/
/** @addtogroup BP_AES S1SBP6A Advanced Encryption Standard
  @{
*/
typedef struct {
    __IOM uint32_t AESCON;                /*!< Offset: 0x000 AES Control                                          */
    __IOM uint32_t AESSTATUS;             /*!< Offset: 0x004 AES Status                                           */
    uint32_t RESERVED1[2];
    __IOM uint32_t AESINDATA1;            /*!< Offset: 0x010 AES Input Data[31:0]                                 */
    __IOM uint32_t AESINDATA2;            /*!< Offset: 0x014 AES Input Data[63:32]                                */
    __IOM uint32_t AESINDATA3;            /*!< Offset: 0x018 AES Input Data[95:64]                                */
    __IOM uint32_t AESINDATA4;            /*!< Offset: 0x01C AES Input Data[127:96]                               */
    __IM  uint32_t AESOUTDATA1;           /*!< Offset: 0x020 AES Output Data[31:0]                                */
    __IM  uint32_t AESOUTDATA2;           /*!< Offset: 0x024 AES Output Data[63:32]                               */
    __IM  uint32_t AESOUTDATA3;           /*!< Offset: 0x028 AES Output Data[95:64]                               */
    __IM  uint32_t AESOUTDATA4;           /*!< Offset: 0x02C AES Output Data[127:96]                              */
    uint32_t RESERVED2[8];
    __IOM uint32_t AESIV1;                /*!< Offset: 0x050 AES IV[31:0]                                         */
    __IOM uint32_t AESIV2;                /*!< Offset: 0x054 AES IV[63:32]                                        */
    __IOM uint32_t AESIV3;                /*!< Offset: 0x058 AES IV[95:64]                                        */
    __IOM uint32_t AESIV4;                /*!< Offset: 0x05C AES IV[127:96]                                       */
    uint32_t RESERVED3[8];
    __OM  uint32_t AESKEY1;               /*!< Offset: 0x080 AES KEY[31:0]                                        */
    __OM  uint32_t AESKEY2;               /*!< Offset: 0x084 AES KEY[63:32]                                       */
    __OM  uint32_t AESKEY3;               /*!< Offset: 0x088 AES KEY[95:64]                                       */
    __OM  uint32_t AESKEY4;               /*!< Offset: 0x08C AES KEY[127:96]                                      */
    __OM  uint32_t AESKEY5;               /*!< Offset: 0x090 AES KEY[159:128]                                     */
    __OM  uint32_t AESKEY6;               /*!< Offset: 0x094 AES KEY[191:160]                                     */
    __OM  uint32_t AESKEY7;               /*!< Offset: 0x098 AES KEY[223:192]                                     */
    __OM  uint32_t AESKEY8;               /*!< Offset: 0x09C AES KEY[255:224]                                     */
} BP_AES_TypeDef;

/*---------- True Random Number Generator (DTRNG) ----------------------------*/
typedef struct {
    __IOM uint32_t CLKDIV;                /*!< Offset: 0x000 DTRNG Clock Divider                                  */
    __IOM uint32_t CTRL;                  /*!< Offset: 0x004 DTRNG Control                                        */
    __IOM uint32_t POST_CTRL;             /*!< Offset: 0x008 DTRNG Post Processor Control                         */
    __IOM uint32_t FIFO_CTRL;             /*!< Offset: 0x00C DTRNG FIFO Control                                   */
    __IOM uint32_t FIFO_0;                /*!< Offset: 0x010 DTRNG FIFO                                           */
} BP_DTRNG_TypeDef;

/*------------- Flash Control (FLASH) ----------------------------------------*/

typedef struct {
    __IOM uint32_t FRWCON0;               /*!< Offset: 0x000 Flash R/W Control Register                           */
    __IOM uint32_t FSHSTAT;               /*!< Offset: 0x004 Flash Status Register                                */
    uint32_t RESERVED1[2];
    __IOM uint32_t PROTCON;               /*!< Offset: 0x010 Protection Control Register                          */
    __IM  uint32_t PROTMON;               /*!< Offset: 0x014 Protection Monitoring Register                       */
    uint32_t RESERVED2[2];
    __IOM uint32_t DCYCRDCON;             /*!< Offset: 0x020 DCYCREAD Control Register                            */
    uint32_t RESERVED3[3];
    __IOM uint32_t LDTCON;                /*!< Offset: 0x030 LDTCON Register for Test Mode Only                   */
    __IOM uint32_t FSHCON0;               /*!< Offset: 0x034 Flash Test Register #1                               */
    __IOM uint32_t FECCCON;               /*!< Offset: 0x038 Flash Test Register #2                               */
    __IM  uint32_t FECCFADR;              /*!< Offset: 0x03C ECC Fault Address                                    */
    __IOM uint32_t DCT0;                  /*!< Offset: 0x040 DCT0 Option Register                                 */
    __IOM uint32_t DCT1;                  /*!< Offset: 0x044 DCT1 Option Register                                 */
    uint32_t RESERVED4[2];
    __IOM uint32_t LDT0;                  /*!< Offset: 0x050 LDT0 Option Register                                 */
    __IOM uint32_t LDT1;                  /*!< Offset: 0x054 LDT1 Option Register                                 */
    __IOM uint32_t LDT2;                  /*!< Offset: 0x058 LDT2 Option Register                                 */
    __IOM uint32_t LDT3;                  /*!< Offset: 0x05C LDT3 Option Register                                 */
    __IM  uint32_t MODE;                  /*!< Offset: 0x060 MODECELL Option Register                             */
    __IM  uint32_t CPA0;                  /*!< Offset: 0x064 CPA0 Option Register                                 */
    __IM  uint32_t BPU;                   /*!< Offset: 0x068 BPU Option Register                                  */
    uint32_t RESERVED5[9];
    __IOM uint32_t RED0;                  /*!< Offset: 0x090 RED0 Option Register                                 */
    __IOM uint32_t RED0_MBIT;             /*!< Offset: 0x094 RED0 User Mode Register                              */
    __IOM uint32_t RED1;                  /*!< Offset: 0x098 RED1 Option Register                                 */
    __IOM uint32_t RED1_MBIT;             /*!< Offset: 0x09C RED1 User Mode Register                              */
    __IOM uint32_t RED2;                  /*!< Offset: 0x0A0 RED2 Option Register                                 */
    __IOM uint32_t RED2_MBIT;             /*!< Offset: 0x0A4 RED2 User Mode Register                              */
    __IOM uint32_t RED3;                  /*!< Offset: 0x0A8 RED3 Option Register                                 */
    __IOM uint32_t RED3_MBIT;             /*!< Offset: 0x0AC RED3 User Mode Register                              */
} BP_FLASH_TypeDef;

/*------------- System Control (SYSCTRL) -------------------------------------*/
typedef struct {
    uint32_t RESERVED1;
    __IOM uint32_t PMUCTRL;               /*!< Offset: 0x004 Control PMU feature of system                        */
    __IOM uint32_t RESETOP;               /*!< Offset: 0x008 Determine reset function at LOCKUP state             */
    uint32_t RESERVED2;
    __IOM uint32_t RSTINFO;               /*!< Offset: 0x010 Reset information of system                          */
} BP_SYSCTRL_TypeDef;

/*------------- Serial Flash Control (SFLASH) --------------------------------------*/
typedef struct {
    __IOM uint32_t CALL;                  /*!< Offset: 0x000 Call Register                                        */
    __IOM uint32_t SFCON0;                /*!< Offset: 0x004 Control Register                                     */
    __IOM uint32_t ARM_MODE;              /*!< Offset: 0x008 ARM Mode Register                                    */
    __IOM uint32_t THUMB_MODE;            /*!< Offset: 0x00C Thumb Mode Register                                  */
    __IOM uint32_t ERASE_ADDRESS;         /*!< Offset: 0x010 Erase Address Register                               */
    __IOM uint32_t PROT_ADDRESS;          /*!< Offset: 0x014 Protect Address Register                             */
    uint32_t RESERVED0;
    __IOM uint32_t CMD0;                  /*!< Offset: 0x01C Command 0 Register                                   */
    __IOM uint32_t CMD1;                  /*!< Offset: 0x020 Command 1 Register                                   */
    __IOM uint32_t CMD2;                  /*!< Offset: 0x024 Command 2 Register                                   */
    __IOM uint32_t CMD3;                  /*!< Offset: 0x028 Command 3 Register                                   */
    __IOM uint32_t CMD4;                  /*!< Offset: 0x02C Command 4 Register                                   */
    uint32_t RESERVED1[19];
    __IOM uint32_t CMD5;                  /*!< Offset: 0x07C (R/W)  SFLASH Command 5 Register                     */
} BP_SFLASH_TypeDef;

/*----- Samsung Reconfigurable Processor Special Function Register (SlimSRP) -----*/

typedef struct {
    __IM  uint32_t PROC_ID;               /*!< Offset: 0x0000 Processor ID                                        */
    uint32_t RESERVED1[63];
    __IM  uint32_t PROC_STAT;             /*!< Offset: 0x0100 Processor Status                                    */
    __OM  uint32_t PROC_PAUSE;            /*!< Offset: 0x0104 Processor Pause                                     */
    uint32_t RESERVED2[961];
    __IOM uint32_t TCM_INIT;              /*!< Offset: 0x100C TCM_INIT                                            */
    __IOM uint32_t SRP_INTR;              /*!< Offset: 0x1010 SRP_INTR                                            */
    __IOM uint32_t SRP_RESULT;            /*!< Offset: 0x1014 SRP_RESULT                                          */
    __IOM uint32_t SRP_PARAM0;            /*!< Offset: 0x1018 SRP_PARAM0                                          */
    __IOM uint32_t SRP_PARAM1;            /*!< Offset: 0x101C SRP_PARAM1                                          */
    __IOM uint32_t SRP_PARAM2;            /*!< Offset: 0x1020 SRP_PARAM2                                          */
    __IOM uint32_t SRP_PARAM3;            /*!< Offset: 0x1024 SRP_PARAM3                                          */
    uint32_t RESERVED3[5110];
    __IOM uint32_t CNT_CFG;               /*!< Offset: 0x6000 CNT_CFG                                             */
    uint32_t RESERVED4[3];
    __IOM uint32_t CNT_FR_L;              /*!< Offset: 0x6010 CNT_FR_L                                            */
    __IOM uint32_t CNT_FR_H;              /*!< Offset: 0x6014 CNT_FR_H                                            */
    __IOM uint32_t CNT_ST_L;              /*!< Offset: 0x6018 CNT_ST_L                                            */
    __IOM uint32_t CNT_ST_H;              /*!< Offset: 0x601C CNT_ST_H                                            */
    uint32_t RESERVED5[1016];
    __IM  uint32_t SRP_PC;                /*!< Offset: 0x7000 SRP_PC                                              */
} BP_SRP_TypeDef;


#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/

/* Peripheral, Flash, Serial Flash and SRAM base address */
#define BP_FLASH_BASE           (0x00000000UL)                  /*!< (Flash                  ) Base Address */
#define BP_SRAM_BASE            (0x20000000UL)                  /*!< (SRAM                   ) Base Address */
#define BP_APB_BASE             (0x40000000UL)                  /*!< (APB                    ) Base Address */
#define BP_AHB_BASE             (0x40020000UL)                  /*!< (AHB                    ) Base Address */
#define BP_SFLASH_BASE          (0x60000000UL)                  /*!< (Serial Flash           ) Base Address */
#define BP_SRP_DSPM_BASE        (0x80000000UL)                  /*!< (SRP DSPM               ) Base Address */
#define BP_SRP_TDM_BASE         (0x80070000UL)                  /*!< (SRP TCM                ) Base Address */

/* Peripheral memory map */
#define BP_TIMER0_BASE          (BP_APB_BASE + 0x00000UL)       /*!< (Timer0                 ) Base Address */
#define BP_TIMER1_BASE          (BP_APB_BASE + 0x01000UL)       /*!< (Timer1                 ) Base Address */
#define BP_TIMER2_BASE          (BP_APB_BASE + 0x02000UL)       /*!< (Timer2                 ) Base Address */
#define BP_TIMER3_BASE          (BP_APB_BASE + 0x03000UL)       /*!< (Timer3                 ) Base Address */
#define BP_TIMER4_BASE          (BP_APB_BASE + 0x04000UL)       /*!< (Timer4                 ) Base Address */
#define BP_TIMER5_BASE          (BP_APB_BASE + 0x05000UL)       /*!< (Timer5                 ) Base Address */
#define BP_TIMER6_BASE          (BP_APB_BASE + 0x06000UL)       /*!< (Timer6                 ) Base Address */
#define BP_TIMER7_BASE          (BP_APB_BASE + 0x07000UL)       /*!< (Timer7                 ) Base Address */
#define BP_PWM_TIMER_BASE       (BP_APB_BASE + 0x07000UL)       /*!< (PWM Timer              ) Base Address */
#define BP_DUAL_TIMER0_BASE     (BP_APB_BASE + 0x08000UL)       /*!< (Dual Timer0            ) Base Address */
#define BP_DUAL_TIMER1_BASE     (BP_APB_BASE + 0x08020UL)       /*!< (Dual Timer1            ) Base Address */
#define BP_WATCHDOG_BASE        (BP_APB_BASE + 0x09000UL)       /*!< (Watchdog Timer         ) Base Address */
#define BP_UART0_BASE           (BP_APB_BASE + 0x0A000UL)       /*!< (UART0                  ) Base Address */
#define BP_UART1_BASE           (BP_APB_BASE + 0x0B000UL)       /*!< (UART1                  ) Base Address */
#define BP_UART2_BASE           (BP_APB_BASE + 0x0C000UL)       /*!< (UART2                  ) Base Address */
#define BP_SPI0_BASE            (BP_APB_BASE + 0x0D000UL)       /*!< (SPI0                   ) Base Address */
#define BP_SPI1_BASE            (BP_APB_BASE + 0x0E000UL)       /*!< (SPI1                   ) Base Address */
#define BP_SPI2_BASE            (BP_APB_BASE + 0x0F000UL)       /*!< (SPI2                   ) Base Address */
#define BP_SPI3_BASE            (BP_APB_BASE + 0x10000UL)       /*!< (SPI3                   ) Base Address */
#define BP_SPI4_BASE            (BP_APB_BASE + 0x11000UL)       /*!< (SPI4                   ) Base Address */
#define BP_I2C0_BASE            (BP_APB_BASE + 0x12000UL)       /*!< (I2C0                   ) Base Address */
#define BP_I2C1_BASE            (BP_APB_BASE + 0x13000UL)       /*!< (I2C1                   ) Base Address */
#define BP_I2C2_BASE            (BP_APB_BASE + 0x14000UL)       /*!< (I2C2                   ) Base Address */
#define BP_I2C3_BASE            (BP_APB_BASE + 0x15000UL)       /*!< (I2C3                   ) Base Address */
#define BP_I2C4_BASE            (BP_APB_BASE + 0x16000UL)       /*!< (I2C4                   ) Base Address */
#define BP_DELAY_MON_BASE       (BP_APB_BASE + 0x17000UL)       /*!< (Delay Monitor          ) Base Address */
#define BP_RTC_BASE             (BP_APB_BASE + 0x19000UL)       /*!< (Real-Time Counter      ) Base Address */
#define BP_AFE_BASE             (BP_APB_BASE + 0x1A000UL)       /*!< (AFE TOP                ) Base Address */
#define BP_DMA_BASE             (BP_APB_BASE + 0x1B000UL)       /*!< (DMA                    ) Base Address */
#define BP_SYSCON_BASE          (BP_APB_BASE + 0x1C000UL)       /*!< (System Control         ) Base Address */
#define BP_SRC_BASE             (BP_APB_BASE + 0x1D000UL)       /*!< (Sample Rate Converter  ) Base Address */
#define BP_GPIO0_BASE           (BP_AHB_BASE + 0x00000UL)       /*!< (GPIO0                  ) Base Address */
#define BP_GPIO1_BASE           (BP_AHB_BASE + 0x01000UL)       /*!< (GPIO1                  ) Base Address */
#define BP_GPIO2_BASE           (BP_AHB_BASE + 0x02000UL)       /*!< (GPIO2                  ) Base Address */
#define BP_AES_BASE             (BP_AHB_BASE + 0x03000UL)       /*!< (AES                    ) Base Address */
#define BP_DTRNG_BASE           (BP_AHB_BASE + 0x04000UL)       /*!< (DTRNG                  ) Base Address */
#define BP_FLASH_CTRL_BASE      (BP_AHB_BASE + 0x05000UL)       /*!< (Flash Controller       ) Base Address */
#define BP_SYSCTRL_BASE         (BP_AHB_BASE + 0x0F000UL)       /*!< (System Controller      ) Base Address */
#define BP_SFLASH_CTRL_BASE     (0x70000000UL)                  /*!< (Serial Flash Controller) Base Address */
#define BP_SRP_SFR_BASE         (0x80078000UL)                  /*!< (SRP SFR                ) Base Address */

#define BP_AFE_TOP_BASE         (BP_AFE_BASE + 0x000UL)         /*!< (AFE TOP                ) Base Address */
#define BP_AFE_ECG_BASE         (BP_AFE_BASE + 0x100UL)         /*!< (AFE ECG                ) Base Address */
#define BP_AFE_PPG_BASE         (BP_AFE_BASE + 0x200UL)         /*!< (AFE PPG                ) Base Address */
#define BP_AFE_BIA_BASE         (BP_AFE_BASE + 0x300UL)         /*!< (AFE BIA                ) Base Address */
#define BP_AFE_GSR_BASE         (BP_AFE_BASE + 0x400UL)         /*!< (AFE GSR                ) Base Address */
#define BP_ADC_FIFO_BASE         (BP_AFE_BASE + 0x500UL)         /*!< (ADC_FIFO                ) Base Address */
#define BP_ADC_CON_BASE         (BP_AFE_BASE + 0x600UL)         /*!< (ADC_CON                ) Base Address */
#define BP_SDADC_CON_BASE       (BP_AFE_BASE + 0x700UL)         /*!< (SDADC_CON         ) Base Address */

/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/

#define BP_TIMER0               ((BP_TIMER_TypeDef      *) BP_TIMER0_BASE)
#define BP_TIMER1               ((BP_TIMER_TypeDef      *) BP_TIMER1_BASE)
#define BP_TIMER2               ((BP_TIMER_TypeDef      *) BP_TIMER2_BASE)
#define BP_TIMER3               ((BP_TIMER_TypeDef      *) BP_TIMER3_BASE)
#define BP_TIMER4               ((BP_TIMER_TypeDef      *) BP_TIMER4_BASE)
#define BP_TIMER5               ((BP_TIMER_TypeDef      *) BP_TIMER5_BASE)
#define BP_TIMER6               ((BP_TIMER_TypeDef      *) BP_TIMER6_BASE)
#define BP_TIMER7               ((BP_TIMER_TypeDef      *) BP_TIMER7_BASE)
#define BP_PWM_TIMER            ((BP_PWM_TIMER_TypeDef  *) BP_PWM_TIMER_BASE)
#define BP_DUAL_TIMER0          ((BP_DUAL_TIMER_TypeDef *) BP_DUAL_TIMER0_BASE)
#define BP_DUAL_TIMER1          ((BP_DUAL_TIMER_TypeDef *) BP_DUAL_TIMER1_BASE)
#define BP_WATCHDOG             ((BP_WATCHDOG_TypeDef   *) BP_WATCHDOG_BASE)
#define BP_UART0                ((BP_UART_TypeDef       *) BP_UART0_BASE)
#define BP_UART1                ((BP_UART_TypeDef       *) BP_UART1_BASE)
#define BP_UART2                ((BP_UART_TypeDef       *) BP_UART2_BASE)
#define BP_SPI0                 ((BP_SPI_TypeDef        *) BP_SPI0_BASE)
#define BP_SPI1                 ((BP_SPI_TypeDef        *) BP_SPI1_BASE)
#define BP_SPI2                 ((BP_SPI_TypeDef        *) BP_SPI2_BASE)
#define BP_SPI3                 ((BP_SPI_TypeDef        *) BP_SPI3_BASE)
#define BP_SPI4                 ((BP_SPI_TypeDef        *) BP_SPI4_BASE)
#define BP_I2C0                 ((BP_I2C_TypeDef        *) BP_I2C0_BASE)
#define BP_I2C1                 ((BP_I2C_TypeDef        *) BP_I2C1_BASE)
#define BP_I2C2                 ((BP_I2C_TypeDef        *) BP_I2C2_BASE)
#define BP_I2C3                 ((BP_I2C_TypeDef        *) BP_I2C3_BASE)
#define BP_I2C4                 ((BP_I2C_TypeDef        *) BP_I2C4_BASE)
#define BP_DELAY_MON            ((BP_DELAY_MON_TypeDef  *) BP_DELAY_MON_BASE)
#define BP_RTC                  ((BP_RTC_TypeDef        *) BP_RTC_BASE)
#define BP_AFE_TOP              ((BP_AFE_TOP_TypeDef    *) BP_AFE_TOP_BASE)
#define BP_AFE_ECG              ((BP_AFE_ECG_TypeDef    *) BP_AFE_ECG_BASE)
#define BP_AFE_PPG              ((BP_AFE_PPG_TypeDef    *) BP_AFE_PPG_BASE)
#define BP_AFE_BIA              ((BP_AFE_BIA_TypeDef    *) BP_AFE_BIA_BASE)
#define BP_AFE_GSR              ((BP_AFE_GSR_TypeDef    *) BP_AFE_GSR_BASE)
#define BP_ADC_FIFO             ((BP_ADC_FIFO_TypeDef   *) BP_ADC_FIFO_BASE)
#define BP_ADC_CON              ((BP_ADC_CON_TypeDef    *) BP_ADC_CON_BASE)
#define BP_SDADC_CON            ((BP_SDADC_CON_TypeDef  *) BP_SDADC_CON_BASE)
#define BP_DMA                  ((BP_DMA_TypeDef        *) BP_DMA_BASE)
#define BP_SYSCON               ((BP_SYSCON_TypeDef     *) BP_SYSCON_BASE)
#define BP_SRC                  ((BP_SRC_TypeDef        *) BP_SRC_BASE)
#define BP_GPIO0                ((BP_GPIO_TypeDef       *) BP_GPIO0_BASE)
#define BP_GPIO1                ((BP_GPIO_TypeDef       *) BP_GPIO1_BASE)
#define BP_GPIO2                ((BP_GPIO_TypeDef       *) BP_GPIO2_BASE)
#define BP_AES                  ((BP_AES_TypeDef        *) BP_AES_BASE)
#define BP_DTRNG                ((BP_DTRNG_TypeDef      *) BP_DTRNG_BASE)
#define BP_FLASH                ((BP_FLASH_TypeDef      *) BP_FLASH_CTRL_BASE)
#define BP_SYSCTRL              ((BP_SYSCTRL_TypeDef    *) BP_SYSCTRL_BASE)
#define BP_SFLASH               ((BP_SFLASH_TypeDef     *) BP_SFLASH_CTRL_BASE)
#define BP_SRP                  ((BP_SRP_TypeDef        *) BP_SRP_SFR_BASE)

#define BP_SFLASH_ADDR_CMD      ((uint8_t *) BP_SFLASH_CTRL_BASE + 0x59UL)
#define BP_SFALSH_SECTOR_ERASE  ((uint8_t *) BP_SFLASH_CTRL_BASE + 0x5EUL)
#define BP_SFALSH_MODE          ((uint8_t *) BP_SFLASH_CTRL_BASE + 0x74UL)
#define BP_SFALSH_RDID          ((uint8_t *) BP_SFLASH_CTRL_BASE + 0xACUL)
#define BP_SFALSH_BLOCK_ERASE   ((uint8_t *) BP_SFLASH_CTRL_BASE + 0xBEUL)
#define BP_SFALSH_CHIP_ERASE    ((uint8_t *) BP_SFLASH_CTRL_BASE + 0xCEUL)
#define BP_SFALSH_RDSR          ((uint8_t *) BP_SFLASH_CTRL_BASE + 0xDCUL)
#define BP_SFLASH_WRDI          ((uint8_t *) BP_SFLASH_CTRL_BASE + 0xDDUL)
#define BP_SFALSH_WRSR          ((uint8_t *) BP_SFLASH_CTRL_BASE + 0xDEUL)
#define BP_SFALSH_WREN          ((uint8_t *) BP_SFLASH_CTRL_BASE + 0xEEUL)
#define BP_SFALSH_PROT_DIS      ((uint8_t *) BP_SFLASH_CTRL_BASE + 0xF0UL)
#define BP_SFALSH_PROT_EN       ((uint8_t *) BP_SFLASH_CTRL_BASE + 0xF1UL)

#endif  /* __S1SBP6A_H */
