;/**
; *******************************************************************************
; * @file    startup_TMPM4KNA.s
; * @brief   CMSIS Cortex-M4 Core Device Startup File for the
; *          TOSHIBA 'TMPM4KNA' Device Series
; * @version V1.0.0.0
; * $Date::            #$
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; * 
; * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
; * 
; * Copyright(C) Toshiba Electronic Device Solutions Corporation 2021
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
                DCD     INT00_IRQHandler          ; 0:	Interrupt pin 00
                DCD     INT01_IRQHandler          ; 1:	Interrupt pin 01a/00b
                DCD     INT02_IRQHandler          ; 2:	Interrupt pin 02a/00b
                DCD     INT03_IRQHandler          ; 3:	Interrupt pin 03a/03b
                DCD     INT04_IRQHandler          ; 4:	Interrupt pin 04a/04b
                DCD     INT05_IRQHandler          ; 5:	Interrupt pin 05a/05b
                DCD     INT06_IRQHandler          ; 6:	Interrupt pin 06a/06b
                DCD     INT07_IRQHandler          ; 7:	Interrupt pin 07a/07b
                DCD     INT08_IRQHandler          ; 8:	Interrupt pin 08a/08b
                DCD     INT09_IRQHandler          ; 9:	Interrupt pin 09
                DCD     INT10_IRQHandler          ; 10:	Interrupt pin 10
                DCD     INT11_IRQHandler          ; 11:	Interrupt pin 11a/11b
                DCD     INT12_IRQHandler          ; 12:	Interrupt pin 12
                DCD     INT13_IRQHandler          ; 13:	Interrupt pin 13
                DCD     INT14_IRQHandler          ; 14:	Interrupt pin 14a/14b
                DCD     INT15_IRQHandler          ; 15:	Interrupt pin 15
                DCD     INT16_IRQHandler          ; 16:	Interrupt pin 16a/16b
                DCD     INT17_IRQHandler          ; 17:	Interrupt pin 17a/17b
                DCD     INT18_IRQHandler          ; 18:	Interrupt pin 18a/18b
                DCD     0                         ; 19:	Reserved
                DCD     0                         ; 20:	Reserved
                DCD     INT21_IRQHandler          ; 21:	Interrupt pin 21
                DCD     INTVCN0_IRQHandler        ; 22:	A-VE+ ch0 Interrupt
                DCD     INTVCT0_IRQHandler        ; 23:	A-VE+ ch0 Task termination interrupt
                DCD     INTEMG0_IRQHandler        ; 24:	A-PMD ch0 EMG interrupt
                DCD     INTEMG1_IRQHandler        ; 25:	A-PMD ch1 EMG interrupt
                DCD     INTEMG2_IRQHandler        ; 26:	A-PMD ch2 EMG interrupt
                DCD     INTOVV0_IRQHandler        ; 27:	A-PMD ch0 OVV interrupt
                DCD     INTOVV1_IRQHandler        ; 28:	A-PMD ch1 OVV interrupt
                DCD     INTOVV2_IRQHandler        ; 29:	A-PMD ch2 OVV interrupt
                DCD     INTPWM0_IRQHandler        ; 30:	A-PMD ch0 PWM interrupt
                DCD     INTPWM1_IRQHandler        ; 31:	A-PMD ch1 PWM interrupt
                DCD     INTPWM2_IRQHandler        ; 32:	A-PMD ch2 PWM interrupt
                DCD     INTENC00_IRQHandler       ; 33:	A-ENC32 ch0 Encoder interrupt 0
                DCD     INTENC01_IRQHandler       ; 34:	A-ENC32 ch0 Encoder interrupt 1
                DCD     INTENC10_IRQHandler       ; 35:	A-ENC32 ch1 Encoder interrupt 0
                DCD     INTENC11_IRQHandler       ; 36:	A-ENC32 ch1 Encoder interrupt 1
                DCD     INTENC20_IRQHandler       ; 37:	A-ENC32 ch2 Encoder interrupt 0
                DCD     INTENC21_IRQHandler       ; 38:	A-ENC32 ch2 Encoder interrupt 1
                DCD     INTADAPDA_IRQHandler      ; 39:	ADC unit A PMD trigger program interrupt A
                DCD     INTADAPDB_IRQHandler      ; 40:	ADC unit A PMD trigger program interrupt B
                DCD     INTADACP0_IRQHandler      ; 41:	ADC unit A Monitor function 0 interrupt
                DCD     INTADACP1_IRQHandler      ; 42:	ADC unit A Monitor function 1 interrupt
                DCD     INTADATRG_IRQHandler      ; 43:	ADC unit A General purpose trigger program interrupt
                DCD     INTADASGL_IRQHandler      ; 44:	ADC unit A Single program interrupt
                DCD     INTADACNT_IRQHandler      ; 45:	ADC unit A Continuity program interrupt
                DCD     INTADBPDA_IRQHandler      ; 46:	ADC unit B PMD trigger program interrupt A
                DCD     INTADBPDB_IRQHandler      ; 47:	ADC unit B PMD trigger program interrupt B
                DCD     INTADBCP0_IRQHandler      ; 48:	ADC unit B Monitor function 0 interrupt
                DCD     INTADBCP1_IRQHandler      ; 49:	ADC unit B Monitor function 1 interrupt
                DCD     INTADBTRG_IRQHandler      ; 50:	ADC unit B General purpose trigger program interrupt
                DCD     INTADBSGL_IRQHandler      ; 51:	ADC unit B Single program interrupt
                DCD     INTADBCNT_IRQHandler      ; 52:	ADC unit B Continuity program interrupt
                DCD     INTADCPDA_IRQHandler      ; 53:	ADC unit C PMD trigger program interrupt A
                DCD     INTADCPDB_IRQHandler      ; 54:	ADC unit C PMD trigger program interrupt B
                DCD     INTADCCP0_IRQHandler      ; 55:	ADC unit C Monitor function 0 interrupt
                DCD     INTADCCP1_IRQHandler      ; 56:	ADC unit C Monitor function 1 interrupt
                DCD     INTADCTRG_IRQHandler      ; 57:	ADC unit C General purpose trigger program interrupt
                DCD     INTADCSGL_IRQHandler      ; 58:	ADC unit C Single program interrupt
                DCD     INTADCCNT_IRQHandler      ; 59:	ADC unit C Continuity program interrupt
                DCD     INTSC0RX_IRQHandler       ; 60:	TSPI/UART ch0 Reception interrupt
                DCD     INTSC0TX_IRQHandler       ; 61:	TSPI/UART ch0 Transmit interrupt
                DCD     INTSC0ERR_IRQHandler      ; 62:	TSPI/UART ch0 Error interrupt
                DCD     INTSC1RX_IRQHandler       ; 63:	TSPI/UART ch1 Reception interrupt
                DCD     INTSC1TX_IRQHandler       ; 64:	TSPI/UART ch1 Transmit interrupt
                DCD     INTSC1ERR_IRQHandler      ; 65:	TSPI/UART ch1 Error interrupt
                DCD     INTSC2RX_IRQHandler       ; 66:	UART ch2 Reception interrupt
                DCD     INTSC2TX_IRQHandler       ; 67:	UART ch2 Transmit interrupt
                DCD     INTSC2ERR_IRQHandler      ; 68:	UART ch2 Error interrupt
                DCD     INTSC3RX_IRQHandler       ; 69:	UART ch3 Reception interrupt
                DCD     INTSC3TX_IRQHandler       ; 70:	UART ch3 Transmit interrupt
                DCD     INTSC3ERR_IRQHandler      ; 71:	UART ch3 Error interrupt
                DCD     INTI2C0NST_IRQHandler     ; 72:	I2C ch0 Interrupt / EI2C ch0 Status interrupt
                DCD     INTI2C0ATX_IRQHandler     ; 73:	I2C ch0 Arbitration lost detection interrupt / EI2C ch0 Transmission buffer empty interrupt
                DCD     INTI2C0BRX_IRQHandler     ; 74:	I2C ch0 Bus free detection interrupt / EI2C ch0 Reception buffer full interrupt
                DCD     INTI2C0NA_IRQHandler      ; 75:	I2C ch0 NACK detection interrupt
                DCD     INTI2C1NST_IRQHandler     ; 76:	I2C ch1 Interrupt / EI2C ch1 Status interrupt
                DCD     INTI2C1ATX_IRQHandler     ; 77:	I2C ch1 Arbitration lost detection interrupt / EI2C ch1 Transmission buffer empty interrupt
                DCD     INTI2C1BRX_IRQHandler     ; 78:	I2C ch1 Bus free detection interrupt / EI2C ch1 Reception buffer full interrupt
                DCD     INTI2C1NA_IRQHandler      ; 79:	I2C ch1 NACK detection interrupt
                DCD     0                         ; 80:	Reserved
                DCD     0                         ; 81:	Reserved
                DCD     0                         ; 82:	Reserved
                DCD     INTT32A00AC_IRQHandler    ; 83:	T32A ch0 Timer A/C Compare match detection / Over flow / Under flow
                DCD     INTT32A00ACCAP0_IRQHandler; 84:	T32A ch0 Timer A/C Input capture 0
                DCD     INTT32A00ACCAP1_IRQHandler; 85:	T32A ch0 Timer A/C Input capture 1
                DCD     INTT32A00B_IRQHandler     ; 86:	T32A ch0 Timer B Compare match detection / Over flow / Under flow
                DCD     INTT32A00BCAP0_IRQHandler ; 87:	T32A ch0 Timer B Input capture 0
                DCD     INTT32A00BCAP1_IRQHandler ; 88:	T32A ch0 Timer B Input capture 1
                DCD     INTT32A01AC_IRQHandler    ; 89:	T32A ch1 Timer A/C Compare match detection / Over flow / Under flow
                DCD     INTT32A01ACCAP0_IRQHandler; 90:	T32A ch1 Timer A/C Input capture 0
                DCD     INTT32A01ACCAP1_IRQHandler; 91:	T32A ch1 Timer A/C Input capture 1
                DCD     INTT32A01B_IRQHandler     ; 92:	T32A ch1 Timer B Compare match detection / Over flow / Under flow
                DCD     INTT32A01BCAP0_IRQHandler ; 93:	T32A ch1 Timer B Input capture 0
                DCD     INTT32A01BCAP1_IRQHandler ; 94:	T32A ch1 Timer B Input capture 1
                DCD     INTT32A02AC_IRQHandler    ; 95:	T32A ch2 Timer A/C Compare match detection / Over flow / Under flow
                DCD     INTT32A02ACCAP0_IRQHandler; 96:	T32A ch2 Timer A/C Input capture 0
                DCD     INTT32A02ACCAP1_IRQHandler; 97:	T32A ch2 Timer A/C Input capture 1
                DCD     INTT32A02B_IRQHandler     ; 98:	T32A ch2 Timer B Compare match detection / Over flow / Under flow
                DCD     INTT32A02BCAP0_IRQHandler ; 99:	T32A ch2 Timer B Input capture 0
                DCD     INTT32A02BCAP1_IRQHandler ; 100:	T32A ch2 Timer B Input capture 1
                DCD     INTT32A03AC_IRQHandler    ; 101:	T32A ch3 Timer A/C Compare match detection / Over flow / Under flow
                DCD     INTT32A03ACCAP0_IRQHandler; 102:	T32A ch3 Timer A/C Input capture 0
                DCD     INTT32A03ACCAP1_IRQHandler; 103:	T32A ch3 Timer A/C Input capture 1
                DCD     INTT32A03B_IRQHandler     ; 104:	T32A ch3 Timer B Compare match detection / Over flow / Under flow
                DCD     INTT32A03BCAP0_IRQHandler ; 105:	T32A ch3 Timer B Input capture 0
                DCD     INTT32A03BCAP1_IRQHandler ; 106:	T32A ch3 Timer B Input capture 1
                DCD     INTT32A04AC_IRQHandler    ; 107:	T32A ch4 Timer A/C Compare match detection / Over flow / Under flow
                DCD     INTT32A04ACCAP0_IRQHandler; 108:	T32A ch4 Timer A/C Input capture 0
                DCD     INTT32A04ACCAP1_IRQHandler; 109:	T32A ch4 Timer A/C Input capture 1
                DCD     INTT32A04B_IRQHandler     ; 110:	T32A ch4 Timer B Compare match detection / Over flow / Under flow
                DCD     INTT32A04BCAP0_IRQHandler ; 111:	T32A ch4 Timer B Input capture 0
                DCD     INTT32A04BCAP1_IRQHandler ; 112:	T32A ch4 Timer B Input capture 1
                DCD     INTT32A05AC_IRQHandler    ; 113:	T32A ch5 Timer A/C Compare match detection / Over flow / Under flow
                DCD     INTT32A05ACCAP0_IRQHandler; 114:	T32A ch5 Timer A/C Input capture 0
                DCD     INTT32A05ACCAP1_IRQHandler; 115:	T32A ch5 Timer A/C Input capture 1
                DCD     INTT32A05B_IRQHandler     ; 116:	T32A ch5 Timer B Compare match detection / Over flow / Under flow
                DCD     INTT32A05BCAP0_IRQHandler ; 117:	T32A ch5 Timer B Input capture 0
                DCD     INTT32A05BCAP1_IRQHandler ; 118:	T32A ch5 Timer B Input capture 1
                DCD     INTPARI0_IRQHandler       ; 119:	RAMP ch0 Parity error interrupt
                DCD     INTPARI1_IRQHandler       ; 120:	RAMP ch1 Parity error interrupt
                DCD     INTDMAATC_IRQHandler      ; 121:	DMAC unit A End of transfer (ch0 - 31)
                DCD     INTDMAAERR_IRQHandler     ; 122:	DMAC unit A Transfer error
                DCD     INTFLCRDY_IRQHandler      ; 123:	Code FLASH Ready interrupt
                DCD     INTFLDRDY_IRQHandler      ; 124:	Data FLASH Ready interrupt



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
                EXPORT  INT17_IRQHandler          [WEAK]
                EXPORT  INT18_IRQHandler          [WEAK]
                EXPORT  INT21_IRQHandler          [WEAK]
                EXPORT  INTVCN0_IRQHandler        [WEAK]
                EXPORT  INTVCT0_IRQHandler        [WEAK]
                EXPORT  INTEMG0_IRQHandler        [WEAK]
                EXPORT  INTEMG1_IRQHandler        [WEAK]
                EXPORT  INTEMG2_IRQHandler        [WEAK]
                EXPORT  INTOVV0_IRQHandler        [WEAK]
                EXPORT  INTOVV1_IRQHandler        [WEAK]
                EXPORT  INTOVV2_IRQHandler        [WEAK]
                EXPORT  INTPWM0_IRQHandler        [WEAK]
                EXPORT  INTPWM1_IRQHandler        [WEAK]
                EXPORT  INTPWM2_IRQHandler        [WEAK]
                EXPORT  INTENC00_IRQHandler       [WEAK]
                EXPORT  INTENC01_IRQHandler       [WEAK]
                EXPORT  INTENC10_IRQHandler       [WEAK]
                EXPORT  INTENC11_IRQHandler       [WEAK]
                EXPORT  INTENC20_IRQHandler       [WEAK]
                EXPORT  INTENC21_IRQHandler       [WEAK]
                EXPORT  INTADAPDA_IRQHandler      [WEAK]
                EXPORT  INTADAPDB_IRQHandler      [WEAK]
                EXPORT  INTADACP0_IRQHandler      [WEAK]
                EXPORT  INTADACP1_IRQHandler      [WEAK]
                EXPORT  INTADATRG_IRQHandler      [WEAK]
                EXPORT  INTADASGL_IRQHandler      [WEAK]
                EXPORT  INTADACNT_IRQHandler      [WEAK]
                EXPORT  INTADBPDA_IRQHandler      [WEAK]
                EXPORT  INTADBPDB_IRQHandler      [WEAK]
                EXPORT  INTADBCP0_IRQHandler      [WEAK]
                EXPORT  INTADBCP1_IRQHandler      [WEAK]
                EXPORT  INTADBTRG_IRQHandler      [WEAK]
                EXPORT  INTADBSGL_IRQHandler      [WEAK]
                EXPORT  INTADBCNT_IRQHandler      [WEAK]
                EXPORT  INTADCPDA_IRQHandler      [WEAK]
                EXPORT  INTADCPDB_IRQHandler      [WEAK]
                EXPORT  INTADCCP0_IRQHandler      [WEAK]
                EXPORT  INTADCCP1_IRQHandler      [WEAK]
                EXPORT  INTADCTRG_IRQHandler      [WEAK]
                EXPORT  INTADCSGL_IRQHandler      [WEAK]
                EXPORT  INTADCCNT_IRQHandler      [WEAK]
                EXPORT  INTSC0RX_IRQHandler       [WEAK]
                EXPORT  INTSC0TX_IRQHandler       [WEAK]
                EXPORT  INTSC0ERR_IRQHandler      [WEAK]
                EXPORT  INTSC1RX_IRQHandler       [WEAK]
                EXPORT  INTSC1TX_IRQHandler       [WEAK]
                EXPORT  INTSC1ERR_IRQHandler      [WEAK]
                EXPORT  INTSC2RX_IRQHandler       [WEAK]
                EXPORT  INTSC2TX_IRQHandler       [WEAK]
                EXPORT  INTSC2ERR_IRQHandler      [WEAK]
                EXPORT  INTSC3RX_IRQHandler       [WEAK]
                EXPORT  INTSC3TX_IRQHandler       [WEAK]
                EXPORT  INTSC3ERR_IRQHandler      [WEAK]
                EXPORT  INTI2C0NST_IRQHandler     [WEAK]
                EXPORT  INTI2C0ATX_IRQHandler     [WEAK]
                EXPORT  INTI2C0BRX_IRQHandler     [WEAK]
                EXPORT  INTI2C0NA_IRQHandler      [WEAK]
                EXPORT  INTI2C1NST_IRQHandler     [WEAK]
                EXPORT  INTI2C1ATX_IRQHandler     [WEAK]
                EXPORT  INTI2C1BRX_IRQHandler     [WEAK]
                EXPORT  INTI2C1NA_IRQHandler      [WEAK]
                EXPORT  INTT32A00AC_IRQHandler    [WEAK]
                EXPORT  INTT32A00ACCAP0_IRQHandler[WEAK]
                EXPORT  INTT32A00ACCAP1_IRQHandler[WEAK]
                EXPORT  INTT32A00B_IRQHandler     [WEAK]
                EXPORT  INTT32A00BCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A00BCAP1_IRQHandler [WEAK]
                EXPORT  INTT32A01AC_IRQHandler    [WEAK]
                EXPORT  INTT32A01ACCAP0_IRQHandler[WEAK]
                EXPORT  INTT32A01ACCAP1_IRQHandler[WEAK]
                EXPORT  INTT32A01B_IRQHandler     [WEAK]
                EXPORT  INTT32A01BCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A01BCAP1_IRQHandler [WEAK]
                EXPORT  INTT32A02AC_IRQHandler    [WEAK]
                EXPORT  INTT32A02ACCAP0_IRQHandler[WEAK]
                EXPORT  INTT32A02ACCAP1_IRQHandler[WEAK]
                EXPORT  INTT32A02B_IRQHandler     [WEAK]
                EXPORT  INTT32A02BCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A02BCAP1_IRQHandler [WEAK]
                EXPORT  INTT32A03AC_IRQHandler    [WEAK]
                EXPORT  INTT32A03ACCAP0_IRQHandler[WEAK]
                EXPORT  INTT32A03ACCAP1_IRQHandler[WEAK]
                EXPORT  INTT32A03B_IRQHandler     [WEAK]
                EXPORT  INTT32A03BCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A03BCAP1_IRQHandler [WEAK]
                EXPORT  INTT32A04AC_IRQHandler    [WEAK]
                EXPORT  INTT32A04ACCAP0_IRQHandler[WEAK]
                EXPORT  INTT32A04ACCAP1_IRQHandler[WEAK]
                EXPORT  INTT32A04B_IRQHandler     [WEAK]
                EXPORT  INTT32A04BCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A04BCAP1_IRQHandler [WEAK]
                EXPORT  INTT32A05AC_IRQHandler    [WEAK]
                EXPORT  INTT32A05ACCAP0_IRQHandler[WEAK]
                EXPORT  INTT32A05ACCAP1_IRQHandler[WEAK]
                EXPORT  INTT32A05B_IRQHandler     [WEAK]
                EXPORT  INTT32A05BCAP0_IRQHandler [WEAK]
                EXPORT  INTT32A05BCAP1_IRQHandler [WEAK]
                EXPORT  INTPARI0_IRQHandler       [WEAK]
                EXPORT  INTPARI1_IRQHandler       [WEAK]
                EXPORT  INTDMAATC_IRQHandler      [WEAK]
                EXPORT  INTDMAAERR_IRQHandler     [WEAK]
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
INT17_IRQHandler
INT18_IRQHandler
INT21_IRQHandler
INTVCN0_IRQHandler
INTVCT0_IRQHandler
INTEMG0_IRQHandler
INTEMG1_IRQHandler
INTEMG2_IRQHandler
INTOVV0_IRQHandler
INTOVV1_IRQHandler
INTOVV2_IRQHandler
INTPWM0_IRQHandler
INTPWM1_IRQHandler
INTPWM2_IRQHandler
INTENC00_IRQHandler
INTENC01_IRQHandler
INTENC10_IRQHandler
INTENC11_IRQHandler
INTENC20_IRQHandler
INTENC21_IRQHandler
INTADAPDA_IRQHandler
INTADAPDB_IRQHandler
INTADACP0_IRQHandler
INTADACP1_IRQHandler
INTADATRG_IRQHandler
INTADASGL_IRQHandler
INTADACNT_IRQHandler
INTADBPDA_IRQHandler
INTADBPDB_IRQHandler
INTADBCP0_IRQHandler
INTADBCP1_IRQHandler
INTADBTRG_IRQHandler
INTADBSGL_IRQHandler
INTADBCNT_IRQHandler
INTADCPDA_IRQHandler
INTADCPDB_IRQHandler
INTADCCP0_IRQHandler
INTADCCP1_IRQHandler
INTADCTRG_IRQHandler
INTADCSGL_IRQHandler
INTADCCNT_IRQHandler
INTSC0RX_IRQHandler
INTSC0TX_IRQHandler
INTSC0ERR_IRQHandler
INTSC1RX_IRQHandler
INTSC1TX_IRQHandler
INTSC1ERR_IRQHandler
INTSC2RX_IRQHandler
INTSC2TX_IRQHandler
INTSC2ERR_IRQHandler
INTSC3RX_IRQHandler
INTSC3TX_IRQHandler
INTSC3ERR_IRQHandler
INTI2C0NST_IRQHandler
INTI2C0ATX_IRQHandler
INTI2C0BRX_IRQHandler
INTI2C0NA_IRQHandler
INTI2C1NST_IRQHandler
INTI2C1ATX_IRQHandler
INTI2C1BRX_IRQHandler
INTI2C1NA_IRQHandler
INTT32A00AC_IRQHandler
INTT32A00ACCAP0_IRQHandler
INTT32A00ACCAP1_IRQHandler
INTT32A00B_IRQHandler
INTT32A00BCAP0_IRQHandler
INTT32A00BCAP1_IRQHandler
INTT32A01AC_IRQHandler
INTT32A01ACCAP0_IRQHandler
INTT32A01ACCAP1_IRQHandler
INTT32A01B_IRQHandler
INTT32A01BCAP0_IRQHandler
INTT32A01BCAP1_IRQHandler
INTT32A02AC_IRQHandler
INTT32A02ACCAP0_IRQHandler
INTT32A02ACCAP1_IRQHandler
INTT32A02B_IRQHandler
INTT32A02BCAP0_IRQHandler
INTT32A02BCAP1_IRQHandler
INTT32A03AC_IRQHandler
INTT32A03ACCAP0_IRQHandler
INTT32A03ACCAP1_IRQHandler
INTT32A03B_IRQHandler
INTT32A03BCAP0_IRQHandler
INTT32A03BCAP1_IRQHandler
INTT32A04AC_IRQHandler
INTT32A04ACCAP0_IRQHandler
INTT32A04ACCAP1_IRQHandler
INTT32A04B_IRQHandler
INTT32A04BCAP0_IRQHandler
INTT32A04BCAP1_IRQHandler
INTT32A05AC_IRQHandler
INTT32A05ACCAP0_IRQHandler
INTT32A05ACCAP1_IRQHandler
INTT32A05B_IRQHandler
INTT32A05BCAP0_IRQHandler
INTT32A05BCAP1_IRQHandler
INTPARI0_IRQHandler
INTPARI1_IRQHandler
INTDMAATC_IRQHandler
INTDMAAERR_IRQHandler
INTFLCRDY_IRQHandler
INTFLDRDY_IRQHandler

                B       .

                ENDP


                ALIGN
                END

