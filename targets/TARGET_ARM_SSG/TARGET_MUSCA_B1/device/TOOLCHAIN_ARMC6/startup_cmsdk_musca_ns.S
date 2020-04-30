;/*
; * Copyright (c) 2019-2020 Arm Limited
; *
; * SPDX-License-Identifier: Apache-2.0
; *
; * Licensed under the Apache License, Version 2.0 (the "License");
; * you may not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; *     http://www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an "AS IS" BASIS,
; * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; */
;
; This file is derivative of CMSIS V5.01 startup_ARMv8MML.s
; Git SHA: 8a1d9d6ee18b143ae5befefa14d89fb5b3f99c75

;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/

; Vector Table Mapped to Address 0 at Reset

                AREA    VECTOR, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

                IMPORT  |Image$$ARM_LIB_STACK$$ZI$$Limit|

__Vectors       ;Core Interrupts
                DCD     |Image$$ARM_LIB_STACK$$ZI$$Limit|; Top of Stack
                DCD     Reset_Handler                    ; Reset Handler
                DCD     NMI_Handler                      ; NMI Handler
                DCD     HardFault_Handler                ; Hard Fault Handler
                DCD     MemManage_Handler                ; MPU Fault Handler
                DCD     BusFault_Handler                 ; Bus Fault Handler
                DCD     UsageFault_Handler               ; Usage Fault Handler
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     SVC_Handler                      ; SVCall Handler
                DCD     DebugMon_Handler                 ; Debug Monitor Handler
                DCD     0                                ; Reserved
                DCD     PendSV_Handler                   ; PendSV Handler
                DCD     SysTick_Handler                  ; SysTick Handler
                ;SSE-200 Interrupts
                DCD    NS_WATCHDOG_RESET_IRQHandler      ;  0: Non-Secure Watchdog Reset Request Interrupt
                DCD    NS_WATCHDOG_IRQHandler            ;  1: Non-Secure Watchdog Interrupt
                DCD    S32K_TIMER_IRQHandler             ;  2: S32K Timer Interrupt
                DCD    TIMER0_IRQHandler                 ;  3: CMSDK Timer 0 Interrupt
                DCD    TIMER1_IRQHandler                 ;  4: CMSDK Timer 1 Interrupt
                DCD    DUALTIMER_IRQHandler              ;  5: CMSDK Dual Timer Interrupt
                DCD    MHU0_IRQHandler                   ;  6: Message Handling Unit 0 Interrupt
                DCD    MHU1_IRQHandler                   ;  7: Message Handling Unit 1 Interrupt
                DCD    CRYPTOCELL_IRQHandler             ;  8: CryptoCell-312 Interrupt
                DCD    0                                 ;  9: Reserved
                DCD    0                                 ; 10: Reserved
                DCD    0                                 ; 11: Reserved
                DCD    0                                 ; 12: Reserved
                DCD    I_CACHE_INV_ERR_IRQHandler        ; 13: Intsruction Cache Invalidation Interrupt
                DCD    0                                 ; 14: Reserved
                DCD    SYS_PPU_IRQHandler                ; 15: System PPU Interrupt
                DCD    CPU0_PPU_IRQHandler               ; 16: CPU0 PPU Interrupt
                DCD    CPU1_PPU_IRQHandler               ; 17: CPU1 PPU Interrupt
                DCD    CPU0_DGB_PPU_IRQHandler           ; 18: CPU0 Debug PPU Interrupt
                DCD    CPU1_DGB_PPU_IRQHandler           ; 19: CPU1 Debug PPU Interrupt
                DCD    CRYPTOCELL_PPU_IRQHandler         ; 20: CryptoCell PPU Interrupt
                DCD    0                                 ; 21: Reserved
                DCD    RAM0_PPU_IRQHandler               ; 22: RAM 0 PPU Interrupt
                DCD    RAM1_PPU_IRQHandler               ; 23: RAM 1 PPU Interrupt
                DCD    RAM2_PPU_IRQHandler               ; 24: RAM 2 PPU Interrupt
                DCD    RAM3_PPU_IRQHandler               ; 25: RAM 3 PPU Interrupt
                DCD    DEBUG_PPU_IRQHandler              ; 26: Debug PPU Interrupt
                DCD    0                                 ; 27: Reserved
                DCD    CPUn_CTI_0_IRQHandler             ; 28: CPUn CTI Interrupt 0
                DCD    CPUn_CTI_1_IRQHandler             ; 29: CPUn CTI Interrupt 1
                DCD    0                                 ; 30: Reserved
                DCD    0                                 ; 31: Reserved
                ;Expansion Interrupts
                DCD    0                                 ; 32: Reserved
                DCD    GpTimer_IRQHandler                ; 33: General Purpose Timer
                DCD    I2C0_IRQHandler                   ; 34: I2C0
                DCD    I2C1_IRQHandler                   ; 35: I2C1
                DCD    I2S_IRQHandler                    ; 36: I2S
                DCD    SPI_IRQHandler                    ; 37: SPI
                DCD    QSPI_IRQHandler                   ; 38: QSPI
                DCD    UARTRX0_Handler                   ; 39: UART0 receive FIFO interrupt
                DCD    UARTTX0_Handler                   ; 40: UART0 transmit FIFO interrupt
                DCD    UART0_RxTimeout_IRQHandler        ; 41: UART0 receive timeout interrupt
                DCD    UART0_ModemStatus_IRQHandler      ; 42: UART0 modem status interrupt
                DCD    UART0_Error_IRQHandler            ; 43: UART0 error interrupt
                DCD    UART0_IRQHandler                  ; 44: UART0 interrupt
                DCD    UARTRX1_Handler                   ; 45: UART0 receive FIFO interrupt
                DCD    UARTTX1_Handler                   ; 46: UART0 transmit FIFO interrupt
                DCD    UART1_RxTimeout_IRQHandler        ; 47: UART0 receive timeout interrupt
                DCD    UART1_ModemStatus_IRQHandler      ; 48: UART0 modem status interrupt
                DCD    UART1_Error_IRQHandler            ; 49: UART0 error interrupt
                DCD    UART1_IRQHandler                  ; 50: UART0 interrupt
                DCD    GPIO_0_IRQHandler                 ; 51: GPIO 0 interrupt
                DCD    GPIO_1_IRQHandler                 ; 52: GPIO 1 interrupt
                DCD    GPIO_2_IRQHandler                 ; 53: GPIO 2 interrupt
                DCD    GPIO_3_IRQHandler                 ; 54: GPIO 3 interrupt
                DCD    GPIO_4_IRQHandler                 ; 55: GPIO 4 interrupt
                DCD    GPIO_5_IRQHandler                 ; 56: GPIO 5 interrupt
                DCD    GPIO_6_IRQHandler                 ; 57: GPIO 6 interrupt
                DCD    GPIO_7_IRQHandler                 ; 58: GPIO 7 interrupt
                DCD    GPIO_8_IRQHandler                 ; 59: GPIO 8 interrupt
                DCD    GPIO_9_IRQHandler                 ; 60: GPIO 9 interrupt
                DCD    GPIO_10_IRQHandler                ; 61: GPIO 10 interrupt
                DCD    GPIO_11_IRQHandler                ; 62: GPIO 11 interrupt
                DCD    GPIO_12_IRQHandler                ; 63: GPIO 12 interrupt
                DCD    GPIO_13_IRQHandler                ; 64: GPIO 13 interrupt
                DCD    GPIO_14_IRQHandler                ; 65: GPIO 14 interrupt
                DCD    GPIO_15_IRQHandler                ; 66: GPIO 15 interrupt
                DCD    GPIO_Combined_IRQHandler          ; 67: GPIO Combined interrupt
                DCD    PVT_IRQHandler                    ; 68: PVT sensor interrupt
                DCD    0                                 ; 69: Reserved
                DCD    PWM_0_IRQHandler                  ; 70: PWM0 interrupt
                DCD    RTC_IRQHandler                    ; 71: RTC interrupt
                DCD    GpTimer1_IRQHandler               ; 72: General Purpose Timer1
                DCD    GpTimer0_IRQHandler               ; 73: General Purpose Timer0
                DCD    PWM_1_IRQHandler                  ; 74: PWM1 interrupt
                DCD    PWM_2_IRQHandler                  ; 75: PWM2 interrupt
                DCD    GPIO_Combined_NS_IRQHandler       ; 76: GPIO Combined Non-secure interrupt
                DCD    SDIO_IRQHandler                   ; 77: SDIO interrupt handler
                DCD    0                                 ; 78: Reserved
                DCD    0                                 ; 79: Reserved
                DCD    0                                 ; 80: Reserved
                DCD    0                                 ; 81: Reserved
                DCD    0                                 ; 82: Reserved
                DCD    0                                 ; 83: Reserved
                DCD    CryptoSS_Reset_Status_IRQHandler  ; 84: Crypto SS reset status
                DCD    HostMHUS0_Int_Acc_NR2R_IRQHandler ; 85: MHU0 Sender IRQ not-ready to ready
                DCD    HostMHUS0_Int_Acc_R2NR_IRQHandler ; 86: MHU0 Sender IRQ ready to not ready
                DCD    HostMHUR0_IRQ_Reg0_IRQHandler     ; 87: MHU0 Receiver IRQ Register 0
                DCD    HostMHUR0_IRQ_Reg1_IRQHandler     ; 88: MHU0 Receiver IRQ Register 1
                DCD    HostMHUR0_IRQComb_IRQHandler      ; 89: MHU0 Receiver IRQ combined
                DCD    HostMHUS1_Int_Acc_NR2R_IRQHandler ; 90: MHU1 Sender IRQ not-ready to ready
                DCD    HostMHUS1_Int_Acc_R2NR_IRQHandler ; 91: MHU1 Sender IRQ ready to not ready
                DCD    HostMHUR1_IRQ_Reg0_IRQHandler     ; 92: MHU1 Receiver IRQ Register 0
                DCD    HostMHUR1_IRQ_Reg1_IRQHandler     ; 93: MHU1 Receiver IRQ Register 1
                DCD    HostMHUR1_IRQComb_IRQHandler      ; 94: MHU1 Receiver IRQ combined
                DCD    0                                 ; 95: Reserved
                DCD    0                                 ; 96: Reserved
                DCD    0                                 ; 97:127 Reserved


__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

; Reset Handler
                AREA    RESET, CODE, READONLY
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP
End_Of_Main
                B       .

                ALIGN 4

; Dummy Exception Handlers (infinite loops which can be modified)
                MACRO
                Default_Handler $handler_name
$handler_name   PROC
                EXPORT  $handler_name             [WEAK]
                B       .
                ENDP
                MEND

                Default_Handler NMI_Handler
                Default_Handler HardFault_Handler
                Default_Handler MemManage_Handler
                Default_Handler BusFault_Handler
                Default_Handler UsageFault_Handler
                Default_Handler SVC_Handler
                Default_Handler DebugMon_Handler
                Default_Handler PendSV_Handler
                Default_Handler SysTick_Handler

                Default_Handler NS_WATCHDOG_RESET_IRQHandler
                Default_Handler NS_WATCHDOG_IRQHandler
                Default_Handler S32K_TIMER_IRQHandler
                Default_Handler TIMER0_IRQHandler
                Default_Handler TIMER1_IRQHandler
                Default_Handler DUALTIMER_IRQHandler
                Default_Handler MHU0_IRQHandler
                Default_Handler MHU1_IRQHandler
                Default_Handler CRYPTOCELL_IRQHandler
                Default_Handler I_CACHE_INV_ERR_IRQHandler
                Default_Handler SYS_PPU_IRQHandler
                Default_Handler CPU0_PPU_IRQHandler
                Default_Handler CPU1_PPU_IRQHandler
                Default_Handler CPU0_DGB_PPU_IRQHandler
                Default_Handler CPU1_DGB_PPU_IRQHandler
                Default_Handler CRYPTOCELL_PPU_IRQHandler
                Default_Handler RAM0_PPU_IRQHandler
                Default_Handler RAM1_PPU_IRQHandler
                Default_Handler RAM2_PPU_IRQHandler
                Default_Handler RAM3_PPU_IRQHandler
                Default_Handler DEBUG_PPU_IRQHandler
                Default_Handler CPUn_CTI_0_IRQHandler
                Default_Handler CPUn_CTI_1_IRQHandler

                Default_Handler GpTimer_IRQHandler
                Default_Handler I2C0_IRQHandler
                Default_Handler I2C1_IRQHandler
                Default_Handler I2S_IRQHandler
                Default_Handler SPI_IRQHandler
                Default_Handler QSPI_IRQHandler
                Default_Handler UARTRX0_Handler
                Default_Handler UARTTX0_Handler
                Default_Handler UART0_RxTimeout_IRQHandler
                Default_Handler UART0_ModemStatus_IRQHandler
                Default_Handler UART0_Error_IRQHandler
                Default_Handler UART0_IRQHandler
                Default_Handler UARTRX1_Handler
                Default_Handler UARTTX1_Handler
                Default_Handler UART1_RxTimeout_IRQHandler
                Default_Handler UART1_ModemStatus_IRQHandler
                Default_Handler UART1_Error_IRQHandler
                Default_Handler UART1_IRQHandler
                Default_Handler GPIO_0_IRQHandler
                Default_Handler GPIO_1_IRQHandler
                Default_Handler GPIO_2_IRQHandler
                Default_Handler GPIO_3_IRQHandler
                Default_Handler GPIO_4_IRQHandler
                Default_Handler GPIO_5_IRQHandler
                Default_Handler GPIO_6_IRQHandler
                Default_Handler GPIO_7_IRQHandler
                Default_Handler GPIO_8_IRQHandler
                Default_Handler GPIO_9_IRQHandler
                Default_Handler GPIO_10_IRQHandler
                Default_Handler GPIO_11_IRQHandler
                Default_Handler GPIO_12_IRQHandler
                Default_Handler GPIO_13_IRQHandler
                Default_Handler GPIO_14_IRQHandler
                Default_Handler GPIO_15_IRQHandler
                Default_Handler GPIO_Combined_IRQHandler
                Default_Handler PVT_IRQHandler
                Default_Handler PWM_0_IRQHandler
                Default_Handler RTC_IRQHandler
                Default_Handler GpTimer1_IRQHandler
                Default_Handler GpTimer0_IRQHandler
                Default_Handler PWM_1_IRQHandler
                Default_Handler PWM_2_IRQHandler
                Default_Handler GPIO_Combined_NS_IRQHandler
                Default_Handler SDIO_IRQHandler
                Default_Handler CryptoSS_Reset_Status_IRQHandler
                Default_Handler HostMHUS0_Int_Acc_NR2R_IRQHandler
                Default_Handler HostMHUS0_Int_Acc_R2NR_IRQHandler
                Default_Handler HostMHUR0_IRQ_Reg0_IRQHandler
                Default_Handler HostMHUR0_IRQ_Reg1_IRQHandler
                Default_Handler HostMHUR0_IRQComb_IRQHandler
                Default_Handler HostMHUS1_Int_Acc_NR2R_IRQHandler
                Default_Handler HostMHUS1_Int_Acc_R2NR_IRQHandler
                Default_Handler HostMHUR1_IRQ_Reg0_IRQHandler
                Default_Handler HostMHUR1_IRQ_Reg1_IRQHandler
                Default_Handler HostMHUR1_IRQComb_IRQHandler

                ALIGN

                END
