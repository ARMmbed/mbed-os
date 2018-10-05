/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2016, MultiTech Systems
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of MultiTech nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

#ifndef __XDOT_EEPROM_H__
#define __XDOT_EEPROM_H__

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

/* xdot_eeprom_write_buf
 * attempts to write size bytes from buf to addr
 * implements read-before-write because writes are expensive
 * uses most efficient write possible (byte, half-word, or word write) based on alignment
 *  and number of bytes that need to be written
 * buf must be non-null and size bytes or larger
 * valid addresses are 0x0000 - 0x1FFF
 * returns 0 if all data was successfully written otherwise -1
 */
int xdot_eeprom_write_buf(uint32_t addr, uint8_t *buf, uint32_t size);

/* xdot_eeprom_read_buf
 * attempts to read size bytes into buf starting at addr
 * buf must be non-null and size bytes or larger
 * valid addresses are 0x0000 - 0x1FFF
 * returns 0 if all data was successfully read otherwise -1
 */
int xdot_eeprom_read_buf(uint32_t addr, uint8_t *buf, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* __XDOT_EEPROM_H__ */
