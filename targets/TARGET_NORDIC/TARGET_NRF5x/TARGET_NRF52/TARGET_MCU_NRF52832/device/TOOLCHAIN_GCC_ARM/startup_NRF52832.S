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
    .arch armv7e-m

    .section .Vectors
    .align 2
    .globl __Vectors
__Vectors:
    .long   __StackTop                  /* Top of Stack */
    .long   Reset_Handler
    .long   NMI_Handler
    .long   HardFault_Handler
    .long   MemoryManagement_Handler
    .long   BusFault_Handler
    .long   UsageFault_Handler
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   SVC_Handler
    .long   DebugMon_Handler
    .long   0                           /*Reserved */
    .long   PendSV_Handler
    .long   SysTick_Handler

  /* External Interrupts */
    .long   POWER_CLOCK_IRQHandler
    .long   RADIO_IRQHandler
    .long   UARTE0_UART0_IRQHandler_v
    .long   SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler_v
    .long   SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler_v
    .long   NFCT_IRQHandler_v
    .long   GPIOTE_IRQHandler_v
    .long   SAADC_IRQHandler_v
    .long   TIMER0_IRQHandler_v
    .long   TIMER1_IRQHandler_v
    .long   TIMER2_IRQHandler_v
    .long   RTC0_IRQHandler
    .long   TEMP_IRQHandler_v
    .long   RNG_IRQHandler
    .long   ECB_IRQHandler
    .long   CCM_AAR_IRQHandler
    .long   WDT_IRQHandler_v
    .long   RTC1_IRQHandler_v
    .long   QDEC_IRQHandler_v
    .long   COMP_LPCOMP_IRQHandler_v
    .long   SWI0_EGU0_IRQHandler_v
    .long   SWI1_EGU1_IRQHandler_v
    .long   SWI2_EGU2_IRQHandler_v
    .long   SWI3_EGU3_IRQHandler_v
    .long   SWI4_EGU4_IRQHandler
    .long   SWI5_EGU5_IRQHandler
    .long   TIMER3_IRQHandler_v
    .long   TIMER4_IRQHandler_v
    .long   PWM0_IRQHandler_v
    .long   PDM_IRQHandler_v
    .long   0                           /*Reserved */
    .long   0                           /*Reserved */
    .long   MWU_IRQHandler
    .long   PWM1_IRQHandler_v
    .long   PWM2_IRQHandler_v
    .long   SPIM2_SPIS2_SPI2_IRQHandler_v
    .long   RTC2_IRQHandler_v
    .long   I2S_IRQHandler_v
    .long   FPU_IRQHandler_v


    .size    __Vectors, . - __Vectors

/* Reset Handler */

    .text
    .thumb
    .thumb_func
    .align 1
    .globl    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
    .fnstart


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


    .weak   MemoryManagement_Handler
    .type   MemoryManagement_Handler, %function
MemoryManagement_Handler:
    B       .
    .size   MemoryManagement_Handler, . - MemoryManagement_Handler


    .weak   BusFault_Handler
    .type   BusFault_Handler, %function
BusFault_Handler:
    B       .
    .size   BusFault_Handler, . - BusFault_Handler


    .weak   UsageFault_Handler
    .type   UsageFault_Handler, %function
UsageFault_Handler:
    B       .
    .size   UsageFault_Handler, . - UsageFault_Handler


    .weak   SVC_Handler
    .type   SVC_Handler, %function
SVC_Handler:
    B       .
    .size   SVC_Handler, . - SVC_Handler


    .weak   DebugMon_Handler
    .type   DebugMon_Handler, %function
DebugMon_Handler:
    b       .
    .size   DebugMon_Handler, . - DebugMon_Handler


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

    IRQ  POWER_CLOCK_IRQHandler                          /* restricted */
    IRQ  RADIO_IRQHandler                                /* blocked    */
    IRQ  UARTE0_UART0_IRQHandler_v
    IRQ  SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler_v
    IRQ  SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler_v
    IRQ  NFCT_IRQHandler_v
    IRQ  GPIOTE_IRQHandler_v
    IRQ  SAADC_IRQHandler_v
    IRQ  TIMER0_IRQHandler_v
    IRQ  TIMER1_IRQHandler_v
    IRQ  TIMER2_IRQHandler_v
    IRQ  RTC0_IRQHandler                                 /* blocked    */
    IRQ  TEMP_IRQHandler_v
    IRQ  RNG_IRQHandler                                  /* restricted */
    IRQ  ECB_IRQHandler                                  /* restricted */
    IRQ  CCM_AAR_IRQHandler                              /* blocked    */
    IRQ  WDT_IRQHandler_v
    IRQ  RTC1_IRQHandler_v
    IRQ  QDEC_IRQHandler_v
    IRQ  COMP_LPCOMP_IRQHandler_v
    IRQ  SWI0_EGU0_IRQHandler_v
    IRQ  SWI1_EGU1_IRQHandler_v                          /* restricted for Radio Notification */
    IRQ  SWI2_EGU2_IRQHandler_v                          /* blocked for SoftDevice Event */
    IRQ  SWI3_EGU3_IRQHandler_v
    IRQ  SWI4_EGU4_IRQHandler                            /* blocked    */
    IRQ  SWI5_EGU5_IRQHandler                            /* blocked    */
    IRQ  TIMER3_IRQHandler_v
    IRQ  TIMER4_IRQHandler_v
    IRQ  PWM0_IRQHandler_v
    IRQ  PDM_IRQHandler_v
    IRQ  MWU_IRQHandler                                  /* restricted */
    IRQ  PWM1_IRQHandler_v
    IRQ  PWM2_IRQHandler_v
    IRQ  SPIM2_SPIS2_SPI2_IRQHandler_v
    IRQ  RTC2_IRQHandler_v
    IRQ  I2S_IRQHandler_v
    IRQ  FPU_IRQHandler_v
    
  .end
