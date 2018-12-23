/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#ifndef MBED_RSPIF_BLOCK_DEVICE_H
#define MBED_RSPIF_BLOCK_DEVICE_H

#include "SPI.h"
#include "DigitalOut.h"
#include "BlockDevice.h"

/** Reduced BlockDevice for SPI based flash devices
 *  *Should only be used by Boot Loader*
 *
 *  @code
 *  // Here's an example using the SPI flash device on K82F
 *  #include "mbed.h"
 *  #include "SPIFReducedBlockDevice.h"
 *
 *  // Create flash device on SPI bus with PTE5 as chip select
 *  SPIFReducedBlockDevice rspif(PTE2, PTE4, PTE1, PTE5);
 *
 *  int main() {
 *      printf("reduced spif test\n");
 *
 *      // Initialize the Reduced SPI flash device and print the memory layout
 *      rspif.init();
 *      printf("rspif size: %llu\n",         rspif.size());
 *      printf("rspif read size: %llu\n",    rspif.get_read_size());
 *      printf("rspif program size: %llu\n", rspif.get_program_size());
 *      printf("rspif erase size: %llu\n",   rspif.get_erase_size());
 *
 *      // Write "Hello World!" to the first block
 *      char *buffer = (char*)malloc(rspif.get_erase_size());
 *      sprintf(buffer, "Hello World!\n");
 *      rspif.erase(0, rspif.get_erase_size());
 *      rspif.program(buffer, 0, rspif.get_erase_size());
 *
 *      // Read back what was stored
 *      rspif.read(buffer, 0, rspif.get_erase_size());
 *      printf("%s", buffer);
 *
 *      // Deinitialize the device
 *      rspif.deinit();
 *  }
 *  @endcode
 */
class SPIFReducedBlockDevice : public mbed::BlockDevice {
public:
    /** Creates a SPIFReducedBlockDevice on a SPI bus specified by pins
     *
     *  @param mosi     SPI master out, slave in pin
     *  @param miso     SPI master in, slave out pin
     *  @param sclk     SPI clock pin
     *  @param csel     SPI chip select pin
     *  @param freq     Clock speed of the SPI bus (defaults to 40MHz)
     */
    SPIFReducedBlockDevice(PinName mosi, PinName miso, PinName sclk, PinName csel, int freq = 40000000);

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
    virtual int read(void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Program blocks to a block device
     *
     *  The blocks must have been erased prior to being programmed
     *
     *  @param buffer   Buffer of data to write to blocks
     *  @param addr     Address of block to begin writing to
     *  @param size     Size to write in bytes, must be a multiple of program block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int program(const void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Erase blocks on a block device
     *
     *  The state of an erased block is undefined until it has been programmed
     *
     *  @param addr     Address of block to begin erasing
     *  @param size     Size to erase in bytes, must be a multiple of erase block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int erase(mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Get the size of a readable block
     *
     *  @return         Size of a readable block in bytes
     */
    virtual mbed::bd_size_t get_read_size() const;

    /** Get the size of a programable block
     *
     *  @return         Size of a programable block in bytes
     *  @note Must be a multiple of the read size
     */
    virtual mbed::bd_size_t get_program_size() const;

    /** Get the size of a eraseable block
     *
     *  @return         Size of a eraseable block in bytes
     *  @note Must be a multiple of the program size
     */
    virtual mbed::bd_size_t get_erase_size() const;

    /** Get the size of a eraseable block
     *
     *  @param addr     Address of block to query erase size
     *  @return         Size of a eraseable block in bytes
     *  @note Must be a multiple of the program size
     */
    virtual mbed::bd_size_t get_erase_size(mbed::bd_addr_t addr) const;

    /** Get the value of storage byte after it was erased
     *
     *  If get_erase_value returns a non-negative byte value, the underlying
     *  storage is set to that value when erased, and storage containing
     *  that value can be programmed without another erase.
     *
     *  @return         The value of storage when erased, or -1 if you can't
     *                  rely on the value of erased storage
     */
    virtual int get_erase_value() const;

    /** Get the total size of the underlying device
     *
     *  @return         Size of the underlying device in bytes
     */
    virtual mbed::bd_size_t size() const;

    /** Get the BlockDevice class type.
     *
     *  @return         A string represent the BlockDevice class type.
     */
    virtual const char *get_type() const;

private:
    // Master side hardware
    mbed::SPI _spi;
    mbed::DigitalOut _cs;

    // Device configuration discovered through sfdp
    mbed::bd_size_t _size;

    // Internal functions
    int _wren();
    int _sync();
    void _cmdread(uint8_t op, uint32_t addrc, uint32_t retc,
                  uint32_t addr, uint8_t *rets);
    void _cmdwrite(uint8_t op, uint32_t addrc, uint32_t argc,
                   uint32_t addr, const uint8_t *args);
};
#endif
