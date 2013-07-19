;/**************************************************************************//**
; * @file     startup_LPC11xx.s
; * @brief    CMSIS Cortex-M0 Core Device Startup File
; *           for the NXP LPC11xx/LPC11Cxx Device Series
; * @version  V1.10
; * @date     24. November 2010
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; *
; * @note
; * Copyright (C) 2009-2010 ARM Limited. All rights reserved.
; *
; * @par
; * ARM Limited (ARM) is supplying this software for use with Cortex-M 
; * processor based microcontrollers.  This file can be freely distributed 
; * within development tools that are supporting such ARM based processors. 
; *
; * @par
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     WAKEUP_IRQHandler         ; 16+ 0: Wakeup PIO0.0
                DCD     WAKEUP_IRQHandler         ; 16+ 1: Wakeup PIO0.1
                DCD     WAKEUP_IRQHandler         ; 16+ 2: Wakeup PIO0.2
                DCD     WAKEUP_IRQHandler         ; 16+ 3: Wakeup PIO0.3
                DCD     WAKEUP_IRQHandler         ; 16+ 4: Wakeup PIO0.4
                DCD     WAKEUP_IRQHandler         ; 16+ 5: Wakeup PIO0.5
                DCD     WAKEUP_IRQHandler         ; 16+ 6: Wakeup PIO0.6
                DCD     WAKEUP_IRQHandler         ; 16+ 7: Wakeup PIO0.7
                DCD     WAKEUP_IRQHandler         ; 16+ 8: Wakeup PIO0.8
                DCD     WAKEUP_IRQHandler         ; 16+ 9: Wakeup PIO0.9
                DCD     WAKEUP_IRQHandler         ; 16+10: Wakeup PIO0.10
                DCD     WAKEUP_IRQHandler         ; 16+11: Wakeup PIO0.11
                DCD     WAKEUP_IRQHandler         ; 16+12: Wakeup PIO1.0
                DCD     CAN_IRQHandler            ; 16+13: CAN
                DCD     SSP1_IRQHandler           ; 16+14: SSP1
                DCD     I2C_IRQHandler            ; 16+15: I2C
                DCD     TIMER16_0_IRQHandler      ; 16+16: 16-bit Counter-Timer 0
                DCD     TIMER16_1_IRQHandler      ; 16+17: 16-bit Counter-Timer 1
                DCD     TIMER32_0_IRQHandler      ; 16+18: 32-bit Counter-Timer 0
                DCD     TIMER32_1_IRQHandler      ; 16+19: 32-bit Counter-Timer 1
                DCD     SSP0_IRQHandler           ; 16+20: SSP0
                DCD     UART_IRQHandler           ; 16+21: UART
                DCD     USB_IRQHandler            ; 16+22: USB IRQ
                DCD     USB_FIQHandler            ; 16+24: USB FIQ
                DCD     ADC_IRQHandler            ; 16+24: A/D Converter
                DCD     WDT_IRQHandler            ; 16+25: Watchdog Timer
                DCD     BOD_IRQHandler            ; 16+26: Brown Out Detect
                DCD     FMC_IRQHandler            ; 16+27: IP2111 Flash Memory Controller
                DCD     PIOINT3_IRQHandler        ; 16+28: PIO INT3
                DCD     PIOINT2_IRQHandler        ; 16+29: PIO INT2
                DCD     PIOINT1_IRQHandler        ; 16+30: PIO INT1
                DCD     PIOINT0_IRQHandler        ; 16+31: PIO INT0


                IF      :LNOT::DEF:NO_CRP
                AREA    |.ARM.__at_0x02FC|, CODE, READONLY
CRP_Key         DCD     0xFFFFFFFF
                ENDIF


                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)                

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WAKEUP_IRQHandler         [WEAK]
                EXPORT  CAN_IRQHandler            [WEAK]
                EXPORT  SSP1_IRQHandler           [WEAK]
                EXPORT  I2C_IRQHandler            [WEAK]
                EXPORT  TIMER16_0_IRQHandler      [WEAK]
                EXPORT  TIMER16_1_IRQHandler      [WEAK]
                EXPORT  TIMER32_0_IRQHandler      [WEAK]
                EXPORT  TIMER32_1_IRQHandler      [WEAK]
                EXPORT  SSP0_IRQHandler           [WEAK]
                EXPORT  UART_IRQHandler           [WEAK]
                EXPORT  USB_IRQHandler            [WEAK]
                EXPORT  USB_FIQHandler            [WEAK]
                EXPORT  ADC_IRQHandler            [WEAK]
                EXPORT  WDT_IRQHandler            [WEAK]
                EXPORT  BOD_IRQHandler            [WEAK]
                EXPORT  FMC_IRQHandler            [WEAK]
                EXPORT  PIOINT3_IRQHandler        [WEAK]
                EXPORT  PIOINT2_IRQHandler        [WEAK]
                EXPORT	PIOINT1_IRQHandler        [WEAK]
                EXPORT	PIOINT0_IRQHandler        [WEAK]

WAKEUP_IRQHandler
CAN_IRQHandler
SSP1_IRQHandler
I2C_IRQHandler
TIMER16_0_IRQHandler
TIMER16_1_IRQHandler
TIMER32_0_IRQHandler
TIMER32_1_IRQHandler
SSP0_IRQHandler
UART_IRQHandler
USB_IRQHandler
USB_FIQHandler
ADC_IRQHandler
WDT_IRQHandler
BOD_IRQHandler
FMC_IRQHandler
PIOINT3_IRQHandler  
PIOINT2_IRQHandler 
PIOINT1_IRQHandler
PIOINT0_IRQHandler

                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB
                
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                
                ELSE
                
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
