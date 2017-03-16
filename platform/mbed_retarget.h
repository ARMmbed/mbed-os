/*
 * mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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
 *
 */

#ifndef RETARGET_H
#define RETARGET_H

#include <stdint.h>
#include <stddef.h>

/* We can get the following standard types from sys/types for gcc, but we
 * need to define the types ourselves for the other compilers that normally
 * target embedded systems */
#if defined(__ARMCC_VERSION) || defined(__ICCARM__)
typedef int ssize_t;    ///< Signed size type, usually encodes negative errors
typedef long off_t;     ///< Offset in a data stream
typedef int mode_t;     ///< Mode for opening files

#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR   2
#define O_CREAT  0x0200
#define O_TRUNC  0x0400
#define O_APPEND 0x0008

#define NAME_MAX 255    ///< Maximum size of a name in a file path

#else
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/syslimits.h>
#endif


/* DIR declarations must also be here */
#if __cplusplus
namespace mbed { class Dir; }
typedef mbed::Dir DIR;
#else
typedef struct Dir DIR;
#endif

#if __cplusplus
extern "C" {
#endif
    DIR *opendir(const char*);
    struct dirent *readdir(DIR *);
    int closedir(DIR*);
    void rewinddir(DIR*);
    long telldir(DIR*);
    void seekdir(DIR*, long);
    int mkdir(const char *name, mode_t n);
#if __cplusplus
};
#endif


#if defined(__ARMCC_VERSION) || defined(__ICCARM__)
/* The intent of this section is to unify the errno error values to match
 * the POSIX definitions for the GCC_ARM, ARMCC and IAR compilers. This is
 * necessary because the ARMCC/IAR errno.h, or sys/stat.h are missing some
 * symbol definitions used by the POSIX filesystem API to return errno codes.
 * Note also that ARMCC errno.h defines some symbol values differently from
 * the GCC_ARM/IAR/standard POSIX definitions. The definitions guard against
 * this and future changes by changing the symbol definition as shown below. */
#ifdef ENOENT
#undef ENOENT
#endif
#define ENOENT      2       /* No such file or directory. */

#ifdef EIO
#undef EIO
#endif
#define EIO         5       /* I/O error */

#ifdef ENXIO
#undef ENXIO
#endif
#define ENXIO       6       /* No such device or address */

#ifdef ENOEXEC
#undef ENOEXEC
#endif
#define ENOEXEC     8       /* Exec format error */

#ifdef EBADF
#undef EBADF
#endif
#define EBADF       9       /* Bad file number */

#ifdef ENOMEM
#undef ENOMEM
#endif
#define ENOMEM      12      /* Not enough space */

#ifdef EACCES
#undef EACCES
#endif
#define EACCES      13      /* Permission denied */

#ifdef EFAULT
#undef EFAULT
#endif
#define EFAULT      14      /* Bad address */

#ifdef EEXIST
#undef EEXIST
#endif
#define EEXIST      17      /* File exists */

#ifdef EINVAL
#undef EINVAL
#endif
#define EINVAL      22      /* Invalid argument */

#ifdef ENFILE
#undef ENFILE
#endif
#define ENFILE      23      /* Too many open files in system */

#ifdef EMFILE
#undef EMFILE
#endif
#define EMFILE      24      /* File descriptor value too large */

#ifdef ENOSYS
#undef ENOSYS
#endif
#define ENOSYS      38      /* Function not implemented */

/* Missing stat.h defines.
 * The following are sys/stat.h definitions not currently present in the ARMCC
 * errno.h. Note, ARMCC errno.h defines some symbol values differing from
 * GCC_ARM/IAR/standard POSIX definitions. Guard against this and future
 * changes by changing the symbol definition for filesystem use. */
#define     _IFDIR  0040000 /* directory */
#define     _IFREG  0100000 /* regular */

#define S_IFDIR     _IFDIR
#define S_IFREG     _IFREG

#define S_IRWXU     (S_IRUSR | S_IWUSR | S_IXUSR)
#define     S_IRUSR 0000400 /* read permission, owner */
#define     S_IWUSR 0000200 /* write permission, owner */
#define     S_IXUSR 0000100/* execute/search permission, owner */
#define S_IRWXG     (S_IRGRP | S_IWGRP | S_IXGRP)
#define     S_IRGRP 0000040 /* read permission, group */
#define     S_IWGRP 0000020 /* write permission, grougroup */
#define     S_IXGRP 0000010/* execute/search permission, group */
#define S_IRWXO     (S_IROTH | S_IWOTH | S_IXOTH)
#define     S_IROTH 0000004 /* read permission, other */
#define     S_IWOTH 0000002 /* write permission, other */
#define     S_IXOTH 0000001/* execute/search permission, other */

#endif /* defined(__ARMCC_VERSION) || defined(__ICCARM__) */


/* The following are dirent.h definitions are declared here to garuntee
 * consistency where structure may be different with different toolchains */
struct dirent {
    char d_name[NAME_MAX+1];
    uint8_t d_type;
};

enum {
    DT_UNKNOWN, // The file type could not be determined.
    DT_FIFO,    // This is a named pipe (FIFO).
    DT_CHR,     // This is a character device.
    DT_DIR,     // This is a directory.
    DT_BLK,     // This is a block device.
    DT_REG,     // This is a regular file.
    DT_LNK,     // This is a symbolic link.
    DT_SOCK,    // This is a UNIX domain socket.
};


#endif /* RETARGET_H */
