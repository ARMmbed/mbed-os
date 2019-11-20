; MPS2 CMSIS Library
;
; Copyright (c) 2006-2018 ARM Limited
; All rights reserved.
;
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the following conditions are met:
;
; 1. Redistributions of source code must retain the above copyright notice,
; this list of conditions and the following disclaimer.
;
; 2. Redistributions in binary form must reproduce the above copyright notice,
; this list of conditions and the following disclaimer in the documentation
; and/or other materials provided with the distribution.
;
; 3. Neither the name of the copyright holder nor the names of its contributors
; may be used to endorse or promote products derived from this software without
; specific prior written permission.
;
; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
; AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
; ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
; LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
; CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
; SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
; INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
; CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
; ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
; POSSIBILITY OF SUCH DAMAGE.
;******************************************************************************
; @file     startup_CMSDK_CM3.s
; @brief    CMSIS Core Device Startup File for
;           CMSDK_CM3 Device
;
;******************************************************************************

                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size
                IMPORT  |Image$$ARM_LIB_STACK$$ZI$$Limit|

__Vectors       DCD     |Image$$ARM_LIB_STACK$$ZI$$Limit| ; Top of Stack
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
                EXPORT UARTRX0_Handler            [WEAK]
                EXPORT UARTTX0_Handler            [WEAK]
                EXPORT UARTRX1_Handler            [WEAK]
                EXPORT UARTTX1_Handler            [WEAK]
                EXPORT UARTRX2_Handler            [WEAK]
                EXPORT UARTTX2_Handler            [WEAK]
                EXPORT PORT0_COMB_Handler         [WEAK]
                EXPORT PORT1_COMB_Handler         [WEAK]
                EXPORT TIMER0_Handler             [WEAK]
                EXPORT TIMER1_Handler             [WEAK]
                EXPORT DUALTIMER_HANDLER          [WEAK]
                EXPORT SPI_Handler                [WEAK]
                EXPORT UARTOVF_Handler            [WEAK]
                EXPORT ETHERNET_Handler           [WEAK]
                EXPORT I2S_Handler                [WEAK]
                EXPORT TSC_Handler                [WEAK]
                EXPORT PORT2_COMB_Handler         [WEAK]
                EXPORT PORT3_COMB_Handler         [WEAK]
                EXPORT UARTRX3_Handler            [WEAK]
                EXPORT UARTTX3_Handler            [WEAK]
                EXPORT UARTRX4_Handler            [WEAK]
                EXPORT UARTTX4_Handler            [WEAK]
                EXPORT ADCSPI_Handler             [WEAK]
                EXPORT SHIELDSPI_Handler          [WEAK]
                EXPORT PORT0_0_Handler            [WEAK]
                EXPORT PORT0_1_Handler            [WEAK]
                EXPORT PORT0_2_Handler            [WEAK]
                EXPORT PORT0_3_Handler            [WEAK]
                EXPORT PORT0_4_Handler            [WEAK]
                EXPORT PORT0_5_Handler            [WEAK]
                EXPORT PORT0_6_Handler            [WEAK]
                EXPORT PORT0_7_Handler            [WEAK]

UARTRX0_Handler
UARTTX0_Handler
UARTRX1_Handler
UARTTX1_Handler
UARTRX2_Handler
UARTTX2_Handler
PORT0_COMB_Handler
PORT1_COMB_Handler
TIMER0_Handler
TIMER1_Handler
DUALTIMER_HANDLER
SPI_Handler
UARTOVF_Handler
ETHERNET_Handler
I2S_Handler
TSC_Handler
PORT2_COMB_Handler       
PORT3_COMB_Handler       
UARTRX3_Handler          
UARTTX3_Handler          
UARTRX4_Handler          
UARTTX4_Handler          
ADCSPI_Handler           
SHIELDSPI_Handler        
PORT0_0_Handler                                   
PORT0_1_Handler                                   
PORT0_2_Handler                                   
PORT0_3_Handler                                   
PORT0_4_Handler                                   
PORT0_5_Handler                                   
PORT0_6_Handler                                   
PORT0_7_Handler                                   
                B       .

                ENDP


                ALIGN

                END
