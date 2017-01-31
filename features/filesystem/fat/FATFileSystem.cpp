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

#include "ffconf.h"
#include "mbed_debug.h"

#include "FATFileSystem.h"
#include "FATFileHandle.h"
#include "FATDirHandle.h"
//<<<<<<< HEAD
//<<<<<<< HEAD
#include "mbed_critical.h"
//=======
//#include "critical.h"
//=======
//#include "critical.h"
#include "ff.h"
//>>>>>>> Added errno codes to retarget, mkdir() and ftell() tests.
#include <errno.h>
//>>>>>>> Filesystem: Added EEXIST reporting to mkdir through errno

DWORD get_fattime(void) {
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

FATFileSystem *FATFileSystem::_ffs[_VOLUMES] = {0};
static PlatformMutex * mutex = NULL;

PlatformMutex * get_fat_mutex() {
    PlatformMutex * new_mutex = new PlatformMutex;

    core_util_critical_section_enter();
    if (NULL == mutex) {
        mutex = new_mutex;
    }
    core_util_critical_section_exit();

    if (mutex != new_mutex) {
        delete new_mutex;
    }
    return mutex;
}

/* @brief   Set errno based on the error code returned from underlying filesystem
 *
 * @param   res     result returned from underlying filesystem
 *
 * @return  No return value
 */
static void FATFileSystemSetErrno(FRESULT res)
{
    /* todo: remove this temporary fix to overcome undefined symbols when compile for ARMCC */
#ifndef TOOLCHAIN_ARM_STD
    switch(res) {
        case FR_DISK_ERR:               /* (1) A hard error occurred in the low level disk I/O layer */
        case FR_NOT_READY:              /* (3) The physical drive cannot work */
            errno = EIO;                /* I/O error */
            break;
        case FR_NO_FILE:                /* (4) Could not find the file */
        case FR_NO_PATH:                /* (5) Could not find the path */
        case FR_INVALID_NAME:           /* (6) The path name format is invalid */
        case FR_INVALID_DRIVE:          /* (11) The logical drive number is invalid */
        case FR_NO_FILESYSTEM:          /* (13) There is no valid FAT volume */
            errno = ENOENT;             /* No such file or directory */
            break;
        case FR_DENIED:                 /* (7) Access denied due to prohibited access or directory full */
        case FR_EXIST:                  /* (8) Access denied due to prohibited access */
        case FR_WRITE_PROTECTED:        /* (10) The physical drive is write protected */
        case FR_LOCKED:                 /* (16) The operation is rejected according to the file sharing policy */
            errno = EACCES;             /* Permission denied */
            break;
        case FR_INVALID_OBJECT:         /* (9) The file/directory object is invalid */
            errno = EFAULT;             /* Bad address */
            break;
        case FR_NOT_ENABLED:            /* (12) The volume has no work area */
            errno = ENXIO;              /* No such device or address */
            break;
        case FR_NOT_ENOUGH_CORE:        /* (17) LFN working buffer could not be allocated */
            errno = ENOMEM;             /* Not enough space */
            break;
        case FR_TOO_MANY_OPEN_FILES:    /* (18) Number of open files > _FS_LOCK */
            errno = ENFILE;             /* Too many open files in system */
            break;
        case FR_INT_ERR:                /* (2) Assertion failed */
        case FR_TIMEOUT:                /* (15) Could not get a grant to access the volume within defined period */
        default:
            errno = EBADF;              /* Bad file number */
            break;
    }
#endif  /* TOOLCHAIN_ARM_STD */
    return;
}

FATFileSystem::FATFileSystem(const char* n) : FileSystemLike(n), _mutex(get_fat_mutex()) {
    lock();
    debug_if(FFS_DBG, "FATFileSystem(%s)\n", n);
    for(int i=0; i<_VOLUMES; i++) {
        if(_ffs[i] == 0) {
            _ffs[i] = this;
            _fsid[0] = '0' + i;
            _fsid[1] = '\0';
            debug_if(FFS_DBG, "Mounting [%s] on ffs drive [%s]\n", getName(), _fsid);
            f_mount(&_fs, _fsid, 0);
            unlock();
            return;
        }
    }
    error("Couldn't create %s in FATFileSystem::FATFileSystem\n", n);
    unlock();
}

FATFileSystem::~FATFileSystem() {
    lock();
    for (int i=0; i<_VOLUMES; i++) {
        if (_ffs[i] == this) {
            _ffs[i] = 0;
            f_mount(NULL, _fsid, 0);
        }
    }
    unlock();
}

FileHandle *FATFileSystem::open(const char* name, int flags) {
    lock();
    debug_if(FFS_DBG, "open(%s) on filesystem [%s], drv [%s]\n", name, getName(), _fsid);
    char n[64];
    sprintf(n, "%s:/%s", _fsid, name);

    /* POSIX flags -> FatFS open mode */
    BYTE openmode;
    if (flags & O_RDWR) {
        openmode = FA_READ|FA_WRITE;
    } else if(flags & O_WRONLY) {
        openmode = FA_WRITE;
    } else {
        openmode = FA_READ;
    }
    if(flags & O_CREAT) {
        if(flags & O_TRUNC) {
            openmode |= FA_CREATE_ALWAYS;
        } else {
            openmode |= FA_OPEN_ALWAYS;
        }
    }

    FIL fh;
    FRESULT res = f_open(&fh, n, openmode);
    if (res) {
        debug_if(FFS_DBG, "f_open('w') failed: %d\n", res);
        unlock();
        FATFileSystemSetErrno(res);
        return NULL;
    }
    if (flags & O_APPEND) {
        f_lseek(&fh, fh.fsize);
    }
    FATFileHandle * handle = new FATFileHandle(fh, _mutex);
    unlock();
    return handle;
}

int FATFileSystem::remove(const char *filename) {
    lock();
    FRESULT res = f_unlink(filename);
    if (res) {
        debug_if(FFS_DBG, "f_unlink() failed: %d\n", res);
        unlock();
        return -1;
    }
    unlock();
    return 0;
}

int FATFileSystem::rename(const char *oldname, const char *newname) {
    lock();
    FRESULT res = f_rename(oldname, newname);
    if (res) {
        debug_if(FFS_DBG, "f_rename() failed: %d\n", res);
        unlock();
        return -1;
    }
    unlock();
    return 0;
}

int FATFileSystem::format() {
    lock();
    FRESULT res = f_mkfs(_fsid, 0, 512); // Logical drive number, Partitioning rule, Allocation unit size (bytes per cluster)
    if (res) {
        debug_if(FFS_DBG, "f_mkfs() failed: %d\n", res);
        unlock();
        return -1;
    }
    unlock();
    return 0;
}

DirHandle *FATFileSystem::opendir(const char *name) {
    lock();
    FATFS_DIR dir;
    FRESULT res = f_opendir(&dir, name);
    if (res != 0) {
        unlock();
        return NULL;
    }
    FATDirHandle *handle = new FATDirHandle(dir, _mutex);
    unlock();
    return handle;
}

int FATFileSystem::mkdir(const char *name, mode_t mode) {
    lock();
    FRESULT res = f_mkdir(name);
    /* todo: remove this temporary fix to overcome undefined symbols when compile for ARMCC */
#ifndef TOOLCHAIN_ARM_STD
    if (res != 0) {
        errno = (res == FR_EXIST) ? EEXIST : 0;
    }
#endif  /* TOOLCHAIN_ARM_STD */
    unlock();
    return res == 0 ? 0 : -1;
}

int FATFileSystem::stat(const char *name, struct stat *st) {
    lock();
    FILINFO f;
    memset(&f, 0, sizeof(f));

    FRESULT res = f_stat(name, &f);
    if (res != 0) {
        unlock();
        return -1;
    }

    /* todo: remove this temporary fix to overcome undefined symbols when compile for ARMCC */
#ifndef TOOLCHAIN_ARM_STD
    st->st_size = f.fsize;
    st->st_mode = 0;
    st->st_mode |= (f.fattrib & AM_DIR) ? S_IFDIR : S_IFREG;
    st->st_mode |= (f.fattrib & AM_RDO) ?
        (S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) :
        (S_IRWXU | S_IRWXG | S_IRWXO);
#endif
    unlock();
    return 0;
}

int FATFileSystem::mount() {
    lock();
    FRESULT res = f_mount(&_fs, _fsid, 1);
    unlock();
    return res == 0 ? 0 : -1;
}

int FATFileSystem::unmount() {
    lock();
    if (disk_sync()) {
        unlock();
        return -1;
    }
    FRESULT res = f_mount(NULL, _fsid, 0);
    unlock();
    return res == 0 ? 0 : -1;
}

void FATFileSystem::lock() {
    _mutex->lock();
}

void FATFileSystem::unlock() {
    _mutex->unlock();
}
