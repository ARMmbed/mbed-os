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

#include "mbed.h"
#include "filesystem/Dir.h"
#include "filesystem/File.h"
#include "filesystem/FileSystem.h"
#include <errno.h>


FileSystem::FileSystem(const char *name)
    : FileSystemLike(name)
{
}

int FileSystem::reformat(BlockDevice *bd)
{
    return -ENOSYS;
}

int FileSystem::remove(const char *path)
{
    return -ENOSYS;
}

int FileSystem::rename(const char *path, const char *newpath)
{
    return -ENOSYS;
}

int FileSystem::stat(const char *path, struct stat *st)
{
    return -ENOSYS;
}

int FileSystem::mkdir(const char *path, mode_t mode)
{
    return -ENOSYS;
}

int FileSystem::statvfs(const char *path, struct statvfs *buf)
{
    return -ENOSYS;
}

int FileSystem::file_sync(fs_file_t file)
{
    return 0;
}

int FileSystem::file_isatty(fs_file_t file)
{
    return false;
}

off_t FileSystem::file_tell(fs_file_t file)
{
    return file_seek(file, 0, SEEK_CUR);
}

void FileSystem::file_rewind(fs_file_t file)
{
    file_seek(file, 0, SEEK_SET);
}

off_t FileSystem::file_size(fs_file_t file)
{
    off_t off = file_tell(file);
    off_t size = file_seek(file, 0, SEEK_END);
    file_seek(file, off, SEEK_SET);
    return size;
}

int FileSystem::dir_open(fs_dir_t *dir, const char *path)
{
    return -ENOSYS;
}

int FileSystem::dir_close(fs_dir_t dir)
{
    return -ENOSYS;
}

ssize_t FileSystem::dir_read(fs_dir_t dir, struct dirent *ent)
{
    return -ENOSYS;
}

void FileSystem::dir_seek(fs_dir_t dir, off_t offset)
{
}

off_t FileSystem::dir_tell(fs_dir_t dir)
{
    return 0;
}

void FileSystem::dir_rewind(fs_dir_t dir)
{
    // Note, the may not satisfy rewind on all filesystems
    dir_seek(dir, 0);
}

size_t FileSystem::dir_size(fs_dir_t dir)
{
    off_t off = dir_tell(dir);
    size_t size = 0;
    struct dirent *ent = new struct dirent;

    dir_rewind(dir);
    while (true) {
        int res = dir_read(dir, ent);
        if (res <= 0) {
            break;
        }

        size += 1;
    }
    dir_seek(dir, off);

    delete ent;
    return size;
}

// Internally used file wrapper that manages memory on close
template <typename F>
class Managed : public F {
public:
    virtual int close() {
        int err = F::close();
        delete this;
        return err;
    }
};

int FileSystem::open(FileHandle **file, const char *path, int flags)
{
    File *f = new Managed<File>;
    int err = f->open(this, path, flags);
    if (err) {
        delete f;
        return err;
    }

    *file = f;
    return 0;
}

int FileSystem::open(DirHandle **dir, const char *path) {
    Dir *d = new Managed<Dir>;
    int err = d->open(this, path);
    if (err) {
        delete d;
        return err;
    }

    *dir = d;
    return 0;
}
