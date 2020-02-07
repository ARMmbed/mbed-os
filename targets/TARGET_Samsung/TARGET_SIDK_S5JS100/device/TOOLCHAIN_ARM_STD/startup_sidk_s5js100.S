/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size
                IMPORT  |Image$$ARM_LIB_STACK$$ZI$$Limit|

__Vectors       DCD     |Image$$ARM_LIB_STACK$$ZI$$Limit|  ; Top of Stack
                DCD     Reset_Handler              ; Reset Handler
                DCD     NMI_Handler                ; NMI Handler
                DCD     HardFault_Handler          ; Hard Fault Handler
                DCD     MemManage_Handler          ; MPU Fault Handler
                DCD     BusFault_Handler           ; Bus Fault Handler
                DCD     UsageFault_Handler         ; Usage Fault Handler
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     SVC_Handler                ; SVCall Handler
                DCD     DebugMon_Handler           ; Debug Monitor Handler
                DCD     0                          ; Reserved
                DCD     PendSV_Handler             ; PendSV Handler
                DCD     SysTick_Handler            ; SysTick Handler

                ; External Interrupts
				/* External interrupts */
				DCD    WDT_Handler            /* 0:Watchdog Timer Interrupt      */
				DCD    PMU_APTIMER_Handler    /* 1:PMU ATimer wakeup source      */
				DCD    PMU_ALIVEPAD_Handler   /* 2:PMU AlivePad wakeup source    */
				DCD    PMU_JTAG_Handler       /* 3:PMU JTAG wakeup source        */
				DCD    SSS_SSSINT_Handler     /* 4:SSS Secure Interrupt          */
				DCD    SSS_MB_Handler         /* 5:SSS Mailbox Interrupt         */
				DCD    SSS_KM_Handler         /* 6:SSS Key Manager Interrupt     */
				DCD    DMAC_Handler           /* 7:PDMAC  Interrupt              */
				DCD    SDIO_Handler           /* 8:SDIO CTRL Interrupt           */
				DCD    TINT0_Handler          /* 9:ATIMER 0 Interrupt            */
				DCD    TINT1_Handler          /* 10:ATIMER 1 Interrupt            */
				DCD    TINT2_Handler          /* 11:ATIMER 2 Interrupt            */
				DCD    TINT3_Handler          /* 12:ATIMER 3 Interrupt            */
				DCD    TINT4_Handler          /* 13:ATIMER 4 Interrupt            */
				DCD    TINT5_Handler          /* 14:ATIMER 5 Interrupt            */
				DCD    GPIO_INTR0_Handler     /* 15:Gpio Group0 Interrupt         */
				DCD    GPIO_INTR1_Handler     /* 16:Gpio Group1 Interrupt         */
				DCD    GPIO_INTR2_Handler     /* 17:Gpio Group2 Interrupt         */
				DCD    USI0_Handler           /* 18:USI 0 Interrupt               */
				DCD    USI1_Handler           /* 19:USI 1 Interrupt               */
				DCD    SPI_Handler            /* 20:SPI Interrupt                 */
				DCD    I2C_Handler            /* 21:I2C Interrupt                 */
				DCD    PWM0_Handler           /* 22:PWM Port0 Interrupt           */
				DCD    PWM1_Handler           /* 23:PWM Port1 Interrupt           */
				DCD    PWM2_Handler           /* 24:PWM Port2 Interrupt           */
				DCD    PWM3_Handler           /* 25:PWM Port3 Interrupt           */
				DCD    PWM4_Handler           /* 26:PWM Port4 Interrupt           */
				DCD    PPMU_Handler           /* 27:Performance Monitor Interrupt */
				DCD    EFUSE_WR_Handler       /* 28:Efuse Writer Interrupt        */
				DCD    CM7_CTT0_Handler       /* 29:CM7 CTI0 Interrupt            */
				DCD    CM7_CTT1_Handler       /* 30:CM7 CTI1 Interrupt            */
				DCD    MB_AP_Handler          /* 31:Mailbox AP Interrupt          */
				DCD    UART0_Handler          /* 32:UART0 Interrupt               */
				DCD    UART1_Handler          /* 33:UART1 Interrupt               */
				DCD    GPADC_Handler          /* 34:ADC Interrupt                 */
				DCD    MCPU_WDT_Handler       /* 35:MCPU Watchdog Timer Interrupt */
				DCD    SSS1_Handler           /* 36:SSS1 Host Interrupt           */
				DCD    SSS2_Handler           /* 37:SSS2 Host Interrupt           */
				DCD    SSS_RESET_Handler      /* 38:SSS Reset Interrupt           */
				DCD    SLEEP_Handler          /* 39:SLEEP Counter Interrupt       */
				DCD    TSU0_Handler           /* 40:TSU0 Interrupt                */
				DCD    TSU1_Handler           /* 41:TSU1 Interrupt                */
 
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; Reset handler
Reset_Handler    PROC
                 EXPORT  Reset_Handler             [WEAK]
        IMPORT  SystemInit
        IMPORT  __main
                 LDR     R0, =SystemInit
                 BLX     R0
                 LDR     SP, =|Image$$ARM_LIB_STACK$$ZI$$Limit| ;ARMCC should intentionally change stack point?
                 LDR     R0, =__main
                 BX      R0
                 ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler                [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler          [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler          [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler           [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler           [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler             [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler            [WEAK]
                B       .
                ENDP

Default_Handler PROC
			/* External interrupts */
			EXPORT    WDT_Handler            [WEAK]/* 0:Watchdog Timer Interrupt      */
			EXPORT    PMU_APTIMER_Handler    [WEAK]/* 1:PMU ATimer wakeup source      */
			EXPORT    PMU_ALIVEPAD_Handler   [WEAK]/* 2:PMU AlivePad wakeup source    */
			EXPORT    PMU_JTAG_Handler       [WEAK]/* 3:PMU JTAG wakeup source        */
			EXPORT    SSS_SSSINT_Handler     [WEAK]/* 4:SSS Secure Interrupt          */
			EXPORT    SSS_MB_Handler         [WEAK]/* 5:SSS Mailbox Interrupt         */
			EXPORT    SSS_KM_Handler         [WEAK]/* 6:SSS Key Manager Interrupt     */
			EXPORT    DMAC_Handler           [WEAK]/* 7:PDMAC  Interrupt              */
			EXPORT    SDIO_Handler           [WEAK]/* 8:SDIO CTRL Interrupt           */
			EXPORT    TINT0_Handler          [WEAK]/* 9:ATIMER 0 Interrupt            */
			EXPORT    TINT1_Handler          [WEAK]/* 10:ATIMER 1 Interrupt            */
			EXPORT    TINT2_Handler          [WEAK]/* 11:ATIMER 2 Interrupt            */
			EXPORT    TINT3_Handler          [WEAK]/* 12:ATIMER 3 Interrupt            */
			EXPORT    TINT4_Handler          [WEAK]/* 13:ATIMER 4 Interrupt            */
			EXPORT    TINT5_Handler          [WEAK]/* 14:ATIMER 5 Interrupt            */
			EXPORT    GPIO_INTR0_Handler     [WEAK]/* 15:Gpio Group0 Interrupt         */
			EXPORT    GPIO_INTR1_Handler     [WEAK]/* 16:Gpio Group1 Interrupt         */
			EXPORT    GPIO_INTR2_Handler     [WEAK]/* 17:Gpio Group2 Interrupt         */
			EXPORT    USI0_Handler           [WEAK]/* 18:USI 0 Interrupt               */
			EXPORT    USI1_Handler           [WEAK]/* 19:USI 1 Interrupt               */
			EXPORT    SPI_Handler            [WEAK]/* 20:SPI Interrupt                 */
			EXPORT    I2C_Handler            [WEAK]/* 21:I2C Interrupt                 */
			EXPORT    PWM0_Handler           [WEAK]/* 22:PWM Port0 Interrupt           */
			EXPORT    PWM1_Handler           [WEAK]/* 23:PWM Port1 Interrupt           */
			EXPORT    PWM2_Handler           [WEAK]/* 24:PWM Port2 Interrupt           */
			EXPORT    PWM3_Handler           [WEAK]/* 25:PWM Port3 Interrupt           */
			EXPORT    PWM4_Handler           [WEAK]/* 26:PWM Port4 Interrupt           */
			EXPORT    PPMU_Handler           [WEAK]/* 27:Performance Monitor Interrupt */
			EXPORT    EFUSE_WR_Handler       [WEAK]/* 28:Efuse Writer Interrupt        */
			EXPORT    CM7_CTT0_Handler       [WEAK]/* 29:CM7 CTI0 Interrupt            */
			EXPORT    CM7_CTT1_Handler       [WEAK]/* 30:CM7 CTI1 Interrupt            */
			EXPORT    MB_AP_Handler          [WEAK]/* 31:Mailbox AP Interrupt          */
			EXPORT    UART0_Handler          [WEAK]/* 32:UART0 Interrupt               */
			EXPORT    UART1_Handler          [WEAK]/* 33:UART1 Interrupt               */
			EXPORT    GPADC_Handler          [WEAK]/* 34:ADC Interrupt                 */
			EXPORT    MCPU_WDT_Handler       [WEAK]/* 35:MCPU Watchdog Timer Interrupt */
			EXPORT    SSS1_Handler           [WEAK]/* 36:SSS1 Host Interrupt           */
			EXPORT    SSS2_Handler           [WEAK]/* 37:SSS2 Host Interrupt           */
			EXPORT    SSS_RESET_Handler      [WEAK]/* 38:SSS Reset Interrupt           */
			EXPORT    SLEEP_Handler          [WEAK]/* 39:SLEEP Counter Interrupt       */
			EXPORT    TSU0_Handler           [WEAK]/* 40:TSU0 Interrupt                */
			EXPORT    TSU1_Handler           [WEAK]/* 41:TSU1 Interrupt                */
WDT_Handler           
PMU_APTIMER_Handler   
PMU_ALIVEPAD_Handler  
PMU_JTAG_Handler      
SSS_SSSINT_Handler    
SSS_MB_Handler        
SSS_KM_Handler        
DMAC_Handler          
SDIO_Handler          
TINT0_Handler         
TINT1_Handler         
TINT2_Handler         
TINT3_Handler         
TINT4_Handler         
TINT5_Handler         
GPIO_INTR0_Handler    
GPIO_INTR1_Handler    
GPIO_INTR2_Handler    
USI0_Handler          
USI1_Handler          
SPI_Handler           
I2C_Handler           
PWM0_Handler          
PWM1_Handler          
PWM2_Handler          
PWM3_Handler          
PWM4_Handler          
PPMU_Handler          
EFUSE_WR_Handler      
CM7_CTT0_Handler      
CM7_CTT1_Handler      
MB_AP_Handler         
UART0_Handler         
UART1_Handler         
GPADC_Handler         
MCPU_WDT_Handler      
SSS1_Handler          
SSS2_Handler          
SSS_RESET_Handler     
SLEEP_Handler         
TSU0_Handler          
TSU1_Handler          
                B       .

                ENDP

                ALIGN
                END

;************************ (C) COPYRIGHT Samsung electronics *****END OF FILE*****

