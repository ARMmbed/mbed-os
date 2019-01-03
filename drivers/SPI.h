/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

#include "platform/PlatformMutex.h"
#include "hal/spi_api.h"
#include "platform/SingletonPtr.h"
#include "platform/NonCopyable.h"

#if DEVICE_SPI_ASYNCH
#include "platform/CThunk.h"
#include "hal/dma_api.h"
#include "platform/CircularBuffer.h"
#include "platform/FunctionPointer.h"
#include "platform/Transaction.h"
#endif

namespace mbed {
/** \addtogroup drivers */

/** A SPI Master, used for communicating with SPI slave devices.
 *
 * The default format is set to 8-bits, mode 0, and a clock frequency of 1MHz.
 *
 * Most SPI devices will also require Chip Select and Reset signals. These
 * can be controlled using DigitalOut pins.
 *
 * @note Synchronization level: Thread safe
 *
 * Example of how to send a byte to a SPI slave and record the response:
 * @code
 * #include "mbed.h"
 *
 * SPI device(SPI_MOSI, SPI_MISO, SPI_SCLK)
 *
 * DigitalOut chip_select(SPI_CS);
 *
 * int main() {
 *     device.lock();
 *     chip_select = 0;
 *
 *     int response = device.write(0xFF);
 *
 *     chip_select = 1;
 *     device.unlock();
 * }
 * @endcode
 *
 * Example using hardware Chip Select line:
 * @code
 * #include "mbed.h"
 *
 * SPI device(SPI_MOSI, SPI_MISO, SPI_SCLK, SPI_CS)
 *
 * int main() {
 *     device.lock();
 *     int response = device.write(0xFF);
 *     device.unlock();
 * }
 * @endcode
 * @ingroup drivers
 */
class SPI : private NonCopyable<SPI> {

public:

    /** Create a SPI master connected to the specified pins.
     *
     *  @note You can specify mosi or miso as NC if not used.
     *
     *  @param mosi SPI Master Out, Slave In pin.
     *  @param miso SPI Master In, Slave Out pin.
     *  @param sclk SPI Clock pin.
     *  @param ssel SPI Chip Select pin.
     */
    SPI(PinName mosi, PinName miso, PinName sclk, PinName ssel = NC);
    virtual ~SPI();

    /** Configure the data transmission format.
     *
     *  @param bits Number of bits per SPI frame (4 - 16).
     *  @param mode Clock polarity and phase mode (0 - 3).
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

    /** Set the SPI bus clock frequency.
     *
     *  @param hz Clock frequency in Hz (default = 1MHz).
     */
    void frequency(int hz = 1000000);

    /** Write to the SPI Slave and return the response.
     *
     *  @param value Data to be sent to the SPI slave.
     *
     *  @return Response from the SPI slave.
     */
    virtual int write(int value);

    /** Write to the SPI Slave and obtain the response.
     *
     *  The total number of bytes sent and received will be the maximum of
     *  tx_length and rx_length. The bytes written will be padded with the
     *  value 0xff.
     *
     *  @param tx_buffer Pointer to the byte-array of data to write to the device.
     *  @param tx_length Number of bytes to write, may be zero.
     *  @param rx_buffer Pointer to the byte-array of data to read from the device.
     *  @param rx_length Number of bytes to read, may be zero.
     *  @return
     *      The number of bytes written and read from the device. This is
     *      maximum of tx_length and rx_length.
     */
    virtual int write(const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length);

    /** Acquire exclusive access to this SPI bus.
     */
    virtual void lock(void);

    /** Release exclusive access to this SPI bus.
     */
    virtual void unlock(void);

    /** Set default write data.
      * SPI requires the master to send some data during a read operation.
      * Different devices may require different default byte values.
      * For example: A SD Card requires default bytes to be 0xFF.
      *
      * @param data Default character to be transmitted during a read operation.
      */
    void set_default_write_value(char data);

#if DEVICE_SPI_ASYNCH

    /** Start non-blocking SPI transfer using 8bit buffers.
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
     * @param event     The event mask of events to modify. @see spi_api.h for SPI events.
     *
     * @return Operation result.
     * @retval 0 If the transfer has started.
     * @retval -1 If SPI peripheral is busy.
     */
    template<typename Type>
    int transfer(const Type *tx_buffer, int tx_length, Type *rx_buffer, int rx_length, const event_callback_t &callback, int event = SPI_EVENT_COMPLETE)
    {
        if (spi_active(&_spi)) {
            return queue_transfer(tx_buffer, tx_length, rx_buffer, rx_length, sizeof(Type) * 8, callback, event);
        }
        start_transfer(tx_buffer, tx_length, rx_buffer, rx_length, sizeof(Type) * 8, callback, event);
        return 0;
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


#if TRANSACTION_QUEUE_SIZE_SPI

    /** Start a new transaction.
     *
     *  @param data Transaction data.
     */
    void start_transaction(transaction_t *data);

    /** Dequeue a transaction and start the transfer if there was one pending.
     */
    void dequeue_transaction();

    /* Queue of pending transfers */
    static CircularBuffer<Transaction<SPI>, TRANSACTION_QUEUE_SIZE_SPI> _transaction_buffer;
#endif

#endif //!defined(DOXYGEN_ONLY)

#endif //DEVICE_SPI_ASYNCH

#if !defined(DOXYGEN_ONLY)
protected:
    /* Internal SPI object identifying the resources */
    spi_t _spi;

#if DEVICE_SPI_ASYNCH
    /* Interrupt */
    CThunk<SPI> _irq;
    /* Interrupt handler callback */
    event_callback_t _callback;
    /* Current preferred DMA mode @see dma_api.h */
    DMAUsage _usage;
    /* Current sate of the sleep manager */
    bool _deep_sleep_locked;
#endif

    /* Take over the physical SPI and apply our settings (thread safe) */
    void aquire(void);
    /* Current user of the SPI */
    static SPI *_owner;
    /* Used by lock and unlock for thread safety */
    static SingletonPtr<PlatformMutex> _mutex;
    /* Size of the SPI frame */
    int _bits;
    /* Clock polairy and phase */
    int _mode;
    /* Clock frequency */
    int _hz;
    /* Default character used for NULL transfers */
    char _write_fill;

private:
    /** Private acquire function without locking/unlocking.
     *  Implemented in order to avoid duplicate locking and boost performance.
     */
    void _acquire(void);

#endif //!defined(DOXYGEN_ONLY)
};

} // namespace mbed

#endif

#endif
