; MPS2 CMSIS Library
;
; Copyright (c) 2006-2016 ARM Limited
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
; @file     startup_CMSDK_BEID.s
; @brief    CMSIS Core Device Startup File for
;           CMSDK_BEID Device
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
                DCD     UART0_IRQHandler        ;   0:UART 0 RX and TX Combined Interrupt
                DCD     Spare_IRQHandler        ;   1:Undefined
                DCD     UART1_IRQHandler        ;   2:UART 1 RX and TX Combined Interrupt
                DCD     I2C0_IRQHandler         ;   3:I2C 0 Interrupt
                DCD     I2C1_IRQHandler         ;   4:I2C 1 Interrupt
                DCD     RTC_IRQHandler          ;   5:RTC Interrupt
                DCD     PORT0_IRQHandler        ;   6:GPIO Port 0 combined Interrupt
                DCD     PORT1_ALL_IRQHandler    ;   7:GPIO Port 1 combined Interrupt
                DCD     TIMER0_IRQHandler       ;   8:TIMER 0 Interrupt
                DCD     TIMER1_IRQHandler       ;   9:TIMER 1 Interrupt
                DCD     DUALTIMER_IRQHandler    ;   10:Dual Timer Interrupt
                DCD     SPI0_IRQHandler         ;   11:SPI 0 Interrupt
                DCD     UARTOVF_IRQHandler      ;   12:UART 0,1,2 Overflow Interrupt
                DCD     SPI1_IRQHandler         ;   13:SPI 1 Interrupt
                DCD     RESERVED0_IRQHandler    ;   14:Reserved
                DCD     TSC_IRQHandler          ;   15:Touch Screen Interrupt
                DCD     PORT01_0_IRQHandler     ;   16:GPIO Port 0 pin 0 Handler
                DCD     PORT01_1_IRQHandler     ;   17:GPIO Port 0 pin 1 Handler
                DCD     PORT01_2_IRQHandler     ;   18:GPIO Port 0 pin 2 Handler
                DCD     PORT01_3_IRQHandler     ;   19:GPIO Port 0 pin 3 Handler
                DCD     PORT01_4_IRQHandler     ;   20:GPIO Port 0 pin 4 Handler
                DCD     PORT01_5_IRQHandler     ;   21:GPIO Port 0 pin 5 Handler
                DCD     PORT01_6_IRQHandler     ;   22:GPIO Port 0 pin 6 Handler
                DCD     PORT01_7_IRQHandler     ;   23:GPIO Port 0 pin 7 Handler
                DCD     PORT01_8_IRQHandler     ;   24:GPIO Port 0 pin 8 Handler
                DCD     PORT01_9_IRQHandler     ;   25:GPIO Port 0 pin 9 Handler
                DCD     PORT01_10_IRQHandler    ;   26:GPIO Port 0 pin 10 Handler
                DCD     PORT01_11_IRQHandler    ;   27:GPIO Port 0 pin 11 Handler
                DCD     PORT01_12_IRQHandler    ;   28:GPIO Port 0 pin 12 Handler
                DCD     PORT01_13_IRQHandler    ;   29:GPIO Port 0 pin 13 Handler
                DCD     PORT01_14_IRQHandler    ;   30:GPIO Port 0 pin 14 Handler
                DCD     PORT01_15_IRQHandler    ;   31:GPIO Port 0 pin 15 Handler
                DCD     SYSERROR_IRQHandler     ;   32:System Error Interrupt
                DCD     EFLASH_IRQHandler       ;   33:Embedded Flash Interrupt
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

                EXPORT     UART0_IRQHandler        [WEAK]       ;   0:UART 0 RX and TX Combined Interrupt
                EXPORT     Spare_IRQHandler        [WEAK]       ;   1:Undefined
                EXPORT     UART1_IRQHandler        [WEAK]       ;   2:UART 1 RX and TX Combined Interrupt
                EXPORT     I2C0_IRQHandler         [WEAK]       ;   3:I2C 0 Interrupt
                EXPORT     I2C1_IRQHandler         [WEAK]       ;   4:I2C 1 Interrupt
                EXPORT     RTC_IRQHandler          [WEAK]       ;   5:RTC Interrupt
                EXPORT     PORT0_IRQHandler        [WEAK]       ;   6:GPIO Port 0 combined Interrupt
                EXPORT     PORT1_ALL_IRQHandler    [WEAK]       ;   7:GPIO Port 1 combined Interrupt
                EXPORT     TIMER0_IRQHandler       [WEAK]       ;   8:TIMER 0 Interrupt
                EXPORT     TIMER1_IRQHandler       [WEAK]       ;   9:TIMER 1 Interrupt
                EXPORT     DUALTIMER_IRQHandler    [WEAK]       ;   10:Dual Timer Interrupt
                EXPORT     SPI0_IRQHandler         [WEAK]       ;   11:SPI 0 Interrupt
                EXPORT     UARTOVF_IRQHandler      [WEAK]       ;   12:UART 0,1,2 Overflow Interrupt
                EXPORT     SPI1_IRQHandler         [WEAK]       ;   13:SPI 1 Interrupt
                EXPORT     RESERVED0_IRQHandler    [WEAK]       ;   14:Reserved
                EXPORT     TSC_IRQHandler          [WEAK]       ;   15:Touch Screen Interrupt
                EXPORT     PORT01_0_IRQHandler     [WEAK]       ;   16:GPIO Port 0 pin 0 Handler
                EXPORT     PORT01_1_IRQHandler     [WEAK]       ;   17:GPIO Port 0 pin 1 Handler
                EXPORT     PORT01_2_IRQHandler     [WEAK]       ;   18:GPIO Port 0 pin 2 Handler
                EXPORT     PORT01_3_IRQHandler     [WEAK]       ;   19:GPIO Port 0 pin 3 Handler
                EXPORT     PORT01_4_IRQHandler     [WEAK]       ;   20:GPIO Port 0 pin 4 Handler
                EXPORT     PORT01_5_IRQHandler     [WEAK]       ;   21:GPIO Port 0 pin 5 Handler
                EXPORT     PORT01_6_IRQHandler     [WEAK]       ;   22:GPIO Port 0 pin 6 Handler
                EXPORT     PORT01_7_IRQHandler     [WEAK]       ;   23:GPIO Port 0 pin 7 Handler
                EXPORT     PORT01_8_IRQHandler     [WEAK]       ;   24:GPIO Port 0 pin 8 Handler
                EXPORT     PORT01_9_IRQHandler     [WEAK]       ;   25:GPIO Port 0 pin 9 Handler
                EXPORT     PORT01_10_IRQHandler    [WEAK]       ;   26:GPIO Port 0 pin 10 Handler
                EXPORT     PORT01_11_IRQHandler    [WEAK]       ;   27:GPIO Port 0 pin 11 Handler
                EXPORT     PORT01_12_IRQHandler    [WEAK]       ;   28:GPIO Port 0 pin 12 Handler
                EXPORT     PORT01_13_IRQHandler    [WEAK]       ;   29:GPIO Port 0 pin 13 Handler
                EXPORT     PORT01_14_IRQHandler    [WEAK]       ;   30:GPIO Port 0 pin 14 Handler
                EXPORT     PORT01_15_IRQHandler    [WEAK]       ;   31:GPIO Port 0 pin 15 Handler
                EXPORT     SYSERROR_IRQHandler     [WEAK]       ;   32:System Error Interrupt
                EXPORT     EFLASH_IRQHandler       [WEAK]       ;   33:Embedded Flash Interrupt
                EXPORT     RESERVED1_IRQHandler    [WEAK]       ;   34:Reserved
                EXPORT     RESERVED2_IRQHandler    [WEAK]       ;   35:Reserved
                EXPORT     RESERVED3_IRQHandler    [WEAK]       ;   36:Reserved
                EXPORT     RESERVED4_IRQHandler    [WEAK]       ;   37:Reserved
                EXPORT     RESERVED5_IRQHandler    [WEAK]       ;   38:Reserved
                EXPORT     RESERVED6_IRQHandler    [WEAK]       ;   39:Reserved
                EXPORT     RESERVED7_IRQHandler    [WEAK]       ;   40:Reserved
                EXPORT     RESERVED8_IRQHandler    [WEAK]       ;   41:Reserved
                EXPORT     PORT2_ALL_IRQHandler    [WEAK]       ;   42:GPIO Port 2 combined Interrupt
                EXPORT     PORT3_ALL_IRQHandler    [WEAK]       ;   43:GPIO Port 3 combined Interrupt
                EXPORT     TRNG_IRQHandler         [WEAK]       ;   44:Random number generator Interrupt
                EXPORT     UART2_IRQHandler        [WEAK]       ;   45:UART 2 RX and TX Combined Interrupt
                EXPORT     UART3_IRQHandler        [WEAK]       ;   46:UART 3 RX and TX Combined Interrupt
                EXPORT     ETHERNET_IRQHandler     [WEAK]       ;   47:Ethernet interrupt     t.b.a.
                EXPORT     I2S_IRQHandler          [WEAK]       ;   48:I2S Interrupt
                EXPORT     MPS2_SPI0_IRQHandler    [WEAK]       ;   49:SPI Interrupt (spi header)
                EXPORT     MPS2_SPI1_IRQHandler    [WEAK]       ;   50:SPI Interrupt (clcd)
                EXPORT     MPS2_SPI2_IRQHandler    [WEAK]       ;   51:SPI Interrupt (spi 1 ADC replacement)
                EXPORT     MPS2_SPI3_IRQHandler    [WEAK]       ;   52:SPI Interrupt (spi 0 shield 0 replacement)
                EXPORT     MPS2_SPI4_IRQHandler    [WEAK]       ;   53:SPI Interrupt  (shield 1)

UART0_IRQHandler
Spare_IRQHandler
UART1_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
RTC_IRQHandler
PORT0_IRQHandler
PORT1_ALL_IRQHandler
TIMER0_IRQHandler
TIMER1_IRQHandler
DUALTIMER_IRQHandler
SPI0_IRQHandler
UARTOVF_IRQHandler
SPI1_IRQHandler
RESERVED0_IRQHandler
TSC_IRQHandler
PORT01_0_IRQHandler
PORT01_1_IRQHandler
PORT01_2_IRQHandler
PORT01_3_IRQHandler
PORT01_4_IRQHandler
PORT01_5_IRQHandler
PORT01_6_IRQHandler
PORT01_7_IRQHandler
PORT01_8_IRQHandler
PORT01_9_IRQHandler
PORT01_10_IRQHandler
PORT01_11_IRQHandler
PORT01_12_IRQHandler
PORT01_13_IRQHandler
PORT01_14_IRQHandler
PORT01_15_IRQHandler
SYSERROR_IRQHandler
EFLASH_IRQHandler
RESERVED1_IRQHandler
RESERVED2_IRQHandler
RESERVED3_IRQHandler
RESERVED4_IRQHandler
RESERVED5_IRQHandler
RESERVED6_IRQHandler
RESERVED7_IRQHandler
RESERVED8_IRQHandler
PORT2_ALL_IRQHandler
PORT3_ALL_IRQHandler
TRNG_IRQHandler
UART2_IRQHandler
UART3_IRQHandler
ETHERNET_IRQHandler
I2S_IRQHandler
MPS2_SPI0_IRQHandler
MPS2_SPI1_IRQHandler
MPS2_SPI2_IRQHandler
MPS2_SPI3_IRQHandler
MPS2_SPI4_IRQHandler
                B       .

                ENDP


                ALIGN


                END
