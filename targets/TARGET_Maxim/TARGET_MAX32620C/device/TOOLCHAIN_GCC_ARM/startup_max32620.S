/*******************************************************************************
 * Copyright (C) 2018 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

    .syntax unified
    .arch armv7-m

    .section .stack
    .align 3
#ifdef __STACK_SIZE
    .equ    Stack_Size, __STACK_SIZE
#else
    .equ    Stack_Size, 0x00001000
#endif
    .globl    __StackTop
    .globl    __StackLimit
__StackLimit:
    .space    Stack_Size
    .size   __StackLimit, . - __StackLimit
__StackTop:
    .size   __StackTop, . - __StackTop

    .section .heap
    .align 3
#ifdef __HEAP_SIZE
    .equ    Heap_Size, __HEAP_SIZE
#else
    .equ    Heap_Size, 0x00004000
#endif
    .globl  __HeapBase
    .globl  __HeapLimit
__HeapBase:
    .space  Heap_Size
    .size   __HeapBase, . - __HeapBase
__HeapLimit:
    .size __HeapLimit, . - __HeapLimit

    .section .isr_vector
    .align 2
    .globl  __isr_vector
__isr_vector:
    .long    __StackTop            /* Top of Stack */
    .long    Reset_Handler         /* Reset Handler */
    .long    NMI_Handler           /* NMI Handler */
    .long    HardFault_Handler     /* Hard Fault Handler */
    .long    MemManage_Handler     /* MPU Fault Handler */
    .long    BusFault_Handler      /* Bus Fault Handler */
    .long    UsageFault_Handler    /* Usage Fault Handler */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    SVC_Handler           /* SVCall Handler */
    .long    DebugMon_Handler      /* Debug Monitor Handler */
    .long    0                     /* Reserved */
    .long    PendSV_Handler        /* PendSV Handler */
    .long    SysTick_Handler       /* SysTick Handler */

    /* External interrupts */
    .long    CLKMAN_IRQHandler     /* 16:00 CLKMAN */
    .long    PWRMAN_IRQHandler     /* 17:01 PWRMAN */
    .long    FLC_IRQHandler        /* 18:02 Flash Controller */
    .long    RTC0_IRQHandler       /* 19:03 RTC INT0 */
    .long    RTC1_IRQHandler       /* 20:04 RTC INT1 */
    .long    RTC2_IRQHandler       /* 21:05 RTC INT2 */
    .long    RTC3_IRQHandler       /* 22:06 RTC INT3 */
    .long    PMU_IRQHandler        /* 23:07 PMU */
    .long    USB_IRQHandler        /* 24:08 USB */
    .long    AES_IRQHandler        /* 25:09 AES */
    .long    MAA_IRQHandler        /* 26:10 MAA */
    .long    WDT0_IRQHandler       /* 27:11 WATCHDOG0 */
    .long    WDT0_P_IRQHandler     /* 28:12 WATCHDOG0 PRE-WINDOW */
    .long    WDT1_IRQHandler       /* 29:13 WATCHDOG1 */
    .long    WDT1_P_IRQHandler     /* 30:14 WATCHDOG1 PRE-WINDOW */
    .long    GPIO_P0_IRQHandler    /* 31:15 GPIO Port 0  */
    .long    GPIO_P1_IRQHandler    /* 32:16 GPIO Port 1  */
    .long    GPIO_P2_IRQHandler    /* 33:17 GPIO Port 2  */
    .long    GPIO_P3_IRQHandler    /* 34:18 GPIO Port 3  */
    .long    GPIO_P4_IRQHandler    /* 35:19 GPIO Port 4  */
    .long    GPIO_P5_IRQHandler    /* 36:20 GPIO Port 5  */
    .long    GPIO_P6_IRQHandler    /* 37:21 GPIO Port 6  */
    .long    TMR0_IRQHandler       /* 38:22 Timer32-0 */
    .long    TMR16_0_IRQHandler    /* 39:23 Timer16-s0 */
    .long    TMR1_IRQHandler       /* 40:24 Timer32-1 */
    .long    TMR16_1_IRQHandler    /* 41:25 Timer16-s1 */
    .long    TMR2_IRQHandler       /* 42:26 Timer32-2 */
    .long    TMR16_2_IRQHandler    /* 43:27 Timer16-s2 */
    .long    TMR3_IRQHandler       /* 44:28 Timer32-3 */
    .long    TMR16_3_IRQHandler    /* 45:29 Timer16-s3 */
    .long    TMR4_IRQHandler       /* 46:30 Timer32-4 */
    .long    TMR16_4_IRQHandler    /* 47:31 Timer16-s4 */
    .long    TMR5_IRQHandler       /* 48:32 Timer32-5 */
    .long    TMR16_5_IRQHandler    /* 49:33 Timer16-s5 */
    .long    UART0_IRQHandler      /* 50:34 UART0 */
    .long    UART1_IRQHandler      /* 51:35 UART1 */
    .long    UART2_IRQHandler      /* 52:36 UART2 */
    .long    UART3_IRQHandler      /* 53:37 UART3 */
    .long    PT_IRQHandler         /* 54:38 PT */
    .long    I2CM0_IRQHandler      /* 55:39 I2C Master 0 */
    .long    I2CM1_IRQHandler      /* 56:40 I2C Master 1 */
    .long    I2CM2_IRQHandler      /* 57:41 I2C Master 2 */
    .long    I2CS_IRQHandler       /* 58:42 I2C Slave */
    .long    SPI0_IRQHandler       /* 59:43 SPI0 */
    .long    SPI1_IRQHandler       /* 60:44 SPI1 */
    .long    SPI2_IRQHandler       /* 61:45 SPI2 */
    .long    SPIB_IRQHandler       /* 62:46 SPI Bridge */
    .long    OWM_IRQHandler        /* 63:47 1-Wire Master */
    .long    AFE_IRQHandler        /* 64:48 AFE */
    .long    SPIS_IRQHandler       /* 65:49 SPI Slave */

    .text
    .thumb
    .thumb_func
    .align 2
    .globl  Reset_Handler
    .type   Reset_Handler, %function
Reset_Handler:
    ldr     r0, =__StackTop
    mov     sp, r0

    /* PreInit runs before any RAM initialization. Example usage: DDR setup, etc. */
    ldr     r0, =PreInit
    blx     r0
    cbnz    r0, .SKIPRAMINIT

/*     Loop to copy data from read only memory to RAM. The ranges
 *      of copy from/to are specified by following symbols evaluated in
 *      linker script.
 *      __etext: End of code section, i.e., begin of data sections to copy from.
 *      __data_start__/__data_end__: RAM address range that data should be
 *      copied to. Both must be aligned to 4 bytes boundary.
 */
    ldr     r1, =__etext
    ldr     r2, =__data_start__
    ldr     r3, =__data_end__

    subs    r3, r2
    ble     .LC1
.LC0:
    subs    r3, #4
    ldr     r0, [r1, r3]
    str     r0, [r2, r3]
    bgt     .LC0
.LC1:

/*
 *     Loop to zero out BSS section, which uses following symbols
 *     in linker script:
 *      _bss  : start of BSS section. Must align to 4
 *      _ebss : end of BSS section. Must align to 4
 */
    ldr     r1, =__bss_start__
    ldr     r2, =__bss_end__

    movs    r0, 0
.LC2:
    cmp     r1, r2
    itt     lt
    strlt   r0, [r1], #4
    blt     .LC2

.SKIPRAMINIT:

    /* Perform system initialization after RAM initialization */
    ldr     r0, =SystemInit
    blx     r0

    /* This must be called to walk the constructor array for static C++ objects            */
    /* Note: The linker file must have .data symbols for __X_array_start and __X_array_end */
    /*        where X is {preinit, init, fini}                                             */
    ldr     r0, =_start
    blx     r0

.SPIN:
    /* Enter LP2 if main() ever returns. */
    wfi
    bl      .SPIN

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers
 */
    .macro    def_default_handler    handler_name
    .align 1
    .thumb_func
    .weak    \handler_name
    .type    \handler_name, %function
\handler_name :
    b    .
    .size    \handler_name, . - \handler_name
    .endm

    def_default_handler    NMI_Handler
    def_default_handler    HardFault_Handler
    def_default_handler    MemManage_Handler
    def_default_handler    BusFault_Handler
    def_default_handler    UsageFault_Handler
    def_default_handler    SVC_Handler
    def_default_handler    DebugMon_Handler
    def_default_handler    PendSV_Handler
    def_default_handler    SysTick_Handler
    def_default_handler    Default_Handler

    .macro    def_irq_default_handler    handler_name
    .weak     \handler_name
    .set      \handler_name, Default_Handler
    .endm

    def_irq_default_handler CLKMAN_IRQHandler         /* 16:00 CLKMAN */
    def_irq_default_handler PWRMAN_IRQHandler         /* 17:01 PWRMAN */
    def_irq_default_handler FLC_IRQHandler            /* 18:02 Flash Controller */
    def_irq_default_handler RTC0_IRQHandler           /* 19:03 RTC INT0 */
    def_irq_default_handler RTC1_IRQHandler           /* 20:04 RTC INT1 */
    def_irq_default_handler RTC2_IRQHandler           /* 21:05 RTC INT2 */
    def_irq_default_handler RTC3_IRQHandler           /* 22:06 RTC INT3 */
    def_irq_default_handler PMU_IRQHandler            /* 23:07 PMU */
    def_irq_default_handler USB_IRQHandler            /* 24:08 USB */
    def_irq_default_handler AES_IRQHandler            /* 25:09 AES */
    def_irq_default_handler MAA_IRQHandler            /* 26:10 MAA */
    def_irq_default_handler WDT0_IRQHandler           /* 27:11 WATCHDOG0 */
    def_irq_default_handler WDT0_P_IRQHandler         /* 28:12 WATCHDOG0 PRE-WINDOW */
    def_irq_default_handler WDT1_IRQHandler           /* 29:13 WATCHDOG1 */
    def_irq_default_handler WDT1_P_IRQHandler         /* 30:14 WATCHDOG1 PRE-WINDOW */
    def_irq_default_handler GPIO_P0_IRQHandler        /* 31:15 GPIO Port 0  */
    def_irq_default_handler GPIO_P1_IRQHandler        /* 32:16 GPIO Port 1  */
    def_irq_default_handler GPIO_P2_IRQHandler        /* 33:17 GPIO Port 2  */
    def_irq_default_handler GPIO_P3_IRQHandler        /* 34:18 GPIO Port 3  */
    def_irq_default_handler GPIO_P4_IRQHandler        /* 35:19 GPIO Port 4  */
    def_irq_default_handler GPIO_P5_IRQHandler        /* 36:20 GPIO Port 5  */
    def_irq_default_handler GPIO_P6_IRQHandler        /* 37:21 GPIO Port 6  */
    def_irq_default_handler TMR0_IRQHandler           /* 38:22 Timer32-0 */
    def_irq_default_handler TMR16_0_IRQHandler        /* 39:23 Timer16-s0 */
    def_irq_default_handler TMR1_IRQHandler           /* 40:24 Timer32-1 */
    def_irq_default_handler TMR16_1_IRQHandler        /* 41:25 Timer16-s1 */
    def_irq_default_handler TMR2_IRQHandler           /* 42:26 Timer32-2 */
    def_irq_default_handler TMR16_2_IRQHandler        /* 43:27 Timer16-s2 */
    def_irq_default_handler TMR3_IRQHandler           /* 44:28 Timer32-3 */
    def_irq_default_handler TMR16_3_IRQHandler        /* 45:29 Timer16-s3 */
    def_irq_default_handler TMR4_IRQHandler           /* 46:30 Timer32-4 */
    def_irq_default_handler TMR16_4_IRQHandler        /* 47:31 Timer16-s4 */
    def_irq_default_handler TMR5_IRQHandler           /* 48:32 Timer32-5 */
    def_irq_default_handler TMR16_5_IRQHandler        /* 49:33 Timer16-s5 */
    def_irq_default_handler PT_IRQHandler             /* 50:34 PT */
    def_irq_default_handler UART0_IRQHandler          /* 51:35 UART0 */
    def_irq_default_handler UART1_IRQHandler          /* 52:36 UART1 */
    def_irq_default_handler UART2_IRQHandler          /* 53:37 UART0 */
    def_irq_default_handler UART3_IRQHandler          /* 54:38 UART1 */
    def_irq_default_handler I2CM0_IRQHandler          /* 55:39 I2C Master 0 */
    def_irq_default_handler I2CM1_IRQHandler          /* 56:40 I2C Master 1 */
    def_irq_default_handler I2CM2_IRQHandler          /* 57:41 I2C Master 2 */
    def_irq_default_handler I2CS_IRQHandler           /* 58:42 I2C Slave */
    def_irq_default_handler SPI0_IRQHandler           /* 59:43 SPI0 */
    def_irq_default_handler SPI1_IRQHandler           /* 60:44 SPI1 */
    def_irq_default_handler SPI2_IRQHandler           /* 61:45 SPI2 */
    def_irq_default_handler SPIB_IRQHandler           /* 62:46 SPI Bridge */
    def_irq_default_handler OWM_IRQHandler            /* 63:47 SPI Bridge */
    def_irq_default_handler AFE_IRQHandler            /* 64:48 AFE */
    def_irq_default_handler SPIS_IRQHandler           /* 65:49 SPI Slave */

    .end
