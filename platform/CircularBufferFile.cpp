#include "platform/CircularBufferFile.h"
#include "platform/mbed_retarget.h"

// For CircularFile
#if MBED_CONF_RTOS_PRESENT
#include "rtos/ThisThread.h"
#else
#include "platform/mbed_wait_api.h"
#endif
//End

using namespace mbed;

off_t CircularBufferFile::seek(off_t offset, int whence = SEEK_SET)
{
    return -ESPIPE;
}
off_t CircularBufferFile::size()
{
    return _buffer.size();
}
int CircularBufferFile::isatty()
{
    return false;
}
int CircularBufferFile::close()
{
    return 0;
}

CircularBufferFile::~CircularBufferFile() {}

void CircularBufferFile::api_lock(void)
{
    _mutex.lock();
}

void CircularBufferFile::api_unlock(void)
{
    _mutex.unlock();
}

ssize_t CircularBufferFile::write(const void *buffer, size_t size)
{
    const char *b = static_cast<const char *>(buffer);
    if (size == 0) {
        return 0;
    }
    api_lock();
    for (size_t i = 0; i < size; i++) {
        _buffer.push(b[i]);
    }
    api_unlock();
    size_t data_written =  size  % CIRCULAR_BUFFER_FILE_DEPTH;
    return data_written;
}

// Read is a forward iterator stream
ssize_t CircularBufferFile::read(void *buffer, size_t size)
{
    char *b = static_cast<char *>(buffer);
    size_t i = 0;
    if (size == 0) {
        return 0;
    }

    api_lock();
    while (_buffer.empty()) {
        /*if (!_blocking) {
            api_unlock();
            return -EAGAIN;
        }*/
        api_unlock();
        wait_ms(1);  // XXX todo - proper wait, WFE for non-rtos ?
        api_lock();
    }
    i = 0;
    while (i < size && !_buffer.empty()) {
        _buffer.pop(b[i++]);
    }
    api_unlock();
    return i;
}

void CircularBufferFile::wait_ms(uint32_t millisec)
{
    /* wait_ms implementation for RTOS spins until exact microseconds - we
     * want to just sleep until next tick.
     */
#if MBED_CONF_RTOS_PRESENT
    // Can make this smarter in the future
    rtos::ThisThread::sleep_for(millisec);
#else
    ::wait_ms(millisec);
#endif
}

# if MBED_CONF_PLATFORM_SOFT_FILE_BUFFER
FileHandle *mbed_override_console(int fd)
{
    static CircularBufferFile console;
    return &console;
}
#endif
