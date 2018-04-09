;*******************************************************************************
;* Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
;*
;* Permission is hereby granted, free of charge, to any person obtaining a
;* copy of this software and associated documentation files (the "Software"),
;* to deal in the Software without restriction, including without limitation
;* the rights to use, copy, modify, merge, publish, distribute, sublicense,
;* and/or sell copies of the Software, and to permit persons to whom the
;* Software is furnished to do so, subject to the following conditions:
;*
;* The above copyright notice and this permission notice shall be included
;* in all copies or substantial portions of the Software.
;*
;* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
;* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
;* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
;* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
;* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
;* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
;* OTHER DEALINGS IN THE SOFTWARE.
;*
;* Except as contained in this notice, the name of Maxim Integrated
;* Products, Inc. shall not be used except as stated in the Maxim Integrated
;* Products, Inc. Branding Policy.
;*
;* The mere transfer of this software does not imply any licenses
;* of trade secrets, proprietary technology, copyrights, patents,
;* trademarks, maskwork rights, or any other form of intellectual
;* property whatsoever. Maxim Integrated Products, Inc. retains all
;* ownership rights.
;*
;* Description        : MAX32620 device vector table for IAR EWARM toolchain.
;*                      - Sets the initial SP
;*                      - Sets the initial PC == _iar_program_start,
;*                      - Set the vector table entries with the exceptions ISR
;*                        address, all set as PUBWEAK. User may override any ISR
;*                        defined as PUBWEAK.
;*                      - Branches to main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the Cortex-M4 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;*******************************************************************************
    MODULE  ?cstartup

    ;; Forward declaration of sections.
    SECTION CSTACK:DATA:NOROOT(3)

    SECTION .intvec:CODE:NOROOT(2)

    EXTERN  __iar_program_start
    EXTERN  PreInit
    EXTERN  SystemInit
    PUBLIC  __vector_table
    PUBLIC  __vector_table_modify
    PUBLIC  __Vectors
    PUBLIC  __Vectors_End
    PUBLIC  __Vectors_Size

    DATA
__vector_table
    DCD     sfe(CSTACK)
    DCD     Reset_Handler               ; Reset Handler

    DCD     NMI_Handler                 ; NMI Handler
    DCD     HardFault_Handler           ; Hard Fault Handler
    DCD     MemManage_Handler           ; MPU Fault Handler
    DCD     BusFault_Handler            ; Bus Fault Handler
    DCD     UsageFault_Handler          ; Usage Fault Handler
__vector_table_modify
    DCD     0                           ; Reserved
    DCD     0                           ; Reserved
    DCD     0                           ; Reserved
    DCD     0                           ; Reserved
    DCD     SVC_Handler                 ; SVCall Handler
    DCD     DebugMon_Handler            ; Debug Monitor Handler
    DCD     0                           ; Reserved
    DCD     PendSV_Handler              ; PendSV Handler
    DCD     SysTick_Handler             ; SysTick Handler
    ; MAX32620C Specific Interrupts
    DCD     CLKMAN_IRQHandler           ; 16:00 CLKMAN */
    DCD     PWRMAN_IRQHandler           ; 17:01 PWRMAN */
    DCD     FLC_IRQHandler              ; 18:02 Flash Controller */
    DCD     RTC0_IRQHandler             ; 19:03 RTC INT0 */
    DCD     RTC1_IRQHandler             ; 20:04 RTC INT1 */
    DCD     RTC2_IRQHandler             ; 21:05 RTC INT2 */
    DCD     RTC3_IRQHandler             ; 22:06 RTC INT3 */
    DCD     PMU_IRQHandler              ; 23:07 PMU */
    DCD     USB_IRQHandler              ; 24:08 USB */
    DCD     AES_IRQHandler              ; 25:09 AES */
    DCD     MAA_IRQHandler              ; 26:10 MAA */
    DCD     WDT0_IRQHandler             ; 27:11 WATCHDOG0 */
    DCD     WDT0_P_IRQHandler           ; 28:12 WATCHDOG0 PRE-WINDOW */
    DCD     WDT1_IRQHandler             ; 29:13 WATCHDOG1 */
    DCD     WDT1_P_IRQHandler           ; 30:14 WATCHDOG1 PRE-WINDOW */
    DCD     GPIO_P0_IRQHandler          ; 31:15 GPIO Port 0  */
    DCD     GPIO_P1_IRQHandler          ; 32:16 GPIO Port 1  */
    DCD     GPIO_P2_IRQHandler          ; 33:17 GPIO Port 2  */
    DCD     GPIO_P3_IRQHandler          ; 34:18 GPIO Port 3  */
    DCD     GPIO_P4_IRQHandler          ; 35:19 GPIO Port 4  */
    DCD     GPIO_P5_IRQHandler          ; 36:20 GPIO Port 5  */
    DCD     GPIO_P6_IRQHandler          ; 37:21 GPIO Port 6  */
    DCD     TMR0_IRQHandler             ; 38:22 Timer32-0 */
    DCD     TMR16_0_IRQHandler          ; 39:23 Timer16-s0 */
    DCD     TMR1_IRQHandler             ; 40:24 Timer32-1 */
    DCD     TMR16_1_IRQHandler          ; 41:25 Timer16-s1 */
    DCD     TMR2_IRQHandler             ; 42:26 Timer32-2 */
    DCD     TMR16_2_IRQHandler          ; 43:27 Timer16-s2 */
    DCD     TMR3_IRQHandler             ; 44:28 Timer32-3 */
    DCD     TMR16_3_IRQHandler          ; 45:29 Timer16-s3 */
    DCD     TMR4_IRQHandler             ; 46:30 Timer32-4 */
    DCD     TMR16_4_IRQHandler          ; 47:31 Timer16-s4 */
    DCD     TMR5_IRQHandler             ; 48:32 Timer32-5 */
    DCD     TMR16_5_IRQHandler          ; 49:33 Timer16-s5 */
    DCD     UART0_IRQHandler            ; 50:34 UART0 */
    DCD     UART1_IRQHandler            ; 51:35 UART1 */
    DCD     UART2_IRQHandler            ; 52:36 UART2 */
    DCD     UART3_IRQHandler            ; 53:37 UART3 */
    DCD     PT_IRQHandler               ; 54:38 PT */
    DCD     I2CM0_IRQHandler            ; 55:39 I2C Master 0 */
    DCD     I2CM1_IRQHandler            ; 56:40 I2C Master 1 */
    DCD     I2CM2_IRQHandler            ; 57:41 I2C Master 2 */
    DCD     I2CS_IRQHandler             ; 58:42 I2C Slave */
    DCD     SPIM0_IRQHandler            ; 59:43 SPI Master 0 */
    DCD     SPIM1_IRQHandler            ; 60:44 SPI Master 1 */
    DCD     SPIM2_IRQHandler            ; 61:45 SPI Master 2 */
    DCD     SPIB_IRQHandler             ; 62:46 SPI Bridge */
    DCD     OWM_IRQHandler              ; 63:47 One-wire Master */
    DCD     AFE_IRQHandler              ; 64:48 AFE */
    DCD     SPIS_IRQHandler             ; 65:49 SPI Slave

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

        LDR        R0, =PreInit
        BLX        R0
        LDR        R0, =SystemInit
        BLX        R0
        LDR        R0, =__iar_program_start
        BX         R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B HardFault_Handler

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

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DebugMon_Handler
        B DebugMon_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B SysTick_Handler

        PUBWEAK CLKMAN_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
CLKMAN_IRQHandler
        B       CLKMAN_IRQHandler

        PUBWEAK PWRMAN_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWRMAN_IRQHandler
        B       PWRMAN_IRQHandler

        PUBWEAK FLC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
FLC_IRQHandler
        B       FLC_IRQHandler

        PUBWEAK RTC0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RTC0_IRQHandler
        B       RTC0_IRQHandler

        PUBWEAK RTC1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RTC1_IRQHandler
        B       RTC1_IRQHandler

        PUBWEAK RTC2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RTC2_IRQHandler
        B       RTC2_IRQHandler

        PUBWEAK RTC3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RTC3_IRQHandler
        B       RTC3_IRQHandler

        PUBWEAK PMU_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PMU_IRQHandler
        B       PMU_IRQHandler

        PUBWEAK USB_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
USB_IRQHandler
        B       USB_IRQHandler

        PUBWEAK AES_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
AES_IRQHandler
        B       AES_IRQHandler

        PUBWEAK MAA_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
MAA_IRQHandler
        B       MAA_IRQHandler

        PUBWEAK WDT0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
WDT0_IRQHandler
        B       WDT0_IRQHandler

        PUBWEAK WDT0_P_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
WDT0_P_IRQHandler
        B       WDT0_P_IRQHandler

        PUBWEAK WDT1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
WDT1_IRQHandler
        B       WDT1_IRQHandler

        PUBWEAK WDT1_P_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
WDT1_P_IRQHandler
        B       WDT1_P_IRQHandler

        PUBWEAK GPIO_P0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIO_P0_IRQHandler
        B       GPIO_P0_IRQHandler

        PUBWEAK GPIO_P1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIO_P1_IRQHandler
        B       GPIO_P1_IRQHandler

        PUBWEAK GPIO_P2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIO_P2_IRQHandler
        B       GPIO_P2_IRQHandler

        PUBWEAK GPIO_P3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIO_P3_IRQHandler
        B       GPIO_P3_IRQHandler

        PUBWEAK GPIO_P4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIO_P4_IRQHandler
        B       GPIO_P4_IRQHandler

        PUBWEAK GPIO_P5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIO_P5_IRQHandler
        B       GPIO_P5_IRQHandler

        PUBWEAK GPIO_P6_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIO_P6_IRQHandler
        B       GPIO_P6_IRQHandler

        PUBWEAK TMR0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TMR0_IRQHandler
        B       TMR0_IRQHandler

        PUBWEAK TMR16_0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TMR16_0_IRQHandler
        B       TMR16_0_IRQHandler

        PUBWEAK TMR1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TMR1_IRQHandler
        B       TMR1_IRQHandler

        PUBWEAK TMR16_1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TMR16_1_IRQHandler
        B       TMR16_1_IRQHandler

        PUBWEAK TMR2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TMR2_IRQHandler
        B       TMR2_IRQHandler

        PUBWEAK TMR16_2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TMR16_2_IRQHandler
        B       TMR16_2_IRQHandler

        PUBWEAK TMR3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TMR3_IRQHandler
        B       TMR3_IRQHandler

        PUBWEAK TMR16_3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TMR16_3_IRQHandler
        B       TMR16_3_IRQHandler

        PUBWEAK TMR4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TMR4_IRQHandler
        B       TMR4_IRQHandler

        PUBWEAK TMR16_4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TMR16_4_IRQHandler
        B       TMR16_4_IRQHandler

        PUBWEAK TMR5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TMR5_IRQHandler
        B       TMR5_IRQHandler

        PUBWEAK TMR16_5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TMR16_5_IRQHandler
        B       TMR16_5_IRQHandler

        PUBWEAK UART0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART0_IRQHandler
        B       UART0_IRQHandler

        PUBWEAK UART1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART1_IRQHandler
        B       UART1_IRQHandler

        PUBWEAK UART2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART2_IRQHandler
        B       UART2_IRQHandler

        PUBWEAK UART3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART3_IRQHandler
        B       UART3_IRQHandler

        PUBWEAK PT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PT_IRQHandler
        B       PT_IRQHandler

        PUBWEAK I2CM0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2CM0_IRQHandler
        B       I2CM0_IRQHandler

        PUBWEAK I2CM1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2CM1_IRQHandler
        B       I2CM1_IRQHandler

        PUBWEAK I2CM2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2CM2_IRQHandler
        B       I2CM2_IRQHandler

        PUBWEAK I2CS_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2CS_IRQHandler
        B       I2CS_IRQHandler

        PUBWEAK SPIM0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPIM0_IRQHandler
        B       SPIM0_IRQHandler

        PUBWEAK SPIM1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPIM1_IRQHandler
        B       SPIM1_IRQHandler

        PUBWEAK SPIM2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPIM2_IRQHandler
        B       SPIM2_IRQHandler

        PUBWEAK SPIB_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPIB_IRQHandler
        B       SPIB_IRQHandler

        PUBWEAK OWM_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
OWM_IRQHandler
        B       OWM_IRQHandler

        PUBWEAK AFE_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
AFE_IRQHandler
        B       AFE_IRQHandler

        PUBWEAK SPIS_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPIS_IRQHandler
        B       SPIS_IRQHandler
        END
