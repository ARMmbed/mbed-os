;/**************************************************************************//**
; * @file     startup_CC3220SF.s
; * @brief    CMSIS Cortex-M4 Core Device Startup File for
; *           Device CC3220SF
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

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table
        PUBLIC  __Vectors

        DATA
__vector_table
        DCD     sfe(CSTACK)
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
        DCD     INT_GPIOA0_IRQHandler     /* 16 */
        DCD     INT_GPIOA1_IRQHandler     /* 17 */
        DCD     INT_GPIOA2_IRQHandler     /* 18 */
        DCD     INT_GPIOA3_IRQHandler     /* 19 */
        DCD     0                         /* 20 Reserved interrupt */
        DCD     INT_UARTA0_IRQHandler     /* 21 */
        DCD     INT_UARTA1_IRQHandler     /* 22 */
        DCD     0                         /* 23 Reserved interrupt */
        DCD     INT_I2CA0_IRQHandler      /* 24 */
        DCD     0                         /* 25 Reserved interrupt */
        DCD     0                         /* 26 Reserved interrupt */
        DCD     0                         /* 27 Reserved interrupt */
        DCD     0                         /* 28 Reserved interrupt */
        DCD     0                         /* 29 Reserved interrupt */
        DCD     INT_ADCCH0_IRQHandler     /* 30 */
        DCD     INT_ADCCH1_IRQHandler     /* 31 */
        DCD     INT_ADCCH2_IRQHandler     /* 32 */
        DCD     INT_ADCCH3_IRQHandler     /* 33 */
        DCD     INT_WDT_IRQHandler        /* 34 */
        DCD     INT_TIMERA0A_IRQHandler   /* 35 */
        DCD     INT_TIMERA0B_IRQHandler   /* 36 */
        DCD     INT_TIMERA1A_IRQHandler   /* 37 */
        DCD     INT_TIMERA1B_IRQHandler   /* 38 */
        DCD     INT_TIMERA2A_IRQHandler   /* 39 */
        DCD     INT_TIMERA2B_IRQHandler   /* 40 */
        DCD     0                         /* 41 Reserved interrupt */
        DCD     0                         /* 42 Reserved interrupt */
        DCD     0                         /* 43 Reserved interrupt */
        DCD     0                         /* 44 Reserved interrupt */
        DCD     INT_FLASH_IRQHandler      /* 45 */
        DCD     0                         /* 46 Reserved interrupt */
        DCD     0                         /* 47 Reserved interrupt */
        DCD     0                         /* 48 Reserved interrupt */
        DCD     0                         /* 49 Reserved interrupt */
        DCD     0                         /* 50 Reserved interrupt */
        DCD     INT_TIMERA3A_IRQHandler   /* 51 */
        DCD     INT_TIMERA3B_IRQHandler   /* 52 */
        DCD     0                         /* 53 Reserved interrupt */
        DCD     0                         /* 54 Reserved interrupt */
        DCD     0                         /* 55 Reserved interrupt */
        DCD     0                         /* 56 Reserved interrupt */
        DCD     0                         /* 57 Reserved interrupt */
        DCD     0                         /* 58 Reserved interrupt */
        DCD     0                         /* 59 Reserved interrupt */
        DCD     0                         /* 60 Reserved interrupt */
        DCD     0                         /* 61 Reserved interrupt */
        DCD     INT_UDMA_IRQHandler       /* 62 */
        DCD     INT_UDMAERR_IRQHandler    /* 63 */
        DCD     0                         /* 64 Reserved interrupt */
        DCD     0                         /* 65 Reserved interrupt */
        DCD     0                         /* 66 Reserved interrupt */
        DCD     0                         /* 67 Reserved interrupt */
        DCD     0                         /* 68 Reserved interrupt */
        DCD     0                         /* 69 Reserved interrupt */
        DCD     0                         /* 70 Reserved interrupt */
        DCD     0                         /* 71 Reserved interrupt */
        DCD     0                         /* 72 Reserved interrupt */
        DCD     0                         /* 73 Reserved interrupt */
        DCD     0                         /* 74 Reserved interrupt */
        DCD     0                         /* 75 Reserved interrupt */
        DCD     0                         /* 76 Reserved interrupt */
        DCD     0                         /* 77 Reserved interrupt */
        DCD     0                         /* 78 Reserved interrupt */
        DCD     0                         /* 79 Reserved interrupt */
        DCD     0                         /* 80 Reserved interrupt */
        DCD     0                         /* 81 Reserved interrupt */
        DCD     0                         /* 82 Reserved interrupt */
        DCD     0                         /* 83 Reserved interrupt */
        DCD     0                         /* 84 Reserved interrupt */
        DCD     0                         /* 85 Reserved interrupt */
        DCD     0                         /* 86 Reserved interrupt */
        DCD     0                         /* 87 Reserved interrupt */
        DCD     0                         /* 88 Reserved interrupt */
        DCD     0                         /* 89 Reserved interrupt */
        DCD     0                         /* 90 Reserved interrupt */
        DCD     0                         /* 91 Reserved interrupt */
        DCD     0                         /* 92 Reserved interrupt */
        DCD     0                         /* 93 Reserved interrupt */
        DCD     0                         /* 94 Reserved interrupt */
        DCD     0                         /* 95 Reserved interrupt */
        DCD     0                         /* 96 Reserved interrupt */
        DCD     0                         /* 97 Reserved interrupt */
        DCD     0                         /* 98 Reserved interrupt */
        DCD     0                         /* 99 Reserved interrupt */
        DCD     0                         /* 100 Reserved interrupt */
        DCD     0                         /* 101 Reserved interrupt */
        DCD     0                         /* 102 Reserved interrupt */
        DCD     0                         /* 103 Reserved interrupt */
        DCD     0                         /* 104 Reserved interrupt */
        DCD     0                         /* 105 Reserved interrupt */
        DCD     0                         /* 106 Reserved interrupt */
        DCD     0                         /* 107 Reserved interrupt */
        DCD     0                         /* 108 Reserved interrupt */
        DCD     0                         /* 109 Reserved interrupt */
        DCD     0                         /* 110 Reserved interrupt */
        DCD     0                         /* 111 Reserved interrupt */
        DCD     0                         /* 112 Reserved interrupt */
        DCD     0                         /* 113 Reserved interrupt */
        DCD     0                         /* 114 Reserved interrupt */
        DCD     0                         /* 115 Reserved interrupt */
        DCD     0                         /* 116 Reserved interrupt */
        DCD     0                         /* 117 Reserved interrupt */
        DCD     0                         /* 118 Reserved interrupt */
        DCD     0                         /* 119 Reserved interrupt */
        DCD     0                         /* 120 Reserved interrupt */
        DCD     0                         /* 121 Reserved interrupt */
        DCD     0                         /* 122 Reserved interrupt */
        DCD     0                         /* 123 Reserved interrupt */
        DCD     0                         /* 124 Reserved interrupt */
        DCD     0                         /* 125 Reserved interrupt */
        DCD     0                         /* 126 Reserved interrupt */
        DCD     0                         /* 127 Reserved interrupt */
        DCD     0                         /* 128 Reserved interrupt */
        DCD     0                         /* 129 Reserved interrupt */
        DCD     0                         /* 130 Reserved interrupt */
        DCD     0                         /* 131 Reserved interrupt */
        DCD     0                         /* 132 Reserved interrupt */
        DCD     0                         /* 133 Reserved interrupt */
        DCD     0                         /* 134 Reserved interrupt */
        DCD     0                         /* 135 Reserved interrupt */
        DCD     0                         /* 136 Reserved interrupt */
        DCD     0                         /* 137 Reserved interrupt */
        DCD     0                         /* 138 Reserved interrupt */
        DCD     0                         /* 139 Reserved interrupt */
        DCD     0                         /* 140 Reserved interrupt */
        DCD     0                         /* 141 Reserved interrupt */
        DCD     0                         /* 142 Reserved interrupt */
        DCD     0                         /* 143 Reserved interrupt */
        DCD     0                         /* 144 Reserved interrupt */
        DCD     0                         /* 145 Reserved interrupt */
        DCD     0                         /* 146 Reserved interrupt */
        DCD     0                         /* 147 Reserved interrupt */
        DCD     0                         /* 148 Reserved interrupt */
        DCD     0                         /* 149 Reserved interrupt */
        DCD     0                         /* 150 Reserved interrupt */
        DCD     0                         /* 151 Reserved interrupt */
        DCD     0                         /* 152 Reserved interrupt */
        DCD     0                         /* 153 Reserved interrupt */
        DCD     0                         /* 154 Reserved interrupt */
        DCD     0                         /* 155 Reserved interrupt */
        DCD     0                         /* 156 Reserved interrupt */
        DCD     0                         /* 157 Reserved interrupt */
        DCD     0                         /* 158 Reserved interrupt */
        DCD     0                         /* 159 Reserved interrupt */
        DCD     0                         /* 160 Reserved interrupt */
        DCD     0                         /* 161 Reserved interrupt */
        DCD     0                         /* 162 Reserved interrupt */
        DCD     0                         /* 163 Reserved interrupt */
        DCD     INT_SHA_IRQHandler        /* 164 */
        DCD     0                         /* 165 Reserved interrupt */
        DCD     0                         /* 166 Reserved interrupt */
        DCD     INT_AES_IRQHandler        /* 167 */
        DCD     0                         /* 168 Reserved interrupt */
        DCD     INT_DES_IRQHandler        /* 169 */
        DCD     0                         /* 170 Reserved interrupt */
        DCD     0                         /* 171 Reserved interrupt */
        DCD     0                         /* 172 Reserved interrupt */
        DCD     0                         /* 173 Reserved interrupt */
        DCD     0                         /* 174 Reserved interrupt */
        DCD     INT_MMCHS_IRQHandler      /* 175 */
        DCD     0                         /* 176 Reserved interrupt */
        DCD     INT_I2S_IRQHandler        /* 177 */
        DCD     0                         /* 178 Reserved interrupt */
        DCD     INT_CAMERA_IRQHandler     /* 179 */
        DCD     0                         /* 180 Reserved interrupt */
        DCD     0                         /* 181 Reserved interrupt */
        DCD     0                         /* 182 Reserved interrupt */
        DCD     0                         /* 183 Reserved interrupt */
        DCD     0                         /* 184 Reserved interrupt */
        DCD     0                         /* 185 Reserved interrupt */
        DCD     0                         /* 186 Reserved interrupt */
        DCD     INT_NWPIC_IRQHandler      /* 187 */
        DCD     INT_PRCM_IRQHandler       /* 188 */
        DCD     0                         /* 189 Reserved interrupt */
        DCD     0                         /* 190 Reserved interrupt */
        DCD     INT_SSPI_IRQHandler       /* 191 */
        DCD     INT_GSPI_IRQHandler       /* 192 */
        DCD     INT_LSPI_IRQHandler       /* 193 */
        DCD     0                         /* 194 Reserved interrupt */

__Vectors       EQU   __vector_table

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB
        PUBWEAK Reset_Handler
        SECTION .text:CODE:NOROOT:REORDER(2)
Reset_Handler

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

        PUBWEAK INT_GPIOA0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_GPIOA0_IRQHandler
        B INT_GPIOA0_IRQHandler

        PUBWEAK INT_GPIOA1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_GPIOA1_IRQHandler
        B INT_GPIOA1_IRQHandler

        PUBWEAK INT_GPIOA2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_GPIOA2_IRQHandler
        B INT_GPIOA2_IRQHandler

        PUBWEAK INT_GPIOA3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_GPIOA3_IRQHandler
        B INT_GPIOA3_IRQHandler

        PUBWEAK INT_UARTA0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_UARTA0_IRQHandler
        B INT_UARTA0_IRQHandler

        PUBWEAK INT_UARTA1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_UARTA1_IRQHandler
        B INT_UARTA1_IRQHandler

        PUBWEAK INT_I2CA0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_I2CA0_IRQHandler
        B INT_I2CA0_IRQHandler

        PUBWEAK INT_ADCCH0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_ADCCH0_IRQHandler
        B INT_ADCCH0_IRQHandler

        PUBWEAK INT_ADCCH1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_ADCCH1_IRQHandler
        B INT_ADCCH1_IRQHandler

        PUBWEAK INT_ADCCH2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_ADCCH2_IRQHandler
        B INT_ADCCH2_IRQHandler

        PUBWEAK INT_ADCCH3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_ADCCH3_IRQHandler
        B INT_ADCCH3_IRQHandler

        PUBWEAK INT_WDT_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_WDT_IRQHandler
        B INT_WDT_IRQHandler

        PUBWEAK INT_TIMERA0A_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_TIMERA0A_IRQHandler
        B INT_TIMERA0A_IRQHandler

        PUBWEAK INT_TIMERA0B_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_TIMERA0B_IRQHandler
        B INT_TIMERA0B_IRQHandler

        PUBWEAK INT_TIMERA1A_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_TIMERA1A_IRQHandler
        B INT_TIMERA1A_IRQHandler

        PUBWEAK INT_TIMERA1B_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_TIMERA1B_IRQHandler
        B INT_TIMERA1B_IRQHandler

        PUBWEAK INT_TIMERA2A_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_TIMERA2A_IRQHandler
        B INT_TIMERA2A_IRQHandler

        PUBWEAK INT_TIMERA2B_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_TIMERA2B_IRQHandler
        B INT_TIMERA2B_IRQHandler

        PUBWEAK INT_FLASH_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_FLASH_IRQHandler
        B INT_FLASH_IRQHandler

        PUBWEAK INT_TIMERA3A_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_TIMERA3A_IRQHandler
        B INT_TIMERA3A_IRQHandler

        PUBWEAK INT_TIMERA3B_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_TIMERA3B_IRQHandler
        B INT_TIMERA3B_IRQHandler

        PUBWEAK INT_UDMA_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_UDMA_IRQHandler
        B INT_UDMA_IRQHandler

        PUBWEAK INT_UDMAERR_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_UDMAERR_IRQHandler
        B INT_UDMAERR_IRQHandler

        PUBWEAK INT_SHA_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_SHA_IRQHandler
        B INT_SHA_IRQHandler

        PUBWEAK INT_AES_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_AES_IRQHandler
        B INT_AES_IRQHandler

        PUBWEAK INT_DES_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_DES_IRQHandler
        B INT_DES_IRQHandler

        PUBWEAK INT_MMCHS_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_MMCHS_IRQHandler
        B INT_MMCHS_IRQHandler

        PUBWEAK INT_I2S_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_I2S_IRQHandler
        B INT_I2S_IRQHandler

        PUBWEAK INT_CAMERA_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_CAMERA_IRQHandler
        B INT_CAMERA_IRQHandler

        PUBWEAK INT_NWPIC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_NWPIC_IRQHandler
        B INT_NWPIC_IRQHandler

        PUBWEAK INT_PRCM_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_PRCM_IRQHandler
        B INT_PRCM_IRQHandler

        PUBWEAK INT_SSPI_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_SSPI_IRQHandler
        B INT_SSPI_IRQHandler

        PUBWEAK INT_GSPI_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_GSPI_IRQHandler
        B INT_GSPI_IRQHandler

        PUBWEAK INT_LSPI_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
INT_LSPI_IRQHandler
        B INT_LSPI_IRQHandler
        END
