/**
 * \file
 *
 * \brief Chip-specific system clock management functions.
 *
 * Copyright (c) 2020 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef CHIP_SYSCLK_H_INCLUDED
#define CHIP_SYSCLK_H_INCLUDED

#include "compiler.h" 
#include "osc.h"
#include "pll.h"
#include "genclk.h"
#include "pic32cx.h"
#include "clocking.h"

/**
 * \page sysclk_quickstart Quick Start Guide for the System Clock Management service (SAM4C)
 *
 * This is the quick start guide for the \ref sysclk_group "System Clock Management"
 * service, with step-by-step instructions on how to configure and use the service for
 * specific use cases.
 *
 * \section sysclk_quickstart_usecases System Clock Management use cases
 * - \ref sysclk_quickstart_basic
 *
 * \section sysclk_quickstart_basic Basic usage of the System Clock Management service
 * This section will present a basic use case for the System Clock Management service.
 * This use case will configure the main system clock to 200MHz, using an internal PLLB
 * module to multiply the frequency of a crystal attached to the microcontroller.
 *
 * \subsection sysclk_quickstart_use_case_1_prereq Prerequisites
 *  - None
 *
 * \subsection sysclk_quickstart_use_case_1_setup_steps Initialization code
 * Add to the application initialization code:
 * \code
 *      sysclk_init();
 * \endcode
 *
 * \subsection sysclk_quickstart_use_case_1_setup_steps_workflow Workflow
 * -# Configure the system clocks according to the settings in conf_clock.h:
 *    \code sysclk_init(); \endcode
 *
 * \subsection sysclk_quickstart_use_case_1_example_code Example code
 *   Add or uncomment the following in your conf_clock.h header file, commenting out all other
 *   definitions of the same symbol(s):
 *   \code
 **#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_PLLBCK
 *
 *         // Fpll1 = (Fclk * PLL_mul) / PLL_div
 **#define CONFIG_PLL1_SOURCE          PLLB_SRC_MAINCK_XTAL
 **#define CONFIG_PLL1_MUL             (600000000UL / BOARD_FREQ_MAINCK_XTAL)
 **#define CONFIG_PLL1_DIV0             3
 *
 *         // Fbus = Fsys / BUS_div
 **#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_1
 * \endcode
 *
 * \subsection sysclk_quickstart_use_case_1_example_workflow Workflow
 *  -# Configure the main system clock to use the output of the PLL module as its source:
 *   \code #define CONFIG_SYSCLK_SOURCE          SYSCLK_SRC_PLLBCK \endcode
 *  -# Configure the PLL module to use the external crystal oscillator as its source:
 *   \code #define CONFIG_PLL1_SOURCE            PLLB_SRC_MAINCK_XTAL \endcode
 *  -# Configure the PLL module to multiply the external crystal oscillator frequency up to 200MHz:
 *   \code
 **#define CONFIG_PLL1_MUL             (600000000UL / BOARD_FREQ_MAINCK_XTAL)
 **#define CONFIG_PLL1_DIV0             3
 * \endcode
 *   \note For user boards, \c BOARD_FREQ_MAINCK_XTAL should be defined in the board \c conf_board.h configuration
 *         file as the frequency of the fast crystal attached to the microcontroller.
 *  -# Configure the main clock to run at the full 120MHz, set prescaler to 1:
 *    \code
 **#define CONFIG_SYSCLK_PRES         SYSCLK_PRES_1
 * \endcode
 */

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* / @endcond */

/**
 * \weakgroup sysclk_group
 * @{
 */

/* ! \name Configuration Symbols */
/* @{ */

/**
 * \def CONFIG_SYSCLK_SOURCE
 * \brief Initial/static main system clock source for core 0
 *
 * The main system clock will be configured to use this clock during
 * initialization.
 */
#ifndef CONFIG_SYSCLK_SOURCE
# define CONFIG_SYSCLK_SOURCE   SYSCLK_SRC_MAINCK_4M_RC
#endif

/**
 * \def CONFIG_SYSCLK_PRES
 * \brief Initial CPU clock divider for core 0 (mck)
 *
 * The MCK will run at
 * \f[
 *   f_{MCK} = \frac{f_{sys}}{\mathrm{CONFIG\_SYSCLK\_PRES}}\,\mbox{Hz}
 * \f]
 * after initialization.
 */
#ifndef CONFIG_SYSCLK_PRES
# define CONFIG_SYSCLK_PRES  SYSCLK_PRES_1
#endif

/**
 * \def CONFIG_CPCLK_SOURCE
 * \brief Initial/static main system clock source for core 1
 *
 * The main system clock will be configured to use this clock during
 * initialization.
 */
#ifndef CONFIG_CPCLK_SOURCE
# define CONFIG_CPCLK_SOURCE   CPCLK_SRC_MCK
#endif

/**
 * \def CONFIG_CPCLK_PRES
 * \brief Initial CPU clock divider for core 1 (mck)
 *
 * The MCK will run at
 * \f[
 *   f_{MCK} = \frac{f_{sys}}{\mathrm{CONFIG\_CPCLK\_PRES}}\,\mbox{Hz}
 * \f]
 * after initialization.
 */
#ifndef CONFIG_CPCLK_PRES
# define CONFIG_CPCLK_PRES  1
#endif

/* @} */

/* ! \name Master Clock Sources (MCK) */
/* @{ */
#define SYSCLK_SRC_SLCK_RC              0       /* !< Internal 32kHz RC oscillator as master source clock */
#define SYSCLK_SRC_SLCK_XTAL            1       /* !< External 32kHz crystal oscillator as master source clock */
#define SYSCLK_SRC_SLCK_BYPASS          2       /* !< External 32kHz bypass oscillator as master source clock */
#define SYSCLK_SRC_MAINCK_12M_RC        4       /* !< Internal 12MHz RC oscillator as master source clock */
#define SYSCLK_SRC_MAINCK_XTAL          5       /* !< External crystal oscillator as master source clock */
#define SYSCLK_SRC_MAINCK_BYPASS        6       /* !< External bypass oscillator as master source clock */
#define SYSCLK_SRC_PLLACK1              7       /* !< Use PLLACK1 as master source clock */
#define SYSCLK_SRC_PLLBCK0              8       /* !< Use PLLBCK0 as master source clock */
#define SYSCLK_SRC_PLLCCK0              9       /* !< Use PLLCCK0 as master source clock */
/* @} */

/* ! \name Master Clock Prescalers (MCK) */
/* @{ */
#define SYSCLK_PRES_1           PMC_CPU_CKR_PRES_CLK_1     /* !< Set master clock prescaler to 1 */
#define SYSCLK_PRES_2           PMC_CPU_CKR_PRES_CLK_2     /* !< Set master clock prescaler to 2 */
#define SYSCLK_PRES_4           PMC_CPU_CKR_PRES_CLK_4     /* !< Set master clock prescaler to 4 */
#define SYSCLK_PRES_8           PMC_CPU_CKR_PRES_CLK_8     /* !< Set master clock prescaler to 8 */
#define SYSCLK_PRES_16          PMC_CPU_CKR_PRES_CLK_16    /* !< Set master clock prescaler to 16 */
#define SYSCLK_PRES_32          PMC_CPU_CKR_PRES_CLK_32    /* !< Set master clock prescaler to 32 */
#define SYSCLK_PRES_64          PMC_CPU_CKR_PRES_CLK_64    /* !< Set master clock prescaler to 64 */
#define SYSCLK_PRES_3           PMC_CPU_CKR_PRES_CLK_3     /* !< Set master clock prescaler to 3 */
/* @} */

/* ! \name Coprocessor Master Clock Sources (CPMCK) */
/* @{ */
#define CPCLK_SRC_MDSLCK         0   /* !< MD_SLCK as coprocessor master source clock */
#define CPCLK_SRC_MAINCK         1   /* !< Main Clock as coprocessor master source clock */
#define CPCLK_SRC_MCK0           2   /* !< MCK0 as coprocessor master source clock */
#define CPCLK_SRC_PLLACK1        3   /* !< Use PLLACK1 as coprocessor master source clock */
#define CPCLK_SRC_PLLBCK0        4   /* !< Use PLLBCK0 as coprocessor master source clock */
#define CPCLK_SRC_PLLCCK0        5   /* !< Use PLLCCK0 as coprocessor master source clock */
/* @} */

/* ! \name Coprocessor Master Clock Prescalers (CPMCK) */
/* @{ */
#define CPCLK_PRES_MAX          15    /* !< Maximal value for coprocessor master clock prescaler */
/* @} */

/**
 * \name Querying the system clock
 *
 * The following functions may be used to query the current frequency of
 * the system clock and the CPU and bus clocks derived from it.
 * sysclk_get_main_hz() and sysclk_get_cpu_hz() can be assumed to be
 * available on all platforms, although some platforms may define
 * additional accessors for various chip-internal bus clocks. These are
 * usually not intended to be queried directly by generic code.
 */
/* @{ */
uint32_t sysclk_get_main_hz_core0(void);
#ifdef CONFIG_CPCLK_ENABLE
uint32_t sysclk_get_main_hz_core1(void);
#endif

/**
 * \brief Return the current rate in Hz of the main system clock
 *
 * \todo This function assumes that the main clock source never changes
 * once it's been set up, and that PLL always runs at the compile-time
 * configured default rate. While this is probably the most common
 * configuration, which we want to support as a special case for
 * performance reasons, we will at some point need to support more
 * dynamic setups as well.
 */
static inline uint32_t sysclk_get_main_hz(void)
{
	return sysclk_get_main_hz_core0();
}

/**
 * \brief Return the current rate in Hz of the CPU clock
 *
 * \todo This function assumes that the CPU always runs at the system
 * clock frequency. We want to support at least two more scenarios:
 * Fixed CPU/bus clock dividers (config symbols) and dynamic CPU/bus
 * clock dividers (which may change at run time). Ditto for all the bus
 * clocks.
 *
 * \return Frequency of the CPU clock, in Hz.
 */
static inline uint32_t sysclk_get_cpu_hz(void)
{
	/* CONFIG_SYSCLK_PRES is the register value for setting the expected */
	/* prescaler, not an immediate value. */
	return sysclk_get_main_hz() /
	       ((CONFIG_SYSCLK_PRES == SYSCLK_PRES_3) ? 3 :
	       (1 << (CONFIG_SYSCLK_PRES >> PMC_CPU_CKR_PRES_Pos)));
}

/**
 * \brief Retrieves the current rate in Hz of the peripheral clocks.
 *
 * \return Frequency of the peripheral clocks, in Hz.
 */
static inline uint32_t sysclk_get_peripheral_hz(void)
{
	if (PMC->PMC_CPU_CKR & PMC_CPU_CKR_RATIO_MCK0DIV) {
		return (sysclk_get_cpu_hz() >> 1);
	} else {
		return sysclk_get_cpu_hz();
	}
}

#ifdef CONFIG_CPCLK_ENABLE
/**
 * \brief Retrieves the current rate in Hz of the peripheral clocks.
 *
 * \return Frequency of the peripheral clocks, in Hz.
 */
static inline uint32_t sysclk_get_coproc_peripheral_hz(void)
{
	if (PMC->PMC_CPU_CKR & PMC_CPU_CKR_RATIO_MCK1DIV) {
		return (sysclk_get_main_hz_core1() >> 1);
	} else {
		return sysclk_get_main_hz_core1();
	}
}
#endif

/**
 * \brief Retrieves the current rate in Hz of the Peripheral Bus clock attached
 *        to the specified peripheral.
 *
 * \param module Pointer to the module's base address.
 *
 * \return Frequency of the bus attached to the specified peripheral, in Hz.
 */
static inline uint32_t sysclk_get_peripheral_bus_hz(const volatile void *module)
{
	return sysclk_get_peripheral_hz();
}

/* @} */

/* ! \name Enabling and disabling synchronous clocks */
/* @{ */

/**
 * \brief Enable a peripheral's clock.
 *
 * \param ul_id Id (number) of the peripheral clock.
 */
static inline void sysclk_enable_peripheral_clock(uint32_t ul_id)
{
	pmc_enable_periph_clk(ul_id);
}

/**
 * \brief Disable a peripheral's clock.
 *
 * \param ul_id Id (number) of the peripheral clock.
 */
static inline void sysclk_disable_peripheral_clock(uint32_t ul_id)
{
	pmc_disable_periph_clk(ul_id);
}

/* @} */

/* ! \name System Clock Source and Prescaler configuration */
/* @{ */

extern void sysclk_set_prescalers(uint32_t ul_pres);
extern void sysclk_set_source(uint32_t ul_src);

/* @} */

extern void sysclk_init(void);

/* ! @} */

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* / @endcond */

#endif /* CHIP_SYSCLK_H_INCLUDED */
