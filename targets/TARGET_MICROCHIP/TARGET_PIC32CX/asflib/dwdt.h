/**
 * \file
 *
 * \brief Dual Watchdog Timer (DWDT) driver for PIC32CX.
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

#ifndef DWDT_H_INCLUDED
#define DWDT_H_INCLUDED

#include "compiler.h"
#include "pic32cx.h"

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* / @endcond */

typedef enum dwdt_id {
	WDT0_ID = 0,
	WDT1_ID = 1
} dwdt_id_t;

typedef struct dwdt_cfg {
	/** Slow Clock */
	uint32_t ul_slck;
	/** DWDT Watchdog Mode */
	uint32_t ul_mode;
	/** DWDT Prescaler Ratio */
	uint32_t ul_prescaler;
	/** DWDT Watchdog Time in ms (Period) */
	uint32_t ul_time;
	/** DWDT Repeat Threshold */
	uint32_t ul_rep_time;
	/** DWDT Level Threshold */
	uint32_t ul_intlvl_time;
} dwdt_cfg_t;

#define DWDT_INVALID_ARGUMENT 0xFFFF

uint32_t dwdt_get_timeout_value(uint32_t ul_us, uint32_t ul_sclk, uint32_t uc_presc);
uint32_t dwdt_init(Dwdt *p_dwdt, dwdt_id_t dwdt_id, dwdt_cfg_t *p_cfg);
void dwdt_disable(Dwdt *p_dwdt, dwdt_id_t dwdt_id);
void dwdt_restart(Dwdt *p_dwdt, dwdt_id_t dwdt_id);
uint32_t dwdt_get_value(Dwdt *p_dwdt, dwdt_id_t dwdt_id);
void dwdt_enable_interrupt(Dwdt *p_dwdt, dwdt_id_t dwdt_id, const uint32_t ul_mask);
void dwdt_disable_interrupt(Dwdt *p_dwdt, dwdt_id_t dwdt_id, const uint32_t ul_mask);
uint32_t dwdt_get_interrupt_status(Dwdt *p_dwdt, dwdt_id_t dwdt_id);
uint32_t dwdt_get_interrupt_mask(Dwdt *p_dwdt, dwdt_id_t dwdt_id);
uint32_t dwdt_get_us_timeout_period(Dwdt *p_dwdt, dwdt_id_t dwdt_id, uint32_t ul_sclk);

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* / @endcond */

#endif /* DWDT_H_INCLUDED */
