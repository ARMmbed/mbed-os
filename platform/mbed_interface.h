/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#ifndef MBED_INTERFACE_H
#define MBED_INTERFACE_H

#include <stdarg.h>

#include "platform/mbed_toolchain.h"
#include "device.h"

/** \addtogroup platform-public-api */
/** @{*/

/**
 * \defgroup platform_interface Network interface and other utility functions
 * @{
 */

/* Mbed interface mac address
 * if MBED_MAC_ADD_x are zero, interface uid sets mac address,
 * otherwise MAC_ADD_x are used.
 */
#define MBED_MAC_ADDR_INTERFACE 0x00
#define MBED_MAC_ADDR_0  MBED_MAC_ADDR_INTERFACE
#define MBED_MAC_ADDR_1  MBED_MAC_ADDR_INTERFACE
#define MBED_MAC_ADDR_2  MBED_MAC_ADDR_INTERFACE
#define MBED_MAC_ADDR_3  MBED_MAC_ADDR_INTERFACE
#define MBED_MAC_ADDR_4  MBED_MAC_ADDR_INTERFACE
#define MBED_MAC_ADDR_5  MBED_MAC_ADDR_INTERFACE
#define MBED_MAC_ADDRESS_SUM (MBED_MAC_ADDR_0 | MBED_MAC_ADDR_1 | MBED_MAC_ADDR_2 | MBED_MAC_ADDR_3 | MBED_MAC_ADDR_4 | MBED_MAC_ADDR_5)

#ifdef __cplusplus
extern "C" {
#endif

#if DEVICE_SEMIHOST

/**
 * \defgroup platform_interface interface functions
 * @{
 */

/** Functions to control the mbed interface
 *
 * mbed Microcontrollers have a built-in interface to provide functionality such as
 * drag-n-drop download, reset, serial-over-usb, and access to the mbed local file
 * system. These functions provide means to control the interface using semihost
 * calls it supports.
 */

/** Determine whether the mbed interface is connected, based on whether debug is enabled
 *
 *  @returns
 *    1 if interface is connected,
 *    0 otherwise
 */
int mbed_interface_connected(void);

/** Instruct the mbed interface to reset, as if the reset button had been pressed
 *
 *  @returns
 *    1 if successful,
 *    0 otherwise (e.g. interface not present)
 */
int mbed_interface_reset(void);

/** This will disconnect the debug aspect of the interface, so semihosting will be disabled.
 * The interface will still support the USB serial aspect
 *
 *  @returns
 *    0 if successful,
 *   -1 otherwise (e.g. interface not present)
 */
int mbed_interface_disconnect(void);

/** This will disconnect the debug aspect of the interface, and if the USB cable is not
 * connected, also power down the interface. If the USB cable is connected, the interface
 * will remain powered up and visible to the host
 *
 *  @returns
 *    0 if successful,
 *   -1 otherwise (e.g. interface not present)
 */
int mbed_interface_powerdown(void);

/** This returns a string containing the 32-character UID of the mbed interface
 *  This is a weak function that can be overwritten if required
 *
 *  @param uid A 33-byte array to write the null terminated 32-byte string
 *
 *  @returns
 *    0 if successful,
 *   -1 otherwise (e.g. interface not present)
 */
int mbed_interface_uid(char *uid);

#endif

/** This returns a unique 6-byte MAC address, based on the interface UID
 * If the interface is not present, it returns a default fixed MAC address (00:02:F7:F0:00:00)
 *
 * This is a weak function that can be overwritten if you want to provide your own mechanism to
 * provide a MAC address.
 *
 *  @param mac A 6-byte array to write the MAC address
 */
void mbed_mac_address(char *mac);

/** Cause the mbed to flash the BLOD (Blue LEDs Of Death) sequence
 */
MBED_NORETURN void mbed_die(void);

/** Print out an error message.  This is typically called when
 * handling a crash.
 *
 * @note Synchronization level: Interrupt safe, as long as the
 *       FileHandle::write of the stderr device is. See mbed_error_puts
 *       for more information.
 * @note This uses an internal 128-byte buffer to format the string,
 *       so the output may be truncated. If you need to write a potentially
 *       long string, use mbed_error_puts.
 *
 * @param format    C string that contains data stream to be printed.
 *                  Code snippets below show valid format.
 *
 * @code
 * mbed_error_printf("Failed: %s, file: %s, line %d \n", expr, file, line);
 * @endcode
 *
 */
void mbed_error_printf(const char *format, ...) MBED_PRINTF(1, 2);

/** Print out an error message.  Similar to mbed_error_printf
 * but uses a va_list.
 *
 * @note Synchronization level: Interrupt safe, as long as the
 *       FileHandle::write of the stderr device is. See mbed_error_puts
 *       for more information.
 *
 * @param format    C string that contains data stream to be printed.
 * @param arg       Variable arguments list
 *
 */
void mbed_error_vprintf(const char *format, va_list arg) MBED_PRINTF(1, 0);

/** Print out an error message. This is typically called when
 * handling a crash.
 *
 * Unlike mbed_error_printf, there is no limit to the maximum output
 * length. Unlike standard puts, but like standard fputs, this does not
 * append a '\n' character.
 *
 * @note Synchronization level: Interrupt safe, as long as the
 *       FileHandle::write of the stderr device is. The default
 *       serial console is safe, either buffered or not. If the
 *       console has not previously been initialized, an attempt
 *       to use this from interrupt may crash during console initialization.
 *       Special handling of `mbed_error` relaxes various system traps
 *       to increase the chance of initialization working.
 *
 * @param str    C string that contains data stream to be printed.
 *
 */
void mbed_error_puts(const char *str);

/** @}*/


#ifdef __cplusplus
}
#endif

#endif

/** @}*/
