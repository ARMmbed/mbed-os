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


/************************************************************************************
 * Version: GCC for ARM Embedded Processors
 ************************************************************************************/
	.syntax	unified
	.arch	armv7-m
/************************************************************************************
 * Vectors
 ************************************************************************************/
	.section .vectors, "a"
	.align 2
	.globl	__Vectors
__Vectors:
	.long	__StackTop              /* Top of Stack */
	.long	Reset_Handler           /* Reset Handler */
	.long	NMI_Handler             /* NMI Handler */
	.long	HardFault_Handler       /* Hard Fault Handler */
	.long	MemManage_Handler       /* MPU Fault Handler */
	.long	BusFault_Handler        /* Bus Fault Handler */
	.long	UsageFault_Handler      /* Usage Fault Handler */
	.long	0                       /* Reserved */
	.long	0                       /* Reserved */
	.long	0                       /* Reserved */
	.long	0                       /* Reserved */
	.long	SVC_Handler             /* SVCall Handler */
	.long	DebugMon_Handler        /* Debug Monitor Handler */
	.long	0                       /* Reserved */
	.long	PendSV_Handler          /* PendSV Handler */
	.long	SysTick_Handler         /* SysTick Handler */

	/* External interrupts */
	.long	BOR_Handler             /* 00: Brown Out Reset Handler */
	.long	UART0_Handler           /* 01: UART 0 Handler */
	.long	UART1_Handler           /* 02: UART 1 Handler */
	.long	UART2_Handler           /* 03: UART 2 Handler */
	.long	SPI0_Handler            /* 04: SPI 0 Handler */
	.long	SPI1_Handler            /* 05: SPI 1 Handler */
	.long	SPI2_Handler            /* 06: SPI 2 Handler */
	.long	SPI3_Handler            /* 07: SPI 3 Handler */
	.long	SPI4_Handler            /* 08: SPI 4 Handler */
	.long	I2C0_Handler            /* 09: I2C 0 Handler */
	.long	I2C1_Handler            /* 10: I2C 1 Handler */
	.long	I2C2_Handler            /* 11: I2C 2 Handler */
	.long	I2C3_Handler            /* 12: I2C 3 Handler */
	.long	I2C4_Handler            /* 13: I2C 4 Handler */
	.long	TIMER0_Handler          /* 14: TIMER 0 Handler */
	.long	TIMER1_Handler          /* 15: TIMER 1 Handler */
	.long	TIMER2_Handler          /* 16: TIMER 2 Handler */
	.long	TIMER3_Handler          /* 17: TIMER 3 Handler */
	.long	TIMER4_Handler          /* 18: TIMER 4 Handler */
	.long	TIMER5_Handler          /* 19: TIMER 5 Handler */
	.long	TIMER6_Handler          /* 20: TIMER 6 Handler */
	.long	PWM_TIMER_Handler       /* 21: PWM TIMER Handler */
	.long	DUAL_TIMER_Handler      /* 22: Dual TIMER Handler */
	.long	AFECON_Handler          /* 23: Analog Front End Control Handler */
	.long	SRC_Handler             /* 24: Sample Rate Converter Handler */
	.long	DMA_ERROR_Handler       /* 25: DMA Error Handler */
	.long	FLASH_Handler           /* 26: FLASH Handler */
	.long	RTC0_Handler            /* 27: Real-Time Counter 0 Handler */
	.long	RTC1_Handler            /* 28: Real-Time Counter 1 Handler */
	.long	RTC2_Handler            /* 29: Real-Time Counter 2 Handler */
	.long	FPU_Handler             /* 30: Cortex-M4 Floating-point Processing Unit exceptions Handler */
	.long	SRP0_Handler            /* 31: SRP 0 Handler */
	.long	SRP1_Handler            /* 32: SRP 1 Handler */
	.long	DELAY_MON_Handler       /* 33: Delay Monitor Handler */
	.long	AES_Handler             /* 34: AES Handler */
	.long	EXT_WAKEUP_Handler      /* 35: External Wakeup Handler */
	.long	DMA0_Handler            /* 36: DMA 0 Handler */
	.long	DMA1_Handler            /* 37: DMA 1 Handler */
	.long	DMA2_Handler            /* 38: DMA 2 Handler */
	.long	DMA3_Handler            /* 39: DMA 3 Handler */
	.long	DMA4_Handler            /* 40: DMA 4 Handler */
	.long	DMA5_Handler            /* 41: DMA 5 Handler */
	.long	DMA6_Handler            /* 42: DMA 6 Handler */
	.long	DMA7_Handler            /* 43: DMA 7 Handler */
	.long	DMA8_Handler            /* 44: DMA 8 Handler */
	.long	DMA9_Handler            /* 45: DMA 9 Handler */
	.long	DMA10_Handler           /* 46: DMA 10 Handler */
	.long	DMA11_Handler           /* 47: DMA 11 Handler */
	.long	DMA12_Handler           /* 48: DMA 12 Handler */
	.long	DMA13_Handler           /* 49: DMA 13 Handler */
	.long	DMA14_Handler           /* 50: DMA 14 Handler */
	.long	DMA15_Handler           /* 51: DMA 15 Handler */
	.long	DMA16_Handler           /* 52: DMA 16 Handler */
	.long	DMA17_Handler           /* 53: DMA 17 Handler */
	.long	DMA18_Handler           /* 54: DMA 18 Handler */
	.long	DMA19_Handler           /* 55: DMA 19 Handler */
	.long	DMA20_Handler           /* 56: DMA 20 Handler */
	.long	DMA21_Handler           /* 57: DMA 21 Handler */
	.long	DMA22_Handler           /* 58: DMA 22 Handler */
	.long	DMA23_Handler           /* 59: DMA 23 Handler */
	.long	DMA24_Handler           /* 60: DMA 24 Handler */
	.long	DMA25_Handler           /* 61: DMA 25 Handler */
	.long	DMA26_Handler           /* 62: DMA 26 Handler */
	.long	DMA27_Handler           /* 63: DMA 27 Handler */
	.long	DMA28_Handler           /* 64: DMA 28 Handler */
	.long	DMA29_Handler           /* 65: DMA 29 Handler */
	.long	DMA30_Handler           /* 66: DMA 30 Handler */
	.long	DMA31_Handler           /* 67: DMA 31 Handler */
	.long	PORT0_Handler           /* 68: PORT0 Handler */
	.long	PORT1_Handler           /* 69: PORT1 Handler */
	.long	PORT2_Handler           /* 70: PORT2 Handler */
	.long	PORT0_0_Handler         /* 71: GPIO Port 0 pin 0 Handler */
	.long	PORT0_1_Handler         /* 72: GPIO Port 0 pin 1 Handler */
	.long	PORT0_2_Handler         /* 73: GPIO Port 0 pin 2 Handler */
	.long	PORT0_3_Handler         /* 74: GPIO Port 0 pin 3 Handler */
	.long	PORT0_4_Handler         /* 75: GPIO Port 0 pin 4 Handler */
	.long	PORT0_5_Handler         /* 76: GPIO Port 0 pin 5 Handler */
	.long	PORT0_6_Handler         /* 77: GPIO Port 0 pin 6 Handler */
	.long	PORT0_7_Handler         /* 78: GPIO Port 0 pin 7 Handler */
	.long	PORT0_8_Handler         /* 79: GPIO Port 0 pin 8 Handler */
	.long	PORT0_9_Handler         /* 80: GPIO Port 0 pin 9 Handler */
	.long	PORT0_10_Handler        /* 81: GPIO Port 0 pin 10 Handler */
	.long	PORT0_11_Handler        /* 82: GPIO Port 0 pin 11 Handler */
	.long	PORT0_12_Handler        /* 83: GPIO Port 0 pin 12 Handler */
	.long	PORT0_13_Handler        /* 84: GPIO Port 0 pin 13 Handler */
	.long	PORT0_14_Handler        /* 85: GPIO Port 0 pin 14 Handler */
	.long	PORT0_15_Handler        /* 86: GPIO Port 0 pin 15 Handler */
	.long	PORT1_0_Handler         /* 87: GPIO Port 1 pin 0 Handler */
	.long	PORT1_1_Handler         /* 88: GPIO Port 1 pin 1 Handler */
	.long	PORT1_2_Handler         /* 89: GPIO Port 1 pin 2 Handler */
	.long	PORT1_3_Handler         /* 90: GPIO Port 1 pin 3 Handler */
	.long	PORT1_4_Handler         /* 91: GPIO Port 1 pin 4 Handler */
	.long	PORT1_5_Handler         /* 92: GPIO Port 1 pin 5 Handler */
	.long	PORT1_6_Handler         /* 93: GPIO Port 1 pin 6 Handler */
	.long	PORT1_7_Handler         /* 94: GPIO Port 1 pin 7 Handler */
	.long	PORT1_8_Handler         /* 95: GPIO Port 1 pin 8 Handler */
	.long	PORT1_9_Handler         /* 96: GPIO Port 1 pin 9 Handler */
	.long	PORT1_10_Handler        /* 97: GPIO Port 1 pin 10 Handler */
	.long	PORT1_11_Handler        /* 98: GPIO Port 1 pin 11 Handler */
	.long	PORT1_12_Handler        /* 99: GPIO Port 1 pin 12 Handler */
	.long	PORT1_13_Handler        /* 100: GPIO Port 1 pin 13 Handler */
	.long	PORT1_14_Handler        /* 101: GPIO Port 1 pin 14 Handler */
	.long	PORT1_15_Handler        /* 102: GPIO Port 1 pin 15 Handler */
	.long	PORT2_0_Handler         /* 103: GPIO Port 2 pin 0 Handler */
	.long	PORT2_1_Handler         /* 104: GPIO Port 2 pin 1 Handler */
	.long	PORT2_2_Handler         /* 105: GPIO Port 2 pin 2 Handler */
	.long	PORT2_3_Handler         /* 106: GPIO Port 2 pin 3 Handler */
	.long	PORT2_4_Handler         /* 107: GPIO Port 2 pin 4 Handler */
	.long	PORT2_5_Handler         /* 108: GPIO Port 2 pin 5 Handler */
	.long	PORT2_6_Handler         /* 109: GPIO Port 2 pin 6 Handler */
	.long	PORT2_7_Handler         /* 110: GPIO Port 2 pin 7 Handler */
	.long	PORT2_8_Handler         /* 111: GPIO Port 2 pin 8 Handler */
	.long	PORT2_9_Handler         /* 112: GPIO Port 2 pin 9 Handler */
	.long	PORT2_10_Handler        /* 113: GPIO Port 2 pin 10 Handler */
	.long	PORT2_11_Handler        /* 114: GPIO Port 2 pin 11 Handler */
	.long	PORT2_12_Handler        /* 115: GPIO Port 2 pin 12 Handler */
	.long	PORT2_13_Handler        /* 116: GPIO Port 2 pin 13 Handler */
	.long	PORT2_14_Handler        /* 117: GPIO Port 2 pin 14 Handler */
	.long	PORT2_15_Handler        /* 118: GPIO Port 2 pin 15 Handler */
	.size	__Vectors, . - __Vectors

/************************************************************************************
 * text
 ************************************************************************************/
	.text
	.thumb
	.thumb_func
	.align	2
	.globl	Reset_Handler
	.type	Reset_Handler, %function

Reset_Handler:
    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

    subs   r3, r2
    ble    .Lflash_to_ram_loop_end

    movs   r4, 0
.Lflash_to_ram_loop:
    ldr    r0, [r1,r4]
    str    r0, [r2,r4]
    adds   r4, 4
    cmp    r4, r3
    blt    .Lflash_to_ram_loop
.Lflash_to_ram_loop_end:

/* Initialize .bss */
init_bss:
    ldr    r1, =__bss_start__
    ldr    r2, =__bss_end__
    ldr    r3, =bss_size

    cmp    r3, #0
    beq    system_startup

    mov    r4, #0
zero:
    strb   r4, [r1], #1
    subs   r3, r3, #1
    bne    zero

system_startup:
    ldr    r0, =SystemInit
    blx    r0
    ldr    r0, =_start
    bx     r0
    .pool
    .size Reset_Handler, . - Reset_Handler

    .text

/* Function to define default IRQ handler*/
    .align  1
    .thumb_func
    .weak Default_Handler
    .type Default_Handler, %function
Default_Handler:
    b Default_Handler
    .size Default_Handler, . - Default_Handler

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro def_irq_handler  handler_name
    .weak \handler_name
    .set  \handler_name, Default_Handler
    .endm

	def_irq_handler	NMI_Handler
	def_irq_handler	HardFault_Handler
	def_irq_handler	MemManage_Handler
	def_irq_handler	BusFault_Handler
	def_irq_handler	UsageFault_Handler
	def_irq_handler	SVC_Handler
	def_irq_handler	DebugMon_Handler
	def_irq_handler	PendSV_Handler
	def_irq_handler	SysTick_Handler

/* IRQ Handlers */
	def_irq_handler	BOR_Handler
	def_irq_handler	UART0_Handler
	def_irq_handler	UART1_Handler
	def_irq_handler	UART2_Handler
	def_irq_handler	SPI0_Handler
	def_irq_handler	SPI1_Handler
	def_irq_handler	SPI2_Handler
	def_irq_handler	SPI3_Handler
	def_irq_handler	SPI4_Handler
	def_irq_handler	I2C0_Handler
	def_irq_handler	I2C1_Handler
	def_irq_handler	I2C2_Handler
	def_irq_handler	I2C3_Handler
	def_irq_handler	I2C4_Handler
	def_irq_handler	TIMER0_Handler
	def_irq_handler	TIMER1_Handler
	def_irq_handler	TIMER2_Handler
	def_irq_handler	TIMER3_Handler
	def_irq_handler	TIMER4_Handler
	def_irq_handler	TIMER5_Handler
	def_irq_handler	TIMER6_Handler
	def_irq_handler	PWM_TIMER_Handler
	def_irq_handler	DUAL_TIMER_Handler
	def_irq_handler	AFECON_Handler
	def_irq_handler	SRC_Handler
	def_irq_handler	DMA_ERROR_Handler
	def_irq_handler	FLASH_Handler
	def_irq_handler	RTC0_Handler
	def_irq_handler	RTC1_Handler
	def_irq_handler	RTC2_Handler
	def_irq_handler	FPU_Handler
	def_irq_handler	SRP0_Handler
	def_irq_handler	SRP1_Handler
	def_irq_handler	DELAY_MON_Handler
	def_irq_handler	AES_Handler
	def_irq_handler	EXT_WAKEUP_Handler
	def_irq_handler	DMA0_Handler
	def_irq_handler	DMA1_Handler
	def_irq_handler	DMA2_Handler
	def_irq_handler	DMA3_Handler
	def_irq_handler	DMA4_Handler
	def_irq_handler	DMA5_Handler
	def_irq_handler	DMA6_Handler
	def_irq_handler	DMA7_Handler
	def_irq_handler	DMA8_Handler
	def_irq_handler	DMA9_Handler
	def_irq_handler	DMA10_Handler
	def_irq_handler	DMA11_Handler
	def_irq_handler	DMA12_Handler
	def_irq_handler	DMA13_Handler
	def_irq_handler	DMA14_Handler
	def_irq_handler	DMA15_Handler
	def_irq_handler	DMA16_Handler
	def_irq_handler	DMA17_Handler
	def_irq_handler	DMA18_Handler
	def_irq_handler	DMA19_Handler
	def_irq_handler	DMA20_Handler
	def_irq_handler	DMA21_Handler
	def_irq_handler	DMA22_Handler
	def_irq_handler	DMA23_Handler
	def_irq_handler	DMA24_Handler
	def_irq_handler	DMA25_Handler
	def_irq_handler	DMA26_Handler
	def_irq_handler	DMA27_Handler
	def_irq_handler	DMA28_Handler
	def_irq_handler	DMA29_Handler
	def_irq_handler	DMA30_Handler
	def_irq_handler	DMA31_Handler
	def_irq_handler	PORT0_Handler
	def_irq_handler	PORT1_Handler
	def_irq_handler	PORT2_Handler
	def_irq_handler PORT0_0_Handler
	def_irq_handler PORT0_1_Handler
	def_irq_handler PORT0_2_Handler
	def_irq_handler PORT0_3_Handler
	def_irq_handler PORT0_4_Handler
	def_irq_handler PORT0_5_Handler
	def_irq_handler PORT0_6_Handler
	def_irq_handler PORT0_7_Handler
	def_irq_handler PORT0_8_Handler
	def_irq_handler PORT0_9_Handler
	def_irq_handler PORT0_10_Handler
	def_irq_handler PORT0_11_Handler
	def_irq_handler PORT0_12_Handler
	def_irq_handler PORT0_13_Handler
	def_irq_handler PORT0_14_Handler
	def_irq_handler PORT0_15_Handler
	def_irq_handler PORT1_0_Handler
	def_irq_handler PORT1_1_Handler
	def_irq_handler PORT1_2_Handler
	def_irq_handler PORT1_3_Handler
	def_irq_handler PORT1_4_Handler
	def_irq_handler PORT1_5_Handler
	def_irq_handler PORT1_6_Handler
	def_irq_handler PORT1_7_Handler
	def_irq_handler PORT1_8_Handler
	def_irq_handler PORT1_9_Handler
	def_irq_handler PORT1_10_Handler
	def_irq_handler PORT1_11_Handler
	def_irq_handler PORT1_12_Handler
	def_irq_handler PORT1_13_Handler
	def_irq_handler PORT1_14_Handler
	def_irq_handler PORT1_15_Handler
	def_irq_handler PORT2_0_Handler
	def_irq_handler PORT2_1_Handler
	def_irq_handler PORT2_2_Handler
	def_irq_handler PORT2_3_Handler
	def_irq_handler PORT2_4_Handler
	def_irq_handler PORT2_5_Handler
	def_irq_handler PORT2_6_Handler
	def_irq_handler PORT2_7_Handler
	def_irq_handler PORT2_8_Handler
	def_irq_handler PORT2_9_Handler
	def_irq_handler PORT2_10_Handler
	def_irq_handler PORT2_11_Handler
	def_irq_handler PORT2_12_Handler
	def_irq_handler PORT2_13_Handler
	def_irq_handler PORT2_14_Handler
	def_irq_handler PORT2_15_Handler

	.end
