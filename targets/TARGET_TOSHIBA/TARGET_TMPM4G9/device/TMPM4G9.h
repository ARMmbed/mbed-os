/**
 *******************************************************************************
 * @file    TMPM4G9.h
 * @brief   CMSIS Cortex-M4 Core Peripheral Access Layer Header File for the
 *          TOSHIBA 'TMPM4G9' Device Series
 * @version V1.0.9.0
 * $Date:: 2018-04-02 #$
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright(C) 2019, Toshiba Electronic Device Solutions Corporation
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of Nuvoton Technology Corp. nor the names of its contributors
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
 *******************************************************************************
 */
/** @addtogroup TOSHIBA_TXZ_MICROCONTROLLER
  * @{
  */
  
/** @addtogroup TMPM4G9
  * @{
  */

#ifndef __TMPM4G9_H__
#define __TMPM4G9_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Configuration_of_CMSIS
  * @{
  */

/** Interrupt Number Definition */
typedef enum IRQn
{
/******  Cortex-M4 Processor Exceptions Numbers ***************************************************************/
  NonMaskableInt_IRQn           = -14,       /*!< 2 Non Maskable Interrupt                                    */
  HardFault_IRQn                = -13,       /*!< 3 Cortex-M4 Hard Fault Interrupt                            */
  MemoryManagement_IRQn         = -12,       /*!< 4 Cortex-M4 Memory Management Interrupt                     */
  BusFault_IRQn                 = -11,       /*!< 5 Cortex-M4 Bus Fault Interrupt                             */
  UsageFault_IRQn               = -10,       /*!< 6 Cortex-M4 Usage Fault Interrupt                           */
  SVCall_IRQn                   = -5,        /*!< 11 Cortex-M4 SV Call Interrupt                              */
  DebugMonitor_IRQn             = -4,        /*!< 12 Cortex-M4 Debug Monitor Interrupt                        */
  PendSV_IRQn                   = -2,        /*!< 14 Cortex-M4 Pend SV Interrupt                              */
  SysTick_IRQn                  = -1,        /*!< 15 Cortex-M4 System Tick Interrupt                          */

/******  TMPM4G9 Specific Interrupt Numbers *******************************************************************/
  INT00_IRQn                    = 0,         /*!< Interrupt pin 00a/00b                                       */
  INT01_IRQn                    = 1,         /*!< Interrupt pin 01a/00b                                       */
  INT02_IRQn                    = 2,         /*!< Interrupt pin 02a/00b                                       */
  INT03_IRQn                    = 3,         /*!< Interrupt pin 03a/03b                                       */
  INT04_IRQn                    = 4,         /*!< Interrupt pin 04a/04b                                       */
  INT05_IRQn                    = 5,         /*!< Interrupt pin 05a/05b                                       */
  INT06_IRQn                    = 6,         /*!< Interrupt pin 06a/06b                                       */
  INT07_IRQn                    = 7,         /*!< Interrupt pin 07a/07b                                       */
  INT08_IRQn                    = 8,         /*!< Interrupt pin 08a/08b                                       */
  INT09_IRQn                    = 9,         /*!< Interrupt pin 09a/09b                                       */
  INT10_IRQn                    = 10,        /*!< Interrupt pin 10a/10b                                       */
  INT11_IRQn                    = 11,        /*!< Interrupt pin 11a/11b                                       */
  INT12_IRQn                    = 12,        /*!< Interrupt pin 12a/12b                                       */
  INT13_IRQn                    = 13,        /*!< Interrupt pin 13a/13b                                       */
  INT14_IRQn                    = 14,        /*!< Interrupt pin 14a/14b                                       */
  INT15_IRQn                    = 15,        /*!< Interrupt pin 15a/15b                                       */
  INTRTC_IRQn                   = 16,        /*!< Real time clock(XHz) interrupt                              */
  INTCEC0RX_IRQn                = 17,        /*!< CEC reception interrupt (channel 0)                         */
  INTCEC0TX_IRQn                = 18,        /*!< CEC transmission interrupt (channel 0)                      */
  INTISDA_IRQn                  = 19,        /*!< Interval Sensing Detector Interrupt (Unit A)                */
  INTISDB_IRQn                  = 20,        /*!< Interval Sensing Detector Interrupt (Unit B)                */
  INTISDC_IRQn                  = 21,        /*!< Interval Sensing Detector Interrupt (Unit C)                */
  INTRMC0_IRQn                  = 22,        /*!< Remote control reception interrupt 0                        */
  INTRMC1_IRQn                  = 23,        /*!< Remote control reception interrupt 1                        */
  INTLTTMR0_IRQn                = 24,        /*!< Long Term Timer Interrupt(channel 0)                        */
  INTHDMAATC_IRQn               = 25,        /*!< HDMA Complete of transfer(Unit A)                           */
  INTHDMAAERR_IRQn              = 26,        /*!< HDMA transfer error(Unit A)                                 */
  INTHDMABTC_IRQn               = 27,        /*!< HDMA end of transfer(Unit B)                                */
  INTHDMABERR_IRQn              = 28,        /*!< HDMA transfer error(Unit B)                                 */
  INTMDMAATC_IRQn               = 29,        /*!< MDMA Complete of transfer(Unit A)                           */
  INTT32A00_A_CT_IRQn           = 30,        /*!< T32A00 TimerA All Interrupt/Timer Interrupt C               */
  INTT32A00_B_C01_CPC_IRQn      = 31,        /*!< T32A00 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C*/
  INTT32A01_A_CT_IRQn           = 32,        /*!< T32A01 TimerA All Interrupt/Timer Interrupt C               */
  INTT32A01_B_C01_CPC_IRQn      = 33,        /*!< T32A01 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C*/
  INTT32A02_A_CT_IRQn           = 34,        /*!< T32A02 TimerA All Interrupt/Timer Interrupt C               */
  INTT32A02_B_C01_CPC_IRQn      = 35,        /*!< T32A02 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C*/
  INTT32A03_A_CT_IRQn           = 36,        /*!< T32A03 TimerA All Interrupt/Timer Interrupt C               */
  INTT32A03_B_C01_CPC_IRQn      = 37,        /*!< T32A03 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C*/
  INTT32A04_A_CT_IRQn           = 38,        /*!< T32A04 TimerA All Interrupt/Timer Interrupt C               */
  INTT32A04_B_C01_CPC_IRQn      = 39,        /*!< T32A04 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C*/
  INTT32A05_A_CT_IRQn           = 40,        /*!< T32A05 TimerA All Interrupt/Timer Interrupt C               */
  INTT32A05_B_C01_CPC_IRQn      = 41,        /*!< T32A05 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C*/
  INTT32A06_A_CT_IRQn           = 42,        /*!< T32A06 TimerA All Interrupt/Timer Interrupt C               */
  INTT32A06_B_C01_CPC_IRQn      = 43,        /*!< T32A06 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C*/
  INTT32A07_A_CT_IRQn           = 44,        /*!< T32A07 TimerA All Interrupt/Timer Interrupt C               */
  INTT32A07_B_C01_CPC_IRQn      = 45,        /*!< T32A07 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C*/
  INTT32A08_A_CT_IRQn           = 46,        /*!< T32A08 TimerA All Interrupt/Timer Interrupt C               */
  INTT32A08_B_C01_CPC_IRQn      = 47,        /*!< T32A08 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C*/
  INTT32A09_A_CT_IRQn           = 48,        /*!< T32A09 TimerA All Interrupt/Timer Interrupt C               */
  INTT32A09_B_C01_CPC_IRQn      = 49,        /*!< T32A09 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C*/
  INTT32A10_A_CT_IRQn           = 50,        /*!< T32A10 TimerA All Interrupt/Timer Interrupt C               */
  INTT32A10_B_C01_CPC_IRQn      = 51,        /*!< T32A10 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C*/
  INTT32A11_A_CT_IRQn           = 52,        /*!< T32A11 TimerA All Interrupt/Timer Interrupt C               */
  INTT32A11_B_C01_CPC_IRQn      = 53,        /*!< T32A11 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C*/
  INTT32A12_A_CT_IRQn           = 54,        /*!< T32A12 TimerA All Interrupt/Timer Interrupt C               */
  INTT32A12_B_C01_CPC_IRQn      = 55,        /*!< T32A12 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C*/
  INTT32A13_A_CT_IRQn           = 56,        /*!< T32A13 TimerA All Interrupt/Timer Interrupt C               */
  INTT32A13_B_C01_CPC_IRQn      = 57,        /*!< T32A13 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C*/
  INTEMG0_IRQn                  = 58,        /*!< PMD0 EMG interrupt                                          */
  INTOVV0_IRQn                  = 59,        /*!< PMD0 OVV interrupt                                          */
  INTPWM0_IRQn                  = 60,        /*!< PMD0 interrupt                                              */
  INTT0RX_IRQn                  = 61,        /*!< TSPI/SIO reception (channel 0)                              */
  INTT0TX_IRQn                  = 62,        /*!< TSPI/SIO transmit (channel 0)                               */
  INTT0ERR_IRQn                 = 63,        /*!< TSPI/SIO error (channel 0)                                  */
  INTT1RX_IRQn                  = 64,        /*!< TSPI/SIO reception (channel 1)                              */
  INTT1TX_IRQn                  = 65,        /*!< TSPI/SIO transmit (channel 1)                               */
  INTT1ERR_IRQn                 = 66,        /*!< TSPI/SIO error (channel 1)                                  */
  INTT2RX_IRQn                  = 67,        /*!< TSPI/SIO reception (channel 2)                              */
  INTT2TX_IRQn                  = 68,        /*!< TSPI/SIO transmit (channel 2)                               */
  INTT2ERR_IRQn                 = 69,        /*!< TSPI/SIO error (channel 2)                                  */
  INTT3RX_IRQn                  = 70,        /*!< TSPI/SIO reception (channel 3)                              */
  INTT3TX_IRQn                  = 71,        /*!< TSPI/SIO transmit (channel 3)                               */
  INTT3ERR_IRQn                 = 72,        /*!< TSPI/SIO error (channel 3)                                  */
  INTT4RX_IRQn                  = 73,        /*!< TSPI/SIO reception (channel 4)                              */
  INTT4TX_IRQn                  = 74,        /*!< TSPI/SIO transmit (channel 4)                               */
  INTT4ERR_IRQn                 = 75,        /*!< TSPI/SIO error (channel 4)                                  */
  INTT5RX_IRQn                  = 76,        /*!< TSPI/SIO reception (channel 5)                              */
  INTT5TX_IRQn                  = 77,        /*!< TSPI/SIO transmit (channel 5)                               */
  INTT5ERR_IRQn                 = 78,        /*!< TSPI/SIO error (channel 5)                                  */
  INTT6RX_IRQn                  = 79,        /*!< TSPI/SIO reception (channel 6)                              */
  INTT6TX_IRQn                  = 80,        /*!< TSPI/SIO transmit (channel 6)                               */
  INTT6ERR_IRQn                 = 81,        /*!< TSPI/SIO error (channel 6)                                  */
  INTT7RX_IRQn                  = 82,        /*!< TSPI/SIO reception (channel 7)                              */
  INTT7TX_IRQn                  = 83,        /*!< TSPI/SIO transmit (channel 7)                               */
  INTT7ERR_IRQn                 = 84,        /*!< TSPI/SIO error (channel 7)                                  */
  INTT8RX_IRQn                  = 85,        /*!< TSPI/SIO reception (channel 8)                              */
  INTT8TX_IRQn                  = 86,        /*!< TSPI/SIO transmit (channel 8)                               */
  INTT8ERR_IRQn                 = 87,        /*!< TSPI/SIO error (channel 8)                                  */
  INTSMI0_IRQn                  = 88,        /*!< Serial Memory Interface Interrupt                           */
  INTUART0RX_IRQn               = 89,        /*!< UART reception (channel 0)                                  */
  INTUART0TX_IRQn               = 90,        /*!< UART transmit (channel 0)                                   */
  INTUART0ERR_IRQn              = 91,        /*!< UART error (channel 0)                                      */
  INTUART1RX_IRQn               = 92,        /*!< UART reception (channel 1)                                  */
  INTUART1TX_IRQn               = 93,        /*!< UART transmit (channel 1)                                   */
  INTUART1ERR_IRQn              = 94,        /*!< UART error (channel 1)                                      */
  INTUART2RX_IRQn               = 95,        /*!< UART reception (channel 2)                                  */
  INTUART2TX_IRQn               = 96,        /*!< UART transmit (channel 2)                                   */
  INTUART2ERR_IRQn              = 97,        /*!< UART error (channel 2)                                      */
  INTUART3RX_IRQn               = 98,        /*!< UART reception (channel 3)                                  */
  INTUART3TX_IRQn               = 99,        /*!< UART transmit (channel 3)                                   */
  INTUART3ERR_IRQn              = 100,       /*!< UART error (channel 3)                                      */
  INTUART4RX_IRQn               = 101,       /*!< UART reception (channel 4)                                  */
  INTUART4TX_IRQn               = 102,       /*!< UART transmit (channel 4)                                   */
  INTUART4ERR_IRQn              = 103,       /*!< UART error (channel 4)                                      */
  INTUART5RX_IRQn               = 104,       /*!< UART reception (channel 5)                                  */
  INTUART5TX_IRQn               = 105,       /*!< UART transmit (channel 5)                                   */
  INTUART5ERR_IRQn              = 106,       /*!< UART error (channel 5)                                      */
  INTFUART0_IRQn                = 107,       /*!< FUART Interrupt(channel 0)                                  */
  INTFUART1_IRQn                = 108,       /*!< FUART Interrupt(channel 1)                                  */
  INTI2C0_IRQn                  = 109,       /*!< I2C0 transmission and reception interrupt                   */
  INTI2C0AL_IRQn                = 110,       /*!< I2C0 arbitration lost interrupt                             */
  INTI2C0BF_IRQn                = 111,       /*!< I2C0 bus free interrupt                                     */
  INTI2C0NACK_IRQn              = 112,       /*!< I2C0 no ack interrupt                                       */
  INTI2C1_IRQn                  = 113,       /*!< I2C1 transmission and reception interrupt                   */
  INTI2C1AL_IRQn                = 114,       /*!< I2C1 arbitration lost interrupt                             */
  INTI2C1BF_IRQn                = 115,       /*!< I2C1 bus free interrupt                                     */
  INTI2C1NACK_IRQn              = 116,       /*!< I2C1 no ack interrupt                                       */
  INTI2C2_IRQn                  = 117,       /*!< I2C2 transmission and reception interrupt                   */
  INTI2C2AL_IRQn                = 118,       /*!< I2C2 arbitration lost interrupt                             */
  INTI2C2BF_IRQn                = 119,       /*!< I2C2 bus free interrupt                                     */
  INTI2C2NACK_IRQn              = 120,       /*!< I2C2 no ack interrupt                                       */
  INTI2C3_IRQn                  = 121,       /*!< I2C3 transmission and reception interrupt                   */
  INTI2C3AL_IRQn                = 122,       /*!< I2C3 arbitration lost interrupt                             */
  INTI2C3BF_IRQn                = 123,       /*!< I2C3 bus free interrupt                                     */
  INTI2C3NACK_IRQn              = 124,       /*!< I2C3 no ack interrupt                                       */
  INTI2C4_IRQn                  = 125,       /*!< I2C4 transmission and reception interrupt                   */
  INTI2C4AL_IRQn                = 126,       /*!< I2C4 arbitration lost interrupt                             */
  INTI2C4BF_IRQn                = 127,       /*!< I2C4 bus free interrupt                                     */
  INTI2C4NACK_IRQn              = 128,       /*!< I2C4 no ack interrupt                                       */
  INTADACP0_IRQn                = 129,       /*!< ADC conversion monitoring function interrupt 0              */
  INTADACP1_IRQn                = 130,       /*!< ADC conversion monitoring function interrupt 1              */
  INTADATRG_IRQn                = 131,       /*!< ADC conversion triggered by General purpose is finished     */
  INTADASGL_IRQn                = 132,       /*!< ADC conversion triggered by Single program is finished      */
  INTADACNT_IRQn                = 133,       /*!< ADC conversion triggered by Continuity program is finished  */
  INTADAHP_IRQn                 = 134,       /*!< ADC High Priority AD conversion interrupt                   */
  INTFLDRDY_IRQn                = 135,       /*!< Data FLASH Ready interrupt                                  */
  INTFLCRDY0_IRQn               = 136,       /*!< Code FLASH Area0/1 Ready interrupt                          */
  INTFLCRDY1_IRQn               = 137,       /*!< Code FLASH Area2 Ready interrupt                            */
  INTMDMAABERR_IRQn             = 139,       /*!< MDMA bus error(Unit A)                                      */
  INTMDMAADERR_IRQn             = 140        /*!< MDMA descriptor error(Unit A)                               */
} IRQn_Type;

/** Processor and Core Peripheral Section */

/* Configuration of the Cortex-M4 Processor and Core Peripherals */
#define __CM4_REV              0x0001       /*!< Cortex-M4 Core Revision                           */
#define __NVIC_PRIO_BITS          4         /*!< Number of Bits used for Priority Levels           */
#define __MPU_PRESENT             1         /*!< MPU present or not                                */
#define __FPU_PRESENT             1         /*!< FPU present or not                                */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used      */

/** @} */ /* End of group Configuration_of_CMSIS */

#include "core_cm4.h"                       /* Cortex-M4 processor and core peripherals            */
#include "system_TMPM4G9.h"                 /* TMPM4G9 System                                      */

/** @addtogroup Device_Peripheral_registers
  * @{
  */

/** Device Specific Peripheral registers structures */

/**
  * @brief DMA Controller
  */
typedef struct
{
  __I  uint32_t INTSTATUS;         /*!< DMAC Interrupt Status Register               */
  __I  uint32_t INTTCSTATUS;       /*!< DMAC Interrupt Terminal Count Status Register*/
  __O  uint32_t INTTCCLEAR;        /*!< DMAC Interrupt Terminal Count Clear Register */
  __I  uint32_t INTERRORSTATUS;    /*!< DMAC Interrupt Error Status Register         */
  __O  uint32_t INTERRCLR;         /*!< DMAC Interrupt Error Clear Register          */
  __I  uint32_t RAWINTTCSTATUS;    /*!< DMAC Raw Interrupt Terminal Count Status Register*/
  __I  uint32_t RAWINTERRORSTATUS; /*!< DMAC Raw Error Interrupt Status Register     */
  __I  uint32_t ENBLDCHNS;         /*!< DMAC Enabled Channel Register                */
  __IO uint32_t SOFTBREQ;          /*!< DMAC Software Burst Request Register         */
  __IO uint32_t SOFTSREQ;          /*!< DMAC Software Single Request Register        */
       uint32_t RESERVED0[2];
  __IO uint32_t CONFIGURATION;     /*!< DMAC Configuration Register                  */
       uint32_t RESERVED1[51];
  __IO uint32_t C0SRCADDR;         /*!< DMAC Channel 0 Source Address Register       */
  __IO uint32_t C0DESTADDR;        /*!< DMAC Channel 0 Destination Address Register  */
  __IO uint32_t C0LLI;             /*!< DMAC Channel 0 Linked List Item Register     */
  __IO uint32_t C0CONTROL;         /*!< DMAC Channel 0 Control Register              */
  __IO uint32_t C0CONFIGURATION;   /*!< DMAC Channel 0 Configuration Register        */
       uint32_t RESERVED2[3];
  __IO uint32_t C1SRCADDR;         /*!< DMAC Channel 1 Source Address Register       */
  __IO uint32_t C1DESTADDR;        /*!< DMAC Channel 1 Destination Address Register  */
  __IO uint32_t C1LLI;             /*!< DMAC Channel 1 Linked List Item Register     */
  __IO uint32_t C1CONTROL;         /*!< DMAC Channel 1 Control Register              */
  __IO uint32_t C1CONFIGURATION;   /*!< DMAC Channel 1 Configuration Register        */
} TSB_DMAC_TypeDef;

/**
  * @brief 
  */
typedef struct
{
  __IO uint32_t MAP0;              /*!< SMIF Flash Memory Map0 Register              */
  __IO uint32_t MAP1;              /*!< SMIF Flash Memory Map1 Register              */
  __IO uint32_t DACR0;             /*!< SMIF Direct Access Control Register 0        */
  __IO uint32_t DACR1;             /*!< SMIF Direct Access Control Register 1        */
  __IO uint32_t DRCR0;             /*!< SMIF Direct Read Control Register 0          */
  __IO uint32_t DRCR1;             /*!< SMIF Direct Read Control Register 1          */
       uint32_t RESERVED0[250];
  __IO uint32_t RACR0;             /*!< SMIF Program Register Access Control Register 0*/
  __IO uint32_t RACR1;             /*!< SMIF Program Register Access Control Register 1*/
  __IO uint32_t INT;               /*!< SMIF Program Register Access Interrupt Enable Register*/
  __IO uint32_t STAT;              /*!< SMIF Program Register Access Status Register */
       uint32_t RESERVED1[60];
  __IO uint32_t PBUF0;             /*!< SMIF Program Register Primary Buffer Data Register 0*/
  __IO uint32_t PBUF1;             /*!< SMIF Program Register Primary Buffer Data Register 1*/
       uint32_t RESERVED2[62];
  __IO uint32_t SBUF00;            /*!< SMIF Program Register Secondary Buffer Data Register 00*/
  __IO uint32_t SBUF01;            /*!< SMIF Program Register Secondary Buffer Data Register 01*/
  __IO uint32_t SBUF02;            /*!< SMIF Program Register Secondary Buffer Data Register 02*/
  __IO uint32_t SBUF03;            /*!< SMIF Program Register Secondary Buffer Data Register 03*/
  __IO uint32_t SBUF04;            /*!< SMIF Program Register Secondary Buffer Data Register 04*/
  __IO uint32_t SBUF05;            /*!< SMIF Program Register Secondary Buffer Data Register 05*/
  __IO uint32_t SBUF06;            /*!< SMIF Program Register Secondary Buffer Data Register 06*/
  __IO uint32_t SBUF07;            /*!< SMIF Program Register Secondary Buffer Data Register 07*/
  __IO uint32_t SBUF08;            /*!< SMIF Program Register Secondary Buffer Data Register 08*/
  __IO uint32_t SBUF09;            /*!< SMIF Program Register Secondary Buffer Data Register 09*/
  __IO uint32_t SBUF10;            /*!< SMIF Program Register Secondary Buffer Data Register 10*/
  __IO uint32_t SBUF11;            /*!< SMIF Program Register Secondary Buffer Data Register 11*/
  __IO uint32_t SBUF12;            /*!< SMIF Program Register Secondary Buffer Data Register 12*/
  __IO uint32_t SBUF13;            /*!< SMIF Program Register Secondary Buffer Data Register 13*/
  __IO uint32_t SBUF14;            /*!< SMIF Program Register Secondary Buffer Data Register 14*/
  __IO uint32_t SBUF15;            /*!< SMIF Program Register Secondary Buffer Data Register 15*/
  __IO uint32_t SBUF16;            /*!< SMIF Program Register Secondary Buffer Data Register 16*/
  __IO uint32_t SBUF17;            /*!< SMIF Program Register Secondary Buffer Data Register 17*/
  __IO uint32_t SBUF18;            /*!< SMIF Program Register Secondary Buffer Data Register 18*/
  __IO uint32_t SBUF19;            /*!< SMIF Program Register Secondary Buffer Data Register 19*/
  __IO uint32_t SBUF20;            /*!< SMIF Program Register Secondary Buffer Data Register 20*/
  __IO uint32_t SBUF21;            /*!< SMIF Program Register Secondary Buffer Data Register 21*/
  __IO uint32_t SBUF22;            /*!< SMIF Program Register Secondary Buffer Data Register 22*/
  __IO uint32_t SBUF23;            /*!< SMIF Program Register Secondary Buffer Data Register 23*/
  __IO uint32_t SBUF24;            /*!< SMIF Program Register Secondary Buffer Data Register 24*/
  __IO uint32_t SBUF25;            /*!< SMIF Program Register Secondary Buffer Data Register 25*/
  __IO uint32_t SBUF26;            /*!< SMIF Program Register Secondary Buffer Data Register 26*/
  __IO uint32_t SBUF27;            /*!< SMIF Program Register Secondary Buffer Data Register 27*/
  __IO uint32_t SBUF28;            /*!< SMIF Program Register Secondary Buffer Data Register 28*/
  __IO uint32_t SBUF29;            /*!< SMIF Program Register Secondary Buffer Data Register 29*/
  __IO uint32_t SBUF30;            /*!< SMIF Program Register Secondary Buffer Data Register 30*/
  __IO uint32_t SBUF31;            /*!< SMIF Program Register Secondary Buffer Data Register 31*/
  __IO uint32_t SBUF32;            /*!< SMIF Program Register Secondary Buffer Data Register 32*/
  __IO uint32_t SBUF33;            /*!< SMIF Program Register Secondary Buffer Data Register 33*/
  __IO uint32_t SBUF34;            /*!< SMIF Program Register Secondary Buffer Data Register 34*/
  __IO uint32_t SBUF35;            /*!< SMIF Program Register Secondary Buffer Data Register 35*/
  __IO uint32_t SBUF36;            /*!< SMIF Program Register Secondary Buffer Data Register 36*/
  __IO uint32_t SBUF37;            /*!< SMIF Program Register Secondary Buffer Data Register 37*/
  __IO uint32_t SBUF38;            /*!< SMIF Program Register Secondary Buffer Data Register 38*/
  __IO uint32_t SBUF39;            /*!< SMIF Program Register Secondary Buffer Data Register 39*/
  __IO uint32_t SBUF40;            /*!< SMIF Program Register Secondary Buffer Data Register 40*/
  __IO uint32_t SBUF41;            /*!< SMIF Program Register Secondary Buffer Data Register 41*/
  __IO uint32_t SBUF42;            /*!< SMIF Program Register Secondary Buffer Data Register 42*/
  __IO uint32_t SBUF43;            /*!< SMIF Program Register Secondary Buffer Data Register 43*/
  __IO uint32_t SBUF44;            /*!< SMIF Program Register Secondary Buffer Data Register 44*/
  __IO uint32_t SBUF45;            /*!< SMIF Program Register Secondary Buffer Data Register 45*/
  __IO uint32_t SBUF46;            /*!< SMIF Program Register Secondary Buffer Data Register 46*/
  __IO uint32_t SBUF47;            /*!< SMIF Program Register Secondary Buffer Data Register 47*/
  __IO uint32_t SBUF48;            /*!< SMIF Program Register Secondary Buffer Data Register 48*/
  __IO uint32_t SBUF49;            /*!< SMIF Program Register Secondary Buffer Data Register 49*/
  __IO uint32_t SBUF50;            /*!< SMIF Program Register Secondary Buffer Data Register 50*/
  __IO uint32_t SBUF51;            /*!< SMIF Program Register Secondary Buffer Data Register 51*/
  __IO uint32_t SBUF52;            /*!< SMIF Program Register Secondary Buffer Data Register 52*/
  __IO uint32_t SBUF53;            /*!< SMIF Program Register Secondary Buffer Data Register 53*/
  __IO uint32_t SBUF54;            /*!< SMIF Program Register Secondary Buffer Data Register 54*/
  __IO uint32_t SBUF55;            /*!< SMIF Program Register Secondary Buffer Data Register 55*/
  __IO uint32_t SBUF56;            /*!< SMIF Program Register Secondary Buffer Data Register 56*/
  __IO uint32_t SBUF57;            /*!< SMIF Program Register Secondary Buffer Data Register 57*/
  __IO uint32_t SBUF58;            /*!< SMIF Program Register Secondary Buffer Data Register 58*/
  __IO uint32_t SBUF59;            /*!< SMIF Program Register Secondary Buffer Data Register 59*/
  __IO uint32_t SBUF60;            /*!< SMIF Program Register Secondary Buffer Data Register 60*/
  __IO uint32_t SBUF61;            /*!< SMIF Program Register Secondary Buffer Data Register 61*/
  __IO uint32_t SBUF62;            /*!< SMIF Program Register Secondary Buffer Data Register 62*/
  __IO uint32_t SBUF63;            /*!< SMIF Program Register Secondary Buffer Data Register 63*/
} TSB_SMI_TypeDef;

/**
  * @brief Interrupt control A Register
  */
typedef struct
{
  __IO uint8_t  NIC00;             /*!< Non Maskable interrupt Mode Control Register A 00*/
       uint8_t  RESERVED0[31];
  __IO uint8_t  IMC00;             /*!< interrupt Mode Control Register A 00         */
  __IO uint8_t  IMC01;             /*!< interrupt Mode Control Register A 01         */
  __IO uint8_t  IMC02;             /*!< interrupt Mode Control Register A 02         */
  __IO uint8_t  IMC03;             /*!< interrupt Mode Control Register A 03         */
  __IO uint8_t  IMC04;             /*!< interrupt Mode Control Register A 04         */
  __IO uint8_t  IMC05;             /*!< interrupt Mode Control Register A 05         */
  __IO uint8_t  IMC06;             /*!< interrupt Mode Control Register A 06         */
  __IO uint8_t  IMC07;             /*!< interrupt Mode Control Register A 07         */
  __IO uint8_t  IMC08;             /*!< interrupt Mode Control Register A 08         */
  __IO uint8_t  IMC09;             /*!< interrupt Mode Control Register A 09         */
  __IO uint8_t  IMC10;             /*!< interrupt Mode Control Register A 10         */
  __IO uint8_t  IMC11;             /*!< interrupt Mode Control Register A 11         */
  __IO uint8_t  IMC12;             /*!< interrupt Mode Control Register A 12         */
  __IO uint8_t  IMC13;             /*!< interrupt Mode Control Register A 13         */
  __IO uint8_t  IMC14;             /*!< interrupt Mode Control Register A 14         */
  __IO uint8_t  IMC15;             /*!< interrupt Mode Control Register A 15         */
  __IO uint8_t  IMC16;             /*!< interrupt Mode Control Register A 16         */
  __IO uint8_t  IMC17;             /*!< interrupt Mode Control Register A 17         */
  __IO uint8_t  IMC18;             /*!< interrupt Mode Control Register A 18         */
  __IO uint8_t  IMC19;             /*!< interrupt Mode Control Register A 19         */
  __IO uint8_t  IMC20;             /*!< interrupt Mode Control Register A 20         */
  __IO uint8_t  IMC21;             /*!< interrupt Mode Control Register A 21         */
  __IO uint8_t  IMC22;             /*!< interrupt Mode Control Register A 22         */
  __IO uint8_t  IMC23;             /*!< interrupt Mode Control Register A 23         */
  __IO uint8_t  IMC24;             /*!< interrupt Mode Control Register A 24         */
  __IO uint8_t  IMC25;             /*!< interrupt Mode Control Register A 25         */
  __IO uint8_t  IMC26;             /*!< interrupt Mode Control Register A 26         */
  __IO uint8_t  IMC27;             /*!< interrupt Mode Control Register A 27         */
  __IO uint8_t  IMC28;             /*!< interrupt Mode Control Register A 28         */
  __IO uint8_t  IMC29;             /*!< interrupt Mode Control Register A 29         */
  __IO uint8_t  IMC30;             /*!< interrupt Mode Control Register A 30         */
  __IO uint8_t  IMC31;             /*!< interrupt Mode Control Register A 31         */
       uint8_t  RESERVED1[17];
  __IO uint8_t  IMC49;             /*!< interrupt Mode Control Register A 49         */
  __IO uint8_t  IMC50;             /*!< interrupt Mode Control Register A 50         */
  __IO uint8_t  IMC51;             /*!< interrupt Mode Control Register A 51         */
  __IO uint8_t  IMC52;             /*!< interrupt Mode Control Register A 52         */
  __IO uint8_t  IMC53;             /*!< interrupt Mode Control Register A 53         */
  __IO uint8_t  IMC54;             /*!< interrupt Mode Control Register A 54         */
  __IO uint8_t  IMC55;             /*!< interrupt Mode Control Register A 55         */
  __IO uint8_t  IMC56;             /*!< interrupt Mode Control Register A 56         */
  __IO uint8_t  IMC57;             /*!< interrupt Mode Control Register A 57         */
} TSB_IA_TypeDef;

/**
  * @brief Reset Low power Management Register
  */
typedef struct
{
  __IO uint8_t  LOSCCR;            /*!< Low OSC and IHOSC2 clock supply Control Register*/
  __IO uint8_t  SHTDNOP;           /*!< Power Shut Down Control Register             */
  __IO uint8_t  RSTFLG0;           /*!< Reset flag register 0                        */
  __IO uint8_t  RSTFLG1;           /*!< Reset flag register 1                        */
       uint8_t  RESERVED0[11];
  __IO uint8_t  PROTECT;           /*!< Protect Register                             */
} TSB_RLM_TypeDef;

/**
  * @brief LVD0
  */
typedef struct
{
  __IO uint8_t  CR1;               /*!< LVD Control register1                        */
  __IO uint8_t  CR2;               /*!< LVD Control register2                        */
  __IO uint8_t  LVL1;              /*!< LVD detection voltage select register 1      */
  __IO uint8_t  LVL2;              /*!< LVD detection voltage select register 2      */
  __I  uint8_t  SR;                /*!< LVD status register                          */
} TSB_LVD_TypeDef;

/**
  * @brief TRGSEL
  */
typedef struct
{
  __IO uint32_t CR0;               /*!< TRGSEL Control register 0                    */
  __IO uint32_t CR1;               /*!< TRGSEL Control register 1                    */
  __IO uint32_t CR2;               /*!< TSEL Control register 2                      */
  __IO uint32_t CR3;               /*!< TRGSEL Control register 3                    */
  __IO uint32_t CR4;               /*!< TRGSEL Control register 4                    */
  __IO uint32_t CR5;               /*!< TRGSEL Control register 5                    */
  __IO uint32_t CR6;               /*!< TRGSEL Control register 6                    */
  __IO uint32_t CR7;               /*!< TRGSEL Control register 7                    */
  __IO uint32_t CR8;               /*!< TRGSEL Control register 8                    */
  __IO uint32_t CR9;               /*!< TRGSEL Control register 9                    */
  __IO uint32_t CR10;              /*!< TRGSEL Control register 10                   */
  __IO uint32_t CR11;              /*!< TRGSEL Control register 11                   */
  __IO uint32_t CR12;              /*!< TRGSEL Control register 12                   */
  __IO uint32_t CR13;              /*!< TRGSEL Control register 13                   */
} TSB_TSEL_TypeDef;

/**
  * @brief Long Term Timer(LTTMR)
  */
typedef struct
{
  __IO uint8_t  CR0;               /*!< Long Term Control Register                   */
  __IO uint8_t  VALL;              /*!< Long Term Setting Register                   */
  __IO uint8_t  VALH;              /*!< Long Term Setting Register                   */
} TSB_LTT_TypeDef;

/**
  * @brief Serial Interface (TSPI)
  */
typedef struct
{
  __IO uint32_t CR0;               /*!< TSPI Control Register 0                      */
  __IO uint32_t CR1;               /*!< TSPI Control Register 1                      */
  __IO uint32_t CR2;               /*!< TSPI Control Register 2                      */
  __IO uint32_t CR3;               /*!< TSPI Control Register 3                      */
  __IO uint32_t BR;                /*!< TSPI Baud Rate Generator Control Register    */
  __IO uint32_t FMTR0;             /*!< TSPI Format Control Register 0               */
  __IO uint32_t FMTR1;             /*!< TSPI Format Control Register 1               */
       uint32_t RESERVED0[57];
  __IO uint32_t DR;                /*!< TSPI Data Register                           */
       uint32_t RESERVED1[63];
  __IO uint32_t SR;                /*!< TSPI Status Register                         */
  __IO uint32_t ERR;               /*!< TSPI Parity Error Flag Register              */
} TSB_TSPI_TypeDef;

/**
  * @brief External Bus Interface(EXB)
  */
typedef struct
{
  __IO uint32_t MOD;               /*!< External Bus Mode Control Register           */
       uint32_t RESERVED0[3];
  __IO uint32_t AS0;               /*!< External Bus Base Address and CS Space setting Register 0*/
  __IO uint32_t AS1;               /*!< External Bus Base Address and CS Space setting Register 1 */
  __IO uint32_t AS2;               /*!< External Bus Base Address and CS Space setting Register 2*/
  __IO uint32_t AS3;               /*!< External Bus Base Address and CS Space setting Register 3*/
       uint32_t RESERVED1[8];
  __IO uint32_t CS0;               /*!< Chip Select and Wait Controller Register 0   */
  __IO uint32_t CS1;               /*!< Chip Select and Wait Controller Register 1   */
  __IO uint32_t CS2;               /*!< Chip Select and Wait Controller Register 2   */
  __IO uint32_t CS3;               /*!< Chip Select and Wait Controller Register 3   */
       uint32_t RESERVED2[4];
  __IO uint32_t CLKCTL;            /*!< Clock output controlRegister                 */
} TSB_EXB_TypeDef;

/**
  * @brief Clock Generator (CG)
  */
typedef struct
{
  __IO uint32_t PROTECT;           /*!< Protect Register                             */
  __IO uint32_t OSCCR;             /*!< Oscillation Control Register                 */
  __IO uint32_t SYSCR;             /*!< System Clock Control Register                */
  __IO uint32_t STBYCR;            /*!< Standby Control Register                     */
       uint32_t RESERVED0[4];
  __IO uint32_t PLL0SEL;           /*!< PLL Selection Register 0                     */
       uint32_t RESERVED1[3];
  __IO uint32_t WUPHCR;            /*!< High OSC Warming-up Register                 */
  __IO uint32_t WUPLCR;            /*!< Low OSC Warming-up Register                  */
       uint32_t RESERVED2[4];
  __IO uint32_t FSYSMENA;          /*!< Middle fsys Supply Stop Register A           */
  __IO uint32_t FSYSMENB;          /*!< Middle fsys Supply Stop Register A           */
  __IO uint32_t FSYSENA;           /*!< High fsys Supply Stop Register A             */
       uint32_t RESERVED3;
  __IO uint32_t FCEN;              /*!< FC Supply Stop Register                      */
  __IO uint32_t SPCLKEN;           /*!< ADC TRACE Clock Supply Stop Register         */
       uint32_t RESERVED4[2];
  __IO uint32_t EXTEND2;           /*!< Extend for MDMAC Register                    */
} TSB_CG_TypeDef;

/**
  * @brief Interrupt Control B Register
  */
typedef struct
{
       uint8_t  RESERVED0[16];
  __IO uint8_t  NIC00;             /*!< Non maskable interrupt Control Register 00   */
       uint8_t  RESERVED1[79];
  __IO uint8_t  IMC000;            /*!< interrupt Mode Control Register 000          */
  __IO uint8_t  IMC001;            /*!< interrupt Mode Control Register 001          */
  __IO uint8_t  IMC002;            /*!< interrupt Mode Control Register 002          */
  __IO uint8_t  IMC003;            /*!< interrupt Mode Control Register 003          */
  __IO uint8_t  IMC004;            /*!< interrupt Mode Control Register 004          */
  __IO uint8_t  IMC005;            /*!< interrupt Mode Control Register 005          */
  __IO uint8_t  IMC006;            /*!< interrupt Mode Control Register 006          */
  __IO uint8_t  IMC007;            /*!< interrupt Mode Control Register 007          */
  __IO uint8_t  IMC008;            /*!< interrupt Mode Control Register 008          */
  __IO uint8_t  IMC009;            /*!< interrupt Mode Control Register 009          */
  __IO uint8_t  IMC010;            /*!< interrupt Mode Control Register 010          */
  __IO uint8_t  IMC011;            /*!< interrupt Mode Control Register 011          */
  __IO uint8_t  IMC012;            /*!< interrupt Mode Control Register 012          */
  __IO uint8_t  IMC013;            /*!< interrupt Mode Control Register 013          */
  __IO uint8_t  IMC014;            /*!< interrupt Mode Control Register 014          */
  __IO uint8_t  IMC015;            /*!< interrupt Mode Control Register 015          */
  __IO uint8_t  IMC016;            /*!< interrupt Mode Control Register 016          */
  __IO uint8_t  IMC017;            /*!< interrupt Mode Control Register 017          */
  __IO uint8_t  IMC018;            /*!< interrupt Mode Control Register 018          */
  __IO uint8_t  IMC019;            /*!< interrupt Mode Control Register 019          */
  __IO uint8_t  IMC020;            /*!< interrupt Mode Control Register 020          */
  __IO uint8_t  IMC021;            /*!< interrupt Mode Control Register 021          */
  __IO uint8_t  IMC022;            /*!< interrupt Mode Control Register 022          */
  __IO uint8_t  IMC023;            /*!< interrupt Mode Control Register 023          */
  __IO uint8_t  IMC024;            /*!< interrupt Mode Control Register 024          */
  __IO uint8_t  IMC025;            /*!< interrupt Mode Control Register 025          */
  __IO uint8_t  IMC026;            /*!< interrupt Mode Control Register 026          */
  __IO uint8_t  IMC027;            /*!< interrupt Mode Control Register 027          */
  __IO uint8_t  IMC028;            /*!< interrupt Mode Control Register 028          */
  __IO uint8_t  IMC029;            /*!< interrupt Mode Control Register 029          */
  __IO uint8_t  IMC030;            /*!< interrupt Mode Control Register 030          */
  __IO uint8_t  IMC031;            /*!< interrupt Mode Control Register 031          */
  __IO uint8_t  IMC032;            /*!< interrupt Mode Control Register 032          */
  __IO uint8_t  IMC033;            /*!< interrupt Mode Control Register 033          */
  __IO uint8_t  IMC034;            /*!< interrupt Mode Control Register 034          */
  __IO uint8_t  IMC035;            /*!< interrupt Mode Control Register 035          */
  __IO uint8_t  IMC036;            /*!< interrupt Mode Control Register 036          */
  __IO uint8_t  IMC037;            /*!< interrupt Mode Control Register 037          */
  __IO uint8_t  IMC038;            /*!< interrupt Mode Control Register 038          */
  __IO uint8_t  IMC039;            /*!< interrupt Mode Control Register 039          */
  __IO uint8_t  IMC040;            /*!< interrupt Mode Control Register 040          */
  __IO uint8_t  IMC041;            /*!< interrupt Mode Control Register 041          */
  __IO uint8_t  IMC042;            /*!< interrupt Mode Control Register 042          */
  __IO uint8_t  IMC043;            /*!< interrupt Mode Control Register 043          */
  __IO uint8_t  IMC044;            /*!< interrupt Mode Control Register 044          */
  __IO uint8_t  IMC045;            /*!< interrupt Mode Control Register 045          */
  __IO uint8_t  IMC046;            /*!< interrupt Mode Control Register 046          */
  __IO uint8_t  IMC047;            /*!< interrupt Mode Control Register 047          */
  __IO uint8_t  IMC048;            /*!< interrupt Mode Control Register 048          */
  __IO uint8_t  IMC049;            /*!< interrupt Mode Control Register 049          */
  __IO uint8_t  IMC050;            /*!< interrupt Mode Control Register 050          */
  __IO uint8_t  IMC051;            /*!< interrupt Mode Control Register 051          */
  __IO uint8_t  IMC052;            /*!< interrupt Mode Control Register 052          */
  __IO uint8_t  IMC053;            /*!< interrupt Mode Control Register 053          */
  __IO uint8_t  IMC054;            /*!< interrupt Mode Control Register 054          */
  __IO uint8_t  IMC055;            /*!< interrupt Mode Control Register 055          */
  __IO uint8_t  IMC056;            /*!< interrupt Mode Control Register 056          */
  __IO uint8_t  IMC057;            /*!< interrupt Mode Control Register 057          */
  __IO uint8_t  IMC058;            /*!< interrupt Mode Control Register 058          */
  __IO uint8_t  IMC059;            /*!< interrupt Mode Control Register 059          */
  __IO uint8_t  IMC060;            /*!< interrupt Mode Control Register 060          */
  __IO uint8_t  IMC061;            /*!< interrupt Mode Control Register 061          */
  __IO uint8_t  IMC062;            /*!< interrupt Mode Control Register 062          */
  __IO uint8_t  IMC063;            /*!< interrupt Mode Control Register 063          */
  __IO uint8_t  IMC064;            /*!< interrupt Mode Control Register 064          */
  __IO uint8_t  IMC065;            /*!< interrupt Mode Control Register 065          */
  __IO uint8_t  IMC066;            /*!< interrupt Mode Control Register 066          */
  __IO uint8_t  IMC067;            /*!< interrupt Mode Control Register 067          */
  __IO uint8_t  IMC068;            /*!< interrupt Mode Control Register 068          */
  __IO uint8_t  IMC069;            /*!< interrupt Mode Control Register 069          */
  __IO uint8_t  IMC070;            /*!< interrupt Mode Control Register 070          */
  __IO uint8_t  IMC071;            /*!< interrupt Mode Control Register 071          */
  __IO uint8_t  IMC072;            /*!< interrupt Mode Control Register 072          */
  __IO uint8_t  IMC073;            /*!< interrupt Mode Control Register 073          */
  __IO uint8_t  IMC074;            /*!< interrupt Mode Control Register 074          */
  __IO uint8_t  IMC075;            /*!< interrupt Mode Control Register 075          */
  __IO uint8_t  IMC076;            /*!< interrupt Mode Control Register 076          */
  __IO uint8_t  IMC077;            /*!< interrupt Mode Control Register 077          */
  __IO uint8_t  IMC078;            /*!< interrupt Mode Control Register 078          */
  __IO uint8_t  IMC079;            /*!< interrupt Mode Control Register 079          */
  __IO uint8_t  IMC080;            /*!< interrupt Mode Control Register 080          */
  __IO uint8_t  IMC081;            /*!< interrupt Mode Control Register 081          */
  __IO uint8_t  IMC082;            /*!< interrupt Mode Control Register 082          */
  __IO uint8_t  IMC083;            /*!< interrupt Mode Control Register 083          */
  __IO uint8_t  IMC084;            /*!< interrupt Mode Control Register 084          */
  __IO uint8_t  IMC085;            /*!< interrupt Mode Control Register 085          */
  __IO uint8_t  IMC086;            /*!< interrupt Mode Control Register 086          */
  __IO uint8_t  IMC087;            /*!< interrupt Mode Control Register 087          */
  __IO uint8_t  IMC088;            /*!< interrupt Mode Control Register 088          */
  __IO uint8_t  IMC089;            /*!< interrupt Mode Control Register 089          */
  __IO uint8_t  IMC090;            /*!< interrupt Mode Control Register 090          */
  __IO uint8_t  IMC091;            /*!< interrupt Mode Control Register 091          */
  __IO uint8_t  IMC092;            /*!< interrupt Mode Control Register 092          */
  __IO uint8_t  IMC093;            /*!< interrupt Mode Control Register 093          */
  __IO uint8_t  IMC094;            /*!< interrupt Mode Control Register 094          */
  __IO uint8_t  IMC095;            /*!< interrupt Mode Control Register 095          */
  __IO uint8_t  IMC096;            /*!< interrupt Mode Control Register 096          */
  __IO uint8_t  IMC097;            /*!< interrupt Mode Control Register 097          */
  __IO uint8_t  IMC098;            /*!< interrupt Mode Control Register 098          */
  __IO uint8_t  IMC099;            /*!< interrupt Mode Control Register 099          */
  __IO uint8_t  IMC100;            /*!< interrupt Mode Control Register 100          */
  __IO uint8_t  IMC101;            /*!< interrupt Mode Control Register 101          */
  __IO uint8_t  IMC102;            /*!< interrupt Mode Control Register 102          */
  __IO uint8_t  IMC103;            /*!< interrupt Mode Control Register 103          */
  __IO uint8_t  IMC104;            /*!< interrupt Mode Control Register 104          */
  __IO uint8_t  IMC105;            /*!< interrupt Mode Control Register 105          */
  __IO uint8_t  IMC106;            /*!< interrupt Mode Control Register 106          */
  __IO uint8_t  IMC107;            /*!< interrupt Mode Control Register 107          */
  __IO uint8_t  IMC108;            /*!< interrupt Mode Control Register 108          */
  __IO uint8_t  IMC109;            /*!< interrupt Mode Control Register 109          */
  __IO uint8_t  IMC110;            /*!< interrupt Mode Control Register 110          */
  __IO uint8_t  IMC111;            /*!< interrupt Mode Control Register 111          */
  __IO uint8_t  IMC112;            /*!< interrupt Mode Control Register 112          */
  __IO uint8_t  IMC113;            /*!< interrupt Mode Control Register 113          */
  __IO uint8_t  IMC114;            /*!< interrupt Mode Control Register 114          */
  __IO uint8_t  IMC115;            /*!< interrupt Mode Control Register 115          */
  __IO uint8_t  IMC116;            /*!< interrupt Mode Control Register 116          */
  __IO uint8_t  IMC117;            /*!< interrupt Mode Control Register 117          */
  __IO uint8_t  IMC118;            /*!< interrupt Mode Control Register 118          */
  __IO uint8_t  IMC119;            /*!< interrupt Mode Control Register 119          */
  __IO uint8_t  IMC120;            /*!< interrupt Mode Control Register 120          */
  __IO uint8_t  IMC121;            /*!< interrupt Mode Control Register 121          */
  __IO uint8_t  IMC122;            /*!< interrupt Mode Control Register 122          */
  __IO uint8_t  IMC123;            /*!< interrupt Mode Control Register 123          */
  __IO uint8_t  IMC124;            /*!< interrupt Mode Control Register 124          */
  __IO uint8_t  IMC125;            /*!< interrupt Mode Control Register 125          */
  __IO uint8_t  IMC126;            /*!< interrupt Mode Control Register 126          */
  __IO uint8_t  IMC127;            /*!< interrupt Mode Control Register 127          */
  __IO uint8_t  IMC128;            /*!< interrupt Mode Control Register 128          */
  __IO uint8_t  IMC129;            /*!< interrupt Mode Control Register 129          */
  __IO uint8_t  IMC130;            /*!< interrupt Mode Control Register 130          */
  __IO uint8_t  IMC131;            /*!< interrupt Mode Control Register 131          */
  __IO uint8_t  IMC132;            /*!< interrupt Mode Control Register 132          */
  __IO uint8_t  IMC133;            /*!< interrupt Mode Control Register 133          */
  __IO uint8_t  IMC134;            /*!< interrupt Mode Control Register 134          */
  __IO uint8_t  IMC135;            /*!< interrupt Mode Control Register 135          */
  __IO uint8_t  IMC136;            /*!< interrupt Mode Control Register 136          */
  __IO uint8_t  IMC137;            /*!< interrupt Mode Control Register 137          */
  __IO uint8_t  IMC138;            /*!< interrupt Mode Control Register 138          */
  __IO uint8_t  IMC139;            /*!< interrupt Mode Control Register 139          */
  __IO uint8_t  IMC140;            /*!< interrupt Mode Control Register 140          */
  __IO uint8_t  IMC141;            /*!< interrupt Mode Control Register 141          */
} TSB_IB_TypeDef;

/**
  * @brief Interrupt Monitor Register
  */
typedef struct
{
  __I  uint32_t FLGNMI;            /*!< NMI Interrupt Monitor Flag 0                 */
  __I  uint32_t FLG1;              /*!< Interrupt Monitor Flag 1 (032 - 063)         */
  __I  uint32_t FLG2;              /*!< Interrupt Monitor Flag 2 (064 - 095)         */
  __I  uint32_t FLG3;              /*!< Interrupt Monitor Flag 3 (096 - 127)         */
  __I  uint32_t FLG4;              /*!< Interrupt Monitor Flag 4 (128 - 159)         */
  __I  uint32_t FLG5;              /*!< Interrupt Monitor Flag 5 (160 - 191)         */
  __I  uint32_t FLG6;              /*!< Interrupt Monitor Flag 6 (192 - 223)         */
  __I  uint32_t FLG7;              /*!< Interrupt Monitor Flag 7 (224 - 225)         */
} TSB_IMN_TypeDef;

/**
  * @brief DNF
  */
typedef struct
{
  __IO uint32_t CKCR;              /*!< DNF clock Control register                   */
  __IO uint32_t ENCR;              /*!< DNF Enable register                          */
} TSB_DNF_TypeDef;

/**
  * @brief Watchdog Timer (WD)
  */
typedef struct
{
  __IO uint32_t PRO;               /*!< SIWD Protect Register                        */
  __IO uint32_t EN;                /*!< SIWD Enable Register                         */
  __O  uint32_t CR;                /*!< SIWD Control Register                        */
  __IO uint32_t MOD;               /*!< SIWD Mode Register                           */
  __I  uint32_t MONI;              /*!< SIWD Monitor Register                        */
} TSB_SIWD_TypeDef;

/**
  * @brief NBD
  */
typedef struct
{
  __IO uint32_t CR0;               /*!< NBD control register 0                       */
  __IO uint32_t CR1;               /*!< NBD control register 1                       */
} TSB_NBD_TypeDef;

/**
  * @brief Malti Porpose Direct Memory Accsess(MDMA)
  */
typedef struct
{
       uint32_t RESERVED0;
  __IO uint32_t CEN;               /*!< MDMAC Channel Enable Register                */
  __IO uint32_t REQ;               /*!< MDMAC Transfer Request Register              */
  __IO uint32_t SUS;               /*!< MDMAC Transfer Suspension Register           */
  __IO uint32_t ACT;               /*!< MDMAC Transfer Active Register               */
  __IO uint32_t END;               /*!< MDMAC Transfer End Register                  */
  __IO uint32_t PRI;               /*!< MDMAC Priority Setting Register              */
  __IO uint32_t ENE;               /*!< MDMAC End Interrupt Enable Register          */
  __IO uint32_t DTAB;              /*!< MDMAC Channel Information Address Register   */
       uint32_t RESERVED1;
  __I  uint32_t CHN;               /*!< MDMAC Channel Number Register                */
  __I  uint32_t XFTYP;             /*!< MDMAC Channel Number Register                */
  __I  uint32_t XFSAD;             /*!< MDMAC Transfer Source Address Register       */
  __I  uint32_t XFDAD;             /*!< MDMAC Transfer Destination Address Register  */
  __I  uint32_t XFSIZ;             /*!< MDMAC Transfer Size Register                 */
  __I  uint32_t DSADS;             /*!< MDMAC Descriptor Address Register            */
  __I  uint32_t DSNUM;             /*!< MDMAC Descriptor Number Register             */
       uint32_t RESERVED2[3];
  __I  uint32_t C00XFTYP;          /*!< MDMAC Channel 00 Number Avoidance Register   */
  __I  uint32_t C00XFSAD;          /*!< MDMAC Channel 00 Transfer Source Address Avoidance Register*/
  __I  uint32_t C00XFDAD;          /*!< MDMAC Channel 00 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C00XFSIZ;          /*!< MDMAC Channel 00 Transfer Size Avoidance Register*/
  __I  uint32_t C00DSADS;          /*!< MDMAC Channel 00 Descriptor Address Avoidance Register*/
  __I  uint32_t C00DSNUM;          /*!< MDMAC Channel 00 Descriptor Number Avoidance Register*/
       uint32_t RESERVED3[2];
  __I  uint32_t C01XFTYP;          /*!< MDMAC Channel 01 Number Avoidance Register   */
  __I  uint32_t C01XFSAD;          /*!< MDMAC Channel 01 Transfer Source Address Avoidance Register*/
  __I  uint32_t C01XFDAD;          /*!< MDMAC Channel 01 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C01XFSIZ;          /*!< MDMAC Channel 01 Transfer Size Avoidance Register*/
  __I  uint32_t C01DSADS;          /*!< MDMAC Channel 01 Descriptor Address Avoidance Register*/
  __I  uint32_t C01DSNUM;          /*!< MDMAC Channel 01 Descriptor Number Avoidance Register*/
       uint32_t RESERVED4[2];
  __I  uint32_t C02XFTYP;          /*!< MDMAC Channel 02 Number Avoidance Register   */
  __I  uint32_t C02XFSAD;          /*!< MDMAC Channel 02 Transfer Source Address Avoidance Register*/
  __I  uint32_t C02XFDAD;          /*!< MDMAC Channel 02 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C02XFSIZ;          /*!< MDMAC Channel 02 Transfer Size Avoidance Register*/
  __I  uint32_t C02DSADS;          /*!< MDMAC Channel 02 Descriptor Address Avoidance Register*/
  __I  uint32_t C02DSNUM;          /*!< MDMAC Channel 02 Descriptor Number Avoidance Register*/
       uint32_t RESERVED5[2];
  __I  uint32_t C03XFTYP;          /*!< MDMAC Channel 03 Number Avoidance Register   */
  __I  uint32_t C03XFSAD;          /*!< MDMAC Channel 03 Transfer Source Address Avoidance Register*/
  __I  uint32_t C03XFDAD;          /*!< MDMAC Channel 03 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C03XFSIZ;          /*!< MDMAC Channel 03 Transfer Size Avoidance Register*/
  __I  uint32_t C03DSADS;          /*!< MDMAC Channel 03 Descriptor Address Avoidance Register*/
  __I  uint32_t C03DSNUM;          /*!< MDMAC Channel 03 Descriptor Number Avoidance Register*/
       uint32_t RESERVED6[2];
  __I  uint32_t C04XFTYP;          /*!< MDMAC Channel 04 Number Avoidance Register   */
  __I  uint32_t C04XFSAD;          /*!< MDMAC Channel 04 Transfer Source Address Avoidance Register*/
  __I  uint32_t C04XFDAD;          /*!< MDMAC Channel 04 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C04XFSIZ;          /*!< MDMAC Channel 04 Transfer Size Avoidance Register*/
  __I  uint32_t C04DSADS;          /*!< MDMAC Channel 04 Descriptor Address Avoidance Register*/
  __I  uint32_t C04DSNUM;          /*!< MDMAC Channel 04 Descriptor Number Avoidance Register*/
       uint32_t RESERVED7[2];
  __I  uint32_t C05XFTYP;          /*!< MDMAC Channel 05 Number Avoidance Register   */
  __I  uint32_t C05XFSAD;          /*!< MDMAC Channel 05 Transfer Source Address Avoidance Register*/
  __I  uint32_t C05XFDAD;          /*!< MDMAC Channel 05 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C05XFSIZ;          /*!< MDMAC Channel 05 Transfer Size Avoidance Register*/
  __I  uint32_t C05DSADS;          /*!< MDMAC Channel 05 Descriptor Address Avoidance Register*/
  __I  uint32_t C05DSNUM;          /*!< MDMAC Channel 05 Descriptor Number Avoidance Register*/
       uint32_t RESERVED8[2];
  __I  uint32_t C06XFTYP;          /*!< MDMAC Channel 06 Number Avoidance Register   */
  __I  uint32_t C06XFSAD;          /*!< MDMAC Channel 06 Transfer Source Address Avoidance Register*/
  __I  uint32_t C06XFDAD;          /*!< MDMAC Channel 06 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C06XFSIZ;          /*!< MDMAC Channel 06 Transfer Size Avoidance Register*/
  __I  uint32_t C06DSADS;          /*!< MDMAC Channel 06 Descriptor Address Avoidance Register*/
  __I  uint32_t C06DSNUM;          /*!< MDMAC Channel 06 Descriptor Number Avoidance Register*/
       uint32_t RESERVED9[2];
  __I  uint32_t C07XFTYP;          /*!< MDMAC Channel 07 Number Avoidance Register   */
  __I  uint32_t C07XFSAD;          /*!< MDMAC Channel 07 Transfer Source Address Avoidance Register*/
  __I  uint32_t C07XFDAD;          /*!< MDMAC Channel 07 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C07XFSIZ;          /*!< MDMAC Channel 07 Transfer Size Avoidance Register*/
  __I  uint32_t C07DSADS;          /*!< MDMAC Channel 07 Descriptor Address Avoidance Register*/
  __I  uint32_t C07DSNUM;          /*!< MDMAC Channel 07 Descriptor Number Avoidance Register*/
       uint32_t RESERVED10[2];
  __I  uint32_t C08XFTYP;          /*!< MDMAC Channel 08 Number Avoidance Register   */
  __I  uint32_t C08XFSAD;          /*!< MDMAC Channel 08 Transfer Source Address Avoidance Register*/
  __I  uint32_t C08XFDAD;          /*!< MDMAC Channel 08 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C08XFSIZ;          /*!< MDMAC Channel 08 Transfer Size Avoidance Register*/
  __I  uint32_t C08DSADS;          /*!< MDMAC Channel 08 Descriptor Address Avoidance Register*/
  __I  uint32_t C08DSNUM;          /*!< MDMAC Channel 08 Descriptor Number Avoidance Register*/
       uint32_t RESERVED11[2];
  __I  uint32_t C09XFTYP;          /*!< MDMAC Channel 09 Number Avoidance Register   */
  __I  uint32_t C09XFSAD;          /*!< MDMAC Channel 09 Transfer Source Address Avoidance Register*/
  __I  uint32_t C09XFDAD;          /*!< MDMAC Channel 09 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C09XFSIZ;          /*!< MDMAC Channel 09 Transfer Size Avoidance Register*/
  __I  uint32_t C09DSADS;          /*!< MDMAC Channel 09 Descriptor Address Avoidance Register*/
  __I  uint32_t C09DSNUM;          /*!< MDMAC Channel 09 Descriptor Number Avoidance Register*/
       uint32_t RESERVED12[2];
  __I  uint32_t C10XFTYP;          /*!< MDMAC Channel 10 Number Avoidance Register   */
  __I  uint32_t C10XFSAD;          /*!< MDMAC Channel 10 Transfer Source Address Avoidance Register*/
  __I  uint32_t C10XFDAD;          /*!< MDMAC Channel 10 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C10XFSIZ;          /*!< MDMAC Channel 10 Transfer Size Avoidance Register*/
  __I  uint32_t C10DSADS;          /*!< MDMAC Channel 10 Descriptor Address Avoidance Register*/
  __I  uint32_t C10DSNUM;          /*!< MDMAC Channel 10 Descriptor Number Avoidance Register*/
       uint32_t RESERVED13[2];
  __I  uint32_t C11XFTYP;          /*!< MDMAC Channel 11 Number Avoidance Register   */
  __I  uint32_t C11XFSAD;          /*!< MDMAC Channel 11 Transfer Source Address Avoidance Register*/
  __I  uint32_t C11XFDAD;          /*!< MDMAC Channel 11 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C11XFSIZ;          /*!< MDMAC Channel 11 Transfer Size Avoidance Register*/
  __I  uint32_t C11DSADS;          /*!< MDMAC Channel 11 Descriptor Address Avoidance Register*/
  __I  uint32_t C11DSNUM;          /*!< MDMAC Channel 11 Descriptor Number Avoidance Register*/
       uint32_t RESERVED14[2];
  __I  uint32_t C12XFTYP;          /*!< MDMAC Channel 12 Number Avoidance Register   */
  __I  uint32_t C12XFSAD;          /*!< MDMAC Channel 12 Transfer Source Address Avoidance Register*/
  __I  uint32_t C12XFDAD;          /*!< MDMAC Channel 12 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C12XFSIZ;          /*!< MDMAC Channel 12 Transfer Size Avoidance Register*/
  __I  uint32_t C12DSADS;          /*!< MDMAC Channel 12 Descriptor Address Avoidance Register*/
  __I  uint32_t C12DSNUM;          /*!< MDMAC Channel 12 Descriptor Number Avoidance Register*/
       uint32_t RESERVED15[2];
  __I  uint32_t C13XFTYP;          /*!< MDMAC Channel 13 Number Avoidance Register   */
  __I  uint32_t C13XFSAD;          /*!< MDMAC Channel 13 Transfer Source Address Avoidance Register*/
  __I  uint32_t C13XFDAD;          /*!< MDMAC Channel 13 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C13XFSIZ;          /*!< MDMAC Channel 13 Transfer Size Avoidance Register*/
  __I  uint32_t C13DSADS;          /*!< MDMAC Channel 13 Descriptor Address Avoidance Register*/
  __I  uint32_t C13DSNUM;          /*!< MDMAC Channel 13 Descriptor Number Avoidance Register*/
       uint32_t RESERVED16[2];
  __I  uint32_t C14XFTYP;          /*!< MDMAC Channel 14 Number Avoidance Register   */
  __I  uint32_t C14XFSAD;          /*!< MDMAC Channel 14 Transfer Source Address Avoidance Register*/
  __I  uint32_t C14XFDAD;          /*!< MDMAC Channel 14 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C14XFSIZ;          /*!< MDMAC Channel 14 Transfer Size Avoidance Register*/
  __I  uint32_t C14DSADS;          /*!< MDMAC Channel 14 Descriptor Address Avoidance Register*/
  __I  uint32_t C14DSNUM;          /*!< MDMAC Channel 14 Descriptor Number Avoidance Register*/
       uint32_t RESERVED17[2];
  __I  uint32_t C15XFTYP;          /*!< MDMAC Channel 15 Number Avoidance Register   */
  __I  uint32_t C15XFSAD;          /*!< MDMAC Channel 15 Transfer Source Address Avoidance Register*/
  __I  uint32_t C15XFDAD;          /*!< MDMAC Channel 15 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C15XFSIZ;          /*!< MDMAC Channel 15 Transfer Size Avoidance Register*/
  __I  uint32_t C15DSADS;          /*!< MDMAC Channel 15 Descriptor Address Avoidance Register*/
  __I  uint32_t C15DSNUM;          /*!< MDMAC Channel 15 Descriptor Number Avoidance Register*/
       uint32_t RESERVED18[2];
  __I  uint32_t C16XFTYP;          /*!< MDMAC Channel 16 Number Avoidance Register   */
  __I  uint32_t C16XFSAD;          /*!< MDMAC Channel 16 Transfer Source Address Avoidance Register*/
  __I  uint32_t C16XFDAD;          /*!< MDMAC Channel 16 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C16XFSIZ;          /*!< MDMAC Channel 16 Transfer Size Avoidance Register*/
  __I  uint32_t C16DSADS;          /*!< MDMAC Channel 16 Descriptor Address Avoidance Register*/
  __I  uint32_t C16DSNUM;          /*!< MDMAC Channel 16 Descriptor Number Avoidance Register*/
       uint32_t RESERVED19[2];
  __I  uint32_t C17XFTYP;          /*!< MDMAC Channel 17 Number Avoidance Register   */
  __I  uint32_t C17XFSAD;          /*!< MDMAC Channel 17 Transfer Source Address Avoidance Register*/
  __I  uint32_t C17XFDAD;          /*!< MDMAC Channel 17 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C17XFSIZ;          /*!< MDMAC Channel 17 Transfer Size Avoidance Register*/
  __I  uint32_t C17DSADS;          /*!< MDMAC Channel 17 Descriptor Address Avoidance Register*/
  __I  uint32_t C17DSNUM;          /*!< MDMAC Channel 17 Descriptor Number Avoidance Register*/
       uint32_t RESERVED20[2];
  __I  uint32_t C18XFTYP;          /*!< MDMAC Channel 18 Number Avoidance Register   */
  __I  uint32_t C18XFSAD;          /*!< MDMAC Channel 18 Transfer Source Address Avoidance Register*/
  __I  uint32_t C18XFDAD;          /*!< MDMAC Channel 18 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C18XFSIZ;          /*!< MDMAC Channel 18 Transfer Size Avoidance Register*/
  __I  uint32_t C18DSADS;          /*!< MDMAC Channel 18 Descriptor Address Avoidance Register*/
  __I  uint32_t C18DSNUM;          /*!< MDMAC Channel 18 Descriptor Number Avoidance Register*/
       uint32_t RESERVED21[2];
  __I  uint32_t C19XFTYP;          /*!< MDMAC Channel 19 Number Avoidance Register   */
  __I  uint32_t C19XFSAD;          /*!< MDMAC Channel 19 Transfer Source Address Avoidance Register*/
  __I  uint32_t C19XFDAD;          /*!< MDMAC Channel 19 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C19XFSIZ;          /*!< MDMAC Channel 19 Transfer Size Avoidance Register*/
  __I  uint32_t C19DSADS;          /*!< MDMAC Channel 19 Descriptor Address Avoidance Register*/
  __I  uint32_t C19DSNUM;          /*!< MDMAC Channel 19 Descriptor Number Avoidance Register*/
       uint32_t RESERVED22[2];
  __I  uint32_t C20XFTYP;          /*!< MDMAC Channel 20 Number Avoidance Register   */
  __I  uint32_t C20XFSAD;          /*!< MDMAC Channel 20 Transfer Source Address Avoidance Register*/
  __I  uint32_t C20XFDAD;          /*!< MDMAC Channel 20 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C20XFSIZ;          /*!< MDMAC Channel 20 Transfer Size Avoidance Register*/
  __I  uint32_t C20DSADS;          /*!< MDMAC Channel 20 Descriptor Address Avoidance Register*/
  __I  uint32_t C20DSNUM;          /*!< MDMAC Channel 20 Descriptor Number Avoidance Register*/
       uint32_t RESERVED23[2];
  __I  uint32_t C21XFTYP;          /*!< MDMAC Channel 21 Number Avoidance Register   */
  __I  uint32_t C21XFSAD;          /*!< MDMAC Channel 21 Transfer Source Address Avoidance Register*/
  __I  uint32_t C21XFDAD;          /*!< MDMAC Channel 21 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C21XFSIZ;          /*!< MDMAC Channel 21 Transfer Size Avoidance Register*/
  __I  uint32_t C21DSADS;          /*!< MDMAC Channel 21 Descriptor Address Avoidance Register*/
  __I  uint32_t C21DSNUM;          /*!< MDMAC Channel 21 Descriptor Number Avoidance Register*/
       uint32_t RESERVED24[2];
  __I  uint32_t C22XFTYP;          /*!< MDMAC Channel 22 Number Avoidance Register   */
  __I  uint32_t C22XFSAD;          /*!< MDMAC Channel 22 Transfer Source Address Avoidance Register*/
  __I  uint32_t C22XFDAD;          /*!< MDMAC Channel 22 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C22XFSIZ;          /*!< MDMAC Channel 22 Transfer Size Avoidance Register*/
  __I  uint32_t C22DSADS;          /*!< MDMAC Channel 22 Descriptor Address Avoidance Register*/
  __I  uint32_t C22DSNUM;          /*!< MDMAC Channel 22 Descriptor Number Avoidance Register*/
       uint32_t RESERVED25[2];
  __I  uint32_t C23XFTYP;          /*!< MDMAC Channel 23 Number Avoidance Register   */
  __I  uint32_t C23XFSAD;          /*!< MDMAC Channel 23 Transfer Source Address Avoidance Register*/
  __I  uint32_t C23XFDAD;          /*!< MDMAC Channel 23 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C23XFSIZ;          /*!< MDMAC Channel 23 Transfer Size Avoidance Register*/
  __I  uint32_t C23DSADS;          /*!< MDMAC Channel 23 Descriptor Address Avoidance Register*/
  __I  uint32_t C23DSNUM;          /*!< MDMAC Channel 23 Descriptor Number Avoidance Register*/
       uint32_t RESERVED26[2];
  __I  uint32_t C24XFTYP;          /*!< MDMAC Channel 24 Number Avoidance Register   */
  __I  uint32_t C24XFSAD;          /*!< MDMAC Channel 24 Transfer Source Address Avoidance Register*/
  __I  uint32_t C24XFDAD;          /*!< MDMAC Channel 24 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C24XFSIZ;          /*!< MDMAC Channel 24 Transfer Size Avoidance Register*/
  __I  uint32_t C24DSADS;          /*!< MDMAC Channel 24 Descriptor Address Avoidance Register*/
  __I  uint32_t C24DSNUM;          /*!< MDMAC Channel 24 Descriptor Number Avoidance Register*/
       uint32_t RESERVED27[2];
  __I  uint32_t C25XFTYP;          /*!< MDMAC Channel 25 Number Avoidance Register   */
  __I  uint32_t C25XFSAD;          /*!< MDMAC Channel 25 Transfer Source Address Avoidance Register*/
  __I  uint32_t C25XFDAD;          /*!< MDMAC Channel 25 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C25XFSIZ;          /*!< MDMAC Channel 25 Transfer Size Avoidance Register*/
  __I  uint32_t C25DSADS;          /*!< MDMAC Channel 25 Descriptor Address Avoidance Register*/
  __I  uint32_t C25DSNUM;          /*!< MDMAC Channel 25 Descriptor Number Avoidance Register*/
       uint32_t RESERVED28[2];
  __I  uint32_t C26XFTYP;          /*!< MDMAC Channel 26 Number Avoidance Register   */
  __I  uint32_t C26XFSAD;          /*!< MDMAC Channel 26 Transfer Source Address Avoidance Register*/
  __I  uint32_t C26XFDAD;          /*!< MDMAC Channel 26 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C26XFSIZ;          /*!< MDMAC Channel 26 Transfer Size Avoidance Register*/
  __I  uint32_t C26DSADS;          /*!< MDMAC Channel 26 Descriptor Address Avoidance Register*/
  __I  uint32_t C26DSNUM;          /*!< MDMAC Channel 26 Descriptor Number Avoidance Register*/
       uint32_t RESERVED29[2];
  __I  uint32_t C27XFTYP;          /*!< MDMAC Channel 27 Number Avoidance Register   */
  __I  uint32_t C27XFSAD;          /*!< MDMAC Channel 27 Transfer Source Address Avoidance Register*/
  __I  uint32_t C27XFDAD;          /*!< MDMAC Channel 27 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C27XFSIZ;          /*!< MDMAC Channel 27 Transfer Size Avoidance Register*/
  __I  uint32_t C27DSADS;          /*!< MDMAC Channel 27 Descriptor Address Avoidance Register*/
  __I  uint32_t C27DSNUM;          /*!< MDMAC Channel 27 Descriptor Number Avoidance Register*/
       uint32_t RESERVED30[2];
  __I  uint32_t C28XFTYP;          /*!< MDMAC Channel 28 Number Avoidance Register   */
  __I  uint32_t C28XFSAD;          /*!< MDMAC Channel 28 Transfer Source Address Avoidance Register*/
  __I  uint32_t C28XFDAD;          /*!< MDMAC Channel 28 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C28XFSIZ;          /*!< MDMAC Channel 28 Transfer Size Avoidance Register*/
  __I  uint32_t C28DSADS;          /*!< MDMAC Channel 28 Descriptor Address Avoidance Register*/
  __I  uint32_t C28DSNUM;          /*!< MDMAC Channel 28 Descriptor Number Avoidance Register*/
       uint32_t RESERVED31[2];
  __I  uint32_t C29XFTYP;          /*!< MDMAC Channel 29 Number Avoidance Register   */
  __I  uint32_t C29XFSAD;          /*!< MDMAC Channel 29 Transfer Source Address Avoidance Register*/
  __I  uint32_t C29XFDAD;          /*!< MDMAC Channel 29 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C29XFSIZ;          /*!< MDMAC Channel 29 Transfer Size Avoidance Register*/
  __I  uint32_t C29DSADS;          /*!< MDMAC Channel 29 Descriptor Address Avoidance Register*/
  __I  uint32_t C29DSNUM;          /*!< MDMAC Channel 29 Descriptor Number Avoidance Register*/
       uint32_t RESERVED32[2];
  __I  uint32_t C30XFTYP;          /*!< MDMAC Channel 30 Number Avoidance Register   */
  __I  uint32_t C30XFSAD;          /*!< MDMAC Channel 30 Transfer Source Address Avoidance Register*/
  __I  uint32_t C30XFDAD;          /*!< MDMAC Channel 30 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C30XFSIZ;          /*!< MDMAC Channel 30 Transfer Size Avoidance Register*/
  __I  uint32_t C30DSADS;          /*!< MDMAC Channel 30 Descriptor Address Avoidance Register*/
  __I  uint32_t C30DSNUM;          /*!< MDMAC Channel 30 Descriptor Number Avoidance Register*/
       uint32_t RESERVED33[2];
  __I  uint32_t C31XFTYP;          /*!< MDMAC Channel 31 Number Avoidance Register   */
  __I  uint32_t C31XFSAD;          /*!< MDMAC Channel 31 Transfer Source Address Avoidance Register*/
  __I  uint32_t C31XFDAD;          /*!< MDMAC Channel 31 Transfer Destination Address Avoidance Register*/
  __I  uint32_t C31XFSIZ;          /*!< MDMAC Channel 31 Transfer Size Avoidance Register*/
  __I  uint32_t C31DSADS;          /*!< MDMAC Channel 31 Descriptor Address Avoidance Register*/
  __I  uint32_t C31DSNUM;          /*!< MDMAC Channel 31 Descriptor Number Avoidance Register*/
       uint32_t RESERVED34[238];
  __IO uint32_t MSK;               /*!< MDMAC Mask Register                          */
} TSB_MDMA_TypeDef;

#if defined ( __CC_ARM   )           /* RealView Compiler */
#pragma anon_unions
#elif (defined (__ICCARM__))         /*  ICC Compiler     */
#pragma language=extended
#endif

/**
  * @brief ARM Prime Cell PL011
  */
typedef struct
{
  __IO uint32_t DR;                /*!< Data Register                                */
union {
  __I  uint32_t RSR;               /*!< Receive Status Register                      */
  __O  uint32_t ECR;               /*!< Error Clear Register                         */
  };
       uint32_t RESERVED0[4];
  __I  uint32_t FR;                /*!< Flag Register                                */
       uint32_t RESERVED1;
  __IO uint32_t ILPR;              /*!< IrDA Low-power Counter register              */
  __IO uint32_t BRD;               /*!< Integer Baud Rate Register                   */
  __IO uint32_t FBRD;              /*!< Fractional Baud Rate Register                */
  __IO uint32_t LCR_H;             /*!< Line Control Register                        */
  __IO uint32_t CR;                /*!< Cntrol Register                              */
  __IO uint32_t IFLS;              /*!< Interrupt FIFO Level Select Register         */
  __IO uint32_t IMSC;              /*!< Interrupt Mask set/Clear Register            */
  __I  uint32_t RIS;               /*!< Raw Interrupt Status Register                */
  __I  uint32_t MIS;               /*!< Masked Interrupt Status Register             */
  __O  uint32_t ICR;               /*!< Interrupt Clear Register                     */
  __IO uint32_t DMACR;             /*!< DMA Control Register                         */
} TSB_FURT_TypeDef;

/**
  * @brief ADC
  */
typedef struct
{
  __IO uint32_t CR0;               /*!< AD Control Register 0                        */
  __IO uint32_t CR1;               /*!< AD Control Register 1                        */
  __I  uint32_t ST;                /*!< AD Status Register                           */
  __IO uint32_t CLK;               /*!< AD Conversion Clock Setting Register         */
  __IO uint32_t MOD0;              /*!< AD Mode Control Register 0                   */
  __IO uint32_t MOD1;              /*!< AD Mode Control Register 1                   */
  __IO uint32_t MOD2;              /*!< AD Mode Control Register 2                   */
       uint32_t RESERVED0;
  __IO uint32_t CMPEN;             /*!< AD Monitoring interrupt permission register  */
  __IO uint32_t CMPCR0;            /*!< AD Monitoring Setting Register 0             */
  __IO uint32_t CMPCR1;            /*!< AD Monitoring Setting Register 1             */
  __IO uint32_t CMP0;              /*!< AD Conversion Result Comparison Register 0   */
  __IO uint32_t CMP1;              /*!< AD Conversion Result Comparison Register 1   */
  __IO uint32_t CMPCR2;            /*!< AD Conversion Monitor Function setting Register 2*/
  __IO uint32_t CMPCR3;            /*!< AD Conversion Monitor Function setting Register 3*/
  __IO uint32_t CMP2;              /*!< AD Conversion Result Comparison Register 2   */
  __IO uint32_t CMP3;              /*!< AD Conversion Result Comparison Register 3   */
       uint32_t RESERVED1[30];
  __IO uint32_t EXAZSEL;           /*!< AIN sampling period selection register       */
  __IO uint32_t TSET0;             /*!< AD General purpose Trigger Program Register 0*/
  __IO uint32_t TSET1;             /*!< AD General purpose Trigger Program Register 1*/
  __IO uint32_t TSET2;             /*!< AD General purpose Trigger Program Register 2*/
  __IO uint32_t TSET3;             /*!< AD General purpose Trigger Program Register 3*/
  __IO uint32_t TSET4;             /*!< AD General purpose Trigger Program Register 4*/
  __IO uint32_t TSET5;             /*!< AD General purpose Trigger Program Register 5*/
  __IO uint32_t TSET6;             /*!< AD General purpose Trigger Program Register 6*/
  __IO uint32_t TSET7;             /*!< AD General purpose Trigger Program Register 7*/
  __IO uint32_t TSET8;             /*!< AD General purpose Trigger Program Register 8*/
  __IO uint32_t TSET9;             /*!< AD General purpose Trigger Program Register 9*/
  __IO uint32_t TSET10;            /*!< AD General purpose Trigger Program Register 10*/
  __IO uint32_t TSET11;            /*!< AD General purpose Trigger Program Register 11*/
  __IO uint32_t TSET12;            /*!< AD General purpose Trigger Program Register 12*/
  __IO uint32_t TSET13;            /*!< AD General purpose Trigger Program Register 13*/
  __IO uint32_t TSET14;            /*!< AD General purpose Trigger Program Register 14*/
  __IO uint32_t TSET15;            /*!< AD General purpose Trigger Program Register 15*/
  __IO uint32_t TSET16;            /*!< AD General purpose Trigger Program Register 16*/
  __IO uint32_t TSET17;            /*!< AD General purpose Trigger Program Register 17*/
  __IO uint32_t TSET18;            /*!< AD General purpose Trigger Program Register 18*/
  __IO uint32_t TSET19;            /*!< AD General purpose Trigger Program Register 19*/
  __IO uint32_t TSET20;            /*!< AD General purpose Trigger Program Register 20*/
  __IO uint32_t TSET21;            /*!< AD General purpose Trigger Program Register 21*/
  __IO uint32_t TSET22;            /*!< AD General purpose Trigger Program Register 22*/
  __IO uint32_t TSET23;            /*!< AD General purpose Trigger Program Register 23*/
       uint32_t RESERVED2[8];
  __I  uint32_t REG0;              /*!< AD AD Conversion Result Register 0           */
  __I  uint32_t REG1;              /*!< AD Conversion Result Register 1              */
  __I  uint32_t REG2;              /*!< AD Conversion Result Register 2              */
  __I  uint32_t REG3;              /*!< AD Conversion Result Register 3              */
  __I  uint32_t REG4;              /*!< AD Conversion Result Register 4              */
  __I  uint32_t REG5;              /*!< AD Conversion Result Register 5              */
  __I  uint32_t REG6;              /*!< AD Conversion Result Register 6              */
  __I  uint32_t REG7;              /*!< AD Conversion Result Register 7              */
  __I  uint32_t REG8;              /*!< AD Conversion Result Register 8              */
  __I  uint32_t REG9;              /*!< AD Conversion Result Register 9              */
  __I  uint32_t REG10;             /*!< AD Conversion Result Register 10             */
  __I  uint32_t REG11;             /*!< AD Conversion Result Register 11             */
  __I  uint32_t REG12;             /*!< AD Conversion Result Register 12             */
  __I  uint32_t REG13;             /*!< AD Conversion Result Register 13             */
  __I  uint32_t REG14;             /*!< AD Conversion Result Register 14             */
  __I  uint32_t REG15;             /*!< AD Conversion Result Register 15             */
  __I  uint32_t REG16;             /*!< AD Conversion Result Register 16             */
  __I  uint32_t REG17;             /*!< AD Conversion Result Register 17             */
  __I  uint32_t REG18;             /*!< AD Conversion Result Register 18             */
  __I  uint32_t REG19;             /*!< AD Conversion Result Register 19             */
  __I  uint32_t REG20;             /*!< AD Conversion Result Register 20             */
  __I  uint32_t REG21;             /*!< AD Conversion Result Register 21             */
  __I  uint32_t REG22;             /*!< AD Conversion Result Register 22             */
  __I  uint32_t REG23;             /*!< AD Conversion Result Register 23             */
} TSB_AD_TypeDef;

/**
  * @brief Digital analog converter (DAC)
  */
typedef struct
{
  __IO uint32_t CR;                /*!< DAC Control Register                         */
  __IO uint32_t REG;               /*!< DAC output Register                          */
} TSB_DA_TypeDef;

/**
  * @brief 16-bit Timer/Event Counter (TB)
  */
typedef struct
{
  __IO uint32_t MOD;               /*!< T32A Mode Register                           */
       uint32_t RESERVED0[15];
  __IO uint32_t RUNA;              /*!< T32A Run Register A                          */
  __IO uint32_t CRA;               /*!< T32A Counter Control Register A              */
  __IO uint32_t CAPCRA;            /*!< T32A Capture Control Register A              */
  __O  uint32_t OUTCRA0;           /*!< T32A Output Control Register A0              */
  __IO uint32_t OUTCRA1;           /*!< T32A Output Control Register A1              */
  __IO uint32_t STA;               /*!< T32A Status Register A                       */
  __IO uint32_t IMA;               /*!< T32A Interrupt Mask Register A               */
  __I  uint32_t TMRA;              /*!< T32A Counter Capture Register A              */
  __IO uint32_t RELDA;             /*!< T32A Counter Reload Register A               */
  __IO uint32_t RGA0;              /*!< T32A Timer Register A0                       */
  __IO uint32_t RGA1;              /*!< T32A Timer Register A1                       */
  __I  uint32_t CAPA0;             /*!< T32A Timer Capture A0 Register               */
  __I  uint32_t CAPA1;             /*!< T32A Timer Cupture A1 Register               */
  __IO uint32_t DMAA;              /*!< T32A DMA Request Enable Register A           */
       uint32_t RESERVED1[2];
  __IO uint32_t RUNB;              /*!< T32A Run Register B                          */
  __IO uint32_t CRB;               /*!< T32A Counter Control Register B              */
  __IO uint32_t CAPCRB;            /*!< T32A Capture Control Register B              */
  __O  uint32_t OUTCRB0;           /*!< T32A Output Control Register B0              */
  __IO uint32_t OUTCRB1;           /*!< T32A Output Control Register B1              */
  __IO uint32_t STB;               /*!< T32A Status Register B                       */
  __IO uint32_t IMB;               /*!< T32A Interrupt Mask Register B               */
  __I  uint32_t TMRB;              /*!< T32A Counter Capture Register B              */
  __IO uint32_t RELDB;             /*!< T32A Counter Reload Register B               */
  __IO uint32_t RGB0;              /*!< T32A Timer Register B0                       */
  __IO uint32_t RGB1;              /*!< T32A Timer Register B1                       */
  __I  uint32_t CAPB0;             /*!< T32A Timer Capture B0 Register               */
  __I  uint32_t CAPB1;             /*!< T32A Timer Capture B1 Register               */
  __IO uint32_t DMAB;              /*!< T32A DMA Request Enable Register B           */
       uint32_t RESERVED2[2];
  __IO uint32_t RUNC;              /*!< T32A Run Register C                          */
  __IO uint32_t CRC;               /*!< T32A Counter Control Register C              */
  __IO uint32_t CAPCRC;            /*!< T32A Capture Control Register C              */
  __O  uint32_t OUTCRC0;           /*!< T32A Output Control Register C0              */
  __IO uint32_t OUTCRC1;           /*!< T32A Output Control Register C1              */
  __IO uint32_t STC;               /*!< T32A Status Register C                       */
  __IO uint32_t IMC;               /*!< T32A Interrupt Mask Register C               */
  __I  uint32_t TMRC;              /*!< T32A Counter Capture Register C              */
  __IO uint32_t RELDC;             /*!< T32A Counter Reload Register C               */
  __IO uint32_t RGC0;              /*!< T32A Timer Register C0                       */
  __IO uint32_t RGC1;              /*!< T32A Timer Register C1                       */
  __I  uint32_t CAPC0;             /*!< T32A Timer Capture C0 Register               */
  __I  uint32_t CAPC1;             /*!< T32A Capture Register C1                     */
  __IO uint32_t DMAC;              /*!< T32A DMA Request Enable Register C           */
  __IO uint32_t PLSCR;             /*!< T32A Pulse Count Control Register            */
} TSB_T32A_TypeDef;

/**
  * @brief UART
  */
typedef struct
{
  __IO uint32_t SWRST;             /*!< UART Software Reset Register                 */
  __IO uint32_t CR0;               /*!< UART Control Register 0                      */
  __IO uint32_t CR1;               /*!< UART Control Register 1                      */
  __IO uint32_t CLK;               /*!< UART Clock Control Register                  */
  __IO uint32_t BRD;               /*!< UART Baud Rate Register                      */
  __IO uint32_t TRANS;             /*!< UART Transfer Enable Register                */
  __IO uint32_t DR;                /*!< UART Data Register                           */
  __IO uint32_t SR;                /*!< UART Status Register                         */
  __O  uint32_t FIFOCLR;           /*!< UART FIFO Clear Register                     */
  __IO uint32_t ERR;               /*!< UART Error Register                          */
} TSB_UART_TypeDef;

/**
  * @brief I2C 
  */
typedef struct
{
  __IO uint32_t CR1;               /*!< I2C Control Register 1                       */
  __IO uint32_t DBR;               /*!< I2C Data Buffer Register                     */
  __IO uint32_t AR;                /*!< I2C Bus address Register                     */
union {
  __O  uint32_t CR2;               /*!< I2C Control Register 2                       */
  __I  uint32_t SR;                /*!< I2C Status Register                          */
  };
  __IO uint32_t PRS;               /*!< I2C Prescaler clcok setting Register         */
  __IO uint32_t IE;                /*!< I2C Interrupt Enable Register                */
  __IO uint32_t ST;                /*!< Interrupt Register                           */
  __IO uint32_t OP;                /*!< Optiononal Function register                 */
  __I  uint32_t PM;                /*!< Bus Monitor register                         */
  __IO uint32_t AR2;               /*!< Second Slave address register                */
} TSB_I2C_TypeDef;

/**
  * @brief Port A
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port A Data Register                         */
  __IO uint32_t CR;                /*!< Port A Output Control Register               */
  __IO uint32_t FR1;               /*!< Port A Function Register 1                   */
  __IO uint32_t FR2;               /*!< Port A Function Register 2                   */
  __IO uint32_t FR3;               /*!< Port A Function Register 3                   */
       uint32_t RESERVED0;
  __IO uint32_t FR5;               /*!< Port A Function Register 5                   */
  __IO uint32_t FR6;               /*!< Port A Function Register 6                   */
  __IO uint32_t FR7;               /*!< Port A Function Register 7                   */
       uint32_t RESERVED1;
  __IO uint32_t OD;                /*!< Port A Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port A Pull-up Control Register              */
  __IO uint32_t PDN;               /*!< Port A Pull-down Control Register            */
       uint32_t RESERVED2;
  __IO uint32_t IE;                /*!< Port A Input Control Register                */
} TSB_PA_TypeDef;

/**
  * @brief Port B
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port B Data Register                         */
  __IO uint32_t CR;                /*!< Port B Output Control Register               */
  __IO uint32_t FR1;               /*!< Port B Function Register 1                   */
  __IO uint32_t FR2;               /*!< Port B Function Register 2                   */
  __IO uint32_t FR3;               /*!< Port B Function Register 3                   */
       uint32_t RESERVED0;
  __IO uint32_t FR5;               /*!< Port B Function Register 5                   */
  __IO uint32_t FR6;               /*!< Port B Function Register 6                   */
       uint32_t RESERVED1[2];
  __IO uint32_t OD;                /*!< Port B Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port B Pull-up Control Register              */
  __IO uint32_t PDN;               /*!< Port B Pull-down Control Register            */
       uint32_t RESERVED2;
  __IO uint32_t IE;                /*!< Port B Input Control Register                */
} TSB_PB_TypeDef;

/**
  * @brief Port C
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port C Data Register                         */
  __IO uint32_t CR;                /*!< Port C Output Control Register               */
  __IO uint32_t FR1;               /*!< Port C Function Register 1                   */
       uint32_t RESERVED0;
  __IO uint32_t FR3;               /*!< Port C Function Register 3                   */
       uint32_t RESERVED1;
  __IO uint32_t FR5;               /*!< Port C Function Register 5                   */
       uint32_t RESERVED2[3];
  __IO uint32_t OD;                /*!< Port C Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port C Pull-up Control Register              */
  __IO uint32_t PDN;               /*!< Port C Pull-down Control Register            */
       uint32_t RESERVED3;
  __IO uint32_t IE;                /*!< Port C Input Control Register                */
} TSB_PC_TypeDef;

/**
  * @brief Port D
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port D Data Register                         */
  __IO uint32_t CR;                /*!< Port D Output Control Register               */
  __IO uint32_t FR1;               /*!< Port D Function Register 1                   */
  __IO uint32_t FR2;               /*!< Port D Function Register 2                   */
  __IO uint32_t FR3;               /*!< Port D Function Register 3                   */
  __IO uint32_t FR4;               /*!< Port D Function Register 4                   */
  __IO uint32_t FR5;               /*!< Port D Function Register 5                   */
  __IO uint32_t FR6;               /*!< Port D Function Register 6                   */
  __IO uint32_t FR7;               /*!< Port D Function Register 7                   */
       uint32_t RESERVED0;
  __IO uint32_t OD;                /*!< Port D Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port D Pull-up Control Register              */
  __IO uint32_t PDN;               /*!< Port D Pull-down Control Register            */
       uint32_t RESERVED1;
  __IO uint32_t IE;                /*!< Port D Input Control Register                */
} TSB_PD_TypeDef;

/**
  * @brief Port E
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port E Data Register                         */
  __IO uint32_t CR;                /*!< Port E Output Control Register               */
  __IO uint32_t FR1;               /*!< Port E Function Register 1                   */
  __IO uint32_t FR2;               /*!< Port E Function Register 2                   */
  __IO uint32_t FR3;               /*!< Port E Function Register 3                   */
  __IO uint32_t FR4;               /*!< Port E Function Register 4                   */
  __IO uint32_t FR5;               /*!< Port E Function Register 5                   */
       uint32_t RESERVED0;
  __IO uint32_t FR7;               /*!< Port E Function Register 7                   */
       uint32_t RESERVED1;
  __IO uint32_t OD;                /*!< Port E Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port E Pull-up Control Register              */
  __IO uint32_t PDN;               /*!< Port E Pull-down Control Register            */
       uint32_t RESERVED2;
  __IO uint32_t IE;                /*!< Port E Input Control Register                */
} TSB_PE_TypeDef;

/**
  * @brief Port F
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port F Data Register                         */
  __IO uint32_t CR;                /*!< Port F Output Control Register               */
  __IO uint32_t FR1;               /*!< Port F Function Register 1                   */
       uint32_t RESERVED0[5];
  __IO uint32_t FR7;               /*!< Port F Function Register 7                   */
       uint32_t RESERVED1;
  __IO uint32_t OD;                /*!< Port F Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port F Pull-up Control Register              */
  __IO uint32_t PDN;               /*!< Port F Pull-down Control Register            */
       uint32_t RESERVED2;
  __IO uint32_t IE;                /*!< Port F Input Control Register                */
} TSB_PF_TypeDef;

/**
  * @brief Port G
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port G Data Register                         */
  __IO uint32_t CR;                /*!< Port G Output Control Register               */
  __IO uint32_t FR1;               /*!< Port G Function Register 1                   */
  __IO uint32_t FR2;               /*!< Port G Function Register 2                   */
  __IO uint32_t FR3;               /*!< Port G Function Register 3                   */
  __IO uint32_t FR4;               /*!< Port G Function Register 4                   */
  __IO uint32_t FR5;               /*!< Port G Function Register 5                   */
       uint32_t RESERVED0;
  __IO uint32_t FR7;               /*!< Port G Function Register 7                   */
       uint32_t RESERVED1;
  __IO uint32_t OD;                /*!< Port G Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port G Pull-up Control Register              */
  __IO uint32_t PDN;               /*!< Port G Pull-down Control Register            */
       uint32_t RESERVED2;
  __IO uint32_t IE;                /*!< Port G Input Control Register                */
} TSB_PG_TypeDef;

/**
  * @brief Port H
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port H Data Register                         */
  __IO uint32_t CR;                /*!< Port H Output Control Register               */
  __IO uint32_t FR1;               /*!< Port H Function Register 1                   */
       uint32_t RESERVED0;
  __IO uint32_t FR3;               /*!< Port H Function Register 3                   */
  __IO uint32_t FR4;               /*!< Port H Function Register 4                   */
  __IO uint32_t FR5;               /*!< Port H Function Register 5                   */
       uint32_t RESERVED1[3];
  __IO uint32_t OD;                /*!< Port H Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port H Pull-up Control Register              */
  __IO uint32_t PDN;               /*!< Port H Pull-down Control Register            */
       uint32_t RESERVED2;
  __IO uint32_t IE;                /*!< Port H Input Control Register                */
} TSB_PH_TypeDef;

/**
  * @brief Port J
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port J Data Register                         */
  __IO uint32_t CR;                /*!< Port J Output Control Register               */
       uint32_t RESERVED0;
  __IO uint32_t FR2;               /*!< Port J Function Register 2                   */
  __IO uint32_t FR3;               /*!< Port J Function Register 3                   */
       uint32_t RESERVED1;
  __IO uint32_t FR5;               /*!< Port J Function Register 5                   */
       uint32_t RESERVED2;
  __IO uint32_t FR7;               /*!< Port J Function Register 7                   */
       uint32_t RESERVED3;
  __IO uint32_t OD;                /*!< Port J Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port J Pull-up Control Register              */
  __IO uint32_t PDN;               /*!< Port J Pull-down Control Register            */
       uint32_t RESERVED4;
  __IO uint32_t IE;                /*!< Port J Input Control Register                */
} TSB_PJ_TypeDef;

/**
  * @brief Port K
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port K Data Register                         */
  __IO uint32_t CR;                /*!< Port K Output Control Register               */
  __IO uint32_t FR1;               /*!< Port K Function Register 1                   */
  __IO uint32_t FR2;               /*!< Port K Function Register 2                   */
  __IO uint32_t FR3;               /*!< Port K Function Register 3                   */
  __IO uint32_t FR4;               /*!< Port K Function Register 4                   */
       uint32_t RESERVED0;
  __IO uint32_t FR6;               /*!< Port K Function Register 6                   */
  __IO uint32_t FR7;               /*!< Port K Function Register 7                   */
       uint32_t RESERVED1;
  __IO uint32_t OD;                /*!< Port K Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port K Pull-up Control Register              */
  __IO uint32_t PDN;               /*!< Port K Pull-up Control Register              */
       uint32_t RESERVED2;
  __IO uint32_t IE;                /*!< Port K Input Control Register                */
} TSB_PK_TypeDef;

/**
  * @brief Port L
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port L Data Register                         */
  __IO uint32_t CR;                /*!< Port L Output Control Register               */
  __IO uint32_t FR1;               /*!< Port L Function Register 1                   */
  __IO uint32_t FR2;               /*!< Port L Function Register 2                   */
  __IO uint32_t FR3;               /*!< Port L Function Register 3                   */
       uint32_t RESERVED0[2];
  __IO uint32_t FR6;               /*!< Port L Function Register 6                   */
  __IO uint32_t FR7;               /*!< Port L Function Register 7                   */
       uint32_t RESERVED1;
  __IO uint32_t OD;                /*!< Port L Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port L Pull-up Control Register              */
  __IO uint32_t PDN;               /*!< Port L Pull-down Control Register            */
       uint32_t RESERVED2;
  __IO uint32_t IE;                /*!< Port L Input Control Register                */
} TSB_PL_TypeDef;

/**
  * @brief Port M
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port M Data Register                         */
  __IO uint32_t CR;                /*!< Port M Output Control Register               */
       uint32_t RESERVED0;
  __IO uint32_t FR2;               /*!< Port M Function Register 2                   */
  __IO uint32_t FR3;               /*!< Port M Function Register 3                   */
  __IO uint32_t FR4;               /*!< Port M Function Register 4                   */
  __IO uint32_t FR5;               /*!< Port M Function Register 5                   */
  __IO uint32_t FR6;               /*!< Port M Function Register 6                   */
  __IO uint32_t FR7;               /*!< Port M Function Register 7                   */
       uint32_t RESERVED1;
  __IO uint32_t OD;                /*!< Port M Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port M Pull-up Control Register              */
  __IO uint32_t PDN;               /*!< Port M Pull-up Control Register              */
       uint32_t RESERVED2;
  __IO uint32_t IE;                /*!< Port M Input Control Register                */
} TSB_PM_TypeDef;

/**
  * @brief Port N
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port N Data Register                         */
  __IO uint32_t CR;                /*!< Port N Output Control Register               */
       uint32_t RESERVED0[8];
  __IO uint32_t OD;                /*!< Port N Opend Drain Control Register          */
  __IO uint32_t PUP;               /*!< Port N Pull-up Control Register              */
  __IO uint32_t PDN;               /*!< Port N Pull-down Control Register            */
       uint32_t RESERVED1;
  __IO uint32_t IE;                /*!< Port N Input Control Register                */
} TSB_PN_TypeDef;

/**
  * @brief Port P
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port P Data Register                         */
  __IO uint32_t CR;                /*!< Port P Output Control Register               */
       uint32_t RESERVED0;
  __IO uint32_t FR2;               /*!< Port P Function Register 2                   */
  __IO uint32_t FR3;               /*!< Port P Function Register 3                   */
       uint32_t RESERVED1;
  __IO uint32_t FR5;               /*!< Port P Function Register 5                   */
       uint32_t RESERVED2[3];
  __IO uint32_t OD;                /*!< Port P Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port P Pull-up Control Register              */
  __IO uint32_t PDN;               /*!< Port P Pull-down Control Register            */
       uint32_t RESERVED3;
  __IO uint32_t IE;                /*!< Port P Input Control Register                */
} TSB_PP_TypeDef;

/**
  * @brief Port R
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port R Data Register                         */
  __IO uint32_t CR;                /*!< Port R Output Control Register               */
       uint32_t RESERVED0;
  __IO uint32_t FR2;               /*!< Port R Function Register 2                   */
  __IO uint32_t FR3;               /*!< Port R Function Register 3                   */
       uint32_t RESERVED1[5];
  __IO uint32_t OD;                /*!< Port R Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port R Pull-up Control Register              */
  __IO uint32_t PDN;               /*!< Port R Pull-down Control Register            */
       uint32_t RESERVED2;
  __IO uint32_t IE;                /*!< Port R Input Control Register                */
} TSB_PR_TypeDef;

/**
  * @brief Port T
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port T Data Register                         */
  __IO uint32_t CR;                /*!< Port T Output Control Register               */
  __IO uint32_t FR1;               /*!< Port T Function Register 1                   */
  __IO uint32_t FR2;               /*!< Port T Function Register 2                   */
  __IO uint32_t FR3;               /*!< Port T Function Register 3                   */
       uint32_t RESERVED0[2];
  __IO uint32_t FR6;               /*!< Port T Function Register 6                   */
  __IO uint32_t FR7;               /*!< Port T Function Register 7                   */
       uint32_t RESERVED1;
  __IO uint32_t OD;                /*!< Port T Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port T Pull-up Control Register              */
  __IO uint32_t PDN;               /*!< Port T Pull-down Control Register            */
       uint32_t RESERVED2;
  __IO uint32_t IE;                /*!< Port T Input Control Register                */
} TSB_PT_TypeDef;

/**
  * @brief Port U
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port U Data Register                         */
  __IO uint32_t CR;                /*!< Port U Output Control Register               */
       uint32_t RESERVED0;
  __IO uint32_t FR2;               /*!< Port U Function Register 2                   */
  __IO uint32_t FR3;               /*!< Port U Function Register 3                   */
       uint32_t RESERVED1[3];
  __IO uint32_t FR7;               /*!< Port U Function Register 7                   */
       uint32_t RESERVED2;
  __IO uint32_t OD;                /*!< Port U Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port U Pull-up Control Register              */
  __IO uint32_t PDN;               /*!< Port U Pull-down Control Register            */
       uint32_t RESERVED3;
  __IO uint32_t IE;                /*!< Port U Input Control Register                */
} TSB_PU_TypeDef;

/**
  * @brief Port V
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port V Data Register                         */
  __IO uint32_t CR;                /*!< Port V OutPut Control Register               */
       uint32_t RESERVED0;
  __IO uint32_t FR2;               /*!< Port V Function Register 2                   */
  __IO uint32_t FR3;               /*!< Port V Function Register 3                   */
  __IO uint32_t FR4;               /*!< Port V Function Register 4                   */
  __IO uint32_t FR5;               /*!< Port V Function Register 5                   */
  __IO uint32_t FR6;               /*!< Port V Function Register 6                   */
  __IO uint32_t FR7;               /*!< Port V Function Register 7                   */
       uint32_t RESERVED1;
  __IO uint32_t OD;                /*!< Port V Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port V Pull-up Control Register              */
  __IO uint32_t PDN;               /*!< Port V Pull-down Control Register            */
       uint32_t RESERVED2;
  __IO uint32_t IE;                /*!< Port V InPut Control Register                */
} TSB_PV_TypeDef;

/**
  * @brief Port W
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port W Data Register                         */
  __IO uint32_t CR;                /*!< Port W OutPut Control Register               */
       uint32_t RESERVED0[2];
  __IO uint32_t FR3;               /*!< Port W Function Register 3                   */
  __IO uint32_t FR4;               /*!< Port W Function Register 4                   */
  __IO uint32_t FR5;               /*!< Port W Function Register 5                   */
  __IO uint32_t FR6;               /*!< Port W Function Register 6                   */
  __IO uint32_t FR7;               /*!< Port W Function Register 7                   */
       uint32_t RESERVED1;
  __IO uint32_t OD;                /*!< Port W Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port W Pull-up Control Register              */
  __IO uint32_t PDN;               /*!< Port W Pull-down Control Register            */
       uint32_t RESERVED2;
  __IO uint32_t IE;                /*!< Port W InPut Control Register                */
} TSB_PW_TypeDef;

/**
  * @brief Port Y
  */
typedef struct
{
  __IO uint32_t DATA;              /*!< Port Y Data Register                         */
  __IO uint32_t CR;                /*!< Port Y OutPut Control Register               */
  __IO uint32_t FR1;               /*!< Port Y Function Register 1                   */
       uint32_t RESERVED0[2];
  __IO uint32_t FR4;               /*!< Port Y Function Register 4                   */
       uint32_t RESERVED1[4];
  __IO uint32_t OD;                /*!< Port Y Open Drain Control Register           */
  __IO uint32_t PUP;               /*!< Port Y Pull-up Control Register              */
  __IO uint32_t PDN;               /*!< Port Y Pull-down Control Register            */
       uint32_t RESERVED2;
  __IO uint32_t IE;                /*!< Port Y InPut Control Register                */
} TSB_PY_TypeDef;

/**
  * @brief Internal High-speed Oscillation Adjustment
  */
typedef struct
{
  __IO uint32_t OSCPRO;            /*!< TRM Protection Register                      */
  __IO uint32_t OSCEN;             /*!< TRM Enable Register                          */
  __I  uint32_t OSCINIT;           /*!< TRM Initial Trimming Level Monitor Register  */
  __IO uint32_t OSCSET;            /*!< TRM Trimming Level Setting Register          */
} TSB_TRM_TypeDef;

/**
  * @brief Oscillation Frequency Detector (OFD)
  */
typedef struct
{
  __IO uint32_t CR1;               /*!< OFD Control Register 1                       */
  __IO uint32_t CR2;               /*!< OFD Control Register 2                       */
  __IO uint32_t MN0;               /*!< OFD Lower Detection Frequency Setting Register0*/
  __IO uint32_t MN1;               /*!< OFD Lower Detection Frequency Setting Register1*/
  __IO uint32_t MX0;               /*!< OFD Higher Detection Frequency Setting Register0*/
  __IO uint32_t MX1;               /*!< OFD Higher Detection Frequency Setting Register1*/
  __IO uint32_t RST;               /*!< OFD Reset Enable Control Register            */
  __I  uint32_t STAT;              /*!< OFD Status Register                          */
  __IO uint32_t MON;               /*!< OFD External high frequency oscillaion clock monitor register */
} TSB_OFD_TypeDef;

/**
  * @brief Real Time Clock (RTC)
  */
typedef struct
{
  __IO uint8_t  SECR;              /*!< RTC Sec setting register                     */
  __IO uint8_t  MINR;              /*!< RTC Min settging register                    */
  __IO uint8_t  HOURR;             /*!< RTC Hour setting register                    */
       uint8_t  RESERVED0;
  __IO uint8_t  DAYR;              /*!< RTC Day setting register                     */
  __IO uint8_t  DATER;             /*!< RTC Date setting register                    */
  __IO uint8_t  MONTHR;            /*!< RTC Month settging register PAGE0            */
  __IO uint8_t  YEARR;             /*!< RTC Year setting register PAGE0              */
  __IO uint8_t  PAGER;             /*!< RTC Page register                            */
       uint8_t  RESERVED1[3];
  __IO uint8_t  RESTR;             /*!< RTC Reset register                           */
       uint8_t  RESERVED2;
  __IO uint8_t  PROTECT;           /*!< RTC protect register                         */
  __IO uint8_t  ADJCTL;            /*!< RTC clock adjust control register            */
  __IO uint8_t  ADJDAT;            /*!< RTC clock adjust data register               */
  __IO uint8_t  ADJSIGN;           /*!< RTC clock adjust sign register               */
} TSB_RTC_TypeDef;

/**
  * @brief Consumer Electronics Control (CEC)
  */
typedef struct
{
  __IO uint32_t EN;                /*!< CEC Enable Register                          */
  __IO uint32_t ADD;               /*!< CEC Logical Address Register                 */
  __O  uint32_t RESET;             /*!< CEC Software Reset Register                  */
  __IO uint32_t REN;               /*!< CEC Receive Enable Register                  */
  __I  uint32_t RBUF;              /*!< CEC Receive Buffer Register                  */
  __IO uint32_t RCR1;              /*!< CEC Receive Control Register 1               */
  __IO uint32_t RCR2;              /*!< CEC Receive Control Register 2               */
  __IO uint32_t RCR3;              /*!< CEC Receive Control Register 3               */
  __IO uint32_t TEN;               /*!< CEC Transmit Enable Register                 */
  __IO uint32_t TBUF;              /*!< CEC Transmit Buffer Register                 */
  __IO uint32_t TCR;               /*!< CEC Transmit Control Register                */
  __I  uint32_t RSTAT;             /*!< CEC Receive Interrupt Status Register        */
  __I  uint32_t TSTAT;             /*!< CEC Transmit Interrupt Status Register       */
  __IO uint32_t FSSEL;             /*!< CEC sampling clock selection Register        */
} TSB_CEC_TypeDef;

/**
  * @brief Remote Control Signal Preprocessor (RMC)
  */
typedef struct
{
  __IO uint32_t EN;                /*!< RMC Enable Register                          */
  __IO uint32_t REN;               /*!< RMC Receive Enable Register                  */
  __I  uint32_t RBUF1;             /*!< RMC Receive Data Buffer Register 1           */
  __I  uint32_t RBUF2;             /*!< RMC Receive Data Buffer Register 2           */
  __I  uint32_t RBUF3;             /*!< RMC Receive Data Buffer Register 3           */
  __IO uint32_t RCR1;              /*!< RMC Receive Control Register 1               */
  __IO uint32_t RCR2;              /*!< RMC Receive Control Register 2               */
  __IO uint32_t RCR3;              /*!< RMC Receive Control Register 3               */
  __IO uint32_t RCR4;              /*!< RMC Receive Control Register 4               */
  __I  uint32_t RSTAT;             /*!< RMC Receive Status Register                  */
  __IO uint32_t END1;              /*!< RMC Receive End Bit Number Register 1        */
  __IO uint32_t END2;              /*!< RMC Receive End Bit Number Register 2        */
  __IO uint32_t END3;              /*!< RMC Receive End Bit Number Register 3        */
  __IO uint32_t FSSEL;             /*!< RMC Frequency Selection Register             */
} TSB_RMC_TypeDef;

/**
  * @brief 
  */
typedef struct
{
  __IO uint32_t MDEN;              /*!< PMD Enable Register                          */
  __IO uint32_t PORTMD;            /*!< PMD Port Output Mode Register                */
  __IO uint32_t MDCR;              /*!< PMD Control Register                         */
  __I  uint32_t CARSTA;            /*!< PWM Carrier Status Register                  */
  __I  uint32_t BCARI;             /*!< PWM Basic Carrier Register                   */
  __IO uint32_t RATE;              /*!< PWM Frequency Register                       */
  __IO uint32_t CMPU;              /*!< PMD PWM Compare U Register                   */
  __IO uint32_t CMPV;              /*!< PMD PWM Compare V Register                   */
  __IO uint32_t CMPW;              /*!< PMD PWM Compare W Register                   */
  __IO uint32_t MODESEL;           /*!< PMD Mode Select Register                     */
  __IO uint32_t MDOUT;             /*!< PMD Conduction Control Register              */
  __IO uint32_t MDPOT;             /*!< PMD Output Setting Register                  */
  __O  uint32_t EMGREL;            /*!< PMD EMG Release Register                     */
  __IO uint32_t EMGCR;             /*!< PMD EMG Control Register                     */
  __I  uint32_t EMGSTA;            /*!< PMD EMG Status Register                      */
  __IO uint32_t OVVCR;             /*!< PMD OVV Control Register                     */
  __I  uint32_t OVVSTA;            /*!< PMD OVV Status Register                      */
  __IO uint32_t DTR;               /*!< PMD Dead Time Register                       */
  __IO uint32_t TRGCMP0;           /*!< PMD Trigger Compare Register 0               */
  __IO uint32_t TRGCMP1;           /*!< PMD Trigger Compare Register 1               */
  __IO uint32_t TRGCMP2;           /*!< PMD Trigger Compare Register 2               */
  __IO uint32_t TRGCMP3;           /*!< PMD Trigger Compare Register 3               */
  __IO uint32_t TRGCR;             /*!< PMD Trigger Control Register                 */
  __IO uint32_t TRGMD;             /*!< PMD Trigger Output Mode Setting Register     */
  __IO uint32_t TRGSEL;            /*!< PMD Trigger Output Select Register           */
  __IO uint32_t TRGSYNCR;          /*!< PMD Trigger Update Timing Setting Register   */
  __IO uint32_t VPWMPH;            /*!< Phase difference setting of the V-phase PWM  */
  __IO uint32_t WPWMPH;            /*!< Phase difference setting of the W-phase PWM  */
  __IO uint32_t MBUFCR;            /*!< Update timing of the triple buffer           */
  __IO uint32_t SYNCCR;            /*!< Synchronization control between the PMD channel*/
} TSB_PMD_TypeDef;

/**
  * @brief 
  */
typedef struct
{
  __IO uint32_t EN;                /*!< Interval Sencing Detector(ISD)               */
  __IO uint32_t CLKCR;             /*!< Clock Control Register                       */
  __IO uint32_t OCR0;              /*!< Output Control Register 0                    */
  __IO uint32_t OCR1;              /*!< Output Control Register 1                    */
  __IO uint32_t ICR;               /*!< Input Control Register                       */
  __IO uint32_t CR;                /*!< Control Register                             */
  __I  uint32_t BR;                /*!< Buffer Register                              */
  __I  uint32_t SR;                /*!< Status Register                              */
  __IO uint32_t INTCR;             /*!< interrupt Control Register                   */
} TSB_ISD_TypeDef;

/**
  * @brief 
  */
typedef struct
{
       uint32_t RESERVED0[4];
  __IO uint32_t SBMR;              /*!< Flash Security Bit Mask Register             */
  __IO uint32_t SSR;               /*!< Flash Security Status Register               */
  __O  uint32_t KCR;               /*!< Flash Key Code Register                      */
       uint32_t RESERVED1;
  __IO uint32_t SR0;               /*!< Flash Status Register 0                      */
       uint32_t RESERVED2[3];
  __I  uint32_t PSR0;              /*!< Flash Protect Status Register 0              */
  __I  uint32_t PSR1;              /*!< Flash Protect Status Register 1              */
       uint32_t RESERVED3;
  __I  uint32_t PSR3;              /*!< Flash Protect Status Register 3              */
  __I  uint32_t PSR4;              /*!< Flash Protect Status Register 4              */
       uint32_t RESERVED4;
  __I  uint32_t PSR6;              /*!< Flash Protect Status Register 6              */
       uint32_t RESERVED5;
  __IO uint32_t PMR0;              /*!< Flash Protect Mask Register 0                */
  __IO uint32_t PMR1;              /*!< Flash Protect Mask Register 1                */
       uint32_t RESERVED6;
  __IO uint32_t PMR3;              /*!< Flash Protect Mask Register 3                */
  __IO uint32_t PMR4;              /*!< Flash Protect Mask Register 4                */
       uint32_t RESERVED7;
  __IO uint32_t PMR6;              /*!< Flash Protect Mask Register 6                */
       uint32_t RESERVED8[37];
  __I  uint32_t SR1;               /*!< Flash Status Register 1                      */
  __I  uint32_t SWPSR;             /*!< Flash Memory SWP Status Register             */
       uint32_t RESERVED9[14];
  __IO uint32_t AREASEL;           /*!< Flash Area Selection Register                */
       uint32_t RESERVED10;
  __IO uint32_t CR;                /*!< Flash Control Register                       */
  __IO uint32_t STSCLR;            /*!< Flash Status Clear Register                  */
  __IO uint32_t BNKCR;             /*!< Flash Bank Change Register                   */
       uint32_t RESERVED11;
  __IO uint32_t BUFDISCLR;         /*!< Flash Buffer Disable and Clear Register      */
} TSB_FC_TypeDef;


/* Memory map */
#define FLASH_BASE            (0x00000000UL)
#define RAM_BASE              (0x20000000UL)
#define PERI_BASE             (0x40000000UL)


#define TSB_DMACA_BASE             (PERI_BASE  + 0x0000000UL)
#define TSB_DMACB_BASE             (PERI_BASE  + 0x0001000UL)
#define TSB_SMI0_BASE              (PERI_BASE  + 0x000C000UL)
#define TSB_IA_BASE                (PERI_BASE  + 0x003E000UL)
#define TSB_RLM_BASE               (PERI_BASE  + 0x003E400UL)
#define TSB_LVD_BASE               (PERI_BASE  + 0x003EC00UL)
#define TSB_TSEL0_BASE             (PERI_BASE  + 0x00A0400UL)
#define TSB_LTT0_BASE              (PERI_BASE  + 0x003FF00UL)
#define TSB_TSPI0_BASE             (PERI_BASE  + 0x006A000UL)
#define TSB_TSPI1_BASE             (PERI_BASE  + 0x006A400UL)
#define TSB_TSPI2_BASE             (PERI_BASE  + 0x006A800UL)
#define TSB_TSPI3_BASE             (PERI_BASE  + 0x006AC00UL)
#define TSB_TSPI4_BASE             (PERI_BASE  + 0x006B000UL)
#define TSB_TSPI5_BASE             (PERI_BASE  + 0x006B400UL)
#define TSB_TSPI6_BASE             (PERI_BASE  + 0x00CB800UL)
#define TSB_TSPI7_BASE             (PERI_BASE  + 0x00CBC00UL)
#define TSB_TSPI8_BASE             (PERI_BASE  + 0x00CC000UL)
#define TSB_EXB_BASE               (PERI_BASE  + 0x0076000UL)
#define TSB_CG_BASE                (PERI_BASE  + 0x0083000UL)
#define TSB_IB_BASE                (PERI_BASE  + 0x0083200UL)
#define TSB_IMN_BASE               (PERI_BASE  + 0x0083300UL)
#define TSB_DNFA_BASE              (PERI_BASE  + 0x00A0200UL)
#define TSB_DNFB_BASE              (PERI_BASE  + 0x00A0300UL)
#define TSB_SIWD0_BASE             (PERI_BASE  + 0x00A0600UL)
#define TSB_NBD_BASE               (PERI_BASE  + 0x00A2000UL)
#define TSB_MDMAA_BASE             (PERI_BASE  + 0x00A4000UL)
#define TSB_FURT0_BASE             (PERI_BASE  + 0x00A8000UL)
#define TSB_FURT1_BASE             (PERI_BASE  + 0x00A9000UL)
#define TSB_ADA_BASE               (PERI_BASE  + 0x00BA000UL)
#define TSB_DA0_BASE               (PERI_BASE  + 0x00BC800UL)
#define TSB_DA1_BASE               (PERI_BASE  + 0x00BC900UL)
#define TSB_T32A0_BASE             (PERI_BASE  + 0x00C1000UL)
#define TSB_T32A1_BASE             (PERI_BASE  + 0x00C1400UL)
#define TSB_T32A2_BASE             (PERI_BASE  + 0x00C1800UL)
#define TSB_T32A3_BASE             (PERI_BASE  + 0x00C1C00UL)
#define TSB_T32A4_BASE             (PERI_BASE  + 0x00C2000UL)
#define TSB_T32A5_BASE             (PERI_BASE  + 0x00C2400UL)
#define TSB_T32A6_BASE             (PERI_BASE  + 0x00C2800UL)
#define TSB_T32A7_BASE             (PERI_BASE  + 0x00C2C00UL)
#define TSB_T32A8_BASE             (PERI_BASE  + 0x00C3000UL)
#define TSB_T32A9_BASE             (PERI_BASE  + 0x00C3400UL)
#define TSB_T32A10_BASE            (PERI_BASE  + 0x00C3800UL)
#define TSB_T32A11_BASE            (PERI_BASE  + 0x00C3C00UL)
#define TSB_T32A12_BASE            (PERI_BASE  + 0x00C4000UL)
#define TSB_T32A13_BASE            (PERI_BASE  + 0x00C4400UL)
#define TSB_UART0_BASE             (PERI_BASE  + 0x00CE000UL)
#define TSB_UART1_BASE             (PERI_BASE  + 0x00CE400UL)
#define TSB_UART2_BASE             (PERI_BASE  + 0x00CE800UL)
#define TSB_UART3_BASE             (PERI_BASE  + 0x00CEC00UL)
#define TSB_UART4_BASE             (PERI_BASE  + 0x00CF000UL)
#define TSB_UART5_BASE             (PERI_BASE  + 0x00CF400UL)
#define TSB_I2C0_BASE              (PERI_BASE  + 0x00D1000UL)
#define TSB_I2C1_BASE              (PERI_BASE  + 0x00D2000UL)
#define TSB_I2C2_BASE              (PERI_BASE  + 0x00D3000UL)
#define TSB_I2C3_BASE              (PERI_BASE  + 0x00D4000UL)
#define TSB_I2C4_BASE              (PERI_BASE  + 0x00D5000UL)
#define TSB_PA_BASE                (PERI_BASE  + 0x00E0000UL)
#define TSB_PB_BASE                (PERI_BASE  + 0x00E0100UL)
#define TSB_PC_BASE                (PERI_BASE  + 0x00E0200UL)
#define TSB_PD_BASE                (PERI_BASE  + 0x00E0300UL)
#define TSB_PE_BASE                (PERI_BASE  + 0x00E0400UL)
#define TSB_PF_BASE                (PERI_BASE  + 0x00E0500UL)
#define TSB_PG_BASE                (PERI_BASE  + 0x00E0600UL)
#define TSB_PH_BASE                (PERI_BASE  + 0x00E0700UL)
#define TSB_PJ_BASE                (PERI_BASE  + 0x00E0800UL)
#define TSB_PK_BASE                (PERI_BASE  + 0x00E0900UL)
#define TSB_PL_BASE                (PERI_BASE  + 0x00E0A00UL)
#define TSB_PM_BASE                (PERI_BASE  + 0x00E0B00UL)
#define TSB_PN_BASE                (PERI_BASE  + 0x00E0C00UL)
#define TSB_PP_BASE                (PERI_BASE  + 0x00E0D00UL)
#define TSB_PR_BASE                (PERI_BASE  + 0x00E0E00UL)
#define TSB_PT_BASE                (PERI_BASE  + 0x00E0F00UL)
#define TSB_PU_BASE                (PERI_BASE  + 0x00E1000UL)
#define TSB_PV_BASE                (PERI_BASE  + 0x00E1100UL)
#define TSB_PW_BASE                (PERI_BASE  + 0x00E1200UL)
#define TSB_PY_BASE                (PERI_BASE  + 0x00E1300UL)
#define TSB_TRM_BASE               (PERI_BASE  + 0x00E3100UL)
#define TSB_OFD_BASE               (PERI_BASE  + 0x00E4000UL)
#define TSB_RTC_BASE               (PERI_BASE  + 0x00E4800UL)
#define TSB_CEC0_BASE              (PERI_BASE  + 0x00E8000UL)
#define TSB_RMC0_BASE              (PERI_BASE  + 0x00E8100UL)
#define TSB_RMC1_BASE              (PERI_BASE  + 0x00E8200UL)
#define TSB_PMD0_BASE              (PERI_BASE  + 0x00E9000UL)
#define TSB_ISDA_BASE              (PERI_BASE  + 0x00F0000UL)
#define TSB_ISDB_BASE              (PERI_BASE  + 0x00F0100UL)
#define TSB_ISDC_BASE              (PERI_BASE  + 0x00F0200UL)
#define TSB_FC_BASE                (PERI_BASE  + 0x1DFF0000UL)


/* Peripheral declaration */
#define TSB_DMACA                  ((   TSB_DMAC_TypeDef *) TSB_DMACA_BASE)
#define TSB_DMACB                  ((   TSB_DMAC_TypeDef *) TSB_DMACB_BASE)
#define TSB_SMI0                   ((    TSB_SMI_TypeDef *)  TSB_SMI0_BASE)
#define TSB_IA                     ((     TSB_IA_TypeDef *)    TSB_IA_BASE)
#define TSB_RLM                    ((    TSB_RLM_TypeDef *)   TSB_RLM_BASE)
#define TSB_LVD                    ((    TSB_LVD_TypeDef *)   TSB_LVD_BASE)
#define TSB_TSEL0                  ((   TSB_TSEL_TypeDef *) TSB_TSEL0_BASE)
#define TSB_LTT0                   ((    TSB_LTT_TypeDef *)  TSB_LTT0_BASE)
#define TSB_TSPI0                  ((   TSB_TSPI_TypeDef *) TSB_TSPI0_BASE)
#define TSB_TSPI1                  ((   TSB_TSPI_TypeDef *) TSB_TSPI1_BASE)
#define TSB_TSPI2                  ((   TSB_TSPI_TypeDef *) TSB_TSPI2_BASE)
#define TSB_TSPI3                  ((   TSB_TSPI_TypeDef *) TSB_TSPI3_BASE)
#define TSB_TSPI4                  ((   TSB_TSPI_TypeDef *) TSB_TSPI4_BASE)
#define TSB_TSPI5                  ((   TSB_TSPI_TypeDef *) TSB_TSPI5_BASE)
#define TSB_TSPI6                  ((   TSB_TSPI_TypeDef *) TSB_TSPI6_BASE)
#define TSB_TSPI7                  ((   TSB_TSPI_TypeDef *) TSB_TSPI7_BASE)
#define TSB_TSPI8                  ((   TSB_TSPI_TypeDef *) TSB_TSPI8_BASE)
#define TSB_EXB                    ((    TSB_EXB_TypeDef *)   TSB_EXB_BASE)
#define TSB_CG                     ((     TSB_CG_TypeDef *)    TSB_CG_BASE)
#define TSB_IB                     ((     TSB_IB_TypeDef *)    TSB_IB_BASE)
#define TSB_IMN                    ((    TSB_IMN_TypeDef *)   TSB_IMN_BASE)
#define TSB_DNFA                   ((    TSB_DNF_TypeDef *)  TSB_DNFA_BASE)
#define TSB_DNFB                   ((    TSB_DNF_TypeDef *)  TSB_DNFB_BASE)
#define TSB_SIWD0                  ((   TSB_SIWD_TypeDef *) TSB_SIWD0_BASE)
#define TSB_NBD                    ((    TSB_NBD_TypeDef *)   TSB_NBD_BASE)
#define TSB_MDMAA                  ((   TSB_MDMA_TypeDef *) TSB_MDMAA_BASE)
#define TSB_FURT0                  ((   TSB_FURT_TypeDef *) TSB_FURT0_BASE)
#define TSB_FURT1                  ((   TSB_FURT_TypeDef *) TSB_FURT1_BASE)
#define TSB_ADA                    ((     TSB_AD_TypeDef *)   TSB_ADA_BASE)
#define TSB_DA0                    ((     TSB_DA_TypeDef *)   TSB_DA0_BASE)
#define TSB_DA1                    ((     TSB_DA_TypeDef *)   TSB_DA1_BASE)
#define TSB_T32A0                  ((   TSB_T32A_TypeDef *) TSB_T32A0_BASE)
#define TSB_T32A1                  ((   TSB_T32A_TypeDef *) TSB_T32A1_BASE)
#define TSB_T32A2                  ((   TSB_T32A_TypeDef *) TSB_T32A2_BASE)
#define TSB_T32A3                  ((   TSB_T32A_TypeDef *) TSB_T32A3_BASE)
#define TSB_T32A4                  ((   TSB_T32A_TypeDef *) TSB_T32A4_BASE)
#define TSB_T32A5                  ((   TSB_T32A_TypeDef *) TSB_T32A5_BASE)
#define TSB_T32A6                  ((   TSB_T32A_TypeDef *) TSB_T32A6_BASE)
#define TSB_T32A7                  ((   TSB_T32A_TypeDef *) TSB_T32A7_BASE)
#define TSB_T32A8                  ((   TSB_T32A_TypeDef *) TSB_T32A8_BASE)
#define TSB_T32A9                  ((   TSB_T32A_TypeDef *) TSB_T32A9_BASE)
#define TSB_T32A10                 ((   TSB_T32A_TypeDef *)TSB_T32A10_BASE)
#define TSB_T32A11                 ((   TSB_T32A_TypeDef *)TSB_T32A11_BASE)
#define TSB_T32A12                 ((   TSB_T32A_TypeDef *)TSB_T32A12_BASE)
#define TSB_T32A13                 ((   TSB_T32A_TypeDef *)TSB_T32A13_BASE)
#define TSB_UART0                  ((   TSB_UART_TypeDef *) TSB_UART0_BASE)
#define TSB_UART1                  ((   TSB_UART_TypeDef *) TSB_UART1_BASE)
#define TSB_UART2                  ((   TSB_UART_TypeDef *) TSB_UART2_BASE)
#define TSB_UART3                  ((   TSB_UART_TypeDef *) TSB_UART3_BASE)
#define TSB_UART4                  ((   TSB_UART_TypeDef *) TSB_UART4_BASE)
#define TSB_UART5                  ((   TSB_UART_TypeDef *) TSB_UART5_BASE)
#define TSB_I2C0                   ((    TSB_I2C_TypeDef *)  TSB_I2C0_BASE)
#define TSB_I2C1                   ((    TSB_I2C_TypeDef *)  TSB_I2C1_BASE)
#define TSB_I2C2                   ((    TSB_I2C_TypeDef *)  TSB_I2C2_BASE)
#define TSB_I2C3                   ((    TSB_I2C_TypeDef *)  TSB_I2C3_BASE)
#define TSB_I2C4                   ((    TSB_I2C_TypeDef *)  TSB_I2C4_BASE)
#define TSB_PA                     ((     TSB_PA_TypeDef *)    TSB_PA_BASE)
#define TSB_PB                     ((     TSB_PB_TypeDef *)    TSB_PB_BASE)
#define TSB_PC                     ((     TSB_PC_TypeDef *)    TSB_PC_BASE)
#define TSB_PD                     ((     TSB_PD_TypeDef *)    TSB_PD_BASE)
#define TSB_PE                     ((     TSB_PE_TypeDef *)    TSB_PE_BASE)
#define TSB_PF                     ((     TSB_PF_TypeDef *)    TSB_PF_BASE)
#define TSB_PG                     ((     TSB_PG_TypeDef *)    TSB_PG_BASE)
#define TSB_PH                     ((     TSB_PH_TypeDef *)    TSB_PH_BASE)
#define TSB_PJ                     ((     TSB_PJ_TypeDef *)    TSB_PJ_BASE)
#define TSB_PK                     ((     TSB_PK_TypeDef *)    TSB_PK_BASE)
#define TSB_PL                     ((     TSB_PL_TypeDef *)    TSB_PL_BASE)
#define TSB_PM                     ((     TSB_PM_TypeDef *)    TSB_PM_BASE)
#define TSB_PN                     ((     TSB_PN_TypeDef *)    TSB_PN_BASE)
#define TSB_PP                     ((     TSB_PP_TypeDef *)    TSB_PP_BASE)
#define TSB_PR                     ((     TSB_PR_TypeDef *)    TSB_PR_BASE)
#define TSB_PT                     ((     TSB_PT_TypeDef *)    TSB_PT_BASE)
#define TSB_PU                     ((     TSB_PU_TypeDef *)    TSB_PU_BASE)
#define TSB_PV                     ((     TSB_PV_TypeDef *)    TSB_PV_BASE)
#define TSB_PW                     ((     TSB_PW_TypeDef *)    TSB_PW_BASE)
#define TSB_PY                     ((     TSB_PY_TypeDef *)    TSB_PY_BASE)
#define TSB_TRM                    ((    TSB_TRM_TypeDef *)   TSB_TRM_BASE)
#define TSB_OFD                    ((    TSB_OFD_TypeDef *)   TSB_OFD_BASE)
#define TSB_RTC                    ((    TSB_RTC_TypeDef *)   TSB_RTC_BASE)
#define TSB_CEC0                   ((    TSB_CEC_TypeDef *)  TSB_CEC0_BASE)
#define TSB_RMC0                   ((    TSB_RMC_TypeDef *)  TSB_RMC0_BASE)
#define TSB_RMC1                   ((    TSB_RMC_TypeDef *)  TSB_RMC1_BASE)
#define TSB_PMD0                   ((    TSB_PMD_TypeDef *)  TSB_PMD0_BASE)
#define TSB_ISDA                   ((    TSB_ISD_TypeDef *)  TSB_ISDA_BASE)
#define TSB_ISDB                   ((    TSB_ISD_TypeDef *)  TSB_ISDB_BASE)
#define TSB_ISDC                   ((    TSB_ISD_TypeDef *)  TSB_ISDC_BASE)
#define TSB_FC                     ((     TSB_FC_TypeDef *)    TSB_FC_BASE)


/* Bit-Band for Device Specific Peripheral Registers */
#define BITBAND_OFFSET (0x02000000UL)
#define BITBAND_PERI_BASE (PERI_BASE + BITBAND_OFFSET)
#define BITBAND_PERI(addr, bitnum) (BITBAND_PERI_BASE + (((uint32_t)(addr) - PERI_BASE) << 5) + ((uint32_t)(bitnum) << 2))



/* DMA Controller */
#define TSB_DMACA_INTSTATUS_INTSTATUS0            (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->INTSTATUS,0)))
#define TSB_DMACA_INTSTATUS_INTSTATUS1            (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->INTSTATUS,1)))
#define TSB_DMACA_INTTCSTATUS_INTTCSTATUS0        (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->INTTCSTATUS,0)))
#define TSB_DMACA_INTTCSTATUS_INTTCSTATUS1        (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->INTTCSTATUS,1)))
#define TSB_DMACA_INTERRORSTATUS_INTERRSTATUS0    (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->INTERRORSTATUS,0)))
#define TSB_DMACA_INTERRORSTATUS_INTERRSTATUS1    (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->INTERRORSTATUS,1)))
#define TSB_DMACA_RAWINTTCSTATUS_RAWINTTCS0       (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->RAWINTTCSTATUS,0)))
#define TSB_DMACA_RAWINTTCSTATUS_RAWINTTCS1       (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->RAWINTTCSTATUS,1)))
#define TSB_DMACA_RAWINTERRORSTATUS_RAWINTERRS0   (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->RAWINTERRORSTATUS,0)))
#define TSB_DMACA_RAWINTERRORSTATUS_RAWINTERRS1   (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->RAWINTERRORSTATUS,1)))
#define TSB_DMACA_ENBLDCHNS_ENABLEDCH0            (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->ENBLDCHNS,0)))
#define TSB_DMACA_ENBLDCHNS_ENABLEDCH1            (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->ENBLDCHNS,1)))
#define TSB_DMACA_CONFIGURATION_E                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACA->CONFIGURATION,0)))
#define TSB_DMACA_C0CONTROL_SI                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACA->C0CONTROL,26)))
#define TSB_DMACA_C0CONTROL_DI                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACA->C0CONTROL,27)))
#define TSB_DMACA_C0CONTROL_I                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACA->C0CONTROL,31)))
#define TSB_DMACA_C0CONFIGURATION_E               (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACA->C0CONFIGURATION,0)))
#define TSB_DMACA_C0CONFIGURATION_IE              (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACA->C0CONFIGURATION,14)))
#define TSB_DMACA_C0CONFIGURATION_ITC             (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACA->C0CONFIGURATION,15)))
#define TSB_DMACA_C0CONFIGURATION_LOCK            (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACA->C0CONFIGURATION,16)))
#define TSB_DMACA_C0CONFIGURATION_ACTIVE          (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->C0CONFIGURATION,17)))
#define TSB_DMACA_C0CONFIGURATION_HALT            (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACA->C0CONFIGURATION,18)))
#define TSB_DMACA_C1CONTROL_SI                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACA->C1CONTROL,26)))
#define TSB_DMACA_C1CONTROL_DI                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACA->C1CONTROL,27)))
#define TSB_DMACA_C1CONTROL_I                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACA->C1CONTROL,31)))
#define TSB_DMACA_C1CONFIGURATION_E               (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACA->C1CONFIGURATION,0)))
#define TSB_DMACA_C1CONFIGURATION_IE              (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACA->C1CONFIGURATION,14)))
#define TSB_DMACA_C1CONFIGURATION_ITC             (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACA->C1CONFIGURATION,15)))
#define TSB_DMACA_C1CONFIGURATION_LOCK            (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACA->C1CONFIGURATION,16)))
#define TSB_DMACA_C1CONFIGURATION_ACTIVE          (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACA->C1CONFIGURATION,17)))
#define TSB_DMACA_C1CONFIGURATION_HALT            (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACA->C1CONFIGURATION,18)))

#define TSB_DMACB_INTSTATUS_INTSTATUS0            (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->INTSTATUS,0)))
#define TSB_DMACB_INTSTATUS_INTSTATUS1            (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->INTSTATUS,1)))
#define TSB_DMACB_INTTCSTATUS_INTTCSTATUS0        (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->INTTCSTATUS,0)))
#define TSB_DMACB_INTTCSTATUS_INTTCSTATUS1        (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->INTTCSTATUS,1)))
#define TSB_DMACB_INTERRORSTATUS_INTERRSTATUS0    (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->INTERRORSTATUS,0)))
#define TSB_DMACB_INTERRORSTATUS_INTERRSTATUS1    (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->INTERRORSTATUS,1)))
#define TSB_DMACB_RAWINTTCSTATUS_RAWINTTCS0       (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->RAWINTTCSTATUS,0)))
#define TSB_DMACB_RAWINTTCSTATUS_RAWINTTCS1       (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->RAWINTTCSTATUS,1)))
#define TSB_DMACB_RAWINTERRORSTATUS_RAWINTERRS0   (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->RAWINTERRORSTATUS,0)))
#define TSB_DMACB_RAWINTERRORSTATUS_RAWINTERRS1   (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->RAWINTERRORSTATUS,1)))
#define TSB_DMACB_ENBLDCHNS_ENABLEDCH0            (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->ENBLDCHNS,0)))
#define TSB_DMACB_ENBLDCHNS_ENABLEDCH1            (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->ENBLDCHNS,1)))
#define TSB_DMACB_CONFIGURATION_E                 (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACB->CONFIGURATION,0)))
#define TSB_DMACB_C0CONTROL_SI                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACB->C0CONTROL,26)))
#define TSB_DMACB_C0CONTROL_DI                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACB->C0CONTROL,27)))
#define TSB_DMACB_C0CONTROL_I                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACB->C0CONTROL,31)))
#define TSB_DMACB_C0CONFIGURATION_E               (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACB->C0CONFIGURATION,0)))
#define TSB_DMACB_C0CONFIGURATION_IE              (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACB->C0CONFIGURATION,14)))
#define TSB_DMACB_C0CONFIGURATION_ITC             (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACB->C0CONFIGURATION,15)))
#define TSB_DMACB_C0CONFIGURATION_LOCK            (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACB->C0CONFIGURATION,16)))
#define TSB_DMACB_C0CONFIGURATION_ACTIVE          (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->C0CONFIGURATION,17)))
#define TSB_DMACB_C0CONFIGURATION_HALT            (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACB->C0CONFIGURATION,18)))
#define TSB_DMACB_C1CONTROL_SI                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACB->C1CONTROL,26)))
#define TSB_DMACB_C1CONTROL_DI                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACB->C1CONTROL,27)))
#define TSB_DMACB_C1CONTROL_I                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACB->C1CONTROL,31)))
#define TSB_DMACB_C1CONFIGURATION_E               (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACB->C1CONFIGURATION,0)))
#define TSB_DMACB_C1CONFIGURATION_IE              (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACB->C1CONFIGURATION,14)))
#define TSB_DMACB_C1CONFIGURATION_ITC             (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACB->C1CONFIGURATION,15)))
#define TSB_DMACB_C1CONFIGURATION_LOCK            (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACB->C1CONFIGURATION,16)))
#define TSB_DMACB_C1CONFIGURATION_ACTIVE          (*((__I  uint32_t *)BITBAND_PERI(&TSB_DMACB->C1CONFIGURATION,17)))
#define TSB_DMACB_C1CONFIGURATION_HALT            (*((__IO uint32_t *)BITBAND_PERI(&TSB_DMACB->C1CONFIGURATION,18)))


/*  */
#define TSB_SMI0_MAP0_RE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SMI0->MAP0,0)))
#define TSB_SMI0_MAP1_RE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_SMI0->MAP1,0)))
#define TSB_SMI0_DACR0_POLLWIP                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_SMI0->DACR0,6)))
#define TSB_SMI0_DACR1_POLLWIP                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_SMI0->DACR1,6)))
#define TSB_SMI0_RACR1_CYCGO                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_SMI0->RACR1,0)))
#define TSB_SMI0_RACR1_CSNUM                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_SMI0->RACR1,1)))
#define TSB_SMI0_RACR1_PBUFEN                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_SMI0->RACR1,4)))
#define TSB_SMI0_RACR1_SBUFEN                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_SMI0->RACR1,5)))
#define TSB_SMI0_INT_INTEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SMI0->INT,0)))
#define TSB_SMI0_STAT_CYCDONE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_SMI0->STAT,0)))
#define TSB_SMI0_STAT_CYCPROG                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_SMI0->STAT,1)))





/* TRGSEL */
#define TSB_TSEL0_CR0_EN0                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,0)))
#define TSB_TSEL0_CR0_OUTSEL0                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,1)))
#define TSB_TSEL0_CR0_UPDN0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,2)))
#define TSB_TSEL0_CR0_EN1                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,8)))
#define TSB_TSEL0_CR0_OUTSEL1                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,9)))
#define TSB_TSEL0_CR0_UPDN1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,10)))
#define TSB_TSEL0_CR0_EN2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,16)))
#define TSB_TSEL0_CR0_OUTSEL2                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,17)))
#define TSB_TSEL0_CR0_UPDN2                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,18)))
#define TSB_TSEL0_CR0_EN3                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,24)))
#define TSB_TSEL0_CR0_OUTSEL3                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,25)))
#define TSB_TSEL0_CR0_UPDN3                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR0,26)))
#define TSB_TSEL0_CR1_EN4                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,0)))
#define TSB_TSEL0_CR1_OUTSEL4                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,1)))
#define TSB_TSEL0_CR1_UPDN4                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,2)))
#define TSB_TSEL0_CR1_EN5                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,8)))
#define TSB_TSEL0_CR1_OUTSEL5                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,9)))
#define TSB_TSEL0_CR1_UPDN5                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,10)))
#define TSB_TSEL0_CR1_EN6                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,16)))
#define TSB_TSEL0_CR1_OUTSEL6                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,17)))
#define TSB_TSEL0_CR1_UPDN6                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,18)))
#define TSB_TSEL0_CR1_EN7                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,24)))
#define TSB_TSEL0_CR1_OUTSEL7                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,25)))
#define TSB_TSEL0_CR1_UPDN7                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR1,26)))
#define TSB_TSEL0_CR2_EN8                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,0)))
#define TSB_TSEL0_CR2_OUTSEL8                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,1)))
#define TSB_TSEL0_CR2_UPDN8                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,2)))
#define TSB_TSEL0_CR2_EN9                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,8)))
#define TSB_TSEL0_CR2_OUTSEL9                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,9)))
#define TSB_TSEL0_CR2_UPDN9                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,10)))
#define TSB_TSEL0_CR2_EN10                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,16)))
#define TSB_TSEL0_CR2_OUTSEL10                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,17)))
#define TSB_TSEL0_CR2_UPDN10                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,18)))
#define TSB_TSEL0_CR2_EN11                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,24)))
#define TSB_TSEL0_CR2_OUTSEL11                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,25)))
#define TSB_TSEL0_CR2_UPDN11                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR2,26)))
#define TSB_TSEL0_CR3_EN12                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,0)))
#define TSB_TSEL0_CR3_OUTSEL12                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,1)))
#define TSB_TSEL0_CR3_UPDN12                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,2)))
#define TSB_TSEL0_CR3_EN13                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,8)))
#define TSB_TSEL0_CR3_OUTSEL13                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,9)))
#define TSB_TSEL0_CR3_UPDN13                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,10)))
#define TSB_TSEL0_CR3_EN14                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,16)))
#define TSB_TSEL0_CR3_OUTSEL14                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,17)))
#define TSB_TSEL0_CR3_UPDN14                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,18)))
#define TSB_TSEL0_CR3_EN15                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,24)))
#define TSB_TSEL0_CR3_OUTSEL15                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,25)))
#define TSB_TSEL0_CR3_UPDN15                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR3,26)))
#define TSB_TSEL0_CR4_EN16                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,0)))
#define TSB_TSEL0_CR4_OUTSEL16                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,1)))
#define TSB_TSEL0_CR4_UPDN16                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,2)))
#define TSB_TSEL0_CR4_EN17                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,8)))
#define TSB_TSEL0_CR4_OUTSEL17                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,9)))
#define TSB_TSEL0_CR4_UPDN17                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,10)))
#define TSB_TSEL0_CR4_EN18                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,16)))
#define TSB_TSEL0_CR4_OUTSEL18                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,17)))
#define TSB_TSEL0_CR4_UPDN18                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,18)))
#define TSB_TSEL0_CR4_EN19                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,24)))
#define TSB_TSEL0_CR4_OUTSEL19                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,25)))
#define TSB_TSEL0_CR4_UPDN19                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR4,26)))
#define TSB_TSEL0_CR5_EN20                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,0)))
#define TSB_TSEL0_CR5_OUTSEL20                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,1)))
#define TSB_TSEL0_CR5_UPDN20                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,2)))
#define TSB_TSEL0_CR5_EN21                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,8)))
#define TSB_TSEL0_CR5_OUTSEL21                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,9)))
#define TSB_TSEL0_CR5_UPDN21                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,10)))
#define TSB_TSEL0_CR5_EN22                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,16)))
#define TSB_TSEL0_CR5_OUTSEL22                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,17)))
#define TSB_TSEL0_CR5_UPDN22                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,18)))
#define TSB_TSEL0_CR5_EN23                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,24)))
#define TSB_TSEL0_CR5_OUTSEL23                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,25)))
#define TSB_TSEL0_CR5_UPDN23                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR5,26)))
#define TSB_TSEL0_CR6_EN24                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,0)))
#define TSB_TSEL0_CR6_OUTSEL24                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,1)))
#define TSB_TSEL0_CR6_UPDN24                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,2)))
#define TSB_TSEL0_CR6_EN25                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,8)))
#define TSB_TSEL0_CR6_OUTSEL25                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,9)))
#define TSB_TSEL0_CR6_UPDN25                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,10)))
#define TSB_TSEL0_CR6_EN26                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,16)))
#define TSB_TSEL0_CR6_OUTSEL26                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,17)))
#define TSB_TSEL0_CR6_UPDN26                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,18)))
#define TSB_TSEL0_CR6_EN27                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,24)))
#define TSB_TSEL0_CR6_OUTSEL27                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,25)))
#define TSB_TSEL0_CR6_UPDN27                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR6,26)))
#define TSB_TSEL0_CR7_EN28                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,0)))
#define TSB_TSEL0_CR7_OUTSEL28                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,1)))
#define TSB_TSEL0_CR7_UPDN28                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,2)))
#define TSB_TSEL0_CR7_EN29                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,8)))
#define TSB_TSEL0_CR7_OUTSEL29                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,9)))
#define TSB_TSEL0_CR7_UPDN29                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,10)))
#define TSB_TSEL0_CR7_EN30                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,16)))
#define TSB_TSEL0_CR7_OUTSEL30                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,17)))
#define TSB_TSEL0_CR7_UPDN30                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,18)))
#define TSB_TSEL0_CR7_EN31                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,24)))
#define TSB_TSEL0_CR7_OUTSEL31                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,25)))
#define TSB_TSEL0_CR7_UPDN31                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR7,26)))
#define TSB_TSEL0_CR8_EN32                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,0)))
#define TSB_TSEL0_CR8_OUTSEL32                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,1)))
#define TSB_TSEL0_CR8_UPDN32                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,2)))
#define TSB_TSEL0_CR8_EN33                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,8)))
#define TSB_TSEL0_CR8_OUTSEL33                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,9)))
#define TSB_TSEL0_CR8_UPDN33                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,10)))
#define TSB_TSEL0_CR8_EN34                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,16)))
#define TSB_TSEL0_CR8_OUTSEL34                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,17)))
#define TSB_TSEL0_CR8_UPDN34                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,18)))
#define TSB_TSEL0_CR8_EN35                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,24)))
#define TSB_TSEL0_CR8_OUTSEL35                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,25)))
#define TSB_TSEL0_CR8_UPDN35                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR8,26)))
#define TSB_TSEL0_CR9_EN36                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,0)))
#define TSB_TSEL0_CR9_OUTSEL36                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,1)))
#define TSB_TSEL0_CR9_UPDN36                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,2)))
#define TSB_TSEL0_CR9_EN37                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,8)))
#define TSB_TSEL0_CR9_OUTSEL37                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,9)))
#define TSB_TSEL0_CR9_UPDN37                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,10)))
#define TSB_TSEL0_CR9_EN38                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,16)))
#define TSB_TSEL0_CR9_OUTSEL38                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,17)))
#define TSB_TSEL0_CR9_UPDN38                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,18)))
#define TSB_TSEL0_CR9_EN39                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,24)))
#define TSB_TSEL0_CR9_OUTSEL39                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,25)))
#define TSB_TSEL0_CR9_UPDN39                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR9,26)))
#define TSB_TSEL0_CR10_EN40                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR10,0)))
#define TSB_TSEL0_CR10_OUTSEL40                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR10,1)))
#define TSB_TSEL0_CR10_UPDN40                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR10,2)))
#define TSB_TSEL0_CR10_EN41                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR10,8)))
#define TSB_TSEL0_CR10_OUTSEL41                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR10,9)))
#define TSB_TSEL0_CR10_UPDN41                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR10,10)))
#define TSB_TSEL0_CR10_EN42                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR10,16)))
#define TSB_TSEL0_CR10_OUTSEL42                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR10,17)))
#define TSB_TSEL0_CR10_UPDN42                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR10,18)))
#define TSB_TSEL0_CR10_EN43                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR10,24)))
#define TSB_TSEL0_CR10_OUTSEL43                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR10,25)))
#define TSB_TSEL0_CR10_UPDN43                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR10,26)))
#define TSB_TSEL0_CR11_EN44                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR11,0)))
#define TSB_TSEL0_CR11_OUTSEL44                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR11,1)))
#define TSB_TSEL0_CR11_UPDN44                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR11,2)))
#define TSB_TSEL0_CR11_EN45                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR11,8)))
#define TSB_TSEL0_CR11_OUTSEL45                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR11,9)))
#define TSB_TSEL0_CR11_UPDN45                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR11,10)))
#define TSB_TSEL0_CR11_EN46                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR11,16)))
#define TSB_TSEL0_CR11_OUTSEL46                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR11,17)))
#define TSB_TSEL0_CR11_UPDN46                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR11,18)))
#define TSB_TSEL0_CR11_EN47                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR11,24)))
#define TSB_TSEL0_CR11_OUTSEL47                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR11,25)))
#define TSB_TSEL0_CR11_UPDN47                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR11,26)))
#define TSB_TSEL0_CR12_EN48                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR12,0)))
#define TSB_TSEL0_CR12_OUTSEL48                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR12,1)))
#define TSB_TSEL0_CR12_UPDN48                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR12,2)))
#define TSB_TSEL0_CR12_EN49                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR12,8)))
#define TSB_TSEL0_CR12_OUTSEL49                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR12,9)))
#define TSB_TSEL0_CR12_UPDN49                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR12,10)))
#define TSB_TSEL0_CR12_EN50                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR12,16)))
#define TSB_TSEL0_CR12_OUTSEL50                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR12,17)))
#define TSB_TSEL0_CR12_UPDN50                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR12,18)))
#define TSB_TSEL0_CR12_EN51                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR12,24)))
#define TSB_TSEL0_CR12_OUTSEL51                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR12,25)))
#define TSB_TSEL0_CR12_UPDN51                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR12,26)))
#define TSB_TSEL0_CR13_EN52                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR13,0)))
#define TSB_TSEL0_CR13_OUTSEL52                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR13,1)))
#define TSB_TSEL0_CR13_UPDN52                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR13,2)))
#define TSB_TSEL0_CR13_EN53                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR13,8)))
#define TSB_TSEL0_CR13_OUTSEL53                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR13,9)))
#define TSB_TSEL0_CR13_UPDN53                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR13,10)))
#define TSB_TSEL0_CR13_EN54                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR13,16)))
#define TSB_TSEL0_CR13_OUTSEL54                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR13,17)))
#define TSB_TSEL0_CR13_UPDN54                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR13,18)))
#define TSB_TSEL0_CR13_EN55                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR13,24)))
#define TSB_TSEL0_CR13_OUTSEL55                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR13,25)))
#define TSB_TSEL0_CR13_UPDN55                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSEL0->CR13,26)))



/* Serial Interface (TSPI) */
#define TSB_TSPI0_CR0_TSPIE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR0,0)))
#define TSB_TSPI0_CR1_MSTR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR1,12)))
#define TSB_TSPI0_CR1_TSPIMS                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR1,13)))
#define TSB_TSPI0_CR1_TRXE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR1,14)))
#define TSB_TSPI0_CR1_TRGIN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR1,15)))
#define TSB_TSPI0_CR2_DMARE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,0)))
#define TSB_TSPI0_CR2_DMATE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,1)))
#define TSB_TSPI0_CR2_INTERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,2)))
#define TSB_TSPI0_CR2_INTRXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,4)))
#define TSB_TSPI0_CR2_INTRXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,5)))
#define TSB_TSPI0_CR2_INTTXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,6)))
#define TSB_TSPI0_CR2_INTTXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,7)))
#define TSB_TSPI0_CR2_RXDLY                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,16)))
#define TSB_TSPI0_CR2_TXDEMP                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR2,21)))
#define TSB_TSPI0_CR3_RFFLLCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR3,0)))
#define TSB_TSPI0_CR3_TFEMPCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI0->CR3,1)))
#define TSB_TSPI0_FMTR0_CKPOL                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR0,14)))
#define TSB_TSPI0_FMTR0_CKPHA                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR0,15)))
#define TSB_TSPI0_FMTR0_CS0POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR0,16)))
#define TSB_TSPI0_FMTR0_CS1POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR0,17)))
#define TSB_TSPI0_FMTR0_CS2POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR0,18)))
#define TSB_TSPI0_FMTR0_CS3POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR0,19)))
#define TSB_TSPI0_FMTR0_DIR                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR0,31)))
#define TSB_TSPI0_FMTR1_VPM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR1,0)))
#define TSB_TSPI0_FMTR1_VPE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->FMTR1,1)))
#define TSB_TSPI0_SR_RFFLL                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,4)))
#define TSB_TSPI0_SR_INTRXFF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,5)))
#define TSB_TSPI0_SR_RXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,6)))
#define TSB_TSPI0_SR_RXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,7)))
#define TSB_TSPI0_SR_TFEMP                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,20)))
#define TSB_TSPI0_SR_INTTXWF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,21)))
#define TSB_TSPI0_SR_TXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,22)))
#define TSB_TSPI0_SR_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,23)))
#define TSB_TSPI0_SR_TSPISUE                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI0->SR,31)))
#define TSB_TSPI0_ERR_OVRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->ERR,1)))
#define TSB_TSPI0_ERR_UDRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->ERR,2)))
#define TSB_TSPI0_ERR_TRGERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI0->ERR,3)))

#define TSB_TSPI1_CR0_TSPIE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR0,0)))
#define TSB_TSPI1_CR1_MSTR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR1,12)))
#define TSB_TSPI1_CR1_TSPIMS                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR1,13)))
#define TSB_TSPI1_CR1_TRXE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR1,14)))
#define TSB_TSPI1_CR1_TRGIN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR1,15)))
#define TSB_TSPI1_CR2_DMARE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR2,0)))
#define TSB_TSPI1_CR2_DMATE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR2,1)))
#define TSB_TSPI1_CR2_INTERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR2,2)))
#define TSB_TSPI1_CR2_INTRXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR2,4)))
#define TSB_TSPI1_CR2_INTRXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR2,5)))
#define TSB_TSPI1_CR2_INTTXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR2,6)))
#define TSB_TSPI1_CR2_INTTXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR2,7)))
#define TSB_TSPI1_CR2_RXDLY                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR2,16)))
#define TSB_TSPI1_CR2_TXDEMP                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR2,21)))
#define TSB_TSPI1_CR3_RFFLLCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR3,0)))
#define TSB_TSPI1_CR3_TFEMPCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI1->CR3,1)))
#define TSB_TSPI1_FMTR0_CKPOL                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->FMTR0,14)))
#define TSB_TSPI1_FMTR0_CKPHA                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->FMTR0,15)))
#define TSB_TSPI1_FMTR0_CS0POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->FMTR0,16)))
#define TSB_TSPI1_FMTR0_CS1POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->FMTR0,17)))
#define TSB_TSPI1_FMTR0_CS2POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->FMTR0,18)))
#define TSB_TSPI1_FMTR0_CS3POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->FMTR0,19)))
#define TSB_TSPI1_FMTR0_DIR                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->FMTR0,31)))
#define TSB_TSPI1_FMTR1_VPM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->FMTR1,0)))
#define TSB_TSPI1_FMTR1_VPE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->FMTR1,1)))
#define TSB_TSPI1_SR_RFFLL                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI1->SR,4)))
#define TSB_TSPI1_SR_INTRXFF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->SR,5)))
#define TSB_TSPI1_SR_RXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->SR,6)))
#define TSB_TSPI1_SR_RXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI1->SR,7)))
#define TSB_TSPI1_SR_TFEMP                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI1->SR,20)))
#define TSB_TSPI1_SR_INTTXWF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->SR,21)))
#define TSB_TSPI1_SR_TXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->SR,22)))
#define TSB_TSPI1_SR_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI1->SR,23)))
#define TSB_TSPI1_SR_TSPISUE                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI1->SR,31)))
#define TSB_TSPI1_ERR_OVRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->ERR,1)))
#define TSB_TSPI1_ERR_UDRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->ERR,2)))
#define TSB_TSPI1_ERR_TRGERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI1->ERR,3)))

#define TSB_TSPI2_CR0_TSPIE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->CR0,0)))
#define TSB_TSPI2_CR1_MSTR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->CR1,12)))
#define TSB_TSPI2_CR1_TSPIMS                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->CR1,13)))
#define TSB_TSPI2_CR1_TRXE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->CR1,14)))
#define TSB_TSPI2_CR1_TRGIN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->CR1,15)))
#define TSB_TSPI2_CR2_DMARE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->CR2,0)))
#define TSB_TSPI2_CR2_DMATE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->CR2,1)))
#define TSB_TSPI2_CR2_INTERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->CR2,2)))
#define TSB_TSPI2_CR2_INTRXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->CR2,4)))
#define TSB_TSPI2_CR2_INTRXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->CR2,5)))
#define TSB_TSPI2_CR2_INTTXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->CR2,6)))
#define TSB_TSPI2_CR2_INTTXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->CR2,7)))
#define TSB_TSPI2_CR2_RXDLY                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->CR2,16)))
#define TSB_TSPI2_CR2_TXDEMP                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->CR2,21)))
#define TSB_TSPI2_CR3_RFFLLCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI2->CR3,0)))
#define TSB_TSPI2_CR3_TFEMPCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI2->CR3,1)))
#define TSB_TSPI2_FMTR0_CKPOL                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->FMTR0,14)))
#define TSB_TSPI2_FMTR0_CKPHA                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->FMTR0,15)))
#define TSB_TSPI2_FMTR0_CS0POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->FMTR0,16)))
#define TSB_TSPI2_FMTR0_CS1POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->FMTR0,17)))
#define TSB_TSPI2_FMTR0_CS2POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->FMTR0,18)))
#define TSB_TSPI2_FMTR0_CS3POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->FMTR0,19)))
#define TSB_TSPI2_FMTR0_DIR                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->FMTR0,31)))
#define TSB_TSPI2_FMTR1_VPM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->FMTR1,0)))
#define TSB_TSPI2_FMTR1_VPE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->FMTR1,1)))
#define TSB_TSPI2_SR_RFFLL                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI2->SR,4)))
#define TSB_TSPI2_SR_INTRXFF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->SR,5)))
#define TSB_TSPI2_SR_RXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->SR,6)))
#define TSB_TSPI2_SR_RXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI2->SR,7)))
#define TSB_TSPI2_SR_TFEMP                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI2->SR,20)))
#define TSB_TSPI2_SR_INTTXWF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->SR,21)))
#define TSB_TSPI2_SR_TXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->SR,22)))
#define TSB_TSPI2_SR_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI2->SR,23)))
#define TSB_TSPI2_SR_TSPISUE                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI2->SR,31)))
#define TSB_TSPI2_ERR_OVRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->ERR,1)))
#define TSB_TSPI2_ERR_UDRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->ERR,2)))
#define TSB_TSPI2_ERR_TRGERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI2->ERR,3)))

#define TSB_TSPI3_CR0_TSPIE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->CR0,0)))
#define TSB_TSPI3_CR1_MSTR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->CR1,12)))
#define TSB_TSPI3_CR1_TSPIMS                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->CR1,13)))
#define TSB_TSPI3_CR1_TRXE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->CR1,14)))
#define TSB_TSPI3_CR1_TRGIN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->CR1,15)))
#define TSB_TSPI3_CR2_DMARE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->CR2,0)))
#define TSB_TSPI3_CR2_DMATE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->CR2,1)))
#define TSB_TSPI3_CR2_INTERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->CR2,2)))
#define TSB_TSPI3_CR2_INTRXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->CR2,4)))
#define TSB_TSPI3_CR2_INTRXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->CR2,5)))
#define TSB_TSPI3_CR2_INTTXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->CR2,6)))
#define TSB_TSPI3_CR2_INTTXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->CR2,7)))
#define TSB_TSPI3_CR2_RXDLY                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->CR2,16)))
#define TSB_TSPI3_CR2_TXDEMP                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->CR2,21)))
#define TSB_TSPI3_CR3_RFFLLCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI3->CR3,0)))
#define TSB_TSPI3_CR3_TFEMPCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI3->CR3,1)))
#define TSB_TSPI3_FMTR0_CKPOL                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->FMTR0,14)))
#define TSB_TSPI3_FMTR0_CKPHA                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->FMTR0,15)))
#define TSB_TSPI3_FMTR0_CS0POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->FMTR0,16)))
#define TSB_TSPI3_FMTR0_CS1POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->FMTR0,17)))
#define TSB_TSPI3_FMTR0_CS2POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->FMTR0,18)))
#define TSB_TSPI3_FMTR0_CS3POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->FMTR0,19)))
#define TSB_TSPI3_FMTR0_DIR                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->FMTR0,31)))
#define TSB_TSPI3_FMTR1_VPM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->FMTR1,0)))
#define TSB_TSPI3_FMTR1_VPE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->FMTR1,1)))
#define TSB_TSPI3_SR_RFFLL                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI3->SR,4)))
#define TSB_TSPI3_SR_INTRXFF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->SR,5)))
#define TSB_TSPI3_SR_RXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->SR,6)))
#define TSB_TSPI3_SR_RXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI3->SR,7)))
#define TSB_TSPI3_SR_TFEMP                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI3->SR,20)))
#define TSB_TSPI3_SR_INTTXWF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->SR,21)))
#define TSB_TSPI3_SR_TXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->SR,22)))
#define TSB_TSPI3_SR_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI3->SR,23)))
#define TSB_TSPI3_SR_TSPISUE                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI3->SR,31)))
#define TSB_TSPI3_ERR_OVRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->ERR,1)))
#define TSB_TSPI3_ERR_UDRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->ERR,2)))
#define TSB_TSPI3_ERR_TRGERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI3->ERR,3)))

#define TSB_TSPI4_CR0_TSPIE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->CR0,0)))
#define TSB_TSPI4_CR1_MSTR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->CR1,12)))
#define TSB_TSPI4_CR1_TSPIMS                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->CR1,13)))
#define TSB_TSPI4_CR1_TRXE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->CR1,14)))
#define TSB_TSPI4_CR1_TRGIN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->CR1,15)))
#define TSB_TSPI4_CR2_DMARE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->CR2,0)))
#define TSB_TSPI4_CR2_DMATE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->CR2,1)))
#define TSB_TSPI4_CR2_INTERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->CR2,2)))
#define TSB_TSPI4_CR2_INTRXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->CR2,4)))
#define TSB_TSPI4_CR2_INTRXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->CR2,5)))
#define TSB_TSPI4_CR2_INTTXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->CR2,6)))
#define TSB_TSPI4_CR2_INTTXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->CR2,7)))
#define TSB_TSPI4_CR2_RXDLY                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->CR2,16)))
#define TSB_TSPI4_CR2_TXDEMP                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->CR2,21)))
#define TSB_TSPI4_CR3_RFFLLCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI4->CR3,0)))
#define TSB_TSPI4_CR3_TFEMPCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI4->CR3,1)))
#define TSB_TSPI4_FMTR0_CKPOL                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->FMTR0,14)))
#define TSB_TSPI4_FMTR0_CKPHA                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->FMTR0,15)))
#define TSB_TSPI4_FMTR0_CS0POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->FMTR0,16)))
#define TSB_TSPI4_FMTR0_CS1POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->FMTR0,17)))
#define TSB_TSPI4_FMTR0_CS2POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->FMTR0,18)))
#define TSB_TSPI4_FMTR0_CS3POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->FMTR0,19)))
#define TSB_TSPI4_FMTR0_DIR                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->FMTR0,31)))
#define TSB_TSPI4_FMTR1_VPM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->FMTR1,0)))
#define TSB_TSPI4_FMTR1_VPE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->FMTR1,1)))
#define TSB_TSPI4_SR_RFFLL                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI4->SR,4)))
#define TSB_TSPI4_SR_INTRXFF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->SR,5)))
#define TSB_TSPI4_SR_RXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->SR,6)))
#define TSB_TSPI4_SR_RXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI4->SR,7)))
#define TSB_TSPI4_SR_TFEMP                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI4->SR,20)))
#define TSB_TSPI4_SR_INTTXWF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->SR,21)))
#define TSB_TSPI4_SR_TXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->SR,22)))
#define TSB_TSPI4_SR_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI4->SR,23)))
#define TSB_TSPI4_SR_TSPISUE                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI4->SR,31)))
#define TSB_TSPI4_ERR_OVRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->ERR,1)))
#define TSB_TSPI4_ERR_UDRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->ERR,2)))
#define TSB_TSPI4_ERR_TRGERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI4->ERR,3)))

#define TSB_TSPI5_CR0_TSPIE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->CR0,0)))
#define TSB_TSPI5_CR1_MSTR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->CR1,12)))
#define TSB_TSPI5_CR1_TSPIMS                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->CR1,13)))
#define TSB_TSPI5_CR1_TRXE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->CR1,14)))
#define TSB_TSPI5_CR1_TRGIN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->CR1,15)))
#define TSB_TSPI5_CR2_DMARE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->CR2,0)))
#define TSB_TSPI5_CR2_DMATE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->CR2,1)))
#define TSB_TSPI5_CR2_INTERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->CR2,2)))
#define TSB_TSPI5_CR2_INTRXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->CR2,4)))
#define TSB_TSPI5_CR2_INTRXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->CR2,5)))
#define TSB_TSPI5_CR2_INTTXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->CR2,6)))
#define TSB_TSPI5_CR2_INTTXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->CR2,7)))
#define TSB_TSPI5_CR2_RXDLY                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->CR2,16)))
#define TSB_TSPI5_CR2_TXDEMP                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->CR2,21)))
#define TSB_TSPI5_CR3_RFFLLCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI5->CR3,0)))
#define TSB_TSPI5_CR3_TFEMPCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI5->CR3,1)))
#define TSB_TSPI5_FMTR0_CKPOL                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->FMTR0,14)))
#define TSB_TSPI5_FMTR0_CKPHA                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->FMTR0,15)))
#define TSB_TSPI5_FMTR0_CS0POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->FMTR0,16)))
#define TSB_TSPI5_FMTR0_CS1POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->FMTR0,17)))
#define TSB_TSPI5_FMTR0_CS2POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->FMTR0,18)))
#define TSB_TSPI5_FMTR0_CS3POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->FMTR0,19)))
#define TSB_TSPI5_FMTR0_DIR                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->FMTR0,31)))
#define TSB_TSPI5_FMTR1_VPM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->FMTR1,0)))
#define TSB_TSPI5_FMTR1_VPE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->FMTR1,1)))
#define TSB_TSPI5_SR_RFFLL                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI5->SR,4)))
#define TSB_TSPI5_SR_INTRXFF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->SR,5)))
#define TSB_TSPI5_SR_RXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->SR,6)))
#define TSB_TSPI5_SR_RXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI5->SR,7)))
#define TSB_TSPI5_SR_TFEMP                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI5->SR,20)))
#define TSB_TSPI5_SR_INTTXWF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->SR,21)))
#define TSB_TSPI5_SR_TXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->SR,22)))
#define TSB_TSPI5_SR_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI5->SR,23)))
#define TSB_TSPI5_SR_TSPISUE                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI5->SR,31)))
#define TSB_TSPI5_ERR_OVRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->ERR,1)))
#define TSB_TSPI5_ERR_UDRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->ERR,2)))
#define TSB_TSPI5_ERR_TRGERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI5->ERR,3)))

#define TSB_TSPI6_CR0_TSPIE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->CR0,0)))
#define TSB_TSPI6_CR1_MSTR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->CR1,12)))
#define TSB_TSPI6_CR1_TSPIMS                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->CR1,13)))
#define TSB_TSPI6_CR1_TRXE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->CR1,14)))
#define TSB_TSPI6_CR1_TRGIN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->CR1,15)))
#define TSB_TSPI6_CR2_DMARE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->CR2,0)))
#define TSB_TSPI6_CR2_DMATE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->CR2,1)))
#define TSB_TSPI6_CR2_INTERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->CR2,2)))
#define TSB_TSPI6_CR2_INTRXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->CR2,4)))
#define TSB_TSPI6_CR2_INTRXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->CR2,5)))
#define TSB_TSPI6_CR2_INTTXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->CR2,6)))
#define TSB_TSPI6_CR2_INTTXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->CR2,7)))
#define TSB_TSPI6_CR2_RXDLY                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->CR2,16)))
#define TSB_TSPI6_CR2_TXDEMP                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->CR2,21)))
#define TSB_TSPI6_CR3_RFFLLCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI6->CR3,0)))
#define TSB_TSPI6_CR3_TFEMPCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI6->CR3,1)))
#define TSB_TSPI6_FMTR0_CKPOL                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->FMTR0,14)))
#define TSB_TSPI6_FMTR0_CKPHA                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->FMTR0,15)))
#define TSB_TSPI6_FMTR0_CS0POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->FMTR0,16)))
#define TSB_TSPI6_FMTR0_CS1POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->FMTR0,17)))
#define TSB_TSPI6_FMTR0_CS2POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->FMTR0,18)))
#define TSB_TSPI6_FMTR0_CS3POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->FMTR0,19)))
#define TSB_TSPI6_FMTR0_DIR                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->FMTR0,31)))
#define TSB_TSPI6_FMTR1_VPM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->FMTR1,0)))
#define TSB_TSPI6_FMTR1_VPE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->FMTR1,1)))
#define TSB_TSPI6_SR_RFFLL                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI6->SR,4)))
#define TSB_TSPI6_SR_INTRXFF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->SR,5)))
#define TSB_TSPI6_SR_RXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->SR,6)))
#define TSB_TSPI6_SR_RXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI6->SR,7)))
#define TSB_TSPI6_SR_TFEMP                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI6->SR,20)))
#define TSB_TSPI6_SR_INTTXWF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->SR,21)))
#define TSB_TSPI6_SR_TXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->SR,22)))
#define TSB_TSPI6_SR_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI6->SR,23)))
#define TSB_TSPI6_SR_TSPISUE                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI6->SR,31)))
#define TSB_TSPI6_ERR_OVRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->ERR,1)))
#define TSB_TSPI6_ERR_UDRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->ERR,2)))
#define TSB_TSPI6_ERR_TRGERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI6->ERR,3)))

#define TSB_TSPI7_CR0_TSPIE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->CR0,0)))
#define TSB_TSPI7_CR1_MSTR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->CR1,12)))
#define TSB_TSPI7_CR1_TSPIMS                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->CR1,13)))
#define TSB_TSPI7_CR1_TRXE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->CR1,14)))
#define TSB_TSPI7_CR1_TRGIN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->CR1,15)))
#define TSB_TSPI7_CR2_DMARE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->CR2,0)))
#define TSB_TSPI7_CR2_DMATE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->CR2,1)))
#define TSB_TSPI7_CR2_INTERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->CR2,2)))
#define TSB_TSPI7_CR2_INTRXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->CR2,4)))
#define TSB_TSPI7_CR2_INTRXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->CR2,5)))
#define TSB_TSPI7_CR2_INTTXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->CR2,6)))
#define TSB_TSPI7_CR2_INTTXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->CR2,7)))
#define TSB_TSPI7_CR2_RXDLY                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->CR2,16)))
#define TSB_TSPI7_CR2_TXDEMP                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->CR2,21)))
#define TSB_TSPI7_CR3_RFFLLCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI7->CR3,0)))
#define TSB_TSPI7_CR3_TFEMPCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI7->CR3,1)))
#define TSB_TSPI7_FMTR0_CKPOL                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->FMTR0,14)))
#define TSB_TSPI7_FMTR0_CKPHA                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->FMTR0,15)))
#define TSB_TSPI7_FMTR0_CS0POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->FMTR0,16)))
#define TSB_TSPI7_FMTR0_CS1POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->FMTR0,17)))
#define TSB_TSPI7_FMTR0_CS2POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->FMTR0,18)))
#define TSB_TSPI7_FMTR0_CS3POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->FMTR0,19)))
#define TSB_TSPI7_FMTR0_DIR                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->FMTR0,31)))
#define TSB_TSPI7_FMTR1_VPM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->FMTR1,0)))
#define TSB_TSPI7_FMTR1_VPE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->FMTR1,1)))
#define TSB_TSPI7_SR_RFFLL                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI7->SR,4)))
#define TSB_TSPI7_SR_INTRXFF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->SR,5)))
#define TSB_TSPI7_SR_RXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->SR,6)))
#define TSB_TSPI7_SR_RXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI7->SR,7)))
#define TSB_TSPI7_SR_TFEMP                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI7->SR,20)))
#define TSB_TSPI7_SR_INTTXWF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->SR,21)))
#define TSB_TSPI7_SR_TXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->SR,22)))
#define TSB_TSPI7_SR_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI7->SR,23)))
#define TSB_TSPI7_SR_TSPISUE                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI7->SR,31)))
#define TSB_TSPI7_ERR_OVRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->ERR,1)))
#define TSB_TSPI7_ERR_UDRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->ERR,2)))
#define TSB_TSPI7_ERR_TRGERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI7->ERR,3)))

#define TSB_TSPI8_CR0_TSPIE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->CR0,0)))
#define TSB_TSPI8_CR1_MSTR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->CR1,12)))
#define TSB_TSPI8_CR1_TSPIMS                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->CR1,13)))
#define TSB_TSPI8_CR1_TRXE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->CR1,14)))
#define TSB_TSPI8_CR1_TRGIN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->CR1,15)))
#define TSB_TSPI8_CR2_DMARE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->CR2,0)))
#define TSB_TSPI8_CR2_DMATE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->CR2,1)))
#define TSB_TSPI8_CR2_INTERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->CR2,2)))
#define TSB_TSPI8_CR2_INTRXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->CR2,4)))
#define TSB_TSPI8_CR2_INTRXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->CR2,5)))
#define TSB_TSPI8_CR2_INTTXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->CR2,6)))
#define TSB_TSPI8_CR2_INTTXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->CR2,7)))
#define TSB_TSPI8_CR2_RXDLY                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->CR2,16)))
#define TSB_TSPI8_CR2_TXDEMP                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->CR2,21)))
#define TSB_TSPI8_CR3_RFFLLCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI8->CR3,0)))
#define TSB_TSPI8_CR3_TFEMPCLR                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_TSPI8->CR3,1)))
#define TSB_TSPI8_FMTR0_CKPOL                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->FMTR0,14)))
#define TSB_TSPI8_FMTR0_CKPHA                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->FMTR0,15)))
#define TSB_TSPI8_FMTR0_CS0POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->FMTR0,16)))
#define TSB_TSPI8_FMTR0_CS1POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->FMTR0,17)))
#define TSB_TSPI8_FMTR0_CS2POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->FMTR0,18)))
#define TSB_TSPI8_FMTR0_CS3POL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->FMTR0,19)))
#define TSB_TSPI8_FMTR0_DIR                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->FMTR0,31)))
#define TSB_TSPI8_FMTR1_VPM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->FMTR1,0)))
#define TSB_TSPI8_FMTR1_VPE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->FMTR1,1)))
#define TSB_TSPI8_SR_RFFLL                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI8->SR,4)))
#define TSB_TSPI8_SR_INTRXFF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->SR,5)))
#define TSB_TSPI8_SR_RXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->SR,6)))
#define TSB_TSPI8_SR_RXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI8->SR,7)))
#define TSB_TSPI8_SR_TFEMP                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI8->SR,20)))
#define TSB_TSPI8_SR_INTTXWF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->SR,21)))
#define TSB_TSPI8_SR_TXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->SR,22)))
#define TSB_TSPI8_SR_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI8->SR,23)))
#define TSB_TSPI8_SR_TSPISUE                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_TSPI8->SR,31)))
#define TSB_TSPI8_ERR_OVRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->ERR,1)))
#define TSB_TSPI8_ERR_UDRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->ERR,2)))
#define TSB_TSPI8_ERR_TRGERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TSPI8->ERR,3)))


/* External Bus Interface(EXB) */
#define TSB_EXB_MOD_EXBSEL                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->MOD,0)))
#define TSB_EXB_CS0_CSW0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS0,0)))
#define TSB_EXB_CS0_WAIT                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS0,12)))
#define TSB_EXB_CS0_WSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS0,13)))
#define TSB_EXB_CS1_CSW0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS1,0)))
#define TSB_EXB_CS1_WAIT                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS1,12)))
#define TSB_EXB_CS1_WSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS1,13)))
#define TSB_EXB_CS2_CSW0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS2,0)))
#define TSB_EXB_CS2_WAIT                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS2,12)))
#define TSB_EXB_CS2_WSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS2,13)))
#define TSB_EXB_CS3_CSW0                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS3,0)))
#define TSB_EXB_CS3_WAIT                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS3,12)))
#define TSB_EXB_CS3_WSEL                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CS3,13)))
#define TSB_EXB_CLKCTL_CLKEN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_EXB->CLKCTL,0)))


/* Clock Generator (CG) */
#define TSB_CG_OSCCR_IHOSC1EN                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,0)))
#define TSB_CG_OSCCR_OSCSEL                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,8)))
#define TSB_CG_OSCCR_OSCF                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,9)))
#define TSB_CG_OSCCR_IHOSC1F                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_CG->OSCCR,16)))
#define TSB_CG_PLL0SEL_PLL0ON                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->PLL0SEL,0)))
#define TSB_CG_PLL0SEL_PLL0SEL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->PLL0SEL,1)))
#define TSB_CG_PLL0SEL_PLL0ST                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_CG->PLL0SEL,2)))
#define TSB_CG_WUPHCR_WUON                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_CG->WUPHCR,0)))
#define TSB_CG_WUPHCR_WUEF                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_CG->WUPHCR,1)))
#define TSB_CG_WUPHCR_WUCLK                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->WUPHCR,8)))
#define TSB_CG_WUPLCR_WULON                       (*((__O  uint32_t *)BITBAND_PERI(&TSB_CG->WUPLCR,0)))
#define TSB_CG_WUPLCR_WULEF                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_CG->WUPLCR,1)))
#define TSB_CG_FSYSMENA_IPMENA00                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,0)))
#define TSB_CG_FSYSMENA_IPMENA01                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,1)))
#define TSB_CG_FSYSMENA_IPMENA02                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,2)))
#define TSB_CG_FSYSMENA_IPMENA03                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,3)))
#define TSB_CG_FSYSMENA_IPMENA04                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,4)))
#define TSB_CG_FSYSMENA_IPMENA05                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,5)))
#define TSB_CG_FSYSMENA_IPMENA06                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,6)))
#define TSB_CG_FSYSMENA_IPMENA07                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,7)))
#define TSB_CG_FSYSMENA_IPMENA08                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,8)))
#define TSB_CG_FSYSMENA_IPMENA09                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,9)))
#define TSB_CG_FSYSMENA_IPMENA10                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,10)))
#define TSB_CG_FSYSMENA_IPMENA11                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,11)))
#define TSB_CG_FSYSMENA_IPMENA12                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,12)))
#define TSB_CG_FSYSMENA_IPMENA13                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,13)))
#define TSB_CG_FSYSMENA_IPMENA14                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,14)))
#define TSB_CG_FSYSMENA_IPMENA15                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,15)))
#define TSB_CG_FSYSMENA_IPMENA16                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,16)))
#define TSB_CG_FSYSMENA_IPMENA17                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,17)))
#define TSB_CG_FSYSMENA_IPMENA18                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,18)))
#define TSB_CG_FSYSMENA_IPMENA19                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,19)))
#define TSB_CG_FSYSMENA_IPMENA20                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,20)))
#define TSB_CG_FSYSMENA_IPMENA21                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,21)))
#define TSB_CG_FSYSMENA_IPMENA22                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,22)))
#define TSB_CG_FSYSMENA_IPMENA23                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,23)))
#define TSB_CG_FSYSMENA_IPMENA24                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,24)))
#define TSB_CG_FSYSMENA_IPMENA25                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,25)))
#define TSB_CG_FSYSMENA_IPMENA26                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,26)))
#define TSB_CG_FSYSMENA_IPMENA27                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,27)))
#define TSB_CG_FSYSMENA_IPMENA28                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,28)))
#define TSB_CG_FSYSMENA_IPMENA29                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,29)))
#define TSB_CG_FSYSMENA_IPMENA30                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,30)))
#define TSB_CG_FSYSMENA_IPMENA31                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENA,31)))
#define TSB_CG_FSYSMENB_IPMENB00                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,0)))
#define TSB_CG_FSYSMENB_IPMENB01                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,1)))
#define TSB_CG_FSYSMENB_IPMENB02                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,2)))
#define TSB_CG_FSYSMENB_IPMENB03                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,3)))
#define TSB_CG_FSYSMENB_IPMENB04                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,4)))
#define TSB_CG_FSYSMENB_IPMENB05                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,5)))
#define TSB_CG_FSYSMENB_IPMENB06                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,6)))
#define TSB_CG_FSYSMENB_IPMENB07                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,7)))
#define TSB_CG_FSYSMENB_IPMENB08                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,8)))
#define TSB_CG_FSYSMENB_IPMENB09                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,9)))
#define TSB_CG_FSYSMENB_IPMENB10                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,10)))
#define TSB_CG_FSYSMENB_IPMENB11                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,11)))
#define TSB_CG_FSYSMENB_IPMENB12                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,12)))
#define TSB_CG_FSYSMENB_IPMENB13                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,13)))
#define TSB_CG_FSYSMENB_IPMENB14                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,14)))
#define TSB_CG_FSYSMENB_IPMENB15                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,15)))
#define TSB_CG_FSYSMENB_IPMENB16                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,16)))
#define TSB_CG_FSYSMENB_IPMENB17                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,17)))
#define TSB_CG_FSYSMENB_IPMENB18                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,18)))
#define TSB_CG_FSYSMENB_IPMENB19                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,19)))
#define TSB_CG_FSYSMENB_IPMENB20                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,20)))
#define TSB_CG_FSYSMENB_IPMENB21                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,21)))
#define TSB_CG_FSYSMENB_IPMENB22                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,22)))
#define TSB_CG_FSYSMENB_IPMENB23                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,23)))
#define TSB_CG_FSYSMENB_IPMENB24                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,24)))
#define TSB_CG_FSYSMENB_IPMENB28                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,28)))
#define TSB_CG_FSYSMENB_IPMENB29                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,29)))
#define TSB_CG_FSYSMENB_IPMENB30                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,30)))
#define TSB_CG_FSYSMENB_IPMENB31                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSMENB,31)))
#define TSB_CG_FSYSENA_IPENA00                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,0)))
#define TSB_CG_FSYSENA_IPENA01                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,1)))
#define TSB_CG_FSYSENA_IPENA02                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,2)))
#define TSB_CG_FSYSENA_IPENA03                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,3)))
#define TSB_CG_FSYSENA_IPENA04                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,4)))
#define TSB_CG_FSYSENA_IPENA05                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,5)))
#define TSB_CG_FSYSENA_IPENA06                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,6)))
#define TSB_CG_FSYSENA_IPENA07                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,7)))
#define TSB_CG_FSYSENA_IPENA08                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,8)))
#define TSB_CG_FSYSENA_IPENA09                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,9)))
#define TSB_CG_FSYSENA_IPENA10                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,10)))
#define TSB_CG_FSYSENA_IPENA11                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,11)))
#define TSB_CG_FSYSENA_IPENA12                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FSYSENA,12)))
#define TSB_CG_FCEN_FCIPEN23                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FCEN,23)))
#define TSB_CG_FCEN_FCIPEN26                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FCEN,26)))
#define TSB_CG_FCEN_FCIPEN27                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->FCEN,27)))
#define TSB_CG_SPCLKEN_TRCKEN                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->SPCLKEN,0)))
#define TSB_CG_SPCLKEN_ADCKEN                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->SPCLKEN,16)))
#define TSB_CG_EXTEND2_RSV20                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->EXTEND2,0)))
#define TSB_CG_EXTEND2_RSV21                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->EXTEND2,1)))
#define TSB_CG_EXTEND2_RSV22                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CG->EXTEND2,2)))



/* Interrupt Monitor Register */
#define TSB_IMN_FLGNMI_INT000FLG                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLGNMI,0)))
#define TSB_IMN_FLGNMI_INT016FLG                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLGNMI,16)))
#define TSB_IMN_FLG1_INT032FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,0)))
#define TSB_IMN_FLG1_INT033FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,1)))
#define TSB_IMN_FLG1_INT034FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,2)))
#define TSB_IMN_FLG1_INT035FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,3)))
#define TSB_IMN_FLG1_INT036FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,4)))
#define TSB_IMN_FLG1_INT037FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,5)))
#define TSB_IMN_FLG1_INT038FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,6)))
#define TSB_IMN_FLG1_INT039FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,7)))
#define TSB_IMN_FLG1_INT040FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,8)))
#define TSB_IMN_FLG1_INT041FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,9)))
#define TSB_IMN_FLG1_INT042FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,10)))
#define TSB_IMN_FLG1_INT043FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,11)))
#define TSB_IMN_FLG1_INT044FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,12)))
#define TSB_IMN_FLG1_INT045FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,13)))
#define TSB_IMN_FLG1_INT046FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,14)))
#define TSB_IMN_FLG1_INT047FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,15)))
#define TSB_IMN_FLG1_INT048FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,16)))
#define TSB_IMN_FLG1_INT049FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,17)))
#define TSB_IMN_FLG1_INT050FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,18)))
#define TSB_IMN_FLG1_INT051FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,19)))
#define TSB_IMN_FLG1_INT052FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,20)))
#define TSB_IMN_FLG1_INT053FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,21)))
#define TSB_IMN_FLG1_INT054FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,22)))
#define TSB_IMN_FLG1_INT055FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,23)))
#define TSB_IMN_FLG1_INT056FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,24)))
#define TSB_IMN_FLG1_INT057FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,25)))
#define TSB_IMN_FLG1_INT058FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,26)))
#define TSB_IMN_FLG1_INT059FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,27)))
#define TSB_IMN_FLG1_INT060FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,28)))
#define TSB_IMN_FLG1_INT061FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,29)))
#define TSB_IMN_FLG1_INT062FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,30)))
#define TSB_IMN_FLG1_INT063FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG1,31)))
#define TSB_IMN_FLG2_INT081FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG2,17)))
#define TSB_IMN_FLG2_INT082FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG2,18)))
#define TSB_IMN_FLG2_INT083FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG2,19)))
#define TSB_IMN_FLG2_INT084FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG2,20)))
#define TSB_IMN_FLG2_INT085FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG2,21)))
#define TSB_IMN_FLG2_INT086FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG2,22)))
#define TSB_IMN_FLG2_INT087FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG2,23)))
#define TSB_IMN_FLG2_INT088FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG2,24)))
#define TSB_IMN_FLG2_INT089FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG2,25)))
#define TSB_IMN_FLG3_INT096FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,0)))
#define TSB_IMN_FLG3_INT097FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,1)))
#define TSB_IMN_FLG3_INT098FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,2)))
#define TSB_IMN_FLG3_INT099FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,3)))
#define TSB_IMN_FLG3_INT100FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,4)))
#define TSB_IMN_FLG3_INT101FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,5)))
#define TSB_IMN_FLG3_INT102FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,6)))
#define TSB_IMN_FLG3_INT103FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,7)))
#define TSB_IMN_FLG3_INT104FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,8)))
#define TSB_IMN_FLG3_INT105FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,9)))
#define TSB_IMN_FLG3_INT106FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,10)))
#define TSB_IMN_FLG3_INT107FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,11)))
#define TSB_IMN_FLG3_INT108FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,12)))
#define TSB_IMN_FLG3_INT109FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,13)))
#define TSB_IMN_FLG3_INT110FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,14)))
#define TSB_IMN_FLG3_INT111FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,15)))
#define TSB_IMN_FLG3_INT112FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,16)))
#define TSB_IMN_FLG3_INT113FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,17)))
#define TSB_IMN_FLG3_INT114FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,18)))
#define TSB_IMN_FLG3_INT115FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,19)))
#define TSB_IMN_FLG3_INT116FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,20)))
#define TSB_IMN_FLG3_INT117FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,21)))
#define TSB_IMN_FLG3_INT118FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,22)))
#define TSB_IMN_FLG3_INT119FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,23)))
#define TSB_IMN_FLG3_INT120FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,24)))
#define TSB_IMN_FLG3_INT121FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,25)))
#define TSB_IMN_FLG3_INT122FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,26)))
#define TSB_IMN_FLG3_INT123FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,27)))
#define TSB_IMN_FLG3_INT124FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,28)))
#define TSB_IMN_FLG3_INT125FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,29)))
#define TSB_IMN_FLG3_INT126FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,30)))
#define TSB_IMN_FLG3_INT127FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG3,31)))
#define TSB_IMN_FLG4_INT128FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,0)))
#define TSB_IMN_FLG4_INT129FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,1)))
#define TSB_IMN_FLG4_INT130FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,2)))
#define TSB_IMN_FLG4_INT131FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,3)))
#define TSB_IMN_FLG4_INT132FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,4)))
#define TSB_IMN_FLG4_INT133FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,5)))
#define TSB_IMN_FLG4_INT134FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,6)))
#define TSB_IMN_FLG4_INT135FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,7)))
#define TSB_IMN_FLG4_INT136FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,8)))
#define TSB_IMN_FLG4_INT137FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,9)))
#define TSB_IMN_FLG4_INT138FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,10)))
#define TSB_IMN_FLG4_INT139FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,11)))
#define TSB_IMN_FLG4_INT140FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,12)))
#define TSB_IMN_FLG4_INT141FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,13)))
#define TSB_IMN_FLG4_INT142FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,14)))
#define TSB_IMN_FLG4_INT143FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,15)))
#define TSB_IMN_FLG4_INT144FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,16)))
#define TSB_IMN_FLG4_INT145FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,17)))
#define TSB_IMN_FLG4_INT146FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,18)))
#define TSB_IMN_FLG4_INT147FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,19)))
#define TSB_IMN_FLG4_INT148FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,20)))
#define TSB_IMN_FLG4_INT149FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,21)))
#define TSB_IMN_FLG4_INT150FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,22)))
#define TSB_IMN_FLG4_INT151FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,23)))
#define TSB_IMN_FLG4_INT152FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,24)))
#define TSB_IMN_FLG4_INT153FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,25)))
#define TSB_IMN_FLG4_INT154FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,26)))
#define TSB_IMN_FLG4_INT155FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,27)))
#define TSB_IMN_FLG4_INT156FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,28)))
#define TSB_IMN_FLG4_INT157FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,29)))
#define TSB_IMN_FLG4_INT158FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,30)))
#define TSB_IMN_FLG4_INT159FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG4,31)))
#define TSB_IMN_FLG5_INT160FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,0)))
#define TSB_IMN_FLG5_INT161FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,1)))
#define TSB_IMN_FLG5_INT162FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,2)))
#define TSB_IMN_FLG5_INT163FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,3)))
#define TSB_IMN_FLG5_INT164FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,4)))
#define TSB_IMN_FLG5_INT165FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,5)))
#define TSB_IMN_FLG5_INT166FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,6)))
#define TSB_IMN_FLG5_INT167FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,7)))
#define TSB_IMN_FLG5_INT168FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,8)))
#define TSB_IMN_FLG5_INT169FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,9)))
#define TSB_IMN_FLG5_INT170FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,10)))
#define TSB_IMN_FLG5_INT171FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,11)))
#define TSB_IMN_FLG5_INT172FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,12)))
#define TSB_IMN_FLG5_INT173FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,13)))
#define TSB_IMN_FLG5_INT174FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,14)))
#define TSB_IMN_FLG5_INT175FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,15)))
#define TSB_IMN_FLG5_INT176FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,16)))
#define TSB_IMN_FLG5_INT177FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,17)))
#define TSB_IMN_FLG5_INT178FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,18)))
#define TSB_IMN_FLG5_INT179FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,19)))
#define TSB_IMN_FLG5_INT180FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,20)))
#define TSB_IMN_FLG5_INT181FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,21)))
#define TSB_IMN_FLG5_INT182FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,22)))
#define TSB_IMN_FLG5_INT183FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,23)))
#define TSB_IMN_FLG5_INT184FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,24)))
#define TSB_IMN_FLG5_INT185FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,25)))
#define TSB_IMN_FLG5_INT186FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,26)))
#define TSB_IMN_FLG5_INT187FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,27)))
#define TSB_IMN_FLG5_INT188FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,28)))
#define TSB_IMN_FLG5_INT189FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,29)))
#define TSB_IMN_FLG5_INT190FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,30)))
#define TSB_IMN_FLG5_INT191FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG5,31)))
#define TSB_IMN_FLG6_INT192FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,0)))
#define TSB_IMN_FLG6_INT193FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,1)))
#define TSB_IMN_FLG6_INT194FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,2)))
#define TSB_IMN_FLG6_INT195FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,3)))
#define TSB_IMN_FLG6_INT196FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,4)))
#define TSB_IMN_FLG6_INT197FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,5)))
#define TSB_IMN_FLG6_INT198FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,6)))
#define TSB_IMN_FLG6_INT199FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,7)))
#define TSB_IMN_FLG6_INT200FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,8)))
#define TSB_IMN_FLG6_INT201FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,9)))
#define TSB_IMN_FLG6_INT202FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,10)))
#define TSB_IMN_FLG6_INT203FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,11)))
#define TSB_IMN_FLG6_INT204FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,12)))
#define TSB_IMN_FLG6_INT205FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,13)))
#define TSB_IMN_FLG6_INT206FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,14)))
#define TSB_IMN_FLG6_INT207FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,15)))
#define TSB_IMN_FLG6_INT208FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,16)))
#define TSB_IMN_FLG6_INT209FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,17)))
#define TSB_IMN_FLG6_INT210FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,18)))
#define TSB_IMN_FLG6_INT211FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,19)))
#define TSB_IMN_FLG6_INT212FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,20)))
#define TSB_IMN_FLG6_INT213FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,21)))
#define TSB_IMN_FLG6_INT214FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,22)))
#define TSB_IMN_FLG6_INT215FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,23)))
#define TSB_IMN_FLG6_INT216FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,24)))
#define TSB_IMN_FLG6_INT217FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,25)))
#define TSB_IMN_FLG6_INT218FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,26)))
#define TSB_IMN_FLG6_INT219FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,27)))
#define TSB_IMN_FLG6_INT220FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,28)))
#define TSB_IMN_FLG6_INT221FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,29)))
#define TSB_IMN_FLG6_INT222FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,30)))
#define TSB_IMN_FLG6_INT223FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG6,31)))
#define TSB_IMN_FLG7_INT224FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG7,0)))
#define TSB_IMN_FLG7_INT225FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG7,1)))
#define TSB_IMN_FLG7_INT226FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG7,2)))
#define TSB_IMN_FLG7_INT227FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG7,3)))
#define TSB_IMN_FLG7_INT228FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG7,4)))
#define TSB_IMN_FLG7_INT229FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG7,5)))
#define TSB_IMN_FLG7_INT230FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG7,6)))
#define TSB_IMN_FLG7_INT231FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG7,7)))
#define TSB_IMN_FLG7_INT232FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG7,8)))
#define TSB_IMN_FLG7_INT233FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG7,9)))
#define TSB_IMN_FLG7_INT234FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG7,10)))
#define TSB_IMN_FLG7_INT235FLG                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_IMN->FLG7,11)))


/* DNF */
#define TSB_DNFA_ENCR_NFEN0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,0)))
#define TSB_DNFA_ENCR_NFEN1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,1)))
#define TSB_DNFA_ENCR_NFEN2                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,2)))
#define TSB_DNFA_ENCR_NFEN3                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,3)))
#define TSB_DNFA_ENCR_NFEN4                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,4)))
#define TSB_DNFA_ENCR_NFEN5                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,5)))
#define TSB_DNFA_ENCR_NFEN6                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,6)))
#define TSB_DNFA_ENCR_NFEN7                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,7)))
#define TSB_DNFA_ENCR_NFEN8                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,8)))
#define TSB_DNFA_ENCR_NFEN9                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,9)))
#define TSB_DNFA_ENCR_NFEN10                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,10)))
#define TSB_DNFA_ENCR_NFEN11                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,11)))
#define TSB_DNFA_ENCR_NFEN12                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,12)))
#define TSB_DNFA_ENCR_NFEN13                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,13)))
#define TSB_DNFA_ENCR_NFEN14                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,14)))
#define TSB_DNFA_ENCR_NFEN15                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFA->ENCR,15)))

#define TSB_DNFB_ENCR_NFEN0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFB->ENCR,0)))
#define TSB_DNFB_ENCR_NFEN1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFB->ENCR,1)))
#define TSB_DNFB_ENCR_NFEN2                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFB->ENCR,2)))
#define TSB_DNFB_ENCR_NFEN3                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFB->ENCR,3)))
#define TSB_DNFB_ENCR_NFEN4                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFB->ENCR,4)))
#define TSB_DNFB_ENCR_NFEN5                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFB->ENCR,5)))
#define TSB_DNFB_ENCR_NFEN6                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFB->ENCR,6)))
#define TSB_DNFB_ENCR_NFEN7                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFB->ENCR,7)))
#define TSB_DNFB_ENCR_NFEN8                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFB->ENCR,8)))
#define TSB_DNFB_ENCR_NFEN9                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFB->ENCR,9)))
#define TSB_DNFB_ENCR_NFEN10                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFB->ENCR,10)))
#define TSB_DNFB_ENCR_NFEN11                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFB->ENCR,11)))
#define TSB_DNFB_ENCR_NFEN12                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFB->ENCR,12)))
#define TSB_DNFB_ENCR_NFEN13                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFB->ENCR,13)))
#define TSB_DNFB_ENCR_NFEN14                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFB->ENCR,14)))
#define TSB_DNFB_ENCR_NFEN15                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_DNFB->ENCR,15)))


/* Watchdog Timer (WD) */
#define TSB_SIWD0_EN_WDTE                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_SIWD0->EN,0)))
#define TSB_SIWD0_EN_WDTF                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_SIWD0->EN,1)))
#define TSB_SIWD0_MOD_RESCR                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_SIWD0->MOD,0)))
#define TSB_SIWD0_MOD_INTF                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_SIWD0->MOD,1)))


/* NBD */
#define TSB_NBD_CR0_NBDEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_NBD->CR0,0)))


/* Malti Porpose Direct Memory Accsess(MDMA) */
#define TSB_MDMAA_XFTYP_UMODE                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->XFTYP,16)))
#define TSB_MDMAA_XFTYP_DMODE                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->XFTYP,24)))
#define TSB_MDMAA_DSNUM_DSINF                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->DSNUM,8)))
#define TSB_MDMAA_C00XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C00XFTYP,16)))
#define TSB_MDMAA_C00XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C00XFTYP,24)))
#define TSB_MDMAA_C00DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C00DSNUM,8)))
#define TSB_MDMAA_C01XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C01XFTYP,16)))
#define TSB_MDMAA_C01XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C01XFTYP,24)))
#define TSB_MDMAA_C01DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C01DSNUM,8)))
#define TSB_MDMAA_C02XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C02XFTYP,16)))
#define TSB_MDMAA_C02XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C02XFTYP,24)))
#define TSB_MDMAA_C02DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C02DSNUM,8)))
#define TSB_MDMAA_C03XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C03XFTYP,16)))
#define TSB_MDMAA_C03XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C03XFTYP,24)))
#define TSB_MDMAA_C03DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C03DSNUM,8)))
#define TSB_MDMAA_C04XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C04XFTYP,16)))
#define TSB_MDMAA_C04XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C04XFTYP,24)))
#define TSB_MDMAA_C04DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C04DSNUM,8)))
#define TSB_MDMAA_C05XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C05XFTYP,16)))
#define TSB_MDMAA_C05XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C05XFTYP,24)))
#define TSB_MDMAA_C05DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C05DSNUM,8)))
#define TSB_MDMAA_C06XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C06XFTYP,16)))
#define TSB_MDMAA_C06XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C06XFTYP,24)))
#define TSB_MDMAA_C06DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C06DSNUM,8)))
#define TSB_MDMAA_C07XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C07XFTYP,16)))
#define TSB_MDMAA_C07XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C07XFTYP,24)))
#define TSB_MDMAA_C07DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C07DSNUM,8)))
#define TSB_MDMAA_C08XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C08XFTYP,16)))
#define TSB_MDMAA_C08XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C08XFTYP,24)))
#define TSB_MDMAA_C08DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C08DSNUM,8)))
#define TSB_MDMAA_C09XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C09XFTYP,16)))
#define TSB_MDMAA_C09XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C09XFTYP,24)))
#define TSB_MDMAA_C09DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C09DSNUM,8)))
#define TSB_MDMAA_C10XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C10XFTYP,16)))
#define TSB_MDMAA_C10XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C10XFTYP,24)))
#define TSB_MDMAA_C10DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C10DSNUM,8)))
#define TSB_MDMAA_C11XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C11XFTYP,16)))
#define TSB_MDMAA_C11XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C11XFTYP,24)))
#define TSB_MDMAA_C11DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C11DSNUM,8)))
#define TSB_MDMAA_C12XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C12XFTYP,16)))
#define TSB_MDMAA_C12XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C12XFTYP,24)))
#define TSB_MDMAA_C12DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C12DSNUM,8)))
#define TSB_MDMAA_C13XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C13XFTYP,16)))
#define TSB_MDMAA_C13XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C13XFTYP,24)))
#define TSB_MDMAA_C13DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C13DSNUM,8)))
#define TSB_MDMAA_C14XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C14XFTYP,16)))
#define TSB_MDMAA_C14XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C14XFTYP,24)))
#define TSB_MDMAA_C14DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C14DSNUM,8)))
#define TSB_MDMAA_C15XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C15XFTYP,16)))
#define TSB_MDMAA_C15XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C15XFTYP,24)))
#define TSB_MDMAA_C15DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C15DSNUM,8)))
#define TSB_MDMAA_C16XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C16XFTYP,16)))
#define TSB_MDMAA_C16XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C16XFTYP,24)))
#define TSB_MDMAA_C16DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C16DSNUM,8)))
#define TSB_MDMAA_C17XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C17XFTYP,16)))
#define TSB_MDMAA_C17XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C17XFTYP,24)))
#define TSB_MDMAA_C17DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C17DSNUM,8)))
#define TSB_MDMAA_C18XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C18XFTYP,16)))
#define TSB_MDMAA_C18XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C18XFTYP,24)))
#define TSB_MDMAA_C18DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C18DSNUM,8)))
#define TSB_MDMAA_C19XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C19XFTYP,16)))
#define TSB_MDMAA_C19XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C19XFTYP,24)))
#define TSB_MDMAA_C19DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C19DSNUM,8)))
#define TSB_MDMAA_C20XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C20XFTYP,16)))
#define TSB_MDMAA_C20XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C20XFTYP,24)))
#define TSB_MDMAA_C20DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C20DSNUM,8)))
#define TSB_MDMAA_C21XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C21XFTYP,16)))
#define TSB_MDMAA_C21XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C21XFTYP,24)))
#define TSB_MDMAA_C21DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C21DSNUM,8)))
#define TSB_MDMAA_C22XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C22XFTYP,16)))
#define TSB_MDMAA_C22XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C22XFTYP,24)))
#define TSB_MDMAA_C22DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C22DSNUM,8)))
#define TSB_MDMAA_C23XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C23XFTYP,16)))
#define TSB_MDMAA_C23XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C23XFTYP,24)))
#define TSB_MDMAA_C23DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C23DSNUM,8)))
#define TSB_MDMAA_C24XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C24XFTYP,16)))
#define TSB_MDMAA_C24XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C24XFTYP,24)))
#define TSB_MDMAA_C24DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C24DSNUM,8)))
#define TSB_MDMAA_C25XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C25XFTYP,16)))
#define TSB_MDMAA_C25XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C25XFTYP,24)))
#define TSB_MDMAA_C25DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C25DSNUM,8)))
#define TSB_MDMAA_C26XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C26XFTYP,16)))
#define TSB_MDMAA_C26XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C26XFTYP,24)))
#define TSB_MDMAA_C26DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C26DSNUM,8)))
#define TSB_MDMAA_C27XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C27XFTYP,16)))
#define TSB_MDMAA_C27XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C27XFTYP,24)))
#define TSB_MDMAA_C27DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C27DSNUM,8)))
#define TSB_MDMAA_C28XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C28XFTYP,16)))
#define TSB_MDMAA_C28XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C28XFTYP,24)))
#define TSB_MDMAA_C28DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C28DSNUM,8)))
#define TSB_MDMAA_C29XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C29XFTYP,16)))
#define TSB_MDMAA_C29XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C29XFTYP,24)))
#define TSB_MDMAA_C29DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C29DSNUM,8)))
#define TSB_MDMAA_C30XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C30XFTYP,16)))
#define TSB_MDMAA_C30XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C30XFTYP,24)))
#define TSB_MDMAA_C30DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C30DSNUM,8)))
#define TSB_MDMAA_C31XFTYP_UMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C31XFTYP,16)))
#define TSB_MDMAA_C31XFTYP_DMODE                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C31XFTYP,24)))
#define TSB_MDMAA_C31DSNUM_DSINF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_MDMAA->C31DSNUM,8)))
#define TSB_MDMAA_MSK_MSK0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,0)))
#define TSB_MDMAA_MSK_MSK1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,1)))
#define TSB_MDMAA_MSK_MSK2                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,2)))
#define TSB_MDMAA_MSK_MSK3                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,3)))
#define TSB_MDMAA_MSK_MSK4                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,4)))
#define TSB_MDMAA_MSK_MSK5                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,5)))
#define TSB_MDMAA_MSK_MSK6                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,6)))
#define TSB_MDMAA_MSK_MSK7                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,7)))
#define TSB_MDMAA_MSK_MSK8                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,8)))
#define TSB_MDMAA_MSK_MSK9                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,9)))
#define TSB_MDMAA_MSK_MSK10                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,10)))
#define TSB_MDMAA_MSK_MSK11                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,11)))
#define TSB_MDMAA_MSK_MSK12                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,12)))
#define TSB_MDMAA_MSK_MSK13                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,13)))
#define TSB_MDMAA_MSK_MSK14                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,14)))
#define TSB_MDMAA_MSK_MSK15                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,15)))
#define TSB_MDMAA_MSK_MSK16                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,16)))
#define TSB_MDMAA_MSK_MSK17                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,17)))
#define TSB_MDMAA_MSK_MSK18                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,18)))
#define TSB_MDMAA_MSK_MSK19                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,19)))
#define TSB_MDMAA_MSK_MSK20                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,20)))
#define TSB_MDMAA_MSK_MSK21                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,21)))
#define TSB_MDMAA_MSK_MSK22                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,22)))
#define TSB_MDMAA_MSK_MSK23                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,23)))
#define TSB_MDMAA_MSK_MSK24                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,24)))
#define TSB_MDMAA_MSK_MSK25                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,25)))
#define TSB_MDMAA_MSK_MSK26                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,26)))
#define TSB_MDMAA_MSK_MSK27                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,27)))
#define TSB_MDMAA_MSK_MSK28                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,28)))
#define TSB_MDMAA_MSK_MSK29                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,29)))
#define TSB_MDMAA_MSK_MSK30                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,30)))
#define TSB_MDMAA_MSK_MSK31                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_MDMAA->MSK,31)))


/* ARM Prime Cell PL011 */
#define TSB_FURT0_DR_FE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->DR,8)))
#define TSB_FURT0_DR_PE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->DR,9)))
#define TSB_FURT0_DR_BE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->DR,10)))
#define TSB_FURT0_DR_OE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->DR,11)))
#define TSB_FURT0_RSR_FE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->RSR,0)))
#define TSB_FURT0_RSR_PE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->RSR,1)))
#define TSB_FURT0_RSR_BE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->RSR,2)))
#define TSB_FURT0_RSR_OE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->RSR,3)))
#define TSB_FURT0_FR_CTS                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->FR,0)))
#define TSB_FURT0_FR_BUSY                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->FR,3)))
#define TSB_FURT0_FR_RXFE                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->FR,4)))
#define TSB_FURT0_FR_TXFF                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->FR,5)))
#define TSB_FURT0_FR_RXFF                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->FR,6)))
#define TSB_FURT0_FR_TXFE                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->FR,7)))
#define TSB_FURT0_LCR_H_BRK                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->LCR_H,0)))
#define TSB_FURT0_LCR_H_PEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->LCR_H,1)))
#define TSB_FURT0_LCR_H_EPS                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->LCR_H,2)))
#define TSB_FURT0_LCR_H_STP2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->LCR_H,3)))
#define TSB_FURT0_LCR_H_FEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->LCR_H,4)))
#define TSB_FURT0_LCR_H_SPS                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->LCR_H,7)))
#define TSB_FURT0_CR_UARTEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->CR,0)))
#define TSB_FURT0_CR_SIREN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->CR,1)))
#define TSB_FURT0_CR_SIRLP                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->CR,2)))
#define TSB_FURT0_CR_TXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->CR,8)))
#define TSB_FURT0_CR_RXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->CR,9)))
#define TSB_FURT0_CR_RTSEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->CR,14)))
#define TSB_FURT0_CR_CTSEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->CR,15)))
#define TSB_FURT0_IMSC_RXIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->IMSC,4)))
#define TSB_FURT0_IMSC_TXIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->IMSC,5)))
#define TSB_FURT0_IMSC_RTIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->IMSC,6)))
#define TSB_FURT0_IMSC_FEIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->IMSC,7)))
#define TSB_FURT0_IMSC_PEIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->IMSC,8)))
#define TSB_FURT0_IMSC_BEIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->IMSC,9)))
#define TSB_FURT0_IMSC_OEIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->IMSC,10)))
#define TSB_FURT0_RIS_RXRIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->RIS,4)))
#define TSB_FURT0_RIS_TXRIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->RIS,5)))
#define TSB_FURT0_RIS_RTRIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->RIS,6)))
#define TSB_FURT0_RIS_FERIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->RIS,7)))
#define TSB_FURT0_RIS_PERIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->RIS,8)))
#define TSB_FURT0_RIS_BERIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->RIS,9)))
#define TSB_FURT0_RIS_OERIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->RIS,10)))
#define TSB_FURT0_MIS_RXMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->MIS,4)))
#define TSB_FURT0_MIS_TXMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->MIS,5)))
#define TSB_FURT0_MIS_RTMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->MIS,6)))
#define TSB_FURT0_MIS_FEMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->MIS,7)))
#define TSB_FURT0_MIS_PEMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->MIS,8)))
#define TSB_FURT0_MIS_BEMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->MIS,9)))
#define TSB_FURT0_MIS_OEMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT0->MIS,10)))
#define TSB_FURT0_ICR_RXIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_FURT0->ICR,4)))
#define TSB_FURT0_ICR_TXIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_FURT0->ICR,5)))
#define TSB_FURT0_ICR_RTIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_FURT0->ICR,6)))
#define TSB_FURT0_ICR_FEIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_FURT0->ICR,7)))
#define TSB_FURT0_ICR_PEIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_FURT0->ICR,8)))
#define TSB_FURT0_ICR_BEIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_FURT0->ICR,9)))
#define TSB_FURT0_ICR_OEIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_FURT0->ICR,10)))
#define TSB_FURT0_DMACR_RXDMAE                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->DMACR,0)))
#define TSB_FURT0_DMACR_TXDMAE                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->DMACR,1)))
#define TSB_FURT0_DMACR_DMAONERR                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT0->DMACR,2)))

#define TSB_FURT1_DR_FE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->DR,8)))
#define TSB_FURT1_DR_PE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->DR,9)))
#define TSB_FURT1_DR_BE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->DR,10)))
#define TSB_FURT1_DR_OE                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->DR,11)))
#define TSB_FURT1_RSR_FE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->RSR,0)))
#define TSB_FURT1_RSR_PE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->RSR,1)))
#define TSB_FURT1_RSR_BE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->RSR,2)))
#define TSB_FURT1_RSR_OE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->RSR,3)))
#define TSB_FURT1_FR_CTS                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->FR,0)))
#define TSB_FURT1_FR_BUSY                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->FR,3)))
#define TSB_FURT1_FR_RXFE                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->FR,4)))
#define TSB_FURT1_FR_TXFF                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->FR,5)))
#define TSB_FURT1_FR_RXFF                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->FR,6)))
#define TSB_FURT1_FR_TXFE                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->FR,7)))
#define TSB_FURT1_LCR_H_BRK                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->LCR_H,0)))
#define TSB_FURT1_LCR_H_PEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->LCR_H,1)))
#define TSB_FURT1_LCR_H_EPS                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->LCR_H,2)))
#define TSB_FURT1_LCR_H_STP2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->LCR_H,3)))
#define TSB_FURT1_LCR_H_FEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->LCR_H,4)))
#define TSB_FURT1_LCR_H_SPS                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->LCR_H,7)))
#define TSB_FURT1_CR_UARTEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->CR,0)))
#define TSB_FURT1_CR_SIREN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->CR,1)))
#define TSB_FURT1_CR_SIRLP                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->CR,2)))
#define TSB_FURT1_CR_TXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->CR,8)))
#define TSB_FURT1_CR_RXE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->CR,9)))
#define TSB_FURT1_CR_RTSEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->CR,14)))
#define TSB_FURT1_CR_CTSEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->CR,15)))
#define TSB_FURT1_IMSC_RXIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->IMSC,4)))
#define TSB_FURT1_IMSC_TXIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->IMSC,5)))
#define TSB_FURT1_IMSC_RTIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->IMSC,6)))
#define TSB_FURT1_IMSC_FEIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->IMSC,7)))
#define TSB_FURT1_IMSC_PEIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->IMSC,8)))
#define TSB_FURT1_IMSC_BEIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->IMSC,9)))
#define TSB_FURT1_IMSC_OEIM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->IMSC,10)))
#define TSB_FURT1_RIS_RXRIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->RIS,4)))
#define TSB_FURT1_RIS_TXRIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->RIS,5)))
#define TSB_FURT1_RIS_RTRIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->RIS,6)))
#define TSB_FURT1_RIS_FERIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->RIS,7)))
#define TSB_FURT1_RIS_PERIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->RIS,8)))
#define TSB_FURT1_RIS_BERIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->RIS,9)))
#define TSB_FURT1_RIS_OERIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->RIS,10)))
#define TSB_FURT1_MIS_RXMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->MIS,4)))
#define TSB_FURT1_MIS_TXMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->MIS,5)))
#define TSB_FURT1_MIS_RTMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->MIS,6)))
#define TSB_FURT1_MIS_FEMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->MIS,7)))
#define TSB_FURT1_MIS_PEMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->MIS,8)))
#define TSB_FURT1_MIS_BEMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->MIS,9)))
#define TSB_FURT1_MIS_OEMIS                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_FURT1->MIS,10)))
#define TSB_FURT1_ICR_RXIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_FURT1->ICR,4)))
#define TSB_FURT1_ICR_TXIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_FURT1->ICR,5)))
#define TSB_FURT1_ICR_RTIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_FURT1->ICR,6)))
#define TSB_FURT1_ICR_FEIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_FURT1->ICR,7)))
#define TSB_FURT1_ICR_PEIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_FURT1->ICR,8)))
#define TSB_FURT1_ICR_BEIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_FURT1->ICR,9)))
#define TSB_FURT1_ICR_OEIC                        (*((__O  uint32_t *)BITBAND_PERI(&TSB_FURT1->ICR,10)))
#define TSB_FURT1_DMACR_RXDMAE                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->DMACR,0)))
#define TSB_FURT1_DMACR_TXDMAE                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->DMACR,1)))
#define TSB_FURT1_DMACR_DMAONERR                  (*((__IO uint32_t *)BITBAND_PERI(&TSB_FURT1->DMACR,2)))


/* ADC */
#define TSB_ADA_CR0_CNT                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CR0,0)))
#define TSB_ADA_CR0_SGL                           (*((__O  uint32_t *)BITBAND_PERI(&TSB_ADA->CR0,1)))
#define TSB_ADA_CR0_HPSGL                         (*((__O  uint32_t *)BITBAND_PERI(&TSB_ADA->CR0,2)))
#define TSB_ADA_CR0_ADEN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CR0,7)))
#define TSB_ADA_CR1_TRGEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CR1,0)))
#define TSB_ADA_CR1_HPTRGEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CR1,1)))
#define TSB_ADA_CR1_TRGDMEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CR1,4)))
#define TSB_ADA_CR1_SGLDMEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CR1,5)))
#define TSB_ADA_CR1_CNTDMEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CR1,6)))
#define TSB_ADA_CR1_HPDMEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CR1,7)))
#define TSB_ADA_ST_HPF                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->ST,0)))
#define TSB_ADA_ST_TRGF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->ST,1)))
#define TSB_ADA_ST_SNGF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->ST,2)))
#define TSB_ADA_ST_CNTF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->ST,3)))
#define TSB_ADA_ST_ADBF                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->ST,7)))
#define TSB_ADA_MOD0_DACON                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->MOD0,0)))
#define TSB_ADA_MOD0_RCUT                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->MOD0,1)))
#define TSB_ADA_CMPEN_CMP0EN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CMPEN,0)))
#define TSB_ADA_CMPEN_CMP1EN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CMPEN,1)))
#define TSB_ADA_CMPEN_CMP2EN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CMPEN,2)))
#define TSB_ADA_CMPEN_CMP3EN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CMPEN,3)))
#define TSB_ADA_CMPCR0_ADBIG0                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CMPCR0,5)))
#define TSB_ADA_CMPCR0_CMPCND0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CMPCR0,6)))
#define TSB_ADA_CMPCR1_ADBIG1                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CMPCR1,5)))
#define TSB_ADA_CMPCR1_CMPCND1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CMPCR1,6)))
#define TSB_ADA_CMPCR2_ADBIG2                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CMPCR2,5)))
#define TSB_ADA_CMPCR2_CMPCND2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CMPCR2,6)))
#define TSB_ADA_CMPCR3_ADBIG3                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CMPCR3,5)))
#define TSB_ADA_CMPCR3_CMPCND3                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->CMPCR3,6)))
#define TSB_ADA_TSET0_ENINT0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET0,7)))
#define TSB_ADA_TSET1_ENINT1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET1,7)))
#define TSB_ADA_TSET2_ENINT2                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET2,7)))
#define TSB_ADA_TSET3_ENINT3                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET3,7)))
#define TSB_ADA_TSET4_ENINT4                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET4,7)))
#define TSB_ADA_TSET5_ENINT5                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET5,7)))
#define TSB_ADA_TSET6_ENINT6                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET6,7)))
#define TSB_ADA_TSET7_ENINT7                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET7,7)))
#define TSB_ADA_TSET8_ENINT8                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET8,7)))
#define TSB_ADA_TSET9_ENINT9                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET9,7)))
#define TSB_ADA_TSET10_ENINT10                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET10,7)))
#define TSB_ADA_TSET11_ENINT11                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET11,7)))
#define TSB_ADA_TSET12_ENINT12                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET12,7)))
#define TSB_ADA_TSET13_ENINT13                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET13,7)))
#define TSB_ADA_TSET14_ENINT14                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET14,7)))
#define TSB_ADA_TSET15_ENINT15                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET15,7)))
#define TSB_ADA_TSET16_ENINT16                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET16,7)))
#define TSB_ADA_TSET17_ENINT17                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET17,7)))
#define TSB_ADA_TSET18_ENINT18                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET18,7)))
#define TSB_ADA_TSET19_ENINT19                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET19,7)))
#define TSB_ADA_TSET20_ENINT20                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET20,7)))
#define TSB_ADA_TSET21_ENINT21                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET21,7)))
#define TSB_ADA_TSET22_ENINT22                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET22,7)))
#define TSB_ADA_TSET23_ENINT23                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_ADA->TSET23,7)))
#define TSB_ADA_REG0_ADRF0                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG0,0)))
#define TSB_ADA_REG0_ADOVRF0                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG0,1)))
#define TSB_ADA_REG0_ADRF_M0                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG0,28)))
#define TSB_ADA_REG0_ADOVR_M0                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG0,29)))
#define TSB_ADA_REG1_ADRF1                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG1,0)))
#define TSB_ADA_REG1_ADOVRF1                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG1,1)))
#define TSB_ADA_REG1_ADRF_M1                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG1,28)))
#define TSB_ADA_REG1_ADOVR_M1                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG1,29)))
#define TSB_ADA_REG2_ADRF2                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG2,0)))
#define TSB_ADA_REG2_ADOVRF2                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG2,1)))
#define TSB_ADA_REG2_ADRF_M2                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG2,28)))
#define TSB_ADA_REG2_ADOVR_M2                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG2,29)))
#define TSB_ADA_REG3_ADRF3                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG3,0)))
#define TSB_ADA_REG3_ADOVRF3                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG3,1)))
#define TSB_ADA_REG3_ADRF_M3                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG3,28)))
#define TSB_ADA_REG3_ADOVR_M3                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG3,29)))
#define TSB_ADA_REG4_ADRF4                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG4,0)))
#define TSB_ADA_REG4_ADOVRF4                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG4,1)))
#define TSB_ADA_REG4_ADRF_M4                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG4,28)))
#define TSB_ADA_REG4_ADOVR_M4                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG4,29)))
#define TSB_ADA_REG5_ADRF5                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG5,0)))
#define TSB_ADA_REG5_ADOVRF5                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG5,1)))
#define TSB_ADA_REG5_ADRF_M5                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG5,28)))
#define TSB_ADA_REG5_ADOVR_M5                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG5,29)))
#define TSB_ADA_REG6_ADRF6                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG6,0)))
#define TSB_ADA_REG6_ADOVRF6                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG6,1)))
#define TSB_ADA_REG6_ADRF_M6                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG6,28)))
#define TSB_ADA_REG6_ADOVR_M6                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG6,29)))
#define TSB_ADA_REG7_ADRF7                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG7,0)))
#define TSB_ADA_REG7_ADOVRF7                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG7,1)))
#define TSB_ADA_REG7_ADRF_M7                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG7,28)))
#define TSB_ADA_REG7_ADOVR_M7                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG7,29)))
#define TSB_ADA_REG8_ADRF8                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG8,0)))
#define TSB_ADA_REG8_ADOVRF8                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG8,1)))
#define TSB_ADA_REG8_ADRF_M8                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG8,28)))
#define TSB_ADA_REG8_ADOVR_M8                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG8,29)))
#define TSB_ADA_REG9_ADRF9                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG9,0)))
#define TSB_ADA_REG9_ADOVRF9                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG9,1)))
#define TSB_ADA_REG9_ADRF_M9                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG9,28)))
#define TSB_ADA_REG9_ADOVR_M9                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG9,29)))
#define TSB_ADA_REG10_ADRF10                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG10,0)))
#define TSB_ADA_REG10_ADOVRF10                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG10,1)))
#define TSB_ADA_REG10_ADRF_M10                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG10,28)))
#define TSB_ADA_REG10_ADOVR_M10                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG10,29)))
#define TSB_ADA_REG11_ADRF11                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG11,0)))
#define TSB_ADA_REG11_ADOVRF11                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG11,1)))
#define TSB_ADA_REG11_ADRF_M11                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG11,28)))
#define TSB_ADA_REG11_ADOVR_M11                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG11,29)))
#define TSB_ADA_REG12_ADRF12                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG12,0)))
#define TSB_ADA_REG12_ADOVRF12                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG12,1)))
#define TSB_ADA_REG12_ADRF_M12                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG12,28)))
#define TSB_ADA_REG12_ADOVR_M12                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG12,29)))
#define TSB_ADA_REG13_ADRF13                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG13,0)))
#define TSB_ADA_REG13_ADOVRF13                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG13,1)))
#define TSB_ADA_REG13_ADRF_M13                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG13,28)))
#define TSB_ADA_REG13_ADOVR_M13                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG13,29)))
#define TSB_ADA_REG14_ADRF14                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG14,0)))
#define TSB_ADA_REG14_ADOVRF14                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG14,1)))
#define TSB_ADA_REG14_ADRF_M14                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG14,28)))
#define TSB_ADA_REG14_ADOVR_M14                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG14,29)))
#define TSB_ADA_REG15_ADRF15                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG15,0)))
#define TSB_ADA_REG15_ADOVRF15                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG15,1)))
#define TSB_ADA_REG15_ADRF_M15                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG15,28)))
#define TSB_ADA_REG15_ADOVR_M15                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG15,29)))
#define TSB_ADA_REG16_ADRF16                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG16,0)))
#define TSB_ADA_REG16_ADOVRF16                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG16,1)))
#define TSB_ADA_REG16_ADRF_M16                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG16,28)))
#define TSB_ADA_REG16_ADOVR_M16                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG16,29)))
#define TSB_ADA_REG17_ADRF17                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG17,0)))
#define TSB_ADA_REG17_ADOVRF17                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG17,1)))
#define TSB_ADA_REG17_ADRF_M17                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG17,28)))
#define TSB_ADA_REG17_ADOVR_M17                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG17,29)))
#define TSB_ADA_REG18_ADRF18                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG18,0)))
#define TSB_ADA_REG18_ADOVRF18                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG18,1)))
#define TSB_ADA_REG18_ADRF_M18                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG18,28)))
#define TSB_ADA_REG18_ADOVR_M18                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG18,29)))
#define TSB_ADA_REG19_ADRF19                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG19,0)))
#define TSB_ADA_REG19_ADOVRF19                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG19,1)))
#define TSB_ADA_REG19_ADRF_M19                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG19,28)))
#define TSB_ADA_REG19_ADOVR_M19                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG19,29)))
#define TSB_ADA_REG20_ADRF20                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG20,0)))
#define TSB_ADA_REG20_ADOVRF20                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG20,1)))
#define TSB_ADA_REG20_ADRF_M20                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG20,28)))
#define TSB_ADA_REG20_ADOVR_M20                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG20,29)))
#define TSB_ADA_REG21_ADRF21                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG21,0)))
#define TSB_ADA_REG21_ADOVRF21                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG21,1)))
#define TSB_ADA_REG21_ADRF_M21                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG21,28)))
#define TSB_ADA_REG21_ADOVR_M21                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG21,29)))
#define TSB_ADA_REG22_ADRF22                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG22,0)))
#define TSB_ADA_REG22_ADOVRF22                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG22,1)))
#define TSB_ADA_REG22_ADRF_M22                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG22,28)))
#define TSB_ADA_REG22_ADOVR_M22                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG22,29)))
#define TSB_ADA_REG23_ADRF23                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG23,0)))
#define TSB_ADA_REG23_ADOVRF23                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG23,1)))
#define TSB_ADA_REG23_ADRF_M23                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG23,28)))
#define TSB_ADA_REG23_ADOVR_M23                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_ADA->REG23,29)))


/* Digital analog converter (DAC) */
#define TSB_DA0_CR_EN                             (*((__IO uint32_t *)BITBAND_PERI(&TSB_DA0->CR,0)))

#define TSB_DA1_CR_EN                             (*((__IO uint32_t *)BITBAND_PERI(&TSB_DA1->CR,0)))


/* 16-bit Timer/Event Counter (TB) */
#define TSB_T32A0_MOD_MODE32                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->MOD,0)))
#define TSB_T32A0_MOD_HALT                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->MOD,1)))
#define TSB_T32A0_RUNA_RUNA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNA,0)))
#define TSB_T32A0_RUNA_SFTSTAA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNA,1)))
#define TSB_T32A0_RUNA_SFTSTPA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNA,2)))
#define TSB_T32A0_RUNA_RUNFLGA                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNA,4)))
#define TSB_T32A0_CRA_WBFA                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->CRA,20)))
#define TSB_T32A0_STA_INTA0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STA,0)))
#define TSB_T32A0_STA_INTA1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STA,1)))
#define TSB_T32A0_STA_INTOFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STA,2)))
#define TSB_T32A0_STA_INTUFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STA,3)))
#define TSB_T32A0_IMA_IMA0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMA,0)))
#define TSB_T32A0_IMA_IMA1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMA,1)))
#define TSB_T32A0_IMA_IMOFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMA,2)))
#define TSB_T32A0_IMA_IMUFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMA,3)))
#define TSB_T32A0_DMAA_DMAENA0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->DMAA,0)))
#define TSB_T32A0_DMAA_DMAENA1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->DMAA,1)))
#define TSB_T32A0_DMAA_DMAENA2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->DMAA,2)))
#define TSB_T32A0_RUNB_RUNB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNB,0)))
#define TSB_T32A0_RUNB_SFTSTAB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNB,1)))
#define TSB_T32A0_RUNB_SFTSTPB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNB,2)))
#define TSB_T32A0_RUNB_RUNFLGB                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNB,4)))
#define TSB_T32A0_CRB_WBFB                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->CRB,20)))
#define TSB_T32A0_STB_INTB0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STB,0)))
#define TSB_T32A0_STB_INTB1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STB,1)))
#define TSB_T32A0_STB_INTOFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STB,2)))
#define TSB_T32A0_STB_INTUFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STB,3)))
#define TSB_T32A0_IMB_IMB0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMB,0)))
#define TSB_T32A0_IMB_IMB1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMB,1)))
#define TSB_T32A0_IMB_IMOFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMB,2)))
#define TSB_T32A0_IMB_IMUFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMB,3)))
#define TSB_T32A0_DMAB_DMAENB0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->DMAB,0)))
#define TSB_T32A0_DMAB_DMAENB1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->DMAB,1)))
#define TSB_T32A0_DMAB_DMAENB2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->DMAB,2)))
#define TSB_T32A0_RUNC_RUNC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNC,0)))
#define TSB_T32A0_RUNC_SFTSTAC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNC,1)))
#define TSB_T32A0_RUNC_SFTSTPC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNC,2)))
#define TSB_T32A0_RUNC_RUNFLGC                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A0->RUNC,4)))
#define TSB_T32A0_CRC_WBFC                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->CRC,20)))
#define TSB_T32A0_STC_INTC0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STC,0)))
#define TSB_T32A0_STC_INTC1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STC,1)))
#define TSB_T32A0_STC_INTOFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STC,2)))
#define TSB_T32A0_STC_INTUFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STC,3)))
#define TSB_T32A0_STC_INTSTERR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->STC,4)))
#define TSB_T32A0_IMC_IMC0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMC,0)))
#define TSB_T32A0_IMC_IMC1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMC,1)))
#define TSB_T32A0_IMC_IMOFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMC,2)))
#define TSB_T32A0_IMC_IMUFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMC,3)))
#define TSB_T32A0_IMC_IMSTERR                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->IMC,4)))
#define TSB_T32A0_DMAC_DMAENC0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->DMAC,0)))
#define TSB_T32A0_DMAC_DMAENC1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->DMAC,1)))
#define TSB_T32A0_DMAC_DMAENC2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->DMAC,2)))
#define TSB_T32A0_PLSCR_PMODE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->PLSCR,0)))
#define TSB_T32A0_PLSCR_PDIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A0->PLSCR,1)))

#define TSB_T32A1_MOD_MODE32                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->MOD,0)))
#define TSB_T32A1_MOD_HALT                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->MOD,1)))
#define TSB_T32A1_RUNA_RUNA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNA,0)))
#define TSB_T32A1_RUNA_SFTSTAA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNA,1)))
#define TSB_T32A1_RUNA_SFTSTPA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNA,2)))
#define TSB_T32A1_RUNA_RUNFLGA                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNA,4)))
#define TSB_T32A1_CRA_WBFA                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->CRA,20)))
#define TSB_T32A1_STA_INTA0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STA,0)))
#define TSB_T32A1_STA_INTA1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STA,1)))
#define TSB_T32A1_STA_INTOFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STA,2)))
#define TSB_T32A1_STA_INTUFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STA,3)))
#define TSB_T32A1_IMA_IMA0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMA,0)))
#define TSB_T32A1_IMA_IMA1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMA,1)))
#define TSB_T32A1_IMA_IMOFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMA,2)))
#define TSB_T32A1_IMA_IMUFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMA,3)))
#define TSB_T32A1_DMAA_DMAENA0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->DMAA,0)))
#define TSB_T32A1_DMAA_DMAENA1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->DMAA,1)))
#define TSB_T32A1_DMAA_DMAENA2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->DMAA,2)))
#define TSB_T32A1_RUNB_RUNB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNB,0)))
#define TSB_T32A1_RUNB_SFTSTAB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNB,1)))
#define TSB_T32A1_RUNB_SFTSTPB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNB,2)))
#define TSB_T32A1_RUNB_RUNFLGB                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNB,4)))
#define TSB_T32A1_CRB_WBFB                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->CRB,20)))
#define TSB_T32A1_STB_INTB0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STB,0)))
#define TSB_T32A1_STB_INTB1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STB,1)))
#define TSB_T32A1_STB_INTOFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STB,2)))
#define TSB_T32A1_STB_INTUFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STB,3)))
#define TSB_T32A1_IMB_IMB0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMB,0)))
#define TSB_T32A1_IMB_IMB1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMB,1)))
#define TSB_T32A1_IMB_IMOFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMB,2)))
#define TSB_T32A1_IMB_IMUFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMB,3)))
#define TSB_T32A1_DMAB_DMAENB0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->DMAB,0)))
#define TSB_T32A1_DMAB_DMAENB1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->DMAB,1)))
#define TSB_T32A1_DMAB_DMAENB2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->DMAB,2)))
#define TSB_T32A1_RUNC_RUNC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNC,0)))
#define TSB_T32A1_RUNC_SFTSTAC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNC,1)))
#define TSB_T32A1_RUNC_SFTSTPC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNC,2)))
#define TSB_T32A1_RUNC_RUNFLGC                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A1->RUNC,4)))
#define TSB_T32A1_CRC_WBFC                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->CRC,20)))
#define TSB_T32A1_STC_INTC0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STC,0)))
#define TSB_T32A1_STC_INTC1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STC,1)))
#define TSB_T32A1_STC_INTOFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STC,2)))
#define TSB_T32A1_STC_INTUFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STC,3)))
#define TSB_T32A1_STC_INTSTERR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->STC,4)))
#define TSB_T32A1_IMC_IMC0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMC,0)))
#define TSB_T32A1_IMC_IMC1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMC,1)))
#define TSB_T32A1_IMC_IMOFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMC,2)))
#define TSB_T32A1_IMC_IMUFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMC,3)))
#define TSB_T32A1_IMC_IMSTERR                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->IMC,4)))
#define TSB_T32A1_DMAC_DMAENC0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->DMAC,0)))
#define TSB_T32A1_DMAC_DMAENC1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->DMAC,1)))
#define TSB_T32A1_DMAC_DMAENC2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->DMAC,2)))
#define TSB_T32A1_PLSCR_PMODE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->PLSCR,0)))
#define TSB_T32A1_PLSCR_PDIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A1->PLSCR,1)))

#define TSB_T32A2_MOD_MODE32                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->MOD,0)))
#define TSB_T32A2_MOD_HALT                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->MOD,1)))
#define TSB_T32A2_RUNA_RUNA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNA,0)))
#define TSB_T32A2_RUNA_SFTSTAA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNA,1)))
#define TSB_T32A2_RUNA_SFTSTPA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNA,2)))
#define TSB_T32A2_RUNA_RUNFLGA                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNA,4)))
#define TSB_T32A2_CRA_WBFA                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->CRA,20)))
#define TSB_T32A2_STA_INTA0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STA,0)))
#define TSB_T32A2_STA_INTA1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STA,1)))
#define TSB_T32A2_STA_INTOFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STA,2)))
#define TSB_T32A2_STA_INTUFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STA,3)))
#define TSB_T32A2_IMA_IMA0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMA,0)))
#define TSB_T32A2_IMA_IMA1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMA,1)))
#define TSB_T32A2_IMA_IMOFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMA,2)))
#define TSB_T32A2_IMA_IMUFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMA,3)))
#define TSB_T32A2_DMAA_DMAENA0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->DMAA,0)))
#define TSB_T32A2_DMAA_DMAENA1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->DMAA,1)))
#define TSB_T32A2_DMAA_DMAENA2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->DMAA,2)))
#define TSB_T32A2_RUNB_RUNB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNB,0)))
#define TSB_T32A2_RUNB_SFTSTAB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNB,1)))
#define TSB_T32A2_RUNB_SFTSTPB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNB,2)))
#define TSB_T32A2_RUNB_RUNFLGB                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNB,4)))
#define TSB_T32A2_CRB_WBFB                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->CRB,20)))
#define TSB_T32A2_STB_INTB0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STB,0)))
#define TSB_T32A2_STB_INTB1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STB,1)))
#define TSB_T32A2_STB_INTOFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STB,2)))
#define TSB_T32A2_STB_INTUFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STB,3)))
#define TSB_T32A2_IMB_IMB0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMB,0)))
#define TSB_T32A2_IMB_IMB1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMB,1)))
#define TSB_T32A2_IMB_IMOFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMB,2)))
#define TSB_T32A2_IMB_IMUFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMB,3)))
#define TSB_T32A2_DMAB_DMAENB0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->DMAB,0)))
#define TSB_T32A2_DMAB_DMAENB1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->DMAB,1)))
#define TSB_T32A2_DMAB_DMAENB2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->DMAB,2)))
#define TSB_T32A2_RUNC_RUNC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNC,0)))
#define TSB_T32A2_RUNC_SFTSTAC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNC,1)))
#define TSB_T32A2_RUNC_SFTSTPC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNC,2)))
#define TSB_T32A2_RUNC_RUNFLGC                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A2->RUNC,4)))
#define TSB_T32A2_CRC_WBFC                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->CRC,20)))
#define TSB_T32A2_STC_INTC0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STC,0)))
#define TSB_T32A2_STC_INTC1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STC,1)))
#define TSB_T32A2_STC_INTOFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STC,2)))
#define TSB_T32A2_STC_INTUFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STC,3)))
#define TSB_T32A2_STC_INTSTERR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->STC,4)))
#define TSB_T32A2_IMC_IMC0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMC,0)))
#define TSB_T32A2_IMC_IMC1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMC,1)))
#define TSB_T32A2_IMC_IMOFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMC,2)))
#define TSB_T32A2_IMC_IMUFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMC,3)))
#define TSB_T32A2_IMC_IMSTERR                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->IMC,4)))
#define TSB_T32A2_DMAC_DMAENC0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->DMAC,0)))
#define TSB_T32A2_DMAC_DMAENC1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->DMAC,1)))
#define TSB_T32A2_DMAC_DMAENC2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->DMAC,2)))
#define TSB_T32A2_PLSCR_PMODE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->PLSCR,0)))
#define TSB_T32A2_PLSCR_PDIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A2->PLSCR,1)))

#define TSB_T32A3_MOD_MODE32                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->MOD,0)))
#define TSB_T32A3_MOD_HALT                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->MOD,1)))
#define TSB_T32A3_RUNA_RUNA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNA,0)))
#define TSB_T32A3_RUNA_SFTSTAA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNA,1)))
#define TSB_T32A3_RUNA_SFTSTPA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNA,2)))
#define TSB_T32A3_RUNA_RUNFLGA                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNA,4)))
#define TSB_T32A3_CRA_WBFA                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->CRA,20)))
#define TSB_T32A3_STA_INTA0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STA,0)))
#define TSB_T32A3_STA_INTA1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STA,1)))
#define TSB_T32A3_STA_INTOFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STA,2)))
#define TSB_T32A3_STA_INTUFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STA,3)))
#define TSB_T32A3_IMA_IMA0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMA,0)))
#define TSB_T32A3_IMA_IMA1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMA,1)))
#define TSB_T32A3_IMA_IMOFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMA,2)))
#define TSB_T32A3_IMA_IMUFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMA,3)))
#define TSB_T32A3_DMAA_DMAENA0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->DMAA,0)))
#define TSB_T32A3_DMAA_DMAENA1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->DMAA,1)))
#define TSB_T32A3_DMAA_DMAENA2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->DMAA,2)))
#define TSB_T32A3_RUNB_RUNB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNB,0)))
#define TSB_T32A3_RUNB_SFTSTAB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNB,1)))
#define TSB_T32A3_RUNB_SFTSTPB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNB,2)))
#define TSB_T32A3_RUNB_RUNFLGB                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNB,4)))
#define TSB_T32A3_CRB_WBFB                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->CRB,20)))
#define TSB_T32A3_STB_INTB0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STB,0)))
#define TSB_T32A3_STB_INTB1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STB,1)))
#define TSB_T32A3_STB_INTOFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STB,2)))
#define TSB_T32A3_STB_INTUFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STB,3)))
#define TSB_T32A3_IMB_IMB0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMB,0)))
#define TSB_T32A3_IMB_IMB1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMB,1)))
#define TSB_T32A3_IMB_IMOFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMB,2)))
#define TSB_T32A3_IMB_IMUFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMB,3)))
#define TSB_T32A3_DMAB_DMAENB0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->DMAB,0)))
#define TSB_T32A3_DMAB_DMAENB1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->DMAB,1)))
#define TSB_T32A3_DMAB_DMAENB2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->DMAB,2)))
#define TSB_T32A3_RUNC_RUNC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNC,0)))
#define TSB_T32A3_RUNC_SFTSTAC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNC,1)))
#define TSB_T32A3_RUNC_SFTSTPC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNC,2)))
#define TSB_T32A3_RUNC_RUNFLGC                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A3->RUNC,4)))
#define TSB_T32A3_CRC_WBFC                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->CRC,20)))
#define TSB_T32A3_STC_INTC0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STC,0)))
#define TSB_T32A3_STC_INTC1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STC,1)))
#define TSB_T32A3_STC_INTOFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STC,2)))
#define TSB_T32A3_STC_INTUFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STC,3)))
#define TSB_T32A3_STC_INTSTERR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->STC,4)))
#define TSB_T32A3_IMC_IMC0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMC,0)))
#define TSB_T32A3_IMC_IMC1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMC,1)))
#define TSB_T32A3_IMC_IMOFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMC,2)))
#define TSB_T32A3_IMC_IMUFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMC,3)))
#define TSB_T32A3_IMC_IMSTERR                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->IMC,4)))
#define TSB_T32A3_DMAC_DMAENC0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->DMAC,0)))
#define TSB_T32A3_DMAC_DMAENC1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->DMAC,1)))
#define TSB_T32A3_DMAC_DMAENC2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->DMAC,2)))
#define TSB_T32A3_PLSCR_PMODE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->PLSCR,0)))
#define TSB_T32A3_PLSCR_PDIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A3->PLSCR,1)))

#define TSB_T32A4_MOD_MODE32                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->MOD,0)))
#define TSB_T32A4_MOD_HALT                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->MOD,1)))
#define TSB_T32A4_RUNA_RUNA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNA,0)))
#define TSB_T32A4_RUNA_SFTSTAA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNA,1)))
#define TSB_T32A4_RUNA_SFTSTPA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNA,2)))
#define TSB_T32A4_RUNA_RUNFLGA                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNA,4)))
#define TSB_T32A4_CRA_WBFA                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->CRA,20)))
#define TSB_T32A4_STA_INTA0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STA,0)))
#define TSB_T32A4_STA_INTA1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STA,1)))
#define TSB_T32A4_STA_INTOFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STA,2)))
#define TSB_T32A4_STA_INTUFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STA,3)))
#define TSB_T32A4_IMA_IMA0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMA,0)))
#define TSB_T32A4_IMA_IMA1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMA,1)))
#define TSB_T32A4_IMA_IMOFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMA,2)))
#define TSB_T32A4_IMA_IMUFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMA,3)))
#define TSB_T32A4_DMAA_DMAENA0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->DMAA,0)))
#define TSB_T32A4_DMAA_DMAENA1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->DMAA,1)))
#define TSB_T32A4_DMAA_DMAENA2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->DMAA,2)))
#define TSB_T32A4_RUNB_RUNB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNB,0)))
#define TSB_T32A4_RUNB_SFTSTAB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNB,1)))
#define TSB_T32A4_RUNB_SFTSTPB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNB,2)))
#define TSB_T32A4_RUNB_RUNFLGB                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNB,4)))
#define TSB_T32A4_CRB_WBFB                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->CRB,20)))
#define TSB_T32A4_STB_INTB0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STB,0)))
#define TSB_T32A4_STB_INTB1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STB,1)))
#define TSB_T32A4_STB_INTOFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STB,2)))
#define TSB_T32A4_STB_INTUFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STB,3)))
#define TSB_T32A4_IMB_IMB0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMB,0)))
#define TSB_T32A4_IMB_IMB1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMB,1)))
#define TSB_T32A4_IMB_IMOFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMB,2)))
#define TSB_T32A4_IMB_IMUFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMB,3)))
#define TSB_T32A4_DMAB_DMAENB0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->DMAB,0)))
#define TSB_T32A4_DMAB_DMAENB1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->DMAB,1)))
#define TSB_T32A4_DMAB_DMAENB2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->DMAB,2)))
#define TSB_T32A4_RUNC_RUNC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNC,0)))
#define TSB_T32A4_RUNC_SFTSTAC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNC,1)))
#define TSB_T32A4_RUNC_SFTSTPC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNC,2)))
#define TSB_T32A4_RUNC_RUNFLGC                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A4->RUNC,4)))
#define TSB_T32A4_CRC_WBFC                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->CRC,20)))
#define TSB_T32A4_STC_INTC0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STC,0)))
#define TSB_T32A4_STC_INTC1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STC,1)))
#define TSB_T32A4_STC_INTOFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STC,2)))
#define TSB_T32A4_STC_INTUFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STC,3)))
#define TSB_T32A4_STC_INTSTERR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->STC,4)))
#define TSB_T32A4_IMC_IMC0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMC,0)))
#define TSB_T32A4_IMC_IMC1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMC,1)))
#define TSB_T32A4_IMC_IMOFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMC,2)))
#define TSB_T32A4_IMC_IMUFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMC,3)))
#define TSB_T32A4_IMC_IMSTERR                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->IMC,4)))
#define TSB_T32A4_DMAC_DMAENC0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->DMAC,0)))
#define TSB_T32A4_DMAC_DMAENC1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->DMAC,1)))
#define TSB_T32A4_DMAC_DMAENC2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->DMAC,2)))
#define TSB_T32A4_PLSCR_PMODE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->PLSCR,0)))
#define TSB_T32A4_PLSCR_PDIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A4->PLSCR,1)))

#define TSB_T32A5_MOD_MODE32                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->MOD,0)))
#define TSB_T32A5_MOD_HALT                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->MOD,1)))
#define TSB_T32A5_RUNA_RUNA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNA,0)))
#define TSB_T32A5_RUNA_SFTSTAA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNA,1)))
#define TSB_T32A5_RUNA_SFTSTPA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNA,2)))
#define TSB_T32A5_RUNA_RUNFLGA                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNA,4)))
#define TSB_T32A5_CRA_WBFA                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->CRA,20)))
#define TSB_T32A5_STA_INTA0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STA,0)))
#define TSB_T32A5_STA_INTA1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STA,1)))
#define TSB_T32A5_STA_INTOFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STA,2)))
#define TSB_T32A5_STA_INTUFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STA,3)))
#define TSB_T32A5_IMA_IMA0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMA,0)))
#define TSB_T32A5_IMA_IMA1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMA,1)))
#define TSB_T32A5_IMA_IMOFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMA,2)))
#define TSB_T32A5_IMA_IMUFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMA,3)))
#define TSB_T32A5_DMAA_DMAENA0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->DMAA,0)))
#define TSB_T32A5_DMAA_DMAENA1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->DMAA,1)))
#define TSB_T32A5_DMAA_DMAENA2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->DMAA,2)))
#define TSB_T32A5_RUNB_RUNB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNB,0)))
#define TSB_T32A5_RUNB_SFTSTAB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNB,1)))
#define TSB_T32A5_RUNB_SFTSTPB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNB,2)))
#define TSB_T32A5_RUNB_RUNFLGB                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNB,4)))
#define TSB_T32A5_CRB_WBFB                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->CRB,20)))
#define TSB_T32A5_STB_INTB0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STB,0)))
#define TSB_T32A5_STB_INTB1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STB,1)))
#define TSB_T32A5_STB_INTOFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STB,2)))
#define TSB_T32A5_STB_INTUFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STB,3)))
#define TSB_T32A5_IMB_IMB0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMB,0)))
#define TSB_T32A5_IMB_IMB1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMB,1)))
#define TSB_T32A5_IMB_IMOFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMB,2)))
#define TSB_T32A5_IMB_IMUFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMB,3)))
#define TSB_T32A5_DMAB_DMAENB0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->DMAB,0)))
#define TSB_T32A5_DMAB_DMAENB1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->DMAB,1)))
#define TSB_T32A5_DMAB_DMAENB2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->DMAB,2)))
#define TSB_T32A5_RUNC_RUNC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNC,0)))
#define TSB_T32A5_RUNC_SFTSTAC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNC,1)))
#define TSB_T32A5_RUNC_SFTSTPC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNC,2)))
#define TSB_T32A5_RUNC_RUNFLGC                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A5->RUNC,4)))
#define TSB_T32A5_CRC_WBFC                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->CRC,20)))
#define TSB_T32A5_STC_INTC0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STC,0)))
#define TSB_T32A5_STC_INTC1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STC,1)))
#define TSB_T32A5_STC_INTOFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STC,2)))
#define TSB_T32A5_STC_INTUFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STC,3)))
#define TSB_T32A5_STC_INTSTERR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->STC,4)))
#define TSB_T32A5_IMC_IMC0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMC,0)))
#define TSB_T32A5_IMC_IMC1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMC,1)))
#define TSB_T32A5_IMC_IMOFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMC,2)))
#define TSB_T32A5_IMC_IMUFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMC,3)))
#define TSB_T32A5_IMC_IMSTERR                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->IMC,4)))
#define TSB_T32A5_DMAC_DMAENC0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->DMAC,0)))
#define TSB_T32A5_DMAC_DMAENC1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->DMAC,1)))
#define TSB_T32A5_DMAC_DMAENC2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->DMAC,2)))
#define TSB_T32A5_PLSCR_PMODE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->PLSCR,0)))
#define TSB_T32A5_PLSCR_PDIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A5->PLSCR,1)))

#define TSB_T32A6_MOD_MODE32                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->MOD,0)))
#define TSB_T32A6_MOD_HALT                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->MOD,1)))
#define TSB_T32A6_RUNA_RUNA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->RUNA,0)))
#define TSB_T32A6_RUNA_SFTSTAA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A6->RUNA,1)))
#define TSB_T32A6_RUNA_SFTSTPA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A6->RUNA,2)))
#define TSB_T32A6_RUNA_RUNFLGA                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A6->RUNA,4)))
#define TSB_T32A6_CRA_WBFA                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->CRA,20)))
#define TSB_T32A6_STA_INTA0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->STA,0)))
#define TSB_T32A6_STA_INTA1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->STA,1)))
#define TSB_T32A6_STA_INTOFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->STA,2)))
#define TSB_T32A6_STA_INTUFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->STA,3)))
#define TSB_T32A6_IMA_IMA0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->IMA,0)))
#define TSB_T32A6_IMA_IMA1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->IMA,1)))
#define TSB_T32A6_IMA_IMOFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->IMA,2)))
#define TSB_T32A6_IMA_IMUFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->IMA,3)))
#define TSB_T32A6_DMAA_DMAENA0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->DMAA,0)))
#define TSB_T32A6_DMAA_DMAENA1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->DMAA,1)))
#define TSB_T32A6_DMAA_DMAENA2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->DMAA,2)))
#define TSB_T32A6_RUNB_RUNB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->RUNB,0)))
#define TSB_T32A6_RUNB_SFTSTAB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A6->RUNB,1)))
#define TSB_T32A6_RUNB_SFTSTPB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A6->RUNB,2)))
#define TSB_T32A6_RUNB_RUNFLGB                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A6->RUNB,4)))
#define TSB_T32A6_CRB_WBFB                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->CRB,20)))
#define TSB_T32A6_STB_INTB0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->STB,0)))
#define TSB_T32A6_STB_INTB1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->STB,1)))
#define TSB_T32A6_STB_INTOFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->STB,2)))
#define TSB_T32A6_STB_INTUFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->STB,3)))
#define TSB_T32A6_IMB_IMB0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->IMB,0)))
#define TSB_T32A6_IMB_IMB1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->IMB,1)))
#define TSB_T32A6_IMB_IMOFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->IMB,2)))
#define TSB_T32A6_IMB_IMUFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->IMB,3)))
#define TSB_T32A6_DMAB_DMAENB0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->DMAB,0)))
#define TSB_T32A6_DMAB_DMAENB1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->DMAB,1)))
#define TSB_T32A6_DMAB_DMAENB2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->DMAB,2)))
#define TSB_T32A6_RUNC_RUNC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->RUNC,0)))
#define TSB_T32A6_RUNC_SFTSTAC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A6->RUNC,1)))
#define TSB_T32A6_RUNC_SFTSTPC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A6->RUNC,2)))
#define TSB_T32A6_RUNC_RUNFLGC                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A6->RUNC,4)))
#define TSB_T32A6_CRC_WBFC                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->CRC,20)))
#define TSB_T32A6_STC_INTC0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->STC,0)))
#define TSB_T32A6_STC_INTC1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->STC,1)))
#define TSB_T32A6_STC_INTOFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->STC,2)))
#define TSB_T32A6_STC_INTUFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->STC,3)))
#define TSB_T32A6_STC_INTSTERR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->STC,4)))
#define TSB_T32A6_IMC_IMC0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->IMC,0)))
#define TSB_T32A6_IMC_IMC1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->IMC,1)))
#define TSB_T32A6_IMC_IMOFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->IMC,2)))
#define TSB_T32A6_IMC_IMUFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->IMC,3)))
#define TSB_T32A6_IMC_IMSTERR                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->IMC,4)))
#define TSB_T32A6_DMAC_DMAENC0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->DMAC,0)))
#define TSB_T32A6_DMAC_DMAENC1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->DMAC,1)))
#define TSB_T32A6_DMAC_DMAENC2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->DMAC,2)))
#define TSB_T32A6_PLSCR_PMODE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->PLSCR,0)))
#define TSB_T32A6_PLSCR_PDIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A6->PLSCR,1)))

#define TSB_T32A7_MOD_MODE32                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->MOD,0)))
#define TSB_T32A7_MOD_HALT                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->MOD,1)))
#define TSB_T32A7_RUNA_RUNA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->RUNA,0)))
#define TSB_T32A7_RUNA_SFTSTAA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A7->RUNA,1)))
#define TSB_T32A7_RUNA_SFTSTPA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A7->RUNA,2)))
#define TSB_T32A7_RUNA_RUNFLGA                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A7->RUNA,4)))
#define TSB_T32A7_CRA_WBFA                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->CRA,20)))
#define TSB_T32A7_STA_INTA0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->STA,0)))
#define TSB_T32A7_STA_INTA1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->STA,1)))
#define TSB_T32A7_STA_INTOFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->STA,2)))
#define TSB_T32A7_STA_INTUFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->STA,3)))
#define TSB_T32A7_IMA_IMA0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->IMA,0)))
#define TSB_T32A7_IMA_IMA1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->IMA,1)))
#define TSB_T32A7_IMA_IMOFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->IMA,2)))
#define TSB_T32A7_IMA_IMUFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->IMA,3)))
#define TSB_T32A7_DMAA_DMAENA0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->DMAA,0)))
#define TSB_T32A7_DMAA_DMAENA1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->DMAA,1)))
#define TSB_T32A7_DMAA_DMAENA2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->DMAA,2)))
#define TSB_T32A7_RUNB_RUNB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->RUNB,0)))
#define TSB_T32A7_RUNB_SFTSTAB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A7->RUNB,1)))
#define TSB_T32A7_RUNB_SFTSTPB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A7->RUNB,2)))
#define TSB_T32A7_RUNB_RUNFLGB                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A7->RUNB,4)))
#define TSB_T32A7_CRB_WBFB                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->CRB,20)))
#define TSB_T32A7_STB_INTB0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->STB,0)))
#define TSB_T32A7_STB_INTB1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->STB,1)))
#define TSB_T32A7_STB_INTOFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->STB,2)))
#define TSB_T32A7_STB_INTUFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->STB,3)))
#define TSB_T32A7_IMB_IMB0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->IMB,0)))
#define TSB_T32A7_IMB_IMB1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->IMB,1)))
#define TSB_T32A7_IMB_IMOFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->IMB,2)))
#define TSB_T32A7_IMB_IMUFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->IMB,3)))
#define TSB_T32A7_DMAB_DMAENB0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->DMAB,0)))
#define TSB_T32A7_DMAB_DMAENB1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->DMAB,1)))
#define TSB_T32A7_DMAB_DMAENB2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->DMAB,2)))
#define TSB_T32A7_RUNC_RUNC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->RUNC,0)))
#define TSB_T32A7_RUNC_SFTSTAC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A7->RUNC,1)))
#define TSB_T32A7_RUNC_SFTSTPC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A7->RUNC,2)))
#define TSB_T32A7_RUNC_RUNFLGC                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A7->RUNC,4)))
#define TSB_T32A7_CRC_WBFC                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->CRC,20)))
#define TSB_T32A7_STC_INTC0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->STC,0)))
#define TSB_T32A7_STC_INTC1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->STC,1)))
#define TSB_T32A7_STC_INTOFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->STC,2)))
#define TSB_T32A7_STC_INTUFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->STC,3)))
#define TSB_T32A7_STC_INTSTERR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->STC,4)))
#define TSB_T32A7_IMC_IMC0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->IMC,0)))
#define TSB_T32A7_IMC_IMC1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->IMC,1)))
#define TSB_T32A7_IMC_IMOFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->IMC,2)))
#define TSB_T32A7_IMC_IMUFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->IMC,3)))
#define TSB_T32A7_IMC_IMSTERR                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->IMC,4)))
#define TSB_T32A7_DMAC_DMAENC0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->DMAC,0)))
#define TSB_T32A7_DMAC_DMAENC1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->DMAC,1)))
#define TSB_T32A7_DMAC_DMAENC2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->DMAC,2)))
#define TSB_T32A7_PLSCR_PMODE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->PLSCR,0)))
#define TSB_T32A7_PLSCR_PDIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A7->PLSCR,1)))

#define TSB_T32A8_MOD_MODE32                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->MOD,0)))
#define TSB_T32A8_MOD_HALT                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->MOD,1)))
#define TSB_T32A8_RUNA_RUNA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->RUNA,0)))
#define TSB_T32A8_RUNA_SFTSTAA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A8->RUNA,1)))
#define TSB_T32A8_RUNA_SFTSTPA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A8->RUNA,2)))
#define TSB_T32A8_RUNA_RUNFLGA                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A8->RUNA,4)))
#define TSB_T32A8_CRA_WBFA                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->CRA,20)))
#define TSB_T32A8_STA_INTA0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->STA,0)))
#define TSB_T32A8_STA_INTA1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->STA,1)))
#define TSB_T32A8_STA_INTOFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->STA,2)))
#define TSB_T32A8_STA_INTUFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->STA,3)))
#define TSB_T32A8_IMA_IMA0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->IMA,0)))
#define TSB_T32A8_IMA_IMA1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->IMA,1)))
#define TSB_T32A8_IMA_IMOFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->IMA,2)))
#define TSB_T32A8_IMA_IMUFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->IMA,3)))
#define TSB_T32A8_DMAA_DMAENA0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->DMAA,0)))
#define TSB_T32A8_DMAA_DMAENA1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->DMAA,1)))
#define TSB_T32A8_DMAA_DMAENA2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->DMAA,2)))
#define TSB_T32A8_RUNB_RUNB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->RUNB,0)))
#define TSB_T32A8_RUNB_SFTSTAB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A8->RUNB,1)))
#define TSB_T32A8_RUNB_SFTSTPB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A8->RUNB,2)))
#define TSB_T32A8_RUNB_RUNFLGB                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A8->RUNB,4)))
#define TSB_T32A8_CRB_WBFB                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->CRB,20)))
#define TSB_T32A8_STB_INTB0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->STB,0)))
#define TSB_T32A8_STB_INTB1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->STB,1)))
#define TSB_T32A8_STB_INTOFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->STB,2)))
#define TSB_T32A8_STB_INTUFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->STB,3)))
#define TSB_T32A8_IMB_IMB0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->IMB,0)))
#define TSB_T32A8_IMB_IMB1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->IMB,1)))
#define TSB_T32A8_IMB_IMOFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->IMB,2)))
#define TSB_T32A8_IMB_IMUFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->IMB,3)))
#define TSB_T32A8_DMAB_DMAENB0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->DMAB,0)))
#define TSB_T32A8_DMAB_DMAENB1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->DMAB,1)))
#define TSB_T32A8_DMAB_DMAENB2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->DMAB,2)))
#define TSB_T32A8_RUNC_RUNC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->RUNC,0)))
#define TSB_T32A8_RUNC_SFTSTAC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A8->RUNC,1)))
#define TSB_T32A8_RUNC_SFTSTPC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A8->RUNC,2)))
#define TSB_T32A8_RUNC_RUNFLGC                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A8->RUNC,4)))
#define TSB_T32A8_CRC_WBFC                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->CRC,20)))
#define TSB_T32A8_STC_INTC0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->STC,0)))
#define TSB_T32A8_STC_INTC1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->STC,1)))
#define TSB_T32A8_STC_INTOFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->STC,2)))
#define TSB_T32A8_STC_INTUFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->STC,3)))
#define TSB_T32A8_STC_INTSTERR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->STC,4)))
#define TSB_T32A8_IMC_IMC0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->IMC,0)))
#define TSB_T32A8_IMC_IMC1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->IMC,1)))
#define TSB_T32A8_IMC_IMOFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->IMC,2)))
#define TSB_T32A8_IMC_IMUFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->IMC,3)))
#define TSB_T32A8_IMC_IMSTERR                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->IMC,4)))
#define TSB_T32A8_DMAC_DMAENC0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->DMAC,0)))
#define TSB_T32A8_DMAC_DMAENC1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->DMAC,1)))
#define TSB_T32A8_DMAC_DMAENC2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->DMAC,2)))
#define TSB_T32A8_PLSCR_PMODE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->PLSCR,0)))
#define TSB_T32A8_PLSCR_PDIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A8->PLSCR,1)))

#define TSB_T32A9_MOD_MODE32                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->MOD,0)))
#define TSB_T32A9_MOD_HALT                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->MOD,1)))
#define TSB_T32A9_RUNA_RUNA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->RUNA,0)))
#define TSB_T32A9_RUNA_SFTSTAA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A9->RUNA,1)))
#define TSB_T32A9_RUNA_SFTSTPA                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A9->RUNA,2)))
#define TSB_T32A9_RUNA_RUNFLGA                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A9->RUNA,4)))
#define TSB_T32A9_CRA_WBFA                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->CRA,20)))
#define TSB_T32A9_STA_INTA0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->STA,0)))
#define TSB_T32A9_STA_INTA1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->STA,1)))
#define TSB_T32A9_STA_INTOFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->STA,2)))
#define TSB_T32A9_STA_INTUFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->STA,3)))
#define TSB_T32A9_IMA_IMA0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->IMA,0)))
#define TSB_T32A9_IMA_IMA1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->IMA,1)))
#define TSB_T32A9_IMA_IMOFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->IMA,2)))
#define TSB_T32A9_IMA_IMUFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->IMA,3)))
#define TSB_T32A9_DMAA_DMAENA0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->DMAA,0)))
#define TSB_T32A9_DMAA_DMAENA1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->DMAA,1)))
#define TSB_T32A9_DMAA_DMAENA2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->DMAA,2)))
#define TSB_T32A9_RUNB_RUNB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->RUNB,0)))
#define TSB_T32A9_RUNB_SFTSTAB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A9->RUNB,1)))
#define TSB_T32A9_RUNB_SFTSTPB                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A9->RUNB,2)))
#define TSB_T32A9_RUNB_RUNFLGB                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A9->RUNB,4)))
#define TSB_T32A9_CRB_WBFB                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->CRB,20)))
#define TSB_T32A9_STB_INTB0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->STB,0)))
#define TSB_T32A9_STB_INTB1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->STB,1)))
#define TSB_T32A9_STB_INTOFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->STB,2)))
#define TSB_T32A9_STB_INTUFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->STB,3)))
#define TSB_T32A9_IMB_IMB0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->IMB,0)))
#define TSB_T32A9_IMB_IMB1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->IMB,1)))
#define TSB_T32A9_IMB_IMOFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->IMB,2)))
#define TSB_T32A9_IMB_IMUFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->IMB,3)))
#define TSB_T32A9_DMAB_DMAENB0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->DMAB,0)))
#define TSB_T32A9_DMAB_DMAENB1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->DMAB,1)))
#define TSB_T32A9_DMAB_DMAENB2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->DMAB,2)))
#define TSB_T32A9_RUNC_RUNC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->RUNC,0)))
#define TSB_T32A9_RUNC_SFTSTAC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A9->RUNC,1)))
#define TSB_T32A9_RUNC_SFTSTPC                    (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A9->RUNC,2)))
#define TSB_T32A9_RUNC_RUNFLGC                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A9->RUNC,4)))
#define TSB_T32A9_CRC_WBFC                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->CRC,20)))
#define TSB_T32A9_STC_INTC0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->STC,0)))
#define TSB_T32A9_STC_INTC1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->STC,1)))
#define TSB_T32A9_STC_INTOFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->STC,2)))
#define TSB_T32A9_STC_INTUFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->STC,3)))
#define TSB_T32A9_STC_INTSTERR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->STC,4)))
#define TSB_T32A9_IMC_IMC0                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->IMC,0)))
#define TSB_T32A9_IMC_IMC1                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->IMC,1)))
#define TSB_T32A9_IMC_IMOFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->IMC,2)))
#define TSB_T32A9_IMC_IMUFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->IMC,3)))
#define TSB_T32A9_IMC_IMSTERR                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->IMC,4)))
#define TSB_T32A9_DMAC_DMAENC0                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->DMAC,0)))
#define TSB_T32A9_DMAC_DMAENC1                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->DMAC,1)))
#define TSB_T32A9_DMAC_DMAENC2                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->DMAC,2)))
#define TSB_T32A9_PLSCR_PMODE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->PLSCR,0)))
#define TSB_T32A9_PLSCR_PDIR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A9->PLSCR,1)))

#define TSB_T32A10_MOD_MODE32                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->MOD,0)))
#define TSB_T32A10_MOD_HALT                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->MOD,1)))
#define TSB_T32A10_RUNA_RUNA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->RUNA,0)))
#define TSB_T32A10_RUNA_SFTSTAA                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A10->RUNA,1)))
#define TSB_T32A10_RUNA_SFTSTPA                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A10->RUNA,2)))
#define TSB_T32A10_RUNA_RUNFLGA                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A10->RUNA,4)))
#define TSB_T32A10_CRA_WBFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->CRA,20)))
#define TSB_T32A10_STA_INTA0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->STA,0)))
#define TSB_T32A10_STA_INTA1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->STA,1)))
#define TSB_T32A10_STA_INTOFA                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->STA,2)))
#define TSB_T32A10_STA_INTUFA                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->STA,3)))
#define TSB_T32A10_IMA_IMA0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->IMA,0)))
#define TSB_T32A10_IMA_IMA1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->IMA,1)))
#define TSB_T32A10_IMA_IMOFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->IMA,2)))
#define TSB_T32A10_IMA_IMUFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->IMA,3)))
#define TSB_T32A10_DMAA_DMAENA0                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->DMAA,0)))
#define TSB_T32A10_DMAA_DMAENA1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->DMAA,1)))
#define TSB_T32A10_DMAA_DMAENA2                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->DMAA,2)))
#define TSB_T32A10_RUNB_RUNB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->RUNB,0)))
#define TSB_T32A10_RUNB_SFTSTAB                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A10->RUNB,1)))
#define TSB_T32A10_RUNB_SFTSTPB                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A10->RUNB,2)))
#define TSB_T32A10_RUNB_RUNFLGB                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A10->RUNB,4)))
#define TSB_T32A10_CRB_WBFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->CRB,20)))
#define TSB_T32A10_STB_INTB0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->STB,0)))
#define TSB_T32A10_STB_INTB1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->STB,1)))
#define TSB_T32A10_STB_INTOFB                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->STB,2)))
#define TSB_T32A10_STB_INTUFB                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->STB,3)))
#define TSB_T32A10_IMB_IMB0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->IMB,0)))
#define TSB_T32A10_IMB_IMB1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->IMB,1)))
#define TSB_T32A10_IMB_IMOFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->IMB,2)))
#define TSB_T32A10_IMB_IMUFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->IMB,3)))
#define TSB_T32A10_DMAB_DMAENB0                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->DMAB,0)))
#define TSB_T32A10_DMAB_DMAENB1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->DMAB,1)))
#define TSB_T32A10_DMAB_DMAENB2                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->DMAB,2)))
#define TSB_T32A10_RUNC_RUNC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->RUNC,0)))
#define TSB_T32A10_RUNC_SFTSTAC                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A10->RUNC,1)))
#define TSB_T32A10_RUNC_SFTSTPC                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A10->RUNC,2)))
#define TSB_T32A10_RUNC_RUNFLGC                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A10->RUNC,4)))
#define TSB_T32A10_CRC_WBFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->CRC,20)))
#define TSB_T32A10_STC_INTC0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->STC,0)))
#define TSB_T32A10_STC_INTC1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->STC,1)))
#define TSB_T32A10_STC_INTOFC                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->STC,2)))
#define TSB_T32A10_STC_INTUFC                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->STC,3)))
#define TSB_T32A10_STC_INTSTERR                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->STC,4)))
#define TSB_T32A10_IMC_IMC0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->IMC,0)))
#define TSB_T32A10_IMC_IMC1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->IMC,1)))
#define TSB_T32A10_IMC_IMOFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->IMC,2)))
#define TSB_T32A10_IMC_IMUFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->IMC,3)))
#define TSB_T32A10_IMC_IMSTERR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->IMC,4)))
#define TSB_T32A10_DMAC_DMAENC0                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->DMAC,0)))
#define TSB_T32A10_DMAC_DMAENC1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->DMAC,1)))
#define TSB_T32A10_DMAC_DMAENC2                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->DMAC,2)))
#define TSB_T32A10_PLSCR_PMODE                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->PLSCR,0)))
#define TSB_T32A10_PLSCR_PDIR                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A10->PLSCR,1)))

#define TSB_T32A11_MOD_MODE32                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->MOD,0)))
#define TSB_T32A11_MOD_HALT                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->MOD,1)))
#define TSB_T32A11_RUNA_RUNA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->RUNA,0)))
#define TSB_T32A11_RUNA_SFTSTAA                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A11->RUNA,1)))
#define TSB_T32A11_RUNA_SFTSTPA                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A11->RUNA,2)))
#define TSB_T32A11_RUNA_RUNFLGA                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A11->RUNA,4)))
#define TSB_T32A11_CRA_WBFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->CRA,20)))
#define TSB_T32A11_STA_INTA0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->STA,0)))
#define TSB_T32A11_STA_INTA1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->STA,1)))
#define TSB_T32A11_STA_INTOFA                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->STA,2)))
#define TSB_T32A11_STA_INTUFA                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->STA,3)))
#define TSB_T32A11_IMA_IMA0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->IMA,0)))
#define TSB_T32A11_IMA_IMA1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->IMA,1)))
#define TSB_T32A11_IMA_IMOFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->IMA,2)))
#define TSB_T32A11_IMA_IMUFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->IMA,3)))
#define TSB_T32A11_DMAA_DMAENA0                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->DMAA,0)))
#define TSB_T32A11_DMAA_DMAENA1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->DMAA,1)))
#define TSB_T32A11_DMAA_DMAENA2                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->DMAA,2)))
#define TSB_T32A11_RUNB_RUNB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->RUNB,0)))
#define TSB_T32A11_RUNB_SFTSTAB                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A11->RUNB,1)))
#define TSB_T32A11_RUNB_SFTSTPB                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A11->RUNB,2)))
#define TSB_T32A11_RUNB_RUNFLGB                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A11->RUNB,4)))
#define TSB_T32A11_CRB_WBFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->CRB,20)))
#define TSB_T32A11_STB_INTB0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->STB,0)))
#define TSB_T32A11_STB_INTB1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->STB,1)))
#define TSB_T32A11_STB_INTOFB                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->STB,2)))
#define TSB_T32A11_STB_INTUFB                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->STB,3)))
#define TSB_T32A11_IMB_IMB0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->IMB,0)))
#define TSB_T32A11_IMB_IMB1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->IMB,1)))
#define TSB_T32A11_IMB_IMOFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->IMB,2)))
#define TSB_T32A11_IMB_IMUFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->IMB,3)))
#define TSB_T32A11_DMAB_DMAENB0                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->DMAB,0)))
#define TSB_T32A11_DMAB_DMAENB1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->DMAB,1)))
#define TSB_T32A11_DMAB_DMAENB2                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->DMAB,2)))
#define TSB_T32A11_RUNC_RUNC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->RUNC,0)))
#define TSB_T32A11_RUNC_SFTSTAC                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A11->RUNC,1)))
#define TSB_T32A11_RUNC_SFTSTPC                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A11->RUNC,2)))
#define TSB_T32A11_RUNC_RUNFLGC                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A11->RUNC,4)))
#define TSB_T32A11_CRC_WBFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->CRC,20)))
#define TSB_T32A11_STC_INTC0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->STC,0)))
#define TSB_T32A11_STC_INTC1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->STC,1)))
#define TSB_T32A11_STC_INTOFC                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->STC,2)))
#define TSB_T32A11_STC_INTUFC                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->STC,3)))
#define TSB_T32A11_STC_INTSTERR                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->STC,4)))
#define TSB_T32A11_IMC_IMC0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->IMC,0)))
#define TSB_T32A11_IMC_IMC1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->IMC,1)))
#define TSB_T32A11_IMC_IMOFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->IMC,2)))
#define TSB_T32A11_IMC_IMUFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->IMC,3)))
#define TSB_T32A11_IMC_IMSTERR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->IMC,4)))
#define TSB_T32A11_DMAC_DMAENC0                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->DMAC,0)))
#define TSB_T32A11_DMAC_DMAENC1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->DMAC,1)))
#define TSB_T32A11_DMAC_DMAENC2                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->DMAC,2)))
#define TSB_T32A11_PLSCR_PMODE                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->PLSCR,0)))
#define TSB_T32A11_PLSCR_PDIR                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A11->PLSCR,1)))

#define TSB_T32A12_MOD_MODE32                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->MOD,0)))
#define TSB_T32A12_MOD_HALT                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->MOD,1)))
#define TSB_T32A12_RUNA_RUNA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->RUNA,0)))
#define TSB_T32A12_RUNA_SFTSTAA                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A12->RUNA,1)))
#define TSB_T32A12_RUNA_SFTSTPA                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A12->RUNA,2)))
#define TSB_T32A12_RUNA_RUNFLGA                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A12->RUNA,4)))
#define TSB_T32A12_CRA_WBFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->CRA,20)))
#define TSB_T32A12_STA_INTA0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->STA,0)))
#define TSB_T32A12_STA_INTA1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->STA,1)))
#define TSB_T32A12_STA_INTOFA                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->STA,2)))
#define TSB_T32A12_STA_INTUFA                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->STA,3)))
#define TSB_T32A12_IMA_IMA0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->IMA,0)))
#define TSB_T32A12_IMA_IMA1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->IMA,1)))
#define TSB_T32A12_IMA_IMOFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->IMA,2)))
#define TSB_T32A12_IMA_IMUFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->IMA,3)))
#define TSB_T32A12_DMAA_DMAENA0                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->DMAA,0)))
#define TSB_T32A12_DMAA_DMAENA1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->DMAA,1)))
#define TSB_T32A12_DMAA_DMAENA2                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->DMAA,2)))
#define TSB_T32A12_RUNB_RUNB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->RUNB,0)))
#define TSB_T32A12_RUNB_SFTSTAB                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A12->RUNB,1)))
#define TSB_T32A12_RUNB_SFTSTPB                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A12->RUNB,2)))
#define TSB_T32A12_RUNB_RUNFLGB                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A12->RUNB,4)))
#define TSB_T32A12_CRB_WBFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->CRB,20)))
#define TSB_T32A12_STB_INTB0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->STB,0)))
#define TSB_T32A12_STB_INTB1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->STB,1)))
#define TSB_T32A12_STB_INTOFB                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->STB,2)))
#define TSB_T32A12_STB_INTUFB                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->STB,3)))
#define TSB_T32A12_IMB_IMB0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->IMB,0)))
#define TSB_T32A12_IMB_IMB1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->IMB,1)))
#define TSB_T32A12_IMB_IMOFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->IMB,2)))
#define TSB_T32A12_IMB_IMUFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->IMB,3)))
#define TSB_T32A12_DMAB_DMAENB0                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->DMAB,0)))
#define TSB_T32A12_DMAB_DMAENB1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->DMAB,1)))
#define TSB_T32A12_DMAB_DMAENB2                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->DMAB,2)))
#define TSB_T32A12_RUNC_RUNC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->RUNC,0)))
#define TSB_T32A12_RUNC_SFTSTAC                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A12->RUNC,1)))
#define TSB_T32A12_RUNC_SFTSTPC                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A12->RUNC,2)))
#define TSB_T32A12_RUNC_RUNFLGC                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A12->RUNC,4)))
#define TSB_T32A12_CRC_WBFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->CRC,20)))
#define TSB_T32A12_STC_INTC0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->STC,0)))
#define TSB_T32A12_STC_INTC1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->STC,1)))
#define TSB_T32A12_STC_INTOFC                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->STC,2)))
#define TSB_T32A12_STC_INTUFC                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->STC,3)))
#define TSB_T32A12_STC_INTSTERR                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->STC,4)))
#define TSB_T32A12_IMC_IMC0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->IMC,0)))
#define TSB_T32A12_IMC_IMC1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->IMC,1)))
#define TSB_T32A12_IMC_IMOFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->IMC,2)))
#define TSB_T32A12_IMC_IMUFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->IMC,3)))
#define TSB_T32A12_IMC_IMSTERR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->IMC,4)))
#define TSB_T32A12_DMAC_DMAENC0                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->DMAC,0)))
#define TSB_T32A12_DMAC_DMAENC1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->DMAC,1)))
#define TSB_T32A12_DMAC_DMAENC2                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->DMAC,2)))
#define TSB_T32A12_PLSCR_PMODE                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->PLSCR,0)))
#define TSB_T32A12_PLSCR_PDIR                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A12->PLSCR,1)))

#define TSB_T32A13_MOD_MODE32                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->MOD,0)))
#define TSB_T32A13_MOD_HALT                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->MOD,1)))
#define TSB_T32A13_RUNA_RUNA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->RUNA,0)))
#define TSB_T32A13_RUNA_SFTSTAA                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A13->RUNA,1)))
#define TSB_T32A13_RUNA_SFTSTPA                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A13->RUNA,2)))
#define TSB_T32A13_RUNA_RUNFLGA                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A13->RUNA,4)))
#define TSB_T32A13_CRA_WBFA                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->CRA,20)))
#define TSB_T32A13_STA_INTA0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->STA,0)))
#define TSB_T32A13_STA_INTA1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->STA,1)))
#define TSB_T32A13_STA_INTOFA                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->STA,2)))
#define TSB_T32A13_STA_INTUFA                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->STA,3)))
#define TSB_T32A13_IMA_IMA0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->IMA,0)))
#define TSB_T32A13_IMA_IMA1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->IMA,1)))
#define TSB_T32A13_IMA_IMOFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->IMA,2)))
#define TSB_T32A13_IMA_IMUFA                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->IMA,3)))
#define TSB_T32A13_DMAA_DMAENA0                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->DMAA,0)))
#define TSB_T32A13_DMAA_DMAENA1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->DMAA,1)))
#define TSB_T32A13_DMAA_DMAENA2                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->DMAA,2)))
#define TSB_T32A13_RUNB_RUNB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->RUNB,0)))
#define TSB_T32A13_RUNB_SFTSTAB                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A13->RUNB,1)))
#define TSB_T32A13_RUNB_SFTSTPB                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A13->RUNB,2)))
#define TSB_T32A13_RUNB_RUNFLGB                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A13->RUNB,4)))
#define TSB_T32A13_CRB_WBFB                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->CRB,20)))
#define TSB_T32A13_STB_INTB0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->STB,0)))
#define TSB_T32A13_STB_INTB1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->STB,1)))
#define TSB_T32A13_STB_INTOFB                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->STB,2)))
#define TSB_T32A13_STB_INTUFB                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->STB,3)))
#define TSB_T32A13_IMB_IMB0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->IMB,0)))
#define TSB_T32A13_IMB_IMB1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->IMB,1)))
#define TSB_T32A13_IMB_IMOFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->IMB,2)))
#define TSB_T32A13_IMB_IMUFB                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->IMB,3)))
#define TSB_T32A13_DMAB_DMAENB0                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->DMAB,0)))
#define TSB_T32A13_DMAB_DMAENB1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->DMAB,1)))
#define TSB_T32A13_DMAB_DMAENB2                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->DMAB,2)))
#define TSB_T32A13_RUNC_RUNC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->RUNC,0)))
#define TSB_T32A13_RUNC_SFTSTAC                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A13->RUNC,1)))
#define TSB_T32A13_RUNC_SFTSTPC                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_T32A13->RUNC,2)))
#define TSB_T32A13_RUNC_RUNFLGC                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_T32A13->RUNC,4)))
#define TSB_T32A13_CRC_WBFC                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->CRC,20)))
#define TSB_T32A13_STC_INTC0                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->STC,0)))
#define TSB_T32A13_STC_INTC1                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->STC,1)))
#define TSB_T32A13_STC_INTOFC                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->STC,2)))
#define TSB_T32A13_STC_INTUFC                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->STC,3)))
#define TSB_T32A13_STC_INTSTERR                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->STC,4)))
#define TSB_T32A13_IMC_IMC0                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->IMC,0)))
#define TSB_T32A13_IMC_IMC1                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->IMC,1)))
#define TSB_T32A13_IMC_IMOFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->IMC,2)))
#define TSB_T32A13_IMC_IMUFC                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->IMC,3)))
#define TSB_T32A13_IMC_IMSTERR                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->IMC,4)))
#define TSB_T32A13_DMAC_DMAENC0                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->DMAC,0)))
#define TSB_T32A13_DMAC_DMAENC1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->DMAC,1)))
#define TSB_T32A13_DMAC_DMAENC2                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->DMAC,2)))
#define TSB_T32A13_PLSCR_PMODE                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->PLSCR,0)))
#define TSB_T32A13_PLSCR_PDIR                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_T32A13->PLSCR,1)))


/* UART */
#define TSB_UART0_SWRST_SWRSTF                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->SWRST,7)))
#define TSB_UART0_CR0_PE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR0,2)))
#define TSB_UART0_CR0_EVEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR0,3)))
#define TSB_UART0_CR0_SBLEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR0,4)))
#define TSB_UART0_CR0_DIR                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR0,5)))
#define TSB_UART0_CR0_IV                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR0,6)))
#define TSB_UART0_CR0_WU                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR0,8)))
#define TSB_UART0_CR0_RTSE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR0,9)))
#define TSB_UART0_CR0_CTSE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR0,10)))
#define TSB_UART0_CR0_LPB                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR0,15)))
#define TSB_UART0_CR1_DMARE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR1,0)))
#define TSB_UART0_CR1_DMATE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR1,1)))
#define TSB_UART0_CR1_INTERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR1,2)))
#define TSB_UART0_CR1_INTRXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR1,4)))
#define TSB_UART0_CR1_INTRXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR1,5)))
#define TSB_UART0_CR1_INTTXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR1,6)))
#define TSB_UART0_CR1_INTTXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->CR1,7)))
#define TSB_UART0_BRD_KEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->BRD,23)))
#define TSB_UART0_TRANS_RXE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->TRANS,0)))
#define TSB_UART0_TRANS_TXE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->TRANS,1)))
#define TSB_UART0_TRANS_TXTRG                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->TRANS,2)))
#define TSB_UART0_TRANS_BK                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->TRANS,3)))
#define TSB_UART0_DR_BERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->DR,16)))
#define TSB_UART0_DR_FERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->DR,17)))
#define TSB_UART0_DR_PERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->DR,18)))
#define TSB_UART0_SR_RXFF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->SR,5)))
#define TSB_UART0_SR_RXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->SR,6)))
#define TSB_UART0_SR_RXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->SR,7)))
#define TSB_UART0_SR_TXFF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->SR,13)))
#define TSB_UART0_SR_TXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->SR,14)))
#define TSB_UART0_SR_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->SR,15)))
#define TSB_UART0_SR_SUE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART0->SR,31)))
#define TSB_UART0_FIFOCLR_RFCLR                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART0->FIFOCLR,0)))
#define TSB_UART0_FIFOCLR_TFCLR                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART0->FIFOCLR,1)))
#define TSB_UART0_ERR_BERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->ERR,0)))
#define TSB_UART0_ERR_FERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->ERR,1)))
#define TSB_UART0_ERR_PERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->ERR,2)))
#define TSB_UART0_ERR_OVRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->ERR,3)))
#define TSB_UART0_ERR_TRGERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART0->ERR,4)))

#define TSB_UART1_SWRST_SWRSTF                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->SWRST,7)))
#define TSB_UART1_CR0_PE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR0,2)))
#define TSB_UART1_CR0_EVEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR0,3)))
#define TSB_UART1_CR0_SBLEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR0,4)))
#define TSB_UART1_CR0_DIR                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR0,5)))
#define TSB_UART1_CR0_IV                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR0,6)))
#define TSB_UART1_CR0_WU                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR0,8)))
#define TSB_UART1_CR0_RTSE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR0,9)))
#define TSB_UART1_CR0_CTSE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR0,10)))
#define TSB_UART1_CR0_LPB                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR0,15)))
#define TSB_UART1_CR1_DMARE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR1,0)))
#define TSB_UART1_CR1_DMATE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR1,1)))
#define TSB_UART1_CR1_INTERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR1,2)))
#define TSB_UART1_CR1_INTRXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR1,4)))
#define TSB_UART1_CR1_INTRXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR1,5)))
#define TSB_UART1_CR1_INTTXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR1,6)))
#define TSB_UART1_CR1_INTTXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->CR1,7)))
#define TSB_UART1_BRD_KEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->BRD,23)))
#define TSB_UART1_TRANS_RXE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->TRANS,0)))
#define TSB_UART1_TRANS_TXE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->TRANS,1)))
#define TSB_UART1_TRANS_TXTRG                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->TRANS,2)))
#define TSB_UART1_TRANS_BK                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->TRANS,3)))
#define TSB_UART1_DR_BERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->DR,16)))
#define TSB_UART1_DR_FERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->DR,17)))
#define TSB_UART1_DR_PERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->DR,18)))
#define TSB_UART1_SR_RXFF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->SR,5)))
#define TSB_UART1_SR_RXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->SR,6)))
#define TSB_UART1_SR_RXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->SR,7)))
#define TSB_UART1_SR_TXFF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->SR,13)))
#define TSB_UART1_SR_TXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->SR,14)))
#define TSB_UART1_SR_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->SR,15)))
#define TSB_UART1_SR_SUE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART1->SR,31)))
#define TSB_UART1_FIFOCLR_RFCLR                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART1->FIFOCLR,0)))
#define TSB_UART1_FIFOCLR_TFCLR                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART1->FIFOCLR,1)))
#define TSB_UART1_ERR_BERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->ERR,0)))
#define TSB_UART1_ERR_FERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->ERR,1)))
#define TSB_UART1_ERR_PERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->ERR,2)))
#define TSB_UART1_ERR_OVRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->ERR,3)))
#define TSB_UART1_ERR_TRGERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART1->ERR,4)))

#define TSB_UART2_SWRST_SWRSTF                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART2->SWRST,7)))
#define TSB_UART2_CR0_PE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR0,2)))
#define TSB_UART2_CR0_EVEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR0,3)))
#define TSB_UART2_CR0_SBLEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR0,4)))
#define TSB_UART2_CR0_DIR                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR0,5)))
#define TSB_UART2_CR0_IV                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR0,6)))
#define TSB_UART2_CR0_WU                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR0,8)))
#define TSB_UART2_CR0_RTSE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR0,9)))
#define TSB_UART2_CR0_CTSE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR0,10)))
#define TSB_UART2_CR0_LPB                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR0,15)))
#define TSB_UART2_CR1_DMARE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR1,0)))
#define TSB_UART2_CR1_DMATE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR1,1)))
#define TSB_UART2_CR1_INTERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR1,2)))
#define TSB_UART2_CR1_INTRXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR1,4)))
#define TSB_UART2_CR1_INTRXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR1,5)))
#define TSB_UART2_CR1_INTTXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR1,6)))
#define TSB_UART2_CR1_INTTXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->CR1,7)))
#define TSB_UART2_BRD_KEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->BRD,23)))
#define TSB_UART2_TRANS_RXE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->TRANS,0)))
#define TSB_UART2_TRANS_TXE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->TRANS,1)))
#define TSB_UART2_TRANS_TXTRG                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->TRANS,2)))
#define TSB_UART2_TRANS_BK                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->TRANS,3)))
#define TSB_UART2_DR_BERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART2->DR,16)))
#define TSB_UART2_DR_FERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART2->DR,17)))
#define TSB_UART2_DR_PERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART2->DR,18)))
#define TSB_UART2_SR_RXFF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->SR,5)))
#define TSB_UART2_SR_RXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->SR,6)))
#define TSB_UART2_SR_RXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART2->SR,7)))
#define TSB_UART2_SR_TXFF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->SR,13)))
#define TSB_UART2_SR_TXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->SR,14)))
#define TSB_UART2_SR_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART2->SR,15)))
#define TSB_UART2_SR_SUE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART2->SR,31)))
#define TSB_UART2_FIFOCLR_RFCLR                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART2->FIFOCLR,0)))
#define TSB_UART2_FIFOCLR_TFCLR                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART2->FIFOCLR,1)))
#define TSB_UART2_ERR_BERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->ERR,0)))
#define TSB_UART2_ERR_FERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->ERR,1)))
#define TSB_UART2_ERR_PERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->ERR,2)))
#define TSB_UART2_ERR_OVRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->ERR,3)))
#define TSB_UART2_ERR_TRGERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART2->ERR,4)))

#define TSB_UART3_SWRST_SWRSTF                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART3->SWRST,7)))
#define TSB_UART3_CR0_PE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->CR0,2)))
#define TSB_UART3_CR0_EVEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->CR0,3)))
#define TSB_UART3_CR0_SBLEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->CR0,4)))
#define TSB_UART3_CR0_DIR                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->CR0,5)))
#define TSB_UART3_CR0_IV                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->CR0,6)))
#define TSB_UART3_CR0_WU                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->CR0,8)))
#define TSB_UART3_CR0_RTSE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->CR0,9)))
#define TSB_UART3_CR0_CTSE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->CR0,10)))
#define TSB_UART3_CR0_LPB                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->CR0,15)))
#define TSB_UART3_CR1_DMARE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->CR1,0)))
#define TSB_UART3_CR1_DMATE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->CR1,1)))
#define TSB_UART3_CR1_INTERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->CR1,2)))
#define TSB_UART3_CR1_INTRXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->CR1,4)))
#define TSB_UART3_CR1_INTRXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->CR1,5)))
#define TSB_UART3_CR1_INTTXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->CR1,6)))
#define TSB_UART3_CR1_INTTXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->CR1,7)))
#define TSB_UART3_BRD_KEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->BRD,23)))
#define TSB_UART3_TRANS_RXE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->TRANS,0)))
#define TSB_UART3_TRANS_TXE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->TRANS,1)))
#define TSB_UART3_TRANS_TXTRG                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->TRANS,2)))
#define TSB_UART3_TRANS_BK                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->TRANS,3)))
#define TSB_UART3_DR_BERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART3->DR,16)))
#define TSB_UART3_DR_FERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART3->DR,17)))
#define TSB_UART3_DR_PERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART3->DR,18)))
#define TSB_UART3_SR_RXFF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->SR,5)))
#define TSB_UART3_SR_RXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->SR,6)))
#define TSB_UART3_SR_RXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART3->SR,7)))
#define TSB_UART3_SR_TXFF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->SR,13)))
#define TSB_UART3_SR_TXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->SR,14)))
#define TSB_UART3_SR_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART3->SR,15)))
#define TSB_UART3_SR_SUE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART3->SR,31)))
#define TSB_UART3_FIFOCLR_RFCLR                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART3->FIFOCLR,0)))
#define TSB_UART3_FIFOCLR_TFCLR                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART3->FIFOCLR,1)))
#define TSB_UART3_ERR_BERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->ERR,0)))
#define TSB_UART3_ERR_FERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->ERR,1)))
#define TSB_UART3_ERR_PERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->ERR,2)))
#define TSB_UART3_ERR_OVRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->ERR,3)))
#define TSB_UART3_ERR_TRGERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART3->ERR,4)))

#define TSB_UART4_SWRST_SWRSTF                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART4->SWRST,7)))
#define TSB_UART4_CR0_PE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->CR0,2)))
#define TSB_UART4_CR0_EVEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->CR0,3)))
#define TSB_UART4_CR0_SBLEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->CR0,4)))
#define TSB_UART4_CR0_DIR                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->CR0,5)))
#define TSB_UART4_CR0_IV                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->CR0,6)))
#define TSB_UART4_CR0_WU                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->CR0,8)))
#define TSB_UART4_CR0_RTSE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->CR0,9)))
#define TSB_UART4_CR0_CTSE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->CR0,10)))
#define TSB_UART4_CR0_LPB                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->CR0,15)))
#define TSB_UART4_CR1_DMARE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->CR1,0)))
#define TSB_UART4_CR1_DMATE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->CR1,1)))
#define TSB_UART4_CR1_INTERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->CR1,2)))
#define TSB_UART4_CR1_INTRXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->CR1,4)))
#define TSB_UART4_CR1_INTRXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->CR1,5)))
#define TSB_UART4_CR1_INTTXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->CR1,6)))
#define TSB_UART4_CR1_INTTXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->CR1,7)))
#define TSB_UART4_BRD_KEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->BRD,23)))
#define TSB_UART4_TRANS_RXE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->TRANS,0)))
#define TSB_UART4_TRANS_TXE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->TRANS,1)))
#define TSB_UART4_TRANS_TXTRG                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->TRANS,2)))
#define TSB_UART4_TRANS_BK                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->TRANS,3)))
#define TSB_UART4_DR_BERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART4->DR,16)))
#define TSB_UART4_DR_FERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART4->DR,17)))
#define TSB_UART4_DR_PERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART4->DR,18)))
#define TSB_UART4_SR_RXFF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->SR,5)))
#define TSB_UART4_SR_RXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->SR,6)))
#define TSB_UART4_SR_RXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART4->SR,7)))
#define TSB_UART4_SR_TXFF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->SR,13)))
#define TSB_UART4_SR_TXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->SR,14)))
#define TSB_UART4_SR_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART4->SR,15)))
#define TSB_UART4_SR_SUE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART4->SR,31)))
#define TSB_UART4_FIFOCLR_RFCLR                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART4->FIFOCLR,0)))
#define TSB_UART4_FIFOCLR_TFCLR                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART4->FIFOCLR,1)))
#define TSB_UART4_ERR_BERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->ERR,0)))
#define TSB_UART4_ERR_FERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->ERR,1)))
#define TSB_UART4_ERR_PERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->ERR,2)))
#define TSB_UART4_ERR_OVRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->ERR,3)))
#define TSB_UART4_ERR_TRGERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART4->ERR,4)))

#define TSB_UART5_SWRST_SWRSTF                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART5->SWRST,7)))
#define TSB_UART5_CR0_PE                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->CR0,2)))
#define TSB_UART5_CR0_EVEN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->CR0,3)))
#define TSB_UART5_CR0_SBLEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->CR0,4)))
#define TSB_UART5_CR0_DIR                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->CR0,5)))
#define TSB_UART5_CR0_IV                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->CR0,6)))
#define TSB_UART5_CR0_WU                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->CR0,8)))
#define TSB_UART5_CR0_RTSE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->CR0,9)))
#define TSB_UART5_CR0_CTSE                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->CR0,10)))
#define TSB_UART5_CR0_LPB                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->CR0,15)))
#define TSB_UART5_CR1_DMARE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->CR1,0)))
#define TSB_UART5_CR1_DMATE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->CR1,1)))
#define TSB_UART5_CR1_INTERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->CR1,2)))
#define TSB_UART5_CR1_INTRXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->CR1,4)))
#define TSB_UART5_CR1_INTRXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->CR1,5)))
#define TSB_UART5_CR1_INTTXWE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->CR1,6)))
#define TSB_UART5_CR1_INTTXFE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->CR1,7)))
#define TSB_UART5_BRD_KEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->BRD,23)))
#define TSB_UART5_TRANS_RXE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->TRANS,0)))
#define TSB_UART5_TRANS_TXE                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->TRANS,1)))
#define TSB_UART5_TRANS_TXTRG                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->TRANS,2)))
#define TSB_UART5_TRANS_BK                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->TRANS,3)))
#define TSB_UART5_DR_BERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART5->DR,16)))
#define TSB_UART5_DR_FERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART5->DR,17)))
#define TSB_UART5_DR_PERR                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART5->DR,18)))
#define TSB_UART5_SR_RXFF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->SR,5)))
#define TSB_UART5_SR_RXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->SR,6)))
#define TSB_UART5_SR_RXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART5->SR,7)))
#define TSB_UART5_SR_TXFF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->SR,13)))
#define TSB_UART5_SR_TXEND                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->SR,14)))
#define TSB_UART5_SR_TXRUN                        (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART5->SR,15)))
#define TSB_UART5_SR_SUE                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_UART5->SR,31)))
#define TSB_UART5_FIFOCLR_RFCLR                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART5->FIFOCLR,0)))
#define TSB_UART5_FIFOCLR_TFCLR                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_UART5->FIFOCLR,1)))
#define TSB_UART5_ERR_BERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->ERR,0)))
#define TSB_UART5_ERR_FERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->ERR,1)))
#define TSB_UART5_ERR_PERR                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->ERR,2)))
#define TSB_UART5_ERR_OVRERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->ERR,3)))
#define TSB_UART5_ERR_TRGERR                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_UART5->ERR,4)))


/* I2C  */
#define TSB_I2C0_CR1_NOACK                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->CR1,3)))
#define TSB_I2C0_CR1_ACK                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->CR1,4)))
#define TSB_I2C0_AR_ALS                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->AR,0)))
#define TSB_I2C0_CR2_I2CM                         (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C0->CR2,3)))
#define TSB_I2C0_CR2_PIN                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C0->CR2,4)))
#define TSB_I2C0_CR2_BB                           (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C0->CR2,5)))
#define TSB_I2C0_CR2_TRX                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C0->CR2,6)))
#define TSB_I2C0_CR2_MST                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C0->CR2,7)))
#define TSB_I2C0_SR_LRB                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,0)))
#define TSB_I2C0_SR_ADO                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,1)))
#define TSB_I2C0_SR_AAS                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,2)))
#define TSB_I2C0_SR_AL                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,3)))
#define TSB_I2C0_SR_PIN                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,4)))
#define TSB_I2C0_SR_BB                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,5)))
#define TSB_I2C0_SR_TRX                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,6)))
#define TSB_I2C0_SR_MST                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->SR,7)))
#define TSB_I2C0_IE_INTI2C                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->IE,0)))
#define TSB_I2C0_IE_INTI2CAL                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->IE,1)))
#define TSB_I2C0_IE_INTI2CBF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->IE,2)))
#define TSB_I2C0_IE_INTNACK                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->IE,3)))
#define TSB_I2C0_IE_DMARI2CRX                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->IE,4)))
#define TSB_I2C0_IE_DMARI2CTX                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->IE,5)))
#define TSB_I2C0_IE_SELPINCD                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->IE,6)))
#define TSB_I2C0_ST_I2C                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->ST,0)))
#define TSB_I2C0_ST_I2CAL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->ST,1)))
#define TSB_I2C0_ST_I2CBF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->ST,2)))
#define TSB_I2C0_ST_NACK                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->ST,3)))
#define TSB_I2C0_OP_MFACK                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->OP,0)))
#define TSB_I2C0_OP_SREN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->OP,1)))
#define TSB_I2C0_OP_GCDI                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->OP,2)))
#define TSB_I2C0_OP_RSTA                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->OP,3)))
#define TSB_I2C0_OP_NFSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->OP,4)))
#define TSB_I2C0_OP_SAST                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->OP,5)))
#define TSB_I2C0_OP_SA2ST                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->OP,6)))
#define TSB_I2C0_OP_DISAL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->OP,7)))
#define TSB_I2C0_PM_SCL                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->PM,0)))
#define TSB_I2C0_PM_SDA                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C0->PM,1)))
#define TSB_I2C0_AR2_SA2EN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C0->AR2,0)))

#define TSB_I2C1_CR1_NOACK                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->CR1,3)))
#define TSB_I2C1_CR1_ACK                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->CR1,4)))
#define TSB_I2C1_AR_ALS                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->AR,0)))
#define TSB_I2C1_CR2_I2CM                         (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C1->CR2,3)))
#define TSB_I2C1_CR2_PIN                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C1->CR2,4)))
#define TSB_I2C1_CR2_BB                           (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C1->CR2,5)))
#define TSB_I2C1_CR2_TRX                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C1->CR2,6)))
#define TSB_I2C1_CR2_MST                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C1->CR2,7)))
#define TSB_I2C1_SR_LRB                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,0)))
#define TSB_I2C1_SR_ADO                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,1)))
#define TSB_I2C1_SR_AAS                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,2)))
#define TSB_I2C1_SR_AL                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,3)))
#define TSB_I2C1_SR_PIN                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,4)))
#define TSB_I2C1_SR_BB                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,5)))
#define TSB_I2C1_SR_TRX                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,6)))
#define TSB_I2C1_SR_MST                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->SR,7)))
#define TSB_I2C1_IE_INTI2C                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->IE,0)))
#define TSB_I2C1_IE_INTI2CAL                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->IE,1)))
#define TSB_I2C1_IE_INTI2CBF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->IE,2)))
#define TSB_I2C1_IE_INTNACK                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->IE,3)))
#define TSB_I2C1_IE_DMARI2CRX                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->IE,4)))
#define TSB_I2C1_IE_DMARI2CTX                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->IE,5)))
#define TSB_I2C1_IE_SELPINCD                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->IE,6)))
#define TSB_I2C1_ST_I2C                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->ST,0)))
#define TSB_I2C1_ST_I2CAL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->ST,1)))
#define TSB_I2C1_ST_I2CBF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->ST,2)))
#define TSB_I2C1_ST_NACK                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->ST,3)))
#define TSB_I2C1_OP_MFACK                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->OP,0)))
#define TSB_I2C1_OP_SREN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->OP,1)))
#define TSB_I2C1_OP_GCDI                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->OP,2)))
#define TSB_I2C1_OP_RSTA                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->OP,3)))
#define TSB_I2C1_OP_NFSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->OP,4)))
#define TSB_I2C1_OP_SAST                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->OP,5)))
#define TSB_I2C1_OP_SA2ST                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->OP,6)))
#define TSB_I2C1_OP_DISAL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->OP,7)))
#define TSB_I2C1_PM_SCL                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->PM,0)))
#define TSB_I2C1_PM_SDA                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C1->PM,1)))
#define TSB_I2C1_AR2_SA2EN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C1->AR2,0)))

#define TSB_I2C2_CR1_NOACK                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->CR1,3)))
#define TSB_I2C2_CR1_ACK                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->CR1,4)))
#define TSB_I2C2_AR_ALS                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->AR,0)))
#define TSB_I2C2_CR2_I2CM                         (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C2->CR2,3)))
#define TSB_I2C2_CR2_PIN                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C2->CR2,4)))
#define TSB_I2C2_CR2_BB                           (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C2->CR2,5)))
#define TSB_I2C2_CR2_TRX                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C2->CR2,6)))
#define TSB_I2C2_CR2_MST                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C2->CR2,7)))
#define TSB_I2C2_SR_LRB                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,0)))
#define TSB_I2C2_SR_ADO                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,1)))
#define TSB_I2C2_SR_AAS                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,2)))
#define TSB_I2C2_SR_AL                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,3)))
#define TSB_I2C2_SR_PIN                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,4)))
#define TSB_I2C2_SR_BB                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,5)))
#define TSB_I2C2_SR_TRX                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,6)))
#define TSB_I2C2_SR_MST                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->SR,7)))
#define TSB_I2C2_IE_INTI2C                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->IE,0)))
#define TSB_I2C2_IE_INTI2CAL                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->IE,1)))
#define TSB_I2C2_IE_INTI2CBF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->IE,2)))
#define TSB_I2C2_IE_INTNACK                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->IE,3)))
#define TSB_I2C2_IE_DMARI2CRX                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->IE,4)))
#define TSB_I2C2_IE_DMARI2CTX                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->IE,5)))
#define TSB_I2C2_IE_SELPINCD                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->IE,6)))
#define TSB_I2C2_ST_I2C                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->ST,0)))
#define TSB_I2C2_ST_I2CAL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->ST,1)))
#define TSB_I2C2_ST_I2CBF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->ST,2)))
#define TSB_I2C2_ST_NACK                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->ST,3)))
#define TSB_I2C2_OP_MFACK                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->OP,0)))
#define TSB_I2C2_OP_SREN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->OP,1)))
#define TSB_I2C2_OP_GCDI                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->OP,2)))
#define TSB_I2C2_OP_RSTA                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->OP,3)))
#define TSB_I2C2_OP_NFSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->OP,4)))
#define TSB_I2C2_OP_SAST                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->OP,5)))
#define TSB_I2C2_OP_SA2ST                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->OP,6)))
#define TSB_I2C2_OP_DISAL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->OP,7)))
#define TSB_I2C2_PM_SCL                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->PM,0)))
#define TSB_I2C2_PM_SDA                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C2->PM,1)))
#define TSB_I2C2_AR2_SA2EN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C2->AR2,0)))

#define TSB_I2C3_CR1_NOACK                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C3->CR1,3)))
#define TSB_I2C3_CR1_ACK                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C3->CR1,4)))
#define TSB_I2C3_AR_ALS                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C3->AR,0)))
#define TSB_I2C3_CR2_I2CM                         (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C3->CR2,3)))
#define TSB_I2C3_CR2_PIN                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C3->CR2,4)))
#define TSB_I2C3_CR2_BB                           (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C3->CR2,5)))
#define TSB_I2C3_CR2_TRX                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C3->CR2,6)))
#define TSB_I2C3_CR2_MST                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C3->CR2,7)))
#define TSB_I2C3_SR_LRB                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C3->SR,0)))
#define TSB_I2C3_SR_ADO                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C3->SR,1)))
#define TSB_I2C3_SR_AAS                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C3->SR,2)))
#define TSB_I2C3_SR_AL                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C3->SR,3)))
#define TSB_I2C3_SR_PIN                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C3->SR,4)))
#define TSB_I2C3_SR_BB                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C3->SR,5)))
#define TSB_I2C3_SR_TRX                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C3->SR,6)))
#define TSB_I2C3_SR_MST                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C3->SR,7)))
#define TSB_I2C3_IE_INTI2C                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C3->IE,0)))
#define TSB_I2C3_IE_INTI2CAL                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C3->IE,1)))
#define TSB_I2C3_IE_INTI2CBF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C3->IE,2)))
#define TSB_I2C3_IE_INTNACK                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C3->IE,3)))
#define TSB_I2C3_IE_DMARI2CRX                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C3->IE,4)))
#define TSB_I2C3_IE_DMARI2CTX                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C3->IE,5)))
#define TSB_I2C3_IE_SELPINCD                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C3->IE,6)))
#define TSB_I2C3_ST_I2C                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C3->ST,0)))
#define TSB_I2C3_ST_I2CAL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C3->ST,1)))
#define TSB_I2C3_ST_I2CBF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C3->ST,2)))
#define TSB_I2C3_ST_NACK                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C3->ST,3)))
#define TSB_I2C3_OP_MFACK                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C3->OP,0)))
#define TSB_I2C3_OP_SREN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C3->OP,1)))
#define TSB_I2C3_OP_GCDI                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C3->OP,2)))
#define TSB_I2C3_OP_RSTA                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C3->OP,3)))
#define TSB_I2C3_OP_NFSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C3->OP,4)))
#define TSB_I2C3_OP_SAST                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C3->OP,5)))
#define TSB_I2C3_OP_SA2ST                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C3->OP,6)))
#define TSB_I2C3_OP_DISAL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C3->OP,7)))
#define TSB_I2C3_PM_SCL                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C3->PM,0)))
#define TSB_I2C3_PM_SDA                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C3->PM,1)))
#define TSB_I2C3_AR2_SA2EN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C3->AR2,0)))

#define TSB_I2C4_CR1_NOACK                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C4->CR1,3)))
#define TSB_I2C4_CR1_ACK                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C4->CR1,4)))
#define TSB_I2C4_AR_ALS                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C4->AR,0)))
#define TSB_I2C4_CR2_I2CM                         (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C4->CR2,3)))
#define TSB_I2C4_CR2_PIN                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C4->CR2,4)))
#define TSB_I2C4_CR2_BB                           (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C4->CR2,5)))
#define TSB_I2C4_CR2_TRX                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C4->CR2,6)))
#define TSB_I2C4_CR2_MST                          (*((__O  uint32_t *)BITBAND_PERI(&TSB_I2C4->CR2,7)))
#define TSB_I2C4_SR_LRB                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C4->SR,0)))
#define TSB_I2C4_SR_ADO                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C4->SR,1)))
#define TSB_I2C4_SR_AAS                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C4->SR,2)))
#define TSB_I2C4_SR_AL                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C4->SR,3)))
#define TSB_I2C4_SR_PIN                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C4->SR,4)))
#define TSB_I2C4_SR_BB                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C4->SR,5)))
#define TSB_I2C4_SR_TRX                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C4->SR,6)))
#define TSB_I2C4_SR_MST                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C4->SR,7)))
#define TSB_I2C4_IE_INTI2C                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C4->IE,0)))
#define TSB_I2C4_IE_INTI2CAL                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C4->IE,1)))
#define TSB_I2C4_IE_INTI2CBF                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C4->IE,2)))
#define TSB_I2C4_IE_INTNACK                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C4->IE,3)))
#define TSB_I2C4_IE_DMARI2CRX                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C4->IE,4)))
#define TSB_I2C4_IE_DMARI2CTX                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C4->IE,5)))
#define TSB_I2C4_IE_SELPINCD                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C4->IE,6)))
#define TSB_I2C4_ST_I2C                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C4->ST,0)))
#define TSB_I2C4_ST_I2CAL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C4->ST,1)))
#define TSB_I2C4_ST_I2CBF                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C4->ST,2)))
#define TSB_I2C4_ST_NACK                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C4->ST,3)))
#define TSB_I2C4_OP_MFACK                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C4->OP,0)))
#define TSB_I2C4_OP_SREN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C4->OP,1)))
#define TSB_I2C4_OP_GCDI                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C4->OP,2)))
#define TSB_I2C4_OP_RSTA                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C4->OP,3)))
#define TSB_I2C4_OP_NFSEL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C4->OP,4)))
#define TSB_I2C4_OP_SAST                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C4->OP,5)))
#define TSB_I2C4_OP_SA2ST                         (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C4->OP,6)))
#define TSB_I2C4_OP_DISAL                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C4->OP,7)))
#define TSB_I2C4_PM_SCL                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C4->PM,0)))
#define TSB_I2C4_PM_SDA                           (*((__I  uint32_t *)BITBAND_PERI(&TSB_I2C4->PM,1)))
#define TSB_I2C4_AR2_SA2EN                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_I2C4->AR2,0)))


/* Port A */
#define TSB_PA_DATA_PA0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,0)))
#define TSB_PA_DATA_PA1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,1)))
#define TSB_PA_DATA_PA2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,2)))
#define TSB_PA_DATA_PA3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,3)))
#define TSB_PA_DATA_PA4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,4)))
#define TSB_PA_DATA_PA5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,5)))
#define TSB_PA_DATA_PA6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,6)))
#define TSB_PA_DATA_PA7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->DATA,7)))
#define TSB_PA_CR_PA0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,0)))
#define TSB_PA_CR_PA1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,1)))
#define TSB_PA_CR_PA2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,2)))
#define TSB_PA_CR_PA3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,3)))
#define TSB_PA_CR_PA4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,4)))
#define TSB_PA_CR_PA5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,5)))
#define TSB_PA_CR_PA6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,6)))
#define TSB_PA_CR_PA7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->CR,7)))
#define TSB_PA_FR1_PA0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,0)))
#define TSB_PA_FR1_PA1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,1)))
#define TSB_PA_FR1_PA2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,2)))
#define TSB_PA_FR1_PA3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,3)))
#define TSB_PA_FR1_PA4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,4)))
#define TSB_PA_FR1_PA5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,5)))
#define TSB_PA_FR1_PA6F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,6)))
#define TSB_PA_FR1_PA7F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR1,7)))
#define TSB_PA_FR2_PA0F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR2,0)))
#define TSB_PA_FR2_PA3F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR2,3)))
#define TSB_PA_FR2_PA4F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR2,4)))
#define TSB_PA_FR2_PA7F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR2,7)))
#define TSB_PA_FR3_PA0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR3,0)))
#define TSB_PA_FR3_PA1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR3,1)))
#define TSB_PA_FR3_PA2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR3,2)))
#define TSB_PA_FR3_PA3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR3,3)))
#define TSB_PA_FR3_PA4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR3,4)))
#define TSB_PA_FR3_PA5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR3,5)))
#define TSB_PA_FR3_PA6F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR3,6)))
#define TSB_PA_FR3_PA7F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR3,7)))
#define TSB_PA_FR5_PA0F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR5,0)))
#define TSB_PA_FR5_PA1F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR5,1)))
#define TSB_PA_FR5_PA3F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR5,3)))
#define TSB_PA_FR5_PA4F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR5,4)))
#define TSB_PA_FR5_PA5F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR5,5)))
#define TSB_PA_FR5_PA7F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR5,7)))
#define TSB_PA_FR6_PA0F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR6,0)))
#define TSB_PA_FR6_PA3F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR6,3)))
#define TSB_PA_FR6_PA4F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR6,4)))
#define TSB_PA_FR6_PA5F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR6,5)))
#define TSB_PA_FR6_PA6F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR6,6)))
#define TSB_PA_FR6_PA7F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR6,7)))
#define TSB_PA_FR7_PA0F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR7,0)))
#define TSB_PA_FR7_PA1F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR7,1)))
#define TSB_PA_FR7_PA2F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR7,2)))
#define TSB_PA_FR7_PA3F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR7,3)))
#define TSB_PA_FR7_PA4F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR7,4)))
#define TSB_PA_FR7_PA5F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR7,5)))
#define TSB_PA_FR7_PA6F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR7,6)))
#define TSB_PA_FR7_PA7F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->FR7,7)))
#define TSB_PA_OD_PA0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,0)))
#define TSB_PA_OD_PA1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,1)))
#define TSB_PA_OD_PA2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,2)))
#define TSB_PA_OD_PA3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,3)))
#define TSB_PA_OD_PA4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,4)))
#define TSB_PA_OD_PA5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,5)))
#define TSB_PA_OD_PA6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,6)))
#define TSB_PA_OD_PA7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->OD,7)))
#define TSB_PA_PUP_PA0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,0)))
#define TSB_PA_PUP_PA1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,1)))
#define TSB_PA_PUP_PA2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,2)))
#define TSB_PA_PUP_PA3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,3)))
#define TSB_PA_PUP_PA4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,4)))
#define TSB_PA_PUP_PA5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,5)))
#define TSB_PA_PUP_PA6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,6)))
#define TSB_PA_PUP_PA7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PUP,7)))
#define TSB_PA_PDN_PA0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PDN,0)))
#define TSB_PA_PDN_PA1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PDN,1)))
#define TSB_PA_PDN_PA2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PDN,2)))
#define TSB_PA_PDN_PA3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PDN,3)))
#define TSB_PA_PDN_PA4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PDN,4)))
#define TSB_PA_PDN_PA5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PDN,5)))
#define TSB_PA_PDN_PA6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PDN,6)))
#define TSB_PA_PDN_PA7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->PDN,7)))
#define TSB_PA_IE_PA0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,0)))
#define TSB_PA_IE_PA1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,1)))
#define TSB_PA_IE_PA2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,2)))
#define TSB_PA_IE_PA3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,3)))
#define TSB_PA_IE_PA4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,4)))
#define TSB_PA_IE_PA5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,5)))
#define TSB_PA_IE_PA6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,6)))
#define TSB_PA_IE_PA7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PA->IE,7)))


/* Port B */
#define TSB_PB_DATA_PB0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,0)))
#define TSB_PB_DATA_PB1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,1)))
#define TSB_PB_DATA_PB2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,2)))
#define TSB_PB_DATA_PB3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,3)))
#define TSB_PB_DATA_PB4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,4)))
#define TSB_PB_DATA_PB5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,5)))
#define TSB_PB_DATA_PB6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,6)))
#define TSB_PB_DATA_PB7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->DATA,7)))
#define TSB_PB_CR_PB0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,0)))
#define TSB_PB_CR_PB1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,1)))
#define TSB_PB_CR_PB2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,2)))
#define TSB_PB_CR_PB3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,3)))
#define TSB_PB_CR_PB4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,4)))
#define TSB_PB_CR_PB5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,5)))
#define TSB_PB_CR_PB6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,6)))
#define TSB_PB_CR_PB7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->CR,7)))
#define TSB_PB_FR1_PB1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,1)))
#define TSB_PB_FR1_PB2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,2)))
#define TSB_PB_FR1_PB3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,3)))
#define TSB_PB_FR1_PB4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,4)))
#define TSB_PB_FR1_PB5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,5)))
#define TSB_PB_FR1_PB6F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,6)))
#define TSB_PB_FR1_PB7F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR1,7)))
#define TSB_PB_FR2_PB0F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR2,0)))
#define TSB_PB_FR2_PB1F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR2,1)))
#define TSB_PB_FR2_PB6F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR2,6)))
#define TSB_PB_FR2_PB7F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR2,7)))
#define TSB_PB_FR3_PB0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,0)))
#define TSB_PB_FR3_PB1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,1)))
#define TSB_PB_FR3_PB2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,2)))
#define TSB_PB_FR3_PB3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,3)))
#define TSB_PB_FR3_PB4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,4)))
#define TSB_PB_FR3_PB5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,5)))
#define TSB_PB_FR3_PB6F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,6)))
#define TSB_PB_FR3_PB7F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR3,7)))
#define TSB_PB_FR5_PB0F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR5,0)))
#define TSB_PB_FR5_PB1F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR5,1)))
#define TSB_PB_FR5_PB2F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR5,2)))
#define TSB_PB_FR5_PB4F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR5,4)))
#define TSB_PB_FR5_PB6F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR5,6)))
#define TSB_PB_FR5_PB7F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR5,7)))
#define TSB_PB_FR6_PB1F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->FR6,1)))
#define TSB_PB_OD_PB0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,0)))
#define TSB_PB_OD_PB1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,1)))
#define TSB_PB_OD_PB2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,2)))
#define TSB_PB_OD_PB3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,3)))
#define TSB_PB_OD_PB4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,4)))
#define TSB_PB_OD_PB5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,5)))
#define TSB_PB_OD_PB6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,6)))
#define TSB_PB_OD_PB7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->OD,7)))
#define TSB_PB_PUP_PB0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,0)))
#define TSB_PB_PUP_PB1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,1)))
#define TSB_PB_PUP_PB2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,2)))
#define TSB_PB_PUP_PB3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,3)))
#define TSB_PB_PUP_PB4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,4)))
#define TSB_PB_PUP_PB5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,5)))
#define TSB_PB_PUP_PB6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,6)))
#define TSB_PB_PUP_PB7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PUP,7)))
#define TSB_PB_PDN_PB0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PDN,0)))
#define TSB_PB_PDN_PB1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PDN,1)))
#define TSB_PB_PDN_PB2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PDN,2)))
#define TSB_PB_PDN_PB3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PDN,3)))
#define TSB_PB_PDN_PB4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PDN,4)))
#define TSB_PB_PDN_PB5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PDN,5)))
#define TSB_PB_PDN_PB6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PDN,6)))
#define TSB_PB_PDN_PB7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->PDN,7)))
#define TSB_PB_IE_PB0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,0)))
#define TSB_PB_IE_PB1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,1)))
#define TSB_PB_IE_PB2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,2)))
#define TSB_PB_IE_PB3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,3)))
#define TSB_PB_IE_PB4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,4)))
#define TSB_PB_IE_PB5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,5)))
#define TSB_PB_IE_PB6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,6)))
#define TSB_PB_IE_PB7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PB->IE,7)))


/* Port C */
#define TSB_PC_DATA_PC0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,0)))
#define TSB_PC_DATA_PC1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,1)))
#define TSB_PC_DATA_PC2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,2)))
#define TSB_PC_DATA_PC3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,3)))
#define TSB_PC_DATA_PC4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,4)))
#define TSB_PC_DATA_PC5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,5)))
#define TSB_PC_DATA_PC6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,6)))
#define TSB_PC_DATA_PC7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->DATA,7)))
#define TSB_PC_CR_PC0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,0)))
#define TSB_PC_CR_PC1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,1)))
#define TSB_PC_CR_PC2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,2)))
#define TSB_PC_CR_PC3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,3)))
#define TSB_PC_CR_PC4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,4)))
#define TSB_PC_CR_PC5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,5)))
#define TSB_PC_CR_PC6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,6)))
#define TSB_PC_CR_PC7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->CR,7)))
#define TSB_PC_FR1_PC1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,1)))
#define TSB_PC_FR1_PC2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,2)))
#define TSB_PC_FR1_PC3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,3)))
#define TSB_PC_FR1_PC4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,4)))
#define TSB_PC_FR1_PC5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,5)))
#define TSB_PC_FR1_PC6F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,6)))
#define TSB_PC_FR1_PC7F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR1,7)))
#define TSB_PC_FR3_PC1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR3,1)))
#define TSB_PC_FR3_PC2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR3,2)))
#define TSB_PC_FR3_PC3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR3,3)))
#define TSB_PC_FR3_PC4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR3,4)))
#define TSB_PC_FR3_PC5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR3,5)))
#define TSB_PC_FR5_PC1F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR5,1)))
#define TSB_PC_FR5_PC2F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR5,2)))
#define TSB_PC_FR5_PC4F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->FR5,4)))
#define TSB_PC_OD_PC0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,0)))
#define TSB_PC_OD_PC1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,1)))
#define TSB_PC_OD_PC2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,2)))
#define TSB_PC_OD_PC3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,3)))
#define TSB_PC_OD_PC4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,4)))
#define TSB_PC_OD_PC5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,5)))
#define TSB_PC_OD_PC6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,6)))
#define TSB_PC_OD_PC7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->OD,7)))
#define TSB_PC_PUP_PC0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,0)))
#define TSB_PC_PUP_PC1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,1)))
#define TSB_PC_PUP_PC2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,2)))
#define TSB_PC_PUP_PC3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,3)))
#define TSB_PC_PUP_PC4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,4)))
#define TSB_PC_PUP_PC5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,5)))
#define TSB_PC_PUP_PC6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,6)))
#define TSB_PC_PUP_PC7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PUP,7)))
#define TSB_PC_PDN_PC0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PDN,0)))
#define TSB_PC_PDN_PC1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PDN,1)))
#define TSB_PC_PDN_PC2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PDN,2)))
#define TSB_PC_PDN_PC3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PDN,3)))
#define TSB_PC_PDN_PC4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PDN,4)))
#define TSB_PC_PDN_PC5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PDN,5)))
#define TSB_PC_PDN_PC6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PDN,6)))
#define TSB_PC_PDN_PC7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->PDN,7)))
#define TSB_PC_IE_PC0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,0)))
#define TSB_PC_IE_PC1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,1)))
#define TSB_PC_IE_PC2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,2)))
#define TSB_PC_IE_PC3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,3)))
#define TSB_PC_IE_PC4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,4)))
#define TSB_PC_IE_PC5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,5)))
#define TSB_PC_IE_PC6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,6)))
#define TSB_PC_IE_PC7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PC->IE,7)))


/* Port D */
#define TSB_PD_DATA_PD0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,0)))
#define TSB_PD_DATA_PD1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,1)))
#define TSB_PD_DATA_PD2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,2)))
#define TSB_PD_DATA_PD3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,3)))
#define TSB_PD_DATA_PD4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,4)))
#define TSB_PD_DATA_PD5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,5)))
#define TSB_PD_DATA_PD6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,6)))
#define TSB_PD_DATA_PD7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->DATA,7)))
#define TSB_PD_CR_PD0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,0)))
#define TSB_PD_CR_PD1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,1)))
#define TSB_PD_CR_PD2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,2)))
#define TSB_PD_CR_PD3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,3)))
#define TSB_PD_CR_PD4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,4)))
#define TSB_PD_CR_PD5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,5)))
#define TSB_PD_CR_PD6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,6)))
#define TSB_PD_CR_PD7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->CR,7)))
#define TSB_PD_FR1_PD1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR1,1)))
#define TSB_PD_FR1_PD2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR1,2)))
#define TSB_PD_FR1_PD3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR1,3)))
#define TSB_PD_FR1_PD4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR1,4)))
#define TSB_PD_FR1_PD5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR1,5)))
#define TSB_PD_FR1_PD6F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR1,6)))
#define TSB_PD_FR1_PD7F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR1,7)))
#define TSB_PD_FR2_PD0F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR2,0)))
#define TSB_PD_FR2_PD1F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR2,1)))
#define TSB_PD_FR2_PD6F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR2,6)))
#define TSB_PD_FR2_PD7F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR2,7)))
#define TSB_PD_FR3_PD0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR3,0)))
#define TSB_PD_FR3_PD1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR3,1)))
#define TSB_PD_FR3_PD2F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR3,2)))
#define TSB_PD_FR3_PD3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR3,3)))
#define TSB_PD_FR3_PD4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR3,4)))
#define TSB_PD_FR3_PD5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR3,5)))
#define TSB_PD_FR3_PD6F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR3,6)))
#define TSB_PD_FR3_PD7F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR3,7)))
#define TSB_PD_FR4_PD0F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR4,0)))
#define TSB_PD_FR4_PD1F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR4,1)))
#define TSB_PD_FR4_PD2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR4,2)))
#define TSB_PD_FR4_PD3F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR4,3)))
#define TSB_PD_FR5_PD0F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR5,0)))
#define TSB_PD_FR5_PD1F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR5,1)))
#define TSB_PD_FR5_PD2F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR5,2)))
#define TSB_PD_FR5_PD4F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR5,4)))
#define TSB_PD_FR5_PD6F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR5,6)))
#define TSB_PD_FR5_PD7F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR5,7)))
#define TSB_PD_FR6_PD0F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR6,0)))
#define TSB_PD_FR7_PD0F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR7,0)))
#define TSB_PD_FR7_PD1F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR7,1)))
#define TSB_PD_FR7_PD2F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR7,2)))
#define TSB_PD_FR7_PD3F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR7,3)))
#define TSB_PD_FR7_PD4F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR7,4)))
#define TSB_PD_FR7_PD5F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR7,5)))
#define TSB_PD_FR7_PD6F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR7,6)))
#define TSB_PD_FR7_PD7F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->FR7,7)))
#define TSB_PD_OD_PD0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,0)))
#define TSB_PD_OD_PD1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,1)))
#define TSB_PD_OD_PD2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,2)))
#define TSB_PD_OD_PD3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,3)))
#define TSB_PD_OD_PD4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,4)))
#define TSB_PD_OD_PD5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,5)))
#define TSB_PD_OD_PD6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,6)))
#define TSB_PD_OD_PD7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->OD,7)))
#define TSB_PD_PUP_PD0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,0)))
#define TSB_PD_PUP_PD1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,1)))
#define TSB_PD_PUP_PD2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,2)))
#define TSB_PD_PUP_PD3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,3)))
#define TSB_PD_PUP_PD4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,4)))
#define TSB_PD_PUP_PD5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,5)))
#define TSB_PD_PUP_PD6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,6)))
#define TSB_PD_PUP_PD7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PUP,7)))
#define TSB_PD_PDN_PD0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PDN,0)))
#define TSB_PD_PDN_PD1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PDN,1)))
#define TSB_PD_PDN_PD2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PDN,2)))
#define TSB_PD_PDN_PD3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PDN,3)))
#define TSB_PD_PDN_PD4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PDN,4)))
#define TSB_PD_PDN_PD5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PDN,5)))
#define TSB_PD_PDN_PD6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PDN,6)))
#define TSB_PD_PDN_PD7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->PDN,7)))
#define TSB_PD_IE_PD0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,0)))
#define TSB_PD_IE_PD1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,1)))
#define TSB_PD_IE_PD2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,2)))
#define TSB_PD_IE_PD3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,3)))
#define TSB_PD_IE_PD4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,4)))
#define TSB_PD_IE_PD5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,5)))
#define TSB_PD_IE_PD6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,6)))
#define TSB_PD_IE_PD7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PD->IE,7)))


/* Port E */
#define TSB_PE_DATA_PE0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,0)))
#define TSB_PE_DATA_PE1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,1)))
#define TSB_PE_DATA_PE2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,2)))
#define TSB_PE_DATA_PE3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,3)))
#define TSB_PE_DATA_PE4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,4)))
#define TSB_PE_DATA_PE5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,5)))
#define TSB_PE_DATA_PE6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,6)))
#define TSB_PE_DATA_PE7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->DATA,7)))
#define TSB_PE_CR_PE0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,0)))
#define TSB_PE_CR_PE1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,1)))
#define TSB_PE_CR_PE2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,2)))
#define TSB_PE_CR_PE3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,3)))
#define TSB_PE_CR_PE4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,4)))
#define TSB_PE_CR_PE5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,5)))
#define TSB_PE_CR_PE6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,6)))
#define TSB_PE_CR_PE7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->CR,7)))
#define TSB_PE_FR1_PE0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,0)))
#define TSB_PE_FR1_PE1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,1)))
#define TSB_PE_FR1_PE2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,2)))
#define TSB_PE_FR1_PE3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,3)))
#define TSB_PE_FR1_PE4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,4)))
#define TSB_PE_FR1_PE5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,5)))
#define TSB_PE_FR1_PE6F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,6)))
#define TSB_PE_FR1_PE7F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR1,7)))
#define TSB_PE_FR2_PE0F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR2,0)))
#define TSB_PE_FR2_PE7F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR2,7)))
#define TSB_PE_FR3_PE0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR3,0)))
#define TSB_PE_FR3_PE1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR3,1)))
#define TSB_PE_FR3_PE2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR3,2)))
#define TSB_PE_FR3_PE3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR3,3)))
#define TSB_PE_FR3_PE4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR3,4)))
#define TSB_PE_FR3_PE5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR3,5)))
#define TSB_PE_FR3_PE6F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR3,6)))
#define TSB_PE_FR3_PE7F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR3,7)))
#define TSB_PE_FR4_PE0F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR4,0)))
#define TSB_PE_FR4_PE1F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR4,1)))
#define TSB_PE_FR4_PE2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR4,2)))
#define TSB_PE_FR4_PE3F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR4,3)))
#define TSB_PE_FR4_PE4F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR4,4)))
#define TSB_PE_FR4_PE5F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR4,5)))
#define TSB_PE_FR4_PE6F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR4,6)))
#define TSB_PE_FR4_PE7F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR4,7)))
#define TSB_PE_FR5_PE0F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR5,0)))
#define TSB_PE_FR5_PE1F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR5,1)))
#define TSB_PE_FR5_PE2F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR5,2)))
#define TSB_PE_FR5_PE3F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR5,3)))
#define TSB_PE_FR5_PE4F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR5,4)))
#define TSB_PE_FR5_PE5F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR5,5)))
#define TSB_PE_FR5_PE6F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR5,6)))
#define TSB_PE_FR5_PE7F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR5,7)))
#define TSB_PE_FR7_PE0F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR7,0)))
#define TSB_PE_FR7_PE1F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR7,1)))
#define TSB_PE_FR7_PE2F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR7,2)))
#define TSB_PE_FR7_PE3F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->FR7,3)))
#define TSB_PE_OD_PE0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,0)))
#define TSB_PE_OD_PE1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,1)))
#define TSB_PE_OD_PE2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,2)))
#define TSB_PE_OD_PE3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,3)))
#define TSB_PE_OD_PE4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,4)))
#define TSB_PE_OD_PE5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,5)))
#define TSB_PE_OD_PE6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,6)))
#define TSB_PE_OD_PE7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->OD,7)))
#define TSB_PE_PUP_PE0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,0)))
#define TSB_PE_PUP_PE1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,1)))
#define TSB_PE_PUP_PE2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,2)))
#define TSB_PE_PUP_PE3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,3)))
#define TSB_PE_PUP_PE4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,4)))
#define TSB_PE_PUP_PE5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,5)))
#define TSB_PE_PUP_PE6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,6)))
#define TSB_PE_PUP_PE7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PUP,7)))
#define TSB_PE_PDN_PE0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PDN,0)))
#define TSB_PE_PDN_PE1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PDN,1)))
#define TSB_PE_PDN_PE2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PDN,2)))
#define TSB_PE_PDN_PE3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PDN,3)))
#define TSB_PE_PDN_PE4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PDN,4)))
#define TSB_PE_PDN_PE5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PDN,5)))
#define TSB_PE_PDN_PE6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PDN,6)))
#define TSB_PE_PDN_PE7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->PDN,7)))
#define TSB_PE_IE_PE0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,0)))
#define TSB_PE_IE_PE1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,1)))
#define TSB_PE_IE_PE2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,2)))
#define TSB_PE_IE_PE3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,3)))
#define TSB_PE_IE_PE4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,4)))
#define TSB_PE_IE_PE5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,5)))
#define TSB_PE_IE_PE6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,6)))
#define TSB_PE_IE_PE7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PE->IE,7)))


/* Port F */
#define TSB_PF_DATA_PF0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,0)))
#define TSB_PF_DATA_PF1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,1)))
#define TSB_PF_DATA_PF2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,2)))
#define TSB_PF_DATA_PF3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,3)))
#define TSB_PF_DATA_PF4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,4)))
#define TSB_PF_DATA_PF5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,5)))
#define TSB_PF_DATA_PF6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,6)))
#define TSB_PF_DATA_PF7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->DATA,7)))
#define TSB_PF_CR_PF0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,0)))
#define TSB_PF_CR_PF1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,1)))
#define TSB_PF_CR_PF2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,2)))
#define TSB_PF_CR_PF3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,3)))
#define TSB_PF_CR_PF4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,4)))
#define TSB_PF_CR_PF5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,5)))
#define TSB_PF_CR_PF6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,6)))
#define TSB_PF_CR_PF7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->CR,7)))
#define TSB_PF_FR1_PF0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,0)))
#define TSB_PF_FR1_PF1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,1)))
#define TSB_PF_FR1_PF4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,4)))
#define TSB_PF_FR1_PF5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,5)))
#define TSB_PF_FR1_PF6F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,6)))
#define TSB_PF_FR1_PF7F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR1,7)))
#define TSB_PF_FR7_PF2F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR7,2)))
#define TSB_PF_FR7_PF3F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->FR7,3)))
#define TSB_PF_OD_PF0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,0)))
#define TSB_PF_OD_PF1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,1)))
#define TSB_PF_OD_PF2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,2)))
#define TSB_PF_OD_PF3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,3)))
#define TSB_PF_OD_PF4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,4)))
#define TSB_PF_OD_PF5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,5)))
#define TSB_PF_OD_PF6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,6)))
#define TSB_PF_OD_PF7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->OD,7)))
#define TSB_PF_PUP_PF0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,0)))
#define TSB_PF_PUP_PF1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,1)))
#define TSB_PF_PUP_PF2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,2)))
#define TSB_PF_PUP_PF3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,3)))
#define TSB_PF_PUP_PF4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,4)))
#define TSB_PF_PUP_PF5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,5)))
#define TSB_PF_PUP_PF6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,6)))
#define TSB_PF_PUP_PF7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PUP,7)))
#define TSB_PF_PDN_PF0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PDN,0)))
#define TSB_PF_PDN_PF1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PDN,1)))
#define TSB_PF_PDN_PF2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PDN,2)))
#define TSB_PF_PDN_PF3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PDN,3)))
#define TSB_PF_PDN_PF4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PDN,4)))
#define TSB_PF_PDN_PF5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PDN,5)))
#define TSB_PF_PDN_PF6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PDN,6)))
#define TSB_PF_PDN_PF7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->PDN,7)))
#define TSB_PF_IE_PF0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,0)))
#define TSB_PF_IE_PF1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,1)))
#define TSB_PF_IE_PF2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,2)))
#define TSB_PF_IE_PF3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,3)))
#define TSB_PF_IE_PF4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,4)))
#define TSB_PF_IE_PF5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,5)))
#define TSB_PF_IE_PF6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,6)))
#define TSB_PF_IE_PF7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PF->IE,7)))


/* Port G */
#define TSB_PG_DATA_PG0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,0)))
#define TSB_PG_DATA_PG1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,1)))
#define TSB_PG_DATA_PG2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,2)))
#define TSB_PG_DATA_PG3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,3)))
#define TSB_PG_DATA_PG4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,4)))
#define TSB_PG_DATA_PG5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,5)))
#define TSB_PG_DATA_PG6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,6)))
#define TSB_PG_DATA_PG7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->DATA,7)))
#define TSB_PG_CR_PG0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,0)))
#define TSB_PG_CR_PG1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,1)))
#define TSB_PG_CR_PG2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,2)))
#define TSB_PG_CR_PG3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,3)))
#define TSB_PG_CR_PG4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,4)))
#define TSB_PG_CR_PG5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,5)))
#define TSB_PG_CR_PG6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,6)))
#define TSB_PG_CR_PG7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->CR,7)))
#define TSB_PG_FR1_PG0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR1,0)))
#define TSB_PG_FR1_PG1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR1,1)))
#define TSB_PG_FR1_PG6F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR1,6)))
#define TSB_PG_FR1_PG7F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR1,7)))
#define TSB_PG_FR2_PG4F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR2,4)))
#define TSB_PG_FR2_PG5F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR2,5)))
#define TSB_PG_FR3_PG0F3                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_PG->FR3,0)))
#define TSB_PG_FR3_PG1F3                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_PG->FR3,1)))
#define TSB_PG_FR3_PG2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR3,2)))
#define TSB_PG_FR3_PG3F3                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_PG->FR3,3)))
#define TSB_PG_FR3_PG5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR3,5)))
#define TSB_PG_FR4_PG2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR4,2)))
#define TSB_PG_FR4_PG3F4                          (*((__I  uint32_t *)BITBAND_PERI(&TSB_PG->FR4,3)))
#define TSB_PG_FR4_PG4F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR4,4)))
#define TSB_PG_FR4_PG5F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR4,5)))
#define TSB_PG_FR4_PG6F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR4,6)))
#define TSB_PG_FR4_PG7F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR4,7)))
#define TSB_PG_FR5_PG0F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR5,0)))
#define TSB_PG_FR5_PG1F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR5,1)))
#define TSB_PG_FR5_PG2F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR5,2)))
#define TSB_PG_FR5_PG3F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR5,3)))
#define TSB_PG_FR5_PG4F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR5,4)))
#define TSB_PG_FR5_PG5F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR5,5)))
#define TSB_PG_FR5_PG6F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR5,6)))
#define TSB_PG_FR5_PG7F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR5,7)))
#define TSB_PG_FR7_PG2F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR7,2)))
#define TSB_PG_FR7_PG3F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR7,3)))
#define TSB_PG_FR7_PG4F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR7,4)))
#define TSB_PG_FR7_PG5F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->FR7,5)))
#define TSB_PG_OD_PG0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,0)))
#define TSB_PG_OD_PG1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,1)))
#define TSB_PG_OD_PG2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,2)))
#define TSB_PG_OD_PG3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,3)))
#define TSB_PG_OD_PG4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,4)))
#define TSB_PG_OD_PG5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,5)))
#define TSB_PG_OD_PG6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,6)))
#define TSB_PG_OD_PG7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->OD,7)))
#define TSB_PG_PUP_PG0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,0)))
#define TSB_PG_PUP_PG1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,1)))
#define TSB_PG_PUP_PG2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,2)))
#define TSB_PG_PUP_PG3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,3)))
#define TSB_PG_PUP_PG4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,4)))
#define TSB_PG_PUP_PG5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,5)))
#define TSB_PG_PUP_PG6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,6)))
#define TSB_PG_PUP_PG7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PUP,7)))
#define TSB_PG_PDN_PG0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PDN,0)))
#define TSB_PG_PDN_PG1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PDN,1)))
#define TSB_PG_PDN_PG2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PDN,2)))
#define TSB_PG_PDN_PG3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PDN,3)))
#define TSB_PG_PDN_PG4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PDN,4)))
#define TSB_PG_PDN_PG5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PDN,5)))
#define TSB_PG_PDN_PG6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PDN,6)))
#define TSB_PG_PDN_PG7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->PDN,7)))
#define TSB_PG_IE_PG0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,0)))
#define TSB_PG_IE_PG1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,1)))
#define TSB_PG_IE_PG2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,2)))
#define TSB_PG_IE_PG3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,3)))
#define TSB_PG_IE_PG4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,4)))
#define TSB_PG_IE_PG5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,5)))
#define TSB_PG_IE_PG6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,6)))
#define TSB_PG_IE_PG7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PG->IE,7)))


/* Port H */
#define TSB_PH_DATA_PH0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->DATA,0)))
#define TSB_PH_DATA_PH1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->DATA,1)))
#define TSB_PH_DATA_PH2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->DATA,2)))
#define TSB_PH_DATA_PH3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->DATA,3)))
#define TSB_PH_DATA_PH4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->DATA,4)))
#define TSB_PH_DATA_PH5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->DATA,5)))
#define TSB_PH_DATA_PH6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->DATA,6)))
#define TSB_PH_DATA_PH7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->DATA,7)))
#define TSB_PH_CR_PH0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->CR,0)))
#define TSB_PH_CR_PH1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->CR,1)))
#define TSB_PH_CR_PH2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->CR,2)))
#define TSB_PH_CR_PH3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->CR,3)))
#define TSB_PH_CR_PH4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->CR,4)))
#define TSB_PH_CR_PH5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->CR,5)))
#define TSB_PH_CR_PH6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->CR,6)))
#define TSB_PH_CR_PH7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->CR,7)))
#define TSB_PH_FR1_PH0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR1,0)))
#define TSB_PH_FR1_PH1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR1,1)))
#define TSB_PH_FR1_PH2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR1,2)))
#define TSB_PH_FR1_PH3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR1,3)))
#define TSB_PH_FR1_PH4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR1,4)))
#define TSB_PH_FR1_PH5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR1,5)))
#define TSB_PH_FR1_PH6F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR1,6)))
#define TSB_PH_FR1_PH7F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR1,7)))
#define TSB_PH_FR3_PH0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR3,0)))
#define TSB_PH_FR3_PH1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR3,1)))
#define TSB_PH_FR3_PH2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR3,2)))
#define TSB_PH_FR3_PH3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR3,3)))
#define TSB_PH_FR3_PH4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR3,4)))
#define TSB_PH_FR3_PH5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR3,5)))
#define TSB_PH_FR3_PH6F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR3,6)))
#define TSB_PH_FR3_PH7F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR3,7)))
#define TSB_PH_FR4_PH0F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR4,0)))
#define TSB_PH_FR4_PH1F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR4,1)))
#define TSB_PH_FR4_PH2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR4,2)))
#define TSB_PH_FR4_PH3F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR4,3)))
#define TSB_PH_FR5_PH0F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR5,0)))
#define TSB_PH_FR5_PH1F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR5,1)))
#define TSB_PH_FR5_PH2F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR5,2)))
#define TSB_PH_FR5_PH3F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR5,3)))
#define TSB_PH_FR5_PH4F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR5,4)))
#define TSB_PH_FR5_PH5F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR5,5)))
#define TSB_PH_FR5_PH6F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR5,6)))
#define TSB_PH_FR5_PH7F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->FR5,7)))
#define TSB_PH_OD_PH0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->OD,0)))
#define TSB_PH_OD_PH1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->OD,1)))
#define TSB_PH_OD_PH2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->OD,2)))
#define TSB_PH_OD_PH3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->OD,3)))
#define TSB_PH_OD_PH4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->OD,4)))
#define TSB_PH_OD_PH5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->OD,5)))
#define TSB_PH_OD_PH6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->OD,6)))
#define TSB_PH_OD_PH7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->OD,7)))
#define TSB_PH_PUP_PH0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PUP,0)))
#define TSB_PH_PUP_PH1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PUP,1)))
#define TSB_PH_PUP_PH2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PUP,2)))
#define TSB_PH_PUP_PH3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PUP,3)))
#define TSB_PH_PUP_PH4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PUP,4)))
#define TSB_PH_PUP_PH5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PUP,5)))
#define TSB_PH_PUP_PH6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PUP,6)))
#define TSB_PH_PUP_PH7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PUP,7)))
#define TSB_PH_PDN_PH0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PDN,0)))
#define TSB_PH_PDN_PH1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PDN,1)))
#define TSB_PH_PDN_PH2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PDN,2)))
#define TSB_PH_PDN_PH3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PDN,3)))
#define TSB_PH_PDN_PH4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PDN,4)))
#define TSB_PH_PDN_PH5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PDN,5)))
#define TSB_PH_PDN_PH6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PDN,6)))
#define TSB_PH_PDN_PH7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->PDN,7)))
#define TSB_PH_IE_PH0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,0)))
#define TSB_PH_IE_PH1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,1)))
#define TSB_PH_IE_PH2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,2)))
#define TSB_PH_IE_PH3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,3)))
#define TSB_PH_IE_PH4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,4)))
#define TSB_PH_IE_PH5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,5)))
#define TSB_PH_IE_PH6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,6)))
#define TSB_PH_IE_PH7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PH->IE,7)))


/* Port J */
#define TSB_PJ_DATA_PJ0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,0)))
#define TSB_PJ_DATA_PJ1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,1)))
#define TSB_PJ_DATA_PJ2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,2)))
#define TSB_PJ_DATA_PJ3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,3)))
#define TSB_PJ_DATA_PJ4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,4)))
#define TSB_PJ_DATA_PJ5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,5)))
#define TSB_PJ_DATA_PJ6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,6)))
#define TSB_PJ_DATA_PJ7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->DATA,7)))
#define TSB_PJ_CR_PJ0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,0)))
#define TSB_PJ_CR_PJ1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,1)))
#define TSB_PJ_CR_PJ2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,2)))
#define TSB_PJ_CR_PJ3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,3)))
#define TSB_PJ_CR_PJ4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,4)))
#define TSB_PJ_CR_PJ5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,5)))
#define TSB_PJ_CR_PJ6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,6)))
#define TSB_PJ_CR_PJ7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->CR,7)))
#define TSB_PJ_FR2_PJ4F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR2,4)))
#define TSB_PJ_FR2_PJ5F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR2,5)))
#define TSB_PJ_FR3_PJ0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR3,0)))
#define TSB_PJ_FR3_PJ1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR3,1)))
#define TSB_PJ_FR3_PJ2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR3,2)))
#define TSB_PJ_FR3_PJ3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR3,3)))
#define TSB_PJ_FR3_PJ4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR3,4)))
#define TSB_PJ_FR3_PJ5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR3,5)))
#define TSB_PJ_FR5_PJ0F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR5,0)))
#define TSB_PJ_FR5_PJ1F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR5,1)))
#define TSB_PJ_FR5_PJ2F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR5,2)))
#define TSB_PJ_FR5_PJ3F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR5,3)))
#define TSB_PJ_FR5_PJ4F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR5,4)))
#define TSB_PJ_FR5_PJ5F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR5,5)))
#define TSB_PJ_FR5_PJ6F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR5,6)))
#define TSB_PJ_FR5_PJ7F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR5,7)))
#define TSB_PJ_FR7_PJ2F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR7,2)))
#define TSB_PJ_FR7_PJ3F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR7,3)))
#define TSB_PJ_FR7_PJ6F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR7,6)))
#define TSB_PJ_FR7_PJ7F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->FR7,7)))
#define TSB_PJ_OD_PJ0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,0)))
#define TSB_PJ_OD_PJ1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,1)))
#define TSB_PJ_OD_PJ2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,2)))
#define TSB_PJ_OD_PJ3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,3)))
#define TSB_PJ_OD_PJ4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,4)))
#define TSB_PJ_OD_PJ5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,5)))
#define TSB_PJ_OD_PJ6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,6)))
#define TSB_PJ_OD_PJ7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->OD,7)))
#define TSB_PJ_PUP_PJ0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,0)))
#define TSB_PJ_PUP_PJ1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,1)))
#define TSB_PJ_PUP_PJ2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,2)))
#define TSB_PJ_PUP_PJ3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,3)))
#define TSB_PJ_PUP_PJ4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,4)))
#define TSB_PJ_PUP_PJ5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,5)))
#define TSB_PJ_PUP_PJ6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,6)))
#define TSB_PJ_PUP_PJ7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PUP,7)))
#define TSB_PJ_PDN_PJ0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PDN,0)))
#define TSB_PJ_PDN_PJ1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PDN,1)))
#define TSB_PJ_PDN_PJ2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PDN,2)))
#define TSB_PJ_PDN_PJ3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PDN,3)))
#define TSB_PJ_PDN_PJ4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PDN,4)))
#define TSB_PJ_PDN_PJ5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PDN,5)))
#define TSB_PJ_PDN_PJ6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PDN,6)))
#define TSB_PJ_PDN_PJ7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->PDN,7)))
#define TSB_PJ_IE_PJ0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,0)))
#define TSB_PJ_IE_PJ1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,1)))
#define TSB_PJ_IE_PJ2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,2)))
#define TSB_PJ_IE_PJ3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,3)))
#define TSB_PJ_IE_PJ4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,4)))
#define TSB_PJ_IE_PJ5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,5)))
#define TSB_PJ_IE_PJ6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,6)))
#define TSB_PJ_IE_PJ7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PJ->IE,7)))


/* Port K */
#define TSB_PK_DATA_PK0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,0)))
#define TSB_PK_DATA_PK1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,1)))
#define TSB_PK_DATA_PK2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,2)))
#define TSB_PK_DATA_PK3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,3)))
#define TSB_PK_DATA_PK4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,4)))
#define TSB_PK_DATA_PK5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,5)))
#define TSB_PK_DATA_PK6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,6)))
#define TSB_PK_DATA_PK7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->DATA,7)))
#define TSB_PK_CR_PK0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,0)))
#define TSB_PK_CR_PK1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,1)))
#define TSB_PK_CR_PK2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,2)))
#define TSB_PK_CR_PK3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,3)))
#define TSB_PK_CR_PK4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,4)))
#define TSB_PK_CR_PK5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,5)))
#define TSB_PK_CR_PK6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,6)))
#define TSB_PK_CR_PK7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->CR,7)))
#define TSB_PK_FR1_PK0F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR1,0)))
#define TSB_PK_FR1_PK1F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR1,1)))
#define TSB_PK_FR1_PK2F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR1,2)))
#define TSB_PK_FR1_PK3F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR1,3)))
#define TSB_PK_FR1_PK4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR1,4)))
#define TSB_PK_FR1_PK5F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR1,5)))
#define TSB_PK_FR2_PK0F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR2,0)))
#define TSB_PK_FR2_PK1F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR2,1)))
#define TSB_PK_FR2_PK6F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR2,6)))
#define TSB_PK_FR2_PK7F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR2,7)))
#define TSB_PK_FR3_PK0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR3,0)))
#define TSB_PK_FR3_PK1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR3,1)))
#define TSB_PK_FR3_PK6F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR3,6)))
#define TSB_PK_FR3_PK7F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR3,7)))
#define TSB_PK_FR4_PK1F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR4,1)))
#define TSB_PK_FR4_PK4F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR4,4)))
#define TSB_PK_FR4_PK5F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR4,5)))
#define TSB_PK_FR4_PK6F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR4,6)))
#define TSB_PK_FR4_PK7F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR4,7)))
#define TSB_PK_FR6_PK2F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR6,2)))
#define TSB_PK_FR6_PK3F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR6,3)))
#define TSB_PK_FR6_PK4F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR6,4)))
#define TSB_PK_FR6_PK5F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR6,5)))
#define TSB_PK_FR6_PK6F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR6,6)))
#define TSB_PK_FR6_PK7F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR6,7)))
#define TSB_PK_FR7_PK7F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->FR7,7)))
#define TSB_PK_OD_PK0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->OD,0)))
#define TSB_PK_OD_PK1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->OD,1)))
#define TSB_PK_OD_PK2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->OD,2)))
#define TSB_PK_OD_PK3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->OD,3)))
#define TSB_PK_OD_PK4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->OD,4)))
#define TSB_PK_OD_PK5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->OD,5)))
#define TSB_PK_OD_PK6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->OD,6)))
#define TSB_PK_OD_PK7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->OD,7)))
#define TSB_PK_PUP_PK0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,0)))
#define TSB_PK_PUP_PK1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,1)))
#define TSB_PK_PUP_PK2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,2)))
#define TSB_PK_PUP_PK3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,3)))
#define TSB_PK_PUP_PK4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,4)))
#define TSB_PK_PUP_PK5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,5)))
#define TSB_PK_PUP_PK6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,6)))
#define TSB_PK_PUP_PK7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PUP,7)))
#define TSB_PK_PDN_PK0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PDN,0)))
#define TSB_PK_PDN_PK1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PDN,1)))
#define TSB_PK_PDN_PK2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PDN,2)))
#define TSB_PK_PDN_PK3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PDN,3)))
#define TSB_PK_PDN_PK4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PDN,4)))
#define TSB_PK_PDN_PK5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PDN,5)))
#define TSB_PK_PDN_PK6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PDN,6)))
#define TSB_PK_PDN_PK7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->PDN,7)))
#define TSB_PK_IE_PK0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,0)))
#define TSB_PK_IE_PK1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,1)))
#define TSB_PK_IE_PK2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,2)))
#define TSB_PK_IE_PK3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,3)))
#define TSB_PK_IE_PK4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,4)))
#define TSB_PK_IE_PK5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,5)))
#define TSB_PK_IE_PK6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,6)))
#define TSB_PK_IE_PK7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PK->IE,7)))


/* Port L */
#define TSB_PL_DATA_PL0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->DATA,0)))
#define TSB_PL_DATA_PL1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->DATA,1)))
#define TSB_PL_DATA_PL2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->DATA,2)))
#define TSB_PL_DATA_PL3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->DATA,3)))
#define TSB_PL_DATA_PL4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->DATA,4)))
#define TSB_PL_DATA_PL5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->DATA,5)))
#define TSB_PL_DATA_PL6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->DATA,6)))
#define TSB_PL_DATA_PL7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->DATA,7)))
#define TSB_PL_CR_PL0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->CR,0)))
#define TSB_PL_CR_PL1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->CR,1)))
#define TSB_PL_CR_PL2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->CR,2)))
#define TSB_PL_CR_PL3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->CR,3)))
#define TSB_PL_CR_PL4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->CR,4)))
#define TSB_PL_CR_PL5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->CR,5)))
#define TSB_PL_CR_PL6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->CR,6)))
#define TSB_PL_CR_PL7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->CR,7)))
#define TSB_PL_FR1_PL7F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR1,7)))
#define TSB_PL_FR2_PL0F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR2,0)))
#define TSB_PL_FR2_PL3F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR2,3)))
#define TSB_PL_FR2_PL4F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR2,4)))
#define TSB_PL_FR2_PL5F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR2,5)))
#define TSB_PL_FR2_PL6F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR2,6)))
#define TSB_PL_FR2_PL7F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR2,7)))
#define TSB_PL_FR3_PL0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR3,0)))
#define TSB_PL_FR3_PL3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR3,3)))
#define TSB_PL_FR3_PL4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR3,4)))
#define TSB_PL_FR3_PL6F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR3,6)))
#define TSB_PL_FR6_PL0F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR6,0)))
#define TSB_PL_FR6_PL3F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR6,3)))
#define TSB_PL_FR7_PL0F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR7,0)))
#define TSB_PL_FR7_PL1F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR7,1)))
#define TSB_PL_FR7_PL2F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR7,2)))
#define TSB_PL_FR7_PL3F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->FR7,3)))
#define TSB_PL_OD_PL0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->OD,0)))
#define TSB_PL_OD_PL1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->OD,1)))
#define TSB_PL_OD_PL2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->OD,2)))
#define TSB_PL_OD_PL3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->OD,3)))
#define TSB_PL_OD_PL4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->OD,4)))
#define TSB_PL_OD_PL5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->OD,5)))
#define TSB_PL_OD_PL6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->OD,6)))
#define TSB_PL_OD_PL7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->OD,7)))
#define TSB_PL_PUP_PL0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PUP,0)))
#define TSB_PL_PUP_PL1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PUP,1)))
#define TSB_PL_PUP_PL2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PUP,2)))
#define TSB_PL_PUP_PL3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PUP,3)))
#define TSB_PL_PUP_PL4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PUP,4)))
#define TSB_PL_PUP_PL5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PUP,5)))
#define TSB_PL_PUP_PL6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PUP,6)))
#define TSB_PL_PUP_PL7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PUP,7)))
#define TSB_PL_PDN_PL0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PDN,0)))
#define TSB_PL_PDN_PL1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PDN,1)))
#define TSB_PL_PDN_PL2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PDN,2)))
#define TSB_PL_PDN_PL3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PDN,3)))
#define TSB_PL_PDN_PL4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PDN,4)))
#define TSB_PL_PDN_PL5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PDN,5)))
#define TSB_PL_PDN_PL6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PDN,6)))
#define TSB_PL_PDN_PL7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->PDN,7)))
#define TSB_PL_IE_PL0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->IE,0)))
#define TSB_PL_IE_PL1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->IE,1)))
#define TSB_PL_IE_PL2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->IE,2)))
#define TSB_PL_IE_PL3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->IE,3)))
#define TSB_PL_IE_PL4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->IE,4)))
#define TSB_PL_IE_PL5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->IE,5)))
#define TSB_PL_IE_PL6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->IE,6)))
#define TSB_PL_IE_PL7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PL->IE,7)))


/* Port M */
#define TSB_PM_DATA_PM0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->DATA,0)))
#define TSB_PM_DATA_PM1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->DATA,1)))
#define TSB_PM_DATA_PM2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->DATA,2)))
#define TSB_PM_DATA_PM3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->DATA,3)))
#define TSB_PM_DATA_PM4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->DATA,4)))
#define TSB_PM_DATA_PM5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->DATA,5)))
#define TSB_PM_DATA_PM6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->DATA,6)))
#define TSB_PM_DATA_PM7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->DATA,7)))
#define TSB_PM_CR_PM0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->CR,0)))
#define TSB_PM_CR_PM1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->CR,1)))
#define TSB_PM_CR_PM2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->CR,2)))
#define TSB_PM_CR_PM3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->CR,3)))
#define TSB_PM_CR_PM4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->CR,4)))
#define TSB_PM_CR_PM5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->CR,5)))
#define TSB_PM_CR_PM6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->CR,6)))
#define TSB_PM_CR_PM7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->CR,7)))
#define TSB_PM_FR2_PM2F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR2,2)))
#define TSB_PM_FR2_PM3F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR2,3)))
#define TSB_PM_FR2_PM4F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR2,4)))
#define TSB_PM_FR2_PM5F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR2,5)))
#define TSB_PM_FR2_PM6F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR2,6)))
#define TSB_PM_FR2_PM7F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR2,7)))
#define TSB_PM_FR3_PM2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR3,2)))
#define TSB_PM_FR3_PM5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR3,5)))
#define TSB_PM_FR3_PM6F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR3,6)))
#define TSB_PM_FR4_PM0F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR4,0)))
#define TSB_PM_FR4_PM1F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR4,1)))
#define TSB_PM_FR4_PM3F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR4,3)))
#define TSB_PM_FR4_PM4F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR4,4)))
#define TSB_PM_FR4_PM6F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR4,6)))
#define TSB_PM_FR4_PM7F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR4,7)))
#define TSB_PM_FR5_PM0F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR5,0)))
#define TSB_PM_FR5_PM1F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR5,1)))
#define TSB_PM_FR5_PM2F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR5,2)))
#define TSB_PM_FR5_PM3F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR5,3)))
#define TSB_PM_FR5_PM6F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR5,6)))
#define TSB_PM_FR5_PM7F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR5,7)))
#define TSB_PM_FR6_PM0F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR6,0)))
#define TSB_PM_FR6_PM1F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR6,1)))
#define TSB_PM_FR6_PM2F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR6,2)))
#define TSB_PM_FR6_PM3F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR6,3)))
#define TSB_PM_FR6_PM4F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR6,4)))
#define TSB_PM_FR6_PM5F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR6,5)))
#define TSB_PM_FR6_PM6F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR6,6)))
#define TSB_PM_FR6_PM7F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR6,7)))
#define TSB_PM_FR7_PM0F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR7,0)))
#define TSB_PM_FR7_PM1F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR7,1)))
#define TSB_PM_FR7_PM2F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR7,2)))
#define TSB_PM_FR7_PM3F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR7,3)))
#define TSB_PM_FR7_PM4F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR7,4)))
#define TSB_PM_FR7_PM5F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR7,5)))
#define TSB_PM_FR7_PM6F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR7,6)))
#define TSB_PM_FR7_PM7F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->FR7,7)))
#define TSB_PM_OD_PM0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->OD,0)))
#define TSB_PM_OD_PM1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->OD,1)))
#define TSB_PM_OD_PM2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->OD,2)))
#define TSB_PM_OD_PM3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->OD,3)))
#define TSB_PM_OD_PM4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->OD,4)))
#define TSB_PM_OD_PM5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->OD,5)))
#define TSB_PM_OD_PM6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->OD,6)))
#define TSB_PM_OD_PM7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->OD,7)))
#define TSB_PM_PUP_PM0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PUP,0)))
#define TSB_PM_PUP_PM1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PUP,1)))
#define TSB_PM_PUP_PM2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PUP,2)))
#define TSB_PM_PUP_PM3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PUP,3)))
#define TSB_PM_PUP_PM4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PUP,4)))
#define TSB_PM_PUP_PM5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PUP,5)))
#define TSB_PM_PUP_PM6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PUP,6)))
#define TSB_PM_PUP_PM7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PUP,7)))
#define TSB_PM_PDN_PM0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PDN,0)))
#define TSB_PM_PDN_PM1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PDN,1)))
#define TSB_PM_PDN_PM2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PDN,2)))
#define TSB_PM_PDN_PM3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PDN,3)))
#define TSB_PM_PDN_PM4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PDN,4)))
#define TSB_PM_PDN_PM5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PDN,5)))
#define TSB_PM_PDN_PM6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PDN,6)))
#define TSB_PM_PDN_PM7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->PDN,7)))
#define TSB_PM_IE_PM0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->IE,0)))
#define TSB_PM_IE_PM1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->IE,1)))
#define TSB_PM_IE_PM2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->IE,2)))
#define TSB_PM_IE_PM3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->IE,3)))
#define TSB_PM_IE_PM4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->IE,4)))
#define TSB_PM_IE_PM5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->IE,5)))
#define TSB_PM_IE_PM6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->IE,6)))
#define TSB_PM_IE_PM7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PM->IE,7)))


/* Port N */
#define TSB_PN_DATA_PN0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->DATA,0)))
#define TSB_PN_DATA_PN1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->DATA,1)))
#define TSB_PN_DATA_PN2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->DATA,2)))
#define TSB_PN_DATA_PN3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->DATA,3)))
#define TSB_PN_DATA_PN4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->DATA,4)))
#define TSB_PN_DATA_PN5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->DATA,5)))
#define TSB_PN_DATA_PN6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->DATA,6)))
#define TSB_PN_DATA_PN7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->DATA,7)))
#define TSB_PN_CR_PN0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->CR,0)))
#define TSB_PN_CR_PN1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->CR,1)))
#define TSB_PN_CR_PN2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->CR,2)))
#define TSB_PN_CR_PN3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->CR,3)))
#define TSB_PN_CR_PN4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->CR,4)))
#define TSB_PN_CR_PN5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->CR,5)))
#define TSB_PN_CR_PN6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->CR,6)))
#define TSB_PN_CR_PN7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->CR,7)))
#define TSB_PN_OD_PN0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->OD,0)))
#define TSB_PN_OD_PN1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->OD,1)))
#define TSB_PN_OD_PN2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->OD,2)))
#define TSB_PN_OD_PN3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->OD,3)))
#define TSB_PN_OD_PN4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->OD,4)))
#define TSB_PN_OD_PN5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->OD,5)))
#define TSB_PN_OD_PN6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->OD,6)))
#define TSB_PN_OD_PN7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->OD,7)))
#define TSB_PN_PUP_PN0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PUP,0)))
#define TSB_PN_PUP_PN1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PUP,1)))
#define TSB_PN_PUP_PN2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PUP,2)))
#define TSB_PN_PUP_PN3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PUP,3)))
#define TSB_PN_PUP_PN4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PUP,4)))
#define TSB_PN_PUP_PN5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PUP,5)))
#define TSB_PN_PUP_PN6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PUP,6)))
#define TSB_PN_PUP_PN7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PUP,7)))
#define TSB_PN_PDN_PN0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PDN,0)))
#define TSB_PN_PDN_PN1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PDN,1)))
#define TSB_PN_PDN_PN2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PDN,2)))
#define TSB_PN_PDN_PN3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PDN,3)))
#define TSB_PN_PDN_PN4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PDN,4)))
#define TSB_PN_PDN_PN5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PDN,5)))
#define TSB_PN_PDN_PN6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PDN,6)))
#define TSB_PN_PDN_PN7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->PDN,7)))
#define TSB_PN_IE_PN0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->IE,0)))
#define TSB_PN_IE_PN1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->IE,1)))
#define TSB_PN_IE_PN2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->IE,2)))
#define TSB_PN_IE_PN3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->IE,3)))
#define TSB_PN_IE_PN4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->IE,4)))
#define TSB_PN_IE_PN5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->IE,5)))
#define TSB_PN_IE_PN6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->IE,6)))
#define TSB_PN_IE_PN7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PN->IE,7)))


/* Port P */
#define TSB_PP_DATA_PP0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->DATA,0)))
#define TSB_PP_DATA_PP1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->DATA,1)))
#define TSB_PP_DATA_PP2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->DATA,2)))
#define TSB_PP_DATA_PP3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->DATA,3)))
#define TSB_PP_DATA_PP4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->DATA,4)))
#define TSB_PP_DATA_PP5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->DATA,5)))
#define TSB_PP_DATA_PP6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->DATA,6)))
#define TSB_PP_DATA_PP7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->DATA,7)))
#define TSB_PP_CR_PP0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->CR,0)))
#define TSB_PP_CR_PP1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->CR,1)))
#define TSB_PP_CR_PP2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->CR,2)))
#define TSB_PP_CR_PP3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->CR,3)))
#define TSB_PP_CR_PP4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->CR,4)))
#define TSB_PP_CR_PP5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->CR,5)))
#define TSB_PP_CR_PP6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->CR,6)))
#define TSB_PP_CR_PP7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->CR,7)))
#define TSB_PP_FR2_PP0FR2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR2,0)))
#define TSB_PP_FR2_PP1FR2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR2,1)))
#define TSB_PP_FR2_PP2FR2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR2,2)))
#define TSB_PP_FR2_PP3FR2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR2,3)))
#define TSB_PP_FR2_PP4FR2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR2,4)))
#define TSB_PP_FR2_PP5FR2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR2,5)))
#define TSB_PP_FR2_PP6FR2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR2,6)))
#define TSB_PP_FR2_PP7FR2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR2,7)))
#define TSB_PP_FR3_PP0FR3                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR3,0)))
#define TSB_PP_FR3_PP1FR3                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR3,1)))
#define TSB_PP_FR3_PP2FR3                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR3,2)))
#define TSB_PP_FR3_PP3FR3                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR3,3)))
#define TSB_PP_FR3_PP4FR3                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR3,4)))
#define TSB_PP_FR3_PP5FR3                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR3,5)))
#define TSB_PP_FR3_PP6FR3                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR3,6)))
#define TSB_PP_FR3_PP7FR3                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR3,7)))
#define TSB_PP_FR5_PP0FR5                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR5,0)))
#define TSB_PP_FR5_PP1FR5                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR5,1)))
#define TSB_PP_FR5_PP2FR5                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR5,2)))
#define TSB_PP_FR5_PP3FR5                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR5,3)))
#define TSB_PP_FR5_PP4FR5                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR5,4)))
#define TSB_PP_FR5_PP5FR5                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR5,5)))
#define TSB_PP_FR5_PP6FR5                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR5,6)))
#define TSB_PP_FR5_PP7FR5                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->FR5,7)))
#define TSB_PP_OD_PP0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->OD,0)))
#define TSB_PP_OD_PP1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->OD,1)))
#define TSB_PP_OD_PP2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->OD,2)))
#define TSB_PP_OD_PP3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->OD,3)))
#define TSB_PP_OD_PP4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->OD,4)))
#define TSB_PP_OD_PP5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->OD,5)))
#define TSB_PP_OD_PP6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->OD,6)))
#define TSB_PP_OD_PP7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->OD,7)))
#define TSB_PP_PUP_PP0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PUP,0)))
#define TSB_PP_PUP_PP1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PUP,1)))
#define TSB_PP_PUP_PP2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PUP,2)))
#define TSB_PP_PUP_PP3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PUP,3)))
#define TSB_PP_PUP_PP4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PUP,4)))
#define TSB_PP_PUP_PP5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PUP,5)))
#define TSB_PP_PUP_PP6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PUP,6)))
#define TSB_PP_PUP_PP7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PUP,7)))
#define TSB_PP_PDN_PP0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PDN,0)))
#define TSB_PP_PDN_PP1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PDN,1)))
#define TSB_PP_PDN_PP2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PDN,2)))
#define TSB_PP_PDN_PP3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PDN,3)))
#define TSB_PP_PDN_PP4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PDN,4)))
#define TSB_PP_PDN_PP5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PDN,5)))
#define TSB_PP_PDN_PP6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PDN,6)))
#define TSB_PP_PDN_PP7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->PDN,7)))
#define TSB_PP_IE_PP0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->IE,0)))
#define TSB_PP_IE_PP1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->IE,1)))
#define TSB_PP_IE_PP2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->IE,2)))
#define TSB_PP_IE_PP3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->IE,3)))
#define TSB_PP_IE_PP4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->IE,4)))
#define TSB_PP_IE_PP5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->IE,5)))
#define TSB_PP_IE_PP6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->IE,6)))
#define TSB_PP_IE_PP7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PP->IE,7)))


/* Port R */
#define TSB_PR_DATA_PR0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->DATA,0)))
#define TSB_PR_DATA_PR1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->DATA,1)))
#define TSB_PR_DATA_PR2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->DATA,2)))
#define TSB_PR_DATA_PR3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->DATA,3)))
#define TSB_PR_DATA_PR4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->DATA,4)))
#define TSB_PR_DATA_PR5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->DATA,5)))
#define TSB_PR_DATA_PR6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->DATA,6)))
#define TSB_PR_DATA_PR7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->DATA,7)))
#define TSB_PR_CR_PR0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->CR,0)))
#define TSB_PR_CR_PR1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->CR,1)))
#define TSB_PR_CR_PR2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->CR,2)))
#define TSB_PR_CR_PR3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->CR,3)))
#define TSB_PR_CR_PR4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->CR,4)))
#define TSB_PR_CR_PR5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->CR,5)))
#define TSB_PR_CR_PR6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->CR,6)))
#define TSB_PR_CR_PR7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->CR,7)))
#define TSB_PR_FR2_PR0FR2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR2,0)))
#define TSB_PR_FR2_PR1FR2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR2,1)))
#define TSB_PR_FR2_PR2FR2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR2,2)))
#define TSB_PR_FR2_PR3FR2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR2,3)))
#define TSB_PR_FR2_PR4FR2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR2,4)))
#define TSB_PR_FR2_PR5FR2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR2,5)))
#define TSB_PR_FR2_PR6FR2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR2,6)))
#define TSB_PR_FR2_PR7FR2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR2,7)))
#define TSB_PR_FR3_PR0FR3                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR3,0)))
#define TSB_PR_FR3_PR1FR3                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR3,1)))
#define TSB_PR_FR3_PR2FR3                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR3,2)))
#define TSB_PR_FR3_PR3FR3                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR3,3)))
#define TSB_PR_FR3_PR4FR3                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR3,4)))
#define TSB_PR_FR3_PR5FR3                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR3,5)))
#define TSB_PR_FR3_PR6FR3                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR3,6)))
#define TSB_PR_FR3_PR7FR3                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->FR3,7)))
#define TSB_PR_OD_PR0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->OD,0)))
#define TSB_PR_OD_PR1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->OD,1)))
#define TSB_PR_OD_PR2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->OD,2)))
#define TSB_PR_OD_PR3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->OD,3)))
#define TSB_PR_OD_PR4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->OD,4)))
#define TSB_PR_OD_PR5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->OD,5)))
#define TSB_PR_OD_PR6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->OD,6)))
#define TSB_PR_OD_PR7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->OD,7)))
#define TSB_PR_PUP_PR0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PUP,0)))
#define TSB_PR_PUP_PR1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PUP,1)))
#define TSB_PR_PUP_PR2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PUP,2)))
#define TSB_PR_PUP_PR3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PUP,3)))
#define TSB_PR_PUP_PR4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PUP,4)))
#define TSB_PR_PUP_PR5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PUP,5)))
#define TSB_PR_PUP_PR6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PUP,6)))
#define TSB_PR_PUP_PR7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PUP,7)))
#define TSB_PR_PDN_PR0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PDN,0)))
#define TSB_PR_PDN_PR1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PDN,1)))
#define TSB_PR_PDN_PR2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PDN,2)))
#define TSB_PR_PDN_PR3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PDN,3)))
#define TSB_PR_PDN_PR4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PDN,4)))
#define TSB_PR_PDN_PR5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PDN,5)))
#define TSB_PR_PDN_PR6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PDN,6)))
#define TSB_PR_PDN_PR7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->PDN,7)))
#define TSB_PR_IE_PR0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->IE,0)))
#define TSB_PR_IE_PR1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->IE,1)))
#define TSB_PR_IE_PR2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->IE,2)))
#define TSB_PR_IE_PR3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->IE,3)))
#define TSB_PR_IE_PR4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->IE,4)))
#define TSB_PR_IE_PR5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->IE,5)))
#define TSB_PR_IE_PR6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->IE,6)))
#define TSB_PR_IE_PR7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PR->IE,7)))


/* Port T */
#define TSB_PT_DATA_PT0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->DATA,0)))
#define TSB_PT_DATA_PT1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->DATA,1)))
#define TSB_PT_DATA_PT2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->DATA,2)))
#define TSB_PT_DATA_PT3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->DATA,3)))
#define TSB_PT_DATA_PT4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->DATA,4)))
#define TSB_PT_DATA_PT5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->DATA,5)))
#define TSB_PT_CR_PT0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->CR,0)))
#define TSB_PT_CR_PT1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->CR,1)))
#define TSB_PT_CR_PT2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->CR,2)))
#define TSB_PT_CR_PT3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->CR,3)))
#define TSB_PT_CR_PT4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->CR,4)))
#define TSB_PT_CR_PT5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->CR,5)))
#define TSB_PT_FR1_PT3FR1                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->FR1,3)))
#define TSB_PT_FR2_PT3FR2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->FR2,3)))
#define TSB_PT_FR2_PT5FR2                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->FR2,5)))
#define TSB_PT_FR3_PT3FR3                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->FR3,3)))
#define TSB_PT_FR6_PT3FR6                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->FR6,3)))
#define TSB_PT_FR7_PT2FR7                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->FR7,2)))
#define TSB_PT_OD_PT0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->OD,0)))
#define TSB_PT_OD_PT1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->OD,1)))
#define TSB_PT_OD_PT2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->OD,2)))
#define TSB_PT_OD_PT3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->OD,3)))
#define TSB_PT_OD_PT4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->OD,4)))
#define TSB_PT_OD_PT5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->OD,5)))
#define TSB_PT_PUP_PT0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->PUP,0)))
#define TSB_PT_PUP_PT1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->PUP,1)))
#define TSB_PT_PUP_PT2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->PUP,2)))
#define TSB_PT_PUP_PT3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->PUP,3)))
#define TSB_PT_PUP_PT4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->PUP,4)))
#define TSB_PT_PUP_PT5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->PUP,5)))
#define TSB_PT_PDN_PT0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->PDN,0)))
#define TSB_PT_PDN_PT1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->PDN,1)))
#define TSB_PT_PDN_PT2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->PDN,2)))
#define TSB_PT_PDN_PT3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->PDN,3)))
#define TSB_PT_PDN_PT4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->PDN,4)))
#define TSB_PT_PDN_PT5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->PDN,5)))
#define TSB_PT_IE_PT0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->IE,0)))
#define TSB_PT_IE_PT1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->IE,1)))
#define TSB_PT_IE_PT2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->IE,2)))
#define TSB_PT_IE_PT3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->IE,3)))
#define TSB_PT_IE_PT4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->IE,4)))
#define TSB_PT_IE_PT5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PT->IE,5)))


/* Port U */
#define TSB_PU_DATA_PU0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->DATA,0)))
#define TSB_PU_DATA_PU1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->DATA,1)))
#define TSB_PU_DATA_PU2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->DATA,2)))
#define TSB_PU_DATA_PU3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->DATA,3)))
#define TSB_PU_DATA_PU4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->DATA,4)))
#define TSB_PU_DATA_PU5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->DATA,5)))
#define TSB_PU_DATA_PU6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->DATA,6)))
#define TSB_PU_DATA_PU7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->DATA,7)))
#define TSB_PU_CR_PU0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->CR,0)))
#define TSB_PU_CR_PU1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->CR,1)))
#define TSB_PU_CR_PU2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->CR,2)))
#define TSB_PU_CR_PU3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->CR,3)))
#define TSB_PU_CR_PU4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->CR,4)))
#define TSB_PU_CR_PU5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->CR,5)))
#define TSB_PU_CR_PU6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->CR,6)))
#define TSB_PU_CR_PU7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->CR,7)))
#define TSB_PU_FR2_PU0F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR2,0)))
#define TSB_PU_FR2_PU1F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR2,1)))
#define TSB_PU_FR2_PU2F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR2,2)))
#define TSB_PU_FR2_PU3F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR2,3)))
#define TSB_PU_FR2_PU4F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR2,4)))
#define TSB_PU_FR2_PU5F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR2,5)))
#define TSB_PU_FR2_PU6F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR2,6)))
#define TSB_PU_FR2_PU7F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR2,7)))
#define TSB_PU_FR3_PU0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR3,0)))
#define TSB_PU_FR3_PU2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR3,2)))
#define TSB_PU_FR3_PU3F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR3,3)))
#define TSB_PU_FR3_PU4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR3,4)))
#define TSB_PU_FR3_PU5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR3,5)))
#define TSB_PU_FR3_PU6F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR3,6)))
#define TSB_PU_FR7_PU0F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR7,0)))
#define TSB_PU_FR7_PU1F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR7,1)))
#define TSB_PU_FR7_PU2F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR7,2)))
#define TSB_PU_FR7_PU3F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR7,3)))
#define TSB_PU_FR7_PU4F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR7,4)))
#define TSB_PU_FR7_PU5F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR7,5)))
#define TSB_PU_FR7_PU6F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR7,6)))
#define TSB_PU_FR7_PU7F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->FR7,7)))
#define TSB_PU_OD_PU0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->OD,0)))
#define TSB_PU_OD_PU1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->OD,1)))
#define TSB_PU_OD_PU2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->OD,2)))
#define TSB_PU_OD_PU3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->OD,3)))
#define TSB_PU_OD_PU4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->OD,4)))
#define TSB_PU_OD_PU5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->OD,5)))
#define TSB_PU_OD_PU6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->OD,6)))
#define TSB_PU_OD_PU7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->OD,7)))
#define TSB_PU_PUP_PU0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->PUP,0)))
#define TSB_PU_PUP_PU1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->PUP,1)))
#define TSB_PU_PUP_PU2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->PUP,2)))
#define TSB_PU_PUP_PU3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->PUP,3)))
#define TSB_PU_PUP_PU4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->PUP,4)))
#define TSB_PU_PUP_PU5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->PUP,5)))
#define TSB_PU_PUP_PU6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->PUP,6)))
#define TSB_PU_PUP_PU7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->PUP,7)))
#define TSB_PU_PDN_PU0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->PDN,0)))
#define TSB_PU_PDN_PU1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->PDN,1)))
#define TSB_PU_PDN_PU2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->PDN,2)))
#define TSB_PU_PDN_PU3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->PDN,3)))
#define TSB_PU_PDN_PU4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->PDN,4)))
#define TSB_PU_PDN_PU5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->PDN,5)))
#define TSB_PU_PDN_PU6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->PDN,6)))
#define TSB_PU_PDN_PU7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->PDN,7)))
#define TSB_PU_IE_PU0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->IE,0)))
#define TSB_PU_IE_PU1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->IE,1)))
#define TSB_PU_IE_PU2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->IE,2)))
#define TSB_PU_IE_PU3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->IE,3)))
#define TSB_PU_IE_PU4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->IE,4)))
#define TSB_PU_IE_PU5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->IE,5)))
#define TSB_PU_IE_PU6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->IE,6)))
#define TSB_PU_IE_PU7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PU->IE,7)))


/* Port V */
#define TSB_PV_DATA_PV0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->DATA,0)))
#define TSB_PV_DATA_PV1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->DATA,1)))
#define TSB_PV_DATA_PV2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->DATA,2)))
#define TSB_PV_DATA_PV3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->DATA,3)))
#define TSB_PV_DATA_PV4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->DATA,4)))
#define TSB_PV_DATA_PV5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->DATA,5)))
#define TSB_PV_DATA_PV6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->DATA,6)))
#define TSB_PV_DATA_PV7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->DATA,7)))
#define TSB_PV_CR_PV0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->CR,0)))
#define TSB_PV_CR_PV1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->CR,1)))
#define TSB_PV_CR_PV2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->CR,2)))
#define TSB_PV_CR_PV3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->CR,3)))
#define TSB_PV_CR_PV4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->CR,4)))
#define TSB_PV_CR_PV5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->CR,5)))
#define TSB_PV_CR_PV6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->CR,6)))
#define TSB_PV_CR_PV7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->CR,7)))
#define TSB_PV_FR2_PV0F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR2,0)))
#define TSB_PV_FR2_PV1F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR2,1)))
#define TSB_PV_FR2_PV2F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR2,2)))
#define TSB_PV_FR2_PV3F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR2,3)))
#define TSB_PV_FR2_PV4F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR2,4)))
#define TSB_PV_FR2_PV5F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR2,5)))
#define TSB_PV_FR2_PV6F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR2,6)))
#define TSB_PV_FR2_PV7F2                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR2,7)))
#define TSB_PV_FR3_PV0F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR3,0)))
#define TSB_PV_FR3_PV1F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR3,1)))
#define TSB_PV_FR3_PV2F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR3,2)))
#define TSB_PV_FR3_PV5F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR3,5)))
#define TSB_PV_FR3_PV6F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR3,6)))
#define TSB_PV_FR4_PV4F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR4,4)))
#define TSB_PV_FR4_PV5F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR4,5)))
#define TSB_PV_FR4_PV6F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR4,6)))
#define TSB_PV_FR4_PV7F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR4,7)))
#define TSB_PV_FR5_PV0F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR5,0)))
#define TSB_PV_FR5_PV1F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR5,1)))
#define TSB_PV_FR5_PV2F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR5,2)))
#define TSB_PV_FR5_PV3F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR5,3)))
#define TSB_PV_FR5_PV4F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR5,4)))
#define TSB_PV_FR5_PV5F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR5,5)))
#define TSB_PV_FR5_PV6F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR5,6)))
#define TSB_PV_FR5_PV7F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR5,7)))
#define TSB_PV_FR6_PV0F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR6,0)))
#define TSB_PV_FR6_PV1F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR6,1)))
#define TSB_PV_FR6_PV2F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR6,2)))
#define TSB_PV_FR6_PV3F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR6,3)))
#define TSB_PV_FR6_PV4F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR6,4)))
#define TSB_PV_FR6_PV5F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR6,5)))
#define TSB_PV_FR6_PV7F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR6,7)))
#define TSB_PV_FR7_PV0F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR7,0)))
#define TSB_PV_FR7_PV1F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR7,1)))
#define TSB_PV_FR7_PV2F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR7,2)))
#define TSB_PV_FR7_PV3F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR7,3)))
#define TSB_PV_FR7_PV4F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR7,4)))
#define TSB_PV_FR7_PV5F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR7,5)))
#define TSB_PV_FR7_PV6F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR7,6)))
#define TSB_PV_FR7_PV7F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->FR7,7)))
#define TSB_PV_OD_PV0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->OD,0)))
#define TSB_PV_OD_PV1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->OD,1)))
#define TSB_PV_OD_PV2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->OD,2)))
#define TSB_PV_OD_PV3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->OD,3)))
#define TSB_PV_OD_PV4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->OD,4)))
#define TSB_PV_OD_PV5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->OD,5)))
#define TSB_PV_OD_PV6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->OD,6)))
#define TSB_PV_OD_PV7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->OD,7)))
#define TSB_PV_PUP_PV0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->PUP,0)))
#define TSB_PV_PUP_PV1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->PUP,1)))
#define TSB_PV_PUP_PV2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->PUP,2)))
#define TSB_PV_PUP_PV3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->PUP,3)))
#define TSB_PV_PUP_PV4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->PUP,4)))
#define TSB_PV_PUP_PV5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->PUP,5)))
#define TSB_PV_PUP_PV6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->PUP,6)))
#define TSB_PV_PUP_PV7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->PUP,7)))
#define TSB_PV_PDN_PV0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->PDN,0)))
#define TSB_PV_PDN_PV1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->PDN,1)))
#define TSB_PV_PDN_PV2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->PDN,2)))
#define TSB_PV_PDN_PV3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->PDN,3)))
#define TSB_PV_PDN_PV4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->PDN,4)))
#define TSB_PV_PDN_PV5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->PDN,5)))
#define TSB_PV_PDN_PV6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->PDN,6)))
#define TSB_PV_PDN_PV7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->PDN,7)))
#define TSB_PV_IE_PV0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->IE,0)))
#define TSB_PV_IE_PV1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->IE,1)))
#define TSB_PV_IE_PV2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->IE,2)))
#define TSB_PV_IE_PV3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->IE,3)))
#define TSB_PV_IE_PV4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->IE,4)))
#define TSB_PV_IE_PV5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->IE,5)))
#define TSB_PV_IE_PV6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->IE,6)))
#define TSB_PV_IE_PV7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PV->IE,7)))


/* Port W */
#define TSB_PW_DATA_PW0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->DATA,0)))
#define TSB_PW_DATA_PW1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->DATA,1)))
#define TSB_PW_DATA_PW2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->DATA,2)))
#define TSB_PW_DATA_PW3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->DATA,3)))
#define TSB_PW_DATA_PW4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->DATA,4)))
#define TSB_PW_DATA_PW5                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->DATA,5)))
#define TSB_PW_DATA_PW6                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->DATA,6)))
#define TSB_PW_DATA_PW7                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->DATA,7)))
#define TSB_PW_CR_PW0C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->CR,0)))
#define TSB_PW_CR_PW1C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->CR,1)))
#define TSB_PW_CR_PW2C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->CR,2)))
#define TSB_PW_CR_PW3C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->CR,3)))
#define TSB_PW_CR_PW4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->CR,4)))
#define TSB_PW_CR_PW5C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->CR,5)))
#define TSB_PW_CR_PW6C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->CR,6)))
#define TSB_PW_CR_PW7C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->CR,7)))
#define TSB_PW_FR3_PW4F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->FR3,4)))
#define TSB_PW_FR3_PW7F3                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->FR3,7)))
#define TSB_PW_FR4_PW0F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->FR4,0)))
#define TSB_PW_FR4_PW1F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->FR4,1)))
#define TSB_PW_FR4_PW2F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->FR4,2)))
#define TSB_PW_FR4_PW3F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->FR4,3)))
#define TSB_PW_FR5_PW0F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->FR5,0)))
#define TSB_PW_FR5_PW1F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->FR5,1)))
#define TSB_PW_FR5_PW2F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->FR5,2)))
#define TSB_PW_FR5_PW3F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->FR5,3)))
#define TSB_PW_FR5_PW4F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->FR5,4)))
#define TSB_PW_FR5_PW5F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->FR5,5)))
#define TSB_PW_FR5_PW6F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->FR5,6)))
#define TSB_PW_FR5_PW7F5                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->FR5,7)))
#define TSB_PW_FR6_PW0F6                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->FR6,0)))
#define TSB_PW_FR7_PW1F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->FR7,1)))
#define TSB_PW_FR7_PW2F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->FR7,2)))
#define TSB_PW_FR7_PW4F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->FR7,4)))
#define TSB_PW_FR7_PW5F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->FR7,5)))
#define TSB_PW_FR7_PW6F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->FR7,6)))
#define TSB_PW_FR7_PW7F7                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->FR7,7)))
#define TSB_PW_OD_PW0OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->OD,0)))
#define TSB_PW_OD_PW1OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->OD,1)))
#define TSB_PW_OD_PW2OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->OD,2)))
#define TSB_PW_OD_PW3OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->OD,3)))
#define TSB_PW_OD_PW4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->OD,4)))
#define TSB_PW_OD_PW5OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->OD,5)))
#define TSB_PW_OD_PW6OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->OD,6)))
#define TSB_PW_OD_PW7OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->OD,7)))
#define TSB_PW_PUP_PW0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->PUP,0)))
#define TSB_PW_PUP_PW1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->PUP,1)))
#define TSB_PW_PUP_PW2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->PUP,2)))
#define TSB_PW_PUP_PW3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->PUP,3)))
#define TSB_PW_PUP_PW4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->PUP,4)))
#define TSB_PW_PUP_PW5UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->PUP,5)))
#define TSB_PW_PUP_PW6UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->PUP,6)))
#define TSB_PW_PUP_PW7UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->PUP,7)))
#define TSB_PW_PDN_PW0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->PDN,0)))
#define TSB_PW_PDN_PW1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->PDN,1)))
#define TSB_PW_PDN_PW2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->PDN,2)))
#define TSB_PW_PDN_PW3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->PDN,3)))
#define TSB_PW_PDN_PW4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->PDN,4)))
#define TSB_PW_PDN_PW5DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->PDN,5)))
#define TSB_PW_PDN_PW6DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->PDN,6)))
#define TSB_PW_PDN_PW7DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->PDN,7)))
#define TSB_PW_IE_PW0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->IE,0)))
#define TSB_PW_IE_PW1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->IE,1)))
#define TSB_PW_IE_PW2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->IE,2)))
#define TSB_PW_IE_PW3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->IE,3)))
#define TSB_PW_IE_PW4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->IE,4)))
#define TSB_PW_IE_PW5IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->IE,5)))
#define TSB_PW_IE_PW6IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->IE,6)))
#define TSB_PW_IE_PW7IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PW->IE,7)))


/* Port Y */
#define TSB_PY_DATA_PY0                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->DATA,0)))
#define TSB_PY_DATA_PY1                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->DATA,1)))
#define TSB_PY_DATA_PY2                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->DATA,2)))
#define TSB_PY_DATA_PY3                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->DATA,3)))
#define TSB_PY_DATA_PY4                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->DATA,4)))
#define TSB_PY_CR_PY4C                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->CR,4)))
#define TSB_PY_FR1_PY4F1                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->FR1,4)))
#define TSB_PY_FR4_PY4F4                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->FR4,4)))
#define TSB_PY_OD_PY4OD                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->OD,4)))
#define TSB_PY_PUP_PY0UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->PUP,0)))
#define TSB_PY_PUP_PY1UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->PUP,1)))
#define TSB_PY_PUP_PY2UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->PUP,2)))
#define TSB_PY_PUP_PY3UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->PUP,3)))
#define TSB_PY_PUP_PY4UP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->PUP,4)))
#define TSB_PY_PDN_PY0DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->PDN,0)))
#define TSB_PY_PDN_PY1DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->PDN,1)))
#define TSB_PY_PDN_PY2DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->PDN,2)))
#define TSB_PY_PDN_PY3DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->PDN,3)))
#define TSB_PY_PDN_PY4DN                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->PDN,4)))
#define TSB_PY_IE_PY0IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->IE,0)))
#define TSB_PY_IE_PY1IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->IE,1)))
#define TSB_PY_IE_PY2IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->IE,2)))
#define TSB_PY_IE_PY3IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->IE,3)))
#define TSB_PY_IE_PY4IE                           (*((__IO uint32_t *)BITBAND_PERI(&TSB_PY->IE,4)))


/* Internal High-speed Oscillation Adjustment */
#define TSB_TRM_OSCEN_TRIMEN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_TRM->OSCEN,0)))


/* Oscillation Frequency Detector (OFD) */
#define TSB_OFD_RST_OFDRSTEN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_OFD->RST,0)))
#define TSB_OFD_STAT_FRQERR                       (*((__I  uint32_t *)BITBAND_PERI(&TSB_OFD->STAT,0)))
#define TSB_OFD_STAT_OFDBUSY                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_OFD->STAT,1)))
#define TSB_OFD_MON_OFDMON                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_OFD->MON,0)))



/* Consumer Electronics Control (CEC) */
#define TSB_CEC0_EN_CECEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_CEC0->EN,0)))
#define TSB_CEC0_RESET_CECRESET                   (*((__O  uint32_t *)BITBAND_PERI(&TSB_CEC0->RESET,0)))
#define TSB_CEC0_REN_CECREN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CEC0->REN,0)))
#define TSB_CEC0_RBUF_CECEOM                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_CEC0->RBUF,8)))
#define TSB_CEC0_RBUF_CECACK                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_CEC0->RBUF,9)))
#define TSB_CEC0_RCR1_CECOTH                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_CEC0->RCR1,0)))
#define TSB_CEC0_RCR1_CECRIHLD                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CEC0->RCR1,1)))
#define TSB_CEC0_RCR1_CECACKDIS                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_CEC0->RCR1,24)))
#define TSB_CEC0_RCR3_CECWAVEN                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_CEC0->RCR3,0)))
#define TSB_CEC0_RCR3_CECRSTAEN                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_CEC0->RCR3,1)))
#define TSB_CEC0_TEN_CECTEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CEC0->TEN,0)))
#define TSB_CEC0_TEN_CECTRANS                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_CEC0->TEN,1)))
#define TSB_CEC0_TBUF_CECTEOM                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CEC0->TBUF,8)))
#define TSB_CEC0_TCR_CECBRD                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_CEC0->TCR,4)))
#define TSB_CEC0_RSTAT_CECRIEND                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_CEC0->RSTAT,0)))
#define TSB_CEC0_RSTAT_CECRISTA                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_CEC0->RSTAT,1)))
#define TSB_CEC0_RSTAT_CECRIMAX                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_CEC0->RSTAT,2)))
#define TSB_CEC0_RSTAT_CECRIMIN                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_CEC0->RSTAT,3)))
#define TSB_CEC0_RSTAT_CECRIACK                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_CEC0->RSTAT,4)))
#define TSB_CEC0_RSTAT_CECRIOR                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_CEC0->RSTAT,5)))
#define TSB_CEC0_RSTAT_CECRIWAV                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_CEC0->RSTAT,6)))
#define TSB_CEC0_TSTAT_CECTISTA                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_CEC0->TSTAT,0)))
#define TSB_CEC0_TSTAT_CECTIEND                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_CEC0->TSTAT,1)))
#define TSB_CEC0_TSTAT_CECTIAL                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_CEC0->TSTAT,2)))
#define TSB_CEC0_TSTAT_CECTIACK                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_CEC0->TSTAT,3)))
#define TSB_CEC0_TSTAT_CECTIUR                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_CEC0->TSTAT,4)))
#define TSB_CEC0_FSSEL_CECCLK                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_CEC0->FSSEL,0)))


/* Remote Control Signal Preprocessor (RMC) */
#define TSB_RMC0_EN_RMCEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC0->EN,0)))
#define TSB_RMC0_REN_RMCREN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC0->REN,0)))
#define TSB_RMC0_RCR2_RMCPHM                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC0->RCR2,24)))
#define TSB_RMC0_RCR2_RMCLD                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC0->RCR2,25)))
#define TSB_RMC0_RCR2_RMCRPIEN                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC0->RCR2,29)))
#define TSB_RMC0_RCR2_RMCEDIEN                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC0->RCR2,30)))
#define TSB_RMC0_RCR2_RMCLIEN                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC0->RCR2,31)))
#define TSB_RMC0_RCR3_RMCRP                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC0->RCR3,15)))
#define TSB_RMC0_RCR4_RMCPO                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC0->RCR4,7)))
#define TSB_RMC0_RSTAT_RMCRLDR                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_RMC0->RSTAT,7)))
#define TSB_RMC0_RSTAT_RMCRRP                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_RMC0->RSTAT,8)))
#define TSB_RMC0_RSTAT_RMCRRPIF                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_RMC0->RSTAT,11)))
#define TSB_RMC0_RSTAT_RMCEDIF                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_RMC0->RSTAT,12)))
#define TSB_RMC0_RSTAT_RMCDMAXIF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_RMC0->RSTAT,13)))
#define TSB_RMC0_RSTAT_RMCLOIF                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_RMC0->RSTAT,14)))
#define TSB_RMC0_RSTAT_RMCRLIF                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_RMC0->RSTAT,15)))
#define TSB_RMC0_FSSEL_RMCCLK                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC0->FSSEL,0)))

#define TSB_RMC1_EN_RMCEN                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC1->EN,0)))
#define TSB_RMC1_REN_RMCREN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC1->REN,0)))
#define TSB_RMC1_RCR2_RMCPHM                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC1->RCR2,24)))
#define TSB_RMC1_RCR2_RMCLD                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC1->RCR2,25)))
#define TSB_RMC1_RCR2_RMCRPIEN                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC1->RCR2,29)))
#define TSB_RMC1_RCR2_RMCEDIEN                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC1->RCR2,30)))
#define TSB_RMC1_RCR2_RMCLIEN                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC1->RCR2,31)))
#define TSB_RMC1_RCR3_RMCRP                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC1->RCR3,15)))
#define TSB_RMC1_RCR4_RMCPO                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC1->RCR4,7)))
#define TSB_RMC1_RSTAT_RMCRLDR                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_RMC1->RSTAT,7)))
#define TSB_RMC1_RSTAT_RMCRRP                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_RMC1->RSTAT,8)))
#define TSB_RMC1_RSTAT_RMCRRPIF                   (*((__I  uint32_t *)BITBAND_PERI(&TSB_RMC1->RSTAT,11)))
#define TSB_RMC1_RSTAT_RMCEDIF                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_RMC1->RSTAT,12)))
#define TSB_RMC1_RSTAT_RMCDMAXIF                  (*((__I  uint32_t *)BITBAND_PERI(&TSB_RMC1->RSTAT,13)))
#define TSB_RMC1_RSTAT_RMCLOIF                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_RMC1->RSTAT,14)))
#define TSB_RMC1_RSTAT_RMCRLIF                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_RMC1->RSTAT,15)))
#define TSB_RMC1_FSSEL_RMCCLK                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_RMC1->FSSEL,0)))


/*  */
#define TSB_PMD0_MDEN_PWMEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDEN,0)))
#define TSB_PMD0_MDCR_PINT                        (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDCR,3)))
#define TSB_PMD0_MDCR_DTYMD                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDCR,4)))
#define TSB_PMD0_MDCR_SYNTMD                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDCR,5)))
#define TSB_PMD0_MDCR_DCMEN                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDCR,6)))
#define TSB_PMD0_MDCR_DTCREN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDCR,7)))
#define TSB_PMD0_CARSTA_PWMUST                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_PMD0->CARSTA,0)))
#define TSB_PMD0_CARSTA_PWMVST                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_PMD0->CARSTA,1)))
#define TSB_PMD0_CARSTA_PWMWST                    (*((__I  uint32_t *)BITBAND_PERI(&TSB_PMD0->CARSTA,2)))
#define TSB_PMD0_MODESEL_MDSEL0                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MODESEL,0)))
#define TSB_PMD0_MODESEL_MDSEL1                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MODESEL,1)))
#define TSB_PMD0_MODESEL_MDSEL2                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MODESEL,2)))
#define TSB_PMD0_MODESEL_MDSEL3                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MODESEL,3)))
#define TSB_PMD0_MODESEL_DCMPEN                   (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MODESEL,7)))
#define TSB_PMD0_MDOUT_UPWM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDOUT,8)))
#define TSB_PMD0_MDOUT_VPWM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDOUT,9)))
#define TSB_PMD0_MDOUT_WPWM                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDOUT,10)))
#define TSB_PMD0_MDPOT_POLL                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDPOT,2)))
#define TSB_PMD0_MDPOT_POLH                       (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->MDPOT,3)))
#define TSB_PMD0_EMGCR_EMGEN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->EMGCR,0)))
#define TSB_PMD0_EMGCR_EMGRS                      (*((__O  uint32_t *)BITBAND_PERI(&TSB_PMD0->EMGCR,1)))
#define TSB_PMD0_EMGCR_EMGISEL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->EMGCR,2)))
#define TSB_PMD0_EMGCR_INHEN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->EMGCR,5)))
#define TSB_PMD0_EMGCR_EMGIPOL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->EMGCR,7)))
#define TSB_PMD0_EMGCR_CPAIEN                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->EMGCR,13)))
#define TSB_PMD0_EMGCR_CPBIEN                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->EMGCR,14)))
#define TSB_PMD0_EMGCR_CPCIEN                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->EMGCR,15)))
#define TSB_PMD0_EMGSTA_EMGST                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_PMD0->EMGSTA,0)))
#define TSB_PMD0_EMGSTA_EMGI                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_PMD0->EMGSTA,1)))
#define TSB_PMD0_OVVCR_OVVEN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->OVVCR,0)))
#define TSB_PMD0_OVVCR_OVVRS                      (*((__O  uint32_t *)BITBAND_PERI(&TSB_PMD0->OVVCR,1)))
#define TSB_PMD0_OVVCR_OVVISEL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->OVVCR,2)))
#define TSB_PMD0_OVVCR_ADIN0EN                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->OVVCR,5)))
#define TSB_PMD0_OVVCR_ADIN1EN                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->OVVCR,6)))
#define TSB_PMD0_OVVCR_OVVIPOL                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->OVVCR,7)))
#define TSB_PMD0_OVVCR_OVVRSMD                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->OVVCR,15)))
#define TSB_PMD0_OVVSTA_OVVST                     (*((__I  uint32_t *)BITBAND_PERI(&TSB_PMD0->OVVSTA,0)))
#define TSB_PMD0_OVVSTA_OVVI                      (*((__I  uint32_t *)BITBAND_PERI(&TSB_PMD0->OVVSTA,1)))
#define TSB_PMD0_TRGCR_TRG0BE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->TRGCR,3)))
#define TSB_PMD0_TRGCR_TRG1BE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->TRGCR,7)))
#define TSB_PMD0_TRGCR_TRG2BE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->TRGCR,11)))
#define TSB_PMD0_TRGCR_TRG3BE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->TRGCR,15)))
#define TSB_PMD0_TRGCR_CARSEL                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->TRGCR,16)))
#define TSB_PMD0_TRGMD_EMGTGE                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->TRGMD,0)))
#define TSB_PMD0_TRGMD_TRGOUT                     (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->TRGMD,1)))
#define TSB_PMD0_SYNCCR_PWMSMD                    (*((__IO uint32_t *)BITBAND_PERI(&TSB_PMD0->SYNCCR,0)))


/*  */
#define TSB_ISDA_EN_EN                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_ISDA->EN,0)))
#define TSB_ISDA_CLKCR_MS                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_ISDA->CLKCR,0)))
#define TSB_ISDA_CLKCR_SC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_ISDA->CLKCR,1)))
#define TSB_ISDA_OCR0_OP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_ISDA->OCR0,0)))
#define TSB_ISDA_CR_START                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_ISDA->CR,0)))
#define TSB_ISDA_BR_B0                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDA->BR,0)))
#define TSB_ISDA_BR_B1                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDA->BR,1)))
#define TSB_ISDA_BR_B2                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDA->BR,2)))
#define TSB_ISDA_BR_B3                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDA->BR,3)))
#define TSB_ISDA_SR_S0                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDA->SR,0)))
#define TSB_ISDA_SR_S1                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDA->SR,1)))
#define TSB_ISDA_SR_S2                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDA->SR,2)))
#define TSB_ISDA_SR_S3                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDA->SR,3)))
#define TSB_ISDA_INTCR_INTEN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ISDA->INTCR,0)))

#define TSB_ISDB_EN_EN                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_ISDB->EN,0)))
#define TSB_ISDB_CLKCR_MS                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_ISDB->CLKCR,0)))
#define TSB_ISDB_CLKCR_SC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_ISDB->CLKCR,1)))
#define TSB_ISDB_OCR0_OP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_ISDB->OCR0,0)))
#define TSB_ISDB_CR_START                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_ISDB->CR,0)))
#define TSB_ISDB_BR_B0                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDB->BR,0)))
#define TSB_ISDB_BR_B1                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDB->BR,1)))
#define TSB_ISDB_BR_B2                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDB->BR,2)))
#define TSB_ISDB_BR_B3                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDB->BR,3)))
#define TSB_ISDB_SR_S0                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDB->SR,0)))
#define TSB_ISDB_SR_S1                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDB->SR,1)))
#define TSB_ISDB_SR_S2                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDB->SR,2)))
#define TSB_ISDB_SR_S3                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDB->SR,3)))
#define TSB_ISDB_INTCR_INTEN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ISDB->INTCR,0)))

#define TSB_ISDC_EN_EN                            (*((__IO uint32_t *)BITBAND_PERI(&TSB_ISDC->EN,0)))
#define TSB_ISDC_CLKCR_MS                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_ISDC->CLKCR,0)))
#define TSB_ISDC_CLKCR_SC                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_ISDC->CLKCR,1)))
#define TSB_ISDC_OCR0_OP                          (*((__IO uint32_t *)BITBAND_PERI(&TSB_ISDC->OCR0,0)))
#define TSB_ISDC_CR_START                         (*((__IO uint32_t *)BITBAND_PERI(&TSB_ISDC->CR,0)))
#define TSB_ISDC_BR_B0                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDC->BR,0)))
#define TSB_ISDC_BR_B1                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDC->BR,1)))
#define TSB_ISDC_BR_B2                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDC->BR,2)))
#define TSB_ISDC_BR_B3                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDC->BR,3)))
#define TSB_ISDC_SR_S0                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDC->SR,0)))
#define TSB_ISDC_SR_S1                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDC->SR,1)))
#define TSB_ISDC_SR_S2                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDC->SR,2)))
#define TSB_ISDC_SR_S3                            (*((__I  uint32_t *)BITBAND_PERI(&TSB_ISDC->SR,3)))
#define TSB_ISDC_INTCR_INTEN                      (*((__IO uint32_t *)BITBAND_PERI(&TSB_ISDC->INTCR,0)))

/** @} */ /* End of group Device_Peripheral_registers */

#ifdef __cplusplus
}
#endif

#endif  /* __TMPM4G9_H__ */

/** @} */ /* End of group TMPM4G9 */
/** @} */ /* End of group TOSHIBA_TXZ_MICROCONTROLLER */
