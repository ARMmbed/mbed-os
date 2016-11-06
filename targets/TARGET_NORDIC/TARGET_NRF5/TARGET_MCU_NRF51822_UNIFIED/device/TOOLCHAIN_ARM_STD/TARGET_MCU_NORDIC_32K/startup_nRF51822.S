; mbed Microcontroller Library
; Copyright (c) 2013 Nordic Semiconductor.
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

__initial_sp	EQU     0x20008000


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
                DCD      POWER_CLOCK_IRQHandler ;POWER_CLOCK
                DCD      RADIO_IRQHandler ;RADIO
                DCD      UART0_IRQHandler_v ;UART0
                DCD      SPI0_TWI0_IRQHandler_v ;SPI0_TWI0
                DCD      SPI1_TWI1_IRQHandler_v ;SPI1_TWI1
                DCD      0 ;Reserved
                DCD      GPIOTE_IRQHandler_v ;GPIOTE
                DCD      ADC_IRQHandler_v ;ADC
                DCD      TIMER0_IRQHandler ;TIMER0
                DCD      TIMER1_IRQHandler_v ;TIMER1
                DCD      TIMER2_IRQHandler_v ;TIMER2
                DCD      RTC0_IRQHandler ;RTC0
                DCD      TEMP_IRQHandler ;TEMP
                DCD      RNG_IRQHandler ;RNG
                DCD      ECB_IRQHandler ;ECB
                DCD      CCM_AAR_IRQHandler ;CCM_AAR
                DCD      WDT_IRQHandler_v ;WDT
                DCD      RTC1_IRQHandler_v ;RTC1
                DCD      QDEC_IRQHandler_v ;QDEC
                DCD      LPCOMP_IRQHandler_v ;LPCOMP_COMP
                DCD      SWI0_IRQHandler_v ;SWI0
                DCD      SWI1_IRQHandler_v ;SWI1
                DCD      SWI2_IRQHandler_v ;SWI2
                DCD      SWI3_IRQHandler_v ;SWI3
                DCD      SWI4_IRQHandler ;SWI4
                DCD      SWI5_IRQHandler ;SWI5
                DCD      0 ;Reserved
                DCD      0 ;Reserved
                DCD      0 ;Reserved
                DCD      0 ;Reserved
                DCD      0 ;Reserved
                DCD      0 ;Reserved


__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; Reset Handler

NRF_POWER_RAMON_ADDRESS            EQU   0x40000524  ; NRF_POWER->RAMON address
NRF_POWER_RAMONB_ADDRESS           EQU   0x40000554  ; NRF_POWER->RAMONB address
NRF_POWER_RAMONx_RAMxON_ONMODE_Msk EQU   0x3         ; All RAM blocks on in onmode bit mask

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                IMPORT  nrf_reloc_vector_table
                
                MOVS    R1, #NRF_POWER_RAMONx_RAMxON_ONMODE_Msk
                
                LDR     R0, =NRF_POWER_RAMON_ADDRESS
                LDR     R2, [R0]
                ORRS    R2, R2, R1
                STR     R2, [R0]
                
                LDR     R0, =NRF_POWER_RAMONB_ADDRESS
                LDR     R2, [R0]
                ORRS    R2, R2, R1
                STR     R2, [R0]
                
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =nrf_reloc_vector_table
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

                EXPORT   POWER_CLOCK_IRQHandler [WEAK]
                EXPORT   RADIO_IRQHandler [WEAK]
                EXPORT   UART0_IRQHandler_v [WEAK]
                EXPORT   SPI0_TWI0_IRQHandler_v [WEAK]
                EXPORT   SPI1_TWI1_IRQHandler_v [WEAK]
                EXPORT   GPIOTE_IRQHandler_v [WEAK]
                EXPORT   ADC_IRQHandler_v [WEAK]
                EXPORT   TIMER0_IRQHandler [WEAK]
                EXPORT   TIMER1_IRQHandler_v [WEAK]
                EXPORT   TIMER2_IRQHandler_v [WEAK]
                EXPORT   RTC0_IRQHandler [WEAK]
                EXPORT   TEMP_IRQHandler [WEAK]
                EXPORT   RNG_IRQHandler [WEAK]
                EXPORT   ECB_IRQHandler [WEAK]
                EXPORT   CCM_AAR_IRQHandler [WEAK]
                EXPORT   WDT_IRQHandler_v [WEAK]
                EXPORT   RTC1_IRQHandler_v [WEAK]
                EXPORT   QDEC_IRQHandler_v [WEAK]
                EXPORT   LPCOMP_IRQHandler_v [WEAK]
                EXPORT   SWI0_IRQHandler_v [WEAK]
                EXPORT   SWI1_IRQHandler_v [WEAK]
                EXPORT   SWI2_IRQHandler_v [WEAK]
                EXPORT   SWI3_IRQHandler_v [WEAK]
                EXPORT   SWI4_IRQHandler [WEAK]
                EXPORT   SWI5_IRQHandler [WEAK]
POWER_CLOCK_IRQHandler
RADIO_IRQHandler
UART0_IRQHandler_v
SPI0_TWI0_IRQHandler_v
SPI1_TWI1_IRQHandler_v
GPIOTE_IRQHandler_v
ADC_IRQHandler_v
TIMER0_IRQHandler
TIMER1_IRQHandler_v
TIMER2_IRQHandler_v
RTC0_IRQHandler
TEMP_IRQHandler
RNG_IRQHandler
ECB_IRQHandler
CCM_AAR_IRQHandler
WDT_IRQHandler_v
RTC1_IRQHandler_v
QDEC_IRQHandler_v
LPCOMP_IRQHandler_v
SWI0_IRQHandler_v
SWI1_IRQHandler_v
SWI2_IRQHandler_v
SWI3_IRQHandler_v
SWI4_IRQHandler
SWI5_IRQHandler

                B .
                ENDP
                ALIGN
                END

