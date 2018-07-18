;/**
; *******************************************************************************
; * @file    startup_TMPM3H6.s
; * @brief   CMSIS Cortex-M3 Core Device Startup File for the
; *          TOSHIBA 'TMPM3H6' Device Series
; * @version V1.0.0.0
; * $Date::            #$
; * 
; * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LISENCE AGREEMENT.
; * 
; * (C)Copyright TOSHIBA MICROELECTRONICS CORPORATION 2017 All rights reserved
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
                DCD     INT00_IRQHandler          ; 0:	Interrupt Pin0
                DCD     INT01_IRQHandler          ; 1:	Interrupt Pin1
                DCD     INT02_IRQHandler          ; 2:	Interrupt Pin2
                DCD     INT03_IRQHandler          ; 3:	Interrupt Pin3
                DCD     INT04_IRQHandler          ; 4:	Interrupt Pin4
                DCD     INT05_IRQHandler          ; 5:	Interrupt Pin5
                DCD     INT06_IRQHandler          ; 6:	Interrupt Pin6
                DCD     INT07_IRQHandler          ; 7:	Interrupt Pin7
                DCD     INT08_IRQHandler          ; 8:	Interrupt Pin8
                DCD     INT09_IRQHandler          ; 9:	Interrupt Pin9
                DCD     INT10_IRQHandler          ; 10:	Interrupt Pin10
                DCD     INT11_IRQHandler          ; 11:	Interrupt Pin11
                DCD     INT12_IRQHandler          ; 12:	Interrupt Pin12
                DCD     INT13_IRQHandler          ; 13:	Interrupt Pin13
                DCD     INT14_IRQHandler          ; 14:	Interrupt Pin14
                DCD     INT15_IRQHandler          ; 15:	Interrupt Pin15
                DCD     INTEMG0_IRQHandler        ; 16:	PMD0 EMG interrupt
                DCD     INTOVV0_IRQHandler        ; 17:	PMD0 OVV interrupt
                DCD     INTPMD0_IRQHandler        ; 18:	PMD0 interrupt
                DCD     INTENC00_IRQHandler       ; 19:	Encoder 0 interrupt 0
                DCD     INTENC01_IRQHandler       ; 20:	Encoder 0 interrupt 1
                DCD     INTADAPDA_IRQHandler      ; 21:	ADC conversion triggered by PMD is finished A
                DCD     INTADAPDB_IRQHandler      ; 22:	ADC conversion triggered by PMD is finished B
                DCD     INTADACP0_IRQHandler      ; 23:	ADC conversion monitoring function interrupt 0
                DCD     INTADACP1_IRQHandler      ; 24:	ADC conversion monitoring function interrupt 1
                DCD     INTADATRG_IRQHandler      ; 25:	ADC conversion triggered by General purpose is finished
                DCD     INTADASGL_IRQHandler      ; 26:	ADC conversion triggered by Single program is finished
                DCD     INTADACNT_IRQHandler      ; 27:	ADC conversion triggered by Continuity program is finished
                DCD     INTT0RX_IRQHandler        ; 28:	TSPI/SIO reception (channel 0)
                DCD     INTT0TX_IRQHandler        ; 29:	TSPI/SIO transmit (channel 0)
                DCD     INTT0ERR_IRQHandler       ; 30:	TSPI/SIO error (channel 0)
                DCD     INTT1RX_IRQHandler        ; 31:	TSPI/SIO reception (channel 1)
                DCD     INTT1TX_IRQHandler        ; 32:	TSPI/SIO transmit (channel 1)
                DCD     INTT1ERR_IRQHandler       ; 33:	TSPI/SIO error (channel 1)
                DCD     INTI2CWUP_IRQHandler      ; 34:	Serial bus interface (WakeUp) interrupt (channel 0)
                DCD     INTI2C0_IRQHandler        ; 35:	I2C0 transmission and reception interrupt
                DCD     INTI2C0AL_IRQHandler      ; 36:	I2C0 arbitration lost interrupt
                DCD     INTI2C0BF_IRQHandler      ; 37:	I2C0 bus free interrupt
                DCD     INTI2C0NA_IRQHandler      ; 38:	I2C0 no ack interrupt
                DCD     INTI2C1_IRQHandler        ; 39:	I2C1 transmission and reception interrupt
                DCD     INTI2C1AL_IRQHandler      ; 40:	I2C1 arbitration lost interrupt
                DCD     INTI2C1BF_IRQHandler      ; 41:	I2C1 bus free interrupt
                DCD     INTI2C1NA_IRQHandler      ; 42:	I2C1 no ack interrupt
                DCD     INTI2C2_IRQHandler        ; 43:	I2C2 transmission and reception interrupt
                DCD     INTI2C2AL_IRQHandler      ; 44:	I2C2 arbitration lost interrupt
                DCD     INTI2C2BF_IRQHandler      ; 45:	I2C2 bus free interrupt
                DCD     INTI2C2NA_IRQHandler      ; 46:	I2C2 no ack interrupt
                DCD     INTUART0RX_IRQHandler     ; 47:	UART reception (channel 0)
                DCD     INTUART0TX_IRQHandler     ; 48:	UART transmit (channel 0)
                DCD     INTUART0ERR_IRQHandler    ; 49:	UART error (channel 0)
                DCD     INTUART1RX_IRQHandler     ; 50:	UART reception (channel 1)
                DCD     INTUART1TX_IRQHandler     ; 51:	UART transmit (channel 1)
                DCD     INTUART1ERR_IRQHandler    ; 52:	UART error (channel 1)
                DCD     INTUART2RX_IRQHandler     ; 53:	UART reception (channel 2)
                DCD     INTUART2TX_IRQHandler     ; 54:	UART transmit (channel 2)
                DCD     INTUART2ERR_IRQHandler    ; 55:	UART error (channel 2)
                DCD     INTT32A00A_IRQHandler     ; 56:	32bit T32A00A compare match detection 0 / Over flow / under flow
                DCD     INTT32A00ACAP0_IRQHandler ; 57:	32bit T32A00A input capture 0
                DCD     INTT32A00ACAP1_IRQHandler ; 58:	32bit T32A00A input capture 1
                DCD     INTT32A00B_IRQHandler     ; 59:	32bit T32A00B compare match detection 0 / Over flow / under flow
                DCD     INTT32A00BCAP0_IRQHandler ; 60:	32bit T32A00B input capture 0
                DCD     INTT32A00BCAP1_IRQHandler ; 61:	32bit T32A00B input capture 1
                DCD     INTT32A00C_IRQHandler     ; 62:	32bit T32A00C compare match detection 0 / Over flow / under flow
                DCD     INTT32A00CCAP0_IRQHandler ; 63:	32bit T32A00C input capture 0
                DCD     INTT32A00CCAP1_IRQHandler ; 64:	32bit T32A00C input capture 1
                DCD     INTT32A01A_IRQHandler     ; 65:	32bit T32A01A compare match detection 0 / Over flow / under flow
                DCD     INTT32A01ACAP0_IRQHandler ; 66:	32bit T32A01A input capture 0
                DCD     INTT32A01ACAP1_IRQHandler ; 67:	32bit T32A01A input capture 1
                DCD     INTT32A01B_IRQHandler     ; 68:	32bit T32A01B compare match detection 0 / Over flow / under flow
                DCD     INTT32A01BCAP0_IRQHandler ; 69:	32bit T32A01B input capture 0
                DCD     INTT32A01BCAP1_IRQHandler ; 70:	32bit T32A01B input capture 1
                DCD     INTT32A01C_IRQHandler     ; 71:	32bit T32A01C compare match detection 0 / Over flow / under flow
                DCD     INTT32A01CCAP0_IRQHandler ; 72:	32bit T32A01C input capture 0
                DCD     INTT32A01CCAP1_IRQHandler ; 73:	32bit T32A01C input capture 1
                DCD     INTT32A02A_IRQHandler     ; 74:	32bit T32A02A compare match detection 0 / Over flow / under flow
                DCD     INTT32A02ACAP0_IRQHandler ; 75:	32bit T32A02A input capture 0
                DCD     INTT32A02ACAP1_IRQHandler ; 76:	32bit T32A02A input capture 1
                DCD     INTT32A02B_IRQHandler     ; 77:	32bit T32A02B compare match detection 0 / Over flow / under flow
                DCD     INTT32A02BCAP0_IRQHandler ; 78:	32bit T32A02B input capture 0
                DCD     INTT32A02BCAP1_IRQHandler ; 79:	32bit T32A02B input capture 1
                DCD     INTT32A02C_IRQHandler     ; 80:	32bit T32A02C compare match detection 0 / Over flow / under flow
                DCD     INTT32A02CCAP0_IRQHandler ; 81:	32bit T32A02C input capture 0
                DCD     INTT32A02CCAP1_IRQHandler ; 82:	32bit T32A02C input capture 1
                DCD     INTT32A03A_IRQHandler     ; 83:	32bit T32A03A compare match detection 0 / Over flow / under flow
                DCD     INTT32A03ACAP0_IRQHandler ; 84:	32bit T32A03A input capture 0
                DCD     INTT32A03ACAP1_IRQHandler ; 85:	32bit T32A03A input capture 1
                DCD     INTT32A03B_IRQHandler     ; 86:	32bit T32A03B compare match detection 0 / Over flow / under flow
                DCD     INTT32A03BCAP0_IRQHandler ; 87:	32bit T32A03B input capture 0
                DCD     INTT32A03BCAP1_IRQHandler ; 88:	32bit T32A03B input capture 1
                DCD     INTT32A03C_IRQHandler     ; 89:	32bit T32A03C compare match detection 0 / Over flow / under flow
                DCD     INTT32A03CCAP0_IRQHandler ; 90:	32bit T32A03C input capture 0
                DCD     INTT32A03CCAP1_IRQHandler ; 91:	32bit T32A03C input capture 1
                DCD     INTT32A04A_IRQHandler     ; 92:	32bit T32A04A compare match detection 0 / Over flow / under flow
                DCD     INTT32A04ACAP0_IRQHandler ; 93:	32bit T32A04A input capture 0
                DCD     INTT32A04ACAP1_IRQHandler ; 94:	32bit T32A04A input capture 1
                DCD     INTT32A04B_IRQHandler     ; 95:	32bit T32A04B compare match detection 0 / Over flow / under flow
                DCD     INTT32A04BCAP0_IRQHandler ; 96:	32bit T32A04B input capture 0
                DCD     INTT32A04BCAP1_IRQHandler ; 97:	32bit T32A04B input capture 1
                DCD     INTT32A04C_IRQHandler     ; 98:	32bit T32A04C compare match detection 0 / Over flow / under flow
                DCD     INTT32A04CCAP0_IRQHandler ; 99:	32bit T32A04C input capture 0
                DCD     INTT32A04CCAP1_IRQHandler ; 100:	32bit T32A04C input capture 1
                DCD     INTT32A05A_IRQHandler     ; 101:	32bit T32A05A compare match detection 0 / Over flow / under flow
                DCD     INTT32A05ACAP0_IRQHandler ; 102:	32bit T32A05A input capture 0
                DCD     INTT32A05ACAP1_IRQHandler ; 103:	32bit T32A05A input capture 1
                DCD     INTT32A05B_IRQHandler     ; 104:	32bit T32A05B compare match detection 0 / Over flow / under flow
                DCD     INTT32A05BCAP0_IRQHandler ; 105:	32bit T32A05B input capture 0
                DCD     INTT32A05BCAP1_IRQHandler ; 106:	32bit T32A05B input capture 1
                DCD     INTT32A05C_IRQHandler     ; 107:	32bit T32A05C compare match detection 0 / Over flow / under flow
                DCD     INTT32A05CCAP0_IRQHandler ; 108:	32bit T32A05C input capture 0
                DCD     INTT32A05CCAP1_IRQHandler ; 109:	32bit T32A05C input capture 1
                DCD     INTDMAATC_IRQHandler      ; 110:	DMA end of transfer
                DCD     INTDMAAERR_IRQHandler     ; 111:	DMA transfer error
                DCD     INTRTC_IRQHandler         ; 112:	Real time clock(XHz) interrupt
                DCD     0                         ; 113:	Reserved
                DCD     INTRMC0_IRQHandler        ; 114:	Remote control reception interrupt
                DCD     INTFLCRDY_IRQHandler      ; 115:	Code FLASH Ready interrupt
                DCD     INTFLDRDY_IRQHandler      ; 116:	Data FLASH Ready interrupt
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

                PUBWEAK MemManage_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
MemManage_Handler
                B       MemManage_Handler

                PUBWEAK BusFault_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
BusFault_Handler
                B       BusFault_Handler

                PUBWEAK UsageFault_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
UsageFault_Handler
                B       UsageFault_Handler

                PUBWEAK SVC_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
                B       SVC_Handler

                PUBWEAK DebugMon_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
DebugMon_Handler
                B       DebugMon_Handler

                PUBWEAK PendSV_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
                B       PendSV_Handler

                PUBWEAK SysTick_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
                B       SysTick_Handler

                PUBWEAK INT00_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT00_IRQHandler
                B       INT00_IRQHandler

                PUBWEAK INT01_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT01_IRQHandler
                B       INT01_IRQHandler

                PUBWEAK INT02_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT02_IRQHandler
                B       INT02_IRQHandler

                PUBWEAK INT03_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT03_IRQHandler
                B       INT03_IRQHandler

                PUBWEAK INT04_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT04_IRQHandler
                B       INT04_IRQHandler

                PUBWEAK INT05_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT05_IRQHandler
                B       INT05_IRQHandler

                PUBWEAK INT06_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT06_IRQHandler
                B       INT06_IRQHandler

                PUBWEAK INT07_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT07_IRQHandler
                B       INT07_IRQHandler

                PUBWEAK INT08_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT08_IRQHandler
                B       INT08_IRQHandler

                PUBWEAK INT09_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT09_IRQHandler
                B       INT09_IRQHandler

                PUBWEAK INT10_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT10_IRQHandler
                B       INT10_IRQHandler

                PUBWEAK INT11_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT11_IRQHandler
                B       INT11_IRQHandler

                PUBWEAK INT12_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT12_IRQHandler
                B       INT12_IRQHandler

                PUBWEAK INT13_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT13_IRQHandler
                B       INT13_IRQHandler

                PUBWEAK INT14_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT14_IRQHandler
                B       INT14_IRQHandler

                PUBWEAK INT15_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT15_IRQHandler
                B       INT15_IRQHandler

                PUBWEAK INTEMG0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTEMG0_IRQHandler
                B       INTEMG0_IRQHandler

                PUBWEAK INTOVV0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTOVV0_IRQHandler
                B       INTOVV0_IRQHandler

                PUBWEAK INTPMD0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTPMD0_IRQHandler
                B       INTPMD0_IRQHandler

                PUBWEAK INTENC00_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTENC00_IRQHandler
                B       INTENC00_IRQHandler

                PUBWEAK INTENC01_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTENC01_IRQHandler
                B       INTENC01_IRQHandler

                PUBWEAK INTADAPDA_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTADAPDA_IRQHandler
                B       INTADAPDA_IRQHandler

                PUBWEAK INTADAPDB_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTADAPDB_IRQHandler
                B       INTADAPDB_IRQHandler

                PUBWEAK INTADACP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTADACP0_IRQHandler
                B       INTADACP0_IRQHandler

                PUBWEAK INTADACP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTADACP1_IRQHandler
                B       INTADACP1_IRQHandler

                PUBWEAK INTADATRG_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTADATRG_IRQHandler
                B       INTADATRG_IRQHandler

                PUBWEAK INTADASGL_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTADASGL_IRQHandler
                B       INTADASGL_IRQHandler

                PUBWEAK INTADACNT_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTADACNT_IRQHandler
                B       INTADACNT_IRQHandler

                PUBWEAK INTT0RX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT0RX_IRQHandler
                B       INTT0RX_IRQHandler

                PUBWEAK INTT0TX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT0TX_IRQHandler
                B       INTT0TX_IRQHandler

                PUBWEAK INTT0ERR_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT0ERR_IRQHandler
                B       INTT0ERR_IRQHandler

                PUBWEAK INTT1RX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT1RX_IRQHandler
                B       INTT1RX_IRQHandler

                PUBWEAK INTT1TX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT1TX_IRQHandler
                B       INTT1TX_IRQHandler

                PUBWEAK INTT1ERR_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT1ERR_IRQHandler
                B       INTT1ERR_IRQHandler

                PUBWEAK INTI2CWUP_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2CWUP_IRQHandler
                B       INTI2CWUP_IRQHandler

                PUBWEAK INTI2C0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2C0_IRQHandler
                B       INTI2C0_IRQHandler

                PUBWEAK INTI2C0AL_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2C0AL_IRQHandler
                B       INTI2C0AL_IRQHandler

                PUBWEAK INTI2C0BF_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2C0BF_IRQHandler
                B       INTI2C0BF_IRQHandler

                PUBWEAK INTI2C0NA_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2C0NA_IRQHandler
                B       INTI2C0NA_IRQHandler

                PUBWEAK INTI2C1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2C1_IRQHandler
                B       INTI2C1_IRQHandler

                PUBWEAK INTI2C1AL_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2C1AL_IRQHandler
                B       INTI2C1AL_IRQHandler

                PUBWEAK INTI2C1BF_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2C1BF_IRQHandler
                B       INTI2C1BF_IRQHandler

                PUBWEAK INTI2C1NA_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2C1NA_IRQHandler
                B       INTI2C1NA_IRQHandler

                PUBWEAK INTI2C2_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2C2_IRQHandler
                B       INTI2C2_IRQHandler

                PUBWEAK INTI2C2AL_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2C2AL_IRQHandler
                B       INTI2C2AL_IRQHandler

                PUBWEAK INTI2C2BF_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2C2BF_IRQHandler
                B       INTI2C2BF_IRQHandler

                PUBWEAK INTI2C2NA_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2C2NA_IRQHandler
                B       INTI2C2NA_IRQHandler

                PUBWEAK INTUART0RX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUART0RX_IRQHandler
                B       INTUART0RX_IRQHandler

                PUBWEAK INTUART0TX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUART0TX_IRQHandler
                B       INTUART0TX_IRQHandler

                PUBWEAK INTUART0ERR_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUART0ERR_IRQHandler
                B       INTUART0ERR_IRQHandler

                PUBWEAK INTUART1RX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUART1RX_IRQHandler
                B       INTUART1RX_IRQHandler

                PUBWEAK INTUART1TX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUART1TX_IRQHandler
                B       INTUART1TX_IRQHandler

                PUBWEAK INTUART1ERR_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUART1ERR_IRQHandler
                B       INTUART1ERR_IRQHandler

                PUBWEAK INTUART2RX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUART2RX_IRQHandler
                B       INTUART2RX_IRQHandler

                PUBWEAK INTUART2TX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUART2TX_IRQHandler
                B       INTUART2TX_IRQHandler

                PUBWEAK INTUART2ERR_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUART2ERR_IRQHandler
                B       INTUART2ERR_IRQHandler

                PUBWEAK INTT32A00A_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A00A_IRQHandler
                B       INTT32A00A_IRQHandler

                PUBWEAK INTT32A00ACAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A00ACAP0_IRQHandler
                B       INTT32A00ACAP0_IRQHandler

                PUBWEAK INTT32A00ACAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A00ACAP1_IRQHandler
                B       INTT32A00ACAP1_IRQHandler

                PUBWEAK INTT32A00B_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A00B_IRQHandler
                B       INTT32A00B_IRQHandler

                PUBWEAK INTT32A00BCAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A00BCAP0_IRQHandler
                B       INTT32A00BCAP0_IRQHandler

                PUBWEAK INTT32A00BCAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A00BCAP1_IRQHandler
                B       INTT32A00BCAP1_IRQHandler

                PUBWEAK INTT32A00C_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A00C_IRQHandler
                B       INTT32A00C_IRQHandler

                PUBWEAK INTT32A00CCAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A00CCAP0_IRQHandler
                B       INTT32A00CCAP0_IRQHandler

                PUBWEAK INTT32A00CCAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A00CCAP1_IRQHandler
                B       INTT32A00CCAP1_IRQHandler

                PUBWEAK INTT32A01A_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A01A_IRQHandler
                B       INTT32A01A_IRQHandler

                PUBWEAK INTT32A01ACAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A01ACAP0_IRQHandler
                B       INTT32A01ACAP0_IRQHandler

                PUBWEAK INTT32A01ACAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A01ACAP1_IRQHandler
                B       INTT32A01ACAP1_IRQHandler

                PUBWEAK INTT32A01B_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A01B_IRQHandler
                B       INTT32A01B_IRQHandler

                PUBWEAK INTT32A01BCAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A01BCAP0_IRQHandler
                B       INTT32A01BCAP0_IRQHandler

                PUBWEAK INTT32A01BCAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A01BCAP1_IRQHandler
                B       INTT32A01BCAP1_IRQHandler

                PUBWEAK INTT32A01C_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A01C_IRQHandler
                B       INTT32A01C_IRQHandler

                PUBWEAK INTT32A01CCAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A01CCAP0_IRQHandler
                B       INTT32A01CCAP0_IRQHandler

                PUBWEAK INTT32A01CCAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A01CCAP1_IRQHandler
                B       INTT32A01CCAP1_IRQHandler

                PUBWEAK INTT32A02A_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A02A_IRQHandler
                B       INTT32A02A_IRQHandler

                PUBWEAK INTT32A02ACAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A02ACAP0_IRQHandler
                B       INTT32A02ACAP0_IRQHandler

                PUBWEAK INTT32A02ACAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A02ACAP1_IRQHandler
                B       INTT32A02ACAP1_IRQHandler

                PUBWEAK INTT32A02B_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A02B_IRQHandler
                B       INTT32A02B_IRQHandler

                PUBWEAK INTT32A02BCAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A02BCAP0_IRQHandler
                B       INTT32A02BCAP0_IRQHandler

                PUBWEAK INTT32A02BCAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A02BCAP1_IRQHandler
                B       INTT32A02BCAP1_IRQHandler

                PUBWEAK INTT32A02C_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A02C_IRQHandler
                B       INTT32A02C_IRQHandler

                PUBWEAK INTT32A02CCAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A02CCAP0_IRQHandler
                B       INTT32A02CCAP0_IRQHandler

                PUBWEAK INTT32A02CCAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A02CCAP1_IRQHandler
                B       INTT32A02CCAP1_IRQHandler

                PUBWEAK INTT32A03A_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A03A_IRQHandler
                B       INTT32A03A_IRQHandler

                PUBWEAK INTT32A03ACAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A03ACAP0_IRQHandler
                B       INTT32A03ACAP0_IRQHandler

                PUBWEAK INTT32A03ACAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A03ACAP1_IRQHandler
                B       INTT32A03ACAP1_IRQHandler

                PUBWEAK INTT32A03B_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A03B_IRQHandler
                B       INTT32A03B_IRQHandler

                PUBWEAK INTT32A03BCAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A03BCAP0_IRQHandler
                B       INTT32A03BCAP0_IRQHandler

                PUBWEAK INTT32A03BCAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A03BCAP1_IRQHandler
                B       INTT32A03BCAP1_IRQHandler

                PUBWEAK INTT32A03C_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A03C_IRQHandler
                B       INTT32A03C_IRQHandler

                PUBWEAK INTT32A03CCAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A03CCAP0_IRQHandler
                B       INTT32A03CCAP0_IRQHandler

                PUBWEAK INTT32A03CCAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A03CCAP1_IRQHandler
                B       INTT32A03CCAP1_IRQHandler

                PUBWEAK INTT32A04A_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A04A_IRQHandler
                B       INTT32A04A_IRQHandler

                PUBWEAK INTT32A04ACAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A04ACAP0_IRQHandler
                B       INTT32A04ACAP0_IRQHandler

                PUBWEAK INTT32A04ACAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A04ACAP1_IRQHandler
                B       INTT32A04ACAP1_IRQHandler

                PUBWEAK INTT32A04B_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A04B_IRQHandler
                B       INTT32A04B_IRQHandler

                PUBWEAK INTT32A04BCAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A04BCAP0_IRQHandler
                B       INTT32A04BCAP0_IRQHandler

                PUBWEAK INTT32A04BCAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A04BCAP1_IRQHandler
                B       INTT32A04BCAP1_IRQHandler

                PUBWEAK INTT32A04C_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A04C_IRQHandler
                B       INTT32A04C_IRQHandler

                PUBWEAK INTT32A04CCAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A04CCAP0_IRQHandler
                B       INTT32A04CCAP0_IRQHandler

                PUBWEAK INTT32A04CCAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A04CCAP1_IRQHandler
                B       INTT32A04CCAP1_IRQHandler

                PUBWEAK INTT32A05A_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A05A_IRQHandler
                B       INTT32A05A_IRQHandler

                PUBWEAK INTT32A05ACAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A05ACAP0_IRQHandler
                B       INTT32A05ACAP0_IRQHandler

                PUBWEAK INTT32A05ACAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A05ACAP1_IRQHandler
                B       INTT32A05ACAP1_IRQHandler

                PUBWEAK INTT32A05B_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A05B_IRQHandler
                B       INTT32A05B_IRQHandler

                PUBWEAK INTT32A05BCAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A05BCAP0_IRQHandler
                B       INTT32A05BCAP0_IRQHandler

                PUBWEAK INTT32A05BCAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A05BCAP1_IRQHandler
                B       INTT32A05BCAP1_IRQHandler

                PUBWEAK INTT32A05C_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A05C_IRQHandler
                B       INTT32A05C_IRQHandler

                PUBWEAK INTT32A05CCAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A05CCAP0_IRQHandler
                B       INTT32A05CCAP0_IRQHandler

                PUBWEAK INTT32A05CCAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A05CCAP1_IRQHandler
                B       INTT32A05CCAP1_IRQHandler

                PUBWEAK INTDMAATC_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTDMAATC_IRQHandler
                B       INTDMAATC_IRQHandler

                PUBWEAK INTDMAAERR_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTDMAAERR_IRQHandler
                B       INTDMAAERR_IRQHandler

                PUBWEAK INTRTC_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTRTC_IRQHandler
                B       INTRTC_IRQHandler

                PUBWEAK INTRMC0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTRMC0_IRQHandler
                B       INTRMC0_IRQHandler

                PUBWEAK INTFLCRDY_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTFLCRDY_IRQHandler
                B       INTFLCRDY_IRQHandler

                PUBWEAK INTFLDRDY_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTFLDRDY_IRQHandler
                B       INTFLDRDY_IRQHandler

                END
