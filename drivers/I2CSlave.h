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
 * Example Simple I2C responder:
 * @code
 * #include <mbed.h>
 *
 * const int SLAVE_ADDRESS = 0xA0;
 * const char message[] = "Slave!";
 *
 * I2CSlave slave(I2C_SDA, I2C_SCL);
 *
 * int main() {
 *     slave.address(SLAVE_ADDRESS);
 *     while (1) {
 *         int operation = slave.receive();
 *         switch (operation) {
 *             case I2CSlave::ReadAddressed:
 *                 int status = slave.write(message, sizeof(message));
 *                 if (status == 0) {
 *                     printf("Written message: %s\n", message);
 *                 } else {
 *                     printf("Failed to write message.\n");
 *                 }
 *                 break;
 *             case I2CSlave::WriteGeneral:
 *                 int byte_read = slave.read();
 *                 printf("Read General: %c (%d)\n", byte_read, byte_read);
 *                 break;
 *             case I2CSlave::WriteAddressed:
 *                 int byte_read = slave.read();
 *                 printf("Read Addressed: %c (%d)\n", byte_read, byte_read);
 *                 break;
 *         }
 *     }
 * }
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
