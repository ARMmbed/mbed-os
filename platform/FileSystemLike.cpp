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
#include "platform/FileSystemLike.h"

namespace mbed {

class BaseDirHandle : public DirHandle {
public:
    /*
      We keep track of our current location as the n'th object in the
      FileSystemLike list. Using a Base* instead would cause problems if that
      object were to be destroyed between readdirs.
      Using this method does mean though that destroying/creating objects can
      give unusual results from readdir.
    */
    off_t n;

    BaseDirHandle() : DirHandle(), n(0) {
    }

    virtual int close() {
        // No lock can be used in destructor
        delete this;
        return 0;
    }

    virtual int read(struct dirent *ent) {
        lock();
        FileBase *ptr = FileBase::get(n);
        if (ptr == NULL) {
            unlock();
            return -1;
        }

        /* Increment n, so next readdir gets the next item */
        n++;

        /* Setup cur entry and return a pointer to it */
        std::strncpy(ent->d_name, ptr->getName(), NAME_MAX);
        unlock();
        return 0;
    }

    virtual off_t tell() {
        lock();
        off_t offset = n;
        unlock();
        return offset;
    }

    virtual void seek(off_t offset) {
        lock();
        n = offset;
        unlock();
    }

    virtual void rewind() {
        lock();
        n = 0;
        unlock();
    }

protected:
    PlatformMutex _mutex;

    virtual void lock() {
        _mutex.lock();
    }

    virtual void unlock() {
        _mutex.unlock();
    }
};

FileSystemLike::FileSystemLike(const char *name) : FileBase(name, FileSystemPathType) {

}

FileSystemLike::~FileSystemLike() {

}

DirHandle *FileSystemLike::opendir() {
    return new BaseDirHandle();
}

} // namespace mbed
