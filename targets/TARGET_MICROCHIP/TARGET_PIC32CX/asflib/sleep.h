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

#ifndef SLEEP_H
#define SLEEP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <compiler.h>

/**
 * \defgroup sleep_group Power Manager (PM)
 *
 * This is a stub on the PIC32CX Power Manager Control (PMC) for the sleepmgr
 * service.
 *
 * \note To minimize the code overhead, these functions do not feature
 * interrupt-protected access since they are likely to be called inside
 * interrupt handlers or in applications where such protection is not
 * necessary. If such protection is needed, it must be ensured by the calling
 * code.
 *
 * @{
 */

#if defined(__DOXYGEN__)
/**
 * \brief Sets the MCU in the specified sleep mode
 * \param sleep_mode Sleep mode to set.
 */
#endif
# define  PIC32CX_PM_SMODE_ACTIVE     0 /**< Active */
# define  PIC32CX_PM_SMODE_SLEEP_WFE  1 /**< Wait for Events */
# define  PIC32CX_PM_SMODE_SLEEP_WFI  2 /**< Wait for Interrupts */
# define  PIC32CX_PM_SMODE_WAIT_FAST  3 /**< Wait Mode, startup fast (in 3ms) */
# define  PIC32CX_PM_SMODE_WAIT       4 /**< Wait Mode */
# define  PIC32CX_PM_SMODE_BACKUP     5 /**< Backup Mode */

/** (SCR) Sleep deep bit */
#define SCR_SLEEPDEEP   (0x1 <<  2)

struct _pll_settings {
	/** CONTROL0 settings */
	uint32_t ctrl0;
	/** CONTROL1 settings */
	uint32_t ctrl1;
	/** CONTROL2 settings */
	uint32_t ctrl2;
	/** SSR settings */
	uint32_t ssr;
};

struct _clock_settings {
	/** OSC settings */
	uint32_t osc_settings;
	/** PLL settings */
	struct _pll_settings pll_settings[3];
	/** PLLs Start Up Time */
	uint32_t plls_stuptim;
	/** PLLs Lock bits */
	uint32_t plls_lock;
	/** CPU CKR settings */
	uint32_t cpu_ckr_settings;
	/** FMR0 settings */
	uint32_t fmr0_settings;
	/** FMR1 settings */
	uint32_t fmr1_settings;
};

/**
 * Clocks restored callback function type.
 * Registered by routine pmc_wait_wakeup_clocks_restore()
 * Callback called when all clocks are restored.
 */
typedef void (*pmc_callback_wakeup_clocks_restored_t) (void);

/**
 * Enter sleep mode
 * \param sleep_mode Sleep mode to enter
 */
void pmc_sleep(int sleep_mode);

/**
 * Check if clocks are restored after wakeup
 * (For WAIT mode. In WAIT mode, clocks are switched to FASTRC.
 * After wakeup clocks should be restored, before that some of the
 * ISR should not be served, otherwise there may be timing or clock issue.)
 */
bool pmc_is_wakeup_clocks_restored(void);

/**
 * \return true if start waiting
 */
void pmc_wait_wakeup_clocks_restore(
		pmc_callback_wakeup_clocks_restored_t callback);

//! @}

#ifdef __cplusplus
}
#endif

#endif /* SLEEP_H */
