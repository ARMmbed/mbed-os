/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#include "StorageLiteFS.h"

#if STORAGELITE_ENABLED

#include "mbed_retarget.h"
#include "string.h"
#include "stdlib.h"

typedef struct {
    char *file_name;
    int open_flags;
} file_handle_data_t;

// Convert basic StorageLite error numbers to errno based ones
static int convert_to_errno(int err)
{
    switch (err) {
        case STORAGELITE_SUCCESS:
            return  0;
        case STORAGELITE_READ_ERROR:
            return -EIO;
        case STORAGELITE_WRITE_ERROR:
            return -EIO;
        case STORAGELITE_NOT_FOUND:
            return -ENOENT;
        case STORAGELITE_DATA_CORRUPT:
            return -EILSEQ;
        case STORAGELITE_BAD_VALUE:
            return -EINVAL;
        case STORAGELITE_BUFF_TOO_SMALL:
            return -ENOMEM;
        case STORAGELITE_NO_SPACE_ON_BD:
            return -ENOSPC;
        case STORAGELITE_OS_ERROR:
            return -EINTR; // FIXME: Find a better error
        case STORAGELITE_UNINITIALIZED:
            return -ENXIO; // FIXME: Find a better error
        case STORAGELITE_MAX_FILES_REACHED:
            return -ENFILE;
        case STORAGELITE_NOT_SUPPORTED:
            return -ENOSYS;
        default:
            return -err;
    }
}

StorageLiteFS::StorageLiteFS(const char *name, StorageLite *stlite, uint32_t flags)
    : FileSystem(name), _stlite(stlite), _flags(flags), _bd(0)
{
    MBED_ASSERT(stlite);
    mount(_bd);
}

StorageLiteFS::~StorageLiteFS()
{
    unmount();
}

int StorageLiteFS::mount(BlockDevice *bd)
{
    _bd = bd;
    int ret = _stlite->init(bd);
    return convert_to_errno(ret);
}

int StorageLiteFS::unmount()
{
    int ret = _stlite->deinit();
    _bd = 0;
    return convert_to_errno(ret);
}

int StorageLiteFS::reformat(BlockDevice *bd)
{
    if (bd) {
        _bd = bd;
    }

    int ret = _stlite->init(_bd);
    if (ret) {
        return convert_to_errno(ret);
    }

    ret = _stlite->reset();
    if (ret) {
        return convert_to_errno(ret);
    }

    return 0;
}

int StorageLiteFS::remove(const char *filename)
{
    int ret = _stlite->remove(filename);
    return convert_to_errno(ret);
}

int StorageLiteFS::stat(const char *name, struct stat *st)
{
    size_t file_size;
    int ret = _stlite->get_file_size(name, file_size);
    st->st_size = file_size;
    return convert_to_errno(ret);
}

int StorageLiteFS::statvfs(const char *name, struct statvfs *st)
{
    memset(st, 0, sizeof(struct statvfs));
    bd_size_t bsize = _bd->get_erase_size();

    st->f_bsize = bsize;
    st->f_frsize = bsize;
    st->f_blocks = _stlite->size() / bsize;
    st->f_bfree = _stlite->free_size() / bsize;
    st->f_bavail = st->f_bfree;
    st->f_namemax = StorageLite::max_name_size;

    return 0;
}

int StorageLiteFS::file_open(mbed::fs_file_t *file, const char *path, int flags)
{
    file_handle_data_t *handle = new (std::nothrow) file_handle_data_t;
    if (!handle) {
        return -ENOMEM;
    }

    // We don't support opening for read-write, nor for appending
    if ((flags & O_RDWR) || (flags & O_APPEND)) {
        return -EINVAL;
    }

    if ((flags & 1) == O_RDONLY) {
        // When open for reading, we need to check if file exists right now
        int ret = _stlite->file_exists(path);
        if (ret != STORAGELITE_SUCCESS) {
            return convert_to_errno(ret);
        }
    } else {
        if (flags & O_EXCL) {
            int ret = _stlite->file_exists(path);
            // Fail if file exists
            if (ret == STORAGELITE_SUCCESS) {
                return -EEXIST;
            }
            if (ret != STORAGELITE_NOT_FOUND) {
                return convert_to_errno(ret);
            }
        }
    }

    handle->file_name = new (std::nothrow) char[strlen(path) + 1];
    if (!handle->file_name) {
        return -ENOMEM;
    }
    strcpy(handle->file_name, path);
    handle->open_flags = flags;

    *file = handle;
    return 0;
}

int StorageLiteFS::file_close(mbed::fs_file_t file)
{
    file_handle_data_t *handle = (file_handle_data_t *) file;

    if (!handle) {
        return -EINVAL;
    }

    // Special case - opened for write, but didn't write anything before closing.
    // Just create an empty file
    if (((handle->open_flags & 1) == O_WRONLY) && handle->file_name) {
        int ret = _stlite->set(handle->file_name, 0, 0, _flags);
        if (ret) {
            return convert_to_errno(ret);
        }
    }

    if (handle->file_name) {
        delete[] handle->file_name;
    }
    delete handle;

    return 0;
}

ssize_t StorageLiteFS::file_read(mbed::fs_file_t file, void *buffer, size_t len)
{
    file_handle_data_t *handle = (file_handle_data_t *) file;

    if (!handle) {
        return -EINVAL;
    }

    // We only support one (correct) read/write operation after open
    if (((handle->open_flags & 1) == O_WRONLY) || !handle->file_name) {
        return -ENOSYS;
    }

    size_t num_read;
    int ret = _stlite->get(handle->file_name, buffer, len, num_read);
    if (ret) {
        return convert_to_errno(ret);
    }

    // This indicates that the action is complete
    delete[] handle->file_name;
    handle->file_name = 0;

    return num_read;
}

ssize_t StorageLiteFS::file_write(mbed::fs_file_t file, const void *buffer, size_t len)
{
    file_handle_data_t *handle = (file_handle_data_t *) file;

    if (!handle) {
        return -EINVAL;
    }

    // We only support one (correct) read/write operation after open
    if (((handle->open_flags & 1) == O_RDONLY) || !handle->file_name) {
        return -ENOSYS;
    }

    int ret = _stlite->set(handle->file_name, buffer, len, _flags);
    if (ret) {
        return convert_to_errno(ret);
    }

    // This indicates that the action is complete
    delete[] handle->file_name;
    handle->file_name = 0;

    return len;
}

off_t StorageLiteFS::file_seek(mbed::fs_file_t file, off_t offset, int whence)
{
    return -ENOSYS;
}

off_t StorageLiteFS::file_tell(mbed::fs_file_t file)
{
    return -ENOSYS;
}

off_t StorageLiteFS::file_size(mbed::fs_file_t file)
{
    file_handle_data_t *handle = (file_handle_data_t *) file;

    if (!handle) {
        return -EINVAL;
    }

    // We only support one (correct) read/write operation after open
    if (((handle->open_flags & 1) == O_WRONLY) || !handle->file_name) {
        return -ENOSYS;
    }

    size_t file_size;
    int ret = _stlite->get_file_size(handle->file_name, file_size);
    if (ret) {
        return 0;
    }

    return file_size;
}

#endif // STORAGELITE_ENABLED
