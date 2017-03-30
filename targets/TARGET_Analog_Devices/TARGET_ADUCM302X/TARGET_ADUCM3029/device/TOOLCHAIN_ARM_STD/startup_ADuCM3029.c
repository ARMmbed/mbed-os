/* File: startup_ADuCM3029.c
 * Startup file for Analog Devices ADuCM3029 processor
 *
 * Based on startup_ARMCM3.c
 *
 */
/* Copyright (c) 2011 - 2014 ARM LIMITED
   Copyright (c) 2016 - 2017 Analog Devices, Inc.

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

#include <stdint.h>
#include <rt_misc.h>

/*----------------------------------------------------------------------------
  Handler names that can be overridden for a user-specified RTOS
 *----------------------------------------------------------------------------*/

#ifdef USER_SPECIFIED_RTOS
#include <user_rtos_support.h>
#endif

#ifndef SVC_HANDLER
#define SVC_HANDLER SVC_Handler
#endif

#ifndef PENDSV_HANDLER
#define PENDSV_HANDLER PendSV_Handler
#endif

#ifndef SYSTICK_HANDLER
#define SYSTICK_HANDLER SysTick_Handler
#endif

/*----------------------------------------------------------------------------
  Linker generated Symbols
 *----------------------------------------------------------------------------*/
extern uint32_t Image$$ADUCM_STACK$$ZI$$Limit[];
extern uint32_t Image$$ADUCM_HEAP$$Base[];
extern uint32_t Image$$ADUCM_HEAP$$ZI$$Limit[];

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler Function Prototype
 *----------------------------------------------------------------------------*/
typedef void( *pFunc )( void );

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
/* main entry point */
extern void __main(void);

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
/* Default empty handler */
void Default_Handler(void);
/* Reset handler */
void Reset_Handler(void);

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
/* Cortex-M3 Processor Exceptions */
void Reset_Handler               (void) __attribute__ ((weak));
void NMI_Handler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler           (void) __attribute__ ((weak, alias("Default_Handler")));
void MemManage_Handler           (void) __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler          (void) __attribute__ ((weak, alias("Default_Handler")));
void SVC_HANDLER                 (void) __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void PENDSV_HANDLER              (void) __attribute__ ((weak, alias("Default_Handler")));
void SYSTICK_HANDLER             (void) __attribute__ ((weak, alias("Default_Handler")));

/* ARMCM3 Specific Interrupts */
void RTC1_Int_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void Ext_Int0_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void Ext_Int1_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void Ext_Int2_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void Ext_Int3_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void WDog_Tmr_Int_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void Vreg_over_Int_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void Battery_Voltage_Int_Handler (void) __attribute__ ((weak, alias("Default_Handler")));
void RTC0_Int_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void GPIO_A_Int_Handler          (void) __attribute__ ((weak, alias("Default_Handler")));
void GPIO_B_Int_Handler          (void) __attribute__ ((weak, alias("Default_Handler")));
void GP_Tmr0_Int_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void GP_Tmr1_Int_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void Flash0_Int_Handler          (void) __attribute__ ((weak, alias("Default_Handler")));
void UART_Int_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void SPI0_Int_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void SPI2_Int_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void I2C0_Slave_Int_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));
void I2C0_Master_Int_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_Err_Int_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_SPI2_TX_Int_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_SPI2_RX_Int_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_SPORT0A_Int_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_SPORT0B_Int_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_SPI0_TX_Int_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_SPI0_RX_Int_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_SPI1_TX_Int_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_SPI1_RX_Int_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_UART_TX_Int_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_UART_RX_Int_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_I2C0_STX_Int_Handler    (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_I2C0_SRX_Int_Handler    (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_I2C0_MX_Int_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_AES0_IN_Int_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_AES0_OUT_Int_Handler    (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_FLASH0_Int_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));
void SPORT0A_Int_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void SPORT0B_Int_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void Crypto_Int_Handler          (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_ADC0_Int_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void GP_Tmr2_Int_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void Crystal_osc_Int_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));
void SPI1_Int_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void PLL_Int_Handler             (void) __attribute__ ((weak, alias("Default_Handler")));
void RNG_Int_Handler             (void) __attribute__ ((weak, alias("Default_Handler")));
void Beep_Int_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void ADC_Int_Handler             (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_SIP0_Int_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_SIP1_Int_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_SIP2_Int_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_SIP3_Int_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_SIP4_Int_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_SIP5_Int_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_SIP6_Int_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_SIP7_Int_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/
#define ADUCM3029_VECTORS         /* Cortex-M3 Exceptions Handler */ \
  (pFunc) &Image$$ADUCM_STACK$$ZI$$Limit,/* Initial Stack Pointer */ \
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
  SVC_HANDLER,                              /* -5  */                \
  DebugMon_Handler,                         /* -4  */                \
  0,                                        /* -3  */                \
  PENDSV_HANDLER,                           /* -2  */                \
  SYSTICK_HANDLER,                          /* -1  */                \
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
  DMA_SIP7_Int_Handler,                     /* 63 */

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table in ROM
 *----------------------------------------------------------------------------*/
__attribute__ ((section(".vectors")))
const pFunc __Vectors[] = {
	ADUCM3029_VECTORS   
};

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
void Reset_Handler(void) {
    SystemInit();
    __main(); 
    while (1);
}

/*----------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
void Default_Handler(void) {

	while(1);
}

/*----------------------------------------------------------------------------
  stackheap Handler
 *----------------------------------------------------------------------------*/
extern __value_in_regs struct __initial_stackheap __user_setup_stackheap(uint32_t R0, uint32_t R1, uint32_t R2, uint32_t R3) {
    struct __initial_stackheap r;
    r.heap_base = (uint32_t)Image$$ADUCM_HEAP$$Base;
    r.heap_limit = (uint32_t)Image$$ADUCM_HEAP$$ZI$$Limit;
    return r;
}
