;/******************************************************************************
; * @file     startup_ARMCM3.s
; * @brief    CMSIS Cortex-M4 Core Device Startup File
; *           for CM3 Device Series
; * @version  V1.05
; * @date     25. July 2011
; *
; * @note
; * Copyright (C) 2010-2011 ARM Limited. All rights reserved.
; *
; * @par
; * ARM Limited (ARM) is supplying this software for use with Cortex-M
; * processor based microcontrollers.  This file can be freely distributed
; * within development tools that are supporting such ARM based processors.
; *
; * @par
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/


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

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        EXTERN  fInitSource
        EXTERN  HardFault_Handler
        EXTERN  SVC_Handler
        EXTERN  PendSV_Handler
        EXTERN  SysTick_Handler
        EXTERN  fIrqTim0Handler
        EXTERN  fIrqTim1Handler
        EXTERN  fIrqTim2Handler
        EXTERN  fIrqGpioHandler
        EXTERN  fIrqSpiHandler
        EXTERN  fIrqUart1Handler
        EXTERN  fIrqUart2Handler
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler
#ifndef RAM_VECTOR_TABLE
        DCD     NMI_Handler
        DCD     HardFault_Handler
        DCD     MemManage_Handler
        DCD     BusFault_Handler
        DCD     UsageFault_Handler
#endif
__vector_table_0x1c
#ifndef RAM_VECTOR_TABLE
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
        DCD        fIrqTim0Handler
        DCD        fIrqTim1Handler
        DCD        fIrqTim2Handler
        DCD        fIrqUart1Handler
        DCD        fIrqSpiHandler
        DCD        fIrqI2CHandler
        DCD        fIrqGpioHandler
        DCD        fIrqRtcHandler
        DCD        fIrqFlashHandler
        DCD        fIrqMacHwHandler
        DCD        fIrqAesHandler
        DCD        fIrqAdcHandler
        DCD        fIrqClockCalHandler
        DCD        fIrqUart2Handler
        DCD        fIrqUviHandler
        DCD        fIrqDmaHandler
        DCD        fIrqDbgPwrUpHandler
        /* REV C/D interrupts */
        DCD        fIrqSpi2Handler
        DCD        fIrqI2c2Handler
        DCD        FIrqFVDDHCompHandler    /* FVDDH Supply Comparator Trip */
#endif
__Vectors_End

__Vectors       EQU   __vector_table
__Vectors_Size  EQU   __Vectors_End - __Vectors

opt:        DC32     0x2082353F /* Full featured device */
opt_reg:    DC32     0x4001E000
enable:     DC32     0x00000000
per_en:     DC32     0x4001B010

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

;; Taken from article http://netstorage.iar.com/SuppDB/Public/UPDINFO/007040/arm/doc/infocenter/ilinkarm.ENU.html
;; If this line is removed, veneers for functions copied into RAM are flasely also placed in RAM, but are NOT
;; copied into it by __iar_copy_init3
__iar_init$$done

        PUBLIC Reset_Handler
        SECTION .text:CODE:REORDER(2)
Reset_Handler
         LDR     R0,= enable        ;; load R0 with address of enable
         LDR     R0,[R0]            ;; load R0 with what address R0 points to
         LDR     R1,= per_en        ;; load R1 with address of per_en
         LDR     R1,[R1]            ;; load R1 with what address R1 points to
         STR     R0,[R1]            ;; store R0 into address pointed to by R1 /* Disable all peripherals */

         LDR     R0,= opt           ;; load R0 with address of opt
         LDR     R0,[R0]            ;; load R0 with what address R0 points to
         LDR     R1,= opt_reg       ;; load R1 with address of opt_reg
         LDR     R1,[R1]            ;; load R1 with what address R1 points to
         STR     R0, [R1]           ;; store R0 into address pointed to by R1 /* Device option: Full featured device */

         LDR     R0,= sfe(CSTACK)
         MOV     SP,R0
         LDR     R0, =SystemInit
         BLX     R0
         LDR     R0, =__iar_program_start
         BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B NMI_Handler

;        PUBWEAK HardFault_Handler
;        SECTION .text:CODE:REORDER:NOROOT(1)
;HardFault_Handler
;        B HardFault_Handler

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
MemManage_Handler
        B MemManage_Handler

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BusFault_Handler
        B BusFault_Handler

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UsageFault_Handler
        B UsageFault_Handler

;        PUBWEAK vPortSVCHandler
;        SECTION .text:CODE:REORDER:NOROOT(1)
;vPortSVCHandler
;        B vPortSVCHandler

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DebugMon_Handler
        B DebugMon_Handler

;        PUBWEAK xPortPendSVHandler
;        SECTION .text:CODE:REORDER:NOROOT(1)
;xPortPendSVHandler
;        B xPortPendSVHandler

;        PUBWEAK SysTick_Handler
;        SECTION .text:CODE:REORDER:NOROOT(1)
;SysTick_Handler
;        B SysTick_Handler


;        PUBWEAK fIrqTim0Handler
;        SECTION .text:CODE:REORDER:NOROOT(1)
;fIrqTim0Handler
;        B fIrqTim0Handler

;        PUBWEAK fIrqTim1Handler
;        SECTION .text:CODE:REORDER:NOROOT(1)
;fIrqTim1Handler
;        B fIrqTim1Handler

;        PUBWEAK fIrqTim2Handler
;        SECTION .text:CODE:REORDER:NOROOT(1)
;fIrqTim2Handler
;        B fIrqTim2Handler

;        PUBWEAK fIrqUart1Handler
;        SECTION .text:CODE:REORDER:NOROOT(1)
;fIrqUart1Handler
;        B fIrqUart1Handler

;        PUBWEAK fIrqSpiHandler
;        SECTION .text:CODE:REORDER:NOROOT(1)
;fIrqSpiHandler
;        B fIrqSpiHandler

        PUBWEAK fIrqI2CHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
fIrqI2CHandler
        B fIrqI2CHandler

;        PUBWEAK fIrqGpioHandler
;        SECTION .text:CODE:REORDER:NOROOT(1)
;fIrqGpioHandler
;        B fIrqGpioHandler

        PUBWEAK fIrqRtcHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
fIrqRtcHandler
        B fIrqRtcHandler

        PUBWEAK fIrqFlashHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
fIrqFlashHandler
        B fIrqFlashHandler

        PUBWEAK fIrqMacHwHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
fIrqMacHwHandler
        B fIrqMacHwHandler

        PUBWEAK fIrqAesHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
fIrqAesHandler
        B fIrqAesHandler

        PUBWEAK fIrqAdcHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
fIrqAdcHandler
        B fIrqAdcHandler

        PUBWEAK fIrqClockCalHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
fIrqClockCalHandler
        B fIrqClockCalHandler

;        PUBWEAK fIrqUart2Handler
;        SECTION .text:CODE:REORDER:NOROOT(1)
;fIrqUart2Handler
;        B fIrqUart2Handler

        PUBWEAK fIrqDbgPwrUpHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
fIrqDbgPwrUpHandler
        B fIrqDbgPwrUpHandler

        PUBWEAK fIrqDmaHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
fIrqDmaHandler
        B fIrqDmaHandler

        PUBWEAK fIrqUviHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
fIrqUviHandler
        B fIrqUviHandler

        PUBWEAK fIrqSpi2Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
fIrqSpi2Handler
        B fIrqSpi2Handler

        PUBWEAK fIrqI2c2Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
fIrqI2c2Handler
        B fIrqI2c2Handler

        PUBWEAK FIrqFVDDHCompHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
FIrqFVDDHCompHandler
        B FIrqFVDDHCompHandler

        PUBWEAK DEF_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DEF_IRQHandler
        B DEF_IRQHandler

        END
