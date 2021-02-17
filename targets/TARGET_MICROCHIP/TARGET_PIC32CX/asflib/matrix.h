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

#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include "compiler.h"

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* / @endcond */

#ifndef MATRIX_MCFG_ULBT
#define MATRIX_MCFG_ULBT(value)    MATRIX_MCFG0_ULBT(value)
#endif

/** \brief Matrix master identifiers */
typedef enum {
	MASTER_MATRIX0_ID_CM4C0_ICB = 0x00,
	MASTER_MATRIX0_ID_CM4C0_DCB = 0x01,
	MASTER_MATRIX0_ID_CM4C0_SB = 0x02,
	MASTER_MATRIX0_ID_CMCC0 = 0x03,
	MASTER_MATRIX0_ID_CMMCC1 = 0x04,
	MASTER_MATRIX0_ID_MATRIX2 = 0x05,
	MASTER_MATRIX0_ID_MATRIX1 = 0x06,
	MASTER_MATRIX1_ID_MATRIX0_CM4 = 0x10,
	MASTER_MATRIX1_ID_MATRIX0_CMCC = 0x11,
	MASTER_MATRIX1_ID_AESB = 0x12,
	MASTER_MATRIX1_ID_ICM = 0x13,
	MASTER_MATRIX1_ID_PDC0 = 0x14,
	MASTER_MATRIX1_ID_PDC2 = 0x15,
	MASTER_MATRIX2_ID_MATRIX0 = 0x20,
	MASTER_MATRIX2_ID_MATRIX3 = 0x21,
	MASTER_MATRIX2_ID_CM4C0_ICB = 0x22,
	MASTER_MATRIX2_ID_CM4C0_DCB = 0x23,
	MASTER_MATRIX2_ID_CM4C0_SB = 0x24,
	MASTER_MATRIX3_ID_MATRIX2 = 0x30,
	MASTER_MATRIX3_ID_PDC1 = 0x31,
} master_id_type_t;

/** \brief Matrix slave identifiers */
typedef enum {
	SLAVE_MATRIX0_ID_SRAM0_PORT0 = 0x00,
	SLAVE_MATRIX0_ID_SRAM0_PORT1 = 0x01,
	SLAVE_MATRIX0_ID_SRAM0_PORT2 = 0x02,
	SLAVE_MATRIX0_ID_ROM = 0x03,
	SLAVE_MATRIX0_ID_MATRIX1_APB = 0x04,
	SLAVE_MATRIX0_ID_CPKCC = 0x05,
	SLAVE_MATRIX0_ID_APB_BRIDGE3 = 0x06,
	SLAVE_MATRIX0_ID_MATRIX2 = 0x07,
	SLAVE_MATRIX0_ID_CMCC0 = 0x08,
	SLAVE_MATRIX0_ID_CMCC1 = 0x09,
	SLAVE_MATRIX0_ID_FLASH = 0x0A,
	SLAVE_MATRIX0_ID_MATRIX1 = 0x0B,
	SLAVE_MATRIX1_ID_QSPI = 0x10,
	SLAVE_MATRIX1_ID_AESB = 0x11,
	SLAVE_MATRIX1_ID_MATRIX0 = 0x12,
	SLAVE_MATRIX1_ID_APB_BRIDGE0 = 0x13,
	SLAVE_MATRIX1_ID_APB_BRIDGE2 = 0x14,
	SLAVE_MATRIX2_ID_MATRIX2 = 0x20,
	SLAVE_MATRIX2_ID_MATRIX3 = 0x21,
	SLAVE_MATRIX2_ID_APB_BRIDGE4 = 0x22,
	SLAVE_MATRIX2_ID_SRAM1_PORT0 = 0x23,
	SLAVE_MATRIX2_ID_SRAM1_PORT1 = 0x24,
	SLAVE_MATRIX2_ID_SRAM1_PORT2 = 0x25,
	SLAVE_MATRIX2_ID_SRAM1_PORT3 = 0x26,
	SLAVE_MATRIX2_ID_SRAM2_PORT0 = 0x27,
	SLAVE_MATRIX2_ID_SRAM2_PORT1 = 0x28,
	SLAVE_MATRIX2_ID_SRAM2_PORT2 = 0x29,
	SLAVE_MATRIX2_ID_SRAM2_PORT3 = 0x2A,
	SLAVE_MATRIX3_ID_MATRIX2 = 0x30,
	SLAVE_MATRIX3_ID_APB_BRIDGE1 = 0x31,
} slave_id_type_t;

/** \brief Matrix master: undefined length burst type */
typedef enum {
	MATRIX_ULBT_INFINITE_LENGTH_BURST = MATRIX_MCFG_ULBT(0),
	MATRIX_ULBT_SINGLE_ACCESS         = MATRIX_MCFG_ULBT(1),
	MATRIX_ULBT_4_BEAT_BURST          = MATRIX_MCFG_ULBT(2),
	MATRIX_ULBT_8_BEAT_BURST          = MATRIX_MCFG_ULBT(3),
	MATRIX_ULBT_16_BEAT_BURST         = MATRIX_MCFG_ULBT(4),
	MATRIX_ULBT_32_BEAT_BURST         = MATRIX_MCFG_ULBT(5),
	MATRIX_ULBT_64_BEAT_BURST         = MATRIX_MCFG_ULBT(6),
	MATRIX_ULBT_128_BEAT_BURST        = MATRIX_MCFG_ULBT(7),
} burst_type_t;

/** \brief Matrix slave: default master type */
typedef enum {
	MATRIX_DEFMSTR_NO_DEFAULT_MASTER    = MATRIX_SCFG_DEFMSTR_TYPE(0),
	MATRIX_DEFMSTR_LAST_DEFAULT_MASTER  = MATRIX_SCFG_DEFMSTR_TYPE(1),
	MATRIX_DEFMSTR_FIXED_DEFAULT_MASTER = MATRIX_SCFG_DEFMSTR_TYPE(2)
} default_master_t;

/** \brief Matrix Protected Region Top Slave  */
typedef enum {
	MATRIX_SIZE_4KB = 0,
	MATRIX_SIZE_8KB,
	MATRIX_SIZE_16KB,
	MATRIX_SIZE_32KB,
	MATRIX_SIZE_64KB,
	MATRIX_SIZE_128KB,
	MATRIX_SIZE_256KB,
	MATRIX_SIZE_512KB,
	MATRIX_SIZE_1MB,
	MATRIX_SIZE_2MB,
	MATRIX_SIZE_4MB, 
	MATRIX_SIZE_8MB,
	MATRIX_SIZE_16MB,
	MATRIX_SIZE_32MB,
	MATRIX_SIZE_64MB,
	MATRIX_SIZE_128MB
} matrix_size_t;

/** \brief Matrix Protected Peripheral Select bitmasks */
#define MATRIX0_PPSELR2_SEFC0    (0x1u << 0)  /**< \brief MATRIX0 PPSELR2: SEFC0 */
#define MATRIX0_PPSELR2_SEFC1    (0x1u << 1)  /**< \brief MATRIX0 PPSELR2: SEFC1 */
#define MATRIX0_PPSELR2_PMC      (0x1u << 2)  /**< \brief MATRIX0 PPSELR2: PMC */

#define MATRIX0_PPSELR3_CPKCC    (0x1u << 0)  /**< \brief MATRIX0 PPSELR3: CPKCC */
#define MATRIX0_PPSELR3_CMCCI    (0x1u << 2)  /**< \brief MATRIX0 PPSELR3: CMCCI */
#define MATRIX0_PPSELR3_CMCCD    (0x1u << 3)  /**< \brief MATRIX0 PPSELR3: CMCCD */

#define MATRIX1_PPSELR1_SYSC     (0x1u << 0)  /**< \brief MATRIX1 PPSELR1: SYSC */

#define MATRIX1_PPSELR2_CHIPID   (0x1u << 1)  /**< \brief MATRIX1 PPSELR2: CHIPID */
#define MATRIX1_PPSELR2_SFR      (0x1u << 2)  /**< \brief MATRIX1 PPSELR2: SFR */
#define MATRIX1_PPSELR2_SFRBU    (0x1u << 3)  /**< \brief MATRIX1 PPSELR2: SFRBU */

#define MATRIX1_PPSELR3_FLEXCOM0 (0x1u << 0)  /**< \brief MATRIX1 PPSELR3: FLEXCOM0 */
#define MATRIX1_PPSELR3_FLEXCOM1 (0x1u << 1)  /**< \brief MATRIX1 PPSELR3: FLEXCOM1 */
#define MATRIX1_PPSELR3_FLEXCOM2 (0x1u << 2)  /**< \brief MATRIX1 PPSELR3: FLEXCOM2 */
#define MATRIX1_PPSELR3_FLEXCOM3 (0x1u << 3)  /**< \brief MATRIX1 PPSELR3: FLEXCOM3 */
#define MATRIX1_PPSELR3_FLEXCOM4 (0x1u << 4)  /**< \brief MATRIX1 PPSELR3: FLEXCOM4 */
#define MATRIX1_PPSELR3_FLEXCOM5 (0x1u << 5)  /**< \brief MATRIX1 PPSELR3: FLEXCOM5 */
#define MATRIX1_PPSELR3_FLEXCOM6 (0x1u << 6)  /**< \brief MATRIX1 PPSELR3: FLEXCOM6 */
#define MATRIX1_PPSELR3_FLEXCOM7 (0x1u << 7)  /**< \brief MATRIX1 PPSELR3: FLEXCOM7 */
#define MATRIX1_PPSELR3_QSPI     (0x1u << 8)  /**< \brief MATRIX1 PPSELR3: QSPI */
#define MATRIX1_PPSELR3_ADCC     (0x1u << 9)  /**< \brief MATRIX1 PPSELR3: ADCC */
#define MATRIX1_PPSELR3_ACC      (0x1u << 10) /**< \brief MATRIX1 PPSELR3: ACC */
#define MATRIX1_PPSELR3_IPC0     (0x1u << 11) /**< \brief MATRIX1 PPSELR3: IPC0 */
#define MATRIX1_PPSELR3_SLCDC    (0x1u << 12) /**< \brief MATRIX1 PPSELR3: SLCDC */
#define MATRIX1_PPSELR3_MEM2MEM0 (0x1u << 13) /**< \brief MATRIX1 PPSELR3: MEM2MEM0 */
#define MATRIX1_PPSELR3_TC0      (0x1u << 14) /**< \brief MATRIX1 PPSELR3: TC0 */
#define MATRIX1_PPSELR3_TC1      (0x1u << 15) /**< \brief MATRIX1 PPSELR3: TC1 */
#define MATRIX1_PPSELR3_TC2      (0x1u << 16) /**< \brief MATRIX1 PPSELR3: TC2 */
#define MATRIX1_PPSELR3_PIOA     (0x1u << 18) /**< \brief MATRIX1 PPSELR3: PIOA */
#define MATRIX1_PPSELR3_AES      (0x1u << 19) /**< \brief MATRIX1 PPSELR3: AES */
#define MATRIX1_PPSELR3_AESB     (0x1u << 20) /**< \brief MATRIX1 PPSELR3: AESB */
#define MATRIX1_PPSELR3_SHA      (0x1u << 21) /**< \brief MATRIX1 PPSELR3: SHA */
#define MATRIX1_PPSELR3_TRNG     (0x1u << 22) /**< \brief MATRIX1 PPSELR3: TRNG */
#define MATRIX1_PPSELR3_ICM      (0x1u << 23) /**< \brief MATRIX1 PPSELR3: ICM */

#define MATRIX3_PPSELR3_EMAFE    (0x1u << 0)  /**< \brief MATRIX3 PPSELR3: EMAFE */
#define MATRIX3_PPSELR3_MEM2MEM1 (0x1u << 1)  /**< \brief MATRIX3 PPSELR3: MEM2MEM1 */
#define MATRIX3_PPSELR3_TC3      (0x1u << 2)  /**< \brief MATRIX3 PPSELR3: TC3 */
#define MATRIX3_PPSELR3_PIOD     (0x1u << 3)  /**< \brief MATRIX3 PPSELR3: PIOD */
#define MATRIX3_PPSELR3_UART     (0x1u << 4)  /**< \brief MATRIX3 PPSELR3: UART */
#define MATRIX3_PPSELR3_IPC1     (0x1u << 5)  /**< \brief MATRIX3 PPSELR3: IPC1 */
#define MATRIX3_PPSELR3_MCSPI    (0x1u << 6)  /**< \brief MATRIX3 PPSELR3: MCSPI */
#define MATRIX3_PPSELR3_PWM      (0x1u << 7)  /**< \brief MATRIX3 PPSELR3: PWM */
#define MATRIX3_PPSELR3_I2SMCC   (0x1u << 9)  /**< \brief MATRIX3 PPSELR3: I2SMCC */

void matrix_set_master_burst_type(master_id_type_t ul_id, burst_type_t burst_type);
burst_type_t matrix_get_master_burst_type(master_id_type_t ul_id);

void matrix_set_slave_slot_cycle(slave_id_type_t ul_id, uint32_t ul_slot_cycle);
uint32_t matrix_get_slave_slot_cycle(slave_id_type_t ul_id);
void matrix_set_slave_default_master_type(slave_id_type_t ul_id, default_master_t type);
default_master_t matrix_get_slave_default_master_type(slave_id_type_t ul_id);
void matrix_set_slave_fixed_default_master(slave_id_type_t ul_id, uint32_t ul_fixed_id);
uint32_t matrix_get_slave_fixed_default_master(slave_id_type_t ul_id);

void matrix_set_slave_priority(slave_id_type_t ul_id, uint32_t ul_prio, uint32_t ul_lqos);
uint32_t matrix_get_slave_priority(slave_id_type_t ul_id);
void matrix_set_slave_priority_b(slave_id_type_t ul_id, uint32_t ul_prio_b, uint32_t ul_lqos_b);
uint32_t matrix_get_slave_priority_b(slave_id_type_t ul_id);

void matrix_set_master_remap(Matrix *p_matrix, uint32_t ul_remap);
uint32_t matrix_get_master_remap(Matrix *p_matrix);

void matrix_master_enable_interrupt(Matrix *p_matrix, const uint32_t ul_mask);
void matrix_master_disable_interrupt(Matrix *p_matrix, const uint32_t ul_mask);
uint32_t matrix_master_get_interrupt_mask(Matrix *p_matrix);
uint32_t matrix_master_get_error_status(Matrix *p_matrix);
uint32_t matrix_master_get_error_address(master_id_type_t ul_id);

void matrix_set_writeprotect(Matrix *p_matrix, uint32_t ul_enable);
uint32_t matrix_get_writeprotect_status(Matrix *p_matrix);
void matrix_set_configuration_freeze(Matrix *p_matrix);

void matrix_set_slave_protection(slave_id_type_t ul_id, uint32_t ul_protection);
uint32_t matrix_get_slave_protection(slave_id_type_t ul_id);
void matrix_set_slave_protected_areas_split(slave_id_type_t ul_id, uint32_t ul_pasplit);
uint32_t matrix_get_slave_protected_areas_split(slave_id_type_t ul_id);
void matrix_set_slave_protected_region_top(slave_id_type_t ul_id, uint32_t ul_prtop);
uint32_t matrix_get_slave_protected_region_top(slave_id_type_t ul_id);

void matrix_set_slave_protected_periph_select_r1(Matrix *p_matrix, uint32_t ul_pselr1_mask);
uint32_t matrix_get_slave_protected_periph_select_r1(Matrix *p_matrix);
void matrix_set_slave_protected_periph_select_r2(Matrix *p_matrix, uint32_t ul_pselr2_mask);
uint32_t matrix_get_slave_protected_periph_select_r2(Matrix *p_matrix);
void matrix_set_slave_protected_periph_select_r3(Matrix *p_matrix, uint32_t ul_pselr3_mask);
uint32_t matrix_get_slave_protected_periph_select_r3(Matrix *p_matrix);

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* / @endcond */

#endif /* MATRIX_H_INCLUDED */
