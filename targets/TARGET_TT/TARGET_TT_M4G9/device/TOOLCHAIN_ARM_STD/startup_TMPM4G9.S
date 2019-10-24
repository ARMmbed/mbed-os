;/**
; *******************************************************************************
; * @file    startup_TMPM4G9.s
; * @brief   CMSIS Cortex-M4 Core Device Startup File for the
; *          TOSHIBA 'TMPM4G9' Device Series
; * @version V1.0.6.0
; * $Date:: 2017-10-04 #$
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; * 
; * SPDX-License-Identifier: Apache-2.0
; * 
; * (C)Copyright TOSHIBA MICROELECTRONICS CORPORATION 2017 All rights reserved
; *******************************************************************************
; */


                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                IMPORT  |Image$$ARM_LIB_STACK$$ZI$$Limit|

__Vectors       DCD     |Image$$ARM_LIB_STACK$$ZI$$Limit| ; Top of Stack
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
                DCD     INT00_IRQHandler          ; 0:	Interrupt pin 00a/00b
                DCD     INT01_IRQHandler          ; 1:	Interrupt pin 01a/00b
                DCD     INT02_IRQHandler          ; 2:	Interrupt pin 02a/00b
                DCD     INT03_IRQHandler          ; 3:	Interrupt pin 03a/03b
                DCD     INT04_IRQHandler          ; 4:	Interrupt pin 04a/04b
                DCD     INT05_IRQHandler          ; 5:	Interrupt pin 05a/05b
                DCD     INT06_IRQHandler          ; 6:	Interrupt pin 06a/06b
                DCD     INT07_IRQHandler          ; 7:	Interrupt pin 07a/07b
                DCD     INT08_IRQHandler          ; 8:	Interrupt pin 08a/08b
                DCD     INT09_IRQHandler          ; 9:	Interrupt pin 09a/09b
                DCD     INT10_IRQHandler          ; 10:	Interrupt pin 10a/10b
                DCD     INT11_IRQHandler          ; 11:	Interrupt pin 11a/11b
                DCD     INT12_IRQHandler          ; 12:	Interrupt pin 12a/12b
                DCD     INT13_IRQHandler          ; 13:	Interrupt pin 13a/13b
                DCD     INT14_IRQHandler          ; 14:	Interrupt pin 14a/14b
                DCD     INT15_IRQHandler          ; 15:	Interrupt pin 15a/15b
                DCD     INTRTC_IRQHandler         ; 16:	Real time clock(XHz) interrupt
                DCD     INTCEC0RX_IRQHandler      ; 17:	CEC reception interrupt (channel 0)
                DCD     INTCEC0TX_IRQHandler      ; 18:	CEC transmission interrupt (channel 0)
                DCD     INTISDA_IRQHandler        ; 19:	Interval Sensing Detector Interrupt (Unit A)
                DCD     INTISDB_IRQHandler        ; 20:	Interval Sensing Detector Interrupt (Unit B)
                DCD     INTISDC_IRQHandler        ; 21:	Interval Sensing Detector Interrupt (Unit C)
                DCD     INTRMC0_IRQHandler        ; 22:	Remote control reception interrupt 0
                DCD     INTRMC1_IRQHandler        ; 23:	Remote control reception interrupt 1
                DCD     INTLTTMR_IRQHandler       ; 24:	Long Term Timer Interrupt
                DCD     INTHDMAATC_IRQHandler     ; 25:	HDMA Complete of transfer(Unit A)
                DCD     INTHDMAAERR_IRQHandler    ; 26:	HDMA transfer error(Unit A)
                DCD     INTHDMABTC_IRQHandler     ; 27:	HDMA end of transfer(Unit B)
                DCD     INTHDMABERR_IRQHandler    ; 28:	HDMA transfer error(Unit B)
                DCD     INTMDMAATC_IRQHandler     ; 29:	MDMA Complete of transfer(Unit A)
                DCD     INTT32A00_A_CT_IRQHandler ; 30:	T32A00 TimerA All Interrupt/Timer Interrupt C
                DCD     INTT32A00_B_C01_CPC_IRQHandler; 31:	T32A00 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C
                DCD     INTT32A01_A_CT_IRQHandler ; 32:	T32A01 TimerA All Interrupt/Timer Interrupt C
                DCD     INTT32A01_B_C01_CPC_IRQHandler; 33:	T32A01 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C
                DCD     INTT32A02_A_CT_IRQHandler ; 34:	T32A02 TimerA All Interrupt/Timer Interrupt C
                DCD     INTT32A02_B_C01_CPC_IRQHandler; 35:	T32A02 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C
                DCD     INTT32A03_A_CT_IRQHandler ; 36:	T32A03 TimerA All Interrupt/Timer Interrupt C
                DCD     INTT32A03_B_C01_CPC_IRQHandler; 37:	T32A03 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C
                DCD     INTT32A04_A_CT_IRQHandler ; 38:	T32A04 TimerA All Interrupt/Timer Interrupt C
                DCD     INTT32A04_B_C01_CPC_IRQHandler; 39:	T32A04 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C
                DCD     INTT32A05_A_CT_IRQHandler ; 40:	T32A05 TimerA All Interrupt/Timer Interrupt C
                DCD     INTT32A05_B_C01_CPC_IRQHandler; 41:	T32A05 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C
                DCD     INTT32A06_A_CT_IRQHandler ; 42:	T32A06 TimerA All Interrupt/Timer Interrupt C
                DCD     INTT32A06_B_C01_CPC_IRQHandler; 43:	T32A06 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C
                DCD     INTT32A07_A_CT_IRQHandler ; 44:	T32A07 TimerA All Interrupt/Timer Interrupt C
                DCD     INTT32A07_B_C01_CPC_IRQHandler; 45:	T32A07 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C
                DCD     INTT32A08_A_CT_IRQHandler ; 46:	T32A08 TimerA All Interrupt/Timer Interrupt C
                DCD     INTT32A08_B_C01_CPC_IRQHandler; 47:	T32A08 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C
                DCD     INTT32A09_A_CT_IRQHandler ; 48:	T32A09 TimerA All Interrupt/Timer Interrupt C
                DCD     INTT32A09_B_C01_CPC_IRQHandler; 49:	T32A09 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C
                DCD     INTT32A10_A_CT_IRQHandler ; 50:	T32A10 TimerA All Interrupt/Timer Interrupt C
                DCD     INTT32A10_B_C01_CPC_IRQHandler; 51:	T32A10 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C
                DCD     INTT32A11_A_CT_IRQHandler ; 52:	T32A11 TimerA All Interrupt/Timer Interrupt C
                DCD     INTT32A11_B_C01_CPC_IRQHandler; 53:	T32A11 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C
                DCD     INTT32A12_A_CT_IRQHandler ; 54:	T32A12 TimerA All Interrupt/Timer Interrupt C
                DCD     INTT32A12_B_C01_CPC_IRQHandler; 55:	T32A12 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C
                DCD     INTT32A13_A_CT_IRQHandler ; 56:	T32A13 TimerA All Interrupt/Timer Interrupt C
                DCD     INTT32A13_B_C01_CPC_IRQHandler; 57:	T32A13 Timer Interrupt B/Capture C0,C1 Interrupt/ Timer Interrupt C
                DCD     INTEMG0_IRQHandler        ; 58:	PMD0 EMG interrupt
                DCD     INTOVV0_IRQHandler        ; 59:	PMD0 OVV interrupt
                DCD     INTPWM0_IRQHandler        ; 60:	PMD0 interrupt
                DCD     INTT0RX_IRQHandler        ; 61:	TSPI/SIO reception (channel 0)
                DCD     INTT0TX_IRQHandler        ; 62:	TSPI/SIO transmit (channel 0)
                DCD     INTT0ERR_IRQHandler       ; 63:	TSPI/SIO error (channel 0)
                DCD     INTT1RX_IRQHandler        ; 64:	TSPI/SIO reception (channel 1)
                DCD     INTT1TX_IRQHandler        ; 65:	TSPI/SIO transmit (channel 1)
                DCD     INTT1ERR_IRQHandler       ; 66:	TSPI/SIO error (channel 1)
                DCD     INTT2RX_IRQHandler        ; 67:	TSPI/SIO reception (channel 2)
                DCD     INTT2TX_IRQHandler        ; 68:	TSPI/SIO transmit (channel 2)
                DCD     INTT2ERR_IRQHandler       ; 69:	TSPI/SIO error (channel 2)
                DCD     INTT3RX_IRQHandler        ; 70:	TSPI/SIO reception (channel 3)
                DCD     INTT3TX_IRQHandler        ; 71:	TSPI/SIO transmit (channel 3)
                DCD     INTT3ERR_IRQHandler       ; 72:	TSPI/SIO error (channel 3)
                DCD     INTT4RX_IRQHandler        ; 73:	TSPI/SIO reception (channel 4)
                DCD     INTT4TX_IRQHandler        ; 74:	TSPI/SIO transmit (channel 4)
                DCD     INTT4ERR_IRQHandler       ; 75:	TSPI/SIO error (channel 4)
                DCD     INTT5RX_IRQHandler        ; 76:	TSPI/SIO reception (channel 5)
                DCD     INTT5TX_IRQHandler        ; 77:	TSPI/SIO transmit (channel 5)
                DCD     INTT5ERR_IRQHandler       ; 78:	TSPI/SIO error (channel 5)
                DCD     INTT6RX_IRQHandler        ; 79:	TSPI/SIO reception (channel 6)
                DCD     INTT6TX_IRQHandler        ; 80:	TSPI/SIO transmit (channel 6)
                DCD     INTT6ERR_IRQHandler       ; 81:	TSPI/SIO error (channel 6)
                DCD     INTT7RX_IRQHandler        ; 82:	TSPI/SIO reception (channel 7)
                DCD     INTT7TX_IRQHandler        ; 83:	TSPI/SIO transmit (channel 7)
                DCD     INTT7ERR_IRQHandler       ; 84:	TSPI/SIO error (channel 7)
                DCD     INTT8RX_IRQHandler        ; 85:	TSPI/SIO reception (channel 8)
                DCD     INTT8TX_IRQHandler        ; 86:	TSPI/SIO transmit (channel 8)
                DCD     INTT8ERR_IRQHandler       ; 87:	TSPI/SIO error (channel 8)
                DCD     INTSMI0_IRQHandler        ; 88:	Serial Memory Interface Interrupt
                DCD     INTUART0RX_IRQHandler     ; 89:	UART reception (channel 0)
                DCD     INTUART0TX_IRQHandler     ; 90:	UART transmit (channel 0)
                DCD     INTUART0ERR_IRQHandler    ; 91:	UART error (channel 0)
                DCD     INTUART1RX_IRQHandler     ; 92:	UART reception (channel 1)
                DCD     INTUART1TX_IRQHandler     ; 93:	UART transmit (channel 1)
                DCD     INTUART1ERR_IRQHandler    ; 94:	UART error (channel 1)
                DCD     INTUART2RX_IRQHandler     ; 95:	UART reception (channel 2)
                DCD     INTUART2TX_IRQHandler     ; 96:	UART transmit (channel 2)
                DCD     INTUART2ERR_IRQHandler    ; 97:	UART error (channel 2)
                DCD     INTUART3RX_IRQHandler     ; 98:	UART reception (channel 3)
                DCD     INTUART3TX_IRQHandler     ; 99:	UART transmit (channel 3)
                DCD     INTUART3ERR_IRQHandler    ; 100:	UART error (channel 3)
                DCD     INTUART4RX_IRQHandler     ; 101:	UART reception (channel 4)
                DCD     INTUART4TX_IRQHandler     ; 102:	UART transmit (channel 4)
                DCD     INTUART4ERR_IRQHandler    ; 103:	UART error (channel 4)
                DCD     INTUART5RX_IRQHandler     ; 104:	UART reception (channel 5)
                DCD     INTUART5TX_IRQHandler     ; 105:	UART transmit (channel 5)
                DCD     INTUART5ERR_IRQHandler    ; 106:	UART error (channel 5)
                DCD     INTFUART0_IRQHandler      ; 107:	FUART Interrupt(channel 0)
                DCD     INTFUART1_IRQHandler      ; 108:	FUART Interrupt(channel 1)
                DCD     INTI2C0_IRQHandler        ; 109:	I2C0 transmission and reception interrupt
                DCD     INTI2C0AL_IRQHandler      ; 110:	I2C0 arbitration lost interrupt
                DCD     INTI2C0BF_IRQHandler      ; 111:	I2C0 bus free interrupt
                DCD     INTI2C0NACK_IRQHandler    ; 112:	I2C0 no ack interrupt
                DCD     INTI2C1_IRQHandler        ; 113:	I2C1 transmission and reception interrupt
                DCD     INTI2C1AL_IRQHandler      ; 114:	I2C1 arbitration lost interrupt
                DCD     INTI2C1BF_IRQHandler      ; 115:	I2C1 bus free interrupt
                DCD     INTI2C1NACK_IRQHandler    ; 116:	I2C1 no ack interrupt
                DCD     INTI2C2_IRQHandler        ; 117:	I2C2 transmission and reception interrupt
                DCD     INTI2C2AL_IRQHandler      ; 118:	I2C2 arbitration lost interrupt
                DCD     INTI2C2BF_IRQHandler      ; 119:	I2C2 bus free interrupt
                DCD     INTI2C2NACK_IRQHandler    ; 120:	I2C2 no ack interrupt
                DCD     INTI2C3_IRQHandler        ; 121:	I2C3 transmission and reception interrupt
                DCD     INTI2C3AL_IRQHandler      ; 122:	I2C3 arbitration lost interrupt
                DCD     INTI2C3BF_IRQHandler      ; 123:	I2C3 bus free interrupt
                DCD     INTI2C3NACK_IRQHandler    ; 124:	I2C3 no ack interrupt
                DCD     INTI2C4_IRQHandler        ; 125:	I2C4 transmission and reception interrupt
                DCD     INTI2C4AL_IRQHandler      ; 126:	I2C4 arbitration lost interrupt
                DCD     INTI2C4BF_IRQHandler      ; 127:	I2C4 bus free interrupt
                DCD     INTI2C4NACK_IRQHandler    ; 128:	I2C4 no ack interrupt
                DCD     INTADACP0_IRQHandler      ; 129:	ADC conversion monitoring function interrupt 0
                DCD     INTADACP1_IRQHandler      ; 130:	ADC conversion monitoring function interrupt 1
                DCD     INTADATRG_IRQHandler      ; 131:	ADC conversion triggered by General purpose is finished
                DCD     INTADASGL_IRQHandler      ; 132:	ADC conversion triggered by Single program is finished
                DCD     INTADACNT_IRQHandler      ; 133:	ADC conversion triggered by Continuity program is finished
                DCD     INTADAHP_IRQHandler       ; 134:	ADC High Priority AD conversion interrupt
                DCD     INTFLDRDY_IRQHandler      ; 135:	Data FLASH Ready interrupt
                DCD     INTFLCRDY0_IRQHandler     ; 136:	Code FLASH Area0/1 Ready interrupt
                DCD     INTFLCRDY1_IRQHandler     ; 137:	Code FLASH Area2 Ready interrupt
                DCD     0                         ; 138:	Reserved
                DCD     INTMDMAABERR_IRQHandler   ; 139:	MDMA bus error(Unit A)
                DCD     INTMDMAADERR_IRQHandler   ; 140:	MDMA descriptor error(Unit A)

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

                EXPORT  INT00_IRQHandler          [WEAK]
                EXPORT  INT01_IRQHandler          [WEAK]
                EXPORT  INT02_IRQHandler          [WEAK]
                EXPORT  INT03_IRQHandler          [WEAK]
                EXPORT  INT04_IRQHandler          [WEAK]
                EXPORT  INT05_IRQHandler          [WEAK]
                EXPORT  INT06_IRQHandler          [WEAK]
                EXPORT  INT07_IRQHandler          [WEAK]
                EXPORT  INT08_IRQHandler          [WEAK]
                EXPORT  INT09_IRQHandler          [WEAK]
                EXPORT  INT10_IRQHandler          [WEAK]
                EXPORT  INT11_IRQHandler          [WEAK]
                EXPORT  INT12_IRQHandler          [WEAK]
                EXPORT  INT13_IRQHandler          [WEAK]
                EXPORT  INT14_IRQHandler          [WEAK]
                EXPORT  INT15_IRQHandler          [WEAK]
                EXPORT  INTRTC_IRQHandler         [WEAK]
                EXPORT  INTCEC0RX_IRQHandler      [WEAK]
                EXPORT  INTCEC0TX_IRQHandler      [WEAK]
                EXPORT  INTISDA_IRQHandler        [WEAK]
                EXPORT  INTISDB_IRQHandler        [WEAK]
                EXPORT  INTISDC_IRQHandler        [WEAK]
                EXPORT  INTRMC0_IRQHandler        [WEAK]
                EXPORT  INTRMC1_IRQHandler        [WEAK]
                EXPORT  INTLTTMR_IRQHandler       [WEAK]
                EXPORT  INTHDMAATC_IRQHandler     [WEAK]
                EXPORT  INTHDMAAERR_IRQHandler    [WEAK]
                EXPORT  INTHDMABTC_IRQHandler     [WEAK]
                EXPORT  INTHDMABERR_IRQHandler    [WEAK]
                EXPORT  INTMDMAATC_IRQHandler     [WEAK]
                EXPORT  INTT32A00_A_CT_IRQHandler [WEAK]
                EXPORT  INTT32A00_B_C01_CPC_IRQHandler[WEAK]
                EXPORT  INTT32A01_A_CT_IRQHandler [WEAK]
                EXPORT  INTT32A01_B_C01_CPC_IRQHandler[WEAK]
                EXPORT  INTT32A02_A_CT_IRQHandler [WEAK]
                EXPORT  INTT32A02_B_C01_CPC_IRQHandler[WEAK]
                EXPORT  INTT32A03_A_CT_IRQHandler [WEAK]
                EXPORT  INTT32A03_B_C01_CPC_IRQHandler[WEAK]
                EXPORT  INTT32A04_A_CT_IRQHandler [WEAK]
                EXPORT  INTT32A04_B_C01_CPC_IRQHandler[WEAK]
                EXPORT  INTT32A05_A_CT_IRQHandler [WEAK]
                EXPORT  INTT32A05_B_C01_CPC_IRQHandler[WEAK]
                EXPORT  INTT32A06_A_CT_IRQHandler [WEAK]
                EXPORT  INTT32A06_B_C01_CPC_IRQHandler[WEAK]
                EXPORT  INTT32A07_A_CT_IRQHandler [WEAK]
                EXPORT  INTT32A07_B_C01_CPC_IRQHandler[WEAK]
                EXPORT  INTT32A08_A_CT_IRQHandler [WEAK]
                EXPORT  INTT32A08_B_C01_CPC_IRQHandler[WEAK]
                EXPORT  INTT32A09_A_CT_IRQHandler [WEAK]
                EXPORT  INTT32A09_B_C01_CPC_IRQHandler[WEAK]
                EXPORT  INTT32A10_A_CT_IRQHandler [WEAK]
                EXPORT  INTT32A10_B_C01_CPC_IRQHandler[WEAK]
                EXPORT  INTT32A11_A_CT_IRQHandler [WEAK]
                EXPORT  INTT32A11_B_C01_CPC_IRQHandler[WEAK]
                EXPORT  INTT32A12_A_CT_IRQHandler [WEAK]
                EXPORT  INTT32A12_B_C01_CPC_IRQHandler[WEAK]
                EXPORT  INTT32A13_A_CT_IRQHandler [WEAK]
                EXPORT  INTT32A13_B_C01_CPC_IRQHandler[WEAK]
                EXPORT  INTEMG0_IRQHandler        [WEAK]
                EXPORT  INTOVV0_IRQHandler        [WEAK]
                EXPORT  INTPWM0_IRQHandler        [WEAK]
                EXPORT  INTT0RX_IRQHandler        [WEAK]
                EXPORT  INTT0TX_IRQHandler        [WEAK]
                EXPORT  INTT0ERR_IRQHandler       [WEAK]
                EXPORT  INTT1RX_IRQHandler        [WEAK]
                EXPORT  INTT1TX_IRQHandler        [WEAK]
                EXPORT  INTT1ERR_IRQHandler       [WEAK]
                EXPORT  INTT2RX_IRQHandler        [WEAK]
                EXPORT  INTT2TX_IRQHandler        [WEAK]
                EXPORT  INTT2ERR_IRQHandler       [WEAK]
                EXPORT  INTT3RX_IRQHandler        [WEAK]
                EXPORT  INTT3TX_IRQHandler        [WEAK]
                EXPORT  INTT3ERR_IRQHandler       [WEAK]
                EXPORT  INTT4RX_IRQHandler        [WEAK]
                EXPORT  INTT4TX_IRQHandler        [WEAK]
                EXPORT  INTT4ERR_IRQHandler       [WEAK]
                EXPORT  INTT5RX_IRQHandler        [WEAK]
                EXPORT  INTT5TX_IRQHandler        [WEAK]
                EXPORT  INTT5ERR_IRQHandler       [WEAK]
                EXPORT  INTT6RX_IRQHandler        [WEAK]
                EXPORT  INTT6TX_IRQHandler        [WEAK]
                EXPORT  INTT6ERR_IRQHandler       [WEAK]
                EXPORT  INTT7RX_IRQHandler        [WEAK]
                EXPORT  INTT7TX_IRQHandler        [WEAK]
                EXPORT  INTT7ERR_IRQHandler       [WEAK]
                EXPORT  INTT8RX_IRQHandler        [WEAK]
                EXPORT  INTT8TX_IRQHandler        [WEAK]
                EXPORT  INTT8ERR_IRQHandler       [WEAK]
                EXPORT  INTSMI0_IRQHandler        [WEAK]
                EXPORT  INTUART0RX_IRQHandler     [WEAK]
                EXPORT  INTUART0TX_IRQHandler     [WEAK]
                EXPORT  INTUART0ERR_IRQHandler    [WEAK]
                EXPORT  INTUART1RX_IRQHandler     [WEAK]
                EXPORT  INTUART1TX_IRQHandler     [WEAK]
                EXPORT  INTUART1ERR_IRQHandler    [WEAK]
                EXPORT  INTUART2RX_IRQHandler     [WEAK]
                EXPORT  INTUART2TX_IRQHandler     [WEAK]
                EXPORT  INTUART2ERR_IRQHandler    [WEAK]
                EXPORT  INTUART3RX_IRQHandler     [WEAK]
                EXPORT  INTUART3TX_IRQHandler     [WEAK]
                EXPORT  INTUART3ERR_IRQHandler    [WEAK]
                EXPORT  INTUART4RX_IRQHandler     [WEAK]
                EXPORT  INTUART4TX_IRQHandler     [WEAK]
                EXPORT  INTUART4ERR_IRQHandler    [WEAK]
                EXPORT  INTUART5RX_IRQHandler     [WEAK]
                EXPORT  INTUART5TX_IRQHandler     [WEAK]
                EXPORT  INTUART5ERR_IRQHandler    [WEAK]
                EXPORT  INTFUART0_IRQHandler      [WEAK]
                EXPORT  INTFUART1_IRQHandler      [WEAK]
                EXPORT  INTI2C0_IRQHandler        [WEAK]
                EXPORT  INTI2C0AL_IRQHandler      [WEAK]
                EXPORT  INTI2C0BF_IRQHandler      [WEAK]
                EXPORT  INTI2C0NACK_IRQHandler    [WEAK]
                EXPORT  INTI2C1_IRQHandler        [WEAK]
                EXPORT  INTI2C1AL_IRQHandler      [WEAK]
                EXPORT  INTI2C1BF_IRQHandler      [WEAK]
                EXPORT  INTI2C1NACK_IRQHandler    [WEAK]
                EXPORT  INTI2C2_IRQHandler        [WEAK]
                EXPORT  INTI2C2AL_IRQHandler      [WEAK]
                EXPORT  INTI2C2BF_IRQHandler      [WEAK]
                EXPORT  INTI2C2NACK_IRQHandler    [WEAK]
                EXPORT  INTI2C3_IRQHandler        [WEAK]
                EXPORT  INTI2C3AL_IRQHandler      [WEAK]
                EXPORT  INTI2C3BF_IRQHandler      [WEAK]
                EXPORT  INTI2C3NACK_IRQHandler    [WEAK]
                EXPORT  INTI2C4_IRQHandler        [WEAK]
                EXPORT  INTI2C4AL_IRQHandler      [WEAK]
                EXPORT  INTI2C4BF_IRQHandler      [WEAK]
                EXPORT  INTI2C4NACK_IRQHandler    [WEAK]
                EXPORT  INTADACP0_IRQHandler      [WEAK]
                EXPORT  INTADACP1_IRQHandler      [WEAK]
                EXPORT  INTADATRG_IRQHandler      [WEAK]
                EXPORT  INTADASGL_IRQHandler      [WEAK]
                EXPORT  INTADACNT_IRQHandler      [WEAK]
                EXPORT  INTADAHP_IRQHandler       [WEAK]
                EXPORT  INTFLDRDY_IRQHandler      [WEAK]
                EXPORT  INTFLCRDY0_IRQHandler     [WEAK]
                EXPORT  INTFLCRDY1_IRQHandler     [WEAK]
                EXPORT  INTMDMAABERR_IRQHandler   [WEAK]
                EXPORT  INTMDMAADERR_IRQHandler   [WEAK]

INT00_IRQHandler
INT01_IRQHandler
INT02_IRQHandler
INT03_IRQHandler
INT04_IRQHandler
INT05_IRQHandler
INT06_IRQHandler
INT07_IRQHandler
INT08_IRQHandler
INT09_IRQHandler
INT10_IRQHandler
INT11_IRQHandler
INT12_IRQHandler
INT13_IRQHandler
INT14_IRQHandler
INT15_IRQHandler
INTRTC_IRQHandler
INTCEC0RX_IRQHandler
INTCEC0TX_IRQHandler
INTISDA_IRQHandler
INTISDB_IRQHandler
INTISDC_IRQHandler
INTRMC0_IRQHandler
INTRMC1_IRQHandler
INTLTTMR_IRQHandler
INTHDMAATC_IRQHandler
INTHDMAAERR_IRQHandler
INTHDMABTC_IRQHandler
INTHDMABERR_IRQHandler
INTMDMAATC_IRQHandler
INTT32A00_A_CT_IRQHandler
INTT32A00_B_C01_CPC_IRQHandler
INTT32A01_A_CT_IRQHandler
INTT32A01_B_C01_CPC_IRQHandler
INTT32A02_A_CT_IRQHandler
INTT32A02_B_C01_CPC_IRQHandler
INTT32A03_A_CT_IRQHandler
INTT32A03_B_C01_CPC_IRQHandler
INTT32A04_A_CT_IRQHandler
INTT32A04_B_C01_CPC_IRQHandler
INTT32A05_A_CT_IRQHandler
INTT32A05_B_C01_CPC_IRQHandler
INTT32A06_A_CT_IRQHandler
INTT32A06_B_C01_CPC_IRQHandler
INTT32A07_A_CT_IRQHandler
INTT32A07_B_C01_CPC_IRQHandler
INTT32A08_A_CT_IRQHandler
INTT32A08_B_C01_CPC_IRQHandler
INTT32A09_A_CT_IRQHandler
INTT32A09_B_C01_CPC_IRQHandler
INTT32A10_A_CT_IRQHandler
INTT32A10_B_C01_CPC_IRQHandler
INTT32A11_A_CT_IRQHandler
INTT32A11_B_C01_CPC_IRQHandler
INTT32A12_A_CT_IRQHandler
INTT32A12_B_C01_CPC_IRQHandler
INTT32A13_A_CT_IRQHandler
INTT32A13_B_C01_CPC_IRQHandler
INTEMG0_IRQHandler
INTOVV0_IRQHandler
INTPWM0_IRQHandler
INTT0RX_IRQHandler
INTT0TX_IRQHandler
INTT0ERR_IRQHandler
INTT1RX_IRQHandler
INTT1TX_IRQHandler
INTT1ERR_IRQHandler
INTT2RX_IRQHandler
INTT2TX_IRQHandler
INTT2ERR_IRQHandler
INTT3RX_IRQHandler
INTT3TX_IRQHandler
INTT3ERR_IRQHandler
INTT4RX_IRQHandler
INTT4TX_IRQHandler
INTT4ERR_IRQHandler
INTT5RX_IRQHandler
INTT5TX_IRQHandler
INTT5ERR_IRQHandler
INTT6RX_IRQHandler
INTT6TX_IRQHandler
INTT6ERR_IRQHandler
INTT7RX_IRQHandler
INTT7TX_IRQHandler
INTT7ERR_IRQHandler
INTT8RX_IRQHandler
INTT8TX_IRQHandler
INTT8ERR_IRQHandler
INTSMI0_IRQHandler
INTUART0RX_IRQHandler
INTUART0TX_IRQHandler
INTUART0ERR_IRQHandler
INTUART1RX_IRQHandler
INTUART1TX_IRQHandler
INTUART1ERR_IRQHandler
INTUART2RX_IRQHandler
INTUART2TX_IRQHandler
INTUART2ERR_IRQHandler
INTUART3RX_IRQHandler
INTUART3TX_IRQHandler
INTUART3ERR_IRQHandler
INTUART4RX_IRQHandler
INTUART4TX_IRQHandler
INTUART4ERR_IRQHandler
INTUART5RX_IRQHandler
INTUART5TX_IRQHandler
INTUART5ERR_IRQHandler
INTFUART0_IRQHandler
INTFUART1_IRQHandler
INTI2C0_IRQHandler
INTI2C0AL_IRQHandler
INTI2C0BF_IRQHandler
INTI2C0NACK_IRQHandler
INTI2C1_IRQHandler
INTI2C1AL_IRQHandler
INTI2C1BF_IRQHandler
INTI2C1NACK_IRQHandler
INTI2C2_IRQHandler
INTI2C2AL_IRQHandler
INTI2C2BF_IRQHandler
INTI2C2NACK_IRQHandler
INTI2C3_IRQHandler
INTI2C3AL_IRQHandler
INTI2C3BF_IRQHandler
INTI2C3NACK_IRQHandler
INTI2C4_IRQHandler
INTI2C4AL_IRQHandler
INTI2C4BF_IRQHandler
INTI2C4NACK_IRQHandler
INTADACP0_IRQHandler
INTADACP1_IRQHandler
INTADATRG_IRQHandler
INTADASGL_IRQHandler
INTADACNT_IRQHandler
INTADAHP_IRQHandler
INTFLDRDY_IRQHandler
INTFLCRDY0_IRQHandler
INTFLCRDY1_IRQHandler
INTMDMAABERR_IRQHandler
INTMDMAADERR_IRQHandler

                B       .

                ENDP

                ALIGN
                END
