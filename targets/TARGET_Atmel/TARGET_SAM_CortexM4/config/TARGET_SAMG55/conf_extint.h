/**
 * \file
 *
 * \brief SAM G55 External Interrupt Driver Configuration Header
 *
 * Copyright (C) 2014-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
#ifndef CONF_EXTINT_H_INCLUDED
#define CONF_EXTINT_H_INCLUDED

#include <asf.h>

/**
 * Push button definitions for sleep mode and active mode
 * @{
 */
#define PIN_PUSHBUTTON_WAKEUP_PIO    PIOA
#define PIN_PUSHBUTTON_WAKEUP_MASK   PIO_PA2
#define PIN_PUSHBUTTON_WAKEUP_ID     ID_PIOA
#define PIN_PUSHBUTTON_WAKEUP_ATTR   PIO_DEFAULT
/** @} */

/** Wakeup pin for wait mode: SW0 */
#define WAKEUP_WAIT_INPUT_ID    (1u << 2)
/** Wakeup pin for backup mode: Touchscreen controller IRQ pin */
#define WAKEUP_BACKUP_INPUT_ID  (1u << 2)

void button_cb(void);
static inline void button_handler(uint32_t ul_id, uint32_t ul_mask);

/* Button Initialize */
static inline void button_init(void)
{
    /* Adjust PIO debounce filter parameters, using 10 Hz filter. */
    pio_set_debounce_filter(PIN_PUSHBUTTON_WAKEUP_PIO,
                            PIN_PUSHBUTTON_WAKEUP_MASK, 10);

    /* Initialize PIO interrupt handlers, see PIO definition in board.h. */
    pio_handler_set(PIN_PUSHBUTTON_WAKEUP_PIO, PIN_PUSHBUTTON_WAKEUP_ID,
                    PIN_PUSHBUTTON_WAKEUP_MASK, PIN_PUSHBUTTON_WAKEUP_ATTR,
                    button_handler);

    /* Enable PIO controller IRQs. */
    NVIC_EnableIRQ((IRQn_Type)PIN_PUSHBUTTON_WAKEUP_ID);

    /* Enable PIO line interrupts. */
    pio_enable_interrupt(PIN_PUSHBUTTON_WAKEUP_PIO,
                         PIN_PUSHBUTTON_WAKEUP_MASK);
}

/**
 * \brief Handler for button interrupt.
 *
 * \note This interrupt is for waking up from sleep mode or exiting from active
 * mode.
 */
static inline void button_handler(uint32_t ul_id, uint32_t ul_mask)
{
    if (PIN_PUSHBUTTON_WAKEUP_ID == ul_id &&
            PIN_PUSHBUTTON_WAKEUP_MASK == ul_mask) {
        button_cb();
    }
}

#endif
