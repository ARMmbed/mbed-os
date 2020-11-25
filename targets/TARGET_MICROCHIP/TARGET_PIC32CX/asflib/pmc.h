/**
 * \file
 *
 * \brief Power Management Controller (PMC) driver for PIC32CX.
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

#ifndef PMC_H_INCLUDED
#define PMC_H_INCLUDED

#include "compiler.h" 

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

struct _pmc_pll_cfg {
	/** PLL FRACR value */
	uint32_t fracr;
	/** PLL MUL value */
	uint32_t mul;
	/** PLL ID value */
	uint32_t pll_id;
	/** PLL DIVPMC0 value */
	uint32_t div0;
	/** PLL DIVPMC1 value */
	uint32_t div1;
	/** PLL STUPTIM value (number of slow clock cycles before the PLL is locked) */
	uint32_t stuptim;
	/** PLL LOOP_FILTER value */
	uint32_t loop_filter;
	/** PLL Control value */
	uint32_t control;
	/** PLL Source value */
	uint8_t pll_src;
};

/** Bit mask for peripheral clocks (CSR0) */
#define PMC_MASK_PSTATUS0        (0xD383FE00)

/** Bit mask for peripheral clocks (CSR1) */
#define PMC_MASK_PSTATUS1        (0x0AAA00FF)

/** Bit mask for peripheral clocks (CSR2) */
#define PMC_MASK_PSTATUS2        (0x0FA3C800)

/** Bit mask for peripheral clocks (CSR3) */
#define PMC_MASK_PSTATUS3        (0x00000001)

/** Bit mask for generic clocks (GCSR0) */
#define PMC_MASK_GSTATUS0        (0x8181FE00)

/** Bit mask for generic clocks (GCSR1) */
#define PMC_MASK_GSTATUS1        (0x00000024)

/** Bit mask for generic clocks (GCSR2) */
#define PMC_MASK_GSTATUS2        (0x06808800)

/** Bit mask for generic clocks (GCSR3) */
#define PMC_MASK_GSTATUS3        (0x00000001)

/** Loop counter timeout value */
#define PMC_TIMEOUT              (2048)

/** Key to unlock CKGR_MOR register */
#ifndef CKGR_MOR_KEY_PASSWD
#define CKGR_MOR_KEY_PASSWD    CKGR_MOR_KEY(0x37U)
#endif

/** Key used to write SUPC CR register */
#ifndef SUPC_CR_KEY_PASSWD
#define SUPC_CR_KEY_PASSWD     SUPC_CR_KEY(0xA5U)
#endif

/** Key used to write SUPC MR register */
#ifndef SUPC_MR_KEY_PASSWD
#define SUPC_MR_KEY_PASSWD     SUPC_MR_KEY(0xA5U)
#endif

/** Key used to write PMC SCER register */
#ifndef PMC_SCER_CPKEY_PASSWD
#define PMC_SCER_CPKEY_PASSWD    PMC_SCER_CPKEY(0xAU)
#endif

/** Key used to write PMC SDER register */
#ifndef PMC_SCDR_CPKEY_PASSWD
#define PMC_SCDR_CPKEY_PASSWD    PMC_SCDR_CPKEY(0xAU)
#endif

/** Using external oscillator */
#define PMC_OSC_XTAL            0

/** Oscillator in bypass mode */
#define PMC_OSC_BYPASS          1

#define PMC_PCK_0               0 /* PCK0 ID */
#define PMC_PCK_1               1 /* PCK1 ID */
#define PMC_PCK_2               2 /* PCK2 ID */
#define PMC_PCK_3               3 /* PCK3 ID */

/** Flash state in Wait Mode */
#define PMC_WAIT_MODE_FLASH_STANDBY         PMC_FSMR_FLPM_FLASH_STANDBY
#define PMC_WAIT_MODE_FLASH_DEEP_POWERDOWN  PMC_FSMR_FLPM_FLASH_DEEP_POWERDOWN
#define PMC_WAIT_MODE_FLASH_IDLE            PMC_FSMR_FLPM_FLASH_IDLE

/** Convert startup time from us to MOSCXTST */
#define pmc_us_to_moscxtst(startup_us, slowck_freq) \
	((startup_us * slowck_freq / 8 / 1000000) < 0x100 ? \
	(startup_us * slowck_freq / 8 / 1000000) : 0xFF)

/**
 * \name Master clock (MCK) Source and Prescaler configuration
 *
 * \note The following functions may be used to select the clock source and
 * prescaler for the master clock.
 */
/* @{ */

void pmc_mck_set_prescaler(uint32_t ul_pres);
void pmc_mck_set_source(uint32_t ul_source);
uint32_t pmc_switch_mck_to_sclk(uint32_t ul_pres);
uint32_t pmc_switch_mck_to_mainck(uint32_t ul_pres);
uint32_t pmc_switch_mck_to_pllack(uint32_t ul_pres);
uint32_t pmc_switch_mck_to_pllbck(uint32_t ul_pres);

/* @} */

/**
 * \name Slow clock (SLCK) oscillator and configuration
 *
 */
/* @{ */

void pmc_switch_sclk_to_32kxtal(uint32_t ul_bypass);
uint32_t pmc_osc_is_ready_32kxtal(void);

/* @} */

/**
 * \name Main Clock (MAINCK) oscillator and configuration
 *
 */
/* @{ */

void pmc_switch_mainck_to_fastrc(void);
void pmc_osc_enable_fastrc(void);
void pmc_osc_disable_fastrc(void);
uint32_t pmc_osc_is_ready_fastrc(void);

void pmc_osc_enable_main_xtal(uint32_t ul_xtal_startup_time);
void pmc_osc_bypass_main_xtal(void);
void pmc_osc_disable_main_xtal(void);
uint32_t pmc_osc_is_bypassed_main_xtal(void);
uint32_t pmc_osc_is_ready_main_xtal(void);

void pmc_switch_mainck_to_xtal(uint32_t ul_bypass, uint32_t ul_xtal_startup_time);
void pmc_osc_disable_xtal(uint32_t ul_bypass);
uint32_t pmc_osc_is_ready_mainck(void);
void pmc_mainck_osc_select(uint32_t ul_xtal_rc);
uint32_t pmc_is_mck0div_enabled(void);
void pmc_enable_mck0div(void);
void pmc_disable_mck0div(void);
uint32_t pmc_is_mck0div2_enabled(void);
void pmc_enable_mck0div2(void);
void pmc_disable_mck0div2(void);

/* @} */

/**
 * \name PLL oscillator and configuration
 *
 */
/* @{ */
void pmc_enable_pll(const struct _pmc_pll_cfg *pll_cfg);
void pmc_disable_pll(uint32_t ul_pll_id);
void pmc_get_pll_config(uint32_t ul_pll_id, struct _pmc_pll_cfg *pll_cfg);
uint32_t pmc_is_locked_pll(uint32_t ul_pll_id);

/* @} */

/**
 * \name Peripherals clock configuration
 *
 */
/* @{ */
uint32_t pmc_configure_peripheral(uint32_t ul_id, uint32_t ul_div);
uint32_t pmc_enable_periph_clk(uint32_t ul_id);
uint32_t pmc_disable_periph_clk(uint32_t ul_id);
uint32_t pmc_is_periph_clk_enabled(uint32_t ul_id);

/* @} */

/**
 * \name Generic clock configuration
 *
 */
/* @{ */
uint32_t pmc_configure_generic(uint32_t ul_id, uint32_t ul_clock_source, uint32_t ul_div);
uint32_t pmc_enable_generic_clk(uint32_t ul_id);
uint32_t pmc_disable_generic_clk(uint32_t ul_id);
uint32_t pmc_is_generic_clk_enabled(uint32_t ul_id);

/* @} */

/**
 * \name Programmable clock Source and Prescaler configuration
 *
 * The following functions may be used to select the clock source and
 * prescaler for the specified programmable clock.
 */
/* @{ */

void pmc_pck_set_prescaler(uint32_t ul_id, uint32_t ul_pres);
void pmc_pck_set_source(uint32_t ul_id, uint32_t ul_source);
uint32_t pmc_switch_pck_to_md_sclk(uint32_t ul_id, uint32_t ul_pres);
uint32_t pmc_switch_pck_to_td_sclk(uint32_t ul_id, uint32_t ul_pres);
uint32_t pmc_switch_pck_to_mainck(uint32_t ul_id, uint32_t ul_pres);
uint32_t pmc_switch_pck_to_mck0(uint32_t ul_id, uint32_t ul_pres);
uint32_t pmc_switch_pck_to_pllack1(uint32_t ul_id, uint32_t ul_pres);
uint32_t pmc_switch_pck_to_pllbck0(uint32_t ul_id, uint32_t ul_pres);
uint32_t pmc_switch_pck_to_pllcck0(uint32_t ul_id, uint32_t ul_pres);
uint32_t pmc_switch_pck_to_pllcsrc(uint32_t ul_id, uint32_t ul_pres);
void pmc_enable_pck(uint32_t ul_id);
void pmc_disable_pck(uint32_t ul_id);
void pmc_enable_all_pck(void);
void pmc_disable_all_pck(void);
uint32_t pmc_is_pck_enabled(uint32_t ul_id);

/* @} */

/**
 * \name Coprocessor management
 *
 */
/* @{ */
void pmc_enable_cpck(void);
void pmc_disable_cpck(void);
bool pmc_is_cpck_enabled(void);
void pmc_enable_cpbmck(void);
void pmc_disable_cpbmck(void);
bool pmc_is_cpbmck_enabled(void);
void pmc_cpck_set_prescaler(uint32_t ul_pres);
void pmc_cpck_set_source(uint32_t ul_source);
uint32_t pmc_switch_cpck_to_sclk(uint32_t ul_pres);
uint32_t pmc_switch_cpck_to_mainck(uint32_t ul_pres);
uint32_t pmc_switch_cpck_to_pllack(uint32_t ul_pres);
uint32_t pmc_switch_cpck_to_pllbck(uint32_t ul_pres);
uint32_t pmc_switch_cpck_to_pllcck(uint32_t ul_pres);
uint32_t pmc_is_mck1div_enabled(void);
void pmc_enable_mck1div(void);
void pmc_disable_mck1div(void);
/* @} */

/**
 * \name Interrupt and status management
 *
 */
/* @{ */

void pmc_enable_interrupt(uint32_t ul_sources);
void pmc_disable_interrupt(uint32_t ul_sources);
uint32_t pmc_get_interrupt_mask(void);
uint32_t pmc_get_status(void);

/* @} */

/**
 * \name Power management
 *
 * The following functions are used to configure sleep mode and additional
 * wake up inputs.
 */
/* @{ */

void pmc_set_fast_startup_input(uint32_t ul_inputs);
void pmc_clr_fast_startup_input(uint32_t ul_inputs);
void pmc_set_wakeup_input(uint32_t ul_inputs, bool b_polarity);
void pmc_clr_wakeup_input(uint32_t ul_inputs);
void pmc_cp_set_wakeup_input(uint32_t ul_inputs, bool b_polarity);
void pmc_cp_clr_wakeup_input(uint32_t ul_inputs);
void pmc_clear_fault_output(void);
void pmc_cp_set_fast_startup_input(uint32_t ul_inputs);
void pmc_cp_clr_fast_startup_input(uint32_t ul_inputs);
void pmc_enable_sleepmode(uint8_t uc_type);
void pmc_enable_waitmode(void);
void pmc_enable_backupmode(void);
void pmc_set_flash_in_wait_mode(uint32_t ul_flash_state);
void pmc_force_flash_in_wait_mode(uint32_t ul_flash_state);
//@}

/* @} */

/**
 * \name Failure detector
 *
 */
/* @{ */

void pmc_enable_clock_failure_detector(void);
void pmc_disable_clock_failure_detector(void);
void pmc_clear_clock_failure_detector(void);

/* @} */

/**
 * \name Slow Crystal Oscillator Frequency Monitoring
 *
 */
/* @{ */

void pmc_enable_sclk_osc_freq_monitor(void);
void pmc_disable_sclk_osc_freq_monitor(void);

/* @} */

/**
 * \name Write protection
 *
 */
/* @{ */

void pmc_set_writeprotect(uint32_t ul_enable);
uint32_t pmc_get_writeprotect_status(void);

/* @} */

/**
 * \name Sleepwalking configuration
 *
 */
/* @{ */

uint32_t pmc_enable_sleepwalking(uint32_t ul_id);
uint32_t pmc_disable_sleepwalking(uint32_t ul_id);
uint32_t pmc_get_sleepwalking_status(uint32_t ul_id);
uint32_t pmc_get_active_status(uint32_t ul_id);
/* @} */

/**
 * \name MCK0 Monitoring
 *
 */
/* @{ */

void pmc_enable_mck_monitor(uint32_t ul_reset_en);
void pmc_disable_mck_monitor(void);
void pmc_set_mck_monitor_limit_reset(uint32_t ul_high_limit, uint32_t ul_low_limit);
void pmc_set_mck_monitor_limit_interrupt(uint32_t ul_high_limit, uint32_t ul_low_limit);

/* @} */

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

//! @}

/**
 * \page sam_pmc_quickstart Quick start guide for the SAM PMC module
 *
 * This is the quick start guide for the \ref sam_drivers_pmc_group "PMC module",
 * with step-by-step instructions on how to configure and use the driver in a
 * selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function, while
 * the steps for usage can be copied into, e.g., the main application function.
 *
 * \section pmc_use_cases PMC use cases
 * - \ref pmc_basic_use_case Basic use case - Switch Main Clock sources
 * - \ref pmc_use_case_2 Advanced use case - Configure Programmable Clocks
 *
 * \section pmc_basic_use_case Basic use case - Switch Main Clock sources
 * In this use case, the PMC module is configured for a variety of system clock
 * sources and speeds. A LED is used to visually indicate the current clock
 * speed as the source is switched.
 *
 * \section pmc_basic_use_case_setup Setup
 *
 * \subsection pmc_basic_use_case_setup_prereq Prerequisites
 * -# \ref gpio_group "General Purpose I/O Management (gpio)"
 *
 * \subsection pmc_basic_use_case_setup_code Code
 * The following function needs to be added to the user application, to flash a
 * board LED a variable number of times at a rate given in CPU ticks.
 *
 * \code
	 #define FLASH_TICK_COUNT   0x00012345

	 void flash_led(uint32_t tick_count, uint8_t flash_count)
	 {
	     SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
	     SysTick->LOAD = tick_count;

	     while (flash_count--)
	     {
	         gpio_toggle_pin(LED0_GPIO);
	         while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
	         gpio_toggle_pin(LED0_GPIO);
	         while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
	     }
	 }
\endcode
 *
 * \section pmc_basic_use_case_usage Use case
 *
 * \subsection pmc_basic_use_case_usage_code Example code
 * Add to application C-file:
 * \code
	for (;;)
	{
	    pmc_switch_mainck_to_fastrc();
	    flash_led(FLASH_TICK_COUNT, 5);
	    pmc_switch_mainck_to_xtal(0);
	    flash_led(FLASH_TICK_COUNT, 5);
	}
\endcode
 *
 * \subsection pmc_basic_use_case_usage_flow Workflow
 * -# Wrap the code in an infinite loop:
 *   \code
	for (;;)
\endcode
 * -# Switch the Master CPU frequency to the internal 12MHz RC oscillator, flash
 *    a LED on the board several times:
 *   \code
	pmc_switch_mainck_to_fastrc();
	flash_led(FLASH_TICK_COUNT, 5);
\endcode
 * -# Switch the Master CPU frequency to the external crystal oscillator, flash
 *    a LED on the board several times:
 *   \code
	pmc_switch_mainck_to_xtal(0, BOARD_OSC_STARTUP_US);
	flash_led(FLASH_TICK_COUNT, 5);
\endcode
 *
 * \section pmc_use_case_2 Use case #2 - Configure Programmable Clocks
 * In this use case, the PMC module is configured to start the Slow Clock from
 * an attached 32KHz crystal, and start one of the Programmable Clock modules
 * sourced from the Slow Clock divided down with a prescale factor of 64.
 *
 * \section pmc_use_case_2_setup Setup
 *
 * \subsection pmc_use_case_2_setup_prereq Prerequisites
 * -# \ref pio_group "Parallel Input/Output Controller (pio)"
 *
 * \subsection pmc_use_case_2_setup_code Code
 * The following code must be added to the user application:
 * \code
	pio_set_peripheral(PIOA, PIO_PERIPH_B, PIO_PA17);
\endcode
 *
 * \subsection pmc_use_case_2_setup_code_workflow Workflow
 * -# Configure the PCK1 pin to output on a specific port pin (in this case,
 *    PIOA pin 17) of the microcontroller.
 *   \code
	pio_set_peripheral(PIOA, PIO_PERIPH_B, PIO_PA17);
\endcode
 *   \note The peripheral selection and pin will vary according to your selected
 *       SAM device model. Refer to the "Peripheral Signal Multiplexing on I/O
 *       Lines" of your device's datasheet.
 *
 * \section pmc_use_case_2_usage Use case
 * The generated PCK1 clock output can be viewed on an oscilloscope attached to
 * the correct pin of the microcontroller.
 *
 * \subsection pmc_use_case_2_usage_code Example code
 * Add to application C-file:
 * \code
	  pmc_switch_sclk_to_32kxtal(PMC_OSC_XTAL);
	  pmc_switch_pck_to_sclk(PMC_PCK_1, PMC_PCK_PRES_CLK_64);
	  pmc_enable_pck(PMC_PCK_1);

	  for (;;)
	  {
	      // Do Nothing
	  }
\endcode
 *
 * \subsection pmc_use_case_2_usage_flow Workflow
 * -# Switch the Slow Clock source input to an external 32KHz crystal:
 *   \code
	pmc_switch_sclk_to_32kxtal(PMC_OSC_XTAL);
\endcode
 * -# Switch the Programmable Clock module PCK1 source clock to the Slow Clock,
 *    with a prescaler of 64:
 *   \code
	pmc_switch_pck_to_sclk(PMC_PCK_1, PMC_PCK_PRES_CLK_64);
\endcode
 * -# Enable Programmable Clock module PCK1:
 *   \code
	pmc_enable_pck(PMC_PCK_1);
\endcode
 * -# Enter an infinite loop:
 *   \code
	for (;;)
	{
	   // Do Nothing
	}
\endcode
 */

#endif /* PMC_H_INCLUDED */
