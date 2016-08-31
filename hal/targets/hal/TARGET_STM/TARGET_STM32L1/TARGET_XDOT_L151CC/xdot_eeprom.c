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

#include "xdot_eeprom.h"

#define XDOT_EEPROM_START       0x08080000
#define XDOT_EEPROM_SIZE        0x00002000

typedef union {
    uint32_t* w;
    uint8_t* b;
} b2w;

typedef union {
    uint16_t* hw;
    uint8_t* b;
} b2hw;

enum {
    byte_write = 0,
    hword_write,
    word_write
};

static int xdot_eeprom_write_byte(uint32_t addr, uint8_t data) {
    if (addr > XDOT_EEPROM_SIZE - 1) {
        return -1;
    }

    if (HAL_FLASHEx_DATAEEPROM_Program(TYPEPROGRAMDATA_BYTE, XDOT_EEPROM_START + addr, (uint32_t)data) != HAL_OK) {
        return -1;
    } else {
        return 0;
    }
}

static int xdot_eeprom_write_hword(uint32_t addr, uint16_t data) {
    if (addr > XDOT_EEPROM_SIZE - 2) {
        return -1;
    }

    if (HAL_FLASHEx_DATAEEPROM_Program(TYPEPROGRAMDATA_HALFWORD, XDOT_EEPROM_START + addr, (uint32_t)data) != HAL_OK) {
        return -1;
    } else {
        return 0;
    }
}

static int xdot_eeprom_write_word(uint32_t addr, uint32_t data) {
    if (addr > XDOT_EEPROM_SIZE - 4) {
        return -1;
    }

    if (HAL_FLASHEx_DATAEEPROM_Program(TYPEPROGRAMDATA_WORD, XDOT_EEPROM_START + addr, (uint32_t)data) != HAL_OK) {
        return -1;
    } else {
        return 0;
    }
}

static int xdot_eeprom_read_byte(uint32_t addr, uint8_t* data) {
    if (addr > XDOT_EEPROM_SIZE - 1) {
        return -1;
    }

    *data = (*((uint8_t*)(XDOT_EEPROM_START + addr)));

    return 0;
}


int xdot_eeprom_write_buf(uint32_t addr, uint8_t* buf, uint32_t size) {
    uint32_t bytes_written = 0;

    if (addr + size > XDOT_EEPROM_SIZE) {
        return -1;
    }

    HAL_FLASHEx_DATAEEPROM_Unlock();

    while (bytes_written < size) {
        uint32_t bytes_remaining = size - bytes_written;
        uint32_t current_addr = addr + bytes_written;
        uint8_t write_type = 0xFF;
        uint8_t mismatch[4];
        uint8_t match_byte = 0xFF;
        b2w byte2word;
        b2hw byte2hword;
        //printf("0x%04X (%lu) bytes remaining\r\n", bytes_remaining, bytes_remaining);
        //printf("addr 0x%04X\t%d\r\n", current_addr, current_addr % 4);

        mismatch[0] = mismatch[1] = mismatch[2] = mismatch[3] = 0;

        if ((current_addr % 4 == 0) && bytes_remaining >= 4) {
            // aligned for word write
            //printf("aligned for word write\r\n");
            //printf("addr\tcurrent\t\tnew\r\n");
            for (int i = 0; i < 4; i++) {
                if (xdot_eeprom_read_byte(current_addr + i, &match_byte)) {
                    HAL_FLASHEx_DATAEEPROM_Lock();
                    return -1;
                }
                //printf("0x%04X\t0x%08X\t0x%08X\t", current_addr + i, match_byte, buf[bytes_written + i]);
                if (match_byte != buf[bytes_written + i]) {
                    mismatch[i] = 1;
                }
                //printf("%smatch\r\n", mismatch[i] ? "mis" : "");
            }
            if (! (mismatch[0] || mismatch[1] || mismatch[2] || mismatch[3])) {
                //printf("all match - no write necessary\r\n");
                bytes_written += 4;
                continue;
            }
            if ((mismatch[0] || mismatch[1]) && (mismatch[2] || mismatch[3])) {
                // if at least one of byte 1 or byte 2 and one of byte 3 or byte 4 needs to be written, write a word
                // this should also account for all 3 or 4 byte combinations as well
                write_type = word_write;
            } else if ((mismatch[0] && mismatch[1]) || (mismatch[2] && mismatch[3])) {
                // if only bytes 1 and 2 or only bytes 3 and 4 need to be written, write a half-word
                write_type = hword_write;
                // increment bytes_written if we're skipping bytes that match
                if (mismatch[2] && mismatch[3]) {
                    bytes_written += 2;
                    current_addr += 2;
                }
            } else if (mismatch[0] || mismatch[1] || mismatch[2] || mismatch[3]) {
                // anything else is just a byte write
                write_type = byte_write;
                // increment bytes_written if we're skipping bytes that match
                if (mismatch[1]) {
                    bytes_written += 1;
                    current_addr += 1;
                } else if (mismatch[2]) {
                    bytes_written += 2;
                    current_addr += 2;
                } else if (mismatch[3]) {
                    bytes_written += 3;
                    current_addr += 3;
                }
            }
        } else if ((current_addr % 2 == 0) && bytes_remaining >= 2) {
            // aligned for half word write
            //printf("aligned for half word write\r\n");
            //printf("addr\tcurrent\t\tnew\r\n");
            for (int i = 0; i < 2; i++) {
                if (xdot_eeprom_read_byte(current_addr + i, &match_byte)) {
                    HAL_FLASHEx_DATAEEPROM_Lock();
                    return -1;
                }
                //printf("0x%04X\t0x%08X\t0x%08X\t", current_addr + i, match_byte, buf[bytes_written + i]);
                if (match_byte != buf[bytes_written + i]) {
                    mismatch[i] = 1;
                }
                //printf("%smatch\r\n", mismatch[i] ? "mis" : "");
            }
            if (! (mismatch[0] || mismatch[1])) {
                //printf("all match - no write necessary\r\n");
                bytes_written += 2;
                continue;
            }
            if (mismatch[0] && mismatch[1]) {
                // if bytes 1 and 2 need to be written, we truly want to write a half word
                write_type = hword_write;
            } else {
                // anything else is just a byte write
                write_type = byte_write;
                // increment bytes_written if we're skipping bytes that match
                if (mismatch[1]) {
                    bytes_written += 1;
                    current_addr += 1;
                }
            }
        } else {
            // aligned for byte write
            //printf("aligned for byte write\r\n");
            //printf("addr\tcurrent\t\tnew\r\n");
            for (int i = 0; i < 1; i++) {
                if (xdot_eeprom_read_byte(current_addr + i, &match_byte)) {
                    HAL_FLASHEx_DATAEEPROM_Lock();
                    return -1;
                }
                //printf("0x%04X\t0x%08X\t0x%08X\t", current_addr + i, match_byte, buf[bytes_written + i]);
                if (match_byte != buf[bytes_written + i]) {
                    mismatch[i] = 1;
                }
                //printf("%smatch\r\n", mismatch[i] ? "mis" : "");
            }
            if (! mismatch[0]) {
                //printf("all match - no write necessary\r\n");
                bytes_written += 1;
                continue;
            }
            write_type = byte_write;
        }

        switch (write_type) {
            case word_write:
                // we can write a word (32 bits)
                //printf("word write\r\n");
                byte2word.b = buf + bytes_written;
                if (xdot_eeprom_write_word(current_addr, *byte2word.w)) {
                    HAL_FLASHEx_DATAEEPROM_Lock();
                    return -1;
                }
                bytes_written += 4;
                break;
            case hword_write:
                // we can write a half-word (16 bits)
                //printf("half-word write\r\n");
                byte2hword.b = buf + bytes_written;
                if (xdot_eeprom_write_hword(current_addr, *byte2hword.hw)) {
                    HAL_FLASHEx_DATAEEPROM_Lock();
                    return -1;
                }
                bytes_written += 2;
                break;
            case byte_write:
                // we can write a byte (8 bits)
                //printf("byte write\r\n");
                if (xdot_eeprom_write_byte(current_addr, buf[bytes_written])) {
                    HAL_FLASHEx_DATAEEPROM_Lock();
                    return -1;
                }
                bytes_written += 1;
                break;
            default:
                //printf("no write needed\r\n");
                break;
        }
    }

    HAL_FLASHEx_DATAEEPROM_Lock();

    return 0;
}

int xdot_eeprom_read_buf(uint32_t addr, uint8_t* buf, uint32_t size) {
    if (addr + size > XDOT_EEPROM_SIZE) {
        return -1;
    }

    HAL_FLASHEx_DATAEEPROM_Unlock();

    for (uint32_t i = 0; i < size; i++) {
        if (xdot_eeprom_read_byte(addr + i, buf + i)) {
            HAL_FLASHEx_DATAEEPROM_Lock();
            return -1;
        }
    }

    HAL_FLASHEx_DATAEEPROM_Lock();

    return 0;
}

