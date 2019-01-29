;/*
; * Copyright (c) 2009-2019 Arm Limited. All rights reserved.
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
; * This file is derivative of mbed-os V5.10.4 CM3DS startup_MPS2.S for IAR
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
        DCD     sfe(CSTACK)               ; Top of Stack
        DCD     Reset_Handler             ; Reset Handler

        DCD     NMI_Handler               ; NMI Handler
        DCD     HardFault_Handler         ; Hard Fault Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
__vector_table_0x1c
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     SVC_Handler               ; SVCall Handler
        DCD     0                         ; reserved
        DCD     0                         ; Reserved
        DCD     PendSV_Handler            ; PendSV Handler
        DCD     SysTick_Handler           ; SysTick Handler

        ; External Interrupts
        DCD     UARTRX0_Handler           ; UART 0 RX Handler
        DCD     UARTTX0_Handler           ; UART 0 TX Handler
        DCD     UARTRX1_Handler           ; UART 1 RX Handler
        DCD     UARTTX1_Handler           ; UART 1 TX Handler
        DCD     UARTRX2_Handler           ; UART 2 RX Handler
        DCD     UARTTX2_Handler           ; UART 2 TX Handler
        DCD     PORT0_COMB_Handler        ; GPIO Port 0 Combined Handler
        DCD     PORT1_COMB_Handler        ; GPIO Port 1 Combined Handler
        DCD     TIMER0_Handler            ; TIMER 0 handler
        DCD     TIMER1_Handler            ; TIMER 1 handler
        DCD     DUALTIMER_HANDLER         ; Dual timer handler
        DCD     SPI_Handler               ; SPI exceptions Handler
        DCD     UARTOVF_Handler           ; UART 0,1,2 Overflow Handler
        DCD     ETHERNET_Handler          ; Ethernet Overflow Handler
        DCD     I2S_Handler               ; I2S Handler
        DCD     TSC_Handler               ; Touch Screen handler
        DCD     PORT2_COMB_Handler        ; GPIO Port 2 Combined Handler
        DCD     PORT3_COMB_Handler        ; GPIO Port 3 Combined Handler
        DCD     UARTRX3_Handler           ; UART 3 RX Handler
        DCD     UARTTX3_Handler           ; UART 3 TX Handler
        DCD     UARTRX4_Handler           ; UART 4 RX Handler
        DCD     UARTTX4_Handler           ; UART 4 TX Handler
        DCD     ADCSPI_Handler            ; SHIELD ADC SPI exceptions Handler
        DCD     SHIELDSPI_Handler         ; SHIELD SPI exceptions Handler
        DCD     PORT0_0_Handler           ; GPIO Port 0 pin 0 Handler
        DCD     PORT0_1_Handler           ; GPIO Port 0 pin 1 Handler
        DCD     PORT0_2_Handler           ; GPIO Port 0 pin 2 Handler
        DCD     PORT0_3_Handler           ; GPIO Port 0 pin 3 Handler
        DCD     PORT0_4_Handler           ; GPIO Port 0 pin 4 Handler
        DCD     PORT0_5_Handler           ; GPIO Port 0 pin 5 Handler
        DCD     PORT0_6_Handler           ; GPIO Port 0 pin 6 Handler
        DCD     PORT0_7_Handler           ; GPIO Port 0 pin 7 Handler


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

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B SysTick_Handler


        PUBWEAK UARTRX0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UARTRX0_Handler
        B UARTRX0_Handler

        PUBWEAK UARTTX0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UARTTX0_Handler
        B UARTTX0_Handler

        PUBWEAK UARTRX1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UARTRX1_Handler
        B UARTRX1_Handler

        PUBWEAK UARTTX1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UARTTX1_Handler
        B UARTTX1_Handler

        PUBWEAK UARTRX2_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UARTRX2_Handler
        B UARTRX2_Handler

        PUBWEAK UARTTX2_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UARTTX2_Handler
        B UARTTX2_Handler

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

        PUBWEAK SPI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI_Handler
        B SPI_Handler

        PUBWEAK UARTOVF_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UARTOVF_Handler
        B UARTOVF_Handler

        PUBWEAK ETHERNET_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
ETHERNET_Handler
        B ETHERNET_Handler

        PUBWEAK I2S_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2S_Handler
        B I2S_Handler

        PUBWEAK TSC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TSC_Handler
        B TSC_Handler

        PUBWEAK PORT2_COMB_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT2_COMB_Handler
        B PORT2_COMB_Handler

        PUBWEAK PORT3_COMB_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT3_COMB_Handler
        B PORT3_COMB_Handler

        PUBWEAK UARTRX3_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UARTRX3_Handler
        B UARTRX3_Handler

        PUBWEAK UARTTX3_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UARTTX3_Handler
        B UARTTX3_Handler

        PUBWEAK UARTRX4_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UARTRX4_Handler
        B UARTRX4_Handler

        PUBWEAK UARTTX4_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UARTTX4_Handler
        B UARTTX4_Handler

        PUBWEAK ADCSPI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
ADCSPI_Handler
        B ADCSPI_Handler

        PUBWEAK SHIELDSPI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SHIELDSPI_Handler
        B SHIELDSPI_Handler

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

        END
