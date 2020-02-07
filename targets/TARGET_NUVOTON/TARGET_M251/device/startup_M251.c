/*
 * Copyright (c) 2019, Nuvoton Technology Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
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

#include "M251.h"

/* Suppress warning messages */
#if defined(__CC_ARM)
// Suppress warning message: extended constant initializer used
#pragma diag_suppress 1296
#elif defined(__ICCARM__)
#elif defined(__GNUC__)
#endif

/* Macro Definitions */
#if defined(__CC_ARM)
#define WEAK            __attribute__ ((weak))
#define ALIAS(f)        __attribute__ ((weak, alias(#f)))

#define WEAK_ALIAS_FUNC(FUN, FUN_ALIAS) \
void FUN(void) __attribute__ ((weak, alias(#FUN_ALIAS)));

#elif defined(__ICCARM__)
//#define STRINGIFY(x) #x
//#define _STRINGIFY(x) STRINGIFY(x)
#define WEAK_ALIAS_FUNC(FUN, FUN_ALIAS) \
void FUN(void);                         \
_Pragma(_STRINGIFY(_WEAK_ALIAS_FUNC(FUN, FUN_ALIAS)))
#define _WEAK_ALIAS_FUNC(FUN, FUN_ALIAS) weak __WEAK_ALIAS_FUNC(FUN, FUN_ALIAS)
#define __WEAK_ALIAS_FUNC(FUN, FUN_ALIAS) FUN##=##FUN_ALIAS

#elif defined(__GNUC__)
#define WEAK            __attribute__ ((weak))
#define ALIAS(f)        __attribute__ ((weak, alias(#f)))

#define WEAK_ALIAS_FUNC(FUN, FUN_ALIAS) \
void FUN(void) __attribute__ ((weak, alias(#FUN_ALIAS)));

#endif


/* Initialize segments */
#if defined(__ARMCC_VERSION)
extern uint32_t Image$$ARM_LIB_STACK$$ZI$$Limit;
extern void __main(void);
#elif defined(__ICCARM__)
void __iar_program_start(void);
#elif defined(__GNUC__)
extern uint32_t __StackTop;
extern uint32_t __copy_table_start__;
extern uint32_t __copy_table_end__;
extern uint32_t __zero_table_start__;
extern uint32_t __zero_table_end__;

#if defined(TOOLCHAIN_GCC_ARM)
extern void _start(void);
#else
#error("For GCC toolchain, only support GNU ARM Embedded")
#endif
#endif

/* Default empty handler */
void Default_Handler(void);

/* Reset handler */
void Reset_Handler(void);

/* Cortex-M23 core handlers */
WEAK_ALIAS_FUNC(NMI_Handler, Default_Handler)           // -14: Non Maskable Interrupt
WEAK_ALIAS_FUNC(HardFault_Handler, Default_Handler)     // -13: Cortex-M23 Hard Fault Interrupt
WEAK_ALIAS_FUNC(SVC_Handler, Default_Handler)           // -5: Cortex-M23 SV Call Interrupt 

WEAK_ALIAS_FUNC(PendSV_Handler, Default_Handler)        // -2: Cortex-M23 Pend SV Interrupt
WEAK_ALIAS_FUNC(SysTick_Handler, Default_Handler)       // -1: Cortex-M23 System Tick Interrupt

/* Peripherals handlers */
WEAK_ALIAS_FUNC(BOD_IRQHandler, Default_Handler)        // 0: Brown-Out Low Voltage Detected Interrupt
WEAK_ALIAS_FUNC(IRCTRIM_IRQHandler, Default_Handler)    // 1: Watch Dog Timer Interrupt
WEAK_ALIAS_FUNC(PWRWU_IRQHandler, Default_Handler)      // 2: EINT0, EINT2 and EINT4 Interrupt
                                                        // 3: Reserved  
WEAK_ALIAS_FUNC(CLKFAIL_IRQHandler, Default_Handler)    // 4: Clock fail dector Interrupt
                                                        // 5: Reserved  
WEAK_ALIAS_FUNC(RTC_IRQHandler, Default_Handler)        // 6: Real Time Clock Interrupt
WEAK_ALIAS_FUNC(TAMPER_IRQHandler, Default_Handler)     // 7: Tamper detection Interrupt
WEAK_ALIAS_FUNC(WDT_IRQHandler, Default_Handler)        // 8: Watch Dog Timer Interrupt
WEAK_ALIAS_FUNC(WWDT_IRQHandler, Default_Handler)       // 9: Window Watch Dog Timer Interrupt
WEAK_ALIAS_FUNC(EINT0_IRQHandler, Default_Handler)      // 10: External Input 0 Interrupt
WEAK_ALIAS_FUNC(EINT1_IRQHandler, Default_Handler)      // 11: External Input 1 Interrupt
WEAK_ALIAS_FUNC(EINT2_IRQHandler, Default_Handler)      // 12: External Input 2 Interrupt
WEAK_ALIAS_FUNC(EINT3_IRQHandler, Default_Handler)      // 13: External Input 3 Interrupt
WEAK_ALIAS_FUNC(EINT4_IRQHandler, Default_Handler)      // 14: External Input 4 Interrupt
WEAK_ALIAS_FUNC(EINT5_IRQHandler, Default_Handler)      // 15: External Input 5 Interrupt
WEAK_ALIAS_FUNC(GPA_IRQHandler, Default_Handler)        // 16: GPIO PORT A Interrupt
WEAK_ALIAS_FUNC(GPB_IRQHandler, Default_Handler)        // 17: GPIO PORT B Interrupt
WEAK_ALIAS_FUNC(GPC_IRQHandler, Default_Handler)        // 18: GPIO PORT C Interrupt
WEAK_ALIAS_FUNC(GPD_IRQHandler, Default_Handler)        // 19: GPIO PORT D Interrupt
WEAK_ALIAS_FUNC(GPE_IRQHandler, Default_Handler)        // 20: GPIO PORT E Interrupt
WEAK_ALIAS_FUNC(GPF_IRQHandler, Default_Handler)        // 21: GPIO PORT F Interrupt
WEAK_ALIAS_FUNC(QSPI0_IRQHandler, Default_Handler)      // 22: QSPI0 Interrupt
WEAK_ALIAS_FUNC(SPI0_IRQHandler, Default_Handler)       // 23: SPI0 Interrupt
WEAK_ALIAS_FUNC(BRAKE0_IRQHandler, Default_Handler)     // 24: PWM Brake0 Interrupt
WEAK_ALIAS_FUNC(PWM0_P0_IRQHandler, Default_Handler)    // 25: PWM0 P0 Interrupt
WEAK_ALIAS_FUNC(PWM0_P1_IRQHandler, Default_Handler)    // 26: PWM0 P1 Interrupt
WEAK_ALIAS_FUNC(PWM0_P2_IRQHandler, Default_Handler)    // 27: PWM0 P2 Interrupt
WEAK_ALIAS_FUNC(BRAKE1_IRQHandler, Default_Handler)     // 28: PWM Brake1 Interrupt
WEAK_ALIAS_FUNC(PWM1_P0_IRQHandler, Default_Handler)    // 29: PWM1 P0 Interrupt
WEAK_ALIAS_FUNC(PWM1_P1_IRQHandler, Default_Handler)    // 30: PWM1 P1 Interrupt
WEAK_ALIAS_FUNC(PWM1_P2_IRQHandler, Default_Handler)    // 31: PWM1 P2 Interrupt
WEAK_ALIAS_FUNC(TMR0_IRQHandler, Default_Handler)       // 32: TIMER0  Interrupt
WEAK_ALIAS_FUNC(TMR1_IRQHandler, Default_Handler)       // 33: TIMER1  Interrupt
WEAK_ALIAS_FUNC(TMR2_IRQHandler, Default_Handler)       // 34: TIMER2  Interrupt
WEAK_ALIAS_FUNC(TMR3_IRQHandler, Default_Handler)       // 35: TIMER3  Interrupt
WEAK_ALIAS_FUNC(UART0_IRQHandler, Default_Handler)      // 36: UART0  Interrupt
WEAK_ALIAS_FUNC(UART1_IRQHandler, Default_Handler)      // 37: UART1  Interrupt
WEAK_ALIAS_FUNC(I2C0_IRQHandler, Default_Handler)       // 38: I2C0  Interrupt
WEAK_ALIAS_FUNC(I2C1_IRQHandler, Default_Handler)       // 39: I2C1  Interrupt
WEAK_ALIAS_FUNC(PDMA_IRQHandler, Default_Handler)       // 40: Peripheral DMA Interrupt
WEAK_ALIAS_FUNC(DAC_IRQHandler, Default_Handler)        // 41: DAC Interrupt
WEAK_ALIAS_FUNC(EADC_INT0_IRQHandler, Default_Handler)  // 42: Enhance ADC Interrupt 0
WEAK_ALIAS_FUNC(EADC_INT1_IRQHandler, Default_Handler)  // 43: Enhance ADC Interrupt 1
WEAK_ALIAS_FUNC(ACMP01_IRQHandler, Default_Handler)     // 44: ACMP0 Interrupt
WEAK_ALIAS_FUNC(BPWM0_IRQHandler, Default_Handler)      // 45: BPWM0 Interrupt
WEAK_ALIAS_FUNC(EADC_INT2_IRQHandler, Default_Handler)  // 46: Enhance EADC Interrupt 2
WEAK_ALIAS_FUNC(EADC_INT3_IRQHandler, Default_Handler)  // 47: Enhance EADC Interrupt 3
WEAK_ALIAS_FUNC(UART2_IRQHandler, Default_Handler)      // 48: UART2 Interrupt
                                                        // 49: Reserved  
WEAK_ALIAS_FUNC(USCI0_IRQHandler, Default_Handler)      // 50: USCI0 Interrupt
                                                        // 51: Reserved  
WEAK_ALIAS_FUNC(USCI1_IRQHandler, Default_Handler)      // 52: USCI1 Interrupt
WEAK_ALIAS_FUNC(USBD_IRQHandler, Default_Handler)       // 53: USB Device Interrupt
WEAK_ALIAS_FUNC(BPWM1_IRQHandler, Default_Handler)      // 54: BPWM1 Interrupt
WEAK_ALIAS_FUNC(PSIO_IRQHandler, Default_Handler)       // 55: PSIO Interrupt
                                                        // 56: Reserved  
WEAK_ALIAS_FUNC(CRPT_IRQHandler, Default_Handler)       // 57: Cryption Interrupt
WEAK_ALIAS_FUNC(SC0_IRQHandler, Default_Handler)        // 58: Smart Card0 Interrupt
                                                        // 59: Reserved  
WEAK_ALIAS_FUNC(USCI2_IRQHandler, Default_Handler)      // 60: USCI2 Interrupt
                                                        // 61: Reserved  
WEAK_ALIAS_FUNC(OPA_IRQHandler, Default_Handler)        // 62: OPA Interrupt
                                                        // 63: Reserved  

/* Vector table */
#if defined(__ARMCC_VERSION)
__attribute__ ((section("RESET"), used))
const uint32_t __vector_handlers[] = {
#elif defined(__ICCARM__)
extern uint32_t CSTACK$$Limit;
const uint32_t __vector_table[] @ ".intvec" = {
#elif defined(__GNUC__)
__attribute__ ((section(".vector_table")))
const uint32_t __vector_handlers[] = {
#endif

    /* Configure Initial Stack Pointer, using linker-generated symbols */
#if defined(__ARMCC_VERSION)
    (uint32_t) &Image$$ARM_LIB_STACK$$ZI$$Limit,
#elif defined(__ICCARM__)
    //(uint32_t) __sfe("CSTACK"),
    (uint32_t) &CSTACK$$Limit,
#elif defined(__GNUC__)
    (uint32_t) &__StackTop,
#endif

    (uint32_t) Reset_Handler,           // Reset Handler
    (uint32_t) NMI_Handler,             // NMI Handler
    (uint32_t) HardFault_Handler,       // Hard Fault Handler
    0,                                  // Reserved
    0,                                  // Reserved
    0,                                  // Reserved
    0,                                  // Reserved
    0,                                  // Reserved
    0,                                  // Reserved
    0,                                  // Reserved
    (uint32_t) SVC_Handler,             // SVCall Handler
    0,                                  // Reserved
    0,                                  // Reserved
    (uint32_t) PendSV_Handler,          // PendSV Handler
    (uint32_t) SysTick_Handler,         // SysTick Handler

    /* External Interrupts */
    (uint32_t) BOD_IRQHandler,          // 0: Brown-Out Low Voltage Detected Interrupt
    (uint32_t) IRCTRIM_IRQHandler,      // 1: Watch Dog Timer Interrupt
    (uint32_t) PWRWU_IRQHandler,        // 2: EINT0, EINT2 and EINT4 Interrupt
    (uint32_t) Default_Handler,         // 3: Reserved
    (uint32_t) CLKFAIL_IRQHandler,      // 4: Clock fail dector Interrupt
    (uint32_t) Default_Handler,         // 5: Reserved
    (uint32_t) RTC_IRQHandler,          // 6: Real Time Clock Interrupt
    (uint32_t) TAMPER_IRQHandler,       // 7: Tamper detection Interrupt
    (uint32_t) WDT_IRQHandler,          // 8: Watch Dog Timer Interrupt
    (uint32_t) WWDT_IRQHandler,         // 9: Window Watch Dog Timer Interrupt
    (uint32_t) EINT0_IRQHandler,        // 10: External Input 0 Interrupt
    (uint32_t) EINT1_IRQHandler,        // 11: External Input 1 Interrupt
    (uint32_t) EINT2_IRQHandler,        // 12: External Input 2 Interrupt
    (uint32_t) EINT3_IRQHandler,        // 13: External Input 3 Interrupt
    (uint32_t) EINT4_IRQHandler,        // 14: External Input 4 Interrupt
    (uint32_t) EINT5_IRQHandler,        // 15: External Input 5 Interrupt
    (uint32_t) GPA_IRQHandler,          // 16: GPIO PORT A Interrupt
    (uint32_t) GPB_IRQHandler,          // 17: GPIO PORT B Interrupt
    (uint32_t) GPC_IRQHandler,          // 18: GPIO PORT C Interrupt
    (uint32_t) GPD_IRQHandler,          // 19: GPIO PORT D Interrupt
    (uint32_t) GPE_IRQHandler,          // 20: GPIO PORT E Interrupt
    (uint32_t) GPF_IRQHandler,          // 21: GPIO PORT F Interrupt
    (uint32_t) QSPI0_IRQHandler,        // 22: QSPI0 Interrupt
    (uint32_t) SPI0_IRQHandler,         // 23: SPI0 Interrupt
    (uint32_t) BRAKE0_IRQHandler,       // 24: PWM Brake0 Interrupt
    (uint32_t) PWM0_P0_IRQHandler,      // 25: PWM0 P0 Interrupt
    (uint32_t) PWM0_P1_IRQHandler,      // 26: PWM0 P1 Interrupt
    (uint32_t) PWM0_P2_IRQHandler,      // 27: PWM0 P2 Interrupt
    (uint32_t) BRAKE1_IRQHandler,       // 28: PWM Brake1 Interrupt
    (uint32_t) PWM1_P0_IRQHandler,      // 29: PWM1 P0 Interrupt
    (uint32_t) PWM1_P1_IRQHandler,      // 30: PWM1 P1 Interrupt
    (uint32_t) PWM1_P2_IRQHandler,      // 31: PWM1 P2 Interrupt
    (uint32_t) TMR0_IRQHandler,         // 32: TIMER0  Interrupt
    (uint32_t) TMR1_IRQHandler,         // 33: TIMER1  Interrupt
    (uint32_t) TMR2_IRQHandler,         // 34: TIMER2  Interrupt
    (uint32_t) TMR3_IRQHandler,         // 35: TIMER3  Interrupt
    (uint32_t) UART0_IRQHandler,        // 36: UART0  Interrupt
    (uint32_t) UART1_IRQHandler,        // 37: UART1  Interrupt
    (uint32_t) I2C0_IRQHandler,         // 38: I2C0  Interrupt
    (uint32_t) I2C1_IRQHandler,         // 39: I2C1  Interrupt
    (uint32_t) PDMA_IRQHandler,         // 40: Peripheral DMA Interrupt
    (uint32_t) DAC_IRQHandler,          // 41: DAC Interrupt
    (uint32_t) EADC_INT0_IRQHandler,    // 42: Enhance ADC Interrupt 0
    (uint32_t) EADC_INT1_IRQHandler,    // 43: Enhance ADC Interrupt 1
    (uint32_t) ACMP01_IRQHandler,       // 44: ACMP0 Interrupt
    (uint32_t) BPWM0_IRQHandler,        // 45: BPWM0 Interrupt
    (uint32_t) EADC_INT2_IRQHandler,    // 46: Enhance EADC Interrupt 2
    (uint32_t) EADC_INT3_IRQHandler,    // 47: Enhance EADC Interrupt 3
    (uint32_t) UART2_IRQHandler,        // 48: UART2 Interrupt
    (uint32_t) Default_Handler,         // 49: Reserved
    (uint32_t) USCI0_IRQHandler,        // 50: USCI0 Interrupt
    (uint32_t) Default_Handler,         // 51: Reserved
    (uint32_t) USCI1_IRQHandler,        // 52: USCI1 Interrupt
    (uint32_t) USBD_IRQHandler,         // 53: USB Device Interrupt
    (uint32_t) BPWM1_IRQHandler,        // 54: BPWM1 Interrupt
    (uint32_t) PSIO_IRQHandler,         // 55: PSIO Interrupt
    (uint32_t) Default_Handler,         // 56: Reserved
    (uint32_t) CRPT_IRQHandler,         // 57: Cryption Interrupt
    (uint32_t) SC0_IRQHandler,          // 58: Smart Card0 Interrupt
    (uint32_t) Default_Handler,         // 59: Reserved
    (uint32_t) USCI2_IRQHandler,        // 60: USCI2 Interrupt
    (uint32_t) Default_Handler,         // 61: Reserved
    (uint32_t) OPA_IRQHandler,          // 62: OPA Interrupt
    (uint32_t) Default_Handler,         // 63: Reserved
};

void Reset_Handler(void)
{
    /* Disable register write-protection function */
    SYS_UnlockReg();
    
    /* Disable Power-on Reset function */
    SYS_DISABLE_POR();
    /* After power-on, user can turn off internal analog POR circuit to save
     * power by writing 0x5AA5 to this field. */
    SYS->PORCTL1 = 0x5AA5;
    
    /* SystemInit() must be called at the very start. */
    SystemInit();

    /* Enable register write-protection function */
    SYS_LockReg();
    
#if defined(__ARMCC_VERSION)
    __main();
    
#elif defined(__ICCARM__)
    __iar_program_start();

#elif defined(__GNUC__)
    /* Move (multiple) .data section(s) from ROM to RAM */
    {
        /* Struct of copy table entry which must match linker script */
        typedef struct copy_table_entry_ {
            uint32_t    src;            // Address to copy from
            uint32_t    dst;            // Address to copy to
            uint32_t    size;           // Copy size in bytes
        } copy_table_entry;

        copy_table_entry *copy_table_ind = (copy_table_entry *) &__copy_table_start__;
        copy_table_entry *copy_table_end = (copy_table_entry *) &__copy_table_end__;

        for (; copy_table_ind != copy_table_end; copy_table_ind ++) {
            uint32_t *src_ind = (uint32_t *) copy_table_ind->src;
            uint32_t *src_end = (uint32_t *) (copy_table_ind->src + copy_table_ind->size);
            uint32_t *dst_ind = (uint32_t *) copy_table_ind->dst;
            if (src_ind != dst_ind) {
                for (; src_ind < src_end;) {
                    *dst_ind ++ = *src_ind ++;
                }
            }
        }
    }

    /* Initialize (multiple) .bss sections to zero */
    {
        /* Struct of zero table entry which must match linker script */
        typedef struct zero_table_entry_ {
            uint32_t    start;          // Address to start zero'ing
            uint32_t    size;           // Zero size in bytes
        } zero_table_entry;

        zero_table_entry *zero_table_ind = (zero_table_entry *) &__zero_table_start__;
        zero_table_entry *zero_table_end = (zero_table_entry *) &__zero_table_end__;

        for (; zero_table_ind != zero_table_end; zero_table_ind ++) {
            uint32_t *dst_ind = (uint32_t *) zero_table_ind->start;
            uint32_t *dst_end = (uint32_t *) (zero_table_ind->start + zero_table_ind->size);

            for (; dst_ind < dst_end; ) {
                *dst_ind ++ = 0;
            }
        }
    }

    _start();

#endif

    /* Infinite loop */
    while (1);
}

/* Default interrupt handler for unused IRQs. */
void Default_Handler(void)
{
    while (1);
}
