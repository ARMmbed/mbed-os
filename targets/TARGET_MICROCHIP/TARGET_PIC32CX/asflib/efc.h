/**
 * \file
 *
 * \brief Embedded Flash Controller (EFC) driver for SAM.
 *
 * Copyright (c) 2011-2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef EFC_H_INCLUDED
#define EFC_H_INCLUDED

#include "compiler.h"
#include "pic32cx.h"

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* / @endcond */

/*! \name EFC return codes */
/* ! @{ */
typedef enum efc_rc {
	EFC_RC_OK = 0,      /* !< Operation OK */
	EFC_RC_YES = 0,     /* !< Yes */
	EFC_RC_NO = 1,      /* !< No */
	EFC_RC_ERROR = 1,   /* !< General error */
	EFC_RC_INVALID,     /* !< Invalid argument input */
	EFC_RC_NOT_SUPPORT = 0xFFFFFFFF /* !< Operation is not supported */
} efc_rc_t;
/* ! @} */

/*! \name EFC command */
/* ! @{ */
#define EFC_FCMD_GETD    0x00  /* !< Get Flash Descriptor */
#define EFC_FCMD_WP      0x01  /* !< Write page */
#define EFC_FCMD_WPL     0x02  /* !< Write page and lock */
#define EFC_FCMD_EA      0x05  /* !< Erase all */
#define EFC_FCMD_EPA     0x07  /* !< Erase pages */
#define EFC_FCMD_SLB     0x08  /* !< Set Lock Bit */
#define EFC_FCMD_CLB     0x09  /* !< Clear Lock Bit */
#define EFC_FCMD_GLB     0x0A  /* !< Get Lock Bit */
#define EFC_FCMD_SGPB    0x0B  /* !< Set GPNVM Bit */
#define EFC_FCMD_CGPB    0x0C  /* !< Clear GPNVM Bit */
#define EFC_FCMD_GGPB    0x0D  /* !< Get GPNVM Bit */
#define EFC_FCMD_STUI    0x0E  /* !< Start unique ID */
#define EFC_FCMD_SPUI    0x0F  /* !< Stop unique ID */
#define EFC_FCMD_ES      0x11  /* !< Erase sector */
#define EFC_FCMD_WUS     0x12  /* !< Write user signature */
#define EFC_FCMD_EUS     0x13  /* !< Erase user signature */
#define EFC_FCMD_STUS    0x14  /* !< Start read user signature */
#define EFC_FCMD_SPUS    0x15  /* !< Stop read user signature */
#define EFC_FCMD_SUSP    0x17  /* !< Suspend */
#define EFC_FCMD_RES     0x18  /* !< Resume */
#define EFC_FCMD_SCK     0x19  /* !< Send Cryptographic Key */
/* ! @} */

#define Efc  Sefc

uint32_t efc_init(Efc *p_efc, uint32_t ul_optimization_mode, uint32_t ul_fws);
void efc_enable_cloe(Efc *p_efc);
void efc_disable_cloe(Efc *p_efc);
void efc_enable_frdy_interrupt(Efc *p_efc);
void efc_disable_frdy_interrupt(Efc *p_efc);
void efc_set_write_protection(Efc *p_efc, uint32_t ul_mode);
void efc_enable_write_protection(Efc *p_efc);
void efc_disable_write_protection(Efc *p_efc);
void efc_lock_write_erase_commands(Efc *p_efc);
void efc_enable_usr_write_protection(Efc *p_efc);
void efc_disable_usr_write_protection(Efc *p_efc);
void efc_enable_erase_write_commands(Efc *p_efc);
void efc_disable_erase_write_commands(Efc *p_efc);
void efc_enable_lock_bit_write_protection(Efc *p_efc);
void efc_disable_lock_bit_write_protection(Efc *p_efc);
void efc_enable_gpnvm_write_protection(Efc *p_efc);
void efc_disable_gpnvm_write_protection(Efc *p_efc);
uint32_t efc_get_usr_rights(Efc *p_efc);
void efc_set_usr_rights(Efc *p_efc, uint32_t ul_usr_rights);
void efc_lock_key_bus_transfer(Efc *p_efc, uint32_t ul_usr_block);
void efc_unlock_key_bus_transfer(Efc *p_efc);
void efc_set_wait_state(Efc *p_efc, uint32_t ul_fws);
uint32_t efc_get_wait_state(Efc *p_efc);
uint32_t efc_perform_command(Efc *p_efc, uint32_t ul_command, uint32_t ul_argument);
uint32_t efc_get_status(Efc *p_efc);
uint32_t efc_get_result(Efc *p_efc);
uint32_t efc_perform_read_sequence(Efc *p_efc, uint32_t ul_cmd_st, uint32_t ul_cmd_sp,
		uint32_t *p_ul_buf, uint32_t ul_offset, uint32_t ul_size);

/* / @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* / @endcond */

#endif /* EFC_H_INCLUDED */
