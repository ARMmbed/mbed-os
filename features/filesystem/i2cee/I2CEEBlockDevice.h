/* Simple access class for I2C EEPROM chips like Microchip 24LC
 * Copyright (c) 2015 Robin Hourahane
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

/* If the target has no I2C support then I2CEEPROM is not supported */
#ifdef DEVICE_I2C
 
#include <mbed.h>
#include "BlockDevice.h"

 
/** BlockDevice for I2C based flash device such as
 *  Microchip's 24LC or ATMEL's AT24C ranges
 *
 *  @code
 *  #include "mbed.h"
 *  #include "I2CEEBlockDevice.h"
 *
 *  // Create 24LC device with 32Kbytes of memory 
 *  I2CEEBlockDevice flash(D14, D15, 0xa0, 32*1024);
 *
 *  int main() {
 *      printf("flash test\n");
 *      mx52r.init();
 *      printf("flash size: %llu\n", flash.size());
 *      printf("flash read size: %llu\n", flash.get_read_size());
 *      printf("flash program size: %llu\n", flash.get_program_size());
 *      printf("flash erase size: %llu\n", flash.get_erase_size());
 *
 *      uint8_t *buffer = malloc(flash.get_erase_size());
 *      sprintf(buffer, "Hello World!\n");
 *      flash.erase(0, flash.get_erase_size());
 *      flash.write(buffer, 0, flash.get_erase_size());
 *      flash.read(buffer, 0, flash.get_erase_size());
 *      printf("%s", buffer);
 *
 *      flash.deinit();
 *  }
 */
class I2CEEBlockDevice : public BlockDevice {
public:
    /** Constructor to create an I2CEEBlockDevice on I2C pins
     *
     *  @param sda      The pin name for the sda line of the I2C bus.
     *  @param scl      The pin name for the scl line of the I2C bus.
     *  @param addr     The 8bit I2C address of the chip, common range 0xa0 - 0xae.
     *  @param size     The size of the device in bytes
     *  @param block    The block size of the device in bytes, defaults to 32bytes
     *  @param freq     The frequency of the I2C bus, defaults to 400K.
     */
    I2CEEBlockDevice(
            PinName sda, PinName scl, uint8_t address,
            bd_size_t size, bd_size_t block=32,
            int bus_speed=400000);

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
     *  @param buffer   Buffer to write blocks to
     *  @param addr     Address of block to begin reading from
     *  @param size     Size to read in bytes, must be a multiple of read block size
     *  @return         0 on success, negative error code on failure
     */
    virtual bd_error_t read(void *buffer, bd_addr_t addr, bd_size_t size);

    /** Program blocks to a block device
     *
     *  The blocks must have been erased prior to being programmed
     *
     *  @param buffer   Buffer of data to write to blocks
     *  @param addr     Address of block to begin writing to
     *  @param size     Size to write in bytes, must be a multiple of program block size
     *  @return         0 on success, negative error code on failure
     */
    virtual bd_error_t program(const void *buffer, bd_addr_t addr, bd_size_t size);

    /** Erase blocks on a block device
     *
     *  The state of an erased block is undefined until it has been programmed
     *
     *  @param addr     Address of block to begin erasing
     *  @param size     Size to erase in bytes, must be a multiple of erase block size
     *  @return         0 on success, negative error code on failure
     */
    virtual bd_error_t erase(bd_addr_t addr, bd_size_t size);

    /** Get the size of a readable block
     *
     *  @return         Size of a readable block in bytes
     */
    virtual bd_size_t get_read_size();

    /** Get the size of a programable block
     *
     *  @return         Size of a programable block in bytes
     *  @note Must be a multiple of the read size
     */
    virtual bd_size_t get_program_size();

    /** Get the size of a eraseable block
     *
     *  @return         Size of a eraseable block in bytes
     *  @note Must be a multiple of the program size
     */
    virtual bd_size_t get_erase_size();

    /** Get the total size of the underlying device
     *
     *  @return         Size of the underlying device in bytes
     */
    virtual bd_size_t size();
    
private:
    I2C _i2c;
    uint8_t _i2c_addr;
    uint32_t _size;
    uint32_t _block;

    bd_error_t _sync();
};
 
#endif  /* DEVICE_SPI */

#endif  /* MBED_SD_BLOCK_DEVICE_H */
