; mbed Microcontroller Library
; Copyright (c) 2016 u-blox.
;Licensed under the Apache License, Version 2.0 (the "License");
;you may not use this file except in compliance with the License.
;You may obtain a copy of the License at
;http://www.apache.org/licenses/LICENSE-2.0
;Unless required by applicable law or agreed to in writing, software
;distributed under the License is distributed on an "AS IS" BASIS,
;WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;See the License for the specific language governing permissions and
;limitations under the License.

; Description message

__initial_sp    EQU     (0x01000000 + 0x5000 - 256)

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

                EXPORT   IRQ0_RTC_Handler [WEAK]
                EXPORT   IRQ1_TMR0_Handler [WEAK]
                EXPORT   IRQ2_SECURITY_Handler [WEAK]
                EXPORT   IRQ3_PROTOCOL_Handler [WEAK]
                EXPORT   IRQ4_APPS_Handler [WEAK]
                EXPORT   IRQ5_GPIO_Handler [WEAK]
                EXPORT   IRQ6_DMA_Handler [WEAK]
                EXPORT   IRQ7_UART0_Handler [WEAK]
                EXPORT   IRQ8_UART1_Handler [WEAK]
                EXPORT   IRQ9_SSP0_Handler [WEAK]
                EXPORT   IRQ10_SSP1_Handler [WEAK]
                EXPORT   IRQ11_PWM0IN_Handler [WEAK]
                EXPORT   IRQ12_PWM0OUT_Handler [WEAK]
                EXPORT   IRQ13_PWM1IN_Handler [WEAK]
                EXPORT   IRQ14_PWM1OUT_Handler [WEAK]
                EXPORT   IRQ15_I2C_Handler [WEAK]
                EXPORT   IRQ16_LPUART_Handler [WEAK]
                EXPORT   IRQ17_CAP_Handler [WEAK]
IRQ0_RTC_Handler
IRQ1_TMR0_Handler
IRQ2_SECURITY_Handler
IRQ3_PROTOCOL_Handler
IRQ4_APPS_Handler
IRQ5_GPIO_Handler
IRQ6_DMA_Handler
IRQ7_UART0_Handler
IRQ8_UART1_Handler
IRQ9_SSP0_Handler
IRQ10_SSP1_Handler
IRQ11_PWM0IN_Handler
IRQ12_PWM0OUT_Handler
IRQ13_PWM1IN_Handler
IRQ14_PWM1OUT_Handler
IRQ15_I2C_Handler
IRQ16_LPUART_Handler
IRQ17_CAP_Handler

                B .
                ENDP
                ALIGN
                END
