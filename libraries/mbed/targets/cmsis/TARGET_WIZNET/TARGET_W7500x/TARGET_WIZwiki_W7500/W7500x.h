/**************************************************************************//**
 * @file     W7500x.h
 * @brief    CMSIS Cortex-M0 Core Peripheral Access Layer Header File for
 *           Device W7500x
 * @version  V3.01
 * @date     06. March 2012
 *
 * @note
 * Copyright (C) 2010-2012 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M
 * processor based microcontrollers.  This file can be freely distributed
 * within development tools that are supporting such ARM based processors.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/


#ifndef W7500x_H
#define W7500x_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup W7500x_Definitions W7500x Definitions
  This file defines all structures and symbols for W7500x:
    - registers and bitfields
    - peripheral base address
    - peripheral ID
    - Peripheral definitions
  @{
*/


/******************************************************************************/
/*                Processor and Core Peripherals                              */
/******************************************************************************/
/** @addtogroup W7500x_CMSIS Device CMSIS Definitions
  Configuration of the Cortex-M0 Processor and Core Peripherals
  @{
*/

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */

typedef enum IRQn
{
/******  Cortex-M0 Processor Exceptions Numbers **************************************************/

/* ToDo: use this Cortex interrupt numbers if your device is a CORTEX-M0 device                  */
  NonMaskableInt_IRQn          = -14,      /*!<  2 Cortex-M0 Non Maskable Interrupt              */
  HardFault_IRQn               = -13,      /*!<  3 Cortex-M0 Hard Fault Interrupt                */
  SVCall_IRQn                  = -5,       /*!< 11 Cortex-M0 SV Call Interrupt                   */
  PendSV_IRQn                  = -2,       /*!< 14 Cortex-M0 Pend SV Interrupt                   */
  SysTick_IRQn                 = -1,       /*!< 15 Cortex-M0 System Tick Interrupt               */
/******  W7500x Specific Interrupt Numbers *********************************************************/
  SSP0_IRQn                    = 0,        /*!< SSP 0 Interrupt                                   */ 
  SSP1_IRQn                    = 1,        /*!< SSP 1 Interrupt                                   */ 
  UART0_IRQn                   = 2,        /*!< UART 0 Interrupt                                  */
  UART1_IRQn                   = 3,        /*!< UART 1 Interrupt                                  */
  UART2_IRQn                   = 4,        /*!< UART 2 Interrupt                                  */
  I2C0_IRQn                    = 5,        /*!< I2C 0 Interrupt                                   */ 
  I2C1_IRQn                    = 6,        /*!< I2C 1 Interrupt                                   */ 
  PORT0_IRQn                   = 7,        /*!< Port 1 combined Interrupt                         */  
  PORT1_IRQn                   = 8,        /*!< Port 2 combined Interrupt                         */  
  PORT2_IRQn                   = 9,        /*!< Port 2 combined Interrupt                         */  
  PORT3_IRQn                   = 10,       /*!< Port 2 combined Interrupt                         */  
  DMA_IRQn                     = 11,       /*!< DMA combined Interrupt                            */   
  DUALTIMER0_IRQn              = 12,       /*!< Dual Timer 0 Interrupt                            */
  DUALTIMER1_IRQn              = 13,       /*!< Dual Timer 1 Interrupt                            */
  PWM0_IRQn                    = 14,       /*!< PWM 0 Interrupt                                   */ 
  PWM1_IRQn                    = 15,       /*!< PWM 1 Interrupt                                   */ 
  PWM2_IRQn                    = 16,       /*!< PWM 2 Interrupt                                   */ 
  PWM3_IRQn                    = 17,       /*!< PWM 3 Interrupt                                   */ 
  PWM4_IRQn                    = 18,       /*!< PWM 4 Interrupt                                   */ 
  PWM5_IRQn                    = 19,       /*!< PWM 5 Interrupt                                   */ 
  PWM6_IRQn                    = 20,       /*!< PWM 6 Interrupt                                   */ 
  PWM7_IRQn                    = 21,       /*!< PWM 7 Interrupt                                   */ 
  RTC_IRQn                     = 22,       /*!< RTC Interrupt                                     */ 
  ADC_IRQn                     = 23,       /*!< ADC Interrupt                                     */ 
  WZTOE_IRQn                   = 24,       /*!< WZTOE Interrupt                                   */       
  EXTI_IRQn                    = 25        /*!< EXTI Interrupt                                   */       
} IRQn_Type;

/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M0 Processor and Core Peripherals */
#define __CM0_REV                 0x0000    /*!< Core Revision r0p0                               */
#define __NVIC_PRIO_BITS          2         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */
#define __MPU_PRESENT             0         /*!< MPU present or not                               */

/*@}*/ /* end of group W7500x_CMSIS */


#include "core_cm0.h"                    /* Cortex-M0 processor and core peripherals           */
#include "system_W7500x.h"               /* W7500x System include file                          */


/** @addtogroup Exported_types
  * @{
  */  

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;
typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;





/**
  * @}
  */




/** @addtogroup Peripheral_registers_structures
  * @{
  */   

/**
  * @brief Clock Reset Generator
  */
typedef struct
{
  __IO  uint32_t  OSC_PDR;            /*!< Oscillator power down register,               Address offset : 0x00 */
        uint32_t  RESERVED0[3];
  __IO  uint32_t  PLL_PDR;            /*!< PLL power down register,                      Address offset : 0x10 */
  __IO  uint32_t  PLL_FCR;            /*!< PLL frequency calculating register,           Address offset : 0x14 */
  __IO  uint32_t  PLL_OER;            /*!< PLL output enable register,                   Address offset : 0x18 */
  __IO  uint32_t  PLL_BPR;            /*!< PLL bypass register,                          Address offset : 0x1c */     
  __IO  uint32_t  PLL_IFSR;           /*!< PLL input frequency select register,          Address offset : 0x20 */     
        uint32_t  RESERVED1[3];
  __IO  uint32_t  FCLK_SSR;           /*!< FCLK source select register,                  Address offset : 0x30 */     
  __IO  uint32_t  FCLK_PVSR;          /*!< FCLK prescale value select register,          Address offset : 0x34 */     
        uint32_t  RESERVED2[2];
  __IO  uint32_t  SSPCLK_SSR;         /*!< SSPCLK source select register,                Address offset : 0x40 */     
  __IO  uint32_t  SSPCLK_PVSR;        /*!< SSPCLK prescale value select register,        Address offset : 0x44 */     
        uint32_t  RESERVED3[6];
  __IO  uint32_t  ADCCLK_SSR;         /*!< ADCCLK source select register,                Address offset : 0x60 */     
  __IO  uint32_t  ADCCLK_PVSR;        /*!< ADCCLK prescale value select register,        Address offset : 0x64 */     
        uint32_t  RESERVED4[2];
  __IO  uint32_t  TIMER0CLK_SSR;      /*!< TIMER0CLK source select register,             Address offset : 0x70 */     
  __IO  uint32_t  TIMER0CLK_PVSR;     /*!< TIMER0CLK prescale value select register,     Address offset : 0x74 */     
        uint32_t  RESERVED5[2];
  __IO  uint32_t  TIMER1CLK_SSR;      /*!< TIMER1CLK source select register,             Address offset : 0x80 */     
  __IO  uint32_t  TIMER1CLK_PVSR;     /*!< TIMER1CLK prescale value select register,     Address offset : 0x84 */     
        uint32_t  RESERVED6[10];
  __IO  uint32_t  PWM0CLK_SSR;        /*!< PWM0CLK source select register,               Address offset : 0xb0 */     
  __IO  uint32_t  PWM0CLK_PVSR;       /*!< PWM0CLK prescale value select register,       Address offset : 0xb4 */     
        uint32_t  RESERVED7[2];
  __IO  uint32_t  PWM1CLK_SSR;        /*!< PWM1CLK source select register,               Address offset : 0xc0 */     
  __IO  uint32_t  PWM1CLK_PVSR;       /*!< PWM1CLK prescale value select register,       Address offset : 0xc4 */     
        uint32_t  RESERVED8[2];
  __IO  uint32_t  PWM2CLK_SSR;        /*!< PWM2CLK source select register,               Address offset : 0xd0 */     
  __IO  uint32_t  PWM2CLK_PVSR;       /*!< PWM2CLK prescale value select register,       Address offset : 0xd4 */     
        uint32_t  RESERVED9[2];
  __IO  uint32_t  PWM3CLK_SSR;        /*!< PWM3CLK source select register,               Address offset : 0xe0 */     
  __IO  uint32_t  PWM3CLK_PVSR;       /*!< PWM3CLK prescale value select register,       Address offset : 0xe4 */     
        uint32_t  RESERVED10[2];
  __IO  uint32_t  PWM4CLK_SSR;        /*!< PWM4CLK source select register,               Address offset : 0xf0 */     
  __IO  uint32_t  PWM4CLK_PVSR;       /*!< PWM4CLK prescale value select register,       Address offset : 0xf4 */     
        uint32_t  RESERVED11[2];
  __IO  uint32_t  PWM5CLK_SSR;        /*!< PWM5CLK source select register,               Address offset : 0x100 */     
  __IO  uint32_t  PWM5LK_PVSR;        /*!< PWM5CLK prescale value select register,       Address offset : 0x104 */     
        uint32_t  RESERVED12[2];
  __IO  uint32_t  PWM6CLK_SSR;        /*!< PWM6CLK source select register,               Address offset : 0x110 */     
  __IO  uint32_t  PWM6CLK_PVSR;       /*!< PWM6CLK prescale value select register,       Address offset : 0x114 */     
        uint32_t  RESERVED13[2];
  __IO  uint32_t  PWM7CLK_SSR;        /*!< PWM7CLK source select register,               Address offset : 0x120 */     
  __IO  uint32_t  PWM7CLK_PVSR;       /*!< PWM7CLK prescale value select register,       Address offset : 0x124 */     
        uint32_t  RESERVED14[2];
  __IO  uint32_t  RTC_HS_SSR;         /*!< RTC High Speed source select register,             Address offset : 0x130 */     
  __IO  uint32_t  RTC_HS_PVSR;        /*!< RTC High Speed prescale value select register,     Address offset : 0x134 */     
        uint32_t  RESERVED15;
  __IO  uint32_t  RTC_SSR;            /*!< RTC source select register,                        Address offset : 0x13c */

  __IO  uint32_t  WDOGCLK_HS_SSR;     /*!< WDOGCLK High Speed source select register,         Address offset : 0x140 */     
  __IO  uint32_t  WDOGCLK_HS_PVSR;    /*!< WDOGCLK High Speed prescale value select register, Address offset : 0x144 */     
        uint32_t  RESERVED16;
  __IO  uint32_t  WDOGCLK_SSR;        /*!< WDOGCLK source select register,               Address offset : 0x14c */

  __IO  uint32_t  UARTCLK_SSR;        /*!< UARTCLK source select register,               Address offset : 0x150 */     
  __IO  uint32_t  UARTCLK_PVSR;       /*!< UARTCLK prescale value select register,       Address offset : 0x154 */     
        uint32_t  RESERVED17[2];
  __IO  uint32_t  MIICLK_ECR;         /*!< MII clock enable control register,            Address offset : 0x160 */     
        uint32_t  RESERVED18[3];
  __IO  uint32_t  MONCLK_SSR;         /*!< Monitoring clock source select I found Treasure was IoT Base Station in March.register,      Address offset : 0x170 */
}CRG_TypeDef;


/**
  * @brief UART
  */
typedef struct
{
  __IO uint32_t DR;             /*!< Data,                              Address offset : 0x00 */
 union {
  __I  uint32_t RSR;            /*!< Receive Status,                    Address offset : 0x04 */  
  __O  uint32_t ECR;            /*!< Error Clear,                       Address offset : 0x04 */
  } STATUS;
       uint32_t RESERVED0[4];
  __IO uint32_t FR;             /*!< Flags,                             Address offset : 0x18 */
       uint32_t RESERVED1;
  __IO uint32_t ILPR;           /*!< IrDA Low-power Counter,            Address offset : 0x20 */                              
  __IO uint32_t IBRD;           /*!< Integer Baud Rate,                 Address offset : 0x24 */
  __IO uint32_t FBRD;           /*!< Fractional Baud Rate,              Address offset : 0x28 */
  __IO uint32_t LCR_H;          /*!< Line Control,                      Address offset : 0x2C */
  __IO uint32_t CR;             /*!< Control,                           Address offset : 0x30 */
  __IO uint32_t IFLS;           /*!< Interrupt FIFO Level Select,       Address offset : 0x34 */
  __IO uint32_t IMSC;           /*!< Interrupt Mask Set / Clear,        Address offset : 0x38 */
  __IO uint32_t RIS;            /*!< Raw Interrupt Status ,             Address offset : 0x3C */
  __IO uint32_t MIS;            /*!< Masked Interrupt Status ,          Address offset : 0x40 */
  __O  uint32_t ICR;            /*!< Interrupt Clear,                   Address offset : 0x44 */
  __IO uint32_t DMACR;          /*!< DMA Control,                       Address offset : 0x48 */
} UART_TypeDef;


/**
  * @brief Simple UART
  */
typedef struct
{
  __IO   uint32_t  DATA;          /*!< Offset: 0x000 Data Register    (R/W) */
  __IO   uint32_t  STATE;         /*!< Offset: 0x004 Status Register  (R/W) */
  __IO   uint32_t  CTRL;          /*!< Offset: 0x008 Control Register (R/W) */
  union {
    __I    uint32_t  STATUS;   /*!< Offset: 0x00C Interrupt Status Register (R/ ) */
    __O    uint32_t  CLEAR;    /*!< Offset: 0x00C Interrupt Clear Register ( /W) */
    }INT;
  __IO   uint32_t  BAUDDIV;       /*!< Offset: 0x010 Baudrate Divider Register (R/W) */

} S_UART_TypeDef;

/**
  * @brief Analog Digital Converter
  */

typedef struct
{
  __IO  uint32_t  ADC_CTR;          /* ADC control register,                    Address offset : 0x000 */
  __IO  uint32_t  ADC_CHSEL;        /* ADC channel select register,             Address offset : 0x004 */
  __IO  uint32_t  ADC_START;        /* ADC start register,                      Address offset : 0x008 */
  __I   uint32_t  ADC_DATA;         /* ADC conversion data register,            Address offset : 0x00c */
  __IO  uint32_t  ADC_INT;          /* ADC interrupt register,                  Address offset : 0x010 */
        uint32_t  RESERVED0[2];
  __IO  uint32_t  ADC_INTCLR;       /* ADC interrupt clear register,            Address offset : 0x01c */
}ADC_TypeDef;

/**
  * @brief dualtimer
  */
typedef struct
{
  __IO uint32_t TimerLoad;    // <h> Timer Load </h>
  __I  uint32_t TimerValue;   // <h> Timer Counter Current Value <r></h>
  __IO uint32_t TimerControl; // <h> Timer Control
                              //   <o.7> TimerEn: Timer Enable
                              //   <o.6> TimerMode: Timer Mode
                              //     <0=> Freerunning-mode
                              //     <1=> Periodic mode
                              //   <o.5> IntEnable: Interrupt Enable
                              //   <o.2..3> TimerPre: Timer Prescale
                              //     <0=> / 1
                              //     <1=> / 16
                              //     <2=> / 256
                              //     <3=> Undefined!
                              //   <o.1> TimerSize: Timer Size
                              //     <0=> 16-bit counter
                              //     <1=> 32-bit counter
                              //   <o.0> OneShot: One-shoot mode
                              //     <0=> Wrapping mode
                              //     <1=> One-shot mode
                              // </h>
  __O  uint32_t TimerIntClr;  // <h> Timer Interrupt Clear <w></h>
  __I  uint32_t TimerRIS;     // <h> Timer Raw Interrupt Status <r></h>
  __I  uint32_t TimerMIS;     // <h> Timer Masked Interrupt Status <r></h>
  __IO uint32_t TimerBGLoad;  // <h> Background Load Register </h>
} DUALTIMER_TypeDef;

/**
  * @brief GPIO
  */
typedef struct
{
  __IO   uint32_t  DATA;            /* DATA Register (R/W),                        offset : 0x000 */
  __IO   uint32_t  DATAOUT;         /* Data Output Latch Register (R/W),           offset : 0x004 */
         uint32_t  RESERVED0[2];
  __IO   uint32_t  OUTENSET;        /* Output Enable Set Register  (R/W)           offset : 0x010 */
  __IO   uint32_t  OUTENCLR;        /* Output Enable Clear Register  (R/W)         offset : 0x014 */
  __IO   uint32_t  RESERVED1;       /* Alternate Function Set Register  (R/W)      offset : 0x018 */
  __IO   uint32_t  RESERVED2;       /* Alternate Function Clear Register  (R/W)    offset : 0x01C */
  __IO   uint32_t  INTENSET;        /* Interrupt Enable Set Register  (R/W)        offset : 0x020 */
  __IO   uint32_t  INTENCLR;        /* Interrupt Enable Clear Register  (R/W)      offset : 0x024 */
  __IO   uint32_t  INTTYPESET;      /* Interrupt Type Set Register  (R/W)          offset : 0x028 */
  __IO   uint32_t  INTTYPECLR;      /* Interrupt Type Clear Register  (R/W)        offset : 0x02C */
  __IO   uint32_t  INTPOLSET;       /* Interrupt Polarity Set Register  (R/W)      offset : 0x030 */
  __IO   uint32_t  INTPOLCLR;       /* Interrupt Polarity Clear Register  (R/W)    offset : 0x034 */
  union {
    __I    uint32_t  INTSTATUS;     /* Interrupt Status Register (R/ )             offset : 0x038 */
    __O    uint32_t  INTCLEAR;      /* Interrupt Clear Register ( /W)              offset : 0x038 */
    }Interrupt;
         uint32_t RESERVED3[241];
  __IO   uint32_t LB_MASKED[256];   /* Lower byte Masked Access Register (R/W)     offset : 0x400 - 0x7FC  */
  __IO   uint32_t UB_MASKED[256];   /* Upper byte Masked Access Register (R/W)     offset : 0x800 - 0xBFC  */
} GPIO_TypeDef;

typedef struct
{
  __IO  uint32_t  Port[16]; /* Port_00,      offset : 0x00 */
                            /* Port_01,      offset : 0x04 */
                            /* Port_02,      offset : 0x08 */
                            /* Port_03,      offset : 0x0C */
                            /* Port_04,      offset : 0x10 */
                            /* Port_05,      offset : 0x14 */
                            /* Port_06,      offset : 0x18 */
                            /* Port_07,      offset : 0x1C */
                            /* Port_08,      offset : 0x20 */
                            /* Port_09,      offset : 0x24 */
                            /* Port_10,      offset : 0x28 */
                            /* Port_11,      offset : 0x2C */
                            /* Port_12,      offset : 0x30 */
                            /* Port_13,      offset : 0x34 */
                            /* Port_14,      offset : 0x38 */
                            /* Port_15,      offset : 0x3C */
} P_Port_Def;

typedef struct
{
  __IO  uint32_t  Port[5];  /* Port_00,      offset : 0x00 */
                            /* Port_01,      offset : 0x04 */
                            /* Port_02,      offset : 0x08 */
                            /* Port_03,      offset : 0x0C */
                            /* Port_04,      offset : 0x10 */
} P_Port_D_Def;

/**
  * @brief  I2C Register structure definition
  */
typedef struct
{
    __IO uint32_t PRER;              //0x00
    __IO uint32_t CTR;              //0x04
    __IO uint32_t CMDR;              //0x08
    __I  uint32_t SR;               //0x0C
    __IO uint32_t TSR;               //0x10
    __IO uint32_t SADDR;            //0x14
    __IO uint32_t TXR;               //0x18
    __I  uint32_t RXR;               //0x1C
    __I  uint32_t ISR;              //0x20
    __IO uint32_t ISCR;             //0x24
    __IO uint32_t ISMR;             //0x28
}I2C_TypeDef;

/**
 * @brief PWM Register structure definition
 */
typedef struct
{
    __IO    uint32_t    IER;    //Interrupt enable register
                                //   <7>   IE7 : Channel 7 interrupt enable <R/W>
                                //   <6>   IE6 : Channel 6 interrupt enable <R/W>
                                //   <5>   IE5 : Channel 5 interrupt enable <R/W>
                                //   <4>   IE4 : Channel 4 interrupt enable <R/W>
                                //   <3>   IE3 : Channel 3 interrupt enable <R/W>
                                //   <2>   IE2 : Channel 2 interrupt enable <R/W>
                                //   <1>   IE1 : Channel 1 interrupt enable <R/W>
                                //   <0>   IE0 : Channel 0 interrupt enable <R/W>

    __IO    uint32_t    SSR;    //Start Stop register
                                //   <7>   SS7 : Channel 7 TC start or stop <R/W>
                                //   <6>   SS6 : Channel 6 TC start or stop <R/W>
                                //   <5>   SS5 : Channel 5 TC start or stop <R/W>
                                //   <4>   SS4 : Channel 4 TC start or stop <R/W>
                                //   <3>   SS3 : Channel 3 TC start or stop <R/W>
                                //   <2>   SS2 : Channel 2 TC start or stop <R/W>
                                //   <1>   SS1 : Channel 1 TC start or stop <R/W>
                                //   <0>   SS0 : Channel 0 TC start or stop <R/W>

    __IO    uint32_t    PSR;    //Pause register
                                //   <7>   PS7 : Channel 7 TC pasue <R/W>
                                //   <6>   PS6 : Channel 6 TC pasue <R/W>
                                //   <5>   PS5 : Channel 5 TC pasue <R/W>
                                //   <4>   PS4 : Channel 4 TC pasue <R/W>
                                //   <3>   PS3 : Channel 3 TC pasue <R/W>
                                //   <2>   PS2 : Channel 2 TC pasue <R/W>
                                //   <1>   PS1 : Channel 1 TC pasue <R/W>
                                //   <0>   PS0 : Channel 0 TC pasue <R/W>
} PWM_TypeDef;

typedef struct
{
    __I     uint32_t    IR;     //Interrupt register
                                //   <2>   CI : Capture interrupt <R>
                                //   <1>   OI : Overflow interrupt <R>
                                //   <0>   MI : Match interrupt <R>

    __IO    uint32_t    IER;    //Interrupt enable register
                                //   <2>   CIE : Capture interrupt enable <R/W>
                                //   <1>   OIE : Overflow interrupt enable <R/W>
                                //   <0>   MIE : Match interrupt enable <R/W>

    __O     uint32_t    ICR;    //Interrupt clear register
                                //   <2>   CIC : Capture interrupt clear <W>
                                //   <1>   OIC : Overflow interrupt clear <W>
                                //   <0>   MIC : Match interrupt clear <W>

    __I     uint32_t    TCR;    //Timer/Counter register
                                //   <0..31>   TCR : Timer/Counter register <R>

    __I     uint32_t    PCR;    //Prescale counter register
                                //   <0..5>   PCR : Prescale Counter register <R>

    __IO    uint32_t    PR;     //Prescale register
                                //   <0..5>   PR : prescale register <R/W>

    __IO    uint32_t    MR;     //Match register
                                //   <0..31>   MR : Match register <R/W>

    __IO    uint32_t    LR;     //Limit register
                                //   <0..31>   LR : Limit register <R/W>
    __IO    uint32_t    UDMR;   //Up-Down mode register
                                //   <0>   UDM : Up-down mode <R/W>

    __IO    uint32_t    TCMR;   //Timer/Counter mode register
                                //   <0>   TCM : Timer/Counter mode <R/W>

    __IO    uint32_t    PEEER;  //PWM output enable and external input enable register
                                //   <0..1>   PEEE : PWM output enable and external input enable <R/W>

    __IO    uint32_t    CMR;    //Capture mode register
                                //   <0>   CM : Capture mode <R/W>

    __IO    uint32_t    CR;     //Capture register
                                //   <0..31>   CR : Capture register <R>

    __IO    uint32_t    PDMR;   //Periodic mode register
                                //   <0>   PDM : Periodic mode <R/W>

    __IO    uint32_t    DZER;   //Dead-zone enable register
                                //   <0>   DZE : Dead-zone enable <R/W>

    __IO    uint32_t    DZCR;   //Dead-zone counter register
                                //   <0..9>   DZC : Dead-zone counter <R/W>
} PWM_CHn_TypeDef;

typedef struct
{
    __IO uint32_t    PWM_CHn_PR;   //Prescale register
                                   //   <0..5>   PR : prescale register <R/W>
    __IO uint32_t    PWM_CHn_MR;   //Match register
                                   //   <0..31>   MR : Match register <R/W>
    __IO uint32_t    PWM_CHn_LR;   //Limit register
                                   //   <0..31>   LR : Limit register <R/W>
    __IO uint32_t    PWM_CHn_UDMR; //Up-Down mode register
                                   //   <0>   UDM : Up-down mode <R/W>
    __IO uint32_t    PWM_CHn_PDMR; //Periodic mode register
                                   //   <0>   PDM : Periodic mode <R/W>
}PWM_TimerModeInitTypeDef;

typedef struct
{
    __IO uint32_t    PWM_CHn_PR;   //Prescale register
                                   //   <0..5>   PR : prescale register <R/W>
    __IO uint32_t    PWM_CHn_MR;   //Match register
                                   //   <0..31>   MR : Match register <R/W>
    __IO uint32_t    PWM_CHn_LR;   //Limit register
                                   //   <0..31>   LR : Limit register <R/W>
    __IO uint32_t    PWM_CHn_UDMR; //Up-Down mode register
                                   //   <0>   UDM : Up-down mode <R/W>
    __IO uint32_t    PWM_CHn_PDMR; //Periodic mode register
                                   //   <0>   PDM : Peiodic mode <R/W>
    __IO uint32_t    PWM_CHn_CMR;  //Capture mode register
                                   //   <0>   CM : Capture mode <R/W>
}PWM_CaptureModeInitTypeDef;

typedef struct
{
    __IO uint32_t    PWM_CHn_MR;
    __IO uint32_t    PWM_CHn_LR;
    __IO uint32_t    PWM_CHn_UDMR;
    __IO uint32_t    PWM_CHn_PDMR;
    __IO uint32_t    PWM_CHn_TCMR;
}PWM_CounterModeInitTypeDef;


/**
  * @brief Random Number generator
  */
typedef struct
{
  __IO  uint32_t  RNG_RUN;          /* RNG run register,                    Address offset : 0x000 */
  __IO  uint32_t  RNG_SEED;         /* RNG seed value register,             Address offset : 0x004 */
  __IO  uint32_t  RNG_CLKSEL;       /* RNG Clock source select register,    Address offset : 0x008 */
  __IO  uint32_t  RNG_MODE;         /* RNG MODE select register,            Address offset : 0x00c */
  __I   uint32_t  RNG_RN;           /* RNG random number value register,    Address offset : 0x010 */
  __IO  uint32_t  RNG_POLY;         /* RNG polynomial register,             Address offset : 0x014 */
}RNG_TypeDef;

/**
  * @brief Serial Peripheral Interface
  */
typedef struct
{
  __IO uint32_t CR0;
  __IO uint32_t CR1;
  __IO uint32_t DR;
  __IO uint32_t SR;
  __IO uint32_t CPSR;
  __IO uint32_t IMSC;
  __IO uint32_t RIS;
  __IO uint32_t MIS;
  __IO uint32_t ICR;
  __IO uint32_t DMACR;
} SSP_TypeDef;

typedef struct
{
    __IO    uint32_t  WatchdogLoad;         // <h> Watchdog Load Register </h>
    __I     uint32_t  WatchdogValue;        // <h> Watchdog Value Register </h>
    __IO    uint32_t  WatchdogControl;      // <h> Watchdog Control Register
                                            //   <o.1>    RESEN: Reset enable
                                            //   <o.0>    INTEN: Interrupt enable
                                            // </h>
    __O     uint32_t  WatchdogIntClr;       // <h> Watchdog Clear Interrupt Register </h>
    __I     uint32_t  WatchdogRIS;          // <h> Watchdog Raw Interrupt Status Register </h>
    __I     uint32_t  WatchdogMIS;          // <h> Watchdog Interrupt Status Register </h>
            uint32_t  RESERVED[762];
    __IO    uint32_t  WatchdogLock;         // <h> Watchdog Lock Register </h>
}WATCHDOG_TypeDef;

/** @addtogroup Peripheral_memory_map
  * @{
  */

/* Peripheral and SRAM base address */
#define W7500x_FLASH_BASE        (0x00000000UL)  /*!< (FLASH     ) Base Address */
#define W7500x_SRAM_BASE         (0x20000000UL)  /*!< (SRAM      ) Base Address */
#define W7500x_PERIPH_BASE       (0x40000000UL)  /*!< (Peripheral) Base Address */

#define W7500x_RAM_BASE          (0x20000000UL)
#define W7500x_APB1_BASE         (0x40000000UL)
#define W7500x_APB2_BASE         (0x41000000UL)
#define W7500x_AHB_BASE          (0x42000000UL)

#define W7500x_UART0_BASE        (W7500x_APB1_BASE + 0x0000C000UL)
#define W7500x_UART1_BASE        (W7500x_APB1_BASE + 0x0000D000UL)
#define W7500x_UART2_BASE        (W7500x_APB1_BASE + 0x00006000UL)

#define W7500x_CRG_BASE          (W7500x_APB2_BASE + 0x00001000UL)
#define W7500x_ADC_BASE          (W7500x_APB2_BASE + 0x00000000UL)

#define W7500x_INFO_BGT          (0x0003FDB8)
#define W7500x_INFO_OSC          (0x0003FDBC)

#define W7500x_TRIM_BGT          (0x41001210)
#define W7500x_TRIM_OSC          (0x41001004)

#define W7500x_DUALTIMER0_BASE   (W7500x_APB1_BASE + 0x00001000ul)
#define W7500x_DUALTIMER1_BASE   (W7500x_APB1_BASE + 0x00002000ul)

#define EXTI_Px_BASE            (W7500x_APB2_BASE + 0x00002200UL)

#define GPIOA_BASE              (W7500x_AHB_BASE + 0x00000000UL)    // W7500x_AHB_BASE : 0x42000000UL
#define GPIOB_BASE              (W7500x_AHB_BASE + 0x01000000UL)
#define GPIOC_BASE              (W7500x_AHB_BASE + 0x02000000UL)
#define GPIOD_BASE              (W7500x_AHB_BASE + 0x03000000UL)

#define P_AFSR_BASE             (W7500x_APB2_BASE + 0x00002000UL)

#define P_PCR_BASE              (W7500x_APB2_BASE + 0x00003000UL)

#define I2C0_BASE               (W7500x_APB1_BASE + 0x8000)
#define I2C1_BASE               (W7500x_APB1_BASE + 0x9000)

#define W7500x_PWM_BASE         (W7500x_APB1_BASE + 0x00005000UL)

#define W7500x_RNG_BASE         (W7500x_APB1_BASE + 0x00007000UL)

#define SSP0_BASE 	          (0x4000A000)
#define SSP1_BASE 	          (0x4000B000)

#define W7500x_WATCHDOG_BASE    (W7500x_APB1_BASE + 0x0000UL)

/**
  * @}
  */


/** @addtogroup Peripheral_declaration
  * @{
  */  
#define CRG               ((CRG_TypeDef *) W7500x_CRG_BASE)

#define UART0             ((UART_TypeDef  *)   W7500x_UART0_BASE)
#define UART1             ((UART_TypeDef  *)   W7500x_UART1_BASE)
#define UART2             ((S_UART_TypeDef *)  W7500x_UART2_BASE)

#define ADC               ((ADC_TypeDef *) W7500x_ADC_BASE)

#define DUALTIMER0_0      ((DUALTIMER_TypeDef  *) (W7500x_DUALTIMER0_BASE) )
#define DUALTIMER0_1      ((DUALTIMER_TypeDef  *) (W7500x_DUALTIMER0_BASE + 0x20ul))
#define DUALTIMER1_0      ((DUALTIMER_TypeDef  *) (W7500x_DUALTIMER1_BASE) )
#define DUALTIMER1_1      ((DUALTIMER_TypeDef  *) (W7500x_DUALTIMER1_BASE + 0x20ul))

#define EXTI_PA         ((P_Port_Def *)   (EXTI_Px_BASE + 0x00000000UL))  /* PA_XX External interrupt Enable Register */
#define EXTI_PB         ((P_Port_Def *)   (EXTI_Px_BASE + 0x00000040UL))  /* PB_XX External interrupt Enable Register */
#define EXTI_PC         ((P_Port_Def *)   (EXTI_Px_BASE + 0x00000080UL))  /* PC_XX External interrupt Enable Register */
#define EXTI_PD         ((P_Port_D_Def *) (EXTI_Px_BASE + 0x000000C0UL))  /* PD_XX External interrupt Enable Register */

#define GPIOA   ((GPIO_TypeDef *) (GPIOA_BASE) )
#define GPIOB   ((GPIO_TypeDef *) (GPIOB_BASE) )
#define GPIOC   ((GPIO_TypeDef *) (GPIOC_BASE) )
#define GPIOD   ((GPIO_TypeDef *) (GPIOD_BASE) )

#define PA_AFSR ((P_Port_Def *)   (P_AFSR_BASE + 0x00000000UL))  /* PA_XX Pad Alternate Function Select Register */
#define PB_AFSR ((P_Port_Def *)   (P_AFSR_BASE + 0x00000040UL))  /* PB_XX Pad Alternate Function Select Register */
#define PC_AFSR ((P_Port_Def *)   (P_AFSR_BASE + 0x00000080UL))  /* PC_XX Pad Alternate Function Select Register */
#define PD_AFSR ((P_Port_D_Def *) (P_AFSR_BASE + 0x000000C0UL))  /* PD_XX Pad Alternate Function Select Register */

#define PA_PCR  ((P_Port_Def *)   (P_PCR_BASE + 0x00000000UL))   /* PA_XX Pad Control Register */
#define PB_PCR  ((P_Port_Def *)   (P_PCR_BASE + 0x00000040UL))   /* PB_XX Pad Control Register */
#define PC_PCR  ((P_Port_Def *)   (P_PCR_BASE + 0x00000080UL))   /* PC_XX Pad Control Register */
#define PD_PCR  ((P_Port_D_Def *) (P_PCR_BASE + 0x000000C0UL))   /* PD_XX Pad Control Register */

#define I2C0    ((I2C_TypeDef      *)  I2C0_BASE)
#define I2C1    ((I2C_TypeDef      *)  I2C1_BASE)


#define TIMCLKEN0_0                         *(uint32_t *)(W7500x_DUALTIMER0_BASE + 0x80ul)
#define TIMCLKEN0_1                         *(uint32_t *)(W7500x_DUALTIMER0_BASE + 0xA0ul)
#define TIMCLKEN1_0                         *(uint32_t *)(W7500x_DUALTIMER1_BASE + 0x80ul)
#define TIMCLKEN1_1                         *(uint32_t *)(W7500x_DUALTIMER1_BASE + 0xA0ul)

#define PWM                             ((PWM_TypeDef *)         (W7500x_PWM_BASE + 0x800UL ))
#define PWM_CH0                         ((PWM_CHn_TypeDef *)     (W7500x_PWM_BASE))
#define PWM_CH1                         ((PWM_CHn_TypeDef *)     (W7500x_PWM_BASE + 0x100UL))
#define PWM_CH2                         ((PWM_CHn_TypeDef *)     (W7500x_PWM_BASE + 0x200UL))
#define PWM_CH3                         ((PWM_CHn_TypeDef *)     (W7500x_PWM_BASE + 0x300UL))
#define PWM_CH4                         ((PWM_CHn_TypeDef *)     (W7500x_PWM_BASE + 0x400UL))
#define PWM_CH5                         ((PWM_CHn_TypeDef *)     (W7500x_PWM_BASE + 0x500UL))
#define PWM_CH6                         ((PWM_CHn_TypeDef *)     (W7500x_PWM_BASE + 0x600UL))
#define PWM_CH7                         ((PWM_CHn_TypeDef *)     (W7500x_PWM_BASE + 0x700UL))

#define RNG                               ((RNG_TypeDef *) W7500x_RNG_BASE)

#define SSP0    ((SSP_TypeDef*)   (SSP0_BASE))
#define SSP1    ((SSP_TypeDef*)   (SSP1_BASE))

#define WATCHDOG                ((WATCHDOG_TypeDef  *) W7500x_WATCHDOG_BASE)

/**
  * @}
  */



/******************************************************************************/
/*                                                                            */
/*                  Clock Reset Generator                                     */
/*                                                                            */
/******************************************************************************/
/****************   Bit definition for CRG_OSC_PDR   **************************/
#define CRG_OSC_PDR_NRMLOP      (0x0ul)             // Normal Operation
#define CRG_OSC_PDR_PD          (0x1ul)             // Power Down
/****************   Bit definition for CRG_PLL_PDR   **************************/
#define CRG_PLL_PDR_PD          (0x0ul)             // Power Down
#define CRG_PLL_PDR_NRMLOP      (0x1ul)             // Normal Operation
/****************   Bit definition for CRG_PLL_FCR   **************************/
//ToDo
/****************   Bit definition for CRG_PLL_OER   **************************/
#define CRG_PLL_OER_DIS         (0x0ul)             // Clock out is disable
#define CRG_PLL_OER_EN          (0x1ul)             // Clock out is enable
/****************   Bit definition for CRG_PLL_BPR   **************************/
#define CRG_PLL_BPR_DIS         (0x0ul)             // Bypass disable. Normal operation
#define CRG_PLL_BPR_EN          (0x1ul)             // Bypass enable. Clock will be set to external clock
/****************   Bit definition for CRG_PLL_IFSR  **************************/
#define CRG_PLL_IFSR_RCLK       (0x0ul)             // Internal 8MHz RC oscillator clock(RCLK)
#define CRG_PLL_IFSR_OCLK       (0x1ul)             // External oscillator clock (OCLK, 8MHz ~ 24MHz)
/****************   Bit definition for CRG_FCLK_SSR  **************************/
#define CRG_FCLK_SSR_MCLK       (0x01ul)            // 00,01 Output clock of PLL(MCLK)
#define CRG_FCLK_SSR_RCLK       (0x02ul)            // Internal 8MHz RC oscillator clock(RCLK)
#define CRG_FCLK_SSR_OCLK       (0x03ul)            // External oscillator clock(OCLK, 8MHz ~ 24MHz)
/****************   Bit definition for CRG_FCLK_PVSR   **************************/
#define CRG_FCLK_PVSR_DIV1      (0x00ul)            // 1/1 (bypass)
#define CRG_FCLK_PVSR_DIV2      (0x01ul)            // 1/2
#define CRG_FCLK_PVSR_DIV4      (0x02ul)            // 1/4
#define CRG_FCLK_PVSR_DIV8      (0x03ul)            // 1/8
/****************   Bit definition for CRG_SSPCLK_SSR   **************************/
#define CRG_SSPCLK_SSR_DIS      (0x00ul)            // Disable clock
#define CRG_SSPCLK_SSR_MCLK     (0x01ul)            // PLL output clock(MCLK)
#define CRG_SSPCLK_SSR_RCLK     (0x02ul)            // Internal 8MHz RC oscillator clock(RCLK)
#define CRG_SSPCLK_SSR_OCLK     (0x03ul)            // External oscillator clock(OCLK, 8MHz ~ 24MHz)
/****************   Bit definition for CRG_SSPCLK_PVSR   **************************/
#define CRG_SSPCLK_PVSR_DIV1    (0x00ul)            // 1/1 (bypass)
#define CRG_SSPCLK_PVSR_DIV2    (0x01ul)            // 1/2
#define CRG_SSPCLK_PVSR_DIV4    (0x02ul)            // 1/4
#define CRG_SSPCLK_PVSR_DIV8    (0x03ul)            // 1/8
/****************   Bit definition for CRG_ADCCLK_SSR   **************************/
#define CRG_ADCCLK_SSR_DIS      (0x00ul)            // Disable clock
#define CRG_ADCCLK_SSR_MCLK     (0x01ul)            // PLL output clock(MCLK)
#define CRG_ADCCLK_SSR_RCLK     (0x02ul)            // Internal 8MHz RC oscillator clock(RCLK)
#define CRG_ADCCLK_SSR_OCLK     (0x03ul)            // External oscillator clock(OCLK, 8MHz ~ 24MHz)
/****************   Bit definition for CRG_ADCCLK_PVSR   **************************/
#define CRG_ADCCLK_PVSR_DIV1    (0x00ul)            // 1/1 (bypass)
#define CRG_ADCCLK_PVSR_DIV2    (0x01ul)            // 1/2
#define CRG_ADCCLK_PVSR_DIV4    (0x02ul)            // 1/4
#define CRG_ADCCLK_PVSR_DIV8    (0x03ul)            // 1/8
/****************   Bit definition for CRG_TIMER0/1CLK_SSR   **************************/
#define CRG_TIMERCLK_SSR_DIS      (0x00ul)            // Disable clock
#define CRG_TIMERCLK_SSR_MCLK     (0x01ul)            // PLL output clock(MCLK)
#define CRG_TIMERCLK_SSR_RCLK     (0x02ul)            // Internal 8MHz RC oscillator clock(RCLK)
#define CRG_TIMERCLK_SSR_OCLK     (0x03ul)            // External oscillator clock(OCLK, 8MHz ~ 24MHz)
/****************   Bit definition for CRG_TIMER0/1CLK_PVSR   **************************/
#define CRG_TIMERCLK_PVSR_DIV1    (0x00ul)           // 1/1 (bypass)
#define CRG_TIMERCLK_PVSR_DIV2    (0x01ul)           // 1/2
#define CRG_TIMERCLK_PVSR_DIV4    (0x02ul)           // 1/4
#define CRG_TIMERCLK_PVSR_DIV8    (0x03ul)           // 1/8
#define CRG_TIMERCLK_PVSR_DIV16   (0x04ul)           // 1/16
#define CRG_TIMERCLK_PVSR_DIV32   (0x05ul)           // 1/32
#define CRG_TIMERCLK_PVSR_DIV64   (0x06ul)           // 1/64
#define CRG_TIMERCLK_PVSR_DIV128  (0x07ul)           // 1/128
/****************   Bit definition for CRG_PWMnCLK_SSR   **************************/
#define CRG_PWMCLK_SSR_DIS      (0x00ul)            // Disable clock
#define CRG_PWMCLK_SSR_MCLK     (0x01ul)            // PLL output clock(MCLK)
#define CRG_PWMCLK_SSR_RCLK     (0x02ul)            // Internal 8MHz RC oscillator clock(RCLK)
#define CRG_PWMCLK_SSR_OCLK     (0x03ul)            // External oscillator clock(OCLK, 8MHz ~ 24MHz)
/****************   Bit definition for CRG_PWMnCLK_PVSR   **************************/
#define CRG_PWMCLK_PVSR_DIV1    (0x00ul)           // 1/1 (bypass)
#define CRG_PWMCLK_PVSR_DIV2    (0x01ul)           // 1/2
#define CRG_PWMCLK_PVSR_DIV4    (0x02ul)           // 1/4
#define CRG_PWMCLK_PVSR_DIV8    (0x03ul)           // 1/8
#define CRG_PWMCLK_PVSR_DIV16   (0x04ul)           // 1/16
#define CRG_PWMCLK_PVSR_DIV32   (0x05ul)           // 1/32
#define CRG_PWMCLK_PVSR_DIV64   (0x06ul)           // 1/64
#define CRG_PWMCLK_PVSR_DIV128  (0x07ul)           // 1/128
/****************   Bit definition for CRG_RTC_HS_SSR   **************************/
#define CRG_RTC_HS_SSR_DIS      (0x00ul)            // Disable clock
#define CRG_RTC_HS_SSR_MCLK     (0x01ul)            // PLL output clock(MCLK)
#define CRG_RTC_HS_SSR_RCLK     (0x02ul)            // Internal 8MHz RC oscillator clock(RCLK)
#define CRG_RTC_HS_SSR_OCLK     (0x03ul)            // External oscillator clock(OCLK, 8MHz ~ 24MHz)
/****************   Bit definition for CRG_RTC_HS_PVSR   **************************/
#define CRG_RTC_HS_PVSR_DIV1    (0x00ul)           // 1/1 (bypass)
#define CRG_RTC_HS_PVSR_DIV2    (0x01ul)           // 1/2
#define CRG_RTC_HS_PVSR_DIV4    (0x02ul)           // 1/4
#define CRG_RTC_HS_PVSR_DIV8    (0x03ul)           // 1/8
#define CRG_RTC_HS_PVSR_DIV16   (0x04ul)           // 1/16
#define CRG_RTC_HS_PVSR_DIV32   (0x05ul)           // 1/32
#define CRG_RTC_HS_PVSR_DIV64   (0x06ul)           // 1/64
#define CRG_RTC_HS_PVSR_DIV128  (0x07ul)           // 1/128
/****************   Bit definition for CRG_RTC_SSR   **************************/
#define CRG_RTC_SSR_HS          (0x00ul)            // RTCCLK HS(High Speed clock) 
#define CRG_RTC_SSR_LW          (0x01ul)            // 32K_OSC_CLK(Low Speed external oscillator clock) 
/****************   Bit definition for CRG_WDOGCLK_HS_SSR   **************************/
#define CRG_WDOGCLK_HS_SSR_DIS      (0x00ul)        // Disable clock
#define CRG_WDOGCLK_HS_SSR_MCLK     (0x01ul)        // PLL output clock(MCLK)
#define CRG_WDOGCLK_HS_SSR_RCLK     (0x02ul)        // Internal 8MHz RC oscillator clock(RCLK)
#define CRG_WDOGCLK_HS_SSR_OCLK     (0x03ul)        // External oscillator clock(OCLK, 8MHz ~ 24MHz)
/****************   Bit definition for CRG_WDOGCLK_HS_PVSR   **************************/
#define CRG_WDOGCLK_HS_PVSR_DIV1    (0x00ul)       // 1/1 (bypass)
#define CRG_WDOGCLK_HS_PVSR_DIV2    (0x01ul)       // 1/2
#define CRG_WDOGCLK_HS_PVSR_DIV4    (0x02ul)       // 1/4
#define CRG_WDOGCLK_HS_PVSR_DIV8    (0x03ul)       // 1/8
#define CRG_WDOGCLK_HS_PVSR_DIV16   (0x04ul)       // 1/16
#define CRG_WDOGCLK_HS_PVSR_DIV32   (0x05ul)       // 1/32
#define CRG_WDOGCLK_HS_PVSR_DIV64   (0x06ul)       // 1/64
#define CRG_WDOGCLK_HS_PVSR_DIV128  (0x07ul)       // 1/128
/****************   Bit definition for CRG_WDOGCLK_SSR   **************************/
#define CRG_WDOGCLK_SSR_HS          (0x00ul)        // RTCCLK HS(High Speed clock) 
#define CRG_WDOGCLK_SSR_LW          (0x01ul)        // 32K_OSC_CLK(Low Speed external oscillator clock) 
/****************   Bit definition for CRG_UARTCLK_SSR   **************************/
#define CRG_UARTCLK_SSR_DIS      (0x00ul)           // Disable clock
#define CRG_UARTCLK_SSR_MCLK     (0x01ul)           // PLL output clock(MCLK)
#define CRG_UARTCLK_SSR_RCLK     (0x02ul)           // Internal 8MHz RC oscillator clock(RCLK)
#define CRG_UARTCLK_SSR_OCLK     (0x03ul)           // External oscillator clock(OCLK, 8MHz ~ 24MHz)
/****************   Bit definition for CRG_UARTCLK_PVSR   **************************/
#define CRG_UARTCLK_PVSR_DIV1    (0x00ul)           // 1/1 (bypass)
#define CRG_UARTCLK_PVSR_DIV2    (0x01ul)           // 1/2
#define CRG_UARTCLK_PVSR_DIV4    (0x02ul)           // 1/4
#define CRG_UARTCLK_PVSR_DIV8    (0x03ul)           // 1/8
/****************   Bit definition for CRG_MIICLK_ECR   **************************/
#define CRG_MIICLK_ECR_EN_RXCLK  (0x01ul << 0)      // Enable MII_RCK and MII_RCK_N
#define CRG_MIICLK_ECR_EN_TXCLK  (0x01ul << 1)      // Enable MII_TCK and MII_TCK_N
/****************   Bit definition for CRG_MONCLK_SSR   **************************/
#define CRG_MONCLK_SSR_MCLK      (0x00ul)        // PLL output clock (MCLK)
#define CRG_MONCLK_SSR_FCLK      (0x01ul)        // FCLK
#define CRG_MONCLK_SSR_RCLK      (0x02ul)        // Internal 8MHz RC oscillator clock(RCLK)
#define CRG_MONCLK_SSR_OCLK      (0x03ul)        // External oscillator clock(OCLK, 8MHz ~ 24MHz)
#define CRG_MONCLK_SSR_ADCCLK    (0x04ul)        // ADCCLK
#define CRG_MONCLK_SSR_SSPCLK    (0x05ul)        // SSPCLK
#define CRG_MONCLK_SSR_TIMCLK0   (0x06ul)        // TIMCLK0
#define CRG_MONCLK_SSR_TIMCLK1   (0x07ul)        // TIMCLK1
#define CRG_MONCLK_SSR_PWMCLK0   (0x08ul)        // PWMCLK0
#define CRG_MONCLK_SSR_PWMCLK1   (0x09ul)        // PWMCLK1
#define CRG_MONCLK_SSR_PWMCLK2   (0x0Aul)        // PWMCLK2
#define CRG_MONCLK_SSR_PWMCLK3   (0x0Bul)        // PWMCLK3
#define CRG_MONCLK_SSR_PWMCLK4   (0x0Cul)        // PWMCLK4
#define CRG_MONCLK_SSR_PWMCLK5   (0x0Dul)        // PWMCLK5
#define CRG_MONCLK_SSR_PWMCLK6   (0x0Eul)        // PWMCLK6
#define CRG_MONCLK_SSR_PWMCLK7   (0x0Ful)        // PWMCLK7
#define CRG_MONCLK_SSR_UARTCLK   (0x10ul)        // UARTCLK
#define CRG_MONCLK_SSR_MII_RXCLK (0x11ul)        // MII_RXCLK 
#define CRG_MONCLK_SSR_MII_TXCLK (0x12ul)        // MII_TXCLK
#define CRG_MONCLK_SSR_RTCCLK    (0x13ul)        // RTCCLK

/******************************************************************************/
/*                                                                            */
/*                                  UART                                      */
/*                                                                            */
/******************************************************************************/
/****************** Bit definition for UART Data(UARTDR) register *************************/
#define UART_DR_OE           (0x01ul << 11)      // Overrun Error
#define UART_DR_BE           (0x01ul << 10)      // Break Error
#define UART_DR_PE           (0x01ul <<  9)      // Parity Error
#define UART_DR_FE           (0x01ul <<  8)      // Framing Error
//#define UART_DR_DR                               // ToDo
/***************** Bit definition for UART Receive Status(UARTRSR) register ***************/
#define UARTR_SR_OE           (0x01ul << 3)      // Overrun Error
#define UARTR_SR_BE           (0x01ul << 2)      // Break Error
#define UARTR_SR_PE           (0x01ul << 1)      // Parity Error
#define UARTR_SR_FE           (0x01ul << 0)      // Framing Error
/***************** Bit definition for UART Error Clear(UARTECR) register ******************/
#define UARTE_CR_OE           (0x01ul << 3)      // Overrun Error
#define UARTE_CR_BE           (0x01ul << 2)      // Break Error
#define UARTE_CR_PE           (0x01ul << 1)      // Parity Error
#define UARTE_CR_FE           (0x01ul << 0)      // Framing Error
/****************** Bit definition for UART Flags(UARTFR) register ************************/
#define UART_FR_RI            (0x01ul << 8)      // Ring indicator
#define UART_FR_TXFE          (0x01ul << 7)      // Transmit FIFO empty
#define UART_FR_RXFF          (0x01ul << 6)      // Receive FIFO full
#define UART_FR_TXFF          (0x01ul << 5)      // Transmit FIFO full
#define UART_FR_RXFE          (0x01ul << 4)      // Receive FIFO empty
#define UART_FR_BUSY          (0x01ul << 3)      // UART busy
#define UART_FR_DCD           (0x01ul << 2)      // Data carrier detect
#define UART_FR_DSR           (0x01ul << 1)      // Data set ready
#define UART_FR_CTS           (0x01ul << 0)      // Clear to send
/********** Bit definition for UART Low-power Counter(UARTILPR) register *******************/
#define UARTILPR_COUNTER     (0xFFul << 0)      // 8-bit low-power divisor value (0..255)
/********************* Bit definition for Line Control(UARTLCR_H) register *****************/
#define UART_LCR_H_SPS        (0x1ul << 7)       // Stick parity select
#define UART_LCR_H_WLEN(n)    ((n & 0x3ul) << 5) // Word length ( 0=5bits, 1=6bits, 2=7bits, 3=8bits )
#define UART_LCR_H_FEN        (0x1ul << 4)       // Enable FIFOs
#define UART_LCR_H_STP2       (0x1ul << 3)       // Two stop bits select 
#define UART_LCR_H_EPS        (0x1ul << 2)       // Even parity select
#define UART_LCR_H_PEN        (0x1ul << 1)       // Parity enable
#define UART_LCR_H_BRK        (0x1ul << 0)       // Send break
/********************* Bit definition for Contro(UARTCR) register *************************/
#define UART_CR_CTSEn        (0x1ul << 15)    // CTS hardware flow control enable
#define UART_CR_RTSEn        (0x1ul << 14)    // RTS hardware flow control enable
#define UART_CR_Out2         (0x1ul << 13)    // Complement of Out2 modem status output
#define UART_CR_Out1         (0x1ul << 12)    // Complement of Out1 modem status output
#define UART_CR_RTS          (0x1ul << 11)    // Request to send
#define UART_CR_DTR          (0x1ul << 10)    // Data transmit ready
#define UART_CR_RXE          (0x1ul <<  9)    // Receive enable
#define UART_CR_TXE          (0x1ul <<  8)    // Transmit enable
#define UART_CR_LBE          (0x1ul <<  7)    // Loop-back enable
#define UART_CR_SIRLP        (0x1ul <<  2)    // IrDA SIR low power mode
#define UART_CR_SIREN        (0x1ul <<  1)    // SIR enable
#define UART_CR_UARTEN       (0x1ul <<  0)    // UART enable
/******* Bit definition for Interrupt FIFO Level Select(UARTIFLS) register *****************/
#define UART_IFLS_RXIFLSEL(n)    ((n & 0x7ul) << 3) // Receive interrupt FIFO level select(0=1/8 full, 1=1/4 full, 2=1/2 full, 3=3/4 full, 4=7/8 full)
#define UART_IFLS_TXIFLSEL(n)    ((n & 0x7ul) << 0) // Transmit interrupt FIFO level select(0=1/8 full, 1=1/4 full, 2=1/2 full, 3=3/4 full, 4=7/8 full)
/******* Bit definition for Interrupt Mask Set/Clear(UARTIMSC) register ********************/
#define UART_IMSC_OEIM       (0x1ul << 10)    // Overrun error interrupt mask
#define UART_IMSC_BEIM       (0x1ul <<  9)    // Break error interrupt mask
#define UART_IMSC_PEIM       (0x1ul <<  8)    // Parity error interrupt mask
#define UART_IMSC_FEIM       (0x1ul <<  7)    // Framing error interrupt mask
#define UART_IMSC_RTIM       (0x1ul <<  6)    // Receive interrupt mask
#define UART_IMSC_TXIM       (0x1ul <<  5)    // Transmit interrupt mask
#define UART_IMSC_RXIM       (0x1ul <<  4)    // Receive interrupt mask
#define UART_IMSC_DSRMIM     (0x1ul <<  3)    // nUARTDSR modem interrupt mask
#define UART_IMSC_DCDMIM     (0x1ul <<  2)    // nUARTDCD modem interrupt mask
#define UART_IMSC_CTSMIM     (0x1ul <<  1)    // nUARTCTS modem interrupt mask
#define UART_IMSC_RIMIM      (0x1ul <<  0)    // nUARTRI modem interrupt mask
/*************** Bit definition for Raw Interrupt Status(UARTRIS) register *****************/
#define UART_RIS_OEIM       (0x1ul << 10)    // Overrun error interrupt status
#define UART_RIS_BEIM       (0x1ul <<  9)    // Break error interrupt status
#define UART_RIS_PEIM       (0x1ul <<  8)    // Parity error interrupt status 
#define UART_RIS_FEIM       (0x1ul <<  7)    // Framing error interrupt status
#define UART_RIS_RTIM       (0x1ul <<  6)    // Receive interrupt status
#define UART_RIS_TXIM       (0x1ul <<  5)    // Transmit interrupt status
#define UART_RIS_RXIM       (0x1ul <<  4)    // Receive interrupt status
#define UART_RIS_DSRMIM     (0x1ul <<  3)    // nUARTDSR modem interrupt status
#define UART_RIS_DCDMIM     (0x1ul <<  2)    // nUARTDCD modem interrupt status
#define UART_RIS_CTSMIM     (0x1ul <<  1)    // nUARTCTS modem interrupt status
#define UART_RIS_RIMIM      (0x1ul <<  0)    // nUARTRI modem interrupt status
/************** Bit definition for Masked Interrupt Status(UARTMIS) register ****************/
#define UART_MIS_OEIM       (0x1ul << 10)    // Overrun error masked interrupt status
#define UART_MIS_BEIM       (0x1ul <<  9)    // Break error masked interrupt status
#define UART_MIS_PEIM       (0x1ul <<  8)    // Parity error masked interrupt status 
#define UART_MIS_FEIM       (0x1ul <<  7)    // Framing error masked interrupt status
#define UART_MIS_RTIM       (0x1ul <<  6)    // Receive masked interrupt status
#define UART_MIS_TXIM       (0x1ul <<  5)    // Transmit masked interrupt status
#define UART_MIS_RXIM       (0x1ul <<  4)    // Receive masked interrupt status
#define UART_MIS_DSRMIM     (0x1ul <<  3)    // nUARTDSR modem masked interrupt status
#define UART_MIS_DCDMIM     (0x1ul <<  2)    // nUARTDCD modem masked interrupt status
#define UART_MIS_CTSMIM     (0x1ul <<  1)    // nUARTCTS modem masked interrupt status
#define UART_MIS_RIMIM      (0x1ul <<  0)    // nUARTRI modem masked interrupt status
/*************** Bit definition for Interrupt Clear(UARTICR) register ************************/
#define UART_ICR_OEIM       (0x1ul << 10)    // Overrun error interrupt clear 
#define UART_ICR_BEIM       (0x1ul <<  9)    // Break error interrupt clear
#define UART_ICR_PEIM       (0x1ul <<  8)    // Parity error interrupt clear
#define UART_ICR_FEIM       (0x1ul <<  7)    // Framing error interrupt clear
#define UART_ICR_RTIM       (0x1ul <<  6)    // Receive interrupt clear
#define UART_ICR_TXIM       (0x1ul <<  5)    // Transmit interrupt clear
#define UART_ICR_RXIM       (0x1ul <<  4)    // Receive interrupt clear
#define UART_ICR_DSRMIM     (0x1ul <<  3)    // nUARTDSR modem interrupt clear
#define UART_ICR_DCDMIM     (0x1ul <<  2)    // nUARTDCD modem interrupt clear
#define UART_ICR_CTSMIM     (0x1ul <<  1)    // nUARTCTS modem interrupt clear
#define UART_ICR_RIMIM      (0x1ul <<  0)    // nUARTRI modem interrupt clear
/***************** Bit definition for DMA Control(UARTDMACR) register ************************/
#define UART_DMACR_DMAONERR (0x1ul <<  2)    // DMA on error
#define UART_DMACR_TXDMAE   (0x1ul <<  1)    // Transmit DMA enable
#define UART_DMACR_RXDMAE   (0x1ul <<  0)    // Receive DMA enable

/******************************************************************************/
/*                                                                            */
/*                           Simple UART                                      */
/*                                                                            */
/******************************************************************************/
/***************** Bit definition for S_UART Data () register ************************/
#define S_UART_DATA                  (0xFFul << 0)    
/***************** Bit definition for S_UART State() register ************************/
#define S_UART_STATE_TX_BUF_OVERRUN  (0x01ul << 2)       // TX buffer overrun, wirte 1 to clear.
#define S_UART_STATE_RX_BUF_FULL     (0x01ul << 1)       // RX buffer full, read only.
#define S_UART_STATE_TX_BUF_FULL     (0x01ul << 0)       // TX buffer full, read only.
/***************** Bit definition for S_UART Control() register ************************/
#define S_UART_CTRL_HIGH_SPEED_TEST  (0x01ul << 6)       // High-speed test mode for TX only.
#define S_UART_CTRL_RX_OVERRUN_EN    (0x01ul << 5)       // RX overrun interrupt enable.
#define S_UART_CTRL_TX_OVERRUN_EN    (0x01ul << 4)       // TX overrun interrupt enable.
#define S_UART_CTRL_RX_INT_EN        (0x01ul << 3)       // RX interrupt enable.
#define S_UART_CTRL_TX_INT_EN        (0x01ul << 2)       // TX interrupt enable.
#define S_UART_CTRL_RX_EN            (0x01ul << 1)       // RX enable.
#define S_UART_CTRL_TX_EN            (0x01ul << 0)       // TX enable.
/***************** Bit definition for S_UART Interrupt() register ************************/
#define S_UART_INT_RX_OVERRUN        (0x01ul << 3)       // RX overrun interrupt. Wirte 1 to clear
#define S_UART_INT_TX_OVERRUN        (0x01ul << 2)       // TX overrun interrupt. Write 1 to clear
#define S_UART_INT_RX                (0x01ul << 1)       // RX interrupt. Write 1 to clear
#define S_UART_INT_TX                (0x01ul << 0)       // TX interrupt. Write 1 to clear

/******************************************************************************/
/*                                                                            */
/*                          Analog Digital Register                           */
/*                                                                            */
/******************************************************************************/

/*********************** Bit definition for ADC_CTR     ***********************/
//#define ADC_CTR_SAMSEL_ABNORMAL           (0x0ul)	  // Abnormal Operation
//#define ADC_CTR_SAMSEL_NORMAL             (0x1ul)       // Normal Operation
#define ADC_CTR_PWD_NRMOP                 (0x1ul)  // Active Operation
#define ADC_CTR_PWD_PD                    (0x3ul)  // Power down
/*********************** Bit definition for ADC_CHSEL   ***********************/
#define ADC_CHSEL_CH0                     (0x0ul)       // Channel 0
#define ADC_CHSEL_CH1                     (0x1ul)       // Channel 1
#define ADC_CHSEL_CH2                     (0x2ul)       // Channel 2
#define ADC_CHSEL_CH3                     (0x3ul)       // Channel 3
#define ADC_CHSEL_CH4                     (0x4ul)       // Channel 4
#define ADC_CHSEL_CH5                     (0x5ul)       // Channel 5
#define ADC_CHSEL_CH6                     (0x6ul)       // Channel 6
#define ADC_CHSEL_CH7                     (0x7ul)       // Channel 7
#define ADC_CHSEL_CH15                   (0xful)        // LDO output(1.5V)
/*********************** Bit definition for ADC_START   ***********************/
#define ADC_START_START                   (0x1ul)       // ADC conversion start
/*********************** Bit definition for ADC_DATA    ***********************/
//ToDo (Readonly)

/*********************** Bit definition for ADC_INT     ***********************/
#define ADC_INT_MASK_DIS                  (0x0ul << 1)  // Interrupt disable
#define ADC_INT_MASK_ENA                  (0x1ul << 1)  // Interrupt enable
//ToDo (Readonly)

/*********************** Bit definition for ADC_INTCLR  ***********************/
#define ADC_INTCLEAR                      (0x1ul)       // ADC Interrupt clear

#define W7500x_ADC_BASE                   (W7500x_APB2_BASE + 0x00000000UL)
#define ADC                               ((ADC_TypeDef *) W7500x_ADC_BASE)

/******************************************************************************/
/*                                                                            */
/*                                  Dual Timer                                */
/*                                                                            */
/******************************************************************************/

/*********************** Bit definition for dualtimer   ***********************/
#define DUALTIMER_TimerControl_TimerDIsable (0x0ul << 7)
#define DUALTIMER_TimerControl_TimerEnable  (0x1ul << 7)
#define DUALTIMER_TimerControl_FreeRunning  (0x0ul << 6)
#define DUALTIMER_TimerControl_Periodic     (0x1ul << 6)
#define DUALTIMER_TimerControl_IntDisable   (0x0ul << 5)
#define DUALTIMER_TimerControl_IntEnable    (0x1ul << 5)
#define DUALTIMER_TimerControl_Pre_1        (0x0ul << 2)
#define DUALTIMER_TimerControl_Pre_16       (0x1ul << 2)
#define DUALTIMER_TimerControl_Pre_256      (0x2ul << 2)
#define DUALTIMER_TimerControl_Size_16      (0x0ul << 1)
#define DUALTIMER_TimerControl_Size_32      (0x1ul << 1)
#define DUALTIMER_TimerControl_Wrapping     (0x0ul << 0)
#define DUALTIMER_TimerControl_OneShot      (0x1ul << 0)

/******************************************************************************/
/*                                                                            */
/*                              External Interrupt                            */
/*                                                                            */
/******************************************************************************/

/****************   Bit definition for Px_IER   **************************/
#define EXTI_Px_INTPOR_RISING_EDGE   (0x00ul << 0)
#define EXTI_Px_INTPOR_FALLING_EDGE  (0x01ul << 0)
#define EXTI_Px_INTEN_DISABLE        (0x00ul << 1)
#define EXTI_Px_INTEN_ENABLE         (0x01ul << 1)

/******************************************************************************/
/*                                                                            */
/*                             GPIO                                           */
/*                                                                            */
/******************************************************************************/

/****************   Bit definition for Px_AFSR   **************************/
#define Px_AFSR_AF0     (0x00ul)
#define Px_AFSR_AF1     (0x01ul)
#define Px_AFSR_AF2     (0x02ul)
#define Px_AFSR_AF3     (0x03ul)
/****************   Bit definition for Px_PCR   **************************/
#define Px_PCR_PUPD_DOWN    (0x01ul << 0)       // Pull Down
#define Px_PCR_PUPD_UP      (0x01ul << 1)       // Pull Up
#define Px_PCR_DS_HIGH      (0x01ul << 2)       // High Driving
#define Px_PCR_OD           (0x01ul << 3)       // Open Drain
#define Px_PCR_IE           (0x01ul << 5)       // Input Buffer Enable
#define Px_PCR_CS_SUMMIT    (0x01ul << 6)       // Use Summit Trigger Input Buffer

/******************************************************************************/
/*                                                                            */
/*                                   I2C                                      */
/*                                                                            */
/******************************************************************************/

/****************   Bit definition for I2C_CTR    **************************/
#define I2C_CTR_COREEN        (0x01ul  << 7 )   //  0x80
#define I2C_CTR_INTEREN       (0x01ul  << 6 )   //  0x40
#define I2C_CTR_MODE          (0x01ul  << 5 )   //  0x20
#define I2C_CTR_ADDR10        (0x01ul  << 4 )   //  0x10
#define I2C_CTR_CTRRWN        (0x01ul  << 3 )   //  0x08
#define I2C_CTR_CTEN          (0x01ul  << 2 )   //  0x04

/****************   Bit definition for I2C_CMDR   **************************/
#define I2C_CMDR_STA           (0x01ul  << 7 )    //    0x80
#define I2C_CMDR_STO           (0x01ul  << 6 )    //    0x40
#define I2C_CMDR_ACK           (0x01ul  << 5 )    //    0x20
#define I2C_CMDR_RESTA         (0x01ul  << 4 )    //    0x10

/****************   Bit definition for I2C_ISCR   **************************/
#define I2C_ISCR_RST          (0x01ul  << 1)    //     0x01

/****************   Bit definition for I2C_SR     **************************/
#define I2C_SR_TX             (0x01ul  << 9 )   //  0x200
#define I2C_SR_RX             (0x01ul  << 8 )   //  0x100
#define I2C_SR_ACKT           (0x01ul  << 7 )   //  0x080
#define I2C_SR_BT             (0x01ul  << 6 )   //  0x040
#define I2C_SR_SA             (0x01ul  << 5 )   //  0x020
#define I2C_SR_SB             (0x01ul  << 4 )   //  0x010
#define I2C_SR_AL             (0x01ul  << 3 )   //  0x008
#define I2C_SR_TO             (0x01ul  << 2 )   //  0x004
#define I2C_SR_SRW            (0x01ul  << 1 )   //  0x002
#define I2C_SR_ACKR           (0x01ul  << 0 )   //  0x001

/****************   Bit definition for I2C_ISR    **************************/
#define I2C_ISR_STAE            (0x01ul  << 4 )   //  0x010
#define I2C_ISR_STOE            (0x01ul  << 3 )   //  0x008
#define I2C_ISR_TOE             (0x01ul  << 2 )   //  0x004
#define I2C_ISR_ACK_RXE         (0x01ul  << 1 )   //  0x002
#define I2C_ISR_ACK_TXE         (0x01ul  << 0 )   //  0x001

/****************   Bit definition for I2C_ISMR   **************************/
#define I2C_ISR_STAEM            (0x01ul  << 4 )   //  0x010
#define I2C_ISR_STOEM            (0x01ul  << 3 )   //  0x008
#define I2C_ISR_TOEM             (0x01ul  << 2 )   //  0x004
#define I2C_ISR_ACK_RXEM         (0x01ul  << 1 )   //  0x002
#define I2C_ISR_ACK_TXEM         (0x01ul  << 0 )   //  0x001

/******************************************************************************/
/*                                                                            */
/*                                   PWM                                      */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                      Random number generator Register                      */
/*                                                                            */
/******************************************************************************/

/*********************** Bit definition for RNG_RUN     ***********************/
#define RNG_RUN_STOP                      (0x0ul)       // STOP RNG shift register
#define RNG_RUN_RUN                       (0x1ul)       // RUN RNG shift register
/*********************** Bit definition for RNG_SEED    ***********************/
//ToDo

/*********************** Bit definition for RNG_CLKSEL  ***********************/
#define RNG_CLKSEL_RNGCLK                 (0x0ul)       // RNGCLK is source clock for rng shift register
#define RNG_CLKSEL_APBCLK                 (0x1ul)       // APBCLK is source clock for rng shift register
/*********************** Bit definition for RNG_ENABLE  ***********************/
#define RNG_MANUAL_DISABLE                  (0x0ul)       // RNG disble
#define RNG_MANUAL_ENABLE                   (0x1ul)       // RNG enable
/*********************** Bit definition for RNG_RN      ***********************/
//ToDo

/*********************** Bit definition for RNG_POLY    ***********************/
//ToDo



#if !defined  (USE_HAL_DRIVER)
#define USE_HAL_DRIVER
#endif /* USE_HAL_DRIVER */



#if defined (USE_HAL_DRIVER)
    #include "W7500x_conf.h"
#endif

#ifdef __cplusplus
}
#endif

#endif  /* W7500x_H */



/************************ (C) COPYRIGHT Wiznet *****END OF FILE****/
