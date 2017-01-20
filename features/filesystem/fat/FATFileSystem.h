/* mbed Microcontroller Library
 * Copyright (c) 2006-2012 ARM Limited
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
#ifndef MBED_FATFILESYSTEM_H
#define MBED_FATFILESYSTEM_H

#include "FileSystemLike.h"
#include "BlockDevice.h"
#include "FileHandle.h"
#include "ff.h"
#include <stdint.h>
#include "PlatformMutex.h"

using namespace mbed;

/**
 * FATFileSystem based on ChaN's Fat Filesystem library v0.8 
 */
class FATFileSystem : public FileSystemLike {
public:
    FATFileSystem(const char* n, BlockDevice *bd = NULL);
    virtual ~FATFileSystem();
    
    /**
     * Mounts the filesystem
     */
    virtual int mount(BlockDevice *bd);
    
    /**
     * Unmounts the filesystem
     */
    virtual int unmount();

    /**
     * Flush any underlying transactions
     */
    virtual int sync();
    
    /**
     * Formats a logical drive, FDISK partitioning rule, 512 bytes per cluster
     */
    static int format(BlockDevice *bd);

    /**
     * Opens a file on the filesystem
     */
    virtual FileHandle *open(const char* name, int flags);
    
    /**
     * Removes a file path
     */
    virtual int remove(const char *filename);
    
    /**
     * Renames a file
     */
    virtual int rename(const char *oldname, const char *newname);
    
    /**
     * Opens a directory on the filesystem
     */
    virtual DirHandle *opendir(const char *name);
    
    /**
     * Creates a directory path
     */
    virtual int mkdir(const char *name, mode_t mode);

    /**
     * Store information about file in stat structure
     */
    virtual int stat(const char *name, struct stat *st);
    
protected:
    FATFS _fs; // Work area (file system object) for logical drive
    char _fsid[2];
    int _id;

    virtual int mount(BlockDevice *bd, bool force);

    virtual void lock();
    virtual void unlock();
};

#endif
