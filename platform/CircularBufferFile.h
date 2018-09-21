#ifndef CIRCULAR_BUFFER_FILE_H
#define CIRCULAR_BUFFER_FILE_H
#include "platform/platform.h"
#include "platform/FileHandle.h"
#include "platform/CircularBuffer.h"
#include "platform/PlatformMutex.h"


#ifndef CIRCULAR_BUFFER_FILE_DEPTH
#define CIRCULAR_BUFFER_FILE_DEPTH 512
#endif

namespace mbed {
class CircularBufferFile : public FileHandle {
public:
    virtual ~CircularBufferFile();
    virtual ssize_t write(const void *buffer, size_t size);
    virtual ssize_t read(void *buffer, size_t size);
    virtual off_t seek(off_t offset, int whence);
    virtual off_t size();
    virtual int isatty();
    virtual int close();
private:
    /** Acquire mutex */
    virtual void api_lock(void);

    /** Release mutex */
    virtual void api_unlock(void);
    
    void wait_ms(uint32_t millisec);
private:
    CircularBuffer<char, CIRCULAR_BUFFER_FILE_DEPTH> _buffer;
    //Callback<void()> _sigio_cb; // Todo
    PlatformMutex _mutex;
};

}
#endif /* CIRCULAR_BUFFER_FILE_H */
