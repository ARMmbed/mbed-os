/* File: startup_ARMCM3.s
 * Purpose: startup file for Cortex-M3/M4 devices. Should use with 
 *   GNU Tools for ARM Embedded Processors
 * Version: V1.1
 * Date: 17 June 2011
 * 
 * Copyright (c) 2011, 2012, ARM Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the ARM Limited nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ARM LIMITED BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
    .syntax unified
    .arch armv7-m

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
    
    .section .isr_vector
    .align 2
    .globl __isr_vector
__isr_vector:
    .long    __StackTop                 /* Top of Stack */
    .long    Reset_Handler              /* Reset Handler */
    .long    NMI_Handler                /* NMI Handler */
    .long    HardFault_Handler          /* Hard Fault Handler */
    .long    MemManage_Handler          /* MPU Fault Handler */
    .long    BusFault_Handler           /* Bus Fault Handler */
    .long    UsageFault_Handler         /* Usage Fault Handler */
    .long    0                          /* Reserved */
    .long    0                          /* Reserved */
    .long    0                          /* Reserved */
    .long    0                          /* Reserved */
    .long    SVC_Handler                /* SVCall Handler */
    .long    DebugMon_Handler           /* Debug Monitor Handler */
    .long    0                          /* Reserved */
    .long    PendSV_Handler             /* PendSV Handler */
    .long    SysTick_Handler            /* SysTick Handler */
    .long    WWDG_IRQHandler            /* Window Watchdog */

     /* External interrupts */
    .long    PVD_IRQHandler             /* PVD through EXTI Line detect */
    .long    TAMPER_IRQHandler          /* Tamper */
    .long    RTC_IRQHandler             /* RTC */
    .long    FLASH_IRQHandler           /* Flash */
    .long    RCC_IRQHandler             /* RCC */
    .long    EXTI0_IRQHandler           /* EXTI Line 0 */
    .long    EXTI1_IRQHandler           /* EXTI Line 1 */
    .long    EXTI2_IRQHandler           /* EXTI Line 2 */
    .long    EXTI3_IRQHandler           /* EXTI Line 3 */
    .long    EXTI4_IRQHandler           /* EXTI Line 4 */
    .long    DMA1_Channel1_IRQHandler   /* DMA1 Channel 1 */
    .long    DMA1_Channel2_IRQHandler   /* DMA1 Channel 2 */
    .long    DMA1_Channel3_IRQHandler   /* DMA1 Channel 3 */
    .long    DMA1_Channel4_IRQHandler   /* DMA1 Channel 4 */
    .long    DMA1_Channel5_IRQHandler   /* DMA1 Channel 5 */
    .long    DMA1_Channel6_IRQHandler   /* DMA1 Channel 6 */
    .long    DMA1_Channel7_IRQHandler   /* DMA1 Channel 7 */
    .long    ADC1_2_IRQHandler          /* ADC1_2 */
    .long    USB_HP_CAN1_TX_IRQHandler  /* USB High Priority or CAN1 TX */
    .long    USB_LP_CAN1_RX0_IRQHandler /* USB Low  Priority or CAN1 RX0 */
    .long    CAN1_RX1_IRQHandler        /* CAN1 RX1 */
    .long    CAN1_SCE_IRQHandler        /* CAN1 SCE */
    .long    EXTI9_5_IRQHandler         /* EXTI Line 9..5 */
    .long    TIM1_BRK_IRQHandler        /* TIM1 Break */
    .long    TIM1_UP_IRQHandler         /* TIM1 Update */
    .long    TIM1_TRG_COM_IRQHandler    /* TIM1 Trigger and Commutation */
    .long    TIM1_CC_IRQHandler         /* TIM1 Capture Compare */
    .long    TIM2_IRQHandler            /* TIM2 */
    .long    TIM3_IRQHandler            /* TIM3 */
    .long    TIM4_IRQHandler            /* TIM4 */
    .long    I2C1_EV_IRQHandler         /* I2C1 Event */
    .long    I2C1_ER_IRQHandler         /* I2C1 Error */
    .long    I2C2_EV_IRQHandler         /* I2C2 Event */
    .long    I2C2_ER_IRQHandler         /* I2C2 Error */
    .long    SPI1_IRQHandler            /* SPI1 */
    .long    SPI2_IRQHandler            /* SPI2 */
    .long    USART1_IRQHandler          /* USART1 */
    .long    USART2_IRQHandler          /* USART2 */
    .long    USART3_IRQHandler          /* USART3 */
    .long    EXTI15_10_IRQHandler       /* EXTI Line 15..10 */
    .long    RTCAlarm_IRQHandler        /* RTC Alarm through EXTI Line */
    .long    USBWakeUp_IRQHandler       /* USB Wakeup from suspend */

    .size    __isr_vector, . - __isr_vector

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

.LC0:
    cmp     r2, r3
    ittt    lt
    ldrlt   r0, [r1], #4
    strlt   r0, [r2], #4
    blt    .LC0

    ldr    r0, =SystemInit
    blx    r0
    ldr    r0, =_start
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
 
    def_irq_default_handler    WWDG_IRQHandler 
    def_irq_default_handler    PVD_IRQHandler 
    def_irq_default_handler    TAMPER_IRQHandler
    def_irq_default_handler    RTC_IRQHandler
    def_irq_default_handler    FLASH_IRQHandler
    def_irq_default_handler    RCC_IRQHandler
    def_irq_default_handler    EXTI0_IRQHandler
    def_irq_default_handler    EXTI1_IRQHandler
    def_irq_default_handler    EXTI2_IRQHandler
    def_irq_default_handler    EXTI3_IRQHandler
    def_irq_default_handler    EXTI4_IRQHandler
    def_irq_default_handler    DMA1_Channel1_IRQHandler
    def_irq_default_handler    DMA1_Channel2_IRQHandler
    def_irq_default_handler    DMA1_Channel3_IRQHandler  
    def_irq_default_handler    DMA1_Channel4_IRQHandler  
    def_irq_default_handler    DMA1_Channel5_IRQHandler  
    def_irq_default_handler    DMA1_Channel6_IRQHandler  
    def_irq_default_handler    DMA1_Channel7_IRQHandler  

    def_irq_default_handler    ADC1_2_IRQHandler         
    def_irq_default_handler    USB_HP_CAN1_TX_IRQHandler 
    def_irq_default_handler    USB_LP_CAN1_RX0_IRQHandler
    def_irq_default_handler    CAN1_RX1_IRQHandler       
    def_irq_default_handler    CAN1_SCE_IRQHandler       
    def_irq_default_handler    EXTI9_5_IRQHandler        
    def_irq_default_handler    TIM1_BRK_IRQHandler       
    def_irq_default_handler    TIM1_UP_IRQHandler        
    def_irq_default_handler    TIM1_TRG_COM_IRQHandler   
    def_irq_default_handler    TIM1_CC_IRQHandler        
    def_irq_default_handler    TIM2_IRQHandler           
    def_irq_default_handler    TIM3_IRQHandler           
    def_irq_default_handler    TIM4_IRQHandler           
    def_irq_default_handler    I2C1_EV_IRQHandler        
    def_irq_default_handler    I2C1_ER_IRQHandler        
    def_irq_default_handler    I2C2_EV_IRQHandler        
    def_irq_default_handler    I2C2_ER_IRQHandler        
    def_irq_default_handler    SPI1_IRQHandler           
    def_irq_default_handler    SPI2_IRQHandler           
    def_irq_default_handler    USART1_IRQHandler         
    def_irq_default_handler    USART2_IRQHandler         
    def_irq_default_handler    USART3_IRQHandler         
    def_irq_default_handler    EXTI15_10_IRQHandler      
    def_irq_default_handler    RTCAlarm_IRQHandler       
    def_irq_default_handler    USBWakeUp_IRQHandler      

    .end

