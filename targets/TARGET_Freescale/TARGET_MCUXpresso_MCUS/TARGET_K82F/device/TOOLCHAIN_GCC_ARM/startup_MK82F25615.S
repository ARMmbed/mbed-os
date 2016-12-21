/* ---------------------------------------------------------------------------------------*/
/*  @file:    startup_MK82F25615.s                                                        */
/*  @purpose: CMSIS Cortex-M4 Core Device Startup File                                    */
/*            MK82F25615                                                                  */
/*  @version: 1.0                                                                         */
/*  @date:    2015-4-9                                                                    */
/*  @build:   b151210                                                                     */
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
    .arch armv7-m

    .section .isr_vector, "a"
    .align 2
    .globl __isr_vector
__isr_vector:
    .long   __StackTop                                      /* Top of Stack */
    .long   Reset_Handler                                   /* Reset Handler */
    .long   NMI_Handler                                     /* NMI Handler*/
    .long   HardFault_Handler                               /* Hard Fault Handler*/
    .long   MemManage_Handler                               /* MPU Fault Handler*/
    .long   BusFault_Handler                                /* Bus Fault Handler*/
    .long   UsageFault_Handler                              /* Usage Fault Handler*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   SVC_Handler                                     /* SVCall Handler*/
    .long   DebugMon_Handler                                /* Debug Monitor Handler*/
    .long   0                                               /* Reserved*/
    .long   PendSV_Handler                                  /* PendSV Handler*/
    .long   SysTick_Handler                                 /* SysTick Handler*/

                                                            /* External Interrupts*/
    .long   DMA0_DMA16_IRQHandler                           /* DMA channel 0,16 transfer complete*/
    .long   DMA1_DMA17_IRQHandler                           /* DMA channel 1,17 transfer complete*/
    .long   DMA2_DMA18_IRQHandler                           /* DMA channel 2,18 transfer complete*/
    .long   DMA3_DMA19_IRQHandler                           /* DMA channel 3,19 transfer complete*/
    .long   DMA4_DMA20_IRQHandler                           /* DMA channel 4,20 transfer complete*/
    .long   DMA5_DMA21_IRQHandler                           /* DMA channel 5,21 transfer complete*/
    .long   DMA6_DMA22_IRQHandler                           /* DMA channel 6,22 transfer complete*/
    .long   DMA7_DMA23_IRQHandler                           /* DMA channel 7,23 transfer complete*/
    .long   DMA8_DMA24_IRQHandler                           /* DMA channel 8,24 transfer complete*/
    .long   DMA9_DMA25_IRQHandler                           /* DMA channel 9,25 transfer complete*/
    .long   DMA10_DMA26_IRQHandler                          /* DMA channel 10,26 transfer complete*/
    .long   DMA11_DMA27_IRQHandler                          /* DMA channel 11,27 transfer complete*/
    .long   DMA12_DMA28_IRQHandler                          /* DMA channel 12,28 transfer complete*/
    .long   DMA13_DMA29_IRQHandler                          /* DMA channel 13,29 transfer complete*/
    .long   DMA14_DMA30_IRQHandler                          /* DMA channel 14,30 transfer complete*/
    .long   DMA15_DMA31_IRQHandler                          /* DMA channel 15,31 transfer complete*/
    .long   DMA_Error_IRQHandler                            /* DMA channel 0 - 31 error*/
    .long   MCM_IRQHandler                                  /* MCM normal interrupt*/
    .long   FTFA_IRQHandler                                 /* FTFA command complete*/
    .long   Read_Collision_IRQHandler                       /* FTFA read collision*/
    .long   LVD_LVW_IRQHandler                              /* PMC controller low-voltage detect, low-voltage warning*/
    .long   LLWU_IRQHandler                                 /* Low leakage wakeup unit*/
    .long   WDOG_EWM_IRQHandler                             /* Single interrupt vector for  WDOG and EWM*/
    .long   TRNG0_IRQHandler                                /* True randon number generator*/
    .long   I2C0_IRQHandler                                 /* Inter-integrated circuit 0*/
    .long   I2C1_IRQHandler                                 /* Inter-integrated circuit 1*/
    .long   SPI0_IRQHandler                                 /* Serial peripheral Interface 0*/
    .long   SPI1_IRQHandler                                 /* Serial peripheral Interface 1*/
    .long   I2S0_Tx_IRQHandler                              /* Integrated interchip sound 0 transmit interrupt*/
    .long   I2S0_Rx_IRQHandler                              /* Integrated interchip sound 0 receive interrupt*/
    .long   LPUART0_IRQHandler                              /* LPUART0 receive/transmit/error interrupt*/
    .long   LPUART1_IRQHandler                              /* LPUART1 receive/transmit/error interrupt*/
    .long   LPUART2_IRQHandler                              /* LPUART2 receive/transmit/error interrupt*/
    .long   LPUART3_IRQHandler                              /* LPUART3 receive/transmit/error interrupt*/
    .long   LPUART4_IRQHandler                              /* LPUART4 receive/transmit/error interrupt*/
    .long   Reserved51_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved52_IRQHandler                           /* Reserved interrupt*/
    .long   EMVSIM0_IRQHandler                              /* EMVSIM0 common interrupt*/
    .long   EMVSIM1_IRQHandler                              /* EMVSIM1 common interrupt*/
    .long   ADC0_IRQHandler                                 /* Analog-to-digital converter 0*/
    .long   CMP0_IRQHandler                                 /* Comparator 0*/
    .long   CMP1_IRQHandler                                 /* Comparator 1*/
    .long   FTM0_IRQHandler                                 /* FlexTimer module 0 fault, overflow and channels interrupt*/
    .long   FTM1_IRQHandler                                 /* FlexTimer module 1 fault, overflow and channels interrupt*/
    .long   FTM2_IRQHandler                                 /* FlexTimer module 2 fault, overflow and channels interrupt*/
    .long   CMT_IRQHandler                                  /* Carrier modulator transmitter*/
    .long   RTC_IRQHandler                                  /* Real time clock*/
    .long   RTC_Seconds_IRQHandler                          /* Real time clock seconds*/
    .long   PIT0CH0_IRQHandler                              /* Periodic interrupt timer 0 channel 0*/
    .long   PIT0CH1_IRQHandler                              /* Periodic interrupt timer 0 channel 1*/
    .long   PIT0CH2_IRQHandler                              /* Periodic interrupt timer 0 channel 2*/
    .long   PIT0CH3_IRQHandler                              /* Periodic interrupt timer 0 channel 3*/
    .long   PDB0_IRQHandler                                 /* Programmable delay block*/
    .long   USB0_IRQHandler                                 /* USB OTG interrupt*/
    .long   USBDCD_IRQHandler                               /* USB charger detect*/
    .long   Reserved71_IRQHandler                           /* Reserved interrupt*/
    .long   DAC0_IRQHandler                                 /* Digital-to-analog converter 0*/
    .long   MCG_IRQHandler                                  /* Multipurpose clock generator*/
    .long   LPTMR0_LPTMR1_IRQHandler                        /* Single interrupt vector for  Low Power Timer 0 and 1*/
    .long   PORTA_IRQHandler                                /* Port A pin detect interrupt*/
    .long   PORTB_IRQHandler                                /* Port B pin detect interrupt*/
    .long   PORTC_IRQHandler                                /* Port C pin detect interrupt*/
    .long   PORTD_IRQHandler                                /* Port D pin detect interrupt*/
    .long   PORTE_IRQHandler                                /* Port E pin detect interrupt*/
    .long   SWI_IRQHandler                                  /* Software interrupt*/
    .long   SPI2_IRQHandler                                 /* Serial peripheral Interface 2*/
    .long   Reserved82_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved83_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved84_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved85_IRQHandler                           /* Reserved interrupt*/
    .long   FLEXIO0_IRQHandler                              /* FLEXIO0*/
    .long   FTM3_IRQHandler                                 /* FlexTimer module 3 fault, overflow and channels interrupt*/
    .long   Reserved88_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved89_IRQHandler                           /* Reserved interrupt*/
    .long   I2C2_IRQHandler                                 /* Inter-integrated circuit 2*/
    .long   Reserved91_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved92_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved93_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved94_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved95_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved96_IRQHandler                           /* Reserved interrupt*/
    .long   SDHC_IRQHandler                                 /* Secured digital host controller*/
    .long   Reserved98_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved99_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved100_IRQHandler                          /* Reserved interrupt*/
    .long   Reserved101_IRQHandler                          /* Reserved interrupt*/
    .long   Reserved102_IRQHandler                          /* Reserved interrupt*/
    .long   TSI0_IRQHandler                                 /* Touch Sensing Input*/
    .long   TPM1_IRQHandler                                 /* TPM1 single interrupt vector for all sources*/
    .long   TPM2_IRQHandler                                 /* TPM2 single interrupt vector for all sources*/
    .long   Reserved106_IRQHandler                          /* Reserved interrupt*/
    .long   I2C3_IRQHandler                                 /* Inter-integrated circuit 3*/
    .long   Reserved108_IRQHandler                          /* Reserved interrupt*/
    .long   Reserved109_IRQHandler                          /* Reserved interrupt*/
    .long   Reserved110_IRQHandler                          /* Reserved interrupt*/
    .long   Reserved111_IRQHandler                          /* Reserved interrupt*/
    .long   Reserved112_IRQHandler                          /* Reserved interrupt*/
    .long   Reserved113_IRQHandler                          /* Reserved interrupt*/
    .long   Reserved114_IRQHandler                          /* Reserved interrupt*/
    .long   Reserved115_IRQHandler                          /* Reserved interrupt*/
    .long   QuadSPI0_IRQHandler                             /* qspi*/
    .long   Reserved117_IRQHandler                          /* Reserved interrupt*/
    .long   Reserved118_IRQHandler                          /* Reserved interrupt*/
    .long   Reserved119_IRQHandler                          /* Reserved interrupt*/
    .long   LTC0_IRQHandler                                 /* LP Trusted Cryptography*/
    .long   Reserved121_IRQHandler                          /* Reserved interrupt*/
    .long   Reserved122_IRQHandler                          /* Reserved interrupt*/
    .long   DefaultISR                                      /* 123*/
    .long   DefaultISR                                      /* 124*/
    .long   DefaultISR                                      /* 125*/
    .long   DefaultISR                                      /* 126*/
    .long   DefaultISR                                      /* 127*/
    .long   DefaultISR                                      /* 128*/
    .long   DefaultISR                                      /* 129*/
    .long   DefaultISR                                      /* 130*/
    .long   DefaultISR                                      /* 131*/
    .long   DefaultISR                                      /* 132*/
    .long   DefaultISR                                      /* 133*/
    .long   DefaultISR                                      /* 134*/
    .long   DefaultISR                                      /* 135*/
    .long   DefaultISR                                      /* 136*/
    .long   DefaultISR                                      /* 137*/
    .long   DefaultISR                                      /* 138*/
    .long   DefaultISR                                      /* 139*/
    .long   DefaultISR                                      /* 140*/
    .long   DefaultISR                                      /* 141*/
    .long   DefaultISR                                      /* 142*/
    .long   DefaultISR                                      /* 143*/
    .long   DefaultISR                                      /* 144*/
    .long   DefaultISR                                      /* 145*/
    .long   DefaultISR                                      /* 146*/
    .long   DefaultISR                                      /* 147*/
    .long   DefaultISR                                      /* 148*/
    .long   DefaultISR                                      /* 149*/
    .long   DefaultISR                                      /* 150*/
    .long   DefaultISR                                      /* 151*/
    .long   DefaultISR                                      /* 152*/
    .long   DefaultISR                                      /* 153*/
    .long   DefaultISR                                      /* 154*/
    .long   DefaultISR                                      /* 155*/
    .long   DefaultISR                                      /* 156*/
    .long   DefaultISR                                      /* 157*/
    .long   DefaultISR                                      /* 158*/
    .long   DefaultISR                                      /* 159*/
    .long   DefaultISR                                      /* 160*/
    .long   DefaultISR                                      /* 161*/
    .long   DefaultISR                                      /* 162*/
    .long   DefaultISR                                      /* 163*/
    .long   DefaultISR                                      /* 164*/
    .long   DefaultISR                                      /* 165*/
    .long   DefaultISR                                      /* 166*/
    .long   DefaultISR                                      /* 167*/
    .long   DefaultISR                                      /* 168*/
    .long   DefaultISR                                      /* 169*/
    .long   DefaultISR                                      /* 170*/
    .long   DefaultISR                                      /* 171*/
    .long   DefaultISR                                      /* 172*/
    .long   DefaultISR                                      /* 173*/
    .long   DefaultISR                                      /* 174*/
    .long   DefaultISR                                      /* 175*/
    .long   DefaultISR                                      /* 176*/
    .long   DefaultISR                                      /* 177*/
    .long   DefaultISR                                      /* 178*/
    .long   DefaultISR                                      /* 179*/
    .long   DefaultISR                                      /* 180*/
    .long   DefaultISR                                      /* 181*/
    .long   DefaultISR                                      /* 182*/
    .long   DefaultISR                                      /* 183*/
    .long   DefaultISR                                      /* 184*/
    .long   DefaultISR                                      /* 185*/
    .long   DefaultISR                                      /* 186*/
    .long   DefaultISR                                      /* 187*/
    .long   DefaultISR                                      /* 188*/
    .long   DefaultISR                                      /* 189*/
    .long   DefaultISR                                      /* 190*/
    .long   DefaultISR                                      /* 191*/
    .long   DefaultISR                                      /* 192*/
    .long   DefaultISR                                      /* 193*/
    .long   DefaultISR                                      /* 194*/
    .long   DefaultISR                                      /* 195*/
    .long   DefaultISR                                      /* 196*/
    .long   DefaultISR                                      /* 197*/
    .long   DefaultISR                                      /* 198*/
    .long   DefaultISR                                      /* 199*/
    .long   DefaultISR                                      /* 200*/
    .long   DefaultISR                                      /* 201*/
    .long   DefaultISR                                      /* 202*/
    .long   DefaultISR                                      /* 203*/
    .long   DefaultISR                                      /* 204*/
    .long   DefaultISR                                      /* 205*/
    .long   DefaultISR                                      /* 206*/
    .long   DefaultISR                                      /* 207*/
    .long   DefaultISR                                      /* 208*/
    .long   DefaultISR                                      /* 209*/
    .long   DefaultISR                                      /* 210*/
    .long   DefaultISR                                      /* 211*/
    .long   DefaultISR                                      /* 212*/
    .long   DefaultISR                                      /* 213*/
    .long   DefaultISR                                      /* 214*/
    .long   DefaultISR                                      /* 215*/
    .long   DefaultISR                                      /* 216*/
    .long   DefaultISR                                      /* 217*/
    .long   DefaultISR                                      /* 218*/
    .long   DefaultISR                                      /* 219*/
    .long   DefaultISR                                      /* 220*/
    .long   DefaultISR                                      /* 221*/
    .long   DefaultISR                                      /* 222*/
    .long   DefaultISR                                      /* 223*/
    .long   DefaultISR                                      /* 224*/
    .long   DefaultISR                                      /* 225*/
    .long   DefaultISR                                      /* 226*/
    .long   DefaultISR                                      /* 227*/
    .long   DefaultISR                                      /* 228*/
    .long   DefaultISR                                      /* 229*/
    .long   DefaultISR                                      /* 230*/
    .long   DefaultISR                                      /* 231*/
    .long   DefaultISR                                      /* 232*/
    .long   DefaultISR                                      /* 233*/
    .long   DefaultISR                                      /* 234*/
    .long   DefaultISR                                      /* 235*/
    .long   DefaultISR                                      /* 236*/
    .long   DefaultISR                                      /* 237*/
    .long   DefaultISR                                      /* 238*/
    .long   DefaultISR                                      /* 239*/

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

#if 1
/* Here are two copies of loop implemenations. First one favors code size
 * and the second one favors performance. Default uses the first one.
 * Change to "#if 0" to use the second one */
.LC0:
    cmp     r2, r3
    ittt    lt
    ldrlt   r0, [r1], #4
    strlt   r0, [r2], #4
    blt    .LC0
#else
    subs    r3, r2
    ble    .LC1
.LC0:
    subs    r3, #4
    ldr    r0, [r1, r3]
    str    r0, [r2, r3]
    bgt    .LC0
.LC1:
#endif

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

    movs    r0, 0
.LC2:
    cmp     r1, r2
    itt    lt
    strlt   r0, [r1], #4
    blt    .LC2
#endif /* __STARTUP_CLEAR_BSS */

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
    b DefaultISR
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
    .weak DMA0_DMA16_IRQHandler
    .type DMA0_DMA16_IRQHandler, %function
DMA0_DMA16_IRQHandler:
    ldr   r0,=DMA0_DMA16_DriverIRQHandler
    bx    r0
    .size DMA0_DMA16_IRQHandler, . - DMA0_DMA16_IRQHandler

    .align 1
    .thumb_func
    .weak DMA1_DMA17_IRQHandler
    .type DMA1_DMA17_IRQHandler, %function
DMA1_DMA17_IRQHandler:
    ldr   r0,=DMA1_DMA17_DriverIRQHandler
    bx    r0
    .size DMA1_DMA17_IRQHandler, . - DMA1_DMA17_IRQHandler

    .align 1
    .thumb_func
    .weak DMA2_DMA18_IRQHandler
    .type DMA2_DMA18_IRQHandler, %function
DMA2_DMA18_IRQHandler:
    ldr   r0,=DMA2_DMA18_DriverIRQHandler
    bx    r0
    .size DMA2_DMA18_IRQHandler, . - DMA2_DMA18_IRQHandler

    .align 1
    .thumb_func
    .weak DMA3_DMA19_IRQHandler
    .type DMA3_DMA19_IRQHandler, %function
DMA3_DMA19_IRQHandler:
    ldr   r0,=DMA3_DMA19_DriverIRQHandler
    bx    r0
    .size DMA3_DMA19_IRQHandler, . - DMA3_DMA19_IRQHandler

    .align 1
    .thumb_func
    .weak DMA4_DMA20_IRQHandler
    .type DMA4_DMA20_IRQHandler, %function
DMA4_DMA20_IRQHandler:
    ldr   r0,=DMA4_DMA20_DriverIRQHandler
    bx    r0
    .size DMA4_DMA20_IRQHandler, . - DMA4_DMA20_IRQHandler

    .align 1
    .thumb_func
    .weak DMA5_DMA21_IRQHandler
    .type DMA5_DMA21_IRQHandler, %function
DMA5_DMA21_IRQHandler:
    ldr   r0,=DMA5_DMA21_DriverIRQHandler
    bx    r0
    .size DMA5_DMA21_IRQHandler, . - DMA5_DMA21_IRQHandler

    .align 1
    .thumb_func
    .weak DMA6_DMA22_IRQHandler
    .type DMA6_DMA22_IRQHandler, %function
DMA6_DMA22_IRQHandler:
    ldr   r0,=DMA6_DMA22_DriverIRQHandler
    bx    r0
    .size DMA6_DMA22_IRQHandler, . - DMA6_DMA22_IRQHandler

    .align 1
    .thumb_func
    .weak DMA7_DMA23_IRQHandler
    .type DMA7_DMA23_IRQHandler, %function
DMA7_DMA23_IRQHandler:
    ldr   r0,=DMA7_DMA23_DriverIRQHandler
    bx    r0
    .size DMA7_DMA23_IRQHandler, . - DMA7_DMA23_IRQHandler

    .align 1
    .thumb_func
    .weak DMA8_DMA24_IRQHandler
    .type DMA8_DMA24_IRQHandler, %function
DMA8_DMA24_IRQHandler:
    ldr   r0,=DMA8_DMA24_DriverIRQHandler
    bx    r0
    .size DMA8_DMA24_IRQHandler, . - DMA8_DMA24_IRQHandler

    .align 1
    .thumb_func
    .weak DMA9_DMA25_IRQHandler
    .type DMA9_DMA25_IRQHandler, %function
DMA9_DMA25_IRQHandler:
    ldr   r0,=DMA9_DMA25_DriverIRQHandler
    bx    r0
    .size DMA9_DMA25_IRQHandler, . - DMA9_DMA25_IRQHandler

    .align 1
    .thumb_func
    .weak DMA10_DMA26_IRQHandler
    .type DMA10_DMA26_IRQHandler, %function
DMA10_DMA26_IRQHandler:
    ldr   r0,=DMA10_DMA26_DriverIRQHandler
    bx    r0
    .size DMA10_DMA26_IRQHandler, . - DMA10_DMA26_IRQHandler

    .align 1
    .thumb_func
    .weak DMA11_DMA27_IRQHandler
    .type DMA11_DMA27_IRQHandler, %function
DMA11_DMA27_IRQHandler:
    ldr   r0,=DMA11_DMA27_DriverIRQHandler
    bx    r0
    .size DMA11_DMA27_IRQHandler, . - DMA11_DMA27_IRQHandler

    .align 1
    .thumb_func
    .weak DMA12_DMA28_IRQHandler
    .type DMA12_DMA28_IRQHandler, %function
DMA12_DMA28_IRQHandler:
    ldr   r0,=DMA12_DMA28_DriverIRQHandler
    bx    r0
    .size DMA12_DMA28_IRQHandler, . - DMA12_DMA28_IRQHandler

    .align 1
    .thumb_func
    .weak DMA13_DMA29_IRQHandler
    .type DMA13_DMA29_IRQHandler, %function
DMA13_DMA29_IRQHandler:
    ldr   r0,=DMA13_DMA29_DriverIRQHandler
    bx    r0
    .size DMA13_DMA29_IRQHandler, . - DMA13_DMA29_IRQHandler

    .align 1
    .thumb_func
    .weak DMA14_DMA30_IRQHandler
    .type DMA14_DMA30_IRQHandler, %function
DMA14_DMA30_IRQHandler:
    ldr   r0,=DMA14_DMA30_DriverIRQHandler
    bx    r0
    .size DMA14_DMA30_IRQHandler, . - DMA14_DMA30_IRQHandler

    .align 1
    .thumb_func
    .weak DMA15_DMA31_IRQHandler
    .type DMA15_DMA31_IRQHandler, %function
DMA15_DMA31_IRQHandler:
    ldr   r0,=DMA15_DMA31_DriverIRQHandler
    bx    r0
    .size DMA15_DMA31_IRQHandler, . - DMA15_DMA31_IRQHandler

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
    .weak I2C0_IRQHandler
    .type I2C0_IRQHandler, %function
I2C0_IRQHandler:
    ldr   r0,=I2C0_DriverIRQHandler
    bx    r0
    .size I2C0_IRQHandler, . - I2C0_IRQHandler

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
    .weak SPI0_IRQHandler
    .type SPI0_IRQHandler, %function
SPI0_IRQHandler:
    ldr   r0,=SPI0_DriverIRQHandler
    bx    r0
    .size SPI0_IRQHandler, . - SPI0_IRQHandler

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
    .weak I2S0_Tx_IRQHandler
    .type I2S0_Tx_IRQHandler, %function
I2S0_Tx_IRQHandler:
    ldr   r0,=I2S0_Tx_DriverIRQHandler
    bx    r0
    .size I2S0_Tx_IRQHandler, . - I2S0_Tx_IRQHandler

    .align 1
    .thumb_func
    .weak I2S0_Rx_IRQHandler
    .type I2S0_Rx_IRQHandler, %function
I2S0_Rx_IRQHandler:
    ldr   r0,=I2S0_Rx_DriverIRQHandler
    bx    r0
    .size I2S0_Rx_IRQHandler, . - I2S0_Rx_IRQHandler

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
    .weak LPUART2_IRQHandler
    .type LPUART2_IRQHandler, %function
LPUART2_IRQHandler:
    ldr   r0,=LPUART2_DriverIRQHandler
    bx    r0
    .size LPUART2_IRQHandler, . - LPUART2_IRQHandler

    .align 1
    .thumb_func
    .weak LPUART3_IRQHandler
    .type LPUART3_IRQHandler, %function
LPUART3_IRQHandler:
    ldr   r0,=LPUART3_DriverIRQHandler
    bx    r0
    .size LPUART3_IRQHandler, . - LPUART3_IRQHandler

    .align 1
    .thumb_func
    .weak LPUART4_IRQHandler
    .type LPUART4_IRQHandler, %function
LPUART4_IRQHandler:
    ldr   r0,=LPUART4_DriverIRQHandler
    bx    r0
    .size LPUART4_IRQHandler, . - LPUART4_IRQHandler

    .align 1
    .thumb_func
    .weak SPI2_IRQHandler
    .type SPI2_IRQHandler, %function
SPI2_IRQHandler:
    ldr   r0,=SPI2_DriverIRQHandler
    bx    r0
    .size SPI2_IRQHandler, . - SPI2_IRQHandler

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
    .weak I2C2_IRQHandler
    .type I2C2_IRQHandler, %function
I2C2_IRQHandler:
    ldr   r0,=I2C2_DriverIRQHandler
    bx    r0
    .size I2C2_IRQHandler, . - I2C2_IRQHandler

    .align 1
    .thumb_func
    .weak SDHC_IRQHandler
    .type SDHC_IRQHandler, %function
SDHC_IRQHandler:
    ldr   r0,=SDHC_DriverIRQHandler
    bx    r0
    .size SDHC_IRQHandler, . - SDHC_IRQHandler

    .align 1
    .thumb_func
    .weak I2C3_IRQHandler
    .type I2C3_IRQHandler, %function
I2C3_IRQHandler:
    ldr   r0,=I2C3_DriverIRQHandler
    bx    r0
    .size I2C3_IRQHandler, . - I2C3_IRQHandler

    .align 1
    .thumb_func
    .weak QuadSPI0_IRQHandler
    .type QuadSPI0_IRQHandler, %function
QuadSPI0_IRQHandler:
    ldr   r0,=QuadSPI0_DriverIRQHandler
    bx    r0
    .size QuadSPI0_IRQHandler, . - QuadSPI0_IRQHandler


/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro def_irq_handler  handler_name
    .weak \handler_name
    .set  \handler_name, DefaultISR
    .endm

/* Exception Handlers */
    def_irq_handler    MemManage_Handler
    def_irq_handler    BusFault_Handler
    def_irq_handler    UsageFault_Handler
    def_irq_handler    DebugMon_Handler
    def_irq_handler    DMA0_DMA16_DriverIRQHandler
    def_irq_handler    DMA1_DMA17_DriverIRQHandler
    def_irq_handler    DMA2_DMA18_DriverIRQHandler
    def_irq_handler    DMA3_DMA19_DriverIRQHandler
    def_irq_handler    DMA4_DMA20_DriverIRQHandler
    def_irq_handler    DMA5_DMA21_DriverIRQHandler
    def_irq_handler    DMA6_DMA22_DriverIRQHandler
    def_irq_handler    DMA7_DMA23_DriverIRQHandler
    def_irq_handler    DMA8_DMA24_DriverIRQHandler
    def_irq_handler    DMA9_DMA25_DriverIRQHandler
    def_irq_handler    DMA10_DMA26_DriverIRQHandler
    def_irq_handler    DMA11_DMA27_DriverIRQHandler
    def_irq_handler    DMA12_DMA28_DriverIRQHandler
    def_irq_handler    DMA13_DMA29_DriverIRQHandler
    def_irq_handler    DMA14_DMA30_DriverIRQHandler
    def_irq_handler    DMA15_DMA31_DriverIRQHandler
    def_irq_handler    DMA_Error_DriverIRQHandler
    def_irq_handler    MCM_IRQHandler
    def_irq_handler    FTFA_IRQHandler
    def_irq_handler    Read_Collision_IRQHandler
    def_irq_handler    LVD_LVW_IRQHandler
    def_irq_handler    LLWU_IRQHandler
    def_irq_handler    WDOG_EWM_IRQHandler
    def_irq_handler    TRNG0_IRQHandler
    def_irq_handler    I2C0_DriverIRQHandler
    def_irq_handler    I2C1_DriverIRQHandler
    def_irq_handler    SPI0_DriverIRQHandler
    def_irq_handler    SPI1_DriverIRQHandler
    def_irq_handler    I2S0_Tx_DriverIRQHandler
    def_irq_handler    I2S0_Rx_DriverIRQHandler
    def_irq_handler    LPUART0_DriverIRQHandler
    def_irq_handler    LPUART1_DriverIRQHandler
    def_irq_handler    LPUART2_DriverIRQHandler
    def_irq_handler    LPUART3_DriverIRQHandler
    def_irq_handler    LPUART4_DriverIRQHandler
    def_irq_handler    Reserved51_IRQHandler
    def_irq_handler    Reserved52_IRQHandler
    def_irq_handler    EMVSIM0_IRQHandler
    def_irq_handler    EMVSIM1_IRQHandler
    def_irq_handler    ADC0_IRQHandler
    def_irq_handler    CMP0_IRQHandler
    def_irq_handler    CMP1_IRQHandler
    def_irq_handler    FTM0_IRQHandler
    def_irq_handler    FTM1_IRQHandler
    def_irq_handler    FTM2_IRQHandler
    def_irq_handler    CMT_IRQHandler
    def_irq_handler    RTC_IRQHandler
    def_irq_handler    RTC_Seconds_IRQHandler
    def_irq_handler    PIT0CH0_IRQHandler
    def_irq_handler    PIT0CH1_IRQHandler
    def_irq_handler    PIT0CH2_IRQHandler
    def_irq_handler    PIT0CH3_IRQHandler
    def_irq_handler    PDB0_IRQHandler
    def_irq_handler    USB0_IRQHandler
    def_irq_handler    USBDCD_IRQHandler
    def_irq_handler    Reserved71_IRQHandler
    def_irq_handler    DAC0_IRQHandler
    def_irq_handler    MCG_IRQHandler
    def_irq_handler    LPTMR0_LPTMR1_IRQHandler
    def_irq_handler    PORTA_IRQHandler
    def_irq_handler    PORTB_IRQHandler
    def_irq_handler    PORTC_IRQHandler
    def_irq_handler    PORTD_IRQHandler
    def_irq_handler    PORTE_IRQHandler
    def_irq_handler    SWI_IRQHandler
    def_irq_handler    SPI2_DriverIRQHandler
    def_irq_handler    Reserved82_IRQHandler
    def_irq_handler    Reserved83_IRQHandler
    def_irq_handler    Reserved84_IRQHandler
    def_irq_handler    Reserved85_IRQHandler
    def_irq_handler    FLEXIO0_DriverIRQHandler
    def_irq_handler    FTM3_IRQHandler
    def_irq_handler    Reserved88_IRQHandler
    def_irq_handler    Reserved89_IRQHandler
    def_irq_handler    I2C2_DriverIRQHandler
    def_irq_handler    Reserved91_IRQHandler
    def_irq_handler    Reserved92_IRQHandler
    def_irq_handler    Reserved93_IRQHandler
    def_irq_handler    Reserved94_IRQHandler
    def_irq_handler    Reserved95_IRQHandler
    def_irq_handler    Reserved96_IRQHandler
    def_irq_handler    SDHC_DriverIRQHandler
    def_irq_handler    Reserved98_IRQHandler
    def_irq_handler    Reserved99_IRQHandler
    def_irq_handler    Reserved100_IRQHandler
    def_irq_handler    Reserved101_IRQHandler
    def_irq_handler    Reserved102_IRQHandler
    def_irq_handler    TSI0_IRQHandler
    def_irq_handler    TPM1_IRQHandler
    def_irq_handler    TPM2_IRQHandler
    def_irq_handler    Reserved106_IRQHandler
    def_irq_handler    I2C3_DriverIRQHandler
    def_irq_handler    Reserved108_IRQHandler
    def_irq_handler    Reserved109_IRQHandler
    def_irq_handler    Reserved110_IRQHandler
    def_irq_handler    Reserved111_IRQHandler
    def_irq_handler    Reserved112_IRQHandler
    def_irq_handler    Reserved113_IRQHandler
    def_irq_handler    Reserved114_IRQHandler
    def_irq_handler    Reserved115_IRQHandler
    def_irq_handler    QuadSPI0_DriverIRQHandler
    def_irq_handler    Reserved117_IRQHandler
    def_irq_handler    Reserved118_IRQHandler
    def_irq_handler    Reserved119_IRQHandler
    def_irq_handler    LTC0_IRQHandler
    def_irq_handler    Reserved121_IRQHandler
    def_irq_handler    Reserved122_IRQHandler

    .end
