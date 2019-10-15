;/**
; *******************************************************************************
; * @file    startup_TMPM3HQ.s
; * @brief   CMSIS Cortex-M3 Core Device Startup File for the
; *          TOSHIBA 'TMPM3HQ' Device Series
; * @version V1.0.0.0
; * $Date:: 2017-12-08 #$
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; * 
; * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
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
                EXPORT  INT16_IRQHandler          [WEAK]
                EXPORT  INT17_18_IRQHandler       [WEAK]
                EXPORT  INT19_22_IRQHandler       [WEAK]
                EXPORT  INT23_26_IRQHandler       [WEAK]
                EXPORT  INT27_28_IRQHandler       [WEAK]
                EXPORT  INT29_IRQHandler          [WEAK]
                EXPORT  INT30_31_IRQHandler       [WEAK]
                EXPORT  INTEMG0_IRQHandler        [WEAK]
                EXPORT  INTOVV0_IRQHandler        [WEAK]
                EXPORT  INTPMD0_IRQHandler        [WEAK]
                EXPORT  INTENC00_IRQHandler       [WEAK]
                EXPORT  INTENC01_IRQHandler       [WEAK]
                EXPORT  INTADAPDA_IRQHandler      [WEAK]
                EXPORT  INTADAPDB_IRQHandler      [WEAK]
                EXPORT  INTADACP0_IRQHandler      [WEAK]
                EXPORT  INTADACP1_IRQHandler      [WEAK]
                EXPORT  INTADATRG_IRQHandler      [WEAK]
                EXPORT  INTADASGL_IRQHandler      [WEAK]
                EXPORT  INTADACNT_IRQHandler      [WEAK]
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
                EXPORT  INTI2CWUP_IRQHandler      [WEAK]
                EXPORT  INTI2C0_IRQHandler        [WEAK]
                EXPORT  INTI2C0AL_IRQHandler      [WEAK]
                EXPORT  INTI2C0BF_IRQHandler      [WEAK]
                EXPORT  INTI2C0NA_IRQHandler      [WEAK]
                EXPORT  INTI2C1_IRQHandler        [WEAK]
                EXPORT  INTI2C1AL_IRQHandler      [WEAK]
                EXPORT  INTI2C1BF_IRQHandler      [WEAK]
                EXPORT  INTI2C1NA_IRQHandler      [WEAK]
                EXPORT  INTI2C2_IRQHandler        [WEAK]
                EXPORT  INTI2C2AL_IRQHandler      [WEAK]
                EXPORT  INTI2C2BF_IRQHandler      [WEAK]
                EXPORT  INTI2C2NA_IRQHandler      [WEAK]
                EXPORT  INTI2C3_IRQHandler        [WEAK]
                EXPORT  INTI2C3AL_IRQHandler      [WEAK]
                EXPORT  INTI2C3BF_IRQHandler      [WEAK]
                EXPORT  INTI2C3NA_IRQHandler      [WEAK]
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
                EXPORT  INTT32A00A_IRQHandler     [WEAK]
                EXPORT  INTT32A00ACAP0_IRQHandler [WEAK]
                EXPORT  INTT32A00ACAP1_IRQHandler [WEAK]
                EXPORT  INTT32A00B_IRQHandler     [WEAK]
                EXPORT  INTT32A00BCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A00BCAP1_IRQHandler [WEAK]
                EXPORT  INTT32A00C_IRQHandler     [WEAK]
                EXPORT  INTT32A00CCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A00CCAP1_IRQHandler [WEAK]
                EXPORT  INTT32A01A_IRQHandler     [WEAK]
                EXPORT  INTT32A01ACAP0_IRQHandler [WEAK]
                EXPORT  INTT32A01ACAP1_IRQHandler [WEAK]
                EXPORT  INTT32A01B_IRQHandler     [WEAK]
                EXPORT  INTT32A01BCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A01BCAP1_IRQHandler [WEAK]
                EXPORT  INTT32A01C_IRQHandler     [WEAK]
                EXPORT  INTT32A01CCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A01CCAP1_IRQHandler [WEAK]
                EXPORT  INTT32A02A_IRQHandler     [WEAK]
                EXPORT  INTT32A02ACAP0_IRQHandler [WEAK]
                EXPORT  INTT32A02ACAP1_IRQHandler [WEAK]
                EXPORT  INTT32A02B_IRQHandler     [WEAK]
                EXPORT  INTT32A02BCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A02BCAP1_IRQHandler [WEAK]
                EXPORT  INTT32A02C_IRQHandler     [WEAK]
                EXPORT  INTT32A02CCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A02CCAP1_IRQHandler [WEAK]
                EXPORT  INTT32A03A_IRQHandler     [WEAK]
                EXPORT  INTT32A03ACAP0_IRQHandler [WEAK]
                EXPORT  INTT32A03ACAP1_IRQHandler [WEAK]
                EXPORT  INTT32A03B_IRQHandler     [WEAK]
                EXPORT  INTT32A03BCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A03BCAP1_IRQHandler [WEAK]
                EXPORT  INTT32A03C_IRQHandler     [WEAK]
                EXPORT  INTT32A03CCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A03CCAP1_IRQHandler [WEAK]
                EXPORT  INTT32A04A_IRQHandler     [WEAK]
                EXPORT  INTT32A04ACAP0_IRQHandler [WEAK]
                EXPORT  INTT32A04ACAP1_IRQHandler [WEAK]
                EXPORT  INTT32A04B_IRQHandler     [WEAK]
                EXPORT  INTT32A04BCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A04BCAP1_IRQHandler [WEAK]
                EXPORT  INTT32A04C_IRQHandler     [WEAK]
                EXPORT  INTT32A04CCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A04CCAP1_IRQHandler [WEAK]
                EXPORT  INTT32A05A_IRQHandler     [WEAK]
                EXPORT  INTT32A05ACAP0_IRQHandler [WEAK]
                EXPORT  INTT32A05ACAP1_IRQHandler [WEAK]
                EXPORT  INTT32A05B_IRQHandler     [WEAK]
                EXPORT  INTT32A05BCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A05BCAP1_IRQHandler [WEAK]
                EXPORT  INTT32A05C_IRQHandler     [WEAK]
                EXPORT  INTT32A05CCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A05CCAP1_IRQHandler [WEAK]
                EXPORT  INTT32A06A_IRQHandler     [WEAK]
                EXPORT  INTT32A06ACAP0_IRQHandler [WEAK]
                EXPORT  INTT32A06ACAP1_IRQHandler [WEAK]
                EXPORT  INTT32A06B_IRQHandler     [WEAK]
                EXPORT  INTT32A06BCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A06BCAP1_IRQHandler [WEAK]
                EXPORT  INTT32A06C_IRQHandler     [WEAK]
                EXPORT  INTT32A06CCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A06CCAP1_IRQHandler [WEAK]
                EXPORT  INTT32A07A_IRQHandler     [WEAK]
                EXPORT  INTT32A07ACAP0_IRQHandler [WEAK]
                EXPORT  INTT32A07ACAP1_IRQHandler [WEAK]
                EXPORT  INTT32A07B_IRQHandler     [WEAK]
                EXPORT  INTT32A07BCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A07BCAP1_IRQHandler [WEAK]
                EXPORT  INTT32A07C_IRQHandler     [WEAK]
                EXPORT  INTT32A07CCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A07CCAP1_IRQHandler [WEAK]
                EXPORT  INTPARI_IRQHandler        [WEAK]
                EXPORT  INTDMAATC_IRQHandler      [WEAK]
                EXPORT  INTDMAAERR_IRQHandler     [WEAK]
                EXPORT  INTDMABTC_IRQHandler      [WEAK]
                EXPORT  INTDMABERR_IRQHandler     [WEAK]
                EXPORT  INTRTC_IRQHandler         [WEAK]
                EXPORT  INTRMC0_IRQHandler        [WEAK]
                EXPORT  INTFLCRDY_IRQHandler      [WEAK]
                EXPORT  INTFLDRDY_IRQHandler      [WEAK]

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
INT16_IRQHandler
INT17_18_IRQHandler
INT19_22_IRQHandler
INT23_26_IRQHandler
INT27_28_IRQHandler
INT29_IRQHandler
INT30_31_IRQHandler
INTEMG0_IRQHandler
INTOVV0_IRQHandler
INTPMD0_IRQHandler
INTENC00_IRQHandler
INTENC01_IRQHandler
INTADAPDA_IRQHandler
INTADAPDB_IRQHandler
INTADACP0_IRQHandler
INTADACP1_IRQHandler
INTADATRG_IRQHandler
INTADASGL_IRQHandler
INTADACNT_IRQHandler
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
INTI2CWUP_IRQHandler
INTI2C0_IRQHandler
INTI2C0AL_IRQHandler
INTI2C0BF_IRQHandler
INTI2C0NA_IRQHandler
INTI2C1_IRQHandler
INTI2C1AL_IRQHandler
INTI2C1BF_IRQHandler
INTI2C1NA_IRQHandler
INTI2C2_IRQHandler
INTI2C2AL_IRQHandler
INTI2C2BF_IRQHandler
INTI2C2NA_IRQHandler
INTI2C3_IRQHandler
INTI2C3AL_IRQHandler
INTI2C3BF_IRQHandler
INTI2C3NA_IRQHandler
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
INTT32A00A_IRQHandler
INTT32A00ACAP0_IRQHandler
INTT32A00ACAP1_IRQHandler
INTT32A00B_IRQHandler
INTT32A00BCAP0_IRQHandler
INTT32A00BCAP1_IRQHandler
INTT32A00C_IRQHandler
INTT32A00CCAP0_IRQHandler
INTT32A00CCAP1_IRQHandler
INTT32A01A_IRQHandler
INTT32A01ACAP0_IRQHandler
INTT32A01ACAP1_IRQHandler
INTT32A01B_IRQHandler
INTT32A01BCAP0_IRQHandler
INTT32A01BCAP1_IRQHandler
INTT32A01C_IRQHandler
INTT32A01CCAP0_IRQHandler
INTT32A01CCAP1_IRQHandler
INTT32A02A_IRQHandler
INTT32A02ACAP0_IRQHandler
INTT32A02ACAP1_IRQHandler
INTT32A02B_IRQHandler
INTT32A02BCAP0_IRQHandler
INTT32A02BCAP1_IRQHandler
INTT32A02C_IRQHandler
INTT32A02CCAP0_IRQHandler
INTT32A02CCAP1_IRQHandler
INTT32A03A_IRQHandler
INTT32A03ACAP0_IRQHandler
INTT32A03ACAP1_IRQHandler
INTT32A03B_IRQHandler
INTT32A03BCAP0_IRQHandler
INTT32A03BCAP1_IRQHandler
INTT32A03C_IRQHandler
INTT32A03CCAP0_IRQHandler
INTT32A03CCAP1_IRQHandler
INTT32A04A_IRQHandler
INTT32A04ACAP0_IRQHandler
INTT32A04ACAP1_IRQHandler
INTT32A04B_IRQHandler
INTT32A04BCAP0_IRQHandler
INTT32A04BCAP1_IRQHandler
INTT32A04C_IRQHandler
INTT32A04CCAP0_IRQHandler
INTT32A04CCAP1_IRQHandler
INTT32A05A_IRQHandler
INTT32A05ACAP0_IRQHandler
INTT32A05ACAP1_IRQHandler
INTT32A05B_IRQHandler
INTT32A05BCAP0_IRQHandler
INTT32A05BCAP1_IRQHandler
INTT32A05C_IRQHandler
INTT32A05CCAP0_IRQHandler
INTT32A05CCAP1_IRQHandler
INTT32A06A_IRQHandler
INTT32A06ACAP0_IRQHandler
INTT32A06ACAP1_IRQHandler
INTT32A06B_IRQHandler
INTT32A06BCAP0_IRQHandler
INTT32A06BCAP1_IRQHandler
INTT32A06C_IRQHandler
INTT32A06CCAP0_IRQHandler
INTT32A06CCAP1_IRQHandler
INTT32A07A_IRQHandler
INTT32A07ACAP0_IRQHandler
INTT32A07ACAP1_IRQHandler
INTT32A07B_IRQHandler
INTT32A07BCAP0_IRQHandler
INTT32A07BCAP1_IRQHandler
INTT32A07C_IRQHandler
INTT32A07CCAP0_IRQHandler
INTT32A07CCAP1_IRQHandler
INTPARI_IRQHandler
INTDMAATC_IRQHandler
INTDMAAERR_IRQHandler
INTDMABTC_IRQHandler
INTDMABERR_IRQHandler
INTRTC_IRQHandler
INTRMC0_IRQHandler
INTFLCRDY_IRQHandler
INTFLDRDY_IRQHandler

                B       .

                ENDP

                END
