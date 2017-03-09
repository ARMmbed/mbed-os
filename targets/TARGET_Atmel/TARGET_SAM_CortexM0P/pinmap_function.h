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
#ifndef PINMAP_FUNCTION_H
#define PINMAP_FUNCTION_H

#include <compiler.h>
#include "cmsis.h"
#include "PinNames.h"
#include "pinmux.h"
#include "pinmap.h"

#include "PeripheralPins.h"

#ifdef __cplusplus
extern "C" {
#endif

enum sercom_pad_selection {
    SERCOM_USE_DEFAULT_PAD,
    SERCOM_USE_EXTENDED_PAD,
};

/** Find the SERCOM peripheral of given pin
 *
 * Find and return the SERCOM peripheral of input pin, either from default pas, or from extended pads
 * @param[in] pin1  First pin
 * @param[in] pad_select  Second pin
 * @return    SERCOM peripheral if found, else, NC
 */
uint32_t pinmap_find_peripheral_from_pad(PinName pin, enum sercom_pad_selection pad_select);

/** Find the common SERCOM shared by two pins
 *
 * Finds the common SERCOM index of two input pins.
 * If swapping the input argument gives different result, it means, two SERCOMs share both pins
 * @param[in] pin1  First pin
 * @param[in] pin2  Second pin
 * @return          SERCOM index if found, else, NC
 */
uint32_t pinmap_merge_sercom(PinName pin1, PinName pin2);

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
uint32_t pinmap_find_sercom(PinName pin1, PinName pin2, PinName pin3, PinName pin4);

/** Find the MUX function of input pin specific to given SERCOM index
 *
 * @param[in] pin           Pin whose function is to be found out
 * @param[in] sercom_index  SERCOM index
 * @return                  MUX function if found, else, NC
 */
uint32_t pinmap_function_sercom(PinName pin, uint32_t sercom_index);

/** Find the MUX pad of input pin specific to given SERCOM index
 *
 * @param[in] pin           Pin whose function is to be found out
 * @param[in] sercom_index  SERCOM index
 * @return                  MUX pad if found, else, NC
 */
uint32_t pinmap_pad_sercom(PinName pin, uint32_t sercom_index);

/** Find the MUX function of input pin specific to given SERCOM index
 *
 * @param[in] pin           unused
 * @param[in] sercom_index  SERCOM index
 * @return                  base address to SERCOM if found, else NC
 */
uint32_t pinmap_peripheral_sercom(PinName pin, uint32_t sercom_index);

/** Find the channel index of a pin specific to a PWM instance
 *
 * @param[in] pin   pin name
 * @param[in] pwm   pwm peripheral (unused now)
 * @return          Channel index of the specified pin
 */
uint32_t pinmap_channel_pwm(PinName pin, PWMName pwm);

#ifdef __cplusplus
}
#endif

#endif /* PINMAP_FUNCTION_H */
