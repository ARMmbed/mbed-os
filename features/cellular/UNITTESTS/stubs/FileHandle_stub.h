#ifndef __FILE_HANDLE_STUB_H__
#define __FILE_HANDLE_STUB_H__

#include "FileHandle.h"


namespace mbed {

static uint8_t filehandle_stub_short_value_counter = 0;
static char *filehandle_stub_table = NULL;
static uint8_t filehandle_stub_table_pos = 0;

class FileHandle_stub : public FileHandle
{
public:
    size_t size_value;

    FileHandle_stub() {size_value = 0;}

    virtual ssize_t read(void *buffer, size_t size){
        if (filehandle_stub_table) {
            ssize_t ret = strlen(filehandle_stub_table) - filehandle_stub_table_pos;
            if (size < ret) {
                ret = size;
            }
            memcpy(buffer, filehandle_stub_table, ret);
            filehandle_stub_table_pos += ret;
            return ret;
        }
        return 0;
    }

    virtual ssize_t write(const void *buffer, size_t size){
        if( size_value ) {
            size_value--;
            return size;
        }
        return 0;
    }

    virtual off_t seek(off_t offset, int whence = SEEK_SET){return 0;}

    virtual int close(){}

    virtual short poll(short events) const{
        if (filehandle_stub_short_value_counter) {
            filehandle_stub_short_value_counter--;
            return short_value;
        }
        return 0;
    }

    virtual void sigio(Callback<void()> func){func();}

    short short_value;
};

}

#endif


