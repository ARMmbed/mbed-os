/* mbed Microcontroller Library
 * Copyright (c) 2019-2020 Renesas Electronics Corporation
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

#ifndef MBED_USBSERIAL_FILEHANDLE_H
#define MBED_USBSERIAL_FILEHANDLE_H

#include "platform/platform.h"
#include "platform/FileHandle.h"
#include "platform/NonCopyable.h"
#include "USBCDC.h"

namespace mbed {

class USBSerial_FileHandle: public USBCDC, public FileHandle, private NonCopyable<USBSerial_FileHandle> {

public:

    /** Constructor
     *
     *  Construct this object optionally connecting and blocking until it is ready.
     *
     *  @note Do not use this constructor in derived classes.
     *
     *  @param connect_blocking true to perform a blocking connect, false to start in a disconnected state
     *  @param vendor_id Your vendor_id (default: 0x1f00)
     *  @param product_id Your product_id (default: 0x2012)
     *  @param product_release Your product_release (default: 0x0001)
     */
    USBSerial_FileHandle(bool connect_blocking = true, uint16_t vendor_id = 0x1f00, uint16_t product_id = 0x2012, uint16_t product_release = 0x0001)
        : USBCDC(connect_blocking, vendor_id, product_id, product_release) {}

    /** Read the contents of a file into a buffer
     *
     *  Devices acting as FileHandles should follow POSIX semantics:
     *
     *  * if no data is available, and nonblocking set, return -EAGAIN
     *  * if no data is available, and blocking set, wait until some data is available
     *  * If any data is available, call returns immediately
     *
     *  @param buffer   The buffer to read in to
     *  @param size     The number of bytes to read
     *  @return         The number of bytes read, 0 at end of file, negative error on failure
     */
    virtual ssize_t read(void *buffer, size_t size)
    {
        uint32_t data_read = 0;
        USBCDC::receive((uint8_t *)buffer, size, &data_read);
        return (ssize_t)data_read;
    }

    /** Write the contents of a buffer to a file
     *
     *  Devices acting as FileHandles should follow POSIX semantics:
     *
     * * if blocking, block until all data is written
     * * if no data can be written, and nonblocking set, return -EAGAIN
     * * if some data can be written, and nonblocking set, write partial
     *
     *  @param buffer   The buffer to write from
     *  @param size     The number of bytes to write
     *  @return         The number of bytes written, negative error on failure
     */
    virtual ssize_t write(const void *buffer, size_t size)
    {
        USBCDC::send((uint8_t *)buffer, size);
        return size;
    }

    /** Move the file position to a given offset from from a given location
     *
     *  @param offset   The offset from whence to move to
     *  @param whence   The start of where to seek
     *      SEEK_SET to start from beginning of file,
     *      SEEK_CUR to start from current position in file,
     *      SEEK_END to start from end of file
     *  @return         The new offset of the file, negative error code on failure
     */
    virtual off_t seek(off_t offset, int whence = SEEK_SET)
    {
        return -ESPIPE;
    }

    /** Close a file
     *
     *  @return         0 on success, negative error code on failure
     */
    virtual int close()
    {
        return 0;
    }

    /** Check if the file in an interactive terminal device
     *
     *  @return         True if the file is a terminal
     *  @return         False if the file is not a terminal
     *  @return         Negative error code on failure
     */
    virtual int isatty()
    {
        return true;
    }

    /** Get the size of the file
     *
     *  @return         Size of the file in bytes
     */
    virtual off_t size()
    {
        return -EINVAL;
    }

    /** Check for poll event flags
     * You can use or ignore the input parameter. You can return all events
     * or check just the events listed in events.
     * Call is nonblocking - returns instantaneous state of events.
     * Whenever an event occurs, the derived class should call the sigio() callback).
     *
     * @param events        bitmask of poll events we're interested in - POLLIN/POLLOUT etc.
     *
     * @returns             bitmask of poll events that have occurred.
     */
    virtual short poll(short events)
    {
        short revents = 0;

        USBCDC::lock();
        uint8_t size = 0;
        if (!_rx_in_progress) {
            size = _rx_size > 0xFF ? 0xFF : _rx_size;
        }
        USBCDC::unlock();

        if (size != 0) {
            revents |= POLLIN;
        }
        revents |= POLLOUT;

        return revents;
    }

    /** Register a callback on state change of the file.
     *
     *  The specified callback will be called on state changes such as when
     *  the file can be written to or read from.
     *
     *  The callback may be called in an interrupt context and should not
     *  perform expensive operations.
     *
     *  Note! This is not intended as an attach-like asynchronous API, but rather
     *  as a building block for constructing such functionality.
     *
     *  The exact timing of when the registered function
     *  is called is not guaranteed and is susceptible to change. It should be used
     *  as a cue to make read/write/poll calls to find the current state.
     *
     *  @param func     Function to call on state change
     */
    virtual void sigio(Callback<void()> func)
    {
        core_util_critical_section_enter();
        _sigio_cb = func;
        if (_sigio_cb) {
            short current_events = poll(0x7FFF);
            if (current_events) {
                _sigio_cb();
            }
        }
        core_util_critical_section_exit();
    }

protected:
    virtual void data_rx()
    {
        assert_locked();
        if (_sigio_cb) {
            _sigio_cb.call();
        }
    }

private:
    Callback<void()> _sigio_cb;

};
} //namespace mbed

#endif //MBED_USBSERIAL_FILEHANDLE_H
