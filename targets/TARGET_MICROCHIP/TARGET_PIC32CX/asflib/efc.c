/**
 * \file
 *
 * \brief Enhanced Embedded Flash Controller (EEFC) driver for SAM.
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

#include "efc.h"
#include <interrupt_sam_nvic.h>

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \defgroup sam_drivers_efc_group Enhanced Embedded Flash Controller (EEFC)
 *
 * The Enhanced Embedded Flash Controller ensures the interface of the Flash
 * block with the 32-bit internal bus.
 *
 * @{
 */

/* Address definition for read operation */
# define EFC0               SEFC0
# define READ_BUFF_ADDR0    IFLASH0_CNC_ADDR
# define READ_BUFF_ADDR1    IFLASH1_CNC_ADDR

/* Flash Writing Protection Key */
#define FWP_KEY    0x5Au

#define EEFC_ERROR_FLAGS     (EEFC_FSR_FLOCKE | EEFC_FSR_FCMDE | EEFC_FSR_FLERR | EEFC_FSR_WPERR\
	| EEFC_FSR_SECCELSBD | EEFC_FSR_MECCELSBD | EEFC_FSR_SECCEMSBD | EEFC_FSR_MECCEMSBD\
	| EEFC_FSR_SECCELSBL | EEFC_FSR_MECCELSBL | EEFC_FSR_SECCEMSBL | EEFC_FSR_MECCEMSBL)


#ifndef EEFC_FCR_FKEY_PASSWD
#define EEFC_FCR_FKEY_PASSWD EEFC_FCR_FKEY(FWP_KEY)
#endif


/*
 * Local function declaration.
 * Because they are RAM functions, they need 'extern' declaration.
 */
extern void efc_write_fmr(Efc *p_efc, uint32_t ul_fmr);
extern uint32_t efc_perform_fcr(Efc *p_efc, uint32_t ul_fcr);

/**
 * \brief Initialize the EFC controller.
 *
 * \param ul_optimization_mode See EEFC_FMR_SCOD, EEFC_FMR_CLOE.
 * \param ul_fws The number of wait states in cycle (no shift).
 *
 * \return 0 if successful.
 */
uint32_t efc_init(Efc *p_efc, uint32_t ul_optimization_mode, uint32_t ul_fws)
{
	efc_write_fmr(p_efc, ul_optimization_mode | EEFC_FMR_FWS(ul_fws));

	return EFC_RC_OK;
}
/**
 * \brief Enable code loop optimization.
 *
 * \param p_efc Pointer to an EFC instance.
 */
void efc_enable_cloe(Efc *p_efc)
{
	uint32_t ul_fmr = p_efc->EEFC_FMR;
	efc_write_fmr(p_efc, ul_fmr | EEFC_FMR_CLOE);
}

/**
 * \brief Disable code loop optimization.
 *
 * \param p_efc Pointer to an EFC instance.
 */
void efc_disable_cloe(Efc *p_efc)
{
	uint32_t ul_fmr = p_efc->EEFC_FMR;
	efc_write_fmr(p_efc, ul_fmr & (~EEFC_FMR_CLOE));
}

/**
 * \brief Enable the flash ready interrupt.
 *
 * \param p_efc Pointer to an EFC instance.
 */
void efc_enable_frdy_interrupt(Efc *p_efc)
{
	uint32_t ul_fmr = p_efc->EEFC_FMR;

	efc_write_fmr(p_efc, ul_fmr | EEFC_FMR_FRDY);
}

/**
 * \brief Disable the flash ready interrupt.
 *
 * \param p_efc Pointer to an EFC instance.
 */
void efc_disable_frdy_interrupt(Efc *p_efc)
{
	uint32_t ul_fmr = p_efc->EEFC_FMR;

	efc_write_fmr(p_efc, ul_fmr & (~EEFC_FMR_FRDY));
}

/**
 * \brief Enable the write protection.
 *
 * \param p_efc Pointer to an EFC instance.
 */
void efc_enable_write_protection(Efc *p_efc)
{
	uint32_t ul_wpmr = p_efc->EEFC_WPMR;

	ul_wpmr &= ~EEFC_WPMR_WPKEY_Msk;

	p_efc->EEFC_WPMR = ul_wpmr | EEFC_WPMR_WPKEY_PASSWD | EEFC_WPMR_WPEN;
}

/**
 * \brief Disable the write protection.
 *
 * \param p_efc Pointer to an EFC instance.
 */
void efc_disable_write_protection(Efc *p_efc)
{
	p_efc->EEFC_WPMR = EEFC_WPMR_WPKEY_PASSWD;
}

/**
 * \brief Lock all write and erase commands.
 *
 * \param p_efc Pointer to an EFC instance.
 *
 * \note When locking, commands can no longer be unlocked until hardware reset
 */
void efc_lock_write_erase_commands(Efc *p_efc)
{
	uint32_t ul_wpmr = p_efc->EEFC_WPMR;

	ul_wpmr &= ~(EEFC_WPMR_WPKEY_Msk);

	p_efc->EEFC_WPMR = ul_wpmr | EEFC_WPMR_WPKEY_PASSWD | EEFC_WPMR_ERASEWL;
}

/**
 * \brief Enable the User Signature write protection.
 *
 * \param p_efc Pointer to an EFC instance.
 */
void efc_enable_usr_write_protection(Efc *p_efc)
{
	uint32_t ul_wpmr = p_efc->EEFC_WPMR;

	ul_wpmr &= ~EEFC_WPMR_WPKEY_Msk;

	p_efc->EEFC_WPMR = ul_wpmr | EEFC_WPMR_WPKEY_PASSWD | EEFC_WPMR_USRWP;
}

/**
 * \brief Disable the User Signature write protection.
 *
 * \param p_efc Pointer to an EFC instance.
 */
void efc_disable_usr_write_protection(Efc *p_efc)
{
	uint32_t ul_wpmr = p_efc->EEFC_WPMR;

	ul_wpmr &= ~(EEFC_WPMR_WPKEY_Msk | EEFC_WPMR_USRWP);

	p_efc->EEFC_WPMR = EEFC_WPMR_WPKEY_PASSWD;
}

/**
 * \brief Enable all write and erase commands.
 *
 * \param p_efc Pointer to an EFC instance.
 */
void efc_enable_erase_write_commands(Efc *p_efc)
{
	uint32_t ul_wpmr = p_efc->EEFC_WPMR;

	ul_wpmr &= ~(EEFC_WPMR_WPKEY_Msk | EEFC_WPMR_ERASEWP);

	p_efc->EEFC_WPMR = EEFC_WPMR_WPKEY_PASSWD;
}

/**
 * \brief Disable all write and erase commands.
 *
 * \param p_efc Pointer to an EFC instance.
 */
void efc_disable_erase_write_commands(Efc *p_efc)
{
	uint32_t ul_wpmr = p_efc->EEFC_WPMR;

	ul_wpmr &= ~EEFC_WPMR_WPKEY_Msk;

	p_efc->EEFC_WPMR = ul_wpmr | EEFC_WPMR_WPKEY_PASSWD | EEFC_WPMR_ERASEWP;
}

/**
 * \brief Enable �Set lock bit� and �Clear lock bit� commands.
 *
 * \param p_efc Pointer to an EFC instance.
 */
void efc_enable_lock_bit_write_protection(Efc *p_efc)
{
	uint32_t ul_wpmr = p_efc->EEFC_WPMR;

	ul_wpmr &= ~(EEFC_WPMR_WPKEY_Msk | EEFC_WPMR_LOCKWP);

	p_efc->EEFC_WPMR = EEFC_WPMR_WPKEY_PASSWD;
}

/**
 * \brief Disable �Set lock bit� and �Clear lock bit� commands.
 *
 * \param p_efc Pointer to an EFC instance.
 */
void efc_disable_lock_bit_write_protection(Efc *p_efc)
{
	uint32_t ul_wpmr = p_efc->EEFC_WPMR;

	ul_wpmr &= ~EEFC_WPMR_WPKEY_Msk;

	p_efc->EEFC_WPMR = ul_wpmr | EEFC_WPMR_WPKEY_PASSWD | EEFC_WPMR_LOCKWP;
}

/**
 * \brief Enable �Set GPNVM bit� and �Clear GPNVM bit� commands.
 *
 * \param p_efc Pointer to an EFC instance.
 */
void efc_enable_gpnvm_write_protection(Efc *p_efc)
{
	uint32_t ul_wpmr = p_efc->EEFC_WPMR;

	ul_wpmr &= ~(EEFC_WPMR_WPKEY_Msk | EEFC_WPMR_GPNVMWP);

	p_efc->EEFC_WPMR = EEFC_WPMR_WPKEY_PASSWD;
}

/**
 * \brief Disable �Set GPNVM bit� and �Clear GPNVM bit� commands.
 *
 * \param p_efc Pointer to an EFC instance.
 */
void efc_disable_gpnvm_write_protection(Efc *p_efc)
{
	uint32_t ul_wpmr = p_efc->EEFC_WPMR;

	ul_wpmr &= ~EEFC_WPMR_WPKEY_Msk;

	p_efc->EEFC_WPMR = ul_wpmr | EEFC_WPMR_WPKEY_PASSWD | EEFC_WPMR_GPNVMWP;
}

/**
 * \brief Get User Signature Rights.
 *
 * \param p_efc Pointer to an EFC instance.
 */
uint32_t efc_get_usr_rights(Efc *p_efc)
{
	return p_efc->EEFC_USR;
}

/**
 * \brief Set User Signature Rights.
 *
 * \param p_efc Pointer to an EFC instance.
 * \param ul_usr_rights User Signature Rights.
 */
void efc_set_usr_rights(Efc *p_efc, uint32_t ul_usr_rights)
{
	p_efc->EEFC_USR = ul_usr_rights;
}

/**
 * \brief Lock Key Bus transfer from the first page of the User Signature Block.
 *
 * \param p_efc Pointer to an EFC instance.
 * \param ul_usr_block Block of the User Signature.
 *
 * \note When locking, it can no longer be unlocked until hardware reset
 */
void efc_lock_key_bus_transfer(Efc *p_efc, uint32_t ul_usr_block)
{
	uint32_t ul_kblr = p_efc->EEFC_KBLR;

	p_efc->EEFC_KBLR = ul_kblr | ul_usr_block;
}


/**
 * \brief Unlock Key Bus transfer from the first page of the User Signature Block.
 *
 * \param p_efc Pointer to an EFC instance.
 */
void efc_unlock_key_bus_transfer(Efc *p_efc)
{
	p_efc->EEFC_KBLR = 0;
}

/**
 * \brief Set flash wait state.
 *
 * \param p_efc Pointer to an EFC instance.
 * \param ul_fws The number of wait states in cycle (no shift).
 */
void efc_set_wait_state(Efc *p_efc, uint32_t ul_fws)
{
	uint32_t ul_fmr = p_efc->EEFC_FMR & (~EEFC_FMR_FWS_Msk);

	efc_write_fmr(p_efc, ul_fmr | EEFC_FMR_FWS(ul_fws));
}

/**
 * \brief Get flash wait state.
 *
 * \param p_efc Pointer to an EFC instance.
 *
 * \return The number of wait states in cycle (no shift).
 */
uint32_t efc_get_wait_state(Efc *p_efc)
{
	return ((p_efc->EEFC_FMR & EEFC_FMR_FWS_Msk) >> EEFC_FMR_FWS_Pos);
}

/**
 * \brief Perform the given command and wait until its completion (or an error).
 *
 * \note Unique ID commands are not supported, use efc_perform_read_sequence.
 *
 * \param p_efc Pointer to an EFC instance.
 * \param ul_command Command to perform.
 * \param ul_argument Optional command argument.
 *
 * \note This function will automatically choose to use IAP function.
 *
 * \return 0 if successful, otherwise returns an error code.
 */
uint32_t efc_perform_command(Efc *p_efc, uint32_t ul_command, uint32_t ul_argument)
{
	uint32_t result;
	irqflags_t flags;

	/* Unique ID commands are not supported. */
	if (ul_command == EFC_FCMD_STUI || ul_command == EFC_FCMD_SPUI) {
		return EFC_RC_NOT_SUPPORT;
	}

	flags = cpu_irq_save();
	/* Use RAM Function. */
	result = efc_perform_fcr(p_efc,
			EEFC_FCR_FKEY_PASSWD | EEFC_FCR_FARG(ul_argument) |
			EEFC_FCR_FCMD(ul_command));
	cpu_irq_restore(flags);
	return result;
}

/**
 * \brief Get the current status of the EEFC.
 *
 * \note This function clears the value of some status bits (FLOCKE, FCMDE).
 *
 * \param p_efc Pointer to an EFC instance.
 *
 * \return The current status.
 */
uint32_t efc_get_status(Efc *p_efc)
{
	return p_efc->EEFC_FSR;
}

/**
 * \brief Get the result of the last executed command.
 *
 * \param p_efc Pointer to an EFC instance.
 *
 * \return The result of the last executed command.
 */
uint32_t efc_get_result(Efc *p_efc)
{
	return p_efc->EEFC_FRR;
}

/**
 * \brief Perform read sequence. Supported sequences are read Unique ID and
 * read User Signature
 *
 * \param p_efc Pointer to an EFC instance.
 * \param ul_cmd_st Start command to perform.
 * \param ul_cmd_sp Stop command to perform.
 * \param ul_offset Offset from start of the region to read.
 * \param p_ul_buf Pointer to an data buffer.
 * \param ul_size Buffer size.
 *
 * \return 0 if successful, otherwise returns an error code.
 */
__no_inline
RAMFUNC
uint32_t efc_perform_read_sequence(Efc *p_efc, uint32_t ul_cmd_st, uint32_t ul_cmd_sp,
		uint32_t *p_ul_buf, uint32_t ul_offset, uint32_t ul_size)
{
	volatile uint32_t ul_status;
	uint32_t ul_cnt;
	uint32_t ul_reg_fmr;

	uint32_t *p_ul_data = (uint32_t *) ((p_efc == EFC0) ? READ_BUFF_ADDR0 : READ_BUFF_ADDR1);
	p_ul_data += ul_offset;

	if (p_ul_buf == NULL) {
		return EFC_RC_INVALID;
	}

	/* Disable Sequential Code Optimization */
	ul_reg_fmr = p_efc->EEFC_FMR;
	p_efc->EEFC_FMR |= EEFC_FMR_SCOD;

	/* Send the Start Read command */
	p_efc->EEFC_FCR = EEFC_FCR_FKEY_PASSWD | EEFC_FCR_FARG(0) | EEFC_FCR_FCMD(ul_cmd_st);

	/* Wait for the FRDY bit in the Flash Programming Status Register (EEFC_FSR) falls. */
	do {
		ul_status = p_efc->EEFC_FSR;
	} while ((ul_status & EEFC_FSR_FRDY) == EEFC_FSR_FRDY);

	/* The data is located in the first address of the Flash memory mapping. */
	for (ul_cnt = 0; ul_cnt < ul_size; ul_cnt++) {
		p_ul_buf[ul_cnt] = p_ul_data[ul_cnt];
	}

	/* To stop the read mode */
	p_efc->EEFC_FCR = EEFC_FCR_FKEY_PASSWD | EEFC_FCR_FARG(0) | EEFC_FCR_FCMD(ul_cmd_sp);

	/* Wait for the FRDY bit in the Flash Programming Status Register (EEFC_FSR)
	 * rises.
	 */
	do {
		ul_status = p_efc->EEFC_FSR;
	} while ((ul_status & EEFC_FSR_FRDY) != EEFC_FSR_FRDY);

	/* Enable Sequential Code Optimization */
	if ((ul_reg_fmr & EEFC_FMR_SCOD) == 0) {
		p_efc->EEFC_FMR &= ~EEFC_FMR_SCOD;
	}

	return EFC_RC_OK;
}

/**
 * \brief Set mode register.
 *
 * \param p_efc Pointer to an EFC instance.
 * \param ul_fmr Value of mode register
 */
__no_inline
RAMFUNC
void efc_write_fmr(Efc *p_efc, uint32_t ul_fmr)
{
	p_efc->EEFC_FMR = ul_fmr;
}

/**
 * \brief Perform command.
 *
 * \param p_efc Pointer to an EFC instance.
 * \param ul_fcr Flash command.
 *
 * \return The current status.
 */
__no_inline
RAMFUNC
uint32_t efc_perform_fcr(Efc *p_efc, uint32_t ul_fcr)
{
	volatile uint32_t ul_status;
	volatile uint32_t ul_value;

	do {
		p_efc->EEFC_FCR = ul_fcr;
		do {
			ul_status = p_efc->EEFC_FSR;
		} while ((ul_status & EEFC_FSR_FRDY) != EEFC_FSR_FRDY);

		if (ul_fcr == 0x5A00000B) {
			/* Detect Set GPNVM Bit 0 (security bit) */
			/* Get GPNVM bits */
			p_efc->EEFC_FCR = 0x5A00000D;
			do {
				ul_status = p_efc->EEFC_FSR;
			} while ((ul_status & EEFC_FSR_FRDY) != EEFC_FSR_FRDY);
			ul_value = p_efc->EEFC_FRR;

			if (ul_value & 1<<5) {
				/* Clear GPNVM Bit 5 */
				p_efc->EEFC_FCR = 0x5A00050C;
				do {
					ul_status = p_efc->EEFC_FSR;
				} while ((ul_status & EEFC_FSR_FRDY) != EEFC_FSR_FRDY);
				/* Set GPNVM Bit 5 */
				p_efc->EEFC_FCR = 0x5A00050B;
				do {
					ul_status = p_efc->EEFC_FSR;
				} while ((ul_status & EEFC_FSR_FRDY) != EEFC_FSR_FRDY);
			} else {
				/* Set GPNVM Bit 5 */
				p_efc->EEFC_FCR = 0x5A00050B;
				do {
					ul_status = p_efc->EEFC_FSR;
				} while ((ul_status & EEFC_FSR_FRDY) != EEFC_FSR_FRDY);
				/* Clear GPNVM Bit 5 */
				p_efc->EEFC_FCR = 0x5A00050C;
				do {
					ul_status = p_efc->EEFC_FSR;
				} while ((ul_status & EEFC_FSR_FRDY) != EEFC_FSR_FRDY);
			}
		}

	} while ((ul_status & EEFC_FSR_FLERR) == EEFC_FSR_FLERR);

	return (ul_status & EEFC_ERROR_FLAGS);
}

//@}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
