/**
 * \file
 *
 * \brief Supply Controller (SUPC) driver for PIC32CX.
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

#ifndef SUPC_H_INCLUDED
#define SUPC_H_INCLUDED

#include "compiler.h"
#include "pic32cx.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/** Key used to write SUPC registers */
#ifndef SUPC_CR_KEY_PASSWD
#define SUPC_CR_KEY_PASSWD    SUPC_CR_KEY(0xA5U)
#endif

#ifndef SUPC_MR_KEY_PASSWD
#define SUPC_MR_KEY_PASSWD    SUPC_MR_KEY(0xA5U)
#endif

/** Power Mode */
enum slcdc_power_mode {
	/** The internal supply source and the external supply source are both deselected. */
	SLCDC_POWER_MODE_LCDOFF = SUPC_MR_LCDMODE_LCDOFF,
	/** The external supply source for LCD is selected */
	SLCDC_POWER_MODE_LCDON_EXTVR = SUPC_MR_LCDMODE_LCDON_EXTVR,
	/** The internal supply source for LCD is selected */
	SLCDC_POWER_MODE_LCDON_INVR = SUPC_MR_LCDMODE_LCDON_INVR,
};

typedef enum {
	SUPC_SM_THRESHOLD_2v40 = 0,
	SUPC_SM_THRESHOLD_2v47,
	SUPC_SM_THRESHOLD_2v54,
	SUPC_SM_THRESHOLD_2v61,
	SUPC_SM_THRESHOLD_2v68,
	SUPC_SM_THRESHOLD_2v75,
	SUPC_SM_THRESHOLD_2v82,
	SUPC_SM_THRESHOLD_2v89,
	SUPC_SM_THRESHOLD_2v96,
	SUPC_SM_THRESHOLD_3v03,
	SUPC_SM_THRESHOLD_3v10,
	SUPC_SM_THRESHOLD_3v17,
	SUPC_SM_THRESHOLD_3v24,
	SUPC_SM_THRESHOLD_3v31,
	SUPC_SM_THRESHOLD_3v38,
	SUPC_SM_THRESHOLD_3v45
} supc_sm_thrshld_level_t;

void supc_enable_backup_mode(Supc *p_supc);
void supc_enable_shutdown(Supc *p_supc, uint32_t ul_eof);
void supc_switch_sclk_to_32kxtal(Supc *p_supc, uint32_t ul_bypass, uint32_t ul_stup);
void supc_set_monitor_threshold(Supc *p_supc, supc_sm_thrshld_level_t ul_threshold);
void supc_set_monitor_sampling_period(Supc *p_supc, uint32_t ul_period);
void supc_enable_monitor_reset(Supc *p_supc);
void supc_disable_monitor_reset(Supc *p_supc);
void supc_set_monitor_power_mode(Supc *p_supc, uint32_t ul_mode);
void supc_enable_monitor_interrupt(Supc *p_supc);
void supc_disable_monitor_interrupt(Supc *p_supc);
uint32_t supc_get_interrupt_status(Supc *p_supc);
void supc_set_slcd_vol(Supc *p_supc, uint32_t vol);
enum slcdc_power_mode supc_get_slcd_power_mode(Supc *p_supc);
void supc_set_slcd_power_mode(Supc *p_supc, enum slcdc_power_mode mode);
void supc_set_core_monitor_output_mode(Supc *p_supc, uint32_t ul_mode);
void supc_enable_voltage_regulator(Supc *p_supc);
void supc_disable_voltage_regulator(Supc *p_supc);
void supc_enable_core_monitor_reset(Supc *p_supc);
void supc_disable_core_monitor_reset(Supc *p_supc);
void supc_enable_core_monitor(Supc *p_supc);
void supc_disable_core_monitor(Supc *p_supc);
void supc_enable_monitor(Supc *p_supc);
void supc_disable_monitor(Supc *p_supc);
void supc_set_wakeup_mode(Supc *p_supc, uint32_t ul_mode);
void supc_set_wakeup_inputs(Supc *p_supc, uint32_t ul_inputs, uint32_t ul_transition);
uint32_t supc_get_status(Supc *p_supc);
bool supc_monitor_voltage_is_invalid(Supc *p_supc);
void supc_enable_flash_erase_gpbr(Supc *p_supc);
void supc_disable_flash_erase_gpbr(Supc *p_supc);
void supc_enable_full_gpbr_clean(Supc *p_supc);
void supc_enable_half_gpbr_clean(Supc *p_supc);
void supc_enable_voltage_regulator_bandgap(Supc *p_supc);
void supc_disable_voltage_regulator_bandgap(Supc *p_supc);
void supc_set_backup_mode(Supc *p_supc, uint32_t ul_mode);

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

#endif /* SUPC_H_INCLUDED */
