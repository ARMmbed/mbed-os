/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "Stream.h"

#include <cstdarg>

namespace mbed {

Stream::Stream(const char *name) : FileLike(name), _file(NULL) {
    /* open ourselves */
    char buf[12]; /* :0x12345678 + null byte */
    std::sprintf(buf, ":%p", this);
    _file = std::fopen(buf, "w+");
#if defined (__ICCARM__)
    std::setvbuf(_file,buf,_IONBF,NULL);    
#else
    setbuf(_file, NULL);
#endif
}

Stream::~Stream() {
    fclose(_file);
}

int Stream::putc(int c) {
    fflush(_file);
    return std::fputc(c, _file);
}
int Stream::puts(const char *s) {
    fflush(_file);
    return std::fputs(s, _file);
}
int Stream::getc() {
    fflush(_file);
#if defined (__ICCARM__)
    int res = std::fgetc(_file);
    if (res>=0){
        _file->_Mode = (unsigned short)(_file->_Mode & ~ 0x1000);/* Unset read mode */
        _file->_Rend = _file->_Wend;
        _file->_Next = _file->_Wend;
    }    
    return res;
#else    
    return std::fgetc(_file);
#endif    
}
char* Stream::gets(char *s, int size) {
    fflush(_file);
#if defined (__ICCARM__)
    char *str = fgets(s,size,_file);
    if (str!=NULL){
        _file->_Mode = (unsigned short)(_file->_Mode & ~ 0x1000);/* Unset read mode */
        _file->_Rend = _file->_Wend;
        _file->_Next = _file->_Wend;
    }
    return str;
#else    
    return std::fgets(s,size,_file);
#endif
}

int Stream::close() {
    return 0;
}

ssize_t Stream::write(const void* buffer, size_t length) {
    const char* ptr = (const char*)buffer;
    const char* end = ptr + length;
    while (ptr != end) {
        if (_putc(*ptr++) == EOF) {
            break;
        }
    }
    return ptr - (const char*)buffer;
}

ssize_t Stream::read(void* buffer, size_t length) {
    char* ptr = (char*)buffer;
    char* end = ptr + length;
    while (ptr != end) {
        int c = _getc();
        if (c==EOF) break;
        *ptr++ = c;
    }
    return ptr - (const char*)buffer;
}

off_t Stream::lseek(off_t offset, int whence) {
    return 0;
}

int Stream::isatty() {
    return 0;
}

int Stream::fsync() {
    return 0;
}

off_t Stream::flen() {
    return 0;
}

int Stream::printf(const char* format, ...) {
    std::va_list arg;
    va_start(arg, format);
    fflush(_file);
    int r = vfprintf(_file, format, arg);
    va_end(arg);
    return r;
}

int Stream::scanf(const char* format, ...) {
    std::va_list arg;
    va_start(arg, format);
    fflush(_file);
    int r = vfscanf(_file, format, arg);
    va_end(arg);
    return r;
}

} // namespace mbed
