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
#ifndef __I2CEEPROM_H__
#define __I2CEEPROM_H__
 
#include <mbed.h>
 
/// Class to provide simple access to I2C EEPROM chiles like Microchip's 24LC range
/// or AMTELS AT24C range.
/// Chips up to 64Kb in size are directly supported.
/// The class handles multiple page writes so any amount of data can be written in
/// a single call to write. The writes are not buffered so additional memory use 
/// is kept to a minimum.
///
/// Although not tested it should work with the MAC versions of Microchip range
/// as well but the chipSize will need to be set to include the ROM area as well.
class I2CEeprom {
public:
    /// Constructor to create a new instance of the class.
    /// @param sda The pin name for the sda line of the I2C bus.
    /// @param scl The pin name for the scl line of the I2C bus.
    /// @param address The 8bit I2C address of the chip in the range 0xA0 - 0xAE.
    /// @param pageSize The size of the page used in writing to the chip.
    /// @param chipSize The size of the memory in the chip to allow range checkng. Set to
    /// 0 to disable checks.
    /// @param busSpeed The frequency of the I2C bus defaults to 400K.
    I2CEeprom(PinName sda, PinName scl, int address, size_t pageSize, size_t chipSize, int busSpeed = 400000);
    
    /// Read a single byte from the address in memory.
    /// @param address Memory address to read from.
    /// @param value Variable to receive value read.
    /// @returns Number of bytes read from memory.
    size_t read(size_t address, char &value);
    
    /// Read multiple bytes starting from the address in memory.
    /// @param address Memory address to start reading from.
    /// @param buffer Pointer to buffer to hold bytes read from memory.
    /// @param size Number of bytes to be read from memory.
    /// @returns Number of bytes read from memory.
    size_t read(size_t address, char *buffer, size_t size);
    
    /// Read either an instance or an array of a POD type from memory.
    /// Note the value of the type can't contain pointers.
    /// @param address Start address for reading memory.
    /// @param value Object to be read from memory.
    /// @returns Number of bytes read from memory.
    template<typename T> size_t read(size_t address, T &value) {
        return read(address, reinterpret_cast<char *>(&value), sizeof(T));
    }
    
    /// Write a single byte to the address in memory.
    /// @param address Memory address to write to.
    /// @param value Value to be written to memory.
    /// @returns Number of bytes written to memory.
    size_t write(size_t address, char value);
 
    /// Write multiple bytes starting from the address in memory.
    /// @param address Memory address to start writting to.
    /// @param buffer Pointer to buffer holding the bytes to write to memory.
    /// @param size Number of bytes to be written to memory.
    /// @returns Number of bytes written to memory.
    size_t write(size_t address, const char *buffer, size_t size);
    
    /// Write either an instance or an array of a POD type to memory.
    /// Note the value of the type can't contain pointers.
    /// @param address Start address to write to memory.
    /// @returns Number of bytes written to memory.
    template<typename T> size_t write(size_t address, const T &value) {
        return write(address, reinterpret_cast<const char *>(&value), sizeof(T));
    }
    
    /// Fill a range of memory with a single value. No memory is allocated
    /// so large areas can be filled with minimal memory usage.
    /// @param address Starting address to write to.
    /// @param value Value to be written to memory.
    /// @Param size Number of bytes to be written.
    /// @returns Number of bytes written to memory.
    size_t fill(size_t address, char value, size_t size);
    
private:
    // Wait for a write cycle to complete using polling and small waits.
    void waitForWrite();
    
    // Validate that the proposed opperation will fit in the size of
    // the chip.
    bool checkSpace(size_t address, size_t size);
    
private:
    I2C m_i2c;
    int m_i2cAddress;
    size_t m_chipSize;
    size_t m_pageSize;
};
 
#endif
