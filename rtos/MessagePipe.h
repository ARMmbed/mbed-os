/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef MESSAGE_PIPE_H
#define MESSAGE_PIPE_H

#include <stdint.h>
#include <errno.h>

#include "cmsis_os2.h"
#include "MemoryPool.h"
#include "EventFlags.h"

#include "platform/platform.h"
#include "platform/mbed_critical.h"
#include "platform/FileHandle.h"
#include "platform/NonCopyable.h"

namespace rtos {
/** \addtogroup rtos */
/** @{*/

/** The MessagePipe class is abstraction of FileHandle over Mail(subsequently Queue).
 A message is a memory block that is send to a thread or interrupt service routine.
 The MessagePipe should be configured to be not blocking when used in interrupt service routine to prevent blocking calls.

  @tparam  T         data type of a single message element.
  @tparam  queue_sz  maximum number of messages in pipe.

*/

template<typename T, uint32_t pipe_sz>
class MessagePipe : public mbed::FileHandle, private mbed::NonCopyable<MessagePipe<T, pipe_sz> > {

public:
    MessagePipe()
    {
        set_blocking(true);
    }

    /** Equivalent to POSIX poll(). Derived from FileHandle.
     *  Provides a mechanism to multiplex input/output over a set of file handles.
     */
    virtual short poll(short events) const {
        short revents = 0;

        if (!_mail.empty() && (events & POLLIN)) {
            revents |= POLLIN;
        }

        if (!_mail.full() && (events & POLLOUT)) {
            revents |= POLLOUT;
        }

        return revents;
    }

    /** Write the contents of a buffer to a file
     *
     *  @param buffer   The buffer to write from
     *  @param length   The number of bytes to write
     *  @return         The number of bytes written, negative error on failure
     */
    virtual ssize_t write(const void* buffer, size_t length) {
        
        if(length != sizeof(T)) {
            return -EINVAL;
        }

        if(_mail.full() && !_blocking) {
            return -EAGAIN;
        }

        T * message = _mail.alloc(osWaitForever);

        if(message == NULL) {
            return -EIO;
        }

        memcpy(message, buffer, sizeof(T));

        bool mail_was_empty = _mail.empty();

        _mail.put(message);

        //notify on state change 
        if(_mail.full() || mail_was_empty) {
            wake();
        }
        
        return sizeof(T);
    }

    /** Read the contents of a file into a buffer
     *
     *  Follows POSIX semantics:
     *
     *  * if no data is available, and non-blocking set return -EAGAIN
     *  * if no data is available, and blocking set, wait until data is available
     *  * If any data is available, call returns immediately
     *
     *  @param buffer   The buffer to read in to
     *  @param length   The number of bytes to read
     *  @return         The number of bytes read, 0 at end of file, negative error on failure
     */
    virtual ssize_t read(void* buffer, size_t length) {

        if(length != sizeof(T)) {
            return -EINVAL;
        }

        if(_mail.empty() && !_blocking) {
            return -EAGAIN;
        }

        bool mail_was_full = _mail.full();

        osEvent mail_event = _mail.get();

        if(mail_event.status != osEventMail) {
            return -EIO;
        }

        T * message = (T *) mail_event.value.p;

        memcpy(buffer, message, sizeof(T));

        _mail.free(message);

        //notify on state change 
        if(_mail.empty() || mail_was_full) {
            wake();
        }

        return sizeof(T);
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
        return false;
    }

    /** Move the file position to a given offset from from a given location
     *
     * Not valid for a device type FileHandle like MessagePipe.
     * In case of MessagePipe, returns ESPIPE
     *
     *  @param offset   The offset from whence to move to
     *  @param whence   The start of where to seek
     *      SEEK_SET to start from beginning of file,
     *      SEEK_CUR to start from current position in file,
     *      SEEK_END to start from end of file
     *  @return         The new offset of the file, negative error code on failure
     */
    virtual off_t seek(off_t offset, int whence)
    {
        return  -ESPIPE;
    }

    /** Flush any buffers associated with the file
     *
     *  @return         0 on success, negative error code on failure
     */
    virtual int sync()
    {
        return 0;
    }

    /** Set blocking or non-blocking mode
     *  The default is blocking.
     *
     *  @param blocking true for blocking mode, false for non-blocking mode.
     */
    virtual int set_blocking(bool blocking)
    {
        _blocking = blocking;
        return 0;
    }

    /** Register a callback on state change of the file.
     *
     *  The specified callback will be called on state changes such as when
     *  the file can be written to or read from.
     *
     *  The callback may be called in an interrupt context and should not
     *  perform expensive operations.
     *
     *  Note! This is not intended as an attach-like asynchronous api, but rather
     *  as a building block for constructing  such functionality.
     *
     *  The exact timing of when the registered function
     *  is called is not guaranteed and susceptible to change. It should be used
     *  as a cue to make read/write/poll calls to find the current state.
     *
     *  @param func     Function to call on state change
     */
    virtual void sigio(mbed::Callback<void()> func)
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

    virtual ~MessagePipe()
    {
    
    }

private:
    Mail<T, pipe_sz>        _mail;
    mbed::Callback<void()>  _sigio_cb;
    bool                    _blocking;

    void wake(void)
    {
        if (_sigio_cb) {
             _sigio_cb();
        }
    }
};

}
#endif

/** @}*/
