/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#ifndef MBED_I2C_SLAVE_H
#define MBED_I2C_SLAVE_H

#include "platform/platform.h"

#if DEVICE_I2CSLAVE || defined(DOXYGEN_ONLY)

#include "hal/i2c_api.h"

namespace mbed {
/**
 * \defgroup drivers_I2CSlave I2CSlave class
 * \ingroup drivers-public-api-i2c
 * @{
 */

/** An I2C Slave, used for communicating with an I2C Master device.
 *
 * @note Synchronization level: Not protected
 *
 * Example Simple I2C slave and master (requires two Mbed-boards):
 * @code
 * #include <mbed.h>
 * #include <mbed_wait_api.h>
 * #include <string.h>
 *
 * #define BUILD_I2C_SLAVE 1 // Build for slave or master of this example
 *
 * #define SLAVE_ADDR 0xA0
 * #define BUFFER_SIZE 6
 *
 * #if BUILD_I2C_SLAVE
 *
 * // Slave side of the example
 *
 * #if !DEVICE_I2CSLAVE
 * #error [NOT_SUPPORTED] I2C Slave is not supported
 * #endif
 *
 * I2CSlave slave(I2C_SDA, I2C_SCL);
 *
 * int main() {
 *
 *     char buf[BUFFER_SIZE] = "ABCDE";
 *
 *     slave.address(SLAVE_ADDR);
 *     while (1) {
 *         int i = slave.receive();
 *         switch (i) {
 *             case I2CSlave::ReadAddressed:
 *                 // Write back the buffer from the master
 *                 slave.write(buf, BUFFER_SIZE);
 *                 printf("Written to master (addressed): %s\n", buf);
 *                 break;
 *
 *             case I2CSlave::WriteGeneral:
 *                 slave.read(buf, BUFFER_SIZE);
 *                 printf("Read from master (general): %s\n", buf);
 *                 break;
 *
 *             case I2CSlave::WriteAddressed:
 *                 slave.read(buf, BUFFER_SIZE);
 *                 printf("Read from master (addressed): %s\n", buf);
 *                 break;
 *         }
 *     }
 * }
 *
 * #else
 *
 * // Master side of the example
 *
 * I2C master(I2C_SDA, I2C_SCL);
 *
 * static const char* to_send[] = { "abcde", "12345", "EFGHI" };
 *
 * int main() {
 *     char buf[BUFFER_SIZE];
 *     int send_index = 0;
 *
 *     while (1) {
 *         strcpy(buf, to_send[send_index]);
 *
 *         // Write the new message to the slave
 *         if (master.write(SLAVE_ADDR, buf, BUFFER_SIZE)) {
 *             printf("Failed to write to slave!\n");
 *         } else {
 *             printf("Written to slave: %s\n", buf);
 *         }
 *
 *         // Read what the slave has (should be identical)
 *         if (master.read(SLAVE_ADDR, buf, BUFFER_SIZE)) {
 *             printf("Failed to read from slave!\n");
 *         } else {
 *             printf("Read from slave: %s\n", buf);
 *         }
 *
 *         // Change the message we're writing to the slave
 *         send_index++;
 *         if (send_index > 2) {
 *             send_index = 0;
 *         }
 *
 *         wait_us(500000); // Wait 0.5s
 *     }
 * }
 *
 * #endif
 *
 * @endcode
 */
class I2CSlave {

public:
    enum RxStatus {
        NoData         = 0,
        ReadAddressed  = 1,
        WriteGeneral   = 2,
        WriteAddressed = 3
    };

    /** Create an I2C Slave interface, connected to the specified pins.
     *
     *  @param sda I2C data line pin.
     *  @param scl I2C clock line pin.
     */
    I2CSlave(PinName sda, PinName scl);

    /** Create an I2C Slave interface, connected to the specified pins.
     *
     *  @param static_pinmap reference to structure which holds static pinmap.
     */
    I2CSlave(const i2c_pinmap_t &static_pinmap);
    I2CSlave(const i2c_pinmap_t &&) = delete; // prevent passing of temporary objects

    /** Set the frequency of the I2C interface.
     *
     *  @param hz The bus frequency in Hertz.
     */
    void frequency(int hz);

    /** Check if this I2C Slave has been addressed.
     *
     *  @return A status indicating if the device has been addressed and how.
     *  @retval NoData          The slave has not been addressed.
     *  @retval ReadAddressed   The master has requested a read from this slave.
     *  @retval WriteAddressed  The master is writing to this slave.
     *  @retval WriteGeneral    The master is writing to all slave.
     */
    int receive(void);

    /** Read specified number of bytes from an I2C master.
     *
     *  @param data   Pointer to the buffer to read data into.
     *  @param length Number of bytes to read.
     *
     *  @return Result of the operation.
     *  @retval 0       If the number of bytes read is equal to length requested.
     *  @retval nonzero On error or if the number of bytes read is less than requested.
     */
    int read(char *data, int length);

    /** Read a single byte from an I2C master.
     *
     *  @return The byte read.
     */
    int read(void);

    /** Write to an I2C master.
     *
     *  @param data   Pointer to the buffer containing the data to be sent.
     *  @param length Number of bytes to send.
     *
     *  @return
     *  @retval 0       If written all bytes successfully.
     *  @retval nonzero On error or if the number of bytes written is less than requested.
     */
    int write(const char *data, int length);

    /** Write a single byte to an I2C master.
     *
     *  @param data Value to write.
     *
     *  @return Result of the operation.
     *  @retval 0 If a NACK is received.
     *  @retval 1 If an ACK is received.
     *  @retval 2 On timeout.
     */
    int write(int data);

    /** Set the I2C slave address.
     *
     *  @param address The address to set for the slave (least significant bit is ignored).
     *
     *  @note If address is set to 0, the slave will only respond to the
     *  general call address.
     */
    void address(int address);

    /** Reset the I2C slave back into the known ready receiving state.
     */
    void stop(void);

#if !defined(DOXYGEN_ONLY)

protected:
    /* Internal i2c object identifying the resources */
    i2c_t _i2c;

#endif //!defined(DOXYGEN_ONLY)
};

/** @}*/

} // namespace mbed

#endif

#endif
