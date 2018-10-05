/* 
 * Copyright (c) 2013 Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */


/*
NOTE: Template files (including this one) are application specific and therefore
expected to be copied into the application project folder prior to its use!
*/

    .syntax unified
    .arch armv6-m

    .section .stack
    .align 3
#ifdef __STACK_SIZE
    .equ    Stack_Size, __STACK_SIZE
#else
    .equ    Stack_Size, 2048
#endif
    .globl    Stack_Size
    .globl    __StackTop
    .globl    __StackLimit
__StackLimit:
    .space    Stack_Size
    .size __StackLimit, . - __StackLimit
__StackTop:
    .size __StackTop, . - __StackTop

    .section .heap
    .align 3
    .globl    __HeapBase
    .globl    __HeapLimit

    .section .Vectors
    .align 2
    .globl __Vectors
__Vectors:
    .long    __StackTop            /* Top of Stack */
    .long   Reset_Handler               /* Reset Handler */
    .long   NMI_Handler                 /* NMI Handler */
    .long   HardFault_Handler           /* Hard Fault Handler */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   SVC_Handler                 /* SVCall Handler */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   PendSV_Handler              /* PendSV Handler */
    .long   SysTick_Handler             /* SysTick Handler */

  /* External Interrupts */
    .long 	POWER_CLOCK_IRQHandler		 /*POWER_CLOCK */
    .long 	RADIO_IRQHandler		 /*RADIO */
    .long 	UART0_IRQHandler_v		 /*UART0 */
    .long 	SPI0_TWI0_IRQHandler_v		 /*SPI0_TWI0 */
    .long 	SPI1_TWI1_IRQHandler_v		 /*SPI1_TWI1 */
    .long 	0		 /*Reserved */
    .long 	GPIOTE_IRQHandler_v		 /*GPIOTE */
    .long 	ADC_IRQHandler_v		 /*ADC */
    .long 	TIMER0_IRQHandler		 /*TIMER0 */
    .long 	TIMER1_IRQHandler_v	 /*TIMER1 */
    .long 	TIMER2_IRQHandler_v		 /*TIMER2 */
    .long 	RTC0_IRQHandler		 /*RTC0 */
    .long 	TEMP_IRQHandler		 /*TEMP */
    .long 	RNG_IRQHandler		 /*RNG */
    .long 	ECB_IRQHandler		 /*ECB */
    .long 	CCM_AAR_IRQHandler		 /*CCM_AAR */
    .long 	WDT_IRQHandler_v		 /*WDT */
    .long 	RTC1_IRQHandler_v		 /*RTC1 */
    .long 	QDEC_IRQHandler_v		 /*QDEC */
    .long 	LPCOMP_IRQHandler_v		 /*LPCOMP */
    .long 	SWI0_IRQHandler_v		 /*SWI0 */
    .long 	SWI1_IRQHandler_v		 /*SWI1 */
    .long 	SWI2_IRQHandler_v		 /*SWI2 */
    .long 	SWI3_IRQHandler_v		 /*SWI3 */
    .long 	SWI4_IRQHandler		 /*SWI4 */
    .long 	SWI5_IRQHandler		 /*SWI5 */


    .size    __Vectors, . - __Vectors

/* Reset Handler */

    .equ    NRF_POWER_RAMON_ADDRESS,             0x40000524
    .equ    NRF_POWER_RAMONB_ADDRESS,            0x40000554
    .equ    NRF_POWER_RAMONx_RAMxON_ONMODE_Msk,  0x3  

    .text
    .thumb
    .thumb_func
    .align 1
    .globl    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
    .fnstart

    MOVS    R1, #NRF_POWER_RAMONx_RAMxON_ONMODE_Msk
    LDR     R0, =NRF_POWER_RAMON_ADDRESS
    LDR     R2, [R0]
    ORRS    R2, R1
    STR     R2, [R0]

    LDR     R0, =NRF_POWER_RAMONB_ADDRESS
    LDR     R2, [R0]
    ORRS    R2, R1
    STR     R2, [R0]

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
    ble     .LC0

.LC1:
    subs    r3, 4
    ldr    r0, [r1,r3]
    str    r0, [r2,r3]
    bgt    .LC1
.LC0:

    LDR     R0, =SystemInit
    BLX     R0
    LDR     R0, =nrf_reloc_vector_table
    BLX     R0
    LDR     R0, =_start
    BX      R0

    .pool
    .cantunwind
    .fnend
    .size   Reset_Handler,.-Reset_Handler

    .section ".text"


/* Dummy Exception Handlers (infinite loops which can be modified) */

    .weak   NMI_Handler
    .type   NMI_Handler, %function
NMI_Handler:
    B       .
    .size   NMI_Handler, . - NMI_Handler


    .weak   HardFault_Handler
    .type   HardFault_Handler, %function
HardFault_Handler:
    B       .
    .size   HardFault_Handler, . - HardFault_Handler


    .weak   SVC_Handler
    .type   SVC_Handler, %function
SVC_Handler:
    B       .
    .size   SVC_Handler, . - SVC_Handler


    .weak   PendSV_Handler
    .type   PendSV_Handler, %function
PendSV_Handler:
    B       .
    .size   PendSV_Handler, . - PendSV_Handler


    .weak   SysTick_Handler
    .type   SysTick_Handler, %function
SysTick_Handler:
    B       .
    .size   SysTick_Handler, . - SysTick_Handler


/* IRQ Handlers */

    .globl  Default_Handler
    .type   Default_Handler, %function
Default_Handler:
    B       .
    .size   Default_Handler, . - Default_Handler

    .macro  IRQ handler
    .weak   \handler
    .set    \handler, Default_Handler
    .endm

    IRQ  POWER_CLOCK_IRQHandler     /* restricted */
    IRQ  RADIO_IRQHandler           /* blocked */
    IRQ  UART0_IRQHandler_v
    IRQ  SPI0_TWI0_IRQHandler_v
    IRQ  SPI1_TWI1_IRQHandler_v
    IRQ  GPIOTE_IRQHandler_v
    IRQ  ADC_IRQHandler_v
    IRQ  TIMER0_IRQHandler          /* blocked */
    IRQ  TIMER1_IRQHandler_v
    IRQ  TIMER2_IRQHandler_v
    IRQ  RTC0_IRQHandler          /* blocked */
    IRQ  TEMP_IRQHandler          /* restricted */
    IRQ  RNG_IRQHandler           /* restricted */
    IRQ  ECB_IRQHandler           /* restricted */
    IRQ  CCM_AAR_IRQHandler       /* blocked */
    IRQ  WDT_IRQHandler_v
    IRQ  RTC1_IRQHandler_v
    IRQ  QDEC_IRQHandler_v
    IRQ  LPCOMP_IRQHandler_v
    IRQ  SWI0_IRQHandler_v
    IRQ  SWI1_IRQHandler_v         /* restricted for Radio Notification */
    IRQ  SWI2_IRQHandler_v         /* blocked for SoftDevice Event */
    IRQ  SWI3_IRQHandler_v
    IRQ  SWI4_IRQHandler         /* blocked */
    IRQ  SWI5_IRQHandler         /* blocked */


  .end
