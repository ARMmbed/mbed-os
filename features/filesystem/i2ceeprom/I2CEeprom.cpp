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
#include "I2CEeprom.h"
 
I2CEeprom::I2CEeprom(PinName sda, PinName scl, int address, size_t pageSize, size_t chipSize, int busSpeed):
    m_i2c(sda, scl),
    m_i2cAddress(address),
    m_chipSize(chipSize),
    m_pageSize(pageSize)
{
    m_i2c.frequency(busSpeed);
}
 
size_t I2CEeprom::read(size_t address, char &value) {
    // Check the address and size fit onto the chip.
    if (!checkSpace(address, 1))
        return 0;
        
    char values[] = { (address >> 8), (address & 0xFF) };
    if (m_i2c.write(m_i2cAddress, values, 2) == 0) {
        if (m_i2c.read(m_i2cAddress, &value, 1) == 0) {
            return 1;
        }
    }
    
    return 0;
}
 
size_t I2CEeprom::read(size_t address, char *buffer, size_t size) {
    // Check the address and size fit onto the chip.
    if (!checkSpace(address, size))
        return 0;
        
    char values[] = { (address >> 8), (address & 0xFF) };
    if (m_i2c.write(m_i2cAddress, values, 2) == 0) {
        if (m_i2c.read(m_i2cAddress, buffer, size) == 0) {
            return size;
        }
    }
    
    return 0;
}
 
size_t I2CEeprom::write(size_t address, char value) {
    // Check the address and size fit onto the chip.
    if (!checkSpace(address, 1))
        return 0;
        
    char values[] = { (address >> 8), (address & 0xFF), value };
    if (m_i2c.write(m_i2cAddress, values, 3) != 0) {
        return 0;
    }
    
    waitForWrite();
    
    return 1;
}
 
size_t I2CEeprom::write(size_t address, const char *buffer, size_t size) {
    // Check the address and size fit onto the chip.
    if (!checkSpace(address, size))
        return 0;
        
    const char *page = buffer;
    size_t left = size;
    
    // Whle we have some more data to write.
    while (left != 0) {
        // Calculate the number of bytes we can write in the current page.
        // If the address is not page aligned then write enough to page 
        // align it.
        size_t toWrite;
        if ((address % m_pageSize) != 0) {
            toWrite = (((address / m_pageSize) + 1) * m_pageSize) - address;
            if (toWrite > size) {
                toWrite = size;
            }
        } else {
            if (left <= m_pageSize) {
                toWrite = left;
            } else {
                toWrite = m_pageSize;
            }
        }
        
        //printf("Writing [%.*s] at %d size %d\n\r", toWrite, page, address, toWrite);
        // Start the page write with the addres ine one write call.
        char values[] = { (address >> 8), (address & 0xFF) };
        if (m_i2c.write(m_i2cAddress, values, 2, true) != 0) {
            // Write failed to return bytes written so far.
            return size - left;
        }
 
        // Write the bytes out one at a time to avoid having to copy them to
        // another buffer.        
        for (int count = 0; count != toWrite; ++count) {
            if (m_i2c.write(*page) == 0) {
            // Write failed to return bytes written so far.
                return size - left;
            }
            ++page;
        }
        
        // Stop the transaction now we've completed the page
        // write.
        m_i2c.stop();
        
        waitForWrite();
        
        // Update the counters with the amount we've just written
        left -= toWrite;
        address += toWrite;
    }
    
    return size;
}
 
size_t I2CEeprom::fill(size_t address, char value, size_t size) {
    // Check the address and size fit onto the chip.
    if (!checkSpace(address, size))
        return 0;
        
    size_t left = size;
    
    while (left != 0) {
        size_t toWrite;
        if ((address % m_pageSize) != 0) {
            toWrite = (((address / m_pageSize) + 1) * 64) - address;
            if (toWrite > size) {
                toWrite = size;
            }
        } else {
            if (left <= m_pageSize) {
                toWrite = left;
            } else {
                toWrite = m_pageSize;
            }
        }
        
        //printf("Writing %d at %d size %d\n\r", value, address, toWrite);
        char values[] = { (address >> 8), (address & 0xFF) };
        if (m_i2c.write(m_i2cAddress, values, 2, true) != 0) {
            return size - left;
        }
        
        for (int count = 0; count != toWrite; ++count) {
            if (m_i2c.write(value) == 0)
                return size - left;
        }
        
        m_i2c.stop();
        
        waitForWrite();
        
        left -= toWrite;
        address += toWrite;
    }
    
    return size;
}
 
void I2CEeprom::waitForWrite() {
    // The chip doesn't ACK while writing to the actual EEPROM
    // so loop trying to do a zero byte write until it is ACKed
    // by the chip.
    while (m_i2c.write(m_i2cAddress, 0, 0) != 0) {
        // Wait for ack.
        wait_ms(1);
    }
}
 
bool I2CEeprom::checkSpace(size_t address, size_t size) {
    // Only check if chip size is non-zero.
    if (m_chipSize != 0) {
        // Check that the address start in the chip and doesn't
        // extend past.
        if ((address >= m_chipSize) || ((address + size) >= m_chipSize))
            return false;
        else
            return true;
    }
    
    return true;
}
