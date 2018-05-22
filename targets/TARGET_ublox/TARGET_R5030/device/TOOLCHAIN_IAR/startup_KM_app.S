;/**************************************************************************//**
; * @file     startup_KM_app.s
; * @brief    CMSIS Cortex-M7 Core Device Startup File for
; *           Device KM
; * @version  V5.00
; * @date     07. March 2016
; ******************************************************************************/
;/*
; * Copyright (c) 2009-2016 ARM Limited. All rights reserved.
; *
; * SPDX-License-Identifier: Apache-2.0
; *
; * Licensed under the Apache License, Version 2.0 (the License); you may
; * not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; * www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an AS IS BASIS, WITHOUT
; * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; */


;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;
        MODULE  ?cstartup
        ; Stack size default : 1024
        ; Heap size default : 2048
        ; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(4)
        EXTERN  __iar_program_start
        EXTERN  SystemInit
        ;PUBLIC  __vector_table
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA
__Vectors
        DCD     sfe(CSTACK)
        DCD     Reset_Handler
        DCD     NMI_Handler
        DCD     HardFault_Handler
        DCD     MemManage_Handler
        DCD     BusFault_Handler
        DCD     UsageFault_Handler
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     SVC_Handler
        DCD     DebugMon_Handler
        DCD     0
        DCD     PendSV_Handler
        DCD     SysTick_Handler

; External Interrupts
    DCD    APP_CPU_APP_IRQ_CTIIRQ0_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_CTIIRQ1_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_I2C1_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_I2C2_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_I2C3_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_I2S_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_UART1_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_UART2_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_SPI1_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_SPI2_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_TIMER1_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_TIMER2_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_MDM_WDG_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_PHY_WDG_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_RF_WDG_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_SYSCTRL_ERR_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_SYSCTRL_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_DMAC_ERR_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_DMAC_TC_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_DMAC_COMB_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_SDIO_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_OSPI_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_PSRAM_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_CRYPTO_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_DVS_GLB_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_DVS_CORE_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_USB_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_USB_DMA_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_PIO_1_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_PIO_2_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_PIO_WAKEUP_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_PWRCTRL_ERR_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_PWRCTRL_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_BB_IPC_MBX_TX0_IRQHandler
    DCD    APP_CPU_APP_IRQ_BB_IPC_MBX_TX1_IRQHandler
    DCD    APP_CPU_APP_IRQ_BB_IPC_MBX_TX2_IRQHandler
    DCD    APP_CPU_APP_IRQ_BB_IPC_MBX_RX0_IRQHandler
    DCD    APP_CPU_APP_IRQ_BB_IPC_MBX_RX1_IRQHandler
    DCD    APP_CPU_APP_IRQ_BB_IPC_MBX_RX2_IRQHandler
    DCD    APP_CPU_APP_IRQ_GNSS_TIM_IRQHandler
    DCD    APP_CPU_APP_IRQ_OTP_FAULT_IRQHandler
    DCD    APP_CPU_APP_IRQ_ONFI_CAL_IRQHandler
    DCD    APP_CPU_APP_IRQ_PWRCTRL_WAKEUP_IRQHandler
    DCD    APP_CPU_APP_IRQ_ETR_LIMITER_THRESHOLD_IRQHandler
    DCD    APP_CPU_APP_IRQ_USB_WAKEUP_INT_IRQHandler
    DCD    APP_CPU_APP_IRQ_AC_PWR_ALERT_IRQHandler

__Vectors_End

__Vectors_Size                      EQU   __Vectors_End - __Vectors

; Default handlers.
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:NOROOT(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        ; Dummy exception handlers
        PUBWEAK NMI_Handler
        SECTION .text:CODE:NOROOT(1)
NMI_Handler
        B .
        PUBWEAK HardFault_Handler
        SECTION .text:CODE:NOROOT(1)
HardFault_Handler
        B .
        PUBWEAK MemManage_Handler
        SECTION .text:CODE:NOROOT(1)
MemManage_Handler
        B .
        PUBWEAK BusFault_Handler
        SECTION .text:CODE:NOROOT(1)
BusFault_Handler
        B .
        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:NOROOT(1)
UsageFault_Handler
        B .
        PUBWEAK SVC_Handler
        SECTION .text:CODE:NOROOT(1)
SVC_Handler
        B .
        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:NOROOT(1)
DebugMon_Handler
        B .
        PUBWEAK PendSV_Handler
        SECTION .text:CODE:NOROOT(1)
PendSV_Handler
        B .
        PUBWEAK SysTick_Handler
        SECTION .text:CODE:NOROOT(1)
SysTick_Handler
        B .

       ; Dummy interrupt handlers
        PUBWEAK  APP_CPU_APP_IRQ_CTIIRQ0_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_CTIIRQ0_INT_IRQHandler
        B .
        PUBWEAK  APP_CPU_APP_IRQ_CTIIRQ1_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_CTIIRQ1_INT_IRQHandler
        B .
        PUBWEAK  APP_CPU_APP_IRQ_I2C1_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_I2C1_INT_IRQHandler
        B .
        PUBWEAK  APP_CPU_APP_IRQ_I2C2_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_I2C2_INT_IRQHandler
        B .
        PUBWEAK  APP_CPU_APP_IRQ_I2C3_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_I2C3_INT_IRQHandler
        B .
        PUBWEAK  APP_CPU_APP_IRQ_I2S_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_I2S_INT_IRQHandler
        B .
        PUBWEAK  APP_CPU_APP_IRQ_UART1_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_UART1_INT_IRQHandler
        B .
        PUBWEAK  APP_CPU_APP_IRQ_UART2_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_UART2_INT_IRQHandler
        B .
        PUBWEAK  APP_CPU_APP_IRQ_SPI1_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_SPI1_INT_IRQHandler
        B .
        PUBWEAK  APP_CPU_APP_IRQ_SPI2_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_SPI2_INT_IRQHandler
        B .
        PUBWEAK  APP_CPU_APP_IRQ_TIMER1_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_TIMER1_INT_IRQHandler
        B .
        PUBWEAK  APP_CPU_APP_IRQ_TIMER2_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_TIMER2_INT_IRQHandler
        B .
        PUBWEAK  APP_CPU_APP_IRQ_MDM_WDG_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_MDM_WDG_INT_IRQHandler
        B .
        PUBWEAK  APP_CPU_APP_IRQ_PHY_WDG_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_PHY_WDG_INT_IRQHandler
        B .
        PUBWEAK  APP_CPU_APP_IRQ_RF_WDG_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_RF_WDG_INT_IRQHandler
        B .
        PUBWEAK  APP_CPU_APP_IRQ_SYSCTRL_ERR_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_SYSCTRL_ERR_INT_IRQHandler
        B .
        PUBWEAK  APP_CPU_APP_IRQ_SYSCTRL_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_SYSCTRL_INT_IRQHandler
        B .
        PUBWEAK  APP_CPU_APP_IRQ_DMAC_ERR_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_DMAC_ERR_INT_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_DMAC_TC_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_DMAC_TC_INT_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_DMAC_COMB_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_DMAC_COMB_INT_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_SDIO_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_SDIO_INT_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_OSPI_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_OSPI_INT_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_PSRAM_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_PSRAM_INT_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_CRYPTO_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_CRYPTO_INT_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_DVS_GLB_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_DVS_GLB_INT_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_DVS_CORE_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_DVS_CORE_INT_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_USB_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_USB_INT_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_USB_DMA_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_USB_DMA_INT_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_PIO_1_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_PIO_1_INT_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_PIO_2_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_PIO_2_INT_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_PIO_WAKEUP_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_PIO_WAKEUP_INT_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_PWRCTRL_ERR_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_PWRCTRL_ERR_INT_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_PWRCTRL_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_PWRCTRL_INT_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_BB_IPC_MBX_TX0_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_BB_IPC_MBX_TX0_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_BB_IPC_MBX_TX1_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_BB_IPC_MBX_TX1_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_BB_IPC_MBX_TX2_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_BB_IPC_MBX_TX2_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_BB_IPC_MBX_RX0_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_BB_IPC_MBX_RX0_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_BB_IPC_MBX_RX1_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_BB_IPC_MBX_RX1_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_BB_IPC_MBX_RX2_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_BB_IPC_MBX_RX2_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_GNSS_TIM_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_GNSS_TIM_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_OTP_FAULT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_OTP_FAULT_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_ONFI_CAL_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_ONFI_CAL_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_PWRCTRL_WAKEUP_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_PWRCTRL_WAKEUP_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_ETR_LIMITER_THRESHOLD_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_ETR_LIMITER_THRESHOLD_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_USB_WAKEUP_INT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_USB_WAKEUP_INT_IRQHandler
        B.
        PUBWEAK  APP_CPU_APP_IRQ_AC_PWR_ALERT_IRQHandler
        SECTION .text:CODE:NOROOT(1)
APP_CPU_APP_IRQ_AC_PWR_ALERT_IRQHandler
        B.
        END
