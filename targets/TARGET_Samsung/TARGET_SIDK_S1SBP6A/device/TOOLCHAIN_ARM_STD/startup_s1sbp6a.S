/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

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
                DCD     BOR_Handler             ; 00: Brown Out Reset Handler
                DCD     UART0_Handler           ; 01: UART 0 Handler
                DCD     UART1_Handler           ; 02: UART 1 Handler
                DCD     UART2_Handler           ; 03: UART 2 Handler
                DCD     SPI0_Handler            ; 04: SPI 0 Handler
                DCD     SPI1_Handler            ; 05: SPI 1 Handler
                DCD     SPI2_Handler            ; 06: SPI 2 Handler
                DCD     SPI3_Handler            ; 07: SPI 3 Handler
                DCD     SPI4_Handler            ; 08: SPI 4 Handler
                DCD     I2C0_Handler            ; 09: I2C 0 Handler
                DCD     I2C1_Handler            ; 10: I2C 1 Handler
                DCD     I2C2_Handler            ; 11: I2C 2 Handler
                DCD     I2C3_Handler            ; 12: I2C 3 Handler
                DCD     I2C4_Handler            ; 13: I2C 4 Handler
                DCD     TIMER0_Handler          ; 14: TIMER 0 Handler
                DCD     TIMER1_Handler          ; 15: TIMER 1 Handler
                DCD     TIMER2_Handler          ; 16: TIMER 2 Handler
                DCD     TIMER3_Handler          ; 17: TIMER 3 Handler
                DCD     TIMER4_Handler          ; 18: TIMER 4 Handler
                DCD     TIMER5_Handler          ; 19: TIMER 5 Handler
                DCD     TIMER6_Handler          ; 20: TIMER 6 Handler
                DCD     PWM_TIMER_Handler       ; 21: PWM TIMER Handler
                DCD     DUAL_TIMER_Handler      ; 22: Dual TIMER Handler
                DCD     AFECON_Handler          ; 23: Analog Front End Control Handler
                DCD     SRC_Handler             ; 24: Sample Rate Converter Handler
                DCD     DMA_ERROR_Handler       ; 25: DMA Error Handler
                DCD     FLASH_Handler           ; 26: FLASH Handler
                DCD     RTC0_Handler            ; 27: Real-Time Counter 0 Handler
                DCD     RTC1_Handler            ; 28: Real-Time Counter 1 Handler
                DCD     RTC2_Handler            ; 29: Real-Time Counter 2 Handler
                DCD     FPU_Handler             ; 30: Cortex-M4 Floating-point Processing Unit exceptions Handler
                DCD     SRP0_Handler            ; 31: SRP 0 Handler
                DCD     SRP1_Handler            ; 32: SRP 1 Handler
                DCD     DELAY_MON_Handler       ; 33: Delay Monitor Handler
                DCD     AES_Handler             ; 34: AES Handler
                DCD     EXT_WAKEUP_Handler      ; 35: External Wakeup Handler
                DCD     DMA0_Handler            ; 36: DMA 0 Handler
                DCD     DMA1_Handler            ; 37: DMA 1 Handler
                DCD     DMA2_Handler            ; 38: DMA 2 Handler
                DCD     DMA3_Handler            ; 39: DMA 3 Handler
                DCD     DMA4_Handler            ; 40: DMA 4 Handler
                DCD     DMA5_Handler            ; 41: DMA 5 Handler
                DCD     DMA6_Handler            ; 42: DMA 6 Handler
                DCD     DMA7_Handler            ; 43: DMA 7 Handler
                DCD     DMA8_Handler            ; 44: DMA 8 Handler
                DCD     DMA9_Handler            ; 45: DMA 9 Handler
                DCD     DMA10_Handler           ; 46: DMA 10 Handler
                DCD     DMA11_Handler           ; 47: DMA 11 Handler
                DCD     DMA12_Handler           ; 48: DMA 12 Handler
                DCD     DMA13_Handler           ; 49: DMA 13 Handler
                DCD     DMA14_Handler           ; 50: DMA 14 Handler
                DCD     DMA15_Handler           ; 51: DMA 15 Handler
                DCD     DMA16_Handler           ; 52: DMA 16 Handler
                DCD     DMA17_Handler           ; 53: DMA 17 Handler
                DCD     DMA18_Handler           ; 54: DMA 18 Handler
                DCD     DMA19_Handler           ; 55: DMA 19 Handler
                DCD     DMA20_Handler           ; 56: DMA 20 Handler
                DCD     DMA21_Handler           ; 57: DMA 21 Handler
                DCD     DMA22_Handler           ; 58: DMA 22 Handler
                DCD     DMA23_Handler           ; 59: DMA 23 Handler
                DCD     DMA24_Handler           ; 60: DMA 24 Handler
                DCD     DMA25_Handler           ; 61: DMA 25 Handler
                DCD     DMA26_Handler           ; 62: DMA 26 Handler
                DCD     DMA27_Handler           ; 63: DMA 27 Handler
                DCD     DMA28_Handler           ; 64: DMA 28 Handler
                DCD     DMA29_Handler           ; 65: DMA 29 Handler
                DCD     DMA30_Handler           ; 66: DMA 30 Handler
                DCD     DMA31_Handler           ; 67: DMA 31 Handler
                DCD     PORT0_Handler           ; 68: PORT0 Handler
                DCD     PORT1_Handler           ; 69: PORT1 Handler
                DCD     PORT2_Handler           ; 70: PORT2 Handler
                DCD     PORT0_0_Handler         ; 71: GPIO Port 0 pin 0 Handler
                DCD     PORT0_1_Handler         ; 72: GPIO Port 0 pin 1 Handler
                DCD     PORT0_2_Handler         ; 73: GPIO Port 0 pin 2 Handler
                DCD     PORT0_3_Handler         ; 74: GPIO Port 0 pin 3 Handler
                DCD     PORT0_4_Handler         ; 75: GPIO Port 0 pin 4 Handler
                DCD     PORT0_5_Handler         ; 76: GPIO Port 0 pin 5 Handler
                DCD     PORT0_6_Handler         ; 77: GPIO Port 0 pin 6 Handler
                DCD     PORT0_7_Handler         ; 78: GPIO Port 0 pin 7 Handler
                DCD     PORT0_8_Handler         ; 79: GPIO Port 0 pin 8 Handler
                DCD     PORT0_9_Handler         ; 80: GPIO Port 0 pin 9 Handler
                DCD     PORT0_10_Handler        ; 81: GPIO Port 0 pin 10 Handler
                DCD     PORT0_11_Handler        ; 82: GPIO Port 0 pin 11 Handler
                DCD     PORT0_12_Handler        ; 83: GPIO Port 0 pin 12 Handler
                DCD     PORT0_13_Handler        ; 84: GPIO Port 0 pin 13 Handler
                DCD     PORT0_14_Handler        ; 85: GPIO Port 0 pin 14 Handler
                DCD     PORT0_15_Handler        ; 86: GPIO Port 0 pin 15 Handler
                DCD     PORT1_0_Handler         ; 87: GPIO Port 1 pin 0 Handler
                DCD     PORT1_1_Handler         ; 88: GPIO Port 1 pin 1 Handler
                DCD     PORT1_2_Handler         ; 89: GPIO Port 1 pin 2 Handler
                DCD     PORT1_3_Handler         ; 90: GPIO Port 1 pin 3 Handler
                DCD     PORT1_4_Handler         ; 91: GPIO Port 1 pin 4 Handler
                DCD     PORT1_5_Handler         ; 92: GPIO Port 1 pin 5 Handler
                DCD     PORT1_6_Handler         ; 93: GPIO Port 1 pin 6 Handler
                DCD     PORT1_7_Handler         ; 94: GPIO Port 1 pin 7 Handler
                DCD     PORT1_8_Handler         ; 95: GPIO Port 1 pin 8 Handler
                DCD     PORT1_9_Handler         ; 96: GPIO Port 1 pin 9 Handler
                DCD     PORT1_10_Handler        ; 97: GPIO Port 1 pin 10 Handler
                DCD     PORT1_11_Handler        ; 98: GPIO Port 1 pin 11 Handler
                DCD     PORT1_12_Handler        ; 99: GPIO Port 1 pin 12 Handler
                DCD     PORT1_13_Handler        ; 100: GPIO Port 1 pin 13 Handler
                DCD     PORT1_14_Handler        ; 101: GPIO Port 1 pin 14 Handler
                DCD     PORT1_15_Handler        ; 102: GPIO Port 1 pin 15 Handler
                DCD     PORT2_0_Handler         ; 103: GPIO Port 2 pin 0 Handler
                DCD     PORT2_1_Handler         ; 104: GPIO Port 2 pin 1 Handler
                DCD     PORT2_2_Handler         ; 105: GPIO Port 2 pin 2 Handler
                DCD     PORT2_3_Handler         ; 106: GPIO Port 2 pin 3 Handler
                DCD     PORT2_4_Handler         ; 107: GPIO Port 2 pin 4 Handler
                DCD     PORT2_5_Handler         ; 108: GPIO Port 2 pin 5 Handler
                DCD     PORT2_6_Handler         ; 109: GPIO Port 2 pin 6 Handler
                DCD     PORT2_7_Handler         ; 110: GPIO Port 2 pin 7 Handler
                DCD     PORT2_8_Handler         ; 111: GPIO Port 2 pin 8 Handler
                DCD     PORT2_9_Handler         ; 112: GPIO Port 2 pin 9 Handler
                DCD     PORT2_10_Handler        ; 113: GPIO Port 2 pin 10 Handler
                DCD     PORT2_11_Handler        ; 114: GPIO Port 2 pin 11 Handler
                DCD     PORT2_12_Handler        ; 115: GPIO Port 2 pin 12 Handler
                DCD     PORT2_13_Handler        ; 116: GPIO Port 2 pin 13 Handler
                DCD     PORT2_14_Handler        ; 117: GPIO Port 2 pin 14 Handler
                DCD     PORT2_15_Handler        ; 118: GPIO Port 2 pin 15 Handler
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT   Reset_Handler             [WEAK]
                IMPORT   SystemInit
                IMPORT   __main

                LDR      R0, =SystemInit
                BLX      R0
                LDR      R0, =__main
                BX       R0
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


; Default exception/interrupt handler
Default_Handler PROC
                EXPORT  BOR_Handler             [WEAK]   ; 00: Brown Out Reset Handler
                EXPORT  UART0_Handler           [WEAK]   ; 01: UART 0 Handler
                EXPORT  UART1_Handler           [WEAK]   ; 02: UART 1 Handler
                EXPORT  UART2_Handler           [WEAK]   ; 03: UART 2 Handler
                EXPORT  SPI0_Handler            [WEAK]   ; 04: SPI 0 Handler
                EXPORT  SPI1_Handler            [WEAK]   ; 05: SPI 1 Handler
                EXPORT  SPI2_Handler            [WEAK]   ; 06: SPI 2 Handler
                EXPORT  SPI3_Handler            [WEAK]   ; 07: SPI 3 Handler
                EXPORT  SPI4_Handler            [WEAK]   ; 08: SPI 4 Handler
                EXPORT  I2C0_Handler            [WEAK]   ; 09: I2C 0 Handler
                EXPORT  I2C1_Handler            [WEAK]   ; 10: I2C 1 Handler
                EXPORT  I2C2_Handler            [WEAK]   ; 11: I2C 2 Handler
                EXPORT  I2C3_Handler            [WEAK]   ; 12: I2C 3 Handler
                EXPORT  I2C4_Handler            [WEAK]   ; 13: I2C 4 Handler
                EXPORT  TIMER0_Handler          [WEAK]   ; 14: TIMER 0 Handler
                EXPORT  TIMER1_Handler          [WEAK]   ; 15: TIMER 1 Handler
                EXPORT  TIMER2_Handler          [WEAK]   ; 16: TIMER 2 Handler
                EXPORT  TIMER3_Handler          [WEAK]   ; 17: TIMER 3 Handler
                EXPORT  TIMER4_Handler          [WEAK]   ; 18: TIMER 4 Handler
                EXPORT  TIMER5_Handler          [WEAK]   ; 19: TIMER 5 Handler
                EXPORT  TIMER6_Handler          [WEAK]   ; 20: TIMER 6 Handler
                EXPORT  PWM_TIMER_Handler       [WEAK]   ; 21: PWM TIMER Handler
                EXPORT  DUAL_TIMER_Handler      [WEAK]   ; 22: Dual TIMER Handler
                EXPORT  AFECON_Handler          [WEAK]   ; 23: Analog Front End Control Handler
                EXPORT  SRC_Handler             [WEAK]   ; 24: Sample Rate Converter Handler
                EXPORT  DMA_ERROR_Handler       [WEAK]   ; 25: DMA Error Handler
                EXPORT  FLASH_Handler           [WEAK]   ; 26: FLASH Handler
                EXPORT  RTC0_Handler            [WEAK]   ; 27: Real-Time Counter 0 Handler
                EXPORT  RTC1_Handler            [WEAK]   ; 28: Real-Time Counter 1 Handler
                EXPORT  RTC2_Handler            [WEAK]   ; 29: Real-Time Counter 2 Handler
                EXPORT  FPU_Handler             [WEAK]   ; 30: Cortex-M4 Floating-point Processing Unit exceptions Handler
                EXPORT  SRP0_Handler            [WEAK]   ; 31: SRP 0 Handler
                EXPORT  SRP1_Handler            [WEAK]   ; 32: SRP 1 Handler
                EXPORT  DELAY_MON_Handler       [WEAK]   ; 33: Delay Monitor Handler
                EXPORT  AES_Handler             [WEAK]   ; 34: AES Handler
                EXPORT  EXT_WAKEUP_Handler      [WEAK]   ; 35: External Wakeup Handler
                EXPORT  DMA0_Handler            [WEAK]   ; 36: DMA 0 Handler
                EXPORT  DMA1_Handler            [WEAK]   ; 37: DMA 1 Handler
                EXPORT  DMA2_Handler            [WEAK]   ; 38: DMA 2 Handler
                EXPORT  DMA3_Handler            [WEAK]   ; 39: DMA 3 Handler
                EXPORT  DMA4_Handler            [WEAK]   ; 40: DMA 4 Handler
                EXPORT  DMA5_Handler            [WEAK]   ; 41: DMA 5 Handler
                EXPORT  DMA6_Handler            [WEAK]   ; 42: DMA 6 Handler
                EXPORT  DMA7_Handler            [WEAK]   ; 43: DMA 7 Handler
                EXPORT  DMA8_Handler            [WEAK]   ; 44: DMA 8 Handler
                EXPORT  DMA9_Handler            [WEAK]   ; 45: DMA 9 Handler
                EXPORT  DMA10_Handler           [WEAK]   ; 46: DMA 10 Handler
                EXPORT  DMA11_Handler           [WEAK]   ; 47: DMA 11 Handler
                EXPORT  DMA12_Handler           [WEAK]   ; 48: DMA 12 Handler
                EXPORT  DMA13_Handler           [WEAK]   ; 49: DMA 13 Handler
                EXPORT  DMA14_Handler           [WEAK]   ; 50: DMA 14 Handler
                EXPORT  DMA15_Handler           [WEAK]   ; 51: DMA 15 Handler
                EXPORT  DMA16_Handler           [WEAK]   ; 52: DMA 16 Handler
                EXPORT  DMA17_Handler           [WEAK]   ; 53: DMA 17 Handler
                EXPORT  DMA18_Handler           [WEAK]   ; 54: DMA 18 Handler
                EXPORT  DMA19_Handler           [WEAK]   ; 55: DMA 19 Handler
                EXPORT  DMA20_Handler           [WEAK]   ; 56: DMA 20 Handler
                EXPORT  DMA21_Handler           [WEAK]   ; 57: DMA 21 Handler
                EXPORT  DMA22_Handler           [WEAK]   ; 58: DMA 22 Handler
                EXPORT  DMA23_Handler           [WEAK]   ; 59: DMA 23 Handler
                EXPORT  DMA24_Handler           [WEAK]   ; 60: DMA 24 Handler
                EXPORT  DMA25_Handler           [WEAK]   ; 61: DMA 25 Handler
                EXPORT  DMA26_Handler           [WEAK]   ; 62: DMA 26 Handler
                EXPORT  DMA27_Handler           [WEAK]   ; 63: DMA 27 Handler
                EXPORT  DMA28_Handler           [WEAK]   ; 64: DMA 28 Handler
                EXPORT  DMA29_Handler           [WEAK]   ; 65: DMA 29 Handler
                EXPORT  DMA30_Handler           [WEAK]   ; 66: DMA 30 Handler
                EXPORT  DMA31_Handler           [WEAK]   ; 67: DMA 31 Handler
                EXPORT  PORT0_Handler           [WEAK]   ; 68: PORT0 Handler
                EXPORT  PORT1_Handler           [WEAK]   ; 69: PORT1 Handler
                EXPORT  PORT2_Handler           [WEAK]   ; 70: PORT2 Handler
                EXPORT  PORT0_0_Handler         [WEAK]   ; 71: GPIO Port 0 pin 0 Handler
                EXPORT  PORT0_1_Handler         [WEAK]   ; 72: GPIO Port 0 pin 1 Handler
                EXPORT  PORT0_2_Handler         [WEAK]   ; 73: GPIO Port 0 pin 2 Handler
                EXPORT  PORT0_3_Handler         [WEAK]   ; 74: GPIO Port 0 pin 3 Handler
                EXPORT  PORT0_4_Handler         [WEAK]   ; 75: GPIO Port 0 pin 4 Handler
                EXPORT  PORT0_5_Handler         [WEAK]   ; 76: GPIO Port 0 pin 5 Handler
                EXPORT  PORT0_6_Handler         [WEAK]   ; 77: GPIO Port 0 pin 6 Handler
                EXPORT  PORT0_7_Handler         [WEAK]   ; 78: GPIO Port 0 pin 7 Handler
                EXPORT  PORT0_8_Handler         [WEAK]   ; 79: GPIO Port 0 pin 8 Handler
                EXPORT  PORT0_9_Handler         [WEAK]   ; 80: GPIO Port 0 pin 9 Handler
                EXPORT  PORT0_10_Handler        [WEAK]   ; 81: GPIO Port 0 pin 10 Handler
                EXPORT  PORT0_11_Handler        [WEAK]   ; 82: GPIO Port 0 pin 11 Handler
                EXPORT  PORT0_12_Handler        [WEAK]   ; 83: GPIO Port 0 pin 12 Handler
                EXPORT  PORT0_13_Handler        [WEAK]   ; 84: GPIO Port 0 pin 13 Handler
                EXPORT  PORT0_14_Handler        [WEAK]   ; 85: GPIO Port 0 pin 14 Handler
                EXPORT  PORT0_15_Handler        [WEAK]   ; 86: GPIO Port 0 pin 15 Handler
                EXPORT  PORT1_0_Handler         [WEAK]   ; 87: GPIO Port 1 pin 0 Handler
                EXPORT  PORT1_1_Handler         [WEAK]   ; 88: GPIO Port 1 pin 1 Handler
                EXPORT  PORT1_2_Handler         [WEAK]   ; 89: GPIO Port 1 pin 2 Handler
                EXPORT  PORT1_3_Handler         [WEAK]   ; 90: GPIO Port 1 pin 3 Handler
                EXPORT  PORT1_4_Handler         [WEAK]   ; 91: GPIO Port 1 pin 4 Handler
                EXPORT  PORT1_5_Handler         [WEAK]   ; 92: GPIO Port 1 pin 5 Handler
                EXPORT  PORT1_6_Handler         [WEAK]   ; 93: GPIO Port 1 pin 6 Handler
                EXPORT  PORT1_7_Handler         [WEAK]   ; 94: GPIO Port 1 pin 7 Handler
                EXPORT  PORT1_8_Handler         [WEAK]   ; 95: GPIO Port 1 pin 8 Handler
                EXPORT  PORT1_9_Handler         [WEAK]   ; 96: GPIO Port 1 pin 9 Handler
                EXPORT  PORT1_10_Handler        [WEAK]   ; 97: GPIO Port 1 pin 10 Handler
                EXPORT  PORT1_11_Handler        [WEAK]   ; 98: GPIO Port 1 pin 11 Handler
                EXPORT  PORT1_12_Handler        [WEAK]   ; 99: GPIO Port 1 pin 12 Handler
                EXPORT  PORT1_13_Handler        [WEAK]   ; 100: GPIO Port 1 pin 13 Handler
                EXPORT  PORT1_14_Handler        [WEAK]   ; 101: GPIO Port 1 pin 14 Handler
                EXPORT  PORT1_15_Handler        [WEAK]   ; 102: GPIO Port 1 pin 15 Handler
                EXPORT  PORT2_0_Handler         [WEAK]   ; 103: GPIO Port 2 pin 0 Handler
                EXPORT  PORT2_1_Handler         [WEAK]   ; 104: GPIO Port 2 pin 1 Handler
                EXPORT  PORT2_2_Handler         [WEAK]   ; 105: GPIO Port 2 pin 2 Handler
                EXPORT  PORT2_3_Handler         [WEAK]   ; 106: GPIO Port 2 pin 3 Handler
                EXPORT  PORT2_4_Handler         [WEAK]   ; 107: GPIO Port 2 pin 4 Handler
                EXPORT  PORT2_5_Handler         [WEAK]   ; 108: GPIO Port 2 pin 5 Handler
                EXPORT  PORT2_6_Handler         [WEAK]   ; 109: GPIO Port 2 pin 6 Handler
                EXPORT  PORT2_7_Handler         [WEAK]   ; 110: GPIO Port 2 pin 7 Handler
                EXPORT  PORT2_8_Handler         [WEAK]   ; 111: GPIO Port 2 pin 8 Handler
                EXPORT  PORT2_9_Handler         [WEAK]   ; 112: GPIO Port 2 pin 9 Handler
                EXPORT  PORT2_10_Handler        [WEAK]   ; 113: GPIO Port 2 pin 10 Handler
                EXPORT  PORT2_11_Handler        [WEAK]   ; 114: GPIO Port 2 pin 11 Handler
                EXPORT  PORT2_12_Handler        [WEAK]   ; 115: GPIO Port 2 pin 12 Handler
                EXPORT  PORT2_13_Handler        [WEAK]   ; 116: GPIO Port 2 pin 13 Handler
                EXPORT  PORT2_14_Handler        [WEAK]   ; 117: GPIO Port 2 pin 14 Handler
                EXPORT  PORT2_15_Handler        [WEAK]   ; 118: GPIO Port 2 pin 15 Handler


BOR_Handler
UART0_Handler
UART1_Handler
UART2_Handler
SPI0_Handler
SPI1_Handler
SPI2_Handler
SPI3_Handler
SPI4_Handler
I2C0_Handler
I2C1_Handler
I2C2_Handler
I2C3_Handler
I2C4_Handler
TIMER0_Handler
TIMER1_Handler
TIMER2_Handler
TIMER3_Handler
TIMER4_Handler
TIMER5_Handler
TIMER6_Handler
PWM_TIMER_Handler
DUAL_TIMER_Handler
AFECON_Handler
SRC_Handler
DMA_ERROR_Handler
FLASH_Handler
RTC0_Handler
RTC1_Handler
RTC2_Handler
FPU_Handler
SRP0_Handler
SRP1_Handler
DELAY_MON_Handler
AES_Handler
EXT_WAKEUP_Handler
DMA0_Handler
DMA1_Handler
DMA2_Handler
DMA3_Handler
DMA4_Handler
DMA5_Handler
DMA6_Handler
DMA7_Handler
DMA8_Handler
DMA9_Handler
DMA10_Handler
DMA11_Handler
DMA12_Handler
DMA13_Handler
DMA14_Handler
DMA15_Handler
DMA16_Handler
DMA17_Handler
DMA18_Handler
DMA19_Handler
DMA20_Handler
DMA21_Handler
DMA22_Handler
DMA23_Handler
DMA24_Handler
DMA25_Handler
DMA26_Handler
DMA27_Handler
DMA28_Handler
DMA29_Handler
DMA30_Handler
DMA31_Handler
PORT0_Handler
PORT1_Handler
PORT2_Handler
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
PORT1_0_Handler
PORT1_1_Handler
PORT1_2_Handler
PORT1_3_Handler
PORT1_4_Handler
PORT1_5_Handler
PORT1_6_Handler
PORT1_7_Handler
PORT1_8_Handler
PORT1_9_Handler
PORT1_10_Handler
PORT1_11_Handler
PORT1_12_Handler
PORT1_13_Handler
PORT1_14_Handler
PORT1_15_Handler
PORT2_0_Handler
PORT2_1_Handler
PORT2_2_Handler
PORT2_3_Handler
PORT2_4_Handler
PORT2_5_Handler
PORT2_6_Handler
PORT2_7_Handler
PORT2_8_Handler
PORT2_9_Handler
PORT2_10_Handler
PORT2_11_Handler
PORT2_12_Handler
PORT2_13_Handler
PORT2_14_Handler
PORT2_15_Handler

                B       .

                ENDP


                ALIGN

                END
