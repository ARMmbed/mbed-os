/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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
#include "filesystem/mbed_filesystem.h"
#include "LittleFileSystem2.h"
#include "errno.h"
#include "lfs2.h"
#include "lfs2_util.h"
#include "MbedCRC.h"

namespace mbed {

extern "C" uint32_t lfs2_crc(uint32_t crc, const void *buffer, size_t size)
{
    uint32_t initial_xor = lfs2_rbit(crc);
    MbedCRC<POLY_32BIT_ANSI, 32, CrcMode::TABLE> ct(initial_xor, 0x0, true, true);

    ct.compute((void *)buffer, size, &crc);
    return crc;
}

////// Conversion functions //////
static int lfs2_toerror(int err)
{
    switch (err) {
        case LFS2_ERR_OK:
            return 0;
        case LFS2_ERR_IO:
            return -EIO;
        case LFS2_ERR_NOENT:
            return -ENOENT;
        case LFS2_ERR_EXIST:
            return -EEXIST;
        case LFS2_ERR_NOTDIR:
            return -ENOTDIR;
        case LFS2_ERR_ISDIR:
            return -EISDIR;
        case LFS2_ERR_INVAL:
            return -EINVAL;
        case LFS2_ERR_NOSPC:
            return -ENOSPC;
        case LFS2_ERR_NOMEM:
            return -ENOMEM;
        case LFS2_ERR_CORRUPT:
            return -EILSEQ;
        default:
            return err;
    }
}

static int lfs2_fromflags(int flags)
{
    return (
               (((flags & 3) == O_RDONLY) ? LFS2_O_RDONLY : 0) |
               (((flags & 3) == O_WRONLY) ? LFS2_O_WRONLY : 0) |
               (((flags & 3) == O_RDWR)   ? LFS2_O_RDWR   : 0) |
               ((flags & O_CREAT)  ? LFS2_O_CREAT  : 0) |
               ((flags & O_EXCL)   ? LFS2_O_EXCL   : 0) |
               ((flags & O_TRUNC)  ? LFS2_O_TRUNC  : 0) |
               ((flags & O_APPEND) ? LFS2_O_APPEND : 0));
}

static int lfs2_fromwhence(int whence)
{
    switch (whence) {
        case SEEK_SET:
            return LFS2_SEEK_SET;
        case SEEK_CUR:
            return LFS2_SEEK_CUR;
        case SEEK_END:
            return LFS2_SEEK_END;
        default:
            return whence;
    }
}

static int lfs2_tomode(int type)
{
    int mode = S_IRWXU | S_IRWXG | S_IRWXO;
    switch (type) {
        case LFS2_TYPE_DIR:
            return mode | S_IFDIR;
        case LFS2_TYPE_REG:
            return mode | S_IFREG;
        default:
            return 0;
    }
}

static int lfs2_totype(int type)
{
    switch (type) {
        case LFS2_TYPE_DIR:
            return DT_DIR;
        case LFS2_TYPE_REG:
            return DT_REG;
        default:
            return DT_UNKNOWN;
    }
}


////// Block device operations //////
static int lfs2_bd_read(const struct lfs2_config *c, lfs2_block_t block,
                        lfs2_off_t off, void *buffer, lfs2_size_t size)
{
    BlockDevice *bd = (BlockDevice *)c->context;
    return bd->read(buffer, (bd_addr_t)block * c->block_size + off, size);
}

static int lfs2_bd_prog(const struct lfs2_config *c, lfs2_block_t block,
                        lfs2_off_t off, const void *buffer, lfs2_size_t size)
{
    BlockDevice *bd = (BlockDevice *)c->context;
    return bd->program(buffer, (bd_addr_t)block * c->block_size + off, size);
}

static int lfs2_bd_erase(const struct lfs2_config *c, lfs2_block_t block)
{
    BlockDevice *bd = (BlockDevice *)c->context;
    return bd->erase((bd_addr_t)block * c->block_size, c->block_size);
}

static int lfs2_bd_sync(const struct lfs2_config *c)
{
    BlockDevice *bd = (BlockDevice *)c->context;
    return bd->sync();
}


////// Generic filesystem operations //////

// Filesystem implementation (See LittleFileSystem2.h)
LittleFileSystem2::LittleFileSystem2(const char *name, BlockDevice *bd,
                                     lfs2_size_t block_size, uint32_t block_cycles,
                                     lfs2_size_t cache_size, lfs2_size_t lookahead_size)
    : FileSystem(name)
{
    memset(&_config, 0, sizeof(_config));
    _config.block_size = block_size;
    _config.block_cycles = block_cycles;
    _config.cache_size = cache_size;
    _config.lookahead_size = lookahead_size;
    if (bd) {
        mount(bd);
    }
}

LittleFileSystem2::~LittleFileSystem2()
{
    // nop if unmounted
    unmount();
}

int LittleFileSystem2::mount(BlockDevice *bd)
{
    _mutex.lock();
    _bd = bd;
    int err = _bd->init();
    if (err) {
        _bd = NULL;
        _mutex.unlock();
        return err;
    }

    _config.context         = bd;
    _config.read            = lfs2_bd_read;
    _config.prog            = lfs2_bd_prog;
    _config.erase           = lfs2_bd_erase;
    _config.sync            = lfs2_bd_sync;
    _config.read_size       = bd->get_read_size();
    _config.prog_size       = bd->get_program_size();
    _config.block_size      = lfs2_max(_config.block_size, (lfs2_size_t)bd->get_erase_size());
    _config.block_count     = bd->size() / _config.block_size;
    _config.block_cycles    = _config.block_cycles;
    _config.cache_size      = lfs2_max(_config.cache_size, _config.prog_size);
    _config.lookahead_size  = lfs2_min(_config.lookahead_size, 8 * ((_config.block_count + 63) / 64));

    err = lfs2_mount(&_lfs, &_config);
    if (err) {
        _bd = NULL;
        _mutex.unlock();
        return lfs2_toerror(err);
    }

    _mutex.unlock();
    return 0;
}

int LittleFileSystem2::unmount()
{
    _mutex.lock();
    int res = 0;
    if (_bd) {
        int err = lfs2_unmount(&_lfs);
        if (err && !res) {
            res = lfs2_toerror(err);
        }

        err = _bd->deinit();
        if (err && !res) {
            res = err;
        }

        _bd = NULL;
    }

    _mutex.unlock();
    return res;
}

int LittleFileSystem2::format(BlockDevice *bd,
                              lfs2_size_t block_size, uint32_t block_cycles,
                              lfs2_size_t cache_size, lfs2_size_t lookahead_size)
{
    int err = bd->init();
    if (err) {
        return err;
    }

    lfs2_t _lfs;
    struct lfs2_config _config;

    memset(&_config, 0, sizeof(_config));
    _config.context         = bd;
    _config.read            = lfs2_bd_read;
    _config.prog            = lfs2_bd_prog;
    _config.erase           = lfs2_bd_erase;
    _config.sync            = lfs2_bd_sync;
    _config.read_size       = bd->get_read_size();
    _config.prog_size       = bd->get_program_size();
    _config.block_size      = lfs2_max(block_size, (lfs2_size_t)bd->get_erase_size());
    _config.block_count     = bd->size() / _config.block_size;
    _config.block_cycles    = block_cycles;
    _config.cache_size      = lfs2_max(cache_size, _config.prog_size);
    _config.lookahead_size  = lfs2_min(lookahead_size, 8 * ((_config.block_count + 63) / 64));

    err = lfs2_format(&_lfs, &_config);
    if (err) {
        return lfs2_toerror(err);
    }

    err = bd->deinit();
    if (err) {
        return err;
    }

    return 0;
}

int LittleFileSystem2::reformat(BlockDevice *bd)
{
    _mutex.lock();
    if (_bd) {
        if (!bd) {
            bd = _bd;
        }

        int err = unmount();
        if (err) {
            _mutex.unlock();
            return err;
        }
    }

    if (!bd) {
        _mutex.unlock();
        return -ENODEV;
    }

    int err = LittleFileSystem2::format(bd,
                                        _config.block_size,
                                        _config.block_cycles,
                                        _config.cache_size,
                                        _config.lookahead_size);
    if (err) {
        _mutex.unlock();
        return err;
    }

    err = mount(bd);
    if (err) {
        _mutex.unlock();
        return err;
    }

    _mutex.unlock();
    return 0;
}

int LittleFileSystem2::remove(const char *filename)
{
    _mutex.lock();
    int err = lfs2_remove(&_lfs, filename);
    _mutex.unlock();
    return lfs2_toerror(err);
}

int LittleFileSystem2::rename(const char *oldname, const char *newname)
{
    _mutex.lock();
    int err = lfs2_rename(&_lfs, oldname, newname);
    _mutex.unlock();
    return lfs2_toerror(err);
}

int LittleFileSystem2::mkdir(const char *name, mode_t mode)
{
    _mutex.lock();
    int err = lfs2_mkdir(&_lfs, name);
    _mutex.unlock();
    return lfs2_toerror(err);
}

int LittleFileSystem2::stat(const char *name, struct stat *st)
{
    struct lfs2_info info;
    _mutex.lock();
    int err = lfs2_stat(&_lfs, name, &info);
    _mutex.unlock();
    st->st_size = info.size;
    st->st_mode = lfs2_tomode(info.type);
    return lfs2_toerror(err);
}

int LittleFileSystem2::statvfs(const char *name, struct statvfs *st)
{
    memset(st, 0, sizeof(struct statvfs));

    lfs2_ssize_t in_use = 0;
    _mutex.lock();
    in_use = lfs2_fs_size(&_lfs);
    _mutex.unlock();
    if (in_use < 0) {
        return in_use;
    }

    st->f_bsize  = _config.block_size;
    st->f_frsize = _config.block_size;
    st->f_blocks = _config.block_count;
    st->f_bfree  = _config.block_count - in_use;
    st->f_bavail = _config.block_count - in_use;
    st->f_namemax = LFS2_NAME_MAX;
    return 0;
}

////// File operations //////
int LittleFileSystem2::file_open(fs_file_t *file, const char *path, int flags)
{
    lfs2_file_t *f = new lfs2_file_t;
    _mutex.lock();
    int err = lfs2_file_open(&_lfs, f, path, lfs2_fromflags(flags));
    _mutex.unlock();
    if (!err) {
        *file = f;
    } else {
        delete f;
    }
    return lfs2_toerror(err);
}

int LittleFileSystem2::file_close(fs_file_t file)
{
    lfs2_file_t *f = (lfs2_file_t *)file;
    _mutex.lock();
    int err = lfs2_file_close(&_lfs, f);
    _mutex.unlock();
    delete f;
    return lfs2_toerror(err);
}

ssize_t LittleFileSystem2::file_read(fs_file_t file, void *buffer, size_t len)
{
    lfs2_file_t *f = (lfs2_file_t *)file;
    _mutex.lock();
    lfs2_ssize_t res = lfs2_file_read(&_lfs, f, buffer, len);
    _mutex.unlock();
    return lfs2_toerror(res);
}

ssize_t LittleFileSystem2::file_write(fs_file_t file, const void *buffer, size_t len)
{
    lfs2_file_t *f = (lfs2_file_t *)file;
    _mutex.lock();
    lfs2_ssize_t res = lfs2_file_write(&_lfs, f, buffer, len);
    _mutex.unlock();
    return lfs2_toerror(res);
}

int LittleFileSystem2::file_sync(fs_file_t file)
{
    lfs2_file_t *f = (lfs2_file_t *)file;
    _mutex.lock();
    int err = lfs2_file_sync(&_lfs, f);
    _mutex.unlock();
    return lfs2_toerror(err);
}

off_t LittleFileSystem2::file_seek(fs_file_t file, off_t offset, int whence)
{
    lfs2_file_t *f = (lfs2_file_t *)file;
    _mutex.lock();
    off_t res = lfs2_file_seek(&_lfs, f, offset, lfs2_fromwhence(whence));
    _mutex.unlock();
    return lfs2_toerror(res);
}

off_t LittleFileSystem2::file_tell(fs_file_t file)
{
    lfs2_file_t *f = (lfs2_file_t *)file;
    _mutex.lock();
    off_t res = lfs2_file_tell(&_lfs, f);
    _mutex.unlock();
    return lfs2_toerror(res);
}

off_t LittleFileSystem2::file_size(fs_file_t file)
{
    lfs2_file_t *f = (lfs2_file_t *)file;
    _mutex.lock();
    off_t res = lfs2_file_size(&_lfs, f);
    _mutex.unlock();
    return lfs2_toerror(res);
}

int LittleFileSystem2::file_truncate(fs_file_t file, off_t length)
{
    lfs2_file_t *f = (lfs2_file_t *)file;
    _mutex.lock();
    int err = lfs2_file_truncate(&_lfs, f, length);
    _mutex.unlock();
    return lfs2_toerror(err);
}


////// Dir operations //////
int LittleFileSystem2::dir_open(fs_dir_t *dir, const char *path)
{
    lfs2_dir_t *d = new lfs2_dir_t;
    _mutex.lock();
    int err = lfs2_dir_open(&_lfs, d, path);
    _mutex.unlock();
    if (!err) {
        *dir = d;
    } else {
        delete d;
    }
    return lfs2_toerror(err);
}

int LittleFileSystem2::dir_close(fs_dir_t dir)
{
    lfs2_dir_t *d = (lfs2_dir_t *)dir;
    _mutex.lock();
    int err = lfs2_dir_close(&_lfs, d);
    _mutex.unlock();
    delete d;
    return lfs2_toerror(err);
}

ssize_t LittleFileSystem2::dir_read(fs_dir_t dir, struct dirent *ent)
{
    lfs2_dir_t *d = (lfs2_dir_t *)dir;
    struct lfs2_info info;
    _mutex.lock();
    int res = lfs2_dir_read(&_lfs, d, &info);
    _mutex.unlock();
    if (res == 1) {
        ent->d_type = lfs2_totype(info.type);
        strcpy(ent->d_name, info.name);
    }
    return lfs2_toerror(res);
}

void LittleFileSystem2::dir_seek(fs_dir_t dir, off_t offset)
{
    lfs2_dir_t *d = (lfs2_dir_t *)dir;
    _mutex.lock();
    lfs2_dir_seek(&_lfs, d, offset);
    _mutex.unlock();
}

off_t LittleFileSystem2::dir_tell(fs_dir_t dir)
{
    lfs2_dir_t *d = (lfs2_dir_t *)dir;
    _mutex.lock();
    lfs2_soff_t res = lfs2_dir_tell(&_lfs, d);
    _mutex.unlock();
    return lfs2_toerror(res);
}

void LittleFileSystem2::dir_rewind(fs_dir_t dir)
{
    lfs2_dir_t *d = (lfs2_dir_t *)dir;
    _mutex.lock();
    lfs2_dir_rewind(&_lfs, d);
    _mutex.unlock();
}

} // namespace mbed
