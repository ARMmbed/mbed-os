/*
 * Copyright (c) 2009-2018 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This file is derivative of CMSIS V5.00 ARMCM3.h
 */

#ifndef CMSDK_CM3DS_H
#define CMSDK_CM3DS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* -------------------------  Interrupt Number Definition  ------------------------ */

typedef enum IRQn
{
/* -------------------  Cortex-M3 Processor Exceptions Numbers  ------------------- */
  NonMaskableInt_IRQn           = -14,        /*  2 Non Maskable Interrupt          */
  HardFault_IRQn                = -13,        /*  3 HardFault Interrupt             */
  MemoryManagement_IRQn         = -12,        /*  4 Memory Management Interrupt     */
  BusFault_IRQn                 = -11,        /*  5 Bus Fault Interrupt             */
  UsageFault_IRQn               = -10,        /*  6 Usage Fault Interrupt           */
  SVCall_IRQn                   =  -5,        /* 11 SV Call Interrupt               */
  DebugMonitor_IRQn             =  -4,        /* 12 Debug Monitor Interrupt         */
  PendSV_IRQn                   =  -2,        /* 14 Pend SV Interrupt               */
  SysTick_IRQn                  =  -1,        /* 15 System Tick Interrupt           */

/* ----------------------  CMSDK_CM3 Specific Interrupt Numbers  ------------------ */
  UART0_IRQn                    = 0,       /* UART 0 RX and TX Combined Interrupt   */
  Spare_IRQn                    = 1,       /* Undefined                             */
  UART1_IRQn                    = 2,       /* UART 1 RX and TX Combined Interrupt   */
  APB_SLAVE_0_IRQ               = 3,       /* Reserved for APB Slave                */
  APB_SLAVE_1_IRQ               = 4,       /* Reserved for APB Slave                */
  RTC_IRQn                      = 5,       /* RTC Interrupt                         */
  PORT0_ALL_IRQn                = 6,       /* GPIO Port 0 combined Interrupt        */
  PORT1_ALL_IRQn                = 7,       /* GPIO Port 1 combined Interrupt        */
  TIMER0_IRQn                   = 8,       /* TIMER 0 Interrupt                     */
  TIMER1_IRQn                   = 9,       /* TIMER 1 Interrupt                     */
  DUALTIMER_IRQn                = 10,      /* Dual Timer Interrupt                  */
  APB_SLAVE_2_IRQ               = 11,      /* Reserved for APB Slave                */
  UARTOVF_IRQn                  = 12,      /* UART 0,1,2 Overflow Interrupt         */
  APB_SLAVE_3_IRQ               = 13,      /* Reserved for APB Slave                */
  RESERVED0_IRQn                = 14,      /* Reserved                              */
  TSC_IRQn                      = 15,      /* Touch Screen Interrupt                */
  PORT0_0_IRQn                  = 16,      /* GPIO Port 0 pin 0 Handler             */
  PORT0_1_IRQn                  = 17,      /* GPIO Port 0 pin 1 Handler             */
  PORT0_2_IRQn                  = 18,      /* GPIO Port 0 pin 2 Handler             */
  PORT0_3_IRQn                  = 19,      /* GPIO Port 0 pin 3 Handler             */
  PORT0_4_IRQn                  = 20,      /* GPIO Port 0 pin 4 Handler             */
  PORT0_5_IRQn                  = 21,      /* GPIO Port 0 pin 5 Handler             */
  PORT0_6_IRQn                  = 22,      /* GPIO Port 0 pin 6 Handler             */
  PORT0_7_IRQn                  = 23,      /* GPIO Port 0 pin 7 Handler             */
  PORT0_8_IRQn                  = 24,      /* GPIO Port 0 pin 8 Handler             */
  PORT0_9_IRQn                  = 25,      /* GPIO Port 0 pin 9 Handler             */
  PORT0_10_IRQn                 = 26,      /* GPIO Port 0 pin 10 Handler            */
  PORT0_11_IRQn                 = 27,      /* GPIO Port 0 pin 11 Handler            */
  PORT0_12_IRQn                 = 28,      /* GPIO Port 0 pin 12 Handler            */
  PORT0_13_IRQn                 = 29,      /* GPIO Port 0 pin 13 Handler            */
  PORT0_14_IRQn                 = 30,      /* GPIO Port 0 pin 14 Handler            */
  PORT0_15_IRQn                 = 31,      /* GPIO Port 0 pin 15 Handler            */
  FLASH0_IRQn                   = 32,      /* Reserved for Flash                    */
  FLASH1_IRQn                   = 33,      /* Reserved for Flash                    */
  RESERVED1_IRQn                = 34,      /* Reserved for Cordio                   */
  RESERVED2_IRQn                = 35,      /* Reserved for Cordio                   */
  RESERVED3_IRQn                = 36,      /* Reserved for Cordio                   */
  RESERVED4_IRQn                = 37,      /* Reserved for Cordio                   */
  RESERVED5_IRQn                = 38,      /* Reserved for Cordio                   */
  RESERVED6_IRQn                = 39,      /* Reserved for Cordio                   */
  RESERVED7_IRQn                = 40,      /* Reserved for Cordio                   */
  RESERVED8_IRQn                = 41,      /* Reserved for Cordio                   */
  PORT2_ALL_IRQn                = 42,      /* GPIO Port 2 combined Interrupt        */
  PORT3_ALL_IRQn                = 43,      /* GPIO Port 3 combined Interrupt        */
  TRNG_IRQn                     = 44,      /* Random number generator Interrupt     */
  UART2_IRQn                    = 45,      /* UART 2 RX and TX Combined Interrupt   */
  UART3_IRQn                    = 46,      /* UART 3 RX and TX Combined Interrupt   */
  ETHERNET_IRQn                 = 47,      /* Ethernet interrupt     t.b.a.         */
  I2S_IRQn                      = 48,      /* I2S Interrupt                         */
  MPS2_SPI0_IRQn                = 49,      /* SPI Interrupt (spi header)            */
  MPS2_SPI1_IRQn                = 50,      /* SPI Interrupt (clcd)                  */
  MPS2_SPI2_IRQn                = 51,      /* SPI Interrupt (spi 1 ADC replacement) */
  MPS2_SPI3_IRQn                = 52,      /* SPI Interrupt (shield 0)              */
  MPS2_SPI4_IRQn                = 53,      /* SPI Interrupt (shield 1)              */
  PORT4_ALL_IRQn                = 54,      /* GPIO Port 4 combined Interrupt        */
  PORT5_ALL_IRQn                = 55,      /* GPIO Port 5 combined Interrupt        */
  UART4_IRQn                    = 56       /* UART 4 RX and TX Combined Interrupt   */
} IRQn_Type;

/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

/* --------  Configuration of the Cortex-M3 Processor and Core Peripherals  ------- */
#define __CM3DS_REV                 0x0201U   /* Core revision r2p1 */
#define __MPU_PRESENT             1         /* MPU present or not */
#define __VTOR_PRESENT            1         /* VTOR present or not */
#define __NVIC_PRIO_BITS          3         /* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig    0         /* Set to 1 if different SysTick Config is used */

#include <core_cm3.h>                         /* Processor and core peripherals                  */
#include "system_CMSDK_CM3DS.h"               /* System Header                                   */


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

/*------------- System Control (SYSCON) --------------------------------------*/
typedef struct
{
  __IO   uint32_t  REMAP;                    /* Offset: 0x000 (R/W) Remap Control Register */
  __IO   uint32_t  PMUCTRL;                  /* Offset: 0x004 (R/W) PMU Control Register */
  __IO   uint32_t  RESETOP;                  /* Offset: 0x008 (R/W) Reset Option Register */
  __IO   uint32_t  EMICTRL;                  /* Offset: 0x00C (R/W) EMI Control Register */
  __IO   uint32_t  RSTINFO;                  /* Offset: 0x010 (R/W) Reset Information Register */
         uint32_t  RESERVED0[3];
  __IO   uint32_t  AHBPER0SET;               /* Offset: 0x020 (R/W)AHB peripheral access control set */
  __IO   uint32_t  AHBPER0CLR;               /* Offset: 0x024 (R/W)AHB peripheral access control clear */
         uint32_t  RESERVED1[2];
  __IO   uint32_t  APBPER0SET;               /* Offset: 0x030 (R/W)APB peripheral access control set */
  __IO   uint32_t  APBPER0CLR;               /* Offset: 0x034 (R/W)APB peripheral access control clear */
         uint32_t  RESERVED2[2];
  __IO   uint32_t  MAINCLK;                  /* Offset: 0x040 (R/W) Main Clock Control Register */
  __IO   uint32_t  AUXCLK;                   /* Offset: 0x044 (R/W) Auxiliary / RTC Control Register */
  __IO   uint32_t  PLLCTRL;                  /* Offset: 0x048 (R/W) PLL Control Register */
  __IO   uint32_t  PLLSTATUS;                /* Offset: 0x04C (R/W) PLL Status Register */
  __IO   uint32_t  SLEEPCFG;                 /* Offset: 0x050 (R/W) Sleep Control Register */
  __IO   uint32_t  FLASHAUXCFG;              /* Offset: 0x054 (R/W) Flash auxiliary settings Control Register */
         uint32_t  RESERVED3[10];
  __IO   uint32_t  AHBCLKCFG0SET;            /* Offset: 0x080 (R/W) AHB Peripheral Clock set in Active state */
  __IO   uint32_t  AHBCLKCFG0CLR;            /* Offset: 0x084 (R/W) AHB Peripheral Clock clear in Active state */
  __IO   uint32_t  AHBCLKCFG1SET;            /* Offset: 0x088 (R/W) AHB Peripheral Clock set in Sleep state */
  __IO   uint32_t  AHBCLKCFG1CLR;            /* Offset: 0x08C (R/W) AHB Peripheral Clock clear in Sleep state */
  __IO   uint32_t  AHBCLKCFG2SET;            /* Offset: 0x090 (R/W) AHB Peripheral Clock set in Deep Sleep state */
  __IO   uint32_t  AHBCLKCFG2CLR;            /* Offset: 0x094 (R/W) AHB Peripheral Clock clear in Deep Sleep state */
         uint32_t  RESERVED4[2];
  __IO   uint32_t  APBCLKCFG0SET;            /* Offset: 0x0A0 (R/W) APB Peripheral Clock set in Active state */
  __IO   uint32_t  APBCLKCFG0CLR;            /* Offset: 0x0A4 (R/W) APB Peripheral Clock clear in Active state */
  __IO   uint32_t  APBCLKCFG1SET;            /* Offset: 0x0A8 (R/W) APB Peripheral Clock set in Sleep state */
  __IO   uint32_t  APBCLKCFG1CLR;            /* Offset: 0x0AC (R/W) APB Peripheral Clock clear in Sleep state */
  __IO   uint32_t  APBCLKCFG2SET;            /* Offset: 0x0B0 (R/W) APB Peripheral Clock set in Deep Sleep state */
  __IO   uint32_t  APBCLKCFG2CLR;            /* Offset: 0x0B4 (R/W) APB Peripheral Clock clear in Deep Sleep state */
         uint32_t  RESERVED5[2];
  __IO   uint32_t  AHBPRST0SET;              /* Offset: 0x0C0 (R/W) AHB Peripheral reset select set */
  __IO   uint32_t  AHBPRST0CLR;              /* Offset: 0x0C4 (R/W) AHB Peripheral reset select clear */
  __IO   uint32_t  APBPRST0SET;              /* Offset: 0x0C8 (R/W) APB Peripheral reset select set */
  __IO   uint32_t  APBPRST0CLR;              /* Offset: 0x0CC (R/W) APB Peripheral reset select clear */
  __IO   uint32_t  PWRDNCFG0SET;             /* Offset: 0x0D0 (R/W) AHB Power down sleep wakeup source set */
  __IO   uint32_t  PWRDNCFG0CLR;             /* Offset: 0x0D4 (R/W) AHB Power down sleep wakeup source clear */
  __IO   uint32_t  PWRDNCFG1SET;             /* Offset: 0x0D8 (R/W) APB Power down sleep wakeup source set */
  __IO   uint32_t  PWRDNCFG1CLR;             /* Offset: 0x0DC (R/W) APB Power down sleep wakeup source clear */
  __O    uint32_t  RTCRESET;                 /* Offset: 0x0E0 ( /W) RTC reset */
  __IO   uint32_t  EVENTCFG;                 /* Offset: 0x0E4 (R/W) Event interface Control Register */
         uint32_t  RESERVED6[2];
  __IO   uint32_t  PWROVRIDE0;               /* Offset: 0x0F0 (R/W) SRAM Power control overide */
  __IO   uint32_t  PWROVRIDE1;               /* Offset: 0x0F4 (R/W) Embedded Flash Power control overide */
  __I    uint32_t  MEMORYSTATUS;             /* Offset: 0x0F8 (R/ ) Memory Status Register */
         uint32_t  RESERVED7[1];
  __IO   uint32_t  GPIOPADCFG0;              /* Offset: 0x100 (R/W) IO pad settings */
  __IO   uint32_t  GPIOPADCFG1;              /* Offset: 0x104 (R/W) IO pad settings */
  __IO   uint32_t  TESTMODECFG;              /* Offset: 0x108 (R/W) Testmode boot bypass */
} CMSDK_SYSCON_TypeDef;

#define CMSDK_SYSCON_REMAP_Pos                 0
#define CMSDK_SYSCON_REMAP_Msk                 (0x01ul << CMSDK_SYSCON_REMAP_Pos)               /* CMSDK_SYSCON MEME_CTRL: REMAP Mask */

#define CMSDK_SYSCON_PMUCTRL_EN_Pos            0
#define CMSDK_SYSCON_PMUCTRL_EN_Msk            (0x01ul << CMSDK_SYSCON_PMUCTRL_EN_Pos)          /* CMSDK_SYSCON PMUCTRL: PMUCTRL ENABLE Mask */

#define CMSDK_SYSCON_LOCKUPRST_RESETOP_Pos     0
#define CMSDK_SYSCON_LOCKUPRST_RESETOP_Msk     (0x01ul << CMSDK_SYSCON_LOCKUPRST_RESETOP_Pos)   /* CMSDK_SYSCON SYS_CTRL: LOCKUP RESET ENABLE Mask */

#define CMSDK_SYSCON_EMICTRL_SIZE_Pos          24
#define CMSDK_SYSCON_EMICTRL_SIZE_Msk          (0x00001ul << CMSDK_SYSCON_EMICTRL_SIZE_Pos)     /* CMSDK_SYSCON EMICTRL: SIZE Mask */

#define CMSDK_SYSCON_EMICTRL_TACYC_Pos         16
#define CMSDK_SYSCON_EMICTRL_TACYC_Msk         (0x00007ul << CMSDK_SYSCON_EMICTRL_TACYC_Pos)    /* CMSDK_SYSCON EMICTRL: TURNAROUNDCYCLE Mask */

#define CMSDK_SYSCON_EMICTRL_WCYC_Pos          8
#define CMSDK_SYSCON_EMICTRL_WCYC_Msk          (0x00003ul << CMSDK_SYSCON_EMICTRL_WCYC_Pos)     /* CMSDK_SYSCON EMICTRL: WRITECYCLE Mask */

#define CMSDK_SYSCON_EMICTRL_RCYC_Pos          0
#define CMSDK_SYSCON_EMICTRL_RCYC_Msk          (0x00007ul << CMSDK_SYSCON_EMICTRL_RCYC_Pos)     /* CMSDK_SYSCON EMICTRL: READCYCLE Mask */

#define CMSDK_SYSCON_RSTINFO_SYSRESETREQ_Pos   0
#define CMSDK_SYSCON_RSTINFO_SYSRESETREQ_Msk   (0x00001ul << CMSDK_SYSCON_RSTINFO_SYSRESETREQ_Pos) /* CMSDK_SYSCON RSTINFO: SYSRESETREQ Mask */

#define CMSDK_SYSCON_RSTINFO_WDOGRESETREQ_Pos  1
#define CMSDK_SYSCON_RSTINFO_WDOGRESETREQ_Msk  (0x00001ul << CMSDK_SYSCON_RSTINFO_WDOGRESETREQ_Pos) /* CMSDK_SYSCON RSTINFO: WDOGRESETREQ Mask */

#define CMSDK_SYSCON_RSTINFO_LOCKUPRESET_Pos   2
#define CMSDK_SYSCON_RSTINFO_LOCKUPRESET_Msk   (0x00001ul << CMSDK_SYSCON_RSTINFO_LOCKUPRESET_Pos) /* CMSDK_SYSCON RSTINFO: LOCKUPRESET Mask */

/*------------------- WATCHDOG ----------------------------------------------*/
typedef struct
{

  __IO    uint32_t  LOAD;                   /* Offset: 0x000 (R/W) Watchdog Load Register */
  __I     uint32_t  VALUE;                  /* Offset: 0x004 (R/ ) Watchdog Value Register */
  __IO    uint32_t  CTRL;                   /* Offset: 0x008 (R/W) Watchdog Control Register */
  __O     uint32_t  INTCLR;                 /* Offset: 0x00C ( /W) Watchdog Clear Interrupt Register */
  __I     uint32_t  RAWINTSTAT;             /* Offset: 0x010 (R/ ) Watchdog Raw Interrupt Status Register */
  __I     uint32_t  MASKINTSTAT;            /* Offset: 0x014 (R/ ) Watchdog Interrupt Status Register */
        uint32_t  RESERVED0[762];
  __IO    uint32_t  LOCK;                   /* Offset: 0xC00 (R/W) Watchdog Lock Register */
        uint32_t  RESERVED1[191];
  __IO    uint32_t  ITCR;                   /* Offset: 0xF00 (R/W) Watchdog Integration Test Control Register */
  __O     uint32_t  ITOP;                   /* Offset: 0xF04 ( /W) Watchdog Integration Test Output Set Register */
}CMSDK_WATCHDOG_TypeDef;

#define CMSDK_WATCHDOG_LOAD_Pos               0                                              /* CMSDK_WATCHDOG LOAD: LOAD Position */
#define CMSDK_WATCHDOG_LOAD_Msk              (0xFFFFFFFFul << CMSDK_WATCHDOG_LOAD_Pos)       /* CMSDK_WATCHDOG LOAD: LOAD Mask */

#define CMSDK_WATCHDOG_VALUE_Pos              0                                              /* CMSDK_WATCHDOG VALUE: VALUE Position */
#define CMSDK_WATCHDOG_VALUE_Msk             (0xFFFFFFFFul << CMSDK_WATCHDOG_VALUE_Pos)      /* CMSDK_WATCHDOG VALUE: VALUE Mask */

#define CMSDK_WATCHDOG_CTRL_RESEN_Pos         1                                              /* CMSDK_WATCHDOG CTRL_RESEN: Enable Reset Output Position */
#define CMSDK_WATCHDOG_CTRL_RESEN_Msk        (0x1ul << CMSDK_WATCHDOG_CTRL_RESEN_Pos)        /* CMSDK_WATCHDOG CTRL_RESEN: Enable Reset Output Mask */

#define CMSDK_WATCHDOG_CTRL_INTEN_Pos         0                                              /* CMSDK_WATCHDOG CTRL_INTEN: Int Enable Position */
#define CMSDK_WATCHDOG_CTRL_INTEN_Msk        (0x1ul << CMSDK_WATCHDOG_CTRL_INTEN_Pos)        /* CMSDK_WATCHDOG CTRL_INTEN: Int Enable Mask */

#define CMSDK_WATCHDOG_INTCLR_Pos             0                                              /* CMSDK_WATCHDOG INTCLR: Int Clear Position */
#define CMSDK_WATCHDOG_INTCLR_Msk            (0x1ul << CMSDK_WATCHDOG_INTCLR_Pos)            /* CMSDK_WATCHDOG INTCLR: Int Clear Mask */

#define CMSDK_WATCHDOG_RAWINTSTAT_Pos         0                                              /* CMSDK_WATCHDOG RAWINTSTAT: Raw Int Status Position */
#define CMSDK_WATCHDOG_RAWINTSTAT_Msk        (0x1ul << CMSDK_WATCHDOG_RAWINTSTAT_Pos)        /* CMSDK_WATCHDOG RAWINTSTAT: Raw Int Status Mask */

#define CMSDK_WATCHDOG_MASKINTSTAT_Pos        0                                              /* CMSDK_WATCHDOG MASKINTSTAT: Mask Int Status Position */
#define CMSDK_WATCHDOG_MASKINTSTAT_Msk       (0x1ul << CMSDK_WATCHDOG_MASKINTSTAT_Pos)       /* CMSDK_WATCHDOG MASKINTSTAT: Mask Int Status Mask */

#define CMSDK_WATCHDOG_LOCK_Pos               0                                              /* CMSDK_WATCHDOG LOCK: LOCK Position */
#define CMSDK_WATCHDOG_LOCK_Msk              (0x1ul << CMSDK_WATCHDOG_LOCK_Pos)              /* CMSDK_WATCHDOG LOCK: LOCK Mask */

#define CMSDK_WATCHDOG_INTEGTESTEN_Pos        0                                              /* CMSDK_WATCHDOG INTEGTESTEN: Integration Test Enable Position */
#define CMSDK_WATCHDOG_INTEGTESTEN_Msk       (0x1ul << CMSDK_WATCHDOG_INTEGTESTEN_Pos)       /* CMSDK_WATCHDOG INTEGTESTEN: Integration Test Enable Mask */

#define CMSDK_WATCHDOG_INTEGTESTOUTSET_Pos    1                                              /* CMSDK_WATCHDOG INTEGTESTOUTSET: Integration Test Output Set Position */
#define CMSDK_WATCHDOG_INTEGTESTOUTSET_Msk   (0x1ul << CMSDK_WATCHDOG_INTEGTESTOUTSET_Pos)   /* CMSDK_WATCHDOG INTEGTESTOUTSET: Integration Test Output Set Mask */

/*------------------------- Real Time Clock(RTC) ----------------------------------------------*/
typedef struct
{
    __I  uint32_t RTCDR;                     /* 0x00 RO RTC Data Register */
    __IO uint32_t RTCMR;                     /* 0x04 RW RTC Match Register */
    __IO uint32_t RTCLR;                     /* 0x08 RW RTC Load Register */
    __IO uint32_t RTCCR;                     /* 0x0C RW RTC Control Register */
    __IO uint32_t RTCIMSC;                   /* 0x10 RW RTC Inerrupt Mask Set and Clear Register */
    __I  uint32_t RTCRIS;                    /* 0x14 RO RTC Raw Inerrupt Status Register */
    __I  uint32_t RTCMIS;                    /* 0x18 RO RTC Masked Inerrupt Status Register */
    __O  uint32_t RTCICR;                    /* 0x1C WO RTC Interrupt Clear Register */
} CMSDK_RTC_TypeDef;

#define CMSDK_RTC_ENABLE_Pos                  0                                /* CMSDK_RTC Enable: Real Time Clock Enable Position */
#define CMSDK_RTC_ENABLE_Msk                  (0x1ul << CMSDK_RTC_ENABLE_Pos)  /* CMSDK_RTC Enable: Real Time Clock Enable Mask */

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
#define CMSDK_FLASH_BASE        (0x00000000UL)
#define CMSDK_SRAM_BASE         (0x20000000UL)
#define CMSDK_PERIPH_BASE       (0x40000000UL)

#define CMSDK_RAM_BASE          (0x20000000UL)
#define CMSDK_APB_BASE          (0x40000000UL)
#define CMSDK_AHB_BASE          (0x40010000UL)

/* APB peripherals */
#define CMSDK_TIMER0_BASE       (CMSDK_APB_BASE + 0x0000UL)
#define CMSDK_TIMER1_BASE       (CMSDK_APB_BASE + 0x1000UL)
#define CMSDK_DUALTIMER_BASE    (CMSDK_APB_BASE + 0x2000UL)
#define CMSDK_DUALTIMER_1_BASE  (CMSDK_DUALTIMER_BASE)
#define CMSDK_DUALTIMER_2_BASE  (CMSDK_DUALTIMER_BASE + 0x20UL)
#define CMSDK_UART0_BASE        (CMSDK_APB_BASE + 0x4000UL)
#define CMSDK_UART1_BASE        (CMSDK_APB_BASE + 0x5000UL)
#define CMSDK_UART2_BASE        (0x4002C000UL)
#define CMSDK_UART3_BASE        (0x4002D000UL)
#define CMSDK_UART4_BASE        (0x4002E000UL)
#define CMSDK_RTC_BASE          (CMSDK_APB_BASE + 0x6000UL)
#define CMSDK_WATCHDOG_BASE     (CMSDK_APB_BASE + 0x8000UL)

/* AHB peripherals */
#define CMSDK_GPIO0_BASE        (CMSDK_AHB_BASE + 0x0000UL)
#define CMSDK_GPIO1_BASE        (CMSDK_AHB_BASE + 0x1000UL)
#define CMSDK_GPIO2_BASE        (CMSDK_AHB_BASE + 0x2000UL)
#define CMSDK_GPIO3_BASE        (CMSDK_AHB_BASE + 0x3000UL)
#define CMSDK_GPIO4_BASE        (0x40030000UL)
#define CMSDK_GPIO5_BASE        (0x40031000UL)
#define CMSDK_SYSCTRL_BASE      (CMSDK_AHB_BASE + 0xF000UL)



/* ================================================================================ */
/* ================             Peripheral declaration             ================ */
/* ================================================================================ */

#define CMSDK_RTC               ((CMSDK_RTC_TypeDef    *) CMSDK_RTC_BASE )
#define CMSDK_WATCHDOG          ((CMSDK_WATCHDOG_TypeDef  *) CMSDK_WATCHDOG_BASE )
#define CMSDK_SYSCON            ((CMSDK_SYSCON_TypeDef *) CMSDK_SYSCTRL_BASE )

#ifdef __cplusplus
}
#endif

#endif  /* CMSDK_CM3DS_H */
