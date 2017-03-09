/* mbed Microcontroller Library
 * Copyright (c) 2016 u-blox
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
    .syntax unified
    .cpu    cortex-m0
    .fpu    softvfp
    .thumb

/* Setup the stack
 *  Since it grows downward the stack_system is the first location is the highest address
 *  The first other usage should therefore be at stack_system + 4
 *  Should the stack overflow, there would be a hard fault in the core
 */

    .section .stacks,"aw",%progbits

#ifdef __STACK_SIZE
    .equ    Stack_Size, __STACK_SIZE
#else
    .equ    Stack_Size, 1024
#endif
    .globl    Stack_Size
    .globl    __StackTop
    .globl    __StackLimit
    .align  2
    .type   stack_system, %object
    .size   stack_system, Stack_Size
stack_system_end:
    .space  Stack_Size - 4
stack_system:
    .space  4


/* This is the code that gets called when the processor first
 * starts execution following a reset event. Only the absolutely
 * necessary set is performed, after which the application
 * supplied main() routine is called.
 */
        .section        .text.Reset_Handler,"ax",%progbits
        .weak           Reset_Handler
        .type           Reset_Handler, %function
        .thumb
        .func           Reset_Handler
Reset_Handler:

        /* Make sure SP is really at the start of the stack */
        ldr     r0, =stack_system
        msr     msp, r0

        /* Prefill the system stack with 0xefbeadde (or deafbeef in little endian) */
        ldr     r1, =__SYSTEM_STACK_START__
        ldr     r3, =__SYSTEM_STACK_END__
        subs    r3, r3, r1
        beq     .stack_fill_loop_end
        ldr     r2, =0xefbeadde
.stack_fill_loop:
        str     r2, [r1, #0]           /* Store the quad octet initialisation value in r2 into address in r1 */
        adds    r1, r1, #4             /* Increased address in r1 by a quad octet */
        subs    r3, r3, #4             /* Decrease the number of bytes to do by a quad octet */
        bgt     .stack_fill_loop       /* Keep going until it is all done */
.stack_fill_loop_end:

        /* setup .data section */
        ldr     r1, =__data_start__
        ldr     r2, =__data_load__
        ldr     r3, =__data_size__
        cmp     r3, #0
        beq     .end_set_data_loop

.set_data_loop:
        ldrb    r4, [r2, #0]           /* Load the octet value into r4 from address in r2 */
        strb    r4, [r1, #0]           /* Store the octet value in r4 to address in r1 */
        adds    r2, r2, #1             /* Move onto next octet */
        adds    r1, r1, #1
        subs    r3, r3, #1             /* Decrease the number of bytes to do by an octet */
        bgt     .set_data_loop         /* Keep going until it is all done */
.end_set_data_loop:

        /* Call init function */
        ldr     r0, =SystemInit
        blx     r0

        /* Call C++ startup */
        ldr     r0, =_start
        bx      r0
        bl      .

        .size  Reset_Handler, .-Reset_Handler
        .endfunc

/* This is the code that gets called when the processor receives an
 * unexpected interrupt.  This simply enters an infinite loop, preserving
 * the system state for examination by a debugger.
 */
        .section        .text.Default_Handler,"ax",%progbits
        .global         Default_Handler
        .func           Default_Handler
Default_Handler:
Infinite_Loop:
        b       Infinite_Loop

        .endfunc
        .size  Default_Handler, .-Default_Handler

/******************************************************************************
 * The minimal vector table for a Cortex M0+. Note that the proper constructs
 * must be placed on this to ensure that it ends up at physical address 0x0000.0000.
 *******************************************************************************/

        .section        .isr_vector,"a",%progbits
        .type           g_pfnVectors, %object
        .size           g_pfnVectors, .-g_pfnVectors
        .global         g_pfnVectors

g_pfnVectors:
        .word   stack_system
        .word   Reset_Handler
        .word   NMI_Handler
        .word   HardFault_Handler
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   SVC_Handler
        .word   0
        .word   0
        .word   PendSV_Handler
        .word   SysTick_Handler

/* External Interrupts */
        .word   IRQ0_RTC_Handler
        .word   IRQ1_TMR0_Handler
        .word   IRQ2_SECURITY_Handler
        .word   IRQ3_PROTOCOL_Handler
        .word   IRQ4_APPS_Handler
        .word   IRQ5_GPIO_Handler
        .word   IRQ6_DMA_Handler
        .word   IRQ7_UART0_Handler
        .word   IRQ8_UART1_Handler
        .word   IRQ9_SSP0_Handler
        .word   IRQ10_SSP1_Handler
        .word   IRQ11_PWM0IN_Handler
        .word   IRQ12_PWM0OUT_Handler
        .word   IRQ13_PWM1IN_Handler
        .word   IRQ14_PWM1OUT_Handler
        .word   IRQ15_I2C_Handler
        .word   IRQ16_LPUART_Handler
        .word   IRQ17_CAP_Handler
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0

/*******************************************************************************
 * Provide weak aliases for each Exception handler to the Default_Handler.
 * As they are weak aliases, any function with the same name will override this definition.
 * Note: For system exception handlers we don't want those references to be weak
 * - we want to force someone to write those handlers.
 *******************************************************************************/

        .weak       NMI_Handler
        .thumb_set  NMI_Handler,Default_Handler

        .weak       HardFault_Handler
        .thumb_set  HardFault_Handler,Default_Handler

        .weak       SVC_Handler
        .thumb_set  SVC_Handler,Default_Handler

        .weak       PendSV_Handler
        .thumb_set  PendSV_Handler,Default_Handler

        .weak       SysTick_Handler
        .thumb_set  SysTick_Handler,Default_Handler

        .weak       IRQ0_RTC_Handler
        .thumb_set  IRQ0_RTC_Handler,Default_Handler

        .weak       IRQ1_TMR0_Handler
        .thumb_set  IRQ1_TMR0_Handler,Default_Handler

        .weak       IRQ2_SECURITY_Handler
        .thumb_set  IRQ2_SECURITY_Handler,Default_Handler

        .weak       IRQ3_PROTOCOL_Handler
        .thumb_set  IRQ3_PROTOCOL_Handler,Default_Handler

        .weak       IRQ4_APPS_Handler
        .thumb_set  IRQ4_APPS_Handler,Default_Handler

        .weak       IRQ5_GPIO_Handler
        .thumb_set  IRQ5_GPIO_Handler,Default_Handler

        .weak       IRQ6_DMA_Handler
        .thumb_set  IRQ6_DMA_Handler,Default_Handler

        .weak       IRQ7_UART0_Handler
        .thumb_set  IRQ7_UART0_Handler,Default_Handler

        .weak       IRQ8_UART1_Handler
        .thumb_set  IRQ8_UART1_Handler,Default_Handler

        .weak       IRQ9_SSP0_Handler
        .thumb_set  IRQ9_SSP0_Handler,Default_Handler

        .weak       IRQ10_SSP1_Handler
        .thumb_set  IRQ10_SSP1_Handler,Default_Handler

        .weak       IRQ11_PWM0IN_Handler
        .thumb_set  IRQ11_PWM0IN_Handler,Default_Handler

        .weak       IRQ12_PWM0OUT_Handler
        .thumb_set  IRQ12_PWM0OUT_Handler,Default_Handler

        .weak       IRQ13_PWM1IN_Handler
        .thumb_set  IRQ13_PWM1IN_Handler,Default_Handler

        .weak       IRQ14_PWM1OUT_Handler
        .thumb_set  IRQ14_PWM1OUT_Handler,Default_Handler

        .weak       IRQ15_I2C_Handler
        .thumb_set  IRQ15_I2C_Handler,Default_Handler

        .weak       IRQ16_LPUART_Handler
        .thumb_set  IRQ16_LPUART_Handler,Default_Handler

        .weak       IRQ17_CAP_Handler
        .thumb_set  IRQ17_CAP_Handler,Default_Handler

