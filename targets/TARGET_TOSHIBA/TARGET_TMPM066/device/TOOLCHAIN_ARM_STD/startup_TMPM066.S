;/**
; *******************************************************************************
; * @file    startup_TMPM066.S
; * @brief   CMSIS Cortex-M0 Core Device Startup File for the
; *          TOSHIBA 'TMPM066' Device Series
; * @version V2.0.2.1 (Tentative)
; * @date    2015/08/05
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; *
; * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
; *******************************************************************************
; */

__initial_sp    EQU     0x20004000 ; Top of RAM (16KB)

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
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

                EXPORT  INT0_IRQHandler           [WEAK]
                EXPORT  INT1_IRQHandler           [WEAK]
                EXPORT  INT2_IRQHandler           [WEAK]
                EXPORT  INT3_IRQHandler           [WEAK]
                EXPORT  INT4_IRQHandler           [WEAK]
                EXPORT  INT5_IRQHandler           [WEAK]
                EXPORT  INTRX0_IRQHandler         [WEAK]
                EXPORT  INTTX0_IRQHandler         [WEAK]
                EXPORT  INTRX1_IRQHandler         [WEAK]
                EXPORT  INTTX1_IRQHandler         [WEAK]
                EXPORT  INTSPIRX_IRQHandler       [WEAK]
                EXPORT  INTSPITX_IRQHandler       [WEAK]
                EXPORT  INTSPIERR_IRQHandler      [WEAK]
                EXPORT  INTI2C0_IRQHandler        [WEAK]
                EXPORT  INTI2C1_IRQHandler        [WEAK]
                EXPORT  INTDMA_IRQHandler         [WEAK]
                EXPORT  INT16A0_IRQHandler        [WEAK]
                EXPORT  INT16A1_IRQHandler        [WEAK]
                EXPORT  INTTB0_IRQHandler         [WEAK]
                EXPORT  INTTB1_IRQHandler         [WEAK]
                EXPORT  INTTB2_IRQHandler         [WEAK]
                EXPORT  INTTB3_IRQHandler         [WEAK]
                EXPORT  INTTB4_IRQHandler         [WEAK]
                EXPORT  INTTB5_IRQHandler         [WEAK]
                EXPORT  INTTB6_IRQHandler         [WEAK]
                EXPORT  INTTB7_IRQHandler         [WEAK]
                EXPORT  INTI2CS_IRQHandler        [WEAK]
                EXPORT  INTTMRD_IRQHandler        [WEAK]
                EXPORT  INTUSB_IRQHandler         [WEAK]
                EXPORT  INTUSBWKUP_IRQHandler     [WEAK]
                EXPORT  INTADHP_IRQHandler        [WEAK]
                EXPORT  INTAD_IRQHandler          [WEAK]

INT0_IRQHandler
INT1_IRQHandler
INT2_IRQHandler
INT3_IRQHandler
INT4_IRQHandler
INT5_IRQHandler
INTRX0_IRQHandler
INTTX0_IRQHandler
INTRX1_IRQHandler
INTTX1_IRQHandler
INTSPIRX_IRQHandler
INTSPITX_IRQHandler
INTSPIERR_IRQHandler
INTI2C0_IRQHandler
INTI2C1_IRQHandler
INTDMA_IRQHandler
INT16A0_IRQHandler
INT16A1_IRQHandler
INTTB0_IRQHandler
INTTB1_IRQHandler
INTTB2_IRQHandler
INTTB3_IRQHandler
INTTB4_IRQHandler
INTTB5_IRQHandler
INTTB6_IRQHandler
INTTB7_IRQHandler
INTI2CS_IRQHandler
INTTMRD_IRQHandler
INTUSB_IRQHandler
INTUSBWKUP_IRQHandler
INTADHP_IRQHandler
INTAD_IRQHandler

                B       .

                ENDP

                END
