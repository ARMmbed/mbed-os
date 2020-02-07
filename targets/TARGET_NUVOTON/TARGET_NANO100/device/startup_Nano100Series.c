/******************************************************************************
 * @file     startup_Nano100Series.c
 * @version  V1.00
 * $Revision: 4 $
 * $Date: 15/06/08 5:12p $ 
 * @brief    CMSIS ARM Cortex-M0 Core Device Startup File
 *
 * @note
 * Copyright (C) 2013 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/  

#include "Nano100Series.h"

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

/* Cortex-M0 core handlers */
WEAK_ALIAS_FUNC(NMI_Handler, Default_Handler)               // NMI Handler
WEAK_ALIAS_FUNC(HardFault_Handler, Default_Handler)         // Hard Fault Handler
WEAK_ALIAS_FUNC(SVC_Handler, Default_Handler)               // SVCall Handler
WEAK_ALIAS_FUNC(PendSV_Handler, Default_Handler)            // PendSV Handler
WEAK_ALIAS_FUNC(SysTick_Handler, Default_Handler)           // SysTick Handler

/* Peripherals handlers */
WEAK_ALIAS_FUNC(BOD_IRQHandler, Default_Handler)            // Brownout low voltage detected interrupt  
WEAK_ALIAS_FUNC(WDT_IRQHandler, Default_Handler)            // Watch Dog Timer interrupt  
WEAK_ALIAS_FUNC(EINT0_IRQHandler, Default_Handler)          // External signal interrupt from PB.14 pin
WEAK_ALIAS_FUNC(EINT1_IRQHandler, Default_Handler)          // External signal interrupt from PB.15 pin
WEAK_ALIAS_FUNC(GPABC_IRQHandler, Default_Handler)          // External interrupt from PA[15:0]/PB[15:0]/PC[15:0]  
WEAK_ALIAS_FUNC(GPDEF_IRQHandler, Default_Handler)          // External interrupt from PD[15:0]/PE[15:0]/PF[7:0]
WEAK_ALIAS_FUNC(PWM0_IRQHandler, Default_Handler)           // PWM 0 interrupt 
WEAK_ALIAS_FUNC(PWM1_IRQHandler, Default_Handler)           // PWM 1 interrupt  
WEAK_ALIAS_FUNC(TMR0_IRQHandler, Default_Handler)           // Timer 0 interrupt
WEAK_ALIAS_FUNC(TMR1_IRQHandler, Default_Handler)           // Timer 1 interrupt  
WEAK_ALIAS_FUNC(TMR2_IRQHandler, Default_Handler)           // Timer 2 interrupt 
WEAK_ALIAS_FUNC(TMR3_IRQHandler, Default_Handler)           // Timer 3 interrupt 
WEAK_ALIAS_FUNC(UART0_IRQHandler, Default_Handler)          // UART0 interrupt
WEAK_ALIAS_FUNC(UART1_IRQHandler, Default_Handler)          // UART1 interrupt
WEAK_ALIAS_FUNC(SPI0_IRQHandler, Default_Handler)           // SPI0 interrupt 
WEAK_ALIAS_FUNC(SPI1_IRQHandler, Default_Handler)           // SPI1 interrupt 
WEAK_ALIAS_FUNC(SPI2_IRQHandler, Default_Handler)           // SPI2 interrupt 
WEAK_ALIAS_FUNC(HIRC_IRQHandler, Default_Handler)           // HIRC interrupt 
WEAK_ALIAS_FUNC(I2C0_IRQHandler, Default_Handler)           // I2C0 interrupt 
WEAK_ALIAS_FUNC(I2C1_IRQHandler, Default_Handler)           // I2C1 interrupt 
WEAK_ALIAS_FUNC(SC2_IRQHandler, Default_Handler)            // SC2 interrupt
WEAK_ALIAS_FUNC(SC0_IRQHandler, Default_Handler)            // SC0 interrupt
WEAK_ALIAS_FUNC(SC1_IRQHandler, Default_Handler)            // SC1 interrupt
WEAK_ALIAS_FUNC(USBD_IRQHandler, Default_Handler)           // USB FS Device interrupt 
                                                            // Reserved  
WEAK_ALIAS_FUNC(LCD_IRQHandler, Default_Handler)            // LCD interrupt 
WEAK_ALIAS_FUNC(PDMA_IRQHandler, Default_Handler)           // PDMA interrupt
WEAK_ALIAS_FUNC(I2S_IRQHandler, Default_Handler)            // I2S interrupt 
WEAK_ALIAS_FUNC(PDWU_IRQHandler, Default_Handler)           // Power Down Wake up interrupt
WEAK_ALIAS_FUNC(ADC_IRQHandler, Default_Handler)            // ADC interrupt
WEAK_ALIAS_FUNC(DAC_IRQHandler, Default_Handler)            // DAC interrupt
WEAK_ALIAS_FUNC(RTC_IRQHandler, Default_Handler)            // Real time clock interrupt

/* Vector table */
#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
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
#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
    (uint32_t) &Image$$ARM_LIB_STACK$$ZI$$Limit,
#elif defined(__ICCARM__)
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
    (uint32_t) BOD_IRQHandler,          // Brownout low voltage detected interrupt  
    (uint32_t) WDT_IRQHandler,          // Watch Dog Timer interrupt  
    (uint32_t) EINT0_IRQHandler,        // External signal interrupt from PB.14 pin
    (uint32_t) EINT1_IRQHandler,        // External signal interrupt from PB.15 pin
    (uint32_t) GPABC_IRQHandler,        // External interrupt from PA[15:0]/PB[15:0]/PC[15:0]  
    (uint32_t) GPDEF_IRQHandler,        // External interrupt from PD[15:0]/PE[15:0]/PF[7:0]
    (uint32_t) PWM0_IRQHandler,         // PWM 0 interrupt 
    (uint32_t) PWM1_IRQHandler,         // PWM 1 interrupt  
    (uint32_t) TMR0_IRQHandler,         // Timer 0 interrupt
    (uint32_t) TMR1_IRQHandler,         // Timer 1 interrupt  
    (uint32_t) TMR2_IRQHandler,         // Timer 2 interrupt 
    (uint32_t) TMR3_IRQHandler,         // Timer 3 interrupt 
    (uint32_t) UART0_IRQHandler,        // UART0 interrupt
    (uint32_t) UART1_IRQHandler,        // UART1 interrupt
    (uint32_t) SPI0_IRQHandler,         // SPI0 interrupt 
    (uint32_t) SPI1_IRQHandler,         // SPI1 interrupt 
    (uint32_t) SPI2_IRQHandler,         // SPI2 interrupt 
    (uint32_t) HIRC_IRQHandler,         // HIRC interrupt 
    (uint32_t) I2C0_IRQHandler,         // I2C0 interrupt 
    (uint32_t) I2C1_IRQHandler,         // I2C1 interrupt 
    (uint32_t) SC2_IRQHandler,          // SC2 interrupt
    (uint32_t) SC0_IRQHandler,          // SC0 interrupt
    (uint32_t) SC1_IRQHandler,          // SC1 interrupt
    (uint32_t) USBD_IRQHandler,         // USB FS Device interrupt 
    (uint32_t) Default_Handler,         // Reserved  
    (uint32_t) LCD_IRQHandler,          // LCD interrupt 
    (uint32_t) PDMA_IRQHandler,         // PDMA interrupt
    (uint32_t) I2S_IRQHandler,          // I2S interrupt 
    (uint32_t) PDWU_IRQHandler,         // Power Down Wake up interrupt
    (uint32_t) ADC_IRQHandler,          // ADC interrupt
    (uint32_t) DAC_IRQHandler,          // DAC interrupt
    (uint32_t) RTC_IRQHandler,          // Real time clock interrupt
};

/**
 * \brief This is the code that gets called on processor reset.
 */
void Reset_Handler(void)
{
    /* Disable register write-protection function */
    SYS_UnlockReg();
    
    /* Disable Power-on Reset function */
    SYS_DISABLE_POR();
    
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
