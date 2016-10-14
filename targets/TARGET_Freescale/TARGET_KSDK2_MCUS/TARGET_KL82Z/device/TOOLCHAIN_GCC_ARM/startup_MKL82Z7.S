/* ---------------------------------------------------------------------------------------*/
/*  @file:    startup_MKL82Z7.s                                                           */
/*  @purpose: CMSIS Cortex-M0P Core Device Startup File                                   */
/*            MKL82Z7                                                                     */
/*  @version: 1.5                                                                         */
/*  @date:    2015-9-24                                                                   */
/*  @build:   b151217                                                                     */
/* ---------------------------------------------------------------------------------------*/
/*                                                                                        */
/* Copyright (c) 1997 - 2015 , Freescale Semiconductor, Inc.                              */
/* All rights reserved.                                                                   */
/*                                                                                        */
/* Redistribution and use in source and binary forms, with or without modification,       */
/* are permitted provided that the following conditions are met:                          */
/*                                                                                        */
/* o Redistributions of source code must retain the above copyright notice, this list     */
/*   of conditions and the following disclaimer.                                          */
/*                                                                                        */
/* o Redistributions in binary form must reproduce the above copyright notice, this       */
/*   list of conditions and the following disclaimer in the documentation and/or          */
/*   other materials provided with the distribution.                                      */
/*                                                                                        */
/* o Neither the name of Freescale Semiconductor, Inc. nor the names of its               */
/*   contributors may be used to endorse or promote products derived from this            */
/*   software without specific prior written permission.                                  */
/*                                                                                        */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND        */
/* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED          */
/* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE                 */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR       */
/* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES         */
/* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;           */
/* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON         */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT                */
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS          */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                           */
/*****************************************************************************/
/* Version: GCC for ARM Embedded Processors                                  */
/*****************************************************************************/
    .syntax unified
    .arch armv6-m

    .section .isr_vector, "a"
    .align 2
    .globl __isr_vector
__isr_vector:
    .long   __StackTop                                      /* Top of Stack */
    .long   Reset_Handler                                   /* Reset Handler */
    .long   NMI_Handler                                     /* NMI Handler*/
    .long   HardFault_Handler                               /* Hard Fault Handler*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   SVC_Handler                                     /* SVCall Handler*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   PendSV_Handler                                  /* PendSV Handler*/
    .long   SysTick_Handler                                 /* SysTick Handler*/

                                                            /* External Interrupts*/
    .long   DMA0_DMA4_IRQHandler                            /* DMA channel 0, 4 transfer complete*/
    .long   DMA1_DMA5_IRQHandler                            /* DMA channel 1, 5 transfer complete*/
    .long   DMA2_DMA6_IRQHandler                            /* DMA channel 2, 6 transfer complete*/
    .long   DMA3_DMA7_IRQHandler                            /* DMA channel 3, 7 transfer complete*/
    .long   DMA_Error_IRQHandler                            /* DMA channel 0 - 7 error*/
    .long   FLEXIO0_IRQHandler                              /* Flexible IO*/
    .long   TPM0_IRQHandler                                 /* Timer/PWM module 0*/
    .long   TPM1_IRQHandler                                 /* Timer/PWM module 1*/
    .long   TPM2_IRQHandler                                 /* Timer/PWM module 2*/
    .long   PIT0_IRQHandler                                 /* Periodic Interrupt Timer 0*/
    .long   SPI0_IRQHandler                                 /* Serial Peripheral Interface 0*/
    .long   EMVSIM0_IRQHandler                              /* EMVSIM0 common interrupt*/
    .long   LPUART0_IRQHandler                              /* LPUART0 status and error*/
    .long   LPUART1_IRQHandler                              /* LPUART1 status and error*/
    .long   I2C0_IRQHandler                                 /* Inter-Integrated Circuit 0*/
    .long   QSPI0_IRQHandler                                /* QuadSPI0 interrupt*/
    .long   Reserved32_IRQHandler                           /* DryIce tamper detect*/
    .long   PORTA_IRQHandler                                /* Pin detect Port A*/
    .long   PORTB_IRQHandler                                /* Pin detect Port B*/
    .long   PORTC_IRQHandler                                /* Pin detect Port C*/
    .long   PORTD_IRQHandler                                /* Pin detect Port D*/
    .long   PORTE_IRQHandler                                /* Pin detect Port E*/
    .long   LLWU_IRQHandler                                 /* Low Leakage Wakeup*/
    .long   LTC0_IRQHandler                                 /* Low power trusted cryptographic*/
    .long   USB0_IRQHandler                                 /* USB OTG interrupt*/
    .long   ADC0_IRQHandler                                 /* Analog-to-Digital Converter 0*/
    .long   LPTMR0_IRQHandler                               /* Low-Power Timer 0*/
    .long   RTC_Seconds_IRQHandler                          /* RTC seconds*/
    .long   INTMUX0_0_IRQHandler                            /* Selectable peripheral interrupt INTMUX0-0*/
    .long   INTMUX0_1_IRQHandler                            /* Selectable peripheral interrupt INTMUX0-1*/
    .long   INTMUX0_2_IRQHandler                            /* Selectable peripheral interrupt INTMUX0-2*/
    .long   INTMUX0_3_IRQHandler                            /* Selectable peripheral interrupt INTMUX0-3*/
    .long   LPTMR1_IRQHandler                               /* Low-Power Timer 1 (INTMUX source IRQ0)*/
    .long   Reserved49_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ1)*/
    .long   Reserved50_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ2)*/
    .long   Reserved51_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ3)*/
    .long   SPI1_IRQHandler                                 /* Serial Peripheral Interface 1 (INTMUX source IRQ4)*/
    .long   LPUART2_IRQHandler                              /* LPUART2 status and error (INTMUX source IRQ5)*/
    .long   EMVSIM1_IRQHandler                              /* EMVSIM1 common interrupt (INTMUX source IRQ6)*/
    .long   I2C1_IRQHandler                                 /* Inter-Integrated Circuit 1 (INTMUX source IRQ7)*/
    .long   TSI0_IRQHandler                                 /* Touch Sensing Input 0 (INTMUX source IRQ8)*/
    .long   PMC_IRQHandler                                  /* PMC controller low-voltage detect, low-voltage warning (INTMUX source IRQ9)*/
    .long   FTFA_IRQHandler                                 /* FTFA command complete/read collision (INTMUX source IRQ10)*/
    .long   MCG_IRQHandler                                  /* Multipurpose clock generator (INTMUX source IRQ11)*/
    .long   WDOG_EWM_IRQHandler                             /* Single interrupt vector for  WDOG and EWM (INTMUX source IRQ12)*/
    .long   DAC0_IRQHandler                                 /* Digital-to-analog converter 0 (INTMUX source IRQ13)*/
    .long   TRNG0_IRQHandler                                /* True randon number generator (INTMUX source IRQ14)*/
    .long   Reserved63_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ15)*/
    .long   CMP0_IRQHandler                                 /* Comparator 0 (INTMUX source IRQ16)*/
    .long   Reserved65_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ17)*/
    .long   RTC_Alarm_IRQHandler                            /* Real time clock (INTMUX source IRQ18)*/
    .long   Reserved67_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ19)*/
    .long   Reserved68_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ20)*/
    .long   Reserved69_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ21)*/
    .long   Reserved70_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ22)*/
    .long   Reserved71_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ23)*/
    .long   DMA4_IRQHandler                                 /* DMA channel 4 transfer complete (INTMUX source IRQ24)*/
    .long   DMA5_IRQHandler                                 /* DMA channel 5 transfer complete (INTMUX source IRQ25)*/
    .long   DMA6_IRQHandler                                 /* DMA channel 6 transfer complete (INTMUX source IRQ26)*/
    .long   DMA7_IRQHandler                                 /* DMA channel 7 transfer complete (INTMUX source IRQ27)*/
    .long   Reserved76_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ28)*/
    .long   Reserved77_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ29)*/
    .long   Reserved78_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ30)*/
    .long   Reserved79_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ31)*/

    .size    __isr_vector, . - __isr_vector

/* Flash Configuration */
    .section .FlashConfig, "a"
    .long 0xFFFFFFFF
    .long 0xFFFFFFFF
    .long 0xFFFFFFFF
    .long 0xFFFF3DFE

    .text
    .thumb

/* Reset Handler */

    .thumb_func
    .align 2
    .globl   Reset_Handler
    .weak    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
    cpsid   i               /* Mask interrupts */
    .equ    VTOR, 0xE000ED08
    ldr     r0, =VTOR
    ldr     r1, =__isr_vector
    str     r1, [r0]
#ifndef __NO_SYSTEM_INIT
    ldr   r0,=SystemInit
    blx   r0
#endif
/*     Loop to copy data from read only memory to RAM. The ranges
 *      of copy from/to are specified by following symbols evaluated in
 *      linker script.
 *      __etext: End of code section, i.e., begin of data sections to copy from.
 *      __data_start__/__data_end__: RAM address range that data should be
 *      copied to. Both must be aligned to 4 bytes boundary.  */

    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

    subs    r3, r2
    ble     .LC0

.LC1:
    subs    r3, 4
    ldr    r0, [r1,r3]
    str    r0, [r2,r3]
    bgt    .LC1
.LC0:

#ifdef __STARTUP_CLEAR_BSS
/*     This part of work usually is done in C library startup code. Otherwise,
 *     define this macro to enable it in this startup.
 *
 *     Loop to zero out BSS section, which uses following symbols
 *     in linker script:
 *      __bss_start__: start of BSS section. Must align to 4
 *      __bss_end__: end of BSS section. Must align to 4
 */
    ldr r1, =__bss_start__
    ldr r2, =__bss_end__

    subs    r2, r1
    ble .LC3

    movs    r0, 0
.LC2:
    str r0, [r1, r2]
    subs    r2, 4
    bge .LC2
.LC3:
#endif
    cpsie   i               /* Unmask interrupts */
#ifndef __START
#define __START _start
#endif
#ifndef __ATOLLIC__
    ldr   r0,=__START
    blx   r0
#else
    ldr   r0,=__libc_init_array
    blx   r0
    ldr   r0,=main
    bx    r0
#endif
    .pool
    .size Reset_Handler, . - Reset_Handler

    .align  1
    .thumb_func
    .weak DefaultISR
    .type DefaultISR, %function
DefaultISR:
    ldr r0, =DefaultISR
    bx r0
    .size DefaultISR, . - DefaultISR

    .align 1
    .thumb_func
    .weak NMI_Handler
    .type NMI_Handler, %function
NMI_Handler:
    ldr   r0,=NMI_Handler
    bx    r0
    .size NMI_Handler, . - NMI_Handler

    .align 1
    .thumb_func
    .weak HardFault_Handler
    .type HardFault_Handler, %function
HardFault_Handler:
    ldr   r0,=HardFault_Handler
    bx    r0
    .size HardFault_Handler, . - HardFault_Handler

    .align 1
    .thumb_func
    .weak SVC_Handler
    .type SVC_Handler, %function
SVC_Handler:
    ldr   r0,=SVC_Handler
    bx    r0
    .size SVC_Handler, . - SVC_Handler

    .align 1
    .thumb_func
    .weak PendSV_Handler
    .type PendSV_Handler, %function
PendSV_Handler:
    ldr   r0,=PendSV_Handler
    bx    r0
    .size PendSV_Handler, . - PendSV_Handler

    .align 1
    .thumb_func
    .weak SysTick_Handler
    .type SysTick_Handler, %function
SysTick_Handler:
    ldr   r0,=SysTick_Handler
    bx    r0
    .size SysTick_Handler, . - SysTick_Handler

    .align 1
    .thumb_func
    .weak DMA0_DMA4_IRQHandler
    .type DMA0_DMA4_IRQHandler, %function
DMA0_DMA4_IRQHandler:
    ldr   r0,=DMA0_DMA4_DriverIRQHandler
    bx    r0
    .size DMA0_DMA4_IRQHandler, . - DMA0_DMA4_IRQHandler

    .align 1
    .thumb_func
    .weak DMA1_DMA5_IRQHandler
    .type DMA1_DMA5_IRQHandler, %function
DMA1_DMA5_IRQHandler:
    ldr   r0,=DMA1_DMA5_DriverIRQHandler
    bx    r0
    .size DMA1_DMA5_IRQHandler, . - DMA1_DMA5_IRQHandler

    .align 1
    .thumb_func
    .weak DMA2_DMA6_IRQHandler
    .type DMA2_DMA6_IRQHandler, %function
DMA2_DMA6_IRQHandler:
    ldr   r0,=DMA2_DMA6_DriverIRQHandler
    bx    r0
    .size DMA2_DMA6_IRQHandler, . - DMA2_DMA6_IRQHandler

    .align 1
    .thumb_func
    .weak DMA3_DMA7_IRQHandler
    .type DMA3_DMA7_IRQHandler, %function
DMA3_DMA7_IRQHandler:
    ldr   r0,=DMA3_DMA7_DriverIRQHandler
    bx    r0
    .size DMA3_DMA7_IRQHandler, . - DMA3_DMA7_IRQHandler

    .align 1
    .thumb_func
    .weak DMA_Error_IRQHandler
    .type DMA_Error_IRQHandler, %function
DMA_Error_IRQHandler:
    ldr   r0,=DMA_Error_DriverIRQHandler
    bx    r0
    .size DMA_Error_IRQHandler, . - DMA_Error_IRQHandler

    .align 1
    .thumb_func
    .weak FLEXIO0_IRQHandler
    .type FLEXIO0_IRQHandler, %function
FLEXIO0_IRQHandler:
    ldr   r0,=FLEXIO0_DriverIRQHandler
    bx    r0
    .size FLEXIO0_IRQHandler, . - FLEXIO0_IRQHandler

    .align 1
    .thumb_func
    .weak SPI0_IRQHandler
    .type SPI0_IRQHandler, %function
SPI0_IRQHandler:
    ldr   r0,=SPI0_DriverIRQHandler
    bx    r0
    .size SPI0_IRQHandler, . - SPI0_IRQHandler

    .align 1
    .thumb_func
    .weak LPUART0_IRQHandler
    .type LPUART0_IRQHandler, %function
LPUART0_IRQHandler:
    ldr   r0,=LPUART0_DriverIRQHandler
    bx    r0
    .size LPUART0_IRQHandler, . - LPUART0_IRQHandler

    .align 1
    .thumb_func
    .weak LPUART1_IRQHandler
    .type LPUART1_IRQHandler, %function
LPUART1_IRQHandler:
    ldr   r0,=LPUART1_DriverIRQHandler
    bx    r0
    .size LPUART1_IRQHandler, . - LPUART1_IRQHandler

    .align 1
    .thumb_func
    .weak I2C0_IRQHandler
    .type I2C0_IRQHandler, %function
I2C0_IRQHandler:
    ldr   r0,=I2C0_DriverIRQHandler
    bx    r0
    .size I2C0_IRQHandler, . - I2C0_IRQHandler

    .align 1
    .thumb_func
    .weak QSPI0_IRQHandler
    .type QSPI0_IRQHandler, %function
QSPI0_IRQHandler:
    ldr   r0,=QSPI0_DriverIRQHandler
    bx    r0
    .size QSPI0_IRQHandler, . - QSPI0_IRQHandler

    .align 1
    .thumb_func
    .weak INTMUX0_0_IRQHandler
    .type INTMUX0_0_IRQHandler, %function
INTMUX0_0_IRQHandler:
    ldr   r0,=INTMUX0_0_DriverIRQHandler
    bx    r0
    .size INTMUX0_0_IRQHandler, . - INTMUX0_0_IRQHandler

    .align 1
    .thumb_func
    .weak INTMUX0_1_IRQHandler
    .type INTMUX0_1_IRQHandler, %function
INTMUX0_1_IRQHandler:
    ldr   r0,=INTMUX0_1_DriverIRQHandler
    bx    r0
    .size INTMUX0_1_IRQHandler, . - INTMUX0_1_IRQHandler

    .align 1
    .thumb_func
    .weak INTMUX0_2_IRQHandler
    .type INTMUX0_2_IRQHandler, %function
INTMUX0_2_IRQHandler:
    ldr   r0,=INTMUX0_2_DriverIRQHandler
    bx    r0
    .size INTMUX0_2_IRQHandler, . - INTMUX0_2_IRQHandler

    .align 1
    .thumb_func
    .weak INTMUX0_3_IRQHandler
    .type INTMUX0_3_IRQHandler, %function
INTMUX0_3_IRQHandler:
    ldr   r0,=INTMUX0_3_DriverIRQHandler
    bx    r0
    .size INTMUX0_3_IRQHandler, . - INTMUX0_3_IRQHandler

    .align 1
    .thumb_func
    .weak SPI1_IRQHandler
    .type SPI1_IRQHandler, %function
SPI1_IRQHandler:
    ldr   r0,=SPI1_DriverIRQHandler
    bx    r0
    .size SPI1_IRQHandler, . - SPI1_IRQHandler

    .align 1
    .thumb_func
    .weak LPUART2_IRQHandler
    .type LPUART2_IRQHandler, %function
LPUART2_IRQHandler:
    ldr   r0,=LPUART2_DriverIRQHandler
    bx    r0
    .size LPUART2_IRQHandler, . - LPUART2_IRQHandler

    .align 1
    .thumb_func
    .weak I2C1_IRQHandler
    .type I2C1_IRQHandler, %function
I2C1_IRQHandler:
    ldr   r0,=I2C1_DriverIRQHandler
    bx    r0
    .size I2C1_IRQHandler, . - I2C1_IRQHandler

    .align 1
    .thumb_func
    .weak DMA4_IRQHandler
    .type DMA4_IRQHandler, %function
DMA4_IRQHandler:
    ldr   r0,=DMA4_DriverIRQHandler
    bx    r0
    .size DMA4_IRQHandler, . - DMA4_IRQHandler

    .align 1
    .thumb_func
    .weak DMA5_IRQHandler
    .type DMA5_IRQHandler, %function
DMA5_IRQHandler:
    ldr   r0,=DMA5_DriverIRQHandler
    bx    r0
    .size DMA5_IRQHandler, . - DMA5_IRQHandler

    .align 1
    .thumb_func
    .weak DMA6_IRQHandler
    .type DMA6_IRQHandler, %function
DMA6_IRQHandler:
    ldr   r0,=DMA6_DriverIRQHandler
    bx    r0
    .size DMA6_IRQHandler, . - DMA6_IRQHandler

    .align 1
    .thumb_func
    .weak DMA7_IRQHandler
    .type DMA7_IRQHandler, %function
DMA7_IRQHandler:
    ldr   r0,=DMA7_DriverIRQHandler
    bx    r0
    .size DMA7_IRQHandler, . - DMA7_IRQHandler


/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro def_irq_handler  handler_name
    .weak \handler_name
    .set  \handler_name, DefaultISR
    .endm

/* Exception Handlers */
    def_irq_handler    DMA0_DMA4_DriverIRQHandler
    def_irq_handler    DMA1_DMA5_DriverIRQHandler
    def_irq_handler    DMA2_DMA6_DriverIRQHandler
    def_irq_handler    DMA3_DMA7_DriverIRQHandler
    def_irq_handler    DMA_Error_DriverIRQHandler
    def_irq_handler    FLEXIO0_DriverIRQHandler
    def_irq_handler    TPM0_IRQHandler
    def_irq_handler    TPM1_IRQHandler
    def_irq_handler    TPM2_IRQHandler
    def_irq_handler    PIT0_IRQHandler
    def_irq_handler    SPI0_DriverIRQHandler
    def_irq_handler    EMVSIM0_IRQHandler
    def_irq_handler    LPUART0_DriverIRQHandler
    def_irq_handler    LPUART1_DriverIRQHandler
    def_irq_handler    I2C0_DriverIRQHandler
    def_irq_handler    QSPI0_DriverIRQHandler
    def_irq_handler    Reserved32_IRQHandler
    def_irq_handler    PORTA_IRQHandler
    def_irq_handler    PORTB_IRQHandler
    def_irq_handler    PORTC_IRQHandler
    def_irq_handler    PORTD_IRQHandler
    def_irq_handler    PORTE_IRQHandler
    def_irq_handler    LLWU_IRQHandler
    def_irq_handler    LTC0_IRQHandler
    def_irq_handler    USB0_IRQHandler
    def_irq_handler    ADC0_IRQHandler
    def_irq_handler    LPTMR0_IRQHandler
    def_irq_handler    RTC_Seconds_IRQHandler
    def_irq_handler    INTMUX0_0_DriverIRQHandler
    def_irq_handler    INTMUX0_1_DriverIRQHandler
    def_irq_handler    INTMUX0_2_DriverIRQHandler
    def_irq_handler    INTMUX0_3_DriverIRQHandler
    def_irq_handler    LPTMR1_IRQHandler
    def_irq_handler    Reserved49_IRQHandler
    def_irq_handler    Reserved50_IRQHandler
    def_irq_handler    Reserved51_IRQHandler
    def_irq_handler    SPI1_DriverIRQHandler
    def_irq_handler    LPUART2_DriverIRQHandler
    def_irq_handler    EMVSIM1_IRQHandler
    def_irq_handler    I2C1_DriverIRQHandler
    def_irq_handler    TSI0_IRQHandler
    def_irq_handler    PMC_IRQHandler
    def_irq_handler    FTFA_IRQHandler
    def_irq_handler    MCG_IRQHandler
    def_irq_handler    WDOG_EWM_IRQHandler
    def_irq_handler    DAC0_IRQHandler
    def_irq_handler    TRNG0_IRQHandler
    def_irq_handler    Reserved63_IRQHandler
    def_irq_handler    CMP0_IRQHandler
    def_irq_handler    Reserved65_IRQHandler
    def_irq_handler    RTC_Alarm_IRQHandler
    def_irq_handler    Reserved67_IRQHandler
    def_irq_handler    Reserved68_IRQHandler
    def_irq_handler    Reserved69_IRQHandler
    def_irq_handler    Reserved70_IRQHandler
    def_irq_handler    Reserved71_IRQHandler
    def_irq_handler    DMA4_DriverIRQHandler
    def_irq_handler    DMA5_DriverIRQHandler
    def_irq_handler    DMA6_DriverIRQHandler
    def_irq_handler    DMA7_DriverIRQHandler
    def_irq_handler    Reserved76_IRQHandler
    def_irq_handler    Reserved77_IRQHandler
    def_irq_handler    Reserved78_IRQHandler
    def_irq_handler    Reserved79_IRQHandler

    .end
