;/**************************************************************************//**
; * @file     startup_SAML21.s
; * @brief    CMSIS Cortex-M4 Core Device Startup File for
; *           Atmel SAML21 Device Series
; * @version  V1.00
; * @date     10. February 2015
; *
; * @note
; * Copyright (C) 2015 ARM Limited. All rights reserved.
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
__initial_sp        EQU     0x20008000  ;Top of RAM

				PRESERVE8
				THUMB


;Vector Table Mapped to Address 0 at Reset

				AREA    RESET, DATA, READONLY
				EXPORT  __Vectors
				EXPORT  __Vectors_End
				EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ;Top of Stack
				DCD     Reset_Handler             ;Reset Handler
				DCD     NMI_Handler               ;NMI Handler
				DCD     HardFault_Handler         ;Hard Fault Handler
				DCD     0                         ;Reserved
				DCD     0                         ;Reserved
				DCD     0                         ;Reserved
				DCD     0                         ;Reserved
				DCD     0                         ;Reserved
				DCD     0                         ;Reserved
				DCD     0                         ;Reserved
				DCD     SVC_Handler               ;SVCall Handler
				DCD     0                         ;Reserved
				DCD     0                         ;Reserved
				DCD     PendSV_Handler            ;PendSV Handler
				DCD     SysTick_Handler           ;SysTick Handler

				;External Interrupts
				DCD     SYSTEM_Handler            ;0 Main Clock, Oscillators Control, 32k Oscillators Control, Peripheral Access Controller, Power Manager, Supply Controller, Trigger Allocator
				DCD     WDT_Handler               ;1 Watchdog Timer
				DCD     RTC_Handler               ;2 Real-Time Counter
				DCD     EIC_Handler               ;3 External Interrupt Controller
				DCD     NVMCTRL_Handler           ;4 Non-Volatile Memory Controller
				DCD     DMAC_Handler              ;5 Direct Memory Access Controller
				DCD     USB_Handler               ;6 Universal Serial Bus
				DCD     EVSYS_Handler             ;7 Event System Interface
				DCD     SERCOM0_Handler           ;8 Serial Communication Interface 0
				DCD     SERCOM1_Handler           ;9 Serial Communication Interface 1
				DCD     SERCOM2_Handler           ;10 Serial Communication Interface 2
				DCD     SERCOM3_Handler           ;11 Serial Communication Interface 3
				DCD     SERCOM4_Handler           ;12 Serial Communication Interface 4
				DCD     SERCOM5_Handler           ;13 Serial Communication Interface 5
				DCD     TCC0_Handler              ;14 Timer Counter Control 0
				DCD     TCC1_Handler              ;15 Timer Counter Control 1
				DCD     TCC2_Handler              ;16 Timer Counter Control 2
				DCD     TC0_Handler               ;17 Basic Timer Counter 0
				DCD     TC1_Handler               ;18 Basic Timer Counter 1
				DCD     TC2_Handler               ;19 Basic Timer Counter 2
				DCD     TC3_Handler               ;20 Basic Timer Counter 3
				DCD     TC4_Handler               ;21 Basic Timer Counter 4
				DCD     ADC_Handler               ;22 Analog Digital Converter
				DCD     AC_Handler                ;23 Analog Comparators
				DCD     DAC_Handler               ;24 Digital-to-Analog Converter
				DCD     PTC_Handler               ;25 Peripheral Touch Controller
				DCD     AES_Handler               ;26 Advanced Encryption Standard
				DCD     TRNG_Handler              ;27 True Random Generator
				DCD     PICOP_Handler             ;28 PicoProcessor
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

				AREA    |.text|, CODE, READONLY


;Reset Handler

Reset_Handler   PROC
				EXPORT  Reset_Handler             [WEAK]
				IMPORT  SystemInit
				IMPORT  __main
				LDR     R0, =SystemInit
				BLX     R0
				LDR     R0, =__main
				BX      R0
				ENDP


;Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
				EXPORT  NMI_Handler               [WEAK]
				B       .
				ENDP
HardFault_Handler\
				PROC
				EXPORT  HardFault_Handler         [WEAK]
				B       .
				ENDP
SVC_Handler     PROC
				EXPORT  SVC_Handler               [WEAK]
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
				EXPORT  SYSTEM_Handler            [WEAK]
				EXPORT  WDT_Handler               [WEAK]
				EXPORT  RTC_Handler               [WEAK]
				EXPORT  EIC_Handler               [WEAK]
				EXPORT  NVMCTRL_Handler           [WEAK]
				EXPORT  DMAC_Handler              [WEAK]
				EXPORT  USB_Handler               [WEAK]
				EXPORT  EVSYS_Handler             [WEAK]
				EXPORT  SERCOM0_Handler           [WEAK]
				EXPORT  SERCOM1_Handler           [WEAK]
				EXPORT  SERCOM2_Handler           [WEAK]
				EXPORT  SERCOM3_Handler           [WEAK]
				EXPORT  SERCOM4_Handler           [WEAK]
				EXPORT  SERCOM5_Handler           [WEAK]
				EXPORT  TCC0_Handler              [WEAK]
				EXPORT  TCC1_Handler              [WEAK]
				EXPORT  TCC2_Handler              [WEAK]
				EXPORT  TC0_Handler               [WEAK]
				EXPORT  TC1_Handler               [WEAK]
				EXPORT  TC2_Handler               [WEAK]
				EXPORT  TC3_Handler               [WEAK]
				EXPORT  TC4_Handler               [WEAK]
				EXPORT  ADC_Handler               [WEAK]
				EXPORT  AC_Handler                [WEAK]
				EXPORT  DAC_Handler               [WEAK]
				EXPORT  PTC_Handler               [WEAK]
				EXPORT  AES_Handler               [WEAK]
				EXPORT  TRNG_Handler              [WEAK]
				EXPORT  PICOP_Handler             [WEAK]

SYSTEM_Handler
WDT_Handler
RTC_Handler
EIC_Handler
NVMCTRL_Handler
DMAC_Handler
USB_Handler
EVSYS_Handler
SERCOM0_Handler
SERCOM1_Handler
SERCOM2_Handler
SERCOM3_Handler
SERCOM4_Handler
SERCOM5_Handler
TCC0_Handler
TCC1_Handler
TCC2_Handler
TC0_Handler
TC1_Handler
TC2_Handler
TC3_Handler
TC4_Handler
ADC_Handler
AC_Handler
DAC_Handler
PTC_Handler
AES_Handler
TRNG_Handler
PICOP_Handler
				B       .
				ENDP



				ALIGN
				END
