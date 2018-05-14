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
    .arch armv6-m

    .section .vector_table,"a",%progbits
    .align 2
    .globl __isr_vector
__isr_vector:
    .long    __StackTop            /* Top of Stack */
    .long    Reset_Handler         /* Reset Handler */
    .long    NMI_Handler           /* NMI Handler */
    .long    HardFault_Handler     /* Hard Fault Handler */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    SVC_Handler           /* SVCall Handler */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    PendSV_Handler        /* PendSV Handler */
    .long    SysTick_Handler       /* SysTick Handler */

    /* External Interrupts */
    .long     UARTRX0_Handler           /* UART 0 RX Handler                 */
    .long     UARTTX0_Handler           /* UART 0 TX Handler                 */
    .long     UARTRX1_Handler           /* UART 1 RX Handler                 */
    .long     UARTTX1_Handler           /* UART 1 TX Handler                 */
    .long     UARTRX2_Handler           /* UART 2 RX Handler                 */
    .long     UARTTX2_Handler           /* UART 2 TX Handler                 */
    .long     PORT0_COMB_Handler        /* GPIO Port 0 Combined Handler      */
    .long     PORT1_COMB_Handler        /* GPIO Port 1 Combined Handler      */
    .long     TIMER0_Handler            /* TIMER 0 handler                   */
    .long     TIMER1_Handler            /* TIMER 1 handler                   */
    .long     DUALTIMER_HANDLER         /* Dual timer handler                */
    .long     SPI_Handler               /* SPI exceptions Handler            */
    .long     UARTOVF_Handler           /* UART 0,1,2 Overflow Handler       */
    .long     ETHERNET_Handler          /* Ethernet Overflow Handler         */
    .long     I2S_Handler               /* I2S Handler                       */
    .long     TSC_Handler               /* Touch Screen handler              */
    .long     PORT2_COMB_Handler        /* GPIO Port 2 Combined Handler      */
    .long     PORT3_COMB_Handler        /* GPIO Port 3 Combined Handler      */
    .long     UARTRX3_Handler           /* UART 3 RX Handler                 */
    .long     UARTTX3_Handler           /* UART 3 TX Handler                 */
    .long     UARTRX4_Handler           /* UART 4 RX Handler                 */
    .long     UARTTX4_Handler           /* UART 4 TX Handler                 */
    .long     ADCSPI_Handler            /* SHIELD ADC SPI exceptions Handler */
    .long     SHIELDSPI_Handler         /* SHIELD SPI exceptions Handler     */
    .long     PORT0_0_Handler           /* GPIO Port 0 pin 0 Handler         */             
    .long     PORT0_1_Handler           /* GPIO Port 0 pin 1 Handler         */
    .long     PORT0_2_Handler           /* GPIO Port 0 pin 2 Handler         */
    .long     PORT0_3_Handler           /* GPIO Port 0 pin 3 Handler         */
    .long     PORT0_4_Handler           /* GPIO Port 0 pin 4 Handler         */
    .long     PORT0_5_Handler           /* GPIO Port 0 pin 5 Handler         */
    .long     PORT0_6_Handler           /* GPIO Port 0 pin 6 Handler         */
    .long     PORT0_7_Handler           /* GPIO Port 0 pin 7 Handler         */

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

    movs	r0, 0
    
    subs	r2, r1
    ble	.L_zero_done
    
.L_zero:
	subs	r2, #4
	str	r0, [r1, r2]
	bgt	.L_zero
.L_zero_done:

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
    def_default_handler    SVC_Handler
    def_default_handler    PendSV_Handler
    def_default_handler    SysTick_Handler
    def_default_handler    Default_Handler

    .macro    def_irq_default_handler    handler_name
    .weak     \handler_name
    .set      \handler_name, Default_Handler
    .endm

    /* External interrupts */
    def_irq_default_handler     UARTRX0_Handler           /* 0:  UART 0 RX Handler                 */
    def_irq_default_handler     UARTTX0_Handler           /* 1:  UART 0 TX Handler                 */
    def_irq_default_handler     UARTRX1_Handler           /* 2:  UART 1 RX Handler                 */
    def_irq_default_handler     UARTTX1_Handler           /* 3:  UART 1 TX Handler                 */
    def_irq_default_handler     UARTRX2_Handler           /* 4:  UART 2 RX Handler                 */
    def_irq_default_handler     UARTTX2_Handler           /* 5:  UART 2 TX Handler                 */
    def_irq_default_handler     PORT0_COMB_Handler        /* 6:  GPIO Port 0 Combined Handler      */
    def_irq_default_handler     PORT1_COMB_Handler        /* 7:  GPIO Port 1 Combined Handler      */
    def_irq_default_handler     TIMER0_Handler            /* 8:  TIMER 0 handler                   */
    def_irq_default_handler     TIMER1_Handler            /* 9:  TIMER 1 handler                   */
    def_irq_default_handler     DUALTIMER_HANDLER         /* 10: Dual timer handler                */
    def_irq_default_handler     SPI_Handler               /* 11: SPI exceptions Handler            */
    def_irq_default_handler     UARTOVF_Handler           /* 12: UART 0,1,2 Overflow Handler       */
    def_irq_default_handler     ETHERNET_Handler          /* 13: Ethernet Overflow Handler         */
    def_irq_default_handler     I2S_Handler               /* 14: I2S Handler                       */
    def_irq_default_handler     TSC_Handler               /* 15: Touch Screen handler              */
    def_irq_default_handler     PORT2_COMB_Handler        /* 16: GPIO Port 2 Combined Handler      */
    def_irq_default_handler     PORT3_COMB_Handler        /* 17: GPIO Port 3 Combined Handler      */
    def_irq_default_handler     UARTRX3_Handler           /* 18: UART 3 RX Handler                 */
    def_irq_default_handler     UARTTX3_Handler           /* 19: UART 3 TX Handler                 */
    def_irq_default_handler     UARTRX4_Handler           /* 20: UART 4 RX Handler                 */
    def_irq_default_handler     UARTTX4_Handler           /* 21: UART 4 TX Handler                 */
    def_irq_default_handler     ADCSPI_Handler            /* 22: SHIELD ADC SPI exceptions Handler */
    def_irq_default_handler     SHIELDSPI_Handler         /* 23: SHIELD SPI exceptions Handler     */
    def_irq_default_handler     PORT0_0_Handler           /* 24: GPIO Port 0 pin 0 Handler         */             
    def_irq_default_handler     PORT0_1_Handler           /* 25: GPIO Port 0 pin 1 Handler         */
    def_irq_default_handler     PORT0_2_Handler           /* 26: GPIO Port 0 pin 2 Handler         */
    def_irq_default_handler     PORT0_3_Handler           /* 27: GPIO Port 0 pin 3 Handler         */
    def_irq_default_handler     PORT0_4_Handler           /* 28: GPIO Port 0 pin 4 Handler         */
    def_irq_default_handler     PORT0_5_Handler           /* 29: GPIO Port 0 pin 5 Handler         */
    def_irq_default_handler     PORT0_6_Handler           /* 30: GPIO Port 0 pin 6 Handler         */
    def_irq_default_handler     PORT0_7_Handler           /* 31: GPIO Port 0 pin 7 Handler         */

    .end
