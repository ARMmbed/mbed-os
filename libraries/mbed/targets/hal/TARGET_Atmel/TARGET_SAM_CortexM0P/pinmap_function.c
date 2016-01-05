/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

#include "cmsis.h"
#include "mbed_assert.h"
#include "compiler.h"

#include "pinmap_function.h"

extern struct pwm_pin_channel pwn_pins[];

static uint32_t pinmap_merge_pins(uint32_t a, uint32_t b)
{
    /* both are the same (inc both NC) */
    if (a == b)
        return a;

    /* one (or both) is not connected */
    if (a == (uint32_t)NC)
        return b;
    if (b == (uint32_t)NC)
        return a;

    return (uint32_t)NC;
}

/** Find the SERCOM peripheral of given pin
 *
 * Find and return the SERCOM peripheral of input pin, either from default pads, or from extended pads
 * @param[in] pin1        First pin
 * @param[in] pad_select  to select which pad is to be used first to find
 * @return    SERCOM peripheral if found, else, NC
 */
uint32_t pinmap_find_peripheral_from_pad(PinName pin, enum sercom_pad_selection pad_select)
{
    uint32_t pin_sercom =(uint32_t)NC;

    if (pin == NC) return (uint32_t)NC;

    if (pad_select == SERCOM_USE_EXTENDED_PAD) {
        pin_sercom = pinmap_find_peripheral(pin, PinMap_SERCOM_PADEx);
    }
    if (pin_sercom == (uint32_t)NC) {
        pin_sercom = pinmap_find_peripheral(pin, PinMap_SERCOM_PAD);
    }

    return pin_sercom;
}

/** Find the common SERCOM shared by two pins
 *
 * Finds the common SERCOM index of two input pins.
 * Currently uses default pad only
 * @param[in] pin1  First pin
 * @param[in] pin2  Second pin
 * @return          SERCOM index if found, else, NC
 */
uint32_t pinmap_merge_sercom(PinName pin1, PinName pin2)
{
    uint32_t pin1_sercom, pin2_sercom;

    /* Using default pads for now */
    pin1_sercom = pinmap_find_peripheral_from_pad(pin1, SERCOM_USE_DEFAULT_PAD);
    if (pin1_sercom != (uint32_t)NC) {
        pin1_sercom &= 0x0F;
    }
    pin2_sercom = pinmap_find_peripheral_from_pad(pin2, SERCOM_USE_DEFAULT_PAD);
    if (pin2_sercom != (uint32_t)NC) {
        pin2_sercom &= 0x0F;
    }

    return pinmap_merge_pins(pin1_sercom, pin2_sercom);
}

/** Find the common SERCOM shared by four pins
 *
 * Finds the common SERCOM index shared by four input pins.
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
    uint32_t pin_com = (uint32_t)NC;

    sercom_index[0] = pinmap_find_peripheral_from_pad(pin1, SERCOM_USE_DEFAULT_PAD);
    sercom_index[1] = pinmap_find_peripheral_from_pad(pin2, SERCOM_USE_DEFAULT_PAD);
    sercom_index[2] = pinmap_find_peripheral_from_pad(pin3, SERCOM_USE_DEFAULT_PAD);
    sercom_index[3] = pinmap_find_peripheral_from_pad(pin4, SERCOM_USE_DEFAULT_PAD);

    /* Find common SERCOM, if there are conflicts, return NC */
    for (i=0; i<4; i++) {
        if (sercom_index[i] != (uint32_t)NC) {
            if (pin_com == (uint32_t)NC) {
                pin_com = sercom_index[i] & 0x0F;
            } else if (pin_com != (sercom_index[i] & 0x0F)) {
                return (uint32_t)NC;
            }
        }
    }

    return pin_com;
}

/** Find the MUX function of input pin specific to given SERCOM index
 *
 * @param[in] pin           Pin whose function is to be found out
 * @param[in] sercom_index  SERCOM index
 * @return                  MUX function if found, else, NC
 */
uint32_t pinmap_function_sercom(PinName pin, uint32_t sercom_index)
{
    uint32_t func = (uint32_t)NC;
    uint32_t index;
    sercom_index &= 0x0F;

    if ((pin == NC) || (sercom_index >= SERCOM_INST_NUM)) {
        return (uint32_t)NC;
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
    return (uint32_t)NC;
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
        return (uint32_t)NC;
    }
    index = pinmap_peripheral(pin, PinMap_SERCOM_PAD);
    if ((index & 0x0F) == sercom_index) {
        return ((index >> 4) & 0x0F);
    }
    index = pinmap_peripheral(pin, PinMap_SERCOM_PADEx);
    if ((index & 0x0F) == sercom_index) {
        return ((index >> 4) & 0x0F);
    }
    return (uint32_t)NC;
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
#if (SAMD21) || (SAMR21)
        0x42000800UL,  // Base address of SERCOM0
        0x42000C00UL,  // Base address of SERCOM1
        0x42001000UL,  // Base address of SERCOM2
        0x42001400UL,  // Base address of SERCOM3
        0x42001800UL,  // Base address of SERCOM4
        0x42001C00UL   // Base address of SERCOM5
#elif (SAML21)
        0x42000000UL,  // Base address of SERCOM0
        0x42000400UL,  // Base address of SERCOM1
        0x42000800UL,  // Base address of SERCOM2
        0x42000C00UL,  // Base address of SERCOM3
        0x42001000UL,  // Base address of SERCOM4
        0x43000400UL   // Base address of SERCOM5
#endif
    };
    uint32_t index = sercom_index & 0x0F;

    if (index >= SERCOM_INST_NUM) {
        return (uint32_t)NC;
    }
    return sercom_address[(sercom_index&0x0F)];
}

/** Find the channel index of a pin specific to a PWM instance
 *
 * @param[in] pin   pin name
 * @param[in] pwm   pwm peripheral (unused now)
 * @return          Channel index of the specified pin
 */
uint32_t pinmap_channel_pwm(PinName pin, PWMName pwm)
{
    struct pwm_pin_channel *pwm_ch =  pwn_pins;

    while (pwm_ch->pin != NC) {
        if (pin == pwm_ch->pin) {
            return (uint32_t)pwm_ch->channel_index;
        }
        pwm_ch++;
    }
    return (uint32_t)NC;
}
