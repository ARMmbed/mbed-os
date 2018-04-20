/*!
*****************************************************************************
 * @file:    startup_ADuCM3029.h
 * @brief:   CMSIS Cortex-M3 Core Peripheral Access Layer Header File for
 *           ADI ADuCxxx Device Series
 * @version: $Revision: $
 * @date:    $Date: $
 *-----------------------------------------------------------------------------
 *
Copyright (c) 2010-2018 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

/*
WEAK_FUNC(func)
   If this is available for a compiler, apply whatever attributes are needed
   to a function definition ("func") to flag that the function is a "weak" one.
VECTOR_SECTION
   A particular setup may have a requirement that the vector table be placed
   in a particular section. This specifies the name of that section
RESET_EXCPT_HNDLR
   A particular setup may have a requirement for a different reset handler.
   This specifies the name of that handler.
*/

#ifndef __STARTUP_H__
#define __STARTUP_H__

#define VECTOR_SECTION                 ".vectors"
/* IVT typedefs. */
typedef void( *pFunc )( void );

#ifdef __ARMCC_VERSION
void Default_Handler(void);
#define SECTION_NAME(sectionname)      __attribute__((section(sectionname)))
#define SECTION_PLACE(def,sectionname) def __attribute__((section(sectionname)))
#define IVT_NAME                       __Vectors
#define RESET_EXCPT_HNDLR              __main
#define COMPILER_NAME                  "ARMCC"
#define WEAK_FUNCTION(x)               void x (void) __attribute__((weak, alias("Default_Handler")));
extern uint32_t Load$$LR$$LR_IROM1$$Base[];
#define NVIC_FLASH_VECTOR_ADDRESS      ((uint32_t)Load$$LR$$LR_IROM1$$Base)

#elif defined(__ICCARM__)
#pragma diag_suppress=Pm093,Pm140
#define SECTION_PLACE(def,sectionname) def @ sectionname
#define IVT_NAME                       __vector_table
#define WEAK_FUNC(func)                __weak func
#define RESET_EXCPT_HNDLR              __iar_program_start
#define COMPILER_NAME                  "ICCARM"
#define WEAK_FUNCTION(x)    WEAK_FUNC  ( void x (void)) { while(1){} }
#pragma section=VECTOR_SECTION
#define NVIC_FLASH_VECTOR_ADDRESS      ((uint32_t)__section_begin(VECTOR_SECTION))

#elif defined(__GNUC__)
extern unsigned __etext;
extern unsigned __data_start__;
extern unsigned __data_end__;
extern unsigned __copy_table_start__;
extern unsigned __copy_table_end__;
extern unsigned __zero_table_start__;
extern unsigned __zero_table_end__;
extern unsigned __bss_start__;
extern unsigned __bss_end__;
extern unsigned __StackTop;
void Default_Handler(void);
#ifndef __START
extern void  _start(void) __attribute__((noreturn));    /* PreeMain (C library entry point) */
#define RESET_EXCPT_HNDLR              _start
#else
extern int  __START(void) __attribute__((noreturn));    /* main entry point */
#define RESET_EXCPT_HNDLR              __START
#endif
#define SECTION_NAME(sectionname)      __attribute__ ((section(sectionname)))
#define SECTION_PLACE(def,sectionname) def __attribute__ ((section(sectionname)))
#define IVT_NAME                       __Vectors
#define COMPILER_NAME                  "GNUC"
#define WEAK_FUNCTION(x)               void x (void) __attribute__ ((weak, alias("Default_Handler")));
extern const pFunc IVT_NAME[];
#define NVIC_FLASH_VECTOR_ADDRESS      ((uint32_t)IVT_NAME)
#define __STARTUP_CLEAR_BSS_MULTIPLE
#endif // __GNUC__

#define LASTCRCPAGE                    0
#define BLANKX4   0xFFFFFFFF
#define BLANKX20  BLANKX4,BLANKX4,BLANKX4,BLANKX4,BLANKX4,BLANKX4,BLANKX4,BLANKX4
#define BLANKX100 BLANKX20,BLANKX20,BLANKX20,BLANKX20,BLANKX20,BLANKX20,BLANKX20,BLANKX20
#define BLANKX600 BLANKX100,BLANKX100,BLANKX100,BLANKX100,BLANKX100,BLANKX100
#define BLANKX60  BLANKX20,BLANKX20,BLANKX20
void RESET_EXCPT_HNDLR(void);
void Reset_Handler(void);

#define ADUCM3029_VECTORS           /* Cortex-M3 Exceptions Handler */ \
    Reset_Handler,                            /* -15 */                \
    NMI_Handler,                              /* -14 */                \
    HardFault_Handler,                        /* -13 */                \
    MemManage_Handler,                        /* -12 */                \
    BusFault_Handler,                         /* -11 */                \
    UsageFault_Handler,                       /* -10 */                \
    0,                                        /* -9  */                \
    0,                                        /* -8  */                \
    0,                                        /* -7  */                \
    0,                                        /* -6  */                \
    SVC_Handler,                              /* -5  */                \
    DebugMon_Handler,                         /* -4  */                \
    0,                                        /* -3  */                \
    PendSV_Handler,                           /* -2  */                \
    SysTick_Handler,                          /* -1  */                \
    /* External interrupts */                                          \
    RTC1_Int_Handler,                         /* 0  */                 \
    Ext_Int0_Handler,                         /* 1  */                 \
    Ext_Int1_Handler,                         /* 2  */                 \
    Ext_Int2_Handler,                         /* 3  */                 \
    Ext_Int3_Handler,                         /* 4  */                 \
    WDog_Tmr_Int_Handler,                     /* 5  */                 \
    Vreg_over_Int_Handler,                    /* 6  */                 \
    Battery_Voltage_Int_Handler,              /* 7  */                 \
    RTC0_Int_Handler,                         /* 8  */                 \
    GPIO_A_Int_Handler,                       /* 9  */                 \
    GPIO_B_Int_Handler,                       /* 10 */                 \
    GP_Tmr0_Int_Handler,                      /* 11 */                 \
    GP_Tmr1_Int_Handler,                      /* 12 */                 \
    Flash0_Int_Handler,                       /* 13 */                 \
    UART_Int_Handler,                         /* 14 */                 \
    SPI0_Int_Handler,                         /* 15 */                 \
    SPI2_Int_Handler,                         /* 16 */                 \
    I2C0_Slave_Int_Handler,                   /* 17 */                 \
    I2C0_Master_Int_Handler,                  /* 18 */                 \
    DMA_Err_Int_Handler,                      /* 19 */                 \
    DMA_SPI2_TX_Int_Handler,                  /* 20 */                 \
    DMA_SPI2_RX_Int_Handler,                  /* 21 */                 \
    DMA_SPORT0A_Int_Handler,                  /* 22 */                 \
    DMA_SPORT0B_Int_Handler,                  /* 23 */                 \
    DMA_SPI0_TX_Int_Handler,                  /* 24 */                 \
    DMA_SPI0_RX_Int_Handler,                  /* 25 */                 \
    DMA_SPI1_TX_Int_Handler,                  /* 26 */                 \
    DMA_SPI1_RX_Int_Handler,                  /* 27 */                 \
    DMA_UART_TX_Int_Handler,                  /* 28 */                 \
    DMA_UART_RX_Int_Handler,                  /* 29 */                 \
    DMA_I2C0_STX_Int_Handler,                 /* 30 */                 \
    DMA_I2C0_SRX_Int_Handler,                 /* 31 */                 \
    DMA_I2C0_MX_Int_Handler,                  /* 32 */                 \
    DMA_AES0_IN_Int_Handler,                  /* 33 */                 \
    DMA_AES0_OUT_Int_Handler,                 /* 34 */                 \
    DMA_FLASH0_Int_Handler,                   /* 35 */                 \
    SPORT0A_Int_Handler,                      /* 36 */                 \
    SPORT0B_Int_Handler,                      /* 37 */                 \
    Crypto_Int_Handler,                       /* 38 */                 \
    DMA_ADC0_Int_Handler,                     /* 39 */                 \
    GP_Tmr2_Int_Handler,                      /* 40 */                 \
    Crystal_osc_Int_Handler,                  /* 41 */                 \
    SPI1_Int_Handler,                         /* 42 */                 \
    PLL_Int_Handler,                          /* 43 */                 \
    RNG_Int_Handler,                          /* 44 */                 \
    Beep_Int_Handler,                         /* 45 */                 \
    ADC_Int_Handler,                          /* 46 */                 \
    0,                                        /* 47 */                 \
    0,                                        /* 48 */                 \
    0,                                        /* 49 */                 \
    0,                                        /* 50 */                 \
    0,                                        /* 51 */                 \
    0,                                        /* 52 */                 \
    0,                                        /* 53 */                 \
    0,                                        /* 54 */                 \
    0,                                        /* 55 */                 \
    DMA_SIP0_Int_Handler,                     /* 56 */                 \
    DMA_SIP1_Int_Handler,                     /* 57 */                 \
    DMA_SIP2_Int_Handler,                     /* 58 */                 \
    DMA_SIP3_Int_Handler,                     /* 59 */                 \
    DMA_SIP4_Int_Handler,                     /* 60 */                 \
    DMA_SIP5_Int_Handler,                     /* 61 */                 \
    DMA_SIP6_Int_Handler,                     /* 62 */                 \
    DMA_SIP7_Int_Handler,                     /* 63 */                 \
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,          /* 64 - 79 */            \
    (pFunc)BLANKX4, (pFunc)BLANKX4,           /* security_options */   \
    (pFunc)BLANKX4, (pFunc)BLANKX4,                                    \
    (pFunc)0xA79C3203u, (pFunc)LASTCRCPAGE,                            \
    (pFunc)BLANKX4, (pFunc)BLANKX4            /* 80 - 87 */

#endif /* __STARTUP_H__ */
