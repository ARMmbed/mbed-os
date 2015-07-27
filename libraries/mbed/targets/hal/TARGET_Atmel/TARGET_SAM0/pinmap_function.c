/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include <stddef.h>
#include "us_ticker_api.h"
#include "cmsis.h"
#include "mbed_assert.h"
#include "ins_gclk.h"
#include "compiler.h"
#include "system.h"

#include "pinmap_function.h"

static uint32_t pinmap_merge_pins(uint32_t a, uint32_t b)
{
    // both are the same (inc both NC)
    if (a == b)
        return a;

    // one (or both) is not connected
    if (a == (uint32_t)NC)
        return b;
    if (b == (uint32_t)NC)
        return a;

    return (uint32_t)NC;
}

/** Find the common SERCOM shared by two pins
 *
 * Finds the common SERCOM index of two input pins.
 * If swapping the input argument gives different result, it means, two SERCOMs share both pins
 * @param[in] pin1  First pin
 * @param[in] pin2  Second pin
 * @return          SERCOM index if found, else, NC
 */
uint32_t pinmap_merge_sercom(PinName pin1, PinName pin2)
{
    int i, j;
    uint32_t pin1_sercom[2];
    uint32_t pin2_sercom[2];
    uint32_t sercom_index[4];

    uint32_t pin_com = NC;
    uint32_t pin_alt = NC;
    uint32_t count_com = 0;
    uint32_t count_alt = 0;

    /* Adding a condition check just in case we need a different result when swapping arguments */
    if (pin1 >= pin2) {
        pin1_sercom[0] = pinmap_find_peripheral(pin1, PinMap_SERCOM_PAD);
        pin1_sercom[1] = pinmap_find_peripheral(pin1, PinMap_SERCOM_PADEx);
    } else {
        pin1_sercom[0] = pinmap_find_peripheral(pin1, PinMap_SERCOM_PADEx);
        pin1_sercom[1] = pinmap_find_peripheral(pin1, PinMap_SERCOM_PAD);
    }

    pin2_sercom[0] = pinmap_find_peripheral(pin2, PinMap_SERCOM_PAD);
    pin2_sercom[1] = pinmap_find_peripheral(pin2, PinMap_SERCOM_PADEx);

    for  (i=0; i<2; i++) {
        if (pin1_sercom[i] != NC) {
            pin1_sercom[i] &= 0x0F;
        }
        for  (j=0; j<2; j++) {
            if (pin2_sercom[i] != NC) {
                pin2_sercom[i] &= 0x0F;
            }
            sercom_index[(i*2) + j] = pinmap_merge_pins(pin1_sercom[i], pin2_sercom[j]);
        }
    }

    for (i=0; i<4; i++) {
        if (sercom_index[i] != NC) {
            if (pin_com == NC) {
                pin_com = sercom_index[i];
                count_com++;
            } else if (pin_com == sercom_index[i]) {
                count_com++;
            } else if (pin_alt == NC) {
                pin_alt = sercom_index[i];
                count_alt++;
            } else if (pin_alt == sercom_index[i]) {
                count_alt++;
            } else {}
        }
    }
    return ((count_com >= count_alt) ? pin_com : pin_alt);
}

/** Find the common SERCOM shared by four pins
 *
 * Finds the common SERCOM index shared by four input pins.
 * If reversing the input argument order gives different result, it means, two SERCOMs share the pins
 * @param[in] pin1  First pin
 * @param[in] pin2  Second pin
 * @param[in] pin3  Third pin
 * @param[in] pin4  Fourth pin
 * @return          SERCOM index if found, else, NC
 */
uint32_t pinmap_find_sercom(PinName pin1, PinName pin2, PinName pin3, PinName pin4)
{
    int i;
    uint32_t sercom_index[4];
    uint32_t pin_com = NC;
    uint32_t pin_alt = NC;
    uint32_t count_com = 0;
    uint32_t count_alt = 0;

    sercom_index[0] = pinmap_merge_sercom(pin1, pin2);
    sercom_index[1] = pinmap_merge_sercom(pin3, pin3);
    sercom_index[2] = pinmap_merge_sercom(pin1, pin3);
    sercom_index[3] = pinmap_merge_sercom(pin2, pin4);


    for (i=0; i<4; i++) {
        if (sercom_index[i] != NC) {
            if (pin_com == NC) {
                pin_com = sercom_index[i];
                count_com++;
            } else if (pin_com == sercom_index[i]) {
                count_com++;
            } else if (pin_alt == NC) {
                pin_alt = sercom_index[i];
                count_alt++;
            } else if (pin_alt == sercom_index[i]) {
                count_alt++;
            } else {}
        }
    }
    return ((count_com >= count_alt) ? pin_com : pin_alt);
}

/** Find the MUX function of input pin specific to given SERCOM index
 *
 * @param[in] pin           Pin whose function is to be found out
 * @param[in] sercom_index  SERCOM index
 * @return                  MUX function if found, else, NC
 */
uint32_t pinmap_function_sercom(PinName pin, uint32_t sercom_index)
{
    uint32_t func = NC;
    uint32_t index;
    sercom_index &= 0x0F;

    if ((pin == NC) || (sercom_index >= SERCOM_INST_NUM)) {
        return NC;
    }
    index = pinmap_peripheral(pin, PinMap_SERCOM_PAD);
    if ((index & 0x0F) == sercom_index) {
        func = pinmap_function(pin, PinMap_SERCOM_PAD);
        return func;
    }
    index = pinmap_peripheral(pin, PinMap_SERCOM_PADEx);
    if ((index & 0x0F) == sercom_index) {
        func = pinmap_function(pin, PinMap_SERCOM_PADEx);
        return func;
    }
    return NC;
}

/** Find the MUX pad of input pin specific to given SERCOM index
 *
 * @param[in] pin           Pin whose function is to be found out
 * @param[in] sercom_index  SERCOM index
 * @return                  MUX pad if found, else, NC
 */
uint32_t pinmap_pad_sercom(PinName pin, uint32_t sercom_index)
{
    uint32_t index;
    sercom_index &= 0x0F;

    if ((pin == NC) || (sercom_index >= SERCOM_INST_NUM)) {
        return NC;
    }
    index = pinmap_peripheral(pin, PinMap_SERCOM_PAD);
    if ((index & 0x0F) == sercom_index) {
        return ((index >> 4) & 0x0F);
    }
    index = pinmap_peripheral(pin, PinMap_SERCOM_PADEx);
    if ((index & 0x0F) == sercom_index) {
        return ((index >> 4) & 0x0F);
    }
    return NC;
}

/** Find the MUX function of input pin specific to given SERCOM index
 *
 * @param[in] pin           unused
 * @param[in] sercom_index  SERCOM index
 * @return                  base address to SERCOM if found, else NC
 */
uint32_t pinmap_peripheral_sercom(PinName pin, uint32_t sercom_index)
{
    uint32_t sercom_address[6] = {
        0x42000800UL,  // Base address of SERCOM0
        0x42000C00UL,  // Base address of SERCOM1
        0x42001000UL,  // Base address of SERCOM2
        0x42001400UL,  // Base address of SERCOM3
        0x42001800UL,  // Base address of SERCOM4
        0x42001C00UL   // Base address of SERCOM5
    };
    uint32_t index = sercom_index & 0x0F;

    if (index >= SERCOM_INST_NUM) {
        return NC;
    }
    return sercom_address[(sercom_index&0x0F)];
}