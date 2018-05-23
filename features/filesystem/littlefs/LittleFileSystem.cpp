/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include "LittleFileSystem.h"
#include "errno.h"
extern "C" {
#include "lfs.h"
#include "lfs_util.h"
}


////// Conversion functions //////
static int lfs_toerror(int err)
{
    switch (err) {
        case LFS_ERR_OK:        return 0;
        case LFS_ERR_IO:        return -EIO;
        case LFS_ERR_NOENT:     return -ENOENT;
        case LFS_ERR_EXIST:     return -EEXIST;
        case LFS_ERR_NOTDIR:    return -ENOTDIR;
        case LFS_ERR_ISDIR:     return -EISDIR;
        case LFS_ERR_INVAL:     return -EINVAL;
        case LFS_ERR_NOSPC:     return -ENOSPC;
        case LFS_ERR_NOMEM:     return -ENOMEM;
        case LFS_ERR_CORRUPT:   return -EILSEQ;
        default:                return err;
    }
}

static int lfs_fromflags(int flags)
{
    return (
        (((flags & 3) == O_RDONLY) ? LFS_O_RDONLY : 0) |
        (((flags & 3) == O_WRONLY) ? LFS_O_WRONLY : 0) |
        (((flags & 3) == O_RDWR)   ? LFS_O_RDWR   : 0) |
        ((flags & O_CREAT)  ? LFS_O_CREAT  : 0) |
        ((flags & O_EXCL)   ? LFS_O_EXCL   : 0) |
        ((flags & O_TRUNC)  ? LFS_O_TRUNC  : 0) |
        ((flags & O_APPEND) ? LFS_O_APPEND : 0));
}

static int lfs_fromwhence(int whence)
{
    switch (whence) {
        case SEEK_SET: return LFS_SEEK_SET;
        case SEEK_CUR: return LFS_SEEK_CUR;
        case SEEK_END: return LFS_SEEK_END;
        default: return whence;
    }
}

static int lfs_tomode(int type)
{
    int mode = S_IRWXU | S_IRWXG | S_IRWXO;
    switch (type) {
        case LFS_TYPE_DIR: return mode | S_IFDIR;
        case LFS_TYPE_REG: return mode | S_IFREG;
        default: return 0;
    }
}

static int lfs_totype(int type)
{
    switch (type) {
        case LFS_TYPE_DIR: return DT_DIR;
        case LFS_TYPE_REG: return DT_REG;
        default: return DT_UNKNOWN;
    }
}


////// Block device operations //////
static int lfs_bd_read(const struct lfs_config *c, lfs_block_t block,
        lfs_off_t off, void *buffer, lfs_size_t size) {
    BlockDevice *bd = (BlockDevice *)c->context;
    return bd->read(buffer, (bd_addr_t)block*c->block_size + off, size);
}

static int lfs_bd_prog(const struct lfs_config *c, lfs_block_t block,
        lfs_off_t off, const void *buffer, lfs_size_t size) {
    BlockDevice *bd = (BlockDevice *)c->context;
    return bd->program(buffer, (bd_addr_t)block*c->block_size + off, size);
}

static int lfs_bd_erase(const struct lfs_config *c, lfs_block_t block)
{
    BlockDevice *bd = (BlockDevice *)c->context;
    return bd->erase((bd_addr_t)block*c->block_size, c->block_size);
}

static int lfs_bd_sync(const struct lfs_config *c)
{
    BlockDevice *bd = (BlockDevice *)c->context;
    return bd->sync();
}


////// Generic filesystem operations //////

// Filesystem implementation (See LittleFileSystem.h)
LittleFileSystem::LittleFileSystem(const char *name, BlockDevice *bd,
        lfs_size_t read_size, lfs_size_t prog_size,
        lfs_size_t block_size, lfs_size_t lookahead)
        : FileSystem(name)
        , _read_size(read_size)
        , _prog_size(prog_size)
        , _block_size(block_size)
        , _lookahead(lookahead) {
    if (bd) {
        mount(bd);
    }
}

LittleFileSystem::~LittleFileSystem() {
    // nop if unmounted
    unmount();
}

int LittleFileSystem::mount(BlockDevice *bd)
{
    _mutex.lock();
    LFS_INFO("mount(%p)", bd);
    _bd = bd;
    int err = _bd->init();
    if (err) {
        LFS_INFO("mount -> %d", err);
        _mutex.unlock();
        return err;
    }

    memset(&_config, 0, sizeof(_config));
    _config.context = bd;
    _config.read  = lfs_bd_read;
    _config.prog  = lfs_bd_prog;
    _config.erase = lfs_bd_erase;
    _config.sync  = lfs_bd_sync;
    _config.read_size   = bd->get_read_size();
    if (_config.read_size < _read_size) {
        _config.read_size = _read_size;
    }
    _config.prog_size   = bd->get_program_size();
    if (_config.prog_size < _prog_size) {
        _config.prog_size = _prog_size;
    }
    _config.block_size  = bd->get_erase_size();
    if (_config.block_size < _block_size) {
        _config.block_size = _block_size;
    }
    _config.block_count = bd->size() / _config.block_size;
    _config.lookahead = 32 * ((_config.block_count+31)/32);
    if (_config.lookahead > _lookahead) {
        _config.lookahead = _lookahead;
    }

    err = lfs_mount(&_lfs, &_config);
    LFS_INFO("mount -> %d", lfs_toerror(err));
    _mutex.unlock();
    return lfs_toerror(err);
}

int LittleFileSystem::unmount()
{
    _mutex.lock();
    LFS_INFO("unmount(%s)", "");
    if (_bd) {
        int err = lfs_unmount(&_lfs);
        if (err) {
            LFS_INFO("unmount -> %d", lfs_toerror(err));
            _mutex.unlock();
            return lfs_toerror(err);
        }

        err = _bd->deinit();
        if (err) {
            LFS_INFO("unmount -> %d", err);
            _mutex.unlock();
            return err;
        }

        _bd = NULL;
    }
    
    LFS_INFO("unmount -> %d", 0);
    _mutex.unlock();
    return 0;
}

int LittleFileSystem::format(BlockDevice *bd,
        lfs_size_t read_size, lfs_size_t prog_size,
        lfs_size_t block_size, lfs_size_t lookahead) {
    LFS_INFO("format(%p, %ld, %ld, %ld, %ld)",
            bd, read_size, prog_size, block_size, lookahead);
    int err = bd->init();
    if (err) {
        LFS_INFO("format -> %d", err);
        return err;
    }

    lfs_t _lfs;
    struct lfs_config _config;
    
    memset(&_config, 0, sizeof(_config));
    _config.context = bd;
    _config.read  = lfs_bd_read;
    _config.prog  = lfs_bd_prog;
    _config.erase = lfs_bd_erase;
    _config.sync  = lfs_bd_sync;
    _config.read_size   = bd->get_read_size();
    if (_config.read_size < read_size) {
        _config.read_size = read_size;
    }
    _config.prog_size   = bd->get_program_size();
    if (_config.prog_size < prog_size) {
        _config.prog_size = prog_size;
    }
    _config.block_size  = bd->get_erase_size();
    if (_config.block_size < block_size) {
        _config.block_size = block_size;
    }
    _config.block_count = bd->size() / _config.block_size;
    _config.lookahead = 32 * ((_config.block_count+31)/32);
    if (_config.lookahead > lookahead) {
        _config.lookahead = lookahead;
    }

    err = lfs_format(&_lfs, &_config);
    if (err) {
        LFS_INFO("format -> %d", lfs_toerror(err));
        return lfs_toerror(err);
    }

    err = bd->deinit();
    if (err) {
        LFS_INFO("format -> %d", err);
        return err;
    }

    LFS_INFO("format -> %d", 0);
    return 0;
}

int LittleFileSystem::reformat(BlockDevice *bd)
{
    _mutex.lock();
    LFS_INFO("reformat(%p)", bd);
    if (_bd) {
        if (!bd) {
            bd = _bd;
        }

        int err = unmount();
        if (err) {
            LFS_INFO("reformat -> %d", err);
            _mutex.unlock();
            return err;
        }
    }

    if (!bd) {
        LFS_INFO("reformat -> %d", -ENODEV);
        _mutex.unlock();
        return -ENODEV;
    }

    int err = LittleFileSystem::format(bd,
            _read_size, _prog_size, _block_size, _lookahead);
    if (err) {
        LFS_INFO("reformat -> %d", err);
        _mutex.unlock();
        return err;
    }

    err = mount(bd);
    if (err) {
        LFS_INFO("reformat -> %d", err);
        _mutex.unlock();
        return err;
    }

    LFS_INFO("reformat -> %d", 0);
    _mutex.unlock();
    return 0;
}

int LittleFileSystem::remove(const char *filename)
{
    _mutex.lock();
    LFS_INFO("remove(\"%s\")", filename);
    int err = lfs_remove(&_lfs, filename);
    LFS_INFO("remove -> %d", lfs_toerror(err));
    _mutex.unlock();
    return lfs_toerror(err);
}

int LittleFileSystem::rename(const char *oldname, const char *newname)
{
    _mutex.lock();
    LFS_INFO("rename(\"%s\", \"%s\")", oldname, newname);
    int err = lfs_rename(&_lfs, oldname, newname);
    LFS_INFO("rename -> %d", lfs_toerror(err));
    _mutex.unlock();
    return lfs_toerror(err);
}

int LittleFileSystem::mkdir(const char *name, mode_t mode)
{
    _mutex.lock();
    LFS_INFO("mkdir(\"%s\", 0x%lx)", name, mode);
    int err = lfs_mkdir(&_lfs, name);
    LFS_INFO("mkdir -> %d", lfs_toerror(err));
    _mutex.unlock();
    return lfs_toerror(err);
}

int LittleFileSystem::stat(const char *name, struct stat *st)
{
    struct lfs_info info;
    _mutex.lock();
    LFS_INFO("stat(\"%s\", %p)", name, st);
    int err = lfs_stat(&_lfs, name, &info);
    LFS_INFO("stat -> %d", lfs_toerror(err));
    _mutex.unlock();
    st->st_size = info.size;
    st->st_mode = lfs_tomode(info.type);
    return lfs_toerror(err);
}

static int lfs_statvfs_count(void *p, lfs_block_t b)
{
    *(lfs_size_t *)p += 1;
    return 0;
}

int LittleFileSystem::statvfs(const char *name, struct statvfs *st)
{
    memset(st, 0, sizeof(struct statvfs));

    lfs_size_t in_use = 0;
    _mutex.lock();
    LFS_INFO("statvfs(\"%s\", %p)", name, st);
    int err = lfs_traverse(&_lfs, lfs_statvfs_count, &in_use);
    LFS_INFO("statvfs -> %d", lfs_toerror(err));
    _mutex.unlock();
    if (err) {
        return err;
    }

    st->f_bsize  = _config.block_size;
    st->f_frsize = _config.block_size;
    st->f_blocks = _config.block_count;
    st->f_bfree  = _config.block_count - in_use;
    st->f_bavail = _config.block_count - in_use;
    st->f_namemax = LFS_NAME_MAX;
    return 0;
}

////// File operations //////
int LittleFileSystem::file_open(fs_file_t *file, const char *path, int flags)
{
    lfs_file_t *f = new lfs_file_t;
    _mutex.lock();
    LFS_INFO("file_open(%p, \"%s\", 0x%x)", *file, path, flags);
    int err = lfs_file_open(&_lfs, f, path, lfs_fromflags(flags));
    LFS_INFO("file_open -> %d", lfs_toerror(err));
    _mutex.unlock();
    if (!err) {
        *file = f;
    } else {
        delete f;
    }
    return lfs_toerror(err);
}

int LittleFileSystem::file_close(fs_file_t file)
{
    lfs_file_t *f = (lfs_file_t *)file;
    _mutex.lock();
    LFS_INFO("file_close(%p)", file);
    int err = lfs_file_close(&_lfs, f);
    LFS_INFO("file_close -> %d", lfs_toerror(err));
    _mutex.unlock();
    delete f;
    return lfs_toerror(err);
}

ssize_t LittleFileSystem::file_read(fs_file_t file, void *buffer, size_t len)
{
    lfs_file_t *f = (lfs_file_t *)file;
    _mutex.lock();
    LFS_INFO("file_read(%p, %p, %d)", file, buffer, len);
    lfs_ssize_t res = lfs_file_read(&_lfs, f, buffer, len);
    LFS_INFO("file_read -> %d", lfs_toerror(res));
    _mutex.unlock();
    return lfs_toerror(res);
}

ssize_t LittleFileSystem::file_write(fs_file_t file, const void *buffer, size_t len)
{
    lfs_file_t *f = (lfs_file_t *)file;
    _mutex.lock();
    LFS_INFO("file_write(%p, %p, %d)", file, buffer, len);
    lfs_ssize_t res = lfs_file_write(&_lfs, f, buffer, len);
    LFS_INFO("file_write -> %d", lfs_toerror(res));
    _mutex.unlock();
    return lfs_toerror(res);
}

int LittleFileSystem::file_sync(fs_file_t file)
{
    lfs_file_t *f = (lfs_file_t *)file;
    _mutex.lock();
    LFS_INFO("file_sync(%p)", file);
    int err = lfs_file_sync(&_lfs, f);
    LFS_INFO("file_sync -> %d", lfs_toerror(err));
    _mutex.unlock();
    return lfs_toerror(err);
}

off_t LittleFileSystem::file_seek(fs_file_t file, off_t offset, int whence)
{
    lfs_file_t *f = (lfs_file_t *)file;
    _mutex.lock();
    LFS_INFO("file_seek(%p, %ld, %d)", file, offset, whence);
    off_t res = lfs_file_seek(&_lfs, f, offset, lfs_fromwhence(whence));
    LFS_INFO("file_seek -> %d", lfs_toerror(res));
    _mutex.unlock();
    return lfs_toerror(res);
}

off_t LittleFileSystem::file_tell(fs_file_t file)
{
    lfs_file_t *f = (lfs_file_t *)file;
    _mutex.lock();
    LFS_INFO("file_tell(%p)", file);
    off_t res = lfs_file_tell(&_lfs, f);
    LFS_INFO("file_tell -> %d", lfs_toerror(res));
    _mutex.unlock();
    return lfs_toerror(res);
}

off_t LittleFileSystem::file_size(fs_file_t file)
{
    lfs_file_t *f = (lfs_file_t *)file;
    _mutex.lock();
    LFS_INFO("file_size(%p)", file);
    off_t res = lfs_file_size(&_lfs, f);
    LFS_INFO("file_size -> %d", lfs_toerror(res));
    _mutex.unlock();
    return lfs_toerror(res);
}


////// Dir operations //////
int LittleFileSystem::dir_open(fs_dir_t *dir, const char *path)
{
    lfs_dir_t *d = new lfs_dir_t;
    _mutex.lock();
    LFS_INFO("dir_open(%p, \"%s\")", *dir, path);
    int err = lfs_dir_open(&_lfs, d, path);
    LFS_INFO("dir_open -> %d", lfs_toerror(err));
    _mutex.unlock();
    if (!err) {
        *dir = d;
    } else {
        delete d;
    }
    return lfs_toerror(err);
}

int LittleFileSystem::dir_close(fs_dir_t dir)
{
    lfs_dir_t *d = (lfs_dir_t *)dir;
    _mutex.lock();
    LFS_INFO("dir_close(%p)", dir);
    int err = lfs_dir_close(&_lfs, d);
    LFS_INFO("dir_close -> %d", lfs_toerror(err));
    _mutex.unlock();
    delete d;
    return lfs_toerror(err);
}

ssize_t LittleFileSystem::dir_read(fs_dir_t dir, struct dirent *ent)
{
    lfs_dir_t *d = (lfs_dir_t *)dir;
    struct lfs_info info;
    _mutex.lock();
    LFS_INFO("dir_read(%p, %p)", dir, ent);
    int res = lfs_dir_read(&_lfs, d, &info);
    LFS_INFO("dir_read -> %d", lfs_toerror(res));
    _mutex.unlock();
    if (res == 1) {
        ent->d_type = lfs_totype(info.type);
        strcpy(ent->d_name, info.name);
    }
    return lfs_toerror(res);
}

void LittleFileSystem::dir_seek(fs_dir_t dir, off_t offset)
{
    lfs_dir_t *d = (lfs_dir_t *)dir;
    _mutex.lock();
    LFS_INFO("dir_seek(%p, %ld)", dir, offset);
    lfs_dir_seek(&_lfs, d, offset);
    LFS_INFO("dir_seek -> %s", "void");
    _mutex.unlock();
}

off_t LittleFileSystem::dir_tell(fs_dir_t dir)
{
    lfs_dir_t *d = (lfs_dir_t *)dir;
    _mutex.lock();
    LFS_INFO("dir_tell(%p)", dir);
    lfs_soff_t res = lfs_dir_tell(&_lfs, d);
    LFS_INFO("dir_tell -> %d", lfs_toerror(res));
    _mutex.unlock();
    return lfs_toerror(res);
}

void LittleFileSystem::dir_rewind(fs_dir_t dir)
{
    lfs_dir_t *d = (lfs_dir_t *)dir;
    _mutex.lock();
    LFS_INFO("dir_rewind(%p)", dir);
    lfs_dir_rewind(&_lfs, d);
    LFS_INFO("dir_rewind -> %s", "void");
    _mutex.unlock();
}

