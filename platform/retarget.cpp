/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include "platform/platform.h"
#include "drivers/FileHandle.h"
#include "drivers/FileSystemLike.h"
#include "drivers/FilePath.h"
#include "hal/serial_api.h"
#include "platform/toolchain.h"
#include "platform/semihost_api.h"
#include "platform/mbed_interface.h"
#include "platform/SingletonPtr.h"
#include "platform/PlatformMutex.h"
#include "platform/mbed_error.h"
#include "platform/mbed_stats.h"
#include <stdlib.h>
#include <string.h>
#if DEVICE_STDIO_MESSAGES
#include <stdio.h>
#endif
#include <errno.h>

#if defined(__ARMCC_VERSION)
#   include <rt_sys.h>
#   define PREFIX(x)    _sys##x
#   define OPEN_MAX     _SYS_OPEN
#   ifdef __MICROLIB
#       pragma import(__use_full_stdio)
#   endif

#elif defined(__ICCARM__)
#   include <yfuns.h>
#   define PREFIX(x)        _##x
#   define OPEN_MAX         16

#   define STDIN_FILENO     0
#   define STDOUT_FILENO    1
#   define STDERR_FILENO    2

#else
#   include <sys/stat.h>
#   include <sys/unistd.h>
#   include <sys/syslimits.h>
#   define PREFIX(x)    x
#endif

#define FILE_HANDLE_RESERVED    0xFFFFFFFF

using namespace mbed;

#if defined(__MICROLIB) && (__ARMCC_VERSION>5030000)
// Before version 5.03, we were using a patched version of microlib with proper names
extern const char __stdin_name[]  = ":tt";
extern const char __stdout_name[] = ":tt";
extern const char __stderr_name[] = ":tt";

#else
extern const char __stdin_name[]  = "/stdin";
extern const char __stdout_name[] = "/stdout";
extern const char __stderr_name[] = "/stderr";
#endif

// Heap limits - only used if set
unsigned char *mbed_heap_start = 0;
uint32_t mbed_heap_size = 0;

/* newlib has the filehandle field in the FILE struct as a short, so
 * we can't just return a Filehandle* from _open and instead have to
 * put it in a filehandles array and return the index into that array
 * (or rather index+3, as filehandles 0-2 are stdin/out/err).
 */
static FileHandle *filehandles[OPEN_MAX];
static SingletonPtr<PlatformMutex> filehandle_mutex;

FileHandle::~FileHandle() {
    filehandle_mutex->lock();
    /* Remove all open filehandles for this */
    for (unsigned int fh_i = 0; fh_i < sizeof(filehandles)/sizeof(*filehandles); fh_i++) {
        if (filehandles[fh_i] == this) {
            filehandles[fh_i] = NULL;
        }
    }
    filehandle_mutex->unlock();
}

#if DEVICE_SERIAL
extern int stdio_uart_inited;
extern serial_t stdio_uart;
#if MBED_CONF_PLATFORM_STDIO_CONVERT_NEWLINES
static char stdio_in_prev;
static char stdio_out_prev;
#endif
#endif

static void init_serial() {
#if DEVICE_SERIAL
    if (stdio_uart_inited) return;
    serial_init(&stdio_uart, STDIO_UART_TX, STDIO_UART_RX);
#if MBED_CONF_PLATFORM_STDIO_BAUD_RATE
    serial_baud(&stdio_uart, MBED_CONF_PLATFORM_STDIO_BAUD_RATE);
#endif
#endif
}

static inline int openmode_to_posix(int openmode) {
    int posix = openmode;
#ifdef __ARMCC_VERSION
    if (openmode & OPEN_PLUS) {
        posix = O_RDWR;
    } else if(openmode & OPEN_W) {
        posix = O_WRONLY;
    } else if(openmode & OPEN_A) {
        posix = O_WRONLY|O_APPEND;
    } else {
        posix = O_RDONLY;
    }
    /* a, w, a+, w+ all create if file does not already exist */
    if (openmode & (OPEN_A|OPEN_W)) {
        posix |= O_CREAT;
    }
    /* w and w+ truncate */
    if (openmode & OPEN_W) {
        posix |= O_TRUNC;
    }
#elif defined(__ICCARM__)
    switch (openmode & _LLIO_RDWRMASK) {
        case _LLIO_RDONLY: posix = O_RDONLY; break;
        case _LLIO_WRONLY: posix = O_WRONLY; break;
        case _LLIO_RDWR  : posix = O_RDWR  ; break;
    }
    if (openmode & _LLIO_CREAT ) posix |= O_CREAT;
    if (openmode & _LLIO_APPEND) posix |= O_APPEND;
    if (openmode & _LLIO_TRUNC ) posix |= O_TRUNC;
#elif defined(TOOLCHAIN_GCC)
    posix &= ~O_BINARY;
#endif
    return posix;
}

extern "C" WEAK void mbed_sdk_init(void);
extern "C" WEAK void mbed_sdk_init(void) {
}

extern "C" FILEHANDLE PREFIX(_open)(const char* name, int openmode) {
    #if defined(__MICROLIB) && (__ARMCC_VERSION>5030000)
    // Before version 5.03, we were using a patched version of microlib with proper names
    // This is the workaround that the microlib author suggested us
    static int n = 0;
    static int mbed_sdk_inited = 0;
    if (!mbed_sdk_inited) {
        mbed_sdk_inited = 1;
        mbed_sdk_init();
    }
    if (!std::strcmp(name, ":tt")) return n++;
    #else
    /* Use the posix convention that stdin,out,err are filehandles 0,1,2.
     */
    if (std::strcmp(name, __stdin_name) == 0) {
        init_serial();
        return 0;
    } else if (std::strcmp(name, __stdout_name) == 0) {
        init_serial();
        return 1;
    } else if (std::strcmp(name, __stderr_name) == 0) {
        init_serial();
        return 2;
    }
    #endif

    // find the first empty slot in filehandles
    filehandle_mutex->lock();
    unsigned int fh_i;
    for (fh_i = 0; fh_i < sizeof(filehandles)/sizeof(*filehandles); fh_i++) {
        if (filehandles[fh_i] == NULL) break;
    }
    if (fh_i >= sizeof(filehandles)/sizeof(*filehandles)) {
        filehandle_mutex->unlock();
        return -1;
    }
    filehandles[fh_i] = (FileHandle*)FILE_HANDLE_RESERVED;
    filehandle_mutex->unlock();

    FileHandle *res;

    /* FILENAME: ":0x12345678" describes a FileLike* */
    if (name[0] == ':') {
        void *p;
        sscanf(name, ":%p", &p);
        res = (FileHandle*)p;

    /* FILENAME: "/file_system/file_name" */
    } else {
        FilePath path(name);

        if (!path.exists()) {
            // Free file handle
            filehandles[fh_i] = NULL;
            return -1;
        } else if (path.isFile()) {
            res = path.file();
        } else {
            FileSystemLike *fs = path.fileSystem();
            if (fs == NULL) {
                // Free file handle
                filehandles[fh_i] = NULL;
                return -1;
            }
            int posix_mode = openmode_to_posix(openmode);
            res = fs->open(path.fileName(), posix_mode); /* NULL if fails */
        }
    }

    if (res == NULL) {
        // Free file handle
        filehandles[fh_i] = NULL;
        return -1;
    }
    filehandles[fh_i] = res;

    return fh_i + 3; // +3 as filehandles 0-2 are stdin/out/err
}

extern "C" int PREFIX(_close)(FILEHANDLE fh) {
    if (fh < 3) return 0;

    FileHandle* fhc = filehandles[fh-3];
    filehandles[fh-3] = NULL;
    if (fhc == NULL) return -1;

    return fhc->close();
}

#if defined(__ICCARM__)
extern "C" size_t    __write (int        fh, const unsigned char *buffer, size_t length) {
#else
extern "C" int PREFIX(_write)(FILEHANDLE fh, const unsigned char *buffer, unsigned int length, int mode) {
#endif
    int n; // n is the number of bytes written
    if (fh < 3) {
#if DEVICE_SERIAL
        if (!stdio_uart_inited) init_serial();
#if MBED_CONF_PLATFORM_STDIO_CONVERT_NEWLINES
        for (unsigned int i = 0; i < length; i++) {
            if (buffer[i] == '\n' && stdio_out_prev != '\r') {
                 serial_putc(&stdio_uart, '\r');
            }
            serial_putc(&stdio_uart, buffer[i]);
            stdio_out_prev = buffer[i];
        }
#else
        for (unsigned int i = 0; i < length; i++) {
            serial_putc(&stdio_uart, buffer[i]);
        }
#endif
#endif
        n = length;
    } else {
        FileHandle* fhc = filehandles[fh-3];
        if (fhc == NULL) return -1;

        n = fhc->write(buffer, length);
    }
#ifdef __ARMCC_VERSION
    return length-n;
#else
    return n;
#endif
}

#if defined(__ICCARM__)
extern "C" size_t    __read (int        fh, unsigned char *buffer, size_t       length) {
#else
extern "C" int PREFIX(_read)(FILEHANDLE fh, unsigned char *buffer, unsigned int length, int mode) {
#endif
    int n; // n is the number of bytes read
    if (fh < 3) {
        // only read a character at a time from stdin
#if DEVICE_SERIAL
        if (!stdio_uart_inited) init_serial();
#if MBED_CONF_PLATFORM_STDIO_CONVERT_NEWLINES
        while (true) {
            char c = serial_getc(&stdio_uart);
            if ((c == '\r' && stdio_in_prev != '\n') ||
                (c == '\n' && stdio_in_prev != '\r')) {
                stdio_in_prev = c;
                *buffer = '\n';
                break;
            } else if ((c == '\r' && stdio_in_prev == '\n') ||
                       (c == '\n' && stdio_in_prev == '\r')) {
                stdio_in_prev = c;
                // onto next character
                continue;
            } else {
                stdio_in_prev = c;
                *buffer = c;
                break;
            }
        }
#else
        *buffer = serial_getc(&stdio_uart);
#endif
#endif
        n = 1;
    } else {
        FileHandle* fhc = filehandles[fh-3];
        if (fhc == NULL) return -1;

        n = fhc->read(buffer, length);
    }
#ifdef __ARMCC_VERSION
    return length-n;
#else
    return n;
#endif
}

#ifdef __ARMCC_VERSION
extern "C" int PREFIX(_istty)(FILEHANDLE fh)
#else
extern "C" int _isatty(FILEHANDLE fh)
#endif
{
    /* stdin, stdout and stderr should be tty */
    if (fh < 3) return 1;

    FileHandle* fhc = filehandles[fh-3];
    if (fhc == NULL) return -1;

    return fhc->isatty();
}

extern "C"
#if defined(__ARMCC_VERSION)
int _sys_seek(FILEHANDLE fh, long position)
#elif defined(__ICCARM__)
long __lseek(int fh, long offset, int whence)
#else
int _lseek(FILEHANDLE fh, int offset, int whence)
#endif
{
    if (fh < 3) return 0;

    FileHandle* fhc = filehandles[fh-3];
    if (fhc == NULL) return -1;

#if defined(__ARMCC_VERSION)
    return fhc->lseek(position, SEEK_SET);
#else
    return fhc->lseek(offset, whence);
#endif
}

#ifdef __ARMCC_VERSION
extern "C" int PREFIX(_ensure)(FILEHANDLE fh) {
    if (fh < 3) return 0;

    FileHandle* fhc = filehandles[fh-3];
    if (fhc == NULL) return -1;

    return fhc->fsync();
}

extern "C" long PREFIX(_flen)(FILEHANDLE fh) {
    if (fh < 3) return 0;

    FileHandle* fhc = filehandles[fh-3];
    if (fhc == NULL) return -1;

    return fhc->flen();
}
#endif


#if !defined(__ARMCC_VERSION) && !defined(__ICCARM__)
extern "C" int _fstat(int fd, struct stat *st) {
    if ((STDOUT_FILENO == fd) || (STDERR_FILENO == fd) || (STDIN_FILENO == fd)) {
        st->st_mode = S_IFCHR;
        return  0;
    }

    errno = EBADF;
    return -1;
}
#endif

namespace std {
extern "C" int remove(const char *path) {
    FilePath fp(path);
    FileSystemLike *fs = fp.fileSystem();
    if (fs == NULL) return -1;

    return fs->remove(fp.fileName());
}

extern "C" int rename(const char *oldname, const char *newname) {
    FilePath fpOld(oldname);
    FilePath fpNew(newname);
    FileSystemLike *fsOld = fpOld.fileSystem();
    FileSystemLike *fsNew = fpNew.fileSystem();

    /* rename only if both files are on the same FS */
    if (fsOld != fsNew || fsOld == NULL) return -1;

    return fsOld->rename(fpOld.fileName(), fpNew.fileName());
}

extern "C" char *tmpnam(char *s) {
    return NULL;
}

extern "C" FILE *tmpfile() {
    return NULL;
}
} // namespace std

#ifdef __ARMCC_VERSION
extern "C" char *_sys_command_string(char *cmd, int len) {
    return NULL;
}
#endif

extern "C" DIR *opendir(const char *path) {
    /* root dir is FileSystemLike */
    if (path[0] == '/' && path[1] == 0) {
        return FileSystemLike::opendir();
    }

    FilePath fp(path);
    FileSystemLike* fs = fp.fileSystem();
    if (fs == NULL) return NULL;

    return fs->opendir(fp.fileName());
}

extern "C" struct dirent *readdir(DIR *dir) {
    return dir->readdir();
}

extern "C" int closedir(DIR *dir) {
    return dir->closedir();
}

extern "C" void rewinddir(DIR *dir) {
    dir->rewinddir();
}

extern "C" off_t telldir(DIR *dir) {
    return dir->telldir();
}

extern "C" void seekdir(DIR *dir, off_t off) {
    dir->seekdir(off);
}

extern "C" int mkdir(const char *path, mode_t mode) {
    FilePath fp(path);
    FileSystemLike *fs = fp.fileSystem();
    if (fs == NULL) return -1;

    return fs->mkdir(fp.fileName(), mode);
}

#if defined(TOOLCHAIN_GCC)
/* prevents the exception handling name demangling code getting pulled in */
#include "mbed_error.h"
namespace __gnu_cxx {
    void __verbose_terminate_handler() {
        error("Exception");
    }
}
extern "C" WEAK void __cxa_pure_virtual(void);
extern "C" WEAK void __cxa_pure_virtual(void) {
    exit(1);
}

#endif

#if defined(TOOLCHAIN_GCC)

#ifdef  FEATURE_UVISOR
#include "uvisor-lib/uvisor-lib.h"
#endif/* FEATURE_UVISOR */


extern "C" WEAK void software_init_hook_rtos(void)
{
    // Do nothing by default.
}

extern "C" void software_init_hook(void)
{
#ifdef   FEATURE_UVISOR
    int return_code;

    return_code = uvisor_lib_init();
    if (return_code) {
        mbed_die();
    }
#endif/* FEATURE_UVISOR */
    mbed_sdk_init();
    software_init_hook_rtos();
}
#endif

// ****************************************************************************
// mbed_main is a function that is called before main()
// mbed_sdk_init() is also a function that is called before main(), but unlike
// mbed_main(), it is not meant for user code, but for the SDK itself to perform
// initializations before main() is called.

extern "C" WEAK void mbed_main(void);
extern "C" WEAK void mbed_main(void) {
}

#if defined(TOOLCHAIN_ARM)
extern "C" int $Super$$main(void);

extern "C" int $Sub$$main(void) {
    mbed_main();
    return $Super$$main();
}

extern "C" void _platform_post_stackheap_init (void) {
    mbed_sdk_init();
}

#elif defined(TOOLCHAIN_GCC)
extern "C" int __real_main(void);

extern "C" int __wrap_main(void) {
    mbed_main();
    return __real_main();
}
#elif defined(TOOLCHAIN_IAR)
// IAR doesn't have the $Super/$Sub mechanism of armcc, nor something equivalent
// to ld's --wrap. It does have a --redirect, but that doesn't help, since redirecting
// 'main' to another symbol looses the original 'main' symbol. However, its startup
// code will call a function to setup argc and argv (__iar_argc_argv) if it is defined.
// Since mbed doesn't use argc/argv, we use this function to call our mbed_main.
extern "C" void __iar_argc_argv() {
    mbed_main();
}
#endif

// Provide implementation of _sbrk (low-level dynamic memory allocation
// routine) for GCC_ARM which compares new heap pointer with MSP instead of
// SP.  This make it compatible with RTX RTOS thread stacks.
#if defined(TOOLCHAIN_GCC_ARM) || defined(TOOLCHAIN_GCC_CR)
// Linker defined symbol used by _sbrk to indicate where heap should start.
extern "C" int __end__;

#if defined(TARGET_CORTEX_A)
extern "C" uint32_t  __HeapLimit;
#endif

// Turn off the errno macro and use actual global variable instead.
#undef errno
extern "C" int errno;

// For ARM7 only
register unsigned char * stack_ptr __asm ("sp");

// Dynamic memory allocation related syscall.
#if defined(TARGET_NUMAKER_PFM_NUC472) || defined(TARGET_NUMAKER_PFM_M453)
// Overwrite _sbrk() to support two region model (heap and stack are two distinct regions).
// __wrap__sbrk() is implemented in:
// TARGET_NUMAKER_PFM_NUC472    hal/targets/cmsis/TARGET_NUVOTON/TARGET_NUC472/TARGET_NUMAKER_PFM_NUC472/TOOLCHAIN_GCC_ARM/retarget.c
// TARGET_NUMAKER_PFM_M453      hal/targets/cmsis/TARGET_NUVOTON/TARGET_M451/TARGET_NUMAKER_PFM_M453/TOOLCHAIN_GCC_ARM/retarget.c
extern "C" void *__wrap__sbrk(int incr);
extern "C" caddr_t _sbrk(int incr) {
    return (caddr_t) __wrap__sbrk(incr);
}
#else
extern "C" caddr_t _sbrk(int incr) {
    static unsigned char* heap = (unsigned char*)&__end__;
    unsigned char*        prev_heap = heap;
    unsigned char*        new_heap = heap + incr;

#if defined(TARGET_ARM7)
    if (new_heap >= stack_ptr) {
#elif defined(TARGET_CORTEX_A)
    if (new_heap >= (unsigned char*)&__HeapLimit) {     /* __HeapLimit is end of heap section */
#else
    if (new_heap >= (unsigned char*)__get_MSP()) {
#endif
        errno = ENOMEM;
        return (caddr_t)-1;
    }

    // Additional heap checking if set
    if (mbed_heap_size && (new_heap >= mbed_heap_start + mbed_heap_size)) {
        errno = ENOMEM;
        return (caddr_t)-1;
    }

    heap = new_heap;
    return (caddr_t) prev_heap;
}
#endif
#endif

#if defined(TOOLCHAIN_GCC_ARM) || defined(TOOLCHAIN_GCC_CR)
extern "C" void _exit(int return_code) {
#else
namespace std {
extern "C" void exit(int return_code) {
#endif

#if DEVICE_STDIO_MESSAGES
#if MBED_CONF_PLATFORM_STDIO_FLUSH_AT_EXIT
    fflush(stdout);
    fflush(stderr);
#endif
#endif

#if DEVICE_SEMIHOST
    if (mbed_interface_connected()) {
        semihost_exit();
    }
#endif
    if (return_code) {
        mbed_die();
    }

    while (1);
}

#if !defined(TOOLCHAIN_GCC_ARM) && !defined(TOOLCHAIN_GCC_CR)
} //namespace std
#endif

#if defined(TOOLCHAIN_ARM) || defined(TOOLCHAIN_GCC)

// This series of function disable the registration of global destructors
// in a dynamic table which will be called when the application exit.
// In mbed, program never exit properly, it dies.
// More informations about this topic for ARMCC here:
// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.faqs/6449.html
extern "C" {
int __aeabi_atexit(void *object, void (*dtor)(void* /*this*/), void *handle) {
    return 1;
}

int __cxa_atexit(void (*dtor)(void* /*this*/), void *object, void *handle) {
    return 1;
}

void __cxa_finalize(void *handle) {
}

} // end of extern "C"

#endif


#if defined(TOOLCHAIN_GCC)

/*
 * Depending on how newlib is  configured, it is often not enough to define
 * __aeabi_atexit, __cxa_atexit and __cxa_finalize in order to override the
 * behavior regarding the registration of handlers with atexit.
 *
 * To overcome this limitation, exit and atexit are overriden here.
 */
extern "C"{

/**
 * @brief Retarget of exit for GCC.
 * @details Unlike the standard version, this function doesn't call any function
 * registered with atexit before calling _exit.
 */
void __wrap_exit(int return_code) {
    _exit(return_code);
}

/**
 * @brief Retarget atexit from GCC.
 * @details This function will always fail and never register any handler to be
 * called at exit.
 */
int __wrap_atexit(void (*func)()) {
    return 1;
}

}

#endif



namespace mbed {

void mbed_set_unbuffered_stream(FILE *_file) {
#if defined (__ICCARM__)
    char buf[2];
    std::setvbuf(_file,buf,_IONBF,NULL);    
#else
    setbuf(_file, NULL);
#endif
}

int mbed_getc(FILE *_file){
#if defined (__ICCARM__)
    /*This is only valid for unbuffered streams*/
    int res = std::fgetc(_file);
    if (res>=0){
        _file->_Mode = (unsigned short)(_file->_Mode & ~ 0x1000);/* Unset read mode */
        _file->_Rend = _file->_Wend;
        _file->_Next = _file->_Wend;
    }    
    return res;
#else    
    return std::fgetc(_file);
#endif   
}

char* mbed_gets(char*s, int size, FILE *_file){
#if defined (__ICCARM__)
    /*This is only valid for unbuffered streams*/
    char *str = fgets(s,size,_file);
    if (str!=NULL){
        _file->_Mode = (unsigned short)(_file->_Mode & ~ 0x1000);/* Unset read mode */
        _file->_Rend = _file->_Wend;
        _file->_Next = _file->_Wend;
    }
    return str;
#else    
    return std::fgets(s,size,_file);
#endif
}

} // namespace mbed

#if defined (__ICCARM__)
// Stub out locks when an rtos is not present
extern "C" WEAK void __iar_system_Mtxinit(__iar_Rmtx *mutex) {}
extern "C" WEAK void __iar_system_Mtxdst(__iar_Rmtx *mutex) {}
extern "C" WEAK void __iar_system_Mtxlock(__iar_Rmtx *mutex) {}
extern "C" WEAK void __iar_system_Mtxunlock(__iar_Rmtx *mutex) {}
extern "C" WEAK void __iar_file_Mtxinit(__iar_Rmtx *mutex) {}
extern "C" WEAK void __iar_file_Mtxdst(__iar_Rmtx *mutex) {}
extern "C" WEAK void __iar_file_Mtxlock(__iar_Rmtx *mutex) {}
extern "C" WEAK void __iar_file_Mtxunlock(__iar_Rmtx *mutex) {}
#elif defined(__CC_ARM)
// Do nothing
#elif defined (__GNUC__)
struct _reent;
// Stub out locks when an rtos is not present
extern "C" WEAK void __rtos_malloc_lock( struct _reent *_r ) {}
extern "C" WEAK void __rtos_malloc_unlock( struct _reent *_r ) {}
extern "C" WEAK void __rtos_env_lock( struct _reent *_r ) {}
extern "C" WEAK void __rtos_env_unlock( struct _reent *_r ) {}

extern "C" void __malloc_lock( struct _reent *_r )
{
    __rtos_malloc_lock(_r);
}

extern "C" void __malloc_unlock( struct _reent *_r )
{
    __rtos_malloc_unlock(_r);
}

extern "C" void __env_lock( struct _reent *_r )
{
    __rtos_env_lock(_r);
}

extern "C" void __env_unlock( struct _reent *_r )
{
    __rtos_env_unlock(_r);
}

#define CXA_GUARD_INIT_DONE             (1 << 0)
#define CXA_GUARD_INIT_IN_PROGRESS      (1 << 1)
#define CXA_GUARD_MASK                  (CXA_GUARD_INIT_DONE | CXA_GUARD_INIT_IN_PROGRESS)

extern "C" int __cxa_guard_acquire(int *guard_object_p)
{
    uint8_t *guard_object = (uint8_t *)guard_object_p;
    if (CXA_GUARD_INIT_DONE == (*guard_object & CXA_GUARD_MASK)) {
        return 0;
    }
    singleton_lock();
    if (CXA_GUARD_INIT_DONE == (*guard_object & CXA_GUARD_MASK)) {
        singleton_unlock();
        return 0;
    }
    MBED_ASSERT(0 == (*guard_object & CXA_GUARD_MASK));
    *guard_object = *guard_object | CXA_GUARD_INIT_IN_PROGRESS;
    return 1;
}

extern "C" void __cxa_guard_release(int *guard_object_p)
{
    uint8_t *guard_object = (uint8_t *)guard_object_p;
    MBED_ASSERT(CXA_GUARD_INIT_IN_PROGRESS == (*guard_object & CXA_GUARD_MASK));
    *guard_object = (*guard_object & ~CXA_GUARD_MASK) | CXA_GUARD_INIT_DONE;
    singleton_unlock();
}

extern "C" void __cxa_guard_abort(int *guard_object_p)
{
    uint8_t *guard_object = (uint8_t *)guard_object_p;
    MBED_ASSERT(CXA_GUARD_INIT_IN_PROGRESS == (*guard_object & CXA_GUARD_MASK));
    *guard_object = *guard_object & ~CXA_GUARD_INIT_IN_PROGRESS;
    singleton_unlock();
}

#endif

void *operator new(std::size_t count)
{
    void *buffer = malloc(count);
    if (NULL == buffer) {
        error("Operator new out of memory\r\n");
    }
    return buffer;
}

void *operator new[](std::size_t count)
{
    void *buffer = malloc(count);
    if (NULL == buffer) {
        error("Operator new[] out of memory\r\n");
    }
    return buffer;
}

void operator delete(void *ptr)
{
    if (ptr != NULL) {
        free(ptr);
    }
}
void operator delete[](void *ptr)
{
    if (ptr != NULL) {
        free(ptr);
    }
}
