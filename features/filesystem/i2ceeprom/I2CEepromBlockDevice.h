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

 
/** Class to provide simple access to I2C EEPROM chiles like Microchip's 24LC range
 *  or AMTELS AT24C range.
 *  Chips up to 64Kb in size are directly supported.
 *  The class handles multiple page writes so any amount of data can be written in
 *  a single call to write. The writes are not buffered so additional memory use 
 *  is kept to a minimum.
 * 
 *  Although not tested it should work with the MAC versions of Microchip range
 *  as well but the chipSize will need to be set to include the ROM area as well.
 */
class I2CEepromBlockDevice : public BlockDevice {
public:
    /** Constructor to create a new instance of the class.
     *
     *  @param sda          The pin name for the sda line of the I2C bus.
     *  @param scl          The pin name for the scl line of the I2C bus.
     *  @param address      The 8bit I2C address of the chip in the range 0xA0 - 0xAE.
     *  @param block_size   The size of the page used in writing to the chip. Defaults to 32 bytes.
     *  @param block_count  The size of the memory in the chip to allow range checkng. Set to
     *                      0 to disable checks. Defaults to 0.
     *  @param bus_speed    The frequency of the I2C bus. Defaults to 400K.
     */
    I2CEepromBlockDevice(PinName sda, PinName scl, uint8_t address,
            bd_size_t block_size, bd_count_t block_count, int bus_speed=400000);

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

    /** Sync an underlying block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual bd_error_t sync();

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
    
private:
    I2C _i2c;
    int _i2c_address;
    size_t _block_size;
    size_t _block_count;
};
 
#endif  /* DEVICE_SPI */

#endif  /* MBED_SD_BLOCK_DEVICE_H */
