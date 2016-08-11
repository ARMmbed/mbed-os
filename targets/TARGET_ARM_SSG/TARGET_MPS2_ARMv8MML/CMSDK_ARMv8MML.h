/**************************************************************************//**
 * @file     CMSDK_ARMv8MML.h
 * @brief    CMSIS Core Peripheral Access Layer Header File for
 *           CMSDK_ARMv8MML Device (configured for CMSDK_ARMv8MML without FPU)
 * @version  V4.06
 * @date     04. July 2016
 ******************************************************************************/
/* Copyright (c) 2015 - 2016 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/


#ifndef CMSDK_ARMv8MML_H
#define CMSDK_ARMv8MML_H

#ifdef __cplusplus
extern "C" {
#endif


/* -------------------------  Interrupt Number Definition  ------------------------ */

typedef enum IRQn
{
/* --------------------  ARMv8MML Processor Exceptions Numbers  ------------------- */
  NonMaskableInt_IRQn           = -14,      /*  2 Non Maskable Interrupt */
  HardFault_IRQn                = -13,      /*  3 HardFault Interrupt */
  MemoryManagement_IRQn         = -12,      /*  4 Memory Management Interrupt */
  BusFault_IRQn                 = -11,      /*  5 Bus Fault Interrupt */
  UsageFault_IRQn               = -10,      /*  6 Usage Fault Interrupt */
  SecureFault_IRQn              =  -9,      /*  7 Secure Fault Interrupt */
  SVCall_IRQn                   =  -5,      /* 11 SV Call Interrupt */
  DebugMonitor_IRQn             =  -4,      /* 12 Debug Monitor Interrupt */
  PendSV_IRQn                   =  -2,      /* 14 Pend SV Interrupt */
  SysTick_IRQn                  =  -1,      /* 15 System Tick Interrupt */

/* --------------------  ARMv8MML Specific Interrupt Numbers  --------------------- */
  UART0RX_IRQn                  =   0,      /* UART 0 receive interrupt */
  UART0TX_IRQn                  =   1,      /* UART 0 transmit interrupt */
  UART1RX_IRQn                  =   2,      /* UART 1 receive interrupt */
  UART1TX_IRQn                  =   3,      /* UART 1 transmit interrupt */
  UART2RX_IRQn                  =   4,      /* UART 2 receive interrupt */
  UART2TX_IRQn                  =   5,      /* UART 2 transmit interrupt */
  GPIO0ALL_IRQn                 =   6,      /* GPIO 0 combined interrupt */
  GPIO1ALL_IRQn                 =   7,      /* GPIO 1 combined interrupt */
  TIMER0_IRQn                   =   8,      /* Timer 0 interrupt */
  TIMER1_IRQn                   =   9,      /* Timer 1 interrupt */
  DUALTIMER_IRQn                =  10,      /* Dual Timer interrupt */
  SPI_0_1_IRQn                  =  11,      /* SPI #0, #1 interrupt */
  UART_0_1_2_OVF_IRQn           =  12,      /* UART overflow (0, 1 & 2) interrupt */
  ETHERNET_IRQn                 =  13,      /* Ethernet interrupt */
  I2S_IRQn                      =  14,      /* Audio I2S interrupt */
  TOUCHSCREEN_IRQn              =  15,      /* Touch Screen interrupt */
  GPIO2_IRQn                    =  16,      /* GPIO 2 combined interrupt */
  GPIO3_IRQn                    =  17,      /* GPIO 3 combined interrupt */
  UART3RX_IRQn                  =  18,      /* UART 3 receive interrupt */
  UART3TX_IRQn                  =  19,      /* UART 3 transmit interrupt */
  UART4RX_IRQn                  =  20,      /* UART 4 receive interrupt */
  UART4TX_IRQn                  =  21,      /* UART 4 transmit interrupt */
  SPI_2_IRQn                    =  22,      /* SPI #2 interrupt */
  SPI_3_4_IRQn                  =  23,      /* SPI #3, SPI #4 interrupt */
  GPIO0_0_IRQn                  =  24,      /* GPIO 0 individual interrupt ( 0) */
  GPIO0_1_IRQn                  =  25,      /* GPIO 0 individual interrupt ( 1) */
  GPIO0_2_IRQn                  =  26,      /* GPIO 0 individual interrupt ( 2) */
  GPIO0_3_IRQn                  =  27,      /* GPIO 0 individual interrupt ( 3) */
  GPIO0_4_IRQn                  =  28,      /* GPIO 0 individual interrupt ( 4) */
  GPIO0_5_IRQn                  =  29,      /* GPIO 0 individual interrupt ( 5) */
  GPIO0_6_IRQn                  =  30,      /* GPIO 0 individual interrupt ( 6) */
  GPIO0_7_IRQn                  =  31,      /* GPIO 0 individual interrupt ( 7) */
  GPIO1_0_IRQn                  =  32,      /* GPIO 1 individual interrupt ( 0) */
  GPIO1_1_IRQn                  =  33,      /* GPIO 1 individual interrupt ( 1) */
  GPIO1_2_IRQn                  =  34,      /* GPIO 1 individual interrupt ( 2) */
  GPIO1_3_IRQn                  =  35,      /* GPIO 1 individual interrupt ( 3) */
  GPIO1_4_IRQn                  =  36,      /* GPIO 1 individual interrupt ( 4) */
  GPIO1_5_IRQn                  =  37,      /* GPIO 1 individual interrupt ( 5) */
  GPIO1_6_IRQn                  =  38,      /* GPIO 1 individual interrupt ( 6) */
  GPIO1_7_IRQn                  =  39,      /* GPIO 1 individual interrupt ( 7) */
  GPIO1_8_IRQn                  =  40,      /* GPIO 1 individual interrupt ( 0) */
  GPIO1_9_IRQn                  =  41,      /* GPIO 1 individual interrupt ( 9) */
  GPIO1_10_IRQn                 =  42,      /* GPIO 1 individual interrupt (10) */
  GPIO1_11_IRQn                 =  43,      /* GPIO 1 individual interrupt (11) */
  GPIO1_12_IRQn                 =  44,      /* GPIO 1 individual interrupt (12) */
  GPIO1_13_IRQn                 =  45,      /* GPIO 1 individual interrupt (13) */
  GPIO1_14_IRQn                 =  46,      /* GPIO 1 individual interrupt (14) */
  GPIO1_15_IRQn                 =  47,      /* GPIO 1 individual interrupt (15) */
  SPI_0B_IRQn                   =  48,      /* SPI #0 interrupt */
  Reserved_IRQn                 =  49,      /* Reserved */
  SECURETIMER0_IRQn             =  50,      /* Secure Timer 0 interrupt */
  SECURETIMER1_IRQn             =  51,      /* Secure Timer 1 interrupt */
  SPI_1B_IRQn                   =  52,      /* SPI #1 interrupt */
  SPI_2B_IRQn                   =  53,      /* SPI #2 interrupt */
  SPI_3B_IRQn                   =  54,      /* SPI #3 interrupt */
  SPI_4B_IRQn                   =  55       /* SPI #4 interrupt */
} IRQn_Type;


/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

/* -------  Start of section using anonymous unions and disabling warnings  ------- */
#if   defined (__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined (__ICCARM__)
  #pragma language=extended
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc11-extensions"
  #pragma clang diagnostic ignored "-Wreserved-id-macro"
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning 586
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif


/* --------  Configuration of the Cortex-ARMv8MML Processor and Core Peripherals  ------- */
#define __ARMv8MML_REV            0x0001U   /* Core revision r0p1 */
#define __SAUREGION_PRESENT       1U        /* SAU regions are present */
#define __MPU_PRESENT             1U        /* MPU present */
#define __NVIC_PRIO_BITS          3U        /* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig    0U        /* Set to 1 if different SysTick Config is used */
#define __FPU_PRESENT             0U        /* no FPU present */

#include "core_armv8mml.h"                  /* Processor and core peripherals */
#include "system_CMSDK_ARMv8MML.h"          /* System Header */


/* ================================================================================ */
/* ================       Device Specific Peripheral Section       ================ */
/* ================================================================================ */

/*------------- Universal Asynchronous Receiver Transmitter (UART) -----------*/
typedef struct
{
  __IOM  uint32_t  DATA;                     /* Offset: 0x000 (R/W) Data Register    */
  __IOM  uint32_t  STATE;                    /* Offset: 0x004 (R/W) Status Register  */
  __IOM  uint32_t  CTRL;                     /* Offset: 0x008 (R/W) Control Register */
  union {
    __IM   uint32_t  INTSTATUS;              /* Offset: 0x00C (R/ ) Interrupt Status Register */
    __OM   uint32_t  INTCLEAR;               /* Offset: 0x00C ( /W) Interrupt Clear Register  */
    };
  __IOM  uint32_t  BAUDDIV;                  /* Offset: 0x010 (R/W) Baudrate Divider Register */

} CMSDK_UART_TypeDef;

/* CMSDK_UART DATA Register Definitions */
#define CMSDK_UART_DATA_Pos               0                                                  /* CMSDK_UART_DATA_Pos: DATA Position */
#define CMSDK_UART_DATA_Msk              (0xFFUL /*<< CMSDK_UART_DATA_Pos*/)                 /* CMSDK_UART DATA: DATA Mask */

/* CMSDK_UART STATE Register Definitions */
#define CMSDK_UART_STATE_RXOR_Pos         3                                                  /* CMSDK_UART STATE: RXOR Position */
#define CMSDK_UART_STATE_RXOR_Msk        (0x1UL << CMSDK_UART_STATE_RXOR_Pos)                /* CMSDK_UART STATE: RXOR Mask */

#define CMSDK_UART_STATE_TXOR_Pos         2                                                  /* CMSDK_UART STATE: TXOR Position */
#define CMSDK_UART_STATE_TXOR_Msk        (0x1UL << CMSDK_UART_STATE_TXOR_Pos)                /* CMSDK_UART STATE: TXOR Mask */

#define CMSDK_UART_STATE_RXBF_Pos         1                                                  /* CMSDK_UART STATE: RXBF Position */
#define CMSDK_UART_STATE_RXBF_Msk        (0x1UL << CMSDK_UART_STATE_RXBF_Pos)                /* CMSDK_UART STATE: RXBF Mask */

#define CMSDK_UART_STATE_TXBF_Pos         0                                                  /* CMSDK_UART STATE: TXBF Position */
#define CMSDK_UART_STATE_TXBF_Msk        (0x1UL /*<< CMSDK_UART_STATE_TXBF_Pos*/)            /* CMSDK_UART STATE: TXBF Mask */

/* CMSDK_UART CTRL Register Definitions */
#define CMSDK_UART_CTRL_HSTM_Pos          6                                                  /* CMSDK_UART CTRL: HSTM Position */
#define CMSDK_UART_CTRL_HSTM_Msk         (0x01UL << CMSDK_UART_CTRL_HSTM_Pos)                /* CMSDK_UART CTRL: HSTM Mask */

#define CMSDK_UART_CTRL_RXORIRQEN_Pos     5                                                  /* CMSDK_UART CTRL: RXORIRQEN Position */
#define CMSDK_UART_CTRL_RXORIRQEN_Msk    (0x01UL << CMSDK_UART_CTRL_RXORIRQEN_Pos)           /* CMSDK_UART CTRL: RXORIRQEN Mask */

#define CMSDK_UART_CTRL_TXORIRQEN_Pos     4                                                  /* CMSDK_UART CTRL: TXORIRQEN Position */
#define CMSDK_UART_CTRL_TXORIRQEN_Msk    (0x01UL << CMSDK_UART_CTRL_TXORIRQEN_Pos)           /* CMSDK_UART CTRL: TXORIRQEN Mask */

#define CMSDK_UART_CTRL_RXIRQEN_Pos       3                                                  /* CMSDK_UART CTRL: RXIRQEN Position */
#define CMSDK_UART_CTRL_RXIRQEN_Msk      (0x01UL << CMSDK_UART_CTRL_RXIRQEN_Pos)             /* CMSDK_UART CTRL: RXIRQEN Mask */

#define CMSDK_UART_CTRL_TXIRQEN_Pos       2                                                  /* CMSDK_UART CTRL: TXIRQEN Position */
#define CMSDK_UART_CTRL_TXIRQEN_Msk      (0x01UL << CMSDK_UART_CTRL_TXIRQEN_Pos)             /* CMSDK_UART CTRL: TXIRQEN Mask */

#define CMSDK_UART_CTRL_RXEN_Pos          1                                                  /* CMSDK_UART CTRL: RXEN Position */
#define CMSDK_UART_CTRL_RXEN_Msk         (0x01UL << CMSDK_UART_CTRL_RXEN_Pos)                /* CMSDK_UART CTRL: RXEN Mask */

#define CMSDK_UART_CTRL_TXEN_Pos          0                                                  /* CMSDK_UART CTRL: TXEN Position */
#define CMSDK_UART_CTRL_TXEN_Msk         (0x01UL /*<< CMSDK_UART_CTRL_TXEN_Pos*/)            /* CMSDK_UART CTRL: TXEN Mask */

#define CMSDK_UART_INTSTATUS_RXORIRQ_Pos  3                                                  /* CMSDK_UART CTRL: RXORIRQ Position */
#define CMSDK_UART_CTRL_RXORIRQ_Msk      (0x01UL << CMSDK_UART_INTSTATUS_RXORIRQ_Pos)        /* CMSDK_UART CTRL: RXORIRQ Mask */

#define CMSDK_UART_CTRL_TXORIRQ_Pos       2                                                  /* CMSDK_UART CTRL: TXORIRQ Position */
#define CMSDK_UART_CTRL_TXORIRQ_Msk      (0x01UL << CMSDK_UART_CTRL_TXORIRQ_Pos)             /* CMSDK_UART CTRL: TXORIRQ Mask */

#define CMSDK_UART_CTRL_RXIRQ_Pos         1                                                  /* CMSDK_UART CTRL: RXIRQ Position */
#define CMSDK_UART_CTRL_RXIRQ_Msk        (0x01UL << CMSDK_UART_CTRL_RXIRQ_Pos)               /* CMSDK_UART CTRL: RXIRQ Mask */

#define CMSDK_UART_CTRL_TXIRQ_Pos         0                                                  /* CMSDK_UART CTRL: TXIRQ Position */
#define CMSDK_UART_CTRL_TXIRQ_Msk        (0x01UL /*<< CMSDK_UART_CTRL_TXIRQ_Pos*/)           /* CMSDK_UART CTRL: TXIRQ Mask */

/* CMSDK_UART BAUDDIV Register Definitions */
#define CMSDK_UART_BAUDDIV_Pos            0                                                  /* CMSDK_UART BAUDDIV: BAUDDIV Position */
#define CMSDK_UART_BAUDDIV_Msk           (0xFFFFFUL /*<< CMSDK_UART_BAUDDIV_Pos*/)           /* CMSDK_UART BAUDDIV: BAUDDIV Mask */


/*----------------------------- Timer (TIMER) -------------------------------*/
typedef struct
{
  __IOM  uint32_t  CTRL;                     /* Offset: 0x000 (R/W) Control Register */
  __IOM  uint32_t  VALUE;                    /* Offset: 0x004 (R/W) Current Value Register */
  __IOM  uint32_t  RELOAD;                   /* Offset: 0x008 (R/W) Reload Value Register */
  union {
    __IM   uint32_t  INTSTATUS;              /* Offset: 0x00C (R/ ) Interrupt Status Register */
    __OM   uint32_t  INTCLEAR;               /* Offset: 0x00C ( /W) Interrupt Clear Register */
    };

} CMSDK_TIMER_TypeDef;

/* CMSDK_TIMER CTRL Register Definitions */
#define CMSDK_TIMER_CTRL_IRQEN_Pos          3                                                /* CMSDK_TIMER CTRL: IRQEN Position */
#define CMSDK_TIMER_CTRL_IRQEN_Msk         (0x01UL << CMSDK_TIMER_CTRL_IRQEN_Pos)            /* CMSDK_TIMER CTRL: IRQEN Mask */

#define CMSDK_TIMER_CTRL_SELEXTCLK_Pos      2                                                /* CMSDK_TIMER CTRL: SELEXTCLK Position */
#define CMSDK_TIMER_CTRL_SELEXTCLK_Msk     (0x01UL << CMSDK_TIMER_CTRL_SELEXTCLK_Pos)        /* CMSDK_TIMER CTRL: SELEXTCLK Mask */

#define CMSDK_TIMER_CTRL_SELEXTEN_Pos       1                                                /* CMSDK_TIMER CTRL: SELEXTEN Position */
#define CMSDK_TIMER_CTRL_SELEXTEN_Msk      (0x01UL << CMSDK_TIMER_CTRL_SELEXTEN_Pos)         /* CMSDK_TIMER CTRL: SELEXTEN Mask */

#define CMSDK_TIMER_CTRL_EN_Pos             0                                                /* CMSDK_TIMER CTRL: EN Position */
#define CMSDK_TIMER_CTRL_EN_Msk            (0x01UL /*<< CMSDK_TIMER_CTRL_EN_Pos*/)           /* CMSDK_TIMER CTRL: EN Mask */

/* CMSDK_TIMER VAL Register Definitions */
#define CMSDK_TIMER_VAL_CURRENT_Pos         0                                                /* CMSDK_TIMER VALUE: CURRENT Position */
#define CMSDK_TIMER_VAL_CURRENT_Msk        (0xFFFFFFFFUL /*<< CMSDK_TIMER_VAL_CURRENT_Pos*/) /* CMSDK_TIMER VALUE: CURRENT Mask */

/* CMSDK_TIMER RELOAD Register Definitions */
#define CMSDK_TIMER_RELOAD_VAL_Pos          0                                                /* CMSDK_TIMER RELOAD: RELOAD Position */
#define CMSDK_TIMER_RELOAD_VAL_Msk         (0xFFFFFFFFUL /*<< CMSDK_TIMER_RELOAD_VAL_Pos*/)  /* CMSDK_TIMER RELOAD: RELOAD Mask */

/* CMSDK_TIMER INTSTATUS Register Definitions */
#define CMSDK_TIMER_INTSTATUS_Pos           0                                                /* CMSDK_TIMER INTSTATUS: INTSTATUSPosition */
#define CMSDK_TIMER_INTSTATUS_Msk          (0x01UL /*<< CMSDK_TIMER_INTSTATUS_Pos*/)         /* CMSDK_TIMER INTSTATUS: INTSTATUSMask */

/* CMSDK_TIMER INTCLEAR Register Definitions */
#define CMSDK_TIMER_INTCLEAR_Pos            0                                                /* CMSDK_TIMER INTCLEAR: INTCLEAR Position */
#define CMSDK_TIMER_INTCLEAR_Msk           (0x01UL /*<< CMSDK_TIMER_INTCLEAR_Pos*/)          /* CMSDK_TIMER INTCLEAR: INTCLEAR Mask */


/*------------- Timer (TIM) --------------------------------------------------*/
typedef struct
{
  __IOM uint32_t T1LOAD;                     /* Offset: 0x000 (R/W) Timer 1 Load */
  __IM  uint32_t T1VALUE;                    /* Offset: 0x004 (R/ ) Timer 1 Counter Current Value */
  __IOM uint32_t T1CTRL;                     /* Offset: 0x008 (R/W) Timer 1 Control */
  __OM  uint32_t T1INTCLR;                   /* Offset: 0x00C ( /W) Timer 1 Interrupt Clear */
  __IM  uint32_t T1RIS;                      /* Offset: 0x010 (R/ ) Timer 1 Raw Interrupt Status */
  __IM  uint32_t T1MIS;                      /* Offset: 0x014 (R/ ) Timer 1 Masked Interrupt Status */
  __IOM uint32_t T1BGLOAD;                   /* Offset: 0x018 (R/W) Background Load Register */
        uint32_t RESERVED0;
  __IOM uint32_t T2LOAD;                     /* Offset: 0x020 (R/W) Timer 2 Load */
  __IM  uint32_t T2VALUE;                    /* Offset: 0x024 (R/ ) Timer 2 Counter Current Value */
  __IOM uint32_t T2CTRL;                     /* Offset: 0x028 (R/W) Timer 2 Control */
  __OM  uint32_t T2INTCLR;                   /* Offset: 0x02C ( /W) Timer 2 Interrupt Clear */
  __IM  uint32_t T2RIS;                      /* Offset: 0x030 (R/ ) Timer 2 Raw Interrupt Status */
  __IM  uint32_t T2MIS;                      /* Offset: 0x034 (R/ ) Timer 2 Masked Interrupt Status */
  __IOM uint32_t T2BGLOAD;                   /* Offset: 0x038 (R/W) Background Load Register */
        uint32_t RESERVED1[945];
  __IOM uint32_t ITCR;                       /* Offset: 0xF00 (R/W) Integration Test Control Register */
  __OM  uint32_t ITOP;                       /* Offset: 0xF04 ( /W) Integration Test Output Set Register */
} CMSDK_DUALTIMER_BOTH_TypeDef;


typedef struct
{
  __IOM uint32_t LOAD;                       /* Offset: 0x000 (R/W) Timer Load */
  __IM  uint32_t VALUE;                      /* Offset: 0x000 (R/W) Timer Counter Current Value */
  __IOM uint32_t CTRL;                       /* Offset: 0x000 (R/W) Timer Control */
  __OM  uint32_t INTCLR;                     /* Offset: 0x000 (R/W) Timer Interrupt Clear */
  __IM  uint32_t RIS;                        /* Offset: 0x000 (R/W) Timer Raw Interrupt Status */
  __IM  uint32_t MIS;                        /* Offset: 0x000 (R/W) Timer Masked Interrupt Status */
  __IOM uint32_t BGLOAD;                     /* Offset: 0x000 (R/W) Background Load Register */
} CMSDK_DUALTIMER_SINGLE_TypeDef;

/* CMSDK_DUALTIMER_SINGLE LOAD Register Definitions */
#define CMSDK_DUALTIMER_LOAD_Pos             0                                               /* CMSDK_DUALTIMER LOAD: LOAD Position */
#define CMSDK_DUALTIMER_LOAD_Msk            (0xFFFFFFFFUL /*<< CMSDK_DUALTIMER_LOAD_Pos*/)   /* CMSDK_DUALTIMER LOAD: LOAD Mask */

/* CMSDK_DUALTIMER_SINGLE VALUE Register Definitions */
#define CMSDK_DUALTIMER_VALUE_Pos            0                                               /* CMSDK_DUALTIMER VALUE: VALUE Position */
#define CMSDK_DUALTIMER_VALUE_Msk           (0xFFFFFFFFUL /*<< CMSDK_DUALTIMER_VALUE_Pos*/)  /* CMSDK_DUALTIMER VALUE: VALUE Mask */

/* CMSDK_DUALTIMER_SINGLE CTRL Register Definitions */
#define CMSDK_DUALTIMER_CTRL_EN_Pos          7                                               /* CMSDK_DUALTIMER CTRL_EN: CTRL Enable Position */
#define CMSDK_DUALTIMER_CTRL_EN_Msk         (0x1UL << CMSDK_DUALTIMER_CTRL_EN_Pos)           /* CMSDK_DUALTIMER CTRL_EN: CTRL Enable Mask */

#define CMSDK_DUALTIMER_CTRL_MODE_Pos        6                                               /* CMSDK_DUALTIMER CTRL_MODE: CTRL MODE Position */
#define CMSDK_DUALTIMER_CTRL_MODE_Msk       (0x1UL << CMSDK_DUALTIMER_CTRL_MODE_Pos)         /* CMSDK_DUALTIMER CTRL_MODE: CTRL MODE Mask */

#define CMSDK_DUALTIMER_CTRL_INTEN_Pos       5                                               /* CMSDK_DUALTIMER CTRL_INTEN: CTRL Int Enable Position */
#define CMSDK_DUALTIMER_CTRL_INTEN_Msk      (0x1UL << CMSDK_DUALTIMER_CTRL_INTEN_Pos)        /* CMSDK_DUALTIMER CTRL_INTEN: CTRL Int Enable Mask */

#define CMSDK_DUALTIMER_CTRL_PRESCALE_Pos    2                                               /* CMSDK_DUALTIMER CTRL_PRESCALE: CTRL PRESCALE Position */
#define CMSDK_DUALTIMER_CTRL_PRESCALE_Msk   (0x3UL << CMSDK_DUALTIMER_CTRL_PRESCALE_Pos)     /* CMSDK_DUALTIMER CTRL_PRESCALE: CTRL PRESCALE Mask */

#define CMSDK_DUALTIMER_CTRL_SIZE_Pos        1                                               /* CMSDK_DUALTIMER CTRL_SIZE: CTRL SIZE Position */
#define CMSDK_DUALTIMER_CTRL_SIZE_Msk       (0x1UL << CMSDK_DUALTIMER_CTRL_SIZE_Pos)         /* CMSDK_DUALTIMER CTRL_SIZE: CTRL SIZE Mask */

#define CMSDK_DUALTIMER_CTRL_ONESHOOT_Pos    0                                               /* CMSDK_DUALTIMER CTRL_ONESHOOT: CTRL ONESHOOT Position */
#define CMSDK_DUALTIMER_CTRL_ONESHOOT_Msk   (0x1UL /*<< CMSDK_DUALTIMER_CTRL_ONESHOOT_Pos*/) /* CMSDK_DUALTIMER CTRL_ONESHOOT: CTRL ONESHOOT Mask */

/* CMSDK_DUALTIMER_SINGLE INTCLR Register Definitions */
#define CMSDK_DUALTIMER_INTCLR_Pos           0                                               /* CMSDK_DUALTIMER INTCLR: INT Clear Position */
#define CMSDK_DUALTIMER_INTCLR_Msk          (0x1UL /*<< CMSDK_DUALTIMER_INTCLR_Pos*/)        /* CMSDK_DUALTIMER INTCLR: INT Clear  Mask */

/* CMSDK_DUALTIMER_SINGLE RIS Register Definitions */
#define CMSDK_DUALTIMER_RIS_Pos              0                                               /* CMSDK_DUALTIMER RAWINTSTAT: Raw Int Status Position */
#define CMSDK_DUALTIMER_RIS_Msk             (0x1UL /*<< CMSDK_DUALTIMER_RAWINTSTAT_Pos*/)    /* CMSDK_DUALTIMER RAWINTSTAT: Raw Int Status Mask */

/* CMSDK_DUALTIMER_SINGLE MIS Register Definitions */
#define CMSDK_DUALTIMER_MIS_Pos              0                                               /* CMSDK_DUALTIMER MASKINTSTAT: Mask Int Status Position */
#define CMSDK_DUALTIMER_MIS_Msk             (0x1UL /*<< CMSDK_DUALTIMER_MASKINTSTAT_Pos*/)   /* CMSDK_DUALTIMER MASKINTSTAT: Mask Int Status Mask */

/* CMSDK_DUALTIMER_SINGLE BGLOAD Register Definitions */
#define CMSDK_DUALTIMER_BGLOAD_Pos           0                                               /* CMSDK_DUALTIMER BGLOAD: Background Load Position */
#define CMSDK_DUALTIMER_BGLOAD_Msk          (0xFFFFFFFFUL /*<< CMSDK_DUALTIMER_BGLOAD_Pos*/) /* CMSDK_DUALTIMER BGLOAD: Background Load Mask */


/*-------------------- General Purpose Input Output (GPIO) -------------------*/
typedef struct
{
  __IOM  uint32_t  DATA;                     /* Offset: 0x000 (R/W) DATA Register */
  __IOM  uint32_t  DATAOUT;                  /* Offset: 0x004 (R/W) Data Output Latch Register */
         uint32_t  RESERVED0[2];
  __IOM  uint32_t  OUTENSET;                 /* Offset: 0x010 (R/W) Output Enable Set Register */
  __IOM  uint32_t  OUTENCLR;                 /* Offset: 0x014 (R/W) Output Enable Clear Register */
  __IOM  uint32_t  ALTFUNCSET;               /* Offset: 0x018 (R/W) Alternate Function Set Register */
  __IOM  uint32_t  ALTFUNCCLR;               /* Offset: 0x01C (R/W) Alternate Function Clear Register */
  __IOM  uint32_t  INTENSET;                 /* Offset: 0x020 (R/W) Interrupt Enable Set Register */
  __IOM  uint32_t  INTENCLR;                 /* Offset: 0x024 (R/W) Interrupt Enable Clear Register */
  __IOM  uint32_t  INTTYPESET;               /* Offset: 0x028 (R/W) Interrupt Type Set Register */
  __IOM  uint32_t  INTTYPECLR;               /* Offset: 0x02C (R/W) Interrupt Type Clear Register */
  __IOM  uint32_t  INTPOLSET;                /* Offset: 0x030 (R/W) Interrupt Polarity Set Register */
  __IOM  uint32_t  INTPOLCLR;                /* Offset: 0x034 (R/W) Interrupt Polarity Clear Register */
  union {
    __IM   uint32_t  INTSTATUS;              /* Offset: 0x038 (R/ ) Interrupt Status Register */
    __OM   uint32_t  INTCLEAR;               /* Offset: 0x038 ( /W) Interrupt Clear Register */
    };
         uint32_t RESERVED1[241];
  __IOM  uint32_t LB_MASKED[256];            /* Offset: 0x400 - 0x7FC Lower byte Masked Access Register (R/W) */
  __IOM  uint32_t UB_MASKED[256];            /* Offset: 0x800 - 0xBFC Upper byte Masked Access Register (R/W) */
} CMSDK_GPIO_TypeDef;

/* CMSDK_GPIO DATA Register Definitions */
#define CMSDK_GPIO_DATA_Pos            0                                                     /* CMSDK_GPIO DATA: DATA Position */
#define CMSDK_GPIO_DATA_Msk           (0xFFFFUL /*<< CMSDK_GPIO_DATA_Pos*/)                  /* CMSDK_GPIO DATA: DATA Mask */

/* CMSDK_GPIO DATAOUT Register Definitions */
#define CMSDK_GPIO_DATAOUT_Pos         0                                                     /* CMSDK_GPIO DATAOUT: DATAOUT Position */
#define CMSDK_GPIO_DATAOUT_Msk        (0xFFFFUL /*<< CMSDK_GPIO_DATAOUT_Pos*/)               /* CMSDK_GPIO DATAOUT: DATAOUT Mask */

/* CMSDK_GPIO OUTENSET Register Definitions */
#define CMSDK_GPIO_OUTENSET_Pos        0                                                     /* CMSDK_GPIO OUTEN: OUTEN Position */
#define CMSDK_GPIO_OUTENSET_Msk       (0xFFFFUL /*<< CMSDK_GPIO_OUTEN_Pos*/)                 /* CMSDK_GPIO OUTEN: OUTEN Mask */

/* CMSDK_GPIO OUTENCLR Register Definitions */
#define CMSDK_GPIO_OUTENCLR_Pos        0                                                     /* CMSDK_GPIO OUTEN: OUTEN Position */
#define CMSDK_GPIO_OUTENCLR_Msk       (0xFFFFUL /*<< CMSDK_GPIO_OUTEN_Pos*/)                 /* CMSDK_GPIO OUTEN: OUTEN Mask */

/* CMSDK_GPIO ALTFUNCSET Register Definitions */
#define CMSDK_GPIO_ALTFUNCSET_Pos      0                                                     /* CMSDK_GPIO ALTFUNC: ALTFUNC Position */
#define CMSDK_GPIO_ALTFUNCSET_Msk     (0xFFFFUL /*<< CMSDK_GPIO_ALTFUNC_Pos*/)               /* CMSDK_GPIO ALTFUNC: ALTFUNC Mask */

/* CMSDK_GPIO ALTFUNCCLR Register Definitions */
#define CMSDK_GPIO_ALTFUNCCLR_Pos      0                                                     /* CMSDK_GPIO ALTFUNC: ALTFUNC Position */
#define CMSDK_GPIO_ALTFUNCCLR_Msk     (0xFFFFUL /*<< CMSDK_GPIO_ALTFUNC_Pos*/)               /* CMSDK_GPIO ALTFUNC: ALTFUNC Mask */

/* CMSDK_GPIO INTENSET Register Definitions */
#define CMSDK_GPIO_INTENSET_Pos        0                                                     /* CMSDK_GPIO INTEN: INTEN Position */
#define CMSDK_GPIO_INTENSET_Msk       (0xFFFFUL /*<< CMSDK_GPIO_INTEN_Pos*/)                 /* CMSDK_GPIO INTEN: INTEN Mask */

/* CMSDK_GPIO INTENCLR Register Definitions */
#define CMSDK_GPIO_INTENCLR_Pos        0                                                     /* CMSDK_GPIO INTEN: INTEN Position */
#define CMSDK_GPIO_INTENCLR_Msk       (0xFFFFUL /*<< CMSDK_GPIO_INTEN_Pos*/)                 /* CMSDK_GPIO INTEN: INTEN Mask */

/* CMSDK_GPIO INTTYPESET Register Definitions */
#define CMSDK_GPIO_INTTYPESET_Pos      0                                                     /* CMSDK_GPIO INTTYPE: INTTYPE Position */
#define CMSDK_GPIO_INTTYPESET_Msk     (0xFFFFUL /*<< CMSDK_GPIO_INTTYPE_Pos*/)               /* CMSDK_GPIO INTTYPE: INTTYPE Mask */

/* CMSDK_GPIO INTTYPECLR Register Definitions */
#define CMSDK_GPIO_INTTYPECLR_Pos      0                                                     /* CMSDK_GPIO INTTYPE: INTTYPE Position */
#define CMSDK_GPIO_INTTYPECLR_Msk     (0xFFFFUL /*<< CMSDK_GPIO_INTTYPE_Pos*/)               /* CMSDK_GPIO INTTYPE: INTTYPE Mask */

/* CMSDK_GPIO INTPOLSET Register Definitions */
#define CMSDK_GPIO_INTPOLSET_Pos       0                                                     /* CMSDK_GPIO INTPOL: INTPOL Position */
#define CMSDK_GPIO_INTPOLSET_Msk      (0xFFFFUL /*<< CMSDK_GPIO_INTPOL_Pos*/)                /* CMSDK_GPIO INTPOL: INTPOL Mask */

/* CMSDK_GPIO INTPOLCLR Register Definitions */
#define CMSDK_GPIO_INTPOLCLR_Pos       0                                                     /* CMSDK_GPIO INTPOL: INTPOL Position */
#define CMSDK_GPIO_INTPOLCLR_Msk      (0xFFFFUL /*<< CMSDK_GPIO_INTPOL_Pos*/)                /* CMSDK_GPIO INTPOL: INTPOL Mask */

/* CMSDK_GPIO INTCLEAR Register Definitions */
#define CMSDK_GPIO_INTSTATUS_Pos       0                                                     /* CMSDK_GPIO INTSTATUS: INTSTATUS Position */
#define CMSDK_GPIO_INTCLEAR_Msk       (0xFFUL /*<< CMSDK_GPIO_INTSTATUS_Pos*/)               /* CMSDK_GPIO INTSTATUS: INTSTATUS Mask */

/* CMSDK_GPIO INTCLEAR Register Definitions */
#define CMSDK_GPIO_INTCLEAR_Pos        0                                                     /* CMSDK_GPIO INTCLEAR: INTCLEAR Position */
#define CMSDK_GPIO_INTCLEAR_Msk       (0xFFUL /*<< CMSDK_GPIO_INTCLEAR_Pos*/)                /* CMSDK_GPIO INTCLEAR: INTCLEAR Mask */

/* CMSDK_GPIO MASKLOWBYTE Register Definitions */
#define CMSDK_GPIO_MASKLOWBYTE_Pos     0                                                     /* CMSDK_GPIO MASKLOWBYTE: MASKLOWBYTE Position */
#define CMSDK_GPIO_MASKLOWBYTE_Msk    (0x00FFUL /*<< CMSDK_GPIO_MASKLOWBYTE_Pos*/)           /* CMSDK_GPIO MASKLOWBYTE: MASKLOWBYTE Mask */

/* CMSDK_GPIO MASKHIGHBYTE Register Definitions */
#define CMSDK_GPIO_MASKHIGHBYTE_Pos    0                                                     /* CMSDK_GPIO MASKHIGHBYTE: MASKHIGHBYTE Position */
#define CMSDK_GPIO_MASKHIGHBYTE_Msk   (0xFF00UL /*<< CMSDK_GPIO_MASKHIGHBYTE_Pos*/)          /* CMSDK_GPIO MASKHIGHBYTE: MASKHIGHBYTE Mask */


/*------------- System Control (SYSCON) --------------------------------------*/
typedef struct
{
  __IOM  uint32_t  REMAP;                    /* Offset: 0x000 (R/W) Remap Control Register */
  __IOM  uint32_t  PMUCTRL;                  /* Offset: 0x004 (R/W) PMU Control Register */
  __IOM  uint32_t  RESETOP;                  /* Offset: 0x008 (R/W) Reset Option Register */
  __IOM  uint32_t  EMICTRL;                  /* Offset: 0x00C (R/W) EMI Control Register */
  __IOM  uint32_t  RSTINFO;                  /* Offset: 0x010 (R/W) Reset Information Register */
} CMSDK_SYSCON_TypeDef;

/* CMSDK_SYSCON REMAP Register Definitions */
#define CMSDK_SYSCON_REMAP_Pos                 0
#define CMSDK_SYSCON_REMAP_Msk                (0x1UL /*<< CMSDK_SYSCON_REMAP_Pos*/)          /* CMSDK_SYSCON MEME_CTRL: REMAP Mask */

/* CMSDK_SYSCON PMUCTRL Register Definitions */
#define CMSDK_SYSCON_PMUCTRL_EN_Pos            0
#define CMSDK_SYSCON_PMUCTRL_EN_Msk           (0x1UL /*<< CMSDK_SYSCON_PMUCTRL_EN_Pos*/)     /* CMSDK_SYSCON PMUCTRL: PMUCTRL ENABLE Mask */

/* CMSDK_SYSCON LOCKUPRST Register Definitions */
#define CMSDK_SYSCON_LOCKUPRST_RESETOP_Pos     0
#define CMSDK_SYSCON_LOCKUPRST_RESETOP_Msk    (0x1UL /*<< CMSDK_SYSCON_LOCKUPRST_RESETOP_Pos*/)   /* CMSDK_SYSCON SYS_CTRL: LOCKUP RESET ENABLE Mask */

/* CMSDK_SYSCON EMICTRL Register Definitions */
#define CMSDK_SYSCON_EMICTRL_SIZE_Pos         24
#define CMSDK_SYSCON_EMICTRL_SIZE_Msk         (0x1UL << CMSDK_SYSCON_EMICTRL_SIZE_Pos)       /* CMSDK_SYSCON EMICTRL: SIZE Mask */

#define CMSDK_SYSCON_EMICTRL_TACYC_Pos        16
#define CMSDK_SYSCON_EMICTRL_TACYC_Msk        (0x7UL << CMSDK_SYSCON_EMICTRL_TACYC_Pos)      /* CMSDK_SYSCON EMICTRL: TURNAROUNDCYCLE Mask */

#define CMSDK_SYSCON_EMICTRL_WCYC_Pos          8
#define CMSDK_SYSCON_EMICTRL_WCYC_Msk         (0x3UL << CMSDK_SYSCON_EMICTRL_WCYC_Pos)       /* CMSDK_SYSCON EMICTRL: WRITECYCLE Mask */

#define CMSDK_SYSCON_EMICTRL_RCYC_Pos          0
#define CMSDK_SYSCON_EMICTRL_RCYC_Msk         (0x7UL /*<< CMSDK_SYSCON_EMICTRL_RCYC_Pos*/)   /* CMSDK_SYSCON EMICTRL: READCYCLE Mask */

/* CMSDK_SYSCON RSTINFO Register Definitions */
#define CMSDK_SYSCON_RSTINFO_LOCKUPRESET_Pos   2
#define CMSDK_SYSCON_RSTINFO_LOCKUPRESET_Msk  (0x1UL << CMSDK_SYSCON_RSTINFO_LOCKUPRESET_Pos)     /* CMSDK_SYSCON RSTINFO: LOCKUPRESET Mask */

#define CMSDK_SYSCON_RSTINFO_WDOGRESETREQ_Pos  1
#define CMSDK_SYSCON_RSTINFO_WDOGRESETREQ_Msk (0x1UL << CMSDK_SYSCON_RSTINFO_WDOGRESETREQ_Pos)    /* CMSDK_SYSCON RSTINFO: WDOGRESETREQ Mask */

#define CMSDK_SYSCON_RSTINFO_SYSRESETREQ_Pos   0
#define CMSDK_SYSCON_RSTINFO_SYSRESETREQ_Msk  (0x1UL /*<< CMSDK_SYSCON_RSTINFO_SYSRESETREQ_Pos*/) /* CMSDK_SYSCON RSTINFO: SYSRESETREQ Mask */


/*------------------- Watchdog ----------------------------------------------*/
typedef struct
{

  __IOM  uint32_t  LOAD;                    /* Offset: 0x000 (R/W) Watchdog Load Register */
  __IM   uint32_t  VALUE;                   /* Offset: 0x004 (R/ ) Watchdog Value Register */
  __IOM  uint32_t  CTRL;                    /* Offset: 0x008 (R/W) Watchdog Control Register */
  __OM   uint32_t  INTCLR;                  /* Offset: 0x00C ( /W) Watchdog Clear Interrupt Register */
  __IM   uint32_t  RAWINTSTAT;              /* Offset: 0x010 (R/ ) Watchdog Raw Interrupt Status Register */
  __IM   uint32_t  MASKINTSTAT;             /* Offset: 0x014 (R/ ) Watchdog Interrupt Status Register */
         uint32_t  RESERVED0[762];
  __IOM  uint32_t  LOCK;                    /* Offset: 0xC00 (R/W) Watchdog Lock Register */
         uint32_t  RESERVED1[191];
  __IOM  uint32_t  ITCR;                    /* Offset: 0xF00 (R/W) Watchdog Integration Test Control Register */
  __OM   uint32_t  ITOP;                    /* Offset: 0xF04 ( /W) Watchdog Integration Test Output Set Register */
}CMSDK_WATCHDOG_TypeDef;

/* CMSDK_WATCHDOG LOAD Register Definitions */
#define CMSDK_Watchdog_LOAD_Pos               0                                              /* CMSDK_Watchdog LOAD: LOAD Position */
#define CMSDK_Watchdog_LOAD_Msk              (0xFFFFFFFFUL /*<< CMSDK_Watchdog_LOAD_Pos*/)   /* CMSDK_Watchdog LOAD: LOAD Mask */

/* CMSDK_WATCHDOG VALUE Register Definitions */
#define CMSDK_Watchdog_VALUE_Pos              0                                              /* CMSDK_Watchdog VALUE: VALUE Position */
#define CMSDK_Watchdog_VALUE_Msk             (0xFFFFFFFFUL /*<< CMSDK_Watchdog_VALUE_Pos*/)  /* CMSDK_Watchdog VALUE: VALUE Mask */

/* CMSDK_WATCHDOG CTRL Register Definitions */
#define CMSDK_Watchdog_CTRL_RESEN_Pos         1                                              /* CMSDK_Watchdog CTRL_RESEN: Enable Reset Output Position */
#define CMSDK_Watchdog_CTRL_RESEN_Msk        (0x1UL << CMSDK_Watchdog_CTRL_RESEN_Pos)        /* CMSDK_Watchdog CTRL_RESEN: Enable Reset Output Mask */

#define CMSDK_Watchdog_CTRL_INTEN_Pos         0                                              /* CMSDK_Watchdog CTRL_INTEN: Int Enable Position */
#define CMSDK_Watchdog_CTRL_INTEN_Msk        (0x1UL /*<< CMSDK_Watchdog_CTRL_INTEN_Pos*/)    /* CMSDK_Watchdog CTRL_INTEN: Int Enable Mask */

/* CMSDK_WATCHDOG INTCLR Register Definitions */
#define CMSDK_Watchdog_INTCLR_Pos             0                                              /* CMSDK_Watchdog INTCLR: Int Clear Position */
#define CMSDK_Watchdog_INTCLR_Msk            (0x1UL /*<< CMSDK_Watchdog_INTCLR_Pos*/)        /* CMSDK_Watchdog INTCLR: Int Clear Mask */

/* CMSDK_WATCHDOG RAWINTSTAT Register Definitions */
#define CMSDK_Watchdog_RAWINTSTAT_Pos         0                                              /* CMSDK_Watchdog RAWINTSTAT: Raw Int Status Position */
#define CMSDK_Watchdog_RAWINTSTAT_Msk        (0x1UL /*<< CMSDK_Watchdog_RAWINTSTAT_Pos*/)    /* CMSDK_Watchdog RAWINTSTAT: Raw Int Status Mask */

/* CMSDK_WATCHDOG MASKINTSTAT Register Definitions */
#define CMSDK_Watchdog_MASKINTSTAT_Pos        0                                              /* CMSDK_Watchdog MASKINTSTAT: Mask Int Status Position */
#define CMSDK_Watchdog_MASKINTSTAT_Msk       (0x1UL /*<< CMSDK_Watchdog_MASKINTSTAT_Pos*/)   /* CMSDK_Watchdog MASKINTSTAT: Mask Int Status Mask */

/* CMSDK_WATCHDOG LOCK Register Definitions */
#define CMSDK_Watchdog_LOCK_Pos               0                                              /* CMSDK_Watchdog LOCK: LOCK Position */
#define CMSDK_Watchdog_LOCK_Msk              (0x1UL /*<< CMSDK_Watchdog_LOCK_Pos*/)          /* CMSDK_Watchdog LOCK: LOCK Mask */

/* CMSDK_WATCHDOG INTEGTESTEN Register Definitions */
#define CMSDK_Watchdog_INTEGTESTEN_Pos        0                                              /* CMSDK_Watchdog INTEGTESTEN: Integration Test Enable Position */
#define CMSDK_Watchdog_INTEGTESTEN_Msk       (0x1UL /*<< CMSDK_Watchdog_INTEGTESTEN_Pos*/)   /* CMSDK_Watchdog INTEGTESTEN: Integration Test Enable Mask */

/* CMSDK_WATCHDOG INTEGTESTOUTSET Register Definitions */
#define CMSDK_Watchdog_INTEGTESTOUTSET_Pos    1                                                /* CMSDK_Watchdog INTEGTESTOUTSET: Integration Test Output Set Position */
#define CMSDK_Watchdog_INTEGTESTOUTSET_Msk   (0x1UL /*<< CMSDK_Watchdog_INTEGTESTOUTSET_Pos*/) /* CMSDK_Watchdog INTEGTESTOUTSET: Integration Test Output Set Mask */



/* --------------------  End of section using anonymous unions  ------------------- */
#if   defined (__CC_ARM)
  #pragma pop
#elif defined (__ICCARM__)
  /* leave anonymous unions enabled */
#elif (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic pop
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning restore
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif




/* ================================================================================ */
/* ================              Peripheral memory map             ================ */
/* ================================================================================ */

/* Peripheral and SRAM base address */
#define CMSDK_FLASH_BASE        (0x00000000UL)
#define CMSDK_SRAM_BASE         (0x20000000UL)
#define CMSDK_PERIPH_BASE       (0x40000000UL)

#define CMSDK_RAM_BASE          (0x20000000UL)
#define CMSDK_APB_BASE          (0x40000000UL)
#define CMSDK_AHB_BASE          (0x40010000UL)
#define CMSDK_S_APB_BASE        (0x50000000UL)

/* APB peripherals */
#define CMSDK_TIMER0_BASE       (CMSDK_APB_BASE + 0x0000UL)
#define CMSDK_TIMER1_BASE       (CMSDK_APB_BASE + 0x1000UL)
#define CMSDK_DUALTIMER_BASE    (CMSDK_APB_BASE + 0x2000UL)
#define CMSDK_DUALTIMER_1_BASE  (CMSDK_DUALTIMER_BASE)
#define CMSDK_DUALTIMER_2_BASE  (CMSDK_DUALTIMER_BASE + 0x20UL)
#define CMSDK_UART0_BASE        (CMSDK_APB_BASE + 0x4000UL)
#define CMSDK_UART1_BASE        (CMSDK_APB_BASE + 0x5000UL)
#define CMSDK_UART2_BASE        (CMSDK_APB_BASE + 0x6000UL)
#define CMSDK_WATCHDOG_BASE     (CMSDK_APB_BASE + 0x8000UL)

/* AHB peripherals */
#define CMSDK_GPIO0_BASE        (CMSDK_AHB_BASE + 0x0000UL)
#define CMSDK_GPIO1_BASE        (CMSDK_AHB_BASE + 0x1000UL)
#define CMSDK_SYSCTRL_BASE      (CMSDK_AHB_BASE + 0xF000UL)

/* Secure APB peripherals */
#define CMSDK_SECURETIMER0_BASE (CMSDK_S_APB_BASE + 0x0000UL)
#define CMSDK_SECURETIMER1_BASE (CMSDK_S_APB_BASE + 0x1000UL)


/* ================================================================================ */
/* ================             Peripheral declaration             ================ */
/* ================================================================================ */

#define CMSDK_UART0             ((CMSDK_UART_TypeDef              *) CMSDK_UART0_BASE       )
#define CMSDK_UART1             ((CMSDK_UART_TypeDef              *) CMSDK_UART1_BASE       )
#define CMSDK_UART2             ((CMSDK_UART_TypeDef              *) CMSDK_UART2_BASE       )
#define CMSDK_TIMER0            ((CMSDK_TIMER_TypeDef             *) CMSDK_TIMER0_BASE      )
#define CMSDK_TIMER1            ((CMSDK_TIMER_TypeDef             *) CMSDK_TIMER1_BASE      )
#define CMSDK_DUALTIMER         ((CMSDK_DUALTIMER_BOTH_TypeDef    *) CMSDK_DUALTIMER_BASE   )
#define CMSDK_DUALTIMER1        ((CMSDK_DUALTIMER_SINGLE_TypeDef  *) CMSDK_DUALTIMER_1_BASE )
#define CMSDK_DUALTIMER2        ((CMSDK_DUALTIMER_SINGLE_TypeDef  *) CMSDK_DUALTIMER_2_BASE )
#define CMSDK_WATCHDOG          ((CMSDK_WATCHDOG_TypeDef          *) CMSDK_WATCHDOG_BASE    )
#define CMSDK_GPIO0             ((CMSDK_GPIO_TypeDef              *) CMSDK_GPIO0_BASE       )
#define CMSDK_GPIO1             ((CMSDK_GPIO_TypeDef              *) CMSDK_GPIO1_BASE       )
#define CMSDK_SYSCON            ((CMSDK_SYSCON_TypeDef            *) CMSDK_SYSCTRL_BASE     )
#define CMSDK_SECURETIMER0      ((CMSDK_TIMER_TypeDef             *) CMSDK_SECURETIMER0_BASE)
#define CMSDK_SECURETIMER1      ((CMSDK_TIMER_TypeDef             *) CMSDK_SECURETIMER1_BASE)


#ifdef __cplusplus
}
#endif

#endif  /* CMSDK_ARMv8MML_H */
