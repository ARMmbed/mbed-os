/**************************************************************************//**
 * @file     startup_CC3220SF.S
 * @brief    CMSIS Cortex-M4 Core Device Startup File for
 *           Device CC3220SF
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


.syntax unified
.arch    armv7-m

.section .stack
.align 3

/*
// <h> Stack Configuration
//   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
// </h>
*/

#ifdef __STACK_SIZE
.equ    Stack_Size, __STACK_SIZE
#else
.equ    Stack_Size, 0x1000
#endif
.globl    __StackTop
.globl    __StackLimit
__StackLimit:
.space    Stack_Size
.size __StackLimit, . - __StackLimit
__StackTop:
.size __StackTop, . - __StackTop

    .section .isr_vector
    .align 2
    .globl    __Vectors
__Vectors:
    .long   __StackTop                  /* Top of Stack          */
    .long   Reset_Handler               /* Reset Handler         */
    .long   NMI_Handler                 /* NMI Handler           */
    .long   HardFault_Handler           /* Hard Fault Handler    */
    .long   MemManage_Handler           /* MPU Fault Handler     */
    .long   BusFault_Handler            /* Bus Fault Handler     */
    .long   UsageFault_Handler          /* Usage Fault Handler   */
    .long   0                           /* Reserved              */
    .long   0                           /* Reserved              */
    .long   0                           /* Reserved              */
    .long   0                           /* Reserved              */
    .long   SVC_Handler                 /* SVCall Handler        */
    .long   DebugMon_Handler            /* Debug Monitor Handler */
    .long   0                           /* Reserved              */
    .long   PendSV_Handler              /* PendSV Handler        */
    .long   SysTick_Handler             /* SysTick Handler       */

    /* External interrupts */
    .long     INT_GPIOA0_IRQHandler     /* 16 */
    .long     INT_GPIOA1_IRQHandler     /* 17 */
    .long     INT_GPIOA2_IRQHandler     /* 18 */
    .long     INT_GPIOA3_IRQHandler     /* 19 */
    .long     0                         /* 20 Reserved interrupt */
    .long     INT_UARTA0_IRQHandler     /* 21 */
    .long     INT_UARTA1_IRQHandler     /* 22 */
    .long     0                         /* 23 Reserved interrupt */
    .long     INT_I2CA0_IRQHandler      /* 24 */
    .long     0                         /* 25 Reserved interrupt */
    .long     0                         /* 26 Reserved interrupt */
    .long     0                         /* 27 Reserved interrupt */
    .long     0                         /* 28 Reserved interrupt */
    .long     0                         /* 29 Reserved interrupt */
    .long     INT_ADCCH0_IRQHandler     /* 30 */
    .long     INT_ADCCH1_IRQHandler     /* 31 */
    .long     INT_ADCCH2_IRQHandler     /* 32 */
    .long     INT_ADCCH3_IRQHandler     /* 33 */
    .long     INT_WDT_IRQHandler        /* 34 */
    .long     INT_TIMERA0A_IRQHandler   /* 35 */
    .long     INT_TIMERA0B_IRQHandler   /* 36 */
    .long     INT_TIMERA1A_IRQHandler   /* 37 */
    .long     INT_TIMERA1B_IRQHandler   /* 38 */
    .long     INT_TIMERA2A_IRQHandler   /* 39 */
    .long     INT_TIMERA2B_IRQHandler   /* 40 */
    .long     0                         /* 41 Reserved interrupt */
    .long     0                         /* 42 Reserved interrupt */
    .long     0                         /* 43 Reserved interrupt */
    .long     0                         /* 44 Reserved interrupt */
    .long     INT_FLASH_IRQHandler      /* 45 */
    .long     0                         /* 46 Reserved interrupt */
    .long     0                         /* 47 Reserved interrupt */
    .long     0                         /* 48 Reserved interrupt */
    .long     0                         /* 49 Reserved interrupt */
    .long     0                         /* 50 Reserved interrupt */
    .long     INT_TIMERA3A_IRQHandler   /* 51 */
    .long     INT_TIMERA3B_IRQHandler   /* 52 */
    .long     0                         /* 53 Reserved interrupt */
    .long     0                         /* 54 Reserved interrupt */
    .long     0                         /* 55 Reserved interrupt */
    .long     0                         /* 56 Reserved interrupt */
    .long     0                         /* 57 Reserved interrupt */
    .long     0                         /* 58 Reserved interrupt */
    .long     0                         /* 59 Reserved interrupt */
    .long     0                         /* 60 Reserved interrupt */
    .long     0                         /* 61 Reserved interrupt */
    .long     INT_UDMA_IRQHandler       /* 62 */
    .long     INT_UDMAERR_IRQHandler    /* 63 */
    .long     0                         /* 64 Reserved interrupt */
    .long     0                         /* 65 Reserved interrupt */
    .long     0                         /* 66 Reserved interrupt */
    .long     0                         /* 67 Reserved interrupt */
    .long     0                         /* 68 Reserved interrupt */
    .long     0                         /* 69 Reserved interrupt */
    .long     0                         /* 70 Reserved interrupt */
    .long     0                         /* 71 Reserved interrupt */
    .long     0                         /* 72 Reserved interrupt */
    .long     0                         /* 73 Reserved interrupt */
    .long     0                         /* 74 Reserved interrupt */
    .long     0                         /* 75 Reserved interrupt */
    .long     0                         /* 76 Reserved interrupt */
    .long     0                         /* 77 Reserved interrupt */
    .long     0                         /* 78 Reserved interrupt */
    .long     0                         /* 79 Reserved interrupt */
    .long     0                         /* 80 Reserved interrupt */
    .long     0                         /* 81 Reserved interrupt */
    .long     0                         /* 82 Reserved interrupt */
    .long     0                         /* 83 Reserved interrupt */
    .long     0                         /* 84 Reserved interrupt */
    .long     0                         /* 85 Reserved interrupt */
    .long     0                         /* 86 Reserved interrupt */
    .long     0                         /* 87 Reserved interrupt */
    .long     0                         /* 88 Reserved interrupt */
    .long     0                         /* 89 Reserved interrupt */
    .long     0                         /* 90 Reserved interrupt */
    .long     0                         /* 91 Reserved interrupt */
    .long     0                         /* 92 Reserved interrupt */
    .long     0                         /* 93 Reserved interrupt */
    .long     0                         /* 94 Reserved interrupt */
    .long     0                         /* 95 Reserved interrupt */
    .long     0                         /* 96 Reserved interrupt */
    .long     0                         /* 97 Reserved interrupt */
    .long     0                         /* 98 Reserved interrupt */
    .long     0                         /* 99 Reserved interrupt */
    .long     0                         /* 100 Reserved interrupt */
    .long     0                         /* 101 Reserved interrupt */
    .long     0                         /* 102 Reserved interrupt */
    .long     0                         /* 103 Reserved interrupt */
    .long     0                         /* 104 Reserved interrupt */
    .long     0                         /* 105 Reserved interrupt */
    .long     0                         /* 106 Reserved interrupt */
    .long     0                         /* 107 Reserved interrupt */
    .long     0                         /* 108 Reserved interrupt */
    .long     0                         /* 109 Reserved interrupt */
    .long     0                         /* 110 Reserved interrupt */
    .long     0                         /* 111 Reserved interrupt */
    .long     0                         /* 112 Reserved interrupt */
    .long     0                         /* 113 Reserved interrupt */
    .long     0                         /* 114 Reserved interrupt */
    .long     0                         /* 115 Reserved interrupt */
    .long     0                         /* 116 Reserved interrupt */
    .long     0                         /* 117 Reserved interrupt */
    .long     0                         /* 118 Reserved interrupt */
    .long     0                         /* 119 Reserved interrupt */
    .long     0                         /* 120 Reserved interrupt */
    .long     0                         /* 121 Reserved interrupt */
    .long     0                         /* 122 Reserved interrupt */
    .long     0                         /* 123 Reserved interrupt */
    .long     0                         /* 124 Reserved interrupt */
    .long     0                         /* 125 Reserved interrupt */
    .long     0                         /* 126 Reserved interrupt */
    .long     0                         /* 127 Reserved interrupt */
    .long     0                         /* 128 Reserved interrupt */
    .long     0                         /* 129 Reserved interrupt */
    .long     0                         /* 130 Reserved interrupt */
    .long     0                         /* 131 Reserved interrupt */
    .long     0                         /* 132 Reserved interrupt */
    .long     0                         /* 133 Reserved interrupt */
    .long     0                         /* 134 Reserved interrupt */
    .long     0                         /* 135 Reserved interrupt */
    .long     0                         /* 136 Reserved interrupt */
    .long     0                         /* 137 Reserved interrupt */
    .long     0                         /* 138 Reserved interrupt */
    .long     0                         /* 139 Reserved interrupt */
    .long     0                         /* 140 Reserved interrupt */
    .long     0                         /* 141 Reserved interrupt */
    .long     0                         /* 142 Reserved interrupt */
    .long     0                         /* 143 Reserved interrupt */
    .long     0                         /* 144 Reserved interrupt */
    .long     0                         /* 145 Reserved interrupt */
    .long     0                         /* 146 Reserved interrupt */
    .long     0                         /* 147 Reserved interrupt */
    .long     0                         /* 148 Reserved interrupt */
    .long     0                         /* 149 Reserved interrupt */
    .long     0                         /* 150 Reserved interrupt */
    .long     0                         /* 151 Reserved interrupt */
    .long     0                         /* 152 Reserved interrupt */
    .long     0                         /* 153 Reserved interrupt */
    .long     0                         /* 154 Reserved interrupt */
    .long     0                         /* 155 Reserved interrupt */
    .long     0                         /* 156 Reserved interrupt */
    .long     0                         /* 157 Reserved interrupt */
    .long     0                         /* 158 Reserved interrupt */
    .long     0                         /* 159 Reserved interrupt */
    .long     0                         /* 160 Reserved interrupt */
    .long     0                         /* 161 Reserved interrupt */
    .long     0                         /* 162 Reserved interrupt */
    .long     0                         /* 163 Reserved interrupt */
    .long     INT_SHA_IRQHandler        /* 164 */
    .long     0                         /* 165 Reserved interrupt */
    .long     0                         /* 166 Reserved interrupt */
    .long     INT_AES_IRQHandler        /* 167 */
    .long     0                         /* 168 Reserved interrupt */
    .long     INT_DES_IRQHandler        /* 169 */
    .long     0                         /* 170 Reserved interrupt */
    .long     0                         /* 171 Reserved interrupt */
    .long     0                         /* 172 Reserved interrupt */
    .long     0                         /* 173 Reserved interrupt */
    .long     0                         /* 174 Reserved interrupt */
    .long     INT_MMCHS_IRQHandler      /* 175 */
    .long     0                         /* 176 Reserved interrupt */
    .long     INT_I2S_IRQHandler        /* 177 */
    .long     0                         /* 178 Reserved interrupt */
    .long     INT_CAMERA_IRQHandler     /* 179 */
    .long     0                         /* 180 Reserved interrupt */
    .long     0                         /* 181 Reserved interrupt */
    .long     0                         /* 182 Reserved interrupt */
    .long     0                         /* 183 Reserved interrupt */
    .long     0                         /* 184 Reserved interrupt */
    .long     0                         /* 185 Reserved interrupt */
    .long     0                         /* 186 Reserved interrupt */
    .long     INT_NWPIC_IRQHandler      /* 187 */
    .long     INT_PRCM_IRQHandler       /* 188 */
    .long     0                         /* 189 Reserved interrupt */
    .long     0                         /* 190 Reserved interrupt */
    .long     INT_SSPI_IRQHandler       /* 191 */
    .long     INT_GSPI_IRQHandler       /* 192 */
    .long     INT_LSPI_IRQHandler       /* 193 */
    .long     0                         /* 194 Reserved interrupt */
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

#ifndef __NO_SYSTEM_INIT
    bl    SystemInit
#endif

#ifndef __START
#define __START _start
#endif
    bl    __START

    .pool
    .size    Reset_Handler, . - Reset_Handler

    .align    1
    .thumb_func
    .weak    Default_Handler
    .type    Default_Handler, %function
Default_Handler:
    b    .
    .size    Default_Handler, . - Default_Handler

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro    def_irq_handler    handler_name
    .weak    \handler_name
    .set    \handler_name, Default_Handler
    .endm

    def_irq_handler     NMI_Handler
    def_irq_handler     HardFault_Handler
    def_irq_handler     MemManage_Handler
    def_irq_handler     BusFault_Handler
    def_irq_handler     UsageFault_Handler
    def_irq_handler     SVC_Handler
    def_irq_handler     DebugMon_Handler
    def_irq_handler     PendSV_Handler
    def_irq_handler     SysTick_Handler

    def_irq_handler    INT_GPIOA0_IRQHandler
    def_irq_handler    INT_GPIOA1_IRQHandler
    def_irq_handler    INT_GPIOA2_IRQHandler
    def_irq_handler    INT_GPIOA3_IRQHandler
    def_irq_handler    INT_UARTA0_IRQHandler
    def_irq_handler    INT_UARTA1_IRQHandler
    def_irq_handler    INT_I2CA0_IRQHandler
    def_irq_handler    INT_ADCCH0_IRQHandler
    def_irq_handler    INT_ADCCH1_IRQHandler
    def_irq_handler    INT_ADCCH2_IRQHandler
    def_irq_handler    INT_ADCCH3_IRQHandler
    def_irq_handler    INT_WDT_IRQHandler
    def_irq_handler    INT_TIMERA0A_IRQHandler
    def_irq_handler    INT_TIMERA0B_IRQHandler
    def_irq_handler    INT_TIMERA1A_IRQHandler
    def_irq_handler    INT_TIMERA1B_IRQHandler
    def_irq_handler    INT_TIMERA2A_IRQHandler
    def_irq_handler    INT_TIMERA2B_IRQHandler
    def_irq_handler    INT_FLASH_IRQHandler
    def_irq_handler    INT_TIMERA3A_IRQHandler
    def_irq_handler    INT_TIMERA3B_IRQHandler
    def_irq_handler    INT_UDMA_IRQHandler
    def_irq_handler    INT_UDMAERR_IRQHandler
    def_irq_handler    INT_SHA_IRQHandler
    def_irq_handler    INT_AES_IRQHandler
    def_irq_handler    INT_DES_IRQHandler
    def_irq_handler    INT_MMCHS_IRQHandler
    def_irq_handler    INT_I2S_IRQHandler
    def_irq_handler    INT_CAMERA_IRQHandler
    def_irq_handler    INT_NWPIC_IRQHandler
    def_irq_handler    INT_PRCM_IRQHandler
    def_irq_handler    INT_SSPI_IRQHandler
    def_irq_handler    INT_GSPI_IRQHandler
    def_irq_handler    INT_LSPI_IRQHandler
    .end
