/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#ifndef MBED_BUFFEREDSERIAL_H
#define MBED_BUFFEREDSERIAL_H

#include "platform/platform.h"

#if (DEVICE_SERIAL && DEVICE_INTERRUPTIN) || defined(DOXYGEN_ONLY)

#include "platform/FileHandle.h"
#include "drivers/SerialBase.h"
#include "drivers/InterruptIn.h"
#include "platform/PlatformMutex.h"
#include "platform/CircularBuffer.h"
#include "platform/NonCopyable.h"

#ifndef MBED_CONF_DRIVERS_UART_SERIAL_RXBUF_SIZE
#define MBED_CONF_DRIVERS_UART_SERIAL_RXBUF_SIZE  256
#endif

#ifndef MBED_CONF_DRIVERS_UART_SERIAL_TXBUF_SIZE
#define MBED_CONF_DRIVERS_UART_SERIAL_TXBUF_SIZE  256
#endif

namespace mbed {
/**
 * \defgroup drivers_BufferedSerial BufferedSerial class
 * \ingroup drivers-public-api-uart
 * @{
 */

/** Class providing buffered UART communication functionality using separate
 *  circular buffer for send and receive channels
 *
 */

class BufferedSerial:
    private SerialBase,
    public FileHandle,
    private NonCopyable<BufferedSerial> {

public:

    /** Create a BufferedSerial port, connected to the specified transmit and
     *  receive pins, with a particular baud rate.
     *  @param tx Transmit pin
     *  @param rx Receive pin
     *  @param baud The baud rate of the serial port (optional, defaults to
     *              MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE)
     */
    BufferedSerial(
        PinName tx,
        PinName rx,
        int baud = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE
    );

    /** Create a BufferedSerial port, connected to the specified transmit and
     *  receive pins, with a particular baud rate.
     *  @param static_pinmap reference to structure which holds static pinmap
     *  @param baud The baud rate of the serial port (optional, defaults to
     *              MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE)
     */
    BufferedSerial(
        const serial_pinmap_t &static_pinmap,
        int baud = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE
    );

    ~BufferedSerial() override;

    /** Equivalent to POSIX poll(). Derived from FileHandle.
     *  Provides a mechanism to multiplex input/output over a set of file
     *  handles.
     *  The events that can be reported are POLLIN, POLLOUT, POLLHUP.
     */
    short poll(short events) const final;

    /* Resolve ambiguities versus our private SerialBase
     * (for writable, spelling differs, but just in case)
     */
    using FileHandle::readable;
    using FileHandle::writable;

    /** Write the contents of a buffer to a file
     *
     *  Follows POSIX semantics:
     *
     * * if blocking, block until all data is written
     * * if no data can be written, and non-blocking set, return -EAGAIN
     * * if some data can be written, and non-blocking set, write partial
     *
     *  @param buffer   The buffer to write from
     *  @param length   The number of bytes to write
     *  @return         The number of bytes written, negative error on failure
     */
    ssize_t write(const void *buffer, size_t length) override;

    /** Read the contents of a file into a buffer
     *
     *  Follows POSIX semantics:
     *
     *  * if no data is available, and non-blocking set return -EAGAIN
     *  * if no data is available, and blocking set, wait until data is
     *    available
     *  * If any data is available, call returns immediately
     *
     *  @param buffer   The buffer to read in to
     *  @param length   The number of bytes to read
     *  @return         The number of bytes read, 0 at end of file, negative
     *                  error on failure
     */
    ssize_t read(void *buffer, size_t length) override;

    /** Close a file
     *
     *  @return         0 on success, negative error code on failure
     */
    int close() override;

    /** Check if the file in an interactive terminal device
     *
     *  @return         True if the file is a terminal
     *  @return         False if the file is not a terminal
     *  @return         Negative error code on failure
     */
    int isatty() override;

    /** Move the file position to a given offset from from a given location
     *
     * Not valid for a device type FileHandle like BufferedSerial.
     * In case of BufferedSerial, returns ESPIPE
     *
     *  @param offset   The offset from whence to move to
     *  @param whence   The start of where to seek
     *      SEEK_SET to start from beginning of file,
     *      SEEK_CUR to start from current position in file,
     *      SEEK_END to start from end of file
     *  @return         The new offset of the file, negative error code on
     *                  failure
     */
    off_t seek(off_t offset, int whence) override;

    /** Flush any buffers associated with the file
     *
     *  @return         0 on success, negative error code on failure
     */
    int sync() override;

    /** Set blocking or non-blocking mode
     *  The default is blocking.
     *
     *  @param blocking true for blocking mode, false for non-blocking mode.
     */
    int set_blocking(bool blocking) override
    {
        _blocking = blocking;
        return 0;
    }

    /** Check current blocking or non-blocking mode for file operations.
     *
     *  @return true for blocking mode, false for non-blocking mode.
     */
    bool is_blocking() const override
    {
        return _blocking;
    }

    /** Enable or disable input
     *
     * Control enabling of device for input. This is primarily intended
     * for temporary power-saving; the overall ability of the device to operate
     * for input and/or output may be fixed at creation time, but this call can
     * allow input to be temporarily disabled to permit power saving without
     * losing device state.
     *
     *  @param enabled      true to enable input, false to disable.
     *
     *  @return             0 on success
     *  @return             Negative error code on failure
     */
    int enable_input(bool enabled) override;

    /** Enable or disable output
     *
     * Control enabling of device for output. This is primarily intended
     * for temporary power-saving; the overall ability of the device to operate
     * for input and/or output may be fixed at creation time, but this call can
     * allow output to be temporarily disabled to permit power saving without
     * losing device state.
     *
     *  @param enabled      true to enable output, false to disable.
     *
     *  @return             0 on success
     *  @return             Negative error code on failure
     */
    int enable_output(bool enabled) override;

    /** Register a callback on state change of the file.
     *
     *  The specified callback will be called on state changes such as when
     *  the file can be written to or read from.
     *
     *  The callback may be called in an interrupt context and should not
     *  perform expensive operations.
     *
     *  Note! This is not intended as an attach-like asynchronous api, but
     *  rather as a building block for constructing  such functionality.
     *
     *  The exact timing of when the registered function
     *  is called is not guaranteed and susceptible to change. It should be
     *  used as a cue to make read/write/poll calls to find the current state.
     *
     *  @param func     Function to call on state change
     */
    void sigio(Callback<void()> func) override;

    /** Setup interrupt handler for DCD line
     *
     *  If DCD line is connected, an IRQ handler will be setup.
     *  Does nothing if DCD is NC, i.e., not connected.
     *
     *  @param dcd_pin         Pin-name for DCD
     *  @param active_high     a boolean set to true if DCD polarity is active
     *                         low
     */
    void set_data_carrier_detect(PinName dcd_pin, bool active_high = false);

    /** Set the baud rate
     *
     *  @param baud   The baud rate
     */
    void set_baud(int baud);

    // Expose private SerialBase::Parity as BufferedSerial::Parity
    using SerialBase::Parity;
    using SerialBase::None;
    using SerialBase::Odd;
    using SerialBase::Even;
    using SerialBase::Forced1;
    using SerialBase::Forced0;

    /** Set the transmission format used by the serial port
     *
     *  @param bits The number of bits in a word (5-8; default = 8)
     *  @param parity The parity used (None, Odd, Even, Forced1, Forced0;
     *                default = None)
     *  @param stop_bits The number of stop bits (1 or 2; default = 1)
     */
    void set_format(
        int bits = 8, Parity parity = BufferedSerial::None, int stop_bits = 1
    );

#if DEVICE_SERIAL_FC
    // For now use the base enum - but in future we may have extra options
    // such as XON/XOFF or manual GPIO RTSCTS.
    using SerialBase::Flow;
    using SerialBase::Disabled;
    using SerialBase::RTS;
    using SerialBase::CTS;
    using SerialBase::RTSCTS;

    /** Set the flow control type on the serial port
     *
     *  @param type the flow control type (Disabled, RTS, CTS, RTSCTS)
     *  @param flow1 the first flow control pin (RTS for RTS or RTSCTS, CTS for
     *               CTS)
     *  @param flow2 the second flow control pin (CTS for RTSCTS)
     */
    void set_flow_control(Flow type, PinName flow1 = NC, PinName flow2 = NC);
#endif

private:

    /** Acquire mutex
     */
    void api_lock(void);

    /** Release mutex
     */
    void api_unlock(void);

    /** Unbuffered write - invoked when write called from critical section
     *  @param buf_ptr   The buffer to write from
     *  @param length   The number of bytes to write
     *  @return         The number of bytes written, negative error on failure
     */
    ssize_t write_unbuffered(const char *buf_ptr, size_t length);

    /** Enable processing of byte reception IRQs and register a callback to
     * process them if the IRQs are not yet enabled and reception is enabled.
     */
    void update_rx_irq();

    /** Disable processing of byte reception IRQs and de-register callback to
     * process them.
     */
    void disable_rx_irq();

    /** Enable processing of byte transmission IRQs and register a callback to
     * process them if the IRQs are not yet enabled and transmission is enabled.
     */
    void update_tx_irq();

    /** Disable processing of byte transmission IRQs and de-register callback to
     * process them.
     */
    void disable_tx_irq();

    /** Software serial buffers
     *  By default buffer size is 256 for TX and 256 for RX. Configurable
     *  through mbed_app.json
     */
    CircularBuffer<char, MBED_CONF_DRIVERS_UART_SERIAL_RXBUF_SIZE> _rxbuf;
    CircularBuffer<char, MBED_CONF_DRIVERS_UART_SERIAL_TXBUF_SIZE> _txbuf;

    PlatformMutex _mutex;

    Callback<void()> _sigio_cb;

    bool _blocking = true;
    bool _tx_irq_enabled = false;
    bool _rx_irq_enabled = false;
    InterruptIn *_dcd_irq = nullptr;

    /** Device Hanged up
     *  Determines if the device hanged up on us.
     *
     *  @return True, if hanged up
     */
    bool hup() const;

    /** ISRs for serial
     *  Routines to handle interrupts on serial pins.
     *  Copies data into Circular Buffer.
     *  Reports the state change to File handle.
     */
    void tx_irq(void);
    void rx_irq(void);

    /** Execute a callback previously registered for state change of the file.
     */
    void wake(void);

    /** Wake on data carrier detected.
     */
    void dcd_irq(void);
};

/** @}*/

} //namespace mbed

#endif //(DEVICE_SERIAL && DEVICE_INTERRUPTIN) || defined(DOXYGEN_ONLY)
#endif //MBED_BUFFEREDSERIAL_H
