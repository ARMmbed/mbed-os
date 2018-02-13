
#include "FileHandle_stub.h"

namespace mbed {

off_t FileHandle::size()
{
    return 0;
}

std::FILE *fdopen(FileHandle *fh, const char *mode)
{
    return NULL;
}

}
