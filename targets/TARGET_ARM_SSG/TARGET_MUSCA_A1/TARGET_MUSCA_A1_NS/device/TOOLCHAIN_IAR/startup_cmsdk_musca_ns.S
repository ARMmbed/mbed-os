; mbed Microcontroller Library
; Copyright (c) 2019 ARM Limited
; SPDX-License-Identifier: Apache-2.0
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
;     http://www.apache.org/licenses/LICENSE-2.0
;
; Unless required by applicable law or agreed to in writing, software
; distributed under the License is distributed on an "AS IS" BASIS,
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; See the License for the specific language governing permissions and
; limitations under the License.
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
        ; Core Interrupts
        DCD     sfe(CSTACK)                    ; Top of Stack
        DCD     Reset_Handler                  ; Reset Handler
        DCD     NMI_Handler                    ; NMI Handler
        DCD     HardFault_Handler              ; Hard Fault Handler
        DCD     MemManage_Handler              ; MPU Fault Handler
        DCD     BusFault_Handler               ; Bus Fault Handler
        DCD     UsageFault_Handler             ; Usage Fault Handler
__vector_table_0x1c
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     SVC_Handler                    ; SVCall Handler
        DCD     DebugMon_Handler               ; Debug Monitor Handler
        DCD     0                              ; Reserved
        DCD     PendSV_Handler                 ; PendSV Handler
        DCD     SysTick_Handler                ; SysTick Handler

        ; SSE-200 Interrupts
        DCD    NS_WATCHDOG_RESET_IRQHandler    ;  0: Non-Secure Watchdog Reset Request Interrupt
        DCD    NS_WATCHDOG_IRQHandler          ;  1: Non-Secure Watchdog Interrupt
        DCD    S32K_TIMER_IRQHandler           ;  2: S32K Timer Interrupt
        DCD    TIMER0_IRQHandler               ;  3: CMSDK Timer 0 Interrupt
        DCD    TIMER1_IRQHandler               ;  4: CMSDK Timer 1 Interrupt
        DCD    DUALTIMER_IRQHandler            ;  5: CMSDK Dual Timer Interrupt
        DCD    MHU0_IRQHandler                 ;  6: Message Handling Unit 0 Interrupt
        DCD    MHU1_IRQHandler                 ;  7: Message Handling Unit 1 Interrupt
        DCD    CRYPTOCELL_IRQHandler           ;  8: CryptoCell-312 Interrupt
        DCD     0                              ;  9: Reserved
        DCD     0                              ; 10: Reserved
        DCD     0                              ; 11: Reserved
        DCD     0                              ; 12: Reserved
        DCD    I_CACHE_INV_ERR_IRQHandler      ; 13: Intsruction Cache Invalidation Interrupt
        DCD    0                               ; 14: Reserved
        DCD    SYS_PPU_IRQHandler              ; 15: System PPU Interrupt
        DCD    CPU0_PPU_IRQHandler             ; 16: CPU0 PPU Interrupt
        DCD    CPU1_PPU_IRQHandler             ; 17: CPU1 PPU Interrupt
        DCD    CPU0_DGB_PPU_IRQHandler         ; 18: CPU0 Debug PPU Interrupt
        DCD    CPU1_DGB_PPU_IRQHandler         ; 19: CPU1 Debug PPU Interrupt
        DCD    CRYPTOCELL_PPU_IRQHandler       ; 20: CryptoCell PPU Interrupt
        DCD    0                               ; 21: Reserved
        DCD    RAM0_PPU_IRQHandler             ; 22: RAM 0 PPU Interrupt
        DCD    RAM1_PPU_IRQHandler             ; 23: RAM 1 PPU Interrupt
        DCD    RAM2_PPU_IRQHandler             ; 24: RAM 2 PPU Interrupt
        DCD    RAM3_PPU_IRQHandler             ; 25: RAM 3 PPU Interrupt
        DCD    DEBUG_PPU_IRQHandler            ; 26: Debug PPU Interrupt
        DCD    0                               ; 27: Reserved
        DCD    CPU0_CTI_IRQHandler             ; 28: CPU0 CTI Interrupt
        DCD    CPU1_CTI_IRQHandler             ; 29: CPU1 CTI Interrupt
        DCD    0                               ; 30: Reserved
        DCD    0                               ; 31: Reserved
        ;Expansion Interrupts
        DCD    0                               ; 32: Reserved
        DCD    GpTimer_IRQHandler              ; 33: General Purpose Timer
        DCD    I2C0_IRQHandler                 ; 34: I2C0
        DCD    I2C1_IRQHandler                 ; 35: I2C1
        DCD    I2S_IRQHandler                  ; 36: I2S
        DCD    SPI_IRQHandler                  ; 37: SPI
        DCD    QSPI_IRQHandler                 ; 38: QSPI
        DCD    UARTRX0_Handler                 ; 39: UART0 receive FIFO interrupt
        DCD    UARTTX0_Handler                 ; 40: UART0 transmit FIFO interrupt
        DCD    UART0_RxTimeout_IRQHandler      ; 41: UART0 receive timeout interrupt
        DCD    UART0_ModemStatus_IRQHandler    ; 42: UART0 modem status interrupt
        DCD    UART0_Error_IRQHandler          ; 43: UART0 error interrupt
        DCD    UART0_IRQHandler                ; 44: UART0 interrupt
        DCD    UARTRX1_Handler                 ; 45: UART0 receive FIFO interrupt
        DCD    UARTTX1_Handler                 ; 46: UART0 transmit FIFO interrupt
        DCD    UART1_RxTimeout_IRQHandler      ; 47: UART0 receive timeout interrupt
        DCD    UART1_ModemStatus_IRQHandler    ; 48: UART0 modem status interrupt
        DCD    UART1_Error_IRQHandler          ; 49: UART0 error interrupt
        DCD    UART1_IRQHandler                ; 50: UART0 interrupt
        DCD    GPIO_0_IRQHandler               ; 51: GPIO 0 interrupt
        DCD    GPIO_1_IRQHandler               ; 52: GPIO 1 interrupt
        DCD    GPIO_2_IRQHandler               ; 53: GPIO 2 interrupt
        DCD    GPIO_3_IRQHandler               ; 54: GPIO 3 interrupt
        DCD    GPIO_4_IRQHandler               ; 55: GPIO 4 interrupt
        DCD    GPIO_5_IRQHandler               ; 56: GPIO 5 interrupt
        DCD    GPIO_6_IRQHandler               ; 57: GPIO 6 interrupt
        DCD    GPIO_7_IRQHandler               ; 58: GPIO 7 interrupt
        DCD    GPIO_8_IRQHandler               ; 59: GPIO 8 interrupt
        DCD    GPIO_9_IRQHandler               ; 60: GPIO 9 interrupt
        DCD    GPIO_10_IRQHandler              ; 61: GPIO 10 interrupt
        DCD    GPIO_11_IRQHandler              ; 62: GPIO 11 interrupt
        DCD    GPIO_12_IRQHandler              ; 63: GPIO 12 interrupt
        DCD    GPIO_13_IRQHandler              ; 64: GPIO 13 interrupt
        DCD    GPIO_14_IRQHandler              ; 65: GPIO 14 interrupt
        DCD    GPIO_15_IRQHandler              ; 66: GPIO 15 interrupt
        DCD    Combined_IRQHandler             ; 67: Combined interrupt
        DCD    PVT_IRQHandler                  ; 68: PVT sensor interrupt
        DCD    0                               ; 69: Reserved
        DCD    PWM_0_IRQHandler                ; 70: PWM0 interrupt
        DCD    RTC_IRQHandler                  ; 71: RTC interrupt
        DCD    GpTimer1_IRQHandler             ; 72: General Purpose Timer1
        DCD    GpTimer0_IRQHandler             ; 73: General Purpose Timer0
        DCD    PWM_1_IRQHandler                ; 74: PWM1 interrupt
        DCD    PWM_2_IRQHandler                ; 75: PWM2 interrupt
        DCD    IOMUX_IRQHandler                ; 76: IOMUX interrupt
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
        CPSID   I               ; Mask interrupts
        LDR     R0, =sfb(CSTACK)
        MSR     MSPLIM, R0
        LDR     R0, =SystemInit
        BLX     R0
        CPSIE   I               ; Unmask interrupts
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B .

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B .

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
MemManage_Handler
        B .

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BusFault_Handler
        B .

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UsageFault_Handler
        B .

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B .

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DebugMon_Handler
        B .

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B .

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B .

        PUBWEAK NS_WATCHDOG_RESET_IRQHandler
        PUBWEAK NS_WATCHDOG_RESET_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
NS_WATCHDOG_RESET_IRQHandler
        LDR     R0, =NS_WATCHDOG_RESET_DriverIRQHandler
        BX      R0

        PUBWEAK NS_WATCHDOG_IRQHandler
        PUBWEAK NS_WATCHDOG_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
NS_WATCHDOG_IRQHandler
        LDR     R0, =NS_WATCHDOG_DriverIRQHandler
        BX      R0

        PUBWEAK S32K_TIMER_IRQHandler
        PUBWEAK S32K_TIMER_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
S32K_TIMER_IRQHandler
        LDR     R0, =S32K_TIMER_DriverIRQHandler
        BX      R0

        PUBWEAK TIMER0_IRQHandler
        PUBWEAK TIMER0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
TIMER0_IRQHandler
        LDR     R0, =TIMER0_DriverIRQHandler
        BX      R0

        PUBWEAK TIMER1_IRQHandler
        PUBWEAK TIMER1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
TIMER1_IRQHandler
        LDR     R0, =TIMER1_DriverIRQHandler
        BX      R0

        PUBWEAK DUALTIMER_IRQHandler
        PUBWEAK DUALTIMER_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DUALTIMER_IRQHandler
        LDR     R0, =DUALTIMER_DriverIRQHandler
        BX      R0

        PUBWEAK MHU0_IRQHandler
        PUBWEAK MHU0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
MHU0_IRQHandler
        LDR     R0, =MHU0_DriverIRQHandler
        BX      R0

        PUBWEAK MHU1_IRQHandler
        PUBWEAK MHU1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
MHU1_IRQHandler
        LDR     R0, =MHU1_DriverIRQHandler
        BX      R0

        PUBWEAK CRYPTOCELL_IRQHandler
        PUBWEAK CRYPTOCELL_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CRYPTOCELL_IRQHandler
        LDR     R0, =CRYPTOCELL_DriverIRQHandler
        BX      R0

        PUBWEAK I_CACHE_INV_ERR_IRQHandler
        PUBWEAK I_CACHE_INV_ERR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I_CACHE_INV_ERR_IRQHandler
        LDR     R0, =I_CACHE_INV_ERR_DriverIRQHandler
        BX      R0

        PUBWEAK SYS_PPU_IRQHandler
        PUBWEAK SYS_PPU_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SYS_PPU_IRQHandler
        LDR     R0, =SYS_PPU_DriverIRQHandler
        BX      R0

        PUBWEAK CPU0_PPU_IRQHandler
        PUBWEAK CPU0_PPU_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CPU0_PPU_IRQHandler
        LDR     R0, =CPU0_PPU_DriverIRQHandler
        BX      R0

        PUBWEAK CPU1_PPU_IRQHandler
        PUBWEAK CPU1_PPU_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CPU1_PPU_IRQHandler
        LDR     R0, =CPU1_PPU_DriverIRQHandler
        BX      R0

        PUBWEAK CPU0_DGB_PPU_IRQHandler
        PUBWEAK CPU0_DGB_PPU_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CPU0_DGB_PPU_IRQHandler
        LDR     R0, =CPU0_DGB_PPU_DriverIRQHandler
        BX      R0

        PUBWEAK CPU1_DGB_PPU_IRQHandler
        PUBWEAK CPU1_DGB_PPU_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CPU1_DGB_PPU_IRQHandler
        LDR     R0, =CPU1_DGB_PPU_DriverIRQHandler
        BX      R0

        PUBWEAK CRYPTOCELL_PPU_IRQHandler
        PUBWEAK CRYPTOCELL_PPU_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CRYPTOCELL_PPU_IRQHandler
        LDR     R0, =CRYPTOCELL_PPU_DriverIRQHandler
        BX      R0

        PUBWEAK RAM0_PPU_IRQHandler
        PUBWEAK RAM0_PPU_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
RAM0_PPU_IRQHandler
        LDR     R0, =RAM0_PPU_DriverIRQHandler
        BX      R0

        PUBWEAK RAM1_PPU_IRQHandler
        PUBWEAK RAM1_PPU_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
RAM1_PPU_IRQHandler
        LDR     R0, =RAM1_PPU_DriverIRQHandler
        BX      R0

        PUBWEAK RAM2_PPU_IRQHandler
        PUBWEAK RAM2_PPU_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
RAM2_PPU_IRQHandler
        LDR     R0, =RAM2_PPU_DriverIRQHandler
        BX      R0

        PUBWEAK RAM3_PPU_IRQHandler
        PUBWEAK RAM3_PPU_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
RAM3_PPU_IRQHandler
        LDR     R0, =RAM3_PPU_DriverIRQHandler
        BX      R0

        PUBWEAK DEBUG_PPU_IRQHandler
        PUBWEAK DEBUG_PPU_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DEBUG_PPU_IRQHandler
        LDR     R0, =DEBUG_PPU_DriverIRQHandler
        BX      R0

        PUBWEAK CPU0_CTI_IRQHandler
        PUBWEAK CPU0_CTI_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CPU0_CTI_IRQHandler
        LDR     R0, =CPU0_CTI_DriverIRQHandler
        BX      R0

        PUBWEAK CPU1_CTI_IRQHandler
        PUBWEAK CPU1_CTI_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CPU1_CTI_IRQHandler
        LDR     R0, =CPU1_CTI_DriverIRQHandler
        BX      R0

        PUBWEAK GpTimer_IRQHandler
        PUBWEAK GpTimer_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GpTimer_IRQHandler
        LDR     R0, =GpTimer_DriverIRQHandler
        BX      R0

        PUBWEAK I2C0_IRQHandler
        PUBWEAK I2C0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2C0_IRQHandler
        LDR     R0, =I2C0_DriverIRQHandler
        BX      R0

        PUBWEAK I2C1_IRQHandler
        PUBWEAK I2C1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2C1_IRQHandler
        LDR     R0, =I2C1_DriverIRQHandler
        BX      R0

        PUBWEAK I2S_IRQHandler
        PUBWEAK I2S_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2S_IRQHandler
        LDR     R0, =I2S_DriverIRQHandler
        BX      R0

        PUBWEAK SPI_IRQHandler
        PUBWEAK SPI_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SPI_IRQHandler
        LDR     R0, =SPI_DriverIRQHandler
        BX      R0

        PUBWEAK QSPI_IRQHandler
        PUBWEAK QSPI_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
QSPI_IRQHandler
        LDR     R0, =QSPI_DriverIRQHandler
        BX      R0

        PUBWEAK UARTRX0_Handler
        PUBWEAK UARTRX0_DriverHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UARTRX0_Handler
        LDR     R0, =UARTRX0_DriverHandler
        BX      R0

        PUBWEAK UARTTX0_Handler
        PUBWEAK UARTTX0_DriverHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UARTTX0_Handler
        LDR     R0, =UARTTX0_DriverHandler
        BX      R0

        PUBWEAK UART0_RxTimeout_IRQHandler
        PUBWEAK UART0_RxTimeout_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART0_RxTimeout_IRQHandler
        LDR     R0, =UART0_RxTimeout_DriverIRQHandler
        BX      R0

        PUBWEAK UART0_ModemStatus_IRQHandler
        PUBWEAK UART0_ModemStatus_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART0_ModemStatus_IRQHandler
        LDR     R0, =UART0_ModemStatus_DriverIRQHandler
        BX      R0

        PUBWEAK UART0_Error_IRQHandler
        PUBWEAK UART0_Error_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART0_Error_IRQHandler
        LDR     R0, =UART0_Error_DriverIRQHandler
        BX      R0

        PUBWEAK UART0_IRQHandler
        PUBWEAK UART0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART0_IRQHandler
        LDR     R0, =UART0_DriverIRQHandler
        BX      R0

        PUBWEAK UARTRX1_Handler
        PUBWEAK UARTRX1_DriverHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UARTRX1_Handler
        LDR     R0, =UARTRX1_DriverHandler
        BX      R0

        PUBWEAK UARTTX1_Handler
        PUBWEAK UARTTX1_DriverHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UARTTX1_Handler
        LDR     R0, =UARTTX1_DriverHandler
        BX      R0

        PUBWEAK UART1_RxTimeout_IRQHandler
        PUBWEAK UART1_RxTimeout_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART1_RxTimeout_IRQHandler
        LDR     R0, =UART1_RxTimeout_DriverIRQHandler
        BX      R0

        PUBWEAK UART1_ModemStatus_IRQHandler
        PUBWEAK UART1_ModemStatus_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART1_ModemStatus_IRQHandler
        LDR     R0, =UART1_ModemStatus_DriverIRQHandler
        BX      R0

        PUBWEAK UART1_Error_IRQHandler
        PUBWEAK UART1_Error_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART1_Error_IRQHandler
        LDR     R0, =UART1_Error_DriverIRQHandler
        BX      R0

        PUBWEAK UART1_IRQHandler
        PUBWEAK UART1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART1_IRQHandler
        LDR     R0, =UART1_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO_0_IRQHandler
        PUBWEAK GPIO_0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO_0_IRQHandler
        LDR     R0, =GPIO_0_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO_1_IRQHandler
        PUBWEAK GPIO_1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO_1_IRQHandler
        LDR     R0, =GPIO_1_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO_2_IRQHandler
        PUBWEAK GPIO_2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO_2_IRQHandler
        LDR     R0, =GPIO_2_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO_3_IRQHandler
        PUBWEAK GPIO_3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO_3_IRQHandler
        LDR     R0, =GPIO_3_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO_4_IRQHandler
        PUBWEAK GPIO_4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO_4_IRQHandler
        LDR     R0, =GPIO_4_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO_5_IRQHandler
        PUBWEAK GPIO_5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO_5_IRQHandler
        LDR     R0, =GPIO_5_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO_6_IRQHandler
        PUBWEAK GPIO_6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO_6_IRQHandler
        LDR     R0, =GPIO_6_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO_7_IRQHandler
        PUBWEAK GPIO_7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO_7_IRQHandler
        LDR     R0, =GPIO_7_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO_8_IRQHandler
        PUBWEAK GPIO_8_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO_8_IRQHandler
        LDR     R0, =GPIO_8_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO_9_IRQHandler
        PUBWEAK GPIO_9_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO_9_IRQHandler
        LDR     R0, =GPIO_9_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO_10_IRQHandler
        PUBWEAK GPIO_10_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO_10_IRQHandler
        LDR     R0, =GPIO_10_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO_11_IRQHandler
        PUBWEAK GPIO_11_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO_11_IRQHandler
        LDR     R0, =GPIO_11_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO_12_IRQHandler
        PUBWEAK GPIO_12_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO_12_IRQHandler
        LDR     R0, =GPIO_12_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO_13_IRQHandler
        PUBWEAK GPIO_13_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO_13_IRQHandler
        LDR     R0, =GPIO_13_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO_14_IRQHandler
        PUBWEAK GPIO_14_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO_14_IRQHandler
        LDR     R0, =GPIO_14_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO_15_IRQHandler
        PUBWEAK GPIO_15_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO_15_IRQHandler
        LDR     R0, =GPIO_15_DriverIRQHandler
        BX      R0

        PUBWEAK Combined_IRQHandler
        PUBWEAK Combined_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Combined_IRQHandler
        LDR     R0, =Combined_DriverIRQHandler
        BX      R0

        PUBWEAK PVT_IRQHandler
        PUBWEAK PVT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PVT_IRQHandler
        LDR     R0, =PVT_DriverIRQHandler
        BX      R0

        PUBWEAK PWM_0_IRQHandler
        PUBWEAK PWM_0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PWM_0_IRQHandler
        LDR     R0, =PWM_0_DriverIRQHandler
        BX      R0

        PUBWEAK RTC_IRQHandler
        PUBWEAK RTC_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
RTC_IRQHandler
        LDR     R0, =RTC_DriverIRQHandler
        BX      R0

        PUBWEAK GpTimer1_IRQHandler
        PUBWEAK GpTimer1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GpTimer1_IRQHandler
        LDR     R0, =GpTimer1_DriverIRQHandler
        BX      R0

        PUBWEAK GpTimer0_IRQHandler
        PUBWEAK GpTimer0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GpTimer0_IRQHandler
        LDR     R0, =GpTimer0_DriverIRQHandler
        BX      R0

        PUBWEAK PWM_1_IRQHandler
        PUBWEAK PWM_1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PWM_1_IRQHandler
        LDR     R0, =PWM_1_DriverIRQHandler
        BX      R0

        PUBWEAK PWM_2_IRQHandler
        PUBWEAK PWM_2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PWM_2_IRQHandler
        LDR     R0, =PWM_2_DriverIRQHandler
        BX      R0

        PUBWEAK IOMUX_IRQHandler
        PUBWEAK IOMUX_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
IOMUX_IRQHandler
        LDR     R0, =IOMUX_DriverIRQHandler
        BX      R0

NS_WATCHDOG_RESET_DriverIRQHandler
NS_WATCHDOG_DriverIRQHandler
S32K_TIMER_DriverIRQHandler
TIMER0_DriverIRQHandler
TIMER1_DriverIRQHandler
DUALTIMER_DriverIRQHandler
MHU0_DriverIRQHandler
MHU1_DriverIRQHandler
CRYPTOCELL_DriverIRQHandler
I_CACHE_INV_ERR_DriverIRQHandler
SYS_PPU_DriverIRQHandler
CPU0_PPU_DriverIRQHandler
CPU1_PPU_DriverIRQHandler
CPU0_DGB_PPU_DriverIRQHandler
CPU1_DGB_PPU_DriverIRQHandler
CRYPTOCELL_PPU_DriverIRQHandler
RAM0_PPU_DriverIRQHandler
RAM1_PPU_DriverIRQHandler
RAM2_PPU_DriverIRQHandler
RAM3_PPU_DriverIRQHandler
DEBUG_PPU_DriverIRQHandler
CPU0_CTI_DriverIRQHandler
CPU1_CTI_DriverIRQHandler
GpTimer_DriverIRQHandler
I2C0_DriverIRQHandler
I2C1_DriverIRQHandler
I2S_DriverIRQHandler
SPI_DriverIRQHandler
QSPI_DriverIRQHandler
UARTRX0_DriverHandler
UARTTX0_DriverHandler
UART0_RxTimeout_DriverIRQHandler
UART0_ModemStatus_DriverIRQHandler
UART0_Error_DriverIRQHandler
UART0_DriverIRQHandler
UARTRX1_DriverHandler
UARTTX1_DriverHandler
UART1_RxTimeout_DriverIRQHandler
UART1_ModemStatus_DriverIRQHandler
UART1_Error_DriverIRQHandler
UART1_DriverIRQHandler
GPIO_0_DriverIRQHandler
GPIO_1_DriverIRQHandler
GPIO_2_DriverIRQHandler
GPIO_3_DriverIRQHandler
GPIO_4_DriverIRQHandler
GPIO_5_DriverIRQHandler
GPIO_6_DriverIRQHandler
GPIO_7_DriverIRQHandler
GPIO_8_DriverIRQHandler
GPIO_9_DriverIRQHandler
GPIO_10_DriverIRQHandler
GPIO_11_DriverIRQHandler
GPIO_12_DriverIRQHandler
GPIO_13_DriverIRQHandler
GPIO_14_DriverIRQHandler
GPIO_15_DriverIRQHandler
Combined_DriverIRQHandler
PVT_DriverIRQHandler
PWM_0_DriverIRQHandler
RTC_DriverIRQHandler
GpTimer1_DriverIRQHandler
GpTimer0_DriverIRQHandler
PWM_1_DriverIRQHandler
PWM_2_DriverIRQHandler
IOMUX_DriverIRQHandler
DefaultISR
        B .

        END
