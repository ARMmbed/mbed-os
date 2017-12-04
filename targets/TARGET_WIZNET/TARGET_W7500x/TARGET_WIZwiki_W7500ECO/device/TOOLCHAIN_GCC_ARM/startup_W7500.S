/* File: startup_W7500.s
 * Purpose: startup file for Cortex-M0 devices. Should use with
 *   GCC for ARM Embedded Processors
 * Version: V1.4
 * Date: 20 Dezember 2012
 *
 */
/* Copyright (c) 2011 - 2012 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/


    .syntax unified
    .arch armv6-m

    .section .stack
    .align 3

/*
// <h> Stack Configuration
//   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
// </h>
*/

    .section .stack
    .align 3
#ifdef __STACK_SIZE
    .equ    Stack_Size, __STACK_SIZE
#else
    .equ    Stack_Size, 0x200
#endif
    .globl    __StackTop
    .globl    __StackLimit
__StackLimit:
    .space    Stack_Size
    .size __StackLimit, . - __StackLimit
__StackTop:
    .size __StackTop, . - __StackTop


/*
// <h> Heap Configuration
//   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
// </h>
*/

    .section .heap
    .align 3
#ifdef __HEAP_SIZE
    .equ    Heap_Size, __HEAP_SIZE
#else
    .equ    Heap_Size, 0
#endif
    .globl    __HeapBase
    .globl    __HeapLimit
__HeapBase:
    .if    Heap_Size
    .space    Heap_Size
    .endif
    .size __HeapBase, . - __HeapBase
__HeapLimit:
    .size __HeapLimit, . - __HeapLimit


/* Vector Table */

    .section .isr_vector
    .align 2
    .globl __isr_vector
__isr_vector:
    .long   __StackTop                  /* Top of Stack                  */
    .long   Reset_Handler               /* Reset Handler                 */
    .long   NMI_Handler                 /* NMI Handler                   */
    .long   HardFault_Handler           /* Hard Fault Handler            */
    .long   0                           /* Reserved                      */
    .long   0                           /* Reserved                      */
    .long   0                           /* Reserved                      */
    .long   0                           /* Reserved                      */
    .long   0                           /* Reserved                      */
    .long   0                           /* Reserved                      */
    .long   0                           /* Reserved                      */
    .long   SVC_Handler                 /* SVCall Handler                */
    .long   0                           /* Debug Monitor Handler         */
    .long   0                           /* Reserved                      */
    .long   PendSV_Handler              /* PendSV Handler                */
    .long   SysTick_Handler             /* SysTick Handler               */

    /* External Interrupts */
    .long   SSP0_Handler                /* 16+ 0: SSP 0 Handler                   */
    .long   SSP1_Handler                /* 16+ 1: SSP 1 Handler                   */
    .long   UART0_Handler               /* 16+ 2: UART 0 Handler                  */
    .long   UART1_Handler               /* 16+ 3: UART 1 Handler                  */
    .long   UART2_Handler               /* 16+ 4: UART 2 Handler                  */
    .long   I2C0_Handler                /* 16+ 5: I2C 0 Handler                   */
    .long   I2C1_Handler                /* 16+ 6: I2C 1 Handler                   */
    .long   PORT0_Handler               /* 16+ 7: GPIO Port 0 Combined Handler    */
    .long   PORT1_Handler               /* 16+ 8: GPIO Port 1 Combined Handler    */
    .long   PORT2_Handler               /* 16+ 9: GPIO Port 2 Combined Handler    */
    .long   PORT3_Handler               /* 16+10: GPIO Port 3 Combined Handler    */
    .long   DMA_Handler		            /* 16+11: DMA Combined Handler            */
    .long   DUALTIMER0_Handler          /* 16+12: Dual timer 0 handler            */ 
    .long   DUALTIMER1_Handler		     /* 16+ 13: Dual timer 1 Handler	*/
    .long   PWM0_Handler		            /* 16+ 14: PWM0 Handler		*/
    .long   PWM1_Handler		            /* 16+ 15: PWM1 Handler		*/
    .long   PWM2_Handler		            /* 16+ 16: PWM2 Handler		*/
    .long   PWM3_Handler		            /* 16+ 17: PWM3 Handler		*/
    .long   PWM4_Handler		            /* 16+ 18: PWM4 Handler		*/
    .long   PWM5_Handler		            /* 16+ 19: PWM5 Handler		*/
    .long   PWM6_Handler		            /* 16+ 20: PWM6 Handler		*/
    .long   PWM7_Handler		            /* 16+ 21: PWM7 Handler		*/
    .long   RTC_Handler		            /* 16+ 22: RTC Handler			*/
    .long   ADC_Handler		            /* 16+ 23: ADC Handler			*/
    .long   WZTOE_Handler               /* 16+ 24: WZTOE Handler		*/
    .long   EXTI_Handler                /* 16+ 25: EXTI Handler       */

    .size    __isr_vector, . - __isr_vector
/* Reset Handler */
    .text
    .thumb
    .thumb_func
    .align 2
    .globl    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
/*     Loop to copy data from read only memory to RAM. The ranges
 *      of copy from/to are specified by following symbols evaluated in
 *      linker script.
 *      __etext: End of code section, i.e., begin of data sections to copy from.
 *      __data_start__/__data_end__: RAM address range that data should be
 *      copied to. Both must be aligned to 4 bytes boundary.  */

    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

    subs    r3, r2
    ble    .LC1
.LC0:
    subs    r3, #4
    ldr    r0, [r1, r3]
    str    r0, [r2, r3]
    bgt    .LC0
.LC1:

#ifdef __STARTUP_CLEAR_BSS
/*     This part of work usually is done in C library startup code. Otherwise,
 *     define this macro to enable it in this startup.
 *
 *     Loop to zero out BSS section, which uses following symbols
 *     in linker script:
 *      __bss_start__: start of BSS section. Must align to 4
 *      __bss_end__: end of BSS section. Must align to 4
 */
    ldr r1, =__bss_start__
    ldr r2, =__bss_end__

    subs    r2, r1
    ble .LC3

    movs    r0, 0
.LC2:
    str r0, [r1, r2]
    subs    r2, 4
    bge .LC2
.LC3:
#endif /* __STARTUP_CLEAR_BSS */

    bl    _start
    //bl main

    .pool
    .size Reset_Handler, . - Reset_Handler

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

/* System Exception Handlers */

    def_default_handler    NMI_Handler
    def_default_handler    HardFault_Handler
    def_default_handler    MemManage_Handler
    def_default_handler    BusFault_Handler
    def_default_handler    UsageFault_Handler
    def_default_handler    SVC_Handler
    def_default_handler    DebugMon_Handler
    def_default_handler    PendSV_Handler
    def_default_handler    SysTick_Handler

/* IRQ Handlers */

    def_default_handler    SSP0_Handler
    def_default_handler    SSP1_Handler
    def_default_handler    UART0_Handler
    def_default_handler    UART1_Handler
    def_default_handler    UART2_Handler
    def_default_handler    I2C0_Handler
    def_default_handler    I2C1_Handler
    def_default_handler    PORT0_Handler
    def_default_handler    PORT1_Handler
    def_default_handler    PORT2_Handler
    def_default_handler    PORT3_Handler

    def_default_handler    DMA_Handler
    def_default_handler    DUALTIMER0_Handler
    def_default_handler    DUALTIMER1_Handler
    def_default_handler    PWM0_Handler
    def_default_handler    PWM1_Handler
    def_default_handler    PWM2_Handler
    def_default_handler    PWM3_Handler
    def_default_handler    PWM4_Handler
    def_default_handler    PWM5_Handler
    def_default_handler    PWM6_Handler
    def_default_handler    PWM7_Handler
    def_default_handler    RTC_Handler
    def_default_handler    ADC_Handler
    def_default_handler    WZTOE_Handler
    def_default_handler    EXTI_Handler
    
    /*
    def_default_handler    Default_Handler
    .weak    DEF_IRQHandler
    .set    DEF_IRQHandler, Default_Handler
    */

    .end

