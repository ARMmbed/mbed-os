/* mbed Microcontroller Library
 * Copyright (c) 2016 u-blox
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <stdint.h>
#include <stdbool.h>
#include "mbed_assert.h"
#include "cmsis.h"
#include "hi2110_init.h"

/* ----------------------------------------------------------------
 * MACROS
 * ----------------------------------------------------------------*/

/* ----------------------------------------------------------------
 * FUNCTION PROTOTYPES
 * ----------------------------------------------------------------*/

static uint8_t get_owner(uint8_t pin);

/* ----------------------------------------------------------------
 * NON-API FUNCTIONS
 * ----------------------------------------------------------------*/

/* Determine which core owns a given pin
 * 0: None
 * 1: security core
 * 2: protocol core
 * 3: apps core */
static uint8_t get_owner(uint8_t pin)
{
    uint8_t value;
    uint8_t pio_owner_shift = (pin & 0x0F) << 1;
    volatile uint32_t * pio_owner_reg = (&PIO_OWNER0 + (pin >> 4));

    value = 0x03 & (*pio_owner_reg >> pio_owner_shift);

    return value;
}

/* ----------------------------------------------------------------
 * MBED API FUNCTIONS
 * ----------------------------------------------------------------*/

void HI2110_init(void)
{
    __attribute__ ((unused)) uint8_t owner[20];
    
    /* This purely for diagnostics to see who owns which PIO pin.
     * Put a break-point at the end of this function and take a look
     * at the array.
     * Any items marked as 1 or 2 belong to the security or protocol
     * cores.  Otherwise they are up for grabs. */
    for (uint8_t x = 0; x < 20; x++) {
        owner[x] = get_owner(x);
    }
}
