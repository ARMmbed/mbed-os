
/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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
#ifndef MBED_PINMAP_H
#define MBED_PINMAP_H

#include "PinNames.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    PinName pin;
    int peripheral;
    int function;
} PinMap;

typedef struct {
    uint32_t count;
    const PinName *pins;
} PinList;

typedef struct {
    uint32_t count;
    const int *peripheral;
} PeripheralList;

void pin_function(PinName pin, int function);
void pin_mode(PinName pin, PinMode mode);

uint32_t pinmap_peripheral(PinName pin, const PinMap *map);
uint32_t pinmap_function(PinName pin, const PinMap *map);
uint32_t pinmap_merge(uint32_t a, uint32_t b);
void     pinmap_pinout(PinName pin, const PinMap *map);
uint32_t pinmap_find_peripheral(PinName pin, const PinMap *map);
uint32_t pinmap_find_function(PinName pin, const PinMap *map);

/**
 * Find a combination of pins suitable for use given the constraints
 *
 * This function finds pins which meet these specific properties:
 * - The pin is part of the form factor
 * - The pin is not in the restricted list
 * - The pin is contained within the respective pinmap
 * - The pin belongs to the given peripheral
 * - Each pin found is distinct; in the example below
 *      mosi and miso will never be assigned the same pin
 *
 * Example:
 * @code
 * #include "mbed.h"
 * #include "pinmap.h"
 *
 * int main()
 * {
 *     int per = spi_master_cs_pinmap()->peripheral;
 *     const PinList *pins_ff = pinmap_ff_default_pins();
 *     const PinList *pins_avoid = pinmap_restricted_pins();
 *     PinName mosi = NC;
 *     PinName miso = NC;
 *     PinName sclk = NC;
 *     PinName ssel = NC;
 *     const PinMap *maps[] = {
 *         spi_master_mosi_pinmap(),
 *         spi_master_miso_pinmap(),
 *         spi_master_clk_pinmap(),
 *         spi_master_cs_pinmap()
 *     };
 *     PinName *pins[] = {
 *         &mosi,
 *         &miso,
 *         &sclk,
 *         &ssel
 *     };
 *     if (pinmap_find_peripheral_pins(pins_ff, pins_avoid, per, maps, pins, sizeof(maps) / sizeof(maps[0]))) {
 *         printf("Found SPI pins to test instance %i with:\n"
 *                "  mosi=%s\n"
 *                "  miso=%s\n"
 *                "  sclk=%s\n"
 *                "  ssel=%s\n", per,
 *                pinmap_ff_default_pin_to_string(mosi),
 *                pinmap_ff_default_pin_to_string(miso),
 *                pinmap_ff_default_pin_to_string(sclk),
 *                pinmap_ff_default_pin_to_string(ssel));
 *     } else {
 *         printf("Could not find SPI combination to test %i\n", per);
 *     }
 *     return 0;
 * }
 * @endcode
 *
 * @param whitelist List of pins to choose from
 * @param blacklist List of pins which cannot be used
 * @param per Peripheral to which the pins belong
 * @param maps An array of pin maps to select from
 * @param pins An array of pins to find. Pins already set to a value will be
 *             left unchanged. Only pins initialized to NC will be updated by this function
 * @param count The size of maps and pins
 * @return true if a suitable combination of pins was found and
 *         written to the pins array, otherwise false
 */
bool pinmap_find_peripheral_pins(const PinList *whitelist, const PinList *blacklist, int per, const PinMap *const *maps, PinName **pins, uint32_t count);

/**
 * Check if the pin is in the list
 *
 * @param list pin list to check
 * @param pin pin to check for in the list
 * @return true if the pin is in the list, false otherwise
 */
bool pinmap_list_has_pin(const PinList *list, PinName pin);

/**
 * Check if the peripheral is in the list
 *
 * @param list peripheral list to check
 * @param peripheral peripheral to check for in the list
 * @return true if the peripheral is in the list, false otherwise
 */
bool pinmap_list_has_peripheral(const PeripheralList *list, int peripheral);

/**
 * Get the pin list of pins to avoid during testing
 *
 * The restricted pin list is used to indicate to testing
 * that a pin should be skipped due to some caveat about it.
 * For example, using USBRX and USBTX during tests will interfere
 * with the test runner and should be avoided.
 *
 * Targets should override the weak implementation of this
 * function if they have additional pins which should be
 * skipped during testing.
 *
 * @return Pointer to a pin list of pins to avoid
 */
const PinList *pinmap_restricted_pins(void);

/**
 * Get the pin list of peripherals per interface to avoid during testing
 *
 * The restricted peripheral list is used to indicate to testing
 * that a peripheral should be skipped due to some caveat about it.
 * For example, using the USB serial port during tests will interfere
 * with the test runner and should be avoided.
 *
 * Targets should override the weak implementation of this
 * function if they have peripherals which should be
 * skipped during testing.
 *
 * @note Restricting peripheral is at the moment available for UART
 *       interface only as only STDIO UART must be skipped because it is
 *       used by Mbed.
 *       Restricting peripherals for other interfaces should be added
 *       in the future if required.
 *
 * @return Pointer to a peripheral list of peripheral to avoid
 */
#if DEVICE_SERIAL
const PeripheralList *pinmap_uart_restricted_peripherals(void);
#endif

/**
 * Get the pin list of pins to avoid during GPIO/GPIO_IRQ testing
 *
 * The GPIO restricted pin list is used to indicate to testing
 * that a pin should be skipped due to some caveat about it.
 *
 * Targets should override the weak implementation of this
 * function if they have peripherals which should be
 * skipped during testing.
 *
 * @note This is special case only for GPIO/GPIO_IRQ tests because
 *       targets do not provide pin-maps for GPIO.
 *
 * @return Pointer to a peripheral list of peripheral to avoid
 */
const PinList *pinmap_gpio_restricted_pins(void);

#ifdef TARGET_FF_ARDUINO

/**
 * Get the pin list of the Arduino form factor
 *
 * @return Pointer to the Arduino pin list
 */
const PinList *pinmap_ff_arduino_pins(void);

/**
 * Get the string representation of a form factor pin
 *
 * @param pin Pin to get a string for
 * @return String representing the form factor pin
 */
const char *pinmap_ff_arduino_pin_to_string(PinName pin);

/* Default to arduino form factor if unspecified */
#ifndef MBED_CONF_TARGET_DEFAULT_FORM_FACTOR
#define MBED_CONF_TARGET_DEFAULT_FORM_FACTOR arduino
#endif

#endif

#ifdef MBED_CONF_TARGET_DEFAULT_FORM_FACTOR

#define PINMAP_DEFAULT_PINS_(name)              pinmap_ff_ ## name ## _pins
#define PINMAP_DEFAULT_PIN_TO_STRING_(name)     pinmap_ff_ ## name ## _pin_to_string
#define PINMAP_DEFAULT_PINS(name)               PINMAP_DEFAULT_PINS_(name)
#define PINMAP_DEFAULT_PIN_TO_STRING(name)      PINMAP_DEFAULT_PIN_TO_STRING_(name)
#define pinmap_ff_default_pins                  PINMAP_DEFAULT_PINS(MBED_CONF_TARGET_DEFAULT_FORM_FACTOR)
#define pinmap_ff_default_pin_to_string         PINMAP_DEFAULT_PIN_TO_STRING(MBED_CONF_TARGET_DEFAULT_FORM_FACTOR)

/**
 * Get the pin list of the default form factor
 *
 * This is an alias to whichever form factor is set
 * to be the default.
 *
 * @return Pointer to the default pin list
 */
const PinList *pinmap_ff_default_pins(void);

/**
 * Get the string representation of a form factor pin
 *
 * This is an alias to whichever form factor is set
 * to be the default.
 *
 * @param pin Pin to get a string for
 * @return String representing the form factor pin
 */
const char *pinmap_ff_default_pin_to_string(PinName pin);

#endif

#ifdef __cplusplus
}
#endif

#endif

/** @}*/
