/*
 * mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
 *
 */

#ifndef RETARGET_H
#define RETARGET_H

#if __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif //__cplusplus
#include <stdint.h>
#include <stddef.h>

/* Include logic for errno so we can get errno defined but not bring in error_t,
 * including errno here prevents an include later, which would redefine our
 * error codes
 */
#ifndef __error_t_defined
#define __error_t_defined 1
#endif
#include <errno.h>

#if !defined(__ARMCC_VERSION) && defined(__GNUC__)
#include <fcntl.h>
#endif

#if defined __has_include
#  if __has_include (<sys/stat.h>)
#    include <sys/stat.h>
#    define HAVE_SYS_STAT_H
#  endif
#endif

/* We can get the following standard types from sys/types for gcc, but we
 * need to define the types ourselves for the other compilers that normally
 * target embedded systems */
typedef signed   int  ssize_t;  ///< Signed size type, usually encodes negative errors
typedef signed   long off_t;    ///< Offset in a data stream
typedef unsigned int  nfds_t;   ///< Number of file descriptors
typedef unsigned long long fsblkcnt_t;  ///< Count of file system blocks
#if defined(__ARMCC_VERSION) || !defined(__GNUC__)
typedef unsigned int  mode_t;   ///< Mode for opening files
typedef unsigned int  dev_t;    ///< Device ID type
typedef unsigned long ino_t;    ///< File serial number
typedef unsigned int  nlink_t;  ///< Number of links to a file
typedef unsigned int  uid_t;    ///< User ID
typedef unsigned int  gid_t;    ///< Group ID
#endif

/* Flags for open() and fcntl(GETFL/SETFL)
 * At present, fcntl only supports reading and writing O_NONBLOCK
 */
#ifndef O_RDONLY
#define O_RDONLY 0        ///< Open for reading
#endif
#ifndef O_WRONLY
#define O_WRONLY 1        ///< Open for writing
#endif
#ifndef O_RDWR
#define O_RDWR   2        ///< Open for reading and writing
#endif
#ifndef O_NONBLOCK
#define O_NONBLOCK 0x0004 ///< Non-blocking mode
#endif
#ifndef O_APPEND
#define O_APPEND   0x0008 ///< Set file offset to end of file prior to each write
#endif
#ifndef O_CREAT
#define O_CREAT    0x0200 ///< Create file if it does not exist
#endif
#ifndef O_TRUNC
#define O_TRUNC    0x0400 ///< Truncate file to zero length
#endif
#ifndef O_EXCL
#define O_EXCL     0x0800 ///< Fail if file exists
#endif
#ifndef O_BINARY
#define O_BINARY   0x8000 ///< Open file in binary mode
#endif
#ifndef O_ACCMODE
#define O_ACCMODE   (O_RDONLY|O_WRONLY|O_RDWR)
#endif

#define NAME_MAX 255    ///< Maximum size of a name in a file path

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#include <time.h>

/** \addtogroup platform-public-api */
/** @{*/

#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
/**
 * \defgroup platform_retarget Retarget functions
 * @{
 */

/* DIR declarations must also be here */
#if __cplusplus
namespace mbed {
class FileHandle;
class DirHandle;

/** Targets may implement this to change stdin, stdout, stderr.
 *
 * If the application hasn't provided mbed_override_console, this is called
 * to give the target a chance to specify a FileHandle for the console.
 *
 * If this is not provided or returns NULL, the console will be:
 *   - BufferedSerial if configuration option "platform.stdio-buffered-serial" is
 *     true and the target has DEVICE_SERIAL;
 *   - Raw HAL serial via serial_getc and serial_putc if
 *     "platform.stdio-buffered-serial" is false and the target has DEVICE_SERIAL;
 *   - stdout/stderr will be a sink and stdin will input a stream of 0s if the
 *     target does not have DEVICE_SERIAL.
 *
 * @param fd file descriptor - STDIN_FILENO, STDOUT_FILENO or STDERR_FILENO
 * @return  pointer to FileHandle to override normal stream otherwise NULL
 */
FileHandle *mbed_target_override_console(int fd);

/** Applications may implement this to change stdin, stdout, stderr.
 *
 * This hook gives the application a chance to specify a custom FileHandle
 * for the console.
 *
 * If this is not provided or returns NULL, the console will be specified
 * by mbed_target_override_console, else will default to serial - see
 * mbed_target_override_console for more details.
 *
 * Example using BufferedSerial:
 * @code
 * FileHandle *mbed::mbed_override_console(int) {
 *     static BufferedSerial my_serial(D0, D1);
 *     return &my_serial;
 * }
 * @endcode
 *
 * Example using SingleWireOutput:
 * @code
 * FileHandle *mbed::mbed_override_console(int) {
 *     static SerialWireOutput swo;
 *     return &swo;
 * }
 * @endcode
 *
 * Example using arm semihosting:
 * @code
 * FileHandle *mbed::mbed_override_console(int fileno) {
 *    static LocalFileSystem fs("host");
 *    if (fileno == STDIN_FILENO) {
 *        static FileHandle *in_terminal;
 *        static int in_open_result = fs.open(&in_terminal, ":tt", O_RDONLY);
 *        return in_terminal;
 *    } else {
 *        static FileHandle *out_terminal;
 *        static int out_open_result = fs.open(&out_terminal, ":tt", O_WRONLY);
 *        return out_terminal;
 *    }
 * }
 * @endcode
 *
 * @param fd file descriptor - STDIN_FILENO, STDOUT_FILENO or STDERR_FILENO
 * @return  pointer to FileHandle to override normal stream otherwise NULL
 */
FileHandle *mbed_override_console(int fd);

/** Look up the Mbed file handle corresponding to a file descriptor
 *
 * This conversion function permits an application to find the underlying
 * FileHandle object corresponding to a POSIX file descriptor.
 *
 * This allows access to specialized behavior only available via the
 * FileHandle API.
 *
 * Example of saving power by disabling console input - for buffered serial,
 * this would release the RX interrupt handler, which would release the
 * deep sleep lock.
 * @code
 * mbed_file_handle(STDIN_FILENO)->enable_input(false);
 * @endcode
 *
 * @param fd file descriptor
 * @return   FileHandle pointer
 *           NULL if descriptor does not correspond to a FileHandle (only
 *           possible if it's not open with current implementation).
 */
FileHandle *mbed_file_handle(int fd);
}
#endif
typedef struct DIR_impl DIR;
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY

/* The intent of this section is to unify the errno error values to match
 * the POSIX definitions for the GCC_ARM, ARMCC and IAR compilers. This is
 * necessary because the ARMCC/IAR errno.h, or sys/stat.h are missing some
 * symbol definitions used by the POSIX filesystem API to return errno codes.
 * Note also that ARMCC errno.h defines some symbol values differently from
 * the GCC_ARM/IAR/standard POSIX definitions. The definitions guard against
 * this and future changes by changing the symbol definition as shown below.
 */
#ifndef  EPERM
#define EPERM           1       /* Operation not permitted */
#endif
#ifndef  ENOENT
#define ENOENT          2       /* No such file or directory */
#endif
#ifndef  ESRCH
#define ESRCH           3       /* No such process */
#endif
#ifndef  EINTR
#define EINTR           4       /* Interrupted system call */
#endif
#ifndef  EIO
#define EIO             5       /* I/O error */
#endif
#ifndef  ENXIO
#define ENXIO           6       /* No such device or address */
#endif
#ifndef  E2BIG
#define E2BIG           7       /* Argument list too long */
#endif
#ifndef  ENOEXEC
#define ENOEXEC         8       /* Exec format error */
#endif
#ifndef  EBADF
#define EBADF           9       /* Bad file number */
#endif
#ifndef  ECHILD
#define ECHILD          10      /* No child processes */
#endif
#ifndef  EAGAIN
#define EAGAIN          11      /* Try again */
#endif
#ifndef  ENOMEM
#define ENOMEM          12      /* Out of memory */
#endif
#ifndef  EACCES
#define EACCES          13      /* Permission denied */
#endif
#ifndef  EFAULT
#define EFAULT          14      /* Bad address */
#endif
#ifndef  ENOTBLK
#define ENOTBLK         15      /* Block device required */
#endif
#ifndef  EBUSY
#define EBUSY           16      /* Device or resource busy */
#endif
#ifndef  EEXIST
#define EEXIST          17      /* File exists */
#endif
#ifndef  EXDEV
#define EXDEV           18      /* Cross-device link */
#endif
#ifndef  ENODEV
#define ENODEV          19      /* No such device */
#endif
#ifndef  ENOTDIR
#define ENOTDIR         20      /* Not a directory */
#endif
#ifndef  EISDIR
#define EISDIR          21      /* Is a directory */
#endif
#ifndef  EINVAL
#define EINVAL          22      /* Invalid argument */
#endif
#ifndef  ENFILE
#define ENFILE          23      /* File table overflow */
#endif
#ifndef  EMFILE
#define EMFILE          24      /* Too many open files */
#endif
#ifndef  ENOTTY
#define ENOTTY          25      /* Not a typewriter */
#endif
#ifndef  ETXTBSY
#define ETXTBSY         26      /* Text file busy */
#endif
#ifndef  EFBIG
#define EFBIG           27      /* File too large */
#endif
#ifndef  ENOSPC
#define ENOSPC          28      /* No space left on device */
#endif
#ifndef  ESPIPE
#define ESPIPE          29      /* Illegal seek */
#endif
#ifndef  EROFS
#define EROFS           30      /* Read-only file system */
#endif
#ifndef  EMLINK
#define EMLINK          31      /* Too many links */
#endif
#ifndef  EPIPE
#define EPIPE           32      /* Broken pipe */
#endif
#ifndef  EDOM
#define EDOM            33      /* Math argument out of domain of func */
#endif
#ifndef  ERANGE
#define ERANGE          34      /* Math result not representable */
#endif
#ifndef  EDEADLK
#define EDEADLK         35      /* Resource deadlock would occur */
#endif
#ifndef  ENAMETOOLONG
#define ENAMETOOLONG    36      /* File name too long */
#endif
#ifndef  ENOLCK
#define ENOLCK          37      /* No record locks available */
#endif
#ifndef  ENOSYS
#define ENOSYS          38      /* Function not implemented */
#endif
#ifndef  ENOTEMPTY
#define ENOTEMPTY       39      /* Directory not empty */
#endif
#ifndef  ELOOP
#define ELOOP           40      /* Too many symbolic links encountered */
#endif
#ifndef  EWOULDBLOCK
#define EWOULDBLOCK     EAGAIN  /* Operation would block */
#endif
#ifndef  ENOMSG
#define ENOMSG          42      /* No message of desired type */
#endif
#ifndef  EIDRM
#define EIDRM           43      /* Identifier removed */
#endif
#ifndef  ECHRNG
#define ECHRNG          44      /* Channel number out of range */
#endif
#ifndef  EL2NSYNC
#define EL2NSYNC        245      /* Level 2 not synchronized */
#endif
#ifndef  EL3HLT
#define EL3HLT          246      /* Level 3 halted */
#endif
#ifndef  EL3RST
#define EL3RST          47      /* Level 3 reset */
#endif
#ifndef  ELNRNG
#define ELNRNG          48      /* Link number out of range */
#endif
#ifndef  EUNATCH
#define EUNATCH         49      /* Protocol driver not attached */
#endif
#ifndef  ENOCSI
#define ENOCSI          50      /* No CSI structure available */
#endif
#ifndef  EL2HLT
#define EL2HLT          51      /* Level 2 halted */
#endif
#ifndef  EBADE
#define EBADE           52      /* Invalid exchange */
#endif
#ifndef  EBADR
#define EBADR           53      /* Invalid request descriptor */
#endif
#ifndef  EXFULL
#define EXFULL          54      /* Exchange full */
#endif
#ifndef  ENOANO
#define ENOANO          55      /* No anode */
#endif
#ifndef  EBADRQC
#define EBADRQC         56      /* Invalid request code */
#endif
#ifndef  EBADSLT
#define EBADSLT         57      /* Invalid slot */
#endif
#ifndef  EDEADLOCK
#define EDEADLOCK       EDEADLK /* Resource deadlock would occur */
#endif
#ifndef  EBFONT
#define EBFONT          59      /* Bad font file format */
#endif
#ifndef  ENOSTR
#define ENOSTR          60      /* Device not a stream */
#endif
#ifndef  ENODATA
#define ENODATA         61      /* No data available */
#endif
#ifndef  ETIME
#define ETIME           62      /* Timer expired */
#endif
#ifndef  ENOSR
#define ENOSR           63      /* Out of streams resources */
#endif
#ifndef  ENONET
#define ENONET          64      /* Machine is not on the network */
#endif
#ifndef  ENOPKG
#define ENOPKG          65      /* Package not installed */
#endif
#ifndef  EREMOTE
#define EREMOTE         66      /* Object is remote */
#endif
#ifndef  ENOLINK
#define ENOLINK         67      /* Link has been severed */
#endif
#ifndef  EADV
#define EADV            68      /* Advertise error */
#endif
#ifndef  ESRMNT
#define ESRMNT          69      /* Srmount error */
#endif
#ifndef  ECOMM
#define ECOMM           70      /* Communication error on send */
#endif
#ifndef  EPROTO
#define EPROTO          71      /* Protocol error */
#endif
#ifndef  EMULTIHOP
#define EMULTIHOP       72      /* Multihop attempted */
#endif
#ifndef  EDOTDOT
#define EDOTDOT         73      /* RFS specific error */
#endif
#ifndef  EBADMSG
#define EBADMSG         74      /* Not a data message */
#endif
#ifndef  EOVERFLOW
#define EOVERFLOW       75      /* Value too large for defined data type */
#endif
#ifndef  ENOTUNIQ
#define ENOTUNIQ        76      /* Name not unique on network */
#endif
#ifndef  EBADFD
#define EBADFD          277      /* File descriptor in bad state */
#endif
#ifndef  EREMCHG
#define EREMCHG         78      /* Remote address changed */
#endif
#ifndef  ELIBACC
#define ELIBACC         79      /* Can not access a needed shared library */
#endif
#ifndef  ELIBBAD
#define ELIBBAD         80      /* Accessing a corrupted shared library */
#endif
#ifndef  ELIBSCN
#define ELIBSCN         81      /* .lib section in a.out corrupted */
#endif
#ifndef  ELIBMAX
#define ELIBMAX         82      /* Attempting to link in too many shared libraries */
#endif
#ifndef  ELIBEXEC
#define ELIBEXEC        83      /* Cannot exec a shared library directly */
#endif

#if defined ( __ICCARM__ )
#undef  EILSEQ                  /* defined in IAR arm/inc/c/errno.h:  #define EILSEQ  36 */
#define EILSEQ          84      /* Illegal byte sequence */
#else
#ifndef  EILSEQ
#define EILSEQ          84      /* Illegal byte sequence */
#endif
#endif

#ifndef  ERESTART
#define ERESTART        285      /* Interrupted system call should be restarted */
#endif
#ifndef  ESTRPIPE
#define ESTRPIPE        86      /* Streams pipe error */
#endif
#ifndef  EUSERS
#define EUSERS          87      /* Too many users */
#endif
#ifndef  ENOTSOCK
#define ENOTSOCK        88      /* Socket operation on non-socket */
#endif
#ifndef  EDESTADDRREQ
#define EDESTADDRREQ    89      /* Destination address required */
#endif
#ifndef  EMSGSIZE
#define EMSGSIZE        90      /* Message too long */
#endif
#ifndef  EPROTOTYPE
#define EPROTOTYPE      91      /* Protocol wrong type for socket */
#endif
#ifndef  ENOPROTOOPT
#define ENOPROTOOPT     92      /* Protocol not available */
#endif
#ifndef  EPROTONOSUPPORT
#define EPROTONOSUPPORT 93      /* Protocol not supported */
#endif
#ifndef  ESOCKTNOSUPPORT
#define ESOCKTNOSUPPORT 94      /* Socket type not supported */
#endif
#ifndef  EOPNOTSUPP
#define EOPNOTSUPP      95      /* Operation not supported on transport endpoint */
#endif
#ifndef  EPFNOSUPPORT
#define EPFNOSUPPORT    96      /* Protocol family not supported */
#endif
#ifndef  EAFNOSUPPORT
#define EAFNOSUPPORT    97      /* Address family not supported by protocol */
#endif
#ifndef  EADDRINUSE
#define EADDRINUSE      98      /* Address already in use */
#endif
#ifndef  EADDRNOTAVAIL
#define EADDRNOTAVAIL   99      /* Cannot assign requested address */
#endif
#ifndef  ENETDOWN
#define ENETDOWN        100     /* Network is down */
#endif
#ifndef  ENETUNREACH
#define ENETUNREACH     101     /* Network is unreachable */
#endif
#ifndef  ENETRESET
#define ENETRESET       102     /* Network dropped connection because of reset */
#endif
#ifndef  ECONNABORTED
#define ECONNABORTED    103     /* Software caused connection abort */
#endif
#ifndef  ECONNRESET
#define ECONNRESET      104     /* Connection reset by peer */
#endif
#ifndef  ENOBUFS
#define ENOBUFS         105     /* No buffer space available */
#endif
#ifndef  EISCONN
#define EISCONN         106     /* Transport endpoint is already connected */
#endif
#ifndef  ENOTCONN
#define ENOTCONN        107     /* Transport endpoint is not connected */
#endif
#ifndef  ESHUTDOWN
#define ESHUTDOWN       208     /* Cannot send after transport endpoint shutdown */
#endif
#ifndef  ETOOMANYREFS
#define ETOOMANYREFS    109     /* Too many references: cannot splice */
#endif
#ifndef  ETIMEDOUT
#define ETIMEDOUT       110     /* Connection timed out */
#endif
#ifndef  ECONNREFUSED
#define ECONNREFUSED    111     /* Connection refused */
#endif
#ifndef  EHOSTDOWN
#define EHOSTDOWN       112     /* Host is down */
#endif
#ifndef  EHOSTUNREACH
#define EHOSTUNREACH    113     /* No route to host */
#endif
#ifndef  EALREADY
#define EALREADY        114     /* Operation already in progress */
#endif
#ifndef  EINPROGRESS
#define EINPROGRESS     115     /* Operation now in progress */
#endif
#ifndef  ESTALE
#define ESTALE          116     /* Stale NFS file handle */
#endif
#ifndef  EUCLEAN
#define EUCLEAN         217     /* Structure needs cleaning */
#endif
#ifndef  ENOTNAM
#define ENOTNAM         218     /* Not a XENIX named type file */
#endif
#ifndef  ENAVAIL
#define ENAVAIL         219     /* No XENIX semaphores available */
#endif
#ifndef  EISNAM
#define EISNAM          220     /* Is a named type file */
#endif
#ifndef  EREMOTEIO
#define EREMOTEIO       221     /* Remote I/O error */
#endif
#ifndef  EDQUOT
#define EDQUOT          122     /* Quota exceeded */
#endif
#ifndef  ENOMEDIUM
#define ENOMEDIUM       223     /* No medium found */
#endif
#ifndef  EMEDIUMTYPE
#define EMEDIUMTYPE     224     /* Wrong medium type */
#endif
#ifndef  ECANCELED
#define ECANCELED       125     /* Operation Canceled */
#endif
#ifndef  ENOKEY
#define ENOKEY          226     /* Required key not available */
#endif
#ifndef  EKEYEXPIRED
#define EKEYEXPIRED     227     /* Key has expired */
#endif
#ifndef  EKEYREVOKED
#define EKEYREVOKED     228     /* Key has been revoked */
#endif
#ifndef  EKEYREJECTED
#define EKEYREJECTED    229     /* Key was rejected by service */
#endif
#ifndef  EOWNERDEAD
#define EOWNERDEAD      130     /* Owner died */
#endif
#ifndef  ENOTRECOVERABLE
#define ENOTRECOVERABLE 131     /* State not recoverable */
#endif

/* Missing stat.h defines.
 * The following are sys/stat.h definitions not currently present in the ARMCC
 * errno.h. Note, ARMCC errno.h defines some symbol values differing from
 * GCC_ARM/IAR/standard POSIX definitions. Guard against this and future
 * changes by changing the symbol definition for filesystem use.
 */
#define     _IFMT   0170000 //< type of file
#define     _IFSOCK 0140000 //< socket
#define     _IFLNK  0120000 //< symbolic link
#define     _IFREG  0100000 //< regular
#define     _IFBLK  0060000 //< block special
#define     _IFDIR  0040000 //< directory
#define     _IFCHR  0020000 //< character special
#define     _IFIFO  0010000 //< fifo special

#define S_IFMT      _IFMT   //< type of file
#define S_IFSOCK    _IFSOCK //< socket
#define S_IFLNK     _IFLNK  //< symbolic link
#define S_IFREG     _IFREG  //< regular
#define S_IFBLK     _IFBLK  //< block special
#define S_IFDIR     _IFDIR  //< directory
#define S_IFCHR     _IFCHR  //< character special
#define S_IFIFO     _IFIFO  //< fifo special

#define S_IRWXU     (S_IRUSR | S_IWUSR | S_IXUSR)
#define     S_IRUSR 0000400 ///< read permission, owner
#define     S_IWUSR 0000200 ///< write permission, owner
#define     S_IXUSR 0000100 ///< execute/search permission, owner
#define S_IRWXG     (S_IRGRP | S_IWGRP | S_IXGRP)
#define     S_IRGRP 0000040 ///< read permission, group
#define     S_IWGRP 0000020 ///< write permission, group
#define     S_IXGRP 0000010 ///< execute/search permission, group
#define S_IRWXO     (S_IROTH | S_IWOTH | S_IXOTH)
#define     S_IROTH 0000004 ///< read permission, other
#define     S_IWOTH 0000002 ///< write permission, other
#define     S_IXOTH 0000001 ///< execute/search permission, other

#ifndef HAVE_SYS_STAT_H

/* Refer to sys/stat standard
 * Note: Not all fields may be supported by the underlying filesystem
 */
struct stat {
    dev_t     st_dev;     ///< Device ID containing file
    ino_t     st_ino;     ///< File serial number
    mode_t    st_mode;    ///< Mode of file
    nlink_t   st_nlink;   ///< Number of links to file

    uid_t     st_uid;     ///< User ID
    gid_t     st_gid;     ///< Group ID

    off_t     st_size;    ///< Size of file in bytes

    time_t    st_atime;   ///< Time of last access
    time_t    st_mtime;   ///< Time of last data modification
    time_t    st_ctime;   ///< Time of last status change
};

#endif

struct statvfs {
    unsigned long  f_bsize;    ///< Filesystem block size
    unsigned long  f_frsize;   ///< Fragment size (block size)

    fsblkcnt_t     f_blocks;   ///< Number of blocks
    fsblkcnt_t     f_bfree;    ///< Number of free blocks
    fsblkcnt_t     f_bavail;   ///< Number of free blocks for unprivileged users

    unsigned long  f_fsid;     ///< Filesystem ID

    unsigned long  f_namemax;  ///< Maximum filename length
};

/* The following are dirent.h definitions are declared here to guarantee
 * consistency where structure may be different with different toolchains
 */
struct dirent {
    char d_name[NAME_MAX + 1]; ///< Name of file
    uint8_t d_type;          ///< Type of file
};

enum {
    DT_UNKNOWN, ///< The file type could not be determined.
    DT_FIFO,    ///< This is a named pipe (FIFO).
    DT_CHR,     ///< This is a character device.
    DT_DIR,     ///< This is a directory.
    DT_BLK,     ///< This is a block device.
    DT_REG,     ///< This is a regular file.
    DT_LNK,     ///< This is a symbolic link.
    DT_SOCK,    ///< This is a UNIX domain socket.
};

/* fcntl.h defines */
#define F_GETFL 3
#define F_SETFL 4

struct pollfd {
    int fd;
    short events;
    short revents;
};

/* POSIX-compatible I/O functions */
#if __cplusplus
extern "C" {
#endif
#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
    int open(const char *path, int oflag, ...);
#ifndef __IAR_SYSTEMS_ICC__ /* IAR provides fdopen itself */
#if __cplusplus
    std::FILE *fdopen(int fildes, const char *mode);
#else
    FILE *fdopen(int fildes, const char *mode);
#endif
#endif
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
    ssize_t write(int fildes, const void *buf, size_t nbyte);
    ssize_t read(int fildes, void *buf, size_t nbyte);
    int fsync(int fildes);
    int isatty(int fildes);
#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
    off_t lseek(int fildes, off_t offset, int whence);
    int ftruncate(int fildes, off_t length);
    int fstat(int fildes, struct stat *st);
    int fcntl(int fildes, int cmd, ...);
    int poll(struct pollfd fds[], nfds_t nfds, int timeout);
    int close(int fildes);
    int stat(const char *path, struct stat *st);
    int statvfs(const char *path, struct statvfs *buf);
    DIR *opendir(const char *);
    struct dirent *readdir(DIR *);
    int closedir(DIR *);
    void rewinddir(DIR *);
    long telldir(DIR *);
    void seekdir(DIR *, long);
    int mkdir(const char *name, mode_t n);
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
#if __cplusplus
} // extern "C"

namespace mbed {
#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
/** This call is an analogue to POSIX fdopen().
 *
 *  It associates a C stream to an already-opened FileHandle, to allow you to
 *  use C printf/scanf/fwrite etc. The provided FileHandle must remain open -
 *  it will be closed by the C library when fclose(FILE) is called.
 *
 *  The net effect is fdopen(bind_to_fd(fh), mode), with error handling.
 *
 *  @param fh       a pointer to an opened FileHandle
 *  @param mode     operation upon the file descriptor, e.g., "w+"
 *
 *  @returns        a pointer to FILE
 */
std::FILE *fdopen(mbed::FileHandle *fh, const char *mode);

/** Bind an mbed FileHandle to a POSIX file descriptor
 *
 * This is similar to fdopen, but only operating at the POSIX layer - it
 * associates a POSIX integer file descriptor with a FileHandle, to allow you
 * to use POSIX read/write calls etc. The provided FileHandle must remain open -
 * it will be closed when close(int) is called.
 *
 *  @param fh       a pointer to an opened FileHandle
 *
 *  @return         an integer file descriptor, or negative if no descriptors available
 */
int bind_to_fd(mbed::FileHandle *fh);

#else
/** Targets may implement this to override how to write to the console.
 *
 * If the target has provided minimal_console_putc, this is called
 * to give the target a chance to specify an alternative minimal console.
 *
 * If this is not provided, serial_putc will be used if
 * `target.console-uart` is `true`, else there will not be an output.
 *
 *  @param c   The char to write
 *  @return    The written char
 */
int minimal_console_putc(int c);

/** Targets may implement this to override how to read from the console.
 *
 * If the target has provided minimal_console_getc, this is called
 * to give the target a chance to specify an alternative minimal console.
 *
 * If this is not provided, serial_getc will be used if
 * `target.console-uart` is `true`, else no input would be captured.
 *
 *  @return  The char read from the serial port
 */
int minimal_console_getc();
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY

} // namespace mbed

#endif // __cplusplus

/**@}*/

/**@}*/

#endif /* RETARGET_H */
