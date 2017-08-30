/******************************************************************************
 * @file     startup_NUC472_442.c
 * @version  V0.10
 * $Revision: 11 $
 * $Date: 15/09/02 10:02a $
 * @brief    CMSIS Cortex-M4 Core Peripheral Access Layer Source File for NUC472/442 MCU
 *
 * @note
 * Copyright (C) 2013~2015 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#include "NUC472_442.h"

/* Suppress warning messages */
#if defined(__CC_ARM)
// Suppress warning message: extended constant initialiser used
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
#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
extern uint32_t Image$$ARM_LIB_STACK$$ZI$$Limit;
extern void __main(void);
#elif defined(__ICCARM__)
void __iar_program_start(void);
#elif defined(__GNUC__)
extern uint32_t __StackTop;
extern uint32_t __etext;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __bss_extern_start__  WEAK;
extern uint32_t __bss_extern_end__ WEAK;

extern void uvisor_init(void);
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
WEAK_ALIAS_FUNC(BOD_IRQHandler, Default_Handler)        // 0: Brown Out detection
WEAK_ALIAS_FUNC(IRC_IRQHandler, Default_Handler)        // 1: Internal RC
WEAK_ALIAS_FUNC(PWRWU_IRQHandler, Default_Handler)      // 2: Power Down Wake Up 
WEAK_ALIAS_FUNC(SRAMF_IRQHandler, Default_Handler)      // 3: Reserved.
WEAK_ALIAS_FUNC(CLKF_IRQHandler, Default_Handler)       // 4: CLKF
                                                        // 5: Reserved.
WEAK_ALIAS_FUNC(RTC_IRQHandler, Default_Handler)        // 6: Real Time Clock 
WEAK_ALIAS_FUNC(TAMPER_IRQHandler, Default_Handler)     // 7: Tamper detection
WEAK_ALIAS_FUNC(EINT0_IRQHandler, Default_Handler)      // 8: External Input 0
WEAK_ALIAS_FUNC(EINT1_IRQHandler, Default_Handler)      // 9: External Input 1
WEAK_ALIAS_FUNC(EINT2_IRQHandler, Default_Handler)      // 10: External Input 2
WEAK_ALIAS_FUNC(EINT3_IRQHandler, Default_Handler)      // 11: External Input 3
WEAK_ALIAS_FUNC(EINT4_IRQHandler, Default_Handler)      // 12: External Input 4
WEAK_ALIAS_FUNC(EINT5_IRQHandler, Default_Handler)      // 13: External Input 5
WEAK_ALIAS_FUNC(EINT6_IRQHandler, Default_Handler)      // 14: External Input 6
WEAK_ALIAS_FUNC(EINT7_IRQHandler, Default_Handler)      // 15: External Input 7 
WEAK_ALIAS_FUNC(GPA_IRQHandler, Default_Handler)        // 16: GPIO Port A
WEAK_ALIAS_FUNC(GPB_IRQHandler, Default_Handler)        // 17: GPIO Port B
WEAK_ALIAS_FUNC(GPC_IRQHandler, Default_Handler)        // 18: GPIO Port C
WEAK_ALIAS_FUNC(GPD_IRQHandler, Default_Handler)        // 19: GPIO Port D
WEAK_ALIAS_FUNC(GPE_IRQHandler, Default_Handler)        // 20: GPIO Port E
WEAK_ALIAS_FUNC(GPF_IRQHandler, Default_Handler)        // 21: GPIO Port F
WEAK_ALIAS_FUNC(GPG_IRQHandler, Default_Handler)        // 22: GPIO Port G
WEAK_ALIAS_FUNC(GPH_IRQHandler, Default_Handler)        // 23: GPIO Port H
WEAK_ALIAS_FUNC(GPI_IRQHandler, Default_Handler)        // 24: GPIO Port I
                                                        // 25: Reserved.
                                                        // 26: Reserved.
                                                        // 27: Reserved.
                                                        // 28: Reserved.
                                                        // 29: Reserved.
                                                        // 30: Reserved.
                                                        // 31: Reserved.
WEAK_ALIAS_FUNC(TMR0_IRQHandler, Default_Handler)       // 32: Timer 0
WEAK_ALIAS_FUNC(TMR1_IRQHandler, Default_Handler)       // 33: Timer 1
WEAK_ALIAS_FUNC(TMR2_IRQHandler, Default_Handler)       // 34: Timer 2
WEAK_ALIAS_FUNC(TMR3_IRQHandler, Default_Handler)       // 35: Timer 3
                                                        // 36: Reserved.
                                                        // 37: Reserved.
                                                        // 38: Reserved.
                                                        // 39: Reserved.
WEAK_ALIAS_FUNC(PDMA_IRQHandler, Default_Handler)       // 40: Peripheral DMA
                                                        // 41: Reserved.
WEAK_ALIAS_FUNC(ADC_IRQHandler, Default_Handler)        // 42: ADC
                                                        // 43: Reserved.
                                                        // 44: Reserved.
                                                        // 45: Reserved.
WEAK_ALIAS_FUNC(WDT_IRQHandler, Default_Handler)        // 46: Watch Dog Timer
WEAK_ALIAS_FUNC(WWDT_IRQHandler, Default_Handler)       // 47: Window Watch Dog Timer
WEAK_ALIAS_FUNC(EADC0_IRQHandler, Default_Handler)      // 48: EDAC 0
WEAK_ALIAS_FUNC(EADC1_IRQHandler, Default_Handler)      // 49: EDAC 1
WEAK_ALIAS_FUNC(EADC2_IRQHandler, Default_Handler)      // 50: EDAC 2
WEAK_ALIAS_FUNC(EADC3_IRQHandler, Default_Handler)      // 51: EDAC 3
                                                        // 52: Reserved.
                                                        // 53: Reserved.
                                                        // 54: Reserved.
                                                        // 55: Reserved.
WEAK_ALIAS_FUNC(ACMP_IRQHandler, Default_Handler)       // 56: Analog Comparator
                                                        // 57: Reserved.
                                                        // 58: Reserved.
                                                        // 59: Reserved.
WEAK_ALIAS_FUNC(OPA0_IRQHandler, Default_Handler)       // 60: OPA 0
WEAK_ALIAS_FUNC(OPA1_IRQHandler, Default_Handler)       // 61: OPA 1
WEAK_ALIAS_FUNC(ICAP0_IRQHandler, Default_Handler)      // 62: ICAP 0
WEAK_ALIAS_FUNC(ICAP1_IRQHandler, Default_Handler)      // 63: ICAP 1
WEAK_ALIAS_FUNC(PWM0CH0_IRQHandler, Default_Handler)    // 64: PWM0 CH0
WEAK_ALIAS_FUNC(PWM0CH1_IRQHandler, Default_Handler)    // 65: PWM0 CH1
WEAK_ALIAS_FUNC(PWM0CH2_IRQHandler, Default_Handler)    // 66: PWM0 CH2
WEAK_ALIAS_FUNC(PWM0CH3_IRQHandler, Default_Handler)    // 67: PWM0 CH3
WEAK_ALIAS_FUNC(PWM0CH4_IRQHandler, Default_Handler)    // 68: PWM0 CH4
WEAK_ALIAS_FUNC(PWM0CH5_IRQHandler, Default_Handler)    // 69: PWM0 CH5
WEAK_ALIAS_FUNC(PWM0_BRK_IRQHandler, Default_Handler)   // 70: PWM0 Break
WEAK_ALIAS_FUNC(QEI0_IRQHandler, Default_Handler)       // 71: QEI 0
WEAK_ALIAS_FUNC(PWM1CH0_IRQHandler, Default_Handler)    // 72: PWM1 CH0
WEAK_ALIAS_FUNC(PWM1CH1_IRQHandler, Default_Handler)    // 73: PWM1 CH1
WEAK_ALIAS_FUNC(PWM1CH2_IRQHandler, Default_Handler)    // 74: PWM1 CH2
WEAK_ALIAS_FUNC(PWM1CH3_IRQHandler, Default_Handler)    // 75: PWM1 CH3
WEAK_ALIAS_FUNC(PWM1CH4_IRQHandler, Default_Handler)    // 76: PWM1 CH4
WEAK_ALIAS_FUNC(PWM1CH5_IRQHandler, Default_Handler)    // 77: PWM1 CH5
WEAK_ALIAS_FUNC(PWM1_BRK_IRQHandler, Default_Handler)   // 78: PWM1 Break
WEAK_ALIAS_FUNC(QEI1_IRQHandler, Default_Handler)       // 79: QEI 1
WEAK_ALIAS_FUNC(EPWM0_IRQHandler, Default_Handler)      // 80: EPWM0
WEAK_ALIAS_FUNC(EPWM0BRK_IRQHandler, Default_Handler)   // 81: EPWM0 Break
WEAK_ALIAS_FUNC(EPWM1_IRQHandler, Default_Handler)      // 82: EPWM1
WEAK_ALIAS_FUNC(EPWM1BRK_IRQHandler, Default_Handler)   // 83: EPWM1 Break
                                                        // 84: Reserved.
                                                        // 85: Reserved.
                                                        // 86: Reserved.
                                                        // 87: Reserved.
WEAK_ALIAS_FUNC(USBD_IRQHandler, Default_Handler)       // 88: USB Device
WEAK_ALIAS_FUNC(USBH_IRQHandler, Default_Handler)       // 89: USB Host
WEAK_ALIAS_FUNC(USB_OTG_IRQHandler, Default_Handler)    // 90: USB OTG
                                                        // 91: Reserved.
WEAK_ALIAS_FUNC(EMAC_TX_IRQHandler, Default_Handler)    // 92: Ethernet MAC TX
WEAK_ALIAS_FUNC(EMAC_RX_IRQHandler, Default_Handler)    // 93: Ethernet MAC RX
                                                        // 94: Reserved.
                                                        // 95: Reserved.
WEAK_ALIAS_FUNC(SPI0_IRQHandler, Default_Handler)       // 96: SPI 0
WEAK_ALIAS_FUNC(SPI1_IRQHandler, Default_Handler)       // 97: SPI 1
WEAK_ALIAS_FUNC(SPI2_IRQHandler, Default_Handler)       // 98: SPI 2
WEAK_ALIAS_FUNC(SPI3_IRQHandler, Default_Handler)       // 99: SPI 3
                                                        // 100: Reserved.
                                                        // 101: Reserved.
                                                        // 102: Reserved.
                                                        // 103: Reserved.
WEAK_ALIAS_FUNC(UART0_IRQHandler, Default_Handler)      // 104: UART 0
WEAK_ALIAS_FUNC(UART1_IRQHandler, Default_Handler)      // 105: UART 1
WEAK_ALIAS_FUNC(UART2_IRQHandler, Default_Handler)      // 106: UART 2
WEAK_ALIAS_FUNC(UART3_IRQHandler, Default_Handler)      // 107: UART 3
WEAK_ALIAS_FUNC(UART4_IRQHandler, Default_Handler)      // 108: UART 4
WEAK_ALIAS_FUNC(UART5_IRQHandler, Default_Handler)      // 109: UART 5
                                                        // 110: Reserved.
                                                        // 111: Reserved.
WEAK_ALIAS_FUNC(I2C0_IRQHandler, Default_Handler)       // 112: I2C 0
WEAK_ALIAS_FUNC(I2C1_IRQHandler, Default_Handler)       // 113: I2C 1
WEAK_ALIAS_FUNC(I2C2_IRQHandler, Default_Handler)       // 114: I2C 2
WEAK_ALIAS_FUNC(I2C3_IRQHandler, Default_Handler)       // 115: I2C 3
WEAK_ALIAS_FUNC(I2C4_IRQHandler, Default_Handler)       // 116: I2C 4
                                                        // 117: Reserved.
                                                        // 118: Reserved.
                                                        // 119: Reserved.
WEAK_ALIAS_FUNC(SC0_IRQHandler, Default_Handler)        // 120: Smart Card 0
WEAK_ALIAS_FUNC(SC1_IRQHandler, Default_Handler)        // 121: Smart Card 1
WEAK_ALIAS_FUNC(SC2_IRQHandler, Default_Handler)        // 122: Smart Card 2
WEAK_ALIAS_FUNC(SC3_IRQHandler, Default_Handler)        // 123: Smart Card 3
WEAK_ALIAS_FUNC(SC4_IRQHandler, Default_Handler)        // 124: Smart Card 4
WEAK_ALIAS_FUNC(SC5_IRQHandler, Default_Handler)        // 125: Smart Card 5
                                                        // 126: Reserved.
                                                        // 127: Reserved.
WEAK_ALIAS_FUNC(CAN0_IRQHandler, Default_Handler)       // 128: CAN 0
WEAK_ALIAS_FUNC(CAN1_IRQHandler, Default_Handler)       // 129: CAN 1
                                                        // 130: Reserved.
                                                        // 131: Reserved.
WEAK_ALIAS_FUNC(I2S0_IRQHandler, Default_Handler)       // 132: I2S 0
WEAK_ALIAS_FUNC(I2S1_IRQHandler, Default_Handler)       // 133: I2S 1
                                                        // 134: Reserved.
                                                        // 135: Reserved.
WEAK_ALIAS_FUNC(SD_IRQHandler, Default_Handler)         // 136: SD card
                                                        // 137: Reserved.
WEAK_ALIAS_FUNC(PS2D_IRQHandler, Default_Handler)       // 138: PS/2 device
WEAK_ALIAS_FUNC(CAP_IRQHandler, Default_Handler)        // 139: VIN
WEAK_ALIAS_FUNC(CRYPTO_IRQHandler, Default_Handler)     // 140: CRYPTO
WEAK_ALIAS_FUNC(CRC_IRQHandler, Default_Handler)        // 141: CRC

/* Vector table */
#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
__attribute__ ((section("RESET")))
const uint32_t __vector_handlers[] = {
#elif defined(__ICCARM__)
extern uint32_t CSTACK$$Limit;
const uint32_t __vector_table[] @ ".intvec" = {
#elif defined(__GNUC__)
__attribute__ ((section(".vector_table")))
const uint32_t __vector_handlers[] = {
#endif

    /* Configure Initial Stack Pointer, using linker-generated symbols */
#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
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
    (uint32_t) PWRWU_IRQHandler,        // 2: Power Down Wake Up 
    (uint32_t) SRAMF_IRQHandler,        // 3: Reserved.
    (uint32_t) CLKF_IRQHandler,         // 4: CLKF
    (uint32_t) Default_Handler,         // 5: Reserved.
    (uint32_t) RTC_IRQHandler,          // 6: Real Time Clock 
    (uint32_t) TAMPER_IRQHandler,       // 7: Tamper detection
    (uint32_t) EINT0_IRQHandler,        // 8: External Input 0
    (uint32_t) EINT1_IRQHandler,        // 9: External Input 1
    (uint32_t) EINT2_IRQHandler,        // 10: External Input 2
    (uint32_t) EINT3_IRQHandler,        // 11: External Input 3
    (uint32_t) EINT4_IRQHandler,        // 12: External Input 4
    (uint32_t) EINT5_IRQHandler,        // 13: External Input 5
    (uint32_t) EINT6_IRQHandler,        // 14: External Input 6
    (uint32_t) EINT7_IRQHandler,        // 15: External Input 7 
    (uint32_t) GPA_IRQHandler,          // 16: GPIO Port A
    (uint32_t) GPB_IRQHandler,          // 17: GPIO Port B
    (uint32_t) GPC_IRQHandler,          // 18: GPIO Port C
    (uint32_t) GPD_IRQHandler,          // 19: GPIO Port D
    (uint32_t) GPE_IRQHandler,          // 20: GPIO Port E
    (uint32_t) GPF_IRQHandler,          // 21: GPIO Port F
    (uint32_t) GPG_IRQHandler,          // 22: GPIO Port G
    (uint32_t) GPH_IRQHandler,          // 23: GPIO Port H
    (uint32_t) GPI_IRQHandler,          // 24: GPIO Port I
    (uint32_t) Default_Handler,         // 25: Reserved.
    (uint32_t) Default_Handler,         // 26: Reserved.
    (uint32_t) Default_Handler,         // 27: Reserved.
    (uint32_t) Default_Handler,         // 28: Reserved.
    (uint32_t) Default_Handler,         // 29: Reserved.
    (uint32_t) Default_Handler,         // 30: Reserved.
    (uint32_t) Default_Handler,         // 31: Reserved.
    (uint32_t) TMR0_IRQHandler,         // 32: Timer 0
    (uint32_t) TMR1_IRQHandler,         // 33: Timer 1
    (uint32_t) TMR2_IRQHandler,         // 34: Timer 2
    (uint32_t) TMR3_IRQHandler,         // 35: Timer 3
    (uint32_t) Default_Handler,         // 36: Reserved.
    (uint32_t) Default_Handler,         // 37: Reserved.
    (uint32_t) Default_Handler,         // 38: Reserved.
    (uint32_t) Default_Handler,         // 39: Reserved.
    (uint32_t) PDMA_IRQHandler,         // 40: Peripheral DMA
    (uint32_t) Default_Handler,         // 41: Reserved.
    (uint32_t) ADC_IRQHandler,          // 42: ADC
    (uint32_t) Default_Handler,         // 43: Reserved.
    (uint32_t) Default_Handler,         // 44: Reserved.
    (uint32_t) Default_Handler,         // 45: Reserved.
    (uint32_t) WDT_IRQHandler,          // 46: Watch Dog Timer
    (uint32_t) WWDT_IRQHandler,         // 47: Window Watch Dog Timer
    (uint32_t) EADC0_IRQHandler,        // 48: EDAC 0
    (uint32_t) EADC1_IRQHandler,        // 49: EDAC 1
    (uint32_t) EADC2_IRQHandler,        // 50: EDAC 2
    (uint32_t) EADC3_IRQHandler,        // 51: EDAC 3
    (uint32_t) Default_Handler,         // 52: Reserved.
    (uint32_t) Default_Handler,         // 53: Reserved.
    (uint32_t) Default_Handler,         // 54: Reserved.
    (uint32_t) Default_Handler,         // 55: Reserved.
    (uint32_t) ACMP_IRQHandler,         // 56: Analog Comparator
    (uint32_t) Default_Handler,         // 57: Reserved.
    (uint32_t) Default_Handler,         // 58: Reserved.
    (uint32_t) Default_Handler,         // 59: Reserved.
    (uint32_t) OPA0_IRQHandler,         // 60: OPA 0
    (uint32_t) OPA1_IRQHandler,         // 61: OPA 1
    (uint32_t) ICAP0_IRQHandler,        // 62: ICAP 0
    (uint32_t) ICAP1_IRQHandler,        // 63: ICAP 1
    (uint32_t) PWM0CH0_IRQHandler,      // 64: PWM0 CH0
    (uint32_t) PWM0CH1_IRQHandler,      // 65: PWM0 CH1
    (uint32_t) PWM0CH2_IRQHandler,      // 66: PWM0 CH2
    (uint32_t) PWM0CH3_IRQHandler,      // 67: PWM0 CH3
    (uint32_t) PWM0CH4_IRQHandler,      // 68: PWM0 CH4
    (uint32_t) PWM0CH5_IRQHandler,      // 69: PWM0 CH5
    (uint32_t) PWM0_BRK_IRQHandler,     // 70: PWM0 Break
    (uint32_t) QEI0_IRQHandler,         // 71: QEI 0
    (uint32_t) PWM1CH0_IRQHandler,      // 72: PWM1 CH0
    (uint32_t) PWM1CH1_IRQHandler,      // 73: PWM1 CH1
    (uint32_t) PWM1CH2_IRQHandler,      // 74: PWM1 CH2
    (uint32_t) PWM1CH3_IRQHandler,      // 75: PWM1 CH3
    (uint32_t) PWM1CH4_IRQHandler,      // 76: PWM1 CH4
    (uint32_t) PWM1CH5_IRQHandler,      // 77: PWM1 CH5
    (uint32_t) PWM1_BRK_IRQHandler,     // 78: PWM1 Break
    (uint32_t) QEI1_IRQHandler,         // 79: QEI 1
    (uint32_t) EPWM0_IRQHandler,        // 80: EPWM0
    (uint32_t) EPWM0BRK_IRQHandler,     // 81: EPWM0 Break
    (uint32_t) EPWM1_IRQHandler,        // 82: EPWM1
    (uint32_t) EPWM1BRK_IRQHandler,     // 83: EPWM1 Break
    (uint32_t) Default_Handler,         // 84: Reserved.
    (uint32_t) Default_Handler,         // 85: Reserved.
    (uint32_t) Default_Handler,         // 86: Reserved.
    (uint32_t) Default_Handler,         // 87: Reserved.
    (uint32_t) USBD_IRQHandler,         // 88: USB Device
    (uint32_t) USBH_IRQHandler,         // 89: USB Host
    (uint32_t) USB_OTG_IRQHandler,      // 90: USB OTG
    (uint32_t) Default_Handler,         // 91: Reserved.
    (uint32_t) EMAC_TX_IRQHandler,      // 92: Ethernet MAC TX
    (uint32_t) EMAC_RX_IRQHandler,      // 93: Ethernet MAC RX
    (uint32_t) Default_Handler,         // 94: Reserved.
    (uint32_t) Default_Handler,         // 95: Reserved.
    (uint32_t) SPI0_IRQHandler,         // 96: SPI 0
    (uint32_t) SPI1_IRQHandler,         // 97: SPI 1
    (uint32_t) SPI2_IRQHandler,         // 98: SPI 2
    (uint32_t) SPI3_IRQHandler,         // 99: SPI 3
    (uint32_t) Default_Handler,         // 100: Reserved.
    (uint32_t) Default_Handler,         // 101: Reserved.
    (uint32_t) Default_Handler,         // 102: Reserved.
    (uint32_t) Default_Handler,         // 103: Reserved.
    (uint32_t) UART0_IRQHandler,        // 104: UART 0
    (uint32_t) UART1_IRQHandler,        // 105: UART 1
    (uint32_t) UART2_IRQHandler,        // 106: UART 2
    (uint32_t) UART3_IRQHandler,        // 107: UART 3
    (uint32_t) UART4_IRQHandler,        // 108: UART 4
    (uint32_t) UART5_IRQHandler,        // 109: UART 5
    (uint32_t) Default_Handler,         // 110: Reserved.
    (uint32_t) Default_Handler,         // 111: Reserved.
    (uint32_t) I2C0_IRQHandler,         // 112: I2C 0
    (uint32_t) I2C1_IRQHandler,         // 113: I2C 1
    (uint32_t) I2C2_IRQHandler,         // 114: I2C 2
    (uint32_t) I2C3_IRQHandler,         // 115: I2C 3
    (uint32_t) I2C4_IRQHandler,         // 116: I2C 4
    (uint32_t) Default_Handler,         // 117: Reserved.
    (uint32_t) Default_Handler,         // 118: Reserved.
    (uint32_t) Default_Handler,         // 119: Reserved.
    (uint32_t) SC0_IRQHandler,          // 120: Smart Card 0
    (uint32_t) SC1_IRQHandler,          // 121: Smart Card 1
    (uint32_t) SC2_IRQHandler,          // 122: Smart Card 2
    (uint32_t) SC3_IRQHandler,          // 123: Smart Card 3
    (uint32_t) SC4_IRQHandler,          // 124: Smart Card 4
    (uint32_t) SC5_IRQHandler,          // 125: Smart Card 5
    (uint32_t) Default_Handler,         // 126: Reserved.
    (uint32_t) Default_Handler,         // 127: Reserved.
    (uint32_t) CAN0_IRQHandler,         // 128: CAN 0
    (uint32_t) CAN1_IRQHandler,         // 129: CAN 1
    (uint32_t) Default_Handler,         // 130: Reserved.
    (uint32_t) Default_Handler,         // 131: Reserved.
    (uint32_t) I2S0_IRQHandler,         // 132: I2S 0
    (uint32_t) I2S1_IRQHandler,         // 133: I2S 1
    (uint32_t) Default_Handler,         // 134: Reserved.
    (uint32_t) Default_Handler,         // 135: Reserved.
    (uint32_t) SD_IRQHandler,           // 136: SD card
    (uint32_t) Default_Handler,         // 137: Reserved.
    (uint32_t) PS2D_IRQHandler,         // 138: PS/2 device
    (uint32_t) CAP_IRQHandler,          // 139: VIN
    (uint32_t) CRYPTO_IRQHandler,       // 140: CRYPTO
    (uint32_t) CRC_IRQHandler,          // 141: CRC    
};

/**
 * \brief This is the code that gets called on processor reset.
 */
void Reset_Handler(void)
{
    /* Disable register write-protection function */
    SYS_UnlockReg();
    
    /* Disable branch buffer if VCID is 0 */
    if (SYS->VCID == 0) {
        FMC->FTCTL |= 0x80;
    }
    
    /* Disable Power-on Reset function */
    SYS_DISABLE_POR();
    
    /**
     * NOTE 1: Unlock is required for perhaps some register access in SystemInit().
     * NOTE 2: Because EBI (external SRAM) init is done in SystemInit(), SystemInit() must be called at the very start.
     */
    SystemInit();
    
    /* Enable register write-protection function */
    SYS_LockReg();

#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
    __main();
    
#elif defined(__ICCARM__)
    __iar_program_start();

#elif defined(__GNUC__)
    uint32_t *src_ind = (uint32_t *) &__etext;
    uint32_t *dst_ind = (uint32_t *) &__data_start__;
    uint32_t *dst_end = (uint32_t *) &__data_end__;
    
    /* Move .data section from ROM to RAM */
    if (src_ind != dst_ind) {
        for (; dst_ind < dst_end;) {
            *dst_ind ++ = *src_ind ++;
        }
    }
   
    /* Initialize .bss section to zero */
    dst_ind = (uint32_t *) &__bss_start__;
    dst_end = (uint32_t *) &__bss_end__;
    if (dst_ind != dst_end) {
        for (; dst_ind < dst_end;) {
            *dst_ind ++ = 0;
        }
    }
    
    /* Initialize .bss.extern section to zero */
    dst_ind = (uint32_t *) &__bss_extern_start__;
    dst_end = (uint32_t *) &__bss_extern_end__;
    if (dst_ind != dst_end) {
        for (; dst_ind < dst_end;) {
            *dst_ind ++ = 0;
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
