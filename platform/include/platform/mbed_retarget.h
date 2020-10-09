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
#define O_RDONLY 0        ///< Open for reading
#define O_WRONLY 1        ///< Open for writing
#define O_RDWR   2        ///< Open for reading and writing
#define O_NONBLOCK 0x0004 ///< Non-blocking mode
#define O_APPEND   0x0008 ///< Set file offset to end of file prior to each write
#define O_CREAT    0x0200 ///< Create file if it does not exist
#define O_TRUNC    0x0400 ///< Truncate file to zero length
#define O_EXCL     0x0800 ///< Fail if file exists
#define O_BINARY   0x8000 ///< Open file in binary mode

#define O_ACCMODE   (O_RDONLY|O_WRONLY|O_RDWR)

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

typedef mbed::DirHandle DIR;
#else
typedef struct Dir DIR;
#endif
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY

/* The intent of this section is to unify the errno error values to match
 * the POSIX definitions for the GCC_ARM, ARMCC and IAR compilers. This is
 * necessary because the ARMCC/IAR errno.h, or sys/stat.h are missing some
 * symbol definitions used by the POSIX filesystem API to return errno codes.
 * Note also that ARMCC errno.h defines some symbol values differently from
 * the GCC_ARM/IAR/standard POSIX definitions. The definitions guard against
 * this and future changes by changing the symbol definition as shown below.
 */
#undef  EPERM
#define EPERM           1       /* Operation not permitted */
#undef  ENOENT
#define ENOENT          2       /* No such file or directory */
#undef  ESRCH
#define ESRCH           3       /* No such process */
#undef  EINTR
#define EINTR           4       /* Interrupted system call */
#undef  EIO
#define EIO             5       /* I/O error */
#undef  ENXIO
#define ENXIO           6       /* No such device or address */
#undef  E2BIG
#define E2BIG           7       /* Argument list too long */
#undef  ENOEXEC
#define ENOEXEC         8       /* Exec format error */
#undef  EBADF
#define EBADF           9       /* Bad file number */
#undef  ECHILD
#define ECHILD          10      /* No child processes */
#undef  EAGAIN
#define EAGAIN          11      /* Try again */
#undef  ENOMEM
#define ENOMEM          12      /* Out of memory */
#undef  EACCES
#define EACCES          13      /* Permission denied */
#undef  EFAULT
#define EFAULT          14      /* Bad address */
#undef  ENOTBLK
#define ENOTBLK         15      /* Block device required */
#undef  EBUSY
#define EBUSY           16      /* Device or resource busy */
#undef  EEXIST
#define EEXIST          17      /* File exists */
#undef  EXDEV
#define EXDEV           18      /* Cross-device link */
#undef  ENODEV
#define ENODEV          19      /* No such device */
#undef  ENOTDIR
#define ENOTDIR         20      /* Not a directory */
#undef  EISDIR
#define EISDIR          21      /* Is a directory */
#undef  EINVAL
#define EINVAL          22      /* Invalid argument */
#undef  ENFILE
#define ENFILE          23      /* File table overflow */
#undef  EMFILE
#define EMFILE          24      /* Too many open files */
#undef  ENOTTY
#define ENOTTY          25      /* Not a typewriter */
#undef  ETXTBSY
#define ETXTBSY         26      /* Text file busy */
#undef  EFBIG
#define EFBIG           27      /* File too large */
#undef  ENOSPC
#define ENOSPC          28      /* No space left on device */
#undef  ESPIPE
#define ESPIPE          29      /* Illegal seek */
#undef  EROFS
#define EROFS           30      /* Read-only file system */
#undef  EMLINK
#define EMLINK          31      /* Too many links */
#undef  EPIPE
#define EPIPE           32      /* Broken pipe */
#undef  EDOM
#define EDOM            33      /* Math argument out of domain of func */
#undef  ERANGE
#define ERANGE          34      /* Math result not representable */
#undef  EDEADLK
#define EDEADLK         35      /* Resource deadlock would occur */
#undef  ENAMETOOLONG
#define ENAMETOOLONG    36      /* File name too long */
#undef  ENOLCK
#define ENOLCK          37      /* No record locks available */
#undef  ENOSYS
#define ENOSYS          38      /* Function not implemented */
#undef  ENOTEMPTY
#define ENOTEMPTY       39      /* Directory not empty */
#undef  ELOOP
#define ELOOP           40      /* Too many symbolic links encountered */
#undef  EWOULDBLOCK
#define EWOULDBLOCK     EAGAIN  /* Operation would block */
#undef  ENOMSG
#define ENOMSG          42      /* No message of desired type */
#undef  EIDRM
#define EIDRM           43      /* Identifier removed */
#undef  ECHRNG
#define ECHRNG          44      /* Channel number out of range */
#undef  EL2NSYNC
#define EL2NSYNC        45      /* Level 2 not synchronized */
#undef  EL3HLT
#define EL3HLT          46      /* Level 3 halted */
#undef  EL3RST
#define EL3RST          47      /* Level 3 reset */
#undef  ELNRNG
#define ELNRNG          48      /* Link number out of range */
#undef  EUNATCH
#define EUNATCH         49      /* Protocol driver not attached */
#undef  ENOCSI
#define ENOCSI          50      /* No CSI structure available */
#undef  EL2HLT
#define EL2HLT          51      /* Level 2 halted */
#undef  EBADE
#define EBADE           52      /* Invalid exchange */
#undef  EBADR
#define EBADR           53      /* Invalid request descriptor */
#undef  EXFULL
#define EXFULL          54      /* Exchange full */
#undef  ENOANO
#define ENOANO          55      /* No anode */
#undef  EBADRQC
#define EBADRQC         56      /* Invalid request code */
#undef  EBADSLT
#define EBADSLT         57      /* Invalid slot */
#undef  EDEADLOCK
#define EDEADLOCK       EDEADLK /* Resource deadlock would occur */
#undef  EBFONT
#define EBFONT          59      /* Bad font file format */
#undef  ENOSTR
#define ENOSTR          60      /* Device not a stream */
#undef  ENODATA
#define ENODATA         61      /* No data available */
#undef  ETIME
#define ETIME           62      /* Timer expired */
#undef  ENOSR
#define ENOSR           63      /* Out of streams resources */
#undef  ENONET
#define ENONET          64      /* Machine is not on the network */
#undef  ENOPKG
#define ENOPKG          65      /* Package not installed */
#undef  EREMOTE
#define EREMOTE         66      /* Object is remote */
#undef  ENOLINK
#define ENOLINK         67      /* Link has been severed */
#undef  EADV
#define EADV            68      /* Advertise error */
#undef  ESRMNT
#define ESRMNT          69      /* Srmount error */
#undef  ECOMM
#define ECOMM           70      /* Communication error on send */
#undef  EPROTO
#define EPROTO          71      /* Protocol error */
#undef  EMULTIHOP
#define EMULTIHOP       72      /* Multihop attempted */
#undef  EDOTDOT
#define EDOTDOT         73      /* RFS specific error */
#undef  EBADMSG
#define EBADMSG         74      /* Not a data message */
#undef  EOVERFLOW
#define EOVERFLOW       75      /* Value too large for defined data type */
#undef  ENOTUNIQ
#define ENOTUNIQ        76      /* Name not unique on network */
#undef  EBADFD
#define EBADFD          77      /* File descriptor in bad state */
#undef  EREMCHG
#define EREMCHG         78      /* Remote address changed */
#undef  ELIBACC
#define ELIBACC         79      /* Can not access a needed shared library */
#undef  ELIBBAD
#define ELIBBAD         80      /* Accessing a corrupted shared library */
#undef  ELIBSCN
#define ELIBSCN         81      /* .lib section in a.out corrupted */
#undef  ELIBMAX
#define ELIBMAX         82      /* Attempting to link in too many shared libraries */
#undef  ELIBEXEC
#define ELIBEXEC        83      /* Cannot exec a shared library directly */
#undef  EILSEQ
#define EILSEQ          84      /* Illegal byte sequence */
#undef  ERESTART
#define ERESTART        85      /* Interrupted system call should be restarted */
#undef  ESTRPIPE
#define ESTRPIPE        86      /* Streams pipe error */
#undef  EUSERS
#define EUSERS          87      /* Too many users */
#undef  ENOTSOCK
#define ENOTSOCK        88      /* Socket operation on non-socket */
#undef  EDESTADDRREQ
#define EDESTADDRREQ    89      /* Destination address required */
#undef  EMSGSIZE
#define EMSGSIZE        90      /* Message too long */
#undef  EPROTOTYPE
#define EPROTOTYPE      91      /* Protocol wrong type for socket */
#undef  ENOPROTOOPT
#define ENOPROTOOPT     92      /* Protocol not available */
#undef  EPROTONOSUPPORT
#define EPROTONOSUPPORT 93      /* Protocol not supported */
#undef  ESOCKTNOSUPPORT
#define ESOCKTNOSUPPORT 94      /* Socket type not supported */
#undef  EOPNOTSUPP
#define EOPNOTSUPP      95      /* Operation not supported on transport endpoint */
#undef  EPFNOSUPPORT
#define EPFNOSUPPORT    96      /* Protocol family not supported */
#undef  EAFNOSUPPORT
#define EAFNOSUPPORT    97      /* Address family not supported by protocol */
#undef  EADDRINUSE
#define EADDRINUSE      98      /* Address already in use */
#undef  EADDRNOTAVAIL
#define EADDRNOTAVAIL   99      /* Cannot assign requested address */
#undef  ENETDOWN
#define ENETDOWN        100     /* Network is down */
#undef  ENETUNREACH
#define ENETUNREACH     101     /* Network is unreachable */
#undef  ENETRESET
#define ENETRESET       102     /* Network dropped connection because of reset */
#undef  ECONNABORTED
#define ECONNABORTED    103     /* Software caused connection abort */
#undef  ECONNRESET
#define ECONNRESET      104     /* Connection reset by peer */
#undef  ENOBUFS
#define ENOBUFS         105     /* No buffer space available */
#undef  EISCONN
#define EISCONN         106     /* Transport endpoint is already connected */
#undef  ENOTCONN
#define ENOTCONN        107     /* Transport endpoint is not connected */
#undef  ESHUTDOWN
#define ESHUTDOWN       108     /* Cannot send after transport endpoint shutdown */
#undef  ETOOMANYREFS
#define ETOOMANYREFS    109     /* Too many references: cannot splice */
#undef  ETIMEDOUT
#define ETIMEDOUT       110     /* Connection timed out */
#undef  ECONNREFUSED
#define ECONNREFUSED    111     /* Connection refused */
#undef  EHOSTDOWN
#define EHOSTDOWN       112     /* Host is down */
#undef  EHOSTUNREACH
#define EHOSTUNREACH    113     /* No route to host */
#undef  EALREADY
#define EALREADY        114     /* Operation already in progress */
#undef  EINPROGRESS
#define EINPROGRESS     115     /* Operation now in progress */
#undef  ESTALE
#define ESTALE          116     /* Stale NFS file handle */
#undef  EUCLEAN
#define EUCLEAN         117     /* Structure needs cleaning */
#undef  ENOTNAM
#define ENOTNAM         118     /* Not a XENIX named type file */
#undef  ENAVAIL
#define ENAVAIL         119     /* No XENIX semaphores available */
#undef  EISNAM
#define EISNAM          120     /* Is a named type file */
#undef  EREMOTEIO
#define EREMOTEIO       121     /* Remote I/O error */
#undef  EDQUOT
#define EDQUOT          122     /* Quota exceeded */
#undef  ENOMEDIUM
#define ENOMEDIUM       123     /* No medium found */
#undef  EMEDIUMTYPE
#define EMEDIUMTYPE     124     /* Wrong medium type */
#undef  ECANCELED
#define ECANCELED       125     /* Operation Canceled */
#undef  ENOKEY
#define ENOKEY          126     /* Required key not available */
#undef  EKEYEXPIRED
#define EKEYEXPIRED     127     /* Key has expired */
#undef  EKEYREVOKED
#define EKEYREVOKED     128     /* Key has been revoked */
#undef  EKEYREJECTED
#define EKEYREJECTED    129     /* Key was rejected by service */
#undef  EOWNERDEAD
#define EOWNERDEAD      130     /* Owner died */
#undef  ENOTRECOVERABLE
#define ENOTRECOVERABLE 131     /* State not recoverable */

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
