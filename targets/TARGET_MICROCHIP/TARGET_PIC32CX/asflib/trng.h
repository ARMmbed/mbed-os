/**
 * \file
 *
 * \brief API for SAM TRNG.
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

#ifndef TRNG_H_INCLUDED
#define TRNG_H_INCLUDED

#include "compiler.h"
#include "pic32cx.h"

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* / @endcond */

#define TRNG_CR_KEY_Pos  TRNG_CR_WAKEY_Pos
#define TRNG_CR_KEY_Msk  TRNG_CR_WAKEY_Msk
#define TRNG_CR_KEY(value)  TRNG_CR_WAKEY(value)
#define TRNG_CR_KEY_PASSWD  TRNG_CR_WAKEY_PASSWD

/** Key to unlock TRNG_CR register */
#ifndef TRNG_CR_KEY_PASSWD
#define TRNG_CR_KEY_PASSWD    TRNG_CR_KEY(0x524E47)
#endif

enum trng_private_key_bus_slave {
	TRNG_KEY_BUS_SLAVE_AES = TRNG_PKBCR_KSLAVE_AES_ID,
	TRNG_KEY_BUS_SLAVE_AESB = TRNG_PKBCR_KSLAVE_AESB_ID,
};

void trng_enable(Trng *p_trng, uint32_t ul_mck);
void trng_enable_interrupt(Trng *p_trng, uint32_t ul_sources);
void trng_disable_interrupt(Trng *p_trng, uint32_t ul_sources);
void trng_disable(Trng *p_trng);
uint32_t trng_get_interrupt_mask(Trng *p_trng);
uint32_t trng_get_interrupt_status(Trng *p_trng);
uint32_t trng_read_output_data(Trng *p_trng);
void trng_trigger_private_key_bus_transfer(Trng *p_trng, uint32_t ul_key_len,
		enum trng_private_key_bus_slave e_key_slave);
void trng_enable_writeprotect_mode_reg(Trng *p_trng);
void trng_enable_writeprotect_int_regs(Trng *p_trng);
void trng_enable_writeprotect_ctl_regs(Trng *p_trng);
void trng_enable_writeprotect_all_regs(Trng *p_trng);
void trng_disable_writeprotect_all_regs(Trng *p_trng);
void trng_config_protect_violation_report(Trng *p_trng, bool b_keep_first);
uint32_t trng_get_write_protection_status(Trng *p_trng);

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* / @endcond */

#endif /* TRNG_H_INCLUDED */
