/*
 * MPS2 CMSIS Library
 */
/*
 * Copyright (c) 2009-2017 ARM Limited. All rights reserved.
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
    .long    UARTRX0_Handler           /* UART 0 RX Handler    */
    .long    UARTTX0_Handler           /* UART 0 TX Handler    */
    .long    UARTRX1_Handler           /* UART 1 RX Handler    */
    .long    UARTTX1_Handler           /* UART 1 TX Handler    */
    .long    UARTRX2_Handler           /* UART 2 RX Handler    */
    .long    UARTTX2_Handler           /* UART 2 TX Handler    */
    .long    PORT0_COMB_Handler        /* GPIO Port 0 Combined Handler    */
    .long    PORT1_COMB_Handler        /* GPIO Port 1 Combined Handler    */
    .long    TIMER0_Handler            /* TIMER 0 handler    */
    .long    TIMER1_Handler            /* TIMER 1 handler    */
    .long    DUALTIMER_HANDLER         /* Dual timer handler    */
    .long    SPI_Handler               /* SPI exceptions Handler    */
    .long    UARTOVF_Handler           /* UART 0,1,2 Overflow Handler    */
    .long    ETHERNET_Handler          /* Ethernet Overflow Handler    */
    .long    I2S_Handler               /* I2S Handler    */
    .long    TSC_Handler               /* Touch Screen handler    */
    .long    PORT2_COMB_Handler        /* GPIO Port 2 Combined Handler    */
    .long    PORT3_COMB_Handler        /* GPIO Port 3 Combined Handler    */
    .long    UARTRX3_Handler           /* UART 3 RX Handler    */
    .long    UARTTX3_Handler           /* UART 3 TX Handler    */
    .long    UARTRX4_Handler           /* UART 4 RX Handler    */
    .long    UARTTX4_Handler           /* UART 4 TX Handler    */
    .long    ADCSPI_Handler            /* SHIELD ADC SPI exceptions Handler    */
    .long    SHIELDSPI_Handler         /* SHIELD SPI exceptions Handler    */
    .long    PORT0_0_Handler           /* GPIO Port 0 pin 0 Handler               */
    .long    PORT0_1_Handler           /* GPIO Port 0 pin 1 Handler               */
    .long    PORT0_2_Handler           /* GPIO Port 0 pin 2 Handler               */
    .long    PORT0_3_Handler           /* GPIO Port 0 pin 3 Handler               */
    .long    PORT0_4_Handler           /* GPIO Port 0 pin 4 Handler               */
    .long    PORT0_5_Handler           /* GPIO Port 0 pin 5 Handler               */
    .long    PORT0_6_Handler           /* GPIO Port 0 pin 6 Handler               */
    .long    PORT0_7_Handler           /* GPIO Port 0 pin 7 Handler               */

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

    def_irq_default_handler UARTRX0_Handler
    def_irq_default_handler UARTTX0_Handler
    def_irq_default_handler UARTRX1_Handler
    def_irq_default_handler UARTTX1_Handler
    def_irq_default_handler UARTRX2_Handler
    def_irq_default_handler UARTTX2_Handler
    def_irq_default_handler PORT0_COMB_Handler
    def_irq_default_handler PORT1_COMB_Handler
    def_irq_default_handler TIMER0_Handler
    def_irq_default_handler TIMER1_Handler
    def_irq_default_handler DUALTIMER_HANDLER
    def_irq_default_handler SPI_Handler
    def_irq_default_handler UARTOVF_Handler
    def_irq_default_handler ETHERNET_Handler
    def_irq_default_handler I2S_Handler
    def_irq_default_handler TSC_Handler
    def_irq_default_handler PORT2_COMB_Handler
    def_irq_default_handler PORT3_COMB_Handler
    def_irq_default_handler UARTRX3_Handler
    def_irq_default_handler UARTTX3_Handler
    def_irq_default_handler UARTRX4_Handler
    def_irq_default_handler UARTTX4_Handler
    def_irq_default_handler ADCSPI_Handler
    def_irq_default_handler SHIELDSPI_Handler
    def_irq_default_handler PORT0_0_Handler
    def_irq_default_handler PORT0_1_Handler
    def_irq_default_handler PORT0_2_Handler
    def_irq_default_handler PORT0_3_Handler
    def_irq_default_handler PORT0_4_Handler
    def_irq_default_handler PORT0_5_Handler
    def_irq_default_handler PORT0_6_Handler
    def_irq_default_handler PORT0_7_Handler

    .end
