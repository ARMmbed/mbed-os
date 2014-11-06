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

@ Standard definitions of mode bits and interrupt (I & F) flags in PSRs
    .equ    USR_MODE,   0x10
    .equ    FIQ_MODE,   0x11
    .equ    IRQ_MODE,   0x12
    .equ    SVC_MODE,   0x13
    .equ    ABT_MODE,   0x17
    .equ    UND_MODE,   0x1b
    .equ    SYS_MODE,   0x1f
    .equ    Thum_bit,   0x20            @ CPSR/SPSR Thumb bit

/* Memory Model
   The HEAP starts at the end of the DATA section and grows upward.
   
   The STACK starts at the end of the RAM and grows downward.
   
   The HEAP and stack STACK are only checked at compile time:
   (DATA_SIZE + HEAP_SIZE + STACK_SIZE) < RAM_SIZE
   
   This is just a check for the bare minimum for the Heap+Stack area before
   aborting compilation, it is not the run time limit:
   Heap_Size + Stack_Size = 0x80 + 0x80 = 0x100
 */

    .section .stack
    .align 3
#ifdef __STACK_SIZE
    .equ    Stack_Size, __STACK_SIZE
#else
    .equ    Stack_Size, 0xc00
#endif
    .globl    __StackTop
    .globl    __StackLimit
__StackLimit:
    .space    Stack_Size
    .size __StackLimit, . - __StackLimit
__StackTop:
    .size __StackTop, . - __StackTop
__AStackLimit:
    .space    Stack_Size
    .size __AStackLimit, . - __AStackLimit
__AStackTop:
    .size __AStackTop, . - __AStackTop
__BStackLimit:
    .space    Stack_Size
    .size __BStackLimit, . - __StackLimit
__BStackTop:
    .size __BStackTop, . - __BStackTop
__CStackLimit:
    .space    Stack_Size
    .size __CStackLimit, . - __CStackLimit
__CStackTop:
    .size __CStackTop, . - __CStackTop

    .section .heap
    .align 3
#ifdef __HEAP_SIZE
    .equ    Heap_Size, __HEAP_SIZE
#else
    .equ    Heap_Size, 0x800
#endif
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
    .long    0xe59ff018 // 0x00
    .long    0xe59ff018 // 0x04
    .long    0xe59ff018 // 0x08
    .long    0xe59ff018 // 0x0c
    .long    0xe59ff018 // 0x10
    .long    0xe59ff018 // 0x14
    .long    0xe59ff018 // 0x18
    .long    0xe59ff018 // 0x1c

    .long    Reset_Handler         /* 0x20 */
    .long    undefinedInstruction  /* 0x24 */
    .long    softwareInterrupt     /* 0x28 */
    .long    prefetchAboart        /* 0x2c */
    .long    dataAbort             /* 0x30 */
    .long    0                     /* Reserved */
    .long    irq_handler           /* IRQ */
    .long    fiq_handler           /* FIQ */


    .size    __isr_vector, . - __isr_vector

    .text
//    .thumb
//    .thumb_func
    .align 2
    .globl    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
/*     Loop to copy data from read only memory to RAM. The ranges
 *      of copy from/to are specified by following symbols evaluated in 
 *      linker script.
 *      _etext: End of code section, i.e., begin of data sections to copy from.
 *      __data_start__/__data_end__: RAM address range that data should be
 *      copied to. Both must be aligned to 4 bytes boundary.  */

    mrc  p15, 0, r0, c1, c0, 0      @;; Read CP15 System Control register (SCTLR)
    bic  r0, r0, #(0x1 << 12)       @;; Clear I bit 12 to disable I Cache
    bic  r0, r0, #(0x1 <<  2)       @;; Clear C bit  2 to disable D Cache
    bic  r0, r0, #0x1               @;; Clear M bit  0 to disable MMU
    mcr  p15, 0, r0, c1, c0, 0      @;; Write value back to CP15 System Control register
    
                                    @;; SVC Mode(Default)
    LDR  sp, =__AStackTop

    CPS  #IRQ_MODE                  @;; IRQ Mode
    LDR  sp, =__BStackTop

    CPS  #FIQ_MODE                  @;; FIQ Mode
    LDR  sp, =__CStackTop

    @CPS  #ABT_MODE                  @;; ABT Mode
    @LDR  sp, =__StackTop

    CPS  #SYS_MODE                  @;; SYS Mode

@; System mode Stack pointer is set up ARM_LIB_STACK in the __main()->__entry()
    LDR  sp, =__StackTop

    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

.Lflash_to_ram_loop:
    cmp     r2, r3
    ittt    lt
    ldrlt   r0, [r1], #4
    strlt   r0, [r2], #4
    blt    .Lflash_to_ram_loop

    ldr    r0, =set_low_vector
    blx    r0
    ldr    r0, =enable_VFP
    blx    r0

    ldr    r0, =SystemInit
    blx    r0
    ldr    r0, =_start
    bx    r0

set_low_vector:
    mrc    p15, 0, r0, c1, c0, 0
    mov    r1, #0xffffdfff
    and    r0, r1
    mcr    p15, 0, r0, c1, c0, 0

    mrc    p15, 0, r0, c12, c0, 0  // vector set
    mov    r0, #0x20000000
    mcr    p15, 0, r0, c12, c0, 0  // vector set
    bx     lr

.equ    VFPEnable,  0x40000000
enable_VFP:
    ;;
    mrc     p15, 0, r0, c1, c0, 2   ;
    orr     r0, r0, #(3 << 20)      ; 
    orr     r0, r0, #(3 << 22)      ;
    bic     r0, r0, #(3 << 30)      ;
    mcr     p15, 0, r0, c1, c0, 2   ;
    isb                             ;
    ;;
    mov     r0, #VFPEnable
    vmsr    fpexc, r0
    bx      lr
    ;;
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

    def_default_handler    undefinedInstruction  /* 0x24 */
    def_default_handler    softwareInterrupt     /* 0x28 */
    def_default_handler    prefetchAboart        /* 0x2c */
    def_default_handler    dataAbort             /* 0x30 */
    def_default_handler    Default_Handler       /* --- */

    .global __disable_irq
    .global __enable_irq

    .global __disable_fiq
    .global __enable_fiq

__disable_irq:
    mrs     r0,apsr             @ formerly cpsr
    and     r0,r0,#0x80
    cpsid   i
    bx      lr

__enable_irq:
    cpsie   i
    bx      lr


__disable_fiq:
    cpsid   f
    bx      lr

__enable_fiq:
    cpsie   f
    bx      lr



    .end

