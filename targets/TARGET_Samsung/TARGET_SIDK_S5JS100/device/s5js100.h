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

#ifndef S5JS100_H
#define S5JS100_H

#ifdef __cplusplus
extern "C" {
#endif
/* -------------------------  Interrupt Number Definition  ------------------------ */
#define S5JS100_IRQ_FIRST         (16) /* Vector number of the first external interrupt */
typedef enum IRQn {
    /* -------------------  Cortex-M7 Processor Exceptions Numbers  ------------------- */
    NonMaskableInt_IRQn           = -14,        /*  2 Non Maskable Interrupt          */
    HardFault_IRQn                = -13,        /*  3 HardFault Interrupt             */
    MemoryManagement_IRQn         = -12,        /*  4 Memory Management Interrupt     */
    BusFault_IRQn                 = -11,        /*  5 Bus Fault Interrupt             */
    UsageFault_IRQn               = -10,        /*  6 Usage Fault Interrupt           */
    SVCall_IRQn                   =  -5,        /* 11 SV Call Interrupt               */
    DebugMonitor_IRQn             =  -4,        /* 12 Debug Monitor Interrupt         */
    PendSV_IRQn                   =  -2,        /* 14 Pend SV Interrupt               */
    SysTick_IRQn                  =  -1,        /* 15 System Tick Interrupt           */
    /* ---------------------  S5JS100 Specific Interrupt Numbers  ---------------- */
    S5JS100_IRQ_WDG            = 0,
    S5JS100_IRQ_PMU_ApTimer    = 1,
    S5JS100_IRQ_PMU_AlivePad   = 2,
    S5JS100_IRQ_PMU_JtagInt    = 3,
    S5JS100_IRQ_SSS_SSSINT     = 4,
    S5JS100_IRQ_SSS_MBINT      = 5,
    S5JS100_IRQ_SSS_KMINT      = 6,
    S5JS100_IRQ_DMACINTR       = 7,
    S5JS100_IRQ_SDIO_INTREQ_L  = 8,
    S5JS100_IRQ_TINT0          = 9,
    S5JS100_IRQ_TINT1          = 10,
    S5JS100_IRQ_TINT2          = 11,
    S5JS100_IRQ_TINT3          = 12,
    S5JS100_IRQ_TINT4          = 13,
    S5JS100_IRQ_TINT5          = 14,
    S5JS100_IRQ_GPIO_INTR0     = 15,
    S5JS100_IRQ_GPIO_INTR1     = 16,
    S5JS100_IRQ_GPIO_INTR2     = 17,
    S5JS100_IRQ_USI0           = 18,
    S5JS100_IRQ_USI1           = 19,
    S5JS100_IRQ_SPI            = 20,
    S5JS100_IRQ_I2C            = 21,
    S5JS100_IRQ_PWM_INT0       = 22,
    S5JS100_IRQ_PWM_INT1       = 23,
    S5JS100_IRQ_PWM_INT2       = 24,
    S5JS100_IRQ_PWM_INT3       = 25,
    S5JS100_IRQ_PWM_INT4       = 26,
    S5JS100_IRQ_PPMU           = 27,
    S5JS100_IRQ_EFUSE_WR       = 28,
    S5JS100_IRQ_CM7_CTT_0      = 29,
    S5JS100_IRQ_CM7_CTT_1      = 30,
    S5JS100_IRQ_MAILBOX_AP_INT = 31,
    S5JS100_IRQ_UART0          = 32,
    S5JS100_IRQ_UART1          = 33,
    S5JS100_IRQ_GPADC          = 34,
    S5JS100_IRQ_MCPU_WDT       = 35,
    S5JS100_IRQ_SSS_INT1       = 36,
    S5JS100_IRQ_SSS_INT2       = 37,
    S5JS100_IRQ_SSS_RESET      = 38,
    S5JS100_IRQ_SLEEP          = 39,
    S5JS100_IRQ_TSU0           = 40,
    S5JS100_IRQ_TSU1           = 41,
} IRQn_Type;


/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

/* --------  Configuration of the Cortex-M7 Processor and Core Peripherals  ------- */
#define __CM7_REV                 0x0201U   /* Core revision r2p1 */
#define __MPU_PRESENT             1         /* MPU present */
#define __DCACHE_PRESENT          1       /* 32KB d-cache */
#define __ICACHE_PRESENT          1         /* 32KB I-cache */

#define __VTOR_PRESENT            1         /* VTOR present or not */
#define __NVIC_PRIO_BITS          3         /* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig    0         /* Set to 1 if different SysTick Config is used */

#include <core_cm7.h>                         /* Processor and core peripherals                  */
#include "system_s5js100.h"              /* System Header                                   */
/* ================================================================================ */
/* ================       Device Specific Peripheral Section       ================ */
/* ================================================================================ */

/* -------------------  Start of section using anonymous unions  ------------------ */
#if defined ( __CC_ARM   )
#pragma push
#pragma anon_unions
#elif defined(__ICCARM__)
#pragma language=extended
#elif defined(__GNUC__)
/* anonymous unions are enabled by default */
#elif defined(__TMS470__)
/* anonymous unions are enabled by default */
#elif defined(__TASKING__)
#pragma warning 586
#else
#warning Not supported compiler type
#endif

/*------------- Universal Asynchronous Receiver Transmitter (USI_UART) -----------*/
typedef struct {
    __IO   uint32_t  ULCON;                    /* Offset : 0x000 (R/W) Line Control Register */
    __IO   uint32_t  UCON;                    /* Offset: 0x004 (R/W) Control Register  */
    __IO   uint32_t  UFCON;                     /* Offset: 0x008 (R/W) FIFO Control Register */
    __IO   uint32_t  UMCON;                  /* Offset: 0x00C (R/W) Modem Control Register */
    __IO   uint32_t  UTRSTAT;                  /* Offset: 0x010 (R/W) Tx/Rx Status Register */
    __IO   uint32_t  UERSTAT;                  /* Offset: 0x014 (R) Rx Error Status Register */
    __IO   uint32_t  UFSTAT;                  /* Offset: 0x018 (R) FIFO Status Register */
    __IO   uint32_t  UMSTAT;                  /* Offset: 0x01C (R) Modem Status Register */
    __IO   uint32_t  UTXH;                  /* Offset: 0x020 (W) Transmit Buffer Register */
    __IO   uint32_t  URXH;                  /* Offset: 0x024 (R) Receive Buffer Register */
    __IO   uint32_t  UBRDIV;                  /* Offset: 0x028 (R/W) Baud Rate Divisor Register */
    __IO   uint32_t  UFRACVAL;                  /* Offset: 0x02C (R/W) Divisor Fractional value Register */
    __IO   uint32_t  UINTP;                  /* Offset: 0x030 (R/W) Interrupt Pending Register */
    __IO   uint32_t  UINTS;                  /* Offset: 0x034 (R) Interrupt Source Register */
    __IO   uint32_t  UINTM;                  /* Offset: 0x038 (R/W) Interrupt Mask Register */
    __IO     uint32_t  RESERVED0[1];
    __IO   uint32_t  UFTL_CONF;                  /* Offset: 0x040 (R/W) Filter Configuration Register */

} S5JS100_USI_UART_TypeDef;

/*------------- Universal Asynchronous Receiver Transmitter (UART) -----------*/
typedef struct {
    __IO uint32_t   DR;     // 0x0000
    __IO uint32_t   SR;     // 0x0004
    __IO uint32_t   RESERVED0[4];   // 0x008~0x014
    __IO uint32_t   FR;     // 0x018
    __IO uint32_t   RESERVED1;  // 0x01c
    __IO uint32_t   ILPR;       // 0x020
    __IO uint32_t   IBRD;       // 0x024
    __IO uint32_t   FBRD;       // 0x028
    __IO uint32_t   LCRH;       // 0x02c
    __IO uint32_t   CR;     // 0x030
    __IO uint32_t   IFLS;       // 0x034
    __IO uint32_t   IMSC;       // 0x038
    __IO uint32_t   RIS;        // 0x03C
    __IO uint32_t   MIS;        // 0x040
    __IO uint32_t   ICR;        // 0x044
    __IO uint32_t   DMACR;      // 0x048
    __IO uint32_t   MODESEL;    // 0x04C
} S5JS100_UART_TypeDef;
/* S5JS100 DATA Register Definitions */
/* Line Control register ****************************************************/
#define UART_ULCON_SAMPLING_SHIFT       7
#define UART_ULCON_SAMPLING_MASK        (0x1 << UART_ULCON_SAMPLING_SHIFT)
#define UART_ULCON_SAMPLING_16          (0x0 << UART_ULCON_SAMPLING_SHIFT)
#define UART_ULCON_SAMPLING_8           (0x1 << UART_ULCON_SAMPLING_SHIFT)

#define UART_ULCON_INFRARED_SHIFT       6
#define UART_ULCON_INFRARED_MASK        (0x1 << UART_ULCON_INFRARED_SHIFT)
#define UART_ULCON_INFRARED_NORMAL      (0x0 << UART_ULCON_INFRARED_SHIFT)
#define UART_ULCON_INFRARED_IRTXRX      (0x1 << UART_ULCON_INFRARED_SHIFT)

#define UART_ULCON_PARITY_SHIFT         5
#define UART_ULCON_PARITY_MASK          (0x7 << UART_ULCON_PARITY_SHIFT)
#define UART_ULCON_PARITY_NONE          (0x0 << UART_ULCON_PARITY_SHIFT)
#define UART_ULCON_PARITY_ODD           (0x4 << UART_ULCON_PARITY_SHIFT)
#define UART_ULCON_PARITY_EVEN          (0x5 << UART_ULCON_PARITY_SHIFT)
#define UART_ULCON_PARITY_FORCE1        (0x6 << UART_ULCON_PARITY_SHIFT)
#define UART_ULCON_PARITY_FORCE0        (0x7 << UART_ULCON_PARITY_SHIFT)

#define UART_ULCON_STOPBITS_SHIFT       2
#define UART_ULCON_STOPBITS_MASK        (0x1 << UART_ULCON_STOPBITS_SHIFT)
#define UART_ULCON_STOPBITS_1BIT        (0x0 << UART_ULCON_STOPBITS_SHIFT)
#define UART_ULCON_STOPBITS_2BITS       (0x1 << UART_ULCON_STOPBITS_SHIFT)

#define UART_ULCON_DATABITS_SHIFT       0
#define UART_ULCON_DATABITS_MASK        (0x3 << UART_ULCON_DATABITS_SHIFT)
#define UART_ULCON_DATABITS_5BITS       (0x0 << UART_ULCON_DATABITS_SHIFT)
#define UART_ULCON_DATABITS_6BITS       (0x1 << UART_ULCON_DATABITS_SHIFT)
#define UART_ULCON_DATABITS_7BITS       (0x2 << UART_ULCON_DATABITS_SHIFT)
#define UART_ULCON_DATABITS_8BITS       (0x3 << UART_ULCON_DATABITS_SHIFT)

/* Control register *********************************************************/
#define UART_UCON_TX_DMA_BURST_SHIFT    20
#define UART_UCON_TX_DMA_BURST_MASK     (0x7 << UART_UCON_TX_DMA_BURST_SHIFT)
#define UART_UCON_TX_DMA_BURST_1BYTE    (0x0 << UART_UCON_TX_DMA_BURST_SHIFT)
#define UART_UCON_TX_DMA_BURST_4BYTES   (0x1 << UART_UCON_TX_DMA_BURST_SHIFT)
#define UART_UCON_TX_DMA_BURST_8BYTES   (0x2 << UART_UCON_TX_DMA_BURST_SHIFT)

#define UART_UCON_RX_DMA_BURST_SHIFT    16
#define UART_UCON_RX_DMA_BURST_MASK     (0x7 << UART_UCON_RX_DMA_BURST_SHIFT)
#define UART_UCON_RX_DMA_BURST_1BYTE    (0x0 << UART_UCON_RX_DMA_BURST_SHIFT)
#define UART_UCON_RX_DMA_BURST_4BYTES   (0x1 << UART_UCON_RX_DMA_BURST_SHIFT)
#define UART_UCON_RX_DMA_BURST_8BYTES   (0x2 << UART_UCON_RX_DMA_BURST_SHIFT)
#define UART_UCON_RX_DMA_BURST_16BYTES  (0x3 << UART_UCON_RX_DMA_BURST_SHIFT)

#define UART_UCON_RX_TOUT_SHIFT         12
#define UART_UCON_RX_TOUT_MASK          (0x7 << UART_UCON_RX_TOUT_SHIFT)
#define UART_UCON_RX_TOUT_8FRAMES       (0x0 << UART_UCON_RX_TOUT_SHIFT)
#define UART_UCON_RX_TOUT_16FRAMES      (0x1 << UART_UCON_RX_TOUT_SHIFT)
#define UART_UCON_RX_TOUT_24FRAMES      (0x2 << UART_UCON_RX_TOUT_SHIFT)
#define UART_UCON_RX_TOUT_32FRAMES      (0x3 << UART_UCON_RX_TOUT_SHIFT)
#define UART_UCON_RX_TOUT_40FRAMES      (0x4 << UART_UCON_RX_TOUT_SHIFT)
#define UART_UCON_RX_TOUT_48FRAMES      (0x5 << UART_UCON_RX_TOUT_SHIFT)
#define UART_UCON_RX_TOUT_56FRAMES      (0x6 << UART_UCON_RX_TOUT_SHIFT)
#define UART_UCON_RX_TOUT_64FRAMES      (0x7 << UART_UCON_RX_TOUT_SHIFT)

#define UART_UCON_RX_FIFO_EMPTY_SHIFT   11
#define UART_UCON_RX_FIFO_EMPTY_MASK    (0x1 << UART_UCON_RX_FIFO_EMPTY_SHIFT)
#define UART_UCON_RX_FIFO_EMPTY_DISABLE (0x0 << UART_UCON_RX_FIFO_EMPTY_SHIFT)
#define UART_UCON_RX_FIFO_EMPTY_ENABLE  (0x1 << UART_UCON_RX_FIFO_EMPTY_SHIFT)

#define UART_UCON_RX_TOUT_DMAS_SHIFT    10
#define UART_UCON_RX_TOUT_DMAS_MASK     (0x1 << UART_UCON_RX_TOUT_DMAS_SHIFT)
#define UART_UCON_RX_TOUT_DMAS_DISABLE  (0x0 << UART_UCON_RX_TOUT_DMAS_SHIFT)
#define UART_UCON_RX_TOUT_DMAS_ENABLE   (0x1 << UART_UCON_RX_TOUT_DMAS_SHIFT)

#define UART_UCON_TX_INTTYPE_SHIFT             9
#define UART_UCON_TX_INTTYPE_MASK              (UART_UCON_TX_INTTYPE_SHIFT)
#define UART_UCON_TX_INTTYPE_PULSE             (0x0 << UART_UCON_TX_INTTYPE_SHIFT)
#define UART_UCON_TX_INTTYPE_LEVEL             (0x1 << UART_UCON_TX_INTTYPE_SHIFT)

#define UART_UCON_RX_INTTYPE_SHIFT             8
#define UART_UCON_RX_INTTYPE_MASK              (UART_UCON_RX_INTTYPE_SHIFT)
#define UART_UCON_RX_INTTYPE_PULSE             (0x0 << UART_UCON_RX_INTTYPE_SHIFT)
#define UART_UCON_RX_INTTYPE_LEVEL             (0x1 << UART_UCON_RX_INTTYPE_SHIFT)

#define UART_UCON_RX_TOUTINT_SHIFT      7
#define UART_UCON_RX_TOUTINT_MASK       (0x1 << UART_UCON_RX_TOUTINT_SHIFT)
#define UART_UCON_RX_TOUTINT_DISABLE    (0x0 << UART_UCON_RX_TOUTINT_SHIFT)
#define UART_UCON_RX_TOUTINT_ENABLE     (0x1 << UART_UCON_RX_TOUTINT_SHIFT)

#define UART_UCON_RX_ERRINT_SHIFT       6
#define UART_UCON_RX_ERRINT_MASK        (0x1 << UART_UCON_RX_ERRINT_SHIFT)
#define UART_UCON_RX_ERRINT_DISABLE     (0x0 << UART_UCON_RX_ERRINT_SHIFT)
#define UART_UCON_RX_ERRINT_ENABLE      (0x1 << UART_UCON_RX_ERRINT_SHIFT)

#define UART_UCON_LOOPBACK_SHIFT        5
#define UART_UCON_LOOPBACK_MASK         (0x1 << UART_UCON_LOOPBACK_SHIFT)
#define UART_UCON_LOOPBACK_DISABLE      (0x0 << UART_UCON_LOOPBACK_SHIFT)
#define UART_UCON_LOOPBACK_ENABLE       (0x1 << UART_UCON_LOOPBACK_SHIFT)

#define UART_UCON_SEND_BREAK_SHIFT      4
#define UART_UCON_SEND_BREAK            (0x1 << UART_UCON_SEND_BREAK_SHIFT)

#define UART_UCON_TX_MODE_SHIFT         2
#define UART_UCON_TX_MODE_MASK          (0x3 << UART_UCON_TX_MODE_SHIFT)
#define UART_UCON_TX_MODE_DISABLE       (0x0 << UART_UCON_TX_MODE_SHIFT)
#define UART_UCON_TX_MODE_IRQPOLL       (0x1 << UART_UCON_TX_MODE_SHIFT)
#define UART_UCON_TX_MODE_DMA           (0x2 << UART_UCON_TX_MODE_SHIFT)
#define UART_UCON_TX_MODE_RESERVED      (0x3 << UART_UCON_TX_MODE_SHIFT)

#define UART_UCON_RX_MODE_SHIFT         0
#define UART_UCON_RX_MODE_MASK          (0x3 << UART_UCON_RX_MODE_SHIFT)
#define UART_UCON_RX_MODE_DISABLE       (0x0 << UART_UCON_RX_MODE_SHIFT)
#define UART_UCON_RX_MODE_IRQPOLL       (0x1 << UART_UCON_RX_MODE_SHIFT)
#define UART_UCON_RX_MODE_DMA           (0x2 << UART_UCON_RX_MODE_SHIFT)
#define UART_UCON_RX_MODE_RESERVED      (0x3 << UART_UCON_RX_MODE_SHIFT)

/* FIFO Control register ****************************************************/
#define UART_UFCON_TX_FIFO_TRIG_SHIFT   8
#define UART_UFCON_TX_FIFO_TRIG_MASK    (0x7 << UART_UFCON_TX_FIFO_TRIG_SHIFT)
#define UART_UFCON_TX_FIFO_TRIG_0BYTE   (0x0 << UART_UFCON_TX_FIFO_TRIG_SHIFT)
#define UART_UFCON_TX_FIFO_TRIG_2BYTES  (0x1 << UART_UFCON_TX_FIFO_TRIG_SHIFT)
#define UART_UFCON_TX_FIFO_TRIG_4BYTES  (0x2 << UART_UFCON_TX_FIFO_TRIG_SHIFT)
#define UART_UFCON_TX_FIFO_TRIG_6BYTES  (0x3 << UART_UFCON_TX_FIFO_TRIG_SHIFT)
#define UART_UFCON_TX_FIFO_TRIG_8BYTES  (0x4 << UART_UFCON_TX_FIFO_TRIG_SHIFT)
#define UART_UFCON_TX_FIFO_TRIG_10BYTES (0x4 << UART_UFCON_TX_FIFO_TRIG_SHIFT)
#define UART_UFCON_TX_FIFO_TRIG_12BYTES (0x5 << UART_UFCON_TX_FIFO_TRIG_SHIFT)
#define UART_UFCON_TX_FIFO_TRIG_14BYTES (0x6 << UART_UFCON_TX_FIFO_TRIG_SHIFT)

#define UART_UFCON_RX_FIFO_TRIG_SHIFT   4
#define UART_UFCON_RX_FIFO_TRIG_MASK    (0x7 << UART_UFCON_RX_FIFO_TRIG_SHIFT)
#define UART_UFCON_RX_FIFO_TRIG_0BYTE   (0x0 << UART_UFCON_RX_FIFO_TRIG_SHIFT)
#define UART_UFCON_RX_FIFO_TRIG_2BYTES  (0x1 << UART_UFCON_RX_FIFO_TRIG_SHIFT)
#define UART_UFCON_RX_FIFO_TRIG_4BYTES  (0x2 << UART_UFCON_RX_FIFO_TRIG_SHIFT)
#define UART_UFCON_RX_FIFO_TRIG_6BYTES  (0x3 << UART_UFCON_RX_FIFO_TRIG_SHIFT)
#define UART_UFCON_RX_FIFO_TRIG_8BYTES  (0x4 << UART_UFCON_RX_FIFO_TRIG_SHIFT)
#define UART_UFCON_RX_FIFO_TRIG_10BYTES (0x4 << UART_UFCON_RX_FIFO_TRIG_SHIFT)
#define UART_UFCON_RX_FIFO_TRIG_12BYTES (0x5 << UART_UFCON_RX_FIFO_TRIG_SHIFT)
#define UART_UFCON_RX_FIFO_TRIG_14BYTES (0x6 << UART_UFCON_RX_FIFO_TRIG_SHIFT)

#define UART_UFCON_TX_FIFO_RESET_SHIFT  2
#define UART_UFCON_TX_FIFO_RESET        (0x1 << UART_UFCON_TX_FIFO_RESET_SHIFT)

#define UART_UFCON_RX_FIFO_RESET_SHIFT  1
#define UART_UFCON_RX_FIFO_RESET        (0x1 << UART_UFCON_RX_FIFO_RESET_SHIFT)

#define UART_UFCON_FIFO_SHIFT           0
#define UART_UFCON_FIFO_MASK            (0x1 << UART_UFCON_FIFO_SHIFT)
#define UART_UFCON_FIFO_DISABLE         (0x0 << UART_UFCON_FIFO_SHIFT)
#define UART_UFCON_FIFO_ENABLE          (0x1 << UART_UFCON_FIFO_SHIFT)

/* Modem Control register ***************************************************/
#define UART_UMCON_RTS_TRIG_SHIFT       5
#define UART_UMCON_RTS_TRIG_MASK        (0x7 << UART_UMCON_RTS_TRIG_SHIFT)
#define UART_UMCON_RTS_TRIG_15BYTES     (0x0 << UART_UMCON_RTS_TRIG_SHIFT)
#define UART_UMCON_RTS_TRIG_14BYTES     (0x1 << UART_UMCON_RTS_TRIG_SHIFT)
#define UART_UMCON_RTS_TRIG_12BYTES     (0x2 << UART_UMCON_RTS_TRIG_SHIFT)
#define UART_UMCON_RTS_TRIG_10BYTES     (0x3 << UART_UMCON_RTS_TRIG_SHIFT)
#define UART_UMCON_RTS_TRIG_8BYTES      (0x4 << UART_UMCON_RTS_TRIG_SHIFT)
#define UART_UMCON_RTS_TRIG_6BYTES      (0x5 << UART_UMCON_RTS_TRIG_SHIFT)
#define UART_UMCON_RTS_TRIG_4BYTES      (0x6 << UART_UMCON_RTS_TRIG_SHIFT)
#define UART_UMCON_RTS_TRIG_2BYTES      (0x7 << UART_UMCON_RTS_TRIG_SHIFT)

#define UART_UMCON_AFC_SHIFT            4
#define UART_UMCON_AFC_MASK             (0x1 << UART_UMCON_AFC_SHIFT)
#define UART_UMCON_AFC_DISABLE          (0x0 << UART_UMCON_AFC_SHIFT)
#define UART_UMCON_AFC_ENABLE           (0x1 << UART_UMCON_AFC_SHIFT)

#define UART_UMCON_MODEM_INT_SHIFT      3
#define UART_UMCON_MODEM_INT_MASK       (0x1 << UART_UMCON_MODEM_INT_SHIFT)
#define UART_UMCON_MODEM_INT_DISABLE    (0x0 << UART_UMCON_MODEM_INT_SHIFT)
#define UART_UMCON_MODEM_INT_ENABLE     (0x1 << UART_UMCON_MODEM_INT_SHIFT)

#define UART_UMCON_REQ_TO_SEND_SHIFT    0
#define UART_UMCON_REQ_TO_SEND_MASK     (0x1 << UART_UMCON_REQ_TO_SEND_SHIFT)
#define UART_UMCON_REQ_TO_SEND_HIGH     (0x0 << UART_UMCON_REQ_TO_SEND_SHIFT)
#define UART_UMCON_REQ_TO_SEND_LOW      (0x1 << UART_UMCON_REQ_TO_SEND_SHIFT)

/* Tx/Rx Status register ****************************************************/
#define UART_UTRSTAT_RX_FIFO_CNT_SHIFT  16
#define UART_UTRSTAT_RX_FIFO_CNT_MASK   (0xff << UART_UTRSTAT_RXFIFO_CNT_SHIFT)

#define UART_UTRSTAT_TX_DMA_SHIFT       12
#define UART_UTRSTAT_TX_DMA_MASK        (0xf << UART_UTRSTAT_TX_DMA_SHIFT)
#define UART_UTRSTAT_TX_DMA_IDLE        (0x0 << UART_UTRSTAT_TX_DMA_SHIFT)
#define UART_UTRSTAT_TX_DMA_BURST_REQ   (0x1 << UART_UTRSTAT_TX_DMA_SHIFT)
#define UART_UTRSTAT_TX_DMA_BURST_ACK   (0x2 << UART_UTRSTAT_TX_DMA_SHIFT)
#define UART_UTRSTAT_TX_DMA_BURST_NEXT  (0x3 << UART_UTRSTAT_TX_DMA_SHIFT)
#define UART_UTRSTAT_TX_DMA_SINGLE_REQ  (0x4 << UART_UTRSTAT_TX_DMA_SHIFT)
#define UART_UTRSTAT_TX_DMA_SINGLE_ACK  (0x5 << UART_UTRSTAT_TX_DMA_SHIFT)
#define UART_UTRSTAT_TX_DMA_SINGLE_NEXT (0x6 << UART_UTRSTAT_TX_DMA_SHIFT)
#define UART_UTRSTAT_TX_DMA_LBURST_REQ  (0x7 << UART_UTRSTAT_TX_DMA_SHIFT)
#define UART_UTRSTAT_TX_DMA_LBURST_ACK  (0x8 << UART_UTRSTAT_TX_DMA_SHIFT)
#define UART_UTRSTAT_TX_DMA_LSINGLE_REQ (0x9 << UART_UTRSTAT_TX_DMA_SHIFT)
#define UART_UTRSTAT_TX_DMA_LSINGLE_ACK (0xa << UART_UTRSTAT_TX_DMA_SHIFT)

#define UART_UTRSTAT_RX_DMA_SHIFT       8
#define UART_UTRSTAT_RX_DMA_MASK        (0xf << UART_UTRSTAT_RX_DMA_SHIFT)
#define UART_UTRSTAT_RX_DMA_IDLE        (0x0 << UART_UTRSTAT_RX_DMA_SHIFT)
#define UART_UTRSTAT_RX_DMA_BURST_REQ   (0x1 << UART_UTRSTAT_RX_DMA_SHIFT)
#define UART_UTRSTAT_RX_DMA_BURST_ACK   (0x2 << UART_UTRSTAT_RX_DMA_SHIFT)
#define UART_UTRSTAT_RX_DMA_BURST_NEXT  (0x3 << UART_UTRSTAT_RX_DMA_SHIFT)
#define UART_UTRSTAT_RX_DMA_SINGLE_REQ  (0x4 << UART_UTRSTAT_RX_DMA_SHIFT)
#define UART_UTRSTAT_RX_DMA_SINGLE_ACK  (0x5 << UART_UTRSTAT_RX_DMA_SHIFT)
#define UART_UTRSTAT_RX_DMA_SINGLE_NEXT (0x6 << UART_UTRSTAT_RX_DMA_SHIFT)
#define UART_UTRSTAT_RX_DMA_LBURST_REQ  (0x7 << UART_UTRSTAT_RX_DMA_SHIFT)
#define UART_UTRSTAT_RX_DMA_LBURST_ACK  (0x8 << UART_UTRSTAT_RX_DMA_SHIFT)
#define UART_UTRSTAT_RX_DMA_LSINGLE_REQ (0x9 << UART_UTRSTAT_RX_DMA_SHIFT)
#define UART_UTRSTAT_RX_DMA_LSINGLE_ACK (0xa << UART_UTRSTAT_RX_DMA_SHIFT)

#define UART_UTRSTAT_RX_TOUT_SHIFT      3
#define UART_UTRSTAT_RX_TOUT_MASK       (0x1 << UART_UTRSTAT_RX_TOUT_SHIFT)
#define UART_UTRSTAT_RX_TOUT_NONE       (0x0 << UART_UTRSTAT_RX_TOUT_SHIFT)
#define UART_UTRSTAT_RX_TOUT_TIMEOUT    (0x1 << UART_UTRSTAT_RX_TOUT_SHIFT)

#define UART_UTRSTAT_TX_SHIFT           2
#define UART_UTRSTAT_TX_MASK            (0x1 << UART_UTRSTAT_TX_SHIFT)
#define UART_UTRSTAT_TX_NOT_EMPTY       (0x0 << UART_UTRSTAT_TX_SHIFT)
#define UART_UTRSTAT_TX_EMPTY           (0x1 << UART_UTRSTAT_TX_SHIFT)

#define UART_UTRSTAT_TX_BUF_SHIFT       1
#define UART_UTRSTAT_TX_BUF_MASK        (0x1 << UART_UTRSTAT_TX_BUF_SHIFT)
#define UART_UTRSTAT_TX_BUF_NOT_EMPTY   (0x0 << UART_UTRSTAT_TX_BUF_SHIFT)
#define UART_UTRSTAT_TX_BUF_EMPTY       (0x1 << UART_UTRSTAT_TX_BUF_SHIFT)

#define UART_UTRSTAT_RX_BUF_SHIFT       0
#define UART_UTRSTAT_RX_BUF_MASK        (0x1 << UART_UTRSTAT_RX_BUF_SHIFT)
#define UART_UTRSTAT_RX_BUF_NOT_EMPTY   (0x1 << UART_UTRSTAT_RX_BUF_SHIFT)
#define UART_UTRSTAT_RX_BUF_EMPTY       (0x0 << UART_UTRSTAT_RX_BUF_SHIFT)

/* Rx Error Status register *************************************************/
#define UART_UERSTAT_BREAK_DETECT       0x8
#define UART_UERSTAT_FRAME_ERROR        0x4
#define UART_UERSTAT_PARITY_ERROR       0x2
#define UART_UERSTAT_OVERRUN_ERROR      0x1

/* FIFO status register *****************************************************/
#define UART_UFSTAT_TX_FIFO_FULL_SHIFT  24
#define UART_UFSTAT_TX_FIFO_FULL_MASK   (0x1 << UART_UFSTAT_TX_FIFO_FULL_SHIFT)
#define UART_UFSTAT_TX_FIFO_FULL        (0x1 << UART_UFSTAT_TX_FIFO_FULL_SHIFT)

#define UART_UFSTAT_TX_FIFO_COUNT_SHIFT 16
#define UART_UFSTAT_TX_FIFO_COUNT_MASK  (0xff << UART_UFSTAT_TX_FIFO_COUNT_SHIFT)

#define UART_UFSTAT_RX_FIFO_ERROR_SHIFT 9
#define UART_UFSTAT_RX_FIFO_ERROR_MASK  (0x1 << UART_UFSTAT_RX_FIFO_ERROR_SHIFT)
#define UART_UFSTAT_RX_FIFO_ERROR       (0x1 << UART_UFSTAT_RX_FIFO_ERROR_SHIFT)

#define UART_UFSTAT_RX_FIFO_FULL_SHIFT  8
#define UART_UFSTAT_RX_FIFO_FULL_MASK       (0x1 << UART_UFSTAT_RX_FIFO_FULL_SHIFT)
#define UART_UFSTAT_RX_FIFO_FULL        (0x1 << UART_UFSTAT_RX_FIFO_FULL_SHIFT)

#define UART_UFSTAT_RX_FIFO_COUNT_SHIFT 0
#define UART_UFSTAT_RX_FIFO_COUNT_MASK  (0xff << UART_UFSTAT_RX_FIFO_COUNT_SHIFT)

/* Modem Status register ****************************************************/
#define UART_UMSTAT_DELTA_CTS_SHIFT     4
#define UART_UMSTAT_DELTA_CTS_MASK      (0x1 << UART_UMSTAT_DELTA_CTS_SHIFT)
#define UART_UMSTAT_DELTA_CTS_NOCHANGE  (0x0 << UART_UMSTAT_DELTA_CTS_SHIFT)
#define UART_UMSTAT_DELTA_CTS_CHANGED   (0x1 << UART_UMSTAT_DELTA_CTS_SHIFT)

#define UART_UMSTAT_NCTS_SHIFT          0
#define UART_UMSTAT_NCTS_MASK           (0x1 << UART_UMSTAT_NCTS_SHIFT)
#define UART_UMSTAT_NCTS_HIGH           (0x0 << UART_UMSTAT_NCTS_SHIFT)
#define UART_UMSTAT_NCTS_LOW            (0x1 << UART_UMSTAT_NCTS_SHIFT)

/* INterrupt Pending register ***********************************************/
#define UART_UINTP_MODEM_SHIFT          3
#define UART_UINTP_MODEM_MASK           (0x1 << UART_UINTP_MODEM_SHIFT)
#define UART_UINTP_MODEM                (0x1 << UART_UINTP_MODEM_SHIFT)

#define UART_UINTP_TXD_SHIFT            2
#define UART_UINTP_TXD_MASK             (0x1 << UART_UINTP_TXD_SHIFT)
#define UART_UINTP_TXD                  (0x1 << UART_UINTP_TXD_SHIFT)

#define UART_UINTP_ERROR_SHIFT          1
#define UART_UINTP_ERROR_MASK           (0x1 << UART_UINTP_ERROR_SHIFT)
#define UART_UINTP_ERROR                (0x1 << UART_UINTP_ERROR_SHIFT)

#define UART_UINTP_RXD_SHIFT            0
#define UART_UINTP_RXD_MASK             (0x1 << UART_UINTP_RXD_SHIFT)
#define UART_UINTP_RXD                  (0x1 << UART_UINTP_RXD_SHIFT)

/* Interrupt Source register ************************************************/
#define UART_UINTS_MODEM_SHIFT          3
#define UART_UINTS_MODEM_MASK           (0x1 << UART_UINTS_MODEM_SHIFT)
#define UART_UINTS_MODEM                (0x1 << UART_UINTS_MODEM_SHIFT)

#define UART_UINTS_TXD_SHIFT            2
#define UART_UINTS_TXD_MASK             (0x1 << UART_UINTS_TXD_SHIFT)
#define UART_UINTS_TXD                  (0x1 << UART_UINTS_TXD_SHIFT)

#define UART_UINTS_ERROR_SHIFT          1
#define UART_UINTS_ERROR_MASK           (0x1 << UART_UINTS_ERROR_SHIFT)
#define UART_UINTS_ERROR                (0x1 << UART_UINTS_ERROR_SHIFT)

#define UART_UINTS_RXD_SHIFT            0
#define UART_UINTS_RXD_MASK             (0x1 << UART_UINTS_RXD_SHIFT)
#define UART_UINTS_RXD                  (0x1 << UART_UINTS_RXD_SHIFT)

/* Interrupt Mask register **************************************************/
#define UART_UINTM_MODEM_SHIFT          3
#define UART_UINTM_MODEM_MASK           (0x1 << UART_UINTM_MODEM_SHIFT)

#define UART_UINTM_TXD_SHIFT            2
#define UART_UINTM_TXD_MASK             (0x1 << UART_UINTM_TXD_SHIFT)

#define UART_UINTM_ERROR_SHIFT          1
#define UART_UINTM_ERROR_MASK           (0x1 << UART_UINTM_ERROR_SHIFT)

#define UART_UINTM_RXD_SHIFT            0
#define UART_UINTM_RXD_MASK             (0x1 << UART_UINTM_RXD_SHIFT)


/*----------------------------- S5JS100 ATimer (TIMER) -------------------------------*/
typedef struct {
    __IO   uint32_t  LOAD_VALUE;    /* 0x0 */
    __IO   uint32_t  CTRL;              /* 0x4 */
    __IO   uint32_t  LOAD_CON_VALUE;    /* 0x8 */
    __IO   uint32_t  INT_STATUS;        /* 0xC */

    __IO   uint32_t  INT_CLEAR;     /* 0x10 */
    __IO   uint32_t  INT_ENABLE;        /* 0x14 */
    __IO   uint32_t  RSVD3;         /* 0x18 */
    __IO   uint32_t  RSVD4;         /* 0x1C */

    __IO   uint32_t  RSVD5;         /* 0x20 */
    __IO   uint32_t  RSVD6;         /* 0x24 */
    __IO   uint32_t  RSVD7;         /* 0x28 */
    __IO   uint32_t  RSVD8;         /* 0x2C */

    __IO   uint32_t  RSVD9;         /* 0x30 */
    __IO   uint32_t  CNT_VALUE;         /* 0x34 */
    __IO   uint32_t  RSVD10;            /* 0x38 */
    __IO   uint32_t  RSVD11;            /* 0x3C */
} S5JS100_TIMER_TypeDef;

/*----------------------------- S5JS100 USI (UART/SPI/I2C) -------------------------------*/
typedef struct {
    __IO uint32_t   CONFIG;     // 0x0000
    __IO uint32_t   CON;     // 0x0004
    __IO uint32_t   OPTION;   // 0x008~0x014
    __IO uint32_t   VERSION;     // 0x018
    __IO uint32_t   UART_VERSION;  // 0x01c
    __IO uint32_t   SPI_VERSION;       // 0x020
    __IO uint32_t   I2C_VERSION;       // 0x024
    __IO uint32_t   FIFO_DEPTH;       // 0x028
} S5JS100_USI_TypeDef;

#define USI_CONFIG_UART             1
#define USI_CONFIG_SPI              2
#define USI_CONFIG_I2C              4

#define USI_CON_RST             1

#define USI_OPTION_MASTER           1
#define USI_OPTION_HWACG(x)         ((0x3 & (x)) << 1)

#define USI_VERSION_HW(x)           ((x) & 0xFF)
#define USI_VERSION_SW(x)           (((x) >> 8) & 0xFF)
#define USI_VERSION_UART(x)         (((x) >> 16) & 1)
#define USI_VERSION_SPI(x)          (((x) >> 17) & 1)
#define USI_VERSION_USI(x)          (((x) >> 24) & 0xFF)

#define USI_TXFIFO(x)               (((x) >> 16) & 0x1FF)
#define USI_RXFIFO(x)               ((x) & 0x1FF)


/*-------------------- General Purpose Input Output (GPIO) -------------------*/
/*------------------- Watchdog ----------------------------------------------*/
/* --------------------  End of section using anonymous unions  ------------------- */
#if defined ( __CC_ARM   )
#pragma pop
#elif defined(__ICCARM__)
/* leave anonymous unions enabled */
#elif defined(__GNUC__)
/* anonymous unions are enabled by default */
#elif defined(__TMS470__)
/* anonymous unions are enabled by default */
#elif defined(__TASKING__)
#pragma warning restore
#else
#warning Not supported compiler type
#endif




/* ================================================================================ */
/* ================              Peripheral memory map             ================ */
/* ================================================================================ */

/* Peripheral and SRAM base address                                                 */
#define S5JS100_FLASH_BASE      0x00000000
#define S5JS100_TIMER0_BASE     0x83012000
#define S5JS100_TIMER1_BASE     0x83012100
#define S5JS100_USI0_BASE       0x83014000
#define S5JS100_USI1_BASE       0x83015000
#define S5JS100_ACPU_SFR_PADDR  0x80000000
#define S5JS100_MCPU_ROM        0x80140000
#define S5JS100_MCPU_Debug      0x80150000
#define S5JS100_MCPU_CTI        0x80158000
#define S5JS100_PMU_ALIVE       0x81000000
#define S5JS100_SCMU            0x82000000
#define S5JS100_EFUSE           0x82010000
#define S5JS100_PMU_SYS         0x82020000
#define S5JS100_REG_PAD_CONTROL 0x82021000
#define S5JS100_ACMU            0x83000000
#define S5JS100_WDT_BASE        0x83011000
#define S5JS100_ATIMER          0x83012000
#define S5JS100_SDIOC           0x83020000
#define S5JS100_USI0            0x83014000
#define S5JS100_USI1            0x83015000
#define S5JS100_SPI             0x83016000
#define S5JS100_I2C             0x83017000
#define S5JS100_PWM             0x83018000
#define S5JS100_SSS_SS          0x83100000
#define S5JS100_SSS_MB          0x83110000
#define S5JS100_SSS_KM          0x83120000
#define S5JS100_PUF             0x83130000
#define S5JS100_PDMAC           0x83200000
#define S5JS100_ACPU_BUS_GPV    0x83300000
#define S5JS100_PPMU_ACPU       0x83400000
#define S5JS100_MCMU            0x84000000
#define S5JS100_MWDOG           0x84010000
#define S5JS100_MTIMER          0x84011000
#define S5JS100_NBSLEEP         0x84012000
#define S5JS100_USIM            0x84013000
#define S5JS100_GIC_DST         0x84100000
#define S5JS100_GIC_CPU         0x84101000
#define S5JS100_MDMAC           0x84200000
#define S5JS100_MCPU_BUS_GPV    0x84300000
#define S5JS100_MCPU_BUS_CFG    0x84400000
#define S5JS100_BUS_MONITOR     0x84401000
#define S5JS100_PPMU_MCPU       0x84410000
#define S5JS100_MIFCMU          0x85000000
#define S5JS100_BAAW            0x85010000
#define S5JS100_QSPI_SFR        0x85020000
#define S5JS100_GPADCIF0        0x85021000
#define S5JS100_GPADCIF1        0x85022000
#define S5JS100_MAILBOX         0x85023000
#define S5JS100_UART0_BASE      0x85024000
#define S5JS100_UART1_BASE      0x85025000
#define S5JS100_GPIO_BASE       0x85026000
#define S5JS100_PPMU_MCPU2MIF   0x85030000
#define S5JS100_SYS_CFG         0x85040000
#define S5JS100_MIF_BUS_GPV     0x85300000
#define S5JS100_PPMU_ACPU2MIF   0x85400000
#define S5JS100_RFIP            0x86000000
#define S5JS100_DCXO_CFG        0x87000000
#define S5JS100_MCPU2GNSS0      0x88000000
#define S5JS100_MCPU2GNSS1      0xC0000000
#define S5JS100_NB_MDM00        0xDC000000
#define S5JS100_NB_MDM01        0xDC400000
#define S5JS100_NB_MDM02        0xDC800000
#define S5JS100_NB_MDM03        0xDCC00000
#define S5JS100_NB_MDM04        0xDD000000
#define S5JS100_NB_MDM05        0xDD400000
#define S5JS100_NB_MDM06        0xDD800000
#define S5JS100_NB_MDM07        0xDDC00000
#define S5JS100_NB_MDM08        0xDE000000
#define S5JS100_NB_MDM09        0xDE400000
#define S5JS100_NB_MDM10        0xDE800000
#define S5JS100_NB_MDM11        0xDEC00000
#define S5JS100_NB_MDM12        0xDF000000
#define S5JS100_NB_MDM13        0xDF400000
#define S5JS100_ACPU_CM7_PERI   0xE0000000
#define S5JS100_BOOTMEM_MIRROR  0xFFFF0000
/* ================================================================================ */
/* ================             Peripheral declaration             ================ */
/* ================================================================================ */

#define S5JS100_USI0_UART             ((S5JS100_USI_UART_TypeDef   *) S5JS100_USI0_BASE )
#define S5JS100_USI1_UART             ((S5JS100_USI_UART_TypeDef   *) S5JS100_USI1_BASE )
#define S5JS100_UART0 ((S5JS100_UART_TypeDef *) S5JS100_UART0_BASE )
#define S5JS100_UART1 ((S5JS100_UART_TypeDef *) S5JS100_UART1_BASE )
#define S5JS100_TIMER0            ((S5JS100_TIMER_TypeDef  *) S5JS100_TIMER0_BASE  )
#define S5JS100_TIMER1            ((S5JS100_TIMER_TypeDef  *) S5JS100_TIMER1_BASE  )

#define S5JS100_USI0_REG        ((S5JS100_USI_TypeDef   *) (S5JS100_USI0 + 0xC0))
#define S5JS100_USI1_REG        ((S5JS100_USI_TypeDef   *) (S5JS100_USI1 + 0xC0))

#define S5JS100_SYSCFG_USI0_CONF    (*(uint32_t *)(S5JS100_SYS_CFG + 0x1030))
#define S5JS100_SYSCFG_USI0_IPCLK   (*(uint32_t *)(S5JS100_SYS_CFG + 0x1034))
#define S5JS100_SYSCFG_USI1_CONF    (*(uint32_t *)(S5JS100_SYS_CFG + 0x1038))
#define S5JS100_SYSCFG_USI1_IPCLK   (*(uint32_t *)(S5JS100_SYS_CFG + 0x103C))


/*********************************************************************
*        GPIO 2 / 3 BIT FEILD POS, OUTPUTS
*************************************************************************/

#ifdef __cplusplus
}
#endif

#endif  /* S5JS100_H */
