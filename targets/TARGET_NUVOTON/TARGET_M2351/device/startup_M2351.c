/*
 * Copyright (c) 2018-2019, Nuvoton Technology Corporation
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

#include "M2351.h"

/* Suppress warning messages */
#if defined(__ARMCC_VERSION)
// Suppress warning message: extended constant initializer used
#pragma diag_suppress 1296
#elif defined(__ICCARM__)
// Suppress warning message Pe1665
#pragma diag_suppress=Pe1665
#elif defined(__GNUC__)
#endif

/* Macro Definitions */
#if defined(__ARMCC_VERSION)
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
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) && (TFM_LVL > 0)
extern uint32_t Image$$ARM_LIB_STACK_MSP$$ZI$$Limit;
extern uint32_t Image$$ARM_LIB_STACK$$ZI$$Limit;
#else
extern uint32_t Image$$ARM_LIB_STACK$$ZI$$Limit;
#endif
extern void __main(void);
#elif defined(__ICCARM__)
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) && (TFM_LVL > 0)
extern uint32_t Image$$ARM_LIB_STACK_MSP$$ZI$$Limit;
extern uint32_t Image$$ARM_LIB_STACK$$ZI$$Limit;
extern uint32_t CSTACK_MSP$$Limit;
extern uint32_t CSTACK$$Limit;
#else
extern uint32_t Image$$ARM_LIB_STACK$$ZI$$Limit;
extern uint32_t CSTACK$$Limit;
#endif
void __iar_program_start(void);
#elif defined(__GNUC__)
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) && (TFM_LVL > 0)
extern uint32_t Image$$ARM_LIB_STACK_MSP$$ZI$$Limit;
extern uint32_t Image$$ARM_LIB_STACK$$ZI$$Limit;
#else
extern uint32_t Image$$ARM_LIB_STACK$$ZI$$Limit;
#endif
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

/* SCU handler */
void SCU_IRQHandler(void);

/* Default empty handler */
void Default_Handler(void);

/* Reset handler */
void Reset_Handler(void);

/* Cortex-M0+ core handlers */
WEAK_ALIAS_FUNC(NMI_Handler, Default_Handler)
WEAK_ALIAS_FUNC(HardFault_Handler, Default_Handler)
WEAK_ALIAS_FUNC(SVC_Handler, Default_Handler)

WEAK_ALIAS_FUNC(PendSV_Handler, Default_Handler)
WEAK_ALIAS_FUNC(SysTick_Handler, Default_Handler)

/* Peripherals handlers */
WEAK_ALIAS_FUNC(BOD_IRQHandler, Default_Handler)        // 0: Brown Out detection
WEAK_ALIAS_FUNC(IRC_IRQHandler, Default_Handler)        // 1: Internal RC
WEAK_ALIAS_FUNC(PWRWU_IRQHandler, Default_Handler)      // 2: Power down wake up 
WEAK_ALIAS_FUNC(SRAM_IRQHandler, Default_Handler)       // 3: SRAM
WEAK_ALIAS_FUNC(CLKFAIL_IRQHandler, Default_Handler)    // 4: Clock detection fail
                                                        // 5: Reserved
WEAK_ALIAS_FUNC(RTC_IRQHandler, Default_Handler)        // 6: Real Time Clock 
WEAK_ALIAS_FUNC(TAMPER_IRQHandler, Default_Handler)     // 7: Tamper detection
WEAK_ALIAS_FUNC(WDT_IRQHandler, Default_Handler)        // 8: Watchdog timer
WEAK_ALIAS_FUNC(WWDT_IRQHandler, Default_Handler)       // 9: Window watchdog timer
WEAK_ALIAS_FUNC(EINT0_IRQHandler, Default_Handler)      // 10: External Input 0
WEAK_ALIAS_FUNC(EINT1_IRQHandler, Default_Handler)      // 11: External Input 1
WEAK_ALIAS_FUNC(EINT2_IRQHandler, Default_Handler)      // 12: External Input 2
WEAK_ALIAS_FUNC(EINT3_IRQHandler, Default_Handler)      // 13: External Input 3
WEAK_ALIAS_FUNC(EINT4_IRQHandler, Default_Handler)      // 14: External Input 4
WEAK_ALIAS_FUNC(EINT5_IRQHandler, Default_Handler)      // 15: External Input 5
WEAK_ALIAS_FUNC(GPA_IRQHandler, Default_Handler)        // 16: GPIO Port A
WEAK_ALIAS_FUNC(GPB_IRQHandler, Default_Handler)        // 17: GPIO Port B
WEAK_ALIAS_FUNC(GPC_IRQHandler, Default_Handler)        // 18: GPIO Port C
WEAK_ALIAS_FUNC(GPD_IRQHandler, Default_Handler)        // 19: GPIO Port D
WEAK_ALIAS_FUNC(GPE_IRQHandler, Default_Handler)        // 20: GPIO Port E
WEAK_ALIAS_FUNC(GPF_IRQHandler, Default_Handler)        // 21: GPIO Port F
WEAK_ALIAS_FUNC(QSPI0_IRQHandler, Default_Handler)      // 22: QSPI0
WEAK_ALIAS_FUNC(SPI0_IRQHandler, Default_Handler)       // 23: SPI0
WEAK_ALIAS_FUNC(BRAKE0_IRQHandler, Default_Handler)     // 24: BRAKE0
WEAK_ALIAS_FUNC(EPWM0_P0_IRQHandler, Default_Handler)   // 25: EPWM0P0
WEAK_ALIAS_FUNC(EPWM0_P1_IRQHandler, Default_Handler)   // 26: EPWM0P1
WEAK_ALIAS_FUNC(EPWM0_P2_IRQHandler, Default_Handler)   // 27: EPWM0P2
WEAK_ALIAS_FUNC(BRAKE1_IRQHandler, Default_Handler)     // 28: BRAKE1
WEAK_ALIAS_FUNC(EPWM1_P0_IRQHandler, Default_Handler)   // 29: EPWM1P0
WEAK_ALIAS_FUNC(EPWM1_P1_IRQHandler, Default_Handler)   // 30: EPWM1P1
WEAK_ALIAS_FUNC(EPWM1_P2_IRQHandler, Default_Handler)   // 31: EPWM1P2
WEAK_ALIAS_FUNC(TMR0_IRQHandler, Default_Handler)       // 32: Timer 0
WEAK_ALIAS_FUNC(TMR1_IRQHandler, Default_Handler)       // 33: Timer 1
WEAK_ALIAS_FUNC(TMR2_IRQHandler, Default_Handler)       // 34: Timer 2
WEAK_ALIAS_FUNC(TMR3_IRQHandler, Default_Handler)       // 35: Timer 3
WEAK_ALIAS_FUNC(UART0_IRQHandler, Default_Handler)      // 36: UART0
WEAK_ALIAS_FUNC(UART1_IRQHandler, Default_Handler)      // 37: UART1
WEAK_ALIAS_FUNC(I2C0_IRQHandler, Default_Handler)       // 38: I2C0
WEAK_ALIAS_FUNC(I2C1_IRQHandler, Default_Handler)       // 39: I2C1
WEAK_ALIAS_FUNC(PDMA0_IRQHandler, Default_Handler)      // 40: Peripheral DMA 0
WEAK_ALIAS_FUNC(DAC_IRQHandler, Default_Handler)        // 41: DAC
WEAK_ALIAS_FUNC(EADC0_IRQHandler, Default_Handler)      // 42: EADC Source 0
WEAK_ALIAS_FUNC(EADC1_IRQHandler, Default_Handler)      // 43: EADC Source 1
WEAK_ALIAS_FUNC(ACMP01_IRQHandler, Default_Handler)     // 44: ACMP0 and ACMP1
                                                        // 45: Reserved
WEAK_ALIAS_FUNC(EADC2_IRQHandler, Default_Handler)      // 46: EADC Source 2
WEAK_ALIAS_FUNC(EADC3_IRQHandler, Default_Handler)      // 47: EADC Source 3
WEAK_ALIAS_FUNC(UART2_IRQHandler, Default_Handler)      // 48: UART2
WEAK_ALIAS_FUNC(UART3_IRQHandler, Default_Handler)      // 49: UART3
                                                        // 50: Reserved
WEAK_ALIAS_FUNC(SPI1_IRQHandler, Default_Handler)       // 51: SPI1
WEAK_ALIAS_FUNC(SPI2_IRQHandler, Default_Handler)       // 52: SPI2
WEAK_ALIAS_FUNC(USBD_IRQHandler, Default_Handler)       // 53: USB device
WEAK_ALIAS_FUNC(USBH_IRQHandler, Default_Handler)       // 54: USB host
WEAK_ALIAS_FUNC(USBOTG_IRQHandler, Default_Handler)     // 55: USB OTG
WEAK_ALIAS_FUNC(CAN0_IRQHandler, Default_Handler)       // 56: CAN0
                                                        // 57: Reserved
WEAK_ALIAS_FUNC(SC0_IRQHandler, Default_Handler)        // 58: 
WEAK_ALIAS_FUNC(SC1_IRQHandler, Default_Handler)        // 59: 
WEAK_ALIAS_FUNC(SC2_IRQHandler, Default_Handler)        // 60: 
                                                        // 61: 
WEAK_ALIAS_FUNC(SPI3_IRQHandler, Default_Handler)       // 62:
                                                        // 63: Reserved
WEAK_ALIAS_FUNC(SDH0_IRQHandler, Default_Handler)       // 64:                                                        
                                                        // 65 ~67: Reserved                                                        
WEAK_ALIAS_FUNC(I2S0_IRQHandler, Default_Handler)       // 68:
                                                        // 69: Reserved
WEAK_ALIAS_FUNC(OPA0_IRQHandler, Default_Handler)       // 70:
WEAK_ALIAS_FUNC(CRPT_IRQHandler, Default_Handler)       // 71:
WEAK_ALIAS_FUNC(GPG_IRQHandler, Default_Handler)        // 72:
WEAK_ALIAS_FUNC(EINT6_IRQHandler, Default_Handler)      // 73:
WEAK_ALIAS_FUNC(UART4_IRQHandler, Default_Handler)      // 74:
WEAK_ALIAS_FUNC(UART5_IRQHandler, Default_Handler)      // 75:
WEAK_ALIAS_FUNC(USCI0_IRQHandler, Default_Handler)      // 76:
WEAK_ALIAS_FUNC(USCI1_IRQHandler, Default_Handler)      // 77:
WEAK_ALIAS_FUNC(BPWM0_IRQHandler, Default_Handler)      // 78:
WEAK_ALIAS_FUNC(BPWM1_IRQHandler, Default_Handler)      // 79:
                                                        // 80~81: Reserved
WEAK_ALIAS_FUNC(I2C2_IRQHandler, Default_Handler)       // 82:                                                        
                                                        // 83: Reserved
WEAK_ALIAS_FUNC(QEI0_IRQHandler, Default_Handler)       // 84:
WEAK_ALIAS_FUNC(QEI1_IRQHandler, Default_Handler)       // 85:
WEAK_ALIAS_FUNC(ECAP0_IRQHandler, Default_Handler)      // 86:
WEAK_ALIAS_FUNC(ECAP1_IRQHandler, Default_Handler)      // 87:
WEAK_ALIAS_FUNC(GPH_IRQHandler, Default_Handler)        // 88:
WEAK_ALIAS_FUNC(EINT7_IRQHandler, Default_Handler)      // 89:
                                                        // 90~97: Reserved
WEAK_ALIAS_FUNC(PDMA1_IRQHandler, Default_Handler)      // 98: Peripheral DMA 1
                                                        // 99:                            
                                                        // 100:  Reserved
WEAK_ALIAS_FUNC(TRNG_IRQHandler, Default_Handler)       // 101: 


/* Vector table */
#if defined(__ARMCC_VERSION)
__attribute__ ((section("RESET"), used))
const uint32_t __vector_handlers[] = {
#elif defined(__ICCARM__)
const uint32_t __vector_table[] @ ".intvec" = {
#elif defined(__GNUC__)
__attribute__ ((section(".vector_table")))
const uint32_t __vector_handlers[] = {
#endif

    /* Configure Initial Stack Pointer, using linker-generated symbols */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) && (TFM_LVL > 0)
#if defined(__ARMCC_VERSION)
    (uint32_t) &Image$$ARM_LIB_STACK_MSP$$ZI$$Limit,
#elif defined(__ICCARM__)
    (uint32_t) &CSTACK_MSP$$Limit,
#elif defined(__GNUC__)
    (uint32_t) &__StackTop,
#endif
#else
#if defined(__ARMCC_VERSION)
    (uint32_t) &Image$$ARM_LIB_STACK$$ZI$$Limit,
#elif defined(__ICCARM__)
    (uint32_t) &CSTACK$$Limit,
#elif defined(__GNUC__)
    (uint32_t) &__StackTop,
#endif
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
    (uint32_t) BOD_IRQHandler,          // 0: Brown Out detection
    (uint32_t) IRC_IRQHandler,          // 1: Internal RC
    (uint32_t) PWRWU_IRQHandler,        // 2: Power down wake up 
    (uint32_t) SRAM_IRQHandler,         // 3: 
    (uint32_t) CLKFAIL_IRQHandler,      // 4: Clock detection fail
    (uint32_t) Default_Handler,         // 5: Reserved
    (uint32_t) RTC_IRQHandler,          // 6: Real Time Clock 
    (uint32_t) TAMPER_IRQHandler,       // 7: Tamper detection
    (uint32_t) WDT_IRQHandler,          // 8: Watchdog timer
    (uint32_t) WWDT_IRQHandler,         // 9: Window watchdog timer
    (uint32_t) EINT0_IRQHandler,        // 10: External Input 0
    (uint32_t) EINT1_IRQHandler,        // 11: External Input 1
    (uint32_t) EINT2_IRQHandler,        // 12: External Input 2
    (uint32_t) EINT3_IRQHandler,        // 13: External Input 3
    (uint32_t) EINT4_IRQHandler,        // 14: External Input 4
    (uint32_t) EINT5_IRQHandler,        // 15: External Input 5
    (uint32_t) GPA_IRQHandler,          // 16: GPIO Port A
    (uint32_t) GPB_IRQHandler,          // 17: GPIO Port B
    (uint32_t) GPC_IRQHandler,          // 18: GPIO Port C
    (uint32_t) GPD_IRQHandler,          // 19: GPIO Port D
    (uint32_t) GPE_IRQHandler,          // 20: GPIO Port E
    (uint32_t) GPF_IRQHandler,          // 21: GPIO Port F
    (uint32_t) QSPI0_IRQHandler,        // 22: QSPI0
    (uint32_t) SPI0_IRQHandler,         // 23: SPI0
    (uint32_t) BRAKE0_IRQHandler,       // 24: 
    (uint32_t) EPWM0_P0_IRQHandler,     // 25: 
    (uint32_t) EPWM0_P1_IRQHandler,     // 26: 
    (uint32_t) EPWM0_P2_IRQHandler,     // 27: 
    (uint32_t) BRAKE1_IRQHandler,       // 28: 
    (uint32_t) EPWM1_P0_IRQHandler,     // 29: 
    (uint32_t) EPWM1_P1_IRQHandler,     // 30: 
    (uint32_t) EPWM1_P2_IRQHandler,     // 31: 
    (uint32_t) TMR0_IRQHandler,         // 32: Timer 0
    (uint32_t) TMR1_IRQHandler,         // 33: Timer 1
    (uint32_t) TMR2_IRQHandler,         // 34: Timer 2
    (uint32_t) TMR3_IRQHandler,         // 35: Timer 3
    (uint32_t) UART0_IRQHandler,        // 36: UART0
    (uint32_t) UART1_IRQHandler,        // 37: UART1
    (uint32_t) I2C0_IRQHandler,         // 38: I2C0
    (uint32_t) I2C1_IRQHandler,         // 39: I2C1
    (uint32_t) PDMA0_IRQHandler,        // 40: Peripheral DMA 0
    (uint32_t) DAC_IRQHandler,          // 41: DAC
    (uint32_t) EADC0_IRQHandler,        // 42: EADC source 0
    (uint32_t) EADC1_IRQHandler,        // 43: EADC source 1
    (uint32_t) ACMP01_IRQHandler,       // 44: ACMP0 and ACMP1
    (uint32_t) Default_Handler,         // 45: Reserved
    (uint32_t) EADC2_IRQHandler,        // 46: EADC source 2
    (uint32_t) EADC3_IRQHandler,        // 47: EADC source 3
    (uint32_t) UART2_IRQHandler,        // 48: UART2
    (uint32_t) UART3_IRQHandler,        // 49: UART3
    (uint32_t) Default_Handler,         // 50: Reserved
    (uint32_t) SPI1_IRQHandler,         // 51: SPI1
    (uint32_t) SPI2_IRQHandler,         // 52: SPI2
    (uint32_t) USBD_IRQHandler,         // 53: USB device
    (uint32_t) USBH_IRQHandler,         // 54: USB host
    (uint32_t) USBOTG_IRQHandler,       // 55: USB OTG
    (uint32_t) CAN0_IRQHandler,         // 56: CAN0
    (uint32_t) Default_Handler,         // 57: Reserved
    (uint32_t) SC0_IRQHandler,          // 58: 
    (uint32_t) SC1_IRQHandler,          // 59: 
    (uint32_t) SC2_IRQHandler,          // 60: 
    (uint32_t) Default_Handler,         // 61: Reserved.
    (uint32_t) SPI3_IRQHandler,         // 62: 
    (uint32_t) Default_Handler,         // 63: 
    (uint32_t) SDH0_IRQHandler,         // 64:
    (uint32_t) Default_Handler,         // 65:
    (uint32_t) Default_Handler,         // 66:    
    (uint32_t) Default_Handler,         // 67:
    (uint32_t) I2S0_IRQHandler,         // 68:
    (uint32_t) Default_Handler,         // 69:
    (uint32_t) OPA0_IRQHandler,         // 70:
    (uint32_t) CRPT_IRQHandler,         // 71:
    (uint32_t) GPG_IRQHandler,          // 72:
    (uint32_t) EINT6_IRQHandler,        // 73:
    (uint32_t) UART4_IRQHandler,        // 74:
    (uint32_t) UART5_IRQHandler,        // 75:
    (uint32_t) USCI0_IRQHandler,        // 76:
    (uint32_t) USCI1_IRQHandler,        // 77:
    (uint32_t) BPWM0_IRQHandler,        // 78:
    (uint32_t) BPWM1_IRQHandler,        // 79:
    (uint32_t) Default_Handler,         // 80:
    (uint32_t) Default_Handler,         // 81:
    (uint32_t) I2C2_IRQHandler,         // 82:
    (uint32_t) Default_Handler,         // 83:
    (uint32_t) QEI0_IRQHandler,         // 84:    
    (uint32_t) QEI1_IRQHandler,         // 85:
    (uint32_t) ECAP0_IRQHandler,        // 86:    
    (uint32_t) ECAP1_IRQHandler,        // 87:
    (uint32_t) GPH_IRQHandler,          // 88:    
    (uint32_t) EINT7_IRQHandler,        // 89:
    (uint32_t) Default_Handler,         // 90:    
    (uint32_t) Default_Handler,         // 91:
    (uint32_t) Default_Handler,         // 92:    
    (uint32_t) Default_Handler,         // 93:
    (uint32_t) Default_Handler,         // 94:    
    (uint32_t) Default_Handler,         // 95:
    (uint32_t) Default_Handler,         // 96:    
    (uint32_t) Default_Handler,         // 97:
    (uint32_t) PDMA1_IRQHandler,        // 98: Peripheral DMA 1
    (uint32_t) SCU_IRQHandler,          // 99:
    (uint32_t) Default_Handler,         // 100:    
    (uint32_t) TRNG_IRQHandler,         // 101:
};

/* Some reset handler code cannot implement in pure C. Implement it in inline/embedded assembly.
 *
 * Reset_Handler:
 *   For non-secure PSA/non-secure non-PSA/secure non-PSA, jump directly to Reset_Handler_1
 *   For secure PSA, switch from MSP to PSP, then jump to Reset_Handler_1
 * 
 * Reset_Handler_1:
 *   Platform initialization
 *   C/C++ runtime initialization
 */

/* Forward declaration */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) && (TFM_LVL > 0)
/* Limited by inline assembly syntax, esp. on IAR, we cannot get stack limit
 * for PSP just from external symbol. To avoid re-write in assembly, We make up
 * a function here to get this value indirectly. */
uint32_t StackLimit_PSP(void);
#endif
void Reset_Handler_1(void);

/* Add '__attribute__((naked))' here to make sure compiler does not generate prologue and
 * epilogue sequences for Reset_Handler. We don't want MSP is updated by compiler-generated
 * code during stack switch. 
 *
 * Don't allow extended assembly in naked functions:
 * The compiler only supports basic __asm statements in __attribute__((naked))
 * functions. Using extended assembly, parameter references or mixing C code with
 * __asm statements might not work reliably. 
 */
__attribute__((naked)) void Reset_Handler(void)
{
#if defined(__GNUC__)
    __asm(".syntax  unified                                         \n");
#endif

    /* Secure TFM requires PSP as boot stack */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) && (TFM_LVL > 0)
    /* Get stack limit for PSP */
#if !defined(__ICCARM__)
    __asm("movw     r0, #:lower16:StackLimit_PSP                    \n");
    __asm("movt     r0, #:upper16:StackLimit_PSP                    \n");
#else
    __asm("mov32    r0, StackLimit_PSP                              \n");
#endif
    __asm("blx      r0                                              \n");

    /* Switch from MSP to PSP */
    __asm("msr      psp, r0                                         \n");
    __asm("mrs      r0, control                                     \n");
    __asm("movs     r1, #2                                          \n");
    __asm("orrs     r0, r1                                          \n");
    __asm("msr      control, r0                                     \n");
#endif

    /* Jump to Reset_Handler_1 */
#if !defined(__ICCARM__)
    __asm("movw     r0, #:lower16:Reset_Handler_1                   \n");
    __asm("movt     r0, #:upper16:Reset_Handler_1                   \n");
#else
    __asm("mov32    r0, Reset_Handler_1                             \n");
#endif
    __asm("bx       r0                                              \n");
}

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) && (TFM_LVL > 0)
/* Return stack limit for PSP */
uint32_t StackLimit_PSP(void)
{
    uint32_t stacklimit_psp;

    __asm volatile (
#if defined(__GNUC__)
        ".syntax  unified                                           \n"
#endif
        "mov    %[Rd], %[Rn]                                        \n"
        : [Rd] "=r" (stacklimit_psp)                                    /* comma-separated list of output operands */
        : [Rn] "r" (&Image$$ARM_LIB_STACK$$ZI$$Limit)                   /* comma-separated list of input operands */
        : "cc"                                                          /* comma-separated list of clobbered resources */
    );

    return stacklimit_psp;
}
#endif

void Reset_Handler_1(void)
{
#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    /* Disable register write-protection function */
    SYS_UnlockReg();
    
    /* Disable Power-on Reset function */
    SYS_DISABLE_POR();
    
    /* Enable register write-protection function */
    SYS_LockReg();
#endif

    /* SystemInit() must be called at the very start. */
    SystemInit();
    
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

/**
 * \brief Default interrupt handler for unused IRQs.
 */
void Default_Handler(void)
{
    while (1);
}
