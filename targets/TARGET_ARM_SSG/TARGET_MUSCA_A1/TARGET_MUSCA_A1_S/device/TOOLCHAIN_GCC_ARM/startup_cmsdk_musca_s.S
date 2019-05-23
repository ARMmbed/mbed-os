;/*
; * Copyright (c) 2009-2019 ARM Limited
; *
; * SPDX-License-Identifier: Apache-2.0
; *
; * Licensed under the Apache License, Version 2.0 (the "License");
; * you may not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; *     http://www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an "AS IS" BASIS,
; * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; *
; *
; * This file is derivative of CMSIS V5.00 startup_ARMCM33.S
; */

    .syntax    unified
    .arch    armv8-m.main

    .section .vectors
    .align 2
    .globl    __Vectors
__Vectors:
    .long    Image$$ARM_LIB_STACK_MSP$$ZI$$Limit   /* Top of Stack */
    .long    Reset_Handler         /* Reset Handler */
    .long    NMI_Handler           /* NMI Handler */
    .long    HardFault_Handler     /* Hard Fault Handler */
    .long    MemManage_Handler     /* MPU Fault Handler */
    .long    BusFault_Handler      /* Bus Fault Handler */
    .long    UsageFault_Handler    /* Usage Fault Handler */
    .long    SecureFault_Handler   /* Secure Fault Handler */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    SVC_Handler           /* SVCall Handler */
    .long    DebugMon_Handler      /* Debug Monitor Handler */
    .long    0                     /* Reserved */
    .long    PendSV_Handler        /* PendSV Handler */
    .long    SysTick_Handler       /* SysTick Handler */

    /* Core interrupts */
    .long    NS_WATCHDOG_RESET_IRQHandler    /*  0: Non-Secure Watchdog Reset Request Interrupt */
    .long    NS_WATCHDOG_IRQHandler          /*  1: Non-Secure Watchdog Interrupt */
    .long    S32K_TIMER_IRQHandler           /*  2: S32K Timer Interrupt */
    .long    TIMER0_IRQHandler               /*  3: CMSDK Timer 0 Interrupt */
    .long    TIMER1_IRQHandler               /*  4: CMSDK Timer 1 Interrupt */
    .long    DUALTIMER_IRQHandler            /*  5: CMSDK Dual Timer Interrupt */
    .long    MHU0_IRQHandler                 /*  6: Message Handling Unit 0 Interrupt */
    .long    MHU1_IRQHandler                 /*  7: Message Handling Unit 1 Interrupt */
    .long    CRYPTOCELL_IRQHandler           /*  8: CryptoCell-312 Interrupt */
    .long    MPC_Handler                     /*  9: Secure Combined MPC Interrupt */
    .long    PPC_Handler                     /* 10: Secure Combined PPC Interrupt */
    .long    S_MSC_COMBINED_IRQHandler       /* 11: Secure Combined MSC Interrupt */
    .long    S_BRIDGE_ERR_IRQHandler         /* 12: Secure Bridge Error Combined Interrupt */
    .long    I_CACHE_INV_ERR_IRQHandler      /* 13: Intsruction Cache Invalidation Interrupt */
    .long    0                               /* 14: Reserved */
    .long    SYS_PPU_IRQHandler              /* 15: System PPU Interrupt */
    .long    CPU0_PPU_IRQHandler             /* 16: CPU0 PPU Interrupt */
    .long    CPU1_PPU_IRQHandler             /* 17: CPU1 PPU Interrupt */
    .long    CPU0_DGB_PPU_IRQHandler         /* 18: CPU0 Debug PPU Interrupt */
    .long    CPU1_DGB_PPU_IRQHandler         /* 19: CPU1 Debug PPU Interrupt */
    .long    CRYPTOCELL_PPU_IRQHandler       /* 20: CryptoCell PPU Interrupt */
    .long    0                               /* 21: Reserved */
    .long    RAM0_PPU_IRQHandler             /* 22: RAM 0 PPU Interrupt */
    .long    RAM1_PPU_IRQHandler             /* 23: RAM 1 PPU Interrupt */
    .long    RAM2_PPU_IRQHandler             /* 24: RAM 2 PPU Interrupt */
    .long    RAM3_PPU_IRQHandler             /* 25: RAM 3 PPU Interrupt */
    .long    DEBUG_PPU_IRQHandler            /* 26: Debug PPU Interrupt */
    .long    0                               /* 27: Reserved */
    .long    CPU0_CTI_IRQHandler             /* 28: CPU0 CTI Interrupt */
    .long    CPU1_CTI_IRQHandler             /* 29: CPU1 CTI Interrupt */
    .long    0                               /* 30: Reserved */
    .long    0                               /* 31: Reserved */

    /* External interrupts */
    .long    0                               /* 32: Reserved */
    .long    GpTimer_IRQHandler              /* 33: General Purpose Timer */
    .long    I2C0_IRQHandler                 /* 34: I2C0 */
    .long    I2C1_IRQHandler                 /* 35: I2C1 */
    .long    I2S_IRQHandler                  /* 36: I2S */
    .long    SPI_IRQHandler                  /* 37: SPI */
    .long    QSPI_IRQHandler                 /* 38: QSPI */
    .long    UARTRX0_Handler                 /* 39: UART0 receive FIFO interrupt */
    .long    UARTTX0_Handler                 /* 40: UART0 transmit FIFO interrupt */
    .long    UART0_RxTimeout_IRQHandler      /* 41: UART0 receive timeout interrupt */
    .long    UART0_ModemStatus_IRQHandler    /* 42: UART0 modem status interrupt */
    .long    UART0_Error_IRQHandler          /* 43: UART0 error interrupt */
    .long    UART0_IRQHandler                /* 44: UART0 interrupt */
    .long    UARTRX1_Handler                 /* 45: UART0 receive FIFO interrupt */
    .long    UARTTX1_Handler                 /* 46: UART0 transmit FIFO interrupt */
    .long    UART1_RxTimeout_IRQHandler      /* 47: UART0 receive timeout interrupt */
    .long    UART1_ModemStatus_IRQHandler    /* 48: UART0 modem status interrupt */
    .long    UART1_Error_IRQHandler          /* 49: UART0 error interrupt */
    .long    UART1_IRQHandler                /* 50: UART0 interrupt */
    .long    GPIO_0_IRQHandler               /* 51: GPIO 0 interrupt */
    .long    GPIO_1_IRQHandler               /* 52: GPIO 1 interrupt */
    .long    GPIO_2_IRQHandler               /* 53: GPIO 2 interrupt */
    .long    GPIO_3_IRQHandler               /* 54: GPIO 3 interrupt */
    .long    GPIO_4_IRQHandler               /* 55: GPIO 4 interrupt */
    .long    GPIO_5_IRQHandler               /* 56: GPIO 5 interrupt */
    .long    GPIO_6_IRQHandler               /* 57: GPIO 6 interrupt */
    .long    GPIO_7_IRQHandler               /* 58: GPIO 7 interrupt */
    .long    GPIO_8_IRQHandler               /* 59: GPIO 8 interrupt */
    .long    GPIO_9_IRQHandler               /* 60: GPIO 9 interrupt */
    .long    GPIO_10_IRQHandler              /* 61: GPIO 10 interrupt */
    .long    GPIO_11_IRQHandler              /* 62: GPIO 11 interrupt */
    .long    GPIO_12_IRQHandler              /* 63: GPIO 12 interrupt */
    .long    GPIO_13_IRQHandler              /* 64: GPIO 13 interrupt */
    .long    GPIO_14_IRQHandler              /* 65: GPIO 14 interrupt */
    .long    GPIO_15_IRQHandler              /* 66: GPIO 15 interrupt */
    .long    Combined_IRQHandler             /* 67: Combined interrupt */
    .long    PVT_IRQHandler                  /* 68: PVT sensor interrupt */
    .long    0                               /* 69: Reserved */
    .long    PWM_0_IRQHandler                /* 70: PWM0 interrupt */
    .long    RTC_IRQHandler                  /* 71: RTC interrupt */
    .long    GpTimer0_IRQHandler             /* 72: General Purpose Timer0 */
    .long    GpTimer1_IRQHandler             /* 73: General Purpose Timer1 */
    .long    PWM_1_IRQHandler                /* 74: PWM1 interrupt */
    .long    PWM_2_IRQHandler                /* 75: PWM2 interrupt */
    .long    IOMUX_IRQHandler                /* 76: IOMUX interrupt */

    .size    __Vectors, . - __Vectors

    .text
    .thumb
    .thumb_func
    .align    2
    .globl    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
/*  Firstly it copies data from read only memory to RAM. There are two schemes
 *  to copy. One can copy more than one sections. Another can only copy
 *  one section.  The former scheme needs more instructions and read-only
 *  data to implement than the latter.
 *  Macro __STARTUP_COPY_MULTIPLE is used to choose between two schemes.  */

#ifdef __STARTUP_COPY_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of triplets, each of which specify:
 *    offset 0: LMA of start of a section to copy from
 *    offset 4: VMA of start of a section to copy to
 *    offset 8: size of the section to copy. Must be multiply of 4
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
    ldr    r4, =__copy_table_start__
    ldr    r5, =__copy_table_end__

.L_loop0:
    cmp    r4, r5
    bge    .L_loop0_done
    ldr    r1, [r4]
    ldr    r2, [r4, #4]
    ldr    r3, [r4, #8]

.L_loop0_0:
    subs    r3, #4
    ittt    ge
    ldrge    r0, [r1, r3]
    strge    r0, [r2, r3]
    bge    .L_loop0_0

    adds    r4, #12
    b    .L_loop0

.L_loop0_done:
#else
/*  Single section scheme.
 *
 *  The ranges of copy from/to are specified by following symbols
 *    __etext: LMA of start of the section to copy from. Usually end of text
 *    __data_start__: VMA of start of the section to copy to
 *    __data_end__: VMA of end of the section to copy to
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

.L_loop1:
    cmp    r2, r3
    ittt    lt
    ldrlt    r0, [r1], #4
    strlt    r0, [r2], #4
    blt    .L_loop1
#endif /*__STARTUP_COPY_MULTIPLE */

/*  This part of work usually is done in C library startup code. Otherwise,
 *  define this macro to enable it in this startup.
 *
 *  There are two schemes too. One can clear multiple BSS sections. Another
 *  can only clear one section. The former is more size expensive than the
 *  latter.
 *
 *  Define macro __STARTUP_CLEAR_BSS_MULTIPLE to choose the former.
 *  Otherwise efine macro __STARTUP_CLEAR_BSS to choose the later.
 */
#ifdef __STARTUP_CLEAR_BSS_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of tuples specifying:
 *    offset 0: Start of a BSS section
 *    offset 4: Size of this BSS section. Must be multiply of 4
 */
    ldr    r3, =__zero_table_start__
    ldr    r4, =__zero_table_end__

.L_loop2:
    cmp    r3, r4
    bge    .L_loop2_done
    ldr    r1, [r3]
    ldr    r2, [r3, #4]
    movs    r0, 0

.L_loop2_0:
    subs    r2, #4
    itt    ge
    strge    r0, [r1, r2]
    bge    .L_loop2_0

    adds    r3, #8
    b    .L_loop2
.L_loop2_done:
#elif defined (__STARTUP_CLEAR_BSS)
/*  Single BSS section scheme.
 *
 *  The BSS section is specified by following symbols
 *    __bss_start__: start of the BSS section.
 *    __bss_end__: end of the BSS section.
 *
 *  Both addresses must be aligned to 4 bytes boundary.
 */
    ldr    r1, =__bss_start__
    ldr    r2, =__bss_end__

    movs    r0, 0
.L_loop3:
    cmp    r1, r2
    itt    lt
    strlt    r0, [r1], #4
    blt    .L_loop3
#endif /* __STARTUP_CLEAR_BSS_MULTIPLE || __STARTUP_CLEAR_BSS */

    cpsid   i              /* Disable IRQs */
    bl    SystemInit

    mrs     r0, control    /* Get control value */
    orr     r0, r0, #2     /* Select switch to PSP */
    msr     control, r0
    ldr     r0, =Image$$ARM_LIB_STACK$$ZI$$Limit
    msr     psp, r0

#ifndef __START
#define __START _start
#endif
    bl    __START

    .pool
    .size    Reset_Handler, . - Reset_Handler


/*  Macro to define default handlers. */
    .macro    def_irq_handler    handler_name
    .align    1
    .thumb_func
    .weak    \handler_name
    \handler_name:
    b        \handler_name
    .endm

    def_irq_handler             NMI_Handler
    def_irq_handler             HardFault_Handler
    def_irq_handler             MemManage_Handler
    def_irq_handler             BusFault_Handler
    def_irq_handler             UsageFault_Handler
    def_irq_handler             SecureFault_Handler
    def_irq_handler             SVC_Handler
    def_irq_handler             DebugMon_Handler
    def_irq_handler             PendSV_Handler
    def_irq_handler             SysTick_Handler

    /* Core interrupts */
    def_irq_handler     NS_WATCHDOG_RESET_IRQHandler    /*  0: Non-Secure Watchdog Reset Request Interrupt */
    def_irq_handler     NS_WATCHDOG_IRQHandler          /*  1: Non-Secure Watchdog Interrupt */
    def_irq_handler     S32K_TIMER_IRQHandler           /*  2: S32K Timer Interrupt */
    def_irq_handler     TIMER0_IRQHandler               /*  3: CMSDK Timer 0 Interrupt */
    def_irq_handler     TIMER1_IRQHandler               /*  4: CMSDK Timer 1 Interrupt */
    def_irq_handler     DUALTIMER_IRQHandler            /*  5: CMSDK Dual Timer Interrupt */
    def_irq_handler     MHU0_IRQHandler                 /*  6: Message Handling Unit 0 Interrupt */
    def_irq_handler     MHU1_IRQHandler                 /*  7: Message Handling Unit 1 Interrupt */
    def_irq_handler     CRYPTOCELL_IRQHandler           /*  8: CryptoCell-312 Interrupt */
    def_irq_handler     MPC_Handler                     /*  9: Secure Combined MPC Interrupt */
    def_irq_handler     PPC_Handler                     /* 10: Secure Combined PPC Interrupt */
    def_irq_handler     S_MSC_COMBINED_IRQHandler       /* 11: Secure Combined MSC Interrupt */
    def_irq_handler     S_BRIDGE_ERR_IRQHandler         /* 12: Secure Bridge Error Combined Interrupt */
    def_irq_handler     I_CACHE_INV_ERR_IRQHandler      /* 13: Intsruction Cache Invalidation Interrupt */
    def_irq_handler     SYS_PPU_IRQHandler              /* 15: System PPU Interrupt */
    def_irq_handler     CPU0_PPU_IRQHandler             /* 16: CPU0 PPU Interrupt */
    def_irq_handler     CPU1_PPU_IRQHandler             /* 17: CPU1 PPU Interrupt */
    def_irq_handler     CPU0_DGB_PPU_IRQHandler         /* 18: CPU0 Debug PPU Interrupt */
    def_irq_handler     CPU1_DGB_PPU_IRQHandler         /* 19: CPU1 Debug PPU Interrupt */
    def_irq_handler     CRYPTOCELL_PPU_IRQHandler       /* 20: CryptoCell PPU Interrupt */
    def_irq_handler     RAM0_PPU_IRQHandler             /* 22: RAM 0 PPU Interrupt */
    def_irq_handler     RAM1_PPU_IRQHandler             /* 23: RAM 1 PPU Interrupt */
    def_irq_handler     RAM2_PPU_IRQHandler             /* 24: RAM 2 PPU Interrupt */
    def_irq_handler     RAM3_PPU_IRQHandler             /* 25: RAM 3 PPU Interrupt */
    def_irq_handler     DEBUG_PPU_IRQHandler            /* 26: Debug PPU Interrupt */
    def_irq_handler     CPU0_CTI_IRQHandler             /* 28: CPU0 CTI Interrupt */
    def_irq_handler     CPU1_CTI_IRQHandler             /* 29: CPU1 CTI Interrupt */

    /* External interrupts */
    def_irq_handler     GpTimer_IRQHandler              /* 33: General Purpose Timer */
    def_irq_handler     I2C0_IRQHandler                 /* 34: I2C0 */
    def_irq_handler     I2C1_IRQHandler                 /* 35: I2C1 */
    def_irq_handler     I2S_IRQHandler                  /* 36: I2S */
    def_irq_handler     SPI_IRQHandler                  /* 37: SPI */
    def_irq_handler     QSPI_IRQHandler                 /* 38: QSPI */
    def_irq_handler     UARTRX0_Handler                 /* 39: UART0 receive FIFO interrupt */
    def_irq_handler     UARTTX0_Handler                 /* 40: UART0 transmit FIFO interrupt */
    def_irq_handler     UART0_RxTimeout_IRQHandler      /* 41: UART0 receive timeout interrupt */
    def_irq_handler     UART0_ModemStatus_IRQHandler    /* 42: UART0 modem status interrupt */
    def_irq_handler     UART0_Error_IRQHandler          /* 43: UART0 error interrupt */
    def_irq_handler     UART0_IRQHandler                /* 44: UART0 interrupt */
    def_irq_handler     UARTRX1_Handler                 /* 45: UART0 receive FIFO interrupt */
    def_irq_handler     UARTTX1_Handler                 /* 46: UART0 transmit FIFO interrupt */
    def_irq_handler     UART1_RxTimeout_IRQHandler      /* 47: UART0 receive timeout interrupt */
    def_irq_handler     UART1_ModemStatus_IRQHandler    /* 48: UART0 modem status interrupt */
    def_irq_handler     UART1_Error_IRQHandler          /* 49: UART0 error interrupt */
    def_irq_handler     UART1_IRQHandler                /* 50: UART0 interrupt */
    def_irq_handler     GPIO_0_IRQHandler               /* 51: GPIO 0 interrupt */
    def_irq_handler     GPIO_1_IRQHandler               /* 52: GPIO 1 interrupt */
    def_irq_handler     GPIO_2_IRQHandler               /* 53: GPIO 2 interrupt */
    def_irq_handler     GPIO_3_IRQHandler               /* 54: GPIO 3 interrupt */
    def_irq_handler     GPIO_4_IRQHandler               /* 55: GPIO 4 interrupt */
    def_irq_handler     GPIO_5_IRQHandler               /* 56: GPIO 5 interrupt */
    def_irq_handler     GPIO_6_IRQHandler               /* 57: GPIO 6 interrupt */
    def_irq_handler     GPIO_7_IRQHandler               /* 58: GPIO 7 interrupt */
    def_irq_handler     GPIO_8_IRQHandler               /* 59: GPIO 8 interrupt */
    def_irq_handler     GPIO_9_IRQHandler               /* 60: GPIO 9 interrupt */
    def_irq_handler     GPIO_10_IRQHandler              /* 61: GPIO 10 interrupt */
    def_irq_handler     GPIO_11_IRQHandler              /* 62: GPIO 11 interrupt */
    def_irq_handler     GPIO_12_IRQHandler              /* 63: GPIO 12 interrupt */
    def_irq_handler     GPIO_13_IRQHandler              /* 64: GPIO 13 interrupt */
    def_irq_handler     GPIO_14_IRQHandler              /* 65: GPIO 14 interrupt */
    def_irq_handler     GPIO_15_IRQHandler              /* 66: GPIO 15 interrupt */
    def_irq_handler     Combined_IRQHandler             /* 67: Combined interrupt */
    def_irq_handler     PVT_IRQHandler                  /* 68: PVT sensor interrupt */
    def_irq_handler     PWM_0_IRQHandler                /* 70: PWM0 interrupt */
    def_irq_handler     RTC_IRQHandler                  /* 71: RTC interrupt */
    def_irq_handler     GpTimer0_IRQHandler             /* 72: General Purpose Timer0 */
    def_irq_handler     GpTimer1_IRQHandler             /* 73: General Purpose Timer1 */
    def_irq_handler     PWM_1_IRQHandler                /* 74: PWM1 interrupt */
    def_irq_handler     PWM_2_IRQHandler                /* 75: PWM2 interrupt */
    def_irq_handler     IOMUX_IRQHandler                /* 76: IOMUX interrupt */

    .end
