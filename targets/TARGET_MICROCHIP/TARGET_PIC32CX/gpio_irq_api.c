/***************************************************************************//**
 * @file gpio_irq_api.h
 *******************************************************************************
 * @section License
 * <b>Copyright (c) 2021 Microchip Technology Inc. and its subsidiaries.</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "device.h"
#if DEVICE_INTERRUPTIN

#include "pic32cx.h"
#include "gpio_irq_api.h"
#include "mbed_assert.h"
#include "pinmap.h"

#include "ioport.h"
#include "pio.h"
#include "sleep_api.h"

/* Macro return index of the LSB flag which is set. */
#if ((__CORTEX_M == 3) || (__CORTEX_M == 4))
#define GPIOINT_MASK2IDX(mask) (__CLZ(__RBIT(mask)))
#elif __CORTEX_M == 0
#define GPIOINT_MASK2IDX(mask) (countTrailingZeros(mask))
__STATIC_INLINE uint32_t countTrailingZeros(uint32_t mask)
{
    uint32_t zeros;
    for(zeros=0; (zeros<32) && (0 == (mask&0x1)); zeros++, mask>>=1);
    return zeros;
}
#else
#error Unsupported architecture.
#endif

/**
 * Maximum number of interrupt sources that can be defined. This
 * constant can be increased, but the current value is the smallest possible one
 * that will be compatible with all existing projects.
 */
#define MAX_INTERRUPT_SOURCES       7

/**
 * Describes a PIO interrupt source, including the PIO instance triggering the
 * interrupt and the associated interrupt handler.
 */
struct s_interrupt_source {
	gpio_irq_t *obj;
	uint32_t id;
	uint32_t mask;
    gpio_irq_event event;

	/* Interrupt handler. */
	void (*handler)(const uint32_t, const gpio_irq_event uint32_t);
};

/* List of interrupt sources. */
static struct s_interrupt_source gs_interrupt_sources[MAX_INTERRUPT_SOURCES];

/* Number of currently defined interrupt sources. */
static uint32_t gs_ul_nb_sources = 0;

static struct s_interrupt_source *gpio_irq_get_source(gpio_irq_t *obj)
{
    uint8_t i;
    struct s_interrupt_source *pSource;
    
    // Check interrupt for this pin, if already defined, redefine it.
	for (i = 0; i <= gs_ul_nb_sources; i++) {
		pSource = &(gs_interrupt_sources[i]);
		if (pSource->obj == obj) {
			return pSource;
		}
	}
    return NULL;
}

static void gpio_irq_handler_process(PioGroup *p_pio_group, uint32_t ul_id)
{
	uint32_t status;
	uint32_t i;

	/* Read PIO controller status */
	status = pio_get_interrupt_status(p_pio_group);
	status &= pio_get_interrupt_mask(p_pio_group);

	/* Check pending events */
	if (status != 0) {
		/* Find triggering source */
		i = 0;
		while (status != 0) {
			/* Source is configured on the same controller */
			if (gs_interrupt_sources[i].id == ul_id) {
				/* Source has PIOs whose statuses have changed */
				if ((status & gs_interrupt_sources[i].mask) != 0) {
					gs_interrupt_sources[i].handler(gs_interrupt_sources[i].id,
							gs_interrupt_sources[i].event);
					status &= ~(gs_interrupt_sources[i].mask);
				}
			}

			i++;
			if (i >= MAX_INTERRUPT_SOURCES) {
				break;
			}
		}
	}
}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    PioGroup *pio;
    ioport_port_t port;
    ioport_port_mask_t mask;
    uint32_t pioId;
	struct s_interrupt_source *pSource;
    uint8_t i;
    
    // Init pin
    obj->pin = pin;
    obj->risingEdge = 0;
    obj->fallingEdge = 0;
    
    // Get PIO definitions
    port = ioport_pin_to_port_id(pin);
    pio = ioport_port_to_base(port);
    mask = ioport_pin_to_mask(pin);
    pioId = ioport_pin_to_port_id(pin);

    // Check MAX interrupt sources
	if (gs_ul_nb_sources >= MAX_INTERRUPT_SOURCES) {
		return 1;
	}

	// Check interrupt for this pin, if already defined, redefine it.
	for (i = 0; i <= gs_ul_nb_sources; i++) {
		pSource = &(gs_interrupt_sources[i]);
		if (pSource->id == id && pSource->mask == mask) {
			break;
		}
	}

	/* Define new source */
    pSource->obj = obj;
	pSource->id = id;
	pSource->mask = mask;
	pSource->handler = handler;
	if (i == gs_ul_nb_sources + 1) {
		gs_ul_nb_sources++;
	}
    
    // Adjust pio debounce filter parameters, uses 10 Hz filter.
	pio_set_debounce_filter(pio, mask, 10);
    
     // Disable PIO line interrupts.
	pio_disable_interrupt(pio, mask);
    
    // Disable NVIC
    NVIC_DisableIRQ((IRQn_Type)pioId);

    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    // Destructor
    gpio_irq_disable(obj); // Disable interrupt channel
//    pin_mode(obj->pin, Disabled); // Disable input pin
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    PioGroup *pio;
    ioport_port_t port;
    ioport_port_mask_t mask;
    uint32_t pioId;
	struct s_interrupt_source *pSource;
    
    // Check previous init of interrupt source
    pSource = gpio_irq_get_source(obj);
    if (pSource) {
        pSource->event = event;
        
        port = ioport_pin_to_port_id(obj->pin);
        pio = ioport_port_to_base(port);
        mask = ioport_pin_to_mask(obj->pin);
        pioId = ioport_pin_to_port_id(obj->pin);
        
        switch (event) {
            case (IRQ_RISE):
                obj->risingEdge = enable;
                pio_configure_interrupt(pio, mask, PIO_IT_RISING);
                break;
            case (IRQ_FALL):
                obj->fallingEdge = enable;
                pio_configure_interrupt(pio, mask, PIO_IT_FALLING);
                break;
            case (IRQ_NONE):
                break;
        }
        
        // Configure NVIC
        NVIC_ClearPendingIRQ((IRQn_Type)pioId);
        if (enable) {
                // Enable NVIC
                NVIC_EnableIRQ((IRQn_Type)pioId);
        } else {
                // Disable NVIC
                NVIC_DisableIRQ((IRQn_Type)pioId);
        }

        /* Disable PIO line interrupts. */
        pio_disable_interrupt(pio, mask);
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    PioGroup *pio;
    ioport_port_mask_t mask;
	struct s_interrupt_source *pSource;
    
    // Check previous init of interrupt source
    pSource = gpio_irq_get_source(obj);
    if (pSource) {
        pio = ioport_pin_to_base(obj->pin);
        mask = ioport_pin_to_mask(obj->pin);
        
        /* Enable PIO line interrupts. */
        pio_enable_interrupt(pio, mask);
    }
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    PioGroup *pio;
    ioport_port_mask_t mask;
	struct s_interrupt_source *pSource;
    
    // Check previous init of interrupt source
    pSource = gpio_irq_get_source(obj);
    if (pSource) {
        pio = ioport_pin_to_base(obj->pin);
        mask = ioport_pin_to_mask(obj->pin);

        /* Disable PIO line interrupts. */
        pio_disable_interrupt(pio, mask);
    }
}

/**
 * \brief Parallel IO Controller A interrupt handler.
 * Redefined PIOA interrupt handler for NVIC interrupt table.
 */
void PIOA_Handler(void)
{
	gpio_irq_handler_process(PIOA, ID_PIOA);
}

/**
 * \brief Parallel IO Controller B interrupt handler
 * Redefined PIOB interrupt handler for NVIC interrupt table.
 */
void PIOB_Handler(void)
{
	gpio_irq_handler_process(PIOB, ID_PIOB);
}

/**
 * \brief Parallel IO Controller C interrupt handler.
 * Redefined PIOC interrupt handler for NVIC interrupt table.
 */
void PIOC_Handler(void)
{
	gpio_irq_handler_process(PIOC, ID_PIOC);
}

#ifdef ID_PIOD
/**
 * \brief Parallel IO Controller D interrupt handler.
 * Redefined PIOD interrupt handler for NVIC interrupt table.
 */
void PIOD_Handler(void)
{
	gpio_irq_handler_process(PIOD, ID_PIOD);
}
#endif

#endif
