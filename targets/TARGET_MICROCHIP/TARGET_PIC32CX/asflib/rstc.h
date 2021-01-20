/**
 * \file
 *
 * \brief SAM Reset Controller (RSTC) driver.
 *
 * Copyright (c) 2011-2020 Microchip Technology Inc. and its subsidiaries.
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

#ifndef RSTC_H_INCLUDED
#define RSTC_H_INCLUDED

#include "pic32cx.h"

/**
 * \defgroup asfdoc_sam_drivers_rstc_group SAM3/4C/4CM/4CP/4E/4N/4S/G/V71/V70/S70/E70 and PIC32CX Reset Controller (RSTC) Driver
 * This driver for Atmel&reg; | SMART ARM&reg;-based microcontrollers
 * provides an interface for the configuration and management of the device's
 * Reset Controller functionality.
 *
 * The Reset Controller manages all Resets of the System including;
 * external devices (via the NRST pin), Processor Reset and Peripheral Reset.
 * It also provides the status of the last reset source.
 *
 * The following peripheral is used by this module:
 *
 * - RSTC (Reset Controller)
 *
 * Devices from the following series can use this module:
 * - Atmel | SMART SAM3
 * - Atmel | SMART SAM4C
 * - Atmel | SMART SAM4CM
 * - Atmel | SMART SAM4CP
 * - Atmel | SMART SAM4E
 * - Atmel | SMART SAM4N
 * - Atmel | SMART SAM4S
 * - Atmel | SMART SAM G
 * - Atmel | SMART SAMV71
 * - Atmel | SMART SAMV70
 * - Atmel | SMART SAME70
 * - Atmel | SMART SAMS70
 * - Atmel | SMART PIC32CX
 *
 * The outline of this documentation is as follows:
 *  - \ref asfdoc_sam_drivers_rstc_prerequisites
 *  - \ref asfdoc_sam_drivers_rstc_module_overview
 *  - \ref asfdoc_sam_drivers_rstc_special_considerations
 *  - \ref asfdoc_sam_drivers_rstc_extra_info
 *  - \ref asfdoc_sam_drivers_rstc_examples
 *  - \ref asfdoc_sam_drivers_rstc_api_overview
 *
 *
 * \section asfdoc_sam_drivers_rstc_prerequisites Prerequisites
 * There are no prerequisites for this module.
 *
 *
 * \section asfdoc_sam_drivers_rstc_module_overview Module Overview
 * The Reset Controller contains an NRST Manager and a Reset State Manager. It runs at
 * Slow Clock (SCLK) and generates the following:
 *
 * - Processor and Watchdog Timer reset
 * - Embedded peripheral reset
 * - Co-processor and Co-processor peripheral reset (SAM4C and PIC32CX devices only)
 * - External device reset (via the NRST pin)
 *
 * These reset signals are asserted by the Reset Controller acting on external events
 * or as the result of an action performed by software. The Reset State Manager controls
 * the operation of reset signals and provides a signal to the NRST Manager when an
 * assertion of the NRST pin is required.
 *
 * \subsection asfdoc_sam_drivers_rstc_module_overview_nrst_manager NRST Manager
 * To control an external device reset the NRST Manager shapes the NRST assertion period
 * using a programmable timer. While asserted, after power-up, NRST is an output and
 * driven low. When the programmable time period has elapsed the pin behaves as an input
 * and all the system is held in reset if NRST is tied to GND by an external signal.
 *
 * \subsubsection asfdoc_sam_drivers_rstc_module_overview_nrst_signal NRST Signal or Interrupt
 * The NRST Manager samples the NRST pin at Slow Clock (SCLK) speed
 * (PIC32CX can be configure to sample asynchronously) and drives this pin
 * low when required by the Reset State Manager. When the line is detected as being low a
 * User Reset is reported to the Reset State Manager. However, the NRST Manager can be
 * programmed not to trigger a reset when NRST is asserted.
 *
 * The Reset controller can also be programmed to generate an interrupt instead of generating
 * a reset.
 *
 * \subsubsection asfdoc_sam_drivers_rstc_module_overview_nrst_external NRST External Reset Control
 * The NRST Manager can assert NRST for a programmable time period of between 60µs and 2s
 * (approximately). This allows the Reset Controller to shape the NRST pin level and thus to
 * guarantee that the NRST line is low for a time that is compliant with any external devices
 * also connected to the system reset.
 *
 * \subsection asfdoc_sam_drivers_rstc_module_overview_reset_state Reset State Manager
 * The Reset State Manager handles the different reset sources and generates the internal reset
 * signals.
 *
 * \subsubsection asfdoc_sam_drivers_rstc_module_overview_reset_state_general General Reset
 * A general reset occurs when either a Power-on-reset is detected or the Supply Controller
 * detects a Brownout or Voltage regulation loss.
 *
 * \subsubsection asfdoc_sam_drivers_rstc_module_overview_reset_state_backup Backup Reset
 * A Backup Reset occurs when the chip returns from Backup Mode. The core_backup_reset signal
 * is asserted by the Supply Controller when a Backup Reset occurs.
 *
 * \subsubsection asfdoc_sam_drivers_rstc_module_overview_reset_state_user User Reset
 * If User Reset is enabled then the state is entered when a low level is detected on the
 * NRST pin. When User Reset is entered both the Processor Reset and the Peripheral Reset
 * are asserted.
 *
 * The User Reset state is left when NRST rises and the processor clock is re-enabled as
 * soon as NRST is confirmed high.
 *
 * \subsubsection asfdoc_sam_drivers_rstc_module_overview_reset_state_software Software Reset
 * The Reset Controller allows software to assert the the following reset signals:
 * - Reset the processor and the Watchdog Timer
 * - Reset all the embedded peripherals
 * - Reset the Co-processor (SAM4C and PIC32CX devices only)
 * - Reset all the embedded peripherals associated with the Co-processor (SAM4C and PIC32CX devices only)
 * - Assert NRST for a programmable time period
 *
 * \note The embedded peripheral reset also includes the memory system and the Remap Command
 * and is generally used for debugging purposes.
 *
 * \note For SAM4C devices the Co-processor peripheral reset only affects the embedded peripherals
 * associated with the Co-processor. The processor's peripherals are not reset.
 *
 * \subsubsection asfdoc_sam_drivers_rstc_module_overview_reset_state_watchdog Watchdog Reset
 * The Watchdog Reset is entered when a watchdog fault occurs.
 *
 * \subsubsection asfdoc_sam_drivers_rstc_module_overview_reset_state_core_sm Core Supply Monitor Reset (PIC32CX device only)
 * A Core Supply Monitor Reset occurs when VDDCORE under voltage is detected.
 *
 * \subsubsection asfdoc_sam_drivers_rstc_module_overview_reset_state_cpu_fail CPU CLK Failure Reset (PIC32CX device only)
 * If CPU clock frequency monitor is enabled and the CPU CLK Failure Reset is also enabled,
 * this state is entered when a CPU clock failure is detected.
 *
 * \subsubsection asfdoc_sam_drivers_rstc_module_overview_reset_state_slck_xtal Slow CLK Crystal Failure Reset (PIC32CX device only)
 * If Slow CLK Crystal Failure Reset is enabled, this state is entered when one
 * of the two available monitors for slow clock crystal detects a failure.
 *
 * \subsubsection asfdoc_sam_drivers_rstc_module_overview_reset_state_porio VDDIO (PORIO) Reset (PIC32CX device only)
 * A PORIO Reset occurs when VDDIO Power-On Reset (PORIO) is detected.
 *
 * \subsubsection asfdoc_sam_drivers_rstc_module_overview_reset_state_priorities Reset State Priorities
 * The Reset State Manager manages the following priorities between the different reset sources,
 * given in descending order:
 * - General Reset
 * - VDDIO (PORIO) Reset (PIC32CX device only)
 * - Core Supply Monitor Reset (PIC32CX device only)
 * - Backup Reset
 * - Slow CLK Crystal Failure Reset (PIC32CX device only)
 * - CPU CLK Failure Reset (PIC32CX device only)
 * - Watchdog Reset
 * - Software Reset
 * - User Reset
 *
 * Particular cases are listed below:
 * - When in User Reset:
 *  - A watchdog event is impossible because the Watchdog Timer is being reset
 *  - A Software Reset is impossible because the processor reset is being activated
 * - When in Software Reset:
 *  - A watchdog event has priority over the current state
 *  - The NRST has no effect
 * - When in Watchdog Reset:
 *  - The processor reset is active and so a Software Reset cannot be programmed
 *  - A User Reset cannot be entered
 *
 * \section asfdoc_sam_drivers_rstc_special_considerations Special Considerations
 * System designs using any external devices that require a software controllable
 * reset signal assertion longer than two seconds should use a dedicated I/O output
 * of the microcontroller.
 *
 * \section asfdoc_sam_drivers_rstc_extra_info Extra Information
 *
 * For extra information, see \ref asfdoc_sam_drivers_rstc_extra. This includes:
 *  - \ref asfdoc_sam_drivers_rstc_extra_acronyms
 *  - \ref asfdoc_sam_drivers_rstc_extra_dependencies
 *  - \ref asfdoc_sam_drivers_rstc_extra_errata
 *  - \ref asfdoc_sam_drivers_rstc_extra_history
 *
 *
 * \section asfdoc_sam_drivers_rstc_examples Examples
 *
 * For a list of examples related to this driver, see
 * \ref asfdoc_sam_drivers_rstc_exqsg.
 *
 *
 * \section asfdoc_sam_drivers_rstc_api_overview API Overview
 *
 * @{
 */

#include <compiler.h>

/* / @cond */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* / @endcond */

/* Definitions of Reset Controller Status. */
/* Reset cause */
#define RSTC_GENERAL_RESET   (0 << RSTC_SR_RSTTYP_Pos)
#define RSTC_BACKUP_RESET    (1 << RSTC_SR_RSTTYP_Pos)
#define RSTC_WATCHDOG_RESET  (2 << RSTC_SR_RSTTYP_Pos)
#define RSTC_SOFTWARE_RESET  (3 << RSTC_SR_RSTTYP_Pos)
#define RSTC_USER_RESET      (4 << RSTC_SR_RSTTYP_Pos)
#if PIC32CX
#define RSTC_CORE_SM_RESET   (5 << RSTC_SR_RSTTYP_Pos)
#define RSTC_CPU_FAIL_RESET  (6 << RSTC_SR_RSTTYP_Pos)
#define RSTC_SLCK_XTAL_RESET (7 << RSTC_SR_RSTTYP_Pos)
#define RSTC_WATCHDOG1_RESET (9 << RSTC_SR_RSTTYP_Pos)
#define RSTC_PORIO_RESET     (10 << RSTC_SR_RSTTYP_Pos)
#endif
/* NRST Pin Level. */
#define RSTC_NRST_LOW   (LOW << 16)
#define RSTC_NRST_HIGH  (HIGH << 16)

void rstc_set_external_reset(Rstc *p_rstc, const uint32_t ul_length);
void rstc_enable_user_reset(Rstc *p_rstc);
void rstc_disable_user_reset(Rstc *p_rstc);
void rstc_enable_user_reset_interrupt(Rstc *p_rstc);
void rstc_disable_user_reset_interrupt(Rstc *p_rstc);
void rstc_start_software_reset(Rstc *p_rstc);
void rstc_reset_extern(Rstc *p_rstc);
uint32_t rstc_get_status(Rstc *p_rstc);
uint32_t rstc_get_reset_cause(Rstc *p_rstc);
void rstc_user_reset_config_sync_detection(Rstc *p_rstc);
void rstc_user_reset_config_async_detection(Rstc *p_rstc);
void rstc_enable_slow_clock_switching(Rstc *p_rstc);
void rstc_disable_slow_clock_switching(Rstc *p_rstc);
void rstc_enable_reset_on_cpu_clock_fail(Rstc *p_rstc);
void rstc_disable_reset_on_cpu_clock_fail(Rstc *p_rstc);
void rstc_enable_pmc_reset_on_software_reset(Rstc *p_rstc);
void rstc_disable_pmc_reset_on_software_reset(Rstc *p_rstc);
void rstc_enable_pmc_reset_on_watchdog_0_reset(Rstc *p_rstc);
void rstc_disable_pmc_reset_on_watchdog_0_reset(Rstc *p_rstc);
void rstc_enable_pmc_reset_on_watchdog_1_reset(Rstc *p_rstc);
void rstc_disable_pmc_reset_on_watchdog_1_reset(Rstc *p_rstc);
void rstc_enable_supply_monitor_interrupt(Rstc *p_rstc);
void rstc_disable_supply_monitor_interrupt(Rstc *p_rstc);
void rstc_enable_reset_on_slow_clock_failure(Rstc *p_rstc);
void rstc_disable_reset_on_slow_clock_failure(Rstc *p_rstc);
void rstc_config_backup_area_supplied_vbat(Rstc *p_rstc);
void rstc_config_backup_area_supplied_vddio(Rstc *p_rstc);

#ifndef RSTC_CPMR_KEY_PASSWD
/** \internal */
#define RSTC_CPMR_KEY_PASSWD  RSTC_CPMR_CPKEY(0x5AU)
#endif /* RSTC_CPMR_KEY_PASSWD */

/**
 * \brief Deassert the reset of the Co-processor.
 *
 * \note This function is for SAM4C and PIC32CX devices only.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 * \param[in] reset  The reset to be deasserted as a bitmask, which could be RSTC_CPMR_CPEREN
 * (peripheral reset) and/or RSTC_CPMR_CPROCEN (core reset).
 */
static inline void rstc_deassert_reset_of_coprocessor(
		Rstc *p_rstc,
		const uint32_t reset)
{
	p_rstc->RSTC_MR |= reset | RSTC_MR_KEY_PASSWD;

}

/**
 * \brief Assert the reset of the Co-processor.
 *
 * \note This function is for SAM4C and PIC32CX devices only.
 *
 * \param[in,out] p_rstc Module hardware register base address pointer
 * \param[in] reset  The reset to be asserted as a bitmask, which could be RSTC_CPMR_CPEREN
 * (peripheral reset) and/or RSTC_CPMR_CPROCEN (core reset).
 */
static inline void rstc_assert_reset_of_coprocessor(
		Rstc *p_rstc,
		const uint32_t reset)
{
	/* Validate parameters. */
	Assert(p_rstc);

	uint32_t tmp = p_rstc->RSTC_MR;
	tmp &= ~(RSTC_MR_KEY_Msk | (reset & (RSTC_MR_CPEREN |
			RSTC_MR_CPROCEN)));
	tmp |= RSTC_MR_KEY_PASSWD;
	p_rstc->RSTC_MR = tmp;
}

/** @} */

/**
 * \page asfdoc_sam_drivers_rstc_extra Extra Information for Reset Controller Driver
 *
 * \section asfdoc_sam_drivers_rstc_extra_acronyms Acronyms
 * Below is a table listing the acronyms used in this module, along with their
 * intended meanings.
 *
 * <table>
 *  <tr>
 *      <th>Acronym</th>
 *      <th>Definition</th>
 *  </tr>
 *  <tr>
 *      <td>GND</td>
 *      <td>Ground</td>
 *  </tr>
 *  <tr>
 *      <td>I/O</td>
 *      <td>Input Output</td>
 *  </tr>
 *  <tr>
 *      <td>NRST</td>
 *      <td>Synchronous Microcontroller Reset</td>
 *  </tr>
 *  <tr>
 *      <td>QSG</td>
 *      <td>Quick Start Guide</td>
 *  </tr>
 *  <tr>
 *      <td>SCLK</td>
 *      <td>Slow Clock</td>
 *  </tr>
 * </table>
 *
 *
 * \section asfdoc_sam_drivers_rstc_extra_dependencies Dependencies
 * This driver has the following dependencies:
 *
 *  - None
 *
 *
 * \section asfdoc_sam_drivers_rstc_extra_errata Errata
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_sam_drivers_rstc_extra_history Module History
 * An overview of the module history is presented in the table below, with
 * details on the enhancements and fixes made to the module since its first
 * release. The current version of this corresponds to the newest version in
 * the table.
 *
 * <table>
 *	<tr>
 *		<th>Changelog</th>
 *	</tr>
 *	<tr>
 *		<td>Initial document release</td>
 *	</tr>
 * </table>
 */

/**
 * \page asfdoc_sam_drivers_rstc_exqsg Examples for Reset Controller Driver
 *
 * This is a list of the available Quick Start Guides (QSGs) and example
 * applications for \ref asfdoc_sam_drivers_rstc_group. QSGs are simple examples with
 * step-by-step instructions to configure and use this driver in a selection of
 * use cases. Note that a QSG can be compiled as a standalone application or be
 * added to the user application.
 *
 *  - \subpage asfdoc_sam_drivers_rstc_quick_start
 *  - \subpage asfdoc_sam_drivers_rstc_example1
 *
 * \page asfdoc_sam_drivers_rstc_document_revision_history Document Revision History
 *
 * <table>
 *	<tr>
 *		<th>Doc. Rev.</td>
 *		<th>Date</td>
 *		<th>Comments</td>
 *	</tr>
 *	<tr>
 *		<td>42279B</td>
 *		<td>07/2015</td>
 *		<td>Updated title of application note and added list of supported devices</td>
 *	</tr>
 *	<tr>
 *		<td>42279A</td>
 *		<td>05/2014</td>
 *		<td>Initial document release</td>
 *	</tr>
 * </table>
 */

/**
 * \page asfdoc_sam_drivers_rstc_quick_start Quick Start guide for SAM RSTC driver
 *
 * This is the quick start guide for the
 * \ref asfdoc_sam_drivers_rstc_group,
 * with step-by-step instructions on how to configure and use the driver
 * in a selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function, while
 * the steps for usage can be copied into, e.g., the main application function.
 *
 * \section asfdoc_sam_drivers_rstc_quick_start_basic_use_case Basic Use Case
 * In this basic use case, the User Reset interrupt is enabled and the main
 * application notified about NRST signal assertion events.
 *
 * \subsection asfdoc_sam_drivers_rstc_quick_start_prereq Prerequisites
 * - \ref sysclk_group "System Clock Management"
 * - \ref asfdoc_sam_drivers_wdt_group "Watchdog Timer"
 *
 * \section asfdoc_sam_drivers_rstc_quick_start_setup Setup Steps
 * \subsection asfdoc_sam_drivers_rstc_quick_start_setup_code Example Code
 * Add the following to the application C-file:
 * \snippet rstc_example1.c reset_irq_variable
 * \snippet rstc_example1.c reset_irq_handler
 *
 * \subsection asfdoc_sam_drivers_rstc_quick_start_setup_flow Workflow
 *
 * Enable the User Reset interrupt:
 *   \snippet rstc_example1.c reset_enable_user_reset_interrupt
 *
 * \section rtc_basic_use_case_usage Usage Steps
 * \subsection rtc_basic_use_case_usage_flow Workflow
 * Add to, e.g., main loop in application C-file:
 * \snippet rstc_example1.c reset_irq_has_triggered
 */

/* / @cond */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* / @endcond */

#endif /* RSTC_H_INCLUDED */
