;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 ; Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
 ;
 ; Permission is hereby granted, free of charge, to any person obtaining a
 ; copy of this software and associated documentation files (the "Software"),
 ; to deal in the Software without restriction, including without limitation
 ; the rights to use, copy, modify, merge, publish, distribute, sublicense,
 ; and/or sell copies of the Software, and to permit persons to whom the
 ; Software is furnished to do so, subject to the following conditions:
 ;
 ; The above copyright notice and this permission notice shall be included
 ; in all copies or substantial portions of the Software.
 ;
 ; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 ; OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 ; MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 ; IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 ; OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 ; ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 ; OTHER DEALINGS IN THE SOFTWARE.
 ;
 ; Except as contained in this notice, the name of Maxim Integrated
 ; Products, Inc. shall not be used except as stated in the Maxim Integrated
 ; Products, Inc. Branding Policy.
 ;
 ; The mere transfer of this software does not imply any licenses
 ; of trade secrets, proprietary technology, copyrights, patents,
 ; trademarks, maskwork rights, or any other form of intellectual
 ; property whatsoever. Maxim Integrated Products, Inc. retains all
 ; ownership rights.
 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; To map FreeRTOS function names to their CMSIS equivalents add following lines to FreeRTOSConfig.h
; #define vPortSVCHandler SVC_Handler
; #define xPortPendSVHandler PendSV_Handler
; #define xPortSysTickHandler SysTick_Handler
; *------- <<< Use Configuration Wizard in Context Menu to Modify Stack Size and Heap Size. >>> ----

; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

__initial_sp    EQU     0x20040000  ; Top of RAM

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size
                                EXPORT  __isr_vector

                ; Core Level - CM4                  ; Most names are to help the FreeRTOS port.
__isr_vector
__Vectors       DCD     __initial_sp                ; Top of Stack
                DCD     Reset_Handler               ; Reset Handler
                DCD     NMI_Handler                 ; NMI Handler
                DCD     HardFault_Handler           ; Hard Fault Handler
                DCD     MemManage_Handler           ; MPU Fault Handler
                DCD     BusFault_Handler            ; Bus Fault Handler
                DCD     UsageFault_Handler          ; Usage Fault Handler
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     SVC_Handler                 ; SVCall Handler
                DCD     DebugMon_Handler            ; Debug Monitor Handler
                DCD     0                           ; Reserved
                DCD     PendSV_Handler              ; PendSV Handler
                DCD     SysTick_Handler             ; SysTick Handler

                ; Maxim MAX32620 external interrupts
                DCD     CLKMAN_IRQHandler           ; 16:00 CLKMAN
                DCD     PWRMAN_IRQHandler           ; 17:01 PWRMAN
                DCD     FLC_IRQHandler              ; 18:02 Flash Controller
                DCD     RTC0_IRQHandler             ; 19:03 RTC INT0
                DCD     RTC1_IRQHandler             ; 20:04 RTC INT1
                DCD     RTC2_IRQHandler             ; 21:05 RTC INT2
                DCD     RTC3_IRQHandler             ; 22:06 RTC INT3
                DCD     PMU_IRQHandler              ; 23:07 PMU
                DCD     USB_IRQHandler              ; 24:08 USB
                DCD     AES_IRQHandler              ; 25:09 AES
                DCD     MAA_IRQHandler              ; 26:10 MAA
                DCD     WDT0_IRQHandler             ; 27:11 WATCHDOG0
                DCD     WDT0_P_IRQHandler           ; 28:12 WATCHDOG0 PRE-WINDOW
                DCD     WDT1_IRQHandler             ; 29:13 WATCHDOG1
                DCD     WDT1_P_IRQHandler           ; 30:14 WATCHDOG1 PRE-WINDOW
                DCD     GPIO_P0_IRQHandler          ; 31:15 GPIO Port 0
                DCD     GPIO_P1_IRQHandler          ; 32:16 GPIO Port 1
                DCD     GPIO_P2_IRQHandler          ; 33:17 GPIO Port 2
                DCD     GPIO_P3_IRQHandler          ; 34:18 GPIO Port 3
                DCD     GPIO_P4_IRQHandler          ; 35:19 GPIO Port 4
                DCD     GPIO_P5_IRQHandler          ; 36:20 GPIO Port 5
                DCD     GPIO_P6_IRQHandler          ; 37:21 GPIO Port 6
                DCD     TMR0_IRQHandler             ; 38:22 Timer32-0
                DCD     TMR16_0_IRQHandler          ; 39:23 Timer16-s0
                DCD     TMR1_IRQHandler             ; 40:24 Timer32-1
                DCD     TMR16_1_IRQHandler          ; 41:25 Timer16-s1
                DCD     TMR2_IRQHandler             ; 42:26 Timer32-2
                DCD     TMR16_2_IRQHandler          ; 43:27 Timer16-s2
                DCD     TMR3_IRQHandler             ; 44:28 Timer32-3
                DCD     TMR16_3_IRQHandler          ; 45:29 Timer16-s3
                DCD     TMR4_IRQHandler             ; 46:30 Timer32-4
                DCD     TMR16_4_IRQHandler          ; 47:31 Timer16-s4
                DCD     TMR5_IRQHandler             ; 48:32 Timer32-5
                DCD     TMR16_5_IRQHandler          ; 49:33 Timer16-s5
                DCD     UART0_IRQHandler            ; 50:34 UART0
                DCD     UART1_IRQHandler            ; 51:35 UART1
                DCD     UART2_IRQHandler            ; 52:36 UART2
                DCD     UART3_IRQHandler            ; 53:37 UART3
                DCD     PT_IRQHandler               ; 54:38 PT
                DCD     I2CM0_IRQHandler            ; 55:39 I2C Master 0
                DCD     I2CM1_IRQHandler            ; 56:40 I2C Master 1
                DCD     I2CM2_IRQHandler            ; 57:41 I2C Master 2
                DCD     I2CS_IRQHandler             ; 58:42 I2C Slave
                DCD     SPIM0_IRQHandler            ; 59:43 SPIM0
                DCD     SPIM1_IRQHandler            ; 60:44 SPIM1
                DCD     SPIM2_IRQHandler            ; 61:45 SPIM2
                DCD     SPIB_IRQHandler             ; 62:46 SPI Bridge
                DCD     OWM_IRQHandler              ; 63:47 SPI Bridge
                DCD     AFE_IRQHandler              ; 64:48 AFE
                DCD     SPIS_IRQHandler             ; 65:49 SPI Slave
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

Reset_Handler   PROC
                EXPORT  Reset_Handler               [WEAK]
                IMPORT  PreInit
                IMPORT  SystemInit
                IMPORT  __main

                LDR     R0, =PreInit
                BLX     R0
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
__SPIN
                WFI
                BL      __SPIN
                ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler                 [WEAK]
                B       .
                ENDP

HardFault_Handler\
                                PROC
                EXPORT  HardFault_Handler           [WEAK]
                B       .
                ENDP

MemManage_Handler\
                                PROC
                EXPORT  MemManage_Handler           [WEAK]
                B       .
                ENDP
BusFault_Handler\
                                PROC
                EXPORT  BusFault_Handler            [WEAK]
                B       .
                ENDP

UsageFault_Handler\
                                PROC
                EXPORT  UsageFault_Handler          [WEAK]
                B       .
                ENDP

SVC_Handler\
                PROC
                EXPORT  SVC_Handler                 [WEAK]
                B       .
                ENDP

DebugMon_Handler\
                                PROC
                EXPORT  DebugMon_Handler            [WEAK]
                B       .
                ENDP

PendSV_Handler\
                PROC
                EXPORT  PendSV_Handler              [WEAK]
                B       .
                ENDP

SysTick_Handler\
                PROC
                EXPORT  SysTick_Handler             [WEAK]
                B       .
                ENDP

Default_Handler\
                PROC
                ; MAX32620 Interrupts
                EXPORT  CLKMAN_IRQHandler       [WEAK] ; 16:00 CLKMAN
                EXPORT  PWRMAN_IRQHandler       [WEAK] ; 17:01 PWRMAN
                EXPORT  FLC_IRQHandler          [WEAK] ; 18:02 Flash Controller
                EXPORT  RTC0_IRQHandler         [WEAK] ; 19:03 RTC INT0
                EXPORT  RTC1_IRQHandler         [WEAK] ; 20:04 RTC INT1
                EXPORT  RTC2_IRQHandler         [WEAK] ; 21:05 RTC INT2
                EXPORT  RTC3_IRQHandler         [WEAK] ; 22:06 RTC INT3
                EXPORT  PMU_IRQHandler          [WEAK] ; 23:07 PMU
                EXPORT  USB_IRQHandler          [WEAK] ; 24:08 USB
                EXPORT  AES_IRQHandler          [WEAK] ; 25:09 AES
                EXPORT  MAA_IRQHandler          [WEAK] ; 26:10 MAA
                EXPORT  WDT0_IRQHandler         [WEAK] ; 27:11 WATCHDOG0
                EXPORT  WDT0_P_IRQHandler       [WEAK] ; 28:12 WATCHDOG0 PRE-WINDOW
                EXPORT  WDT1_IRQHandler         [WEAK] ; 29:13 WATCHDOG1
                EXPORT  WDT1_P_IRQHandler       [WEAK] ; 30:14 WATCHDOG1 PRE-WINDOW
                EXPORT  GPIO_P0_IRQHandler      [WEAK] ; 31:15 GPIO Port 0
                EXPORT  GPIO_P1_IRQHandler      [WEAK] ; 32:16 GPIO Port 1
                EXPORT  GPIO_P2_IRQHandler      [WEAK] ; 33:17 GPIO Port 2
                EXPORT  GPIO_P3_IRQHandler      [WEAK] ; 34:18 GPIO Port 3
                EXPORT  GPIO_P4_IRQHandler      [WEAK] ; 35:19 GPIO Port 4
                EXPORT  GPIO_P5_IRQHandler      [WEAK] ; 36:20 GPIO Port 5
                EXPORT  GPIO_P6_IRQHandler      [WEAK] ; 37:21 GPIO Port 6
                EXPORT  TMR0_IRQHandler         [WEAK] ; 38:22 Timer32-0
                EXPORT  TMR16_0_IRQHandler      [WEAK] ; 39:23 Timer16-s0
                EXPORT  TMR1_IRQHandler         [WEAK] ; 40:24 Timer32-1
                EXPORT  TMR16_1_IRQHandler      [WEAK] ; 41:25 Timer16-s1
                EXPORT  TMR2_IRQHandler         [WEAK] ; 42:26 Timer32-2
                EXPORT  TMR16_2_IRQHandler      [WEAK] ; 43:27 Timer16-s2
                EXPORT  TMR3_IRQHandler         [WEAK] ; 44:28 Timer32-3
                EXPORT  TMR16_3_IRQHandler      [WEAK] ; 45:29 Timer16-s3
                EXPORT  TMR4_IRQHandler         [WEAK] ; 46:30 Timer32-4
                EXPORT  TMR16_4_IRQHandler      [WEAK] ; 47:31 Timer16-s4
                EXPORT  TMR5_IRQHandler         [WEAK] ; 48:32 Timer32-5
                EXPORT  TMR16_5_IRQHandler      [WEAK] ; 49:33 Timer16-s5
                EXPORT  UART0_IRQHandler        [WEAK] ; 50:34 UART0
                EXPORT  UART1_IRQHandler        [WEAK] ; 51:35 UART1
                EXPORT  UART2_IRQHandler        [WEAK] ; 52:36 UART2
                EXPORT  UART3_IRQHandler        [WEAK] ; 53:37 UART3
                EXPORT  PT_IRQHandler           [WEAK] ; 54:38 PT
                EXPORT  I2CM0_IRQHandler        [WEAK] ; 55:39 I2C Master 0
                EXPORT  I2CM1_IRQHandler        [WEAK] ; 56:40 I2C Master 1
                EXPORT  I2CM2_IRQHandler        [WEAK] ; 57:41 I2C Master 2
                EXPORT  I2CS_IRQHandler         [WEAK] ; 58:42 I2C Slave
                EXPORT  SPIM0_IRQHandler        [WEAK] ; 59:43 SPIM0
                EXPORT  SPIM1_IRQHandler        [WEAK] ; 60:44 SPIM1
                EXPORT  SPIM2_IRQHandler        [WEAK] ; 61:45 SPIM2
                EXPORT  SPIB_IRQHandler         [WEAK] ; 62:46 SPI Bridge
                EXPORT  OWM_IRQHandler          [WEAK] ; 63:47 SPI Bridge
                EXPORT  AFE_IRQHandler          [WEAK] ; 64:48 AFE
                EXPORT  SPIS_IRQHandler         [WEAK] ; 65:49 SPI Slave

CLKMAN_IRQHandler
PWRMAN_IRQHandler
FLC_IRQHandler
RTC0_IRQHandler
RTC1_IRQHandler
RTC2_IRQHandler
RTC3_IRQHandler
PMU_IRQHandler
USB_IRQHandler
AES_IRQHandler
MAA_IRQHandler
WDT0_IRQHandler
WDT0_P_IRQHandler
WDT1_IRQHandler
WDT1_P_IRQHandler
GPIO_P0_IRQHandler
GPIO_P1_IRQHandler
GPIO_P2_IRQHandler
GPIO_P3_IRQHandler
GPIO_P4_IRQHandler
GPIO_P5_IRQHandler
GPIO_P6_IRQHandler
TMR0_IRQHandler
TMR16_0_IRQHandler
TMR1_IRQHandler
TMR16_1_IRQHandler
TMR2_IRQHandler
TMR16_2_IRQHandler
TMR3_IRQHandler
TMR16_3_IRQHandler
TMR4_IRQHandler
TMR16_4_IRQHandler
TMR5_IRQHandler
TMR16_5_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
UART3_IRQHandler
PT_IRQHandler
I2CM0_IRQHandler
I2CM1_IRQHandler
I2CM2_IRQHandler
I2CS_IRQHandler
SPIM0_IRQHandler
SPIM1_IRQHandler
SPIM2_IRQHandler
SPIB_IRQHandler
OWM_IRQHandler
AFE_IRQHandler
SPIS_IRQHandler

                B .
                ENDP
                ALIGN
                END
