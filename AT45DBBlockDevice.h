/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#ifndef MBED_AT45DB_BLOCK_DEVICE_H
#define MBED_AT45DB_BLOCK_DEVICE_H

#include <mbed.h>
#include "BlockDevice.h"


/** BlockDevice for AT45DB flash devices
 *
 *  @code
 *  #include "mbed.h"
 *  #include "AT45DBBlockDevice.h"
 *
 *  // Create at45db on SPI bus with PTE5 as chip select
 *  AT45DBBlockDevice at45db(PTE2, PTE4, PTE1, PTE5);
 *
 *  // Create at45db on SPI bus with PTE6 as write-protect
 *  AT45DBBlockDevice at45db(PTE2, PTE4, PTE1, PTE5, PTE6);
 *
 *  int main() {
 *      printf("at45db test\n");
 *      at45db.init();
 *      printf("at45db size: %llu\n", at45db.size());
 *      printf("at45db read size: %llu\n", at45db.get_read_size());
 *      printf("at45db program size: %llu\n", at45db.get_program_size());
 *      printf("at45db erase size: %llu\n", at45db.get_erase_size());
 *
 *      uint8_t *buffer = malloc(at45db.get_erase_size());
 *      sprintf(buffer, "Hello World!\n");
 *      at45db.erase(0, at45db.get_erase_size());
 *      at45db.program(buffer, 0, at45db.get_erase_size());
 *      at45db.read(buffer, 0, at45db.get_erase_size());
 *      printf("%s", buffer);
 *
 *      at45db.deinit();
 *  }
 */
class AT45DBBlockDevice : public BlockDevice {
public:
    /** Creates a AT45DBBlockDevice on a SPI bus specified by pins
     *
     *  @param mosi     SPI master out, slave in pin
     *  @param miso     SPI master in, slave out pin
     *  @param sclk     SPI clock pin
     *  @param csel     SPI chip select pin
     *  @param nowp     GPIO not-write-protect
     *  @param freq     Clock speed of the SPI bus (defaults to 40MHz)
     */
    AT45DBBlockDevice(PinName mosi,
                      PinName miso,
                      PinName sclk,
                      PinName csel,
                      int freq = 40000000,
                      PinName nowp = NC);

    /** Initialize a block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual int init();

    /** Deinitialize a block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual int deinit();

    /** Read blocks from a block device
     *
     *  @param buffer   Buffer to write blocks to
     *  @param addr     Address of block to begin reading from
     *  @param size     Size to read in bytes, must be a multiple of read block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int read(void *buffer, bd_addr_t addr, bd_size_t size);

    /** Program blocks to a block device
     *
     *  The blocks must have been erased prior to being programmed
     *
     *  @param buffer   Buffer of data to write to blocks
     *  @param addr     Address of block to begin writing to
     *  @param size     Size to write in bytes, must be a multiple of program block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int program(const void *buffer, bd_addr_t addr, bd_size_t size);

    /** Erase blocks on a block device
     *
     *  The state of an erased block is undefined until it has been programmed
     *
     *  @param addr     Address of block to begin erasing
     *  @param size     Size to erase in bytes, must be a multiple of erase block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int erase(bd_addr_t addr, bd_size_t size);

    /** Get the size of a readable block
     *
     *  @return         Size of a readable block in bytes
     */
    virtual bd_size_t get_read_size() const;

    /** Get the size of a programable block
     *
     *  @return         Size of a programable block in bytes
     *  @note Must be a multiple of the read size
     */
    virtual bd_size_t get_program_size() const;

    /** Get the size of a eraseable block
     *
     *  @return         Size of a eraseable block in bytes
     *  @note Must be a multiple of the program size
     */
    virtual bd_size_t get_erase_size() const;

    /** Get the total size of the underlying device
     *
     *  @return         Size of the underlying device in bytes
     */
    virtual bd_size_t size() const;

private:
    // Master side hardware
    SPI _spi;
    DigitalOut _cs;
    DigitalOut _nwp;

    // Device configuration
    uint32_t _device_size;
    uint16_t _page_size;
    uint16_t _block_size;

    // Internal functions
    uint16_t _get_register(uint8_t opcode);
    void _write_command(uint32_t command, const uint8_t *buffer, uint32_t size);
    void _write_enable(bool enable);
    int _sync(void);
    int _write_page(const uint8_t *buffer, uint32_t addr, uint32_t offset, uint32_t size);
    uint32_t _translate_address(bd_addr_t addr);
};


#endif  /* MBED_AT45DB_BLOCK_DEVICE_H */
