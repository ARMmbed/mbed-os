/* ---------------------------------------------------------------------------------------*/
/*  @file:    startup_MKW24D5.s                                                           */
/*  @purpose: CMSIS Cortex-M4 Core Device Startup File                                    */
/*            MKW24D5                                                                     */
/*  @version: 2.0                                                                         */
/*  @date:    2014-11-26                                                                  */
/*  @build:   b160512                                                                     */
/* ---------------------------------------------------------------------------------------*/
/*                                                                                        */
/* Copyright (c) 1997 - 2016 , Freescale Semiconductor, Inc.                              */
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
    .long   DMA0_IRQHandler                                 /* DMA channel 0 transfer complete*/
    .long   DMA1_IRQHandler                                 /* DMA channel 1 transfer complete*/
    .long   DMA2_IRQHandler                                 /* DMA channel 2 transfer complete*/
    .long   DMA3_IRQHandler                                 /* DMA channel 3 transfer complete*/
    .long   DMA4_IRQHandler                                 /* DMA channel 4 transfer complete*/
    .long   DMA5_IRQHandler                                 /* DMA channel 5 transfer complete*/
    .long   DMA6_IRQHandler                                 /* DMA channel 6 transfer complete*/
    .long   DMA7_IRQHandler                                 /* DMA channel 7 transfer complete*/
    .long   DMA8_IRQHandler                                 /* DMA channel 8 transfer complete*/
    .long   DMA9_IRQHandler                                 /* DMA channel 9 transfer complete*/
    .long   DMA10_IRQHandler                                /* DMA channel 10 transfer complete*/
    .long   DMA11_IRQHandler                                /* DMA channel 11 transfer complete*/
    .long   DMA12_IRQHandler                                /* DMA channel 12 transfer complete*/
    .long   DMA13_IRQHandler                                /* DMA channel 13 transfer complete*/
    .long   DMA14_IRQHandler                                /* DMA channel 14 transfer complete*/
    .long   DMA15_IRQHandler                                /* DMA channel 15 transfer complete*/
    .long   DMA_Error_IRQHandler                            /* DMA channel 0 - 15 error*/
    .long   MCM_IRQHandler                                  /* MCM normal interrupt*/
    .long   FTFL_IRQHandler                                 /* FTFL command complete*/
    .long   FTFL_Collision_IRQHandler                       /* FTFL read collision*/
    .long   PMC_IRQHandler                                  /* PMC controller low-voltage detect, low-voltage warning*/
    .long   LLWU_IRQHandler                                 /* Low leakage wakeup*/
    .long   WDOG_EWM_IRQHandler                             /* Single interrupt vector for  WDOG and EWM*/
    .long   RNG_IRQHandler                                  /* Randon number generator*/
    .long   I2C0_IRQHandler                                 /* Inter-integrated circuit 0*/
    .long   I2C1_IRQHandler                                 /* Inter-integrated circuit 1*/
    .long   SPI0_IRQHandler                                 /* Serial peripheral Interface 0*/
    .long   SPI1_IRQHandler                                 /* Serial peripheral Interface 1*/
    .long   I2S0_Tx_IRQHandler                              /* Integrated interchip sound 0 transmit interrupt*/
    .long   I2S0_Rx_IRQHandler                              /* Integrated interchip sound 0 receive interrupt*/
    .long   Reserved46_IRQHandler                           /* Reserved interrupt*/
    .long   UART0_RX_TX_IRQHandler                          /* UART0 receive/transmit interrupt*/
    .long   UART0_ERR_IRQHandler                            /* UART0 error interrupt*/
    .long   UART1_RX_TX_IRQHandler                          /* UART1 receive/transmit interrupt*/
    .long   UART1_ERR_IRQHandler                            /* UART1 error interrupt*/
    .long   UART2_RX_TX_IRQHandler                          /* UART2 receive/transmit interrupt*/
    .long   UART2_ERR_IRQHandler                            /* UART2 error interrupt*/
    .long   Reserved53_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved54_IRQHandler                           /* Reserved interrupt*/
    .long   ADC0_IRQHandler                                 /* Analog-to-digital converter 0*/
    .long   CMP0_IRQHandler                                 /* Comparator 0*/
    .long   CMP1_IRQHandler                                 /* Comparator 1*/
    .long   FTM0_IRQHandler                                 /* FlexTimer module 0 fault, overflow and channels interrupt*/
    .long   FTM1_IRQHandler                                 /* FlexTimer module 1 fault, overflow and channels interrupt*/
    .long   FTM2_IRQHandler                                 /* FlexTimer module 2 fault, overflow and channels interrupt*/
    .long   CMT_IRQHandler                                  /* Carrier modulator transmitter*/
    .long   RTC_IRQHandler                                  /* Real time clock*/
    .long   RTC_Seconds_IRQHandler                          /* Real time clock seconds*/
    .long   PIT0_IRQHandler                                 /* Periodic interrupt timer channel 0*/
    .long   PIT1_IRQHandler                                 /* Periodic interrupt timer channel 1*/
    .long   PIT2_IRQHandler                                 /* Periodic interrupt timer channel 2*/
    .long   PIT3_IRQHandler                                 /* Periodic interrupt timer channel 3*/
    .long   PDB0_IRQHandler                                 /* Programmable delay block*/
    .long   USB0_IRQHandler                                 /* USB OTG interrupt*/
    .long   USBDCD_IRQHandler                               /* USB charger detect*/
    .long   Reserved71_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved72_IRQHandler                           /* Reserved interrupt*/
    .long   MCG_IRQHandler                                  /* Multipurpose clock generator*/
    .long   LPTMR0_IRQHandler                               /* Low power timer interrupt*/
    .long   PORTA_IRQHandler                                /* Port A pin detect interrupt*/
    .long   PORTB_IRQHandler                                /* Port B pin detect interrupt*/
    .long   PORTC_IRQHandler                                /* Port C pin detect interrupt*/
    .long   PORTD_IRQHandler                                /* Port D pin detect interrupt*/
    .long   PORTE_IRQHandler                                /* Port E pin detect interrupt*/
    .long   SWI_IRQHandler                                  /* Software interrupt*/
    .long   DefaultISR                                      /* 81*/
    .long   DefaultISR                                      /* 82*/
    .long   DefaultISR                                      /* 83*/
    .long   DefaultISR                                      /* 84*/
    .long   DefaultISR                                      /* 85*/
    .long   DefaultISR                                      /* 86*/
    .long   DefaultISR                                      /* 87*/
    .long   DefaultISR                                      /* 88*/
    .long   DefaultISR                                      /* 89*/
    .long   DefaultISR                                      /* 90*/
    .long   DefaultISR                                      /* 91*/
    .long   DefaultISR                                      /* 92*/
    .long   DefaultISR                                      /* 93*/
    .long   DefaultISR                                      /* 94*/
    .long   DefaultISR                                      /* 95*/
    .long   DefaultISR                                      /* 96*/
    .long   DefaultISR                                      /* 97*/
    .long   DefaultISR                                      /* 98*/
    .long   DefaultISR                                      /* 99*/
    .long   DefaultISR                                      /* 100*/
    .long   DefaultISR                                      /* 101*/
    .long   DefaultISR                                      /* 102*/
    .long   DefaultISR                                      /* 103*/
    .long   DefaultISR                                      /* 104*/
    .long   DefaultISR                                      /* 105*/
    .long   DefaultISR                                      /* 106*/
    .long   DefaultISR                                      /* 107*/
    .long   DefaultISR                                      /* 108*/
    .long   DefaultISR                                      /* 109*/
    .long   DefaultISR                                      /* 110*/
    .long   DefaultISR                                      /* 111*/
    .long   DefaultISR                                      /* 112*/
    .long   DefaultISR                                      /* 113*/
    .long   DefaultISR                                      /* 114*/
    .long   DefaultISR                                      /* 115*/
    .long   DefaultISR                                      /* 116*/
    .long   DefaultISR                                      /* 117*/
    .long   DefaultISR                                      /* 118*/
    .long   DefaultISR                                      /* 119*/
    .long   DefaultISR                                      /* 120*/
    .long   DefaultISR                                      /* 121*/
    .long   DefaultISR                                      /* 122*/
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
    .long   DefaultISR                                      /* 240*/
    .long   DefaultISR                                      /* 241*/
    .long   DefaultISR                                      /* 242*/
    .long   DefaultISR                                      /* 243*/
    .long   DefaultISR                                      /* 244*/
    .long   DefaultISR                                      /* 245*/
    .long   DefaultISR                                      /* 246*/
    .long   DefaultISR                                      /* 247*/
    .long   DefaultISR                                      /* 248*/
    .long   DefaultISR                                      /* 249*/
    .long   DefaultISR                                      /* 250*/
    .long   DefaultISR                                      /* 251*/
    .long   DefaultISR                                      /* 252*/
    .long   DefaultISR                                      /* 253*/
    .long   DefaultISR                                      /* 254*/
    .long   0xFFFFFFFF                                      /*  Reserved for user TRIM value*/

    .size    __isr_vector, . - __isr_vector

/* Flash Configuration */
    .section .FlashConfig, "a"
    .long 0xFFFFFFFF
    .long 0xFFFFFFFF
    .long 0xFFFFFFFF
    .long 0xFFFFFFFE

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
    .weak DMA0_IRQHandler
    .type DMA0_IRQHandler, %function
DMA0_IRQHandler:
    ldr   r0,=DMA0_DriverIRQHandler
    bx    r0
    .size DMA0_IRQHandler, . - DMA0_IRQHandler

    .align 1
    .thumb_func
    .weak DMA1_IRQHandler
    .type DMA1_IRQHandler, %function
DMA1_IRQHandler:
    ldr   r0,=DMA1_DriverIRQHandler
    bx    r0
    .size DMA1_IRQHandler, . - DMA1_IRQHandler

    .align 1
    .thumb_func
    .weak DMA2_IRQHandler
    .type DMA2_IRQHandler, %function
DMA2_IRQHandler:
    ldr   r0,=DMA2_DriverIRQHandler
    bx    r0
    .size DMA2_IRQHandler, . - DMA2_IRQHandler

    .align 1
    .thumb_func
    .weak DMA3_IRQHandler
    .type DMA3_IRQHandler, %function
DMA3_IRQHandler:
    ldr   r0,=DMA3_DriverIRQHandler
    bx    r0
    .size DMA3_IRQHandler, . - DMA3_IRQHandler

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

    .align 1
    .thumb_func
    .weak DMA8_IRQHandler
    .type DMA8_IRQHandler, %function
DMA8_IRQHandler:
    ldr   r0,=DMA8_DriverIRQHandler
    bx    r0
    .size DMA8_IRQHandler, . - DMA8_IRQHandler

    .align 1
    .thumb_func
    .weak DMA9_IRQHandler
    .type DMA9_IRQHandler, %function
DMA9_IRQHandler:
    ldr   r0,=DMA9_DriverIRQHandler
    bx    r0
    .size DMA9_IRQHandler, . - DMA9_IRQHandler

    .align 1
    .thumb_func
    .weak DMA10_IRQHandler
    .type DMA10_IRQHandler, %function
DMA10_IRQHandler:
    ldr   r0,=DMA10_DriverIRQHandler
    bx    r0
    .size DMA10_IRQHandler, . - DMA10_IRQHandler

    .align 1
    .thumb_func
    .weak DMA11_IRQHandler
    .type DMA11_IRQHandler, %function
DMA11_IRQHandler:
    ldr   r0,=DMA11_DriverIRQHandler
    bx    r0
    .size DMA11_IRQHandler, . - DMA11_IRQHandler

    .align 1
    .thumb_func
    .weak DMA12_IRQHandler
    .type DMA12_IRQHandler, %function
DMA12_IRQHandler:
    ldr   r0,=DMA12_DriverIRQHandler
    bx    r0
    .size DMA12_IRQHandler, . - DMA12_IRQHandler

    .align 1
    .thumb_func
    .weak DMA13_IRQHandler
    .type DMA13_IRQHandler, %function
DMA13_IRQHandler:
    ldr   r0,=DMA13_DriverIRQHandler
    bx    r0
    .size DMA13_IRQHandler, . - DMA13_IRQHandler

    .align 1
    .thumb_func
    .weak DMA14_IRQHandler
    .type DMA14_IRQHandler, %function
DMA14_IRQHandler:
    ldr   r0,=DMA14_DriverIRQHandler
    bx    r0
    .size DMA14_IRQHandler, . - DMA14_IRQHandler

    .align 1
    .thumb_func
    .weak DMA15_IRQHandler
    .type DMA15_IRQHandler, %function
DMA15_IRQHandler:
    ldr   r0,=DMA15_DriverIRQHandler
    bx    r0
    .size DMA15_IRQHandler, . - DMA15_IRQHandler

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
    .weak UART0_RX_TX_IRQHandler
    .type UART0_RX_TX_IRQHandler, %function
UART0_RX_TX_IRQHandler:
    ldr   r0,=UART0_RX_TX_DriverIRQHandler
    bx    r0
    .size UART0_RX_TX_IRQHandler, . - UART0_RX_TX_IRQHandler

    .align 1
    .thumb_func
    .weak UART0_ERR_IRQHandler
    .type UART0_ERR_IRQHandler, %function
UART0_ERR_IRQHandler:
    ldr   r0,=UART0_ERR_DriverIRQHandler
    bx    r0
    .size UART0_ERR_IRQHandler, . - UART0_ERR_IRQHandler

    .align 1
    .thumb_func
    .weak UART1_RX_TX_IRQHandler
    .type UART1_RX_TX_IRQHandler, %function
UART1_RX_TX_IRQHandler:
    ldr   r0,=UART1_RX_TX_DriverIRQHandler
    bx    r0
    .size UART1_RX_TX_IRQHandler, . - UART1_RX_TX_IRQHandler

    .align 1
    .thumb_func
    .weak UART1_ERR_IRQHandler
    .type UART1_ERR_IRQHandler, %function
UART1_ERR_IRQHandler:
    ldr   r0,=UART1_ERR_DriverIRQHandler
    bx    r0
    .size UART1_ERR_IRQHandler, . - UART1_ERR_IRQHandler

    .align 1
    .thumb_func
    .weak UART2_RX_TX_IRQHandler
    .type UART2_RX_TX_IRQHandler, %function
UART2_RX_TX_IRQHandler:
    ldr   r0,=UART2_RX_TX_DriverIRQHandler
    bx    r0
    .size UART2_RX_TX_IRQHandler, . - UART2_RX_TX_IRQHandler

    .align 1
    .thumb_func
    .weak UART2_ERR_IRQHandler
    .type UART2_ERR_IRQHandler, %function
UART2_ERR_IRQHandler:
    ldr   r0,=UART2_ERR_DriverIRQHandler
    bx    r0
    .size UART2_ERR_IRQHandler, . - UART2_ERR_IRQHandler


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
    def_irq_handler    DMA0_DriverIRQHandler
    def_irq_handler    DMA1_DriverIRQHandler
    def_irq_handler    DMA2_DriverIRQHandler
    def_irq_handler    DMA3_DriverIRQHandler
    def_irq_handler    DMA4_DriverIRQHandler
    def_irq_handler    DMA5_DriverIRQHandler
    def_irq_handler    DMA6_DriverIRQHandler
    def_irq_handler    DMA7_DriverIRQHandler
    def_irq_handler    DMA8_DriverIRQHandler
    def_irq_handler    DMA9_DriverIRQHandler
    def_irq_handler    DMA10_DriverIRQHandler
    def_irq_handler    DMA11_DriverIRQHandler
    def_irq_handler    DMA12_DriverIRQHandler
    def_irq_handler    DMA13_DriverIRQHandler
    def_irq_handler    DMA14_DriverIRQHandler
    def_irq_handler    DMA15_DriverIRQHandler
    def_irq_handler    DMA_Error_DriverIRQHandler
    def_irq_handler    MCM_IRQHandler
    def_irq_handler    FTFL_IRQHandler
    def_irq_handler    FTFL_Collision_IRQHandler
    def_irq_handler    PMC_IRQHandler
    def_irq_handler    LLWU_IRQHandler
    def_irq_handler    WDOG_EWM_IRQHandler
    def_irq_handler    RNG_IRQHandler
    def_irq_handler    I2C0_DriverIRQHandler
    def_irq_handler    I2C1_DriverIRQHandler
    def_irq_handler    SPI0_DriverIRQHandler
    def_irq_handler    SPI1_DriverIRQHandler
    def_irq_handler    I2S0_Tx_DriverIRQHandler
    def_irq_handler    I2S0_Rx_DriverIRQHandler
    def_irq_handler    Reserved46_IRQHandler
    def_irq_handler    UART0_RX_TX_DriverIRQHandler
    def_irq_handler    UART0_ERR_DriverIRQHandler
    def_irq_handler    UART1_RX_TX_DriverIRQHandler
    def_irq_handler    UART1_ERR_DriverIRQHandler
    def_irq_handler    UART2_RX_TX_DriverIRQHandler
    def_irq_handler    UART2_ERR_DriverIRQHandler
    def_irq_handler    Reserved53_IRQHandler
    def_irq_handler    Reserved54_IRQHandler
    def_irq_handler    ADC0_IRQHandler
    def_irq_handler    CMP0_IRQHandler
    def_irq_handler    CMP1_IRQHandler
    def_irq_handler    FTM0_IRQHandler
    def_irq_handler    FTM1_IRQHandler
    def_irq_handler    FTM2_IRQHandler
    def_irq_handler    CMT_IRQHandler
    def_irq_handler    RTC_IRQHandler
    def_irq_handler    RTC_Seconds_IRQHandler
    def_irq_handler    PIT0_IRQHandler
    def_irq_handler    PIT1_IRQHandler
    def_irq_handler    PIT2_IRQHandler
    def_irq_handler    PIT3_IRQHandler
    def_irq_handler    PDB0_IRQHandler
    def_irq_handler    USB0_IRQHandler
    def_irq_handler    USBDCD_IRQHandler
    def_irq_handler    Reserved71_IRQHandler
    def_irq_handler    Reserved72_IRQHandler
    def_irq_handler    MCG_IRQHandler
    def_irq_handler    LPTMR0_IRQHandler
    def_irq_handler    PORTA_IRQHandler
    def_irq_handler    PORTB_IRQHandler
    def_irq_handler    PORTC_IRQHandler
    def_irq_handler    PORTD_IRQHandler
    def_irq_handler    PORTE_IRQHandler
    def_irq_handler    SWI_IRQHandler

    .end
