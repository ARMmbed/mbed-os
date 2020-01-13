/* mbed Microcontroller Library 
 * Copyright (c) 2006-2020 ARM Limited 
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License. 
 * You may obtain a copy of the License at 
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0 
 * 
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 * See the License for the specific language governing permissions and 
 * limitations under the License. 
 */
;/****************************************************************************
; *
; * Copyright 2020 Samsung Electronics All Rights Reserved.
; * SPDX-License-Identifier: Apache-2.0
; *
; * Licensed under the Apache License, Version 2.0 (the "License");
; * you may not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; * http://www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing,
; * software distributed under the License is distributed on an
; * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
; * either express or implied. See the License for the specific
; * language governing permissions and limitations under the License.
; *
; ****************************************************************************/
/* @file  : startup_sidk_s5js100.S
 * @brief : start up code for GCC_ARM
 * @date  : June 2019
 *
 * @note  : Add chip dependent isr vectors and register handlers
 *
 */


/*
 * This file is derivative of CMSIS V5.00 startup_ARMCM3.S
 */
    .syntax unified
    .arch armv7-m

    .section .vector_table,"a",%progbits
    .align 2
    .globl __isr_vector
__isr_vector:
    .long    __StackTop            /* Top of Stack */
    .long    Reset_Handler         /* Reset Handler */
    .long    NMI_Handler           /* NMI Handler */
    .long    HardFault_Handler     /* Hard Fault Handler */
    .long    MemManage_Handler     /* MPU Fault Handler */
    .long    BusFault_Handler      /* Bus Fault Handler */
    .long    UsageFault_Handler    /* Usage Fault Handler */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    SVC_Handler           /* SVCall Handler */
    .long    DebugMon_Handler      /* Debug Monitor Handler */
    .long    0                     /* Reserved */
    .long    PendSV_Handler        /* PendSV Handler */
    .long    SysTick_Handler       /* SysTick Handler */

    /* External interrupts */
    .long    WDT_Handler            /* 0:Watchdog Timer Interrupt      */
    .long    PMU_APTIMER_Handler    /* 1:PMU ATimer wakeup source      */
    .long    PMU_ALIVEPAD_Handler   /* 2:PMU AlivePad wakeup source    */
    .long    PMU_JTAG_Handler       /* 3:PMU JTAG wakeup source        */
    .long    SSS_SSSINT_Handler     /* 4:SSS Secure Interrupt          */
    .long    SSS_MB_Handler         /* 5:SSS Mailbox Interrupt         */
    .long    SSS_KM_Handler         /* 6:SSS Key Manager Interrupt     */
    .long    DMAC_Handler           /* 7:PDMAC  Interrupt              */
    .long    SDIO_Handler           /* 8:SDIO CTRL Interrupt           */
    .long    TINT0_Handler          /* 9:ATIMER 0 Interrupt            */
    .long    TINT1_Handler          /* 10:ATIMER 1 Interrupt            */
    .long    TINT2_Handler          /* 11:ATIMER 2 Interrupt            */
    .long    TINT3_Handler          /* 12:ATIMER 3 Interrupt            */
    .long    TINT4_Handler          /* 13:ATIMER 4 Interrupt            */
    .long    TINT5_Handler          /* 14:ATIMER 5 Interrupt            */
    .long    GPIO_INTR0_Handler     /* 15:Gpio Group0 Interrupt         */
    .long    GPIO_INTR1_Handler     /* 16:Gpio Group1 Interrupt         */
    .long    GPIO_INTR2_Handler     /* 17:Gpio Group2 Interrupt         */
    .long    USI0_Handler           /* 18:USI 0 Interrupt               */
    .long    USI1_Handler           /* 19:USI 1 Interrupt               */
    .long    SPI_Handler            /* 20:SPI Interrupt                 */
    .long    I2C_Handler            /* 21:I2C Interrupt                 */
    .long    PWM0_Handler           /* 22:PWM Port0 Interrupt           */
    .long    PWM1_Handler           /* 23:PWM Port1 Interrupt           */
    .long    PWM2_Handler           /* 24:PWM Port2 Interrupt           */
    .long    PWM3_Handler           /* 25:PWM Port3 Interrupt           */
    .long    PWM4_Handler           /* 26:PWM Port4 Interrupt           */
    .long    PPMU_Handler           /* 27:Performance Monitor Interrupt */
    .long    EFUSE_WR_Handler       /* 28:Efuse Writer Interrupt        */
    .long    CM7_CTT0_Handler       /* 29:CM7 CTI0 Interrupt            */
    .long    CM7_CTT1_Handler       /* 30:CM7 CTI1 Interrupt            */
    .long    MB_AP_Handler          /* 31:Mailbox AP Interrupt          */
    .long    UART0_Handler          /* 32:UART0 Interrupt               */
    .long    UART1_Handler          /* 33:UART1 Interrupt               */
    .long    GPADC_Handler          /* 34:ADC Interrupt                 */
    .long    MCPU_WDT_Handler       /* 35:MCPU Watchdog Timer Interrupt */
    .long    SSS1_Handler           /* 36:SSS1 Host Interrupt           */
    .long    SSS2_Handler           /* 37:SSS2 Host Interrupt           */
    .long    SSS_RESET_Handler      /* 38:SSS Reset Interrupt           */
    .long    SLEEP_Handler          /* 39:SLEEP Counter Interrupt       */
    .long    TSU0_Handler           /* 40:TSU0 Interrupt                */
    .long    TSU1_Handler           /* 41:TSU1 Interrupt                */
    
    .size    __isr_vector, . - __isr_vector
    .section .text.Reset_Handler
    .thumb
    .thumb_func
    .align  2
    .globl   Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
	/*b	.   */
    ldr    r0, =SystemInit
    blx    r0
/* The call to uvisor_init() happens independently of uVisor being enabled or
* not, so it is conditionally compiled only based on FEATURE_UVISOR. */
#if defined(FEATURE_UVISOR) && defined(TARGET_UVISOR_SUPPORTED)
    /* Call uvisor_init() */
    ldr   r0, =uvisor_init
    blx   r0
#endif /* FEATURE_UVISOR && TARGET_UVISOR_SUPPORTED */
/*
 * Loop to copy data from read only memory to RAM. The ranges
 * of copy from/to are specified by following symbols evaluated in
 * linker script.
 * _etext: End of code section, i.e., begin of data sections to copy from.
 * __data_start__/__data_end__: RAM address range that data should be
 * copied to. Both must be aligned to 4 bytes boundary.
 */
    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

    subs   r3, r2
    ble    .Lflash_to_ram_loop_end

    movs    r4, 0
.Lflash_to_ram_loop:
    ldr    r0, [r1,r4]
    str    r0, [r2,r4]
    adds   r4, 4
    cmp    r4, r3
    blt    .Lflash_to_ram_loop
.Lflash_to_ram_loop_end:

/* copy ramfunc from FLASH to RAM */
	ldr	r1, =_framfuncs
	ldr	r2, =_sramfuncs
	ldr	r3, =_eramfuncs

	subs	r3, r2
	ble	.Lramfunc_loop_end

	movs	r4, 0
.Lramfunc_loop:
	ldr	r0, [r1,r4]
	str	r0, [r2,r4]
	adds	r4, 4
	cmp	r4, r3
	blt	.Lramfunc_loop
.Lramfunc_loop_end:

/* Initialize .bss */
init_bss:
        ldr   r1, =__bss_start__
        ldr   r2, =__bss_end__
        ldr   r3, =bss_size

        cmp   r3, #0
        beq   system_startup

        mov   r4, #0
zero:
        strb  r4, [r1], #1
        subs  r3, r3, #1
        bne   zero

system_startup:
    ldr    r0, =SystemInit
    blx    r0
    ldr    r0, =_start
    bx    r0
    .pool
    .size Reset_Handler, . - Reset_Handler

    .text
/*
 * Macro to define default handlers. Default handler
 * will be weak symbol and just dead loops. They can be
 * overwritten by other handlers
 */
    .macro    def_default_handler    handler_name
    .align 1
    .thumb_func
    .weak    \handler_name
    .type    \handler_name, %function
\handler_name :
	nop
	nop
	nop
	nop
	nop
	nop
    b    .
	b		HardFault_Handler
	nop
	nop
	nop
	nop
	nop
	nop
	nop
    .size    \handler_name, . - \handler_name
    .endm

    def_default_handler    NMI_Handler
    def_default_handler    HardFault_Handler
    def_default_handler    MemManage_Handler
    def_default_handler    BusFault_Handler
    def_default_handler    UsageFault_Handler
    def_default_handler    SVC_Handler
    def_default_handler    DebugMon_Handler
    def_default_handler    PendSV_Handler
    def_default_handler    SysTick_Handler
    def_default_handler    Default_Handler

    .macro    def_irq_default_handler    handler_name
    .weak     \handler_name
    .set      \handler_name, Default_Handler
    .endm

    /* External interrupts */
    def_irq_default_handler    WDT_Handler            /* 0:Watchdog Timer Interrupt      */
    def_irq_default_handler    PMU_APTIMER_Handler    /* 1:PMU ATimer wakeup source      */
    def_irq_default_handler    PMU_ALIVEPAD_Handler   /* 2:PMU AlivePad wakeup source    */
    def_irq_default_handler    PMU_JTAG_Handler       /* 3:PMU JTAG wakeup source        */
    def_irq_default_handler    SSS_SSSINT_Handler     /* 4:SSS Secure Interrupt          */
    def_irq_default_handler    SSS_MB_Handler         /* 5:SSS Mailbox Interrupt         */
    def_irq_default_handler    SSS_KM_Handler         /* 6:SSS Key Manager Interrupt     */
    def_irq_default_handler    DMAC_Handler           /* 7:PDMAC  Interrupt              */
    def_irq_default_handler    SDIO_Handler           /* 8:SDIO CTRL Interrupt           */
    def_irq_default_handler    TINT0_Handler          /* 9:ATIMER 0 Interrupt            */
    def_irq_default_handler    TINT1_Handler          /* 10:ATIMER 1 Interrupt            */
    def_irq_default_handler    TINT2_Handler          /* 11:ATIMER 2 Interrupt            */
    def_irq_default_handler    TINT3_Handler          /* 12:ATIMER 3 Interrupt            */
    def_irq_default_handler    TINT4_Handler          /* 13:ATIMER 4 Interrupt            */
    def_irq_default_handler    TINT5_Handler          /* 14:ATIMER 5 Interrupt            */
    def_irq_default_handler    GPIO_INTR0_Handler     /* 15:Gpio Group0 Interrupt         */
    def_irq_default_handler    GPIO_INTR1_Handler     /* 16:Gpio Group1 Interrupt         */
    def_irq_default_handler    GPIO_INTR2_Handler     /* 17:Gpio Group2 Interrupt         */
    def_irq_default_handler    USI0_Handler           /* 18:USI 0 Interrupt               */
    def_irq_default_handler    USI1_Handler           /* 19:USI 1 Interrupt               */
    def_irq_default_handler    SPI_Handler            /* 20:SPI Interrupt                 */
    def_irq_default_handler    I2C_Handler            /* 21:I2C Interrupt                 */
    def_irq_default_handler    PWM0_Handler           /* 22:PWM Port0 Interrupt           */
    def_irq_default_handler    PWM1_Handler           /* 23:PWM Port1 Interrupt           */
    def_irq_default_handler    PWM2_Handler           /* 24:PWM Port2 Interrupt           */
    def_irq_default_handler    PWM3_Handler           /* 25:PWM Port3 Interrupt           */
    def_irq_default_handler    PWM4_Handler           /* 26:PWM Port4 Interrupt           */
    def_irq_default_handler    PPMU_Handler           /* 27:Performance Monitor Interrupt */
    def_irq_default_handler    EFUSE_WR_Handler       /* 28:Efuse Writer Interrupt        */
    def_irq_default_handler    CM7_CTT0_Handler       /* 29:CM7 CTI0 Interrupt            */
    def_irq_default_handler    CM7_CTT1_Handler       /* 30:CM7 CTI1 Interrupt            */
    def_irq_default_handler    MB_AP_Handler          /* 31:Mailbox AP Interrupt          */
    def_irq_default_handler    UART0_Handler          /* 32:UART0 Interrupt               */
    def_irq_default_handler    UART1_Handler          /* 33:UART1 Interrupt               */
    def_irq_default_handler    GPADC_Handler          /* 34:ADC Interrupt                 */
    def_irq_default_handler    MCPU_WDT_Handler       /* 35:MCPU Watchdog Timer Interrupt */
    def_irq_default_handler    SSS1_Handler           /* 36:SSS1 Host Interrupt           */
    def_irq_default_handler    SSS2_Handler           /* 37:SSS2 Host Interrupt           */
    def_irq_default_handler    SSS_RESET_Handler      /* 38:SSS Reset Interrupt           */
    def_irq_default_handler    SLEEP_Handler          /* 39:SLEEP Counter Interrupt       */
    def_irq_default_handler    TSU0_Handler           /* 40:TSU0 Interrupt                */
    def_irq_default_handler    TSU1_Handler           /* 41:TSU1 Interrupt                */

    .end
