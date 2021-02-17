/**
 * \file
 *
 * \brief Matrix driver for PIC32CX.
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

#include "pic32cx.h"
#include  "matrix.h"

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* / @endcond */

/**
 * \defgroup sam_drivers_matrix_group Matrix (MATRIX)
 *
 * \par Purpose
 *
 * The Bus Matrix implements a multi-layer AHB that enables parallel access
 * paths between multiple AHB masters and slaves in a system, which increases
 * the overall bandwidth.
 *
 * @{
 */

#ifndef MATRIX_WPMR_WPKEY_PASSWD
#define MATRIX_WPMR_WPKEY_PASSWD    MATRIX_WPMR_WPKEY(0x4D4154U)
#endif

/**
 * \brief Get Pointer to Matrix instance from Master peripheral.
 *
 * \param ul_id Master index.
 *
 * \return Pointer to Matrix hardware registers.
 */
static Matrix *_get_matrix_master_periph(master_id_type_t ul_id)
{
	Matrix *p_matrix = 0;
	uint8_t ul_matrix_id;

	ul_matrix_id = ul_id >> 4;

	if (ul_matrix_id == 0) {
		p_matrix = MATRIX0;
	} else if (ul_matrix_id == 1) {
		p_matrix = MATRIX1;
	} else if (ul_matrix_id == 2) {
		p_matrix = MATRIX2;
	} else if (ul_matrix_id == 3) {
		p_matrix = MATRIX3;
	}

	return p_matrix;
}

/**
 * \brief Get Pointer to Matrix instance from Slave peripheral.
 *
 * \param ul_id Slave index.
 *
 * \return Pointer to Slave hardware registers.
 */
static Matrix *_get_matrix_slave_periph(slave_id_type_t ul_id)
{
	Matrix *p_matrix = 0;
	uint8_t ul_matrix_id;

	ul_matrix_id = ul_id >> 4;

	if (ul_matrix_id == 0) {
		p_matrix = MATRIX0;
	} else if (ul_matrix_id == 1) {
		p_matrix = MATRIX1;
	} else if (ul_matrix_id == 2) {
		p_matrix = MATRIX2;
	} else if (ul_matrix_id == 3) {
		p_matrix = MATRIX3;
	}

	return p_matrix;
}

/**
 * \brief Set undefined length burst type of the specified master.
 *
 * \param ul_id Master index.
 * \param burst_type Undefined length burst type.
 */
void matrix_set_master_burst_type(master_id_type_t ul_id, burst_type_t burst_type)
{
	Matrix *p_matrix;
	volatile uint32_t ul_reg;

	p_matrix = _get_matrix_master_periph(ul_id);

	ul_reg = p_matrix->MATRIX_MCFG[ul_id & 0x0F] & (~MATRIX_MCFG_ULBT_Msk);
	p_matrix->MATRIX_MCFG[ul_id & 0x0F] = ul_reg | (uint32_t)burst_type;
}

/**
 * \brief Get undefined length burst type of the specified master.
 *
 * \param ul_id Master index.
 *
 * \return Undefined length burst type.
 */
burst_type_t matrix_get_master_burst_type(master_id_type_t ul_id)
{
	Matrix *p_matrix;
	volatile uint32_t ul_reg;

	p_matrix = _get_matrix_master_periph(ul_id);

	ul_reg = p_matrix->MATRIX_MCFG[ul_id & 0x0F] & (MATRIX_MCFG_ULBT_Msk);
	return (burst_type_t)ul_reg;
}

/**
 * \brief Set slot cycle of the specified slave.
 *
 * \param ul_id Slave index.
 * \param ul_slot_cycle Number of slot cycle.
 */
void matrix_set_slave_slot_cycle(slave_id_type_t ul_id, uint32_t ul_slot_cycle)
{
	Matrix *p_matrix;
	volatile uint32_t ul_reg;

	p_matrix = _get_matrix_slave_periph(ul_id);

	ul_reg = p_matrix->MATRIX_SCFG[ul_id & 0x0F] & (~MATRIX_SCFG_SLOT_CYCLE_Msk);
	p_matrix->MATRIX_SCFG[ul_id & 0x0F] = ul_reg | MATRIX_SCFG_SLOT_CYCLE(ul_slot_cycle);
}

/**
 * \brief Get slot cycle of the specified slave.
 *
 * \param ul_id Slave index.
 *
 * \return Number of slot cycle.
 */
uint32_t matrix_get_slave_slot_cycle(slave_id_type_t ul_id)
{
	Matrix *p_matrix;
	volatile uint32_t ul_reg;

	p_matrix = _get_matrix_slave_periph(ul_id);

	ul_reg = p_matrix->MATRIX_SCFG[ul_id & 0x0F] & (MATRIX_SCFG_SLOT_CYCLE_Msk);
	return (ul_reg >> MATRIX_SCFG_SLOT_CYCLE_Pos);
}

/**
 * \brief Set default master type of the specified slave.
 *
 * \param ul_id Slave index.
 * \param type Default master type.
 */
void matrix_set_slave_default_master_type(slave_id_type_t ul_id, default_master_t type)
{
	Matrix *p_matrix;
	volatile uint32_t ul_reg;

	p_matrix = _get_matrix_slave_periph(ul_id);

	ul_reg = p_matrix->MATRIX_SCFG[ul_id & 0x0F] & (~MATRIX_SCFG_DEFMSTR_TYPE_Msk);
	p_matrix->MATRIX_SCFG[ul_id & 0x0F] = ul_reg | (uint32_t)type;
}

/**
 * \brief Get default master type of the specified slave.
 *
 * \param ul_id Slave index.
 *
 * \return Default master type.
 */
default_master_t matrix_get_slave_default_master_type(slave_id_type_t ul_id)
{
	Matrix *p_matrix;
	volatile uint32_t ul_reg;

	p_matrix = _get_matrix_slave_periph(ul_id);

	ul_reg = p_matrix->MATRIX_SCFG[ul_id & 0x0F] & (MATRIX_SCFG_DEFMSTR_TYPE_Msk);
	return (default_master_t)ul_reg;
}

/**
 * \brief Set fixed default master of the specified slave.
 *
 * \param ul_id Slave index.
 * \param ul_fixed_id Fixed default master index.
 */
void matrix_set_slave_fixed_default_master(slave_id_type_t ul_id, uint32_t ul_fixed_id)
{
	Matrix *p_matrix;
	volatile uint32_t ul_reg;

	p_matrix = _get_matrix_slave_periph(ul_id);

	ul_reg = p_matrix->MATRIX_SCFG[ul_id & 0x0F] &  (~MATRIX_SCFG_FIXED_DEFMSTR_Msk);
	p_matrix->MATRIX_SCFG[ul_id & 0x0F] = ul_reg | MATRIX_SCFG_FIXED_DEFMSTR(ul_fixed_id);
}

/**
 * \brief Get fixed default master of the specified slave.
 *
 * \param ul_id Slave index.
 *
 * \return Fixed default master index.
 */
uint32_t matrix_get_slave_fixed_default_master(slave_id_type_t ul_id)
{
	Matrix *p_matrix;
	volatile uint32_t ul_reg;

	p_matrix = _get_matrix_slave_periph(ul_id);

	ul_reg = p_matrix->MATRIX_SCFG[ul_id & 0x0F] & (MATRIX_SCFG_FIXED_DEFMSTR_Msk);
	return (ul_reg >> MATRIX_SCFG_FIXED_DEFMSTR_Pos);
}

/**
 * \brief Set priority for the specified slave access.
 *
 * \param ul_id Slave index.
 * \param ul_prio Bitmask OR of priorities of master x.
 * \param ul_lqos Bitmask OR of latency Quality of Service Enable for master x.
 */
void matrix_set_slave_priority(slave_id_type_t ul_id, uint32_t ul_prio, uint32_t ul_lqos)
{
	Matrix *p_matrix;

	p_matrix = _get_matrix_slave_periph(ul_id);

	p_matrix->MATRIX_PR[ul_id & 0x0F].MATRIX_PRAS = (ul_lqos | ul_prio);
}

/**
 * \brief Get priority and latency QoS Enable for the specified slave access.
 *
 * \param ul_id Slave index.
 *
 * \return Bitmask OR of priorities and latency QoS Enable for  master x.
 */
uint32_t matrix_get_slave_priority(slave_id_type_t ul_id)
{
	Matrix *p_matrix;

	p_matrix = _get_matrix_slave_periph(ul_id);

	return p_matrix->MATRIX_PR[ul_id & 0x0F].MATRIX_PRAS;
}

/**
 * \brief Set priority for the specified slave access.
 *
 * \param ul_id Slave index.
 * \param ul_prio_b Bitmask OR of priorities of master x.
 * \param ul_lqos_b Bitmask OR of latency Quality of Service Enable for master x.
 */
void matrix_set_slave_priority_b(slave_id_type_t ul_id, uint32_t ul_prio_b, uint32_t ul_lqos_b)
{
	Matrix *p_matrix;

	p_matrix = _get_matrix_slave_periph(ul_id);

	p_matrix->MATRIX_PR[ul_id & 0x0F].MATRIX_PRBS = (ul_lqos_b | ul_prio_b);
}

/**
 * \brief Get priority for the specified slave access.
 *
 * \param ul_id Slave index.
 *
 * \return Bitmask OR of priorities and latency QoS Enable for master x.
 */
uint32_t matrix_get_slave_priority_b(slave_id_type_t ul_id)
{
	Matrix *p_matrix;

	p_matrix = _get_matrix_slave_periph(ul_id);

	return p_matrix->MATRIX_PR[ul_id & 0x0F].MATRIX_PRBS;
}

/**
 * \brief Set bus matrix master remap.
 *
 * \param p_matrix Pointer to a Matrix instance.
 * \param ul_remap Bitmask OR of RCBx: 0 for disable, 1 for enable.
 */
void matrix_set_master_remap(Matrix *p_matrix, uint32_t ul_remap)
{
	p_matrix->MATRIX_MRCR = ul_remap;
}

/**
 * \brief Get bus matrix master remap.
 *
 * \param p_matrix Pointer to a Matrix instance.
 * \return Bitmask OR of RCBx: 0 for disable, 1 for enable.
 */
uint32_t matrix_get_master_remap(Matrix *p_matrix)
{
	return (p_matrix->MATRIX_MRCR);
}

/**
 * \brief Enable the Master Access Error interrupt.
 * The MATRIX must be configured as an NVIC interrupt source as well.
 *
 * \param p_matrix Pointer to a Matrix instance.
 * \param ul_mask Interrupt sources bit map.
 */
void matrix_master_enable_interrupt(Matrix *p_matrix, const uint32_t ul_mask)
{
	p_matrix->MATRIX_MEIER = ul_mask;
}

/**
 * \brief Disable the Master Access Error interrupt.
 *
 * \param p_matrix Pointer to a Matrix instance.
 * \param ul_mask Interrupt sources bit map.
 */
void matrix_master_disable_interrupt(Matrix *p_matrix, const uint32_t ul_mask)
{
	p_matrix->MATRIX_MEIDR = ul_mask;
}

/**
 * \brief Get the Master Access Error interrupt mask.
 *
 * \param p_matrix Pointer to a Matrix instance.
 * \return The interrupt mask value.
 */
uint32_t matrix_master_get_interrupt_mask(Matrix *p_matrix)
{
	return p_matrix->MATRIX_MEIMR;
}

/**
 * \brief Get the Master Error status.
 *
 * \param p_matrix Pointer to a Matrix instance.
 * \return The interrupt status value.
 */
uint32_t matrix_master_get_error_status(Matrix *p_matrix)
{
	return p_matrix->MATRIX_MESR;
}

/**
 * \brief Get the Master Error Address.
 *
 * \param ul_id Master index.
 * \return 32 most significant bits of the last access error address.
 */
uint32_t matrix_master_get_error_address(master_id_type_t ul_id)
{
	Matrix *p_matrix;

	p_matrix = _get_matrix_master_periph(ul_id);

	return p_matrix->MATRIX_MEAR[ul_id & 0x0F];
}

/**
 * \brief Enable or disable write protect of MATRIX registers.
 *
 * \param p_matrix Pointer to a Matrix instance.
 * \param ul_enable 1 to enable, 0 to disable.
 */
void matrix_set_writeprotect(Matrix *p_matrix, uint32_t ul_enable)
{
	if (ul_enable) {
		p_matrix->MATRIX_WPMR = MATRIX_WPMR_WPKEY_PASSWD | MATRIX_WPMR_WPEN;
	} else {
		p_matrix->MATRIX_WPMR = MATRIX_WPMR_WPKEY_PASSWD;
	}
}

/**
 * \brief Get write protect status.
 *
 * \param p_matrix Pointer to a Matrix instance.
 * \return Write protect status.
 */
uint32_t matrix_get_writeprotect_status(Matrix *p_matrix)
{
	return (p_matrix->MATRIX_WPSR);
}

/**
 * \brief Freezes the MATRIX configuration until hardware reset.
 *
 * \param p_matrix Pointer to a Matrix instance.
 */
void matrix_set_configuration_freeze(Matrix *p_matrix)
{
	p_matrix->MATRIX_WPMR |= MATRIX_WPMR_CFGFRZ;
}

/**
 * \brief Set Protection of the specified slave.
 *
 * \param ul_id Slave index.
 * \param ul_protection Bitmask OR of LAUSERHx, RDUSERHx, WRUSERHx, DPSOAx.
 */
void matrix_set_slave_protection(slave_id_type_t ul_id, uint32_t ul_protection)
{
	Matrix *p_matrix;

	p_matrix = _get_matrix_slave_periph(ul_id);

	p_matrix->MATRIX_PSR[ul_id & 0x0F] = ul_protection;
}

/**
 * \brief Get Protection of the specified slave.
 *
 * \param ul_id Slave index.
 * \return Bitmask OR of LAUSERHx, RDUSERHx, WRUSERHx, DPSOAx.
 */
uint32_t matrix_get_slave_protection(slave_id_type_t ul_id)
{
	Matrix *p_matrix;

	p_matrix = _get_matrix_slave_periph(ul_id);

	return p_matrix->MATRIX_PSR[ul_id & 0x0F];
}

/**
 * \brief Set Protected Areas Split of the specified slave.
 *
 * \param ul_id Slave index.
 * \param ul_pasplit Bitmask OR of MATRIX_PASSR_PASPLITx.
 */
void matrix_set_slave_protected_areas_split(slave_id_type_t ul_id, uint32_t ul_pasplit)
{
	Matrix *p_matrix;

	p_matrix = _get_matrix_slave_periph(ul_id);

	p_matrix->MATRIX_PASSR[ul_id & 0x0F] = ul_pasplit;
}

/**
 * \brief Get Protected Areas Split of the specified slave.
 *
 * \param ul_id Slave index.
 * \return Bitmask OR of MATRIX_PASSR_PASPLITx.
 */
uint32_t matrix_get_slave_protected_areas_split(slave_id_type_t ul_id)
{
	Matrix *p_matrix;

	p_matrix = _get_matrix_slave_periph(ul_id);

	return p_matrix->MATRIX_PASSR[ul_id & 0x0F];
}

/**
 * \brief Set Protected Region Top of the specified slave.
 *
 * \param ul_id Slave index.
 * \param ul_prtop Bitmask OR of MATRIX_PRTSR_PRTOPx.
 */
void matrix_set_slave_protected_region_top(slave_id_type_t ul_id, uint32_t ul_prtop)
{
	Matrix *p_matrix;

	p_matrix = _get_matrix_slave_periph(ul_id);

	p_matrix->MATRIX_PRTSR[ul_id & 0x0F] = ul_prtop;
}

/**
 * \brief Get Protected Region Top of the specified slave.
 *
 * \param ul_id Slave index.
 * \return ul_prtop Bitmask OR of MATRIX_PRTSR_PRTOPx.
 */
uint32_t matrix_get_slave_protected_region_top(slave_id_type_t ul_id)
{
	Matrix *p_matrix;

	p_matrix = _get_matrix_slave_periph(ul_id);

	return p_matrix->MATRIX_PRTSR[ul_id & 0x0F];
}

/**
 * \brief Set Protected Peripheral Select register 1.
 *
 * \param p_matrix Pointer to a Matrix instance.
 * \param ul_pselr1_mask Bitmask OR of MATRIXx_PPSELR1_xxxx. (see Matrix Protected
 * Peripheral Select bitmasks)
 *
 * \note Set bitmask just for the same MATRIXx instance
 */
void matrix_set_slave_protected_periph_select_r1(Matrix *p_matrix, uint32_t ul_pselr1_mask)
{
	p_matrix->MATRIX_PPSELR[0] = ul_pselr1_mask;
}

/**
 * \brief Get Protected Peripheral Select register 1.
 *
 * \param p_matrix Pointer to a Matrix instance.
 * \return Bitmask OR of MATRIXx_PPSELR1_xxxx. (see Matrix Protected
 * Peripheral Select bitmasks)
 */
uint32_t matrix_get_slave_protected_periph_select_r1(Matrix *p_matrix)
{
	return p_matrix->MATRIX_PPSELR[0];
}

/**
 * \brief Set Protected Peripheral Select register 2.
 *
 * \param p_matrix Pointer to a Matrix instance.
 * \param ul_pselr2_mask Bitmask OR of MATRIXx_PPSELR2_xxxx. (see Matrix Protected
 * Peripheral Select bitmasks)
 *
 * \note Set bitmask just for the same MATRIXx instance
 */
void matrix_set_slave_protected_periph_select_r2(Matrix *p_matrix, uint32_t ul_pselr2_mask)
{
	p_matrix->MATRIX_PPSELR[1] = ul_pselr2_mask;
}

/**
 * \brief Get Protected Peripheral Select register 2.
 *
 * \param p_matrix Pointer to a Matrix instance.
 * \return Bitmask OR of MATRIXx_PPSELR2_xxxx. (see Matrix Protected
 * Peripheral Select bitmasks)
 */
uint32_t matrix_get_slave_protected_periph_select_r2(Matrix *p_matrix)
{
	return p_matrix->MATRIX_PPSELR[1];
}

/**
 * \brief Set Protected Peripheral Select register 3.
 *
 * \param p_matrix Pointer to a Matrix instance.
 * \param ul_pselr3_mask Bitmask OR of MATRIXx_PPSELR3_xxxx. (see Matrix Protected
 * Peripheral Select bitmasks)
 *
 * \note Set bitmask just for the same MATRIXx instance
 */
void matrix_set_slave_protected_periph_select_r3(Matrix *p_matrix, uint32_t ul_pselr3_mask)
{
	p_matrix->MATRIX_PPSELR[2] = ul_pselr3_mask;
}

/**
 * \brief Get Protected Peripheral Select register 3.
 *
 * \param p_matrix Pointer to a Matrix instance.
 * \return Bitmask OR of MATRIXx_PPSELR3_xxxx. (see Matrix Protected
 * Peripheral Select bitmasks)
 */
uint32_t matrix_get_slave_protected_periph_select_r3(Matrix *p_matrix)
{
	return p_matrix->MATRIX_PPSELR[2];
}

/* @} */

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* / @endcond */
