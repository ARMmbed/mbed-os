/* Simple access class for I2C EEPROM chips like Microchip 24LC
 * Copyright (c) 2015 Robin Hourahane
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
#ifndef MBED_I2CEEPROM_BLOCK_DEVICE_H
#define MBED_I2CEEPROM_BLOCK_DEVICE_H

#include "blockdevice/BlockDevice.h"
#include "drivers/I2C.h"

/** BlockDevice for I2C based flash device such as
 *  Microchip's 24LC or ATMEL's AT24C ranges
 *
 *  @code
 *  // Here's an example using a 24LC256 on a GR PEACH
 *  #include "mbed.h"
 *  #include "I2CEEBlockDevice.h"
 *
 *  // Create EEPROM device on I2C bus with 32kbytes of memory
 *  I2CEEBlockDevice i2cee(D14, D15, 0xa0, 32*1024);
 *
 *  int main() {
 *      printf("i2cee test\n");
 *
 *      // Initialize the device and print the memory layout
 *      i2cee.init();
 *      printf("i2cee size: %llu\n",         i2cee.size());
 *      printf("i2cee read size: %llu\n",    i2cee.get_read_size());
 *      printf("i2cee program size: %llu\n", i2cee.get_program_size());
 *      printf("i2cee erase size: %llu\n",   i2cee.get_erase_size());
 *
 *      // Write "Hello World!" to the first block
 *      char *buffer = (char*)malloc(i2cee.get_erase_size());
 *      sprintf(buffer, "Hello World!\n");
 *      i2cee.erase(0, i2cee.get_erase_size());
 *      i2cee.program(buffer, 0, i2cee.get_erase_size());
 *
 *      // Read back what was stored
 *      i2cee.read(buffer, 0, i2cee.get_erase_size());
 *      printf("%s", buffer);
 *
 *      // Deinitialize the device
 *      i2cee.deinit();
 *  }
 *  @endcode
 */
class I2CEEBlockDevice : public BlockDevice {
public:
    /** Constructor to create an I2CEEBlockDevice on I2C pins
     *
     *  @param sda                  The pin name for the sda line of the I2C bus.
     *  @param scl                  The pin name for the scl line of the I2C bus.
     *  @param addr                 The 8bit I2C address of the chip, common range 0xa0 - 0xae.
     *  @param size                 The size of the device in bytes
     *  @param block                The page size of the device in bytes, defaults to 32bytes
     *  @param freq                 The frequency of the I2C bus, defaults to 400K.
     *  @param address_is_eight_bit Specifies whether the EEPROM device is using eight bit
     *                              addresses instead of 16 bit addresses. This is used for example
     *                              in AT24C series chips.
     */
    I2CEEBlockDevice(
        PinName sda, PinName scl, uint8_t address,
        mbed::bd_size_t size, mbed::bd_size_t block = 32,
        int bus_speed = 400000,
        bool address_is_eight_bit = false);

    /** Constructor to create an I2CEEBlockDevice on I2C pins
     *
     *  @param i2c                  The I2C instance pointer
     *  @param addr                 The 8bit I2C address of the chip, common range 0xa0 - 0xae.
     *  @param size                 The size of the device in bytes
     *  @param block                The page size of the device in bytes, defaults to 32bytes
     *  @param freq                 The frequency of the I2C bus, defaults to 400K.
     *  @param address_is_eight_bit Specifies whether the EEPROM device is using eight bit
     *                              addresses instead of 16 bit addresses. This is used for example
     *                              in AT24C series chips.
     */
    I2CEEBlockDevice(
        mbed::I2C *i2c_obj, uint8_t address,
        mbed::bd_size_t size, mbed::bd_size_t block = 32,
        bool address_is_eight_bit = false);

    /** Destructor of I2CEEBlockDevice
     */

    virtual ~I2CEEBlockDevice();

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

    /** Get the total size of the underlying device
     *
     *  @return         Size of the underlying device in bytes
     */
    virtual mbed::bd_size_t size() const;

    /** Get the BlockDevice class type.
     *
     *  @return         A string representation of the BlockDevice class type.
     */
    virtual const char *get_type() const;

private:
    mbed::I2C *_i2c;
    uint32_t _i2c_buffer[sizeof(mbed::I2C) / sizeof(uint32_t)];
    uint8_t _i2c_addr;
    bool _address_is_eight_bit;
    uint32_t _size;
    uint32_t _block;

    int _sync();

    /**
     * Gets the device's I2C address with respect to the requested page.
     * When eight-bit mode is disabled, this function is a noop.
     * When eight-bit mode is enabled, it sets the bits required
     * in the devices address. Other bits remain unchanged.
     * @param address An address in the requested page.
     * @return The device's I2C address for that page
     */
    uint8_t get_paged_device_address(mbed::bd_addr_t address);
};


#endif  /* MBED_SD_BLOCK_DEVICE_H */
