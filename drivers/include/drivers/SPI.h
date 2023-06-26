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
#ifndef MBED_SPI_H
#define MBED_SPI_H

#include "platform/platform.h"

#if DEVICE_SPI || defined(DOXYGEN_ONLY)

#include "rtos/Mutex.h"
#include "hal/spi_api.h"
#include "drivers/DigitalOut.h"
#include "platform/SingletonPtr.h"
#include "platform/NonCopyable.h"

#if defined MBED_CONF_DRIVERS_SPI_COUNT_MAX && DEVICE_SPI_COUNT > MBED_CONF_DRIVERS_SPI_COUNT_MAX
#define SPI_PERIPHERALS_USED MBED_CONF_DRIVERS_SPI_COUNT_MAX
#elif defined DEVICE_SPI_COUNT
#define SPI_PERIPHERALS_USED DEVICE_SPI_COUNT
#else
/* Backwards compatibility with HALs not providing DEVICE_SPI_COUNT */
#define SPI_PERIPHERALS_USED 1
#endif

#if DEVICE_SPI_ASYNCH
#include "platform/CThunk.h"
#include "hal/dma_api.h"
#include "platform/CircularBuffer.h"
#include "platform/Callback.h"
#include "platform/Transaction.h"
#include "rtos/EventFlags.h"
#endif

namespace mbed {
/**
 * \defgroup drivers_SPI SPI class
 * \ingroup drivers-public-api-spi
 * @{
 */

struct use_gpio_ssel_t { };
const use_gpio_ssel_t use_gpio_ssel;

/**
 * @brief An %SPI Master, used for communicating with %SPI slave devices.
 *
 * The default format is set to 8-bits, mode 0, and a clock frequency of 1MHz.
 *
 * @note Synchronization level: Thread safe
 *
 * %SPI allows you to transfer data to and from peripheral devices using single-word transfers, transactions,
 * or an asynchronous API.
 *
 * Here's how to talk to a chip using the single-word API:
 * @code{.cpp}
 * #include "mbed.h"
 *
 * SPI device(SPI_MOSI, SPI_MISO, SPI_SCLK, SPI_CS, use_gpio_ssel)
 *
 * int main() {
 *     device.format(8, 0);
 *
 *     device.lock();
 *     device.select();
 *     int response = device.write(0x0A);
 *     int response2 = device.write(0x0B);
 *     device.deselect();
 *     device.unlock();
 * }
 * @endcode
 *
 * And here's how to do the same thing using a transaction:
 * @code
 * #include "mbed.h"
 *
 * SPI device(SPI_MOSI, SPI_MISO, SPI_SCLK, SPI_CS, use_gpio_ssel)
 *
 * int main() {
 *     device.format(8, 0);
 *
 *     uint8_t command[2] = {0x0A, 0x0B};
 *     uint8_t response[2];
 *     int result = device.write(command, sizeof(command), response, sizeof(response));
 * }
 * @endcode
 *
 * <h1>Hardware vs Software Chip Selects</h1>
 * <p>Most ARM chips have specific pins marked as "hardware chip selects".  This means that they are connected
 * to the %SPI peripheral and are automatically brought low by hardware when data is sent.  However, chips
 * often only have only one pin for each SPI bus that can be used as a hardware chip select.  If you wish to
 * use multiple peripheral devices with one SPI bus, or just don't have access to the HW CS pin, you must use the
 * %SPI object in "GPIO chip select" mode.</p>
 *
 * <p>To use GPIO CS mode, simply pass the CS pin as the 4th constructor parameter, then pass the
 * special constant \c use_gpio_ssel as the 5th parameter.  This puts the object in GPIO mode, where
 * it will operate the CS line as a regular GPIO pin before and after doing an SPI transfer.  This mode
 * is marginally slower than HW CS mode, but otherwise should offer the same functionality.  In fact,
 * since the pin mapping is more flexible, it may be advisable to use GPIO CS mode by default.</p>
 *
 * \warning It is not recommended to control the CS line using a separate DigitalOut instance!
 * This was needed in very old Mbed versions but should now be considered a legacy practice.
 * Not only does it make it difficult to use the asynchronous API, but it can also lead to corner
 * cases which corrupt data.  The CS line, whether done through GPIO or HW, should always be managed through
 * the SPI class.
 *
 * <h1>Sharing a Bus</h1>
 * <p>Multiple %SPI devices may share the same physical bus, so long as each has its own dedicated chip select
 * (CS) pin.  To implement this sharing, each chip should create its own instance of the SPI class, passing
 * the same MOSI, MISO, and SCLK pins but a different CS pin.  Mbed OS will internally share the %SPI hardware
 * between these objects.  Note that this is <i>completely different</i> from how the I2C class handles
 * sharing.</p>
 *
 * <h1>Frame/Word Size</h1>
 * <p>Mbed OS supports configuration of the %SPI frame size, also known as the word size, which controls how
 * many bits are sent in one transfer operation (one call to SPI::write()).  This parameter should match
 * the "register size" of the %SPI peripheral that you are talking to.  For example, if you're working with a chip
 * which uses 16-bit registers, you should set the frame size to 16 using SPI::format().  Some Mbed devices also
 * support 32-bit frames -- use the \c DEVICE_SPI_32BIT_WORDS feature macro to test if yours does.</p>
 *
 * <p>The frame size controls the effective width of data written and read from the chip.  For example, if you set
 * frame size to 8, SPI::write(int) will take one byte and return one byte, but if you set it to 16, SPI::write(int)
 * will take a 16 bit value and return a 16 bit value. You can also do complete transactions
 * (e.g. SPI::write(const char *, int, char *, int)) with frame sizes other than 8.  Just be sure to pass the
 * length in bytes, not words!</p>
 *
 * <p>It should be noted that changing the frame size can perform an apparent "endian swap" on data being
 * transmitted.  For example, suppose you have the 32-bit integer 0x01020408.  On a little-endian processor,
 * this will be encoded with the LSByte <i>first</i> in memory: <tt>08 04 02 01</tt>.  If you send that
 * integer using one-byte word size, it will appear as such.  Consider the following example:</p>
 * @code{.cpp}
 * spi.format(8, 0);
 * uint32_t myInteger = 0x01020408;
 * spi.write(reinterpret_cast<const char *>(&myInteger), sizeof(myInteger), nullptr, 0);
 * @endcode
 *
 * <p>If you ran this code, then used a logic analyzer to view the bytes on the MOSI line, it would show bytes
 * matching the layout of the integer in memory:</p>
 * <pre>
 * MOSI -> 08 04 02 01
 * </pre>
 *
 * <p>But what about if you used a 32-bit frame size?</p>
 * @code{.cpp}
 * spi.format(32, 0);
 * uint32_t myInteger = 0x01020408;
 * spi.write<uint32_t>(&myInteger, sizeof(myInteger), nullptr, 0);
 * @endcode
 *
 * <p>In this case, the complete 32-bit integer is sent as a single unit, from its MSBit to its LSBit, without
 * breaking it into bytes.  This will send the data in an order different from the order in memory.  Viewed as
 * bytes, it would look like:</p>
 *
 * <pre>
 * MOSI -> 01 02 04 08
 * </pre>
 *
 * <p>But viewed by a peripheral chip which uses 32-bit SPI words, it would look like:</p>
 *
 * <pre>
 * MOSI -> 0x01020408
 * </pre>
 *
 * <p>When viewed as bytes, it's almost as if you did an endian swap on the data before sending it,
 * but in fact it's standard %SPI behavior when using frame sizes greater than one byte.  This same rule
 * applies to receiving data, so be sure to check examples in the datasheet to determine what frame
 * size to use and whether byte swapping is needed when working with an external chip.</p>
 *
 * <p>Note: Some Mbed targets support frame sizes that are not standard integer sizes, e.g. 4 bits, 7 bits, or
 * 24 bits.  However, the behavior of these frame sizes is not well defined and may differ across targets
 * or across API calls (e.g. single-byte vs transaction vs async).  More work is needed to make these consistent.</p>
 *
 * <h1>Asynchronous API</h1>
 * <p>On many processors, Mbed OS also supports asynchronous %SPI.  This feature allows you to run %SPI
 * transfers completely in the background, while other threads execute in the foreground.  This can
 * be extremely useful if you need to send large amounts of data over the %SPI bus but don't want to
 * block your main thread for ages.  To see if your processor supports async %SPI, look for the
 * DEVICE_SPI_ASYNCH macro.</p>
 *
 * <p>The asynchronous API has two different modes: nonblocking (where your thread can keep running, and
 * the transfer calls a callback when finished) and blocking (where your thread blocks for the duration of
 * the transfer but others can execute).  Here's a sample of how to send the same data as above
 * using the blocking async API:</p>
 *
 * @code
 * #include "mbed.h"
 *
 * SPI device(SPI_MOSI, SPI_MISO, SPI_SCLK, SPI_CS, use_gpio_ssel)
 *
 * int main() {
 *     device.format(8, 0);
 *
 *     uint8_t command[2] = {0x0A, 0x0B};
 *     uint8_t response[2];
 *     int result = device.transfer_and_wait(command, sizeof(command), response, sizeof(response));
 * }
 * @endcode
 *
 * <p>This code will cause the data in \c command to be sent to the device and the response to be received into
 * \c response .  During the transfer, the current thread is paused, but other threads can execute.
 * The non-blocking API does not pause the current thread, but is a bit more complicated to use.
 * See the SPI::transfer_and_wait() implementation in the header file for an example.</p>
 *
 * <h3>Async: DMA vs Interrupts</h3>
 * <p>Some processors only provide asynchronous %SPI via interrupts, some only support DMA, and some offer both.
 * Using interrupts is simpler and generally doesn't require additional resources from the chip, however,
 * some CPU time will be spent servicing the interrupt while the transfer is running.  This can become very
 * performance-intensive -- on some chips, running async %SPI at frequencies of just a few MHz can be enough
 * to make the interrupt use 100% of CPU time.  In contrast, DMA can require additional resources to be allocated,
 * (e.g. chips with few DMA channels might only support DMA on one or two %SPI busses at a time), but
 * can run the bus at full speed without any CPU overhead.  Generally, DMA should be preferred if available,
 * especially if medium to fast bus speeds are needed.</p>
 *
 * <p>Consult your chip documentation and the Mbed port docs for your target to find out what is needed to enable
 * DMA support.  For example, for STMicro targets, see
 * <a href="https://github.com/mbed-ce/mbed-os/wiki/STM32-DMA-SPI-Support">here</a>.To select DMA or interrupts,
 * use the SPI::set_dma_usage() function.  By default, interrupt %SPI will be used unless you change the
 * setting.</p>
 *
 * <h3>Async: Queueing</h3>
 * <p>The async %SPI system supports an optional transaction queueing mechanism.  When this is enabled,
 * Mbed will allow multiple transactions to be queued up on a single bus, and will execute each one
 * and deliver the appropriate callback in series.  This is mainly useful for the non-blocking
 * async api (SPI::transfer()), though you can also use it with the blocking API by having multiple
 * threads call it at once.</p>
 *
 * <p>The transaction queue size defaults to 2 on most devices, but you can change that using the
 * \c drivers.spi_transaction_queue_len option, e.g.</p>
 * \code{.json}
 * {
 *     "target_overrides": {
 *          "*": {
 *              "drivers.spi_transaction_queue_len": 3
 *          }
 *     }
 * }
 * \endcode
 *
 * <p>To save a little bit of memory, you can also set the queue length to 0 to disable the queueing mechanism.</p>
 *
 * \warning Currently, the value set by SPI::set_default_write_value() is
 * <a href="https://github.com/ARMmbed/mbed-os/issues/13941">not respected</a> by the asynchronous %SPI
 * code.  This needs to be fixed.
 */
class SPI : private NonCopyable<SPI> {

public:

    /** Create a SPI master connected to the specified pins.
     *
     *  @note This constructor passes the SSEL pin selection to the target HAL.
     *  Not all targets support SSEL, so this cannot be relied on in portable code.
     *  Portable code should use the alternative constructor that uses GPIO
     *  for SSEL.
     *
     *  @note You can specify mosi or miso as NC if not used.
     *
     *  @param mosi SPI Master Out, Slave In pin.
     *  @param miso SPI Master In, Slave Out pin.
     *  @param sclk SPI Clock pin.
     *  @param ssel SPI Chip Select pin.
     */
    SPI(PinName mosi, PinName miso, PinName sclk, PinName ssel = NC);

    /** Create a SPI master connected to the specified pins.
     *
     *  @note This constructor manipulates the SSEL pin as a GPIO output
     *  using a DigitalOut object. This should work on any target, and permits
     *  the use of select() and deselect() methods to keep the pin asserted
     *  between transfers.
     *
     *  @note You can specify mosi or miso as NC if not used.
     *
     *  @param mosi SPI Master Out, Slave In pin.
     *  @param miso SPI Master In, Slave Out pin.
     *  @param sclk SPI Clock pin.
     *  @param ssel SPI Chip Select pin.
     */
    SPI(PinName mosi, PinName miso, PinName sclk, PinName ssel, use_gpio_ssel_t);

    /** Create a SPI master connected to the specified pins.
     *
     *  @note This constructor passes the SSEL pin selection to the target HAL.
     *  Not all targets support SSEL, so this cannot be relied on in portable code.
     *  Portable code should use the alternative constructor that uses GPIO
     *  for SSEL.
     *
     *  @note You can specify mosi or miso as NC if not used.
     *
     *  @param static_pinmap reference to structure which holds static pinmap.
     */
    SPI(const spi_pinmap_t &static_pinmap);
    SPI(const spi_pinmap_t &&) = delete; // prevent passing of temporary objects

    /** Create a SPI master connected to the specified pins.
     *
     *  @note This constructor manipulates the SSEL pin as a GPIO output
     *  using a DigitalOut object. This should work on any target, and permits
     *  the use of select() and deselect() methods to keep the pin asserted
     *  between transfers.
     *
     *  @note You can specify mosi or miso as NC if not used.
     *
     *  @param static_pinmap reference to structure which holds static pinmap.
     *  @param ssel SPI Chip Select pin.
     */
    SPI(const spi_pinmap_t &static_pinmap, PinName ssel);
    SPI(const spi_pinmap_t &&, PinName) = delete; // prevent passing of temporary objects

    virtual ~SPI();

    /**
     * @brief Configure the data transmission format.
     *
     * @param bits Number of bits per %SPI frame (4 - 32, target dependent).
     * @param mode Clock polarity and phase mode (0 - 3).
     *
     * @code
     * mode | POL PHA
     * -----+--------
     *   0  |  0   0
     *   1  |  0   1
     *   2  |  1   0
     *   3  |  1   1
     * @endcode
     */
    void format(int bits, int mode = 0);

    /**
     * @brief Set the %SPI bus clock frequency.
     *
     * @param hz Clock frequency in Hz (default = 1MHz).
     */
    void frequency(int hz = 1000000);

    /**
     * @brief Write to the %SPI Slave and return the response.
     *
     * @param value Data to be sent to the SPI slave.  The number of significant bits in this
     *     value depend on the \c bits parameter to format().
     *
     * @return Response from the %SPI slave.  The number of significant bits in this
     *     value depend on the \c bits parameter to format().
     */
    virtual int write(int value);

    /**
     * @brief Write to the SPI Slave and obtain the response.
     *
     * The total number of bytes sent and received will be the maximum of
     * tx_length and rx_length. The bytes written will be padded with the
     * value 0xff.
     *
     * Note: Even if the word size / bits per frame is not 8, \c rx_length and \c tx_length
     * still count bytes of input data, not numbers of words.
     *
     * @param tx_buffer Pointer to the byte-array of data to write to the device.
     * @param tx_length Number of bytes to write, may be zero.
     * @param rx_buffer Pointer to the byte-array of data to read from the device.
     * @param rx_length Number of bytes to read, may be zero.
     * @return
     *      The number of bytes written and read from the device. This is
     *      maximum of tx_length and rx_length.
     */
    virtual int write(const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length);

    // Convenience overload of the above for any integer type instead of char*
    /**
     * @brief Write to the SPI Slave and obtain the response.
     *
     * The total number of bytes sent and received will be the maximum of
     * tx_length and rx_length. The bytes written will be padded with the
     * value 0xff.
     *
     * Note: Even if the word size / bits per frame is not 8, \c rx_length and \c tx_length
     * still count bytes of input data, not numbers of words.
     *
     * @param tx_buffer Pointer to the byte-array of data to write to the device.
     * @param tx_length Number of bytes to write, may be zero.
     * @param rx_buffer Pointer to the byte-array of data to read from the device.
     * @param rx_length Number of bytes to read, may be zero.
     * @return
     *      The number of bytes written and read from the device (as an int). This is
     *      maximum of tx_length and rx_length.
     */
    template<typename WordT>
    typename std::enable_if<std::is_integral<WordT>::value, int>::type
    write(const WordT *tx_buffer, int tx_length, WordT *rx_buffer, int rx_length)
    {
        return write(reinterpret_cast<char const *>(tx_buffer), tx_length, reinterpret_cast<char *>(rx_buffer), rx_length);
    }

    /** Acquire exclusive access to this SPI bus.
     */
    virtual void lock(void);

    /** Release exclusive access to this SPI bus.
     */
    virtual void unlock(void);

    /** Assert the Slave Select line, acquiring exclusive access to this SPI bus.
     *
     * If use_gpio_ssel was not passed to the constructor, this only acquires
     * exclusive access; the Slave Select line will not activate until data is transferred.
     */
    void select(void);

    /** Deassert the Slave Select line, releasing exclusive access to this SPI bus.
     */
    void deselect(void);

    /** Set default write data.
      * SPI requires the master to send some data during a read operation.
      * Different devices may require different default byte values.
      * For example: A SD Card requires default bytes to be 0xFF.
      *
      * @param data Default character to be transmitted during a read operation.
      */
    void set_default_write_value(char data);

#if DEVICE_SPI_ASYNCH

    /**
     * @brief Start non-blocking SPI transfer.
     *
     * This function locks the deep sleep until any event has occurred.
     *
     * @param tx_buffer The TX buffer with data to be transferred. If NULL is passed,
     *                  the default SPI value is sent.
     * @param tx_length The length of TX buffer in bytes.
     * @param rx_buffer The RX buffer which is used for received data. If NULL is passed,
     *                  received data are ignored.
     * @param rx_length The length of RX buffer in bytes.
     * @param callback  The event callback function.
     * @param event     The logical OR of events to subscribe to.  May be #SPI_EVENT_ALL, or some combination
     *        of the flags #SPI_EVENT_ERROR, #SPI_EVENT_COMPLETE, or #SPI_EVENT_RX_OVERFLOW
     *
     * \warning Be sure to call this function with pointer types matching the frame size set for the SPI bus,
     * or undefined behavior may occur!
     *
     * @return Operation result (integer)
     * @retval 0 If the transfer has started.
     * @retval -1 if the transfer could not be enqueued (increase drivers.spi_transaction_queue_len option)
     */
    template<typename WordT>
    typename std::enable_if<std::is_integral<WordT>::value, int>::type
    transfer(const WordT *tx_buffer, int tx_length, WordT *rx_buffer, int rx_length, const event_callback_t &callback, int event = SPI_EVENT_COMPLETE)
    {
        if (spi_active(&_peripheral->spi)) {
            return queue_transfer(tx_buffer, tx_length, rx_buffer, rx_length, sizeof(WordT) * 8, callback, event);
        }
        start_transfer(tx_buffer, tx_length, rx_buffer, rx_length, sizeof(WordT) * 8, callback, event);
        return 0;
    }

    /** Start %SPI transfer and wait until it is complete.  Like the transactional API this blocks the current thread,
     * however all work is done in the background and other threads may execute.
     *
     * As long as there is space, this function will enqueue the transfer request onto the peripheral,
     * and block until it is done.
     *
     * Internally, the chip vendor may implement this function using either DMA or interrupts.
     *
     * @param tx_buffer The TX buffer with data to be transferred.  May be nullptr if tx_length is 0.
     * @param tx_length The length of TX buffer in bytes.  If 0, no transmission is done.
     * @param rx_buffer The RX buffer, which is used for received data.  May be nullptr if tx_length is 0.
     * @param rx_length The length of RX buffer in bytes  If 0, no reception is done.
     * @param timeout timeout value.  Use #rtos::Kernel::wait_for_u32_forever to wait forever (the default).
     *
     * \warning Be sure to call this function with pointer types matching the frame size set for the SPI bus,
     * or undefined behavior may occur!
     *
     * @return Operation result (integer)
     * @retval -1 if the transfer could not be enqueued (increase drivers.spi_transaction_queue_len option)
     * @retval 1 on timeout
     * @retval 2 on other error
     * @retval 0 on success
     */
    template<typename WordT>
    typename std::enable_if<std::is_integral<WordT>::value, int>::type
    transfer_and_wait(const WordT *tx_buffer, int tx_length, WordT *rx_buffer, int rx_length, rtos::Kernel::Clock::duration_u32 timeout = rtos::Kernel::wait_for_u32_forever)
    {
        // Use EventFlags to suspend the thread until the transfer finishes
        rtos::EventFlags transferResultFlags("SPI::transfer_and_wait EvFlags");

        // Simple callback from the transfer that sets the EventFlags using the I2C result event
        event_callback_t transferCallback([&](int event) {
            transferResultFlags.set(event);
        });

        int txRet = transfer(tx_buffer, tx_length, rx_buffer, rx_length, transferCallback, SPI_EVENT_ALL);
        if (txRet != 0) {
            return txRet;
        }

        // Wait until transfer complete, error, or timeout
        uint32_t result = transferResultFlags.wait_any_for(SPI_EVENT_ALL, timeout);

        if (result & osFlagsError) {
            if (result == osFlagsErrorTimeout) {
                // Timeout expired, cancel transfer.
                abort_transfer();
                return 1;
            } else {
                // Other event flags error.  Transfer might be still running so cancel it.
                abort_transfer();
                return 2;
            }
        } else {
            // Note: Cannot use a switch here because multiple flags might be set at the same time (possible
            // in the STM32 HAL code at least).
            if (result == SPI_EVENT_COMPLETE) {
                return 0;
            } else {
                // SPI peripheral level error
                return 2;
            }
        }
    }

    /** Abort the on-going SPI transfer, and continue with transfers in the queue, if any.
     */
    void abort_transfer();

    /** Clear the queue of transfers.
     */
    void clear_transfer_buffer();

    /** Clear the queue of transfers and abort the on-going transfer.
     */
    void abort_all_transfers();

    /** Configure DMA usage suggestion for non-blocking transfers.
     *
     *  @param usage The usage DMA hint for peripheral.
     *
     *  @return Result of the operation.
     *  @retval 0 The usage was set.
     *  @retval -1 Usage cannot be set as there is an ongoing transaction.
     */
    int set_dma_usage(DMAUsage usage);

#if !defined(DOXYGEN_ONLY)
protected:

    /** SPI interrupt handler.
     */
    void irq_handler_asynch(void);

    /** Start the transfer or put it on the queue.
     *
     * @param tx_buffer The TX buffer with data to be transferred. If NULL is passed,
     *                  the default SPI value is sent
     * @param tx_length The length of TX buffer in bytes.
     * @param rx_buffer The RX buffer which is used for received data. If NULL is passed,
     *                  received data are ignored.
     * @param rx_length The length of RX buffer in bytes.
     * @param bit_width The buffers element width in bits.
     * @param callback  The event callback function.
     * @param event     The event mask of events to modify.
     *
     * @return Operation success.
     * @retval 0 A transfer was started or added to the queue.
     * @retval -1 Transfer can't be added because queue is full.
     */
    int transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, unsigned char bit_width, const event_callback_t &callback, int event);

    /** Put a transfer on the transfer queue.
     *
     * @param tx_buffer The TX buffer with data to be transferred. If NULL is passed,
     *                  the default SPI value is sent.
     * @param tx_length The length of TX buffer in bytes.
     * @param rx_buffer The RX buffer which is used for received data. If NULL is passed,
     *                  received data are ignored.
     * @param rx_length The length of RX buffer in bytes.
     * @param bit_width The buffers element width in bits.
     * @param callback  The event callback function.
     * @param event     The event mask of events to modify.
     *
     * @return Operation success.
     * @retval 0 A transfer was added to the queue.
     * @retval -1 Transfer can't be added because queue is full.
     */
    int queue_transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, unsigned char bit_width, const event_callback_t &callback, int event);

    /** Configure a callback, SPI peripheral, and initiate a new transfer.
     *
     * @param tx_buffer The TX buffer with data to be transferred. If NULL is passed,
     *                  the default SPI value is sent.
     * @param tx_length The length of TX buffer in bytes.
     * @param rx_buffer The RX buffer which is used for received data. If NULL is passed,
     *                  received data are ignored.
     * @param rx_length The length of RX buffer in bytes.
     * @param bit_width The buffers element width.
     * @param callback  The event callback function.
     * @param event     The event mask of events to modify.
     */
    void start_transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, unsigned char bit_width, const event_callback_t &callback, int event);

private:
    /** Lock deep sleep only if it is not yet locked */
    void lock_deep_sleep();

    /** Unlock deep sleep in case it is locked */
    void unlock_deep_sleep();


#if MBED_CONF_DRIVERS_SPI_TRANSACTION_QUEUE_LEN
    /** Start a new transaction.
     *
     *  @param data Transaction data.
     */
    void start_transaction(transaction_t *data);

    /** Dequeue a transaction and start the transfer if there was one pending.
     */
    void dequeue_transaction();

#endif // MBED_CONF_DRIVERS_SPI_TRANSACTION_QUEUE_LEN
#endif // !defined(DOXYGEN_ONLY)
#endif // DEVICE_SPI_ASYNCH

#if !defined(DOXYGEN_ONLY)
protected:
#ifdef DEVICE_SPI_COUNT
    // HAL must have defined this as a global enum
    typedef ::SPIName SPIName;
#else
    // HAL may or may not have defined it - use a local definition
    enum SPIName { GlobalSPI };
#endif

    // All members of spi_peripheral_s must be initialized to make the structure
    // constant-initialized, and hence able to be omitted by the linker,
    // as SingletonPtr now relies on C++ constant-initialization. (Previously it
    // worked through C++ zero-initialization). And all the constants should be zero
    // to ensure it stays in the actual zero-init part of the image if used, avoiding
    // an initialized-data cost.
    struct spi_peripheral_s {
        /* Internal SPI name identifying the resources. */
        SPIName name = SPIName(0);
        /* Internal SPI object handling the resources' state. */
        spi_t spi{};
        /* Used by lock and unlock for thread safety */
        SingletonPtr<rtos::Mutex> mutex;
        /* Current user of the SPI */
        SPI *owner = nullptr;
#if DEVICE_SPI_ASYNCH && MBED_CONF_DRIVERS_SPI_TRANSACTION_QUEUE_LEN
        /* Queue of pending transfers */
        SingletonPtr<CircularBuffer<Transaction<SPI>, MBED_CONF_DRIVERS_SPI_TRANSACTION_QUEUE_LEN> > transaction_buffer;
#endif
    };

    // holds spi_peripheral_s per peripheral on the device.
    // Drawback: it costs ram size even if the device is not used, however
    // application can limit the allocation via JSON.
    static spi_peripheral_s _peripherals[SPI_PERIPHERALS_USED];
    static int _peripherals_used;

    // Holds the reference to the associated peripheral.
    spi_peripheral_s *_peripheral;

#if DEVICE_SPI_ASYNCH
    /* Interrupt */
    CThunk<SPI> _irq;
    /* Interrupt handler callback */
    event_callback_t _callback;
    /* Current preferred DMA mode @see dma_api.h */
    DMAUsage _usage;
    /* Current sate of the sleep manager */
    bool _deep_sleep_locked;
#endif // DEVICE_SPI_ASYNCH

    // Configuration.
    PinName _mosi;
    PinName _miso;
    PinName _sclk;
    PinName _hw_ssel;

    // The Slave Select GPIO if we're doing it ourselves.
    DigitalOut _sw_ssel;

    /* Size of the SPI frame */
    int _bits;
    /* Clock polairy and phase */
    int _mode;
    /* Clock frequency */
    int _hz;
    /* Default character used for NULL transfers */
    char _write_fill;
    /* Select count to handle re-entrant selection */
    int8_t _select_count;
    /* Static pinmap data */
    const spi_pinmap_t *_static_pinmap;
    /* SPI peripheral name */
    SPIName _peripheral_name;
    /* Pointer to spi init function */
    void (*_init_func)(SPI *);

private:
    void _do_construct();

    /** Private acquire function without locking/unlocking.
     *  Implemented in order to avoid duplicate locking and boost performance.
     */
    void _acquire(void);
    void _set_ssel(int);

    /** Private lookup in the static _peripherals table.
     */
    static spi_peripheral_s *_lookup(SPIName name);
    /** Allocate an entry in the static _peripherals table.
     */
    static spi_peripheral_s *_alloc();

    static void _do_init(SPI *obj);
    static void _do_init_direct(SPI *obj);


#endif //!defined(DOXYGEN_ONLY)
};

/** @}*/

} // namespace mbed

#endif // DEVICE_SPI || DOXYGEN_ONLY

#endif // MBED_SPI_H
