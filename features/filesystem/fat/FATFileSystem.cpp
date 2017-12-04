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
#include "mbed.h"

#include "diskio.h"
#include "ffconf.h"
#include "mbed_debug.h"
#include "mbed_critical.h"
#include <errno.h>

#include "FATFileSystem.h"


////// Error handling /////

static int fat_error_remap(FRESULT res)
{
    switch(res) {
        case FR_OK:                     /* (0) Succeeded */
            return 0;                   /* no error */
        case FR_DISK_ERR:               /* (1) A hard error occurred in the low level disk I/O layer */
        case FR_NOT_READY:              /* (3) The physical drive cannot work */
            return -EIO;                /* I/O error */
        case FR_NO_FILE:                /* (4) Could not find the file */
        case FR_NO_PATH:                /* (5) Could not find the path */
        case FR_INVALID_NAME:           /* (6) The path name format is invalid */
        case FR_INVALID_DRIVE:          /* (11) The logical drive number is invalid */
        case FR_NO_FILESYSTEM:          /* (13) There is no valid FAT volume */
            return -ENOENT;             /* No such file or directory */
        case FR_DENIED:                 /* (7) Access denied due to prohibited access or directory full */
            return -EACCES;             /* Permission denied */
        case FR_EXIST:                  /* (8) Access denied due to prohibited access */
            return -EEXIST;             /* File exists */
        case FR_WRITE_PROTECTED:        /* (10) The physical drive is write protected */
        case FR_LOCKED:                 /* (16) The operation is rejected according to the file sharing policy */
            return -EACCES;             /* Permission denied */
        case FR_INVALID_OBJECT:         /* (9) The file/directory object is invalid */
            return -EFAULT;             /* Bad address */
        case FR_NOT_ENABLED:            /* (12) The volume has no work area */
            return -ENXIO;              /* No such device or address */
        case FR_NOT_ENOUGH_CORE:        /* (17) LFN working buffer could not be allocated */
            return -ENOMEM;             /* Not enough space */
        case FR_TOO_MANY_OPEN_FILES:    /* (18) Number of open files > _FS_LOCK */
            return -ENFILE;             /* Too many open files in system */
        case FR_INVALID_PARAMETER:      /* (19) Given parameter is invalid */
            return -ENOEXEC;            /* Exec format error */
        case FR_INT_ERR:                /* (2) Assertion failed */
        case FR_MKFS_ABORTED:           /* (14) The f_mkfs() aborted due to any parameter error */
        case FR_TIMEOUT:                /* (15) Could not get a grant to access the volume within defined period */
        default:                        /* Bad file number */
            return -EBADF;
    }
}

// Helper class for deferring operations when variable falls out of scope
template <typename T>
class Deferred {
public:
    T _t;
    Callback<void(T)> _ondefer;

    Deferred(const Deferred&);
    Deferred &operator=(const Deferred&);

public:
    Deferred(T t, Callback<void(T)> ondefer = NULL)
        : _t(t), _ondefer(ondefer)
    {
    }

    operator T()
    {
        return _t;
    }

    ~Deferred()
    {
        if (_ondefer) {
            _ondefer(_t);
        }
    }
};

static void dodelete(const char *data)
{
    delete[] data;
}

// Adds prefix needed internally by fatfs, this can be avoided for the first fatfs
// (id 0) otherwise a prefix of "id:/" is inserted in front of the string.
static Deferred<const char*> fat_path_prefix(int id, const char *path)
{
    // We can avoid dynamic allocation when only on fatfs is in use
    if (id == 0) {
        return path;
    }

    // Prefix path with id, will look something like 2:/hi/hello/filehere.txt
    char *buffer = new char[strlen("0:/") + strlen(path) + 1];
    if (!buffer) {
        return NULL;
    }

    buffer[0] = '0' + id;
    buffer[1] = ':';
    buffer[2] = '/';
    strcpy(buffer + strlen("0:/"), path);
    return Deferred<const char*>(buffer, dodelete);
}


////// Disk operations //////

// Global access to block device from FAT driver
static BlockDevice *_ffs[_VOLUMES] = {0};
static SingletonPtr<PlatformMutex> _ffs_mutex;


// FAT driver functions
DWORD get_fattime(void)
{
    time_t rawtime;
    time(&rawtime);
    struct tm *ptm = localtime(&rawtime);
    return (DWORD)(ptm->tm_year - 80) << 25
           | (DWORD)(ptm->tm_mon + 1  ) << 21
           | (DWORD)(ptm->tm_mday     ) << 16
           | (DWORD)(ptm->tm_hour     ) << 11
           | (DWORD)(ptm->tm_min      ) << 5
           | (DWORD)(ptm->tm_sec/2    );
}

void *ff_memalloc(UINT size)
{
    return malloc(size);
}

void ff_memfree(void *p)
{
    free(p);
}

// Implementation of diskio functions (see ChaN/diskio.h)
static WORD disk_get_sector_size(BYTE pdrv)
{
    WORD ssize = _ffs[pdrv]->get_erase_size();
    if (ssize < 512) {
        ssize = 512;
    }

    MBED_ASSERT(ssize >= _MIN_SS && ssize <= _MAX_SS);
    MBED_ASSERT(_ffs[pdrv]->get_read_size() <= _ffs[pdrv]->get_erase_size());
    MBED_ASSERT(_ffs[pdrv]->get_program_size() <= _ffs[pdrv]->get_erase_size());
    return ssize;
}

static DWORD disk_get_sector_count(BYTE pdrv)
{
    DWORD scount = _ffs[pdrv]->size() / disk_get_sector_size(pdrv);
    MBED_ASSERT(scount >= 64);
    return scount;
}

DSTATUS disk_status(BYTE pdrv)
{
    debug_if(FFS_DBG, "disk_status on pdrv [%d]\n", pdrv);
    return RES_OK;
}

DSTATUS disk_initialize(BYTE pdrv)
{
    debug_if(FFS_DBG, "disk_initialize on pdrv [%d]\n", pdrv);
    return (DSTATUS)_ffs[pdrv]->init();
}

DRESULT disk_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
    debug_if(FFS_DBG, "disk_read(sector %d, count %d) on pdrv [%d]\n", sector, count, pdrv);
    DWORD ssize = disk_get_sector_size(pdrv);
    int err = _ffs[pdrv]->read(buff, sector*ssize, count*ssize);
    return err ? RES_PARERR : RES_OK;
}

DRESULT disk_write(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
{
    debug_if(FFS_DBG, "disk_write(sector %d, count %d) on pdrv [%d]\n", sector, count, pdrv);
    DWORD ssize = disk_get_sector_size(pdrv);
    int err = _ffs[pdrv]->erase(sector*ssize, count*ssize);
    if (err) {
        return RES_PARERR;
    }

    err = _ffs[pdrv]->program(buff, sector*ssize, count*ssize);
    if (err) {
        return RES_PARERR;
    }

    return RES_OK;
}

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
    debug_if(FFS_DBG, "disk_ioctl(%d)\n", cmd);
    switch (cmd) {
        case CTRL_SYNC:
            if (_ffs[pdrv] == NULL) {
                return RES_NOTRDY;
            } else {
                return RES_OK;
            }
        case GET_SECTOR_COUNT:
            if (_ffs[pdrv] == NULL) {
                return RES_NOTRDY;
            } else {
                *((DWORD*)buff) = disk_get_sector_count(pdrv);
                return RES_OK;
            }
        case GET_SECTOR_SIZE:
            if (_ffs[pdrv] == NULL) {
                return RES_NOTRDY;
            } else {
                *((WORD*)buff) = disk_get_sector_size(pdrv);
                return RES_OK;
            }
        case GET_BLOCK_SIZE:
            *((DWORD*)buff) = 1; // default when not known
            return RES_OK;
        case CTRL_TRIM:
            if (_ffs[pdrv] == NULL) {
                return RES_NOTRDY;
            } else {
                DWORD *sectors = (DWORD*)buff;
                DWORD ssize = disk_get_sector_size(pdrv);
                int err = _ffs[pdrv]->trim(sectors[0]*ssize, (sectors[1]-sectors[0]+1)*ssize);
                return err ? RES_PARERR : RES_OK;
            }
    }

    return RES_PARERR;
}


////// Generic filesystem operations //////

// Filesystem implementation (See FATFilySystem.h)
FATFileSystem::FATFileSystem(const char *name, BlockDevice *bd)
        : FileSystem(name), _id(-1) {
    if (bd) {
        mount(bd);
    }
}

FATFileSystem::~FATFileSystem()
{
    // nop if unmounted
    unmount();
}

int FATFileSystem::mount(BlockDevice *bd) {
    // requires duplicate definition to allow virtual overload to work
    return mount(bd, true);
}

int FATFileSystem::mount(BlockDevice *bd, bool mount) {
    lock();
    if (_id != -1) {
        unlock();
        return -EINVAL;
    }

    for (int i = 0; i < _VOLUMES; i++) {
        if (!_ffs[i]) {
            _id = i;
            _ffs[_id] = bd;
            _fsid[0] = '0' + _id;
            _fsid[1] = ':';
            _fsid[2] = '\0';
            debug_if(FFS_DBG, "Mounting [%s] on ffs drive [%s]\n", getName(), _fsid);
            FRESULT res = f_mount(&_fs, _fsid, mount);
            unlock();
            return fat_error_remap(res);
        }
    }

    unlock();
    return -ENOMEM;
}

int FATFileSystem::unmount()
{
    lock();
    if (_id == -1) {
        unlock();
        return -EINVAL;
    }

    FRESULT res = f_mount(NULL, _fsid, 0);
    _ffs[_id] = NULL;
    _id = -1;
    unlock();
    return fat_error_remap(res);
}

/* See http://elm-chan.org/fsw/ff/en/mkfs.html for details of f_mkfs() and
 * associated arguments. */
int FATFileSystem::format(BlockDevice *bd, bd_size_t cluster_size) {
    FATFileSystem fs;
    int err = fs.mount(bd, false);
    if (err) {
        return err;
    }

    // Logical drive number, Partitioning rule, Allocation unit size (bytes per cluster)
    fs.lock();
    FRESULT res = f_mkfs(fs._fsid, 1, cluster_size);
    fs.unlock();
    if (res != FR_OK) {
        return fat_error_remap(res);
    }

    err = fs.unmount();
    if (err) {
        return err;
    }

    return 0;
}

int FATFileSystem::reformat(BlockDevice *bd, int allocation_unit) {
    lock();
    if (_id != -1) {
        if (!bd) {
            bd = _ffs[_id];
        }

        int err = unmount();
        if (err) {
            unlock();
            return err;
        }
    }

    if (!bd) {
        unlock();
        return -ENODEV;
    }

    int err = FATFileSystem::format(bd, allocation_unit);
    if (err) {
        unlock();
        return err;
    }

    err = mount(bd);
    unlock();
    return err;
}

int FATFileSystem::remove(const char *path) {
    Deferred<const char*> fpath = fat_path_prefix(_id, path);

    lock();
    FRESULT res = f_unlink(fpath);
    unlock();

    if (res != FR_OK) {
        debug_if(FFS_DBG, "f_unlink() failed: %d\n", res);
    }
    return fat_error_remap(res);
}

int FATFileSystem::rename(const char *oldpath, const char *newpath) {
    Deferred<const char*> oldfpath = fat_path_prefix(_id, oldpath);
    Deferred<const char*> newfpath = fat_path_prefix(_id, newpath);

    lock();
    FRESULT res = f_rename(oldfpath, newfpath);
    unlock();

    if (res != FR_OK) {
        debug_if(FFS_DBG, "f_rename() failed: %d\n", res);
    }
    return fat_error_remap(res);
}

int FATFileSystem::mkdir(const char *path, mode_t mode) {
    Deferred<const char*> fpath = fat_path_prefix(_id, path);

    lock();
    FRESULT res = f_mkdir(fpath);
    unlock();

    if (res != FR_OK) {
        debug_if(FFS_DBG, "f_mkdir() failed: %d\n", res);
    }
    return fat_error_remap(res);
}

int FATFileSystem::stat(const char *path, struct stat *st) {
    Deferred<const char*> fpath = fat_path_prefix(_id, path);

    lock();
    FILINFO f;
    memset(&f, 0, sizeof(f));

    FRESULT res = f_stat(fpath, &f);
    if (res != FR_OK) {
        unlock();
        return fat_error_remap(res);
    }

    /* ARMCC doesnt support stat(), and these symbols are not defined by the toolchain. */
#ifdef TOOLCHAIN_GCC
    st->st_size = f.fsize;
    st->st_mode = 0;
    st->st_mode |= (f.fattrib & AM_DIR) ? S_IFDIR : S_IFREG;
    st->st_mode |= (f.fattrib & AM_RDO) ?
        (S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) :
        (S_IRWXU | S_IRWXG | S_IRWXO);
#endif /* TOOLCHAIN_GCC */
    unlock();

    return 0;
}

void FATFileSystem::lock() {
    _ffs_mutex->lock();
}

void FATFileSystem::unlock() {
    _ffs_mutex->unlock();
}


////// File operations //////
int FATFileSystem::file_open(fs_file_t *file, const char *path, int flags) {
    debug_if(FFS_DBG, "open(%s) on filesystem [%s], drv [%s]\n", path, getName(), _id);

    FIL *fh = new FIL;
    Deferred<const char*> fpath = fat_path_prefix(_id, path);

    /* POSIX flags -> FatFS open mode */
    BYTE openmode;
    if (flags & O_RDWR) {
        openmode = FA_READ | FA_WRITE;
    } else if (flags & O_WRONLY) {
        openmode = FA_WRITE;
    } else {
        openmode = FA_READ;
    }
    if (flags & O_CREAT) {
        if (flags & O_TRUNC) {
            openmode |= FA_CREATE_ALWAYS;
        } else {
            openmode |= FA_OPEN_ALWAYS;
        }
    }

    lock();
    FRESULT res = f_open(fh, fpath, openmode);

    if (res != FR_OK) {
        unlock();
        debug_if(FFS_DBG, "f_open('w') failed: %d\n", res);
        delete fh;
        return fat_error_remap(res);
    }

    if (flags & O_APPEND) {
        f_lseek(fh, fh->fsize);
    }
    unlock();

    *file = fh;
    return 0;
}

int FATFileSystem::file_close(fs_file_t file) {
    FIL *fh = static_cast<FIL*>(file);

    lock();
    FRESULT res = f_close(fh);
    unlock();

    delete fh;
    return fat_error_remap(res);
}

ssize_t FATFileSystem::file_read(fs_file_t file, void *buffer, size_t len) {
    FIL *fh = static_cast<FIL*>(file);

    lock();
    UINT n;
    FRESULT res = f_read(fh, buffer, len, &n);
    unlock();

    if (res != FR_OK) {
        debug_if(FFS_DBG, "f_read() failed: %d\n", res);
        return fat_error_remap(res);
    } else {
        return n;
    }
}

ssize_t FATFileSystem::file_write(fs_file_t file, const void *buffer, size_t len) {
    FIL *fh = static_cast<FIL*>(file);

    lock();
    UINT n;
    FRESULT res = f_write(fh, buffer, len, &n);
    unlock();

    if (res != FR_OK) {
        debug_if(FFS_DBG, "f_write() failed: %d", res);
        return fat_error_remap(res);
    } else {
        return n;
    }
}

int FATFileSystem::file_sync(fs_file_t file) {
    FIL *fh = static_cast<FIL*>(file);

    lock();
    FRESULT res = f_sync(fh);
    unlock();

    if (res != FR_OK) {
        debug_if(FFS_DBG, "f_sync() failed: %d\n", res);
    }
    return fat_error_remap(res);
}

off_t FATFileSystem::file_seek(fs_file_t file, off_t offset, int whence) {
    FIL *fh = static_cast<FIL*>(file);

    lock();
    if (whence == SEEK_END) {
        offset += fh->fsize;
    } else if(whence==SEEK_CUR) {
        offset += fh->fptr;
    }

    FRESULT res = f_lseek(fh, offset);
    off_t noffset = fh->fptr;
    unlock();

    if (res != FR_OK) {
        debug_if(FFS_DBG, "lseek failed: %d\n", res);
        return fat_error_remap(res);
    } else {
        return noffset;
    }
}

off_t FATFileSystem::file_tell(fs_file_t file) {
    FIL *fh = static_cast<FIL*>(file);

    lock();
    off_t res = fh->fptr;
    unlock();

    return res;
}

off_t FATFileSystem::file_size(fs_file_t file) {
    FIL *fh = static_cast<FIL*>(file);

    lock();
    off_t res = fh->fsize;
    unlock();

    return res;
}


////// Dir operations //////
int FATFileSystem::dir_open(fs_dir_t *dir, const char *path) {
    FATFS_DIR *dh = new FATFS_DIR;
    Deferred<const char*> fpath = fat_path_prefix(_id, path);

    lock();
    FRESULT res = f_opendir(dh, fpath);
    unlock();

    if (res != FR_OK) {
        debug_if(FFS_DBG, "f_opendir() failed: %d\n", res);
        delete dh;
        return fat_error_remap(res);
    }

    *dir = dh;
    return 0;
}

int FATFileSystem::dir_close(fs_dir_t dir) {
    FATFS_DIR *dh = static_cast<FATFS_DIR*>(dir);

    lock();
    FRESULT res = f_closedir(dh);
    unlock();

    delete dh;
    return fat_error_remap(res);
}

ssize_t FATFileSystem::dir_read(fs_dir_t dir, struct dirent *ent) {
    FATFS_DIR *dh = static_cast<FATFS_DIR*>(dir);
    FILINFO finfo;

#if _USE_LFN
    finfo.lfname = ent->d_name;
    finfo.lfsize = NAME_MAX;
#endif // _USE_LFN

    lock();
    FRESULT res = f_readdir(dh, &finfo);
    unlock();

    if (res != FR_OK) {
        return fat_error_remap(res);
    } else if (finfo.fname[0] == 0) {
        return 0;
    }

    ent->d_type = (finfo.fattrib & AM_DIR) ? DT_DIR : DT_REG;

#if _USE_LFN
    if (ent->d_name[0] == 0) {
        // No long filename so use short filename.
        strncpy(ent->d_name, finfo.fname, NAME_MAX);
    }
#else
    strncpy(end->d_name, finfo.fname, len);
#endif

    return 1;
}

void FATFileSystem::dir_seek(fs_dir_t dir, off_t offset) {
    FATFS_DIR *dh = static_cast<FATFS_DIR*>(dir);

    lock();
    if (offset < dh->index) {
        f_rewinddir(dh);
    }
    while (dh->index < offset) {
        FILINFO finfo;
        FRESULT res;
#if _USE_LFN
        char lfname[NAME_MAX];
        finfo.lfname = lfname;
        finfo.lfsize = NAME_MAX;
#endif // _USE_LFN
        res = f_readdir(dh, &finfo);
        if (res != FR_OK) {
            break;
        } else if (finfo.fname[0] == 0) {
            break;
        }
    }
    unlock();
}

off_t FATFileSystem::dir_tell(fs_dir_t dir) {
    FATFS_DIR *dh = static_cast<FATFS_DIR*>(dir);

    lock();
    off_t offset = dh->index;
    unlock();

    return offset;
}

void FATFileSystem::dir_rewind(fs_dir_t dir) {
    FATFS_DIR *dh = static_cast<FATFS_DIR*>(dir);

    lock();
    f_rewinddir(dh);
    unlock();
}

