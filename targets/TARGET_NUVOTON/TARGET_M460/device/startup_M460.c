/*
 * Copyright (c) 2022, Nuvoton Technology Corporation
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

#if defined(__GNUC__)

/* Get around error: conflicting type qualifiers for '__copy_table_start__'
 *
 * cmsis_gcc.h also imports the following symbols but with different type qualifier:
 *
 * __copy_table_start__
 * __copy_table_end__
 * __zero_table_start__
 * __zero_table_end__
 *
 * Define `__PROGRAM_START` to exclude __cmsis_start() in cmsis_gcc.h.
 */
#define __PROGRAM_START

#endif

#include "M460.h"

/* Suppress warning messages */
#if defined(__ARMCC_VERSION)
#elif defined(__ICCARM__)
// Suppress warning message Pe1665
#pragma diag_suppress=Pe1665
#elif defined(__GNUC__)
#endif

/* Macro definitions */
#if defined(__ARMCC_VERSION)
#define WEAK            __attribute__ ((weak))
#define ALIAS(f)        __attribute__ ((weak, alias(#f)))
#define USED            __attribute__ ((used))

#define WEAK_ALIAS_FUNC(FUN, FUN_ALIAS) \
void FUN(void) __attribute__ ((weak, alias(#FUN_ALIAS)));

#elif defined(__ICCARM__)
#define USED            __root
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
#define USED            __attribute__ ((used))

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
void Reset_Handler_1(void);
void Reset_Handler_2(void);
USED void Reset_Handler_Cascade(void *sp, void *pc);

/* Cortex-M4 core handlers */
WEAK_ALIAS_FUNC(NMI_Handler, Default_Handler)
WEAK_ALIAS_FUNC(HardFault_Handler, Default_Handler)
WEAK_ALIAS_FUNC(MemManage_Handler, Default_Handler)
WEAK_ALIAS_FUNC(BusFault_Handler , Default_Handler)
WEAK_ALIAS_FUNC(UsageFault_Handler, Default_Handler)
WEAK_ALIAS_FUNC(SVC_Handler, Default_Handler)
WEAK_ALIAS_FUNC(DebugMon_Handler, Default_Handler)
WEAK_ALIAS_FUNC(PendSV_Handler, Default_Handler)
WEAK_ALIAS_FUNC(SysTick_Handler, Default_Handler)

/* Peripherals handlers */
WEAK_ALIAS_FUNC(BOD_IRQHandler,     Default_Handler)    // 0: Brown Out detection
WEAK_ALIAS_FUNC(IRC_IRQHandler,     Default_Handler)    // 1: Internal RC
WEAK_ALIAS_FUNC(PWRWU_IRQHandler,   Default_Handler)    // 2: Power down wake up
WEAK_ALIAS_FUNC(RAMPE_IRQHandler,   Default_Handler)    // 3: RAM parity error
WEAK_ALIAS_FUNC(CKFAIL_IRQHandler,  Default_Handler)    // 4: Clock detection fail
WEAK_ALIAS_FUNC(ISP_IRQHandler,     Default_Handler)    // 5: ISP
WEAK_ALIAS_FUNC(RTC_IRQHandler,     Default_Handler)    // 6: Real Time Clock
WEAK_ALIAS_FUNC(TAMPER_IRQHandler,  Default_Handler)    // 7: Tamper detection
WEAK_ALIAS_FUNC(WDT_IRQHandler,     Default_Handler)    // 8: Watchdog timer
WEAK_ALIAS_FUNC(WWDT_IRQHandler,    Default_Handler)    // 9: Window watchdog timer
WEAK_ALIAS_FUNC(EINT0_IRQHandler,   Default_Handler)    // 10: External Input 0
WEAK_ALIAS_FUNC(EINT1_IRQHandler,   Default_Handler)    // 11: External Input 1
WEAK_ALIAS_FUNC(EINT2_IRQHandler,   Default_Handler)    // 12: External Input 2
WEAK_ALIAS_FUNC(EINT3_IRQHandler,   Default_Handler)    // 13: External Input 3
WEAK_ALIAS_FUNC(EINT4_IRQHandler,   Default_Handler)    // 14: External Input 4
WEAK_ALIAS_FUNC(EINT5_IRQHandler,   Default_Handler)    // 15: External Input 5
WEAK_ALIAS_FUNC(GPA_IRQHandler,     Default_Handler)    // 16: GPIO Port A
WEAK_ALIAS_FUNC(GPB_IRQHandler,     Default_Handler)    // 17: GPIO Port B
WEAK_ALIAS_FUNC(GPC_IRQHandler,     Default_Handler)    // 18: GPIO Port C
WEAK_ALIAS_FUNC(GPD_IRQHandler,     Default_Handler)    // 19: GPIO Port D
WEAK_ALIAS_FUNC(GPE_IRQHandler,     Default_Handler)    // 20: GPIO Port E
WEAK_ALIAS_FUNC(GPF_IRQHandler,     Default_Handler)    // 21: GPIO Port F
WEAK_ALIAS_FUNC(QSPI0_IRQHandler,   Default_Handler)    // 22: QSPI0
WEAK_ALIAS_FUNC(SPI0_IRQHandler,    Default_Handler)    // 23: SPI0
WEAK_ALIAS_FUNC(BRAKE0_IRQHandler,  Default_Handler)    // 24: EPWM0 brake
WEAK_ALIAS_FUNC(EPWM0P0_IRQHandler, Default_Handler)    // 25: EPWM0 pair 0
WEAK_ALIAS_FUNC(EPWM0P1_IRQHandler, Default_Handler)    // 26: EPWM0 pair 1
WEAK_ALIAS_FUNC(EPWM0P2_IRQHandler, Default_Handler)    // 27: EPWM0 pair 2
WEAK_ALIAS_FUNC(BRAKE1_IRQHandler,  Default_Handler)    // 28: EPWM1 brake
WEAK_ALIAS_FUNC(EPWM1P0_IRQHandler, Default_Handler)    // 29: EPWM1 pair 0
WEAK_ALIAS_FUNC(EPWM1P1_IRQHandler, Default_Handler)    // 30: EPWM1 pair 1
WEAK_ALIAS_FUNC(EPWM1P2_IRQHandler, Default_Handler)    // 31: EPWM1 pair 2
WEAK_ALIAS_FUNC(TMR0_IRQHandler,    Default_Handler)    // 32: Timer 0
WEAK_ALIAS_FUNC(TMR1_IRQHandler,    Default_Handler)    // 33: Timer 1
WEAK_ALIAS_FUNC(TMR2_IRQHandler,    Default_Handler)    // 34: Timer 2
WEAK_ALIAS_FUNC(TMR3_IRQHandler,    Default_Handler)    // 35: Timer 3
WEAK_ALIAS_FUNC(UART0_IRQHandler,   Default_Handler)    // 36: UART0
WEAK_ALIAS_FUNC(UART1_IRQHandler,   Default_Handler)    // 37: UART1
WEAK_ALIAS_FUNC(I2C0_IRQHandler,    Default_Handler)    // 38: I2C0
WEAK_ALIAS_FUNC(I2C1_IRQHandler,    Default_Handler)    // 39: I2C1
WEAK_ALIAS_FUNC(PDMA0_IRQHandler,   Default_Handler)    // 40: Peripheral DMA 0
WEAK_ALIAS_FUNC(DAC_IRQHandler,     Default_Handler)    // 41: DAC
WEAK_ALIAS_FUNC(EADC00_IRQHandler,  Default_Handler)    // 42: EADC0 interrupt source 0
WEAK_ALIAS_FUNC(EADC01_IRQHandler,  Default_Handler)    // 43: EADC0 interrupt source 1
WEAK_ALIAS_FUNC(ACMP01_IRQHandler,  Default_Handler)    // 44: ACMP0 and ACMP1
WEAK_ALIAS_FUNC(ACMP23_IRQHandler,  Default_Handler)    // 45: ACMP2 and ACMP3
WEAK_ALIAS_FUNC(EADC02_IRQHandler,  Default_Handler)    // 46: EADC0 interrupt source 2
WEAK_ALIAS_FUNC(EADC03_IRQHandler,  Default_Handler)    // 47: EADC0 interrupt source 3
WEAK_ALIAS_FUNC(UART2_IRQHandler,   Default_Handler)    // 48: UART2
WEAK_ALIAS_FUNC(UART3_IRQHandler,   Default_Handler)    // 49: UART3
WEAK_ALIAS_FUNC(QSPI1_IRQHandler,   Default_Handler)    // 50: QSPI1
WEAK_ALIAS_FUNC(SPI1_IRQHandler,    Default_Handler)    // 51: SPI1
WEAK_ALIAS_FUNC(SPI2_IRQHandler,    Default_Handler)    // 52: SPI2
WEAK_ALIAS_FUNC(USBD_IRQHandler,    Default_Handler)    // 53: USB device
WEAK_ALIAS_FUNC(OHCI_IRQHandler,    Default_Handler)    // 54: OHCI
WEAK_ALIAS_FUNC(USBOTG_IRQHandler,  Default_Handler)    // 55: USB OTG
WEAK_ALIAS_FUNC(BMC_IRQHandler,     Default_Handler)    // 56: BMC
WEAK_ALIAS_FUNC(SPI5_IRQHandler,    Default_Handler)    // 57: SPI5
WEAK_ALIAS_FUNC(SC0_IRQHandler,     Default_Handler)    // 58: SC0
WEAK_ALIAS_FUNC(SC1_IRQHandler,     Default_Handler)    // 59: SC1
WEAK_ALIAS_FUNC(SC2_IRQHandler,     Default_Handler)    // 60: SC2
WEAK_ALIAS_FUNC(GPJ_IRQHandler,     Default_Handler)    // 61: GPIO Port J
WEAK_ALIAS_FUNC(SPI3_IRQHandler,    Default_Handler)    // 62: SPI3
WEAK_ALIAS_FUNC(SPI4_IRQHandler,    Default_Handler)    // 63: SPI4
WEAK_ALIAS_FUNC(SDH0_IRQHandler,    Default_Handler)    // 64: SDH0
WEAK_ALIAS_FUNC(USBD20_IRQHandler,  Default_Handler)    // 65: USBD20
WEAK_ALIAS_FUNC(EMAC0_IRQHandler,   Default_Handler)    // 66: EMAC0
                                                        // 67:
WEAK_ALIAS_FUNC(I2S0_IRQHandler,    Default_Handler)    // 68: I2S0
WEAK_ALIAS_FUNC(I2S1_IRQHandler,    Default_Handler)    // 69: I2S1
WEAK_ALIAS_FUNC(SPI6_IRQHandler,    Default_Handler)    // 70: SPI6
WEAK_ALIAS_FUNC(CRPT_IRQHandler,    Default_Handler)    // 71: CRYPTO
WEAK_ALIAS_FUNC(GPG_IRQHandler,     Default_Handler)    // 72: GPIO Port G
WEAK_ALIAS_FUNC(EINT6_IRQHandler,   Default_Handler)    // 73: External Input 6
WEAK_ALIAS_FUNC(UART4_IRQHandler,   Default_Handler)    // 74: UART4
WEAK_ALIAS_FUNC(UART5_IRQHandler,   Default_Handler)    // 75: UART5
WEAK_ALIAS_FUNC(USCI0_IRQHandler,   Default_Handler)    // 76: USCI0
WEAK_ALIAS_FUNC(SPI7_IRQHandler,    Default_Handler)    // 77: SPI7
WEAK_ALIAS_FUNC(BPWM0_IRQHandler,   Default_Handler)    // 78: BPWM0
WEAK_ALIAS_FUNC(BPWM1_IRQHandler,   Default_Handler)    // 79: BPWM1
WEAK_ALIAS_FUNC(SPIM_IRQHandler,    Default_Handler)    // 80: SPIM
WEAK_ALIAS_FUNC(CCAP_IRQHandler,    Default_Handler)    // 81: CCAP
WEAK_ALIAS_FUNC(I2C2_IRQHandler,    Default_Handler)    // 82: I2C2
WEAK_ALIAS_FUNC(I2C3_IRQHandler,    Default_Handler)    // 83: I2C3
WEAK_ALIAS_FUNC(EQEI0_IRQHandler,   Default_Handler)    // 84: EQEI0
WEAK_ALIAS_FUNC(EQEI1_IRQHandler,   Default_Handler)    // 85: EQEI1
WEAK_ALIAS_FUNC(ECAP0_IRQHandler,   Default_Handler)    // 86: ECAP0
WEAK_ALIAS_FUNC(ECAP1_IRQHandler,   Default_Handler)    // 87: ECAP1
WEAK_ALIAS_FUNC(GPH_IRQHandler,     Default_Handler)    // 88: GPIO Port H
WEAK_ALIAS_FUNC(EINT7_IRQHandler,   Default_Handler)    // 89: External Input 7
WEAK_ALIAS_FUNC(SDH1_IRQHandler,    Default_Handler)    // 90: SDH1
WEAK_ALIAS_FUNC(PSIO_IRQHandler,    Default_Handler)    // 91: PSIO
WEAK_ALIAS_FUNC(EHCI_IRQHandler,    Default_Handler)    // 92: EHCI
WEAK_ALIAS_FUNC(USBOTG20_IRQHandler,Default_Handler)    // 93: HSOTG
WEAK_ALIAS_FUNC(ECAP2_IRQHandler,   Default_Handler)    // 94: ECAP2
WEAK_ALIAS_FUNC(ECAP3_IRQHandler,   Default_Handler)    // 95: ECAP3
WEAK_ALIAS_FUNC(KPI_IRQHandler,     Default_Handler)    // 96: KPI
WEAK_ALIAS_FUNC(HBI_IRQHandler,     Default_Handler)    // 97: HBI
WEAK_ALIAS_FUNC(PDMA1_IRQHandler,   Default_Handler)    // 98: Peripheral DMA 1
WEAK_ALIAS_FUNC(UART8_IRQHandler,   Default_Handler)    // 99: UART8
WEAK_ALIAS_FUNC(UART9_IRQHandler,   Default_Handler)    // 100: UART9
WEAK_ALIAS_FUNC(TRNG_IRQHandler,    Default_Handler)    // 101: TRNG
WEAK_ALIAS_FUNC(UART6_IRQHandler,   Default_Handler)    // 102: UART6
WEAK_ALIAS_FUNC(UART7_IRQHandler,   Default_Handler)    // 103: UART7
WEAK_ALIAS_FUNC(EADC10_IRQHandler,  Default_Handler)    // 104: EADC1 interrupt source 0
WEAK_ALIAS_FUNC(EADC11_IRQHandler,  Default_Handler)    // 105: EADC1 interrupt source 1
WEAK_ALIAS_FUNC(EADC12_IRQHandler,  Default_Handler)    // 106: EADC1 interrupt source 2
WEAK_ALIAS_FUNC(EADC13_IRQHandler,  Default_Handler)    // 107: EADC1 interrupt source 3
WEAK_ALIAS_FUNC(SPI8_IRQHandler,    Default_Handler)    // 108: SPI8
WEAK_ALIAS_FUNC(KS_IRQHandler,      Default_Handler)    // 109: Key Store
WEAK_ALIAS_FUNC(GPI_IRQHandler,     Default_Handler)    // 110: GPIO Port I
WEAK_ALIAS_FUNC(SPI9_IRQHandler,    Default_Handler)    // 111: SPI9
WEAK_ALIAS_FUNC(CANFD00_IRQHandler, Default_Handler)    // 112: CANFD0 interrupt source 0
WEAK_ALIAS_FUNC(CANFD01_IRQHandler, Default_Handler)    // 113: CANFD0 interrupt source 1
WEAK_ALIAS_FUNC(CANFD10_IRQHandler, Default_Handler)    // 114: CANFD1 interrupt source 0
WEAK_ALIAS_FUNC(CANFD11_IRQHandler, Default_Handler)    // 115: CANFD1 interrupt source 1
WEAK_ALIAS_FUNC(EQEI2_IRQHandler,   Default_Handler)    // 116: EQEI2
WEAK_ALIAS_FUNC(EQEI3_IRQHandler,   Default_Handler)    // 117: EQEI3
WEAK_ALIAS_FUNC(I2C4_IRQHandler,    Default_Handler)    // 118: I2C4
WEAK_ALIAS_FUNC(SPI10_IRQHandler,   Default_Handler)    // 119: SPI10
WEAK_ALIAS_FUNC(CANFD20_IRQHandler, Default_Handler)    // 120: CANFD2 interrupt source 0
WEAK_ALIAS_FUNC(CANFD21_IRQHandler, Default_Handler)    // 121: CANFD2 interrupt source 1
WEAK_ALIAS_FUNC(CANFD30_IRQHandler, Default_Handler)    // 122: CANFD3 interrupt source 0
WEAK_ALIAS_FUNC(CANFD31_IRQHandler, Default_Handler)    // 123: CANFD3 interrupt source 1
WEAK_ALIAS_FUNC(EADC20_IRQHandler,  Default_Handler)    // 124: EADC2 interrupt source 0
WEAK_ALIAS_FUNC(EADC21_IRQHandler,  Default_Handler)    // 125: EADC2 interrupt source 1
WEAK_ALIAS_FUNC(EADC22_IRQHandler,  Default_Handler)    // 126: EADC2 interrupt source 2
WEAK_ALIAS_FUNC(EADC23_IRQHandler,  Default_Handler)    // 127: EADC2 interrupt source 3


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
    (uint32_t) &CSTACK$$Limit,
#elif defined(__GNUC__)
    (uint32_t) &__StackTop,
#endif

    (uint32_t) Reset_Handler,           // Reset Handler
    (uint32_t) NMI_Handler,             // NMI Handler
    (uint32_t) HardFault_Handler,       // Hard Fault Handler
    (uint32_t) MemManage_Handler,       // MPU Fault Handler
    (uint32_t) BusFault_Handler,        // Bus Fault Handler
    (uint32_t) UsageFault_Handler,      // Usage Fault Handler
    0,                                  // Reserved
    0,                                  // Reserved
    0,                                  // Reserved
    0,                                  // Reserved
    (uint32_t) SVC_Handler,             // SVCall Handler
    (uint32_t) DebugMon_Handler,        // Debug Monitor Handler
    0,                                  // Reserved
    (uint32_t) PendSV_Handler,          // PendSV Handler
    (uint32_t) SysTick_Handler,         // SysTick Handler

    /* External Interrupts */
    (uint32_t) BOD_IRQHandler,          // 0: Brown Out detection
    (uint32_t) IRC_IRQHandler,          // 1: Internal RC
    (uint32_t) PWRWU_IRQHandler,        // 2: Power down wake up
    (uint32_t) RAMPE_IRQHandler,        // 3: RAM parity error
    (uint32_t) CKFAIL_IRQHandler,       // 4: Clock detection fail
    (uint32_t) ISP_IRQHandler,          // 5: ISP
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
    (uint32_t) BRAKE0_IRQHandler,       // 24: EPWM0 brake
    (uint32_t) EPWM0P0_IRQHandler,      // 25: EPWM0 pair 0
    (uint32_t) EPWM0P1_IRQHandler,      // 26: EPWM0 pair 1
    (uint32_t) EPWM0P2_IRQHandler,      // 27: EPWM0 pair 2
    (uint32_t) BRAKE1_IRQHandler,       // 28: EPWM1 brake
    (uint32_t) EPWM1P0_IRQHandler,      // 29: EPWM1 pair 0
    (uint32_t) EPWM1P1_IRQHandler,      // 30: EPWM1 pair 1
    (uint32_t) EPWM1P2_IRQHandler,      // 31: EPWM1 pair 2
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
    (uint32_t) EADC00_IRQHandler,       // 42: EADC0 interrupt source 0
    (uint32_t) EADC01_IRQHandler,       // 43: EADC0 interrupt source 1
    (uint32_t) ACMP01_IRQHandler,       // 44: ACMP0 and ACMP1
    (uint32_t) ACMP23_IRQHandler,       // 45: ACMP2 and ACMP3
    (uint32_t) EADC02_IRQHandler,       // 46: EADC0 interrupt source 2
    (uint32_t) EADC03_IRQHandler,       // 47: EADC0 interrupt source 3
    (uint32_t) UART2_IRQHandler,        // 48: UART2
    (uint32_t) UART3_IRQHandler,        // 49: UART3
    (uint32_t) QSPI1_IRQHandler,        // 50: QSPI1
    (uint32_t) SPI1_IRQHandler,         // 51: SPI1
    (uint32_t) SPI2_IRQHandler,         // 52: SPI2
    (uint32_t) USBD_IRQHandler,         // 53: USB device
    (uint32_t) OHCI_IRQHandler,         // 54: OHCI
    (uint32_t) USBOTG_IRQHandler,       // 55: USB OTG
    (uint32_t) BMC_IRQHandler,          // 56: BMC
    (uint32_t) SPI5_IRQHandler,         // 57: SPI5
    (uint32_t) SC0_IRQHandler,          // 58: SC0
    (uint32_t) SC1_IRQHandler,          // 59: SC1
    (uint32_t) SC2_IRQHandler,          // 60: SC2
    (uint32_t) GPJ_IRQHandler,          // 61: GPIO Port J
    (uint32_t) SPI3_IRQHandler,         // 62: SPI3
    (uint32_t) SPI4_IRQHandler,         // 63: SPI4
    (uint32_t) SDH0_IRQHandler,         // 64: SDH0
    (uint32_t) USBD20_IRQHandler,       // 65: USBD20
    (uint32_t) EMAC0_IRQHandler,        // 66: EMAC0
    (uint32_t) Default_Handler,         // 67:
    (uint32_t) I2S0_IRQHandler,         // 68: I2S0
    (uint32_t) I2S1_IRQHandler,         // 69: I2S1
    (uint32_t) SPI6_IRQHandler,         // 70: SPI6
    (uint32_t) CRPT_IRQHandler,         // 71: CRYPTO
    (uint32_t) GPG_IRQHandler,          // 72: GPIO Port G
    (uint32_t) EINT6_IRQHandler,        // 73: External Input 6
    (uint32_t) UART4_IRQHandler,        // 74: UART4
    (uint32_t) UART5_IRQHandler,        // 75: UART5
    (uint32_t) USCI0_IRQHandler,        // 76: USCI0
    (uint32_t) SPI7_IRQHandler,         // 77: SPI7
    (uint32_t) BPWM0_IRQHandler,        // 78: BPWM0
    (uint32_t) BPWM1_IRQHandler,        // 79: BPWM1
    (uint32_t) SPIM_IRQHandler,         // 80: SPIM
    (uint32_t) CCAP_IRQHandler,         // 81: CCAP
    (uint32_t) I2C2_IRQHandler,         // 82: I2C2
    (uint32_t) I2C3_IRQHandler,         // 83: I2C3
    (uint32_t) EQEI0_IRQHandler,        // 84: EQEI0
    (uint32_t) EQEI1_IRQHandler,        // 85: EQEI1
    (uint32_t) ECAP0_IRQHandler,        // 86: ECAP0
    (uint32_t) ECAP1_IRQHandler,        // 87: ECAP1
    (uint32_t) GPH_IRQHandler,          // 88: GPIO Port H
    (uint32_t) EINT7_IRQHandler,        // 89: External Input 7
    (uint32_t) SDH1_IRQHandler,         // 90: SDH1
    (uint32_t) PSIO_IRQHandler,         // 91: PSIO
    (uint32_t) EHCI_IRQHandler,         // 92: EHCI
    (uint32_t) USBOTG20_IRQHandler,     // 93: HSOTG
    (uint32_t) ECAP2_IRQHandler,        // 94: ECAP2
    (uint32_t) ECAP3_IRQHandler,        // 95: ECAP3
    (uint32_t) KPI_IRQHandler,          // 96: KPI
    (uint32_t) HBI_IRQHandler,          // 97: HBI
    (uint32_t) PDMA1_IRQHandler,        // 98: Peripheral DMA 1
    (uint32_t) UART8_IRQHandler,        // 99: UART8
    (uint32_t) UART9_IRQHandler,        // 100: UART9
    (uint32_t) TRNG_IRQHandler,         // 101: TRNG
    (uint32_t) UART6_IRQHandler,        // 102: UART6
    (uint32_t) UART7_IRQHandler,        // 103: UART7
    (uint32_t) EADC10_IRQHandler,       // 104: EADC1 interrupt source 0
    (uint32_t) EADC11_IRQHandler,       // 105: EADC1 interrupt source 1
    (uint32_t) EADC12_IRQHandler,       // 106: EADC1 interrupt source 2
    (uint32_t) EADC13_IRQHandler,       // 107: EADC1 interrupt source 3
    (uint32_t) SPI8_IRQHandler,         // 108: SPI8
    (uint32_t) KS_IRQHandler,           // 109: Key Store
    (uint32_t) GPI_IRQHandler,          // 110: GPIO Port I
    (uint32_t) SPI9_IRQHandler,         // 111: SPI9
    (uint32_t) CANFD00_IRQHandler,      // 112: CANFD0 interrupt source 0
    (uint32_t) CANFD01_IRQHandler,      // 113: CANFD0 interrupt source 1
    (uint32_t) CANFD10_IRQHandler,      // 114: CANFD1 interrupt source 0
    (uint32_t) CANFD11_IRQHandler,      // 115: CANFD1 interrupt source 1
    (uint32_t) EQEI2_IRQHandler,        // 116: EQEI2
    (uint32_t) EQEI3_IRQHandler,        // 117: EQEI3
    (uint32_t) I2C4_IRQHandler,         // 118: I2C4
    (uint32_t) SPI10_IRQHandler,        // 119: SPI10
    (uint32_t) CANFD20_IRQHandler,      // 120: CANFD2 interrupt source 0
    (uint32_t) CANFD21_IRQHandler,      // 121: CANFD2 interrupt source 1
    (uint32_t) CANFD30_IRQHandler,      // 122: CANFD3 interrupt source 0
    (uint32_t) CANFD31_IRQHandler,      // 123: CANFD3 interrupt source 1
    (uint32_t) EADC20_IRQHandler,       // 124: EADC2 interrupt source 0
    (uint32_t) EADC21_IRQHandler,       // 125: EADC2 interrupt source 1
    (uint32_t) EADC22_IRQHandler,       // 126: EADC2 interrupt source 2
    (uint32_t) EADC23_IRQHandler,       // 127: EADC2 interrupt source 3
};

/* 
 * Reset_Handler: 
 *   Divert one small memory block for Initial Stack
 *   Continue Initial Stack for Reset_Handler_1
 *   Jump to Reset_Handler_1
 * 
 * Reset_Handler_1
 *   Enable other SRAM modules. From now on, these memory modules could be used for Initial Stack, depending on linker.
 *     - SRAM bank1/2
 *     - EBI SRAM (like NUC472)
 *     - SPIM CCM (like M487)
 *   Configure Initial Stack, using linker-generated symbols for Reset_Handler_2
 *   Jump to Reset_Handler_2
 * 
 * Reset_Handler_2
 *   C/C++ runtime initialization
 */

#if defined (__GNUC__) || defined (__ICCARM__)

__attribute__((naked)) void Reset_Handler(void)
{
    __asm("ldr      sp, =0x20000200                                 \n");
    __asm("mov      r0, sp                                          \n");
    __asm("ldr      r1, =Reset_Handler_1                            \n");
    __asm("b        Reset_Handler_Cascade                           \n");
}

void Reset_Handler_Cascade(void *sp, void *pc)
{
    __asm volatile (
        "mov    sp,  %0             \n"
        "bx     %1                  \n"
        :                           /* output operands */
        : "l"(sp), "l"(pc)          /* input operands */
        : "cc"                      /* list of clobbered registers */
    );
}

#else

#error "Unsupported toolchain"

#endif

void Reset_Handler_1(void)
{
    /* Disable register write-protection function */
    SYS_UnlockReg();

    /**
     * NOTE 1: Some register accesses require unlock.
     * NOTE 2: SystemInit() must be called at the very start to initialize other SRAM modules.
     */
    SystemInit();
    
    /* Enable register write-protection function */
    SYS_LockReg();
    
#if defined(__ARMCC_VERSION)
    Reset_Handler_Cascade((void *) &Image$$ARM_LIB_STACK$$ZI$$Limit, (void *) Reset_Handler_2);
#elif defined(__ICCARM__)
    Reset_Handler_Cascade((void *) &CSTACK$$Limit, (void *) Reset_Handler_2);
#elif defined(__GNUC__)
    Reset_Handler_Cascade((void *) &__StackTop, (void *) Reset_Handler_2);
#endif
}

void Reset_Handler_2(void)
{
#if defined(__ARMCC_VERSION)
    __main();

#elif defined(__ICCARM__)
    /* With 'initialize manually { section .text.nu.hyperram }' in .icf, do the
     * initialization for .text.nu.hyperram manually.
     *
     * NOTE: C runtime not initialized yet, don't invoke memcpy() for safe. */
    {
        /* BSP not defined yet, define it */
        #pragma section=".text.nu.hyperram"
        #pragma section=".text.nu.hyperram_init"
        uint32_t *src_ind = (uint32_t *) __section_begin(".text.nu.hyperram_init");
        uint32_t *src_end = (uint32_t *) __section_end(".text.nu.hyperram_init");
        uint32_t *dst_ind = (uint32_t *) __section_begin(".text.nu.hyperram");
        if (src_ind != dst_ind) {
            for (; src_ind < src_end;) {
                *dst_ind ++ = *src_ind ++;
            }
        }
    }

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
