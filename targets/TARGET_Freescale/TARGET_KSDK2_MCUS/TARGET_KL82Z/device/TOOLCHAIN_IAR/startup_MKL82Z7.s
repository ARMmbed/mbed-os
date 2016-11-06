; ---------------------------------------------------------------------------------------
;  @file:    startup_MKL82Z7.s
;  @purpose: CMSIS Cortex-M0P Core Device Startup File
;            MKL82Z7
;  @version: 1.5
;  @date:    2015-9-24
;  @build:   b151217
; ---------------------------------------------------------------------------------------
;
; Copyright (c) 1997 - 2015 , Freescale Semiconductor, Inc.
; All rights reserved.
;
; Redistribution and use in source and binary forms, with or without modification,
; are permitted provided that the following conditions are met:
;
; o Redistributions of source code must retain the above copyright notice, this list
;   of conditions and the following disclaimer.
;
; o Redistributions in binary form must reproduce the above copyright notice, this
;   list of conditions and the following disclaimer in the documentation and/or
;   other materials provided with the distribution.
;
; o Neither the name of Freescale Semiconductor, Inc. nor the names of its
;   contributors may be used to endorse or promote products derived from this
;   software without specific prior written permission.
;
; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
; ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
; WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
; DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
; ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
; (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
; ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
; (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
; SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler

        DCD     NMI_Handler                                   ;NMI Handler
        DCD     HardFault_Handler                             ;Hard Fault Handler
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
__vector_table_0x1c
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     SVC_Handler                                   ;SVCall Handler
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     PendSV_Handler                                ;PendSV Handler
        DCD     SysTick_Handler                               ;SysTick Handler

                                                              ;External Interrupts
        DCD     DMA0_DMA4_IRQHandler                          ;DMA channel 0, 4 transfer complete
        DCD     DMA1_DMA5_IRQHandler                          ;DMA channel 1, 5 transfer complete
        DCD     DMA2_DMA6_IRQHandler                          ;DMA channel 2, 6 transfer complete
        DCD     DMA3_DMA7_IRQHandler                          ;DMA channel 3, 7 transfer complete
        DCD     DMA_Error_IRQHandler                          ;DMA channel 0 - 7 error
        DCD     FLEXIO0_IRQHandler                            ;Flexible IO
        DCD     TPM0_IRQHandler                               ;Timer/PWM module 0
        DCD     TPM1_IRQHandler                               ;Timer/PWM module 1
        DCD     TPM2_IRQHandler                               ;Timer/PWM module 2
        DCD     PIT0_IRQHandler                               ;Periodic Interrupt Timer 0
        DCD     SPI0_IRQHandler                               ;Serial Peripheral Interface 0
        DCD     EMVSIM0_IRQHandler                            ;EMVSIM0 common interrupt
        DCD     LPUART0_IRQHandler                            ;LPUART0 status and error
        DCD     LPUART1_IRQHandler                            ;LPUART1 status and error
        DCD     I2C0_IRQHandler                               ;Inter-Integrated Circuit 0
        DCD     QSPI0_IRQHandler                              ;QuadSPI0 interrupt
        DCD     Reserved32_IRQHandler                         ;DryIce tamper detect
        DCD     PORTA_IRQHandler                              ;Pin detect Port A
        DCD     PORTB_IRQHandler                              ;Pin detect Port B
        DCD     PORTC_IRQHandler                              ;Pin detect Port C
        DCD     PORTD_IRQHandler                              ;Pin detect Port D
        DCD     PORTE_IRQHandler                              ;Pin detect Port E
        DCD     LLWU_IRQHandler                               ;Low Leakage Wakeup
        DCD     LTC0_IRQHandler                               ;Low power trusted cryptographic
        DCD     USB0_IRQHandler                               ;USB OTG interrupt
        DCD     ADC0_IRQHandler                               ;Analog-to-Digital Converter 0
        DCD     LPTMR0_IRQHandler                             ;Low-Power Timer 0
        DCD     RTC_Seconds_IRQHandler                        ;RTC seconds
        DCD     INTMUX0_0_IRQHandler                          ;Selectable peripheral interrupt INTMUX0-0
        DCD     INTMUX0_1_IRQHandler                          ;Selectable peripheral interrupt INTMUX0-1
        DCD     INTMUX0_2_IRQHandler                          ;Selectable peripheral interrupt INTMUX0-2
        DCD     INTMUX0_3_IRQHandler                          ;Selectable peripheral interrupt INTMUX0-3
        DCD     LPTMR1_IRQHandler                             ;Low-Power Timer 1 (INTMUX source IRQ0)
        DCD     Reserved49_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ1)
        DCD     Reserved50_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ2)
        DCD     Reserved51_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ3)
        DCD     SPI1_IRQHandler                               ;Serial Peripheral Interface 1 (INTMUX source IRQ4)
        DCD     LPUART2_IRQHandler                            ;LPUART2 status and error (INTMUX source IRQ5)
        DCD     EMVSIM1_IRQHandler                            ;EMVSIM1 common interrupt (INTMUX source IRQ6)
        DCD     I2C1_IRQHandler                               ;Inter-Integrated Circuit 1 (INTMUX source IRQ7)
        DCD     TSI0_IRQHandler                               ;Touch Sensing Input 0 (INTMUX source IRQ8)
        DCD     PMC_IRQHandler                                ;PMC controller low-voltage detect, low-voltage warning (INTMUX source IRQ9)
        DCD     FTFA_IRQHandler                               ;FTFA command complete/read collision (INTMUX source IRQ10)
        DCD     MCG_IRQHandler                                ;Multipurpose clock generator (INTMUX source IRQ11)
        DCD     WDOG_EWM_IRQHandler                           ;Single interrupt vector for  WDOG and EWM (INTMUX source IRQ12)
        DCD     DAC0_IRQHandler                               ;Digital-to-analog converter 0 (INTMUX source IRQ13)
        DCD     TRNG0_IRQHandler                              ;True randon number generator (INTMUX source IRQ14)
        DCD     Reserved63_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ15)
        DCD     CMP0_IRQHandler                               ;Comparator 0 (INTMUX source IRQ16)
        DCD     Reserved65_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ17)
        DCD     RTC_Alarm_IRQHandler                          ;Real time clock (INTMUX source IRQ18)
        DCD     Reserved67_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ19)
        DCD     Reserved68_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ20)
        DCD     Reserved69_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ21)
        DCD     Reserved70_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ22)
        DCD     Reserved71_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ23)
        DCD     DMA4_IRQHandler                               ;DMA channel 4 transfer complete (INTMUX source IRQ24)
        DCD     DMA5_IRQHandler                               ;DMA channel 5 transfer complete (INTMUX source IRQ25)
        DCD     DMA6_IRQHandler                               ;DMA channel 6 transfer complete (INTMUX source IRQ26)
        DCD     DMA7_IRQHandler                               ;DMA channel 7 transfer complete (INTMUX source IRQ27)
        DCD     Reserved76_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ28)
        DCD     Reserved77_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ29)
        DCD     Reserved78_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ30)
        DCD     Reserved79_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ31)
__Vectors_End

        SECTION FlashConfig:CODE
__FlashConfig
        DCD 0xFFFFFFFF
        DCD 0xFFFFFFFF
        DCD 0xFFFFFFFF
        DCD 0xFFFF3DFE
__FlashConfig_End

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
        LDR     R0, =0xE000ED08
        LDR     R1, =__vector_table
        STR     R1, [R0]
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

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B .

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B .

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B .

        PUBWEAK DMA0_DMA4_IRQHandler
        PUBWEAK DMA0_DMA4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_DMA4_IRQHandler
        LDR     R0, =DMA0_DMA4_DriverIRQHandler
        BX      R0

        PUBWEAK DMA1_DMA5_IRQHandler
        PUBWEAK DMA1_DMA5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA1_DMA5_IRQHandler
        LDR     R0, =DMA1_DMA5_DriverIRQHandler
        BX      R0

        PUBWEAK DMA2_DMA6_IRQHandler
        PUBWEAK DMA2_DMA6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA2_DMA6_IRQHandler
        LDR     R0, =DMA2_DMA6_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_DMA7_IRQHandler
        PUBWEAK DMA3_DMA7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_DMA7_IRQHandler
        LDR     R0, =DMA3_DMA7_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_Error_IRQHandler
        PUBWEAK DMA_Error_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_Error_IRQHandler
        LDR     R0, =DMA_Error_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXIO0_IRQHandler
        PUBWEAK FLEXIO0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXIO0_IRQHandler
        LDR     R0, =FLEXIO0_DriverIRQHandler
        BX      R0

        PUBWEAK TPM0_IRQHandler
        PUBWEAK TPM1_IRQHandler
        PUBWEAK TPM2_IRQHandler
        PUBWEAK PIT0_IRQHandler
        PUBWEAK SPI0_IRQHandler
        PUBWEAK SPI0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SPI0_IRQHandler
        LDR     R0, =SPI0_DriverIRQHandler
        BX      R0

        PUBWEAK EMVSIM0_IRQHandler
        PUBWEAK LPUART0_IRQHandler
        PUBWEAK LPUART0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART0_IRQHandler
        LDR     R0, =LPUART0_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART1_IRQHandler
        PUBWEAK LPUART1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART1_IRQHandler
        LDR     R0, =LPUART1_DriverIRQHandler
        BX      R0

        PUBWEAK I2C0_IRQHandler
        PUBWEAK I2C0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2C0_IRQHandler
        LDR     R0, =I2C0_DriverIRQHandler
        BX      R0

        PUBWEAK QSPI0_IRQHandler
        PUBWEAK QSPI0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
QSPI0_IRQHandler
        LDR     R0, =QSPI0_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved32_IRQHandler
        PUBWEAK PORTA_IRQHandler
        PUBWEAK PORTB_IRQHandler
        PUBWEAK PORTC_IRQHandler
        PUBWEAK PORTD_IRQHandler
        PUBWEAK PORTE_IRQHandler
        PUBWEAK LLWU_IRQHandler
        PUBWEAK LTC0_IRQHandler
        PUBWEAK USB0_IRQHandler
        PUBWEAK ADC0_IRQHandler
        PUBWEAK LPTMR0_IRQHandler
        PUBWEAK RTC_Seconds_IRQHandler
        PUBWEAK INTMUX0_0_IRQHandler
        PUBWEAK INTMUX0_0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
INTMUX0_0_IRQHandler
        LDR     R0, =INTMUX0_0_DriverIRQHandler
        BX      R0

        PUBWEAK INTMUX0_1_IRQHandler
        PUBWEAK INTMUX0_1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
INTMUX0_1_IRQHandler
        LDR     R0, =INTMUX0_1_DriverIRQHandler
        BX      R0

        PUBWEAK INTMUX0_2_IRQHandler
        PUBWEAK INTMUX0_2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
INTMUX0_2_IRQHandler
        LDR     R0, =INTMUX0_2_DriverIRQHandler
        BX      R0

        PUBWEAK INTMUX0_3_IRQHandler
        PUBWEAK INTMUX0_3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
INTMUX0_3_IRQHandler
        LDR     R0, =INTMUX0_3_DriverIRQHandler
        BX      R0

        PUBWEAK LPTMR1_IRQHandler
        PUBWEAK Reserved49_IRQHandler
        PUBWEAK Reserved50_IRQHandler
        PUBWEAK Reserved51_IRQHandler
        PUBWEAK SPI1_IRQHandler
        PUBWEAK SPI1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SPI1_IRQHandler
        LDR     R0, =SPI1_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART2_IRQHandler
        PUBWEAK LPUART2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART2_IRQHandler
        LDR     R0, =LPUART2_DriverIRQHandler
        BX      R0

        PUBWEAK EMVSIM1_IRQHandler
        PUBWEAK I2C1_IRQHandler
        PUBWEAK I2C1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2C1_IRQHandler
        LDR     R0, =I2C1_DriverIRQHandler
        BX      R0

        PUBWEAK TSI0_IRQHandler
        PUBWEAK PMC_IRQHandler
        PUBWEAK FTFA_IRQHandler
        PUBWEAK MCG_IRQHandler
        PUBWEAK WDOG_EWM_IRQHandler
        PUBWEAK DAC0_IRQHandler
        PUBWEAK TRNG0_IRQHandler
        PUBWEAK Reserved63_IRQHandler
        PUBWEAK CMP0_IRQHandler
        PUBWEAK Reserved65_IRQHandler
        PUBWEAK RTC_Alarm_IRQHandler
        PUBWEAK Reserved67_IRQHandler
        PUBWEAK Reserved68_IRQHandler
        PUBWEAK Reserved69_IRQHandler
        PUBWEAK Reserved70_IRQHandler
        PUBWEAK Reserved71_IRQHandler
        PUBWEAK DMA4_IRQHandler
        PUBWEAK DMA4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_IRQHandler
        LDR     R0, =DMA4_DriverIRQHandler
        BX      R0

        PUBWEAK DMA5_IRQHandler
        PUBWEAK DMA5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA5_IRQHandler
        LDR     R0, =DMA5_DriverIRQHandler
        BX      R0

        PUBWEAK DMA6_IRQHandler
        PUBWEAK DMA6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA6_IRQHandler
        LDR     R0, =DMA6_DriverIRQHandler
        BX      R0

        PUBWEAK DMA7_IRQHandler
        PUBWEAK DMA7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA7_IRQHandler
        LDR     R0, =DMA7_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved76_IRQHandler
        PUBWEAK Reserved77_IRQHandler
        PUBWEAK Reserved78_IRQHandler
        PUBWEAK Reserved79_IRQHandler
        PUBWEAK DefaultISR
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_DMA4_DriverIRQHandler
DMA1_DMA5_DriverIRQHandler
DMA2_DMA6_DriverIRQHandler
DMA3_DMA7_DriverIRQHandler
DMA_Error_DriverIRQHandler
FLEXIO0_DriverIRQHandler
TPM0_IRQHandler
TPM1_IRQHandler
TPM2_IRQHandler
PIT0_IRQHandler
SPI0_DriverIRQHandler
EMVSIM0_IRQHandler
LPUART0_DriverIRQHandler
LPUART1_DriverIRQHandler
I2C0_DriverIRQHandler
QSPI0_DriverIRQHandler
Reserved32_IRQHandler
PORTA_IRQHandler
PORTB_IRQHandler
PORTC_IRQHandler
PORTD_IRQHandler
PORTE_IRQHandler
LLWU_IRQHandler
LTC0_IRQHandler
USB0_IRQHandler
ADC0_IRQHandler
LPTMR0_IRQHandler
RTC_Seconds_IRQHandler
INTMUX0_0_DriverIRQHandler
INTMUX0_1_DriverIRQHandler
INTMUX0_2_DriverIRQHandler
INTMUX0_3_DriverIRQHandler
LPTMR1_IRQHandler
Reserved49_IRQHandler
Reserved50_IRQHandler
Reserved51_IRQHandler
SPI1_DriverIRQHandler
LPUART2_DriverIRQHandler
EMVSIM1_IRQHandler
I2C1_DriverIRQHandler
TSI0_IRQHandler
PMC_IRQHandler
FTFA_IRQHandler
MCG_IRQHandler
WDOG_EWM_IRQHandler
DAC0_IRQHandler
TRNG0_IRQHandler
Reserved63_IRQHandler
CMP0_IRQHandler
Reserved65_IRQHandler
RTC_Alarm_IRQHandler
Reserved67_IRQHandler
Reserved68_IRQHandler
Reserved69_IRQHandler
Reserved70_IRQHandler
Reserved71_IRQHandler
DMA4_DriverIRQHandler
DMA5_DriverIRQHandler
DMA6_DriverIRQHandler
DMA7_DriverIRQHandler
Reserved76_IRQHandler
Reserved77_IRQHandler
Reserved78_IRQHandler
Reserved79_IRQHandler
DefaultISR
        LDR R0, =DefaultISR
        BX R0

        END
