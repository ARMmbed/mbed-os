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
#ifndef MBED_I2C_H
#define MBED_I2C_H

#include "platform/platform.h"
#include "hal/gpio_api.h"

#if DEVICE_I2C || defined(DOXYGEN_ONLY)

#include "hal/i2c_api.h"
#include "platform/SingletonPtr.h"
#include "rtos/Mutex.h"
#include "platform/NonCopyable.h"

#if DEVICE_I2C_ASYNCH
#include "platform/CThunk.h"
#include "hal/dma_api.h"
#include "platform/Callback.h"
#endif

namespace mbed {
/** \defgroup drivers-public-api-i2c I2C
 * \ingroup drivers-public-api
 */

/**
 * \defgroup drivers_I2C I2C class
 * \ingroup drivers-public-api-i2c
 * @{
 */

// Note: In the below comments, Doxygen wants to auto-link the word "I2C" to the class name.  A percent sign is used to
// suppress this behavior.

/** An %I2C Master, used for communicating with %I2C slave devices
 *
 * There are three different forms of the %I2C API usable via this class:
 * <ul>
 * <li>Transaction-based %I2C</li>
 * <li>Single-byte %I2C</li>
 * <li>Asynchronous %I2C</li>
 * </ul>
 *
 * All three of these APIs let you execute %I2C operations, but they work differently.
 *
 * <h1>Transaction-Based API</h1>
 *
 * The simplest API, which should be appropriate for most use cases, is the transaction-based API, which is
 * accessed through the \link I2C::read(int address, char *data, int length, bool repeated) read() \endlink and the
 * \link write(int address, const char *data, int length, bool repeated) write() \endlink functions.  These functions
 * execute an entire %I2C transaction (the start condition, address, data bytes, and stop condition) in a single
 * function call.
 *
 * The bytes to be read/written are passed in through an array, which requires that you can predict the
 * size of the data ahead of time.  If this information is not known, you may want to use the single-byte API instead
 * (see below).
 *
 * Example of using the transaction-based API to read the temperature from an LM75BD:
 * @code
 * #include "mbed.h"
 * I2C i2c(I2C_SDA , I2C_SCL);
 * const int addr7bit = 0x48;      // 7-bit I2C address
 * const int addr8bit = 0x48 << 1; // 8-bit I2C address, 0x90
 *
 * int main() {
 *     char cmd[2];
 *     while (1) {
 *         cmd[0] = 0x01;
 *         cmd[1] = 0x00;
 *
 *         // read and write takes the 8-bit version of the address.
 *         // set up configuration register (at 0x01)
 *         I2C::Result result = i2c.write(addr8bit, cmd, 2);
 *
 *         if(result != I2C::ACK)
 *         {
 *             // Chip not accessible, handle error....
 *         }
 *
 *         ThisThread::sleep_for(500);
 *
 *         // read temperature register
 *         cmd[0] = 0x00;
 *         i2c.write(addr8bit, cmd, 1, true); // Set repeated to true so that we don't give up the bus after this transaction
 *         i2c.read(addr8bit | 1, cmd, 2);
 *
 *         float tmp = (float((cmd[0]<<8)|cmd[1]) / 256.0);
 *         printf("Temp = %.2f\n", tmp);
 *   }
 * }
 * @endcode
 *
 *
 *
 * <h1>Single-Byte API</h1>
 *
 * The single-byte API consists of the \link I2C::start() start() \endlink, \link I2C::write_byte() write_byte()
 * \endlink, \link I2C::read_byte() read_byte() \endlink, and \link I2C::stop() stop() \endlink functions.
 * With the single-byte API, you have manual control over each condition and data byte put onto the I2C bus.
 * This is useful for dealing with devices which can return variable amounts of data in one I2C operation,
 * or when you don't want to create buffers to store the data.  However, this API is more verbose than the
 * transaction-based API and will have a bit more overhead since there's more code executing per byte.
 *
 * The following is an example that accomplishes the same thing as the above code, but using the single-byte API.
 * @code
 * #include "mbed.h"
 * I2C i2c(I2C_SDA , I2C_SCL);
 * const int addr7bit = 0x48;      // 7-bit I2C address
 * const int addr8bit = 0x48 << 1; // 8-bit I2C address, 0x90
 *
 * int main() {
 *     while (1) {
 *         // read and write takes the 8-bit version of the address.
 *         // set up configuration register (at 0x01)
 *         i2c.lock();
 *         i2c.start();
 *         I2C::Result result = i2c.write_byte(addr8bit); // Write address, LSBit low to indicate write
 *         i2c.write_byte(0x01);
 *         i2c.write_byte(0x00);
 *         i2c.stop();
 *         i2c.unlock();
 *
 *         if(result != I2C::ACK)
 *         {
 *             // Chip not accessible, handle error....
 *         }
 *
 *         ThisThread::sleep_for(500);
 *
 *         // Set register to read
 *         i2c.lock();
 *         i2c.start();
 *         i2c.write_byte(addr8bit); // Write address
 *         i2c.write_byte(0x00);
 *         // To create a repeated start condition, we do not call stop() here
 *
 *         i2c.start();
 *         i2c.write_byte(addr8bit | 1); // Write address, LSBit high to indicate read
 *
 *         // Read the two byte temperature word
 *         uint16_t temperatureBinary = 0;
 *         temperatureBinary |= static_cast<uint16_t>(i2c.read_byte(true)) << 8;
 *         temperatureBinary |= static_cast<uint16_t>(i2c.read_byte(false)); // send NACK to indicate last byte
 *         i2c.stop();
 *         i2c.unlock();
 *
 *         float tmp = (float(temperatureBinary) / 256.0);
 *         printf("Temp = %.2f\n", tmp);
 *   }
 * }
 * @endcode
 *
 * \attention If a single I2C object is being shared among multiple threads, you should surround usage of the
 * single-byte API with \link I2C::lock() lock() \endlink and \link I2C::unlock() unlock() \endlink.  This
 * ensures that a transaction by one thread is not interrupted by another.  It may also improve performance
 * because the backing mutex will not need to be locked for each byte.
 *
 * <h1>Asynchronous API</h1>
 *
 * The asynchronous API allows you to run %I2C operations in the background.  This API is only
 * available if your device has the I2C_ASYNCH feature.  To use this API, use \link I2C::transfer() transfer() \endlink
 * to start an operation and \link I2C::abort_transfer() abort_transfer() \endlink to stop it.  Alternately, use the
 * \link I2C::transfer_and_wait() transfer_and_wait() \endlink function to block the current thread until
 * the transfer finishes.
 *
 * Some devices implement these features using DMA, others use interrupts, so be mindful that there may still be
 * significant CPU usage if you have multiple and/or high-rate transfers going on.
 *
 * <h1>A Note about Addressing</h1>
 * Most %I2C devices make use of 7-bit addresses (see <a href="https://www.i2c-bus.org/addressing/">here</a> for details).
 * Mbed OS, however, works with addresses in 8-bit format, where the least significant bit specifies if the transaction
 * is a read (1) or a write (0).  Due to this, you will generally need to use bitshifts and bitwise ORs when passing
 * addresses to I2C functions.  See the documentation on each function for details.
 *
 * %I2C also has a <a href="https://www.i2c-bus.org/addressing/10-bit-addressing/">10-bit addressing mode</a>, where
 * the address is sent in two physical bytes on the bus.  Some, but not all, Mbed targets support this mode -- refer
 * to your MCU datasheet and your target's HAL code for details.  For 10-bit addresses, use the same format to
 * pass them to I2C functions -- shift them left by one and set the LSBit to indicate the read/write direction.
 * On MCUs that do not natively support 10-bit addressing, you can emulate support by using the single-byte API
 * to send two address bytes; see the linked page above for details.
 *
 * <h1>Other Info</h1>
 *
 * The I2C class is thread-safe, and uses a mutex to prevent multiple threads from using it at the same time.
 *
 * \warning Mbed OS requires that you only create one instance of the I2C class per physical %I2C bus on your chip.
 * This means that if you have multiple sensors connected together on a bus, you must create one I2C object at the
 * top level and pass it in to the drivers for each sensor.  Violating this directive will cause undefined
 * behavior in your code.
 *
 * \attention Due to how %I2C works, if multiple devices are sharing a bus which support different %I2C speeds, you cannot
 * go faster than the maximum bus speed of any of the devices.  Otherwise, slower devices may misinterpret messages
 * that are too fast for them and cause interference on the bus.  For example, if you have two 400kHz devices and one
 * 100kHz device on a bus, you must run the entire bus at 100kHz!
 */
class I2C : private NonCopyable<I2C> {

public:

    /**
     * Result code for I2C operations
     */
    enum Result : int {
        /// ACK was received
        ACK = 0,
        /// NACK was received
        NACK,
        /// Timeout waiting for I2C hardware
        TIMEOUT,
        /// Other error in I2C operation
        OTHER_ERROR
    };


    /** Create an I2C Master interface, connected to the specified pins.
     * The new object defaults to 100kHz speed.
     *
     *  @param sda I2C data line pin
     *  @param scl I2C clock line pin
     */
    I2C(PinName sda, PinName scl);

    /** Create an I2C Master interface, connected to the specified pins.
     * The new object defaults to 100kHz speed.
     *
     *  @param static_pinmap reference to structure which holds static pinmap.
     */
    I2C(const i2c_pinmap_t &static_pinmap);
    I2C(const i2c_pinmap_t &&) = delete; // prevent passing of temporary objects

    /** Set the frequency of the I2C interface.
     * If you do not call this function, the I2C will run at 100kHz speed.
     *
     * Note: Some underlying HALs only support a very limited set of common I2C frequencies, such as 100kHz and
     * 400kHz.  Other implementations support all frequencies.  If the frequency you set is not supported, you will get
     * an assertion failure after calling this function.
     *
     *  @param hz The bus frequency in hertz
     */
    void frequency(int hz);

    /** Read from an %I2C slave
     *
     * Performs a complete read transaction. The least significant bit of
     * the address must be 1 to indicate a read.
     *
     *  @param address 8/11-bit I2C slave address [ (7 or 10 bit addr << 1) | 1 ]
     *  @param data Pointer to the byte-array to read data in to
     *  @param length Number of bytes to read
     *  @param repeated Set up for a repeated start.  If true, the Mbed processor does not relinquish the bus after
     *       this read operation.  You may then call write(), read(), or start() again to start another operation.
     *
     *  @returns Result enum describing whether the I2C transaction succeeded or failed
     */
    Result read(int address, char *data, int length, bool repeated = false);

    /** Write to an %I2C slave
     *
     * Performs a complete write transaction. The least significant bit of
     * the address must be 0 to indicate a write.
     *
     *  @param address 8/11-bit I2C slave address [ (7 or 10 bit addr << 1) | 0 ]
     *  @param data Pointer to the byte-array data to send
     *  @param length Number of bytes to send
     *  @param repeated Set up for a repeated start.  If true, the Mbed processor does not relinquish the bus after
     *       this write operation.  You may then call write(), read(), or start() again to start another operation.
     *
     *  @returns Result enum describing whether the I2C transaction succeeded or failed
     */
    Result write(int address, const char *data, int length, bool repeated = false);

    /** Creates a start condition on the %I2C bus.  After calling this function, you should call
     * \link write_byte() \endlink to send the %I2C address.
     */
    void start(void);

    /** Read a single byte from the %I2C bus.
     *
     * After calling this function, you may call it again to read another byte from the slave.  Alternately,
     * you may call \link stop() \endlink to stop the current transaction, or \link start() \endlink to
     * start a new transaction.
     *
     * Note: Reads are not acknowledged by the slave device in I2C, which is why this function does not
     * return an ACK/NACK result.
     *
     *  @param ack indicates if the byte is to be acknowledged (true = acknowledge).  Use false to indicate to
     *  the slave that you don't want to read any more data.
     *
     *  @returns
     *    the byte read, or -1 on error.
     */
    int read_byte(bool ack);

    /** Read a single byte from the %I2C bus.  This function is a legacy alias for \link read_byte() \endlink
     *
     * After calling this function, you may call it again to read another byte from the slave.  Alternately,
     * you may call \link stop() \endlink to stop the current transaction, or \link start() \endlink to
     * start a new transaction.
     *
     * Note: Reads are not acknowledged by the slave device in I2C, which is why this function does not
     * return an ACK/NACK result.
     *
     *  @param ack indicates if the byte is to be acknowledged (1 = acknowledge)
     *
     *  @returns
     *    the byte read
     */
    int read(int ack)
    {
        return read_byte(ack);
    }

    /** Write a single byte out on the %I2C bus.  The very first write_byte() call after calling start()
     * is used to set up the slave address.
     *
     * After calling this function, you may call \link write_byte() \endlink again to write bytes in a write operation,
     * or \link read_byte() \endlink to read bytes in a read operation.  Once done, call \link stop() \endlink
     * to stop the current transaction or \link start() \endlink to start a new transaction.
     *
     *  @param data data to write out on bus.  Note: This is an int, not a uint8_t, to support addressing modes
     *      with more than 7 bits.
     *
     *  @returns Result enum describing whether the I2C byte was acknowledged or not
     */
    Result write_byte(int data);

    /** Write a single byte out on the %I2C bus.  Deprecated version of \link write_byte() \endlink, with a legacy
     * return code format.
     *
     *  @param data data to write out on bus
     *
     *  @returns
     *    '0' - NAK was received
     *    '1' - ACK was received,
     *    '2' - timeout
     */
    MBED_DEPRECATED_SINCE("mbed-ce", "Use I2C::write_byte() instead for better readability and return codes")
    int write(int data);

    /**
     * Creates a stop condition on the %I2C bus. This puts the bus back into an idle state where new transactions can be
     * initiated by this device or others.
     */
    void stop(void);

    /** Acquire exclusive access to this %I2C bus
     */
    virtual void lock(void);

    /** Release exclusive access to this %I2C bus
     */
    virtual void unlock(void);

    virtual ~I2C()
    {
        // Do nothing
    }

#if DEVICE_I2C_ASYNCH

    /** Start nonblocking %I2C transfer.
     *
     * The %I2C peripheral will begin a transmit and/or receive operation in the background.  If only a transmit
     * or receive buffer is specified, only a transmit or receive will be done.  If both buffers are specified,
     * first the transmission is done to the given slave address, then the specified number of bytes are received.
     *
     * If you wish to find out when the transfer is done, pass a callback function to the callback argument
     * and set the event argument to the events you wish to receive.
     * This callback will be called when the transfer completes or errors out.  Be careful: if you
     * only request the I2C_EVENT_TRANSFER_COMPLETE event, and the transfer errors, the callback will never be called.
     *
     * Internally, the chip vendor may implement this function using either DMA or interrupts.
     *
     * This function locks the deep sleep until any event has occurred.
     *
     * You may not call any other functions on this class instance until the transfer is complete, has errored,
     * or is aborted.  Trying to start multiple transfers at once will return an error.
     *
     * @param address   8/11 bit %I2C slave address
     * @param tx_buffer The TX buffer with data to be transferred.  May be nullptr if tx_length is 0.
     * @param tx_length The length of TX buffer in bytes.  If 0, no transmission is done.
     * @param rx_buffer The RX buffer, which is used for received data.  May be nullptr if tx_length is 0.
     * @param rx_length The length of RX buffer in bytes  If 0, no reception is done.
     * @param event     The logical OR of events to subscribe to.  May be I2C_EVENT_ALL, or some combination
     *        of the flags I2C_EVENT_ERROR, I2C_EVENT_ERROR_NO_SLAVE, I2C_EVENT_TRANSFER_COMPLETE, or I2C_EVENT_TRANSFER_EARLY_NACK
     * @param callback  The event callback function
     * @param repeated Set up for a repeated start.  If true, the Mbed processor does not relinquish the bus after
     *       this write operation.  You may then call write(), read(), or start() again to start another operation.
     *
     * @returns Zero if the transfer has started, or -1 if I2C peripheral is busy
     */
    int transfer(int address, const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length, const event_callback_t &callback, int event = I2C_EVENT_TRANSFER_COMPLETE, bool repeated = false);

    /** Abort the ongoing I2C transfer
     */
    void abort_transfer();

    /** Start %I2C transfer and wait until it is complete.  Like the transactional API this blocks the current thread,
     * however all work is done in the background and other threads may execute.
     *
     * The %I2C peripheral will begin a transmit and/or receive operation in the background.  If only a transmit
     * or receive buffer is specified, only a transmit or receive will be done.  If both buffers are specified,
     * first the transmission is done to the given slave address, then the specified number of bytes are received.
     *
     * Internally, the chip vendor may implement this function using either DMA or interrupts.
     *
     * This function locks the deep sleep until it returns.
     *
     * @param address   8/11 bit %I2C slave address
     * @param tx_buffer The TX buffer with data to be transferred.  May be nullptr if tx_length is 0.
     * @param tx_length The length of TX buffer in bytes.  If 0, no transmission is done.
     * @param rx_buffer The RX buffer, which is used for received data.  May be nullptr if tx_length is 0.
     * @param rx_length The length of RX buffer in bytes  If 0, no reception is done.
     * @param timeout timeout value.  Use #rtos::Kernel::wait_for_u32_forever to wait forever (the default).
     * @param repeated Set up for a repeated start.  If true, the Mbed processor does not relinquish the bus after
     *       this operation.  You may then call write(), read(), or start() again to start another operation.
     *
     * @returns Result code describing whether the transfer succeeded or not.
     */
    Result transfer_and_wait(int address, const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length, rtos::Kernel::Clock::duration_u32 timeout = rtos::Kernel::wait_for_u32_forever, bool repeated = false);

#if !defined(DOXYGEN_ONLY)
protected:
    /** Lock deep sleep only if it is not yet locked */
    void lock_deep_sleep();

    /** Unlock deep sleep only if it has been locked */
    void unlock_deep_sleep();

    void irq_handler_asynch(void);
    event_callback_t _callback;
    CThunk<I2C> _irq;
    DMAUsage _usage;
    bool _deep_sleep_locked;
#endif
#endif

#if !defined(DOXYGEN_ONLY)
protected:

    i2c_t _i2c;
    int    _hz;
    SingletonPtr<rtos::Mutex> _mutex;
    PinName _sda;
    PinName _scl;

private:
    /** Recover I2C bus, when stuck with SDA low
     *  @note : Initialization of I2C bus is required after this API.
     *
     *  @param sda I2C data line pin
     *  @param scl I2C clock line pin
     *
     * @returns
     *    '0' - Successfully recovered
     *    'I2C_ERROR_BUS_BUSY' - In case of failure
     *
     */
    int recover(PinName sda, PinName scl);
#endif
};

/** @}*/

} // namespace mbed

#endif

#endif
