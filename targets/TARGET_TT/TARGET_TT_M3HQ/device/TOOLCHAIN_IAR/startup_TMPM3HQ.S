;/**
; *******************************************************************************
; * @file    startup_TMPM3HQ.s
; * @brief   CMSIS Cortex-M3 Core Device Startup File for the
; *          TOSHIBA 'TMPM3HQ' Device Series
; * @version V1.0.0.0
; * $Date:: 2017-12-08 #$
; * 
; * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
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
                DCD     INT00_IRQHandler          ; 0:	Interrupt Pin 00
                DCD     INT01_IRQHandler          ; 1:	Interrupt Pin 01
                DCD     INT02_IRQHandler          ; 2:	Interrupt Pin 02
                DCD     INT03_IRQHandler          ; 3:	Interrupt Pin 03
                DCD     INT04_IRQHandler          ; 4:	Interrupt Pin 04
                DCD     INT05_IRQHandler          ; 5:	Interrupt Pin 05
                DCD     INT06_IRQHandler          ; 6:	Interrupt Pin 06
                DCD     INT07_IRQHandler          ; 7:	Interrupt Pin 07
                DCD     INT08_IRQHandler          ; 8:	Interrupt Pin 08
                DCD     INT09_IRQHandler          ; 9:	Interrupt Pin 09
                DCD     INT10_IRQHandler          ; 10:	Interrupt Pin 10
                DCD     INT11_IRQHandler          ; 11:	Interrupt Pin 11
                DCD     INT12_IRQHandler          ; 12:	Interrupt Pin 12
                DCD     INT13_IRQHandler          ; 13:	Interrupt Pin 13
                DCD     INT14_IRQHandler          ; 14:	Interrupt Pin 14
                DCD     INT15_IRQHandler          ; 15:	Interrupt Pin 15
                DCD     INT16_IRQHandler          ; 16:	Interrupt Pin 16
                DCD     INT17_18_IRQHandler       ; 17:	Interrupt Pin 17_18
                DCD     INT19_22_IRQHandler       ; 18:	Interrupt Pin 19_22
                DCD     INT23_26_IRQHandler       ; 19:	Interrupt Pin 23_26
                DCD     INT27_28_IRQHandler       ; 20:	Interrupt Pin 27_28
                DCD     INT29_IRQHandler          ; 21:	Interrupt Pin 29
                DCD     INT30_31_IRQHandler       ; 22:	Interrupt Pin 30_31
                DCD     INTEMG0_IRQHandler        ; 23:	PMD0 EMG interrupt
                DCD     INTOVV0_IRQHandler        ; 24:	PMD0 OVV interrupt
                DCD     INTPMD0_IRQHandler        ; 25:	PMD0 interrupt
                DCD     INTENC00_IRQHandler       ; 26:	Encoder 0 interrupt 0
                DCD     INTENC01_IRQHandler       ; 27:	Encoder 0 interrupt 1
                DCD     INTADAPDA_IRQHandler      ; 28:	ADC conversion triggered by PMD is finished A
                DCD     INTADAPDB_IRQHandler      ; 29:	ADC conversion triggered by PMD is finished B
                DCD     INTADACP0_IRQHandler      ; 30:	ADC conversion monitoring function interrupt 0
                DCD     INTADACP1_IRQHandler      ; 31:	ADC conversion monitoring function interrupt 1
                DCD     INTADATRG_IRQHandler      ; 32:	ADC conversion triggered by General purpose is finished
                DCD     INTADASGL_IRQHandler      ; 33:	ADC conversion triggered by Single program is finished
                DCD     INTADACNT_IRQHandler      ; 34:	ADC conversion triggered by Continuity program is finished
                DCD     INTT0RX_IRQHandler        ; 35:	TSPI/SIO reception (channel 0)
                DCD     INTT0TX_IRQHandler        ; 36:	TSPI/SIO transmit (channel 0)
                DCD     INTT0ERR_IRQHandler       ; 37:	TSPI/SIO error (channel 0)
                DCD     INTT1RX_IRQHandler        ; 38:	TSPI/SIO reception (channel 1)
                DCD     INTT1TX_IRQHandler        ; 39:	TSPI/SIO transmit (channel 1)
                DCD     INTT1ERR_IRQHandler       ; 40:	TSPI/SIO error (channel 1)
                DCD     INTT2RX_IRQHandler        ; 41:	TSPI/SIO reception (channel 2)
                DCD     INTT2TX_IRQHandler        ; 42:	TSPI/SIO transmit (channel 2)
                DCD     INTT2ERR_IRQHandler       ; 43:	TSPI/SIO error (channel 2)
                DCD     INTT3RX_IRQHandler        ; 44:	TSPI/SIO reception (channel 3)
                DCD     INTT3TX_IRQHandler        ; 45:	TSPI/SIO transmit (channel 3)
                DCD     INTT3ERR_IRQHandler       ; 46:	TSPI/SIO error (channel 3)
                DCD     INTT4RX_IRQHandler        ; 47:	TSPI/SIO reception (channel 4)
                DCD     INTT4TX_IRQHandler        ; 48:	TSPI/SIO transmit (channel 4)
                DCD     INTT4ERR_IRQHandler       ; 49:	TSPI/SIO error (channel 4)
                DCD     INTI2CWUP_IRQHandler      ; 50:	Serial bus interface (WakeUp) interrupt (channel 0)
                DCD     INTI2C0_IRQHandler        ; 51:	I2C0 transmission and reception interrupt
                DCD     INTI2C0AL_IRQHandler      ; 52:	I2C0 arbitration lost interrupt
                DCD     INTI2C0BF_IRQHandler      ; 53:	I2C0 bus free interrupt
                DCD     INTI2C0NA_IRQHandler      ; 54:	I2C0 no ack interrupt
                DCD     INTI2C1_IRQHandler        ; 55:	I2C1 transmission and reception interrupt
                DCD     INTI2C1AL_IRQHandler      ; 56:	I2C1 arbitration lost interrupt
                DCD     INTI2C1BF_IRQHandler      ; 57:	I2C1 bus free interrupt
                DCD     INTI2C1NA_IRQHandler      ; 58:	I2C1 no ack interrupt
                DCD     INTI2C2_IRQHandler        ; 59:	I2C2 transmission and reception interrupt
                DCD     INTI2C2AL_IRQHandler      ; 60:	I2C2 arbitration lost interrupt
                DCD     INTI2C2BF_IRQHandler      ; 61:	I2C2 bus free interrupt
                DCD     INTI2C2NA_IRQHandler      ; 62:	I2C2 no ack interrupt
                DCD     INTI2C3_IRQHandler        ; 63:	I2C3 transmission and reception interrupt
                DCD     INTI2C3AL_IRQHandler      ; 64:	I2C3 arbitration lost interrupt
                DCD     INTI2C3BF_IRQHandler      ; 65:	I2C3 bus free interrupt
                DCD     INTI2C3NA_IRQHandler      ; 66:	I2C3 no ack interrupt
                DCD     INTUART0RX_IRQHandler     ; 67:	UART reception (channel 0)
                DCD     INTUART0TX_IRQHandler     ; 68:	UART transmit (channel 0)
                DCD     INTUART0ERR_IRQHandler    ; 69:	UART error (channel 0)
                DCD     INTUART1RX_IRQHandler     ; 70:	UART reception (channel 1)
                DCD     INTUART1TX_IRQHandler     ; 71:	UART transmit (channel 1)
                DCD     INTUART1ERR_IRQHandler    ; 72:	UART error (channel 1)
                DCD     INTUART2RX_IRQHandler     ; 73:	UART reception (channel 2)
                DCD     INTUART2TX_IRQHandler     ; 74:	UART transmit (channel 2)
                DCD     INTUART2ERR_IRQHandler    ; 75:	UART error (channel 2)
                DCD     INTUART3RX_IRQHandler     ; 76:	UART reception (channel 3)
                DCD     INTUART3TX_IRQHandler     ; 77:	UART transmit (channel 3)
                DCD     INTUART3ERR_IRQHandler    ; 78:	UART error (channel 3)
                DCD     INTUART4RX_IRQHandler     ; 79:	UART reception (channel 4)
                DCD     INTUART4TX_IRQHandler     ; 80:	UART transmit (channel 4)
                DCD     INTUART4ERR_IRQHandler    ; 81:	UART error (channel 4)
                DCD     INTUART5RX_IRQHandler     ; 82:	UART reception (channel 5)
                DCD     INTUART5TX_IRQHandler     ; 83:	UART transmit (channel 5)
                DCD     INTUART5ERR_IRQHandler    ; 84:	UART error (channel 5)
                DCD     INTT32A00A_IRQHandler     ; 85:	32bit T32A00A compare match detection 0 / Over flow / under flow
                DCD     INTT32A00ACAP0_IRQHandler ; 86:	32bit T32A00A input capture 0
                DCD     INTT32A00ACAP1_IRQHandler ; 87:	32bit T32A00A input capture 1
                DCD     INTT32A00B_IRQHandler     ; 88:	32bit T32A00B compare match detection 0 / Over flow / under flow
                DCD     INTT32A00BCAP0_IRQHandler ; 89:	32bit T32A00B input capture 0
                DCD     INTT32A00BCAP1_IRQHandler ; 90:	32bit T32A00B input capture 1
                DCD     INTT32A00C_IRQHandler     ; 91:	32bit T32A00C compare match detection 0 / Over flow / under flow
                DCD     INTT32A00CCAP0_IRQHandler ; 92:	32bit T32A00C input capture 0
                DCD     INTT32A00CCAP1_IRQHandler ; 93:	32bit T32A00C input capture 1
                DCD     INTT32A01A_IRQHandler     ; 94:	32bit T32A01A compare match detection 0 / Over flow / under flow
                DCD     INTT32A01ACAP0_IRQHandler ; 95:	32bit T32A01A input capture 0
                DCD     INTT32A01ACAP1_IRQHandler ; 96:	32bit T32A01A input capture 1
                DCD     INTT32A01B_IRQHandler     ; 97:	32bit T32A01B compare match detection 0 / Over flow / under flow
                DCD     INTT32A01BCAP0_IRQHandler ; 98:	32bit T32A01B input capture 0
                DCD     INTT32A01BCAP1_IRQHandler ; 99:	32bit T32A01B input capture 1
                DCD     INTT32A01C_IRQHandler     ; 100:	32bit T32A01C compare match detection 0 / Over flow / under flow
                DCD     INTT32A01CCAP0_IRQHandler ; 101:	32bit T32A01C input capture 0
                DCD     INTT32A01CCAP1_IRQHandler ; 102:	32bit T32A01C input capture 1
                DCD     INTT32A02A_IRQHandler     ; 103:	32bit T32A02A compare match detection 0 / Over flow / under flow
                DCD     INTT32A02ACAP0_IRQHandler ; 104:	32bit T32A02A input capture 0
                DCD     INTT32A02ACAP1_IRQHandler ; 105:	32bit T32A02A input capture 1
                DCD     INTT32A02B_IRQHandler     ; 106:	32bit T32A02B compare match detection 0 / Over flow / under flow
                DCD     INTT32A02BCAP0_IRQHandler ; 107:	32bit T32A02B input capture 0
                DCD     INTT32A02BCAP1_IRQHandler ; 108:	32bit T32A02B input capture 1
                DCD     INTT32A02C_IRQHandler     ; 109:	32bit T32A02C compare match detection 0 / Over flow / under flow
                DCD     INTT32A02CCAP0_IRQHandler ; 110:	32bit T32A02C input capture 0
                DCD     INTT32A02CCAP1_IRQHandler ; 111:	32bit T32A02C input capture 1
                DCD     INTT32A03A_IRQHandler     ; 112:	32bit T32A03A compare match detection 0 / Over flow / under flow
                DCD     INTT32A03ACAP0_IRQHandler ; 113:	32bit T32A03A input capture 0
                DCD     INTT32A03ACAP1_IRQHandler ; 114:	32bit T32A03A input capture 1
                DCD     INTT32A03B_IRQHandler     ; 115:	32bit T32A03B compare match detection 0 / Over flow / under flow
                DCD     INTT32A03BCAP0_IRQHandler ; 116:	32bit T32A03B input capture 0
                DCD     INTT32A03BCAP1_IRQHandler ; 117:	32bit T32A03B input capture 1
                DCD     INTT32A03C_IRQHandler     ; 118:	32bit T32A03C compare match detection 0 / Over flow / under flow
                DCD     INTT32A03CCAP0_IRQHandler ; 119:	32bit T32A03C input capture 0
                DCD     INTT32A03CCAP1_IRQHandler ; 120:	32bit T32A03C input capture 1
                DCD     INTT32A04A_IRQHandler     ; 121:	32bit T32A04A compare match detection 0 / Over flow / under flow
                DCD     INTT32A04ACAP0_IRQHandler ; 122:	32bit T32A04A input capture 0
                DCD     INTT32A04ACAP1_IRQHandler ; 123:	32bit T32A04A input capture 1
                DCD     INTT32A04B_IRQHandler     ; 124:	32bit T32A04B compare match detection 0 / Over flow / under flow
                DCD     INTT32A04BCAP0_IRQHandler ; 125:	32bit T32A04B input capture 0
                DCD     INTT32A04BCAP1_IRQHandler ; 126:	32bit T32A04B input capture 1
                DCD     INTT32A04C_IRQHandler     ; 127:	32bit T32A04C compare match detection 0 / Over flow / under flow
                DCD     INTT32A04CCAP0_IRQHandler ; 128:	32bit T32A04C input capture 0
                DCD     INTT32A04CCAP1_IRQHandler ; 129:	32bit T32A04C input capture 1
                DCD     INTT32A05A_IRQHandler     ; 130:	32bit T32A05A compare match detection 0 / Over flow / under flow
                DCD     INTT32A05ACAP0_IRQHandler ; 131:	32bit T32A05A input capture 0
                DCD     INTT32A05ACAP1_IRQHandler ; 132:	32bit T32A05A input capture 1
                DCD     INTT32A05B_IRQHandler     ; 133:	32bit T32A05B compare match detection 0 / Over flow / under flow
                DCD     INTT32A05BCAP0_IRQHandler ; 134:	32bit T32A05B input capture 0
                DCD     INTT32A05BCAP1_IRQHandler ; 135:	32bit T32A05B input capture 1
                DCD     INTT32A05C_IRQHandler     ; 136:	32bit T32A05C compare match detection 0 / Over flow / under flow
                DCD     INTT32A05CCAP0_IRQHandler ; 137:	32bit T32A05C input capture 0
                DCD     INTT32A05CCAP1_IRQHandler ; 138:	32bit T32A05C input capture 1
                DCD     INTT32A06A_IRQHandler     ; 139:	32bit T32A06A compare match detection 0 / Over flow / under flow
                DCD     INTT32A06ACAP0_IRQHandler ; 140:	32bit T32A06A input capture 0
                DCD     INTT32A06ACAP1_IRQHandler ; 141:	32bit T32A06A input capture 1
                DCD     INTT32A06B_IRQHandler     ; 142:	32bit T32A06B compare match detection 0 / Over flow / under flow
                DCD     INTT32A06BCAP0_IRQHandler ; 143:	32bit T32A06B input capture 0
                DCD     INTT32A06BCAP1_IRQHandler ; 144:	32bit T32A06B input capture 1
                DCD     INTT32A06C_IRQHandler     ; 145:	32bit T32A06C compare match detection 0 / Over flow / under flow
                DCD     INTT32A06CCAP0_IRQHandler ; 146:	32bit T32A06C input capture 0
                DCD     INTT32A06CCAP1_IRQHandler ; 147:	32bit T32A06C input capture 1
                DCD     INTT32A07A_IRQHandler     ; 148:	32bit T32A07A compare match detection 0 / Over flow / under flow
                DCD     INTT32A07ACAP0_IRQHandler ; 149:	32bit T32A07A input capture 0
                DCD     INTT32A07ACAP1_IRQHandler ; 150:	32bit T32A07A input capture 1
                DCD     INTT32A07B_IRQHandler     ; 151:	32bit T32A07B compare match detection 0 / Over flow / under flow
                DCD     INTT32A07BCAP0_IRQHandler ; 152:	32bit T32A07B input capture 0
                DCD     INTT32A07BCAP1_IRQHandler ; 153:	32bit T32A07B input capture 1
                DCD     INTT32A07C_IRQHandler     ; 154:	32bit T32A07C compare match detection 0 / Over flow / under flow
                DCD     INTT32A07CCAP0_IRQHandler ; 155:	32bit T32A07C input capture 0
                DCD     INTT32A07CCAP1_IRQHandler ; 156:	32bit T32A07C input capture 1
                DCD     INTPARI_IRQHandler        ; 157:	RAM parity interrupt
                DCD     INTDMAATC_IRQHandler      ; 158:	DMAA end of transfer
                DCD     INTDMAAERR_IRQHandler     ; 159:	DMAA transfer error
                DCD     INTDMABTC_IRQHandler      ; 160:	DMAB end of transfer
                DCD     INTDMABERR_IRQHandler     ; 161:	DMAB transfer error
                DCD     INTRTC_IRQHandler         ; 162:	Real time clock interrupt
                DCD     INTRMC0_IRQHandler        ; 163:	Remote control reception interrupt
                DCD     INTFLCRDY_IRQHandler      ; 164:	Code FLASH Ready interrupt
                DCD     INTFLDRDY_IRQHandler      ; 165:	Data FLASH Ready interrupt
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

                PUBWEAK INT16_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT16_IRQHandler
                B       INT16_IRQHandler

                PUBWEAK INT17_18_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT17_18_IRQHandler
                B       INT17_18_IRQHandler

                PUBWEAK INT19_22_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT19_22_IRQHandler
                B       INT19_22_IRQHandler

                PUBWEAK INT23_26_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT23_26_IRQHandler
                B       INT23_26_IRQHandler

                PUBWEAK INT27_28_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT27_28_IRQHandler
                B       INT27_28_IRQHandler

                PUBWEAK INT29_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT29_IRQHandler
                B       INT29_IRQHandler

                PUBWEAK INT30_31_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INT30_31_IRQHandler
                B       INT30_31_IRQHandler

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

                PUBWEAK INTT2RX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT2RX_IRQHandler
                B       INTT2RX_IRQHandler

                PUBWEAK INTT2TX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT2TX_IRQHandler
                B       INTT2TX_IRQHandler

                PUBWEAK INTT2ERR_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT2ERR_IRQHandler
                B       INTT2ERR_IRQHandler

                PUBWEAK INTT3RX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT3RX_IRQHandler
                B       INTT3RX_IRQHandler

                PUBWEAK INTT3TX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT3TX_IRQHandler
                B       INTT3TX_IRQHandler

                PUBWEAK INTT3ERR_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT3ERR_IRQHandler
                B       INTT3ERR_IRQHandler

                PUBWEAK INTT4RX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT4RX_IRQHandler
                B       INTT4RX_IRQHandler

                PUBWEAK INTT4TX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT4TX_IRQHandler
                B       INTT4TX_IRQHandler

                PUBWEAK INTT4ERR_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT4ERR_IRQHandler
                B       INTT4ERR_IRQHandler

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

                PUBWEAK INTI2C3_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2C3_IRQHandler
                B       INTI2C3_IRQHandler

                PUBWEAK INTI2C3AL_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2C3AL_IRQHandler
                B       INTI2C3AL_IRQHandler

                PUBWEAK INTI2C3BF_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2C3BF_IRQHandler
                B       INTI2C3BF_IRQHandler

                PUBWEAK INTI2C3NA_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTI2C3NA_IRQHandler
                B       INTI2C3NA_IRQHandler

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

                PUBWEAK INTUART3RX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUART3RX_IRQHandler
                B       INTUART3RX_IRQHandler

                PUBWEAK INTUART3TX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUART3TX_IRQHandler
                B       INTUART3TX_IRQHandler

                PUBWEAK INTUART3ERR_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUART3ERR_IRQHandler
                B       INTUART3ERR_IRQHandler

                PUBWEAK INTUART4RX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUART4RX_IRQHandler
                B       INTUART4RX_IRQHandler

                PUBWEAK INTUART4TX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUART4TX_IRQHandler
                B       INTUART4TX_IRQHandler

                PUBWEAK INTUART4ERR_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUART4ERR_IRQHandler
                B       INTUART4ERR_IRQHandler

                PUBWEAK INTUART5RX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUART5RX_IRQHandler
                B       INTUART5RX_IRQHandler

                PUBWEAK INTUART5TX_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUART5TX_IRQHandler
                B       INTUART5TX_IRQHandler

                PUBWEAK INTUART5ERR_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTUART5ERR_IRQHandler
                B       INTUART5ERR_IRQHandler

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

                PUBWEAK INTT32A06A_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A06A_IRQHandler
                B       INTT32A06A_IRQHandler

                PUBWEAK INTT32A06ACAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A06ACAP0_IRQHandler
                B       INTT32A06ACAP0_IRQHandler

                PUBWEAK INTT32A06ACAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A06ACAP1_IRQHandler
                B       INTT32A06ACAP1_IRQHandler

                PUBWEAK INTT32A06B_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A06B_IRQHandler
                B       INTT32A06B_IRQHandler

                PUBWEAK INTT32A06BCAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A06BCAP0_IRQHandler
                B       INTT32A06BCAP0_IRQHandler

                PUBWEAK INTT32A06BCAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A06BCAP1_IRQHandler
                B       INTT32A06BCAP1_IRQHandler

                PUBWEAK INTT32A06C_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A06C_IRQHandler
                B       INTT32A06C_IRQHandler

                PUBWEAK INTT32A06CCAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A06CCAP0_IRQHandler
                B       INTT32A06CCAP0_IRQHandler

                PUBWEAK INTT32A06CCAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A06CCAP1_IRQHandler
                B       INTT32A06CCAP1_IRQHandler

                PUBWEAK INTT32A07A_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A07A_IRQHandler
                B       INTT32A07A_IRQHandler

                PUBWEAK INTT32A07ACAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A07ACAP0_IRQHandler
                B       INTT32A07ACAP0_IRQHandler

                PUBWEAK INTT32A07ACAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A07ACAP1_IRQHandler
                B       INTT32A07ACAP1_IRQHandler

                PUBWEAK INTT32A07B_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A07B_IRQHandler
                B       INTT32A07B_IRQHandler

                PUBWEAK INTT32A07BCAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A07BCAP0_IRQHandler
                B       INTT32A07BCAP0_IRQHandler

                PUBWEAK INTT32A07BCAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A07BCAP1_IRQHandler
                B       INTT32A07BCAP1_IRQHandler

                PUBWEAK INTT32A07C_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A07C_IRQHandler
                B       INTT32A07C_IRQHandler

                PUBWEAK INTT32A07CCAP0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A07CCAP0_IRQHandler
                B       INTT32A07CCAP0_IRQHandler

                PUBWEAK INTT32A07CCAP1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTT32A07CCAP1_IRQHandler
                B       INTT32A07CCAP1_IRQHandler

                PUBWEAK INTPARI_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTPARI_IRQHandler
                B       INTPARI_IRQHandler

                PUBWEAK INTDMAATC_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTDMAATC_IRQHandler
                B       INTDMAATC_IRQHandler

                PUBWEAK INTDMAAERR_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTDMAAERR_IRQHandler
                B       INTDMAAERR_IRQHandler

                PUBWEAK INTDMABTC_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTDMABTC_IRQHandler
                B       INTDMABTC_IRQHandler

                PUBWEAK INTDMABERR_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
INTDMABERR_IRQHandler
                B       INTDMABERR_IRQHandler

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
