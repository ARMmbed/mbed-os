/**
 * \file
 *
 * \brief Parallel Input/Output (PIO) Controller driver for PIC32CX.
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

#include "pio.h"

#ifndef PIO_WPMR_WPKEY_PASSWD
#  define PIO_WPMR_WPKEY_PASSWD PIO_WPMR_WPKEY(0x50494Fu)
#endif

/**
 * \defgroup sam_drivers_pio_group Peripheral Parallel Input/Output (PIO) Controller
 *
 * \par Purpose
 *
 * The Parallel Input/Output Controller (PIO) manages up to 32 fully
 * programmable input/output lines. Each I/O line may be dedicated as a
 * general-purpose I/O or be assigned to a function of an embedded peripheral.
 * This assures effective optimization of the pins of a product.
 *
 * @{
 */

#ifndef FREQ_SLOW_CLOCK_EXT
/* External slow clock frequency (hz) */
#define FREQ_SLOW_CLOCK_EXT 32768
#endif

/**
 * \brief Configure PIO internal pull-up.
 *
 * \param p_pio_group Pointer to a PIO instance.
 * \param ul_mask Bitmask of one or more pin(s) to configure.
 * \param ul_pull_up_enable Indicates if the pin(s) internal pull-up shall be configured.
 */
void pio_pull_up(PioGroup *p_pio_group, const uint32_t ul_mask, const uint32_t ul_pull_up_enable)
{
	if (ul_pull_up_enable) {
		/* Enable Pull-up(s) */
		p_pio_group->PIO_MSKR = ul_mask;
		p_pio_group->PIO_CFGR |= PIO_CFGR_PUEN;
	} else {
		/* Disable Pull-up(s) */
		p_pio_group->PIO_MSKR = ul_mask;
		p_pio_group->PIO_CFGR &= ~PIO_CFGR_PUEN;
	}
}

/**
 * \brief Configure Glitch or Debouncing filter for the specified input(s).
 *
 * \param p_pio_group Pointer to a PIO instance.
 * \param ul_mask Bitmask of one or more pin(s) to configure.
 * \param ul_cut_off Cuts off frequency for debouncing filter
 */
void pio_set_debounce_filter(PioGroup *p_pio_group, const uint32_t ul_mask, const uint32_t ul_cut_off)
{
	Pio *p_pio;

	if (p_pio_group == PIOD) {
		p_pio = (Pio *)PIOD;
	} else {
		p_pio = (Pio *)PIOA;
	}

	/*
	 * The debouncing filter is able to filter pulses
	 * with a duration < tdiv_slck/2 for the selected I/O linesk
	 */
	p_pio_group->PIO_MSKR = ul_mask;
	p_pio_group->PIO_CFGR = PIO_CFGR_IFSCEN;

	/*
	 * The debouncing filter can filter a pulse of less than 1/2 Period of a
	 * programmable Divided Slow Clock:
	 * Tdiv_slclk = ((DIV+1)*2)*Tslow_clock
	 */
	p_pio->PIO_P_SCDR = PIO_P_SCDR_DIV((FREQ_SLOW_CLOCK_EXT / (2 * (ul_cut_off))) - 1);
}

/**
 * \brief Set a high output level on all the PIOs defined in ul_mask.
 * This has no immediate effects on PIOs that are not output, but the PIO
 * controller will save the value if they are changed to outputs.
 *
 * \param p_pio_group Pointer to a PIO instance.
 * \param ul_mask Bitmask of one or more pin(s) to configure.
 */
void pio_set(PioGroup *p_pio_group, const uint32_t ul_mask)
{
	p_pio_group->PIO_SODR = ul_mask;
}

/**
 * \brief Set a low output level on all the PIOs defined in ul_mask.
 * This has no immediate effects on PIOs that are not output, but the PIO
 * controller will save the value if they are changed to outputs.
 *
 * \param p_pio_group Pointer to a PIO instance.
 * \param ul_mask Bitmask of one or more pin(s) to configure.
 */
void pio_clear(PioGroup *p_pio_group, const uint32_t ul_mask)
{
	p_pio_group->PIO_CODR = ul_mask;
}

/**
 * \brief Return 1 if one or more PIOs of the given Pin instance currently have
 * a high level; otherwise returns 0. This method returns the actual value that
 * is being read on the pin. To return the supposed output value of a pin, use
 * pio_get_output_data_status() instead.
 *
 * \param p_pio_group Pointer to a PIO instance.
 * \param ul_type PIO type.
 * \param ul_mask Bitmask of one or more pin(s) to configure.
 *
 * \retval 1 at least one PIO currently has a high level.
 * \retval 0 all PIOs have a low level.
 */
uint32_t pio_get(PioGroup *p_pio_group, const pio_type_t ul_type, const uint32_t ul_mask)
{
	uint32_t ul_reg;

	if ((ul_type == PIO_OUTPUT_0) || (ul_type == PIO_OUTPUT_1)) {
		ul_reg = p_pio_group->PIO_ODSR;
	} else {
		ul_reg = p_pio_group->PIO_PDSR;
	}

	if ((ul_reg & ul_mask) == 0) {
		return 0;
	} else {
		return 1;
	}
}

/**
 * \brief Configure IO of a PIO controller as being controlled by a specific peripheral.
 *
 * \param p_pio_group Pointer to a PIO instance.
 * \param ul_type PIO type.
 * \param ul_mask Bitmask of one or more pin(s) to configure.
 */
void pio_set_peripheral(PioGroup *p_pio_group, const pio_type_t ul_type, const uint32_t ul_mask)
{
	/* Disable interrupts on the pin(s) */
	p_pio_group->PIO_IDR = ul_mask;

	/* Select mask */
	p_pio_group->PIO_MSKR = ul_mask;

	p_pio_group->PIO_CFGR &= ~PIO_CFGR_FUNC_Msk;
	switch (ul_type) {
	case PIO_PERIPH_A:
		p_pio_group->PIO_CFGR |= PIO_CFGR_FUNC_PERIPH_A;
		break;

	case PIO_PERIPH_B:
		p_pio_group->PIO_CFGR |= PIO_CFGR_FUNC_PERIPH_B;
		break;

	case PIO_PERIPH_C:
		p_pio_group->PIO_CFGR |= PIO_CFGR_FUNC_PERIPH_C;
		break;

	case PIO_PERIPH_D:
		p_pio_group->PIO_CFGR |= PIO_CFGR_FUNC_PERIPH_D;
		break;

	/* Other types are invalid in this function */
	case PIO_INPUT:
		p_pio_group->PIO_CFGR &= ~PIO_CFGR_DIR;
		break;

	case PIO_OUTPUT_0:
	case PIO_OUTPUT_1:
		p_pio_group->PIO_CFGR |= PIO_CFGR_DIR;
		break;

	case PIO_NOT_A_PIN:
		return;
	}
}

/**
 * \brief Configure one or more pin(s) or a PIO controller as inputs.
 * Optionally, the corresponding internal pull-up(s) and glitch filter(s) can
 * be enabled.
 *
 * \param p_pio_group Pointer to a PIO instance.
 * \param ul_mask Bitmask indicating which pin(s) to configure as input(s).
 * \param ul_attribute PIO attribute(s).
 */
void pio_set_input(PioGroup *p_pio_group, const uint32_t ul_mask, const uint32_t ul_attribute)
{
	uint32_t ul_cfgr;

	pio_disable_interrupt(p_pio_group, ul_mask);
	pio_pull_up(p_pio_group, ul_mask, ul_attribute & PIO_PULLUP);

	/* Select mask */
	p_pio_group->PIO_MSKR = ul_mask;

	/* Read current configuration */
	ul_cfgr = p_pio_group->PIO_CFGR;

	/* Set Input */
	ul_cfgr &= ~PIO_CFGR_FUNC_Msk;
	ul_cfgr &= ~PIO_CFGR_DIR;

	/* Enable de-glitch or de-bounce if necessary */
	if (ul_attribute & PIO_DEGLITCH) {
		ul_cfgr &= ~PIO_CFGR_IFSCEN;
		ul_cfgr |= PIO_CFGR_IFEN;
	} else if (ul_attribute & PIO_DEBOUNCE) {
		ul_cfgr &= ~PIO_CFGR_IFEN;
		ul_cfgr |= PIO_CFGR_IFSCEN;
	}

	/* Set configuration */
	p_pio_group->PIO_CFGR = ul_cfgr;
}

/**
 * \brief Configure one or more pin(s) of a PIO controller as outputs, with
 * the given default value. Optionally, the multi-drive feature can be enabled
 * on the pin(s).
 *
 * \param p_pio_group Pointer to a PIO instance.
 * \param ul_mask Bitmask indicating which pin(s) to configure.
 * \param ul_default_level Default level on the pin(s).
 * \param ul_multidrive_enable Indicates if the pin(s) shall be configured as
 * open-drain.
 * \param ul_pull_up_enable Indicates if the pin shall have its pull-up
 * activated.
 */
void pio_set_output(PioGroup *p_pio_group, const uint32_t ul_mask,
		const uint32_t ul_default_level,
		const uint32_t ul_multidrive_enable,
		const uint32_t ul_pull_up_enable)
{
	uint32_t ul_cfgr;

	pio_disable_interrupt(p_pio_group, ul_mask);
	pio_pull_up(p_pio_group, ul_mask, ul_pull_up_enable);

	/* Select mask */
	p_pio_group->PIO_MSKR = ul_mask;

	/* Read current configuration */
	ul_cfgr = p_pio_group->PIO_CFGR;

	/* Set Output */
	ul_cfgr &= ~PIO_CFGR_FUNC_Msk;
	ul_cfgr |= PIO_CFGR_DIR;

	/* Enable multi-drive if necessary */
	if (ul_multidrive_enable) {
		ul_cfgr |= PIO_CFGR_OPD;
	} else {
		ul_cfgr &= ~PIO_CFGR_OPD;
	}

	/* Set default value */
	if (ul_default_level) {
		p_pio_group->PIO_SODR = ul_mask;
	} else {
		p_pio_group->PIO_CODR = ul_mask;
	}

	/* Set configuration */
	p_pio_group->PIO_CFGR = ul_cfgr;
}

/**
 * \brief Perform complete pin(s) configuration; general attributes and PIO init
 * if necessary.
 *
 * \param p_pio_group Pointer to a PIO instance.
 * \param ul_type PIO type.
 * \param ul_mask Bitmask of one or more pin(s) to configure.
 * \param ul_attribute Pins attributes.
 *
 * \return Whether the pin(s) have been configured properly.
 */
uint32_t pio_configure(PioGroup *p_pio_group, const pio_type_t ul_type,
		const uint32_t ul_mask, const uint32_t ul_attribute)
{
	/* Configure pins */
	switch (ul_type) {
	case PIO_PERIPH_A:
	case PIO_PERIPH_B:
	case PIO_PERIPH_C:
	case PIO_PERIPH_D:
		pio_set_peripheral(p_pio_group, ul_type, ul_mask);
		pio_pull_up(p_pio_group, ul_mask, (ul_attribute & PIO_PULLUP));
		break;

	case PIO_INPUT:
		pio_set_input(p_pio_group, ul_mask, ul_attribute);
		break;

	case PIO_OUTPUT_0:
	case PIO_OUTPUT_1:
		pio_set_output(p_pio_group, ul_mask, (ul_type == PIO_OUTPUT_1),
				(ul_attribute & PIO_OPENDRAIN) ? 1 : 0,
				(ul_attribute & PIO_PULLUP) ? 1 : 0);
		break;

	default:
		return 0;
	}

	return 1;
}

/**
 * \brief Return 1 if one or more PIOs of the given Pin are configured to
 * output a high level (even if they are not output).
 * To get the actual value of the pin, use PIO_Get() instead.
 *
 * \param p_pio_group Pointer to a PIO instance.
 * \param ul_mask Bitmask of one or more pin(s).
 *
 * \retval 1 At least one PIO is configured to output a high level.
 * \retval 0 All PIOs are configured to output a low level.
 */
uint32_t pio_get_output_data_status(const PioGroup *p_pio_group, const uint32_t ul_mask)
{
	if ((p_pio_group->PIO_ODSR & ul_mask) == 0) {
		return 0;
	} else {
		return 1;
	}
}

/**
 * \brief Get Lock status.
 *
 * \param p_pio Pointer to a PIO instance.
 *
 * \return The lock status mask value.
 */
uint32_t pio_get_lock_status(const PioGroup *p_pio_group)
{
	return p_pio_group->PIO_LOCKSR;
}

/**
 * \brief Configure PIO pin multi-driver.
 *
 * \param p_pio_group Pointer to a PIO instance.
 * \param ul_mask Bitmask of one or more pin(s) to configure.
 * \param ul_multi_driver_enable Indicates if the pin(s) multi-driver shall be configured.
 */
void pio_set_multi_driver(PioGroup *p_pio_group, const uint32_t ul_mask, const uint32_t ul_multi_driver_enable)
{
	/* Select mask */
	p_pio_group->PIO_MSKR = ul_mask;

	/* Enable the multi-driver if necessary */
	if (ul_multi_driver_enable) {
		p_pio_group->PIO_CFGR |= PIO_CFGR_OPD;
	} else {
		p_pio_group->PIO_CFGR &= ~PIO_CFGR_OPD;
	}
}

/**
 * \brief Configure PIO pin internal pull-down.
 *
 * \param p_pio_group Pointer to a PIO instance.
 * \param ul_mask Bitmask of one or more pin(s) to configure.
 * \param ul_pull_down_enable Indicates if the pin(s) internal pull-down shall
 * be configured.
 */
void pio_pull_down(PioGroup *p_pio_group, const uint32_t ul_mask, const uint32_t ul_pull_down_enable)
{
	/* Select mask */
	p_pio_group->PIO_MSKR = ul_mask;

	/* Enable the pull-down if necessary */
	if (ul_pull_down_enable) {
		/* Disable Pull-up */
		p_pio_group->PIO_CFGR &= ~PIO_CFGR_PUEN;
		/* Enable Pull-down */
		p_pio_group->PIO_CFGR |= PIO_CFGR_PDEN;
	} else {
		p_pio_group->PIO_CFGR &= ~PIO_CFGR_PDEN;
	}
}

/**
 * \brief Synchronously write on output pins.
 * \note Only bits unmasked by PIO_OWSR (Output Write Status Register) are
 * written.
 *
 * \param p_pio_group Pointer to a PIO instance.
 * \param ul_mask Bitmask of one or more pin(s) to configure.
 */
void pio_sync_output_write(PioGroup *p_pio_group, const uint32_t ul_mask)
{
	p_pio_group->PIO_ODSR = ul_mask;
}

/**
 * \brief Configure PIO pin schmitt trigger. By default the Schmitt trigger is
 * active.
 * Disabling the Schmitt Trigger is requested when using the QTouch Library.
 *
 * \param p_pio_group Pointer to a PIO instance.
 * \param ul_mask Bitmask of one or more pin(s) to configure.
 */
void pio_set_schmitt_trigger(PioGroup *p_pio_group, const uint32_t ul_mask)
{
	/* Select mask */
	p_pio_group->PIO_MSKR = ul_mask;

	p_pio_group->PIO_CFGR &= ~PIO_CFGR_SCHMITT;
}

/**
 * \brief Configure the given interrupt source.
 * Interrupt can be configured to trigger on rising edge, falling edge,
 * high level, low level or simply on level change.
 *
 * \param p_pio_group Pointer to a PIO instance.
 * \param ul_mask Interrupt source bit map.
 * \param ul_attr Interrupt source attributes.
 */
void pio_configure_interrupt(PioGroup *p_pio_group, const uint32_t ul_mask, const uint32_t ul_attr)
{
	/* Select mask */
	p_pio_group->PIO_MSKR = ul_mask;

	/* Event selection */
	switch (ul_attr) {
	case PIO_IT_FALLING:
		p_pio_group->PIO_CFGR &= ~PIO_CFGR_EVTSEL_Msk;
		p_pio_group->PIO_CFGR |= PIO_CFGR_EVTSEL_FALLING;
		break;

	case PIO_IT_RISING:
		p_pio_group->PIO_CFGR &= ~PIO_CFGR_EVTSEL_Msk;
		p_pio_group->PIO_CFGR |= PIO_CFGR_EVTSEL_RISING;
		break;

	case PIO_IT_BOTH:
		p_pio_group->PIO_CFGR &= ~PIO_CFGR_EVTSEL_Msk;
		p_pio_group->PIO_CFGR |= PIO_CFGR_EVTSEL_BOTH;
		break;

	case PIO_IT_LOW:
		p_pio_group->PIO_CFGR &= ~PIO_CFGR_EVTSEL_Msk;
		p_pio_group->PIO_CFGR |= PIO_CFGR_EVTSEL_LOW;
		break;

	case PIO_IT_HIGH:
		p_pio_group->PIO_CFGR &= ~PIO_CFGR_EVTSEL_Msk;
		p_pio_group->PIO_CFGR |= PIO_CFGR_EVTSEL_HIGH;
		break;

	default:
		break;
	}
}

/**
 * \brief Enable the given interrupt source.
 * The PIO must be configured as an NVIC interrupt source as well.
 *
 * \param p_pio_group Pointer to a PIO instance.
 * \param ul_mask Interrupt sources bit map.
 */
void pio_enable_interrupt(PioGroup *p_pio_group, const uint32_t ul_mask)
{
	p_pio_group->PIO_IER = ul_mask;
}

/**
 * \brief Disable a given interrupt source, with no added side effects.
 *
 * \param p_pio_group Pointer to a PIO instance.
 * \param ul_mask Interrupt sources bit map.
 */
void pio_disable_interrupt(PioGroup *p_pio_group, const uint32_t ul_mask)
{
	p_pio_group->PIO_IDR = ul_mask;
}

/**
 * \brief Read and clear PIO interrupt status.
 *
 * \param p_pio_group Pointer to a PIO instance.
 *
 * \return The interrupt status value.
 */
uint32_t pio_get_interrupt_status(const PioGroup *p_pio_group)
{
	return p_pio_group->PIO_ISR;
}

/**
 * \brief Read PIO interrupt mask.
 *
 * \param p_pio_group Pointer to a PIO instance.
 *
 * \return The interrupt mask value.
 */
uint32_t pio_get_interrupt_mask(const PioGroup *p_pio_group)
{
	return p_pio_group->PIO_IMR;
}

/**
 * \brief Freeze Physical Configuration.
 *
 * \param p_pio_group Pointer to an PIO peripheral.
 * \param ul_mask Bitmask of one or more pin(s).
 *
 * \note Only a hardware reset can release the configuration
 */
void pio_set_physical_freeze(PioGroup *p_pio_group, const uint32_t ul_mask)
{
	p_pio_group->PIO_MSKR = ul_mask;
	p_pio_group->PIO_IOFR = (PIO_IOFR_FRZKEY_PASSWD | PIO_IOFR_FPHY);
}

/**
 * \brief Freeze Interrupt Configuration.
 *
 * \param p_pio_group Pointer to an PIO peripheral.
 * \param ul_mask Bitmask of one or more pin(s).
 *
 * \note Only a hardware reset can release the configuration
 */
void pio_set_interrupt_freeze(PioGroup *p_pio_group, const uint32_t ul_mask)
{
	p_pio_group->PIO_MSKR = ul_mask;
	p_pio_group->PIO_IOFR = (PIO_IOFR_FRZKEY_PASSWD | PIO_IOFR_FINT);
}

/**
 * \brief Enable or disable write protect of PIO registers.
 *
 * \param p_pio_group Pointer to a PIO instance.
 * \param ul_enable 1 to enable, 0 to disable.
 */
void pio_set_writeprotect(PioGroup *p_pio_group, const uint32_t ul_enable)
{
	Pio *p_pio;

	if (p_pio_group == PIOD) {
		p_pio = (Pio *)PIOD;
	} else {
		p_pio = (Pio *)PIOA;
	}

	p_pio->PIO_WPMR = PIO_WPMR_WPKEY_PASSWD | (ul_enable & PIO_WPMR_WPEN);
}

/**
 * \brief Read write protect status.
 *
 * \param p_pio_group Pointer to a PIO instance.
 *
 * \return Return write protect status.
 */
uint32_t pio_get_writeprotect_status(const PioGroup *p_pio_group)
{
	Pio *p_pio;

	if (p_pio_group == PIOD) {
		p_pio = (Pio *)PIOD;
	} else {
		p_pio = (Pio *)PIOA;
	}

	return p_pio->PIO_WPSR;
}

/**
 * \brief Enable or disable write protect of PIO interrupt registers.
 *
 * \param p_pio_group Pointer to a PIO instance.
 * \param ul_enable 1 to enable, 0 to disable.
 */
void pio_set_interrupt_writeprotect(PioGroup *p_pio_group, const uint32_t ul_enable)
{
	Pio *p_pio;

	if (p_pio_group == PIOD) {
		p_pio = (Pio *)PIOD;
	} else {
		p_pio = (Pio *)PIOA;
	}

	p_pio->PIO_WPMR = PIO_WPMR_WPKEY_PASSWD | (ul_enable & PIO_WPMR_WPITEN);
}

/**
 * \brief Return the value of a pin.
 *
 * \param ul_pin The pin number.
 *
 * \return The pin value.
 *
 * \note If pin is output: a pull-up or pull-down could hide the actual value.
 *       The function \ref pio_get can be called to get the actual pin output
 *       level.
 * \note If pin is input: PIOx must be clocked to sample the signal.
 *       See PMC driver.
 */
uint32_t pio_get_pin_value(uint32_t ul_pin)
{
	PioGroup *p_pio_group = pio_get_pin_group(ul_pin);

	return (p_pio_group->PIO_PDSR >> (ul_pin & 0x1F)) & 1;
}

/**
 * \brief Drive a GPIO pin to 1.
 *
 * \param ul_pin The pin index.
 *
 * \note The function \ref pio_configure_pin must be called beforehand.
 */
void pio_set_pin_high(uint32_t ul_pin)
{
	PioGroup *p_pio_group = pio_get_pin_group(ul_pin);

	/* Value to be driven on the I/O line: 1. */
	p_pio_group->PIO_SODR = 1 << (ul_pin & 0x1F);
}

/**
 * \brief Drive a GPIO pin to 0.
 *
 * \param ul_pin The pin index.
 *
 * \note The function \ref pio_configure_pin must be called before.
 */
void pio_set_pin_low(uint32_t ul_pin)
{
	PioGroup *p_pio_group = pio_get_pin_group(ul_pin);

	/* Value to be driven on the I/O line: 0. */
	p_pio_group->PIO_CODR = 1 << (ul_pin & 0x1F);
}

/**
 * \brief Toggle a GPIO pin.
 *
 * \param ul_pin The pin index.
 *
 * \note The function \ref pio_configure_pin must be called before.
 */
void pio_toggle_pin(uint32_t ul_pin)
{
	PioGroup *p_pio_group = pio_get_pin_group(ul_pin);

	if (p_pio_group->PIO_ODSR & (1 << (ul_pin & 0x1F))) {
		/* Value to be driven on the I/O line: 0. */
		p_pio_group->PIO_CODR = 1 << (ul_pin & 0x1F);
	} else {
		/* Value to be driven on the I/O line: 1. */
		p_pio_group->PIO_SODR = 1 << (ul_pin & 0x1F);
	}
}

/**
 * \brief Perform complete pin(s) configuration; general attributes and PIO init
 * if necessary.
 *
 * \param ul_pin The pin index.
 * \param ul_flags Pins attributes.
 *
 * \return Whether the pin(s) have been configured properly.
 */
uint32_t pio_configure_pin(uint32_t ul_pin, const uint32_t ul_flags)
{
	PioGroup *p_pio_group = pio_get_pin_group(ul_pin);

	/* Configure pins */
	switch (ul_flags & PIO_TYPE_Msk) {
	case PIO_TYPE_PIO_PERIPH_A:
		pio_set_peripheral(p_pio_group, PIO_PERIPH_A, (1 << (ul_pin & 0x1F)));
		pio_pull_up(p_pio_group, (1 << (ul_pin & 0x1F)), (ul_flags & PIO_PULLUP));
		break;

	case PIO_TYPE_PIO_PERIPH_B:
		pio_set_peripheral(p_pio_group, PIO_PERIPH_B, (1 << (ul_pin & 0x1F)));
		pio_pull_up(p_pio_group, (1 << (ul_pin & 0x1F)), (ul_flags & PIO_PULLUP));
		break;

	case PIO_TYPE_PIO_PERIPH_C:
		pio_set_peripheral(p_pio_group, PIO_PERIPH_C, (1 << (ul_pin & 0x1F)));
		pio_pull_up(p_pio_group, (1 << (ul_pin & 0x1F)), (ul_flags & PIO_PULLUP));
		break;

	case PIO_TYPE_PIO_PERIPH_D:
		pio_set_peripheral(p_pio_group, PIO_PERIPH_D, (1 << (ul_pin & 0x1F)));
		pio_pull_up(p_pio_group, (1 << (ul_pin & 0x1F)), (ul_flags & PIO_PULLUP));
		break;

	case PIO_TYPE_PIO_INPUT:
		pio_set_input(p_pio_group, (1 << (ul_pin & 0x1F)), ul_flags);
		break;

	case PIO_TYPE_PIO_OUTPUT_0:
	case PIO_TYPE_PIO_OUTPUT_1:
		pio_set_output(p_pio_group, (1 << (ul_pin & 0x1F)),
				((ul_flags & PIO_TYPE_PIO_OUTPUT_1)
				== PIO_TYPE_PIO_OUTPUT_1) ? 1 : 0,
				(ul_flags & PIO_OPENDRAIN) ? 1 : 0,
				(ul_flags & PIO_PULLUP) ? 1 : 0);
		break;

	default:
		return 0;
	}

	return 1;
}

/**
 * \brief Drive a GPIO port to 1.
 *
 * \param p_pio_group Base address of the PIO port.
 * \param ul_mask Bitmask of one or more pin(s) to toggle.
 */
void pio_set_pin_group_high(PioGroup *p_pio_group, uint32_t ul_mask)
{
	/* Value to be driven on the I/O line: 1. */
	p_pio_group->PIO_SODR = ul_mask;
}

/**
 * \brief Drive a GPIO port to 0.
 *
 * \param p_pio_group Base address of the PIO port.
 * \param ul_mask Bitmask of one or more pin(s) to toggle.
 */
void pio_set_pin_group_low(PioGroup *p_pio_group, uint32_t ul_mask)
{
	/* Value to be driven on the I/O line: 0. */
	p_pio_group->PIO_CODR = ul_mask;
}

/**
 * \brief Toggle a GPIO group.
 *
 * \param p_pio_group Pointer to a PIO instance.
 * \param ul_mask Bitmask of one or more pin(s) to configure.
 */
void pio_toggle_pin_group(PioGroup *p_pio_group, uint32_t ul_mask)
{
	if (p_pio_group->PIO_ODSR & ul_mask) {
		/* Value to be driven on the I/O line: 0. */
		p_pio_group->PIO_CODR = ul_mask;
	} else {
		/* Value to be driven on the I/O line: 1. */
		p_pio_group->PIO_SODR = ul_mask;
	}
}

/**
 * \brief Perform complete pin(s) configuration; general attributes and PIO init
 * if necessary.
 *
 * \param p_pio_group Pointer to a PIO instance.
 * \param ul_mask Bitmask of one or more pin(s) to configure.
 * \param ul_flags Pin(s) attributes.
 *
 * \return Whether the pin(s) have been configured properly.
 */
uint32_t pio_configure_pin_group(PioGroup *p_pio_group,	uint32_t ul_mask, const uint32_t ul_flags)
{
	/* Configure pins */
	switch (ul_flags & PIO_TYPE_Msk) {
	case PIO_TYPE_PIO_PERIPH_A:
		pio_set_peripheral(p_pio_group, PIO_PERIPH_A, ul_mask);
		pio_pull_up(p_pio_group, ul_mask, (ul_flags & PIO_PULLUP));
		break;

	case PIO_TYPE_PIO_PERIPH_B:
		pio_set_peripheral(p_pio_group, PIO_PERIPH_B, ul_mask);
		pio_pull_up(p_pio_group, ul_mask, (ul_flags & PIO_PULLUP));
		break;

	case PIO_TYPE_PIO_PERIPH_C:
		pio_set_peripheral(p_pio_group, PIO_PERIPH_C, ul_mask);
		pio_pull_up(p_pio_group, ul_mask, (ul_flags & PIO_PULLUP));
		break;

	case PIO_TYPE_PIO_PERIPH_D:
		pio_set_peripheral(p_pio_group, PIO_PERIPH_D, ul_mask);
		pio_pull_up(p_pio_group, ul_mask, (ul_flags & PIO_PULLUP));
		break;

	case PIO_TYPE_PIO_INPUT:
		pio_set_input(p_pio_group, ul_mask, ul_flags);
		break;

	case PIO_TYPE_PIO_OUTPUT_0:
	case PIO_TYPE_PIO_OUTPUT_1:
		pio_set_output(p_pio_group, ul_mask,
				((ul_flags & PIO_TYPE_PIO_OUTPUT_1)
				== PIO_TYPE_PIO_OUTPUT_1) ? 1 : 0,
				(ul_flags & PIO_OPENDRAIN) ? 1 : 0,
				(ul_flags & PIO_PULLUP) ? 1 : 0);
		break;

	default:
		return 0;
	}

	return 1;
}

/**
 * \brief Enable interrupt for a GPIO pin.
 *
 * \param ul_pin The pin index.
 *
 * \note The function \ref gpio_configure_pin must be called before.
 */
void pio_enable_pin_interrupt(uint32_t ul_pin)
{
	PioGroup *p_pio_group = pio_get_pin_group(ul_pin);

	p_pio_group->PIO_IER = 1 << (ul_pin & 0x1F);
}

/**
 * \brief Disable interrupt for a GPIO pin.
 *
 * \param ul_pin The pin index.
 *
 * \note The function \ref gpio_configure_pin must be called before.
 */
void pio_disable_pin_interrupt(uint32_t ul_pin)
{
	PioGroup *p_pio_group = pio_get_pin_group(ul_pin);

	p_pio_group->PIO_IDR = 1 << (ul_pin & 0x1F);
}

/**
 * \brief Return GPIO port for a GPIO pin.
 *
 * \param ul_pin The pin index.
 *
 * \return Pointer to \ref Pio struct for GPIO port. NULL
 */
PioGroup *pio_get_pin_group(uint32_t ul_pin)
{
	PioGroup *p_pio_group = 0;
	uint8_t uc_group_idx;

	uc_group_idx = ul_pin >> 5;

	if (uc_group_idx == 0) {
		p_pio_group = PIOA;
	} else if (uc_group_idx == 1) {
		p_pio_group = PIOB;
	} else if (uc_group_idx == 2) {
		p_pio_group = PIOC;
	}
#ifdef ID_PIOD
	else if (uc_group_idx == 3) {
		p_pio_group = PIOD;
	}
#endif

	return p_pio_group;
}

/**
 * \brief Return GPIO port peripheral ID for a GPIO pin.
 *
 * \param ul_pin The pin index.
 *
 * \return GPIO port peripheral ID.
 */
uint32_t pio_get_pin_group_id(uint32_t ul_pin)
{
	uint32_t ul_id = 0;
	uint8_t uc_group_idx;

	uc_group_idx = ul_pin >> 5;

	if (uc_group_idx == 0) {
		ul_id = ID_PIOA;
	} else if (uc_group_idx == 1) {
		ul_id = ID_PIOB;
	} else if (uc_group_idx == 2) {
		ul_id = ID_PIOC;
	}
#ifdef ID_PIOD
	else if (uc_group_idx == 3) {
		ul_id = ID_PIOD;
	}
#endif

	return ul_id;
}

/**
 * \brief Return GPIO port pin mask for a GPIO pin.
 *
 * \param ul_pin The pin index.
 *
 * \return GPIO port pin mask.
 */
uint32_t pio_get_pin_group_mask(uint32_t ul_pin)
{
	uint32_t ul_mask = 1 << (ul_pin & 0x1F);
	return ul_mask;
}

/* @} */
