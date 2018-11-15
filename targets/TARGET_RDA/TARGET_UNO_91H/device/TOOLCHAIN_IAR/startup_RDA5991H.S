;/*****************************************************************************
; * @file:    startup_RDA5991H.s
; * @purpose: CMSIS Cortex-M4 Core Device Startup File
; *           for the RDA5981x Device Series
; * @version: V1.00
; * @date:    09. June 2018
; *----------------------------------------------------------------------------
; *
; * Copyright (C) 2010 ARM Limited. All rights reserved.
; *
; * ARM Limited (ARM) is supplying this software for use with Cortex-Mx
; * processor based microcontrollers.  This file can be freely distributed
; * within development tools that are supporting such ARM based processors.
; *
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/


        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        EXTERN  rda_ccfg_boot
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler

        DCD     NMI_Handler
        DCD     HardFault_Handler
        DCD     MemManage_Handler
        DCD     BusFault_Handler
        DCD     UsageFault_Handler
__vector_table_0x1c
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
        DCD     SPIFLASH_IRQHandler
        DCD     PTA_IRQHandler
        DCD     SDIO_IRQHandler
        DCD     USBDMA_IRQHandler
        DCD     USB_IRQHandler
        DCD     GPIO_IRQHandler
        DCD     TIMER0_IRQHandler
        DCD     UART0_IRQHandler
        DCD     MACHW_IRQHandler
        DCD     UART1_IRQHandler
        DCD     AHBDMA_IRQHandler
        DCD     PSRAM_IRQHandler
        DCD     SDMMC_IRQHandler
        DCD     EXIF_IRQHandler
        DCD     I2C_IRQHandler
__Vectors_End
__Vectors       EQU   __vector_table
__Vectors_Size  EQU   __Vectors_End - __Vectors


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:NOROOT:REORDER(2)
Reset_Handler
        LDR     R0, =sfe(CSTACK)
        MSR     MSP, R0
        LDR     R0, =rda_ccfg_boot
        BLX     R0
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
MemManage_Handler
        B MemManage_Handler

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
BusFault_Handler
        B BusFault_Handler

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
UsageFault_Handler
        B UsageFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
DebugMon_Handler
        B DebugMon_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SysTick_Handler
        B SysTick_Handler

        PUBWEAK SPIFLASH_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPIFLASH_IRQHandler
        B SPIFLASH_IRQHandler

        PUBWEAK PTA_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PTA_IRQHandler
        B PTA_IRQHandler

        PUBWEAK SDIO_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SDIO_IRQHandler
        B SDIO_IRQHandler

        PUBWEAK USBDMA_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
USBDMA_IRQHandler
        B USBDMA_IRQHandler

        PUBWEAK USB_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
USB_IRQHandler
        B USB_IRQHandler

        PUBWEAK GPIO_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
GPIO_IRQHandler
        B GPIO_IRQHandler

        PUBWEAK TIMER0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIMER0_IRQHandler
        B TIMER0_IRQHandler

        PUBWEAK UART0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART0_IRQHandler
        B UART0_IRQHandler

        PUBWEAK MACHW_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
MACHW_IRQHandler
        B MACHW_IRQHandler

        PUBWEAK UART1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART1_IRQHandler
        B UART1_IRQHandler

        PUBWEAK AHBDMA_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
AHBDMA_IRQHandler
        B AHBDMA_IRQHandler

        PUBWEAK PSRAM_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PSRAM_IRQHandler
        B PSRAM_IRQHandler

        PUBWEAK SDMMC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SDMMC_IRQHandler
        B SDMMC_IRQHandler

        PUBWEAK EXIF_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXIF_IRQHandler
        B EXIF_IRQHandler

        PUBWEAK I2C_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C_IRQHandler
        B I2C_IRQHandler

        END
