/**
 * \file
 *
 * \brief Supply Controller (SUPC) driver for SAM.
 *
 * Copyright (c) 2011-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef SUPC_H_INCLUDED
#define SUPC_H_INCLUDED

#include "compiler.h"

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

#if (SAM4C || SAM4CP || SAM4CM)
    /** Power Mode */
enum slcdc_power_mode {
    /** The internal supply source and the external supply source are both deselected. */
    SLCDC_POWER_MODE_LCDOFF = SUPC_MR_LCDMODE_LCDOFF,
    /** The external supply source for LCD is selected */
    SLCDC_POWER_MODE_LCDON_EXTVR = SUPC_MR_LCDMODE_LCDON_EXTVR,
    /** The internal supply source for LCD is selected */
    SLCDC_POWER_MODE_LCDON_INVR = SUPC_MR_LCDMODE_LCDON_INVR,
};
#endif

#if (!SAMG)
void supc_enable_backup_mode(Supc *p_supc);
void supc_enable_voltage_regulator(Supc *p_supc);
void supc_disable_voltage_regulator(Supc *p_supc);
#endif
void supc_switch_sclk_to_32kxtal(Supc *p_supc, uint32_t ul_bypass);
void supc_enable_brownout_detector(Supc *p_supc);
void supc_disable_brownout_detector(Supc *p_supc);
void supc_enable_brownout_reset(Supc *p_supc);
void supc_disable_brownout_reset(Supc *p_supc);
void supc_set_monitor_threshold(Supc *p_supc, uint32_t ul_threshold);
void supc_set_monitor_sampling_period(Supc *p_supc, uint32_t ul_period);
void supc_enable_monitor_reset(Supc *p_supc);
void supc_disable_monitor_reset(Supc *p_supc);
void supc_enable_monitor_interrupt(Supc *p_supc);
void supc_disable_monitor_interrupt(Supc *p_supc);
#if (!(SAMG51 || SAMG53 || SAMG54))
void supc_set_wakeup_mode(Supc *p_supc, uint32_t ul_mode);
void supc_set_wakeup_inputs(Supc *p_supc, uint32_t ul_inputs,
                            uint32_t ul_transition);
#endif
uint32_t supc_get_status(Supc *p_supc);
#if (SAM4C || SAM4CP || SAM4CM)
void supc_enable_backup_power_on_reset(Supc *p_supc);
void supc_disable_backup_power_on_reset(Supc *p_supc);
enum slcdc_power_mode supc_get_slcd_power_mode(Supc *p_supc);
void supc_set_slcd_power_mode(Supc *p_supc, enum slcdc_power_mode mode);
void supc_set_slcd_vol(Supc *p_supc, uint32_t vol);
#endif
#if SAMG54
void supc_set_regulator_trim_factory(Supc *p_supc);
void supc_set_regulator_trim_user(Supc *p_supc, uint32_t value);
#endif
#if (SAMV70 || SAMV71 || SAME70 || SAMS70)
void supc_backup_sram_on(Supc *p_supc);
void supc_backup_sram_off(Supc *p_supc);
#endif

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

#endif /* SUPC_H_INCLUDED */
