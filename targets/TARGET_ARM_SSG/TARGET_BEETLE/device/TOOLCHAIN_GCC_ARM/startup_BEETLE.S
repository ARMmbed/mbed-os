/*
 * BEETLE CMSIS Library
 */
/*
 * Copyright (c) 2009-2016 ARM Limited. All rights reserved.
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
 */
/*
 * This file is derivative of CMSIS V5.00 startup_ARMCM3.S
 */
    .syntax unified
    .arch armv7-m

    .section .vector_table,"a",%progbits
    .align 2
    .globl __isr_vector
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
    .long    UART0_Handler       /* 0:UART 0 RX and TX Combined Interrupt        */
    .long    Spare_Handler       /* 1:Undefined                                  */
    .long    UART1_Handler       /* 2:UART 1 RX and TX Combined Interrupt        */
    .long    I2C0_Handler        /* 3:I2C 0 Interrupt                            */
    .long    I2C1_Handler        /* 4:I2C 1 Interrupt                            */
    .long    RTC_Handler         /* 5:RTC Interrupt                              */
    .long    PORT0_Handler       /* 6:GPIO Port 0 combined Interrupt             */
    .long    PORT1_ALL_Handler   /* 7:GPIO Port 1 combined Interrupt             */
    .long    TIMER0_Handler      /* 8:TIMER 0 Interrupt                          */
    .long    TIMER1_Handler      /* 9:TIMER 1 Interrupt                          */
    .long    DUALTIMER_Handler   /* 10:Dual Timer Interrupt                      */
    .long    SPI0_Handler        /* 11:SPI 0 Interrupt                           */
    .long    UARTOVF_Handler     /* 12:UART 0,1,2 Overflow Interrupt             */
    .long    SPI1_Handler        /* 13:SPI 1 Interrupt                           */
    .long    QSPI_Handler        /* 14:QUAD SPI Interrupt                        */
    .long    DMA_Handler         /* 15:Touch Screen Interrupt                    */
    .long    PORT0_0_Handler     /* 16:All P0 and P1I/O pins used as irq source  */
    .long    PORT0_1_Handler     /* 17:There are 16 pins in total                */
    .long    PORT0_2_Handler     /* 18:                                          */
    .long    PORT0_3_Handler     /* 19:                                          */
    .long    PORT0_4_Handler     /* 20:                                          */
    .long    PORT0_5_Handler     /* 21:                                          */
    .long    PORT0_6_Handler     /* 22:                                          */
    .long    PORT0_7_Handler     /* 23:                                          */
    .long    PORT0_8_Handler     /* 24:                                          */
    .long    PORT0_9_Handler     /* 25:                                          */
    .long    PORT0_10_Handler    /* 26:                                          */
    .long    PORT0_11_Handler    /* 27:                                          */
    .long    PORT0_12_Handler    /* 28:                                          */
    .long    PORT0_13_Handler    /* 29:                                          */
    .long    PORT0_14_Handler    /* 30:                                          */
    .long    PORT0_15_Handler    /* 31:                                          */
    .long    SysError_Handler    /* 32: System Error (Flash Cache)               */
    .long    EFLASH_Handler      /* 33: Embedded Flash                           */
    .long    LLCC_TXCMD_EMPTY_Handler        /* 34: LLCC_TXCMDIRQ                */
    .long    LLCC_TXEVT_EMPTY_Handler        /* 35: LLCC_TXEVTIRQ                */
    .long    LLCC_TXDMAH_DONE_Handler        /* 36: LLCC_TXDMA0IRQ               */
    .long    LLCC_TXDMAL_DONE_Handler        /* 37: LLCC_TXDMA1IRQ               */
    .long    LLCC_RXCMD_VALID_Handler        /* 38: LLCC_RXCMDIRQ                */
    .long    LLCC_RXEVT_VALID_Handler        /* 39: LLCC_RXEVTIRQ                */
    .long    LLCC_RXDMAH_DONE_Handler        /* 40: LLCC_RXDMA0IRQ               */
    .long    LLCC_RXDMAL_DONE_Handler        /* 41: LLCC_RXDMA1IRQ               */
    .long    PORT2_COMB_Handler              /* 42: GPIO 2                       */
    .long    PORT3_COMB_Handler              /* 43: GPIO 3                       */
    .long    TRNG_Handler                    /* 44: TRNG                         */

    .size    __isr_vector, . - __isr_vector

    .section .text.Reset_Handler
    .thumb
    .thumb_func
    .align  2
    .globl   Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
    ldr    r0, =SystemInit
    blx    r0
/* The call to uvisor_init() happens independently of uVisor being enabled or
* not, so it is conditionally compiled only based on FEATURE_UVISOR. */
#if defined(FEATURE_UVISOR) && defined(TARGET_UVISOR_SUPPORTED)
    /* Call uvisor_init() */
    ldr   r0, =uvisor_init
    blx   r0
#endif /* FEATURE_UVISOR && TARGET_UVISOR_SUPPORTED */
/*
 * Loop to copy data from read only memory to RAM. The ranges
 * of copy from/to are specified by following symbols evaluated in
 * linker script.
 * _etext: End of code section, i.e., begin of data sections to copy from.
 * __data_start__/__data_end__: RAM address range that data should be
 * copied to. Both must be aligned to 4 bytes boundary.
 */

    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

    subs   r3, r2
    ble    .Lflash_to_ram_loop_end

    movs    r4, 0
.Lflash_to_ram_loop:
    ldr    r0, [r1,r4]
    str    r0, [r2,r4]
    adds   r4, 4
    cmp    r4, r3
    blt    .Lflash_to_ram_loop
.Lflash_to_ram_loop_end:

/* Initialize .bss */
init_bss:
        ldr   r1, =__bss_start__
        ldr   r2, =__bss_end__
        ldr   r3, =bss_size

        cmp   r3, #0
        beq   system_startup

        mov   r4, #0
zero:
        strb  r4, [r1], #1
        subs  r3, r3, #1
        bne   zero

system_startup:
    ldr    r0, =SystemInit
    blx    r0
    ldr    r0, =_start
    bx    r0
    .pool
    .size Reset_Handler, . - Reset_Handler

    .text
/*
 * Macro to define default handlers. Default handler
 * will be weak symbol and just dead loops. They can be
 * overwritten by other handlers
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

    /* External interrupts */
    def_irq_default_handler     UART0_Handler       /* 0:UART 0 RX and TX Combined Interrupt        */
    def_irq_default_handler     Spare_Handler       /* 1:Undefined                                  */
    def_irq_default_handler     UART1_Handler       /* 2:UART 1 RX and TX Combined Interrupt        */
    def_irq_default_handler     I2C0_Handler        /* 3:I2C 0 Interrupt                            */
    def_irq_default_handler     I2C1_Handler        /* 4:I2C 1 Interrupt                            */
    def_irq_default_handler     RTC_Handler         /* 5:RTC Interrupt                              */
    def_irq_default_handler     PORT0_Handler       /* 6:GPIO Port 0 combined Interrupt             */
    def_irq_default_handler     PORT1_ALL_Handler   /* 7:GPIO Port 1 combined Interrupt             */
    def_irq_default_handler     TIMER0_Handler      /* 8:TIMER 0 Interrupt                          */
    def_irq_default_handler     TIMER1_Handler      /* 9:TIMER 1 Interrupt                          */
    def_irq_default_handler     DUALTIMER_Handler   /* 10:Dual Timer Interrupt                      */
    def_irq_default_handler     SPI0_Handler        /* 11:SPI 0 Interrupt                           */
    def_irq_default_handler     UARTOVF_Handler     /* 12:UART 0,1,2 Overflow Interrupt             */
    def_irq_default_handler     SPI1_Handler        /* 13:SPI 1 Interrupt                           */
    def_irq_default_handler     QSPI_Handler        /* 14:QUAD SPI Interrupt                        */
    def_irq_default_handler     DMA_Handler         /* 15:Touch Screen Interrupt                    */
    def_irq_default_handler     PORT0_0_Handler     /* 16:All P0 and P1I/O pins used as irq source  */
    def_irq_default_handler     PORT0_1_Handler     /* 17:There are 16 pins in total                */
    def_irq_default_handler     PORT0_2_Handler     /* 18:                                          */
    def_irq_default_handler     PORT0_3_Handler     /* 19:                                          */
    def_irq_default_handler     PORT0_4_Handler     /* 20:                                          */
    def_irq_default_handler     PORT0_5_Handler     /* 21:                                          */
    def_irq_default_handler     PORT0_6_Handler     /* 22:                                          */
    def_irq_default_handler     PORT0_7_Handler     /* 23:                                          */
    def_irq_default_handler     PORT0_8_Handler     /* 24:                                          */
    def_irq_default_handler     PORT0_9_Handler     /* 25:                                          */
    def_irq_default_handler     PORT0_10_Handler    /* 26:                                          */
    def_irq_default_handler     PORT0_11_Handler    /* 27:                                          */
    def_irq_default_handler     PORT0_12_Handler    /* 28:                                          */
    def_irq_default_handler     PORT0_13_Handler    /* 29:                                          */
    def_irq_default_handler     PORT0_14_Handler    /* 30:                                          */
    def_irq_default_handler     PORT0_15_Handler    /* 31:                                          */
    def_irq_default_handler     SysError_Handler    /* 32: System Error (Flash Cache)               */
    def_irq_default_handler     EFLASH_Handler      /* 33: Embedded Flash                           */
    def_irq_default_handler     LLCC_TXCMD_EMPTY_Handler        /* 34: LLCC_TXCMDIRQ                */
    def_irq_default_handler     LLCC_TXEVT_EMPTY_Handler	       /* 35: LLCC_TXEVTIRQ                */
    def_irq_default_handler     LLCC_TXDMAH_DONE_Handler        /* 36: LLCC_TXDMA0IRQ               */
    def_irq_default_handler     LLCC_TXDMAL_DONE_Handler        /* 37: LLCC_TXDMA1IRQ               */
    def_irq_default_handler     LLCC_RXCMD_VALID_Handler        /* 38: LLCC_RXCMDIRQ                */
    def_irq_default_handler     LLCC_RXEVT_VALID_Handler        /* 39: LLCC_RXEVTIRQ                */
    def_irq_default_handler     LLCC_RXDMAH_DONE_Handler        /* 40: LLCC_RXDMA0IRQ               */
    def_irq_default_handler     LLCC_RXDMAL_DONE_Handler        /* 41: LLCC_RXDMA1IRQ               */
    def_irq_default_handler     PORT2_COMB_Handler              /* 42: GPIO 2                       */
    def_irq_default_handler     PORT3_COMB_Handler              /* 43: GPIO 3                       */
    def_irq_default_handler     TRNG_Handler                    /* 44: TRNG                         */

    .end
