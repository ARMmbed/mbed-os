;/* File: startup_ncs36510.s
; * Purpose: startup file for Cortex-M3 devices. Should use with
; *   ARMGCC for ARM Embedded Processors
; * Version: V2.00
; * Date: 25 Feb 2016
; *
; */
;/* Copyright (c) 2011 - 2014 ARM LIMITED
;
;   All rights reserved.
;   Redistribution and use in source and binary forms, with or without
;   modification, are permitted provided that the following conditions are met:
;   - Redistributions of source code must retain the above copyright
;     notice, this list of conditions and the following disclaimer.
;   - Redistributions in binary form must reproduce the above copyright
;     notice, this list of conditions and the following disclaimer in the
;     documentation and/or other materials provided with the distribution.
;   - Neither the name of ARM nor the names of its contributors may be used
;     to endorse or promote products derived from this software without
;     specific prior written permission.
;   *
;   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
;   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
;   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
;   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
;   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
;   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
;   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
;   POSSIBILITY OF SUCH DAMAGE.
;   ---------------------------------------------------------------------------*/


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0x3000 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size
                IMPORT  |Image$$ARM_LIB_HEAP$$ZI$$Limit|

__Vectors       DCD     |Image$$ARM_LIB_HEAP$$ZI$$Limit|  ; Top of RAM
                DCD     Reset_Handler  ; Reset Handler
                DCD     NMI_Handler  ; NMI Handler
                DCD     HardFault_Handler  ; Hard Fault Handler
                DCD     MemManage_Handler  ; MPU Fault Handler
                DCD     BusFault_Handler  ; Bus Fault Handler
                DCD     UsageFault_Handler  ; Usage Fault Handler
                DCD     0  ; Reserved
                DCD     0  ; Reserved
                DCD     0  ; Reserved
                DCD     0  ; Reserved
                DCD     SVC_Handler  ; SVCall Handler
                DCD     DebugMon_Handler  ; Debug Monitor Handler
                DCD     0  ; Reserved
                DCD     PendSV_Handler  ; PendSV Handler
                DCD     SysTick_Handler  ; SysTick Handler

                ; External Interrupts
                DCD     fIrqTim0Handler
                DCD     fIrqTim1Handler
                DCD     fIrqTim2Handler
                DCD     fIrqUart1Handler
                DCD     fIrqSpiHandler
                DCD     fIrqI2CHandler
                DCD     fIrqGpioHandler
                DCD     fIrqRtcHandler
                DCD     fIrqFlashHandler
                DCD     fIrqMacHwHandler
                DCD     fIrqAesHandler
                DCD     fIrqAdcHandler
                DCD     fIrqClockCalHandler
                DCD     fIrqUart2Handler
                DCD     fIrqUviHandler
                DCD     fIrqDmaHandler
                DCD     fIrqDbgPwrUpHandler
                DCD     fIrqSpi2Handler
                DCD     fIrqI2C2Handler
                DCD     fIrqFVDDHCompHandler
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

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
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
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
                EXPORT      fIrqTim0Handler  [WEAK]
                EXPORT      fIrqTim1Handler  [WEAK]
                EXPORT      fIrqTim2Handler  [WEAK]
                EXPORT      fIrqUart1Handler  [WEAK]
                EXPORT      fIrqSpiHandler  [WEAK]
                EXPORT      fIrqI2CHandler  [WEAK]
                EXPORT      fIrqGpioHandler  [WEAK]
                EXPORT      fIrqRtcHandler  [WEAK]
                EXPORT      fIrqFlashHandler  [WEAK]
                EXPORT      fIrqMacHwHandler  [WEAK]
                EXPORT      fIrqAesHandler  [WEAK]
                EXPORT      fIrqAdcHandler  [WEAK]
                EXPORT      fIrqClockCalHandler  [WEAK]
                EXPORT      fIrqUart2Handler  [WEAK]
                EXPORT      fIrqUviHandler  [WEAK]
                EXPORT      fIrqDmaHandler  [WEAK]
                EXPORT      fIrqDbgPwrUpHandler  [WEAK]
                EXPORT      fIrqSpi2Handler  [WEAK]
                EXPORT      fIrqI2C2Handler  [WEAK]
                EXPORT      fIrqFVDDHCompHandler  [WEAK]

fIrqTim0Handler
fIrqTim1Handler
fIrqTim2Handler
fIrqUart1Handler
fIrqSpiHandler
fIrqI2CHandler
fIrqGpioHandler
fIrqRtcHandler
fIrqFlashHandler
fIrqMacHwHandler
fIrqAesHandler
fIrqAdcHandler
fIrqClockCalHandler
fIrqUart2Handler
fIrqUviHandler
fIrqDmaHandler
fIrqDbgPwrUpHandler
fIrqSpi2Handler
fIrqI2C2Handler
fIrqFVDDHCompHandler
DefaultISR

                B       .

                ENDP

                EXPORT  __user_initial_stackheap
                IMPORT  |Image$$ARM_LIB_HEAP$$Base|
                IMPORT  |Image$$ARM_LIB_HEAP$$ZI$$Limit|

__user_initial_stackheap PROC
                LDR     R0, = |Image$$ARM_LIB_HEAP$$Base|
                LDR     R2, = |Image$$ARM_LIB_HEAP$$ZI$$Limit|
                BX      LR
                ENDP

                ALIGN
                END
