/* File: startup_ARMCM3.s
 * Purpose: startup file for Cortex-M3/M4 devices. Should use with 
 *   GNU Tools for ARM Embedded Processors
 * Version: V1.1
 * Date: 17 June 2011
 * 
 * Copyright (C) 2011 ARM Limited. All rights reserved.
 * ARM Limited (ARM) is supplying this software for use with Cortex-M3/M4 
 * processor based microcontrollers.  This file can be freely distributed 
 * within development tools that are supporting such ARM based processors. 
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 */
    .syntax unified
    .extern _start
 
@ Standard definitions of mode bits and interrupt (I & F) flags in PSRs
    .equ    Mode_USR        ,   0x10
    .equ    Mode_FIQ        ,   0x11
    .equ    Mode_IRQ        ,   0x12
    .equ    Mode_SVC        ,   0x13
    .equ    Mode_ABT        ,   0x17
    .equ    Mode_UND        ,   0x1B
    .equ    Mode_SYS        ,   0x1F

    .equ    I_Bit           ,   0x80            @ when I bit is set, IRQ is disabled 
    .equ    F_Bit           ,   0x40            @ when F bit is set, FIQ is disabled 
    .equ    T_Bit           ,   0x20            @ when T bit is set, core is in Thumb state 
 
@ Stack Configuration

    .EQU    UND_Stack_Size  , 0x00000100
    .EQU    SVC_Stack_Size  , 0x00008000
    .EQU    ABT_Stack_Size  , 0x00000100
    .EQU    FIQ_Stack_Size  , 0x00000100
    .EQU    IRQ_Stack_Size  , 0x0000F000

    .EQU    ISR_Stack_Size, (UND_Stack_Size + SVC_Stack_Size + ABT_Stack_Size + FIQ_Stack_Size + IRQ_Stack_Size)

    .section .stack
    .align 3
    .globl    __StackTop
    .globl    __StackLimit
__StackLimit:
    .space    ISR_Stack_Size
__initial_sp:
    .size __StackLimit, . - __StackLimit
__StackTop:
    .size __StackTop, . - __StackTop


@ Heap Configuration

    .EQU    Heap_Size       , 0x00080000

    .section .heap
    .align 3
    .globl    __HeapBase
    .globl    __HeapLimit
__HeapBase:
    .space    Heap_Size
    .size __HeapBase, . - __HeapBase
__HeapLimit:
    .size __HeapLimit, . - __HeapLimit


    .section .isr_vector
    .align 2
    .globl __isr_vector
__isr_vector:
    .long   0xe59ff018 /* 0x00 */
    .long   0xe59ff018 /* 0x04 */
    .long   0xe59ff018 /* 0x08 */
    .long   0xe59ff018 /* 0x0c */
    .long   0xe59ff018 /* 0x10 */
    .long   0xe59ff018 /* 0x14 */
    .long   0xe59ff018 /* 0x18 */
    .long   0xe59ff018 /* 0x1c */

    .long   Reset_Handler         /* 0x20 */
    .long   Undef_Handler         /* 0x24 */
    .long   SVC_Handler           /* 0x28 */
    .long   PAbt_Handler          /* 0x2c */
    .long   DAbt_Handler          /* 0x30 */
    .long   0                     /* Reserved */
    .long   IRQ_Handler           /* IRQ */
    .long   FIQ_Handler           /* FIQ */


    .size    __isr_vector, . - __isr_vector

    .text
    .align 2
    .globl         Reset_Handler
    .type          Reset_Handler, %function
Reset_Handler:
    @ Mask interrupts
    CPSID   if                           

    @ Put any cores other than 0 to sleep
    mrc     p15, 0, r0, c0, c0, 5   @ Read MPIDR
    ands    r0, r0, #3
goToSleep:
    wfine
    bne     goToSleep

    @ Reset SCTLR Settings
    mrc     p15, 0, r0, c1, c0, 0       @ Read CP15 System Control register 
    bic     r0, r0, #(0x1 << 12)        @ Clear I bit 12 to disable I Cache 
    bic     r0, r0, #(0x1 <<  2)        @ Clear C bit  2 to disable D Cache 
    bic     r0, r0, #0x1                @ Clear M bit  0 to disable MMU 
    bic     r0, r0, #(0x1 << 11)        @ Clear Z bit 11 to disable branch prediction 
    bic     r0, r0, #(0x1 << 13)        @ Clear V bit 13 to disable hivecs 
    mcr     p15, 0, r0, c1, c0, 0       @ Write value back to CP15 System Control register 
    isb

    @ Configure ACTLR
    MRC     p15, 0, r0, c1, c0, 1       @ Read CP15 Auxiliary Control Register
    ORR     r0, r0, #(1 <<  1)          @ Enable L2 prefetch hint (UNK/WI since r4p1)
    MCR     p15, 0, r0, c1, c0, 1       @ Write CP15 Auxiliary Control Register

    @ Set Vector Base Address Register (VBAR) to point to this application's vector table
    ldr     r0, =__isr_vector
    mcr     p15, 0, r0, c12, c0, 0
 
@ Setup Stack for each exceptional mode 
    ldr     r0, =__StackTop

@ Enter Undefined Instruction Mode and set its Stack Pointer 
    msr     cpsr_c, #(Mode_UND | I_Bit | F_Bit)
    mov     sp, r0
    sub     r0, r0, #UND_Stack_Size

@ Enter Abort Mode and set its Stack Pointer 
    msr     cpsr_c, #(Mode_ABT | I_Bit | F_Bit)
    mov     sp, r0 
    sub     r0, r0, #ABT_Stack_Size

@ Enter FIQ Mode and set its Stack Pointer 
    msr     cpsr_c, #(Mode_FIQ | I_Bit | F_Bit)
    mov     sp, r0 
    sub     r0, r0, #FIQ_Stack_Size

@ Enter IRQ Mode and set its Stack Pointer 
    msr     cpsr_c, #(Mode_IRQ | I_Bit | F_Bit)
    mov     sp, r0 
    sub     r0, r0, #IRQ_Stack_Size

@ Enter Supervisor Mode and set its Stack Pointer 
    msr     cpsr_c, #(Mode_SVC | I_Bit | F_Bit)
    mov     sp, r0 

@ Enter System Mode to complete initialization and enter kernel 
    msr     cpsr_c, #(Mode_SYS | I_Bit | F_Bit)
    mov     sp, r0 

@  USR/SYS stack pointer will be set during kernel init
    ldr     r0, =SystemInit
    blx     r0

    @ Unmask interrupts
    CPSIE  if

@ data sections copy
    ldr     r4, =__copy_table_start__
    ldr     r5, =__copy_table_end__

.L_loop0:
    cmp     r4, r5
    bge     .L_loop0_done
    ldr     r1, [r4]
    ldr     r2, [r4, #4]
    ldr     r3, [r4, #8]

.L_loop0_0:
    subs    r3, #4
    ittt    ge
    ldrge   r0, [r1, r3]
    strge   r0, [r2, r3]
    bge     .L_loop0_0

    adds    r4, #12
    b       .L_loop0

.L_loop0_done:

@ bss sections clear
    ldr     r3, =__zero_table_start__
    ldr     r4, =__zero_table_end__

.L_loop2:
    cmp     r3, r4
    bge     .L_loop2_done
    ldr     r1, [r3]
    ldr     r2, [r3, #4]
    movs    r0, 0

.L_loop2_0:
    subs    r2, #4
    itt     ge
    strge   r0, [r1, r2]
    bge     .L_loop2_0

    adds    r3, #8
    b       .L_loop2
.L_loop2_done:


    ldr     r0, =_start
    bx      r0

    ldr     r0, sf_boot     @ dummy to keep boot loader area
loop_here:
    b       loop_here

sf_boot: 
    .word   boot_loader

    .pool
    .size Reset_Handler, . - Reset_Handler


    .text

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
                .macro    def_default_handler    handler_name
                .align 1
                .thumb_func
                .weak    \handler_name
                .type    \handler_name, %function
\handler_name :
                b    .
                .size    \handler_name, . - \handler_name
                .endm

                def_default_handler    Undef_Handler
                def_default_handler    SVC_Handler
                def_default_handler    PAbt_Handler
                def_default_handler    DAbt_Handler
                def_default_handler    IRQ_Handler
                def_default_handler    FIQ_Handler

                .END
