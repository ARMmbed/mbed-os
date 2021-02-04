/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#ifndef MBED_DATAFLASH_BLOCK_DEVICE_H
#define MBED_DATAFLASH_BLOCK_DEVICE_H

#include "platform/PlatformMutex.h"
#include "PinNames.h"
#include "blockdevice/BlockDevice.h"
#include "drivers/SPI.h"
#include "drivers/DigitalOut.h"

#ifndef MBED_CONF_DATAFLASH_SPI_MOSI
#define MBED_CONF_DATAFLASH_SPI_MOSI NC
#endif
#ifndef MBED_CONF_DATAFLASH_SPI_MISO
#define MBED_CONF_DATAFLASH_SPI_MISO NC
#endif
#ifndef MBED_CONF_DATAFLASH_SPI_CLK
#define MBED_CONF_DATAFLASH_SPI_CLK NC
#endif
#ifndef MBED_CONF_DATAFLASH_SPI_CS
#define MBED_CONF_DATAFLASH_SPI_CS NC
#endif
#ifndef MBED_CONF_DATAFLASH_SPI_FREQ
#define MBED_CONF_DATAFLASH_SPI_FREQ 40000000
#endif

/** BlockDevice for DataFlash flash devices
 *
 *  @code
 *  // Here's an example using the AT45DB on the K64F
 *  #include "mbed.h"
 *  #include "DataFlashBlockDevice.h"
 *
 *  // Create DataFlash on SPI bus with PTE5 as chip select
 *  DataFlashBlockDevice dataflash(PTE2, PTE4, PTE1, PTE5);
 *
 *  // Create DataFlash on SPI bus with PTE6 as write-protect
 *  DataFlashBlockDevice dataflash2(PTE2, PTE4, PTE1, PTE5, PTE6);
 *
 *  int main() {
 *      printf("dataflash test\n");
 *
 *      // Initialize the SPI flash device and print the memory layout
 *      dataflash.init();
 *      printf("dataflash size: %llu\n", dataflash.size());
 *      printf("dataflash read size: %llu\n", dataflash.get_read_size());
 *      printf("dataflash program size: %llu\n", dataflash.get_program_size());
 *      printf("dataflash erase size: %llu\n", dataflash.get_erase_size());
 *
 *      // Write "Hello World!" to the first block
 *      char *buffer = (char*)malloc(dataflash.get_erase_size());
 *      sprintf(buffer, "Hello World!\n");
 *      dataflash.erase(0, dataflash.get_erase_size());
 *      dataflash.program(buffer, 0, dataflash.get_erase_size());
 *
 *      // Read back what was stored
 *      dataflash.read(buffer, 0, dataflash.get_erase_size());
 *      printf("%s", buffer);
 *
 *      // Deinitialize the device
 *      dataflash.deinit();
 *  }
 *  @endcode
 */
class DataFlashBlockDevice : public mbed::BlockDevice {
public:
    /** Creates a DataFlashBlockDevice on a SPI bus specified by pins
     *
     *  @param mosi     SPI master out, slave in pin
     *  @param miso     SPI master in, slave out pin
     *  @param sclk     SPI clock pin
     *  @param csel     SPI chip select pin
     *  @param nowp     GPIO not-write-protect
     *  @param freq     Clock speed of the SPI bus (defaults to 40MHz)
     *  @param nwp      Not-write-protected pin
     */
    DataFlashBlockDevice(PinName mosi = MBED_CONF_DATAFLASH_SPI_MOSI,
                         PinName miso = MBED_CONF_DATAFLASH_SPI_MISO,
                         PinName sclk = MBED_CONF_DATAFLASH_SPI_CLK,
                         PinName csel = MBED_CONF_DATAFLASH_SPI_CS,
                         int freq = MBED_CONF_DATAFLASH_SPI_FREQ,
                         PinName nwp = NC);

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

    /** Get the size of an erasable block given address
     *
     *  @param addr     Address within the erasable block
     *  @return         Size of an erasable block in bytes
     *  @note Must be a multiple of the program size
     */
    virtual mbed::bd_size_t get_erase_size(mbed::bd_addr_t addr) const;

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
    mbed::DigitalOut _nwp;

    // Device configuration
    uint32_t _device_size;
    uint16_t _page_size;
    uint16_t _block_size;
    bool _is_initialized;
    uint32_t _init_ref_count;

    // Internal functions
    uint16_t _get_register(uint8_t opcode);
    void _write_command(uint32_t command, const uint8_t *buffer, uint32_t size);
    void _write_enable(bool enable);
    int _sync(void);
    int _write_page(const uint8_t *buffer, uint32_t addr, uint32_t offset, uint32_t size);
    uint32_t _translate_address(mbed::bd_addr_t addr);

    // Mutex for thread safety
    mutable PlatformMutex _mutex;
};


#endif  /* MBED_DATAFLASH_BLOCK_DEVICE_H */
