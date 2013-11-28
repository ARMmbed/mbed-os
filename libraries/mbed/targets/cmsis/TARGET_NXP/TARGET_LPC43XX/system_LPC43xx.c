/*
 * @brief LPC43xx System Initialization
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 *
 * Modified by Micromint USA <support@micromint.com>
 */
#include "LPC43xx.h"

#define COUNT_OF(a) (sizeof(a)/sizeof(a[0]))

/* Clock variables */
//uint32_t SystemCoreClock = CRYSTAL_MAIN_FREQ_IN;		/*!< System Clock Frequency (Core Clock)*/
uint32_t SystemCoreClock = 204000000;

#if !defined(CORE_M0)
/* SCU pin definitions for pin muxing */
typedef struct {
    __IO uint32_t *reg; /* SCU register address */
    uint16_t mode;      /* SCU pin mode and function */
} PINMUX_GRP_T;

/* Local functions */
static void SystemCoreClockUpdate(void);
static void SystemSetupClock(void);
static void SystemSetupPins(const PINMUX_GRP_T *mux, uint32_t n);
static void SystemSetupMemory(void);
static void WaitUs(uint32_t us);

/* Pins to initialize before clocks are configured */
static const PINMUX_GRP_T pre_clock_mux[] = {
    /* SPIFI pins */
    {SCU_REG(0x3, 3), (SCU_PINIO_FAST | 0x3)},  // P3_3 SPIFI CLK
    {SCU_REG(0x3, 4), (SCU_PINIO_FAST | 0x3)},  // P3_4 SPIFI D3
    {SCU_REG(0x3, 5), (SCU_PINIO_FAST | 0x3)},  // P3_5 SPIFI D2
    {SCU_REG(0x3, 6), (SCU_PINIO_FAST | 0x3)},  // P3_6 SPIFI D1
    {SCU_REG(0x3, 7), (SCU_PINIO_FAST | 0x3)},  // P3_7 SPIFI D0
    {SCU_REG(0x3, 8), (SCU_PINIO_FAST | 0x3)}   // P3_8 SPIFI CS/SSEL
};

/* Pins to initialize after clocks are configured */
static const PINMUX_GRP_T post_clock_mux[] = {
    /* Boot pins */
    {SCU_REG(0x1, 1), (SCU_PINIO_FAST | 0x0)},  // P1_1  BOOT0
    {SCU_REG(0x1, 2), (SCU_PINIO_FAST | 0x0)},  // P1_2  BOOT1
    {SCU_REG(0x2, 8), (SCU_PINIO_FAST | 0x0)},  // P2_8  BOOT2
    {SCU_REG(0x2, 9), (SCU_PINIO_FAST | 0x0)}   // P2_9  BOOT3
};
#endif /* !defined(CORE_M0) */

/*
 * SystemInit() - Initialize the system
 */
void SystemInit(void)
{
#if !defined(CORE_M0)
    unsigned int *pSCB_VTOR = (unsigned int *) 0xE000ED08;

#if defined(__ARMCC_VERSION)
    extern void *__Vectors;

    *pSCB_VTOR = (unsigned int) &__Vectors;
#elif defined(__IAR_SYSTEMS_ICC__)
    extern void *__vector_table;

    *pSCB_VTOR = (unsigned int) &__vector_table;
#elif defined(TOOLCHAIN_GCC_ARM)
    extern void *__isr_vector;

    *pSCB_VTOR = (unsigned int) &__isr_vector;
#else /* defined(__GNUC__) and others */
    extern void *g_pfnVectors;

    *pSCB_VTOR = (unsigned int) &g_pfnVectors;
#endif

#if defined(__FPU_PRESENT) && __FPU_PRESENT == 1
    /* Initialize floating point */
    fpuInit();
#endif

    SystemSetupPins(pre_clock_mux, COUNT_OF(pre_clock_mux)); /* Configure pins */

    SystemSetupClock();   /* Configure processor and peripheral clocks */
    SystemSetupPins(post_clock_mux, COUNT_OF(post_clock_mux)); /* Configure pins */
    SystemSetupMemory();  /* Configure external memory */
#endif /* !defined(CORE_M0) */

    SystemCoreClockUpdate(); /* Update SystemCoreClock variable */
}

/*
 * SystemCoreClockUpdate() - Update SystemCoreClock variable
 */
void SystemCoreClockUpdate(void)
{
}

#if !defined(CORE_M0)
/*
 * SystemSetupClock() - Set processor and peripheral clocks
 */
void SystemSetupClock(void)
{
#if (CLOCK_SETUP)
    /* Switch main clock to Internal RC (IRC) */
    LPC_CGU->BASE_CLK[CLK_BASE_MX] = ((1 << 11) | (CLKIN_IRC << 24));

    /* Enable the oscillator and wait 100 us */
    LPC_CGU->XTAL_OSC_CTRL = 0;
    WaitUs(100);

#if (SPIFI_INIT)
    /* Switch IDIVA clock to IRC and connect to SPIFI clock */
    LPC_CGU->IDIV_CTRL[CLK_IDIV_A] =  ((1 << 11) | (CLKIN_IRC << 24));
    LPC_CGU->BASE_CLK[CLK_BASE_SPIFI] =  ((1 << 11) | (CLKIN_IDIVA << 24));
#endif /* SPIFI_INIT */

    /* Power down PLL1 */
    LPC_CGU->PLL1_CTRL |= 1;

    /* Change PLL1 to 108 Mhz (msel=9, 12 MHz*9=108 MHz) */
//    LPC_CGU->PLL1_CTRL = (DIRECT << 7) | (PSEL << 8) | (1 << 11) | (P(NSEL-1) << 12)  | ((MSEL-1) << 16) | (CLKIN_PLL1 << 24);
    LPC_CGU->PLL1_CTRL = (1 << 7) | (0 << 8) | (1 << 11) | (0 << 12) | (8 << 16) | (CLKIN_PLL1 << 24);
    while (!(LPC_CGU->PLL1_STAT & 1)); /* Wait for PLL1 to lock */
    WaitUs(100);

    /* Change PLL1 to 204 Mhz (msel=17, 12 MHz*17=204 MHz) */
    LPC_CGU->PLL1_CTRL = (1 << 7) | (0 << 8) | (1 << 11) | (0 << 12) | (16 << 16) | (CLKIN_PLL1 << 24);
    while (!(LPC_CGU->PLL1_STAT & 1)); /* Wait for PLL1 to lock */

    /* Switch main clock to PLL1 */
    LPC_CGU->BASE_CLK[CLK_BASE_MX] = ((1 << 11) | (CLKIN_PLL1 << 24));
    SystemCoreClock = 204000000;
#endif /* CLOCK_SETUP */
}

/*
 * SystemSetupPins() - Configure MCU pins
 */
void SystemSetupPins(const PINMUX_GRP_T *mux, uint32_t n)
{
    uint16_t i;

    for (i = 0; i < n; i++) {
        *(mux[i].reg) = mux[i].mode;
    }
}

/*
 * SystemSetupMemory() - Configure external memory
 */
void SystemSetupMemory(void)
{
#if (MEMORY_SETUP)
    /* None required for boards without external memory */
#endif /* MEMORY_SETUP */
}

#if defined(__FPU_PRESENT) && __FPU_PRESENT == 1
/*
 * fpuInit() - Early initialization of the FPU
 */
void fpuInit(void)
{
	// from ARM TRM manual:
	//   ; CPACR is located at address 0xE000ED88
	//   LDR.W R0, =0xE000ED88
	//   ; Read CPACR
	//   LDR R1, [R0]
	//   ; Set bits 20-23 to enable CP10 and CP11 coprocessors
	//   ORR R1, R1, #(0xF << 20)
	//   ; Write back the modified value to the CPACR
	//   STR R1, [R0]

	volatile uint32_t *regCpacr = (uint32_t *) LPC_CPACR;
	volatile uint32_t *regMvfr0 = (uint32_t *) SCB_MVFR0;
	volatile uint32_t *regMvfr1 = (uint32_t *) SCB_MVFR1;
	volatile uint32_t Cpacr;
	volatile uint32_t Mvfr0;
	volatile uint32_t Mvfr1;
	char vfpPresent = 0;

	Mvfr0 = *regMvfr0;
	Mvfr1 = *regMvfr1;

	vfpPresent = ((SCB_MVFR0_RESET == Mvfr0) && (SCB_MVFR1_RESET == Mvfr1));

	if (vfpPresent) {
		Cpacr = *regCpacr;
		Cpacr |= (0xF << 20);
		*regCpacr = Cpacr;	// enable CP10 and CP11 for full access
	}

}
#endif /* defined(__FPU_PRESENT) && __FPU_PRESENT == 1 */

/* Approximate delay function */
#define CPU_NANOSEC(x)  (((uint64_t) (x) * SystemCoreClock) / 1000000000)

static void WaitUs(uint32_t us)
{
    uint32_t  cyc = us * CPU_NANOSEC(1000) / 4;
    while (cyc--)
        ;
}

#endif /* !defined(CORE_M0) */
