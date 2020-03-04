/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this list
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA
 *      integrated circuit in a product or a software update for such product, must reproduce
 *      the above copyright notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be
 *      used to endorse or promote products derived from this software without specific prior
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse
 *      engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "PeripheralPins.h"
#include "PeripheralPinMaps.h"

// Pinmap used for testing only
const PinMap PinMap_I2C_testing[] = {
    {P0_0,  0, 0},
    {P0_1,  0, 0},
    {P0_2,  0, 0},
    {P0_3,  0, 0},
    {P0_4,  0, 0},
    {P0_5,  0, 0},
    {P0_6,  0, 0},
    {P0_7,  0, 0},
    {P0_8,  0, 0},
    {P0_9,  0, 0},
    {P0_10,  0, 0},
    {P0_11,  0, 0},
    {P0_12,  0, 0},
    {P0_13,  0, 0},
    {P0_14,  0, 0},
    {P0_15,  0, 0},
    {P0_16,  0, 0},
    {P0_17,  0, 0},
    {P0_18,  0, 0},
    {P0_19,  0, 0},
    {P0_20,  0, 0},
    {P0_21,  0, 0},
    {P0_22,  0, 0},
    {P0_23,  0, 0},
    {P0_24,  0, 0},
    {P0_25,  0, 0},
    {P0_28,  0, 0},
    {P0_29,  0, 0},
    {P0_30,  0, 0},
    {P0_31,  0, 0},

    {P1_0,  0, 0},
    {P1_1,  0, 0},
    {P1_2,  0, 0},
    {P1_3,  0, 0},
    {P1_4,  0, 0},
    {P1_5,  0, 0},
    {P1_6,  0, 0},
    {P1_7,  0, 0},
    {P1_8,  0, 0},
    {P1_9,  0, 0},
    {P1_10,  0, 0},
    {P1_11,  0, 0},
    {P1_12,  0, 0},
    {P1_13,  0, 0},
    {P1_14,  0, 0},
    {P1_15,  0, 0},

    {NC, NC, 0}
};

// Pinmap used for testing only
const PinMap PinMap_SPI_testing[] = {
    {P0_0,  0, 0},
    {P0_1,  0, 0},
    {P0_2,  0, 0},
    {P0_3,  0, 0},
    {P0_4,  0, 0},
    {P0_5,  0, 0},
    {P0_6,  0, 0},
    {P0_7,  0, 0},
    {P0_8,  0, 0},
    {P0_9,  0, 0},
    {P0_10,  0, 0},
    {P0_11,  0, 0},
    {P0_12,  0, 0},
    {P0_13,  0, 0},
    {P0_14,  0, 0},
    {P0_15,  0, 0},
    {P0_16,  0, 0},
    {P0_17,  0, 0},
    {P0_18,  0, 0},
    {P0_19,  0, 0},
    {P0_20,  0, 0},
    {P0_21,  0, 0},
    {P0_22,  0, 0},
    {P0_23,  0, 0},
    {P0_24,  0, 0},
    {P0_25,  0, 0},
    {P0_28,  0, 0},
    {P0_29,  0, 0},
    {P0_30,  0, 0},
    {P0_31,  0, 0},

    {P1_0,  0, 0},
    {P1_1,  0, 0},
    {P1_2,  0, 0},
    {P1_3,  0, 0},
    {P1_4,  0, 0},
    {P1_5,  0, 0},
    {P1_6,  0, 0},
    {P1_7,  0, 0},
    {P1_8,  0, 0},
    {P1_9,  0, 0},
    {P1_10,  0, 0},
    {P1_11,  0, 0},
    {P1_12,  0, 0},
    {P1_13,  0, 0},
    {P1_14,  0, 0},
    {P1_15,  0, 0},

    {NC, NC, 0}
};

// Pinmap used for testing only
const PinMap PinMap_UART_testing[] = {
    {P0_0,  0, 0},
    {P0_1,  0, 0},
    {P0_2,  0, 0},
    {P0_3,  0, 0},
    {P0_4,  0, 0},
    {P0_5,  0, 0},
    {P0_6,  0, 0},
    {P0_7,  0, 0},
    {P0_8,  0, 0},
    {P0_9,  0, 0},
    {P0_10,  0, 0},
    {P0_11,  0, 0},
    {P0_12,  0, 0},
    {P0_13,  0, 0},
    {P0_14,  0, 0},
    {P0_15,  0, 0},
    {P0_16,  0, 0},
    {P0_17,  0, 0},
    {P0_18,  0, 0},
    {P0_19,  0, 0},
    {P0_20,  0, 0},
    {P0_21,  0, 0},
    {P0_22,  0, 0},
    {P0_23,  0, 0},
    {P0_24,  0, 0},
    {P0_25,  0, 0},
    {P0_28,  0, 0},
    {P0_29,  0, 0},
    {P0_30,  0, 0},
    {P0_31,  0, 0},

    {P1_0,  0, 0},
    {P1_1,  0, 0},
    {P1_2,  0, 0},
    {P1_3,  0, 0},
    {P1_4,  0, 0},
    {P1_5,  0, 0},
    {P1_6,  0, 0},
    {P1_7,  0, 0},
    {P1_8,  0, 0},
    {P1_9,  0, 0},
    {P1_10,  0, 0},
    {P1_11,  0, 0},
    {P1_12,  0, 0},
    {P1_13,  0, 0},
    {P1_14,  0, 0},
    {P1_15,  0, 0},

    {NC, NC, 0}
};

// Pinmap used for testing only
const PinMap PinMap_PWM_testing[] = {
    {P0_0,  0, 0},
    {P0_1,  0, 0},
    {P0_2,  0, 0},
    {P0_3,  0, 0},
    {P0_4,  0, 0},
    {P0_5,  0, 0},
    {P0_6,  0, 0},
    {P0_7,  0, 0},
    {P0_8,  0, 0},
    {P0_9,  0, 0},
    {P0_10,  0, 0},
    {P0_11,  0, 0},
    {P0_12,  0, 0},
    {P0_13,  0, 0},
    {P0_14,  0, 0},
    {P0_15,  0, 0},
    {P0_16,  0, 0},
    {P0_17,  0, 0},
    {P0_18,  0, 0},
    {P0_19,  0, 0},
    {P0_20,  0, 0},
    {P0_21,  0, 0},
    {P0_22,  0, 0},
    {P0_23,  0, 0},
    {P0_24,  0, 0},
    {P0_25,  0, 0},
    {P0_28,  0, 0},
    {P0_29,  0, 0},
    {P0_30,  0, 0},
    {P0_31,  0, 0},

    {P1_0,  0, 0},
    {P1_1,  0, 0},
    {P1_2,  0, 0},
    {P1_3,  0, 0},
    {P1_4,  0, 0},
    {P1_5,  0, 0},
    {P1_6,  0, 0},
    {P1_7,  0, 0},
    {P1_8,  0, 0},
    {P1_9,  0, 0},
    {P1_10,  0, 0},
    {P1_11,  0, 0},
    {P1_12,  0, 0},
    {P1_13,  0, 0},
    {P1_14,  0, 0},
    {P1_15,  0, 0},

    {NC, NC, 0}
};

// Pinmap used for testing only
const PinMap PinMap_QSPI_testing[] = {
    {P0_0,  0, 0},
    {P0_1,  0, 0},
    {P0_2,  0, 0},
    {P0_3,  0, 0},
    {P0_4,  0, 0},
    {P0_5,  0, 0},
    {P0_6,  0, 0},
    {P0_7,  0, 0},
    {P0_8,  0, 0},
    {P0_9,  0, 0},
    {P0_10,  0, 0},
    {P0_11,  0, 0},
    {P0_12,  0, 0},
    {P0_13,  0, 0},
    {P0_14,  0, 0},
    {P0_15,  0, 0},
    {P0_16,  0, 0},
    {P0_17,  0, 0},
    {P0_18,  0, 0},
    {P0_19,  0, 0},
    {P0_20,  0, 0},
    {P0_21,  0, 0},
    {P0_22,  0, 0},
    {P0_23,  0, 0},
    {P0_24,  0, 0},
    {P0_25,  0, 0},
    {P0_28,  0, 0},
    {P0_29,  0, 0},
    {P0_30,  0, 0},
    {P0_31,  0, 0},

    {P1_0,  0, 0},
    {P1_1,  0, 0},
    {P1_2,  0, 0},
    {P1_3,  0, 0},
    {P1_4,  0, 0},
    {P1_5,  0, 0},
    {P1_6,  0, 0},
    {P1_7,  0, 0},
    {P1_8,  0, 0},
    {P1_9,  0, 0},
    {P1_10,  0, 0},
    {P1_11,  0, 0},
    {P1_12,  0, 0},
    {P1_13,  0, 0},
    {P1_14,  0, 0},
    {P1_15,  0, 0},

    {NC, NC, 0}
};

/* On NRF we have just dummy pin-maps for testing (no peripherals are specified).
   Disable restriction for STDIO uart peripheral. */
const PeripheralList *pinmap_uart_restricted_peripherals()
{
    static const PeripheralList peripheral_list = {
        0,
        0
    };
    return &peripheral_list;
}
