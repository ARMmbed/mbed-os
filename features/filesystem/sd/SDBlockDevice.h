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
#ifndef MBED_SD_BLOCK_DEVICE_H
#define MBED_SD_BLOCK_DEVICE_H

/* If the target has no SPI support then SDCard is not supported */
#ifdef DEVICE_SPI

#include "BlockDevice.h"
#include "mbed.h"


/** Access an SD Card using SPI
 *
 * @code
 * #include "mbed.h"
 * #include "SDBlockDevice.h"
 *
 * SDBlockDevice sd(p5, p6, p7, p12); // mosi, miso, sclk, cs
 * uint8_t block[512] = "Hello World!\n";
 *
 * int main() {
 *     sd.init();
 *     sd.write(block, 0);
 *     sd.read(block, 0);
 *     printf("%s", block);
 *     sd.deinit();
 * }
 */
class SDBlockDevice : public BlockDevice {
public:
    /** Lifetime of an SD card
     */
    SDBlockDevice(PinName mosi, PinName miso, PinName sclk, PinName cs);
    virtual ~SDBlockDevice();

    /** Initialize a block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual bd_error_t init();

    /** Deinitialize a block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual bd_error_t deinit();

    /** Read blocks from a block device
     *
     *  @param buffer   Buffer to write block to
     *  @param block    Index of block to begin reading from
     *  @param count    Number of blocks to read (defaults to 1)
     *  @return         Either the number of blocks read or a negative error code on failure
     */
    virtual bd_count_or_error_t read(void *buffer, bd_block_t block, bd_count_t count = 1);

    /** Write blocks to a block device
     *
     *  @param buffer   Buffer of data to write to blocks
     *  @param block    Index of block to begin writing to
     *  @param count    Number of blocks to write (defaults to 1)
     *  @return         Either the number of blocks written or a negative error code on failure
     */
    virtual bd_count_or_error_t write(const void *buffer, bd_block_t block, bd_count_t count = 1);

    /** Queries the status of the underlying block device
     *
     *  @return         Status of the underlying device, this is 0 for
     *                  a ready device, or a negative error code if a read
     *                  or write could fail.
     */
    virtual bd_error_t status();

    /** Get the block size of the underlying device
     *
     *  @return         The number of 8-bit bytes in a block
     */
    virtual bd_size_t get_block_size();

    /** Get the block count of the underlying device
     *
     *  @return         The number of blocks on the device
     */
    virtual bd_count_t get_block_count();

    /** Enable or disable debugging
     *
     *  @param          State of debugging
     */
    virtual void debug(bool dbg);

protected:
    int _cmd(int cmd, int arg);
    int _cmdx(int cmd, int arg);
    int _cmd8();
    int _cmd58();
    bd_error_t initialise_card();
    bd_error_t initialise_card_v1();
    bd_error_t initialise_card_v2();

    int _read(uint8_t * buffer, uint32_t length);
    int _write(const uint8_t *buffer, uint32_t length);
    uint32_t _sd_sectors();
    uint32_t _sectors;

    void set_init_sck(uint32_t sck) { _init_sck = sck; }
    // Note: The highest SPI clock rate is 20 MHz for MMC and 25 MHz for SD
    void set_transfer_sck(uint32_t sck) { _transfer_sck = sck; }
    uint32_t _init_sck;
    uint32_t _transfer_sck;

    SPI _spi;
    DigitalOut _cs;
    int cdv;
    int _is_initialized;
    bool _dbg;
    Mutex _lock;
};


#endif  /* DEVICE_SPI */

#endif  /* MBED_SD_BLOCK_DEVICE_H */
