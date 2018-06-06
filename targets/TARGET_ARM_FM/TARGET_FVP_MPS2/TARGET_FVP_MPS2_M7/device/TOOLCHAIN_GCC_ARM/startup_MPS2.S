/*
 * MPS2 CMSIS Library
 */
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

    /* External Interrupts */
    .long    UART0_IRQHandler        /*   0:  UART 0 RX and TX Combined Interrupt         */
    .long    Spare_IRQHandler        /*   1:  Undefined                                   */
    .long    UART1_IRQHandler        /*   2:  UART 1 RX and TX Combined Interrupt         */
    .long    APB_Slave0_IRQHandler   /*   3:  Reserved for APB Slave                      */
    .long    APB_Slave1_IRQHandler   /*   4:  Reserved for APB Slave                      */
    .long    RTC_IRQHandler          /*   5:  RTC Interrupt                               */
    .long    PORT0_IRQHandler        /*   6:  GPIO Port 0 combined Interrupt              */
    .long    PORT1_ALL_IRQHandler    /*   7:  GPIO Port 1 combined Interrupt              */
    .long    TIMER0_IRQHandler       /*   8:  TIMER 0 Interrupt                           */
    .long    TIMER1_IRQHandler       /*   9:  TIMER 1 Interrupt                           */
    .long    DUALTIMER_IRQHandler    /*   10: Dual Timer Interrupt                        */
    .long    APB_Slave2_IRQHandler   /*   11: Reserved for APB Slave                      */
    .long    UARTOVF_IRQHandler      /*   12: UART 0,1,2 Overflow Interrupt               */
    .long    APB_Slave3_IRQHandler   /*   13: Reserved for APB Slave                      */
    .long    RESERVED0_IRQHandler    /*   14: Reserved                                    */
    .long    TSC_IRQHandler          /*   15: Touch Screen Interrupt                      */
    .long    PORT0_0_IRQHandler      /*   16: GPIO Port 0 pin 0 Handler                   */
    .long    PORT0_1_IRQHandler      /*   17: GPIO Port 0 pin 1 Handler                   */
    .long    PORT0_2_IRQHandler      /*   18: GPIO Port 0 pin 2 Handler                   */
    .long    PORT0_3_IRQHandler      /*   19: GPIO Port 0 pin 3 Handler                   */
    .long    PORT0_4_IRQHandler      /*   20: GPIO Port 0 pin 4 Handler                   */
    .long    PORT0_5_IRQHandler      /*   21: GPIO Port 0 pin 5 Handler                   */
    .long    PORT0_6_IRQHandler      /*   22: GPIO Port 0 pin 6 Handler                   */
    .long    PORT0_7_IRQHandler      /*   23: GPIO Port 0 pin 7 Handler                   */
    .long    PORT0_8_IRQHandler      /*   24: GPIO Port 0 pin 8 Handler                   */
    .long    PORT0_9_IRQHandler      /*   25: GPIO Port 0 pin 9 Handler                   */
    .long    PORT0_10_IRQHandler     /*   26: GPIO Port 0 pin 10 Handler                  */
    .long    PORT0_11_IRQHandler     /*   27: GPIO Port 0 pin 11 Handler                  */
    .long    PORT0_12_IRQHandler     /*   28: GPIO Port 0 pin 12 Handler                  */
    .long    PORT0_13_IRQHandler     /*   29: GPIO Port 0 pin 13 Handler                  */
    .long    PORT0_14_IRQHandler     /*   30: GPIO Port 0 pin 14 Handler                  */
    .long    PORT0_15_IRQHandler     /*   31: GPIO Port 0 pin 15 Handler                  */
    .long    FLASH0_IRQHandler       /*   32: Reserved for Flash                          */
    .long    FLASH1_IRQHandler       /*   33: Reserved for Flash                          */
    .long    RESERVED1_IRQHandler    /*   34: Reserved                                    */
    .long    RESERVED2_IRQHandler    /*   35: Reserved                                    */
    .long    RESERVED3_IRQHandler    /*   36: Reserved                                    */
    .long    RESERVED4_IRQHandler    /*   37: Reserved                                    */
    .long    RESERVED5_IRQHandler    /*   38: Reserved                                    */
    .long    RESERVED6_IRQHandler    /*   39: Reserved                                    */
    .long    RESERVED7_IRQHandler    /*   40: Reserved                                    */
    .long    RESERVED8_IRQHandler    /*   41: Reserved                                    */
    .long    PORT2_ALL_IRQHandler    /*   42: GPIO Port 2 combined Interrupt              */
    .long    PORT3_ALL_IRQHandler    /*   43: GPIO Port 3 combined Interrupt              */
    .long    TRNG_IRQHandler         /*   44: Random number generator Interrupt           */
    .long    UART2_IRQHandler        /*   45: UART 2 RX and TX Combined Interrupt         */
    .long    UART3_IRQHandler        /*   46: UART 3 RX and TX Combined Interrupt         */
    .long    ETHERNET_IRQHandler     /*   47: Ethernet interrupt     t.b.a.               */
    .long    I2S_IRQHandler          /*   48: I2S Interrupt                               */
    .long    MPS2_SPI0_IRQHandler    /*   49: SPI Interrupt (spi header)                  */
    .long    MPS2_SPI1_IRQHandler    /*   50: SPI Interrupt (clcd)                        */
    .long    MPS2_SPI2_IRQHandler    /*   51: SPI Interrupt (spi 1 ADC replacement)       */
    .long    MPS2_SPI3_IRQHandler    /*   52: SPI Interrupt (spi 0 shield 0 replacement)  */
    .long    MPS2_SPI4_IRQHandler    /*   53: SPI Interrupt  (shield 1)                   */
    .long    PORT4_ALL_IRQHandler    /*   54: GPIO Port 4 combined Interrupt              */
    .long    PORT5_ALL_IRQHandler    /*   55: GPIO Port 5 combined Interrupt              */
    .long    UART4_IRQHandler        /*   56: UART 4 RX and TX Combined Interrupt         */

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
    def_irq_default_handler     UART0_IRQHandler        /* 0:  UART 0 RX and TX Combined Interrupt        */
    def_irq_default_handler     Spare_IRQHandler        /* 1:  Undefined                                  */
    def_irq_default_handler     UART1_IRQHandler        /* 2:  UART 1 RX and TX Combined Interrupt        */
    def_irq_default_handler     APB_Slave0_IRQHandler   /* 3:  Reserved for APB Slave                     */
    def_irq_default_handler     APB_Slave1_IRQHandler   /* 4:  Reserved for APB Slave                     */
    def_irq_default_handler     RTC_IRQHandler          /* 5:  RTC Interrupt                              */
    def_irq_default_handler     PORT0_IRQHandler        /* 6:  GPIO Port 0 combined Interrupt             */
    def_irq_default_handler     PORT1_ALL_IRQHandler    /* 7:  GPIO Port 1 combined Interrupt             */
    def_irq_default_handler     TIMER0_IRQHandler       /* 8:  TIMER 0 Interrupt                          */
    def_irq_default_handler     TIMER1_IRQHandler       /* 9:  TIMER 1 Interrupt                          */
    def_irq_default_handler     DUALTIMER_IRQHandler    /* 10: Dual Timer Interrupt                       */
    def_irq_default_handler     APB_Slave2_IRQHandler   /* 11: Reserved for APB Slave                     */
    def_irq_default_handler     UARTOVF_IRQHandler      /* 12: UART 0,1,2 Overflow Interrupt              */
    def_irq_default_handler     APB_Slave3_IRQHandler   /* 13: Reserved for APB Slave                     */
    def_irq_default_handler     RESERVED0_IRQHandler    /* 14: Reserved                                   */
    def_irq_default_handler     TSC_IRQHandler          /* 15: Touch Screen Interrupt                     */
    def_irq_default_handler     PORT0_0_IRQHandler      /* 16: GPIO Port 0 pin 0 Handler                  */
    def_irq_default_handler     PORT0_1_IRQHandler      /* 17: GPIO Port 0 pin 1 Handler                  */
    def_irq_default_handler     PORT0_2_IRQHandler      /* 18: GPIO Port 0 pin 2 Handler                  */
    def_irq_default_handler     PORT0_3_IRQHandler      /* 19: GPIO Port 0 pin 3 Handler                  */
    def_irq_default_handler     PORT0_4_IRQHandler      /* 20: GPIO Port 0 pin 4 Handler                  */
    def_irq_default_handler     PORT0_5_IRQHandler      /* 21: GPIO Port 0 pin 5 Handler                  */
    def_irq_default_handler     PORT0_6_IRQHandler      /* 22: GPIO Port 0 pin 6 Handler                  */
    def_irq_default_handler     PORT0_7_IRQHandler      /* 23: GPIO Port 0 pin 7 Handler                  */
    def_irq_default_handler     PORT0_8_IRQHandler      /* 24: GPIO Port 0 pin 8 Handler                  */
    def_irq_default_handler     PORT0_9_IRQHandler      /* 25: GPIO Port 0 pin 9 Handler                  */
    def_irq_default_handler     PORT0_10_IRQHandler     /* 26: GPIO Port 0 pin 10 Handler                 */
    def_irq_default_handler     PORT0_11_IRQHandler     /* 27: GPIO Port 0 pin 11 Handler                 */
    def_irq_default_handler     PORT0_12_IRQHandler     /* 28: GPIO Port 0 pin 12 Handler                 */
    def_irq_default_handler     PORT0_13_IRQHandler     /* 29: GPIO Port 0 pin 13 Handler                 */
    def_irq_default_handler     PORT0_14_IRQHandler     /* 30: GPIO Port 0 pin 14 Handler                 */
    def_irq_default_handler     PORT0_15_IRQHandler     /* 31: GPIO Port 0 pin 15 Handler                 */
    def_irq_default_handler     FLASH0_IRQHandler       /* 32: Reserved for Flash                         */
    def_irq_default_handler     FLASH1_IRQHandler       /* 33: Reserved for Flash                         */
    def_irq_default_handler     RESERVED1_IRQHandler    /* 34: Reserved                                   */
    def_irq_default_handler     RESERVED2_IRQHandler    /* 35: Reserved                                   */
    def_irq_default_handler     RESERVED3_IRQHandler    /* 36: Reserved                                   */
    def_irq_default_handler     RESERVED4_IRQHandler    /* 37: Reserved                                   */
    def_irq_default_handler     RESERVED5_IRQHandler    /* 38: Reserved                                   */
    def_irq_default_handler     RESERVED6_IRQHandler    /* 39: Reserved                                   */
    def_irq_default_handler     RESERVED7_IRQHandler    /* 40: Reserved                                   */
    def_irq_default_handler     RESERVED8_IRQHandler    /* 41: Reserved                                   */
    def_irq_default_handler     PORT2_ALL_IRQHandler    /* 42: GPIO Port 2 combined Interrupt             */
    def_irq_default_handler     PORT3_ALL_IRQHandler    /* 43: GPIO Port 3 combined Interrupt             */
    def_irq_default_handler     TRNG_IRQHandler         /* 44: Random number generator Interrupt          */
    def_irq_default_handler     UART2_IRQHandler        /* 45: UART 2 RX and TX Combined Interrupt        */
    def_irq_default_handler     UART3_IRQHandler        /* 46: UART 3 RX and TX Combined Interrupt        */
    def_irq_default_handler     ETHERNET_IRQHandler     /* 47: Ethernet interrupt     t.b.a.              */
    def_irq_default_handler     I2S_IRQHandler          /* 48: I2S Interrupt                              */
    def_irq_default_handler     MPS2_SPI0_IRQHandler    /* 49: SPI Interrupt (spi header)                 */
    def_irq_default_handler     MPS2_SPI1_IRQHandler    /* 50: SPI Interrupt (clcd)                       */
    def_irq_default_handler     MPS2_SPI2_IRQHandler    /* 51: SPI Interrupt (spi 1 ADC replacement)      */
    def_irq_default_handler     MPS2_SPI3_IRQHandler    /* 52: SPI Interrupt (spi 0 shield 0 replacement) */
    def_irq_default_handler     MPS2_SPI4_IRQHandler    /* 53: SPI Interrupt  (shield 1)                  */
    def_irq_default_handler     PORT4_ALL_IRQHandler    /* 54: GPIO Port 4 combined Interrupt             */
    def_irq_default_handler     PORT5_ALL_IRQHandler    /* 55: GPIO Port 5 combined Interrupt             */
    def_irq_default_handler     UART4_IRQHandler        /* 56: UART 4 RX and TX Combined Interrupt        */

    .end
