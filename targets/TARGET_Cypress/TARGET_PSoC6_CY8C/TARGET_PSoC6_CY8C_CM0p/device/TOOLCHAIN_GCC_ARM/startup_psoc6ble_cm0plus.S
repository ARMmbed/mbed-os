/**************************************************************************//**
 * @file     startup_psoc6ble_cm0plus.s
 * @brief    CMSIS Core Device Startup File for
 *           ARMCM0plus Device Series
 * @version  V5.00
 * @date     02. March 2016
 ******************************************************************************/
/*
 * Copyright (c) 2009-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

    /* Address of the NMI handler */
    #define CY_NMI_HANLDER_ADDR         0x0000000D

    /* The Cortex-M4 VTOR register */
    #define CY_CORTEX_M4_VTOR_ADDR      0x402102C0

    /* The CPU VTOR register */
    #define CY_CPU_VTOR_ADDR                 0xE000ED08

    /* Copy flash vectors and data section to RAM */
    #define __STARTUP_COPY_MULTIPLE

    /* Clear single BSS section */
    #define __STARTUP_CLEAR_BSS

    .syntax    unified
    .arch    armv6-m

    .section .stack
    .align    3
#ifdef __STACK_SIZE
    .equ    Stack_Size, __STACK_SIZE
#else
    .equ    Stack_Size, 0x00001000
#endif
    .globl    __StackTop
    .globl    __StackLimit
__StackLimit:
    .space    Stack_Size
    .size    __StackLimit, . - __StackLimit
__StackTop:
    .size    __StackTop, . - __StackTop

    .section .heap
    .align    3
#ifdef __HEAP_SIZE
    .equ    Heap_Size, __HEAP_SIZE
#else
    .equ    Heap_Size, 0x00000400
#endif
    .globl    __HeapBase
    .globl    __HeapLimit
__HeapBase:
    .if    Heap_Size
    .space    Heap_Size
    .endif
    .size    __HeapBase, . - __HeapBase
__HeapLimit:
    .size    __HeapLimit, . - __HeapLimit

    .section .vectors
    .align 2
    .globl    __Vectors
__Vectors:
    .long    __StackTop            /* Top of Stack */
    .long    Reset_Handler         /* Reset Handler */
    .long    CY_NMI_HANLDER_ADDR    /* NMI Handler */
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

     /* External interrupts                            Power Mode  Description                                           */
    .long    ioss_interrupts_gpio_0_IRQHandler      /* DeepSleep   GPIO Port Interrupt #0                                */
    .long    ioss_interrupts_gpio_1_IRQHandler      /* DeepSleep   GPIO Port Interrupt #1                                */
    .long    ioss_interrupts_gpio_2_IRQHandler      /* DeepSleep   GPIO Port Interrupt #2                                */
    .long    ioss_interrupts_gpio_3_IRQHandler      /* DeepSleep   GPIO Port Interrupt #3                                */
    .long    ioss_interrupts_gpio_4_IRQHandler      /* DeepSleep   GPIO Port Interrupt #4                                */
    .long    ioss_interrupts_gpio_5_IRQHandler      /* DeepSleep   GPIO Port Interrupt #5                                */
    .long    ioss_interrupts_gpio_6_IRQHandler      /* DeepSleep   GPIO Port Interrupt #6                                */
    .long    ioss_interrupts_gpio_7_IRQHandler      /* DeepSleep   GPIO Port Interrupt #7                                */
    .long    ioss_interrupts_gpio_8_IRQHandler      /* DeepSleep   GPIO Port Interrupt #8                                */
    .long    ioss_interrupts_gpio_9_IRQHandler      /* DeepSleep   GPIO Port Interrupt #9                                */
    .long    ioss_interrupts_gpio_10_IRQHandler     /* DeepSleep   GPIO Port Interrupt #10                               */
    .long    ioss_interrupts_gpio_11_IRQHandler     /* DeepSleep   GPIO Port Interrupt #11                               */
    .long    ioss_interrupts_gpio_12_IRQHandler     /* DeepSleep   GPIO Port Interrupt #12                               */
    .long    ioss_interrupts_gpio_13_IRQHandler     /* DeepSleep   GPIO Port Interrupt #13                               */
    .long    ioss_interrupts_gpio_14_IRQHandler     /* DeepSleep   GPIO Port Interrupt #14                               */
    .long    ioss_interrupt_gpio_IRQHandler         /* DeepSleep   GPIO All Ports                                        */
    .long    ioss_interrupt_vdd_IRQHandler          /* DeepSleep   GPIO Supply Detect Interrupt                          */
    .long    lpcomp_interrupt_IRQHandler            /* DeepSleep   Low Power Comparator Interrupt                        */
    .long    scb_8_interrupt_IRQHandler             /* DeepSleep   Serial Communication Block #8 (DeepSleep capable)     */
    .long    srss_interrupt_mcwdt_0_IRQHandler      /* DeepSleep   Multi Counter Watchdog Timer interrupt                */
    .long    srss_interrupt_mcwdt_1_IRQHandler      /* DeepSleep   Multi Counter Watchdog Timer interrupt                */
    .long    srss_interrupt_backup_IRQHandler       /* DeepSleep   Backup domain interrupt                               */
    .long    srss_interrupt_IRQHandler              /* DeepSleep   Other combined Interrupts for SRSS (LVD, WDT, CLKCAL) */
    .long    pass_interrupt_ctbs_IRQHandler         /* DeepSleep   CTBm Interrupt (all CTBms)                            */
    .long    bless_interrupt_IRQHandler             /* DeepSleep   Bluetooth Radio interrupt                             */
    .long    cpuss_interrupts_ipc_0_IRQHandler      /* DeepSleep   CPUSS Inter Process Communication Interrupt #0        */
    .long    cpuss_interrupts_ipc_1_IRQHandler      /* DeepSleep   CPUSS Inter Process Communication Interrupt #1        */
    .long    cpuss_interrupts_ipc_2_IRQHandler      /* DeepSleep   CPUSS Inter Process Communication Interrupt #2        */
    .long    cpuss_interrupts_ipc_3_IRQHandler      /* DeepSleep   CPUSS Inter Process Communication Interrupt #3        */
    .long    cpuss_interrupts_ipc_4_IRQHandler      /* DeepSleep   CPUSS Inter Process Communication Interrupt #4        */
    .long    cpuss_interrupts_ipc_5_IRQHandler      /* DeepSleep   CPUSS Inter Process Communication Interrupt #5        */
    .long    cpuss_interrupts_ipc_6_IRQHandler      /* DeepSleep   CPUSS Inter Process Communication Interrupt #6        */

    .size    __Vectors, . - __Vectors
    .equ    __VectorsSize, . - __Vectors

    .section .ram_vectors
    .align 2
    .globl __ramVectors
__ramVectors:
    .space  __VectorsSize
    .size   __ramVectors, . - __ramVectors


    .text
    .thumb
    .thumb_func
    .align  2

    /* Device startup customization */
    .weak   Cy_OnResetUser
    .func Cy_OnResetUser, Cy_OnResetUser
    .type   Cy_OnResetUser, %function
Cy_OnResetUser:

    bx lr
    .size   Cy_OnResetUser, . - Cy_OnResetUser
    .endfunc

    /* Saves and disables the interrupts */
    .global Cy_SaveIRQ
    .func   Cy_SaveIRQ, Cy_SaveIRQ
    .type   Cy_SaveIRQ, %function
Cy_SaveIRQ:
    mrs r0, PRIMASK
    cpsid i
    bx lr
    .size   Cy_SaveIRQ, . - Cy_SaveIRQ
    .endfunc

    /* Restores the interrupts */
    .global Cy_RestoreIRQ
    .func   Cy_RestoreIRQ, Cy_RestoreIRQ
    .type   Cy_RestoreIRQ, %function
Cy_RestoreIRQ:
    msr PRIMASK, r0
    bx lr
    .size   Cy_RestoreIRQ, . - Cy_RestoreIRQ
    .endfunc

    /* Reset handler */
    .weak    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:

    bl Cy_OnResetUser
    cpsid i

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
    blt    .L_loop0_0_done
    ldr    r0, [r1, r3]
    str    r0, [r2, r3]
    b    .L_loop0_0

.L_loop0_0_done:
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

    subs    r3, r2
    ble    .L_loop1_done

.L_loop1:
    subs    r3, #4
    ldr    r0, [r1,r3]
    str    r0, [r2,r3]
    bgt    .L_loop1

.L_loop1_done:
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
    blt    .L_loop2_0_done
    str    r0, [r1, r2]
    b    .L_loop2_0
.L_loop2_0_done:

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

    subs    r2, r1
    ble    .L_loop3_done

.L_loop3:
    subs    r2, #4
    str    r0, [r1, r2]
    bgt    .L_loop3
.L_loop3_done:
#endif /* __STARTUP_CLEAR_BSS_MULTIPLE || __STARTUP_CLEAR_BSS */

    /* Update Vector Table Offset Register. */
    ldr r0, =__ramVectors
     ldr r1, =CY_CPU_VTOR_ADDR
    str r0, [r1]
    dsb 0xF

#ifndef __NO_SYSTEM_INIT
    bl    SystemInit
#endif

    bl    _start

    /* Should never get here */
    b   .

    .pool
    .size    Reset_Handler, . - Reset_Handler

    .align    1
    .thumb_func
    .weak    Default_Handler
    .type    Default_Handler, %function
Default_Handler:
    b    .
    .size    Default_Handler, . - Default_Handler


    .weak    Cy_SysLib_FaultHandler
    .type    Cy_SysLib_FaultHandler, %function
Cy_SysLib_FaultHandler:
    b    .
    .size    Cy_SysLib_FaultHandler, . - Cy_SysLib_FaultHandler


    .type Fault_Handler, %function
Fault_Handler:
    /* Storing LR content for Creator call stack trace */
    push {LR}
    movs r0, #4
    mov r1, LR
    tst r0, r1
    beq .L_MSP
    mrs r0, PSP
    b .L_API_call
.L_MSP:
    mrs r0, MSP
.L_API_call:
    /* Compensation of stack pointer address due to pushing 4 bytes of LR */
    adds r0, r0, #4
    bl Cy_SysLib_FaultHandler
    b   .
    .size    Fault_Handler, . - Fault_Handler

.macro    def_fault_Handler    fault_handler_name
    .weak    \fault_handler_name
    .set    \fault_handler_name, Fault_Handler
    .endm

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro    def_irq_handler    handler_name
    .weak    \handler_name
    .set    \handler_name, Default_Handler
    .endm

    def_irq_handler    NMI_Handler

    def_fault_Handler HardFault_Handler

    def_irq_handler    SVC_Handler
    def_irq_handler    PendSV_Handler
    def_irq_handler    SysTick_Handler

    def_irq_handler ioss_interrupts_gpio_0_IRQHandler
    def_irq_handler ioss_interrupts_gpio_1_IRQHandler
    def_irq_handler ioss_interrupts_gpio_2_IRQHandler
    def_irq_handler ioss_interrupts_gpio_3_IRQHandler
    def_irq_handler ioss_interrupts_gpio_4_IRQHandler
    def_irq_handler ioss_interrupts_gpio_5_IRQHandler
    def_irq_handler ioss_interrupts_gpio_6_IRQHandler
    def_irq_handler ioss_interrupts_gpio_7_IRQHandler
    def_irq_handler ioss_interrupts_gpio_8_IRQHandler
    def_irq_handler ioss_interrupts_gpio_9_IRQHandler
    def_irq_handler ioss_interrupts_gpio_10_IRQHandler
    def_irq_handler ioss_interrupts_gpio_11_IRQHandler
    def_irq_handler ioss_interrupts_gpio_12_IRQHandler
    def_irq_handler ioss_interrupts_gpio_13_IRQHandler
    def_irq_handler ioss_interrupts_gpio_14_IRQHandler
    def_irq_handler ioss_interrupt_gpio_IRQHandler
    def_irq_handler ioss_interrupt_vdd_IRQHandler
    def_irq_handler lpcomp_interrupt_IRQHandler
    def_irq_handler scb_8_interrupt_IRQHandler
    def_irq_handler srss_interrupt_mcwdt_0_IRQHandler
    def_irq_handler srss_interrupt_mcwdt_1_IRQHandler
    def_irq_handler srss_interrupt_backup_IRQHandler
    def_irq_handler srss_interrupt_IRQHandler
    def_irq_handler pass_interrupt_ctbs_IRQHandler
    def_irq_handler bless_interrupt_IRQHandler
    def_irq_handler cpuss_interrupts_ipc_0_IRQHandler
    def_irq_handler cpuss_interrupts_ipc_1_IRQHandler
    def_irq_handler cpuss_interrupts_ipc_2_IRQHandler
    def_irq_handler cpuss_interrupts_ipc_3_IRQHandler
    def_irq_handler cpuss_interrupts_ipc_4_IRQHandler
    def_irq_handler cpuss_interrupts_ipc_5_IRQHandler
    def_irq_handler cpuss_interrupts_ipc_6_IRQHandler

    .end


/* [] END OF FILE */
