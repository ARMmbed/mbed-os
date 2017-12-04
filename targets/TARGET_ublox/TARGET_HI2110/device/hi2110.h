/* mbed Microcontroller Library
 * Copyright (c) 2016 u-blox
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HI2110_H
#define HI2110_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "system_hi2110.h"

/******************************************************************************/
/*                Processor and Core Peripherals                              */
/******************************************************************************/

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */

typedef enum IRQn
{
/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
  Thread_mode                   = -16,      /*!<  0 Thread mode                                   */
  NonMaskableInt_IRQn           = -14,      /*!<  2 Non Maskable Interrupt                        */
  HardFault_IRQn                = -13,      /*!<  3 Hard Fault Interrupt                          */
  SVCall_IRQn                   = -5,       /*!< 11 SV Call Interrupt                             */
  PendSV_IRQn                   = -2,       /*!< 14 Pend SV Interrupt                             */
  SysTick_IRQn                  = -1,       /*!< 15 System Tick Interrupt                         */

/******  Device Specific Interrupt Numbers ********************************************************/
  RTC_IRQn                      = 0,        /*!< RTC Interrupt                                    */
  Timer_IRQn                    = 1,        /*!< Timer Interrupt                                  */
  Security_IRQn                 = 2,        /*!< From Security Interrupt                          */
  Protocol_IRQn                 = 3,        /*!< From Protocol Interrupt                          */
  Apps_IRQn                     = 4,        /*!< Core Self Interrupt                              */
  GPIO_IRQn                     = 5,        /*!< GPIO Interrupt                                   */
  DMA_IRQn                      = 6,        /*!< DMA Interrupt                                    */
  UART0_IRQn                    = 7,        /*!< UART0 Interrupt                                  */
  UART1_IRQn                    = 8,        /*!< UART1 Interrupt                                  */
  SSP0_IRQn                     = 9,        /*!< SPI0 Interrupt                                   */
  SSP1_IRQn                     = 10,       /*!< SPI1 Interrupt                                   */
  PWM0_Inner_IRQn               = 11,       /*!< PW0 Inner Interrupt                              */
  PWM0_Outer_IRQn               = 12,       /*!< PW0 Outer Interrupt                              */
  PWM1_Inner_IRQn               = 13,       /*!< PW1 Inner Interrupt                              */
  PWM1_Outer_IRQn               = 14,       /*!< PW1 Outer Interrupt                              */
  I2C_IRQn                      = 15,       /*!< I2C Interrupt                                    */
  LPUART_IRQn                   = 16,       /*!< Low Power UART Interrupt                         */
  CAP_IRQn                      = 17,       /*!< CAP Interrupt                                    */
  COMP_IRQn                     = 18,       /*!< COMP Interrupt                                   */
  EDGE_IRQn                     = 19,       /*!< EDGE Interrupt                                   */
  Pulse_SWD_IRQn                = 23,       /*!< SWD Pulse Interrupt                              */

} IRQn_Type;

/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M# Processor and Core Peripherals */
#define __CM0_REV                 0x0000    /*!< Core Revision r2p1                               */
#define __NVIC_PRIO_BITS          2         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */
#define __MPU_PRESENT             0         /*!< MPU present or not                               */
#define __FPU_PRESENT             0         /*!< FPU present or not                               */

#include <core_cm0.h>                       /* Cortex-M# processor and core peripherals */

/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/

/* UART */
typedef struct {
    uint32_t UARTDR;
    uint32_t UARTRSR;
    uint32_t res0;
    uint32_t res1;
    uint32_t res2;
    uint32_t res3;
    uint32_t UARTFR;
    uint32_t res4;
    uint32_t UARTILPR;
    uint32_t UARTIBRD; // Integer baud divider
    uint32_t UARTFBRD; // Fractional Baud divider
    uint32_t UARTLCR_H;
    uint32_t UARTCR;
    uint32_t UARTIFLS;
    uint32_t UARTIMSC;
    uint32_t UARTRIS;
    uint32_t UARTMIS;
    uint32_t UARTICR;
    uint32_t UARTDMACR;
} uart_ctrl_t;

/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/

#define RTC_IRQ_TIME_LSBS (*(volatile uint32_t *)(0x40002000))
#define RTC_IRQ_TIME_LSBS_BITSET (*(volatile uint32_t *)(0x40002400))
#define RTC_IRQ_TIME_LSBS_BITCLR (*(volatile uint32_t *)(0x40002800))
#define RTC_IRQ_TIME_LSBS_BITTOG (*(volatile uint32_t *)(0x40002C00))
#define RTC_IRQ_TIME_MSBS (*(volatile uint32_t *)(0x40002004))
#define RTC_IRQ_TIME_MSBS_BITSET (*(volatile uint32_t *)(0x40002404))
#define RTC_IRQ_TIME_MSBS_BITCLR (*(volatile uint32_t *)(0x40002804))
#define RTC_IRQ_TIME_MSBS_BITTOG (*(volatile uint32_t *)(0x40002C04))
#define RTC_IRQ_CLR (*(volatile uint32_t *)(0x40002008))
#define RTC_IRQ_CLR_BITSET (*(volatile uint32_t *)(0x40002408))
#define RTC_IRQ_CLR_BITCLR (*(volatile uint32_t *)(0x40002808))
#define RTC_IRQ_CLR_BITTOG (*(volatile uint32_t *)(0x40002C08))
#define TIMER0_LOAD (*(volatile uint32_t *)(0x4000200C))
#define TIMER0_LOAD_BITSET (*(volatile uint32_t *)(0x4000240C))
#define TIMER0_LOAD_BITCLR (*(volatile uint32_t *)(0x4000280C))
#define TIMER0_LOAD_BITTOG (*(volatile uint32_t *)(0x40002C0C))
#define TIMER0_CTRL (*(volatile uint32_t *)(0x40002010))
#define TIMER0_CTRL_BITSET (*(volatile uint32_t *)(0x40002410))
#define TIMER0_CTRL_BITCLR (*(volatile uint32_t *)(0x40002810))
#define TIMER0_CTRL_BITTOG (*(volatile uint32_t *)(0x40002C10))
#define TIMER0_TIME (*(volatile uint32_t *)(0x40002014))
#define ARM_IRQ_REG (*(volatile uint32_t *)(0x40002018))
#define ARM_IRQ_REG_BITSET (*(volatile uint32_t *)(0x40002418))
#define ARM_IRQ_REG_BITCLR (*(volatile uint32_t *)(0x40002818))
#define ARM_IRQ_REG_BITTOG (*(volatile uint32_t *)(0x40002C18))
#define PIO_FUNC0 (*(volatile uint32_t *)(0x4000201C))
#define PIO_FUNC0_BITSET (*(volatile uint32_t *)(0x4000241C))
#define PIO_FUNC0_BITCLR (*(volatile uint32_t *)(0x4000281C))
#define PIO_FUNC0_BITTOG (*(volatile uint32_t *)(0x40002C1C))
#define PIO_FUNC1 (*(volatile uint32_t *)(0x40002020))
#define PIO_FUNC1_BITSET (*(volatile uint32_t *)(0x40002420))
#define PIO_FUNC1_BITCLR (*(volatile uint32_t *)(0x40002820))
#define PIO_FUNC1_BITTOG (*(volatile uint32_t *)(0x40002C20))
#define PIO_FUNC2 (*(volatile uint32_t *)(0x40002024))
#define PIO_FUNC2_BITSET (*(volatile uint32_t *)(0x40002424))
#define PIO_FUNC2_BITCLR (*(volatile uint32_t *)(0x40002824))
#define PIO_FUNC2_BITTOG (*(volatile uint32_t *)(0x40002C24))
#define PIO_FUNC3 (*(volatile uint32_t *)(0x40002028))
#define PIO_FUNC3_BITSET (*(volatile uint32_t *)(0x40002428))
#define PIO_FUNC3_BITCLR (*(volatile uint32_t *)(0x40002828))
#define PIO_FUNC3_BITTOG (*(volatile uint32_t *)(0x40002C28))
#define PIO_FUNC4 (*(volatile uint32_t *)(0x4000202C))
#define PIO_FUNC4_BITSET (*(volatile uint32_t *)(0x4000242C))
#define PIO_FUNC4_BITCLR (*(volatile uint32_t *)(0x4000282C))
#define PIO_FUNC4_BITTOG (*(volatile uint32_t *)(0x40002C2C))
#define GPIO_DIR (*(volatile uint32_t *)(0x40002030))
#define GPIO_DIR_BITSET (*(volatile uint32_t *)(0x40002430))
#define GPIO_DIR_BITCLR (*(volatile uint32_t *)(0x40002830))
#define GPIO_DIR_BITTOG (*(volatile uint32_t *)(0x40002C30))
#define GPIO_OUT (*(volatile uint32_t *)(0x40002034))
#define GPIO_OUT_BITSET (*(volatile uint32_t *)(0x40002434))
#define GPIO_OUT_BITCLR (*(volatile uint32_t *)(0x40002834))
#define GPIO_OUT_BITTOG (*(volatile uint32_t *)(0x40002C34))
#define GPIO_DRIVE (*(volatile uint32_t *)(0x40002038))
#define GPIO_DRIVE_BITSET (*(volatile uint32_t *)(0x40002438))
#define GPIO_DRIVE_BITCLR (*(volatile uint32_t *)(0x40002838))
#define GPIO_DRIVE_BITTOG (*(volatile uint32_t *)(0x40002C38))
#define GPIO_PULLEN (*(volatile uint32_t *)(0x4000203C))
#define GPIO_PULLEN_BITSET (*(volatile uint32_t *)(0x4000243C))
#define GPIO_PULLEN_BITCLR (*(volatile uint32_t *)(0x4000283C))
#define GPIO_PULLEN_BITTOG (*(volatile uint32_t *)(0x40002C3C))
#define GPIO_INT_RISE (*(volatile uint32_t *)(0x40002040))
#define GPIO_INT_RISE_BITSET (*(volatile uint32_t *)(0x40002440))
#define GPIO_INT_RISE_BITCLR (*(volatile uint32_t *)(0x40002840))
#define GPIO_INT_RISE_BITTOG (*(volatile uint32_t *)(0x40002C40))
#define GPIO_INT_FALL (*(volatile uint32_t *)(0x40002044))
#define GPIO_INT_FALL_BITSET (*(volatile uint32_t *)(0x40002444))
#define GPIO_INT_FALL_BITCLR (*(volatile uint32_t *)(0x40002844))
#define GPIO_INT_FALL_BITTOG (*(volatile uint32_t *)(0x40002C44))
#define GPIO_INT_HIGH (*(volatile uint32_t *)(0x40002048))
#define GPIO_INT_HIGH_BITSET (*(volatile uint32_t *)(0x40002448))
#define GPIO_INT_HIGH_BITCLR (*(volatile uint32_t *)(0x40002848))
#define GPIO_INT_HIGH_BITTOG (*(volatile uint32_t *)(0x40002C48))
#define GPIO_INT_LOW (*(volatile uint32_t *)(0x4000204C))
#define GPIO_INT_LOW_BITSET (*(volatile uint32_t *)(0x4000244C))
#define GPIO_INT_LOW_BITCLR (*(volatile uint32_t *)(0x4000284C))
#define GPIO_INT_LOW_BITTOG (*(volatile uint32_t *)(0x40002C4C))
#define GPIO_INT_CLR (*(volatile uint32_t *)(0x40002050))
#define GPIO_INT_CLR_BITSET (*(volatile uint32_t *)(0x40002450))
#define GPIO_INT_CLR_BITCLR (*(volatile uint32_t *)(0x40002850))
#define GPIO_INT_CLR_BITTOG (*(volatile uint32_t *)(0x40002C50))
#define GPIO_VALUE (*(volatile uint32_t *)(0x40002054))
#define GPIO_IRQ (*(volatile uint32_t *)(0x40002058))
#define WDT_INTERVAL (*(volatile uint32_t *)(0x4000205C))
#define WDT_INTERVAL_BITSET (*(volatile uint32_t *)(0x4000245C))
#define WDT_INTERVAL_BITCLR (*(volatile uint32_t *)(0x4000285C))
#define WDT_INTERVAL_BITTOG (*(volatile uint32_t *)(0x40002C5C))
#define WDT_CTRL (*(volatile uint32_t *)(0x40002060))
#define WDT_CTRL_BITSET (*(volatile uint32_t *)(0x40002460))
#define WDT_CTRL_BITCLR (*(volatile uint32_t *)(0x40002860))
#define WDT_CTRL_BITTOG (*(volatile uint32_t *)(0x40002C60))
#define WDT_TIME (*(volatile uint32_t *)(0x40002064))
#define RESET_CAUSE (*(volatile uint32_t *)(0x40002134))
#define PWM0_CTRL (*(volatile uint32_t *)(0x40002068))
#define PWM0_CTRL_BITSET (*(volatile uint32_t *)(0x40002468))
#define PWM0_CTRL_BITCLR (*(volatile uint32_t *)(0x40002868))
#define PWM0_CTRL_BITTOG (*(volatile uint32_t *)(0x40002C68))
#define PWM0_COUNT (*(volatile uint32_t *)(0x4000206C))
#define PWM0_COUNT_BITSET (*(volatile uint32_t *)(0x4000246C))
#define PWM0_COUNT_BITCLR (*(volatile uint32_t *)(0x4000286C))
#define PWM0_COUNT_BITTOG (*(volatile uint32_t *)(0x40002C6C))
#define PWM1_CTRL (*(volatile uint32_t *)(0x40002070))
#define PWM1_CTRL_BITSET (*(volatile uint32_t *)(0x40002470))
#define PWM1_CTRL_BITCLR (*(volatile uint32_t *)(0x40002870))
#define PWM1_CTRL_BITTOG (*(volatile uint32_t *)(0x40002C70))
#define PWM1_COUNT (*(volatile uint32_t *)(0x40002074))
#define PWM1_COUNT_BITSET (*(volatile uint32_t *)(0x40002474))
#define PWM1_COUNT_BITCLR (*(volatile uint32_t *)(0x40002874))
#define PWM1_COUNT_BITTOG (*(volatile uint32_t *)(0x40002C74))
#define PWM_STATUS (*(volatile uint32_t *)(0x40002078))
#define CLKEN_REG (*(volatile uint32_t *)(0x4000207C))
#define CLKEN_REG_BITSET (*(volatile uint32_t *)(0x4000247C))
#define CLKEN_REG_BITCLR (*(volatile uint32_t *)(0x4000287C))
#define CLKEN_REG_BITTOG (*(volatile uint32_t *)(0x40002C7C))
#define I2C_INTERRUPT_STATUS (*(volatile uint32_t *)(0x40002080))
#define I2C_INTERRUPT_CLEAR (*(volatile uint32_t *)(0x40002084))
#define I2C_INTERRUPT_CLEAR_BITSET (*(volatile uint32_t *)(0x40002484))
#define I2C_INTERRUPT_CLEAR_BITCLR (*(volatile uint32_t *)(0x40002884))
#define I2C_INTERRUPT_CLEAR_BITTOG (*(volatile uint32_t *)(0x40002C84))
#define I2C_INTERRUPT_ENABLE (*(volatile uint32_t *)(0x40002088))
#define I2C_INTERRUPT_ENABLE_BITSET (*(volatile uint32_t *)(0x40002488))
#define I2C_INTERRUPT_ENABLE_BITCLR (*(volatile uint32_t *)(0x40002888))
#define I2C_INTERRUPT_ENABLE_BITTOG (*(volatile uint32_t *)(0x40002C88))
#define I2C_MODE (*(volatile uint32_t *)(0x4000208C))
#define I2C_MODE_BITSET (*(volatile uint32_t *)(0x4000248C))
#define I2C_MODE_BITCLR (*(volatile uint32_t *)(0x4000288C))
#define I2C_MODE_BITTOG (*(volatile uint32_t *)(0x40002C8C))
#define I2C_TX_DATA (*(volatile uint32_t *)(0x40002090))
#define I2C_TX_DATA_BITSET (*(volatile uint32_t *)(0x40002490))
#define I2C_TX_DATA_BITCLR (*(volatile uint32_t *)(0x40002890))
#define I2C_TX_DATA_BITTOG (*(volatile uint32_t *)(0x40002C90))
#define I2C_RX_DATA (*(volatile uint32_t *)(0x40002144))
#define I2C_TX_RD_WRB (*(volatile uint32_t *)(0x40002094))
#define I2C_TX_RD_WRB_BITSET (*(volatile uint32_t *)(0x40002494))
#define I2C_TX_RD_WRB_BITCLR (*(volatile uint32_t *)(0x40002894))
#define I2C_TX_RD_WRB_BITTOG (*(volatile uint32_t *)(0x40002C94))
#define I2C_TX_NO_BYTES (*(volatile uint32_t *)(0x40002098))
#define I2C_TX_NO_BYTES_BITSET (*(volatile uint32_t *)(0x40002498))
#define I2C_TX_NO_BYTES_BITCLR (*(volatile uint32_t *)(0x40002898))
#define I2C_TX_NO_BYTES_BITTOG (*(volatile uint32_t *)(0x40002C98))
#define I2C_RX_NO_BYTES (*(volatile uint32_t *)(0x4000209C))
#define I2C_RX_NO_BYTES_MASTER (*(volatile uint32_t *)(0x400020A0))
#define I2C_RX_NO_BYTES_MASTER_BITSET (*(volatile uint32_t *)(0x400024A0))
#define I2C_RX_NO_BYTES_MASTER_BITCLR (*(volatile uint32_t *)(0x400028A0))
#define I2C_RX_NO_BYTES_MASTER_BITTOG (*(volatile uint32_t *)(0x40002CA0))
#define I2C_GO (*(volatile uint32_t *)(0x400020A4))
#define I2C_GO_BITSET (*(volatile uint32_t *)(0x400024A4))
#define I2C_GO_BITCLR (*(volatile uint32_t *)(0x400028A4))
#define I2C_GO_BITTOG (*(volatile uint32_t *)(0x40002CA4))
#define I2C_RX_EARLY_THRESHOLD (*(volatile uint32_t *)(0x400020A8))
#define I2C_RX_EARLY_THRESHOLD_BITSET (*(volatile uint32_t *)(0x400024A8))
#define I2C_RX_EARLY_THRESHOLD_BITCLR (*(volatile uint32_t *)(0x400028A8))
#define I2C_RX_EARLY_THRESHOLD_BITTOG (*(volatile uint32_t *)(0x40002CA8))
#define I2C_RX_AUTO_NAG_BYTE_CNT (*(volatile uint32_t *)(0x400020AC))
#define I2C_RX_AUTO_NAG_BYTE_CNT_BITSET (*(volatile uint32_t *)(0x400024AC))
#define I2C_RX_AUTO_NAG_BYTE_CNT_BITCLR (*(volatile uint32_t *)(0x400028AC))
#define I2C_RX_AUTO_NAG_BYTE_CNT_BITTOG (*(volatile uint32_t *)(0x40002CAC))
#define I2C_HALF_TIME (*(volatile uint32_t *)(0x400020B0))
#define I2C_HALF_TIME_BITSET (*(volatile uint32_t *)(0x400024B0))
#define I2C_HALF_TIME_BITCLR (*(volatile uint32_t *)(0x400028B0))
#define I2C_HALF_TIME_BITTOG (*(volatile uint32_t *)(0x40002CB0))
#define I2C_ADDRESS (*(volatile uint32_t *)(0x400020B4))
#define I2C_ADDRESS_BITSET (*(volatile uint32_t *)(0x400024B4))
#define I2C_ADDRESS_BITCLR (*(volatile uint32_t *)(0x400028B4))
#define I2C_ADDRESS_BITTOG (*(volatile uint32_t *)(0x40002CB4))
#define I2C_ADDR_TYPE (*(volatile uint32_t *)(0x400020B8))
#define I2C_ADDR_TYPE_BITSET (*(volatile uint32_t *)(0x400024B8))
#define I2C_ADDR_TYPE_BITCLR (*(volatile uint32_t *)(0x400028B8))
#define I2C_ADDR_TYPE_BITTOG (*(volatile uint32_t *)(0x40002CB8))
#define I2C_SOFT_RESET (*(volatile uint32_t *)(0x400020BC))
#define I2C_SOFT_RESET_BITSET (*(volatile uint32_t *)(0x400024BC))
#define I2C_SOFT_RESET_BITCLR (*(volatile uint32_t *)(0x400028BC))
#define I2C_SOFT_RESET_BITTOG (*(volatile uint32_t *)(0x40002CBC))
#define I2C_SLAVE_RWB (*(volatile uint32_t *)(0x400020C0))
#define I2C_MASTER_SM (*(volatile uint32_t *)(0x400020C4))
#define I2C_SLAVE_SM (*(volatile uint32_t *)(0x400020C8))
#define I2C_SLAVE_ENABLE (*(volatile uint32_t *)(0x400020CC))
#define I2C_SLAVE_ENABLE_BITSET (*(volatile uint32_t *)(0x400024CC))
#define I2C_SLAVE_ENABLE_BITCLR (*(volatile uint32_t *)(0x400028CC))
#define I2C_SLAVE_ENABLE_BITTOG (*(volatile uint32_t *)(0x40002CCC))
#define I2C_MASTER_SEND_RESTART (*(volatile uint32_t *)(0x400020D0))
#define I2C_MASTER_SEND_RESTART_BITSET (*(volatile uint32_t *)(0x400024D0))
#define I2C_MASTER_SEND_RESTART_BITCLR (*(volatile uint32_t *)(0x400028D0))
#define I2C_MASTER_SEND_RESTART_BITTOG (*(volatile uint32_t *)(0x40002CD0))
#define DMA_MUX (*(volatile uint32_t *)(0x400020D4))
#define DMA_MUX_BITSET (*(volatile uint32_t *)(0x400024D4))
#define DMA_MUX_BITCLR (*(volatile uint32_t *)(0x400028D4))
#define DMA_MUX_BITTOG (*(volatile uint32_t *)(0x40002CD4))
#define DMA_CTRL_STAT (*(volatile uint32_t *)(0x400020D8))
#define COMP_CTRL (*(volatile uint32_t *)(0x400020DC))
#define COMP_CTRL_BITSET (*(volatile uint32_t *)(0x400024DC))
#define COMP_CTRL_BITCLR (*(volatile uint32_t *)(0x400028DC))
#define COMP_CTRL_BITTOG (*(volatile uint32_t *)(0x40002CDC))
#define COMP_STAT (*(volatile uint32_t *)(0x400020E0))
#define LP_UART_CTRL (*(volatile uint32_t *)(0x400020E4))
#define LP_UART_CTRL_BITSET (*(volatile uint32_t *)(0x400024E4))
#define LP_UART_CTRL_BITCLR (*(volatile uint32_t *)(0x400028E4))
#define LP_UART_CTRL_BITTOG (*(volatile uint32_t *)(0x40002CE4))
#define LP_UART_STATUS (*(volatile uint32_t *)(0x400020E8))
#define LP_UART_DATA (*(volatile uint32_t *)(0x40002154))
#define CAP_FILT_CONF (*(volatile uint32_t *)(0x400020EC))
#define CAP_FILT_CONF_BITSET (*(volatile uint32_t *)(0x400024EC))
#define CAP_FILT_CONF_BITCLR (*(volatile uint32_t *)(0x400028EC))
#define CAP_FILT_CONF_BITTOG (*(volatile uint32_t *)(0x40002CEC))
#define CAP_IRQ_CONF (*(volatile uint32_t *)(0x400020F0))
#define CAP_IRQ_CONF_BITSET (*(volatile uint32_t *)(0x400024F0))
#define CAP_IRQ_CONF_BITCLR (*(volatile uint32_t *)(0x400028F0))
#define CAP_IRQ_CONF_BITTOG (*(volatile uint32_t *)(0x40002CF0))
#define CAP_STATUS (*(volatile uint32_t *)(0x400020F4))
#define CORE_ENABLE_SWD_ACCESS_APPS (*(volatile uint32_t *)(0x400020F8))
#define CORE_ENABLE_SWD_ACCESS_APPS_BITSET (*(volatile uint32_t *)(0x400024F8))
#define CORE_ENABLE_SWD_ACCESS_APPS_BITCLR (*(volatile uint32_t *)(0x400028F8))
#define CORE_ENABLE_SWD_ACCESS_APPS_BITTOG (*(volatile uint32_t *)(0x40002CF8))
#define APPS_DEBUGGER_TO_CORE_DATA (*(volatile uint32_t *)(0x400020FC))
#define APPS_CORE_TO_DEBUGGER_DATA (*(volatile uint32_t *)(0x40002100))
#define APPS_CORE_TO_DEBUGGER_DATA_BITSET (*(volatile uint32_t *)(0x40002500))
#define APPS_CORE_TO_DEBUGGER_DATA_BITCLR (*(volatile uint32_t *)(0x40002900))
#define APPS_CORE_TO_DEBUGGER_DATA_BITTOG (*(volatile uint32_t *)(0x40002D00))
#define APPS_DEBUG_DATA_TO_CORE_AVAILABLE (*(volatile uint32_t *)(0x40002104))
#define APPS_DEBUG_DATA_TO_CORE_ACCEPTED (*(volatile uint32_t *)(0x40002108))
#define APPS_DEBUG_DATA_TO_CORE_ACCEPTED_BITSET (*(volatile uint32_t *)(0x40002508))
#define APPS_DEBUG_DATA_TO_CORE_ACCEPTED_BITCLR (*(volatile uint32_t *)(0x40002908))
#define APPS_DEBUG_DATA_TO_CORE_ACCEPTED_BITTOG (*(volatile uint32_t *)(0x40002D08))
#define APPS_CORE_DATA_TO_DEBUGGER_AVAILABLE (*(volatile uint32_t *)(0x4000210C))
#define APPS_CORE_DATA_TO_DEBUGGER_AVAILABLE_BITSET (*(volatile uint32_t *)(0x4000250C))
#define APPS_CORE_DATA_TO_DEBUGGER_AVAILABLE_BITCLR (*(volatile uint32_t *)(0x4000290C))
#define APPS_CORE_DATA_TO_DEBUGGER_AVAILABLE_BITTOG (*(volatile uint32_t *)(0x40002D0C))
#define APPS_CORE_DATA_TO_DEBUGGER_ACCEPTED (*(volatile uint32_t *)(0x40002110))
#define SWD_REQUEST (*(volatile uint32_t *)(0x40002114))
#define EDGE_CTRL0 (*(volatile uint32_t *)(0x40002118))
#define EDGE_CTRL0_BITSET (*(volatile uint32_t *)(0x40002518))
#define EDGE_CTRL0_BITCLR (*(volatile uint32_t *)(0x40002918))
#define EDGE_CTRL0_BITTOG (*(volatile uint32_t *)(0x40002D18))
#define EDGE_CTRL1 (*(volatile uint32_t *)(0x4000211C))
#define EDGE_CTRL1_BITSET (*(volatile uint32_t *)(0x4000251C))
#define EDGE_CTRL1_BITCLR (*(volatile uint32_t *)(0x4000291C))
#define EDGE_CTRL1_BITTOG (*(volatile uint32_t *)(0x40002D1C))
#define EDGE_COUNT (*(volatile uint32_t *)(0x40002120))
#define RESET_REG (*(volatile uint32_t *)(0x40002124))
#define RESET_REG_BITSET (*(volatile uint32_t *)(0x40002524))
#define RESET_REG_BITCLR (*(volatile uint32_t *)(0x40002924))
#define RESET_REG_BITTOG (*(volatile uint32_t *)(0x40002D24))
#define DIGITAL_VERSION (*(volatile uint32_t *)(0x40000000))
#define CLK_FREQ_DAC (*(volatile uint32_t *)(0x40000004))
#define CLK_FREQ_SET (*(volatile uint32_t *)(0x40000008))
#define CLK_FREQ_SET_BITSET (*(volatile uint32_t *)(0x40000408))
#define CLK_FREQ_SET_BITCLR (*(volatile uint32_t *)(0x40000808))
#define CLK_FREQ_SET_BITTOG (*(volatile uint32_t *)(0x40000C08))
#define CLK_FREQ_NREFCLKS (*(volatile uint32_t *)(0x4000000C))
#define CLK_FREQ_NREFCLKS_BITSET (*(volatile uint32_t *)(0x4000040C))
#define CLK_FREQ_NREFCLKS_BITCLR (*(volatile uint32_t *)(0x4000080C))
#define CLK_FREQ_NREFCLKS_BITTOG (*(volatile uint32_t *)(0x40000C0C))
#define CLK_FREQ_REF_SEL (*(volatile uint32_t *)(0x40000010))
#define CLK_FREQ_REF_SEL_BITSET (*(volatile uint32_t *)(0x40000410))
#define CLK_FREQ_REF_SEL_BITCLR (*(volatile uint32_t *)(0x40000810))
#define CLK_FREQ_REF_SEL_BITTOG (*(volatile uint32_t *)(0x40000C10))
#define CLK_FREQ_DIG_CLKS (*(volatile uint32_t *)(0x40000014))
#define CLK_FREQ_HIGHTARGET (*(volatile uint32_t *)(0x40000018))
#define CLK_FREQ_HIGHTARGET_BITSET (*(volatile uint32_t *)(0x40000418))
#define CLK_FREQ_HIGHTARGET_BITCLR (*(volatile uint32_t *)(0x40000818))
#define CLK_FREQ_HIGHTARGET_BITTOG (*(volatile uint32_t *)(0x40000C18))
#define CLK_FREQ_LOWTARGET (*(volatile uint32_t *)(0x4000001C))
#define CLK_FREQ_LOWTARGET_BITSET (*(volatile uint32_t *)(0x4000041C))
#define CLK_FREQ_LOWTARGET_BITCLR (*(volatile uint32_t *)(0x4000081C))
#define CLK_FREQ_LOWTARGET_BITTOG (*(volatile uint32_t *)(0x40000C1C))
#define CLK_FREQ_LP_BACKOFF (*(volatile uint32_t *)(0x40000020))
#define CLK_FREQ_LP_BACKOFF_BITSET (*(volatile uint32_t *)(0x40000420))
#define CLK_FREQ_LP_BACKOFF_BITCLR (*(volatile uint32_t *)(0x40000820))
#define CLK_FREQ_LP_BACKOFF_BITTOG (*(volatile uint32_t *)(0x40000C20))
#define CLK_FREQ_ENABLE (*(volatile uint32_t *)(0x40000024))
#define CLK_FREQ_ENABLE_BITSET (*(volatile uint32_t *)(0x40000424))
#define CLK_FREQ_ENABLE_BITCLR (*(volatile uint32_t *)(0x40000824))
#define CLK_FREQ_ENABLE_BITTOG (*(volatile uint32_t *)(0x40000C24))
#define CLK_GATE_SYS (*(volatile uint32_t *)(0x40000028))
#define CLK_GATE_SYS_BITSET (*(volatile uint32_t *)(0x40000428))
#define CLK_GATE_SYS_BITCLR (*(volatile uint32_t *)(0x40000828))
#define CLK_GATE_SYS_BITTOG (*(volatile uint32_t *)(0x40000C28))
#define CLK_GATE_MODEM (*(volatile uint32_t *)(0x4000002C))
#define CLK_GATE_MODEM_BITSET (*(volatile uint32_t *)(0x4000042C))
#define CLK_GATE_MODEM_BITCLR (*(volatile uint32_t *)(0x4000082C))
#define CLK_GATE_MODEM_BITTOG (*(volatile uint32_t *)(0x40000C2C))
#define CLK_GATE_RADIO (*(volatile uint32_t *)(0x40000030))
#define CLK_GATE_RADIO_BITSET (*(volatile uint32_t *)(0x40000430))
#define CLK_GATE_RADIO_BITCLR (*(volatile uint32_t *)(0x40000830))
#define CLK_GATE_RADIO_BITTOG (*(volatile uint32_t *)(0x40000C30))
#define CLK_GATE_DEBUG (*(volatile uint32_t *)(0x40000034))
#define CLK_GATE_DEBUG_BITSET (*(volatile uint32_t *)(0x40000434))
#define CLK_GATE_DEBUG_BITCLR (*(volatile uint32_t *)(0x40000834))
#define CLK_GATE_DEBUG_BITTOG (*(volatile uint32_t *)(0x40000C34))
#define CLK_GATE_RBIST (*(volatile uint32_t *)(0x40000038))
#define CLK_GATE_RBIST_BITSET (*(volatile uint32_t *)(0x40000438))
#define CLK_GATE_RBIST_BITCLR (*(volatile uint32_t *)(0x40000838))
#define CLK_GATE_RBIST_BITTOG (*(volatile uint32_t *)(0x40000C38))
#define LPC_CTRL (*(volatile uint32_t *)(0x4000003C))
#define LPC_CTRL_BITSET (*(volatile uint32_t *)(0x4000043C))
#define LPC_CTRL_BITCLR (*(volatile uint32_t *)(0x4000083C))
#define LPC_CTRL_BITTOG (*(volatile uint32_t *)(0x40000C3C))
#define LPC_TEST (*(volatile uint32_t *)(0x40000040))
#define LPC_TEST_BITSET (*(volatile uint32_t *)(0x40000440))
#define LPC_TEST_BITCLR (*(volatile uint32_t *)(0x40000840))
#define LPC_TEST_BITTOG (*(volatile uint32_t *)(0x40000C40))
#define FPGA_FLASH_WR (*(volatile uint32_t *)(0x40000044))
#define FPGA_FLASH_WR_BITSET (*(volatile uint32_t *)(0x40000444))
#define FPGA_FLASH_WR_BITCLR (*(volatile uint32_t *)(0x40000844))
#define FPGA_FLASH_WR_BITTOG (*(volatile uint32_t *)(0x40000C44))
#define FPGA_FLASH_RD (*(volatile uint32_t *)(0x40000048))
#define PMU_CTRL (*(volatile uint32_t *)(0x4000004C))
#define PMU_CTRL_BITSET (*(volatile uint32_t *)(0x4000044C))
#define PMU_CTRL_BITCLR (*(volatile uint32_t *)(0x4000084C))
#define PMU_CTRL_BITTOG (*(volatile uint32_t *)(0x40000C4C))
#define APP_CTRL0 (*(volatile uint32_t *)(0x40000050))
#define APP_CTRL0_BITSET (*(volatile uint32_t *)(0x40000450))
#define APP_CTRL0_BITCLR (*(volatile uint32_t *)(0x40000850))
#define APP_CTRL0_BITTOG (*(volatile uint32_t *)(0x40000C50))
#define APP_CTRL1 (*(volatile uint32_t *)(0x40000054))
#define APP_CTRL1_BITSET (*(volatile uint32_t *)(0x40000454))
#define APP_CTRL1_BITCLR (*(volatile uint32_t *)(0x40000854))
#define APP_CTRL1_BITTOG (*(volatile uint32_t *)(0x40000C54))
#define APP_CTRL2 (*(volatile uint32_t *)(0x40000058))
#define APP_CTRL2_BITSET (*(volatile uint32_t *)(0x40000458))
#define APP_CTRL2_BITCLR (*(volatile uint32_t *)(0x40000858))
#define APP_CTRL2_BITTOG (*(volatile uint32_t *)(0x40000C58))
#define APP_CTRL3 (*(volatile uint32_t *)(0x4000005C))
#define APP_CTRL3_BITSET (*(volatile uint32_t *)(0x4000045C))
#define APP_CTRL3_BITCLR (*(volatile uint32_t *)(0x4000085C))
#define APP_CTRL3_BITTOG (*(volatile uint32_t *)(0x40000C5C))
#define PMU_STAT (*(volatile uint32_t *)(0x40000060))
#define PMUBIST_ADC_CONF (*(volatile uint32_t *)(0x40000064))
#define PMUBIST_ADC_CONF_BITSET (*(volatile uint32_t *)(0x40000464))
#define PMUBIST_ADC_CONF_BITCLR (*(volatile uint32_t *)(0x40000864))
#define PMUBIST_ADC_CONF_BITTOG (*(volatile uint32_t *)(0x40000C64))
#define PMUBIST_ADC_DATA (*(volatile uint32_t *)(0x40000068))
#define STATUS (*(volatile uint32_t *)(0x4000006C))
#define LPC_STATUS (*(volatile uint32_t *)(0x40000070))
#define LPC_PDTIMER (*(volatile uint32_t *)(0x40000074))
#define LPC_PDTIMER_BITSET (*(volatile uint32_t *)(0x40000474))
#define LPC_PDTIMER_BITCLR (*(volatile uint32_t *)(0x40000874))
#define LPC_PDTIMER_BITTOG (*(volatile uint32_t *)(0x40000C74))
#define PIO_OWNER0 (*(volatile uint32_t *)(0x40000078))
#define PIO_OWNER1 (*(volatile uint32_t *)(0x4000007C))
#define RTC_TIME_LSBS (*(volatile uint32_t *)(0x40000080))
#define RTC_TIME_MSBS (*(volatile uint32_t *)(0x40000084))
#define DEBUG_SEL (*(volatile uint32_t *)(0x40000088))
#define DEBUG_SEL_BITSET (*(volatile uint32_t *)(0x40000488))
#define DEBUG_SEL_BITCLR (*(volatile uint32_t *)(0x40000888))
#define DEBUG_SEL_BITTOG (*(volatile uint32_t *)(0x40000C88))
#define FLASH_STATUS (*(volatile uint32_t *)(0x4000008C))
#define CHIP_WDT_INTERVAL (*(volatile uint32_t *)(0x40000090))
#define CHIP_WDT_INTERVAL_BITSET (*(volatile uint32_t *)(0x40000490))
#define CHIP_WDT_INTERVAL_BITCLR (*(volatile uint32_t *)(0x40000890))
#define CHIP_WDT_INTERVAL_BITTOG (*(volatile uint32_t *)(0x40000C90))
#define CHIP_WDT_CTRL (*(volatile uint32_t *)(0x40000094))
#define CHIP_WDT_CTRL_BITSET (*(volatile uint32_t *)(0x40000494))
#define CHIP_WDT_CTRL_BITCLR (*(volatile uint32_t *)(0x40000894))
#define CHIP_WDT_CTRL_BITTOG (*(volatile uint32_t *)(0x40000C94))
#define CHIP_WDT_TIME (*(volatile uint32_t *)(0x40000098))
#define CHIP_RESET (*(volatile uint32_t *)(0x4000009C))
#define CHIP_RESET_BITSET (*(volatile uint32_t *)(0x4000049C))
#define CHIP_RESET_BITCLR (*(volatile uint32_t *)(0x4000089C))
#define CHIP_RESET_BITTOG (*(volatile uint32_t *)(0x40000C9C))
#define SWD_PIN_CFG (*(volatile uint32_t *)(0x400000A0))
#define SWD_PIN_CFG_BITSET (*(volatile uint32_t *)(0x400004A0))
#define SWD_PIN_CFG_BITCLR (*(volatile uint32_t *)(0x400008A0))
#define SWD_PIN_CFG_BITTOG (*(volatile uint32_t *)(0x40000CA0))

/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/

/* UART Defines */
#define UART0_BASE 0x50003000
#define UART1_BASE 0x50004000

#ifdef __cplusplus
}
#endif

#endif  /* HI2110_H */
