/**
 * \file
 *
 * \brief Sleep mode access
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

#include <compiler.h>
#include "sleep.h"
#include "sysclk.h"

#include "pmc.h"
#include "supc.h"
#include "board.h"

/* Checking board configuration of main clock xtal statup time */
#if !defined(BOARD_OSC_STARTUP_US)
# warning The board main clock xtal statup time has not been defined. Using default settings.
# define BOARD_OSC_STARTUP_US    (15625UL)
#endif

/** If clocks are switched for some sleep mode */
static volatile bool b_is_sleep_clock_used = false;
/** Callback invoked once when clocks are restored */
static pmc_callback_wakeup_clocks_restored_t callback_clocks_restored = NULL;

void pmc_sleep(int sleep_mode)
{
	switch (sleep_mode) {
	case PIC32CX_PM_SMODE_SLEEP_WFI:
	case PIC32CX_PM_SMODE_SLEEP_WFE:
		SCB->SCR &= (uint32_t) ~SCR_SLEEPDEEP;
		cpu_irq_enable();
		__DSB();
		__WFI();
		break;

	case PIC32CX_PM_SMODE_WAIT_FAST:
	case PIC32CX_PM_SMODE_WAIT:
	{
		cpu_irq_disable();
		b_is_sleep_clock_used = true;

		/* Stop Subsystem 1 */
		pmc_disable_cpck();
		pmc_disable_cpbmck();

		/* Select the 12 MHz fast RC Oscillator as Main Clock for the entire system */
		pmc_switch_mainck_to_fastrc();
		pmc_switch_mck_to_mainck(0);

		/* Disable PLLs */
		pmc_disable_pll(0);
		pmc_disable_pll(1);
		pmc_disable_pll(2);
		
		/* Disable the Main Crystal Oscillator */
		pmc_osc_disable_main_xtal();
		
		/* Set the FLPM field in the PMC Fast Start-up Mode register */
		if (sleep_mode == PIC32CX_PM_SMODE_WAIT_FAST) {
			pmc_set_flash_in_wait_mode(PMC_FSMR_FLPM_FLASH_STANDBY);
		} else {
			pmc_set_flash_in_wait_mode(PMC_FSMR_FLPM_FLASH_DEEP_POWERDOWN);
		}

		/* Set FWS = 0 */
		SEFC0->EEFC_FMR &= (~EEFC_FMR_FWS_Msk);
#if defined(SEFC1)
		SEFC1->EEFC_FMR &= (~EEFC_FMR_FWS_Msk);
#endif

		/* Enter wait mode */
		cpu_irq_enable();
		pmc_enable_waitmode();

		cpu_irq_disable();
		/* Recall SysInit function to restore clock system by default */
		sysclk_init();

		b_is_sleep_clock_used = false;
		if (callback_clocks_restored) {
			callback_clocks_restored();
			callback_clocks_restored = NULL;
		}

		cpu_irq_enable();

		break;
	}

	case PIC32CX_PM_SMODE_BACKUP:
	{
		cpu_irq_disable();

		/* Stop Subsystem 1 */
		pmc_disable_cpck();
		pmc_disable_cpbmck();

		/* Select the 12 MHz fast RC Oscillator as Main Clock for the entire system */
		pmc_switch_mainck_to_fastrc();
		pmc_switch_mck_to_mainck(0);

		/* Disable PLLs */
		pmc_disable_pll(0);
		pmc_disable_pll(1);
		pmc_disable_pll(2);
		
		/* Disable the Main Crystal Oscillator */
		pmc_osc_disable_main_xtal();
		
		/* Enable the RTT in 1 Hz mode */
		if ((RTT->RTT_MR & RTT_MR_RTTDIS) == 0) {
			/* Disable RTT */
			RTT->RTT_MR |= RTT_MR_RTTDIS;
			/* Real-Time Clock 1Hz Clock Selection */
			RTT->RTT_MR &= ~RTT_MR_RTC1HZ;
			/* Enable RTT */
			RTT->RTT_MR &= ~RTT_MR_RTTDIS;
		}
		
		/* Disable the VDDCORE Supply Monitor */
		supc_disable_core_monitor(SUPC);
		
		/* Enter Backup Mode */
		pmc_enable_backupmode();
		break;
	}
	}
}

bool pmc_is_wakeup_clocks_restored(void)
{
	return !b_is_sleep_clock_used;
}

void pmc_wait_wakeup_clocks_restore(pmc_callback_wakeup_clocks_restored_t callback)
{
	if (b_is_sleep_clock_used) {
		cpu_irq_disable();
		callback_clocks_restored = callback;
	} else if (callback) {
		callback();
	}
}
