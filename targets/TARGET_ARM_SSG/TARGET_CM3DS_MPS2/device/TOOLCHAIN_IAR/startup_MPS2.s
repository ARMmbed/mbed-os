;/*
; * MPS2 CMSIS Library
; */
;/*
; * Copyright (c) 2009-2017 ARM Limited. All rights reserved.
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
                DCD     UART0_IRQHandler        ;   0:UART 0 RX and TX Combined Interrupt
                DCD     Spare_IRQHandler        ;   1:Undefined
                DCD     UART1_IRQHandler        ;   2:UART 1 RX and TX Combined Interrupt
                DCD     APB_Slave0_IRQHandler   ;   3:Reserved for APB Slave
                DCD     APB_Slave1_IRQHandler   ;   4:Reserved for APB Slave
                DCD     RTC_IRQHandler          ;   5:RTC Interrupt
                DCD     PORT0_IRQHandler        ;   6:GPIO Port 0 combined Interrupt
                DCD     PORT1_ALL_IRQHandler    ;   7:GPIO Port 1 combined Interrupt
                DCD     TIMER0_IRQHandler       ;   8:TIMER 0 Interrupt
                DCD     TIMER1_IRQHandler       ;   9:TIMER 1 Interrupt
                DCD     DUALTIMER_IRQHandler    ;   10:Dual Timer Interrupt
                DCD     APB_Slave2_IRQHandler   ;   11:Reserved for APB Slave
                DCD     UARTOVF_IRQHandler      ;   12:UART 0,1,2 Overflow Interrupt
                DCD     APB_Slave3_IRQHandler   ;   13:Reserved for APB Slave
                DCD     RESERVED0_IRQHandler    ;   14:Reserved
                DCD     TSC_IRQHandler          ;   15:Touch Screen Interrupt
                DCD     PORT0_0_IRQHandler      ;   16:GPIO Port 0 pin 0 Handler
                DCD     PORT0_1_IRQHandler      ;   17:GPIO Port 0 pin 1 Handler
                DCD     PORT0_2_IRQHandler      ;   18:GPIO Port 0 pin 2 Handler
                DCD     PORT0_3_IRQHandler      ;   19:GPIO Port 0 pin 3 Handler
                DCD     PORT0_4_IRQHandler      ;   20:GPIO Port 0 pin 4 Handler
                DCD     PORT0_5_IRQHandler      ;   21:GPIO Port 0 pin 5 Handler
                DCD     PORT0_6_IRQHandler      ;   22:GPIO Port 0 pin 6 Handler
                DCD     PORT0_7_IRQHandler      ;   23:GPIO Port 0 pin 7 Handler
                DCD     PORT0_8_IRQHandler      ;   24:GPIO Port 0 pin 8 Handler
                DCD     PORT0_9_IRQHandler      ;   25:GPIO Port 0 pin 9 Handler
                DCD     PORT0_10_IRQHandler     ;   26:GPIO Port 0 pin 10 Handler
                DCD     PORT0_11_IRQHandler     ;   27:GPIO Port 0 pin 11 Handler
                DCD     PORT0_12_IRQHandler     ;   28:GPIO Port 0 pin 12 Handler
                DCD     PORT0_13_IRQHandler     ;   29:GPIO Port 0 pin 13 Handler
                DCD     PORT0_14_IRQHandler     ;   30:GPIO Port 0 pin 14 Handler
                DCD     PORT0_15_IRQHandler     ;   31:GPIO Port 0 pin 15 Handler
                DCD     FLASH0_IRQHandler       ;   32:Reserved for Flash
                DCD     FLASH1_IRQHandler       ;   33:Reserved for Flash
                DCD     RESERVED1_IRQHandler    ;   34:Reserved
                DCD     RESERVED2_IRQHandler    ;   35:Reserved
                DCD     RESERVED3_IRQHandler    ;   36:Reserved
                DCD     RESERVED4_IRQHandler    ;   37:Reserved
                DCD     RESERVED5_IRQHandler    ;   38:Reserved
                DCD     RESERVED6_IRQHandler    ;   39:Reserved
                DCD     RESERVED7_IRQHandler    ;   40:Reserved
                DCD     RESERVED8_IRQHandler    ;   41:Reserved
                DCD     PORT2_ALL_IRQHandler    ;   42:GPIO Port 2 combined Interrupt
                DCD     PORT3_ALL_IRQHandler    ;   43:GPIO Port 3 combined Interrupt
                DCD     TRNG_IRQHandler         ;   44:Random number generator Interrupt
                DCD     UART2_IRQHandler        ;   45:UART 2 RX and TX Combined Interrupt
                DCD     UART3_IRQHandler        ;   46:UART 3 RX and TX Combined Interrupt
                DCD     ETHERNET_IRQHandler     ;   47:Ethernet interrupt     t.b.a.
                DCD     I2S_IRQHandler          ;   48:I2S Interrupt
                DCD     MPS2_SPI0_IRQHandler    ;   49:SPI Interrupt (spi header)
                DCD     MPS2_SPI1_IRQHandler    ;   50:SPI Interrupt (clcd)
                DCD     MPS2_SPI2_IRQHandler    ;   51:SPI Interrupt (spi 1 ADC replacement)
                DCD     MPS2_SPI3_IRQHandler    ;   52:SPI Interrupt (spi 0 shield 0 replacement)
                DCD     MPS2_SPI4_IRQHandler    ;   53:SPI Interrupt  (shield 1)
                DCD     PORT4_ALL_IRQHandler    ;   54:GPIO Port 4 combined Interrupt
                DCD     PORT5_ALL_IRQHandler    ;   55:GPIO Port 5 combined Interrupt
                DCD     UART4_IRQHandler        ;   56:UART 4 RX and TX Combined Interrupt
 
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


        PUBWEAK UART0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART0_IRQHandler
        B UART0_IRQHandler

        PUBWEAK Spare_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
Spare_IRQHandler
        B Spare_IRQHandler

        PUBWEAK UART1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART1_IRQHandler
        B UART1_IRQHandler

        PUBWEAK APB_Slave0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
APB_Slave0_IRQHandler
        B APB_Slave0_IRQHandler

        PUBWEAK APB_Slave1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
APB_Slave1_IRQHandler
        B APB_Slave1_IRQHandler

        PUBWEAK RTC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RTC_IRQHandler
        B RTC_IRQHandler

        PUBWEAK PORT0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_IRQHandler
        B PORT0_IRQHandler

        PUBWEAK PORT1_ALL_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT1_ALL_IRQHandler
        B PORT1_ALL_IRQHandler

        PUBWEAK TIMER0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER0_IRQHandler
        B TIMER0_IRQHandler

        PUBWEAK TIMER1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER1_IRQHandler
        B TIMER1_IRQHandler

        PUBWEAK DUALTIMER_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DUALTIMER_IRQHandler
        B DUALTIMER_IRQHandler

        PUBWEAK APB_Slave2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
APB_Slave2_IRQHandler
        B APB_Slave2_IRQHandler

        PUBWEAK UARTOVF_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UARTOVF_IRQHandler
        B UARTOVF_IRQHandler

        PUBWEAK APB_Slave3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
APB_Slave3_IRQHandler
        B APB_Slave3_IRQHandler

        PUBWEAK RESERVED0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RESERVED0_IRQHandler
        B RESERVED0_IRQHandler

        PUBWEAK TSC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TSC_IRQHandler
        B TSC_IRQHandler

        PUBWEAK PORT0_0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_0_IRQHandler
        B PORT0_0_IRQHandler

        PUBWEAK PORT0_1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_1_IRQHandler
        B PORT0_1_IRQHandler

        PUBWEAK PORT0_2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_2_IRQHandler
        B PORT0_2_IRQHandler

        PUBWEAK PORT0_3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_3_IRQHandler
        B PORT0_3_IRQHandler

        PUBWEAK PORT0_4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_4_IRQHandler
        B PORT0_4_IRQHandler

        PUBWEAK PORT0_5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_5_IRQHandler
        B PORT0_5_IRQHandler

        PUBWEAK PORT0_6_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_6_IRQHandler
        B PORT0_6_IRQHandler

        PUBWEAK PORT0_7_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_7_IRQHandler
        B PORT0_7_IRQHandler

        PUBWEAK PORT0_8_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_8_IRQHandler
        B PORT0_8_IRQHandler

        PUBWEAK PORT0_9_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_9_IRQHandler
        B PORT0_9_IRQHandler

        PUBWEAK PORT0_10_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_10_IRQHandler
        B PORT0_10_IRQHandler

        PUBWEAK PORT0_11_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_11_IRQHandler
        B PORT0_11_IRQHandler

        PUBWEAK PORT0_12_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_12_IRQHandler
        B PORT0_12_IRQHandler

        PUBWEAK PORT0_13_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_13_IRQHandler
        B PORT0_13_IRQHandler

        PUBWEAK PORT0_14_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_14_IRQHandler
        B PORT0_14_IRQHandler

        PUBWEAK PORT0_15_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT0_15_IRQHandler
        B PORT0_15_IRQHandler

        PUBWEAK FLASH0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
FLASH0_IRQHandler
        B FLASH0_IRQHandler

        PUBWEAK FLASH1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
FLASH1_IRQHandler
        B FLASH1_IRQHandler

        PUBWEAK RESERVED1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RESERVED1_IRQHandler
        B RESERVED1_IRQHandler

        PUBWEAK RESERVED2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RESERVED2_IRQHandler
        B RESERVED2_IRQHandler

        PUBWEAK RESERVED3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RESERVED3_IRQHandler
        B RESERVED3_IRQHandler

        PUBWEAK RESERVED4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RESERVED4_IRQHandler
        B RESERVED4_IRQHandler

        PUBWEAK RESERVED5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RESERVED5_IRQHandler
        B RESERVED5_IRQHandler

        PUBWEAK RESERVED6_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RESERVED6_IRQHandler
        B RESERVED6_IRQHandler

        PUBWEAK RESERVED7_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RESERVED7_IRQHandler
        B RESERVED7_IRQHandler

        PUBWEAK RESERVED8_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RESERVED8_IRQHandler
        B RESERVED8_IRQHandler

        PUBWEAK PORT2_ALL_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT2_ALL_IRQHandler
        B PORT2_ALL_IRQHandler

        PUBWEAK PORT3_ALL_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT3_ALL_IRQHandler
        B PORT3_ALL_IRQHandler

        PUBWEAK TRNG_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TRNG_IRQHandler
        B TRNG_IRQHandler

        PUBWEAK UART2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART2_IRQHandler
        B UART2_IRQHandler

        PUBWEAK UART3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART3_IRQHandler
        B UART3_IRQHandler

        PUBWEAK ETHERNET_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
ETHERNET_IRQHandler
        B ETHERNET_IRQHandler

        PUBWEAK I2S_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2S_IRQHandler
        B I2S_IRQHandler

        PUBWEAK MPS2_SPI0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
MPS2_SPI0_IRQHandler
        B MPS2_SPI0_IRQHandler

        PUBWEAK MPS2_SPI1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
MPS2_SPI1_IRQHandler
        B MPS2_SPI1_IRQHandler

        PUBWEAK MPS2_SPI2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
MPS2_SPI2_IRQHandler
        B MPS2_SPI2_IRQHandler

        PUBWEAK MPS2_SPI3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
MPS2_SPI3_IRQHandler
        B MPS2_SPI3_IRQHandler

        PUBWEAK MPS2_SPI4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
MPS2_SPI4_IRQHandler
        B MPS2_SPI4_IRQHandler

        PUBWEAK PORT4_ALL_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT4_ALL_IRQHandler
        B PORT4_ALL_IRQHandler

        PUBWEAK PORT5_ALL_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PORT5_ALL_IRQHandler
        B PORT5_ALL_IRQHandler

        PUBWEAK UART4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART4_IRQHandler
        B UART4_IRQHandler


        END
