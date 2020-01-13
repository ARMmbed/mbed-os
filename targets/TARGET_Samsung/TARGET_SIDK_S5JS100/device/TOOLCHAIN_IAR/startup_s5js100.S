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


        MODULE  ?cstartup
        SECTION .isr_vector:DATA:NOROOT(3)

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit      
        EXTERN  _start
        PUBLIC  __vector_table        
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size
 
        DATA

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler

        DCD     NMI_Handler
        DCD     HardFault_Handler
        DCD     MemManage_Handler
        DCD     BusFault_Handler
        DCD     UsageFault_Handler
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     SVC_Handler
        DCD     DebugMon_Handler
        DCD     0
        DCD     PendSV_Handler
        DCD     SysTick_Handler

        ; External Interrupts
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
__Vectors       EQU   __vector_table
__Vectors_Size  EQU   __Vectors_End - __Vectors


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:NOROOT:REORDER(2)
Reset_Handler
        LDR     R0, =sfe(CSTACK)
        MSR     MSP, R0
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
MemManage_Handler
        B MemManage_Handler

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
BusFault_Handler
        B BusFault_Handler

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
UsageFault_Handler
        B UsageFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
DebugMon_Handler
        B DebugMon_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SysTick_Handler
        B SysTick_Handler     

        PUBLIC Default_Handler
Default_Handler
	/* External interrupts */
        PUBWEAK    WDT_Handler            /* 0:Watchdog Timer Interrupt      */
        PUBWEAK    PMU_APTIMER_Handler    /* 1:PMU ATimer wakeup source      */
        PUBWEAK    PMU_ALIVEPAD_Handler   /* 2:PMU AlivePad wakeup source    */
        PUBWEAK    PMU_JTAG_Handler       /* 3:PMU JTAG wakeup source        */
        PUBWEAK    SSS_SSSINT_Handler     /* 4:SSS Secure Interrupt          */
        PUBWEAK    SSS_MB_Handler         /* 5:SSS Mailbox Interrupt         */
        PUBWEAK    SSS_KM_Handler         /* 6:SSS Key Manager Interrupt     */
        PUBWEAK    DMAC_Handler           /* 7:PDMAC  Interrupt              */
        PUBWEAK    SDIO_Handler           /* 8:SDIO CTRL Interrupt           */
        PUBWEAK    TINT0_Handler          /* 9:ATIMER 0 Interrupt            */
        PUBWEAK    TINT1_Handler          /* 10:ATIMER 1 Interrupt            */
        PUBWEAK    TINT2_Handler          /* 11:ATIMER 2 Interrupt            */
        PUBWEAK    TINT3_Handler          /* 12:ATIMER 3 Interrupt            */
        PUBWEAK    TINT4_Handler          /* 13:ATIMER 4 Interrupt            */
        PUBWEAK    TINT5_Handler          /* 14:ATIMER 5 Interrupt            */
        PUBWEAK    GPIO_INTR0_Handler     /* 15:Gpio Group0 Interrupt         */
        PUBWEAK    GPIO_INTR1_Handler     /* 16:Gpio Group1 Interrupt         */
        PUBWEAK    GPIO_INTR2_Handler     /* 17:Gpio Group2 Interrupt         */
        PUBWEAK    USI0_Handler           /* 18:USI 0 Interrupt               */
        PUBWEAK    USI1_Handler           /* 19:USI 1 Interrupt               */
        PUBWEAK    SPI_Handler            /* 20:SPI Interrupt                 */
        PUBWEAK    I2C_Handler            /* 21:I2C Interrupt                 */
        PUBWEAK    PWM0_Handler           /* 22:PWM Port0 Interrupt           */
        PUBWEAK    PWM1_Handler           /* 23:PWM Port1 Interrupt           */
        PUBWEAK    PWM2_Handler           /* 24:PWM Port2 Interrupt           */
        PUBWEAK    PWM3_Handler           /* 25:PWM Port3 Interrupt           */
        PUBWEAK    PWM4_Handler           /* 26:PWM Port4 Interrupt           */
        PUBWEAK    PPMU_Handler           /* 27:Performance Monitor Interrupt */
        PUBWEAK    EFUSE_WR_Handler       /* 28:Efuse Writer Interrupt        */
        PUBWEAK    CM7_CTT0_Handler       /* 29:CM7 CTI0 Interrupt            */
        PUBWEAK    CM7_CTT1_Handler       /* 30:CM7 CTI1 Interrupt            */
        PUBWEAK    MB_AP_Handler          /* 31:Mailbox AP Interrupt          */
        PUBWEAK    UART0_Handler          /* 32:UART0 Interrupt               */
        PUBWEAK    UART1_Handler          /* 33:UART1 Interrupt               */
        PUBWEAK    GPADC_Handler          /* 34:ADC Interrupt                 */
        PUBWEAK    MCPU_WDT_Handler       /* 35:MCPU Watchdog Timer Interrupt */
        PUBWEAK    SSS1_Handler           /* 36:SSS1 Host Interrupt           */
        PUBWEAK    SSS2_Handler           /* 37:SSS2 Host Interrupt           */
        PUBWEAK    SSS_RESET_Handler      /* 38:SSS Reset Interrupt           */
        PUBWEAK    SLEEP_Handler          /* 39:SLEEP Counter Interrupt       */
        PUBWEAK    TSU0_Handler           /* 40:TSU0 Interrupt                */
        PUBWEAK    TSU1_Handler           /* 41:TSU1 Interrupt                */
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

        END
