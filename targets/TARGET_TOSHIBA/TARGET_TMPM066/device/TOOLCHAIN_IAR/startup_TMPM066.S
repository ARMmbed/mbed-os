;/**
; *******************************************************************************
; * @file    startup_TMPM066.S
; * @brief   CMSIS Cortex-M0 Core Device Startup File for the
; *          TOSHIBA 'TMPM066' Device Series
; * @version V2.0.2.1 (Tentative)
; * @date    2015/08/05
; *
; * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
; *******************************************************************************
; */
;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
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

                DATA
__vector_table  DCD     sfe(CSTACK)
                DCD     Reset_Handler

                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     INT0_IRQHandler           ; 0:	Interrupt Pin0
                DCD     INT1_IRQHandler           ; 1:	Interrupt Pin1
                DCD     INT2_IRQHandler           ; 2:	Interrupt Pin2
                DCD     INT3_IRQHandler           ; 3:	Interrupt Pin3
                DCD     INT4_IRQHandler           ; 4:	Interrupt Pin4
                DCD     INT5_IRQHandler           ; 5:	Interrupt Pin5
                DCD     INTRX0_IRQHandler         ; 6:	Serial reception interrupt(channel0)
                DCD     INTTX0_IRQHandler         ; 7:	Serial transmission interrupt(channel0)
                DCD     INTRX1_IRQHandler         ; 8:	Serial reception interrupt(channel1)
                DCD     INTTX1_IRQHandler         ; 9:	Serial transmission interrupt(channel1)
                DCD     INTSPIRX_IRQHandler       ; 10:	SPI serial reception interrupt
                DCD     INTSPITX_IRQHandler       ; 11:	SPI serial transmission interrupt
                DCD     INTSPIERR_IRQHandler      ; 12:	SPI serial error interrupt
                DCD     INTI2C0_IRQHandler        ; 13:	Serial bus interface (channel.0)
                DCD     INTI2C1_IRQHandler        ; 14:	Serial bus interface (channel.1)
                DCD     INTDMA_IRQHandler         ; 15:	DMAC interrupt
                DCD     INT16A0_IRQHandler        ; 16:	16-bit TMR16A match detection (channel.0)
                DCD     INT16A1_IRQHandler        ; 17:	16-bit TMR16A match detection (channel.1)
                DCD     INTTB0_IRQHandler         ; 18:	16-bit TMRB interrupt(channel.0)
                DCD     INTTB1_IRQHandler         ; 19:	16-bit TMRB interrupt(channel.1)
                DCD     INTTB2_IRQHandler         ; 20:	16-bit TMRB interrupt(channel.2)
                DCD     INTTB3_IRQHandler         ; 21:	16-bit TMRB interrupt(channel.3)
                DCD     INTTB4_IRQHandler         ; 22:	16-bit TMRB interrupt(channel.4)
                DCD     INTTB5_IRQHandler         ; 23:	16-bit TMRB interrupt(channel.5)
                DCD     INTTB6_IRQHandler         ; 24:	16-bit TMRB interrupt(channel.6)
                DCD     INTTB7_IRQHandler         ; 25:	16-bit TMRB interrupt(channel.7)
                DCD     INTI2CS_IRQHandler        ; 26:	Serial bus interface for Wakeup(channel.1)
                DCD     INTTMRD_IRQHandler        ; 27:	TMRD interrupt
                DCD     INTUSB_IRQHandler         ; 28:	USB interrupt
                DCD     INTUSBWKUP_IRQHandler     ; 29:	USB wakeup interrupt
                DCD     INTADHP_IRQHandler        ; 30:	High Priority A/D conversion interrupt
                DCD     INTAD_IRQHandler          ; 31:	Normal A/D conversion completion interrupt
                THUMB
; Dummy Exception Handlers (infinite loops which can be modified)

                PUBWEAK Reset_Handler
                SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__iar_program_start
                BX      R0

                PUBWEAK NMI_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
                B       NMI_Handler

                PUBWEAK HardFault_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
                B       HardFault_Handler

                PUBWEAK SVC_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
                B       SVC_Handler

                PUBWEAK PendSV_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
                B       PendSV_Handler

                PUBWEAK SysTick_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
                B       SysTick_Handler

                PUBWEAK INT0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT0_IRQHandler
                B       INT0_IRQHandler

                PUBWEAK INT1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT1_IRQHandler
                B       INT1_IRQHandler

                PUBWEAK INT2_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT2_IRQHandler
                B       INT2_IRQHandler

                PUBWEAK INT3_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT3_IRQHandler
                B       INT3_IRQHandler

                PUBWEAK INT4_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT4_IRQHandler
                B       INT4_IRQHandler

                PUBWEAK INT5_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT5_IRQHandler
                B       INT5_IRQHandler

                PUBWEAK INTRX0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTRX0_IRQHandler
                B       INTRX0_IRQHandler

                PUBWEAK INTTX0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTX0_IRQHandler
                B       INTTX0_IRQHandler

                PUBWEAK INTRX1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTRX1_IRQHandler
                B       INTRX1_IRQHandler

                PUBWEAK INTTX1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTX1_IRQHandler
                B       INTTX1_IRQHandler

                PUBWEAK INTSPIRX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTSPIRX_IRQHandler
                B       INTSPIRX_IRQHandler

                PUBWEAK INTSPITX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTSPITX_IRQHandler
                B       INTSPITX_IRQHandler

                PUBWEAK INTSPIERR_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTSPIERR_IRQHandler
                B       INTSPIERR_IRQHandler

                PUBWEAK INTI2C0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2C0_IRQHandler
                B       INTI2C0_IRQHandler

                PUBWEAK INTI2C1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2C1_IRQHandler
                B       INTI2C1_IRQHandler

                PUBWEAK INTDMA_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTDMA_IRQHandler
                B       INTDMA_IRQHandler

                PUBWEAK INT16A0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT16A0_IRQHandler
                B       INT16A0_IRQHandler

                PUBWEAK INT16A1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT16A1_IRQHandler
                B       INT16A1_IRQHandler

                PUBWEAK INTTB0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTB0_IRQHandler
                B       INTTB0_IRQHandler

                PUBWEAK INTTB1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTB1_IRQHandler
                B       INTTB1_IRQHandler

                PUBWEAK INTTB2_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTB2_IRQHandler
                B       INTTB2_IRQHandler

                PUBWEAK INTTB3_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTB3_IRQHandler
                B       INTTB3_IRQHandler

                PUBWEAK INTTB4_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTB4_IRQHandler
                B       INTTB4_IRQHandler

                PUBWEAK INTTB5_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTB5_IRQHandler
                B       INTTB5_IRQHandler

                PUBWEAK INTTB6_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTB6_IRQHandler
                B       INTTB6_IRQHandler

                PUBWEAK INTTB7_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTB7_IRQHandler
                B       INTTB7_IRQHandler

                PUBWEAK INTI2CS_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2CS_IRQHandler
                B       INTI2CS_IRQHandler

                PUBWEAK INTTMRD_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTTMRD_IRQHandler
                B       INTTMRD_IRQHandler

                PUBWEAK INTUSB_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUSB_IRQHandler
                B       INTUSB_IRQHandler

                PUBWEAK INTUSBWKUP_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUSBWKUP_IRQHandler
                B       INTUSBWKUP_IRQHandler

                PUBWEAK INTADHP_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTADHP_IRQHandler
                B       INTADHP_IRQHandler

                PUBWEAK INTAD_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTAD_IRQHandler
                B       INTAD_IRQHandler

                END
