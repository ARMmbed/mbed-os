; mbed Microcontroller Library
; Copyright (c) 2016 u-blox.
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
; http://www.apache.org/licenses/LICENSE-2.0
; Unless required by applicable law or agreed to in writing, software
; distributed under the License is distributed on an "AS IS" BASIS,
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; See the License for the specific language governing permissions and
; limitations under the License.

; Description message

        MODULE  ?cstartup

        ; Stack size default : 1024
        ; Heap size default : 2048

        ; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler
        DCD     NMI_Handler
        DCD     HardFault_Handler
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     SVC_Handler
        DCD     0
        DCD     0
        DCD     PendSV_Handler
        DCD     SysTick_Handler

; External Interrupts
        DCD      IRQ0_RTC_Handler
        DCD      IRQ1_TMR0_Handler
        DCD      IRQ2_SECURITY_Handler
        DCD      IRQ3_PROTOCOL_Handler
        DCD      IRQ4_APPS_Handler
        DCD      IRQ5_GPIO_Handler
        DCD      IRQ6_DMA_Handler
        DCD      IRQ7_UART0_Handler
        DCD      IRQ8_UART1_Handler
        DCD      IRQ9_SSP0_Handler
        DCD      IRQ10_SSP1_Handler
        DCD      IRQ11_PWM0IN_Handler
        DCD      IRQ12_PWM0OUT_Handler
        DCD      IRQ13_PWM1IN_Handler
        DCD      IRQ14_PWM1OUT_Handler
        DCD      IRQ15_I2C_Handler
        DCD      IRQ16_LPUART_Handler
        DCD      IRQ17_CAP_Handler
        DCD      0
        DCD      0
        DCD      0
        DCD      0
        DCD      0
        DCD      0
        DCD      0
        DCD      0
        DCD      0
        DCD      0
        DCD      0
        DCD      0
        DCD      0
        DCD      0

__Vectors_End
__Vectors                           EQU   __vector_table
__Vectors_Size                      EQU   __Vectors_End - __Vectors

; Default handlers.
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        ; Dummy exception handlers

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B .

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B .

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B .

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B .

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B .

       ; Dummy interrupt handlers

        PUBWEAK  IRQ0_RTC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ0_RTC_Handler
        B .
        PUBWEAK  IRQ1_TMR0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ1_TMR0_Handler
        B .
        PUBWEAK  IRQ2_SECURITY_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ2_SECURITY_Handler
        B .
        PUBWEAK  IRQ3_PROTOCOL_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ3_PROTOCOL_Handler
        B .
        PUBWEAK  IRQ4_APPS_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ4_APPS_Handler
        B .
        PUBWEAK  IRQ5_GPIO_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ5_GPIO_Handler
        B .
        PUBWEAK  IRQ6_DMA_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ6_DMA_Handler
        B .
        PUBWEAK  IRQ7_UART0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ7_UART0_Handler
        B .
        PUBWEAK  IRQ8_UART1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ8_UART1_Handler
        B .
        PUBWEAK  IRQ9_SSP0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ9_SSP0_Handler
        B .
        PUBWEAK  IRQ10_SSP1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ10_SSP1_Handler
        B .
        PUBWEAK  IRQ11_PWM0IN_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ11_PWM0IN_Handler
        B .
        PUBWEAK  IRQ12_PWM0OUT_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ12_PWM0OUT_Handler
        B .
        PUBWEAK  IRQ13_PWM1IN_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ13_PWM1IN_Handler
        B .
        PUBWEAK  IRQ14_PWM1OUT_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ14_PWM1OUT_Handler
        B .
        PUBWEAK  IRQ15_I2C_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ15_I2C_Handler
        B .
        PUBWEAK  IRQ16_LPUART_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ16_LPUART_Handler
        B .
        PUBWEAK  IRQ17_CAP_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
IRQ17_CAP_Handler

        END
