/* mbed Microcontroller Library
 * Copyright (c) 2006-2012 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __SD_CARD_INTERFACE_H__
#define __SD_CARD_INTERFACE_H__

#include <stdint.h>

class SDCardInterface {
public:
    /**
     * Initialize the SD Card in SPI mode.
     * @return 0 upon success
     */
    virtual int      disk_initialize();

    /**
     * Read a single sector from a given sector-aligned address.
     * @param  buffer
     * @param  addr
     *           Address at which to read a sector from.
     * @return 0 upon success.
     */
    virtual int      disk_read(uint8_t* buffer, uint64_t addr);

    /**
     * Write a single sector at a given sector-aligned address.
     * @param  buffer
     * @param  addr
     *           Address at which to write data.
     * @return 0 upon success.
     */
    virtual int      disk_write(const uint8_t* buffer, uint64_t addr);

    /**
     * @return the number of sectors available in the hardware.
     */
    virtual uint64_t disk_sectors();
};

#endif /* #if __SD_CARD_INTERFACE_H__ */
