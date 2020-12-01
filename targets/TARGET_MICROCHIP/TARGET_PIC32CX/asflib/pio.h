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

#ifndef PIO_H_INCLUDED
#define PIO_H_INCLUDED

#include "compiler.h"
#include "pic32cx.h"

#ifdef __cplusplus
extern "C" {
#endif

/* GPIO Support */
#define PIO_TYPE_Pos                  0
/* PIO Type Mask */
#define PIO_TYPE_Msk                  (0x7u << PIO_TYPE_Pos)
/* The pin is not a function pin. */
#define PIO_TYPE_NOT_A_PIN            (0x0u << PIO_TYPE_Pos)
/* The pin is controlled by the peripheral A. */
#define PIO_TYPE_PIO_PERIPH_A         (0x1u << PIO_TYPE_Pos)
/* The pin is controlled by the peripheral B. */
#define PIO_TYPE_PIO_PERIPH_B         (0x2u << PIO_TYPE_Pos)
/* The pin is controlled by the peripheral C. */
#define PIO_TYPE_PIO_PERIPH_C         (0x3u << PIO_TYPE_Pos)
/* The pin is controlled by the peripheral D. */
#define PIO_TYPE_PIO_PERIPH_D         (0x4u << PIO_TYPE_Pos)
/* The pin is an input. */
#define PIO_TYPE_PIO_INPUT            (0x5u << PIO_TYPE_Pos)
/* The pin is an output and has a default level of 0. */
#define PIO_TYPE_PIO_OUTPUT_0         (0x6u << PIO_TYPE_Pos)
/* The pin is an output and has a default level of 1. */
#define PIO_TYPE_PIO_OUTPUT_1         (0x7u << PIO_TYPE_Pos)

typedef enum _pio_type {
	PIO_NOT_A_PIN   = PIO_TYPE_NOT_A_PIN,
	PIO_PERIPH_A    = PIO_TYPE_PIO_PERIPH_A,
	PIO_PERIPH_B    = PIO_TYPE_PIO_PERIPH_B,
	PIO_PERIPH_C    = PIO_TYPE_PIO_PERIPH_C,
	PIO_PERIPH_D    = PIO_TYPE_PIO_PERIPH_D,
	PIO_INPUT       = PIO_TYPE_PIO_INPUT,
	PIO_OUTPUT_0    = PIO_TYPE_PIO_OUTPUT_0,
	PIO_OUTPUT_1    = PIO_TYPE_PIO_OUTPUT_1
} pio_type_t;

/*  Default pin configuration (no attribute). */
#define PIO_DEFAULT             (0u << 0)
/*  The internal pin pull-up is active. */
#define PIO_PULLUP              (1u << 0)
/*  The internal glitch filter is active. */
#define PIO_DEGLITCH            (1u << 1)
/*  The pin is open-drain. */
#define PIO_OPENDRAIN           (1u << 2)
/*  The internal debouncing filter is active. */
#define PIO_DEBOUNCE            (1u << 3)

/*  Event detection on input falling edge */
#define PIO_IT_FALLING          0
/*  Event detection on input rising edge */
#define PIO_IT_RISING           1
/*  Event detection on input both edge */
#define PIO_IT_BOTH             2
/*  Event detection on low level input */
#define PIO_IT_LOW              3
/*  Event detection on high level input */
#define PIO_IT_HIGH             4

/*
 *  The #attribute# field is a bitmask that can either be set to PIO_DEFAULT,
 *  or combine (using bitwise OR '|') any number of the following constants:
 *     - PIO_PULLUP
 *     - PIO_DEGLITCH
 *     - PIO_DEBOUNCE
 *     - PIO_OPENDRAIN
 */
void pio_pull_up(PioGroup *p_pio_group, const uint32_t ul_mask, const uint32_t ul_pull_up_enable);
void pio_set_debounce_filter(PioGroup *p_pio_group, const uint32_t ul_mask, const uint32_t ul_cut_off);
void pio_set(PioGroup *p_pio_group, const uint32_t ul_mask);
void pio_clear(PioGroup *p_pio_group, const uint32_t ul_mask);
uint32_t pio_get(PioGroup *p_pio_group, const pio_type_t ul_type, const uint32_t ul_mask);
void pio_set_peripheral(PioGroup *p_pio_group, const pio_type_t ul_type, const uint32_t ul_mask);
void pio_set_input(PioGroup *p_pio_group, const uint32_t ul_mask, const uint32_t ul_attribute);
void pio_set_output(PioGroup *p_pio_group, const uint32_t ul_mask, const uint32_t ul_default_level,
		const uint32_t ul_multidrive_enable, const uint32_t ul_pull_up_enable);
uint32_t pio_configure(PioGroup *p_pio_group, const pio_type_t ul_type, const uint32_t ul_mask, const uint32_t ul_attribute);
uint32_t pio_get_lock_status(const PioGroup *p_pio_group);
uint32_t pio_get_output_data_status(const PioGroup *p_pio_group, const uint32_t ul_mask);
void pio_set_multi_driver(PioGroup *p_pio_group, const uint32_t ul_mask, const uint32_t ul_multi_driver_enable);
void pio_pull_down(PioGroup *p_pio_group, const uint32_t ul_mask, const uint32_t ul_pull_down_enable);
void pio_sync_output_write(PioGroup *p_pio_group, const uint32_t ul_mask);
void pio_set_schmitt_trigger(PioGroup *p_pio_group, const uint32_t ul_mask);

void pio_configure_interrupt(PioGroup *p_pio_group, const uint32_t ul_mask, const uint32_t ul_attr);
void pio_enable_interrupt(PioGroup *p_pio_group, const uint32_t ul_mask);
void pio_disable_interrupt(PioGroup *p_pio_group, const uint32_t ul_mask);
uint32_t pio_get_interrupt_status(const PioGroup *p_pio_group);
uint32_t pio_get_interrupt_mask(const PioGroup *p_pio_group);

void pio_set_physical_freeze(PioGroup *p_pio_group, const uint32_t ul_mask);
void pio_set_interrupt_freeze(PioGroup *p_pio_group, const uint32_t ul_mask);

void pio_set_writeprotect(PioGroup *p_pio_group, const uint32_t ul_enable);
uint32_t pio_get_writeprotect_status(const PioGroup *p_pio_group);
void pio_set_interrupt_writeprotect(PioGroup *p_pio_group, const uint32_t ul_enable);

/* GPIO Support */
uint32_t pio_get_pin_value(uint32_t pin);
void pio_set_pin_high(uint32_t pin);
void pio_set_pin_low(uint32_t pin);
void pio_toggle_pin(uint32_t pin);
uint32_t pio_configure_pin(uint32_t ul_pin, const uint32_t ul_flags);

void pio_set_pin_group_high(PioGroup *p_pio_group, uint32_t ul_mask);
void pio_set_pin_group_low(PioGroup *p_pio_group, uint32_t ul_mask);
void pio_toggle_pin_group(PioGroup *p_pio_group, uint32_t ul_mask);
uint32_t pio_configure_pin_group(PioGroup *p_pio_group, uint32_t ul_mask, const uint32_t ul_flags);

void pio_enable_pin_interrupt(uint32_t pin);
void pio_disable_pin_interrupt(uint32_t pin);

PioGroup *pio_get_pin_group(uint32_t pin);
uint32_t pio_get_pin_group_id(uint32_t pin);
uint32_t pio_get_pin_group_mask(uint32_t pin);


/**
 * \page sam_pio_quickstart Quick Start Guide for the SAM PIO driver
 *
 * This is the quick start guide for the \ref sam_drivers_pio_group "PIO Driver",
 * with step-by-step instructions on how to configure and use the driver for
 * specific use cases.
 *
 * The section described below can be compiled into e.g. the main application
 * loop or any other function that will need to interface with the IO port.
 *
 * \section sam_pio_usecases PIO use cases
 * - \ref sam_pio_quickstart_basic
 * - \ref sam_pio_quickstart_use_case_2
 *
 * \section sam_pio_quickstart_basic Basic usage of the PIO driver
 * This section will present a basic use case for the PIO driver. This use case
 * will configure pin 23 on port A as output and pin 16 as an input with pullup,
 * and then toggle the output pin's value to match that of the input pin.
 *
 * \subsection sam_pio_quickstart_use_case_1_prereq Prerequisites
 *  - \ref group_pmc "Power Management Controller driver"
 *
 * \subsection sam_pio_quickstart_use_case_1_setup_steps Initialization code
 * Add to the application initialization code:
 * \code
 *          pmc_enable_periph_clk(ID_PIOA);
 *
 *          pio_set_output(PIOA, PIO_PA23, LOW, DISABLE, ENABLE);
 *          pio_set_input(PIOA, PIO_PA16, PIO_PULLUP);
 * \endcode
 *
 * \subsection sam_pio_quickstart_use_case_1_setup_steps_workflow Workflow
 * -# Enable the module clock to the PIOA peripheral:
 *    \code pmc_enable_periph_clk(ID_PIOA); \endcode
 * -# Set pin 23 direction on PIOA as output, default low level:
 *     \code pio_set_output(PIOA, PIO_PA23, LOW, DISABLE, ENABLE); \endcode
 * -# Set pin 16 direction on PIOA as input, with pullup:
 *     \code pio_set_input(PIOA, PIO_PA16, PIO_PULLUP); \endcode
 *
 * \subsection sam_pio_quickstart_use_case_1_example_code Example code
 *   Set the state of output pin 23 to match input pin 16:
 *   \code
 *      if (pio_get(PIOA, PIO_TYPE_PIO_INPUT, PIO_PA16))
 *          pio_clear(PIOA, PIO_PA23);
 *      else
 *          pio_set(PIOA, PIO_PA23);
 * \endcode
 *
 * \subsection sam_pio_quickstart_use_case_1_example_workflow Workflow
 * -# We check the value of the pin:
 *     \code
 *      if (pio_get(PIOA, PIO_TYPE_PIO_INPUT, PIO_PA16))
 * \endcode
 * -# Then we set the new output value based on the read pin value:
 *     \code
 *          pio_clear(PIOA, PIO_PA23);
 *      else
 *          pio_set(PIOA, PIO_PA23);
 * \endcode
 */

/**
 * \page sam_pio_quickstart_use_case_2 Advanced use case - Interrupt driven edge detection
 *
 * \section sam_pio_quickstart_use_case_2 Advanced Use Case 1
 * This section will present a more advanced use case for the PIO driver. This use case
 * will configure pin 23 on port A as output and pin 16 as an input with pullup,
 * and then toggle the output pin's value to match that of the input pin using the interrupt
 * controller within the device.
 *
 * \subsection sam_pio_quickstart_use_case_2_prereq Prerequisites
 *  - \ref group_pmc "Power Management Controller driver"
 *
 * \subsection sam_pio_quickstart_use_case_2_setup_steps Initialization code
 * Add to the application initialization code:
 * \code
 *       pmc_enable_periph_clk(ID_PIOA);
 *
 *       pio_set_output(PIOA, PIO_PA23, LOW, DISABLE, ENABLE);
 *       pio_set_input(PIOA, PIO_PA16, PIO_PULLUP);
 *
 *       pio_handler_set(PIOA, ID_PIOA, PIO_PA16, PIO_IT_EDGE, pin_edge_handler);
 *       pio_enable_interrupt(PIOA, PIO_PA16);
 *
 *       NVIC_EnableIRQ(PIOA_IRQn);
 * \endcode
 *
 * \subsection sam_pio_quickstart_use_case_2_setup_steps_workflow Workflow
 * -# Enable the module clock to the PIOA peripheral:
 *    \code pmc_enable_periph_clk(ID_PIOA); \endcode
 * -# Set pin 23 direction on PIOA as output, default low level:
 *     \code pio_set_output(PIOA, PIO_PA23, LOW, DISABLE, ENABLE); \endcode
 * -# Set pin 16 direction on PIOA as input, with pullup:
 *     \code pio_set_input(PIOA, PIO_PA16, PIO_PULLUP); \endcode
 * -# Configure the input pin 16 interrupt mode and handler:
 *     \code pio_handler_set(PIOA, ID_PIOA, PIO_PA16, PIO_IT_EDGE, pin_edge_handler); \endcode
 * -# Enable the interrupt for the configured input pin:
 *     \code pio_enable_interrupt(PIOA, PIO_PA16); \endcode
 * -# Enable interrupt handling from the PIOA module:
 *     \code NVIC_EnableIRQ(PIOA_IRQn); \endcode
 *
 * \subsection sam_pio_quickstart_use_case_2_example_code Example code
 * Add the following function to your application:
 * \code
 *      void pin_edge_handler(const uint32_t id, const uint32_t index)
 *      {
 *              if ((id == ID_PIOA) && (index == PIO_PA16)){
 *                      if (pio_get(PIOA, PIO_TYPE_PIO_INPUT, PIO_PA16))
 *                              pio_clear(PIOA, PIO_PA23);
 *                      else
 *                              pio_set(PIOA, PIO_PA23);
 *              }
 *      }
 * \endcode
 *
 * \subsection sam_pio_quickstart_use_case_2_example_workflow Workflow
 * -# We check the value of the pin:
 *     \code
 *      if (pio_get(PIOA, PIO_TYPE_PIO_INPUT, PIO_PA16))
 * \endcode
 * -# Then we set the new output value based on the read pin value:
 *     \code
 *          pio_clear(PIOA, PIO_PA23);
 *      else
 *          pio_set(PIOA, PIO_PA23);
 * \endcode
 */

#ifdef __cplusplus
}
#endif

#endif /* PIO_H_INCLUDED */
