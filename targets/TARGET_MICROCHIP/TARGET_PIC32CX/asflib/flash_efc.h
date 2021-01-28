/**
 * \file
 *
 * \brief Embedded Flash service for SAM.
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

#ifndef FLASH_H_INCLUDED
#define FLASH_H_INCLUDED

#include "compiler.h"
#include "efc.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
 extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/*! \name Flash driver return codes */
//! @{
typedef enum flash_efc_rc {
	FLASH_RC_OK = 0,        //!< Operation OK
	FLASH_RC_YES = 1,       //!< Yes
	FLASH_RC_NO = 0,        //!< No
	FLASH_RC_ERROR = 0x10,  //!< General error
	FLASH_RC_INVALID,       //!< Invalid argument input
	FLASH_RC_NOT_SUPPORT = 0xFFFFFFFF    //!< Operation is not supported
} flash_efc_rc_t;
//! @}

/*! \name Flash erase page num in FARG[2:0] */
//! @{
typedef enum flash_efc_farg_page_num {
	/* 8 of pages to be erased with EPA command*/
	IFLASH_ERASE_PAGES_8 = 1,
	/* 16 of pages to be erased with EPA command*/
	IFLASH_ERASE_PAGES_16,
	/* 32 of pages to be erased with EPA command*/
	IFLASH_ERASE_PAGES_32,
	/* 64 of pages to be erased with EPA command*/
	IFLASH_ERASE_PAGES_64,
	/* Parameter is not support */
	IFLASH_ERASE_PAGES_INVALID,
}flash_efc_farg_page_num_t;
//! @}

/*! \name User signature blocks
  \note User Signature Areas 1 (block 0) to 3 (block 2) erase upon hardware
  erase signal assertion. User signature areas 4 (block 3) to 6 (block 5) are not erased */
//! @{
typedef enum flash_efc_usr_sig_block {
	/* Block 0 of user signature. Erasable by erase signal */
	IFLASH_USR_SIG_BLOCK_0 = 1,
	/* Block 1 of user signature. Erasable by erase signal */
	IFLASH_USR_SIG_BLOCK_1,
	/* Block 2 of user signature. Erasable by erase signal */
	IFLASH_USR_SIG_BLOCK_2,
	/* Block 3 of user signature. No erasable by erase signal */
	IFLASH_USR_SIG_BLOCK_3,
	/* Block 4 of user signature. No erasable by erase signal */
	IFLASH_USR_SIG_BLOCK_4,
	/* Block 5 of user signature. No erasable by erase signal */
	IFLASH_USR_SIG_BLOCK_5,
}flash_efc_usr_sig_block_t;
//! @}

/*! \name Flash access mode */
//! @{
#define FLASH_OPT_MODE_CLOE    EEFC_FMR_CLOE
#define FLASH_OPT_MODE_SCOD    EEFC_FMR_SCOD
//! @}

uint32_t flash_efc_init(uint32_t ul_mode, uint32_t ul_fws);
uint32_t flash_efc_set_wait_state(uint32_t ul_address, uint32_t ul_fws);
uint32_t flash_efc_set_wait_state_adaptively(uint32_t ul_address);
uint32_t flash_efc_get_wait_state(uint32_t ul_address);
uint32_t flash_efc_get_descriptor(uint32_t ul_address, uint32_t *pul_flash_efc_descriptor, uint32_t ul_size);
uint32_t flash_efc_get_page_count(const uint32_t *pul_flash_efc_descriptor);
uint32_t flash_efc_get_page_count_per_region(const uint32_t *pul_flash_efc_descriptor);
uint32_t flash_efc_get_region_count(const uint32_t *pul_flash_efc_descriptor);
uint32_t flash_efc_erase_all(uint32_t ul_address);
uint32_t flash_efc_erase_page(uint32_t ul_address, uint8_t uc_page_num);
uint32_t flash_efc_erase_sector(uint32_t ul_address);
uint32_t flash_efc_write(uint32_t ul_address, const void *p_buffer,
		uint32_t ul_size, uint32_t ul_erase_flag);
uint32_t flash_efc_lock(uint32_t ul_start, uint32_t ul_end,
		uint32_t *pul_actual_start, uint32_t *pul_actual_end);
uint32_t flash_efc_unlock(uint32_t ul_start, uint32_t ul_end,
		uint32_t *pul_actual_start, uint32_t *pul_actual_end);
uint32_t flash_efc_is_locked(uint32_t ul_start, uint32_t ul_end);
uint32_t flash_efc_set_gpnvm(uint32_t ul_gpnvm);
uint32_t flash_efc_clear_gpnvm(uint32_t ul_gpnvm);
uint32_t flash_efc_is_gpnvm_set(uint32_t ul_gpnvm);
uint32_t flash_efc_get_gpnvm(void);
uint32_t flash_efc_enable_security_bit(void);
uint32_t flash_efc_is_security_bit_enabled(void);
uint32_t flash_efc_read_unique_id(uint32_t *pul_data, uint32_t ul_size);
uint32_t flash_efc_read_user_signature(uint32_t *p_data, uint32_t ul_block, uint32_t ul_page, uint32_t ul_size);
uint32_t flash_efc_write_user_signature(const void *p_buffer, uint32_t ul_block, uint32_t ul_page, uint32_t ul_size);
uint32_t flash_efc_erase_user_signature(uint32_t ul_block);
void flash_efc_set_rights_user_signature(uint32_t ul_rights);
void flash_efc_set_key_bus_lock_user_signature(uint32_t ul_key_bus_lock);
uint32_t flash_efc_send_cryptographic_key(uint32_t ul_periph_id, uint32_t ul_sec_mode, uint32_t ul_key_length,
		uint32_t ul_block, uint32_t ul_offset);
uint32_t flash_efc_write_erase_suspend(uint32_t ul_address);
uint32_t flash_efc_write_erase_resume(uint32_t ul_address);

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

#endif /* FLASH_H_INCLUDED */
