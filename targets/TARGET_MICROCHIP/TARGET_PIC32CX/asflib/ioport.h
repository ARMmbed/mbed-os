/**
 * \file
 *
 * \brief Common IOPORT service main header file for AVR, UC3, ARM
 *        and PIC32CX architectures.
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
#ifndef IOPORT_H
#define IOPORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <compiler.h>
#include "sysclk.h"

/**
 * \defgroup ioport_group Common IOPORT API
 *
 * See \ref ioport_quickstart.
 *
 * This is common IOPORT service for GPIO pin configuration and control in a
 * standardized manner across the MEGA, MEGA_RF, XMEGA, UC3 and ARM devices.
 *
 * Port pin control code is optimized for each platform, and should produce
 * both compact and fast execution times when used with constant values.
 *
 * \section dependencies Dependencies
 * This driver depends on the following modules:
 * - \ref sysclk_group for clock speed and functions.
 * @{
 */

/**
 * \def IOPORT_CREATE_PIN(port, pin)
 * \brief Create IOPORT pin number
 *
 * Create a IOPORT pin number for use with the IOPORT functions.
 *
 * \param port IOPORT port (e.g. PORTA, PA or PIOA depending on chosen
 *             architecture)
 * \param pin IOPORT zero-based index of the I/O pin
 */
 
typedef uint32_t ioport_mode_t;
typedef uint32_t ioport_pin_t;
typedef uint32_t ioport_port_t;
typedef uint32_t ioport_port_mask_t;

#define IOPORT_PIOA     0
#define IOPORT_PIOB     1
#define IOPORT_PIOC     2
#define IOPORT_PIOD     3

/** \name IOPORT Mode bit definitions */
/** @{ */
#define IOPORT_MODE_MUX_MASK            (0x7 << 0) /*!< MUX bits mask */

#define IOPORT_MODE_MUX_A               (1 << 0)   /*!< MUX function A */
#define IOPORT_MODE_MUX_B               (2 << 0)   /*!< MUX function B */
#define IOPORT_MODE_MUX_C               (3 << 0)   /*!< MUX function C */
#define IOPORT_MODE_MUX_D               (4 << 0)   /*!< MUX function D */

#define IOPORT_MODE_DEFAULT             (0)
#define IOPORT_MODE_PULLUP              (1 << 3)   /*!< Pull-up */
#define IOPORT_MODE_PULLDOWN            (1 << 4)   /*!< Pull-down */
#define IOPORT_MODE_OPEN_DRAIN          (1 << 5)   /*!< Open drain */
#define IOPORT_MODE_GLITCH_FILTER       (1 << 6)   /*!< Glitch filter */
#define IOPORT_MODE_DEBOUNCE            (1 << 7)   /*!< Input debounce */
/** @} */

/** \brief IOPORT pin directions */
enum ioport_direction {
	IOPORT_DIR_INPUT,  /*!< IOPORT input direction */
	IOPORT_DIR_OUTPUT, /*!< IOPORT output direction */
};

/** \brief IOPORT levels */
enum ioport_value {
	IOPORT_PIN_LEVEL_LOW,  /*!< IOPORT pin value low */
	IOPORT_PIN_LEVEL_HIGH, /*!< IOPORT pin value high */
};

/** \brief IOPORT edge sense modes */
enum ioport_sense {
	IOPORT_SENSE_BOTHEDGES, /*!< IOPORT sense both rising and falling edges */
	IOPORT_SENSE_FALLING,   /*!< IOPORT sense falling edges */
	IOPORT_SENSE_RISING,    /*!< IOPORT sense rising edges */
	IOPORT_SENSE_LEVEL_LOW, /*!< IOPORT sense low level  */
	IOPORT_SENSE_LEVEL_HIGH, /*!< IOPORT sense High level  */
};

__always_inline ioport_port_t ioport_pin_to_port_id(ioport_pin_t pin)
{
	return pin >> 5;
}

__always_inline ioport_port_mask_t ioport_pin_to_mask(ioport_pin_t pin)
{
	return 1U << (pin & 0x1F);
}

__always_inline PioGroup *ioport_port_to_base(ioport_port_t port)
{
	if (port == IOPORT_PIOA) {
		return (PioGroup *)(uintptr_t)PIOA;
	} else if (port == IOPORT_PIOB) {
		return (PioGroup *)(uintptr_t)PIOB;
	} else if (port == IOPORT_PIOC) {
		return (PioGroup *)(uintptr_t)PIOC;
	} else if (port == IOPORT_PIOD) {
		return (PioGroup *)(uintptr_t)PIOD;
	}

	return 0;
}

__always_inline PioGroup *ioport_pin_to_base(ioport_pin_t pin)
{
	return ioport_port_to_base(ioport_pin_to_port_id(pin));
}

/**
 * \brief Initializes the IOPORT service, ready for use.
 *
 * This function must be called before using any other functions in the IOPORT
 * service.
 */
static __always_inline void ioport_init(ioport_pin_t pin)
{
	ioport_port_t port = ioport_pin_to_port_id(pin);
	
	if (port == IOPORT_PIOA) {
		sysclk_enable_peripheral_clock(ID_PIOA);
	} else if (port == IOPORT_PIOB) {
		sysclk_enable_peripheral_clock(ID_PIOB);
	} else if (port == IOPORT_PIOC) {
		sysclk_enable_peripheral_clock(ID_PIOC);
	} else if (port == IOPORT_PIOD) {
		sysclk_enable_peripheral_clock(ID_PIOD);
	}
}

/**
 * \brief Set multiple pin modes in a single IOPORT port, such as pull-up,
 * pull-down, etc. configuration.
 *
 * \param port IOPORT port to configure
 * \param mask Pin mask of pins to configure
 * \param mode Mode masks to configure for the specified pins (\ref
 * ioport_modes)
 */
static __always_inline void ioport_set_port_mode(ioport_port_t port,
		ioport_port_mask_t mask, ioport_mode_t mode)
{
	PioGroup *pio = ioport_port_to_base(port);
	uint32_t ul_cfgr = 0;

	/* Select mask */
	pio->PIO_MSKR = mask;

	if (mode & IOPORT_MODE_PULLUP) {
		ul_cfgr |= PIO_CFGR_PUEN;
	}

	if (mode & IOPORT_MODE_PULLDOWN) {
		ul_cfgr |= PIO_CFGR_PDEN;
	}

	if (mode & IOPORT_MODE_OPEN_DRAIN) {
		ul_cfgr |= PIO_CFGR_OPD;
	}

	if (mode & (IOPORT_MODE_GLITCH_FILTER)) {
		ul_cfgr |= PIO_CFGR_IFEN;
	}

	if (mode & (IOPORT_MODE_DEBOUNCE)) {
		ul_cfgr |= PIO_CFGR_IFSCEN;
	}

	if ((mode & IOPORT_MODE_MUX_MASK) == IOPORT_MODE_MUX_A) {
		ul_cfgr |= PIO_CFGR_FUNC_PERIPH_A;
	} else if ((mode & IOPORT_MODE_MUX_MASK) == IOPORT_MODE_MUX_B) {
		ul_cfgr |= PIO_CFGR_FUNC_PERIPH_B;
	} else if ((mode & IOPORT_MODE_MUX_MASK) == IOPORT_MODE_MUX_C) {
		ul_cfgr |= PIO_CFGR_FUNC_PERIPH_C;
	}	else if ((mode & IOPORT_MODE_MUX_MASK) == IOPORT_MODE_MUX_D) {
		ul_cfgr |= PIO_CFGR_FUNC_PERIPH_D;
	}

	/* Set configuration */
	pio->PIO_CFGR = ul_cfgr;
}

/**
 * \brief Set pin mode for one single IOPORT pin.
 *
 * \param pin IOPORT pin to configure
 * \param mode Mode masks to configure for the specified pin (\ref ioport_modes)
 */
static __always_inline void ioport_set_pin_mode(ioport_pin_t pin, ioport_mode_t mode)
{
	ioport_set_port_mode(ioport_pin_to_port_id(pin), ioport_pin_to_mask(pin), mode);
}

/**
 * \brief Reset multiple pin modes in a specified IOPORT port to defaults.
 *
 * \param port IOPORT port to configure
 * \param mask Mask of pins whose mode configuration is to be reset
 */
static __always_inline void ioport_reset_port_mode(ioport_port_t port,
		ioport_port_mask_t mask)
{
	ioport_set_port_mode(port, mask, 0);
}

/**
 * \brief Reset pin mode configuration for a single IOPORT pin
 *
 * \param pin IOPORT pin to configure
 */
static __always_inline void ioport_reset_pin_mode(ioport_pin_t pin)
{
	ioport_set_pin_mode(pin, 0);
}

/**
 * \brief Set I/O direction for a group of pins in a single IOPORT.
 *
 * \param port IOPORT port to configure
 * \param mask Pin mask of pins to configure
 * \param dir Direction to set for the specified pins (\ref ioport_direction)
 */
static __always_inline void ioport_set_port_dir(ioport_port_t port,
		ioport_port_mask_t mask, enum ioport_direction dir)
{
    PioGroup *pio = ioport_port_to_base(port);

	/* Select mask */
	pio->PIO_MSKR = mask;

	if (dir == IOPORT_DIR_OUTPUT) {
		pio->PIO_CFGR |= PIO_CFGR_DIR;
	} else if (dir == IOPORT_DIR_INPUT) {
		pio->PIO_CFGR &= ~PIO_CFGR_DIR;
	}
}

/**
 * \brief Set direction for a single IOPORT pin.
 *
 * \param pin IOPORT pin to configure
 * \param dir Direction to set for the specified pin (\ref ioport_direction)
 */
static __always_inline void ioport_set_pin_dir(ioport_pin_t pin,
		enum ioport_direction dir)
{
    ioport_set_port_dir(ioport_pin_to_port_id(pin), ioport_pin_to_mask(pin), dir);
}

/**
 * \brief Set an IOPORT pin to a specified logical value.
 *
 * \param pin IOPORT pin to configure
 * \param level Logical value of the pin
 */
static __always_inline void ioport_set_pin_level(ioport_pin_t pin, bool level)
{
	PioGroup *pio = ioport_pin_to_base(pin);

	if (level) {
		pio->PIO_SODR = ioport_pin_to_mask(pin);
	} else {
		pio->PIO_CODR = ioport_pin_to_mask(pin);
	}
}

/**
 * \brief Set a group of IOPORT pins in a single port to a specified logical
 * value.
 *
 * \param port IOPORT port to write to
 * \param mask Pin mask of pins to modify
 * \param level Level of the pins to be modified
 */
static __always_inline void ioport_set_port_level(ioport_port_t port,
		ioport_port_mask_t mask, enum ioport_value level)
{
	PioGroup *pio = ioport_port_to_base(port);

	if (level) {
		pio->PIO_SODR = mask;
	} else {
		pio->PIO_CODR = mask;
	}
}

/**
 * \brief Get current value of an IOPORT pin, which has been configured as an
 * input.
 *
 * \param pin IOPORT pin to read
 * \return Current logical value of the specified pin
 */
static __always_inline bool ioport_get_pin_level(ioport_pin_t pin)
{
	PioGroup *port = ioport_pin_to_base(pin);

	return port->PIO_PDSR & ioport_pin_to_mask(pin);
}

/**
 * \brief Get current value of several IOPORT pins in a single port, which have
 * been configured as an inputs.
 *
 * \param port IOPORT port to read
 * \param mask Pin mask of pins to read
 * \return Logical levels of the specified pins from the read port, returned as
 * a mask.
 */
static __always_inline ioport_port_mask_t ioport_get_port_level(ioport_pin_t port,
		ioport_port_mask_t mask)
{
	PioGroup *pio = ioport_port_to_base(port);

	return pio->PIO_PDSR & mask;
}

/**
 * \brief Toggle the value of an IOPORT pin, which has previously configured as
 * an output.
 *
 * \param pin IOPORT pin to toggle
 */
static __always_inline void ioport_toggle_pin_level(ioport_pin_t pin)
{
	PioGroup *port = ioport_pin_to_base(pin);
	ioport_port_mask_t mask = ioport_pin_to_mask(pin);

	if (port->PIO_PDSR & mask) {
		port->PIO_CODR = mask;
	} else {
		port->PIO_SODR = mask;
	}
}

/**
 * \brief Toggle the values of several IOPORT pins located in a single port.
 *
 * \param port IOPORT port to modify
 * \param mask Pin mask of pins to toggle
 */
static __always_inline void ioport_toggle_port_level(ioport_port_t port,
		ioport_port_mask_t mask)
{
	PioGroup *pio = ioport_port_to_base(port);

	/* Select mask */
	pio->PIO_MSKR = mask;

	/* Toggle status */
	pio->PIO_ODSR ^= mask;
}

/**
 * \brief Set the pin sense mode of a multiple IOPORT pins on a single port.
 *
 * \param port IOPORT port to configure
 * \param mask Bitmask if pins whose edge sense is to be configured
 * \param pin_sense Edge to sense for the pins (\ref ioport_sense)
 */
static __always_inline void ioport_set_port_sense_mode(ioport_port_t port,
		ioport_port_mask_t mask,
		enum ioport_sense pin_sense)
{
	PioGroup *pio = ioport_port_to_base(port);
	uint32_t ul_cfgr = 0;

	/* Select mask */
	pio->PIO_MSKR = mask;

	/* Read configuration: If several bits are set in PIO_MSKRx, then
	 * the read configuration in PIO_CFGRx is the configuration of
	 * the I/O line with the lowest index.
	 */
	ul_cfgr = pio->PIO_CFGR;

	/* Clear Event Selection */
	ul_cfgr &= ~PIO_CFGR_EVTSEL_Msk;

	/* Set Event Selection */
	switch (pin_sense) {
	case IOPORT_SENSE_LEVEL_LOW:
		ul_cfgr |= PIO_CFGR_EVTSEL_LOW;
		break;

	case IOPORT_SENSE_LEVEL_HIGH:
		ul_cfgr |= PIO_CFGR_EVTSEL_HIGH;
		break;

	case IOPORT_SENSE_FALLING:
		ul_cfgr |= PIO_CFGR_EVTSEL_FALLING;
		break;

	case IOPORT_SENSE_RISING:
		ul_cfgr |= PIO_CFGR_EVTSEL_RISING;
		break;

	default:
		ul_cfgr |= PIO_CFGR_EVTSEL_BOTH;
		return;
	}

	/* Set configuration */
	pio->PIO_CFGR = ul_cfgr;
}

/**
 * \brief Set the pin sense mode of a single IOPORT pin.
 *
 * \param pin IOPORT pin to configure
 * \param pin_sense Edge to sense for the pin (\ref ioport_sense)
 */
static __always_inline void ioport_set_pin_sense_mode(ioport_pin_t pin,
		enum ioport_sense pin_sense)
{
	ioport_set_port_sense_mode(ioport_pin_to_port_id(pin), ioport_pin_to_mask(pin), 
	pin_sense);
}

#ifdef __cplusplus
}
#endif

#endif /* IOPORT_H */
