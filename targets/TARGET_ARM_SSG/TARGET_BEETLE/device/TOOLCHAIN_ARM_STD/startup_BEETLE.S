;/*
; * BEETLE CMSIS Library
; */
;/*
; * Copyright (c) 2009-2016 ARM Limited. All rights reserved.
; *
; * SPDX-License-Identifier: Apache-2.0
; *
; * Licensed under the Apache License, Version 2.0 (the License); you may
; * not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; * http://www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an AS IS BASIS, WITHOUT
; * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; */
;
; This file is derivative of CMSIS V5.00 startup_ARMCM3.s
;
;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


__initial_sp        EQU     0x20020000  ; Top of RAM

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     UART0_Handler             ; UART 0 RX and TX Handler
                DCD     Spare_IRQ_Handler         ; Undefined
                DCD     UART1_Handler             ; UART 1 RX and TX Handler
                DCD     I2C0_Handler              ; I2C 0 Handler
                DCD     I2C1_Handler              ; I2C 1 Handler
                DCD     RTC_Handler               ; RTC Handler
                DCD     PORT0_COMB_Handler        ; GPIO Port 0 Combined Handler
                DCD     PORT1_COMB_Handler        ; GPIO Port 1 Combined Handler
                DCD     TIMER0_Handler            ; TIMER 0 handler
                DCD     TIMER1_Handler            ; TIMER 1 handler
                DCD     DUALTIMER_HANDLER         ; Dual timer handler
                DCD     SPI0_Handler              ; SPI 0 Handler
                DCD     UARTOVF_Handler           ; UART 0,1 Overflow Handler
                DCD     SPI1_Handler              ; SPI 1 Handler
                DCD     QSPI_Handler              ; QSPI Handler
                DCD     DMA_Handler               ; DMA handler
                DCD     PORT0_0_Handler           ; GPIO Port 0 pin 0 Handler
                DCD     PORT0_1_Handler           ; GPIO Port 0 pin 1 Handler
                DCD     PORT0_2_Handler           ; GPIO Port 0 pin 2 Handler
                DCD     PORT0_3_Handler           ; GPIO Port 0 pin 3 Handler
                DCD     PORT0_4_Handler           ; GPIO Port 0 pin 4 Handler
                DCD     PORT0_5_Handler           ; GPIO Port 0 pin 5 Handler
                DCD     PORT0_6_Handler           ; GPIO Port 0 pin 6 Handler
                DCD     PORT0_7_Handler           ; GPIO Port 0 pin 7 Handler
                DCD     PORT0_8_Handler           ; GPIO Port 0 pin 8 Handler
                DCD     PORT0_9_Handler           ; GPIO Port 0 pin 9 Handler
                DCD     PORT0_10_Handler          ; GPIO Port 0 pin 10 Handler
                DCD     PORT0_11_Handler          ; GPIO Port 0 pin 11 Handler
                DCD     PORT0_12_Handler          ; GPIO Port 0 pin 12 Handler
                DCD     PORT0_13_Handler          ; GPIO Port 0 pin 13 Handler
                DCD     PORT0_14_Handler          ; GPIO Port 0 pin 14 Handler
                DCD     PORT0_15_Handler          ; GPIO Port 0 pin 15 Handler
                DCD     SysError_Handler          ; System Error (Flash Cache)
                DCD     EFLASH_Handler            ; Embedded Flash
                DCD     LLCC_TXCMD_EMPTY_Handler  ; LLCC_TXCMDIRQ
                DCD     LLCC_TXEVT_EMPTY_Handler  ; LLCC_TXEVTIRQ
                DCD     LLCC_TXDMAH_DONE_Handler  ; LLCC_TXDMA0IRQ
                DCD     LLCC_TXDMAL_DONE_Handler  ; LLCC_TXDMA1IRQ
                DCD     LLCC_RXCMD_VALID_Handler  ; LLCC_RXCMDIRQ
                DCD     LLCC_RXEVT_VALID_Handler  ; LLCC_RXEVTIRQ
                DCD     LLCC_RXDMAH_DONE_Handler  ; LLCC_RXDMA0IRQ
                DCD     LLCC_RXDMAL_DONE_Handler  ; LLCC_RXDMA1IRQ
                DCD     PORT2_COMB_Handler        ; GPIO 2
                DCD     PORT3_COMB_Handler        ; GPIO 3
                DCD     TRNG_Handler              ; TRNG
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
                EXPORT UART0_Handler              [WEAK]
                EXPORT Spare_IRQ_Handler          [WEAK]
                EXPORT UART1_Handler              [WEAK]
                EXPORT I2C0_Handler               [WEAK]
                EXPORT I2C1_Handler               [WEAK]
                EXPORT RTC_Handler                [WEAK]
                EXPORT PORT0_COMB_Handler         [WEAK]
                EXPORT PORT1_COMB_Handler         [WEAK]
                EXPORT TIMER0_Handler             [WEAK]
                EXPORT TIMER1_Handler             [WEAK]
                EXPORT DUALTIMER_HANDLER          [WEAK]
                EXPORT SPI0_Handler               [WEAK]
                EXPORT UARTOVF_Handler            [WEAK]
                EXPORT SPI1_Handler               [WEAK]
                EXPORT QSPI_Handler               [WEAK]
                EXPORT DMA_Handler                [WEAK]
                EXPORT PORT0_0_Handler            [WEAK]
                EXPORT PORT0_1_Handler            [WEAK]
                EXPORT PORT0_2_Handler            [WEAK]
                EXPORT PORT0_3_Handler            [WEAK]
                EXPORT PORT0_4_Handler            [WEAK]
                EXPORT PORT0_5_Handler            [WEAK]
                EXPORT PORT0_6_Handler            [WEAK]
                EXPORT PORT0_7_Handler            [WEAK]
                EXPORT PORT0_8_Handler            [WEAK]
                EXPORT PORT0_9_Handler            [WEAK]
                EXPORT PORT0_10_Handler           [WEAK]
                EXPORT PORT0_11_Handler           [WEAK]
                EXPORT PORT0_12_Handler           [WEAK]
                EXPORT PORT0_13_Handler           [WEAK]
                EXPORT PORT0_14_Handler           [WEAK]
                EXPORT PORT0_15_Handler           [WEAK]
                EXPORT SysError_Handler           [WEAK]
                EXPORT EFLASH_Handler             [WEAK]
                EXPORT LLCC_TXEVT_EMPTY_Handler   [WEAK]
                EXPORT LLCC_TXCMD_EMPTY_Handler   [WEAK]
                EXPORT LLCC_RXEVT_VALID_Handler   [WEAK]
                EXPORT LLCC_RXCMD_VALID_Handler   [WEAK]
                EXPORT LLCC_TXDMAL_DONE_Handler   [WEAK]
                EXPORT LLCC_RXDMAL_DONE_Handler   [WEAK]
                EXPORT LLCC_TXDMAH_DONE_Handler   [WEAK]
                EXPORT LLCC_RXDMAH_DONE_Handler   [WEAK]
                EXPORT PORT2_COMB_Handler         [WEAK]
                EXPORT PORT3_COMB_Handler         [WEAK]
                EXPORT TRNG_Handler               [WEAK]

UART0_Handler
Spare_IRQ_Handler
UART1_Handler
I2C0_Handler
I2C1_Handler
RTC_Handler
PORT0_COMB_Handler
PORT1_COMB_Handler
TIMER0_Handler
TIMER1_Handler
DUALTIMER_HANDLER
SPI0_Handler
UARTOVF_Handler
SPI1_Handler
QSPI_Handler
DMA_Handler
PORT0_0_Handler
PORT0_1_Handler
PORT0_2_Handler
PORT0_3_Handler
PORT0_4_Handler
PORT0_5_Handler
PORT0_6_Handler
PORT0_7_Handler
PORT0_8_Handler
PORT0_9_Handler
PORT0_10_Handler
PORT0_11_Handler
PORT0_12_Handler
PORT0_13_Handler
PORT0_14_Handler
PORT0_15_Handler
SysError_Handler
EFLASH_Handler
LLCC_TXEVT_EMPTY_Handler
LLCC_TXCMD_EMPTY_Handler
LLCC_RXEVT_VALID_Handler
LLCC_RXCMD_VALID_Handler
LLCC_TXDMAL_DONE_Handler
LLCC_RXDMAL_DONE_Handler
LLCC_TXDMAH_DONE_Handler
LLCC_RXDMAH_DONE_Handler
PORT2_COMB_Handler
PORT3_COMB_Handler
TRNG_Handler
                B       .

                ENDP


                ALIGN

                END
