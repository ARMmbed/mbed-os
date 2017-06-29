/* File: startup_ncs36510.S
 * Purpose: startup file for Cortex-M3 devices. Should use with
 *   GCC for ARM Embedded Processors
 * Version: V2.00
 * Date: 15 Jan 2016
 *
 */
/* Copyright (c) 2011 - 2014 ARM LIMITED

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


.syntax    unified
.arch    armv7-m

.section .stack
.align 3
#ifdef __STACK_SIZE
.equ    Stack_Size, __STACK_SIZE
#else
.equ    Stack_Size, 0x400
#endif
.globl    __StackTop
.globl    __StackLimit
__StackLimit:
.space    Stack_Size
.size __StackLimit, . - __StackLimit
__StackTop:
.size __StackTop, . - __StackTop

.section .heap
.align 3
#ifdef __HEAP_SIZE
.equ    Heap_Size, __HEAP_SIZE
#else
.equ    Heap_Size, 0x400
#endif
.globl    __HeapBase
.globl    __HeapLimit
__HeapBase:
.space    Heap_Size
.size __HeapBase, . - __HeapBase
__HeapLimit:
.size __HeapLimit, . - __HeapLimit

.section .vector_table,"a",%progbits
.align 2
.globl __Vectors
__Vectors:
.long   __StackTop                  /* Top of Stack */
.long   Reset_Handler               /* Reset Handler */
.long   NMI_Handler                 /* NMI Handler                  */
.long   HardFault_Handler           /* Hard Fault Handler           */
.long   MemManage_Handler           /* MPU Fault Handler            */
.long   BusFault_Handler            /* Bus Fault Handler            */
.long   UsageFault_Handler          /* Usage Fault Handler          */
.long   0                           /* Reserved                     */
.long   0                           /* Reserved                     */
.long   0                           /* Reserved                     */
.long   0                           /* Reserved                     */
.long   SVC_Handler                 /* SVCall Handler               */
.long   DebugMon_Handler            /* Debug Monitor Handler        */
.long   0                           /* Reserved                     */
.long   PendSV_Handler              /* PendSV Handler               */
.long   SysTick_Handler             /* SysTick Handler              */

/* External Interrupts */
.long    fIrqTim0Handler
.long    fIrqTim1Handler
.long    fIrqTim2Handler
.long    fIrqUart1Handler
.long    fIrqSpiHandler
.long    fIrqI2CHandler
.long    fIrqGpioHandler
.long    fIrqRtcHandler
.long    fIrqFlashHandler
.long    fIrqMacHwHandler
.long    fIrqAesHandler
.long    fIrqAdcHandler
.long    fIrqClockCalHandler
.long    fIrqUart2Handler
.long    fIrqUviHandler
.long    fIrqDmaHandler
.long    fIrqDbgPwrUpHandler
.long    fIrqSpi2Handler
.long    fIrqI2C2Handler
.long    fIrqFVDDHCompHandler

.size    __Vectors, . - __Vectors

.section .text.Reset_Handler
.thumb
.thumb_func
.align  2
.globl   Reset_Handler
.type    Reset_Handler, %function
Reset_Handler:
/*     Loop to copy data from read only memory to RAM. The ranges
*      of copy from/to are specified by following symbols evaluated in
*      linker script.
*      __etext: End of code section, i.e., begin of data sections to copy from.
*      __data_start__/__data_end__: RAM address range that data should be
*      copied to. Both must be aligned to 4 bytes boundary.  */

disable_watchdog:
/*MPL - Need to implement?! */

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

  ldr   r0, =SystemInit
  blx   r0
/* TODO -  Uncomment when uvisor support is added */
/*
  ldr   r0, =uvisor_init
  blx   r0
*/
  ldr   r0, =_start
  bx    r0
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

/* Exception Handlers */

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

/* IRQ Handlers */
  def_irq_default_handler     fIrqTim0Handler
  def_irq_default_handler     fIrqTim1Handler
  def_irq_default_handler     fIrqTim2Handler
  def_irq_default_handler     fIrqUart1Handler
  def_irq_default_handler     fIrqSpiHandler
  def_irq_default_handler     fIrqI2CHandler
  def_irq_default_handler     fIrqGpioHandler
  def_irq_default_handler     fIrqRtcHandler
  def_irq_default_handler     fIrqFlashHandler
  def_irq_default_handler     fIrqMacHwHandler
  def_irq_default_handler     fIrqAesHandler
  def_irq_default_handler     fIrqAdcHandler
  def_irq_default_handler     fIrqClockCalHandler
  def_irq_default_handler     fIrqUart2Handler
  def_irq_default_handler     fIrqUviHandler
  def_irq_default_handler     fIrqDmaHandler
  def_irq_default_handler     fIrqDbgPwrUpHandler
  def_irq_default_handler     fIrqSpi2Handler
  def_irq_default_handler     fIrqI2C2Handler
  def_irq_default_handler     fIrqFVDDHCompHandler
  def_irq_default_handler     DefaultISR
  
  .end
