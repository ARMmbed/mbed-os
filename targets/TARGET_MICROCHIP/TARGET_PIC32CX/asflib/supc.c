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

#include "supc.h"
#include "pic32cx.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \defgroup sam_drivers_supc_group Supply Controller (SUPC)
 *
 * Driver for the SUPC (Supply Controller). This driver provides access to the main
 * features of the Supply Controller.
 *
 * @{
 */

/**
 * \brief Switch off the voltage regulator to put the device in backup mode.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_enable_backup_mode(Supc *p_supc)
{
	p_supc->SUPC_CR = SUPC_CR_KEY_PASSWD | SUPC_CR_VROFF;
	uint32_t ul_dummy = p_supc->SUPC_MR;
	__DSB();
	__WFE();
	__WFI();
	UNUSED(ul_dummy);
}

/**
 * \brief Enable the shutdown pin.
 *
 * \param p_supc   Pointer to a SUPC instance.
 * \param ul_eof   1 to wait the end of frame of the LCD driver occurs, 0 for inmediately.
 */
void supc_enable_shutdown(Supc *p_supc, uint32_t ul_eof)
{
	if (ul_eof == 1) {
		p_supc->SUPC_CR |= SUPC_CR_KEY_PASSWD | SUPC_CR_SHDWEOF;
	} else {
		p_supc->SUPC_CR |= SUPC_CR_KEY_PASSWD | SUPC_CR_SHDW;
	}
}

/**
 * \brief Switch slow clock source selection to external 32k (Xtal or Bypass) oscillator.
 * This function disables the PLLs.
 *
 * \note Switching sclk back to 32krc is only possible by shutting down the VDDIO power supply.
 *
 * \param ul_bypass 0 for Xtal, 1 for bypass.
 * \param ul_stup   0 for fast startup (52800 slow RC periods), 1 for normal startup (98400 slow RC periods).
 */
void supc_switch_sclk_to_32kxtal(Supc *p_supc, uint32_t ul_bypass, uint32_t ul_stup)
{
	/* Set Bypass mode if required */
	if (ul_bypass == 1) {
		p_supc->SUPC_MR |= SUPC_MR_KEY_PASSWD | SUPC_MR_OSCBYPASS;
	} else {
		p_supc->SUPC_MR &= ~(SUPC_MR_KEY_PASSWD | SUPC_MR_OSCBYPASS);
	}

	/* Set Fast Startup mode if required */
	if (ul_stup == 1) {
		p_supc->SUPC_MR |= SUPC_MR_KEY_PASSWD | SUPC_MR_FXTALSTUP;
	} else {
		p_supc->SUPC_MR &= ~(SUPC_MR_KEY_PASSWD | SUPC_MR_FXTALSTUP);
	}

	p_supc->SUPC_CR |= SUPC_CR_KEY_PASSWD | SUPC_CR_TDXTALSEL;
}

/**
 * \brief Set VDDIO Supply monitor threshold.
 *
 * \param p_supc Pointer to a SUPC instance.
 * \param ul_threshold Supply monitor threshold (between 1.9V and 3.4V).
 */
void supc_set_monitor_threshold(Supc *p_supc, supc_sm_thrshld_level_t ul_threshold)
{
	uint32_t ul_smmr = p_supc->SUPC_SMMR & (~SUPC_SMMR_IOSMTH_Msk);
	p_supc->SUPC_SMMR = ul_smmr | (SUPC_SMMR_IOSMTH_Msk & ul_threshold);
}

/**
 * \brief Set VDDIO Supply monitor sampling period.
 *
 * \param p_supc Pointer to a SUPC instance.
 * \param ul_period Supply monitor sampling period.
 */
void supc_set_monitor_sampling_period(Supc *p_supc, uint32_t ul_period)
{
	uint32_t ul_smmr = p_supc->SUPC_SMMR & (~SUPC_SMMR_IOSMSMPL_Msk);
	p_supc->SUPC_SMMR = ul_smmr | (SUPC_SMMR_IOSMSMPL_Msk & ul_period);
}

/**
 * \brief Enable the assertion of the core reset signal when a VDDIO supply monitor detection occurs.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_enable_monitor_reset(Supc *p_supc)
{
	p_supc->SUPC_SMMR |= SUPC_SMMR_IOSMRSTEN;
}

/**
 * \brief Disable the assertion of core reset signal when a VDDIO supply monitor detection occurs.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_disable_monitor_reset(Supc *p_supc)
{
	p_supc->SUPC_SMMR &= ~SUPC_SMMR_IOSMRSTEN;
}

/**
 * \brief Set VDDIO Supply monitor power supply mode.
 *
 * \param p_supc   Pointer to a SUPC instance.
 * \param ul_mode  0 for manual, 1 for auto.
 *
 * \note For manual mode, the VDDBU power source selection is controlled by configuring the bit
 * RSTC_MR.PWRSW. For auto mode, the VDDBU power source is VBAT when a VDDIO under voltage is detected.
 */
void supc_set_monitor_power_mode(Supc *p_supc, uint32_t ul_mode)
{
	if (ul_mode == 1) {
		p_supc->SUPC_SMMR |= SUPC_SMMR_IOSMPWRM;
	} else {
		p_supc->SUPC_SMMR &= ~SUPC_SMMR_IOSMPWRM;
	}
}

/**
 * \brief Enable the assertion of SUPC interrupt signal when VDDIO and VBAT supply monitor
 * detection occurs.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_enable_monitor_interrupt(Supc *p_supc)
{
	p_supc->SUPC_IER |= (SUPC_IER_IOSMEV | SUPC_IER_VBATSMEV);
}

/**
 * \brief Disable the assertion of SUPC interrupt signal when VDDIO and VBAT supply monitor
 * detection occurs.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_disable_monitor_interrupt(Supc *p_supc)
{
	p_supc->SUPC_IER &= ~(SUPC_IER_IOSMEV | SUPC_IER_VBATSMEV);
}



/**
 * \brief Get interrupt status.
 *
 * \param p_supc Pointer to a SUPC instance.
 * \return The status of supply controller interrupts
 */
uint32_t supc_get_interrupt_status(Supc *p_supc)
{
	return p_supc->SUPC_ISR;
}

/**
 * \brief Set LCD Voltage Regulator Output.
 *
 * \param p_supc Pointer to a SUPC instance.
 * \param vol  The voltage of Regulator Output.
 */
void supc_set_slcd_vol(Supc *p_supc, uint32_t vol)
{
	uint32_t tmp= p_supc->SUPC_MR;
	tmp &= ~SUPC_MR_LCDOUT_Msk;
	tmp |=  SUPC_MR_KEY_PASSWD |  SUPC_MR_LCDOUT(vol);
	p_supc->SUPC_MR = tmp;
}

/**
 * \brief Get SLCD power mode.
 *
 * \param p_supc Pointer to a SUPC instance.
 *
 * \return The mode of SLCDC.
 */
enum slcdc_power_mode supc_get_slcd_power_mode(Supc *p_supc)
{
	return (enum slcdc_power_mode)(p_supc->SUPC_MR & SUPC_MR_LCDMODE_Msk);
}

/**
 * \brief Set SLCD power mode.
 *
 * \param p_supc Pointer to a SUPC instance.
 * \param mode The mode of SLCDC.
 */
void supc_set_slcd_power_mode(Supc *p_supc, enum slcdc_power_mode mode)
{
	enum slcdc_power_mode pre_mode;
	uint32_t tmp;

	pre_mode = supc_get_slcd_power_mode(p_supc);

	if ((pre_mode == SLCDC_POWER_MODE_LCDON_EXTVR) && (mode == SLCDC_POWER_MODE_LCDON_INVR)) {
		return;
	} else if ((pre_mode == SLCDC_POWER_MODE_LCDON_INVR) && (mode == SLCDC_POWER_MODE_LCDON_EXTVR)) {
		return;
	}

	tmp = p_supc->SUPC_MR;
	tmp &= ~SUPC_MR_LCDMODE_Msk;
	tmp |=  SUPC_MR_KEY_PASSWD | mode;
	p_supc->SUPC_MR = tmp;

	if (mode == SLCDC_POWER_MODE_LCDOFF) {
		while(supc_get_status(p_supc) & SUPC_SR_LCDS);
	} else {
		while(!(supc_get_status(p_supc) & SUPC_SR_LCDS));
	}
}

/**
 * \brief Set VDDCORE monitor output mode.
 *
 * \param p_supc   Pointer to a SUPC instance.
 * \param ul_mode  0 for no effect, 1 to valid mode.
 *
 * \note For valid mode, VDDCORE supply monitor output value is checked to validate
 * the VDDCORE domain power-on.
 */
void supc_set_core_monitor_output_mode(Supc *p_supc, uint32_t ul_mode)
{
	if (ul_mode == 1) {
		p_supc->SUPC_MR |= SUPC_MR_CORSMM;
	} else {
		p_supc->SUPC_MR &= ~SUPC_MR_CORSMM;
	}
}

/**
 * \brief Enable the internal VDDCORE voltage regulator.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_enable_voltage_regulator(Supc *p_supc)
{
	uint32_t ul_mr = p_supc->SUPC_MR & (~(SUPC_MR_KEY_Msk | SUPC_MR_VREGDIS));
	p_supc->SUPC_MR = SUPC_MR_KEY_PASSWD1 | ul_mr;
}

/**
 * \brief Disable the internal voltage regulator to supply VDDCORE with an external supply.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_disable_voltage_regulator(Supc *p_supc)
{
	uint32_t ul_mr = p_supc->SUPC_MR & (~(SUPC_MR_KEY_Msk | SUPC_MR_VREGDIS));
	p_supc->SUPC_MR = SUPC_MR_KEY_PASSWD1 | ul_mr | SUPC_MR_VREGDIS;
}

/**
 * \brief Enable the assertion of the core reset signal when a VDDCORE supply monitor detection occurs.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_enable_core_monitor_reset(Supc *p_supc)
{
	uint32_t ul_mr = p_supc->SUPC_MR & (~(SUPC_MR_KEY_Msk | SUPC_MR_CORSMRSTEN));
	p_supc->SUPC_MR = SUPC_MR_KEY_PASSWD | ul_mr | SUPC_MR_CORSMRSTEN;
}

/**
 * \brief Disable the assertion of core reset signal when a VDDCORE supply monitor detection occurs.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_disable_core_monitor_reset(Supc *p_supc)
{
	uint32_t ul_mr = p_supc->SUPC_MR & (~(SUPC_MR_KEY_Msk | SUPC_MR_CORSMRSTEN));
	p_supc->SUPC_MR = SUPC_MR_KEY_PASSWD | ul_mr;
}

/**
 * \brief Enable the VDDCORE supply monitor.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_enable_core_monitor(Supc *p_supc)
{
	uint32_t ul_mr = p_supc->SUPC_MR & (~(SUPC_MR_KEY_Msk | SUPC_MR_CORSMDIS));
	p_supc->SUPC_MR = SUPC_MR_KEY_PASSWD | ul_mr | SUPC_MR_CORSMDIS;
}

/**
 * \brief Disable the VDDCORE supply monitor.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_disable_core_monitor(Supc *p_supc)
{
	uint32_t ul_mr = p_supc->SUPC_MR & (~(SUPC_MR_KEY_Msk | SUPC_MR_CORSMDIS));
	p_supc->SUPC_MR = SUPC_MR_KEY_PASSWD | ul_mr;
}

/**
 * \brief Enable the VDDIO supply monitor.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_enable_monitor(Supc *p_supc)
{
	uint32_t ul_mr = p_supc->SUPC_MR & (~(SUPC_MR_KEY_Msk | SUPC_MR_VDDIOSMEN));
	p_supc->SUPC_MR = SUPC_MR_KEY_PASSWD | ul_mr | SUPC_MR_VDDIOSMEN;
}

/**
 * \brief Disable the VDDIO supply monitor.
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_disable_monitor(Supc *p_supc)
{
	uint32_t ul_mr = p_supc->SUPC_MR & (~(SUPC_MR_KEY_Msk | SUPC_MR_VDDIOSMEN));
	p_supc->SUPC_MR = SUPC_MR_KEY_PASSWD | ul_mr;
}

/**
 * \brief Set system controller wake up mode.
 *
 * \param p_supc Pointer to a SUPC instance.
 * \param ul_mode Bitmask of wake up mode (please refer to datasheet for more details).
 */
void supc_set_wakeup_mode(Supc *p_supc, uint32_t ul_mode)
{
	p_supc->SUPC_WUMR = ul_mode;
}

/**
 * \brief Set system controller wake up inputs.
 *
 * \param p_supc Pointer to a SUPC instance.
 * \param ul_inputs Bitmask of wake-up inputs that can force wake up of
 * the core power supply.
 * \param ul_transition Bitmask of level transition of the wake-up inputs.
 * 1 means a high-to-low level transition forces the wake up of core power supply.
 * 0 means a low-to-high level transition forces the wake up of core power supply.
 */
void supc_set_wakeup_inputs(Supc *p_supc, uint32_t ul_inputs, uint32_t ul_transition)
{
	p_supc->SUPC_WUIR = ul_inputs | ul_transition;
}

/**
 * \brief Get supply controller status.
 *
 * \param p_supc Pointer to a SUPC instance.
 *
 * \return The status of supply controller.
 */
uint32_t supc_get_status(Supc *p_supc)
{
	return p_supc->SUPC_SR;
}

/**
 * \brief Check if VDDIO supply monitor output reports an invalid voltage.
 *
 * \param p_supc Pointer to a SUPC instance.
 *
 * \return True if it is invalid, false if it is valid.
 */
bool supc_monitor_voltage_is_invalid(Supc *p_supc)
{
	return ((p_supc->SUPC_SR & SUPC_SR_IOSMS)? true : false);
}

/**
 * \brief Enable the clearing of the GPBR registers when a Flash Erase occurs
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_enable_flash_erase_gpbr(Supc *p_supc)
{
	p_supc->SUPC_EMR |= SUPC_EMR_FLRSGPBR;
}

/**
 * \brief Disable the clearing of the GPBR registers when a Flash Erase occurs
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_disable_flash_erase_gpbr(Supc *p_supc)
{
	p_supc->SUPC_EMR &= ~SUPC_EMR_FLRSGPBR;
}

/**
 * \brief Enable the clearing of the full GPBR registers
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_enable_full_gpbr_clean(Supc *p_supc)
{
	p_supc->SUPC_EMR |= SUPC_EMR_FULLGPBRC;
}

/**
 * \brief Enable the clearing of the half GPBR registers
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_enable_half_gpbr_clean(Supc *p_supc)
{
	p_supc->SUPC_EMR &= ~SUPC_EMR_FULLGPBRC;
}

/**
 * \brief Enable the VDDCORE Voltage Regulator Bandgap
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_enable_voltage_regulator_bandgap(Supc *p_supc)
{
	p_supc->SUPC_EMR |= SUPC_EMR_COREBGEN;
}

/**
 * \brief Disable the VDDCORE Voltage Regulator Bandga
 *
 * \param p_supc Pointer to a SUPC instance.
 */
void supc_disable_voltage_regulator_bandgap(Supc *p_supc)
{
	p_supc->SUPC_EMR &= ~SUPC_EMR_COREBGEN;
}

/**
 * \brief Set system controller backup up mode.
 *
 * \param p_supc Pointer to a SUPC instance.
 * \param ul_mode Bitmask of backup mode (please refer to datasheet for more details).
 */
void supc_set_backup_mode(Supc *p_supc, uint32_t ul_mode)
{
	p_supc->SUPC_BMR = SUPC_BMR_KEY_PASSWD | ul_mode;
}

//@}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
