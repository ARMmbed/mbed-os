/*
 * MPS2 CMSIS Library
 *
 * Copyright (c) 2009-2018 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * This file is derivative of CMSIS V5.00 startup_ARMCM3.s
 */

#include "../memory_zones.h"

__initial_sp    EQU     ZBT_SSRAM23_START + ZBT_SSRAM23_SIZE ; Top of ZBT SSRAM2 and 3, used for data

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
                EXPORT     APB_Slave0_IRQHandler   [WEAK]       ;   3:Reserved for APB Slave
                EXPORT     APB_Slave1_IRQHandler   [WEAK]       ;   4:Reserved for APB Slave
                EXPORT     RTC_IRQHandler          [WEAK]       ;   5:RTC Interrupt
                EXPORT     PORT0_IRQHandler        [WEAK]       ;   6:GPIO Port 0 combined Interrupt
                EXPORT     PORT1_ALL_IRQHandler    [WEAK]       ;   7:GPIO Port 1 combined Interrupt
                EXPORT     TIMER0_IRQHandler       [WEAK]       ;   8:TIMER 0 Interrupt
                EXPORT     TIMER1_IRQHandler       [WEAK]       ;   9:TIMER 1 Interrupt
                EXPORT     DUALTIMER_IRQHandler    [WEAK]       ;   10:Dual Timer Interrupt
                EXPORT     APB_Slave2_IRQHandler   [WEAK]       ;   11:Reserved for APB Slave
                EXPORT     UARTOVF_IRQHandler      [WEAK]       ;   12:UART 0,1,2 Overflow Interrupt
                EXPORT     APB_Slave3_IRQHandler   [WEAK]       ;   13:Reserved for APB Slave
                EXPORT     RESERVED0_IRQHandler    [WEAK]       ;   14:Reserved
                EXPORT     TSC_IRQHandler          [WEAK]       ;   15:Touch Screen Interrupt
                EXPORT     PORT0_0_IRQHandler      [WEAK]       ;   16:GPIO Port 0 pin 0 Handler
                EXPORT     PORT0_1_IRQHandler      [WEAK]       ;   17:GPIO Port 0 pin 1 Handler
                EXPORT     PORT0_2_IRQHandler      [WEAK]       ;   18:GPIO Port 0 pin 2 Handler
                EXPORT     PORT0_3_IRQHandler      [WEAK]       ;   19:GPIO Port 0 pin 3 Handler
                EXPORT     PORT0_4_IRQHandler      [WEAK]       ;   20:GPIO Port 0 pin 4 Handler
                EXPORT     PORT0_5_IRQHandler      [WEAK]       ;   21:GPIO Port 0 pin 5 Handler
                EXPORT     PORT0_6_IRQHandler      [WEAK]       ;   22:GPIO Port 0 pin 6 Handler
                EXPORT     PORT0_7_IRQHandler      [WEAK]       ;   23:GPIO Port 0 pin 7 Handler
                EXPORT     PORT0_8_IRQHandler      [WEAK]       ;   24:GPIO Port 0 pin 8 Handler
                EXPORT     PORT0_9_IRQHandler      [WEAK]       ;   25:GPIO Port 0 pin 9 Handler
                EXPORT     PORT0_10_IRQHandler     [WEAK]       ;   26:GPIO Port 0 pin 10 Handler
                EXPORT     PORT0_11_IRQHandler     [WEAK]       ;   27:GPIO Port 0 pin 11 Handler
                EXPORT     PORT0_12_IRQHandler     [WEAK]       ;   28:GPIO Port 0 pin 12 Handler
                EXPORT     PORT0_13_IRQHandler     [WEAK]       ;   29:GPIO Port 0 pin 13 Handler
                EXPORT     PORT0_14_IRQHandler     [WEAK]       ;   30:GPIO Port 0 pin 14 Handler
                EXPORT     PORT0_15_IRQHandler     [WEAK]       ;   31:GPIO Port 0 pin 15 Handler
                EXPORT     FLASH0_IRQHandler       [WEAK]       ;   32:Reserved for Flash
                EXPORT     FLASH1_IRQHandler       [WEAK]       ;   33:Reserved for Flash
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
                EXPORT     PORT4_ALL_IRQHandler    [WEAK]       ;   54:GPIO Port 4 combined Interrupt
                EXPORT     PORT5_ALL_IRQHandler    [WEAK]       ;   55:GPIO Port 5 combined Interrupt
                EXPORT     UART4_IRQHandler        [WEAK]       ;   56:UART 4 RX and TX Combined Interrupt

UART0_IRQHandler
Spare_IRQHandler
UART1_IRQHandler
APB_Slave0_IRQHandler
APB_Slave1_IRQHandler
RTC_IRQHandler
PORT0_IRQHandler
PORT1_ALL_IRQHandler
TIMER0_IRQHandler
TIMER1_IRQHandler
DUALTIMER_IRQHandler
APB_Slave2_IRQHandler
UARTOVF_IRQHandler
APB_Slave3_IRQHandler
RESERVED0_IRQHandler
TSC_IRQHandler
PORT0_0_IRQHandler
PORT0_1_IRQHandler
PORT0_2_IRQHandler
PORT0_3_IRQHandler
PORT0_4_IRQHandler
PORT0_5_IRQHandler
PORT0_6_IRQHandler
PORT0_7_IRQHandler
PORT0_8_IRQHandler
PORT0_9_IRQHandler
PORT0_10_IRQHandler
PORT0_11_IRQHandler
PORT0_12_IRQHandler
PORT0_13_IRQHandler
PORT0_14_IRQHandler
PORT0_15_IRQHandler
FLASH0_IRQHandler
FLASH1_IRQHandler
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
PORT4_ALL_IRQHandler
PORT5_ALL_IRQHandler
UART4_IRQHandler

                B       .

                ENDP


                ALIGN




                END
