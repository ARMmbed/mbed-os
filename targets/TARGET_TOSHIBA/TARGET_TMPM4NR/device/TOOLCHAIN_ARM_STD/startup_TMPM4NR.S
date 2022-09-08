;/**
; *******************************************************************************
; * @file    startup_TMPM4NR.s
; * @brief   CMSIS Cortex-M4 Core Device Startup File for the
; *          TOSHIBA 'TMPM4NR' Device Series
; * @version V1.0.0
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; * 
; * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
; * 
; * Copyright(C) TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2021
; *******************************************************************************
; */


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                IMPORT  |Image$$ARM_LIB_STACK$$ZI$$Limit|

__Vectors       DCD     |Image$$ARM_LIB_STACK$$ZI$$Limit|              ; Top of Stack
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
                DCD     INTISDA_IRQHandler        ; 19:	Interval Sensing Detector interrupt (Unit A)
                DCD     INTISDB_IRQHandler        ; 20:	Interval Sensing Detector interrupt (Unit B)
                DCD     INTISDC_IRQHandler        ; 21:	Interval Sensing Detector interrupt (Unit C)
                DCD     INTRMC0_IRQHandler        ; 22:	Remote control reception interrupt 0
                DCD     INTRMC1_IRQHandler        ; 23:	Remote control reception interrupt 1
                DCD     INTLTTMR0_IRQHandler      ; 24:	Long Term Timer Interrupt(channel 0)
                DCD     INTHDMAATC_IRQHandler     ; 25:	HDMA complete of transfer(Unit A)
                DCD     INTHDMAAERR_IRQHandler    ; 26:	HDMA transfer error(Unit A)
                DCD     INTHDMABTC_IRQHandler     ; 27:	HDMA end of transfer(Unit B)
                DCD     INTHDMABERR_IRQHandler    ; 28:	HDMA transfer error(Unit B)
                DCD     INTMDMAATC_IRQHandler     ; 29:	MDMA complete of transfer(Unit A)
                DCD     INTT32A00_A_CT_IRQHandler ; 30:	T32A00 TimerA all interrupt /TimerC match, overflow, and underflow
                DCD     INTT32A00_B_C01_CPC_IRQHandler; 31:	T32A00 TimerB all interrupt /TimerC capture C0_C1, pulse count interrupt
                DCD     INTT32A01_A_CT_IRQHandler ; 32:	T32A01 TimerA all interrupt /TimerC match, overflow, and underflow
                DCD     INTT32A01_B_C01_CPC_IRQHandler; 33:	T32A01 TimerB all interrupt /TimerC capture C0_C1, pulse count interrupt
                DCD     INTT32A02_A_CT_IRQHandler ; 34:	T32A02 TimerA all interrupt /TimerC match, overflow, and underflow
                DCD     INTT32A02_B_C01_CPC_IRQHandler; 35:	T32A02 TimerB all interrupt /TimerC capture C0_C1, pulse count interrupt
                DCD     INTT32A03_A_CT_IRQHandler ; 36:	T32A03 TimerA all interrupt /TimerC match, overflow, and underflow
                DCD     INTT32A03_B_C01_CPC_IRQHandler; 37:	T32A03 TimerB all interrupt /TimerC capture C0_C1, pulse count interrupt
                DCD     INTT32A04_A_CT_IRQHandler ; 38:	T32A04 TimerA all interrupt /TimerC match, overflow, and underflow
                DCD     INTT32A04_B_C01_CPC_IRQHandler; 39:	T32A04 TimerB all interrupt /TimerC capture C0_C1, pulse count interrupt
                DCD     INTT32A05_A_CT_IRQHandler ; 40:	T32A05 TimerA all interrupt /TimerC match, overflow, and underflow
                DCD     INTT32A05_B_C01_CPC_IRQHandler; 41:	T32A05 TimerB all interrupt /TimerC capture C0_C1, pulse count interrupt
                DCD     INTT32A06_A_CT_IRQHandler ; 42:	T32A06 TimerA all interrupt /TimerC match, overflow, and underflow
                DCD     INTT32A06_B_C01_CPC_IRQHandler; 43:	T32A06 TimerB all interrupt /TimerC capture C0_C1, pulse count interrupt
                DCD     INTT32A07_A_CT_IRQHandler ; 44:	T32A07 TimerA all interrupt /TimerC match, overflow, and underflow
                DCD     INTT32A07_B_C01_CPC_IRQHandler; 45:	T32A07 TimerB all interrupt /TimerC capture C0_C1, pulse count interrupt
                DCD     INTT32A08_A_CT_IRQHandler ; 46:	T32A08 TimerA all interrupt /TimerC match, overflow, and underflow
                DCD     INTT32A08_B_C01_CPC_IRQHandler; 47:	T32A08 TimerB all interrupt /TimerC capture C0_C1, pulse count interrupt
                DCD     INTT32A09_A_CT_IRQHandler ; 48:	T32A09 TimerA all interrupt /TimerC match, overflow, and underflow
                DCD     INTT32A09_B_C01_CPC_IRQHandler; 49:	T32A09 TimerB all interrupt /TimerC capture C0_C1, pulse count interrupt
                DCD     INTT32A10_A_CT_IRQHandler ; 50:	T32A10 TimerA all interrupt /TimerC match, overflow, and underflow
                DCD     INTT32A10_B_C01_CPC_IRQHandler; 51:	T32A10 TimerB all interrupt /TimerC capture C0_C1, pulse count interrupt
                DCD     INTT32A11_A_CT_IRQHandler ; 52:	T32A11 TimerA all interrupt /TimerC match, overflow, and underflow
                DCD     INTT32A11_B_C01_CPC_IRQHandler; 53:	T32A11 TimerB all interrupt /TimerC capture C0_C1, pulse count interrupt
                DCD     INTT32A12_A_CT_IRQHandler ; 54:	T32A12 TimerA all interrupt /TimerC match, overflow, and underflow
                DCD     INTT32A12_B_C01_CPC_IRQHandler; 55:	T32A12 TimerB all interrupt /TimerC capture C0_C1, pulse count interrupt
                DCD     INTT32A13_A_CT_IRQHandler ; 56:	T32A13 TimerA all interrupt /TimerC match, overflow, and underflow
                DCD     INTT32A13_B_C01_CPC_IRQHandler; 57:	T32A13 TimerB all interrupt /TimerC capture C0_C1, pulse count interrupt
                DCD     INTEMG0_IRQHandler        ; 58:	PMD0 EMG interrupt
                DCD     INTOVV0_IRQHandler        ; 59:	PMD0 OVV interrupt
                DCD     INTPWM0_IRQHandler        ; 60:	PMD0 interrupt
                DCD     INTT0RX_IRQHandler        ; 61:	TSPI/SIO reception (channel 0) interrupt
                DCD     INTT0TX_IRQHandler        ; 62:	TSPI/SIO transmit (channel 0) interrupt
                DCD     INTT0ERR_IRQHandler       ; 63:	TSPI/SIO error (channel 0) interrupt
                DCD     INTT1RX_IRQHandler        ; 64:	TSPI/SIO reception (channel 1) interrupt
                DCD     INTT1TX_IRQHandler        ; 65:	TSPI/SIO transmit (channel 1) interrupt
                DCD     INTT1ERR_IRQHandler       ; 66:	TSPI/SIO error (channel 1) interrupt
                DCD     INTT2RX_IRQHandler        ; 67:	TSPI/SIO reception (channel 2) interrupt
                DCD     INTT2TX_IRQHandler        ; 68:	TSPI/SIO transmit (channel 2) interrupt
                DCD     INTT2ERR_IRQHandler       ; 69:	TSPI/SIO error (channel 2) interrupt
                DCD     INTT3RX_IRQHandler        ; 70:	TSPI/SIO reception (channel 3) interrupt
                DCD     INTT3TX_IRQHandler        ; 71:	TSPI/SIO transmit (channel 3) interrupt
                DCD     INTT3ERR_IRQHandler       ; 72:	TSPI/SIO error (channel 3) interrupt
                DCD     INTT4RX_IRQHandler        ; 73:	TSPI/SIO reception (channel 4) interrupt
                DCD     INTT4TX_IRQHandler        ; 74:	TSPI/SIO transmit (channel 4) interrupt
                DCD     INTT4ERR_IRQHandler       ; 75:	TSPI/SIO error (channel 4) interrupt
                DCD     INTT5RX_IRQHandler        ; 76:	TSPI/SIO reception (channel 5) interrupt
                DCD     INTT5TX_IRQHandler        ; 77:	TSPI/SIO transmit (channel 5) interrupt
                DCD     INTT5ERR_IRQHandler       ; 78:	TSPI/SIO error (channel 5) interrupt
                DCD     INTT6RX_IRQHandler        ; 79:	TSPI/SIO reception (channel 6) interrupt
                DCD     INTT6TX_IRQHandler        ; 80:	TSPI/SIO transmit (channel 6) interrupt
                DCD     INTT6ERR_IRQHandler       ; 81:	TSPI/SIO error (channel 6) interrupt
                DCD     INTT7RX_IRQHandler        ; 82:	TSPI/SIO reception (channel 7) interrupt
                DCD     INTT7TX_IRQHandler        ; 83:	TSPI/SIO transmit (channel 7) interrupt
                DCD     INTT7ERR_IRQHandler       ; 84:	TSPI/SIO error (channel 7) interrupt
                DCD     INTT8RX_IRQHandler        ; 85:	TSPI/SIO reception (channel 8) interrupt
                DCD     INTT8TX_IRQHandler        ; 86:	TSPI/SIO transmit (channel 8) interrupt
                DCD     INTT8ERR_IRQHandler       ; 87:	TSPI/SIO error (channel 8) interrupt
                DCD     INTSMI0_IRQHandler        ; 88:	Serial Memory Interface interrupt
                DCD     INTUART0RX_IRQHandler     ; 89:	UART reception (channel 0) interrupt
                DCD     INTUART0TX_IRQHandler     ; 90:	UART transmit (channel 0) interrupt
                DCD     INTUART0ERR_IRQHandler    ; 91:	UART error (channel 0) interrupt
                DCD     INTUART1RX_IRQHandler     ; 92:	UART reception (channel 1) interrupt
                DCD     INTUART1TX_IRQHandler     ; 93:	UART transmit (channel 1) interrupt
                DCD     INTUART1ERR_IRQHandler    ; 94:	UART error (channel 1) interrupt
                DCD     INTUART2RX_IRQHandler     ; 95:	UART reception (channel 2) interrupt
                DCD     INTUART2TX_IRQHandler     ; 96:	UART transmit (channel 2) interrupt
                DCD     INTUART2ERR_IRQHandler    ; 97:	UART error (channel 2) interrupt
                DCD     INTUART3RX_IRQHandler     ; 98:	UART reception (channel 3) interrupt
                DCD     INTUART3TX_IRQHandler     ; 99:	UART transmit (channel 3) interrupt
                DCD     INTUART3ERR_IRQHandler    ; 100:	UART error (channel 3) interrupt
                DCD     INTUART4RX_IRQHandler     ; 101:	UART reception (channel 4) interrupt
                DCD     INTUART4TX_IRQHandler     ; 102:	UART transmit (channel 4) interrupt
                DCD     INTUART4ERR_IRQHandler    ; 103:	UART error (channel 4) interrupt
                DCD     INTUART5RX_IRQHandler     ; 104:	UART reception (channel 5) interrupt
                DCD     INTUART5TX_IRQHandler     ; 105:	UART transmit (channel 5) interrupt
                DCD     INTUART5ERR_IRQHandler    ; 106:	UART error (channel 5) interrupt
                DCD     INTFUART0_IRQHandler      ; 107:	FUART interrupt(channel 0)
                DCD     INTFUART1_IRQHandler      ; 108:	FUART interrupt(channel 1)
                DCD     INTI2C0NST_IRQHandler     ; 109:	I2C0 interrupt / EI2C0 status interrupt
                DCD     INTI2C0ATX_IRQHandler     ; 110:	I2C0 arbitration lost detection interrupt / EI2C0 transmit buffer empty interrupt
                DCD     INTI2C0BRX_IRQHandler     ; 111:	I2C0 bus free detection interrupt / EI2C0receive buffer empty interrupt
                DCD     INTI2C0NA_IRQHandler      ; 112:	I2C0 NACK detection interrupt
                DCD     INTI2C1NST_IRQHandler     ; 113:	I2C1 interrupt / EI2C0 status interrupt
                DCD     INTI2C1ATX_IRQHandler     ; 114:	I2C1 arbitration lost detection interrupt / EI2C0 transmit buffer empty interrupt
                DCD     INTI2C1BRX_IRQHandler     ; 115:	I2C1 bus free detection interrupt / EI2C0receive buffer empty interrupt
                DCD     INTI2C1NA_IRQHandler      ; 116:	I2C1 NACK detection interrupt
                DCD     INTI2C2NST_IRQHandler     ; 117:	I2C2 interrupt / EI2C0 status interrupt
                DCD     INTI2C2ATX_IRQHandler     ; 118:	I2C2 arbitration lost detection interrupt / EI2C0 transmit buffer empty interrupt
                DCD     INTI2C2BRX_IRQHandler     ; 119:	I2C2 bus free detection interrupt / EI2C0receive buffer empty interrupt
                DCD     INTI2C2NA_IRQHandler      ; 120:	I2C2 NACK detection interrupt
                DCD     INTI2C3NST_IRQHandler     ; 121:	I2C3 interrupt / EI2C0 status interrupt
                DCD     INTI2C3ATX_IRQHandler     ; 122:	I2C3 arbitration lost detection interrupt / EI2C0 transmit buffer empty interrupt
                DCD     INTI2C3BRX_IRQHandler     ; 123:	I2C3 bus free detection interrupt / EI2C0receive buffer empty interrupt
                DCD     INTI2C3NA_IRQHandler      ; 124:	I2C3 NACK detection interrupt
                DCD     INTI2C4NST_IRQHandler     ; 125:	I2C4 interrupt / EI2C0 status interrupt
                DCD     INTI2C4ATX_IRQHandler     ; 126:	I2C4 arbitration lost detection interrupt / EI2C0 transmit buffer empty interrupt
                DCD     INTI2C4BRX_IRQHandler     ; 127:	I2C4 bus free detection interrupt / EI2C0receive buffer empty interrupt
                DCD     INTI2C4NA_IRQHandler      ; 128:	I2C4 NACK detection interrupt
                DCD     INTADACP0_IRQHandler      ; 129:	ADC conversion monitoring function interrupt 0
                DCD     INTADACP1_IRQHandler      ; 130:	ADC conversion monitoring function interrupt 1
                DCD     INTADATRG_IRQHandler      ; 131:	ADC conversion triggered by General purpose is finished
                DCD     INTADASGL_IRQHandler      ; 132:	ADC conversion triggered by Single program is finished
                DCD     INTADACNT_IRQHandler      ; 133:	ADC conversion triggered by Continuity program is finished
                DCD     INTADAHP_IRQHandler       ; 134:	ADC high priority AD conversion interrupt
                DCD     INTFLDRDY_IRQHandler      ; 135:	Data FLASH Ready interrupt
                DCD     INTFLCRDY_IRQHandler      ; 136:	Code FLASH Area0/1 Ready interrupt
                DCD     0                         ; 137:	Reserved
                DCD     0                         ; 138:	Reserved
                DCD     INTMDMAABERR_IRQHandler   ; 139:	MDMA bus error(Unit A) interrupt
                DCD     INTMDMAADERR_IRQHandler   ; 140:	MDMA descriptor error(Unit A) interrupt
                DCD     INTI2S0SI_IRQHandler      ; 141:	I2S ch0 SI transfer end interrupt
                DCD     INTI2S0SIERR_IRQHandler   ; 142:	I2S ch0 SI error interrupt
                DCD     INTI2S0SO_IRQHandler      ; 143:	I2S ch0 SO transfer end interrupt
                DCD     INTI2S0SOERR_IRQHandler   ; 144:	I2S ch0 SO error interrupt
                DCD     INTI2S1SI_IRQHandler      ; 145:	I2S ch1 SI transfer end interrupt
                DCD     INTI2S1SIERR_IRQHandler   ; 146:	I2S ch1 SI error interrupt
                DCD     INTI2S1SO_IRQHandler      ; 147:	I2S ch1 SO transfer end interrupt
                DCD     INTI2S1SOERR_IRQHandler   ; 148:	I2S ch1 SO error interrupt
                DCD     INTFIR0_IRQHandler        ; 149:	FIR ch0 interrupt
                DCD     INTTSSI0RX_IRQHandler     ; 150:	TSSI ch0 RX interrupt
                DCD     INTTSSI0TX_IRQHandler     ; 151:	TSSI ch0 TX interrupt
                DCD     INTTSSI0ERR_IRQHandler    ; 152:	TSSI ch0 error interrupt
                DCD     INTTSSI1RX_IRQHandler     ; 153:	TSSI ch1 RX interrupt
                DCD     INTTSSI1TX_IRQHandler     ; 154:	TSSI ch1 TX interrupt
                DCD     INTTSSI1ERR_IRQHandler    ; 155:	TSSI ch1 error interrupt
                DCD     INTEMA_IRQHandler         ; 156:	Ether MAC interrupt
                DCD     INTEMAMP_IRQHandler       ; 157:	Ether MAC magic packet detection interrupt
                DCD     INTCAN0RX_IRQHandler      ; 158:	CAN ch0 receive interrupt
                DCD     INTCAN0TX_IRQHandler      ; 159:	CAN ch0 transmission interrupt
                DCD     INTCAN0GLB_IRQHandler     ; 160:	CAN ch0 Global interrupt
                DCD     INTCAN1RX_IRQHandler      ; 161:	CAN ch1 receive interrupt
                DCD     INTCAN1TX_IRQHandler      ; 162:	CAN ch1 transmission interrupt
                DCD     INTCAN1GLB_IRQHandler     ; 163:	CAN ch1 Global interrupt
                DCD     INTUSBOTGA_IRQHandler     ; 164:	USB OTG unit A interrupt
                DCD     INTUSBOTGAWK_IRQHandler   ; 165:	USB OTG unit A Wakeup interrupt
                DCD     INTUSBOTGB_IRQHandler     ; 166:	USB OTG unit B interrupt
                DCD     INTUSBOTGBWK_IRQHandler   ; 167:	USB OTG unit B Wakeup interrupt
                DCD     INTT32A14_A_IRQHandler    ; 168:	T32A14 TimerA all interrupt
                DCD     INTT32A14_B_IRQHandler    ; 169:	T32A14 TimerB all interrupt
                DCD     INTT32A14_CT_IRQHandler   ; 170:	T32A14 TimerC match, overflow, and underflow Interrupt
                DCD     INTT32A15_A_IRQHandler    ; 171:	T32A15 TimerA all interrupt
                DCD     INTT32A15_B_IRQHandler    ; 172:	T32A15 TimerB all interrupt
                DCD     INTT32A15_C_IRQHandler    ; 173:	T32A15 TimerC all interrupt
                DCD     INTMDMAADISR_IRQHandler   ; 174:	MDMAC unit A Descriptor interrupt



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
                EXPORT  INTLTTMR0_IRQHandler      [WEAK]
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
                EXPORT  INTI2C0NST_IRQHandler     [WEAK]
                EXPORT  INTI2C0ATX_IRQHandler     [WEAK]
                EXPORT  INTI2C0BRX_IRQHandler     [WEAK]
                EXPORT  INTI2C0NA_IRQHandler      [WEAK]
                EXPORT  INTI2C1NST_IRQHandler     [WEAK]
                EXPORT  INTI2C1ATX_IRQHandler     [WEAK]
                EXPORT  INTI2C1BRX_IRQHandler     [WEAK]
                EXPORT  INTI2C1NA_IRQHandler      [WEAK]
                EXPORT  INTI2C2NST_IRQHandler     [WEAK]
                EXPORT  INTI2C2ATX_IRQHandler     [WEAK]
                EXPORT  INTI2C2BRX_IRQHandler     [WEAK]
                EXPORT  INTI2C2NA_IRQHandler      [WEAK]
                EXPORT  INTI2C3NST_IRQHandler     [WEAK]
                EXPORT  INTI2C3ATX_IRQHandler     [WEAK]
                EXPORT  INTI2C3BRX_IRQHandler     [WEAK]
                EXPORT  INTI2C3NA_IRQHandler      [WEAK]
                EXPORT  INTI2C4NST_IRQHandler     [WEAK]
                EXPORT  INTI2C4ATX_IRQHandler     [WEAK]
                EXPORT  INTI2C4BRX_IRQHandler     [WEAK]
                EXPORT  INTI2C4NA_IRQHandler      [WEAK]
                EXPORT  INTADACP0_IRQHandler      [WEAK]
                EXPORT  INTADACP1_IRQHandler      [WEAK]
                EXPORT  INTADATRG_IRQHandler      [WEAK]
                EXPORT  INTADASGL_IRQHandler      [WEAK]
                EXPORT  INTADACNT_IRQHandler      [WEAK]
                EXPORT  INTADAHP_IRQHandler       [WEAK]
                EXPORT  INTFLDRDY_IRQHandler      [WEAK]
                EXPORT  INTFLCRDY_IRQHandler      [WEAK]
                EXPORT  INTMDMAABERR_IRQHandler   [WEAK]
                EXPORT  INTMDMAADERR_IRQHandler   [WEAK]
                EXPORT  INTI2S0SI_IRQHandler      [WEAK]
                EXPORT  INTI2S0SIERR_IRQHandler   [WEAK]
                EXPORT  INTI2S0SO_IRQHandler      [WEAK]
                EXPORT  INTI2S0SOERR_IRQHandler   [WEAK]
                EXPORT  INTI2S1SI_IRQHandler      [WEAK]
                EXPORT  INTI2S1SIERR_IRQHandler   [WEAK]
                EXPORT  INTI2S1SO_IRQHandler      [WEAK]
                EXPORT  INTI2S1SOERR_IRQHandler   [WEAK]
                EXPORT  INTFIR0_IRQHandler        [WEAK]
                EXPORT  INTTSSI0RX_IRQHandler     [WEAK]
                EXPORT  INTTSSI0TX_IRQHandler     [WEAK]
                EXPORT  INTTSSI0ERR_IRQHandler    [WEAK]
                EXPORT  INTTSSI1RX_IRQHandler     [WEAK]
                EXPORT  INTTSSI1TX_IRQHandler     [WEAK]
                EXPORT  INTTSSI1ERR_IRQHandler    [WEAK]
                EXPORT  INTEMA_IRQHandler         [WEAK]
                EXPORT  INTEMAMP_IRQHandler       [WEAK]
                EXPORT  INTCAN0RX_IRQHandler      [WEAK]
                EXPORT  INTCAN0TX_IRQHandler      [WEAK]
                EXPORT  INTCAN0GLB_IRQHandler     [WEAK]
                EXPORT  INTCAN1RX_IRQHandler      [WEAK]
                EXPORT  INTCAN1TX_IRQHandler      [WEAK]
                EXPORT  INTCAN1GLB_IRQHandler     [WEAK]
                EXPORT  INTUSBOTGA_IRQHandler     [WEAK]
                EXPORT  INTUSBOTGAWK_IRQHandler   [WEAK]
                EXPORT  INTUSBOTGB_IRQHandler     [WEAK]
                EXPORT  INTUSBOTGBWK_IRQHandler   [WEAK]
                EXPORT  INTT32A14_A_IRQHandler    [WEAK]
                EXPORT  INTT32A14_B_IRQHandler    [WEAK]
                EXPORT  INTT32A14_CT_IRQHandler   [WEAK]
                EXPORT  INTT32A15_A_IRQHandler    [WEAK]
                EXPORT  INTT32A15_B_IRQHandler    [WEAK]
                EXPORT  INTT32A15_C_IRQHandler    [WEAK]
                EXPORT  INTMDMAADISR_IRQHandler   [WEAK]

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
INTLTTMR0_IRQHandler
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
INTI2C0NST_IRQHandler
INTI2C0ATX_IRQHandler
INTI2C0BRX_IRQHandler
INTI2C0NA_IRQHandler
INTI2C1NST_IRQHandler
INTI2C1ATX_IRQHandler
INTI2C1BRX_IRQHandler
INTI2C1NA_IRQHandler
INTI2C2NST_IRQHandler
INTI2C2ATX_IRQHandler
INTI2C2BRX_IRQHandler
INTI2C2NA_IRQHandler
INTI2C3NST_IRQHandler
INTI2C3ATX_IRQHandler
INTI2C3BRX_IRQHandler
INTI2C3NA_IRQHandler
INTI2C4NST_IRQHandler
INTI2C4ATX_IRQHandler
INTI2C4BRX_IRQHandler
INTI2C4NA_IRQHandler
INTADACP0_IRQHandler
INTADACP1_IRQHandler
INTADATRG_IRQHandler
INTADASGL_IRQHandler
INTADACNT_IRQHandler
INTADAHP_IRQHandler
INTFLDRDY_IRQHandler
INTFLCRDY_IRQHandler
INTMDMAABERR_IRQHandler
INTMDMAADERR_IRQHandler
INTI2S0SI_IRQHandler
INTI2S0SIERR_IRQHandler
INTI2S0SO_IRQHandler
INTI2S0SOERR_IRQHandler
INTI2S1SI_IRQHandler
INTI2S1SIERR_IRQHandler
INTI2S1SO_IRQHandler
INTI2S1SOERR_IRQHandler
INTFIR0_IRQHandler
INTTSSI0RX_IRQHandler
INTTSSI0TX_IRQHandler
INTTSSI0ERR_IRQHandler
INTTSSI1RX_IRQHandler
INTTSSI1TX_IRQHandler
INTTSSI1ERR_IRQHandler
INTEMA_IRQHandler
INTEMAMP_IRQHandler
INTCAN0RX_IRQHandler
INTCAN0TX_IRQHandler
INTCAN0GLB_IRQHandler
INTCAN1RX_IRQHandler
INTCAN1TX_IRQHandler
INTCAN1GLB_IRQHandler
INTUSBOTGA_IRQHandler
INTUSBOTGAWK_IRQHandler
INTUSBOTGB_IRQHandler
INTUSBOTGBWK_IRQHandler
INTT32A14_A_IRQHandler
INTT32A14_B_IRQHandler
INTT32A14_CT_IRQHandler
INTT32A15_A_IRQHandler
INTT32A15_B_IRQHandler
INTT32A15_C_IRQHandler
INTMDMAADISR_IRQHandler

                B       .

                ENDP


                ALIGN











                END

