;/*****************************************************************************
; * @file     startup_PIC32CX.s
; * @brief    CMSIS Cortex-M4 Core Device Startup File for
; *           Microchip PIC32CX Device Series
; * @version  V1.00
; * @date     13. July 2020
; *
; * @note
; * Copyright (C) 2020 Microchip Technology Inc. All rights reserved.
; *
; * @par
; * Microchip Technology is supplying this software for use with Cortex-M
; * processor based microcontrollers.  This file can be freely distributed
; * within development tools that are supporting such ARM based processors.
; *
; * @par
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * MICROCHIP SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/
;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00002000

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ;    Top of Stack
                DCD     Reset_Handler             ;    Reset Handler
                DCD     NMI_Handler               ;    NMI Handler
                DCD     HardFault_Handler         ;    Hard Fault Handler
                DCD     MemManage_Handler         ;    MPU Fault Handler
                DCD     BusFault_Handler          ;    Bus Fault Handler
                DCD     UsageFault_Handler        ;    Usage Fault Handler
                DCD     0                         ;    Reserved
                DCD     0                         ;    Reserved
                DCD     0                         ;    Reserved
                DCD     0                         ;    Reserved
                DCD     SVC_Handler               ;    SVCall Handler
                DCD     DebugMon_Handler          ;    Debug Monitor Handler
                DCD     0                         ;    Reserved
                DCD     PendSV_Handler            ;    PendSV Handler
                DCD     SysTick_Handler           ;    SysTick Handler

                ; External Interrupts
                DCD     SUPC_Handler              ;  0 Supply Controller
                DCD     RSTC_Handler              ;  1 Reset Controller
                DCD     RTC_Handler               ;  2 Real Time Clock
                DCD     RTT_Handler               ;  3 Real Time Timer
                DCD     DWDT_Handler              ;  4 Dual Watchdog Timer
                DCD     0                         ;    Reserved
                DCD     PMC_Handler               ;  6 Power Management Controller
                DCD     SEFC0_Handler             ;  7 Embedded Flash Controller 0
                DCD     SEFC1_Handler             ;  8 Embedded Flash Controller 1
                DCD     FLEXCOM0_Handler          ;  9 FLEXCOM 0 (USART0/SPI0/TWI0)
                DCD     FLEXCOM1_Handler          ; 10 FLEXCOM 1 (USART1/SPI1/TWI1)
                DCD     FLEXCOM2_Handler          ; 11 FLEXCOM 2 (USART2/SPI2/TWI2)
                DCD     FLEXCOM3_Handler          ; 12 FLEXCOM 3 (USART3/SPI3/TWI3)
                DCD     FLEXCOM4_Handler          ; 13 FLEXCOM 4 (USART4/SPI4/TWI4)
                DCD     FLEXCOM5_Handler          ; 14 FLEXCOM 5 (USART5/SPI5/TWI5)
                DCD     FLEXCOM6_Handler          ; 15 FLEXCOM 6 (USART6/SPI6/TWI6)
                DCD     FLEXCOM7_Handler          ; 16 FLEXCOM 7 (USART7/SPI7/TWI7)
                DCD     PIOA_Handler              ; 17 Application Core Parallel I/O Controller A (PIOA)
                DCD     PIOA_SEC_Handler          ; 18 Application Core Parallel I/O Controller A (PIOA) Secure Event Interrupt
                DCD     PIOB_Handler              ; 19 Application Core Parallel I/O Controller B (PIOB)
                DCD     PIOB_SEC_Handler          ; 20 Application Core Parallel I/O Controller B (PIOB) Secure Event Interrupt
                DCD     PIOC_Handler              ; 21 Application Core Parallel I/O Controller C (PIOC)
                DCD     PIOC_SEC_Handler          ; 22 Application Core Parallel I/O Controller C (PIOC) Secure Event Interrupt
                DCD     QSPI_Handler              ; 23 Quad IO Serial Peripheral Interface
                DCD     ADC_Handler               ; 24 Analog To Digital Converter
                DCD     ACC_Handler               ; 25 Analog Comparator
                DCD     ARM0_FPU_Handler          ; 26 Application Core (CORE0) Floating Point Unit except IXC
                DCD     ARM0_IXC_Handler          ; 27 Application Core (CORE0) FPU Interrupt IXC associated with FPU cumulative exception bit
                DCD     IPC0_Handler              ; 28 Application Core Interprocessor communication (IPC0)
                DCD     SLCDC_Handler             ; 29 Segment LCD Controller
                DCD     MEM2MEM0_Handler          ; 30 Application Core Memory to Memory Transfer Controller (MEM2MEM0)
                DCD     TC0_CHANNEL0_Handler      ; 31 Timer Counter 0, Channel 0
                DCD     TC0_CHANNEL1_Handler      ; 32 Timer Counter 0, Channel 1
                DCD     TC0_CHANNEL2_Handler      ; 33 Timer Counter 0, Channel 2
                DCD     TC1_CHANNEL0_Handler      ; 34 Timer Counter 1, Channel 0
                DCD     TC1_CHANNEL1_Handler      ; 35 Timer Counter 1, Channel 1
                DCD     TC1_CHANNEL2_Handler      ; 36 Timer Counter 1, Channel 2
                DCD     TC2_CHANNEL0_Handler      ; 37 Timer Counter 2, Channel 0
                DCD     TC2_CHANNEL1_Handler      ; 38 Timer Counter 2, Channel 1
                DCD     TC2_CHANNEL2_Handler      ; 39 Timer Counter 2, Channel 2
                DCD     TC0_C0SEC_Handler         ; 40 Timer Counter 0, Channel 0, Secure IRQ
                DCD     TC0_C1SEC_Handler         ; 41 Timer Counter 0, Channel 1, Secure IRQ
                DCD     TC0_C2SEC_Handler         ; 42 Timer Counter 0, Channel 2, Secure IRQ
                DCD     TC1_C0SEC_Handler         ; 43 Timer Counter 1, Channel 0, Secure IRQ
                DCD     TC1_C1SEC_Handler         ; 44 Timer Counter 1, Channel 1, Secure IRQ
                DCD     TC1_C2SEC_Handler         ; 45 Timer Counter 1, Channel 2, Secure IRQ
                DCD     TC2_C0SEC_Handler         ; 46 Timer Counter 2, Channel 0, Secure IRQ
                DCD     TC2_C1SEC_Handler         ; 47 Timer Counter 2, Channel 1, Secure IRQ
                DCD     TC2_C2SEC_Handler         ; 48 Timer Counter 2, Channel 2, Secure IRQ
                DCD     AES_Handler               ; 49 Advanced Enhanced Standard
                DCD     AES_AESSEC_Handler        ; 50 AES Secure Event Interrupt
                DCD     AESB_Handler              ; 51 AES Bridge
                DCD     AESB_AESBSEC_Handler      ; 52 AES Bridge Secure Interrupt
                DCD     SHA_Handler               ; 53 Secure Hash Algorithm
                DCD     SHA_SHASEC_Handler        ; 54 SHA Secure Event Interrupt
                DCD     TRNG_Handler              ; 55 True Random Generator
                DCD     TRNG_TRNGSEC_Handler      ; 56 TRNG Secure Event Interrupt
                DCD     ICM_Handler               ; 57 Integrity Check Module
                DCD     ICM_ICMSEC_Handler        ; 58 Integrity Check Module
                DCD     CPKCC_Handler             ; 59 Public Key Cryptography Controller
                DCD     MATRIX0_Handler           ; 60 High-Speed Application Core Matrix (HSACM) (MATRIX0)
                DCD     MATRIX1_Handler           ; 61 Low-Speed Application Core Matrix (LSACM) (MATRIX1)
                DCD     SUPC_WKUP3_Handler        ; 62 External interrupt 3
                DCD     SUPC_WKUP4_Handler        ; 63 External interrupt 4
                DCD     SUPC_WKUP5_Handler        ; 64 External interrupt 5
                DCD     SUPC_WKUP6_Handler        ; 65 External interrupt 6
                DCD     SUPC_WKUP7_Handler        ; 66 External interrupt 7
                DCD     SUPC_WKUP8_Handler        ; 67 External interrupt 8
                DCD     SUPC_WKUP9_Handler        ; 68 External interrupt 9
                DCD     SUPC_WKUP10_Handler       ; 69 External interrupt 10
                DCD     SUPC_WKUP11_Handler       ; 70 External interrupt 11
                DCD     SUPC_WKUP12_Handler       ; 71 External interrupt 12
                DCD     SUPC_WKUP13_Handler       ; 72 External interrupt 13
                DCD     SUPC_WKUP14_Handler       ; 73 External interrupt 14
                DCD     SUPC_WKUP15_Handler       ; 74 External interrupt 15
                DCD     0                         ;    Reserved
                DCD     SEAFEC_SLINK_Handler      ; 76 Smart Energy Analog Front-End Controller Serial Link Interrupt
                DCD     SEAFEC_DATA_Handler       ; 77 Smart Energy Analog Front-End Controller Data Ready Interrupt
                DCD     MEM2MEM1_Handler          ; 78 Metrology Core Memory to Memory Transfer Controller 1 (MEM2MEM1)
                DCD     TC3_CHANNEL0_Handler      ; 79 Timer Counter 3, Channel 0
                DCD     TC3_CHANNEL1_Handler      ; 80 Timer Counter 3, Channel 1
                DCD     TC3_CHANNEL2_Handler      ; 81 Timer Counter 3, Channel 2
                DCD     TC3_C0SEC_Handler         ; 82 Timer Counter 3, Channel 0, Secure IRQ
                DCD     TC3_C1SEC_Handler         ; 83 Timer Counter 3, Channel 1, Secure IRQ
                DCD     TC3_C2SEC_Handler         ; 84 Timer Counter 3, Channel 2, Secure IRQ
                DCD     PIOD_Handler              ; 85 Metrology Core Parallel I/O Controller D (PIOD)
                DCD     PIOD_SEC_Handler          ; 86 Metrology Core Parallel I/O Controller A Secure Interupt (named as PIOD)
                DCD     UART_Handler              ; 87 Optical UART
                DCD     IPC1_Handler              ; 88 Metrology Core Interprocessor communication (IPC1)
                DCD     MCSPI_Handler             ; 89 Multi-Channel SPI
                DCD     PWM_Handler               ; 90 Pulse Width Modulation
                DCD     0                         ;    Reserved
                DCD     ARM1_FPU_Handler          ; 92 Metrology Core (CORE1) Floating Point Unit except IXC
                DCD     ARM1_IXC_Handler          ; 93 Metrology Core (CORE1) FPU Interrupt IXC associated with FPU cumulative exception bit
                DCD     MATRIX2_Handler           ; 94 High-Speed Metrology Core Matrix (HSMCM) (Formely MATRIX2)
                DCD     MATRIX3_Handler           ; 95 Low-Speed Metrology Core Matrix (LSMCM) (Formely MATRIX3)
                DCD     0                         ;    Reserved
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

                EXPORT  ACC_Handler               [WEAK]
                EXPORT  ADC_Handler               [WEAK]
                EXPORT  AES_Handler               [WEAK]
                EXPORT  AES_AESSEC_Handler        [WEAK]
                EXPORT  AESB_Handler              [WEAK]
                EXPORT  AESB_AESBSEC_Handler      [WEAK]
                EXPORT  ARM0_FPU_Handler          [WEAK]
                EXPORT  ARM0_IXC_Handler          [WEAK]
                EXPORT  ARM1_FPU_Handler          [WEAK]
                EXPORT  ARM1_IXC_Handler          [WEAK]
                EXPORT  CPKCC_Handler             [WEAK]
                EXPORT  DWDT_Handler              [WEAK]
                EXPORT  FLEXCOM0_Handler          [WEAK]
                EXPORT  FLEXCOM1_Handler          [WEAK]
                EXPORT  FLEXCOM2_Handler          [WEAK]
                EXPORT  FLEXCOM3_Handler          [WEAK]
                EXPORT  FLEXCOM4_Handler          [WEAK]
                EXPORT  FLEXCOM5_Handler          [WEAK]
                EXPORT  FLEXCOM6_Handler          [WEAK]
                EXPORT  FLEXCOM7_Handler          [WEAK]
                EXPORT  ICM_Handler               [WEAK]
                EXPORT  ICM_ICMSEC_Handler        [WEAK]
                EXPORT  IPC0_Handler              [WEAK]
                EXPORT  IPC1_Handler              [WEAK]
                EXPORT  MATRIX0_Handler           [WEAK]
                EXPORT  MATRIX1_Handler           [WEAK]
                EXPORT  MATRIX2_Handler           [WEAK]
                EXPORT  MATRIX3_Handler           [WEAK]
                EXPORT  MCSPI_Handler             [WEAK]
                EXPORT  MEM2MEM0_Handler          [WEAK]
                EXPORT  MEM2MEM1_Handler          [WEAK]
                EXPORT  PIOA_Handler              [WEAK]
                EXPORT  PIOA_SEC_Handler          [WEAK]
                EXPORT  PIOB_Handler              [WEAK]
                EXPORT  PIOB_SEC_Handler          [WEAK]
                EXPORT  PIOC_Handler              [WEAK]
                EXPORT  PIOC_SEC_Handler          [WEAK]
                EXPORT  PIOD_Handler              [WEAK]
                EXPORT  PIOD_SEC_Handler          [WEAK]
                EXPORT  PMC_Handler               [WEAK]
                EXPORT  PWM_Handler               [WEAK]
                EXPORT  QSPI_Handler              [WEAK]
                EXPORT  RSTC_Handler              [WEAK]
                EXPORT  RTC_Handler               [WEAK]
                EXPORT  RTT_Handler               [WEAK]
                EXPORT  SEAFEC_DATA_Handler       [WEAK]
                EXPORT  SEAFEC_SLINK_Handler      [WEAK]
                EXPORT  SEFC0_Handler             [WEAK]
                EXPORT  SEFC1_Handler             [WEAK]
                EXPORT  SHA_Handler               [WEAK]
                EXPORT  SHA_SHASEC_Handler        [WEAK]
                EXPORT  SLCDC_Handler             [WEAK]
                EXPORT  SUPC_Handler              [WEAK]
                EXPORT  SUPC_WKUP3_Handler        [WEAK]
                EXPORT  SUPC_WKUP4_Handler        [WEAK]
                EXPORT  SUPC_WKUP5_Handler        [WEAK]
                EXPORT  SUPC_WKUP6_Handler        [WEAK]
                EXPORT  SUPC_WKUP7_Handler        [WEAK]
                EXPORT  SUPC_WKUP8_Handler        [WEAK]
                EXPORT  SUPC_WKUP9_Handler        [WEAK]
                EXPORT  SUPC_WKUP10_Handler       [WEAK]
                EXPORT  SUPC_WKUP11_Handler       [WEAK]
                EXPORT  SUPC_WKUP12_Handler       [WEAK]
                EXPORT  SUPC_WKUP13_Handler       [WEAK]
                EXPORT  SUPC_WKUP14_Handler       [WEAK]
                EXPORT  SUPC_WKUP15_Handler       [WEAK]
                EXPORT  TC0_CHANNEL0_Handler      [WEAK]
                EXPORT  TC0_CHANNEL1_Handler      [WEAK]
                EXPORT  TC0_CHANNEL2_Handler      [WEAK]
                EXPORT  TC0_C0SEC_Handler         [WEAK]
                EXPORT  TC0_C1SEC_Handler         [WEAK]
                EXPORT  TC0_C2SEC_Handler         [WEAK]
                EXPORT  TC1_CHANNEL0_Handler      [WEAK]
                EXPORT  TC1_CHANNEL1_Handler      [WEAK]
                EXPORT  TC1_CHANNEL2_Handler      [WEAK]
                EXPORT  TC1_C0SEC_Handler         [WEAK]
                EXPORT  TC1_C1SEC_Handler         [WEAK]
                EXPORT  TC1_C2SEC_Handler         [WEAK]
                EXPORT  TC2_CHANNEL0_Handler      [WEAK]
                EXPORT  TC2_CHANNEL1_Handler      [WEAK]
                EXPORT  TC2_CHANNEL2_Handler      [WEAK]
                EXPORT  TC2_C0SEC_Handler         [WEAK]
                EXPORT  TC2_C1SEC_Handler         [WEAK]
                EXPORT  TC2_C2SEC_Handler         [WEAK]
                EXPORT  TC3_CHANNEL0_Handler      [WEAK]
                EXPORT  TC3_CHANNEL1_Handler      [WEAK]
                EXPORT  TC3_CHANNEL2_Handler      [WEAK]
                EXPORT  TC3_C0SEC_Handler         [WEAK]
                EXPORT  TC3_C1SEC_Handler         [WEAK]
                EXPORT  TC3_C2SEC_Handler         [WEAK]
                EXPORT  TRNG_Handler              [WEAK]
                EXPORT  TRNG_TRNGSEC_Handler      [WEAK]
                EXPORT  UART_Handler              [WEAK]

ACC_Handler
ADC_Handler
AES_Handler
AES_AESSEC_Handler
AESB_Handler
AESB_AESBSEC_Handler
ARM0_FPU_Handler
ARM0_IXC_Handler
ARM1_FPU_Handler
ARM1_IXC_Handler
CPKCC_Handler
DWDT_Handler
FLEXCOM0_Handler
FLEXCOM1_Handler
FLEXCOM2_Handler
FLEXCOM3_Handler
FLEXCOM4_Handler
FLEXCOM5_Handler
FLEXCOM6_Handler
FLEXCOM7_Handler
ICM_Handler
ICM_ICMSEC_Handler
IPC0_Handler
IPC1_Handler
MATRIX0_Handler
MATRIX1_Handler
MATRIX2_Handler
MATRIX3_Handler
MCSPI_Handler
MEM2MEM0_Handler
MEM2MEM1_Handler
PIOA_Handler
PIOA_SEC_Handler
PIOB_Handler
PIOB_SEC_Handler
PIOC_Handler
PIOC_SEC_Handler
PIOD_Handler
PIOD_SEC_Handler
PMC_Handler
PWM_Handler
QSPI_Handler
RSTC_Handler
RTC_Handler
RTT_Handler
SEAFEC_DATA_Handler
SEAFEC_SLINK_Handler
SEFC0_Handler
SEFC1_Handler
SHA_Handler
SHA_SHASEC_Handler
SLCDC_Handler
SUPC_Handler
SUPC_WKUP3_Handler
SUPC_WKUP4_Handler
SUPC_WKUP5_Handler
SUPC_WKUP6_Handler
SUPC_WKUP7_Handler
SUPC_WKUP8_Handler
SUPC_WKUP9_Handler
SUPC_WKUP10_Handler
SUPC_WKUP11_Handler
SUPC_WKUP12_Handler
SUPC_WKUP13_Handler
SUPC_WKUP14_Handler
SUPC_WKUP15_Handler
TC0_CHANNEL0_Handler
TC0_CHANNEL1_Handler
TC0_CHANNEL2_Handler
TC0_C0SEC_Handler
TC0_C1SEC_Handler
TC0_C2SEC_Handler
TC1_CHANNEL0_Handler
TC1_CHANNEL1_Handler
TC1_CHANNEL2_Handler
TC1_C0SEC_Handler
TC1_C1SEC_Handler
TC1_C2SEC_Handler
TC2_CHANNEL0_Handler
TC2_CHANNEL1_Handler
TC2_CHANNEL2_Handler
TC2_C0SEC_Handler
TC2_C1SEC_Handler
TC2_C2SEC_Handler
TC3_CHANNEL0_Handler
TC3_CHANNEL1_Handler
TC3_CHANNEL2_Handler
TC3_C0SEC_Handler
TC3_C1SEC_Handler
TC3_C2SEC_Handler
TRNG_Handler
TRNG_TRNGSEC_Handler
UART_Handler
                B       .
                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
