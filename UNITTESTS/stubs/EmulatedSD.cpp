#include "EmulatedSD.h"
#include <stdio.h>

class EmulatedSD_Private {
public:
    ~EmulatedSD_Private() {
        if (fs) {
            fclose(fs);
        }
    }
    const char *path;
    FILE *fs;
    bd_size_t size;
};

EmulatedSD::EmulatedSD(const char *path)
{
    _p = new EmulatedSD_Private;
    _p->path = path;
}

EmulatedSD::~EmulatedSD()
{
    delete _p;
}

int EmulatedSD::init()
{
    _p->fs = fopen(_p->path, "r+");
    if (!_p->fs) {
        perror("fdopen():");
        return -1;
    }
    if (fseek(_p->fs, 0, SEEK_END)) {
        perror("fseek()");
        fclose(_p->fs);
        _p->fs = nullptr;
        return -1;
    }
    _p->size = ftell(_p->fs);
    rewind(_p->fs);
    return 0;
}

int EmulatedSD::deinit()
{
    fclose(_p->fs);
    _p->fs = nullptr;
}

int EmulatedSD::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    if (!_p->fs) {
        return -1;
    }
    if (fseek(_p->fs, addr, SEEK_SET)) {
        perror("fseek()");
        return -1;
    }
    size_t r = fread(buffer, size, 1, _p->fs);
    if (r < 1) {
        perror("fread()");
        return -1;
    }
    return 0;
}

int EmulatedSD::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    if (!_p->fs) {
        return -1;
    }
    if (fseek(_p->fs, addr, SEEK_SET)) {
        perror("fseek()");
        return -1;
    }
    size_t r = fwrite(buffer, size, 1, _p->fs);
    if (r < 1) {
        perror("fread()");
        return -1;
    }
    return 0;
}

bd_size_t EmulatedSD::get_read_size() const
{
    return 512;
}
bd_size_t EmulatedSD::get_program_size() const
{
    return 512;
}
bd_size_t EmulatedSD::size() const
{
    if (!_p->fs) {
        return -1;
    }
    return _p->size;
}
const char *EmulatedSD::get_type() const
{
    return "SD";
}
