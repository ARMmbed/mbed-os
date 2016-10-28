;/* Copyright (c) 2012 ARM LIMITED
;
;   All rights reserved.
;   Redistribution and use in source and binary forms, with or without
;   modification, are permitted provided that the following conditions are met:
;   - Redistributions of source code must retain the above copyright
;     notice, this list of conditions and the following disclaimer.
;   - Redistributions in binary form must reproduce the above copyright
;     notice, this list of conditions and the following disclaimer in the
;     documentation and/or other materials provided with the distribution.
;   - Neither the name of ARM nor the names of its contributors may be used
;     to endorse or promote products derived from this software without
;     specific prior written permission.
;   *
;   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
;   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
;   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
;   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
;   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
;   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
;   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
;   POSSIBILITY OF SUCH DAMAGE.
;   ---------------------------------------------------------------------------*/


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



        MODULE  ?cstartup

        ;; Stack size default : Defined in *.icf (linker file). Can be modified inside EW.
        ;; Heap size default : Defined in *.icf (linker file). Can be modified inside EW.

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        EXTERN  nrf_reloc_vector_table
        PUBLIC  __vector_table
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler
        DCD     NMI_Handler
        DCD     HardFault_Handler
        DCD     MemoryManagement_Handler
        DCD     BusFault_Handler
        DCD     UsageFault_Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     SVC_Handler
        DCD     DebugMonitor_Handler
        DCD     0                         ; Reserved
        DCD     PendSV_Handler
        DCD     SysTick_Handler

        ; External Interrupts
        DCD     POWER_CLOCK_IRQHandler
        DCD     RADIO_IRQHandler
        DCD     UARTE0_UART0_IRQHandler_v
        DCD     SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler_v
        DCD     SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler_v
        DCD     NFCT_IRQHandler_v
        DCD     GPIOTE_IRQHandler_v
        DCD     SAADC_IRQHandler_v
        DCD     TIMER0_IRQHandler_v
        DCD     TIMER1_IRQHandler_v
        DCD     TIMER2_IRQHandler_v
        DCD     RTC0_IRQHandler
        DCD     TEMP_IRQHandler_v
        DCD     RNG_IRQHandler
        DCD     ECB_IRQHandler
        DCD     CCM_AAR_IRQHandler
        DCD     WDT_IRQHandler_v
        DCD     RTC1_IRQHandler_v
        DCD     QDEC_IRQHandler_v
        DCD     COMP_LPCOMP_IRQHandler_v
        DCD     SWI0_EGU0_IRQHandler_v
        DCD     SWI1_EGU1_IRQHandler_v
        DCD     SWI2_EGU2_IRQHandler_v
        DCD     SWI3_EGU3_IRQHandler_v
        DCD     SWI4_EGU4_IRQHandler
        DCD     SWI5_EGU5_IRQHandler
        DCD     TIMER3_IRQHandler_v
        DCD     TIMER4_IRQHandler_v
        DCD     PWM0_IRQHandler_v
        DCD     PDM_IRQHandler_v
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     MWU_IRQHandler
        DCD     PWM1_IRQHandler_v
        DCD     PWM2_IRQHandler_v
        DCD     SPIM2_SPIS2_SPI2_IRQHandler_v
        DCD     RTC2_IRQHandler_v
        DCD     I2S_IRQHandler_v
        DCD     FPU_IRQHandler_v


__Vectors_End
__Vectors                           EQU   __vector_table
__Vectors_Size                      EQU   __Vectors_End - __Vectors


; Default handlers.
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:NOROOT(2)
Reset_Handler

        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =nrf_reloc_vector_table
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

        PUBWEAK MemoryManagement_Handler
        SECTION .text:CODE:NOROOT(1)
MemoryManagement_Handler
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

        PUBWEAK DebugMonitor_Handler
        SECTION .text:CODE:NOROOT(1)
DebugMonitor_Handler
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

        PUBWEAK  POWER_CLOCK_IRQHandler
        SECTION .text:CODE:NOROOT(1)
POWER_CLOCK_IRQHandler
        B .

        PUBWEAK  RADIO_IRQHandler
        SECTION .text:CODE:NOROOT(1)
RADIO_IRQHandler
        B .

        PUBWEAK  UARTE0_UART0_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
UARTE0_UART0_IRQHandler_v
        B .

        PUBWEAK  SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler_v
        B .

        PUBWEAK  SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler_v
        B .

        PUBWEAK  NFCT_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
NFCT_IRQHandler_v
        B .

        PUBWEAK  GPIOTE_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
GPIOTE_IRQHandler_v
        B .

        PUBWEAK  SAADC_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
SAADC_IRQHandler_v
        B .

        PUBWEAK  TIMER0_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
TIMER0_IRQHandler_v
        B .

        PUBWEAK  TIMER1_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
TIMER1_IRQHandler_v
        B .

        PUBWEAK  TIMER2_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
TIMER2_IRQHandler_v
        B .

        PUBWEAK  RTC0_IRQHandler
        SECTION .text:CODE:NOROOT(1)
RTC0_IRQHandler
        B .

        PUBWEAK  TEMP_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
TEMP_IRQHandler_v
        B .

        PUBWEAK  RNG_IRQHandler
        SECTION .text:CODE:NOROOT(1)
RNG_IRQHandler
        B .

        PUBWEAK  ECB_IRQHandler
        SECTION .text:CODE:NOROOT(1)
ECB_IRQHandler
        B .

        PUBWEAK  CCM_AAR_IRQHandler
        SECTION .text:CODE:NOROOT(1)
CCM_AAR_IRQHandler
        B .

        PUBWEAK  WDT_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
WDT_IRQHandler_v
        B .

        PUBWEAK  RTC1_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
RTC1_IRQHandler_v
        B .

        PUBWEAK  QDEC_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
QDEC_IRQHandler_v
        B .

        PUBWEAK  COMP_LPCOMP_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
COMP_LPCOMP_IRQHandler_v
        B .

        PUBWEAK  SWI0_EGU0_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
SWI0_EGU0_IRQHandler_v
        B .

        PUBWEAK  SWI1_EGU1_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
SWI1_EGU1_IRQHandler_v
        B .

        PUBWEAK  SWI2_EGU2_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
SWI2_EGU2_IRQHandler_v
        B .

        PUBWEAK  SWI3_EGU3_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
SWI3_EGU3_IRQHandler_v
        B .

        PUBWEAK  SWI4_EGU4_IRQHandler
        SECTION .text:CODE:NOROOT(1)
SWI4_EGU4_IRQHandler
        B .

        PUBWEAK  SWI5_EGU5_IRQHandler
        SECTION .text:CODE:NOROOT(1)
SWI5_EGU5_IRQHandler
        B .

        PUBWEAK  TIMER3_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
TIMER3_IRQHandler_v
        B .

        PUBWEAK  TIMER4_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
TIMER4_IRQHandler_v
        B .

        PUBWEAK  PWM0_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
PWM0_IRQHandler_v
        B .

        PUBWEAK  PDM_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
PDM_IRQHandler_v
        B .

        PUBWEAK  MWU_IRQHandler
        SECTION .text:CODE:NOROOT(1)
MWU_IRQHandler
        B .

        PUBWEAK  PWM1_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
PWM1_IRQHandler_v
        B .

        PUBWEAK  PWM2_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
PWM2_IRQHandler_v
        B .

        PUBWEAK  SPIM2_SPIS2_SPI2_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
SPIM2_SPIS2_SPI2_IRQHandler_v
        B .

        PUBWEAK  RTC2_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
RTC2_IRQHandler_v
        B .

        PUBWEAK  I2S_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
I2S_IRQHandler_v
        B .

        PUBWEAK  FPU_IRQHandler_v
        SECTION .text:CODE:NOROOT(1)
FPU_IRQHandler_v
        B .


        END


