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
;/*
; * This file is derivative of CMSIS V5.00 startup_Device.s
; */


;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler

        DCD     NMI_Handler
        DCD     HardFault_Handler
        DCD     MemManage_Handler
        DCD     BusFault_Handler
        DCD     UsageFault_Handler
__vector_table_0x1c
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     SVC_Handler
        DCD     DebugMon_Handler
        DCD     0
        DCD     PendSV_Handler
        DCD     SysTick_Handler

        ; External Interrupts
        DCD     UART0_Handler             ;  0: UART 0 RX and TX Handler
        DCD     Spare_IRQ_Handler         ;  1: Undefined
        DCD     UART1_Handler             ;  2: UART 1 RX and TX Handler
        DCD     I2C0_Handler              ;  3: I2C 0 Handler
        DCD     I2C1_Handler              ;  4: I2C 1 Handler
        DCD     RTC_Handler               ;  5: RTC Handler
        DCD     PORT0_COMB_Handler        ;  6: GPIO Port 0 Combined Handler
        DCD     PORT1_COMB_Handler        ;  7: GPIO Port 1 Combined Handler
        DCD     TIMER0_Handler            ;  8: TIMER 0 handler
        DCD     TIMER1_Handler            ;  9: TIMER 1 handler
        DCD     DUALTIMER_HANDLER         ; 10: Dual timer handler
        DCD     SPI0_Handler              ; 11: SPI 0 Handler
        DCD     UARTOVF_Handler           ; 12: UART 0,1 Overflow Handler
        DCD     SPI1_Handler              ; 13: SPI 1 Handler
        DCD     QSPI_Handler              ; 14: QSPI Handler
        DCD     DMA_Handler               ; 15: DMA handler
        DCD     PORT0_0_Handler           ; 16: GPIO Port 0 pin 0 Handler
        DCD     PORT0_1_Handler           ; 17: GPIO Port 0 pin 1 Handler
        DCD     PORT0_2_Handler           ; 18: GPIO Port 0 pin 2 Handler
        DCD     PORT0_3_Handler           ; 19: GPIO Port 0 pin 3 Handler
        DCD     PORT0_4_Handler           ; 20: GPIO Port 0 pin 4 Handler
        DCD     PORT0_5_Handler           ; 21: GPIO Port 0 pin 5 Handler
        DCD     PORT0_6_Handler           ; 22: GPIO Port 0 pin 6 Handler
        DCD     PORT0_7_Handler           ; 23: GPIO Port 0 pin 7 Handler
        DCD     PORT0_8_Handler           ; 24: GPIO Port 0 pin 8 Handler
        DCD     PORT0_9_Handler           ; 25: GPIO Port 0 pin 9 Handler
        DCD     PORT0_10_Handler          ; 26: GPIO Port 0 pin 10 Handler
        DCD     PORT0_11_Handler          ; 27: GPIO Port 0 pin 11 Handler
        DCD     PORT0_12_Handler          ; 28: GPIO Port 0 pin 12 Handler
        DCD     PORT0_13_Handler          ; 29: GPIO Port 0 pin 13 Handler
        DCD     PORT0_14_Handler          ; 30: GPIO Port 0 pin 14 Handler
        DCD     PORT0_15_Handler          ; 31: GPIO Port 0 pin 15 Handler
        DCD     SysError_Handler          ; 32: System Error (Flash Cache)
        DCD     EFLASH_Handler            ; 33: Embedded Flash
        DCD     LLCC_TXCMD_EMPTY_Handler  ; 34: LLCC_TXCMDIRQ
        DCD     LLCC_TXEVT_EMPTY_Handler  ; 35: LLCC_TXEVTIRQ
        DCD     LLCC_TXDMAH_DONE_Handler  ; 36: LLCC_TXDMA0IRQ
        DCD     LLCC_TXDMAL_DONE_Handler  ; 37: LLCC_TXDMA1IRQ
        DCD     LLCC_RXCMD_VALID_Handler  ; 38: LLCC_RXCMDIRQ
        DCD     LLCC_RXEVT_VALID_Handler  ; 39: LLCC_RXEVTIRQ
        DCD     LLCC_RXDMAH_DONE_Handler  ; 40: LLCC_RXDMA0IRQ
        DCD     LLCC_RXDMAL_DONE_Handler  ; 41: LLCC_RXDMA1IRQ
        DCD     PORT2_COMB_Handler        ; 42: GPIO 2
        DCD     PORT3_COMB_Handler        ; 43: GPIO 3
        DCD     TRNG_Handler              ; 44: TRNG
__Vectors_End

__Vectors       EQU   __vector_table
__Vectors_Size  EQU   __Vectors_End - __Vectors


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
MemManage_Handler
        B MemManage_Handler

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BusFault_Handler
        B BusFault_Handler

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UsageFault_Handler
        B UsageFault_Handler

        PUBWEAK SecureFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SecureFault_Handler
        B SecureFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DebugMon_Handler
        B DebugMon_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B SysTick_Handler


        PUBWEAK UART0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART0_Handler
        B UART0_Handler

        PUBWEAK Spare_IRQ_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
Spare_IRQ_Handler
        B Spare_IRQ_Handler

        PUBWEAK UART1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART1_Handler
        B UART1_Handler

        PUBWEAK I2C0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C0_Handler
        B I2C0_Handler

        PUBWEAK I2C1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C1_Handler
        B I2C1_Handler

        PUBWEAK RTC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
RTC_Handler
        B RTC_Handler

        PUBWEAK PORT0_COMB_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_COMB_Handler
        B PORT0_COMB_Handler

        PUBWEAK PORT1_COMB_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT1_COMB_Handler
        B PORT1_COMB_Handler

        PUBWEAK TIMER0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER0_Handler
        B TIMER0_Handler

        PUBWEAK TIMER1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER1_Handler
        B TIMER1_Handler

        PUBWEAK DUALTIMER_HANDLER
        SECTION .text:CODE:REORDER:NOROOT(1)
DUALTIMER_HANDLER
        B DUALTIMER_HANDLER

        PUBWEAK SPI0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI0_Handler
        B SPI0_Handler

        PUBWEAK UARTOVF_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UARTOVF_Handler
        B UARTOVF_Handler

        PUBWEAK SPI1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI1_Handler
        B SPI1_Handler

        PUBWEAK QSPI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
QSPI_Handler
        B QSPI_Handler

        PUBWEAK DMA_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA_Handler
        B DMA_Handler

        PUBWEAK PORT0_0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_0_Handler
        B PORT0_0_Handler

        PUBWEAK PORT0_1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_1_Handler
        B PORT0_1_Handler

        PUBWEAK PORT0_2_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_2_Handler
        B PORT0_2_Handler

        PUBWEAK PORT0_3_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_3_Handler
        B PORT0_3_Handler

        PUBWEAK PORT0_4_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_4_Handler
        B PORT0_4_Handler

        PUBWEAK PORT0_5_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_5_Handler
        B PORT0_5_Handler

        PUBWEAK PORT0_6_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_6_Handler
        B PORT0_6_Handler

        PUBWEAK PORT0_7_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_7_Handler
        B PORT0_7_Handler

        PUBWEAK PORT0_8_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_8_Handler
        B PORT0_8_Handler

        PUBWEAK PORT0_9_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_9_Handler
        B PORT0_9_Handler

        PUBWEAK PORT0_10_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_10_Handler
        B PORT0_10_Handler

        PUBWEAK PORT0_11_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_11_Handler
        B PORT0_11_Handler

        PUBWEAK PORT0_12_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_12_Handler
        B PORT0_12_Handler

        PUBWEAK PORT0_13_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_13_Handler
        B PORT0_13_Handler

        PUBWEAK PORT0_14_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_14_Handler
        B PORT0_14_Handler

        PUBWEAK PORT0_15_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_15_Handler
        B PORT0_15_Handler

        PUBWEAK SysError_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysError_Handler
        B SysError_Handler

        PUBWEAK EFLASH_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
EFLASH_Handler
        B EFLASH_Handler

        PUBWEAK LLCC_TXCMD_EMPTY_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
LLCC_TXCMD_EMPTY_Handler
        B LLCC_TXCMD_EMPTY_Handler

        PUBWEAK LLCC_TXEVT_EMPTY_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
LLCC_TXEVT_EMPTY_Handler
        B LLCC_TXEVT_EMPTY_Handler

        PUBWEAK LLCC_TXDMAH_DONE_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
LLCC_TXDMAH_DONE_Handler
        B LLCC_TXDMAH_DONE_Handler

        PUBWEAK LLCC_TXDMAL_DONE_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
LLCC_TXDMAL_DONE_Handler
        B LLCC_TXDMAL_DONE_Handler

        PUBWEAK LLCC_RXCMD_VALID_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
LLCC_RXCMD_VALID_Handler
        B LLCC_RXCMD_VALID_Handler

        PUBWEAK LLCC_RXEVT_VALID_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
LLCC_RXEVT_VALID_Handler
        B LLCC_RXEVT_VALID_Handler

        PUBWEAK LLCC_RXDMAH_DONE_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
LLCC_RXDMAH_DONE_Handler
        B LLCC_RXDMAH_DONE_Handler

        PUBWEAK LLCC_RXDMAL_DONE_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
LLCC_RXDMAL_DONE_Handler
        B LLCC_RXDMAL_DONE_Handler

        PUBWEAK PORT2_COMB_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT2_COMB_Handler
        B PORT2_COMB_Handler

        PUBWEAK PORT3_COMB_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT3_COMB_Handler
        B PORT3_COMB_Handler

        PUBWEAK TRNG_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TRNG_Handler
        B TRNG_Handler

        END
